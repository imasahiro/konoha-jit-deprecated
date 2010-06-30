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

/* ------------------------------------------------------------------------ */
/* [hmem] */

size_t knh_good_entrysize(size_t hsize, size_t wsize, size_t n)
{
	size_t msize = knh_good_size(hsize + (wsize * n));
	size_t entry = (msize - hsize) / wsize;
	if(entry != n) {
		DBG_P("entry wsize=%ld, resize %ld => %ld", wsize, n, entry);
	}
	return entry;
}

/* ------------------------------------------------------------------------ */

knh_hmem_t* knh_hmalloc(Ctx *ctx, size_t wsize, size_t n)
{
	size_t msize = knh_good_size(sizeof(knh_hmem_t) + (wsize * n));
	knh_hmem_t *h = (knh_hmem_t*)KNH_MALLOC(ctx, msize);
	knh_bzero(h, msize);
	h->wsize = wsize;
	h->capacity = (msize - sizeof(knh_hmem_t)) / wsize;
	DBG_ASSERT(knh_good_size((h->wsize * h->capacity) + sizeof(knh_hmem_t)) == msize);
	return (h+1);
}

/* ------------------------------------------------------------------------ */

knh_hmem_t* knh_hrealloc(Ctx *ctx, knh_hmem_t *p, size_t newn)
{
	knh_hmem_t *h = p - 1;
	size_t size = knh_good_size(sizeof(knh_hmem_t) + (h->wsize * h->capacity));
	size_t newsize = knh_good_size(sizeof(knh_hmem_t) + (h->wsize * newn));
	knh_hmem_t *newh = (knh_hmem_t*)KNH_MALLOC(ctx, newsize);
	if(newsize > size) {
		knh_memcpy(newh, h, size);
		knh_bzero((char*)newh + size, newsize - size);
	}
	else {
		knh_memcpy(newh, h, newsize);
	}
	newh->capacity = (newsize - sizeof(knh_hmem_t)) / h->wsize;
	newh->wsize = h->wsize;
	KNH_FREE(ctx, h, size);
	return (newh+1);
}

/* ------------------------------------------------------------------------ */

knh_hmem_t* knh_hgrow(Ctx *ctx, knh_hmem_t *p)
{
	knh_hmem_t *h = p - 1;
	size_t size = knh_good_size(sizeof(knh_hmem_t) + (h->wsize * h->capacity));
	size_t newsize = size * 2;
	knh_hmem_t *newh = (knh_hmem_t*)KNH_MALLOC(ctx, newsize);
	knh_memcpy(newh, h, size);
	knh_bzero((char*)newh + size, newsize - size);
	newh->capacity = (newsize - sizeof(knh_hmem_t)) / h->wsize;
	newh->wsize = h->wsize;
	KNH_FREE(ctx, h, size);
	DBG_P("GROW ptr=%p, size=%ld, newsize=%ld", h, size, newsize);
	return (newh+1);
}

/* ------------------------------------------------------------------------ */

void knh_hfree(Ctx *ctx, knh_hmem_t *p)
{
	knh_hmem_t *h = p - 1;
	size_t size = knh_good_size(sizeof(knh_hmem_t) + (h->wsize * h->capacity));
	KNH_FREE(ctx, (void*)h, size);
}

/* ------------------------------------------------------------------------ */

void knh_Array_grow(Ctx *ctx, knh_Array_t *a, size_t newsize, size_t reqsize)
{
	if(newsize < reqsize) newsize = reqsize;
	if(a->hmem[-1].capacity == 0) {
		size_t wsize = (knh_Array_isNDATA(a)) ? sizeof(knh_ndata_t) : sizeof(Object*);
		a->hmem = knh_hmalloc(ctx, wsize, newsize);
	}
	else {
		a->hmem = knh_hrealloc(ctx, a->hmem, newsize);
	}
}

/* ------------------------------------------------------------------------ */
/* [api] */

static METHOD knh_Farray_getO(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Array_t *a = sfp[0].a;
	size_t n2 = knh_array_index(ctx, Int_to(size_t, ctx->esp[-1]), a->size);
	RETURN_(a->list[n2]);
}

static METHOD knh_Farray_getN(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Array_t *a = sfp[0].a;
	size_t n2 = knh_array_index(ctx, Int_to(size_t, ctx->esp[-1]), a->size);
	RETURNd_(a->nlist[n2]);
}

static METHOD knh_Farray_setO(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Array_t *a = sfp[0].a;
	size_t n2 = knh_array_index(ctx, Int_to(size_t, sfp[1]), a->size);
	KNH_SETv(ctx, a->list[n2], sfp[2].o);
	RETURN_(sfp[2].o);
}

