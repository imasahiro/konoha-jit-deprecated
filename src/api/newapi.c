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

#ifdef KNH_CC_METHODAPI

/* ------------------------------------------------------------------------ */
/* [new] */

//## @Virtual method This! Object.new();

static METHOD Object_new(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	DBG_P("DEFAULT CONSTRUCTOR? %s", CLASSNo(sfp[0].o));
	KNH_RETURN(ctx, sfp, sfp[0].o);
}

/* ------------------------------------------------------------------------ */

static
void knh_ObjectField_setValue(Ctx *ctx, knh_ObjectField_t *of, knh_index_t idx, knh_type_t type, Object *value)
{
	if(IS_NULL(value)) {
		if(IS_NNTYPE(type)) return;
		goto L_SET;
	}
	else {
		knh_class_t tcid = CLASS_type(type);
		knh_class_t scid = knh_Object_cid(value);
		DBG2_ASSERT_cid(tcid);
		DBG2_ASSERT_cid(scid);
		if(scid == tcid || knh_class_instanceof(ctx, scid, tcid)) {
			goto L_SET;
		}
		DBG2_P("COERCION %s -> %s", CLASSN(scid), CLASSN(tcid));
		TODO();
//		knh_sfp_t *lsfp = KNH_LOCAL(ctx);
//		KNH_LPUSH(ctx, o);
//		VM_MAP(ctx, tcid);
//		o = ctx->esp[0].o;
//		KNH_LOCALBACK(ctx, lsfp);
		return ;
	}

	L_SET:;
#ifdef KNH_USING_UNBOXFIELD
	if(IS_ubxint(type)) {
		knh_int_t *data = (knh_int_t*)(of->fields + idx);
		data[0] = toint(value);
	}
	else if(IS_ubxfloat(type)) {
		knh_float_t *data = (knh_float_t*)(of->fields + idx);
		data[0] = tofloat(value);
	}
	else if(IS_ubxboolean(type)) {
		knh_boolean_t *data = (knh_boolean_t*)(of->fields +idx);
		data[0] = tobool(value);
	}
	else
#endif/*KNH_USING_UNBOXFIELD*/
	{
		DBG2_ASSERT(of->fields[idx] != NULL);
		KNH_SETv(ctx, of->fields[idx], value);
	}
}

/* ------------------------------------------------------------------------ */
//## @Hidden method This! Object.new:dictmap(Any value, ...);

static METHOD Object_new__dictmap(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_ObjectField_t *of = (knh_ObjectField_t*)sfp[0].o;
	knh_class_t cid = knh_Object_cid(of);
	knh_sfp_t *v = sfp + 1;
	size_t i, ac = knh_stack_argc(ctx, v);
	for(i = 0; i < ac; i+= 2) {
		if(IS_bString(v[i].s)) {
			knh_fieldn_t fn = knh_getfnq(ctx, __tobytes(v[i].s), FIELDN_NONAME);
			if(fn == FIELDN_NONAME) continue;
			knh_index_t idx = knh_Class_queryField(ctx, cid, fn);
			if(idx == -1) continue;
			knh_cfield_t *cf = knh_Class_fieldAt(ctx, cid, idx);
			knh_type_t type = knh_pmztype_totype(ctx, cf->type, cid);
			if(type == TYPE_void) continue;
			DBG2_P("[%d] %s%s %s", (int)(idx), TYPEQN(type), __tochar(v[i].s));
			knh_ObjectField_setValue(ctx, of, idx, type, v[i+1].o);
		}
	}
	KNH_RETURN(ctx, sfp, of);
}

/* ------------------------------------------------------------------------ */
/* [Bytes] */
//## method This! Bytes.new(Int? init);

