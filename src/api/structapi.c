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
#include"../../include/konoha/konoha_stmt_.h"
#include"../../include/konoha/konoha_code_.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ======================================================================== */
/* Data */
/* ======================================================================== */

static MAPPER Array_Iterator(Ctx *ctx, knh_sfp_t *sfp METHODOPT);
static MAPPER Iterator_Array(Ctx *ctx, knh_sfp_t *sfp METHODOPT);
static knh_String_t* knh_fgetkey__default(Ctx *ctx, knh_sfp_t *lsfp);

/* ------------------------------------------------------------------------ */
/* @data*/
typedef struct {
	char                  *name;
	int                    index;
} knh_StringData_t ;

typedef struct {
	char                  *name;
	knh_struct_t           sid;
	knh_flag_t             flag;
	size_t                 size;
	knh_fstruct_init       finit;
	knh_fstruct_copy       fcopy;
	knh_fstruct_traverse   ftraverse;
	knh_fstruct_compareTo  fcompareTo;
	knh_fstruct_hashCode   fhashCode;
	knh_fstruct_newClass   fnewClass;
	knh_fstruct_getkey     fgetkey;
} knh_StructData_t ;

typedef struct {
	char *name;
	knh_class_t cid;
	knh_flag_t flag;
	knh_struct_t bcid;
	knh_class_t supcid;
	int method_size;
	int formatter_size;
	int mapper_size; /* copy Any if 0 */
	knh_type_t r0;
	knh_type_t p1;
	knh_type_t p2;
	knh_type_t p3;
} knh_ClassData_t ;

typedef struct {
	char *name;
	knh_flag_t flag;
	knh_expt_t eid;
	knh_expt_t peid;
} knh_ExptData_t ;

typedef struct {
	char *name;
	knh_fieldn_t fn;
} knh_FieldNameData_t ;

typedef struct {
	int mfindex;
	knh_ushort_t psize;
	knh_type_t rtype;
	knh_type_t t1;
	knh_fieldn_t f1;
	knh_type_t t2;
	knh_fieldn_t f2;
	knh_type_t t3;
	knh_fieldn_t f3;
	knh_type_t t4;
	knh_fieldn_t f4;
} knh_MethodFieldData_t ;

typedef struct {
	knh_fmethod func;
	knh_flag_t flag;
	knh_class_t cid;
	knh_methodn_t mn;
	knh_ushort_t delta;
	int mfindex;
	void *optfunc;
} knh_MethodData_t ;

typedef struct {
	knh_fmapper func;
	knh_flag_t flag;
	knh_class_t scid;
	knh_class_t tcid;
} knh_MapperData_t ;

/* ======================================================================== */
/* Object */

#define knh_ObjectField_init_ NULL
#define knh_ObjectField_copy NULL
#define knh_ObjectField_traverse_ NULL
#define knh_ObjectField_compareTo NULL
#define knh_ObjectField_hashCode NULL
#define knh_ObjectField_newClass NULL
#define knh_ObjectField_getkey_  NULL

/* ------------------------------------------------------------------------ */

static
void knh_ClassField_initField(Ctx *ctx, knh_ClassField_t *cs, knh_class_t self_cid, Object **v)
{
	size_t i;
	knh_cfield_t *cf = cs->fields;
	for(i = 0; i < cs->fsize; i++) {
		knh_type_t type = cf[i].type;
		Object *value = cf[i].value;
		if(KNH_FLAG_IS(cf[i].flag, FLAG_ClassField_Property)) {
			value = knh_getProperty(ctx, __tobytes((knh_String_t*)value));
			DBG2_P("type=%s%s object=%s", TYPEQN(cf[i].type), CLASSNo(value));
		}
#ifdef KNH_USING_UNBOXFIELD
		if(type == TYPE_void) {
			continue;
		}
		else if(IS_ubxint(type)) {
			knh_int_t *data = (knh_int_t*)(v + i);
			data[0] = value == NULL ? 0 : toint(value);
			continue;
		}
		else if(IS_ubxfloat(type)) {
			knh_float_t *data = (knh_float_t*)(v + i);
			data[0] = value == NULL ? KNH_FLOAT_ZERO : tofloat(value);
			continue;
		}
		else if(IS_ubxboolean(type)) {
			knh_bool_t *data = (knh_bool_t*)(v + i);
			data[0] = value == NULL ? 0 : tobool(value);
			continue;
		}
		else
#endif/*KNH_USING_UNBOXFIELD*/
		if(value == NULL) {
			knh_class_t cid = knh_pmztype_toclass(ctx, type, self_cid);
			DBG2_ASSERT(IS_NNTYPE(type));
			KNH_INITv(v[i], KNH_DEF(ctx, cid));
		}
		else {
			KNH_INITv(v[i], value);
		}
	}
}

/* ------------------------------------------------------------------------ */

void knh_ObjectField_init(Ctx *ctx, knh_ObjectField_t *of, int init)
{
	knh_class_t cid = of->h.cid;
	if(ClassTable(cid).size > 0) {
		Object **v = (Object**)of->fields;
		size_t offset;
		while((offset = ClassTable(cid).offset) != 0) {
			knh_ClassField_initField(ctx, ClassTable(cid).cstruct, of->h.cid, v + offset);
			cid = ClassTable(cid).supcid;
			DBG2_ASSERT_cid(cid);
		}
		knh_ClassField_initField(ctx, ClassTable(cid).cstruct, of->h.cid, v + offset);
		of->fields = v;
		of->bsize = ctx->share->ClassTable[of->h.cid].bsize;
	}
	else {
		of->fields = NULL;
		of->bsize = 0;
	}
}

/* ------------------------------------------------------------------------ */

void knh_ObjectField_traverse(Ctx *ctx, knh_ObjectField_t *of, knh_ftraverse ftr)
{
	knh_class_t cid = knh_Object_cid(of);
	while(cid != CLASS_Object) {
		knh_ClassField_t *cs = ClassTable(cid).cstruct;
		size_t i, offset = ClassTable(cid).offset;
		for(i = 0; i < cs->fsize; i++) {
			knh_type_t type = cs->fields[i].type;
			//DBG2_P("i=%d, fn=%s, type=%s%s", i, FIELDN(cs->fields[i].fn), TYPEQN(type));
			if(IS_ubxtype(type) || type == TYPE_void) {
				continue;
			}
			if(of->fields[i + offset] == NULL) return; /* for Script */
			if(cs->fields[i].fn == FIELDN_/*register*/) continue;
			ftr(ctx, of->fields[i + offset]);
		}
		cid = ClassTable(cid).supcid;
	}
}

/* ------------------------------------------------------------------------ */

static knh_String_t* knh_ObjectField_getkey(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	int keyidx = ClassTable(knh_Object_cid(sfp[0].o)).keyidx;
	DBG2_P("getkey keyidx=%d of %s", keyidx, CLASSN(knh_Object_cid(sfp[0].o)));
	if(keyidx != -1) {
		knh_ObjectField_t *of = (knh_ObjectField_t*)sfp[0].o;
		DBG2_ASSERT(IS_bString(of->fields[keyidx]));
		return (knh_String_t*)of->fields[keyidx];
	}
	return knh_fgetkey__default(ctx, sfp);
}

/* ======================================================================== */
/* Any */

#define knh_Any_init_ NULL
#define knh_Any_copy NULL
#define knh_Any_traverse_ NULL
#define knh_Any_compareTo NULL
#define knh_Any_hashCode NULL
#define knh_Any_newClass NULL
#define knh_Any_getkey NULL

static void knh_Any_init(Ctx *ctx, knh_Glue_t *g, int init)
{
	knh_Glue_init(ctx, g, NULL, NULL);
}

static void knh_Any_traverse(Ctx *ctx, knh_Glue_t *g, knh_ftraverse ftr)
{
	if(IS_SWEEP(ftr)) {
		if(IS_NULL(g)) {
			DBG_P("freeing null");
		}
		else {
			g->gfree(ctx, g);
			knh_Glue_init(ctx, g, NULL, NULL);
		}
	}
}

/* ======================================================================== */
/* Boolean */

#define knh_Boolean_init_ NULL
#define knh_Boolean_copy NULL
#define knh_Boolean_traverse NULL
#define knh_Boolean_compareTo NULL
#define knh_Boolean_hashCode NULL
#define knh_Boolean_newClass_ NULL

static void knh_Boolean_init(Ctx *ctx, knh_Boolean_t *o, int init)
{
	o->n.bvalue = init;
}

static void knh_Boolean_newClass(Ctx *ctx, knh_class_t cid)
{
	knh_setClassDefaultValue(ctx, cid, UP(KNH_FALSE), NULL);
}

static knh_String_t *knh_Boolean_getkey(Ctx *ctx, knh_sfp_t *lsfp)
{
	if(lsfp[0].bvalue) {
		return TS_true;
	}
	else {
		return TS_false;
	}
}

/* ======================================================================== */
/* Number */

#define knh_Number_init NULL
#define knh_Number_copy NULL
#define knh_Number_traverse NULL
#define knh_Number_compareTo NULL
#define knh_Number_hashCode NULL
#define knh_Number_newClass NULL
#define knh_Number_getkey NULL

/* ======================================================================== */
/* Int */

#define knh_Int_init_ NULL
#define knh_Int_copy NULL
#define knh_Int_traverse NULL
#define knh_Int_compareTo_ NULL
#define knh_Int_hashCode_ NULL
#define knh_Int_newClass NULL

static void knh_Int_init(Ctx *ctx, knh_Int_t *n, int init)
{
	n->n.ivalue = (knh_int_t)init;
}

static knh_hashcode_t knh_Int_hashCode(Ctx *ctx, knh_Int_t *o)
{
	return (knh_hashcode_t)o->n.ivalue;
}

static int knh_Int_compareTo(Ctx *ctx, knh_Int_t *o, knh_Int_t *o2)
{
	if(o->h.cid == CLASS_Int || o2->h.cid == CLASS_Int) {
		return (int)(o->n.ivalue - o2->n.ivalue);
	}
	else {
		knh_ClassSpec_t *u = knh_getClassSpec(ctx, o->h.cid);
		return DP(u)->ficmp(u, o->n.ivalue, o2->n.ivalue);
	}
}

static knh_String_t *knh_Int_getkey(Ctx *ctx, knh_sfp_t *lsfp)
{
	char buf[40];
#ifdef KNH_USING_INT32
	knh_snprintf(buf, sizeof(buf), "%016lu", lsfp[0].uvalue);
#else
	//18446744073709551615ULL
	knh_snprintf(buf, sizeof(buf), "%020llu", lsfp[0].uvalue);
#endif
	return new_String(ctx, B(buf), NULL);
}

/* ======================================================================== */
/* Float */

#define knh_Float_init_ NULL
#define knh_Float_copy NULL
#define knh_Float_traverse NULL
#define knh_Float_compareTo_ NULL
#define knh_Float_hashCode_ NULL
#define knh_Float_newClass NULL
#define knh_Float_getkey NULL

static void knh_Float_init(Ctx *ctx, knh_Float_t *f, int init)
{
	f->n.fvalue = (knh_float_t)init;
}

static knh_hashcode_t knh_Float_hashCode(Ctx *ctx, knh_Int_t *f)
{
	return (knh_hashcode_t)f->n.ivalue;
}

