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

#define USE_STEXT 1
//#define USE_B     1
//#define USE_bytes_strcmp    1
//#define USE_bytes_parseint  1

#include"commons.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif


/* ======================================================================== */
/* [getkey] */

knh_String_t *knh_Object_getkey(Ctx *ctx, Object *o)
{
	knh_sfp_t lsfp;
	lsfp.o = o;
	lsfp.data = ((knh_Int_t*)o)->n.data;
	return (knh_String_t*)ClassTBL(knh_Object_bcid(o)).cspi->hashkey(ctx, &lsfp, KNH_FOBJECT_KEY);
}

/* ======================================================================== */
/* [copy] */

Object *knh_Object_copy(Ctx *ctx, Object *b)
{
	if(knh_Object_isImmutable(b)) {
		return b;
	}
	DBG_P("!IMMUTABLE %s", CLASS__(knh_Object_cid(b)));
	return b;
}

/* ======================================================================== */
/* [commons] */

//knh_hashcode_t knh_Object_hashCode(Ctx *ctx, Object *o)
//{
//	knh_sfp_t lsfp;
//	lsfp.o = o;
//	lsfp.data = ((knh_Int_t*)o)->n.data;
//	return (knh_hashcode_t)ClassTBL(knh_Object_bcid(o)).cspi->hashkey(ctx, &lsfp, KNH_FOBJECT_HASH);
//}

/* ------------------------------------------------------------------------ */

knh_String_t *knh_Object_key(Ctx *ctx, Object *o)
{
	knh_String_t *s;
	knh_sfp_t *lsfp = ctx->esp + 1;
	KNH_SETv(ctx, lsfp[0].o, o);
	lsfp[0].data = knh_Object_data(o);
	s = (knh_String_t*)ClassTBL(o->h.bcid).cspi->hashkey(ctx, lsfp, KNH_FOBJECT_KEY);
	KNH_SETv(ctx, lsfp[-1].o, s);
	return s;
}
/* ------------------------------------------------------------------------ */

int knh_Object_compareTo(Ctx *ctx, Object *o1, Object *o2)
{
	knh_class_t bcid1 = o1->h.bcid;
	knh_class_t bcid2 = o2->h.bcid;
	int res;
	if(bcid1 == bcid2) {
		res = ClassTBL(bcid1).cspi->compareTo(ctx, o1, o2);
	}
	else {
		res = (int)(o1 - o2);
		DBG_P("compared %s %s res=%d", O__(o1), O__(o2), res);
	}
	return res;
}

/* ------------------------------------------------------------------------ */

int knh_Object_compareTo2(Ctx *ctx, Object **o1, Object **o2)
{
	Object* _o1 = *((Object**) o1);
	Object* _o2 = *((Object**) o2);
	knh_class_t bcid1 = _o1->h.bcid;
	knh_class_t bcid2 = _o2->h.bcid;
	int res;
	if(bcid1 == bcid2) {
		res = ClassTBL(bcid1).cspi->compareTo(ctx, _o1, _o2);
	}
	else {
		res = (int)(_o1 - _o2);
		DBG_P("compared %s %s res=%d", O__(_o1), O__(_o2), res);
	}
	return res;
}

/* ------------------------------------------------------------------------ */

knh_bool_t knh_Object_equals(Ctx *ctx, Object *o1, Object *o2)
{
	return (knh_Object_compareTo(ctx, o1, o2) == 0);
}

/* ------------------------------------------------------------------------ */

knh_bytes_t knh_Object_tobytes(Ctx *ctx, Object *o)
{
	switch(o->h.bcid) {
		case CLASS_String : return S_tobytes((knh_String_t*)o);
		case CLASS_Bytes : return BA_tobytes((knh_Bytes_t*)o);
	}
	return STEXT("");
}

/* ------------------------------------------------------------------------ */

void knh_Object_toNULL_(Ctx *ctx, Object *o)
{
	knh_Object_setNullObject(o, 1);
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
