#ifndef IJT_ASM_H
#define IJT_ASM_H

#define OPCode_1_int union opcode { \
	uchar_t code[5];    \
	struct {            \
		uchar_t op;intptr_t ival;\
	} __attribute__((packed));\
}

#define OPCode_2_int union opcode {\
	uchar_t code[6];      \
	struct {                    \
		uchar_t op1;\
		uchar_t op2;\
		intptr_t ival;\
	} __attribute__((packed));  \
}

#define OPCode_2_i64 union opcode {\
	uchar_t code[10];      \
	struct {                    \
		uchar_t op1;\
		uchar_t op2;\
		knh_int_t ival;\
	} __attribute__((packed));  \
}

#define OPCode_4_int union opcode {\
	uchar_t code[8];      \
	struct {                    \
		uchar_t op1;\
		uchar_t op2;\
		uchar_t op3;\
		uchar_t op4;\
		int ival;\
	} __attribute__((packed));  \
}



#define OPCode_4 union opcode {\
	uchar_t code[4];      \
	struct {                    \
		uchar_t op1;\
		uchar_t op2;\
		uchar_t op3;\
		uchar_t op4;\
	} __attribute__((packed));  \
}

#define OPCode_3 union opcode {\
	uchar_t code[3];      \
	struct {                    \
		uchar_t op1;\
		uchar_t op2;\
		uchar_t op3;\
	} __attribute__((packed));  \
}

#define OPCode_3_i32 union opcode {\
	uchar_t code[7];      \
	struct {                    \
		uchar_t op1;\
		uchar_t op2;\
		uchar_t op3;\
		int ival;\
	} __attribute__((packed));  \
}



// jit_1__halt
// f4
#define jit_1__halt(mem) { \
    imem_push(mem,0xf4); \
} \


// jit_1__int
// cc
#define jit_1__int(mem) { \
    imem_push(mem,0xcc); \
} \


// jit_1__leave
// c9
#define jit_1__leave(mem) { \
    imem_push(mem,0xc9); \
} \


// jit_1__pop_ebx
// 5b
#define jit_1__pop_ebx(mem) { \
    imem_push(mem,0x5b); \
} \


// jit_1__pop_edi
// 5f
#define jit_1__pop_edi(mem) { \
    imem_push(mem,0x5f); \
} \


// jit_1__pop_esi
// 5e
#define jit_1__pop_esi(mem) { \
    imem_push(mem,0x5e); \
} \


// jit_1__push_ebp
// 55
#define jit_1__push_ebp(mem) { \
    imem_push(mem,0x55); \
} \


// jit_1__push_ebx
// 53
#define jit_1__push_ebx(mem) { \
    imem_push(mem,0x53); \
} \


// jit_1__push_edi
// 57
#define jit_1__push_edi(mem) { \
    imem_push(mem,0x57); \
} \


// jit_1__push_esi
// 56
#define jit_1__push_esi(mem) { \
    imem_push(mem,0x56); \
} \


// jit_1__ret
// c3
#define jit_1__ret(mem) { \
    imem_push(mem,0xc3); \
} \


// jit_2__addl_eax_eax
// 01 c0
#define jit_2__addl_eax_eax(mem) { \
    imem_push(mem,0x01); \
    imem_push(mem,0xc0); \
} \


// jit_2__addl_edx_eax
// 01 d0
#define jit_2__addl_edx_eax(mem) { \
    imem_push(mem,0x01); \
    imem_push(mem,0xd0); \
} \


// jit_2__call_eax
// ff d0
#define jit_2__call_eax(mem) { \
    imem_push(mem,0xff); \
    imem_push(mem,0xd0); \
} \


// jit_2__jmp
// eb 08
#define jit_2__jmp(mem,idx) { \
    imem_push(mem,0xeb); \
    imem_push(mem,idx); \
} \


// jit_2__je_0x1b
// 74 1b
#define jit_2__je_to(mem,idx) { \
    imem_push(mem,0x74); \
    imem_push(mem,idx); \
} \


// jit_2__jg_0x0c
// 74 0c
#define jit_2__jg_to(mem,idx) { \
    imem_push(mem,0x74); \
    imem_push(mem,idx); \
} \


// jit_2__jne_0xf7
// 75 f7
#define jit_2__jne_to(mem,idx) { \
    imem_push(mem,0x75); \
    imem_push(mem,idx); \
} \


// jit_2__movl_eax_ecx
// 89 c1
#define jit_2__movl_eax_ecx(mem) { \
    imem_push(mem,0x89); \
    imem_push(mem,0xc1); \
} \


// jit_2__movl_eax_edx
// 89 10
#define jit_2__movl_eax_edx(mem) { \
    imem_push(mem,0x89); \
    imem_push(mem,0x10); \
} \


// jit_2__movl_eax_esi
// 89 c6
#define jit_2__movl_eax_esi(mem) { \
    imem_push(mem,0x89); \
    imem_push(mem,0xc6); \
} \


// jit_2__movl_edi_edx
// 89 fa
#define jit_2__movl_edi_edx(mem) { \
    imem_push(mem,0x89); \
    imem_push(mem,0xfa); \
} \


// jit_2__movl_edx_eax
// 89 d0
#define jit_2__movl_edx_eax(mem) { \
    imem_push(mem,0x89); \
    imem_push(mem,0xd0); \
} \


// jit_2__movl_esp_ebp
// 89 e5
#define jit_2__movl_esp_ebp(mem) { \
    imem_push(mem,0x89); \
    imem_push(mem,0xe5); \
} \


// jit_2__pop_r12
// 41 5c
#define jit_2__pop_r12(mem) { \
    imem_push(mem,0x41); \
    imem_push(mem,0x5c); \
} \


// jit_2__pop_r13
// 41 5d
#define jit_2__pop_r13(mem) { \
    imem_push(mem,0x41); \
    imem_push(mem,0x5d); \
} \


// jit_2__pop_r14
// 41 5e
#define jit_2__pop_r14(mem) { \
    imem_push(mem,0x41); \
    imem_push(mem,0x5e); \
} \


// jit_2__pop_r15
// 41 5f
#define jit_2__pop_r15(mem) { \
    imem_push(mem,0x41); \
    imem_push(mem,0x5f); \
} \

// jit_2__push_r12
// 48 54
#define jit_2__push_r12(mem) { \
    imem_push(mem,0x41); \
    imem_push(mem,0x54); \
} \


// jit_2__push_r13
// 41 55
#define jit_2__push_r13(mem) { \
    imem_push(mem,0x41); \
    imem_push(mem,0x55); \
} \


// jit_2__push_r14
// 41 56
#define jit_2__push_r14(mem) { \
    imem_push(mem,0x41); \
    imem_push(mem,0x56); \
} \


