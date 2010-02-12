#ifndef KONOHA_CODE_H_
#define KONOHA_CODE_H_
// THIS FILE WAS AUTOMATICALLY GENERATED

#define OPCODE_HALT ((knh_opcode_t)0)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	void *nextaddr;   /* sameas next codeaddr */
} klr_HALT_t;

#define OPSIZE_HALT  (sizeof(klr_HALT_t)-sizeof(void*))

#define OPCODE_ENTER ((knh_opcode_t)1)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	void *nextaddr;   /* sameas next codeaddr */
} klr_ENTER_t;

#define OPSIZE_ENTER  (sizeof(klr_ENTER_t)-sizeof(void*))

#define OPCODE_EXEC ((knh_opcode_t)2)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	void *nextaddr;   /* sameas next codeaddr */
} klr_EXEC_t;

#define OPSIZE_EXEC  (sizeof(klr_EXEC_t)-sizeof(void*))

#define OPCODE_YEILD ((knh_opcode_t)3)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	void *nextaddr;   /* sameas next codeaddr */
} klr_YEILD_t;

#define OPSIZE_YEILD  (sizeof(klr_YEILD_t)-sizeof(void*))

#define OPCODE_EXIT ((knh_opcode_t)4)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	void *nextaddr;   /* sameas next codeaddr */
} klr_EXIT_t;

#define OPSIZE_EXIT  (sizeof(klr_EXIT_t)-sizeof(void*))

#define OPCODE_TR ((knh_opcode_t)5)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	klr_Ftr a1;
	knh_sfpidx_t a2;
	knh_class_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_TR_t;

#define OPSIZE_TR  (sizeof(klr_TR_t)-sizeof(void*))

#define OPCODE_OTR ((knh_opcode_t)6)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	klr_Ftr a1;
	knh_sfpidx_t a2;
	knh_class_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_OTR_t;

#define OPSIZE_OTR  (sizeof(klr_OTR_t)-sizeof(void*))

#define OPCODE_NULTR ((knh_opcode_t)7)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	klr_Ftr a1;
	knh_sfpidx_t a2;
	knh_class_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_NULTR_t;

#define OPSIZE_NULTR  (sizeof(klr_NULTR_t)-sizeof(void*))

#define OPCODE_UNBOX ((knh_opcode_t)8)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	void *nextaddr;   /* sameas next codeaddr */
} klr_UNBOX_t;

#define OPSIZE_UNBOX  (sizeof(klr_UNBOX_t)-sizeof(void*))

#define OPCODE_iCAST ((knh_opcode_t)9)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iCAST_t;

#define OPSIZE_iCAST  (sizeof(klr_iCAST_t)-sizeof(void*))

#define OPCODE_fCAST ((knh_opcode_t)10)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fCAST_t;

#define OPSIZE_fCAST  (sizeof(klr_fCAST_t)-sizeof(void*))

#define OPCODE_OSET ((knh_opcode_t)11)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_Object_t* a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_OSET_t;

#define OPSIZE_OSET  (sizeof(klr_OSET_t)-sizeof(void*))

#define OPCODE_NSET ((knh_opcode_t)12)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_int_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_NSET_t;

#define OPSIZE_NSET  (sizeof(klr_NSET_t)-sizeof(void*))

#define OPCODE_MOV ((knh_opcode_t)13)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_MOV_t;

#define OPSIZE_MOV  (sizeof(klr_MOV_t)-sizeof(void*))

#define OPCODE_OMOV ((knh_opcode_t)14)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_OMOV_t;

#define OPSIZE_OMOV  (sizeof(klr_OMOV_t)-sizeof(void*))

#define OPCODE_NMOV ((knh_opcode_t)15)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_NMOV_t;

#define OPSIZE_NMOV  (sizeof(klr_NMOV_t)-sizeof(void*))

#define OPCODE_OMOVx ((knh_opcode_t)16)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_OMOVx_t;

#define OPSIZE_OMOVx  (sizeof(klr_OMOVx_t)-sizeof(void*))

