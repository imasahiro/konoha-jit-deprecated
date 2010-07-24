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

/* ======================================================================== */
/* [Bytes, byte[]] */

/* ------------------------------------------------------------------------ */
//## method void Bytes.putc(Int char);

static METHOD Bytes_putc(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Bytes_t *o = (knh_Bytes_t*)sfp[0].o;
	knh_Bytes_putc(ctx, o, Int_to(size_t, sfp[1]));
	RETURNvoid_();
}

/* ------------------------------------------------------------------------ */
//## method void Bytes.write(Bytes buf, Int offset, Int length);
//## method void Bytes.add(Bytes buf, Int offset, Int length);

static METHOD Bytes_write(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Bytes_t *ba = sfp[0].ba;
	knh_bytes_t t = BA_tobytes(sfp[1].ba);
	if(sfp[2].ivalue != 0) {
		size_t n = knh_array_index(ctx, sfp, Int_to(size_t, sfp[2]), t.len);
		t = knh_bytes_last(t, n);
	}
	if(sfp[3].ivalue != 0) {
		size_t l = Int_to(size_t, sfp[3]);
		if(l < t.len) t.len = l;
	}
	knh_Bytes_write(ctx, ba, t);
	RETURNvoid_();
}

///* ------------------------------------------------------------------------ */
////## method void Bytes.memcpy(Int offset, Bytes buf, Int offset, Int length);
//
//static METHOD Bytes_memcpy(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	knh_Bytes_t *d = (knh_Bytes_t*)sfp[0].o;
//	size_t doff = IS_NULL(sfp[1].o) ? 0 : Int_to(size_t, sfp[1]);
//	knh_Bytes_t *s = (knh_Bytes_t*)sfp[2].o;
//	size_t soff = IS_NULL(sfp[3].o) ? 0 : Int_to(size_t, sfp[3]);
//	size_t slen = IS_NULL(sfp[4].o) ? 0 : ((s)->size - soff);
//	if(doff + slen < (d)->size) {
//		knh_memcpy(d->buf + doff, s->buf + soff, slen);
//	}
//	else {
//		KNH_THROW_OUTOFINDEX(ctx, doff + slen, (d)->size);
//	}
//	RETURNvoid_();
//}

/* ------------------------------------------------------------------------ */
/* [Array, IArray, FArray] */

/* ------------------------------------------------------------------------ */
//## method T1 Array.get(Int n);

static METHOD Array_get(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	(sfp[0].a)->api->get(ctx, sfp, rix);
}

/* ------------------------------------------------------------------------ */
//## method T1 Array.set(Int n, T1 v);

static METHOD Array_set(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	(sfp[0].a)->api->set(ctx, sfp, rix);
}

/* ------------------------------------------------------------------------ */
//## method T1 Array.setAll(T1 v);

static METHOD Array_setAll(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	size_t i;
	knh_ndata_t data = sfp[2].data;  // @restrict
	KNH_SETv(ctx, sfp[2].o, sfp[1].o);
	sfp[2].data = sfp[1].data;
	for(i = 0; i < (sfp[0].a)->size; i++) {
		sfp[1].ivalue = i;
		(sfp[0].a)->api->set(ctx, sfp, rix);
	}
	sfp[1].data = data;  // @restrict
}

/* ------------------------------------------------------------------------ */
//## method void Array.add(T1 value, ...);
//## method void Array.opSEND(T1 value, ...);

static METHOD Array_add(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Array_t *a = sfp[0].a;
	a->api->add(ctx, a, sfp+1);
	RETURNvoid_();
}

///* ------------------------------------------------------------------------ */
//
//static void knh_Array_remove(Ctx *ctx, knh_Array_t *a, size_t n)
//{
//	DBG_ASSERT(n < a->size);
//	size_t i;
//	Object *removed = a->list[n];
//	for(i = n; i < a->size - 1; i++) {
//		a->list[i] = a->list[i+1];
//	}
//	a->size--;
//	a->list[a->size] = removed;
//}

