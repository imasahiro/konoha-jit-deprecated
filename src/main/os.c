/****************************************************************************
 * KONOHA COPYRIGHT, LICENSE NOTICE, AND DISCRIMER
 *
 * Copyright (c) 2006-2011, Kimio Kuramitsu <kimio at ynu.ac.jp>
 *           (c) 2008-      Konoha Team konohaken@googlegroups.com
 * All rights reserved.
 *
 * You may choose one of the following two licenses when you use konoha.
 * If you want to use the latter license, please contact us.
 *
 * (1) GNU General Public License 3.0 (with K_UNDER_GPL)
 * (2) Konoha Non-Disclosure License 1.0
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

#ifdef K_USING_POSIX_
#include <unistd.h>
#include <signal.h>
#include <dirent.h>
#include <sys/stat.h>
#include <dlfcn.h>
#include <time.h>
#include <sys/time.h>
#include <sys/param.h>
#include <sys/sysctl.h>
#ifdef K_USING_MACOSX_
#include <mach-o/dyld.h>
#endif
#endif/*K_USING_POSIX_*/

#ifdef K_USING_WINDOWS_
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

#define sfp NULL  /* for NOTE_ */

/* ------------------------------------------------------------------------ */
/* sysinfo */

#ifdef K_USING_BSDSYSCTL_
static size_t sysctl_getint(const char *name, int defdata)
{
	int data = defdata;
	size_t len = sizeof(int);
	if(sysctlbyname(name, &data, &len, NULL, 0) == -1) {
		//fprintf(stderr, "sysctl: %s=FAILED\n", name);
		return (size_t)data;
	}
	//fprintf(stderr, "sysctl: %s=%d\n", name, data);
	return (size_t)data;
}

static const char *sysctl_getstring(const char *name, char *buffer, size_t len)
{
	size_t len2 = 0;
	if(sysctlbyname(name, NULL, &len2, NULL, 0) == -1) {
		//fprintf(stderr, "sysctl: %s=FAILED, len2=%d<%d\n", name, len2, len);
		return NULL;
	}
	if(len2 <= len) {
		sysctlbyname(name, buffer, &len2, NULL, 0);
		//fprintf(stderr, "sysctl: %s=%s\n", name, buffer);
	}
	else {
		//fprintf(stderr, "sysctl: %s=FAILED, len2=%d<%d\n", name, len2, len);
	}
	return (const char*)buffer;
}

static void setsysinfo_sysctl(knh_sysinfo_t *sysinfo)
{
	static char cpu_brand[128] = {0};
	static char hw_machine[80] = {0};
	static char hw_model[128] = {0};
	static char kern_ostype[40] = {0};
	static char kern_version[40] = {0};
	static char kern_desc[128] = {0};
	sysinfo->cpu_desc = sysctl_getstring("machdep.cpu.brand_string", cpu_brand, sizeof(cpu_brand));
	sysinfo->hw_machine = sysctl_getstring("hw.machine", hw_machine, sizeof(hw_machine));
	sysinfo->hw_model = sysctl_getstring("hw.model", hw_model, sizeof(hw_model));
	sysinfo->hw_ncpu = sysctl_getint("hw.ncpu", 1);
	sysinfo->hw_availcpu = sysctl_getint("hw.availcpu", sysinfo->hw_ncpu);
	sysinfo->hw_physmem = sysctl_getint("hw.physmem", 0);
	sysinfo->hw_usermem = sysctl_getint("hw.usermem", 0);
	sysinfo->hw_cpufrequency = sysctl_getint("hw.cpufrequency", 0);
	sysinfo->hw_l2cachesize = sysctl_getint("hw.l2cachesize", 0);
	sysinfo->kern_ostype = sysctl_getstring("kern.ostype", kern_ostype, sizeof(kern_ostype));
	sysinfo->kern_version = sysctl_getstring("kern.osrelease", kern_version, sizeof(kern_version));
	sysinfo->kern_desc = sysctl_getstring("kern.version", kern_desc, sizeof(kern_desc));
}
#endif/*#ifdef K_USING_BSDSYSCTL*/

