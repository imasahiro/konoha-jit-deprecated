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
#define USE_bytes_startsWith 1
#define USE_cwb_open      1

#include"commons.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ======================================================================== */
/* [ClassTable] */

char *knh_getStructName(Ctx *ctx, knh_class_t bcid)
{
	if(bcid < ctx->share->ClassTableSize) {
		return ClassTable(bcid).cspi->name;
	}
	DBG_P("unknown bcid=%d", bcid);
	return "STRUCT_unknown";
}

/* ------------------------------------------------------------------------ */

knh_Class_t *new_Type(Ctx *ctx, knh_type_t type)
{
	knh_class_t cid = CLASS_type(type);
	knh_ClassTable_t *t = pClassTable(ctx, cid);
	if(t->typeNULL == NULL) {
		knh_Class_t *o = new_(Class);
		o->cid = cid;
		o->type = type;
		KNH_INITv(t->typeNULL, o);
	}
	return t->typeNULL;
}

/* ------------------------------------------------------------------------ */

void knh_write_sname(Ctx *ctx, knh_OutputStream_t *w, knh_class_t cid)
{
	if(cid == TYPE_void) {
		knh_write(ctx, w, STEXT("void"));
	}
	else if(cid == TYPE_This) {
		knh_write(ctx, w, STEXT("This"));
	}
	else if(cid == TYPE_var) {
		knh_write(ctx, w, STEXT("var"));
	}
	else if(cid > TYPE_This) {
		knh_write(ctx, w, STEXT("T"));
		knh_write_ifmt(ctx, w, K_INT_FMT, cid - TYPE_This);
	}
	else {
		knh_write(ctx, w, S_tobytes(ClassTable(cid).sname));
	}
}

/* ------------------------------------------------------------------------ */

void knh_write_cid(Ctx *ctx, knh_OutputStream_t *w, knh_class_t cid)
{
	if(cid == TYPE_void) {
		knh_write(ctx, w, STEXT("void"));
	}
	else if(cid == TYPE_This) {
		knh_write(ctx, w, STEXT("This"));
	}
	else if(cid > TYPE_This) {
		knh_write(ctx, w, STEXT("T"));
		knh_write_ifmt(ctx, w, K_INT_FMT, cid - TYPE_This);
	}
	else if(cid == TYPE_var) {
		knh_write(ctx, w, STEXT("var"));
	}
	else {
		knh_write(ctx, w, S_tobytes(ClassTable(cid).lname));
	}
}

/* ------------------------------------------------------------------------ */

static knh_Object_t *knh_Fdefnull_newValue(Ctx *ctx, knh_class_t cid);

/* ------------------------------------------------------------------------ */

void knh_setClassName(Ctx *ctx, knh_class_t cid, knh_String_t *lname, knh_String_t *snameNULL)
{
	DBG_ASSERT_cid(cid);
	knh_ClassTable_t *t = pClassTable(ctx, cid);
	DBG_ASSERT(t->lname == NULL);
	KNH_INITv(t->lname, lname);
	if(snameNULL != NULL) {
		KNH_INITv(t->sname, snameNULL);
	}
	else if(t->bcid == cid) {
		KNH_INITv(t->sname, new_T(t->cspi->name));
	}
//	else if(t->bcid == CLASS_Array) {
//		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
//		knh_write_sname(ctx, cwb->w, knh_class_p1(cid));
//		knh_write(ctx, cwb->w, STEXT("[]"));
//		KNH_INITv(t->sname, knh_cwb_newString(ctx, cwb));
//	}
//	else if(t->bcid == CLASS_Iterator) {
//		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
//		knh_write_sname(ctx, cwb->w, knh_class_p1(cid));
//		knh_write(ctx, cwb->w, STEXT(".."));
//		KNH_INITv(t->sname, knh_cwb_newString(ctx, cwb));
//	}
	if(t->sname == NULL) {
		KNH_INITv(t->sname, t->lname);
	}
	KNH_LOCK(ctx, LOCK_SYSTBL, NULL);
	{
		knh_DictSet_append(ctx, DP(ctx->sys)->ClassNameDictSet, lname, cid+1);
		if(S_startsWith(lname, STEXT("konoha."))) {
			knh_DictSet_append(ctx, DP(ctx->sys)->ClassNameDictSet, t->sname, cid+1);
		}
	}
	KNH_UNLOCK(ctx, LOCK_SYSTBL, NULL);
	if(cid > 54) {
		DBG_P("class=%d, lname=%s, sname=%s", cid, S_tochar(lname), S_tochar(t->sname));
	}
	if(t->fdefnull == NULL) {
		t->fdefnull = knh_Fdefnull_newValue;
	}
}

