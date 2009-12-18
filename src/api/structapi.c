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

static MAPPER Array_Iterator(Ctx *ctx, knh_sfp_t *sfp);
static MAPPER Iterator_Array(Ctx *ctx, knh_sfp_t *sfp);

/* ------------------------------------------------------------------------ */
/* DEFAULT */

static FASTAPI(void) DEFAULT_init(Ctx *ctx, Object *o, int init)
{
}

static FASTAPI(Object*) DEFAULT_copy(Ctx *ctx, Object *o, int opt)
{
	return o;
}

static FASTAPI(void) DEFAULT_traverse(Ctx *ctx, Object *o, knh_ftraverse f)
{

}

static int DEFAULT_compareTo(Ctx *ctx, Object *o1, Object *o2)
{
	return (int)((knh_intptr_t)o1 - (knh_intptr_t)o2);
}

static FASTAPI(void*) DEFAULT_hashkey(Ctx *ctx, knh_sfp_t *sfp, int opt)
{
	if(opt == KNH_FOBJECT_KEY) {
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		knh_write_cid(ctx, cwb->w, knh_Object_cid(sfp[0].o));
		knh_putc(ctx, cwb->w, ':');
		knh_write__p(ctx, cwb->w, sfp[0].o);
		return (void*)knh_cwb_newString(ctx, cwb);
	}
	else {
		knh_hashcode_t h = ((knh_hashcode_t)sfp[0].o) / sizeof(Object*);
		return (void*)h;
	}
}

static knh_Mapper_t* DEFAULT_genmap(Ctx *ctx, knh_class_t scid, knh_class_t tcid)
{
	//DBG2_P("set default value cid=%d", cid);
	return NULL;
}

/* ======================================================================== */
/* Object */

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