#define OPCODE_iMOVx ((knh_opcode_t)17)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iMOVx_t;

#define OPSIZE_iMOVx  (sizeof(klr_iMOVx_t)-sizeof(void*))

#define OPCODE_fMOVx ((knh_opcode_t)18)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fMOVx_t;

#define OPSIZE_fMOVx  (sizeof(klr_fMOVx_t)-sizeof(void*))

#define OPCODE_bMOVx ((knh_opcode_t)19)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_bMOVx_t;

#define OPSIZE_bMOVx  (sizeof(klr_bMOVx_t)-sizeof(void*))

#define OPCODE_XMOV ((knh_opcode_t)20)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfx_t a1;
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_XMOV_t;

#define OPSIZE_XMOV  (sizeof(klr_XMOV_t)-sizeof(void*))

#define OPCODE_XMOVx ((knh_opcode_t)21)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfx_t a1;
	knh_sfx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_XMOVx_t;

#define OPSIZE_XMOVx  (sizeof(klr_XMOVx_t)-sizeof(void*))

#define OPCODE_XOSET ((knh_opcode_t)22)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfx_t a1;
	knh_Object_t* a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_XOSET_t;

#define OPSIZE_XOSET  (sizeof(klr_XOSET_t)-sizeof(void*))

#define OPCODE_XIMOV ((knh_opcode_t)23)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfx_t a1;
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_XIMOV_t;

#define OPSIZE_XIMOV  (sizeof(klr_XIMOV_t)-sizeof(void*))

#define OPCODE_XFMOV ((knh_opcode_t)24)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfx_t a1;
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_XFMOV_t;

#define OPSIZE_XFMOV  (sizeof(klr_XFMOV_t)-sizeof(void*))

#define OPCODE_XBMOV ((knh_opcode_t)25)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfx_t a1;
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_XBMOV_t;

#define OPSIZE_XBMOV  (sizeof(klr_XBMOV_t)-sizeof(void*))

#define OPCODE_MOVe ((knh_opcode_t)26)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_ushort_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_MOVe_t;

#define OPSIZE_MOVe  (sizeof(klr_MOVe_t)-sizeof(void*))

#define OPCODE_SWAP ((knh_opcode_t)27)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_SWAP_t;

#define OPSIZE_SWAP  (sizeof(klr_SWAP_t)-sizeof(void*))

#define OPCODE_CHKNUL ((knh_opcode_t)28)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	void *nextaddr;   /* sameas next codeaddr */
} klr_CHKNUL_t;

#define OPSIZE_CHKNUL  (sizeof(klr_CHKNUL_t)-sizeof(void*))

#define OPCODE_CHKTYPE ((knh_opcode_t)29)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_class_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_CHKTYPE_t;

#define OPSIZE_CHKTYPE  (sizeof(klr_CHKTYPE_t)-sizeof(void*))

#define OPCODE_CHKNULx ((knh_opcode_t)30)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfx_t a1;
	void *nextaddr;   /* sameas next codeaddr */
} klr_CHKNULx_t;

#define OPSIZE_CHKNULx  (sizeof(klr_CHKNULx_t)-sizeof(void*))

#define OPCODE_CHKTYPEx ((knh_opcode_t)31)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfx_t a1;
	knh_class_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_CHKTYPEx_t;

#define OPSIZE_CHKTYPEx  (sizeof(klr_CHKTYPEx_t)-sizeof(void*))

#define OPCODE_LDMETHOD ((knh_opcode_t)32)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	klr_Fmethod a1;
	knh_sfpidx_t a2;
	knh_methodn_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_LDMETHOD_t;

#define OPSIZE_LDMETHOD  (sizeof(klr_LDMETHOD_t)-sizeof(void*))

#define OPCODE_CALL ((knh_opcode_t)33)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_ushort_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_CALL_t;

#define OPSIZE_CALL  (sizeof(klr_CALL_t)-sizeof(void*))

#define OPCODE_SCALL ((knh_opcode_t)34)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_ushort_t a2;
	knh_Method_t* a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_SCALL_t;

