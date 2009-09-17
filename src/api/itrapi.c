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
/* [Array] */

/* ------------------------------------------------------------------------ */
//## @General mapper Array Iterator;

static MAPPER Array_Iterator(Ctx *ctx, knh_sfp_t *sfp)
{
	KNH_MAPPED(ctx, sfp, new_ArrayIterator(ctx, sfp[0].a));
}

/* ------------------------------------------------------------------------ */
//## method T1.. Array.opItr();

static METHOD Array_opItr(Ctx *ctx, knh_sfp_t *sfp)
{
	KNH_RETURN(ctx, sfp, new_ArrayIterator(ctx, sfp[0].a));
}

/* ------------------------------------------------------------------------ */
//## @General mapper IArray Iterator!;

static MAPPER knh_IArray_Iterator(Ctx *ctx, knh_sfp_t *sfp)
{
	KNH_MAPPED(ctx, sfp, new_ArrayIterator(ctx, (knh_Array_t*)sfp[0].ia));
}

/* ------------------------------------------------------------------------ */
//## method T1.. IArray.opItr();

static METHOD IArray_opItr(Ctx *ctx, knh_sfp_t *sfp)
{
	KNH_RETURN(ctx, sfp, new_ArrayIterator(ctx, (knh_Array_t*)sfp[0].ia));
}

/* ------------------------------------------------------------------------ */
//## @General mapper FArray Iterator!;

static MAPPER knh_FArray_Iterator(Ctx *ctx, knh_sfp_t *sfp)
{
	KNH_MAPPED(ctx, sfp, new_ArrayIterator(ctx, (knh_Array_t*)sfp[0].fa));
}

/* ------------------------------------------------------------------------ */
//## method T1.. FArray.opItr();

static METHOD FArray_opItr(Ctx *ctx, knh_sfp_t *sfp)
{
	KNH_RETURN(ctx, sfp, new_ArrayIterator(ctx, (knh_Array_t*)sfp[0].fa));
}

/* ======================================================================== */
/* [DictMap] */

//static ITRNEXT knh_DictMap_key_next(Ctx *ctx, knh_sfp_t *sfp, int n)
//{
//	Iterator *it = sfp[0].it;
//	DictMap *o = (DictMap*)DP(it)->source;
//	size_t pos;
//	for(pos = DP(it)->pos; pos < o->size; pos++) {
//		if(IS_NOTNULL(o->list[pos].value)) {
//			DP(it)->pos = pos+1;
//			KNH_ITRNEXT(ctx, sfp, n, (Object*)o->list[pos].key);
//		}
//	}
//	DP(it)->pos = pos;
//	KNH_ITREND(ctx, sfp, n);
//}

/* ------------------------------------------------------------------------ */

typedef void (*knh_fadd_dict)(Ctx *, knh_Array_t *a, knh_String_t *key, Object *value);

static void knh_DictMap_array(Ctx *ctx, knh_DictMap_t *d, knh_Array_t *a, knh_fadd_dict fadd)
{
	size_t i;
	knh_DictMap_sort(d);
	for(i = 0; i < d->size; i++) {
		Object *v = knh_DictMap_valueAt(d, i);
		if(IS_NOTNULL(v)) {
			fadd(ctx, a, knh_DictMap_keyAt(d, i), v);
		}
	}
}

static void knh_fadd_dictkey(Ctx *ctx, knh_Array_t *a, knh_String_t *key, Object *value)
{
	knh_Array_add(ctx, a, UP(key));
}

static void knh_fadd_dictentry(Ctx *ctx, knh_Array_t *a, knh_String_t *key, Object *value)
{
	knh_Pair_t *p = (knh_Pair_t*)new_hObject(ctx, FLAG_Pair, CLASS_Pair, knh_Object_p1(a));
	KNH_INITv(p->first, key);
	KNH_INITv(p->second, value);
	knh_Array_add(ctx, a, UP(p));
}

/* ------------------------------------------------------------------------ */
//## mapper DictMap Iterator!;
//## mapper DictMap String..!;

static MAPPER knh_DictMap_String__(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_Array_t *a = new_Array(ctx, CLASS_String, (sfp[0].dmap)->size);
	knh_DictMap_array(ctx, sfp[0].dmap, a, knh_fadd_dictkey);
	KNH_MAPPED(ctx, sfp, new_ArrayIterator(ctx, a));
}

/* ------------------------------------------------------------------------ */
//## method String.. DictMap.opItr() */

static METHOD DictMap_opItr(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_Array_t *a = new_Array(ctx, CLASS_String, (sfp[0].dmap)->size);
	knh_DictMap_array(ctx, sfp[0].dmap, a, knh_fadd_dictkey);
	KNH_RETURN(ctx, sfp, new_ArrayIterator(ctx, a));
}

/* ------------------------------------------------------------------------ */
//## method PairST1.. DictMap.opItr:2() */