/* ------------------------------------------------------------------------ */
/* [name] */

knh_class_t knh_getcid(Ctx *ctx, knh_bytes_t lname)
{
	knh_uintptr_t cid = knh_DictSet_get(DP(ctx->sys)->ClassNameDictSet, lname);
	if(cid > 0) return (knh_class_t)(cid-1);
#if defined(K_USING_SEMANTICS)
	if(lname.buf[lname.len-1] == '}') {
		return knh_findcidx(ctx, lname);
	}
#endif/*K_USING_SEMANTICS*/
	return CLASS_unknown;
}

/* ------------------------------------------------------------------------ */

char* knh_Context_CLASSN(Ctx *ctx, knh_class_t cid)
{
	DBG_ASSERT_cid(cid);
	knh_String_t *name = ClassTable(cid).lname;
	if(S_startsWith(name, STEXT("konoha."))) {
		return S_tochar(ClassTable(cid).sname);
	}
	else {
		knh_class_t cid2 = knh_NameSpace_getcid(ctx, DP(ctx->gma)->ns, S_tobytes(ClassTable(cid).sname));
		if(cid == cid2) {
			return S_tochar(ClassTable(cid).sname);
		}
		return CLASSN(cid);
	}
}

char *knh_ClassTable_CLASSN(Ctx *ctx, knh_class_t cid)
{
	if(cid == CLASS_unknown) {
		return "unknown";
	}
	else {
		DBG_ASSERT_cid(cid);
		DBG_ASSERT(ClassTable(cid).sname != NULL);
		if(S_startsWith(ClassTable(cid).lname, STEXT("konoha."))) {
			return S_tochar(ClassTable(cid).sname);
		}
		else {
			return S_tochar(ClassTable(cid).lname);
		}
	}
}

/* ------------------------------------------------------------------------ */
/* [default] */

static knh_Object_t *knh_Fdefnull_getConst(Ctx *ctx, knh_class_t cid)
{
	return ClassTable(cid).defnull;
}

static knh_Object_t *knh_Fdefnull_newValue(Ctx *ctx, knh_class_t cid)
{
	knh_ClassTable_t *t = pClassTable(ctx, cid);
	Object *v = new_Object_init2(ctx, FLAG_Object_Immutable | FLAG_Object_NullObject, t->bcid, cid);
	DBG_P("create new default value of %s", CLASSN(cid));
	KNH_ASSERT(t->defnull == NULL);
	KNH_INITv(t->defnull, v);
	t->fdefnull = knh_Fdefnull_getConst;
	return t->defnull;
}

/* ------------------------------------------------------------------------ */

void knh_setClassDefaultValue_(Ctx *ctx, knh_class_t cid, Object *value, knh_Fdefnull f)
{
	DBG_ASSERT_cid(cid);
	knh_ClassTable_t *t = pClassTable(ctx, cid);
	if(value == NULL) {
		if(f == NULL) f = knh_Fdefnull_newValue;
	}
	else {
		KNH_ASSERT(t->defnull == NULL);
		KNH_INITv(t->defnull, value);
		if(f == NULL) f = knh_Fdefnull_getConst;
	}
	t->fdefnull = f;
}

/* ------------------------------------------------------------------------ */

Object *knh_getClassDefaultValue(Ctx *ctx, knh_class_t cid)
{
	DBG_ASSERT_cid(cid);
	return ClassTable(cid).fdefnull(ctx, cid);
}

/* ======================================================================== */
/* [field] */

knh_index_t knh_Class_indexOfField(Ctx *ctx, knh_class_t cid, knh_fieldn_t fn)
{
	DBG_ASSERT_cid(cid);
	while(1) {
		knh_ClassTable_t *t = pClassTable(ctx, cid);
		if(t->fields != NULL) {
			knh_index_t idx;
			for(idx = 0; idx < t->fsize; idx++) {
				knh_fieldn_t fn2 = t->fields[idx].fn;
				if(fn2 == FN_NONAME) return -1;
				if(fn2 == fn) return t->offset + idx;
			}
		}
		if(t->offset == 0) return -1;
		cid = t->supcid;
	}
}

/* ------------------------------------------------------------------------ */