// jit_2__push_r15
// 41 57
#define jit_2__push_r15(mem) { \
    imem_push(mem,0x41); \
    imem_push(mem,0x57); \
} \


// jit_2__pushq_$0x00
// 6a 00
#define jit_2__pushq_imm8(mem,imm8) { \
    imem_push(mem,0x6a); \
    imem_push(mem,imm8); \
} \


// jit_2__test_eax_eax
// 85 c0
#define jit_2__test_eax_eax(mem) { \
    imem_push(mem,0x85); \
    imem_push(mem,0xc0); \
} \


// jit_2__xor_eax_eax
// 31 c0
#define jit_2__xor_eax_eax(mem) { \
    imem_push(mem,0x31); \
    imem_push(mem,0xc0); \
} \


// jit_2__xorl_esi_esi
// 31 f6
#define jit_2__xorl_esi_esi(mem) { \
    imem_push(mem,0x31); \
    imem_push(mem,0xf6); \
} \


// jit_3__addl_$0x01_edx
// 83 c2 01
#define jit_3__addl_imm8_edx(mem,imm8) { \
    imem_push(mem,0x83); \
    imem_push(mem,0xc2); \
    imem_push(mem,imm8); \
} \


// jit_3__addl_$0x14_eax
// 83 c0 14
#define jit_3__addl_imm8_eax(mem,imm8) { \
    imem_push(mem,0x83); \
    imem_push(mem,0xc0); \
    imem_push(mem,imm8); \
} \


// jit_3__addl_$0x14_esp
// 83 c4 14
#define jit_3__addl_imm8_esp(mem,imm8) { \
    imem_push(mem,0x83); \
    imem_push(mem,0xc4); \
    imem_push(mem,imm8); \
} \


// jit_3__addl_0xf0(_ebp)_eax
// 03 45 f0
#define jit_3__addl_ebp_at_eax(mem,idx) { \
    imem_push(mem,0x03); \
    imem_push(mem,0x45); \
    imem_push(mem,idx); \
} \


// jit_3__addl_0xf0(_ebp)_ecx
// 03 4d f0
#define jit_3__addl_ebp_idx_ecx(mem,idx) { \
    imem_push(mem,0x03); \
    imem_push(mem,0x4d); \
    imem_push(mem,idx); \
} \


// jit_3__addq_rdi_rsi
// 48 01 fe
#define jit_3__addq_rdi_rsi(mem) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x01); \
    imem_push(mem,0xfe); \
} \


// jit_3__addq_rsi_rdx
// 48 01 f2
#define jit_3__addq_rsi_rdx(mem) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x01); \
    imem_push(mem,0xf2); \
} \


// jit_3__andl_$0x01_ebx
// 83 c3 01
#define jit_3__andl_imm8_ebx(mem,imm8) { \
    imem_push(mem,0x83); \
    imem_push(mem,0xc3); \
    imem_push(mem,imm8); \
} \


// jit_3__andl_$0xf0_esp
// 83 e4 f0
#define jit_3__andl_imm8_esp(mem) { \
    imem_push(mem,0x83); \
    imem_push(mem,0xe4); \
    imem_push(mem,imm8); \
} \


// jit_3__cmp_$0x01_ecx
// 83 f9 01
#define jit_3__cmp_imm8_ecx(mem,imm8) { \
    imem_push(mem,0x83); \
    imem_push(mem,0xf9); \
    imem_push(mem,imm8); \
} \


// jit_3__cmp_$0x01_esi
// 83 fe 01
#define jit_3__cmp_imm8_esi(mem,imm8) { \
    imem_push(mem,0x83); \
    imem_push(mem,0xfe); \
    imem_push(mem,imm8); \
} \


// jit_3__eb_0x0a
// eb 05 0a
#define jit_3__eb_to(mem,idx) { \
    imem_push(mem,0xeb); \
    imem_push(mem,0x05); \
    imem_push(mem,idx); \
} \


// jit_3__leal_(_eax,_eax,2)_eax
// 8d 04 40
#define jit_3__leal_to_eax(mem,idx) { \
    imem_push(mem,0x8d); \
    imem_push(mem,0x04); \
    imem_push(mem,0x40); \
} \


// jit_3__leal_(_ecx,_ecx,2)_ecx
// 8d 0c 49
#define jit_3__leal_to_ecx(mem,idx) { \
    imem_push(mem,0x8d); \
    imem_push(mem,0x0c); \
    imem_push(mem,0x49); \
} \


// jit_3__leal_0x08(_ebp)_eax
// 8d 45 80
#define jit_3__leal_ebp_at_eax(mem,idx) { \
    imem_push(mem,0x8d); \
    imem_push(mem,0x45); \
    imem_push(mem,idx); \
} \


// jit_3__leal_0x08(_ebp)_ecx
// 8d 4d 08
#define jit_3__leal_ebp_at_ecx(mem,idx) { \
    imem_push(mem,0x8d); \
    imem_push(mem,0x4d); \
    imem_push(mem,idx); \
} \


// jit_3__movl_0x04(_ebp)_ebx
// 8b 5d 04
#define jit_3__movl_ebp_at_ebx(mem,idx) { \
    imem_push(mem,0x8b); \
    imem_push(mem,0x5d); \
    imem_push(mem,idx); \
} \


// jit_3__movl_0x04(_ebp)_edx
// 8b 55 04
#define jit_3__movl_ebp_at_edx(mem,idx) { \
    imem_push(mem,0x8b); \
    imem_push(mem,0x55); \
    imem_push(mem,idx); \
} \


// jit_3__movl_0x08(_ebp)_ecx
// 8b 4d 08
#define jit_3__movl_ebp_at_ecx(mem,idx) { \
    imem_push(mem,0x8b); \
    imem_push(mem,0x4d); \
    imem_push(mem,idx); \
} \


// jit_3__movl_0x0c(_ebp)_eax
// 8b 45 0c
#define jit_3__movl_ebp_at_eax(mem,idx) { \
    imem_push(mem,0xc0); \
} \


// jit_3__movl_eax_0x04(_ecp)
// 89 41 04
#define jit_3__movl_eax_ecp_to(mem,idx) { \
    imem_push(mem,0x89); \
    imem_push(mem,0x41); \
    imem_push(mem,idx); \
} \


// jit_3__movl_eax_esp
// 89 04 24
#define jit_3__movl_eax_esp(mem) { \
    imem_push(mem,0x89); \
    imem_push(mem,0x04); \
    imem_push(mem,0x24); \
} \


