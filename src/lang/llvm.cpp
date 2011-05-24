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

#define USE_STEXT 1
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
	Module *m = (Module*) DP(ctx->gma)->bbNC;
	return m;
}
static inline void LLVM_MODULE_SET(CTX ctx, Module *m)
{
	DP(ctx->gma)->bbNC = (knh_BasicBlock_t*) m;
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

#define NC_(sfpidx)    (((sfpidx) * 2) + 1)
#define OC_(sfpidx)    ((sfpidx) * 2)
#define SFP_(sfpidx)   ((sfpidx) * 2)

#define RIX_(rix)      rix

/* ------------------------------------------------------------------------ */
/* [Gamma] */

static Value *ValueStack_get(CTX ctx, int index)
{
	knh_Array_t *lstacks = DP(ctx->gma)->lstacks;
	knh_sfp_t lsfp = {};
	int idx = index;
	index = index + (-1 * K_RTNIDX);
	lsfp.a = lstacks;
	lstacks->api->get(ctx, &lsfp, index, 0);
	Value *v = (Value*) lsfp.ndata;
	return v;
}
#define knh_Array_capacity(a) ((a)->dim->capacity)
static void ValueStack_set(CTX ctx, int index, Value *v)
{
	knh_Array_t *lstacks = DP(ctx->gma)->lstacks;
	knh_sfp_t lsfp = {};
	int idx = index;
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

#define LLVM_CONTEXT() (llvm::getGlobalContext())
#define LLVMTYPE_Void  (Type::getVoidTy(LLVM_CONTEXT()))
#define LLVMTYPE_Bool  (Type::getInt1Ty(LLVM_CONTEXT()))
#define LLVMTYPE_Int   (Type::getInt64Ty(LLVM_CONTEXT()))
#define LLVMTYPE_Float (Type::getDoubleTy(LLVM_CONTEXT()))
static const Type *LLVMTYPE_ObjectField = NULL;
static const Type *LLVMTYPE_Object = NULL;
static const Type *LLVMTYPE_Method = NULL;
static const Type *LLVMTYPE_context = NULL;
static const Type *LLVMTYPE_fcall = NULL;
static const Type *LLVMTYPE_sfp   = NULL;
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

static void ASM_SMOVx(CTX ctx, knh_type_t atype, int a, knh_type_t btype, knh_sfx_t bx)
{
	Value *v = ValueStack_get(ctx, bx.i);
	IRBuilder<> *builder = LLVM_BUILDER(ctx);
	if(IS_Tunbox(btype)) {
		const Type *ty = convert_type(ctx, atype);
		v = builder->CreateBitCast(v, LLVMTYPE_ObjectField, "cast");
		v = builder->CreateStructGEP(v, 1, "gep");
		v = builder->CreateBitCast(v, PointerType::get(ty, 0), "cast");
		v = builder->CreateConstInBoundsGEP1_32(v, bx.n, "get_");
		v = builder->CreateLoad(v);
		ValueStack_set(ctx, a, v);
		ASM_BOX2(ctx, atype, btype, (a)); /* TODO */
	}
	else {
		asm volatile("int3");
		fprintf(stderr, "IS_Tunbox(type)=%d\n", IS_Tunbox(btype));
		ASM(OMOVx, OC_(a), bx);
		ASM_UNBOX(ctx, btype, a);
	}
}

static Value *Fset_CTX(CTX ctx, Value *arg_ctx)
{
	LLVM_TODO("SYSVAL ctx");
}
static Value *Fset_CTXIN(CTX ctx, Value *arg_ctx)
{
	IRBuilder<> *builder = LLVM_BUILDER(ctx);
	Value *v = builder->CreateConstGEP2_32(arg_ctx, 0, 27, "gep");
	v = builder->CreateLoad(v, "l1");
	return v;
}
static Value *Fset_CTXOUT(CTX ctx, Value *arg_ctx)
{
	IRBuilder<> *builder = LLVM_BUILDER(ctx);
	Value *v = builder->CreateConstGEP2_32(arg_ctx, 0, 28, "gep");
	v = builder->CreateLoad(v, "l1");
	return v;
}
static Value *Fset_CTXERR(CTX ctx, Value *arg_ctx)
{
	IRBuilder<> *builder = LLVM_BUILDER(ctx);
	Value *v = builder->CreateConstGEP2_32(arg_ctx, 0, 29, "gep");
	v = builder->CreateLoad(v, "l1");
	return v;
}
static Value *Fset_STDIN(CTX ctx, Value *arg_ctx)
{
	IRBuilder<> *builder = LLVM_BUILDER(ctx);
	Value *v = builder->CreateConstGEP2_32(arg_ctx, 0, 3, "gep");
	v = builder->CreateLoad(v, "l1");
	v = builder->CreateConstGEP2_32(v, 0, 1, "gep_");
	return builder->CreateConstGEP2_32(v, 0, 3, "gep");
}
static Value *Fset_STDOUT(CTX ctx, Value *arg_ctx)
{
	IRBuilder<> *builder = LLVM_BUILDER(ctx);
	Value *v = builder->CreateConstGEP2_32(arg_ctx, 0, 3, "gep");
	v = builder->CreateLoad(v, "l1");
	v = builder->CreateConstGEP2_32(v, 0, 1);
	return builder->CreateConstGEP2_32(v, 0, 4, "gep");
}
static Value *Fset_STDERR(CTX ctx, Value *arg_ctx)
{
	IRBuilder<> *builder = LLVM_BUILDER(ctx);
	Value *v = builder->CreateConstGEP2_32(arg_ctx, 0, 3, "gep");
	v = builder->CreateLoad(v, "l1");
	v = builder->CreateConstGEP2_32(v, 0, 1);
	return builder->CreateConstGEP2_32(v, 0, 5, "gep");
}
static Value *Fset_SYS(CTX ctx, Value *arg_ctx)
{
	IRBuilder<> *builder = LLVM_BUILDER(ctx);
	return builder->CreateConstGEP2_32(arg_ctx, 0, 3, "gep");
}
static Value *Fset_SCRIPT(CTX ctx, Value *arg_ctx)
{
	IRBuilder<> *builder = LLVM_BUILDER(ctx);
	return builder->CreateConstGEP2_32(arg_ctx, 0, 4, "gep");
}
typedef Value* (*Fset)(CTX, Value*);
static Fset SYSVAL_LOAD_INSTS[] = {
	Fset_CTX,
	Fset_CTXIN,
	Fset_CTXOUT,
	Fset_CTXERR,
	Fset_STDIN,
	Fset_STDOUT,
	Fset_STDERR,
	Fset_SYS,
	Fset_SCRIPT,
};

static Value *getctx(CTX ctx)
{
		Function::arg_iterator args = LLVM_FUNCTION(ctx)->arg_begin();
		return args;
}
static Value *getsfp(CTX ctx)
{
		Function::arg_iterator args = LLVM_FUNCTION(ctx)->arg_begin();
		args++;/* ctx */
		return args;
}

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
					knh_num_t num;
					num.data = O_data(o);
					v = ConstantFP::get(LLVMTYPE_Float, num.fvalue);
				}
				else if (IS_Tbool(atype)) {
					v = ConstantInt::get(LLVMTYPE_Bool, O_data(o));
				}
				ValueStack_set(ctx, a, v);
			}
			else if(IS_Tnumbox(atype)) {
				Value *v = ConstantInt::get(LLVMTYPE_Int, (knh_int_t)o);
				v = LLVM_BUILDER(ctx)->CreateIntToPtr(v, LLVMTYPE_Object);
				LLVM_TODO("NumBox type");
				ValueStack_set(ctx, a, v);
			}
			else {
				Value *v = ConstantInt::get(LLVMTYPE_Int, (knh_int_t)o);
				v = LLVM_BUILDER(ctx)->CreateIntToPtr(v, LLVMTYPE_Object);
				ValueStack_set(ctx, a, v);
			}
			break;
		}
		case TT_FUNCVAR:
		case TT_LOCAL: {
			int b = Token_index(tkb);
			if(IS_Tunbox(btype)) {
				Value *v = ValueStack_get(ctx, b);
				ValueStack_set(ctx, a, v);
			}
			else {
				Value *v = ValueStack_get(ctx, b);
				ValueStack_set(ctx, a, v);
				//ASM(OMOV, OC_(a), OC_(b));
				//if(IS_Tnumbox(btype)) {
				//	if(IS_Tnumbox(atype)) {
				//		ASM(NMOV, NC_(a), NC_(b));
				//	}
				//	else {
				//		ASM(TR, OC_(a), SFP_(b), RIX_(a-b), ClassTBL(atype), _OBOX);
				//	}
				//}
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
			Value *arg_ctx = getctx(ctx);
			Value *v = SYSVAL_LOAD_INSTS[sysid](ctx, arg_ctx);
			ValueStack_set(ctx, a, v);
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
	IRBuilder<> *builder = LLVM_BUILDER(ctx);
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
			Object *o = (tkb)->data;
			if(IS_Tunbox(atype)) {
				Value *vdata = ConstantInt::get(LLVMTYPE_Int, O_data(o));
				const Type *ty = convert_type(ctx, atype);
				Value *v = ValueStack_get(ctx, a);
				v = builder->CreateBitCast(v, LLVMTYPE_ObjectField, "cast");
				v = builder->CreateConstInBoundsGEP2_32(v, 0, 1, "gep");
				v = builder->CreateBitCast(v, PointerType::get(ty, 0), "cast");
				v = builder->CreateConstInBoundsGEP1_32(v, an, "get_");
				const Type *tp = cast<PointerType>(v->getType())->getElementType();
				builder->CreateStore(vdata, v, false);
				asm volatile("int3");
				//ASM(XNSET, ax, O_data(o));
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
				asm volatile("int3");
				ASM(XNMOV, ax, NC_(b));
			}
			else {
				asm volatile("int3");
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
			asm volatile("int3");
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
	if(!IS_Method(mtd)) {
		return TYPE_dyn;  // boxing
	}
	if(n == 1) { // base
		if(IS_Tunbox(cid) && cid != (mtd)->cid) {
			/* Object, Number, dynamic */
			cid = (mtd)->cid;
		}
		DBG_(
		if(cid != (mtd)->cid) {
			DBG_P("** WATCH OUT ** mtd_cid=%s, (mtd)->cid=%s", CLASS__(cid), CLASS__((mtd)->cid));
		});
		return cid;
	}
	else {
		knh_type_t ptype = knh_ParamArray_getptype(DP(mtd)->mp, n - 2);
		return knh_type_tocid(ctx, ptype, cid);
	}
}
static Value *create_loadsfp(CTX ctx, IRBuilder<> *builder, Value *v, knh_type_t type, int idx0);

static int CALLPARAMs_asm(CTX ctx, knh_Stmt_t *stmt, size_t s, int local, knh_class_t cid, knh_Method_t *mtd)
{
	size_t i;
	if(s == 1 && Method_isStatic(mtd))
		// ignoring static caller, like Script
		s = 2;
	if(DP(stmt)->size + DP(ctx->gma)->espidx > 32) {
		LLVM_TODO("check stack");
	}
	Value *arg_sfp = getsfp(ctx);
	IRBuilder<> *builder = LLVM_BUILDER(ctx);
	for(i = s; i < DP(stmt)->size; i++) {
		knh_type_t reqt = Tn_ptype(ctx, stmt, i, cid, mtd);
		int a = local + i + (K_CALLDELTA-1);
		Tn_asm(ctx, stmt, i, reqt, a);
		Value *v = ValueStack_get(ctx, a);
		const Type *ty = v->getType();
	if (reqt == TYPE_dyn && ty == LLVMTYPE_Int) {
		reqt = TYPE_Int;
	} else if (reqt == TYPE_dyn && ty == LLVMTYPE_Bool) {
		reqt = TYPE_Boolean;
	}

		Value *ptr = create_loadsfp(ctx, builder, arg_sfp, reqt, a);
		const Type *pty = cast<PointerType>(ptr->getType())->getElementType();
		const Type *vty = v->getType();
		builder->CreateStore(v, ptr, false/*isVolatile*/);
	}
	return 1;
}

/* for debug */
static const char* getname(CTX ctx, const Type *v)
{
	return LLVM_MODULE(ctx)->getTypeName(v).c_str();
}

static void asm_shift_esp(CTX ctx, Value *vctx, Value *vsfp, size_t idx)
{
	IRBuilder<> *builder = LLVM_BUILDER(ctx);
	Value *vsfp_, *vesp;
	vesp = builder->CreateConstInBoundsGEP2_32(vctx, 0, 6, "gep"); /* esp */
	vsfp_ = builder->CreateConstInBoundsGEP1_32(vsfp, idx);
	builder->CreateStore(vsfp_, vesp, false);
}

static void _CALL(CTX ctx, knh_type_t reqt, int sfpidx, knh_type_t rtype, knh_Method_t *mtd, int isStatic, size_t argc)
{
	int thisidx = sfpidx + K_CALLDELTA;
	IRBuilder<> *builder = LLVM_BUILDER(ctx);
	DBG_ASSERT(IS_Method(mtd));
	KNH_ASSERT(sfpidx >= DP(ctx->gma)->espidx);
	if(Method_isFinal(mtd) || isStatic) {
		if(Method_isKonohaCode(mtd) || DP(ctx->gma)->mtd == mtd) {
			Value *idx, *vctx, *vsfp;
			std::vector<Value*> params;
			Function::arg_iterator arg = LLVM_FUNCTION(ctx)->arg_begin();
			vctx = arg++;
			vsfp = arg++;
			idx = ConstantInt::get(LLVMTYPE_Int, thisidx);
			params.push_back(vctx);
			params.push_back(builder->CreateGEP(vsfp, idx, "sfpsft"));
			params.push_back(ConstantInt::get(LLVMTYPE_Int, K_RTNIDX));

			asm_shift_esp(ctx, vctx, vsfp, argc+1+thisidx);

			Value *v = ConstantInt::get(LLVMTYPE_Int, (knh_int_t) mtd);
			v = builder->CreateIntToPtr(v, LLVMTYPE_Method);
			v = builder->CreateConstGEP2_32(v, 0, 4, "mtdgep");
			v = builder->CreateLoad(v, "fcall");

			Value *call = builder->CreateCall(v, params.begin(), params.end());
			knh_class_t retTy = knh_ParamArray_rtype(DP(mtd)->mp);
			if(retTy != TYPE_void){
				Value *ptr = create_loadsfp(ctx, builder, vsfp, retTy, thisidx+K_RTNIDX);
				Value *ret_v = builder->CreateLoad(ptr, "ret_v");
				ValueStack_set(ctx, sfpidx, ret_v);
			}
		}
		else {
			knh_Fmethod func = mtd->fcall_1;
			Value *v, *idx, *vctx, *vsfp;
			std::vector<Value*> params;
			Function::arg_iterator arg = LLVM_FUNCTION(ctx)->arg_begin();
			idx = ConstantInt::get(LLVMTYPE_Int, thisidx);
			vctx = arg++;
			vsfp = arg++;
			params.push_back(vctx);
			params.push_back(builder->CreateGEP(vsfp, idx, "sfpsft"));
			params.push_back(ConstantInt::get(LLVMTYPE_Int, K_RTNIDX));

			asm_shift_esp(ctx, vctx, vsfp, argc+1+thisidx);
			v = ConstantInt::get(LLVMTYPE_Int, (knh_int_t)func);
			v = builder->CreateIntToPtr(v, LLVMTYPE_fcall);
			Value *call = builder->CreateCall(v, params.begin(), params.end());
			knh_class_t retTy = knh_ParamArray_rtype(DP(mtd)->mp);
			if(retTy != TYPE_void){
				Value *ptr = create_loadsfp(ctx, builder, vsfp, retTy, thisidx+K_RTNIDX);
				Value *ret_v = builder->CreateLoad(ptr, "ret_v");
				ValueStack_set(ctx, thisidx+K_RTNIDX, ret_v);//call);
			}
		}
	}
	else {
		LLVM_TODO("LOADMTD");
	}
}

static int _CALL_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	int local = ASML(sfpidx);
	knh_Token_t *tkMTD = tkNN(stmt, 0);
	knh_Method_t *mtd = (tkMTD)->mtd;
	knh_class_t cid = Tn_cid(stmt, 1);
	if(!IS_Method(mtd)) {
		LLVM_TODO("LOADMTD");
		return 0;
	}
	knh_class_t mtd_cid = (mtd)->cid;
	knh_methodn_t mtd_mn = (mtd)->mn;
	if(mtd_cid == CLASS_Array) {
		IRBuilder<> *builder = LLVM_BUILDER(ctx);
		int obj = Tn_put(ctx, stmt, 1, cid, local+1);
		Value *vobj = ValueStack_get(ctx, obj);
		const Type *type = convert_type(ctx, knh_class_p1(cid));
		const Type *ptype = PointerType::get(type, 0);
		if(Tn_isCONST(stmt, 2)) {
			knh_intptr_t n = (knh_intptr_t)Tn_int(stmt, 2);
			if(n < 0) {
				goto L_USECALL;
			}
			Value *nlist_ptr = builder->CreateConstGEP2_32(vobj, 0, 1, "nlist_ptr");
			Value *nlist = builder->CreateLoad(nlist_ptr, "nlist");
			nlist = builder->CreateBitCast(nlist, ptype, "cast");
			Value *v_ptr = builder->CreateConstInBoundsGEP2_32(nlist, 0, n, "v_ptr");
			Value *v = builder->CreateLoad(v_ptr, "v");
			ValueStack_set(ctx, sfpidx, v);

		} else {
			int n = Tn_put(ctx, stmt, 2, TYPE_Int, local+2);
			Value *vn = ValueStack_get(ctx, n);
			Value *nlist_ptr = builder->CreateConstGEP2_32(vobj, 0, 1, "nlist_ptr");
			Value *nlist = builder->CreateLoad(nlist_ptr, "nlist");
			nlist = builder->CreateBitCast(nlist, ptype, "cast");
			Value *v_ptr = builder->CreateGEP(nlist, vn, "v_ptr");
			Value *v = builder->CreateLoad(v_ptr, "v");
			ValueStack_set(ctx, sfpidx, v);
		}
		return 0;
	}
#if defined(OPCODE_BGETIDX)
	if(mtd_cid == CLASS_Bytes) {
		LLVM_TODO("CALL Bytes get/set");
	}
#endif
#ifdef OPCODE_bNUL
	if(mtd_cid == CLASS_Object) {
		LLVM_TODO("Object null check");
	}
#endif
	{
		knh_index_t deltaidx = knh_Method_indexOfGetterField(mtd);
		if(deltaidx != -1) {
			int b = Tn_put(ctx, stmt, 1, mtd_cid, local + 1);
			knh_type_t type = knh_ParamArray_rtype(DP(mtd)->mp);
			knh_sfx_t bx = {b, deltaidx};
			ASM_SMOVx(ctx, reqt, sfpidx, type, bx);
			return 0;
		}
		deltaidx = knh_Method_indexOfSetterField(mtd);
		if(deltaidx != -1) {
			int b = Tn_put(ctx, stmt, 1, mtd_cid, local + 1);
			knh_type_t reqt2 = knh_Method_ptype(ctx, mtd, cid, 0);
			knh_Token_t *tkV = Tn_putTK(ctx, stmt, 2, reqt, local + 2);
			ASM_XMOV(ctx, reqt2, b, deltaidx, tkV);
			if(reqt != TYPE_void) {
				ASM_SMOV(ctx, reqt, sfpidx, tkV);
			}
		}
	}
	L_USECALL:;
	{
#ifdef OPCODE_FASTCALL0
		//knh_type_t rtype = knh_type_tocid(ctx, knh_ParamArray_rtype(DP(mtd)->mp), cid);
		//if(DP(stmt)->size == 2 && Method_isFASTCALL0(ctx, mtd)) {
		//	int a = local;
		//	if(!Method_isStatic(mtd)) {
		//		a = Tn_put(ctx, stmt, 1, TYPE_cid(mtd_cid), local + 1);
		//	}
		//	//KNH_P("FASTCALL %s.%s", CLASS__((mtd)->cid), MN__((mtd)->mn));
		//	ASM(FASTCALL0, RTNIDX_(ctx, sfpidx, rtype), SFP_(a), RIX_(sfpidx - a), SP(mtd)->fcall_1, mtd);
		//	return;
		//}
		//if(DP(stmt)->size == 3 && Method_isStatic(mtd) && Method_isFASTCALL0(ctx, mtd)) {
		//	knh_param_t *p = knh_ParamArray_get(DP(mtd)->mp, 0);
		//	knh_type_t ptype = knh_type_tocid(ctx, p->type, mtd_cid);
		//	int a = Tn_put(ctx, stmt, 2, ptype, local + 2);
		//	//KNH_P("STATIC FASTCALL %s.%s", CLASS__((mtd)->cid), MN__((mtd)->mn));
		//	ASM(FASTCALL0, RTNIDX_(ctx, sfpidx, rtype), SFP_(a - 1), RIX_(sfpidx - (a - 1)), SP(mtd)->fcall_1, mtd);
		//	return;
		//}
#endif
		if(CALLPARAMs_asm(ctx, stmt, 1, local, cid, mtd)) {
			knh_type_t rtype = knh_type_tocid(ctx, knh_ParamArray_rtype(DP(mtd)->mp), cid);
			int isStatic = Method_isStatic(mtd);
			_CALL(ctx, reqt, local, rtype, mtd, isStatic, DP(stmt)->size - 2);
			ASM_MOVL(ctx, reqt, sfpidx, SP(stmt)->type, local);
		}
	}

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
	int i, local = ASML(sfpidx),  size = DP(stmt)->size;
	Module *m = LLVM_MODULE(ctx);
	IRBuilder<> *builder = LLVM_BUILDER(ctx);
	BasicBlock *lbTrue = BasicBlock::Create(m->getContext(), "true", LLVM_FUNCTION(ctx));
	BasicBlock *lbNext = BasicBlock::Create(m->getContext(), "next", LLVM_FUNCTION(ctx));
	std::vector<BasicBlock *> blocks;

	for(i = 0; i < size; i++){
		int n = Tn_put(ctx, stmt, i, TYPE_Boolean, local + 1);
		Value *cond = ValueStack_get(ctx, n);
		builder->CreateCondBr(cond, lbTrue, lbNext);

		blocks.push_back(builder->GetInsertBlock());
		builder->SetInsertPoint(lbNext);
		if(i + 1 != size)
			lbNext = BasicBlock::Create(m->getContext(), "next", LLVM_FUNCTION(ctx));
	}
	builder->CreateBr(lbTrue);
	builder->SetInsertPoint(lbTrue);

	PHINode *phi = builder->CreatePHI(LLVMTYPE_Bool, "or_result");
	phi->addIncoming(ConstantInt::get(LLVMTYPE_Bool, 0), lbNext);

	std::vector<BasicBlock*>::iterator itr;
	for (itr = blocks.begin(); itr != blocks.end(); itr++) {
		phi->addIncoming(ConstantInt::get(LLVMTYPE_Bool, 1), *itr);
	}
	ValueStack_set(ctx, sfpidx, phi);

	return 0;
}

static int _AND_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	int i, local = ASML(sfpidx), size = DP(stmt)->size;
	Module *m = LLVM_MODULE(ctx);
	IRBuilder<> *builder = LLVM_BUILDER(ctx);
	BasicBlock *lbFalse = BasicBlock::Create(m->getContext(), "false", LLVM_FUNCTION(ctx));
	BasicBlock *lbNext = BasicBlock::Create(m->getContext(), "next", LLVM_FUNCTION(ctx));
	std::vector<BasicBlock *> blocks;
	for(i = 0; i < size; i++){
		int n = Tn_put(ctx, stmt, i, TYPE_Boolean, local + 1);
		Value *cond = ValueStack_get(ctx, n);
		builder->CreateCondBr(cond, lbNext, lbFalse);
		blocks.push_back(builder->GetInsertBlock());
		builder->SetInsertPoint(lbNext);
		if(i + 1 != size)
			lbNext = BasicBlock::Create(m->getContext(), "next", LLVM_FUNCTION(ctx));
	}
	builder->CreateBr(lbFalse);
	builder->SetInsertPoint(lbFalse);

	PHINode *phi = builder->CreatePHI(LLVMTYPE_Bool, "and_result");
	phi->addIncoming(ConstantInt::get(LLVMTYPE_Bool, 1), lbNext);
	std::vector<BasicBlock*>::iterator itr;
	for (itr = blocks.begin(); itr != blocks.end(); itr++) {
		phi->addIncoming(ConstantInt::get(LLVMTYPE_Bool, 0), *itr);
	}
	ValueStack_set(ctx, sfpidx, phi);
	return 0;
}

