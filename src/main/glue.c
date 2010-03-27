/****************************************************************************
 * KONOHA COPYRIGHT, LICENSE NOTICE, AND DISCRIMER
 *
 * Copyright (c) 2005-2009, Kimio Kuramitsu <kimio at ynu.ac.jp>
 *           (c) 2008-      Konoha Software Foundation
 * All rights reserved.
 *
 * You may choose one of the following two licenses when you use konoha.
 * See www.konohaware.org/license.html for further information.
 *
 * (1) GNU Lesser General Public License 3.0 (with KONOHA_UNDER_LGPL3)
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

#if !defined(KONOHA_ON_LKM) && !defined(KONOHA_ON_TB) && !defined(KONOHA_ON_WINDOWS)
#include <sys/mman.h> // for PROT_READ,PROT_WRITE,PROT_EXEC
#endif



/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif


/* ------------------------------------------------------------------------ */

int knh_fcallback_cmpr(Object *obj, Object *obj2)
{
	Ctx *ctx = knh_getCurrentContext();
	knh_sfp_t *lsfp = KNH_LOCAL(ctx);
	knh_putsfp(ctx, lsfp, 2, obj);
	knh_putsfp(ctx, lsfp, 3, obj2);
	knh_Closure_invokesfp(ctx, (knh_Closure_t*)(-1), lsfp, /* argc*/ 2);
	return (int)lsfp[0].ivalue;
}

/* ------------------------------------------------------------------------ */

void *knh_generateCallBackFunc(Ctx *ctx, void *func, knh_Closure_t *c)
{
	// (1) duplicate func;
	// (2) replace (-1) with c
	//
	return func;
}

/* ------------------------------------------------------------------------ */

static inline int knh_pow2(int n)
{
	int p = 0;
	if(n <= 0) return 1;
	--n;
	for(;n != 0;n >>=1) {
		p = (p << 1) + 1;
	}
	return p + 1;
}

/* ------------------------------------------------------------------------ */
// HOWTO USE callback func
// 
// first, prepare callback function like "void knh_qsort(void *d1, void *d2)"
// and add additional argument "knh_Closure_t* cc" to knh_qsort.
// So, interface of knh_qsort is like "void knh_qsort(void *d1, void *d2 ,knh_Closure_t* cc)"
// 
// Next, make dummy callback function, dummy_callback.
// dummy_callback's interface is same as previous knh_qsort interface.
// void dummy_callback(void *d1, void *d2);
// dummy_callback function is not used, but these replica use.
// 
//// void Array.myqsort(Closure c);
//METHOD Array_myqsort(Ctx *ctx, knh_sfp_t *sfp)
//{
//	knh_Array_t   *a  = (knh_Array_t *) sfp[0].o;
//	knh_Closure_t *cc = (knh_Closure_t *) sfp[1].cc;
//	void *callback = knh_copy_callbackfunc((void*)dummy_callback,(void *)knh_qsort,cc);
//	((void (*)(int))callback)(a->list,a->size, sizeof(Object*),callback);
//}
//
//static int knh_qsort(void *d1, void *d2 ,knh_Closure_t* cc)
//{
//	Ctx *ctx = knh_getCurrentContext();
//	knh_sfp_t *lsfp = KNH_LOCAL(ctx);
//	Object *arg1 = (Object *)new_Int(ctx,(int)d1);
//	Object *arg2 = (Object *)new_Int(ctx,(int)d2);
//	knh_putsfp(ctx, lsfp, 2, arg1);
//	knh_putsfp(ctx, lsfp, 3, arg2);
//	knh_Closure_invokesfp(ctx, cc, lsfp, /* argc*/ 2);
//	return (int) lsfp[0].ivalue;
//}
//
//static int dummy_callback(void *d1, void *d2)
//{
//	return knh_qsort(d1,d2,(knh_Closure_t*) -1);
//}
//
#define MAX_FUNC_SIZE 256
// copy callback func and return function that allocate form heap.
#if defined(__x86_64__)
#define NEG 0xffffffffffffff
#else
#define NEG 0xffffffff
#endif
#define uchar_t unsigned char
static void knh_func_dump(unsigned char *f)
{
        int i;
        int max = 48;
        //int max = MAX_FUNC_SIZE;
        for (i = 0; i < max; i++) {
                fprintf(stderr, "%02x ",f[i]);
                if (i % 16 == 15) {
                        fprintf(stderr, "\n");
                }
                /*
                if(f[i] == 0xc9 && f[i+1] == 0xc3) {
                        fprintf(stderr, "end\n");
                        //break;
                }
                if(f[i] == 0xc9 && f[i+1] == 0xe9)
                {
                        fprintf(stderr, "end\n");
                        //break;
                }
                */
        }
        fprintf(stderr, "\n");
}