static FASTAPI(void) knh_ObjectField_init(Ctx *ctx, Object *o, int init)
{
	knh_ObjectField_t *of = (knh_ObjectField_t*)o;
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

static FASTAPI(void) knh_ObjectField_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
	knh_ObjectField_t *of = (knh_ObjectField_t*)o;
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

static int knh_ObjectField_compareTo(Ctx *ctx, Object *o, Object *o2)
{
	TODO();
	return o - o2;
}

static FASTAPI(void*) knh_ObjectField_hashkey(Ctx *ctx, knh_sfp_t *sfp, int opt)
{
	if(opt == KNH_FOBJECT_KEY) {
		int keyidx = ClassTable(knh_Object_cid(sfp[0].o)).keyidx;
		if(keyidx != -1) {
			knh_ObjectField_t *of = (knh_ObjectField_t*)sfp[0].o;
			DBG2_ASSERT(IS_bString(of->fields[keyidx]));
			return (void*)of->fields[keyidx];
		}
	}
	return DEFAULT_hashkey(ctx, sfp, opt);
}


static knh_ObjectCSPI_t ObjectSPI = {
	"Object", 0,
	knh_ObjectField_init,
	DEFAULT_copy,
	knh_ObjectField_traverse,
	knh_ObjectField_compareTo,
	knh_ObjectField_hashkey,
	DEFAULT_genmap,
};

/* ======================================================================== */
/* Any */

static FASTAPI(void) knh_Any_init(Ctx *ctx, Object *o, int init)
{
	knh_Glue_t *g = (knh_Glue_t *)o;
	knh_Glue_init(ctx, g, NULL, NULL);
}

static FASTAPI(void) knh_Any_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
	knh_Glue_t *g = (knh_Glue_t *)o;
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

static knh_ObjectCSPI_t AnySPI = {
	"Any", 0,
	knh_Any_init,
	DEFAULT_copy,
	knh_Any_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap,
};

/* ======================================================================== */
/* Boolean */

static FASTAPI(void*) knh_Boolean_hashkey(Ctx *ctx, knh_sfp_t *sfp, int opt)
{
	if(sfp[0].bvalue) {
		return ((opt == KNH_FOBJECT_KEY) ? (void*)TS_true : (void*)1);
	}
	else {
		return ((opt == KNH_FOBJECT_KEY) ? (void*)TS_false : (void*)0);
	}
}

static knh_ObjectCSPI_t BooleanSPI = {
	"Boolean", 0,
	DEFAULT_init,
	DEFAULT_copy,
	DEFAULT_traverse,
	DEFAULT_compareTo,
	knh_Boolean_hashkey,
	DEFAULT_genmap,
};

/* ======================================================================== */
/* Number */

static knh_ObjectCSPI_t NumberSPI = {
	"Number", 0,
	DEFAULT_init,
	DEFAULT_copy,
	DEFAULT_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap,
};

/* ======================================================================== */
/* Int */

static int knh_Int_compareTo(Ctx *ctx, Object *o, Object *o2)
{
	return (int)(knh_Object_data(o) - knh_Object_data(o2));
//	if(o->h.cid == CLASS_Int || o2->h.cid == CLASS_Int) {
//		return (int)(o->n.ivalue - o2->n.ivalue);
//	}
//	else {
//		knh_Semantics_t *u = knh_getSemantics(ctx, o->h.cid);
//		return DP(u)->ficmp(u, o->n.ivalue, o2->n.ivalue);
//	}
}

static FASTAPI(void*) knh_Int_hashkey(Ctx *ctx, knh_sfp_t *lsfp, int opt)
{
	if(opt == KNH_FOBJECT_KEY) {
		char buf[40];
#ifdef KNH_USING_INT32
		knh_snprintf(buf, sizeof(buf), "%016lu", lsfp[0].uvalue);
#else
		//18446744073709551615ULL
		knh_snprintf(buf, sizeof(buf), "%020llu", lsfp[0].uvalue);
#endif
		return (void*)new_String(ctx, B(buf), NULL);
	}
	else {
		return (void*)((knh_intptr_t)lsfp[0].uvalue);
	}
}

static FASTAPI(knh_int_t) knh_Int_toint(Ctx *ctx, knh_sfp_t *sfp)
{
	return sfp[0].ivalue;
}

static FASTAPI(knh_float_t) knh_Int_tofloat(Ctx *ctx, knh_sfp_t *sfp)
{
	return (knh_float_t)sfp[0].ivalue;
}

static knh_NumberCSPI_t IntSPI = {
	{"Int", 0,
	DEFAULT_init,
	DEFAULT_copy,
	DEFAULT_traverse,
	knh_Int_compareTo,
	knh_Int_hashkey,
	DEFAULT_genmap},
	knh_Int_toint,
	knh_Int_tofloat,
};

/* ======================================================================== */
/* Float */

static int knh_Float_compareTo(Ctx *ctx, Object *o, Object *o2)
{
	knh_float_t f = (knh_float_t)knh_Object_data(o);
	knh_float_t f2 = (knh_float_t)knh_Object_data(o2);
	if(f == f2) return 0;
	if(f < f2) return -1; else return 1;
}

static FASTAPI(knh_int_t) knh_Float_toint(Ctx *ctx, knh_sfp_t *sfp)
{
	return (knh_int_t)sfp[0].fvalue;
}

static FASTAPI(knh_float_t) knh_Float_tofloat(Ctx *ctx, knh_sfp_t *sfp)
{
	return sfp[0].fvalue;
}

static knh_NumberCSPI_t FloatSPI = {
	{"Float", 0,
	DEFAULT_init,
	DEFAULT_copy,
	DEFAULT_traverse,
	knh_Float_compareTo,
	knh_Int_hashkey,
	DEFAULT_genmap},
	knh_Float_toint, knh_Float_tofloat,
};

/* ======================================================================== */
/* String */

static FASTAPI(void) knh_String_init(Ctx *ctx, knh_Object_t *o, int init)
{
	knh_String_t *s = (knh_String_t*)o;
	s->str = (knh_uchar_t*)"";
	s->size = 0;
	s->orign = NULL;
	knh_String_setTextSgm(s, 1);
}

static FASTAPI(void) knh_String_traverse(Ctx *ctx, knh_Object_t *o, knh_ftraverse ftr)
{
	knh_String_t *s = (knh_String_t*)o;
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

static int knh_String_compareTo(Ctx *ctx, knh_Object_t *o, knh_Object_t *o2)
{
	knh_String_t *s = (knh_String_t*)o;
	knh_String_t *s2 = (knh_String_t*)o2;
	if(s->h.cid == CLASS_String || s2->h.cid == CLASS_String) {
		size_t i, max = (s->size< s2->size) ? s->size : s2->size;
		for(i = 0; i < max; i++) {
			int res = s->str[i] - s2->str[i];
			if(res != 0) return res;
		}
		return s->size - s2->size;
	}
	else {
		if(s->h.cid == s2->h.cid) {
			knh_Semantics_t *u = knh_getSemantics(ctx, s->h.cid);
			return DP(u)->fscmp(u, __tobytes(s), __tobytes(s2));
		}
		return (int)(s - s2);
	}
}

static FASTAPI(void*) knh_String_hashkey(Ctx *ctx, knh_sfp_t *sfp, int opt)
{
	if(opt == KNH_FOBJECT_KEY) {
		return (void*)sfp[0].s;
	}
	else {
		knh_String_t *s = (sfp[0].s);
		knh_hashcode_t h = s->size;
		size_t i;
		for(i = 0; i < s->size; i++) {
			h = s->str[i] + 31 * h;
		}
		return (void*)h;
	}
}

static knh_ObjectCSPI_t StringSPI = {
	"String", 0,
	knh_String_init,
	DEFAULT_copy,
	knh_String_traverse,
	knh_String_compareTo,
	knh_String_hashkey,
	DEFAULT_genmap,
};

/* ======================================================================== */
/* Bytes */

static FASTAPI(void) knh_Bytes_init(Ctx *ctx, Object *o, int init)
{
	knh_Bytes_t *b = (knh_Bytes_t*)o;
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

static FASTAPI(void) knh_Bytes_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
	knh_Bytes_t *b = (knh_Bytes_t*)o;
	if(IS_SWEEP(ftr) && b->capacity > 0) {
		KNH_FREE(ctx, b->buf, b->capacity);
		b->buf = NULL;
		b->capacity = 0;
		b->size = 0;
	}
}

static knh_ObjectCSPI_t BytesSPI = {
	"Bytes", 0,
	knh_Bytes_init,
	DEFAULT_copy,
	knh_Bytes_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap,
};

/* ======================================================================== */
/* Pair */

static FASTAPI(void) knh_Pair_init(Ctx *ctx, Object *o, int init)
{
	knh_Pair_t *t = (knh_Pair_t*)o;
	KNH_INITv(t->first, KNH_NULL);
	KNH_INITv(t->second, KNH_NULL);
}

static FASTAPI(void) knh_Pair_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
	knh_Pair_t *t = (knh_Pair_t*)o;
	ftr(ctx, t->first);
	ftr(ctx, t->second);
}

static knh_ObjectCSPI_t PairSPI = {
	"Pair", 0,
	knh_Pair_init,
	DEFAULT_copy,
	knh_Pair_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap,
};


/* ======================================================================== */
/* Tuple */

static FASTAPI(void) knh_Tuple_init(Ctx *ctx, Object *o, int init)
{
	knh_Tuple_t *t = (knh_Tuple_t*)o;
	DBG2_ASSERT(init == 0);
	t->size = 0; /* @see Tuple_new */
	t->list = NULL;
}

static FASTAPI(void) knh_Tuple_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
	knh_Tuple_t *t = (knh_Tuple_t*)o;
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

static knh_ObjectCSPI_t TupleSPI = {
	"Tuple", 0,
	knh_Tuple_init,
	DEFAULT_copy,
	knh_Tuple_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap,
};

/* ======================================================================== */
/* Range */

static MAPPER Range_Iterator(Ctx *ctx, knh_sfp_t *sfp);
#define FLAG_Mapper_Iteration (FLAG_Mapper_Synonym | FLAG_Mapper_Total)

static knh_Mapper_t* knh_Range_genmap(Ctx *ctx, knh_class_t cid, knh_class_t tcid)
{
	if(knh_class_bcid(tcid) == CLASS_Iterator) {
		knh_class_t p1 = knh_class_p1(cid); //Range<p1>
		knh_class_t tp1 = knh_class_p1(tcid);  //Iterator<tp2>
		if(p1 == tp1 || tp1 == CLASS_Any || knh_class_instanceof(ctx, p1, tp1)) {
			return new_Mapper(ctx, FLAG_Mapper_Iteration, cid, tcid, Range_Iterator, KNH_NULL);
		}
	}
	return NULL;
}

static knh_ObjectCSPI_t RangeSPI = {
	"Range", 0,
	knh_Pair_init,
	DEFAULT_copy,
	knh_Pair_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	knh_Range_genmap,
};

/* ======================================================================== */
/* Array */

static FASTAPI(void) knh_Array_init(Ctx *ctx, Object *o, int init)
{
	knh_Array_t *a = (knh_Array_t*)o;
	a->list = NULL;
	a->capacity = 0;
	a->size = 0;
	if(init > 0) {
		knh_Array_grow(ctx, a, init, KNH_NULL);
	}
}

static FASTAPI(void) knh_Array_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
	knh_Array_t *a = (knh_Array_t*)o;
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

static knh_Mapper_t* knh_Array_genmap(Ctx *ctx, knh_class_t cid, knh_class_t tcid)
{
	if(knh_class_bcid(tcid) == CLASS_Iterator) {
		knh_class_t p1 = knh_class_p1(cid);
		knh_class_t tp1 = knh_class_p1(tcid);
		if(p1 == tp1 || tp1 == CLASS_Any || knh_class_instanceof(ctx, p1, tp1)) {
			return new_Mapper(ctx, FLAG_Mapper_Iteration, cid, tcid, Array_Iterator, KNH_NULL);
		}
	}
	return NULL;
}

static knh_ObjectCSPI_t ArraySPI = {
	"Array", 0,
	knh_Array_init,
	DEFAULT_copy,
	knh_Array_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	knh_Array_genmap,
};

/* ======================================================================== */
/* IArray */

static FASTAPI(void) knh_IArray_init(Ctx *ctx, Object *o, int init)
{
	knh_IArray_t *a = (knh_IArray_t*)o;
	a->ilist = NULL;
	a->capacity = 0;
	a->size = 0;
	KNH_ASSERT(init == 0);
}

static FASTAPI(void) knh_IArray_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
	knh_IArray_t *a = (knh_IArray_t*)o;
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

static knh_ObjectCSPI_t IArraySPI = {
	"IArray", 0,
	knh_IArray_init,
	DEFAULT_copy,
	knh_IArray_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap/*knh_IArray_genmap*/,
};

/* ======================================================================== */
/* FArray */

static FASTAPI(void) knh_FArray_init(Ctx *ctx, Object *o, int init)
{
	knh_FArray_t *a = (knh_FArray_t*)o;
	a->flist = NULL;
	a->capacity = 0;
	a->size = 0;
	KNH_ASSERT(init == 0);
}

static FASTAPI(void) knh_FArray_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
	knh_FArray_t *a = (knh_FArray_t*)o;
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

static knh_ObjectCSPI_t FArraySPI = {
	"FArray", 0,
	knh_FArray_init,
	DEFAULT_copy,
	knh_FArray_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap /*knh_FArray_genmap*/,
};

/* ======================================================================== */
/* Iterator */

static ITRNEXT knh_fitrnext_single(Ctx *ctx, knh_sfp_t *sfp, int n)
{
	knh_Iterator_t *it = sfp[0].it;
	if(DP(it)->pos == 0) {
		DP(it)->pos = 1;
		KNH_ITRNEXT(ctx, sfp, n, DP(it)->source);
	}
	KNH_ITREND(ctx, sfp, n);
}

static FASTAPI(void) knh_Iterator_init(Ctx *ctx, Object *o, int init)
{
	knh_Iterator_t *it = (knh_Iterator_t*)o;
	knh_Iterator_struct *b = DP(it);
	b->fnext  =  knh_fitrnext_single;
	KNH_INITv(b->source, KNH_NULL);
	b->pos      =  0;
	b->ref      =  NULL;
	b->freffree = NULL;
}

static FASTAPI(void) knh_Iterator_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
	knh_Iterator_t *it = (knh_Iterator_t*)o;
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

static MAPPER Iterator_Iterator(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_Mapper_t *mpr = KNH_GETMAPPER(ctx, sfp);
	DBG2_ASSERT(IS_Mapper(mpr));
	KNH_MAPPED(ctx, sfp, new_Iterator(ctx, ClassTable(DP(mpr)->tcid).p1, sfp[0].o, knh_Iterator_filterNext));
}

static knh_Mapper_t* knh_Iterator_genmap(Ctx *ctx, knh_class_t cid, knh_class_t tcid)
{
	if(cid == CLASS_Iterator && knh_class_bcid(cid) == CLASS_Iterator) {
		return new_Mapper(ctx, FLAG_Mapper_Iteration, CLASS_Iterator, cid, Iterator_Iterator, KNH_NULL);
	}
	if(knh_class_bcid(tcid) == CLASS_Array) {
		knh_class_t p1 = knh_class_p1(cid);
		knh_class_t tp1 = knh_class_p1(tcid);
		if(p1 == tp1 || tp1 == CLASS_Any || knh_class_instanceof(ctx, p1, tp1)) {
			return new_Mapper(ctx, FLAG_Mapper_Iteration, cid, tcid, Iterator_Array, KNH_NULL);
		}
	}
	return NULL;
}

static knh_ObjectCSPI_t IteratorSPI = {
	"Iterator", sizeof(knh_Iterator_struct),
	knh_Iterator_init,
	DEFAULT_copy,
	knh_Iterator_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	knh_Iterator_genmap,
};

/* ======================================================================== */
/* DictMap */

static FASTAPI(void) knh_DictMap_init(Ctx *ctx, Object *o, int init)
{
	knh_DictMap_t *d = (knh_DictMap_t*)o;
	d->_list = knh_dictmap_malloc(ctx, (size_t)init);
	d->size = 0;
	d->fcmp = knh_bytes_strcmp;
}

static FASTAPI(void) knh_DictMap_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
	knh_DictMap_t *d = (knh_DictMap_t*)o;
	knh_dict_traverse(ctx, d->_list, ftr);
}