#define OPSIZE_SCALL  (sizeof(klr_SCALL_t)-sizeof(void*))

#define OPCODE_VCALL ((knh_opcode_t)35)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_ushort_t a2;
	knh_Method_t* a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_VCALL_t;

#define OPSIZE_VCALL  (sizeof(klr_VCALL_t)-sizeof(void*))

#define OPCODE_RET ((knh_opcode_t)36)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	void *nextaddr;   /* sameas next codeaddr */
} klr_RET_t;

#define OPSIZE_RET  (sizeof(klr_RET_t)-sizeof(void*))

#define OPCODE_FUNCCALL ((knh_opcode_t)37)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	void *nextaddr;   /* sameas next codeaddr */
} klr_FUNCCALL_t;

#define OPSIZE_FUNCCALL  (sizeof(klr_FUNCCALL_t)-sizeof(void*))

#define OPCODE_SMAP ((knh_opcode_t)38)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_Mapper_t* a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_SMAP_t;

#define OPSIZE_SMAP  (sizeof(klr_SMAP_t)-sizeof(void*))

#define OPCODE_SMAPnc ((knh_opcode_t)39)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_Mapper_t* a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_SMAPnc_t;

#define OPSIZE_SMAPnc  (sizeof(klr_SMAPnc_t)-sizeof(void*))

#define OPCODE_MAP ((knh_opcode_t)40)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_class_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_MAP_t;

#define OPSIZE_MAP  (sizeof(klr_MAP_t)-sizeof(void*))

#define OPCODE_MAPnc ((knh_opcode_t)41)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_class_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_MAPnc_t;

#define OPSIZE_MAPnc  (sizeof(klr_MAPnc_t)-sizeof(void*))

#define OPCODE_AMAP ((knh_opcode_t)42)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_class_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_AMAP_t;

#define OPSIZE_AMAP  (sizeof(klr_AMAP_t)-sizeof(void*))

#define OPCODE_JMP ((knh_opcode_t)43)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	THREADEDCODE(void *jumpaddr;)
	knh_KLRInst_t* a1;  /* label */
	void *nextaddr;   /* sameas next codeaddr */
} klr_JMP_t;

#define OPSIZE_JMP  (sizeof(klr_JMP_t)-sizeof(void*))

#define OPCODE_NOPJMP ((knh_opcode_t)44)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	THREADEDCODE(void *jumpaddr;)
	knh_KLRInst_t* a1;  /* label */
	void *nextaddr;   /* sameas next codeaddr */
} klr_NOPJMP_t;

#define OPSIZE_NOPJMP  (sizeof(klr_NOPJMP_t)-sizeof(void*))

#define OPCODE_JMPT ((knh_opcode_t)45)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	THREADEDCODE(void *jumpaddr;)
	knh_KLRInst_t* a1;  /* label */
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_JMPT_t;

#define OPSIZE_JMPT  (sizeof(klr_JMPT_t)-sizeof(void*))

#define OPCODE_JMPF ((knh_opcode_t)46)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	THREADEDCODE(void *jumpaddr;)
	knh_KLRInst_t* a1;  /* label */
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_JMPF_t;

#define OPSIZE_JMPF  (sizeof(klr_JMPF_t)-sizeof(void*))

#define OPCODE_JMPF_LOOP ((knh_opcode_t)47)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	THREADEDCODE(void *jumpaddr;)
	knh_KLRInst_t* a1;  /* label */
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_JMPF_LOOP_t;

#define OPSIZE_JMPF_LOOP  (sizeof(klr_JMPF_LOOP_t)-sizeof(void*))

#define OPCODE_JMPNUL ((knh_opcode_t)48)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	THREADEDCODE(void *jumpaddr;)
	knh_KLRInst_t* a1;  /* label */
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_JMPNUL_t;

#define OPSIZE_JMPNUL  (sizeof(klr_JMPNUL_t)-sizeof(void*))

#define OPCODE_JMPNN ((knh_opcode_t)49)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	THREADEDCODE(void *jumpaddr;)
	knh_KLRInst_t* a1;  /* label */
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_JMPNN_t;

