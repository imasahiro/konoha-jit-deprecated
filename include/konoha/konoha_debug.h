#ifndef KONOHA_DEBUG_H_
#define KONOHA_DEBUG_H_

#include<syslog.h>

/* ======================================================================== */
/* [FASTMODE] */

#ifdef KNH_FASTMODE
#define KNH_ASSERT(c)
#define SAFE_(stmt)

#else/*KNH_FASTMODE*/
#define KNH_ASSERT(c)    assert(c)
#define SAFE_(stmt)      stmt
#endif/*KNH_FASTMODE*/

#define KNH_ABORT()      abort()

#define LOGARG          ,char *_file, int _line, char *_func
#define LOGDATA         ,(char*)__FILE__, (int)__LINE__, (char*)__FUNCTION__

#define KNH_SYSLOG0(ctx, priority, fmt, ...) \
	fflush(stdout); \
	fprintf(stderr, "konoha (log:%d): ", priority); \
	fprintf(stderr, fmt, ## __VA_ARGS__); \
	fprintf(stderr, "\n"); \

#define KNH_SYSLOG(ctx, priority, fmt, ...) \
	fflush(stdout); \
	fprintf(stderr, "SYSLOG(%d)[%s:%d/%s]\n\t", priority, knh_safefile(__FILE__), __LINE__, __FUNCTION__); \
	fprintf(stderr, fmt, ## __VA_ARGS__); \
	fprintf(stderr, "\n"); \

#define KNH_SYSLOG2(ctx, priority, fmt, ...) \
	fflush(stdout); \
	fprintf(stderr, "SYSLOG(%d)[%s:%d/%s]\n\t", priority, knh_safefile(_file), _line, _func); \
	fprintf(stderr, fmt, ## __VA_ARGS__); \
	fprintf(stderr, "\n"); \

/* ======================================================================== */
/* [DBGMODE2] */

#ifdef KNH_DBGMODE2
#define KNH_DBGMODE     1

#define DBG2_(stmt)  stmt
#define DBG2_ASSERT(c)  KNH_ASSERT(c)
#define DBG2_ABORT()    KNH_ABORT()

#define DBG2_P(fmt, ...) \
	fflush(stdout); \
	fprintf(stderr, "DBG2[%s:%d/%s]\n\t", knh_safefile(__FILE__), __LINE__, __FUNCTION__); \
	fprintf(stderr, fmt, ## __VA_ARGS__); \
	fprintf(stderr, "\n"); \

#define DBG2_DUMP(ctx, o, opt, msg) \
	fflush(stdout); \
	fprintf(stdout, "DBG2[%s]: %s\n", __FUNCTION__, msg); \
	knh_format(ctx, KNH_STDOUT, METHODN__dump, UP(o), UP(opt)); \
	knh_flush(ctx, KNH_STDOUT);\
	fprintf(stdout, "\n"); \

#ifdef KNH_USING_FASTMALLOC
#define KNH_MALLOC(ctx, size)    knh_fastmalloc(ctx, size)
#define KNH_FREE(ctx, p, size)   knh_fastfree(ctx, p, size)
#else
#define KNH_MALLOC(ctx, size)    DBG2_malloc(ctx, size, LOGDATA)
#define KNH_FREE(ctx, p, size)   DBG2_free(ctx, p, size, LOGDATA)
#endif

#else /*KNH_DBGMODE2*/

#define DBG2_(stmt)
#define DBG2_ASSERT(c) KNH_ASSERT(c)
#define DBG2_ABORT()   KNH_ABORT()

#ifdef KONOHA_PREVIEW
#define DBG2_P(fmt, ...) \
	if(konoha_isSystemDump2()) {\
		fflush(stdout); \
		fprintf(stderr, "DBG2[%s:%d/%s]\n\t", knh_safefile(__FILE__), __LINE__, __FUNCTION__); \
		fprintf(stderr, fmt, ## __VA_ARGS__); \
		fprintf(stderr, "\n"); \
	}\

#define DBG2_DUMP(ctx, o, opt, msg) \
	if(konoha_isSystemDump2()) {\
		fflush(stdout); \
		fprintf(stdout, "DBG2[%s]: %s\n", __FUNCTION__, msg); \
		knh_format(ctx, KNH_STDOUT, METHODN__dump, UP(o), UP(opt)); \
		knh_flush(ctx, KNH_STDOUT);\
		fprintf(stdout, "\n"); \
	}\

#else
#define DBG2_P(fmt, ...)
#define DBG2_DUMP(ctx, o, opt, msg)
#endif

#ifdef KNH_USING_FASTMALLOC
#define 	KNH_MALLOC(ctx, size)    knh_fastmalloc(ctx, size)
#define 	KNH_FREE(ctx, p, size)   knh_fastfree(ctx, p, size)
#else/*KNH_USING_FASTMALLOC*/
#define 	KNH_MALLOC(ctx, size)    knh_malloc(ctx, size)
#define 	KNH_FREE(ctx, p, size)   knh_free(ctx, p, size)
#endif/*KNH_USING_FASTMALLOC*/

#endif/*KNH_DBGMODE2*/

/* ======================================================================== */
/* KONOHA_DEBUGMODE */

#ifdef KNH_DBGMODE

#define DBG_(stmt)  stmt

#define DBG_P(fmt, ...) \
	fflush(stdout); \
	fprintf(stderr, "DBG[%s:%d/%s]: ", knh_safefile(__FILE__), __LINE__, __FUNCTION__); \
	fprintf(stderr, fmt, ## __VA_ARGS__); \
	fprintf(stderr, "\n"); \

#define DBG_DUMP(ctx, o, opt, msg) \
	fflush(stdout); \
	fprintf(stdout, "DBG[%s]: %s\n", __FUNCTION__, msg); \
	knh_format(ctx, KNH_STDOUT, METHODN__dump, UP(o), UP(opt)); \
	knh_flush(ctx, KNH_STDOUT);\
	fprintf(stdout, "\n"); \

#define TODO() \
	fprintf(stderr, "TODO[%s:%d/%s]:\n", knh_safefile(__FILE__) , __LINE__, __FUNCTION__); \


#else/*KNH_DBGMODE*/

#define DBG_(stmt)
#define DBG_P(fmt, ...) \
	if(konoha_isSystemDump2()) {\
		fflush(stdout); \
		fprintf(stderr, "DBG[%s:%d/%s]: ", knh_safefile(__FILE__), __LINE__, __FUNCTION__); \
		fprintf(stderr, fmt, ## __VA_ARGS__); \
		fprintf(stderr, "\n"); \
	}\

#define DBG_DUMP(ctx, o, opt, msg) \
	if(konoha_isSystemDump2()) {\
		fprintf(stdout, "DBG[%s]: %s\n", __FUNCTION__, msg); \
		knh_format(ctx, KNH_STDOUT, METHODN__dump, UP(o), UP(opt)); \
		knh_flush(ctx, KNH_STDOUT);\
		fprintf(stdout, "\n"); \
	}\

#define TODO()
#define DBG_ASSERT(c, fmt, ...)   KNH_ASSERT(c)

#define KNH_CTXASSERT(ctx, c, fmt, ...) \
	if(!(c)) { \
		char ebuf_[256];\
		knh_snprintf(ebuf_, sizeof(ebuf_), "Konoha!!: " fmt, ## __VA_ARGS__);\
		KNH_THROWs(ctx, ebuf_);\
	} \

#endif/*KNH_DBGMODE2*/

/* ======================================================================== */

#define KNH_BUGSTOP() {\
		fprintf(stderr, "BUGSTOP[%s:%d]. Please report the above.\n", knh_safefile(__FILE__), __LINE__); \
		exit(0); \
	}\

#define KNH_FUTURE(msg) {\
		fprintf(stderr, "FUTURE[%s:%d] %s will be supported in the near future.\n", knh_safefile(__FILE__), __LINE__, msg); \
	}\

/* ======================================================================== */

#ifdef KNH_USING_THREAD

#define TODO_LOCK() \
	fprintf(stderr, "TODO[%s:%d/%s]: LOCK\n", knh_safefile(__FILE__) , __LINE__, __FUNCTION__); \

#define TODO_UNLOCK() \
	fprintf(stderr, "TODO[%s:%d/%s]: UNLOCK\n", knh_safefile(__FILE__) , __LINE__, __FUNCTION__); \

#else

#define TODO_LOCK()
#define TODO_UNLOCK()

#endif

/* ======================================================================== */

#endif /*KONOHA_DEBUG_H_*/