static char *append_string(char *p, const char *str, size_t len)
{
	size_t i;
	for(i = 0; i < len; i++) p[i] = str[i];
	p[len-1] = ' ';
	return p + (len);
}

#define APPEND_OPTION(p, STR)  p = append_string(p, STR, sizeof(STR));

static void setkonohainfo(knh_sysinfo_t *sysinfo)
{
	static char options[512] = {0};
	static char date[40] = {0};
	char *p = options;
	sysinfo->konoha_type = K_PROGNAME;
	sysinfo->konoha_codename = K_CODENAME;
	sysinfo->konoha_version = K_VERSION;
	sysinfo->konoha_disttype = K_DISTTYPE;
	sysinfo->konoha_disturl  = K_DISTURL;
	sysinfo->konoha_major_version = K_MAJOR_VERSION;
	sysinfo->konoha_minor_version = K_MINOR_VERSION;
	sysinfo->konoha_patch_version = K_PATCH_VERSION;
#if defined(K_PATCH_LEVEL)
	sysinfo->konoha_patch_level = K_PATCH_LEVEL;
#elif defined(K_REVISION)
	sysinfo->konoha_patch_level = K_REVISION;
#endif
	sysinfo->konoha_cc = CC_TYPE;
	knh_snprintf(date, sizeof(date), "%s %s", __DATE__, __TIME__);
	sysinfo->konoha_builtdate = date;
	sysinfo->konoha_config = CC_PLATFORM;
	sysinfo->konoha_disttype = K_DISTTYPE;
	sysinfo->konoha_disturl = K_DISTURL;
	sysinfo->konoha_systembits = (sizeof(void*) * 8);
	sysinfo->konoha_options = (const char*)options;
	/* THESE OPTIONS MUST BE ORDERED ALPHABETICALLY */
#if defined(K_USING_FASTEST)
	APPEND_OPTION(p, "fastest");
#endif
#if defined(K_USING_ICONV)
	APPEND_OPTION(p, "iconv");
#endif
#if defined(K_USING_INT32)
	APPEND_OPTION(p, "int32");
#endif
//#if defined(K_USING_PREFETCH)
//	APPEND_OPTION(p, "prefetch");
//#endif
//#if defined(K_USING_READLINE)
//	APPEND_OPTION(p, "readline");
//#endif
#if defined(K_USING_RCGC)
	APPEND_OPTION(p, "rcgc");
#else
	APPEND_OPTION(p, "msgc");
#endif
#if defined(K_USING_THCODE_)
	APPEND_OPTION(p, "thcode");
#else
	APPEND_OPTION(p, "switch");
#endif
#if defined(K_USING_SQLITE3)
	APPEND_OPTION(p, "sqlite3");
#endif
#if defined(K_USING_SYSLOG)
	APPEND_OPTION(p, "syslog");
#endif
#if defined(K_USING_THREAD)
	APPEND_OPTION(p, "thread");
#endif
#if defined(K_CONFIG_OS)
	if(sysinfo->kern_ostype == NULL) sysinfo->kern_ostype = K_CONFIG_OS;
#endif
}

static const knh_sysinfo_t *sysinfo_ = NULL;

const knh_sysinfo_t* knh_getsysinfo(void)
{
	if(sysinfo_ == NULL) {
		static knh_sysinfo_t sysinfo = {0};
#ifdef K_USING_BSDSYSCTL_
		setsysinfo_sysctl(&sysinfo);
#endif
		setkonohainfo(&sysinfo);
		sysinfo_ = &sysinfo;
	}
	return sysinfo_;
}

static void dump_int(CTX ctx, knh_OutputStream_t *w, const char *name, knh_intptr_t data, int isALL)
{
	if(data == -1) {
		if(!isALL) return;
		if(ctx == NULL) {
			fprintf(stderr, "%s: UNDEFINED\n", name);
		}
		else {
			knh_printf(ctx, w, "%s: UNDEFINED\n", name);
		}
	}
	else {
		if(ctx == NULL) {
			fprintf(stderr, "%s: %ld\n", name, (long)data);
		}
		else {
			knh_printf(ctx, w, "%s: %d\n", name, data);
		}
	}
}