void *knh_copyCallbackfunc(Ctx* ctx, void *func, void *target,knh_Closure_t *cc)
{
    void *callback = NULL;
#if !defined(KONOHA_ON_LKM) && !defined(KONOHA_ON_TB) && !defined(KONOHA_ON_WINDOWS)
    int i,_ffffffff = -1,jmp_pos = -1;
    uchar_t *f = (uchar_t *) func;


    for (i = 0; i < MAX_FUNC_SIZE; i++) {
        // find 0xffffffff(that is dummy forknh_Closure_t pointer)
        if(*(int*) &f[i] == -1) {
            _ffffffff = i;
#if __x86_64__
            i+=7;
#else
            i+=3;
#endif
        }
        // find jmp instraction
        // if arch is i386, jmp instraction is start at "0xe8"
        if(f[i] == 0xe8 && _ffffffff > 0) {
            jmp_pos = i;
        }
        // epilog of function is always 0xc9,0xc3.
        if(f[i] == 0xc9 && f[i+1] == 0xc3 && _ffffffff > 0) {
            i+=2;
            break;
        }
        if(f[i] == 0xc9 && f[i+1] == 0xe9 && _ffffffff > 0){
            //jmp_pos = i + 1;
            jmp_pos = i;
            i+=6;
            break;
        }
    }
    size_t size = knh_pow2(i);
    callback = valloc(size);
    memcpy(callback,func,i);
    mprotect(callback, size, PROT_READ | PROT_WRITE | PROT_EXEC);

    f = (unsigned char *)callback;
#if defined(KNH_DBGMODE)
    fprintf(stderr, "before\n");
    knh_func_dump(f);
#endif

    // patch for 0xffffffff
    if(_ffffffff > 0) {
#ifdef __x86_64__
        *(intptr_t*) &f[_ffffffff] = (intptr_t)cc;
#else
        *(int*) &f[_ffffffff] = (int)cc;
#endif
    }

    // patch for function relocation.
    if(jmp_pos > 0) {
#ifdef __x86_64__
        union opcode {
            uchar_t code[10];
            struct {
                uchar_t op1;uchar_t op2;
                knh_int_t ival;
            } __attribute__((packed));
        } o;
        o.op1 = 0x48; o.op2 = 0xb8;
        o.ival = (intptr_t)target;
        memcpy(&f[jmp_pos], (void*)o.code, 10);
        // 0xff 0xd0 is "call %rax"
        f[jmp_pos + 10] = 0xff;
        f[jmp_pos + 11] = 0xd0;
        // 0xc9 0xc3 "leave; ret;"
        f[jmp_pos + 12] = 0xc9;
        f[jmp_pos + 13] = 0xc3;

#if defined(KNH_DBGMODE)
	fprintf(stderr, "after\n");
	knh_func_dump(f);
#endif
#else
        intptr_t diff = (intptr_t)target - (intptr_t)(&f[jmp_pos] + 5);
        *(intptr_t*) &f[jmp_pos+1] = (intptr_t) diff;
#endif
    }
#endif /* KONOHA_ON_LKM, TB, Windows */
    return callback;
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