static int Tn_CondAsm(CTX ctx, knh_Stmt_t *stmt, size_t n, int isTRUE, int flocal);

static int _TRI_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	int local = DP(ctx->gma)->espidx;
	Module *m = LLVM_MODULE(ctx);
	IRBuilder<> *builder = LLVM_BUILDER(ctx);
	BasicBlock *bbThen  = BasicBlock::Create(m->getContext(), "then", LLVM_FUNCTION(ctx));
	BasicBlock *bbElse  = BasicBlock::Create(m->getContext(), "else", LLVM_FUNCTION(ctx));
	BasicBlock *bbMerge = BasicBlock::Create(m->getContext(), "merge",LLVM_FUNCTION(ctx));

	int a = Tn_CondAsm(ctx, stmt, 0, 0, sfpidx);
	Value *cond = ValueStack_get(ctx, a);
	builder->CreateCondBr(cond, bbThen, bbElse);

	builder->SetInsertPoint(bbThen);
	int b = Tn_put(ctx, stmt, 1, reqt, local);
	Value* vb = ValueStack_get(ctx, b);
	builder->CreateBr(bbMerge);

	builder->SetInsertPoint(bbElse);
	int c = Tn_put(ctx, stmt, 2, reqt, local);
	Value* vc = ValueStack_get(ctx, c);
	builder->CreateBr(bbMerge);

	builder->SetInsertPoint(bbMerge);
	PHINode *phi = builder->CreatePHI(convert_type(ctx, reqt), "tri_result");
	phi->addIncoming(vb, bbThen);
	phi->addIncoming(vc, bbElse);
	ValueStack_set(ctx, sfpidx, phi);
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
static void ASM_BBLAST(CTX ctx, void *ptr, void (*func)(CTX, void*))
{
	BasicBlock *bb = LLVM_BUILDER(ctx)->GetInsertBlock();
	BasicBlock::iterator itr;
	for(itr = bb->begin(); itr != bb->end(); itr++) {
		Instruction &inst = *itr;
		if(ReturnInst::classof(&inst)){
			return;
		}
	}
	func(ctx, ptr);
}

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