static void dump_string(CTX ctx, knh_OutputStream_t *w, const char *name, const char *data, int isALL)
{
	if(data == NULL) {
		if(!isALL) return;
		data = "";
	}
	if(ctx == NULL) {
		fprintf(stderr, "%s: %s\n", name, data);
	}
	else {
		knh_printf(ctx, w, "%s: %s\n", name, data);
	}
}

void dump_sysinfo(CTX ctx, knh_OutputStream_t *w, int isALL)
{
	const knh_sysinfo_t *sysinfo = knh_getsysinfo();
	dump_string(ctx, w, "konoha.type", sysinfo->konoha_type, isALL);
	dump_string(ctx, w, "konoha.version", sysinfo->konoha_version, isALL);
	dump_int(ctx, w, "konoha.patch.level", sysinfo->konoha_patch_level, isALL);
	dump_string(ctx, w, "konoha.options", sysinfo->konoha_options, isALL);
	dump_int(ctx, w, "konoha.systembits", sysinfo->konoha_systembits, isALL);
	dump_string(ctx, w, "konoha.config", sysinfo->konoha_config, isALL);
	dump_string(ctx, w, "konoha.built.cc", sysinfo->konoha_cc, isALL);
	dump_string(ctx, w, "konoha.built.date", sysinfo->konoha_builtdate, isALL);
	dump_string(ctx, w, "konoha.dist_type", sysinfo->konoha_disttype, isALL);
	dump_string(ctx, w, "konoha.dist_url", sysinfo->konoha_disturl, isALL);
	dump_string(ctx, w, "kern.ostype", sysinfo->kern_ostype, isALL);
	dump_string(ctx, w, "kern.version", sysinfo->kern_version, isALL);
	dump_string(ctx, w, "kern.desc", sysinfo->kern_desc, isALL);
	dump_string(ctx, w, "hw.machine", sysinfo->hw_machine, isALL);
	dump_string(ctx, w, "hw.model", sysinfo->hw_model, isALL);
	dump_string(ctx, w, "cpu.desc", sysinfo->cpu_desc, isALL);
	dump_int(ctx, w, "hw.ncpu", sysinfo->hw_ncpu, isALL);
	dump_int(ctx, w, "hw.availcpu", sysinfo->hw_availcpu, isALL);
	dump_int(ctx, w, "hw.physmem", sysinfo->hw_physmem, isALL);
	dump_int(ctx, w, "hw.usermem", sysinfo->hw_usermem, isALL);
	dump_int(ctx, w, "hw.cpufrequency", sysinfo->hw_cpufrequency, isALL);
	dump_int(ctx, w, "hw.l2cachesize", sysinfo->hw_l2cachesize, isALL);
}

/* ------------------------------------------------------------------------ */

void knh_buff_addpath(CTX ctx, knh_Bytes_t *ba, size_t pos, int needsSEP, knh_bytes_t t)
{
	size_t i;
	if(needsSEP) {
		knh_bytes_t b = {{ba->bu.text}, pos};
		if(!(b.len > 0 && b.buf[b.len-1] == '/')) {
			knh_Bytes_putc(ctx, ba, '/');
		}
	}
	for(i = 0; i < t.len; i++) {
		int ch = t.ubuf[i];
		knh_Bytes_putc(ctx, ba, ch);
	}
}

void knh_buff_addospath(CTX ctx, knh_Bytes_t *ba, size_t pos, int needsSEP, knh_bytes_t t)
{
	size_t i;
	if(needsSEP) {
		knh_bytes_t b = {{ba->bu.text}, pos};
		if(!(b.len > 0 && b.buf[b.len-1] == K_SEP)) {
			knh_Bytes_putc(ctx, ba, K_SEP);
		}
	}
	for(i = 0; i < t.len; i++) {
		int ch = t.ubuf[i];
		if(ch == '\\' || ch == '/') ch = K_SEP;
		if(ch < 127) {
			knh_Bytes_putc(ctx, ba, ch);
			continue;
		}
		knh_Bytes_putc(ctx, ba, ch);
	}
}

