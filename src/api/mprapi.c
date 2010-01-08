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
/* [toBoolean] */

/* ======================================================================== */
/* [toInt] */

/* ------------------------------------------------------------------------ */
//## @Const @Final mapper Float Int;

static MAPPER Float_Int(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	knh_int_t v = (knh_int_t)sfp[0].fvalue;
	KNH_MAPPED_Int(ctx, sfp, v);
}

/* ------------------------------------------------------------------------ */
//## @Const @Final mapper String Int?;

static MAPPER String_Int(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	knh_int_t v = 0;
	if(knh_bytes_parseint(__tobytes(sfp[0].s), &v)) {
		KNH_MAPPED_Int(ctx, sfp, v);
	}
	KNH_MAPPED(ctx, sfp, KNH_NULL);
}

/* ======================================================================== */
/* [toFloat] */

/* ------------------------------------------------------------------------ */
//## @Const @Final mapper Int Float;

static MAPPER Int_Float(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	knh_float_t v = (knh_float_t)sfp[0].ivalue;
	KNH_MAPPED_Float(ctx, sfp, v);
}

/* ------------------------------------------------------------------------ */
//## @Final @Const mapper String Float?;

static MAPPER String_Float(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	knh_float_t f = KNH_FLOAT_ZERO;
	if(knh_bytes_parsefloat(__tobytes(sfp[0].s), &f)) {
		KNH_MAPPED_Float(ctx, sfp, f);
	}
	else {
		KNH_MAPPED(ctx, sfp, KNH_NULL);
	}
}

/* ======================================================================== */
/* [toString] */

/* ------------------------------------------------------------------------ */
//## mapper Object String;

static MAPPER Object_String(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	Object *o = sfp[0].o;
	knh_Method_t *mtd = knh_lookupFormatter(ctx, knh_Object_cid(o), METHODN__s);
	if(IS_NULL(mtd)) {
		KNH_MAPPED(ctx, sfp, TS_EMPTY);
	}
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	KNH_MOV(ctx, sfp[2].o, o);
	KNH_MOV(ctx, sfp[3].o, cwb->w);
	KNH_MOV(ctx, sfp[4].o, KNH_NULL);
	KNH_SCALL(ctx, sfp, 1, mtd, 2);
	KNH_MAPPED(ctx, sfp, knh_cwb_newString(ctx, cwb));
}

/* ------------------------------------------------------------------------ */
//## @Const @Final @LossLess mapper Boolean String;

static MAPPER Boolean_String(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	knh_String_t *s = (p_bool(sfp[0])) ? TS_true : TS_false;
	KNH_MAPPED(ctx, sfp, s);
}

/* ------------------------------------------------------------------------ */
//## @Const @Final @LossLess mapper Int String;

static MAPPER Int_String(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	char buf[256];
	knh_snprintf(buf, sizeof(buf), KNH_INT_FMT, sfp[0].ivalue);
	KNH_MAPPED(ctx, sfp, new_String(ctx, B(buf), NULL));
}

/* ------------------------------------------------------------------------ */
//## @Const @Final @LossLess mapper Float String;

static MAPPER Float_String(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	char buf[256];
	knh_snprintf(buf, sizeof(buf), KNH_FLOAT_FMT, sfp[0].fvalue);
	KNH_MAPPED(ctx, sfp, new_String(ctx, B(buf), NULL));
}

/* ------------------------------------------------------------------------ */
//## @Const @Final mapper Bytes String;

static MAPPER Bytes_String(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	knh_bytes_t t = knh_Bytes_tobytes((knh_Bytes_t*)sfp[0].o);
	knh_String_t *s = TS_EMPTY;
	if(knh_bytes_checkENCODING(t)) {
		s = new_String(ctx, t, NULL);
	}
	KNH_MAPPED(ctx, sfp, s);
}

/* ======================================================================== */
/* [toBytes] */

/* ------------------------------------------------------------------------ */
//## @Const @Final mapper String Bytes;

static MAPPER String_Bytes(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	knh_Bytes_t *b = new_Bytes(ctx, knh_String_strlen(sfp[0].s) + 1);
	knh_Bytes_write(ctx, b, __tobytes(sfp[0].s));
	KNH_MAPPED(ctx, sfp, b);
}

/* ======================================================================== */
/* [Iterator] */

/* ------------------------------------------------------------------------ */
//## @Const mapper Object Iterator!;

static MAPPER Object_Iterator(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	Object *o = sfp[0].o;
	KNH_MAPPED(ctx, sfp, new_Iterator(ctx, o->h.cid, o, NULL));
}

/* ------------------------------------------------------------------------ */
//## method This.. Object.opItr();

static METHOD Object_opItr(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	KNH_RETURN(ctx, sfp, new_Iterator(ctx, knh_Object_cid(sfp[0].o), sfp[0].o, NULL));
}

/* ======================================================================== */
/* [Iterator] */

/* ------------------------------------------------------------------------ */
//## mapper Iterator Array!;

static MAPPER Iterator_Array(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	knh_Iterator_t *it = sfp[0].it;
	knh_Array_t *a = new_Array(ctx, ctx->share->ClassTable[it->h.cid].p1, 0);
	while(it->fnext_1(ctx, sfp, 1)) {
		knh_stack_boxing(ctx, sfp + 1);
		knh_Array_add(ctx, a, sfp[1].o);
	}
	KNH_MAPPED(ctx, sfp, a);
}

/* ------------------------------------------------------------------------ */

#endif/* KNH_CC_METHODAPI*/

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
