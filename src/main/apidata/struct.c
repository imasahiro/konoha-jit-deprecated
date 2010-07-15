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
#define USE_bytes_first       1
#define USE_bytes_index       1
#define USE_bytes_last        1
#define USE_bytes_index       1
#define USE_bytes_rindex      1
#define USE_bytes_indexOf     1
#define USE_bytes_strcmp      1
#define USE_bytes_startsWith  1
#define USE_bytes_endsWith    1
#define USE_bytes_parseint    1
//#define USE_bytes_subbytes    1

#define USE_cwb_open      1
#define USE_cwb_openinit  1
#define USE_cwb_tobytes   1
#define USE_cwb_write     1
#define USE_cwb_putc      1
#define USE_cwb_size      1

#define USE_array_index   1

#define K_PERROR_LIBNAME   "stdc"
#define USE_fopen   1
#define USE_fclose  1
#define USE_fread   1
#define USE_fwrite  1
#define USE_fflush  1
#define USE_fclose  1

#define USE_TIME_H
#define USE_getTimeMilliSecond 1

#include"commons.h"
#include"../../../include/konoha/konoha_code_.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ======================================================================== */
/* Data */
/* ======================================================================== */

#define K_USING_DEFAULTAPI
#include"dspi.c"

static TCAST Array_Iterator(Ctx *ctx, knh_sfp_t *sfp, long rix);
static TCAST Iterator_Array(Ctx *ctx, knh_sfp_t *sfp, long rix);

#define KNH_MALLOCBODY(ctx, C)   (knh_##C##EX_t*)KNH_MALLOC(ctx, sizeof(knh_##C##EX_t))
#define KNH_FREEBODY(ctx, p, C)  KNH_FREE(ctx, p, sizeof(knh_##C##EX_t))

/* ------------------------------------------------------------------------ */
/* DEFAULT */

KNHFASTAPI(void) DEFAULT_init(Ctx *ctx, Object *o)
{
	DBG_ASSERT((sizeof(knh_Object_t) - sizeof(knh_hObject_t)) == sizeof(knh_intptr_t) * 4);
	knh_intptr_t *p = (knh_intptr_t*)&(o->ref);
	p[0] = K_INT0; p[1] = K_INT0; p[2] = K_INT0; p[3] = K_INT0;
}

/* ------------------------------------------------------------------------ */

KNHFASTAPI(void) ABSTRACT_init(Ctx *ctx, Object *o)
{
	DBG_P("ABSTRACT CLASS? %s", O__(o));
	DBG_ASSERT(knh_Object_isNullObject(o));
}

/* ------------------------------------------------------------------------ */

KNHFASTAPI(void) DEFAULT_initcopy(Ctx *ctx, Object *dst, Object *src)
{
	KNH_TODO("copy operation");
}

/* ------------------------------------------------------------------------ */

KNHFASTAPI(void) DEFAULT_traverse(Ctx *ctx, Object *o, knh_Ftraverse ftr)
{
}

/* ------------------------------------------------------------------------ */

KNHFASTAPI(void) DEFAULT_free(Ctx *ctx, Object *o)
{
	knh_ClassTBL_t *t = pClassTBL(ctx, o->h.cid);
	if(t->size > 0) {
		DBG_P("FREE? %s", O__(o));
		KNH_FREE(ctx, o->ref, t->size);
	}
}

/* ------------------------------------------------------------------------ */

KNHFASTAPI(void) DEFAULT_checkout(Ctx *ctx, Object *o, int init)
{
}

/* ------------------------------------------------------------------------ */

KNHAPI(int) DEFAULT_compareTo(Ctx *ctx, Object *o1, Object *o2)
{
	return (int)((knh_intptr_t)o1 - (knh_intptr_t)o2);
}

/* ------------------------------------------------------------------------ */

KNHFASTAPI(void*) DEFAULT_hashkey(Ctx *ctx, knh_sfp_t *sfp, int opt)
{
	if(opt == KNH_FOBJECT_KEY) {
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		knh_write_sname(ctx, cwb->w, knh_Object_cid(sfp[0].o));
		knh_putc(ctx, cwb->w, ':');
		knh_write__p(ctx, cwb->w, sfp[0].o);
		return (void*)knh_cwb_newString(ctx, cwb);
	}
	else {
		knh_hashcode_t h = ((knh_hashcode_t)sfp[0].o) / sizeof(Object*);
		return (void*)h;
	}
}

/* ------------------------------------------------------------------------ */

KNHAPI(knh_Translator_t*) DEFAULT_genmap(Ctx *ctx, knh_class_t scid, knh_class_t tcid)
{
	//DBG_P("set default value cid=%d", cid);
	return NULL;
}

/* ======================================================================== */
/* Object */

static void knh_ClassField_initField(Ctx *ctx, knh_ClassTBL_t *t, knh_class_t self_cid, Object **v)
{
	size_t i;
	knh_fields_t *cf = t->fields;
	for(i = 0; i < t->fsize; i++) {
		knh_type_t type = cf[i].type;
		knh_String_t *value = (knh_String_t*)cf[i].value;
		if(FLAG_is(cf[i].flag, FLAG_Field_Property)) {
			value = knh_getPropertyNULL(ctx, S_tobytes(value));
			DBG_ASSERT(value != NULL);
			DBG_P("type=%s object=%s", TYPE__(cf[i].type), O__(value));
		}
		if(type == TYPE_void) {
			continue;
		}
		else if(IS_Tint(type)) {
			knh_int_t *data = (knh_int_t*)(v + i);
			data[0] = value == NULL ? 0 : N_toint(value);
			continue;
		}
		else if(IS_Tfloat(type)) {
			knh_float_t *data = (knh_float_t*)(v + i);
			data[0] = value == NULL ? K_FLOAT_ZERO : N_tofloat(value);
			continue;
		}
		else if(IS_Tbool(type)) {
			knh_bool_t *data = (knh_bool_t*)(v + i);
			data[0] = value == NULL ? 0 : N_tobool(value);
			continue;
		}
		else if(value == NULL) {
			knh_class_t cid = knh_type_tocid(ctx, type, self_cid);
			KNH_INITv(v[i], KNH_NULVAL(cid));
		}
		else {
			KNH_INITv(v[i], value);
		}
	}
}

static FASTAPI(void) knh_ObjectField_init(Ctx *ctx, Object *o)
{
	knh_ObjectField_t *of = (knh_ObjectField_t*)o;
	knh_class_t cid = of->h.cid;
	size_t size = ClassTBL(cid).size;
	if(size > 0) {
		Object **v = (Object**)KNH_MALLOC(ctx, size);
		size_t offset;
		while((offset = ClassTBL(cid).offset) != 0) {
			knh_ClassField_initField(ctx, pClassTBL(ctx, cid), of->h.cid, v + offset);
			cid = ClassTBL(cid).supcid;
			DBG_ASSERT_cid(cid);
		}
		knh_ClassField_initField(ctx, pClassTBL(ctx, cid), of->h.cid, v + offset);
		of->fields = v;
		of->bsize = size / sizeof(Object*);
	}
	else {
		of->fields = NULL;
		of->bsize = 0;
	}
}

static FASTAPI(void) knh_ObjectField_traverse(Ctx *ctx, Object *o, knh_Ftraverse ftr)
{
	knh_ObjectField_t *of = (knh_ObjectField_t*)o;
	knh_class_t cid = knh_Object_cid(of);
	while(cid != CLASS_Object) {
		knh_ClassTBL_t *t = pClassTBL(ctx, cid);
		size_t i, offset = ClassTBL(cid).offset;
		for(i = 0; i < t->fsize; i++) {
			knh_type_t type = t->fields[i].type;
			//DBG_P("i=%d, fn=%s, type=%s%s", i, FN__(cs->fields[i].fn), TYPEQN(type));
			if(IS_Tunbox(type) || type == TYPE_void) {
				continue;
			}
			if(of->fields[i + offset] == NULL) return; /* for Script */
			if(t->fields[i].fn == FN_/*register*/) continue;
			KNH_FTR(ctx, ftr, of->fields[i + offset]);
		}
		cid = ClassTBL(cid).supcid;
	}
}

static FASTAPI(void) knh_ObjectField_free(Ctx *ctx, Object *o)
{
	knh_ObjectField_t *of = (knh_ObjectField_t*)o;
	if(of->bsize > 0) {
		KNH_FREE(ctx, of->fields, of->bsize * sizeof(knh_Object_t*));
	}
}

static int knh_ObjectField_compareTo(Ctx *ctx, Object *o, Object *o2)
{
	return o - o2;
}

static FASTAPI(void*) knh_ObjectField_hashkey(Ctx *ctx, knh_sfp_t *sfp, int opt)
{
	if(opt == KNH_FOBJECT_KEY) {
		int keyidx = ClassTBL(knh_Object_cid(sfp[0].o)).keyidx;
		if(keyidx != -1) {
			knh_ObjectField_t *of = (knh_ObjectField_t*)sfp[0].o;
			DBG_ASSERT(IS_bString(of->fields[keyidx]));
			return (void*)of->fields[keyidx];
		}
	}
	return DEFAULT_hashkey(ctx, sfp, opt);
}


static knh_ObjectCSPI_t ObjectSPI = {
	"Object", 0, CFLAG_Object,
	knh_ObjectField_init, DEFAULT_initcopy,
	knh_ObjectField_traverse, knh_ObjectField_free,
	DEFAULT_checkout, knh_ObjectField_compareTo,
	knh_ObjectField_hashkey, DEFAULT_genmap,
};

/* ======================================================================== */
/* Any */

static FASTAPI(void) knh_Any_init(Ctx *ctx, Object *o)
{
	knh_RawPtr_t *g = (knh_RawPtr_t *)o;
	ctx->api->RawPtr_init(ctx, g, NULL, NULL);
}

static FASTAPI(void) knh_Any_free(Ctx *ctx, Object *o)
{
	knh_RawPtr_t *g = (knh_RawPtr_t *)o;
	if((Object*)g == KNH_NULL) {
		DBG_P("freeing null");
	}
	else {
		g->pfree(ctx, g);
	}
}

static knh_ObjectCSPI_t AnySPI = {
	"Any", 0, CFLAG_Any,
	knh_Any_init, DEFAULT_initcopy,
	DEFAULT_traverse, knh_Any_free,
	DEFAULT_checkout, DEFAULT_compareTo,
	DEFAULT_hashkey, DEFAULT_genmap,
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
	"Boolean", 0, CFLAG_Boolean,
	DEFAULT_init, DEFAULT_initcopy,
	DEFAULT_traverse,  DEFAULT_free,
	DEFAULT_checkout, DEFAULT_compareTo,
	knh_Boolean_hashkey, DEFAULT_genmap,
};

/* ======================================================================== */
/* Number */

