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
//#define USE_bytes_strcmp    1
//#define USE_bytes_parseint  1
#define USE_cwb_open      1

#include"commons.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ======================================================================== */
/* [ParamArray] */

knh_ParamArray_t *new_ParamArrayR0(Ctx *ctx, knh_type_t t)
{
	knh_ParamArray_t *pa = new_ParamArray(ctx);
	knh_param_t p = {t, FN_return};
	knh_ParamArray_radd(ctx, pa, p);
	return pa;
}

/* ------------------------------------------------------------------------ */

knh_ParamArray_t *new_ParamArrayP1(Ctx *ctx, knh_type_t rtype, knh_type_t p1, knh_fieldn_t fn1)
{
	knh_ParamArray_t *mp = new_ParamArray(ctx);
	knh_param_t p = {p1, fn1}, 	r = {rtype, FN_return};
	knh_ParamArray_add(ctx, mp, p);
	knh_ParamArray_radd(ctx, mp, r);
	return mp;
}

/* ------------------------------------------------------------------------ */

void knh_ParamArray_add(Ctx *ctx, knh_ParamArray_t *pa, knh_param_t p)
{
	size_t size = pa->psize + pa->rsize;
	if(size == 0) {
		pa->p0 = p;
		pa->p2 = p;  // last
	}
	else if(size == 1) {
		pa->p1 = p;
		pa->p2 = p;  // last
	}
	else if(size == 2) {
		pa->p2 = p;
	}
	else if(size == 3) {
		knh_param_t *params = (knh_param_t*)KNH_MALLOC(ctx, K_FASTMALLOC_SIZE);
		params[0] = pa->p0;
		params[1] = pa->p1;
		params[2] = pa->p2;
		params[3] = p;
		pa->capacity = K_FASTMALLOC_SIZE / sizeof(knh_param_t);
		pa->params = params;
	}
	else {
		if(pa->capacity == size) {
			KNH_TODO("large parameter n > 8");
		}
		pa->params[size] = p;
	}
	pa->psize += 1;
}

/* ------------------------------------------------------------------------ */

void knh_ParamArray_radd(Ctx *ctx, knh_ParamArray_t *pa, knh_param_t p)
{
	knh_ParamArray_add(ctx, pa, p);
	pa->psize -= 1;
	pa->rsize += 1;
}

/* ------------------------------------------------------------------------ */

knh_param_t* knh_ParamArray_get(knh_ParamArray_t *pa, size_t n)
{
	size_t size = pa->psize + pa->rsize;
	DBG_ASSERT(n < size);
	if(size <= 3) {
		switch(n) {
			case 0: return &pa->p0;
			case 1: return &pa->p1;
			case 2: return &pa->p2;
			default: return &pa->p2;
		}
	}
	else {
		return pa->params + n;
	}
}

/* ------------------------------------------------------------------------ */

knh_type_t knh_ParamArray_getptype(knh_ParamArray_t *pa, size_t n)
{
	if(!(n < pa->psize)) {
		if(pa->psize == 0) return TYPE_Any;
		n = pa->psize-1;
	}
	return knh_ParamArray_get(pa, n)->type;
}

/* ------------------------------------------------------------------------ */

knh_bool_t knh_ParamArray_equalsType(knh_ParamArray_t *pa, knh_ParamArray_t *pa2)
{
	size_t i;
	if(pa->psize != pa2->psize || pa->rsize != pa2->rsize) return 0;
	for(i = 0; i < pa->psize; i++) {
		knh_param_t *p = knh_ParamArray_get(pa, i);
		knh_param_t *p2 = knh_ParamArray_get(pa2, i);
		if(p->type != p2->type) return 0;
	}
	for(i = 0; i < pa->rsize; i++) {
		knh_param_t *p = knh_ParamArray_rget(pa, i);
		knh_param_t *p2 = knh_ParamArray_rget(pa2, i);
		if(p->type != p2->type) return 0;
	}
	return 1;
}

/* ------------------------------------------------------------------------ */

knh_type_t knh_ParamArray_rtype(knh_ParamArray_t *pa)
{
	if(pa->rsize != 0) {
		knh_param_t *p = knh_ParamArray_rget(pa, 0);
		return p->type;
	}
	return TYPE_void;
}

/* ======================================================================== */
/* [mn] */

