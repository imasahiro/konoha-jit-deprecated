#ifndef IJT_CONVERT_H
#define IJT_CONVERT_H

#include "asm.h"
#define IJT_DBG_MODE 1
#if IJT_DBG_MODE
#define IDEBUG(fmt, ...) {\
    fprintf(stdout, fmt, ## __VA_ARGS__); \
}
#else
#define IDEBUG(fmt, ...) 
#endif

#define jit_start(mem) {        \
    /* pushq %rbp */            \
    jit_1__push_ebp(mem);       \
    /* movq %rsp %rbp */        \
    jit_3__movq_rsp_rbp(mem);   \
    /* save ctx */              \
    /* pushq %r12 */            \
    jit_2__push_r12(mem);       \
    /* movq %rdi %r12 */        \
    jit_3__movq_rdi_r12(mem);   \
    /* save sfp */              \
    /* pushq %rbx (%ebx) */     \
    jit_1__push_ebx(mem);       \
    /* movq %rsi %rbx */        \
    jit_3__movq_rsi_rbx(mem);   \
}

#define jit_complete(mem) {\
    jit_1__pop_ebx(mem);    \
    jit_2__pop_r12(mem);    \
    jit_1__leave(mem);      \
    jit_1__ret(mem);        \
}

#define JIT_ASM_HALT(ctx) {\
	imem_setLabel(mem, op->opcode, OPSIZE_HALT, 0); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_HALT_); \
	goto CTT_FIN;\
}
#define JIT_ASM_MOVa(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_MOVa, 0); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_MOVa_); \
}
#define JIT_ASM_MOVn(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_MOVn, 0); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_MOVn_); \
}
#define JIT_ASM_MOVo(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_MOVo, 0); \
	IDEBUG("%-8s a1=%d,a2=%p\n","MOVo",a1,(void*)a2); \
	jit_10__movq_imm64_rcx(mem,(knh_int_t)a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_MOVo_); \
}
#define JIT_ASM_MOVi(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_MOVi, 0); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_MOVi_); \
}
#define JIT_ASM_MOVx(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_MOVx, 0); \
	jit_6__movq_imm32_r8d(mem,a2.n); \
	jit_5__movl_imm32_ecx(mem,a2.i); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_MOVx_); \
}
#define JIT_ASM_MOVDEF(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_MOVDEF, 0); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_MOVDEF_); \
}
#define JIT_ASM_MOVSYS(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_MOVSYS, 0); \
	IDEBUG( "%-8s a1=%d,a2=%d\n","MOVSYS",a1,a2); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_MOVSYS_); \
}
#define JIT_ASM_MOVxi(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_MOVxi, 0); \
	jit_6__movq_imm32_r8d(mem,a2.n); \
	jit_5__movl_imm32_ecx(mem,a2.i); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_MOVxi_); \
}
#define JIT_ASM_MOVxf(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_MOVxf, 0); \
	jit_6__movq_imm32_r8d(mem,a2.n); \
	jit_5__movl_imm32_ecx(mem,a2.i); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_MOVxf_); \
}
#define JIT_ASM_MOVxb(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_MOVxb, 0); \
	jit_6__movq_imm32_r8d(mem,a2.n); \
	jit_5__movl_imm32_ecx(mem,a2.i); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_MOVxb_); \
}
#define JIT_ASM_MOVe(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_MOVe, 0); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_MOVe_); \
}
#define JIT_ASM_XMOVs(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_XMOVs, 0); \
	jit_6__movq_imm32_r8d(mem,a2); \
	jit_5__movl_imm32_ecx(mem,a1.n); \
	jit_5__movl_imm32_edx(mem,a1.i); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_XMOVs_); \
}
#define JIT_ASM_XMOVo(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_XMOVo, 0); \
	jit_10__movq_imm64_r8(mem,(knh_int_t)a2); \
	jit_5__movl_imm32_ecx(mem,a1.n); \
	jit_5__movl_imm32_edx(mem,a1.i); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_XMOVo_); \
}
#define JIT_ASM_XMOVx(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_XMOVx, 0); \
	jit_6__movq_imm32_r9d(mem,a2.n); \
	jit_6__movq_imm32_r8d(mem,a2.i); \
	jit_5__movl_imm32_ecx(mem,a1.n); \
	jit_5__movl_imm32_edx(mem,a1.i); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_XMOVx_); \
}
#define JIT_ASM_XMOVDEF(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_XMOVDEF, 0); \
	jit_6__movq_imm32_r8d(mem,a2); \
	jit_5__movl_imm32_ecx(mem,a1.n); \
	jit_5__movl_imm32_edx(mem,a1.i); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_XMOVDEF_); \
}
#define JIT_ASM_XMOVSYS(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_XMOVSYS, 0); \
	jit_6__movq_imm32_r8d(mem,a2); \
	jit_5__movl_imm32_ecx(mem,a1.n); \
	jit_5__movl_imm32_edx(mem,a1.i); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_XMOVSYS_); \
}
#define JIT_ASM_XMOVsi(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_XMOVsi, 0); \
	jit_6__movq_imm32_r8d(mem,a2); \
	jit_5__movl_imm32_ecx(mem,a1.n); \
	jit_5__movl_imm32_edx(mem,a1.i); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_XMOVsi_); \
}
#define JIT_ASM_XMOVsf(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_XMOVsf, 0); \
	jit_6__movq_imm32_r8d(mem,a2); \
	jit_5__movl_imm32_ecx(mem,a1.n); \
	jit_5__movl_imm32_edx(mem,a1.i); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_XMOVsf_); \
}
#define JIT_ASM_XMOVsb(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_XMOVsb, 0); \
	jit_6__movq_imm32_r8d(mem,a2); \
	jit_5__movl_imm32_ecx(mem,a1.n); \
	jit_5__movl_imm32_edx(mem,a1.i); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_XMOVsb_); \
}
#define JIT_ASM_SWAP(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_SWAP, 0); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_SWAP_); \
}
#define JIT_ASM_PARAMDEF(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_PARAMDEF, 0); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_PARAMDEF_); \
}
#define JIT_ASM_PARAMo(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_PARAMo, 0); \
	jit_10__movq_imm64_rcx(mem,(knh_int_t)a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_PARAMo_); \
}
#define JIT_ASM_PARAMPROP(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_PARAMPROP, 0); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_PARAMPROP_); \
}
#define JIT_ASM_PARAMS(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_PARAMS, 0); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_PARAMS_); \
}
#define JIT_ASM_CHKESP(ctx,a1) {\
	imem_setLabel(mem, op->opcode, OPSIZE_CHKESP, 0); \
	IDEBUG( "%-8s a1=%d\n","CHECKESP",a1); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_CHKESP_); \
}
#define JIT_ASM_RET(ctx) {\
	imem_setLabel(mem, op->opcode, OPSIZE_RET, 0); \
	IDEBUG( "%-8s\n","RET"); \
	jit_complete(mem);\
}
#define JIT_ASM_YEILDBREAK(ctx) {\
	imem_setLabel(mem, op->opcode, OPSIZE_YEILDBREAK, 0); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_YEILDBREAK_); \
}
#define JIT_ASM_BOX(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_BOX, 0); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_BOX_); \
}
#define JIT_ASM_BOXnc(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_BOXnc, 0); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_BOXnc_); \
}
#define JIT_ASM_NNBOX(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_NNBOX, 0); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_NNBOX_); \
}
#define JIT_ASM_NNBOXnc(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_NNBOXnc, 0); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_NNBOXnc_); \
}
#define JIT_ASM_UNBOX(ctx,a1) {\
	imem_setLabel(mem, op->opcode, OPSIZE_UNBOX, 0); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_UNBOX_); \
}
#define JIT_ASM_CHKNUL(ctx,a1) {\
	imem_setLabel(mem, op->opcode, OPSIZE_CHKNUL, 0); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_CHKNUL_); \
}
#define JIT_ASM_CHKNULx(ctx,a1) {\
	imem_setLabel(mem, op->opcode, OPSIZE_CHKNULx, 0); \
	jit_5__movl_imm32_ecx(mem,a1.n); \
	jit_5__movl_imm32_edx(mem,a1.i); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_CHKNULx_); \
}
#define JIT_ASM_CHKTYPE(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_CHKTYPE, 0); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_CHKTYPE_); \
}
#define JIT_ASM_FCALL(ctx,a1,a2,a3,a4) {\
	imem_setLabel(mem, op->opcode, OPSIZE_FCALL, 0); \
	IDEBUG("%-8s a1=%d,a2=%d,a3=%d,a4=%p\n","FCALL",a1,a2,a3, (void*) a4); \
	jit_10__movq_imm64_r9(mem,(knh_int_t)a4); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_FCALL_); \
}
#define JIT_ASM_SCALL(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_SCALL, 0); \
	IDEBUG( "%-8s a1=%d,a2=%d,a3=%p\n","SCALL",a1,a2,(void*)a3); \
	jit_10__movq_imm64_r8(mem, (knh_int_t)a3); \
	jit_5__movl_imm32_ecx(mem,(int)a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_SCALL_); \
}
#define JIT_ASM_AINVOKE(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_AINVOKE, 0); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_AINVOKE_); \
}
#define JIT_ASM_CALL(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_CALL, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_CALL_); \
}
#define JIT_ASM_ACALL(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_ACALL, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_ACALL_); \
}
// FIXME "a5" is not imm32, also imm64.
// we have to find how to put imm64 data to rsp.
#define JIT_ASM_NEW(ctx,a1,a2,a3,a4,a5) {\
	imem_setLabel(mem, op->opcode, OPSIZE_NEW, 0); \
	jit_7__movq_imm32_rsp(mem,(int)a5); \
	jit_6__movq_imm32_r9d(mem,a4); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_NEW_); \
}
#define JIT_ASM_COPYSFP(ctx,a1) {\
	imem_setLabel(mem, op->opcode, OPSIZE_COPYSFP, 0); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_COPYSFP_); \
}
#define JIT_ASM_STR(ctx,a1,a2,a3,a4) {\
	imem_setLabel(mem, op->opcode, OPSIZE_STR, 0); \
	jit_10__movq_imm64_r9(mem, (knh_int_t)a4); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_STR_); \
}
#define JIT_ASM_SSTR(ctx,a1,a2,a3,a4) {\
	imem_setLabel(mem, op->opcode, OPSIZE_SSTR, 0); \
	jit_10__movq_imm64_r9(mem, (knh_int_t)a4); \
	jit_10__movq_imm64_r8(mem, (knh_int_t)a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_SSTR_); \
}
#define JIT_ASM_SMAP(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_SMAP, 0); \
	jit_10__movq_imm64_rcx(mem, (knh_int_t)a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_SMAP_); \
}
#define JIT_ASM_SMAPnc(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_SMAPnc, 0); \
	jit_10__movq_imm64_rcx(mem, (knh_int_t)a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_SMAPnc_); \
}
#define JIT_ASM_MAP(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_MAP, 0); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_MAP_); \
}
#define JIT_ASM_MAPnc(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_MAPnc, 0); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_MAPnc_); \
}
#define JIT_ASM_AMAP(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_AMAP, 0); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_AMAP_); \
}
#define JIT_ASM_NNMAP(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_NNMAP, 0); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_NNMAP_); \
}
#define JIT_ASM_JMP(ctx,a1) {\
	imem_setLabel(mem, op->opcode, OPSIZE_JMP, (a1 - pc)); \
	jit_5__jmp(mem, 0); \
}
#define JIT_ASM_SKIP(ctx,a1) {\
	imem_setLabel(mem, op->opcode, OPSIZE_SKIP, (a1 - pc)); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_SKIP_); \
}
#define JIT_ASM_bJIFT(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_bJIFT, (a1 - pc)); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_bJIFT_); \
}
#define JIT_ASM_bJIFF(ctx,a1,a2) {\
	int _tmp =  0x08 + a2 * 0x10; \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	if(a2 < 8) { \
		jit_5__cmp_imm8_rsi_at(mem,1,_tmp); \
	} else { \
		jit_8__cmp_imm8_rsi_at(mem,1,_tmp); \
	} \
	imem_setLabel(mem, op->opcode, OPSIZE_bJIFF, (a1 - pc)); \
	jit_2__jne_to(mem, 0); \
}
#define JIT_ASM_bJIFF_LOOP(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_bJIFF_LOOP, (a1 - pc)); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_bJIFF_LOOP_); \
}
#define JIT_ASM_JIFNUL(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_JIFNUL, 0); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_JIFNUL_); \
}
#define JIT_ASM_JIFNN(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_JIFNN, 0); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_JIFNN_); \
}
#define JIT_ASM_NEXT(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_NEXT, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_NEXT_); \
}
#define JIT_ASM_INEXT(ctx,a1,a2,a3,a4) {\
	imem_setLabel(mem, op->opcode, OPSIZE_INEXT, 0); \
	jit_6__movq_imm32_r9d(mem,a4); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_INEXT_); \
}
#define JIT_ASM_TRY(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_TRY, 0); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_TRY_); \
}
#define JIT_ASM_TRYEND(ctx,a1) {\
	imem_setLabel(mem, op->opcode, OPSIZE_TRYEND, 0); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_TRYEND_); \
}
#define JIT_ASM_CATCH(ctx,a1,a2,a3,a4) {\
	imem_setLabel(mem, op->opcode, OPSIZE_CATCH, 0); \
	jit_6__movq_imm32_r9d(mem,a4); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_CATCH_); \
}
#define JIT_ASM_PUSH(ctx,a1) {\
	imem_setLabel(mem, op->opcode, OPSIZE_PUSH, 0); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_PUSH_); \
}
#define JIT_ASM_POP(ctx,a1) {\
	imem_setLabel(mem, op->opcode, OPSIZE_POP, 0); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_POP_); \
}
#define JIT_ASM_THROW(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_THROW, 0); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_THROW_); \
}
#define JIT_ASM_THROWs(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_THROWs, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_THROWs_); \
}
#define JIT_ASM_THROW_AGAIN(ctx,a1) {\
	imem_setLabel(mem, op->opcode, OPSIZE_THROW_AGAIN, 0); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_THROW_AGAIN_); \
}
#define JIT_ASM_P(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_P, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_P_); \
}
#define JIT_ASM_PMSG(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_PMSG, 0); \
	jit_10__movq_imm64_rcx(mem, (knh_int_t)a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_PMSG_); \
}
#define JIT_ASM_iCAST(ctx,a1) {\
	imem_setLabel(mem, op->opcode, OPSIZE_iCAST, 0); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_iCAST_); \
}
#define JIT_ASM_inCAST(ctx,a1) {\
	imem_setLabel(mem, op->opcode, OPSIZE_inCAST, 0); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_inCAST_); \
}
#define JIT_ASM_fCAST(ctx,a1) {\
	imem_setLabel(mem, op->opcode, OPSIZE_fCAST, 0); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_fCAST_); \
}
#define JIT_ASM_fnCAST(ctx,a1) {\
	imem_setLabel(mem, op->opcode, OPSIZE_fnCAST, 0); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_fnCAST_); \
}
#define JIT_ASM_bNOT(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_bNOT, 0); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_bNOT_); \
}
#define JIT_ASM_iNEG(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_iNEG, 0); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_iNEG_); \
}
#define JIT_ASM_iADD(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_iADD, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_iADD_); \
}
#define JIT_ASM_iADDn(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_iADDn, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_iADDn_); \
}
#define JIT_ASM_iSUB(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_iSUB, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_iSUB_); \
}
#define JIT_ASM_iSUBn(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_iSUBn, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_iSUBn_); \
}
#define JIT_ASM_iMUL(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_iMUL, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_iMUL_); \
}
#define JIT_ASM_iMULn(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_iMULn, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_iMULn_); \
}
#define JIT_ASM_iDIV(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_iDIV, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_iDIV_); \
}
#define JIT_ASM_iDIVn(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_iDIVn, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_iDIVn_); \
}
#define JIT_ASM_iMOD(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_iMOD, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_iMOD_); \
}
#define JIT_ASM_iMODn(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_iMODn, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_iMODn_); \
}
#define JIT_ASM_iEQ(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_iEQ, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_iEQ_); \
}
#define JIT_ASM_iEQn(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_iEQn, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_iEQn_); \
}
#define JIT_ASM_iNEQ(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_iNEQ, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_iNEQ_); \
}
#define JIT_ASM_iNEQn(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_iNEQn, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_iNEQn_); \
}
#define JIT_ASM_iLT(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_iLT, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_iLT_); \
}
#define JIT_ASM_iLTn(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_iLTn, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_iLTn_); \
}
#define JIT_ASM_iLTE(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_iLTE, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_iLTE_); \
}
#define JIT_ASM_iLTEn(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_iLTEn, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_iLTEn_); \
}
#define JIT_ASM_iGT(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_iGT, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_iGT_); \
}
#define JIT_ASM_iGTn(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_iGTn, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_iGTn_); \
}
#define JIT_ASM_iGTE(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_iGTE, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_iGTE_); \
}
#define JIT_ASM_iGTEn(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_iGTEn, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_iGTEn_); \
}
#define JIT_ASM_fNEG(ctx,a1,a2) {\
	imem_setLabel(mem, op->opcode, OPSIZE_fNEG, 0); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_fNEG_); \
}
#define JIT_ASM_fADD(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_fADD, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_fADD_); \
}
#define JIT_ASM_fADDn(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_fADDn, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_fADDn_); \
}
#define JIT_ASM_fSUB(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_fSUB, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_fSUB_); \
}
#define JIT_ASM_fSUBn(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_fSUBn, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_fSUBn_); \
}
#define JIT_ASM_fMUL(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_fMUL, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_fMUL_); \
}
#define JIT_ASM_fMULn(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_fMULn, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_fMULn_); \
}
#define JIT_ASM_fDIV(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_fDIV, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_fDIV_); \
}
#define JIT_ASM_fDIVn(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_fDIVn, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_fDIVn_); \
}
#define JIT_ASM_fEQ(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_fEQ, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_fEQ_); \
}
#define JIT_ASM_fEQn(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_fEQn, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_fEQn_); \
}
#define JIT_ASM_fNEQ(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_fNEQ, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_fNEQ_); \
}
#define JIT_ASM_fNEQn(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_fNEQn, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_fNEQn_); \
}
#define JIT_ASM_fLT(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_fLT, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_fLT_); \
}
#define JIT_ASM_fLTn(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_fLTn, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_fLTn_); \
}
#define JIT_ASM_fLTE(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_fLTE, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_fLTE_); \
}
#define JIT_ASM_fLTEn(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_fLTEn, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_fLTEn_); \
}
#define JIT_ASM_fGT(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_fGT, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_fGT_); \
}
#define JIT_ASM_fGTn(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_fGTn, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_fGTn_); \
}
#define JIT_ASM_fGTE(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_fGTE, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_fGTE_); \
}
#define JIT_ASM_fGTEn(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_fGTEn, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_fGTEn_); \
}
#define JIT_ASM_ARYGET(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_ARYGET, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_ARYGET_); \
}
#define JIT_ASM_ARYGETn(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_ARYGETn, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_ARYGETn_); \
}
#define JIT_ASM_iARYGET(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_iARYGET, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_iARYGET_); \
}
#define JIT_ASM_iARYGETn(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_iARYGETn, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_iARYGETn_); \
}
#define JIT_ASM_fARYGET(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_fARYGET, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_fARYGET_); \
}
#define JIT_ASM_fARYGETn(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_fARYGETn, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_fARYGETn_); \
}
#define JIT_ASM_ARYSET(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_ARYSET, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_ARYSET_); \
}
#define JIT_ASM_ARYSETn(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_ARYSETn, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_ARYSETn_); \
}
#define JIT_ASM_iARYSET(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_iARYSET, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_iARYSET_); \
}
#define JIT_ASM_iARYSETn(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_iARYSETn, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_iARYSETn_); \
}
#define JIT_ASM_fARYSET(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_fARYSET, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_fARYSET_); \
}
#define JIT_ASM_fARYSETn(ctx,a1,a2,a3) {\
	imem_setLabel(mem, op->opcode, OPSIZE_fARYSETn, 0); \
	jit_6__movq_imm32_r8d(mem,a3); \
	jit_5__movl_imm32_ecx(mem,a2); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_fARYSETn_); \
}
#define JIT_ASM_THCODE(ctx,a1) {\
	/*
	imem_setLabel(mem, op->opcode, OPSIZE_THCODE, 0); \
	jit_5__movl_imm32_edx(mem,a1); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_THCODE_); \
	*/\
}
#define JIT_ASM_NOP(ctx) {\
	imem_setLabel(mem, op->opcode, OPSIZE_NOP, 0); \
	jit_3__movq_rbx_rsi(mem); \
	jit_3__movq_r12_rdi(mem); \
	jit_5__call(mem, JIT_OP_NOP_); \
}
#endif
