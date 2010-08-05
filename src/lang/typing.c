/****************************************************************************
 * KONOHA COPYRIGHT, LICENSE NOTICE, AND DISCRIMER
 *
 * Copyright (c) 2006-2010, Kimio Kuramitsu <kimio at ynu.ac.jp>
 *           (c) 2008-      Konoha Team konohaken@googlegroups.com
 * All rights reserved.
 *
 * You may choose one of the following two licenses when you use konoha.
 * See www.konohaware.org/license.html for further information.

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
#define USE_bytes_endsWith    1
#define USE_bytes_parseint    1
#define USE_bytes_parsefloat  1

#include"commons.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ======================================================================== */

static knh_Term_t *knh_Stmt_typing(Ctx *ctx, knh_Stmt_t *stmtITR, knh_type_t reqt);
static knh_Term_t *knh_StmtCALL_typing(Ctx *ctx, knh_Stmt_t *stmtITR, knh_class_t reqt);
static int TERMs_typing(Ctx *ctx, knh_Stmt_t *stmt, size_t n, knh_type_t reqt, knh_flag_t modeflag);

#define BEGIN_BLOCK(X) \
	size_t X = DP(ctx->gma)->espidx;\
	DBG_ASSERT(DP(ctx->gma)->gf[X].fn == FN_NONAME);\

#define END_BLOCK(X) \
	DBG_P("ESP %s start=%d, end=%d, stmt->espidx=%d", TT__(STT_(stmt)), X, DP(ctx->gma)->espidx, DP(stmt)->espidx);\
	DBG_ASSERT(DP(ctx->gma)->gf[DP(ctx->gma)->espidx].fn == FN_NONAME);\
	if(X != (size_t)DP(ctx->gma)->espidx) {\
		DP(stmt)->espidx = DP(ctx->gma)->espidx; \
		DBG_P("*** stt=%s release espidx=%d ***", TT__(STT_(stmt)), (int)X);\
		knh_Gamma_clear(ctx, X);\
	}\

#define CONSTPOOL(v)   knh_getConstPools(ctx, v)

#define _FREADONLY   FLAG_Field_ReadOnly
#define _FGETTER     FLAG_Field_Getter
#define _FSETTER     FLAG_Field_Setter
#define _FKEY        FLAG_Field_Key

#define IS_SCRIPTLEVEL(ctx)       IS_NULL(DP(ctx->gma)->mtd)

static void knh_Stmt_toSTT(knh_Stmt_t *stmt, knh_term_t stt)
{
	DBG_P("SWITCH %s ==> %s", TT__(STT_(stmt)), TT__(stt));
	STT_(stmt) = stt;
}

/* ------------------------------------------------------------------------ */

void knh_Gamma_perror(Ctx *ctx, int pe, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	knh_vperror(ctx, SP(ctx->gma)->uri, (int)SP(ctx->gma)->line, pe, fmt, ap);
	va_end(ap);
}

/* ------------------------------------------------------------------------ */
/* [TypeVariable] */

#define TYPEINF()    TODO()

static knh_type_t GMA_newTVAR(Ctx *ctx)
{
	return TYPE_Any;
}

static int GMA_type(Ctx *ctx, knh_type_t type)
{
	if(type < TYPE_T0) {
		return type;
	}
	else {
		return DP(ctx->gma)->typevars[type - TYPE_T0].type;
	}
}

static int GMA_isTVAR(Ctx *ctx, knh_type_t type)
{
	if(type < TYPE_T0) return 0;
	type = DP(ctx->gma)->typevars[type - TYPE_T0].type;
	if(type == TYPE_var || type == TYPE_Any) return 1;
	return 0;
}

static int GMA_typingTVAR(Ctx *ctx, knh_type_t type, knh_type_t reqt)
{
	DBG_ASSERT(!(type < TYPE_T0));
	if(GMA_isTVAR(ctx, reqt)) {

	}
	return 1;
}

static knh_Term_t *knh_Stmt_typed(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t type)
{
	SP(stmt)->type = type;
	return TM(stmt);
}

/* ------------------------------------------------------------------------ */

static knh_class_t knh_class_FuncType(Ctx *ctx, knh_class_t this_cid, knh_Method_t *mtd)
{
	knh_class_t cid;
	knh_ParamArray_t *pa = DP(mtd)->mp;
	if(knh_ParamArray_hasTypeVar(pa)) {
		BEGIN_LOCAL(ctx, lsfp, 1);
		knh_ParamArray_t *npa = new_ParamArray(ctx);
		KNH_SETv(ctx, lsfp[0].o, npa);
		knh_ParamArray_tocid(ctx, pa, this_cid, npa);
		cid = knh_class_Generics(ctx, CLASS_Func, npa);
		END_LOCAL(ctx, lsfp);
	}
	else {
		cid = knh_class_Generics(ctx, CLASS_Func, pa);
	}
	return cid;
}

static knh_Func_t * new_StaticFunc(Ctx *ctx, knh_class_t cid, knh_Method_t *mtd)
{
	knh_Func_t *fo = new_H(Func);
	fo->h.cid = cid;
	KNH_INITv(fo->mtd, mtd);
	fo->baseNULL = NULL;
	fo->reserved = NULL;
	return fo;
}

/* ------------------------------------------------------------------------ */
/* [Token] */

static knh_Term_t* knh_Token_toCONST(Ctx *ctx, knh_Token_t *tk)
{
	DBG_ASSERT(IS_Token(tk));
	TT_(tk) = TT_CONST;
	SP(tk)->type = knh_Object_cid(DP(tk)->data);
	return (knh_Term_t*)tk;
}

static knh_Term_t* knh_Token_setCONST_(Ctx *ctx, knh_Token_t *tk, Any *data)
{
	KNH_SETv(ctx, DP(tk)->data, data);
	return knh_Token_toCONST(ctx, tk);
}

static knh_Token_t* new_TokenCONST_(Ctx *ctx, Any *data)
{
	knh_Token_t *tk = new_(Token);
	knh_Token_setCONST_(ctx, tk, data);
	return tk;
}

knh_Term_t* knh_Token_toTYPED(Ctx *ctx, knh_Token_t *tk, knh_term_t tt, knh_type_t type, knh_short_t nn)
{
	TK_typing(tk, tt, type, nn);
	if(tt == TT_FIELD) {knh_Gamma_foundFIELD(ctx->gma, 1); }
	return TM(tk);
}

knh_Token_t* new_TokenTYPED(Ctx *ctx, knh_term_t tt, knh_type_t type, knh_short_t nn)
{
	knh_Token_t *tk = new_(Token);
	knh_Token_toTYPED(ctx, tk, tt, type, nn);
	return tk;
}

/* ------------------------------------------------------------------------ */

knh_bool_t knh_StmtMETA_is(Ctx *ctx, knh_Stmt_t *stmt, knh_bytes_t name)
{
	if(IS_Map(DP(stmt)->metaDictCaseMap)) {
		Object *v = knh_DictCaseMap_getNULL(ctx, DP(stmt)->metaDictCaseMap, name);
		if(v != NULL) return 1;
	}
	return 0;
}

#define knh_Stmt_insert(ctx, stmt, n, tm)   knh_Stmt_insert_(ctx, stmt, n, TM(tm))

static void knh_Stmt_insert_(Ctx *ctx, knh_Stmt_t *stmt, size_t n, knh_Term_t *tm)
{
	size_t i = DP(stmt)->size;
	DBG_ASSERT(n <= DP(stmt)->size);
	knh_Stmt_add_(ctx, stmt, tm, NULL); // DP(o)->size++;
	for(; n < i ; i--) {
		knh_Stmt_swap(ctx, stmt, i - 1, i);
	}
}

static knh_fieldn_t knh_Token_getfnq(Ctx *ctx, knh_Token_t *tk)
{
	knh_fieldn_t fn = FN_NONAME;
	if(TT_(tk) == TT_NAME || TT_(tk) == TT_UNAME) {
		fn = knh_getfnq(ctx, TK_tobytes(tk), FN_NEWID);
	}
	return fn;
}

static knh_methodn_t knh_Token_getmn(Ctx *ctx, knh_Token_t *tk)
{
	if(TT_(tk) == TT_FUNCNAME || TT_(tk) == TT_NAME || TT_(tk) == TT_UNAME) {
		TT_(tk) = TT_MN;
		DP(tk)->mn = knh_getmn(ctx, TK_tobytes(tk), MN_NEWID);
	}
	if(TT_(tk) == TT_NEW) {
		TT_(tk) = TT_MN;
		DP(tk)->mn = knh_getmn(ctx, TK_tobytes(tk), MN_NEWID);
	}
	DBG_ASSERT(TT_(tk) == TT_MN);
	if(knh_Token_isGetter(tk)) {
		DP(tk)->mn = MN_toGETTER(MN_toFN(DP(tk)->mn));
		knh_Token_setGetter(tk, 0);
	}
	if(knh_Token_isSetter(tk)) {
		DP(tk)->mn = MN_toSETTER(MN_toFN(DP(tk)->mn));
		knh_Token_setSetter(tk, 0);
	}
	return DP(tk)->mn;
}

static knh_Term_t *knh_TokenTYPE_typing(Ctx *ctx, knh_Token_t *tk, knh_type_t reqt);

knh_class_t knh_Token_getcid(Ctx *ctx, knh_Token_t *tk, knh_class_t defc)
{
	knh_class_t cid = CLASS_unknown;
	if(knh_Token_isExceptionType(tk)) {
		cid = CLASS_Exception;
	}
	else if(TT_(tk) == TT_CID) {
		cid = DP(tk)->cid;
	}
	else if(TT_(tk) == TT_VAR) {
		cid = TYPE_var;
	}
	else if(TT_(tk) == TT_VOID) {
		cid = TYPE_void;
	}
	else if(TT_(tk) == TT_UNAME || TT_(tk) == TT_FUNCNAME) {
		knh_NameSpace_t *ns = DP(ctx->gma)->ns;
		cid = knh_NameSpace_getcid(ctx, ns, TK_tobytes(tk));
		if(cid == CLASS_unknown && defc != CLASS_unknown) {
			cid = defc;
			knh_Gamma_perror(ctx, KERR_ERRATA, _("unknown class: %L ==> %C"), tk, defc);
		}
	}
	else {
		DBG_ASSERT(TT_(tk) == TT_TYPE);
		knh_Term_t *tm = knh_TokenTYPE_typing(ctx, tk, TYPE_Any);
		if(tm == NULL && defc != CLASS_unknown) {
			cid = defc;
			knh_Gamma_perror(ctx, KERR_ERRATA, _("unknown class: %L ==> %C"), tk, defc);
		}
		else {
			DBG_ASSERT(TT_(tk) == TT_CID);
			return DP(tk)->cid;
		}
	}
	if(cid != CLASS_unknown) {
		TK_typing(tk, TT_CID, TYPE_Class, cid);
	}
	return cid;
}

/* ------------------------------------------------------------------------ */
/* [VARIABLE] */
/* ------------------------------------------------------------------------ */

#define IS_SYSVAL(t,v)  (knh_bytes_strcasecmp(t, STEXT(v)) == 0)

static int knh_Token_toSYSVAL(Ctx *ctx, knh_Token_t *tk)
{
	knh_bytes_t t = TK_tobytes(tk);
	if(IS_SYSVAL(t, "CTX")) {
		TK_typing(tk, TT_SYSVAL, TYPE_Context, KNH_SYS_CTX);
	}
	else if(IS_SYSVAL(t, "IN")) {
		TK_typing(tk, TT_SYSVAL, TYPE_InputStream, KNH_SYS_CTXIN);
	}
	else if(IS_SYSVAL(t, "OUT")) {
		if(knh_Gamma_isCompilingFormatter(ctx)) {
			TK_typing(tk, TT_LOCAL, TYPE_OutputStream, 0);
		}
		else {
			TK_typing(tk, TT_SYSVAL, TYPE_OutputStream, KNH_SYS_CTXOUT);
		}
	}
	else if(IS_SYSVAL(t, "ERR")) {
		TK_typing(tk, TT_SYSVAL, TYPE_OutputStream, KNH_SYS_CTXERR);
	}
	else if(IS_SYSVAL(t, "STDIN")) {
		TK_typing(tk, TT_SYSVAL, TYPE_InputStream, KNH_SYS_STDIN);
	}
	else if(IS_SYSVAL(t, "STDOUT")) {
		TK_typing(tk, TT_SYSVAL, TYPE_OutputStream, KNH_SYS_STDOUT);
	}
	else if(IS_SYSVAL(t, "STDERR")) {
		TK_typing(tk, TT_SYSVAL, TYPE_OutputStream, KNH_SYS_STDERR);
	}
	else if(IS_SYSVAL(t, "OS")) {
		TK_typing(tk, TT_SYSVAL, TYPE_System, KNH_SYS_OS);
	}
	else if(ISB(t, "__")) {
		TT_(tk) = TT_SYSVAL;
		DP(tk)->index = KNH_SYS_SCRIPT; SP(tk)->type = TYPE_Script;
	}
	else if(IS_SYSVAL(t, "__line__")) {
		knh_Token_setCONST(ctx, tk, new_Int(ctx, CLASS_Int, SP(tk)->line));
	}
	else if(IS_SYSVAL(t, "__file__")) {
		knh_Token_setCONST(ctx, tk, knh_getURN(ctx, SP(tk)->uri));
	}
//	else if(IS_SYSVAL(t, "__method__") || IS_SYSVAL(t, "__function__")) {
//		knh_Token_setCONST(ctx, tk, knh_Object_getkey(ctx, UPCAST(DP(ctx->gma)->mtd)));
//	}
//	else if(IS_SYSVAL(t, "__namespace__") || IS_SYSVAL(t, "__ns__")) {
//		knh_NameSpace_t *ns = knh_getGammaNameSpace(ctx);
//		knh_Token_setCONST(ctx, tk, DP(ns)->nsname);
//	}
	else if(IS_SYSVAL(t, "EOL")) {
		knh_Token_setCONST(ctx, tk, TS_EOL);
	}
	else if(IS_SYSVAL(t, "BEGIN")) {
		knh_Token_setCONST(ctx, tk, TS_BEGIN);
	}
	else if(IS_SYSVAL(t, "END")) {
		knh_Token_setCONST(ctx, tk, TS_END);
	}
	else {
		return 0;
	}
	knh_style(ctx, 1);
	return 1;
}

/* ------------------------------------------------------------------------ */

static knh_index_t knh_Gamma_rindexFNQ(Ctx *ctx, knh_fieldn_t fnq)
{
	knh_fieldn_t fn = FN_UNMASK(fnq);
	knh_index_t idx;
	knh_fields_t *gf = DP(ctx->gma)->gf;
	for(idx = K_GAMMASIZE - 1; idx >= 0; idx--) {
		if(gf[idx].fn == fn) return idx;
	}
	return -1;
}

static knh_Term_t *knh_TokenNAME_typing(Ctx *ctx, knh_Token_t *tk, knh_type_t reqt, int isLVALUE, int toCALL)
{
	if(TT_(tk) == TT_LOCAL || TT_(tk) == TT_FIELD) {
		return TM(tk);
	}
	else {
		knh_fieldn_t fnq = knh_Token_getfnq(ctx, tk);
		knh_index_t idx;
		if(fnq == FN_NONAME) goto L_ERROR;
		if(FN_isU1(fnq) || FN_isSUPER(fnq)) goto L_FIELD;  /* _name */
		if(FN_isU2(fnq)) {
			if(DP(ctx->gma)->this_cid == knh_Object_cid(knh_getGammaScript(ctx))) goto L_FIELD;
			goto L_SCRIPT; /* __name */
		}
		idx = knh_Gamma_rindexFNQ(ctx, fnq);
		if(idx != -1) {
			knh_fields_t *gf = DP(ctx->gma)->gf + idx;
			if(GMA_isTVAR(ctx, gf->type)) {
				gf->type = GMA_typingTVAR(ctx, gf->type, reqt);
			}
			if(idx < DP(ctx->gma)->goffset) {
				knh_Token_toTYPED(ctx, tk, TT_STACK, gf->type, idx);
				knh_Token_setReadOnly(tk, 1);
			}
			else {
				knh_Token_toTYPED(ctx, tk, TT_LOCAL, gf->type, idx - DP(ctx->gma)->goffset);
				knh_Token_setReadOnly(tk, ((gf->flag & _FREADONLY) == _FREADONLY));
			}
			return TM(tk);
		}

		L_FIELD:;
		idx = knh_Class_queryField(ctx, DP(ctx->gma)->this_cid, fnq);
		if(idx != -1) {
			knh_fields_t *cf = knh_Class_fieldAt(ctx, DP(ctx->gma)->this_cid, idx);
			knh_type_t type = GMA_type(ctx, cf->type);
			knh_Token_toTYPED(ctx, tk, TT_FIELD, type, idx);
			knh_Token_setReadOnly(tk, ((cf->flag & _FREADONLY) == _FREADONLY));
			return TM(tk);
		}

		if(FN_isSUPER(fnq)) goto L_ERROR;
		L_SCRIPT:;
		if(toCALL && DP(ctx->gma)->this_cid != knh_Object_cid(knh_getGammaScript(ctx))) {
			KNH_TEST("script variable in a class method");
			knh_methodn_t mn = (isLVALUE) ? MN_toSETTER(FN_UNMASK(fnq)) : MN_toGETTER(FN_UNMASK(fnq));
			knh_Method_t *mtd = knh_getMethodNULL(ctx, knh_Object_cid(knh_getGammaScript(ctx)), mn);
			if(mtd != NULL) {
				knh_Stmt_t *stmtC =
					new_Stmt2(ctx, STT_CALL, new_TokenMN(ctx, mn), new_TokenCONST(ctx, KNH_NULVAL(DP(mtd)->cid)), NULL);
				return knh_StmtCALL_typing(ctx, stmtC, reqt);
			}
		}

		if(!isLVALUE && !FN_isU1(fnq) && !FN_isU2(fnq)) {
			knh_Method_t *mtd = NULL;
			knh_NameSpace_t *ns = DP(ctx->gma)->ns;
			knh_class_t this_cid = DP(ctx->gma)->this_cid;
			knh_class_t mtd_cid = knh_NameSpace_getFuncClass(ctx, ns, fnq);
			if(mtd_cid != CLASS_unknown) {
				mtd = knh_getMethodNULL(ctx, mtd_cid, fnq);
				if(!knh_Method_isStatic(mtd)) mtd = NULL;
			}
			if(mtd == NULL) {
				mtd = knh_getMethodNULL(ctx, this_cid, fnq);
				if(mtd != NULL && !knh_Method_isStatic(mtd)) {
					mtd = NULL;
				}
			}
			if(mtd == NULL) {
				mtd_cid = knh_Object_cid(knh_getGammaScript(ctx));
				mtd = knh_getMethodNULL(ctx, mtd_cid, fnq);
				if(mtd != NULL && !knh_Method_isStatic(mtd)) {
					mtd = NULL;
				}
			}
			if(mtd != NULL) {
				this_cid = knh_class_FuncType(ctx, this_cid, mtd);
				return knh_Token_setCONST(ctx, tk, new_StaticFunc(ctx, this_cid, mtd));
			}
		}
		L_ERROR:;
		if(isLVALUE == 0) {
			knh_Gamma_perror(ctx, KERR_ERR, _("undefined variable: %L"), tk);
			TT_(tk) = TT_ERR;
		}
		return NULL;
	}
}

static Object *knh_NameSpace_getConstNULL(Ctx *ctx, knh_NameSpace_t *ns, knh_bytes_t name)
{
	L_TAIL:
	if(DP(ns)->lconstDictCaseMapNULL != NULL) {
		knh_index_t idx = knh_DictMap_index(DP(ns)->lconstDictCaseMapNULL, name);
		if(idx != -1) return knh_DictMap_valueAt(DP(ns)->lconstDictCaseMapNULL, idx);
	}
	if(DP(ns)->parentNULL != NULL) {
		ns = DP(ns)->parentNULL;
		goto L_TAIL;
	}
	return knh_getClassConstNULL(ctx, DP(ctx->gma)->this_cid, name);
}

static knh_Term_t* knh_TokenUNAME_typing(Ctx *ctx, knh_Token_t *tk, int isLVALUE)
{
	knh_class_t cid = knh_Token_getcid(ctx, tk, CLASS_unknown);
	if(cid != CLASS_unknown) {
		return knh_Token_toTYPED(ctx, tk, TT_CID, TYPE_Class, cid);
	}
	else {
		knh_NameSpace_t *ns = knh_getGammaNameSpace(ctx);
		Object *value = knh_NameSpace_getConstNULL(ctx, ns, TK_tobytes(tk));
		if(value != NULL) {
			return isLVALUE ? TM(tk) : knh_Token_setCONST(ctx, tk, value);
		}
	}
	if(!isLVALUE) {
		if(knh_Token_toSYSVAL(ctx, tk)) {
			return TM(tk);
		}
		knh_Gamma_perror(ctx, KERR_ERR, _("undefined name: %L"), tk);
		TT_(tk) = TT_ERR;
	}
	return NULL;
}

