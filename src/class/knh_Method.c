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

/* ------------------------------------------------------------------------ */

METHOD knh_fmethod_abstract(Ctx *ctx, knh_sfp_t *sfp METHODOPT);

/* ======================================================================== */
/* [MethodField] */

knh_MethodField_t* new_MethodField(Ctx *ctx, knh_type_t rtype, size_t psize)
{
	knh_MethodField_t *mf = (knh_MethodField_t*)new_Object_init(ctx, FLAG_MethodField, CLASS_MethodField, psize);
	mf->rtype = rtype;
	return mf;
}

/* ------------------------------------------------------------------------ */

knh_hashcode_t knh_mparam_hcode(knh_hashcode_t h, knh_type_t type, knh_fieldn_t fn)
{
	h = h * 31 + type;
	return h + fn;
}

/* ------------------------------------------------------------------------ */
/* [param] */

void knh_MethodField_set(knh_MethodField_t *o, size_t n, knh_type_t type, knh_fieldn_t fn)
{
	//DBG2_P("o->psize=%d, n=%d", o->psize, n);
	if(o->psize > KNH_METHODFIELD_2) {
		DBG2_ASSERT(n < o->psize);
		o->mparams[n].type = type;
		o->mparams[n].fn = fn;
	}
	else if(n == 0) {
		DBG2_ASSERT(o->psize == 1 || o->psize == 2);
		o->p0.type = type;
		o->p0.fn= fn;
	}else {
		DBG2_ASSERT(n == 1 && o->psize == 2);
		o->p1.type = type;
		o->p1.fn = fn;
	}
}

/* ------------------------------------------------------------------------ */

knh_type_t knh_MethodField_pztype(knh_MethodField_t *o, size_t n)
{
	if(o->psize > KNH_METHODFIELD_2) {
		if(n < o->psize) {
			return o->mparams[n].type;
		}
		return o->mparams[o->psize - 1].type; /* last type */
	}
	else if(n == 0 || o->psize != 2) {
		return o->p0.type;  /* last type or void */
	}else {
		DBG2_ASSERT(n == 1 && o->psize == 2);
		return o->p1.type;
	}
}

/* ------------------------------------------------------------------------ */

knh_mparam_t knh_MethodField_param(knh_MethodField_t *o, size_t n)
{
	if(o->psize > KNH_METHODFIELD_2) {
		if(n < o->psize) {
			return o->mparams[n];
		}
		return o->mparams[o->psize - 1]; /* last param */
	}
	else if(n == 0 || o->psize != 2) {
		return o->p0;  /* last type or void */
	}else {
		DBG2_ASSERT(n == 1 && o->psize == 2);
		return o->p1;
	}
}

/* ------------------------------------------------------------------------ */

knh_type_t knh_Method_rtype(Ctx *ctx, knh_Method_t *mtd, knh_class_t cid)
{
	return knh_pmztype_totype(ctx, DP(mtd)->mf->rtype, cid);
}

/* ------------------------------------------------------------------------ */

knh_type_t knh_Method_ptype(Ctx *ctx, knh_Method_t *o, knh_class_t cid, size_t n)
{
	return knh_pmztype_totype(ctx, knh_MethodField_pztype(DP(o)->mf, n), cid);
}

/* ------------------------------------------------------------------------ */

knh_mparam_t knh_Method_param(knh_Method_t *o, size_t n)
{
	return knh_MethodField_param(DP(o)->mf, n);
}

/* ------------------------------------------------------------------------ */
/* [Type] */

int knh_MethodField_equalsType(knh_MethodField_t *o, knh_MethodField_t *o2)
{
	if(o->psize != o2->psize || o->rtype != o2->rtype) return 0;
	if(o->psize > KNH_METHODFIELD_2) {
		size_t i;
		for(i = 0; i < o->psize; i++) {
			if(o->mparams[i].type != o2->mparams[i].type) return 0;
		}
	}
	else {
		if(o->p0.type != o2->p0.type || o->p1.type != o2->p1.type) return 0;
	}
	return 1;
}

/* ======================================================================== */
/* [AbstractMethod] */