knh_index_t knh_Class_queryField(Ctx *ctx, knh_class_t cid, knh_fieldn_t fnq)
{
	knh_fieldn_t fn = FN_UNMASK(fnq);
	DBG_ASSERT_cid(cid);
	while(1) {
		knh_ClassTable_t *t = pClassTable(ctx, cid);
		if(FN_isSUPER(fnq)) {
			fnq = fn;
			goto L_SUPER;
		}
		if(t->fields != NULL) {
			knh_index_t idx;
			for(idx = t->fsize - 1; idx >= 0; idx--) {
				if(t->fields[idx].fn == fn) {
					return t->offset + idx;
				}
			}
		}
		L_SUPER:;
		if(t->offset == 0) return -1;
		cid = t->supcid;
	}
}

/* ------------------------------------------------------------------------ */

knh_fields_t *knh_Class_fieldAt(Ctx *ctx, knh_class_t cid, size_t n)
{
	DBG_ASSERT_cid(cid);
	KNH_ASSERT(/*0 <= n &&*/ n < ClassTable(cid).size);
	while(1) {
		knh_ClassTable_t *t = pClassTable(ctx, cid);
		size_t offset = ClassTable(cid).offset;
		if(offset <= n) {
			if(t->fields == NULL) {
				return NULL;
			}
			else {
				return &(t->fields[n - offset]);
			}
		}
		cid = t->supcid;
	}
}

/* ------------------------------------------------------------------------ */

int knh_ClassMap_util_cmp(const knh_Translator_t *m1, const knh_Translator_t *m2)
{
	int res = DP(m1)->flag - DP(m2)->flag;
	return (res == 0) ? SP(m2)->tcid - SP(m1)->tcid : res;
}

///* ------------------------------------------------------------------------ */
//
//void knh_ClassMap_sort(Ctx *ctx, knh_ClassMap_t *cmap)
//{
//	if(!knh_ClassMap_isSorted(cmap)) {
//		knh_qsort((cmap)->maplist, (cmap)->size, sizeof(knh_Translator_t*),
//					(int (*)(const void*, const void*))knh_ClassMap_util_cmp);
//		knh_ClassMap_setSorted(cmap, 1);
//	}
//}

/* ------------------------------------------------------------------------ */

static knh_Array_t* knh_Class_domain(Ctx *ctx)
{
	knh_Array_t *a = new_Array(ctx, CLASS_Class, 0);
	size_t cid = 0;
	for(cid = 0; cid < ctx->share->ClassTableSize; cid++) {
		if(knh_class_isPrivate(cid) || knh_class_isTypeVariable(cid)) continue;
		knh_Array_add_(ctx, a, UP(new_Type(ctx, cid)));
	}
	return a;
}

/* ------------------------------------------------------------------------ */

static knh_Array_t* knh_Method_domain(Ctx *ctx)
{
	knh_Array_t *a = new_Array(ctx, CLASS_Method, 0);
	size_t cid = 0;
	for(cid = 0; cid < ctx->share->ClassTableSize; cid++) {
		knh_Array_t *ma = ClassTable(cid).methods;
		size_t i;
		for(i = 0; i < knh_Array_size(ma); i++) {
			knh_Method_t *mtd = (knh_Method_t*)knh_Array_n(ma, i);
			if(DP(mtd)->cid == cid) {
				knh_Array_add_(ctx, a, UP(mtd));
			}
		}
	}
	return a;
}

/* ------------------------------------------------------------------------ */

knh_Array_t* knh_getClassDomain(Ctx *ctx, knh_class_t cid)
{
	knh_Array_t *a = NULL;
	switch(cid) {
	case CLASS_Class:
		a = knh_Class_domain(ctx);
		break;
	case CLASS_Method:
		a = knh_Method_domain(ctx);
		break;
	default:
		a = NULL;
	}
	if(a == NULL) {
		DBG_P("Empty domain cid=%s", CLASSN(cid));
		a = new_Array0(ctx, 0);
	}
	return a;
}

/* ------------------------------------------------------------------------ */
/* [instaceof] */

