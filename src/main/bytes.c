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

#define USE_cwb_tobytes 1

#include"commons.h"

#ifdef KONOHA_ON_LKM
#include<linux/string.h>
#else
#include<string.h>
#endif

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ======================================================================== */
/* [memory] */

size_t knh_good_size(size_t ss)
{
	size_t s = ss;
	if(s > 64 * 1024) return s;
	if(s <= K_FASTMALLOC_SIZE) return K_FASTMALLOC_SIZE;
	s |= s >> 1;
	s |= s >> 2;
	s |= s >> 4;
	s |= s >> 8;
	s |= s >> 16;
	return ((s + 1) == ss * 2) ? ss : s + 1;
}

///* added by ide */
///* find last bit set in a long int */
//static int knh_fls(int i)
//{
//	return (sizeof(int) * 8) - __builtin_clz(i);
//}
//
//static const size_t log2table[] = {
//	/*   1  */ K_FASTMALLOC_SIZE, /*   2  */ K_FASTMALLOC_SIZE,
//	/*   4  */ K_FASTMALLOC_SIZE, /*   8  */ K_FASTMALLOC_SIZE,
//	/*  16  */ K_FASTMALLOC_SIZE, /*  32  */ K_FASTMALLOC_SIZE,
//#if defined(__x86_64__)
//	/*  64  */ K_FASTMALLOC_SIZE,
//#else
//	/*  64  */ 1 << 6,
//#endif
//	/* 128  */ 1 <<  7,
//	/* 256  */ 1 <<  8, /* 512  */ 1 <<  9,
//	/*   1K */ 1 << 10, /*   2K */ 1 << 11,
//	/*   4K */ 1 << 12, /*   8K */ 1 << 13,
//	/*  16K */ 1 << 14, /*  32K */ 1 << 15,
//	/*  64K */ 1 << 16, /* 128K */ 1 << 17,
//	/* 256K */ 1 << 18, /* 512K */ 1 << 19,
//	/*   1M */ 1 << 20, /*   2M */ 1 << 21,
//	/*   4M */ 1 << 22, /*   8M */ 1 << 23,
//	/*  16M */ 1 << 24, /*  32M */ 1 << 25,
//	/*  64M */ 1 << 26, /* 128M */ 1 << 27,
//	/* 256M */ 1 << 28, /* 512M */ 1 << 29,
//	/*   1G */ 1 << 30, /*   2G */ 1 << 31,
//};
//
///* XXX "s" must be larger than 0 (s > 0) */
//size_t knh_good_size2(size_t s)
//{
//	return log2table[knh_fls(s-1)];
//}
/* ------------------------------------------------------------------------ */

static void knh_Bytes_checkstack(Ctx *ctx, knh_uchar_t*oldstart, knh_uchar_t *oldend, knh_uchar_t* newstart)
{
	knh_uchar_t **cstack_top = (knh_uchar_t**)(&ctx);
	knh_uchar_t **p = (knh_uchar_t**)ctx->cstack_bottom;
	DBG_P("cstack_bottom=%p, cstack_top=%p basedata=%p", p[0], cstack_top, &oldstart);
	if(!(p < cstack_top)) {
		knh_uchar_t **tmp = cstack_top;
		cstack_top = p; p = tmp;
	}
	while(p <= cstack_top) {
		if((oldstart <= p[0] && p[0] < oldend)) {
			knh_uchar_t *newc = p[0] + (newstart - oldstart);
			KNH_SYSLOG(ctx, LOG_DEBUG, "RewrittenCharPointer", "*oldptr=%p, newptr=%p", p[0], newc);
			p[0] = newc;
		}
		p++;
	}
}

