/****************************************************************************
 * KONOHA COPYRIGHT, LICENSE NOTICE, AND DISCRIMER
 *
 * Copyright (c) 2006-2010, Kimio Kuramitsu <kimio at ynu.ac.jp>
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
	DBG2_ASSERT_cid(scid);
	DBG2_ASSERT_cid(tcid);
	if(scid == tcid || tcid == CLASS_Object || tcid == CLASS_Any ) return 1;
	if(ClassTable(scid).bcid == tcid) return 1; /* Int:km Int */
	if(knh_class_isGenerics(scid)) {
		// Immutable simulates covariance of generics
		if(knh_class_isImmutable(scid) && ClassTable(scid).bcid == ClassTable(tcid).bcid) {
			// Iterator<C> Iterator<D>;
			return (knh_class_instanceof(ctx, ClassTable(scid).p1, ClassTable(tcid).p1) &&
					knh_class_instanceof(ctx, ClassTable(scid).p2, ClassTable(tcid).p2));
		}
	}
	else {
		while(scid != CLASS_Object) {
			knh_ClassMap_t *cmap = ClassTable(scid).cmap;
			int i;
			for(i = 0; i < cmap->size; i++) {
				knh_Mapper_t *mpr = cmap->maplist[i];
				if(DP(mpr)->tcid == tcid && DP(mpr)->flag == FLAG_Mapper_Interface) return 1;
			}
			scid = ClassTable(scid).supcid;
			if(scid == tcid) return 1;
		}
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
	DBG2_ASSERT_cid(c1);
	DBG2_ASSERT_cid(c2);
	if(c1 == c2) return c1;
	p1 = ClassTable(c1).supcid;
	p2 = ClassTable(c2).supcid;
	if(p1 == c2 || p1 == p2) return p1;
	if(p2 == c1) return p2;
	if(ClassTable(c1).bcid == ClassTable(c2).bcid) return ClassTable(c1).bcid;
	TODO();
	return CLASS_Object;
}

/* ------------------------------------------------------------------------ */

knh_Exception_t* new_Exception__type(Ctx *ctx, Object *value, knh_type_t spec_type)
{
	if(IS_NULL(value)) {
		return new_Exception__T(ctx, "Null!!");
	}
	else {
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		knh_printf(ctx, cwb->w, "Type!!: requested=%T given=%C", spec_type, knh_Object_cid(value));
		return knh_cwb_newException(ctx, cwb);
	}
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