static METHOD DictMap_opItr__2(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_class_t cid = knh_class_Generics(ctx, CLASS_Pair, CLASS_String, knh_Object_p1(sfp[0].dmap));
	knh_Array_t *a = new_Array(ctx, cid, (sfp[0].dmap)->size);
	knh_DictMap_array(ctx, sfp[0].dmap, a, knh_fadd_dictentry);
	KNH_RETURN(ctx, sfp, new_ArrayIterator(ctx, a));
}

/* ======================================================================== */
/* [mapping] */

//static
//ITRNEXT knh_HashMap_var_next(Ctx *ctx, knh_sfp_t *sfp, int n)
//{
//	Iterator *it = sfp[0].it;
//	knh_Hash_t *o = (knh_Hash_t*)DP(it)->source;
//	KNH_ASSERT(IS_bHashMap(o));
//	size_t pos = DP(it)->pos;
//	size_t max = (KNH_HASH_TABLESIZE / o->hashop->size) * DP(o)->tables_size;
//	while(pos < max) {
//		knh_hashentry_t *e = knh_hashentry_at(o, pos);
//		if(e != NULL) {
//			DP(it)->pos = pos + 1;
//			KNH_ITRNEXT(ctx, sfp, n, e->key);
//		}
//		pos++;
//	}
//	KNH_ITREND(ctx, sfp, n);
//}

/* ------------------------------------------------------------------------ */

typedef void (*knh_fadd_hash)(Ctx *, knh_Array_t *a, knh_hashentry_t *e);

static void knh_Hash_toArray(Ctx *ctx, knh_Hash_t *d, knh_Array_t *a, knh_fadd_hash fadd)
{
	size_t i, max = (KNH_HASH_TABLESIZE / d->hashop->size) * DP(d)->tables_size;
	for(i = 0; i < max; i++) {
		knh_hashentry_t *e = knh_hashentry_at(d, i);
		if(e != NULL && IS_NOTNULL(e->value)) {
			fadd(ctx, a, e);
		}
	}
}

static void knh_fadd_hashkey(Ctx *ctx, knh_Array_t *a, knh_hashentry_t *e)
{
	knh_Array_add(ctx, a, e->key);
}

static void knh_fadd_hashikey(Ctx *ctx, knh_Array_t *a, knh_hashentry_t *e)
{
	knh_IArray_add(ctx, (knh_IArray_t*)a, toint(e->key));
}

static void knh_fadd_hashfkey(Ctx *ctx, knh_Array_t *a, knh_hashentry_t *e)
{
	knh_FArray_add(ctx, (knh_FArray_t*)a, tofloat(e->key));
}

static void knh_fadd_hashentry(Ctx *ctx, knh_Array_t *a, knh_hashentry_t *e)
{
	knh_Pair_t *p = (knh_Pair_t*)new_hObject(ctx, FLAG_Pair, CLASS_Pair, knh_Object_p1(a));
	KNH_INITv(p->first, e->key);
	KNH_INITv(p->second, e->value);
	knh_Array_add(ctx, a, UP(p));
}

/* ------------------------------------------------------------------------ */
//## @Final mapper HashMap Iterator!
//## @Final mapper HashMap T1..

static MAPPER knh_HashMap_Iterator(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_Array_t *a = new_Array(ctx, knh_Object_p1(sfp[0].hmap), DP(sfp[0].hmap)->size);
	knh_fadd_hash fadd = knh_fadd_hashkey;
	if(IS_bIArray(a)) fadd = knh_fadd_hashikey;
	if(IS_bFArray(a)) fadd = knh_fadd_hashfkey;
	knh_Hash_toArray(ctx, (knh_Hash_t*)sfp[0].hmap, a, fadd);
	KNH_MAPPED(ctx, sfp, new_ArrayIterator(ctx, a));
}

/* ------------------------------------------------------------------------ */
//## method T1.. HashMap.opItr();

static METHOD HashMap_opItr(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_Array_t *a = new_Array(ctx, knh_Object_p1(sfp[0].hmap), DP(sfp[0].hmap)->size);
	knh_fadd_hash fadd = knh_fadd_hashkey;
	if(IS_bIArray(a)) fadd = knh_fadd_hashikey;
	if(IS_bFArray(a)) fadd = knh_fadd_hashfkey;
	knh_Hash_toArray(ctx, (knh_Hash_t*)sfp[0].hmap, a, fadd);
	KNH_RETURN(ctx, sfp, new_ArrayIterator(ctx, a));
}

/* ------------------------------------------------------------------------ */
//## method PairT1T2.. HashMap.opItr:2();

static METHOD HashMap_opItr__2(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_class_t cid = knh_class_Generics(ctx, CLASS_Pair, knh_Object_p1(sfp[0].hmap), knh_Object_p2(sfp[0].hmap));
	knh_Array_t *a = new_Array(ctx, cid, DP(sfp[0].hmap)->size);
	knh_Hash_toArray(ctx, (knh_Hash_t*)sfp[0].hmap, a, knh_fadd_hashentry);
	KNH_RETURN(ctx, sfp, new_ArrayIterator(ctx, a));
}

#endif/* KNH_CC_METHODAPI*/

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif