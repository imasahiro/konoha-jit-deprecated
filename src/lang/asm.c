/****************************************************************************
 * KONOHA COPYRIGHT, LICENSE NOTICE, AND DISCRIMER
 *
 * Copyright (c) 2006-2010, Kimio Kuramitsu <kimio at ynu.ac.jp>
 *           (c) 2008-      Konoha Team konohaken@googlegroups.com
 * All rights reserved.
 *
 * You may choose one of the following two licenses when you use konoha.
 * See www.konohaware.org/license.html for further information.
 *
 * (1) GNU Lesser General Public License 3.0 (with KONOHA_UNDER_LGPL3)
 * (2) Konoha Software Foundation License 1.0
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/* ************************************************************************ */

#define USE_STEXT 1
#define USE_B     1
#define USE_bytes_equals    1
#define USE_cwb_open      1

#include"commons.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ======================================================================== */
/* kcode */

#ifndef OPCODE_LOOPJMPF
#define OPCODE_LOOPJMPF  OPCODE_JMPF
#endif

#define K_INLINECODE    (10)

static int knh_Gamma_inTry(Ctx *ctx);
static knh_KLRInst_t *new_KLRInstLABEL(Ctx *ctx);
static void TERMs_asm(Ctx *ctx, knh_Stmt_t *stmt, size_t n, knh_type_t reqt, int local);
static void knh_asmop(Ctx *ctx, knh_opset_t *op, size_t opsize);
static void knh_Stmt_asmBLOCK(Ctx *ctx, knh_Stmt_t *stmtH, knh_type_t reqt);

#if defined(K_USING_RCGC)
static void knh_ftraverse_inc(Ctx* ctx, Object *o)
{
	knh_Object_RCinc(o);
}
#endif

static knh_KLRInst_t* new_KLRInst(Ctx *ctx, knh_opset_t *pc, size_t opsize)
{
	knh_KLRInst_t *inst = new_(KLRInst);
	knh_memcpy(inst->op, pc, opsize);
#if defined(K_USING_RCGC)
	knh_opcode_traverse(ctx, inst->op, knh_ftraverse_inc);
#endif
	inst->opcode = pc->opcode;
	return inst;
}

static knh_KLRInst_t* knh_InstList_lastNULL(knh_Array_t *insts)
{
	size_t size = knh_Array_size(insts);
	if(size > 0) {
		return knh_KLRINSTs_n(insts, size-1);
	}
	return NULL;
}

static void knh_KLRInst_rename(knh_KLRInst_t *opl, knh_opcode_t opcode)
{
	DBG_P("RENAME: %s => %s", knh_opcode_tochar(opl->opcode), knh_opcode_tochar(opcode));
	opl->opcode = opcode;
	opl->op->opcode = opcode;
}

/* ------------------------------------------------------------------------ */
/* [func] */

static KLRAPI(void) _NEW(Ctx *ctx, knh_sfp_t *sfp, knh_sfpidx_t c, knh_class_t cid)
{
	klr_mov(ctx, sfp[c].o, new_Object_init2(ctx, 0, knh_class_bcid(cid), cid));
}
static KLRAPI(void) _NULVAL(Ctx *ctx, knh_sfp_t *sfp, knh_sfpidx_t c, knh_class_t cid)
{
	klr_mov(ctx, sfp[c].o, KNH_NULVAL(cid));
	sfp[c].data = knh_Object_data(sfp[c].o);
}
static KLRAPI(void) _SYS(Ctx *ctx, knh_sfp_t *sfp, knh_sfpidx_t c, knh_class_t cid)
{
	klr_mov(ctx, sfp[c].o, KNH_SYS(ctx, (int)sfp[0].ivalue));
}
static KLRAPI(void) _PROP(Ctx *ctx, knh_sfp_t *sfp, knh_sfpidx_t c, knh_class_t cid)
{
	Object *v = (Object*)knh_getPropertyNULL(ctx, S_tobytes(sfp[0].s));
	if(v == NULL) v = KNH_NULVAL(cid);
	klr_mov(ctx, sfp[c].o, v);
}
static KLRAPI(void) _VARGS(Ctx *ctx, knh_sfp_t *sfp, knh_sfpidx_t c, knh_class_t cid)
{
	knh_Array_t *a = new_Array(ctx, cid, ctx->esp - (sfp - 1));
	a->api->add(ctx, a, sfp - 1);
	klr_mov(ctx, sfp[c].o, a);
}
static KLRAPI(void) _bBOX(Ctx *ctx, knh_sfp_t *sfp, knh_sfpidx_t c, knh_class_t cid)
{
	klr_mov(ctx, sfp[c].o, sfp[0].bvalue ? KNH_TRUE : KNH_FALSE);
}
static KLRAPI(void) _BOX(Ctx *ctx, knh_sfp_t *sfp, knh_sfpidx_t c, knh_class_t cid)
{
	Object *v = new_Object_boxing(ctx, cid, sfp);
	klr_mov(ctx, sfp[c].o, v);
}
static KLRAPI(void) _ERR(Ctx *ctx, knh_sfp_t *sfp, knh_sfpidx_t c, knh_class_t cid)
{
	if(IS_bString(sfp[0].o)) {
		knh_Context_setThrowingException(ctx, new_Exception(ctx, sfp[0].s));
	}
	else {
		DBG_ASSERT(IS_Exception(sfp[0].o));
		knh_Context_setThrowingException(ctx, sfp[0].e);
	}
}
static KLRAPI(void) _CHKTYPE(Ctx *ctx, knh_sfp_t *sfp, knh_sfpidx_t c, knh_class_t cid)
{
	if(!knh_Object_opTypeOf(ctx, sfp[0].o, cid)) {
		knh_Context_setThrowingException(ctx, new_Exception__T(ctx, "ClassCast!!"));
	}
}
static KLRAPI(knh_Method_t*) klr_setMethod(Ctx *ctx, knh_sfp_t *sfp, int thisidx, knh_Method_t *mtd)
{
	return mtd;
}
static KLRAPI(knh_Method_t*) klr_lookupMethod(Ctx *ctx, knh_sfp_t *sfp, int thisidx, knh_Method_t *mtd)
{
	knh_class_t cid = knh_Object_cid(sfp[thisidx].o);
	if(cid != DP(mtd)->cid) {
		mtd = knh_lookupMethod(ctx, cid, DP(mtd)->mn);
	}
	return mtd;
}
static KLRAPI(knh_Method_t*) klr_checkParams(Ctx *ctx, knh_sfp_t *sfp, int thisidx, knh_Method_t *mtd)
{
	knh_class_t cid = knh_Object_cid(sfp[thisidx].o);
	if(cid != DP(mtd)->cid) {
		mtd = knh_lookupMethod(ctx, cid, DP(mtd)->mn);
	}
	knh_stack_typecheck(ctx, sfp + thisidx, mtd, NULL);
	return mtd;
}
//static KLRAPI(int) klr_inext(Ctx *ctx, knh_sfp_t *sfp, int c, knh_class_t reqc)
//{
//	L_TAIL:;
//	if((sfp[0].it)->fnext_1(ctx, sfp, c)) {
//		knh_class_t cid = knh_Object_cid(sfp[c].o);
//		if(cid != reqc && !knh_class_instanceof(ctx, reqc, cid)) {
//			goto L_TAIL;
//		}
//		return 1;
//	}
//	else {
//		KNH_SETv(ctx, sfp[c].o, KNH_NULVAL(reqc));
//		return 0;
//	}
//}
//static KLRAPI(void) klr_size(Ctx *ctx, knh_sfp_t *sfp, int c, int a)
//{
//	sfp[c].ivalue = (sfp[a].a)->size;
//}
static KLRAPI(int) klr_isskip(Ctx *ctx, knh_sfp_t *sfp, int a)
{
	return 0;
}

/* ======================================================================== */
/* [Gamma] */

static void knh_Gamma_gc(Ctx *ctx)
{
	KNH_SETv(ctx, DP(ctx->gma)->mtd, KNH_NULL);
	knh_Array_clear(ctx, DP(ctx->gma)->lstacks, 0);
	knh_Array_clear(ctx, DP(ctx->gma)->insts, 0);
}

/* ------------------------------------------------------------------------ */
/* new_KLRCode */

static knh_KLRCode_t* new_KLRCode(Ctx *ctx, knh_Array_t *ia)
{
	size_t i, inst_size = knh_Array_size(ia), codesize = 0;
	knh_KLRInst_t *inst;
	// count opcode size
	for(i = 0; i < inst_size; i++) {
		inst = knh_KLRINSTs_n(ia, i);
		if(inst->opcode == OPCODE_LABEL) {
			klr_LABEL_t *iLABEL = (klr_LABEL_t*)inst->op;
			if(iLABEL->id == 0) continue;
		}
		if(inst->opcode == OPCODE_JMP) {
			size_t j;
			knh_KLRInst_t *opLABEL = ((klr_JMP_t*)inst->op)->lb;
			for(j = i + 1; j < inst_size; j++) {
				knh_KLRInst_t *opNEXT = knh_KLRINSTs_n(ia, j);
				if(opNEXT->opcode != OPCODE_LABEL) break;
				if(opNEXT == opLABEL) {
					knh_KLRInst_rename(inst, OPCODE_NOP); break;
				}
			}
		}
		if(inst->opcode == OPCODE_NOP) continue;
		codesize += sizeof(knh_opset_t);
	}
	{
		knh_KLRCode_t *kcode = new_(KLRCode);
		knh_opset_t *pc, *pc_last = NULL;
		int isCHKSTACK = 0;
		SP(kcode)->uri = SP(ctx->gma)->uri;
		SP(kcode)->codesize = codesize;
		SP(kcode)->code = (knh_opset_t*)KNH_MALLOC(ctx, SP(kcode)->codesize);
		pc = SP(kcode)->code;
		for(i = 0; i < inst_size; i++) {
			inst = knh_KLRINSTs_n(ia, i);
			inst->code_pos = pc;
			if(inst->opcode == OPCODE_NOP) continue;
			if(inst->opcode == OPCODE_LABEL) {
				klr_LABEL_t *iLABEL = (klr_LABEL_t*)inst->op;
				if(iLABEL->id == 0) continue;
			}
			if(inst->opcode == OPCODE_RET) {
				pc_last = pc;
			}
			pc += 1;
		}

	// if hasjump(inst->opcode), set where jump to.
		pc = SP(kcode)->code;
		for(i = 0; i < inst_size; i++) {
			inst = knh_KLRINSTs_n(ia, i);
			if(inst->opcode == OPCODE_NOP) continue;
			if(inst->opcode == OPCODE_LABEL) {
				klr_LABEL_t *iLABEL = (klr_LABEL_t*)inst->op;
				isCHKSTACK = 0;
				if(iLABEL->id == 0) continue;
			}
			if(knh_opcode_hasjump(inst->opcode)) {
				klr_JMP_t *opJMP = (klr_JMP_t*)(inst->op);
				(opJMP)->jumppc = (opJMP->lb)->code_pos;
				if((opJMP)->jumppc == pc_last && opJMP->opcode == OPCODE_JMP) {
					opJMP->opcode = OPCODE_JMP_;
				}
			}
			if(isCHKSTACK == 1 && inst->opcode == OPCODE_VCALL) {
				inst->op->opcode = OPCODE_VCALL_;
			}
			knh_memcpy(pc, inst->op, sizeof(knh_opset_t));
			knh_opcode_traverse(ctx, pc, knh_ftraverse_inc);
			pc += 1;
			if(inst->opcode == OPCODE_CALL || inst->opcode == OPCODE_VCALL) {
				isCHKSTACK = 1;
			}
		}
		return kcode;
	}
}

/* ------------------------------------------------------------------------ */

METHOD knh_Fmethod_runVM(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	DBG_ASSERT(IS_Method(sfp[K_MTDIDX].callmtd));
	DBG_P("invoking Konoha_VM");
	knh_VirtualMachine_run(ctx, sfp, CODE_VEXEC);
}

static void knh_Method_threadCode(Ctx *ctx, knh_Method_t *mtd, knh_KLRCode_t *kcode)
{
	knh_Method_setFunc(ctx, mtd, knh_Fmethod_runVM);
	if(knh_Method_isObjectCode(mtd)) {
		KNH_SETv(ctx, DP(mtd)->kcode, kcode);
	}else {
		knh_Method_setObjectCode(mtd, 1);
		KNH_INITv(DP(mtd)->kcode, kcode);
	}
	(mtd)->pc_start = knh_VirtualMachine_run(ctx, ctx->esp + 1, SP(kcode)->code);
	if(knh_isSystemVerbose()) {
		knh_Method_t *mtdf = knh_lookupFormatter(ctx, CLASS_Method, MN__dump);
		knh_write_Object(ctx, KNH_STDOUT, ctx->esp, &mtdf, UP(mtd));
		knh_write_EOL(ctx, KNH_STDOUT);
	}
}

/* @see optimizer.c */

static void knh_Gamma_compile(Ctx *ctx)
{
	knh_Method_t *mtd = DP(ctx->gma)->mtd;
	knh_Array_t *insts = DP(ctx->gma)->insts;
	knh_KLRCode_t *kcode = new_KLRCode(ctx, insts);
	DBG_ASSERT(IS_Method(mtd));
	knh_Method_threadCode(ctx, mtd, kcode);
	if(knh_Gamma_isCancelled(ctx->gma)) {
		if(DP(mtd)->mn != MN_LAMBDA) {
			knh_Gamma_perror(ctx, KERR_DWARN, "abstract? %C.%M", DP(mtd)->cid, DP(mtd)->mn);
		}
		knh_Method_toAbstract(ctx, mtd);
	}
	knh_Gamma_gc(ctx);
}

