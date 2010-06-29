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

/* ======================================================================== */
/* K_PATH_DSPI */

static knh_bytes_t knh_bytes_path(knh_bytes_t t)
{
	knh_index_t loc = knh_bytes_index(t, ':');
	if(loc >= 0) {
		return knh_bytes_last(t, loc + 1);
	}
	return t;
}

static knh_bool_t NOPATH_exists(Ctx *ctx, knh_bytes_t path, knh_intptr_t *id)
{
	return 0;
}

static knh_Object_t* NOPATH_newObjectNULL(Ctx *ctx, knh_class_t cid, knh_String_t *s)
{
	return NULL/*(knh_Object_t*)s*/;
}

static knh_PathDSPI_t NOPATH_DSPI = {
	K_PATH_DSPI, "NOP",
	PATH_INTERNAL, CLASS_Tvoid,
	NOPATH_exists, NOPATH_newObjectNULL,
};

static Object* new_Converter(Ctx *ctx, knh_class_t cid, knh_String_t *path, knh_ConverterDSPI_t *dspi)
{
	knh_Converter_t *c = (knh_Converter_t*)new_Object_init2(ctx, CLASS_Converter, cid, cid);
	if(dspi->open != NULL) {
		c->conv = dspi->open(ctx, S_tobytes(path).text, NULL);
		if(c->conv != NULL) {
			c->dspi = dspi;
		}
	}
	else {
		c->dspi = dspi;
	}
	return (knh_Object_t*)c;
}

static knh_bool_t TO_exists(Ctx *ctx, knh_bytes_t path, knh_intptr_t *id)
{
	knh_ConverterDSPI_t *dspi = knh_getConvTODSPINULL(ctx, knh_bytes_path(path));
	if(dspi != NULL) {
		*id = (knh_intptr_t)dspi;
	}
	return (dspi != NULL);
}

static knh_Object_t* TO_newObjectNULL(Ctx *ctx, knh_class_t cid, knh_String_t *s)
{
	knh_bytes_t path = knh_bytes_path(S_tobytes(s));
	knh_ConverterDSPI_t *dspi = knh_getConvTODSPINULL(ctx, path);
	if(dspi != NULL) {
		if(cid == CLASS_StringConverter && dspi->sconv != NULL) {
			return new_Converter(ctx, cid, s, dspi);
		}
		if(cid == CLASS_StringEncoder && dspi->enc != NULL) {
			return new_Converter(ctx, cid, s, dspi);
		}
		if(cid == CLASS_StringDecoder && dspi->dec != NULL) {
			return new_Converter(ctx, cid, s, dspi);
		}
		if(cid == CLASS_Converter && dspi->conv != NULL) {
			return new_Converter(ctx, cid, s, dspi);
		}
		KNH_SYSLOG(ctx, LOG_WARNING, "ConverterType", "%B does not provide %C", path, cid);
	}
	return NULL;
}

static knh_bool_t FROM_exists(Ctx *ctx, knh_bytes_t path, knh_intptr_t *id)
{
	knh_ConverterDSPI_t *dspi = knh_getConvFROMDSPINULL(ctx, knh_bytes_path(path));
	if(dspi != NULL) {
		*id = (knh_intptr_t)dspi;
	}
	return (dspi != NULL);
}

static knh_Object_t* FROM_newObjectNULL(Ctx *ctx, knh_class_t cid, knh_String_t *s)
{
	knh_bytes_t path = knh_bytes_path(S_tobytes(s));
	knh_ConverterDSPI_t *dspi = knh_getConvFROMDSPINULL(ctx, path);
	if(dspi != NULL) {
		if(cid == CLASS_StringConverter && dspi->sconv != NULL) {
			return new_Converter(ctx, cid, s, dspi);
		}
		if(cid == CLASS_StringEncoder && dspi->enc != NULL) {
			return new_Converter(ctx, cid, s, dspi);
		}
		if(cid == CLASS_StringDecoder && dspi->dec != NULL) {
			return new_Converter(ctx, cid, s, dspi);
		}
		if(cid == CLASS_Converter && dspi->conv != NULL) {
			return new_Converter(ctx, cid, s, dspi);
		}
		KNH_SYSLOG(ctx, LOG_WARNING, "ConverterType", "%B does not provide %C", path, cid);
	}
	return NULL;
}

static knh_PathDSPI_t TO_DSPI = {
	K_PATH_DSPI, "to",
	PATH_CONVERTER, CLASS_Converter,
	TO_exists, TO_newObjectNULL,
};

static knh_PathDSPI_t FROM_DSPI = {
	K_PATH_DSPI, "from",
	PATH_CONVERTER, CLASS_Converter,
	FROM_exists, FROM_newObjectNULL,
};

static knh_bool_t CHARSET_exists(Ctx *ctx, knh_bytes_t path, knh_intptr_t *id)
{
	knh_bytes_t t = knh_bytes_path(path);
	if(ctx->share->iconvDSPI == NULL) {
		((knh_share_t*)ctx->share)->iconvDSPI = knh_getConvTODSPINULL(ctx, STEXT("iconv"));
	}
	if(ctx->share->iconvDSPI != NULL) {
		knh_conv_t *conv = ctx->share->iconvDSPI->open(ctx, (const char*)t.buf, K_ENCODING);
		return (conv != 0);
	}
	return (B_equals(t, K_ENCODING));
}

