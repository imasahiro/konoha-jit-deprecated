/***************************************************************************
 *
 *  Copyright (C) 2011 by Masahiro Ide
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 ****************************************************************************/
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/Constants.h"
#include "llvm/GlobalVariable.h"
#include "llvm/Function.h"
#include "llvm/BasicBlock.h"
#include "llvm/Instructions.h"
#include "llvm/Pass.h"
#include "llvm/PassManager.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Support/IRBuilder.h"
#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/ExecutionEngine/Interpreter.h"
#include "llvm/Target/TargetSelect.h"
#include <vector>

#undef HAVE_SYS_TYPES_H
#include "commons.h"
#define ASM_PREFIX llvmasm::
#define LLVM_TODO(str) {\
	fprintf(stderr, "(TODO: %s %d):", __func__, __LINE__);\
	fprintf(stderr, "%s\n", str);\
	asm volatile("int3");\
}
#define LLVM_WARN(str) {\
	fprintf(stderr, "(WARN: %s %d):", __func__, __LINE__);\
	fprintf(stderr, "%s\n", str);\
}
#define _UNUSED_ __attribute__((unused))

/* ************************************************************************ */

namespace llvmasm {
	using namespace llvm;
/* ------------------------------------------------------------------------ */
/* kcode */

#define ASM(X, ...)
#define ASMop(X, ...)
#define ASML(idx) ((idx < DP(ctx->gma)->espidx) ? (DP(ctx->gma)->espidx) : idx)
static void Tn_asm(CTX ctx, knh_Stmt_t *stmt, size_t n, knh_type_t reqt, int local);
static int _BLOCK_asm(CTX ctx, knh_Stmt_t *stmtH, knh_type_t reqt, int sfpidx);

struct llvm_context {
	Module *m;
	Function *curFunc;
	BasicBlock *curBB;
	IRBuilder<> *builder;
};
#define LLVM_IDX_MODULE    (0)
#define LLVM_IDX_FUNCTION  (1)
#define LLVM_IDX_BB        (2)
#define LLVM_IDX_BUILDER   (3)
static inline Module *LLVM_MODULE(CTX ctx)
{
	knh_Array_t *a = DP(ctx->gma)->insts;
	return (Module*)a->ilist[LLVM_IDX_MODULE];
}
static inline Function *LLVM_FUNCTION(CTX ctx)
{
	knh_Array_t *a = DP(ctx->gma)->insts;
	return (Function*)a->ilist[LLVM_IDX_FUNCTION];
}
static inline BasicBlock *LLVM_BB(CTX ctx)
{
	knh_Array_t *a = DP(ctx->gma)->insts;
	return (BasicBlock*)a->ilist[LLVM_IDX_BB];
}
static inline IRBuilder<> *LLVM_BUILDER(CTX ctx)
{
	knh_Array_t *a = DP(ctx->gma)->insts;
	return (IRBuilder<>*)a->ilist[LLVM_IDX_BUILDER];
}
#define PUSH_LABEL(ctx, ...) /*TODO*/
#define POP_LABEL(ctx)       /*TODO*/

#ifdef K_USING_RBP_
#define NC_(sfpidx)    (((sfpidx) * 2) + 1)
#define OC_(sfpidx)    ((sfpidx) * 2)
#define SFP_(sfpidx)   ((sfpidx) * 2)
#else
#define NC_(sfpidx)    sfpidx
#define OC_(sfpidx)    sfpidx
#define SFP_(sfpidx)   sfpidx
#endif

#define RIX_(rix)      rix

/* ------------------------------------------------------------------------ */
/* [Gamma] */

static Value *ValueStack_get(CTX ctx, int index)
{
	knh_Array_t *lstacks = DP(ctx->gma)->lstacks;
	knh_sfp_t lsfp = {};
	index = index + (-1 * K_RTNIDX);
	lsfp.a = lstacks;
	lstacks->api->get(ctx, &lsfp, index, 0);
	return (Value*) lsfp.ndata;
}
#define knh_Array_capacity(a) ((a)->dim->capacity)
static void ValueStack_set(CTX ctx, int index, Value *v)
{
	knh_Array_t *lstacks = DP(ctx->gma)->lstacks;
	knh_sfp_t lsfp = {};
	index = index + (-1 * K_RTNIDX);
	if ((int)knh_Array_capacity(lstacks) < index) {
		knh_Array_grow(ctx, lstacks, index, index);
	}
	lsfp.ndata = (knh_ndata_t) v;
	lstacks->api->set(ctx, lstacks, index, &lsfp);
}

#define TT_isSFPIDX(tk)   (TT_(tk) == TT_LOCAL || TT_(tk) == TT_FUNCVAR)
#define Token_index(tk)   Token_index_(ctx, tk)
static int Token_index_(CTX ctx, knh_Token_t *tk)
{
	return (int)(tk)->index + ((TT_(tk) == TT_LOCAL) ? DP(ctx->gma)->ebpidx : 0);
}

static void ASM_BOX2(CTX ctx, knh_type_t reqt, knh_type_t atype, int a)
{
	knh_class_t cid = CLASS_t(atype);
	knh_class_t bcid = ClassTBL(cid)->bcid;
	if(bcid == CLASS_Boolean || bcid == CLASS_Int || bcid == CLASS_Float) {
		knh_class_t rcid = ClassTBL(CLASS_t(reqt))->bcid;
		if(rcid != bcid && reqt != TYPE_void) {
			if(cid == CLASS_Boolean) {
				//ASM(TR, OC_(a), SFP_(a), RIX_(a-a), ClassTBL(cid), _bBOX);
			}
			else if (IS_Tnumbox(reqt)) {
				LLVM_TODO("Boxing");
				//Object *v = (cid == CLASS_Int) ? UPCAST(KNH_INT0) : UPCAST(KNH_FLOAT0);
				//ASM(OSET, OC_(a), v);
			}
			else {
				ASM(TR, OC_(a), SFP_(a), RIX_(a-a), ClassTBL(cid), _BOX);
			}
		}
	}
	else if(atype == CLASS_Tdynamic && IS_Tnumbox(reqt)) {
		ASM(UNBOX, NC_(a), OC_(a), ClassTBL(reqt));
	}
	else if(IS_Tnumbox(atype) && reqt == CLASS_Tdynamic) {
		ASM(TR, OC_(a), SFP_(a), RIX_(a-a), ClassTBL(atype), _OBOX);
	}
}

static void ASM_UNBOX(CTX ctx, knh_type_t atype, int a)
{
	LLVM_TODO("ASM_UNBOX");
}

static void ASM_MOVL(CTX ctx, knh_type_t reqt, int sfpidx, knh_type_t ltype, int local)
{
	if(sfpidx < DP(ctx->gma)->espidx) {
		Value *v = ValueStack_get(ctx, local);
		ValueStack_set(ctx, sfpidx, v);
	}
}
static int Tn_put(CTX ctx, knh_Stmt_t *stmt, size_t n, knh_type_t reqt, int sfpidx)
{
	knh_Token_t *tk = tkNN(stmt, n);
	if(TT_(tk) == TT_FUNCVAR || TT_(tk) == TT_LOCAL) return Token_index(tk);
	Tn_asm(ctx, stmt, n, reqt, sfpidx);
	return sfpidx;
}

static knh_Token_t* Tn_putTK(CTX ctx, knh_Stmt_t *stmt, size_t n, knh_type_t reqt, int sfpidx)
{
	knh_Token_t *tk = tkNN(stmt, n);
	if(IS_Stmt(tk)) {
		Tn_asm(ctx, stmt, n, reqt, sfpidx);
		DBG_ASSERT(IS_Token(tkNN(stmt, n)));
		return tkNN(stmt, n);
	}
	return tk;
}


static void ASM_SMOVx(CTX ctx, knh_type_t atype, int a, knh_type_t btype, knh_sfx_t bx)
{
	if(IS_Tunbox(btype)) {
		ASM(NMOVx, NC_(a), bx)
		ASM_BOX2(ctx, atype, btype, (a));
	}
	else {
		ASM(OMOVx, OC_(a), bx);
		ASM_UNBOX(ctx, btype, a);
	}
}

#define LLVM_CONTEXT() (llvm::getGlobalContext())
#define LLVMTYPE_Void  (Type::getVoidTy(LLVM_CONTEXT()))
#define LLVMTYPE_Bool  (Type::getInt1Ty(LLVM_CONTEXT()))
#define LLVMTYPE_Int   (Type::getInt64Ty(LLVM_CONTEXT()))
#define LLVMTYPE_Float (Type::getDoubleTy(LLVM_CONTEXT()))
#define LLVMTYPE_Object (LLVM_MODULE(ctx)->getTypeByName("Object"))
static void ASM_SMOV(CTX ctx, knh_type_t atype, int a/*flocal*/, knh_Token_t *tkb)
{
	knh_type_t btype = SP(tkb)->type;
	switch(TT_(tkb)) {
		case TT_ASIS:
			break;
		case TT_NULL/*DEFVAL*/: {
			knh_class_t cid = (tkb)->cid;
			knh_Object_t *o = KNH_NULVAL(cid);
			if(o != ClassTBL(cid)->defnull) {
				ASM(TR, OC_(a), SFP_(a), RIX_(a-a), ClassTBL(cid), _NULVAL);
				break;
			}
			KNH_SETv(ctx, (tkb)->data, o);
			goto L_CONST;
		}
		case TT_CID:
			KNH_SETv(ctx, (tkb)->data, new_Type(ctx, (tkb)->cid));
		case TT_CONST: L_CONST:; {
			Object *o = (tkb)->data;
			if(IS_Tunbox(atype)) {
				Value *v = NULL;
				if (IS_Tint(atype)) {
					v = ConstantInt::get(LLVMTYPE_Int, O_data(o));
				}
				else if (IS_Tfloat(atype)) {
					v = ConstantInt::get(LLVMTYPE_Float, O_data(o));
				}
				else if (IS_Tbool(atype)) {
					v = ConstantInt::get(LLVMTYPE_Bool, O_data(o));
				}
				ValueStack_set(ctx, a, v);
			}
			else if(IS_Tnumbox(atype)) {
				ASM(NSET, NC_(a), O_data((o)));
				ASM(OSET, OC_(a), v);
			}
			else {
				ASM(OSET, OC_(a), v);
			}
			break;
		}
		case TT_FUNCVAR:
		case TT_LOCAL: {
			int b = Token_index(tkb);
			knh_type_t btype = SP(tkb)->type;
			if(IS_Tunbox(btype)) {
				Value *v = ValueStack_get(ctx, b);
				ValueStack_set(ctx, a, v);
			}
			else {
				ASM(OMOV, OC_(a), OC_(b));
				if(IS_Tnumbox(btype)) {
					if(IS_Tnumbox(atype)) {
						ASM(NMOV, NC_(a), NC_(b));
					}
					else {
						ASM(TR, OC_(a), SFP_(b), RIX_(a-b), ClassTBL(atype), _OBOX);
					}
				}
			}
			break;
		}
		case TT_FIELD: {
			int b = (int)(tkb)->index;
			knh_sfx_t bx = {OC_(0), (size_t)b};
			if(IS_Token(tkb->token) && TT_isSFPIDX(tkb->token)) {
				bx.i = OC_(Token_index(tkb->token));
			}
			ASM_SMOVx(ctx, atype, a, btype, bx);
			break;
		}
		case TT_SCRFIELD: {
			int b = (int)(tkb)->index;
			knh_sfx_t bx = {OC_(DP(ctx->gma)->scridx), (size_t)b};
			ASM_SMOVx(ctx, atype, a, btype, bx);
			break;
		}
		case TT_SYSVAL: {
			size_t sysid = (tkb)->index;
			KNH_ASSERT(sysid < K_SYSVAL_MAX);
			ASM(FASTCALL0, SFP_(a), SFP_(a), 0, SYSVAL_MTD[sysid]);
			break;
		}
		case TT_PROPN: {
			ASM(OSET, OC_(a), (tkb)->data);
			if(IS_Tunbox(btype)) {
				ASM(TR, NC_(a), SFP_(a), RIX_(a-a), ClassTBL(CLASS_t(btype)), _NPROP);
			}
			else {
				ASM(TR, OC_(a), SFP_(a), RIX_(a-a), ClassTBL(CLASS_t(btype)), _PROP);
			}
			ASM_BOX2(ctx, atype, btype, a);
			break;
		}
		default: {
			DBG_ABORT("unknown TT=%s", TT__(TT_(tkb)));
		}
	}/*switch*/
}

static void ASM_XMOVx(CTX ctx, knh_type_t atype, knh_sfx_t ax, knh_type_t btype, knh_sfx_t bx)
{
	int espidx = DP(ctx->gma)->espidx;
	if(IS_Tunbox(atype)) {
		if(IS_Tunbox(btype)) {
			ASM(XNMOVx, ax, bx);
		}
		else {
			ASM(OMOVx, NC_(espidx), bx);
			ASM_UNBOX(ctx, atype, espidx);
			ASM(XNMOV, ax, NC_(espidx));
		}
	}
	else {
		DBG_ASSERT(atype == TYPE_dyn || IS_Tnumbox(atype));
		if(IS_Tunbox(btype)) { // dynamic a = b; // int b;
			ASM(NMOVx, NC_(espidx), bx);
			ASM(TR, OC_(espidx), SFP_(espidx), RIX_(espidx-espidx), ClassTBL(CLASS_t(btype)), _BOX);
			ASM(XMOV, ax, OC_(espidx));
		}
		else {
			ASM(XMOVx, ax, bx);
		}
	}
}

static void ASM_XMOV(CTX ctx, knh_type_t atype, int a, size_t an, knh_Token_t *tkb)
{
	knh_sfx_t ax = {OC_(a), an};
	knh_type_t btype = SP(tkb)->type;
	long espidx = -1;
	DBG_ASSERT(DP(ctx->gma)->espidx != -1);
	switch(TT_(tkb)) {
		case TT_NULL/*DEFVAL*/: {
			knh_class_t cid = (tkb)->cid;
			knh_Object_t *v = KNH_NULVAL(cid);
			if(v != ClassTBL(cid)->defnull) {
				espidx = DP(ctx->gma)->espidx;
				ASM(TR, OC_(espidx), SFP_(espidx), RIX_(espidx-espidx), ClassTBL(cid), _NULVAL);
				break;
			}
			KNH_SETv(ctx, (tkb)->data, v);
			goto L_CONST;
		}
		case TT_CID:
			KNH_SETv(ctx, (tkb)->data, new_Type(ctx, (tkb)->cid));
		case TT_CONST: L_CONST:; {
			Object *v = (tkb)->data;
			if(IS_Tunbox(atype)) {
				ASM(XNSET, ax, O_data(v));
			}
			else {
				ASM(XOSET, ax, v);
			}
			break;
		}
		case TT_FUNCVAR:
		case TT_LOCAL: {
			int b = Token_index(tkb);
			ASM_BOX2(ctx, atype, btype, b);
			if(IS_Tunbox(atype)) {
				ASM(XNMOV, ax, NC_(b));
			}
			else {
				ASM(XMOV, ax, OC_(b));
			}
			break;
		}
		case TT_FIELD: {
			int b = (int)(tkb)->index;
			knh_sfx_t bx = {OC_(0), (size_t)b};
			if(IS_Token(tkb->token) && TT_isSFPIDX(tkb->token)) {
				bx.i = OC_(Token_index(tkb->token));
			}
			ASM_XMOVx(ctx, atype, ax, btype, bx);
			break;
		}
		case TT_SCRFIELD: {
			int b = (int)(tkb)->index;
			knh_sfx_t bx = {OC_(DP(ctx->gma)->scridx), (size_t)b};
			ASM_XMOVx(ctx, atype, ax, btype, bx);
			break;
		}
		case TT_SYSVAL: {
			size_t sysid = (tkb)->index;
			KNH_ASSERT(sysid < K_SYSVAL_MAX);
			espidx = DP(ctx->gma)->espidx;
			ASM(FASTCALL0, SFP_(espidx), SFP_(espidx), 0, SYSVAL_MTD[sysid]);
			break;
		}
		case TT_PROPN: {
			espidx = DP(ctx->gma)->espidx;
			ASM(OSET, OC_(espidx), (tkb)->data);
			if(IS_Tunbox(atype)) {
				ASM(TR, NC_(espidx), SFP_(espidx), RIX_(espidx-espidx), ClassTBL(CLASS_t(atype)), _NPROP);
			}
			else {
				ASM(TR, OC_(espidx), SFP_(espidx), RIX_(espidx-espidx), ClassTBL(CLASS_t(atype)), _PROP);
			}
			ASM_BOX2(ctx, atype, btype, espidx);
			break;
		}
		default: {
			DBG_ABORT("unknown TT=%d", TT_(tkb));
		}
	}/*switch*/
	if(espidx != -1) {
		if(IS_Tunbox(atype)) {
			ASM(XNMOV, ax, NC_(espidx));
		}
		else {
			ASM(XMOV, ax, OC_(espidx));
		}
	}
}

static void ASM_MOV(CTX ctx, knh_Token_t *tka, knh_Token_t *tkb)
{
	DBG_ASSERT(Token_isTyped(tka));
	DBG_ASSERT(IS_Token(tkb));
	DBG_ASSERT(Token_isTyped(tkb));
	knh_type_t atype = SP(tka)->type;
	if(TT_(tka) == TT_LOCAL || TT_(tka) == TT_FUNCVAR) {
		ASM_SMOV(ctx, atype, Token_index(tka), tkb);
	}
	else {
		DBG_ASSERT(TT_(tka) == TT_FIELD);
		int an = (int)(tka)->index;
		ASM_XMOV(ctx, atype, 0, an, tkb);
	}
}

/* ------------------------------------------------------------------------ */
/* [EXPR] */

static knh_int_t Tn_int(knh_Stmt_t *stmt, size_t n)
{
	knh_Token_t *tk = tkNN(stmt, n);
	DBG_ASSERT(TT_(tk) == TT_CONST);
	return ((tk)->num)->n.ivalue;
}

static knh_float_t Tn_float(knh_Stmt_t *stmt, size_t n)
{
	knh_Token_t *tk = tkNN(stmt, n);
	DBG_ASSERT(TT_(tk) == TT_CONST);
	return ((tk)->num)->n.fvalue;
}

static int _CALL_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx);
static knh_opcode_t OPCODE_imn(knh_methodn_t mn, int diff)
{
	switch(mn) {
	case MN_opNEG: return OPCODE_iNEG;
	case MN_opADD: return OPCODE_iADD + diff;
	case MN_opSUB: return OPCODE_iSUB + diff;
	case MN_opMUL: return OPCODE_iMUL + diff;
	case MN_opDIV: return OPCODE_iDIV+ diff;
	case MN_opMOD: return OPCODE_iMOD+ diff;
	case MN_opEQ:  return OPCODE_iEQ+ diff;
	case MN_opNOTEQ: return OPCODE_iNEQ+ diff;
	case MN_opLT:  return OPCODE_iLT+ diff;
	case MN_opLTE: return OPCODE_iLTE+ diff;
	case MN_opGT:  return OPCODE_iGT+ diff;
	case MN_opGTE: return OPCODE_iGTE+ diff;
#ifdef OPCODE_iAND
	case MN_opLAND :  return OPCODE_iAND  + diff;
	case MN_opLOR  :  return OPCODE_iOR   + diff;
	case MN_opLXOR :  return OPCODE_iXOR  + diff;
	case MN_opLSFT:  return OPCODE_iLSFT  + diff;
	case MN_opRSFT:  return OPCODE_iRSFT  + diff;
#endif
	}
	return OPCODE_NOP;
}