static int knh_Float_compareTo(Ctx *ctx, knh_Float_t *o, knh_Float_t *o2)
{
	if(o->h.cid == CLASS_Float || o2->h.cid == CLASS_Float) {
		return (int)(o->n.fvalue - o2->n.fvalue);
	}
	else {
		knh_ClassSpec_t *fu = knh_getClassSpec(ctx, o->h.cid);
		return DP(fu)->ffcmp(fu, o->n.fvalue, o2->n.fvalue);
	}
}

//static
//String *knh_Float_getkey(Ctx *ctx, knh_sfp_t *lsfp)
//{
//	char buf[40]; // formating as int64
//	knh_snprintf(buf, sizeof(buf), "%020llu", lsfp[0].ivalue);
//	return new_String(ctx, B(buf), NULL);
//}

/* ======================================================================== */
/* String */

#define knh_String_init_ NULL
#define knh_String_copy NULL
#define knh_String_traverse_ NULL
#define knh_String_compareTo_ NULL
#define knh_String_hashCode_ NULL
#define knh_String_newClass NULL

static void knh_String_init(Ctx *ctx, knh_String_t *s, int init)
{
	s->str = (knh_uchar_t*)""; // for safety
	s->size = 0;
	s->orign = NULL;
}

static void knh_String_traverse(Ctx *ctx, knh_String_t *s, knh_ftraverse ftr)
{
	if(s->orign != NULL) {
		ftr(ctx, UP(s->orign));
	}
	else if(!knh_String_isTextSgm(s)) {
		if(IS_SWEEP(ftr)) {
			KNH_FREE(ctx, s->str, KNH_SIZE(s->size + 1));
			s->str = NULL;
		}
	}
}

static knh_hashcode_t knh_String_hashCode(Ctx *ctx, knh_String_t *o)
{
	KNH_ASSERT(IS_bString(o));
	knh_hashcode_t h = o->size;
	size_t i;
	for(i = 0; i < o->size; i++) {
		h = o->str[i] + 31 * h;
	}
	return h;
}

static int knh_String_compareTo(Ctx *ctx, knh_String_t *o, knh_String_t *o2)
{
	if(o->h.cid == CLASS_String || o2->h.cid == CLASS_String) {
		size_t i, max = (o->size< o2->size) ? o->size : o2->size;
		for(i = 0; i < max; i++) {
			int res = o->str[i] - o2->str[i];
			if(res != 0) return res;
		}
		return o->size - o2->size;
	}
	else {
		if(o->h.cid == o2->h.cid) {
			knh_ClassSpec_t *u = knh_getClassSpec(ctx, o->h.cid);
			return DP(u)->fscmp(u, __tobytes(o), __tobytes(o2));
		}
		return (int)(o - o2);
	}
}

static knh_String_t *knh_String_getkey(Ctx *ctx, knh_sfp_t *lsfp)
{
	return lsfp[0].s;
}

/* ======================================================================== */
/* Bytes */

#define knh_Bytes_init_ NULL
#define knh_Bytes_copy NULL
#define knh_Bytes_traverse_ NULL
#define knh_Bytes_compareTo NULL
#define knh_Bytes_hashCode NULL
#define knh_Bytes_newClass NULL
#define knh_Bytes_getkey NULL

/* ------------------------------------------------------------------------ */

static void knh_Bytes_init(Ctx *ctx, knh_Bytes_t *b, int init)
{
	b->size = 0;
	if(init <= 0) {
		b->buf = (knh_uchar_t*)"";
		b->capacity = 0;
	}
	else {
		b->capacity = knh_bytes_newsize(init);
		b->buf = (knh_uchar_t*)KNH_MALLOC(ctx, b->capacity);
	}
}

static void knh_Bytes_traverse(Ctx *ctx, knh_Bytes_t *b, knh_ftraverse ftr)
{
	if(b->capacity > 0 && IS_SWEEP(ftr)) {
		KNH_FREE(ctx, b->buf, b->capacity);
		b->buf = NULL;
		b->capacity = 0;
		b->size = 0;
	}
}

/* ======================================================================== */
/* Pair */

#define knh_Pair_init_ NULL
#define knh_Pair_copy NULL
#define knh_Pair_traverse_ NULL
#define knh_Pair_compareTo NULL
#define knh_Pair_hashCode NULL
#define knh_Pair_newClass NULL
#define knh_Pair_getkey NULL

static void knh_Pair_init(Ctx *ctx, knh_Pair_t *t, int init)
{
	KNH_INITv(t->first, KNH_NULL);
	KNH_INITv(t->second, KNH_NULL);
}

static void knh_Pair_traverse(Ctx *ctx, knh_Pair_t *t, knh_ftraverse ftr)
{
	ftr(ctx, t->first);
	ftr(ctx, t->second);
}

/* ======================================================================== */
/* Tuple */

#define knh_Tuple_init_ NULL
#define knh_Tuple_copy NULL
#define knh_Tuple_traverse_ NULL
#define knh_Tuple_compareTo NULL
#define knh_Tuple_hashCode NULL
#define knh_Tuple_newClass NULL
#define knh_Tuple_getkey NULL

static
void knh_Tuple_init(Ctx *ctx, knh_Tuple_t *t, int init)
{
	DBG2_ASSERT(init == 0);
	t->size = 0; /* @see Tuple_new */
	t->list = NULL;
}

static
void knh_Tuple_traverse(Ctx *ctx, knh_Tuple_t *t, knh_ftraverse ftr)
{
	if(knh_Tuple_isTriple(t)) {
		ftr(ctx, t->first);
		ftr(ctx, t->second);
		ftr(ctx, t->third);
	}
	else if(t->size > 0){
		size_t i;
		for(i = 0; i < t->size; i++) {
			ftr(ctx, t->list[i]);
		}
		if(IS_SWEEP(ftr)) {
			KNH_FREE(ctx, t->list, sizeof(void*) * t->size);
			t->size = 0;
		}
	}
}

/* ======================================================================== */
/* Range */

#define knh_Range_init knh_Pair_init
#define knh_Range_copy NULL
#define knh_Range_traverse knh_Pair_traverse
#define knh_Range_compareTo NULL
#define knh_Range_hashCode NULL
#define knh_Range_getkey NULL

static MAPPER Range_Iterator(Ctx *ctx, knh_sfp_t *sfp METHODOPT);

#define FLAG_Mapper_Iteration (FLAG_Mapper_Synonym | FLAG_Mapper_Total)

static void knh_Range_newClass(Ctx *ctx, knh_class_t cid)
{
	DBG2_ASSERT_cid(cid);
	knh_class_t p1 = ClassTable(cid).p1;
	knh_class_t icid = knh_class_Iterator(ctx, p1);
	DBG2_P("*** %s, %s", CLASSN(p1), CLASSN(icid));
	knh_addMapperFunc(ctx, FLAG_Mapper_Iteration, cid, icid, Range_Iterator, KNH_NULL);
}

/* ======================================================================== */
/* Array */

#define knh_Array_init_ NULL
#define knh_Array_copy NULL
#define knh_Array_traverse_ NULL
#define knh_Array_compareTo NULL
#define knh_Array_hashCode NULL
#define knh_Array_getkey NULL

static void knh_Array_init(Ctx *ctx, knh_Array_t *a, int init)
{
	a->list = NULL;
	a->capacity = 0;
	a->size = 0;
	if(init > 0) {
		knh_Array_grow(ctx, a, init, KNH_NULL);
	}
}

static void knh_Array_traverse(Ctx *ctx, knh_Array_t *a, knh_ftraverse ftr)
{
	size_t i;
	for(i = 0; i < a->capacity; i++) {
		ftr(ctx, a->list[i]);
	}
	if(a->capacity > 0 && IS_SWEEP(ftr)) {
		if(knh_Array_hasDimension(a)) {
			knh_array_dfree(ctx, a->list, a->capacity, sizeof(Object*));
		}
		else {
			KNH_FREE(ctx, a->list, a->capacity * sizeof(Object*));
		}
		a->list = NULL;
		a->size = 0;
		a->capacity = 0;
	}
}

static void knh_Array_newClass(Ctx *ctx, knh_class_t cid)
{
	DBG2_ASSERT_cid(cid);
	knh_class_t p1 = ClassTable(cid).p1;
	knh_class_t icid = knh_class_Iterator(ctx, p1);
	DBG2_P("*** %s, %s", CLASSN(p1), CLASSN(icid));
	knh_addMapperFunc(ctx, FLAG_Mapper_Iteration, cid, icid, Array_Iterator, KNH_NULL);
	knh_addMapperFunc(ctx, FLAG_Mapper_Iteration, icid, cid, Iterator_Array, KNH_NULL);
}

/* ======================================================================== */
/* IArray */

#define knh_IArray_init_ NULL
#define knh_IArray_copy NULL
#define knh_IArray_traverse_ NULL
#define knh_IArray_compareTo NULL
#define knh_IArray_hashCode NULL
#define knh_IArray_newClass NULL
#define knh_IArray_getkey NULL

static void knh_IArray_init(Ctx *ctx, knh_IArray_t *a, int init)
{
	a->ilist = NULL;
	a->capacity = 0;
	a->size = 0;
	KNH_ASSERT(init == 0);
}

static void knh_IArray_traverse(Ctx *ctx, knh_IArray_t *a, knh_ftraverse ftr)
{
	if(a->capacity > 0 && IS_SWEEP(ftr)) {
		if(knh_IArray_hasDimension(a)) {
			knh_array_dfree(ctx, a->ilist, a->capacity, sizeof(knh_int_t));
		}
		else {
			KNH_FREE(ctx, a->ilist, a->capacity * sizeof(knh_int_t));
		}
		a->size = 0;
		a->capacity = 0;
	}
}

/* ======================================================================== */
/* FArray */

#define knh_FArray_init_ NULL
#define knh_FArray_copy NULL
#define knh_FArray_traverse_ NULL
#define knh_FArray_compareTo NULL
#define knh_FArray_hashCode NULL
#define knh_FArray_newClass NULL
#define knh_FArray_getkey NULL

static void knh_FArray_init(Ctx *ctx, knh_FArray_t *a, int init)
{
	a->flist = NULL;
	a->capacity = 0;
	a->size = 0;
	KNH_ASSERT(init == 0);
}

static void knh_FArray_traverse(Ctx *ctx, knh_FArray_t *a, knh_ftraverse ftr)
{
	if(a->capacity > 0 && IS_SWEEP(ftr)) {
		if(knh_FArray_hasDimension(a)) {
			knh_array_dfree(ctx, a->flist, a->capacity, sizeof(knh_float_t));
		}
		else {
			KNH_FREE(ctx, a->flist, a->capacity * sizeof(knh_float_t));
		}
		a->size = 0;
		a->capacity = 0;
	}
}

/* ======================================================================== */
/* Iterator */

#define knh_Iterator_init_ NULL
#define knh_Iterator_copy NULL
#define knh_Iterator_traverse_ NULL
#define knh_Iterator_compareTo NULL
#define knh_Iterator_hashCode NULL
#define knh_Iterator_newClass_ NULL
#define knh_Iterator_getkey NULL

static ITRNEXT knh_fitrnext_single(Ctx *ctx, knh_sfp_t *sfp, int n)
{
	knh_Iterator_t *it = sfp[0].it;
	if(DP(it)->pos == 0) {
		DP(it)->pos = 1;
		KNH_ITRNEXT(ctx, sfp, n, DP(it)->source);
	}
	KNH_ITREND(ctx, sfp, n);
}

static void knh_Iterator_init(Ctx *ctx, knh_Iterator_t *it, int init)
{
	knh_Iterator_struct *b = DP(it);
	b->fnext  =  knh_fitrnext_single;
	KNH_INITv(b->source, KNH_NULL);
	b->pos      =  0;
	b->ref      =  NULL;
	b->freffree = NULL;
}