knh_StringDecoder_t* new_StringDecoderNULL(Ctx *ctx, knh_bytes_t t)
{
	if(B_equals(t, K_ENCODING)) {
		return KNH_TNULL(StringDecoder);
	}
	if(ctx->share->iconvDSPI == NULL) {
		((knh_share_t*)ctx->share)->iconvDSPI = knh_getConvTODSPINULL(ctx, STEXT("iconv"));
	}
	if(ctx->share->iconvDSPI != NULL) {
		const knh_ConverterDSPI_t *dspi = ctx->share->iconvDSPI;
		knh_conv_t *conv = dspi->open(ctx, t.text, K_ENCODING);
		if(conv != NULL) {
			knh_StringDecoder_t *c = new_(StringDecoder);
			c->conv = conv;
			c->dspi = dspi;
			return c;
		}
	}
	return NULL;
}

knh_StringEncoder_t* new_StringEncoderNULL(Ctx *ctx, knh_bytes_t t)
{
	if(B_equals(t, K_ENCODING)) {
		return KNH_TNULL(StringEncoder);
	}
	if(ctx->share->iconvDSPI == NULL) {
		((knh_share_t*)ctx->share)->iconvDSPI = knh_getConvTODSPINULL(ctx, STEXT("iconv"));
	}
	if(ctx->share->iconvDSPI != NULL) {
		const knh_ConverterDSPI_t *dspi = ctx->share->iconvDSPI;
		knh_conv_t *conv = dspi->open(ctx, t.text, K_ENCODING);
		if(conv != NULL) {
			knh_StringEncoder_t *c = new_(StringEncoder);
			c->conv = conv;
			c->dspi = dspi;
			return c;
		}
	}
	return NULL;
}

static knh_Object_t* CHARSET_newObjectNULL(Ctx *ctx, knh_class_t cid, knh_String_t *s)
{
	knh_bytes_t t = knh_bytes_path(S_tobytes(s));
	if(cid == CLASS_StringEncoder) {
		return (knh_Object_t*)new_StringEncoderNULL(ctx, t);
	}
	if(cid == CLASS_StringDecoder) {
		return (knh_Object_t*)new_StringDecoderNULL(ctx, t);
	}
	return NULL;
}

static knh_PathDSPI_t CHARSETPATH_DSPI = {
	K_PATH_DSPI, "charset",
	PATH_INTERNAL, CLASS_StringEncoder,
	CHARSET_exists, CHARSET_newObjectNULL,
};

static knh_bool_t CLASS_exists(Ctx *ctx, knh_bytes_t path, knh_intptr_t *id)
{
	knh_class_t cid = knh_NameSpace_getcid(ctx, knh_getGammaNameSpace(ctx), knh_bytes_path(path));
	if(cid != CLASS_unknown) {
		if(id != NULL) *id = cid;
	}
	return (cid != CLASS_unknown);
}

static knh_Object_t* CLASS_newObjectNULL(Ctx *ctx, knh_class_t cid, knh_String_t *s)
{
	cid = knh_NameSpace_getcid(ctx, knh_getGammaNameSpace(ctx), knh_bytes_path(S_tobytes(s)));
	DBG_ASSERT(cid != CLASS_unknown);
	return UP(new_Type(ctx, cid));
}

static knh_PathDSPI_t CLASSPATH_DSPI = {
	K_PATH_DSPI, "class",
	PATH_INTERNAL, CLASS_Class,
	CLASS_exists, CLASS_newObjectNULL,
};

static knh_bool_t METHOD_exists(Ctx *ctx, knh_bytes_t path, knh_intptr_t *id)
{
	knh_bytes_t t = knh_bytes_path(path);
	knh_index_t idx = knh_bytes_rindex(t, '.');
	if(idx != -1) {
		knh_class_t cid = knh_NameSpace_getcid(ctx, knh_getGammaNameSpace(ctx), knh_bytes_first(t, idx));
		if(cid != CLASS_unknown) {
			knh_methodn_t mn = knh_getmn(ctx, knh_bytes_last(t, idx+1), MN_NONAME);
			knh_Method_t *mtd = knh_getMethodNULL(ctx, cid, mn);
			if(mtd != NULL) {
				if(id != NULL) *id = (knh_intptr_t)mtd;
			}
			return (mtd != NULL);
		}
	}
	return 0;
}

static knh_Object_t* METHOD_newObjectNULL(Ctx *ctx, knh_class_t cid, knh_String_t *s)
{
	if(cid == CLASS_Method) {
		knh_intptr_t id = 0;
		METHOD_exists(ctx, S_tobytes(s), &id);
		return (knh_Object_t*)id;
	}
	return NULL;
}

static knh_PathDSPI_t METHODPATH_DSPI = {
	K_PATH_DSPI, "method",
	PATH_INTERNAL, CLASS_Method,
	METHOD_exists, METHOD_newObjectNULL,
};

