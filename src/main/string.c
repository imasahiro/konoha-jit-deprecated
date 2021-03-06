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

#define USE_STEXT

#include"commons.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ======================================================================== */

/* ------------------------------------------------------------------------ */
/* These utf8 functions were originally written by Shinpei Nakata */

static knh_bool_t knh_utf8_isLead(knh_uchar_t c)
{
	if ((c & 0xC0) != 0x80) return 1;
	return 0;
}

static knh_ushort_t knh_utf8_getBytes(knh_uchar_t c)
{
	DBG_ASSERT(knh_utf8_isLead(c));
	if ((c & 0xF0) == 0xE0) {
		return 3;
	} else {
		switch(c & 0xF0){
		case 0xC0:
			return 2;
		case 0xF0:
			return 4;
		default:
			switch (c & 0x0F) {
			case 0x8:
				return 5;
			case 0xC:
				return 6;
			default:
				return -1;
			}

		}
	}
}

static knh_bool_t knh_utf8_isTrail(knh_uchar_t c)
{
	if ((0x80 <= c) && (c <= 0xBF)) return 1;
	return 0;
}

static knh_bool_t knh_utf8_isSingleton(knh_uchar_t c)
{
	if (c <= 0x7f) return 1;
	// if ((c & 0x80) == 0) return 1;	// which is faster??
	return 0;
}

/* ------------------------------------------------------------------------ */

knh_bool_t knh_bytes_checkENCODING(knh_bytes_t s)
{
#ifdef K_USING_UTF8
	size_t i, j;
	knh_uchar_t ch;
	size_t bytes = 0;
	for (i=0; i < s.len; i++) {
		ch = s.ustr[i];
		/* UTF8 must be in
		 * single: 0x00 - 0x7f
		 * lead: 0xC0 - 0xFD (actually, 0xC2-0xF4)
		 * trail: 0x80 - 0xBF
		 * consequently, invalid utf8 is ranged
		 * 0xC0 - 0xC1, 0xF5 - 0xFF
		 *  */
		if (knh_utf8_isSingleton(ch)) {
		} else if (knh_utf8_isLead(ch)) {
			bytes = knh_utf8_getBytes(ch);
			for (j=1;j<bytes;j++)
			{
				ch = s.ustr[i+j];
				if (!knh_utf8_isTrail(ch)) {
					DBG_P("invalid UTF!");
					return 0;
				}
			}
			i += bytes;
		}
	}
	return 1;
#else
	return 1;
#endif
}

/* ------------------------------------------------------------------------ */

size_t knh_bytes_mlen(knh_bytes_t s)
{
#ifdef K_USING_UTF8
	size_t i, len = 0, ulen = 0;
	for (i = 0; i < s.len; i++) {
		if (knh_utf8_isLead(s.ustr[i]))
			ulen++;
		else if (knh_utf8_isSingleton(s.ustr[i]))
			len++;
	}
	return ulen + len;
#else
	return s.len;
#endif
}

/* ------------------------------------------------------------------------ */

knh_bytes_t knh_bytes_mofflen(knh_bytes_t m, size_t moff, size_t mlen)
{
#ifdef K_USING_UTF8
	size_t i, s = m.len, len = 0, ulen = 0;
	size_t bytes = 0;
	knh_uchar_t ch;

	// boundary check
	if(knh_bytes_mlen(m) < mlen){
		mlen = knh_bytes_mlen(m) - moff;
	}

	for (i = 0; i < m.len; i++) {
		if (knh_utf8_isLead(m.ustr[i])) ulen++;
		else if (knh_utf8_isSingleton(m.ustr[i])) len++;
		if (ulen + len == moff+1) {
			s = i;
			break;
		}
	}
	if (!(s < m.len)) {
		m.text = "";
		m.len = 0;
		return m;
	}

	m.ustr = m.ustr + s;
	for(i = 0; i < mlen; i++) {
		ch = m.ustr[bytes];
		if (knh_utf8_isSingleton(ch)) {
			bytes += 1;
		} else {
			bytes += knh_utf8_getBytes(ch);
		}
	}
	m.len = bytes;
	return m;
#else
	return knh_bytes_subbytes(m, moff, mlen); /* if K_ENCODING is not set */
#endif
}

/* ------------------------------------------------------------------------ */