///* ------------------------------------------------------------------------ */
////## method void Array.insert(Int n, T1! value);
//
//static METHOD Array_insert(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	knh_Array_t *o = (knh_Array_t*)sfp[0].o;
//	size_t i, n = knh_array_index(ctx, Int_to(size_t, sfp[1]), o->size);
//	Object *temp;
//	if(o->size == o->capacity) {
//		knh_Array_grow(ctx, o, knh_array_newsize(o->capacity * 2, sizeof(Object*)), KNH_NULL);
//	}
//	temp = o->list[o->size];
//	o->size++;
//	for(i = o->size - 1; i > n ; i--) {
//		o->list[i] = o->list[i-1];
//	}
//	o->list[n] = temp;
//	knh_stack_boxing(ctx, sfp + 2);
//	KNH_SETv(ctx, o->list[n], sfp[2].o);
//	RETURNvoid_();
//}
//
///* ------------------------------------------------------------------------ */
////## method void IArray.insert(Int n, Int value);
//
//static METHOD IArray_insert(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//
//	knh_IArray_t *o = sfp[0].ia;
//	size_t i, n = knh_array_index(ctx, Int_to(size_t, sfp[1]), o->size);
//	knh_int_t temp;
//	if(o->size == o->capacity) {
//		knh_IArray_grow(ctx, o, knh_array_newsize(o->capacity * 2, sizeof(knh_int_t)), 0);
//	}
//	temp = o->ilist[o->size];
//	o->size++;
//	for(i = o->size - 1; i > n ; i--) {
//		o->ilist[i] = o->ilist[i-1];
//	}
//	o->ilist[n] = temp;
//	o->ilist[n] = sfp[2].fvalue;
//	RETURNvoid_();
//}
//
///* ------------------------------------------------------------------------ */
////## method void FArray.insert(Int n, Float! value);
//
//static METHOD FArray_insert(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//
//	knh_FArray_t *o = sfp[0].fa;
//	size_t i, n = knh_array_index(ctx, Int_to(size_t, sfp[1]), o->size);
//	knh_float_t temp;
//	if(o->size == o->capacity) {
//		knh_FArray_grow(ctx, o, knh_array_newsize(o->capacity * 2, sizeof(knh_float_t)), K_FLOAT_ZERO);
//	}
//	temp = o->flist[o->size];
//	o->size++;
//	for(i = o->size - 1; i > n ; i--) {
//		o->flist[i] = o->flist[i-1];
//	}
//	o->flist[n] = temp;
//	o->flist[n] = sfp[2].fvalue;
//	RETURNvoid_();
//}
//
///* ------------------------------------------------------------------------ */
////## method void Array.clear();
//
//static METHOD Array_clear(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//
//	knh_Array_clear(ctx, sfp[0].a);
//	RETURNvoid_();
//}
//
//
///* ------------------------------------------------------------------------ */
////## method void Array.remove(Int n);
//
//static METHOD Array_remove(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	knh_Array_t *o = (knh_Array_t*)sfp[0].o;
//	size_t n = knh_array_index(ctx, Int_to(size_t, sfp[1]), o->size);
//	knh_Array_remove(ctx, o, n);
//	RETURNvoid_();
//}
//
///* ------------------------------------------------------------------------ */
////## method void IArray.remove(Int n);
//
//static METHOD IArray_remove(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	knh_IArray_t *o = sfp[0].ia;
//	size_t i, n = knh_array_index(ctx, Int_to(size_t, sfp[1]), o->size);
//	for(i = n; i < o->size - 1; i++) {
//		o->ilist[i] = o->ilist[i+1];
//	}
//	o->size--;
//	RETURNvoid_();
//}
//
///* ------------------------------------------------------------------------ */
////## method void FArray.remove(Int n);
//
//static METHOD FArray_remove(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	knh_FArray_t *o = sfp[0].fa;
//	size_t i, n = knh_array_index(ctx, Int_to(size_t, sfp[1]), o->size);
//	for(i = n; i < o->size - 1; i++) {
//		o->flist[i] = o->flist[i+1];
//	}
//	o->size--;
//	RETURNvoid_();
//}
//
///* ------------------------------------------------------------------------ */
////## method T1! Array.pop();
//
//static METHOD Array_pop(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//
//	knh_Array_t *o = (knh_Array_t*)sfp[0].o;
//	if(o->size > 0) {
//		o->size--;
//		RETURN_(o->list[o->size]);
//	} else {
//		KNH_THROW_OUTOFINDEX(ctx, 0, 0);
//	}
//}
//
///* ------------------------------------------------------------------------ */
////## method T1! IArray.pop();
//
//static METHOD IArray_pop(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	
//	knh_IArray_t *o = sfp[0].ia;
//	if(o->size > 0) {
//		o->size--;
//		RETURNi_(o->ilist[o->size]);
//	} else {
//		KNH_THROW_OUTOFINDEX(ctx, 0, 0);
//	}
//}
//
///* ------------------------------------------------------------------------ */
////## method T1! FArray.pop();
//
//static METHOD FArray_pop(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	
//	knh_FArray_t *o = sfp[0].fa;
//	if(o->size > 0) {
//		o->size--;
//		RETURNi_(o->flist[o->size]);
//	} else {
//		KNH_THROW_OUTOFINDEX(ctx, 0, 0);
//	}
//}
//
///* ------------------------------------------------------------------------ */
////## method Int Array.indexOf(T1 value);
//
//static METHOD Array_indexOf(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	
//	knh_Array_t *o = (knh_Array_t*)sfp[0].o;
//	knh_int_t res = -1;
//	size_t i;
//	knh_stack_boxing(ctx, sfp + 1);
//	for(i = 0; i < o->size; i++) {
//		if(knh_Object_compareTo(ctx, o->list[i], sfp[1].o) == 0) {
//			res = i; break;
//		}
//	}
//	RETURNi_(res);
//}
//
///* ------------------------------------------------------------------------ */
////## method Int IArray.indexOf(Int value);
//
//static METHOD IArray_indexOf(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	
//	knh_IArray_t *o = sfp[0].ia;
//	size_t i;
//	knh_int_t res = -1;
//	for(i = 0; i < o->size; i++) {
//		if(o->ilist[i] == sfp[1].ivalue) {
//			res = i; break;
//		}
//	}
//	RETURNi_(res);
//}
//
///* ------------------------------------------------------------------------ */
////## method Int FArray.indexOf(Float! value);
//
//static METHOD FArray_indexOf(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	
//	knh_FArray_t *o = sfp[0].fa;
//	size_t i;
//	knh_int_t res = -1;
//	for(i = 0; i < o->size; i++) {
//		if(o->flist[i] == sfp[1].fvalue) {
//			res = i; break;
//		}
//	}
//	RETURNi_(res);
//}
//
///* ------------------------------------------------------------------------ */
////## method Int Array.lastIndexOf(T1 value);
//
//static METHOD Array_lastIndexOf(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	
//	knh_int_t res = -1;
//	knh_Array_t *o = (knh_Array_t*)sfp[0].o;
//	long i;
//	knh_stack_boxing(ctx, sfp + 1);
//	for(i = o->size - 1; i >= 0; i--) {
//		if(knh_Object_compareTo(ctx, o->list[i], sfp[1].o) == 0) {
//			res = i; break;
//		}
//	}
//	RETURNi_(res);
//}
//
///* ------------------------------------------------------------------------ */
////## method Int IArray.lastIndexOf(Int value);
//
//static METHOD IArray_lastIndexOf(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//
//	knh_int_t res = -1;
//	knh_IArray_t *o = sfp[0].ia;
//	long i;
//	for(i = o->size - 1; i >= 0; i--) {
//		if(o->ilist[i] == sfp[1].ivalue) {
//			res = i; break;
//		}
//	}
//	RETURNi_(res);
//}
//
///* ------------------------------------------------------------------------ */
////## method Int FArray.lastIndexOf(Float! value);
//
//static METHOD FArray_lastIndexOf(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	
//	knh_int_t res = -1;
//	knh_FArray_t *o = sfp[0].fa;
//	long i;
//	for(i = o->size - 1; i >= 0; i--) {
//		if(o->flist[i] == sfp[1].fvalue) {
//			res = i; break;
//		}
//	}
//	RETURNi_(res);
//}
//
///* ======================================================================== */
///* [Collections] */
//
//typedef struct {
//	Ctx *ctx;
//	knh_sfp_t *sfp;
//} knh_env_t;
//
//static int knh_env_comp(knh_env_t *env, Object **a1, Object **a2)
//{
//	Ctx *ctx = env->ctx;
//	knh_sfp_t *lsfp = env->sfp + 2;
//	knh_putsfp(ctx, lsfp, 2, a1[0]);
//	knh_putsfp(ctx, lsfp, 3, a2[0]);
//	knh_Func_invokesfp(ctx, env->sfp[1].cc, lsfp, 1/*rtnidx*/, 2);
//	return (int)lsfp[0].ivalue;
//}
//
///* ------------------------------------------------------------------------ */
////## method void Array.sort(Cmpr? cc);
//
//static METHOD Array_sort(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	
//	knh_Array_t *o = sfp[0].a;
//	if(IS_NULL(sfp[1].o)) {
//		knh_qsort_r(o->list, o->size, sizeof(Object*),
//			(void*)ctx, (int (*)(void*, const void*, const void*))knh_Object_compareTo2);
//	}
//	else {
//		knh_env_t env = {ctx, sfp};
//		knh_qsort_r(o->list, o->size, sizeof(Object*), (void*)&env,
//				(int (*)(void *, const void* , const void*))knh_env_comp);
//	}
//	RETURNvoid_();
//}
//
///* ------------------------------------------------------------------------ */
//
//static int qsort_icmp(const void* ap, const void* bp)
//{
//	knh_int_t a = *((knh_int_t*)ap);
//	knh_int_t b = *((knh_int_t*)bp);
//	if(a < b) return -1;
//	if(a > b) return 1;
//	return 0;
//}
//
///* ------------------------------------------------------------------------ */
////## method void IArray.sort();
//
//static METHOD IArray_sort(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	
//	knh_IArray_t *o = (knh_IArray_t*)sfp[0].o;
//	knh_qsort(o->ilist, o->size, sizeof(knh_int_t), qsort_icmp);
//	RETURNvoid_();
//}
//
///* ------------------------------------------------------------------------ */
//
//static int qsort_fcmp(const void* ap, const void* bp)
//{
//	knh_float_t a = *((knh_float_t*)ap);
//	knh_float_t b = *((knh_float_t*)bp);
//	if(a < b) return -1;
//	if(a > b) return 1;
//	return 0;
//}
//
///* ------------------------------------------------------------------------ */
////## method void FArray.sort();
//
//static METHOD FArray_sort(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	
//	knh_FArray_t *o = (knh_FArray_t*)sfp[0].o;
//	knh_qsort(o->flist, o->size, sizeof(knh_float_t), qsort_fcmp);
//	RETURNvoid_();
//}
//
///* ------------------------------------------------------------------------ */
////## method void Array.reverse();
//
//static METHOD Array_reverse(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	
//	knh_Array_t *o = (knh_Array_t*)sfp[0].o;
//	size_t i;
//	for(i = 0; i < o->size / 2; i++) {
//		size_t n = o->size - i - 1;
//		Object *temp = o->list[i];
//		o->list[i] = o->list[n];
//		o->list[n] = temp;
//	}
//	RETURNvoid_();
//}
//
///* ------------------------------------------------------------------------ */
////## method void IArray.reverse();
//
//static METHOD IArray_reverse(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	
//	knh_IArray_t *o = (knh_IArray_t*)sfp[0].o;
//	size_t i;
//	for(i = 0; i < o->size / 2; i++) {
//		size_t n = o->size - i - 1;
//		knh_int_t temp = o->ilist[i];
//		o->ilist[i] = o->ilist[n];
//		o->ilist[n] = temp;
//	}
//	RETURNvoid_();
//}
//
///* ------------------------------------------------------------------------ */
////## method void FArray.reverse();
//
//static METHOD FArray_reverse(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//
//	knh_FArray_t *o = (knh_FArray_t*)sfp[0].o;
//	size_t i;
//	for(i = 0; i < o->size / 2; i++) {
//		size_t n = o->size - i - 1;
//		knh_float_t temp = o->flist[i];
//		o->flist[i] = o->flist[n];
//		o->flist[n] = temp;
//	}
//	RETURNvoid_();
//}
//
///* ------------------------------------------------------------------------ */
////## method void Array.swap(Int m, Int n);
//
//static METHOD Array_swap(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	
//	knh_Array_t *o = (knh_Array_t*)sfp[0].o;
//	size_t m = knh_array_index(ctx, Int_to(size_t, sfp[1]), o->size);
//	size_t n = knh_array_index(ctx, Int_to(size_t, sfp[2]), o->size);
//	Object *temp = o->list[n];
//	o->list[n] = o->list[m];
//	o->list[m] = temp;
//	RETURNvoid_();
//}
//
///* ------------------------------------------------------------------------ */
////## method void IArray.swap(Int m, Int n);
//
//static METHOD IArray_swap(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	
//	knh_IArray_t *o = (knh_IArray_t*)sfp[0].o;
//	size_t m = knh_array_index(ctx, Int_to(size_t, sfp[1]), o->size);
//	size_t n = knh_array_index(ctx, Int_to(size_t, sfp[2]), o->size);
//	knh_int_t temp = o->ilist[n];
//	o->ilist[n] = o->ilist[m];
//	o->ilist[m] = temp;
//	RETURNvoid_();
//}
//
///* ------------------------------------------------------------------------ */
////## method void FArray.swap(Int m, Int n);
//
//static METHOD FArray_swap(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	
//	knh_FArray_t *o = (knh_FArray_t*)sfp[0].o;
//	size_t m = knh_array_index(ctx, Int_to(size_t, sfp[1]), o->size);
//	size_t n = knh_array_index(ctx, Int_to(size_t, sfp[2]), o->size);
//	knh_float_t temp = o->flist[n];
//	o->flist[n] = o->flist[m];
//	o->flist[m] = temp;
//	RETURNvoid_();
//}
//
///* ------------------------------------------------------------------------ */
////## method void Array.shuffle();
//
//static METHOD Array_shuffle(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	
//	knh_Array_t *o = (knh_Array_t*)sfp[0].o;
//	size_t i;
//	for(i = 0; i < o->size; i++) {
//		size_t m = knh_rand() % o->size;
//		size_t n = knh_rand() % o->size;
//		Object *temp = o->list[n];
//		o->list[n] = o->list[m];
//		o->list[m] = temp;
//	}
//	RETURNvoid_();
//}
//
///* ------------------------------------------------------------------------ */
////## method void IArray.shuffle();
//
//static METHOD IArray_shuffle(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	
//	knh_IArray_t *o = (knh_IArray_t*)sfp[0].o;
//	size_t i;
//	for(i = 0; i < o->size; i++) {
//		size_t m = knh_rand() % o->size;
//		size_t n = knh_rand() % o->size;
//		knh_int_t temp = o->ilist[n];
//		o->ilist[n] = o->ilist[m];
//		o->ilist[m] = temp;
//	}
//	RETURNvoid_();
//}
//
///* ------------------------------------------------------------------------ */
////## method void FArray.shuffle();
//
//static METHOD FArray_shuffle(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	
//	knh_FArray_t *o = (knh_FArray_t*)sfp[0].o;
//	size_t i;
//	for(i = 0; i < o->size; i++) {
//		size_t m = knh_rand() % o->size;
//		size_t n = knh_rand() % o->size;
//		knh_float_t temp = o->flist[n];
//		o->flist[n] = o->flist[m];
//		o->flist[m] = temp;
//	}
//	RETURNvoid_();
//}

/* ------------------------------------------------------------------------ */

#endif/*K_USING_DEFAULTAPI*/

#ifdef __cplusplus
}
#endif
