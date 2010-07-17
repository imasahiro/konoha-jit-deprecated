/****************************************************************************
 * KONOHA COPYRIGHT, LICENSE NOTICE, AND DISCRIMER
 *
 * Copyright (c) 2006-2010, Kimio Kuramitsu <kimio at ynu.ac.jp>
 *           (c) 2008-      Konoha Team konohaken@googlegroups.com
 * All rights reserved.
 * You may choose one of the following two licenses when you use konoha.
 * See www.konohaware.org/license.html for further information.
 *
 * (1) GNU Lesser General Public License 3.0 (with K_UNDER_LGPL)
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

#define K_INLINECODE    (10)

static void TERMs_asm(Ctx *ctx, knh_Stmt_t *stmt, size_t n, knh_type_t reqt, int local);
static void knh_Stmt_asmBLOCK(Ctx *ctx, knh_Stmt_t *stmtH, knh_type_t reqt);
#define knh_GammaLabel(ctx, n)   (knh_BasicBlock_t*)knh_Array_n(DP(ctx->gma)->lstacks, n)

#define HAS_OPCODE(C)     ctx->share->jitSPI->isOPCODE(OPCODE_##C)

static knh_BasicBlock_t* new_BasicBlockLABEL(Ctx *ctx)
{
	knh_BasicBlock_t *bb = new_(BasicBlock);
	bb->listNC = DP(ctx->gma)->insts;
	DP(bb)->id = knh_Array_size(bb->listNC);
	knh_Array_add(ctx, DP(ctx->gma)->insts, bb);
	return bb;
}

#if defined(K_USING_THREADEDCODE)
#define TADDR   NULL, 0/*counter*/
#else
#define TADDR   0/*counter*/
#endif/*K_USING_THREADEDCODE*/

#define ASMLINE  0

#define KNH_ASM(T, ...) { \
		klr_##T##_t op_ = {TADDR, OPCODE_##T, ASMLINE, ## __VA_ARGS__}; \
		knh_Gamma_asm(ctx, (knh_opline_t*)(&op_)); \
	}\

#define KNH_ASMOP(T, OP, ...) { \
		klr_##T##_t op_ = {TADDR, OP, ASMLINE, ## __VA_ARGS__}; \
		knh_Gamma_asm(ctx, (knh_opline_t*)(&op_)); \
	}\

#define KNH_ASM_BRANCH(T, lb, ...) { \
		klr_##T##_t op_ = {TADDR, OPCODE_##T, ASMLINE, NULL, ## __VA_ARGS__}; \
		KNH_ASM_BRANCH_(ctx, lb, (knh_opline_t*)(&op_)); \
	}\

#define knh_BasicBlock_add(ctx, bb, T, ...) { \
		klr_##T##_t op_ = {TADDR, OPCODE_##T, ASMLINE, ## __VA_ARGS__};\
		knh_BasicBlock_add_(ctx, bb, 0, (knh_opline_t*)(&op_));\
	}\

static void knh_BasicBlock_expand(Ctx *ctx, knh_BasicBlock_t *bb, size_t newsize)
{
	knh_opline_t* newbuf = (knh_opline_t*)KNH_MALLOC(ctx, sizeof(knh_opline_t) * newsize);
	knh_memcpy(newbuf, DP(bb)->opbuf, DP(bb)->capacity * sizeof(knh_opline_t));
	KNH_FREE(ctx, DP(bb)->opbuf, DP(bb)->capacity * sizeof(knh_opline_t));
	DP(bb)->opbuf = newbuf;
	DP(bb)->capacity = newsize;
}

#if defined(K_USING_RCGC)
static void knh_ftraverse_inc(Ctx* ctx, Object *o)
{
	knh_Object_RCinc(o);
}
#endif

static void knh_BasicBlock_add_(Ctx *ctx, knh_BasicBlock_t *bb, int line, knh_opline_t *op)
{
	if(DP(bb)->capacity == 0) {
		DP(bb)->opbuf = (knh_opline_t*)KNH_MALLOC(ctx, sizeof(knh_opline_t));
		DP(bb)->capacity = 1;
	}
	else if(DP(bb)->capacity == 1) {
		knh_BasicBlock_expand(ctx, bb, 4);
	}
	else if(DP(bb)->size == DP(bb)->capacity) {
		knh_BasicBlock_expand(ctx, bb, DP(bb)->capacity * 2);
	}
	{
		knh_opline_t *pc = DP(bb)->opbuf + DP(bb)->size;
		knh_memcpy(pc, op, sizeof(knh_opline_t));
		pc->line = (knh_ushort_t)line;
#if defined(K_USING_RCGC)
		knh_opline_traverse(ctx, pc, knh_ftraverse_inc);
#endif
		DP(bb)->size += 1;
	}
}

static KLRAPI(void) _bBOX(Ctx *ctx, knh_sfp_t *sfp, knh_sfpidx_t c, knh_class_t cid);
static KLRAPI(void) _ifBOX(Ctx *ctx, knh_sfp_t *sfp, knh_sfpidx_t c, knh_class_t cid);
static KLRAPI(void) _OBOX(Ctx *ctx, knh_sfp_t *sfp, knh_sfpidx_t c, knh_class_t cid);

static void knh_Gamma_asm(Ctx *ctx, knh_opline_t *op)
{
	knh_BasicBlock_t *bb = DP(ctx->gma)->bbNC;
	DBG_ASSERT(op->opcode != OPCODE_JMPF);
	if(op->opcode == OPCODE_iADDn && HAS_OPCODE(iINC)) {
		klr_iADDn_t *opN = (klr_iADDn_t*)op;
		if(opN->a == opN->c && opN->n == 1) {
			op->opcode = OPCODE_iINC;
		}
	}
	if(op->opcode == OPCODE_iSUBn && HAS_OPCODE(iDEC)) {
		klr_iSUBn_t *opN = (klr_iSUBn_t*)op;
		if(opN->a == opN->c && opN->n == 1) {
			op->opcode = OPCODE_iDEC;
		}
	}
	if(DP(bb)->size > 0) {
		knh_opline_t *opP = DP(bb)->opbuf + (DP(bb)->size - 1);
		if(op->opcode == opP->opcode) {
			size_t i, size = knh_opcode_size(op->opcode);
			for(i = 0; i < size; i++) {
				if(op->data[i] != opP->data[i]) goto L_REMOVE;
			}
			DBG_P("PEEPHOLE: removed same one");
			return;
		}
		L_REMOVE:;
		if(opP->opcode == OPCODE_UNBOX && op->opcode == OPCODE_TR) {
			klr_UNBOX_t *opUNBOX = (klr_UNBOX_t*)opP;
			klr_TR_t *opTR = (klr_TR_t*)op;
			if(opUNBOX->b == opTR->a && (opTR->tr == _OBOX || opTR->tr == _ifBOX || opTR->tr == _bBOX)) {
				DBG_ASSERT(opUNBOX->a == opTR->b); // CHECK Object o = 1;
				DBG_P("PEEPHOLE: removed _BOX"); // this is safe, however
				DP(bb)->size -= 1;
				DBG_P("PEEPHOLE: removed UNBOX. IS OK?"); // is it ok?
				return;
			}
		}
		if(op->opcode == OPCODE_NMOV) {
			if(opP->opcode == OPCODE_NMOV && HAS_OPCODE(NNMOV)) {
				klr_NNMOV_t *opMOV = (klr_NNMOV_t*)opP;
				opMOV->c = ((klr_NMOV_t*)op)->a;
				opMOV->d = ((klr_NMOV_t*)op)->b;
				opP->opcode = OPCODE_NNMOV;
				return;
			}
			if(opP->opcode == OPCODE_OMOV && HAS_OPCODE(ONMOV)) {
				klr_ONMOV_t *opMOV = (klr_ONMOV_t *)opP;
				opMOV->c = ((klr_NMOV_t*)op)->a;
				opMOV->d = ((klr_NMOV_t*)op)->b;
				opP->opcode = OPCODE_ONMOV;
				return;
			}
		}
		if(op->opcode == OPCODE_OMOV) {
			if(opP->opcode == OPCODE_OMOV && HAS_OPCODE(OOMOV)) {
				klr_OOMOV_t *opMOV = (klr_OOMOV_t*)opP;
				opMOV->c = ((klr_OMOV_t*)op)->a;
				opMOV->d = ((klr_OMOV_t*)op)->b;
				opP->opcode = OPCODE_OOMOV;
				return;
			}
			if(opP->opcode == OPCODE_OMOV && HAS_OPCODE(ONMOV)) {
				klr_ONMOV_t *opMOV = (klr_ONMOV_t *)opP;
				opMOV->c = opMOV->a;
				opMOV->d = opMOV->b;
				opMOV->a = ((klr_OMOV_t*)op)->a;
				opMOV->b = ((klr_OMOV_t*)op)->b;
				opP->opcode = OPCODE_ONMOV;
				return;
			}
		}
	}
	knh_BasicBlock_add_(ctx, bb, SP(ctx->gma)->line, op);
}