static METHOD Bytes_new(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Bytes_t *o = (knh_Bytes_t*)sfp[0].o;
	size_t init = IS_NULL(sfp[1].o) ? 0 : knh_bytes_newsize(p_size(sfp[1]));
	DBG2_ASSERT(o->capacity == 0);
	if(init < KNH_FASTMALLOC_SIZE) {
		init = KNH_FASTMALLOC_SIZE;
	}
	else if(init < KONOHA_SMALLPAGESIZE) {
		init = KONOHA_SMALLPAGESIZE;
	}
	o->buf = (knh_uchar_t*)KNH_MALLOC(ctx, init);
	knh_bzero(o->buf, init);
	o->capacity = init;
	KNH_RETURN(ctx, sfp, o);
}

/* ------------------------------------------------------------------------ */
//## method Bytes! Bytes.new:array(Int init);

static METHOD Bytes_new__array(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Bytes_t *o = (knh_Bytes_t*)sfp[0].o;
	size_t init, size = IS_NULL(sfp[1].o) ? 0 : knh_bytes_newsize(p_size(sfp[1]));
	DBG2_ASSERT(o->capacity == 0);
	if(size < KNH_FASTMALLOC_SIZE) {
		init = KNH_FASTMALLOC_SIZE;
	}
	else if(size < KONOHA_SMALLPAGESIZE) {
		init = KONOHA_SMALLPAGESIZE;
	}
	else {
		init = size;
	}
	o->buf = (knh_uchar_t*)KNH_MALLOC(ctx, init);
	knh_bzero(o->buf, init);
	o->capacity = init;
	o->size = size;
	KNH_RETURN(ctx, sfp, o);
}

/* ------------------------------------------------------------------------ */
//## method String! String.new(Bytes! buf, String? enc);

static METHOD String_new(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_String_t *s;
	if(IS_NULL(sfp[2].o)) {
		s = new_String(ctx, knh_Bytes_tobytes(sfp[1].ba), NULL);
	}
	else {
		knh_BytesConv_t *bc = new_BytesConv__in(ctx, __tochar(sfp[2].s));
		KNH_SETv(ctx, sfp[3].o, bc);
		s = new_String__bconv(ctx, knh_Bytes_tobytes(sfp[1].ba), bc);
	}
	KNH_RETURN(ctx, sfp, s);
}

/* ------------------------------------------------------------------------ */
//## @Const method Regex Regex.new(String! pattern, String? option);

static
METHOD Regex_new(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Regex_t *o = (knh_Regex_t*)sfp[0].o;
	knh_bytes_t p = __tobytes(sfp[1].s);
	knh_index_t loc = knh_bytes_index(p, ':');
	KNH_SETv(ctx, o->pattern, sfp[1].s);
	if(loc == -1) {
		o->df = knh_System_getRegexDriver(ctx, STEXT("re"));
	}
	else {
		o->df = knh_System_getRegexDriver(ctx, knh_bytes_first(p, loc));
	}
	o->reg = o->df->regmalloc(ctx);
	{
		char *ptn = (char*)(knh_bytes_last(p, loc+1).buf);
		char *opt = IS_NULL(sfp[2].o) ? "" : __tochar(sfp[2].s);
		o->df->regcomp(ctx, o->reg, ptn, opt);
	}
	KNH_RETURN(ctx, sfp, sfp[0].o);
}

/* ------------------------------------------------------------------------ */
/* [Pair, Tuple, Range] */
//## @Hidden @Const method This! Pair.new:init(T1 first, T2 second);

static METHOD Pair_new__init(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Pair_t *o = (knh_Pair_t*)sfp[0].o;
	knh_stack_boxing(ctx, sfp + 1);
	KNH_SETv(ctx, o->first, sfp[1].o);
	knh_stack_boxing(ctx, sfp + 2);
	KNH_SETv(ctx, o->second, sfp[2].o);
	KNH_RETURN(ctx, sfp, o);
}

/* ------------------------------------------------------------------------ */
//## @Hidden @Const method This! Tuple.new:init(Any? value, ...);

