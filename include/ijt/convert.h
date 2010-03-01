#ifndef IJT_CONVERT_H
#define IJT_CONVERT_H

#include "asm.h"
#define IJT_DBG_MODE 0
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

#define JIT_MOV__wogc(mem,a1,a2) {\
    int _a1 =  a1 * 0x10; \
    int _a2 =  a2 * 0x10; \
    jit_4__movq_rsi_at_rdx(mem, _a2);   \
    jit_4__incq_rdx_at(mem,0x18);       \
    jit_4__movq_rsi_at_rax(mem, _a1);   \
    jit_4__decq_rax_at(mem,0x18);       \
    jit_4__movq_rdx_rsi_to(mem,a1);     \
}

#define JIT_MOV_sfp(mem,a1,a2) {\
    jit_4__movq_rsi_at_r12(mem,_v1);    \
    jit_5__incq_r12_at(mem,0x18);       \
    jit_4__movq_rsi_at_rax(mem,_v2);    \
    jit_4__decq_rax_at(mem,0x18);       \
    jit_4__movq_rsi_at_rsi(mem,_v2);    \
    jit_5__cmp_imm8_rsi_at(mem,0x18);   \
    jit_2__jne_to(mem,0x05);            \
    jit_5__call(mem,knh_Object_free);   \
    jit_4__movq_r12_rbx_to(mem,0x20);   \
}

/*
#define KLR_MOV(ctx, v1, v2) {\
		Object *v2_ = (Object*)v2;\
		knh_Object_RCinc(v2_); \
		knh_Object_RCdec(v1); \
		if(knh_Object_isRC0(v1)) { \
			knh_Object_free(ctx, v1); \
		} \
		v1 = v2_; \
	}\
*/


#define JIT_ASM_HALT(ctx) {\
    imem_setLabel(mem, op->opcode, OPSIZE_HALT, 0); \
    IDEBUG("%-8s\n","HALT"); \
    jit_5__call(mem, JIT_OP_HALT_); \
    goto CTT_FIN;\
}\

#define JIT_ASM_MOVa(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_MOVA, 0); \
    IDEBUG("%-8s a1=%d,a2=%d\n","MOVa",a1,a2); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_MOVa_); \
}\

#define JIT_ASM_MOVn(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_MOVN, 0); \
    IDEBUG("%-8s a1=%d,a2=%d\n","MOVn",a1,a2); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_MOVn_); \
}\

#define JIT_ASM_MOVo(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_MOVO, 0); \
    IDEBUG("%-8s a1=%d,a2=%p\n","MOVo",a1,(void*)a2); \
    jit_10__movq_imm64_rcx(mem, (intptr_t)a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_MOVo_); \
}\

#define JIT_ASM_MOVx(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_MOVX, 0); \
    IDEBUG("%-8s a1=%d,a2=(%d,%zd)\n","MOVx",a1,a2.i,a2.n); \
    jit_6__movq_imm32_r8d(mem,a2.n); \
    jit_5__movl_imm32_ecx(mem,a2.i); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_MOVx_); \
}\

#define JIT_ASM_MOVDEF(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_MOVDEF, 0); \
    IDEBUG( "%-8s a1=%d,a2=%d\n","MOVDEF",a1,a2); \
    jit_5__movl_imm32_ecx(mem,(intptr_t)a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_MOVDEF_); \
}\

#define JIT_ASM_MOVSYS(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_MOVSYS, 0); \
    IDEBUG( "%-8s a1=%d,a2=%d\n","MOVSYS",a1,a2); \
    jit_5__movl_imm32_ecx(mem,(intptr_t)a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_MOVSYS_); \
}\

#define JIT_ASM_XMOVs(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_XMOVS, 0); \
    IDEBUG("%-8s a1=(%d,%zd),a2=%d\n","XMOVs",a1.i,a1.n,a2); \
    jit_6__movq_imm32_r8d(mem,a2); \
    jit_5__movl_imm32_ecx(mem,a1.n); \
    jit_5__movl_imm32_edx(mem,a1.i); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_XMOVs_); \
}\

#define JIT_ASM_XMOVo(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_XMOVO, 0); \
    IDEBUG("%-8s a1=(%d,%zd),a2=%p\n","XMOVo",a1.i,a1.n,(void*)a2); \
    jit_10__movq_imm64_r8(mem, (intptr_t)a2); \
    jit_5__movl_imm32_ecx(mem,a1.n); \
    jit_5__movl_imm32_edx(mem,a1.i); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_XMOVo_); \
}\

#define JIT_ASM_XMOVx(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_XMOVX, 0); \
    IDEBUG("%-8s a1=(%d,%zd),a2=(%d,%zd)\n","XMOVx",a1.i, \
            a1.n, a2.i,a2.n); \
    jit_6__movq_imm32_r9d(mem,a2.n); \
    jit_6__movq_imm32_r8d(mem,a2.i); \
    jit_5__movl_imm32_ecx(mem,a1.n); \
    jit_5__movl_imm32_edx(mem,a1.i); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
   jit_5__call(mem, JIT_OP_XMOVx_); \
}\

#define JIT_ASM_XMOVDEF(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_XMOVDEF, 0); \
    IDEBUG("%-8s a1=(%d,%zd),a2=%d\n","XMOVDEF",a1.i,a1.n,a2); \
    jit_6__movq_imm32_r8d(mem,a2); \
    jit_5__movl_imm32_ecx(mem,a1.n); \
    jit_5__movl_imm32_edx(mem,a1.i); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
   jit_5__call(mem, JIT_OP_XMOVDEF_); \
}\

#define JIT_ASM_XMOVSYS(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_XMOVSYS, 0); \
    IDEBUG("%-8s a1=(%d,%zd),a2=%d\n","XMOVSYS",a1.i,a1.n,a2); \
    jit_6__movq_imm32_r8d(mem,a2); \
    jit_5__movl_imm32_ecx(mem,a1.n); \
    jit_5__movl_imm32_edx(mem,a1.i); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_XMOVSYS_); \
}\