// jit_3__movl_ecx_0x04(_ebp)
// 89 4d 04
#define jit_3__movl_ecx_ebp_to(mem,idx) { \
    imem_push(mem,0x89); \
    imem_push(mem,0x4d); \
    imem_push(mem,idx); \
} \


// jit_3__movl_edx_0x04(_ecp)
// 89 51 04
#define jit_3__movl_edx_ecp_to(mem,idx) { \
    imem_push(mem,0x89); \
    imem_push(mem,0x51); \
    imem_push(mem,idx); \
} \


// jit_3__movl_edx_0x04(_ecp)
// 89 51 04
#define jit_3__movl_edx_ecp_to(mem,idx) { \
    imem_push(mem,0x89); \
    imem_push(mem,0x51); \
    imem_push(mem,idx); \
} \


// jit_3__movl_edx_esp
// 89 14 24
#define jit_3__movl_edx_esp(mem) { \
    imem_push(mem,0x89); \
    imem_push(mem,0x14); \
    imem_push(mem,0x24); \
} \


// jit_3__movl_esp_esi
// 8b 34 24
#define jit_3__movl_esp_esi(mem) { \
    imem_push(mem,0x8b); \
    imem_push(mem,0x34); \
    imem_push(mem,0x24); \
} \


// jit_3__movq_r12_rdi
// 4c 89 e7
#define jit_3__movq_r12_rdi(mem) { \
    imem_push(mem,0x4c); \
    imem_push(mem,0x89); \
    imem_push(mem,0xe7); \
} \


// jit_3__movq_rax_rdi
// 48 8b 38
#define jit_3__movq_rax_rdi(mem) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x8b); \
    imem_push(mem,0x38); \
} \


// jit_3__movq_rbx_rdi
// 48 89 df
#define jit_3__movq_rbx_rdi(mem) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x89); \
    imem_push(mem,0xdf); \
} \


// jit_3__movq_rbx_rsi
// 48 89 de
#define jit_3__movq_rbx_rsi(mem) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x89); \
    imem_push(mem,0xde); \
} \


// jit_3__movq_rdi_r12
// 49 89 fc
#define jit_3__movq_rdi_r12(mem) { \
    imem_push(mem,0x49); \
    imem_push(mem,0x89); \
    imem_push(mem,0xfc); \
} \


// jit_3__movq_rsi_rbx
// 48 89 f3
#define jit_3__movq_rsi_rbx(mem) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x89); \
    imem_push(mem,0xf3); \
} \


// jit_3__movq_rsp_rbp
// 48 89 e5
#define jit_3__movq_rsp_rbp(mem) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x89); \
    imem_push(mem,0xe5); \
} \


// jit_3__movq_rsp_rbp
// 48 89 e5
#define jit_3__movq_rsp_rbp(mem) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x89); \
    imem_push(mem,0xe5); \
} \


// jit_3__movslq_ecx_rcx
// 48 63 c9
#define jit_3__movslq_ecx_rcx(mem) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x63); \
    imem_push(mem,0xc9); \
} \


// jit_3__movslq_esi_rsi
// 48 63 f6
#define jit_3__movslq_esi_rsi(mem) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x63); \
    imem_push(mem,0xf6); \
} \


// jit_3__movsw_(_dx)_edx
// 0f bf d2
#define jit_3__movsw_dx_edx(mem) { \
    imem_push(mem,0x0f); \
    imem_push(mem,0xbf); \
    imem_push(mem,0xd2); \
} \

// jit_3__movzbl_al_eax
// 0f b6 c0
#define jit_3__movzbl_al_eax(mem) { \
    imem_push(mem,0x0f); \
    imem_push(mem,0xb6); \
    imem_push(mem,0xc0); \
} \


// jit_3__ret_$0x0008
// c2 08 00
#define jit_3__ret_imm16(mem,imm16) { \
    imem_push(mem,0xc2); \
    imem_push(mem,(imm16 >> (0 * 8)) & 0xff); \
    imem_push(mem,(imm16 >> (1 * 8)) & 0xff); \
} \


// jit_3__setl_al
// 0f 9c c0
#define jit_3__setl_al(mem) { \
    imem_push(mem,0x0f); \
    imem_push(mem,0x9c); \
    imem_push(mem,0xc0); \
} \
 
// jit_3__setle_al
// 0f 9e c0
#define jit_3__setle_al(mem) { \
    imem_push(mem,0x0f); \
    imem_push(mem,0x9e); \
    imem_push(mem,0xc0); \
} \

// jit_3__shl_$0x02_eax
// c1 e0 02
#define jit_3__shl_imm8_eax(mem,imm8) { \
    imem_push(mem,0xc1); \
    imem_push(mem,0xe0); \
    imem_push(mem,imm8); \
} \


// jit_3__subl_$0x08_esp
// 83 ec 08
#define jit_3__subl_imm8_esp(mem,imm8) { \
    imem_push(mem,0x83); \
    imem_push(mem,0xec); \
    imem_push(mem,imm8); \
} \


// jit_3__subl_$0x14_eax
// 83 e8 14
#define jit_3__subl_imm8_eax(mem,imm8) { \
    imem_push(mem,0x83); \
    imem_push(mem,0xe8); \
    imem_push(mem,imm8); \
} \


// jit_3__test_rax_rax
// 48 85 c0
#define jit_3__test_rax_rax(mem) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x85); \
    imem_push(mem,0xc0); \
} \


// jit_4__addq_$0x08_rcx
// 48 83 c1 08
#define jit_4__addq_imm8_rcx(mem,imm8) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x83); \
    imem_push(mem,0xc1); \
    imem_push(mem,imm8); \
} \


// jit_4__addq_0x38(_rsi)_rax
// 48 03 46 38
#define jit_4__addq_rsi_at_rax(mem,idx) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x03); \
    imem_push(mem,0x46); \
    imem_push(mem,idx); \
} \


// jit_4__addq_0xf0(_rbp)_rax
// 48 03 45 f0
#define jit_4__addq_rbp_at_rax(mem,idx) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x03); \
    imem_push(mem,0x45); \
    imem_push(mem,idx); \
} \


// jit_4__andq_$0xf0_rsp
// 48 83 e4 f0
#define jit_4__andq_imm8_rsp(mem,imm8) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x83); \
    imem_push(mem,0xe4); \
    imem_push(mem,imm8); \
} \


// jit_4__cmp_$0x00_rcx
// 48 83 39 00
#define jit_4__cmp_imm8_rcx(mem,imm8) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x83); \
    imem_push(mem,0x39); \
    imem_push(mem,imm8); \
} \


