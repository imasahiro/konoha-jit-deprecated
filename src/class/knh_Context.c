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
/* [Context] */

Object* knh_getProperty(Ctx *ctx, knh_bytes_t key)
{
	Object *v = knh_DictMap_get__b(ctx,  (ctx)->props, key);
	if(IS_NULL(v)) {
		v = knh_System_getProperty(ctx, ctx->sys, key);
	}
	return v;
}

/* ------------------------------------------------------------------------ */

knh_type_t knh_getPropertyType(Ctx *ctx, knh_bytes_t key)
{
	knh_type_t type = TYPE_Any;
	Object *v = knh_DictMap_get__b(ctx,  ctx->props, key);
	if(IS_NULL(v)) {
		v = knh_System_getProperty(ctx, ctx->sys, key);
		KNH_SETv(ctx, ctx->esp[0].o, v);
	}
	if(IS_NOTNULL(v)) {
		type = NNTYPE_cid(knh_Object_cid(v));
	}
	return type;
}

/* ------------------------------------------------------------------------ */

void knh_setProperty(Ctx *ctx, knh_String_t *key, Any *value)
{
	knh_type_t type = knh_getPropertyType(ctx, __tobytes(key));
	if(type == TYPE_Any || knh_Object_cid(value) == CLASS_type(type)) {
		knh_DictMap_set(ctx, ctx->props, key, value);
	}
	else {
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		knh_printf(ctx, cwb->w, "PropertyType!!: $%s must be %T", knh_String_text(ctx, key), type);
		knh_String_t *s = knh_cwb_newString(ctx, cwb);
		KNH_THROW(ctx, s);
	}
}

/* ------------------------------------------------------------------------ */

void knh_Context_setEncoding(Ctx *ctx, knh_Context_t *o, knh_String_t *enc)
{
	if(IS_NULL(enc)) {
		enc = KNH_ENC;
	}
	KNH_SETv(ctx, o->enc, enc);
	if(knh_bytes_strcasecmp(__tobytes(enc), STEXT(KONOHA_ENCODING))==0) {
		KNH_SETv(ctx, DP(o->in)->bconv, KNH_NULL);
		KNH_SETv(ctx, DP(o->out)->bconv, KNH_NULL);
		KNH_SETv(ctx, DP(o->err)->bconv, KNH_NULL);
	}
	else {
		knh_BytesConv_t *bin = new_BytesConv__in(ctx, __tochar(enc));
		knh_BytesConv_t *bout = new_BytesConv__out(ctx, __tochar(enc));
		if(IS_NULL(bin) || IS_NULL(bout)) {
			KNH_WARNING(ctx, "unsupported character encoding: %s", __tochar(enc));
		}
		KNH_SETv(ctx, DP(o->in)->bconv, bin);
		KNH_SETv(ctx, DP(o->out)->bconv, bout);
		KNH_SETv(ctx, DP(o->err)->bconv, bout);
	}
}

/* ------------------------------------------------------------------------ */

void knh_Context_clearstack(Ctx *ctx)
{
	size_t i;
	for(i = ctx->esp - ctx->stack + 1; i < ctx->stacksize - 1; i++) {
		KNH_SETv(ctx, ctx->stack[i].o, KNH_NULL);
	}
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
