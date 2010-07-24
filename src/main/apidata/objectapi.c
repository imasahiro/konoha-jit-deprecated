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

#ifdef K_USING_DEFAULTAPI

/* ------------------------------------------------------------------------ */
//## @Const method Class! Object.getClass();

static METHOD Object_getClass(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURN_(new_Type(ctx, (sfp[0].o)->h.cid));
}

/* ------------------------------------------------------------------------ */
//## method Int Object.hashCode();

static METHOD Object_hashCode(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_hashcode_t h =
		(knh_hashcode_t)ClassTBL(knh_Object_bcid(sfp[0].o)).cspi->hashkey(ctx, sfp, KNH_FOBJECT_HASH);
	RETURNi_(h);
}

/* ------------------------------------------------------------------------ */
//## @Const @Hidden method Boolean Object.isNull();

static METHOD Object_isNull(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(IS_NULL(sfp[0].o));
}

/* ------------------------------------------------------------------------ */
//## @Const @Hidden method Boolean Object.isNotNull();

static METHOD Object_isNotNull(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(IS_NOTNULL(sfp[0].o));
}

/* ------------------------------------------------------------------------ */
//## @Const @Virtual method String Object.getKey();

static METHOD Object_getKey(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_String_t *s =
		(knh_String_t*)ClassTBL(knh_Object_bcid(sfp[0].o)).cspi->hashkey(ctx, sfp, KNH_FOBJECT_KEY);
	KNH_ASSERT(IS_String(s));
	RETURN_(s);
}

///* ------------------------------------------------------------------------ */
////## @Hidden method Array! Class.domain();
//
//static METHOD Class_domain(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	RETURN_(knh_getClassDomain(ctx, (sfp[0].c)->cid));
//}

/* ======================================================================== */
/* [Method] */

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Method.isAbstract();

static METHOD Method_isAbstract(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_Method_isAbstract(sfp[0].mtdOBJ));
}

/* ------------------------------------------------------------------------ */
//## @Const method String Method.getName();

static METHOD Method_getName(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_write_mn(ctx, cwb->w, DP(sfp[0].mtdOBJ)->mn);
	RETURN_(knh_cwb_newString(ctx, cwb));
}

///* ------------------------------------------------------------------------ */
////## @Const @Hidden method String Method.format(Any v, ...);
//
//static METHOD Method_format(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
//	long ac = knh_stack_argc(ctx, sfp+1);
//	knh_Method_t *mtdf = sfp[0].mtdOBJ;
//	if(ac == 1) {
//		knh_class_t cid = knh_Object_cid(sfp[1].o);
//		if(cid != DP(mtdf)->cid) {
//			mtdf = knh_getSystemFormatter(ctx, cid, DP(mtdf)->mn);
//		}
//		KNH_SETv(ctx, sfp[2].o, cwb->w);
//		KNH_SCALL(ctx, sfp, -2, mtdf, 1);
//	}
//	else {
//		TODO();
//	}
//	RETURN_(knh_cwb_newString(ctx, cwb));
//}
//
///* ------------------------------------------------------------------------ */
////## @Const @Hidden method String Method.format:FINAL(Object v, ...);
//
//static METHOD Method_format__FINAL(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
//	long ac = knh_stack_argc(ctx, sfp+1);
//	knh_Method_t *mtdf = sfp[0].mtdOBJ;
//	if(ac == 1) {
//		KNH_SETv(ctx, sfp[2].o, cwb->w);
//		KNH_SCALL(ctx, sfp, -2, mtdf, 1);
//	}
//	else {
//		DBG_P("ac=%ld", ac);
//		TODO();
//	}
//	RETURN_(knh_cwb_newString(ctx, cwb));
//}

///* ------------------------------------------------------------------------ */
////## @Const method void Method.setTrace(Int trace);
//
//static METHOD Method_setTrace(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	
//	knh_Method_trace(ctx, sfp[0].mtd, (int)sfp[1].ivalue);
//	RETURNvoid_();
//}

/* ------------------------------------------------------------------------ */
//## @Hidden @Private method Any Func.invoke(Any x);

static METHOD Func_invoke(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Func_t* fo = sfp[0].fo;
	if(fo->baseNULL != NULL) {
		klr_mov(ctx, sfp[0].o, fo->baseNULL);
	}
	klr_setcallmtd(ctx, sfp[K_MTDIDX], fo->mtd);
	KNH_SELFCALL(ctx, sfp, fo->mtd, rix);
}

///* ------------------------------------------------------------------------ */
////## method T1 Thunk.eval();
//
//static METHOD Thunk_eval(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	
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
//static METHOD Thunk_value(Ctx *ctx, knh_sfp_t *sfp, long rix)
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

#endif/* K_USING_DEFAULTAPI*/

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