static void __asm_br(CTX ctx, void *ptr)
{
	BasicBlock *bbMerge = (BasicBlock*) ptr;
	LLVM_BUILDER(ctx)->CreateBr(bbMerge);
}
static void __asm_ret(CTX ctx, void *ptr)
{
	LLVM_BUILDER(ctx)->CreateRetVoid();
}

static int PHI_asm(CTX ctx, knh_Array_t *prev, knh_Array_t *thenArray, knh_Array_t *elseArray, BasicBlock *bbThen, BasicBlock *bbElse)
{
	int i, size;
	size = knh_Array_capacity(prev) + K_RTNIDX;
	for(i=0; i<size; i++){
		Value *vp = (Value *)prev->nlist[i - K_RTNIDX];
		if(vp == NULL) continue;
		Value *v1 = (Value *)thenArray->nlist[i - K_RTNIDX];
		Value *v2 = (Value *)elseArray->nlist[i - K_RTNIDX];
		if(vp != v1 || vp != v2){
			PHINode *phi = LLVM_BUILDER(ctx)->CreatePHI(v1->getType(), "phi");
			phi->addIncoming(v1, bbThen);
			phi->addIncoming(v2, bbElse);//bbElse);
			prev->nlist[i - K_RTNIDX] = (knh_ndata_t)phi;
		}
	}
	return 1;
}