static knh_ObjectCSPI_t DictMapSPI = {
	"DictMap", 0,
	knh_DictMap_init,
	DEFAULT_copy,
	knh_DictMap_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap /*knh_DictMap_genmap*/,
};

/* ======================================================================== */
/* DictSet */

static FASTAPI(void) knh_DictSet_init(Ctx *ctx, Object *o, int init)
{
	knh_DictSet_t *d = (knh_DictSet_t*)o;
	d->_list = knh_dictset_malloc(ctx, (size_t)init);
	d->size = 0;
	d->fcmp = knh_bytes_strcmp;
}

static knh_ObjectCSPI_t DictSetSPI = {
	"DictSet", 0,
	knh_DictSet_init,
	DEFAULT_copy,
	knh_DictMap_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap /*knh_DictMap_genmap*/,
};

/* ======================================================================== */
/* HashMap */

static FASTAPI(void) knh_Hash_init(Ctx *ctx, knh_Hash_t *hash, int init)
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

static FASTAPI(void) knh_hashmapentry_traverse(Ctx *ctx, knh_hashentry_t *e, knh_ftraverse ftr)
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

static FASTAPI(void) knh_HashMap_init(Ctx *ctx, Object *o, int init)
{
	knh_Hash_t *hash = (knh_Hash_t*)o;
	knh_Hash_init(ctx, hash, init);
	DP(hash)->DBG_name = "HashMap";
	hash->fcmp = knh_Object_compareTo;
	hash->hashop = &knh_hashmap_op;
}

static FASTAPI(void) knh_Hash_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
	knh_Hash_t *hash = (knh_Hash_t*)o;
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

static knh_ObjectCSPI_t HashMapSPI = {
	"HashMap", sizeof(knh_HashMap_struct),
	knh_HashMap_init,
	DEFAULT_copy,
	knh_Hash_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap /*knh_DictMap_genmap*/,
};

/* ======================================================================== */
/* HashSet */

static FASTAPI(void) knh_hashsetentry_traverse(Ctx *ctx, knh_hashentry_t *e, knh_ftraverse ftr)
{
	DBG2_ASSERT(e->key != NULL);
	ftr(ctx, e->key);
}

static knh_hash_op knh_hashset_op = {
	sizeof(knh_hashentry_t),
	NULL,
	knh_hashsetentry_traverse,
};

static FASTAPI(void) knh_HashSet_init(Ctx *ctx, Object *o, int init)
{
	knh_Hash_t *hash = (knh_Hash_t*)o;
	knh_Hash_init(ctx, hash, init);
	DP(hash)->DBG_name = "HashSet";
	hash->fcmp = knh_Object_compareTo;
	hash->hashop = &knh_hashset_op;
}

static knh_ObjectCSPI_t HashSetSPI = {
	"HashSet", sizeof(knh_HashSet_struct),
	knh_HashSet_init,
	DEFAULT_copy,
	knh_Hash_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap /*knh_DictMap_genmap*/,
};

/* ======================================================================== */
/* DictIdx */

static FASTAPI(void) knh_DictIdx_init(Ctx *ctx, Object *o, int init)
{
	knh_DictIdx_t *d = (knh_DictIdx_t*)o;
	KNH_INITv(d->terms, new_Array(ctx, CLASS_String, init));
	KNH_INITv(d->termsDictSet, new_DictSet(ctx, init));
	d->offset = 0;
}

static FASTAPI(void) knh_DictIdx_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
	knh_DictIdx_t *d = (knh_DictIdx_t*)o;
	ftr(ctx, UP(d->terms));
	ftr(ctx, UP(d->termsDictSet));
}

static knh_ObjectCSPI_t DictIdxSPI = {
	"DictIdx", 0,
	knh_DictIdx_init,
	DEFAULT_copy,
	knh_DictIdx_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap /*knh_DictMap_genmap*/,
};

/* ======================================================================== */
/* Class */

static FASTAPI(void) knh_Class_init(Ctx *ctx, Object *o, int init)
{
	knh_Class_t *c = (knh_Class_t*)o;
	c->cid = (knh_class_t)init;
	c->type = (knh_type_t)init;
}

static int knh_Class_compareTo(Ctx *ctx, knh_Object_t *o, knh_Object_t *o2)
{
	knh_Class_t *c = (knh_Class_t*)o;
	knh_Class_t *c2 = (knh_Class_t*)o2;
	return knh_strcmp(CLASSN(c->cid), CLASSN(c2->cid));
}

static FASTAPI(void*) knh_Class_hashkey(Ctx *ctx,knh_sfp_t *sfp, int opt)
{
	knh_Class_t *c = (knh_Class_t*)sfp[0].o;
	knh_intptr_t cid = c->cid;
	if(opt == KNH_FOBJECT_KEY) {
		DBG2_ASSERT_cid(cid);
		return (void*)(ClassTable(cid).lname);
	}
	else {
		return (void*)cid;
	}
}

static knh_ObjectCSPI_t ClassSPI = {
	"Class", 0,
	knh_Class_init,
	DEFAULT_copy,
	DEFAULT_traverse,
	knh_Class_compareTo,
	knh_Class_hashkey,
	DEFAULT_genmap /*knh_DictMap_genmap*/,
};

/* ======================================================================== */
/* ClassField */

