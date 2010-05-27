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

/* ======================================================================== */
/* [Bytes, byte[]] */

/* ------------------------------------------------------------------------ */
//## method void Bytes.putc(Int! char);

static METHOD Bytes_putc(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_Bytes_t *o = (knh_Bytes_t*)sfp[0].o;
	knh_Bytes_putc(ctx, o, p_int(sfp[1]));
	KNH_RETURN_void(ctx, sfp);
}


/* ------------------------------------------------------------------------ */
//## method void Bytes.write(Bytes! buf, Int? offset, Int? length);
//## method void Bytes.add(Bytes! buf, Int? offset, Int? length);

static METHOD Bytes_write(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_Bytes_t *o = (knh_Bytes_t*)sfp[0].o;
	knh_bytes_t t = knh_Bytes_tobytes(sfp[1].ba);
	if(IS_NOTNULL(sfp[2].o)) {
		size_t n = knh_array_index(ctx, p_int(sfp[2]), t.len);
		t = knh_bytes_last(t, n);
	}
	if(IS_NOTNULL(sfp[3].o)) {
		size_t l = p_int(sfp[3]);
		if(l < t.len) {
			t.len = l;
		}
	}
	knh_Bytes_write(ctx, o, t);
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## method void Bytes.memcpy(Int offset, Bytes! buf, Int? offset, Int? length);

static METHOD Bytes_memcpy(Ctx *ctx, knh_sfp_t *sfp)
{
	/*
	knh_Bytes_t *d = (knh_Bytes_t*)sfp[0].o;
	size_t doff = IS_NULL(sfp[1].o) ? 0 : p_int(sfp[1]);
	knh_Bytes_t *s = (knh_Bytes_t*)sfp[2].o;
	size_t soff = IS_NULL(sfp[3].o) ? 0 : p_int(sfp[3]);
	size_t slen = IS_NULL(sfp[4].o) ? 0 : ((s)->size - soff);
	if(doff + slen < (d)->size) {
		knh_memcpy(d->buf + doff, s->buf + soff, slen);
	}
	else {
	KNH_THROW_OUTOFINDEX(ctx, doff + slen, (d)->size);
	}
	KNH_RETURN_void(ctx, sfp);
	*/
	knh_Bytes_t *d = (knh_Bytes_t*)sfp[0].o;
	size_t doff = IS_NULL(sfp[1].o) ? 0 : p_int(sfp[1]);
	knh_Bytes_t *s = (knh_Bytes_t*)sfp[2].o;
	/* TODO
	 there is still buggy.
	 when sfp[3] is negative, soff is strange value.
	 e.g. sfp[3].ivalue = -1, soff = 4294967295; */
	size_t soff = IS_NULL(sfp[3].o) ? 0 : p_int(sfp[3]);
	size_t slen = s->size - soff;
	if(IS_NOTNULL(sfp[4].o) && p_int(sfp[4]) < s->size && p_int(sfp[4]) > 0) {
		slen = p_int(sfp[4]);
	}
	if(doff + slen < d->capacity) {
		knh_memcpy(d->buf + doff, s->buf + soff, slen);
		d->size += slen;
	}
	else {
		KNH_THROW_OUTOFINDEX(ctx, doff + slen, d->size);
	}
	KNH_RETURN_void(ctx, sfp);

}

/* ======================================================================== */
/* [Array, IArray, FArray] */

/* ------------------------------------------------------------------------ */
//## method void Array.add(T1 value);

static METHOD Array_add(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_Array_t *o = (knh_Array_t*)sfp[0].o;
	if(o->size == o->capacity) {
		knh_Array_grow(ctx, o, knh_array_newsize(o->capacity * 2, sizeof(Object*)), KNH_NULL);
	}
	knh_stack_boxing(ctx, sfp + 1);
	KNH_SETv(ctx, o->list[o->size], sfp[1].o);
	o->size++;
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## method void IArray.add(Int! value);

static METHOD IArray_add(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_IArray_t *o = (knh_IArray_t*)sfp[0].o;
	if(o->size == o->capacity) {
		knh_IArray_grow(ctx, o, knh_array_newsize(o->capacity * 2, sizeof(knh_int_t)), 0);
	}
	o->ilist[o->size] = sfp[1].ivalue;
	o->size++;
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## method void FArray.add(Float! value);

static METHOD FArray_add(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_FArray_t *o = (knh_FArray_t*)sfp[0].o;
	if(o->size == o->capacity) {
		knh_FArray_grow(ctx, o, knh_array_newsize(o->capacity*2, sizeof(knh_float_t)), KNH_FLOAT_ZERO);
	}
	o->flist[o->size] = sfp[1].fvalue;
	o->size++;
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## method void Array.opAppend(T1 value, ...);

static METHOD Array_opAppend(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_Array_t *o = (knh_Array_t*)sfp[0].o;
	knh_sfp_t *v = sfp + 1;
	int i, ac = knh_stack_argc(ctx, v);
	for(i = 0; i < ac; i++) {
		knh_stack_boxing(ctx, v + i);
		knh_Array_add(ctx, o, v[i].o);
	}
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## method void IArray.opAppend(Int! value, ...);

static METHOD IArray_opAppend(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_IArray_t *o = (knh_IArray_t*)sfp[0].o;
	knh_sfp_t *v = sfp + 1;
	int i, ac = knh_stack_argc(ctx, v);
	for(i = 0; i < ac; i++) {
		knh_IArray_add(ctx, o, v[i].ivalue);
	}
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## method void FArray.opAppend(Float! value, ...);

static METHOD FArray_opAppend(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_FArray_t *o = (knh_FArray_t*)sfp[0].o;
	knh_sfp_t *v = sfp + 1;
	int i, ac = knh_stack_argc(ctx, v);
	for(i = 0; i < ac; i++) {
		knh_FArray_add(ctx, o, v[i].fvalue);
	}
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## method void Array.insert(Int! n, T1! value);

static METHOD Array_insert(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_Array_t *o = (knh_Array_t*)sfp[0].o;
	size_t i, n = knh_array_index(ctx, p_int(sfp[1]), o->size);
	Object *temp;
	if(o->size == o->capacity) {
		knh_Array_grow(ctx, o, knh_array_newsize(o->capacity * 2, sizeof(Object*)), KNH_NULL);
	}
	temp = o->list[o->size];
	o->size++;
	for(i = o->size - 1; i > n ; i--) {
		o->list[i] = o->list[i-1];
	}
	o->list[n] = temp;
	knh_stack_boxing(ctx, sfp + 2);
	KNH_SETv(ctx, o->list[n], sfp[2].o);
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## method void IArray.insert(Int! n, Int! value);

static METHOD IArray_insert(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_IArray_t *o = sfp[0].ia;
	size_t i, n = knh_array_index(ctx, p_int(sfp[1]), o->size);
	knh_int_t temp;
	if(o->size == o->capacity) {
		knh_IArray_grow(ctx, o, knh_array_newsize(o->capacity * 2, sizeof(knh_int_t)), 0);
	}
	temp = o->ilist[o->size];
	o->size++;
	for(i = o->size - 1; i > n ; i--) {
		o->ilist[i] = o->ilist[i-1];
	}
	o->ilist[n] = temp;
	o->ilist[n] = sfp[2].fvalue;
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## method void FArray.insert(Int! n, Float! value);

static METHOD FArray_insert(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_FArray_t *o = sfp[0].fa;
	size_t i, n = knh_array_index(ctx, p_int(sfp[1]), o->size);
	knh_float_t temp;
	if(o->size == o->capacity) {
#if !defined(KNH_USING_NOFLOT)
		knh_FArray_grow(ctx, o, knh_array_newsize(o->capacity * 2, sizeof(knh_float_t)), 0.0);
#else
		knh_FArray_grow(ctx, o, knh_array_newsize(o->capacity * 2, sizeof(knh_float_t)), 0);
#endif
	}
	temp = o->flist[o->size];
	o->size++;
	for(i = o->size - 1; i > n ; i--) {
		o->flist[i] = o->flist[i-1];
	}
	o->flist[n] = temp;
	o->flist[n] = sfp[2].fvalue;
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## method void Array.clear();

static
METHOD Array_clear(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_Array_clear(ctx, sfp[0].a);
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## method void IArray.clear();

static
METHOD IArray_clear(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_IArray_t *o = sfp[0].ia;
	size_t i;
	for(i = 0; i < o->size; i++) {
		o->ilist[i] = 0;
	}
	o->size = 0;
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## method void FArray.clear();

static
METHOD FArray_clear(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_FArray_t *o = sfp[0].fa;
	size_t i;
	for(i = 0; i < o->size; i++) {
		o->flist[i] = 0;
	}
	o->size = 0;
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## method void Array.remove(Int! n);

static METHOD Array_remove(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_Array_t *o = (knh_Array_t*)sfp[0].o;
	size_t n = knh_array_index(ctx, p_int(sfp[1]), o->size);
	knh_Array_remove(ctx, o, n);
	KNH_RETURN_void(ctx,sfp);
}

/* ------------------------------------------------------------------------ */
//## method void IArray.remove(Int! n);

static METHOD IArray_remove(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_IArray_t *o = sfp[0].ia;
	size_t i, n = knh_array_index(ctx, p_int(sfp[1]), o->size);
	for(i = n; i < o->size - 1; i++) {
		o->ilist[i] = o->ilist[i+1];
	}
	o->size--;
	KNH_RETURN_void(ctx,sfp);
}

/* ------------------------------------------------------------------------ */
//## method void FArray.remove(Int! n);

static METHOD FArray_remove(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_FArray_t *o = sfp[0].fa;
	size_t i, n = knh_array_index(ctx, p_int(sfp[1]), o->size);
	for(i = n; i < o->size - 1; i++) {
		o->flist[i] = o->flist[i+1];
	}
	o->size--;
	KNH_RETURN_void(ctx,sfp);
}

/* ------------------------------------------------------------------------ */
//## method T1! Array.pop();

static METHOD Array_pop(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_Array_t *o = (knh_Array_t*)sfp[0].o;
	if(o->size > 0) {
		o->size--;
		KNH_RETURN(ctx, sfp, o->list[o->size]);
	} else {
		KNH_THROW_OUTOFINDEX(ctx, 0, 0);
	}
}

/* ------------------------------------------------------------------------ */
//## method T1! IArray.pop();

static METHOD IArray_pop(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_IArray_t *o = sfp[0].ia;
	if(o->size > 0) {
		o->size--;
		KNH_RETURN_Int(ctx, sfp, o->ilist[o->size]);
	} else {
		KNH_THROW_OUTOFINDEX(ctx, 0, 0);
	}
}

/* ------------------------------------------------------------------------ */
//## method T1! FArray.pop();

static METHOD FArray_pop(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_FArray_t *o = sfp[0].fa;
	if(o->size > 0) {
		o->size--;
		KNH_RETURN_Int(ctx, sfp, o->flist[o->size]);
	} else {
		KNH_THROW_OUTOFINDEX(ctx, 0, 0);
	}
}

/* ------------------------------------------------------------------------ */
//## method Int! Array.indexOf(T1 value);

static METHOD Array_indexOf(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_Array_t *o = (knh_Array_t*)sfp[0].o;
	knh_int_t res = -1;
	size_t i;
	knh_stack_boxing(ctx, sfp + 1);
	for(i = 0; i < o->size; i++) {
		if(knh_Object_compareTo(ctx, o->list[i], sfp[1].o) == 0) {
			res = i; break;
		}
	}
	KNH_RETURN_Int(ctx, sfp, res);
}

/* ------------------------------------------------------------------------ */
//## method Int! IArray.indexOf(Int! value);

static METHOD IArray_indexOf(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_IArray_t *o = sfp[0].ia;
	size_t i;
	knh_int_t res = -1;
	for(i = 0; i < o->size; i++) {
		if(o->ilist[i] == sfp[1].ivalue) {
			res = i; break;
		}
	}
	KNH_RETURN_Int(ctx, sfp, res);
}

/* ------------------------------------------------------------------------ */
//## method Int! FArray.indexOf(Float! value);

static METHOD FArray_indexOf(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_FArray_t *o = sfp[0].fa;
	size_t i;
	knh_int_t res = -1;
	for(i = 0; i < o->size; i++) {
		if(o->flist[i] == sfp[1].fvalue) {
			res = i; break;
		}
	}
	KNH_RETURN_Int(ctx, sfp, res);
}

/* ------------------------------------------------------------------------ */
//## method Int! Array.lastIndexOf(T1 value);

static METHOD Array_lastIndexOf(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_int_t res = -1;
	knh_Array_t *o = (knh_Array_t*)sfp[0].o;
	long i;
	knh_stack_boxing(ctx, sfp + 1);
	for(i = o->size - 1; i >= 0; i--) {
		if(knh_Object_compareTo(ctx, o->list[i], sfp[1].o) == 0) {
			res = i; break;
		}
	}
	KNH_RETURN_Int(ctx, sfp, res);
}

/* ------------------------------------------------------------------------ */
//## method Int! IArray.lastIndexOf(Int! value);

static METHOD IArray_lastIndexOf(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_int_t res = -1;
	knh_IArray_t *o = sfp[0].ia;
	long i;
	for(i = o->size - 1; i >= 0; i--) {
		if(o->ilist[i] == sfp[1].ivalue) {
			res = i; break;
		}
	}
	KNH_RETURN_Int(ctx, sfp, res);
}

/* ------------------------------------------------------------------------ */
//## method Int! FArray.lastIndexOf(Float! value);

static METHOD FArray_lastIndexOf(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_int_t res = -1;
	knh_FArray_t *o = sfp[0].fa;
	long i;
	for(i = o->size - 1; i >= 0; i--) {
		if(o->flist[i] == sfp[1].fvalue) {
			res = i; break;
		}
	}
	KNH_RETURN_Int(ctx, sfp, res);
}


/* ======================================================================== */
/* [Collections] */

typedef struct {
	Ctx *ctx;
	knh_sfp_t *sfp;
} knh_env_t;

static int knh_env_comp(knh_env_t *env, Object **a1, Object **a2)
{
	Ctx *ctx = env->ctx;
	knh_sfp_t *lsfp = env->sfp + 2;
	knh_putsfp(ctx, lsfp, 2, a1[0]);
	knh_putsfp(ctx, lsfp, 3, a2[0]);
	knh_Closure_invokesfp(ctx, env->sfp[1].cc, lsfp, 2);
	return (int)lsfp[0].ivalue;
}

/* ------------------------------------------------------------------------ */
//## method void Array.sort(Cmpr? c);

static METHOD Array_sort(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_Array_t *o = sfp[0].a;
	if(IS_NULL(sfp[1].o)) {
		knh_qsort_r(o->list, o->size, sizeof(Object*),
			(int (*)(void*, const void*, const void*))knh_Object_compareTo2, (void*)ctx);
	}
	//else {
	//	knh_env_t env = {ctx, sfp};
	//	knh_qsort_r(o->list, o->size, sizeof(Object*), &env,
	//			(int (*)(void *, const void* , const void*))knh_env_comp);
	//}
}

/* ------------------------------------------------------------------------ */

static int qsort_icmp(const void* ap, const void* bp)
{
	knh_int_t a = *((knh_int_t*)ap);
	knh_int_t b = *((knh_int_t*)bp);
	if(a < b) return -1;
	if(a > b) return 1;
	return 0;
}

/* ------------------------------------------------------------------------ */
//## method void IArray.sort();

static METHOD IArray_sort(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_IArray_t *o = (knh_IArray_t*)sfp[0].o;
	knh_qsort(o->ilist, o->size, sizeof(knh_int_t), qsort_icmp);
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */

static int qsort_fcmp(const void* ap, const void* bp)
{
	knh_float_t a = *((knh_float_t*)ap);
	knh_float_t b = *((knh_float_t*)bp);
	if(a < b) return -1;
	if(a > b) return 1;
	return 0;
}

/* ------------------------------------------------------------------------ */
//## method void FArray.sort();

static METHOD FArray_sort(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_FArray_t *o = (knh_FArray_t*)sfp[0].o;
	knh_qsort(o->flist, o->size, sizeof(knh_float_t), qsort_fcmp);
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## method void Array.reverse();

static METHOD Array_reverse(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_Array_t *o = (knh_Array_t*)sfp[0].o;
	size_t i;
	for(i = 0; i < o->size / 2; i++) {
		size_t n = o->size - i - 1;
		Object *temp = o->list[i];
		o->list[i] = o->list[n];
		o->list[n] = temp;
	}
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## method void IArray.reverse();

static METHOD IArray_reverse(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_IArray_t *o = (knh_IArray_t*)sfp[0].o;
	size_t i;
	for(i = 0; i < o->size / 2; i++) {
		size_t n = o->size - i - 1;
		knh_int_t temp = o->ilist[i];
		o->ilist[i] = o->ilist[n];
		o->ilist[n] = temp;
	}
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## method void FArray.reverse();

static METHOD FArray_reverse(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_FArray_t *o = (knh_FArray_t*)sfp[0].o;
	size_t i;
	for(i = 0; i < o->size / 2; i++) {
		size_t n = o->size - i - 1;
		knh_float_t temp = o->flist[i];
		o->flist[i] = o->flist[n];
		o->flist[n] = temp;
	}
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## method void Array.swap(Int! m, Int! n);

static METHOD Array_swap(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_Array_t *o = (knh_Array_t*)sfp[0].o;
	size_t m = knh_array_index(ctx, p_int(sfp[1]), o->size);
	size_t n = knh_array_index(ctx, p_int(sfp[2]), o->size);
	Object *temp = o->list[n];
	o->list[n] = o->list[m];
	o->list[m] = temp;
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## method void IArray.swap(Int! m, Int! n);

static METHOD IArray_swap(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_IArray_t *o = (knh_IArray_t*)sfp[0].o;
	size_t m = knh_array_index(ctx, p_int(sfp[1]), o->size);
	size_t n = knh_array_index(ctx, p_int(sfp[2]), o->size);
	knh_int_t temp = o->ilist[n];
	o->ilist[n] = o->ilist[m];
	o->ilist[m] = temp;
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## method void FArray.swap(Int! m, Int! n);

static METHOD FArray_swap(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_FArray_t *o = (knh_FArray_t*)sfp[0].o;
	size_t m = knh_array_index(ctx, p_int(sfp[1]), o->size);
	size_t n = knh_array_index(ctx, p_int(sfp[2]), o->size);
	knh_float_t temp = o->flist[n];
	o->flist[n] = o->flist[m];
	o->flist[m] = temp;
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## method void Array.shuffle();

static METHOD Array_shuffle(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_Array_t *o = (knh_Array_t*)sfp[0].o;
	size_t i;
	for(i = 0; i < o->size; i++) {
		size_t m = knh_rand() % o->size;
		size_t n = knh_rand() % o->size;
		Object *temp = o->list[n];
		o->list[n] = o->list[m];
		o->list[m] = temp;
	}
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## method void IArray.shuffle();

static METHOD IArray_shuffle(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_IArray_t *o = (knh_IArray_t*)sfp[0].o;
	size_t i;
	for(i = 0; i < o->size; i++) {
		size_t m = knh_rand() % o->size;
		size_t n = knh_rand() % o->size;
		knh_int_t temp = o->ilist[n];
		o->ilist[n] = o->ilist[m];
		o->ilist[m] = temp;
	}
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## method void FArray.shuffle();

static METHOD FArray_shuffle(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_FArray_t *o = (knh_FArray_t*)sfp[0].o;
	size_t i;
	for(i = 0; i < o->size; i++) {
		size_t m = knh_rand() % o->size;
		size_t n = knh_rand() % o->size;
		knh_float_t temp = o->flist[n];
		o->flist[n] = o->flist[m];
		o->flist[m] = temp;
	}
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */

//knh_Mapper_t *knh_tMapper_newIteratorArray(Ctx *ctx, knh_class_t icid, knh_class_t acid)
//{
//	TODO();
//	Mapper *mpr = new_Mapper(ctx, FLAG_Mapper_Total, acid, icid, knh_Array_Iterator, KNH_NULL);
//	knh_ClassMap_add(ctx, ctx->share->ClassTable[acid].cmap, mpr);
//	mpr = new_Mapper(ctx, FLAG_Mapper_Total, icid, acid, knh_Iterator_Array, KNH_NULL);
//	knh_ClassMap_add(ctx, ctx->share->ClassTable[icid].cmap, mpr);
//	return mpr;
//}

/* ------------------------------------------------------------------------ */

//knh_Mapper_t *knh_tMapper_newArrayIterator(Ctx *ctx, knh_class_t acid, knh_class_t icid)
//{
//	TODO();
//	Mapper *mpr = new_Mapper(ctx, FLAG_Mapper_Total, icid, acid, knh_Iterator_Array, KNH_NULL);
//	knh_ClassMap_add(ctx, ctx->share->ClassTable[icid].cmap, mpr);
//	mpr = new_Mapper(ctx, FLAG_Mapper_Total, acid, icid, knh_Array_Iterator, KNH_NULL);
//	knh_ClassMap_add(ctx, ctx->share->ClassTable[acid].cmap, mpr);
//	return mpr;
//}

/* ======================================================================== */
/* [2d,3d] */
/* @data */

typedef struct {
	size_t x;
	size_t y;
	size_t z;
	size_t w;   /* unused */
} knh_darray_t;

/* ------------------------------------------------------------------------ */

static
void *knh_array_dmalloc(Ctx *ctx, size_t x, size_t y, size_t z, size_t usize)
{
	size_t capacity = x * y * z;
	knh_darray_t *d = (knh_darray_t*)KNH_MALLOC(ctx, (capacity * usize) + sizeof(knh_darray_t));
	d->x = x; d->y = y; d->z = z; d->w = d->x * d->y;
	return (void*)(d+1);
}

/* ------------------------------------------------------------------------ */

void knh_array_dfree(Ctx *ctx, void *ptr, size_t capacity, size_t usize)
{
	knh_darray_t *d = ((knh_darray_t*)ptr) - 1;
	KNH_FREE(ctx, d, (capacity * usize) + sizeof(knh_darray_t));
}

/* ------------------------------------------------------------------------ */
/* [Array] */

static
void knh_ArrayDim_init(Ctx *ctx, knh_Array_t *a, size_t x, size_t y, size_t z)
{
	knh_Array_setDimension(a, 1);
	a->list = (Object**)knh_array_dmalloc(ctx, x, y, z, sizeof(Object*));
	a->capacity = x * y * z;
	knh_class_t p1 = ctx->share->ClassTable[knh_Object_cid(a)].p1;
	Object *v = (p1 == CLASS_Any) ? KNH_NULL : knh_getClassDefaultValue(ctx, p1);
	size_t i = 0;
	for(i = 0; i < a->capacity; i++) {
		KNH_INITv(a->list[i], v);
	}
	a->size = a->capacity;
}

/* ------------------------------------------------------------------------ */
//## @Private method This! Array.new:array2D(Int! x, Int! y);

static METHOD Array_new__array2D(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_ArrayDim_init(ctx, (knh_Array_t*)sfp[0].o, p_size(sfp[1]), p_size(sfp[2]), 1);
	KNH_RETURN(ctx, sfp, sfp[0].o);
}

/* ------------------------------------------------------------------------ */
//## @Private method This! Array.new:array3D(Int! x, Int! y, Int! z);

static METHOD Array_new__array3D(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_ArrayDim_init(ctx, (knh_Array_t*)sfp[0].o, p_size(sfp[1]), p_size(sfp[2]), p_size(sfp[3]));
	KNH_RETURN(ctx, sfp, sfp[0].o);
}

/* ------------------------------------------------------------------------ */
//## @Hidden method T1 Array.get2D(Int! x, Int! y);

static METHOD Array_get2D(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_Array_t *o = (knh_Array_t*)sfp[0].o;
	knh_darray_t *d = ((knh_darray_t*)o->list) - 1;
	size_t x = knh_array_index(ctx, p_int(sfp[1]), d->x);
	size_t y = knh_array_index(ctx, p_int(sfp[2]), d->y);
	KNH_RETURN(ctx, sfp, o->list[x + y * d->x]);
}

/* ------------------------------------------------------------------------ */
//## @Hidden method T1 Array.get3D(Int! x, Int! y, Int! z);

static METHOD Array_get3D(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_Array_t *o = (knh_Array_t*)sfp[0].o;
	knh_darray_t *d = ((knh_darray_t*)o->list) - 1;
	size_t x = knh_array_index(ctx, p_int(sfp[1]), d->x);
	size_t y = knh_array_index(ctx, p_int(sfp[2]), d->y);
	size_t z = knh_array_index(ctx, p_int(sfp[3]), d->z);
	KNH_RETURN(ctx, sfp, o->list[x + y * d->x + z * d->w]);
}

/* ------------------------------------------------------------------------ */
//## @Hidden method void Array.set2D(Int! x, Int! y, T1 value);

static METHOD Array_set2D(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_Array_t *o = (knh_Array_t*)sfp[0].o;
	knh_darray_t *d = ((knh_darray_t*)o->list) - 1;
	size_t x = knh_array_index(ctx, p_int(sfp[1]), d->x);
	size_t y = knh_array_index(ctx, p_int(sfp[2]), d->y);
	knh_stack_boxing(ctx, sfp + 3);
	KNH_SETv(ctx, o->list[x + y * d->x], sfp[3].o);
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## @Hidden method void Array.set3D(Int! x, Int! y, Int! z, T1 value);

static METHOD Array_set3D(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_Array_t *o = (knh_Array_t*)sfp[0].o;
	knh_darray_t *d = ((knh_darray_t*)o->list) - 1;
	size_t x = knh_array_index(ctx, p_int(sfp[1]), d->x);
	size_t y = knh_array_index(ctx, p_int(sfp[2]), d->y);
	size_t z = knh_array_index(ctx, p_int(sfp[3]), d->z);
	knh_stack_boxing(ctx, sfp + 4);
	KNH_SETv(ctx, o->list[x + y * d->x + z * d->w], sfp[4].o);
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
/* [IArray] */

static
void knh_IArrayDim_init(Ctx *ctx, knh_IArray_t *a, size_t x, size_t y, size_t z)
{
	knh_IArray_setDimension(a, 1);
	a->ilist = (knh_int_t*)knh_array_dmalloc(ctx, x, y, z, sizeof(knh_int_t));
	a->capacity = x * y * z;
	int i = 0;
	for(i = 0; i < a->capacity; i++) {
		a->ilist[i] = 0;
	}
	a->size = a->capacity;
}

/* ------------------------------------------------------------------------ */
//## @Private method This! IArray.new:array2D(Int! x, Int! y);

static METHOD IArray_new__array2D(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_IArrayDim_init(ctx, (knh_IArray_t*)sfp[0].o, p_size(sfp[1]), p_size(sfp[2]), 1);
	KNH_RETURN(ctx, sfp, sfp[0].o);
}

/* ------------------------------------------------------------------------ */
//## @Private method This! IArray.new:array3D(Int! x, Int! y, Int! z);

static METHOD IArray_new__array3D(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_IArrayDim_init(ctx, (knh_IArray_t*)sfp[0].o, p_size(sfp[1]), p_size(sfp[2]), p_size(sfp[3]));
	KNH_RETURN(ctx, sfp, sfp[0].o);
}

/* ------------------------------------------------------------------------ */
//## @Hidden method T1! IArray.get2D(Int! x, Int! y);

static METHOD IArray_get2D(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_IArray_t *o = (knh_IArray_t*)sfp[0].o;
	knh_darray_t *d = ((knh_darray_t*)o->ilist) - 1;
	size_t x = knh_array_index(ctx, p_int(sfp[1]), d->x);
	size_t y = knh_array_index(ctx, p_int(sfp[2]), d->y);
	KNH_RETURN_Int(ctx, sfp, o->ilist[x + y * d->x]);
}

/* ------------------------------------------------------------------------ */
//## @Hidden method T1! IArray.get3D(Int! x, Int! y, Int! z);

static METHOD IArray_get3D(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_IArray_t *o = (knh_IArray_t*)sfp[0].o;
	knh_darray_t *d = ((knh_darray_t*)o->ilist) - 1;
	size_t x = knh_array_index(ctx, p_int(sfp[1]), d->x);
	size_t y = knh_array_index(ctx, p_int(sfp[2]), d->y);
	size_t z = knh_array_index(ctx, p_int(sfp[3]), d->z);
	KNH_RETURN_Int(ctx, sfp, o->ilist[x + y * d->x + z * d->w]);
}

/* ------------------------------------------------------------------------ */
//## @Hidden method void IArray.set2D(Int! x, Int! y, T1! value);

static METHOD IArray_set2D(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_IArray_t *o = (knh_IArray_t*)sfp[0].o;
	knh_darray_t *d = ((knh_darray_t*)o->ilist) - 1;
	size_t x = knh_array_index(ctx, p_int(sfp[1]), d->x);
	size_t y = knh_array_index(ctx, p_int(sfp[2]), d->y);
	o->ilist[x + y * d->x] = sfp[3].ivalue;
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## @Hidden method void IArray.set3D(Int! x, Int! y, Int! z, T1! value);

static METHOD IArray_set3D(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_IArray_t *o = (knh_IArray_t*)sfp[0].o;
	knh_darray_t *d = ((knh_darray_t*)o->ilist) - 1;
	size_t x = knh_array_index(ctx, p_int(sfp[1]), d->x);
	size_t y = knh_array_index(ctx, p_int(sfp[2]), d->y);
	size_t z = knh_array_index(ctx, p_int(sfp[3]), d->z);
	o->ilist[x + y * d->x + z * d->w] = sfp[4].ivalue;
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
/* [FArray] */

static
void knh_FArrayDim_init(Ctx *ctx, knh_FArray_t *a, size_t x, size_t y, size_t z)
{
	knh_FArray_setDimension(a, 1);
	a->flist = (knh_float_t*)knh_array_dmalloc(ctx, x, y, z, sizeof(knh_float_t));
	a->capacity = x * y * z;
	int i = 0;
	for(i = 0; i < a->capacity; i++) {
		a->flist[i] = KNH_FLOAT_ZERO;
	}
	a->size = a->capacity;
}

/* ------------------------------------------------------------------------ */
//## @Private method This! FArray.new:array2D(Int! x, Int! y);

static METHOD FArray_new__array2D(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_FArrayDim_init(ctx, (knh_FArray_t*)sfp[0].o, p_size(sfp[1]), p_size(sfp[2]), 1);
	KNH_RETURN(ctx, sfp, sfp[0].o);
}

/* ------------------------------------------------------------------------ */
//## @Private method This! FArray.new:array3D(Int! x, Int! y, Int! z);

static METHOD FArray_new__array3D(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_FArrayDim_init(ctx, (knh_FArray_t*)sfp[0].o, p_size(sfp[1]), p_size(sfp[2]), p_size(sfp[3]));
	KNH_RETURN(ctx, sfp, sfp[0].o);
}

/* ------------------------------------------------------------------------ */
//## @Hidden method T1! FArray.get2D(Int! x, Int! y);

static METHOD FArray_get2D(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_FArray_t *o = (knh_FArray_t*)sfp[0].o;
	knh_darray_t *d = ((knh_darray_t*)o->flist) - 1;
	size_t x = knh_array_index(ctx, p_int(sfp[1]), d->x);
	size_t y = knh_array_index(ctx, p_int(sfp[2]), d->y);
	KNH_RETURN_Float(ctx, sfp, o->flist[x + y * d->x]);
}

/* ------------------------------------------------------------------------ */
//## @Hidden method T1! FArray.get3D(Int! x, Int! y, Int! z);

static METHOD FArray_get3D(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_FArray_t *o = (knh_FArray_t*)sfp[0].o;
	knh_darray_t *d = ((knh_darray_t*)o->flist) - 1;
	size_t x = knh_array_index(ctx, p_int(sfp[1]), d->x);
	size_t y = knh_array_index(ctx, p_int(sfp[2]), d->y);
	size_t z = knh_array_index(ctx, p_int(sfp[3]), d->z);
	KNH_RETURN_Float(ctx, sfp, o->flist[x + y * d->x + z * d->w]);
}

/* ------------------------------------------------------------------------ */
//## @Hidden method void FArray.set2D(Int! x, Int! y, T1 value);

static METHOD FArray_set2D(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_FArray_t *o = (knh_FArray_t*)sfp[0].o;
	knh_darray_t *d = ((knh_darray_t*)o->flist) - 1;
	size_t x = knh_array_index(ctx, p_int(sfp[1]), d->x);
	size_t y = knh_array_index(ctx, p_int(sfp[2]), d->y);
	o->flist[x + y * d->x] = sfp[3].fvalue;
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## @Hidden method void FArray.set3D(Int! x, Int! y, Int! z, T1 value);

static METHOD FArray_set3D(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_FArray_t *o = (knh_FArray_t*)sfp[0].o;
	knh_darray_t *d = ((knh_darray_t*)o->flist) - 1;
	size_t x = knh_array_index(ctx, p_int(sfp[1]), d->x);
	size_t y = knh_array_index(ctx, p_int(sfp[2]), d->y);
	size_t z = knh_array_index(ctx, p_int(sfp[3]), d->z);
	o->flist[x + y * d->x + z * d->w] = sfp[4].fvalue;
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */

#endif/*KNH_CC_METHODAPI*/


#ifdef __cplusplus
}
#endif
