/****************************************************************************
 * KONOHA COPYRIGHT, LICENSE NOTICE, AND DISCRIMER
 *
 * Copyright (c) 2005-2009, Kimio Kuramitsu <kimio at ynu.ac.jp>
 *           (c) 2008-      Konoha Software Foundation
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

#include"commons.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif


/* ======================================================================== */
/* @data */

static int knh_Gamma_inTry(Ctx *ctx);

#define ASML(idx) (idx < DP(ctx->kc)->esp) ? (DP(ctx->kc)->esp) : idx

#define MOVL(ctx, local, idx) {\
		if(idx < DP(ctx->kc)->esp) { \
			KNH_ASM(MOVa, sfi_(idx), sfi_(local));\
		}\
	}\

#define nMOVL(ctx, local, idx) {\
		if(idx < DP(ctx->kc)->esp) { \
			KNH_ASM(MOVn, sfi_(idx), sfi_(local));\
		}\
	}\

static knh_KLRInst_t *new_KLRInstLABEL(Ctx *ctx);

#define ASM_ASSERT(ctx, c)   DBG2_ASSERT(c)

#define KNH_ASM_PANIC(ctx, fmt, ...) {\
		knh_Gamma_setCancelled(ctx->kc, 1);\
		fprintf(stderr, "PANIC[%s:%d/%s]: ", knh_safefile(__FILE__), __LINE__, __FUNCTION__); \
		fprintf(stderr, fmt, ## __VA_ARGS__); \
		fprintf(stderr, "\n"); \
	}\

static
void TERMs_asm(Ctx *ctx, knh_Stmt_t *stmt, size_t n, knh_type_t reqt, int sfpidx);

/* ======================================================================== */
/* [namespace] */

knh_NameSpace_t *knh_getGammaNameSpace(Ctx *ctx)
{
	return DP(ctx->kc)->ns;
}

/* ------------------------------------------------------------------------ */

knh_NameSpace_t *knh_setGammaNameSpace(Ctx *ctx, knh_String_t *nsname)
{
	knh_Gamma_t *kc = ctx->kc;
	if(IS_NULL(nsname) || knh_String_equals(nsname, STEXT("main"))) {
		KNH_SETv(ctx, DP(kc)->ns, ctx->share->mainns);
	}
	else {
		KNH_SETv(ctx, DP(kc)->ns, knh_getNameSpace(ctx, __tobytes(nsname)));
	}
	return DP(kc)->ns;
}

/* ------------------------------------------------------------------------ */

knh_NameSpace_t *knh_switchGammaNameSpace(Ctx *ctx, knh_NameSpace_t *newns)
{
	knh_Gamma_t *kc = ctx->kc;
	knh_NameSpace_t *oldns = DP(kc)->ns;
	KNH_SETv(ctx, DP(kc)->ns, newns);
	return oldns;
}

/* ------------------------------------------------------------------------ */

knh_Script_t *knh_getGammaScript(Ctx *ctx)
{
	return knh_NameSpace_getScript(ctx, DP(ctx->kc)->ns);
}

/* ------------------------------------------------------------------------ */

void knh_Gamma_clear(Ctx *ctx, size_t offset, int isAll)
{
	knh_Gamma_t *kc = ctx->kc;
	size_t i;
	for(i = offset; i < K_GAMMASIZE; i++) {
		if(DP(kc)->gamma[i].fn != FIELDN_NONAME) {
			if(isAll || !KNH_FLAG_IS(DP(kc)->gamma[i].flag, FLAG_GAMMA_FuncScope)) {
				DP(kc)->gamma[i].flag  = 0;
				DP(kc)->gamma[i].type  = TYPE_void;
				DP(kc)->gamma[i].fn    = FIELDN_NONAME;
				if(DP(kc)->gamma[i].value != NULL) {
					KNH_FINALv(ctx, DP(kc)->gamma[i].value);
					DP(kc)->gamma[i].value = NULL;
				}
			}
		}
	}
	DP(kc)->esp = -1;
}

/* ------------------------------------------------------------------------ */

void knh_Gamma_prepare(Ctx *ctx, knh_Method_t *mtd, knh_Stmt_t *stmt)
{
	knh_Gamma_t *kc = ctx->kc;
	DP(kc)->flag  = 0;
	DP(kc)->pflag = 0;
	KNH_SETv(ctx, DP(kc)->mtd, mtd);
	DP(kc)->this_cid = DP(mtd)->cid;
	DP(kc)->rtype = knh_Method_rztype(DP(kc)->mtd);
	if(DP(kc)->rtype != TYPE_var) {
		DP(kc)->rtype = knh_pmztype_totype(ctx, DP(kc)->rtype, DP(kc)->this_cid);
	}
	knh_Gamma_clear(ctx, 0, 1/*isAll*/);
	DP(kc)->esp = -1;
	DP(kc)->globalidx = -1;
	DP(kc)->testidx = -1;
	knh_Array_clear(ctx, DP(kc)->lstacks);
	knh_Array_clear(ctx, DP(kc)->insts);
	knh_Array_clear(ctx, DP(kc)->decls);
	knh_Array_clear(ctx, DP(kc)->untypes);
	SP(kc)->uri  = SP(stmt)->uri;
	SP(kc)->line = 0;
}

/* ------------------------------------------------------------------------ */

void knh_Gamma_initThis(Ctx *ctx, knh_class_t cid)
{
	DBG2_ASSERT_cid(cid);
	knh_Gamma_t *kc = ctx->kc;
	int goffset = DP(kc)->goffset;
	ASM_ASSERT(ctx, goffset < K_GAMMASIZE);
	DP(kc)->this_cid = cid;
	DP(kc)->gamma[goffset].flag = 0;
	DP(kc)->gamma[goffset].type = NNTYPE_cid(cid);
	DP(kc)->gamma[goffset].fn   = FIELDN_this;
	DBG2_ASSERT(DP(kc)->gamma[goffset].value == NULL);
	DP(kc)->psize = 0;
}


/* ------------------------------------------------------------------------ */

static void knh_Gamma_gc(Ctx *ctx)
{
	knh_Gamma_t *kc = ctx->kc;
	KNH_SETv(ctx, DP(kc)->mtd, KNH_NULL);
	knh_Gamma_clear(ctx, 0, 1/*isAll*/);
	knh_Array_clear(ctx, DP(kc)->lstacks);
	knh_Array_clear(ctx, DP(kc)->insts);
	knh_Array_clear(ctx, DP(kc)->decls);
	knh_Array_clear(ctx, DP(kc)->untypes);
}

/* ------------------------------------------------------------------------ */

static void knh_Gamma_finish(Ctx *ctx)
{
	knh_Gamma_t *kc = ctx->kc;
	knh_Method_t *mtd = DP(kc)->mtd;
	DBG2_ASSERT(IS_Method(mtd));

	if(knh_Gamma_isCancelled(kc)) {
		knh_Gamma_gc(ctx);
		return;
	}

	knh_KLRCode_t *vmc = knh_InstList_newKLRCode(ctx, DP(kc)->insts);
	knh_Method_setKLRCode(ctx, mtd, vmc);
	if(knh_Gamma_isCancelled(kc)) {
		if(DP(mtd)->mn != METHODN_LAMBDA) {
			knh_Gamma_perror(ctx, KERR_DWARN, "abstract? %C.%M", DP(mtd)->cid, DP(mtd)->mn);
		}
		knh_Method_toAbstract(ctx, mtd);
	}
	else {
		knh_sfp_t *lsfp = KNH_LOCAL(ctx);
		KNH_SETv(ctx, lsfp[0].o, mtd);
		//lsfp[0].pc = mtd->pc_start;
#ifdef KNH_KLR_CONVERT
		knh_KLRCode_toCttCode(ctx, lsfp+1); /* convert KLRCode to CTT  */
#else
		knh_KLRCode_exec(ctx, lsfp+1); /* code threading */
#endif
		DBG2_P("mtd(%p)", mtd);
		DBG2_DUMP(ctx, mtd, KNH_NULL, "Compiled Code");
	}
	knh_Gamma_gc(ctx);
}

/* ======================================================================== */
/* [ASM] */

#define sfi_(n)    ((knh_sfpidx_t)(n))

#define IS_VAL(t) (IS_BOOL(t)||IS_INT(t)||IS_FLOAT(t))
#define IS_ANY(t) (CLASS_type(t) == CLASS_Any || CLASS_type(t) == CLASS_Object)
#define IS_BOX(t)


/* ------------------------------------------------------------------------ */

void knh_asmop(Ctx *ctx, knh_inst_t *op)
{
	knh_Gamma_t *kc = ctx->kc;
	knh_KLRInst_t *inst = new_KLRInst(ctx, op);
	inst->line = SP(kc)->line;
	knh_Array_add(ctx, DP(kc)->insts, UP(inst));
}

/* ------------------------------------------------------------------------ */

//                            int   Int    Any(sfp)   Any(obj)
// int [??, 1]                             [Int0, 1]  [Int, 1]
// Int [null, ??] [??, 1]

//                            int   Int    Any(sfp)   Any(obj)
// int [Int0, 1]                           [Int0, 1]  [Int, 1]
// Int [null, ??] [Int0, 1]

/* ------------------------------------------------------------------------ */

static
void KNH_ASM_BOX(Ctx *ctx, knh_type_t reqt, knh_type_t atype, int a)
{
	knh_Gamma_t *kc = ctx->kc;
	knh_KLRInst_t *iLAST = knh_InstList_lastNULL(DP(kc)->insts);
	if(iLAST != NULL) {
		if(OPCODE_NNBOX == iLAST->opcode) { /* PEEPHOLE */
			knh_KLRInst_setopcode(iLAST, OPCODE_BOX);
		}
		else if(OPCODE_NNBOXnc == iLAST->opcode) { /* PEEPHOLE */
			knh_KLRInst_setopcode(iLAST, OPCODE_BOXnc);
		}
	}
	else {
		knh_class_t cid = CLASS_type(atype);
		DBG2_ASSERT_cid(cid);
		knh_class_t bcid = ClassTable(cid).bcid;
		if(bcid == CLASS_Boolean || bcid == CLASS_Int || bcid == CLASS_Float) {
			knh_class_t rcid = ClassTable(CLASS_type(reqt)).bcid;
			if(rcid != bcid) {
				if(IS_NNTYPE(atype)) {
					KNH_ASM(BOX, a, cid);
				}
				else {
					KNH_ASM(BOXnc, a, cid);
				}
			}
		}
	}
}

/* ------------------------------------------------------------------------ */

static
void KNH_ASM_NNBOX(Ctx *ctx, knh_type_t reqt, knh_type_t atype, int a)
{
	knh_class_t cid = CLASS_type(atype);
	DBG2_ASSERT_cid(cid);
	knh_class_t bcid = ClassTable(cid).bcid;
	if(bcid == CLASS_Boolean || bcid == CLASS_Int || bcid == CLASS_Float) {
		knh_class_t rcid = ClassTable(CLASS_type(reqt)).bcid;
		if(rcid != bcid) {
			if(IS_NNTYPE(atype)) {
				KNH_ASM(NNBOX, a, cid);
			}
			else {
				KNH_ASM(NNBOXnc, a, cid);
			}
		}
	}
}

/* ------------------------------------------------------------------------ */

static
void KNH_ASM_SMOVx(Ctx *ctx, knh_type_t atype, int a, knh_type_t btype, knh_sfx_t bx)
{
#ifdef KNH_USING_UNBOXFIELD
	if(IS_ubxint(btype)) {
		DBG2_P("atype=%s%s, btype=%s%s", TYPEQN(atype), TYPEQN(btype));
		KNH_ASM(MOVxi, sfi_(a), bx);
		KNH_ASM_NNBOX(ctx, atype, btype, sfi_(a));
		return;
	}
	if(IS_ubxfloat(btype)) {
		KNH_ASM(MOVxf, sfi_(a), bx);
		KNH_ASM_NNBOX(ctx, atype, btype, sfi_(a));
		return;
	}
	if(IS_ubxboolean(btype)) {
		KNH_ASM(MOVxb, sfi_(a), bx);
		KNH_ASM_NNBOX(ctx, atype, btype, sfi_(a));
		return;
	}
#endif
	if(IS_NNTYPE(atype) && !IS_NNTYPE(btype)) {
		KNH_ASM(CHKNULx, bx);
		btype = NNTYPE_cid(btype);
	}
	KNH_ASM(MOVx, sfi_(a), bx);
}

/* ------------------------------------------------------------------------ */