void knh_write_mn(Ctx *ctx, knh_OutputStream_t *w, knh_methodn_t mn)
{
	knh_bytes_t t = B(MN_tochar(mn));
	if(MN_isFMT(mn)) {
		knh_putc(ctx, w, '%');
		knh_write(ctx, w, t);
	}
	else if(MN_isISBOOL(mn)) {
		knh_write(ctx, w, STEXT("is"));
		knh_write_cap(ctx, w, t);
	}
	else if(MN_isGETTER(mn)) {
		knh_write(ctx, w, STEXT("get"));
		knh_write_cap(ctx, w, t);
	}
	else if(MN_isSETTER(mn)) {
		knh_write(ctx, w, STEXT("set"));
		knh_write_cap(ctx, w, t);
	}
	else {
		knh_write(ctx, w, t);
	}
}

/* ======================================================================== */
/* [Method] */

static METHOD knh_Fmethod_abstract(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Method_t *mtd = sfp[K_MTDIDX].callmtd;
	knh_type_t rtype = knh_type_tocid(ctx, knh_ParamArray_rtype(DP(mtd)->mp), knh_Object_cid(sfp[0].o));
	KNH_SYSLOG(ctx, LOG_NOTICE, "CALLED_AbstractMethod", "%O", mtd);
	if(rtype != TYPE_void) {
		if(IS_Tunbox(rtype)) {
			RETURNi_(K_INT0);
		}
		else {
			RETURN_(KNH_NULVAL(CLASS_type(rtype)));
		}
	}
}

/* ------------------------------------------------------------------------ */

knh_bool_t knh_Method_isAbstract(knh_Method_t *o)
{
	return (DP(o)->fproceed == knh_Fmethod_abstract);
}

/* ------------------------------------------------------------------------ */

void knh_Method_toAbstract(Ctx *ctx, knh_Method_t *mtd)
{
	if(knh_Method_isObjectCode(mtd)) {
		KNH_FINALv(ctx, DP(mtd)->code);
		knh_Method_setObjectCode(mtd, 0);
	}
	knh_Method_setFunc(ctx, mtd, knh_Fmethod_abstract);
}

/* ------------------------------------------------------------------------ */

void knh_Method_setFunc(Ctx *ctx, knh_Method_t *mtd, knh_Fmethod func)
{
	func = (func == NULL) ? knh_Fmethod_abstract : func;
	DP(mtd)->fproceed = func;
	(mtd)->fcall_1 = func;
	(mtd)->pc_start = CODE_FUNCCALL;
	DBG_ASSERT((mtd)->pc_start != NULL);
}

/* ------------------------------------------------------------------------ */

knh_Method_t* new_Method(Ctx *ctx, knh_flag_t flag, knh_class_t cid, knh_methodn_t mn, knh_Fmethod func)
{
	knh_Method_t* mtd = new_(Method);
	DP(mtd)->flag  = flag;
	DP(mtd)->cid   = cid;
	DP(mtd)->mn    = mn;
	DP(mtd)->code  = NULL;
	knh_Method_setFunc(ctx, mtd, func);
	return mtd;
}

/* ------------------------------------------------------------------------ */

static METHOD knh_Fmethod_NoSuchMethod(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_Bytes_write(ctx, cwb->ba, STEXT("NoSuchMethod!!: "));
	knh_write_sname(ctx, cwb->w, knh_Object_cid(sfp[0].o));
	knh_putc(ctx, cwb->w, '.');
	knh_write_mn(ctx, cwb->w, DP(sfp[K_MTDIDX].callmtd)->mn);
	knh_stack_throw(ctx, ctx->esp, knh_cwb_newException(ctx, cwb));
}

/* ------------------------------------------------------------------------ */

knh_bool_t knh_Method_isNoSuchMethod(knh_Method_t *o)
{
	return (DP(o)->fproceed == knh_Fmethod_NoSuchMethod);
}

/* ------------------------------------------------------------------------ */

knh_Method_t* new_Method__NoSuchMethod(Ctx *ctx, knh_class_t cid, knh_methodn_t mn)
{
	knh_Method_t *mtd = new_Method(ctx, 0, cid, mn, knh_Fmethod_NoSuchMethod);
	return mtd;
}

/* ------------------------------------------------------------------------ */
/* [Method] */