static knh_ObjectCSPI_t NumberSPI = {
	"Number", 0, CFLAG_Number,
	ABSTRACT_init, DEFAULT_initcopy,
	DEFAULT_traverse, DEFAULT_free,
	DEFAULT_checkout, DEFAULT_compareTo,
	DEFAULT_hashkey, DEFAULT_genmap,
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
#ifdef K_USING_INT32
		knh_snprintf(buf, sizeof(buf), "%016lu", lsfp[0].uvalue);
#else
		//18446744073709551615ULL
		knh_snprintf(buf, sizeof(buf), "%020llu", lsfp[0].uvalue);
#endif
		return (void*)new_S(ctx, B(buf));
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
	{
		"Int", 0, CFLAG_Int,
		DEFAULT_init, DEFAULT_initcopy,
		DEFAULT_traverse,  DEFAULT_free,
		DEFAULT_checkout, knh_Int_compareTo,
		knh_Int_hashkey, DEFAULT_genmap
	},
	K_NUMBERCSPI_MAGIC,
	knh_Int_toint, knh_Int_tofloat,
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
	{
		"Float", 0, CFLAG_Float,
		DEFAULT_init, DEFAULT_initcopy,
		DEFAULT_traverse,  DEFAULT_free,
		DEFAULT_checkout,knh_Float_compareTo,
		knh_Int_hashkey,DEFAULT_genmap
	},
	K_NUMBERCSPI_MAGIC,
	knh_Float_toint, knh_Float_tofloat,
};

/* ======================================================================== */
/* String */

static FASTAPI(void) knh_String_init(Ctx *ctx, Object *o)
{
	knh_String_t *s = (knh_String_t*)o;
	s->str.text = "";
	s->str.len = 0;
	s->memoNULL = NULL;
	knh_String_setTextSgm(s, 1);
}

static FASTAPI(void) knh_String_traverse(Ctx *ctx, knh_Object_t *o, knh_Ftraverse ftr)
{
	knh_String_t *s = (knh_String_t*)o;
	if(!knh_String_isTextSgm(s)) {
		KNH_NULLFTR(ctx, ftr, s->memoNULL);
	}
}

static FASTAPI(void) knh_String_free(Ctx *ctx, knh_Object_t *o)
{
	knh_String_t *s = (knh_String_t*)o;
	if(!s->memoNULL && !knh_String_isTextSgm(s)) {
		KNH_FREE(ctx, s->str.ubuf, KNH_SIZE(S_size(s) + 1));
	}
}

static int knh_String_compareTo(Ctx *ctx, knh_Object_t *o, knh_Object_t *o2)
{
	knh_String_t *s1 = (knh_String_t*)o;
	knh_String_t *s2 = (knh_String_t*)o2;
#if defined(K_USING_SEMANTICS)
	if(s1->h.cid == CLASS_String || s2->h.cid == CLASS_String) {
		size_t max = KNH_MAX(S_size(s1), S_size(s2));
		return knh_strncmp(S_tochar(s1) ,S_tochar(s2), max);
	}
	else {
		if(s1->h.cid == s2->h.cid) {
			knh_Semantics_t *u = knh_getSemantics(ctx, s1->h.cid);
			return DP(u)->fscmp(u, S_tobytes(s1), S_tobytes(s2));
		}
		return (int)(s1 - s2);
	}
#else
	size_t max = KNH_MAX(S_size(s1), S_size(s2));
	return knh_strncmp(S_tochar(s1) ,S_tochar(s2), max);
#endif
}

static FASTAPI(void*) knh_String_hashkey(Ctx *ctx, knh_sfp_t *sfp, int opt)
{
	if(opt == KNH_FOBJECT_KEY) {
		return (void*)sfp[0].s;
	}
	else {
		knh_String_t *s = (sfp[0].s);
		knh_hashcode_t h = S_size(s);
		return (void*) knh_ustr_hcode(h, s->str.ustr, S_size(s));
	}
}

static knh_ObjectCSPI_t StringSPI = {
	"String", 0, CFLAG_String,
	knh_String_init, DEFAULT_initcopy,
	knh_String_traverse, knh_String_free,
	DEFAULT_checkout, knh_String_compareTo,
	knh_String_hashkey, DEFAULT_genmap,
};

/* ======================================================================== */
/* Bytes */

static FASTAPI(void) knh_Bytes_init(Ctx *ctx, Object *o)
{
	knh_Bytes_t *ba = (knh_Bytes_t*)o;
	ba->bu.len = 0;
	ba->bu.ubuf = NULL;
	ba->capacity = 0;
}

static FASTAPI(void) knh_Bytes_free(Ctx *ctx, Object *o)
{
	knh_Bytes_t *ba = (knh_Bytes_t*)o;
	if(ba->capacity > 0) {
		KNH_FREE(ctx, ba->bu.ubuf, ba->capacity);
	}
}

static knh_ObjectCSPI_t BytesSPI = {
	"Bytes", 0, CFLAG_Bytes,
	knh_Bytes_init, DEFAULT_initcopy,
	DEFAULT_traverse, knh_Bytes_free,
	DEFAULT_checkout, DEFAULT_compareTo,
	DEFAULT_hashkey, DEFAULT_genmap,
};

/* ======================================================================== */
/* Tuple */

static FASTAPI(void) knh_Tuple_init(Ctx *ctx, Object *o)
{
	KNH_TODO("new tuple");
//	knh_Tuple_t *t = (knh_Tuple_t*)o;
//	t->size = 0; /* @see Tuple_new */
//	t->list = NULL;
}

static FASTAPI(void) knh_Tuple_traverse(Ctx *ctx, Object *o, knh_Ftraverse ftr)
{
//	knh_Tuple_t *t = (knh_Tuple_t*)o;
//	if(knh_Tuple_isTriple(t)) {
//		KNH_FTR(ctx, ftr, t->first);
//		KNH_FTR(ctx, ftr, t->second);
//		KNH_NULLFTR(ctx, ftr, t->thirdNULL);
//	}
//	else if(t->size > 0){
//		size_t i;
//		for(i = 0; i < t->size; i++) {
//			KNH_FTR(ctx, ftr, t->list[i]);
//		}
//		if(IS_SWEEP(ftr)) {
//			KNH_FREE(ctx, t->list, sizeof(void*) * t->size);
//			t->size = 0;
//		}
//	}
}

static knh_ObjectCSPI_t TupleSPI = {
	"Tuple", 0, CFLAG_Tuple,
	knh_Tuple_init, DEFAULT_initcopy,
	knh_Tuple_traverse, DEFAULT_free,
	DEFAULT_checkout, DEFAULT_compareTo,
	DEFAULT_hashkey, DEFAULT_genmap,
};

/* ======================================================================== */
/* Range */

static FASTAPI(void) knh_Range_init(Ctx *ctx, Object *o)
{
	KNH_TODO("new range");
//	knh_Range_t *t = (knh_Range_t*)o;
//	KNH_INITv(t->start, KNH_NULL);
//	KNH_INITv(t->end, KNH_NULL);
}

static FASTAPI(void) knh_Range_traverse(Ctx *ctx, Object *o, knh_Ftraverse ftr)
{
	knh_Range_t *t = (knh_Range_t*)o;
	KNH_FTR(ctx, ftr, t->start);
	KNH_FTR(ctx, ftr, t->end);
}

static TCAST Range_Iterator(Ctx *ctx, knh_sfp_t *sfp, long rix);
#define FLAG_Translator_Iteration (FLAG_Translator_Total)

static knh_Translator_t* knh_Range_genmap(Ctx *ctx, knh_class_t cid, knh_class_t tcid)
{
	if(knh_class_bcid(tcid) == CLASS_Iterator) {
		knh_class_t p1 = knh_class_p1(cid); //Range<p1>
		knh_class_t tp1 = knh_class_p1(tcid);  //Iterator<tp2>
		if(p1 == tp1 || tp1 == CLASS_Any || knh_class_instanceof(ctx, p1, tp1)) {
			return new_Translator(ctx, FLAG_Translator_Iteration, cid, tcid, Range_Iterator);
		}
	}
	return NULL;
}

static knh_ObjectCSPI_t RangeSPI = {
	"Range", 0, CFLAG_Range,
	knh_Range_init, DEFAULT_initcopy,
	knh_Range_traverse, DEFAULT_free,
	DEFAULT_checkout, DEFAULT_compareTo,
	DEFAULT_hashkey, knh_Range_genmap,
};

/* ======================================================================== */
/* Array */

static knh_hmem_t HMEM_ZERO = {0, 0};
#define HMEM_SAFEZERO         (&HMEM_ZERO) + 1

static FASTAPI(void) knh_Array_init(Ctx *ctx, Object *o)
{
	knh_Array_t *a = (knh_Array_t*)o;
	a->hmem = HMEM_SAFEZERO; DBG_ASSERT(a->hmem[-1].capacity == 0);
	a->size = 0;
	knh_Array_initAPI(ctx, a);
}

static FASTAPI(void) knh_Array_traverse(Ctx *ctx, Object *o, knh_Ftraverse ftr)
{
	knh_Array_t *a = (knh_Array_t*)o;
	if(!knh_Array_isNDATA(a)) {
		size_t i;
		for(i = 0; i < a->size; i++) {
			KNH_FTR(ctx, ftr, a->list[i]);
		}
	}
}

static FASTAPI(void) knh_Array_free(Ctx *ctx, Object *o)
{
	knh_Array_t *a = (knh_Array_t*)o;
	if(a->hmem[-1].capacity > 0) {
		knh_hfree(ctx, a->hmem);
	}
}

static knh_Translator_t* knh_Array_genmap(Ctx *ctx, knh_class_t cid, knh_class_t tcid)
{
	if(knh_class_bcid(tcid) == CLASS_Iterator) {
		knh_class_t p1 = knh_class_p1(cid);
		knh_class_t tp1 = knh_class_p1(tcid);
		if(p1 == tp1 || tp1 == CLASS_Any || knh_class_instanceof(ctx, p1, tp1)) {
			return new_Translator(ctx, FLAG_Translator_Iteration, cid, tcid, Array_Iterator);
		}
	}
	return NULL;
}

static knh_ObjectCSPI_t ArraySPI = {
	"Array", 0, CFLAG_Array,
	knh_Array_init, DEFAULT_initcopy,
	knh_Array_traverse, knh_Array_free,
	DEFAULT_checkout, DEFAULT_compareTo,
	DEFAULT_hashkey, knh_Array_genmap,
};

/* ======================================================================== */
/* Iterator */

static ITRNEXT knh_fitrnext_single(Ctx *ctx, knh_sfp_t *sfp, long rtnidx)
{
	knh_Iterator_t *it = sfp[0].it;
	if(DP(it)->pos == 0) {
		DP(it)->pos = 1;
		ITRNEXT_(DP(it)->source);
	}
	ITREND_();
}

static FASTAPI(void) knh_Iterator_init(Ctx *ctx, Object *o)
{
	knh_Iterator_t *it = (knh_Iterator_t*)o;
	knh_IteratorEX_t *b = KNH_MALLOCBODY(ctx, Iterator);
	b->fnext  =  knh_fitrnext_single;
	KNH_INITv(b->source, KNH_NULL);
	b->pos      =  0;
	b->ref      =  NULL;
	b->freffree = NULL;
	it->b = b;
}

static FASTAPI(void) knh_Iterator_traverse(Ctx *ctx, Object *o, knh_Ftraverse ftr)
{
	knh_Iterator_t *it = (knh_Iterator_t*)o;
	knh_Iterator_close(ctx, it);
	KNH_FTR(ctx, ftr, DP(it)->source);
}

static FASTAPI(void) knh_Iterator_free(Ctx *ctx, Object *o)
{
	knh_Iterator_t *it = (knh_Iterator_t*)o;
	knh_Iterator_close(ctx, it);
	KNH_FREEBODY(ctx, it->b, Iterator);
}

