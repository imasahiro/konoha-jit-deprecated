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

#define USE_STEXT 1
#define USE_B     1
#define USE_bytes_startsWith    1
#define USE_bytes_endsWith      1

#define USE_cwb_open      1
#define USE_cwb_openinit  1
#define USE_cwb_tobytes   1
#define USE_cwb_write     1
#define USE_cwb_putc      1

#include"commons.h"

#ifdef KONOHA_ON_LKM
#undef K_USING_POSIX
#endif

#ifdef K_USING_POSIX
#include <unistd.h>
#include <signal.h>
#include <dirent.h>
#include <sys/stat.h>
#include <dlfcn.h>
#include <time.h>
#include <sys/time.h>
#include <sys/param.h>
#ifdef KONOHA_ON_MACOSX
#include <mach-o/dyld.h>
#endif

#endif/*K_USING_POSIX*/

#ifdef K_USING_WINDOWS
#include<windows.h>
#endif

#ifdef K_USING_BTRON
#include<btron/file.h>
#include<btron/dynload.h>
#include<tstring.h>
#endif

#ifdef HAVE_LOCALE_H
#include<locale.h>
#endif

#ifdef HAVE_LOCALCHARSET_H
#include<localcharset.h>
#endif

#ifdef HAVE_LANGINFO_H
#include<langinfo.h>
#endif

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

#define K_PERROR_BEFORE_RETRUN

#if defined(K_USING_WINDOWS)
#define K_PERROR_LIBNAME "Win32/64"
#define K_PERROR_OK  1
#define K_PERROR_FAILED  0

#else
#define K_PERROR_LIBNAME "libc"
#define K_PERROR_OK       0
#define K_PERROR_FAILED  -1

#if !defined(K_USING_POSIX)
static int UnsupportedAPI(Ctx* ctx, const char *funcname)
{
	ctx->api->perror(ctx, NULL, K_PERROR_LIBNAME, funcname);
	return K_PERROR_OK;
}
#endif
#endif

static void knh_Bytes_remove(Ctx *ctx, knh_Bytes_t *ba, size_t offset, size_t len)
{
	DBG_ASSERT(offset + len < BA_size(ba));
	memmove(ba->bu.ubuf + offset + len, ba->bu.ustr + offset, len);
	knh_bzero(ba->bu.ubuf + (BA_size(ba) - len), len);
	BA_size(ba) = BA_size(ba) - len;
}

static void knh_Bytes_concatZero(Ctx *ctx, knh_Bytes_t *ba)
{
	DBG_ASSERT(!knh_Bytes_isStatic(ba));
	if(BA_size(ba) > 0 && ba->bu.ubuf[BA_size(ba)-1] == 0) {
		BA_size(ba) -= 1;
	}
}

/* ------------------------------------------------------------------------ */

const char* knh_cwb_ospath(Ctx *ctx, knh_cwb_t* cwb)
{
	knh_bytes_t path = knh_cwb_tobytes(cwb);
	int hasUTF8 = 0;
	if(knh_bytes_startsWith(path, STEXT("file:"))) {
		knh_Bytes_remove(ctx, cwb->ba, cwb->pos, 5);
	}
	{
		size_t i;
		for(i = 0; i < path.len; i++) {
			int ch = path.ubuf[i];
			if(ch == '/' || ch == '\\') {
				path.ubuf[i] = K_FILESEPARATOR;
			}
			if(ch > 127) hasUTF8 = 1;
		}
	}
	if(hasUTF8) {
		KNH_TODO("multibytes file path");
	}
	return knh_cwb_tochar(ctx, cwb);
}

/* ------------------------------------------------------------------------ */