static FASTAPI(void) knh_ClassField_init(Ctx *ctx, Object *o, int init)
{
	knh_ClassField_t *b = (knh_ClassField_t*)o;
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

static FASTAPI(void) knh_ClassField_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
	knh_ClassField_t *b = (knh_ClassField_t*)o;
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

static knh_ObjectCSPI_t ClassFieldSPI = {
	"ClassField", 0,
	knh_ClassField_init,
	DEFAULT_copy,
	knh_ClassField_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap /*knh_DictMap_genmap*/,
};

/* ======================================================================== */
/* MethodField */

static FASTAPI(void) knh_MethodField_init(Ctx *ctx, Object *o, int init)
{
	knh_MethodField_t *mf = (knh_MethodField_t*)o;
	mf->psize = (size_t)init;
	if(mf->psize > KNH_METHODFIELD_2) {
		mf->mparams = (knh_mparam_t*)KNH_MALLOC(ctx, init * sizeof(knh_mparam_t));
		size_t i;
		for(i = 0; i < mf->psize; i++) {
			mf->mparams[i].type = TYPE_void;
			mf->mparams[i].fn   = FIELDN_NONAME;
		}
	}
	else {
		mf->p0.type = TYPE_void;
		mf->p0.fn = FIELDN_NONAME;
		mf->p1.type = TYPE_void;
		mf->p1.fn = FIELDN_NONAME;
	}
	mf->rtype = TYPE_Any;
}

static FASTAPI(void) knh_MethodField_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
	knh_MethodField_t *mf = (knh_MethodField_t*)o;
	if(IS_SWEEP(ftr) && mf->psize > KNH_METHODFIELD_2) {
		KNH_FREE(ctx, mf->mparams, mf->psize * sizeof(knh_mparam_t));
	}
}

static FASTAPI(void*) knh_MethodField_hashkey(Ctx *ctx, knh_sfp_t *sfp, int opt)
{
	if(opt == KNH_FOBJECT_HASH) {
		knh_MethodField_t *mf = (knh_MethodField_t*)sfp[0].o;
		knh_hashcode_t h = mf->rtype;
		if(mf->psize == 1) {
			h = knh_mparam_hcode(h, mf->p0.type, mf->p0.fn);
		}
		else if(mf->psize == 2) {
			h = knh_mparam_hcode(h, mf->p0.type, mf->p0.fn);
			h = knh_mparam_hcode(h, mf->p1.type, mf->p1.fn);
		}
		else {
			size_t i;
			for(i = 0; i < mf->psize; i++) {
				h = knh_mparam_hcode(h, mf->mparams[i].type, mf->mparams[i].fn);
			}
		}
		return (void*)h;
	}
	return DEFAULT_hashkey(ctx, sfp, opt);
}

static knh_ObjectCSPI_t MethodFieldSPI = {
	"MethodField", 0,
	knh_MethodField_init,
	DEFAULT_copy,
	knh_MethodField_traverse,
	DEFAULT_compareTo,
	knh_MethodField_hashkey,
	DEFAULT_genmap /*knh_DictMap_genmap*/,
};

/* ======================================================================== */
/* Method */

static FASTAPI(void) knh_Method_init(Ctx *ctx, Object *o, int init)
{
	knh_Method_t *mtd = (knh_Method_t*)o;
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

static FASTAPI(void) knh_Method_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
	knh_Method_t *mtd = (knh_Method_t*)o;
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

static FASTAPI(void*) knh_Method_hashkey(Ctx *ctx, knh_sfp_t *sfp, int opt)
{
	if(opt == KNH_FOBJECT_KEY) {
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		knh_write_lcid(ctx, cwb->w, DP(sfp[0].mtd)->cid);
		knh_putc(ctx, cwb->w, '.');
		knh_write_mn(ctx, cwb->w, knh_Method_rztype(sfp[0].mtd), DP(sfp[0].mtd)->mn);
		return (void*)knh_cwb_newString(ctx, cwb);
	}
	return DEFAULT_hashkey(ctx, sfp, opt);
}

static knh_ObjectCSPI_t MethodSPI = {
	"Method", sizeof(knh_Method_struct),
	knh_Method_init,
	DEFAULT_copy,
	knh_Method_traverse,
	DEFAULT_compareTo,
	knh_Method_hashkey,
	DEFAULT_genmap /*knh_DictMap_genmap*/,
};

/* ======================================================================== */
/* Mapper */

static FASTAPI(void) knh_Mapper_init(Ctx *ctx, Object *o, int init)
{
	knh_Mapper_t *mpr = (knh_Mapper_t*)o;
	knh_Mapper_struct *b = DP(mpr);
	b->size = 0;
	b->flag = 0;
	b->scid = CLASS_Object;
	b->tcid = CLASS_Object;
	//b->fmap = NULL;
	KNH_INITv(b->mapdata, KNH_NULL);
	KNH_INITv(b->m2, KNH_NULL);
}

static FASTAPI(void) knh_Mapper_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
	knh_Mapper_t *mpr = (knh_Mapper_t*)o;
	knh_Mapper_struct *b = DP(mpr);
	ftr(ctx, b->mapdata);
	ftr(ctx, UP(b->m2));
}

static FASTAPI(void*) knh_Mapper_hashkey(Ctx *ctx, knh_sfp_t *sfp, int opt)
{
	if(opt == KNH_FOBJECT_KEY) {
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		knh_write_lcid(ctx, cwb->w, DP(sfp[0].mpr)->scid);
		knh_write(ctx, cwb->w, STEXT("->"));
		knh_write_lcid(ctx, cwb->w, DP(sfp[0].mpr)->tcid);
		return (void*)knh_cwb_newString(ctx, cwb);
	}
	return DEFAULT_hashkey(ctx, sfp, opt);
}

static knh_ObjectCSPI_t MapperSPI = {
	"Mapper", sizeof(knh_Mapper_struct),
	knh_Mapper_init,
	DEFAULT_copy,
	knh_Mapper_traverse,
	DEFAULT_compareTo,
	knh_Mapper_hashkey,
	DEFAULT_genmap /*knh_DictMap_genmap*/,
};


/* ======================================================================== */
/* ClassMap */

static FASTAPI(void) knh_ClassMap_init(Ctx *ctx, Object *o, int init)
{
	knh_ClassMap_t *cmap = (knh_ClassMap_t*)o;
	cmap->size   = 0;
	cmap->capacity = (init < KNH_FASTMALLOC_BSIZE) ? KNH_FASTMALLOC_BSIZE : init;
	cmap->maplist   = (knh_Mapper_t**)KNH_MALLOC(ctx, cmap->capacity * sizeof(knh_Mapper_t*));
	knh_bzero(cmap->maplist, cmap->capacity * sizeof(knh_Mapper_t*));
}

static FASTAPI(void) knh_ClassMap_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
	knh_ClassMap_t *cmap = (knh_ClassMap_t*)o;
	int i;
	for(i = 0; i < cmap->size; i++) {
		ftr(ctx, UP(cmap->maplist[i]));
	}
	if(IS_SWEEP(ftr)) {
		KNH_FREE(ctx, cmap->maplist, cmap->capacity * sizeof(knh_Mapper_t*));
	}
}

static knh_ObjectCSPI_t ClassMapSPI = {
	"ClassMap", 0,
	knh_ClassMap_init,
	DEFAULT_copy,
	knh_ClassMap_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap,
};

/* ======================================================================== */
/* Closure */

static FASTAPI(void) knh_Closure_init(Ctx *ctx, Object *o, int init)
{
	knh_Closure_t *cc = (knh_Closure_t*)o;
	cc->base = NULL;
	cc->mtd  = NULL;
	cc->envsfp = NULL;
}

static FASTAPI(void) knh_Closure_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
	knh_Closure_t *cc = (knh_Closure_t*)o;
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

static knh_ObjectCSPI_t ClosureSPI = {
	"Closure", 0,
	knh_Closure_init,
	DEFAULT_copy,
	knh_Closure_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap,
};

/* ======================================================================== */
/* Thunk */

static FASTAPI(void) knh_Thunk_init(Ctx *ctx, Object *o, int init)
{
	knh_Thunk_t *thk = (knh_Thunk_t*)o;
	thk->envsfp = NULL;
	thk->envsize = 0;
	knh_Thunk_setEvaluated(thk, 0);
}