static ITRNEXT knh_Iterator_filterNext(Ctx *ctx, knh_sfp_t *sfp, long rtnidx)
{
	knh_Iterator_t *itr2 = (knh_Iterator_t*)DP(sfp[0].it)->source;
	knh_class_t cid = knh_Object_p1(sfp[0].it);
	DBG_ASSERT(IS_bIterator(itr2));
	sfp = sfp + 1;
	rtnidx = rtnidx - 1;
	KNH_SETv(ctx, sfp[0].o, itr2);
	int res = SP(itr2)->fnext_1(ctx, sfp, rtnidx);
	while(res != 0) {
		if(knh_class_instanceof(ctx, knh_Object_cid(sfp[rtnidx].o), cid)) break;
		res = SP(itr2)->fnext_1(ctx, sfp, rtnidx);
	}
	return res;
}

static TCAST Iterator_Iterator(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Translator_t *trl = sfp[K_TRLIDX].casttrl;
	RETURN_(new_Iterator(ctx, knh_class_p1(SP(trl)->tcid), sfp[K_TRLIDX].o, knh_Iterator_filterNext));
}

static knh_Translator_t* knh_Iterator_genmap(Ctx *ctx, knh_class_t cid, knh_class_t tcid)
{
	if(cid == CLASS_Iterator && knh_class_bcid(cid) == CLASS_Iterator) {
		return new_Translator(ctx, FLAG_Translator_Iteration, CLASS_Iterator, cid, Iterator_Iterator);
	}
	if(knh_class_bcid(tcid) == CLASS_Array) {
		knh_class_t p1 = knh_class_p1(cid);
		knh_class_t tp1 = knh_class_p1(tcid);
		if(p1 == tp1 || tp1 == CLASS_Any || knh_class_instanceof(ctx, p1, tp1)) {
			return new_Translator(ctx, FLAG_Translator_Iteration, cid, tcid, Iterator_Array);
		}
	}
	return NULL;
}

static knh_ObjectCSPI_t IteratorSPI = {
	"Iterator", sizeof(knh_IteratorEX_t), CFLAG_Iterator,
	knh_Iterator_init, DEFAULT_initcopy,
	knh_Iterator_traverse, knh_Iterator_free,
	DEFAULT_checkout, DEFAULT_compareTo,
	DEFAULT_hashkey, knh_Iterator_genmap,
};

///* ======================================================================== */
///* DictMap */
//
//static FASTAPI(void) knh_DictMap_init(Ctx *ctx, Object *o)
//{
//	knh_DictMap_t *d = (knh_DictMap_t*)o;
//	d->_list = knh_dictmap_malloc(ctx, 0);
//	d->size = 0;
//	d->fcmp = knh_bytes_strcmp;
//}
//
//static FASTAPI(void) knh_DictMap_traverse(Ctx *ctx, Object *o, knh_Ftraverse ftr)
//{
//	knh_DictMap_t *d = (knh_DictMap_t*)o;
//	knh_dict_traverse(ctx, d->_list, ftr);
//}
//
//static FASTAPI(void) knh_DictMap_free(Ctx *ctx, Object *o)
//{
//	knh_DictMap_t *d = (knh_DictMap_t*)o;
//	knh_dict_free(ctx, d->_list);
//}
//
//static knh_ObjectCSPI_t DictMapSPI = {
//	"DictMap", 0, CFLAG_DictMap,
//	knh_DictMap_init, DEFAULT_initcopy,
//	knh_DictMap_traverse, knh_DictMap_free,
//	DEFAULT_checkout, DEFAULT_compareTo,
//	DEFAULT_hashkey, DEFAULT_genmap /*knh_DictMap_genmap*/,
//};
//
///* ======================================================================== */
///* DictSet */
//
//static FASTAPI(void) knh_DictSet_init(Ctx *ctx, Object *o)
//{
//	knh_DictSet_t *d = (knh_DictSet_t*)o;
//	d->_list = knh_dictset_malloc(ctx, 0);
//	d->size = 0;
//	d->fcmp = knh_bytes_strcmp;
//}
//
//static knh_ObjectCSPI_t DictSetSPI = {
//	"DictSet", 0, CFLAG_DictSet,
//	knh_DictSet_init, DEFAULT_initcopy,
//	knh_DictMap_traverse, knh_DictMap_free,
//	DEFAULT_checkout, DEFAULT_compareTo,
//	DEFAULT_hashkey, DEFAULT_genmap /*knh_DictMap_genmap*/,
//};

/* ======================================================================== */
/* DictSet */

static FASTAPI(void) knh_Map_init(Ctx *ctx, Object *o)
{
	knh_Map_t *m = (knh_Map_t*)o;
	m->dspi = NULL;
	m->map = NULL;
}

static FASTAPI(void) knh_Map_traverse(Ctx *ctx, Object *o, knh_Ftraverse ftr)
{
	knh_Map_t *m = (knh_Map_t*)o;
	m->dspi->ftrmap(ctx, m->map, ftr);
}

static FASTAPI(void) knh_Map_free(Ctx *ctx, Object *o)
{
	knh_Map_t *m = (knh_Map_t*)o;
	m->dspi->freemap(ctx, m->map);
}

static knh_ObjectCSPI_t MapSPI = {
	"Map", 0, CFLAG_Map,
	knh_Map_init, DEFAULT_initcopy,
	knh_Map_traverse, knh_Map_free,
	DEFAULT_checkout, DEFAULT_compareTo,
	DEFAULT_hashkey, DEFAULT_genmap /*knh_DictMap_genmap*/,
};

/* ======================================================================== */
/* Class */

static int knh_Class_compareTo(Ctx *ctx, knh_Object_t *o, knh_Object_t *o2)
{
	knh_Class_t *c = (knh_Class_t*)o;
	knh_Class_t *c2 = (knh_Class_t*)o2;
	return knh_strcmp(CLASS__(c->cid), CLASS__(c2->cid));
}

static FASTAPI(void*) knh_Class_hashkey(Ctx *ctx,knh_sfp_t *sfp, int opt)
{
	knh_Class_t *c = (knh_Class_t*)sfp[0].o;
	knh_class_t cid = c->cid;
	if(opt == KNH_FOBJECT_KEY) {
		DBG_ASSERT_cid(cid);
		return (void*)(ClassTBL(cid).lname);
	}
	else {
		return (void*)cid;
	}
}

static knh_ObjectCSPI_t ClassSPI = {
	"Class", 0, CFLAG_Class,
	DEFAULT_init, DEFAULT_initcopy,
	DEFAULT_traverse, DEFAULT_free,
	DEFAULT_checkout, knh_Class_compareTo,
	knh_Class_hashkey, DEFAULT_genmap /*knh_DictMap_genmap*/,
};

/* ======================================================================== */
/* ParamArray */

static FASTAPI(void) knh_ParamArray_init(Ctx *ctx, Object *o)
{
	knh_ParamArray_t *pa = (knh_ParamArray_t*)o;
	pa->psize = 0;
	pa->rsize = 0;
	pa->params = NULL;
	pa->capacity = 0;
}

static FASTAPI(void) knh_ParamArray_free(Ctx *ctx, Object *o)
{
	knh_ParamArray_t *pa = (knh_ParamArray_t*)o;
	if(pa->psize + pa->rsize > 3) {
		KNH_FREE(ctx, pa->params, pa->capacity * sizeof(knh_param_t));
	}
}

static knh_ObjectCSPI_t ParamArraySPI = {
	"ParamArray", 0, CFLAG_ParamArray,
	knh_ParamArray_init, DEFAULT_initcopy,
	DEFAULT_traverse, knh_ParamArray_free,
	DEFAULT_checkout, DEFAULT_compareTo,
	DEFAULT_hashkey, DEFAULT_genmap,
};

/* ======================================================================== */
/* Method */

static FASTAPI(void) knh_Method_init(Ctx *ctx, Object *o)
{
	knh_Method_t *mtd = (knh_Method_t*)o;
	knh_MethodEX_t *b = KNH_MALLOCBODY(ctx, Method);
	b->flag   = 0;
	b->delta  = 0;
	b->cid    = CLASS_Object;
	b->mn     = MN_NONAME;
	b->fproceed  = NULL;
	KNH_INITv(b->mp, KNH_NULVAL(CLASS_ParamArray));
	b->code  = NULL;
	b->uri  = 0;  b->domain = 0;
	mtd->b = b;
}

static FASTAPI(void) knh_Method_traverse(Ctx *ctx, Object *o, knh_Ftraverse ftr)
{
	knh_Method_t *mtd = (knh_Method_t*)o;
	knh_MethodEX_t *b = DP(mtd);
	KNH_FTR(ctx, ftr, (b->mp));
	if(knh_Method_isObjectCode(mtd)) {
		KNH_FTR(ctx, ftr, (Object*)b->code);
	}
}

static FASTAPI(void) knh_Method_free(Ctx *ctx, Object *o)
{
	KNH_FREEBODY(ctx, o->ref, Method);
}

static FASTAPI(void*) knh_Method_hashkey(Ctx *ctx, knh_sfp_t *sfp, int opt)
{
	if(opt == KNH_FOBJECT_KEY) {
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		knh_write_cid(ctx, cwb->w, DP(sfp[0].mtdOBJ)->cid);
		knh_putc(ctx, cwb->w, '.');
		knh_write_mn(ctx, cwb->w, DP(sfp[0].mtdOBJ)->mn);
		return (void*)knh_cwb_newString(ctx, cwb);
	}
	return DEFAULT_hashkey(ctx, sfp, opt);
}

static knh_ObjectCSPI_t MethodSPI = {
	"Method", sizeof(knh_MethodEX_t), CFLAG_Method,
	knh_Method_init, DEFAULT_initcopy,
	knh_Method_traverse, knh_Method_free,
	DEFAULT_checkout, DEFAULT_compareTo,
	knh_Method_hashkey, DEFAULT_genmap,
};

/* ======================================================================== */
/* Translator */

static FASTAPI(void) knh_Translator_init(Ctx *ctx, Object *o)
{
	knh_Translator_t *trl = (knh_Translator_t*)o;
	knh_TranslatorEX_t *b = KNH_MALLOCBODY(ctx, Translator);
	b->flag = 0;
	SP(trl)->scid = CLASS_Object;
	SP(trl)->tcid = CLASS_Object;
	KNH_INITv(b->mapdata, KNH_NULL);
	KNH_INITv(b->trl2, KNH_NULL);
	trl->b = b;
}

static FASTAPI(void) knh_Translator_traverse(Ctx *ctx, Object *o, knh_Ftraverse ftr)
{
	knh_Translator_t *trl = (knh_Translator_t*)o;
	knh_TranslatorEX_t *b = DP(trl);
	KNH_FTR(ctx, ftr, b->mapdata);
	KNH_FTR(ctx, ftr, (b->trl2));
}

static FASTAPI(void) knh_Translator_free(Ctx *ctx, Object *o)
{
	KNH_FREEBODY(ctx, o->ref, Translator);
}

static FASTAPI(void*) knh_Translator_hashkey(Ctx *ctx, knh_sfp_t *sfp, int opt)
{
	if(opt == KNH_FOBJECT_KEY) {
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		knh_write_cid(ctx, cwb->w, SP(sfp[0].trlOBJ)->scid);
		knh_write(ctx, cwb->w, STEXT("->"));
		knh_write_cid(ctx, cwb->w, SP(sfp[0].trlOBJ)->tcid);
		return (void*)knh_cwb_newString(ctx, cwb);
	}
	return DEFAULT_hashkey(ctx, sfp, opt);
}