METHOD knh_fmethod_abstract(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Method_t *mtd = sfp[-1].mtd;
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_printf(ctx, cwb->w, "AbstractMethod!!: %C.%M", knh_Object_cid(sfp[0].o), DP(mtd)->mn);
	knh_stack_throw(ctx, ctx->esp, knh_cwb_newException(ctx, cwb), NULL, 0);
}

/* ------------------------------------------------------------------------ */

knh_bool_t knh_Method_isAbstract(knh_Method_t *o)
{
	return (DP(o)->fproceed == knh_fmethod_abstract);
}

/* ------------------------------------------------------------------------ */

void knh_Method_toAbstract(Ctx *ctx, knh_Method_t *o)
{
	if(knh_Method_isObjectCode(o)) {
		KNH_FINALv(ctx, DP(o)->code);
		knh_Method_setObjectCode(o, 0);
	}
	knh_Method_syncFunc(o, knh_fmethod_abstract);
	o->pc_start  = NULL;
}

/* ======================================================================== */
/* [Method] */

void knh_Method_syncFunc(knh_Method_t *o, knh_fmethod f)
{
	if(f == NULL) {
		f = knh_fmethod_abstract;
	}
	DP(o)->fproceed = f;
	(o)->fcall_1 = f;
}

/* ------------------------------------------------------------------------ */

knh_Method_t* new_Method(Ctx *ctx, knh_flag_t flag, knh_class_t cid, knh_methodn_t mn, knh_fmethod func)
{
	knh_Method_t* o = (knh_Method_t*)new_Object_bcid(ctx, CLASS_Method, 0);
	DP(o)->flag  = flag;
	DP(o)->cid    = cid;
	DP(o)->mn     = mn;
	if(METHODN_IS_MOVTEXT(mn)) {
		KNH_SETv(ctx, DP(o)->mf, knh_findMethodField0(ctx, TYPE_void));
	}
	func = (func == NULL) ? knh_fmethod_abstract : func;
	knh_Method_syncFunc(o, func);
	DP(o)->code  = NULL;
	o->pc_start  = NULL;
	return o;
}

/* ======================================================================== */
/* [NoSuchMethod] */

static
METHOD knh_fmethod_NoSuchMethod(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_Bytes_write(ctx, cwb->ba, STEXT("NoSuchMethod!!: "));
	knh_write_cid(ctx, cwb->w, knh_Object_cid(sfp[0].o));
	knh_putc(ctx, cwb->w, '.');
	knh_write_mn(ctx, cwb->w, knh_Method_rztype(sfp[-1].mtd), DP(sfp[-1].mtd)->mn);
	knh_stack_throw(ctx, ctx->esp, knh_cwb_newException(ctx, cwb), NULL, 0);
}

/* ------------------------------------------------------------------------ */

knh_bool_t knh_Method_isNoSuchMethod(knh_Method_t *o)
{
	return (DP(o)->fproceed == knh_fmethod_NoSuchMethod);
}

/* ------------------------------------------------------------------------ */

knh_Method_t* new_Method__NoSuchMethod(Ctx *ctx, knh_class_t cid, knh_methodn_t mn)
{
	knh_Method_t *mtd = new_Method(ctx, 0, cid, mn, knh_fmethod_NoSuchMethod);
	return mtd;
}

/* ======================================================================== */
/* [method type] */

int knh_methodn_isNew(Ctx *ctx, knh_methodn_t mn)
{
	if(mn == METHODN_new) return 1;
	if(!METHODN_IS_MOVTEXT(mn) && !METHODN_IS_GETTER(mn) && !METHODN_IS_SETTER(mn)) {
		char *n = FIELDN(METHODN_TOFIELDN(mn));
		if(n[0] == 'n' && n[1] == 'e' && n[2] == 'w' && n[3] == ':') {
			return 1;
		}
	}
	return 0;
}

/* ------------------------------------------------------------------------ */

int knh_Method_isConstructor(Ctx *ctx, knh_Method_t *o)
{
	if(IS_Method(o)) {
		knh_class_t rtype = CLASS_type(knh_Method_rztype(o));
		if(rtype != DP(o)->cid) {
			return 0;
		}
		return knh_methodn_isNew(ctx, DP(o)->mn);
	}
	return 0;
}

/* ------------------------------------------------------------------------ */