#define JIT_ASM_MOVxi(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_MOVXI, 0); \
    IDEBUG("%-8s a1=%d,a2=(%d,%zd)\n","MOVxi",a1,a2.i,a2.n); \
    jit_6__movq_imm32_r8d(mem,a2.n); \
    jit_5__movl_imm32_ecx(mem,a2.i); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_MOVxi_); \
}\

#define JIT_ASM_XMOVsi(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_XMOVSI, 0); \
    IDEBUG("%-8s a1=(%d,%zd),a2=%d\n","XMOVsi",a1.i,a1.n,a2); \
    jit_6__movq_imm32_r8d(mem,a2); \
    jit_5__movl_imm32_ecx(mem,a1.n); \
    jit_5__movl_imm32_edx(mem,a1.i); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_XMOVsi_); \
}\

#define JIT_ASM_XMOVoi(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_XMOVOI, 0); \
    IDEBUG("%-8s a1=(%d,%zd),a2=%p\n","XMOVoi",a1.i,a1.n,(void*)a2); \
    jit_10__movq_imm64_r8(mem, (intptr_t)a2); \
    jit_5__movl_imm32_ecx(mem,a1.n); \
    jit_5__movl_imm32_edx(mem,a1.i); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_XMOVoi_); \
}\

#define JIT_ASM_XMOVxi(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_XMOVXI, 0); \
    IDEBUG("%-8s a1=(%d,%zd),a2=(%d,%zd)\n","XMOVxi",a1.i,a1.n, \
            a2.i,a2.n); \
    jit_6__movq_imm32_r9d(mem,a2.n); \
    jit_6__movq_imm32_r8d(mem,a2.i); \
    jit_5__movl_imm32_ecx(mem,a1.n); \
    jit_5__movl_imm32_edx(mem,a1.i); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
   jit_5__call(mem, JIT_OP_XMOVxi_); \
}\

#define JIT_ASM_XMOVxio(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_XMOVXIO, 0); \
    IDEBUG("%-8s a1=(%d,%zd),a2=(%d,%zd)\n","XMOVxio",a1.i,a1.n, \
            a2.i,a2.n); \
    jit_6__movq_imm32_r9d(mem,a2.n); \
    jit_6__movq_imm32_r8d(mem,a2.i); \
    jit_5__movl_imm32_ecx(mem,a1.n); \
    jit_5__movl_imm32_edx(mem,a1.i); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
   jit_5__call(mem, JIT_OP_XMOVxio_); \
}\

#define JIT_ASM_XMOVxBXi(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_XMOVXBXI, 0); \
    IDEBUG("%-8s a1=(%d,%zd),a2=(%d,%zd),a3=%d\n","XMOVxBXi",a1.i,\
            a1.n, a2.i,a2.n,a3); \
    jit_7__movq_imm32_rsp(mem,a3);   \
    jit_6__movq_imm32_r9d(mem,a2.n); \
    jit_6__movq_imm32_r8d(mem,a2.i); \
    jit_5__movl_imm32_ecx(mem,a1.n); \
    jit_5__movl_imm32_edx(mem,a1.i); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_XMOVxBXi_); \
}\

#define JIT_ASM_MOVxf(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_MOVXF, 0); \
    IDEBUG("%-8s a1=%d,a2=(%d,%zd)\n","MOVxf",a1,a2.i,a2.n); \
    jit_6__movq_imm32_r8d(mem,a2.n); \
    jit_5__movl_imm32_ecx(mem,a2.i); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_MOVxf_); \
}\

#define JIT_ASM_XMOVsf(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_XMOVSF, 0); \
    IDEBUG("%-8s a1=(%d,%zd),a2=%d\n","XMOVsf",a1.i,a1.n,a2); \
    jit_6__movq_imm32_r8d(mem,a2); \
    jit_5__movl_imm32_ecx(mem,a1.n); \
    jit_5__movl_imm32_edx(mem,a1.i); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_XMOVsf_); \
}\

#define JIT_ASM_XMOVof(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_XMOVOF, 0); \
    IDEBUG("%-8s a1=(%d,%zd),a2=%p\n","XMOVof",a1.i,a1.n,(void*)a2); \
    jit_10__movq_imm64_r8(mem, (intptr_t)a2); \
    jit_5__movl_imm32_ecx(mem,a1.n); \
    jit_5__movl_imm32_edx(mem,a1.i); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
   jit_5__call(mem, JIT_OP_XMOVof_); \
}\

#define JIT_ASM_XMOVxf(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_XMOVXF, 0); \
     IDEBUG("%-8s a1=(%d,%zd),a2=(%d,%zd)\n","XMOVxf",a1.i, \
            a1.n, a2.i,a2.n); \
    jit_6__movq_imm32_r9d(mem,a2.n); \
    jit_6__movq_imm32_r8d(mem,a2.i); \
    jit_5__movl_imm32_ecx(mem,a1.n); \
    jit_5__movl_imm32_edx(mem,a1.i); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
   jit_5__call(mem, JIT_OP_XMOVxf_); \
}\

#define JIT_ASM_XMOVxfo(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_XMOVXFO, 0); \
     IDEBUG("%-8s a1=(%d,%zd),a2=(%d,%zd)\n","XMOVxfo",a1.i, \
            a1.n, a2.i,a2.n); \
    jit_6__movq_imm32_r9d(mem,a2.n); \
    jit_6__movq_imm32_r8d(mem,a2.i); \
    jit_5__movl_imm32_ecx(mem,a1.n); \
    jit_5__movl_imm32_edx(mem,a1.i); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
   jit_5__call(mem, JIT_OP_XMOVxfo_); \
}\

#define JIT_ASM_XMOVxBXf(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_XMOVXBXF, 0); \
     IDEBUG("%-8s a1=(%d,%zd),a2=(%d,%zd),a3=%d\n","XMOVxBXf",a1.i,\
            a1.n, a2.i,a2.n,a3); \
    jit_7__movq_imm32_rsp(mem,a3);   \
    jit_6__movq_imm32_r9d(mem,a2.n); \
    jit_6__movq_imm32_r8d(mem,a2.i); \
    jit_5__movl_imm32_ecx(mem,a1.n); \
    jit_5__movl_imm32_edx(mem,a1.i); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
   jit_5__call(mem, JIT_OP_XMOVxBXf_); \
}\