knh_bool_t knh_class_instanceof(Ctx *ctx, knh_class_t scid, knh_class_t tcid)
{
	DBG_ASSERT_cid(scid); DBG_ASSERT_cid(tcid);
	if(scid == tcid || tcid == CLASS_Object || tcid == CLASS_Any ) return 1;
//  if(ClassTable(scid).bcid == tcid) return 1; /* Int:km Int */
//	if(knh_class_isGenerics(scid)) {
//		// Immutable simulates covariance of generics
//		if(knh_class_isImmutable(scid) && ClassTable(scid).bcid == ClassTable(tcid).bcid) {
//			// Iterator<C> Iterator<D>;
//			return (knh_class_instanceof(ctx, ClassTable(scid).p1, ClassTable(tcid).p1) &&
//					knh_class_instanceof(ctx, ClassTable(scid).p2, ClassTable(tcid).p2));
//		}
//	}
//	else {
		while(scid != CLASS_Object) {
			knh_Array_t *a = ClassTable(scid).tmaps;
			size_t i;
			for(i = 0; i < knh_Array_size(a); i++) {
				knh_Translator_t *trl = a->trans[i];
				if(SP(trl)->tcid == tcid && DP(trl)->flag == FLAG_Translator_Interface) return 1;
			}
			scid = ClassTable(scid).supcid;
			if(scid == tcid) return 1;
		}
//	}
	return 0;
}

/* ------------------------------------------------------------------------ */

knh_bool_t knh_Object_opTypeOf(Ctx *ctx, Object *o, knh_type_t t)
{
	return knh_class_instanceof(ctx, o->h.cid, CLASS_type(t));
}

/* ------------------------------------------------------------------------ */
/* [class param] */

int knh_class_isGenerics(Ctx *ctx, knh_class_t cid)
{
	DBG_ASSERT_cid(cid);
	return (ClassTable(cid).cparam != NULL && IS_ParamArray(ClassTable(cid).cparam));
}

/* ------------------------------------------------------------------------ */

knh_class_t knh_class_p(Ctx *ctx, knh_class_t cid, size_t n)
{
	knh_param_t *p = knh_ParamArray_get(ClassTable(cid).cparam, n);
	return p->type;
}

/* ------------------------------------------------------------------------ */

void knh_write_typeF(Ctx *ctx, knh_OutputStream_t *w, knh_type_t type, knh_Fwritecid f)
{
	knh_class_t cid = CLASS_type(type);
	if(cid < ctx->share->ClassTableSize) {
		knh_class_t bcid = knh_class_bcid(cid);
		if(knh_class_isGenerics(ctx, cid) && cid != bcid) {
			int i;
			knh_ParamArray_t *pa = ClassTable(cid).cparam;
			f(ctx, w, bcid);
			knh_putc(ctx, w, '<');
			for(i = 0; i < pa->psize; i++) {
				knh_param_t *p = knh_ParamArray_get(pa, i);
				if(i > 0) knh_putc(ctx, w, ',');
				knh_write_typeF(ctx, w, p->type, f);
			}
			if(pa->rsize > 0) knh_write(ctx, w, STEXT("=>"));
			for(i = 0; i < pa->rsize; i++) {
				knh_param_t *p = knh_ParamArray_rget(pa, i);
				if(i > 0) knh_putc(ctx, w, ',');
				knh_write_typeF(ctx, w, p->type, f);
			}
			knh_putc(ctx, w, '>');
			return;
		}
	}
	f(ctx, w, cid);
}

/* ------------------------------------------------------------------------ */

knh_class_t knh_addGenericsClass(Ctx *ctx, knh_class_t cid, knh_class_t bcid, knh_ParamArray_t *pa)
{
	knh_ClassTable_t *t, *bt;
	if(cid == CLASS_newid) cid = new_ClassId(ctx);
	DBG_ASSERT(bcid < cid);
	t = pClassTable(ctx, cid);
	bt = pClassTable(ctx, bcid);
	t->cflag  = bt->cflag;
	t->oflag  = bt->oflag;
	t->cspi    = bt->cspi;
	t->bcid   = bcid;
	t->supcid = bt->supcid;
	t->offset = bt->offset;
	t->size = bt->size;
	KNH_INITv(t->methods, bt->methods);
	KNH_INITv(t->cparam, pa);
	DBG_ASSERT(bt->fields == NULL);
	KNH_INITv(t->tmaps, KNH_EMPTYLIST);
	{
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		knh_String_t *lname, *sname;
		DBG_ASSERT(t->lname == NULL);
		knh_write_typeF(ctx, cwb->w, cid, knh_write_cid);
		lname = knh_cwb_newString(ctx, cwb);
		knh_write_typeF(ctx, cwb->w, cid, knh_write_sname);
		sname = knh_cwb_newString(ctx, cwb);
		knh_setClassName(ctx, cid, lname, sname);
	}
	return cid;
}