static knh_Term_t *knh_TokenTYPE_typing(Ctx *ctx, knh_Token_t *tk, knh_type_t reqt)
{
	knh_class_t cid = CLASS_unknown;
	if(IS_Stmt(DP(tk)->data)) {
		knh_Stmt_t *stmt = (knh_Stmt_t*)DP(tk)->data;
		knh_Term_t *tm = knh_Stmt_typing(ctx, stmt, reqt);
		if(tm == NULL) return NULL;
		KNH_SETv(ctx, DP(tk)->data, tm);  // TO AVOID GC
		cid = CLASS_type(tm->type);
	}
	else {
		knh_Token_t *tkC = knh_TOKENs_n(DP(tk)->list, 0);
		knh_ParamArray_t *bpa = NULL;
		DBG_ASSERT(IS_Array(DP(tk)->list));
		cid = knh_Token_getcid(ctx, tkC, CLASS_Any);
		bpa = ClassTBL(cid).cparam;
		if(bpa == NULL) {
			if(cid != CLASS_Any) {
				knh_Gamma_perror(ctx, KERR_EWARN, "%C is not parameterized", cid);
			}
		}
		else {
			BEGIN_LOCAL(ctx, lsfp, 1);
			LOCAL_NEW(ctx, lsfp, 0, knh_ParamArray_t*, pa, new_ParamArray(ctx));
			size_t i;
			for(i = 1; i < knh_Array_size(DP(tk)->list); i++) {
				knh_Token_t *tkT = knh_TOKENs_n(DP(tk)->list, i);
				if(TT_(tkT) == TT_FUNCARROW) { i++; break; }
				knh_param_t p = {knh_Token_getcid(ctx, tkT, CLASS_Any), FN_NONAME};
				knh_ParamArray_add(ctx, pa, p);
			}
			for(; i < knh_Array_size(DP(tk)->list); i++) {
				knh_Token_t *tkT = knh_TOKENs_n(DP(tk)->list, i);
				knh_param_t p = {knh_Token_getcid(ctx, tkT, CLASS_Any), FN_NONAME};
				knh_ParamArray_radd(ctx, pa, p);
			}
			if((pa)->psize != (bpa)->psize || (pa)->rsize != (bpa)->rsize) {
				if(cid != CLASS_Func && cid != CLASS_Tuple && cid != CLASS_Iterator) {
					knh_Gamma_perror(ctx, KERR_EWARN, "different parameter size of %C", cid);
					goto L_RETURN;
				}
			}
			for(i = 0; i < (size_t)((pa)->psize + (pa)->rsize); i++) {
				knh_param_t* p = knh_ParamArray_get(pa, i);
				knh_param_t* bp = knh_ParamArray_get(bpa, i);
				p->fn = bp->fn;
				if(bp->type == TYPE_Any) continue;
				if(p->type != bp->type && !knh_class_instanceof(ctx, CLASS_type(p->type), bp->type)) {
					knh_Gamma_perror(ctx, KERR_EWARN, "incompatible type parameter of %C: %T must be of %T", cid, p->type, bp->type);
					goto L_RETURN;
				}
			}
			cid = knh_class_Generics(ctx, cid, pa);
			L_RETURN:;
			END_LOCAL(ctx, lsfp);
		}
	}
	TK_typing(tk, TT_CID, TYPE_Class, cid);
	KNH_SETv(ctx, DP(tk)->text, ClassTBL(cid).sname);
	return TM(tk);
}

static knh_Term_t* knh_TokenPROPN_typing(Ctx *ctx, knh_Token_t *tk, knh_type_t reqt, int isLVALUE)
{
	knh_bytes_t n = S_tobytes(DP(tk)->text);
	if(B_endsWith(n, "*")) {
		if(isLVALUE) return NULL;
		knh_Token_toCONST(ctx, tk);
		knh_Stmt_t *stmt =
			new_Stmt2(ctx, STT_CALL, new_TokenMN(ctx, MN_listProperties), new_TokenCONST(ctx, ctx->sys), tk, NULL);
		return knh_StmtCALL_typing(ctx, stmt, reqt);
	}
	else {
		knh_String_t *v = knh_getPropertyNULL(ctx, n);
		if(v != NULL) {
			SP(tk)->type = knh_Object_cid(v);
		}
		else {
			if(isLVALUE == 0) {
				Object *v2;
				if(reqt == TYPE_Any || reqt == TYPE_void) reqt = TYPE_String;
				v2 = KNH_NULVAL(CLASS_type(reqt));
				knh_setProperty(ctx, DP(tk)->text, v2);
				SP(tk)->type = reqt;
				knh_Gamma_perror(ctx, KERR_TINFO, "$%B is supposed to be %T", n, reqt);
			}
		}
		return TM(tk);
	}
}

/* ------------------------------------------------------------------------ */
/* [NUM] */

#ifdef K_USING_SEMANTICS
static
knh_class_t knh_Token_tagcNUM(Ctx *ctx, knh_Token_t *tk, knh_class_t reqc, knh_NameSpace_t *ns)
{
	knh_bytes_t t = TK_tobytes(ctx, tk), tag = STEXT("");
	size_t i = 1;
	int ishex = 0;
	if(t.ustr[0] == '0' && (t.ustr[1] == 'x' || t.ustr[1] == 'b')) {
		i = 2;
		ishex = 1;
		knh_style(ctx, 1);
	}
	for(; i < t.len; i++) {
		if(isdigit(t.ustr[i]) || t.ustr[i] == '_' || t.ustr[i] == '.') continue;
		if(t.ustr[i] == '[') {
			int loc;
			tag.buf = t.buf + i + 1;
			tag.len = t.len - (i + 1);
			loc = knh_bytes_index(tag, ']');
			if(loc > 0) {
				tag = knh_bytes_first(tag, loc);
			}
			break;
		}
		else if(t.ustr[i] == ':') {
			tag.buf = t.buf + i + 1;
			tag.len = t.len - (i + 1);
			break;
		}
		else {
			if((t.ustr[i] == 'E' || t.ustr[i] == 'e')) {
				if(isdigit(t.ustr[i+1]) || t.ustr[i+1] == '-' || t.ustr[i+1] == '+') {
					i++;
					continue;
				}
			}
			tag.buf = t.buf + i;
			tag.len = t.len - (i);
			break;
		}
	}
	if(tag.len == 0 || ishex) {
		return reqc;
	}
	else {
		knh_class_t tagc = knh_NameSpace_tagcid(ctx, ns, reqc, tag);
		if(tagc == CLASS_unknown) {
			knh_Gamma_perror(ctx, tk, KERR_DWARN, _("unknown class tag: %L"), tk);
			return reqc;
		}
		knh_style(ctx, 1);
		return tagc;
	}
}
#endif

/* ------------------------------------------------------------------------ */

static knh_class_t knh_bytes_guessNUMcid(Ctx *ctx, knh_bytes_t t)
{
	size_t i;
	if(t.ustr[0] == 0 && (t.ustr[1] == 'x' || t.ustr[1]=='b')) {
		return CLASS_Int;
	}
	for(i = 1; i < t.len; i++) {
		if(t.ustr[i] == '_') {
#ifdef CLASS_Decimal
			return CLASS_Decimal;
#endif
		}
		else if(t.ustr[i] == '.') {
			return CLASS_Float;
		}
		if(!isdigit(t.ustr[i])) break;
	}
	return CLASS_Int;
}

/* ------------------------------------------------------------------------ */

static knh_Term_t *knh_TokenNUM_typing(Ctx *ctx, knh_Token_t *tk, knh_class_t reqt)
{
	knh_bytes_t t = TK_tobytes(tk);
	knh_class_t breqc = knh_class_bcid(reqt);
	if(reqt == CLASS_Boolean) {
		if(t.ustr[0] == '0') {
			knh_Gamma_perror(ctx, KERR_ERRATA, _("false is better than: %B"), t);
			return knh_Token_setCONST(ctx, tk, KNH_FALSE);
		}
		else {
			knh_Gamma_perror(ctx, KERR_ERRATA, _("true is better than: %B"), t);
			return knh_Token_setCONST(ctx, tk, KNH_TRUE);
		}
	}
	if(breqc == CLASS_Int || breqc == CLASS_Float) {
	}
	else if(breqc == CLASS_Any || breqc == CLASS_Object || breqc == CLASS_Number) {
		breqc = knh_bytes_guessNUMcid(ctx, t);
		reqt = breqc;
	}
	else {
		knh_Gamma_perror(ctx, KERR_TERROR, _("not numeric: %C"), reqt);
		TT_(tk) = TT_ERR;
		return NULL;
	}

	if(breqc == CLASS_Float) {
		knh_float_t n = K_FLOAT_ZERO;
		if(!knh_bytes_parsefloat(t, &n)) {
			knh_Gamma_perror(ctx, KERR_EWARN, _("float overflow: %B"), t);
		}
#if defined(K_USING_SEMANTICS)
		knh_class_t tagc = knh_Token_tagcNUM(ctx, tk, reqc, ns);
		knh_Semantics_t *u = knh_getSemantics(ctx, tagc);
		if(!DP(u)->ffchk(u, n)) {
			knh_Gamma_perror(ctx, KERR_ERRATA, _("%C: out of range: %B ==> %O"), tagc, t, DP(u)->fvalue);
			return knh_Token_setCONST(ctx, tk, UPCAST(DP(u)->fvalue));
		}
		else {
			return knh_Token_setCONST(ctx, tk, UPCAST(new_FloatX(ctx, tagc, n)));
		}
#else
		return knh_Token_setCONST(ctx, tk, CONSTPOOL(new_Float(ctx, CLASS_Float, n)));
#endif/*K_USING_SEMANTICS*/
	}
	else { /* if(req_bcid == CLASS_Int) */
		knh_int_t n = 0;
		if(!knh_bytes_parseint(t, &n)) {
			knh_Gamma_perror(ctx, KERR_EWARN, _("int overflow: %B"), t);
		}
#if defined(K_USING_SEMANTICS)
		knh_class_t tagc = knh_Token_tagcNUM(ctx, tk, reqc, ns);
		knh_Semantics_t *u = knh_getSemantics(ctx, tagc);
		if(!DP(u)->fichk(u, n)) {
			knh_Gamma_perror(ctx, KERR_ERRATA, _("%C: out of range: %B ==> %O"), tagc, t, DP(u)->ivalue);
			return knh_Token_setCONST(ctx, tk, UPCAST(DP(u)->ivalue));
		}
		else {
			return knh_Token_setCONST(ctx, tk, UPCAST(new_IntX(ctx, tagc, n)));
		}
#else
		return knh_Token_setCONST(ctx, tk, CONSTPOOL(new_Int(ctx, CLASS_Int, n)));
#endif/*K_USING_SEMANTICS*/
	}
}

static knh_Term_t* knh_TokenTSTR_typing(Ctx *ctx, knh_Token_t *tk, knh_class_t reqt)
{
	knh_bytes_t t = TK_tobytes(tk);
	if(CLASS_type(reqt) != CLASS_String && knh_bytes_mlen(t) == 1) {
		/* 'A' ==> int if not String */
		knh_bytes_t sub = knh_bytes_mofflen(t, 0, 1);
		return knh_Token_setCONST(ctx, tk, new_Int(ctx, CLASS_Int, knh_uchar_toucs4(&sub.ustr[0])));
	}
	return knh_Token_toCONST(ctx, tk);
}


static knh_bool_t knh_bytes_isFMT(knh_bytes_t t)
{
	size_t i = 0, size = t.len - 1;
	if(t.len < 2) return 0;
	L_AGAIN:;
	for(i = 0; i < size; i++) {
		if(t.ustr[i] == '%') {
			int ch = t.ustr[i+1];
			i++;
			if(isdigit(ch) || ch == ' ' || ch == '.' || ch == '+' || ch == '-' || ch == '#') {
				goto L_CFMT;
			}
			if(isalpha(ch)) goto L_KFMT;
		}
	}
	return 0;
	L_CFMT:;
	for(; i < size; i++) {
		int ch = t.ustr[i];
		if(isalpha(ch) || t.ustr[i+1] == '{') return 1;
		if(!isdigit(ch) && ch != '.') goto L_AGAIN;
	}
	return 0;
	L_KFMT:;
	for(; i < size; i++) {
		int ch = t.ustr[i];
		if(ch == '{') return 1;
		if(!isalnum(ch) && ch != ':') goto L_AGAIN;
	}
	return 0;
}

static knh_class_t knh_bytes_CFMT(knh_bytes_t t)
{
	if(t.ustr[0] == '%' && (isdigit(t.ustr[1]) || t.ustr[1] == ' ' || t.ustr[1] == '.')) {
		int ch = t.ustr[t.len - 1];
		switch(ch) {
		case 'd': case 'u': case 'x': case 'c': return CLASS_Int;
		case 'e': case 'f': return CLASS_Float;
		case 's': return CLASS_String;
		}
	}
	return CLASS_unknown;
}

static knh_methodn_t knh_bytes_parsemn(Ctx *ctx, knh_bytes_t t)
{
	if(t.ustr[0] == '%' && t.ustr[1] != '%') {
		size_t i;
		for(i = 1; i < t.len; i++) {
			int ch = t.ustr[i];
			if(isalnum(ch) || ch == ':' || ch == ' ') continue;
			if(ch == '.' && !isalpha(t.ustr[i-1])) continue;
			break;
		}
		if(i == t.len) {
			return knh_getmn(ctx, t, MN_NEWID);
		}
	}
	return MN_NONAME;
}

static void knh_Stmt_trimSize(Ctx *ctx, knh_Stmt_t *stmt, size_t size)
{
	size_t i;
	for(i = size; i < DP(stmt)->size; i++) {
		KNH_FINALv(ctx, DP(stmt)->terms[i]);
		DP(stmt)->terms[i] = NULL;
	}
	DP(stmt)->size = size;
}

static void knh_Token_toMTD(Ctx *ctx, knh_Token_t *tk, knh_methodn_t mn, knh_Method_t *mtd)
{
	TT_(tk) = TT_MN;
	DP(tk)->mn = mn;
	SP(tk)->type = TYPE_Method;
	if(mtd != NULL) {
		KNH_SETv(ctx, DP(tk)->data, mtd);
	}
}

static knh_Term_t *knh_StmtPARAMs_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_class_t mtd_cid, knh_Method_t *mtd);
static knh_Term_t *knh_StmtEXPR_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_class_t reqt);

static knh_Term_t *knh_StmtFMT_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	knh_Token_t *tkFMT = DP(stmt)->tokens[0];
	knh_bytes_t fmt = S_tobytes(DP(tkFMT)->text);
	if(fmt.ustr[0] == '%') {
		if(!knh_bytes_isFMT(fmt)) {
			knh_class_t cid = knh_bytes_CFMT(fmt);
			if(DP(stmt)->size == 2) {
				knh_type_t type = (cid == CLASS_unknown) ? CLASS_String : cid;
				knh_Stmt_add(ctx, stmt, new_TokenTYPED(ctx, TT_NULL/*DEFVAL*/, type, type));
			}
			if(DP(stmt)->size > 3) {
				knh_Gamma_perror(ctx, KERR_DWARN, _("too many parameters of '%B'"), fmt);
				knh_Stmt_trimSize(ctx, stmt, 3);
			}
			if(cid != CLASS_unknown) {  // "%4d"(1)
				knh_Method_t *mtd = knh_getMethodNULL(ctx, cid, MN_format);
				knh_Token_setCONST(ctx, DP(stmt)->tokens[1], DP(tkFMT)->data);
				TYPING(ctx, stmt, 2, cid, _ICAST);
				knh_Stmt_swap(ctx, stmt, 1, 2);
				knh_Token_toMTD(ctx, tkFMT, MN_format, mtd);
				STT_(stmt) = STT_CALL;
				return knh_StmtPARAMs_typing(ctx, stmt, cid, mtd);
			}
			else if(fmt.ustr[0] == '%') { // "%bit" (a)
				knh_methodn_t mn = knh_bytes_parsemn(ctx, fmt);
				if(mn == MN_NONAME) {
					knh_Gamma_perror(ctx, KERR_DWARN, "invalid formatter: '%s'", fmt.text);
					return knh_Token_toCONST(ctx, tkFMT);
				}
				TYPING(ctx, stmt, 2, TYPE_Object, 0);
				KNH_SETv(ctx, DP(tkFMT)->data, DP(stmt)->terms[2]);
				KNH_SETv(ctx, DP(stmt)->terms[2], knh_Token_toTYPED(ctx, tkFMT, TT_MT, TYPE_String, mn));
				DBG_ASSERT(TT_(DP(stmt)->terms[1]) == TT_ASIS);
				DP(stmt)->wstart = 2;
				goto L_TOWRITE;
			}
		}
		goto L_FMT;
	}
	else if(knh_bytes_isFMT(fmt)) {
		goto L_FMT;
	}
	else {  // "foo"()
		knh_Gamma_perror(ctx, KERR_DWARN, "nothing to format");
		return knh_Token_toCONST(ctx, tkFMT);
	}
	L_FMT:;
	{
		size_t i, argc, size = DP(stmt)->size;
		long min = LONG_MAX, max = 0;
		BEGIN_LOCAL(ctx, lsfp, 2);
		LOCAL_NEW(ctx, lsfp, 0, knh_Array_t*, a, new_Array0(ctx, 0));
		KNH_SETv(ctx, lsfp[1].o, DP(stmt)->terms[1]); // backups
		knh_String_parseFMT(ctx, DP(tkFMT)->text, a, SP(tkFMT)->uri, SP(tkFMT)->line);
		knh_Method_t *mtd = knh_getSystemFormatter(ctx, CLASS_Array, MN__k);
		knh_write_Object(ctx, KNH_STDOUT, ctx->esp, &mtd, UPCAST(a));
		for(i = 0; i < knh_Array_size(a); i+= 2) {
			if(IS_String(a->list[i]) && IS_String(a->list[i+1])) {
				knh_bytes_t mt = S_tobytes(a->strings[i]);
				knh_bytes_t idx = S_tobytes(a->strings[i]);
				if(mt.ubuf[0] == '%' && idx.ubuf[0] == '#') {
					long n = strtol(idx.text+1, NULL, 10);
					if(n > (long)size || n <= 0) {
						knh_Gamma_perror(ctx, KERR_ERR, _("too large/small index for formatting: %d"), n);
						return NULL;
					}
					if(n < LONG_MAX) min = n;
					if(n > max) max = n;
				}
			}
			if(IS_Stmt(a->list[i+1])) {
				knh_Stmt_t *stmtINLINE = a->stmts[i+1];
				if(knh_stmt_isExpr(STT_(stmtINLINE))) {
					knh_Term_t *tm = knh_StmtEXPR_typing(ctx, stmtINLINE, TYPE_Any);
					if(tm == NULL) return NULL;
					KNH_SETv(ctx, a->list[i+1], tm);
				}
				else {
					knh_Gamma_perror(ctx, KERR_ERR, _("%d of inline formating is not an expression"), (int)((i / 2) + 1));
					return NULL;
				}
			}
		}
		if(min == LONG_MAX) {
			argc = 0; min = 0; max = 0;
		}
		else {
			argc = max - min + 1;
		}
		DBG_P("min=%ld max=%ld, argc=%d", min, max, argc);
		if(size - 2 > argc) {
			knh_Gamma_perror(ctx, KERR_DWARN, _("too many parameters of inline formating"));
			knh_Stmt_trimSize(ctx, stmt, argc + 2);
		}
		for(i = size; i < argc + 2; i++) {
			knh_Stmt_add(ctx, stmt, new_TokenTYPED(ctx, TT_NULL/*DEFVAL*/, CLASS_String, CLASS_String));
		}
		DP(stmt)->wstart = argc + 2;
		DBG_ASSERT(DP(stmt)->size == DP(stmt)->wstart);
//		for(i = 0; i < knh_Array_size(a); i+= 2) {
//			if(IS_String(a->list[i]) && IS_String(a->list[i+1])) {
//				knh_bytes_t mt = S_tobytes(a->strings[i]);
//				knh_bytes_t idx = S_tobytes(a->strings[i]);
//				if(mt.ubuf[0] == '%' && idx.ubuf[0] == '#') {
//					long n = strtol(idx.text+1, NULL, 10);
//				}
//			}
//			if(IS_Stmt(a->list[i+1])) {
//				knh_Stmt_t *stmtINLINE = a->stmts[i+1];
//				if(knh_stmt_isExpr(STT_(stmtINLINE))) {
//					knh_Term_t *tm = knh_StmtEXPR_typing(ctx, stmtINLINE, TYPE_Any);
//					if(tm == NULL) return NULL;
//					KNH_SETv(ctx, a->list[i+1], tm);
//				}
//				else {
//					knh_Gamma_perror(ctx, KERR_ERR, _("%d of inline formating is not an expression"), (int)((i / 2) + 1));
//					return NULL;
//				}
//			}
//		}
		END_LOCAL(ctx, lsfp);
	}
	return NULL;
	L_TOWRITE:;
	STT_(stmt) = STT_W;
	knh_Stmt_setCWB(stmt, 1);
	return knh_Stmt_typed(ctx, stmt, TYPE_String);
}

static knh_Term_t* knh_TokenESTR_typing(Ctx *ctx, knh_Token_t *tk, knh_class_t reqt)
{
	if(knh_bytes_isFMT(S_tobytes(DP(tk)->text))) {
		knh_Stmt_t *stmt = new_Stmt2(ctx, STT_FMT, tk, new_(Token)/*ASIS*/, NULL);
		return knh_StmtFMT_typing(ctx, stmt, reqt);
	}
	else {
		return knh_Token_toCONST(ctx, tk);
	}
}

static knh_Term_t* knh_TokenURN_typing(Ctx *ctx, knh_Token_t *tk, knh_class_t reqt)
{
	knh_bytes_t path = S_tobytes(DP(tk)->text);
	knh_NameSpace_t *ns = DP(ctx->gma)->ns;
	knh_PathDSPI_t *dspi = knh_NameSpace_getPathDSPINULL(ctx, ns, path);
	if(dspi == NULL) {
		knh_Gamma_perror(ctx, KERR_ERR, "unknown path: %B", path);
		return NULL;
	}
	if(TT_(tk) == TT_QPATH) {
		return knh_Token_toCONST(ctx, tk);
	}
	if(reqt == TYPE_Any) reqt = dspi->cid;
	if(!dspi->isTyped(ctx, CLASS_type(reqt))) {
		knh_Gamma_perror(ctx, KERR_ERR, "%B can be %T, NOT %T, ", path, dspi->cid, reqt);
		return NULL;
	}
	if(reqt == TYPE_Boolean) {
		knh_Object_t *tf = dspi->exists(ctx, path, ns) == PATH_unknown ? KNH_FALSE : KNH_TRUE;
		return knh_Token_setCONST(ctx, tk, tf);
	}
	else {
		knh_Object_t *o = dspi->newObjectNULL(ctx, reqt, DP(tk)->text, ns);
		if(o == NULL) {
			if(reqt == TYPE_String) {
				o = DP(tk)->data;
			}
			else {
				o = KNH_NULVAL(reqt);
			}
		}
		return knh_Token_setCONST(ctx, tk, o);
	}
}

