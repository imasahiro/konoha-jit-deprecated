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

#ifndef KONOHA_GLUE_H_
#define KONOHA_GLUE_H_

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */
/* UTILS_API */

typedef struct knh_ExportsAPI_t {
	void* (*malloc)(Ctx *, size_t);
	void  (*free)(Ctx *, void *, size_t);
	// stack
	void  (*setsfp)(Ctx *, knh_sfp_t *, void *);
	void  (*closeIterator)(Ctx *, knh_Iterator_t *);
	void  (*ebilog)(Ctx *, const char*, const char*, int, const char*, ...);
	void  (*perror)(Ctx *, knh_sfp_t *sfp, const char*, const char*);
	void  (*dbg_p)(const char*, const char*, int, const char*, ...);
	void  (*todo_p)(const char*, const char*, int, const char*, ...);
	// cwbbuf
	void (*cwb_clear)(knh_cwb_t *, size_t);
	char* (*cwb_tochar)(Ctx*, knh_cwb_t *);
	// String
	knh_String_t* (*new_String)(Ctx*, char*);
	char* (*tochar)(Ctx*, knh_String_t*);
	// RawPtr
	knh_RawPtr_t* (*new_RawPtr)(Ctx*, void*, knh_FfreeRawPtr, knh_class_t, const char*);
	void (*RawPtr_init)(Ctx *, knh_RawPtr_t*, void *, knh_FfreeRawPtr);
	knh_InputStream_t* (*new_InputStreamNULL)(Ctx *, knh_String_t *, char *mode);
	knh_OutputStream_t* (*new_OutputStreamNULL)(Ctx *, knh_String_t *, char *mode);
	// OutputStream, Bytes
	void (*putc)(Ctx *, void *, int);
	void (*write)(Ctx *, void *, char *, size_t);
} knh_ExportsAPI_t;

#ifdef K_EXPORTS
//#define knh_cwb_open(ctx, cwbbuf)   ctx->api->cwb_open(ctx, cwbbuf)
//#define knh_cwb_clear(cwb, s)       ctx->api->cwb_clear(cwb, s)
#define knh_cwb_tochar(ctx, cwb)    ctx->api->cwb_tochar(ctx, cwb)
//#define knh_cwb_tobytes(cwb, s)     ctx->api->cwb_clear(cwb, s)
//#define new_String(ctx, s)          ctx->api->new_String(ctx, s)
//#define S_tochar(s)                 ctx->api->tochar(ctx, s)
#define new_RawPtr(ctx, p, f, cid, n)  ctx->api->new_RawPtr(ctx, p, f, cid, n)
#define knh_RawPtr_init(ctx, rp, p, f) ctx->api->RawPtr_init(ctx, rp, p, f)
#define new_InputStreamNULL(ctx, s, mode)  ctx->api->new_InputStreamNULL(ctx, s, mode)
#define new_OutputStreamNULL(ctx, s, mode)  ctx->api->new_OutputStreamNULL(ctx, s, mode)
//#define knh_putc(ctx, w, ch)         ctx->api->putc(ctx, w, ch)
//#define knh_write(ctx, w, s, len)    ctx->api->write(ctx, w, s, len)
#endif

/* ------------------------------------------------------------------------ */
/* driver */

typedef struct {
	int   type;
	char *name;
} knh_DriverSPI_t ;

#define K_PATH_DSPI             1
#define K_CONVTO_DSPI           2
#define K_CONVFROM_DSPI         3
#define K_STREAM_DSPI           4
#define K_REGEX_DSPI            5
#define K_QUERY_DSPI            6
#define K_MAP_DSPI              7

#define IS_DRVAPI(c)   (0 < c && c < 8)
#define K_DEFAULT_DSPI          STEXT("")

/* ------------------------------------------------------------------------ */
/* K_PATH_DSPI */

