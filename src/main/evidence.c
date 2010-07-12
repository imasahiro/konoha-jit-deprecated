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

//#define USE_B             1
#define USE_bytes_rindex  1
#define USE_cwb_open      1

#include"commons.h"
#ifndef KONOHA_ON_LKM
#include<time.h>
#endif

#ifdef KONOHA_ON_LKM
#include<linux/errno.h>
static int errno;
#else

#ifdef K_USING_POSIX
#include<errno.h>
#elif !defined(KONOHA_ON_WINDOWS)
static int errno_WHO_ADDED_THIS;      //@ Who added this? (kimio)
#endif
#endif

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */
/* [file] */

const char* knh_sfile(const char *file)
{
	if(file != NULL) {
		knh_bytes_t t;
		t.text = file; t.len = knh_strlen(file);
		int loc = knh_bytes_rindex(t, '/');
		if(loc == -1) {
			return file;
		}
		return t.text + loc + 1;
	}
	return "(unknown)";
}

/* ------------------------------------------------------------------------ */

void dbg_p(const char *file, const char *func, int line, const char *fmt, ...)
{
#ifndef K_USING_DEBUG
	if(knh_isSystemVerbose()) {
#endif
		va_list ap;
		va_start(ap , fmt);
		fflush(stdout);
		fprintf(stderr, "DEBUG(%s:%d) ", func, line);
		vfprintf(stderr, fmt, ap);
		fprintf(stderr, "\n");
		va_end(ap);
#ifndef K_USING_DEBUG
	}
#endif
}

/* ------------------------------------------------------------------------ */

void todo_p(const char *file, const char *func, int line, const char *fmt, ...)
{
	va_list ap;
	va_start(ap , fmt);
	fflush(stdout);
	fprintf(stderr, "TODO(%s:%d) ", knh_sfile((char*)file), line);
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	va_end(ap);
}

/* ------------------------------------------------------------------------ */

void knh_write_cline(Ctx *ctx, knh_OutputStream_t *w, char *file, long line)
{
	knh_putc(ctx, w, '[');
	knh_write_text(ctx, w, knh_sfile(file));
	knh_putc(ctx, w, ':');
	knh_write_dfmt(ctx, w, K_INTPTR_FMT, line);
	knh_putc(ctx, w, ']'); knh_putc(ctx, w, ' ');
}

/* ------------------------------------------------------------------------ */

void knh_write_uline(Ctx *ctx, knh_OutputStream_t *w, knh_uri_t uri, long line)
{
	knh_putc(ctx, w, '[');
	knh_write_text(ctx, w, FILENAME__(uri));
	knh_putc(ctx, w, ':');
	knh_write_dfmt(ctx, w, K_INTPTR_FMT, line);
	knh_putc(ctx, w, ']'); knh_putc(ctx, w, ' ');
}

/* ------------------------------------------------------------------------ */

const char* LOG_tochar(int p)
{
	switch(p) {
	case LOG_EMERG:   return "PANIC";
	case LOG_ALERT:   return "ALERT";
	case LOG_CRIT:    return "CRIT";
	case LOG_ERR:     return "ERROR";
	case LOG_WARNING: return "WARNING";
	case LOG_NOTICE:  return "NOTICE";
	case LOG_INFO:    return "INFO";
	case LOG_DEBUG:   return "DEBUG";
	}
	DBG_P("priority=%d", p);
	return "unknown";
}

/* ------------------------------------------------------------------------ */
/* [syslog] */

static void knh_vsyslog(Ctx *ctx, int p, const char *fmt, va_list ap)
{
	if(p > LOG_WARNING && !knh_isSystemVerbose()) return;
#ifdef KONOHA_ON_LKM
	//TODO_IDE();
	vprintk(fmt, ap);
#else
	if(ctx == NULL) {
		fprintf(stderr, "konoha[%s] ", LOG_tochar(p));
		vfprintf(stderr, fmt, ap);
		fprintf(stderr, "\n");
	}
	else {
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		knh_printf(ctx, cwb->w, "konoha[%s] ", LOG_tochar(p));
		knh_vprintf(ctx, cwb->w, fmt, ap);
		fprintf(stderr, "%s\n", knh_cwb_tochar(ctx, cwb));
		knh_cwb_clear(cwb, 0);
	}
#endif
}