/* ------------------------------------------------------------------------ */
/* [ASM] */

static void knh_asmop(Ctx *ctx, knh_opset_t *op, size_t opsize)
{
	size_t n = knh_Array_size(DP(ctx->gma)->insts);
	if(n > 0) {
		knh_KLRInst_t *instP = knh_KLRINSTs_n(DP(ctx->gma)->insts, n-1);
		if(op->opcode == OPCODE_NMOV) {
			if(instP->opcode == OPCODE_NMOV) {
				klr_NNMOV_t *opMOV = (klr_NNMOV_t*)instP->op;
				opMOV->c = ((klr_NMOV_t*)op)->a;
				opMOV->d = ((klr_NMOV_t*)op)->b;
				knh_KLRInst_rename(instP, OPCODE_NNMOV);
				return;
			}
			if(instP->opcode == OPCODE_OMOV) {
				klr_ONMOV_t *opMOV = (klr_ONMOV_t *)instP->op;
				opMOV->c = ((klr_NMOV_t*)op)->a;
				opMOV->d = ((klr_NMOV_t*)op)->b;
				knh_KLRInst_rename(instP, OPCODE_ONMOV);
				return;
			}
		}
		if(op->opcode == OPCODE_OMOV) {
			if(instP->opcode == OPCODE_OMOV) {
				klr_OOMOV_t *opMOV = (klr_OOMOV_t*)instP->op;
				opMOV->c = ((klr_OMOV_t*)op)->a;
				opMOV->d = ((klr_OMOV_t*)op)->b;
				knh_KLRInst_rename(instP, OPCODE_OOMOV);
				return;
			}
			if(instP->opcode == OPCODE_OMOV) {
				klr_ONMOV_t *opMOV = (klr_ONMOV_t *)instP->op;
				opMOV->c = opMOV->a;
				opMOV->d = opMOV->b;
				opMOV->a = ((klr_OMOV_t*)op)->a;
				opMOV->b = ((klr_OMOV_t*)op)->b;
				knh_KLRInst_rename(instP, OPCODE_ONMOV);
				return;
			}
		}
		if((instP)->opcode == op->opcode && op->opcode == OPCODE_JMP) {
			DBG_P("PEEPHOLE: removed meaningless JMP: %s", knh_opcode_tochar(op->opcode));
			return;
		}
		if((instP)->opcode == op->opcode && op->opcode != OPCODE_LABEL) {
			knh_opset_t *opP = instP->op;
			size_t i, size = knh_opcode_size(op->opcode);
			for(i = 0; i < size; i++) {
				if(opP->data[i] != op->data[i]) goto L_OK;
			}
			DBG_P("PEEPHOLE: removed same one: %s", knh_opcode_tochar(op->opcode));
			return;
		}
		L_OK:;
		/* LEVEL3*/
#ifdef OPCODE_iINC
		if(op->opcode == OPCODE_iADDn) {
			klr_iADDn_t *opN = (klr_iADDn_t*)op;
			if(opN->a == opN->c && opN->n == 1) {
				opN->opcode = OPCODE_iINC;
			}
		}
		if(op->opcode == OPCODE_iSUBn) {
			klr_iSUBn_t *opN = (klr_iSUBn_t*)op;
			if(opN->a == opN->c && opN->n == 1) {
				opN->opcode = OPCODE_iDEC;
			}
		}
#endif/*OPCODE_iDEC*/
#ifdef OPCODE_iJEQ
		if(op->opcode == OPCODE_JMPF) {
			knh_opset_t *opP = instP->op;
			if(OPCODE_iEQ <= opP->opcode && opP->opcode <= OPCODE_iGTE) {
				klr_iJEQ_t *opN = (klr_iJEQ_t*)opP;
				knh_sfpidx_t a = ((klr_iEQ_t*)opP)->a;
				knh_sfpidx_t b = ((klr_iEQ_t*)opP)->b;
				opN->lb = ((klr_JMP_t*)op)->lb;
				opN->a = a; opN->b = b;
				knh_KLRInst_rename(instP, OPCODE_iJEQ + ((opP)->opcode - OPCODE_iEQ));
				return;
			}
			if(OPCODE_iEQn <= opP->opcode && opP->opcode <= OPCODE_iGTEn) {
				klr_iJEQn_t *opN = (klr_iJEQn_t*)opP;
				knh_sfpidx_t a = ((klr_iEQn_t*)opP)->a;
				knh_int_t n = ((klr_iEQn_t*)opP)->n;
				opN->lb = ((klr_JMP_t*)op)->lb;
				opN->a = a; opN->n = n;
				knh_KLRInst_rename(instP, OPCODE_iJEQn + ((opP)->opcode - OPCODE_iEQn));
				return;
			}
			if(OPCODE_fEQ <= opP->opcode && opP->opcode <= OPCODE_fGTE) {
				klr_fJEQ_t *opN = (klr_fJEQ_t*)opP;
				knh_sfpidx_t a = ((klr_fEQ_t*)opP)->a;
				knh_sfpidx_t b = ((klr_fEQ_t*)opP)->b;
				opN->lb = ((klr_JMP_t*)op)->lb;
				opN->a = a; opN->b = b;
				knh_KLRInst_rename(instP, OPCODE_fJEQ + ((opP)->opcode - OPCODE_fEQ));
				return;
			}
			if(OPCODE_fEQn <= opP->opcode && opP->opcode <= OPCODE_fGTEn) {
				klr_fJEQn_t *opN = (klr_fJEQn_t*)opP;
				knh_sfpidx_t a = ((klr_fEQn_t*)opP)->a;
				knh_int_t n = ((klr_fEQn_t*)opP)->n;
				opN->lb = ((klr_JMP_t*)op)->lb;
				opN->a = a; opN->n = n;
				knh_KLRInst_rename(instP, OPCODE_fJEQn + ((opP)->opcode - OPCODE_fEQn));
				return;
			}
		}
#endif
	}
	{
		knh_KLRInst_t *inst = new_KLRInst(ctx, op, opsize);
		inst->op->line = SP(ctx->gma)->line;
		inst->line = SP(ctx->gma)->line;
		knh_Array_add(ctx, DP(ctx->gma)->insts, inst);
	}
}

#define KNH_RENAME(opcode, opcode2)  knh_Gamma_renameOPCODE(ctx, opcode, opcode2)

static void knh_Gamma_renameOPCODE(Ctx *ctx, int opcode, int opcode2)
{
	if(opcode != opcode2) {
		size_t n = knh_Array_size(DP(ctx->gma)->insts);
		knh_KLRInst_t *instP = knh_KLRINSTs_n(DP(ctx->gma)->insts, n-1);
		DBG_ASSERT(instP->opcode == opcode);
		knh_KLRInst_rename(instP, opcode2);
	}
}

static void KNH_ASM_BOX(Ctx *ctx, knh_type_t reqt, knh_type_t atype, int a)
{
	knh_class_t cid = CLASS_type(atype);
	knh_class_t bcid = ClassTable(cid).bcid;
	if(bcid == CLASS_Boolean || bcid == CLASS_Int || bcid == CLASS_Float) {
		knh_class_t rcid = ClassTable(CLASS_type(reqt)).bcid;
		if(rcid != bcid && reqt != TYPE_void) {
			//DBG_P("reqt=%s, atype=%s", TYPEN(reqt), TYPEN(atype));
			KNH_ASM(TR, a, a, cid, _BOX);
		}
	}
}

static void KNH_ASM_UNBOX(Ctx *ctx, knh_type_t atype, int a)
{
	knh_class_t bcid = knh_class_bcid(CLASS_type(atype));
	if(bcid == CLASS_Boolean || bcid == CLASS_Int || bcid == CLASS_Float) {
		KNH_ASM(UNBOX, a, a);
	}
}

#define ASML(idx) (idx < DP(ctx->gma)->espidx) ? (DP(ctx->gma)->espidx) : idx

static void KNH_ASM_MOVL(Ctx *ctx, knh_type_t reqt, int sfpidx, knh_type_t ltype, int local)
{
	if(sfpidx < DP(ctx->gma)->espidx) {
		if(IS_Tunbox(ltype)) {
			KNH_ASM(NMOV, (sfpidx), (local));
		}
		else {
#if defined(K_USING_RCGC)
			KNH_ASM(SWAP, (sfpidx), (local));
#else
			KNH_ASM(MOV, (sfpidx), (local));
#endif
		}
	}
}

static int TERMs_put(Ctx *ctx, knh_Stmt_t *stmt, size_t n, knh_type_t reqt, int sfpidx)
{
	knh_Token_t *tk = DP(stmt)->tokens[n];
	if(IS_Token(tk) && TT_(tk) == TT_LOCAL) {
		return DP(tk)->index;
	}
	TERMs_asm(ctx, stmt, n, reqt, sfpidx);
	return sfpidx;
}

static void KNH_ASM_SMOVx(Ctx *ctx, knh_type_t atype, int a, knh_type_t btype, knh_sfx_t bx)
{
	if(IS_Tint(btype)) {
		KNH_ASM(iMOVx, (a), bx);
		KNH_ASM_BOX(ctx, atype, btype, (a));
	}
	else if(IS_Tfloat(btype)) {
		KNH_ASM(fMOVx, (a), bx);
		KNH_ASM_BOX(ctx, atype, btype, (a));
	}
	else if(IS_Tbool(btype)) {
		KNH_ASM(bMOVx, (a), bx);
		KNH_ASM_BOX(ctx, atype, btype, (a));
	}
	else {
		KNH_ASM(OMOVx, (a), bx);
		KNH_ASM_UNBOX(ctx, atype, a);
	}
}

static void KNH_ASM_SMOV(Ctx *ctx, knh_type_t atype, int a, knh_Token_t *tkb)
{
	knh_type_t btype = SP(tkb)->type;
	switch(TT_(tkb)) {
		case TT_NULL/*DEFVAL*/: {
			knh_class_t cid = DP(tkb)->cid;
			knh_Object_t *v = KNH_NULVAL(cid);
			if(v != ClassTable(cid).defnull) {
				KNH_ASM(TR, a, a, cid, _NULVAL);
				break;
			}
			KNH_SETv(ctx, DP(tkb)->data, v);
			goto L_CONST;
		}
		case TT_CID:
			KNH_SETv(ctx, DP(tkb)->data, new_Type(ctx, DP(tkb)->cid));
		case TT_CONST: L_CONST:; {
			Object *v = DP(tkb)->data;
			if(IS_Tunbox(atype)) {
				KNH_ASM(NSET, (a), knh_Object_data(DP(tkb)->data));
			}
			else {
				KNH_ASM(OSET, (a), v);
				KNH_ASM_UNBOX(ctx, SP(tkb)->type, a);
			}
			break;
		}
		case TT_LOCAL: {
			int b = (int)DP(tkb)->index;
			knh_type_t btype = SP(tkb)->type;
			if(IS_Tunbox(btype)) {
				KNH_ASM(NMOV, (a), (b));
				KNH_ASM_BOX(ctx, atype, btype, a);
			}
			else {
				KNH_ASM(OMOV, (a), (b));
			}
			break;
		}
		case TT_FIELD: {
			int b = (int)DP(tkb)->index;
			knh_sfx_t bx = {(0), (size_t)b};
			KNH_ASM_SMOVx(ctx, atype, a, btype, bx);
			break;
		}
		case TT_SYSVAL: {
			knh_ushort_t sysid = DP(tkb)->index;
			KNH_ASM(NSET, (a), (knh_int_t)sysid);
			KNH_ASM(TR, a, a, CLASS_type(atype), _SYS);
			break;
		}
		case TT_PROPN: {
			KNH_ASM(OSET, (a), DP(tkb)->data);
			KNH_ASM(TR, a, a, CLASS_type(btype), _PROP);
			break;
		}
		default: {
			DBG_P("unknown TT=%s", TT_tochar(TT_(tkb)));
			DBG_ABORT();
		}
	}/*switch*/
}

static void KNH_ASM_XMOVx(Ctx *ctx, knh_type_t atype, knh_sfx_t ax, knh_type_t btype, knh_sfx_t bx)
{
	int espidx = DP(ctx->gma)->espidx;
	if(IS_Tint(atype)) {
		if(IS_Tint(btype)) {
			KNH_ASM(iMOVx, espidx, bx);
		}
		else {
			KNH_ASM(OMOVx, espidx, bx);
			KNH_ASM_UNBOX(ctx, atype, espidx);
		}
		KNH_ASM(XIMOV, ax, espidx);
	}
	else if(IS_Tfloat(atype)) {
		if(IS_Tfloat(btype)) {
			KNH_ASM(fMOVx, espidx, bx);
		}
		else {
			KNH_ASM(OMOVx, espidx, bx);
			KNH_ASM_UNBOX(ctx, atype, espidx);
		}
		KNH_ASM(XFMOV, ax, espidx);
	}
	else if(IS_Tbool(atype)) {
		if(IS_Tbool(btype)) {
			KNH_ASM(bMOVx, espidx, bx);
		}
		else {
			KNH_ASM(OMOVx, espidx, bx);
			KNH_ASM_UNBOX(ctx, atype, espidx);
		}
		KNH_ASM(XBMOV, ax, espidx);
	}
	else if(IS_Tint(btype)) { // Any a = b; // int b;
		KNH_ASM(iMOVx, espidx, bx);
		KNH_ASM(TR, espidx, espidx, CLASS_type(btype), _BOX);
		KNH_ASM(XMOV, ax, espidx);
	}
	else if(IS_Tfloat(btype)) { // Any a = b; // float b;
		KNH_ASM(fMOVx, espidx, bx);
		KNH_ASM(TR, espidx, espidx, CLASS_type(btype), _BOX);
		KNH_ASM(XMOV, ax, espidx);
	}
	else if(IS_Tbool(btype)) { // Any a = b; // boolean b;
		KNH_ASM(bMOVx, espidx, bx);
		KNH_ASM(TR, espidx, espidx, CLASS_type(btype), _bBOX);
		KNH_ASM(XMOV, ax, espidx);
	}
	else {
		KNH_ASM(XMOVx, ax, bx);
	}
}