void knh_buff_trim(CTX ctx, knh_Bytes_t *ba, size_t pos, int ch)
{
	knh_uchar_t *ubuf = ba->bu.ubuf + pos;
	long i, len = BA_size(ba) - pos;
	if(ch == '/' && ch != K_SEP) ch = K_SEP;
	for(i = len - 1; i >= 0 ; i--) {
		if(ubuf[i] == ch) {
			knh_Bytes_clear(ba, pos + i);
			return;
		}
	}
}

knh_String_t *knh_buff_newRealPath(CTX ctx, knh_Bytes_t *ba, size_t pos)
{
	char buf[K_PATHMAX], *path = (char*)knh_Bytes_ensureZero(ctx, ba) + pos;
#if defined(K_USING_WINDOWS_)
	char *ptr = _fullpath(buf, path, K_PATHMAX);
#elif defined(K_USING_POSIX_)
	char *ptr = realpath(path, buf);
#else
	char *ptr = NULL;
	KNH_TODO("realpath in your new environment");
#endif
	knh_String_t *s = new_String(ctx, (const char*)ptr);
	if(ptr != buf && ptr != NULL) {
		free(ptr);
	}
	return s;
}

KNHAPI2(knh_bool_t) knh_String_ospath(CTX ctx, knh_String_t *s, knh_NameSpace_t *ns, char *buf, size_t bufsiz)
{
	knh_bool_t res = 0;
	knh_index_t loc = knh_bytes_index(S_tobytes(s), ':');
	if(loc == -1) {
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
//		knh_buff_addospath(ctx, cwb->ba, cwb->pos, 0, S_tobytes(ns->rpath));
		knh_buff_addospath(ctx, cwb->ba, cwb->pos, 0, S_tobytes(s));
		const char *p = knh_cwb_tochar(ctx, cwb);
		size_t len = knh_strlen(p) + 1;
		if(len < bufsiz) {
			knh_memcpy(buf, p, len); res = 1;
		}
		knh_cwb_close(cwb);
	}
	else {
		knh_Link_t *lnk = knh_NameSpace_getLinkNULL(ctx, ns, S_tobytes(s));
		if(lnk != NULL && knh_Link_hasType(ctx, lnk, CLASS_Bytes)) {
			knh_Bytes_t *ba = (knh_Bytes_t*)knh_Link_newObjectNULL(ctx, lnk, ns, s, CLASS_Bytes);
			if(ba != NULL) {
				KNH_SETv(ctx, ctx->esp[0].o, ba);  //TOGC
				knh_Bytes_ensureZero(ctx, ba);
				if(BA_size(ba) + 1 < bufsiz) {
					knh_memcpy(buf, ba->bu.buf, BA_size(ba) + 1); res = 1;
				}
			}
		}
	}
	if(res == 1) {
		DBG_P("ospath='%s'", buf);
	}
	else {
		buf[0] = 0;
	}
	return res;
}

knh_bool_t knh_exists(CTX ctx, const char *fname)
{
	knh_bool_t res = 0;
	if(fname == NULL || fname[0] == 0) return 0;
#if defined(K_USING_WINDOWS_)
	DWORD attr = GetFileAttributesA(fname);
	res = (attr != -1);
#elif defined(K_USING_POSIX_)
	struct stat buf;
	res = (stat(fname, &buf) != -1);
#else
#endif
	if(res == 0) {
		DBG_P("'%s' NOTFOUND", fname);
	}
	return res;
}