int knh_methodn_isOp(Ctx *ctx, knh_methodn_t mn)
{
	if(!METHODN_IS_MOVTEXT(mn) && !METHODN_IS_GETTER(mn) && !METHODN_IS_SETTER(mn)) {
		char *n = FIELDN(METHODN_TOFIELDN(mn));
		if(n[0] == 'o' && n[1] == 'p' && isupper(n[2])) {
			return 1;
		}
	}
	return 0;
}


/* ======================================================================== */
/* [methods] */

/* ------------------------------------------------------------------------ */

knh_String_t* knh_Method_getName(Ctx *ctx, knh_Method_t *o)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_write_mn(ctx, cwb->w, knh_Method_rztype(o), DP(o)->mn);
	return knh_cwb_newString(ctx, cwb);
}

/* ======================================================================== */
/* [Method] */

void knh_Class_addMethod(Ctx *ctx, knh_class_t cid, knh_Method_t *mtd)
{
	KNH_ASSERT(IS_Method(mtd));
	KNH_ASSERT(cid == DP(mtd)->cid);
	DBG2_ASSERT_cid(cid);
	knh_Array_t *methods = ClassTable(cid).methods;
	size_t i;
	for(i = 0; i < knh_Array_size(methods); i++) {
		knh_Method_t *mtd2 = (knh_Method_t*)knh_Array_n(methods, i);
		if(DP(mtd2)->mn == DP(mtd)->mn) {
			KNH_WARNING(ctx, "Duplicated method: %s", FIELDN(METHODN_TOFIELDN(DP(mtd)->mn)));
			return ;
		}
	}
	if(knh_class_isSingleton(cid)) {
		DP(mtd)->flag = DP(mtd)->flag | FLAG_Method_Static;
	}
	knh_Array_add(ctx, methods, UP(mtd));
}

/* ------------------------------------------------------------------------ */
/* [MethodField] */

void knh_addMethodFieldTable(Ctx *ctx, knh_MethodField_t *mf)
{
	knh_sfp_t lsfp = {{mf}, {0}};
	knh_hashcode_t h =
		(knh_hashcode_t)ClassTable(CLASS_MethodField).cspi->hashkey(ctx, &lsfp, KNH_FOBJECT_HASH);
	knh_HashMap_t *hmap = DP(ctx->sys)->MethodFieldHashMap;
	if(IS_NOTNULL(hmap)) {
		KNH_LOCK(ctx, LOCK_SYSTBL, NULL);
		knh_MethodField_t *mf2 = (knh_MethodField_t*)knh_HashMap_get__hcode(ctx, hmap, h);
		if(IS_MethodField(mf2)) {
			//DBG2_P("MISS!! %lu  rtype=%s%s psize=%d", h, TYPEQN(mf2->rtype), mf2->psize);
		}
		if(mf2->psize != 0) {
			knh_HashMap_set__hcode(ctx, hmap, h, UP(mf));
			DBG2_({
				mf2 = (knh_MethodField_t*)knh_HashMap_get__hcode(ctx, hmap, h);
				KNH_ASSERT(mf == mf2);
			});
		}
		KNH_UNLOCK(ctx, LOCK_SYSTBL, NULL);
	}
}

/* ------------------------------------------------------------------------ */

knh_MethodField_t *knh_findMethodField0(Ctx *ctx, knh_type_t rtype)
{
	knh_HashMap_t *hmap = DP(ctx->sys)->MethodFieldHashMap;
	if(IS_NOTNULL(hmap)) {
		knh_hashcode_t h = rtype;
		KNH_LOCK(ctx, LOCK_SYSTBL, NULL);
		knh_MethodField_t *mf = (knh_MethodField_t*)knh_HashMap_get__hcode(ctx, hmap, h);
		KNH_UNLOCK(ctx, LOCK_SYSTBL, NULL);
		if(IS_MethodField(mf)) {
			if(mf->rtype == rtype && mf->psize == 0) {
				return mf;
			}
		}
	}
	knh_MethodField_t *mf = new_MethodField(ctx, rtype, 0);
	knh_addMethodFieldTable(ctx, mf);
	return mf;
}

/* ------------------------------------------------------------------------ */