static
void KNH_ASM_SMOV(Ctx *ctx, knh_type_t atype, int a, knh_Token_t *tkb)
{
	KNH_ASSERT(IS_Token(tkb));
	knh_type_t btype = DP(tkb)->type;

	switch(TT_(tkb)) {
		case TT_CLASSID:
		case TT_CONST: {
			Object *v = DP(tkb)->data;
			KNH_ASM(MOVo, sfi_(a), v);
			break;
		}
		case TT_LOCAL: {
			int b = (int)DP(tkb)->index;
			knh_type_t btype = DP(tkb)->type;
			if(IS_NNTYPE(atype) && IS_NATYPE(btype)) {
				KNH_ASM(CHKNUL, b);
				btype = NNTYPE_cid(btype);
			}
			if(IS_ubxtype(btype)) {
				KNH_ASM(MOVn, sfi_(a), sfi_(b));
				KNH_ASM_NNBOX(ctx, atype, btype, a);
			}
			else if(IS_bxint(btype) || IS_bxfloat(btype)) {
				KNH_ASM(MOVa, sfi_(a), sfi_(b));
				KNH_ASM_NNBOX(ctx, atype, btype, a);
			}
			else {
				KNH_ASM(MOVa, sfi_(a), sfi_(b));
			}
			break;
		}

		case TT_FIELD: {
			int b = (int)DP(tkb)->index;
			knh_sfx_t bx = {sfi_(0), (size_t)b};
			KNH_ASM_SMOVx(ctx, atype, a, btype, bx);
			break;
		}

		case TT_SCRIPT: {
			knh_Gamma_t *kc = ctx->kc;
			int b = (int)DP(tkb)->index;
			knh_sfx_t bx = {sfi_(DP(kc)->globalidx), (size_t)b};
			ASM_ASSERT(ctx, bx.i != -1);
			KNH_ASM_SMOVx(ctx, atype, a, btype, bx);
			break;
		}

		case TT_MEMBER: {
			int idx = (int)DP(tkb)->index;
			size_t b = (size_t)(DP(tkb)->num)->n.ivalue;
			knh_sfx_t bx = {sfi_(idx), b};
			KNH_ASM_SMOVx(ctx, atype, a, btype, bx);
			break;
		}

		case TT_DEFVAL: {
			knh_class_t cid = DP(tkb)->cid;
			KNH_ASM(MOVDEF, sfi_(a), cid);
			break;
		}

		case TT_SYSVAL: {
			knh_ushort_t sysid = DP(tkb)->index;
			KNH_ASM(MOVSYS, sfi_(a), sysid);
			break;
		}

		case TT_NOP: {
			return;
		}

		case TT_CLOSURE: {
			knh_Gamma_t *kc = ctx->kc;
			int local = ASML(a);
			knh_Method_t *mtd = DP(tkb)->mtd;
			knh_class_t cid = DP(mtd)->cid;
			if(cid == DP(kc)->this_cid || knh_class_instanceof(ctx, DP(kc)->this_cid, cid)) {
				KNH_ASM(MOVa, sfi_(local+2), sfi_(0));
			}
			else if(cid == knh_Object_cid(knh_getGammaScript(ctx))) {
				KNH_ASM(MOVo, sfi_(local+2), UP(knh_getGammaScript(ctx)));
			}
			else {
				KNH_ASM(MOVDEF, sfi_(local+2), cid);
			}
			KNH_ASM(MOVo, sfi_(local+3), UP(mtd));
			mtd = knh_Class_getMethod(ctx, CLASS_Closure, METHODN_new);
			DBG2_ASSERT(IS_Method(mtd));
			KNH_ASM(NEW, local, 0, CLASS_type(DP(tkb)->type), 2, mtd);
			if(knh_Token_isOUTERCLOSURE(tkb)) {
				KNH_ASM(COPYSFP, local);
			}
			MOVL(ctx, local, a);
			break;
		}
		default: {
			DBG2_P("unknown TT=%s", knh_token_tochar(TT_(tkb)));
			KNH_ASSERT(ctx == NULL);
		}
	}/*switch*/
}

/* ------------------------------------------------------------------------ */

static
void KNH_ASM_XMOVx(Ctx *ctx, knh_type_t atype, knh_sfx_t ax, knh_type_t btype, knh_sfx_t bx)
{
	if(IS_NNTYPE(atype) && !IS_NNTYPE(btype)) {
		KNH_ASM(CHKNULx, bx);
	}
	if(IS_ubxint(atype)) {
		if(IS_ubxint(btype)) {
			KNH_ASM(XMOVxi, ax, bx);
		}
		else {
			KNH_ASM(XMOVxio, ax, bx);
		}
		return;
	}
	if(IS_ubxfloat(atype)) {
		if(IS_ubxfloat(btype)) {
			KNH_ASM(XMOVxf, ax, bx);
		}
		else {
			KNH_ASM(XMOVxfo, ax, bx);
		}
		return;
	}
	if(IS_ubxboolean(atype) && IS_ubxboolean(btype)) {
		KNH_ASM(XMOVxb, ax, bx);
		return;
	}
	if(IS_bxint(atype) && IS_ubxint(btype)) {
		KNH_ASM(XMOVxBXi, ax, bx, CLASS_type(btype));
		return;
	}
	if(IS_bxfloat(atype) && IS_ubxfloat(btype)) {
		KNH_ASM(XMOVxBXf, ax, bx, CLASS_type(btype));
		return;
	}
	if(IS_ubxint(btype)) { // Any a = b; // int b;
		DBG2_P("atype=%s%s", TYPEQN(atype));
		KNH_ASM(MOVxi, DP(ctx->kc)->esp, bx);
		KNH_ASM(BOX, DP(ctx->kc)->esp, CLASS_type(btype));
		KNH_ASM(XMOVs, ax, DP(ctx->kc)->esp);
		return;
	}
	if(IS_ubxfloat(btype)) { // Any a = b; // float b;
		DBG2_P("atype=%s%s", TYPEQN(atype));
		KNH_ASM(MOVxf, DP(ctx->kc)->esp, bx);
		KNH_ASM(BOX, DP(ctx->kc)->esp, CLASS_type(btype));
		KNH_ASM(XMOVs, ax, DP(ctx->kc)->esp);
		return;
	}
	if(IS_ubxboolean(btype)) { // Any a = b; // boolean b;
		DBG2_P("atype=%s%s", TYPEQN(atype));
		KNH_ASM(MOVxb, DP(ctx->kc)->esp, bx);
		KNH_ASM(BOX, DP(ctx->kc)->esp, CLASS_type(btype));
		KNH_ASM(XMOVs, ax, DP(ctx->kc)->esp);
		return;
	}
	KNH_ASM(XMOVx, ax, bx);
}

/* ------------------------------------------------------------------------ */

static
void KNH_ASM_XMOV(Ctx *ctx, knh_type_t atype, int a, size_t an, knh_Token_t *tkb)
{
	KNH_ASSERT(IS_Token(tkb) && knh_Token_isTyped(tkb));
	knh_sfx_t ax = {sfi_(a), an};
	knh_type_t btype = DP(tkb)->type;
	switch(TT_(tkb)) {
		case TT_CLASSID:
		case TT_CONST: {
			Object *v = DP(tkb)->data;
#ifdef KNH_USING_UNBOXFIELD
			if(IS_ubxint(atype)) {
				KNH_ASM(XMOVoi, ax, v);
				break;
			}
			if(IS_ubxfloat(atype)) {
				KNH_ASM(XMOVof, ax, v);
				break;
			}
			if(IS_ubxboolean(atype)) {
				KNH_ASM(XMOVob, ax, v);
				break;
			}
#endif/*KNU_USING_UNBOXFIED*/
			KNH_ASM(XMOVo, ax, v);
			break;
		}

		case TT_LOCAL: {
			int b = (int)DP(tkb)->index;
			if(IS_NNTYPE(atype) && !IS_NNTYPE(btype)) {
				KNH_ASM(CHKNUL, b);
			}
#ifdef KNH_USING_UNBOXFIELD
			if(IS_ubxint(atype)) {
				KNH_ASM(XMOVsi, ax, sfi_(b));
				break;
			}
			if(IS_ubxfloat(atype)) {
				KNH_ASM(XMOVsf, ax, sfi_(b));
				break;
			}
			if(IS_ubxboolean(atype)) {
				KNH_ASM(XMOVsb, ax, sfi_(b));
				break;
			}
#endif/*KNU_USING_UNBOXFIED*/
			KNH_ASM_BOX(ctx, atype, btype, b);
			KNH_ASM(XMOVs, ax, sfi_(b));
			break;
		}

		case TT_FIELD: {
			int b = (int)DP(tkb)->index;
			knh_sfx_t bx = {(knh_sfpidx_t)0, (size_t)b};
			KNH_ASM_XMOVx(ctx, atype, ax, btype, bx);
			break;
		}

		case TT_SCRIPT: {
			knh_Gamma_t *kc = ctx->kc;
			int b = (int)DP(tkb)->index;
			knh_sfx_t bx = {(knh_sfpidx_t)DP(kc)->globalidx, (size_t)b};
			ASM_ASSERT(ctx, bx.i != -1);
			KNH_ASM_XMOVx(ctx, atype, ax, btype, bx);
			break;
		}

		case TT_MEMBER : {
			int idx = (int)DP(tkb)->index;
			size_t b = (size_t)(DP(tkb)->num)->n.ivalue;
			knh_sfx_t bx = {sfi_(idx), b};
			KNH_ASM_XMOVx(ctx, atype, ax, btype, bx);
			break;

		}
		case TT_DEFVAL: {
#ifdef KNH_USING_UNBOXFIELD
			if(IS_ubxint(atype)) {
				KNH_ASM(XMOVoi, ax, KNH_DEF(ctx, CLASS_type(atype)));
			}
			else if(IS_ubxfloat(atype)) {
				KNH_ASM(XMOVof, ax, KNH_DEF(ctx, CLASS_type(atype)));
			}
			else if(IS_ubxboolean(atype)) {
				KNH_ASM(XMOVob, ax, KNH_FALSE);
			}
			else
#endif/*KNU_USING_UNBOXFIED*/
			{
				knh_class_t cid = DP(tkb)->cid;
				KNH_ASM(XMOVDEF, ax, cid);
			}
			break;
		}

		case TT_SYSVAL: {
			knh_ushort_t sysid = DP(tkb)->index;
			KNH_ASM(XMOVSYS, ax, sysid);
			break;
		}

		case TT_CLOSURE: {
			knh_Gamma_t *kc = ctx->kc;
			int local = DP(ctx->kc)->esp;
			knh_Method_t *mtd = DP(tkb)->mtd;
			knh_class_t cid = DP(mtd)->cid;
			if(cid == DP(kc)->this_cid || knh_class_instanceof(ctx, DP(kc)->this_cid, cid)) {
				KNH_ASM(MOVa, sfi_(local+2), sfi_(0));
			}
			else if(cid == knh_Object_cid(knh_getGammaScript(ctx))) {
				KNH_ASM(MOVo, sfi_(local+2), UP(knh_getGammaScript(ctx)));
			}
			else {
				KNH_ASM(MOVDEF, sfi_(local+2), cid);
			}
			KNH_ASM(MOVo, sfi_(local+3), UP(mtd));
			mtd = knh_Class_getMethod(ctx, CLASS_Closure, METHODN_new);
			KNH_ASSERT(IS_Method(mtd));
			KNH_ASM(NEW, local, 0, CLASS_type(DP(tkb)->type), 2, mtd);
			KNH_ASM(XMOVs, ax, sfi_(local));
			break;
		}

		case TT_NOP: {
			return;
		}

		default: {
			DBG2_P("unknown TT=%d", TT_(tkb));
			KNH_ASSERT(ctx == NULL);
		}
	}/*switch*/

}

/* ------------------------------------------------------------------------ */

static
void KNH_ASM_MOV(Ctx *ctx, knh_Token_t *tka, knh_Token_t *tkb)
{
	knh_Gamma_t *kc = ctx->kc;
	KNH_ASSERT(IS_Token(tka) && knh_Token_isTyped(tka));
	knh_type_t atype = DP(tka)->type;

	if(TT_(tka) == TT_LOCAL) {
		KNH_ASM_SMOV(ctx, atype, (int)DP(tka)->index, tkb);
	}
	else if(TT_(tka) == TT_FIELD) {
		int an = (int)DP(tka)->index;
		KNH_ASM_XMOV(ctx, atype, 0, an, tkb);
	}
	else if(TT_(tka) == TT_SCRIPT) {
		int a = (int)DP(kc)->globalidx;
		ASM_ASSERT(ctx, a != -1);
		int an = (int)DP(tka)->index;
		KNH_ASM_XMOV(ctx, atype, a, an, tkb);
	}
	else if(TT_(tka) == TT_MEMBER) {
		int a = (int)DP(tka)->index;
		ASM_ASSERT(ctx, IS_Int(DP(tka)->data));
		int an = (int)(DP(tka)->num)->n.ivalue;
		KNH_ASM_XMOV(ctx, atype, a, an, tkb);
	}
	else if(TT_(tka) == TT_NOP) {

	}
	else {
		DBG2_P("unknown TT=%d", TT_(tka));
		KNH_ASSERT(ctx == NULL);
	}
}

