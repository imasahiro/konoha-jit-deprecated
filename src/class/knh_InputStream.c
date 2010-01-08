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
/* [Bytes] */

KNHAPI(knh_InputStream_t*) new_InputStream__io(Ctx *ctx, knh_String_t *urn, knh_io_t fd, knh_StreamDSPI_t *drv)
{
	knh_InputStream_t* o = (knh_InputStream_t*)new_Object_bcid(ctx, CLASS_InputStream, 0);
	KNH_SETv(ctx, DP(o)->urn, urn);
	DP(o)->fd = fd;
	if(fd != -1) {
		DP(o)->driver = drv;
		DP(o)->bufsiz = drv->bufsiz;
		if(DP(o)->bufsiz > 0) {
			KNH_SETv(ctx, DP(o)->ba, new_Bytes(ctx, DP(o)->bufsiz));
			DP(o)->buf = (char*)(DP(o)->ba)->buf;
		}
		else {
			knh_InputStream_setFILE(o, 1);
		}
		DP(o)->bufpos = 0;
		DP(o)->bufend = 0;  /* empty */
		DP(o)->driver->finit(ctx, (Object*)o, "r");
	}
	return o;
}

/* ------------------------------------------------------------------------ */

KNHAPI(knh_InputStream_t*) new_InputStream__FILE(Ctx *ctx, knh_String_t *urn, FILE *fp, knh_StreamDSPI_t *drv)
{
	knh_InputStream_t* o = (knh_InputStream_t*)new_Object_bcid(ctx, CLASS_InputStream, 0);
	KNH_SETv(ctx, DP(o)->urn, urn);
	if(fp != NULL) {
		DP(o)->fp = fp;
		DP(o)->driver = drv;
		knh_InputStream_setFILE(o, 1);
	}
	return o;
}

/* ======================================================================== */
/* [methods] */

Object *knh_InputStream_open(Ctx *ctx, knh_InputStream_t *o, knh_String_t *urn, knh_String_t *m)
{
	knh_bytes_t fname = __tobytes(urn);
	knh_index_t loc = knh_bytes_index(fname, ':');
	if(loc == -1 || (loc == 1 && isalpha(fname.buf[0]))) {  /* 'C:/' */
		DP(o)->driver = knh_getIODriver(ctx, STEXT("file"));
	}
	else {
		DP(o)->driver = knh_getIODriver(ctx, knh_bytes_first(fname, loc));
	}
	KNH_SETv(ctx, DP(o)->urn, new_String(ctx, fname, NULL));
	char *mode = "r";
	if(IS_NOTNULL(m)) mode = __tochar(m);
	DP(o)->fd = DP(o)->driver->fopen(ctx, fname, mode, knh_Context_isStrict(ctx));
	if(DP(o)->fd != -1) {
		KNH_SETv(ctx, DP(o)->urn, urn);
		DP(o)->bufsiz = DP(o)->driver->bufsiz;
		if(DP(o)->bufsiz > 0) {
			KNH_SETv(ctx, DP(o)->ba, new_Bytes(ctx, DP(o)->bufsiz));
			DP(o)->buf = (char*)(DP(o)->ba)->buf;
		}
		else {
			knh_InputStream_setFILE(o, 1);
		}
		DP(o)->bufpos = 0;
		DP(o)->bufend = 0;  /* empty */
		DP(o)->driver->finit(ctx, (Object*)o, mode);
	}
	else {
		DP(o)->driver = knh_getDefaultIODriver();
	}
	return (Object*)o;
}

/* ------------------------------------------------------------------------ */

int knh_InputStream_getc(Ctx *ctx, knh_InputStream_t *o)
{
	int ch;
	if(knh_InputStream_isFILE(o)) {
		ch = knh_fgetc(ctx, DP(o)->fp);
	}
	else {
		while(1) {
			if(DP(o)->bufpos < DP(o)->bufend) {
				ch = (knh_uchar_t)DP(o)->buf[DP(o)->bufpos];
				DP(o)->bufpos++;
				break;
			}
			DP(o)->bufpos = 0;
			DP(o)->bufend = DP(o)->driver->fread(ctx, DP(o)->fd, DP(o)->buf, DP(o)->bufsiz);
			if(DP(o)->bufend == 0) return EOF;
		}
	}
	{ /* statstics */
		DP(o)->size++;
		if(ch == '\n') {
			if(DP(o)->prev != '\r') {
				DP(o)->line++;
			}
		}else if(ch == '\r') {
			DP(o)->line++;
		}
	}
	return ch;
}

/* ------------------------------------------------------------------------ */

size_t
knh_InputStream_read(Ctx *ctx, knh_InputStream_t *o, char *buf, size_t bufsiz)
{
	if(knh_InputStream_isFILE(o)) {
		return knh_fread(ctx, buf, bufsiz, DP(o)->fp);
	}
	else {
		size_t inbufsiz = (DP(o)->bufend - DP(o)->bufpos);
		if(bufsiz <= inbufsiz) {
			knh_memcpy(buf, DP(o)->buf, bufsiz);
			DP(o)->bufpos += bufsiz;
			{
				DP(o)->size += bufsiz;
			}
			return bufsiz;
		}
		// XXX when both InputStream.read and InputStream.readLine method call,
		// it seams strange. so, move DP(o)->buf's pointer to bufpos.
		knh_memcpy(buf, DP(o)->buf + DP(o)->bufpos, inbufsiz);
		DP(o)->bufpos += inbufsiz;
		DP(o)->size += bufsiz;
		buf += inbufsiz;
		size_t s = DP(o)->driver->fread(ctx, DP(o)->fd, buf+inbufsiz, bufsiz-inbufsiz);
		DP(o)->size += s;
		return s + inbufsiz;
	}
}