static int knh_Gamma_asmJMPF(Ctx *ctx, klr_JMPF_t *op)
{
	knh_BasicBlock_t *bb = DP(ctx->gma)->bbNC;
	DBG_ASSERT(op->opcode == OPCODE_JMPF);
	int swap = 0;
	while(DP(bb)->size > 0) {
		knh_opline_t *opP = DP(bb)->opbuf + (DP(bb)->size - 1);
		if(opP->opcode == OPCODE_bNOT) {
			klr_bNOT_t *opN = (klr_bNOT_t*)opP;
			DBG_P("REWRITE JMPF index %d => %d", op->a, opN->a);
			op->a = opN->a;
			swap = (swap == 0) ? 1 : 0;
			DP(bb)->size -= 1;
			continue;
		}
		if(OPCODE_iEQ <= opP->opcode && opP->opcode <= OPCODE_iGTE && HAS_OPCODE(iJEQ)) {
			klr_iJEQ_t *opN = (klr_iJEQ_t*)opP;
			knh_sfpidx_t a = ((klr_iEQ_t*)opP)->a;
			knh_sfpidx_t b = ((klr_iEQ_t*)opP)->b;
			opN->jumppc = ((klr_JMP_t*)op)->jumppc;
			opN->a = a; opN->b = b;
			opP->opcode = OPCODE_iJEQ + ((opP)->opcode - OPCODE_iEQ);
			return swap;
		}
		if(OPCODE_iEQn <= opP->opcode && opP->opcode <= OPCODE_iGTEn && HAS_OPCODE(iJEQn)) {
			klr_iJEQn_t *opN = (klr_iJEQn_t*)opP;
			knh_sfpidx_t a = ((klr_iEQn_t*)opP)->a;
			knh_int_t n = ((klr_iEQn_t*)opP)->n;
			opN->jumppc = ((klr_JMP_t*)op)->jumppc;
			opN->a = a; opN->n = n;
			opP->opcode = OPCODE_iJEQn + ((opP)->opcode - OPCODE_iEQn);
			return swap;
		}
		if(OPCODE_fEQ <= opP->opcode && opP->opcode <= OPCODE_fGTE && HAS_OPCODE(fJEQ)) {
			klr_fJEQ_t *opN = (klr_fJEQ_t*)opP;
			knh_sfpidx_t a = ((klr_fEQ_t*)opP)->a;
			knh_sfpidx_t b = ((klr_fEQ_t*)opP)->b;
			opN->jumppc = ((klr_JMP_t*)op)->jumppc;
			opN->a = a; opN->b = b;
			opP->opcode = OPCODE_fJEQ + ((opP)->opcode - OPCODE_fEQ);
			return swap;
		}
		if(OPCODE_fEQn <= opP->opcode && opP->opcode <= OPCODE_fGTEn && HAS_OPCODE(fJEQn)) {
			klr_fJEQn_t *opN = (klr_fJEQn_t*)opP;
			knh_sfpidx_t a = ((klr_fEQn_t*)opP)->a;
			knh_float_t n = ((klr_fEQn_t*)opP)->n;
			opN->jumppc = ((klr_JMP_t*)op)->jumppc;
			opN->a = a; opN->n = n;
			opP->opcode = OPCODE_fJEQn + ((opP)->opcode - OPCODE_fEQn);
			return swap;
		}
		break;
	}
	knh_BasicBlock_add_(ctx, bb, SP(ctx->gma)->line, (knh_opline_t*)op);
	return swap;
}

/* ------------------------------------------------------------------------ */
/* new_KLRCode */

static knh_opcode_t knh_BasicBlock_opcode(knh_BasicBlock_t *bb)
{
	if(DP(bb)->size == 0) return OPCODE_NOP;
	return DP(bb)->opbuf->opcode;
}

#define BB(bb)   knh_opcode_tochar(knh_BasicBlock_opcode(bb))

static void dumpBB(knh_BasicBlock_t *bb, const char *indent)
{
	size_t i;
	DBG_P("%sid=%i, size=%d", indent, DP(bb)->id, DP(bb)->size);
	if(bb->nextNC != NULL) {
		DBG_P("%s\tnext=%d", indent, DP(bb->nextNC)->id);
		if(indent[0] == 0) dumpBB(bb->nextNC, "\t");
	}
	if(bb->jumpNC != NULL) {
		DBG_P("%s\tjump=%d", indent, DP(bb->jumpNC)->id);
		if(indent[0] == 0) dumpBB(bb->jumpNC, "\t");
	}
	for(i = 0; i < DP(bb)->size; i++) {
		knh_opline_t *op = DP(bb)->opbuf + i;
		DBG_P("%s\t opcode=%s", indent, knh_opcode_tochar(op->opcode));
		(void)op;
	}
}

static void knh_BasicBlock_strip0(Ctx *ctx, knh_BasicBlock_t *bb)
{
	L_TAIL:;
	if(knh_BasicBlock_isVisited(bb)) return;
	knh_BasicBlock_setVisited(bb, 1);
	if(bb->jumpNC != NULL) {
		L_JUMP:;
		knh_BasicBlock_t *bbJ = bb->jumpNC;
		if(DP(bbJ)->size == 0 && bbJ->jumpNC != NULL && bbJ->nextNC == NULL) {
			//DBG_P("DIRECT JMP id=%d JMP to id=%d", DP(bbJ)->id, DP(bbJ->jumpNC)->id);
			DP(bbJ)->incoming -= 1;
			bb->jumpNC = bbJ->jumpNC;
			DP(bb->jumpNC)->incoming += 1;
			goto L_JUMP;
		}
		if(DP(bbJ)->size == 0 && bbJ->jumpNC == NULL && bbJ->nextNC != NULL) {
			//DBG_P("DIRECT JMP id=%d NEXT to id=%d", DP(bbJ)->id, DP(bbJ->nextNC)->id);
			DP(bbJ)->incoming -= 1;
			bb->jumpNC = bbJ->nextNC;
			DP(bb->jumpNC)->incoming += 1;
			goto L_JUMP;
		}
		if(bb->nextNC == NULL) {
			if(DP(bbJ)->incoming == 1 ) {
				//DBG_P("REMOVED %d JMP TO %d", DP(bb)->id, DP(bbJ)->id);
				bb->nextNC = bbJ;
				bb->jumpNC = NULL;
				goto L_NEXT;
			}
		}
		knh_BasicBlock_strip0(ctx, bbJ);
	}
	if(bb->jumpNC != NULL && bb->nextNC != NULL) {
		bb = bb->nextNC;
		goto L_TAIL;
	}
	L_NEXT:;
	if(bb->nextNC != NULL) {
		knh_BasicBlock_t *bbN = bb->nextNC;
		if(DP(bbN)->size == 0 && bbN->nextNC != NULL && bbN->jumpNC == NULL) {
			//DBG_P("DIRECT NEXT id=%d to NEXT id=%d", DP(bbN)->id, DP(bbN->nextNC)->id);
			DP(bbN)->incoming -= 1;
			bb->nextNC = bbN->nextNC;
			DP(bb->nextNC)->incoming += 1;
			goto L_NEXT;
		}
		if(DP(bbN)->size == 0 && bbN->nextNC == NULL && bbN->jumpNC != NULL) {
			//DBG_P("DIRECT NEXT id=%d to JUMP id=%d", DP(bbN)->id, DP(bbN->jumpNC)->id);
			DP(bbN)->incoming -= 1;
			bb->nextNC = NULL;
			bb->jumpNC = bbN->jumpNC;
			DP(bb->jumpNC)->incoming += 1;
			goto L_JUMP;
		}
		bb = bb->nextNC;
		goto L_TAIL;
	}
}

static void knh_BasicBlock_freebuf(Ctx *ctx, knh_BasicBlock_t *bb)
{
	KNH_FREE(ctx, DP(bb)->opbuf, sizeof(knh_opline_t) * DP(bb)->capacity);
	DP(bb)->capacity = 0;
	DP(bb)->size = 0;
	DP(bb)->opbuf = NULL;
}

static void knh_BasicBlock_join(Ctx *ctx, knh_BasicBlock_t *bb, knh_BasicBlock_t *bbN)
{
	DBG_P("join %d(%s) size=%d and %d(%s) size=%d", DP(bb)->id, BB(bb), DP(bb)->size, DP(bbN)->id, BB(bbN), DP(bbN)->size);
	bb->nextNC = bbN->nextNC;
	bb->jumpNC = bbN->jumpNC;
	if(DP(bbN)->size == 0) {
		return;
	}
	if(DP(bb)->size == 0) {
		DBG_ASSERT(DP(bb)->capacity == 0);
		DP(bb)->opbuf = DP(bbN)->opbuf;
		DP(bb)->capacity = DP(bbN)->capacity;
		DP(bb)->size = DP(bbN)->size;
		DP(bbN)->opbuf = NULL;
		DP(bbN)->capacity = 0;
		DP(bbN)->size = 0;
		return;
	}
	if(DP(bb)->capacity < DP(bb)->size + DP(bbN)->size) {
		knh_BasicBlock_expand(ctx, bb, DP(bb)->size + DP(bbN)->size);
	}
	knh_memcpy(DP(bb)->opbuf + DP(bb)->size, DP(bbN)->opbuf, sizeof(knh_opline_t) * DP(bbN)->size);
	DP(bb)->size += DP(bbN)->size;
	knh_BasicBlock_freebuf(ctx, bbN);
}

static void knh_BasicBlock_strip1(Ctx *ctx, knh_BasicBlock_t *bb)
{
	L_TAIL:;
	if(!knh_BasicBlock_isVisited(bb)) return;
	knh_BasicBlock_setVisited(bb, 0);  // MUST call after strip0
	if(bb->jumpNC != NULL) {
		if(bb->nextNC == NULL) {
			bb = bb->jumpNC;
			goto L_TAIL;
		}
		else {
			//DBG_P("** branch next=%d, jump%d", DP(bb->nextNC)->id, DP(bb->jumpNC)->id);
			knh_BasicBlock_strip1(ctx, bb->jumpNC);
			bb = bb->nextNC;
			goto L_TAIL;
		}
	}
	if(bb->nextNC != NULL) {
		knh_BasicBlock_t *bbN = bb->nextNC;
		if(DP(bbN)->incoming == 1 && knh_BasicBlock_opcode(bbN) != OPCODE_RET) {
			knh_BasicBlock_join(ctx, bb, bbN);
			knh_BasicBlock_setVisited(bb, 1);
			goto L_TAIL;
		}
		bb = bb->nextNC;
		goto L_TAIL;
	}
}

static size_t knh_BasicBlock_size(Ctx *ctx, knh_BasicBlock_t *bb, size_t c)
{
	L_TAIL:;
	if(bb == NULL || knh_BasicBlock_isVisited(bb)) return c;
	knh_BasicBlock_setVisited(bb, 1);
	if(bb->jumpNC != NULL && bb->nextNC != NULL) {
		c = knh_BasicBlock_size(ctx, bb->nextNC, c + DP(bb)->size);
		bb = bb->jumpNC; goto L_TAIL;
	}
	if(bb->jumpNC != NULL) { DBG_ASSERT(bb->nextNC == NULL);
		if(knh_BasicBlock_opcode(bb->jumpNC) == OPCODE_RET) {
			knh_BasicBlock_add(ctx, bb, JMP_);
		}
		else {
			knh_BasicBlock_add(ctx, bb, JMP);
		}
		c = DP(bb)->size + c; bb = bb->jumpNC;
		goto L_TAIL;
	}
	if(bb->nextNC != NULL &&
		(knh_BasicBlock_isVisited(bb) || knh_BasicBlock_opcode(bb->nextNC) == OPCODE_RET)) {
		DBG_P("INSERT JMP");
		knh_BasicBlock_t *bb2 = new_BasicBlockLABEL(ctx);
		bb2->jumpNC = bb->nextNC;
		bb->nextNC = bb2;
	}
	c = DP(bb)->size + c;
	bb = bb->nextNC;
	goto L_TAIL;
}

