/****************************************************************************
 * KONOHA COPYRIGHT, LICENSE NOTICE, AND DISCRIMER
 *
 * Copyright (c) 2006-2010, Kimio Kuramitsu <kimio at ynu.ac.jp>
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
/* [ClassTable] */

knh_Class_t *new_Type(Ctx *ctx, knh_type_t type)
{
	knh_class_t cid = CLASS_type(type);
	DBG2_ASSERT_cid(cid);
	knh_ClassTable_t *t = pClassTable(ctx, cid);
	if(IS_NATYPE(type)) {
		if(t->class_natype == NULL) {
			knh_Class_t *o = (knh_Class_t*)new_hObject(ctx, FLAG_Class, CLASS_Class, CLASS_Class);
			o->cid = cid;
			o->type = type;
			KNH_INITv(t->class_natype, o);
		}
		return t->class_natype;
	}
	else {
		if(t->class_cid == NULL) {
			knh_Class_t *o = (knh_Class_t*)new_hObject(ctx, FLAG_Class, CLASS_Class, CLASS_Class);
			o->cid = cid;
			o->type = type;
			KNH_INITv(t->class_cid, o);
		}
		return t->class_cid;
	}
}

/* ------------------------------------------------------------------------ */

void knh_setClassName(Ctx *ctx, knh_class_t cid, knh_String_t *lname)
{
	DBG2_ASSERT_cid(cid);
	knh_ClassTable_t *t = pClassTable(ctx, cid);
	DBG2_ASSERT(t->sname == NULL);
	KNH_INITv(t->lname, lname);
	{
		knh_bytes_t n = __tobytes(lname);
		knh_index_t idx = knh_bytes_index(n, '{');
		if(idx != -1) {
			KNH_INITv(t->sname, lname);
			goto L_ADDDICT;
		}
		if(knh_bytes_endsWith(n, STEXT(".."))) {
			n.len -= 2;
			idx = knh_bytes_rindex(n, '.');
			n.len += 2;
		}
		else {
			idx = knh_bytes_rindex(n, '.');
		}
		if(idx == -1) {
			KNH_INITv(t->sname, lname);
		}
		else {
			KNH_INITv(t->sname, new_String(ctx, knh_bytes_last(n, idx + 1), lname));
		}
	}

	L_ADDDICT:
	//DBG2_P("NEW CLASS: %s, %s", __tochar(lname), __tochar(t->sname));
	KNH_LOCK(ctx, LOCK_SYSTBL, NULL);
	{
		knh_DictSet_append(ctx, DP(ctx->sys)->ClassNameDictSet, lname, cid+1);
		if(knh_String_startsWith(lname, STEXT("konoha."))) {
			knh_DictSet_append(ctx, DP(ctx->sys)->ClassNameDictSet, ClassTable(cid).sname, cid+1);
		}
	}
	KNH_UNLOCK(ctx, LOCK_SYSTBL, NULL);
}

/* ------------------------------------------------------------------------ */
/* [name] */

knh_class_t knh_getcid(Ctx *ctx, knh_bytes_t lname)
{
	knh_uintptr_t cid = knh_DictSet_get__b(DP(ctx->sys)->ClassNameDictSet, lname);
	if(cid > 0) return (knh_class_t)(cid-1);
	if(lname.buf[lname.len-1] == '}') {
		return knh_findcidx(ctx, lname);
	}
	return CLASS_unknown;
}

/* ------------------------------------------------------------------------ */