static METHOD knh_Farray_setN(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Array_t *a = sfp[0].a;
	size_t n2 = knh_array_index(ctx, Int_to(size_t, sfp[1]), a->size);
	a->nlist[n2] = sfp[2].data;
	RETURNd_(sfp[2].data);
}

static void knh_Farray_addO(Ctx *ctx, knh_Array_t *a, knh_sfp_t *v)
{
	size_t i, n = knh_stack_argc(ctx, v);
	if(!(a->size + n < a->hmem[-1].capacity)) {
		knh_Array_grow(ctx, a, a->size *2, a->size + n);
	}
	for(i = 0; i < n; i++) {
		KNH_INITv(a->list[a->size+i], v[i].o);
	}
	a->size += n;
}

static void knh_Farray_addN(Ctx *ctx, knh_Array_t *a, knh_sfp_t *v)
{
	size_t i, n = knh_stack_argc(ctx, v);
	if(!(a->size + n < a->hmem[-1].capacity)) {
		knh_Array_grow(ctx, a, a->size *2, a->size + n);
	}
	for(i = 0; i < n; i++) {
		a->nlist[a->size+i] = v[i].data;
	}
	a->size += n;
}

static knh_ArrayAPI_t ArrayOAPI = {
	knh_Farray_getO,
	knh_Farray_addO,
	knh_Farray_setO
};

static knh_ArrayAPI_t ArrayNAPI = {
	knh_Farray_getN,
	knh_Farray_addN,
	knh_Farray_setN
};

void knh_Array_initAPI(Ctx *ctx, knh_Array_t *a)
{
	knh_class_t p1 = knh_Object_p1(a);
	if(IS_Tint(p1) || IS_Tfloat(p1)) {
		knh_Array_setNDATA(a, 1);
		a->api = &ArrayNAPI;
	}
	else {
		a->api = &ArrayOAPI;
	}
}

/* ======================================================================== */
/* [constructors] */

knh_Array_t* new_Array0(Ctx *ctx, size_t capacity)
{
	knh_Array_t *a = new_H(Array);
	a->hmem = knh_hmalloc(ctx, sizeof(Object*), capacity);
	a->size = 0;
	a->api = &ArrayOAPI;
	return a;
}

/* ------------------------------------------------------------------------ */

KNHAPI(knh_Array_t*) new_Array(Ctx *ctx, knh_class_t p1, size_t capacity)
{
	knh_class_t cid = knh_class_P1(ctx, CLASS_Array, p1);
	knh_Array_t *a = (knh_Array_t*)new_hObject_(ctx, FLAG_Array, CLASS_Array, cid);
	knh_Array_initAPI(ctx, a);
	a->hmem = knh_hmalloc(ctx, knh_Array_isNDATA(a) ? sizeof(knh_ndata_t) : sizeof(Object*), capacity);
	a->size = 0;
	return a;
}

/* ------------------------------------------------------------------------ */

void knh_Array_clear(Ctx *ctx, knh_Array_t *a, size_t n)
{
	if(!knh_Array_isNDATA(a)) {
		size_t i;
		for(i = n; i < a->size; i++) {
			KNH_FINALv(ctx, a->list[i]);
		}
	}
	a->size = n;
}

/* ------------------------------------------------------------------------ */

KNHAPI(void) knh_Array_add_(Ctx *ctx, knh_Array_t *a, Any *value)
{
	if(a->size == a->hmem[-1].capacity) {
		size_t newsize = a->hmem[-1].capacity * 2;
		knh_Array_grow(ctx, a, newsize, a->size + 1);
	}
	DBG_ASSERT(a->size < a->hmem[-1].capacity);
	KNH_INITv(a->list[a->size], value);
	a->size++;
}

///* ------------------------------------------------------------------------ */
//
//void knh_Array_remove(Ctx *ctx, knh_Array_t *a, size_t n)
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
//
//Any* knh_Array_pop(Ctx *ctx, knh_Array_t *o)
//{
//	DBG_ASSERT(o->size > 0);
//	if(!knh_Array_isNDATA(a)) {
//		KNH_FINALv(ctx, a->list[i]);
//	}
//
//	o->size--;
//		return o->list[o->size];
//	}else {
//		return KNH_NULL;
//	}
//}

/* ------------------------------------------------------------------------ */

size_t knh_array_index0(Ctx *ctx, knh_intptr_t n, size_t size)
{
	size_t idx = (size_t)n;
	if(!(idx < size)) KNH_THROW_OUTOFINDEX(ctx, n, size);
	return idx;
}

/* ------------------------------------------------------------------------ */

size_t knh_array_index(Ctx *ctx, knh_intptr_t n, size_t size)
{
	size_t idx = (n < 0) ? size + n : n;
	if(!(idx < size)) KNH_THROW_OUTOFINDEX(ctx, n, size);
	return idx;
}

/* ------------------------------------------------------------------------ */


#ifdef __cplusplus
}
#endif