static void KNH_ASM_XMOV(Ctx *ctx, knh_type_t atype, int a, size_t an, knh_Token_t *tkb)
{
	knh_sfx_t ax = {(a), an};
	knh_type_t btype = SP(tkb)->type;
	long espidx = -1;
	DBG_ASSERT(DP(ctx->gma)->espidx != -1);
	switch(TT_(tkb)) {
		case TT_NULL/*DEFVAL*/: {
			knh_class_t cid = DP(tkb)->cid;
			knh_Object_t *v = KNH_NULVAL(cid);
			if(v != ClassTable(cid).defnull) {
				espidx = DP(ctx->gma)->espidx;
				KNH_ASM(TR, espidx, espidx, cid, _NULVAL);
				break;
			}
			KNH_SETv(ctx, DP(tkb)->data, v);
			goto L_CONST;
		}
		case TT_CID:
			KNH_SETv(ctx, DP(tkb)->data, new_Type(ctx, DP(tkb)->cid));
		case TT_CONST: L_CONST:; {
			Object *v = DP(tkb)->data;
			if(IS_Tunbox(atype)) {
				espidx = DP(ctx->gma)->espidx;
				KNH_ASM(NSET, espidx, knh_Object_data(v));
			}
			else {
				KNH_ASM(XOSET, ax, v);
			}
			break;
		}
		case TT_LOCAL: {
			int b = (int)DP(tkb)->index;
			if(IS_Tint(atype)) {
				KNH_ASM(XIMOV, ax, (b));
				break;
			}
			if(IS_Tfloat(atype)) {
				KNH_ASM(XFMOV, ax, (b));
				break;
			}
			if(IS_Tbool(atype)) {
				KNH_ASM(XBMOV, ax, (b));
				break;
			}
			KNH_ASM_BOX(ctx, atype, btype, b);
			KNH_ASM(XMOV, ax, (b));
			break;
		}
		case TT_FIELD: {
			int b = (int)DP(tkb)->index;
			knh_sfx_t bx = {(knh_sfpidx_t)0, (size_t)b};
			KNH_ASM_XMOVx(ctx, atype, ax, btype, bx);
			break;
		}
		case TT_SYSVAL: {
			knh_ushort_t sysid = DP(tkb)->index;
			espidx = DP(ctx->gma)->espidx;
			KNH_ASM(NSET, espidx, (knh_int_t)sysid);
			KNH_ASM(TR, espidx, espidx, CLASS_type(atype), _SYS);
			break;
		}
		case TT_PROPN: {
			espidx = DP(ctx->gma)->espidx;
			KNH_ASM(OSET, espidx, DP(tkb)->data);
			KNH_ASM(TR, espidx, espidx, CLASS_type(atype), _PROP);
			break;
		}
		default: {
			DBG_P("unknown TT=%d", TT_(tkb));
			DBG_ABORT();
		}
	}/*switch*/
	if(espidx != -1) {
		if(IS_Tint(atype)) {
			KNH_ASM(XIMOV, ax, espidx);
		}
		else if(IS_Tfloat(atype)) {
			KNH_ASM(XFMOV, ax, espidx);
		}
		else if(IS_Tbool(atype)) {
			KNH_ASM(XBMOV, ax, espidx);
		}
		else {
			KNH_ASM(XMOV, ax, espidx);
		}
	}
}

static void KNH_ASM_MOV(Ctx *ctx, knh_Token_t *tka, knh_Token_t *tkb)
{
	DBG_ASSERT(knh_Token_isTyped(tka));
	DBG_ASSERT(IS_Token(tkb));
	DBG_ASSERT(knh_Token_isTyped(tkb));
	knh_type_t atype = SP(tka)->type;
	if(TT_(tka) == TT_LOCAL) {
		KNH_ASM_SMOV(ctx, atype, (int)DP(tka)->index, tkb);
	}
	else {
		DBG_ASSERT(TT_(tka) == TT_FIELD);
		int an = (int)DP(tka)->index;
		KNH_ASM_XMOV(ctx, atype, 0, an, tkb);
	}
}

/* ------------------------------------------------------------------------ */
/* [JMP] */

static knh_KLRInst_t* new_KLRInstLABEL(Ctx *ctx)
{
	klr_LABEL_t op_ = {TADDR OPCODE_LABEL ASMLINE, 0, TS_EMPTY};
	return new_KLRInst(ctx, (knh_opset_t*)(&op_), sizeof(klr_LABEL_t));
}

static void KNH_ASM_LABEL(Ctx *ctx, knh_KLRInst_t* label)
{
	knh_KLRInst_t *iLAST = knh_InstList_lastNULL(DP(ctx->gma)->insts);
	if(iLAST != NULL && iLAST->opcode == OPCODE_JMP) {
		klr_JMP_t *op = (klr_JMP_t*)(iLAST)->op;
		if(op->lb == label) { /* PEEPHOLE*/
			knh_Array_t *a = DP(ctx->gma)->insts;
			KNH_SETv(ctx, (a)->list[knh_Array_size(a)-1], label);
			DBG_P("PEEPHOLE: JMP REDUCTION");
			return;
		}
	}
	label->line = SP(ctx->gma)->line;
	knh_Array_add(ctx, DP(ctx->gma)->insts, label);
}

static void KNH_ASM_RET(Ctx *ctx)
{
	knh_KLRInst_t *lbEND = knh_KLRINSTs_n(DP(ctx->gma)->lstacks, 2);
	KNH_ASM(JMP, {lbEND});
}

//static int TERMs_isCALLISNUL(knh_Stmt_t *stmt, size_t n)
//{
//	if(IS_Stmt(DP(stmt)->stmts[n])) {
//		knh_Token_t *tk = DP(DP(stmt)->stmts[n])->tokens[0];
//		return (IS_Token(tk) && IS_Method(DP(tk)->mtd) && DP(DP(tk)->mtd)->mn == MN_isNull);
//	}
//	return 0;
//}
//
//static int TERMs_isCALLISNN(knh_Stmt_t *stmt, size_t n)
//{
//	if(IS_Stmt(DP(stmt)->stmts[n])) {
//		knh_Token_t *tk = DP(DP(stmt)->stmts[n])->tokens[0];
//		return (IS_Token(tk) && IS_Method(DP(tk)->mtd) && DP(DP(tk)->mtd)->mn == MN_isNotNull);
//	}
//	return 0;
//}

//static void TERMs_ASM_JMPNUL(Ctx *ctx, knh_Stmt_t *stmt, size_t n, knh_KLRInst_t* label)
//{
//	knh_Token_t *tk = DP(stmt)->tokens[n];
//	if(IS_Token(tk) && TT_(tk) == TT_LOCAL) {
//		KNH_ASM(JMPNUL, TADDR label, (DP(tk)->index));
//	}
//	else {
//		int espidx = DP(ctx->gma)->espidx;
//		TERMs_asm(ctx, stmt, n, TERMs_gettype(stmt, n), espidx);
//		KNH_ASM(JMPNUL, TADDR label, (espidx));
//	}
//	TODO(); DBG_ABORT();
//}

//static void TERMs_ASM_JMPNN(Ctx *ctx, knh_Stmt_t *stmt, size_t n, knh_KLRInst_t* label)
//{
//	knh_Token_t *tk = DP(stmt)->tokens[n];
//	knh_sfpidx_t sfpidx;
//	if(IS_Token(tk) && TT_(tk) == TT_LOCAL) {
//		sfpidx = DP(tk)->index;
//	}
//	else {
//		sfpidx = DP(ctx->gma)->espidx;
//		TERMs_asm(ctx, stmt, n, TERMs_gettype(stmt, n), sfpidx);
//	}
//	KNH_ASM(IS_NULL, sfpidx));
//	KNH_ASM(JMPF, TADDR label, sfpidx);
//	TODO(); DBG_ABORT();
//}

static void TERMs_JMPIF(Ctx *ctx, knh_Stmt_t *stmt, size_t n, int isTRUE, knh_KLRInst_t* label)
{
	knh_Token_t *tk = DP(stmt)->tokens[n];
	if(IS_Token(tk) && TT_(tk) == TT_LOCAL) {
		if(isTRUE) KNH_ASM(bNOT, DP(tk)->index, DP(tk)->index);
		KNH_ASM(JMPF, {label}, (DP(tk)->index));
	}
	else {
//		if(TERMs_isCALLISNUL(stmt, n)) {
//			TERMs_ASM_JMPNN(ctx, DP(stmt)->stmts[n], 1, label);
//		}
//		else if(TERMs_isCALLISNN(stmt, n)) {
//			TERMs_ASM_JMPNUL(ctx, DP(stmt)->stmts[n], 1, label);
//		}
//		else {
			int espidx = DP(ctx->gma)->espidx;
			TERMs_asm(ctx, stmt, n, TYPE_Boolean, espidx);
			if(isTRUE) KNH_ASM(bNOT, espidx, espidx);
			KNH_ASM(JMPF, {label}, (espidx));
//		}
	}
}

/* ------------------------------------------------------------------------ */
/* [EXPR] */

static knh_int_t TERMs_int(knh_Stmt_t *stmt, size_t n)
{
	knh_Token_t *tk = DP(stmt)->tokens[n];
	DBG_ASSERT(TT_(tk) == TT_CONST);
	return (DP(tk)->num)->n.ivalue;
}

static knh_float_t TERMs_float(knh_Stmt_t *stmt, size_t n)
{
	knh_Token_t *tk = DP(stmt)->tokens[n];
	DBG_ASSERT(TT_(tk) == TT_CONST);
	return (DP(tk)->num)->n.fvalue;
}

static knh_opcode_t OPCODE_imn(knh_methodn_t mn)
{
	switch(mn) {
	case MN_opNEG: return OPCODE_iNEG;
	case MN_opADD: return OPCODE_iADD;
	case MN_opSUB: return OPCODE_iSUB;
	case MN_opMUL: return OPCODE_iMUL;
	case MN_opDIV: return OPCODE_iDIV;
	case MN_opMOD: return OPCODE_iMOD;
	case MN_opEQ:  return OPCODE_iEQ;
	case MN_opNOTEQ: return OPCODE_iNEQ;
	case MN_opLT:  return OPCODE_iLT;
	case MN_opLTE: return OPCODE_iLTE;
	case MN_opGT:  return OPCODE_iGT;
	case MN_opGTE: return OPCODE_iGTE;
	}
	return OPCODE_NOP;
}

static knh_opcode_t OPCODE_fmn(knh_methodn_t mn)
{
	switch(mn) {
	case MN_opNEG: return OPCODE_fNEG;
	case MN_opADD: return OPCODE_fADD;
	case MN_opSUB: return OPCODE_fSUB;
	case MN_opMUL: return OPCODE_fMUL;
	case MN_opDIV: return OPCODE_fDIV;
	case MN_opEQ:  return OPCODE_fEQ;
	case MN_opNOTEQ: return OPCODE_fNEQ;
	case MN_opLT:  return OPCODE_fLT;
	case MN_opLTE: return OPCODE_fLTE;
	case MN_opGT:  return OPCODE_fGT;
	case MN_opGTE: return OPCODE_fGTE;
	}
	return OPCODE_NOP;
}

static int knh_StmtOP_checkConst(Ctx *ctx, knh_Stmt_t *stmt, knh_methodn_t *mn, int swap)
{
	int isCONST = (TT_(DP(stmt)->terms[2]) == TT_CONST);
	if(swap != 0 && TT_(DP(stmt)->terms[1]) == TT_CONST) {
		knh_methodn_t newmn = *mn;
		knh_Stmt_swap(ctx, stmt, 1, 2);
		if(*mn == MN_opLT) newmn = MN_opGT;  /* 1 < n ==> n > 1 */
		else if(*mn == MN_opLTE) newmn = MN_opGTE; /* 1 <= n => n >= 1 */
		else if(*mn == MN_opGT) newmn = MN_opLT;
		else if(*mn == MN_opGTE) newmn = MN_opLTE;
		*mn = newmn;
		isCONST = 1;
	}
	if(swap == 2 && TT_(DP(stmt)->terms[1]) == TT_CONST) {
		knh_methodn_t newmn = *mn;
		knh_Stmt_swap(ctx, stmt, 1, 2);
		if(*mn == MN_opLT) newmn = MN_opGT;  /* 1 < n ==> n > 1 */
		else if(*mn == MN_opLTE) newmn = MN_opGTE; /* 1 <= n => n >= 1 */
		else if(*mn == MN_opGT) newmn = MN_opLT;
		else if(*mn == MN_opGTE) newmn = MN_opLTE;
		*mn = newmn;
		isCONST = 1;
	}
	return isCONST;
}