/* ------------------------------------------------------------------------ */
/* [Token] */

static knh_Term_t *knh_Token_typing(Ctx *ctx, knh_Token_t *tk, knh_type_t reqt)
{
	knh_Term_t *tm = NULL;
	if(knh_Token_isTyped(tk)) return (knh_Term_t*)tk;
	switch(TT_(tk)) {
	case TT_ASIS: TT_(tk) = TT_NULL;
	case TT_NULL: tm = knh_Token_toTYPED(ctx, tk, TT_NULL/*DEFVAL*/, reqt, CLASS_type(reqt)); break;
	case TT_TRUE: tm = knh_Token_setCONST(ctx, tk, KNH_TRUE); break;
	case TT_FALSE: tm = knh_Token_setCONST(ctx, tk, KNH_FALSE); break;
	case TT_NAME: tm = knh_TokenNAME_typing(ctx, tk, reqt, 0/*isLVALUE*/, 1/*toCALL*/); break;
	case TT_UNAME: tm = knh_TokenUNAME_typing(ctx, tk, 0/*isLVALUE*/); break;
	case TT_TYPE: tm = knh_TokenTYPE_typing(ctx, tk, reqt); break;
	case TT_PROPN: tm = knh_TokenPROPN_typing(ctx, tk, reqt, 0/*isLVALUE*/); break;
	case TT_REGEX: case TT_STR:
		tm = knh_Token_toCONST(ctx, tk); break;
	case TT_TSTR: tm = knh_TokenTSTR_typing(ctx, tk, reqt); break;
	case TT_ESTR: tm = knh_TokenESTR_typing(ctx, tk, reqt); break;
	case TT_NUM: tm = knh_TokenNUM_typing(ctx, tk, reqt); break;
	case TT_URN: case TT_QPATH:
		tm = knh_TokenURN_typing(ctx, tk, reqt); break;
	case TT_ERR: return NULL;
	}
	if(tm != NULL && SP(tm)->type == TYPE_var) {
		knh_Gamma_perror(ctx, KERR_ERR, _("syntax error: why %L?"), tk);
		TT_(tk) = TT_ERR;
	}
	return tm;
}

/* ------------------------------------------------------------------------ */

#define GF_FIELD  K_FLAG_T1
#define GF_UNIQUE K_FLAG_T2

static knh_index_t knh_Gamma_add(Ctx *ctx, knh_fields_t *gf, size_t s, size_t e, knh_fields_t *decl, knh_flag_t op)
{
	knh_index_t idx;
	for(idx = (knh_index_t)s; idx < (knh_index_t)e; idx++) {
		if(gf[idx].fn == decl->fn && decl->fn != FN_) {
			if(FLAG_is(op, GF_UNIQUE)) {
				if(gf[idx].type == decl->type) return idx;
				knh_Gamma_perror(ctx, KERR_TERROR, _("differently declared: previous type %T %N"), gf->type, decl->fn);
				return -1;
			}
		}
		if(gf[idx].fn != FN_NONAME) continue;
		DBG_ASSERT(gf[idx].value == NULL);
		gf[idx] = *decl;
		if(decl->value != NULL) {
			KNH_INITv(gf[idx].value, decl->value);
		}
		if(FLAG_is(op, GF_FIELD)) {
			if((IS_Tint(decl->type) && (sizeof(knh_int_t) > sizeof(void*))) ||
					(IS_Tfloat(decl->type) && (sizeof(knh_float_t) > sizeof(void*)))) {
				gf[idx+1].flag = 0;
				gf[idx+1].type = TYPE_void;
				gf[idx+1].fn   = FN_;
				DBG_ASSERT(gf[idx+1].value == NULL);
			}
		}
		DP(ctx->gma)->espidx = idx + 1;
		return idx;
	}
	knh_Gamma_perror(ctx, KERR_ERR, _("too many variables"));
	return -1;
}

static knh_index_t knh_Gamma_addLocal(Ctx *ctx, knh_fields_t *decl, knh_flag_t op)
{
	size_t s = DP(ctx->gma)->goffset;
	return knh_Gamma_add(ctx, DP(ctx->gma)->gf, s, K_GAMMASIZE - s, decl, op);
}

static void knh_Gamma_clear(Ctx *ctx, size_t offset)
{
	size_t i;
	knh_fields_t *gf = DP(ctx->gma)->gf;
	for(i = offset; i < K_GAMMASIZE; i++) {
		if(gf[i].fn == FN_NONAME) break;
		gf[i].flag  = 0;
		gf[i].type  = TYPE_void;
		gf[i].fn    = FN_NONAME;
		if(gf[i].value != NULL) {
			KNH_FINALv(ctx, gf[i].value);
			gf[i].value = NULL;
		}
	}
	DP(ctx->gma)->espidx = offset;
}

/* ------------------------------------------------------------------------ */
/* STMT */

static knh_Term_t *knh_StmtLET_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt, int scope);

static knh_flag_t knh_StmtDECL_flag(Ctx *ctx, knh_Stmt_t *o)
{
	knh_flag_t flag = 0;
	if(IS_Map(DP(o)->metaDictCaseMap)) {
		ADD_FLAG(flag, "Getter", _FGETTER);
		ADD_FLAG(flag, "Setter", _FSETTER);
		ADD_FLAG(flag, "Volatile", FLAG_Field_Volatile);
		ADD_FLAG(flag, "ReadOnly", _FREADONLY);
		ADD_FLAG(flag, "Key", _FKEY);
	}
	if((flag & _FREADONLY) == _FREADONLY) {
		flag = ((~_FSETTER) & flag);
	}
	return flag;
}

static void knh_Script_checkFieldSize(Ctx *ctx, knh_Script_t *scr, knh_type_t type)
{
	size_t usesize = 1;
	knh_ClassTBL_t *t = (knh_ClassTBL_t*)(&ClassTBL(knh_Object_cid(scr)));
	if(sizeof(knh_int_t) > sizeof(Object*) && IS_Tint(type)) usesize += 1;
	if(sizeof(knh_float_t) > sizeof(Object*) && IS_Tfloat(type)) usesize += 1;
	if(!(scr->fsizeUSED + usesize < t->fsize)) {
		size_t i, newsize = t->fsize == 0 ? (K_FASTMALLOC_SIZE / sizeof(void*)) : t->fsize * 2;
		knh_fields_t *newfields = (knh_fields_t*)KNH_MALLOC(ctx, sizeof(knh_fields_t) * newsize);
		knh_Object_t **newscrb = (knh_Object_t**)KNH_MALLOC(ctx, sizeof(Object*) * newsize);
		if(t->size > 0) {
			knh_memcpy(newfields, t->fields, t->fsize * sizeof(knh_fields_t));
			knh_memcpy(newscrb, scr->fields, t->fsize * sizeof(Object*));
			KNH_FREE(ctx, t->fields, sizeof(knh_fields_t) * t->fsize);
			KNH_FREE(ctx, scr->fields, sizeof(Object*) * t->fsize);
		}
		t->fields = newfields;
		scr->fields = newscrb;
		for(i = t->fsize; i < newsize; i++) {
			newfields[i].flag  = 0;
			newfields[i].type  = TYPE_void;
			newfields[i].fn    = FN_NONAME;
			newfields[i].value = NULL;
		}
		knh_bzero(&newscrb[t->fsize], sizeof(Object*) * (newsize - t->fsize));
		t->fsize = newsize;
		t->size = t->fsize * sizeof(knh_Object_t*);
	}
	if(!IS_Tunbox(type)) {
		KNH_INITv(scr->fields[scr->fsizeUSED], KNH_NULVAL(CLASS_type(type)));
	}
	scr->fsizeUSED += usesize;
}

#define CHECK_INDEX(c)  if((idx = c) == -1) {\
		knh_Stmt_toERR(ctx, stmt, NULL); return NULL;\
	}\

static knh_Term_t *knh_StmtDECL_typing(Ctx *ctx, knh_Stmt_t *stmt, size_t offset, knh_type_t reqt, int scope)
{
	knh_index_t idx;
	knh_fields_t declbuf, *decl = &declbuf;
	knh_Token_t *tkT = DP(stmt)->tokens[offset+0];
	knh_Token_t *tkN = DP(stmt)->tokens[offset+1];
	knh_Token_t *tkV = DP(stmt)->tokens[offset+2];
	knh_fieldn_t fnq = knh_Token_getfnq(ctx, tkN);
	decl->flag = knh_StmtDECL_flag(ctx, stmt);
	decl->type = knh_Token_getcid(ctx, tkT, TYPE_Any);
	decl->fn  = FN_UNMASK(fnq);
	decl->value = NULL;
	if(scope < SCOPE_SCRIPT && FN_isU2(fnq)) {
		knh_Gamma_perror(ctx, KERR_ERRATA, _("__%N ==> local variable"), decl->fn);
	}
	else if(scope < SCOPE_FIELD && FN_isU1(fnq)) {
		knh_Gamma_perror(ctx, KERR_ERRATA, _("_%N ==> local variable"), decl->fn);
	}
	if(decl->type == TYPE_var && (reqt != TYPE_void && reqt != TYPE_Any)) {
		decl->type = reqt;
	}
	if(decl->type == TYPE_var) {  /* TYPE INFERENCE */
		TYPING(ctx, stmt, offset+2, TYPE_var, _NOTCHECK);
		decl->type = TERMs_gettype(stmt, offset+2);
		if(decl->type == TYPE_var) {
			decl->type = (scope == SCOPE_PARAM) ? GMA_newTVAR(ctx) : TYPE_Any;
		}
		DP(tkT)->cid = decl->type;
	}
	else {
		if(TT_(tkV) == TT_ASIS) TT_(tkV) = TT_NULL;
		TYPING(ctx, stmt, offset+2, decl->type, 0);
	}
	if(TT_(tkV) == TT_CONST) decl->value = DP(tkV)->data;
	if(TT_(tkV) == TT_PROPN) {
		decl->value = DP(tkV)->data;
		decl->flag |= FLAG_Field_Property;
	}
	if(scope == SCOPE_LOCAL) {
		CHECK_INDEX(knh_Gamma_addLocal(ctx, decl, 0));
	}
	else if(scope == SCOPE_PARAM) {
		if(decl->value == NULL && TT_(tkV) != TT_NULL) {
			knh_Gamma_perror(ctx, KERR_DWARN, _("default value must be constant: %N"), decl->fn);
		}
		CHECK_INDEX(knh_Gamma_addLocal(ctx, decl, GF_UNIQUE));
		DP(stmt)->espidx = idx;
		return knh_Stmt_typed(ctx, stmt, TYPE_void);
	}
	else if(scope == SCOPE_FIELD) {
		if(!FN_isU1(fnq)) {
			decl->flag |= _FGETTER | _FSETTER;
		}
		if((decl->flag & _FREADONLY) == _FREADONLY) {
			decl->flag = ((~_FSETTER) & decl->flag);
		}
		if((decl->flag & _FKEY) == _FKEY) {
			if(knh_class_bcid(CLASS_type(decl->type)) != CLASS_String) {
				knh_Gamma_perror(ctx, KERR_EWARN, _("%T is not for key"), decl->type);
				decl->flag = ((~_FKEY) & decl->flag);
			}
		}
		if(decl->fn == FN_key) {
			if(knh_class_bcid(CLASS_type(decl->type)) == CLASS_String) {
				decl->flag |= _FKEY;
			}
		}
		CHECK_INDEX(knh_Gamma_add(ctx, DP(ctx->gma)->gf, 0, K_GAMMASIZE, decl, GF_UNIQUE|GF_FIELD));
		return knh_Stmt_done(ctx, stmt);
	}
	else { DBG_ASSERT(scope == SCOPE_SCRIPT);
		knh_Script_t *scr = knh_getGammaScript(ctx);
		knh_ClassTBL_t *t = pClassTBL(ctx, knh_Object_cid(scr));
		decl->flag |= _FGETTER | _FSETTER;
		if((decl->flag & _FREADONLY) == _FREADONLY) {
			decl->flag = ((~_FSETTER) & decl->flag);
		}
		knh_Script_checkFieldSize(ctx, scr, decl->type);
		CHECK_INDEX(knh_Gamma_add(ctx, t->fields, 0, t->fsize, decl, GF_FIELD|GF_UNIQUE));
	}
	knh_Stmt_toSTT(stmt, STT_LET);
	if(TT_(tkV) == TT_ASIS) {
		knh_Token_toTYPED(ctx, tkV, TT_NULL/*DEFVAL*/, decl->type, decl->type);
	}
	knh_Token_setReadOnly(tkN, 0);
	return knh_StmtLET_typing(ctx, stmt, reqt, scope);
}

#define MN_isNEW(mn)  knh_methodn_isNEW(ctx, mn)

static int knh_methodn_isNEW(Ctx *ctx, knh_methodn_t mn)
{
	if(mn == MN_new) return 1;
	if(!MN_isFMT(mn) && !MN_isGETTER(mn) && !MN_isSETTER(mn)) {
		const char *n = MN__(mn);
		if(n[0] == 'n' && n[1] == 'e' && n[2] == 'w' && n[3] == ':') {
			return 1;
		}
	}
	return 0;
}

static knh_Term_t *knh_StmtLET_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt, int scope)
{
	knh_Token_t *tkN = DP(stmt)->tokens[1];
	const char * emsg = _("illegal assignment: %L");
	if(TT_(tkN) == TT_UNAME) {
		if(!IS_SCRIPTLEVEL(ctx)) {
			emsg = _("wrong place to define %L"); goto L_ERROR;
		}
		if(knh_TokenUNAME_typing(ctx, tkN, 1/*isLVALUE*/) != NULL) {
			if(knh_StmtMETA_is(ctx, stmt, STEXT("Once"))) {
				return knh_Stmt_done(ctx, stmt);
			}
			emsg = _("defined name: %L"); goto L_ERROR;
		}
		knh_Stmt_toSTT(stmt, STT_CALL);
		KNH_SETv(ctx, DP(stmt)->tokens[0], new_TokenMN(ctx, MN_setConst));
		knh_Stmt_insert(ctx, stmt, 1, new_TokenCONST(ctx, DP(ctx->gma)->ns));
		TT_(tkN) = TT_STR;
		return knh_StmtCALL_typing(ctx, stmt, reqt);
	}
	else if(TT_(tkN) == TT_PROPN) {
		knh_Term_t *tm = knh_TokenPROPN_typing(ctx, tkN, reqt, 1/*isLVALUE*/);
		if(tm == NULL || TT_(tm) != TT_PROPN) goto L_ERROR;
		if(SP(tkN)->type != TYPE_var) reqt = SP(tkN)->type;
		TYPING(ctx, stmt, 2, reqt, 0);
		if(SP(tkN)->type == TYPE_var) {  // again
			reqt = TERMs_gettype(stmt, 2);
			knh_TokenPROPN_typing(ctx, tkN, reqt, 0/*isLVALUE*/);
		}
		knh_Stmt_toSTT(stmt, STT_CALL);
		KNH_SETv(ctx, DP(stmt)->tokens[0], new_TokenMN(ctx, MN_setProperty));
		knh_Stmt_insert(ctx, stmt, 1, new_TokenTYPED(ctx, TT_CID, CLASS_System, CLASS_System));
		TT_(tkN) = TT_STR; SP(tkN)->type = TYPE_var; // reset
		knh_StmtCALL_typing(ctx, stmt, reqt);
		SP(stmt)->type = reqt;
		return TM(stmt);
	}
	else if(TT_(tkN) == TT_NAME || TT_(tkN) == TT_LOCAL || TT_(tkN) == TT_FIELD) {
		knh_Term_t *tm = knh_TokenNAME_typing(ctx, tkN, reqt, 1/*isLVALUE*/, 1/*toCALL*/);
		if(tm == NULL) {
			return knh_StmtDECL_typing(ctx, stmt, +0, reqt, scope);
		}
		if(knh_Token_isReadOnly(tkN)) {
			knh_Method_t *mtd = DP(ctx->gma)->mtd;
			if(!(TT_(tkN) == TT_FIELD && MN_isNEW(DP(mtd)->mn))) {
				emsg = _("readonly name: %L");
				goto L_ERROR;
			}
		}
		TYPING(ctx, stmt, 2, tm->type, 0);
		if(TT_(tm) == STT_CALL) {
			knh_Stmt_t *stmtL = (knh_Stmt_t*)tm;
			DBG_ASSERT(DP(stmtL)->size ==2);
			DBG_ASSERT(DP(stmt)->size == 3);
			KNH_SETv(ctx, DP(stmt)->terms[0], DP(stmtL)->terms[0]);
			KNH_SETv(ctx, DP(stmt)->terms[1], DP(stmtL)->terms[1]);
			knh_Stmt_toSTT(stmt, STT_(stmtL));
			KNH_SETv(ctx, ctx->esp[0].o, stmtL); // to avoid memory leak
			return knh_StmtCALL_typing(ctx, stmt, reqt);
		}
		return knh_Stmt_typed(ctx, stmt, tm->type);
	}
	L_ERROR:
	knh_Gamma_perror(ctx, KERR_ERR, emsg, tkN);
	TT_(tkN) = TT_ERR;
	return NULL;
}

static knh_Term_t *knh_StmtLETM_typingNAME(Ctx *ctx, knh_Stmt_t *stmt, size_t size, knh_class_t mtd_cid, knh_ParamArray_t *pa, int scope)
{
	BEGIN_LOCAL(ctx, lsfp, 1);
	size_t i;
	knh_Token_t *tk = new_(Token); // ASIS
	knh_Stmt_t *stmtLET = new_Stmt2(ctx, STT_LET, tk, tk, tk, NULL);
	KNH_SETv(ctx, lsfp[0].o, stmtLET);
	for(i = 0; i < size; i++) {
		knh_Term_t *tm = NULL;
		KNH_SETv(ctx, DP(stmtLET)->terms[1], DP(stmt)->terms[i]);
		if(pa != NULL) {
			knh_param_t *p = knh_ParamArray_rget(pa, i);
			knh_type_t rtype = knh_type_tocid(ctx, p->type, mtd_cid);
			KNH_SETv(ctx, DP(stmtLET)->terms[2], new_TokenTYPED(ctx, TT_NULL, rtype, CLASS_type(rtype)));
		}
		else {
			KNH_SETv(ctx, DP(stmtLET)->terms[2], DP(stmt)->terms[size+i]);
		}
		if(knh_StmtLET_typing(ctx, stmtLET, TYPE_void, scope) == NULL) {
			return NULL;
		}
		if(tm == NULL || TT_(tm) == STT_CALL) {
			KNH_TODO("non field/local variable");
			KNH_SETv(ctx, DP(stmt)->terms[i], tm);
			stmtLET = new_Stmt2(ctx, STT_LET, tk, tk, tk, NULL);
			KNH_SETv(ctx, lsfp[0].o, stmtLET);
		}
	}
	END_LOCAL(ctx, lsfp);
	return knh_Stmt_typed(ctx, stmt, TYPE_void);
}

static knh_Term_t *knh_StmtLETM_typing(Ctx *ctx, knh_Stmt_t *stmt, int scope)
{
	size_t size = DP(stmt)->size - 1;
	TYPING(ctx, stmt, size, TYPE_Any, 0);
	knh_Stmt_t *stmtRVAL = DP(stmt)->stmts[size];
	if(STT_(stmtRVAL) == STT_CALL) {
		knh_Token_t *tkM = DP(stmtRVAL)->tokens[0];
		if(IS_Method(DP(tkM)->mtd)) {
			knh_ParamArray_t *pa = DP(DP(tkM)->mtd)->mp;
			if(size < pa->rsize) {
				knh_class_t mtd_cid = TERMs_getcid(stmtRVAL, 1);
				return knh_StmtLETM_typingNAME(ctx, stmt, size, mtd_cid, pa, scope);
			}
		}
	}
	knh_class_t cid = knh_class_bcid(TERMs_getcid(stmt, size));
	if(knh_class_bcid(cid)  == CLASS_Tuple) {
		return knh_StmtLETM_typingNAME(ctx, stmt, size, DP(ctx->gma)->this_cid, ClassTBL(cid).cparam, scope);
	}
	knh_Gamma_perror(ctx, KERR_ERR, _("not multi-value"));
	return NULL;
}

static knh_Term_t *knh_StmtPUSHM_typing(Ctx *ctx, knh_Stmt_t *stmt, int scope)
{
#ifdef MN_push
	size_t i, size = DP(stmt)->size / 2;
	knh_Term_t *tm = knh_StmtLETM_typingNAME(ctx, stmt, size, TYPE_void, NULL, scope);
	if(tm != NULL) {
		knh_Stmt_t *stmtCALL2 = new_Stmt2(ctx, STT_CALL, new_TokenMN(ctx, MN_push), new_TokenCID(ctx, CLASS_System), NULL);
		for(i = 0; i < size; i++) {
			knh_Stmt_add(ctx, stmtCALL2, DP(stmt)->terms[size+i]);
		}
		knh_Stmt_trimSize(ctx, stmt, size);
		knh_Stmt_add(ctx, stmt, stmtCALL2);
		SP(stmtCALL2)->type = TYPE_void;
		knh_Stmt_toSTT(stmt, STT_LETM);
	}
	return tm;
#endif
	KNH_TODO("PUSHM");
	return NULL;
}


static knh_type_t knh_Gamma_getReturnType(Ctx *ctx)
{
	knh_Method_t *mtd = DP(ctx->gma)->mtd;
	return GMA_type(ctx, knh_ParamArray_rtype(DP(mtd)->mp));
}