static knh_opcode_t OPCODE_fmn(knh_methodn_t mn, int diff)
{
	switch(mn) {
	case MN_opNEG: return OPCODE_fNEG;
	case MN_opADD: return OPCODE_fADD + diff;
	case MN_opSUB: return OPCODE_fSUB + diff;
	case MN_opMUL: return OPCODE_fMUL + diff;
	case MN_opDIV: return OPCODE_fDIV + diff;
	case MN_opEQ:  return OPCODE_fEQ + diff;
	case MN_opNOTEQ: return OPCODE_fNEQ + diff;
	case MN_opLT:  return OPCODE_fLT + diff;
	case MN_opLTE: return OPCODE_fLTE + diff;
	case MN_opGT:  return OPCODE_fGT + diff;
	case MN_opGTE: return OPCODE_fGTE + diff;
	}
	return OPCODE_NOP;
}

static knh_bool_t OPR_hasCONST(CTX ctx, knh_Stmt_t *stmt, knh_methodn_t *mn, int swap)
{
	int isCONST = (TT_(tmNN(stmt, 2)) == TT_CONST);
	if(swap == 1 && TT_(tmNN(stmt, 1)) == TT_CONST) {
		knh_methodn_t newmn = *mn;
		knh_Stmt_swap(ctx, stmt, 1, 2);
		if(*mn == MN_opLT) newmn = MN_opGT;  /* 1 < n ==> n > 1 */
		else if(*mn == MN_opLTE) newmn = MN_opGTE; /* 1 <= n => n >= 1 */
		else if(*mn == MN_opGT) newmn = MN_opLT;
		else if(*mn == MN_opGTE) newmn = MN_opLTE;
		//DBG_P("swap %s ==> %s", MN__(*mn), MN__(newmn));
		*mn = newmn;
		isCONST = 1;
	}
	return isCONST;
}