static knh_opcode_t knh_opcode_shiftCONST(knh_opcode_t opcode, knh_methodn_t mn)
{
	int diff = 0;
	if (OPCODE_iADD <= opcode && opcode <= OPCODE_iGTE) {
		diff = (OPCODE_iADDn - OPCODE_iADD);
	}
	else if (OPCODE_fADD <= opcode && opcode <= OPCODE_fGTE) {
		diff = (OPCODE_fADDn - OPCODE_fADD);
	}
	switch(mn) {
	case MN_opADD: case MN_opSUB:
	case MN_opMUL: case MN_opDIV: case MN_opMOD:
		return opcode + diff;
	case MN_opEQ:  case MN_opNOTEQ:
	case MN_opLT:  case MN_opLTE:
	case MN_opGT:  case MN_opGTE:
		return opcode + diff;
	default:
		DBG_ABORT();
	}
	return opcode;
}

static void knh_StmtCALL_asm(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx);

static int knh_StmtOP_asm(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	int local = ASML(sfpidx);
	knh_Method_t *mtd = DP(DP(stmt)->tokens[0])->mtd;
	if(IS_NULL(mtd)) return 0;
	knh_methodn_t mn = DP(mtd)->mn;
	knh_class_t cid = CLASS_type(SP(DP(stmt)->tokens[1])->type);
	knh_opcode_t opcode;
	if(cid == CLASS_Int && ((opcode = OPCODE_imn(mn)) != OPCODE_NOP)) {
		int swap = 1;
		if(mn == MN_opNEG) {
			int a = TERMs_put(ctx, stmt, 1, TYPE_Int, local + 1);
			KNH_ASM(iNEG, (sfpidx), (a));
		}
		if(mn == MN_opSUB || mn == MN_opDIV || mn == MN_opMOD) swap = 0;
		if(knh_StmtOP_checkConst(ctx, stmt, &mn, swap)) {
			int a = TERMs_put(ctx, stmt, 1, TYPE_Int, local + 1);
			knh_int_t b = TERMs_int(stmt, 2);
			if(b == 0 && (mn == MN_opDIV || mn == MN_opMOD)) {
				b = 1;
				knh_Gamma_perror(ctx, KERR_ERRATA, _("divided by zero: /0 ==> /1"));
			}
			opcode = knh_opcode_shiftCONST(opcode, mn);
			klr_iADDn_t op = {TADDR opcode ASMLINE, (sfpidx), a, b};
			knh_asmop(ctx, (knh_opset_t*)(&op), sizeof(klr_iADDn_t));
		}
		else {
			int a = TERMs_put(ctx, stmt, 1, TYPE_Int, local + 1);
			int b = TERMs_put(ctx, stmt, 2, TYPE_Int, local + 2);
			klr_iADD_t op = {TADDR opcode ASMLINE, (sfpidx), a, b};
			knh_asmop(ctx, (knh_opset_t*)(&op), sizeof(klr_iADD_t));
		}
		return 1;
	} /* CLASS_Int */
	if(cid == CLASS_Float && ((opcode = OPCODE_fmn(mn)) != OPCODE_NOP)) {
		int swap = 1;
		if(mn == MN_opNEG) {
			int a = TERMs_put(ctx, stmt, 1, TYPE_Float, local + 1);
			KNH_ASM(fNEG, (sfpidx), (a));
			return 1;
		}
		if(mn == MN_opSUB || mn == MN_opDIV || mn == MN_opMOD) swap = 0;
		if(knh_StmtOP_checkConst(ctx, stmt, &mn, swap)) {
			int a = TERMs_put(ctx, stmt, 1, TYPE_Float, local + 1);
			knh_float_t b = TERMs_float(stmt, 2);
			if(b == K_FLOAT_ZERO && mn == MN_opDIV) {
				b = K_FLOAT_ONE;
				knh_Gamma_perror(ctx, KERR_ERRATA, _("divided by zero: 0.0 ==> 1.0"));
			}
			opcode = knh_opcode_shiftCONST(opcode, mn);
			klr_fADDn_t op = {TADDR opcode ASMLINE, (sfpidx), a, b};
			knh_asmop(ctx, (knh_opset_t*)(&op), sizeof(klr_fADDn_t));
		}
		else {
			int a = TERMs_put(ctx, stmt, 1, TYPE_Float, local + 1);
			int b = TERMs_put(ctx, stmt, 2, TYPE_Float, local + 2);
			klr_fADD_t op = {TADDR opcode ASMLINE, (sfpidx), a, b};
			knh_asmop(ctx, (knh_opset_t*)(&op), sizeof(klr_fADD_t));
		}
		return 1;
	} /* CLASS_Float */
	if(cid == CLASS_String && mn == MN_opADD) {
		DBG_P("string concat");
		//STT_(DP(stmt)->stmts[0]);
		//STT_(DP(stmt)->stmts[1])
	}
	knh_StmtCALL_asm(ctx, stmt, reqt, sfpidx);
	return 0;
}

/* ------------------------------------------------------------------------ */
/* CALL */

static knh_type_t TERMs_ptype(Ctx *ctx, knh_Stmt_t *stmt, size_t n, knh_class_t cid, knh_Method_t *mtd)
{
	if(!IS_Method(mtd)) {
		return TYPE_Any;  // boxing
	}
	if(n == 1) { // base
		if(IS_Tunbox(cid) && cid != DP(mtd)->cid) {
			/* Object, Number, Any */
			cid = DP(mtd)->cid;
		}
		DBG_(
		if(cid != DP(mtd)->cid) {
			DBG_P("** WATCH OUT ** mtd_cid=%s, DP(mtd)->cid=%s", CLASSN(cid), CLASSN(DP(mtd)->cid));
		});
		return cid;
	}
	else {
		knh_type_t ptype = knh_ParamArray_getptype(DP(mtd)->mp, n - 2);
		return knh_type_tocid(ctx, ptype, cid);
	}
}

static int knh_StmtPARAMs_asm(Ctx *ctx, knh_Stmt_t *stmt, size_t s, int local, knh_class_t cid, knh_Method_t *mtd)
{
	size_t i;
	if(s == 1 && knh_Method_isStatic(mtd)) s = 2;   // ignoring static caller, like Script
	if(DP(stmt)->size + DP(ctx->gma)->espidx > 32) {
		KNH_ASM(CHKSTACK, DP(stmt)->size + DP(ctx->gma)->espidx);
	}
	for(i = s; i < DP(stmt)->size; i++) {
		knh_type_t reqt = TERMs_ptype(ctx, stmt, i, cid, mtd);
		TERMs_asm(ctx, stmt, i, reqt, local + i + 2);
	}
	if(knh_Stmt_isTAILRECURSION(stmt) && knh_Gamma_isTailRecursion(ctx->gma)) {
		knh_KLRInst_t *lbBEGIN = knh_KLRINSTs_n(DP(ctx->gma)->lstacks, 1);
		for(i = s; i < DP(stmt)->size; i++) {
			knh_type_t reqt = TERMs_ptype(ctx, stmt, i, cid, mtd);
			if(IS_Tunbox(reqt)) {
				KNH_ASM(NMOV, (i-1), local + i + 2);
			}
			else {
				KNH_ASM(OMOV, (i-1), local + i + 2);
			}
		}
		KNH_ASM(JMP, {lbBEGIN});
		return 0;
	}
	return 1;
}

static knh_bool_t knh_opset_isHeadOfBasicBlock(knh_opset_t *codeblock, void *cur)
{
	knh_opset_t *pc = codeblock;
	while(pc->opcode != OPCODE_RET) {
		if(knh_opcode_hasjump(pc->opcode)) {
			if(pc->p[0] == cur) {
				DBG_P("%s jumppc=%p, cur=%p", knh_opcode_tochar(pc->opcode), pc->p[0], cur);
				return 1;
			}
		}
		pc++;
	}
	return 0;
}

static void KNH_ASM_INLINE(Ctx *ctx, int sfpshift, knh_opset_t *codeblock, size_t isize)
{
	knh_opset_t *pc = codeblock;
	knh_Array_t *a = DP(ctx->gma)->insts;
	size_t start = knh_Array_size(a);
	while(1) {
		if(knh_opset_isHeadOfBasicBlock(codeblock, pc)) {
			knh_KLRInst_t *lb = new_KLRInstLABEL(ctx);
			lb->code_pos = pc;
			knh_Array_add(ctx, a, lb);
		}
		if(pc->opcode == OPCODE_RET) {
			break;
		}
		else {
			knh_KLRInst_t *ix = new_KLRInst(ctx, pc, sizeof(knh_opset_t));
			ix->code_pos = pc;
			ix->op->count = 0;
			knh_opcode_idxshift(ix->op, sfpshift);
			knh_Array_add(ctx, a, ix);
			if(ix->opcode == OPCODE_JMP_) {
				ix->opcode = OPCODE_JMP;
				ix->op->opcode = OPCODE_JMP;
			}
			if(pc->opcode == OPCODE_RET) break;
			pc++;
		}
	}
	{
		size_t j;
		for(j = start; j < knh_Array_size(a); j++) {
			pc = a->insts[j]->op;
			if(knh_opcode_hasjump(pc->opcode)) {
				size_t i;
				for(i = start; i < knh_Array_size(a); i++) {
					knh_KLRInst_t *ix = a->insts[i];
					if(ix->opcode != OPCODE_LABEL) continue;
					if(pc->p[0] == (void*)ix->code_pos) {
						pc->p[0] = (void*)ix; break;
					}
				}
				DBG_ASSERT(i != knh_Array_size(a));
			}
		}
	}
}


#define knh_espshift(sfpidx, args)   (sfpidx + args + K_CALLDELTA + 1)

static void KNH_ASM_CALL(Ctx *ctx, knh_type_t reqt, int sfpidx, knh_Method_t *mtd, int isStatic, size_t args)
{
	DBG_ASSERT(IS_Method(mtd));
	KNH_ASSERT(sfpidx >= DP(ctx->gma)->espidx);
	if(knh_Method_isFinal(mtd) || isStatic) {
		if(knh_Gamma_hasJIT(ctx)) {
			KNH_ASM(LDMETHOD, klr_setMethod, sfpidx+K_CALLDELTA, mtd);
			KNH_ASM(CALL, sfpidx, knh_espshift(sfpidx, args));
		}
		else if(knh_Method_isKLRCode(mtd) || DP(ctx->gma)->mtd == mtd) {
			if(knh_Gamma_isInlineFunction(ctx->gma) && DP(ctx->gma)->mtd != mtd) {
				knh_KLRCode_t *kcode = DP(mtd)->kcode;
				size_t isize = kcode->codesize / sizeof(knh_opset_t);
				if(isize < K_INLINECODE) {
					knh_Gamma_perror(ctx, KERR_INFO, _("inlineing: %C.%M"), DP(mtd)->cid, DP(mtd)->mn);
					KNH_ASM_INLINE(ctx, sfpidx + K_CALLDELTA, kcode->code + 1, isize - 2);
					return;
				}
			}
			if(args == 0) {
				KNH_ASM(VCALL_, sfpidx, knh_espshift(sfpidx, args), mtd);
			}
			else {
				KNH_ASM(VCALL, sfpidx, knh_espshift(sfpidx, args), mtd);
			}
		}
		else {
			KNH_ASM(SCALL, sfpidx, knh_espshift(sfpidx, args), mtd);
		}
	}
	else {
		KNH_ASM(LDMETHOD, klr_lookupMethod, sfpidx+K_CALLDELTA, mtd);
		KNH_ASM(CALL, sfpidx, knh_espshift(sfpidx, args));
	}
}

static knh_Method_t* knh_lookupDynamicMethod(Ctx *ctx, knh_methodn_t mn)
{
	size_t cid, i, size = ctx->share->ClassTableSize;
	for(cid = 0; cid < size; cid++) {
		knh_Array_t *a = ClassTable(cid).methods;
		for(i = 0; i < knh_Array_size(a); i++) {
			if(DP(a->methods[i])->mn == mn) return a->methods[i];
		}
	}
	return knh_lookupMethod(ctx, CLASS_Tvoid, mn);
}

