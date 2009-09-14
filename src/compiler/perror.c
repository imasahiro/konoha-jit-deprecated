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
/* [perror] */

static volatile size_t countErrorWarning = 0;
static volatile size_t countBadManner    = 0;
static volatile size_t countKonohaStyle  = 0;
static volatile size_t countLine         = 1;

/* ------------------------------------------------------------------------ */

void knh_resetSourceMoniter(void)
{
	countErrorWarning = 0;
	countBadManner = 0;
	countKonohaStyle = 0;
	countLine = 1;
}

/* ------------------------------------------------------------------------ */

void knh_addSourceLine(size_t line)
{
	countLine += line;
}

/* ------------------------------------------------------------------------ */

void knh_foundKonohaStyle(size_t score)
{
	countKonohaStyle += score;
}

/* ------------------------------------------------------------------------ */

size_t knh_getSourceScore(void)
{
	return (countErrorWarning + countBadManner) * 1000 / countLine;
}

/* ------------------------------------------------------------------------ */

size_t knh_getKonohaScore(void)
{
	return (countKonohaStyle) * 1000 / countLine;
}

/* ------------------------------------------------------------------------ */
/* @data */

static const char *KERR_MSG[] = {
	"(error)", "(type error)", "(warning)",
	"(warning)", "(errata)", "(bad manner)",
	"(type inference)", "(notice)", "(info)",
};

/* ------------------------------------------------------------------------ */

void knh_vperror(Ctx *ctx, knh_uri_t uri, int line, int pe, char *fmt, va_list ap)
{
	KNH_ASSERT(pe <= KERR_INFO);
	if(pe < KERR_DWARN) {
		countErrorWarning += 1;
	}
	else if(pe < KERR_TINFO) {
		countBadManner += 1;
	}
	if(knh_Context_isInteractive(ctx)) {
		goto L_PRINT;
	}
	if(pe > KERR_EWARN && !knh_Context_isCompiling(ctx)) {
		return;
	}
	L_PRINT:;
	{
		knh_OutputStream_t *w = KNH_STDERR;
		knh_printf(ctx, w, " - [%s:%d]:%s ", FILEN(uri), (knh_intptr_t)line, KERR_MSG[pe]);
		knh_vprintf(ctx, w, fmt, ap);
		knh_write_EOL(ctx, w);
		((knh_Context_t*)ctx)->hasError = 1;
	}
}

/* ------------------------------------------------------------------------ */

void knh_perror(Ctx *ctx, knh_uri_t uri, int line, int pe, char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	knh_vperror(ctx, uri, line, pe, fmt, ap);
	va_end(ap);
}

/* ------------------------------------------------------------------------ */

void knh_Token_perror(Ctx *ctx, knh_Token_t *tk, int pe, char *fmt, ...)
{
	if(SP(tk)->tt != TT_ERR) {
		va_list ap;
		va_start(ap, fmt);
		knh_vperror(ctx, SP(tk)->uri, SP(tk)->line, pe, fmt, ap);
		va_end(ap);
		if(pe < KERR_EWARN) {
			SP(tk)->tt = TT_ERR;
		}
	}
}

/* ------------------------------------------------------------------------ */

void knh_Asm_perror(Ctx *ctx, knh_Asm_t *abr, int pe, char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	knh_vperror(ctx, DP(abr)->uri, (int)DP(abr)->line, pe, fmt, ap);
	va_end(ap);
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