/* ------------------------------------------------------------------------ */

static
void KNH_ASM_RET(Ctx *ctx)
{
	knh_KLRInst_t *iLAST = knh_InstList_lastNULL(DP(ctx->kc)->insts);
	if(iLAST != NULL) {
		knh_opcode_t p = iLAST->opcode;
		klr_MOVn_t *op = (klr_MOVn_t*)(iLAST)->op;
		if(OPCODE_RET <= p && p <= OPCODE_RETx) {
			return;
		}
		if(p == OPCODE_MOVn) {
			if(op->a1 == -1) {
				knh_KLRInst_setopcode(iLAST, OPCODE_RETn);
				return;
			}
		}
		else if(p == OPCODE_MOVa) { /* PEEPHOLE */
			if(op->a1 == -1) {
				knh_KLRInst_setopcode(iLAST, OPCODE_RETa);
				return;
			}
		}
		else if(p == OPCODE_MOVo) { /* PEEPHOLE */
			if(op->a1 == -1) {
				knh_KLRInst_setopcode(iLAST, OPCODE_RETo);
				return;
			}
		}
		else if(p == OPCODE_MOVx) { /* PEEPHOLE */
			if(op->a1 == -1) {
				knh_KLRInst_setopcode(iLAST, OPCODE_RETx);
				return;
			}
		}

	}
	KNH_ASM(RET);
}

/* ------------------------------------------------------------------------ */

static
void KNH_ASM_CALL(Ctx *ctx, knh_type_t reqt, int sfpidx, knh_Token_t *tkb, size_t args)
{
	KNH_ASSERT(IS_Token(tkb));
	KNH_ASSERT(sfpidx >= DP(ctx->kc)->esp);
	knh_Method_t *mtd = DP(tkb)->mtd;
	knh_type_t rtype = CLASS_Any;
	if(IS_Method(mtd)) {
		if(knh_Method_isFinal(mtd) || knh_Method_isConstructor(ctx, mtd) || knh_Token_isSUPER(tkb)) {
			KNH_ASM(SCALL, sfpidx, args + 2, mtd);
		}
		else {
			KNH_ASM(CALL, sfpidx, args + 2, DP(mtd)->mn);
		}
		rtype = knh_Method_rztype(mtd);
		if(IS_NNTYPE(reqt) && !IS_NNTYPE(rtype)) {
			KNH_ASM(CHKNUL, sfpidx);
		}
	}
	else if(TT_(tkb) == TT_MN) {
		KNH_ASM(ACALL, sfpidx, args + 2, DP(tkb)->mn);
		if(IS_NNTYPE(reqt) && !IS_NNTYPE(rtype)) {
			KNH_ASM(CHKNUL, sfpidx);
		}
	}
	else {
		KNH_ASM_PANIC(ctx, "unknown call type");
	}
}

/* ------------------------------------------------------------------------ */

static
void KNH_ASM_NEW(Ctx *ctx, knh_type_t reqt, int sfpidx, knh_Token_t *tkb,
		knh_flag_t flag, knh_class_t cid, size_t args)
{
	KNH_ASSERT(IS_Token(tkb));
	KNH_ASSERT(sfpidx >= DP(ctx->kc)->esp);
	knh_Method_t *mtd = DP(tkb)->mtd;
	KNH_ASM(NEW, sfpidx, flag, cid, args + 2, mtd);

	knh_type_t rtype = knh_Method_rztype(mtd);
	if(IS_NNTYPE(reqt) && !IS_NNTYPE(rtype)) {
		KNH_ASM(CHKNUL, sfpidx);
	}
}

/* ------------------------------------------------------------------------ */

static
void KNH_ASM_MAP(Ctx *ctx, knh_type_t reqt, int sfpidx, knh_Token_t *tkb, knh_type_t srct, int isNonNullCast)
{
	DBG2_ASSERT(IS_Token(tkb));
	knh_Mapper_t *mpr = DP(tkb)->mpr;

	if(IS_Mapper(mpr)) {
//		DBG2_P("reqt=%s%s tcid=%s srct=%s%s isNonNullCast=%d",
//				TYPEQN(reqt), CLASSN(DP(mpr)->tcid), TYPEQN(srct), isNonNullCast);
		if(knh_Mapper_isFinal(mpr)) {
			if(IS_NNTYPE(srct)) {
				if(DP(mpr)->scid == CLASS_Int && DP(mpr)->tcid == CLASS_Float) {
					KNH_ASM(fCAST, sfpidx);
					return;
				}
				else if(DP(mpr)->scid == CLASS_Float && DP(mpr)->tcid == CLASS_Int) {
					KNH_ASM(iCAST, sfpidx);
					return;
				}
				KNH_ASM(SMAP, sfpidx, mpr);
				if(!knh_Mapper_isTotal(mpr)) srct = CLASS_type(srct);
			}
			else {
				if(DP(mpr)->scid == CLASS_Int && DP(mpr)->tcid == CLASS_Float) {
					KNH_ASM(fnCAST, sfpidx);
					return;
				}
				else if(DP(mpr)->scid == CLASS_Float && DP(mpr)->tcid == CLASS_Int) {
					KNH_ASM(inCAST, sfpidx);
					return;
				}
				KNH_ASM(SMAPnc, sfpidx, mpr);
			}
		}
		else {
			if(IS_NNTYPE(srct)) {
				KNH_ASM(MAP, sfpidx, DP(mpr)->tcid);
				if(!knh_Mapper_isTotal(mpr)) {
					srct = CLASS_type(srct);
				}
			}
			else {
				KNH_ASM(MAPnc, sfpidx, DP(mpr)->tcid);
			}
		}
	}
	else {
		KNH_ASSERT(TT_(tkb) == TT_MPR);
		//DBG2_P("reqt=%s%s mprcid=%s srct=%s%s isNonNullCast=%d", TYPEQN(reqt), DP(tkb)->cid, TYPEQN(srct), isNonNullCast);
		KNH_ASM(AMAP, sfpidx, DP(tkb)->cid);
		srct = CLASS_type(srct);
	}
	if(!IS_NNTYPE(srct) && isNonNullCast) {
		KNH_ASM(NNMAP, sfpidx, DP(tkb)->cid);
	}
	else if(IS_NNTYPE(reqt)) {
		KNH_ASM(CHKNUL, sfpidx);
	}
}

/* ======================================================================== */
/* [CONTROL] */

static
knh_KLRInst_t* new_KLRInstLABEL(Ctx *ctx)
{
	knh_KLRInst_t *inst
		= (knh_KLRInst_t*)new_Object_init(ctx, FLAG_KLRInst, CLASS_KLRInst, OPCODE_LABEL);
	return inst;
}

/* ------------------------------------------------------------------------ */

static void KNH_ASM_LABEL(Ctx *ctx, knh_KLRInst_t* label)
{
	knh_Gamma_t *kc = ctx->kc;
	label->line = SP(kc)->line;
	knh_Array_add(ctx, DP(kc)->insts, UP(label));
}

/* ------------------------------------------------------------------------ */

static int TERMs_isCALLISNUL(knh_Stmt_t *stmt, size_t n)
{
	if(IS_Stmt(DP(stmt)->stmts[n])) {
		knh_Token_t *tk = DP(DP(stmt)->stmts[n])->tokens[0];
		return (IS_Token(tk) && IS_Method(DP(tk)->mtd) && DP(DP(tk)->mtd)->mn == METHODN_isNull);
	}
	return 0;
}

/* ------------------------------------------------------------------------ */

static int TERMs_isCALLISNN(knh_Stmt_t *stmt, size_t n)
{
	if(IS_Stmt(DP(stmt)->stmts[n])) {
		knh_Token_t *tk = DP(DP(stmt)->stmts[n])->tokens[0];
		return (IS_Token(tk) && IS_Method(DP(tk)->mtd) && DP(DP(tk)->mtd)->mn == METHODN_isNotNull);
	}
	return 0;
}

/* ------------------------------------------------------------------------ */

static
void TERMs_ASM_JIFNUL(Ctx *ctx, knh_Stmt_t *stmt, size_t n, knh_KLRInst_t* label)
{
	knh_Token_t *tk = DP(stmt)->tokens[n];
	if(IS_Token(tk) && TT_(tk) == TT_LOCAL /*TT_LOCAL ??*/) {
		KNH_ASM(JIFNUL, TADDR label, sfi_(DP(tk)->index));
	}
	else {
		TERMs_asm(ctx, stmt, n, TERMs_gettype(stmt, n), DP(ctx->kc)->esp);
		KNH_ASM(JIFNUL, TADDR label, sfi_(DP(ctx->kc)->esp));
	}
}

/* ------------------------------------------------------------------------ */

static
void TERMs_ASM_JIFNN(Ctx *ctx, knh_Stmt_t *stmt, size_t n, knh_KLRInst_t* label)
{
	knh_Token_t *tk = DP(stmt)->tokens[n];
	if(IS_Token(tk) && TT_(tk) == TT_LOCAL /*TT_LOCAL ??*/) {
		KNH_ASM(JIFNN, TADDR label, sfi_(DP(tk)->index));
	}
	else {
		TERMs_asm(ctx, stmt, n, TERMs_gettype(stmt, n), DP(ctx->kc)->esp);
		KNH_ASM(JIFNN, TADDR label, sfi_(DP(ctx->kc)->esp));
	}
}

/* ------------------------------------------------------------------------ */

static
void TERMs_ASM_JIFF(Ctx *ctx, knh_Stmt_t *stmt, size_t n, knh_KLRInst_t* label)
{
	knh_Token_t *tk = DP(stmt)->tokens[n];
	if(IS_Token(tk) && TT_(tk) == TT_LOCAL /*TT_LOCAL ??*/) {
		KNH_ASM(bJIFF, TADDR label, sfi_(DP(tk)->index));
	}
	else {
		if(TERMs_isCALLISNUL(stmt, n)) {
			// o == null -> false  o == null;
			TERMs_ASM_JIFNN(ctx, DP(stmt)->stmts[n], 1, label);
			return;
		}
		else if(TERMs_isCALLISNN(stmt, n)) {
			TERMs_ASM_JIFNUL(ctx, DP(stmt)->stmts[n], 1, label);
			return;
		}
		TERMs_asm(ctx, stmt, n, NNTYPE_Boolean, DP(ctx->kc)->esp);
		KNH_ASM(bJIFF, TADDR label, sfi_(DP(ctx->kc)->esp));
	}
}

/* ------------------------------------------------------------------------ */

static
void TERMs_ASM_JIFF_LOOP(Ctx *ctx, knh_Stmt_t *stmt, size_t n, knh_KLRInst_t* label)
{
	knh_Token_t *tk = DP(stmt)->tokens[n];
	if(IS_Token(tk) && TT_(tk) == TT_LOCAL /*TT_LOCAL ??*/) {
		KNH_ASM(bJIFF_LOOP, TADDR label, sfi_(DP(tk)->index));
	}
	else {
		if(TERMs_isCALLISNUL(stmt, n)) {
			// o == null -> false  o == null;
			TERMs_ASM_JIFNN(ctx, DP(stmt)->stmts[n], 1, label);
			return;
		}
		else if(TERMs_isCALLISNN(stmt, n)) {
			TERMs_ASM_JIFNUL(ctx, DP(stmt)->stmts[n], 1, label);
			return;
		}
		TERMs_asm(ctx, stmt, n, NNTYPE_Boolean, DP(ctx->kc)->esp);
		KNH_ASM(bJIFF_LOOP, TADDR label, sfi_(DP(ctx->kc)->esp));
	}
}

/* ------------------------------------------------------------------------ */

static
void TERMs_ASM_JIFT(Ctx *ctx, knh_Stmt_t *stmt, size_t n, knh_KLRInst_t* label)
{
	knh_Token_t *tk = DP(stmt)->tokens[n];
	if(IS_Token(tk) && TT_(tk) == TT_LOCAL /*TT_LOCAL ??*/) {
		KNH_ASM(bJIFT, TADDR label, sfi_(DP(tk)->index));
	}
	else {
		if(TERMs_isCALLISNUL(stmt, n)) {
			// o == null -> false  o == null;
			TERMs_ASM_JIFNN(ctx, DP(stmt)->stmts[n], 1, label);
			return;
		}
		else if(TERMs_isCALLISNN(stmt, n)) {
			TERMs_ASM_JIFNUL(ctx, DP(stmt)->stmts[n], 1, label);
			return;
		}
		TERMs_asm(ctx, stmt, n, NNTYPE_Boolean, DP(ctx->kc)->esp);
		KNH_ASM(bJIFT, TADDR label, sfi_(DP(ctx->kc)->esp));
	}
}