static knh_ObjectCSPI_t TranslatorSPI = {
	"Translator", sizeof(knh_TranslatorEX_t), CFLAG_Translator,
	knh_Translator_init, DEFAULT_initcopy,
	knh_Translator_traverse, knh_Translator_free,
	DEFAULT_checkout, DEFAULT_compareTo,
	knh_Translator_hashkey, DEFAULT_genmap,
};

/* ======================================================================== */
/* Func */

static METHOD knh_Fmethod_funcRTYPE(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_type_t rtype = knh_ParamArray_rtype(DP(sfp[K_MTDIDX].callmtd)->mp);
	if(rtype != TYPE_void) {
		if(IS_Tunbox(rtype)) {
			RETURNi_(K_INT0);  // same results in Float, Boolean
		}
		else {
			RETURN_(KNH_NULVAL(CLASS_type(rtype)));
		}
	}
}

static FASTAPI(void) knh_Func_init(Ctx *ctx, Object *o)
{
	knh_Func_t *fo = (knh_Func_t*)o;
	knh_ClassTBL_t *t = pClassTBL(ctx, knh_Object_cid(o));
	knh_Method_t *mtd;
	if(t->defnull == NULL) {
		mtd = new_Method(ctx, 0, knh_Object_cid(o), MN_LAMBDA, knh_Fmethod_funcRTYPE);
		KNH_SETv(ctx, DP(mtd)->mp, t->cparam);
	}
	else {
		mtd = t->deffunc->mtd;
	}
	KNH_INITv(fo->mtd, mtd);
	fo->baseNULL = NULL;
}

static FASTAPI(void) knh_Func_traverse(Ctx *ctx, Object *o, knh_Ftraverse ftr)
{
	knh_Func_t *fo = (knh_Func_t*)o;
	KNH_FTR(ctx, ftr, (fo->mtd));
	KNH_NULLFTR(ctx, ftr, fo->baseNULL);
//		size_t i, stacksize = (fo)->hstacksize[-1];
//		for(i = 0; i < stacksize; i++) {
//			KNH_FTR(ctx, ftr, (fo)->envsfp[i].o);
//		}
}

static FASTAPI(void) knh_Func_free(Ctx *ctx, Object *o)
{
//	knh_Func_t *cc = (knh_Func_t*)o;
//	if(knh_Func_isStoredEnv(cc)) {
//		size_t stacksize = (cc)->hstacksize[-1];
//		KNH_FREE(ctx, (cc)->envsfp, (sizeof(knh_sfp_t) * stacksize) + sizeof(size_t));
//		(cc)->envsfp = NULL;
//		knh_Func_setStoredEnv(cc, 0);
//	}
}

static knh_ObjectCSPI_t FuncSPI = {
	"Func", 0, CFLAG_Func,
	knh_Func_init, DEFAULT_initcopy,
	knh_Func_traverse, knh_Func_free,
	DEFAULT_checkout, DEFAULT_compareTo,
	DEFAULT_hashkey, DEFAULT_genmap,
};

/* ======================================================================== */
/* Thunk */

static FASTAPI(void) knh_Thunk_init(Ctx *ctx, Object *o)
{
	KNH_TODO("Thunk object");
//	knh_Thunk_t *thk = (knh_Thunk_t*)o;
//	thk->envsfp = NULL;
//	thk->envsize = 0;
//	knh_Thunk_setEvaluated(thk, 0);
}

static FASTAPI(void) knh_Thunk_traverse(Ctx *ctx, Object *o, knh_Ftraverse ftr)
{
//	knh_Thunk_t *thk = (knh_Thunk_t*)o;
//	if(thk->envsize > 0) {
//		size_t i;
//		for(i = 0; i < (thk)->envsize; i++) {
//			KNH_FTR(ctx, ftr, (thk)->envsfp[i].o);
//		}
//		if(IS_SWEEP(ftr)) {
//			KNH_FREE(ctx, (thk)->envsfp, (sizeof(knh_sfp_t) * (thk)->envsize));
//			(thk)->envsfp = NULL;
//			(thk)->envsize = 0;
//			knh_Thunk_setEvaluated(thk, 0);
//		}
//	}
}

static FASTAPI(void) knh_Thunk_free(Ctx *ctx, Object *o)
{
//	knh_Thunk_t *thk = (knh_Thunk_t*)o;
//	if(thk->envsize > 0) {
//		size_t i;
//		for(i = 0; i < (thk)->envsize; i++) {
//			KNH_FTR(ctx, ftr, (thk)->envsfp[i].o);
//		}
//		if(IS_SWEEP(ftr)) {
//			KNH_FREE(ctx, (thk)->envsfp, (sizeof(knh_sfp_t) * (thk)->envsize));
//			(thk)->envsfp = NULL;
//			(thk)->envsize = 0;
//			knh_Thunk_setEvaluated(thk, 0);
//		}
//	}
}

static knh_ObjectCSPI_t ThunkSPI = {
	"Thunk", 0, CFLAG_Thunk,
	knh_Thunk_init, DEFAULT_initcopy,
	knh_Thunk_traverse, knh_Thunk_free,
	DEFAULT_checkout, DEFAULT_compareTo,
	DEFAULT_hashkey, DEFAULT_genmap,
};

/* ======================================================================== */
/* Exception */

static FASTAPI(void) knh_Exception_init(Ctx *ctx, Object *o)
{
	knh_ExceptionEX_t *b = KNH_MALLOCBODY(ctx, Exception);
	b->eid  = 1;
	b->flag = 0;
	KNH_INITv(b->msg, TS_EMPTY);
	KNH_INITv(b->bag, KNH_NULL);
	KNH_INITv(b->traces, KNH_NULL);
	b->file = "";
	b->line = 0;
	o->ref = b;
}

static FASTAPI(void) knh_Exception_traverse(Ctx *ctx, Object *o, knh_Ftraverse ftr)
{
	knh_ExceptionEX_t *b = DP((knh_Exception_t*)o);
	KNH_FTR(ctx, ftr, (b->msg));
	KNH_FTR(ctx, ftr, b->bag);
	KNH_FTR(ctx, ftr, (b->traces));
}

static FASTAPI(void) knh_Exception_free(Ctx *ctx, Object *o)
{
	KNH_FREEBODY(ctx, o->ref, Exception);
}

static knh_ObjectCSPI_t ExceptionSPI = {
	"Exception", sizeof(knh_ExceptionEX_t), CFLAG_Exception,
	knh_Exception_init, DEFAULT_initcopy,
	knh_Exception_traverse, knh_Exception_free,
	DEFAULT_checkout, DEFAULT_compareTo,
	DEFAULT_hashkey, DEFAULT_genmap,
};

/* ======================================================================== */
/* ExceptionHandler */

static knh_ObjectCSPI_t ExceptionHandlerSPI = {
	"ExceptionHandler", 0, CFLAG_ExceptionHandler,
	DEFAULT_init, DEFAULT_initcopy,
	DEFAULT_traverse, DEFAULT_free,
	DEFAULT_checkout, DEFAULT_compareTo,
	DEFAULT_hashkey, DEFAULT_genmap,
};

/* ======================================================================== */
/* Regex */

static knh_regex_t* knh_strregex_malloc(Ctx *ctx, knh_String_t *pattern)
{
	return (knh_regex_t*)pattern;
}
static int knh_strregex_parsecflags(Ctx *ctx, const char *opt)
{
	return 0;
}
static int knh_strregex_parseeflags(Ctx *ctx, const char *opt)
{
	return 0;
}
static int knh_strregex_regcomp(Ctx *ctx, knh_regex_t *reg, const char *pattern, int cflags)
{
	return 0;
}
static size_t knh_strregex_regerror(int errcode, knh_regex_t *reg, char *ebuf, size_t ebuf_size)
{
	ebuf[0] = 0;
	return 0;
}
static int knh_strregex_regexec(Ctx *ctx, knh_regex_t *reg, const char *str, size_t nmatch, knh_regmatch_t p[], int flags)
{
	size_t e = 0;
	knh_String_t *ptn = (knh_String_t*)reg;
	const char *po = strstr(str, S_tochar(ptn));
	if(po != NULL) {
		p[e].rm_so = po - str;
		p[e].rm_eo = p[e].rm_so + knh_strlen(str);
		p[e].rm_name.ubuf = NULL;
		p[e].rm_name.len = 0;
		e++;
	}
	DBG_ASSERT(e < nmatch);
	p[e].rm_so = -1;
	p[e].rm_eo = -1;
	p[e].rm_name.ubuf = NULL;
	p[e].rm_name.len = 0;
	return 0;
}

static void knh_strregex_regfree(Ctx *ctx, knh_regex_t *reg)
{
}

static knh_RegexSPI_t STRREGEXSPI = {
	"strregex",
	knh_strregex_malloc,
	knh_strregex_parsecflags,
	knh_strregex_parseeflags,
	knh_strregex_regcomp,
	knh_strregex_regexec,
	knh_strregex_regerror,
	knh_strregex_regfree
};

static FASTAPI(void) knh_Regex_init(Ctx *ctx, Object *o)
{
	knh_Regex_t *re = (knh_Regex_t*)o;
	KNH_INITv(re->pattern, TS_EMPTY);
	re->spi = &STRREGEXSPI;
	re->reg = NULL;
}

static FASTAPI(void) knh_Regex_traverse(Ctx *ctx, Object *o, knh_Ftraverse ftr)
{
	knh_Regex_t *re = (knh_Regex_t*)o;
	KNH_FTR(ctx, ftr, (re->pattern));
}

static FASTAPI(void) knh_Regex_free(Ctx *ctx, Object *o)
{
	knh_Regex_t *re = (knh_Regex_t*)o;
	if(re->reg != NULL) {
		re->spi->regfree(ctx, re->reg);
		re->spi = NULL;
		re->reg = NULL;
	}
}

static knh_ObjectCSPI_t RegexSPI = {
	"Regex", 0, CFLAG_Regex,
	knh_Regex_init, DEFAULT_initcopy,
	knh_Regex_traverse, knh_Regex_free,
	DEFAULT_checkout, DEFAULT_compareTo,
	DEFAULT_hashkey, DEFAULT_genmap,
};

/* ------------------------------------------------------------------------ */

knh_bool_t knh_Regex_isSTRREGEX(knh_Regex_t *re)
{
	return (re->spi == &STRREGEXSPI);
}

/* ======================================================================== */
/* StringEncoder */

static knh_conv_t *knh_open_NOCONV(Ctx *ctx, const char*f, const char *t)
{
	return NULL;
}
static knh_bool_t knh_conv_NOCONV(Ctx *ctx, knh_conv_t *c, knh_bytes_t t, knh_Bytes_t *tobuf)
{
	knh_Bytes_write(ctx, tobuf, t);
	return 1;
}
static void knh_conv_NOSET(Ctx *ctx, knh_conv_t *c, void *k, void *v)
{
}
static void knh_close_NOCONV(Ctx *ctx, knh_conv_t *c)
{
}