char* knh_Context_CLASSN(Ctx *ctx, knh_class_t cid)
{
	DBG2_ASSERT_cid(cid);
	knh_String_t *name = ClassTable(cid).lname;
	if(knh_String_startsWith(name, STEXT("konoha."))) {
		return __tochar(ClassTable(cid).sname);
	}
	else {
		knh_uintptr_t cid2 =
			knh_DictSet_get__b(DP(ctx->share->mainns)->name2cidDictSet, __tobytes(ClassTable(cid).sname));
		if(cid2 > 0 && cid == cid2 - 1) {
			return __tochar(ClassTable(cid).sname);
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
		DBG2_ASSERT_cid(cid);
		DBG2_ASSERT(ClassTable(cid).sname != NULL);
		if(knh_String_startsWith(ClassTable(cid).lname, STEXT("konoha."))) {
			return __tochar(ClassTable(cid).sname);
		}
		else {
			return __tochar(ClassTable(cid).lname);
		}
	}
}

/* ------------------------------------------------------------------------ */
/* [default] */

knh_Object_t *knh_fdefault__CONST(Ctx *ctx, knh_class_t cid)
{
	return ClassTable(cid).defval;
}

/* ------------------------------------------------------------------------ */

knh_Object_t *knh_fdefault__NEWVALUE(Ctx *ctx, knh_class_t cid)
{
	knh_ClassTable_t *t = pClassTable(ctx, cid);
	Object *v = new_Object_init(ctx, ClassTable(cid).oflag | FLAG_Object_Immutable | FLAG_Object_Undefined, cid, 0);
	DBG2_P("create new default value of %s", CLASSN(cid));
	KNH_ASSERT(t->cspec == NULL);
	KNH_INITv(t->cspec, v);
	t->fdefault = knh_fdefault__CONST;
	return t->defval;
}

/* ------------------------------------------------------------------------ */

void knh_setClassDefaultValue_(Ctx *ctx, knh_class_t cid, Object *value, knh_Fdefault fdefault)
{
	DBG2_ASSERT_cid(cid);
	knh_ClassTable_t *t = pClassTable(ctx, cid);
	if(value == NULL) {
		if(fdefault == NULL) fdefault = knh_fdefault__NEWVALUE;
	}
	else {
		KNH_ASSERT(t->cspec == NULL);
		KNH_INITv(t->cspec, value);
		if(fdefault == NULL) fdefault = knh_fdefault__CONST;
	}
	t->fdefault = fdefault;
}

/* ------------------------------------------------------------------------ */

Object *knh_getClassDefaultValue(Ctx *ctx, knh_class_t cid)
{
	DBG2_ASSERT_cid(cid);
	return ClassTable(cid).fdefault(ctx, cid);
}

/* ------------------------------------------------------------------------ */
/* [PARAM] */

void knh_setClassParam(Ctx *ctx, knh_class_t cid, knh_class_t p1, knh_class_t p2)
{
	DBG2_ASSERT_cid(cid);
	knh_ClassTable_t *t = pClassTable(ctx, cid);
	t->p1 = p1;
	t->p2 = p2;
	if(!knh_class_isCyclic(cid)) {
		if(knh_class_isCyclic(p1) || (p2 != CLASS_Tvoid && knh_class_isCyclic(p2))) {
			knh_class_setCyclic(cid, 1);
		}
	}
}

/* ------------------------------------------------------------------------ */

KNHAPI(void) knh_loadClassData(Ctx *ctx0, knh_ClassData_t *data)
{
	KNH_ASSERT_CTX0(ctx0);
	knh_NameSpace_t *ns = DP(ctx0->kc)->ns;
	while(data->cspi != NULL) {
		knh_class_t cid = new_ClassId(ctx0);
		knh_class_t supcid = (data->supname == NULL) ? CLASS_Object : knh_NameSpace_findcid(ctx0, ns, B(data->supname));
		knh_class_t p1 = (data->p1 == NULL) ? TYPE_void : knh_NameSpace_findcid(ctx0, ns, B(data->p1));
		knh_class_t p2 = (data->p2 == NULL) ? TYPE_void : knh_NameSpace_findcid(ctx0, ns, B(data->p2));
		knh_ClassTable_t *t = pClassTable(ctx0, cid);
		t->cflag  = data->cspi->cflag;
		t->oflag  = t->oflag;
		t->cspi   = data->cspi;
		t->bcid   = cid;
		t->supcid = supcid;
		t->offset = 0;
		t->size   = data->cspi->size;
		t->bsize  = t->size / sizeof(Object*);
		{
			knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx0, &cwbbuf);
			knh_NameSpace_t *ns = DP(ctx0->kc)->ns;
			knh_String_t *name;
			knh_Bytes_write(ctx0, cwb->ba, __tobytes(DP(ns)->nsname));
			knh_Bytes_putc(ctx0, cwb->ba, '.');
			knh_Bytes_write(ctx0, cwb->ba, B(data->cname));
			name = knh_cwb_newString(ctx0, cwb);
			knh_setClassName(ctx0, cid, name);
		}
		KNH_INITv(t->methods, new_Array0(ctx0, 0));
		KNH_ASSERT(t->fields == NULL);  /* naitive */
		KNH_INITv(t->cmap, knh_ClassMap_fdefault(ctx0, CLASS_ClassMap));
		knh_setClassParam(ctx0, cid, p1, p2);
		data++;
	}
}