static knh_bool_t FILE_exists(Ctx *ctx, knh_bytes_t path, knh_intptr_t *id)
{
	knh_bytes_t n = knh_bytes_path(path);
	knh_cwb_t cwbbuf, *cwb = knh_cwb_openinit(ctx, &cwbbuf, n);
	knh_cwb_ospath(ctx, cwb);
	knh_bool_t res = knh_cwb_isfile(ctx, cwb);
	knh_cwb_close(cwb);
	if(res == 1) {
		if(id != NULL) *id = knh_getURI(ctx, n);
	}
	return res;
}

static knh_Object_t* FILE_newObjectNULL(Ctx *ctx, knh_class_t cid, knh_String_t *s)
{
	if(cid == CLASS_InputStream) {
		knh_InputStream_t *in = ctx->api->new_InputStreamNULL(ctx, s, "r");
		return (knh_Object_t*)in;
	}
	return NULL;
}

static knh_bool_t DIR_exists(Ctx *ctx, knh_bytes_t path, knh_intptr_t *id)
{
	knh_bytes_t n = knh_bytes_path(path);
	knh_cwb_t cwbbuf, *cwb = knh_cwb_openinit(ctx, &cwbbuf, n);
	knh_cwb_ospath(ctx, cwb);
	knh_bool_t res = knh_cwb_isdir(ctx, cwb);
	knh_cwb_close(cwb);
	if(res == 1) {
		if(id != NULL) *id = knh_getURI(ctx, n);
	}
	return res;
}


static knh_Object_t* DIR_newObjectNULL(Ctx *ctx, knh_class_t cid, knh_String_t *s)
{
	knh_bytes_t n = knh_bytes_path(S_tobytes(s));
	if(cid == CLASS_StringARRAY || cid == CLASS_Array) {
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		knh_cwb_write(ctx, cwb, n);
		knh_cwb_ospath(ctx, cwb);
		if(knh_cwb_isdir(ctx, cwb)) {
			knh_Array_t *a = (cid == CLASS_StringARRAY)
				? new_Array(ctx, CLASS_String, 0) : new_Array0(ctx, 0);
#if defined(K_USING_POSIX)
			char *dirname = knh_cwb_tochar(ctx, cwb);
			DIR *dirptr = opendir(dirname);
			KNH_PERROR_IF(ctx, NULL, (dirptr == NULL), "opendir");
			if (dirptr != NULL) {
				struct dirent *direntp;
				while ((direntp = readdir(dirptr)) != NULL) {
					char *p = direntp->d_name;
					if(p[0] == '.' && (p[1] == 0 || p[1] == '.')) continue;
					knh_Array_add(ctx, a, new_S(ctx, B(p)));
				}
				closedir(dirptr);
			}
#else
			KNH_TODO("opendir for this platform");
#endif
			knh_cwb_close(cwb);
			return (knh_Object_t*)a;
		}
		knh_cwb_close(cwb);
	}
	return NULL;
}

static knh_PathDSPI_t FILEPATH_DSPI = {
	K_PATH_DSPI, "file",
	PATH_STREAM, CLASS_InputStream,
	FILE_exists, FILE_newObjectNULL,
};

static knh_PathDSPI_t DIRPATH_DSPI = {
	K_PATH_DSPI, "dir",
	PATH_SYSTEM, CLASS_StringARRAY,
	DIR_exists, DIR_newObjectNULL,
};

static knh_bool_t LIB_exists(Ctx *ctx, knh_bytes_t path, knh_intptr_t *id)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_bytes_t libname = knh_bytes_path(path), funcname = STEXT(""); /* lib:m.sin */
	knh_index_t loc = knh_bytes_index(libname, '.');
	if(loc != -1) {
		libname = knh_bytes_first(libname, loc);
		funcname = knh_bytes_last(libname, loc+1);
	}
	knh_cwb_write(ctx, cwb, libname);
	knh_cwb_ospath(ctx, cwb);
	void *p = knh_cwb_dlopen(ctx, cwb, 0/*isPERROR*/);
	int res = 0;
	if(p == NULL && !knh_bytes_startsWith(libname, STEXT("lib"))) {
		knh_cwb_clear(cwb, 0);
		knh_cwb_write(ctx, cwb, STEXT("lib"));
		knh_cwb_write(ctx, cwb, libname);
		knh_cwb_ospath(ctx, cwb);
		p = knh_cwb_dlopen(ctx, cwb, 0/*isPERROR*/);
	}
	if(p != NULL) {
		res = 1;
		if(funcname.len != 0) {
			void *f = knh_dlsym(ctx, p, (char*)funcname.buf, 0/*isPERROR*/);
			res = (f != NULL);
		}
		knh_dlclose(ctx, p);
	}
	knh_cwb_close(cwb);
	return res;
}

static knh_PathDSPI_t LIBPATH_DSPI = {
	K_PATH_DSPI, "lib",
	PATH_SYSTEM, CLASS_String,
	LIB_exists, NULL
};

/* ------------------------------------------------------------------------ */