#define JIT_ASM_MOVxb(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_MOVXB, 0); \
    IDEBUG("%-8s a1=%d,a2=(%d,%zd)\n","MOVxb",a1,a2.i,a2.n); \
    jit_6__movq_imm32_r8d(mem,a2.n); \
    jit_5__movl_imm32_ecx(mem,a2.i); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
   jit_5__call(mem, JIT_OP_MOVxb_); \
}\

#define JIT_ASM_XMOVsb(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_XMOVSB, 0); \
    IDEBUG("%-8s a1=(%d,%zd),a2=%d\n","XMOVsb",a1.i,a1.n,a2); \
    jit_6__movq_imm32_r8d(mem,a2); \
    jit_5__movl_imm32_ecx(mem,a1.n); \
    jit_5__movl_imm32_edx(mem,a1.i); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
   jit_5__call(mem, JIT_OP_XMOVsb_); \
}\

#define JIT_ASM_XMOVob(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_XMOVOB, 0); \
    IDEBUG("%-8s a1=(%d,%zd),a2=%p\n","XMOVob",a1.i,a1.n,(void*)a2); \
    jit_10__movq_imm64_r8(mem, (intptr_t)a2); \
    jit_5__movl_imm32_ecx(mem,a1.n); \
    jit_5__movl_imm32_edx(mem,a1.i); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_XMOVob_); \
}\

#define JIT_ASM_XMOVxb(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_XMOVXB, 0); \
    IDEBUG("%-8s a1=(%d,%zd),a2=(%d,%zd)\n","XMOVxb",a1.i,\
            a1.n, a2.i,a2.n); \
    jit_6__movq_imm32_r9d(mem,a2.n); \
    jit_6__movq_imm32_r8d(mem,a2.i); \
    jit_5__movl_imm32_ecx(mem,a1.n); \
    jit_5__movl_imm32_edx(mem,a1.i); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
   jit_5__call(mem, JIT_OP_XMOVxb_); \
}\

#define JIT_ASM_EMOVs(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_EMOVS, 0); \
    IDEBUG("%-8s a1=%d,a2=%d\n","EMOVs",a1,a2); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_EMOVs_); \
}\

#define JIT_ASM_EMOVo(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_EMOVO, 0); \
    IDEBUG("%-8s a1=%d,a2=%p\n","EMOVo",a1,(void*)a2); \
    jit_10__movq_imm64_rcx(mem, (intptr_t)a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_EMOVo_); \
}\

#define JIT_ASM_EMOVe(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_EMOVE, 0); \
    IDEBUG("%-8s a1=%d,a2=%d\n","EMOVe",a1,a2); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_EMOVe_); \
}\

#define JIT_ASM_EMOVDEF(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_EMOVDEF, 0); \
    IDEBUG("%-8s a1=%d,a2=%d\n","EMOVDEF",a1,a2); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_EMOVDEF_); \
}\

#define JIT_ASM_EMOVSYS(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_EMOVSYS, 0); \
    IDEBUG("%-8s a1=%d,a2=%d\n","EMOVSYS",a1,a2); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_EMOVSYS_); \
}\

#define JIT_ASM_SWAP(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_SWAP, 0); \
    IDEBUG( "%-8s\n","SWAP"); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_SWAP_); \
}\

#define JIT_ASM_PINIDEF(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_PINIDEF, 0); \
    IDEBUG( "%-8s a1=%d,a2=%d\n","PINIDEF",a1,a2); \
    jit_5__movl_imm32_ecx(mem,(intptr_t)a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_PINIDEF_); \
}\

#define JIT_ASM_PINIo(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_PINIO, 0); \
    IDEBUG( "%-8s a1=%d,a2=%p\n","PINIo",a1,(void*)a2); \
    jit_10__movq_imm64_rcx(mem, (intptr_t)a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_PINIo_); \
}\

#define JIT_ASM_PARAMS(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_PARAMS, 0); \
    IDEBUG( "%-8s a1=%d,a2=%d\n","PARAMS",a1,a2); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_PARAMS_); \
}\

#define JIT_ASM_SETESP(ctx, a1) {\
    imem_setLabel(mem, op->opcode, OPSIZE_SETESP, 0); \
    IDEBUG( "%-8s a1=%d\n","SETESP",a1); \
    jit_5__movl_imm32_edx(mem,a1); \
    /*jit_3__movq_rbx_rsi(mem); */\
    /*jit_3__movq_r12_rdi(mem); */\
    jit_5__call(mem, JIT_OP_SETESP_); \
}\

#define JIT_ASM_CHECKESP(ctx, a1) {\
    imem_setLabel(mem, op->opcode, OPSIZE_CHECKESP, 0); \
    IDEBUG( "%-8s a1=%d\n","CHECKESP",a1); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_CHECKESP_); \
}\

#define JIT_ASM_RET(ctx) {\
    imem_setLabel(mem, op->opcode, OPSIZE_RET, 0); \
    IDEBUG( "%-8s\n","RET"); \
    /*imem_push(mem,0xcc);*/ \
    jit_complete(mem);\
}\

#define JIT_ASM_RETn(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_RETN, 0); \
    IDEBUG( "%-8s a1=%d\n","RETn",a1); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_RETn_); \
    jit_complete(mem);\
}\

#define JIT_ASM_RETa(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_RETA, 0); \
    IDEBUG( "%-8s a1=%d,a2=%d\n","RETa",a1,a2); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_RETa_); \
    jit_complete(mem);\
}\

#define JIT_ASM_RETo(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_RETO, 0); \
    IDEBUG("%-8s a1=%d,a2=%p\n","RETo",a1,(void*)a2); \
    jit_10__movq_imm64_rcx(mem, (intptr_t)a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_RETo_); \
    jit_complete(mem);\
}\

#define JIT_ASM_RETx(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_RETX, 0); \
    IDEBUG("%-8s a1=%d,a2=(%d,%zd)\n","RETx",a1,a2.i,a2.n); \
    jit_6__movq_imm32_r8d(mem,a2.n); \
    jit_5__movl_imm32_ecx(mem,a2.i); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_RETx_); \
    jit_complete(mem);\
}\