#define OPSIZE_JMPNN  (sizeof(klr_JMPNN_t)-sizeof(void*))

#define OPCODE_JMPchk ((knh_opcode_t)50)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	THREADEDCODE(void *jumpaddr;)
	knh_KLRInst_t* a1;  /* label */
	klr_Fchk a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_JMPchk_t;

#define OPSIZE_JMPchk  (sizeof(klr_JMPchk_t)-sizeof(void*))

#define OPCODE_JMPcmp ((knh_opcode_t)51)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	THREADEDCODE(void *jumpaddr;)
	knh_KLRInst_t* a1;  /* label */
	klr_Fcmp a2;
	knh_sfpidx_t a3;
	knh_sfpidx_t a4;
	void *nextaddr;   /* sameas next codeaddr */
} klr_JMPcmp_t;

#define OPSIZE_JMPcmp  (sizeof(klr_JMPcmp_t)-sizeof(void*))

#define OPCODE_JMPcmpi ((knh_opcode_t)52)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	THREADEDCODE(void *jumpaddr;)
	knh_KLRInst_t* a1;  /* label */
	klr_Fcmpi a2;
	knh_sfpidx_t a3;
	knh_int_t a4;
	void *nextaddr;   /* sameas next codeaddr */
} klr_JMPcmpi_t;

#define OPSIZE_JMPcmpi  (sizeof(klr_JMPcmpi_t)-sizeof(void*))

#define OPCODE_JMPcmpf ((knh_opcode_t)53)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	THREADEDCODE(void *jumpaddr;)
	knh_KLRInst_t* a1;  /* label */
	klr_Fcmpf a2;
	knh_sfpidx_t a3;
	knh_float_t a4;
	void *nextaddr;   /* sameas next codeaddr */
} klr_JMPcmpf_t;

#define OPSIZE_JMPcmpf  (sizeof(klr_JMPcmpf_t)-sizeof(void*))

#define OPCODE_NEXT ((knh_opcode_t)54)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	THREADEDCODE(void *jumpaddr;)
	knh_KLRInst_t* a1;  /* label */
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_NEXT_t;

#define OPSIZE_NEXT  (sizeof(klr_NEXT_t)-sizeof(void*))

#define OPCODE_NEXTf ((knh_opcode_t)55)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	THREADEDCODE(void *jumpaddr;)
	knh_KLRInst_t* a1;  /* label */
	klr_Fnext a2;
	knh_class_t a3;
	knh_sfpidx_t a4;
	knh_sfpidx_t a5;
	void *nextaddr;   /* sameas next codeaddr */
} klr_NEXTf_t;

#define OPSIZE_NEXTf  (sizeof(klr_NEXTf_t)-sizeof(void*))

#define OPCODE_TRY ((knh_opcode_t)56)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	THREADEDCODE(void *jumpaddr;)
	knh_KLRInst_t* a1;  /* label */
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_TRY_t;

#define OPSIZE_TRY  (sizeof(klr_TRY_t)-sizeof(void*))

#define OPCODE_TRYEND ((knh_opcode_t)57)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	void *nextaddr;   /* sameas next codeaddr */
} klr_TRYEND_t;

#define OPSIZE_TRYEND  (sizeof(klr_TRYEND_t)-sizeof(void*))

#define OPCODE_CATCH ((knh_opcode_t)58)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	THREADEDCODE(void *jumpaddr;)
	knh_KLRInst_t* a1;  /* label */
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	knh_String_t* a4;
	void *nextaddr;   /* sameas next codeaddr */
} klr_CATCH_t;

#define OPSIZE_CATCH  (sizeof(klr_CATCH_t)-sizeof(void*))

#define OPCODE_THROW ((knh_opcode_t)59)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_THROW_t;

#define OPSIZE_THROW  (sizeof(klr_THROW_t)-sizeof(void*))

#define OPCODE_THROWs ((knh_opcode_t)60)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_String_t* a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_THROWs_t;

#define OPSIZE_THROWs  (sizeof(klr_THROWs_t)-sizeof(void*))

