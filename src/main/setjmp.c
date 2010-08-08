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

/* ------------------------------------------------------------------------ */

knh_ExceptionHandler_t* knh_ExceptionHandler_setjmp(Ctx *ctx, knh_ExceptionHandler_t *hdr)
{
	knh_uintptr_t rsp;
	__asm__ volatile ("movq %%rsp, %0; " : "=r" (rsp));
//	fprintf (stderr, "The stack pointer is 0x%lx\n", rsp);
	DP(hdr)->return_address = __builtin_return_address(0);
	DP(hdr)->frame_address = __builtin_frame_address(1);
	DP(hdr)->stack_pointer =rsp;
	return NULL;
}

knh_ExceptionHandler_t *knh_ExceptionHandler_longjmp(Ctx *ctx, knh_ExceptionHandler_t *hdr)
{
	knh_uintptr_t rsp = DP(hdr)->stack_pointer;
	void** rbp = (void**)__builtin_frame_address(0);
//	fprintf(stderr, "@%s: return_addr=%p, frame_addr=%p\n",
//			__FUNCTION__, DP(hdr)->return_address, DP(hdr)->frame_address);
//	fprintf(stderr, " sp[0]=%p, sp[1]=%p, sp[2]=%p, sp[3]=%p, sp[4]=%p\n", sp[0], sp[1], sp[2], sp[3], sp[4]);

	rbp[1] = DP(hdr)->return_address;
	KNH_ASSERT(DP(hdr)->return_address == __builtin_return_address(0));
	DP(hdr)->return_address = NULL;
	rbp[0] = DP(hdr)->frame_address;
	KNH_ASSERT(DP(hdr)->frame_address == __builtin_frame_address(1));
	__asm__ volatile ("movq %0, %%rsp; " : : "r" (rsp): "%rsp");
	return hdr;
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