static void knh_Iterator_traverse(Ctx *ctx, knh_Iterator_t *it, knh_ftraverse ftr)
{
	knh_Iterator_close(ctx, it);
	ftr(ctx, DP(it)->source);
}

static ITRNEXT knh_Iterator_filterNext(Ctx *ctx, knh_sfp_t *sfp, int n)
{
	knh_Iterator_t *itr2 = (knh_Iterator_t*)DP(sfp[0].it)->source;
	knh_class_t cid = knh_Object_p1(sfp[0].it);
	DBG2_ASSERT(IS_bIterator(itr2));
	sfp = sfp + 1;
	n = n - 1;
	KNH_SETv(ctx, sfp[0].o, itr2);
	int res = SP(itr2)->fnext_1(ctx, sfp, n);
	while(res != 0) {
		if(knh_class_instanceof(ctx, knh_Object_cid(sfp[n].o), cid)) break;
		res = SP(itr2)->fnext_1(ctx, sfp, n);
	}
	return res;
}

static MAPPER Iterator_Iterator(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Mapper_t *mpr = KNH_GETMAPPER(ctx, sfp);
	DBG2_ASSERT(IS_Mapper(mpr));
	KNH_MAPPED(ctx, sfp, new_Iterator(ctx, ClassTable(DP(mpr)->tcid).p1, sfp[0].o, knh_Iterator_filterNext));
}

static void knh_Iterator_newClass(Ctx *ctx, knh_class_t cid)
{
	if(cid != CLASS_Iterator) {
		DBG2_ASSERT_cid(cid);
		DBG2_P("*** %s", CLASSN(cid));
		knh_addMapperFunc(ctx, FLAG_Mapper_Iteration, CLASS_Iterator, cid, Iterator_Iterator, KNH_NULL);
	}
}

/* ======================================================================== */
/* DictMap */

#define knh_DictMap_init_ NULL
#define knh_DictMap_copy NULL
#define knh_DictMap_traverse_ NULL
#define knh_DictMap_compareTo NULL
#define knh_DictMap_hashCode NULL
#define knh_DictMap_newClass NULL
#define knh_DictMap_getkey NULL

static void knh_DictMap_init(Ctx *ctx, knh_DictMap_t *d, int init)
{
	d->_list = knh_dictmap_malloc(ctx, (size_t)init);
	d->size = 0;
	d->fcmp = knh_bytes_strcmp;
}

static void knh_DictMap_traverse(Ctx *ctx, knh_DictMap_t *d, knh_ftraverse ftr)
{
	knh_dict_traverse(ctx, d->_list, ftr);
}

/* ======================================================================== */
/* DictSet */

#define knh_DictSet_init_ NULL
#define knh_DictSet_copy NULL
#define knh_DictSet_traverse knh_DictMap_traverse
#define knh_DictSet_compareTo NULL
#define knh_DictSet_hashCode NULL
#define knh_DictSet_newClass NULL
#define knh_DictSet_getkey NULL

static void knh_DictSet_init(Ctx *ctx, knh_DictSet_t *d, int init)
{
	d->_list = knh_dictset_malloc(ctx, (size_t)init);
	d->size = 0;
	d->fcmp = knh_bytes_strcmp;
}

/* ======================================================================== */
/* HashMap */

#define knh_HashMap_init_ NULL
#define knh_HashMap_copy NULL
#define knh_HashMap_traverse knh_Hash_traverse
#define knh_HashMap_compareTo NULL
#define knh_HashMap_hashCode NULL
#define knh_HashMap_newClass NULL
#define knh_HashMap_getkey NULL

static void knh_Hash_init(Ctx *ctx, knh_Hash_t *hash, int init)
{
	knh_Hash_struct *b = DP(hash);
	b->DBG_name = "HashMap";
	if(init > 0) {
		b->hmax = init;
		b->capacity = knh_bytes_newsize(init);
		b->array = (knh_hashentry_t**)KNH_MALLOC(ctx, sizeof(knh_hashentry_t*) * b->capacity);
		knh_bzero(b->array, sizeof(knh_hashentry_t*) * b->capacity);
	}
	else {
		b->hmax = 0;
		b->capacity = 0;
		b->array = NULL;
	}
	b->size = 0;
	b->tables = NULL;
	b->tables_size = 0;
	b->tables_capacity = 0;
	b->tables_first = NULL;
	b->unused_first = NULL;
	//
	hash->fcmp = knh_Object_compareTo;
}

static void knh_hashmapentry_traverse(Ctx *ctx, knh_hashentry_t *e, knh_ftraverse ftr)
{
	DBG2_ASSERT(e->key != NULL);
	ftr(ctx, e->key);
	ftr(ctx, e->value);
}

/* ------------------------------------------------------------------------ */
/* @data */

static knh_hash_op knh_hashmap_op = {
	sizeof(knh_hashentry_t),
	NULL,
	knh_hashmapentry_traverse,
};

static
void knh_HashMap_init(Ctx *ctx, knh_HashMap_t *o, int init)
{
	knh_Hash_t *hash = (knh_Hash_t*)o;
	knh_Hash_init(ctx, hash, init);
	DP(hash)->DBG_name = "HashMap";
	hash->fcmp = knh_Object_compareTo;
	hash->hashop = &knh_hashmap_op;
}

static void knh_Hash_traverse(Ctx *ctx, knh_Hash_t *hash, knh_ftraverse ftr)
{
	knh_Hash_struct *b = DP(hash);
	size_t i, j, esize = hash->hashop->size, tsize = KNH_HASH_TABLESIZE / esize;
	for(j = 0; j < b->tables_size; j++) {
		for(i = 0; i < tsize; i++) {
			knh_hashentry_t *e = KNH_HASHENTRY(b->tables[j], esize, i);
			if(e->key != NULL) {
				hash->hashop->ftraverse(ctx, e, ftr);
			}
		}
	}
	if(IS_SWEEP(ftr)) {
		if(b->array != NULL) {
			KNH_FREE(ctx, b->array, sizeof(knh_hashentry_t*) * b->capacity);
			b->array = NULL;
			b->size = 0;
			b->capacity = 0;
		}
		if(b->tables_size > 0) {
			for(i = 0; i < b->tables_size; i++) {
				KNH_FREE(ctx, b->tables[i], KNH_HASH_TABLESIZE);
			}
			if(b->tables_capacity > 1) {
				KNH_FREE(ctx, b->tables, sizeof(knh_hashentry_t*) * b->capacity);
			}
			b->tables = NULL;
			b->tables_capacity = 0;
			b->tables_size = 0;
		}
	}
}

/* ======================================================================== */
/* HashSet */

#define knh_HashSet_init_ NULL
#define knh_HashSet_copy NULL
#define knh_HashSet_traverse knh_Hash_traverse
#define knh_HashSet_compareTo NULL
#define knh_HashSet_hashCode NULL
#define knh_HashSet_newClass NULL
#define knh_HashSet_getkey NULL

static void knh_hashsetentry_traverse(Ctx *ctx, knh_hashentry_t *e, knh_ftraverse ftr)
{
	DBG2_ASSERT(e->key != NULL);
	ftr(ctx, e->key);
}

static knh_hash_op knh_hashset_op = {
	sizeof(knh_hashentry_t),
	NULL,
	knh_hashsetentry_traverse,
};

static void knh_HashSet_init(Ctx *ctx, knh_HashSet_t *o, int init)
{
	knh_Hash_t *hash = (knh_Hash_t*)o;
	knh_Hash_init(ctx, hash, init);
	DP(hash)->DBG_name = "HashSet";
	hash->fcmp = knh_Object_compareTo;
	hash->hashop = &knh_hashset_op;
}

/* ======================================================================== */
/* DictIdx */

#define knh_DictIdx_init_ NULL
#define knh_DictIdx_copy NULL
#define knh_DictIdx_traverse_ NULL
#define knh_DictIdx_compareTo NULL
#define knh_DictIdx_hashCode NULL
#define knh_DictIdx_newClass NULL
#define knh_DictIdx_getkey NULL

static void knh_DictIdx_init(Ctx *ctx, knh_DictIdx_t *d, int init)
{
	KNH_INITv(d->terms, new_Array(ctx, CLASS_String, init));
	KNH_INITv(d->termsDictSet, new_DictSet(ctx, init));
	d->offset = 0;
}

static void knh_DictIdx_traverse(Ctx *ctx, knh_DictIdx_t *d, knh_ftraverse ftr)
{
	ftr(ctx, UP(d->terms));
	ftr(ctx, UP(d->termsDictSet));
}

/* ======================================================================== */
/* Class */

#define knh_Class_init_ NULL
#define knh_Class_copy NULL
#define knh_Class_traverse NULL
#define knh_Class_compareTo_ NULL
#define knh_Class_hashCode_ NULL
#define knh_Class_newClass_ NULL

static void knh_Class_init(Ctx *ctx, knh_Class_t *c, int init)
{
	c->cid = (knh_class_t)init;
	c->type = (knh_type_t)init;
}

static knh_hashcode_t knh_Class_hashCode(Ctx *ctx, knh_Class_t *o)
{
	return (knh_hashcode_t)o->cid;
}

static int knh_Class_compareTo(Ctx *ctx, knh_Class_t *o, knh_Class_t *o2)
{
	return knh_strcmp(CLASSN(o->cid), CLASSN(o2->cid));
}

static void knh_Class_newClass(Ctx *ctx, knh_class_t cid)
{
	DBG2_ASSERT(cid == CLASS_Class);
	knh_setClassDefaultValue(ctx, cid, UP(new_Class(ctx, CLASS_Object)), NULL);
}

static knh_String_t *knh_Class_getkey(Ctx *ctx,knh_sfp_t *lsfp)
{
	knh_Class_t *c = (knh_Class_t*)lsfp[0].o;
	DBG2_ASSERT_cid(c->cid);
	return ClassTable(c->cid).lname;
}

/* ======================================================================== */
/* ClassField */

#define knh_ClassField_init_ NULL
#define knh_ClassField_copy NULL
#define knh_ClassField_traverse_ NULL
#define knh_ClassField_compareTo NULL
#define knh_ClassField_hashCode NULL
#define knh_ClassField_newClass NULL
#define knh_ClassField_getkey NULL

static
void knh_ClassField_init(Ctx *ctx, knh_ClassField_t *b, int init)
{
	b->fsize = init;
	if(b->fsize == 0) {
		b->fields = NULL;
	}else {
		size_t i;
		b->fields = (knh_cfield_t*)KNH_MALLOC(ctx, b->fsize * sizeof(knh_cfield_t));
		for(i = 0; i < b->fsize; i++) {
			b->fields[i].flag  = 0;
			b->fields[i].type  = TYPE_void;
			b->fields[i].fn    = FIELDN_NONAME;
			b->fields[i].value = NULL;
		}
	}
	KNH_INITv(b->methods, KNH_NULL);
}

static void knh_ClassField_traverse(Ctx *ctx, knh_ClassField_t *b, knh_ftraverse ftr)
{
	ftr(ctx, UP(b->methods));
	if(b->fields != NULL) {
		size_t i;
		for(i = 0; i < b->fsize; i++) {
			if(b->fields[i].value !=NULL) {
				ftr(ctx, b->fields[i].value);
			}
		}
		if(IS_SWEEP(ftr)) {
			//DBG2_P("freeing b->fields=%p", b->fields);
			KNH_FREE(ctx, b->fields, b->fsize * sizeof(knh_cfield_t));
			b->fields = NULL;
		}
	}
}