// jit_4__cmp_$0x02_0x01(_ebp)
// 83 7d 01 02
#define jit_4__cmp_imm8_ebp_at(mem,imm8,idx) { \
    imem_push(mem,0x83); \
    imem_push(mem,0x7d); \
    imem_push(mem,idx); \
    imem_push(mem,imm8); \
} \


// jit_4__cmpq_rax,0x28(_rsi)
// 48 39 46 28
#define jit_4__cmp_rax_rsi_at(mem,imm8) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x39); \
    imem_push(mem,0x46); \
    imem_push(mem,imm8); \
} \


// jit_4__cvtsi2ss_ecx_xmm0
// f3 0f 2a c1
#define jit_4__cvtsi2ss_ecx_xmm0(mem) { \
    imem_push(mem,0xf3); \
    imem_push(mem,0x0f); \
    imem_push(mem,0x2a); \
    imem_push(mem,0xc1); \
} \


// jit_4__cvtsi2ss_edi_xmm0
// f3 0f 2a c7
#define jit_4__cvtsi2ss_edi_xmm0(mem) { \
    imem_push(mem,0xf3); \
    imem_push(mem,0x0f); \
    imem_push(mem,0x2a); \
    imem_push(mem,0xc7); \
} \


// jit_4__cvtsi2ss_edx_xmm0
// f3 0f 2a c2
#define jit_4__cvtsi2ss_edx_xmm0(mem) { \
    imem_push(mem,0xf3); \
    imem_push(mem,0x0f); \
    imem_push(mem,0x2a); \
    imem_push(mem,0xc2); \
} \


// jit_4__cvtsi2ss_esi_xmm0
// f3 0f 2a c6
#define jit_4__cvtsi2ss_esi_xmm0(mem) { \
    imem_push(mem,0xf3); \
    imem_push(mem,0x0f); \
    imem_push(mem,0x2a); \
    imem_push(mem,0xc6); \
} \


// jit_4__decq_0x18(_rax)
// 48 ff 48 f0
#define jit_4__decq_rax_at(mem,idx) { \
    imem_push(mem,0x48); \
    imem_push(mem,0xff); \
    imem_push(mem,0x48); \
    imem_push(mem,idx); \
} \


// jit_4__incq_0x18(_rcx)
// 48 ff 41 18
#define jit_4__incq_rcx_at(mem,idx) { \
    imem_push(mem,0x48); \
    imem_push(mem,0xff); \
    imem_push(mem,0x41); \
    imem_push(mem,idx); \
} \


// jit_4__incq_0x18(_rdx)
// 48 ff 42 18
#define jit_4__incq_rdx_at(mem,idx) { \
    imem_push(mem,0x48); \
    imem_push(mem,0xff); \
    imem_push(mem,0x42); \
    imem_push(mem,idx); \
} \


// jit_4__leal_0x10(_rdi)_rax
// 48 8d 47 10
#define jit_4__leal_rdi_rax(mem,idx) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x8d); \
    imem_push(mem,0x47); \
    imem_push(mem,idx); \
} \


// jit_4__leaq_0x10(_rbp)_rsi
// 48 8d 75 10
#define jit_4__leaq_rbp_at_rsi(mem,idx) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x8d); \
    imem_push(mem,0x75); \
    imem_push(mem,idx); \
} \


// jit_4__leaq_0x20(_rsi)_rdx
// 48 8d 56 20
#define jit_4__leaq_rsi_at_rdx(mem,idx) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x8d); \
    imem_push(mem,0x56); \
    imem_push(mem,idx); \
} \


// jit_4__movl_$0xe0(_rbp)_r8d
// 44 8b 45 e0
#define jit_4__movl_rbp_at_r8d(mem,idx) { \
    imem_push(mem,0x44); \
    imem_push(mem,0x8b); \
    imem_push(mem,0x45); \
    imem_push(mem,idx); \
} \


// jit_4__movl_0x04(_esp)_edi
// 8b 7c 24 04
#define jit_4__movl_esp_at_edi(mem,idx) { \
    imem_push(mem,0x8b); \
    imem_push(mem,0x7c); \
    imem_push(mem,0x24); \
    imem_push(mem,idx); \
} \


// jit_4__movl_eax_0x04(_esp)
// 89 44 24 04
#define jit_4__movl_eax_esp_to(mem,idx) { \
    imem_push(mem,0x89); \
    imem_push(mem,0x44); \
    imem_push(mem,0x24); \
    imem_push(mem,idx); \
} \

// FIXME
// it seames duplicate... orz
// jit_4__movl_ebx_esp_to also has bug.

// jit_4__movl_ebx_0x04(_esp)
// 89 4c 24 04
#define jit_4__movl_ebx_esp_to(mem,idx) { \
    imem_push(mem,0x89); \
    imem_push(mem,0x4c); \
    imem_push(mem,0x24); \
    imem_push(mem,idx); \
} \


// jit_4__movl_ebx_0x08(_esp)
// 89 54 24 08
#define jit_4__movl_ebx2_esp_to(mem,idx) { \
    imem_push(mem,0x89); \
    imem_push(mem,0x54); \
    imem_push(mem,0x24); \
    imem_push(mem,idx); \
} \


// jit_4__movl_ecx_0x04(_esp)
// 89 5c 24 04
#define jit_4__movl_ecx_esp_to(mem,idx) { \
    imem_push(mem,0x89); \
    imem_push(mem,0x5c); \
    imem_push(mem,0x24); \
    imem_push(mem,idx); \
} \


// jit_4__movl_esi_0x08(_esp)
// 89 74 24 08
#define jit_4__movl_esp_to(mem,idx) { \
    imem_push(mem,0x89); \
    imem_push(mem,0x74); \
    imem_push(mem,0x24); \
    imem_push(mem,idx); \
} \


// jit_4__movq_$0xe8(_rbp)_r9
// 4c 8b 4d e8
#define jit_4__movq_rbp_at_r9(mem,idx) { \
    imem_push(mem,0x4c); \
    imem_push(mem,0x8b); \
    imem_push(mem,0x4d); \
    imem_push(mem,idx); \
} \


// jit_4__movq_0x08(_rax)_rax
// 48 8b 40 08
#define jit_4__movq_rax_at_rax(mem,idx) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x8b); \
    imem_push(mem,0x40); \
    imem_push(mem,idx); \
} \


// jit_4__movq_0x08(_rbp)_rdi
// 48 8b 7d 08
#define jit_4__movq_rbp_at_rdi(mem,idx) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x8b); \
    imem_push(mem,0x7d); \
    imem_push(mem,idx); \
} \

// jit_4__movq_0x08(_rsi)_rax
// 48 8b 46 08
#define jit_4__movq_rsi_at_rax(mem,imm8) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x8b); \
    imem_push(mem,0x46); \
    imem_push(mem,imm8); \
} \