#define K_EVENT_FORMAT "[%s:%s] "

void knh_makeEvidence(Ctx *ctx, const char *ns, const char *event, int p, const char *fmt, ...)
{
	if(p > LOG_WARNING && !knh_isSystemVerbose()) return;
	va_list ap;
	va_start(ap , fmt);
	if(fmt[0] == '*') {
		char newfmt[512];
		knh_snprintf(newfmt, sizeof(newfmt), K_EVENT_FORMAT "%s", ns, event, fmt+1);
		ctx->share->ebiSPI->vsyslog(p, newfmt, ap);
	}
	else {
		DBG_ASSERT(ctx->bufa != NULL);
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		knh_printf(ctx, cwb->w, K_EVENT_FORMAT, ns, event);
		if(ctx->gma != NULL && SP(ctx->gma)->line != 0) {
			knh_write_uline(ctx, cwb->w, SP(ctx->gma)->uri, SP(ctx->gma)->line);
		}
		knh_vprintf(ctx, cwb->w, (char*)fmt, ap);
		ctx->share->ebiSPI->syslog(p, knh_cwb_tochar(ctx, cwb));
		knh_cwb_clear(cwb, 0);
	}
	va_end(ap);
}

void knh_stack_perror(Ctx *ctx, knh_sfp_t *sfp, const char *ns, const char *event)
{
	int errno_ = errno;
	int p = (errno_ != 13) ? LOG_ERR : LOG_ALERT;
#ifndef KONOHA_ON_WINDOWS
	char *emsg = strerror(errno_);
#else
	char emsg[256];
	strerror_s(emsg, 256, errno_);
#endif
	ctx->share->ebiSPI->syslog(p, K_EVENT_FORMAT "errno=%d, msg='%s'", ns, event, errno_, emsg);
}

/* ======================================================================== */
/* [perror] */

void knh_foundKonohaStyle(Ctx *ctx, size_t score)
{
	DP(ctx->gma)->statKonohaStyle += score;
}

/* ------------------------------------------------------------------------ */
/* @data */

static const char* KERR_tochar(int p)
{
	switch(p) {
	case LOG_EMERG:
	case LOG_ALERT:
	case LOG_CRIT: return "(error)";
	case LOG_ERR:  return "(type error)";
	case LOG_WARNING: return "(warning)";
	case LOG_NOTICE:  return "(bad manner)";
	case LOG_INFO:    return "(info)";
	case LOG_DEBUG:   return "(errata)";
	}
	DBG_P("priority=%d", p);
	return "unknown";
}

/* ------------------------------------------------------------------------ */

void knh_vperror(Ctx *ctx, knh_uri_t uri, int line, int pe, const char *fmt, va_list ap)
{
	DBG_ASSERT(pe <= LOG_DEBUG);
	if(knh_Gamma_isQuiet(ctx->gma)) {
		return;
	}
	if(pe < KERR_DWARN) {
		DP(ctx->gma)->statError += 1;
	}
	else if(pe < KERR_TINFO) {
		DP(ctx->gma)->statBadManner += 1;
	}
	if(knh_Context_isInteractive(ctx)) {
		goto L_PRINT;
	}
	if(pe > KERR_EWARN && !knh_Context_isCompiling(ctx)) {
		return;
	}
	L_PRINT:;
	if(line > 0) {
		knh_OutputStream_t *w = KNH_STDERR;
		knh_printf(ctx, w, " - [%s:%d]:%s ", FILENAME__(uri), (knh_intptr_t)line, KERR_tochar(pe));
		knh_vprintf(ctx, w, fmt, ap);
		knh_write_EOL(ctx, w);
	}
	else {
		knh_vsyslog(ctx, pe, fmt, ap);
	}
}

/* ------------------------------------------------------------------------ */

void knh_perror(Ctx *ctx, knh_uri_t uri, int line, int pe, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	knh_vperror(ctx, uri, line, pe, fmt, ap);
	va_end(ap);
}


/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