static knh_ConverterDSPI_t NOCONV_DSPI = {
	K_CONVTO_DSPI, "NOP",
	knh_open_NOCONV,
	knh_conv_NOCONV,
	knh_conv_NOCONV,
	knh_conv_NOCONV,
	knh_conv_NOCONV,
	knh_close_NOCONV,
	knh_conv_NOSET,
};

static FASTAPI(void) knh_Converter_init(Ctx *ctx, Object *o)
{
	knh_StringEncoder_t *bc = (knh_StringEncoder_t*)o;
	bc->conv = NULL;
	bc->dspi = &NOCONV_DSPI;
}

static FASTAPI(void) knh_Converter_free(Ctx *ctx, Object *o)
{
	knh_StringEncoder_t *bc = (knh_StringEncoder_t*)o;
	if(bc->conv != NULL) {
		bc->dspi->close(ctx, bc->conv);
		bc->conv = NULL;
		bc->dspi = &NOCONV_DSPI;
	}
}

static knh_ObjectCSPI_t ConverterSPI = {
	"Converter", 0, CFLAG_Converter,
	knh_Converter_init, DEFAULT_initcopy,
	DEFAULT_traverse, knh_Converter_free,
	DEFAULT_checkout, DEFAULT_compareTo,
	DEFAULT_hashkey, DEFAULT_genmap,
};

static knh_ObjectCSPI_t StringEncoderSPI = {
	"StringEncoder", 0, CFLAG_StringEncoder,
	knh_Converter_init, DEFAULT_initcopy,
	DEFAULT_traverse, knh_Converter_free,
	DEFAULT_checkout, DEFAULT_compareTo,
	DEFAULT_hashkey, DEFAULT_genmap,
};

static knh_ObjectCSPI_t StringDecoderSPI = {
	"StringEncoder", 0, CFLAG_StringDecoder,
	knh_Converter_init, DEFAULT_initcopy,
	DEFAULT_traverse, knh_Converter_free,
	DEFAULT_checkout, DEFAULT_compareTo,
	DEFAULT_hashkey, DEFAULT_genmap,
};

static knh_ObjectCSPI_t StringConverterSPI = {
	"StringConverter", 0, CFLAG_StringConverter,
	knh_Converter_init, DEFAULT_initcopy,
	DEFAULT_traverse, knh_Converter_free,
	DEFAULT_checkout, DEFAULT_compareTo,
	DEFAULT_hashkey, DEFAULT_genmap,
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
	if(delta == K_FLOAT_ZERO) return 0;
	return delta < 0 ? -1 : 1;
}

static knh_String_t *knh_fsnew__default(Ctx *ctx, knh_class_t cid, knh_bytes_t t, knh_String_t *orig, int *foundError)
{
	return new_String_(ctx, cid, t, orig);
}

static int knh_fscmp__default(knh_Semantics_t *u, knh_bytes_t v1, knh_bytes_t v2)
{
	return knh_bytes_strcmp(v1, v2);
}

static FASTAPI(void) knh_Semantics_init(Ctx *ctx, Object *o)
{
//	knh_Semantics_t *u = (knh_Semantics_t*)o;
	knh_SemanticsEX_t *b = KNH_MALLOCBODY(ctx, Semantics);
	// common
	b->flag = 0;
	b->ucid  = 0;
	KNH_INITv(b->urn, TS_EMPTY);
	KNH_INITv(b->tag,  TS_EMPTY);
	b->ivalue = NULL;
	b->fvalue = NULL;
	b->svalue = NULL;

	// int
	b->imax  = K_INT_MAX;
	b->imin  = K_INT_MIN;
	b->fichk = knh_fichk__nop;
	b->ficmp = knh_ficmp__signed;

	// float
#ifndef KONOHA_ON_LKM
	b->fstep = 0.001;
#else
	b->fstep = 1;
#endif
	b->fmax  = K_FLOAT_MAX;
	b->fmin  = K_FLOAT_MIN;
	b->ffchk = knh_ffchk__default;
	b->ffcmp = knh_ffcmp__default;
//	b->ffmt = knh_funitfmt__default;
//	b->FMT  = K_FLOAT_FMT;

	// String
	b->fsnew = knh_fsnew__default;
	b->fscmp = knh_fscmp__default;
	//b->fsconv = NULL;
	KNH_INITv(b->bconv, KNH_NULL);
	b->charlen = 0;
	b->bytelen = 0;
	KNH_INITv(b->pattern, KNH_NULL);
//	KNH_INITv(b->vocabDictIdx, KNH_NULL);
	o->ref = b;
}

static FASTAPI(void) knh_Semantics_traverse(Ctx *ctx, Object *o, knh_Ftraverse ftr)
{
	knh_Semantics_t *u = (knh_Semantics_t*)o;
	knh_SemanticsEX_t *b = DP(u);
	KNH_FTR(ctx, ftr, (b->urn));
	KNH_FTR(ctx, ftr, (b->tag));

	if(b->ivalue != NULL) KNH_FTR(ctx, ftr, (b->ivalue));
	if(b->fvalue != NULL) KNH_FTR(ctx, ftr, (b->fvalue));
	if(b->svalue != NULL) KNH_FTR(ctx, ftr, (b->svalue));

	KNH_FTR(ctx, ftr, (b->bconv));
	KNH_FTR(ctx, ftr, (b->pattern));
//	KNH_FTR(ctx, ftr, (b->vocabDictIdx));
}

static FASTAPI(void) knh_Semantics_free(Ctx *ctx, Object *o)
{
	KNH_FREEBODY(ctx, o->ref, Semantics);
}

static FASTAPI(void*) knh_Semantics_hashkey(Ctx *ctx, knh_sfp_t *sfp, int opt)
{
	if(opt == KNH_FOBJECT_KEY) {
		return (void*)DP((knh_Semantics_t*)sfp[0].o)->urn;
	}
	return DEFAULT_hashkey(ctx, sfp, opt);
}


static knh_ObjectCSPI_t SemanticsSPI = {
	"Semantics", sizeof(knh_SemanticsEX_t), CFLAG_Semantics,
	knh_Semantics_init, DEFAULT_initcopy,
	knh_Semantics_traverse, knh_Semantics_free,
	DEFAULT_checkout, DEFAULT_compareTo,
	knh_Semantics_hashkey, DEFAULT_genmap,
};

/* ======================================================================== */
/* InputStream */

static FASTAPI(void) knh_InputStream_init(Ctx *ctx, Object *o)
{
	knh_InputStream_t *in = (knh_InputStream_t*)o;
	knh_InputStreamEX_t *b = KNH_MALLOCBODY(ctx, InputStream);
	SP(in)->dspi = knh_getStreamDSPI(ctx, K_DEFAULT_DSPI);
	b->fd = -1;
	KNH_INITv(b->ba, KNH_NULL);
	b->buf = NULL;
	b->bufpos = 0;
	b->bufend = 0;
	b->bufsiz = 0;
	b->decNULL = NULL;
	KNH_INITv(b->urn, TS_DEVNULL);
	b->size    = 0;
	b->line    = 1;
	b->prev    = '\n';
	b->uri   = 0;
	in->b = b;
}

static FASTAPI(void) knh_InputStream_traverse(Ctx *ctx, Object *o, knh_Ftraverse ftr)
{
	knh_InputStream_t *in = (knh_InputStream_t*)o;
	knh_InputStreamEX_t *b = DP(in);
	KNH_FTR(ctx, ftr, (b->ba));
	KNH_NULLFTR(ctx, ftr, b->decNULL);
	KNH_FTR(ctx, ftr, (b->urn));
}

static FASTAPI(void) knh_InputStream_free(Ctx *ctx, Object *o)
{
	knh_InputStream_t *in = (knh_InputStream_t*)o;
	knh_InputStreamEX_t *b = DP(in);
	if(b->fd != IO_NULL) {
		SP(in)->dspi->fclose(ctx, b->fd);
		b->fd = IO_NULL;
	}
	KNH_FREEBODY(ctx, b, InputStream);
}

static knh_ObjectCSPI_t InputStreamSPI = {
	"InputStream", sizeof(knh_InputStreamEX_t), CFLAG_InputStream,
	knh_InputStream_init, DEFAULT_initcopy,
	knh_InputStream_traverse, knh_InputStream_free,
	DEFAULT_checkout, DEFAULT_compareTo,
	DEFAULT_hashkey, DEFAULT_genmap,
};

/* ======================================================================== */
/* OutputStream */

static FASTAPI(void) knh_OutputStream_init(Ctx *ctx, Object *o)
{
	knh_OutputStream_t *w = (knh_OutputStream_t*)o;
	knh_OutputStreamEX_t *b = KNH_MALLOCBODY(ctx, OutputStream);
	SP(w)->dspi = knh_getStreamDSPI(ctx, K_DEFAULT_DSPI);
	b->fd = IO_NULL;
	KNH_INITv(b->ba, new_Bytes(ctx, K_PAGESIZE));
	b->encNULL = NULL;
	KNH_INITv(b->urn, TS_DEVNULL);
	b->size = 0;
	b->line = 0;
	KNH_INITv(b->NEWLINE, TS_EOL);
	KNH_INITv(b->TAB, TS_TAB);
	b->indent = 0;
	b->uri = 0;
	o->ref = b;
}

static FASTAPI(void) knh_OutputStream_traverse(Ctx *ctx, Object *o, knh_Ftraverse ftr)
{
	knh_OutputStream_t *w = (knh_OutputStream_t*)o;
	knh_OutputStreamEX_t *b = DP(w);
	KNH_FTR(ctx, ftr, (b->ba));
	KNH_NULLFTR(ctx, ftr, (b->encNULL));
	KNH_FTR(ctx, ftr, (b->urn));
	KNH_FTR(ctx, ftr, (b->NEWLINE));
	KNH_FTR(ctx, ftr, (b->TAB));
}

static FASTAPI(void) knh_OutputStream_free(Ctx *ctx, Object *o)
{
	knh_OutputStream_t *w = (knh_OutputStream_t*)o;
	knh_OutputStreamEX_t *b = DP(w);
	if(b->fd != IO_NULL) {
		SP(w)->dspi->fclose(ctx, b->fd);
		b->fd = IO_NULL;
	}
	KNH_FREEBODY(ctx, b, OutputStream);
}

static knh_ObjectCSPI_t OutputStreamSPI = {
	"OutputStream", sizeof(knh_InputStreamEX_t), CFLAG_OutputStream,
	knh_OutputStream_init, DEFAULT_initcopy,
	knh_OutputStream_traverse, knh_OutputStream_free,
	DEFAULT_checkout, DEFAULT_compareTo,
	DEFAULT_hashkey, DEFAULT_genmap,
};

/* ======================================================================== */
/* Channel */

static FASTAPI(void) knh_Channel_init(Ctx *ctx, Object *o)
{
	knh_Channel_t *so = (knh_Channel_t*)o;
//	so->sd = -1;
	KNH_INITv(so->urn, TS_EMPTY);
	KNH_INITv(so->in, KNH_NULVAL(CLASS_InputStream));
	KNH_INITv(so->out, KNH_NULVAL(CLASS_OutputStream));
}

static FASTAPI(void) knh_Channel_traverse(Ctx *ctx, Object *o, knh_Ftraverse ftr)
{
	knh_Channel_t *so = (knh_Channel_t*)o;
	KNH_FTR(ctx, ftr, (so->urn));
	KNH_FTR(ctx, ftr, (so->in));
	KNH_FTR(ctx, ftr, (so->out));
}