knh_PathDSPI_t *knh_getPathDSPINULL(Ctx *ctx, knh_bytes_t path)
{
	return (knh_PathDSPI_t *)knh_getDriverSPI(ctx, K_PATH_DSPI, path);
}

/* ------------------------------------------------------------------------ */

knh_ConverterDSPI_t *knh_getConvTODSPINULL(Ctx *ctx, knh_bytes_t path)
{
	return (knh_ConverterDSPI_t *)knh_getDriverSPI(ctx, K_CONVTO_DSPI, path);
}

/* ------------------------------------------------------------------------ */

knh_ConverterDSPI_t *knh_getConvFROMDSPINULL(Ctx *ctx, knh_bytes_t path)
{
	return (knh_ConverterDSPI_t *)knh_getDriverSPI(ctx, K_CONVFROM_DSPI, path);
}

/* ======================================================================== */
/* K_STREAM_DSPI */

static knh_io_t NOFILE_open(Ctx *ctx, knh_bytes_t n, char *mode)
{
	return IO_NULL;
}
static knh_intptr_t NOFILE_read(Ctx *ctx, knh_io_t fd, char *buf, size_t bufsiz)
{
	return 0;
}
static knh_intptr_t NOFILE_write(Ctx *ctx, knh_io_t fd, char *buf, size_t bufsiz)
{
	return bufsiz;
}
static void NOFILE_close(Ctx *ctx, knh_io_t fd)
{
}
static int NOFILE_feof(Ctx *ctx, knh_io_t fd)
{
	return 1;
}
static int NOFILE_getc(Ctx *ctx, knh_io_t fd)
{
	return -1;
}

static knh_StreamDSPI_t NOFILE_DSPI = {
	K_STREAM_DSPI, "NOP", 0,
	NOFILE_open, NOFILE_open, NOFILE_read, NOFILE_write, NOFILE_close,
	NOFILE_feof, NOFILE_getc
};

/* ------------------------------------------------------------------------ */

static knh_io_t FILE_open(Ctx *ctx, knh_bytes_t path, char *mode)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_openinit(ctx, &cwbbuf, knh_bytes_path(path));
	knh_cwb_ospath(ctx, cwb);
	FILE *fp = knh_fopen(ctx, knh_cwb_tochar(ctx, cwb), mode, /*isPERROR*/1);
	knh_cwb_close(cwb);
	return (knh_io_t)fp;
}
static knh_io_t NOFILE_wopen(Ctx *ctx, knh_bytes_t n, char *mode)
{
	KNH_SYSLOG(ctx, LOG_ALERT, "ReadOnlyFile", "path=%B, mode='%s'", n, mode);
	return IO_NULL;
}
static knh_intptr_t FILE_read(Ctx *ctx, knh_io_t fd, char *buf, size_t bufsiz)
{
	return knh_fread(ctx, buf, bufsiz, (FILE*)fd);
}

static knh_intptr_t FILE_write(Ctx *ctx, knh_io_t fd, char *buf, size_t bufsiz)
{
	size_t ssize = knh_fwrite(ctx, buf, bufsiz, (FILE*)fd);
	knh_fflush(ctx, (FILE*)fd);
	return ssize;
}

static void FILE_close(Ctx *ctx, knh_io_t fd)
{
	knh_fclose(ctx, (FILE*)fd);
}
static int FILE_feof(Ctx *ctx, knh_io_t fd)
{
	return feof((FILE*)fd);
}
static int FILE_getc(Ctx *ctx, knh_io_t fd)
{
	return fgetc((FILE*)fd);
}

static knh_StreamDSPI_t FILE_DSPI = {
	K_STREAM_DSPI, "file", 0,
	FILE_open, FILE_open, FILE_read, FILE_write, FILE_close,
	FILE_feof, FILE_getc,
};

static knh_StreamDSPI_t STDIO_DSPI = {
	K_STREAM_DSPI, "stdio", 0,
	NOFILE_open, NOFILE_open, FILE_read, FILE_write, NOFILE_close,
	FILE_feof, FILE_getc,
};

static knh_InputStream_t *new_InputStream__stdio(Ctx *ctx, FILE *fp, knh_String_t *enc)
{
	knh_InputStream_t* o = new_InputStreamDSPI(ctx, (knh_io_t)stdin, &STDIO_DSPI);
	KNH_SETv(ctx, DP(o)->urn, TS_DEVSTDIN);
	//	knh_InputStream_setEncoding(ctx, o, enc);
	return o;
}

static knh_OutputStream_t *new_OutputStream__stdio(Ctx *ctx, FILE *fp, knh_String_t *enc)
{
#ifdef K_USING_NOFILE
	knh_OutputStream_t* o = new_OutputStream__FILE(ctx, TS_DEVSTDOUT, NULL, &STDIO_DSPI);
#else
	KNH_ASSERT(fp == stdout || fp == stderr);
	knh_OutputStream_t* o = new_OutputStreamDSPI(ctx, (knh_io_t)fp, &STDIO_DSPI);
	if(fp == stdout) {
		KNH_SETv(ctx, DP(o)->urn, TS_DEVSTDOUT);
	}
	else {
		KNH_SETv(ctx, DP(o)->urn, TS_DEVSTDERR);
	}
#endif
	knh_OutputStream_setAutoFlush(o, 1);
//	knh_OutputStream_setEncoding(ctx, o, enc);
	return o;
}

