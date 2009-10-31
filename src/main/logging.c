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
#ifndef KONOHA_ON_LKM
#include<time.h>
#endif

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ======================================================================== */
/* [verbose] */

static
knh_OutputStream_t *knh_stack_pstream(Ctx *ctx, knh_flag_t flag)
{
	return KNH_STDOUT;
}

/* ------------------------------------------------------------------------ */

static
void knh_stack_beginPRINT(Ctx *ctx, knh_flag_t flag, knh_OutputStream_t *w)
{
	if(KNH_FLAG_IS(flag, KNH_FLAG_PF_BOL)) {
		if(KNH_FLAG_IS(flag, KNH_FLAG_PF_TIME)) {
	//		char buf[80];
	//		time_t timer;
	//		struct tm now_buf, *now;
	//		time(&timer);
	//		now = localtime_r(&timer, &now_buf);
	//		knh_snprintf(buf, sizeof(buf), "[%d/%d/%d %d/%d/%d] ", now->tm_year, now->tm_mon, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
	//		knh_write__s(ctx, w, buf);
		}
	}
}

/* ------------------------------------------------------------------------ */

static
void knh_stack_endPRINT(Ctx *ctx, knh_flag_t flag, knh_OutputStream_t *w)
{
	if(KNH_FLAG_IS(flag, KNH_FLAG_PF_BOL)) {
		knh_putc(ctx, w, ' ');
	}
	else if(KNH_FLAG_IS(flag, KNH_FLAG_PF_EOL)) {
		knh_write_EOL(ctx, w);
		knh_flush(ctx, w);
	}
	else if(KNH_FLAG_IS(flag, KNH_FLAG_PF_NAME)) {
		knh_putc(ctx, w, '=');
	}
	else {
		knh_putc(ctx, w, ',');
		knh_putc(ctx, w, ' ');
	}
}

/* ------------------------------------------------------------------------ */

void knh_stack_pmsg(Ctx *ctx, knh_flag_t flag, knh_String_t *s)
{
	knh_OutputStream_t *w = knh_stack_pstream(ctx, flag);
	knh_stack_beginPRINT(ctx, flag, w);
	DBG2_ASSERT(IS_bString(s));
//	if((s)->size > 0) {  /* remove TS_EMPTY */
	knh_print(ctx, w, __tobytes(s));
//	}
//	else {
//		flag = (flag & ~(KNH_FLAG_PF_BOL));
//	}
	knh_stack_endPRINT(ctx, flag, w);
}

/* ------------------------------------------------------------------------ */

void knh_stack_p(Ctx *ctx, knh_sfp_t *sfp, knh_flag_t flag, knh_methodn_t mn)
{
	knh_OutputStream_t *w = knh_stack_pstream(ctx, flag);
	knh_stack_beginPRINT(ctx, flag, w);
	knh_Method_t *mtd = knh_lookupFormatter(ctx, knh_Object_cid(sfp[1].o), mn);
	KNH_SETv(ctx, sfp[2].o, w);
	KNH_SETv(ctx, sfp[3].o, KNH_NULL);
	KNH_SCALL(ctx, sfp, 0, mtd, /*args*/2);
	knh_stack_endPRINT(ctx, flag, w);
}


///* ------------------------------------------------------------------------ */
//
//static int tglobalVerbose = 5;
//
///* ------------------------------------------------------------------------ */
//
//int knh_verbose(void)
//{
//	return tglobalVerbose;
//}

/* ======================================================================== */
/* [file] */

KNHAPI(char*) knh_safefile(char *file)
{
	if(file == NULL) return "(unknown)";
	knh_bytes_t t = B(file);
	int loc = knh_bytes_rindex(t, '/');
	if(loc == -1) {
		return file;
	}
	return (char*)t.buf + loc + 1;
}


/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