static FASTAPI(void) knh_Channel_free(Ctx *ctx, Object *o)
{
//	knh_Channel_t *so = (knh_Channel_t*)o;
//	if(so->sd != -1) {
//		knh_socket_close(ctx, so->sd);
//	}
}

static knh_ObjectCSPI_t ChannelSPI = {
	"Channel", 0, CFLAG_Channel,
	knh_Channel_init, DEFAULT_initcopy,
	knh_Channel_traverse, knh_Channel_free,
	DEFAULT_checkout,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap,
};

/* ======================================================================== */
/* Connection */

static FASTAPI(void) knh_Connection_init(Ctx *ctx, Object *o)
{
	knh_Connection_t *c = (knh_Connection_t*)o;
	c->conn = NULL;
	c->dspi = knh_getQueryDSPI(ctx, K_DEFAULT_DSPI);
	KNH_INITv(c->urn, TS_EMPTY);
}

static FASTAPI(void) knh_Connection_traverse(Ctx *ctx, Object *o, knh_Ftraverse ftr)
{
	knh_Connection_t *c = (knh_Connection_t*)o;
	KNH_FTR(ctx, ftr, (c->urn));
}

static FASTAPI(void) knh_Connection_free(Ctx *ctx, Object *o)
{
	knh_Connection_t *c = (knh_Connection_t*)o;
	if(c->conn != NULL) {
		c->dspi->qclose(ctx, c->conn);
	}
}

static knh_ObjectCSPI_t ConnectionSPI = {
	"Connection", 0, CFLAG_Connection,
	knh_Connection_init, DEFAULT_initcopy,
	knh_Connection_traverse, knh_Connection_free,
	DEFAULT_checkout,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap,
};

/* ======================================================================== */
/* ResultSet */

static FASTAPI(void) knh_ResultSet_init(Ctx *ctx, Object *o)
{
	knh_ResultSetEX_t *b = KNH_MALLOCBODY(ctx, ResultSet);
	b->qcur = NULL;
	b->tcid = CLASS_ResultSet;
	b->column_size = 0;
	b->column = NULL;
	KNH_INITv(b->databuf, new_Bytes(ctx, 256));
	KNH_INITv(b->conn, KNH_NULL);
	b->qcurfree = knh_getQueryDSPI(ctx, K_DEFAULT_DSPI)->qcurfree;
	b->count = 0;
	o->ref = b;
}

static FASTAPI(void) knh_ResultSet_traverse(Ctx *ctx, Object *o, knh_Ftraverse ftr)
{
	size_t i = 0;
	knh_ResultSetEX_t *b = DP((knh_ResultSet_t*)o);
	KNH_FTR(ctx, ftr, (b->databuf));
	for(i = 0; i < b->column_size; i++) {
		KNH_FTR(ctx, ftr, (b->column[i].name));
	}
	KNH_FTR(ctx, ftr, (b->conn));
}

static FASTAPI(void) knh_ResultSet_free(Ctx *ctx, Object *o)
{
	knh_ResultSetEX_t *b = DP((knh_ResultSet_t*)o);
	if(b->column != NULL) {
		KNH_FREE(ctx, b->column, sizeof(knh_dbschema_t) * b->column_size);
		b->column = NULL;
	}
	if(b->qcur != NULL) {
		b->qcurfree(b->qcur);
		b->qcur = NULL;
	}
	KNH_FREEBODY(ctx, b, ResultSet);
}

static knh_ObjectCSPI_t ResultSetSPI = {
	"ResultSet", sizeof(knh_ResultSetEX_t), CFLAG_ResultSet,
	knh_ResultSet_init, DEFAULT_initcopy,
	knh_ResultSet_traverse, knh_ResultSet_free,
	DEFAULT_checkout, DEFAULT_compareTo,
	DEFAULT_hashkey, DEFAULT_genmap,
};

/* ======================================================================== */
/* Script */

static FASTAPI(void) knh_Script_init(Ctx *ctx, Object *o)
{
	knh_Script_t *scr = (knh_Script_t*)o;
	knh_class_t cid = new_ClassId(ctx);
	knh_ClassTBL_t *t = pClassTBL(ctx, cid);
	scr->h.cid = cid;
	DBG_ASSERT(t->cspi == NULL);
	t->cflag  = CFLAG_Script;
	t->oflag  = FLAG_Script;
	t->bcid   = CLASS_Script;
	t->supcid = CLASS_Script;
	t->offset = 0;
	t->cspi = ClassTBL(CLASS_Script).cspi;
	KNH_INITv(t->methods, KNH_EMPTYLIST);
	KNH_INITv(t->tmaps, KNH_EMPTYLIST);
	knh_setClassName(ctx, cid, ClassTBL(CLASS_Script).sname, ClassTBL(CLASS_Script).sname);
	DBG_ASSERT(t->defnull == NULL);
	scr->fields = NULL;
	scr->fsizeUSED = 0;
	knh_setClassDefaultValue(ctx, cid, scr, NULL);
}

static FASTAPI(void) knh_Script_free(Ctx *ctx, Object *o)
{
	knh_ClassTBL_t *t = pClassTBL(ctx, o->h.cid);
	if(t->size > 0) {
		DBG_P("Freeing Script %s, size=%d", O__(o), (int)t->size);
		KNH_FREE(ctx, o->ref, t->size);
	}
}

static knh_ObjectCSPI_t ScriptSPI = {
	"Script", 0, CFLAG_Script,
	knh_Script_init, DEFAULT_initcopy,
	knh_ObjectField_traverse, knh_Script_free,
	DEFAULT_checkout, DEFAULT_compareTo,
	DEFAULT_hashkey, DEFAULT_genmap,
};

/* ======================================================================== */
/* NameSpace */

static FASTAPI(void) knh_NameSpace_init(Ctx *ctx, Object *o)
{
	knh_NameSpaceEX_t *b = KNH_MALLOCBODY(ctx, NameSpace);
	KNH_INITv(b->nsname, TS_main);
	b->parentNULL = NULL;
	b->aliasDictMapNULL = NULL;
	b->name2cidDictSetNULL = NULL;
	b->lconstDictCaseMapNULL = NULL;
	b->func2cidDictSetNULL = NULL;
	b->strregexSPI = &STRREGEXSPI;
	b->regexSPI = &STRREGEXSPI;
	o->ref = b;
}

static FASTAPI(void) knh_NameSpace_traverse(Ctx *ctx, Object *o, knh_Ftraverse ftr)
{
	knh_NameSpaceEX_t *b = DP((knh_NameSpace_t*)o);
	KNH_FTR(ctx, ftr, (b->nsname));
	KNH_NULLFTR(ctx, ftr, (b->parentNULL));
	KNH_NULLFTR(ctx, ftr, (b->aliasDictMapNULL));
	KNH_NULLFTR(ctx, ftr, (b->name2cidDictSetNULL));
	KNH_NULLFTR(ctx, ftr, (b->func2cidDictSetNULL));
	KNH_NULLFTR(ctx, ftr, (b->lconstDictCaseMapNULL));
}

static FASTAPI(void) knh_NameSpace_free(Ctx *ctx, Object *o)
{
	KNH_FREEBODY(ctx, o->ref, NameSpace);
}

static FASTAPI(void*) knh_NameSpace_hashkey(Ctx *ctx, knh_sfp_t *sfp, int opt)
{
	if(opt == KNH_FOBJECT_KEY) {
		return (void*)DP(sfp[0].ns)->nsname;
	}
	return DEFAULT_hashkey(ctx, sfp, opt);
}

static knh_ObjectCSPI_t NameSpaceSPI = {
	"NameSpace", sizeof(knh_NameSpaceEX_t), CFLAG_NameSpace,
	knh_NameSpace_init, DEFAULT_initcopy,
	knh_NameSpace_traverse, knh_NameSpace_free,
	DEFAULT_checkout, DEFAULT_compareTo,
	knh_NameSpace_hashkey, DEFAULT_genmap,
};

/* ======================================================================== */
/* Package */

static FASTAPI(void) knh_Package_init(Ctx *ctx, Object *o)
{
	knh_Package_t *pkg = (knh_Package_t*)o;
	pkg->ns = NULL;
	pkg->script = NULL;
	pkg->hdlr = NULL;
}

static FASTAPI(void) knh_Package_traverse(Ctx *ctx, Object *o, knh_Ftraverse ftr)
{
	knh_Package_t *pkg = (knh_Package_t*)o;
	KNH_NULLFTR(ctx, ftr, pkg->ns);
	KNH_NULLFTR(ctx, ftr, pkg->script);
}

static knh_ObjectCSPI_t PackageSPI = {
	"Package", 0, CFLAG_Package,
	knh_Package_init, DEFAULT_initcopy,
	knh_Package_traverse, DEFAULT_free,
	DEFAULT_checkout, DEFAULT_compareTo,
	DEFAULT_hashkey, DEFAULT_genmap,
};

/* ======================================================================== */
/* System */

static knh_uintptr_t knh_autoSystemId = 0;

#ifdef K_USING_NOFILE
#define stdin  NULL
#define stdout NULL
#define stderr NULL
#endif

static FASTAPI(void) knh_System_init(Ctx *ctx, Object *o)
{
	knh_SystemEX_t *sys = KNH_MALLOCBODY(ctx, System);
	sys->sysid = knh_autoSystemId++;
	sys->ctxcount = 0;

	KNH_INITv(sys->ClassNameDictSet, new_DictSet0(ctx, 0));
	KNH_INITv(sys->EventDictCaseSet, new_DictCaseSet0(ctx, 0));
	KNH_INITv(sys->enc,   new_T(knh_getSystemEncoding()));
	KNH_INITv(sys->in,    new_InputStream__stdio(ctx, stdin, sys->enc));
	KNH_INITv(sys->out,   new_OutputStream__stdio(ctx, stdout, sys->enc));
	KNH_INITv(sys->err,   new_OutputStream__stdio(ctx, stderr, sys->enc));

	KNH_INITv(sys->props, new_DictCaseMap0(ctx, 20));
	{
		size_t ncapacity = knh_good_entrysize(sizeof(knh_hmem_t), sizeof(knh_NameInfo_t), K_TFIELD_SIZE + 10);
		KNH_INITv(sys->nameDictCaseSet, new_DictCaseSet0(ctx, K_TFIELD_SIZE + 10));
		sys->hnameinfo = knh_hmalloc(ctx, sizeof(knh_NameInfo_t), ncapacity);
	}
	KNH_INITv(sys->urnDictSet, new_DictSet0(ctx, 0));
	KNH_INITv(sys->urns, new_Array0(ctx, 1));
	KNH_INITv(sys->tokenDictSet, new_DictSet0(ctx, K_TOKEN_MAXSIZ));
	KNH_INITv(sys->dspiDictSet, new_DictSet0(ctx, 32));
	KNH_INITv(sys->PackageDictMap, new_DictMap0(ctx, 0));
	KNH_INITv(sys->URNAliasDictMap, new_DictMap0(ctx, 0));
	o->ref = sys;
}