static void knh_Gamma_inferReturnType(Ctx *ctx, size_t n, knh_type_t rtype)
{
}

static knh_Term_t *knh_StmtRETURN_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	size_t i, size = DP(stmt)->size;
	knh_Method_t *mtd = DP(ctx->gma)->mtd;
	knh_ParamArray_t *pa = DP(mtd)->mp;
	knh_class_t this_cid = DP(ctx->gma)->this_cid;
	if(pa->rsize < size) {
		knh_Gamma_perror(ctx, KERR_DWARN, _("too many return values"));
		size = pa->rsize;
		knh_Stmt_trimSize(ctx, stmt, size);
	}
	if(size == 0 && MN_isNEW(DP(mtd)->mn)) {
		knh_Token_t *tk = new_TokenTYPED(ctx, TT_LOCAL, this_cid, 0);
		knh_Stmt_add(ctx, stmt, tk); size = 1;
	}
	for(i = 0; i < size; i++) {
		knh_param_t *p = knh_ParamArray_rget(pa, i);
		knh_type_t type = GMA_type(ctx, p->type);
		TYPING(ctx, stmt, i, type, 0);
		knh_Gamma_inferReturnType(ctx, i, TERMs_gettype(stmt, i));
	}
	if(size == 1 && STT_(DP(stmt)->stmts[0]) == STT_CALL) {
		knh_Token_t *tkF = DP(DP(stmt)->stmts[0])->tokens[0];
		if(DP(ctx->gma)->mtd == DP(tkF)->mtd) {
			knh_Stmt_setTAILRECURSION(DP(stmt)->stmts[0], 1);
		}
	}
	if(size < pa->rsize) {
		for(i = size; i < pa->rsize; i++) {
			knh_param_t *p = knh_ParamArray_rget(pa, i);
			knh_type_t type = GMA_type(ctx, p->type);
			knh_Token_t *tk = new_TokenTYPED(ctx, TT_NULL/*DEFVAL*/, type, CLASS_type(type));
			knh_Stmt_add(ctx, stmt, tk);
			knh_Gamma_perror(ctx, KERR_ERRATA, _("return default value of %C"), CLASS_type(type));
		}
	}
	return knh_Stmt_typed(ctx, stmt, reqt);
}

static knh_Term_t *knh_StmtCALL1_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	if(IS_Stmt(DP(stmt)->stmtPOST)) {
		knh_Term_t *tm = knh_Stmt_typing(ctx, DP(stmt)->stmtPOST, reqt);
		if(tm == NULL) return NULL;
	}
	if(reqt == TYPE_void) {
		if(!IS_Stmt(DP(stmt)->stmtPOST)) {
			knh_Gamma_perror(ctx, KERR_DWARN, _("no effect"));
			return knh_Stmt_done(ctx, stmt);
		}
		else {
			knh_Stmt_t *stmt2 = DP(stmt)->stmtPOST;
			knh_StmtEX_t *stmtEX = DP(stmt);
			STT_(stmt) = STT_(stmt2); SP(stmt)->type = SP(stmt2)->type;
			DP(stmt) = DP(stmt2);
			DP(stmt2) = stmtEX;
			DP(stmt)->nextNULL = DP(stmt2)->nextNULL;
			DP(stmt2)->nextNULL = NULL;
			KNH_FINALv(ctx, DP(stmt2)->stmtPOST); // GC stmt2
			return TM(stmt);
		}
	}
	DBG_P("STT=%s", TT__(TT_(DP(stmt)->terms[0])));
	TYPING(ctx, stmt, 0, reqt, 0);
	DBG_P("STT=%s", TT__(TT_(DP(stmt)->terms[0])));
	if(IS_Stmt(DP(stmt)->stmtPOST)) {
		return knh_Stmt_typed(ctx, stmt, TERMs_gettype(stmt, 0));
	}
	return DP(stmt)->terms[0];
}

/* ------------------------------------------------------------------------ */

static void KNH_BOX(Ctx *ctx, knh_sfp_t *sfp, knh_type_t type)
{
	knh_class_t cid = CLASS_type(type);
	DBG_ASSERT_cid(cid);
	knh_class_t bcid = ClassTBL(cid).bcid;
	if(bcid == CLASS_Int || bcid == CLASS_Float || bcid == CLASS_Boolean) {
		KNH_SETv(ctx, sfp[0].o, new_Object_boxing(ctx, cid, sfp));
	}
}

static void KNH_UNBOX(Ctx *ctx, knh_sfp_t *sfp)
{
	sfp[0].data = (sfp[0].i)->n.ivalue;
}

static knh_Term_t *knh_StmtCALL_toCONST(Ctx *ctx, knh_Stmt_t *stmt, knh_Method_t *mtd)
{
	BEGIN_LOCAL(ctx, lsfp, DP(stmt)->size + 1);
	if(knh_Method_isConst(mtd)) {
		size_t i;
		for(i = 1; i < DP(stmt)->size; i++) {
			knh_Token_t *tk = DP(stmt)->tokens[i];
			if(TT_(tk) != TT_CONST && TT_(tk) != TT_NULL) goto L_NONSTATIC;
			KNH_SETv(ctx, lsfp[i+2].o, DP(tk)->data);
			KNH_UNBOX(ctx, &lsfp[i+2]);
		}
		KNH_SCALL(ctx, lsfp, 0, mtd, (DP(stmt)->size - 2));
		KNH_BOX(ctx, &lsfp[0], knh_ParamArray_rtype(DP(mtd)->mp));
		DBG_P("STMT = %s TURNED INTO CONST", TT__(STT_(stmt)));
		END_LOCAL(ctx, lsfp);
		return ((DP(mtd)->mp)->rsize == 0) ?
			knh_Stmt_done(ctx, stmt) :
			knh_Token_setCONST(ctx, DP(stmt)->tokens[0], lsfp[0].o);
	}
	L_NONSTATIC:;
	END_LOCAL(ctx, lsfp);
	return TM(stmt);
}

/* ------------------------------------------------------------------------ */
/* [built-in function] */

static knh_Term_t* knh_StmtLIKELY_typing(Ctx *ctx, knh_Stmt_t *stmt)
{
	if(TERMs_typing(ctx, stmt, 2, TYPE_Boolean, _NOTCAST)) {
		return DP(stmt)->terms[2];
	}
	return NULL;
}

//static knh_Term_t* knh_StmtDOMAIN_typing(Ctx *ctx, knh_Stmt_t *stmt)
//{
//	if(TERMs_typing(ctx, stmt, 2, CLASS_Any, TCHECK_)) {
//		knh_class_t cid;
//		if(TERMs_isCID(stmt, 2)) {
//			cid = DP(DP(stmt)->tokens[2])->cid;
//		}
//		else {
//			cid = TERMs_getcid(stmt, 2);
//		}
//		DBG_ASSERT_cid(cid);
//		knh_Token_setCONST(ctx, DP(stmt)->tokens[1], UPCAST(new_Type(ctx, cid)));
//		DP(stmt)->size = 2;
//		knh_Token_toMTD(ctx, DP(stmt)->tokens[0], MN_domain, knh_getMethodNULL(ctx, CLASS_Class, MN_domain));
//		return knh_Stmt_typed(ctx, stmt, TYPE_cid(knh_class_Generics(ctx, CLASS_Array, cid, CLASS_Any)));
//	}
//	TODO();
//	return NULL;
//}

static knh_Term_t* knh_StmtDEFINED_typing(Ctx *ctx, knh_Stmt_t *stmt)
{
	knh_Token_t *tk = DP(stmt)->tokens[0];
	if(TERMs_typing(ctx, stmt, 2, TYPE_Any, _NOTWARN | _NOTCHECK)) {
		knh_Token_setCONST(ctx, tk, KNH_TRUE);
	}
	else {
		knh_Token_setCONST(ctx, tk, KNH_FALSE);
	}
	return TM(tk);
}

static knh_Term_t* knh_StmtTHIS_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_methodn_t mn)
{
	knh_class_t mtd_cid = DP(ctx->gma)->this_cid;
	knh_Token_t *tkMTD = DP(stmt)->tokens[0]; /* change */
	knh_Method_t *mtd;
	if(mn == MN_super) {
		DBG_ASSERT_cid(mtd_cid);
		if(ClassTBL(mtd_cid).supcid == CLASS_Object) {
			knh_Gamma_perror(ctx, KERR_ERR, _("not extended: %C"), mtd_cid);
			return NULL;
		}
		mtd_cid = ClassTBL(mtd_cid).supcid;
	}
	mtd = knh_getMethodNULL(ctx, mtd_cid, MN_new);
	if(mtd == NULL || DP(mtd)->cid != mtd_cid) {
		knh_Gamma_perror(ctx, KERR_ERR, _("undefined constructor: %C"), mtd_cid);
		return NULL;
	}
	knh_Token_toTYPED(ctx, DP(stmt)->tokens[1], TT_LOCAL, TYPE_cid(mtd_cid), 0); /* this */
	knh_Token_toMTD(ctx, tkMTD, MN_new, mtd);
	return TM(stmt);
}

//static knh_Term_t *knh_StmtTRACE_typing(Ctx *ctx, knh_Stmt_t *stmt)
//{
//	/* trace(funcname, 1) */
//	if(TERMs_typing(ctx, stmt, 2, TYPE_Method, TCHECK_)) {
//
//	}
//	if(TERMs_typing(ctx, stmt, 3, TYPE_Int, TCHECK_)) {
//
//	}
//	knh_Gamma_perror(ctx, KERR_ERR, _("syntax error: %L"), DP(stmt)->tokens[0]);
//	return NULL;
//}

static knh_Term_t* knh_StmtDELEGATE_typing(Ctx *ctx, knh_Stmt_t *stmt)
{
	if(DP(stmt)->size == 4) {
		knh_Token_t *tkMN = DP(stmt)->tokens[3];
		TYPING(ctx, stmt, 2, TYPE_Any, _NOTCHECK);
		if(TERMs_isCID(stmt, 2)) { /* delegate(Class, f) */
			knh_Token_t *tkC = DP(stmt)->tokens[2];
			knh_Method_t *mtd = knh_getMethodNULL(ctx, DP(tkC)->cid, knh_Token_getmn(ctx, tkMN));
			if(mtd == NULL) {
				knh_Gamma_perror(ctx, KERR_ERR, _("undefined method: %L"), tkMN);
				return NULL;
			}
			if(!knh_Method_isStatic(mtd)) {
				knh_Gamma_perror(ctx, KERR_ERR, _("method is not static: %L"), tkMN);
				return NULL;
			}
			{
				knh_class_t cid = knh_class_FuncType(ctx, DP(ctx->gma)->this_cid, mtd);
				return knh_Token_setCONST(ctx, tkMN, new_StaticFunc(ctx, cid, mtd));
			}
		}
		else {
			knh_class_t cid = TERMs_getcid(stmt, 2);
			knh_Method_t *mtd = knh_getMethodNULL(ctx, cid, knh_Token_getmn(ctx, tkMN));
			if(mtd == NULL) {
				knh_Gamma_perror(ctx, KERR_ERR, _("undefined method: %L"), tkMN);
				return NULL;
			}
			cid = knh_class_FuncType(ctx, DP(ctx->gma)->this_cid, mtd);
			if(knh_Method_isStatic(mtd)) {
				cid = knh_class_FuncType(ctx, DP(ctx->gma)->this_cid, mtd);
				return knh_Token_setCONST(ctx, tkMN, new_StaticFunc(ctx, cid, mtd));
			}
			knh_Token_toTYPED(ctx, DP(stmt)->tokens[1], TT_CID, TYPE_Class, cid);
			knh_Token_setCONST(ctx, DP(stmt)->tokens[3], mtd);
			STT_(stmt) = STT_NEW;
			mtd = knh_getMethodNULL(ctx, CLASS_Func, MN_new);
			DBG_ASSERT(mtd != NULL);
			knh_Token_toMTD(ctx, DP(stmt)->tokens[0], DP(mtd)->mn, mtd);
			return knh_Stmt_typed(ctx, stmt, cid);
		}
	}
	else {
		knh_Gamma_perror(ctx, KERR_ERR, _("delegate must take (expr, funcname)"));
		return NULL;
	}
}
//
//static knh_Method_t* knh_lookupDynamicMethod(Ctx *ctx, knh_methodn_t mn)
//{
//	size_t cid, i, size = ctx->share->ClassTBLSize;
//	for(cid = 0; cid < size; cid++) {
//		knh_Array_t *a = ClassTBL(cid).methods;
//		for(i = 0; i < knh_Array_size(a); i++) {
//			if(DP(a->methods[i])->mn == mn) return a->methods[i];
//		}
//	}
//	return NULL;
//}

static knh_Term_t *knh_StmtFUNC_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	TYPING(ctx, stmt, 0, TYPE_Any, 0);
	knh_class_t cid = TERMs_getcid(stmt, 0);
	if(!IS_Tfunc(cid)) {
		knh_Gamma_perror(ctx, KERR_ERR, _("not Func: %C"), cid);
		return NULL;
	}
	else {
		size_t i;
		knh_ParamArray_t *pa = ClassTBL(cid).cparam;
		knh_Method_t *mtd = knh_getMethodNULL(ctx, cid, MN_invoke);
		DBG_ASSERT(mtd != NULL);
		/* 0 1 2 3 4 .. 5 */
		knh_Stmt_swap(ctx, stmt, 0, 1);
		DBG_ASSERT(TT_(DP(stmt)->tokens[0]) == TT_ASIS);
		knh_Token_toMTD(ctx, DP(stmt)->tokens[0], MN_invoke, mtd);
		for(i = 0; i < pa->psize; i++) {
			knh_param_t *p = knh_ParamArray_get(pa, i);
			knh_type_t type = GMA_type(ctx, p->type);
			DBG_P("i=%d, type=%s", (int)i, CLASS__(type));
			if(2 + i < DP(stmt)->size) {
				TYPING(ctx, stmt, 2 + i, type, 0);
			}
			else {
				knh_Stmt_add(ctx, stmt, new_TokenTYPED(ctx, TT_NULL/*DEFVAL*/, type, CLASS_type(type)));
			}
		}
		if(pa->psize + 2 < DP(stmt)->size) {
			knh_Gamma_perror(ctx, KERR_DWARN, _("too many parameters: %C"), cid);
			knh_Stmt_trimSize(ctx, stmt, pa->psize);
		}
		reqt = GMA_type(ctx, knh_ParamArray_rtype(pa));
	}
	return knh_Stmt_typed(ctx, stmt, reqt);
}

static knh_Term_t *knh_StmtCALL_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_class_t reqt)
{
	knh_Token_t *tkM = DP(stmt)->tokens[0];
	knh_Token_t *tkO = DP(stmt)->tokens[1];
	if(TT_(tkM) == TT_UNAME && TT_(tkO) == TT_UNAME) {  // CLASS CONST
		knh_class_t cid = knh_Token_getcid(ctx, tkO, CLASS_unknown);
		if(cid == CLASS_unknown) {
			knh_Gamma_perror(ctx, KERR_TERROR, "unknown class: %L", tkO);
			return NULL;
		}
		else {
			knh_Object_t *v = knh_getClassConstNULL(ctx, cid, S_tobytes(DP(tkM)->text));
			if(DP(stmt)->size == 2) { /* .MAX Int */
				if(v == NULL) {
					knh_Gamma_perror(ctx, KERR_TERROR, "undefined const: %L.%B", tkO, S_tobytes(DP(tkM)->text));
					return NULL;
				}
				return knh_Token_setCONST(ctx, tkM, v);
			}
			else {
				DBG_ASSERT(DP(stmt)->size == 3);
				if(v != NULL) {
					if(!knh_StmtMETA_is(ctx, stmt, STEXT("Once"))) {
						knh_Gamma_perror(ctx, KERR_DWARN, "defined: %L.%B = %O", tkO, S_tobytes(DP(tkM)->text), v);
					}
					return knh_Token_setCONST(ctx, tkM, v);
				}
				knh_Token_setCONST(ctx, tkM, new_Type(ctx, cid));
				//DBG_P("tkM=%p, TT=%s", tkM, TT__(tkM));
				knh_Stmt_insert(ctx, stmt, 0, new_TokenMN(ctx, MN_setConst));
				knh_Stmt_swap(ctx, stmt, 1, 2);
				return knh_StmtCALL_typing(ctx, stmt, reqt);
			}
		}
	}
	else {
		knh_methodn_t mn = knh_Token_getmn(ctx, tkM);
		knh_Method_t *mtd = NULL;
		knh_class_t mtd_cid = CLASS_Object;
		if(MN_isNEW(mn)) { /* reported by Dr. Maeda */
			knh_Gamma_perror(ctx, KERR_ERR, _("don't call a %M method"), mn);
			return NULL;
		}
		if(TT_(tkO) == TT_ASIS) {  /* (func _ ...) */ /* built-in function */
			switch(mn) {
				case MN_likely:
				case MN_unlikely: return knh_StmtLIKELY_typing(ctx, stmt);
//				case MN_format:  return knh_StmtFFORMAT_typing(ctx, stmt);
//				case MN_domain:  return knh_StmtDOMAIN_typing(ctx, stmt);
				case MN_defined: return knh_StmtDEFINED_typing(ctx, stmt);
				case MN_this:
				case MN_super: return knh_StmtTHIS_typing(ctx, stmt, mn);
				case MN_delegate: return knh_StmtDELEGATE_typing(ctx, stmt);
			}
			mtd_cid = knh_NameSpace_getFuncClass(ctx, DP(ctx->gma)->ns, mn);
			if(mtd_cid != CLASS_unknown) {
				mtd = knh_getMethodNULL(ctx, mtd_cid, mn);
				if(mtd != NULL) {
					knh_Token_toTYPED(ctx, tkO, TT_NULL/*DEFVAL*/, mtd_cid, mtd_cid);
				}
			}
			if(mtd == NULL) {
				mtd_cid = DP(ctx->gma)->this_cid;
				mtd = knh_getMethodNULL(ctx, mtd_cid, mn);
				if(mtd != NULL) {
					knh_Token_toTYPED(ctx, tkO, TT_LOCAL, mtd_cid, 0);
				}
			}
			if(mtd == NULL) {
				mtd_cid = knh_Object_cid(knh_getGammaScript(ctx));
				mtd = knh_getMethodNULL(ctx, mtd_cid, mn);
				if(mtd != NULL) {
					knh_Token_toTYPED(ctx, tkO, TT_NULL/*DEFVAL*/, mtd_cid, mtd_cid);
				}
			}
			if(mtd == NULL) {
				STT_(stmt) = STT_FUNC;
				TT_(DP(stmt)->tokens[0]) = TT_NAME;
				return knh_StmtFUNC_typing(ctx, stmt, reqt);
			}
		}
		else {
			TYPING(ctx, stmt, 1, TYPE_Any, _NOTCHECK);
			if(TERMs_isCID(stmt, 1)) {
				DBG_P("**** BASE CLASS *******");
				knh_Token_toTYPED(ctx, tkO, TT_NULL/*DEFVAL*/, DP(tkO)->cid, DP(tkO)->cid);
			}
			mtd_cid = TERMs_getcid(stmt, 1);
			DBG_P("@@@@@@ mtd_cid=%s", CLASS__(mtd_cid));
			mtd = knh_getMethodNULL(ctx, mtd_cid, mn);
		}
		knh_Token_toMTD(ctx, tkM, mn, mtd);
		if(mtd == NULL) {
			long i;
			for(i = 2; i < DP(stmt)->size; i++) {
				TYPING(ctx, stmt, i, TYPE_Any, _NOTCHECK);
			}
			if(mtd_cid != TYPE_Any) {
				knh_Gamma_perror(ctx, KERR_ERR, _("undefined method: %C.%M"), mtd_cid, mn);
				return NULL;
			}
//			if(mtd_cid != TYPE_Any) {
//				knh_ParamArray_t *pa = new_ParamArray(ctx);
//				knh_Gamma_perror(ctx, KERR_DWARN, _("undefined method: %C.%M"), mtd_cid, mn);
//				mtd = new_Method(ctx, 0, mtd_cid, mn, NULL);
//				KNH_SETv(ctx, DP(mtd)->mp, pa);
//				for(i = 2; i < DP(stmt)->size; i++) {
//					knh_param_t p = {TERMs_gettype(stmt, i), FN_};
//					knh_ParamArray_add(ctx, pa, p);
//				}
//				if(reqt != TYPE_void) {
//					knh_param_t p = {reqt, FN_return};
//					knh_ParamArray_radd(ctx, pa, p);
//				}
//				knh_Token_toMTD(ctx, tkM, mn, mtd);
//				return knh_Stmt_typed(ctx, stmt, reqt);
//			}
			return knh_Stmt_typed(ctx, stmt, TYPE_Any);
		}
		return knh_StmtPARAMs_typing(ctx, stmt, mtd_cid, mtd);
	}
}

static knh_Term_t *knh_StmtPARAMs_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_class_t mtd_cid, knh_Method_t *mtd)
{
	size_t i, size = DP(stmt)->size;
	knh_ParamArray_t *pa = DP(mtd)->mp;
	knh_type_t rtype = knh_type_tocid(ctx, knh_ParamArray_rtype(pa), mtd_cid);
	knh_Stmt_typed(ctx, stmt, rtype);
	for(i = 0; i < pa->psize; i++) {
		knh_type_t reqt = knh_ParamArray_getptype(pa, i);
		reqt = knh_type_tocid(ctx, reqt, mtd_cid); //GMA_type(ctx, p->type);
		size_t n = i + 2;
		if(n < size) {
			TYPING(ctx, stmt, n, reqt, 0);
		}
		else {
			if(!(n < size) && !knh_ParamArray_isVARGs(pa)) {
				knh_Stmt_add(ctx, stmt, new_TokenTYPED(ctx, TT_NULL/*DEFVAL*/, reqt, CLASS_type(reqt)));
			}
		}
	}
	if(knh_ParamArray_isVARGs(pa)) {
		knh_type_t reqt = knh_ParamArray_getptype(pa, i);
		reqt = knh_type_tocid(ctx, reqt, mtd_cid); //GMA_type(ctx, p->type);
		for(i = pa->psize; i + 2 < size; i++) {
			TYPING(ctx, stmt, i+1, reqt, 0);
		}
	}
	else if(i + 2 < size) {
		knh_Gamma_perror(ctx, KERR_DWARN, _("too many parameters: %C.%M"), mtd_cid, DP(mtd)->mn);
		knh_Stmt_trimSize(ctx, stmt, i+2);
	}
	return knh_StmtCALL_toCONST(ctx, stmt, mtd);
}