static FASTAPI(void) knh_Thunk_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
	knh_Thunk_t *thk = (knh_Thunk_t*)o;
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

static knh_ObjectCSPI_t ThunkSPI = {
	"Thunk", 0,
	knh_Thunk_init,
	DEFAULT_copy,
	knh_Thunk_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap,
};

/* ======================================================================== */
/* AffineConv */

static knh_ObjectCSPI_t AffineConvSPI = {
	"AffineConv", 0,
	DEFAULT_init,
	DEFAULT_copy,
	DEFAULT_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap,
};

/* ======================================================================== */
/* Regex */

static FASTAPI(void) knh_Regex_init(Ctx *ctx, Object *o, int init)
{
	knh_Regex_t *re = (knh_Regex_t*)o;
	KNH_INITv(re->pattern, TS_EMPTY);
	re->df = knh_System_getRegexDriver(ctx, STEXT("NOP"));
	re->reg = NULL;
}

static FASTAPI(void) knh_Regex_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
	knh_Regex_t *re = (knh_Regex_t*)o;
	ftr(ctx, UP(re->pattern));
	if(IS_SWEEP(ftr)) {
		if(re->reg != NULL) {
			re->df->regfree(ctx, re->reg);
			re->df = NULL;
			re->reg = NULL;
		}
	}
}

static knh_ObjectCSPI_t RegexSPI = {
	"Regex", 0,
	knh_Regex_init,
	DEFAULT_copy,
	knh_Regex_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap,
};

/* ======================================================================== */
/* BytesConv */

size_t knh_fbyteconv_nop(Ctx *ctx, knh_BytesConv_t *bc, knh_bytes_t t, knh_Bytes_t *ba)
{
	return 0;
}

void knh_fbyteconvfree_nop(Ctx *ctx, knh_BytesConv_t *bc)
{
}

static FASTAPI(void) knh_BytesConv_init(Ctx *ctx, Object *o, int init)
{
	knh_BytesConv_t *bc = (knh_BytesConv_t*)o;
	bc->fbconv  = knh_fbyteconv_nop;
	bc->fbconvfree = knh_fbyteconvfree_nop;
	bc->convp = NULL;
}

static FASTAPI(void) knh_BytesConv_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
	knh_BytesConv_t *bc = (knh_BytesConv_t*)o;
	if(IS_SWEEP(ftr)) {
		bc->fbconvfree(ctx, bc);
		bc->fbconv = knh_fbyteconv_nop;
		bc->fbconvfree = knh_fbyteconvfree_nop;
	}
}

static knh_ObjectCSPI_t BytesConvSPI = {
	"BytesConv", 0,
	knh_BytesConv_init,
	DEFAULT_copy,
	knh_BytesConv_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap,
};

/* ======================================================================== */
/* Semantics */

static int knh_fichk__nop(knh_Semantics_t *u, knh_int_t v)
{
	return 1;
}

static int knh_ficmp__signed(knh_Semantics_t *u, knh_int_t v1, knh_int_t v2)
{
	return (int)(v1 - v2);
}

static int knh_ffchk__default(knh_Semantics_t *u, knh_float_t v)
{
	return 1;
}

static int knh_ffcmp__default(knh_Semantics_t *u, knh_float_t v1, knh_float_t v2)
{
	knh_float_t delta = v1 - v2;
	if(delta == KNH_FLOAT_ZERO) return 0;
	return delta < 0 ? -1 : 1;
}

static knh_String_t *knh_fsnew__default(Ctx *ctx, knh_class_t cid, knh_bytes_t t, knh_String_t *orig, int *foundError)
{
	return new_StringX__fast(ctx, cid, t, orig);
}

static int knh_fscmp__default(knh_Semantics_t *u, knh_bytes_t v1, knh_bytes_t v2)
{
	return knh_bytes_strcmp(v1, v2);
}

static FASTAPI(void) knh_Semantics_init(Ctx *ctx, Object *o, int init)
{
	knh_Semantics_t *u = (knh_Semantics_t*)o;
	knh_Semantics_struct *b = DP(u);
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

static FASTAPI(void) knh_Semantics_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
	knh_Semantics_t *u = (knh_Semantics_t*)o;
	knh_Semantics_struct *b = DP(u);
	ftr(ctx, UP(b->urn));
	ftr(ctx, UP(b->tag));

	if(b->ivalue != NULL) ftr(ctx, UP(b->ivalue));
	if(b->fvalue != NULL) ftr(ctx, UP(b->fvalue));
	if(b->svalue != NULL) ftr(ctx, UP(b->svalue));

	ftr(ctx, UP(b->bconv));
	ftr(ctx, UP(b->pattern));
	ftr(ctx, UP(b->vocabDictIdx));
}

static FASTAPI(void*) knh_Semantics_hashkey(Ctx *ctx, knh_sfp_t *sfp, int opt)
{
	if(opt == KNH_FOBJECT_KEY) {
		return (void*)DP((knh_Semantics_t*)sfp[0].o)->urn;
	}
	return DEFAULT_hashkey(ctx, sfp, opt);
}


static knh_ObjectCSPI_t SemanticsSPI = {
	"Semantics", sizeof(knh_Semantics_struct),
	knh_Semantics_init,
	DEFAULT_copy,
	knh_Semantics_traverse,
	DEFAULT_compareTo,
	knh_Semantics_hashkey,
	DEFAULT_genmap,
};

/* ======================================================================== */
/* InputStream */

static FASTAPI(void) knh_InputStream_init(Ctx *ctx, Object *o, int init)
{
	knh_InputStream_struct *b = DP((knh_InputStream_t*)o);
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

static FASTAPI(void) knh_InputStream_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
	knh_InputStream_struct *b = DP((knh_InputStream_t*)o);
	if(IS_SWEEP(ftr) && b->fd != -1) {
		b->driver->fclose(ctx, b->fd);
		b->fd = -1;
	}
	ftr(ctx, UP(b->ba));
	ftr(ctx, UP(b->enc));
	ftr(ctx, UP(b->bconv));
	ftr(ctx, UP(b->urn));
}

static knh_ObjectCSPI_t InputStreamSPI = {
	"InputStream", sizeof(knh_InputStream_struct),
	knh_InputStream_init,
	DEFAULT_copy,
	knh_InputStream_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap,
};

/* ======================================================================== */
/* OutputStream */

static FASTAPI(void) knh_OutputStream_init(Ctx *ctx, Object *o, int init)
{
	knh_OutputStream_struct *b = DP((knh_OutputStream_t*)o);
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

static FASTAPI(void) knh_OutputStream_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
	knh_OutputStream_struct *b = DP((knh_OutputStream_t*)o);
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

static knh_ObjectCSPI_t OutputStreamSPI = {
	"OutputStream", sizeof(knh_InputStream_struct),
	knh_OutputStream_init,
	DEFAULT_copy,
	knh_OutputStream_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap,
};

/* ======================================================================== */
/* Socket */

static FASTAPI(void) knh_Socket_init(Ctx *ctx, Object *o, int init)
{
	knh_Socket_struct *so = DP((knh_Socket_t*)o);
	so->sd = -1;
	so->port = init;
	KNH_INITv(so->urn, TS_EMPTY);
	KNH_INITv(so->in, KNH_DEF(ctx, CLASS_InputStream));
	KNH_INITv(so->out, KNH_DEF(ctx, CLASS_OutputStream));
}

static FASTAPI(void) knh_Socket_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
	knh_Socket_struct *so = DP((knh_Socket_t*)o);
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

static knh_ObjectCSPI_t SocketSPI = {
	"Socket", sizeof(knh_Socket_struct),
	knh_Socket_init,
	DEFAULT_copy,
	knh_Socket_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap,
};

/* ======================================================================== */
/* Connection */

static FASTAPI(void) knh_Connection_init(Ctx *ctx, Object *o, int init)
{
	knh_Connection_t *c = (knh_Connection_t*)o;
	c->conn = NULL;
	KNH_INITv(c->urn, TS_EMPTY);
	c->df = knh_System_getDefaultDBDriver();
}

static FASTAPI(void) knh_Connection_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
	knh_Connection_t *c = (knh_Connection_t*)o;
	ftr(ctx, UP(c->urn));
	if(IS_SWEEP(ftr) && c->conn != NULL) {
		c->df->dbclose(ctx, c->conn);
		c->conn = NULL;
		c->df = NULL;
	}
}