#define JIT_ASM_YEILDBREAK(ctx) {\
    imem_setLabel(mem, op->opcode, OPSIZE_YEILDBREAK, 0); \
    IDEBUG( "%-8s\n","YEILDBREAK"); \
    /*jit_5__call(mem, JIT_OP_YEILDBREAK_);*/ \
    /*jit_1__ret(mem); */\
    goto CTT_FIN;\
}\

#define JIT_ASM_BOX(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_BOX, 0); \
    IDEBUG( "%-8s a1=%d,a2=%d\n","BOX",a1,a2); \
    jit_5__movl_imm32_ecx(mem,(intptr_t)a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_BOX_); \
}\

#define JIT_ASM_BOXnc(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_BOXNC, 0); \
    IDEBUG( "%-8s a1=%d,a2=%d\n","BOXnc",a1,a2); \
    jit_5__movl_imm32_ecx(mem,(intptr_t)a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_BOXnc_); \
}\

#define JIT_ASM_NNBOX(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_NNBOX, 0); \
    IDEBUG( "%-8s a1=%d,a2=%d\n","NNBOX",a1,a2); \
    jit_5__movl_imm32_ecx(mem,(intptr_t)a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_NNBOX_); \
}\

#define JIT_ASM_NNBOXnc(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_NNBOXNC, 0); \
    IDEBUG( "%-8s a1=%d,a2=%d\n","NNBOXnc",a1,a2); \
    jit_5__movl_imm32_ecx(mem,(intptr_t)a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_NNBOXnc_); \
}\

#define JIT_ASM_UNBOX(ctx, a1) {\
    imem_setLabel(mem, op->opcode, OPSIZE_UNBOX, 0); \
    IDEBUG( "%-8s a1=%d\n","UNBOX",a1); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_UNBOX_); \
}\

#define JIT_ASM_CHECKNULL(ctx, a1) {\
    imem_setLabel(mem, op->opcode, OPSIZE_CHECKNULL, 0); \
    IDEBUG( "%-8s a1=%d\n","CHECKNULL",a1); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_CHECKNULL_); \
}\

#define JIT_ASM_CHECKNULLx(ctx, a1) {\
    imem_setLabel(mem, op->opcode, OPSIZE_CHECKNULLX, 0); \
    IDEBUG("%-8s a1=(%d,%zd)\n","CHECKNULLx",a1.i,a1.n); \
    jit_5__movl_imm32_ecx(mem,a1.n); \
    jit_5__movl_imm32_edx(mem,a1.i); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_CHECKNULLx_); \
}\

#define JIT_ASM_CHECKTYPE(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_CHECKTYPE, 0); \
    IDEBUG( "%-8s a1=%d,a2=%d\n","CHECKTYPE",a1,a2); \
    jit_5__movl_imm32_ecx(mem,(intptr_t)a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_CHECKTYPE_); \
}\

#define JIT_ASM_CHECKNNTYPE(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_CHECKNNTYPE, 0); \
    IDEBUG( "%-8s a1=%d,a2=%d\n","CHECKNNTYPE",a1,a2); \
    jit_5__movl_imm32_ecx(mem,(intptr_t)a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_CHECKNNTYPE_); \
}\

#define JIT_ASM_FCALL(ctx, a1, a2, a3, a4) {\
    imem_setLabel(mem, op->opcode, OPSIZE_FCALL, 0); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%d,a4=%p\n","FCALL",a1,a2,a3, \
             (void*) a4); \
    jit_10__movq_imm64_r9(mem,(intptr_t)a4); \
    jit_6__movq_imm32_r8d(mem,a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
   jit_5__call(mem, JIT_OP_FCALL_); \
}\

#define JIT_ASM_SCALL(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_SCALL, 0); \
    IDEBUG( "%-8s a1=%d,a2=%d,a3=%p\n","SCALL",\
            a1,a2,(void*)a3); \
    jit_10__movq_imm64_r8(mem, (intptr_t)a3); \
    jit_5__movl_imm32_ecx(mem,(intptr_t)a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_SCALL_); \
}\

#define JIT_ASM_AINVOKE(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_AINVOKE, 0); \
    IDEBUG( "%-8s a1=%d,a2=%d\n","AINVOKE",a1,a2); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_AINVOKE_); \
}\

#define JIT_ASM_CALL(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_CALL, 0); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%d\n","CALL",a1,a2,a3); \
    jit_6__movq_imm32_r8d(mem,a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_CALL_); \
}\

#define JIT_ASM_ACALL(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_ACALL, 0); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%d\n","ACALL",a1,a2,a3); \
    jit_6__movq_imm32_r8d(mem,a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_ACALL_); \
}\

// FIXME a5 is not imm32, also imm64.
// we have to find how to put imm64 data to rsp.
#define JIT_ASM_NEW(ctx, a1, a2, a3, a4, a5) {\
    imem_setLabel(mem, op->opcode, OPSIZE_NEW, 0); \
    IDEBUG( "%-8s a1=%d,a2=%d,a3=%d,a4=%d,a5=%p\n","NEW",a1,a2,a3,\
            a4,(void*)a5); \
    jit_7__movq_imm32_rsp(mem,(intptr_t)a5);   \
    jit_6__movq_imm32_r9d(mem,a4); \
    jit_6__movq_imm32_r8d(mem,a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_5__call(mem, JIT_OP_NEW_); \
}\

#define JIT_ASM_TOSTR(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_TOSTR, 0); \
    IDEBUG("%-8s a1=%d,a2=%d\n","TOSTR",a1,a2); \
    jit_5__movl_imm32_ecx(mem,(intptr_t)a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_TOSTR_); \
}\

#define JIT_ASM_TOSTRf(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_TOSTRF, 0); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%p\n","TOSTRf",\
            a1,a2,(void*)a3); \
    jit_10__movq_imm64_r8(mem, (intptr_t)a3); \
    jit_5__movl_imm32_ecx(mem,(intptr_t)a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_TOSTRf_); \
}\

#define JIT_ASM_SMAP(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_SMAP, 0); \
    IDEBUG("%-8s a1=%d,a2=%p\n","SMAP",a1,(void*)a2); \
    jit_10__movq_imm64_rcx(mem, (intptr_t)a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_SMAP_); \
}\