/* ======================================================================== */
/* MethodField */

#define knh_MethodField_init_ NULL
#define knh_MethodField_copy NULL
#define knh_MethodField_traverse_ NULL
#define knh_MethodField_compareTo NULL
#define knh_MethodField_hashCode_ NULL
#define knh_MethodField_newClass NULL
#define knh_MethodField_getkey NULL

static void knh_MethodField_init(Ctx *ctx, knh_MethodField_t *o, int init)
{
	o->psize = (size_t)init;
	if(o->psize > KNH_METHODFIELD_2) {
		o->mparams = (knh_mparam_t*)KNH_MALLOC(ctx, init * sizeof(knh_mparam_t));
		size_t i;
		for(i = 0; i < o->psize; i++) {
			o->mparams[i].type = TYPE_void;
			o->mparams[i].fn   = FIELDN_NONAME;
		}
	}
	else {
		o->p0.type = TYPE_void;
		o->p0.fn = FIELDN_NONAME;
		o->p1.type = TYPE_void;
		o->p1.fn = FIELDN_NONAME;
	}
	o->rtype = TYPE_Any;
}

/* ------------------------------------------------------------------------ */

knh_hashcode_t knh_MethodField_hashCode(Ctx *ctx, knh_MethodField_t *o)
{
	knh_hashcode_t h = o->rtype;
	if(o->psize == 1) {
		return knh_mparam_hcode(h, o->p0.type, o->p0.fn);
	}
	else if(o->psize == 2) {
		h = knh_mparam_hcode(h, o->p0.type, o->p0.fn);
		return knh_mparam_hcode(h, o->p1.type, o->p1.fn);
	}
	else {
		size_t i;
		for(i = 0; i < o->psize; i++) {
			h = knh_mparam_hcode(h, o->mparams[i].type, o->mparams[i].fn);
		}
	}
	return h;
}

/* ------------------------------------------------------------------------ */

static void knh_MethodField_traverse(Ctx *ctx, knh_MethodField_t *o, knh_ftraverse ftr)
{
	if(IS_SWEEP(ftr) && o->psize > KNH_METHODFIELD_2) {
		KNH_FREE(ctx, o->mparams, o->psize * sizeof(knh_mparam_t));
	}
}

/* ======================================================================== */
/* Method */

#define knh_Method_init_ NULL
#define knh_Method_copy NULL
#define knh_Method_traverse_ NULL
#define knh_Method_compareTo NULL
#define knh_Method_hashCode NULL
#define knh_Method_newClass NULL

static void knh_Method_init(Ctx *ctx, knh_Method_t *mtd, int init)
{
	knh_Method_struct *b = DP(mtd);
	b->flag   = 0;
	b->delta  = 0;
	b->cid    = CLASS_Object;
	b->mn     = METHODN_NONAME;
	b->fproceed  = knh_fmethod_abstract;
	KNH_INITv(b->mf, knh_findMethodField0(ctx, TYPE_void));
	b->code  = NULL;
	b->uri  = 0;  b->domain = 0;
//	b->prof_count = 0;
//	b->prof_time = 0;
}

static void knh_Method_traverse(Ctx *ctx, knh_Method_t *mtd, knh_ftraverse ftr)
{
	knh_Method_struct *b = DP(mtd);
	ftr(ctx, UP(b->mf));
	if(knh_Method_isObjectCode(mtd)) {
		ftr(ctx, (Object*)b->code);
		if(IS_SWEEP(ftr)) {
			b->code = NULL;
			knh_Method_setObjectCode(mtd, 0);
		}
	}
}

static knh_String_t *knh_Method_getkey(Ctx *ctx, knh_sfp_t *lsfp)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_write_lcid(ctx, cwb->w, DP(lsfp[0].mtd)->cid);
	knh_putc(ctx, cwb->w, '.');
	knh_write_mn(ctx, cwb->w, knh_Method_rztype(lsfp[0].mtd), DP(lsfp[0].mtd)->mn);
	return knh_cwb_newString(ctx, cwb);
}

/* ======================================================================== */
/* Mapper */

#define knh_Mapper_init_ NULL
#define knh_Mapper_copy NULL
#define knh_Mapper_traverse_ NULL
#define knh_Mapper_compareTo NULL
#define knh_Mapper_hashCode NULL
#define knh_Mapper_newClass NULL

static void knh_Mapper_init(Ctx *ctx, knh_Mapper_t *mpr, int init)
{
	knh_Mapper_struct *b = DP(mpr);
	b->size = 0;
	b->flag = 0;
	b->scid = CLASS_Object;
	b->tcid = CLASS_Object;
	//b->fmap = NULL;
	KNH_INITv(b->mapdata, KNH_NULL);
	KNH_INITv(b->m2, KNH_NULL);
}

static void knh_Mapper_traverse(Ctx *ctx, knh_Mapper_t *mpr, knh_ftraverse ftr)
{
	knh_Mapper_struct *b = DP(mpr);
	ftr(ctx, b->mapdata);
	ftr(ctx, UP(b->m2));
}

static knh_String_t *knh_Mapper_getkey(Ctx *ctx, knh_sfp_t *lsfp)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_write_lcid(ctx, cwb->w, DP(lsfp[0].mpr)->scid);
	knh_write(ctx, cwb->w, STEXT("->"));
	knh_write_lcid(ctx, cwb->w, DP(lsfp[0].mpr)->tcid);
	return knh_cwb_newString(ctx, cwb);
}

/* ======================================================================== */
/* ClassMap */

#define knh_ClassMap_init_ NULL
#define knh_ClassMap_copy NULL
#define knh_ClassMap_traverse_ NULL
#define knh_ClassMap_compareTo NULL
#define knh_ClassMap_hashCode NULL
#define knh_ClassMap_newClass NULL
#define knh_ClassMap_getkey NULL

static void knh_ClassMap_init(Ctx *ctx, knh_ClassMap_t *cmap, int init)
{
	cmap->size   = 0;
	cmap->capacity = (init < KNH_FASTMALLOC_BSIZE) ? KNH_FASTMALLOC_BSIZE : init;
	cmap->maplist   = (knh_Mapper_t**)KNH_MALLOC(ctx, cmap->capacity * sizeof(knh_Mapper_t*));
	knh_bzero(cmap->maplist, cmap->capacity * sizeof(knh_Mapper_t*));
}

static
void knh_ClassMap_traverse(Ctx *ctx, knh_ClassMap_t *cmap, knh_ftraverse ftr)
{
	int i;
	for(i = 0; i < cmap->size; i++) {
		ftr(ctx, UP(cmap->maplist[i]));
	}
	if(IS_SWEEP(ftr)) {
		KNH_FREE(ctx, cmap->maplist, cmap->capacity * sizeof(knh_Mapper_t*));
	}
}

/* ======================================================================== */
/* Closure */

#define knh_Closure_init_ NULL
#define knh_Closure_copy NULL
#define knh_Closure_traverse_ NULL
#define knh_Closure_compareTo NULL
#define knh_Closure_hashCode NULL
#define knh_Closure_newClass_ NULL
#define knh_Closure_getkey NULL

static void knh_Closure_init(Ctx *ctx, knh_Closure_t *cc, int init)
{
	cc->base = NULL;
	cc->mtd  = NULL;
	cc->envsfp = NULL;
}

static void knh_Closure_traverse(Ctx *ctx, knh_Closure_t *cc, knh_ftraverse ftr)
{
	ftr(ctx, UP(cc->mtd));
	if(cc->self != cc) {
		ftr(ctx, UP(cc->base));
	}
	if(knh_Closure_isStoredEnv(cc)) {
		size_t i, stacksize = (cc)->hstacksize[-1];
		for(i = 0; i < stacksize; i++) {
			ftr(ctx, (cc)->envsfp[i].o);
		}
		if(IS_SWEEP(ftr)) {
			KNH_FREE(ctx, (cc)->envsfp, (sizeof(knh_sfp_t) * stacksize) + sizeof(size_t));
			(cc)->envsfp = NULL;
			knh_Closure_setStoredEnv(cc, 0);
		}
	}
}

/* ======================================================================== */
/* Thunk */

#define knh_Thunk_init_ NULL
#define knh_Thunk_copy NULL
#define knh_Thunk_traverse_ NULL
#define knh_Thunk_compareTo NULL
#define knh_Thunk_hashCode NULL
#define knh_Thunk_newClass NULL
#define knh_Thunk_getkey NULL

static void knh_Thunk_init(Ctx *ctx, knh_Thunk_t *thk, int init)
{
	thk->envsfp = NULL;
	thk->envsize = 0;
	knh_Thunk_setEvaluated(thk, 0);
}

static void knh_Thunk_traverse(Ctx *ctx, knh_Thunk_t *thk, knh_ftraverse ftr)
{
	if(thk->envsize > 0) {
		size_t i;
		for(i = 0; i < (thk)->envsize; i++) {
			ftr(ctx, (thk)->envsfp[i].o);
		}
		if(IS_SWEEP(ftr)) {
			KNH_FREE(ctx, (thk)->envsfp, (sizeof(knh_sfp_t) * (thk)->envsize));
			(thk)->envsfp = NULL;
			(thk)->envsize = 0;
			knh_Thunk_setEvaluated(thk, 0);
		}
	}
}

/* ======================================================================== */
/* AffineConv */

#define knh_AffineConv_init_ NULL
#define knh_AffineConv_copy NULL
#define knh_AffineConv_traverse NULL
#define knh_AffineConv_compareTo NULL
#define knh_AffineConv_hashCode NULL
#define knh_AffineConv_newClass NULL
#define knh_AffineConv_getkey NULL

static void knh_AffineConv_init(Ctx *ctx, knh_AffineConv_t *o, int init)
{
	o->scale = KNH_FLOAT_ONE;
	o->shift = KNH_FLOAT_ZERO;
}

/* ======================================================================== */
/* Regex */

#define knh_Regex_init_ NULL
#define knh_Regex_copy NULL
#define knh_Regex_traverse_ NULL
#define knh_Regex_compareTo NULL
#define knh_Regex_hashCode NULL
#define knh_Regex_newClass_ NULL
#define knh_Regex_getkey NULL

static void knh_Regex_init(Ctx *ctx, knh_Regex_t *o, int init)
{
	KNH_INITv(o->pattern, TS_EMPTY);
	o->df = knh_System_getRegexDriver(ctx, STEXT("NOP"));
	o->reg = NULL;
}

static void knh_Regex_traverse(Ctx *ctx, knh_Regex_t *o, knh_ftraverse ftr)
{
	ftr(ctx, UP(o->pattern));
	if(IS_SWEEP(ftr)) {
		if(o->reg != NULL) {
			o->df->regfree(ctx, o->reg);
			o->df = NULL;
			o->reg = NULL;
		}
	}
}

/* ======================================================================== */
/* BytesConv */

#define knh_BytesConv_init_ NULL
#define knh_BytesConv_copy NULL
#define knh_BytesConv_traverse_ NULL
#define knh_BytesConv_compareTo NULL
#define knh_BytesConv_hashCode NULL
#define knh_BytesConv_newClass NULL
#define knh_BytesConv_getkey NULL

size_t knh_fbyteconv_nop(Ctx *ctx, knh_BytesConv_t *bc, knh_bytes_t t, knh_Bytes_t *ba)
{
	return 0;
}

void knh_fbyteconvfree_nop(Ctx *ctx, knh_BytesConv_t *bc)
{
}

