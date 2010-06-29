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

#ifdef K_USING_DEFAULTAPI

/* ======================================================================== */
/* [DictMap] */

/* ------------------------------------------------------------------------ */
//## method Boolean DictMap.opHAS(String key);

static METHOD DictMap_opHAS(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	
	RETURNb_(knh_DictMap_index((knh_DictMap_t*)sfp[0].o, S_tobytes(sfp[1].s)) != -1);
}

/* ------------------------------------------------------------------------ */
//## method T1? DictMap.get(String key);

static METHOD DictMap_get(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	Object *v = knh_DictMap_getNULL(ctx, sfp[0].dmap, S_tobytes(sfp[1].s));
	if(v == NULL) {
		v = KNH_NULVAL(knh_class_p1(knh_Object_cid(sfp[0].o)));
	}
	RETURN_(v);
}

/* ------------------------------------------------------------------------ */
//## method void DictMap.set(String key, T1? value);

static METHOD DictMap_set(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_stack_boxing(ctx, sfp + 2);
	knh_DictMap_set_(ctx, sfp[0].dmap, sfp[1].s, sfp[2].o);
	RETURNvoid_();
}

/* ------------------------------------------------------------------------ */
//## method void DictMap.remove(String key);

static METHOD DictMap_remove(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_DictMap_remove(ctx, sfp[0].dmap, S_tobytes(sfp[1].s));
	RETURNvoid_();
}

/* ------------------------------------------------------------------------ */
//## method void DictMap.clear();

static METHOD DictMap_clear(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_DictMap_t *o = (knh_DictMap_t*)sfp[0].o;
	knh_DictMap_clear(ctx, o);
	RETURNvoid_();
}

/* ------------------------------------------------------------------------ */
//## method String[] DictMap.keys();

static METHOD DictMap_keys(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_DictMap_t *o = (knh_DictMap_t*)sfp[0].o;
	knh_Array_t *a = new_Array(ctx, CLASS_String, o->size);
	size_t i;
	for(i=0; i < o->size; i++) {
		knh_Array_add_(ctx, a, UP(o->list[i].key));
	}
	RETURN_(UP(a));
}

/* ------------------------------------------------------------------------ */
//## method T1[] DictMap.values();

static METHOD DictMap_values(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	
	knh_DictMap_t *o = (knh_DictMap_t*)sfp[0].o;
	knh_Array_t *a = new_Array0(ctx, o->size);
	size_t i;
	for(i=0; i < o->size; i++) {
		knh_Array_add_(ctx, a, UP(o->list[i].value));
	}
	RETURN_(UP(a));
}

///* ======================================================================== */
///* [DictSet] */
//
///* ------------------------------------------------------------------------ */
////## method This DictSet.new(Int n) */
//
//static METHOD DictSet_new(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	DictSet *o = (DictSet*)sfp[0].o;
//	size_t init = IS_NULL(sfp[1].o) ? K_DICT_INITSIZE: Int_to(size_t, sfp[1]);
//	if(init > 0) {
//		o->_list = knh_dictset_malloc(ctx, init);
//		o->size = 0;
//	}
//	RETURN_(o);
//}
//
///* ======================================================================== */
///* [method] */
//
///* ------------------------------------------------------------------------ */
////## method void DictSet.add(String key) */
//
//static METHOD DictSet_add(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	DictSet *o = (DictSet*)sfp[0].o;
//	if(!knh_Object_isImmutable(o)) {
//		knh_DictSet_add(ctx, o, sfp[1].s);
//	}
//	RETURNvoid_();
//}
//
///* ------------------------------------------------------------------------ */
////## method Int DictSet.get(String key) */
//
//static METHOD DictSet_get(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	RETURNi_(knh_DictSet_get(ctx, (DictSet*)sfp[0].o, sfp[1].s));
//}
//
///* ------------------------------------------------------------------------ */
////## method Boolean DictSet.opHAS(String key) */
//
//static METHOD DictSet_opHAS(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	RETURNb_(knh_DictSet_get__b((DictSet*)sfp[0].o, S_tobytes(sfp[1].s)) != 0);
//}
//
///* ------------------------------------------------------------------------ */
////## method void DictSet.set(String key, Int value) */
//
//static METHOD DictSet_set(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	DictSet *o = (DictSet*)sfp[0].o;
//	if(!knh_Object_isImmutable(o)) {
//		knh_DictSet_set(ctx, o, sfp[1].s, Int_to(size_t, sfp[2]));
//	}
//	RETURNvoid_();
//}
//
///* ------------------------------------------------------------------------ */
////## method void DictSet.remove(String key) */
//
//static METHOD DictSet_remove(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	DictSet *o = (DictSet*)sfp[0].o;
//	if(!knh_Object_isImmutable(o)) {
//		knh_DictSet_remove(ctx, o, sfp[1].s);
//	}
//	RETURNvoid_();
//}
//
///* ------------------------------------------------------------------------ */
////## method void DictSet.clear() */
//
//static METHOD DictSet_clear(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	DictSet *o = (DictSet*)sfp[0].o;
//	if(!knh_Object_isImmutable(o)) {
//		knh_DictSet_clear(ctx, o);
//	}
//	RETURNvoid_();
//}
//
///* ======================================================================== */
////## method Int DictSet.getSize() */
//
//static METHOD DictSet_getSize(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	RETURNi_(knh_DictSet_size((DictSet*)sfp[0].o));
//}
//
///* ------------------------------------------------------------------------ */
////## method void DictSet.opLSFT(String v) @VARGs */
//
//static METHOD DictSet_opLSFT(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	DictSet *o = (DictSet*)sfp[0].o;
//	if(!knh_Object_isImmutable(o)) {
//		knh_sfp_t *v = sfp + 1;
//		int ac = knh_stack_argc(ctx, v);
//		size_t i;
//		for(i = 0; i < ac; i++) {
//			if(IS_NULL(v[1].o)) continue;
//			knh_DictSet_append(ctx, o, v[i].s, 1);
//		}
//		knh_DictSet_sort(o);
//	}
//	RETURNvoid_();
//}
//
///* ======================================================================== */
///* [movabletext] */


/* ------------------------------------------------------------------------ */

#endif/*K_USING_DEFAULTAPI*/

#ifdef __cplusplus
}
#endif