static knh_ObjectCSPI_t ConnectionSPI = {
	"Connection", 0,
	knh_Connection_init,
	DEFAULT_copy,
	knh_Connection_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap,
};

/* ======================================================================== */
/* ResultSet */

static FASTAPI(void) knh_ResultSet_init(Ctx *ctx, Object *o, int init)
{
	knh_ResultSet_struct *b = DP((knh_ResultSet_t*)o);
	b->dbcur = NULL;
	b->tcid = CLASS_ResultSet;
	b->column_size = 0;
	b->column = NULL;
	KNH_INITv(b->databuf, new_Bytes(ctx, 256));
	KNH_INITv(b->conn, KNH_NULL);
	b->dbcur_free = knh_dbcurfree__NOP;
	b->count = 0;
}

static FASTAPI(void) knh_ResultSet_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
	knh_ResultSet_struct *b = DP((knh_ResultSet_t*)o);
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

static knh_ObjectCSPI_t ResultSetSPI = {
	"ResultSet", sizeof(knh_ResultSet_struct),
	knh_ResultSet_init,
	DEFAULT_copy,
	knh_ResultSet_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap,
};

/* ======================================================================== */
/* Exception */

static FASTAPI(void) knh_Exception_init(Ctx *ctx, Object *o, int init)
{
	knh_Exception_struct *b = DP((knh_Exception_t*)o);
	b->eid  = 1;
	b->flag = 0;
	KNH_INITv(b->msg, TS_EMPTY);
	KNH_INITv(b->bag, KNH_NULL);
	KNH_INITv(b->traces, KNH_NULL);
	b->file = "";
	b->line = 0;
}

static FASTAPI(void) knh_Exception_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
	knh_Exception_struct *b = DP((knh_Exception_t*)o);
	ftr(ctx, UP(b->msg));
	ftr(ctx, b->bag);
	ftr(ctx, UP(b->traces));
}

static knh_ObjectCSPI_t ExceptionSPI = {
	"Exception", sizeof(knh_Exception_struct),
	knh_Exception_init,
	DEFAULT_copy,
	knh_Exception_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap,
};

/* ======================================================================== */
/* ExceptionHandler */

static FASTAPI(void) knh_ExceptionHandler_init(Ctx *ctx, Object *o, int init)
{
	knh_ExceptionHandler_struct *b = DP((knh_ExceptionHandler_t*)o);
	b->esp = ctx->esp;
	KNH_INITv(b->caught, KNH_NULL);
}

static FASTAPI(void) knh_ExceptionHandler_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
	knh_ExceptionHandler_struct *b = DP((knh_ExceptionHandler_t*)o);
	ftr(ctx, UP(b->caught));
}

static knh_ObjectCSPI_t ExceptionHandlerSPI = {
	"ExceptionHandler", sizeof(knh_ExceptionHandler_struct),
	knh_ExceptionHandler_init,
	DEFAULT_copy,
	knh_ExceptionHandler_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap,
};

/* ======================================================================== */
/* Script */

static FASTAPI(void) knh_Script_init(Ctx *ctx, Object *o, int init)
{
	knh_Script_t *s = (knh_Script_t*)o;
	s->fields = (Object**)KNH_MALLOC(ctx, sizeof(Object*) * KNH_SCRIPT_FIELDSIZE);
	knh_bzero(s->fields, sizeof(Object*) * KNH_SCRIPT_FIELDSIZE);
}

static knh_ObjectCSPI_t ScriptSPI = {
	"Script", 0,
	knh_Script_init,
	DEFAULT_copy,
	knh_ObjectField_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap,
};

/* ======================================================================== */
/* NameSpace */

static FASTAPI(void) knh_NameSpace_init(Ctx *ctx, Object *o, int init)
{
	knh_NameSpace_struct *b = DP((knh_NameSpace_t*)o);
	KNH_INITv(b->nsname, KNH_NULL);
	KNH_INITv(b->name2cidDictSet, new_DictSet(ctx, 64));
	KNH_INITv(b->script, KNH_NULL);
	KNH_INITv(b->importedNameSpaces, KNH_NULL);
	KNH_INITv(b->lconstDictMap, KNH_NULL);
	KNH_INITv(b->tag2urnDictMap, KNH_NULL);
	KNH_INITv(b->func2cidDictSet, KNH_NULL);
	KNH_INITv(b->pathTrustDictSet, KNH_NULL);
}

static FASTAPI(void) knh_NameSpace_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
	knh_NameSpace_struct *b = DP((knh_NameSpace_t*)o);
	ftr(ctx, UP(b->nsname));
	ftr(ctx, UP(b->script));
	ftr(ctx, UP(b->importedNameSpaces));
	ftr(ctx, UP(b->lconstDictMap));
	ftr(ctx, UP(b->name2cidDictSet));
	ftr(ctx, UP(b->tag2urnDictMap));
	ftr(ctx, UP(b->func2cidDictSet));
	ftr(ctx, UP(b->pathTrustDictSet));
}

static FASTAPI(void*) knh_NameSpace_hashkey(Ctx *ctx, knh_sfp_t *sfp, int opt)
{
	if(opt == KNH_FOBJECT_KEY) {
		return (void*)DP(sfp[0].ns)->nsname;
	}
	return DEFAULT_hashkey(ctx, sfp, opt);
}

static knh_ObjectCSPI_t NameSpaceSPI = {
	"NameSpace", sizeof(knh_NameSpace_struct),
	knh_NameSpace_init,
	DEFAULT_copy,
	knh_NameSpace_traverse,
	DEFAULT_compareTo,
	knh_NameSpace_hashkey,
	DEFAULT_genmap,
};

/* ======================================================================== */
/* System */

static knh_uintptr_t knh_autoSystemId = 0;

static FASTAPI(void) knh_System_init(Ctx *ctx, Object *o, int init)
{
	knh_System_struct *sys = DP((knh_System_t*)o);
	sys->sysid = knh_autoSystemId++;
	sys->ctxcount = 0;

	KNH_INITv(sys->ClassNameDictSet, new_DictSet(ctx, 0));
	KNH_INITv(sys->ExptNameDictSet, new_DictSet(ctx, 0));

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
	KNH_INITv(sys->MethodFieldHashMap, new_HashMap(ctx, "System.MethodField", 0));
	KNH_INITv(sys->DriversTableDictSet, new_DictSet(ctx, 32));
	KNH_INITv(sys->SpecFuncDictSet, new_DictSet(ctx, 32));

	KNH_INITv(sys->NameSpaceTableDictMap, new_DictMap0(ctx, 8));
	KNH_INITv(sys->URNAliasDictMap, new_DictMap0(ctx, 8));
	KNH_INITv(sys->UsingResources, new_Array0(ctx, 0));

	KNH_INITv(sys->listenerDictMap, new_DictMap0(ctx, 0));
	KNH_INITv(sys->trustedPathDictSet, new_DictSet(ctx, 0));
}

static FASTAPI(void) knh_System_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
	knh_System_struct *sys = DP((knh_System_t*)o);

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

static knh_ObjectCSPI_t SystemSPI = {
	"System", sizeof(knh_System_struct),
	knh_System_init,
	DEFAULT_copy,
	knh_System_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap,
};

/* ======================================================================== */
/* Context */

static FASTAPI(void) knh_Context_init(Ctx *ctx, Object *o, int init)
{
	/* Context can be only instatiated in new_Context() */
	KNH_THROW__T(ctx, "Abort!!");
}

static knh_ObjectCSPI_t ContextSPI = {
	"Context", 0,
	knh_Context_init,
	DEFAULT_copy,
	DEFAULT_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap,
};