static
void knh_BytesConv_init(Ctx *ctx, knh_BytesConv_t *bc, int init)
{
	bc->fbconv  = knh_fbyteconv_nop;
	bc->fbconvfree = knh_fbyteconvfree_nop;
	bc->convp = NULL;
}

static
void knh_BytesConv_traverse(Ctx *ctx, knh_BytesConv_t *bc, knh_ftraverse ftr)
{
	if(IS_SWEEP(ftr)) {
		bc->fbconvfree(ctx, bc);
		bc->fbconv = knh_fbyteconv_nop;
		bc->fbconvfree = knh_fbyteconvfree_nop;
	}
}

/* ======================================================================== */
/* ClassSpec */

#define knh_ClassSpec_init_ NULL
#define knh_ClassSpec_copy NULL
#define knh_ClassSpec_traverse_ NULL
#define knh_ClassSpec_compareTo NULL
#define knh_ClassSpec_hashCode NULL
#define knh_ClassSpec_newClass_ NULL

static
int knh_fichk__nop(knh_ClassSpec_t *u, knh_int_t v)
{
	return 1;
}

static
int knh_ficmp__signed(knh_ClassSpec_t *u, knh_int_t v1, knh_int_t v2)
{
	return (int)(v1 - v2);
}

static
int knh_ffchk__default(knh_ClassSpec_t *u, knh_float_t v)
{
	return 1;
}

static
int knh_ffcmp__default(knh_ClassSpec_t *u, knh_float_t v1, knh_float_t v2)
{
	knh_float_t delta = v1 - v2;
	if(delta == KNH_FLOAT_ZERO) return 0;
	return delta < 0 ? -1 : 1;
}

static
knh_String_t *knh_fsnew__default(Ctx *ctx, knh_class_t cid, knh_bytes_t t, knh_String_t *orig, int *foundError)
{
	return new_StringX__fast(ctx, cid, t, orig);
}

static int knh_fscmp__default(knh_ClassSpec_t *u, knh_bytes_t v1, knh_bytes_t v2)
{
	return knh_bytes_strcmp(v1, v2);
}

static
void knh_ClassSpec_init(Ctx *ctx, knh_ClassSpec_t *u, int init)
{
	knh_ClassSpec_struct *b = DP(u);
	// common
	b->flag = 0;
	b->ucid  = (knh_class_t)init;
	KNH_INITv(b->urn, TS_EMPTY);
	KNH_INITv(b->tag,  TS_EMPTY);
	b->ivalue = NULL;
	b->fvalue = NULL;
	b->svalue = NULL;

	// int
	b->imax  = KNH_INT_MAX;
	b->imin  = KNH_INT_MIN;
	b->fichk = knh_fichk__nop;
	b->ficmp = knh_ficmp__signed;

	// float
#ifndef KONOHA_ON_LKM
	b->fstep = 0.001;
#else
	b->fstep = 1;
#endif
	b->fmax  = KNH_FLOAT_MAX;
	b->fmin  = KNH_FLOAT_MIN;
	b->ffchk = knh_ffchk__default;
	b->ffcmp = knh_ffcmp__default;
//	b->ffmt = knh_funitfmt__default;
//	b->FMT  = KNH_FLOAT_FMT;

	// String
	b->fsnew = knh_fsnew__default;
	b->fscmp = knh_fscmp__default;
	//b->fsconv = NULL;
	KNH_INITv(b->bconv, KNH_NULL);
	b->charlen = 0;
	b->bytelen = 0;
	KNH_INITv(b->pattern, KNH_NULL);
	KNH_INITv(b->vocabDictIdx, KNH_NULL);
}

static
void knh_ClassSpec_traverse(Ctx *ctx, knh_ClassSpec_t *u, knh_ftraverse ftr)
{
	knh_ClassSpec_struct *b = DP(u);
	ftr(ctx, UP(b->urn));
	ftr(ctx, UP(b->tag));

	if(b->ivalue != NULL) ftr(ctx, UP(b->ivalue));
	if(b->fvalue != NULL) ftr(ctx, UP(b->fvalue));
	if(b->svalue != NULL) ftr(ctx, UP(b->svalue));

	ftr(ctx, UP(b->bconv));
	ftr(ctx, UP(b->pattern));
	ftr(ctx, UP(b->vocabDictIdx));
}

static Object *knh_Int_fdefault(Ctx *ctx, knh_class_t cid)
{
	return (Object*)KNH_INT0;
}

static Object *knh_Float_fdefault(Ctx *ctx, knh_class_t cid)
{
	return (Object*)KNH_FLOAT0;
}

static Object *knh_String_fdefault(Ctx *ctx, knh_class_t cid)
{
	return (Object*)TS_EMPTY;
}

static void knh_ClassSpec_newClass(Ctx *ctx, knh_class_t cid)
{
	knh_ClassSpec_t *u = (knh_ClassSpec_t*)new_Object_bcid(ctx, CLASS_ClassSpec, 0);
	KNH_INITv(DP(u)->ivalue, KNH_INT0);
	KNH_INITv(DP(u)->fvalue, KNH_FLOAT0);
	KNH_INITv(DP(u)->svalue, TS_EMPTY);
	knh_setClassDefaultValue(ctx, cid, UP(u), NULL);

	knh_setClassDefaultValue(ctx, CLASS_Int,    UP(u), knh_Int_fdefault);
	knh_setClassDefaultValue(ctx, CLASS_Float,  UP(u), knh_Float_fdefault);
	knh_setClassDefaultValue(ctx, CLASS_String, UP(u), knh_String_fdefault);
}

static knh_String_t* knh_ClassSpec_getkey(Ctx *ctx, knh_sfp_t *lsfp)
{
	return DP((knh_ClassSpec_t*)lsfp[0].o)->urn;
}

/* ======================================================================== */
/* InputStream */

#define knh_InputStream_init_ NULL
#define knh_InputStream_copy NULL
#define knh_InputStream_traverse_ NULL
#define knh_InputStream_compareTo NULL
#define knh_InputStream_hashCode NULL
#define knh_InputStream_newClass_ NULL

static
void knh_InputStream_init(Ctx *ctx, knh_InputStream_t *in, int init)
{
	knh_InputStream_struct *b = DP(in);
	b->fd = -1;
	KNH_INITv(b->ba, KNH_NULL);
	b->buf = "";
	b->bufpos = 0;
	b->bufend = 0;
	b->bufsiz = 0;
	b->driver = knh_getDefaultIODriver();
	KNH_INITv(b->bconv, KNH_NULL);
	KNH_INITv(b->enc, TS_ENCODING);
	KNH_INITv(b->urn, TS_DEVNULL);
	b->size    = 0;
	b->line    = 1;
	b->prev    = '\n';
	b->uri   = 0;
}

static
void knh_InputStream_traverse(Ctx *ctx, knh_InputStream_t *in, knh_ftraverse ftr)
{
	knh_InputStream_struct *b = DP(in);
	if(IS_SWEEP(ftr) && b->fd != -1) {
		b->driver->fclose(ctx, b->fd);
		b->fd = -1;
	}
	ftr(ctx, UP(b->ba));
	ftr(ctx, UP(b->enc));
	ftr(ctx, UP(b->bconv));
	ftr(ctx, UP(b->urn));
}

static
knh_String_t* knh_InputStream_getkey(Ctx *ctx, knh_sfp_t *lsfp)
{
	return DP((knh_InputStream_t*)lsfp[0].o)->urn;
}

/* ======================================================================== */
/* OutputStream */

#define knh_OutputStream_init_ NULL
#define knh_OutputStream_copy NULL
#define knh_OutputStream_traverse_ NULL
#define knh_OutputStream_compareTo NULL
#define knh_OutputStream_hashCode NULL
#define knh_OutputStream_newClass NULL

void
knh_OutputStream_init(Ctx *ctx, knh_OutputStream_t *out, int init)
{
	knh_OutputStream_struct *b = DP(out);
	b->fd = -1;
	b->driver = knh_getDefaultIODriver();
	if(init == -1) {
		KNH_INITv(b->ba, KNH_NULL);
	}
	else {
		KNH_INITv(b->ba, new_Bytes(ctx, 4096));
	}
	KNH_INITv(b->bconv, KNH_NULL);
	KNH_INITv(b->enc, TS_ENCODING);
	KNH_INITv(b->urn, TS_DEVNULL);
	b->size = 0;
	b->line = 0;
	KNH_INITv(b->NEWLINE, TS_EOL);
	KNH_INITv(b->TAB, TS_TAB);
	b->indent = 0;
	b->flag = 0;
}

static
void knh_OutputStream_traverse(Ctx *ctx, knh_OutputStream_t *out, knh_ftraverse ftr)
{
	knh_OutputStream_struct *b = DP(out);
	if(IS_SWEEP(ftr) && b->fd != -1) {
		b->driver->fclose(ctx, b->fd);
		b->fd = -1;
	}
	ftr(ctx, UP(b->ba));
	ftr(ctx, UP(b->enc));
	ftr(ctx, UP(b->bconv));
	ftr(ctx, UP(b->urn));
	ftr(ctx, UP(b->NEWLINE));
	ftr(ctx, UP(b->TAB));
}

static
knh_String_t* knh_OutputStream_getkey(Ctx *ctx, knh_sfp_t *lsfp)
{
	return DP((knh_OutputStream_t*)lsfp[0].o)->urn;
}

/* ======================================================================== */
/* Socket */

#define knh_Socket_init_ NULL
#define knh_Socket_copy NULL
#define knh_Socket_traverse_ NULL
#define knh_Socket_compareTo NULL
#define knh_Socket_hashCode NULL
#define knh_Socket_newClass_ NULL

static void knh_Socket_init(Ctx *ctx, knh_Socket_t *o, int init)
{
	knh_Socket_struct *so = DP(o);
	so->sd = -1;
	so->port = init;
	KNH_INITv(so->urn, TS_EMPTY);
	KNH_INITv(so->in, KNH_DEF(ctx, CLASS_InputStream));
	KNH_INITv(so->out, KNH_DEF(ctx, CLASS_OutputStream));
}

static
void knh_Socket_traverse(Ctx *ctx, knh_Socket_t *o, knh_ftraverse ftr)
{
	knh_Socket_struct *so = DP(o);
	if(IS_SWEEP(ftr)) {
		if(so->sd != -1) {
			knh_InputStream_close(ctx, so->in);
			knh_OutputStream_close(ctx, so->out);
			knh_socket_close(ctx, so->sd);
			so->sd = -1;
		}
	}
	ftr(ctx, UP(so->urn));
	ftr(ctx, UP(so->in));
	ftr(ctx, UP(so->out));
}

static
knh_String_t* knh_Socket_getkey(Ctx *ctx, knh_sfp_t *lsfp)
{
	return DP((knh_Socket_t*)lsfp[0].o)->urn;
}

/* ======================================================================== */
/* Connection */

#define knh_Connection_init_ NULL
#define knh_Connection_copy NULL
#define knh_Connection_traverse_ NULL
#define knh_Connection_compareTo NULL
#define knh_Connection_hashCode NULL
#define knh_Connection_newClass_ NULL
#define knh_Connection_getkey NULL

static void knh_Connection_init(Ctx *ctx, knh_Connection_t *c, int init)
{
	c->conn = NULL;
	KNH_INITv(c->urn, TS_EMPTY);
	c->df = knh_System_getDefaultDBDriver();
}