/* ------------------------------------------------------------------------ */
/* [NEW] */

//static knh_type_t knh_class_getFieldType(Ctx *ctx, knh_class_t cid, knh_bytes_t name)
//{
//	knh_fieldn_t fn = FN_UNMASK(knh_getfnq(ctx, name, FN_NONAME));
//	if(fn == FN_NONAME) return TYPE_Any;
//	knh_index_t idx = knh_Class_queryField(ctx, cid, fn);
//	if(idx != -1) {
//		knh_fields_t *cf = knh_Class_fieldAt(ctx, cid, idx);
//		return knh_type_tocid(ctx, cf->type, cid);
//	}
//	else {
//		knh_Method_t *mtd = knh_getMethodNULL(ctx, cid, MN_toSETTER(fn));
//		if(IS_NOTNULL(mtd) && knh_Method_psize(mtd) == 1) {
//			return knh_Method_ptype(ctx, mtd, cid, 0);
//		}
//	}
//	return TYPE_Any;
//}

knh_class_t knh_class_parent(Ctx *ctx, knh_class_t c1, knh_class_t c2)
{
	knh_class_t p1, p2;
	DBG_ASSERT_cid(c1);
	DBG_ASSERT_cid(c2);
	if(c1 == c2) return c1;
	p1 = ClassTBL(c1).supcid;
	p2 = ClassTBL(c2).supcid;
	if(p1 == c2 || p1 == p2) return p1;
	if(p2 == c1) return p2;
	if(ClassTBL(c1).bcid == ClassTBL(c2).bcid) return ClassTBL(c1).bcid;
	TODO();
	return CLASS_Object;
}

static knh_type_t knh_Stmt_findCommonType(Ctx *ctx, knh_Stmt_t *stmt, int isDICTMAP)
{
	size_t i, step = isDICTMAP ? 2 : 1;
	knh_type_t vart = TYPE_Object;
	for(i = 2; i < DP(stmt)->size; i+= step) {
		knh_type_t t = TERMs_gettype(stmt, i);
		if(t == TYPE_Any) return t;
		if(t == vart || knh_class_instanceof(ctx, t, vart)) {
			vart = t;
			continue;
		}

	}
	if(vart == TYPE_Object) vart = TYPE_Any;
	return vart;
}

static knh_Term_t *knh_StmtNEW_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_class_t reqt)
{
	knh_Token_t *tkNEW = DP(stmt)->tokens[0];
	knh_Token_t *tkC = DP(stmt)->tokens[1];
	knh_methodn_t mn = knh_Token_getmn(ctx, tkNEW);
	knh_class_t bcid, mtd_cid = knh_Token_getcid(ctx, tkC, CLASS_unknown);
	if(TT_(tkC) == TT_ASIS) { /* new () */
		if(reqt == TYPE_Any || reqt == TYPE_void) {
			knh_Gamma_perror(ctx, KERR_TERROR, _("needs class"));
			return NULL;
		}
		mtd_cid = CLASS_type(reqt);
		knh_Token_toTYPED(ctx, tkC, TT_CID, CLASS_Class, mtd_cid);
	}
	if(mtd_cid == CLASS_unknown) { /* new UnknownClass(...) */
		if(reqt == TYPE_Any || reqt == TYPE_void) {
			knh_Gamma_perror(ctx, KERR_ERR, _("unknown class: %L"), tkC);
			return NULL;
		}
		mtd_cid = CLASS_type(reqt);
		knh_Token_toTYPED(ctx, tkC, TT_CID, CLASS_Class, mtd_cid);
	}

	bcid = knh_class_bcid(mtd_cid);  /* String[] s = new Array() */
	if(knh_class_isGenerics(ctx, mtd_cid) && bcid == mtd_cid) {
		if(bcid == knh_class_bcid(CLASS_type(reqt))) {
			mtd_cid = CLASS_type(reqt);
			knh_Token_toTYPED(ctx, tkC, TT_CID, CLASS_Class, mtd_cid);
		}
	}
	if(mn == MN_new__MAP) {
		size_t i;
		knh_type_t p1 = knh_class_p1(mtd_cid);
		for(i = 2; i < DP(stmt)->size; i+= 2) {
			TYPING(ctx, stmt, i, CLASS_String, _NOTCAST);
			TYPING(ctx, stmt, i+1, p1, 0);
		}
		if(p1 == TYPE_Any) {
			p1 = knh_Stmt_findCommonType(ctx, stmt, 1/*isDictMap*/);
			if(p1 != TYPE_Any) {
				mtd_cid = knh_class_P1(ctx, knh_class_bcid(mtd_cid), p1);
			}
		}
		knh_Token_toTYPED(ctx, tkC, TT_CID, CLASS_Class, mtd_cid);
		goto L_LOOKUPMETHOD;
	}

	if(mn == MN_new__LIST) {
		size_t i;
		knh_type_t p1 = knh_class_p1(mtd_cid);
		for(i = 2; i < DP(stmt)->size; i++) {
			TYPING(ctx, stmt, i, p1, 0);
		}
		if(p1 == TYPE_Any) {
			p1 = knh_Stmt_findCommonType(ctx, stmt, 0);
			if(p1 != TYPE_Any) {
				mtd_cid = knh_class_P1(ctx, knh_class_bcid(mtd_cid), p1);
			}
		}
		knh_Token_toTYPED(ctx, tkC, TT_CID, CLASS_Class, mtd_cid);
		goto L_LOOKUPMETHOD;
	}

	if(mtd_cid == CLASS_Exception) {
		knh_ebi_t eid = knh_geteid(ctx, S_tobytes(DP(tkC)->text), EBI_unknown);
		if(eid == EBI_unknown) {
			knh_Gamma_perror(ctx, KERR_DWARN, "undefined exception event: %B", S_tobytes(DP(tkC)->text));
		}
		knh_Token_toCONST(ctx, tkC);
		tkC = new_TokenCID(ctx, CLASS_Exception);
		knh_Token_toTYPED(ctx, tkC, TT_CID, CLASS_Class, mtd_cid);
		knh_Stmt_insert(ctx, stmt, 1, tkC);
		DBG_ASSERT(TT_(DP(stmt)->terms[2]) == TT_CONST);
		DBG_ASSERT((DP(stmt)->terms[2])->type == CLASS_String);
		goto L_LOOKUPMETHOD;
	}

	L_LOOKUPMETHOD:
	DBG_ASSERT_cid(mtd_cid);
	{
		knh_Method_t *mtd = knh_getMethodNULL(ctx, mtd_cid, mn);
		if(mtd == NULL || ClassTBL(DP(mtd)->cid).bcid != ClassTBL(mtd_cid).bcid) {
			knh_Gamma_perror(ctx, KERR_ERR, _("unknown constructor: %L %C(...)"), tkNEW, mtd_cid);
			return NULL;
		}
		knh_Token_toMTD(ctx, tkNEW, mn, mtd);
		knh_Token_setCONST(ctx, tkC, new_Type(ctx, mtd_cid));
		return knh_StmtPARAMs_typing(ctx, stmt, mtd_cid, mtd);
	}
}

/* ------------------------------------------------------------------------ */
/* [OP] */

static knh_class_t knh_StmtopADD_basecid(Ctx *ctx, knh_Stmt_t *stmt)
{
	knh_class_t cid1 = TERMs_getcid(stmt, 1);
	knh_class_t cid2 = TERMs_getcid(stmt, 2);
	knh_class_t bcid1 = knh_class_bcid(cid1);
	knh_class_t bcid2 = knh_class_bcid(cid2);
	if(cid1 == CLASS_Any || cid2 == CLASS_Any) return CLASS_Any;
	if(cid1 == cid2) return cid1;
	if(bcid1 == bcid2) return bcid1;
	if(bcid1 == CLASS_Float && bcid2 == CLASS_Int) return cid1;
	if(bcid2 == CLASS_Float && bcid1 == CLASS_Int) return cid2;
	if(bcid1 == CLASS_String || bcid2 == CLASS_String) return CLASS_String;
	return cid1;
}

static knh_class_t knh_StmtopEQ_basecid(Ctx *ctx, knh_Stmt_t *stmt)
{
	knh_class_t cid1 = TERMs_getcid(stmt, 1);
	knh_class_t cid2 = TERMs_getcid(stmt, 2);
	knh_class_t bcid1 = knh_class_bcid(cid1);
	knh_class_t bcid2 = knh_class_bcid(cid2);
	if(cid1 == cid2) return cid1;
	if(cid1 == CLASS_Any || cid2 == CLASS_Any) return CLASS_Any;
	if(bcid1 == cid2) return bcid1;
	if(bcid2 == cid1) return bcid2;
	if(bcid1 == CLASS_Float && bcid2 == CLASS_Int) return cid1;
	if(bcid2 == CLASS_Float && bcid1 == CLASS_Int) return cid2;
	return CLASS_unknown;
}

/* ------------------------------------------------------------------------ */

//static int knh_Gamma_isTypedBINARYOP(Ctx *ctx, knh_Stmt_t *stmt)
//{
//	DBG_ASSERT(DP(stmt)->size == 3);
//	knh_type_t t1 = TERMs_gettype(stmt, 1);
//	knh_type_t t2 = TERMs_gettype(stmt, 2);
//	//DBG_P("TYPEINFERENCE %s%s %s%s", TYPEQN(t1), TYPEQN(t2));
//	if(t1 == TYPE_var) {
//		if(t2 == TYPE_var || t2 == TYPE_Any) {
//			return 0; // untyped
//		}
//		if(IS_Token(DP(stmt)->terms[1])) {
//			knh_Token_t *tkVAR = DP(stmt)->tokens[1];
//			DBG_ASSERT(knh_Token_isUntyped(tkVAR));
//			knh_Gamma_derivedParamType(ctx, tkVAR, t2);
//			return 1;
//		}
//		return 0;
//	}
//	if(t2 == TYPE_var) {
//		if(t1 == TYPE_Any) {
//			return 0; // untyped
//		}
//		if(IS_Token(DP(stmt)->terms[2])) {
//			knh_Token_t *tkVAR = DP(stmt)->tokens[2];
//			DBG_ASSERT(knh_Token_isUntyped(tkVAR));
//			knh_Gamma_derivedParamType(ctx, tkVAR, t1);
//			return 1;
//		}
//		return 0;
//	}
//	return 1;
//}

static knh_Term_t *knh_StmtOP_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	size_t i, opsize = DP(stmt)->size - 1;
	knh_Token_t *tkOP = DP(stmt)->tokens[0];
	knh_class_t mtd_cid = CLASS_Any;
	knh_methodn_t mn = TT_(tkOP) - TT_LET;
	if(TT_isBINARY(TT_(tkOP)) && opsize != 2) {
		knh_Gamma_perror(ctx, KERR_ERR, "%s must be binary operator", knh_getopname(mn));
		return NULL;
	}
	if(TT_(tkOP) == TT_EXISTS) mtd_cid = CLASS_String;
	for(i = 1; i < opsize + 1; i++) {
		TYPING(ctx, stmt, i, mtd_cid, 0);
	}
	if(mn == MN_opEQ || mn == MN_opNOTEQ) {
		knh_term_t tt = TT_TERMs(stmt, 1);
		DBG_P("****** tm1=%s, tm2=%s", TERMs__(stmt, 1), TERMs__(stmt, 2));
		if(tt == TT_NULL || tt == TT_TRUE || tt == TT_FALSE || tt == TT_CONST) {
			knh_Stmt_swap(ctx, stmt, 1, 2);
		}
		if(TERMs_isNULL(stmt, 2)) { /* o == null, o != null */
			knh_class_t cid = TERMs_getcid(stmt, 1);
			mn = (mn == MN_opEQ) ? MN_isNull : MN_isNotNull;
			if(IS_Tunbox(cid)) {
				knh_Gamma_perror(ctx, KERR_DWARN, "%C doesn't take null", cid);
			}
			mtd_cid = CLASS_Object;
			knh_Stmt_trimToSize(ctx, stmt, 2);
			goto L_LOOKUPMETHOD;
		}
	}
	else {
		//		if(knh_Gamma_isTypedBINARYOP(ctx, stmt)) {
		//			return knh_Stmt_untyped(ctx, stmt);
		//		}
	}
	for(i = 1; i < opsize + 1; i++) {
		if(TT_(DP(stmt)->terms[i]) == TT_NULL) {
			knh_Gamma_perror(ctx, KERR_ERR,
				_("undefined behavior for null"));
			return NULL;
		}
	}

	switch(mn) {
	case MN_opADD: /* a + b*/
	case MN_opSUB: /* a - b */
	case MN_opMUL: /* a * b */
	case MN_opDIV: /* a / b */
	case MN_opMOD: /* a % b */
	{
		mtd_cid = knh_StmtopADD_basecid(ctx, stmt);
		if(mtd_cid == CLASS_String && mn == MN_opADD) {
			knh_Token_t *tk = new_(Token);
			TT_(tk) = TT_ASIS;
			knh_Stmt_insert(ctx, stmt, 1, tk);
			STT_(stmt) = STT_W;
			DP(stmt)->wstart = 2;
			return knh_Stmt_typed(ctx, stmt, TYPE_String);
		}
		goto L_LOOKUPMETHOD;
	}
	case MN_opPLUS:
		return DP(stmt)->terms[1];

	case MN_opEQ: case MN_opNOTEQ:
	{
		if(TERMs_isTRUE(stmt, 1) || TERMs_isFALSE(stmt, 1)) {
			knh_Stmt_swap(ctx, stmt, 1, 2);
		}
		if((TERMs_isTRUE(stmt, 2) && (mn == MN_opEQ)) /* b == true */
		|| (TERMs_isFALSE(stmt, 2) && (mn == MN_opNOTEQ))) {  /* b != false */
			TYPING(ctx, stmt, 1, TYPE_Boolean, 0);
			return DP(stmt)->terms[1];
		}
		if((TERMs_isTRUE(stmt, 2) && (mn == MN_opNOTEQ)) /* b != true */
		|| (TERMs_isFALSE(stmt, 2) && (mn == MN_opEQ))) {  /* b == false */
			TYPING(ctx, stmt, 1, TYPE_Boolean, 0);
			mn = MN_opNOT; mtd_cid = CLASS_Boolean;
			knh_Stmt_trimToSize(ctx, stmt, 2);
			goto L_LOOKUPMETHOD;
		}
		mtd_cid = knh_StmtopEQ_basecid(ctx, stmt);
		goto L_LOOKUPMETHOD;
	}
	case MN_opGT: case MN_opGTE: case MN_opLT: case MN_opLTE:
	{
		mtd_cid = knh_StmtopEQ_basecid(ctx, stmt);
		if(mtd_cid == CLASS_unknown) {
			knh_Gamma_perror(ctx, KERR_TERROR,
				_("comparison of different type: %T %T"), TERMs_gettype(stmt, 1), TERMs_gettype(stmt, 2));
			return NULL;
		}
		goto L_LOOKUPMETHOD;
	}
	case MN_opHAS:
	{
		knh_Stmt_swap(ctx, stmt, 1, 2);
		mtd_cid = TERMs_getcid(stmt, 1);
		goto L_LOOKUPMETHOD;
	}

	case MN_opSEND:
	{
		mtd_cid = TERMs_getcid(stmt, 1);
		if(IS_Tint(mtd_cid)) {
			mn = MN_opLSFT;
		}
		if(mtd_cid == CLASS_OutputStream) {
//			knh_Method_t *mtd = knh_getMethodNULL(ctx, mtd_cid, mn);
//			knh_Token_toMTD(ctx, tkOP, mn, mtd);
			TYPING(ctx, stmt, 1, mtd_cid, 0);
			STT_(stmt) = STT_W;
			DP(stmt)->wstart = 2;
			return knh_Stmt_typed(ctx, stmt, TYPE_void);
		}
		goto L_LOOKUPMETHOD;
	}

	case MN_opOF:
	{
		knh_Token_t *tkC = DP(stmt)->tokens[2];
		knh_class_t cid = CLASS_type((tkC)->type);
		mtd_cid = TERMs_getcid(stmt, 1);
		if(TT_(tkC) == TT_CID) {
			if(mtd_cid == CLASS_Exception) {
				mtd_cid = CLASS_Object;
			}
			cid = DP(tkC)->cid;
		}
		if(mtd_cid == CLASS_Exception){
			goto L_LOOKUPMETHOD;
		}
		//DBG_P("%s instanceof %s true? %d", TYPE__(mtd_cid), TYPE__(cid), knh_class_instanceof(ctx, mtd_cid, cid));
		if(mtd_cid == cid || cid == CLASS_Any) {
			//knh_Gamma_perror(ctx, KERR_DWARN, _("always true: %C instanceof %C"), mtd_cid, cid);
			return knh_Token_setCONST(ctx, tkC, KNH_TRUE);
		}
		if(!knh_class_instanceof(ctx, cid, mtd_cid)) {
			//knh_Gamma_perror(ctx, KERR_DWARN, _("always false: %C instanceof %C"), mtd_cid, cid);
			return knh_Token_setCONST(ctx, tkC, KNH_FALSE);
		}
		goto L_LOOKUPMETHOD;
	}

	case MN_opEXISTS:
	{
		knh_Stmt_add(ctx, stmt, new_TokenCONST(ctx, DP(ctx->gma)->ns));
		goto L_LOOKUPMETHOD;
	}


	default:
		mtd_cid = TERMs_getcid(stmt, 1);
		break;
	}

	L_LOOKUPMETHOD:;
	if(mtd_cid == CLASS_unknown) {
		knh_Gamma_perror(ctx, KERR_TERROR,
			_("comparison of different type: %T %T"), TERMs_gettype(stmt, 1), TERMs_gettype(stmt, 2));
		return NULL;
	}
	else {
		knh_Method_t *mtd = knh_getMethodNULL(ctx, mtd_cid, mn);
		if(mtd == NULL) {
			knh_Gamma_perror(ctx, KERR_ERR, _("unsupported %s in %C"), knh_getopname(mn), mtd_cid);
			return NULL;
		}
		knh_Token_toMTD(ctx, tkOP, mn, mtd);
		TYPING(ctx, stmt, 1, mtd_cid, 0);
		return knh_StmtPARAMs_typing(ctx, stmt, mtd_cid, mtd);
	}
}

/* ------------------------------------------------------------------------ */
/* [TCAST] */

static void knh_Token_toMPR(Ctx *ctx, knh_Token_t *tk, knh_class_t cid, knh_Translator_t *trlNULL)
{
	DP(tk)->cid = cid;
	if(trlNULL != NULL) {
		KNH_SETv(ctx, DP(tk)->data, trlNULL);
	}
}

static knh_Term_t *knh_StmtTCAST_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	knh_Token_t *tkC = DP(stmt)->tokens[0];
	knh_class_t scid, tcid = knh_Token_getcid(ctx, tkC, CLASS_unknown);
	if(tcid == CLASS_unknown) {
		knh_Gamma_perror(ctx, KERR_ERR, _("unknown class: %L"), tkC);
		return NULL;
	}
	if(tcid == CLASS_Any) {   /* (Any)expr */
		tcid = CLASS_type(reqt);
		if(tcid == CLASS_Any) {
			TYPING(ctx, stmt, 1, TYPE_Any, _NOTCHECK);
			return DP(stmt)->terms[1];
		}
	}
	DBG_P("TCID(reqt)= %s", CLASS__(tcid));
	TYPING(ctx, stmt, 1, tcid, _NOTCHECK | _NOTCAST);
	scid = TERMs_getcid(stmt, 1);
	if(scid == CLASS_Any) {     /* (T)anyexpr */
		knh_Token_toMPR(ctx, tkC, tcid, NULL);
		return knh_Stmt_typed(ctx, stmt, tcid);
	}
	if(tcid == scid || knh_class_instanceof(ctx, scid, tcid)) {
		if(!TERMs_isCONST(stmt, 1) && tcid == scid) {
			knh_Gamma_perror(ctx, KERR_DWARN, _("upcast (%C)expr of %C"), tcid, scid);
		}
		return DP(stmt)->terms[1];
	}

	if(knh_class_instanceof(ctx, tcid, scid)) {  /* downcast */
		knh_Stmt_setDOWNCAST(stmt, 1);
		knh_Token_toMPR(ctx, tkC, tcid, NULL);
		knh_Gamma_perror(ctx, KERR_DWARN, _("downcast (%C)expr of %C"), tcid, scid);
		return knh_Stmt_typed(ctx, stmt, tcid);
	}

	{
		knh_Translator_t *trl = knh_getTranslatorNULL(ctx, scid, tcid);
		if(trl == NULL) {
			if(scid != CLASS_Any) {
				knh_Gamma_perror(ctx, KERR_ERR, _("no translation: %C ==> %C"), scid, tcid);
				return NULL;
			}
			knh_Token_toMPR(ctx, tkC, tcid, NULL);
			return knh_Stmt_typed(ctx, stmt, tcid);
		}
		if(knh_Translator_isConst(trl) && TERMs_isCONST(stmt, 1)) {
			BEGIN_LOCAL(ctx, lsfp, 1);
			knh_Token_t *tk1 = DP(stmt)->tokens[1];
			KNH_SETv(ctx, lsfp[0].o, DP(tk1)->data);
			KNH_SCAST(ctx, lsfp, 0, trl);
			KNH_BOX(ctx, &lsfp[0], SP(trl)->tcid);
			DBG_P("TCAST TO CONST .. %s ==> %s, %s", CLASS__(SP(trl)->scid), CLASS__(SP(trl)->tcid), O__(lsfp[0].o));
			knh_Token_setCONST(ctx, tk1, lsfp[0].o);
			END_LOCAL(ctx, lsfp);
			return TM(tk1);
		}
		else {
			knh_Token_toMPR(ctx, tkC, tcid, trl);
			return knh_Stmt_typed(ctx, stmt, tcid);
		}
	}
}