#define PATH_INTERNAL    0
#define PATH_SYSTEM      1
#define PATH_STREAM      2
#define PATH_EXTERNAL    3
#define PATH_CONVERTER   4

typedef struct knh_PathDSPI_t {
	int   type;
	char *name;
	knh_ushort_t pathtype;
	knh_ushort_t cid;
	knh_bool_t (*exists)(Ctx *, knh_bytes_t, knh_intptr_t *);
	Object* (*newObjectNULL)(Ctx *, knh_class_t, knh_String_t *s);
} knh_PathDSPI_t;

/* ------------------------------------------------------------------------ */
/* K_BCONV_DSPI */

typedef struct knh_ConverterDSPI_t {
	int  type;
	char *name;
	knh_conv_t* (*open)(Ctx *, const char*, const char*);
	knh_bool_t (*conv)(Ctx *, knh_conv_t *, knh_bytes_t t, knh_Bytes_t *);
	knh_bool_t (*enc)(Ctx *, knh_conv_t *, knh_bytes_t t, knh_Bytes_t *);
	knh_bool_t (*dec)(Ctx *, knh_conv_t *, knh_bytes_t t, knh_Bytes_t *);
	knh_bool_t (*sconv)(Ctx *, knh_conv_t *, knh_bytes_t t, knh_Bytes_t *);
	void (*close)(Ctx *ctx, knh_conv_t*);
	void (*setparam)(Ctx *ctx, knh_conv_t *, void *, void *);
} knh_ConverterDSPI_t;

/* ------------------------------------------------------------------------ */
/* K_STREAM_DSPI */

typedef void   (*knh_Fclose)(Ctx *, knh_io_t);

typedef struct knh_StreamDSPI_t {
	int type;
	char *name;
	size_t bufsiz;  /* knh_io_t == FILE* if bufsiz == 0 */
	knh_io_t (*fopen)(Ctx *, knh_bytes_t, char *);
	knh_io_t (*wopen)(Ctx *, knh_bytes_t, char *);
	knh_intptr_t (*fread)(Ctx *, knh_io_t, char *, size_t);
	knh_intptr_t (*fwrite)(Ctx *, knh_io_t, char *, size_t);
	void   (*fclose)(Ctx *, knh_io_t);
	int (*feof)(Ctx *, knh_io_t);
	int (*fgetc)(Ctx *, knh_io_t);
	const char* (*getContentType)(Ctx *, knh_io_t);
	const char* (*getCharset)(Ctx *, knh_io_t);
} knh_StreamDSPI_t;

/* ------------------------------------------------------------------------ */
/* K_QUERY_DSPI */

typedef struct knh_QueryDPI_t {
	int   type;
	char *name;
	knh_qconn_t* (*qopen)(Ctx *ctx, knh_bytes_t);
	knh_qcur_t* (*qexec)(Ctx *ctx, knh_qconn_t *, knh_bytes_t, knh_ResultSet_t*);
	void   (*qclose)(Ctx *ctx, knh_qconn_t *);
	int    (*qcurnext)(Ctx *, knh_qcur_t *, knh_ResultSet_t*);
	void   (*qcurfree)(knh_qcur_t *);
} knh_QueryDSPI_t;

/* ------------------------------------------------------------------------ */
/* K_MAP_DSPI */

typedef struct knh_MapDSPI_t {
	int   type;
	char *name;
	const struct knh_MapDSPI_t* (*config)(Ctx *, knh_class_t, knh_class_t);
	knh_map_t* (*init)(Ctx *, size_t, char*, void *);
	void (*ftrmap)(Ctx *, knh_map_t*, knh_Ftraverse ftr);
	void (*freemap)(Ctx *, knh_map_t*);
	knh_bool_t (*get)(Ctx *, knh_map_t*, knh_sfp_t*, knh_sfp_t *);
	knh_map_t* (*set)(Ctx *, knh_map_t*, knh_sfp_t *);  // map pointer might be reallocated
	void (*remove)(Ctx *, knh_map_t*, knh_sfp_t *);
	size_t (*size)(Ctx *, knh_map_t*);
	knh_bool_t (*setIterator)(Ctx *, knh_map_t*, knh_Iterator_t *);
} knh_MapDSPI_t;