#define OPCODE_THROW_AGAIN ((knh_opcode_t)61)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	void *nextaddr;   /* sameas next codeaddr */
} klr_THROW_AGAIN_t;

#define OPSIZE_THROW_AGAIN  (sizeof(klr_THROW_AGAIN_t)-sizeof(void*))

#define OPCODE_P ((knh_opcode_t)62)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_flag_t a1;
	knh_methodn_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_P_t;

#define OPSIZE_P  (sizeof(klr_P_t)-sizeof(void*))

#define OPCODE_PMSG ((knh_opcode_t)63)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_flag_t a1;
	knh_String_t* a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_PMSG_t;

#define OPSIZE_PMSG  (sizeof(klr_PMSG_t)-sizeof(void*))

#define OPCODE_bNOT ((knh_opcode_t)64)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_bNOT_t;

#define OPSIZE_bNOT  (sizeof(klr_bNOT_t)-sizeof(void*))

#define OPCODE_iNEG ((knh_opcode_t)65)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iNEG_t;

#define OPSIZE_iNEG  (sizeof(klr_iNEG_t)-sizeof(void*))

#define OPCODE_iADD ((knh_opcode_t)66)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iADD_t;

#define OPSIZE_iADD  (sizeof(klr_iADD_t)-sizeof(void*))

#define OPCODE_iSUB ((knh_opcode_t)67)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iSUB_t;

#define OPSIZE_iSUB  (sizeof(klr_iSUB_t)-sizeof(void*))

#define OPCODE_iMUL ((knh_opcode_t)68)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iMUL_t;

#define OPSIZE_iMUL  (sizeof(klr_iMUL_t)-sizeof(void*))

#define OPCODE_iDIV ((knh_opcode_t)69)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iDIV_t;

#define OPSIZE_iDIV  (sizeof(klr_iDIV_t)-sizeof(void*))

#define OPCODE_iMOD ((knh_opcode_t)70)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iMOD_t;

#define OPSIZE_iMOD  (sizeof(klr_iMOD_t)-sizeof(void*))

#define OPCODE_iDIVn ((knh_opcode_t)71)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_int_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iDIVn_t;

#define OPSIZE_iDIVn  (sizeof(klr_iDIVn_t)-sizeof(void*))

#define OPCODE_iADDn ((knh_opcode_t)72)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_int_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iADDn_t;

#define OPSIZE_iADDn  (sizeof(klr_iADDn_t)-sizeof(void*))

#define OPCODE_iSUBn ((knh_opcode_t)73)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_int_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iSUBn_t;

#define OPSIZE_iSUBn  (sizeof(klr_iSUBn_t)-sizeof(void*))

#define OPCODE_iMULn ((knh_opcode_t)74)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_int_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iMULn_t;

#define OPSIZE_iMULn  (sizeof(klr_iMULn_t)-sizeof(void*))

#define OPCODE_iMODn ((knh_opcode_t)75)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_int_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iMODn_t;

#define OPSIZE_iMODn  (sizeof(klr_iMODn_t)-sizeof(void*))

#define OPCODE_iEQ ((knh_opcode_t)76)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iEQ_t;

#define OPSIZE_iEQ  (sizeof(klr_iEQ_t)-sizeof(void*))

#define OPCODE_iNEQ ((knh_opcode_t)77)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iNEQ_t;

#define OPSIZE_iNEQ  (sizeof(klr_iNEQ_t)-sizeof(void*))

#define OPCODE_iLT ((knh_opcode_t)78)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iLT_t;

#define OPSIZE_iLT  (sizeof(klr_iLT_t)-sizeof(void*))

#define OPCODE_iLTE ((knh_opcode_t)79)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iLTE_t;

#define OPSIZE_iLTE  (sizeof(klr_iLTE_t)-sizeof(void*))

#define OPCODE_iGT ((knh_opcode_t)80)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iGT_t;

#define OPSIZE_iGT  (sizeof(klr_iGT_t)-sizeof(void*))

