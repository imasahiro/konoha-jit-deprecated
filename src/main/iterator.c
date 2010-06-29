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

/* ======================================================================== */
/* [private] */

static ITRNEXT knh_fitrnext_end(Ctx *ctx, knh_sfp_t *sfp, long rtnidx)
{
	ITREND_();
}

/* ------------------------------------------------------------------------ */

KNHAPI(void) knh_Iterator_close(Ctx *ctx, knh_Iterator_t *it)
{
	DBG_ASSERT(IS_bIterator(it));
	KNH_SETv(ctx, DP(it)->source, KNH_NULL);
	if(DP(it)->freffree != NULL) {
		if(DP(it)->ref != NULL) {
			DP(it)->freffree(DP(it)->ref);
		}
		DP(it)->freffree = NULL;
	}
	DP(it)->ref = NULL;
	DP(it)->fnext = knh_fitrnext_end;
	it->fnext_1   = knh_fitrnext_end;
}

/* ------------------------------------------------------------------------ */

KNHAPI(knh_Iterator_t*) new_Iterator(Ctx *ctx, knh_class_t p1, Any *source, knh_Fitrnext fnext)
{
	knh_class_t cid = knh_class_P1(ctx, CLASS_Iterator, p1);
	knh_Iterator_t *it = new_O(Iterator, cid);
	if(IS_NULL(source)) fnext = knh_fitrnext_end;
	if(fnext != NULL) DP(it)->fnext = fnext;
	KNH_SETv(ctx, DP(it)->source, source);
	it->fnext_1 = DP(it)->fnext;
	return it;
}

/* ------------------------------------------------------------------------ */
/* [constructors] */

KNHAPI(knh_Iterator_t*) new_RawPtrIterator(Ctx *ctx, knh_class_t p1, void *ref, knh_Fitrnext fnext, knh_Ffree ffree)
{
	knh_class_t cid = knh_class_P1(ctx, CLASS_Iterator, p1);
	knh_Iterator_t *it = new_O(Iterator, cid);
	if(ref == NULL) {
		fnext = knh_fitrnext_end;
		ffree = NULL;
	}
	DP(it)->ref = ref;
	DP(it)->freffree = ffree;
	if(fnext != NULL) DP(it)->fnext = fnext;
	it->fnext_1 = DP(it)->fnext;
	return it;
}

/* ------------------------------------------------------------------------ */
/* [ArrayIterator] */

static ITRNEXT knh_Array_nextO(Ctx *ctx, knh_sfp_t *sfp, long rtnidx)
{
	knh_Array_t *a = (knh_Array_t*)DP(sfp[0].it)->source;
	size_t pos = DP(sfp[0].it)->pos;
	while(pos < a->size) {
		DP(sfp[0].it)->pos = pos+1;
		ITRNEXT_(a->list[pos]);
		pos++;
	}
	ITREND_();
}

static ITRNEXT knh_Array_nextN(Ctx *ctx, knh_sfp_t *sfp, long rtnidx)
{
	knh_Array_t *a = (knh_Array_t*)DP(sfp[0].it)->source;
	size_t pos = DP(sfp[0].it)->pos;
	while(pos < a->size) {
		DP(sfp[0].it)->pos = pos+1;
		ITRNEXTd_(a->nlist[pos]);
		pos++;
	}
	ITREND_();
}

/* ------------------------------------------------------------------------ */

KNHAPI(knh_Iterator_t*) new_ArrayIterator(Ctx *ctx, knh_Array_t *a)
{
	knh_class_t cid = knh_Object_p1(a);
	knh_Fitrnext fnext = knh_Array_isNDATA(a) ? knh_Array_nextN : knh_Array_nextO;
	return new_Iterator(ctx, cid, UP(a), fnext);
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