// jit_4__movq_0x20(_rsi)_r12
// 48 8b 66 20
#define jit_4__movq_rsi_at_r12(mem,imm8) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x8b); \
    imem_push(mem,0x66); \
    imem_push(mem,imm8); \
} \


// jit_4__movq_0x20(_rsi)_rdx
// 48 8b 56 20
#define jit_4__movq_rsi_at_rdx(mem,imm8) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x8b); \
    imem_push(mem,0x56); \
    imem_push(mem,imm8); \
} \


// jit_4__movq_0x20(_rsi)_rsi
// 48 8b 76 20
#define jit_4__movq_rsi_at_rsi(mem,imm8) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x8b); \
    imem_push(mem,0x76); \
    imem_push(mem,imm8); \
} \


// jit_4__movq_0x28(_rcx)_rax
// 48 8b 41 28
#define jit_4__movq_rcx_at_rax(mem,imm8) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x8b); \
    imem_push(mem,0x41); \
    imem_push(mem,imm8); \
} \


// jit_4__movq_r12_0xf8(_rbp)
// 4c 89 65 f8
#define jit_4__movq_r12_rbp_to(mem,idx) { \
    imem_push(mem,0x4c); \
    imem_push(mem,0x89); \
    imem_push(mem,0x65); \
    imem_push(mem,idx); \
} \


// jit_4__movq_r8_0x18(_rsi)
// 4c 89 46 18
#define jit_4__movq_r8_rsi_at(mem,idx) { \
    imem_push(mem,0x4c); \
    imem_push(mem,0x89); \
    imem_push(mem,0x46); \
    imem_push(mem,idx); \
} \


// jit_4__movq_r9_0x18(_rsi)
// 4c 89 4e 18
#define jit_4__movq_r9_rsi_at(mem,idx) { \
    imem_push(mem,0x4c); \
    imem_push(mem,0x89); \
    imem_push(mem,0x4e); \
    imem_push(mem,idx); \
} \


// jit_4__movq_rax_0x18(_rsi)
// 48 89 46 18
#define jit_4__movq_rax_rsi_to(mem,imm8) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x89); \
    imem_push(mem,0x46); \
    imem_push(mem,imm8); \
} \


// jit_4__movq_rcx_0x18(_rsi)
// 48 89 4e 18
#define jit_4__movq_rcx_rsi_at(mem,imm8) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x89); \
    imem_push(mem,0x4e); \
    imem_push(mem,imm8); \
} \


// jit_4__movq_rdx_0x18(_rsi)
// 48 89 56 18
#define jit_4__movq_rdx_rsi_at(mem,imm8) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x89); \
    imem_push(mem,0x56); \
    imem_push(mem,imm8); \
} \

// jit_4__movsw_0x0c(_ebp)_ecx
// 0f bf 4d 0c
#define jit_4__movsw_ebp_at_ecx(mem,idx) { \
    imem_push(mem,0x0f); \
    imem_push(mem,0xbf); \
    imem_push(mem,0x4d); \
    imem_push(mem,idx); \
} \


// jit_4__movsw_0xf6(_ebp)_edx
// 0f bf 55 f6
#define jit_4__movsw_ebp_at_edx(mem,idx) { \
    imem_push(mem,0x0f); \
    imem_push(mem,0xbf); \
    imem_push(mem,0x55); \
    imem_push(mem,idx); \
} \


// jit_4__movw_ax_0xe4(_ebp)
// 66 89 45 e4
#define jit_4__movw_ax_ebp_to(mem,idx) { \
    imem_push(mem,0x66); \
    imem_push(mem,0x89); \
    imem_push(mem,0x45); \
    imem_push(mem,idx); \
} \


// jit_4__movw_dx_0xe4(_ebp)
// 66 89 55 e4
#define jit_4__movw_dx_ebp_to(mem,idx) { \
    imem_push(mem,0x66); \
    imem_push(mem,0x89); \
    imem_push(mem,0x55); \
    imem_push(mem,idx); \
} \


// jit_4__shlq_$0x04_rax
// 48 c1 e0 04
#define jit_4__shlq_imm8_rax(mem,imm8) { \
    imem_push(mem,0x48); \
    imem_push(mem,0xc1); \
    imem_push(mem,0xe0); \
    imem_push(mem,imm8); \
} \

// jit_4__shlq_$0x04_rcx
// 48 c1 e1 04
#define jit_4__shlq_imm8_rcx(mem,imm8) { \
    imem_push(mem,0x48); \
    imem_push(mem,0xc1); \
    imem_push(mem,0xe1); \
    imem_push(mem,imm8); \
} \


// jit_4__shlq_$0x04_rsi
// 48 c1 e6 04
#define jit_4__shlq_imm8_rsi(mem,imm8) { \
    imem_push(mem,0x48); \
    imem_push(mem,0xc1); \
    imem_push(mem,0xe6); \
    imem_push(mem,imm8); \
} \


// jit_4__subq_$0x02_rax
// 48 83 e8 02
#define jit_4__subq_rax_imm8(mem,imm8) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x83); \
    imem_push(mem,0xe8); \
    imem_push(mem,imm8); \
} \


// jit_4__subq_$0x10_rsp
// 48 83 ec 10
#define jit_4__subq_imm8_rsp(mem,imm8) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x83); \
    imem_push(mem,0xec); \
    imem_push(mem,imm8); \
} \


// jit_5__call_0x01020304
// e8 04 03 02 01
#define jit_5__call(mem,func) { \
    intptr_t ptr = (intptr_t) func - (intptr_t) (mem->addr + mem->index + 5); \
    OPCode_1_int _o;\
    _o.op   = 0xe8;\
    _o.ival = ptr;   \
    imem_write(mem,_o.code, 5); \
} \


// jit_5__cmp_$0x00_0x08(_rsi)
// 48 83 7e 08 00
#define jit_5__cmp_imm8_rsi_at(mem,imm8,idx) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x83); \
    imem_push(mem,0x7e); \
    imem_push(mem,idx); \
    imem_push(mem,imm8); \
} \

// jit_5__cvtsi2ss_r8d_xmm0
// f3 41 0f 2a c0
#define jit_5__cvtsi2ss_r8d_xmm0(mem) { \
    imem_push(mem,0xf3); \
    imem_push(mem,0x41); \
    imem_push(mem,0x0f); \
    imem_push(mem,0x2a); \
    imem_push(mem,0xc0); \
} \

 
// jit_5__imulq_0x28(_rsi)_r8
// 4c 0f af 46 28
#define jit_5__imulq_rsi_at_r8(mem,idx) { \
    imem_push(mem,0x4c); \
    imem_push(mem,0x0f); \
    imem_push(mem,0xaf); \
    imem_push(mem,0x46); \
    imem_push(mem,idx); \
} \