static void knh_cwb_record(Ctx *ctx, knh_cwb_t *cwb, knh_intptr_t *id)
{
	if(id != NULL) {
		*id = knh_getURI(ctx, knh_cwb_tobytes(cwb));
	}
}

static void knh_cwb_writeTOOL(Ctx *ctx, knh_cwb_t *cwb, knh_bytes_t tpath, knh_bytes_t path)
{
	knh_cwb_clear(cwb, 0);
	knh_cwb_write(ctx, cwb, tpath);
	knh_cwb_putc(ctx, cwb, '/');
	knh_cwb_write(ctx, cwb, path);
	knh_cwb_write(ctx, cwb, STEXT(".k"));
	knh_cwb_ospath(ctx, cwb);
}

static knh_bool_t knh_cwb_existsTOOL(Ctx *ctx, knh_cwb_t *cwb, knh_bytes_t path)
{
	knh_String_t *tpath = knh_getPropertyNULL(ctx, STEXT("konoha.tool.path"));
	if(tpath != NULL) {
		knh_cwb_writeTOOL(ctx, cwb, S_tobytes(tpath), path);
		if(knh_cwb_isfile(ctx, cwb)) return 1;
	}
	tpath = knh_getPropertyNULL(ctx, STEXT("user.tool.path"));
	if(tpath != NULL) {
		knh_cwb_writeTOOL(ctx, cwb, S_tobytes(tpath), path);
		if(knh_cwb_isfile(ctx, cwb)) return 1;
	}
	knh_cwb_clear(cwb, 0);
	knh_cwb_write(ctx, cwb, path);
	knh_cwb_write(ctx, cwb, STEXT(".k"));
	knh_cwb_ospath(ctx, cwb);
	return (knh_cwb_isfile(ctx, cwb));
}

static knh_bool_t TOOL_exists(Ctx *ctx, knh_bytes_t path, knh_intptr_t *id)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_bool_t res = knh_cwb_existsTOOL(ctx, cwb, knh_bytes_path(path));
	if(res == 1) {
		knh_cwb_record(ctx, cwb, id);
	}
	knh_cwb_close(cwb);
	return res;
}

static knh_PathDSPI_t TOOLPATH_DSPI = {
	K_PATH_DSPI, "tool",
	PATH_STREAM, CLASS_InputStream,
	TOOL_exists, NULL,
};

static knh_io_t TOOL_open(Ctx *ctx, knh_bytes_t path, char *mode)
{
	FILE *fp = NULL;
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_bool_t res = knh_cwb_existsTOOL(ctx, cwb, knh_bytes_path(path));
	if(res == 1) {
		fp = knh_fopen(ctx, knh_cwb_tochar(ctx, cwb), mode, /*isPERROR*/1);
	}
	knh_cwb_close(cwb);
	return (knh_io_t)fp;
}

static knh_StreamDSPI_t TOOLFILE_DSPI = {
	K_STREAM_DSPI, "tool", 0,
	TOOL_open, NOFILE_wopen, FILE_read, NOFILE_write, FILE_close,
	FILE_feof, FILE_getc,
};

static knh_bytes_t knh_bytes_lastname(knh_bytes_t t)
{
	knh_index_t loc = knh_bytes_rindex(t, '.');
	if(loc != -1) {
		return knh_bytes_last(t, loc+1);
	}
	return t;
}

static void knh_cwb_writePKG(Ctx *ctx, knh_cwb_t *cwb, knh_bytes_t tpath, knh_bytes_t path)
{
	knh_cwb_clear(cwb, 0);
	knh_cwb_write(ctx, cwb, tpath);
	knh_cwb_putc(ctx, cwb, '/');
	knh_cwb_write(ctx, cwb, path); // konoha.math
	knh_cwb_putc(ctx, cwb, '/');
	knh_cwb_write(ctx, cwb, knh_bytes_lastname(path));
	knh_cwb_write(ctx, cwb, STEXT(".k"));
	knh_cwb_ospath(ctx, cwb);
}

static knh_bool_t knh_cwb_existsPKG(Ctx *ctx, knh_cwb_t *cwb, knh_bytes_t path)
{
	char *epath = knh_getenv("KONOHA_PACKAGE");
	knh_String_t *tpath = knh_getPropertyNULL(ctx, STEXT("konoha.package.path"));
	if(epath != NULL) {
		knh_cwb_writePKG(ctx, cwb, B(epath), path);
		if(knh_cwb_isfile(ctx, cwb)) return 1;
	}
	if(tpath != NULL) {
		knh_cwb_writePKG(ctx, cwb, S_tobytes(tpath), path);
		if(knh_cwb_isfile(ctx, cwb)) return 1;
	}
	tpath = knh_getPropertyNULL(ctx, STEXT("user.package.path"));
	if(tpath != NULL) {
		knh_cwb_writePKG(ctx, cwb, S_tobytes(tpath), path);
		if(knh_cwb_isfile(ctx, cwb)) return 1;
	}
	return 0;
}