static int ASMiop(CTX ctx, knh_opcode_t opcode, Value *va, Value *vb, int local)
{
#define VSET(ctx, local, create) ValueStack_set(ctx, local, LLVM_BUILDER(ctx)->create)
	switch(opcode) {
	case OPCODE_iNEG:  VSET(ctx, local, CreateNeg(va, "neg"));break;
	case OPCODE_iADD:  VSET(ctx, local, CreateAdd(va, vb, "add"));break;
	case OPCODE_iSUB:  VSET(ctx, local, CreateSub(va, vb, "sub"));break;
	case OPCODE_iMUL:  VSET(ctx, local, CreateMul(va, vb, "mul"));break;
	case OPCODE_iDIV:  VSET(ctx, local, CreateSDiv(va, vb, "div"));break;
	case OPCODE_iMOD:  VSET(ctx, local, CreateSRem(va, vb, "mod"));break;
	case OPCODE_iEQ :  VSET(ctx, local, CreateICmpEQ(va, vb, "eq"));break;
	case OPCODE_iNEQ:  VSET(ctx, local, CreateICmpNE(va, vb, "ne"));break;
	case OPCODE_iLT :  VSET(ctx, local, CreateICmpSLT(va, vb, "lt"));break;
	case OPCODE_iLTE:  VSET(ctx, local, CreateICmpSLE(va, vb, "le"));break;
	case OPCODE_iGT :  VSET(ctx, local, CreateICmpSGT(va, vb, "gt"));break;
	case OPCODE_iGTE:  VSET(ctx, local, CreateICmpSGE(va, vb, "ge"));break;
#ifdef OPCODE_iAND
	case OPCODE_iAND : VSET(ctx, local, CreateAnd(va, vb, "and"));break;
	case OPCODE_iOR  : VSET(ctx, local, CreateOr(va, vb, "or"));break;
	case OPCODE_iXOR : VSET(ctx, local, CreateXor(va, vb, "xor"));break;
	case OPCODE_iLSFT: VSET(ctx, local, CreateLShr(va, vb, "lshr"));break;
	case OPCODE_iRSFT: VSET(ctx, local, CreateAShr(va, vb, "rshr"));break;
#endif
#undef VSET
	}
	return 1;
}
static int ASMfop(CTX ctx, knh_opcode_t opcode, Value *va, Value *vb, int local)
{
#define VSET(ctx, local, create) ValueStack_set(ctx, local, LLVM_BUILDER(ctx)->create)
	switch(opcode) {
	case OPCODE_fNEG:  VSET(ctx, local, CreateFNeg   (va, "neg"));break;
	case OPCODE_fADD:  VSET(ctx, local, CreateFAdd   (va, vb, "add"));break;
	case OPCODE_fSUB:  VSET(ctx, local, CreateFSub   (va, vb, "sub"));break;
	case OPCODE_fMUL:  VSET(ctx, local, CreateFMul   (va, vb, "mul"));break;
	case OPCODE_fDIV:  VSET(ctx, local, CreateFDiv   (va, vb, "div"));break;
	case OPCODE_fEQ :  VSET(ctx, local, CreateFCmpOEQ(va, vb, "eq"));break;
	case OPCODE_fNEQ:  VSET(ctx, local, CreateFCmpONE(va, vb, "ne"));break;
	case OPCODE_fLT :  VSET(ctx, local, CreateFCmpOLT(va, vb, "lt"));break;
	case OPCODE_fLTE:  VSET(ctx, local, CreateFCmpOLE(va, vb, "le"));break;
	case OPCODE_fGT :  VSET(ctx, local, CreateFCmpOGT(va, vb, "gt"));break;
	case OPCODE_fGTE:  VSET(ctx, local, CreateFCmpOGE(va, vb, "ge"));break;
#undef VSET
	}
	return 1;
}
static int _OPR_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	int local = ASML(sfpidx);
	knh_Method_t *mtd = (tkNN(stmt, 0))->mtd;
	if (IS_NULL(mtd)) return 0;
	knh_methodn_t mn = (mtd)->mn;
	knh_class_t cid = CLASS_t(SP(tkNN(stmt, 1))->type);
	knh_opcode_t opcode;
	if(cid == CLASS_Boolean && mn == MN_opNOT) {
		int a = Tn_put(ctx, stmt, 1, TYPE_Boolean, local + 1);
		Value *va = ValueStack_get(ctx, a);
		va = LLVM_BUILDER(ctx)->CreateNot(va, "not");
		ValueStack_set(ctx, sfpidx, va);
		return 1;
	}
	if (cid == CLASS_Int && ((opcode = OPCODE_imn(mn, 0)) != OPCODE_NOP)) {
		int swap = 1;
		if (mn == MN_opNEG) {
			int a = Tn_put(ctx, stmt, 1, TYPE_Int, local + 1);
			Value *va = ValueStack_get(ctx, a);
			ASMiop(ctx, OPCODE_iNEG, va, NULL, sfpidx);
			return 1;
		}
		if (mn == MN_opSUB || mn == MN_opDIV || mn == MN_opMOD) swap =0;
		if (OPR_hasCONST(ctx, stmt, &mn, swap)) {
			int a = Tn_put(ctx, stmt, 1, TYPE_Int, local + 1);
			knh_int_t b = Tn_int(stmt, 2);
			if (b == 0 && mn == MN_opDIV) {
				b = 1;
				WarningDividedByZero(ctx);
			}
			Value *va = ValueStack_get(ctx, a);
			Value *vb = ConstantInt::get(LLVMTYPE_Int, b);
			ASMiop(ctx, opcode, va, vb, sfpidx);
		}
		else {
			int a = Tn_put(ctx, stmt, 1, TYPE_Int, local + 1);
			int b = Tn_put(ctx, stmt, 2, TYPE_Int, local + 2);
			Value *va = ValueStack_get(ctx, a);
			Value *vb = ValueStack_get(ctx, b);
			ASMiop(ctx, opcode, va, vb, sfpidx);
		}
	}
	if (cid == CLASS_Float && ((opcode = OPCODE_fmn(mn, 0)) != OPCODE_NOP)) {
		int swap = 1;
		if (mn == MN_opNEG) {
			int a = Tn_put(ctx, stmt, 1, TYPE_Float, local + 1);
			Value *va = ValueStack_get(ctx, a);
			ASMfop(ctx, OPCODE_fNEG, va, NULL, sfpidx);
			return 1;
		}
		if (mn == MN_opSUB || mn == MN_opDIV || mn == MN_opMOD) swap =0;
		if (OPR_hasCONST(ctx, stmt, &mn, swap)) {
			int a = Tn_put(ctx, stmt, 1, TYPE_Float, local + 1);
			knh_float_t b = Tn_float(stmt, 2);
			if (b == K_FLOAT_ZERO && mn == MN_opDIV) {
				b = K_FLOAT_ONE;
				WarningDividedByZero(ctx);
			}
			opcode = OPCODE_fmn(mn, (OPCODE_fADDC - OPCODE_fADD));
			Value *va = ValueStack_get(ctx, a);
			Value *vb = ConstantFP::get(LLVMTYPE_Float, b);
			ASMfop(ctx, opcode, va, vb, sfpidx);
		}
		else {
			int a = Tn_put(ctx, stmt, 1, TYPE_Float, local + 1);
			int b = Tn_put(ctx, stmt, 2, TYPE_Float, local + 2);
			Value *va = ValueStack_get(ctx, a);
			Value *vb = ValueStack_get(ctx, b);
			ASMfop(ctx, opcode, va, vb, sfpidx);
		}
	}
	return 0;
}