#define JIT_ASM_SMAPnc(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_SMAPNC, 0); \
    IDEBUG( "%-8s\n","SMAPnc"); \
    jit_10__movq_imm64_rcx(mem, (intptr_t)a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_SMAPnc_); \
}\

#define JIT_ASM_MAP(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_MAP, 0); \
    IDEBUG("%-8s a1=%d,a2=%d\n","MAP",a1,a2); \
    jit_5__movl_imm32_ecx(mem,(intptr_t)a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_MAP_); \
}\

#define JIT_ASM_MAPnc(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_MAPNC, 0); \
    IDEBUG("%-8s a1=%d,a2=%d\n","MAPnc",a1,a2); \
    jit_5__movl_imm32_ecx(mem,(intptr_t)a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_MAPnc_); \
}\

#define JIT_ASM_AMAP(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_AMAP, 0); \
    IDEBUG("%-8s a1=%d,a2=%d\n","AMAP",a1,a2); \
    jit_5__movl_imm32_ecx(mem,(intptr_t)a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_AMAP_); \
}\

#define JIT_ASM_NNMAP(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_NNMAP, 0); \
    IDEBUG("%-8s a1=%d,a2=%d\n","NNMAP",a1,a2); \
    jit_5__movl_imm32_ecx(mem,(intptr_t)a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_NNMAP_); \
}\

#define JIT_ASM_JMP(ctx, jumppc, label) {\
    imem_setLabel(mem, op->opcode, OPSIZE_JMP, (jumppc - pc)); \
    IDEBUG( "%-8s jumppc=%p,label=%d\n","JMP",(void*)jumppc,label); \
    jit_5__call(mem, JIT_OP_JMP_); \
}\

#define JIT_ASM_SKIP(ctx, jumppc, label) {\
    imem_setLabel(mem, op->opcode, OPSIZE_SKIP, (jumppc - pc)); \
    IDEBUG( "%-8s jumppc=%p,label=%d\n","SKIP",(void*)jumppc,label); \
    jit_5__call(mem, JIT_OP_SKIP_); \
}\

#define JIT_ASM_bJIFT(ctx, jumppc, label, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_BJIFT, (jumppc - pc)); \
    IDEBUG( "%-8s jumppc=%p,label=%d,a2=%d\n","bJIFT",(void*)jumppc, \
            label,a2); \
    jit_5__call(mem, JIT_OP_bJIFT_); \
}\

#define JIT_ASM_bJIFF(ctx, jmppc, label, a2) {\
    IDEBUG("%-8s jumppc=%p,label=%d,a2=%d\n","bJIFF",(void*)(jmppc-pc),\
            label,a2); \
    int _tmp =  0x08 + a2 * 0x10; \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    if(a2 < 8) { \
        jit_5__cmp_imm8_rsi_at(mem,1,_tmp); \
    } else { \
        jit_8__cmp_imm8_rsi_at(mem,1,_tmp); \
    } \
    imem_setLabel(mem, op->opcode, OPSIZE_BJIFF, (jmppc - pc)); \
    jit_2__jne_to(mem, 0); \
}\

#define JIT_ASM_bJIFF_LOOP(ctx, jumppc, label, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_BJIFF,  (jumppc - pc)); \
    IDEBUG( "%-8s jumppc=%p,label=%d,a2=%d\n","bJIFF_LOOP",\
            (void*)jumppc, label,a2); \
    jit_5__call(mem, JIT_OP_bJIFF_LOOP_); \
}\

#define JIT_ASM_JIFNUL(ctx, jumppc, label, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_JIFNUL, (jumppc - pc)); \
    IDEBUG( "%-8s\n","JIFNUL"); \
    jit_5__call(mem, JIT_OP_JIFNUL_); \
}\

#define JIT_ASM_JIFNN(ctx, jumppc, label, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_JIFNN, (jumppc - pc)); \
    IDEBUG( "%-8s\n","JIFNN"); \
    jit_5__call(mem, JIT_OP_JIFNN_); \
}\

#define JIT_ASM_NEXT(ctx, jumppc, label, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_NEXT, (jumppc - pc)); \
    IDEBUG( "%-8s\n","NEXT"); \
    jit_5__call(mem, JIT_OP_NEXT_); \
}\

#define JIT_ASM_INEXT(ctx, jumppc, label, a2, a3, a4) {\
    imem_setLabel(mem, op->opcode, OPSIZE_INEXT, (jumppc - pc)); \
    IDEBUG( "%-8s\n","INEXT"); \
    jit_5__call(mem, JIT_OP_INEXT_); \
}\

#define JIT_ASM_MAPNEXT(ctx, jumppc, label, a2, a3, a4) {\
    imem_setLabel(mem, op->opcode, OPSIZE_MAPNEXT, (jumppc - pc)); \
    IDEBUG( "%-8s\n","MAPNEXT"); \
    jit_5__call(mem, JIT_OP_MAPNEXT_); \
}\

#define JIT_ASM_SMAPNEXT(ctx, jumppc, label, a2, a3, a4) {\
    imem_setLabel(mem, op->opcode, OPSIZE_SMAPNEXT, (jumppc - pc)); \
    IDEBUG( "%-8s\n","SMAPNEXT"); \
    jit_5__call(mem, JIT_OP_SMAPNEXT_); \
}\

#define JIT_ASM_TRY(ctx, jumppc, label, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_TRY, (jumppc - pc)); \
    IDEBUG( "%-8s\n","TRY"); \
    jit_5__call(mem, JIT_OP_TRY_); \
}\

#define JIT_ASM_TRYEND(ctx, a1) {\
    imem_setLabel(mem, op->opcode, OPSIZE_TRYEND, 0); \
    IDEBUG( "%-8s\n","TRYEND"); \
    jit_5__call(mem, JIT_OP_TRYEND_); \
}\

#define JIT_ASM_CATCH(ctx, jumppc, label, a2, a3, a4) {\
    imem_setLabel(mem, op->opcode, OPSIZE_CATCH, (jumppc - pc)); \
    IDEBUG( "%-8s\n","CATCH"); \
    jit_5__call(mem, JIT_OP_CATCH_); \
}\