static METHOD Tuple_new__init(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Tuple_t *t = (knh_Tuple_t*)sfp[0].o;
	knh_sfp_t *v = sfp + 1;
	size_t i, ac = knh_stack_argc(ctx, v);
	for(i = 0; i < ac; i++) {
		knh_stack_boxing(ctx, v + i);
	}
	DBG2_ASSERT(t->size == 0);
	if(ac <= 3) {
		Object *tv = (ac < 1) ? KNH_NULL : v[0].o;
		knh_Tuple_setTriple(t, 1);
		KNH_INITv(t->first, tv);
		tv = (ac < 2) ? KNH_NULL : v[1].o;
		KNH_INITv(t->second, tv);
		tv = (ac < 3) ? KNH_NULL : v[2].o;
		KNH_INITv(t->third, tv);
	}
	else {
		t->size = ac;
		t->list = (knh_Object_t**)KNH_MALLOC(ctx, sizeof(void*) * t->size);
		for(i = 0; i < t->size; i++) {
			KNH_INITv(t->list[i], v[i].o);
		}
	}
	KNH_RETURN(ctx, sfp, t);
}

/* ------------------------------------------------------------------------ */
//## @Hidden @Const method This! Range.new:init(T1! start, T1! end);

static METHOD Range_new__init(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Range_t *r = (knh_Range_t*)sfp[0].o;
	knh_stack_boxing(ctx, sfp + 1);
	KNH_SETv(ctx, r->start, sfp[1].o);
	knh_stack_boxing(ctx, sfp + 2);
	KNH_SETv(ctx, r->end, sfp[2].o);
	KNH_RETURN(ctx, sfp, r);
}

/* ------------------------------------------------------------------------ */
//## @Hidden @Const method This! Range.new(T1! start, T1! end);

static METHOD Range_new(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	Range_new__init(ctx, sfp);
	knh_Range_setInclusive((knh_Range_t*)sfp[0].o, 1);
}

/* ------------------------------------------------------------------------ */
/* [Array] */
//## method This! Array.new(Int? initCapacity);

static METHOD Array_new(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Array_t *o = (knh_Array_t*)sfp[0].o;
	int init = IS_NULL(sfp[1].o) ? KNH_ARRAY_INITSIZE: p_int(sfp[1]);
	if(init > 0) {
		knh_Array_grow(ctx, o, init, KNH_NULL);
	}
	o->size = 0;
	KNH_RETURN(ctx, sfp, o);
}

/* ------------------------------------------------------------------------ */
//## @Hidden method This! Array.new:array(Int size);

static METHOD Array_new__array(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Array_t *o = (knh_Array_t*)sfp[0].o;
	knh_int_t init = sfp[1].ivalue;
	if(0 < init && init < LONG_MAX) {
		knh_class_t p1 = ctx->share->ClassTable[knh_Object_cid(o)].p1;
		Object *v = knh_getClassDefaultValue(ctx, p1);
		knh_Array_grow(ctx, o, init, v);
		o->size = init;
	}
	else if(init < 0) {
		KNH_THROW_OUTOFINDEX(ctx, init, 0);
	}
	KNH_RETURN(ctx, sfp, o);
}

/* ------------------------------------------------------------------------ */
//## @Hidden method This! Array.new:init(T1? value, ...);

static METHOD Array_new__init(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Array_t *o = (knh_Array_t*)sfp[0].o;
	knh_sfp_t *v = sfp + 1;
	int i, ac = knh_stack_argc(ctx, v);
	if(ac > 0) {
		knh_Array_grow(ctx, o, ac, KNH_NULL);
	}
	for(i = 0; i < ac; i++) {
		knh_stack_boxing(ctx, v + i);
		knh_Array_add(ctx, o, v[i].o);
	}
	KNH_RETURN(ctx, sfp, o);
}

/* ------------------------------------------------------------------------ */
//## @Hidden method This! IArray.new(Int? initCapacity);