static knh_bool_t PKG_exists(Ctx *ctx, knh_bytes_t path, knh_intptr_t *id)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_bool_t res = knh_cwb_existsPKG(ctx, cwb, knh_bytes_path(path));
	if(res == 1) {
		knh_cwb_record(ctx, cwb, id);
	}
	knh_cwb_close(cwb);
	return res;
}

static knh_io_t PKG_open(Ctx *ctx, knh_bytes_t path, char *mode)
{
	FILE *fp = NULL;
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_bool_t res = knh_cwb_existsPKG(ctx, cwb, knh_bytes_path(path));
	if(res == 1) {
		fp = knh_fopen(ctx, knh_cwb_tochar(ctx, cwb), mode, /*isPERROR*/1);
	}
	knh_cwb_close(cwb);
	return (knh_io_t)fp;
}

static knh_PathDSPI_t PKGPATH_DSPI = {
	K_PATH_DSPI, "package",
	PATH_STREAM, CLASS_InputStream,
	PKG_exists, NULL,
};

static knh_StreamDSPI_t PKGFILE_DSPI = {
	K_STREAM_DSPI, "package", 0,
	PKG_open, NOFILE_wopen, FILE_read, NOFILE_write, FILE_close,
	FILE_feof, FILE_getc,
};

static void knh_cwb_writeSCRIPT(Ctx *ctx, knh_cwb_t *cwb, knh_bytes_t path)
{
	knh_bytes_t t = S_tobytes(knh_getURN(ctx, SP(ctx->gma)->uri));
	if(t.buf[0] != '(') {  /* NOT (eval) */
		knh_cwb_write(ctx, cwb, t);
		knh_bool_t ret = knh_cwb_parentpath(ctx, cwb, NULL);
		if (ret) {
			knh_cwb_putc(ctx, cwb, '/');
		}
	}
	{
		size_t i;
		for(i = 0; i < path.len; i++) {
			if(path.buf[i] == '.' && path.buf[i+1] == '.'
				&& (path.buf[i+2] == '/' || path.buf[i+2] == '\\')) {
				i += 2; continue;
			}
			knh_Bytes_putc(ctx, cwb->ba, path.buf[i]);
		}
	}
	knh_cwb_ospath(ctx, cwb);
}

static knh_bool_t SCRIPT_exists(Ctx *ctx, knh_bytes_t path, knh_intptr_t *id)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_cwb_writeSCRIPT(ctx, cwb, knh_bytes_path(path));
	knh_bool_t res = knh_cwb_isfile(ctx, cwb);
	if(res == 1) {
		knh_cwb_record(ctx, cwb, id);
	}
	knh_cwb_close(cwb);
	return res;
}

static knh_io_t SCRIPT_open(Ctx *ctx, knh_bytes_t path, char *mode)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_cwb_writeSCRIPT(ctx, cwb, knh_bytes_path(path));
	FILE *fp = knh_fopen(ctx, knh_cwb_tochar(ctx, cwb), mode, /*isPERROR*/1);
	knh_cwb_close(cwb);
	return (knh_io_t)fp;
}

static knh_PathDSPI_t SCRIPTPATH_DSPI = {
	K_PATH_DSPI, "script",
	PATH_STREAM, CLASS_InputStream,
	SCRIPT_exists, NULL,
};

static knh_StreamDSPI_t SCRIPTFILE_DSPI = {
	K_STREAM_DSPI, "script", 0,
	SCRIPT_open, NOFILE_wopen, FILE_read, FILE_write, FILE_close,
	FILE_feof, FILE_getc,
};

/* ------------------------------------------------------------------------ */

static void SYSLOG_UnknownPathType(Ctx *ctx, knh_bytes_t path)
{
	KNH_SYSLOG(ctx, LOG_WARNING, "UnknownPathType", "path='%B'", path);
}

knh_StreamDSPI_t *knh_getStreamDSPI(Ctx *ctx, knh_bytes_t path)
{
	if(path.len == 0) {
		return &NOFILE_DSPI;
	}
	else {
		knh_StreamDSPI_t *p = (knh_StreamDSPI_t*)knh_getDriverSPI(ctx, K_STREAM_DSPI, path);
		if(p == NULL) {
			SYSLOG_UnknownPathType(ctx, path);
			p = &NOFILE_DSPI;
		}
		return p;
	}
}

/* ------------------------------------------------------------------------ */
/* K_QUERY_DSPI */

static void NOP_qfree(knh_qcur_t *qcur)
{
}
static knh_qconn_t *NOP_qopen(Ctx *ctx, knh_bytes_t url)
{
	return NULL;
}
static knh_qcur_t *NOP_query(Ctx *ctx, knh_qconn_t *hdr, knh_bytes_t sql, knh_ResultSet_t *rs)
{
	return NULL;
}
static void NOP_qclose(Ctx *ctx, knh_qconn_t *hdr)
{
}
static int NOP_qnext(Ctx *ctx, knh_qcur_t *qcur, struct knh_ResultSet_t *rs)
{
	return 0;  /* NOMORE */
}