const char* knh_cwb_realpath(Ctx *ctx, knh_cwb_t *cwb)
{
	const char *p = knh_cwb_tochar(ctx, cwb);
#if defined(K_USING_POSIX)
#if defined(PATH_MAX)
	char buf[PATH_MAX] = {0};
#else
	char *buf = NULL;
#endif
	p = realpath(p, buf);
	if(p == NULL) {
		knh_cwb_clear(cwb, 0);
		knh_Bytes_write(ctx, cwb->ba, B(buf));
#if !defined(PATH_MAX)
	free(p);
#endif
	}
	p = knh_cwb_tochar(ctx, cwb);
#elif defined(KONOHA_ON_WINDOWS)
#if defined(PATH_MAX)
	char buf[PATH_MAX] = {0};
	int path_max = PATH_MAX;
#else
	char *buf = NULL;
	int path_max = 0;
#endif
	if(_fullpath(buf, p, path_max) != NULL) {
		knh_cwb_clear(cwb, 0);
		knh_Bytes_write(ctx, cwb->ba, B(buf));
#if !defined(PATH_MAX)
	free(p);
#endif
	}
	p = knh_cwb_tochar(ctx, cwb);
#else
	KNH_HELP("realpath function for this architecture");
#endif
	return p;
}

/* ------------------------------------------------------------------------ */

