#ifndef INLINELIBS_H_
#define INLINELIBS_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */
/* [bytes] */

#if defined(USE_new_bytes) || defined(USE_B)
static knh_bytes_t new_bytes(char *c)
{
	knh_bytes_t v;
	DBG_ASSERT(c != NULL);
	v.buf = (knh_uchar_t*)c;
	v.len = knh_strlen(c);
	return v;
}
#endif

/* ------------------------------------------------------------------------ */

#if defined(USE_new_bytes2) || defined(USE_STEXT)
static knh_bytes_t new_bytes2(char *buf, size_t len)
{
	knh_bytes_t v;
	v.buf = (knh_uchar_t*)buf;
	v.len = len;
	return v;
}
#endif


#if defined(USE_bytes_indexOf)
static knh_index_t knh_bytes_indexOf(knh_bytes_t base, knh_bytes_t delim)
{
	size_t i, j;
	if(delim.len > base.len) {
		return -1;
	}
	for (i = 0; i <= base.len - delim.len; i++) {
		j = 0;
		while ((j < delim.len) && (base.buf[i+j] == delim.buf[j])) {
			j++;
		}
		if (j == delim.len) return i;
	}
	return -1;
}
#endif

#ifdef USE_bytes_strcmp
static int knh_bytes_strcmp(knh_bytes_t v1, knh_bytes_t v2)
{
	int res1;
	if(v1.len < v2.len) {
		int res = knh_strncmp(v1.buf, v2.buf, v1.len);
		res1 = (res == 0) ? -1 : res;
	}
	else if(v1.len > v2.len) {
		int res = knh_strncmp(v1.buf, v2.buf, v2.len);
		res1 = (res == 0) ? 1 : res;
	}
	else {
		res1 = knh_strncmp(v1.buf, v2.buf, v1.len);
	}
	return res1;
}
#endif

/* ------------------------------------------------------------------------ */

#ifdef USE_bytes_strcasecmp
static int knh_bytes_strcasecmp(knh_bytes_t v1, knh_bytes_t v2)
{
	if(v1.len < v2.len) {
		int res = knh_strncasecmp(v1.buf, v2.buf, v1.len);
		return (res == 0) ? -1 : res;
	}
	else if(v1.len > v2.len) {
		int res = knh_strncasecmp(v1.buf, v2.buf, v2.len);
		return (res == 0) ? 1 : res;
	}
	else {
		return knh_strncasecmp(v1.buf, v2.buf, v1.len);
	}
}
#endif

#ifdef USE_bytes_equals
static knh_bool_t knh_bytes_equals(knh_bytes_t v1, knh_bytes_t v2)
{
	return (v1.len == v2.len && knh_strncmp(v1.buf, v2.buf, v1.len) == 0);
}
#endif

#ifdef USE_bytes_equalsIgnoreCase
static  knh_bool_t knh_bytes_equalsIgnoreCase(knh_bytes_t v1, knh_bytes_t v2)
{
	return (v1.len == v2.len && knh_strncasecmp(v1.buf, v2.buf, v1.len) == 0);
}
#endif


#ifdef USE_bytes_startsWith
static int knh_bytes_startsWith(knh_bytes_t v1, knh_bytes_t v2)
{
	if(v1.len < v2.len) return 0;
	return (knh_strncmp(v1.buf, v2.buf, v2.len) == 0);
}
#endif

#ifdef USE_bytes_endsWith
static knh_bool_t knh_bytes_endsWith(knh_bytes_t v1, knh_bytes_t v2)
{
	if(v1.len < v2.len) return 0;
	knh_uchar_t *p = v1.buf + (v1.len-v2.len);
	return (knh_strncmp(p, v2.buf, v2.len) == 0);
}
#endif

#ifdef USE_bytes_index
static knh_index_t knh_bytes_index(knh_bytes_t v, int ch)
{
	size_t i;
	for(i = 0; i < v.len; i++) {
		if(v.buf[i] == ch) return (knh_index_t)i;
	}
	return -1;
}
#endif

#ifdef USE_bytes_rindex
static knh_index_t knh_bytes_rindex(knh_bytes_t v, int ch)
{
	knh_index_t i;
	for(i = v.len - 1; i >= 0; i--) {
		if(v.buf[i] == ch) return i;
	}
	return -1;
}
#endif


#ifdef USE_bytes_first
static knh_bytes_t knh_bytes_first(knh_bytes_t t, knh_intptr_t loc)
{
	knh_bytes_t t2;
	t2.buf = t.buf;
	t2.len = loc;
	return t2;
}
#endif