static knh_Array_t *ValueStack_copy(CTX ctx, knh_Array_t *lstacks){
	int size = (int)knh_Array_capacity(lstacks);
	knh_Array_t *newlstacks = new_Array(ctx, CLASS_Int, size);
	memcpy(newlstacks->nlist, lstacks->nlist, sizeof(knh_ndata_t) * size);
	return newlstacks;
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

	knh_Array_t *prev = DP(ctx->gma)->lstacks;
	knh_Array_t *st1 = ValueStack_copy(ctx, prev);
	knh_Array_t *st2 = ValueStack_copy(ctx, prev);
	BEGIN_LOCAL(ctx, lsfp, 3);
	KNH_SETv(ctx, lsfp[0].o, prev);
	KNH_SETv(ctx, lsfp[1].o, st1);
	KNH_SETv(ctx, lsfp[2].o, st2);

	builder->SetInsertPoint(bbThen);
	DP(ctx->gma)->lstacks = st1;
	Tn_asmBLOCK(ctx, stmt, 1, reqt);
	ASM_BBLAST(ctx, (void*)bbMerge, __asm_br);
	bbThen = builder->GetInsertBlock();

	builder->SetInsertPoint(bbElse);
	DP(ctx->gma)->lstacks = st2;
	Tn_asmBLOCK(ctx, stmt, 2, reqt);
	ASM_BBLAST(ctx, (void*)bbMerge, __asm_br);
	bbElse = builder->GetInsertBlock();

	builder->SetInsertPoint(bbMerge);
	PHI_asm(ctx, prev, st1, st2, bbThen, bbElse);
	DP(ctx->gma)->lstacks = prev;
	END_LOCAL_NONGC(ctx, lsfp);

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
	BasicBlock *bbContinue = BasicBlock::Create(m->getContext(), "continue", LLVM_FUNCTION(ctx));
	BasicBlock *bbBreak    = BasicBlock::Create(m->getContext(), "break", LLVM_FUNCTION(ctx));
	BasicBlock *bbBlock    = BasicBlock::Create(m->getContext(), "block", LLVM_FUNCTION(ctx));
	BasicBlock *bbPrev     = builder->GetInsertBlock();

	PUSH_LABEL(ctx, stmt, bbContinue, bbBreak);
	builder->CreateBr(bbContinue);
	builder->SetInsertPoint(bbContinue);

	knh_Array_t *prev = DP(ctx->gma)->lstacks;
	// replace phi
	int i, size;
	size = knh_Array_capacity(prev) + K_RTNIDX;
	for(i=0; i<size; i++){
		Value *v = (Value *)prev->nlist[i - K_RTNIDX];
		if(v != NULL){
			PHINode *phi = builder->CreatePHI(v->getType(), "phi");
			phi->addIncoming(v, bbPrev);
			prev->nlist[i - K_RTNIDX] = (knh_ndata_t)phi;
		}
	}
	knh_Array_t *stCon = ValueStack_copy(ctx, prev);
	BEGIN_LOCAL(ctx, lsfp, 3);
	KNH_SETv(ctx, lsfp[0].o, prev);
	KNH_SETv(ctx, lsfp[1].o, stCon);

	DP(ctx->gma)->lstacks = stCon;
	if (!Tn_isTRUE(stmt, 0)) {
		int n = Tn_put(ctx, stmt, 0, TYPE_Boolean, local);
		cond = ValueStack_get(ctx, n);
	} else {
		cond = ConstantInt::get(LLVMTYPE_Bool, 1);
	}
	builder->CreateCondBr(cond, bbBlock, bbBreak);

	knh_Array_t *stBlock = ValueStack_copy(ctx, stCon);
	KNH_SETv(ctx, lsfp[2].o, stBlock);
	DP(ctx->gma)->lstacks = stBlock;

	builder->SetInsertPoint(bbBlock);
	Tn_asmBLOCK(ctx, stmt, 1, TYPE_void);
	builder->CreateBr(bbContinue);
	// add phi
	for(i=0; i<size; i++){
		Value *v = (Value *)stBlock->nlist[i - K_RTNIDX];
		PHINode *phi = (PHINode *)prev->nlist[i - K_RTNIDX];
		if(phi != NULL && v != phi){
			phi->addIncoming(v, bbBlock);
		}
	}

	DP(ctx->gma)->lstacks = stCon;

	builder->SetInsertPoint(bbBreak);
	POP_LABEL(ctx);
	END_LOCAL_NONGC(ctx, lsfp);
	return 0;
}

