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

#ifdef KNH_CC_METHODAPI

/* ======================================================================== */
/* [Object] */

/* ------------------------------------------------------------------------ */
//## @Const @NullBase method Class! Object.getClass();

static METHOD Object_getClass(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	KNH_CHKESP(ctx, sfp);
	KNH_RETURN(ctx, sfp, new_Type(ctx, (sfp[0].o)->h.cid));
}

/* ------------------------------------------------------------------------ */
//## @NullBase method Int Object.hashCode();

static METHOD Object_hashCode(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	KNH_CHKESP(ctx, sfp);
	knh_hashcode_t h =
		(knh_hashcode_t)ClassTable(knh_Object_bcid(sfp[0].o)).cspi->hashkey(ctx, sfp, KNH_FOBJECT_HASH);
	KNH_RETURNi(ctx, sfp, h);
}

/* ------------------------------------------------------------------------ */
//## @Const @NullBase @Hidden method Boolean! Object.isNull();

static METHOD Object_isNull(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	KNH_RETURNb_(ctx, sfp, IS_NULL(sfp[0].o));
}

/* ------------------------------------------------------------------------ */
//## @Const @NullBase @Hidden method Boolean! Object.isNotNull();

static METHOD Object_isNotNull(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	KNH_RETURNb_(ctx, sfp, IS_NOTNULL(sfp[0].o));
}

/* ------------------------------------------------------------------------ */
//## @Const @Virtual method String! Object.getKey();

static METHOD Object_getKey(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	KNH_CHKESP(ctx, sfp);
	knh_String_t *s =
		(knh_String_t*)ClassTable(knh_Object_bcid(sfp[0].o)).cspi->hashkey(ctx, sfp, KNH_FOBJECT_KEY);
	KNH_ASSERT(IS_String(s));
	KNH_RETURN(ctx, sfp, s);
}

/* ------------------------------------------------------------------------ */
//## @NullBase method String! Object.format(String fmt);

static METHOD Object_format(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	KNH_CHKESP(ctx, sfp);
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_Method_t *mtd = sfp[-1].mtd;
	if((mtd)->fcall_1 == Object_format) {
		knh_methodn_t mn = METHODN__s;
		TODO();
		mtd = knh_lookupFormatter(ctx, knh_Object_cid(sfp[0].o), mn);
		klr_mov(ctx, sfp[-1].o, mtd);
	}
	else if(!knh_Method_isVirtual(mtd)){
		mtd = knh_lookupFormatter(ctx, knh_Object_cid(sfp[0].o), DP(mtd)->mn);
		klr_mov(ctx, sfp[-1].o, mtd);
	}
	klr_mov(ctx, sfp[2].o, sfp[1].o);  // fmt
	klr_mov(ctx, sfp[1].o, cwb->w);
	KNH_SCALL(ctx, sfp, 0, 2);
	{
		knh_String_t *s = knh_cwb_newString(ctx, cwb);
		KNH_RETURN(ctx, sfp, s);
	}
}


/* ======================================================================== */
/* [Class] */

/* ------------------------------------------------------------------------ */
//## @Hidden method Array! Class.domain();

static METHOD Class_domain(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	KNH_CHKESP(ctx, sfp);
	KNH_RETURN(ctx, sfp, knh_getClassDomain(ctx, (sfp[0].c)->cid));
}

/* ======================================================================== */
/* [Method] */

/* ------------------------------------------------------------------------ */
//## @Const method Boolean! Method.isAbstract();

static METHOD Method_isAbstract(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	KNH_CHKESP(ctx, sfp);
	KNH_RETURNb(ctx, sfp, knh_Method_isAbstract(sfp[0].mtd));
}

/* ------------------------------------------------------------------------ */
//## @Const method String Method.getName();

static METHOD Method_getName(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	KNH_CHKESP(ctx, sfp);
	knh_String_t *s = knh_Method_getName(ctx, sfp[0].mtd);
	KNH_RETURN(ctx, sfp, s);
}

/* ------------------------------------------------------------------------ */
//## @Const method void Method.setTrace(Int trace);

static METHOD Method_setTrace(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	KNH_CHKESP(ctx, sfp);
	knh_Method_trace(ctx, sfp[0].mtd, (int)sfp[1].ivalue);
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## method T0 Closure.invoke(T1 x, T2 y, T3 z, ...);

static METHOD Closure_invoke(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	knh_Closure_t* cc = sfp[0].cc;
	DBG2_ASSERT(IS_bClosure(cc));
	KNH_TEST("0.7a");
	klr_mov(ctx, sfp[0].o, (cc)->base);
	klr_mov(ctx, sfp[-1].o, (cc)->mtd);
	KNH_SCALL0(ctx, sfp, -2);
}

///* ------------------------------------------------------------------------ */
////## method T1 Thunk.eval();
//
//static METHOD Thunk_eval(Ctx *ctx, knh_sfp_t *sfp METHODARG)
//{
//	KNH_CHKESP(ctx, sfp);
//	knh_Thunk_t *thk = (knh_Thunk_t*)sfp[0].o;
//	size_t i;
//	if((thk)->envsize > 0) {
//		for(i = 2; i < (thk)->envsize; i++) {
//			KNH_SETv(ctx, sfp[i].o, (thk)->envsfp[i].o);
//			sfp[i].data = (thk)->envsfp[i].data;
//		}
//		KNH_SCALL(ctx, sfp, 1, (thk)->envsfp[1].mtd, ((thk)->envsize-3));
//		KNH_SETv(ctx, (thk)->envsfp[0].o, sfp[1].o);
//		(thk)->envsfp[0].data = sfp[1].data;
//		knh_Thunk_setEvaluated(thk, 1);
//	}
//	KNH_SETv(ctx, sfp[-1].o, (thk)->envsfp[0].o);
//	sfp[-1].data = (thk)->envsfp[0].data;
//}

///* ------------------------------------------------------------------------ */
////## method T1 Thunk.value();
//
//static METHOD Thunk_value(Ctx *ctx, knh_sfp_t *sfp METHODARG)
//{
//	knh_Thunk_t *thk = (knh_Thunk_t*)sfp[0].o;
//	if(knh_Thunk_isEvaluated(thk)) {
//		KNH_SETv(ctx, sfp[-1].o, (thk)->envsfp[0].o);
//		sfp[-1].data = (thk)->envsfp[0].data;
//	}
//	else {
//		Thunk_eval(ctx, sfp);
//	}
//}

#endif/* KNH_CC_METHODAPI*/

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