knh_int_t knh_uchar_toucs4(knh_ustr_t *utf8)   /* utf8 -> ucs4 */
{
#if defined(K_USING_UTF8)
	/* BOM bigendinan */
	knh_int_t ucs4 = 0;
	int i= 0;
	knh_uchar_t ret = 0;
	if (!knh_utf8_isSingleton(utf8[0])) {
		knh_ushort_t length_utf8 = knh_utf8_getBytes(utf8[i]);
		knh_uchar_t mask = (knh_uchar_t)(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);

		switch(length_utf8){
		case 2:
			/* 110xxxxx 10xxxxxx */
			TODO();
			break;
		case 3:
			/* format 1110xxxx 10xxxxxx 10xxxxxx */
			// first 4 bits
			ucs4 = 0;
			ret = utf8[0] & mask;
			ucs4 = ucs4 | ret;
			// second bit
			ucs4 = ucs4 << 6;
			mask = mask | 1 << 4 | 1 << 5;
			ret = utf8[1] & mask;
			ucs4 = ucs4  | ret;
			// third bit
			ucs4 = ucs4 << 6;
			ret = mask & utf8[2];
			ucs4 = ucs4 | ret;
			break;
		default:
			/* TODO: */
			break;
		}
	} else {
		/* ASCII, let it goes...*/
		ucs4 = utf8[0];
	}
	return ucs4;
#else
	return (knh_int_t)utf8[0];
#endif
}

/* ------------------------------------------------------------------------ */
/* ucs4 -> utf8 */

char *knh_format_utf8(char *buf, size_t bufsiz, knh_uint_t ucs4)
{
	/* TODO: here, we assume that BOM bigEndian
	 and only 3 bytes or 1 byte UTF
	 */
	knh_uint_t mask = 0x0;
	knh_uint_t byte1 = 0x7F;
	knh_uint_t byte2 = 0x7FF;
	knh_uint_t byte3 = 0xFFFF;

	char *ret = buf;
	unsigned char utf8[8];
	if (ucs4 <= byte1) {
		/* 7 bits */
		knh_snprintf(buf, bufsiz, "%c", (int)(0xffff & ucs4));
		ret = buf;
	} else if (ucs4 <= byte2) {
		/* cut last 6 bits */
		TODO();
		/* first 5 bits */
	} else if (ucs4 <= byte3) {
		/* cut last 6 bits */
		mask = 1 << 0 | 1 << 1 | 1 << 2 | 1 << 3| 1 << 4 | 1 << 5;
		utf8[2] = (unsigned char)(ucs4 & mask);
		utf8[2] = utf8[2] | 1 << 7;
		/* cut next 6 bits */
		ucs4 = ucs4 >> 6;
		utf8[1] = (unsigned char)(ucs4 & mask);
		utf8[1] = utf8[1] | 1 << 7;
		/* first 4 bits */
		mask = 1 << 0 | 1 << 1 | 1 << 2 | 1 << 3;
		ucs4 = ucs4 >> 6;
		utf8[0] = (unsigned char)(ucs4 & mask);
		utf8[0] = utf8[0] | 1 << 7 | 1 << 6 | 1 << 5;
		utf8[3] = '\0';
		knh_snprintf(buf, bufsiz, "%s", utf8);
	} else {
		TODO();
	}
	return ret;
}

/* ======================================================================== */
/* [String] */

static void knh_String_checkASCII(knh_String_t *o)
{
	knh_ustr_t *p = o->str.ustr;
	size_t i;
	for(i = 0; i < o->str.len; i++) {
		if(p[i] > 127) return;
	}
	knh_String_setASCII(o, 1);
}

/* ------------------------------------------------------------------------ */

KNHAPI(knh_String_t*) new_String_(Ctx *ctx, knh_class_t cid, knh_bytes_t t, knh_String_t *memoNULL)
{
	knh_String_t *o = (knh_String_t*)new_hObject_(ctx, FLAG_String, CLASS_String, cid);
	if(t.len + 1 < sizeof(void*) * 2) {
		o->str.ubuf = (knh_uchar_t*)(&(o->memoNULL));
		o->str.len = t.len;
		knh_memcpy(o->str.ubuf, t.ustr, t.len);
		o->str.ubuf[o->str.len] = '\0';
		knh_String_checkASCII(o);
		knh_String_setTextSgm(o, 1);
		//DBG_P("INLINE STRING ** t.len=%d, '%s'", t.len, o->str.buf);
	}
	else if(memoNULL == NULL) {
		o->str.len = t.len;
		o->str.ubuf = (knh_uchar_t*)KNH_MALLOC(ctx, KNH_SIZE(o->str.len+1));
		knh_memcpy(o->str.ubuf, t.ustr, t.len);
		o->str.ubuf[o->str.len] = '\0';
		o->memoNULL = NULL;
		knh_String_checkASCII(o);
	}else {
		DBG_ASSERT(IS_bString(memoNULL));
		o->str.len = t.len;
		o->str.ustr = t.ustr;
		if(memoNULL->memoNULL == NULL) {
			KNH_INITv(o->memoNULL, memoNULL);
		}
		else {
			DBG_ASSERT(IS_bString(memoNULL->memoNULL));
			KNH_INITv(o->memoNULL, memoNULL->memoNULL);
		}
		if(knh_String_isASCII(memoNULL)) {
			knh_String_setASCII(o, 1);
		}
		else {
			knh_String_checkASCII(o);
		}
	}
	return o;
}