/* ------------------------------------------------------------------------ */

static
void TERMs_ASM_THROW(Ctx *ctx, knh_Stmt_t *stmt, size_t n)
{
	knh_Token_t *tk = DP(stmt)->tokens[n];

	if(IS_Token(tk) && TT_(tk) == TT_LOCAL /*TT_LOCAL ??*/) {
		KNH_ASM(THROW, knh_Gamma_inTry(ctx), sfi_(DP(tk)->index));
	}
	else {
		TERMs_asm(ctx, stmt, n, NNTYPE_Exception, DP(ctx->kc)->esp);
		KNH_ASM(THROW, knh_Gamma_inTry(ctx), sfi_(DP(ctx->kc)->esp));
	}
}

/* ------------------------------------------------------------------------ */
/* [EXPR] */

static
knh_type_t knh_Method_reqtTERMs(Ctx *ctx, knh_Method_t *mtd, knh_class_t mtd_cid, knh_Stmt_t *stmt, size_t n)
{
	if(!IS_Method(mtd)) {
		return TYPE_Any;  // boxing
	}
	if(n == 1) { // base
		DBG2_(
		if(mtd_cid != DP(mtd)->cid) {
			DBG2_P("mtd_cid=%s, DP(mtd)->cid=%s", CLASSN(mtd_cid), CLASSN(DP(mtd)->cid));
		});
		mtd_cid = DP(mtd)->cid;
		return knh_Method_isNullBase(mtd) ? NATYPE_cid(mtd_cid) : NNTYPE_cid(mtd_cid);
	}
	else {
		knh_type_t ptype = knh_Method_pztype(mtd, n - 2);
		if(ptype == TYPE_Object || ptype == TYPE_T1 || ptype == TYPE_T2) {
			return TYPE_Any; // boxing
		}
		if(ptype == NNTYPE_This) return NNTYPE_cid(mtd_cid);
		if(ptype == TYPE_This) return mtd_cid;
		return ptype;
	}
}

/* ------------------------------------------------------------------------ */

static
int TERMs_putLOCAL(Ctx *ctx, knh_Stmt_t *stmt, size_t n, knh_type_t reqt, int sfpidx)
{
	knh_Token_t *tk = DP(stmt)->tokens[n];
	if(IS_Token(tk) && TT_(tk) == TT_LOCAL) {
		int a = DP(tk)->index;
		if(!IS_NNTYPE(DP(tk)->type)) {
			KNH_ASM(CHKNUL, a);
		}
		return a;
	}
	TERMs_asm(ctx, stmt, n, reqt, sfpidx);
	return sfpidx;
}

/* ------------------------------------------------------------------------ */

static
int knh_StmtOP_checkConst(Ctx *ctx, knh_Stmt_t *stmt, knh_methodn_t *mn, int swap)
{
	Term *tm = DP(stmt)->terms[1];
	if(swap && IS_Token(tm) && TT_((knh_Token_t*)tm) == TT_CONST) {
		knh_methodn_t newmn = *mn;
		knh_Stmt_swap(ctx, stmt, 1, 2);
		if(*mn == METHODN_opLt) newmn = METHODN_opGt;
		else if(*mn == METHODN_opLte) newmn = METHODN_opGte;
		else if(*mn == METHODN_opGt) newmn = METHODN_opLt;
		else if(*mn == METHODN_opGte) newmn = METHODN_opLte;
		*mn = newmn;
		return 1;
	}
	else {
		knh_Token_t *tk = DP(stmt)->tokens[2];
		return (IS_Token(tk) && TT_(tk) == TT_CONST);
	}
}

/* ------------------------------------------------------------------------ */

static
knh_int_t TERMs_int(knh_Stmt_t *stmt, size_t n)
{
	knh_Token_t *tk = DP(stmt)->tokens[n];
	KNH_ASSERT(TT_(tk) == TT_CONST);
	return (DP(tk)->num)->n.ivalue;
}

/* ------------------------------------------------------------------------ */

static
knh_float_t TERMs_float(knh_Stmt_t *stmt, size_t n)
{
	knh_Token_t *tk = DP(stmt)->tokens[n];
	KNH_ASSERT(TT_(tk) == TT_CONST);
	return (DP(tk)->num)->n.fvalue;
}

/* ------------------------------------------------------------------------ */

static
int IS_OPSIM(knh_methodn_t mn)
{
	switch(mn) {
	case METHODN_opAdd:
	case METHODN_opMul:
	case METHODN_opEq:
	case METHODN_opNeq:
	case METHODN_opLt:
	case METHODN_opLte:
	case METHODN_opGt:
	case METHODN_opGte:
		return 1;
	}
	return 0;
}

/* ------------------------------------------------------------------------ */

static
int IS_OPASIM(knh_methodn_t mn)
{
	switch(mn) {
	case METHODN_opSub:
	case METHODN_opDiv:
	case METHODN_opMod:
		return 1;
	}
	return 0;
}

/* ------------------------------------------------------------------------ */

static
int knh_StmtOP_asm(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	int local = ASML(sfpidx);
	knh_Method_t *mtd = DP(DP(stmt)->tokens[0])->mtd;
	if(IS_NULL(mtd)) return 0;
	knh_methodn_t mn = DP(mtd)->mn;
	knh_class_t cid = CLASS_type(DP(DP(stmt)->tokens[1])->type);

	if(cid == CLASS_Int) {
		if(IS_OPSIM(mn)) {
			if(knh_StmtOP_checkConst(ctx, stmt, &mn, /*swap*/ 1)) {
				int a = TERMs_putLOCAL(ctx, stmt, 1, NNTYPE_Int, local + 1);
				knh_int_t b = TERMs_int(stmt, 2);
				switch(mn) {
				case METHODN_opAdd:
					KNH_ASM(iADDn, sfi_(sfpidx), sfi_(a), b);
					return 1;
				case METHODN_opMul:
					KNH_ASM(iMULn, sfi_(sfpidx), sfi_(a), b);
					return 1;
				case METHODN_opEq:
					KNH_ASM(iEQn, sfi_(sfpidx), sfi_(a), b);
					return 1;
				case METHODN_opNeq:
					KNH_ASM(iNEQn, sfi_(sfpidx), sfi_(a), b);
					return 1;
				case METHODN_opLt:
					KNH_ASM(iLTn, sfi_(sfpidx), sfi_(a), b);
					return 1;
				case METHODN_opLte:
					KNH_ASM(iLTEn, sfi_(sfpidx), sfi_(a), b);
					return 1;
				case METHODN_opGt:
					KNH_ASM(iGTn, sfi_(sfpidx), sfi_(a), b);
					return 1;
				case METHODN_opGte:
					KNH_ASM(iGTEn, sfi_(sfpidx), sfi_(a), b);
					return 1;
				}
				return 0;
			}
			else {
				int a = TERMs_putLOCAL(ctx, stmt, 1, NNTYPE_Int, local + 1);
				int b = TERMs_putLOCAL(ctx, stmt, 2, NNTYPE_Int, local + 2);
				switch(mn) {
				case METHODN_opAdd:
					KNH_ASM(iADD, sfi_(sfpidx), sfi_(a), sfi_(b));
					return 1;
				case METHODN_opMul:
					KNH_ASM(iMUL, sfi_(sfpidx), sfi_(a), sfi_(b));
					return 1;
				case METHODN_opEq:
					KNH_ASM(iEQ, sfi_(sfpidx), sfi_(a), sfi_(b));
					return 1;
				case METHODN_opNeq:
					KNH_ASM(iNEQ, sfi_(sfpidx), sfi_(a), sfi_(b));
					return 1;
				case METHODN_opLt:
					KNH_ASM(iLT, sfi_(sfpidx), sfi_(a), sfi_(b));
					return 1;
				case METHODN_opLte:
					KNH_ASM(iLTE, sfi_(sfpidx), sfi_(a), sfi_(b));
					return 1;
				case METHODN_opGt:
					KNH_ASM(iGT, sfi_(sfpidx), sfi_(a), sfi_(b));
					return 1;
				case METHODN_opGte:
					KNH_ASM(iGTE, sfi_(sfpidx), sfi_(a), sfi_(b));
					return 1;
				}
				return 0;
			}
		}
		else if(IS_OPASIM(mn)) {
			if(knh_StmtOP_checkConst(ctx, stmt, &mn, /*swap*/ 0)) {
				int a = TERMs_putLOCAL(ctx, stmt, 1, NNTYPE_Int, local + 1);
				knh_int_t b = TERMs_int(stmt, 2);
				switch(mn) {
				case METHODN_opSub:
					KNH_ASM(iSUBn, sfi_(sfpidx), sfi_(a), b);
					return 1;
				case METHODN_opDiv:
					if(b == 0) {
						b = 1;
						knh_Gamma_perror(ctx, KERR_ERRATA, _("divided by zero: /0 ==> /1"));
					}
					KNH_ASM(iDIVn, sfi_(sfpidx), sfi_(a), b);
					return 1;

				case METHODN_opMod:
					if(b == 0) {
						b = 1;
						knh_Gamma_perror(ctx, KERR_ERRATA, _("divided by zero: %0 ==> %1"));
					}
					KNH_ASM(iMODn, sfi_(sfpidx), sfi_(a), b);
					return 1;
				}
			}
			else {
				int a = TERMs_putLOCAL(ctx, stmt, 1, NNTYPE_Int, local + 1);
				int b = TERMs_putLOCAL(ctx, stmt, 2, NNTYPE_Int, local + 2);
				switch(mn) {
				case METHODN_opSub:
					KNH_ASM(iSUB, sfi_(sfpidx), sfi_(a), sfi_(b));
					return 1;
				case METHODN_opDiv:
					KNH_ASM(iDIV, sfi_(sfpidx), sfi_(a), sfi_(b));
					return 1;
				case METHODN_opMod:
					KNH_ASM(iMOD, sfi_(sfpidx), sfi_(a), sfi_(b));
					return 1;
				}
			}
			return 0;
		}
	} /* CLASS_Int */
	if(cid == CLASS_Float) {
		if(IS_OPSIM(mn)) {
			if(knh_StmtOP_checkConst(ctx, stmt, &mn, /*swap*/ 1)) {
				int a = TERMs_putLOCAL(ctx, stmt, 1, NNTYPE_Float, local + 1);
				knh_float_t b = TERMs_float(stmt, 2);
				switch(mn) {
				case METHODN_opAdd:
					KNH_ASM(fADDn, sfi_(sfpidx), sfi_(a), b);
					return 1;
				case METHODN_opMul:
					KNH_ASM(fMULn, sfi_(sfpidx), sfi_(a), b);
					return 1;
				case METHODN_opEq:
					KNH_ASM(fEQn, sfi_(sfpidx), sfi_(a), b);
					return 1;
				case METHODN_opNeq:
					KNH_ASM(fNEQn, sfi_(sfpidx), sfi_(a), b);
					return 1;
				case METHODN_opLt:
					KNH_ASM(fLTn, sfi_(sfpidx), sfi_(a), b);
					return 1;
				case METHODN_opLte:
					KNH_ASM(fLTEn, sfi_(sfpidx), sfi_(a), b);
					return 1;
				case METHODN_opGt:
					KNH_ASM(fGTn, sfi_(sfpidx), sfi_(a), b);
					return 1;
				case METHODN_opGte:
					KNH_ASM(fGTEn, sfi_(sfpidx), sfi_(a), b);
					return 1;
				}
				return 0;
			}
			else {
				int a = TERMs_putLOCAL(ctx, stmt, 1, NNTYPE_Float, local + 1);
				int b = TERMs_putLOCAL(ctx, stmt, 2, NNTYPE_Float, local + 2);
				switch(mn) {
				case METHODN_opAdd:
					KNH_ASM(fADD, sfi_(sfpidx), sfi_(a), sfi_(b));
					return 1;
				case METHODN_opMul:
					KNH_ASM(fMUL, sfi_(sfpidx), sfi_(a), sfi_(b));
					return 1;
				case METHODN_opEq:
					KNH_ASM(fEQ, sfi_(sfpidx), sfi_(a), sfi_(b));
					return 1;
				case METHODN_opNeq:
					KNH_ASM(fNEQ, sfi_(sfpidx), sfi_(a), sfi_(b));
					return 1;
				case METHODN_opLt:
					KNH_ASM(fLT, sfi_(sfpidx), sfi_(a), sfi_(b));
					return 1;
				case METHODN_opLte:
					KNH_ASM(fLTE, sfi_(sfpidx), sfi_(a), sfi_(b));
					return 1;
				case METHODN_opGt:
					KNH_ASM(fGT, sfi_(sfpidx), sfi_(a), sfi_(b));
					return 1;
				case METHODN_opGte:
					KNH_ASM(fGTE, sfi_(sfpidx), sfi_(a), sfi_(b));
					return 1;
				}
				return 0;
			}
		}
		else if(IS_OPASIM(mn)) {
			if(knh_StmtOP_checkConst(ctx, stmt, &mn, /*swap*/ 0)) {
				int a = TERMs_putLOCAL(ctx, stmt, 1, NNTYPE_Float, local + 1);
				knh_float_t b = TERMs_float(stmt, 2);
				switch(mn) {
				case METHODN_opSub:
					KNH_ASM(fSUBn, sfi_(sfpidx), sfi_(a), b);
					return 1;
				case METHODN_opDiv:
#ifndef KONOHA_ON_LKM
					if(b == 0.0) {
						b = 1.0;
						knh_Gamma_perror(ctx, KERR_ERRATA, _("divided by zero: 0.0 ==> 1.0"));
					}
#else
					if(b == 0) {
						b = 1;
						knh_Gamma_perror(ctx, KERR_ERRATA, _("divided by zero: 0.0 ==> 1.0"));
					}
#endif
					KNH_ASM(fDIVn, sfi_(sfpidx), sfi_(a), b);
					return 1;
				}
			}
			else {
				int a = TERMs_putLOCAL(ctx, stmt, 1, NNTYPE_Float, local + 1);
				int b = TERMs_putLOCAL(ctx, stmt, 2, NNTYPE_Float, local + 2);
				switch(mn) {
				case METHODN_opSub:
					KNH_ASM(fSUB, sfi_(sfpidx), sfi_(a), sfi_(b));
					return 1;
				case METHODN_opDiv:
					KNH_ASM(fDIV, sfi_(sfpidx), sfi_(a), sfi_(b));
					return 1;
				}
			}
			return 0;
		}
	} /* CLASS_Float */
	return 0;
}