static knh_QueryDSPI_t NOP_DSPI = {
	K_QUERY_DSPI, "NOP",
	NOP_qopen, NOP_query, NOP_qclose, NOP_qnext, NOP_qfree
};

#ifdef K_USING_SQLITE3

#include<sqlite3.h>

static void knh_sqlite3_perror(Ctx *ctx, sqlite3 *db, int r)
{
	char *msg = "sqlite3";
	if(r == SQLITE_PERM || r == SQLITE_AUTH) {
		msg = "Security";
	}
	KNH_SYSLOG(ctx, LOG_WARNING, msg, "sqlite3_error='%s'", sqlite3_errmsg(db));
}

static knh_qconn_t *SQLITE3_qopen(Ctx *ctx, knh_bytes_t url)
{
	sqlite3 *db = NULL;
	url = knh_bytes_path(url);
	int r = sqlite3_open((char*)url.buf, &db);
	if (r != SQLITE_OK) {
		return NULL;
	}
	return (knh_qconn_t*)db;
}

static int SQLITE3_qnext(Ctx *ctx, knh_qcur_t *qcur, struct knh_ResultSet_t *rs)
{
	sqlite3_stmt *stmt = (sqlite3_stmt*)qcur;
	int r = sqlite3_step(stmt);
	if(SQLITE_ROW == r) {
		size_t i;
		knh_ResultSet_initData(ctx, rs);
		for(i = 0; i < DP(rs)->column_size; i++) {
			int type = sqlite3_column_type(stmt, i);
			switch(type) {
				case SQLITE_INTEGER: {
					knh_ResultSet_setInt(ctx, rs, i, (knh_int_t)sqlite3_column_int64(stmt, i));
					break;
				}
				case SQLITE_FLOAT: {
					knh_ResultSet_setFloat(ctx, rs, i, (knh_float_t)sqlite3_column_double(stmt, i));
					break;
				}
				case SQLITE_TEXT: {
					knh_bytes_t t = {{(knh_uchar_t*)sqlite3_column_text(stmt,i)}, sqlite3_column_bytes(stmt, i)};
					knh_ResultSet_setText(ctx, rs, i, t);
					break;
				}
				case SQLITE_BLOB: {
					knh_bytes_t t = {{(knh_uchar_t*)sqlite3_column_blob(stmt,i)}, sqlite3_column_bytes(stmt, i)};
					knh_ResultSet_setBlob(ctx, rs, i, t);
					break;
				}
				case SQLITE_NULL:
				default: {
					knh_ResultSet_setNULL(ctx, rs, i);
				}
			}
		}
		return 1;
	}
	else if (r != SQLITE_DONE) {
	   //
	}
	return 0;  /* NOMORE */
}

static knh_qcur_t *SQLITE3_query(Ctx *ctx, knh_qconn_t *hdr, knh_bytes_t sql, knh_ResultSet_t *rs)
{
	if(rs == NULL) {
		int r = sqlite3_exec((sqlite3*)hdr, (const char*)sql.buf, NULL, NULL, NULL);
		if(r != SQLITE_OK) {
			knh_sqlite3_perror(ctx, (sqlite3*)hdr, r);
		}
		return NULL;
	}
	else {
		sqlite3_stmt *stmt = NULL;
		sqlite3_prepare((sqlite3*)hdr, (char*)sql.buf, sql.len, &stmt, NULL);
//	if (r != SQLITE_OK) {
//		sqlite3_finalize(stmt);
//		DBG_P("msg='%s', sqlite3_errmsg((sqlite3)hdr));
//		return NULL;
//	}
//		r = sqlite3_reset(stmt);
//	if(r != SQLITE_OK) {
//		sqlite3_finalize(stmt);
//		return NULL;
//	}
		size_t column_size = (size_t)sqlite3_column_count(stmt);
		//DBG_P("column_size=%d", column_size);
		knh_ResultSet_initColumn(ctx, rs, column_size);
		if(column_size > 0) {
			size_t i;
			for(i = 0; i < DP(rs)->column_size; i++) {
				char *n = (char*)sqlite3_column_name(stmt, i);
				//DBG_P("(%d) name = '%s'", i, n);
				if(n != NULL) {
					knh_ResultSet_setName(ctx, rs, i, new_S(ctx, B(n)));
				}
			}
		}
		return (knh_qcur_t*)stmt;
	}
}

static void SQLITE3_qclose(Ctx *ctx, knh_qconn_t *hdr)
{
	sqlite3_close((sqlite3*)hdr);
}

static void SQLITE3_qfree(knh_qcur_t *qcur)
{
	sqlite3_stmt *stmt = (sqlite3_stmt*)qcur;
	sqlite3_finalize(stmt);
}

static knh_QueryDSPI_t DB__sqlite3 = {
	K_QUERY_DSPI, "sqlite3",
	SQLITE3_qopen, SQLITE3_query, SQLITE3_qclose, SQLITE3_qnext, SQLITE3_qfree
};