/* ------------------------------------------------------------------------ */
/* SHELL_SPI */

typedef struct knh_ShellAPI_t {
	int (*checkstmt)(knh_bytes_t t);
	knh_bool_t (*command)(Ctx *ctx, knh_cwb_t *cwb);
} knh_ShellAPI_t;

typedef struct knh_ShellSPI_t {
	char *name;  // shell name
	void* (*shell_init)(Ctx *ctx, char *msg, char *optstr);
	knh_bool_t (*shell_readstmt)(Ctx *, void* status, knh_cwb_t *cwb, const knh_ShellAPI_t *api);
	void  (*shell_display)(Ctx *, void*, char *, const knh_ShellAPI_t *api);
	void  (*shell_cleanup)(Ctx *, void*);
} knh_ShellSPI_t;

/* ------------------------------------------------------------------------ */
/* REGEX_SPI */

#ifndef K_REGEX_MATCHSIZE
#define K_REGEX_MATCHSIZE    16
#endif

typedef struct {
	int rm_so;   /* start of match */
	int rm_eo;   /* end of match */
	knh_bytes_t rm_name;  /* {NULL, 0}, if not NAMED */
} knh_regmatch_t;

typedef struct knh_RegexSPI_t {
	char *name;
	knh_regex_t* (*regmalloc)(Ctx *, knh_String_t *);
	int (*parse_cflags)(Ctx *, char *opt);
	int (*parse_eflags)(Ctx *, char *opt);
	int (*regcomp)(Ctx *, knh_regex_t *, char *, int);
	int (*regexec)(Ctx *, knh_regex_t *, char *, size_t, knh_regmatch_t*, int);
	size_t (*regerror)(int, knh_regex_t *, char *, size_t);
	void (*regfree)(Ctx *, knh_regex_t *);
} knh_RegexSPI_t;

/* ------------------------------------------------------------------------ */
/* EBI_SPI */

typedef struct knh_EvidenceSPI_t {
	char *name;
	void (*syslog)(int, const char *, ...);
	void (*vsyslog)(int, const char *, va_list);
} knh_EvidenceSPI_t;

/* ------------------------------------------------------------------------ */
/* JIT_SPI */

typedef struct knh_CompilerSPI_t {
	char *name;
	knh_bool_t (*canCompile)(Ctx *ctx, knh_opline_t* line);
	knh_Fmethod (*compile)(Ctx *ctx, knh_opline_t* line);
} knh_CompilerSPI_t;

/* ------------------------------------------------------------------------ */
/* ConstData  */
/* ------------------------------------------------------------------------ */

typedef struct {
	char *name;
	knh_int_t ivalue;
} knh_IntData_t;

typedef struct {
	char *name;
	knh_float_t fvalue;
} knh_FloatData_t;

typedef struct {
	char *name;
	char *value;
} knh_StringData_t;

typedef struct {
	char *name;
	void *ptr;
} knh_NamedPointerData_t;

#define _DATA(s)    (knh_intptr_t)(s)
#define DATA_END      0
#define DATA_STRUCT   1
#define DATA_CLASS    2
#define DATA_CPARAM   3
#define DATA_GENERICS 4
#define DATA_EXPT     5
#define DATA_METHOD0  6
#define DATA_METHOD   7
#define DATA_TCAST   8
//{cid, psize, rsize, 0, 0}
#define DATA_PARAM    9

typedef knh_intptr_t knh_data_t;

/* ------------------------------------------------------------------------ */