static knh_opline_t* knh_BasicBlock_copy(Ctx *ctx, knh_opline_t *dst, knh_BasicBlock_t *bb, knh_BasicBlock_t **prev)
{
	knh_BasicBlock_setVisited(bb, 0);
	DBG_ASSERT(!knh_BasicBlock_isVisited(bb));
	if(DP(bb)->code != NULL) return dst;
	if(prev[0] != NULL && prev[0]->nextNC == NULL && prev[0]->jumpNC == bb) {
		dst -= 1;
		DBG_P("REMOVE unnecessary JMP/(?%s)", knh_opcode_tochar(dst->opcode));
		DBG_ASSERT(dst->opcode == OPCODE_JMP || dst->opcode == OPCODE_JMP_);
		prev[0]->jumpNC = NULL;
		prev[0]->nextNC = bb;
	}
	DP(bb)->code = dst;
	if(DP(bb)->size > 0) {
		size_t i;
		knh_memcpy(dst, DP(bb)->opbuf, sizeof(knh_opline_t) * DP(bb)->size);
		if(bb->jumpNC != NULL) {
			DP(bb)->opjmp = (dst + (DP(bb)->size - 1));
			DBG_ASSERT(knh_opcode_hasjump(DP(bb)->opjmp->opcode));
		}
		for(i = 0; i < DP(bb)->size; i++) {
			knh_opline_t *op = dst + i;
			if(op->opcode == OPCODE_VCALL) {
				if(knh_BasicBlock_isStackChecked(bb)) {
					op->opcode = OPCODE_VCALL_;
				}
				else {
					knh_BasicBlock_setStackChecked(bb, 1);
				}
			}
		}
		dst = dst + DP(bb)->size;
		knh_BasicBlock_freebuf(ctx, bb);
		prev[0] = bb;
		DBG_P("asm bb=%d, %p, %s nextNC=%p", DP(bb)->id, dst, BB(bb), bb->nextNC);
	}
	if(bb->nextNC != NULL) {
		if(knh_BasicBlock_isStackChecked(bb) && DP(bb->nextNC)->incoming == 1) {
			knh_BasicBlock_setStackChecked(bb->nextNC, 1);
		}
		dst = knh_BasicBlock_copy(ctx, dst, bb->nextNC, prev);
	}
	if(bb->jumpNC != NULL) {
		if(knh_BasicBlock_isStackChecked(bb) && DP(bb->jumpNC)->incoming == 1) {
			knh_BasicBlock_setStackChecked(bb->jumpNC, 1);
		}
		dst = knh_BasicBlock_copy(ctx, dst, bb->jumpNC, prev);
	}
	return dst;
}

static void knh_BasicBlock_setjump(knh_BasicBlock_t *bb)
{
	while(bb != NULL) {
		knh_BasicBlock_setVisited(bb, 1);
		if(bb->jumpNC != NULL) {
			knh_BasicBlock_t *bbJ = bb->jumpNC;
			klr_JMP_t *j = (klr_JMP_t*)DP(bb)->opjmp;
			j->jumppc = DP(bbJ)->code;
			DBG_P("jump from id=%d to id=%d %s jumppc=%p", DP(bb)->id, DP(bbJ)->id, knh_opcode_tochar(j->opcode), DP(bbJ)->code);
			bb->jumpNC = NULL;
			if(!knh_BasicBlock_isVisited(bbJ)) {
				knh_BasicBlock_setVisited(bbJ, 1);
				knh_BasicBlock_setjump(bbJ);
			}
		}
		bb = bb->nextNC;
	}
}

static knh_KLRCode_t* knh_BasicBlock_link(Ctx *ctx, knh_BasicBlock_t *bb, knh_BasicBlock_t *bbRET)
{
	knh_KLRCode_t *kcode = new_(KLRCode);
	knh_BasicBlock_t *prev[1] = {};
	SP(kcode)->uri = SP(ctx->gma)->uri;
	SP(kcode)->codesize = knh_BasicBlock_size(ctx, bb, 0) * sizeof(knh_opline_t);
	SP(kcode)->code = (knh_opline_t*)KNH_MALLOC(ctx, SP(kcode)->codesize);
	DP(bbRET)->code = SP(kcode)->code; // dummy
	{
		knh_opline_t *op = knh_BasicBlock_copy(ctx, SP(kcode)->code, bb, prev);
		DBG_ASSERT(op - kcode->code > 0);
		DP(bbRET)->code = NULL;
		knh_BasicBlock_copy(ctx, op, bbRET, prev);
		knh_BasicBlock_setjump(bb);
	}
	return kcode;
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
		knh_Method_t *mtdf = knh_getSystemFormatter(ctx, CLASS_Method, MN__dump);
		knh_write_Object(ctx, KNH_STDOUT, ctx->esp, &mtdf, UPCAST(mtd));
		knh_write_EOL(ctx, KNH_STDOUT);
	}
}

static void knh_Gamma_gc(Ctx *ctx)
{
	KNH_SETv(ctx, DP(ctx->gma)->mtd, KNH_NULL);
	knh_Array_clear(ctx, DP(ctx->gma)->lstacks, 0);
	knh_Array_clear(ctx, DP(ctx->gma)->insts, 0);
}