#ifdef USE_bytes_last
static knh_bytes_t knh_bytes_last(knh_bytes_t t, knh_intptr_t loc)
{
	knh_bytes_t t2;
	t2.buf = t.buf + loc;
	t2.len = t.len - loc;
	return t2;
}
#endif

#ifdef USE_bytes_slice
static knh_bytes_t knh_bytes_slice(knh_bytes_t t, size_t s, size_t e)
{
	knh_bytes_t t2;
	t2.buf = t.buf + s;
	t2.len = e - s;
	DBG_ASSERT(s + t2.len <= t.len);
	return t2;
}
#endif

#ifdef USE_bytes_subbytes
static knh_bytes_t knh_bytes_subbytes(knh_bytes_t t, size_t off, size_t len)
{
	knh_bytes_t t2;
	t2.buf = t.buf + off;
	t2.len = len;
	DBG_ASSERT(off + len <= t.len);
	return t2;
}
#endif

#ifdef USE_bytes_parseint
static int knh_bytes_parseint(knh_bytes_t t, knh_int_t *value)
{
	knh_uint_t n = 0, prev = 0, base = 10;
	size_t i = 0;
	if(t.len > 1) {
		if(t.buf[0] == '0') {
			if(t.buf[1] == 'x') {
				base = 16; i = 2;
			}
			else if(t.buf[1] == 'b') {
				base = 2;  i = 2;
			}
		}else if(t.buf[0] == '-') {
			base = 10; i = 1;
		}
	}
	for(;i < t.len; i++) {
		int c = t.buf[i];
		if('0' <= c && c <= '9') {
			prev = n;
			n = n * base + (c - '0');
		}else if(base == 16) {
			if('A' <= c && c <= 'F') {
				prev = n;
				n = n * 16 + (10 + c - 'A');
			}else if('a' <= c && c <= 'f') {
				prev = n;
				n = n * 16 + (10 + c - 'a');
			}else {
				break;
			}
		}else if(c == '_') {
			continue;
		}else {
			break;
		}
		if(!(n >= prev)) {
			*value = 0;
			return 0;
		}
	}
	if(t.buf[0] == '-') n = -((knh_int_t)n);
	*value = n;
	return 1;
}

#endif

/* ------------------------------------------------------------------------ */

#ifdef USE_bytes_parsefloat
static int knh_bytes_parsefloat(knh_bytes_t t, knh_float_t *value)
{
#if defined(K_USING_NOFLOAT)
	{
		knh_int_t v = 0;
		knh_bytes_parseint(t, &v);
		*value = (knh_float_t)v;
	}
#elif defined(K_USING_LONGDOUBLE)
	*value = strtold((char*)t.buf, NULL);
#elif defined(K_USING_FLOAT)
	*value = strtof((char*)t.buf, NULL);
#else
	*value = strtod((char*)t.buf, NULL);
#endif
	return 1;
}
#endif

/* ------------------------------------------------------------------------ */
/* [cwb] */

#if defined(USE_cwb_open)
static knh_cwb_t *knh_cwb_open(Ctx* ctx, knh_cwb_t *cwb)
{
	cwb->ba = ((knh_Context_t*)ctx)->bufa;
	cwb->w  = ((knh_Context_t*)ctx)->bufw;
	cwb->pos = BA_size(((knh_Context_t*)ctx)->bufa);
	return cwb;
}
#endif

#if defined(USE_cwb_openinit)
static knh_cwb_t *knh_cwb_openinit(Ctx* ctx, knh_cwb_t *cwb, knh_bytes_t t)
{
	cwb->ba = ctx->bufa;
	cwb->w  = ctx->bufw;
	cwb->pos = BA_size(ctx->bufa);
	KNH_ASSERT(!(cwb->ba->bu.buf <= t.buf && t.buf < (cwb->ba->bu.buf + cwb->pos)));
	knh_Bytes_write(ctx, (cwb->ba), t);
	return cwb;
}
#endif

#if defined(USE_cwb_putc)
static void knh_cwb_putc(Ctx *ctx, knh_cwb_t *cwb, int ch)
{
	knh_Bytes_putc(ctx, (cwb->ba), ch);
}
#endif

#if defined(USE_cwb_write)
static void knh_cwb_write(Ctx *ctx, knh_cwb_t *cwb, knh_bytes_t t)
{
	KNH_ASSERT(!(cwb->ba->bu.buf <= t.buf && t.buf < (cwb->ba->bu.buf + cwb->pos)));
	knh_Bytes_write(ctx, (cwb->ba), t);
}
#endif