void knh_Class_addMethod(Ctx *ctx, knh_class_t cid, knh_Method_t *mtd)
{
	DBG_ASSERT(IS_Method(mtd));
	DBG_ASSERT(cid == DP(mtd)->cid);
	size_t i;
	knh_Array_t *a = ClassTable(cid).methods;
	if(unlikely(a == KNH_EMPTYLIST)) {
		knh_ClassTable_t *t = pClassTable(ctx, cid);
		KNH_ASSERT(knh_Array_size(a) == 0);
		a = new_Array0(ctx, 1);
		KNH_SETv(ctx, t->methods, a);
	}
	for(i = 0; i < knh_Array_size(a); i++) {
		knh_Method_t *mtd2 = a->methods[i];
		if(DP(mtd2)->mn == DP(mtd)->mn) {
			KNH_SYSLOG(ctx, LOG_WARNING, "RedefinedMethod", "%C.%M", DP(mtd)->cid, DP(mtd)->mn);
			return ;
		}
	}
	if(knh_class_isSingleton(cid)) {
		DP(mtd)->flag = DP(mtd)->flag | FLAG_Method_Static;
	}
	knh_Array_add(ctx, a, mtd);
}

/* ------------------------------------------------------------------------ */
/* [VirtualField] */

static size_t knh_typeof(Ctx *ctx, knh_type_t t)
{
	if(IS_Tbool(t)) {
		return 1;
	}
	else if(IS_Tint(t)) {
		return 2;
	}
	else if(IS_Tfloat(t)) {
		return 3;
	}
	return 0;
}

/* ------------------------------------------------------------------------ */
/* [field_method] */

static METHOD knh_Fmethod_getter(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	int delta = DP(sfp[K_MTDIDX].callmtd)->delta;
	RETURN_((sfp[0].ox)->fields[delta]);
}

static METHOD knh_Fmethod_igetter(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	int delta = DP(sfp[K_MTDIDX].callmtd)->delta;
	knh_int_t *data = (knh_int_t*)(&(sfp[0].ox->fields[delta]));
	RETURNi_(data[0]);
}

static METHOD knh_Fmethod_fgetter(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	int delta = DP(sfp[K_MTDIDX].callmtd)->delta;
	knh_float_t *data = (knh_float_t*)(&(sfp[0].ox->fields[delta]));
	RETURNf_(data[0]);
}

static METHOD knh_Fmethod_bgetter(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	int delta = DP(sfp[K_MTDIDX].callmtd)->delta;
	knh_bool_t *data = (knh_bool_t*)(&(sfp[0].ox->fields[delta]));
	RETURNb_(data[0]);
}

static METHOD knh_Fmethod_setter(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	int delta = DP(sfp[K_MTDIDX].callmtd)->delta;
	klr_mov(ctx, (sfp[0].ox)->fields[delta], sfp[1].o);
	RETURN_(sfp[1].o);
}

static METHOD knh_Fmethod_bsetter(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	int delta = DP(sfp[K_MTDIDX].callmtd)->delta;
	knh_bool_t *data = (knh_bool_t*)(&((sfp[0].ox)->fields[delta]));
	data[0] = sfp[1].bvalue;
	RETURNb_(sfp[1].bvalue);
}

static METHOD knh_Fmethod_isetter(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	int delta = DP(sfp[K_MTDIDX].callmtd)->delta;
	knh_int_t *data = (knh_int_t*)(&(sfp[0].ox)->fields[delta]);
	data[0] = sfp[1].ivalue;
	RETURNi_(sfp[1].ivalue);
}

static METHOD knh_Fmethod_fsetter(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	int delta = DP(sfp[K_MTDIDX].callmtd)->delta;
	knh_float_t *data = (knh_float_t*)(&(sfp[0].ox)->fields[delta]);
	data[0] = sfp[1].fvalue;
	RETURNf_(sfp[1].fvalue);
}

static knh_Fmethod getter[] = {
		knh_Fmethod_getter,
		knh_Fmethod_bgetter,
		knh_Fmethod_igetter,
		knh_Fmethod_fgetter,
};

static knh_Fmethod setter[] = {
		knh_Fmethod_setter,
		knh_Fmethod_bsetter,
		knh_Fmethod_isetter,
		knh_Fmethod_fsetter,
};

static knh_Method_t *new_GetterMethod(Ctx *ctx, knh_class_t cid, knh_methodn_t mn, knh_type_t type, int idx)
{
	knh_Method_t *mtd = new_Method(ctx, 0, cid, mn, getter[knh_typeof(ctx, type)]);
	DP(mtd)->delta = idx;
	KNH_SETv(ctx, DP(mtd)->mp, new_ParamArrayR0(ctx, type));
	return mtd;
}