#endif/*K_USING_SQLITE3*/

/* ------------------------------------------------------------------------ */

knh_QueryDSPI_t *knh_getQueryDSPI(Ctx *ctx, knh_bytes_t path)
{
	if(path.len == 0) {
		return &NOP_DSPI;
	}
	else {
		knh_QueryDSPI_t *p = (knh_QueryDSPI_t *)knh_getDriverSPI(ctx, K_QUERY_DSPI, path);
		if(p == NULL) {
			SYSLOG_UnknownPathType(ctx, path);
			p = &NOP_DSPI;
		}
		return p;
	}
}

/* ------------------------------------------------------------------------ */

static knh_bool_t tolowercase(Ctx *ctx, knh_conv_t *cv, knh_bytes_t t, knh_Bytes_t *tobuf)
{
	size_t i, s = BA_size(tobuf);
	knh_Bytes_write(ctx, tobuf, t);
	knh_bytes_t tt = {{tobuf->bu.buf + s}, BA_size(tobuf) - s};
	for(i = 0; i < tt.len; i++) {
		int ch = tt.buf[i];
		if('A' <= ch && ch <= 'Z') {
			tt.buf[i] = ch - 'A' + 'a';
		}
	}
	return 1;
}

static knh_bool_t touppercase(Ctx *ctx, knh_conv_t *cv, knh_bytes_t t, knh_Bytes_t *tobuf)
{
	size_t i, s = BA_size(tobuf);
	knh_Bytes_write(ctx, tobuf, t);
	knh_bytes_t tt = {{tobuf->bu.buf + s}, BA_size(tobuf) - s};
	for(i = 0; i < tt.len; i++) {
		int ch = tt.buf[i];
		if('a' <= ch && ch <= 'z') {
			tt.buf[i] = ch - 'a' + 'A';
		}
	}
	return 1;
}

static knh_ConverterDSPI_t TO_lower = {
	K_CONVTO_DSPI, "lower",
	NULL, tolowercase, tolowercase, tolowercase, tolowercase, NULL, NULL,
};

static knh_ConverterDSPI_t TO_upper = {
	K_CONVTO_DSPI, "upper",
	NULL, touppercase, touppercase, touppercase, touppercase, NULL, NULL,
};


void knh_loadDriver(Ctx *ctx)
{
	const knh_PackageLoaderAPI_t *api = knh_getPackageAPI();
	api->addPathDSPI(ctx, NULL, &NOPATH_DSPI, 0/*isOVERRIDE*/);
	api->addPathDSPI(ctx, "charset", &CHARSETPATH_DSPI, 0/*isOVERRIDE*/);
	api->addPathDSPI(ctx, "to", &TO_DSPI, 0/*isOVERRIDE*/);
	api->addPathDSPI(ctx, "from", &FROM_DSPI, 0/*isOVERRIDE*/);
	api->addPathDSPI(ctx, "class", &CLASSPATH_DSPI, 0/*isOVERRIDE*/);
	api->addPathDSPI(ctx, "method", &METHODPATH_DSPI, 0/*isOVERRIDE*/);
	api->addPathDSPI(ctx, "file", &FILEPATH_DSPI, 0/*isOVERRIDE*/);
	api->addPathDSPI(ctx, "dir", &DIRPATH_DSPI, 0/*isOVERRIDE*/);
	api->addPathDSPI(ctx, "lib", &LIBPATH_DSPI, 0/*isOVERRIDE*/);

	api->addPathDSPI(ctx, "package", &PKGPATH_DSPI, 0/*isOVERRIDE*/);
	api->addPathDSPI(ctx, "script", &SCRIPTPATH_DSPI, 0/*isOVERRIDE*/);
	api->addPathDSPI(ctx, "tool", &TOOLPATH_DSPI, 0/*isOVERRIDE*/);

	api->addConverterDSPI(ctx, "lower", &TO_lower, 0/*isOVERRIDE*/);
	api->addConverterDSPI(ctx, "upper", &TO_upper, 0/*isOVERRIDE*/);

	api->addStreamDSPI(ctx, NULL, &NOFILE_DSPI, 0/*isOVERRIDE*/);
	api->addStreamDSPI(ctx, "file", &FILE_DSPI, 0/*isOVERRIDE*/);
	api->addStreamDSPI(ctx, "script", &SCRIPTFILE_DSPI, 0/*isOVERRIDE*/);
	api->addStreamDSPI(ctx, "tool", &TOOLFILE_DSPI, 0/*isOVERRIDE*/);
	api->addStreamDSPI(ctx, "package", &PKGFILE_DSPI, 0/*isOVERRIDE*/);

	api->addQueryDSPI(ctx, NULL, &NOP_DSPI, 0/*isOVERRIDE*/);
#ifdef K_USING_SQLITE3
	api->addQueryDSPI(ctx, "sqlite3", &DB__sqlite3, 0/*isOVERRIDE*/);
#endif
}

/* ------------------------------------------------------------------------ */
#endif/* K_USING_DEFAULTAPI*/

#ifdef __cplusplus
}
#endif