knh_MethodField_t *knh_findMethodField1(Ctx *ctx, knh_type_t rtype, knh_type_t p1, knh_fieldn_t fn1)
{
	knh_HashMap_t *hmap = DP(ctx->sys)->MethodFieldHashMap;
	if(IS_NOTNULL(hmap)) {
		knh_hashcode_t h = rtype;
		h = knh_mparam_hcode(h, p1, fn1);
		KNH_LOCK(ctx, LOCK_SYSTBL, NULL);
		knh_MethodField_t *mf = (knh_MethodField_t*)knh_HashMap_get__hcode(ctx, hmap, h);
		KNH_UNLOCK(ctx, LOCK_SYSTBL, NULL);
		if(IS_MethodField(mf)) {
			if(mf->rtype == rtype && mf->psize == 1 && mf->p0.type == p1 && mf->p0.fn == fn1) {
				return mf;
			}
		}
	}
	knh_MethodField_t *mf = new_MethodField(ctx, rtype, 1);
	mf->p0.type = p1;
	mf->p0.fn = fn1;
	knh_addMethodFieldTable(ctx, mf);
	return mf;
}

/* ------------------------------------------------------------------------ */
/* [field_method] */

static METHOD knh_fmethod_getter(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	DBG2_ASSERT(IS_Method(sfp[-1].mtd));
	int delta = DP(sfp[-1].mtd)->delta;
	KNH_RETURN(ctx, sfp, (sfp[0].ox)->fields[delta]);
}

/* ------------------------------------------------------------------------ */

static
METHOD knh_fmethod_igetter(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	DBG2_ASSERT(IS_Method(sfp[-1].mtd));
	int delta = DP(sfp[-1].mtd)->delta;
#ifdef KNH_USING_UNBOXFIELD
	knh_int_t *data = (knh_int_t*)(&(sfp[0].ox->fields[delta]));
	KNH_RETURN_Int(ctx, sfp, data[0]);
#else/*KNH_USING_UNBOXFIELD*/
	knh_Int_t *o = (knh_Int_t*)KNH_FIELDn(sfp[0].o, DP(sfp[-1].mtd)->delta);
	KNH_RETURN_Int(ctx, sfp, o->n.ivalue);
#endif/*KNH_USING_UNBOXFIELD*/
}

/* ------------------------------------------------------------------------ */

static
METHOD knh_fmethod_fgetter(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	DBG2_ASSERT(IS_Method(sfp[-1].mtd));
	int delta = DP(sfp[-1].mtd)->delta;
#ifdef KNH_USING_UNBOXFIELD
	knh_float_t *data = (knh_float_t*)(&(sfp[0].ox->fields[delta]));
	KNH_RETURN_Float(ctx, sfp, data[0]);
#else/*KNH_USING_UNBOXFIELD*/
	knh_Float_t *o = (knh_Float_t*)KNH_FIELDn(sfp[0].o, DP(sfp[-1].mtd)->delta);
	KNH_RETURN_Float(ctx, sfp, o->n.fvalue);
#endif/*KNH_USING_UNBOXFIELD*/
}

/* ------------------------------------------------------------------------ */

static
METHOD knh_fmethod_bgetter(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	DBG2_ASSERT(IS_Method(sfp[-1].mtd));
	int delta = DP(sfp[-1].mtd)->delta;
#ifdef KNH_USING_UNBOXFIELD
	knh_bool_t *data = (knh_bool_t*)(&(sfp[0].ox->fields[delta]));
	KNH_RETURN_Boolean(ctx, sfp, data[0]);
#else/*KNH_USING_UNBOXFIELD*/
	knh_Boolean_t *o = (knh_Boolean_t*)KNH_FIELDn(sfp[0].o, DP(sfp[-1].mtd)->delta);
	KNH_RETURN_Boolean(ctx, sfp, o->n.bvalue);
#endif/*KNH_USING_UNBOXFIELD*/
}

/* ------------------------------------------------------------------------ */