static void knh_StmtCALL_asm(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	int local = ASML(sfpidx);
	knh_Token_t *tkMTD = DP(stmt)->tokens[0];
	knh_Method_t *mtd = DP(tkMTD)->mtd;
	knh_class_t cid = TERMs_getcid(stmt, 1);
	if(!IS_Method(mtd)) {
		knh_StmtPARAMs_asm(ctx, stmt, 1, local, cid, mtd);
		mtd = knh_lookupDynamicMethod(ctx, DP(tkMTD)->mn);
		KNH_ASM(LDMETHOD, klr_checkParams, local+K_CALLDELTA, mtd);
		KNH_ASM(CALL, local, knh_espshift(local, DP(stmt)->size - 2));
		KNH_ASM_MOVL(ctx, reqt, sfpidx, TYPE_Any, local);
		return;
	}
	knh_class_t mtd_cid = DP(mtd)->cid;
	knh_methodn_t mtd_mn = DP(mtd)->mn;
//	if(cid == CLASS_Func && mtd_mn == MN_invoke) {
//		knh_StmtPARAMs_asm(ctx, stmt, 1, local, cid, mtd);
//		KNH_ASM(LDMETHOD, klr_invokeFunc, (local+K_CALLDELTA), mtd);
//		KNH_ASM(CALL, (local), (knh_ushort_t)DP(stmt)->size + 1);
//		KNH_ASM_MOVL(ctx, reqt, sfpidx, TYPE_Any, local);
//		return;
//	}
#if defined(OPCODE_OEVAL)
	if(mtd_mn == MN_getSize) {
		switch(mtd_cid) {
		case CLASS_Bytes: case CLASS_Array:
			{
				int a = TERMs_put(ctx, stmt, 1, TYPE_cid(mtd_cid), local + 1);
				KNH_ASM(OEVAL, klr_size, (sfpidx), (a));
				return;
			}
		}
	}
#endif/*OPCODE_OEVAL*/
	if(mtd_cid == CLASS_Array) {
		knh_class_t p1 = knh_class_p1(cid);
		DBG_P("*************** cid=%s, p1=%s", CLASSN(cid), CLASSN(p1));
		if(mtd_mn == MN_get) {
			int a = TERMs_put(ctx, stmt, 1, cid, local + 1);
			if(TERMs_isCONST(stmt, 2)) {
				knh_intptr_t n = (knh_intptr_t)TERMs_int(stmt, 2);
				if(IS_Tunbox(p1)) {
					KNH_ASM(NGETIDXn, (sfpidx), (a), n);
					KNH_ASM_BOX(ctx, reqt, p1, sfpidx);
				}
				else {
					KNH_ASM(OGETIDXn, (sfpidx), (a), n);
				}
			}
			else {
				int an = TERMs_put(ctx, stmt, 2, TYPE_Int, local + 2);
				if(IS_Tunbox(p1)) {
					KNH_ASM(NGETIDX, (sfpidx), (a), an);
					KNH_ASM_BOX(ctx, reqt, p1, sfpidx);
				}
				else {
					KNH_ASM(OGETIDX, (sfpidx), (a), an);
				}
			}
			return;
		}
		if(mtd_mn == MN_set) {
			int a = TERMs_put(ctx, stmt, 1, cid, local + 1);
			knh_type_t ptype = knh_Method_ptype(ctx, mtd, cid, 1);
			int v = TERMs_put(ctx, stmt, 3, ptype, local + 3);
			if(TERMs_isCONST(stmt, 2)) {
				knh_intptr_t n = (knh_intptr_t)TERMs_int(stmt, 2);
				knh_class_t p1 = knh_class_p1(cid);
				if(IS_Tunbox(p1)) {
					KNH_ASM(NSETIDXn, (sfpidx), (a), n, v);
					KNH_ASM_BOX(ctx, reqt, p1, sfpidx);
				}
				else {
					KNH_ASM(OSETIDXn, (sfpidx), (a), n, v);
				}
			}
			else {
				int an = TERMs_put(ctx, stmt, 2, TYPE_Int, local + 2);
				if(IS_Tunbox(p1)) {
					KNH_ASM(NSETIDXn, (sfpidx), (a), an, v);
					KNH_ASM_BOX(ctx, reqt, p1, sfpidx);
				}
				else {
					KNH_ASM(OSETIDXn, (sfpidx), (a), an, v);
				}
			}
			return;
		}
	}
	{
		knh_index_t deltaidx = knh_Method_indexOfGetterField(mtd);
		if(deltaidx != -1) {
			int b = TERMs_put(ctx, stmt, 1, TYPE_Object, local + 1);
			knh_type_t type = knh_ParamArray_rtype(DP(mtd)->mp);
			knh_sfx_t bx = {b, deltaidx};
			if(IS_Tint(type)) {
				KNH_ASM(iMOVx, (sfpidx), bx);
				KNH_ASM_BOX(ctx, reqt, TYPE_Int, sfpidx);
			}
			else if(IS_Tfloat(type)) {
				KNH_ASM(fMOVx, (sfpidx), bx);
				KNH_ASM_BOX(ctx, reqt, TYPE_Float, sfpidx);
			}
			else if(IS_Tbool(type)) {
				KNH_ASM(bMOVx, (sfpidx), bx);
			}
			else {
				KNH_ASM(OMOVx, (sfpidx), bx);
				KNH_ASM_UNBOX(ctx, type, sfpidx);
			}
			return;
		}
		deltaidx = knh_Method_indexOfSetterField(mtd);
		if(deltaidx != -1) {
			int b = TERMs_put(ctx, stmt, 1, TYPE_Object, local + 1);
			knh_type_t type = knh_Method_ptype(ctx, mtd, cid, 0);
			knh_sfx_t bx = {b, deltaidx};
			int c = TERMs_put(ctx, stmt, 2, type, local + 2);
			if(IS_Tint(type)) {
				KNH_ASM(XIMOV, bx, (c));
			}
			else if(IS_Tfloat(type)) {
				KNH_ASM(XFMOV, bx, (c));
			}
			else if(IS_Tbool(type)) {
				KNH_ASM(XBMOV, bx, (c));
			}
			else {
				KNH_ASM_BOX(ctx, type, SP(stmt)->type, c);
				KNH_ASM(XMOV, bx, (c));
			}
			if(reqt != TYPE_void) {
				if(IS_Tunbox(type)) {
					KNH_ASM(NMOV, sfpidx, c);
					KNH_ASM_BOX(ctx, reqt, type, sfpidx);
				}
				else {
					KNH_ASM(OMOV, sfpidx, c);
					KNH_ASM_UNBOX(ctx, type, sfpidx);
				}
			}
			return;
		}
	}
	if(knh_StmtPARAMs_asm(ctx, stmt, 1, local, cid, mtd)) {
		KNH_ASM_CALL(ctx, reqt, local, mtd, 0/*TODO*/, DP(stmt)->size - 2);
		KNH_ASM_MOVL(ctx, reqt, sfpidx, SP(stmt)->type, local);
	}
}

static void knh_StmtFUNC_asm(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	int local = ASML(sfpidx);
	knh_Method_t *mtd = DP(DP(stmt)->tokens[0])->mtd;
	knh_class_t cid = TERMs_getcid(stmt, 1);
	knh_ParamArray_t *pa = ClassTable(cid).cparam;
	size_t i;
	for(i = 0; i < pa->psize; i++) {
		knh_param_t *p = knh_ParamArray_get(pa, i);
		knh_type_t reqt = knh_type_tocid(ctx, p->type, DP(ctx->gma)->this_cid);
		TERMs_asm(ctx, stmt, i+2, reqt, local + i + 4);
	}
	TERMs_asm(ctx, stmt, 1, cid, local + 3);
	KNH_ASM_CALL(ctx, reqt, local, mtd, 0/*TODO*/, DP(stmt)->size - 2);
	KNH_ASM_MOVL(ctx, reqt, sfpidx, SP(stmt)->type, local);
}

static void knh_StmtEXPR_asm(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx);

static void knh_StmtCALL1_asm(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	if(IS_Stmt(DP(stmt)->stmtPOST)) {  /* a++ */
		int local = ASML(sfpidx);
		TERMs_asm(ctx, stmt, 0, reqt, local);
		knh_StmtEXPR_asm(ctx, DP(stmt)->stmtPOST, reqt, local+1);
		KNH_ASM_MOVL(ctx, reqt, sfpidx, SP(stmt)->type, local);
	}
	else {
		TERMs_asm(ctx, stmt, 0, reqt, sfpidx);
	}
}

static void knh_StmtNEW_asm(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	int local = ASML(sfpidx);
	knh_Method_t *mtd = DP(DP(stmt)->tokens[0])->mtd;
	knh_class_t cid = DP(DP(stmt)->tokens[1])->cid;
	knh_StmtPARAMs_asm(ctx, stmt, 2, local, cid, mtd);
	KNH_ASM(TR, local + K_CALLDELTA, local + K_CALLDELTA, cid, _NEW);
	KNH_ASM_CALL(ctx, reqt, local, mtd, 1, DP(stmt)->size - 2);
	KNH_ASM_MOVL(ctx, cid, sfpidx, SP(stmt)->type, local);
}

static void knh_StmtTCAST_asm(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	knh_type_t srct = TERMs_gettype(stmt, 1);
	if(srct != reqt) {
		int local = ASML(sfpidx);
		knh_Token_t *tkC = DP(stmt)->tokens[0];
		knh_Translator_t *trl = DP(tkC)->mpr;
		TERMs_asm(ctx, stmt, 1, srct, local);
		if(IS_Translator(trl)) {
			knh_class_t scid = SP(trl)->scid, tcid = SP(trl)->tcid;
			if(knh_Translator_isFinal(trl)) {
				if(scid == CLASS_Int && tcid == CLASS_Float) {
					KNH_ASM(fCAST, local, local);
				}
				else if(scid == CLASS_Float && tcid == CLASS_Int) {
					KNH_ASM(iCAST, local, local);
				}
				else {
					KNH_ASM_BOX(ctx, TYPE_Any, srct, local);
					KNH_ASM(SCAST, local, local, trl);
				}
			}
			else {
				KNH_ASM_BOX(ctx, TYPE_Any, srct, local);
				KNH_ASM(TCAST, local, local, trl);
			}
		}
		else {
			if(knh_Stmt_isDOWNCAST(stmt)) {
				KNH_ASM(TR, local, local, DP(tkC)->cid, _CHKTYPE);
			}
			else {
				TODO(); DBG_ABORT();
				//KNH_ASM(ACAST, local, KNH_NULVAL(CLASS_Translator));
			}
		}
		KNH_ASM_MOVL(ctx, reqt, sfpidx, SP(stmt)->type, local);
	}
}

static void knh_StmtALT_asm(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
//	knh_KLRInst_t* label = new_KLRInstLABEL(ctx);
//	int local = ASML(sfpidx);
//	int i, size = DP(stmt)->size;
//	knh_type_t reqc = CLASS_type(reqt);
//	for(i = 0; i < size - 1; i++) {
//		TERMs_asm(ctx, stmt, i, reqc, local);
//		TERMs_ASM_JMPNN(ctx, stmt, i, label);
//	}
//	KNH_ASM_LABEL(ctx, label);
//	KNH_ASM_MOVL(ctx, reqt, sfpidx, SP(stmt)->type, local);
	TODO();DBG_ABORT();
}

static void knh_StmtOR_asm(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	int local = ASML(sfpidx);
	knh_KLRInst_t*  label = new_KLRInstLABEL(ctx);
	int i, size = DP(stmt)->size;
	for(i = 0; i < size; i++) {
		TERMs_asm(ctx, stmt, i, TYPE_Boolean, local);
		TERMs_JMPIF(ctx, stmt, i, 1/*TRUE*/, label);
	}
	KNH_ASM_LABEL(ctx, label);
	KNH_ASM_MOVL(ctx, TYPE_Boolean, sfpidx, SP(stmt)->type, local);
}

static void knh_StmtAND_asm(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	int local = ASML(sfpidx);
	knh_KLRInst_t*  label = new_KLRInstLABEL(ctx);
	int i, size = DP(stmt)->size;
	for(i = 0; i < size; i++) {
		TERMs_asm(ctx, stmt, i, TYPE_Boolean, local);
		TERMs_JMPIF(ctx, stmt, i, 0/*FALSE*/, label);
	}
	KNH_ASM_LABEL(ctx, label);
	KNH_ASM_MOVL(ctx, TYPE_Boolean, sfpidx, SP(stmt)->type, local);
}

static void knh_StmtTRI_asm(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	int local = ASML(sfpidx);
	knh_KLRInst_t*  lbelse = new_KLRInstLABEL(ctx);
	knh_KLRInst_t*  lbend  = new_KLRInstLABEL(ctx);
	TERMs_asm(ctx, stmt, 0, TYPE_Boolean, local);
	TERMs_JMPIF(ctx, stmt, 0, 0/*FALSE*/, lbelse);
	TERMs_asm(ctx, stmt, 1, reqt, local);
	KNH_ASM(JMP, {lbend});
	/* else */
	KNH_ASM_LABEL(ctx, lbelse);
	TERMs_asm(ctx, stmt, 2, reqt, local);
	KNH_ASM_LABEL(ctx, lbend);
	KNH_ASM_MOVL(ctx, reqt, sfpidx, SP(stmt)->type, local);
}

static void knh_StmtLET_asm(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	knh_Token_t *tkL = DP(stmt)->tokens[1];
	if(TT_(tkL) == TT_LOCAL) {
		TERMs_asm(ctx, stmt, 2, SP(tkL)->type, DP(tkL)->index);
		if(reqt != TYPE_void) {
			KNH_ASM_SMOV(ctx, reqt, sfpidx, tkL);
		}
	}
	else if(IS_Token(DP(stmt)->tokens[2])) {
		KNH_ASM_MOV(ctx, tkL, DP(stmt)->tokens[2]);
		if(reqt != TYPE_void) {
			KNH_ASM_SMOV(ctx, reqt, sfpidx, tkL);
		}
	}
	else {
		int local = ASML(sfpidx);
		knh_term_t tt = TT_(tkL); /* NOTE: tkL is reused inside stmt */
		knh_short_t index = DP(tkL)->index;
		TERMs_asm(ctx, stmt, 2, SP(tkL)->type, local);
		TT_(tkL) = tt;
		DP(tkL)->index = index;
		KNH_ASM_MOV(ctx, tkL, DP(stmt)->tokens[2]);
		if(reqt != TYPE_void && local != sfpidx) {
			KNH_ASM_SMOV(ctx, reqt, sfpidx, tkL);
		}
	}
}

