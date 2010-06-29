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

#ifndef KONOHA_DEPS_H_
#define KONOHA_DEPS_H_

/* ======================================================================== */
/* [UNIX] */

#ifdef KONOHA_ON_UNKNOWN
	#define K_USING_STDC  1
	#define KONOHA_OS_ENCODING "UTF-8"
	#define KONOHA_OS_LINEFEED "\n"
	#define K_OSDLLEXT   ".so"
#endif

#ifdef KONOHA_ON_BSD
	#define K_USING_POSIX    1
	#define KONOHA_OS_ENCODING "UTF-8"
	#define KONOHA_OS_LINEFEED "\n"
	#define K_OSDLLEXT ".so"
#endif

#ifdef KONOHA_ON_UNKNOWN_POSIX
	#define K_USING_POSIX    1
	#define KONOHA_OS_ENCODING "UTF-8"
	#define KONOHA_OS_LINEFEED "\n"
	#define K_OSDLLEXT ".so"
#endif

#ifdef KONOHA_ON_UNKNOWN_UNIX
	#define K_USING_POSIX    1
	#define K_USING_UNIX     1
	#define KONOHA_OS_ENCODING "UTF-8"
	#define KONOHA_OS_LINEFEED "\n"
	#define K_OSDLLEXT ".so"
#endif


#ifdef KONOHA_ON_LINUX
#define     K_USING_POSIX 1
#define     K_USING_UNIX  1
#define     K_USING_SOCKET 1
#define     K_USING_THREAD   1
#define     K_USING_PTHREAD  1
#define     KONOHA_OS_ENCODING "UTF-8"
#define     K_USING_KONOHAGET 1
#ifdef HAVE_ICONV_H
#define     K_USING_ICONV 1
#endif
//#ifdef HAVE_REGEX_H
#define     K_USING_REGEX 1
//#endif
#define     KONOHA_OS_LINEFEED "\n"
#define     K_OSDLLEXT   ".so"
#endif

#ifdef KONOHA_ON_MACOSX
#define     K_USING_POSIX    1
#define     K_USING_UNIX     1
#define     K_USING_SOCKET   1
#define     K_USING_ICONV    1
#define     K_USING_REGEX    1
#define     KONOHA_OS_ENCODING "UTF-8"
#define     KONOHA_OS_LINEFEED "\n"
#define     K_OSDLLEXT ".dylib"
#endif

#ifdef KONOHA_ON_FREEBSD
#define     K_USING_POSIX    1
#define     KONOHA_OS_ENCODING "UTF-8"
#define     KONOHA_OS_LINEFEED "\n"
#define     K_OSDLLEXT ".so"
#endif

#ifdef KONOHA_ON_UNKNOWN_BSD
#define     K_USING_POSIX    1
#define     KONOHA_OS_ENCODING "UTF-8"
#define     KONOHA_OS_LINEFEED "\n"
#define     K_OSDLLEXT ".so"
#endif

#ifdef KONOHA_ON_UNKNOWN_POSIX
#define     K_USING_POSIX    1
#define     KONOHA_OS_ENCODING "UTF-8"
#define     KONOHA_OS_LINEFEED "\n"
#define     K_OSDLLEXT ".so"
#endif

#ifdef KONOHA_ON_UNKNOWN_UNIX
#define     K_USING_POSIX    1
#define     KONOHA_OS_ENCODING "UTF-8"
#define     KONOHA_OS_LINEFEED "\n"
#define     K_OSDLLEXT ".so"
#endif

/* ------------------------------------------------------------------------ */

#ifdef KONOHA_ON_LKM
#define K_USING_INT32      1
#define K_USING_NOFLOAT    1
#define K_USING_NOFILE     1

#define KNH_EXT_QSORT  1
#define KNH_EXT_SETJMP 1

#define FILEPATH_BUFSIZ     80  /* TO SAVE STACKSIZE */
#define KONOHA_HOMEPATH     "/dev/konoha"   /* meaningless */

#undef K_USING_SOCKET
#undef K_USING_ICONV
#undef K_USING_REGEX
#undef K_USING_SQLITE3
#undef K_USING_KONOHAGET
#undef K_USING_PTHREAD
#undef K_USING_THREAD
#undef K_USING_READLINE

#define KONOHA_OS_ENCODING "UTF-8"
#define KONOHA_OS_LINEFEED "\n"
#define K_OSDLLEXT   ""
#endif

/* ======================================================================== */
/* [WINDOWS] */

#ifdef KONOHA_ON_WINDOWS
#include<windows.h>
#define 	K_USING_WIN32    1
#define 	K_USING_WINDOWS  1
#define     K_USING_MATH     1
#define 	KONOHA_OS_LINEFEED "\r\n"
#define     KNH_CC_EXPORT __declspec(dllexport)
#ifdef KNHAPI_IMPORTS
#define 	KNHAPI(T__)      KNH_CC_EXPORT T__ __cdecl
#define     FASTAPI(T__)     T__
#define 	KNHFASTAPI(T__)  KNH_CC_EXPORT T__
#else
#define 	KNHAPI(T__)      KNH_CC_EXPORT  T__ __cdecl
#define     FASTAPI(T__)     T__
#define 	KNHFASTAPI(T__)  KNH_CC_EXPORT T__ KNH_CC_FASTCALL
#endif
#define 	KNH_EXPORTS(T__)  KNH_CC_EXPORT T__ __cdecl