#define JIT_ASM_PUSH(ctx, a1) {\
    imem_setLabel(mem, op->opcode, OPSIZE_PUSH, 0); \
    IDEBUG( "%-8s\n","PUSH a1=%d"); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_PUSH_); \
}\

#define JIT_ASM_POP(ctx, a1) {\
    imem_setLabel(mem, op->opcode, OPSIZE_POP, 0); \
    IDEBUG( "%-8s\n","POP a1=%d"); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_POP_); \
}\

#define JIT_ASM_THROW(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_THROW, 0); \
    IDEBUG("%-8s a1=%d,a2=%d\n","THROW",a1,a2); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,(intptr_t)a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_THROW_); \
}\

#define JIT_ASM_THROWs(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_THROWS, 0); \
    IDEBUG("%-8s a1=%d,a2=%p\n","THROWs",a1,(void*)a2); \
    jit_10__movq_imm64_rcx(mem, (intptr_t)a2); \
    jit_5__movl_imm32_edx(mem,(intptr_t)a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_THROWs_); \
}\

#define JIT_ASM_THROW_AGAIN(ctx, a1) {\
    imem_setLabel(mem, op->opcode, OPSIZE_THROW, 0); \
    IDEBUG( "%-8s\n","THROW_AGAIN"); \
    IDEBUG("%-8s a1=%d\n","THROW_AGAIN",a1); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_THROW_AGAIN_); \
}\

#define JIT_ASM_P(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_P, 0); \
    IDEBUG( "%-8s\n","P"); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%d\n","P",a1,a2,a3); \
    jit_6__movq_imm32_r8d(mem,a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
   jit_5__call(mem, JIT_OP_P_); \
}\

#define JIT_ASM_PMSG(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_PMSG, 0); \
    IDEBUG("%-8s a1=%d,a2=%p\n","PMSG",a1,(void*)a2); \
    jit_10__movq_imm64_rcx(mem, (intptr_t)a2); \
    jit_5__movl_imm32_edx(mem,(intptr_t)a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_PMSG_); \
}\

#define JIT_ASM_iCAST(ctx, a1) {\
    imem_setLabel(mem, op->opcode, OPSIZE_ICAST, 0); \
    IDEBUG( "%-8s\n","iCAST"); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_iCAST_); \
}\

#define JIT_ASM_inCAST(ctx, a1) {\
    imem_setLabel(mem, op->opcode, OPSIZE_INCAST, 0); \
    IDEBUG( "%-8s\n","inCAST"); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_inCAST_); \
}\

#define JIT_ASM_fCAST(ctx, a1) {\
    imem_setLabel(mem, op->opcode, OPSIZE_FCAST, 0); \
    IDEBUG( "%-8s\n","fCAST"); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_fCAST_); \
}\

#define JIT_ASM_fnCAST(ctx, a1) {\
    imem_setLabel(mem, op->opcode, OPSIZE_FNCAST, 0); \
    IDEBUG( "%-8s\n","fnCAST"); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_fnCAST_); \
}\

#define JIT_ASM_bNOT(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_BNOT, 0); \
    IDEBUG( "%-8s\n","bNOT"); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_bNOT_); \
}\

#define JIT_ASM_iNEG(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_INEG, 0); \
    IDEBUG( "%-8s\n","iNEG"); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_iNEG_); \
}\

#define JIT_ASM_iADD(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_IADD, 0); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%d\n","iADD",a1,a2,a3); \
    int _a1 =  0x08 + a1 * 0x10; \
    int _a2 =  0x08 + a2 * 0x10; \
    int _a3 =  0x08 + a3 * 0x10; \
    jit_3__movq_rbx_rsi(mem); \
    jit_4__movq_rsi_at_rax(mem,_a2); \
    jit_4__addq_rsi_at_rax(mem,_a3); \
    jit_4__movq_rax_rsi_to(mem,_a1); \
}\

#define JIT_ASM_iADDn(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_IADDN, 0); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%lld\n","iADDn",a1,a2,a3); \
    jit_6__movq_imm32_r8d(mem,(intptr_t)a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_iADDn_); \
}\

#define JIT_ASM_iSUB(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_ISUB, 0); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%d\n","iSUB",a1,a2,a3); \
    jit_6__movq_imm32_r8d(mem,a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_iSUB_); \
}\

#define JIT_ASM_iSUBn(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_ISUBN, 0); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%lld\n","iSUBn",a1,a2,a3); \
    int _a1 =  0x08 + a1 * 0x10; \
    int _a2 =  0x08 + a2 * 0x10; \
    jit_3__movq_rbx_rsi(mem); \
    jit_4__movq_rsi_at_rax(mem,_a2); \
    jit_6__subq_imm32_eax(mem,a3); \
    jit_4__movq_rax_rsi_to(mem,_a1); \
    \
}\

#define JIT_ASM_iMUL(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_IMUL, 0); \
    IDEBUG( "%-8s\n","iMUL"); \
    int _a1 =  0x08 + a1 * 0x10; \
    int _a2 =  0x08 + a2 * 0x10; \
    int _a3 =  0x08 + a3 * 0x10; \
    jit_3__movq_rbx_rsi(mem); \
    jit_4__movq_rsi_at_rax(mem,_a2); \
    jit_5__imulq_rsi_at_rax(mem,_a3); \
    jit_4__movq_rax_rsi_to(mem,_a1); \
}\

#define JIT_ASM_iMULn(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_IMULN, 0); \
    IDEBUG( "%-8s\n","iMULn"); \
    jit_6__movq_imm32_r8d(mem,(intptr_t)a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_iMULn_); \
}\

#define JIT_ASM_iDIV(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_IDIV, 0); \
    IDEBUG( "%-8s\n","iDIV"); \
    jit_6__movq_imm32_r8d(mem,a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_iDIV_); \
}\

#define JIT_ASM_iDIVn(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_IDIVN, 0); \
    IDEBUG( "%-8s\n","iDIVn"); \
    jit_6__movq_imm32_r8d(mem,(intptr_t)a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_iDIVn_); \
}\

#define JIT_ASM_iMOD(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_IMOD, 0); \
    IDEBUG( "%-8s\n","iMOD"); \
    jit_6__movq_imm32_r8d(mem,a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_iMOD_); \
}\