static METHOD IArray_new(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_IArray_t *o = (knh_IArray_t*)sfp[0].o;
	int init = IS_NULL(sfp[1].o) ? KNH_IARRAY_INITSIZE: p_int(sfp[1]);
	if(init > 0) {
		knh_IArray_grow(ctx, o, init, 0);
	}
	o->size = 0;
	KNH_RETURN(ctx, sfp, o);
}

/* ------------------------------------------------------------------------ */
//## @Hidden method This! IArray.new:array(Int! size);

static METHOD IArray_new__array(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_IArray_t *o = (knh_IArray_t*)sfp[0].o;
	knh_int_t init = sfp[1].ivalue;
	if(0 < init && init < LONG_MAX) {
		knh_IArray_grow(ctx, o, init, 0);
		o->size = init;
	}
	else if(init < 0) {
		KNH_THROW_OUTOFINDEX(ctx, init, 0);
	}
	KNH_RETURN(ctx, sfp, o);
}

/* ------------------------------------------------------------------------ */
//## @Hidden method This! IArray.new:init(Int? value, ...);

static METHOD IArray_new__init(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_IArray_t *o = (knh_IArray_t*)sfp[0].o;
	knh_sfp_t *v = sfp + 1;
	int i, ac = knh_stack_argc(ctx, v);
	if(ac > 0) {
		knh_IArray_grow(ctx, o, ac, 0);
	}
	for(i = 0; i < ac; i++) {
		knh_IArray_add(ctx, o, v[i].ivalue);
	}
	KNH_RETURN(ctx, sfp, o);
}

/* ------------------------------------------------------------------------ */
//## @Hidden @Const method This! IArray.new:range(Int! start, Int! end, Int? isInclusive);

static METHOD IArray_new__range(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_IArray_t *o = (knh_IArray_t*)sfp[0].o;
	knh_int_t s = sfp[1].ivalue;
	knh_int_t e = sfp[2].ivalue;
	if(e < s) {
		s = e;
		e = sfp[1].ivalue;
	}

	knh_IArray_grow(ctx, o, e - s + 1, 0);
	for(; s < e; s++) {
		knh_IArray_add(ctx, o, s);
	}
	if(IS_NULL(sfp[3].o) || sfp[3].ivalue != 0) {
		knh_IArray_add(ctx, o, e);
	}
	KNH_RETURN(ctx, sfp, o);
}

/* ------------------------------------------------------------------------ */
//## @Hidden method This! FArray.new(Int? initCapacity);

static METHOD FArray_new(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_FArray_t *o = (knh_FArray_t*)sfp[0].o;
	long init = IS_NULL(sfp[1].o) ? KNH_FARRAY_INITSIZE: p_int(sfp[1]);
	if(init > 0) {
		knh_FArray_grow(ctx, o, init, KNH_FLOAT_ZERO);
	}
	o->size = 0;
	KNH_RETURN(ctx, sfp, o);
}

/* ------------------------------------------------------------------------ */
//## @Hidden method This! FArray.new:array(Int size);

static METHOD FArray_new__array(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_FArray_t *o = (knh_FArray_t*)sfp[0].o;
	knh_int_t init = sfp[1].ivalue;
	if(0 < init && init < LONG_MAX) {
		knh_FArray_grow(ctx, o, init, KNH_FLOAT_ZERO);
		o->size = init;
	}
	else if(init < 0) {
		KNH_THROW_OUTOFINDEX(ctx, init, 0);
	}
	KNH_RETURN(ctx, sfp, o);
}

/* ------------------------------------------------------------------------ */
//## @Hidden method This! FArray.new:init(Float? value, ...);

static METHOD FArray_new__init(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_FArray_t *o = (knh_FArray_t*)sfp[0].o;
	knh_sfp_t *v = sfp + 1;
	int i, ac = knh_stack_argc(ctx, v);
	if(ac > 0) {
		knh_FArray_grow(ctx, o, ac, KNH_FLOAT_ZERO);
	}
	for(i = 0; i < ac; i++) {
		knh_FArray_add(ctx, o, v[i].fvalue);
	}
	KNH_RETURN(ctx, sfp, o);
}