/* ------------------------------------------------------------------------ */

#define CASE_ASM(XX, ...) case STT_##XX : { \
		knh_Stmt##XX##_asm(ctx, stmt, ## __VA_ARGS__); \
		break;\
	}\

static void knh_StmtEXPR_asm(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	if(knh_Stmt_isTailReturn(stmt)) sfpidx = K_RTNIDX;
	switch(STT_(stmt)) {
	CASE_ASM(LET, reqt, sfpidx);
	CASE_ASM(FUNC, reqt, sfpidx);
	CASE_ASM(CALL, reqt, sfpidx);
	CASE_ASM(CALL1, reqt, sfpidx);
	CASE_ASM(OP, reqt, sfpidx);
	CASE_ASM(NEW, reqt, sfpidx);
	CASE_ASM(TCAST, reqt, sfpidx);
	CASE_ASM(AND, reqt, sfpidx);
	CASE_ASM(OR, reqt, sfpidx);
	CASE_ASM(ALT, reqt, sfpidx);
	CASE_ASM(TRI, reqt, sfpidx);
	default: DBG_P("unknown stt=%s", TT_tochar(STT_(stmt)));
	}
	KNH_ASM_BOX(ctx, reqt, SP(stmt)->type, sfpidx);
	if(sfpidx == K_RTNIDX) KNH_ASM_RET(ctx);
}

static void TERMs_asm(Ctx *ctx, knh_Stmt_t *stmt, size_t n, knh_type_t reqt, int local)
{
	if(IS_Token(DP(stmt)->tokens[n])) {
		knh_Token_t *tk = DP(stmt)->tokens[n];
		KNH_ASM_SMOV(ctx, reqt, local, tk);
	}
	else {
		knh_StmtEXPR_asm(ctx, DP(stmt)->stmts[n], reqt, local);
	}
	if(IS_Token(DP(stmt)->tokens[n])) {
		knh_Token_t *tk = DP(stmt)->tokens[n];
		knh_Token_toTYPED(ctx, tk, TT_LOCAL, reqt, local);
	}
	else {
		knh_Token_t *tk = new_TokenTYPED(ctx, TT_LOCAL, reqt, local);
		KNH_SETv(ctx, DP(stmt)->tokens[n], tk);
	}
	DBG_ASSERT(IS_Token(DP(stmt)->tokens[n]));
}

/* ======================================================================== */
/* [LABEL]  */

static
void knh_Gamma_pushLABEL(Ctx *ctx, knh_Stmt_t *stmt, knh_KLRInst_t *lbC, knh_KLRInst_t *lbB)
{
	knh_Object_t *tkL = NULL;
	if(IS_DictMap(DP(stmt)->metaDictMap)) {
		tkL = knh_DictMap_getNULL(ctx, DP(stmt)->metaDictMap, S_tobytes(TS_ATlabel));
	}
	if(tkL == NULL) {
		tkL = KNH_NULVAL(CLASS_Any);
	}
	knh_Array_add_(ctx, DP(ctx->gma)->lstacks, tkL);
	knh_Array_add_(ctx, DP(ctx->gma)->lstacks, UP(lbC));
	knh_Array_add_(ctx, DP(ctx->gma)->lstacks, UP(lbB));
}

static void knh_Gamma_popLABEL(Ctx *ctx)
{
	knh_Array_t *a = DP(ctx->gma)->lstacks;
	DBG_ASSERT(knh_Array_size(a) - 3 >= 0);
	knh_Array_clear(ctx, a, knh_Array_size(a) - 3);
}

/* ======================================================================== */
/* [IF, WHILE, DO, FOR, FOREACH]  */

static int TERMs_isDONE(knh_Stmt_t *stmt, size_t n)
{
	knh_Stmt_t *cur = DP(stmt)->stmts[n];
	return (IS_Stmt(cur) && SP(cur)->stt == STT_DONE);
}

static knh_Token_t *knh_Stmt_getLOCAL(knh_Stmt_t *stmt, size_t n)
{
	DBG_ASSERT(n < DP(stmt)->size);
	knh_Token_t *tkIT = DP(stmt)->tokens[n];
	DBG_ASSERT(IS_Token(tkIT));
	DBG_ASSERT(TT_(tkIT) == TT_LOCAL);
	return tkIT;
}

static void TERMs_asmBLOCK(Ctx *ctx, knh_Stmt_t *stmt, size_t n, knh_type_t reqt)
{
	DBG_ASSERT(IS_Stmt(DP(stmt)->stmts[n]));
	knh_Stmt_asmBLOCK(ctx, DP(stmt)->stmts[n], reqt);
}

static void knh_StmtIF_asm(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	knh_KLRInst_t*  lbELSE = new_KLRInstLABEL(ctx);
	knh_KLRInst_t*  lbEND  = new_KLRInstLABEL(ctx);
	/* if */
	TERMs_JMPIF(ctx, stmt, 0, 0/*FALSE*/, lbELSE);
	/* then */
	TERMs_asmBLOCK(ctx, stmt, 1, reqt);

	if(TERMs_isDONE(stmt, 2)) { /* PEEPHOLE this isn't a bug */
		KNH_ASM_LABEL(ctx, lbELSE);
		KNH_ASM_LABEL(ctx, lbEND);
		return ;
	}
	KNH_ASM(JMP, {lbEND});
	/* else */
	KNH_ASM_LABEL(ctx, lbELSE);
	TERMs_asmBLOCK(ctx, stmt, 2, reqt);
	/* endif */
	KNH_ASM_LABEL(ctx, lbEND);
}

static void knh_StmtSWITCH_asm(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	knh_Stmt_t *stmtCASE;
	knh_Token_t *tkIT = knh_Stmt_getLOCAL(stmt, 2);
	knh_KLRInst_t* lbC = new_KLRInstLABEL(ctx);
	knh_KLRInst_t* lbB = new_KLRInstLABEL(ctx);
	knh_Gamma_pushLABEL(ctx, stmt, lbC, lbB);
	KNH_ASM_LABEL(ctx, lbC);
	TERMs_asm(ctx, stmt, 0, SP(tkIT)->type, (DP(tkIT)->index));
	stmtCASE = DP(stmt)->stmts[1];
	while(stmtCASE != NULL) {
		if(SP(stmtCASE)->stt == STT_CASE) {
			if(!TERMs_isASIS(stmtCASE, 0)) {
				knh_KLRInst_t*  lbEND = new_KLRInstLABEL(ctx);
				DP(ctx->gma)->espidx = DP(stmtCASE)->espidx;
				DBG_P("it=%d, esp=%d, stmtCASE->esp=%d", DP(tkIT)->index, DP(ctx->gma)->espidx, DP(stmtCASE)->espidx);
				TERMs_JMPIF(ctx, stmtCASE, 0, 0/*FALSE*/, lbEND);
				TERMs_asmBLOCK(ctx, stmtCASE, 1, reqt);
				KNH_ASM_LABEL(ctx, lbEND);
			}
		}
		stmtCASE = DP(stmtCASE)->nextNULL;
	}
	stmtCASE = DP(stmt)->stmts[1];
	while(stmtCASE !=NULL) {
		if(SP(stmtCASE)->stt == STT_CASE) {
			if(TERMs_isASIS(stmtCASE, 0)) {
				TERMs_asmBLOCK(ctx, stmtCASE, 1, reqt);
			}
		}
		stmtCASE = DP(stmtCASE)->nextNULL;
	}
	KNH_ASM_LABEL(ctx, lbB);
	knh_Gamma_popLABEL(ctx);
}

static void KNH_ASM_JUMPLABEL(Ctx *ctx, knh_Stmt_t *stmt, int delta)
{
	size_t s = knh_Array_size(DP(ctx->gma)->lstacks);
	if(s < 3) {
		knh_Gamma_perror(ctx, KERR_ERR, _("don't use %s HERE"), cSTT_(stmt));
	}
	else {
		knh_Token_t *tkL = NULL;
		knh_KLRInst_t *lbBLOCK = NULL;
		if(DP(stmt)->size == 1) {
			tkL = DP(stmt)->tokens[0];
			if(TT_(tkL) == TT_ASIS) tkL = NULL;
		}
		if(tkL != NULL) {
			int i;
			knh_bytes_t lname = S_tobytes(DP(tkL)->text);
			for(i = s - 3; i >= 0; i -= 3) {
				knh_Token_t *tkSTACK = knh_TOKENs_n(DP(ctx->gma)->lstacks, i);
				if(IS_NOTNULL(tkSTACK) && S_equals(DP(tkSTACK)->text, lname)) {
					lbBLOCK = knh_KLRINSTs_n(DP(ctx->gma)->lstacks, i + delta);
					goto L_JUMP;
				}
			}
			knh_Gamma_perror(ctx, KERR_EWARN, _("undefined label: %L"), tkL);
		}
		lbBLOCK = knh_KLRINSTs_n(DP(ctx->gma)->lstacks, s - 3 + delta);
		L_JUMP:;
		KNH_ASM(JMP, {lbBLOCK})
	}
}

static void knh_StmtCONTINUE_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	KNH_ASM_JUMPLABEL(ctx, stmt, 1);
}

static void knh_StmtBREAK_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	KNH_ASM_JUMPLABEL(ctx, stmt, 2);
}

static void knh_StmtWHILE_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	knh_KLRInst_t* lbC = new_KLRInstLABEL(ctx);
	knh_KLRInst_t* lbB = new_KLRInstLABEL(ctx);
	knh_Gamma_pushLABEL(ctx, stmt, lbC, lbB);
	KNH_ASM_LABEL(ctx, lbC);
	if(!TERMs_isTRUE(stmt, 0)) {
		TERMs_JMPIF(ctx, stmt, 0, 0/*FALSE*/, lbB);
		KNH_RENAME(OPCODE_JMPF, OPCODE_LOOPJMPF);
	}
	TERMs_asmBLOCK(ctx, stmt, 1, TYPE_void);
	KNH_ASM(JMP, {lbC});
	KNH_ASM_LABEL(ctx, lbB);
	knh_Gamma_popLABEL(ctx);
}

static void knh_StmtDO_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	knh_KLRInst_t* lbC = new_KLRInstLABEL(ctx);
	knh_KLRInst_t* lbB = new_KLRInstLABEL(ctx);
	knh_Gamma_pushLABEL(ctx, stmt, lbC, lbB);
	KNH_ASM_LABEL(ctx, lbC);
	TERMs_asmBLOCK(ctx, stmt, 0, TYPE_void);
	TERMs_JMPIF(ctx, stmt, 1, 0/*FALSE*/, lbB);
	KNH_RENAME(OPCODE_JMPF, OPCODE_LOOPJMPF);
	KNH_ASM(JMP, {lbC});
	KNH_ASM_LABEL(ctx, lbB);
	knh_Gamma_popLABEL(ctx);
}

static void knh_StmtFOR_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	knh_KLRInst_t* lbC = new_KLRInstLABEL(ctx);
	knh_KLRInst_t* lbB = new_KLRInstLABEL(ctx);
	knh_KLRInst_t* lbREDO = new_KLRInstLABEL(ctx);
	knh_Gamma_pushLABEL(ctx, stmt, lbC, lbB);
	/* i = 1 part */
	TERMs_asmBLOCK(ctx, stmt, 0, TYPE_void);
	KNH_ASM(JMP, {lbREDO});
	/* i++ part */
	KNH_ASM_LABEL(ctx, lbC); /* CONTINUE */
	TERMs_asmBLOCK(ctx, stmt, 2, TYPE_void);
	/* i < 10 part */
	KNH_ASM_LABEL(ctx, lbREDO);
	if(!TERMs_isTRUE(stmt, 1)) {
		TERMs_JMPIF(ctx, stmt, 1, 0/*FALSE*/, lbB);
		KNH_RENAME(OPCODE_JMPF, OPCODE_LOOPJMPF);
	}
	TERMs_asmBLOCK(ctx, stmt, 3, TYPE_void);
	KNH_ASM(JMP, {lbC});

	KNH_ASM_LABEL(ctx, lbB);
	knh_Gamma_popLABEL(ctx);
}

/* ------------------------------------------------------------------------ */

static knh_Token_t *new_TokenLOCAL(Ctx *ctx, knh_type_t type, int sfpidx)
{
	knh_Token_t *tk = new_Token(ctx, 0, TT_LOCAL);
	DP(tk)->index = (knh_short_t)sfpidx;
	SP(tk)->type = type;
	return tk;
}

/* ------------------------------------------------------------------------ */

static void knh_StmtLETM_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	size_t i, size = DP(stmt)->size / 2;
	int local = DP(ctx->gma)->espidx;
	for(i = 0; i < size; i++) {
		int n = (i*2);
		knh_type_t type = TERMs_gettype(stmt, n);
		TERMs_asm(ctx, stmt, n+1, type, local+i);
		KNH_SETv(ctx, DP(stmt)->tokens[n+1], new_TokenLOCAL(ctx, type, local+i));
	}
	for(i = 0; i < size; i++) {
		int n = (i*2);
		if(IS_Token(DP(stmt)->terms[n])) {
			KNH_ASM_MOV(ctx, DP(stmt)->tokens[n], DP(stmt)->tokens[n+1]);
		}
		else {
			knh_Stmt_t *stmtSET = DP(stmt)->stmts[n];
			DBG_ASSERT(STT_(stmtSET) == STT_CALL);
			DBG_ASSERT(TT_(DP(stmt)->tokens[n+1]) == TT_LOCAL);
			KNH_SETv(ctx, DP(stmtSET)->tokens[DP(stmtSET)->size - 1], DP(stmt)->tokens[n+1]);
			knh_StmtCALL_asm(ctx, stmtSET, TYPE_void, local + size);
		}
	}
}

