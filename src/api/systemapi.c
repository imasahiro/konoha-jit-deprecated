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
#if !defined(KONOHA_ON_LKM) && !defined(KNH_USING_BTRON) && !defined(KNH_USING_WINDOWS)
#include <dirent.h>
#endif

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef KNH_CC_METHODAPI

/* ======================================================================== */
/* [System] */

/* ------------------------------------------------------------------------ */
//## @Static method InputStream! System.getIn();

static METHOD System_getIn(Ctx *ctx, knh_sfp_t *sfp)
{
	KNH_RETURN(ctx, sfp, DP(ctx->sys)->in);
}

/* ------------------------------------------------------------------------ */
//## @Static method OutputStream! System.getOut();

static METHOD System_getOut(Ctx *ctx, knh_sfp_t *sfp)
{
	KNH_RETURN(ctx, sfp, DP(ctx->sys)->out);
}

/* ------------------------------------------------------------------------ */
//## @Static method OutputStream! System.getErr();

static METHOD System_getErr(Ctx *ctx, knh_sfp_t *sfp)
{
	KNH_RETURN(ctx, sfp, DP(ctx->sys)->err);
}

/* ------------------------------------------------------------------------ */
//## @Static method Any? System.getProperty(String! key);

static METHOD System_getProperty(Ctx *ctx, knh_sfp_t *sfp)
{
	KNH_RETURN(ctx, sfp,
		knh_System_getProperty(ctx,(knh_System_t*)sfp[0].o, __tobytes(sfp[1].s)));
}

/* ------------------------------------------------------------------------ */
//## @Static method void System.setProperty(String! key, Any? value);