knh_bool_t knh_cwb_isfile(Ctx *ctx, knh_cwb_t *cwb)
{
	knh_bool_t res = 1;
	const char *pathname = knh_cwb_tochar(ctx, cwb);
#if defined(K_USING_WINDOWS)
	DWORD attr = GetFileAttributesA(pathname);
	if(attr == -1 || (attr & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) res = 0;
#elif defined(K_USING_POSIX)
    struct stat buf;
	if(stat(pathname, &buf) == -1) res = 0;
	else res = S_ISREG(buf.st_mode);
#elif defined(K_USING_BTRON)
	FILE* in = fopen(pathname,"r");
	if(in == NULL)  res = 0;
	else fclose(in);
#else
	(void)pathname;
	return 0;
#endif
	if(res == 0) {
		DBG_P("isfile='%s' NOTFOUND", pathname);
	}
	return res;
}

/* ------------------------------------------------------------------------ */

knh_bool_t knh_cwb_isdir(Ctx *ctx, knh_cwb_t *cwb)
{
	const char *pathname = knh_cwb_tochar(ctx, cwb);
#if defined(K_USING_WINDOWS)
	DWORD attr = GetFileAttributesA(pathname);
	if(attr == -1) return 0;
	return ((attr & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY);
#elif defined(K_USING_POSIX)
    struct stat buf;
	if(stat(pathname, &buf) == -1) return 0;
	return S_ISDIR(buf.st_mode);
#else
	// avoid "unused variable" warning unused variable
	(void)pathname;
	return 0;
#endif
}

/* ------------------------------------------------------------------------ */

#define SUBPATH_BUFSIZ 40

knh_bool_t knh_cwb_parentpath(Ctx *ctx, knh_cwb_t *cwb, char *subbuf)
{
	knh_bytes_t path = knh_cwb_tobytes(cwb);
	knh_intptr_t i;
	for(i = path.len - 1; i > 0; i--) {
		if(path.ustr[i] == '/' || path.ustr[i] == '\\') {
			if(subbuf != NULL) {
				knh_snprintf(subbuf, SUBPATH_BUFSIZ, "%s", path.text + i);
			}
			knh_cwb_clear(cwb, i);
			return 1;
		}
	}
	knh_cwb_clear(cwb, 0);
	return 0;
}

/* ------------------------------------------------------------------------ */

static knh_bool_t knh_cwb_mkdir(Ctx *ctx, knh_cwb_t *cwb, char *subpath)
{
	const char *pathname;
	if(knh_cwb_isdir(ctx, cwb)) {
		char subbuf[SUBPATH_BUFSIZ];
		if(knh_cwb_parentpath(ctx, cwb, subbuf)) {
			if(knh_cwb_mkdir(ctx, cwb, subpath) == 0) {
				knh_cwb_write(ctx, cwb, B(subbuf));
			}
			else {
				return 0;
			}
		}
	}
	pathname = knh_cwb_tochar(ctx, cwb);
#if defined(K_USING_WINDOWS)
	PERROR_returnb_(CreateDirectoryA, pathname, NULL);
#elif defined(K_USING_POSIX)
	PERROR_returnb_(mkdir, pathname, 0777);
#else
	PERROR_returnb_(UnsupportedAPI, ctx, __FUNCTION__);
#endif
}

/* ------------------------------------------------------------------------ */

knh_bool_t knh_mkdir(Ctx *ctx, knh_bytes_t path)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_openinit(ctx, &cwbbuf, path);
	knh_bool_t res = 1;
	knh_cwb_ospath(ctx, cwb);
	if(!knh_cwb_isdir(ctx, cwb)) {
		knh_cwb_realpath(ctx, cwb);
		res = knh_cwb_mkdir(ctx, cwb, NULL);
	}
	knh_cwb_close(cwb);
	return res;
}

/* ======================================================================== */
/* [homepath] */

// $konoha.path /usr/local/konoha
// $konoha.bin.path  /usr/local/bin/konoha
// $konoha.package.path {$konoha.path}/package
// $konoha.script.path  {$konoha.path}/script

void knh_System_initPath(Ctx *ctx, knh_System_t *o)
{
	knh_SystemEX_t *sys = DP(o);
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	char *homepath = knh_getenv("KONOHA_HOME");
	knh_String_t *shome;
	knh_bytes_t home = {{NULL}, 0};
#if defined(KNH_PREFIX)
	if(homepath == NULL) {
		homepath = KNH_PREFIX "/konoha";
		DBG_P("homepath='%s'", homepath);
	}
#endif
	if(homepath != NULL) {
		knh_DictMap_set_(ctx, sys->props, new_T("konoha.path"), UPCAST(new_T(homepath)));
		home = B(homepath);
	}
#if defined(K_USING_WINDOWS)
	{
		char buf[FILEPATH_BUFSIZ];
		int bufsiz = FILEPATH_BUFSIZ;
		HMODULE h = LoadLibrary(NULL);
		GetModuleFileNameA(h, buf, bufsiz);
		knh_cwb_write(ctx, cwb, B(buf));
		knh_DictMap_set_(ctx, sys->props,
			new_T("konoha.bin.path"), UPCAST(knh_cwb_newString(ctx, cwb)));
		if(homepath == NULL) {
			GetModuleFileNameA(h, buf, bufsiz);
			knh_cwb_write(ctx, cwb, B(buf));
			knh_cwb_parentpath(ctx, cwb, NULL);
			knh_cwb_parentpath(ctx, cwb, NULL);
			shome = knh_cwb_newString(ctx, cwb);
			home = S_tobytes(shome);
			knh_DictMap_set_(ctx, sys->props, new_T("konoha.path"), UPCAST(shome));
		}
	}
#elif defined(KONOHA_ON_LINUX)
	// @url(http://shinh.skr.jp/binary/b2con.html)
	// http://doc.trolltech.com/3.3/qapplication.html#applicationDirPath
	{
		char buf[FILEPATH_BUFSIZ];
		int bufsiz = FILEPATH_BUFSIZ;
		size_t size = readlink("/proc/self/exe", buf, bufsiz);
		knh_cwb_write(ctx, cwb, B2(buf, size));
		knh_DictMap_set_(ctx, sys->props,
			new_T("konoha.bin.path"), UPCAST(knh_cwb_newString(ctx, cwb)));
		if(homepath == NULL) {
			size = readlink("/proc/self/exe", buf, bufsiz);
			knh_cwb_write(ctx, cwb, B2(buf, size));
			knh_cwb_parentpath(ctx, cwb, NULL);
			knh_cwb_parentpath(ctx, cwb, NULL);
			shome = knh_cwb_newString(ctx, cwb);
			home = S_tobytes(shome);
			knh_DictMap_set_(ctx, sys->props, new_T("konoha.path"), UPCAST(shome));
		}
	}
#elif defined(KONOHA_ON_MACOSX)
	{
		char buf[PATH_MAX];
		char *s = (char*)_dyld_get_image_name(0);
		s = realpath(s, buf);
		knh_cwb_write(ctx, cwb, B(buf));
		knh_DictMap_set_(ctx, sys->props,
			new_T("konoha.bin.path"), UPCAST(knh_cwb_newString(ctx, cwb)));
		if(homepath == NULL) {
			knh_cwb_write(ctx, cwb, B(s));
			knh_cwb_parentpath(ctx, cwb, NULL);
			knh_cwb_parentpath(ctx, cwb, NULL);
			shome = knh_cwb_newString(ctx, cwb);
			home = S_tobytes(shome);
			knh_DictMap_set_(ctx, sys->props, new_T("konoha.path"), UPCAST(shome));
		}
		//free(s);
	}
#else
	home = STEXT("/konoha");
	knh_DictMap_set_(ctx, sys->props, new_T("konoha.path"), UPCAST(new_T("/konoha")));
#endif
	KNH_ASSERT(home. buf != NULL);
	/* $konoha.package.path {$konoha.path}/package */
	knh_cwb_clear(cwb, 0);
	knh_cwb_write(ctx, cwb, home);
	knh_cwb_write(ctx, cwb, STEXT("/package/" LIBK_VERSION));
	knh_DictMap_set_(ctx, sys->props,
		new_T("konoha.package.path"), UPCAST(knh_cwb_newString(ctx, cwb)));

	/* $konoha.script.path {$konoha.path}/script */
	knh_cwb_write(ctx, cwb, home);
	knh_cwb_write(ctx, cwb, STEXT("/script/" LIBK_VERSION));
	knh_DictMap_set_(ctx, sys->props, new_T("konoha.tool.path"), UPCAST(knh_cwb_newString(ctx, cwb)));

	homepath = knh_getenv("HOME");
	if(homepath != NULL) {
		/* $user.path */
		knh_cwb_write(ctx, cwb, B(homepath));
		knh_cwb_putc(ctx, cwb, '/');
		knh_cwb_write(ctx, cwb, STEXT(KONOHA_FOLDER));
		shome = knh_cwb_newString(ctx, cwb);
		home = S_tobytes(shome);
		knh_DictMap_set_(ctx, sys->props, new_T("user.path"), UPCAST(shome));
		/* $konoha.temp.path ${user.path}/temp */
		knh_cwb_write(ctx, cwb, home);
		knh_cwb_write(ctx, cwb, STEXT("/temp"));
		knh_DictMap_set_(ctx, sys->props, new_T("konoha.temp.path"), UPCAST(knh_cwb_newString(ctx, cwb)));

		/* $user.package.path ${user.path}/package */
		knh_cwb_write(ctx, cwb, home);
		knh_cwb_write(ctx, cwb, STEXT("/package/" LIBK_VERSION));
		knh_DictMap_set_(ctx, sys->props, new_T("user.package.path"), UPCAST(knh_cwb_newString(ctx, cwb)));

		/* $user.script.path ${user.path}/script */
		knh_cwb_write(ctx, cwb, home);
		knh_cwb_write(ctx, cwb, STEXT("/script/" LIBK_VERSION));
		knh_DictMap_set_(ctx, sys->props, new_T("user.tool.path"), UPCAST(knh_cwb_newString(ctx, cwb)));
	}
	else {
#if defined(KONOHA_ON_WINDOWS)
		knh_cwb_write(ctx, cwb, STEXT("\\Temp"));
#else
		knh_cwb_write(ctx, cwb, STEXT("/tmp"));
#endif
		knh_DictMap_set_(ctx, sys->props, new_T("konoha.temp.path"), UPCAST(knh_cwb_newString(ctx, cwb)));
	}
}

/* ======================================================================== */
/* [dlopen] */

void *knh_cwb_dlopen(Ctx *ctx, knh_cwb_t *cwb, int isPERROR)
{
	const char *file;
	void *hdlr = NULL;
	if(!knh_bytes_endsWith(knh_cwb_tobytes(cwb), STEXT(K_OSDLLEXT))) {
		knh_Bytes_concatZero(ctx, cwb->ba);
		knh_Bytes_write(ctx, cwb->ba, STEXT(K_OSDLLEXT));
	}
	file = knh_cwb_ospath(ctx, cwb);
#if defined(K_USING_WINDOWS)
	hdlr = (void*)LoadLibraryA((LPCTSTR)file);
	if(isPERROR && hdlr == NULL) {

	}
#elif defined(K_USING_POSIX)
	hdlr = dlopen(file, RTLD_LAZY);
	if(isPERROR && hdlr == NULL) {
		KNH_SYSLOG(ctx, LOG_ERR, K_PERROR_LIBNAME, "dlopen", "%s", dlerror());
	}
#elif defined(K_USING_BTRON)
//	TC buff_tc[FILEPATH_BUFSIZ];
//	LINK lnk;
//	W err;
//	eucstotcs(buff_tc, file);
//	err = b_get_lnk(buff_tc, &lnk, F_NORM);
//	if (err < 0) {
//		return NULL;
//	}
//	err = b_dlopen(&lnk, DL_LAZY);
//	if (err <= 0) {
//		return NULL;
//	}
//	// Be careful that BTRON dlopen handles are of type W, not void*!
//	return (void*)err;
#else

#endif
	if(hdlr != NULL) {
		KNH_SYSLOG(ctx, LOG_NOTICE, "OpenDynamicLink", "file=%B, hdlr=%p", knh_cwb_tobytes(cwb), hdlr);
	}
	return hdlr;
}

/* ------------------------------------------------------------------------ */

void *knh_dlsym(Ctx *ctx, void* hdr, const char* symbol, int isPERROR)
{
#if defined(K_USING_WINDOWS)
	void *p = GetProcAddress((HMODULE)hdr, (LPCSTR)symbol);
	if(p == NULL && isPERROR) {
		KNH_SYSLOG(ctx, LOG_ERR, K_PERROR_LIBNAME, "GetProcAddress", "func=%s, errormsg='not found'", symbol);
	}
	return p;
#elif defined(K_USING_POSIX)
	void *p = dlsym(hdr, symbol);
	if(p == NULL && isPERROR) {
		KNH_SYSLOG(ctx, LOG_ERR, K_PERROR_LIBNAME, "dlsym", "func=%s, errormsg='%s'", symbol, dlerror());
	}
	return p;
#elif defined(K_USING_BTRON)
	UW val = NULL;
	// FIXME
	// If you call b_dlsym, it makes bad.
	//W err = b_dlsym((W)hdr, symbol, &val);
	W err = -1;
	if (err < 0) {
		return NULL;
	}
	return (void*)val;
#else
	return NULL;
#endif
}

/* ------------------------------------------------------------------------ */

int knh_dlclose(Ctx *ctx, void* hdr)
{
#if defined(K_USING_WINDOWS)
    return (int)FreeLibrary((HMODULE)hdr);
#elif defined(K_USING_POSIX)
    return dlclose(hdr);
#elif defined(K_USING_BTRON)
    return b_dlclose((W)hdr);
#else
    return 0;
#endif
}

/* ======================================================================== */
/* [charset] */

#ifdef KONOHA_ON_WINDOWS

#define HAVE_LOCALCHARSET_H 1

static char *locale_charset(void)
{
	static char codepage[64];
	knh_snprintf(codepage, sizeof(codepage), "CP%d", (int)GetACP());
	return codepage;
}

#endif

/* ------------------------------------------------------------------------ */

const char *knh_getSystemEncoding(void)
{
#if defined(KONOHA_OS_ENCODING)
	return KONOHA_OS_ENCODING;
#elif defined(HAVE_LOCALCHARSET_H)
	return (char*)locale_charset();
#else
//	char *enc = knh_getenv("LC_CTYPE");
//	if(enc != NULL) {
//		return (char*)enc;
//	}
	return K_ENCODING;
#endif
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