/* ------------------------------------------------------------------------ */

knh_String_t* knh_InputStream_readLine(Ctx *ctx, knh_InputStream_t *in)
{
	int ch;
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
		if(ch == '\r') {
			DP(in)->prev = ch;
			return new_String__cwbconv(ctx, cwb, DP(in)->bconv);
		}
		else if(ch == '\n') {
			if(DP(in)->prev == '\r') continue;
			DP(in)->prev = ch;
			return new_String__cwbconv(ctx, cwb, DP(in)->bconv);
		}
		else {
			knh_Bytes_putc(ctx, cwb->ba, ch);
		}
	}
	if(knh_cwb_size(cwb) != 0) {
		return new_String__cwbconv(ctx, cwb, DP(in)->bconv);
	}
	return (knh_String_t*)KNH_NULL;
}

/* ------------------------------------------------------------------------ */

void knh_InputStream_close(Ctx *ctx, knh_InputStream_t *o)
{
	knh_Fclose f = DP(o)->driver->fclose;
	DP(o)->driver = knh_getDefaultIODriver();
	f(ctx, DP(o)->fd);
	DP(o)->fd = -1;
	KNH_SETv(ctx, DP(o)->ba, KNH_NULL);
}

/* ------------------------------------------------------------------------ */

int knh_InputStream_isClosed(Ctx *ctx, knh_InputStream_t *o)
{
	return (DP(o)->fd == -1 && IS_NULL(DP(o)->ba));
}

/* ------------------------------------------------------------------------ */

void knh_InputStream_setEncoding(Ctx *ctx, knh_InputStream_t *o, knh_String_t *enc)
{
	if(IS_NULL(enc)) {
		KNH_SETv(ctx, DP(o)->enc, TS_ENCODING);
		KNH_SETv(ctx, DP(o)->bconv, KNH_NULL);
	}
	else if(knh_bytes_strcasecmp(__tobytes(enc), STEXT(KONOHA_ENCODING)) == 0) {
		KNH_SETv(ctx, DP(o)->enc, TS_ENCODING);
		KNH_SETv(ctx, DP(o)->bconv, KNH_NULL);
	}
	else {
		knh_BytesConv_t *bin = new_BytesConv__in(ctx, __tochar(enc));
		if(IS_NULL(bin)) {
			KNH_SETv(ctx, DP(o)->enc, TS_ENCODING);
			KNH_WARNING(ctx, "unsupported character encoding: %s", __tochar(enc));
		}
		else {
			KNH_SETv(ctx, DP(o)->enc, enc);
		}
		KNH_SETv(ctx, DP(o)->bconv, bin);
	}
}

/* ======================================================================== */
/* [File] */

KNHAPI(knh_InputStream_t*) new_FileInputStream(Ctx *ctx, knh_bytes_t file, int isThrowable)
{
	knh_StreamDSPI_t *drv = knh_getIODriver(ctx, STEXT("file"));
	knh_io_t fd = drv->fopen(ctx, file, "r", isThrowable);
	return new_InputStream__io(ctx, new_String(ctx, file, NULL), fd, drv);
}

/* ======================================================================== */
/* [Bytes] */

KNHAPI(knh_InputStream_t*) new_BytesInputStream(Ctx *ctx, knh_Bytes_t *ba, size_t s, size_t e)
{
	knh_InputStream_t* o = (knh_InputStream_t*)new_Object_bcid(ctx, CLASS_InputStream, 0);
	DP(o)->fd = -1;
	KNH_ASSERT(IS_Bytes(ba));
	KNH_SETv(ctx, DP(o)->ba, ba);
	DP(o)->buf = (char*)(ba)->buf;
	DP(o)->bufsiz = (ba)->size;
	KNH_ASSERT(e <= knh_Bytes_size(ba));
	KNH_ASSERT(s <= e);
	DP(o)->bufpos   = s;
	DP(o)->bufend   = e;
	return o;
}

/* ------------------------------------------------------------------------ */
/* @map Bytes InputStream! */

MAPPER knh_Bytes_InputStream(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	knh_Bytes_t *ba = (knh_Bytes_t*)sfp[0].o;
	KNH_MAPPED(ctx, sfp, new_BytesInputStream(ctx, ba, 0, knh_Bytes_size(ba)));
}

/* ======================================================================== */
/* [String] */

KNHAPI(knh_InputStream_t*) new_StringInputStream(Ctx *ctx, knh_String_t *str, size_t s, size_t e)
{
	knh_InputStream_t* o = (knh_InputStream_t*)new_Object_bcid(ctx, CLASS_InputStream, 0);
	DP(o)->fd = -1;
	DBG2_ASSERT(IS_String(str));
	KNH_SETv(ctx, DP(o)->str, str);
	DP(o)->buf = (char*)(str)->str;
	DP(o)->bufsiz = (str)->size;
	DBG2_ASSERT(e <= (str)->size);
	DBG2_ASSERT(s <= e);
	DP(o)->bufpos   = s;
	DP(o)->bufend   = e;
	return o;
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