static int _DO_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt _UNUSED_, int sfpidx _UNUSED_)
{
	Value *cond = NULL;//expr
	int local = DP(ctx->gma)->espidx;
	Module *m = LLVM_MODULE(ctx);
	IRBuilder<> *builder = LLVM_BUILDER(ctx);
	BasicBlock *bbContinue = BasicBlock::Create(m->getContext(), "continue", LLVM_FUNCTION(ctx));
	BasicBlock *bbBreak    = BasicBlock::Create(m->getContext(), "break", LLVM_FUNCTION(ctx));
	PUSH_LABEL(ctx, stmt, bbContinue, bbBreak);

	builder->CreateBr(bbContinue);
	/* body */
	Tn_asmBLOCK(ctx, stmt, 0, TYPE_void);

	/* cond */
	int n = Tn_put(ctx, stmt, 1, TYPE_Boolean, local);
	cond = ValueStack_get(ctx, n);

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

static void ASM_LastRET(CTX ctx, knh_Stmt_t *stmt)
{
	//while(DP(stmt)->nextNULL != NULL) {
	//	stmt = DP(stmt)->nextNULL;
	//	if(STT_(stmt) == STT_CHKOUT) {
	//		CHKOUT_asm(ctx, stmt);
	//	}
	//}
	BasicBlock *bb = LLVM_BUILDER(ctx)->GetInsertBlock();
	ASM_BBLAST(ctx, (void*)bb, __asm_ret);
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

static knh_flag_t PRINT_flag(CTX ctx, knh_Stmt_t *o)
{
	knh_flag_t flag = 0;
	if(IS_Map(DP(o)->metaDictCaseMap)) {
		Object *v = knh_DictMap_getNULL(ctx,  DP(o)->metaDictCaseMap, STEXT("Time"));
		if(v != NULL) {
			flag |= K_FLAG_PF_TIME;
		}
	}
	return flag;
}

static Function *PRINT_func(CTX ctx, const char *name, knh_class_t cid)
{
	Module *m = LLVM_MODULE(ctx);
	std::vector<const Type*>args_list;
	args_list.push_back(LLVMTYPE_context);
	args_list.push_back(LLVMTYPE_sfp);
	args_list.push_back(LLVMTYPE_Int); /* flag */
	args_list.push_back(LLVMTYPE_Int); /* uline */
	args_list.push_back(LLVMTYPE_Object); /* msg */
	args_list.push_back(convert_type(ctx, cid));
	FunctionType* fnTy = FunctionType::get(LLVMTYPE_Void, args_list, false);
	Function *func = cast<Function>(m->getOrInsertFunction(name, fnTy));
	return func;
}

struct print_data {
	knh_class_t cid;
	const char *name;
};
static struct print_data PRINT_DATA[] = {
	{CLASS_Boolean, "llvm_PRINTb"},
	{CLASS_Int,     "llvm_PRINTi"},
	{CLASS_Float,   "llvm_PRINTf"},
	{CLASS_Object,  "llvm_PRINT"},
};

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))
static void init_print_func(CTX ctx)
{
	Module *m = LLVM_MODULE(ctx);
	Function* func;
	std::vector<const Type*>args;
	int i;
	for (i = 0; i < ARRAY_SIZE(PRINT_DATA); i++) {
		knh_class_t cid = PRINT_DATA[i].cid;
		const char *name = PRINT_DATA[i].name;
		args.push_back(LLVMTYPE_context);
		args.push_back(LLVMTYPE_sfp);
		args.push_back(LLVMTYPE_Int); /* flag */
		args.push_back(LLVMTYPE_Int); /* uline */
		args.push_back(LLVMTYPE_Object); /* msg */
		args.push_back(convert_type(ctx, cid));

		FunctionType* fnTy = FunctionType::get(LLVMTYPE_Void, args, false);
		func = Function::Create(fnTy, GlobalValue::ExternalLinkage, name, m);
		func->setCallingConv(CallingConv::C);
		args.clear();
	}
}