/* ------------------------------------------------------------------------ */
/* CALL */

static knh_type_t Tn_ptype(CTX ctx, knh_Stmt_t *stmt, size_t n, knh_class_t cid, knh_Method_t *mtd)
{
	LLVM_TODO("Tn_ptype");
	return TYPE_void;
}

static int CALLPARAMs_asm(CTX ctx, knh_Stmt_t *stmt, size_t s, int local, knh_class_t cid, knh_Method_t *mtd)
{
	size_t i;
	if(s == 1 && Method_isStatic(mtd))
		// ignoring static caller, like Script
		s = 2;
	if(DP(stmt)->size + DP(ctx->gma)->espidx > 32) {
		LLVM_TODO("check stack");
	}
	for(i = s; i < DP(stmt)->size; i++) {
		knh_type_t reqt = Tn_ptype(ctx, stmt, i, cid, mtd);
		Tn_asm(ctx, stmt, i, reqt, local + i + (K_CALLDELTA-1));
	}
	return 1;
}

static int _CALL_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	LLVM_TODO("CALL");
	return 0;
}

static int _FUNCCALL_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	LLVM_TODO("FUNCCALL");
	return 0;
}

static int _EXPR_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx);

static int _CALL1_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	if(IS_Stmt(DP(stmt)->stmtPOST)) {  /* a++ */
		int local = ASML(sfpidx);
		Tn_asm(ctx, stmt, 0, reqt, local);
		_EXPR_asm(ctx, DP(stmt)->stmtPOST, reqt, local+1);
		ASM_MOVL(ctx, reqt, sfpidx, SP(stmt)->type, local);
	}
	else {
		Tn_asm(ctx, stmt, 0, reqt, sfpidx);
	}
	return 0;
}

static int _NEW_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	//int local = ASML(sfpidx), thisidx = local + K_CALLDELTA;
	//knh_Method_t *mtd = (tkNN(stmt, 0))->mtd;
	//knh_class_t cid = (tkNN(stmt, 1))->cid;
	//if(DP(stmt)->size == 2 && (mtd)->cid == CLASS_Object && (mtd)->mn == MN_new) {
	//	ASM(TR, OC_(sfpidx), SFP_(thisidx), RIX_(sfpidx-thisidx), ClassTBL(cid), TR_NEW);
	//}
	//else {
	//	ASM(TR, OC_(thisidx), SFP_(thisidx), RIX_(thisidx-thisidx), ClassTBL(cid), TR_NEW);
	//	CALLPARAMs_asm(ctx, stmt, 2, local, cid, mtd);
	//	//ASM_CALL(ctx, reqt, local, cid, mtd, 1, DP(stmt)->size - 2);
	//	ASM_MOVL(ctx, cid, sfpidx, SP(stmt)->type, local);
	//}
	LLVM_TODO("NEW");
	return 0;
}

static int _TCAST_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	knh_type_t srct = Tn_type(stmt, 1);
	if(srct != reqt) {
		int local = ASML(sfpidx);
		knh_Token_t *tkC = tkNN(stmt, 0);
		knh_TypeMap_t *trl = (tkC)->mpr;
		Tn_asm(ctx, stmt, 1, srct, local);
		if(IS_TypeMap(trl)) {
			knh_class_t scid = SP(trl)->scid, tcid = SP(trl)->tcid;
			if(1/*TypeMap_isFinal(trl)*/) {
				if(scid == CLASS_Int && tcid == CLASS_Float) {
					Value *v = ValueStack_get(ctx, local);
					v = LLVM_BUILDER(ctx)->CreateSIToFP(v, LLVMTYPE_Float, "fcast");
					ValueStack_set(ctx, local, v);
				}
				else if(scid == CLASS_Float && tcid == CLASS_Int) {
					Value *v = ValueStack_get(ctx, local);
					v = LLVM_BUILDER(ctx)->CreateSIToFP(v, LLVMTYPE_Int, "icast");
					ValueStack_set(ctx, local, v);
				}
				else {
					LLVM_TODO("SCAST");
					ASM_BOX2(ctx, TYPE_Object, srct, local);
					ASM(SCAST, RTNIDX_(ctx, local, stmt->type), SFP_(local), RIX_(local-local), trl);
				}
			}
			else {
				LLVM_TODO("!TypeMap_isFinal(trl)");
			}
		}
		else {
			LLVM_TODO("ACAST");
		}
		ASM_MOVL(ctx, reqt, sfpidx, SP(stmt)->type, local);
	}
	return 0;
}

static int _ALT_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	DBG_ABORT("TODO: ALT");
	return 0;
}

static int _OR_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	LLVM_TODO("x OR y");
	return 0;
}

static int _AND_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	LLVM_TODO("x AND y");
	return 0;
}

static int _TRI_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	LLVM_TODO("TRI (x)?y:z");
	return 0;
}

