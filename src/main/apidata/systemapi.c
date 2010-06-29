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

#include"commons.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef K_USING_DEFAULTAPI

#if !defined(KONOHA_ON_LKM) && !defined(K_USING_BTRON) && !defined(K_USING_WINDOWS)
#include <dirent.h>
#endif

/* ------------------------------------------------------------------------ */
//## @Static method InputStream System.getIn();

static METHOD System_getIn(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURN_(DP(ctx->sys)->in);
}

/* ------------------------------------------------------------------------ */
//## @Static method OutputStream System.getOut();

static METHOD System_getOut(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURN_(DP(ctx->sys)->out);
}

/* ------------------------------------------------------------------------ */
//## @Static method OutputStream System.getErr();

static METHOD System_getErr(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURN_(DP(ctx->sys)->err);
}

/* ------------------------------------------------------------------------ */
//## @Static method Any System.getProperty(String key);

static METHOD System_getProperty(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Object_t *v = (knh_Object_t*)knh_getPropertyNULL(ctx, S_tobytes(sfp[1].s));
	if(v == NULL) {
		v = KNH_NULVAL(CLASS_String);
		knh_setProperty(ctx, sfp[1].s, v);
	}
	RETURNa_(v);
}

/* ------------------------------------------------------------------------ */
//## @Static method Any System.setProperty(String key, Any value);

static METHOD System_setProperty(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_setProperty(ctx, sfp[1].s, sfp[2].o);
	RETURNa_(sfp[2].o);
}

/* ------------------------------------------------------------------------ */
//## @Hidden method String[] System.listProperties(String key);

static METHOD System_listProperties(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Array_t *a = new_Array(ctx, CLASS_String, 0);
	knh_bytes_t prefix = IS_NULL(sfp[1].s) ? STEXT("") : S_tobytes(sfp[1].s);
	knh_DictMap_t *map = DP(ctx->sys)->props;
	size_t i;
	for(i = 0; i < map->size; i++) {
		knh_String_t *key = knh_DictMap_keyAt(map, i);
		if(knh_bytes_matchWildCard(S_tobytes(key), prefix)) {
			knh_Array_add(ctx, a, key);
		}
	}
	RETURN_(a);
}

/* ------------------------------------------------------------------------ */
//## @Static method void System.gc();

static METHOD System_gc(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_System_gc(ctx);
}

/* ------------------------------------------------------------------------ */
//## @Static @Hidden method void System.push(Object value, ...);

static METHOD System_push(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	long i, ac = knh_stack_argc(ctx, (sfp+1));
	for(i = 0; i < ac; i++) {
		KNH_SETv(ctx, sfp[rix+i].o, sfp[i+1].o);
		sfp[rix+i].data = sfp[i+1].data;
	}
}

///* ------------------------------------------------------------------------ */
////## @Hidden method void System.test(Boolean result, String msg);
//
//static METHOD System_test(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	char *result = (sfp[1].bvalue) ? "PASS" : "FAILED";
//	knh_intptr_t line = (knh_intptr_t)sfp[0].ivalue;
//	knh_printf(ctx, KNH_STDERR, "[%s:%d]", result, line);
//	if(IS_bString(sfp[2].s)) {
//		knh_putc(ctx, KNH_STDERR, ' ');
//		knh_print(ctx, KNH_STDERR, S_tobytes(sfp[2].s));
//	}
//	knh_write_EOL(ctx, KNH_STDERR);
//}

/* ------------------------------------------------------------------------ */
//## method Int System.getTime();

static METHOD System_getTime(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNi_(knh_getTimeMilliSecond());
}

/* ------------------------------------------------------------------------ */
//## @Static @Unsafe method void System.exit(Int status);

static METHOD System_exit(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
#if defined(K_USING_STDC)
	int status = IS_NULL(sfp[1].o) ? 0 : Int_to(size_t, sfp[1]);
	KNH_SECURE(ctx, sfp);
	KNH_SYSLOG(ctx, LOG_NOTICE, "EXIT", "exiting by a user");
	exit(status);
#endif
	RETURNvoid_();
}