/* ------------------------------------------------------------------------ */
/* [QCAST] */

static knh_Term_t* knh_StmtQCAST_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	knh_Token_t *tkQPATH = DP(stmt)->tokens[2];
	knh_bytes_t path = S_tobytes(DP(tkQPATH)->text);
	DBG_ASSERT(TT_(tkQPATH) == TT_QPATH);
	TYPING(ctx, stmt, 1, TYPE_String, 0);
	{
		knh_PathDSPI_t *dspi = knh_NameSpace_getPathDSPINULL(ctx, DP(ctx->gma)->ns, path);
		if(dspi == NULL) {
			knh_Gamma_perror(ctx, KERR_ERR, _("path may be uninstalled: %B"), path);
			return NULL;
		}
		else {
			if(reqt == TYPE_Any) {
				reqt = dspi->cid;
				knh_Gamma_perror(ctx, KERR_INFO, _("%B: can be %T"), path, reqt);
			}
			if(!dspi->isTyped(ctx, CLASS_type(reqt))) {
				knh_Gamma_perror(ctx, KERR_INFO, _("%B: must NOT be %T"), path, reqt);
				return NULL;
			}
		}
		DBG_ASSERT(DP(stmt)->size == 3);
		STT_(stmt) = STT_CALL;
		knh_Token_toMTD(ctx, DP(stmt)->tokens[0], MN_path, knh_getMethodNULL(ctx, CLASS_String, MN_path));
		TT_(tkQPATH) = TT_CONST;
		knh_Stmt_add(ctx, stmt, new_TokenCONST(ctx, DP(ctx->gma)->ns));
		knh_Stmt_add(ctx, stmt, new_TokenCONST(ctx, new_Type(ctx, reqt)));
		return knh_Stmt_typed(ctx, stmt, reqt);
	}
}

/* ======================================================================== */
/* [MT,AND,OR,] */


static knh_Term_t *knh_StmtAND_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	size_t i;
	for(i = 0; i < DP(stmt)->size; i++) {
		TYPING(ctx, stmt, i, TYPE_Boolean, _NOTCAST);
		if(TERMs_isFALSE(stmt, i)) {
			return new_TermCONST(ctx, KNH_FALSE);
		}
	}
	return knh_Stmt_typed(ctx, stmt, TYPE_Boolean);
}

static knh_Term_t *knh_StmtOR_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	size_t i;
	for(i = 0; i < DP(stmt)->size; i++) {
		TYPING(ctx, stmt, i, TYPE_Boolean, _NOTCAST);
		if(TERMs_isTRUE(stmt, i)) {
			return new_TermCONST(ctx, KNH_TRUE);
		}
	}
	return knh_Stmt_typed(ctx, stmt, TYPE_Boolean);
}

static knh_Term_t *knh_StmtALT_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	size_t i;
	DBG_ASSERT(DP(stmt)->size > 1);
	for(i = 0; i < DP(stmt)->size; i++) {
		TYPING(ctx, stmt, i, reqt, 0);
	}
	return knh_Stmt_typed(ctx, stmt, reqt);
}

static knh_Term_t *knh_StmtTRI_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	int res = 1;
	if(!TERMs_typing(ctx, stmt, 0, TYPE_Boolean, _NOTCAST)) res = 0;
	if(!TERMs_typing(ctx, stmt, 1, reqt, 0)) res = 0;
	if(!TERMs_typing(ctx, stmt, 2, reqt, 0)) res = 0;
	if(res == 0) return NULL;
	if(TERMs_isTRUE(stmt, 0)) return DP(stmt)->terms[1];
	if(TERMs_isFALSE(stmt, 0)) return DP(stmt)->terms[2];
	return knh_Stmt_typed(ctx, stmt, reqt);
}

/* ======================================================================== */

#define CASE_STMT(XX, ...) case STT_##XX : { \
		tm = knh_Stmt##XX##_typing(ctx, ## __VA_ARGS__); \
		break;\
	}\

static knh_Term_t *knh_StmtFUNCTION_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt);

static knh_Term_t *knh_StmtEXPR_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_class_t reqt)
{
	knh_Term_t *tm = NULL;
	switch(STT_(stmt)) {
		CASE_STMT(CALL1, stmt, reqt);
		CASE_STMT(LET, stmt, reqt, SCOPE_LOCAL);
		CASE_STMT(FUNC, stmt, reqt);
		CASE_STMT(CALL, stmt, reqt);
		CASE_STMT(NEW, stmt, reqt);
		CASE_STMT(OP, stmt, reqt);
		CASE_STMT(TCAST, stmt, reqt);
		CASE_STMT(QCAST, stmt, reqt);
		CASE_STMT(FMT, stmt, reqt);
		CASE_STMT(AND, stmt, reqt);
		CASE_STMT(OR, stmt, reqt);
		CASE_STMT(ALT, stmt, reqt);
		CASE_STMT(TRI, stmt, reqt);
		CASE_STMT(FUNCTION, stmt, reqt);
	default:
		DBG_P("undefined stmt=%s", cSTT_((stmt)));
	}
	if(tm == NULL) {
		knh_Stmt_toERR(ctx, stmt, NULL);
	}
	return tm;
}

/* ------------------------------------------------------------------------ */

static void TERMs_perrorTYPE(Ctx *ctx, knh_Stmt_t *stmt, size_t n, knh_type_t reqt)
{
	knh_type_t type = TYPE_void;
	if(n < DP(stmt)->size) {
		type = TERMs_gettype(stmt, n);
	}
	switch(SP(stmt)->stt) {
	case STT_NEW:
	{
		knh_Method_t *mtd = DP(DP(stmt)->tokens[0])->mtd;
		if(IS_Method(mtd)) {
			knh_Gamma_perror(ctx, KERR_TERROR,
					_("parameter %d of %M %C must be %T, not %T"), n - 1, DP(mtd)->mn,
					DP(mtd)->cid, reqt, type);
		}
		else {
			knh_Gamma_perror(ctx, KERR_TERROR,
					_("parameter %d must be %T, not %T"), n - 1, reqt, type);
		}
		break;
	}
	case STT_CALL:
	case STT_OP:
	{
		knh_Method_t *mtd = DP(DP(stmt)->tokens[0])->mtd;
		if(IS_Method(mtd)) {
			knh_Gamma_perror(ctx, KERR_TERROR, _("parameter %d of %M must be %T, not %T"), n - 1, DP(mtd)->mn, reqt, type);
		}
		else {
			knh_Gamma_perror(ctx, KERR_TERROR, _("parameter %d must be %T, not %T"), n - 1, reqt, type);
		}
		break;
	}
	case STT_LET:
	case STT_DECL:
		knh_Gamma_perror(ctx, KERR_TERROR, _("%L must be %T, not %T"), DP(stmt)->tokens[1], reqt, type);
		break;
	default :
		knh_Gamma_perror(ctx, KERR_TERROR, _("%s(%d) must be %T, not %T"), TT__(SP(stmt)->stt), n, reqt, type);
	}
}

static knh_Term_t *new_TermTCAST(Ctx *ctx, knh_class_t reqt, knh_Translator_t *trlNULL, knh_Token_t *tk2)
{
	if(TT_(tk2) == TT_CONST && trlNULL != NULL && knh_Translator_isConst(trlNULL)) {
		BEGIN_LOCAL(ctx, lsfp, 1);
		klr_mov(ctx, lsfp[0].o, DP(tk2)->data);
		KNH_SCAST(ctx, lsfp, 0, trlNULL);
		KNH_BOX(ctx, lsfp, SP(trlNULL)->tcid);
		knh_Token_setCONST(ctx, tk2, lsfp[0].o);
		END_LOCAL(ctx, lsfp);
		return TM(tk2);
	}
	else {
		knh_Stmt_t *cstmt = new_Stmt2(ctx, STT_TCAST, NULL);
		knh_Token_t *tk = new_TokenTYPED(ctx, TT_CID, TYPE_Class, CLASS_Any);
		knh_Token_toMPR(ctx, tk, reqt, trlNULL);
		knh_Stmt_add(ctx, cstmt, tk);
		knh_Stmt_add(ctx, cstmt, tk2);
		return knh_Stmt_typed(ctx, cstmt, reqt);
	}
}

#define ERROR_IF(c)     if(c)  goto L_ERROR;
#define PERROR_IF(c)    if(c)  goto L_PERROR;

static int TERMs_typing(Ctx *ctx, knh_Stmt_t *stmt, size_t n, knh_type_t reqt, knh_flag_t modeflag)
{
	knh_flag_t flag = DP(ctx->gma)->flag;
	int res = 1;
	ERROR_IF(STT_(stmt) == STT_ERR);
	if(FLAG_is(modeflag, _NOTWARN)) {
		knh_Gamma_setQuiet(ctx->gma, 1);
	}
	PERROR_IF(!(n < DP(stmt)->size));
	/* TYPING */{
		knh_Term_t *tmN = DP(stmt)->terms[n];
		knh_Term_t *tm = IS_Token(tmN)
			? knh_Token_typing(ctx, (knh_Token_t*)tmN, reqt) :
			  knh_Stmt_typing(ctx, (knh_Stmt_t*)tmN, reqt);
		ERROR_IF(tm == NULL);
		if(tmN != tm) {
			KNH_SETv(ctx, DP(stmt)->terms[n], tm);
		}
	}

	if(FLAG_is(modeflag, _NOTCHECK))  {
		goto L_RETURN;
	}
	if(FLAG_is(modeflag, _BCHECK)) {
		knh_class_t cid = TERMs_getcid(stmt, n);
		if(knh_class_bcid(cid) == reqt) goto L_RETURN;
		res = 0;
	}
	else { 	/*TYPECHECK*/
		knh_type_t vart = TERMs_gettype(stmt, n);
		if(vart == reqt || knh_class_instanceof(ctx, vart, reqt)) goto L_RETURN;
		res = 0;
	}
	if(FLAG_is(modeflag, _NOTCAST)) {
		goto L_RETURN;
	}
	else {
		knh_type_t vart = TERMs_gettype(stmt, n);
		res = 1;
		if(vart == TYPE_Any) {
			KNH_SETv(ctx, DP(stmt)->terms[n], new_TermTCAST(ctx, reqt, NULL, DP(stmt)->tokens[n]));
			goto L_RETURN;
		}
		else {
			knh_Translator_t *trl = knh_getTranslatorNULL(ctx, vart, reqt);
			if((vart == TYPE_Int && reqt == TYPE_Float) || (vart == TYPE_Float && vart == TYPE_Int)) {
				KNH_SETv(ctx, DP(stmt)->terms[n], new_TermTCAST(ctx, reqt, trl, DP(stmt)->tokens[n]));
				goto L_RETURN;
			}
			if(reqt == TYPE_Regex && vart == TYPE_String) {
				KNH_SETv(ctx, DP(stmt)->terms[n], new_TermTCAST(ctx, reqt, trl, DP(stmt)->tokens[n]));
				goto L_RETURN;
			}
			if((trl != NULL && knh_Translator_isSemantic(trl)) || FLAG_is(modeflag, _ICAST)) {
				KNH_SETv(ctx, DP(stmt)->terms[n], new_TermTCAST(ctx, reqt, trl, DP(stmt)->tokens[n]));
				goto L_RETURN;
			}
		}
		DBG_P("stt=%s n=%d, reqt=%s, vart=%s", TT__(SP(stmt)->stt), (int)n, TYPE__(reqt), TYPE__(vart));
	}

	L_PERROR:;
	TERMs_perrorTYPE(ctx, stmt, n, reqt);

	L_ERROR:;
	res = 0;
	knh_Stmt_toERR(ctx, stmt, NULL);

	L_RETURN:;
	DP(ctx->gma)->flag = flag;
	return res;
}

/* ------------------------------------------------------------------------ */

static void knh_Stmt_setESPIDX(Ctx *ctx, knh_Stmt_t *stmt)
{
	if(DP(stmt)->espidx == 0) {
		DP(stmt)->espidx = DP(ctx->gma)->espidx;
	}
}

static knh_Term_t *knh_StmtITR_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt);

#define TYPING_STMTs(ctx, stmt, n, reqt) \
	if(knh_StmtITR_typing(ctx, DP(stmt)->stmts[n], reqt) == NULL) return NULL;

static knh_Term_t* knh_Stmt_toBLOCK(Ctx *ctx, knh_Stmt_t *stmt, size_t n, knh_type_t reqt)
{
	DBG_ASSERT(DP(stmt)->size > 0);
	STT_(stmt) = STT_BLOCK;
	KNH_SETv(ctx, DP(stmt)->terms[0], DP(stmt)->terms[n]);
	knh_Stmt_trimToSize(ctx, stmt, 1);
	return knh_Stmt_typed(ctx, stmt, reqt);
}

static knh_Term_t *knh_StmtIF_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	TYPING(ctx, stmt, 0, TYPE_Boolean, _NOTCAST);
	if(TERMs_isTRUE(stmt, 0)) {
		knh_Stmt_done(ctx, DP(stmt)->stmts[2]);
		TYPING_STMTs(ctx, stmt, 1, reqt);
		return knh_Stmt_toBLOCK(ctx, stmt, 1, reqt);
	}
	else if(TERMs_isFALSE(stmt, 0)) {
		knh_Stmt_done(ctx, DP(stmt)->stmts[1]);
		TYPING_STMTs(ctx, stmt, 2, reqt);
		return knh_Stmt_toBLOCK(ctx, stmt, 2, reqt);
	}
	else {
		TYPING_STMTs(ctx, stmt, 1, reqt);
		TYPING_STMTs(ctx, stmt, 2, reqt);
	}
	return knh_Stmt_typed(ctx, stmt, reqt);
}

//knh_Stmt_t *knh_StmtIF_decl(Ctx *ctx, knh_Stmt_t *stmt)
//{
//	knh_Stmt_t *thisStmt = NULL; /* Conditional Statement */
//	if(TERMs_typing(ctx, stmt, 0, TYPE_Boolean, _NOTCAST)) {
//		if(TERMs_isTRUE(stmt, 0)) {
//			knh_Stmt_done(ctx, DP(stmt)->stmts[2]);
//			thisStmt = DP(stmt)->stmts[1];
//		}
//		else if(TERMs_isFALSE(stmt, 0)) {
//			knh_Stmt_done(ctx, DP(stmt)->stmts[1]);
//			thisStmt = DP(stmt)->stmts[1];
//		}
//	}
//	if(thisStmt == NULL) {
//		knh_Gamma_perror(ctx, KERR_EWARN, _("not static condition"));
//	}
//	return thisStmt;
//}

static knh_Token_t *TERMs_it(Ctx *ctx, knh_Stmt_t *stmt, size_t n, knh_type_t type)
{
	knh_Token_t *tkIT = DP(stmt)->tokens[n];
	if(TT_(tkIT) != TT_LOCAL) {
		knh_fields_t decl = {0, type, FN_/*register*/, 0, NULL};
		knh_index_t idx = knh_Gamma_addLocal(ctx, &decl, 0);
		DBG_ASSERT(TT_(tkIT) == TT_ASIS);
		KNH_ASSERT(idx != -1);
		knh_Token_toTYPED(ctx, tkIT, TT_LOCAL, type, idx);
	}
	DBG_P("******** %s index=%d", TYPE__(type), DP(tkIT)->index);
	return tkIT;
}

static knh_Term_t *knh_StmtSWITCH_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	BEGIN_BLOCK(esp);
	knh_Stmt_t *stmtCASE, *stmtDEFAULT = NULL;
	knh_Token_t *tkIT = TERMs_it(ctx, stmt, 2/*IT*/, TYPE_Any);
	int c = 0;
	TYPING(ctx, stmt, 0, TYPE_Any, _NOTCHECK);
	stmtCASE = DP(stmt)->stmts[1];
	while(stmtCASE != NULL) {
		if(SP(stmtCASE)->stt == STT_CASE) {
			knh_Stmt_setESPIDX(ctx, stmtCASE);
			if(TERMs_isASIS(stmtCASE, 0)) {
				if(stmtDEFAULT != NULL) {
					knh_Gamma_perror(ctx, KERR_EWARN, _("multiple default in switch"));
					knh_Stmt_done(ctx, stmtCASE);
					goto L_NEXT;
				}
				stmtDEFAULT = stmtCASE;
				goto L_STMT;
			}
			else if(!TERMs_typing(ctx, stmtCASE, 0, TYPE_Any, _NOTCHECK)) {
				knh_Stmt_done(ctx, stmtCASE);
				goto L_NEXT;
			}
			{
				knh_Stmt_t *stmtOP = new_Stmt2(ctx, STT_CALL, new_TokenMN(ctx, MN_opCASE), DP(stmtCASE)->terms[0], tkIT, NULL);
				KNH_SETv(ctx, DP(stmtCASE)->terms[0], stmtOP);
				if(!TERMs_typing(ctx, stmtCASE, 0, TYPE_Boolean, _NOTCAST)) {
					knh_Stmt_done(ctx, stmtCASE);
					goto L_NEXT;
				}
			}
			L_STMT:;
			TYPING_STMTs(ctx, stmtCASE, 1, reqt);
			c++;
		}
		L_NEXT:;
		stmtCASE = DP(stmtCASE)->nextNULL;
	}
	END_BLOCK(esp);
	if(c == 0) {
		return knh_Stmt_done(ctx, stmt);
	}
	return TM(stmt);
}

static knh_Term_t* knh_StmtVOID_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
//	if(reqt != TYPE_void) {
//		knh_Gamma_perror(ctx, KERR_ERR, _("must be always a statement"));
//		return NULL;
//	}
	return knh_Stmt_typed(ctx, stmt, TYPE_void);
}

static knh_Term_t *knh_StmtWHILE_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	TYPING(ctx, stmt, 0, TYPE_Boolean, _NOTCAST);
	if(TERMs_isFALSE(stmt, 0)) {
		return knh_Stmt_done(ctx, stmt);
	}
	else {
		TYPING_STMTs(ctx, stmt, 1, TYPE_void);
	}
	return knh_StmtVOID_typing(ctx, stmt, reqt);
}

static knh_Term_t *knh_StmtDO_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	TYPING_STMTs(ctx, stmt, 0, TYPE_void);
	TYPING(ctx, stmt, 1, TYPE_Boolean, _NOTCAST);
	return knh_StmtVOID_typing(ctx, stmt, reqt);
}

static knh_Term_t *knh_StmtFOR_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	knh_Term_t *tm;
	BEGIN_BLOCK(esp);
	tm = knh_Stmt_typing(ctx, DP(stmt)->stmts[0], TYPE_void);
	if(tm != NULL) {
		TYPING(ctx, stmt, 1, TYPE_Boolean, _NOTCAST);
		if(TERMs_isFALSE(stmt, 1)) {
			return DP(stmt)->terms[0];
		}
		else {
			TYPING_STMTs(ctx, stmt, 2, TYPE_void);
			TYPING_STMTs(ctx, stmt, 3, TYPE_void);
		}
		tm = knh_StmtVOID_typing(ctx, stmt, reqt);
	}
	END_BLOCK(esp);
	return tm;
}

static knh_class_t knh_StmtDECL_typingITR(Ctx *ctx, knh_Stmt_t *stmt)
{
	if(isSINGLEFOREACH(stmt)) {
		knh_Token_t *tkT = DP(stmt)->tokens[0];
		knh_Token_t *tkN = DP(stmt)->tokens[1];
		DBG_P("T=%s", TT__(TT_(tkT)));
		if(TT_(tkT) == TT_VAR) {
			knh_TokenNAME_typing(ctx, tkN, TYPE_void, 1/*isLVALUE*/, 0/*toCONST*/);
			if(TT_(tkN) == TT_LOCAL) {
				knh_Token_toTYPED(ctx, tkT, TT_CID, SP(tkN)->type, CLASS_type(SP(tkN)->type));
				return knh_class_P1(ctx, CLASS_Iterator, CLASS_type(SP(tkN)->type));
			}
			if(TT_(tkN) == TT_FIELD) {
				knh_Gamma_perror(ctx, KERR_ERRATA, _("redeclare %T %L as local variable"), SP(tkN)->type, tkN);
				TT_(tkN) = TT_NAME;
				knh_Token_toTYPED(ctx, tkT, TT_CID, SP(tkN)->type, CLASS_type(SP(tkN)->type));
				goto L_DECL1;
			}
			DBG_P("N=%s", TT__(TT_(tkN)));
			DBG_ASSERT(TT_(tkN) == TT_NAME);
			return TYPE_var; // type inferencing
		}
		L_DECL1:;
		if(knh_StmtDECL_typing(ctx, stmt, 0, TYPE_void, SCOPE_LOCAL) == NULL) {
			return CLASS_unknown; // STOP
		}
		DBG_P("T=%s", TT__(TT_(tkT)));
		return knh_class_P1(ctx, CLASS_Iterator, DP(tkT)->cid);
	}
	else {
		KNH_TODO("multivalue foreach");
		return CLASS_unknown;
	}
}