/* ------------------------------------------------------------------------ */

knh_class_t
knh_addGenericsClass(Ctx *ctx, knh_class_t cid, knh_String_t *name, knh_class_t bcid, knh_class_t p1, knh_class_t p2)
{
	knh_ClassTable_t *t, *bt;
	knh_flag_t mask = 0;
	if(cid == CLASS_newid) {
		cid = new_ClassId(ctx);
	}
	DBG2_ASSERT(bcid < cid);
	t = pClassTable(ctx, cid);
	bt = pClassTable(ctx, bcid);

	if(knh_class_isTypeVariable(CLASS_type(p1)) ||
			(p2 != CLASS_Tvoid && knh_class_isTypeVariable(CLASS_type(p2)))) {
		mask = FLAG_Class_TypeVariable;
	}

	t->cflag  = bt->cflag | mask;
	t->oflag  = bt->oflag;
	t->cspi    = bt->cspi;
	t->bcid   = bcid;
	t->supcid = bt->supcid;
	t->offset = bt->offset;
	t->size = bt->size;
	t->bsize  = bt->bsize;

	knh_setClassName(ctx, cid, name);
	KNH_INITv(t->methods, bt->methods);
	KNH_ASSERT(bt->fields == NULL);
	KNH_INITv(t->cmap, knh_ClassMap_fdefault(ctx, CLASS_ClassMap));
	knh_setClassParam(ctx, cid, p1, p2);
	return cid;
}

/* ======================================================================== */
/* [field] */

knh_index_t knh_Class_indexOfField(Ctx *ctx, knh_class_t cid, knh_fieldn_t fn)
{
	DBG2_ASSERT_cid(cid);
	while(1) {
		knh_ClassTable_t *t = pClassTable(ctx, cid);
		if(t->fields != NULL) {
			knh_index_t idx;
			for(idx = 0; idx < t->fsize; idx++) {
				if(t->fields[idx].fn == fn) {
					return t->offset + idx;
				}
			}
		}
		if(t->offset == 0) return -1;
		cid = t->supcid;
	}
}

/* ------------------------------------------------------------------------ */