/* ======================================================================== */
/* Thread */

static FASTAPI(void) knh_Thread_init(Ctx *ctx, Object *o, int init)
{
}

static FASTAPI(void) knh_Thread_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
}

static knh_ObjectCSPI_t ThreadSPI = {
	"Thread", 0,
	knh_Thread_init,
	DEFAULT_copy,
	knh_Thread_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap,
};

/* ======================================================================== */
/* ScriptEngine */

static FASTAPI(void) knh_ScriptEngine_init(Ctx *ctx, Object *o, int init)
{
}

static FASTAPI(void) knh_ScriptEngine_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
}

static knh_ObjectCSPI_t ScriptEngineSPI = {
	"ScriptEngine", 0,
	knh_ScriptEngine_init,
	DEFAULT_copy,
	knh_ScriptEngine_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap,
};

/* ======================================================================== */
/* Token */

static FASTAPI(void) knh_Token_init(Ctx *ctx, Object *o, int init)
{
	knh_Token_struct *b = DP((knh_Token_t*)o);
	b->type    =  TYPE_var;
	b->tt_next =  TT_EOT;
	KNH_INITv(b->data, KNH_NULL);
}

static FASTAPI(void) knh_Token_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
	knh_Token_struct *b = DP((knh_Token_t*)o);
	ftr(ctx, b->data);
}

static knh_ObjectCSPI_t TokenSPI = {
	"Token", sizeof(knh_Token_struct),
	knh_Token_init,
	DEFAULT_copy,
	knh_Token_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap,
};

/* ======================================================================== */
/* Stmt */

static FASTAPI(void) knh_Stmt_init(Ctx *ctx, Object *o, int init)
{
	knh_Stmt_struct *b = DP((knh_Stmt_t*)o);
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

static FASTAPI(void) knh_Stmt_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
	knh_Stmt_struct *b = DP((knh_Stmt_t*)o);
	if(b->terms != NULL) {
		knh_Stmt_terms_traverse(ctx, b, ftr);
	}
	ftr(ctx, UP(b->metaDictMap));
	ftr(ctx, UP(b->next));
}

static knh_ObjectCSPI_t StmtSPI = {
	"Stmt", sizeof(knh_Stmt_struct),
	knh_Stmt_init,
	DEFAULT_copy,
	knh_Stmt_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap,
};

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

static FASTAPI(void) knh_Gamma_init(Ctx *ctx, Object *o, int init)
{
	knh_intptr_t i;
	knh_Gamma_struct *b = DP((knh_Gamma_t*)o);

	knh_bzero(b, sizeof(*b));
	b->flag = 0;
	b->this_cid = CLASS_Object;
	b->goffset = 0;
	KNH_INITv(b->ns, ctx->share->mainns);
	DBG2_ASSERT(IS_NameSpace(b->ns));
	KNH_INITv(b->mtd,   KNH_NULL);

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
	b->symbolDictMap = NULL;
	b->constPools = NULL;
//	KNH_INITv(b->symbolDictMap, new_DictMap0(ctx, 256));
//	KNH_INITv(b->constPools, KNH_NULL);

	b->statStmt = 1;
	b->statError = 0;
	b->statWarning = 0;
	b->statBadManner = 0;
	b->statKonohaStyle = 0;
}

static FASTAPI(void) knh_Gamma_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
	size_t i;
	knh_Gamma_struct *b = DP((knh_Gamma_t*)o);
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
	if(b->symbolDictMap != NULL) {
		ftr(ctx, UP(b->symbolDictMap));
	}
	if(b->constPools != NULL) {
		ftr(ctx, UP(b->constPools));
	}
}

static knh_ObjectCSPI_t GammaSPI = {
	"Gamma", sizeof(knh_Gamma_struct),
	knh_Gamma_init,
	DEFAULT_copy,
	knh_Gamma_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap,
};

/* ======================================================================== */
/* KLRInst */

static FASTAPI(void) knh_KLRInst_init(Ctx *ctx, Object *o, int opcode)
{
	knh_KLRInst_t *inst = (knh_KLRInst_t*)o;
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

static FASTAPI(void) knh_KLRInst_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
	knh_KLRInst_t *inst = (knh_KLRInst_t*)o;
	if(inst->opcode <= OPCODE_NOP) {
		knh_opcode_traverse(ctx, inst->op, ftr);
		if(IS_SWEEP(ftr)) {
			KNH_FREE(ctx, inst->op, knh_opcode_size(inst->opcode));
			inst->op = NULL;
		}
	}
}

static knh_ObjectCSPI_t KLRInstSPI = {
	"KLRInst", 0,
	knh_KLRInst_init,
	DEFAULT_copy,
	knh_KLRInst_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap,
};

/* ======================================================================== */
/* KLRCode */

static FASTAPI(void) knh_KLRCode_init(Ctx *ctx, Object *o, int init)
{
	knh_KLRCode_struct *b = DP((knh_KLRCode_t*)o);
	b->size = 0;
	b->code = (knh_code_t*)"";
	b->uri = 0;
	b->dwarf_size  = 0;
	b->dwarf2_size = 0;
}

static FASTAPI(void) knh_KLRCode_traverse(Ctx *ctx, Object *o, knh_ftraverse ftr)
{
	knh_KLRCode_struct *b = DP((knh_KLRCode_t*)o);
	knh_code_t *pc = b->code;
	while(KNH_OPCODE(pc) != OPCODE_RET) {
		knh_opcode_traverse(ctx, (knh_inst_t*)pc, ftr);
		pc += knh_opcode_size(KNH_OPCODE(pc));
	}
	if(IS_SWEEP(ftr)) {
		KNH_FREE(ctx, b->code, KNH_SIZE(b->size));
	}
}

static knh_ObjectCSPI_t KLRCodeSPI = {
	"KLRCode", sizeof(knh_KLRCode_struct),
	knh_KLRCode_init,
	DEFAULT_copy,
	knh_KLRCode_traverse,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap,
};

/* ======================================================================== */
/* @data*/

typedef struct {
	char                  *name;
	int                    index;
} knh_StringData0_t ;

typedef struct {
	knh_ObjectCSPI_t *cspi;
	knh_class_t cid;
	knh_flag_t flag;
} knh_StructData0_t ;

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
} knh_ClassData0_t ;

typedef struct {
	char *name;
	knh_flag_t flag;
	knh_expt_t eid;
	knh_expt_t peid;
} knh_ExptData0_t ;

typedef struct {
	char *name;
	knh_fieldn_t fn;
} knh_FieldNameData0_t ;

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
} knh_MethodFieldData0_t ;

typedef struct {
	knh_fmethod func;
	knh_flag_t flag;
	knh_class_t cid;
	knh_methodn_t mn;
	knh_ushort_t delta;
	int mfindex;
	void *optfunc;
} knh_MethodData0_t ;

typedef struct {
	knh_fmapper func;
	knh_flag_t flag;
	knh_class_t scid;
	knh_class_t tcid;
} knh_MapperData0_t ;

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

char *knh_getStructTableName(Ctx *ctx, knh_class_t bcid)
{
	if(bcid < ctx->share->ClassTableSize) {
		return ClassTable(bcid).cspi->name;
	}
	DBG2_P("unknown bcid=%d", bcid);
	return "STRUCT_unknown";
}
/* ------------------------------------------------------------------------ */