// jit_5__imulq_0x28(_rsi)_r9
// 4c 0f af 4e 28
#define jit_5__imulq_rsi_at_r9(mem,idx) { \
    imem_push(mem,0x4c); \
    imem_push(mem,0x0f); \
    imem_push(mem,0xaf); \
    imem_push(mem,0x4e); \
    imem_push(mem,idx); \
} \


// jit_5__imulq_0x28(_rsi)_rcx
// 48 0f af 4e 28
#define jit_5__imulq_rsi_at_rcx(mem,idx) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x0f); \
    imem_push(mem,0xaf); \
    imem_push(mem,0x4e); \
    imem_push(mem,0x28); \
} \


// jit_5__imulq_0x28(_rsi)_rdx
// 48 0f af 56 28
#define jit_5__imulq_rsi_at_rdx(mem,idx) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x0f); \
    imem_push(mem,0xaf); \
    imem_push(mem,0x56); \
    imem_push(mem,idx); \
} \


// jit_5__imulq_0x38(_rsi)_rax
// 48 0f af 46 38
#define jit_5__imulq_rsi_at_rax(mem,idx) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x0f); \
    imem_push(mem,0xaf); \
    imem_push(mem,0x46); \
    imem_push(mem,idx); \
} \


// jit_5__incq_0x18(_r12)
// 49 ff 44 24 18
#define jit_5__incq_r12_at(mem,idx) { \
    imem_push(mem,0x49); \
    imem_push(mem,0xff); \
    imem_push(mem,0x44); \
    imem_push(mem,0x24); \
    imem_push(mem,idx); \
} \

// jit_5__jmp_0x01020304
// e9 04 03 02 01
#define jit_5__jmp(mem,imm32) { \
	/*intptr_t ptr = (intptr_t) func - (intptr_t) (mem->addr + mem->index + 5); */\
	OPCode_1_int _o;\
	_o.op   = 0xe9;\
	_o.ival = imm32;   \
	imem_write(mem,_o.code, 5); \
} \


// jit_5__movl_$0x00000001_esi
// be 01 00 00 00
#define jit_5__movl_imm32_esi(mem,imm32) { \
	OPCode_1_int _o;\
	_o.op   = 0xbe;\
	_o.ival = imm32;   \
	imem_write(mem,_o.code, 5); \
} \


// jit_5__movl_$0x00000002_ecx
// b9 02 00 00 00
#define jit_5__movl_imm32_ecx(mem,imm32) { \
	OPCode_1_int _o;\
	_o.op   = 0xb9;\
	_o.ival = imm32;   \
	imem_write(mem,_o.code, 5); \
} \


// jit_5__movl_$0x00000002_edi
// bf 02 00 00 00
#define jit_5__movl_imm32_edi(mem,imm32) { \
	OPCode_1_int _o;\
	_o.op   = 0xbf;\
	_o.ival = imm32;   \
	imem_write(mem,_o.code, 5); \
} \


// jit_5__movl_$0x00000004_edx
// ba 04 00 00 00
#define jit_5__movl_imm32_edx(mem,imm32) { \
	OPCode_1_int _o;\
	_o.op   = 0xba;\
	_o.ival = imm32;   \
	imem_write(mem,_o.code, 5); \
} \


// jit_5__movl_0x01020304_eax
// b8 04 03 02 01
#define jit_5__movl_imm32_eax(mem,imm32) { \
	OPCode_1_int _o;\
	_o.op   = 0xb8;\
	_o.ival = imm32;   \
	imem_write(mem,_o.code, 5); \
} \


// jit_6__cmp_$0x00_0x08(_rcx,rsi)
// 48 83 7c 31 08 00
#define jit_6__cmp_imm8_rcx_at(mem,imm8,idx) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x83); \
    imem_push(mem,0x7c); \
    imem_push(mem,0x31); \
    imem_push(mem,idx); \
    imem_push(mem,imm8); \
} \


// jit_6__jneq_0xfffffeef
// 0f 85 ef fe ff ff
#define jit_6__jneq_imm32(mem,imm32) { \
	OPCode_2_int _o;\
	_o.op1  = 0x0f;\
	_o.op2  = 0x85;\
	_o.ival = imm32;   \
	imem_write(mem,_o.code, 6); \
} \


// jit_6__leal_0x72-0xf(_ebx)_eax
// 8d 83 63 00 00 00
#define jit_6__leal_ebx_at_eax(mem,imm32) { \
	OPCode_2_int _o;\
	_o.op1  = 0x8d;\
	_o.op2  = 0x83;\
	_o.ival = imm32;   \
	imem_write(mem,_o.code, 6); \
} \


// jit_6__movq_$0x00000001_r8d
// 41 b8 01 00 00 00
#define jit_6__movq_imm32_r8d(mem,imm32) { \
	OPCode_2_int _o;\
	_o.op1  = 0x41;\
	_o.op2  = 0xb8;\
	_o.ival = imm32;   \
	imem_write(mem,_o.code, 6); \
} \

// jit_6__movq_$0x00000001_r9d
// 41 b9 02 00 00 00
#define jit_6__movq_imm32_r9d(mem,imm32) { \
	OPCode_2_int _o;\
	_o.op1  = 0x41;\
	_o.op2  = 0xb9;\
	_o.ival = imm32;   \
	imem_write(mem,_o.code, 6); \
} \


// jit_6__movw_0x0001_0xf4(_ebp)
// 66 c7 45 f4 01 00
#define jit_6__movw_imm16_ebp_at(mem,imm16,idx) { \
    imem_push(mem,0x66); \
    imem_push(mem,0xc7); \
    imem_push(mem,0x45); \
    imem_push(mem,idx); \
    imem_push(mem,(imm16 >> (0 * 8)) & 0xff); \
    imem_push(mem,(imm16 >> (1 * 8)) & 0xff); \
} \


// jit_6__subq_$0x00000111_eax
// 48 2d 11 01 00 00
#define jit_6__subq_imm32_eax(mem,imm32) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x2d); \
    imem_push(mem,(imm32 >> (0 * 8)) & 0xff); \
    imem_push(mem,(imm32 >> (1 * 8)) & 0xff); \
    imem_push(mem,(imm32 >> (2 * 8)) & 0xff); \
    imem_push(mem,(imm32 >> (3 * 8)) & 0xff); \
} \