///* ------------------------------------------------------------------------ */
////## method String[] System.listDir(String path);
//
//static METHOD System_listDir(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
//	knh_Array_t *a = new_Array(ctx, CLASS_String, 0);
//	knh_bytes_t t = (IS_NULL(sfp[1].s)) ? STEXT(".") : S_tobytes(sfp[1].s);
//	knh_cwb_write(ctx, cwb, t);
//	knh_cwb_ospath(ctx, cwb);
//	KNH_SETv(ctx, sfp[2].o, a);
//#if defined(K_USING_POSIX)
//	{
//		char *dirname = knh_cwb_tochar(ctx, cwb);
//		DIR *dirptr = opendir(dirname);
//		KNH_PERROR_IF(ctx, sfp, (dirptr == NULL), "opendir");
//		if (dirptr != NULL) {
//			struct dirent *direntp;
//			while ((direntp = readdir(dirptr)) != NULL) {
//				char *p = direntp->d_name;
//				if(p[0] == '.' && (p[1] == 0 || p[1] == '.')) continue;
//				knh_Array_add(ctx, a, new_S(ctx, B(p)));
//			}
//			closedir(dirptr);
//		}
//	}
//#else
//	KNH_TODO("opendir for this platform");
//#endif
//	knh_cwb_close(cwb);
//	RETURN_(a);
//}

/* ------------------------------------------------------------------------ */
//## @Static method InputStream Context.setIn(InputStream? in);

static METHOD Context_setIn(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	KNH_SETv(ctx, ((knh_Context_t*)ctx)->in, sfp[1].o);
	RETURN_(sfp[1].o);
}

/* ------------------------------------------------------------------------ */
//## @Static method OutputStream Context.setOut(OutputStream? out);

static METHOD Context_setOut(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	KNH_SETv(ctx, ((knh_Context_t*)ctx)->out, sfp[1].o);
	RETURN_(sfp[1].o);
}

/* ------------------------------------------------------------------------ */
//## @Static method OutputStream Context.setErr(OutputStream? out);

static METHOD Context_setErr(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	KNH_SETv(ctx, ((knh_Context_t*)ctx)->err, sfp[1].o);
	RETURN_(sfp[1].o);
}

/* ------------------------------------------------------------------------ */
//## method Boolean Exception.opOF(Any msg);

static METHOD Exception_opOF(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	int isa = 0;
	if(IS_Class(sfp[1].o)) {
		isa = knh_Object_cid(sfp[0].o) == CLASS_Exception ? 1 : 0;
	}
	else if(IS_bString(sfp[1].o)){
		isa = knh_Exception_isa(ctx, sfp[0].e, sfp[1].s);
	}
	else if(IS_Exception(sfp[1].o)) {
		isa = knh_expt_isa(ctx, DP(sfp[0].e)->eid, DP(sfp[1].e)->eid);
	}
	RETURNb_(isa);
}

/* ------------------------------------------------------------------------ */
//## @Hidden @Const method Any NameSpace.setConst(String name, Object value);

static METHOD NameSpace_setConst(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_NameSpace_t *ns = sfp[0].ns;
	if(DP(ns)->lconstDictMapNULL == NULL) {
		KNH_INITv(DP(ns)->lconstDictMapNULL, new_DictMap0(ctx, 0));
	}
	knh_DictMap_set_(ctx, DP(ns)->lconstDictMapNULL, sfp[1].s, sfp[2].o);
	RETURNa_(sfp[2].o);
}

/* ------------------------------------------------------------------------ */
//## @Hidden @Const method Any Class.setConst(String name, Object value);

static METHOD Class_setConst(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_addClassConst(ctx, (sfp[0].c)->cid, sfp[1].s, sfp[2].o);
	RETURNa_(sfp[2].o);
}

/* ------------------------------------------------------------------------ */

#endif/*K_USING_DEFAULTAPI*/

#ifdef __cplusplus
}
#endif