/* ------------------------------------------------------------------------ */

static
int TERMs_isLOCAL(knh_Stmt_t *stmt, size_t n)
{
	knh_Token_t *tk = DP(stmt)->tokens[n];
	return (IS_Token(tk) && TT_(tk) == TT_LOCAL);
}

/* ------------------------------------------------------------------------ */

static
void knh_StmtCALL_asm(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	if(knh_StmtOP_asm(ctx, stmt, reqt, sfpidx)) return ;
	int local = ASML(sfpidx);
	knh_Method_t *mtd = DP(DP(stmt)->tokens[0])->mtd;
	knh_class_t cid = TERMs_getcid(stmt, 1);
	knh_class_t mtd_cid = IS_Method(mtd) ? DP(mtd)->cid : CLASS_Any;
	size_t i;
	/* UNTYPED CLOSURE */
	if(cid == CLASS_Closure && DP(mtd)->mn == METHODN_invoke) {
		DBG2_P("mtd_cid=%s, cid=%s", CLASSN(mtd_cid), CLASSN(cid));
		for(i = 1; i < DP(stmt)->size; i++) {
			TERMs_asm(ctx, stmt, i, TYPE_Any, local + i);
		}
		KNH_ASM(AINVOKE, sfi_(local), (knh_ushort_t)DP(stmt)->size/*, UP(mtd)*/);
		goto L_RTYPE;
	}
	/* INSTRUCTION */
	if(mtd_cid == CLASS_Array || mtd_cid == CLASS_IArray || mtd_cid == CLASS_FArray) {
		if(DP(mtd)->mn == METHODN_get) {
			int a = TERMs_putLOCAL(ctx, stmt, 1, NNTYPE_Array, local + 1);
			if(TERMs_isCONST(stmt, 2)) {
				knh_intptr_t n = (knh_intptr_t)TERMs_int(stmt, 2);
				if(mtd_cid == CLASS_Array) {
					KNH_ASM(ARYGETn, sfi_(sfpidx), sfi_(a), n);
				}
				else if(mtd_cid == CLASS_IArray) {
					KNH_ASM(iARYGETn, sfi_(sfpidx), sfi_(a), n);
				}
				else {
					KNH_ASM(fARYGETn, sfi_(sfpidx), sfi_(a), n);
				}
			}
			else {
				int an = TERMs_putLOCAL(ctx, stmt, 2, NNTYPE_Int, local + 2);
				if(mtd_cid == CLASS_Array) {
					KNH_ASM(ARYGET, sfi_(sfpidx), sfi_(a), sfi_(an));
				}
				else if(mtd_cid == CLASS_IArray) {
					KNH_ASM(iARYGET, sfi_(sfpidx), sfi_(a), sfi_(an));
				}
				else {
					KNH_ASM(fARYGET, sfi_(sfpidx), sfi_(a), sfi_(an));
				}
			}
			return;
		}
		if(DP(mtd)->mn == METHODN_set) {
			int a = TERMs_putLOCAL(ctx, stmt, 1, NNTYPE_Array, local + 1);
			knh_type_t ptype = knh_Method_ptype(ctx, mtd, cid, 1);
			knh_type_t vtype = TERMs_gettype(stmt, 3);
			int v = TERMs_putLOCAL(ctx, stmt, 3, ptype, local + 3);
			if(TERMs_isCONST(stmt, 2)) {
				knh_intptr_t n = (knh_intptr_t)TERMs_int(stmt, 2);
				if(mtd_cid == CLASS_Array) {
					KNH_ASM_BOX(ctx, TYPE_Any, vtype, sfi_(v));
					KNH_ASM(ARYSETn, sfi_(v), sfi_(a), n);
				}
				else if(mtd_cid == CLASS_IArray) {
					KNH_ASM(iARYSETn, sfi_(v), sfi_(a), n);
				}
				else {
					KNH_ASM(fARYSETn, sfi_(v), sfi_(a), n);
				}
			}
			else {
				int an = TERMs_putLOCAL(ctx, stmt, 2, NNTYPE_Int, local + 2);
				if(mtd_cid == CLASS_Array) {
					KNH_ASM_BOX(ctx, TYPE_Any, vtype, sfi_(v));
					KNH_ASM(ARYSET, sfi_(v), sfi_(a), sfi_(an));
				}
				else if(mtd_cid == CLASS_IArray) {
					KNH_ASM(iARYSET, sfi_(v), sfi_(a), sfi_(an));
				}
				else {
					KNH_ASM(fARYSET, sfi_(v), sfi_(a), sfi_(an));
				}
			}
			return;
		}
	}/* INSTRUCTION*/

	/* PEEPHOLE */
	if(mtd == DP(ctx->kc)->mtd) {
		for(i = 2; i < DP(stmt)->size; i++) {
			knh_type_t reqt2 = knh_Method_reqtTERMs(ctx, mtd, cid, stmt, i);
			TERMs_asm(ctx, stmt, i, reqt2, local + i);
		}
		KNH_ASM(RCALL, sfi_(local), (knh_ushort_t)DP(stmt)->size);
		goto L_RTYPE;
	}
	if(IS_Method(mtd) && knh_Method_isFinal(mtd) && TERMs_isLOCAL(stmt, 1)) {
		int a = DP(DP(stmt)->tokens[1])->index;
		knh_type_t reqt2 = knh_Method_reqtTERMs(ctx, mtd, cid, stmt, 1);
		knh_type_t vart2 = TERMs_gettype(stmt, 1);
		if(IS_NNTYPE(reqt2) && !IS_NNTYPE(vart2)) {
			KNH_ASM(CHKNUL, sfi_(a));
		}
		for(i = 2; i < DP(stmt)->size; i++) {
			reqt2 = knh_Method_reqtTERMs(ctx, mtd, cid, stmt, i);
			TERMs_asm(ctx, stmt, i, reqt2, local + i);
		}
		KNH_ASM(FCALL, sfi_(local), (knh_ushort_t)DP(stmt)->size, sfi_(a), mtd);
		goto L_RTYPE;
	}/*PEEPHOLE*/

	for(i = 1; i < DP(stmt)->size; i++) {
		knh_type_t reqt2 = knh_Method_reqtTERMs(ctx, mtd, cid, stmt, i);
		TERMs_asm(ctx, stmt, i, reqt2, local + i);
	}
	KNH_ASM_CALL(ctx, reqt, local, DP(stmt)->tokens[0], DP(stmt)->size - 2);

	L_RTYPE:;
	knh_type_t rtype = DP(stmt)->type;
	if(IS_ubxtype(rtype)) {
		nMOVL(ctx, local, sfpidx);
	}
	else {
		MOVL(ctx, local, sfpidx);
	}
}

/* ------------------------------------------------------------------------ */

static
knh_flag_t knh_StmtNEW_flag(Ctx *ctx, knh_Stmt_t *stmt)
{
	return 0;
}

static
void knh_StmtNEW_asm(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	int local = ASML(sfpidx);
	knh_Method_t *mtd = DP(DP(stmt)->tokens[0])->mtd;
	knh_class_t cid = DP(DP(stmt)->tokens[1])->cid;
	size_t i;
	for(i = 2; i < DP(stmt)->size; i++) {
		knh_type_t reqt = knh_Method_reqtTERMs(ctx, mtd, cid, stmt, i);
		TERMs_asm(ctx, stmt, i, reqt, local + i);
	}
	KNH_ASM_NEW(ctx, reqt, local, DP(stmt)->tokens[0],
			knh_StmtNEW_flag(ctx, stmt), cid, DP(stmt)->size - 2);
	MOVL(ctx, local, sfpidx);
}

static
void knh_StmtMAPCAST_asm(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	int local = ASML(sfpidx);
	knh_type_t srct = TERMs_gettype(stmt, 1);
	TERMs_asm(ctx, stmt, 1, srct, local);
	KNH_ASM_MAP(ctx, reqt, local, DP(stmt)->tokens[0], srct, knh_Stmt_isNNCAST(stmt));
	MOVL(ctx, local, sfpidx);
}

static
void knh_StmtMT_asm(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	int local = ASML(sfpidx);
	TERMs_asm(ctx, stmt, 1, TYPE_Any, local);
	knh_Token_t *tk = DP(stmt)->tokens[0];
	if(DP(stmt)->size == 2) {
		KNH_ASM(TOSTR, local, DP(tk)->mn);
	}
	else {
		knh_String_t *fmt = DP(DP(stmt)->tokens[2])->text;
		ASM_ASSERT(ctx, IS_String(fmt));
		KNH_ASM(TOSTRf, local, DP(tk)->mn, UP(fmt));
	}
	MOVL(ctx, local, sfpidx);
}

static
void knh_StmtALT_asm(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	knh_KLRInst_t* label = new_KLRInstLABEL(ctx);
	int local = ASML(sfpidx);
	int i, size = DP(stmt)->size;
	knh_type_t reqc = CLASS_type(reqt);
	for(i = 0; i < size - 1; i++) {
		TERMs_asm(ctx, stmt, i, reqc, local);
		TERMs_ASM_JIFNN(ctx, stmt, i, label);
	}
	KNH_ASM_LABEL(ctx, label);
	MOVL(ctx, local, sfpidx);
}

static
void knh_StmtOR_asm(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	int local = ASML(sfpidx);
	knh_KLRInst_t*  label = new_KLRInstLABEL(ctx);
	int i, size = DP(stmt)->size;
	for(i = 0; i < size; i++) {
		TERMs_asm(ctx, stmt, i, NNTYPE_Boolean, local);
		TERMs_ASM_JIFT(ctx, stmt, i, label);
	}
	KNH_ASM_LABEL(ctx, label);
	nMOVL(ctx, local, sfpidx);
}

static
void knh_StmtAND_asm(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	int local = ASML(sfpidx);
	knh_KLRInst_t*  label = new_KLRInstLABEL(ctx);
	int i, size = DP(stmt)->size;
	for(i = 0; i < size; i++) {
		TERMs_asm(ctx, stmt, i, NNTYPE_Boolean, local);
		TERMs_ASM_JIFF(ctx, stmt, i, label);
	}
	KNH_ASM_LABEL(ctx, label);
	nMOVL(ctx, local, sfpidx);
}

static
void knh_StmtTRI_asm(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	int local = ASML(sfpidx);
	knh_KLRInst_t*  lbelse = new_KLRInstLABEL(ctx);
	knh_KLRInst_t*  lbend  = new_KLRInstLABEL(ctx);
	TERMs_asm(ctx, stmt, 0, NNTYPE_Boolean, local);
	TERMs_ASM_JIFF(ctx, stmt, 0, lbelse);

	TERMs_asm(ctx, stmt, 1, reqt, local);
	KNH_ASM(JMP, TADDR lbend);

	/* else */
	KNH_ASM_LABEL(ctx, lbelse);
	TERMs_asm(ctx, stmt, 2, reqt, local);
	KNH_ASM_LABEL(ctx, lbend);

	MOVL(ctx, local, sfpidx);
}