static void knh_StmtFOREACH_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	knh_KLRInst_t* lbC = new_KLRInstLABEL(ctx);
	knh_KLRInst_t* lbB = new_KLRInstLABEL(ctx);
	knh_Stmt_t *stmtDECL = DP(stmt)->stmts[0];
	int itridx = DP(stmt)->espidx;
	knh_Gamma_pushLABEL(ctx, stmt, lbC, lbB);
	if(isSINGLEFOREACH(stmtDECL)) {
		knh_Token_t *tkN = DP(stmtDECL)->tokens[1];
		TERMs_asm(ctx, stmt, 1, TYPE_Iterator, itridx);
		KNH_ASM_LABEL(ctx, lbC);
//		if(reqc == foundc || reqc == CLASS_Any || knh_class_instanceof(ctx, reqc, foundc)) {
//			KNH_ASM(NEXT, TADDRx lbB, (DP(tkN)->index), (DP(tkITR)->index));
//		}
//		else {
//			KNH_ASM(NEXTf, TADDRx lbB, klr_inext, reqc, (DP(tkN)->index), (DP(tkITR)->index));
//		}
		DBG_P("variable=%d, iterator=%d", (DP(tkN)->index), itridx);
		KNH_ASM(NEXT, {lbB}, (DP(tkN)->index), itridx);
	}
	else {

	}
	TERMs_asmBLOCK(ctx, stmt, 2, TYPE_void);
	KNH_ASM(JMP, {lbC});
	/* end */
	KNH_ASM_LABEL(ctx, lbB);
	knh_Gamma_popLABEL(ctx);
}

/* ======================================================================== */
/* [TRY] */

static int knh_Gamma_inTry(Ctx *ctx)
{
	return IS_Stmt(DP(ctx->gma)->finallyStmt);
}

static void knh_Gamma_setFinallyStmt(Ctx *ctx, knh_Stmt_t *stmt)
{
	if(IS_NOTNULL(stmt)) {
		if(IS_NOTNULL(DP(ctx->gma)->finallyStmt)) {
			knh_Gamma_perror(ctx, KERR_ERR, _("cannot use nested try statements"));
			return;
		}
		KNH_SETv(ctx, DP(ctx->gma)->finallyStmt, stmt);
	}
	else { /* stmt == null */
		KNH_SETv(ctx, DP(ctx->gma)->finallyStmt, stmt);
	}
}

static void KNH_ASM_FINALLY(Ctx *ctx)
{
	if(IS_NOTNULL(DP(ctx->gma)->finallyStmt)) {
		knh_Stmt_asmBLOCK(ctx, DP(ctx->gma)->finallyStmt, TYPE_void);
	}
}

static void knh_StmtTRY_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	knh_KLRInst_t*  lbCATCH   = new_KLRInstLABEL(ctx);
	knh_KLRInst_t*  lbFINALLY = new_KLRInstLABEL(ctx);
	knh_Token_t *tkIT = knh_Stmt_getLOCAL(stmt, 3/*HDR*/);
	knh_Gamma_setFinallyStmt(ctx, DP(stmt)->stmts[2/*finally*/]);

	/* try { */
	KNH_ASM(TRY, {lbCATCH}, (DP(tkIT)->index));
	TERMs_asmBLOCK(ctx, stmt, 0/*try*/, TYPE_void);
	//KNH_ASM(TRYEND, (DP(tkIT)->index));
	KNH_ASM(JMP, {lbFINALLY});
	knh_Gamma_setFinallyStmt(ctx, (knh_Stmt_t*)KNH_NULL); // InTry

	/* catch */
	KNH_ASM_LABEL(ctx, lbCATCH);
	knh_Stmt_t *stmtCATCH = DP(stmt)->stmts[1/*catch*/];
	KNH_ASSERT(IS_Stmt(stmtCATCH));
	while(stmtCATCH != NULL) {
		if(SP(stmtCATCH)->stt == STT_CATCH) {
			knh_String_t *emsg = DP(DP(stmtCATCH)->tokens[0])->text;
			knh_Token_t *tkn = DP(stmtCATCH)->tokens[1];
			DBG_ASSERT(IS_String(emsg));
			DBG_ASSERT(TT_(tkn) == TT_LOCAL);
			lbCATCH = new_KLRInstLABEL(ctx);
			KNH_ASM(CATCH, {lbCATCH}, (DP(tkn)->index), emsg);
			TERMs_asmBLOCK(ctx, stmtCATCH, 2, TYPE_void);
			KNH_ASM(JMP, {lbFINALLY});  /* GOTO FINALLY */
			KNH_ASM_LABEL(ctx, lbCATCH); /* _CATCH_NEXT_ */
		}
		stmtCATCH = DP(stmtCATCH)->nextNULL;
	}
	KNH_ASM_LABEL(ctx, lbFINALLY); /* FINALLY */
	TERMs_asmBLOCK(ctx, stmt, 2/*finally*/, TYPE_void);
	KNH_ASM(THROW, (DP(tkIT)->index));
	KNH_ASM(OSET, (DP(tkIT)->index), KNH_NULL);
}

static void knh_StmtTHROW_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	int start = 0, espidx = DP(ctx->gma)->espidx;
	if(knh_Gamma_inTry(ctx)) {
		start = espidx;
	}
	TERMs_asm(ctx, stmt, 0, TYPE_Exception, espidx);
	KNH_ASM(TR, espidx, espidx, TYPE_Exception, _ERR);
	KNH_ASM(THROW, start);
}

static void knh_StmtRETURN_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	KNH_ASM_FINALLY(ctx);
	size_t size = DP(stmt)->size;
	if(size == 1) {
		knh_type_t rtype = TERMs_gettype(stmt, 0);
		TERMs_asm(ctx, stmt, 0, rtype, K_RTNIDX);
		if(IS_Tunbox(rtype)) {
			knh_ParamArray_t *pa = DP(DP(ctx->gma)->mtd)->mp;
			knh_param_t *p = knh_ParamArray_rget(pa, 0);
			KNH_ASM_BOX(ctx, knh_type_tocid(ctx, p->type, DP(ctx->gma)->this_cid), rtype, K_RTNIDX);
		}
	}
	else if(size > 1) {
		long i, espidx = DP(stmt)->espidx;
		for(i = size-1; i >= 1; i--) {
			knh_type_t rtype = TERMs_gettype(stmt, i);
			TERMs_asm(ctx, stmt, i, rtype, espidx + i);
		}
		for(i = size-1; i >= 1; i--) {
			knh_type_t rtype = TERMs_gettype(stmt, i);
			if(IS_Tunbox(rtype)) {
				knh_ParamArray_t *pa = DP(DP(ctx->gma)->mtd)->mp;
				knh_param_t *p = knh_ParamArray_rget(pa, i);
				KNH_ASM(NMOV, (K_RTNIDX+i), (espidx+i));
				KNH_ASM_BOX(ctx, knh_type_tocid(ctx, p->type, DP(ctx->gma)->this_cid), rtype, (K_RTNIDX+i));
			}
			else {
				KNH_ASM(OMOV, (K_RTNIDX+i), (espidx+i));
			}
		}
	}
	if(IS_Stmt(DP(stmt)->stmtPOST)) {
		knh_StmtEXPR_asm(ctx, DP(stmt)->stmtPOST, TERMs_gettype(stmt, 0), DP(DP(stmt)->stmtPOST)->espidx+1);
	}
	KNH_ASM_RET(ctx);
}

static void knh_StmtERR_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	int start = 0, espidx = DP(ctx->gma)->espidx;
	if(knh_Gamma_inTry(ctx)) {
		start = espidx;
	}
	if(!IS_bString(DP(stmt)->errMsg)) {
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		knh_printf(ctx, cwb->w, "Script!!: konoha -c %s at %d for debugging", FILEN(SP(stmt)->uri), SP(stmt)->line);
		KNH_SETv(ctx, DP(stmt)->errMsg, knh_cwb_newString(ctx, cwb));
		if(DP(stmt)->nextNULL != NULL) {
			KNH_FINALv(ctx, DP(stmt)->nextNULL);
			DP(stmt)->nextNULL = NULL;
		}
	}
	KNH_ASM(OSET, espidx, UP(DP(stmt)->errMsg));
	KNH_ASM(TR, espidx, espidx, CLASS_Exception, _ERR);
	KNH_ASM(THROW, start);
}

/* ======================================================================== */
/* [PRINT] */

static int knh_Gamma_isDEBUG(Ctx *ctx)
{
	if(knh_Method_isDebug(DP(ctx->gma)->mtd)) return 1;
	return knh_Context_isDebug(ctx);
}

static void KNH_ASM_SKIP(Ctx *ctx, knh_KLRInst_t* lbskip)
{
	if(!knh_Method_isDebug(DP(ctx->gma)->mtd)) {
		KNH_ASM(DYJMP, {lbskip}, 0, klr_isskip);
	}
}

static knh_flag_t knh_StmtPRINT_flag(Ctx *ctx, knh_Stmt_t *o)
{
	knh_flag_t flag = 0;
	if(IS_bDictMap(DP(o)->metaDictMap)) {
		Object *v = knh_DictMap_getNULL(ctx,  DP(o)->metaDictMap, STEXT("time"));
		if(v != NULL) {
			flag |= K_FLAG_PF_TIME;
		}
	}
	return flag;
}

KLRAPI(void) _PRINT(Ctx *ctx, knh_sfp_t *sfp, struct klr_P_t *op)
{
	knh_flag_t flag = (knh_flag_t)op->flag;
	knh_OutputStream_t *w = KNH_STDOUT; //knh_stack_pstream(ctx, flag);
	if(FLAG_is(flag, K_FLAG_PF_BOL)) {
		if(FLAG_is(flag, K_FLAG_PF_LINE)) {
			knh_Method_t *mtd = sfp[-1].callmtd;
			DBG_ASSERT(IS_Method(mtd));
			knh_write_uline(ctx, w, DP(mtd)->uri, op->line);
		}
	}
	if(IS_NOTNULL(op->msg)) {
		DBG_ASSERT(IS_bString(op->msg));
		if((op->msg)->str.len > 0) {
			knh_print(ctx, w, S_tobytes(op->msg));
		}
	}
	if(FLAG_is(flag, K_FLAG_PF_NAME)) {
		knh_putc(ctx, w, '=');
	}
	if(op->fmt != NULL) {
		long n = op->n;
		long rtnidx = n + 1;
		KNH_SETv(ctx, sfp[rtnidx+K_CALLDELTA].o, sfp[n].o);
		sfp[rtnidx+K_CALLDELTA].data = sfp[n].data;
		KNH_SETv(ctx, sfp[rtnidx+K_CALLDELTA+1].o, w);
		KNH_SCALL(ctx, sfp, rtnidx, op->fmt, 1);
	}
	if(FLAG_is(flag, K_FLAG_PF_EOL)) {
		knh_write_EOL(ctx, w);
	}
	else {
		if(op->fmt != NULL) knh_putc(ctx, w, ',');
		knh_putc(ctx, w, ' ');
	}
}

static void knh_StmtPRINT_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	if(DP(stmt)->size == 0) return ;
	knh_flag_t flag = knh_StmtPRINT_flag(ctx, stmt);
	knh_KLRInst_t*  lbskip = new_KLRInstLABEL(ctx);
	int i;
	KNH_ASM_SKIP(ctx, lbskip);
	if(!knh_Context_isInteractive(ctx)) {
		flag = flag | K_FLAG_PF_BOL | K_FLAG_PF_LINE;
	}
	for(i = 0; i < DP(stmt)->size; i++) {
		knh_flag_t mask = 0;
		knh_String_t *msg = (knh_String_t*)KNH_NULL;
		L_REDO:;
		knh_Token_t *tkn = DP(stmt)->tokens[i];
		if(i == DP(stmt)->size - 1) {
			mask |= K_FLAG_PF_EOL;
		}
		if(TT_(tkn) == TT_CONST && IS_String(DP(tkn)->data)) {
			if(knh_Token_isPNAME(tkn)) { /* name= */
				msg = DP(tkn)->text;
				mask |= K_FLAG_PF_NAME; i++;
				goto L_REDO;
			}
			KNH_ASM(P, _PRINT, flag | mask, DP(tkn)->text, NULL, 0); flag = 0;
		}
		else {
			long espidx = DP(ctx->gma)->espidx;
			knh_Method_t *mtd = knh_lookupFormatter(ctx, TERMs_getcid(stmt, i), MN__s);
			TERMs_asm(ctx, stmt, i, TERMs_gettype(stmt, i), espidx);
			KNH_ASM(P, _PRINT, flag | mask, msg, mtd, espidx); flag=0;
		}
	}
	KNH_ASM_LABEL(ctx, lbskip);
}

