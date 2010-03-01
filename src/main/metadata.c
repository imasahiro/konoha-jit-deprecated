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

/* ======================================================================== */

//static
//knh_bool_t knh_isKeyUniqueClass(Ctx *ctx, knh_class_t cid)
//{
//	return 1;
//}

/* ------------------------------------------------------------------------ */

Object *knh_getKeyObject(Ctx *ctx, knh_String_t *key)
{
	knh_class_t cid;
	knh_bytes_t k = __tobytes(key);
	k = knh_bytes_parsecid(ctx, k, ':', &cid);
	if(cid == CLASS_unknown) cid = CLASS_Object;
	DBG2_ASSERT_cid(cid);
	{
		knh_ClassTable_t *t = (knh_ClassTable_t*)(&ClassTable(cid));
		if(t->dataKeyMap == NULL) return KNH_NULL;
		return knh_DictMap_get__b(ctx, t->dataKeyMap, k);
	}
}

/* ------------------------------------------------------------------------ */

void knh_addKeyObject(Ctx *ctx, Object *value)
{
	knh_String_t *key = knh_Object_key(ctx, value);
	knh_bytes_t k = __tobytes(key);
	knh_class_t cid;
	if(knh_bytes_index(k, ':') > 0) {
		knh_bytes_parsecid(ctx, k, ':', &cid);
		if(cid == CLASS_unknown) cid = CLASS_Object;
	}
	else {
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		cid = knh_Object_cid(value);
		knh_write_cid(ctx, cwb->w, knh_Object_cid(value));
		knh_putc(ctx, cwb->w, ':');
		knh_write(ctx, cwb->w, k);
		key = knh_cwb_newString(ctx, cwb);
	}
	{
		knh_ClassTable_t *t = (knh_ClassTable_t*)(&ClassTable(cid));
		if(t->dataKeyMap == NULL) {
			DBG2_P("initiating dataKeyMap of %s", CLASSN(cid));
			KNH_INITv(t->dataKeyMap, new_DictMap0(ctx, 4));
		}
		return knh_DictMap_set(ctx, t->dataKeyMap, key, value);
	}
}

/* ======================================================================== */



///* [global] */
//
//#define tmeta(ctx) ((Context*)ctx)->runtime->tmetaHashMap
//
///* ======================================================================== */
///* [methods] */
//
//void knh_metadata_set(Ctx *ctx, Object *b, String *key, Any *value)
//{
//	DictMap *dm = (DictMap*)knh_HashMap_get(ctx, tmeta(ctx), (knh_uint_t)b, NULL);
//	if(IS_NOTNULL(value)) {
//		if(IS_NULL(dm)) {
//			dm = new_DictMap(ctx, 2);
//			knh_HashMap_put(ctx, tmeta(ctx), (knh_uint_t)b, NULL, dm);
//			knh_Object_setMetaData(b, 1);
//		}
//		knh_DictMap_put(ctx, dm, key, value);
//	}
//	else{
//		if(IS_NOTNULL(dm)) {
//			knh_DictMap_remove(ctx, dm, key);
//		}
//	}
//}
//
///* ------------------------------------------------------------------------ */
//
//Any *knh_metadata_get(Ctx *ctx, Object *b, knh_bytes_t key)
//{
//	if(knh_Object_hasMetaData(b)) {
//		DictMap *dm = (DictMap*)knh_HashMap_get(ctx, tmeta(ctx), (knh_uint_t)b, NULL);
//		if(IS_NOTNULL(dm)) {
//			return knh_DictMap_get__b(ctx,  dm, key);
//		}
//	}
//	return KNH_NULL;
//}
//
///* ------------------------------------------------------------------------ */
//
//void knh_metadata_clear(Ctx *ctx, Object *b)
//{
//	DBG2_P("Removing metadata %p ..\n", b);
//	knh_Object_setMetaData(b, 0);
//	knh_HashMap_remove(ctx, tmeta(ctx), (knh_uint_t)b, NULL);
//}
//
///* ------------------------------------------------------------------------ */
//
////
////void knh_Object_setmeta(Ctx *ctx, Object *b, DictMap *dm)
////{
////	CHECK_knh_Object_setmeta;
////#ifdef OF_METADATA
////	KNH_ASSERT(!knh_Object_hasMetaData(b));
////	knh_HashMap_put(ctx, tmeta(ctx), (knh_uint_t)b, NULL, dm);
////	knh_Object_setflag(b, OF_METADATA);
////#endif
////}
////
/////* ------------------------------------------------------------------------ */
////
/////* @method[PRIVATE] DictMap Object.meta() */
////
////
////DictMap *knh_Object_meta(Ctx *ctx, Object *b)
////{
////#ifdef OF_METADATA
////	if(knh_Object_isflag(b, OF_METADATA)) {
////		return knh_HashMap_get(ctx, tmeta(ctx), (knh_uint_t)b, NULL);
////	}
////#endif
////	return KNH_NULL;
////}
//


#ifdef __cplusplus
}
#endif