static
void knh_Connection_traverse(Ctx *ctx, knh_Connection_t *c, knh_ftraverse ftr)
{
	ftr(ctx, UP(c->urn));
	if(IS_SWEEP(ftr) && c->conn != NULL) {
		c->df->dbclose(ctx, c->conn);
		c->conn = NULL;
		c->df = NULL;
	}
}

/* ======================================================================== */
/* ResultSet */

#define knh_ResultSet_init_ NULL
#define knh_ResultSet_copy NULL
#define knh_ResultSet_traverse_ NULL
#define knh_ResultSet_compareTo NULL
#define knh_ResultSet_hashCode NULL
#define knh_ResultSet_newClass NULL
#define knh_ResultSet_getkey NULL


static
void knh_ResultSet_init(Ctx *ctx, knh_ResultSet_t *rs, int init)
{
	knh_ResultSet_struct *b = DP(rs);
	b->dbcur = NULL;
	b->tcid = CLASS_ResultSet;
	b->column_size = 0;
	b->column = NULL;
	KNH_INITv(b->databuf, new_Bytes(ctx, 256));
	KNH_INITv(b->conn, KNH_NULL);
	b->dbcur_free = knh_dbcurfree__NOP;
	b->count = 0;
}

static
void knh_ResultSet_traverse(Ctx *ctx, knh_ResultSet_t *rs, knh_ftraverse ftr)
{
	knh_ResultSet_struct *b = DP(rs);
	ftr(ctx, UP(b->databuf));
	int i = 0;
	for(i = 0; i < b->column_size; i++) {
		ftr(ctx, UP(b->column[i].name));
	}
	if(IS_SWEEP(ftr)) {
		if(b->column != NULL) {
			KNH_FREE(ctx, b->column, sizeof(knh_dbschema_t) * b->column_size);
			b->column = NULL;
		}
		if(b->dbcur != NULL) {
			b->dbcur_free(b->dbcur);
			b->dbcur = NULL;
		}
	}
	ftr(ctx, UP(b->conn));
}

/* ======================================================================== */
/* Exception */

#define knh_Exception_init_ NULL
#define knh_Exception_copy NULL
#define knh_Exception_traverse_ NULL
#define knh_Exception_compareTo NULL
#define knh_Exception_hashCode NULL
#define knh_Exception_newClass NULL
#define knh_Exception_getkey NULL

static
void knh_Exception_init(Ctx *ctx, knh_Exception_t *e, int init)
{
	knh_Exception_struct *b = DP(e);
	b->eid  = 1;
	b->flag = 0;
	KNH_INITv(b->msg, TS_EMPTY);
	KNH_INITv(b->bag, KNH_NULL);
	KNH_INITv(b->traces, KNH_NULL);
	b->file = "";
	b->line = 0;
}

static
void knh_Exception_traverse(Ctx *ctx, knh_Exception_t *e, knh_ftraverse ftr)
{
	knh_Exception_struct *b = DP(e);
	ftr(ctx, UP(b->msg));
	ftr(ctx, b->bag);
	ftr(ctx, UP(b->traces));
}


/* ======================================================================== */
/* ExceptionHandler */

#define knh_ExceptionHandler_init_ NULL
#define knh_ExceptionHandler_copy NULL
#define knh_ExceptionHandler_traverse_ NULL
#define knh_ExceptionHandler_compareTo NULL
#define knh_ExceptionHandler_hashCode NULL
#define knh_ExceptionHandler_newClass_ NULL
#define knh_ExceptionHandler_getkey NULL

static
void knh_ExceptionHandler_init(Ctx *ctx, knh_ExceptionHandler_t *hdr, int init)
{
	knh_ExceptionHandler_struct *b = DP(hdr);
	b->esp = ctx->esp;
	KNH_INITv(b->caught, KNH_NULL);
}

void
knh_ExceptionHandler_traverse(Ctx *ctx, knh_ExceptionHandler_t *hdr, knh_ftraverse ftr)
{
	knh_ExceptionHandler_struct *b = DP(hdr);
	ftr(ctx, UP(b->caught));
}

static Object *knh_ExceptionHandler_fdefault(Ctx *ctx, knh_class_t cid)
{
	return (Object*)new_Object_bcid(ctx, CLASS_ExceptionHandler, 0);
}

static void knh_ExceptionHandler_newClass(Ctx *ctx, knh_class_t cid)
{
	knh_setClassDefaultValue(ctx, cid, KNH_NULL, knh_ExceptionHandler_fdefault);
}

/* ======================================================================== */
/* Script */

#define knh_Script_init_ NULL
#define knh_Script_copy NULL
#define knh_Script_traverse knh_ObjectField_traverse
#define knh_Script_compareTo NULL
#define knh_Script_hashCode NULL
#define knh_Script_newClass NULL
#define knh_Script_getkey NULL

static
void knh_Script_init(Ctx *ctx, knh_Script_t *s, int init)
{
	s->fields = (Object**)KNH_MALLOC(ctx, sizeof(Object*) * KNH_SCRIPT_FIELDSIZE);
	knh_bzero(s->fields, sizeof(Object*) * KNH_SCRIPT_FIELDSIZE);
}

/* ======================================================================== */
/* NameSpace */

#define knh_NameSpace_init_ NULL
#define knh_NameSpace_copy NULL
#define knh_NameSpace_traverse_ NULL
#define knh_NameSpace_compareTo NULL
#define knh_NameSpace_hashCode NULL
#define knh_NameSpace_newClass_ NULL

static
void knh_NameSpace_init(Ctx *ctx, knh_NameSpace_t *ns, int init)
{
	knh_NameSpace_struct *b = DP(ns);
	if(init != KNH_OBJECT_RAWINIT) { /* @see(new_NameSpace) */
		KNH_INITv(b->nsname, KNH_NULL);
	}
	KNH_INITv(b->name2cidDictSet, new_DictSet(ctx, 64));
	KNH_INITv(b->script, KNH_NULL);
	KNH_INITv(b->importedNameSpaces, KNH_NULL);
	KNH_INITv(b->lconstDictMap, KNH_NULL);
	KNH_INITv(b->tag2urnDictMap, KNH_NULL);
	KNH_INITv(b->func2cidDictSet, KNH_NULL);
	KNH_INITv(b->pathTrustDictSet, KNH_NULL);
}

static
void knh_NameSpace_traverse(Ctx *ctx, knh_NameSpace_t *ns, knh_ftraverse ftr)
{
	knh_NameSpace_struct *b = DP(ns);
	ftr(ctx, UP(b->nsname));
	ftr(ctx, UP(b->script));
	ftr(ctx, UP(b->importedNameSpaces));
	ftr(ctx, UP(b->lconstDictMap));
	ftr(ctx, UP(b->name2cidDictSet));
	ftr(ctx, UP(b->tag2urnDictMap));
	ftr(ctx, UP(b->func2cidDictSet));
	ftr(ctx, UP(b->pathTrustDictSet));
}

static Object *knh_NameSpace_fdefault(Ctx *ctx, knh_class_t cid)
{
	return UP(ctx->share->mainns);
}

static void knh_NameSpace_newClass(Ctx *ctx, knh_class_t cid)
{
	knh_setClassDefaultValue(ctx, cid, KNH_NULL, knh_NameSpace_fdefault);
}

static knh_String_t *knh_NameSpace_getkey(Ctx *ctx, knh_sfp_t *lsfp)
{
	return DP((knh_NameSpace_t*)lsfp[0].o)->nsname;
}

/* ======================================================================== */
/* System */

#define knh_System_init_ NULL
#define knh_System_copy NULL
#define knh_System_traverse_ NULL
#define knh_System_compareTo NULL
#define knh_System_hashCode NULL
#define knh_System_newClass_ NULL
#define knh_System_getkey NULL

static knh_uintptr_t knh_autoSystemId = 0;

static void knh_System_init(Ctx *ctx, knh_System_t *o, int init)
{
	knh_System_struct *sys = DP(o);
	sys->sysid = knh_autoSystemId++;
	sys->ctxcount = 0;

	KNH_INITv(sys->ClassNameDictSet, new_DictSet(ctx, KNH_TCLASS_SIZE/2));
	KNH_INITv(sys->ExptNameDictSet, new_DictSet(ctx, KNH_TEXPT_SIZE/2));

	KNH_INITv(sys->enc,   T__(konoha_encoding()));
#ifdef KNH_USING_NOFILE
	KNH_INITv(sys->in,    new_InputStream__stdio(ctx, NULL, sys->enc));
	KNH_INITv(sys->out,   new_OutputStream__stdio(ctx, NULL, sys->enc));
	KNH_INITv(sys->err,   new_OutputStream__stdio(ctx, NULL,  sys->enc));
#else
	KNH_INITv(sys->in,    new_InputStream__stdio(ctx, stdin, sys->enc));
	KNH_INITv(sys->out,   new_OutputStream__stdio(ctx, stdout, sys->enc));
	KNH_INITv(sys->err,   new_OutputStream__stdio(ctx, stderr, sys->enc));
#endif

	KNH_INITv(sys->props, new_DictMap0(ctx, 64));
	KNH_INITv(sys->FieldNameDictIdx, new_DictIdx0__ignoreCase(ctx, KNH_TFIELDN_SIZE * 2, 0));
	KNH_INITv(sys->ResourceDictIdx, new_DictIdx0(ctx, 32, 0));
	KNH_INITv(sys->MethodFieldHashMap, new_HashMap(ctx, "System.MethodField", (KNH_TCLASS_SIZE * 2) + 31 ));
	KNH_INITv(sys->DriversTableDictSet, new_DictSet(ctx, 32));
	KNH_INITv(sys->SpecFuncDictSet, new_DictSet(ctx, 32));

	KNH_INITv(sys->NameSpaceTableDictMap, new_DictMap0(ctx, 8));
	KNH_INITv(sys->URNAliasDictMap, new_DictMap0(ctx, 8));
	KNH_INITv(sys->UsingResources, new_Array0(ctx, 0));

	KNH_INITv(sys->listenerDictMap, new_DictMap0(ctx, 0));
	KNH_INITv(sys->trustedPathDictSet, new_DictSet(ctx, 0));
}

static void knh_System_traverse(Ctx *ctx, knh_System_t *o, knh_ftraverse ftr)
{
	knh_System_struct *sys = DP(o);

	ftr(ctx, UP(sys->enc));
	ftr(ctx, UP(sys->in));
	ftr(ctx, UP(sys->out));
	ftr(ctx, UP(sys->err));

	ftr(ctx, UP(sys->props));
	ftr(ctx, UP(sys->ExptNameDictSet));
	ftr(ctx, UP(sys->ClassNameDictSet));

	ftr(ctx, UP(sys->FieldNameDictIdx));
	ftr(ctx, UP(sys->ResourceDictIdx));

	ftr(ctx, UP(sys->MethodFieldHashMap));
	ftr(ctx, UP(sys->NameSpaceTableDictMap));
	ftr(ctx, UP(sys->URNAliasDictMap));

	ftr(ctx, UP(sys->DriversTableDictSet));
	ftr(ctx, UP(sys->SpecFuncDictSet));
	ftr(ctx, UP(sys->UsingResources));
	ftr(ctx, UP(sys->listenerDictMap));
	ftr(ctx, UP(sys->trustedPathDictSet));
}

static Object *knh_System_fdefault(Ctx *ctx, knh_class_t cid)
{
	return (Object*)ctx->sys;
}

static void knh_System_newClass(Ctx *ctx, knh_class_t cid)
{
	knh_setClassDefaultValue(ctx, cid, KNH_NULL, knh_System_fdefault);
}