static knh_Term_t *knh_StmtFOREACH_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	BEGIN_BLOCK(esp);
	TERMs_it(ctx, stmt, 3, TYPE_Iterator);
	knh_Stmt_t *stmtDECL = DP(stmt)->stmts[0];
	knh_class_t cid = knh_StmtDECL_typingITR(ctx, stmtDECL);
	if(cid == CLASS_unknown) {
		return NULL; //
	}
	if(cid == TYPE_var) {
		if(isSINGLEFOREACH(stmtDECL)) {
			TYPING(ctx, stmt, 1, TYPE_Any, 0);
			cid = TERMs_getcid(stmt, 1);
			if(knh_class_bcid(cid) != CLASS_Iterator) {
				knh_Method_t *mtd = knh_getMethodNULL(ctx, cid, MN_opITR);
				knh_type_t rtype = knh_type_tocid(ctx, knh_ParamArray_rtype(DP(mtd)->mp), cid);
				knh_Stmt_t *stmtC =
					new_Stmt2(ctx, STT_CALL, new_TokenMN(ctx, MN_opITR), DP(stmt)->terms[1], NULL);
				KNH_SETv(ctx, DP(stmt)->terms[1], knh_StmtCALL_typing(ctx, stmtC, rtype));
				KNH_ASSERT(knh_class_bcid(rtype) == CLASS_Iterator);
				cid = rtype;
			}
			cid = knh_class_p1(cid);
			knh_Token_toTYPED(ctx, DP(stmtDECL)->tokens[0], TT_CID, cid, cid);
			if(knh_StmtDECL_typing(ctx, stmtDECL, 0, TYPE_void, SCOPE_LOCAL) == NULL) {
				return NULL;
			}
			knh_Gamma_perror(ctx, KERR_INFO, "%L can be %T", DP(stmtDECL)->tokens[1], cid);
		}
		else {
			KNH_TODO("multivalue foreach");
		}
	}
	else {
		DBG_P("CLASS %s", CLASS__(cid));
		TYPING(ctx, stmt, 1, cid, _ICAST);
	}
	TYPING_STMTs(ctx, stmt, 2, TYPE_void);
	END_BLOCK(esp);
	return knh_StmtVOID_typing(ctx, stmt, reqt);
}

static knh_Term_t *knh_StmtTRY_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	BEGIN_BLOCK(esp);
	TERMs_it(ctx, stmt, 3/*HDR*/, TYPE_ExceptionHandler);
	TYPING_STMTs(ctx, stmt, 0/*try*/, TYPE_void);
	{
		knh_Stmt_t *stmtCATCH = DP(stmt)->stmts[1/*catch*/];
		while(stmtCATCH != NULL) {
			if(SP(stmtCATCH)->stt == STT_CATCH) {
				BEGIN_BLOCK(esp2);
				knh_fieldn_t fn = knh_Token_getfnq(ctx, DP(stmtCATCH)->tokens[1]);
				knh_fields_t decl = {0, TYPE_Exception, fn, 0, NULL};
				knh_index_t idx = knh_Gamma_addLocal(ctx, &decl, 0);
				if(idx == -1) {
					knh_Stmt_done(ctx, stmtCATCH);
				}
				else {
					knh_Stmt_setESPIDX(ctx, stmtCATCH);
					if(!TERMs_typing(ctx, stmtCATCH, 1, TYPE_Exception, 0)) {
						knh_Stmt_done(ctx, stmtCATCH);
					}
					else {
						TYPING_STMTs(ctx, stmtCATCH, 2, TYPE_void);
					}
				}
				END_BLOCK(esp2);
			}
			stmtCATCH = DP(stmtCATCH)->nextNULL;
		}
	}
	TYPING_STMTs(ctx, stmt, 2/*finally*/, reqt);
	END_BLOCK(esp);
	return TM(stmt);
}

static knh_Term_t *knh_StmtTHROW_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	TYPING(ctx, stmt, 0, TYPE_Exception, 0);
	return TM(stmt);
}

static knh_Term_t *knh_StmtLSEND_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	size_t i;
	for(i = 0; i < DP(stmt)->size; i++) {
		TYPING(ctx, stmt, i, TYPE_Any, _NOTCHECK);
	}
	TODO();
	return NULL;
}

static knh_Term_t *knh_StmtPRINT_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	size_t i;
	for(i = 0; i < DP(stmt)->size; i++) {
		TYPING(ctx, stmt, i, TYPE_Any, _NOTCHECK);
	}
	return knh_StmtVOID_typing(ctx, stmt, reqt);
}

static knh_Term_t *knh_StmtASSERT_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	TYPING(ctx, stmt, 0, TYPE_Boolean, _NOTCAST);
	if(TERMs_isTRUE(stmt, 0)) {
		return knh_Stmt_done(ctx, stmt);
	}
	if(TERMs_isFALSE(stmt, 0)) {
		knh_Gamma_perror(ctx, KERR_EWARN, _("always throw Assertion!!"));
	}
	TYPING_STMTs(ctx, stmt, 1, TYPE_void);
	return knh_StmtVOID_typing(ctx, stmt, reqt);
}

//static knh_Term_t *knh_StmtTEST_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt)
//{
//	knh_Gamma_t *kc = ctx->gma;
//	int testidx = DP(kc)->testidx;
//	knh_Token_t *tkIT;
//	BEGIN_BLOCK(esp);
//	if(!TERMs_typing(ctx, stmt, 0, TYPE_String, TCHECK_)) {
//		return NULL;
//	}
//	tkIT = TERMs_it(ctx, stmt, TYPE_String, TEST_IT);
//	DP(kc)->testidx = DP(tkIT)->index;
//	TERMs_typingBLOCK(ctx, stmt, 1, 1/*isIteration*/);
//	DP(kc)->testidx = testidx;
//	END_BLOCK(esp);
//	return TM(stmt);
//}

/* ======================================================================== */
/* [REGISTER] */

static knh_Stmt_t *knh_Gamma_register(Ctx *ctx, knh_Stmt_t *stmt)
{
	knh_type_t type = SP(stmt)->type;
	knh_fields_t decl = {FLAG_GAMMA_Register, type, FN_/*register*/, 0, UPCAST(stmt)};
	knh_index_t idx = knh_Gamma_addLocal(ctx, &decl, 0);
	knh_Stmt_t *stmtLET = new_Stmt2(ctx, STT_LET, NULL);
	knh_Token_t *tkN = new_TokenTYPED(ctx, TT_LOCAL, type, idx);
	knh_Stmt_add(ctx, stmtLET, tkN, stmt);
	SP(stmtLET)->type = type;
	if(idx == -1) {
		knh_Stmt_done(ctx, stmtLET);
	}
	return stmtLET;
}

static knh_Term_t *knh_StmtREGISTER_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	size_t i;
	for(i = 0; i < DP(stmt)->size; i++) {
		TYPING(ctx, stmt, i, TYPE_Any, _NOTWARN | _NOTCHECK);
		if(IS_Stmt(DP(stmt)->terms[i])) {
			KNH_SETv(ctx, DP(stmt)->terms[i], knh_Gamma_register(ctx, DP(stmt)->stmts[i]));
		}
	}
	return knh_StmtVOID_typing(ctx, stmt, reqt);
}

//static int knh_Token_equals(Ctx *ctx, knh_Token_t *tk, knh_Term_t *tm)
//{
//	if(IS_Stmt(tm)) return 0;
//	knh_Token_t *atk = (knh_Token_t*)tm;
//	if(SP(tk)->tt != SP(atk)->tt) return 0;
//	if(SP(tk)->type != DP(atk)->type) return 0;
//	if(DP(tk)->index != DP(atk)->index) return 0;
//	if(SP(tk)->tt == TT_CONST) {
//		if(knh_Object_cid(DP(tk)->data) != knh_Object_cid(DP(atk)->data)) return 0;
//		return (knh_Object_compareTo(ctx, DP(tk)->data, DP(atk)->data) == 0);
//	}
//	return 1;
//}

//static int knh_Stmt_equals(Ctx *ctx, knh_Stmt_t *stmt, knh_Term_t *tm)
//{
//	if(!IS_Stmt(tm)) return 0;
//	knh_Stmt_t *astmt = (knh_Stmt_t*)tm;
//	if(SP(stmt)->stt != SP(astmt)->stt) return 0;
//	//	if(SP(stmt)->flag != SP(astmt)->flag) return 0;
//	if(SP(stmt)->type != DP(astmt)->type) return 0;
//	if(DP(stmt)->size != DP(astmt)->size) return 0;
//	size_t i;
//	for(i = 0; i < DP(stmt)->size; i++) {
//		if(IS_Stmt(DP(stmt)->terms[i])) {
//			if(!knh_Stmt_equals(ctx, DP(stmt)->stmts[i], DP(astmt)->terms[i])) return 0;
//		}
//		if(IS_Token(DP(stmt)->terms[i])) {
//			if(!knh_Token_equals(ctx, DP(stmt)->tokens[i], DP(astmt)->terms[i])) return 0;
//		}
//	}
//	return 1;
//}

static knh_Term_t *knh_Gamma_lookupRegisteredStmt(Ctx *ctx, knh_Stmt_t *stmt)
{
//	DBG_ASSERT(IS_Stmt(stmt));
//	knh_Gamma_t *kc = ctx->gma;
//	knh_intptr_t i, offset = DP(kc)->goffset;
//	for(i = K_GAMMASIZE - 1; offset <= i; i--) {
//		if(DP(kc)->gamma[i].fn == FN_
//			&& FLAG_is(DP(kc)->gamma[i].flag, FLAG_GAMMA_Register)) {
//			knh_Stmt_t *stmtREG = (knh_Stmt_t*)DP(kc)->gamma[i].value;
//			DBG_ASSERT(IS_Stmt(stmtREG));
//			if(knh_Stmt_equals(ctx, stmtREG, TM(stmt))) {
//				DBG_P("REGISTER(%s) TO sfp[=%d]", cSTT_((stmt)), (int)i);
//				return TM(new_TokenLOCAL(ctx, TM(stmt), SP(stmt)->type, i));
//			}
//		}
//	}
	return TM(stmt);
}

/* ------------------------------------------------------------------------ */
/* [METHOD] */

static knh_class_t knh_StmtMETHOD_class(Ctx *ctx, knh_Stmt_t *stmt)
{
	knh_Token_t *tkC = DP(stmt)->tokens[1]; DBG_ASSERT(IS_Token(tkC));
	knh_class_t this_cid = DP(ctx->gma)->this_cid;
	if(TT_(tkC) == TT_ASIS) {
		return this_cid;
	}
	else {
		knh_class_t cid = knh_Token_getcid(ctx, tkC, DP(ctx->gma)->this_cid);
		if(this_cid != knh_Object_cid(knh_getGammaScript(ctx)) && this_cid != cid) {
			cid = this_cid;
			knh_Gamma_perror(ctx, KERR_ERRATA, _("different class: %B ==> %C"), TK_tobytes(tkC), cid);
		}
		return cid;
	}
}

static knh_methodn_t knh_StmtMETHOD_name(Ctx *ctx, knh_Stmt_t *stmt)
{
	knh_Token_t *tkN = DP(stmt)->tokens[2]; DBG_ASSERT(IS_Token(tkN));
	return (TT_(tkN) == TT_ASIS) ? MN_new : knh_Token_getmn(ctx, tkN);
}

static knh_flag_t knh_StmtMETHOD_flag(Ctx *ctx, knh_Stmt_t *o, knh_class_t cid)
{
	knh_flag_t flag = 0;
	if(IS_Map(DP(o)->metaDictCaseMap)) {
		ADD_FLAG(flag, "Virtual", FLAG_Method_Virtual);
		ADD_FLAG(flag, "Private", FLAG_Method_Private);
		ADD_FLAG(flag, "Const", FLAG_Method_Const);
		ADD_FLAG(flag, "Static", FLAG_Method_Static);
		ADD_FLAG(flag, "Debug", FLAG_Method_Debug);
//		ADD_FLAG(flag, "Inline", FLAG_Method_Inline);
		if(knh_class_isSingleton(cid)) flag |= FLAG_Method_Static;
		if(knh_class_isDebug(cid)) flag |= FLAG_Method_Debug;
	}
	return flag;
}

/* ------------------------------------------------------------------------ */

static knh_bool_t knh_ParamArray_setStmtM(Ctx *ctx, knh_ParamArray_t* mp, knh_Stmt_t *stmtM)
{
	size_t i, psize=0, rsize=0;
	knh_Stmt_t *stmtP = DP(stmtM)->stmts[3/*PARAMs*/];
	knh_fields_t *gf = DP(ctx->gma)->gf;
	DBG_ASSERT(STT_(stmtP) == STT_DECL);
	for(i = 0; i < DP(stmtP)->size; i += 3) {
		knh_Token_t *tkN = DP(stmtP)->tokens[i+1];
		if(knh_Token_isDOT(tkN)) break;
		if(knh_StmtDECL_typing(ctx, stmtP, +i, TYPE_void, SCOPE_PARAM) == NULL) {
			return 0;
		}
		else {
			int idx = DP(stmtP)->espidx;
			knh_param_t p = {gf[idx].type, gf[idx].fn};
			knh_ParamArray_add(ctx, mp, p);
		}
	}
	if(knh_Stmt_isVARGs(stmtP)) {
		knh_fields_t decl = {0, CLASS_Array, FN_vargs, 0, NULL};
		knh_Gamma_addLocal(ctx, &decl, 0);
		knh_ParamArray_setVARGs(mp, 1);
	}
	psize = i / 3;
	for(; i < DP(stmtP)->size; i += 3) {
		knh_Token_t *tkN = DP(stmtP)->tokens[i+1];
		if(TT_(tkN) == TT_ASIS) {
			knh_Token_t *tkT = DP(stmtP)->tokens[i];
			knh_param_t p = {knh_Token_getcid(ctx, tkT, TYPE_Any), 0};
			knh_ParamArray_radd(ctx, mp, p);
		}
		else if(knh_StmtDECL_typing(ctx, stmtP, +i, TYPE_void, SCOPE_PARAM) == NULL) {
			return 0;
		}
		else {
			int idx = DP(stmtP)->espidx;
			knh_param_t p = {gf[idx].type, idx /* not FN */};
			knh_ParamArray_radd(ctx, mp, p);
		}
	}
	rsize = (i / 3) - psize;
	if(rsize == 0) {
		knh_Token_t *tkR = DP(stmtM)->tokens[0/*RTYPE*/];
		knh_type_t rtype = (TT_(tkR) == TT_ASIS) ? TYPE_var: knh_Token_getcid(ctx, tkR, TYPE_Any);
		if(rtype != TYPE_void) {
			knh_param_t p = {rtype, 0};
			knh_ParamArray_radd(ctx, mp, p);
		}
	}
	return 1;
}

static void *knh_Gamma_loadFunc(Ctx *ctx, char *funcname, int isREQUIRED)
{
	knh_Gamma_t *kc = ctx->gma;
	if(DP(kc)->dlhdr != NULL) {
		void *f = knh_dlsym(ctx, DP(kc)->dlhdr, funcname, isREQUIRED);
		if(f != NULL) return f;
	}
	return NULL;
}

static knh_Fmethod knh_Gamma_loadMethodFunc(Ctx *ctx, knh_class_t cid, knh_methodn_t mn, int isNaitive)
{
	DBG_ASSERT_cid(cid);
	char buf[80];
	const char *cname = S_tochar(ClassTBL(cid).sname);
	if(MN_isFMT(mn)) {
		knh_snprintf(buf, sizeof(buf), "%s__%s", cname, FN__(MN_toFN(mn)));
	}
	else if(MN_isGETTER(mn)) {
		int off = knh_strlen(cname)+4;
		knh_snprintf(buf, sizeof(buf), "%s_get%s", cname, FN__(MN_toFN(mn)));
		if(islower(buf[off])) buf[off] = toupper(buf[off]);
	}
	else if(MN_isSETTER(mn)) {
		int off = knh_strlen(cname)+4;
		knh_snprintf(buf, sizeof(buf), "%s_set%s", cname, FN__(MN_toFN(mn)));
		if(islower(buf[off])) buf[off] = toupper(buf[off]);
	}
	else if(MN_isISBOOL(mn)) {
		int off = knh_strlen(cname)+4;
		knh_snprintf(buf, sizeof(buf), "%s_sis%s", cname, FN__(MN_toFN(mn)));
		if(islower(buf[off])) buf[off] = toupper(buf[off]);
	}
	else {
		knh_snprintf(buf, sizeof(buf), "%s_%s", cname, FN__(mn));
	}
	return (knh_Fmethod)knh_Gamma_loadFunc(ctx, buf, isNaitive);
}

static knh_Term_t *knh_StmtMTD_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_Method_t *mtd, knh_class_t mtd_cid)
{
	DP(mtd)->uri = SP(stmt)->uri;
	knh_Token_setCONST(ctx, DP(stmt)->tokens[2/*method*/], mtd);
	{
		knh_Fmethod func = NULL;
		func = knh_Gamma_loadMethodFunc(ctx, mtd_cid, DP(mtd)->mn, knh_StmtMETA_is(ctx, stmt, STEXT("Naitive")));
		if(func != NULL) {
			knh_Method_setFunc(ctx, mtd, func);
			return knh_Stmt_done(ctx, stmt);
		}
	}
	if(DP(stmt)->size == 4) {
		return knh_Stmt_done(ctx, stmt);
	}
	return TM(stmt);
}

static knh_Term_t* knh_StmtMETHOD_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt, int scope)
{
	BEGIN_LOCAL(ctx, lsfp, 1);
	knh_class_t mtd_cid = knh_StmtMETHOD_class(ctx, stmt);
	knh_flag_t flag   = knh_StmtMETHOD_flag(ctx, stmt, mtd_cid);
	knh_methodn_t mn = knh_StmtMETHOD_name(ctx, stmt);
	knh_Method_t *mtd = knh_getMethodNULL(ctx, mtd_cid, mn);
	LOCAL_NEW(ctx, lsfp, 0, knh_ParamArray_t*, mp, new_ParamArray(ctx));
	if(knh_ParamArray_setStmtM(ctx, mp, stmt) == 0) return NULL;
	DBG_({
		size_t i;
		DBG_P("mp->psize=%d, rsize=%d", mp->psize, mp->rsize);
		for(i = 0; i < mp->psize; i++) {
			knh_param_t *p = knh_ParamArray_get(mp, i);
			DBG_P("param type=%s fn=%s", TYPE__(p->type), FN__(p->fn));
		}
		for(i = 0; i < mp->rsize; i++) {
			knh_param_t *p = knh_ParamArray_rget(mp, i);
			DBG_P("return type=%s, idx=%d", TYPE__(p->type), p->fn);
		}
	});

	/* Check the type of Constructor */
//	if(knh_methodn_isNEW(ctx, mn)) {
//		knh_type_t rtype = knh_ParamArray_rtype(mp);
//		knh_Gamma_perror(ctx, KERR_ERRATA, _("constructor class: %C ==> %C"), CLASS_type(rtype), mtd_cid);
//		return NULL;
//	}

	if(mtd != NULL && DP(mtd)->mn < MN_OPSIZE) { /* op */
		knh_Gamma_perror(ctx, KERR_EWARN, _("operator overriding is deprecated"));
		if(!knh_Context_isInteractive(ctx)) return NULL;
		mtd = NULL;
	}
	/* New method, and constructors are always new */
	if(mtd == NULL || knh_methodn_isNEW(ctx, mn) || (knh_Method_isPrivate(mtd) && DP(mtd)->cid != mtd_cid)) {
		mtd = new_Method(ctx, flag, mtd_cid, mn, NULL);
		KNH_SETv(ctx, DP(mtd)->mp, mp);
		knh_Class_addMethod(ctx, mtd_cid, mtd);
	}
	else {
		if(DP(mtd)->cid != mtd_cid) { /* Overriding */
			if(knh_Method_isFinal(mtd)) { /* CHECK @Final */
				knh_Gamma_perror(ctx, KERR_ERR, _("%C.%M is final; add @Virtual %C.%M"),
						DP(mtd)->cid, mn, ClassTBL(DP(mtd)->cid).supcid, mn);
				return NULL;
			}
		}
		if(!knh_StmtMETA_is(ctx, stmt, STEXT("Override")) &&
				!knh_Method_isAbstract(mtd) && DP(mtd)->mn != MN_main) {
			/* Konoha: To override, you need @Override */
			if(!knh_Context_isInteractive(ctx)) {
				knh_Gamma_perror(ctx, KERR_DWARN, _("%C.%M is overriden"), mtd_cid, mn);
			}
		}
//		if(knh_ParamArray_hasUntyped(mp)) {
//			TODO(); /* inferred from DP(mtd)->mp */
//		}
		if(!knh_ParamArray_equalsType(DP(mtd)->mp, mp)) {
			knh_Gamma_perror(ctx, KERR_TERROR, _("%C.%M must be defined as %O"), mtd_cid, mn, DP(mtd)->mp);
			knh_Stmt_done(ctx, stmt);
			return NULL;
		}
	}
	END_LOCAL(ctx, lsfp);
	return knh_StmtMTD_typing(ctx, stmt, mtd, mtd_cid);
}

/* ------------------------------------------------------------------------ */

//static knh_Gamma_t *new_Gamma(Ctx *ctx)
//{
//	knh_Gamma_t *ctxkc = ctx->gma, *kc = (knh_Gamma_t*)new_Object_bcid(ctx, CLASS_Gamma, 0);
//	KNH_SETv(ctx, DP(kc)->ns, DP(ctxkc)->ns);
//	KNH_INITv(DP(kc)->symbolDictMap, DP(ctxkc)->symbolDictMap);
//	KNH_INITv(DP(kc)->constPools, DP(ctxkc)->constPools);
//	DP(kc)->this_cid = DP(ctxkc)->this_cid;
//	return kc;
//}

/* ------------------------------------------------------------------------ */

static knh_Term_t* knh_StmtFUNCTION_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{

	knh_Stmt_done(ctx, stmt);
	return TM(stmt);
}


static knh_class_t knh_StmtPARAMs_getFormatterClass(Ctx *ctx, knh_Stmt_t *stmt)
{
	knh_class_t cid = CLASS_unknown;
	DBG_ASSERT(STT_(stmt) == STT_DECL);
	if(DP(stmt)->size == 3) {
		knh_Token_t *tkT = DP(stmt)->tokens[0];
		cid = knh_Token_getcid(ctx, tkT, CLASS_Any);
		return cid;
	}
	knh_Gamma_perror(ctx, KERR_ERR, _("formatter method takes one parameter"));
	return cid;
}