static FASTAPI(void) knh_System_traverse(Ctx *ctx, Object *o, knh_Ftraverse ftr)
{
	knh_SystemEX_t *sys = DP((knh_System_t*)o);
	KNH_FTR(ctx, ftr, (sys->enc));
	KNH_FTR(ctx, ftr, (sys->in));
	KNH_FTR(ctx, ftr, (sys->out));
	KNH_FTR(ctx, ftr, (sys->err));
	KNH_FTR(ctx, ftr, (sys->props));
	KNH_FTR(ctx, ftr, (sys->EventDictCaseSet));
	KNH_FTR(ctx, ftr, (sys->ClassNameDictSet));
	{
		size_t i, size = knh_DictSet_size(sys->nameDictCaseSet);
		for(i = 0; i < size; i++) {
			KNH_FTR(ctx, ftr, sys->nameinfo[i].name);
		}
	}
	KNH_FTR(ctx, ftr, (sys->nameDictCaseSet));
	KNH_FTR(ctx, ftr, (sys->urnDictSet));
	KNH_FTR(ctx, ftr, (sys->urns));
	KNH_FTR(ctx, ftr, (sys->tokenDictSet));

	KNH_FTR(ctx, ftr, (sys->PackageDictMap));
	KNH_FTR(ctx, ftr, (sys->URNAliasDictMap));
	KNH_FTR(ctx, ftr, (sys->dspiDictSet));
}

static FASTAPI(void) knh_System_free(Ctx *ctx, Object *o)
{
	knh_SystemEX_t *sys = DP((knh_System_t*)o);
	knh_hfree(ctx, sys->hnameinfo);
	sys->hnameinfo = NULL;
	KNH_FREEBODY(ctx, sys, System);
}

static knh_ObjectCSPI_t SystemSPI = {
	"System", sizeof(knh_SystemEX_t), CFLAG_System,
	knh_System_init, DEFAULT_initcopy,
	knh_System_traverse, knh_System_free,
	DEFAULT_checkout,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap,
};

/* ======================================================================== */
/* Context */

static FASTAPI(void) knh_Context_init(Ctx *ctx, Object *o)
{
	/* Context can be only instatiated in new_Context() */
	KNH_THROW__T(ctx, "Abort!!");
}

static knh_ObjectCSPI_t ContextSPI = {
	"Context", 0, CFLAG_Context,
	knh_Context_init, DEFAULT_initcopy,
	DEFAULT_traverse, DEFAULT_free,
	DEFAULT_checkout,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap,
};

///* ======================================================================== */
///* Thread */
//
//static FASTAPI(void) knh_Thread_init(Ctx *ctx, Object *o)
//{
//}
//
//static FASTAPI(void) knh_Thread_traverse(Ctx *ctx, Object *o, knh_Ftraverse ftr)
//{
//}
//
//static knh_ObjectCSPI_t ThreadSPI = {
//	"Thread", 0, CFLAG_Thread,
//	knh_Thread_init,
//	DEFAULT_initcopy,
//	knh_Thread_traverse, DEFAULT_checkout,
//	DEFAULT_compareTo,
//	DEFAULT_hashkey,
//	DEFAULT_genmap,
//};
//
///* ======================================================================== */
///* ScriptEngine */
//
//static FASTAPI(void) knh_ScriptEngine_init(Ctx *ctx, Object *o)
//{
//}
//
//static FASTAPI(void) knh_ScriptEngine_traverse(Ctx *ctx, Object *o, knh_Ftraverse ftr)
//{
//}
//
//static knh_ObjectCSPI_t ScriptEngineSPI = {
//	"ScriptEngine", 0, CFLAG_ScriptEngine,
//	knh_ScriptEngine_init,
//	DEFAULT_initcopy,
//	knh_ScriptEngine_traverse, DEFAULT_checkout,
//	DEFAULT_compareTo,
//	DEFAULT_hashkey,
//	DEFAULT_genmap,
//};

/* ======================================================================== */
/* Token */

static FASTAPI(void) knh_Token_init(Ctx *ctx, Object *o)
{
	knh_Token_t *tk = (knh_Token_t*)o;
	knh_TokenEX_t *b = KNH_MALLOCBODY(ctx, Token);
	tk->tt        =  TT_ASIS;
	tk->type      =  TYPE_var;
	SP(tk)->uri   =   0;
	SP(tk)->line  =   0;
	KNH_INITv(b->data, KNH_NULL);
	tk->b = b;
}

static FASTAPI(void) knh_Token_traverse(Ctx *ctx, Object *o, knh_Ftraverse ftr)
{
	knh_Token_t *tk = (knh_Token_t*)o;
	KNH_FTR(ctx, ftr, DP(tk)->data);
}

static FASTAPI(void) knh_Token_free(Ctx *ctx, Object *o)
{
	KNH_FREEBODY(ctx, o->ref, Token);
}

static knh_ObjectCSPI_t TermSPI = {
	"Term", 0, 0,
	ABSTRACT_init, DEFAULT_initcopy,
	DEFAULT_traverse, DEFAULT_free,
	DEFAULT_checkout, DEFAULT_compareTo,
	DEFAULT_hashkey, DEFAULT_genmap,
};

static knh_ObjectCSPI_t TokenSPI = {
	"Token", sizeof(knh_TokenEX_t), CFLAG_Token,
	knh_Token_init, DEFAULT_initcopy,
	knh_Token_traverse, knh_Token_free,
	DEFAULT_checkout, DEFAULT_compareTo,
	DEFAULT_hashkey,  DEFAULT_genmap,
};

/* ======================================================================== */
/* Stmt */

static FASTAPI(void) knh_Stmt_init(Ctx *ctx, Object *o)
{
	knh_Stmt_t *stmt = (knh_Stmt_t*)o;
	knh_StmtEX_t *b = KNH_MALLOCBODY(ctx, Stmt);
	SP(stmt)->uri   = 0;
	SP(stmt)->line  = 0;
	SP(stmt)->stt   = STT_DONE;
	SP(stmt)->type = TYPE_var;
	b->flag0 = 0;
	b->espidx = 0;
	b->size = 0;
	b->capacity = 0;
	b->terms = NULL;
	b->nextNULL = NULL;
	KNH_INITv(b->metaDictCaseMap,  KNH_NULL);
	o->ref = b;
}

static FASTAPI(void) knh_Stmt_traverse(Ctx *ctx, Object *o, knh_Ftraverse ftr)
{
	knh_StmtEX_t *b = DP((knh_Stmt_t*)o);
	if(b->terms != NULL) {
		size_t i;
		for(i = 0; i < b->size; i++) {
			KNH_FTR(ctx, ftr, b->terms[i]);
		}
	}
	KNH_FTR(ctx, ftr, (b->metaDictCaseMap));
	KNH_NULLFTR(ctx, ftr, (b->nextNULL));
}

static FASTAPI(void) knh_Stmt_free(Ctx *ctx, Object *o)
{
	knh_StmtEX_t *b = DP((knh_Stmt_t*)o);
	if(b->terms != NULL) {
		KNH_FREE(ctx, b->terms, sizeof(knh_Term_t*) * b->capacity);
	}
	KNH_FREEBODY(ctx, b, Stmt);
}

static knh_ObjectCSPI_t StmtSPI = {
	"Stmt", sizeof(knh_StmtEX_t), CFLAG_Stmt,
	knh_Stmt_init, DEFAULT_initcopy,
	knh_Stmt_traverse, knh_Stmt_free,
	DEFAULT_checkout, DEFAULT_compareTo,
	DEFAULT_hashkey, DEFAULT_genmap,
};

/* ------------------------------------------------------------------------ */

knh_Term_t *knh_Stmt_done(Ctx *ctx, knh_Stmt_t *o)
{
	knh_StmtEX_t *b = DP((knh_Stmt_t*)o);
	SP(o)->stt = STT_DONE;
	if(b->terms != NULL) {
		size_t i;
		for(i = 0; i < b->size; i++) {
			KNH_FINALv(ctx, b->terms[i]);
		}
		KNH_FREE(ctx, b->terms, sizeof(knh_Term_t*) * b->capacity);
		b->terms = NULL;
		b->capacity = 0;
		b->size = 0;
	}
	KNH_SETv(ctx, b->metaDictCaseMap, KNH_NULL);
	return (knh_Term_t*)(o);
}

/* ======================================================================== */
/* Gamma */

static FASTAPI(void) knh_Gamma_init(Ctx *ctx, Object *o)
{
	knh_intptr_t i;
	knh_GammaEX_t *b = KNH_MALLOCBODY(ctx, Gamma);
	knh_bzero(b, sizeof(knh_GammaEX_t));
	b->cflag = FLAG_Gamma_InlineFunction | FLAG_Gamma_TailRecursion;
	KNH_INITv(b->ns, ctx->share->mainns);
	DBG_ASSERT(IS_NameSpace(b->ns));
	KNH_INITv(b->mtd,   KNH_NULL);
	b->gf = (knh_fields_t*)KNH_MALLOC(ctx, K_GAMMASIZE * sizeof(knh_fields_t));
	knh_bzero(b->gf, K_GAMMASIZE * sizeof(knh_fields_t));
	for(i = 0; i < K_GAMMASIZE; i++) {
		b->gf[i].flag  = 0;
		b->gf[i].type  = TYPE_void;
		b->gf[i].fn    = FN_NONAME;
		b->gf[i].value = NULL;
	}
	b->psize = 0;
	b->typevars = (knh_typevars_t*)KNH_MALLOC(ctx, K_GAMMASIZE * sizeof(knh_typevars_t));
	knh_bzero(b->typevars, K_GAMMASIZE * sizeof(knh_typevars_t));
	b->tvsize = 0;
	KNH_INITv(b->lstacks, new_Array0(ctx, 0));
	KNH_INITv(b->insts, new_Array0(ctx, 64));
	KNH_INITv(b->finallyStmt, KNH_NULL);
	b->dlhdr = NULL;
	b->symbolDictMap = NULL;
	b->constPools = NULL;
	b->statStmt = 1;
	b->statError = 0;
	b->statWarning = 0;
	b->statBadManner = 0;
	b->statKonohaStyle = 0;
	o->ref = b;
}

static FASTAPI(void) knh_Gamma_traverse(Ctx *ctx, Object *o, knh_Ftraverse ftr)
{
	size_t i;
	knh_GammaEX_t *b = DP((knh_Gamma_t*)o);
	for(i = 0; i < K_GAMMASIZE; i++) {
		KNH_NULLFTR(ctx, ftr, b->gf[i].value);
	}
	KNH_FTR(ctx, ftr, (b->ns));
	KNH_FTR(ctx, ftr, (b->script));
	KNH_FTR(ctx, ftr, (b->mtd));
	KNH_FTR(ctx, ftr, (b->lstacks));
	KNH_FTR(ctx, ftr, (b->insts));
	KNH_FTR(ctx, ftr, (b->finallyStmt));
	KNH_NULLFTR(ctx, ftr, b->symbolDictMap);
	KNH_NULLFTR(ctx, ftr, b->constPools);
}

static FASTAPI(void) knh_Gamma_free(Ctx *ctx, Object *o)
{
	knh_GammaEX_t *b = DP((knh_Gamma_t*)o);
	KNH_FREE(ctx, b->gf, K_GAMMASIZE * sizeof(knh_fields_t));
	KNH_FREE(ctx, b->typevars, K_GAMMASIZE * sizeof(knh_typevars_t));
	KNH_FREEBODY(ctx, b, Gamma);
}

