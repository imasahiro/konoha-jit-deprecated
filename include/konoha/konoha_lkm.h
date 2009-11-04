#ifndef KONOHA_LKM_H_
#define KONOHA_LKM_H_

#ifdef KONOHA_ON_LKM
#include<linux/kernel.h>
#include<linux/string.h>
#include<linux/version.h>
#include<linux/slab.h>
#include<linux/types.h>
#include<linux/ctype.h>

#define KNH_USING_INT32      1
#define KNH_USING_NOFLOAT    1
#define KNH_USING_NOFILE     1

#define KNH_EXT_QSORT  1
#define KNH_EXT_SETJMP 1

#undef KNH_USING_SOCKET
#undef KNH_USING_ICONV
#undef KNH_USING_REGEX
#undef KNH_USING_SQLITE3
#undef KNH_USING_KONOHAGET
#undef KNH_USING_PTHREAD
#undef KNH_USING_THREAD
#undef KNH_USING_READLINE

/* stdint.h */
#ifndef _STDINT_H
#define _STDINT_H
typedef long                      intptr_t;
#endif

/* /usr/include/inttypes.h */
#define PRIdPTR "d"
#define PRIuPTR "u"


typedef long jmp_buf ;
typedef intptr_t FILE;

#define TODO_LKM
#define getenv(a) NULL
#define stdin  ((FILE*)NULL)
#define stdout KERN_INFO
#define stderr KERN_ALERT

#define malloc(x) kmalloc(x,GFP_KERNEL)
#define free(x)   kfree(x)

#define fprintf(out,fmt, arg...) printk(out fmt , ##arg)
#define fputs(prompt, fp) 
#define fgetc(fp) (-1)
#define EOF -1
#define fflush(x)
#define exit(i)  printk(KERN_EMERG "KONOHA_exit!!!")
#define setjmp(j) 0
#define longjmp(a,b)
#define assert(x) BUG_ON(!(x))
#define abort() BUG_ON(1)

#include "konoha_debug.h"

/* ../../src/ext/qsort.c */
void knh_qsort (void *const pbase, size_t total_elems, size_t size,
        int (*cmp)(const void*,const void*));

void knh_qsort_r (void *const pbase, size_t total_elems, size_t size,
        int (*cmp)(void* ,const void*,const void*),void* thunk);

/* ../../src/ext/strerror.c */
char* strerror(int errno);
#endif

/* ------------------------------------------------------------------------ */

#endif
