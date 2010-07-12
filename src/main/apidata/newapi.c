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

#include"commons.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef K_USING_DEFAULTAPI

/* ------------------------------------------------------------------------ */
/* [new] */

//## @Virtual method This Object.new();

static METHOD Object_new(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	
	DBG_P("DEFAULT CONSTRUCTOR? %s", CLASSNo(sfp[0].o));
	RETURN_(sfp[0].o);
}

/* ------------------------------------------------------------------------ */

static void knh_ObjectField_setValue(Ctx *ctx, knh_ObjectField_t *of, knh_index_t idx, knh_type_t type, Object *value)
{
	knh_class_t tcid = CLASS_type(type);
	knh_class_t scid = knh_Object_cid(value);
	DBG_ASSERT_cid(tcid);
	DBG_ASSERT_cid(scid);
	if(scid == tcid || knh_class_instanceof(ctx, scid, tcid)) {
		goto L_SETVAL;
	}
	DBG_P("COERCION %s -> %s", CLASSN(scid), CLASSN(tcid));
	TODO();
//		BEGIN_LOCAL(ctx, lsfp);
//		KNH_LPUSH(ctx, o);
//		VM_MAP(ctx, tcid);
//		o = ctx->esp[0].o;
//		END_LOCAL(ctx, lsfp);
	return ;

	L_SETVAL:;
	if(IS_Tint(type)) {
		knh_int_t *data = (knh_int_t*)(of->fields + idx);
		data[0] = N_toint(value);
	}
	else if(IS_Tfloat(type)) {
		knh_float_t *data = (knh_float_t*)(of->fields + idx);
		data[0] = N_tofloat(value);
	}
	else if(IS_Tbool(type)) {
		knh_boolean_t *data = (knh_boolean_t*)(of->fields +idx);
		data[0] = N_tobool(value);
	}
	else {
		DBG_ASSERT(of->fields[idx] != NULL);
		KNH_SETv(ctx, of->fields[idx], value);
	}
}

/* ------------------------------------------------------------------------ */
//## @Hidden method This Object.new:MAP(Any value, ...);

static METHOD Object_new__MAP(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_ObjectField_t *of = (knh_ObjectField_t*)sfp[0].o;
	knh_class_t cid = knh_Object_cid(of);
	knh_sfp_t *v = sfp + 1;
	size_t i, ac = knh_stack_argc(ctx, v);
	for(i = 0; i < ac; i+= 2) {
		if(IS_bString(v[i].s)) {
			knh_fieldn_t fn = knh_getfnq(ctx, S_tobytes(v[i].s), FN_NONAME);
			if(fn == FN_NONAME) continue;
			knh_index_t idx = knh_Class_queryField(ctx, cid, fn);
			if(idx == -1) continue;
			knh_fields_t *cf = knh_Class_fieldAt(ctx, cid, idx);
			knh_type_t type = knh_type_tocid(ctx, cf->type, cid);
			if(type == TYPE_void) continue;
			DBG_P("[%d] %s %s", (int)(idx), TYPEN(type), S_tochar(v[i].s));
			knh_ObjectField_setValue(ctx, of, idx, type, v[i+1].o);
		}
	}
	RETURN_(of);
}

/* ------------------------------------------------------------------------ */
/* [Bytes] */
//## method Bytes Bytes.new(Int init);

static METHOD Bytes_new(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Bytes_t *ba = sfp[0].ba;
	size_t init = sfp[1].ivalue <= 0 ? 0 : knh_good_size(Int_to(size_t, sfp[1]));
	DBG_ASSERT(ba->capacity == 0);
	if(init < K_FASTMALLOC_SIZE) {
		init = K_FASTMALLOC_SIZE;
	}
	else if(init < K_SMALLPAGESIZE) {
		init = K_SMALLPAGESIZE;
	}
	ba->bu.ubuf = (knh_uchar_t*)KNH_MALLOC(ctx, init);
	knh_bzero(ba->bu.ubuf, init);
	ba->capacity = init;
	RETURN_(ba);
}

/* ------------------------------------------------------------------------ */
//## method Bytes Bytes.new:ARRAY(Int init);

static METHOD Bytes_new__ARRAY(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	size_t size = sfp[1].ivalue <= 0 ? 0 : knh_good_size(Int_to(size_t, sfp[1]));
	Bytes_new(ctx, sfp, rix);
	sfp[rix].ba->bu.len = size;
}