static knh_Method_t* knh_NameSpace_getFormatterNULL(Ctx *ctx, knh_NameSpace_t *ns, knh_class_t cid, knh_methodn_t mn)
{
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
	return NULL;
}

static knh_Term_t *knh_StmtFORMAT_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt, int scope)
{
	knh_class_t cid = knh_StmtPARAMs_getFormatterClass(ctx, DP(stmt)->stmts[3]);
	knh_methodn_t mn = MN_toFMT(knh_Token_getmn(ctx, DP(stmt)->tokens[2]));
	DBG_P("DP(stmt)->size=%ld, cid=%s", DP(stmt)->size, CLASS__(cid));
	if(cid != CLASS_unknown) {
		knh_Method_t *mtd = knh_NameSpace_getFormatterNULL(ctx, DP(ctx->gma)->ns, cid, mn);
		if(mtd == NULL) {
			mtd = new_Method(ctx, 0, cid, mn, NULL);
			KNH_SETv(ctx, DP(mtd)->mp, KNH_TNULL(ParamArray));
			knh_NameSpace_addFormatter(ctx, DP(ctx->gma)->ns, mtd);
		}
		return knh_StmtMTD_typing(ctx, stmt, mtd, cid);
	}
	return NULL;
}

/* ------------------------------------------------------------------------ */

static int knh_Gamma_initClassTBLField(Ctx *ctx, knh_class_t cid)
{
	knh_ClassTBL_t *t = pClassTBL(ctx, cid);
	DP(ctx->gma)->flag = 0;
	DP(ctx->gma)->this_cid = cid;
	knh_Gamma_clear(ctx, 0);
	return (t->fields == NULL && t->fsize == 0);
}

#ifdef K_USING_DEBUG
static void knh_cfield_dump(Ctx *ctx, knh_fields_t *cf, size_t offset, size_t fsize, knh_OutputStream_t *w)
{
	size_t idx = 0;
	for(idx = 0; idx < fsize; idx++) {
		if(cf[idx].fn == FN_NONAME) {
			knh_printf(ctx, w, "[%d] -\n", (offset+idx));
			continue;
		}
		if(cf[idx].fn == FN_/*register*/ || cf[idx].type == CLASS_unknown) continue;
		if(cf[idx].value != NULL) {
			knh_printf(ctx, w, "[%d] %T %N = %O\n", (offset+idx), cf[idx].type, cf[idx].fn, cf[idx].value);
		}
		else {
			knh_printf(ctx, w, "[%d] %T %N = default\n", (offset+idx), cf[idx].type, cf[idx].fn);
		}
	}
}
#endif

static void knh_Gamma_declareClassField(Ctx *ctx, knh_class_t cid)
{
	knh_ClassTBL_t *t = pClassTBL(ctx, cid);
	int i, fsize = DP(ctx->gma)->espidx;
	DBG_ASSERT(t->fsize == 0);
	if(fsize > 0) {
		knh_fields_t *cf = (knh_fields_t*)KNH_MALLOC(ctx, sizeof(knh_fields_t) * fsize);
		for(i = 0; i < fsize; i++) {
			cf[i] = DP(ctx->gma)->gf[i];
			if(cf[i].value != NULL) {
				DP(ctx->gma)->gf[i].value = NULL; /* COPY TO GC */
			}
			if(FLAG_is(cf[i].flag, _FKEY)) {
				DBG_P("@Key keyidx=%d, %d", t->keyidx, i);
				if(t->keyidx == -1) t->keyidx = i;
			}
		}
		t->fields = cf;
		t->fsize = fsize;
	}
	t->cspi = ClassTBL(CLASS_Object).cspi;
	if(t->supcid != CLASS_Object) {
		t->offset = ClassTBL(t->supcid).size / sizeof(Object*);
	}
	t->size = sizeof(Object*) * (fsize + t->offset);
	DBG_({
		DBG_P("HERE IS DEFINED STRUCT (offset=%d, fsize=%d)", t->offset, fsize);
		knh_cfield_dump(ctx, t->fields, t->offset, fsize, KNH_STDOUT);
	});
	/* update default value */
	DBG_ASSERT(knh_Object_cid(t->defnull) == cid);
	{
		knh_ObjectField_t *of = (knh_ObjectField_t*)t->defnull;
		of->bsize = t->size / sizeof(Object*);
		if(t->size == 0) {
			of->fields = NULL;
		}
		else {
			of->fields = (knh_Object_t**)KNH_MALLOC(ctx, sizeof(Object*) * of->bsize);
			//ClassTBL(CLASS_ObjectField).cspi->init(ctx, UPCAST(of));
			t->cspi->init(ctx, UPCAST(of));
		}
	}
}

static knh_Term_t *knh_StmtCLASS_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	knh_Token_t *tkC = DP(stmt)->tokens[0];
	knh_class_t prev_cid = DP(ctx->gma)->this_cid;
	knh_class_t this_cid = DP(tkC)->cid;
	knh_Stmt_t *stmtFIELD = knh_Token_parseStmt(ctx, DP(stmt)->tokens[4]);
	KNH_SETv(ctx, DP(stmt)->stmts[4], stmtFIELD);
	if(knh_Gamma_initClassTBLField(ctx, this_cid)) {
		stmtFIELD = DP(stmt)->stmts[4/*instmt*/];
		while(stmtFIELD != NULL) {
			knh_Term_t *tm = NULL;
			knh_Gamma_setLine(ctx, SP(stmtFIELD)->line);
			switch(STT_(stmtFIELD)) {
				CASE_STMT(DECL, stmtFIELD, +0, TYPE_void, SCOPE_FIELD);
				CASE_STMT(LET, stmtFIELD, TYPE_void, SCOPE_FIELD);
			}
			stmtFIELD = DP(stmtFIELD)->nextNULL;
		}
		knh_Gamma_declareClassField(ctx, this_cid);
		DBG_ASSERT(IS_String(DP(tkC)->text));
	}

	stmtFIELD = DP(stmt)->stmts[4/*instmt*/];
	while(stmtFIELD != NULL) {
		knh_Term_t *tm = NULL;
		SP(ctx->gma)->line = SP(stmtFIELD)->line;
		knh_Gamma_setLine(ctx, SP(stmtFIELD)->line);
		switch(STT_(stmtFIELD)) {
			CASE_STMT(METHOD, stmtFIELD, TYPE_void, SCOPE_FIELD);
			CASE_STMT(FORMAT, stmtFIELD, TYPE_void, SCOPE_FIELD);
			case STT_DONE: case STT_DECL: case STT_LET: case STT_ERR: break;
			default:
				knh_Gamma_perror(ctx, KERR_DWARN, _("don't use %s in class{}"), TT__(STT_(stmtFIELD)));
		}
		stmtFIELD = DP(stmtFIELD)->nextNULL;
	}
	DP(ctx->gma)->this_cid = prev_cid;
	return TM(stmt);
}

/* ------------------------------------------------------------------------ */

static knh_Term_t *knh_StmtBLOCK_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	return knh_StmtITR_typing(ctx, DP(stmt)->stmts[0], reqt);
}

static knh_Term_t *knh_Stmt_typing(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt)
{
	knh_Term_t *tm = NULL;
	if(knh_Stmt_isTyped(stmt)) return TM(stmt);
	if(knh_stmt_isExpr(STT_(stmt))) {
		tm = knh_StmtEXPR_typing(ctx, stmt, reqt);
		if(tm != NULL && IS_Stmt(tm)) {
			return knh_Gamma_lookupRegisteredStmt(ctx, (knh_Stmt_t*)tm);
		}
	}
	else {
		switch(STT_(stmt)) {
			CASE_STMT(BLOCK, stmt, reqt);
			CASE_STMT(DECL, stmt, +0, TYPE_void, SCOPE_LOCAL);
			CASE_STMT(LETM, stmt, SCOPE_LOCAL);
			CASE_STMT(PUSHM, stmt, SCOPE_LOCAL);
			CASE_STMT(IF, stmt, reqt);
			CASE_STMT(SWITCH, stmt, reqt);
			CASE_STMT(WHILE, stmt, reqt);
			CASE_STMT(DO, stmt, reqt);
			CASE_STMT(FOR, stmt, reqt);
			CASE_STMT(FOREACH, stmt, reqt);
			CASE_STMT(TRY, stmt, reqt);
			CASE_STMT(THROW, stmt, reqt);
			CASE_STMT(RETURN, stmt, reqt);
			CASE_STMT(LSEND, stmt, reqt);
			CASE_STMT(PRINT, stmt, reqt);
			CASE_STMT(REGISTER, stmt, reqt);
			CASE_STMT(ASSERT, stmt, reqt);
			case STT_BREAK:
			case STT_CONTINUE:
			case STT_DONE:
				tm = knh_Stmt_typed(ctx, stmt, TYPE_void);
				break;
			default:
				DBG_P("unknown stmt=%s", TT__(STT_(stmt)));
				tm = NULL;
		}
	}
	return tm;
}

static void knh_Stmt_toCALL1(Ctx *ctx, knh_Stmt_t *stmt, knh_Term_t *tm)
{
	size_t i;
	STT_(stmt) = STT_CALL1;
	KNH_SETv(ctx, DP(stmt)->terms[0], tm);
	for(i = 1; i < DP(stmt)->size; i++) {
		DBG_ASSERT(DP(stmt)->terms[i] != NULL);
		KNH_FINALv(ctx, DP(stmt)->terms[i]);
	}
	DP(stmt)->size = 1;
	knh_Stmt_typed(ctx, stmt, tm->type);
}

#define _isLast(stmt) (knh_Stmt_isEveryLine(stmt) || DP(stmt)->nextNULL == NULL)

static knh_Term_t *knh_StmtITR_typing(Ctx *ctx, knh_Stmt_t *stmtITR, knh_type_t reqt)
{
	knh_Stmt_t *stmt = stmtITR;
	BEGIN_BLOCK(espidx);
	while(stmt != NULL) {
		knh_Term_t *tm;
		knh_type_t rtype = (reqt != TYPE_void) ? knh_Gamma_getReturnType(ctx) : TYPE_void;
		rtype = (_isLast(stmt)) ? rtype : TYPE_void;
		knh_Gamma_setLine(ctx, SP(stmt)->line);
		tm = knh_Stmt_typing(ctx, stmt, rtype);
		if(tm == NULL) {
			knh_Stmt_toERR(ctx, stmtITR, TM(stmt)); return NULL;
		}
		DBG_P("TYPING=%s, TYPED=%s, %s", TT__(STT_(stmt)), TT__(TT_(tm)), TYPE__(tm->type));
		if(IS_Token(tm)) {
			if(rtype != TYPE_void) {
				knh_Stmt_toCALL1(ctx, stmt, tm);
			}
			else {
				knh_Gamma_perror(ctx, KERR_DWARN, _("no effect"));
				knh_Stmt_done(ctx, stmt);
			}
		}
		knh_Stmt_setESPIDX(ctx, stmt);
		DP(ctx->gma)->typeIT = tm->type;
		DP(ctx->gma)->idxIT = DP(stmt)->espidx;
		if(STT_(stmt) == STT_RETURN || STT_(stmt) == STT_THROW) {
			if(DP(stmt)->nextNULL != NULL) {
				KNH_FINALv(ctx, DP(stmt)->nextNULL);
			}
			goto L_RETURN;
		}
		if(rtype != TYPE_void) {
			if(tm->type == TYPE_void && DP(stmt)->nextNULL == NULL) {
				if(rtype != TYPE_Any) {
					KNH_INITv(DP(stmt)->nextNULL, new_Stmt2(ctx, STT_RETURN, NULL));
				}
			}
			else if(knh_stmt_isExpr(STT_(stmt))) {
				knh_Gamma_inferReturnType(ctx, 0, tm->type);
				knh_Stmt_setTailReturn(stmt, 1);
			}
		}
		stmt = knh_Stmt_isEveryLine(stmt) ? NULL : DP(stmt)->nextNULL;
	}
	L_RETURN:;
	stmt = stmtITR;
	END_BLOCK(espidx);
	return TM(stmtITR);
}

static void knh_Gamma_initThisM(Ctx *ctx, knh_class_t cid)
{
	size_t i, goffset = DP(ctx->gma)->goffset;
	knh_fields_t *gamma = DP(ctx->gma)->gf;
	knh_typevars_t *typevars = DP(ctx->gma)->typevars;
	DBG_ASSERT(goffset < K_GAMMASIZE);
	knh_Gamma_clear(ctx, goffset);
	DP(ctx->gma)->psize = 0;
	DBG_ASSERT_cid(cid);
	DP(ctx->gma)->this_cid = cid;
	gamma[goffset].type = TYPE_cid(cid);
	gamma[goffset].fn   = FN_this;
	DP(ctx->gma)->espidx = goffset + 1;
	typevars[0].type = cid;
	DP(ctx->gma)->tvsize = 1;
	if(ClassTBL(cid).cparam != NULL) {
		knh_ParamArray_t *cp = ClassTBL(cid).cparam;
		for(i = 0; i < cp->psize; i++) {
			knh_param_t *p = knh_ParamArray_get(cp, i);
			typevars[i+1].type = p->type;
			typevars[i+1].pn = p->fn;
		}
		for(i = 0; i < cp->rsize; i++) {
			knh_param_t *p = knh_ParamArray_rget(cp, i);
			typevars[i+1+(cp->psize)].type = p->type;
			typevars[i+1+(cp->psize)].pn = p->fn;
		}
		DP(ctx->gma)->tvsize += (cp->psize + cp->rsize);
	}
	//DBG_P("goffset=%ld, this=%s, tvsize=%d", goffset, CLASS__(cid), (int)DP(ctx->gma)->tvsize);
}

#define CASE_STMT2(XX, ...) case STT_##XX : { \
		knh_Gamma_initThisM(ctx, this_cid); \
		tm = knh_Stmt##XX##_typing(ctx, ## __VA_ARGS__); \
		break;\
	}\

int knh_StmtITR_scriptTyping(Ctx *ctx, knh_Stmt_t *stmtITR, knh_type_t reqt)
{
	knh_Stmt_t *stmt = stmtITR;
	knh_class_t this_cid = knh_Object_cid(knh_getGammaScript(ctx));
	while(stmt != NULL) {
		knh_Term_t *tm = TM(stmt);
		knh_Gamma_setLine(ctx, SP(stmt)->line);
		KNH_SETv(ctx, DP(ctx->gma)->mtd, KNH_NULL);
		switch(STT_(stmt)) {
			CASE_STMT(CLASS, stmt, TYPE_void);
			CASE_STMT2(METHOD, stmt, TYPE_void, SCOPE_SCRIPT);
			CASE_STMT(FORMAT, stmt, TYPE_void, SCOPE_SCRIPT);
			CASE_STMT2(DECL, stmt, +0, TYPE_void, SCOPE_SCRIPT);  // Gamma
			CASE_STMT2(LET, stmt, TYPE_void, SCOPE_SCRIPT);  // Gamma
			CASE_STMT2(LETM, stmt, SCOPE_SCRIPT);  // Gamma
			CASE_STMT2(PUSHM, stmt, SCOPE_SCRIPT);  // Gamma
		}
		if(tm == NULL) {
			knh_Stmt_done(ctx, stmt); return 0;
		}
		if(IS_Token(tm)) {
			knh_Stmt_done(ctx, stmt);
		}
		stmt = knh_Stmt_isEveryLine(stmt) ? NULL : DP(stmt)->nextNULL;
	}
	return 1;
}

/* ------------------------------------------------------------------------ */
/* [asm] */


knh_bool_t knh_Method_typing(Ctx *ctx, knh_Method_t *mtd, knh_Stmt_t *stmtP, knh_Stmt_t *stmtB, knh_type_t reqt)
{
	KNH_SETv(ctx, DP(ctx->gma)->mtd, mtd);
	knh_Method_toAbstract(ctx, mtd);
	DP(ctx->gma)->flag  = 0;
	knh_Gamma_initThisM(ctx, DP(mtd)->cid);
	if(stmtP != NULL) {
		size_t i;
		DBG_ASSERT(STT_(stmtP) == STT_DECL);
		for(i = 0; i < DP(stmtP)->size; i += 3) {
			knh_Token_t *tkN = DP(stmtP)->tokens[i+1];
			if(knh_Token_isDOT(tkN)) break;
			knh_StmtDECL_typing(ctx, stmtP, +i, TYPE_void, SCOPE_PARAM); /*checked*/
		}
		DP(ctx->gma)->psize = i / 3;
		if(knh_Stmt_isVARGs(stmtP)) {
			TODO();DBG_ABORT();
		}
		for(; i < DP(stmtP)->size; i += 3) {
			knh_StmtDECL_typing(ctx, stmtP, +i, TYPE_void, SCOPE_PARAM); /*checked*/
		}
		DBG_P("psize = %d", DP(ctx->gma)->psize);
	}
	knh_Array_clear(ctx, DP(ctx->gma)->lstacks, 0);
	knh_Array_clear(ctx, DP(ctx->gma)->insts, 0);
	return (knh_StmtITR_typing(ctx, stmtB, reqt) != NULL);
}

knh_bool_t knh_Formatter_typing(Ctx *ctx, knh_Method_t *mtd, knh_Stmt_t *stmtP, knh_Stmt_t *stmtB, knh_type_t reqt)
{
	knh_Gamma_initThisM(ctx, CLASS_OutputStream);
	knh_Token_t *tkT = DP(stmtP)->tokens[0];
	knh_Token_t *tkN = DP(stmtP)->tokens[1];
	knh_fields_t decl = {0, 0, knh_Token_getcid(ctx, tkT, CLASS_Any), knh_Token_getfnq(ctx, tkN)};
	knh_Gamma_addLocal(ctx, &decl, 0);
	KNH_SETv(ctx, DP(ctx->gma)->mtd, mtd);
	knh_Method_toAbstract(ctx, mtd);
	DP(ctx->gma)->flag  = 0;
	knh_Array_clear(ctx, DP(ctx->gma)->lstacks, 0);
	knh_Array_clear(ctx, DP(ctx->gma)->insts, 0);
	return (knh_StmtITR_typing(ctx, stmtB, reqt) != NULL);
}

static void knh_Method_setSourceCode(Ctx*ctx, knh_Method_t *mtd, knh_String_t *source)
{
	if(knh_Method_isObjectCode(mtd) && IS_KLRCode(DP(mtd)->kcode)) {
		KNH_SETv(ctx, (DP(mtd)->kcode)->source, source);
	}
}

static void knh_StmtMETHOD_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	knh_Method_t *mtd = DP(DP(stmt)->tokens[2/*method*/])->mtd;
	DBG_ASSERT(DP(stmt)->size == 6);
	DBG_ASSERT(TT_(DP(stmt)->tokens[5]) == TT_BLOCK);
	KNH_SETv(ctx, DP(stmt)->stmts[5], knh_Token_parseStmt(ctx, DP(stmt)->tokens[5]));
	knh_Method_asm(ctx, mtd, DP(stmt)->stmts[3/*StmtP*/], DP(stmt)->stmts[5/*StmtB*/], TYPE_void, knh_Method_typing);
	knh_Method_setSourceCode(ctx, mtd, DP(DP(stmt)->tokens[4/*source*/])->text);
}


static void knh_StmtFORMAT_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	knh_Method_t *mtd = DP(DP(stmt)->tokens[2/*method*/])->mtd;
	if(DP(stmt)->size == 5) {
		knh_Stmt_t *stmtOP = new_Stmt2(ctx, STT_OP, NULL); // TODO
		knh_Stmt_add(ctx, stmt, stmtOP);
	}
	else {
		DBG_ASSERT(DP(stmt)->size == 6);
		DBG_ASSERT(TT_(DP(stmt)->tokens[5]) == TT_BLOCK);
		KNH_SETv(ctx, DP(stmt)->stmts[5], knh_Token_parseStmt(ctx, DP(stmt)->tokens[5]));
	}
	knh_Method_asm(ctx, mtd, DP(stmt)->stmts[3/*StmtP*/], DP(stmt)->stmts[5/*StmtB*/], TYPE_void, knh_Formatter_typing);
	knh_Method_setSourceCode(ctx, mtd, DP(DP(stmt)->tokens[4/*source*/])->text);
}

static void knh_StmtCLASS_asm(Ctx *ctx, knh_Stmt_t *stmt)
{
	knh_Gamma_t *kc = ctx->gma;
	knh_class_t prev_cid = DP(kc)->this_cid;
	knh_class_t this_cid = DP(DP(stmt)->tokens[0])->cid;
	knh_Stmt_t *stmtFIELD = DP(stmt)->stmts[4/*instmt*/];
	DP(kc)->this_cid = this_cid;
	while(stmtFIELD != NULL) {
		knh_Gamma_setLine(ctx, SP(stmtFIELD)->line);
		if(SP(stmtFIELD)->stt == STT_METHOD) {
			knh_StmtMETHOD_asm(ctx, stmtFIELD);
		}
		else if(SP(stmtFIELD)->stt == STT_FORMAT) {
			knh_StmtFORMAT_asm(ctx, stmtFIELD);
		}
		stmtFIELD = DP(stmtFIELD)->nextNULL;
	}
	DP(kc)->this_cid = prev_cid;
	knh_Stmt_done(ctx, stmt);
}

void knh_Stmt_scriptAsm(Ctx *ctx, knh_Stmt_t *stmt)
{
	switch(STT_(stmt)) {
	case STT_CLASS: knh_StmtCLASS_asm(ctx, stmt); break;
	case STT_METHOD: knh_StmtMETHOD_asm(ctx, stmt); break;
	case STT_FORMAT: knh_StmtFORMAT_asm(ctx, stmt); break;
	default: return;
	}
	knh_Stmt_done(ctx, stmt);
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