static int _LET_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	knh_Token_t *tkL = tkNN(stmt, 1);
	if(TT_(tkL) == TT_LOCAL || TT_(tkL) == TT_FUNCVAR) {
		int index = Token_index(tkL);
		Tn_asm(ctx, stmt, 2, SP(tkL)->type, index);
	}
	else if(IS_Token(tkNN(stmt, 2))) {
		ASM_MOV(ctx, tkL, tkNN(stmt, 2));
	}
	else {
		int local = ASML(sfpidx);
		knh_term_t tt = TT_(tkL); /* NOTE: tkL is reused inside stmt */
		knh_short_t index = (tkL)->index;
		Tn_asm(ctx, stmt, 2, SP(tkL)->type, local);
		TT_(tkL) = tt;
		(tkL)->index = index;
		ASM_MOV(ctx, tkL, tkNN(stmt, 2));
		if(local == sfpidx)
			// we do not need to let variable.
			return 0;
	}
	if(stmt->type != TYPE_void && reqt != TYPE_void) {
		ASM_SMOV(ctx, reqt, sfpidx, tkL);
	}

	return 0;
}

static METHOD Fmethod_empty(CTX ctx, knh_sfp_t *sfp _RIX) {}

static knh_Method_t* Gamma_getFmt(CTX ctx, knh_class_t cid, knh_methodn_t mn0)
{
	knh_methodn_t mn = mn0;
	knh_NameSpace_t *ns = K_GMANS;
	knh_Method_t *mtd = knh_NameSpace_getFmtNULL(ctx, ns, cid, mn);
	if(mtd == NULL) {
		WarningUndefinedFmt(ctx, cid, mn0);
		mtd = new_Method(ctx, 0, cid, mn0, Fmethod_empty);
		KNH_SETv(ctx, DP(mtd)->mp, KNH_TNULL(ParamArray));
		knh_NameSpace_addFmt(ctx, ns, mtd);
	}
	return mtd;
}

static int _W1_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	LLVM_TODO("W1");
	return 0;
}

static int _FMTCALL_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	LLVM_TODO("FMTCALL");
	return 0;
}

static int _SEND_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	LLVM_TODO("SEND");
	return 0;
}

/* ------------------------------------------------------------------------ */

#define CASE_ASM(XX, ...) case STT_##XX : { \
		_##XX##_asm(ctx, stmt, ## __VA_ARGS__); \
		break;\
	}\

static int _EXPR_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	switch(STT_(stmt)) {
	CASE_ASM(LET, reqt, sfpidx);
	CASE_ASM(FUNCCALL, reqt, sfpidx);
	CASE_ASM(CALL, reqt, sfpidx);
	CASE_ASM(CALL1, reqt, sfpidx);
	CASE_ASM(OPR, reqt, sfpidx);
	CASE_ASM(NEW, reqt, sfpidx);
	CASE_ASM(TCAST, reqt, sfpidx);
	CASE_ASM(AND, reqt, sfpidx);
	CASE_ASM(OR, reqt, sfpidx);
	CASE_ASM(ALT, reqt, sfpidx);
	CASE_ASM(TRI, reqt, sfpidx);
	CASE_ASM(SEND, reqt, sfpidx);
	CASE_ASM(W1, reqt, sfpidx);
	CASE_ASM(FMTCALL, reqt, sfpidx);
	default:
		DBG_ABORT("unknown stt=%s", TT__(STT_(stmt)));
	}
	ASM_BOX2(ctx, reqt, SP(stmt)->type, sfpidx);
	return 0;
}

static void Tn_asm(CTX ctx, knh_Stmt_t *stmt, size_t n, knh_type_t reqt, int local)
{
	if(IS_Token(tkNN(stmt, n))) {
		knh_Token_t *tk = tkNN(stmt, n);
		ASM_SMOV(ctx, reqt, local, tk);
	}
	else {
		_EXPR_asm(ctx, stmtNN(stmt, n), reqt, local);
	}
	if(IS_Token(tkNN(stmt, n))) {
		knh_Token_t *tk = tkNN(stmt, n);
		//DBG_P("@START %s tk=%p tk->index=%d local=%d esp=%d", TT__(tk->tt), tk, tk->index, local, DP(ctx->gma)->espidx);
		if(TT_(tk) == TT_LOCAL) {
			knh_Token_toTYPED(ctx, tk, TT_FUNCVAR, tk->type, tk->index + DP(ctx->gma)->ebpidx);
		}
		if(TT_(tk) != TT_FUNCVAR) {
			knh_Token_toTYPED(ctx, tk, TT_FUNCVAR, reqt, local);
		}
		//DBG_P("@END %s tk=%p tk->index=%d local=%d esp=%d", TT__(tk->tt), tk, tk->index, local, DP(ctx->gma)->espidx);
	}
	else {
		knh_Token_t *tk = new_TokenTYPED(ctx, TT_FUNCVAR, reqt, local);
		KNH_SETv(ctx, tkNN(stmt, n), tk);
	}
	DBG_ASSERT(IS_Token(tkNN(stmt, n)));
}

/* ------------------------------------------------------------------------ */
/* [IF, WHILE, DO, FOR, FOREACH]  */

static int Tn_CondAsm(CTX ctx, knh_Stmt_t *stmt, size_t n, int isTRUE, int flocal)
{
	knh_Token_t *tk = tkNN(stmt, n);
	if(TT_(tk) == TT_CONST) {
		int isTRUE2 = IS_TRUE((tk)->data);
		ASM_SMOV(ctx, TYPE_Boolean, flocal, tk);
		return flocal;
	}
	if(TT_(tk) == TT_LOCAL || TT_(tk) == TT_FUNCVAR) {
		int index = Token_index(tk);
		if(isTRUE) {
			ASM(bNOT, NC_(index), NC_(index));
		}
		return index;
	}
	else {
		Tn_asm(ctx, stmt, n, TYPE_Boolean, flocal);
		if(isTRUE) {
			ASM(bNOT, NC_(flocal), NC_(flocal));
		}
		return flocal;
	}
}

static knh_Token_t *Tn_it(knh_Stmt_t *stmt, size_t n)
{
	DBG_ASSERT(n < DP(stmt)->size);
	knh_Token_t *tkIT = tkNN(stmt, n);
	DBG_ASSERT(TT_(tkIT) == TT_LOCAL);
	return tkIT;
}

static inline void Tn_asmBLOCK(CTX ctx, knh_Stmt_t *stmt, size_t n, knh_type_t reqt)
{
	DBG_ASSERT(IS_Stmt(stmtNN(stmt, n)));
	_BLOCK_asm(ctx, stmtNN(stmt, n), reqt, 0);
}

static int _IF_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx _UNUSED_)
{
	Value *cond = NULL;//expr
	int local = DP(ctx->gma)->espidx;
	Module *m = LLVM_MODULE(ctx);
	IRBuilder<> *builder = LLVM_BUILDER(ctx);
	BasicBlock *bbThen  = BasicBlock::Create(m->getContext(), "then", LLVM_FUNCTION(ctx));
	BasicBlock *bbElse  = BasicBlock::Create(m->getContext(), "else", LLVM_FUNCTION(ctx));
	BasicBlock *bbMerge = BasicBlock::Create(m->getContext(), "ifcont", LLVM_FUNCTION(ctx));

	//Tn_JMPIF(ctx, stmt, 0, 0/*FALSE*/, ElseBB, local);
	int a = Tn_CondAsm(ctx, stmt, 0, 0, local);
	cond = ValueStack_get(ctx, a);
	builder->CreateCondBr(cond, bbThen, bbElse);

	builder->SetInsertPoint(bbThen);
	Tn_asmBLOCK(ctx, stmt, 1, reqt);
	builder->CreateBr(bbMerge);

	builder->SetInsertPoint(bbElse);
	Tn_asmBLOCK(ctx, stmt, 2, reqt);
	builder->CreateBr(bbMerge);
	builder->SetInsertPoint(bbMerge);
	return 0;
}