/* ------------------------------------------------------------------------ */
//## method String String.new(Bytes buf, StringDecoder c);

static METHOD String_new(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_String_t *s;
	if(IS_NULL(sfp[2].o)) {
		s = new_S(ctx, BA_tobytes(sfp[1].ba));
	}
	else {
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		knh_StringDecoder_t *c = (knh_StringDecoder_t*)sfp[2].o;
		c->dspi->dec(ctx, c->conv, BA_tobytes(sfp[1].ba), cwb->ba);
		s = knh_cwb_newString(ctx, cwb);
	}
	RETURN_(s);
}

/* ------------------------------------------------------------------------ */
//## @Const method Regex Regex.new(String pattern, String option, NameSpace ns);

static METHOD Regex_new(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Regex_t *re = (knh_Regex_t*)sfp[0].o;
	const char *ptn = ctx->api->tochar(ctx, sfp[1].s);
	const char *opt = IS_NULL(sfp[2].o) ? "" : ctx->api->tochar(ctx, sfp[2].s);
	KNH_SETv(ctx, re->pattern, sfp[1].s);
	re->spi = DP(sfp[2].ns)->regexSPI;
	re->reg = re->spi->regmalloc(ctx, sfp[1].s);
	re->spi->regcomp(ctx, re->reg, ptn, re->spi->parse_cflags(ctx, opt));
	re->eflags = re->spi->parse_eflags(ctx, opt);
	RETURN_(sfp[0].o);
}

/* ------------------------------------------------------------------------ */
//## @Const mapper String Regex!;

static METHOD String_Regex(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Regex_t *re = new_(Regex);
	const char *ptn = ctx->api->tochar(ctx, sfp[0].s);
	KNH_SETv(ctx, re->pattern, sfp[0].s);
	re->reg = re->spi->regmalloc(ctx, sfp[0].s);
	re->spi->regcomp(ctx, re->reg, ptn, 0);
	re->eflags = 0;
	RETURN_(re);
}



/* ------------------------------------------------------------------------ */
/* [Pair, Tuple, Range] */

//## @Hidden @Const method This Tuple.new:TUPLE(T1 k, T2 v, Any value, ...);

static METHOD Tuple_new__TUPLE(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Tuple_t *t = (knh_Tuple_t*)sfp[0].o;
	knh_sfp_t *v = sfp + 1;
	size_t i, ac = knh_stack_argc(ctx, v);
	for(i = 0; i < ac; i++) {
		knh_stack_boxing(ctx, v + i);
	}
	DBG_ASSERT(t->size == 0);
	if(ac <= 3) {
		KNH_INITv(t->first, v[0].o);
		KNH_INITv(t->second, v[1].o);
		if(ac == 3) {
			KNH_INITv(t->thirdNULL, v[2].o);
		}
		else {
			t->thirdNULL = NULL;
		}
	}
	else {
		t->size = ac;
		t->list = (knh_Object_t**)KNH_MALLOC(ctx, sizeof(void*) * t->size);
		for(i = 0; i < t->size; i++) {
			KNH_INITv(t->list[i], v[i].o);
		}
	}
	RETURN_(t);
}

/* ------------------------------------------------------------------------ */
//## @Hidden @Const method This Range.new:LIST(T1! start, T1! end);

static METHOD Range_new__LIST(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	
	knh_Range_t *r = (knh_Range_t*)sfp[0].o;
	knh_stack_boxing(ctx, sfp + 1);
	KNH_SETv(ctx, r->start, sfp[1].o);
	knh_stack_boxing(ctx, sfp + 2);
	KNH_SETv(ctx, r->end, sfp[2].o);
	RETURN_(r);
}

/* ------------------------------------------------------------------------ */
//## @Hidden @Const method This Range.new:RANGE(T1! start, T1! end);

static METHOD Range_new__RANGE(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	Range_new__LIST(ctx, sfp, rix);
	knh_Range_setInclusive((knh_Range_t*)sfp[0].o, 1);
}

/* ------------------------------------------------------------------------ */
/* [Array] */

//## method This Array.new(Int initCapacity);

static METHOD Array_new(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Array_grow(ctx, sfp[0].a, Int_to(size_t, sfp[1]), 0);
	DBG_ASSERT((sfp[0].a)->size == 0);
	RETURN_(sfp[0].a);
}