static knh_ObjectCSPI_t GammaSPI = {
	"Gamma", sizeof(knh_GammaEX_t), CFLAG_Gamma,
	knh_Gamma_init, DEFAULT_initcopy,
	knh_Gamma_traverse, knh_Gamma_free,
	DEFAULT_checkout, DEFAULT_compareTo,
	DEFAULT_hashkey, DEFAULT_genmap,
};

///* ======================================================================== */
///* KLRInst */
//
//static FASTAPI(void) knh_KLRInst_init(Ctx *ctx, Object *o)
//{
//	knh_KLRInst_t *inst = (knh_KLRInst_t*)o;
//	inst->op = (knh_opline_t*)KNH_MALLOC(ctx, sizeof(knh_opline_t));
//	knh_bzero(inst->op, sizeof(knh_opline_t));
//	inst->line   = 0;
//	inst->code_pos = NULL;
//}
//
//static FASTAPI(void) knh_KLRInst_traverse(Ctx *ctx, Object *o, knh_Ftraverse ftr)
//{
//	knh_KLRInst_t *inst = (knh_KLRInst_t*)o;
//	DBG_ASSERT(inst->opcode <= OPCODE_NOP);
//	knh_opline_traverse(ctx, inst->op, ftr);
//}
//
//static FASTAPI(void) knh_KLRInst_free(Ctx *ctx, Object *o)
//{
//	knh_KLRInst_t *inst = (knh_KLRInst_t*)o;
//	KNH_FREE(ctx, inst->op, sizeof(knh_opline_t));
//}
//
//static knh_ObjectCSPI_t KLRInstSPI = {
//	"KLRInst", 0, CFLAG_KLRInst,
//	knh_KLRInst_init, DEFAULT_initcopy,
//	knh_KLRInst_traverse, knh_KLRInst_free,
//	DEFAULT_checkout, DEFAULT_compareTo,
//	DEFAULT_hashkey, DEFAULT_genmap,
//};

/* ======================================================================== */
/* BasicBlock */

static FASTAPI(void) knh_BasicBlock_init(Ctx *ctx, Object *o)
{
	knh_BasicBlock_t *bb = (knh_BasicBlock_t*)o;
	bb->b = KNH_MALLOCBODY(ctx, BasicBlock);
	knh_bzero(bb->b, sizeof(knh_BasicBlockEX_t));
	bb->listNC  = NULL;
	bb->nextNC  = NULL;
	bb->jumpNC  = NULL;
}

static FASTAPI(void) knh_BasicBlock_traverse(Ctx *ctx, Object *o, knh_Ftraverse ftr)
{
	knh_BasicBlock_t *bb = (knh_BasicBlock_t*)o;
	size_t i;
	for(i = 0; i < DP(bb)->size; i++) {
		knh_opline_traverse(ctx, DP(bb)->opbuf + i, ftr);
	}
}

static FASTAPI(void) knh_BasicBlock_free(Ctx *ctx, Object *o)
{
	knh_BasicBlock_t *bb = (knh_BasicBlock_t*)o;
	if(DP(bb)->capacity > 0) {
		KNH_FREE(ctx, DP(bb)->opbuf, DP(bb)->capacity * sizeof(knh_opline_t));
	}
	KNH_FREEBODY(ctx, DP(bb), BasicBlock);
}

static knh_ObjectCSPI_t BasicBlockSPI = {
	"BasicBlock", 0, CFLAG_BasicBlock,
	knh_BasicBlock_init, DEFAULT_initcopy,
	knh_BasicBlock_traverse, knh_BasicBlock_free,
	DEFAULT_checkout, DEFAULT_compareTo,
	DEFAULT_hashkey, DEFAULT_genmap,
};

/* ======================================================================== */
/* KLRCode */

static FASTAPI(void) knh_KLRCode_init(Ctx *ctx, Object *o)
{
	knh_KLRCode_t *b = (knh_KLRCode_t*)o;
	b->codesize = 0;
	b->code = NULL;
	b->uri = 0;
	KNH_INITv(b->source, TS_EMPTY);
}

static FASTAPI(void) knh_KLRCode_traverse(Ctx *ctx, Object *o, knh_Ftraverse ftr)
{
	knh_KLRCode_t *b = (knh_KLRCode_t*)o;
	knh_opline_t *pc = b->code;
	KNH_FTR(ctx, ftr, b->source);
	while(pc->opcode != OPCODE_RET) {
		knh_opline_traverse(ctx, pc, ftr);
		pc++;
	}
}

static FASTAPI(void) knh_KLRCode_free(Ctx *ctx, Object *o)
{
	knh_KLRCode_t *b = (knh_KLRCode_t*)o;
#ifdef K_USING_VMCOUNT
	knh_opline_t *pc = b->code;
	while(pc->opcode != OPCODE_RET) {
		knh_opcode_count(ctx, pc);
		pc++;
	}
#endif
	KNH_FREE(ctx, b->code, b->codesize);
}

static knh_ObjectCSPI_t KLRCodeSPI = {
	"KLRCode", 0, CFLAG_KLRCode,
	knh_KLRCode_init, DEFAULT_initcopy,
	knh_KLRCode_traverse, knh_KLRCode_free,
	DEFAULT_checkout,
	DEFAULT_compareTo,
	DEFAULT_hashkey,
	DEFAULT_genmap,
};

/* ======================================================================== */
/* @data*/

typedef struct {
	const char   *name;
	int           index;
} knh_StringData0_t ;

typedef struct {
	const char *name;
	knh_fieldn_t fn;
} knh_FieldNameData0_t ;

/* ======================================================================== */

#ifdef __cplusplus
}
#endif

#define K_USING_DEFAULTAPI

#include"newapi.c"
#include"objectapi.c"
#include"numberapi.c"
#include"stringapi.c"
#include"arrayapi.c"
#include"dictapi.c"

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

/* ------------------------------------------------------------------------ */

static Object *knh_Context_fdefault(Ctx *ctx, knh_class_t cid)
{
	return (Object*)ctx;
}
static Object *knh_NameSpace_fdefault(Ctx *ctx, knh_class_t cid)
{
	return UPCAST(ctx->share->mainns);
}
static void knh_setDefaultValues(Ctx *ctx)
{
	knh_setClassDefaultValue(ctx, CLASS_Object, KNH_NULL, NULL);
	knh_setClassDefaultValue(ctx, CLASS_Any, KNH_NULL, NULL);
	knh_setClassDefaultValue(ctx, CLASS_Boolean, KNH_FALSE, NULL);
#if defined(K_USING_SEMANTICS)
	{
		knh_Semantics_t *u = new_(Semantics);
		KNH_INITv(DP(u)->ivalue, KNH_INT0);
		KNH_INITv(DP(u)->fvalue, KNH_FLOAT0);
		KNH_INITv(DP(u)->svalue, TS_EMPTY);
		knh_setClassDefaultValue(ctx, CLASS_Semantics, u, NULL);
	}
#endif
	// load file/Channel/regex/db drivers
	knh_loadDriver(ctx);
//	knh_loadDefaultChannelDriver(ctx);
	knh_setClassDefaultValue(ctx, CLASS_Context, KNH_NULL, knh_Context_fdefault);
	knh_setClassDefaultValue(ctx, CLASS_NameSpace, KNH_NULL, knh_NameSpace_fdefault);
	knh_setClassDefaultValue(ctx, CLASS_System, UPCAST(ctx->sys), NULL);
}

static void knh_loadFieldNameData0(Ctx *ctx, knh_FieldNameData0_t *data)
{
	while(data->name != NULL) {
		knh_String_t *name = new_T(data->name);
#if defined(K_USING_DEBUG)
		knh_fieldn_t fn = knh_addname(ctx, name, knh_DictCaseSet_append);
		//DBG_P("name=%s, fn=%d, data->fn=%d, %d", data->name, fn, data->fn, data->fn-MN_OPSIZE);
		DBG_ASSERT(fn == data->fn - MN_OPSIZE);
#else
		knh_addname(ctx, name, knh_DictCaseSet_append);
#endif
		data++;
	}
}

/* ------------------------------------------------------------------------ */
/* @data */

static knh_IntData_t IntConstData0[] = {
	{"Int.MAX", K_INT_MAX},
	{"Int.MIN", K_INT_MIN},
//	{"Method.NOTRACE", KNH_NOTRACE},
//	{"Method.SECURITY", KNH_SECURITYTRACE},
//	{"Method.AUDIT", KNH_AUDITTRACE},
//	{"Method.PROFILE", KNH_PROFILER},
//	{"Method.STACK", KNH_STACKTRACE},
//	{"Method.FUNCTION", KNH_FUNCTIONTRACE},
	{NULL, 0}
};

static knh_FloatData_t FloatConstData0[] = {
	{"Float.MAX", K_FLOAT_MAX},
	{"Float.MIN", K_FLOAT_MIN},

	{NULL, K_FLOAT_ZERO}
};

static knh_StringData_t StringConstData0[] = {
	{"$konoha.version", K_VERSION},
	{"$konoha.os", KONOHA_OS},
	{"$konoha.platform", K_PLATFORM},
	{NULL, NULL},
};

static knh_data_t CParamData0[] = {
	DATA_CPARAM, CLASS_Iterator, 1, 0, TYPE_Any, FN_V,
	DATA_CPARAM, CLASS_Tuple, 1, 0, TYPE_Any, FN_V,
	DATA_CPARAM, CLASS_Range, 1, 0, TYPE_Any, FN_V,
	DATA_CPARAM, CLASS_Array, 1, 0, TYPE_Any, FN_V,
	DATA_CPARAM, CLASS_Map, 2, 0, TYPE_Any, FN_K, TYPE_Any, FN_V,
	DATA_CPARAM, CLASS_Func, 1, 1, TYPE_Any, FN_P, TYPE_Any, FN_V,
	0,
};

void knh_loadSystemStructData(Ctx *ctx, const knh_PackageLoaderAPI_t *kapi)
{
	kapi->loadData(ctx, StructData0, NULL);
}

void knh_loadSystemString(Ctx *ctx)
{
	const char **data = StringData0;
	size_t i = 0;
	for(i = 0; *data != NULL; i++) {
		DBG_ASSERT(ctx->share->tString[i] == NULL);
		DBG_ASSERT(i < (SIZEOF_TSTRING / sizeof(knh_String_t*)));
		KNH_INITv(ctx->share->tString[i], new_T(*data));
		data++;
	}
}

void knh_loadSystemData(Ctx *ctx, const knh_PackageLoaderAPI_t *kapi)
{
	kapi->loadData(ctx, ClassData0, NULL);
	kapi->loadData(ctx, CParamData0, NULL);
	kapi->loadIntData(ctx, IntConstData0);
	kapi->loadFloatData(ctx, FloatConstData0);
	kapi->loadStringData(ctx, StringConstData0);
	knh_getURI(ctx, STEXT("(eval)"));  // URI_EVAL
	knh_setDefaultValues(ctx);
}

void knh_loadSystemMethod(Ctx *ctx, const knh_PackageLoaderAPI_t *kapi)
{
	knh_ParamArray_t *pools[K_PARAM0_SIZE];
	knh_loadFieldNameData0(ctx, FieldNameData0);
	kapi->loadData(ctx, ParamArrayData0, pools);
	kapi->loadData(ctx, APIData0, pools);
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