static int _SWITCH_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	knh_Stmt_t *stmtCASE;
	Value *cond = NULL;
	Module *m = LLVM_MODULE(ctx);
	IRBuilder<> *builder = LLVM_BUILDER(ctx);
	//BasicBlock *bbContinue = BasicBlock::Create(m->getContext(), "continue");
	BasicBlock *bbBreak    = BasicBlock::Create(m->getContext(), "break");
	BasicBlock *bbCase     = BasicBlock::Create(m->getContext(), "case");
	BasicBlock *bbDefault  = BasicBlock::Create(m->getContext(), "default");

	knh_Token_t *tkIT = Tn_it(stmt, 2);
	PUSH_LABEL(ctx, stmt, bbContinue, bbBreak);
	Tn_asm(ctx, stmt, 0, SP(tkIT)->type, Token_index(tkIT));
	cond = ValueStack_get(ctx, Token_index(tkIT));

	SwitchInst *SwInst = builder->CreateSwitch(cond, bbDefault, 10);
	// 10 is a hint for the number of cases

	stmtCASE = stmtNN(stmt, 1);
	while (stmtCASE != NULL) {
		if (STT_(stmtCASE) == STT_CASE && !Tn_isASIS(stmtCASE, 0)) {
			LLVM_TODO("switch-case");
			DP(ctx->gma)->espidx = DP(stmtCASE)->espidx + DP(ctx->gma)->ebpidx;
			builder->SetInsertPoint(bbCase);
			SwInst->addCase((ConstantInt*)cond, bbCase);
			Tn_asmBLOCK(ctx, stmtCASE, 1, reqt);
			bbCase = BasicBlock::Create(m->getContext(), "case");
			builder->CreateBr(bbCase);
		}
		stmtCASE = DP(stmtCASE)->nextNULL;
	}
	builder->SetInsertPoint(bbBreak);
	POP_LABEL(ctx);
	return 0;
}

static void ASM_JUMPLABEL(CTX ctx, knh_Stmt_t *stmt, int delta)
{
	LLVM_TODO("JUMPLABEL");
}

static int _CONTINUE_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt _UNUSED_, int sfpidx _UNUSED_)
{
	LLVM_TODO("CONTINUE");
	return 0;
}

static int _BREAK_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt _UNUSED_, int sfpidx _UNUSED_)
{
	LLVM_TODO("BREAK");
	return 0;
}

static int _WHILE_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt _UNUSED_, int sfpidx _UNUSED_)
{
	int local = DP(ctx->gma)->espidx;
	Value *cond;
	Module *m = LLVM_MODULE(ctx);
	IRBuilder<> *builder = LLVM_BUILDER(ctx);
	BasicBlock *bbContinue = BasicBlock::Create(m->getContext());
	BasicBlock *bbBreak    = BasicBlock::Create(m->getContext());
	BasicBlock *bbBlock    = BasicBlock::Create(m->getContext());
	PUSH_LABEL(ctx, stmt, bbContinue, bbBreak);
	builder->SetInsertPoint(bbContinue);
	if (!Tn_isTRUE(stmt, 0)) {
		LLVM_TODO("while loop");
		_EXPR_asm(ctx, stmt, 0, local);
	}
	cond = ValueStack_get(ctx, local);
	builder->CreateCondBr(cond, bbBlock, bbBreak);
	builder->SetInsertPoint(bbBlock);
	Tn_asmBLOCK(ctx, stmt, 1, TYPE_void);
	builder->CreateBr(bbContinue);
	builder->SetInsertPoint(bbBreak);
	POP_LABEL(ctx);
	return 0;
}

static int _DO_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt _UNUSED_, int sfpidx _UNUSED_)
{
	Value *cond = NULL;//expr
	int local = DP(ctx->gma)->espidx;
	Module *m = LLVM_MODULE(ctx);
	IRBuilder<> *builder = LLVM_BUILDER(ctx);
	BasicBlock *bbContinue = BasicBlock::Create(m->getContext());
	BasicBlock *bbBreak    = BasicBlock::Create(m->getContext());
	PUSH_LABEL(ctx, stmt, bbContinue, bbBreak);

	builder->SetInsertPoint(bbContinue);
	/* body */
	Tn_asmBLOCK(ctx, stmt, 0, TYPE_void);

	/* cond */
	_EXPR_asm(ctx, stmt, 1, local);
	cond = ValueStack_get(ctx, local);

	builder->CreateCondBr(cond, bbContinue, bbBreak);
	builder->SetInsertPoint(bbBreak);
	POP_LABEL(ctx);

	return 0;
}

static int _FOR_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt _UNUSED_, int sfpidx _UNUSED_)
{
	Value *cond = NULL;//expr
	int local = DP(ctx->gma)->espidx;
	Module *m = LLVM_MODULE(ctx);
	IRBuilder<> *builder = LLVM_BUILDER(ctx);
	BasicBlock *bbContinue = BasicBlock::Create(m->getContext());
	BasicBlock *bbBreak = BasicBlock::Create(m->getContext());
	BasicBlock *bbRedo = BasicBlock::Create(m->getContext());
	BasicBlock *bbBlock = BasicBlock::Create(m->getContext());
	PUSH_LABEL(ctx, stmt, bbContinue, bbBreak);
	// i = 1 part
	Tn_asmBLOCK(ctx, stmt, 0, TYPE_void);
	builder->CreateBr(bbRedo);
	// i++ part
	builder->SetInsertPoint(bbContinue);
	Tn_asmBLOCK(ctx, stmt, 1, TYPE_void);
	builder->CreateBr(bbRedo);
	// i < 10 part
	builder->SetInsertPoint(bbRedo);
	_EXPR_asm(ctx, stmt, 2, local);
	cond = ValueStack_get(ctx, local);
	builder->CreateCondBr(cond, bbBlock, bbBreak);
	// block
	builder->SetInsertPoint(bbBlock);
	Tn_asmBLOCK(ctx, stmt, 3, TYPE_void);
	builder->CreateBr(bbContinue);

	builder->SetInsertPoint(bbBreak);
	POP_LABEL(ctx);

	return 0;
}

static int _FOREACH_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt _UNUSED_, int sfpidx _UNUSED_)
{
	LLVM_TODO("FOREACH");
	return 0;
}

/* ------------------------------------------------------------------------ */
/* [TRY] */

#define Gamma_inTry(ctx)  IS_Stmt(DP(ctx->gma)->finallyStmt)

static void Gamma_setFINALLY(CTX ctx, knh_Stmt_t *stmt)
{
	if(IS_NOTNULL(stmt)) {
		if(IS_NOTNULL(DP(ctx->gma)->finallyStmt)) {
			ErrorMisplaced(ctx);
			KNH_HINT(ctx, "try"); // not nested try
			return;
		}
		KNH_SETv(ctx, DP(ctx->gma)->finallyStmt, stmt);
	}
	else { /* stmt == null */
		KNH_SETv(ctx, DP(ctx->gma)->finallyStmt, stmt);
	}
}

static void ASM_FINALLY(CTX ctx)
{
	if(IS_NOTNULL(DP(ctx->gma)->finallyStmt)) {
		_BLOCK_asm(ctx, DP(ctx->gma)->finallyStmt, TYPE_void, 0);
	}
}

static int _TRY_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt _UNUSED_, int sfpidx _UNUSED_)
{
	LLVM_TODO("TRY");
	return 0;
}

static int _ASSURE_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt _UNUSED_, int sfpidx _UNUSED_)
{
	LLVM_TODO("ASSURE");
	return 0;
}

static int _THROW_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt _UNUSED_, int sfpidx _UNUSED_)
{
	LLVM_TODO("THROW");
	return 0;
}

static void ASM_RET(CTX ctx, knh_Stmt_t *stmt)
{
//	while(DP(stmt)->nextNULL != NULL) {
//		stmt = DP(stmt)->nextNULL;
//		if(STT_(stmt) == STT_CHKOUT) {
//			CHKOUT_asm(ctx, stmt);
//		}
//	}
//	{
//	}
		LLVM_BUILDER(ctx)->CreateRet(NULL);
}


static int _RETURN_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt _UNUSED_, int sfpidx _UNUSED_)
{
	ASM_FINALLY(ctx);
	size_t size = DP(stmt)->size;
	if(size == 1) {
		knh_type_t rtype = Tn_type(stmt, 0);
		Tn_asm(ctx, stmt, 0, rtype, K_RTNIDX);
		if(IS_Tunbox(rtype)) {
			knh_ParamArray_t *pa = DP(DP(ctx->gma)->mtd)->mp;
			knh_param_t *p = knh_ParamArray_rget(pa, 0);
			ASM_BOX2(ctx, knh_type_tocid(ctx, p->type, DP(ctx->gma)->this_cid), rtype, K_RTNIDX);
		}
	}
	if(IS_Stmt(DP(stmt)->stmtPOST)) {
		_EXPR_asm(ctx, DP(stmt)->stmtPOST, Tn_type(stmt, 0), DP(DP(stmt)->stmtPOST)->espidx+1);
	}
	if(!Stmt_isImplicit(stmt)) {
		Value *v = ValueStack_get(ctx, K_RTNIDX);
		LLVM_BUILDER(ctx)->CreateRet(v);
	}
	return 0;
}

static int _YIELD_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt _UNUSED_, int sfpidx _UNUSED_)
{
	LLVM_TODO("yield");
	return 0;
}