/* ======================================================================== */
/* Context */

#define knh_Context_init_ NULL
#define knh_Context_copy NULL
#define knh_Context_traverse NULL
#define knh_Context_compareTo NULL
#define knh_Context_hashCode NULL
#define knh_Context_newClass_ NULL
#define knh_Context_getkey NULL

void knh_Context_init(Ctx *ctx, knh_Context_t *o, int init)
{
	/* Context can be only instatiated in new_Context() */
	KNH_THROW__T(ctx, "Abort!!");
}

static Object *knh_Context_fdefault(Ctx *ctx, knh_class_t cid)
{
	return (Object*)ctx;
}

static void knh_Context_newClass(Ctx *ctx, knh_class_t cid)
{
	DBG2_ASSERT(cid == CLASS_Context);
	knh_setClassDefaultValue(ctx, cid, KNH_NULL, knh_Context_fdefault);
}

/* ======================================================================== */
/* Token */

#define knh_Token_init_ NULL
#define knh_Token_copy NULL
#define knh_Token_traverse_ NULL
#define knh_Token_compareTo NULL
#define knh_Token_hashCode NULL
#define knh_Token_newClass NULL
#define knh_Token_getkey NULL

static void knh_Token_init(Ctx *ctx, knh_Token_t *tk, int init)
{
	knh_Token_struct *b = DP(tk);
	b->type    =  TYPE_var;
	b->tt_next =  TT_EOT;
	KNH_INITv(b->data, KNH_NULL);
}

static void knh_Token_traverse(Ctx *ctx, knh_Token_t *tk, knh_ftraverse ftr)
{
	knh_Token_struct *b = DP(tk);
	ftr(ctx, b->data);
}

/* ======================================================================== */
/* Stmt */

#define knh_Stmt_init_ NULL
#define knh_Stmt_copy NULL
#define knh_Stmt_traverse_ NULL
#define knh_Stmt_compareTo NULL
#define knh_Stmt_hashCode NULL
#define knh_Stmt_newClass NULL
#define knh_Stmt_getkey NULL

static void knh_Stmt_init(Ctx *ctx, knh_Stmt_t *stmt, int init)
{
	knh_Stmt_struct *b = DP(stmt);
	b->type = TYPE_var;
	b->esp = 0;
	b->size = 0;
	b->capacity = init;
	if(b->capacity > 0) {
		size_t i;
		b->terms = (Object**)KNH_MALLOC(ctx, sizeof(Term*) * b->capacity);
		for(i = 0; i < b->capacity; i++) {
			KNH_INITv(b->terms[i], KNH_NULL);
		}
	}else {
		b->terms = NULL;
	}
	KNH_INITv(b->metaDictMap,  KNH_NULL);
	KNH_INITv(b->next,  KNH_NULL);
}

static void knh_Stmt_terms_traverse(Ctx *ctx, knh_Stmt_struct *b, knh_ftraverse ftr)
{
	size_t i;
	for(i = 0; i < b->capacity; i++) {
		ftr(ctx, b->terms[i]);
	}
	if(IS_SWEEP(ftr)) {
		KNH_FREE(ctx, b->terms, sizeof(Term*) * b->capacity);
		b->terms = NULL;
		b->capacity = 0;
		b->size = 0;
	}
}

static void knh_Stmt_traverse(Ctx *ctx, knh_Stmt_t *stmt, knh_ftraverse ftr)
{
	knh_Stmt_struct *b = DP(stmt);
	if(b->terms != NULL) {
		knh_Stmt_terms_traverse(ctx, b, ftr);
	}
	ftr(ctx, UP(b->metaDictMap));
	ftr(ctx, UP(b->next));
}

/* ------------------------------------------------------------------------ */

Term *knh_Stmt_done(Ctx *ctx, knh_Stmt_t *o)
{
	SP(o)->stt = STT_DONE;
	if(DP(o)->terms != NULL) {
		knh_Stmt_terms_traverse(ctx, DP(o), knh_Object_sweep);
	}
	KNH_SETv(ctx, DP(o)->metaDictMap, KNH_NULL);
	return (Term*)(o);
}

/* ======================================================================== */
/* Gamma */

#define knh_Gamma_init_ NULL
#define knh_Gamma_copy NULL
#define knh_Gamma_traverse_ NULL
#define knh_Gamma_compareTo NULL
#define knh_Gamma_hashCode NULL
#define knh_Gamma_newClass NULL
#define knh_Gamma_getkey NULL

static void knh_Gamma_init(Ctx *ctx, knh_Gamma_t *kc, int init)
{
	knh_Gamma_struct *b = DP(kc);
	b->flag = 0;
	b->this_cid = CLASS_Object;
	KNH_INITv(b->ns, ctx->share->mainns);
	DBG2_ASSERT(IS_NameSpace(b->ns));
	KNH_INITv(b->mtd,   KNH_NULL);

	knh_intptr_t i;
	b->gamma = (knh_cfield_t*)KNH_MALLOC(ctx, K_GAMMASIZE * sizeof(knh_cfield_t));
	knh_bzero(b->gamma, K_GAMMASIZE * sizeof(knh_cfield_t));
	for(i = 0; i < K_GAMMASIZE; i++) {
		b->gamma[i].flag  = 0;
		b->gamma[i].type  = TYPE_void;
		b->gamma[i].fn    = FIELDN_NONAME;
		b->gamma[i].value = NULL;
	}
	b->psize = 0;

	KNH_INITv(b->lstacks, new_Array0(ctx, 0));
	KNH_INITv(b->insts, new_Array0(ctx, 256));
	KNH_INITv(b->decls, new_Array0(ctx, 0));
	KNH_INITv(b->untypes, new_Array0(ctx, 0));
	KNH_INITv(b->finallyStmt, KNH_NULL);

	b->dlhdr = NULL;
	KNH_INITv(b->symbolDictMap, new_DictMap0(ctx, 256));
	KNH_INITv(b->constPools, KNH_NULL);
}

static void knh_Gamma_traverse(Ctx *ctx, knh_Gamma_t *kc, knh_ftraverse ftr)
{
	size_t i;
	knh_Gamma_struct *b = DP(kc);
	for(i = 0; i < K_GAMMASIZE; i++) {
		if(b->gamma[i].value != NULL) {
			ftr(ctx, b->gamma[i].value);
		}
	}
	if(IS_SWEEP(ftr)) {
		KNH_FREE(ctx, b->gamma, K_GAMMASIZE * sizeof(knh_cfield_t));
		b->dlhdr = NULL;
	}
	ftr(ctx, UP(b->ns));
	ftr(ctx, UP(b->mtd));
	ftr(ctx, UP(b->lstacks));
	ftr(ctx, UP(b->insts));
	ftr(ctx, UP(b->decls));
	ftr(ctx, UP(b->untypes));
	ftr(ctx, UP(b->finallyStmt));
	ftr(ctx, UP(b->symbolDictMap));
	ftr(ctx, UP(b->constPools));
}

/* ======================================================================== */
/* KLRInst */

#define knh_KLRInst_init_ NULL
#define knh_KLRInst_copy NULL
#define knh_KLRInst_traverse_ NULL
#define knh_KLRInst_compareTo NULL
#define knh_KLRInst_hashCode NULL
#define knh_KLRInst_newClass NULL
#define knh_KLRInst_getkey NULL

static void knh_KLRInst_init(Ctx *ctx, knh_KLRInst_t *inst, int opcode)
{
	if(opcode < OPCODE_NOP) {
		size_t size = knh_opcode_size(opcode);
		inst->op = (knh_inst_t*)KNH_MALLOC(ctx, size);
		knh_bzero(inst->op, size);
		inst->op->opcode = opcode;
	}
	else {
		inst->op = NULL;
	}
	inst->opcode = opcode;
	inst->line   = 0;
	inst->code_pos = NULL;
}

static void knh_KLRInst_traverse(Ctx *ctx, knh_KLRInst_t *inst, knh_ftraverse ftr)
{
	if(inst->opcode <= OPCODE_NOP) {
		knh_opcode_traverse(ctx, inst->op, ftr);
		if(IS_SWEEP(ftr)) {
			KNH_FREE(ctx, inst->op, knh_opcode_size(inst->opcode));
			inst->op = NULL;
		}
	}
}

/* ======================================================================== */
/* KLRCode */

#define knh_KLRCode_init_ NULL
#define knh_KLRCode_copy NULL
#define knh_KLRCode_traverse_ NULL
#define knh_KLRCode_compareTo NULL
#define knh_KLRCode_hashCode NULL
#define knh_KLRCode_newClass NULL
#define knh_KLRCode_getkey NULL

static void knh_KLRCode_init(Ctx *ctx, knh_KLRCode_t *o, int init)
{
	knh_KLRCode_struct *b = DP(o);
	b->size = 0;
	b->code = (knh_code_t*)"";
	b->uri = 0;
	b->dwarf_size  = 0;
	b->dwarf2_size = 0;
}

static void knh_KLRCode_traverse(Ctx *ctx, knh_KLRCode_t *o, knh_ftraverse ftr)
{
	knh_KLRCode_struct *b = DP(o);
	knh_code_t *pc = DP(o)->code;
	while(KNH_OPCODE(pc) != OPCODE_HALT) {
		knh_opcode_traverse(ctx, (knh_inst_t*)pc, ftr);
		pc += knh_opcode_size(KNH_OPCODE(pc));
	}
	if(IS_SWEEP(ftr)) {
		KNH_FREE(ctx, b->code, KNH_SIZE(b->size));
	}
}

/* ======================================================================== */

#ifdef __cplusplus
}
#endif

#define KNH_CC_METHODAPI
#include"newapi.c"
#include"objectapi.c"
#include"numberapi.c"
#include"stringapi.c"

#include"arrayapi.c"
#include"dictapi.c"
#include"hashapi.c"

#include"systemapi.c"
#include"streamapi.c"
#include"dbapi.c"

#include"oprapi.c"
#include"mprapi.c"
#include"itrapi.c"
#include"fmtapi.c"

#include"compilerapi.c"
#include"narutoapi.c"

#include"structdata.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ======================================================================== */
/* [load] */

char *knh_getStructTableName(Ctx *ctx, knh_struct_t sid)
{
	if(sid < (sizeof(StructNameData) / sizeof(char*))) {
		return StructNameData[sid];
	}
	DBG2_P("unknown sid=%d", sid);
	return "STRUCT_unknown";
}

/* ------------------------------------------------------------------------ */

static
void knh_loadStringData(Ctx *ctx, char **data)
{
	size_t i = 0;
	for(i = 0; *data != NULL; i++) {
		DBG2_ASSERT(ctx->share->tString[i] == NULL);
		DBG2_ASSERT(i < (SIZEOF_TSTRING / sizeof(knh_String_t*)));
		KNH_INITv(ctx->share->tString[i], T__(*data));
		data++;
	}
}

/* ------------------------------------------------------------------------ */

void knh_loadSystemString(Ctx *ctx)
{
	knh_loadStringData(ctx, StringData);
}

/* ------------------------------------------------------------------------ */
/* StructData */

static void knh_finit__default(Ctx *ctx, Object *o, int init)
{
}

static void knh_ftraverse__default(Ctx *ctx, Object *o, knh_ftraverse f)
{
}

static int knh_fcompareTo__default(Ctx *ctx, Object *o1, Object *o2)
{
	return (int)((knh_intptr_t)o1 - (knh_intptr_t)o2);
}

static Object *knh_fcopy__default(Ctx *ctx, Object *o)
{
	return o;
}