static void knh_loadClassData0(Ctx *ctx, knh_ClassData0_t *data)
{
	while(data->name != NULL) {
		knh_class_t cid = knh_ClassTable_newId(ctx);
		DBG2_ASSERT(cid == data->cid);
		if(data->bcid == CLASS_Closure && data->cid != data->bcid) {
			knh_addClosureClass(ctx, data->cid, T__(data->name), data->r0, data->p1, data->p2, data->p3);
		}
		else if(data->cid != data->bcid && knh_class_isGenerics(data->bcid)) {
			knh_addGenericsClass(ctx, data->cid, T__(data->name), data->bcid, CLASS_type(data->p1), CLASS_type(data->p2));
		}
		else {
			knh_ClassTable_t *t = pClassTable(cid);
			knh_setClassName(ctx, cid, T__(data->name));
			t->cflag  = data->flag;
			t->oflag  = knh_flag_oflag(data->flag);
			t->bcid   = data->bcid;
			if(cid != data->bcid) {
				t->cspi = ClassTable(data->bcid).cspi;
				t->size =  ClassTable(data->bcid).cspi->size;
			}
			t->supcid = data->supcid;
			if(data->bcid == CLASS_Closure) {
				t->p1 = data->p1; t->p2 = data->p2;
				t->r0 = data->r0; t->p3 = data->p3;
			}
			else {
				t->p1 = CLASS_type(data->p1);
				t->p2 = CLASS_type(data->p2);
			}
			t->offset = 0;
			t->bsize  = t->size / sizeof(knh_Object_t*);
			DBG2_ASSERT(t->cstruct == NULL);
			if(data->method_size + data->formatter_size > 0) {
				KNH_INITv(t->cstruct, new_ClassField0(ctx, 0, data->method_size + data->formatter_size));
			}
			else {
				KNH_INITv(t->cstruct, ClassTable(data->supcid).cstruct);
			}
			if(t->cmap == NULL) {
				KNH_INITv(t->cmap, knh_ClassMap_fdefault(ctx, CLASS_ClassMap));
			}
		}
		data++;
	}
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

static Object *knh_Context_fdefault(Ctx *ctx, knh_class_t cid)
{
	return (Object*)ctx;
}

static Object *knh_ExceptionHandler_fdefault(Ctx *ctx, knh_class_t cid)
{
	return (Object*)new_Object_bcid(ctx, CLASS_ExceptionHandler, 0);
}

static Object *knh_NameSpace_fdefault(Ctx *ctx, knh_class_t cid)
{
	return UP(ctx->share->mainns);
}

static Object *knh_System_fdefault(Ctx *ctx, knh_class_t cid)
{
	return (Object*)ctx->sys;
}

static void knh_setDefaultValues(Ctx *ctx)
{
	knh_setClassDefaultValue(ctx, CLASS_Boolean, UP(KNH_FALSE), NULL);
	knh_setClassDefaultValue(ctx, CLASS_Class, UP(new_Type(ctx, CLASS_Object)), NULL);
	{
		knh_Semantics_t *u = (knh_Semantics_t*)new_Object_bcid(ctx, CLASS_Semantics, 0);
		KNH_INITv(DP(u)->ivalue, KNH_INT0);
		KNH_INITv(DP(u)->fvalue, KNH_FLOAT0);
		KNH_INITv(DP(u)->svalue, TS_EMPTY);
		knh_setClassDefaultValue(ctx, CLASS_Semantics, UP(u), NULL);
		knh_setClassDefaultValue(ctx, CLASS_Int,    UP(u), knh_Int_fdefault);
		knh_setClassDefaultValue(ctx, CLASS_Float,  UP(u), knh_Float_fdefault);
		knh_setClassDefaultValue(ctx, CLASS_String, UP(u), knh_String_fdefault);
	}

	// load file/socket/regex/db drivers
	knh_loadDefaultIODriver(ctx);
	knh_loadDefaultSocketDriver(ctx);
	knh_loadDefaultRegexDriver(ctx);
	knh_loadDefaultDBDriver(ctx);

	knh_setClassDefaultValue(ctx, CLASS_Context, KNH_NULL, knh_Context_fdefault);
	knh_setClassDefaultValue(ctx, CLASS_NameSpace, KNH_NULL, knh_NameSpace_fdefault);
	knh_setClassDefaultValue(ctx, CLASS_System, KNH_NULL, knh_System_fdefault);
	knh_setClassDefaultValue(ctx, CLASS_ExceptionHandler, KNH_NULL, knh_ExceptionHandler_fdefault);
}

static void knh_loadExptData0(Ctx *ctx, knh_ExptData0_t *data)
{
	while(data->name != NULL) {
		knh_addException(ctx, data->flag, data->eid, T__(data->name), data->peid);
		data++;
	}
}

static void knh_loadFieldNameData0(Ctx *ctx, knh_FieldNameData0_t *data)
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

static
void knh_loadMethodFieldData0(Ctx *ctx, knh_MethodFieldData0_t *data, knh_MethodField_t **pools)
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

static
void knh_loadMethodData0(Ctx *ctx, knh_MethodData0_t *data, knh_MethodField_t **pools)
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

static void knh_loadMapperData0(Ctx *ctx, knh_MapperData0_t *data)
{
	while(data->func != NULL) {
		knh_addMapperFunc(ctx, data->flag, data->scid, data->tcid, data->func, KNH_NULL);
		data++;
	}
}

static void knh_loadStringPropertyData0(Ctx *ctx, knh_StringData_t *data)
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

static knh_IntData_t IntConstData0[] = {
	{"Int.MAX", KNH_INT_MAX},
	{"Int.MIN", KNH_INT_MIN},
	{"Method.NOTRACE", KNH_NOTRACE},
	{"Method.SECURITY", KNH_SECURITYTRACE},
	{"Method.AUDIT", KNH_AUDITTRACE},
	{"Method.PROFILE", KNH_PROFILER},
	{"Method.STACK", KNH_STACKTRACE},
	{"Method.FUNCTION", KNH_FUNCTIONTRACE},
	{NULL, 0}
};

static knh_FloatData_t FloatConstData0[] = {
	{"Float.MAX", KNH_FLOAT_MAX},
	{"Float.MIN", KNH_FLOAT_MIN},
	{NULL, KNH_FLOAT_ZERO}
};

static knh_StringData_t StringConstData0[] = {
	{NULL, NULL}
};

static knh_StringData_t StringPropertyData0[] = {
	{"konoha.version", KONOHA_VERSION},
	{"konoha.os", KONOHA_OS},
	{"konoha.platform", KONOHA_PLATFORM},
	{NULL, NULL}
};

/* ------------------------------------------------------------------------ */

void knh_loadSystemStructData(Ctx *ctx)
{
	knh_StructData0_t *data = StructData0;
	while(data->cspi != NULL) {
		knh_ClassTable_t *t = pClassTable(data->cid);
		t->cspi = data->cspi;
		t->cflag = data->flag;
		t->oflag = knh_flag_oflag(data->flag);
		t->size = data->cspi->size;
		data++;
	}
}

/* ------------------------------------------------------------------------ */

void knh_loadSystemString(Ctx *ctx)
{
	char **data = StringData0;
	size_t i = 0;
	for(i = 0; *data != NULL; i++) {
		DBG2_ASSERT(ctx->share->tString[i] == NULL);
		DBG2_ASSERT(i < (SIZEOF_TSTRING / sizeof(knh_String_t*)));
		KNH_INITv(ctx->share->tString[i], T__(*data));
		data++;
	}
}

/* ------------------------------------------------------------------------ */

void knh_loadSystemData(Ctx *ctx)
{
	knh_loadClassData0(ctx, ClassData0);
	knh_loadIntConst(ctx, IntConstData0);
	knh_loadFloatConst(ctx, FloatConstData0);
	knh_loadStringConst(ctx, StringConstData0);
	knh_loadStringPropertyData0(ctx, StringPropertyData0);
	knh_getResourceId(ctx, STEXT("(eval)"));  // URI_EVAL
	knh_loadExptData0(ctx, ExptData0);
	knh_loadFieldNameData0(ctx, FieldNameData0);
	{
		int i;
		knh_MethodField_t *pools[KNH_TMETHODFIELD_SIZE];
		knh_loadMethodFieldData0(ctx, MethodFieldData0, pools);
		knh_loadMethodData0(ctx, MethodData0, pools);
		for(i = 0; i < KNH_TMETHODFIELD_SIZE; i++) {
			knh_addMethodFieldTable(ctx, pools[i]);
		}
	}
	knh_loadMapperData0(ctx, MapperData0);
	knh_setDefaultValues(ctx);
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