/* ------------------------------------------------------------------------ */

knh_String_t *new_TEXT(Ctx *ctx, knh_class_t cid, knh_TEXT_t text)
{
	knh_String_t *o = (knh_String_t*)new_hObject_(ctx, FLAG_String, CLASS_String, cid);
	o->str.text = text;
	o->str.len = knh_strlen(text);
	o->memoNULL = NULL;
	knh_String_setTextSgm(o, 1);
	knh_String_checkASCII(o);
	return o;
}

/* ------------------------------------------------------------------------ */

int knh_bytes_strcasecmp(knh_bytes_t v1, knh_bytes_t v2)
{
	if(v1.len < v2.len) {
		int res = knh_strncasecmp(v1.text, v2.text, v1.len);
		return (res == 0) ? -1 : res;
	}
	else if(v1.len > v2.len) {
		int res = knh_strncasecmp(v1.text, v2.text, v2.len);
		return (res == 0) ? 1 : res;
	}
	else {
		return knh_strncasecmp(v1.text, v2.text, v1.len);
	}
}

/* ------------------------------------------------------------------------ */

knh_StringDecoder_t* new_StringDecoderNULL(Ctx *ctx, knh_bytes_t t, knh_NameSpace_t *ns)
{
	if(knh_bytes_strcasecmp(t, STEXT(K_ENCODING))) {
		return KNH_TNULL(StringDecoder);
	}
	if(ctx->share->iconvDSPI == NULL) {
		((knh_share_t*)ctx->share)->iconvDSPI = knh_NameSpace_getConvTODSPINULL(ctx, ns, STEXT("iconv"));
	}
	if(ctx->share->iconvDSPI != NULL) {
		const knh_ConvDSPI_t *dspi = ctx->share->iconvDSPI;
		knh_conv_t *conv = dspi->open(ctx, K_ENCODING, t.text);
		if(conv != NULL) {
			knh_StringDecoder_t *c = new_(StringDecoder);
			c->conv = conv;
			c->dspi = dspi;
			return c;
		}
	}
	return NULL;
}

/* ------------------------------------------------------------------------ */

knh_StringEncoder_t* new_StringEncoderNULL(Ctx *ctx, knh_bytes_t t, knh_NameSpace_t *ns)
{
	if(knh_bytes_strcasecmp(t, STEXT(K_ENCODING))) {
		return KNH_TNULL(StringEncoder);
	}
	if(ctx->share->iconvDSPI == NULL) {
		((knh_share_t*)ctx->share)->iconvDSPI = knh_NameSpace_getConvTODSPINULL(ctx, ns, STEXT("iconv"));
	}
	if(ctx->share->iconvDSPI != NULL) {
		const knh_ConvDSPI_t *dspi = ctx->share->iconvDSPI;
		knh_conv_t *conv = dspi->open(ctx, t.text, K_ENCODING);
		if(conv != NULL) {
			knh_StringEncoder_t *c = new_(StringEncoder);
			c->conv = conv;
			c->dspi = dspi;
			return c;
		}
	}
	return NULL;
}

/* ------------------------------------------------------------------------ */

knh_String_t *knh_cwb_newStringDECODE(Ctx *ctx, knh_cwb_t *cwb, knh_StringDecoder_t *c)
{
	BEGIN_LOCAL(ctx, lsfp, 1);
	LOCAL_NEW(ctx, lsfp, 0, knh_String_t*, s, knh_cwb_newString(ctx, cwb));
	if(!knh_String_isASCII(s)) {
		c->dspi->dec(ctx, c->conv, S_tobytes(s), cwb->ba);
		s = knh_cwb_newString(ctx, cwb);
		KNH_SETv(ctx, lsfp[0].o, KNH_NULL); //
	}
	END_LOCAL(ctx, lsfp);
	return s;
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