static knh_Method_t *new_SetterMethod(Ctx *ctx, knh_class_t cid, knh_methodn_t mn, knh_type_t type, int idx)
{
	knh_Method_t *mtd = new_Method(ctx, 0, cid, mn, setter[knh_typeof(ctx, type)]);
	DP(mtd)->delta = idx;
	KNH_SETv(ctx, DP(mtd)->mp, new_ParamArrayP1(ctx, type, type, FN_v));
	return mtd;
}

/* ------------------------------------------------------------------------ */

knh_index_t knh_Method_indexOfGetterField(knh_Method_t *o)
{
	knh_Fmethod f = SP(o)->fcall_1;
	if(f== knh_Fmethod_getter ||
		f == knh_Fmethod_igetter ||
		f == knh_Fmethod_fgetter ||
		f == knh_Fmethod_bgetter) {
		return (knh_index_t)DP(o)->delta;
	}
	return -1;
}

/* ------------------------------------------------------------------------ */

knh_index_t knh_Method_indexOfSetterField(knh_Method_t *o)
{
	knh_Fmethod f = SP(o)->fcall_1;
	if(f == knh_Fmethod_isetter ||
		f == knh_Fmethod_fsetter ||
		f == knh_Fmethod_bsetter) {
		return (knh_index_t)DP(o)->delta;
	}
	return -1;
}

///* ------------------------------------------------------------------------ */
//
//static knh_methodn_t knh_methodn_base(Ctx *ctx, knh_methodn_t mn)
//{
//	size_t i;
//	knh_bytes_t n = S_tobytes(knh_getFieldName(ctx, MN_toFN(mn)));
//	for(i = 0; i < n.len; i++) {
//		if(n.buf[i] == ':') {
//			n.len = i;
//			knh_fieldn_t fn = knh_getfnq(ctx, n, FN_NEWID);
//			return (fn | ((~K_FLAG_MN_FMT) & mn));
//		}
//	}
//	return mn;
//}

/* ------------------------------------------------------------------------ */

knh_Method_t* knh_findMethodNULL(Ctx *ctx, knh_class_t this_cid, knh_methodn_t mn, int isGEN)
{
	knh_class_t cid = this_cid;
	DBG_ASSERT_cid(cid);
	while (1) {
		size_t i;
		knh_Array_t *a = ClassTable(cid).methods;
		for(i = 0; i < knh_Array_size(a); i++) {
			knh_Method_t *mtd = a->methods[i];
			if(DP(mtd)->mn == mn) return mtd;
		}
		if(cid == CLASS_Object) {
			cid = this_cid;
			goto L_GenerateField;
		}
		cid = ClassTable(cid).supcid;
	}

	L_GenerateField:;
	cid = this_cid;
	if(MN_isGETTER(mn) || MN_isISBOOL(mn)) {
		knh_index_t idx = knh_Class_indexOfField(ctx, this_cid, MN_toFN(mn));
		if(idx == -1) {
			goto L_NoSuchMethod;
		}
		else {
			knh_fields_t *cf = knh_Class_fieldAt(ctx, this_cid, idx);
			if(MN_isISBOOL(mn) && !IS_Tbool(cf->type)) {
				goto L_NoSuchMethod;
			}
			if(!FLAG_is(cf->flag, FLAG_Field_Getter)) {
				goto L_NoSuchMethod;
			}
			else {
				knh_Method_t *mtd = new_GetterMethod(ctx, this_cid, mn, cf->type, idx);
				knh_Array_t *methods = ClassTable(this_cid).methods;
				knh_Array_add(ctx, methods, mtd);
				return mtd;
			}
		}
	}
	else if(MN_isSETTER(mn)) {
		knh_index_t idx = knh_Class_indexOfField(ctx, this_cid, MN_toFN(mn));
		if(idx == -1) {
			goto L_NoSuchMethod;
		}
		else {
			knh_fields_t *cf = knh_Class_fieldAt(ctx, this_cid, idx);
			if(!FLAG_is(cf->flag, FLAG_Field_Setter)) {
				goto L_NoSuchMethod;
			}
			else {
				knh_Method_t *mtd = new_SetterMethod(ctx, this_cid, mn, cf->type, idx);
				knh_Array_t *methods = ClassTable(this_cid).methods;
				knh_Array_add_(ctx, methods, UP(mtd));
				return mtd;
			}
		}
	}
//	else if(!MN_isFMT(mn)) {
//		knh_methodn_t mnbase = knh_methodn_base(ctx, mn);
//		if(mnbase != mn) {
//			DBG_P("searching again %s, %s", FN_tochar(MN_toFN(mn)), FN_tochar(MN_toFN(mnbase)));
//			mn = mnbase;
//			goto TAIL_RECURSION;
//		}
//	}

	L_NoSuchMethod:;
	if(isGEN) {
		if(MN_isFMT(mn)) {
			DBG_P("Generating %%empty: cid=%s mn=%%%s", CLASSN(cid), MN_tochar(mn));
			return knh_getMethodNULL(ctx, cid, MN__empty);
		}
		else {
			knh_Method_t *mtd = new_Method__NoSuchMethod(ctx, cid, mn);
			knh_Array_t *methods = ClassTable(this_cid).methods;
			knh_Array_add(ctx, methods, mtd);
			return mtd;
		}
	}
	else {
		return NULL;
	}
}