static void knh_Gamma_compile(Ctx *ctx, knh_BasicBlock_t *bb, knh_BasicBlock_t *bbRET)
{
	knh_Method_t *mtd = DP(ctx->gma)->mtd;
	knh_BasicBlock_strip0(ctx, bb);
	knh_BasicBlock_strip1(ctx, bb);
	knh_KLRCode_t *kcode = knh_BasicBlock_link(ctx, bb, bbRET);
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

static void KNH_LABEL(Ctx *ctx, knh_BasicBlock_t *label)
{
	if(label != NULL) {
		knh_BasicBlock_t *bb = DP(ctx->gma)->bbNC;
		if(bb != NULL) {
			bb->nextNC = label;
			DP(label)->incoming += 1;
			bb->jumpNC = NULL;
		}
		DP(ctx->gma)->bbNC = label;
	}
}

static void KNH_ASM_JMP(Ctx *ctx, knh_BasicBlock_t *label)
{
	knh_BasicBlock_t *bb = DP(ctx->gma)->bbNC;
	if(bb != NULL) {
		bb->nextNC = NULL;
		bb->jumpNC = label;  DP(label)->incoming += 1;
	}
	DP(ctx->gma)->bbNC = NULL; /*KNH_TNULL(BasicBlock);*/
}

static knh_BasicBlock_t* KNH_ASM_JMPF(Ctx *ctx, int sfpidx, knh_BasicBlock_t *lbJUMP)
{
	knh_BasicBlock_t *bb = DP(ctx->gma)->bbNC;
	knh_BasicBlock_t *lbNEXT = new_BasicBlockLABEL(ctx);
	klr_JMPF_t op = {TADDR, OPCODE_JMPF, ASMLINE, NULL, sfpidx};
	if(knh_Gamma_asmJMPF(ctx, &op)) {
		bb->jumpNC = lbNEXT;
		bb->nextNC = lbJUMP;
	}
	else {
		bb->jumpNC = lbJUMP;
		bb->nextNC = lbNEXT;
	}
	DP(lbJUMP)->incoming += 1;
	DP(lbNEXT)->incoming += 1;
	DP(ctx->gma)->bbNC = lbNEXT;
	return lbJUMP;
}

static void KNH_ASM_BRANCH_(Ctx *ctx, knh_BasicBlock_t *jump, knh_opline_t *op)
{
	knh_BasicBlock_t *bb = DP(ctx->gma)->bbNC;
	knh_BasicBlock_t *newbb = new_BasicBlockLABEL(ctx);
	knh_Gamma_asm(ctx, op);
	bb->jumpNC = jump;  DP(jump)->incoming += 1;
	bb->nextNC = newbb;  DP(newbb)->incoming += 1;
	DP(ctx->gma)->bbNC = newbb;
}

static void KNH_ASM_RET(Ctx *ctx)
{
	knh_BasicBlock_t *bbEND = knh_GammaLabel(ctx,  2);
	KNH_ASM_JMP(ctx, bbEND);
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
static KLRAPI(void) _ifBOX(Ctx *ctx, knh_sfp_t *sfp, knh_sfpidx_t c, knh_class_t cid)
{
	Object *v = new_Object_boxing(ctx, cid, sfp);
	klr_mov(ctx, sfp[c].o, v);
}
static KLRAPI(void) _OBOX(Ctx *ctx, knh_sfp_t *sfp, knh_sfpidx_t c, knh_class_t cid)
{
	knh_class_t cid2 = knh_Object_cid(sfp[0].o);
	if(IS_Tunbox(cid2)) {
		Object *v = new_Object_boxing(ctx, cid2, sfp);
		DBG_P("sfp: ivalue=%lld fvalue=%f bvalue=%d", sfp[0].ivalue, sfp[0].fvalue, (int)sfp[0].bvalue);
		DBG_P("box: ivalue=%lld fvalue=%f bvalue=%d", ((knh_Int_t*)v)->n.ivalue, ((knh_Int_t*)v)->n.fvalue, (int)((knh_Int_t*)v)->n.bvalue);
		klr_mov(ctx, sfp[c].o, v);
	}
}
static KLRAPI(void) _CWB(Ctx *ctx, knh_sfp_t *sfp, knh_sfpidx_t c, knh_class_t cid)
{
	size_t pos = BA_size(ctx->bufa);
	klr_mov(ctx, sfp[c].o, ctx->bufw);
	sfp[c].ivalue = pos;
	DBG_P("sfpidx=%d, pos=%zd", (sfp + c) - ctx->stack, pos);
}
static KLRAPI(void) _TOSTR(Ctx *ctx, knh_sfp_t *sfp, knh_sfpidx_t c, knh_class_t cid)
{
	DBG_ASSERT(IS_OutputStream(sfp[0].w));
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	cwb->pos = (size_t)(sfp[0].ivalue); // reset
	DBG_P("sfpidx=%d, pos=%zd", (sfp) - ctx->stack, cwb->pos);
	knh_String_t *s = knh_cwb_newString(ctx, cwb);
	klr_mov(ctx, sfp[c].o, s);
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

static void KNH_ASM_BOX2(Ctx *ctx, knh_type_t reqt, knh_type_t atype, int a)
{
	knh_class_t cid = CLASS_type(atype);
	knh_class_t bcid = ClassTBL(cid).bcid;
	if(bcid == CLASS_Boolean || bcid == CLASS_Int || bcid == CLASS_Float) {
		knh_class_t rcid = ClassTBL(CLASS_type(reqt)).bcid;
		if(rcid != bcid && reqt != TYPE_void) {
			if(cid == CLASS_Boolean) {
				KNH_ASM(TR, a, a, cid, _bBOX);
			}
			else if (IS_Tnumbox(reqt)) {
				Object *v = (cid == CLASS_Int) ? UPCAST(KNH_INT0) : UPCAST(KNH_FLOAT0);
				KNH_ASM(OSET, a, v);
			}
			else {
				KNH_ASM(TR, a, a, cid, _ifBOX);
			}
		}
	}
	else if(atype == CLASS_Any && IS_Tnumbox(reqt)) {
		KNH_ASM(UNBOX, a, a);
	}
	else if(IS_Tnumbox(atype) && reqt == CLASS_Any) {
		KNH_ASM(TR, a, a, atype, _OBOX);
	}
}

static void KNH_ASM_UNBOX2(Ctx *ctx, knh_type_t atype, int a)
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
			KNH_ASM(OMOV, (sfpidx), (local));
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
		KNH_ASM_BOX2(ctx, atype, btype, (a));
	}
	else if(IS_Tfloat(btype)) {
		KNH_ASM(fMOVx, (a), bx);
		KNH_ASM_BOX2(ctx, atype, btype, (a));
	}
	else if(IS_Tbool(btype)) {
		KNH_ASM(bMOVx, (a), bx);
		KNH_ASM_BOX2(ctx, atype, btype, (a));
	}
	else {
		KNH_ASM(OMOVx, (a), bx);
		if(IS_Tnumbox(btype)) {
			KNH_ASM(UNBOX, a, a);
		}
	}
}

static void KNH_ASM_SMOV(Ctx *ctx, knh_type_t atype, int a, knh_Token_t *tkb)
{
	knh_type_t btype = SP(tkb)->type;
	switch(TT_(tkb)) {
		case TT_NULL/*DEFVAL*/: {
			knh_class_t cid = DP(tkb)->cid;
			knh_Object_t *v = KNH_NULVAL(cid);
			if(v != ClassTBL(cid).defnull) {
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
			else if(IS_Tnumbox(atype)) {
				KNH_ASM(NSET, (a), knh_Object_data(DP(tkb)->data));
				KNH_ASM(OSET, (a), v);
			}
			else {
				DBG_P("****************** atype=%s", TYPE__(atype));
				KNH_ASM(OSET, (a), v);
				//KNH_ASM_UNBOX(ctx, SP(tkb)->type, a);
			}
			break;
		}
		case TT_LOCAL: {
			int b = (int)DP(tkb)->index;
			knh_type_t btype = SP(tkb)->type;
			if(IS_Tunbox(btype)) {
				KNH_ASM(NMOV, (a), (b));
				KNH_ASM_BOX2(ctx, atype, btype, a);
			}
			else {
				KNH_ASM(OMOV, (a), (b));
				if(IS_Tnumbox(btype)) {
					if(IS_Tnumbox(atype)) {
						KNH_ASM(NMOV, (a), (b));
					}
					else {
						KNH_ASM(TR, a, b, atype, _OBOX);
					}
				}
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
			KNH_ASM_BOX2(ctx, atype, btype, a);
			break;
		}
		case TT_PROPN: {
			KNH_ASM(OSET, (a), DP(tkb)->data);
			KNH_ASM(TR, a, a, CLASS_type(btype), _PROP);
			KNH_ASM_BOX2(ctx, atype, btype, a);
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
			KNH_ASM_UNBOX2(ctx, atype, espidx);
		}
		KNH_ASM(XIMOV, ax, espidx);
	}
	else if(IS_Tfloat(atype)) {
		if(IS_Tfloat(btype)) {
			KNH_ASM(fMOVx, espidx, bx);
		}
		else {
			KNH_ASM(OMOVx, espidx, bx);
			KNH_ASM_UNBOX2(ctx, atype, espidx);
		}
		KNH_ASM(XFMOV, ax, espidx);
	}
	else if(IS_Tbool(atype)) {
		if(IS_Tbool(btype)) {
			KNH_ASM(bMOVx, espidx, bx);
		}
		else {
			KNH_ASM(OMOVx, espidx, bx);
			KNH_ASM_UNBOX2(ctx, atype, espidx);
		}
		KNH_ASM(XBMOV, ax, espidx);
	}
	else {
		DBG_ASSERT(atype == TYPE_Any || IS_Tnumbox(atype));
		if(IS_Tint(btype)) { // Any a = b; // int b;
			KNH_ASM(iMOVx, espidx, bx);
			KNH_ASM(TR, espidx, espidx, CLASS_type(btype), _ifBOX);
			KNH_ASM(XMOV, ax, espidx);
		}
		else if(IS_Tfloat(btype)) { // Any a = b; // float b;
			KNH_ASM(fMOVx, espidx, bx);
			KNH_ASM(TR, espidx, espidx, CLASS_type(btype), _ifBOX);
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
			if(v != ClassTBL(cid).defnull) {
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
			KNH_ASM_BOX2(ctx, atype, btype, b);
			if(IS_Tint(atype)) {
				KNH_ASM(XIMOV, ax, (b));
			}
			else if(IS_Tfloat(atype)) {
				KNH_ASM(XFMOV, ax, (b));
			}
			else if(IS_Tbool(atype)) {
				KNH_ASM(XBMOV, ax, (b));
			}
			else {
				KNH_ASM(XMOV, ax, (b));
			}
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
			KNH_ASM_BOX2(ctx, atype, btype, espidx);
			break;
		}
		case TT_PROPN: {
			espidx = DP(ctx->gma)->espidx;
			KNH_ASM(OSET, espidx, DP(tkb)->data);
			KNH_ASM(TR, espidx, espidx, CLASS_type(atype), _PROP);
			KNH_ASM_BOX2(ctx, atype, btype, espidx);
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

static knh_BasicBlock_t* TERMs_JMPIF(Ctx *ctx, knh_Stmt_t *stmt, size_t n, int isTRUE, knh_BasicBlock_t* label, int local)
{
	knh_Token_t *tk = DP(stmt)->tokens[n];
	if(TT_(tk) == TT_CONST) {
		int isTRUE2 = IS_TRUE(DP(tk)->data);
		KNH_ASM_SMOV(ctx, TYPE_Boolean, local, tk);
		if((!isTRUE && !isTRUE2) || (isTRUE && isTRUE2)) {
			KNH_ASM_JMP(ctx, label);
			return label;
		}
		return NULL;
	}
	if(TT_(tk) == TT_LOCAL) {
		if(isTRUE) {
			KNH_ASM(bNOT, DP(tk)->index, DP(tk)->index);
		}
		return KNH_ASM_JMPF(ctx, DP(tk)->index, label);
	}
	else {
		TERMs_asm(ctx, stmt, n, TYPE_Boolean, local);
		if(isTRUE) {
			KNH_ASM(bNOT, local, local);
		}
		return KNH_ASM_JMPF(ctx, local, label);
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

static knh_bool_t knh_StmtOPR_hasCONST(Ctx *ctx, knh_Stmt_t *stmt, knh_methodn_t *mn, int swap)
{
	int isCONST = (TT_(DP(stmt)->terms[2]) == TT_CONST);
	if(swap == 1 && TT_(DP(stmt)->terms[1]) == TT_CONST) {
		knh_methodn_t newmn = *mn;
		knh_Stmt_swap(ctx, stmt, 1, 2);
		if(*mn == MN_opLT) newmn = MN_opGT;  /* 1 < n ==> n > 1 */
		else if(*mn == MN_opLTE) newmn = MN_opGTE; /* 1 <= n => n >= 1 */
		else if(*mn == MN_opGT) newmn = MN_opLT;
		else if(*mn == MN_opGTE) newmn = MN_opLTE;
		DBG_P("swap %s ==> %s", MN__(*mn), MN__(newmn));
		*mn = newmn;
		isCONST = 1;
	}
	return isCONST;
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
	if(cid == CLASS_Boolean && mn == MN_opNOT) {
		int a = TERMs_put(ctx, stmt, 1, TYPE_Boolean, local + 1);
		KNH_ASM(bNOT, (sfpidx), (a));
		return 1;
	}
	if(cid == CLASS_Int && ((opcode = OPCODE_imn(mn, 0)) != OPCODE_NOP)) {
		int swap = 1;
		if(mn == MN_opNEG) {
			int a = TERMs_put(ctx, stmt, 1, TYPE_Int, local + 1);
			KNH_ASM(iNEG, (sfpidx), (a));
			return 1;
		}
		if(mn == MN_opSUB || mn == MN_opDIV || mn == MN_opMOD) swap = 0;
		if(knh_StmtOPR_hasCONST(ctx, stmt, &mn, swap)) {
			int a = TERMs_put(ctx, stmt, 1, TYPE_Int, local + 1);
			knh_int_t b = TERMs_int(stmt, 2);
			if(b == 0 && (mn == MN_opDIV || mn == MN_opMOD)) {
				b = 1;
				knh_Gamma_perror(ctx, KERR_ERRATA, _("divided by zero: /0 ==> /1"));
			}
			opcode = OPCODE_imn(mn, (OPCODE_iADDn - OPCODE_iADD));
			//opcode = knh_opcode_shiftCONST(OPCODE_imn(mn), mn);
			KNH_ASMOP(iADDn, opcode, sfpidx, a, b);
		}
		else {
			int a = TERMs_put(ctx, stmt, 1, TYPE_Int, local + 1);
			int b = TERMs_put(ctx, stmt, 2, TYPE_Int, local + 2);
			KNH_ASMOP(iADD, opcode, sfpidx, a, b);
		}
		return 1;
	} /* CLASS_Int */
	if(cid == CLASS_Float && ((opcode = OPCODE_fmn(mn, 0)) != OPCODE_NOP)) {
		int swap = 1;
		if(mn == MN_opNEG) {
			int a = TERMs_put(ctx, stmt, 1, TYPE_Float, local + 1);
			KNH_ASM(fNEG, (sfpidx), (a));
			return 1;
		}
		if(mn == MN_opSUB || mn == MN_opDIV || mn == MN_opMOD) swap = 0;
		if(knh_StmtOPR_hasCONST(ctx, stmt, &mn, swap)) {
			int a = TERMs_put(ctx, stmt, 1, TYPE_Float, local + 1);
			knh_float_t b = TERMs_float(stmt, 2);
			if(b == K_FLOAT_ZERO && mn == MN_opDIV) {
				b = K_FLOAT_ONE;
				knh_Gamma_perror(ctx, KERR_ERRATA, _("divided by zero: 0.0 ==> 1.0"));
			}
			opcode = OPCODE_fmn(mn, (OPCODE_fADDn - OPCODE_fADD));
			//opcode = knh_opcode_shiftCONST(OPCODE_imn(mn), mn);
			KNH_ASMOP(fADDn, opcode, (sfpidx), a, b);
		}
		else {
			int a = TERMs_put(ctx, stmt, 1, TYPE_Float, local + 1);
			int b = TERMs_put(ctx, stmt, 2, TYPE_Float, local + 2);
			KNH_ASMOP(fADD, opcode, (sfpidx), a, b);
		}
		return 1;
	} /* CLASS_Float */
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
			DBG_P("** WATCH OUT ** mtd_cid=%s, DP(mtd)->cid=%s", CLASS__(cid), CLASS__(DP(mtd)->cid));
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
		knh_BasicBlock_t *lbBEGIN = knh_GammaLabel(ctx, 1);
		for(i = s; i < DP(stmt)->size; i++) {
			knh_type_t reqt = TERMs_ptype(ctx, stmt, i, cid, mtd);
			if(IS_Tunbox(reqt)) {
				KNH_ASM(NMOV, (i-1), local + i + 2);
			}
			else {
				KNH_ASM(OMOV, (i-1), local + i + 2);
			}
		}
		KNH_ASM_JMP(ctx, lbBEGIN);
		return 0;
	}
	return 1;
}

//static knh_bool_t knh_opset_isHeadOfBasicBlock(knh_opline_t *codeblock, void *cur)
//{
//	knh_opline_t *pc = codeblock;
//	while(pc->opcode != OPCODE_RET) {
//		if(knh_opcode_hasjump(pc->opcode)) {
//			if(pc->p[0] == cur) {
//				DBG_P("%s jumppc=%p, cur=%p", knh_opcode_tochar(pc->opcode), pc->p[0], cur);
//				return 1;
//			}
//		}
//		pc++;
//	}
//	return 0;
//}
//
//static void KNH_ASM_INLINE(Ctx *ctx, int sfpshift, knh_opline_t *codeblock, size_t isize)
//{
//	size_t i;
//	knh_BasicBlock_t *bb = new_BasicBlockLABEL(ctx);
//	DP(bb)->opbuf = (knh_opline_t*)KNH_MALLOC(ctx, sizeof(knh_opline_t) * isize);
//	DP(bb)->capacity = isize;
//	DP(bb)->size = isize;
//	for(i = 0; i < isize; i++) {
//		knh_opline_t *op = DP(bb)->opbuf + i;
//		knh_opcode_idxshift(op, sfpshift);
//		if(op->opcode == OPCODE_JMP_) {
//			op->opcode = OPCODE_JMP;
//		}
//	}
//	DP(bb)->code = codeblock;
//	knh_BasicBlock_
//			ix->code_pos = pc;
//			ix->op->count = 0;
//			knh_Array_add(ctx, a, ix);
//				ix->op->opcode = OPCODE_JMP;
//			}
//			if(pc->opcode == OPCODE_RET) break;
//			pc++;
//		}
//	}
//	{
//		size_t j;
//		for(j = start; j < knh_Array_size(a); j++) {
//			pc = a->insts[j]->op;
//			if(knh_opcode_hasjump(pc->opcode)) {
//				size_t i;
//				for(i = start; i < knh_Array_size(a); i++) {
//					knh_BasicBlock_t *ix = a->insts[i];
//					if(ix->opcode != OPCODE_LABEL) continue;
//					if(pc->p[0] == (void*)ix->code_pos) {
//						pc->p[0] = (void*)ix; break;
//					}
//				}
//				DBG_ASSERT(i != knh_Array_size(a));
//			}
//		}
//	}
//}

#define ESP_(sfpidx, args)   (sfpidx + args + K_CALLDELTA + 1)

static void KNH_ASM_CALL(Ctx *ctx, knh_type_t reqt, int sfpidx, knh_Method_t *mtd, int isStatic, size_t args)
{
	DBG_ASSERT(IS_Method(mtd));
	KNH_ASSERT(sfpidx >= DP(ctx->gma)->espidx);
	if(knh_Method_isFinal(mtd) || isStatic) {
		if(knh_Gamma_hasJIT(ctx)) {
			KNH_ASM(LDMETHOD, klr_setMethod, sfpidx+K_CALLDELTA, mtd);
			KNH_ASM(CALL, sfpidx, ESP_(sfpidx, args));
		}
		else if(knh_Method_isKLRCode(mtd) || DP(ctx->gma)->mtd == mtd) {
//			if(knh_Gamma_isInlineFunction(ctx->gma) && DP(ctx->gma)->mtd != mtd) {
//				knh_KLRCode_t *kcode = DP(mtd)->kcode;
//				size_t isize = kcode->codesize / sizeof(knh_opline_t);
//				if(isize < K_INLINECODE) {
//					knh_Gamma_perror(ctx, KERR_INFO, _("inlineing: %C.%M"), DP(mtd)->cid, DP(mtd)->mn);
//					KNH_ASM_INLINE(ctx, sfpidx + K_CALLDELTA, kcode->code + 1, isize - 2);
//					return;
//				}
//			}
			if(args == 0) {
				KNH_ASM(VCALL_, sfpidx, ESP_(sfpidx, args), mtd);
			}
			else {
				KNH_ASM(VCALL, sfpidx, ESP_(sfpidx, args), mtd);
			}
		}
		else {
			KNH_ASM(SCALL, sfpidx, ESP_(sfpidx, args), mtd);
		}
	}
	else {
		KNH_ASM(LDMETHOD, klr_lookupMethod, sfpidx+K_CALLDELTA, mtd);
		KNH_ASM(CALL, sfpidx, ESP_(sfpidx, args));
	}
}

static knh_Method_t* knh_lookupDynamicMethod(Ctx *ctx, knh_methodn_t mn)
{
	size_t cid, i, size = ctx->share->ClassTBLSize;
	for(cid = 0; cid < size; cid++) {
		knh_Array_t *a = ClassTBL(cid).methods;
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
		KNH_ASM(CALL, local, ESP_(local, DP(stmt)->size - 2));
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
		if(mtd_mn == MN_get) {
			int a = TERMs_put(ctx, stmt, 1, cid, local + 1);
			if(TERMs_isCONST(stmt, 2)) {
				knh_intptr_t n = (knh_intptr_t)TERMs_int(stmt, 2);
				if(n < 0) {
					goto L_USECALL;
				}
				if(IS_Tunbox(p1)) {
					KNH_ASM(NGETIDXn, (sfpidx), (a), n);
					KNH_ASM_BOX2(ctx, reqt, p1, sfpidx);
				}
				else {
					KNH_ASM(OGETIDXn, (sfpidx), (a), n);
					if(IS_Tnumbox(reqt)) {
						KNH_ASM(UNBOX, sfpidx, sfpidx);
					}
				}
			}
			else {
				int an = TERMs_put(ctx, stmt, 2, TYPE_Int, local + 2);
				if(IS_Tunbox(p1)) {
					KNH_ASM(NGETIDX, (sfpidx), (a), an);
					KNH_ASM_BOX2(ctx, reqt, p1, sfpidx);
				}
				else {
					KNH_ASM(OGETIDX, (sfpidx), (a), an);
					if(IS_Tnumbox(reqt)) {
						KNH_ASM(UNBOX, sfpidx, sfpidx);
					}
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
				if(n < 0) {
					goto L_USECALL;
				}
				knh_class_t p1 = knh_class_p1(cid);
				if(IS_Tunbox(p1)) {
					KNH_ASM(NSETIDXn, (sfpidx), (a), n, v);
					KNH_ASM_BOX2(ctx, reqt, p1, sfpidx);
				}
				else {
					KNH_ASM(OSETIDXn, (sfpidx), (a), n, v);
					if(IS_Tnumbox(reqt)) {
						KNH_ASM(UNBOX, sfpidx, sfpidx);
					}
				}
			}
			else {
				int an = TERMs_put(ctx, stmt, 2, TYPE_Int, local + 2);
				if(IS_Tunbox(p1)) {
					KNH_ASM(NSETIDXn, (sfpidx), (a), an, v);
					KNH_ASM_BOX2(ctx, reqt, p1, sfpidx);
				}
				else {
					KNH_ASM(OSETIDXn, (sfpidx), (a), an, v);
					if(IS_Tnumbox(reqt)) {
						KNH_ASM(UNBOX, sfpidx, sfpidx);
					}
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
			}
			else if(IS_Tfloat(type)) {
				KNH_ASM(fMOVx, (sfpidx), bx);
			}
			else if(IS_Tbool(type)) {
				KNH_ASM(bMOVx, (sfpidx), bx);
			}
			else {
				KNH_ASM(OMOVx, (sfpidx), bx);
			}
			KNH_ASM_BOX2(ctx, reqt, type, sfpidx);
			return;
		}
		deltaidx = knh_Method_indexOfSetterField(mtd);
		if(deltaidx != -1) {
			int b = TERMs_put(ctx, stmt, 1, TYPE_Object, local + 1);
			knh_type_t reqt2 = knh_Method_ptype(ctx, mtd, cid, 0);
			knh_sfx_t bx = {b, deltaidx};
			int c = TERMs_put(ctx, stmt, 2, reqt2, local + 2);
			if(IS_Tint(reqt2)) {
				KNH_ASM(XIMOV, bx, (c));
			}
			else if(IS_Tfloat(reqt2)) {
				KNH_ASM(XFMOV, bx, (c));
			}
			else if(IS_Tbool(reqt2)) {
				KNH_ASM(XBMOV, bx, (c));
			}
			else {
				KNH_ASM_BOX2(ctx, reqt2, SP(stmt)->type, c);
				KNH_ASM(XMOV, bx, (c));
			}
			if(reqt != TYPE_void) {
				if(IS_Tunbox(reqt2)) {
					KNH_ASM(NMOV, sfpidx, c);
					KNH_ASM_BOX2(ctx, reqt, reqt2, sfpidx);
				}
				else {
					KNH_ASM(OMOV, sfpidx, c);
					KNH_ASM_UNBOX2(ctx, reqt2, sfpidx);
				}
			}
			return;
		}
	}
	L_USECALL:;
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
	knh_ParamArray_t *pa = ClassTBL(cid).cparam;
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
					KNH_ASM_BOX2(ctx, TYPE_Object, srct, local);
					KNH_ASM(SCAST, local, local, trl);
				}
			}
			else {
				KNH_ASM_BOX2(ctx, TYPE_Object, srct, local);
				if(IS_Tunbox(srct) && IS_Tnumbox(reqt)) {

				}
				else {
					KNH_ASM(TCAST, local, local, trl);
				}
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
//	knh_BasicBlock_t* label = new_BasicBlockLABEL(ctx);
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
	int i, size = DP(stmt)->size;
	knh_BasicBlock_t*  lbTRUE = new_BasicBlockLABEL(ctx);
	knh_BasicBlock_t*  lbFALSE = new_BasicBlockLABEL(ctx);
	for(i = 0; i < size; i++) {
		TERMs_JMPIF(ctx, stmt, i, 1/*TRUE*/, lbTRUE, local);
	}
	KNH_ASM(NSET, sfpidx, knh_Object_data(KNH_FALSE));
	KNH_ASM_JMP(ctx, lbFALSE);
	KNH_LABEL(ctx, lbTRUE);
	KNH_ASM(NSET, sfpidx, knh_Object_data(KNH_TRUE));
	KNH_LABEL(ctx, lbFALSE); // false
	//KNH_ASM_MOVL(ctx, TYPE_Boolean, sfpidx, SP(stmt)->type, local);
}

static void knh_StmtAND_asm(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	int local = ASML(sfpidx);
	int i, size = DP(stmt)->size;
	knh_BasicBlock_t*  lbTRUE = new_BasicBlockLABEL(ctx);
	knh_BasicBlock_t*  lbFALSE = new_BasicBlockLABEL(ctx);
	for(i = 0; i < size; i++) {
		TERMs_JMPIF(ctx, stmt, i, 0/*FALSE*/, lbFALSE, local);
	}
	KNH_ASM(NSET, sfpidx, knh_Object_data(KNH_TRUE));
	KNH_ASM_JMP(ctx, lbTRUE);
	KNH_LABEL(ctx, lbFALSE); // false
	KNH_ASM(NSET, sfpidx, knh_Object_data(KNH_FALSE));
	KNH_LABEL(ctx, lbTRUE);   // TRUE
	//KNH_ASM_MOVL(ctx, TYPE_Boolean, sfpidx, SP(stmt)->type, local);
}

static void knh_StmtTRI_asm(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	int local = ASML(sfpidx);
	knh_BasicBlock_t*  lbelse = new_BasicBlockLABEL(ctx);
	knh_BasicBlock_t*  lbend  = new_BasicBlockLABEL(ctx);
	//TERMs_asm(ctx, stmt, 0, TYPE_Boolean, local);
	TERMs_JMPIF(ctx, stmt, 0, 0/*FALSE*/, lbelse, local);
	TERMs_asm(ctx, stmt, 1, reqt, local);
	KNH_ASM_JMP(ctx, lbend);
	/* else */
	KNH_LABEL(ctx, lbelse);
	TERMs_asm(ctx, stmt, 2, reqt, local);
	KNH_LABEL(ctx, lbend);
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

static knh_Method_t* knh_NameSpace_getFormatterNULL(Ctx *ctx, knh_NameSpace_t *ns, knh_class_t cid, knh_methodn_t mn)
{
	L_TAIL:;
	if(DP(ns)->formattersNULL != NULL) {
		knh_Array_t *a = DP(ns)->formattersNULL;
		size_t i;
		for(i = 0; knh_Array_size(a); i++) {
			knh_Method_t *mtd = a->methods[i];
			if(DP(mtd)->cid == cid && DP(mtd)->mn == mn) {
				return mtd;
			}
		}
	}
	if(DP(ns)->parentNULL != NULL) {
		ns = DP(ns)->parentNULL;
		goto L_TAIL;
	}
	return NULL;
}

static void knh_NameSpace_addFormatter(Ctx *ctx, knh_NameSpace_t *ns, knh_Method_t *mtd)
{
	if(DP(ns)->formattersNULL != NULL) {
		KNH_INITv(DP(ns)->formattersNULL, new_Array0(ctx, 0));
	}
	knh_Array_add(ctx, DP(ns)->formattersNULL, mtd);
}

static METHOD knh_Fmethod_dynamic(Ctx *ctx, knh_sfp_t *sfp, long rix)
{

}

static knh_Method_t* knh_Gamma_getFormatter(Ctx *ctx, knh_class_t cid, knh_methodn_t mn0)
{
	knh_methodn_t mn = mn0;
	knh_Method_t *mtd = knh_NameSpace_getFormatterNULL(ctx, DP(ctx->gma)->ns, cid, mn);
	if(mtd == NULL) {
		mtd = knh_getFormatterNULL(ctx, cid, mn);
	}
	if(mtd != NULL && mn == MN__data) {
		mn = MN__k;
		mtd = knh_getFormatterNULL(ctx, cid, mn);
	}
	if(mtd != NULL && mn == MN__k) {
		mn = MN__s;
		mtd = knh_getFormatterNULL(ctx, cid, mn);
	}
	if(mtd == NULL) {
		knh_Gamma_perror(ctx, KERR_DWARN, _("undefined formatter: %M for %C"), mn0, cid);
		mtd = new_Method(ctx, 0, cid, mn0, knh_Fmethod_dynamic);
		KNH_SETv(ctx, DP(mtd)->mp, KNH_TNULL(ParamArray));
		knh_NameSpace_addFormatter(ctx, DP(ctx->gma)->ns, mtd);
	}
	return mtd;
}

static void knh_StmtW_asm(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx);

static void knh_StmtW_asmIN(Ctx *ctx, knh_Stmt_t *stmt, size_t thisidx)
{
	size_t i;
	for(i = DP(stmt)->wstart; i < DP(stmt)->size; i++) {
		knh_Token_t *tk = DP(stmt)->tokens[i];
		knh_Method_t *mtd = NULL;
		if(TT_(tk) == TT_MT) {
			knh_Term_t *tm = (knh_Term_t*)DP(tk)->data;
			knh_type_t type = tm->type;
			DBG_ASSERT(IS_Stmt(tm) || IS_Token(tm));
			mtd = knh_Gamma_getFormatter(ctx, CLASS_type(type), DP(tk)->mn);
			KNH_SETv(ctx, DP(stmt)->terms[i], tm);
			TERMs_asm(ctx, stmt, i, type, thisidx + 1);
			KNH_ASM(SCALL, thisidx-K_CALLDELTA, ESP_((thisidx-K_CALLDELTA), 1), mtd);
		}
//		else if(TT_(tk) == TT_CONST) {
//			KNH_ASM_SMOV(ctx, tk->type, thisidx + 1, tk);
//			if(IS_bString(DP(tk)->data)) {
//				mtd = knh_getMethodNULL(ctx, CLASS_OutputStream, MN_opSEND);
//				DBG_P(mtd != NULL);
//			}
//			else {
//				mtd = knh_NameSpace_getFormatterNULL(ctx, knh_Object_cid(DP(tk)->data), MN__s);
//			}
//			KNH_ASM(SCALL, thisidx-K_CALLDELTA, ESP_((thisidx-K_CALLDELTA), 1), mtd);
//		}
		else if(TT_(tk) == STT_W) {
			knh_Stmt_t *stmtIN = (knh_Stmt_t*)tk;
			DBG_ASSERT(stmtIN->type == TYPE_String);
			knh_Token_toTYPED(ctx, DP(stmtIN)->tokens[1], TT_LOCAL, TYPE_OutputStream, thisidx);
			if(DP(stmtIN)->wstart == 2) {
				knh_StmtW_asmIN(ctx, stmtIN, thisidx);
			}
			else {
				DP(stmtIN)->espidx = thisidx + 2;
				knh_StmtW_asm(ctx, stmtIN, TYPE_String, thisidx);
			}
		}
		else {
			knh_class_t cid = CLASS_type(tk->type);
			TERMs_asm(ctx, stmt, i, cid, thisidx + 1);
			if(IS_Tstr(cid)) {
				mtd = knh_getMethodNULL(ctx, CLASS_OutputStream, MN_opSEND);
				DBG_ASSERT(mtd != NULL);
			}
			else {
				mtd = knh_Gamma_getFormatter(ctx, cid, MN__s);
			}
			KNH_ASM(SCALL, thisidx-K_CALLDELTA, ESP_((thisidx-K_CALLDELTA), 1), mtd);
		}
	}
}

static void knh_StmtW_asm(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	int isCWB = 0;
	int local = ASML(sfpidx);
	size_t i, thisidx = local + K_CALLDELTA + DP(stmt)->wstart - 2;
	DBG_P("espidx=%d", DP(stmt)->espidx);
	for(i = 2; i < DP(stmt)->wstart; i++) {
		TERMs_asm(ctx, stmt, i, TYPE_Object, local + i - 1);
	}
	DBG_P("@@@@@@@ local=%d, wstart=%d, thisidx=%d", local, DP(stmt)->wstart, thisidx);
	if(TT_(DP(stmt)->terms[1]) == TT_ASIS) {
		KNH_ASM(TR, thisidx, thisidx, CLASS_OutputStream, _CWB);
		KNH_SETv(ctx, DP(stmt)->terms[1], knh_Token_toTYPED(ctx, DP(stmt)->tokens[1], TT_LOCAL, TYPE_OutputStream, thisidx));
		isCWB = 1;
	}
	else {
		TERMs_asm(ctx, stmt, 1, TYPE_OutputStream, thisidx);
	}
	knh_StmtW_asmIN(ctx, stmt, thisidx);
	if(isCWB) {
		KNH_ASM(TR, sfpidx, thisidx, CLASS_String, _TOSTR);
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
	CASE_ASM(W, reqt, sfpidx);
	default:
		DBG_P("unknown stt=%s", TT_tochar(STT_(stmt)));
	}
	KNH_ASM_BOX2(ctx, reqt, SP(stmt)->type, sfpidx);
	if(sfpidx == K_RTNIDX) {
		KNH_ASM_RET(ctx);
	}
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
		if(TT_(tk) != TT_LOCAL) {
			knh_Token_toTYPED(ctx, tk, TT_LOCAL, reqt, local);
		}
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
void knh_Gamma_pushLABEL(Ctx *ctx, knh_Stmt_t *stmt, knh_BasicBlock_t *lbC, knh_BasicBlock_t *lbB)
{
	knh_Object_t *tkL = NULL;
	if(IS_Map(DP(stmt)->metaDictCaseMap)) {
		tkL = knh_DictCaseMap_getNULL(ctx, DP(stmt)->metaDictCaseMap, S_tobytes(TS_ATlabel));
	}
	if(tkL == NULL) {
		tkL = KNH_NULVAL(CLASS_Any);
	}
	knh_Array_add_(ctx, DP(ctx->gma)->lstacks, tkL);
	knh_Array_add_(ctx, DP(ctx->gma)->lstacks, UPCAST(lbC));
	knh_Array_add_(ctx, DP(ctx->gma)->lstacks, UPCAST(lbB));
}

static void knh_Gamma_popLABEL(Ctx *ctx)
{
	knh_Array_t *a = DP(ctx->gma)->lstacks;
	DBG_ASSERT(knh_Array_size(a) - 3 >= 0);
	knh_Array_clear(ctx, a, knh_Array_size(a) - 3);
}

/* ======================================================================== */
/* [IF, WHILE, DO, FOR, FOREACH]  */

static knh_Token_t *TERMs_it(knh_Stmt_t *stmt, size_t n)
{
	DBG_ASSERT(n < DP(stmt)->size);
	knh_Token_t *tkIT = DP(stmt)->tokens[n];
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
	knh_BasicBlock_t*  lbELSE = new_BasicBlockLABEL(ctx);
	knh_BasicBlock_t*  lbEND  = new_BasicBlockLABEL(ctx);
	/* if */
	lbELSE = TERMs_JMPIF(ctx, stmt, 0, 0/*FALSE*/, lbELSE, DP(ctx->gma)->espidx);
	/* then */
	TERMs_asmBLOCK(ctx, stmt, 1, reqt);
	KNH_ASM_JMP(ctx, lbEND);
	/* else */
	KNH_LABEL(ctx, lbELSE);
	TERMs_asmBLOCK(ctx, stmt, 2, reqt);
	/* endif */
	KNH_LABEL(ctx, lbEND);
}

static void knh_StmtSWITCH_asm(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	knh_Stmt_t *stmtCASE;
	knh_Token_t *tkIT = TERMs_it(stmt, 2);
	knh_BasicBlock_t* lbCONTINUE = new_BasicBlockLABEL(ctx);
	knh_BasicBlock_t* lbBREAK = new_BasicBlockLABEL(ctx);
	knh_BasicBlock_t *lbNEXT = NULL;
	knh_Gamma_pushLABEL(ctx, stmt, lbCONTINUE, lbBREAK);
	KNH_LABEL(ctx, lbCONTINUE);
	//switch(it)
	TERMs_asm(ctx, stmt, 0, SP(tkIT)->type, (DP(tkIT)->index));
	stmtCASE = DP(stmt)->stmts[1];
	while(stmtCASE != NULL) {
		// case 'a' :
		if(STT_(stmtCASE) == STT_CASE && !TERMs_isASIS(stmtCASE, 0)) {
			knh_BasicBlock_t *lbEND = new_BasicBlockLABEL(ctx);
			DP(ctx->gma)->espidx = DP(stmtCASE)->espidx;
			TERMs_JMPIF(ctx, stmtCASE, 0, 0/*FALSE*/, lbEND, DP(ctx->gma)->espidx);
			if(lbNEXT != NULL) {
				KNH_LABEL(ctx, lbNEXT); lbNEXT = NULL;
			}
			TERMs_asmBLOCK(ctx, stmtCASE, 1, reqt);
			lbNEXT = new_BasicBlockLABEL(ctx);
			KNH_ASM_JMP(ctx, lbNEXT);
			KNH_LABEL(ctx, lbEND);
		}
		stmtCASE = DP(stmtCASE)->nextNULL;
	}
	if(lbNEXT != NULL) {
		KNH_LABEL(ctx, lbNEXT); lbNEXT = NULL;
	}
	stmtCASE = DP(stmt)->stmts[1];
	while(stmtCASE !=NULL) {
		if(STT_(stmtCASE) == STT_CASE && TERMs_isASIS(stmtCASE, 0)) {
			TERMs_asmBLOCK(ctx, stmtCASE, 1, reqt);
		}
		stmtCASE = DP(stmtCASE)->nextNULL;
	}
	KNH_LABEL(ctx, lbBREAK);
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
		knh_BasicBlock_t *lbBLOCK = NULL;
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
					lbBLOCK = knh_GammaLabel(ctx,  i + delta);
					goto L_JUMP;
				}
			}
			knh_Gamma_perror(ctx, KERR_EWARN, _("undefined label: %L"), tkL);
		}
		lbBLOCK = knh_GammaLabel(ctx,  s - 3 + delta);
		L_JUMP:;
		KNH_ASM_JMP(ctx, lbBLOCK);
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
	knh_BasicBlock_t* lbCONTINUE = new_BasicBlockLABEL(ctx);
	knh_BasicBlock_t* lbBREAK = new_BasicBlockLABEL(ctx);
	knh_Gamma_pushLABEL(ctx, stmt, lbCONTINUE, lbBREAK);
	KNH_LABEL(ctx, lbCONTINUE);
	if(!TERMs_isTRUE(stmt, 0)) {
		TERMs_JMPIF(ctx, stmt, 0, 0/*FALSE*/, lbBREAK, DP(ctx->gma)->espidx);
	}
	TERMs_asmBLOCK(ctx, stmt, 1, TYPE_void);
	KNH_ASM_JMP(ctx, lbCONTINUE);
	KNH_LABEL(ctx, lbBREAK);
	knh_Gamma_popLABEL(ctx);
}

static void knh_StmtDO_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	knh_BasicBlock_t* lbCONTINUE = new_BasicBlockLABEL(ctx);
	knh_BasicBlock_t* lbBREAK = new_BasicBlockLABEL(ctx);
	knh_Gamma_pushLABEL(ctx, stmt, lbCONTINUE, lbBREAK);
	KNH_LABEL(ctx, lbCONTINUE);
	TERMs_asmBLOCK(ctx, stmt, 0, TYPE_void);
	TERMs_JMPIF(ctx, stmt, 1, 0/*FALSE*/, lbBREAK, DP(ctx->gma)->espidx);
	KNH_ASM_JMP(ctx, lbCONTINUE);
	KNH_LABEL(ctx, lbBREAK);
	knh_Gamma_popLABEL(ctx);
}

static void knh_StmtFOR_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	knh_BasicBlock_t* lbCONTINUE = new_BasicBlockLABEL(ctx);
	knh_BasicBlock_t* lbBREAK = new_BasicBlockLABEL(ctx);
	knh_BasicBlock_t* lbREDO = new_BasicBlockLABEL(ctx);
	knh_Gamma_pushLABEL(ctx, stmt, lbCONTINUE, lbBREAK);
	/* i = 1 part */
	TERMs_asmBLOCK(ctx, stmt, 0, TYPE_void);
	KNH_ASM_JMP(ctx, lbREDO);
	/* i++ part */
	KNH_LABEL(ctx, lbCONTINUE); /* CONTINUE */
	TERMs_asmBLOCK(ctx, stmt, 2, TYPE_void);
	/* i < 10 part */
	KNH_LABEL(ctx, lbREDO);
	if(!TERMs_isTRUE(stmt, 1)) {
		TERMs_JMPIF(ctx, stmt, 1, 0/*FALSE*/, lbBREAK, DP(ctx->gma)->espidx);
	}
	TERMs_asmBLOCK(ctx, stmt, 3, TYPE_void);
	KNH_ASM_JMP(ctx, lbCONTINUE);
	KNH_LABEL(ctx, lbBREAK);
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
	knh_BasicBlock_t* lbC = new_BasicBlockLABEL(ctx);
	knh_BasicBlock_t* lbB = new_BasicBlockLABEL(ctx);
	knh_Stmt_t *stmtDECL = DP(stmt)->stmts[0];
	knh_Token_t *tkIT = DP(stmt)->tokens[3];
	int itridx = DP(tkIT)->index;
	knh_Gamma_pushLABEL(ctx, stmt, lbC, lbB);
	if(isSINGLEFOREACH(stmtDECL)) {
		knh_Token_t *tkN = DP(stmtDECL)->tokens[1];
		TERMs_asm(ctx, stmt, 1, TYPE_Iterator, itridx);
		KNH_LABEL(ctx, lbC);
		DBG_P("variable=%d, iterator=%d", (DP(tkN)->index), itridx);
		KNH_ASM_BRANCH(NEXT, lbB, DP(tkN)->index, itridx);
	}
	else {

	}
	TERMs_asmBLOCK(ctx, stmt, 2, TYPE_void);
	KNH_ASM_JMP(ctx, lbC);
	/* end */
	KNH_LABEL(ctx, lbB);
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
	knh_BasicBlock_t*  lbCATCH   = new_BasicBlockLABEL(ctx);
	knh_BasicBlock_t*  lbFINALLY = new_BasicBlockLABEL(ctx);
	knh_Token_t *tkIT = TERMs_it(stmt, 3/*HDR*/);
	knh_Gamma_setFinallyStmt(ctx, DP(stmt)->stmts[2/*finally*/]);

	/* try { */
	KNH_ASM_BRANCH(TRY, lbCATCH, (DP(tkIT)->index));
	TERMs_asmBLOCK(ctx, stmt, 0/*try*/, TYPE_void);
	//KNH_ASM(TRYEND, (DP(tkIT)->index));
	KNH_ASM_JMP(ctx, lbFINALLY);
	knh_Gamma_setFinallyStmt(ctx, (knh_Stmt_t*)KNH_NULL); // InTry

	/* catch */
	KNH_LABEL(ctx, lbCATCH);
	knh_Stmt_t *stmtCATCH = DP(stmt)->stmts[1/*catch*/];
	DBG_ASSERT(IS_Stmt(stmtCATCH));
	while(stmtCATCH != NULL) {
		if(SP(stmtCATCH)->stt == STT_CATCH) {
			knh_String_t *emsg = DP(DP(stmtCATCH)->tokens[0])->text;
			knh_Token_t *tkn = DP(stmtCATCH)->tokens[1];
			DBG_ASSERT(IS_String(emsg));
			DBG_ASSERT(TT_(tkn) == TT_LOCAL);
			lbCATCH = new_BasicBlockLABEL(ctx);
			KNH_ASM_BRANCH(CATCH, lbCATCH, (DP(tkn)->index), emsg);
			TERMs_asmBLOCK(ctx, stmtCATCH, 2, TYPE_void);
			KNH_ASM_JMP(ctx, lbFINALLY);  /* GOTO FINALLY */
			KNH_LABEL(ctx, lbCATCH); /* _CATCH_NEXT_ */
		}
		stmtCATCH = DP(stmtCATCH)->nextNULL;
	}
	KNH_LABEL(ctx, lbFINALLY); /* FINALLY */
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
			KNH_ASM_BOX2(ctx, knh_type_tocid(ctx, p->type, DP(ctx->gma)->this_cid), rtype, K_RTNIDX);
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
				KNH_ASM_BOX2(ctx, knh_type_tocid(ctx, p->type, DP(ctx->gma)->this_cid), rtype, (K_RTNIDX+i));
			}
			else {
				KNH_ASM(OMOV, (K_RTNIDX+i), (espidx+i));
				if(IS_Tnumbox(rtype)) {
					KNH_ASM(UNBOX, (K_RTNIDX+i), (K_RTNIDX+i));
				}
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
		knh_printf(ctx, cwb->w, "Script!!: konoha -c %s at %d for debugging", FILENAME__(SP(stmt)->uri), SP(stmt)->line);
		KNH_SETv(ctx, DP(stmt)->errMsg, knh_cwb_newString(ctx, cwb));
		if(DP(stmt)->nextNULL != NULL) {
			KNH_FINALv(ctx, DP(stmt)->nextNULL);
			DP(stmt)->nextNULL = NULL;
		}
	}
	KNH_ASM(OSET, espidx, UPCAST(DP(stmt)->errMsg));
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

static void KNH_ASM_SKIP(Ctx *ctx, knh_BasicBlock_t* lbskip)
{
	if(!knh_Method_isDebug(DP(ctx->gma)->mtd)) {
		KNH_ASM_BRANCH(DYJMP, lbskip, 0, klr_isskip);
	}
}

static knh_flag_t knh_StmtPRINT_flag(Ctx *ctx, knh_Stmt_t *o)
{
	knh_flag_t flag = 0;
	if(IS_Map(DP(o)->metaDictCaseMap)) {
		Object *v = knh_DictCaseMap_getNULL(ctx,  DP(o)->metaDictCaseMap, STEXT("time"));
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
	knh_BasicBlock_t*  lbskip = new_BasicBlockLABEL(ctx);
	size_t i;
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
			knh_Method_t *mtd = knh_Gamma_getFormatter(ctx, TERMs_getcid(stmt, i), MN__s);
			TERMs_asm(ctx, stmt, i, TERMs_gettype(stmt, i), espidx);
			KNH_ASM(P, _PRINT, flag | mask, msg, mtd, espidx); flag=0;
		}
	}
	KNH_LABEL(ctx, lbskip);
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
	knh_BasicBlock_t* lbskip = new_BasicBlockLABEL(ctx);
	if(!isRelease) {
		KNH_ASM_BRANCH(DYJMP, lbskip, 0, klr_isskip);
	}
	/* if */
	TERMs_JMPIF(ctx, stmt, 0, 1, lbskip, DP(ctx->gma)->espidx);
	/*then*/
	TERMs_asmBLOCK(ctx, stmt, 1, TYPE_void);
	KNH_ASM(OSET, espidx, UPCAST(TS_AssertionException));
	KNH_ASM(TR, espidx, espidx, CLASS_Exception, _ERR);
	KNH_ASM(THROW, start);
	KNH_LABEL(ctx, lbskip);
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
		DBG_P("ASM %s %p etype=%s,%s, espidx=%d", TT_tochar(STT_(stmt)), stmt, TYPE__(etype), TYPE__(stmt->type), DP(ctx->gma)->espidx);
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
	DBG_P("****** %s TYPE=%s, NEXT=%p", TT_tochar(STT_(stmtB)), TYPE__(stmtB->type), DP(stmtB)->nextNULL);
	if(STT_(stmtB) == STT_ERR) return ;
	DBG_ASSERT(knh_Array_size(DP(ctx->gma)->insts) == 0);
	{
		knh_BasicBlock_t* lbINIT = new_BasicBlockLABEL(ctx);
		knh_BasicBlock_t* lbBEGIN = new_BasicBlockLABEL(ctx);
		knh_BasicBlock_t* lbEND = new_BasicBlockLABEL(ctx);
		size_t i, xi;
		knh_Method_t *mtd = DP(ctx->gma)->mtd;
		DBG_ASSERT(IS_Method(mtd));
		knh_Gamma_pushLABEL(ctx, stmtB, lbBEGIN, lbEND);
		SP(ctx->gma)->line = SP(stmtB)->line;
		DP(ctx->gma)->bbNC = lbINIT;
		KNH_ASM(THCODE);
		if(knh_Method_isStatic(mtd) && knh_Gamma_hasFIELD(ctx->gma)) {
			KNH_ASM(TR, 0, 0, DP(ctx->gma)->this_cid, _NULVAL);
		}
		KNH_LABEL(ctx, lbBEGIN);
		for(i = 1;i < (size_t)(knh_Method_psize(mtd) + 1); i++) {
			xi = i + DP(ctx->gma)->goffset;
			Object *value = DP(ctx->gma)->gf[xi].value;
			DBG_(
			knh_type_t ztype = knh_ParamArray_getptype(DP(mtd)->mp, i - 1);
			knh_type_t ptype = DP(ctx->gma)->gf[xi].type;
			DBG_P("PARAM TYPE %s (%s) i=%ld, xi=%ld %s value=%p", TYPE__(ztype), TYPE__(ptype), i, xi, FN__(DP(ctx->gma)->gf[xi].fn), value);
			DBG_ASSERT(CLASS_type(ztype) == CLASS_type(ptype));
			);
			if(value == NULL) {
//				KNH_ASM(NULTR, Ftr_DEF, (i), CLASS_type(ptype));
			}
			else {
//				if(IS_NOTNULL(value)) {
//					knh_BasicBlock_t*  lb = new_BasicBlockLABEL(ctx);
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
		KNH_LABEL(ctx, lbEND);
		KNH_ASM(RET);
		knh_Gamma_popLABEL(ctx);
		DBG_ASSERT(knh_Array_size(DP(ctx->gma)->lstacks) == 0);
		knh_Gamma_compile(ctx, lbINIT, lbEND);
	}
	DP(ctx->gma)->this_cid = prev_cid;
}

/* ------------------------------------------------------------------------ */
/* [loadSystem] */

//static
//KLRAPI(void) knh_Fprobe_printStackTrace(Ctx *ctx, knh_sfp_t *sfp, int n, knh_opline_t **pc)
//{
//	knh_Exception_t *e = DP(sfp[n].hdr)->caught;
//	DBG_ASSERT(IS_ExceptionHandler(sfp[-3].hdr));
//	DBG_ASSERT(IS_Exception(e));
//	knh_write_Object(ctx, KNH_STDERR, MN__dump, UPCAST(e), KNH_NULL);
//}

static knh_opline_t* knh_code_findOPCODE(Ctx *ctx, knh_opline_t *op, knh_opcode_t opcode)
{
	while(1) {
		if(op->opcode == opcode) return op;
		if(op->opcode == OPCODE_RET) break;
		op++;
	}
	DBG_ABORT();
	return NULL;
}

void knh_loadSystemKLRCode(Ctx *ctx)
{
	BEGIN_LOCAL(ctx, lsfp, 4);
	knh_BasicBlock_t* ia = new_(BasicBlock);
	knh_BasicBlock_t* ib = new_(BasicBlock);
	knh_BasicBlock_t* ic = new_(BasicBlock);
	knh_BasicBlock_t* id = new_(BasicBlock);
	KNH_SETv(ctx, lsfp[0].o, ia);
	KNH_SETv(ctx, lsfp[1].o, ib);
	KNH_SETv(ctx, lsfp[2].o, ic);
	KNH_SETv(ctx, lsfp[3].o, id);
	knh_BasicBlock_add(ctx, ia, THCODE);
	knh_BasicBlock_add(ctx, ia, TRY, NULL/*lb*/, (-3));
	ia->nextNC = ib;
	ia->jumpNC = ic;
	knh_BasicBlock_add(ctx, ib, ENTER);
	knh_BasicBlock_add(ctx, ib, EXIT);
	ib->nextNC = ic;
	knh_BasicBlock_add(ctx, ic, EXIT);
	knh_BasicBlock_add(ctx, ic, FUNCCALL);
	knh_BasicBlock_add(ctx, ic, VEXEC);
	knh_BasicBlock_add(ctx, ic, EXIT);
	ic->nextNC = id;
	knh_BasicBlock_add(ctx, id, RET);  // NEED TERMINATION
	{
		knh_KLRCode_t *kcode = knh_BasicBlock_link(ctx, ia, id);
		knh_opline_t *pc = knh_VirtualMachine_run(ctx, ctx->esp, SP(kcode)->code);
		knh_setClassDefaultValue(ctx, CLASS_KLRCode, kcode, NULL);
		((knh_share_t*)ctx->share)->PC_LAUNCH = knh_code_findOPCODE(ctx, pc, OPCODE_TRY);
		((knh_share_t*)ctx->share)->PC_FUNCCALL = knh_code_findOPCODE(ctx, pc, OPCODE_FUNCCALL);
		((knh_share_t*)ctx->share)->PC_VEXEC = knh_code_findOPCODE(ctx, pc, OPCODE_VEXEC);
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
	_FUNC(_PRINT, "PRINT"), _FUNC(_ifBOX, "BOX"), _FUNC(_NEW, "NEW"),
	_FUNC(_NULVAL, "NULL"), _FUNC(_SYS, "SYS"), _FUNC(_CWB, "CWB"), _FUNC(_TOSTR, "TOSTR"),
	_FUNC(klr_setMethod, "setMethod"), _FUNC(klr_lookupMethod, "lookupMethod"),
	_FUNC(_PROP, "PROP"), _FUNC(_bBOX, "bBOX"), _FUNC(_OBOX, "OBOX"), _FUNC(_VARGS, "VARGS"),
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