static void ASM_P(CTX ctx, const char *name, knh_flag_t flag, knh_uline_t line, knh_String_t *msg, knh_class_t cid, Value *v)
{
	Function *f = PRINT_func(ctx, name, cid);
	Function::arg_iterator args = LLVM_FUNCTION(ctx)->arg_begin();
	IRBuilder<> *builder = LLVM_BUILDER(ctx);
	Value *arg_ctx = args++;
	Value *arg_sfp = args;
	std::vector<Value*> params;
	params.push_back(arg_ctx);
	params.push_back(arg_sfp);
	params.push_back(ConstantInt::get(LLVMTYPE_Int, flag));
	params.push_back(ConstantInt::get(LLVMTYPE_Int, line));
	Value *vmsg = ConstantInt::get(LLVMTYPE_Int, (knh_int_t) msg);
	params.push_back(builder->CreateIntToPtr(vmsg, LLVMTYPE_Object)); /* msg */
	params.push_back(v);
	builder->CreateCall(f, params.begin(), params.end());
}

static int _PRINT_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt _UNUSED_, int sfpidx _UNUSED_)
{
	knh_flag_t flag = PRINT_flag(ctx, stmt) | K_FLAG_PF_BOL | K_FLAG_PF_LINE;
	long i, espidx = DP(ctx->gma)->espidx;
	for(i = 0; i < DP(stmt)->size; i++) {
		knh_Token_t *tkn = tkNN(stmt, i);
		if(TT_(tkn) != TT_CONST || !IS_String((tkn)->data)) {
			knh_class_t cid = Tn_cid(stmt, i);
			Tn_asm(ctx, stmt, i, cid, espidx + i);
		}
	}
	for(i = 0; i < DP(stmt)->size; i++) {
		knh_flag_t mask = 0;
		knh_String_t *msg = (knh_String_t*)KNH_NULL;
		L_REDO:;
		knh_Token_t *tkn = tkNN(stmt, i);
		if(i == (long)DP(stmt)->size - 1) {
			mask |= K_FLAG_PF_EOL;
		}
		if(TT_(tkn) == TT_CONST && IS_String((tkn)->data)) {
			if(Token_isPNAME(tkn)) { /* name= */
				msg = (tkn)->text;
				mask |= K_FLAG_PF_NAME; i++;
				goto L_REDO;
			}
			DBG_ASSERT(stmt->uline == ctx->gma->uline);
			//ASM(P, _PRINTm, flag | mask, (tkn)->text, 0); flag = 0;
		}
		else {
			knh_class_t cid = Tn_cid(stmt, i);
			Value *v = ValueStack_get(ctx, espidx+i);
			const char *fname;
			if(IS_Tint(cid))        fname = "llvm_PRINTi";
			else if(IS_Tfloat(cid)) fname = "llvm_PRINTf";
			else if(IS_Tbool(cid))  fname = "llvm_PRINTb";
			else                    fname = "llvm_PRINT";
			ASM_P(ctx, fname, flag | mask, tkNN(stmt, i)->uline, msg, cid, v);
			flag=0;
		}
	}
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

static void ConstructObjectStruct(Module *m)
{
	// Type Definitions
	const Type *longTy , *shortTy;
	const Type *voidTy  = Type::getVoidTy(m->getContext());
	const Type *hObjectTy, *objectPtr;
	StructType* structTy;
#if defined(SIZEOF_VOIDP) && (SIZEOF_VOIDP == 4)
	longTy  = Type::getInt32Ty(m->getContext());
	shortTy = Type::getInt16Ty(m->getContext());
#else
	longTy  = Type::getInt64Ty(m->getContext());
	shortTy = Type::getInt32Ty(m->getContext());
#endif
	const Type *voidPtr = PointerType::get(longTy, 0);

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
	LLVMTYPE_Object = objectPtr;
	m->addTypeName("Object", objectPtr);
	fields.clear();

	/* ObjectField */
	fields.push_back(hObjectTy);
	fields.push_back(objectPtr);
	fields.push_back(PointerType::get(objectPtr, 0));
	structTy = StructType::get(m->getContext(), fields, /*isPacked=*/false);
	LLVMTYPE_ObjectField = PointerType::get(structTy, 0);
	m->addTypeName("ObjectField", LLVMTYPE_ObjectField);
	fields.clear();

	/* sfp */
	fields.push_back(objectPtr);
	//fields.push_back(LLVMTYPE_Int);
	fields.push_back(LLVMTYPE_Float);
	structTy = StructType::get(m->getContext(), fields, /*isPacked=*/false);
	const Type *sfpPtr = PointerType::get(structTy, 0);
	m->addTypeName("struct.knh_sfp_t", structTy);
	m->addTypeName("knh_sfp_t_ptr", sfpPtr);
	LLVMTYPE_sfp = sfpPtr;
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
	/* 00 */fields.push_back(voidPtr);   /* share */
	/* 01 */fields.push_back(voidPtr);   /* stat */
	/* 02 */fields.push_back(voidPtr);   /* spi */
	/* 03 */fields.push_back(sysPtr);    /* sys */
	/* 04 */fields.push_back(voidPtr); /* script */
	/* stack */
	/* 05 */fields.push_back(sfpPtr); //knh_sfp_t*                   stack;
	/* 06 */fields.push_back(sfpPtr); //knh_sfp_t*                   esp;
	/* 07 */fields.push_back(longTy); //size_t                       stacksize;
	/* 08 */fields.push_back(sfpPtr); //knh_sfp_t*                   stacktop;
	/* 09 */fields.push_back(voidPtr); //void*                        cstack_bottom;
	/* 10 */fields.push_back(objectPtr); //struct knh_Exception_t      *e;
	/* 11 */fields.push_back(objectPtr); //struct knh_Monitor_t        *mon;
	/* memory */
	/* 12 */fields.push_back(objectPtr); //knh_Object_t                *freeObjectList;
	/* 13 */fields.push_back(objectPtr); //knh_Object_t                *freeObjectTail;
	/* 14 */fields.push_back(longTy); //size_t                       freeObjectListSize;
	/* 15 */fields.push_back(longTy); //knh_uintptr_t                mscheck;
	/* 16 */fields.push_back(voidPtr); //knh_fastmem_t               *freeMemoryList;
	/* 17 */fields.push_back(voidPtr); //knh_fastmem_t               *freeMemoryTail;

	/* cache */
	/* 18 */fields.push_back(voidPtr); //knh_mtdcache_t              *mtdcache;
	/* 19 */fields.push_back(voidPtr); //knh_tmrcache_t             *tmrcache;
	/* 20 */fields.push_back(voidPtr); //struct knh_Object_t        **refs;
	/* 21 */fields.push_back(longTy); //size_t                       ref_size;
	/* 22 */fields.push_back(voidPtr); //struct knh_Object_t        **ref_buf;
	/* 23 */fields.push_back(longTy); //size_t                       ref_capacity;
	/* 24 */fields.push_back(voidPtr); //struct knh_Object_t        **queue;
	/* 25 */fields.push_back(longTy); //size_t                       queue_capacity;

	/* 26 */fields.push_back(objectPtr); //struct knh_String_t*         enc;
	/* 27 */fields.push_back(objectPtr); //struct knh_InputStream_t*    in;
	/* 28 */fields.push_back(objectPtr); //struct knh_OutputStream_t*   out;
	/* 29 */fields.push_back(objectPtr); //struct knh_OutputStream_t*   err;
	/* 30 */fields.push_back(objectPtr); //struct knh_Bytes_t*          bufa;
	/* 31 */fields.push_back(objectPtr); //struct knh_OutputStream_t*   bufw;
	/* 32 */fields.push_back(objectPtr); //struct knh_Gamma_t*          gma;
	/* 33 */fields.push_back(objectPtr); //struct knh_DictMap_t*        symbolDictMap;
	/* 34 */fields.push_back(objectPtr); //struct knh_Array_t*          constPools;

	fields.push_back(shortTy);   //knh_flag_t                   flag;
	fields.push_back(shortTy);   //knh_ushort_t                 ctxid;
	fields.push_back(voidPtr);   //struct knh_Context_t        *ctxobjNC;
	fields.push_back(voidPtr);   //struct knh_context_t        *parent;
	fields.push_back(voidPtr);   //knh_mutex_t                 *ctxlock;

	fields.push_back(voidPtr);   //const struct _knh_ExportsAPI_t *api;
	fields.push_back(voidPtr);   //const struct knh_api2_t        *api2;
	fields.push_back(voidPtr);   //char                            trace[16];
	fields.push_back(voidPtr);   //knh_uint_t                      seq;
	// add here for new entry
	fields.push_back(voidPtr);   //struct knh_ExceptionHandler_t  *ehdrNC;
	fields.push_back(voidPtr);   //struct knh_Object_t            *evaled;


	structTy = StructType::get(m->getContext(), fields, /*isPacked=*/false);
	const Type *ctxPtr = PointerType::get(structTy, 0);
	m->addTypeName("struct.context", structTy);
	m->addTypeName("ctx", ctxPtr);
	LLVMTYPE_context = ctxPtr;
	fields.clear();

	// Pointer Definitions
	std::vector<const Type*>args;
	args.push_back(ctxPtr);
	args.push_back(sfpPtr);
	args.push_back(longTy);
	FunctionType* callTy = FunctionType::get(/*Result=*/voidTy,/*Params=*/args,/*isVarArg=*/false);
	LLVMTYPE_fcall = PointerType::get(callTy, 0);
	m->addTypeName("fcall", callTy);

	/* ObjectField */
	fields.push_back(hObjectTy);
	fields.push_back(voidPtr);
	fields.push_back(shortTy);
	fields.push_back(shortTy);
	fields.push_back(LLVMTYPE_fcall);
	structTy = StructType::get(m->getContext(), fields, /*isPacked=*/false);
	LLVMTYPE_Method = PointerType::get(structTy, 0);
	m->addTypeName("Method", LLVMTYPE_Method);
	fields.clear();

}

static Function *build_function(CTX ctx, Module *m, knh_Method_t *mtd)
{
	size_t i;
	knh_ParamArray_t *pa = DP(mtd)->mp;
	knh_class_t retTy = knh_ParamArray_rtype(pa);;
	std::vector<const Type*> argsTy;
	std::string name(knh_getmnname(ctx, SP(mtd)->mn));

	argsTy.push_back(LLVMTYPE_context);
	argsTy.push_back(LLVMTYPE_sfp);
	for (i = 0; i < pa->psize; i++) {
		knh_type_t type = knh_ParamArray_getptype(pa, i);
		argsTy.push_back(convert_type(ctx, type));
	}
	FunctionType *fnTy = FunctionType::get(convert_type(ctx, retTy), argsTy, false);
	return cast<Function>(m->getOrInsertFunction(name, fnTy));
}

#define TC_(type, i) ((IS_Tunbox(type))?(NC_(i)):(OC_(i)))

static Value *create_loadsfp(CTX ctx, IRBuilder<> *builder, Value *v, knh_type_t type, int idx0)
{
	unsigned int idx = TC_(type, 0);
	//Value *idx1  = ConstantInt::get(Type::getInt64Ty(LLVM_CONTEXT()), idx);
	//std::vector<Value*> vec;
	//vec.push_back(idx0);
	//vec.push_back(idx1);
	//v = GetElementPtrInst::Create(v, vec.begin(), vec.end(), "get");
	v = builder->CreateConstGEP2_32(v, (unsigned)idx0, (unsigned)idx, "gep");
	if (IS_Tint(type) || IS_Tbool(type)) {
		v = builder->CreateBitCast(v, PointerType::get(convert_type(ctx, type), 0), "cast");
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
	params.push_back(arg_ctx);
	params.push_back(arg_sfp);
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
		Value *callinst = builder->CreateCall(orig_func, params.begin(), params.end());
		if (retTy != TYPE_void) {
			Value *v = arg_sfp;
			v = create_loadsfp(ctx, builder, v, retTy, K_RTNIDX);
			builder->CreateStore(callinst, v, false/*isVolatile*/);
		}
		builder->CreateRetVoid();
	}
	return f;
}

static void init_first(CTX ctx)
{
	Module *m = new Module("test", LLVM_CONTEXT());
	ConstructObjectStruct(m);
	LLVM_MODULE_SET(ctx, m);
	init_print_func(ctx);
}

static void Init(CTX ctx, knh_Method_t *mtd, knh_Array_t *a)
{
	Module *m = LLVM_MODULE(ctx);
	Function *func;
	BasicBlock *bb;
	IRBuilder<> *builder;

	func = build_function(ctx, m, mtd);
	bb = BasicBlock::Create(LLVM_CONTEXT(), "EntryBlock", func);
	builder = new IRBuilder<>(bb);

	a->ilist[LLVM_IDX_MODULE] = (knh_int_t) m;
	a->ilist[LLVM_IDX_FUNCTION] = (knh_int_t) func;
	a->ilist[LLVM_IDX_BB] = (knh_int_t) bb;
	a->ilist[LLVM_IDX_BUILDER] = (knh_int_t) builder;

	size_t i;
	Function::arg_iterator args = func->arg_begin();
	args++;/*ctx*/
	args++;/*sfp*/
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
	ASM_LastRET(ctx, stmt);
	/* asm for script function is done. */

	/* build wrapper function and compile to native code. */
	ExecutionEngine *ee = EngineBuilder(m).setEngineKind(EngineKind::JIT).create();
	Function *func1 = build_wrapper_func(ctx, m, mtd, func);
#ifdef K_USING_DEBUG
	(*m).dump();
#endif
	f = (knh_Fmethod) ee->getPointerToFunction(func1);
	knh_Method_setFunc(ctx, mtd, f);
	delete LLVM_BUILDER(ctx);
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
		llvmasm::init_first(ctx);
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
static void llvm_PRINTh(CTX ctx, knh_sfp_t *sfp, knh_OutputStream_t *w, knh_flag_t flag, knh_uline_t uline, knh_String_t *msg)
{
	knh_write_ascii(ctx, w, TERM_BNOTE(ctx, LOG_NOTICE));
	if(FLAG_is(flag, K_FLAG_PF_BOL) && FLAG_is(flag, K_FLAG_PF_LINE)) {
		knh_Method_t *mtd = sfp[-1].mtdNC;
		DBG_ASSERT(IS_Method(mtd));
		ULINE_setURI(uline, DP(mtd)->uri);
		knh_write_uline(ctx, w, uline);
	}
	if(IS_bString(msg)) {
		if((msg)->str.len > 0) {
			knh_write_utf8(ctx, w, S_tobytes(msg), !String_isASCII(msg));
		}
	}
	if(FLAG_is(flag, K_FLAG_PF_NAME)) {
		knh_putc(ctx, w, '=');
	}
}

static void llvm_PRINTln(CTX ctx, knh_sfp_t *sfp, knh_OutputStream_t *w, knh_flag_t flag)
{
	if(FLAG_is(flag, K_FLAG_PF_EOL)) {
		knh_write_ascii(ctx, w, TERM_ENOTE(ctx, LOG_NOTICE));
		knh_write_EOL(ctx, w);
	}
	else {
		knh_putc(ctx, w, ',');
		knh_putc(ctx, w, ' ');
	}
}

void llvm_PRINT(CTX ctx, knh_sfp_t *sfp, knh_flag_t flag, knh_uline_t uline, knh_String_t *msg, knh_Object_t *o)
{
	knh_OutputStream_t *w = KNH_STDOUT;
	llvm_PRINTh(ctx, sfp, w, flag, uline, msg);
	knh_write_Object(ctx, w, o, FMT_data);
	llvm_PRINTln(ctx, sfp, w, flag);
}

void llvm_PRINTm(CTX ctx, knh_sfp_t *sfp, knh_String_t *msg, knh_flag_t flag, knh_uline_t uline)
{
	knh_OutputStream_t *w = KNH_STDOUT;
	llvm_PRINTh(ctx, sfp, w, flag, uline, msg);
	if(FLAG_is(flag, K_FLAG_PF_EOL)) {
		knh_write_ascii(ctx, w, TERM_ENOTE(ctx, LOG_NOTICE));
		knh_write_EOL(ctx, w);
	}
}

void llvm_PRINTi(CTX ctx, knh_sfp_t *sfp, knh_flag_t flag, knh_uline_t uline, knh_String_t *msg, knh_int_t n)
{
	knh_OutputStream_t *w = KNH_STDOUT;
	llvm_PRINTh(ctx, sfp, w, flag, uline, msg);
	knh_write_ifmt(ctx, w, K_INT_FMT, n);
	llvm_PRINTln(ctx, sfp, w, flag);
}

void llvm_PRINTf(CTX ctx, knh_sfp_t *sfp, knh_flag_t flag, knh_uline_t uline, knh_String_t *msg, knh_float_t f)
{
	knh_OutputStream_t *w = KNH_STDOUT;
	llvm_PRINTh(ctx, sfp, w, flag, uline, msg);
	knh_write_ffmt(ctx, w, K_FLOAT_FMT, f);
	llvm_PRINTln(ctx, sfp, w, flag);
}

void llvm_PRINTb(CTX ctx, knh_sfp_t *sfp, knh_flag_t flag, knh_uline_t uline, knh_String_t *msg, knh_bool_t b)
{
	knh_OutputStream_t *w = KNH_STDOUT;
	llvm_PRINTh(ctx, sfp, w, flag, uline, msg);
	knh_write_bool(ctx, w, b);
	llvm_PRINTln(ctx, sfp, w, flag);
}

#ifdef __cplusplus
}
#endif