static
METHOD knh_fmethod_setter(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	DBG2_ASSERT(IS_Method(sfp[-1].mtd));
	int delta = DP(sfp[-1].mtd)->delta;
	KNH_MOV(ctx, (sfp[0].ox)->fields[delta], sfp[1].o);
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */

static
METHOD knh_fmethod_bsetter(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	DBG2_ASSERT(IS_Method(sfp[-1].mtd));
	int delta = DP(sfp[-1].mtd)->delta;
#ifdef KNH_USING_UNBOXFIELD
	knh_bool_t *data = (knh_bool_t*)(&((sfp[0].ox)->fields[delta]));
	data[0] = sfp[1].bvalue;
#else/*KNH_USING_UNBOXFIELD*/
	KNH_MOV(ctx, KNH_FIELDn(sfp[0].o, DP(sfp[-1].mtd)->delta), new_Boolean(ctx, sfp[1].bvalue));
#endif/*KNH_USING_UNBOXFIELD*/
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */

static
METHOD knh_fmethod_isetter(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	DBG2_ASSERT(IS_Method(sfp[-1].mtd));
	int delta = DP(sfp[-1].mtd)->delta;
#ifdef KNH_USING_UNBOXFIELD
	knh_int_t *data = (knh_int_t*)(&(sfp[0].ox)->fields[delta]);
	data[0] = sfp[1].ivalue;
#else/*KNH_USING_UNBOXFIELD*/
	knh_Int_t *n = new_Int(ctx, sfp[1].ivalue);
	KNH_MOV(ctx, KNH_FIELDn(sfp[0].o, DP(sfp[-1].mtd)->delta), n);
#endif/*KNH_USING_UNBOXFIELD*/
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */

static
METHOD knh_fmethod_insetter(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	DBG2_ASSERT(IS_Method(sfp[-1].mtd));
	int delta = DP(sfp[-1].mtd)->delta;
	knh_Int_t *n = IS_NULL(sfp[1].o) ? sfp[1].i : new_Int(ctx, sfp[1].ivalue);
	KNH_MOV(ctx, (sfp[0].ox)->fields[delta], n);
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */

static
METHOD knh_fmethod_fsetter(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	DBG2_ASSERT(IS_Method(sfp[-1].mtd));
	int delta = DP(sfp[-1].mtd)->delta;
#ifdef KNH_USING_UNBOXFIELD
	knh_float_t *data = (knh_float_t*)(&(sfp[0].ox)->fields[delta]);
	data[0] = sfp[1].fvalue;
#else/*KNH_USING_UNBOXFIELD*/
	knh_Float_t *n = new_Float(ctx, sfp[1].fvalue);
	KNH_MOV(ctx, KNH_FIELDn(sfp[0].o, DP(sfp[-1].mtd)->delta), n);
#endif/*KNH_USING_UNBOXFIELD*/
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */

static
METHOD knh_fmethod_fnsetter(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	DBG2_ASSERT(IS_Method(sfp[-1].mtd));
	int delta = DP(sfp[-1].mtd)->delta;
	knh_Float_t *n = IS_NULL(sfp[1].o) ? sfp[1].f : new_Float(ctx, sfp[1].fvalue);
	KNH_MOV(ctx, (sfp[0].ox)->fields[delta], n);
	KNH_RETURN_void(ctx, sfp);
}


/* ------------------------------------------------------------------------ */

static
knh_Method_t *new_Method_getter(Ctx *ctx, knh_class_t cid, knh_methodn_t mn, knh_type_t type, int idx)
{
	knh_fmethod f = knh_fmethod_getter;
	if(IS_ubxint(type)) {
		f = knh_fmethod_igetter;
	}
	else if(IS_ubxfloat(type)) {
		f = knh_fmethod_fgetter;
	}
	else if(IS_ubxboolean(type)) {
		f = knh_fmethod_bgetter;
	}
	knh_Method_t *mtd = new_Method(ctx, 0, cid, mn, f);
	DP(mtd)->delta = idx;
	KNH_SETv(ctx, DP(mtd)->mf, knh_findMethodField0(ctx, type));
	return mtd;
}

/* ------------------------------------------------------------------------ */

static
knh_Method_t *new_Method_setter(Ctx *ctx, knh_class_t cid, knh_methodn_t mn, knh_type_t type, int idx)
{
	knh_fmethod f = knh_fmethod_setter;
	if(IS_ubxint(type)) {
		f = knh_fmethod_isetter;
	}
	else if(IS_bxint(type)) {
		f = knh_fmethod_insetter;
	}
	else if(IS_ubxfloat(type)) {
		f = knh_fmethod_fsetter;
	}
	else if(IS_bxfloat(type)) {
		f = knh_fmethod_fnsetter;
	}
	else if(IS_ubxboolean(type)) {
		f = knh_fmethod_bsetter;
	}
	knh_Method_t *mtd = new_Method(ctx, 0, cid, mn, f);
	DP(mtd)->delta = idx;
	KNH_SETv(ctx, DP(mtd)->mf, knh_findMethodField1(ctx, TYPE_void, type, FIELDN_v));
	return mtd;
}

/* ------------------------------------------------------------------------ */

knh_index_t knh_Method_indexOfGetter(knh_Method_t *o)
{
	knh_fmethod f = SP(o)->fcall_1;
	if(f== knh_fmethod_getter ||
		f == knh_fmethod_igetter ||
		f == knh_fmethod_fgetter ||
		f == knh_fmethod_bgetter) {
		return (knh_index_t)DP(o)->delta;
	}
	return -1;
}

/* ------------------------------------------------------------------------ */

knh_index_t knh_Method_indexOfSetter(knh_Method_t *o)
{
	knh_fmethod f = SP(o)->fcall_1;
	if(f == knh_fmethod_isetter ||
		f == knh_fmethod_insetter ||
		f == knh_fmethod_fsetter ||
		f == knh_fmethod_fnsetter ||
		f == knh_fmethod_bsetter) {
		return (knh_index_t)DP(o)->delta;
	}
	return -1;
}

/* ------------------------------------------------------------------------ */

static knh_methodn_t knh_methodn_base(Ctx *ctx, knh_methodn_t mn)
{
	size_t i;
	knh_bytes_t n = __tobytes(knh_getFieldName(ctx, METHODN_TOFIELDN(mn)));
	for(i = 0; i < n.len; i++) {
		if(n.buf[i] == ':') {
			n.len = i;
			knh_fieldn_t fn = knh_getfnq(ctx, n, FIELDN_NEWID);
			return (fn | ((~KNH_FLAG_MN_MOVTEXT) & mn));
		}
	}
	return mn;
}

/* ------------------------------------------------------------------------ */

knh_Method_t*
knh_Class_getMethod__(Ctx *ctx, knh_class_t this_cid, knh_methodn_t mn, knh_bool_t gen)
{
	knh_class_t cid = this_cid;
	TAIL_RECURSION:;
	DBG2_ASSERT_cid(cid);
	{
		knh_Array_t *methods = ClassTable(cid).methods;
		size_t i;
		for(i = 0; i < knh_Array_size(methods); i++) {
			knh_Method_t *mtd = (knh_Method_t*)knh_Array_n(methods, i);
			if(DP(mtd)->mn == mn) return mtd;
		}
		if(cid == CLASS_Object) {
			cid = this_cid;
			goto L_GenerateField;
		}
		cid = ClassTable(cid).supcid;
	}
	goto TAIL_RECURSION;

	L_GenerateField:;

	if(METHODN_IS_GETTER(mn)) {
		knh_index_t idx = knh_Class_indexOfField(ctx, this_cid, METHODN_TOFIELDN(mn));
		if(idx == -1) {
			goto L_NoSuchMethod;
		}
		else {
			knh_fields_t *cf = knh_Class_fieldAt(ctx, this_cid, idx);
			if(!KNH_FLAG_IS(cf->flag, FLAG_Field_Getter)) {
				goto L_NoSuchMethod;
			}
			else {
				knh_Method_t *mtd = new_Method_getter(ctx, this_cid, mn, cf->type, idx);
				knh_Array_t *methods = ClassTable(this_cid).methods;
				knh_Array_add(ctx, methods, UP(mtd));
				return mtd;
			}
		}
	}
	else if(METHODN_IS_SETTER(mn)) {
		knh_index_t idx = knh_Class_indexOfField(ctx, this_cid, METHODN_TOFIELDN(mn));
		if(idx == -1) {
			goto L_NoSuchMethod;
		}
		else {
			knh_fields_t *cf = knh_Class_fieldAt(ctx, this_cid, idx);
			if(!KNH_FLAG_IS(cf->flag, FLAG_Field_Setter)) {
				goto L_NoSuchMethod;
			}
			else {
				knh_Method_t *mtd = new_Method_setter(ctx, this_cid, mn, cf->type, idx);
				knh_Array_t *methods = ClassTable(this_cid).methods;
				knh_Array_add(ctx, methods, UP(mtd));
				return mtd;
			}
		}
	}
	else if(!METHODN_IS_MOVTEXT(mn)) {
		knh_methodn_t mnbase = knh_methodn_base(ctx, mn);
		if(mnbase != mn) {
			DBG2_P("searching again %s, %s", FIELDN(METHODN_TOFIELDN(mn)), FIELDN(METHODN_TOFIELDN(mnbase)));
			mn = mnbase;
			goto TAIL_RECURSION;
		}
	}

	L_NoSuchMethod:;
	if(gen) {
		if(METHODN_IS_MOVTEXT(mn)) {
			DBG2_P("Generating %%empty: cid=%s mn=%%%s", CLASSN(cid), METHODN(mn));
			return knh_Class_getMethod(ctx, cid, METHODN__empty);
		}
		else {
			knh_Method_t *mtd = new_Method__NoSuchMethod(ctx, cid, mn);
			knh_Array_t *methods = ClassTable(this_cid).methods;
			knh_Array_add(ctx, methods, UP(mtd));
			return mtd;
		}
	}
	else {
		return (knh_Method_t*)KNH_NULL;
	}
}

/* ======================================================================== */
/* [utils] */

knh_Method_t *knh_lookupMethod(Ctx *ctx, knh_class_t cid, knh_methodn_t mn)
{
	KNH_ASSERT(!METHODN_IS_MOVTEXT(mn));
	knh_hashcode_t h = ((((knh_hashcode_t)cid) << (sizeof(knh_class_t) * 8)) + mn) % ctx->cachesize;
	knh_Method_t *mtd = ctx->mtdCache[h];
	if(mtd != NULL) {
		if(DP(mtd)->mn == mn) {
			if(DP(mtd)->cid == cid || knh_class_instanceof(ctx, cid, DP(mtd)->cid)) {
				knh_stat_mtdCacheHit(ctx);
				return mtd;
			}
		}
		knh_stat_mtdCacheMiss(ctx);
		DBG2_P("Cache missed[%ld]. looking up %s.%s", h, CLASSN(cid), METHODN(mn));
	}
	mtd = knh_Class_findMethod(ctx, cid, mn);
	ctx->mtdCache[h] = mtd;
	return mtd;
}

/* ------------------------------------------------------------------------ */

knh_Method_t *knh_lookupFormatter(Ctx *ctx, knh_class_t cid, knh_methodn_t mn)
{
	DBG2_ASSERT(METHODN_IS_MOVTEXT(mn));
	knh_hashcode_t h = ((((knh_hashcode_t)cid) << (sizeof(knh_class_t) * 8)) + mn) % ctx->cachesize;
	knh_Method_t *mtd = ctx->fmtCache[h];
	if(mtd != NULL) {
		if(DP(mtd)->mn == mn) {
			if(DP(mtd)->cid == cid || knh_class_instanceof(ctx, cid, DP(mtd)->cid)) {
				knh_stat_fmtCacheHit(ctx);
				return mtd;
			}
		}
		knh_stat_fmtCacheMiss(ctx);
		DBG2_P("Cache[%ld] missed. looking up %s.%%%s <%s>", h, CLASSN(cid), METHODN(mn), CLASSN(DP(mtd)->cid));
	}
	mtd = knh_Class_findMethod(ctx, cid, mn);
	ctx->fmtCache[h] = mtd;
	return mtd;
}

/* ------------------------------------------------------------------------ */
/* [tracer] */


METHOD knh_fmethod_securityTrace(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	//knh_Method_t *mtd = sfp[-1].mtd;

}

/* ------------------------------------------------------------------------ */

void knh_Method_trace(Ctx *ctx, knh_Method_t *mtd, int trace)
{
	DP(mtd)->prof_count = 0;
	DP(mtd)->prof_time  = 0;
	switch(trace) {
		case KNH_SECURITYTRACE:
			break;
		case KNH_AUDITTRACE:
			break;
		case KNH_PROFILER:
			(mtd)->fcall_1 = knh_fmethod_profiler;
			break;
		case KNH_STACKTRACE:
			(mtd)->fcall_1 = knh_fmethod_stackTrace;
			break;
		case KNH_NOTRACE:
		default:
			(mtd)->fcall_1 = DP(mtd)->fproceed;
	}
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
