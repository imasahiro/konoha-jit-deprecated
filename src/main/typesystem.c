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
/* [instaceof] */

knh_bool_t knh_class_instanceof(Ctx *ctx, knh_class_t scid, knh_class_t tcid)
{
	KNH_ASSERT_cid(scid);
	KNH_ASSERT_cid(tcid);
	//DBG2_P("@@@@@@@@@ %s %s", CLASSN(ctx->share->ClassTable[scid].bcid), CLASSN(tcid));
	if(scid == CLASS_Any) return 0;
	if(ctx->share->ClassTable[scid].bcid == tcid /* Int:km Int */
			|| scid == tcid
			|| tcid == CLASS_Object  /* Int    Object */
			|| tcid == CLASS_Any ) return 1;   /* Any */

	if(ctx->share->ClassTable[scid].bcid == ctx->share->ClassTable[tcid].bcid && knh_class_isGenerics(scid) && knh_class_isGenerics(tcid)) {
		return knh_class_instanceof(ctx, ctx->share->ClassTable[scid].p1, ctx->share->ClassTable[tcid].p1) &&
			knh_class_instanceof(ctx, ctx->share->ClassTable[scid].p2, ctx->share->ClassTable[tcid].p2);
	}

	while(scid != CLASS_Object) {
		knh_ClassMap_t *cmap = ctx->share->ClassTable[scid].cmap;
		int i;
		for(i = 0; i < cmap->size; i++) {
			knh_Mapper_t *mpr = cmap->maplist[i];
			if(DP(mpr)->tcid == tcid && DP(mpr)->flag == FLAG_Mapper_Interface) return 1;
		}
		scid = ctx->share->ClassTable[scid].supcid;
		if(scid == tcid) return 1;
	}
	return 0;
}

/* ------------------------------------------------------------------------ */


knh_bool_t knh_Object_opTypeOf(Ctx *ctx, Object *o, knh_type_t t)
{
	if(IS_NULL(o)) {
		return IS_NATYPE(t);
	}
	return knh_class_instanceof(ctx, o->h.cid, CLASS_type(t));
}

/* ------------------------------------------------------------------------ */

knh_class_t knh_class_parent(Ctx *ctx, knh_class_t c1, knh_class_t c2)
{
	knh_class_t p1, p2;
	KNH_ASSERT_cid(c1);
	KNH_ASSERT_cid(c2);
	if(c1 == c2) return c1;
	p1 = ClassTable(c1).supcid;
	p2 = ClassTable(c2).supcid;
	if(p1 == c2 || p1 == p2) return p1;
	if(p2 == c1) return p2;
	if(ClassTable(c1).bcid == ClassTable(c2).bcid) return ClassTable(c1).bcid;
	TODO();
	return CLASS_Object;
}

///* ------------------------------------------------------------------------ */
//
//
//knh_bool_t knh_class_isSynonym(Ctx *ctx, knh_class_t scid, knh_class_t tcid)
//{
//	KNH_ASSERT_cid(scid);
//	KNH_ASSERT_cid(tcid);
//	return 0;
//}


/* ------------------------------------------------------------------------ */
/* [coercion] */

///* ------------------------------------------------------------------------ */
///* @method Object Object.opAs(Class! c) */
//
//METHOD knh__Object_opAs(Ctx *ctx, knh_sfp_t *sfp)
//{
//	KNH_RETURN(ctx, sfp, knh_Object_opAs(ctx, sfp[0].o, p_cid(sfp[1])));
//}
//
///* ------------------------------------------------------------------------ */
//
//Object* knh_Object_opAs(Ctx *ctx, Object *o, knh_class_t tcid)
//{
//	KNH_ASSERT_cid(tcid);
//	if(IS_NULL(o)) {
//		return o;  /* @see */
//	}
//	else {
//		knh_class_t scid = knh_Object_cid(o);
//		if(scid == tcid || ctx->share->ClassTable[scid].bcid == tcid || knh_class_instanceof(ctx, scid, tcid)) {
//			return o;
//		}
//		else if(ctx->share->ClassTable[tcid].bcid == scid) {
//			if(IS_String(o)) {
//				String *s = (String*)o;
//				return UP(new_StringX(ctx, tcid, knh_String_tobytes(s), s));
//			}
//			return o;
//		}
//		else {
//			knh_sfp_t *lsfp = KNH_LOCAL(ctx);
//			DBG2_P("COERCION %s -> %s", CLASSN(scid), CLASSN(tcid));
//			KNH_LPUSH(ctx, o);
//			VM_MAP(ctx, tcid);
//			o = ctx->esp[0].o;
//			KNH_LOCALBACK(ctx, lsfp);
//			return o;
//		}
//	}
//}
//

/* ======================================================================== */
/* [Exception_type] */

knh_Exception_t* new_Exception__type(Ctx *ctx, Object *value, knh_type_t spec_type)
{
	if(IS_NULL(value)) {
		return new_NullException (ctx, value);
	}
	else {
		char buf[CLASSNAME_BUFSIZ];
		knh_snprintf(buf, sizeof(buf), "Type!!: req=%s given=%s", CLASSN(CLASS_type(spec_type)), CLASSN(knh_Object_cid(value)));
		return new_Exception__s(ctx, buf);
	}
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