/* ------------------------------------------------------------------------ */
//## method This! DictMap.new(Int? initCapacity);

static METHOD DictMap_new(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_DictMap_t *o = (knh_DictMap_t*)sfp[0].o;
	size_t init = IS_NULL(sfp[1].o) ? 0: p_int(sfp[1]);
	if(init > knh_dict_capacity(o->_list)) {
		o->_list = knh_dict_resize(ctx, o->_list, init);
		o->size = 0;
	}
	KNH_RETURN(ctx, sfp, o);
}

/* ------------------------------------------------------------------------ */
//## @Hidden method This! DictMap.new:dictmap(T1 value, ...);

static METHOD DictMap_new__dictmap(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_DictMap_t *o = (knh_DictMap_t*)sfp[0].o;
	knh_sfp_t *v = sfp + 1;
	size_t i, ac = knh_stack_argc(ctx, v);
	for(i = 0; i < ac; i+=2) {
		if(IS_bString(v[i].s)) {
			knh_stack_boxing(ctx, v + i + 1);
			knh_DictMap_append(ctx, o, v[i].s, v[i+1].o);
		}
	}
	knh_DictMap_sort(o);
	KNH_RETURN(ctx, sfp, o);
}

/* ------------------------------------------------------------------------ */
//## method Exception! Exception.new(String? msg, Any? bag);

static METHOD Exception_new(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Exception_t *o = sfp[0].e;
	DP(o)->eid  = EXPT_Exception;
	DP(o)->flag = ctx->share->ExptTable[EXPT_Exception].flag;

	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_write_char(ctx, cwb->w, "Exception!!");
	if(IS_NOTNULL(sfp[1].o)) {
		knh_write_char(ctx, cwb->w, ": ");
		knh_write(ctx, cwb->w, __tobytes(sfp[1].s));
	}
	KNH_SETv(ctx, DP(o)->msg, knh_cwb_newString(ctx, cwb));
	KNH_SETv(ctx, DP(o)->bag, sfp[2].o);
	KNH_RETURN(ctx, sfp, o);
}

/* ------------------------------------------------------------------------ */
//## @Hidden method This! Exception.new:init(String? e, String? msg, Object? bag);

static METHOD Exception_new__init(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN(ctx, sfp,
		knh_Exception_new__init(ctx, sfp[0].e, sfp[1].s, sfp[2].s, sfp[3].o));
}


/* ------------------------------------------------------------------------ */
//## method This! Closure.new(Any! base, Method! method);

static METHOD Closure_new(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	DBG2_ASSERT(IS_bClosure(sfp[0].cc));
	DBG2_ASSERT(IS_bMethod(sfp[2].mtd));
	KNH_INITv((sfp[0].cc)->base, sfp[1].o);
	KNH_INITv((sfp[0].cc)->mtd, sfp[2].mtd);
	(sfp[0].cc)->envsfp = NULL;
	KNH_RETURN(ctx, sfp, sfp[0].o);
}

/* ------------------------------------------------------------------------ */
//## type void(Array) Thrd Closure void Array! void;
//## method Thread! Thread.new(Thrd! c, Array! args);

static METHOD Thread_new(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Thread_t *t = (knh_Thread_t*)sfp[0].o;
	DBG2_ASSERT(IS_bThread(t));
	DBG2_ASSERT(IS_bClosure(sfp[1].cc));
	DBG2_ASSERT(IS_Array(sfp[2].a));
	//KNH_INITv((sfp[0].cc)->base, sfp[1].o);
	//KNH_INITv((sfp[0].cc)->mtd, sfp[2].mtd);
	//(sfp[0].cc)->envsfp = NULL;
	KNH_RETURN(ctx, sfp, sfp[0].o);
}


/* ------------------------------------------------------------------------ */

#endif/* KNH_CC_METHODAPI*/

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
