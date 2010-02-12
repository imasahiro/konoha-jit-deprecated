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


static
knh_class_t knh_addSpecializedType(Ctx *ctx, knh_class_t cid, knh_class_t bcid, knh_Semantics_t *u);

/* ======================================================================== */
/* [Int] */

static
int knh_fichk__range(knh_Semantics_t *u, knh_int_t v)
{
	return (DP(u)->imin <= v && v <= DP(u)->imax);
}

/* ------------------------------------------------------------------------ */

static
int knh_fichk__umax(knh_Semantics_t *u, knh_uint_t v)
{
	return (v <= DP(u)->umax);
}

/* ------------------------------------------------------------------------ */

static
int knh_fichk__urange(knh_Semantics_t *u, knh_uint_t v)
{
	return (DP(u)->umin <= v && v <= DP(u)->umax);
}

/* ------------------------------------------------------------------------ */

static
int knh_ficmp__unsigned(knh_Semantics_t *u, knh_uint_t v1, knh_uint_t v2)
{
	if(v1 == v2) return 0;
	return (v1 > v2) ? 1 : -1;
}

/* ------------------------------------------------------------------------ */
/* [Float] */

static
int knh_ffchk__range(knh_Semantics_t *u, knh_float_t v)
{
	return (DP(u)->fmin <= v && v <= DP(u)->fmax);
}

/* ------------------------------------------------------------------------ */

static
int knh_ffcmp__step(knh_Semantics_t *u, knh_float_t v1, knh_float_t v2)
{
	knh_float_t v = v1 - v2;
	if(v >= DP(u)->fstep) return 1;
	if(v <= -(DP(u)->fstep)) return -1;
	return 0;
}

/* ------------------------------------------------------------------------ */

static
void knh_Semantics_initIntRange(Ctx *ctx, knh_Semantics_t *u, knh_int_t min, knh_int_t max)
{
	DP(u)->imin = min;
	DP(u)->fmin = (knh_float_t)min;
	DP(u)->imax = max;
	DP(u)->fmax = (knh_float_t)max;
#ifndef KONOHA_ON_LKM
	DP(u)->fstep = 1.0;
#else
	DP(u)->fstep = 1;
#endif
	if(min >= 0) {
		DP(u)->ficmp = (knh_Ficmp)knh_ficmp__unsigned;
		if(min == 0) {
			DP(u)->fichk = (knh_Fichk)knh_fichk__umax;
		}
		else {
			DP(u)->fichk = (knh_Fichk)knh_fichk__urange;
		}
		DP(u)->ffchk = knh_ffchk__range;
	}
	else {
		if(min != KNH_INT_MIN || max != KNH_INT_MAX) {
			DP(u)->fichk = knh_fichk__range;
			DP(u)->ffchk = knh_ffchk__range;
		}
	}
	DP(u)->ffcmp = knh_ffcmp__step;
}

/* ------------------------------------------------------------------------ */

void knh_write_intx(Ctx *ctx, knh_OutputStream_t *w, knh_Semantics_t *u, knh_int_t v)
{
	char buf[KNH_INT_FMTSIZ];
	char *fmt = KNH_INT_FMT;
	if(DP(u)->imin >= 0) {
		fmt = KNH_UINT_FMT;
	}
	knh_snprintf(buf, sizeof(buf), fmt, v);
	knh_write(ctx, w, B(buf));
	knh_bytes_t tag = __tobytes(DP(u)->tag);
	if(tag.len > 0) {
		knh_putc(ctx, w, '[');
		knh_write(ctx, w, tag);
		knh_putc(ctx, w, ']');
	}
}

/* ------------------------------------------------------------------------ */