static int _ERR_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt _UNUSED_, int sfpidx _UNUSED_)
{
	LLVM_TODO("ERR");
	return 0;
}

static int _PRINT_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt _UNUSED_, int sfpidx _UNUSED_)
{
	LLVM_TODO("PRINT");
	return 0;
}

static int _ASSERT_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt _UNUSED_, int sfpidx _UNUSED_)
{
	LLVM_TODO("ASSERT");
	return 0;
}

/* ------------------------------------------------------------------------ */

static int _BLOCK_asm(CTX ctx, knh_Stmt_t *stmtH, knh_type_t reqt, int sfpidx _UNUSED_)
{
	knh_Stmt_t *stmt = stmtH;
	while(stmt != NULL) {
		knh_type_t etype = (DP(stmt)->nextNULL == NULL) ? reqt : TYPE_void;
		ctx->gma->uline = stmt->uline;
		DP(ctx->gma)->espidx = DP(stmt)->espidx + DP(ctx->gma)->ebpidx;
		//DBG_P("ASM %s %p etype=%s,%s, espidx=%d", TT__(STT_(stmt)), stmt, TYPE__(etype), TYPE__(stmt->type), DP(ctx->gma)->espidx);
		switch(STT_(stmt)) {
		case STT_REGISTER:
		{
			size_t i;
			for(i = 0; i < DP(stmt)->size; i++) {
				if(IS_Stmt(tmNN(stmt, i)) && STT_(stmtNN(stmt, i)) == STT_LET) {
					_LET_asm(ctx, stmtNN(stmt, i), TYPE_void, DP(ctx->gma)->espidx);
				}
			}
			break;
		}
		case STT_BLOCK :
		{
			_BLOCK_asm(ctx, stmtNN(stmt, 0), etype, sfpidx);
			break;
		}
		CASE_ASM(IF      , etype    , 0);
		CASE_ASM(SWITCH  , etype    , 0);
		CASE_ASM(WHILE   , TYPE_void, 0);
		CASE_ASM(DO      , TYPE_void, 0);
		CASE_ASM(FOR     , TYPE_void, 0);
		CASE_ASM(FOREACH , TYPE_void, 0);
		CASE_ASM(BREAK   , TYPE_void, 0);
		CASE_ASM(CONTINUE, TYPE_void, 0);
		CASE_ASM(TRY     , TYPE_void, 0);
		CASE_ASM(THROW   , TYPE_void, 0);
		CASE_ASM(RETURN  , TYPE_void, 0);
		CASE_ASM(YIELD   , TYPE_void, 0);
		CASE_ASM(PRINT   , TYPE_void, 0);
		CASE_ASM(ASSURE  , TYPE_void, 0);
		CASE_ASM(ASSERT  , TYPE_void, 0);
		CASE_ASM(ERR     , TYPE_void, 0);
		case STT_DECL: case STT_DONE: break;
		default:
			_EXPR_asm(ctx, stmt, etype, DP(ctx->gma)->espidx);
		}
		stmt = DP(stmt)->nextNULL;
	}
	return 0;
}

static const Type *convert_type(CTX ctx, knh_class_t cid)
{
	switch (cid) {
		case TYPE_void:
			return LLVMTYPE_Void;
		case TYPE_Boolean:
			return LLVMTYPE_Bool;
		case TYPE_Int:
			return LLVMTYPE_Int;
		case TYPE_Float:
			return LLVMTYPE_Float;
	}
	return LLVMTYPE_Object;
}

static void ConstructObjectStruct(Module *m)
{
	// Type Definitions
	const Type *longTy , *shortTy;
	const Type *voidTy  = Type::getVoidTy(m->getContext());
	const Type *voidPtr = PointerType::get(voidTy, 0);
	const Type *hObjectTy, *objectPtr;
	StructType* structTy;
#if defined(SIZEOF_VOIDP) && (SIZEOF_VOIDP == 4)
	longTy  = Type::getInt32Ty(m->getContext());
	shortTy = Type::getInt16Ty(m->getContext());
#else
	longTy  = Type::getInt64Ty(m->getContext());
	shortTy = Type::getInt32Ty(m->getContext());
#endif

	/* hObject */
	std::vector<const Type*>fields;
	fields.push_back(longTy);  /* magicflag */
	fields.push_back(voidPtr); /* cTBL */
	fields.push_back(longTy);  /* refc */
	fields.push_back(voidPtr); /* meta */

	structTy = StructType::get(m->getContext(), fields, /*isPacked=*/false);
	m->addTypeName("struct.hObject", structTy);
	hObjectTy = structTy;
	fields.clear();

	/* Object */
	fields.push_back(structTy);
	fields.push_back(voidPtr);
	fields.push_back(voidPtr);
	fields.push_back(voidPtr);
	fields.push_back(voidPtr);
	structTy = StructType::get(m->getContext(), fields, /*isPacked=*/false);
	objectPtr = PointerType::get(structTy, 0);
	m->addTypeName("Object", objectPtr);
	fields.clear();

	/* ObjectField */
	fields.push_back(hObjectTy);
	fields.push_back(objectPtr);
	fields.push_back(PointerType::get(objectPtr, 0));
	structTy = StructType::get(m->getContext(), fields, /*isPacked=*/false);
	m->addTypeName("ObjectField", PointerType::get(structTy, 0));
	fields.clear();

	/* sfp */
	fields.push_back(LLVMTYPE_Int);
	fields.push_back(objectPtr);
	structTy = StructType::get(m->getContext(), fields, /*isPacked=*/false);
	const Type *sfpPtr = PointerType::get(structTy, 0);
	m->addTypeName("knh_sfp_t", sfpPtr);
	fields.clear();

	/* InputStream */
	/* System */
	/* SystemEX */
	fields.push_back(longTy);  // knh_uintptr_t sysid;
	fields.push_back(longTy);  // size_t     ctxcount;
	fields.push_back(voidPtr); // struct knh_DictMap_t*      props;
	fields.push_back(objectPtr/*TODO*/);  // struct knh_InputStream_t*  in;
	fields.push_back(objectPtr/*TODO*/);  // struct knh_OutputStream_t* out;
	fields.push_back(objectPtr/*TODO*/);  // struct knh_OutputStream_t* err;
	fields.push_back(objectPtr/*TODO*/);  // struct knh_String_t*       enc;
	fields.push_back(voidPtr); // struct knh_DictSet_t       *tokenDictSet;
	fields.push_back(voidPtr); // struct knh_DictSet_t   *nameDictCaseSet;
	fields.push_back(longTy);  // size_t                      namecapacity;
	fields.push_back(voidPtr); // knh_nameinfo_t             *nameinfo;
	fields.push_back(voidPtr); // struct knh_DictSet_t      *urnDictSet;
	fields.push_back(voidPtr); // struct knh_Array_t        *urns;
	fields.push_back(voidPtr); // struct knh_DictSet_t      *ClassNameDictSet;
	fields.push_back(voidPtr); // struct knh_DictSet_t  *EventDictCaseSet;
	fields.push_back(voidPtr); // struct knh_DictMap_t      *PackageDictMap;
	fields.push_back(voidPtr); // struct knh_DictMap_t      *URNAliasDictMap;
	fields.push_back(voidPtr); // struct knh_DictSet_t      *dspiDictSet;

	structTy = StructType::get(m->getContext(), fields, /*isPacked=*/false);
	fields.clear();
	fields.push_back(hObjectTy);
	fields.push_back(structTy);
	structTy = StructType::get(m->getContext(), fields, /*isPacked=*/false);
	const Type *sysPtr = PointerType::get(structTy, 0);
	m->addTypeName("System", sysPtr);
	fields.clear();

	/* ctx */
	fields.push_back(voidPtr);   /* share */
	fields.push_back(voidPtr);   /* stat */
	fields.push_back(voidPtr);   /* spi */
	fields.push_back(sysPtr);    /* sys */
	fields.push_back(voidPtr); /* script */
	structTy = StructType::get(m->getContext(), fields, /*isPacked=*/false);
	const Type *ctxPtr = PointerType::get(structTy, 0);
	m->addTypeName("ctx", ctxPtr);

	// Pointer Definitions
	std::vector<const Type*>args;
	args.push_back(ctxPtr);
	args.push_back(sfpPtr);
	args.push_back(longTy);
	FunctionType* callTy = FunctionType::get(/*Result=*/voidTy,/*Params=*/args,/*isVarArg=*/false);
	m->addTypeName("fcall", callTy);
}