knh_index_t knh_Class_queryField(Ctx *ctx, knh_class_t cid, knh_fieldn_t fnq)
{
	knh_fieldn_t fn = FIELDN_UNMASK(fnq);
	DBG2_ASSERT_cid(cid);
	while(1) {
		knh_ClassTable_t *t = pClassTable(ctx, cid);
		if(FIELDN_IS_SUPER(fnq)) {
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
	DBG2_ASSERT_cid(cid);
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

/* ======================================================================== */
/* [ClassMap] */

knh_ClassMap_t* new_ClassMap0(Ctx *ctx, knh_ushort_t capacity)
{
	if(capacity < KNH_FASTMALLOC_BSIZE) capacity = KNH_FASTMALLOC_BSIZE;
	knh_ClassMap_t *o = (knh_ClassMap_t*)new_Object_bcid(ctx, CLASS_ClassMap, capacity);
	knh_ClassMap_setSorted(o, 1);
	return o;
}

/* ------------------------------------------------------------------------ */

Object *knh_ClassMap_fdefault(Ctx *ctx, knh_class_t cid)
{
	if(unlikely(ClassTable(CLASS_Any).cmap == NULL)) {
		knh_ClassTable_t *t = pClassTable(ctx, CLASS_Any);
		KNH_INITv(t->cmap, new_ClassMap0(ctx, 0));
	}
	return (Object*)ClassTable(CLASS_Any).cmap;
}

/* ------------------------------------------------------------------------ */

static
int knh_ClassMap_isDefault(Ctx *ctx, knh_ClassMap_t *o)
{
	return (o == (knh_ClassMap_t*)knh_ClassMap_fdefault(ctx, CLASS_Any));
}

/* ------------------------------------------------------------------------ */

void knh_readyClassMap(Ctx *ctx, knh_class_t cid)
{
	DBG2_ASSERT_cid(cid);
	KNH_ASSERT(cid != CLASS_Any);
	if(knh_ClassMap_isDefault(ctx, ClassTable(cid).cmap)) {
		knh_ClassTable_t *t = pClassTable(ctx, cid);
		KNH_SETv(ctx, t->cmap, new_ClassMap0(ctx, 4));
	}
}

/* ------------------------------------------------------------------------ */

void knh_ClassMap_resize(Ctx *ctx, knh_ClassMap_t *cmap, size_t newsize)
{
	knh_intptr_t i;
	knh_Mapper_t **newlist =
		(knh_Mapper_t**)KNH_MALLOC(ctx, newsize * sizeof(knh_Mapper_t*));
	knh_bzero(newlist, newsize * sizeof(knh_Mapper_t*));
	for(i = 0; i < (cmap)->size; i++) {
		newlist[i] = (cmap)->maplist[i];
	}
	KNH_FREE(ctx, (cmap)->maplist, (cmap)->capacity * sizeof(knh_Mapper_t*));
	(cmap)->maplist = newlist;
	(cmap)->capacity = newsize;
}

/* ------------------------------------------------------------------------ */

int
knh_ClassMap_util_cmp(const knh_Mapper_t *m1, const knh_Mapper_t *m2)
{
	int res = DP(m1)->flag - DP(m2)->flag;
	return (res == 0) ? DP(m2)->tcid - DP(m1)->tcid : res;
}

/* ------------------------------------------------------------------------ */

void knh_ClassMap_sort(Ctx *ctx, knh_ClassMap_t *cmap)
{
	if(!knh_ClassMap_isSorted(cmap)) {
		knh_qsort((cmap)->maplist, (cmap)->size, sizeof(knh_Mapper_t*),
					(int (*)(const void*, const void*))knh_ClassMap_util_cmp);
		knh_ClassMap_setSorted(cmap, 1);
	}
}

/* ------------------------------------------------------------------------ */

void knh_ClassMap_add(Ctx *ctx, knh_ClassMap_t *cmap, knh_Mapper_t *mpr)
{
	DBG2_ASSERT(!knh_ClassMap_isDefault(ctx, cmap));
	if((cmap)->size == (cmap)->capacity) {
		knh_ClassMap_resize(ctx, cmap, (cmap)->capacity + 8);
	}
	DBG2_ASSERT((cmap)->size < (cmap)->capacity);
	DBG2_ASSERT((cmap)->maplist[(cmap)->size] == NULL);
	KNH_INITv((cmap)->maplist[(cmap)->size], mpr);
	(cmap)->size++;
}

/* ======================================================================== */
/* [DataPool] */
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
		int i;
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
		DBG2_P("Empty domain cid=%s", CLASSN(cid));
		a = new_Array0(ctx, 0);
	}
	return a;
}

/* ------------------------------------------------------------------------ */


#ifdef __cplusplus
}
#endif