static knh_hashcode_t knh_fhashCode__default(Ctx *ctx, Object* o1)
{
	return ((knh_hashcode_t)o1) / sizeof(Object*);
}

static void knh_fnewClass__default(Ctx *ctx, knh_class_t cid)
{
	//DBG2_P("set default value cid=%d", cid);
}

static knh_String_t* knh_fgetkey__default(Ctx *ctx, knh_sfp_t *lsfp)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_write_cid(ctx, cwb->w, knh_Object_cid(lsfp[0].o));
	knh_putc(ctx, cwb->w, ':');
	knh_write__p(ctx, cwb->w, lsfp[0].o);
	return knh_cwb_newString(ctx, cwb);
}

/* ------------------------------------------------------------------------ */

static
void knh_loadStructData(Ctx *ctx, knh_StructData_t *data)
{
	KNH_LOCK(ctx, LOCK_SYSTBL, NULL);
	knh_StructData_t *d = data;
	while(d->name != NULL) {
		int sid = d->sid;
		if(sid == STRUCT_newid) sid = ctx->share->StructTableSize;
		KNH_ASSERT(sid == ctx->share->StructTableSize);
		((knh_SharedData_t*)ctx->share)->StructTableSize += 1;
		KNH_ASSERT(sid >= 0);
		if(!(sid < KNH_TSTRUCT_SIZE)) {
			KNH_EXIT("enlarge KNH_TSTRUCT_SIZE %d", KNH_TSTRUCT_SIZE);
			goto L_UNLOCK;
		}
		{
			knh_StructTable_t *t = pStructTable(sid);
			t->name = d->name;
			t->size = d->size;
			DBG2_ASSERT(d->size % sizeof(Object*) == 0);
			t->flag = knh_flag_oflag(d->flag);
			t->sid  = (knh_struct_t)sid;
			t->finit = (d->finit != NULL) ? d->finit : knh_finit__default;
			t->ftraverse = (d->ftraverse != NULL) ? d->ftraverse : knh_ftraverse__default;
			t->fcopy = (d->fcopy != NULL) ? d->fcopy : knh_fcopy__default;
			t->fcompareTo = (d->fcompareTo != NULL) ? d->fcompareTo : knh_fcompareTo__default;
			t->fhashCode = (d->fhashCode != NULL) ? d->fhashCode : knh_fhashCode__default;
			t->fnewClass = (d->fnewClass != NULL) ? d->fnewClass : knh_fnewClass__default;
			t->fgetkey = (d->fgetkey != NULL) ? d->fgetkey : knh_fgetkey__default;
		}
		d++;
	}
	L_UNLOCK:;
	KNH_UNLOCK(ctx, LOCK_SYSTBL, NULL);
}

/* ------------------------------------------------------------------------ */

static void knh_loadClassData(Ctx *ctx, knh_ClassData_t *data)
{
	while(data->name != NULL) {
		if(data->bcid == CLASS_Closure && data->cid != data->bcid) {
			knh_addClosureClass(ctx, data->cid, T__(data->name), data->r0, data->p1, data->p2, data->p3);
		}
		else if(data->cid != data->bcid && knh_class_isGenerics(data->bcid)) {
			knh_addGenericsClass(ctx, data->cid, T__(data->name), data->bcid, CLASS_type(data->p1), CLASS_type(data->p2));
		}
		else {
			knh_class_t cid = data->cid;
			knh_ClassTable_t *t = (knh_ClassTable_t*)(ctx->share->ClassTable + cid);
			if(cid + 1 == ctx->share->ClassTableSize) {
				((knh_SharedData_t*)ctx->share)->ClassTableSize = cid;
			}
			knh_setClassName(ctx, cid, T__(data->name));
			t->cflag  = data->flag;
			t->oflag  = knh_flag_oflag(data->flag);
			t->bcid   = data->bcid;
			t->supcid = data->supcid;
			if(data->bcid == CLASS_Closure) {
				t->p1 = data->p1; t->p2 = data->p2;
				t->r0 = data->r0; t->p3 = data->p3;
			}
			else {
				t->p1 = CLASS_type(data->p1); t->p2 = CLASS_type(data->p2);
			}
//			if(data->bcid == CLASS_Object && data->supcid != CLASS_Object) {
//				t->offset = ClassTable(data->supcid).bsize;
//			}else {
				t->offset = 0;
//			}
//			DBG2_ASSERT(t->offset == 0);
			t->sid    = (knh_struct_t)data->bcid;
			t->size =   StructTable(data->bcid).size;
			t->bsize  = t->size / sizeof(knh_Object_t*);
			DBG2_ASSERT(t->cstruct == NULL);
			if(data->method_size + data->formatter_size > 0) {
				KNH_INITv(t->cstruct, new_ClassField0(ctx, 0, data->method_size + data->formatter_size));
			}
			else {
				//DBG2_P("no method %s, %d,%d", data->name, data->method_size, data->formatter_size);
				KNH_INITv(t->cstruct, ClassTable(data->supcid).cstruct);
			}
			if(t->cmap == NULL) {
				KNH_INITv(t->cmap, knh_ClassMap_fdefault(ctx, CLASS_ClassMap));
			}
			StructTable(data->bcid).fnewClass(ctx, cid);
		}
		data++;
	}
}

/* ------------------------------------------------------------------------ */

static
void knh_loadExptData(Ctx *ctx, knh_ExptData_t *data)
{
	while(data->name != NULL) {
		knh_addException(ctx, data->flag, data->eid, T__(data->name), data->peid);
		data++;
	}
}

/* ------------------------------------------------------------------------ */

static
void knh_loadFieldNameData(Ctx *ctx, knh_FieldNameData_t *data)
{
	while(data->name != NULL) {
		knh_String_t *name = T__(data->name);
#if defined(KNH_DBGMODE2)
		knh_fieldn_t fn = (knh_fieldn_t)knh_DictIdx_add__fast(ctx, DP(ctx->sys)->FieldNameDictIdx, name);
			//DEBUG("'%s' %d %d", __tochar(name), fn, fn2);
		KNH_ASSERT(fn == data->fn);
#else
		knh_DictIdx_add__fast(ctx, DP(ctx->sys)->FieldNameDictIdx, name);
#endif
		data++;
	}
}

/* ------------------------------------------------------------------------ */

static
void knh_loadMethodFieldData(Ctx *ctx, knh_MethodFieldData_t *data, knh_MethodField_t **pools)
{
	int chk = -1;
	while(data->mfindex != -1) {
		DBG2_ASSERT(data->mfindex < KNH_TMETHODFIELD_SIZE);
		DBG2_ASSERT(data->mfindex > chk); chk = data->mfindex;
		knh_MethodField_t *mf = new_MethodField(ctx, data->rtype, data->psize);
		if(mf->psize == 1) {
			mf->p0.type = data->t1; mf->p0.fn = data->f1;
		}
		else if(mf->psize == 2) {
			mf->p0.type = data->t1; mf->p0.fn = data->f1;
			mf->p1.type = data->t2; mf->p1.fn = data->f2;
		}
		else if(mf->psize > KNH_METHODFIELD_2) {
			if(data->psize >= 1) {
				mf->mparams[0].type = data->t1;
				mf->mparams[0].fn = data->f1;
			}
			if(data->psize >= 2) {
				mf->mparams[1].type = data->t2;
				mf->mparams[1].fn = data->f2;
			}
			if(data->psize >= 3) {
				mf->mparams[2].type = data->t3;
				mf->mparams[2].fn = data->f3;
			}
			if(data->psize >= 4) {
				mf->mparams[3].type = data->t4;
				mf->mparams[3].fn = data->f4;
			}
		}
		pools[data->mfindex] = mf;
		data++;
	}
}

/* ------------------------------------------------------------------------ */

static
void knh_loadMethodData(Ctx *ctx, knh_MethodData_t *data, knh_MethodField_t **pools)
{
	while(data->func != NULL) {
		DBG2_ASSERT_cid(data->cid);
		knh_Method_t *mtd = new_Method(ctx, data->flag, data->cid, data->mn, data->func);
		DP(mtd)->code  = data->optfunc;
		DP(mtd)->delta = data->delta;
		DBG2_ASSERT(data->mfindex < KNH_TMETHODFIELD_SIZE);
		knh_MethodField_t *mf = pools[data->mfindex];
		DBG2_ASSERT(IS_MethodField(mf));
		KNH_SETv(ctx, DP(mtd)->mf, mf);
		if(knh_class_isSingleton(data->cid)) {
			DP(mtd)->flag = DP(mtd)->flag | FLAG_Method_Static;
		}
		{
			knh_ClassField_t *cs = ClassTable(data->cid).cstruct;
			knh_Array_add(ctx, cs->methods, UP(mtd));
		}
		data++;
	}
}

/* ------------------------------------------------------------------------ */

static
void knh_loadMapperData(Ctx *ctx, knh_MapperData_t *data)
{
	while(data->func != NULL) {
		knh_addMapperFunc(ctx, data->flag, data->scid, data->tcid, data->func, KNH_NULL);
		data++;
	}
}

/* ------------------------------------------------------------------------ */

static void knh_loadStringPropertyData(Ctx *ctx, knh_StringConstData_t *data)
{
	knh_DictMap_t *pmap = DP(ctx->sys)->props;
	while(data->name != NULL) {
		knh_String_t *n = T__(data->name);
		knh_DictMap_append(ctx, pmap, n, UP(T__(data->value)));
		data++;
	}
}

/* ------------------------------------------------------------------------ */
/* @data */

static knh_IntConstData_t IntConstData[] = {
	{"Int.MAX", KNH_INT_MAX},
	{"Int.MIN", KNH_INT_MIN},
	{NULL, 0}
};

static knh_FloatConstData_t FloatConstData[] = {
	{"Float.MAX", KNH_FLOAT_MAX},
	{"Float.MIN", KNH_FLOAT_MIN},
#ifndef KONOHA_ON_LKM
	{NULL, 0.0}
#else
	{NULL, 0.0}
#endif
};

static knh_StringConstData_t StringConstData[] = {
	{NULL, NULL}
};

static knh_StringConstData_t StringPropertyData[] = {
	{"konoha.version", KONOHA_VERSION},
	{"konoha.os", KONOHA_OS},
	{"konoha.platform", KONOHA_PLATFORM},
	{NULL, NULL}
};

/* ------------------------------------------------------------------------ */

void knh_loadSystemStructData(Ctx *ctx)
{
	knh_loadStructData(ctx, StructData);
}

/* ------------------------------------------------------------------------ */

void knh_loadSystemData(Ctx *ctx)
{
	knh_loadClassData(ctx, ClassData);
	knh_loadIntConstData(ctx, IntConstData);
	knh_loadFloatConstData(ctx, FloatConstData);
	knh_loadStringConstData(ctx, StringConstData);
	knh_loadStringPropertyData(ctx, StringPropertyData);
	knh_getResourceId(ctx, STEXT("(eval)"));  // URI_EVAL
	knh_loadExptData(ctx, ExptData);
	knh_loadFieldNameData(ctx, FieldNameData);
	{
		int i;
		knh_MethodField_t *pools[KNH_TMETHODFIELD_SIZE];
		knh_loadMethodFieldData(ctx, MethodFieldData, pools);
		knh_loadMethodData(ctx, MethodData, pools);
		for(i = 0; i < KNH_TMETHODFIELD_SIZE; i++) {
			knh_addMethodFieldTable(ctx, pools[i]);
		}
	}
	knh_loadMapperData(ctx, MapperData);
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