knh_bool_t knh_isfile(CTX ctx, const char *phname)
{
	knh_bool_t res = 1;
	if(phname[0] == 0) return 0;
#if defined(K_USING_WINDOWS_)
	DWORD attr = GetFileAttributesA(phname);
	if(attr == -1 || (attr & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) res = 0;
#elif defined(K_USING_POSIX_)
	struct stat buf;
	if(stat(phname, &buf) == -1) res = 0;
	else res = S_ISREG(buf.st_mode);
#else
	FILE* in = fopen(phname,"r");
	if(in == NULL)  res = 0;
	else fclose(in);
#endif
	if(res == 0) {
		DBG_P("isfile='%s' NOTFOUND", phname);
	}
	return res;
}

knh_bool_t knh_Bytes_isfile(CTX ctx, knh_Bytes_t *ba, size_t pos)
{
	return knh_isfile(ctx, knh_Bytes_ensureZero(ctx, ba) + pos);
}

/* ------------------------------------------------------------------------ */

//knh_bool_t knh_path_isfile(CTX ctx, knh_path_t *ph)
//{
//	knh_bool_t res = 1;
//	const char *phname = P_text(ph) + ph->pbody;
//	if(phname[0] == 0) return 0;
//#if defined(K_USING_WINDOWS_)
//	DWORD attr = GetFileAttributesA(phname);
//	if(attr == -1 || (attr & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) res = 0;
//#elif defined(K_USING_POSIX_)
//	struct stat buf;
//	if(stat(phname, &buf) == -1) res = 0;
//	else res = S_ISREG(buf.st_mode);
//#else
//	FILE* in = fopen(phname,"r");
//	if(in == NULL)  res = 0;
//	else fclose(in);
//#endif
//	if(res == 0) {
//		DBG_P("isfile='%s' NOTFOUND", P_text(ph));
//	}
//	return res;
//}

knh_bool_t knh_isdir(CTX ctx, const char *pname)
{
#if defined(K_USING_WINDOWS_)
	DWORD attr = GetFileAttributesA(pname);
	if(attr == -1) return 0;
	return ((attr & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY);
#elif defined(K_USING_POSIX_)
	struct stat buf;
	if(stat(pname, &buf) == -1) return 0;
	return S_ISDIR(buf.st_mode);
#else
	// avoid "unused variable" warning unused variable
	(void)phname;
	return 0;
#endif
}

/* ------------------------------------------------------------------------ */

static knh_bool_t knh_mkdir(CTX ctx, const char *pname)
{
	DBG_P("path='%s'", pname);
#if defined(K_USING_WINDOWS_)
	return (CreateDirectory(pname, NULL) != 0);
#elif defined(K_USING_POSIX_)
	return (mkdir(pname, 0777) != -1);
#else
	return 0;
#endif
}

knh_bool_t knh_Bytes_mkdir(CTX ctx, knh_Bytes_t *ba, size_t pos)
{
	knh_uchar_t *ubuf = ba->bu.ubuf;
	size_t i, len = BA_size(ba);
	for(i = pos + 1; i < len; i++) {
		int ch = ubuf[i];
		if(ch == K_SEP) {
			int res = 0;
			ubuf[i] = 0;
			if(knh_isdir(ctx, (const char*)ubuf + pos)) {
				res = knh_mkdir(ctx, (const char*)ubuf + pos);
			}
			ubuf[i] = ch;
			if(res == -1) return 0;
		}
	}
	return knh_mkdir(ctx, (const char*)ubuf + pos);
}

/* Linux, MacOSX */
// $konoha.home.path /usr/local/konoha
// $konoha.bin.path  /usr/local/bin/konoha
// $konoha.package.path {$konoha.home.path}/package
// $konoha.script.path  {$konoha.home.path}/script

#ifdef K_USING_DEBUG
#define K_KONOHAHOME "/usr/local/konoha"
#endif

#define SETPROP(K, V)  knh_DictMap_set_(ctx, sysprops, new_T(K), UPCAST(V))

void knh_System_initPath(CTX ctx, knh_System_t *o)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_DictMap_t *sysprops = DP(o)->props;
	knh_bytes_t home = {{NULL}, 0}, user = {{NULL}, 0};

	// current working directory
	knh_buff_addospath(ctx, cwb->ba, cwb->pos, 0, STEXT("."));
	KNH_SETv(ctx, ctx->share->rootns->rpath, knh_buff_newRealPath(ctx, cwb->ba, cwb->pos));

	home.text = (const char*)knh_getenv("KONOHAHOME");
#if defined(K_KONOHAHOME)
	if(home.text == NULL) {
		home.text = K_KONOHAHOME;
	}
#endif
	if(home.text != NULL) {
		home.len = knh_strlen(home.text);
		SETPROP("konoha.home.path", new_T(home.text));
	}
#if defined(K_USING_WINDOWS_)
	{
		char buf[K_PATHMAX];
		int bufsiz = K_PATHMAX;
		HMODULE h = LoadLibrary(NULL);
		GetModuleFileNameA(h, buf, bufsiz);
		knh_cwb_clear(cwb, 0);
		knh_buff_addospath(ctx, cwb->ba, cwb->pos, 0, B(buf));
		SETPROP("konoha.bin.path", knh_buff_newRealPath(ctx, cwb->ba, cwb->pos));
		if(home.text == NULL) {
			knh_String_t *s;
			knh_cwb_clear(cwb, 0);
			knh_buff_addpath(ctx, cwb->ba, cwb->pos, 0, new_bytes2(buf, size));
			knh_buff_trim(ctx, cwb->ba, cwb->pos, '\\');
			knh_buff_addpath(ctx, cwb->ba, cwb->pos, 1/*isSep*/, STEXT("konoha"));
			s = knh_cwb_newString(ctx, cwb);
			SETPROP("konoha.home.path", s);
			home = S_tobytes(s);
		}
	}
#elif defined(K_USING_LINUX_)
	// @url(http://shinh.skr.jp/binary/b2con.html)
	// http://doc.trolltech.com/3.3/qapplication.html#applicationDirPath
	{
		char buf[K_PATHMAX];
		int bufsiz = K_PATHMAX;
		size_t size = readlink("/proc/self/exe", buf, bufsiz);
		knh_cwb_clear2(cwb, 0);
		knh_buff_addospath(ctx, cwb->ba, cwb->pos, 0, new_bytes2(buf, size));
		SETPROP("konoha.bin.path", knh_buff_newRealPath(ctx, cwb->ba, cwb->pos));
		if(home.text == NULL) {
			knh_String_t *s;
			knh_cwb_clear2(cwb, 0);
			knh_buff_addpath(ctx, cwb->ba, cwb->pos, 0, new_bytes2(buf, size));
			knh_buff_trim(ctx, cwb->ba, cwb->pos, '/');
			knh_buff_trim(ctx, cwb->ba, cwb->pos, '/');
			knh_buff_addpath(ctx, cwb->ba, cwb->pos, 1/*isSep*/, STEXT("konoha"));
			s = knh_cwb_newString(ctx, cwb);
			SETPROP("konoha.home.path", s);
			home = S_tobytes(s);
		}
	}
#elif defined(K_USING_MACOSX_)
	knh_cwb_clear2(cwb, 0);
	knh_buff_addospath(ctx, cwb->ba, cwb->pos, 0, B(_dyld_get_image_name(0)));
	SETPROP("konoha.bin.path", knh_buff_newRealPath(ctx, cwb->ba, cwb->pos));
	if(home.text == NULL) {
		knh_cwb_clear2(cwb, 0);
		knh_buff_addpath(ctx, cwb->ba, cwb->pos, 0, B(_dyld_get_image_name(0)));
		knh_buff_trim(ctx, cwb->ba, cwb->pos, '/');
		knh_buff_trim(ctx, cwb->ba, cwb->pos, '/');
		knh_buff_addpath(ctx, cwb->ba, cwb->pos, 1/*isSep*/, STEXT("konoha"));
		knh_String_t *s = knh_cwb_newString(ctx, cwb);
		SETPROP("konoha.home.path", s);
		home = S_tobytes(s);
	}
#else
	home = STEXT("/opt/konoha");
	SETPROP("konoha.home.path", new_T("/opt/konoha"));
#endif
	DBG_ASSERT(home.utext != NULL);

	/* $konoha.package.path {$konoha.home.path}/package */
	knh_cwb_clear2(cwb, 0);
	knh_buff_addpath(ctx, cwb->ba, cwb->pos, 0, home);
	knh_buff_addpath(ctx, cwb->ba, cwb->pos, 1/*sep*/, STEXT("package"));
	knh_buff_addpath(ctx, cwb->ba, cwb->pos, 1/*sep*/, STEXT(LIBK_VERSION));
	SETPROP("konoha.package.path", knh_cwb_newString(ctx, cwb));

	/* $konoha.script.path {$konoha.home.path}/script */
	knh_buff_addpath(ctx, cwb->ba, cwb->pos, 0, home);
	knh_buff_addpath(ctx, cwb->ba, cwb->pos, 1/*sep*/, STEXT("script"));
	knh_buff_addpath(ctx, cwb->ba, cwb->pos, 1/*sep*/, STEXT(LIBK_VERSION));
	SETPROP("konoha.script.path", knh_cwb_newString(ctx, cwb));

#if defined(K_USING_WINDOWS_)
	user.text = knh_getenv("USERPROFILE");
#else
	user.text = knh_getenv("HOME");
#endif
	if(user.text != NULL) {
		/* $user.path */
		user.len = knh_strlen(user.text);
		knh_buff_addpath(ctx, cwb->ba, cwb->pos, 0, user);
		knh_buff_addpath(ctx, cwb->ba, cwb->pos, 1, STEXT(K_KONOHAFOLDER));
		SETPROP("user.path", knh_cwb_newString(ctx, cwb));
		knh_buff_addospath(ctx, cwb->ba, cwb->pos, 0, user);
		knh_buff_addospath(ctx, cwb->ba, cwb->pos, 1, STEXT(K_KONOHAFOLDER));
		knh_Bytes_mkdir(ctx, cwb->ba, cwb->pos);
		knh_cwb_clear2(cwb, 0);
		knh_buff_addospath(ctx, cwb->ba, cwb->pos, 0, user);
		knh_buff_addospath(ctx, cwb->ba, cwb->pos, 1/*sep*/, STEXT(K_KONOHAFOLDER));
		knh_buff_addospath(ctx, cwb->ba, cwb->pos, 1/*sep*/, STEXT("package"));
		knh_buff_addospath(ctx, cwb->ba, cwb->pos, 1/*sep*/, STEXT(LIBK_VERSION));
		SETPROP("user.package.path", knh_cwb_newString(ctx, cwb));
	}
	knh_cwb_close(cwb);
}

/* ------------------------------------------------------------------------ */
/* [dlopen] */

void *knh_dlopen(CTX ctx, const char* path)
{
	const char *func = __FUNCTION__;
	void *handler = NULL;
#if defined(K_USING_WINDOWS_)
	func = "LoadLibrary";
	handler = (void*)LoadLibraryA((LPCTSTR)path);
#elif defined(K_USING_POSIX_)
	func = "dlopen";
	handler = dlopen(path, RTLD_LAZY);
#else

#endif
	if(handler == NULL) {
		LOGDATA = {sDATA("path", path)};
		NOTE_Failed(func);
	}
	else {
		LOGDATA = {sDATA("path", path), pDATA("handler", handler)};
		NOTE_OK(func);
	}
	return handler;
}

void *knh_dlsym(CTX ctx, void* handler, const char* symbol, int isTest)
{
	const char *func = __FUNCTION__, *emsg = NULL;
	void *p = NULL;
#if defined(K_USING_WINDOWS_)
	func = "GetProcAddress";
	p = GetProcAddress((HMODULE)handler, (LPCSTR)symbol);
	return p;
#elif defined(K_USING_POSIX_)
	func = "dlsym";
	p = dlsym(handler, symbol);
	if(p == NULL) {
		emsg = dlerror();
	}
#else
#endif
	if(!isTest) {
		if(p == NULL) {
			LOGDATA = {pDATA("handler", handler), sDATA("symbol", symbol), LOGMSG(emsg)};
			NOTE_Failed(func);
		}
		else {
			LOGDATA = {pDATA("handler", handler), sDATA("symbol", symbol)};
			NOTE_OK(func);
		}
	}
	return p;
}

int knh_dlclose(CTX ctx, void* handler)
{
#if defined(K_USING_WINDOWS_)
	return (int)FreeLibrary((HMODULE)handler);
#elif defined(K_USING_POSIX_)
	return dlclose(handler);
#else
    return 0;
#endif
}

/* ------------------------------------------------------------------------ */
/* [charset] */

#if defined(K_USING_WINDOWS_)
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
	const char *enc = knh_getenv("KONOHAENC");
	if(enc != NULL) {
		return enc;
	}
#if defined(K_OSENCODING)
	return K_OSENCODING;
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