/* ------------------------------------------------------------------------ */
//## @Hidden method This Array.new:ARRAY(Int size);

static METHOD Array_new__ARRAY(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Array_t *a = sfp[0].a;
	knh_int_t init = sfp[1].ivalue;
	if(0 <= init && init < LONG_MAX) {
		knh_Array_grow(ctx, a, (size_t)init, 0);
		a->size = (size_t)init;
		if(!knh_Array_isNDATA(a)) {
			knh_class_t p1 = knh_Object_p1(a);
			Object *v = knh_getClassDefaultValue(ctx, p1);
			size_t i;
			for(i = 0; i < a->size; i++) {
				KNH_INITv(a->list[i], v);
			}
		}
	}
	else if(init < 0) {
		KNH_THROW_OUTOFINDEX(ctx, init, 0);
	}
	RETURN_(a);
}

/* ------------------------------------------------------------------------ */
//## @Hidden method This Array.new:LIST(T1 value, ...);

static METHOD Array_new__LIST(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Array_t *a = sfp[0].a;
	a->api->add(ctx, a, sfp+1);
	RETURN_(a);
}

/* ------------------------------------------------------------------------ */
//## method This Map.new(String path);

static METHOD Map_new(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	KNH_TODO(__FUNCTION__);
//	knh_DictMap_t *o = (knh_DictMap_t*)sfp[0].o;
//	size_t init = sfp[1].ivalue <= 0 ? 0: Int_to(size_t, sfp[1]);
//	if(init > knh_dict_capacity(o->_list)) {
//		o->_list = knh_dict_resize(ctx, o->_list, init);
//		o->size = 0;
//	}
//	RETURN_(o);
}

/* ------------------------------------------------------------------------ */
//## @Hidden method This Map.new:MAP(T1 value, ...);

static METHOD Map_new__MAP(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	KNH_TODO(__FUNCTION__);
//	knh_DictMap_t *o = (knh_DictMap_t*)sfp[0].o;
//	knh_sfp_t *v = sfp + 1;
//	size_t i, ac = knh_stack_argc(ctx, v);
//	for(i = 0; i < ac; i+=2) {
//		if(IS_bString(v[i].s)) {
//			knh_stack_boxing(ctx, v + i + 1);
//			knh_DictMap_append(ctx, o, v[i].s, v[i+1].o);
//		}
//	}
//	knh_DictMap_sort(o);
//	RETURN_(o);
}

/* ------------------------------------------------------------------------ */
//## method Exception! Exception.new(String msg, Any bag);

static METHOD Exception_new(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	TODO();DBG_ABORT();
//	
//	KNH_RETURN(ctx, sfp,
//		knh_Exception_new__init(ctx, sfp[0].e, sfp[1].s, sfp[2].s, sfp[3].o));
//
//	
//	knh_Exception_t *o = sfp[0].e;
//	DP(o)->eid  = EXPT_Exception;
//	DP(o)->flag = ctx->share->ExptTable[EXPT_Exception].flag;
//
//	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
//	knh_write_char(ctx, cwb->w, "Exception!!");
//	if(IS_NOTNULL(sfp[1].o)) {
//		knh_write_char(ctx, cwb->w, ": ");
//		knh_write(ctx, cwb->w, S_tobytes(sfp[1].s));
//	}
//	KNH_SETv(ctx, DP(o)->msg, knh_cwb_newString(ctx, cwb));
//	KNH_SETv(ctx, DP(o)->bag, sfp[2].o);
//	RETURN_(o);
}

/* ------------------------------------------------------------------------ */
//## method This Func.new(Any! base, Method! method);

static METHOD Func_new(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Func_t   *fo = sfp[0].fo;
	knh_Method_t *mtd = sfp[2].mtdOBJ;
	KNH_SETv(ctx, (fo)->mtd, sfp[2].mtdOBJ);
	if(!knh_Method_isStatic(mtd)) {
		KNH_INITv((fo)->baseNULL, sfp[1].o);
	}
	//(sfp[0].cc)->envsfp = NULL;
	RETURN_(fo);
}

/* ------------------------------------------------------------------------ */

#endif/* K_USING_DEFAULTAPI*/

#ifdef __cplusplus
}
#endif