static Function *build_function(CTX ctx, Module *m, knh_Method_t *mtd)
{
	size_t i;
	knh_ParamArray_t *pa = DP(mtd)->mp;
	knh_class_t retTy = knh_ParamArray_rtype(pa);;
	std::vector<const Type*> argsTy;
	char const *name = knh_getmnname(ctx, SP(mtd)->mn);
	for (i = 0; i < pa->psize; i++) {
		knh_type_t type = knh_ParamArray_getptype(pa, i);
		argsTy.push_back(convert_type(ctx, type));
	}
	FunctionType *fnTy = FunctionType::get(convert_type(ctx, retTy), argsTy, false);
	return cast<Function>(m->getOrInsertFunction(name, fnTy));
}

#define TC_(type, i) ((IS_Tnumbox(type))?(NC_(i)):(OC_(i)))

//static Value *build_sfp2value(CTX ctx, IRBuilder<> *builder, knh_type_t type, Value *v)
//{
//	Value *one  = ConstantInt::get(Type::getInt64Ty(LLVM_CONTEXT()), 1);
//	switch (type) {
//		case TYPE_Boolean:
//			return builder->CreateInBoundsGEP(v, idx, one, "gep");
//		case TYPE_Int:
//			return builder->CreateInBoundsGEP(v, idx, one, "gep");
//		case TYPE_Float:
//			return builder->CreateInBoundsGEP(v, idx, one, "gep");
//		default:
//			return builder->CreateInBoundsGEP(v, idx, one, "gep");
//	}
//}
static Value *create_loadsfp(CTX ctx, IRBuilder<> *builder, Value *v, knh_type_t type, int idx0)
{
	unsigned int idx = TC_(type, 0);
	//Value *idx1  = ConstantInt::get(Type::getInt64Ty(LLVM_CONTEXT()), idx);
	//std::vector<Value*> vec;
	//vec.push_back(idx0);
	//vec.push_back(idx1);
	//v = GetElementPtrInst::Create(v, vec.begin(), vec.end(), "get");
	v = builder->CreateConstGEP2_32(v, (unsigned)idx0, (unsigned)idx, "gep");
	if (IS_Tfloat(type)) {
		v = builder->CreateBitCast(v, LLVMTYPE_Float, "fcast");
	}
	return v;
}

static Function *build_wrapper_func(CTX ctx, Module *m, knh_Method_t *mtd, Function *orig_func)
{
	size_t i;
	knh_ParamArray_t *pa = DP(mtd)->mp;
	knh_class_t retTy = knh_ParamArray_rtype(pa);
	std::string name(knh_getmnname(ctx, SP(mtd)->mn));
	const FunctionType *fnTy = cast<FunctionType>(m->getTypeByName("fcall"));
	Function *f = cast<Function>(m->getOrInsertFunction(name+"_wrapper", fnTy));
	BasicBlock *bb = BasicBlock::Create(LLVM_CONTEXT(), "EntryBlock", f);
	IRBuilder<> *builder = new IRBuilder<>(bb);

	Function::arg_iterator args = f->arg_begin();
	Value *arg_ctx = args++;
	arg_ctx->setName("ctx");
	Value *arg_sfp = args++;
	arg_sfp->setName("sfp");
	Value *arg_rix = args++;
	arg_rix->setName("rix");

	builder->SetInsertPoint(bb);

	/* load konoha args from sfp and build call params*/
	std::vector<Value*> params;
	for (i = 0; i < pa->psize; i++) {
		knh_param_t *p = knh_ParamArray_get(DP(mtd)->mp, i);
		Value *v = arg_sfp;
		//Value *idx = ConstantInt::get(LLVMTYPE_Int, i+1);
		v = create_loadsfp(ctx, builder, v, p->type, i+1);
		v = builder->CreateLoad(v, "load");
		params.push_back(v);
	}

	/* call function and  set return values */
	{
		Value *callinst = builder->CreateCall(orig_func, params.begin(), params.end(), "call");
		if (retTy != TYPE_void) {
			Value *v = arg_sfp;
			v = create_loadsfp(ctx, builder, v, retTy, K_RTNIDX);
			builder->CreateStore(callinst, v, "store");
		}
		builder->CreateRetVoid();
	}
	return f;
}

static void Init(CTX ctx, knh_Method_t *mtd, knh_Array_t *a)
{
	Module *m = new Module("test", LLVM_CONTEXT());
	Function *func = build_function(ctx, m, mtd);
	BasicBlock *bb = BasicBlock::Create(LLVM_CONTEXT(), "EntryBlock", func);
	IRBuilder<> *builder = new IRBuilder<>(bb);
	ConstructObjectStruct(m);

	a->ilist[LLVM_IDX_MODULE] = (knh_int_t) m;
	a->ilist[LLVM_IDX_FUNCTION] = (knh_int_t) func;
	a->ilist[LLVM_IDX_BB] = (knh_int_t) bb;
	a->ilist[LLVM_IDX_BUILDER] = (knh_int_t) builder;

	size_t i;
	Function::arg_iterator args = func->arg_begin();
	for (i = 0; i < DP(mtd)->mp->psize; i++) {
		knh_param_t *p = knh_ParamArray_get(DP(mtd)->mp, i);
		Value *arg = args++;
		arg->setName(FN__(p->fn));
		ValueStack_set(ctx, i+1, arg);
	}
}

static void Finish(CTX ctx, knh_Method_t *mtd, knh_Array_t *a, knh_Stmt_t *stmt)
{
	knh_Fmethod f;
	Module *m = LLVM_MODULE(ctx);
	Function *func = LLVM_FUNCTION(ctx);
	//ASM_RET(ctx, stmt);
	/* asm for script function is done. */

	/* build wrapper function and compile to native code. */
	ExecutionEngine *ee = EngineBuilder(m).setEngineKind(EngineKind::JIT).create();
	Function *func1 = build_wrapper_func(ctx, m, mtd, func);
	(*m).dump();
	f = (knh_Fmethod) ee->getPointerToFunction(func1);
	knh_Method_setFunc(ctx, mtd, f);
}

#define _ALLOW_asm _EXPR_asm
#define _DENY_asm _EXPR_asm
#define _CASE_asm _EXPR_asm
#define _REGISTER_asm _EXPR_asm
#define _DECL_asm _EXPR_asm
#define _ACALL_asm _EXPR_asm
#include "codeasm.h"

} /* namespace llvm */

#ifdef __cplusplus
extern "C" {
#endif

void LLVMMethod_asm(CTX ctx, knh_Method_t *mtd, knh_Stmt_t *stmtP, knh_type_t ittype, knh_Stmt_t *stmtB, knh_Ftyping typing)
{
	static int LLVM_IS_INITED = 0;
	if (!LLVM_IS_INITED) {
		llvm::InitializeNativeTarget();
		LLVM_IS_INITED = 1;
	}
	knh_Array_t *lstack_org, *lstack;
	knh_Array_t *insts_org, *insts;
	insts     = new_Array(ctx, CLASS_Int, 8);
	lstack    = new_Array(ctx, CLASS_Int, 8);
	insts_org  = DP(ctx->gma)->insts;
	lstack_org = DP(ctx->gma)->lstacks;
	BEGIN_LOCAL(ctx, lsfp, 4);
	KNH_SETv(ctx, lsfp[0].o, insts);
	KNH_SETv(ctx, lsfp[1].o, insts_org);
	KNH_SETv(ctx, lsfp[2].o, lstack);
	KNH_SETv(ctx, lsfp[3].o, lstack_org);
	KNH_SETv(ctx, DP(ctx->gma)->insts, insts);
	KNH_SETv(ctx, DP(ctx->gma)->lstacks, lstack);

	{
		typing(ctx, mtd, stmtP, ittype, stmtB);
		DBG_ASSERT(knh_Array_size(DP(ctx->gma)->insts) == 0);
		SP(ctx->gma)->uline = SP(stmtB)->uline;
		llvmasm::Init(ctx, mtd, insts);
		if(Method_isStatic(mtd) && Gamma_hasFIELD(ctx->gma)) {
			ASM(TR, OC_(0), SFP_(0), RIX_(0), ClassTBL(DP(ctx->gma)->this_cid), _NULVAL);
		}
		llvmasm::_BLOCK_asm(ctx, stmtB, knh_ParamArray_rtype(DP(mtd)->mp), 0);
		llvmasm::Finish(ctx, mtd, insts, stmtB);
	}

	KNH_SETv(ctx, DP(ctx->gma)->insts, insts_org);
	KNH_SETv(ctx, DP(ctx->gma)->lstacks, lstack_org);
	END_LOCAL_NONGC(ctx, lsfp);
}

#ifdef __cplusplus
}
#endif