#define OPCODE_iGTE ((knh_opcode_t)81)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iGTE_t;

#define OPSIZE_iGTE  (sizeof(klr_iGTE_t)-sizeof(void*))

#define OPCODE_iEQn ((knh_opcode_t)82)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_int_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iEQn_t;

#define OPSIZE_iEQn  (sizeof(klr_iEQn_t)-sizeof(void*))

#define OPCODE_iNEQn ((knh_opcode_t)83)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_int_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iNEQn_t;

#define OPSIZE_iNEQn  (sizeof(klr_iNEQn_t)-sizeof(void*))

#define OPCODE_iLTn ((knh_opcode_t)84)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_int_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iLTn_t;

#define OPSIZE_iLTn  (sizeof(klr_iLTn_t)-sizeof(void*))

#define OPCODE_iLTEn ((knh_opcode_t)85)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_int_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iLTEn_t;

#define OPSIZE_iLTEn  (sizeof(klr_iLTEn_t)-sizeof(void*))

#define OPCODE_iGTn ((knh_opcode_t)86)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_int_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iGTn_t;

#define OPSIZE_iGTn  (sizeof(klr_iGTn_t)-sizeof(void*))

#define OPCODE_iGTEn ((knh_opcode_t)87)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_int_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iGTEn_t;

#define OPSIZE_iGTEn  (sizeof(klr_iGTEn_t)-sizeof(void*))

#define OPCODE_fNEG ((knh_opcode_t)88)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fNEG_t;

#define OPSIZE_fNEG  (sizeof(klr_fNEG_t)-sizeof(void*))

#define OPCODE_fADD ((knh_opcode_t)89)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fADD_t;

#define OPSIZE_fADD  (sizeof(klr_fADD_t)-sizeof(void*))

#define OPCODE_fSUB ((knh_opcode_t)90)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fSUB_t;

#define OPSIZE_fSUB  (sizeof(klr_fSUB_t)-sizeof(void*))

#define OPCODE_fMUL ((knh_opcode_t)91)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fMUL_t;

#define OPSIZE_fMUL  (sizeof(klr_fMUL_t)-sizeof(void*))

#define OPCODE_fDIV ((knh_opcode_t)92)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fDIV_t;

#define OPSIZE_fDIV  (sizeof(klr_fDIV_t)-sizeof(void*))

#define OPCODE_fADDn ((knh_opcode_t)93)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_float_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fADDn_t;

#define OPSIZE_fADDn  (sizeof(klr_fADDn_t)-sizeof(void*))

#define OPCODE_fSUBn ((knh_opcode_t)94)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_float_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fSUBn_t;

#define OPSIZE_fSUBn  (sizeof(klr_fSUBn_t)-sizeof(void*))

#define OPCODE_fMULn ((knh_opcode_t)95)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_float_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fMULn_t;

#define OPSIZE_fMULn  (sizeof(klr_fMULn_t)-sizeof(void*))

#define OPCODE_fDIVn ((knh_opcode_t)96)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_float_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fDIVn_t;

#define OPSIZE_fDIVn  (sizeof(klr_fDIVn_t)-sizeof(void*))

#define OPCODE_fEQ ((knh_opcode_t)97)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fEQ_t;

#define OPSIZE_fEQ  (sizeof(klr_fEQ_t)-sizeof(void*))

#define OPCODE_fNEQ ((knh_opcode_t)98)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fNEQ_t;

#define OPSIZE_fNEQ  (sizeof(klr_fNEQ_t)-sizeof(void*))

#define OPCODE_fEQn ((knh_opcode_t)99)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_float_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fEQn_t;

#define OPSIZE_fEQn  (sizeof(klr_fEQn_t)-sizeof(void*))

#define OPCODE_fNEQn ((knh_opcode_t)100)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_float_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fNEQn_t;

#define OPSIZE_fNEQn  (sizeof(klr_fNEQn_t)-sizeof(void*))

#define OPCODE_fLT ((knh_opcode_t)101)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fLT_t;

#define OPSIZE_fLT  (sizeof(klr_fLT_t)-sizeof(void*))