static METHOD System_setProperty(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_System_setProperty(ctx, (knh_System_t*)sfp[0].o, sfp[1].s, sfp[2].o);
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## @Static method void System.gc();

static METHOD System_gc(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_System_gc(ctx);
}

/* ------------------------------------------------------------------------ */
//## @static method Int Script.system(String cmd);
//## @static method Int Script.sudo(String cmd);

static METHOD Script_system(Ctx *ctx, knh_sfp_t *sfp)
{
    KNH_SECURE(ctx,sfp);
    int ret = system(String_to(char*, sfp[1]));
    if(ret  == -1) {
        KNH_PERRNO(ctx, NULL, "OS!!", "system", knh_Context_isStrict(ctx));
    }
    KNH_RETURN_Int(ctx, sfp,ret);
}

/* ------------------------------------------------------------------------ */
//## @Static method Boolean! System.hasLib(String! lib, String? func);

static METHOD System_hasLib(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_bytes_t libname = __tobytes(sfp[1].s);
	knh_cwb_write(ctx, cwb, libname);
	knh_cwb_ospath(ctx, cwb);
	void *p = knh_cwb_dlopen(ctx, cwb);
	int res = 0;
	if(p == NULL && !knh_bytes_startsWith(libname, STEXT("lib"))) {
		knh_cwb_subclear(cwb, 0);
		knh_cwb_write(ctx, cwb, STEXT("lib"));
		knh_cwb_write(ctx, cwb, libname);
		p = knh_cwb_dlopen(ctx, cwb);
	}
	if(p != NULL) {
		res = 1;
		if(IS_NOTNULL(sfp[2].o)) {
			void *f = knh_dlsym(ctx, p, p_char(sfp[2]));
			res = (f != NULL);
		}
	}
	knh_cwb_close(cwb);
	KNH_RETURN_Boolean(ctx, sfp, res);
}

/* ------------------------------------------------------------------------ */
//## @Static @Audit method void System.exit(Int status);

static METHOD System_exit(Ctx *ctx, knh_sfp_t *sfp)
{
#if defined(KNH_USING_STDC)
	int status = IS_NULL(sfp[1].o) ? 0 : p_int(sfp[1]);
	KNH_SECURE(ctx, sfp);
	KNH_NOTICE(ctx, "exiting by a user");
	exit(status);
#endif
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## method String[] System.listDir(String? path);

static METHOD System_listDir(Ctx *ctx, knh_sfp_t *sfp)
{
#if defined(KNH_USING_POSIX)
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_Array_t *a = new_Array(ctx, CLASS_String, 0);
	knh_bytes_t t = (IS_NULL(sfp[1].s)) ? STEXT(".") : __tobytes(sfp[1].s);
	knh_cwb_write(ctx, cwb, t);
	knh_cwb_ospath(ctx, cwb);
	KNH_SETv(ctx, sfp[2].o, a);
	DIR *dirptr;
	char *dirname = knh_cwb__tochar(ctx, cwb);
	if ((dirptr = opendir(dirname)) == NULL) {
		KNH_PERRNO(ctx, cwb, "OS!!", "opendir", knh_Context_isStrict(ctx));
	} else {
		struct dirent *direntp;
		while ((direntp = readdir(dirptr)) != NULL) {
			char *p = direntp->d_name;
			if(p[0] == '.' && (p[1] == 0 || p[1] == '.')) continue;
			knh_Array_add(ctx, a, UP(new_String(ctx, B(p), NULL)));
		}
		closedir(dirptr);
	}
	knh_cwb_close(cwb);
	KNH_RETURN(ctx, sfp, a);
#endif
}

/* ------------------------------------------------------------------------ */
//## method Boolean! System.hasDir(String! path);

static METHOD System_hasDir(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_openinit(ctx, &cwbbuf, __tobytes(sfp[1].s));
	knh_cwb_ospath(ctx, cwb);
	knh_bool_t res = knh_cwb_isdir(ctx, cwb);
	knh_cwb_close(cwb);
	KNH_RETURN_Boolean(ctx,sfp, res);
}

/* ------------------------------------------------------------------------ */
//## method Boolean! System.hasFile(String! path);

static METHOD System_hasFile(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_openinit(ctx, &cwbbuf, __tobytes(sfp[1].s));
	knh_cwb_ospath(ctx, cwb);
	knh_bool_t res = knh_cwb_isfile(ctx, cwb);
	knh_cwb_close(cwb);
	KNH_RETURN_Boolean(ctx,sfp, res);
}

/* ------------------------------------------------------------------------ */
//## @Audit method Boolean! System.mkdir(String! path);

static METHOD System_mkdir(Ctx *ctx, knh_sfp_t *sfp)
{
	KNH_SECURE(ctx, sfp);
	KNH_RETURN_Boolean(ctx, sfp,
		knh_mkdir(ctx, __tobytes(sfp[1].s), knh_Context_isStrict(ctx))
	);
}

/* ------------------------------------------------------------------------ */
//## @Audit method Boolean! System.unlink(String! path);

static METHOD System_unlink(Ctx *ctx, knh_sfp_t *sfp)
{
	KNH_SECURE(ctx, sfp);
	KNH_RETURN_Boolean(ctx, sfp,
		knh_unlink(ctx, __tobytes(sfp[1].s), knh_Context_isStrict(ctx))
	);
}

/* ------------------------------------------------------------------------ */
//## @Audit method Boolean! System.rename(String path, String newpath);

static METHOD System_rename(Ctx *ctx, knh_sfp_t *sfp)
{
	KNH_SECURE(ctx, sfp);
	KNH_RETURN_Boolean(ctx, sfp,
		knh_rename(ctx, __tobytes(sfp[1].s),
			__tobytes(sfp[2].s), knh_Context_isStrict(ctx))
	);
}

/* ======================================================================== */
/* [Context] */

/* ------------------------------------------------------------------------ */
//## @Static method Any Context.getProperty(String! key);

static METHOD Context_getProperty(Ctx *ctx, knh_sfp_t *sfp)
{
	KNH_RETURN(ctx, sfp,
		knh_Context_getProperty(ctx, (knh_Context_t*)sfp[0].o,
				__tobytes(sfp[1].s)));
}

/* ------------------------------------------------------------------------ */
//## @Static method void Context.setProperty(String! key, Any? value);

static METHOD Context_setProperty(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_Context_setProperty(ctx, (knh_Context_t*)sfp[0].o, sfp[1].s, sfp[2].o);
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## @Static method void Context.setEncoding(String? enc);

static METHOD Context_setEncoding(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_Context_setEncoding(ctx, (knh_Context_t*)sfp[0].o, sfp[1].s);
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## @Hidden method String[] Context.listProperties(String key);

static METHOD Context_listProperties(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_DictSet_t *ds = new_DictSet(ctx, 64);
	KNH_MOV(ctx, sfp[0].o, ds); // TO RCGC
	if(IS_NULL(sfp[1].s)) {
		KNH_MOV(ctx, sfp[1].o, TS_EMPTY);
	}
	knh_bytes_t prefix = __tobytes(sfp[1].s);
	knh_DictMap_t *map = DP(ctx->sys)->props;
	size_t i;
	for(i = 0; i < map->size; i++) {
		if(IS_NOTNULL(knh_DictMap_valueAt(map, i))) {
			knh_String_t *key = knh_DictMap_keyAt(map, i);
			if(knh_bytes_matchWildCard(__tobytes(key), prefix)) {
				knh_DictSet_add(ctx, ds, key);
			}
		}
	}
	map = ctx->props;
	for(i = 0; i < map->size; i++) {
		if(IS_NOTNULL(knh_DictMap_valueAt(map, i))) {
			knh_String_t *key = knh_DictMap_keyAt(map, i);
			if(knh_bytes_matchWildCard(__tobytes(key), prefix)) {
				knh_DictSet_add(ctx, ds, key);
			}
		}
	}
	knh_Array_t *a = new_Array(ctx, CLASS_String, ds->size);
	for(i = 0; i < ds->size; i++) {
		knh_Array_add(ctx, a, UP(knh_DictSet_keyAt(ds, i)));
	}
	KNH_RETURN(ctx, sfp, a);
}

/* ------------------------------------------------------------------------ */
//## @Static method void Context.setIn(InputStream? in);

static METHOD Context_setIn(Ctx *ctx, knh_sfp_t *sfp)
{
	if(IS_NULL(sfp[1].o)) {
		KNH_SETv(ctx, ((knh_Context_t*)ctx)->in, DP(ctx->sys)->in);
	}
	else {
		KNH_SETv(ctx, ((knh_Context_t*)ctx)->in, sfp[1].o);
	}
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## @Static method void Context.setOut(OutputStream? out);

static METHOD Context_setOut(Ctx *ctx, knh_sfp_t *sfp)
{
	if(IS_NULL(sfp[1].o)) {
		KNH_SETv(ctx, ((knh_Context_t*)ctx)->out, DP(ctx->sys)->out);
	}
	else {
		KNH_SETv(ctx, ((knh_Context_t*)ctx)->out, sfp[1].o);
	}
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## @Static method void Context.setErr(OutputStream? out);

static METHOD Context_setErr(Ctx *ctx, knh_sfp_t *sfp)
{
	if(IS_NULL(sfp[1].o)) {
		KNH_SETv(ctx, ((knh_Context_t*)ctx)->err, DP(ctx->sys)->err);
	}
	else {
		KNH_SETv(ctx, ((knh_Context_t*)ctx)->err, sfp[1].o);
	}
	KNH_RETURN_void(ctx, sfp);
}

/* ======================================================================== */
/* [movabletext] */

//## method void Context.%dump(OutputStream w, String m);

static void knh_Context__dump(Ctx *ctx, knh_Context_t *b, knh_OutputStream_t *w, knh_String_t *m)
{
//	knh_int_t i = 0;
//	for(i = b->esp - b->stack; i >= 0; i--) {
//		knh_printf(ctx, w, "[%d]\t%o\n", i, b->stack[i]);
//	}
}

/* ------------------------------------------------------------------------ */



/* ------------------------------------------------------------------------ */
//## method Boolean! Exception.opIsa(String msg);

static METHOD Exception_opIsa(Ctx *ctx, knh_sfp_t *sfp)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_Exception_isa(ctx, sfp[0].e, sfp[1].s));
}

/* ------------------------------------------------------------------------ */
//## @Hidden @Const method void NameSpace.setConst(String! name, Any value);

static METHOD NameSpace_setConst(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_NameSpace_addConst(ctx, sfp[0].ns, sfp[1].s, sfp[2].o);
}

/* ------------------------------------------------------------------------ */

#endif/*KNH_CC_METHODAPI*/

#ifdef __cplusplus
}
#endif
