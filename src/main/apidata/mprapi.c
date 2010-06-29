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

/* ======================================================================== */
/* [toInt] */

//## @Const @Final mapper Float Int;

static TCAST Float_Int(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	DBG_ASSERT(IS_bFloat(sfp[K_TRLIDX].o));
	knh_int_t v = (knh_int_t)(sfp[K_TRLIDX].f)->n.fvalue;
	RETURNi_(v);
}

/* ------------------------------------------------------------------------ */
//## @Const @Final mapper String Int;

static TCAST String_Int(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_int_t v = 0;
	if(!knh_bytes_parseint(S_tobytes(sfp[K_TRLIDX].s), &v)) {
		KNH_SETv(ctx, sfp[rix].o, KNH_NULVAL(CLASS_Int));
	}
	RETURNi_(v);
}

/* ======================================================================== */
/* [toFloat] */

//## @Const @Final mapper Int Float;

static TCAST Int_Float(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	DBG_ASSERT(IS_bInt(sfp[K_TRLIDX].o));
	knh_float_t v = (knh_float_t)(sfp[K_TRLIDX].i)->n.ivalue;
	RETURNf_(v);
}

/* ======================================================================== */
/* [toString] */

///* ------------------------------------------------------------------------ */
////## @Const @Final @LossLess mapper Boolean String;
//
//static TCAST Boolean_String(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	knh_String_t *s = (p_bool(sfp[K_TRLIDX])) ? TS_true : TS_false;
//	RETURN_(s);
//}
//
///* ------------------------------------------------------------------------ */
////## @Const @Final @LossLess mapper Int String;
//
//static TCAST Int_String(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	char buf[256];
//	knh_snprintf(buf, sizeof(buf), K_INT_FMT, sfp[K_TRLIDX].ivalue);
//	RETURN_(new_S(ctx, B(buf)));
//}
//
///* ------------------------------------------------------------------------ */
////## @Const @Final @LossLess mapper Float String;
//
//static TCAST Float_String(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	char buf[256];
//	knh_snprintf(buf, sizeof(buf), K_FLOAT_FMT, sfp[K_TRLIDX].fvalue);
//	RETURN_(new_S(ctx, B(buf)));
//}
//
///* ------------------------------------------------------------------------ */
////## @Const @Final mapper Bytes String;
//
//static TCAST Bytes_String(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	knh_bytes_t t = knh_Bytes_tobytes((knh_Bytes_t*)sfp[K_TRLIDX].o);
//	knh_String_t *s = TS_EMPTY;
//	if(knh_bytes_checkENCODING(t)) {
//		s = new_S(ctx, t);
//	}
//	RETURN_(s);
//}

/* ======================================================================== */
/* [toBytes] */

///* ------------------------------------------------------------------------ */
////## @Const @Final mapper String Bytes;
//
//static TCAST String_Bytes(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	knh_Bytes_t *b = new_Bytes(ctx, S_size(sfp[K_TRLIDX].s) + 1);
//	knh_Bytes_write(ctx, b, S_tobytes(sfp[K_TRLIDX].s));
//	RETURN_(b);
//}

/* ======================================================================== */
/* [Iterator] */

/* ------------------------------------------------------------------------ */
//## @Const mapper Object Iterator!;
//## method This.. Object.opITR();

static TCAST Object_Iterator(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	Object *o = sfp[K_TRLIDX].o;
	RETURN_(new_Iterator(ctx, o->h.cid, o, NULL));
}

/* ======================================================================== */
/* [Iterator] */

/* ------------------------------------------------------------------------ */
//## mapper Iterator Array!;

static TCAST Iterator_Array(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Iterator_t *it = sfp[K_TRLIDX].it;
	knh_Array_t *a = new_Array(ctx, knh_class_p1(it->h.cid), 0);
	while(it->fnext_1(ctx, sfp, 1)) {
		knh_stack_boxing(ctx, sfp + 1);
		knh_Array_add_(ctx, a, sfp[1].o);
	}
	RETURN_(a);
}

/* ======================================================================== */
/* [Stream] */

/* ------------------------------------------------------------------------ */
//## mapper Bytes InputStream;

static TCAST Bytes_InputStream(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Bytes_t *ba = (knh_Bytes_t*)sfp[K_TRLIDX].o;
	RETURN_(new_BytesInputStream(ctx, ba, 0, BA_size(ba)));
}

/* ------------------------------------------------------------------------ */

#endif/* K_USING_DEFAULTAPI*/

#ifdef __cplusplus
}
#endif