static
void knh_StmtLET_asm(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	knh_Token_t *tkL = DP(stmt)->tokens[0];
	if(TT_(tkL) == TT_LOCAL) {
		TERMs_asm(ctx, stmt, 1, DP(tkL)->type, DP(tkL)->index);
		if(reqt != TYPE_void) {
			KNH_ASM_SMOV(ctx, reqt, sfpidx, tkL);
		}
	}
	else if(IS_Token(DP(stmt)->tokens[1])) {
		KNH_ASM_MOV(ctx, tkL, DP(stmt)->tokens[1]);
		if(reqt != TYPE_void) {
			KNH_ASM_SMOV(ctx, reqt, sfpidx, tkL);
		}
	}
	else {
		int local = ASML(sfpidx);
		knh_token_t tt = TT_(tkL); /* NOTE: tkL is reused inside stmt */
		knh_short_t index = DP(tkL)->index;
		TERMs_asm(ctx, stmt, 1, DP(tkL)->type, local);
		TT_(tkL) = tt;
		DP(tkL)->index = index;
		KNH_ASM_MOV(ctx, tkL, DP(stmt)->tokens[1]);
		if(reqt != TYPE_void && local != sfpidx) {
			KNH_ASM_SMOV(ctx, reqt, sfpidx, tkL);
		}
	}
}

///* ------------------------------------------------------------------------ */
//
//static
//void knh_Token_toLOCAL(Ctx *ctx, knh_Token_t *tk, knh_type_t type, int sfpidx)
//{
//	TT_(tk) = TT_LOCAL;
//	DP(tk)->index = (knh_short_t)sfpidx;
//	DP(tk)->type = type;
//}
//
///* ------------------------------------------------------------------------ */
//
//static
//knh_Token_t *knh_Stmt_toLOCAL(Ctx *ctx, knh_Stmt_t *stmt, int sfpidx, knh_type_t reqt, int level)
//{
////	DBG2_P("stt=%s, toLOCAL=%d", knh_stmt_tochar(SP(stmt)->stt), sfpidx);
//	size_t i;
//	for(i = 0; i < DP(stmt)->size; i++) {
//		if(IS_Token(DP(stmt)->tokens[i])) {
//			knh_Token_toLOCAL(ctx, DP(stmt)->tokens[i], reqt, sfpidx);
//			return DP(stmt)->tokens[i];
//		}
//		else {
//			knh_Token_t *tk = knh_Stmt_toLOCAL(ctx, DP(stmt)->stmts[i], reqt, sfpidx, 1);
//			if(tk != NULL) return tk;
//		}
//	}
//	if(level == 1) return NULL;
//	knh_Token_t *tk = new_TokenNULL(ctx, FL(stmt), reqt);
//	knh_Token_toLOCAL(ctx, tk, reqt, sfpidx);
//	return tk;
//}

/* ------------------------------------------------------------------------ */

static
knh_Token_t *knh_Term_toLOCAL(Ctx *ctx, Term *tm, knh_type_t type, int sfpidx)
{
	if(IS_Stmt(tm)) {
		size_t i;
		knh_Stmt_t *stmt = (knh_Stmt_t*)tm;
		for(i = 0; i < DP(stmt)->size; i++) {
			if(IS_Token(DP(stmt)->tokens[i])) {
				tm = DP(stmt)->terms[i];
				goto L_TOKEN;
			}
		}
		tm = (Term*)new_TokenNULL(ctx, FL(stmt), type);
	}
	L_TOKEN: {
		knh_Token_t *tk = (knh_Token_t*)tm;
		DBG2_ASSERT(IS_Token(tk));
		TT_(tk) = TT_LOCAL;
		DP(tk)->index = (knh_short_t)sfpidx;
		DP(tk)->type = type;
		return tk;
	}
}

/* ------------------------------------------------------------------------ */

void knh_StmtEXPR_asm(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx)
{
	KNH_ASSERT(IS_Stmt(stmt));
	switch(SP(stmt)->stt) {
	case STT_CALL:
	case STT_OP:
		knh_StmtCALL_asm(ctx, stmt, reqt, sfpidx);
		break;
	case STT_NEW:
		knh_StmtNEW_asm(ctx, stmt, reqt, sfpidx);
		break;
	case STT_MT:
		knh_StmtMT_asm(ctx, stmt, reqt, sfpidx);
		break;
	case STT_MAPCAST:
		knh_StmtMAPCAST_asm(ctx, stmt, reqt, sfpidx);
		break;
	case STT_ALT:
		knh_StmtALT_asm(ctx, stmt, reqt, sfpidx);
		break;
	case STT_AND:
		knh_StmtAND_asm(ctx, stmt, reqt, sfpidx);
		break;
	case STT_OR:
		knh_StmtOR_asm(ctx, stmt, reqt, sfpidx);
		break;
	case STT_TRI:
		knh_StmtTRI_asm(ctx, stmt, reqt, sfpidx);
		break;
	case STT_LET:
		knh_StmtLET_asm(ctx, stmt, reqt, sfpidx);
		break;
	default:
		KNH_ASM_PANIC(ctx, "unknown stt=%s", cSTT_(stmt));
	}
	KNH_ASM_NNBOX(ctx, reqt, DP(stmt)->type, sfpidx);
	if(IS_NNTYPE(reqt) && !IS_NNTYPE(DP(stmt)->type)) {
		KNH_ASM(CHKNUL, sfpidx);
	}
	if(knh_Stmt_isAutoReturn(stmt)) {
		KNH_ASM(RETa, 0/*dummy*/, sfpidx);
	}
}

/* ------------------------------------------------------------------------ */

static
void TERMs_asm(Ctx *ctx, knh_Stmt_t *stmt, size_t n, knh_type_t reqt, int sfpidx)
{
	if(IS_Token(DP(stmt)->tokens[n])) {
		knh_Token_t *tk = DP(stmt)->tokens[n];
		KNH_ASM_SMOV(ctx, reqt, sfpidx, tk);
		knh_Term_toLOCAL(ctx, TM(tk), reqt, sfpidx);

	}
	else {
		knh_StmtEXPR_asm(ctx, DP(stmt)->stmts[n], reqt, sfpidx);
		KNH_SETv(ctx, DP(stmt)->tokens[n], knh_Term_toLOCAL(ctx, DP(stmt)->terms[n], reqt, sfpidx));
	}
}

/* ======================================================================== */
/* [LABEL]  */

static
void knh_Gamma_pushLABEL(Ctx *ctx, knh_Stmt_t *stmt, knh_KLRInst_t *lbC, knh_KLRInst_t *lbB)
{
	knh_Object_t *tkL = KNH_NULL;
	if(IS_DictMap(DP(stmt)->metaDictMap)) {
		tkL = knh_DictMap_get(ctx, DP(stmt)->metaDictMap, TS_ATlabel);
	}
	knh_Array_add(ctx, DP(ctx->kc)->lstacks, tkL);
	knh_Array_add(ctx, DP(ctx->kc)->lstacks, UP(lbC));
	knh_Array_add(ctx, DP(ctx->kc)->lstacks, UP(lbB));
}

static
void knh_Gamma_popLABEL(Ctx *ctx)
{
	knh_Array_pop(ctx, DP(ctx->kc)->lstacks);
	knh_Array_pop(ctx, DP(ctx->kc)->lstacks);
	knh_Array_pop(ctx, DP(ctx->kc)->lstacks);
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
	DBG2_ASSERT(n < DP(stmt)->size);
	knh_Token_t *tkIT = DP(stmt)->tokens[n];
	DBG2_ASSERT(IS_Token(tkIT));
	DBG2_ASSERT(TT_(tkIT) == TT_LOCAL);
	return tkIT;
}

static void TERMs_asmBLOCK(Ctx *ctx, knh_Stmt_t *stmt, size_t n)
{
	ASM_ASSERT(ctx, IS_Stmt(DP(stmt)->stmts[n]));
	knh_Stmt_asmBLOCK(ctx, DP(stmt)->stmts[n],1);
}

static void knh_StmtIF_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	knh_KLRInst_t*  lbIF   = new_KLRInstLABEL(ctx);
	knh_KLRInst_t*  lbELSE = new_KLRInstLABEL(ctx);
	knh_KLRInst_t*  lbEND  = new_KLRInstLABEL(ctx);
	/* if */
	KNH_ASM_LABEL(ctx, lbIF);
	TERMs_ASM_JIFF(ctx, stmt, 0, lbELSE);
	/*then*/
	TERMs_asmBLOCK(ctx, stmt, 1);
	if(TERMs_isDONE(stmt, 2)) {
		/* PEEPHOLE this isn't a bug */
		KNH_ASM_LABEL(ctx, lbELSE);
		KNH_ASM_LABEL(ctx, lbEND);
		return ;
	}
	KNH_ASM(JMP, TADDR lbEND);
	/* else */
	KNH_ASM_LABEL(ctx, lbELSE);
	TERMs_asmBLOCK(ctx, stmt, 2);
	/* endif */
	KNH_ASM_LABEL(ctx, lbEND);
}

static
void knh_StmtSWITCH_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	knh_Stmt_t *stmtCASE;
	knh_Token_t *tkIT = knh_Stmt_getLOCAL(stmt, SWITCH_IT);
	knh_KLRInst_t* lbC = new_KLRInstLABEL(ctx);
	knh_KLRInst_t* lbB = new_KLRInstLABEL(ctx);
	knh_Gamma_pushLABEL(ctx, stmt, lbC, lbB);
	KNH_ASM_LABEL(ctx, lbC);

	TERMs_asm(ctx, stmt, 0, DP(tkIT)->type, sfi_(DP(tkIT)->index));
	stmtCASE = DP(stmt)->stmts[1];
	while(IS_Stmt(stmtCASE)) {
		if(SP(stmtCASE)->stt == STT_CASE) {
			if(!TERMs_isASIS(stmtCASE, 0)) {
				knh_KLRInst_t*  lbEND = new_KLRInstLABEL(ctx);
				TERMs_ASM_JIFF(ctx, stmtCASE, 0, lbEND);
				TERMs_asmBLOCK(ctx, stmtCASE, 1);
				KNH_ASM_LABEL(ctx, lbEND);
			}
		}
		stmtCASE = DP(stmtCASE)->next;
	}
	stmtCASE = DP(stmt)->stmts[1];
	while(IS_Stmt(stmtCASE)) {
		if(SP(stmtCASE)->stt == STT_CASE) {
			if(TERMs_isASIS(stmtCASE, 0)) {
				TERMs_asmBLOCK(ctx, stmtCASE, 1);
			}
		}
		stmtCASE = DP(stmtCASE)->next;
	}
	KNH_ASM_LABEL(ctx, lbB);
	knh_Gamma_popLABEL(ctx);
}

/* ------------------------------------------------------------------------ */

static
void KNH_ASM_JUMPLABEL(Ctx *ctx, knh_Stmt_t *stmt, int delta)
{
	knh_Gamma_t *kc = ctx->kc;
	size_t s = knh_Array_size(DP(kc)->lstacks);
	if(s == 0) {
		knh_Gamma_perror(ctx, KERR_ERROR, _("don't use %s HERE"), knh_stmt_tochar(SP(stmt)->stt));
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
			knh_bytes_t lname = __tobytes(DP(tkL)->text);
			for(i = s - 3; i >= 0; i -= 3) {
				knh_Token_t *tkSTACK = (knh_Token_t*)knh_Array_n(DP(kc)->lstacks, i);
				if(IS_NOTNULL(tkSTACK) && knh_String_equals(DP(tkSTACK)->text, lname)) {
					lbBLOCK = (knh_KLRInst_t*)knh_Array_n(DP(kc)->lstacks, i + delta);
					goto L_JUMP;
				}
			}
			knh_Gamma_perror(ctx, KERR_EWARN, _("undefined label: %s"), sToken(tkL));
		}
		lbBLOCK = (knh_KLRInst_t*)knh_Array_n(DP(kc)->lstacks, s - 3 + delta);
		L_JUMP:;
		KNH_ASM(JMP, TADDR lbBLOCK)
	}
}


static
void knh_StmtCONTINUE_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	KNH_ASM_JUMPLABEL(ctx, stmt, 1);
}

static
void knh_StmtBREAK_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	KNH_ASM_JUMPLABEL(ctx, stmt, 2);
}

static
void knh_StmtWHILE_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	knh_KLRInst_t* lbC = new_KLRInstLABEL(ctx);
	knh_KLRInst_t* lbB = new_KLRInstLABEL(ctx);
	knh_Gamma_pushLABEL(ctx, stmt, lbC, lbB);
	KNH_ASM_LABEL(ctx, lbC);
	if(!TERMs_isTRUE(stmt, 0)) {
		TERMs_ASM_JIFF_LOOP(ctx, stmt, 0, lbB);
	}
	TERMs_asmBLOCK(ctx, stmt, 1);
	KNH_ASM(JMP, TADDR lbC);
	KNH_ASM_LABEL(ctx, lbB);
	knh_Gamma_popLABEL(ctx);
}