/* ------------------------------------------------------------------------ */

knh_class_t knh_class_Generics(Ctx *ctx, knh_class_t bcid, knh_ParamArray_t *pa)
{
	size_t i;
	for(i = 0; i < ctx->share->ClassTableSize; i++) {
		knh_ClassTable_t *t = pClassTable(ctx, i);
		if(t->bcid == bcid && knh_ParamArray_equalsType(pa, t->cparam)) {
			return (knh_class_t)i;
		}
	}
	return knh_addGenericsClass(ctx, CLASS_newid, bcid, pa);
}

/* ------------------------------------------------------------------------ */

knh_class_t knh_class_P1(Ctx *ctx, knh_class_t bcid, knh_type_t p1)
{
	size_t i;
	for(i = 0; i < ctx->share->ClassTableSize; i++) {
		knh_ClassTable_t *t = pClassTable(ctx, i);
		if(t->bcid != bcid) continue;
		if(t->cparam->psize == 1) {
			knh_param_t *p = knh_ParamArray_get(t->cparam, 0);
			if(p->type == p1) return (knh_class_t)i;
		}
	}
	{
		knh_ParamArray_t *bpa = ClassTable(bcid).cparam;
		knh_param_t *bp = knh_ParamArray_get(bpa, 0);
		knh_ParamArray_t *pa = new_ParamArray(ctx);
		knh_param_t p = {p1, bp->fn};
		knh_ParamArray_add(ctx, pa, p);
		return knh_addGenericsClass(ctx, CLASS_newid, bcid, pa);
	}
}

/* ------------------------------------------------------------------------ */

knh_bool_t knh_ParamArray_hasTypeVar(knh_ParamArray_t *pa)
{
	size_t i, size = pa->psize + pa->rsize;
	for(i = 0; i < size; i++) {
		knh_param_t *p = knh_ParamArray_get(pa, i);
		if(!(p->type < TYPE_T0)) return 1;
	}
	return 0;
}

/* ------------------------------------------------------------------------ */

void knh_ParamArray_tocid(Ctx *ctx, knh_ParamArray_t *pa, knh_class_t this_cid, knh_ParamArray_t *npa)
{
	size_t i, size = pa->psize + pa->rsize;
	for(i = 0; i < size; i++) {
		knh_param_t *cp = knh_ParamArray_get(pa, i);
		knh_param_t np = *cp;
		np.type = knh_type_tocid(ctx, np.type, this_cid);
		knh_ParamArray_add(ctx, npa, np);
	}
	npa->psize = pa->psize;
	npa->rsize = pa->rsize;
}

/* ------------------------------------------------------------------------ */

knh_class_t knh_type_tocid(Ctx *ctx, knh_type_t ptype, knh_class_t this_cid)
{
	long t = ptype - TYPE_T1;
	if(t == -1) {
		DBG_ASSERT_cid(this_cid);
		return this_cid;
	}
	else if(0 <= t) {
		knh_ParamArray_t *tpa = ClassTable(this_cid).cparam;
		if(tpa != NULL && t < (tpa)->psize + (tpa)->rsize) {
			knh_param_t *p = knh_ParamArray_get(tpa, t);
			return CLASS_type(p->type);
		}
		return CLASS_Any;
	}
	else {
		knh_class_t pcid = CLASS_type(ptype);
		if(knh_class_isGenerics(ctx, pcid)) { /* Iterator<This> */
			BEGIN_LOCAL(ctx, lsfp, 1);
			knh_ParamArray_t *npa = new_ParamArray(ctx);
			KNH_SETv(ctx, lsfp[0].o, npa);
			knh_ParamArray_tocid(ctx, ClassTable(pcid).cparam, this_cid, npa);
			pcid = knh_class_Generics(ctx, knh_class_bcid(pcid), npa);
			END_LOCAL(ctx, lsfp);
		}
		return pcid;
	}
}

/* ------------------------------------------------------------------------ */

char *TYPEQ(knh_type_t type)
{
	return "";
}

/* ------------------------------------------------------------------------ */

char *knh_TYPEN(Ctx *ctx, knh_type_t type)
{
	if(type == TYPE_void) return "void";
	if(type == TYPE_var)  return "var";
	knh_class_t cid = CLASS_type(type);
	return CLASSN(cid);
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
