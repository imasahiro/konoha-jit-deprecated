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
#include <llvm/LLVMContext.h>
#include <llvm/Module.h>
#include <llvm/DerivedTypes.h>
#include <llvm/Constants.h>
#include <llvm/GlobalVariable.h>
#include <llvm/Function.h>
#include <llvm/CallingConv.h>
#include <llvm/BasicBlock.h>
#include <llvm/Instructions.h>
#include <llvm/InlineAsm.h>
#include <llvm/Support/FormattedStream.h>
#include <llvm/Support/MathExtras.h>
#include <llvm/Pass.h>
#include <llvm/PassManager.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/Assembly/PrintModulePass.h>
#include <algorithm>

#undef HAVE_SYS_TYPES_H
#include "commons.h"
#define ASM_PREFIX llvmasm::
#define LLVM_TODO(str) {\
	fprintf(stderr, "%s\n", str);\
	asm volatile("int3");\
}
#define _UNUSED_ __attribute__((unused))

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

namespace llvmasm {
/* ------------------------------------------------------------------------ */
/* kcode */

#define ASM(X, ...)
#define ASMop(X, ...)
static void Tn_asm(CTX ctx, knh_Stmt_t *stmt, size_t n, knh_type_t reqt, int local);
static int _BLOCK_asm(CTX ctx, knh_Stmt_t *stmtH, knh_type_t reqt, int sfpidx);

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

static void ASM_RET(CTX ctx, knh_Stmt_t *stmt)
{
	LLVM_TODO("ASM_RET");
}

/* ------------------------------------------------------------------------ */
/* [Gamma] */

#define TT_isSFPIDX(tk)   (TT_(tk) == TT_LOCAL || TT_(tk) == TT_FUNCVAR)
#define Token_index(tk)   Token_index_(ctx, tk)
static int Token_index_(CTX ctx, knh_Token_t *tk)
{
	return (int)(tk)->index + ((TT_(tk) == TT_LOCAL) ? DP(ctx->gma)->ebpidx : 0);
}

static void ASM_BOX2(CTX ctx, knh_type_t reqt, knh_type_t atype, int a)
{
	LLVM_TODO("ASM_BOX2");
}

static void ASM_UNBOX(CTX ctx, knh_type_t atype, int a)
{
	LLVM_TODO("ASM_UNBOX");
}

static void ASM_MOVL(CTX ctx, knh_type_t reqt, int sfpidx, knh_type_t ltype, int local)
{
	LLVM_TODO("ASM_MOVL");
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

static void ASM_SMOV(CTX ctx, knh_type_t atype, int a/*flocal*/, knh_Token_t *tkb)
{
	knh_type_t btype = SP(tkb)->type;
	switch(TT_(tkb)) {
		case TT_ASIS:
			break;
		case TT_NULL/*DEFVAL*/: {
			knh_class_t cid = (tkb)->cid;
			knh_Object_t *v = KNH_NULVAL(cid);
			if(v != ClassTBL(cid)->defnull) {
				ASM(TR, OC_(a), SFP_(a), RIX_(a-a), ClassTBL(cid), _NULVAL);
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
				ASM(NSET, NC_(a), O_data((tkb)->data));
			}
			else if(IS_Tnumbox(atype)) {
				ASM(NSET, NC_(a), O_data((tkb)->data));
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
				ASM(NMOV, NC_(a), NC_(b));
				ASM_BOX2(ctx, atype, btype, a);
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

static int _OPR_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	LLVM_TODO("Operator");
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
	LLVM_TODO("CALLPARAMs");
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
	LLVM_TODO("CALL1");
	return 0;
}

static int _NEW_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	LLVM_TODO("NEW");
	return 0;
}

static int _TCAST_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	LLVM_TODO("TCAST");
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
	LLVM_TODO("LET");
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
	LLVM_TODO("IF");
	return 0;
}

static int _SWITCH_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	LLVM_TODO("SWITCH");
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
	LLVM_TODO("WHILE");
	return 0;
}

static int _DO_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt _UNUSED_, int sfpidx _UNUSED_)
{
	LLVM_TODO("DO");
	return 0;
}

static int _FOR_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt _UNUSED_, int sfpidx _UNUSED_)
{
	LLVM_TODO("FOR");
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

static int _RETURN_asm(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt _UNUSED_, int sfpidx _UNUSED_)
{
	ASM_FINALLY(ctx);
	LLVM_TODO("RETURN");
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

#define _ALLOW_asm _EXPR_asm
#define _DENY_asm _EXPR_asm
#define _CASE_asm _EXPR_asm
#define _REGISTER_asm _EXPR_asm
#define _DECL_asm _EXPR_asm
#define _ACALL_asm _EXPR_asm
#include "codeasm.h"
} /* namespace llvm */


void LLVMMethod_asm(CTX ctx, knh_Method_t *mtd, knh_Stmt_t *stmtP, knh_type_t ittype, knh_Stmt_t *stmtB, knh_Ftyping typing)
{
	typing(ctx, mtd, stmtP, ittype, stmtB);
	DBG_ASSERT(knh_Array_size(DP(ctx->gma)->insts) == 0);
	SP(ctx->gma)->uline = SP(stmtB)->uline;
	if(Method_isStatic(mtd) && Gamma_hasFIELD(ctx->gma)) {
		ASM(TR, OC_(0), SFP_(0), RIX_(0), ClassTBL(DP(ctx->gma)->this_cid), _NULVAL);
	}
	llvmasm::_BLOCK_asm(ctx, stmtB, knh_ParamArray_rtype(DP(mtd)->mp), 0);
	ASM(RET);
}

#ifdef __cplusplus
}
#endif