#if defined(USE_cwb_size)
static size_t knh_cwb_size(knh_cwb_t *cwb)
{
	return (cwb->ba)->bu.len - cwb->pos;
}
#endif

#if defined(USE_cwb_tobytes)
static knh_bytes_t knh_cwb_tobytes(knh_cwb_t *cwb)
{
	knh_bytes_t t = {{(cwb->ba)->bu.buf + cwb->pos}, (cwb->ba)->bu.len - cwb->pos};
	return t;
}
#endif


/* ------------------------------------------------------------------------ */
/* [FILE] */

#ifdef USE_fopen
static FILE* knh_fopen(Ctx *ctx, char *filename, char *mode, int isPERROR)
{
#if defined(K_USING_NOFILE)
	return NULL;
#elif defined(K_USING_STDC) || defined(K_USING_POSIX)
#ifndef KONOHA_ON_WINDOWS
	FILE *in = fopen(filename, mode);
#else
	FILE *in;
	fopen_s(&in, filename, mode);
#endif
	KNH_PERROR_IF(ctx, NULL, (in == NULL && isPERROR), "fopen");
	return in;
#else
	return NULL;
#endif
}
#endif

#ifdef USE_fgetc
static int knh_fgetc(Ctx *ctx, FILE *in)
{
#if defined(K_USING_NOFILE)
	return -1;
#elif defined(K_USING_STDC) || defined(K_USING_POSIX)
	return fgetc(in);
#else
	return -1;
#endif
}
#endif

#ifdef USE_fread
static size_t knh_fread(Ctx *ctx, void *ptr, size_t size, FILE *in)
{
#if defined(K_USING_NOFILE)
	return 0;
#elif defined(K_USING_STDC) || defined(K_USING_POSIX)
	return fread(ptr, 1, size, in);
#else
	return 0;
#endif
}
#endif

#ifdef USE_fwrite
static size_t knh_fwrite(Ctx *ctx, void *ptr, size_t size, FILE *in)
{
#if defined(K_USING_NOFILE)
	return 0;
#elif defined(K_USING_STDC) || defined(K_USING_POSIX)
	return fwrite(ptr, 1, size, in);
#else
	return 0;
#endif
}
#endif

#ifdef USE_fflush
static int knh_fflush(Ctx *ctx, FILE *in)
{
#if defined(K_USING_NOFILE)
	return 0;
#elif defined(K_USING_STDC) || defined(K_USING_POSIX)
	return fflush(in);
#else
	return 0;
#endif
}
#endif

#ifdef USE_fclose
static int knh_fclose(Ctx *ctx, FILE *in)
{
#if defined(K_USING_NOFILE)
	return 1;
#elif defined(K_USING_STDC) || defined(K_USING_POSIX)
	return fclose(in);
#else
	return 1;
#endif
}
#endif



/* ------------------------------------------------------------------------ */
/* [TIME] */

#ifdef USE_TIME_H
#if defined(K_USING_POSIX)
#include<time.h>
#include<sys/time.h>
#elif defined(K_USING_WINDOWS)
#include<windows.h>
#include <time.h>
#elif defined(K_USING_BTRON)
#include <btron/datetime.h>
#include <btron/event.h>
#endif
#endif

#ifdef USE_time
static knh_uint_t knh_time(void)
{
#if defined(K_USING_WINDOWS)
	return (knh_uint_t)time(NULL);
#elif defined(K_USING_POSIX)
	return (knh_uint_t)time(NULL);
#else
	return K_INT_MAX;
#endif
}
#endif

/* ------------------------------------------------------------------------ */

#ifdef USE_getTimeMilliSecond
static knh_uint64_t knh_getTimeMilliSecond(void)
{
#if defined(K_USING_WINDOWS)
	DWORD tickCount = GetTickCount();
	return (knh_int64_t)tickCount;
#elif defined(K_USING_POSIX)
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
#elif defined(K_USING_BTRON)
	/* FIXME: thread safety */
	static volatile int first = 1;
	static UW start = 0;
	UW current;
	if (first) {
		get_etm(&start);
		first = 0;
	}
	get_etm(&current);
	return (knh_uint64_t)((current - start) & 0x7fffffff);
#else
	return 0;
#endif
}
#endif

#ifdef __cplusplus
}
#endif

#endif /*INLINELIBS_H_*/