#define 	K_OSDLLEXT ".dll"
#define		K_FILESEPARATOR '\\'
#define     KONOHA_FOLDER "Konoha"
#endif

/* ======================================================================== */
/* [TEABOARD] */

#ifdef KONOHA_ON_TB
#define 	K_USING_BTRON    1
#define 	K_USING_TKAPI    1
#define 	KONOHA_OS_ENCODING knh_getLocalEncoding
#define 	KONOHA_OS_LINEFEED "\n"
#define 	K_OSDLLEXT ".so"
#define     ALT_GETENV
#undef      K_USING_I18N
#endif

/* ======================================================================== */
/* [KNHAPI] */

#ifndef KNH_CC_EXPORT
#define KNH_CC_EXPORT
#endif

#ifndef KNHAPI
#define KNHAPI(T)         T
#define FASTAPI(T)        T  KNH_CC_FASTCALL
#define KNHFASTAPI(T)     T  KNH_CC_FASTCALL
#define KNH_EXPORTS(T)    T
#endif

#ifndef K_FILESEPARATOR
#define K_FILESEPARATOR  '/'
#endif

#ifndef KONOHA_FOLDER
#define KONOHA_FOLDER ".konoha"
#endif

#ifdef KNH_CC_LABELPTR
#define K_USING_THREADEDCODE 1
#else
#undef K_USING_THREADEDCODE
#endif

/*
#if defined(__x86_64__) && !defined(KNH_KLR_CONVERT)
#define KNH_KLR_CONVERT
#endif
*/

#ifdef K_USING_THREAD
#if !defined(K_USING_PTHREAD) && defined(K_USING_POSIX)
#define K_USING_PTHREAD 1
#endif
#endif

/* ======================================================================== */
/* [CONFIG_H] */

#if !defined(K_USING_ICONV) && defined(HAVE_LIBICONV)
#define 	K_USING_ICONV 1
#endif

#if !defined(K_USING_REGEX) && defined(HAVE_REGCOMP)
#define 	K_USING_REGEX 1
#endif

/* ======================================================================== */

#ifndef K_USING_ICONV
#undef K_USING_I18N
#endif

/* ======================================================================== */
/* [stdlib] */

#ifdef ALT_SNPRINTF
#define knh_snprintf(b,n,fmt, ...)     ALT_snprintf(b,n,fmt, ## __VA_ARGS__)
#else
#ifdef _MSC_VER
#define knh_snprintf(b,n,fmt, ...)     _snprintf_s(b,n, _TRUNCATE, fmt, ## __VA_ARGS__)
#else
#define knh_snprintf(b,n,fmt, ...)     snprintf(b,n,fmt, ## __VA_ARGS__)
#endif
#endif

/* ------------------------------------------------------------------------ */

#define knh_strlen(c)                  strlen(c)
#define knh_memcpy(d, s, n)            memcpy(d,s,n)
#define knh_memmove(d, s, n)           memmove(d, s, n)
#define knh_bzero(s, n)                memset(s,0,n)
#define knh_strcmp(s1, s2)             strcmp(s1,s2)
#define knh_strncmp(s1, s2, n)         strncmp((const char*)(s1),(const char*)(s2),n)
#define knh_strncasecmp(s1, s2, n)     strncasecmp((const char*)(s1),(const char*)(s2),n)

/* ------------------------------------------------------------------------ */

#ifdef ALT_SORT
#define  knh_sort(b,s,w,f)            ALT_sort(b,s,w,f)
#define  knh_qsort(b,s,w,f)           ALT_qsort(b,s,w,f)
#else /* ALT_SORT */

#ifdef KNH_EXT_QSORT
//#define  knh_qsort(b,s,w,f)           knh_sort(b,s,w,f)
#else
#define  knh_qsort(b,s,w,f)           qsort(b,s,w,f)
#endif

void knh_qsort_r (void *const pbase, size_t total_elems, size_t size, 
        void* thunk,int (*cmp)(void* ,const void*,const void*));

//#ifdef KONOHA_ON_MACOSX
//#define  knh_qsort_r(b,s,w,thunk,f)   qsort_r(b,s,w,thunk,f)
//#elif defined(KONOHA_ON_LINUX)
//#define  knh_qsort_r(b,s,w,thunk,f)   qsort_r(b,s,w,f,thunk)
//#elif defined(KONOHA_ON_WINDOWS)
//#define  knh_qsort_r(b,s,w,thunk,f)   qsort_s(b,s,w,f,thunk)
//#else /* FIXME KONOHA_ON_UNKNOWN ?? */
////#define  knh_qsort_r(b,s,w,thunk,f)   qsort_r(b,s,w,f,thunk)
//#endif

#endif /* ALT_SORT */

/* ======================================================================== */

#if defined(K_USING_POSIX) && !defined(KONOHA_ON_LKM)
#include<dlfcn.h>
#endif

/* ------------------------------------------------------------------------ */

#ifdef ALT_GETENV
	#define knh_getenv(n)           ALT_getenv(n)
#else
	#define knh_getenv(n)           getenv(n)
#endif


#ifndef FILEPATH_BUFSIZ
#define FILEPATH_BUFSIZ  256
#endif

/* ======================================================================== */

#endif /*KONOHA_DEPS_H_*/
