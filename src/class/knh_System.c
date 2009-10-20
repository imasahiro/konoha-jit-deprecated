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
/* [properties] */

static
void knh_cwb_nzenvkey(Ctx *ctx, knh_cwb_t *cwb, knh_bytes_t t)
{
	size_t i;
	for(i = 0; i < t.len; i++) {
		knh_Bytes_putc(ctx, (cwb->ba), toupper(t.buf[i]));
	}
}

/* ------------------------------------------------------------------------ */

Any* knh_System_getProperty(Ctx *ctx, knh_System_t *o, knh_bytes_t key)
{
	if(knh_bytes_startsWith(key, STEXT("env."))) {
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		knh_cwb_nzenvkey(ctx, cwb, knh_bytes_last(key, 4));
		char *v = knh_getenv(knh_cwb_tochar(ctx, cwb));
		knh_cwb_close(cwb);
		if(v == NULL) {
			return KNH_NULL;
		}
		return UP(new_String(ctx, B(v), NULL));
	}
	return knh_DictMap_get__b(ctx,  DP(o)->props, key);
}

/* ------------------------------------------------------------------------ */

void knh_System_setProperty(Ctx *ctx, knh_System_t *o, knh_String_t *key, Any *value)
{
	knh_type_t type = knh_getPropertyType(ctx, __tobytes(key));
	if(type == TYPE_Any || knh_Object_cid(value) == CLASS_type(type)) {
		knh_DictMap_set(ctx, DP(o)->props, key, value);
	}
	else {
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		knh_printf(ctx, cwb->w, "PropertyType!!: $%s must be %T", knh_String_text(ctx, key), type);
		knh_String_t *s = knh_cwb_newString(ctx, cwb);
		KNH_THROW(ctx, s);
	}
}

/* ------------------------------------------------------------------------ */

KNHAPI(void) knh_setSystemPropertyText(Ctx *ctx, char *key, char *value)
{
	knh_String_t *k = T__(key);
	knh_String_t *v = T__(value);
	knh_DictMap_set(ctx, DP(ctx->sys)->props, k, UP(v));
}

/* ------------------------------------------------------------------------ */

knh_bool_t konoha_initcheck(void)
{
#ifdef KONOHA_SAFEMODE
	fprintf(stderr, "\taddress sizeof(void*) = %d bytes\n", (int)sizeof(void *));
	fprintf(stderr, "\tint sizeof(knh_int_t) = %d bytes\n", (int)sizeof(knh_intptr_t));
	fprintf(stderr, "\tfloat sizeof(knh_float_t) = %d bytes\n", (int)sizeof(knh_float_t));
	fprintf(stderr, "\tlong sizeof(knh_long_t) = %d bytes\n", (int)sizeof(knh_int64_t));
	fprintf(stderr, "\tshort sizeof(knh_short_t) = %d bytes\n", (int)sizeof(knh_short_t));
	fprintf(stderr, "\tObject(header) sizeof(hObject) = %d bytes\n", (int)sizeof(knh_hObject_t));
#endif
	KNH_ASSERT(sizeof(knh_intptr_t) == sizeof(void *));
 	KNH_ASSERT(sizeof(knh_short_t) * 2 == sizeof(knh_intptr_t));
 	return 1;
 }

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