static
void knh_StmtDO_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	knh_KLRInst_t* lbC = new_KLRInstLABEL(ctx);
	knh_KLRInst_t* lbB = new_KLRInstLABEL(ctx);
	knh_Gamma_pushLABEL(ctx, stmt, lbC, lbB);
	KNH_ASM_LABEL(ctx, lbC);
	TERMs_asmBLOCK(ctx, stmt, 0);
	TERMs_ASM_JIFF_LOOP(ctx, stmt, 1, lbB);
	KNH_ASM(JMP, TADDR lbC);
	KNH_ASM_LABEL(ctx, lbB);
	knh_Gamma_popLABEL(ctx);
}

static
void knh_StmtFOR_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	knh_KLRInst_t* lbC = new_KLRInstLABEL(ctx);
	knh_KLRInst_t* lbB = new_KLRInstLABEL(ctx);
	knh_KLRInst_t* lbREDO = new_KLRInstLABEL(ctx);
	knh_Gamma_pushLABEL(ctx, stmt, lbC, lbB);

	/* i = 1 part */
	TERMs_asmBLOCK(ctx, stmt, 0);
	KNH_ASM(JMP, TADDR lbREDO);

	/* i++ part */
	KNH_ASM_LABEL(ctx, lbC); /* CONTINUE */
	TERMs_asmBLOCK(ctx, stmt, 2);

	/* i < 10 part */
	KNH_ASM_LABEL(ctx, lbREDO);
	if(!TERMs_isTRUE(stmt, 1)) {
		TERMs_ASM_JIFF_LOOP(ctx, stmt, 1, lbB);
	}
	TERMs_asmBLOCK(ctx, stmt, FOR_loop);
	KNH_ASM(JMP, TADDR lbC);

	KNH_ASM_LABEL(ctx, lbB);
	knh_Gamma_popLABEL(ctx);
}

/* ------------------------------------------------------------------------ */

static knh_Token_t *new_TokenLOCAL(Ctx *ctx, knh_type_t type, int sfpidx)
{
	knh_Token_t *tk = new_Token(ctx, 0, 0, 0, TT_LOCAL);
	DP(tk)->index = (knh_short_t)sfpidx;
	DP(tk)->type = type;
	return tk;
}

/* ------------------------------------------------------------------------ */

void knh_StmtLETM_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	size_t i, size = DP(stmt)->size / 2;
	int local = DP(ctx->kc)->esp;
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
			DBG2_ASSERT(STT_(stmtSET) == STT_CALL);
			DBG2_ASSERT(TT_(DP(stmt)->tokens[n+1]) == TT_LOCAL);
			KNH_SETv(ctx, DP(stmtSET)->tokens[DP(stmtSET)->size - 1], DP(stmt)->tokens[n+1]);
			knh_StmtCALL_asm(ctx, stmtSET, TYPE_void, local + size);
		}
	}
}

/* ------------------------------------------------------------------------ */

static
void knh_StmtSEPARATOR_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	knh_Stmt_t *stmtDECL = DP(stmt)->stmts[0];
	knh_Token_t *tkIT = knh_Stmt_getLOCAL(stmt, SEPARATOR_IT);
	TERMs_asm(ctx, stmt, FOREACH_iter, DP(tkIT)->type, DP(tkIT)->index);
	while(IS_NOTNULL(stmtDECL)) {
		if(STT_(stmtDECL) == STT_LET) {
			knh_StmtLET_asm(ctx, stmtDECL, TYPE_void, 0);
		}
		stmtDECL = DP(stmtDECL)->next;
	}
}

/* ------------------------------------------------------------------------ */

static
int knh_Stmt_isSEPARATOR(knh_Stmt_t *stmt)
{
	return (IS_Stmt(DP(stmt)->next) && STT_(DP(stmt)->next) == STT_LET);
}

static
void knh_StmtFOREACH_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	knh_KLRInst_t* lbC = new_KLRInstLABEL(ctx);
	knh_KLRInst_t* lbB = new_KLRInstLABEL(ctx);
	knh_Gamma_pushLABEL(ctx, stmt, lbC, lbB);

	knh_Stmt_t *stmtDECL = DP(stmt)->stmts[0];
	knh_Token_t *tkITR = knh_Stmt_getLOCAL(stmt, FOREACH_ITR);
	knh_Token_t *tkN = NULL;
	if(knh_Stmt_isSEPARATOR(stmtDECL)) {
		tkN = knh_Stmt_getLOCAL(stmt, FOREACH_SPR);
	}
	else {
		tkN = DP(stmtDECL)->tokens[1];
	}

	TERMs_asm(ctx, stmt, FOREACH_iter, NNTYPE_Iterator, DP(tkITR)->index);
	KNH_ASM_LABEL(ctx, lbC);
	//DBG2_P("**(%d)ntype=%s%s (%d)itrtype=%s%s **", DP(tkn)->index, TYPEQN(DP(tkn)->type), DP(tkitr)->index, TYPEQN(DP(tkitr)->type));
	if(knh_Stmt_isSEPARATOR(stmtDECL)) {
		KNH_ASM(NEXT, TADDR lbB, sfi_(DP(tkN)->index), sfi_(DP(tkITR)->index));
		while(IS_NOTNULL(stmtDECL)) {
			if(STT_(stmtDECL) == STT_LET) {
				knh_StmtLET_asm(ctx, stmtDECL, TYPE_void, 0);
			}
			stmtDECL = DP(stmtDECL)->next;
		}
	}
	else {
		knh_class_t reqc = CLASS_type(DP(tkN)->type);
		knh_class_t foundc = CLASS_type(DP(tkITR)->type);
		DBG2_ASSERT_cid(foundc); foundc = ClassTable(foundc).p1;
		DBG2_ASSERT(reqc != CLASS_Tvar);
		if(reqc == foundc || reqc == CLASS_Any || knh_class_instanceof(ctx, reqc, foundc)) {
			KNH_ASM(NEXT, TADDR lbB, sfi_(DP(tkN)->index), sfi_(DP(tkITR)->index));
		}
		else if(!knh_Stmt_isMAPNEXT(stmt)) {
			KNH_ASM(INEXT, TADDR lbB, reqc, sfi_(DP(tkN)->index), sfi_(DP(tkITR)->index));
		}
		else {
//			knh_Mapper_t *mpr = knh_Class_getMapper(ctx, foundc, reqc);
//			if(IS_NOTNULL(mpr) && knh_Mapper_isFinal(mpr)) {
//				KNH_ASM(SMAPNEXT, lbend, sfi_(DP(tkN)->index), sfi_(DP(tkITR)->index), UP(mpr));
//			}
//			else {
//				KNH_ASM(MAPNEXT, lbend, reqc, sfi_(DP(tkN)->index), sfi_(DP(tkITR)->index));
//			}
		}
	}
	if(!TERMs_isTRUE(stmt, FOREACH_where)) {
		TERMs_ASM_JIFF(ctx, stmt, FOREACH_where, lbC);
	}
	TERMs_asmBLOCK(ctx, stmt, FOREACH_loop);
	KNH_ASM(JMP, TADDR lbC);
	/* end */
	KNH_ASM_LABEL(ctx, lbB);
	knh_Gamma_popLABEL(ctx);
}

/* ======================================================================== */
/* [TRY] */

static int knh_Gamma_inTry(Ctx *ctx)
{
	return IS_Stmt(DP(ctx->kc)->finallyStmt);
}

/* ------------------------------------------------------------------------ */

static
void knh_Gamma_setFinallyStmt(Ctx *ctx, knh_Stmt_t *stmt)
{
	knh_Gamma_t *kc = ctx->kc;
	if(IS_NOTNULL(stmt)) {
		if(IS_NOTNULL(DP(kc)->finallyStmt)) {
			knh_Gamma_perror(ctx, KERR_ERROR, _("cannot use nested try statements"));
			return;
		}
		KNH_SETv(ctx, DP(kc)->finallyStmt, stmt);
	}
	else { /* stmt == null */
		KNH_SETv(ctx, DP(kc)->finallyStmt, stmt);
	}
}

/* ------------------------------------------------------------------------ */

static
void KNH_ASM_FINALLY(Ctx *ctx)
{
	if(IS_NOTNULL(DP(ctx->kc)->finallyStmt)) {
		knh_Stmt_asmBLOCK(ctx, DP(ctx->kc)->finallyStmt, 1/* Iteration */);
	}
}

/* ------------------------------------------------------------------------ */

static
void knh_StmtTRY_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	knh_KLRInst_t*  lbCATCH   = new_KLRInstLABEL(ctx);
	knh_KLRInst_t*  lbFINALLY = new_KLRInstLABEL(ctx);
	knh_Token_t *tkIT = knh_Stmt_getLOCAL(stmt, TRY_HDR);

	knh_Gamma_setFinallyStmt(ctx, StmtTRY_finally(stmt));

	/* try { */
	KNH_ASM(TRY, TADDR lbCATCH, sfi_(DP(tkIT)->index));
	TERMs_asmBLOCK(ctx, stmt, TRY_try);
	KNH_ASM(TRYEND, sfi_(DP(tkIT)->index));
	KNH_ASM(JMP, TADDR lbFINALLY);
	knh_Gamma_setFinallyStmt(ctx, (knh_Stmt_t*)KNH_NULL); // InTry

	/* catch */
	KNH_ASM_LABEL(ctx, lbCATCH);
	knh_Stmt_t *stmtCATCH = DP(stmt)->stmts[TRY_catch];
	KNH_ASSERT(IS_Stmt(stmtCATCH));
	while(IS_Stmt(stmtCATCH)) {
		if(SP(stmtCATCH)->stt == STT_CATCH) {
			knh_String_t *emsg = DP(DP(stmtCATCH)->tokens[0])->text;
			knh_Token_t *tkn = DP(stmtCATCH)->tokens[1];
			DBG2_ASSERT(IS_String(emsg));
			DBG2_ASSERT(TT_(tkn) == TT_LOCAL);
			lbCATCH = new_KLRInstLABEL(ctx);
			KNH_ASM(CATCH, TADDR lbCATCH, sfi_(DP(tkIT)->index), sfi_(DP(tkn)->index), emsg);
			TERMs_asmBLOCK(ctx, stmtCATCH, 2);
			KNH_ASM(JMP, TADDR lbFINALLY);  /* GOTO FINALLY */
			KNH_ASM_LABEL(ctx, lbCATCH); /* _CATCH_NEXT_ */
		}
		stmtCATCH = DP(stmtCATCH)->next;
	}
	KNH_ASM_LABEL(ctx, lbFINALLY); /* FINALLY */
	TERMs_asmBLOCK(ctx, stmt, TRY_finally);
	KNH_ASM(THROW_AGAIN, sfi_(DP(tkIT)->index));
	KNH_ASM(MOVo, sfi_(DP(tkIT)->index), KNH_NULL);
}

/* ------------------------------------------------------------------------ */

static
void knh_StmtTHROW_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	//KNH_ASM_HOOKED_FINALLY(ctx);
	TERMs_ASM_THROW(ctx, stmt, 0);
}


/* ------------------------------------------------------------------------ */

static
void knh_StmtRETURN_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	knh_Gamma_t *kc = ctx->kc;
	KNH_ASM_FINALLY(ctx);
	if(DP(stmt)->size > 0) {
		knh_type_t rtype = TERMs_gettype(stmt, 0);
		if(rtype == TYPE_void) {
			// @see(konohac.c new Eval)
			TERMs_asm(ctx, stmt, 0, DP(kc)->rtype, DP(ctx->kc)->esp);
			knh_Stmt_setVOID(stmt, 1);
		}
		else {
			TERMs_asm(ctx, stmt, 0, DP(kc)->rtype, -1);
		}
	}
	KNH_ASM_RET(ctx);
}

/* ------------------------------------------------------------------------ */

static
void knh_StmtERR_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	if(!IS_bString(DP(stmt)->errMsg)) {
		char buf[512];
		knh_snprintf(buf, sizeof(buf), "Script!!: you'll fix bugs at %s:%d", FILEN(SP(stmt)->uri), SP(stmt)->line);
		KNH_SETv(ctx, DP(stmt)->errMsg, new_String(ctx, B(buf), NULL));
		KNH_SETv(ctx, DP(stmt)->next, KNH_NULL);
	}
	KNH_ASM(THROWs, knh_Gamma_inTry(ctx), DP(stmt)->errMsg);
}

/* ======================================================================== */
/* [PRINT] */

static int knh_Gamma_isDEBUG(Ctx *ctx)
{
	if(knh_Method_isDebug(DP(ctx->kc)->mtd)) return 1;
	return knh_Context_isDebug(ctx);
}

