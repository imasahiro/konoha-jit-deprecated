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
/* [DictMap] */

/* ------------------------------------------------------------------------ */
//## method Boolean! DictMap.opHas(String! key);

static METHOD DictMap_opHas(Ctx *ctx, knh_sfp_t *sfp)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_DictMap_index((knh_DictMap_t*)sfp[0].o, __tobytes(sfp[1].s)) != -1);
}

/* ------------------------------------------------------------------------ */
//## method T1? DictMap.get(String! key);

static METHOD DictMap_get(Ctx *ctx, knh_sfp_t *sfp)
{
	KNH_RETURN(ctx, sfp, knh_DictMap_get(ctx, (knh_DictMap_t*)sfp[0].o, sfp[1].s));
}

/* ------------------------------------------------------------------------ */
//## method void DictMap.set(String! key, T1? value);

static METHOD DictMap_set(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_DictMap_t *o = (knh_DictMap_t*)sfp[0].o;
	knh_stack_boxing(ctx, sfp + 2);
	knh_DictMap_set(ctx, o, sfp[1].s, sfp[2].o);
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## method void DictMap.remove(String! key);

static METHOD DictMap_remove(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_DictMap_t *o = (knh_DictMap_t*)sfp[0].o;
	knh_DictMap_remove(ctx, o, sfp[1].s);
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## method void DictMap.clear();

static METHOD DictMap_clear(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_DictMap_t *o = (knh_DictMap_t*)sfp[0].o;
	knh_DictMap_clear(ctx, o);
	KNH_RETURN_void(ctx, sfp);
}

///* ======================================================================== */
///* [DictSet] */
//
///* ------------------------------------------------------------------------ */
////## method This! DictSet.new(Int n) */
//
//static METHOD DictSet_new(Ctx *ctx, knh_sfp_t *sfp)
//{
//	DictSet *o = (DictSet*)sfp[0].o;
//	size_t init = IS_NULL(sfp[1].o) ? KNH_DICT_INITSIZE: p_int(sfp[1]);
//	if(init > 0) {
//		o->_list = knh_dictset_malloc(ctx, init);
//		o->size = 0;
//	}
//	KNH_RETURN(ctx, sfp, o);
//}
//
///* ======================================================================== */
///* [method] */
//
///* ------------------------------------------------------------------------ */
////## method void DictSet.add(String! key) */
//
//static METHOD DictSet_add(Ctx *ctx, knh_sfp_t *sfp)
//{
//	DictSet *o = (DictSet*)sfp[0].o;
//	if(!knh_Object_isImmutable(o)) {
//		knh_DictSet_add(ctx, o, sfp[1].s);
//	}
//	KNH_RETURN_void(ctx, sfp);
//}
//
///* ------------------------------------------------------------------------ */
////## method Int! DictSet.get(String! key) */
//
//static METHOD DictSet_get(Ctx *ctx, knh_sfp_t *sfp)
//{
//	KNH_RETURN_Int(ctx, sfp, knh_DictSet_get(ctx, (DictSet*)sfp[0].o, sfp[1].s));
//}
//
///* ------------------------------------------------------------------------ */
////## method Boolean! DictSet.opHas(String! key) */
//
//static METHOD DictSet_opHas(Ctx *ctx, knh_sfp_t *sfp)
//{
//	KNH_RETURN_Boolean(ctx, sfp, knh_DictSet_get__b((DictSet*)sfp[0].o, __tobytes(sfp[1].s)) != 0);
//}
//
///* ------------------------------------------------------------------------ */
////## method void DictSet.set(String! key, Int! value) */
//
//static METHOD DictSet_set(Ctx *ctx, knh_sfp_t *sfp)
//{
//	DictSet *o = (DictSet*)sfp[0].o;
//	if(!knh_Object_isImmutable(o)) {
//		knh_DictSet_set(ctx, o, sfp[1].s, p_int(sfp[2]));
//	}
//	KNH_RETURN_void(ctx, sfp);
//}
//
///* ------------------------------------------------------------------------ */
////## method void DictSet.remove(String! key) */
//
//static METHOD DictSet_remove(Ctx *ctx, knh_sfp_t *sfp)
//{
//	DictSet *o = (DictSet*)sfp[0].o;
//	if(!knh_Object_isImmutable(o)) {
//		knh_DictSet_remove(ctx, o, sfp[1].s);
//	}
//	KNH_RETURN_void(ctx, sfp);
//}
//
///* ------------------------------------------------------------------------ */
////## method void DictSet.clear() */
//
//static METHOD DictSet_clear(Ctx *ctx, knh_sfp_t *sfp)
//{
//	DictSet *o = (DictSet*)sfp[0].o;
//	if(!knh_Object_isImmutable(o)) {
//		knh_DictSet_clear(ctx, o);
//	}
//	KNH_RETURN_void(ctx, sfp);
//}
//
///* ======================================================================== */
////## method Int! DictSet.getSize() */
//
//static METHOD DictSet_getSize(Ctx *ctx, knh_sfp_t *sfp)
//{
//	KNH_RETURN_Int(ctx, sfp, knh_DictSet_size((DictSet*)sfp[0].o));
//}
//
///* ------------------------------------------------------------------------ */
////## method[VARARGS] void DictSet.opLshift(String v) @VARARGS */
//
//static METHOD DictSet_opLshift(Ctx *ctx, knh_sfp_t *sfp)
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
//	KNH_RETURN_void(ctx, sfp);
//}
//
///* ======================================================================== */
///* [movabletext] */


/* ------------------------------------------------------------------------ */

#endif/*KNH_CC_METHODAPI*/

#ifdef __cplusplus
}
#endif