static void knh_StmtASSERT_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	int isRelease = knh_StmtMETA_is(ctx, stmt, STEXT("@Release"));
	int start = 0, espidx = DP(ctx->gma)->espidx;
	if(knh_Gamma_inTry(ctx)) {
		start = espidx;
	}
	if(!isRelease) {
		if(!knh_Gamma_isDEBUG(ctx)) {
			return ;
		}
	}
	knh_KLRInst_t* lbskip = new_KLRInstLABEL(ctx);
	if(!isRelease) {
		KNH_ASM(DYJMP, {lbskip}, 0, klr_isskip);
	}
	/* if */
	TERMs_JMPIF(ctx, stmt, 0, 1, lbskip);
	/*then*/
	TERMs_asmBLOCK(ctx, stmt, 1, TYPE_void);
	KNH_ASM(OSET, espidx, UP(TS_AssertionException));
	KNH_ASM(TR, espidx, espidx, CLASS_Exception, _ERR);
	KNH_ASM(THROW, start);
	KNH_ASM_LABEL(ctx, lbskip);
//	}
//	else {
//		int esp = DP(ctx->gma)->espidx;
//		knh_Method_t *mtd = knh_getMethodNULL(ctx, CLASS_System, MN_test);
//		DBG_ASSERT(IS_Method(mtd));
//		DBG_ASSERT(DP(ctx->gma)->testidx < esp);
//		TERMs_put(ctx, stmt, 0, TYPE_Boolean, esp+2);
//		KNH_ASM(MOV, esp+3, DP(ctx->gma)->testidx);
//		KNH_ASM(NSET, esp+1, (knh_int_t)SP(stmt)->line);
//		KNH_ASM(SCALL, esp, 4, mtd);
//	}
	TODO(); DBG_ABORT();
}

//static void knh_StmtTEST_asm(Ctx *ctx, knh_Stmt_t *stmt)
//{
//	knh_Gamma_t *kc = ctx->gma;
//	knh_Token_t *tkIT = knh_Stmt_getLOCAL(stmt, TEST_IT);
//	int testidx = DP(ctx->gma)->testidx;
//	DP(ctx->gma)->testidx = DP(tkIT)->index;
//	TERMs_put(ctx, stmt, 0, TYPE_String, DP(tkIT)->index);
//	TERMs_asmBLOCK(ctx, stmt, 1);
//	DP(ctx->gma)->testidx = testidx;
//}

/* ------------------------------------------------------------------------ */

#define _isLast(stmt) (knh_Stmt_isEveryLine(stmt) || DP(stmt)->nextNULL == NULL)

static void knh_Stmt_asmBLOCK(Ctx *ctx, knh_Stmt_t *stmtH, knh_type_t reqt)
{
	knh_Stmt_t *stmt = stmtH;
	while(stmt != NULL) {
		knh_type_t etype = (_isLast(stmt)) ? reqt : TYPE_void;
		knh_Gamma_setLine(ctx, SP(stmt)->line);
		DP(ctx->gma)->espidx = DP(stmt)->espidx;
		DBG_P("ASM %s %p etype=%s,%s, espidx=%d", TT_tochar(STT_(stmt)), stmt, TYPEN(etype), TYPEN(stmt->type), DP(ctx->gma)->espidx);
		switch(STT_(stmt)) {
		case STT_BLOCK:
			knh_Stmt_asmBLOCK(ctx, DP(stmt)->stmts[0], etype);
			break;
		case STT_REGISTER:
		{
			size_t i;
			for(i = 0; i < DP(stmt)->size; i++) {
				if(IS_Stmt(DP(stmt)->terms[i]) && STT_(DP(stmt)->stmts[i]) == STT_LET) {
					knh_StmtLET_asm(ctx, DP(stmt)->stmts[i], TYPE_void, DP(ctx->gma)->espidx);
				}
			}
			break;
		}
		CASE_ASM(LETM);
		CASE_ASM(IF, etype);
		CASE_ASM(SWITCH, etype);
		CASE_ASM(WHILE);
		CASE_ASM(DO);
		CASE_ASM(FOR);
		CASE_ASM(FOREACH);
		CASE_ASM(BREAK);
		CASE_ASM(CONTINUE);
		CASE_ASM(TRY);
		CASE_ASM(THROW);
		CASE_ASM(RETURN);
		CASE_ASM(PRINT);
		CASE_ASM(ASSERT);
		CASE_ASM(ERR);
		case STT_DECL: case STT_DONE:
			break;
		default:
			knh_StmtEXPR_asm(ctx, stmt, etype, DP(ctx->gma)->espidx);
		}
		if(knh_Stmt_isEveryLine(stmtH)) break;
		stmt = DP(stmt)->nextNULL;
	}
}

/* ------------------------------------------------------------------------ */

void knh_Method_asm(Ctx *ctx, knh_Method_t *mtd, knh_Stmt_t *stmtP, knh_Stmt_t *stmtB, knh_type_t reqt)
{
	knh_class_t prev_cid = DP(ctx->gma)->this_cid;
	if(STT_(stmtB) == STT_ERR) return ;
	knh_Method_typing(ctx, mtd, stmtP, stmtB, reqt);
	DBG_P("****** %s TYPE=%s, NEXT=%p", TT_tochar(STT_(stmtB)), TYPEN(stmtB->type), DP(stmtB)->nextNULL);
	if(STT_(stmtB) == STT_ERR) return ;
	{
		knh_KLRInst_t* lbBEGIN = new_KLRInstLABEL(ctx);
		knh_KLRInst_t* lbEND = new_KLRInstLABEL(ctx);
		size_t i, xi;
		knh_Method_t *mtd = DP(ctx->gma)->mtd;
		DBG_ASSERT(IS_Method(mtd));
		knh_Gamma_pushLABEL(ctx, stmtB, lbBEGIN, lbEND);
		SP(ctx->gma)->line = SP(stmtB)->line;
		KNH_ASM(THCODE);
		if(knh_Method_isStatic(mtd) && knh_Gamma_hasFIELD(ctx->gma)) {
			KNH_ASM(TR, 0, 0, DP(ctx->gma)->this_cid, _NULVAL);
		}
		KNH_ASM_LABEL(ctx, lbBEGIN);
		for(i = 1;i < knh_Method_psize(mtd) + 1; i++) {
			xi = i + DP(ctx->gma)->goffset;
			Object *value = DP(ctx->gma)->gf[xi].value;
			DBG_(
			knh_type_t ztype = knh_ParamArray_getptype(DP(mtd)->mp, i - 1);
			knh_type_t ptype = DP(ctx->gma)->gf[xi].type;
			DBG_P("PARAM TYPE %s%s (%s%s) i=%ld, xi=%ld %s value=%p", TYPEQN(ztype), TYPEQN(ptype), i, xi, FN_tochar(DP(ctx->gma)->gf[xi].fn), value);
			DBG_ASSERT(CLASS_type(ztype) == CLASS_type(ptype));
			);
			if(value == NULL) {
//				KNH_ASM(NULTR, Ftr_DEF, (i), CLASS_type(ptype));
			}
			else {
//				if(IS_NOTNULL(value)) {
//					knh_KLRInst_t*  lb = new_KLRInstLABEL(ctx);
//					KNH_ASM(JMPNN, TADDRx lb, i);
//					KNH_ASM(OSET, (i), value);
//					KNH_ASM_UNBOX(ctx, i, knh_Object_cid(value));
//					KNH_ASM_LABEL(ctx, lb);
//				}
			}
		}
		xi = i + DP(ctx->gma)->goffset;
		if(DP(ctx->gma)->gf[xi].fn == FN_vargs) {
			knh_class_t cid = knh_class_p1(CLASS_type(DP(ctx->gma)->gf[xi].type));
			DBG_ASSERT_cid(cid);
			KNH_ASM(TR, i, i, cid, _VARGS);// i++;
		}
		knh_Stmt_asmBLOCK(ctx, stmtB, knh_ParamArray_rtype(DP(mtd)->mp));
		KNH_ASM_LABEL(ctx, lbEND);
		KNH_ASM(RET);
		knh_Gamma_popLABEL(ctx);
		DBG_ASSERT(knh_Array_size(DP(ctx->gma)->lstacks) == 0);
	}
	knh_Gamma_compile(ctx);
	DP(ctx->gma)->this_cid = prev_cid;
}


/* ------------------------------------------------------------------------ */

knh_opset_t* knh_code_findLabelId(Ctx *ctx, knh_opset_t *op, knh_intptr_t id)
{
	while(1) {
		if(op->opcode == OPCODE_LABEL) {
			klr_LABEL_t *opLABEL = (klr_LABEL_t*)op;
			if(opLABEL->id == id) {
				return op + 1;
			}
		}
		if(op->opcode == OPCODE_RET) break;
		op++;
	}
	DBG_ABORT();
	return NULL;
}

/* ------------------------------------------------------------------------ */
/* [loadSystem] */

//static
//KLRAPI(void) knh_Fprobe_printStackTrace(Ctx *ctx, knh_sfp_t *sfp, int n, knh_opset_t **pc)
//{
//	knh_Exception_t *e = DP(sfp[n].hdr)->caught;
//	DBG_ASSERT(IS_ExceptionHandler(sfp[-3].hdr));
//	DBG_ASSERT(IS_Exception(e));
//	knh_write_Object(ctx, KNH_STDERR, MN__dump, UP(e), KNH_NULL);
//}

#define knh_Array_addInst(ctx, ilist, T, ...) { \
		klr_##T##_t op_ = {TADDR OPCODE_##T ASMLINE, ## __VA_ARGS__};\
		knh_Array_add(ctx, ilist, new_KLRInst(ctx, (knh_opset_t*)(&op_), sizeof(klr_##T##_t)));\
	}\

void knh_loadSystemKLRCode(Ctx *ctx)
{
	BEGIN_LOCAL(ctx, lsfp, 1);
	knh_Array_t* ia = new_Array0(ctx, 0);
	knh_KLRInst_t*  lbERROR = new_KLRInstLABEL(ctx);
	KNH_SETv(ctx, lsfp[0].o, ia);
	knh_Array_addInst(ctx, ia, THCODE);
	knh_Array_addInst(ctx, ia, LABEL, 1, TS_EMPTY); // CODE_LAUNCH
	knh_Array_addInst(ctx, ia, TRY, {lbERROR}, (-3));
	knh_Array_addInst(ctx, ia, ENTER);
	knh_Array_addInst(ctx, ia, EXIT);
	knh_Array_add(ctx, ia, lbERROR);
	KNH_ASM_LABEL(ctx, lbERROR);
//	knh_Array_addInst(ctx, ia, PROBE, knh_Fprobe_printStackTrace, -3);
	knh_Array_addInst(ctx, ia, EXIT);
	knh_Array_addInst(ctx, ia, LABEL, 2, TS_EMPTY); // CODE_FUNCCALL
	knh_Array_addInst(ctx, ia, FUNCCALL);
	knh_Array_addInst(ctx, ia, LABEL, 3, TS_EMPTY); // CODE_VEXEC
	knh_Array_addInst(ctx, ia, VEXEC);
	knh_Array_addInst(ctx, ia, EXIT);
	knh_Array_addInst(ctx, ia, LABEL, 4, TS_EMPTY); // CODE_ABSTRACT
	//knh_Array_addInst(ctx, ia, THROWs, 0, 0, new_T("AbstractMethod!!"));
	knh_Array_addInst(ctx, ia, RET);  // NEED TERMINATION
	{
		knh_KLRCode_t *kcode = new_KLRCode(ctx, ia);
		knh_opset_t *pc = knh_VirtualMachine_run(ctx, ctx->esp, SP(kcode)->code);
		knh_setClassDefaultValue(ctx, CLASS_KLRCode, kcode, NULL);
		((knh_share_t*)ctx->share)->PC_LAUNCH = knh_code_findLabelId(ctx, pc, 1);
		((knh_share_t*)ctx->share)->PC_FUNCCALL = knh_code_findLabelId(ctx, pc, 2);
		((knh_share_t*)ctx->share)->PC_VEXEC = knh_code_findLabelId(ctx, pc, 3);
		((knh_share_t*)ctx->share)->PC_ABSTRACT = knh_code_findLabelId(ctx, pc, 4);
	}
	END_LOCAL(ctx, lsfp);
}

/* ------------------------------------------------------------------------ */

typedef struct knh_funcname_t {
	void *func;
	char *name;
} knh_funcname_t;
#define _FUNC(func, name) { (void*) func, (char *) name }

static struct knh_funcname_t _FuncData[] = {
	_FUNC(_PRINT, "PRINT"), _FUNC(_BOX, "BOX"), _FUNC(_NEW, "NEW"),
	_FUNC(_NULVAL, "NULL"), _FUNC(_SYS, "SYS"),
	_FUNC(klr_setMethod, "setMethod"), _FUNC(klr_lookupMethod, "lookupMethod"),
	_FUNC(_PROP, "PROP"), _FUNC(_bBOX, "bBOX"), _FUNC(_VARGS, "VARGS"),
	_FUNC(_ERR, "ERR"), _FUNC(_CHKTYPE, "CHKTYPE"),
	_FUNC(klr_checkParams, "checkParams"), _FUNC(klr_lookupMethod, "invokeFunc"),
	_FUNC(NULL, NULL),
};

void knh_write_vmfunc(Ctx *ctx, knh_OutputStream_t *w, void *f)
{
	knh_funcname_t *d = _FuncData;
	while(d->func != NULL) {
		if(d->func == f) {
			knh_write(ctx, w, B(d->name)); return;
		}
		d++;
	}
	knh_printf(ctx, w, "func:%p", f);
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