#define JIT_ASM_iMODn(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_IMODN, 0); \
    IDEBUG( "%-8s\n","iMODn"); \
    jit_6__movq_imm32_r8d(mem,(intptr_t)a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_iMODn_); \
}\

#define JIT_ASM_iEQ(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_IEQ, 0); \
    IDEBUG( "%-8s a1=%d,a2=%d,a3=%d\n","iEQ",a1,a2,a3); \
    jit_6__movq_imm32_r8d(mem,a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_iEQ_); \
}\

#define JIT_ASM_iEQn(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_IEQN, 0); \
    IDEBUG( "%-8s a1=%d,a2=%d,a3=%lld\n","iEQn",a1,a2,a3); \
    jit_6__movq_imm32_r8d(mem,(intptr_t)a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_iEQn_); \
}\

#define JIT_ASM_iNEQ(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_INEQ, 0); \
    IDEBUG( "%-8s\n","iNEQ"); \
    jit_6__movq_imm32_r8d(mem,a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_iNEQ_); \
}\

#define JIT_ASM_iNEQn(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_INEQN, 0); \
    IDEBUG( "%-8s\n","iNEQn"); \
    jit_6__movq_imm32_r8d(mem,(intptr_t)a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_iNEQn_); \
}\

#define JIT_ASM_iLT(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_ILT, 0); \
    IDEBUG( "%-8s,a1=%d,a2=%d,a3=%d\n","iLT",a1,a2,a3); \
    jit_6__movq_imm32_r8d(mem,a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_iLT_); \
}\

#define JIT_ASM_iLTn(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_ILTN, 0); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%lld\n","iLTn",a1,a2,a3); \
    int _a1 =  0x08 + a1 * 0x10; \
    int _a2 =  0x08 + a2 * 0x10; \
    jit_3__movq_rbx_rsi(mem); \
    jit_10__movq_imm64_rax(mem,a3); \
    jit_4__cmp_rax_rsi_at(mem,_a2); \
    jit_3__setl_al(mem); \
    jit_3__movzbl_al_eax(mem); \
    jit_4__movq_rax_rsi_to(mem,_a1); \
}\

#define JIT_ASM_iLTE(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_ILTE, 0); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%d\n","iLTE",a1,a2,a3); \
    jit_6__movq_imm32_r8d(mem,a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_iLTE_); \
}\

#define JIT_ASM_iLTEn(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_ILTEN, 0); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%lld\n","iLTEn",a1,a2,a3); \
    jit_6__movq_imm32_r8d(mem,(intptr_t)a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_iLTEn_); \
}\

#define JIT_ASM_iGT(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_IGT, 0); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%d\n","iGT",a1,a2,a3); \
    jit_6__movq_imm32_r8d(mem,a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_iGT_); \
}\

#define JIT_ASM_iGTn(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_IGTN, 0); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%lld\n","iGTn",a1,a2,a3); \
    jit_6__movq_imm32_r8d(mem,(intptr_t)a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_iGTn_); \
}\

#define JIT_ASM_iGTE(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_IGTE, 0); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%d\n","iGTE",a1,a2,a3); \
    jit_6__movq_imm32_r8d(mem,a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_iGTE_); \
}\

#define JIT_ASM_iGTEn(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_IGTEN, 0); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%lld\n","iGTEn",a1,a2,a3); \
    jit_6__movq_imm32_r8d(mem,(intptr_t)a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_iGTEn_); \
}\

#define JIT_ASM_fNEG(ctx, a1, a2) {\
    imem_setLabel(mem, op->opcode, OPSIZE_FNEG, 0); \
    IDEBUG( "%-8s\n","fNEG"); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_fNEG_); \
}\

#define JIT_ASM_fADD(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_FADD, 0); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%d\n","fADD",a1,a2,a3); \
    jit_6__movq_imm32_r8d(mem,a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_fADD_); \
}\

#define JIT_ASM_fADDn(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_FADDN, 0); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%f\n","fADDn",a1,a2,a3); \
    jit_10__movq_imm64_r8(mem, (intptr_t)a3); \
    jit_5__cvtsi2ss_r8d_xmm0(mem); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_fADDn_); \
}\

#define JIT_ASM_fSUB(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_FSUB, 0); \
    IDEBUG( "%-8s\n","fSUB"); \
    jit_6__movq_imm32_r8d(mem,a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_fSUB_); \
}\

#define JIT_ASM_fSUBn(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_FSUBN, 0); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%f\n","fSUBn",a1,a2,a3); \
    jit_10__movq_imm64_r8(mem, (intptr_t)a3); \
    jit_5__cvtsi2ss_r8d_xmm0(mem); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_fSUBn_); \
}\

#define JIT_ASM_fMUL(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_FMUL, 0); \
    IDEBUG( "%-8s\n","fMUL"); \
    jit_6__movq_imm32_r8d(mem,a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_fMUL_); \
}\

#define JIT_ASM_fMULn(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_FMULN, 0); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%f\n","fMULn",a1,a2,a3); \
    jit_10__movq_imm64_r8(mem, (intptr_t)a3); \
    jit_5__cvtsi2ss_r8d_xmm0(mem); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_fMULn_); \
}\

#define JIT_ASM_fDIV(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_FDIV, 0); \
    IDEBUG( "%-8s\n","fDIV"); \
    jit_6__movq_imm32_r8d(mem,a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_fDIV_); \
}\

#define JIT_ASM_fDIVn(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_FDIVN, 0); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%f\n","fDIVn",a1,a2,a3); \
    jit_10__movq_imm64_r8(mem, (intptr_t)a3); \
    jit_5__cvtsi2ss_r8d_xmm0(mem); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_fDIVn_); \
}\

#define JIT_ASM_fEQ(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_FEQ, 0); \
    IDEBUG( "%-8s\n","fEQ"); \
    jit_6__movq_imm32_r8d(mem,a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_fEQ_); \
}\

#define JIT_ASM_fEQn(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_FEQN, 0); \
    IDEBUG( "%-8s\n","fEQn"); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%f\n","fEQn",a1,a2,a3); \
    jit_10__movq_imm64_r8(mem, (intptr_t)a3); \
    jit_5__cvtsi2ss_r8d_xmm0(mem); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_fEQn_); \
}\

