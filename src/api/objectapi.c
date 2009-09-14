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
/* [Object] */

/* ------------------------------------------------------------------------ */
//## @Const @NullBase method Class! Object.getClass();

static METHOD Object_getClass(Ctx *ctx, knh_sfp_t *sfp)
{
	KNH_RETURN(ctx, sfp, new_Class(ctx, (sfp[0].o)->h.cid));
}

/* ------------------------------------------------------------------------ */
//## @Const @NullBase @Hidden method Boolean! Object.isNull();

static METHOD Object_isNull(Ctx *ctx, knh_sfp_t *sfp)
{
	KNH_RETURN_Boolean(ctx, sfp, IS_NULL(sfp[0].o));
}

/* ------------------------------------------------------------------------ */
//## @Const @NullBase @Hidden method Boolean! Object.isNotNull();

static METHOD Object_isNotNull(Ctx *ctx, knh_sfp_t *sfp)
{
	KNH_RETURN_Boolean(ctx, sfp, IS_NOTNULL(sfp[0].o));
}

/* ------------------------------------------------------------------------ */
//## @Const method String! Object.getKey();

static METHOD Object_getKey(Ctx *ctx, knh_sfp_t *sfp)
{
	KNH_RETURN(ctx, sfp, StructTable(knh_Object_bcid(sfp[0].o)).fgetkey(ctx, sfp));
}

/* ======================================================================== */
/* [Class] */

/* ------------------------------------------------------------------------ */
//## @Hidden method Array! Class.domain();

static METHOD Class_domain(Ctx *ctx, knh_sfp_t *sfp)
{
	KNH_RETURN(ctx, sfp, knh_getClassDomain(ctx, (sfp[0].c)->cid));
}

/* ======================================================================== */
/* [Method] */

/* ------------------------------------------------------------------------ */
//## @Const method Boolean! Method.isAbstract();

static METHOD Method_isAbstract(Ctx *ctx, knh_sfp_t *sfp)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_Method_isAbstract(sfp[0].mtd));
}

/* ------------------------------------------------------------------------ */
//## @Const method String Method.getName();

static METHOD Method_getName(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_String_t *s = knh_Method_getName(ctx, sfp[0].mtd);
	KNH_RETURN(ctx, sfp, s);
}

/* ------------------------------------------------------------------------ */
//## method T0 Closure.invoke(T1 x, T2 y, T3 z, ...);

static METHOD Closure_invoke(Ctx *ctx, knh_sfp_t *sfp)
{
	DBG2_ASSERT(IS_bClosure(sfp[0].cc));
	KNH_INVOKE(ctx, sfp);
}

/* ------------------------------------------------------------------------ */

#endif/* KNH_CC_METHODAPI*/

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
