#ifndef KONOHA_LKM_H_
#define KONOHA_LKM_H_

#if defined(KONOHA_ON_LKM)
#if defined(__APPLE__)
#include <libkern/libkern.h>
#include <sys/types.h>
#include <machine/types.h>
#include <stdint.h>
#include <sys/malloc.h>
#include <AssertMacros.h>
#include <machine/limits.h>
#elif defined(__linux__)
#include<linux/kernel.h>
#include<linux/string.h>
#include<linux/version.h>
#include<linux/slab.h>
#include<linux/types.h>
#include<linux/ctype.h>
#endif

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
#if !defined(_STDINT_H) && !defined(_STDINT_H_)
#define _STDINT_H
typedef long                      intptr_t;
#endif

#ifdef u_int16_t
#define u_int16_t uint16_t
#endif

/* /usr/include/inttypes.h */
#define PRIdPTR "d"
#define PRIuPTR "u"


typedef long jmp_buf ;
typedef intptr_t FILE;

#define TODO_LKM
#define getenv(a) NULL


#if (defined(KONOHA_ON_LKM) && defined(__linux__))
#define malloc(x) kmalloc(x,GFP_KERNEL)
#define free(x)   kfree(x)
#define stdin  ((FILE*)NULL)
#define stdout KERN_INFO
#define stderr KERN_ALERT
#define fprintf(out,fmt, arg...) printk(out fmt , ##arg)
#define exit(i)  printk(KERN_EMERG "KONOHA_exit!!!")
#define assert(x) BUG_ON(!(x))
#define abort() BUG_ON(1)

#elif defined(KONOHA_ON_KEXT)

#define malloc(x) _MALLOC(x, M_TEMP,M_WAITOK)
#define free(x)   _FREE(x, M_TEMP)
#define stdin  ((FILE*)NULL)
#define stdout LOG_INFO
#define stderr LOG_ALERT
#define fprintf(out,fmt, arg...) printf(fmt , ##arg)
#define exit(i)  printf("KONOHA_exit!!!")

#define assert(x) __Verify_Action(!(x), (void*)NULL)
#define abort() assert(1)

#define isdigit(c)	('0' <= (c) && (c) <= '9')
#define islower(c)	('a' <= (c) && (c) <= 'z')
#define isupper(c)	('A' <= (c) && (c) <= 'Z')
#define toupper(c)	(islower(c) ? ((c) - 'a' + 'A') : (c))
#define tolower(c)	(isupper(c) ? ((c) - 'A' + 'a') : (c))
#define isalpha(c)  (isupper(c) || islower(c))
#define isalnum(c)	((isdigit(c)) || isupper(c) || islower(c))
#define isspace(c)  (' ' == (c))
#define isprint(c)  (0x20 <= (c) && (c) <= 0x7e)
#endif /* KONOHA_ON_LKM */

#define fputs(prompt, fp) 
#define fgetc(fp) (-1)
#define EOF -1
#define fflush(x)




#define setjmp(j) 0
#define longjmp(a,b)


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