static void KNH_ASM_SKIP(Ctx *ctx, knh_KLRInst_t*  lbskip)
{
	if(!knh_Method_isDebug(DP(ctx->kc)->mtd)) {
		KNH_ASM(SKIP, TADDR lbskip);
	}
}


static knh_methodn_t knh_Stmt_getMT(Ctx *ctx, knh_Stmt_t *stmt, size_t n)
{
	knh_methodn_t mn = METHODN__k;
	knh_Stmt_t *o = DP(stmt)->stmts[n];
	if(IS_Stmt(o) && SP(o)->stt == STT_MT) {
		KNH_ASSERT(DP(o)->size > 1);
		mn = DP(DP(o)->tokens[0])->mn;
		KNH_SETv(ctx, DP(stmt)->terms[n], DP(o)->terms[1]);
	}
	return mn;
}

static void knh_StmtPRINT_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	if(!knh_Gamma_isDEBUG(ctx) || DP(stmt)->size == 0) {
		return ;
	}
	knh_flag_t flag = knh_StmtPRINT_flag(ctx, stmt);
	knh_KLRInst_t*  lbskip = new_KLRInstLABEL(ctx);
	KNH_ASM_SKIP(ctx, lbskip);
	if(konoha_debugLevel() > 1) {
		char buf[128];
		knh_snprintf(buf, sizeof(buf), "[%s:%d]", FILEN(SP(ctx->kc)->uri), SP(ctx->kc)->line);
		KNH_ASM(PMSG, flag | KNH_FLAG_PF_BOL, UP(new_String(ctx, B(buf), NULL)));
	}
	else if(flag != 0 ) {
		KNH_ASM(PMSG, flag | KNH_FLAG_PF_BOL, UP(TS_EMPTY));
	}
	int i;
	for(i = 0; i < DP(stmt)->size; i++) {
		L_REDO:;
		knh_flag_t mask = 0;
		if(i == DP(stmt)->size - 1) {
			mask |= KNH_FLAG_PF_EOL;
		}
		/* name= */ {
			knh_Token_t *tkn = DP(stmt)->tokens[i];
			if(IS_Token(tkn) && knh_Token_isPNAME(tkn)) {
				KNH_ASM(PMSG, flag | mask | KNH_FLAG_PF_NAME, DP(tkn)->data);
				i++;
				KNH_ASSERT(i < DP(stmt)->size);
				goto L_REDO;
			}
		}
		/* "literal"*/ {
			knh_Token_t *tkn = DP(stmt)->tokens[i];
			if(IS_Token(tkn) && knh_Token_isCONST(tkn) && IS_String(DP(tkn)->data)) {
				KNH_ASM(PMSG, flag | mask, DP(tkn)->data);
				continue;
			}
		}
		TERMs_asm(ctx, stmt, i, TYPE_Any, DP(ctx->kc)->esp);
		knh_methodn_t mn = knh_Stmt_getMT(ctx, stmt, i);
		KNH_ASM(P, flag | mask, mn, DP(ctx->kc)->esp);
	}
	KNH_ASM_LABEL(ctx, lbskip);
}

static void knh_StmtASSERT_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	knh_Gamma_t *kc = ctx->kc;
	if(DP(kc)->testidx == -1) {
		int isRelease = knh_StmtMETA_is(ctx, stmt, STEXT("@Release"));
		if(!isRelease) {
			if(!knh_Gamma_isDEBUG(ctx)) {
				return ;
			}
		}
		knh_KLRInst_t* lbskip = new_KLRInstLABEL(ctx);
		if(!isRelease) {
			KNH_ASM(SKIP, TADDR lbskip);
		}
		/* if */
		TERMs_ASM_JIFT(ctx, stmt, 0, lbskip);
		/*then*/
		TERMs_asmBLOCK(ctx, stmt, 1);
		KNH_ASM(THROWs, knh_Gamma_inTry(ctx), knh_getExptName(ctx, EXPT_Assertion));
		KNH_ASM_LABEL(ctx, lbskip);
	}
	else {
		int esp = DP(kc)->esp;
		knh_Method_t *mtd = knh_Class_getMethod(ctx, CLASS_System, METHODN_test);
		DBG2_ASSERT(IS_Method(mtd));
		DBG2_ASSERT(DP(kc)->testidx < esp);
		TERMs_putLOCAL(ctx, stmt, 0, NNTYPE_Boolean, esp+2);
		KNH_ASM(MOVa, esp+3, DP(kc)->testidx);
		KNH_ASM(MOVi, esp+1, (knh_int_t)SP(stmt)->line);
		KNH_ASM(SCALL, esp, 4, mtd);
	}
}

static void knh_StmtTEST_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	knh_Gamma_t *kc = ctx->kc;
	knh_Token_t *tkIT = knh_Stmt_getLOCAL(stmt, TEST_IT);
	int testidx = DP(kc)->testidx;
	DP(kc)->testidx = DP(tkIT)->index;
	TERMs_putLOCAL(ctx, stmt, 0, NNTYPE_String, DP(tkIT)->index);
	TERMs_asmBLOCK(ctx, stmt, 1);
	DP(kc)->testidx = testidx;
}

/* ------------------------------------------------------------------------ */

void knh_Stmt_asmBLOCK(Ctx *ctx, knh_Stmt_t *stmt, int isIteration)
{
	knh_Gamma_t *kc = ctx->kc;
	knh_Stmt_t *cur = stmt;
	while(IS_Stmt(cur)) {
		SP(kc)->line = SP(cur)->line;
		DP(kc)->esp = DP(stmt)->esp;
		switch(SP(cur)->stt) {
		case STT_BLOCK:
			knh_Stmt_asmBLOCK(ctx, DP(cur)->stmts[0], 1); break;
		case STT_LET:
			knh_StmtLET_asm(ctx, cur, TYPE_void, DP(ctx->kc)->esp); break;
		case STT_LETM:
			knh_StmtLETM_asm(ctx, cur); break;
		case STT_SEPARATOR:
			knh_StmtSEPARATOR_asm(ctx, cur); break;
		case STT_REGISTER:
		{
			size_t i;
			for(i = 0; i < DP(cur)->size; i++) {
				if(IS_Stmt(DP(cur)->terms[i]) && STT_(DP(cur)->stmts[i]) == STT_LET) {
					knh_StmtLET_asm(ctx, DP(cur)->stmts[i], TYPE_void, DP(ctx->kc)->esp);
				}
			}
			break;
		}
		case STT_IF:
			knh_StmtIF_asm(ctx, cur); break;
		case STT_SWITCH:
			knh_StmtSWITCH_asm(ctx, cur); break;
		case STT_WHILE:
			knh_StmtWHILE_asm(ctx, cur); break;
		case STT_DO:
			knh_StmtDO_asm(ctx, cur); break;
		case STT_FOR:
			knh_StmtFOR_asm(ctx, cur); break;
		case STT_FOREACH:
			knh_StmtFOREACH_asm(ctx, cur); break;
		case STT_BREAK:
			knh_StmtBREAK_asm(ctx, cur); break;
		case STT_CONTINUE:
			knh_StmtCONTINUE_asm(ctx, cur); break;
		case STT_TRY:
			knh_StmtTRY_asm(ctx, cur); break;
		case STT_THROW:
			knh_StmtTHROW_asm(ctx, cur); break;
		case STT_RETURN:
			knh_StmtRETURN_asm(ctx, cur); break;
		case STT_PRINT:
			knh_StmtPRINT_asm(ctx, cur); break;
		case STT_ASSERT:
			knh_StmtASSERT_asm(ctx, cur); break;
		case STT_TEST:
			knh_StmtTEST_asm(ctx, cur); break;
		case STT_ERR:
			knh_StmtERR_asm(ctx, cur); break;
		case STT_DECL:
		case STT_CALL1:
		case STT_DONE:
			break;
		default:
			if(knh_stmt_isExpr(SP(cur)->stt)) {
				knh_StmtEXPR_asm(ctx, cur, TYPE_void, DP(ctx->kc)->esp);
			}
			else {
				KNH_ASM_PANIC(ctx, "stt=%s", cSTT_(cur));
			}
		}
		if(!isIteration) break;
		cur = DP(cur)->next;
	}
}

/* ------------------------------------------------------------------------ */

static void KNH_ASM_INITLOCAL(Ctx *ctx)
{
	knh_Gamma_t *kc = ctx->kc;
	knh_Method_t *mtd = DP(kc)->mtd;
	size_t i, xi;
	KNH_ASM(THCODE, sfi_(DP(kc)->psize));
	DBG2_ASSERT(IS_Method(mtd));

	for(i = 1;i < knh_Method_psize(mtd) + 1; i++) {
		xi = i + DP(kc)->goffset;
		knh_type_t ztype = knh_Method_pztype(mtd, i - 1);
		knh_type_t ptype = DP(kc)->gamma[xi].type;
		DBG2_P("PARAM TYPE %s%s (%s%s) i=%ld, xi=%ld %s", TYPEQN(ztype), TYPEQN(ptype), i, xi, FIELDN(DP(kc)->gamma[xi].fn));
		DBG2_ASSERT(CLASS_type(ztype) == CLASS_type(ptype));
		if(IS_NATYPE(ztype)) {
			Object *value = DP(kc)->gamma[xi].value;
			if(value == NULL) {
				KNH_ASM(PARAMDEF, sfi_(i), CLASS_type(ptype));
			}
			else {
				KNH_ASM(PARAMo, sfi_(i), value);
			}
		}
	}

	xi = i + DP(kc)->goffset;
	if(DP(kc)->gamma[xi].fn == FIELDN_vargs) {
		knh_class_t cid = ClassTable(CLASS_type(DP(kc)->gamma[xi].type)).p1;
		DBG2_ASSERT_cid(cid);
		KNH_ASM(PARAMS, i, cid); i++;
	}

	if(DP(kc)->globalidx != -1) {
		knh_Object_t *scr = (knh_Object_t*)knh_getGammaScript(ctx);
		KNH_ASM(MOVo, DP(kc)->globalidx, scr);
	}

	for(i=0; i < knh_Array_size(DP(kc)->decls); i++) {
		knh_Stmt_t *stmt = (knh_Stmt_t*)knh_Array_n(DP(kc)->decls, i);
		knh_Stmt_asmBLOCK(ctx, stmt, 1);
	}

}

/* ------------------------------------------------------------------------ */

void KNH_ASM_METHOD(Ctx *ctx, knh_Method_t *mtd, knh_Stmt_t *params, knh_Stmt_t *body, int isIteration)
{
	knh_Gamma_t *kc = ctx->kc;
	knh_class_t prev_cid = DP(kc)->this_cid;
	knh_StmtMETHOD_typingBODY(ctx, mtd, params, body, isIteration);
	if(STT_(body) == STT_ERR) return ;

	DP(kc)->scope = SCOPE_LOCAL;
	SP(kc)->line = SP(body)->line;
	KNH_ASM_INITLOCAL(ctx);
	knh_Stmt_asmBLOCK(ctx, body, isIteration);
	if(params == NULL) {
		KNH_ASM_RET(ctx);
	}

	knh_Gamma_finish(ctx);
	DP(kc)->this_cid = prev_cid;
}

/* ------------------------------------------------------------------------ */

void knh_StmtMETHOD_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	if(DP(stmt)->size == 4) return;
	knh_Method_t *mtd = DP(StmtMETHOD_method(stmt))->mtd;
	KNH_ASM_METHOD(ctx, mtd, DP(stmt)->stmts[3], StmtMETHOD_instmt(stmt), 1 /*Iteration */);
	knh_invokeMethodCompilationListener(ctx, DP(stmt)->metaDictMap, mtd);
}

/* ------------------------------------------------------------------------ */

void knh_StmtFORMAT_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	TODO();
}

/* ------------------------------------------------------------------------ */

void knh_StmtCLASS_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	knh_Gamma_t *kc = ctx->kc;
	knh_class_t prev_cid = DP(kc)->this_cid;
	knh_class_t this_cid = DP(StmtCLASS_class(stmt))->cid;
	DP(kc)->this_cid = this_cid;

	knh_Stmt_t *stmtFIELD = StmtCLASS_instmt(stmt);
	while(IS_Stmt(stmtFIELD)) {
		if(SP(stmtFIELD)->stt == STT_METHOD) {
			SP(kc)->line = SP(stmtFIELD)->line;
			knh_StmtMETHOD_asm(ctx, stmtFIELD);
		}
		else if(SP(stmtFIELD)->stt == STT_FORMAT) {
			SP(kc)->line = SP(stmtFIELD)->line;
			knh_StmtFORMAT_asm(ctx, stmtFIELD);
		}
		stmtFIELD = DP(stmtFIELD)->next;
	}
	DP(kc)->this_cid = prev_cid;
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