/* ------------------------------------------------------------------------ */

knh_Method_t *knh_lookupMethod(Ctx *ctx, knh_class_t cid, knh_methodn_t mn)
{
	DBG_ASSERT(!MN_isFMT(mn));
	knh_hashcode_t h = ((((knh_hashcode_t)cid) << (sizeof(knh_class_t) * 8)) + mn) % K_CACHESIZE;
	knh_Method_t *mtd = ctx->mtdCache[h];
	if(mtd != NULL) {
		if(DP(mtd)->mn == mn) {
			if(knh_Method_isPoly(mtd, cid)) {
				knh_stat_mtdCacheHit(ctx);
				return mtd;
			}
		}
		knh_stat_mtdCacheMiss(ctx);
		DBG_P("Cache missed[%ld]. looking up %s.%s", h, CLASSN(cid), MN_tochar(mn));
	}
	mtd = knh_findMethod(ctx, cid, mn);
	ctx->mtdCache[h] = mtd;
	return mtd;
}

/* ------------------------------------------------------------------------ */

knh_Method_t *knh_lookupFormatter(Ctx *ctx, knh_class_t cid, knh_methodn_t mn)
{
	DBG_ASSERT(MN_isFMT(mn));
	knh_hashcode_t h = ((((knh_hashcode_t)cid) << (sizeof(knh_class_t) * 8)) + mn) % K_CACHESIZE;
	knh_Method_t *mtd = ctx->fmtCache[h];
	if(mtd != NULL) {
		if(DP(mtd)->mn == mn) {
			if(knh_Method_isPoly(mtd, cid)) {
				knh_stat_fmtCacheHit(ctx);
				return mtd;
			}
		}
		knh_stat_fmtCacheMiss(ctx);
		DBG_P("Cache[%ld] missed. looking up %s.%%%s <%s>", h, CLASSN(cid), MN_tochar(mn), CLASSN(DP(mtd)->cid));
	}
	mtd = knh_findMethod(ctx, cid, mn);
	ctx->fmtCache[h] = mtd;
	DBG_ASSERT(IS_Method(mtd));
	return mtd;
}

///* ------------------------------------------------------------------------ */
///* [tracer] */
//
//
//METHOD knh_Fmethod_securityTrace(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	//knh_Method_t *mtd = sfp[K_MTDIDX].callmtd;
//
//}

///* ------------------------------------------------------------------------ */
//
//void knh_Method_trace(Ctx *ctx, knh_Method_t *mtd, int trace)
//{
////	DP(mtd)->prof_count = 0;
////	DP(mtd)->prof_time  = 0;
//	switch(trace) {
//		case KNH_SECURITYTRACE:
//			break;
//		case KNH_AUDITTRACE:
//			break;
//		case KNH_PROFILER:
//			(mtd)->fcall_1 = knh_Fmethod_profiler;
//			break;
//		case KNH_STACKTRACE:
//			(mtd)->fcall_1 = knh_Fmethod_stackTrace;
//			break;
//		case KNH_NOTRACE:
//		default:
//			(mtd)->fcall_1 = DP(mtd)->fproceed;
//	}
//}

/* ------------------------------------------------------------------------ */

char *knh_Method_file(Ctx *ctx, knh_Method_t *mtd)
{
	if(knh_Method_isObjectCode(mtd) && IS_KLRCode(DP(mtd)->kcode)) {
		knh_KLRCode_t *kcode = DP(mtd)->kcode;
		return FILEN(kcode->uri);
	}
	return "(naitive)";
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