#define JIT_ASM_fNEQ(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_FNEQ, 0); \
    IDEBUG( "%-8s\n","fNEQ"); \
    jit_6__movq_imm32_r8d(mem,a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_fNEQ_); \
}\

#define JIT_ASM_fNEQn(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_FNEQN, 0); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%f\n","fNEQn",a1,a2,a3); \
    jit_10__movq_imm64_r8(mem, (intptr_t)a3); \
    jit_5__cvtsi2ss_r8d_xmm0(mem); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_fNEQn_); \
}\

#define JIT_ASM_fLT(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_FLT, 0); \
    IDEBUG( "%-8s\n","fLT"); \
    jit_6__movq_imm32_r8d(mem,a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_fLT_); \
}\

#define JIT_ASM_fLTn(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_FLTN, 0); \
    IDEBUG( "%-8s\n","fLTn"); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%f\n","fLTn",a1,a2,a3); \
    jit_10__movq_imm64_r8(mem, (intptr_t)a3); \
    jit_5__cvtsi2ss_r8d_xmm0(mem); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_fLTn_); \
}\

#define JIT_ASM_fLTE(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_FLTE, 0); \
    IDEBUG( "%-8s\n","fLTE"); \
    jit_6__movq_imm32_r8d(mem,a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_fLTE_); \
}\

#define JIT_ASM_fLTEn(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_FLTEN, 0); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%f\n","fLTEn",a1,a2,a3); \
    jit_10__movq_imm64_r8(mem, (intptr_t)a3); \
    jit_5__cvtsi2ss_r8d_xmm0(mem); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_fLTEn_); \
}\

#define JIT_ASM_fGT(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_FGT, 0); \
    IDEBUG( "%-8s\n","fGT"); \
    jit_6__movq_imm32_r8d(mem,a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_fGT_); \
}\

#define JIT_ASM_fGTn(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_FGTN, 0); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%f\n","fGTn",a1,a2,a3); \
    jit_10__movq_imm64_r8(mem, (intptr_t)a3); \
    jit_5__cvtsi2ss_r8d_xmm0(mem); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_fGTn_); \
}\

#define JIT_ASM_fGTE(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_FGTE, 0); \
    IDEBUG( "%-8s\n","fGTE"); \
    jit_6__movq_imm32_r8d(mem,a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_fGTE_); \
}\

#define JIT_ASM_fGTEn(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_FGTEN, 0); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%f\n","fGTEn",a1,a2,a3); \
    jit_10__movq_imm64_r8(mem, (intptr_t)a3); \
    jit_5__cvtsi2ss_r8d_xmm0(mem); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_fGTEn_); \
}\

#define JIT_ASM_AGET(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_AGET, 0); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%d\n","AGET",a1,a2,a3); \
    jit_6__movq_imm32_r8d(mem,a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_AGET_); \
}\

#define JIT_ASM_AGETn(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_AGETN, 0); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%d\n","AGETn",a1,a2,(int)a3); \
    jit_6__movq_imm32_r8d(mem,a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_AGETn_); \
}\

#define JIT_ASM_IAGET(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_IAGET, 0); \
    IDEBUG( "%-8s a1=%d,a2=%d,a3=%d\n","IAGET",a1,a2,a3); \
    jit_6__movq_imm32_r8d(mem,a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_IAGET_); \
}\

#define JIT_ASM_IAGETn(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_IAGETN, 0); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%d\n","IAGETn",a1,a2,(int)a3); \
    jit_6__movq_imm32_r8d(mem,a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_IAGETn_); \
}\

#define JIT_ASM_FAGET(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_FAGET, 0); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%d\n","FAGET",a1,a2,a3); \
    jit_6__movq_imm32_r8d(mem,a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_FAGET_); \
}\

#define JIT_ASM_FAGETn(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_FAGETN, 0); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%d\n","FAGETn",a1,a2,(int)a3); \
    jit_6__movq_imm32_r8d(mem,a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_FAGETn_); \
}\

#define JIT_ASM_ASET(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_ASET, 0); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%d\n","ASET",a1,a2,a3); \
    jit_6__movq_imm32_r8d(mem,a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_ASET_); \
}\

#define JIT_ASM_ASETn(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_ASETN, 0); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%d\n","ASETn",a1,a2,(int)a3); \
    jit_6__movq_imm32_r8d(mem,a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_ASETn_); \
}\

#define JIT_ASM_IASET(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_IASET, 0); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%d\n","IASET",a1,a2,a3); \
    jit_6__movq_imm32_r8d(mem,a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_IASET_); \
}\

#define JIT_ASM_IASETn(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_IASETN, 0); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%d\n","IASETn",a1,a2,(int)a3); \
    jit_6__movq_imm32_r8d(mem,a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_IASETn_); \
}\

#define JIT_ASM_FASET(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_FASET, 0); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%d\n","FASET",a1,a2,a3); \
    jit_6__movq_imm32_r8d(mem,a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_FASET_); \
}\

#define JIT_ASM_FASETn(ctx, a1, a2, a3) {\
    imem_setLabel(mem, op->opcode, OPSIZE_FASETN, 0); \
    IDEBUG("%-8s a1=%d,a2=%d,a3=%d\n","FASETn",a1,a2,(int)a3); \
    jit_6__movq_imm32_r8d(mem,a3); \
    jit_5__movl_imm32_ecx(mem,a2); \
    jit_5__movl_imm32_edx(mem,a1); \
    jit_3__movq_rbx_rsi(mem); \
    jit_3__movq_r12_rdi(mem); \
    jit_5__call(mem, JIT_OP_FASETn_); \
}\

#define JIT_ASM_INITCODE(ctx, a1) {\
    imem_setLabel(mem, op->opcode, OPSIZE_INITCODE, 0); \
    IDEBUG( "%-8s\n","INITCODE"); \
}\

#define JIT_ASM_NOP(ctx) {\
    imem_setLabel(mem, op->opcode, OPSIZE_NOP, 0); \
    IDEBUG( "%-8s\n","NOP"); \
    jit_5__call(mem, JIT_OP_NOP_); \
}\

#endif
