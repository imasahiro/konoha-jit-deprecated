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
/* [common] */

static
knh_hashcode_t knh_stack_hashCode(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_class_t bcid = (sfp[0].o)->h.bcid;
	if(CLASS_Boolean <= bcid && bcid <= CLASS_Float) {
		return (knh_hashcode_t)(sfp[0].i)->n.data;
	}
	return ctx->share->StructTable[bcid].fhashCode(ctx, sfp[0].o);
}

/* ------------------------------------------------------------------------ */

static
int knh_stack_equals(Ctx *ctx, knh_sfp_t *sfp, Object *o)
{
	knh_class_t bcid = (sfp[0].o)->h.bcid;
	if(CLASS_Boolean <= bcid && bcid <= CLASS_Float) {
		return ((sfp[0].i)->n.data == ((knh_Int_t*)o)->n.data);
	}
	return (knh_Object_compareTo(ctx, sfp[0].o, o) == 0);
}

/* ------------------------------------------------------------------------ */
//## method This! HashMap.new(Int? initCapacity);

static METHOD HashMap_new(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_HashMap_t *o = (knh_HashMap_t*)sfp[0].o;
	int init = IS_NULL(sfp[1].o) ? KNH_HASH_INITSIZE: p_int(sfp[1]);
	if(init > 0) {
		ctx->share->StructTable[STRUCT_HashMap].finit(ctx, sfp[0].o, init);
	}
	KNH_RETURN(ctx, sfp, o);
}

/* ------------------------------------------------------------------------ */
//## method T2 HashMap.get(T1 key);

static METHOD HashMap_get(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_HashMap_t *o = (knh_HashMap_t*)sfp[0].o;
	knh_hashcode_t hcode = knh_stack_hashCode(ctx, sfp + 1);
	knh_uintptr_t h =  hcode % DP(o)->hmax;
	knh_hashentry_t *e = DP(o)->array[h];

	while(e != NULL) {
		if(e->hcode == hcode
				&& knh_Object_cid(sfp[1].o) == knh_Object_cid(e->key)
				&& knh_stack_equals(ctx, sfp + 1, e->key)) {
			KNH_RETURN(ctx, sfp, e->value);
		}
		e = e->next;
	}
	KNH_RETURN(ctx, sfp, KNH_NULL);
}

/* ------------------------------------------------------------------------ */
//## method Boolean! HashMap.opHas(T1 key);

static METHOD HashMap_opHas(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_HashMap_t *o = (knh_HashMap_t*)sfp[0].o;
	knh_hashcode_t hcode = knh_stack_hashCode(ctx, sfp + 1);
	knh_uintptr_t h =  hcode % DP(o)->hmax;
	knh_hashentry_t *e = DP(o)->array[h];

	while(e != NULL) {
		if(e->hcode == hcode
				&& knh_Object_cid(sfp[1].o) == knh_Object_cid(e->key)
				&& knh_stack_equals(ctx, sfp + 1, e->key)) {
			KNH_RETURN_Boolean(ctx, sfp, 1);
		}
		e = e->next;
	}
	KNH_RETURN_Boolean(ctx, sfp, 0);
}

/* ------------------------------------------------------------------------ */
//## method void HashMap.remove(T1 key);

