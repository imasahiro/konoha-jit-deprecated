#ifndef KONOHA_DEBUG_H_
#define KONOHA_DEBUG_H_

#ifndef KONOHA_ON_LKM
#elif !defined(KONOHA_ON_WINDOWS)
#include<syslog.h>
#endif

/* ------------------------------------------------------------------------ */
/* SYSLOG */

#ifndef LOG_EMERG
#define LOG_EMERG    0 /* system is unusable */
#define LOG_ALERT    1 /* action must be taken immediately */
#define LOG_CRIT     2 /* critical conditions */
#define LOG_ERR      3 /* error conditions */
#define LOG_WARNING  4 /* warning conditions */
#define LOG_NOTICE   5 /* normal but significant condition */
#define LOG_INFO     6 /* informational */
#define LOG_DEBUG    7 /* debug-level messages */
#endif

#ifndef K_EVIDENCE_NAMESPACE
#define K_EVIDENCE_NAMESPACE   "knh"
#endif

#define KNH_SYSLOG(ctx, p, e, fmt, ...) \
	ctx->api->ebilog(ctx, K_EVIDENCE_NAMESPACE, e, p, fmt, ## __VA_ARGS__)

#define KNH_PERROR(ctx, sfp, funcname) \
	ctx->api->perror(ctx, sfp, K_PERROR_LIBNAME, funcname)\

#define KNH_PERROR_IF(ctx, sfp, IS, funcname) \
	if(unlikely(IS)) {\
		ctx->api->perror(ctx, sfp, K_PERROR_LIBNAME, funcname);\
	}\

#define PERROR_RETURN_(C, F, ...) {\
		int res_ = F(__VA_ARGS__);\
		if(res_ == K_PERROR_FAILED) {\
			ctx->api->perror(ctx, sfp, K_PERROR_LIBNAME, #F);\
			K_PERROR_BEFORE_RETRUN;\
			RETURN_(ctx->api->new_Null(ctx, CLASS_##C));\
		}\
	}\

#define PERROR_RETURNs_(F, ...) {\
		char *res_ = F(__VA_ARGS__);\
		if(res_ == NULL) {\
			ctx->api->perror(ctx, sfp, K_PERROR_LIBNAME, #F);\
		}\
		K_PERROR_BEFORE_RETRUN;\
		RETURN_(ctx->api->new_String(ctx, res_));\
	}\

#define PERROR_RETURNi_(F, ...) {\
		int res_ = F(__VA_ARGS__);\
		if(res_ == K_PERROR_FAILED) {\
			ctx->api->perror(ctx, sfp, K_PERROR_LIBNAME, #F);\
		}\
		K_PERROR_BEFORE_RETRUN;\
		RETURNi_(res_);\
	}\

#define PERROR_returni_(F, ...) {\
		int res_ = F(__VA_ARGS__);\
		if(res_ == K_PERROR_FAILED) {\
			ctx->api->perror(ctx, NULL, K_PERROR_LIBNAME, #F);\
		}\
		K_PERROR_BEFORE_RETRUN;\
		return res_;\
	}\

#define PERROR_RETURNb_(F, ...) {\
		int res_ = F(__VA_ARGS__);\
		if(res_ == K_PERROR_FAILED) {\
			ctx->api->perror(ctx, sfp, K_PERROR_LIBNAME, #F);\
		}\
		K_PERROR_BEFORE_RETRUN;\
		RETURNb_((res_ != -1));\
	}\

#define PERROR_returnb_(F, ...) {\
		int res_ = F(__VA_ARGS__);\
		if(res_ == K_PERROR_FAILED) {\
			ctx->api->perror(ctx, NULL, K_PERROR_LIBNAME, #F);\
		}\
		K_PERROR_BEFORE_RETRUN;\
		return (res_ != -1);\
	}\

/* ======================================================================== */
/* [DBGMODE] */

#define TRACEARG          ,char *_file, int _line, char *_func
#define TRACEDATA         ,(char*)__FILE__, (int)__LINE__, (char*)__FUNCTION__

#ifdef K_USING_DEBUG0
#undef K_USING_DEBUG
#define KNH_ASSERT(c)
#define KNH_ABORT() {\
		KNH_SYSLOG(ctx, LOG_ERR, "Emergency Exit at %s", (char*)__FUNCTION__);\
		exit(1); \
	} \

//#define SAFE_(stmt)

#else/*KNH_FASTMODE*/
#define KNH_ASSERT(c)    assert(c)
#define KNH_ABORT()      abort()
#endif/*KNH_FASTMODE*/

#ifdef K_USING_DEBUG
#define DBG_(stmt)     stmt
#define DBG_ASSERT(c)  KNH_ASSERT(c)
#define DBG_ABORT()    KNH_ABORT()
#else
#define DBG_(stmt)
#define DBG_ASSERT(c)
#define DBG_ABORT()
#endif

/* MALLOC */

#ifdef K_EXPORTS
#define KNH_MALLOC(ctx, size)       ctx->api->malloc(ctx, size)
#define KNH_FREE(ctx, p, size)      ctx->api->free(ctx, p, size)
#else
//#define K_USING_TRACEMALLOC 1
#if defined(K_USING_TRACEMALLOC)
	#define KNH_MALLOC(ctx, size)    TRACE_malloc(ctx, size TRACEDATA)
	#define KNH_FREE(ctx, p, size)   TRACE_free(ctx, p, size TRACEDATA)
#elif defined(K_USING_FASTMALLOC)
	#define KNH_MALLOC(ctx, size)    knh_fastmalloc(ctx, size)
	#define KNH_FREE(ctx, p, size)   knh_fastfree(ctx, p, size)
#else
	#define KNH_MALLOC(ctx, size)    knh_malloc(ctx, size)
	#define KNH_FREE(ctx, p, size)   knh_free(ctx, p, size)
#endif
#endif

#define KNH_VALLOC(ctx, size)        knh_valloc(ctx, size)
#define KNH_VFREE(ctx, p, size)      knh_vfree(ctx, p, size)



/* error */
#define KERR_ERR       LOG_CRIT
#define KERR_TERROR    LOG_ERR
/* warning */
#define KERR_EWARN     LOG_WARNING
#define KERR_DWARN     LOG_WARNING
#define KERR_ERRATA    LOG_DEBUG
/* info */
#define KERR_BAD       LOG_NOTICE
#define KERR_TINFO     LOG_INFO
#define KERR_NOTICE    LOG_NOTICE
#define KERR_INFO      LOG_INFO

#ifdef K_EXPORTS
#define DBG_P(fmt, ...)  ctx->api->dbg_p(__FILE__, __FUNCTION__, __LINE__, fmt, ## __VA_ARGS__)
#define TODO_P(fmt, ...) ctx->api->todo_p(__FILE__, __FUNCTION__, __LINE__, fmt, ## __VA_ARGS__)

#else/*K_EXPORTS*/
#ifdef K_USING_DEBUG
#define DBG_P(fmt, ...)  dbg_p(__FILE__, __FUNCTION__, __LINE__, fmt, ## __VA_ARGS__)
#define TRACE_P(fmt, ...)  dbg_p(_file, _func, _line, fmt, ## __VA_ARGS__)
#define TODO_P(fmt, ...) todo_p(__FILE__, __FUNCTION__, __LINE__, fmt, ## __VA_ARGS__)
#else
#define DBG_P(fmt, ...)
#define TRACE_P(fmt, ...)
#define TODO_P(fmt, ...)
#endif
#endif/*K_EXPORTS*/

#define TODO0() {\
		fflush(stdout); \
		fprintf(stderr, "%s*TODO(%s:%d) TODO\n", __FUNCTION__, knh_sfile(__FILE__), __LINE__); \
	}\

#define TODO0_P(fmt, ...) {\
		fflush(stdout); \
		fprintf(stderr, "%s*TODO(%s:%d) ", __FUNCTION__, knh_sfile(__FILE__), __LINE__); \
		fprintf(stderr, fmt, ## __VA_ARGS__); \
		fprintf(stderr, "\n"); \
	}\

#ifdef K_USING_DEBUG
#define TODO          TODO0
#else
#define TODO()
#endif/*K_USING_DEBUG0*/

#define K_TODOMSG \
	"\n" \
	"* ------------------------------------------------------------ *\n" \
	"  You are the first person to use: %s.\n" \
	"  This function has been not implemented yet, unfortunately.\n" \
	"  Check out:\n\t%s in %s:%d\n" \
	"  Or, contact konoha development team via:\n" \
	"\thttp://codes.google.com/p/konoha.\n" \
	"\thttp://konoha.sourceforge.jp/.\n" \
	"  Thank you for using konoha. \n" \
	"* ------------------------------------------------------------ *\n" \

#define KNH_TODO(msg) \
	fprintf(stderr, K_TODOMSG, msg, __FUNCTION__, __FILE__, __LINE__);\
	KNH_ABORT();\


#define K_TESTMSG \
	"\n" \
	"* ------------------------------------------------------------ *\n" \
	"  You are the first person to try: %s.\n" \
	"  This function has been tested yet.\n" \
	"  Check out and remove this message at:\n\t%s in %s:%d\n" \
	"  Thank you for testing konoha. \n" \
	"* ------------------------------------------------------------ *\n" \

#define KNH_TEST(msg) \
	fprintf(stderr, K_TESTMSG, msg, __FUNCTION__, __FILE__, __LINE__);\

/* ------------------------------------------------------------------------ */

#define KNH_MAX(a,b)     (((a) > (b)) ? (a) : (b))
#define KNH_MIN(a,b)     (((a) < (b)) ? (a) : (b))
#define KNH_MID(a,b)     (((a) + (b)) / 2)

/* ------------------------------------------------------------------------ */

#endif /*KONOHA_DEBUG_H_*/