typedef struct knh_PackageLoaderAPI_t {
	void (*loadData)(Ctx *, knh_data_t *, knh_ParamArray_t **);
	void (*loadIntData)(Ctx *, knh_IntData_t *);
	void (*loadFloatData)(Ctx *, knh_FloatData_t *);
	void (*loadStringData)(Ctx *, knh_StringData_t *);
	void (*setRegexSPI)(Ctx *, const knh_RegexSPI_t *);
	void (*setShellSPI)(Ctx *, const knh_ShellSPI_t *, int);
	void (*setEvidenceSPI)(Ctx *, const knh_EvidenceSPI_t *, int);
	void (*addPathDSPI)(Ctx *, char*, const knh_PathDSPI_t *, int);
	void (*addStreamDSPI)(Ctx *, char*, const knh_StreamDSPI_t *, int);
	void (*addQueryDSPI)(Ctx *, char *, const knh_QueryDSPI_t *, int);
	void (*addConverterDSPI)(Ctx *, char *, const knh_ConverterDSPI_t*, int);
} knh_PackageLoaderAPI_t;

typedef int  (*knh_FcheckPKG)(void);
typedef void (*knh_FsetupPKG)(Ctx *ctx, const knh_PackageLoaderAPI_t *, char *, int);

#define knh_isSelectedDSPI(c, T)   (c == NULL || strstr(c, ":" T ":") != NULL)

typedef struct {
	const char *name;
	knh_ObjectCSPI_t *cspi;  // if cspi is NULL, rawptr is be used instead.
	void *rawptr;
	knh_FfreeRawPtr freeRawPtr;
} knh_ClassData_t;

typedef knh_ClassData_t* (*knh_Fclass)(void);

/* ------------------------------------------------------------------------ */
/* new version */

#define Boolean_to(T, a)         ((T)a.bvalue)
#define Int_to(T, a)             ((T)a.ivalue)
#define Float_to(T, a)           ((T)a.fvalue)
#define String_to(T, a)          ((T)ctx->api->tochar(ctx, a.s))
#define StringNull_to(T, a, def) ((T)(IS_bString(a.o) ? ctx->api->tochar(ctx, a.s) : def))
#define RawPtr_to(T, a)            ((T)((a.p)->ptr))
#define RawPtrNull_to(T, a, def)   (IS_bRawPtr(a.o) ? ((T)((a.p)->ptr)) : (def))
#define Class_tocid(a)           ((a.c)->cid)

/* ------------------------------------------------------------------------ */

#define RETURN_(vv) {\
		ctx->api->setsfp(ctx, sfp+rix, vv);\
		return; \
	}\

#define RETURNa_(v) {\
		Object *vv_ = (Object*)v;\
		ctx->api->setsfp(ctx, sfp+rix, vv_);\
		sfp[rix].data = knh_Object_data(vv_);\
		return; \
	}\

#define RETURNd_(d) {\
		sfp[rix].data = d; \
		return; \
	}\

#define RETURNb_(c) {\
		sfp[rix].bvalue = c; \
		return; \
	}\

#define RETURNi_(c) {\
		sfp[rix].ivalue = c; \
		return; \
	}\

#define RETURNf_(c) {\
		sfp[rix].fvalue = c; \
		return; \
	}\

#define RETURNvoid_() {\
		return; \
	}\


#define ITREND_() {\
		ctx->api->closeIterator(ctx, sfp[0].it);\
		return 0; \
	}\

#define ITRNEXT_(vv) {\
		ctx->api->setsfp(ctx, sfp+rtnidx, vv);\
		return 1; \
	}\

#define ITRNEXTd_(d) {\
		sfp[rtnidx].data = d;\
		return 1; \
	}\

#define ITRNEXTi_(n) {\
		sfp[rtnidx].ivalue = n;\
		return 1; \
	}\

#define ITRNEXTf_(n) {\
		sfp[rtnidx].fvalue = n;\
		return 1; \
	}\

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif /*KONOHA_GLUE_H_*/