#define OPCODE_fLTE ((knh_opcode_t)102)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fLTE_t;

#define OPSIZE_fLTE  (sizeof(klr_fLTE_t)-sizeof(void*))

#define OPCODE_fGT ((knh_opcode_t)103)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fGT_t;

#define OPSIZE_fGT  (sizeof(klr_fGT_t)-sizeof(void*))

#define OPCODE_fGTE ((knh_opcode_t)104)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fGTE_t;

#define OPSIZE_fGTE  (sizeof(klr_fGTE_t)-sizeof(void*))

#define OPCODE_fLTn ((knh_opcode_t)105)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_float_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fLTn_t;

#define OPSIZE_fLTn  (sizeof(klr_fLTn_t)-sizeof(void*))

#define OPCODE_fLTEn ((knh_opcode_t)106)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_float_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fLTEn_t;

#define OPSIZE_fLTEn  (sizeof(klr_fLTEn_t)-sizeof(void*))

#define OPCODE_fGTn ((knh_opcode_t)107)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_float_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fGTn_t;

#define OPSIZE_fGTn  (sizeof(klr_fGTn_t)-sizeof(void*))

#define OPCODE_fGTEn ((knh_opcode_t)108)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_float_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fGTEn_t;

#define OPSIZE_fGTEn  (sizeof(klr_fGTEn_t)-sizeof(void*))

#define OPCODE_OEVAL ((knh_opcode_t)109)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	klr_Foeval a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_OEVAL_t;

#define OPSIZE_OEVAL  (sizeof(klr_OEVAL_t)-sizeof(void*))

#define OPCODE_GETIDX ((knh_opcode_t)110)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	klr_Fgetidx a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	knh_sfpidx_t a4;
	void *nextaddr;   /* sameas next codeaddr */
} klr_GETIDX_t;

#define OPSIZE_GETIDX  (sizeof(klr_GETIDX_t)-sizeof(void*))

#define OPCODE_SETIDX ((knh_opcode_t)111)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	klr_Fsetidx a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	knh_sfpidx_t a4;
	knh_sfpidx_t a5;
	void *nextaddr;   /* sameas next codeaddr */
} klr_SETIDX_t;

#define OPSIZE_SETIDX  (sizeof(klr_SETIDX_t)-sizeof(void*))

#define OPCODE_GETIDXn ((knh_opcode_t)112)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	klr_Fgetidx a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	knh_intptr_t a4;
	void *nextaddr;   /* sameas next codeaddr */
} klr_GETIDXn_t;

#define OPSIZE_GETIDXn  (sizeof(klr_GETIDXn_t)-sizeof(void*))

#define OPCODE_SETIDXn ((knh_opcode_t)113)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	klr_Fsetidx a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	knh_sfpidx_t a4;
	knh_intptr_t a5;
	void *nextaddr;   /* sameas next codeaddr */
} klr_SETIDXn_t;

#define OPSIZE_SETIDXn  (sizeof(klr_SETIDXn_t)-sizeof(void*))

#define OPCODE_THCODE ((knh_opcode_t)114)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	void *nextaddr;   /* sameas next codeaddr */
} klr_THCODE_t;

#define OPSIZE_THCODE  (sizeof(klr_THCODE_t)-sizeof(void*))

#define OPCODE_LABEL ((knh_opcode_t)115)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_intptr_t a1;
	knh_String_t* a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_LABEL_t;

#define OPSIZE_LABEL  (sizeof(klr_LABEL_t)-sizeof(void*))

#define OPCODE_PROBE ((knh_opcode_t)116)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	klr_Fprobe a1;
	knh_ushort_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_PROBE_t;

#define OPSIZE_PROBE  (sizeof(klr_PROBE_t)-sizeof(void*))

#define OPCODE_NOP ((knh_opcode_t)117)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	void *nextaddr;   /* sameas next codeaddr */
} klr_NOP_t;

#define OPSIZE_NOP  (sizeof(klr_NOP_t)-sizeof(void*))
#endif /* KONOHA_CODE_H_ */
