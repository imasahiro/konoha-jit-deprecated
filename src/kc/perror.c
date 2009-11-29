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

void knh_foundKonohaStyle(Ctx *ctx, size_t score)
{
	DP(ctx->kc)->statKonohaStyle += score;
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
	DBG2_ASSERT(pe <= KERR_INFO);
	knh_Gamma_t *kc = ctx->kc;
	if(pe < KERR_DWARN) {
		DP(kc)->statError += 1;
	}
	else if(pe < KERR_TINFO) {
		DP(kc)->statBadManner += 1;
	}
	if(knh_Gamma_isQuiet(kc)) {
		return;
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

void knh_Gamma_perror(Ctx *ctx, int pe, char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	knh_vperror(ctx, SP(ctx->kc)->uri, (int)SP(ctx->kc)->line, pe, fmt, ap);
	va_end(ap);
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
