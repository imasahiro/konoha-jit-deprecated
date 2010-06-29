#ifndef KONOHA_LKM_H_
#define KONOHA_LKM_H_

#ifdef KONOHA_ON_LKM
#include<linux/kernel.h>
#include<linux/string.h>
#include<linux/version.h>
#include<linux/slab.h>
#include<linux/types.h>
#include<linux/ctype.h>

#define K_USING_INT32      1
#define K_USING_NOFLOAT    1
#define K_USING_NOFILE     1

#define KNH_EXT_QSORT  1
#define KNH_EXT_SETJMP 1

#undef K_USING_SOCKET
#undef K_USING_ICONV
#undef K_USING_REGEX
#undef K_USING_SQLITE3
#undef K_USING_KONOHAGET
#undef K_USING_PTHREAD
#undef K_USING_THREAD
#undef K_USING_READLINE

/* stdint.h */
#ifndef _STDINT_H
#define _STDINT_H
typedef long                      intptr_t;
#endif

/* /usr/include/inttypes.h */
#define PRIdPTR "d"
#define PRIuPTR "u"

typedef intptr_t FILE;

#define TODO_LKM
#define getenv(a) NULL
#define stdin  ((FILE*)NULL)
#define stdout KERN_INFO
#define stderr KERN_ALERT

#define malloc(x) kmalloc(x,GFP_KERNEL)
#define free(x)   kfree(x)

#define fprintf(out,fmt, arg...) printk(out fmt , ##arg)
#define printf(fmt, arg...) printk(fmt , ##arg)
#define fputs(prompt, fp) 
#define fgetc(fp) (-1)
#define EOF -1
#define fflush(x)
#define exit(i)  printk(KERN_EMERG "KONOHA_exit!!!")
#define assert(x) BUG_ON(!(x))
#define abort() BUG_ON(1)

#include "konoha_debug.h"

/* ../../src/ext/qsort.c */
void knh_qsort (void *const pbase, size_t total_elems, size_t size,
        int (*cmp)(const void*,const void*));

void knh_qsort_r (void *const pbase, size_t total_elems, size_t size, 
        void* thunk,int (*cmp)(void* ,const void*,const void*));

//#ifdef KONOHA_ON_MACOSX
//#define  knh_qsort_r(b,s,w,thunk,f)   qsort_r(b,s,w,thunk,f)
//#elif defined(KONOHA_ON_LINUX)
/* ../../src/ext/setjmp.c */
#if defined(__i386__)
/* return_addr, ebx, esp, ebp, esi, edi */
#define JMP_BUFFSIZE 6

#elif defined(__x86_64__)
/* return_addr, rbx, rsp, rbp, r12, r13, r14, r15 */
#define JMP_BUFFSIZE 8
#endif

typedef struct {
	unsigned long __jmp_buf[JMP_BUFFSIZE];
} jmp_buf[1];

#if 1
noinline int knh_setjmp(jmp_buf env);
noinline int knh_longjmp(jmp_buf env, int val);
#else
#define knh_setjmp(a) (0)
#define knh_longjmp(a,b) 
#endif
/* ../../src/ext/strerror.c */
char* strerror(int errno);
#endif

/* ------------------------------------------------------------------------ */

#endif