static METHOD HashMap_remove(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_Hash_t *o = (knh_Hash_t*)sfp[0].o;
	knh_hashcode_t hcode = knh_stack_hashCode(ctx, sfp + 1);
	knh_uintptr_t h =  hcode % DP(o)->hmax;
	knh_hashentry_t *e = DP(o)->array[h];
	knh_hashentry_t **prev_next = &(DP(o)->array[h]);

	while(e != NULL) {
		if(e->hcode == hcode
				&& knh_Object_cid(sfp[1].o) == knh_Object_cid(e->key)
				&& knh_stack_equals(ctx, sfp + 1, e->key)) {
			prev_next[0] = e->next;
			knh_hashentry_collect(ctx, o, e);
			DP(o)->size--;
		}
		prev_next = &(e->next);
		e = e->next;
	}
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## method void HashMap.set(T1! key, T2 value);

static METHOD HashMap_set(Ctx *ctx, knh_sfp_t *sfp)
{
	if(IS_NULL(sfp[2].o)) {
		HashMap_remove(ctx, sfp);
	}
	else {
		knh_Hash_t *o = (knh_Hash_t*)sfp[0].o;
		knh_hashcode_t hcode = knh_stack_hashCode(ctx, sfp + 1);
		knh_uintptr_t h =  hcode % DP(o)->hmax;
		knh_hashentry_t *e = DP(o)->array[h];

		while(e != NULL) {
			if(e->hcode == hcode
					&& knh_Object_cid(sfp[1].o) == knh_Object_cid(e->key)
					&& knh_stack_equals(ctx, sfp + 1, e->key)) {
				knh_stack_boxing(ctx, sfp+2);
				KNH_SETv(ctx, e->value, sfp[2].o);
				KNH_RETURN_void(ctx, sfp);
			}
			e = e->next;
		}

		/* add newentry */ {
			e = new_hashentry(ctx, o);
			e->hcode = hcode;
			knh_stack_boxing(ctx, sfp+1);
			KNH_INITv(e->key, sfp[1].o);
			knh_stack_boxing(ctx, sfp+2);
			KNH_INITv(e->value, sfp[2].o);
			e->next = DP(o)->array[h];
			DP(o)->array[h] = e;
			DP(o)->size++;
		}
		KNH_RETURN_void(ctx, sfp);
	}
}

/* ------------------------------------------------------------------------ */

///* ======================================================================== */
///* [constructor] */
//
///* ------------------------------------------------------------------------ */
////## method HashSet! HashSet.new(Int initialCapacity=0);
//
//HashSet *knh_HashSet_new(Ctx *ctx, HashSet *b, size_t initialCapacity)
//{
//	if(initialCapacity > DP(b)->capacity) {
//		knh_HashSet_resize(ctx, b, (size_t)initialCapacity);
//	}
//	return b;
//}
//
///* ------------------------------------------------------------------------ */
///* ======================================================================== */
///* [method] */
//
///* ------------------------------------------------------------------------ */
////## method Boolean HashSet.opHas(T1 key);
//
//
//knh_bool_t knh_HashSet_opHas(Ctx *ctx, HashSet *b, knh_hashcode_t key, Object *keyobj)
//{
//	return (knh_HashSet_get(ctx, b, key, keyobj) != 0);
//}
//
///* ------------------------------------------------------------------------ */
///* ======================================================================== */
///* [mapping] */
//
///* ------------------------------------------------------------------------ */
///* @map HashSet Iterator! */
//
//Iterator* knh_HashSet_Iterator(Ctx *ctx, HashSet *o, Mapper *map)
//{
//	return new_Iterator(ctx, ctx->share->ClassTable[o->h.cid].p1, o, knh_HashMap_key_next);
//}
//
///* ======================================================================== */
///* [movabletext] */
//
////## method void HashSet.%dump(OutputStream w, String m);
//
//void knh_HashSet__dump(Ctx *ctx, HashSet *b, OutputStream *w, String *m)
//{
////	knh_hashsete_t *cur, *next;
////	size_t c = 0, i;
////	knh_fputc(ctx, w, '[');
////	for(i = 0; i < DP(b)->capacity; i++) {
////		cur = DP(b)->array[i];
////		while(cur != NULL) {
////			next = cur->next;
////			if(c > 0) knh_print_delim(ctx,w);
////			if(!knh_ucheck_todump(lv, c)) {
////				knh_print_dots(ctx, w);
////				break;
////			}
////			knh_Object__dump(ctx, cur->h.keyobj, w, lv+1);
////			if(cur->value != 1) knh_printf(ctx, w, "<%d>", (knh_intptr_t)cur->value);
////			c++;
////			cur = next;
////		}
////	}
////	knh_fputc(ctx, w, ']');
//}
//
//
///* ------------------------------------------------------------------------ */


#endif/*KNH_CC_METHODAPI*/

#ifdef __cplusplus
}
#endif