// jit_7__addl_0x00000004(,_ecx,4),_esi
// 03 34 8d 04 00 00 00
#define jit_7__addl_ecx_at_esi(mem,idx32) { \
    imem_push(mem,0x03); \
    imem_push(mem,0x34); \
    imem_push(mem,0x8d); \
    imem_push(mem,(idx32 >> (0 * 8)) & 0xff); \
    imem_push(mem,(idx32 >> (1 * 8)) & 0xff); \
    imem_push(mem,(idx32 >> (2 * 8)) & 0xff); \
    imem_push(mem,(idx32 >> (3 * 8)) & 0xff); \
} \


// jit_7__addl_0x00000008(,_ecx,8),_edi
// 13 3c 8d 08 00 00 00
#define jit_7__addl_ecx_at_edi(mem,idx32) { \
    imem_push(mem,0x13); \
    imem_push(mem,0x3c); \
    imem_push(mem,0x8d); \
    imem_push(mem,(idx32 >> (0 * 8)) & 0xff); \
    imem_push(mem,(idx32 >> (1 * 8)) & 0xff); \
    imem_push(mem,(idx32 >> (2 * 8)) & 0xff); \
    imem_push(mem,(idx32 >> (3 * 8)) & 0xff); \
} \


// jit_7__addq_$0x000000a8_rsp
// 48 81 c4 a8 00 00 00
#define jit_7__addq_imm32_rsp(mem,imm32) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x81); \
    imem_push(mem,0xc4); \
    imem_push(mem,(imm32 >> (0 * 8)) & 0xff); \
    imem_push(mem,(imm32 >> (1 * 8)) & 0xff); \
    imem_push(mem,(imm32 >> (2 * 8)) & 0xff); \
    imem_push(mem,(imm32 >> (3 * 8)) & 0xff); \
} \


// jit_7__leaq_0xffffff50(_rbp)_rbx
// 48 8d 9d 50 ff ff ff
#define jit_7__leaq_rbp_to_rbx(mem,idx32) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x8d); \
    imem_push(mem,0x9d); \
    imem_push(mem,(idx32 >> (0 * 8)) & 0xff); \
    imem_push(mem,(idx32 >> (1 * 8)) & 0xff); \
    imem_push(mem,(idx32 >> (2 * 8)) & 0xff); \
    imem_push(mem,(idx32 >> (3 * 8)) & 0xff); \
} \


// jit_7__movl_0x00000004(,_eax,4),_esi
// 8b 34 85 04 00 00 00
#define jit_7__movl_eax_at_esi(mem,idx32) { \
    imem_push(mem,0x8b); \
    imem_push(mem,0x34); \
    imem_push(mem,0x85); \
    imem_push(mem,(idx32 >> (0 * 8)) & 0xff); \
    imem_push(mem,(idx32 >> (1 * 8)) & 0xff); \
    imem_push(mem,(idx32 >> (2 * 8)) & 0xff); \
    imem_push(mem,(idx32 >> (3 * 8)) & 0xff); \
} \


// jit_7__movl_0x00000008(,_eax,8),_edi
// 8b 3c 85 08 00 00 00
#define jit_7__movl_eax_at_edi(mem,idx32) { \
    imem_push(mem,0x8b); \
    imem_push(mem,0x3c); \
    imem_push(mem,0x85); \
    imem_push(mem,(idx32 >> (0 * 8)) & 0xff); \
    imem_push(mem,(idx32 >> (1 * 8)) & 0xff); \
    imem_push(mem,(idx32 >> (2 * 8)) & 0xff); \
    imem_push(mem,(idx32 >> (3 * 8)) & 0xff); \
} \


// jit_7__movl_0x01020304_0x08(_ebp)
// c7 45 08 04 03 02 01
#define jit_7__movl_imm32_ebp_at(mem,imm32,idx) { \
    imem_push(mem,0xc7); \
    imem_push(mem,0x45); \
    imem_push(mem,idx); \
    imem_push(mem,(imm32 >> (0 * 8)) & 0xff); \
    imem_push(mem,(imm32 >> (1 * 8)) & 0xff); \
    imem_push(mem,(imm32 >> (2 * 8)) & 0xff); \
    imem_push(mem,(imm32 >> (3 * 8)) & 0xff); \
} \


// jit_7__movl_edi_0x00000008(,_edx,8)
// 89 3c 95 08 00 00 00
#define jit_7__movl_edi_edx_at(mem,idx32) { \
    imem_push(mem,0x89); \
    imem_push(mem,0x3c); \
    imem_push(mem,0x95); \
    imem_push(mem,(idx32 >> (0 * 8)) & 0xff); \
    imem_push(mem,(idx32 >> (1 * 8)) & 0xff); \
    imem_push(mem,(idx32 >> (2 * 8)) & 0xff); \
    imem_push(mem,(idx32 >> (3 * 8)) & 0xff); \
} \


// jit_7__movl_esi_0x00000008(,_edx,8)
// 89 34 95 08 00 00 00
#define jit_7__movl_esi_edx_to(mem,idx32) { \
    imem_push(mem,0x89); \
    imem_push(mem,0x34); \
    imem_push(mem,0x95); \
    imem_push(mem,(imm32 >> (0 * 8)) & 0xff); \
    imem_push(mem,(imm32 >> (1 * 8)) & 0xff); \
    imem_push(mem,(imm32 >> (2 * 8)) & 0xff); \
    imem_push(mem,(imm32 >> (3 * 8)) & 0xff); \
} \


// jit_7__movq_$0x00000000(_rip)_rax
// 48 8b 05 00 00 00 00
#define jit_7__movq_rip_to_rax(mem,idx32) { \
	OPCode_3_i32 _o;\
	_o.op1  = 0x48;\
	_o.op2  = 0x8b;\
	_o.op3  = 0x05;\
	_o.ival = imm32;   \
	imem_write(mem,_o.code, 7); \
} \


// jit_7__movq_$0x00000001_rdi
// 48 c7 07 01 00 00 00
#define jit_7__movq_imm32_rdi(mem,imm32) { \
	OPCode_3_i32 _o;\
	_o.op1  = 0x48;\
	_o.op2  = 0xc7;\
	_o.op3  = 0x07;\
	_o.ival = imm32;   \
	imem_write(mem,_o.code, 7); \
} \


// jit_7__movq_$0x00000001_rsi
// 48 c7 06 01 00 00 00
#define jit_7__movq_imm32_rsi(mem,imm32) { \
	OPCode_3_i32 _o;\
	_o.op1  = 0x48;\
	_o.op2  = 0xc7;\
	_o.op3  = 0x06;\
	_o.ival = imm32;   \
	imem_write(mem,_o.code, 7); \
} \