static void knh_Bytes_expands(Ctx *ctx, knh_Bytes_t *ba, size_t newsize)
{
	if(ba->capacity == 0) {
		newsize = knh_good_size(newsize);
		ba->bu.ubuf = (knh_uchar_t*)KNH_MALLOC(ctx, newsize);
		ba->capacity = newsize;
	}
	else {
		knh_uchar_t *buf = ba->bu.ubuf;
		size_t capacity = ba->capacity;
		knh_uchar_t *newa = (knh_uchar_t*)KNH_MALLOC(ctx, newsize);
		knh_memcpy(newa, buf, capacity);
		knh_bzero(&newa[capacity], newsize - capacity);
		ba->bu.ubuf = newa;
		ba->capacity = newsize;
		KNH_FREE(ctx, buf, capacity);
		if(unlikely(ctx->bufa == ba)) {
			KNH_SYSLOG(ctx, LOG_INFO, "ExtendedContextBuffer", "*newsize=%ld, pointer=(%p => %p)", newsize, buf, newa);
			knh_Bytes_checkstack(ctx, buf, buf + ba->bu.len, newa);
		}
	}
}

/* ------------------------------------------------------------------------ */
/* [Bytes] */

knh_Bytes_t* new_Bytes(Ctx *ctx, size_t capacity)
{
	knh_Bytes_t *ba = new_(Bytes);
	if(capacity > 0) {
		knh_Bytes_expands(ctx, ba, capacity);
	}
	return ba;
}

/* ------------------------------------------------------------------------ */

void knh_Bytes_clear(knh_Bytes_t *ba, size_t pos)
{
	DBG_ASSERT(!knh_Bytes_isStatic(ba));
	if(pos < BA_size(ba)) {
		knh_bzero(ba->bu.ubuf + pos, BA_size(ba) - pos);
		BA_size(ba) = pos;
	}
}

/* ------------------------------------------------------------------------ */

void knh_Bytes_ensureSize(Ctx *ctx, knh_Bytes_t *ba, size_t len)
{
	DBG_ASSERT(!knh_Bytes_isStatic(ba));
	if(ba->capacity < len) {
		knh_Bytes_expands(ctx, ba, len);
	}
	BA_size(ba) = len;
}

/* ------------------------------------------------------------------------ */

const char *knh_Bytes_ensureZero(Ctx *ctx, knh_Bytes_t *ba)
{
	DBG_ASSERT(!knh_Bytes_isStatic(ba));
	size_t size = BA_size(ba);
	if(size == ba->capacity) {
		knh_Bytes_expands(ctx, ba, ba->capacity * 2);
	}
	ba->bu.ubuf[BA_size(ba)] = 0;
//	if(ba->bu.text[size] != 0) {
//		DBG_P("@@@@ pos=%d, ch=%d, %c", size, ba->bu.ubuf[size], ba->bu.ubuf[size]);
//		knh_bzero(ba->bu.ubuf + size, ba->capacity - size);
//	}
	return ba->bu.text;
}

/* ------------------------------------------------------------------------ */

void knh_Bytes_putc(Ctx *ctx, knh_Bytes_t *ba, int ch)
{
	if(BA_size(ba) == ba->capacity) {
		knh_Bytes_expands(ctx, ba, ba->capacity * 2);
	}
	ba->bu.ubuf[BA_size(ba)] = ch;
	BA_size(ba) += 1;
}

/* ------------------------------------------------------------------------ */

void knh_Bytes_unputc(knh_Bytes_t *ba)
{
	if(BA_size(ba) > 0) {
		BA_size(ba) -= 1;
		ba->bu.ubuf[BA_size(ba)] = '\0';
	}
}

/* ------------------------------------------------------------------------ */

void knh_Bytes_write(Ctx *ctx, knh_Bytes_t *ba, knh_bytes_t t)
{
	if(t.len == 0) return ;
	if(BA_size(ba) + t.len >= ba->capacity) {
		size_t newsize = ba->capacity * 2;
		if(newsize < BA_size(ba) + t.len) newsize = knh_good_size(BA_size(ba) + t.len);
		knh_Bytes_expands(ctx, ba, newsize);
	}
	knh_memcpy(ba->bu.ubuf + BA_size(ba), t.ustr, t.len);
	BA_size(ba) += t.len;
}

/* ------------------------------------------------------------------------ */

knh_String_t *knh_cwb_newString(Ctx *ctx, knh_cwb_t *cwb)
{
	if(cwb->pos == (cwb->ba)->bu.len) {
		return TS_EMPTY;
	}
	else {
		knh_bytes_t t = knh_cwb_tobytes(cwb);
		knh_String_t *s = new_S(ctx, t);
		knh_cwb_close(cwb);
		return s;
	}
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