static
void knh_Semantics_initFloatRange(Ctx *ctx, knh_Semantics_t *u, knh_float_t min, knh_float_t max, knh_float_t step)
{
	DP(u)->fmin = min;
	DP(u)->fmax = max;
	DP(u)->fstep = step;
	DP(u)->imax = (knh_int_t)max;
	DP(u)->imin = (knh_int_t)min;

#ifndef KONOHA_ON_LKM
	if(min >= 0.0) {
#else
	if(min >= 0) {
#endif
		DP(u)->ficmp = (knh_Ficmp)knh_ficmp__unsigned;
		if(min == 0) {
			DP(u)->fichk = (knh_Fichk)knh_fichk__umax;
		}
		else {
			DP(u)->fichk = (knh_Fichk)knh_fichk__urange;
		}
		DP(u)->ffchk = knh_ffchk__range;
	}
	else {
		if(min != KNH_FLOAT_MIN || max != KNH_FLOAT_MAX) {
			DP(u)->fichk = knh_fichk__range;
			DP(u)->ffchk = knh_ffchk__range;
		}
	}

#ifndef KONOHA_ON_LKM
	if(step != 0.0) {
#else
	if(step != 0) {
#endif
		DP(u)->ffcmp = knh_ffcmp__step;
	}
}

/* ------------------------------------------------------------------------ */

void knh_write_floatx(Ctx *ctx, knh_OutputStream_t *w, knh_Semantics_t *u, knh_float_t v)
{
	char *FMT = KNH_FLOAT_FMT;
#if !defined(KNH_USING_NOFLOT)
	knh_float_t step = DP(u)->fstep;
	if(0.1 <= step && step < 1.0) {
		FMT = KNH_FLOAT_FMT1;
	}
	else if(0.01 <= step && step < 0.1) {
		FMT = KNH_FLOAT_FMT2;
	}
	else if(0.001 <= step && step < 0.01) {
		FMT = KNH_FLOAT_FMT3;
	}
	else if(0.0001 <= step && step < 0.001) {
		FMT = KNH_FLOAT_FMT4;
	}
#endif
	knh_write_ffmt(ctx, w, FMT, v);
	knh_bytes_t tag = __tobytes(DP(u)->tag);
	if(tag.len > 0) {
		knh_putc(ctx, w, '[');
		knh_write(ctx, w, tag);
		knh_putc(ctx, w, ']');
	}
}

/* ------------------------------------------------------------------------ */

static MAPPER knh_IntX_FloatX(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	knh_float_t v = (knh_float_t)sfp[0].ivalue;
	KNH_MAPPED_Float(ctx, sfp, v);
}

/* ------------------------------------------------------------------------ */

static MAPPER knh_FloatX_IntX(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	knh_int_t v = (knh_int_t)sfp[0].fvalue;
	KNH_MAPPED_Int(ctx, sfp, v);
}

/* ------------------------------------------------------------------------ */

static
int knh_Semantics_isVocab(knh_Semantics_t *u)
{
	return IS_DictIdx(DP(u)->vocabDictIdx);
}

/* ------------------------------------------------------------------------ */

static
knh_String_t *knh_Semantics_getVocabAt(Ctx *ctx, knh_Semantics_t *u, size_t n)
{
	return knh_DictIdx_get__fast(DP(u)->vocabDictIdx, n - DP(u)->imin);
}

/* ------------------------------------------------------------------------ */

static
knh_int_t knh_Semantics_getVocabIdx(Ctx *ctx, knh_Semantics_t *u, knh_String_t *s)
{
	return knh_DictIdx_index(ctx, DP(u)->vocabDictIdx, __tobytes(s)) + DP(u)->imin;
}

/* ------------------------------------------------------------------------ */

static MAPPER knh_IntX_Vocab(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	knh_Semantics_t *u = (knh_Semantics_t*)DP(sfp[1].mpr)->mapdata;
	KNH_ASSERT(IS_Semantics(u));
	KNH_MAPPED(ctx, sfp, knh_Semantics_getVocabAt(ctx, u, sfp[0].ivalue));
}

/* ------------------------------------------------------------------------ */

static MAPPER knh_Vocab_IntX(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	knh_Semantics_t *u = (knh_Semantics_t*)DP(sfp[1].mpr)->mapdata;
	KNH_ASSERT(IS_Semantics(u));
	KNH_MAPPED_Int(ctx, sfp, knh_Semantics_getVocabIdx(ctx, u, sfp[0].s));
}

/* ------------------------------------------------------------------------ */

void knh_Semantics_reuse(Ctx *ctx, knh_Semantics_t *u, knh_class_t cid)
{
	knh_class_t bcid = ClassTable(cid).bcid;
	if(bcid == CLASS_Int) {
		knh_int_t v = 0;
		if(!DP(u)->fichk(u, v)) {
			v = DP(u)->imin;
		}
		KNH_ASSERT(DP(u)->ivalue == NULL);
		KNH_INITv(DP(u)->ivalue, new_IntX__fast(ctx, cid, v));
		if(DP(u)->fvalue != NULL) {
			knh_Mapper_t *mpr = new_Mapper(ctx, FLAG_Mapper_Affine, DP(u)->ucid, cid, knh_FloatX_IntX, (Object*)u);
			knh_addMapper(ctx, mpr);
			mpr = new_Mapper(ctx, FLAG_Mapper_Affine, cid, DP(u)->ucid, knh_IntX_FloatX, (Object*)u);
			knh_addMapper(ctx, mpr);
		}
		if(DP(u)->svalue != NULL) {
			if(knh_Semantics_isVocab(u)) {
				knh_Mapper_t *mpr = new_Mapper(ctx, FLAG_Mapper_Affine, cid, DP(u)->ucid, knh_IntX_Vocab, (Object*)u);
				knh_addMapper(ctx, mpr);
				mpr = new_Mapper(ctx, FLAG_Mapper_Affine, DP(u)->ucid, cid, knh_Vocab_IntX, (Object*)u);
				knh_addMapper(ctx, mpr);
			}
		}
	}
	else if(bcid == CLASS_Float) {
		knh_float_t v = 0;
		if(!DP(u)->ffchk(u, v)) {
			v = DP(u)->fmin;
		}
		KNH_ASSERT(DP(u)->fvalue == NULL);
		KNH_INITv(DP(u)->fvalue, new_FloatX__fast(ctx, cid, v));
		if(DP(u)->ivalue != NULL) {
			knh_Mapper_t *mpr = new_Mapper(ctx, FLAG_Mapper_Affine, cid, DP(u)->ucid, knh_FloatX_IntX, (Object*)u);
			knh_addMapper(ctx, mpr);
			mpr = new_Mapper(ctx, FLAG_Mapper_Affine, DP(u)->ucid, cid, knh_IntX_FloatX, (Object*)u);
			knh_addMapper(ctx, mpr);
		}
	}
	else {
		KNH_ASSERT(bcid == CLASS_String);
		TODO();

	}
}

/* ------------------------------------------------------------------------ */

KNHAPI(knh_Semantics_t*) new_Enum(Ctx *ctx, char *tag, knh_bytes_t urn, knh_int_t min, knh_int_t max)
{
	knh_class_t cid = new_ClassId(ctx);
	knh_Semantics_t* u = (knh_Semantics_t*)new_Object_bcid(ctx, CLASS_Semantics, (int)cid);
	DP(u)->ubcid = CLASS_Int;
	KNH_SETv(ctx, DP(u)->urn, new_String(ctx, urn, NULL));
	if(tag != NULL || tag[0] != 0) {
		KNH_SETv(ctx, DP(u)->tag, T__(tag));
	}
	knh_Semantics_initIntRange(ctx, u, min, max);
	if(!DP(u)->fichk(u, 0)) {
		KNH_INITv(DP(u)->ivalue, new_IntX__fast(ctx, cid, min));
	}
	else {
		KNH_INITv(DP(u)->ivalue, new_IntX__fast(ctx, cid, 0));
	}
	knh_addSpecializedType(ctx, cid, CLASS_Int, u);
	return u;
}

/* ------------------------------------------------------------------------ */


KNHAPI(knh_Semantics_t*) new_Unit(Ctx *ctx, char *tag, knh_bytes_t urn, knh_float_t min, knh_float_t max, knh_float_t step)
{
	knh_class_t cid = new_ClassId(ctx);
	knh_Semantics_t* u = (knh_Semantics_t*)new_Object_bcid(ctx, CLASS_Semantics, (int)cid);
	DP(u)->ubcid = CLASS_Float;
	KNH_SETv(ctx, DP(u)->urn, new_String(ctx, urn, NULL));
	if(tag != NULL || tag[0] != 0) {
		KNH_SETv(ctx, DP(u)->tag, T__(tag));

	}
	knh_Semantics_initFloatRange(ctx, u, min, max, step);
#if !defined(KNH_USING_NOFLOT)
	if(!DP(u)->ffchk(u, 0.0)) {
		KNH_INITv(DP(u)->fvalue, new_FloatX__fast(ctx, cid, min));
	}
	else {
		KNH_INITv(DP(u)->fvalue, new_FloatX__fast(ctx, cid, 0.0));
	}
#endif
	knh_addSpecializedType(ctx, cid, CLASS_Float, u);
	return u;
}

/* ======================================================================== */
/* [String] */

static
knh_String_t *knh_fsnew__dict(Ctx *ctx, knh_class_t cid, knh_bytes_t t, knh_String_t *orign, int *foundError)
{
	knh_Semantics_t *u = knh_getSemantics(ctx, cid);
	knh_index_t n = knh_DictIdx_index(ctx, DP(u)->vocabDictIdx, t);
	if(n == -1) {
		knh_bytes_t tag;
		if(knh_bytes_splitTag(t, &tag, &t)) {
			n = knh_DictIdx_index(ctx, DP(u)->vocabDictIdx, t);
		}
	}
	if(n == -1) {
		*foundError = 1;
		return DP(u)->svalue;
	}
	return knh_DictIdx_get__fast(DP(u)->vocabDictIdx, n);
}

/* ------------------------------------------------------------------------ */

static
int knh_fscmp__dict(knh_Semantics_t *u, knh_bytes_t v1, knh_bytes_t v2)
{
	return knh_DictIdx_index(NULL, DP(u)->vocabDictIdx, v1) - knh_DictIdx_index(NULL, DP(u)->vocabDictIdx, v2);
}

/* ------------------------------------------------------------------------ */

static
knh_DictIdx_t* new_DictIdx__Array(Ctx *ctx, knh_Array_t *a)
{
	knh_DictIdx_t *o = (knh_DictIdx_t*)new_hObject(ctx, FLAG_DictIdx, CLASS_DictIdx, CLASS_DictIdx);
	KNH_INITv(o->terms, a);
	KNH_INITv(o->termsDictSet, new_DictSet(ctx, knh_Array_size(a)));
	o->offset = 0;
	{
		knh_uintptr_t i = 0;
		for(i = 0; i < knh_Array_size(a); i++) {
			knh_String_t *s = (knh_String_t*)knh_Array_n(a, i);
			knh_DictSet_append(ctx, o->termsDictSet, s, i+1);
		}
	}
	//knh_DictSet_toIgnoreCase(o->termsDictSet);
	return o;
}

/* ------------------------------------------------------------------------ */

static
MAPPER knh_Fmapper_vocabidx(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	knh_Semantics_t *u = knh_getSemantics(ctx, knh_Object_cid(sfp[0].o));
	knh_int_t n = knh_Semantics_getVocabIdx(ctx, u, sfp[0].s);
	DBG2_P("n = %d", (int)n);
	KNH_MAPPED_Int(ctx, sfp, n);
}

/* ------------------------------------------------------------------------ */

static
MAPPER knh_Fmapper_vocab(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	knh_Semantics_t *u = (knh_Semantics_t*)DP(sfp[1].mpr)->mapdata;
	size_t n = (size_t)(sfp[0].ivalue - DP(u)->imin);
	DBG2_P("n = %zd", n);
	knh_Array_t *a = (DP(u)->vocabDictIdx)->terms;
	Object *s = KNH_NULL;
	if(n < knh_Array_size(a)) {
		s = knh_Array_n(a, n);
	}
	KNH_MAPPED(ctx, sfp, s);
}

/* ------------------------------------------------------------------------ */

KNHAPI(knh_Semantics_t*) new_Vocab(Ctx *ctx, char *tag, knh_bytes_t urn, int base, char **terms)
{
	knh_class_t cid = new_ClassId(ctx);
	knh_Semantics_t* u = (knh_Semantics_t*)new_Object_bcid(ctx, CLASS_Semantics, (int)cid);
	DP(u)->ubcid = CLASS_String;
	KNH_SETv(ctx, DP(u)->urn, new_String(ctx, urn, NULL));
	if(tag != NULL || tag[0] != 0) {
		KNH_SETv(ctx, DP(u)->tag, T__(tag));
	}
	DP(u)->fsnew = knh_fsnew__dict;
	DP(u)->fscmp = knh_fscmp__dict;
	{
		knh_Array_t *a = new_Array0(ctx, 0);
		while(*terms != NULL) {
			knh_String_t *s = T__(*terms);
			knh_Array_add_(ctx, a, UP(s));
			s->h.cid = cid;
			terms++;
		}
		KNH_SETv(ctx, DP(u)->vocabDictIdx, new_DictIdx__Array(ctx, a));
		knh_Semantics_initIntRange(ctx, u, (knh_int_t)base, (knh_int_t)(base + knh_Array_size(a) - 1));
		KNH_ASSERT(knh_Array_size(a) > 0);
		KNH_INITv(DP(u)->svalue, knh_Array_n(a, 0));
	}
	knh_addSpecializedType(ctx, cid, CLASS_String, u);
	{
		knh_Mapper_t *mpr = new_Mapper(ctx, FLAG_Mapper_Total|FLAG_Mapper_Const|FLAG_Mapper_Final, cid, CLASS_Int, knh_Fmapper_vocabidx, (Object*)u);
		knh_addMapper(ctx, mpr);
		mpr = new_Mapper(ctx, FLAG_Mapper_Const|FLAG_Mapper_Final, CLASS_Int, cid, knh_Fmapper_vocab, (Object*)u);
		knh_addMapper(ctx, mpr);
	}
	return u;
}

///* ------------------------------------------------------------------------ */
//
//static
//MAPPER knh_Mapper__fdict(Ctx *ctx, knh_sfp_t *sfp METHODARG)
//{
//	String *s = sfp[0].s;
//	Semantics *u = knh_getSemantics(ctx, knh_Object_cid(sfp[0].o)].cspec;
//	KNH_ASSERT(IS_Semantics(u));
//	KNH_ASSERT(IS_DictIdx(DP(u)->vocabDictIdx));
//
//	Semantics *u2 = knh_getSemantics(ctx, DP(sfp[1].mpr)->tcid].cspec;
//	KNH_ASSERT(IS_Semantics(u2));
//	KNH_ASSERT(IS_DictIdx(DP(u2)->vocabDictIdx));
//
//	size_t n = (size_t)knh_DictIdx_index(ctx, DP(u)->vocabDictIdx, __tobytes(s));
//	KNH_ASSERT(n < knh_DictIdx_size(DP(u2)->vocabDictIdx));
//	KNH_MAPPED(ctx, sfp, knh_DictIdx_get__fast(DP(u2)->vocabDictIdx, n));
//}
//
///* ------------------------------------------------------------------------ */
//
//KNHAPI(void) knh_addVocabularyMapper(Ctx *ctx, knh_class_t scid, char *text)
//{
//	DBG2_ASSERT_cid(scid);
//	knh_class_t tcid = knh_findcid(ctx, B(text));
//
//	if(tcid != CLASS_unknown || ctx->share->ClassTable[tcid].bcid != tcid) {
////		Semantics *u = knh_getSemantics(ctx, scid].cspec;
////		if(!IS_Semantics(u) || !IS_DictIdx(DP(u)->vocabDictIdx)) {
////			TODO();
////			return;
////		}
//		Semantics *u2 = knh_getSemantics(ctx, tcid].cspec;
//		if(!IS_Semantics(u2) || !IS_DictIdx(DP(u2)->vocabDictIdx)) {
//			TODO();
//			return;
//		}
////		if(knh_DictIdx_size(DP(u)->vocabDictIdx) != knh_DictIdx_size(DP(u2)->vocabDictIdx)) {
////			TODO();
////			return;
////		}
//		Mapper *mpr = new_Mapper(ctx, FLAG_Mapper_Affine, scid, tcid,
//				knh_Mapper_fvocab, (Object*)KNH_NULL);
//		if(ctx->share->ClassTable[scid].cmap == NULL) {
//			knh_ClassTable_t *TC = (knh_ClassTable_t*)(&ctx->share->ClassTable[scid]);
//			KNH_INITv(TC->cmap, new_ClassMap0(ctx, 4));
//		}
//		knh_ClassMap_add(ctx, ctx->share->ClassTable[scid].cmap, mpr);
//
//		mpr = new_Mapper(ctx, FLAG_Mapper_Affine, tcid, scid,
//				knh_Mapper_fvocab, (Object*)KNH_NULL);
//		if(ctx->share->ClassTable[tcid].cmap == NULL) {
//			knh_ClassTable_t *TC = (knh_ClassTable_t*)(&ctx->share->ClassTable[tcid]);
//			KNH_INITv(TC->cmap, new_ClassMap0(ctx, 4));
//		}
//		knh_ClassMap_add(ctx, ctx->share->ClassTable[tcid].cmap, mpr);
//	}
//}

/* ======================================================================== */
/* [urnalias] */

knh_bytes_t knh_getURNAlias(Ctx *ctx, knh_bytes_t aurn)
{
	knh_String_t *s = (knh_String_t*)knh_DictMap_get__b(ctx,  DP(ctx->sys)->URNAliasDictMap, aurn);
	if(IS_NOTNULL(s)) {
		return __tobytes(s);
	}
	return aurn;
}

///* ------------------------------------------------------------------------ */
//
//void knh_setAliasURN(Ctx *ctx, String *alias, String *urn)
//{
//	if(knh_Context_isVerbose(ctx)) {
//		String *s =(String*)knh_DictMap_get__b(ctx,  DP(ctx->sys)->URNAliasDictMap, __tobytes(urn));
//		if(IS_NOTNULL(s)) {
//			KNH_WARNING(ctx, "Overriding %s %s", __tochar(alias), __tochar(s));
//		}
//	}
//	knh_DictMap_set(ctx, DP(ctx->sys)->URNAliasDictMap, alias, UP(urn));
//}

/* ------------------------------------------------------------------------ */

KNHAPI(void) knh_loadURNAliasData(Ctx *ctx, knh_StringData_t *data)
{
	knh_DictMap_t *map = DP(ctx->sys)->URNAliasDictMap;
	knh_StringData_t *d = data;
	while(d->name != NULL) {
		knh_String_t *s =(knh_String_t*)knh_DictMap_get__b(ctx,  map, B(d->name));
		if(IS_NOTNULL(s) && !knh_String_equals(s, B(d->value))) {
			KNH_WARNING(ctx, _("overriding alias %s %s as %s"), d->name, __tochar(s), d->value);
		}
		d++;
	}
	d = data;
	while(d->name != NULL) {
		knh_String_t *n = T__(d->name);
		knh_DictMap_append(ctx, map, n, UP(T__(d->value)));
		d++;
	}
}

/* ------------------------------------------------------------------------ */

KNHAPI(void) knh_loadSemanticsFuncData(Ctx *ctx, knh_NamedPointerData_t *data)
{
	knh_DictSet_t *ds = DP(ctx->sys)->SpecFuncDictSet;
	KNH_LOCK(ctx, LOCK_SYSTBL, NULL);
	while(data->name != NULL) {
		DBG2_P("adding.. '%s'", data->name);
		knh_String_t *n = T__(data->name);
		knh_DictSet_set(ctx, ds, n, (knh_uintptr_t)data->ptr);
		data++;
	}
	KNH_UNLOCK(ctx, LOCK_SYSTBL, NULL);
}

/* ------------------------------------------------------------------------ */

static
knh_Semantics_t *new_SemanticsNULL(Ctx *ctx, knh_bytes_t urn)
{
	knh_Semantics_t *u = NULL;
	KNH_LOCK(ctx, LOCK_SYSTBL, NULL);
	knh_index_t loc = 0;
	knh_bytes_t p = urn;
	while(loc != -1) {
		knh_fspec func = (knh_fspec)knh_DictSet_get__b(DP(ctx->sys)->SpecFuncDictSet, p);
		if(func != NULL) {
			u = func(ctx, urn);
			if(u != NULL) {
				KNH_SETv(ctx, DP(u)->urn, new_String(ctx, urn, NULL));
			}
			goto L_UNLOCK;
		}
		loc = knh_bytes_rindex(p, '/');
		if(loc != -1) {
			p = knh_bytes_first(p, loc);
		}
	}
L_UNLOCK:;
	KNH_UNLOCK(ctx, LOCK_SYSTBL, NULL);
	return u;
}

/* ======================================================================== */
/* [SPEC] */

knh_Semantics_t *knh_getSemantics(Ctx *ctx, knh_class_t cid)
{
	knh_Semantics_t *u = (knh_Semantics_t*)ClassTable(cid).cspec;
	KNH_ASSERT(IS_Semantics(u));
	return u;
}

/* ------------------------------------------------------------------------ */

static
Object *knh_ClassTable_fdefault__ISPEC(Ctx *ctx, knh_class_t cid)
{
	knh_Semantics_t *u = knh_getSemantics(ctx, cid);
	return UP(DP(u)->ivalue);
}

/* ------------------------------------------------------------------------ */

static
Object *knh_ClassTable_fdefault__FSPEC(Ctx *ctx, knh_class_t cid)
{
	knh_Semantics_t *u = knh_getSemantics(ctx, cid);
	return UP(DP(u)->fvalue);
}

/* ------------------------------------------------------------------------ */

static
Object *knh_ClassTable_fdefault__SSPEC(Ctx *ctx, knh_class_t cid)
{
	knh_Semantics_t *u = knh_getSemantics(ctx, cid);
	return UP(DP(u)->svalue);
}

/* ------------------------------------------------------------------------ */

static
knh_class_t knh_addSpecializedType(Ctx *ctx, knh_class_t cid, knh_class_t supcid, knh_Semantics_t *u)
{
	knh_class_t bcid = knh_class_bcid(supcid);
	char bufcn[CLASSNAME_BUFSIZ];
	if(cid == CLASS_newid) {
		cid = new_ClassId(ctx);
	}
	knh_snprintf(bufcn, sizeof(bufcn), KNH_SEMANTICS_FMT, CLASSN(bcid), __tochar(DP(u)->urn));
	knh_setClassName(ctx, cid, new_String(ctx, B(bufcn), NULL));
	{
		knh_ClassTable_t *t = pClassTable(ctx, cid);
		if((DP(u)->tag)->size > 0) {
			knh_snprintf(bufcn, sizeof(bufcn), "%s:%s", CLASSN(bcid), __tochar(DP(u)->tag));
			KNH_SETv(ctx, t->sname, new_String(ctx, B(bufcn), NULL));
		}
		DBG2_P("added %d, '%s'", cid, __tochar(t->lname));

		t->bcid   = bcid;
		t->supcid = bcid;
		t->cflag  = ClassTable(bcid).cflag | FLAG_Class_Final;
		t->oflag  = ClassTable(bcid).oflag;
		t->offset = ClassTable(bcid).offset;

		t->cspi  = ClassTable(bcid).cspi;
		t->size = ClassTable(bcid).size;
		t->bsize  = ClassTable(bcid).bsize;

		DBG2_ASSERT(t->fields == NULL);
		KNH_INITv(t->methods, ClassTable(supcid).methods);

		if(t->cmap == NULL) {
			KNH_INITv(t->cmap, knh_ClassMap_fdefault(ctx, CLASS_ClassMap));
		}
		else {
			KNH_ASSERT(IS_ClassMap(t->cmap));
		}

		DBG2_ASSERT(t->cspec == NULL);
		KNH_INITv(t->cspec, u);

		if(DP(u)->ucid != cid) {
			knh_Semantics_reuse(ctx, u, cid);
		}
		if(bcid == CLASS_Int) {
			KNH_ASSERT(DP(u)->ivalue != NULL);
			t->fdefault = knh_ClassTable_fdefault__ISPEC;
		}
		else if(bcid == CLASS_Float) {
			KNH_ASSERT(DP(u)->fvalue != NULL);
			t->fdefault = knh_ClassTable_fdefault__FSPEC;
		}
		else {
			KNH_ASSERT(bcid == CLASS_String);
			KNH_ASSERT(DP(u)->svalue != NULL);
			t->fdefault = knh_ClassTable_fdefault__SSPEC;
		}
	}
	return cid;
}

/* ------------------------------------------------------------------------ */

static
knh_Semantics_t* knh_findSemanticsNULL(Ctx *ctx, knh_bytes_t lname)
{
	knh_index_t loc = knh_bytes_index(lname, '{');
	if(loc != -1) {
		char *postfix = (char*)lname.buf + loc;
		char cname[CLASSNAME_BUFSIZ];
		knh_snprintf(cname, sizeof(cname), "Int%s", postfix);
		knh_class_t cid = knh_getcid(ctx, B(cname));
		if(cid != CLASS_unknown) {
			return knh_getSemantics(ctx, cid);
		}
		knh_snprintf(cname, sizeof(cname), "Float%s", postfix);
		cid = knh_getcid(ctx, B(cname));
		if(cid != CLASS_unknown) {
			return knh_getSemantics(ctx, cid);
		}
		knh_snprintf(cname, sizeof(cname), "String%s", postfix);
		cid = knh_getcid(ctx, B(cname));
		if(cid != CLASS_unknown) {
			return knh_getSemantics(ctx, cid);
		}
	}
	return NULL;
}

/* ------------------------------------------------------------------------ */

knh_class_t knh_findcidx(Ctx *ctx, knh_bytes_t lname)
{
	knh_Semantics_t *u = 	knh_findSemanticsNULL(ctx, lname);
	if(u != NULL) {
		knh_index_t loc = knh_bytes_index(lname, '{');
		knh_class_t bcid = knh_getcid(ctx, knh_bytes_first(lname, loc));
		return knh_addSpecializedType(ctx, CLASS_newid, bcid, u);
	}
	else {
		knh_index_t loc = knh_bytes_index(lname, '{');
		knh_bytes_t urn = knh_bytes_last(lname, loc+1); urn.len -= 1;
		knh_class_t bcid = knh_getcid(ctx, knh_bytes_first(lname, loc));
		DBG2_P("cid=%d,%s", bcid, CLASSN(bcid));
		u = new_SemanticsNULL(ctx, urn);
		if(u != NULL) {
			knh_class_t ucid = DP(u)->ucid;
			knh_class_t ubcid = DP(u)->ubcid;
			DBG2_P("cid=%d,%s", ubcid, CLASSN(ubcid));
			//knh_addSpecializedType(ctx, ucid, ubcid, u);
			if(ubcid != bcid) {
				return knh_addSpecializedType(ctx, CLASS_newid, bcid, u);
			}
			return ucid;
		}
		return bcid;
	}
}

/* ------------------------------------------------------------------------ */



#ifdef __cplusplus
}
#endif