// jit_7__movq_$0x00000003_rsp
// c7 04 24 03 00 00 00
#define jit_7__movq_imm32_rsp(mem,imm32) { \
	OPCode_3_i32 _o;\
	_o.op1  = 0xc7;\
	_o.op2  = 0x04;\
	_o.op3  = 0x24;\
	_o.ival = imm32;   \
	imem_write(mem,_o.code, 7); \
} \


// jit_7__subq_$0x000000a8_rsp
// 48 81 ec a8 00 00 00
#define jit_7__subq_imm32_rsp(mem,imm32) { \
	OPCode_3_i32 _o;\
	_o.op1  = 0x48;\
	_o.op2  = 0x81;\
	_o.op3  = 0xec;\
	_o.ival = imm32;   \
	imem_write(mem,_o.code, 7); \
} \


// jit_8__cmp_$0x00_0x0000018(_rsi)
// 48 83 be 18 01 00 00 00
#define jit_8__cmp_imm8_rsi_at(mem,imm8,imm32) { \
    imem_push(mem,0x48); \
    imem_push(mem,0x83); \
    imem_push(mem,0xbe); \
    imem_push(mem,(imm32 >> (0 * 8)) & 0xff); \
    imem_push(mem,(imm32 >> (1 * 8)) & 0xff); \
    imem_push(mem,(imm32 >> (2 * 8)) & 0xff); \
    imem_push(mem,(imm32 >> (3 * 8)) & 0xff); \
    imem_push(mem,imm8); \
} \


// jit_8__movl_$0x00000004_edx
// 48 c7 40 08 04 00 00 00
#define jit_8__movl_imm32_edx(mem,imm32) { \
    OPCode_4_int _o;\
    _o.op1  = 0x48;\
    _o.op2  = 0xc7;\
    _o.op3  = 0x40;\
    _o.op4  = 0x08;\
    _o.ival = imm32;   \
    imem_write(mem,_o.code, 8); \
} \


// jit_8__movl_$0x00000078_0x08(_esp)
// c7 44 24 08 78 00 00 00
#define jit_8__movl_imm32_esp_at(mem,imm32,idx) { \
    imem_push(mem,0xc7); \
    imem_push(mem,0x44); \
    imem_push(mem,0x24); \
    imem_push(mem,idx); \
    imem_push(mem,(imm32 >> (0 * 8)) & 0xff); \
    imem_push(mem,(imm32 >> (1 * 8)) & 0xff); \
    imem_push(mem,(imm32 >> (2 * 8)) & 0xff); \
    imem_push(mem,(imm32 >> (3 * 8)) & 0xff); \
} \


// jit_8__movq_$0x00000002_0x08(_rdi)
// 48 c7 47 08 02 00 00 00
#define jit_8__movq_imm32_rdi_at(mem,imm32,idx) { \
    imem_push(mem,0x48); \
    imem_push(mem,0xc7); \
    imem_push(mem,0x47); \
    imem_push(mem,idx); \
    imem_push(mem,(imm32 >> (0 * 8)) & 0xff); \
    imem_push(mem,(imm32 >> (1 * 8)) & 0xff); \
    imem_push(mem,(imm32 >> (2 * 8)) & 0xff); \
    imem_push(mem,(imm32 >> (3 * 8)) & 0xff); \
} \


// jit_8__movq_$0x00000002_0x08(_rsi)
// 48 c7 46 08 02 00 00 00
#define jit_8__movq_imm32_rsi_at(mem,imm32,idx) { \
    imem_push(mem,0x48); \
    imem_push(mem,0xc7); \
    imem_push(mem,0x46); \
    imem_push(mem,idx); \
    imem_push(mem,(imm32 >> (0 * 8)) & 0xff); \
    imem_push(mem,(imm32 >> (1 * 8)) & 0xff); \
    imem_push(mem,(imm32 >> (2 * 8)) & 0xff); \
    imem_push(mem,(imm32 >> (3 * 8)) & 0xff); \
} \


// jit_8__movq_$0x00000003_0x10(_rax)
// 48 c7 47 10 03 00 00 00
#define jit_8__movq_imm32_rax_at(mem,imm32,idx) { \
    imem_push(mem,0x48); \
    imem_push(mem,0xc7); \
    imem_push(mem,0x47); \
    imem_push(mem,idx); \
    imem_push(mem,(imm32 >> (0 * 8)) & 0xff); \
    imem_push(mem,(imm32 >> (1 * 8)) & 0xff); \
    imem_push(mem,(imm32 >> (2 * 8)) & 0xff); \
    imem_push(mem,(imm32 >> (3 * 8)) & 0xff); \
} \


// jit_8__movq_$0x00000003_0xe8(_rbp)
// 48 c7 45 e8 03 00 00 00
#define jit_8__movq_imm32_rbp_at(mem,imm32,idx) { \
    imem_push(mem,0x48); \
    imem_push(mem,0xc7); \
    imem_push(mem,0x45); \
    imem_push(mem,idx); \
    imem_push(mem,(imm32 >> (0 * 8)) & 0xff); \
    imem_push(mem,(imm32 >> (1 * 8)) & 0xff); \
    imem_push(mem,(imm32 >> (2 * 8)) & 0xff); \
    imem_push(mem,(imm32 >> (3 * 8)) & 0xff); \
} \


// jit_10__movq_$0x000000123456788f_rax
// 48 b8 8f 78 56 34 12 00 00 00
#define jit_10__movq_imm64_rax(mem,imm64) { \
	OPCode_2_i64 _o;\
	_o.op1  = 0x48;\
	_o.op2  = 0xb8;\
	_o.ival = imm64;   \
	imem_write(mem,_o.code, 10); \
} \


// jit_10__movq_$0x0000001234567890_r8
// 49 b8 90 78 56 34 12 00 00  00
#define jit_10__movq_imm64_r8(mem,imm64) { \
	OPCode_2_i64 _o;\
	_o.op1  = 0x49;\
	_o.op2  = 0xb8;\
	_o.ival = imm64;   \
	imem_write(mem,_o.code, 10); \
} \


// jit_10__movq_$0x0000001234567890_r9
// 49 b9 90 78 56 34 12 00 00 00
#define jit_10__movq_imm64_r9(mem,imm64) { \
	OPCode_2_i64 _o;\
	_o.op1  = 0x49;\
	_o.op2  = 0xb9;\
	_o.ival = imm64;   \
	imem_write(mem,_o.code, 10); \
} \


// jit_10__movq_$0x0000001234567890_rcx
// 48 b9 90 78 56 34 12 00 00  00
#define jit_10__movq_imm64_rcx(mem,imm64) { \
	OPCode_2_i64 _o;\
	_o.op1  = 0x48;\
	_o.op2  = 0xb9;\
	_o.ival = imm64;   \
	imem_write(mem,_o.code, 10); \
} \


#endif
