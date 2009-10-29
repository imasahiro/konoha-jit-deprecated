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

#define OPCODE_MOVa ((knh_opcode_t)1)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_MOVa_t;

#define OPSIZE_MOVa  (sizeof(klr_MOVa_t)-sizeof(void*))

#define OPCODE_MOVn ((knh_opcode_t)2)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_MOVn_t;

#define OPSIZE_MOVn  (sizeof(klr_MOVn_t)-sizeof(void*))

#define OPCODE_MOVo ((knh_opcode_t)3)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_Object_t* a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_MOVo_t;

#define OPSIZE_MOVo  (sizeof(klr_MOVo_t)-sizeof(void*))

#define OPCODE_MOVi ((knh_opcode_t)4)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_int_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_MOVi_t;

#define OPSIZE_MOVi  (sizeof(klr_MOVi_t)-sizeof(void*))

#define OPCODE_MOVx ((knh_opcode_t)5)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_MOVx_t;

#define OPSIZE_MOVx  (sizeof(klr_MOVx_t)-sizeof(void*))

#define OPCODE_MOVDEF ((knh_opcode_t)6)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_class_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_MOVDEF_t;

#define OPSIZE_MOVDEF  (sizeof(klr_MOVDEF_t)-sizeof(void*))

#define OPCODE_MOVSYS ((knh_opcode_t)7)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_ushort_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_MOVSYS_t;

#define OPSIZE_MOVSYS  (sizeof(klr_MOVSYS_t)-sizeof(void*))

#define OPCODE_XMOVs ((knh_opcode_t)8)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfx_t a1;
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_XMOVs_t;

#define OPSIZE_XMOVs  (sizeof(klr_XMOVs_t)-sizeof(void*))

#define OPCODE_XMOVo ((knh_opcode_t)9)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfx_t a1;
	knh_Object_t* a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_XMOVo_t;

#define OPSIZE_XMOVo  (sizeof(klr_XMOVo_t)-sizeof(void*))

#define OPCODE_XMOVx ((knh_opcode_t)10)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfx_t a1;
	knh_sfx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_XMOVx_t;

#define OPSIZE_XMOVx  (sizeof(klr_XMOVx_t)-sizeof(void*))

#define OPCODE_XMOVDEF ((knh_opcode_t)11)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfx_t a1;
	knh_class_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_XMOVDEF_t;

#define OPSIZE_XMOVDEF  (sizeof(klr_XMOVDEF_t)-sizeof(void*))

#define OPCODE_XMOVSYS ((knh_opcode_t)12)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfx_t a1;
	knh_ushort_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_XMOVSYS_t;

#define OPSIZE_XMOVSYS  (sizeof(klr_XMOVSYS_t)-sizeof(void*))

#define OPCODE_MOVxi ((knh_opcode_t)13)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_MOVxi_t;

#define OPSIZE_MOVxi  (sizeof(klr_MOVxi_t)-sizeof(void*))

#define OPCODE_XMOVsi ((knh_opcode_t)14)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfx_t a1;
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_XMOVsi_t;

#define OPSIZE_XMOVsi  (sizeof(klr_XMOVsi_t)-sizeof(void*))

#define OPCODE_XMOVoi ((knh_opcode_t)15)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfx_t a1;
	knh_Object_t* a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_XMOVoi_t;

#define OPSIZE_XMOVoi  (sizeof(klr_XMOVoi_t)-sizeof(void*))

#define OPCODE_XMOVxi ((knh_opcode_t)16)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfx_t a1;
	knh_sfx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_XMOVxi_t;

#define OPSIZE_XMOVxi  (sizeof(klr_XMOVxi_t)-sizeof(void*))

#define OPCODE_XMOVxio ((knh_opcode_t)17)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfx_t a1;
	knh_sfx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_XMOVxio_t;

#define OPSIZE_XMOVxio  (sizeof(klr_XMOVxio_t)-sizeof(void*))

#define OPCODE_XMOVxBXi ((knh_opcode_t)18)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfx_t a1;
	knh_sfx_t a2;
	knh_class_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_XMOVxBXi_t;

#define OPSIZE_XMOVxBXi  (sizeof(klr_XMOVxBXi_t)-sizeof(void*))

#define OPCODE_MOVxf ((knh_opcode_t)19)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_MOVxf_t;

#define OPSIZE_MOVxf  (sizeof(klr_MOVxf_t)-sizeof(void*))

#define OPCODE_XMOVsf ((knh_opcode_t)20)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfx_t a1;
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_XMOVsf_t;

#define OPSIZE_XMOVsf  (sizeof(klr_XMOVsf_t)-sizeof(void*))

#define OPCODE_XMOVof ((knh_opcode_t)21)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfx_t a1;
	knh_Object_t* a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_XMOVof_t;

#define OPSIZE_XMOVof  (sizeof(klr_XMOVof_t)-sizeof(void*))

#define OPCODE_XMOVxf ((knh_opcode_t)22)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfx_t a1;
	knh_sfx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_XMOVxf_t;

#define OPSIZE_XMOVxf  (sizeof(klr_XMOVxf_t)-sizeof(void*))

#define OPCODE_XMOVxfo ((knh_opcode_t)23)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfx_t a1;
	knh_sfx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_XMOVxfo_t;

#define OPSIZE_XMOVxfo  (sizeof(klr_XMOVxfo_t)-sizeof(void*))

#define OPCODE_XMOVxBXf ((knh_opcode_t)24)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfx_t a1;
	knh_sfx_t a2;
	knh_class_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_XMOVxBXf_t;

#define OPSIZE_XMOVxBXf  (sizeof(klr_XMOVxBXf_t)-sizeof(void*))

#define OPCODE_MOVxb ((knh_opcode_t)25)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_MOVxb_t;

#define OPSIZE_MOVxb  (sizeof(klr_MOVxb_t)-sizeof(void*))

#define OPCODE_XMOVsb ((knh_opcode_t)26)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfx_t a1;
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_XMOVsb_t;

#define OPSIZE_XMOVsb  (sizeof(klr_XMOVsb_t)-sizeof(void*))

#define OPCODE_XMOVob ((knh_opcode_t)27)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfx_t a1;
	knh_Object_t* a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_XMOVob_t;

#define OPSIZE_XMOVob  (sizeof(klr_XMOVob_t)-sizeof(void*))

#define OPCODE_XMOVxb ((knh_opcode_t)28)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfx_t a1;
	knh_sfx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_XMOVxb_t;

#define OPSIZE_XMOVxb  (sizeof(klr_XMOVxb_t)-sizeof(void*))

#define OPCODE_EMOVs ((knh_opcode_t)29)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfe_t a1;
	knh_sfe_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_EMOVs_t;

#define OPSIZE_EMOVs  (sizeof(klr_EMOVs_t)-sizeof(void*))

#define OPCODE_EMOVo ((knh_opcode_t)30)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfe_t a1;
	knh_Object_t* a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_EMOVo_t;

#define OPSIZE_EMOVo  (sizeof(klr_EMOVo_t)-sizeof(void*))

#define OPCODE_EMOVe ((knh_opcode_t)31)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfe_t a1;
	knh_sfe_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_EMOVe_t;

#define OPSIZE_EMOVe  (sizeof(klr_EMOVe_t)-sizeof(void*))

#define OPCODE_EMOVDEF ((knh_opcode_t)32)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfe_t a1;
	knh_class_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_EMOVDEF_t;

#define OPSIZE_EMOVDEF  (sizeof(klr_EMOVDEF_t)-sizeof(void*))

#define OPCODE_EMOVSYS ((knh_opcode_t)33)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfe_t a1;
	knh_ushort_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_EMOVSYS_t;

#define OPSIZE_EMOVSYS  (sizeof(klr_EMOVSYS_t)-sizeof(void*))

#define OPCODE_SWAP ((knh_opcode_t)34)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_SWAP_t;

#define OPSIZE_SWAP  (sizeof(klr_SWAP_t)-sizeof(void*))

#define OPCODE_PARAMDEF ((knh_opcode_t)35)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_class_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_PARAMDEF_t;

#define OPSIZE_PARAMDEF  (sizeof(klr_PARAMDEF_t)-sizeof(void*))

#define OPCODE_PARAMo ((knh_opcode_t)36)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_Object_t* a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_PARAMo_t;

#define OPSIZE_PARAMo  (sizeof(klr_PARAMo_t)-sizeof(void*))

#define OPCODE_PARAMPROP ((knh_opcode_t)37)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_PARAMPROP_t;

#define OPSIZE_PARAMPROP  (sizeof(klr_PARAMPROP_t)-sizeof(void*))

#define OPCODE_PARAMS ((knh_opcode_t)38)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_class_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_PARAMS_t;

#define OPSIZE_PARAMS  (sizeof(klr_PARAMS_t)-sizeof(void*))

#define OPCODE_CHKESP ((knh_opcode_t)39)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	void *nextaddr;   /* sameas next codeaddr */
} klr_CHKESP_t;

#define OPSIZE_CHKESP  (sizeof(klr_CHKESP_t)-sizeof(void*))

#define OPCODE_RET ((knh_opcode_t)40)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	void *nextaddr;   /* sameas next codeaddr */
} klr_RET_t;

#define OPSIZE_RET  (sizeof(klr_RET_t)-sizeof(void*))

#define OPCODE_RETn ((knh_opcode_t)41)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_RETn_t;

#define OPSIZE_RETn  (sizeof(klr_RETn_t)-sizeof(void*))

#define OPCODE_RETa ((knh_opcode_t)42)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_RETa_t;

#define OPSIZE_RETa  (sizeof(klr_RETa_t)-sizeof(void*))

#define OPCODE_RETo ((knh_opcode_t)43)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_Object_t* a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_RETo_t;

#define OPSIZE_RETo  (sizeof(klr_RETo_t)-sizeof(void*))

#define OPCODE_RETx ((knh_opcode_t)44)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_RETx_t;

#define OPSIZE_RETx  (sizeof(klr_RETx_t)-sizeof(void*))

#define OPCODE_YEILDBREAK ((knh_opcode_t)45)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	void *nextaddr;   /* sameas next codeaddr */
} klr_YEILDBREAK_t;

#define OPSIZE_YEILDBREAK  (sizeof(klr_YEILDBREAK_t)-sizeof(void*))

#define OPCODE_BOX ((knh_opcode_t)46)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_class_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_BOX_t;

#define OPSIZE_BOX  (sizeof(klr_BOX_t)-sizeof(void*))

#define OPCODE_BOXnc ((knh_opcode_t)47)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_class_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_BOXnc_t;

#define OPSIZE_BOXnc  (sizeof(klr_BOXnc_t)-sizeof(void*))

#define OPCODE_NNBOX ((knh_opcode_t)48)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_class_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_NNBOX_t;

#define OPSIZE_NNBOX  (sizeof(klr_NNBOX_t)-sizeof(void*))

#define OPCODE_NNBOXnc ((knh_opcode_t)49)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_class_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_NNBOXnc_t;

#define OPSIZE_NNBOXnc  (sizeof(klr_NNBOXnc_t)-sizeof(void*))

#define OPCODE_UNBOX ((knh_opcode_t)50)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	void *nextaddr;   /* sameas next codeaddr */
} klr_UNBOX_t;

#define OPSIZE_UNBOX  (sizeof(klr_UNBOX_t)-sizeof(void*))

#define OPCODE_CHKNUL ((knh_opcode_t)51)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	void *nextaddr;   /* sameas next codeaddr */
} klr_CHKNUL_t;

#define OPSIZE_CHKNUL  (sizeof(klr_CHKNUL_t)-sizeof(void*))

#define OPCODE_CHKNULx ((knh_opcode_t)52)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfx_t a1;
	void *nextaddr;   /* sameas next codeaddr */
} klr_CHKNULx_t;

#define OPSIZE_CHKNULx  (sizeof(klr_CHKNULx_t)-sizeof(void*))

#define OPCODE_CHKTYPE ((knh_opcode_t)53)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_class_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_CHKTYPE_t;

#define OPSIZE_CHKTYPE  (sizeof(klr_CHKTYPE_t)-sizeof(void*))

#define OPCODE_FCALL ((knh_opcode_t)54)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_ushort_t a2;
	knh_sfpidx_t a3;
	knh_Method_t* a4;
	void *nextaddr;   /* sameas next codeaddr */
} klr_FCALL_t;

#define OPSIZE_FCALL  (sizeof(klr_FCALL_t)-sizeof(void*))

#define OPCODE_RCALL ((knh_opcode_t)55)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_ushort_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_RCALL_t;

#define OPSIZE_RCALL  (sizeof(klr_RCALL_t)-sizeof(void*))

#define OPCODE_SCALL ((knh_opcode_t)56)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_ushort_t a2;
	knh_Method_t* a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_SCALL_t;

#define OPSIZE_SCALL  (sizeof(klr_SCALL_t)-sizeof(void*))

#define OPCODE_AINVOKE ((knh_opcode_t)57)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_ushort_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_AINVOKE_t;

#define OPSIZE_AINVOKE  (sizeof(klr_AINVOKE_t)-sizeof(void*))

#define OPCODE_CALL ((knh_opcode_t)58)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_ushort_t a2;
	knh_methodn_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_CALL_t;

#define OPSIZE_CALL  (sizeof(klr_CALL_t)-sizeof(void*))

#define OPCODE_ACALL ((knh_opcode_t)59)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_ushort_t a2;
	knh_methodn_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_ACALL_t;

#define OPSIZE_ACALL  (sizeof(klr_ACALL_t)-sizeof(void*))

#define OPCODE_NEW ((knh_opcode_t)60)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_flag_t a2;
	knh_class_t a3;
	knh_ushort_t a4;
	knh_Method_t* a5;
	void *nextaddr;   /* sameas next codeaddr */
} klr_NEW_t;

#define OPSIZE_NEW  (sizeof(klr_NEW_t)-sizeof(void*))

#define OPCODE_COPYSFP ((knh_opcode_t)61)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	void *nextaddr;   /* sameas next codeaddr */
} klr_COPYSFP_t;

#define OPSIZE_COPYSFP  (sizeof(klr_COPYSFP_t)-sizeof(void*))

#define OPCODE_TOSTR ((knh_opcode_t)62)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_methodn_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_TOSTR_t;

#define OPSIZE_TOSTR  (sizeof(klr_TOSTR_t)-sizeof(void*))

#define OPCODE_TOSTRf ((knh_opcode_t)63)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_methodn_t a2;
	knh_Object_t* a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_TOSTRf_t;

#define OPSIZE_TOSTRf  (sizeof(klr_TOSTRf_t)-sizeof(void*))

#define OPCODE_SMAP ((knh_opcode_t)64)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_Mapper_t* a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_SMAP_t;

#define OPSIZE_SMAP  (sizeof(klr_SMAP_t)-sizeof(void*))

#define OPCODE_SMAPnc ((knh_opcode_t)65)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_Mapper_t* a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_SMAPnc_t;

#define OPSIZE_SMAPnc  (sizeof(klr_SMAPnc_t)-sizeof(void*))

#define OPCODE_MAP ((knh_opcode_t)66)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_class_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_MAP_t;

#define OPSIZE_MAP  (sizeof(klr_MAP_t)-sizeof(void*))

#define OPCODE_MAPnc ((knh_opcode_t)67)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_class_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_MAPnc_t;

#define OPSIZE_MAPnc  (sizeof(klr_MAPnc_t)-sizeof(void*))

#define OPCODE_AMAP ((knh_opcode_t)68)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_class_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_AMAP_t;

#define OPSIZE_AMAP  (sizeof(klr_AMAP_t)-sizeof(void*))

#define OPCODE_NNMAP ((knh_opcode_t)69)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_class_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_NNMAP_t;

#define OPSIZE_NNMAP  (sizeof(klr_NNMAP_t)-sizeof(void*))

#define OPCODE_JMP ((knh_opcode_t)70)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	THREADEDCODE(void *jumpaddr;)
	knh_KLRInst_t* a1;  /* label */
	void *nextaddr;   /* sameas next codeaddr */
} klr_JMP_t;

#define OPSIZE_JMP  (sizeof(klr_JMP_t)-sizeof(void*))

#define OPCODE_SKIP ((knh_opcode_t)71)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	THREADEDCODE(void *jumpaddr;)
	knh_KLRInst_t* a1;  /* label */
	void *nextaddr;   /* sameas next codeaddr */
} klr_SKIP_t;

#define OPSIZE_SKIP  (sizeof(klr_SKIP_t)-sizeof(void*))

#define OPCODE_bJIFT ((knh_opcode_t)72)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	THREADEDCODE(void *jumpaddr;)
	knh_KLRInst_t* a1;  /* label */
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_bJIFT_t;

#define OPSIZE_bJIFT  (sizeof(klr_bJIFT_t)-sizeof(void*))

#define OPCODE_bJIFF ((knh_opcode_t)73)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	THREADEDCODE(void *jumpaddr;)
	knh_KLRInst_t* a1;  /* label */
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_bJIFF_t;

#define OPSIZE_bJIFF  (sizeof(klr_bJIFF_t)-sizeof(void*))

#define OPCODE_bJIFF_LOOP ((knh_opcode_t)74)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	THREADEDCODE(void *jumpaddr;)
	knh_KLRInst_t* a1;  /* label */
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_bJIFF_LOOP_t;

#define OPSIZE_bJIFF_LOOP  (sizeof(klr_bJIFF_LOOP_t)-sizeof(void*))

#define OPCODE_JIFNUL ((knh_opcode_t)75)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	THREADEDCODE(void *jumpaddr;)
	knh_KLRInst_t* a1;  /* label */
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_JIFNUL_t;

#define OPSIZE_JIFNUL  (sizeof(klr_JIFNUL_t)-sizeof(void*))

#define OPCODE_JIFNN ((knh_opcode_t)76)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	THREADEDCODE(void *jumpaddr;)
	knh_KLRInst_t* a1;  /* label */
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_JIFNN_t;

#define OPSIZE_JIFNN  (sizeof(klr_JIFNN_t)-sizeof(void*))

#define OPCODE_NEXT ((knh_opcode_t)77)
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

#define OPCODE_INEXT ((knh_opcode_t)78)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	THREADEDCODE(void *jumpaddr;)
	knh_KLRInst_t* a1;  /* label */
	knh_class_t a2;
	knh_sfpidx_t a3;
	knh_sfpidx_t a4;
	void *nextaddr;   /* sameas next codeaddr */
} klr_INEXT_t;

#define OPSIZE_INEXT  (sizeof(klr_INEXT_t)-sizeof(void*))

#define OPCODE_TRY ((knh_opcode_t)79)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	THREADEDCODE(void *jumpaddr;)
	knh_KLRInst_t* a1;  /* label */
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_TRY_t;

#define OPSIZE_TRY  (sizeof(klr_TRY_t)-sizeof(void*))

#define OPCODE_TRYEND ((knh_opcode_t)80)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	void *nextaddr;   /* sameas next codeaddr */
} klr_TRYEND_t;

#define OPSIZE_TRYEND  (sizeof(klr_TRYEND_t)-sizeof(void*))

#define OPCODE_CATCH ((knh_opcode_t)81)
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

#define OPCODE_PUSH ((knh_opcode_t)82)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	void *nextaddr;   /* sameas next codeaddr */
} klr_PUSH_t;

#define OPSIZE_PUSH  (sizeof(klr_PUSH_t)-sizeof(void*))

#define OPCODE_POP ((knh_opcode_t)83)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	void *nextaddr;   /* sameas next codeaddr */
} klr_POP_t;

#define OPSIZE_POP  (sizeof(klr_POP_t)-sizeof(void*))

#define OPCODE_THROW ((knh_opcode_t)84)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_ushort_t a1;
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_THROW_t;

#define OPSIZE_THROW  (sizeof(klr_THROW_t)-sizeof(void*))

#define OPCODE_THROWs ((knh_opcode_t)85)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_ushort_t a1;
	knh_String_t* a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_THROWs_t;

#define OPSIZE_THROWs  (sizeof(klr_THROWs_t)-sizeof(void*))

#define OPCODE_THROW_AGAIN ((knh_opcode_t)86)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	void *nextaddr;   /* sameas next codeaddr */
} klr_THROW_AGAIN_t;

#define OPSIZE_THROW_AGAIN  (sizeof(klr_THROW_AGAIN_t)-sizeof(void*))

#define OPCODE_P ((knh_opcode_t)87)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_flag_t a1;
	knh_methodn_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_P_t;

#define OPSIZE_P  (sizeof(klr_P_t)-sizeof(void*))

#define OPCODE_PMSG ((knh_opcode_t)88)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_flag_t a1;
	knh_Object_t* a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_PMSG_t;

#define OPSIZE_PMSG  (sizeof(klr_PMSG_t)-sizeof(void*))

#define OPCODE_iCAST ((knh_opcode_t)89)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iCAST_t;

#define OPSIZE_iCAST  (sizeof(klr_iCAST_t)-sizeof(void*))

#define OPCODE_inCAST ((knh_opcode_t)90)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	void *nextaddr;   /* sameas next codeaddr */
} klr_inCAST_t;

#define OPSIZE_inCAST  (sizeof(klr_inCAST_t)-sizeof(void*))

#define OPCODE_fCAST ((knh_opcode_t)91)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fCAST_t;

#define OPSIZE_fCAST  (sizeof(klr_fCAST_t)-sizeof(void*))

#define OPCODE_fnCAST ((knh_opcode_t)92)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fnCAST_t;

#define OPSIZE_fnCAST  (sizeof(klr_fnCAST_t)-sizeof(void*))

#define OPCODE_bNOT ((knh_opcode_t)93)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_bNOT_t;

#define OPSIZE_bNOT  (sizeof(klr_bNOT_t)-sizeof(void*))

#define OPCODE_iNEG ((knh_opcode_t)94)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iNEG_t;

#define OPSIZE_iNEG  (sizeof(klr_iNEG_t)-sizeof(void*))

#define OPCODE_iADD ((knh_opcode_t)95)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iADD_t;

#define OPSIZE_iADD  (sizeof(klr_iADD_t)-sizeof(void*))

#define OPCODE_iADDn ((knh_opcode_t)96)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_int_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iADDn_t;

#define OPSIZE_iADDn  (sizeof(klr_iADDn_t)-sizeof(void*))

#define OPCODE_iSUB ((knh_opcode_t)97)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iSUB_t;

#define OPSIZE_iSUB  (sizeof(klr_iSUB_t)-sizeof(void*))

#define OPCODE_iSUBn ((knh_opcode_t)98)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_int_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iSUBn_t;

#define OPSIZE_iSUBn  (sizeof(klr_iSUBn_t)-sizeof(void*))

#define OPCODE_iMUL ((knh_opcode_t)99)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iMUL_t;

#define OPSIZE_iMUL  (sizeof(klr_iMUL_t)-sizeof(void*))

#define OPCODE_iMULn ((knh_opcode_t)100)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_int_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iMULn_t;

#define OPSIZE_iMULn  (sizeof(klr_iMULn_t)-sizeof(void*))

#define OPCODE_iDIV ((knh_opcode_t)101)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iDIV_t;

#define OPSIZE_iDIV  (sizeof(klr_iDIV_t)-sizeof(void*))

#define OPCODE_iDIVn ((knh_opcode_t)102)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_int_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iDIVn_t;

#define OPSIZE_iDIVn  (sizeof(klr_iDIVn_t)-sizeof(void*))

#define OPCODE_iMOD ((knh_opcode_t)103)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iMOD_t;

#define OPSIZE_iMOD  (sizeof(klr_iMOD_t)-sizeof(void*))

#define OPCODE_iMODn ((knh_opcode_t)104)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_int_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iMODn_t;

#define OPSIZE_iMODn  (sizeof(klr_iMODn_t)-sizeof(void*))

#define OPCODE_iEQ ((knh_opcode_t)105)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iEQ_t;

#define OPSIZE_iEQ  (sizeof(klr_iEQ_t)-sizeof(void*))

#define OPCODE_iEQn ((knh_opcode_t)106)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_int_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iEQn_t;

#define OPSIZE_iEQn  (sizeof(klr_iEQn_t)-sizeof(void*))

#define OPCODE_iNEQ ((knh_opcode_t)107)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iNEQ_t;

#define OPSIZE_iNEQ  (sizeof(klr_iNEQ_t)-sizeof(void*))

#define OPCODE_iNEQn ((knh_opcode_t)108)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_int_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iNEQn_t;

#define OPSIZE_iNEQn  (sizeof(klr_iNEQn_t)-sizeof(void*))

#define OPCODE_iLT ((knh_opcode_t)109)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iLT_t;

#define OPSIZE_iLT  (sizeof(klr_iLT_t)-sizeof(void*))

#define OPCODE_iLTn ((knh_opcode_t)110)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_int_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iLTn_t;

#define OPSIZE_iLTn  (sizeof(klr_iLTn_t)-sizeof(void*))

#define OPCODE_iLTE ((knh_opcode_t)111)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iLTE_t;

#define OPSIZE_iLTE  (sizeof(klr_iLTE_t)-sizeof(void*))

#define OPCODE_iLTEn ((knh_opcode_t)112)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_int_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iLTEn_t;

#define OPSIZE_iLTEn  (sizeof(klr_iLTEn_t)-sizeof(void*))

#define OPCODE_iGT ((knh_opcode_t)113)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iGT_t;

#define OPSIZE_iGT  (sizeof(klr_iGT_t)-sizeof(void*))

#define OPCODE_iGTn ((knh_opcode_t)114)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_int_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iGTn_t;

#define OPSIZE_iGTn  (sizeof(klr_iGTn_t)-sizeof(void*))

#define OPCODE_iGTE ((knh_opcode_t)115)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iGTE_t;

#define OPSIZE_iGTE  (sizeof(klr_iGTE_t)-sizeof(void*))

#define OPCODE_iGTEn ((knh_opcode_t)116)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_int_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iGTEn_t;

#define OPSIZE_iGTEn  (sizeof(klr_iGTEn_t)-sizeof(void*))

#define OPCODE_fNEG ((knh_opcode_t)117)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fNEG_t;

#define OPSIZE_fNEG  (sizeof(klr_fNEG_t)-sizeof(void*))

#define OPCODE_fADD ((knh_opcode_t)118)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fADD_t;

#define OPSIZE_fADD  (sizeof(klr_fADD_t)-sizeof(void*))

#define OPCODE_fADDn ((knh_opcode_t)119)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_float_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fADDn_t;

#define OPSIZE_fADDn  (sizeof(klr_fADDn_t)-sizeof(void*))

#define OPCODE_fSUB ((knh_opcode_t)120)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fSUB_t;

#define OPSIZE_fSUB  (sizeof(klr_fSUB_t)-sizeof(void*))

#define OPCODE_fSUBn ((knh_opcode_t)121)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_float_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fSUBn_t;

#define OPSIZE_fSUBn  (sizeof(klr_fSUBn_t)-sizeof(void*))

#define OPCODE_fMUL ((knh_opcode_t)122)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fMUL_t;

#define OPSIZE_fMUL  (sizeof(klr_fMUL_t)-sizeof(void*))

#define OPCODE_fMULn ((knh_opcode_t)123)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_float_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fMULn_t;

#define OPSIZE_fMULn  (sizeof(klr_fMULn_t)-sizeof(void*))

#define OPCODE_fDIV ((knh_opcode_t)124)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fDIV_t;

#define OPSIZE_fDIV  (sizeof(klr_fDIV_t)-sizeof(void*))

#define OPCODE_fDIVn ((knh_opcode_t)125)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_float_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fDIVn_t;

#define OPSIZE_fDIVn  (sizeof(klr_fDIVn_t)-sizeof(void*))

#define OPCODE_fEQ ((knh_opcode_t)126)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fEQ_t;

#define OPSIZE_fEQ  (sizeof(klr_fEQ_t)-sizeof(void*))

#define OPCODE_fEQn ((knh_opcode_t)127)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_float_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fEQn_t;

#define OPSIZE_fEQn  (sizeof(klr_fEQn_t)-sizeof(void*))

#define OPCODE_fNEQ ((knh_opcode_t)128)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fNEQ_t;

#define OPSIZE_fNEQ  (sizeof(klr_fNEQ_t)-sizeof(void*))

#define OPCODE_fNEQn ((knh_opcode_t)129)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_float_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fNEQn_t;

#define OPSIZE_fNEQn  (sizeof(klr_fNEQn_t)-sizeof(void*))

#define OPCODE_fLT ((knh_opcode_t)130)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fLT_t;

#define OPSIZE_fLT  (sizeof(klr_fLT_t)-sizeof(void*))

#define OPCODE_fLTn ((knh_opcode_t)131)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_float_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fLTn_t;

#define OPSIZE_fLTn  (sizeof(klr_fLTn_t)-sizeof(void*))

#define OPCODE_fLTE ((knh_opcode_t)132)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fLTE_t;

#define OPSIZE_fLTE  (sizeof(klr_fLTE_t)-sizeof(void*))

#define OPCODE_fLTEn ((knh_opcode_t)133)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_float_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fLTEn_t;

#define OPSIZE_fLTEn  (sizeof(klr_fLTEn_t)-sizeof(void*))

#define OPCODE_fGT ((knh_opcode_t)134)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fGT_t;

#define OPSIZE_fGT  (sizeof(klr_fGT_t)-sizeof(void*))

#define OPCODE_fGTn ((knh_opcode_t)135)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_float_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fGTn_t;

#define OPSIZE_fGTn  (sizeof(klr_fGTn_t)-sizeof(void*))

#define OPCODE_fGTE ((knh_opcode_t)136)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fGTE_t;

#define OPSIZE_fGTE  (sizeof(klr_fGTE_t)-sizeof(void*))

#define OPCODE_fGTEn ((knh_opcode_t)137)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_float_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fGTEn_t;

#define OPSIZE_fGTEn  (sizeof(klr_fGTEn_t)-sizeof(void*))

#define OPCODE_ARYGET ((knh_opcode_t)138)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_ARYGET_t;

#define OPSIZE_ARYGET  (sizeof(klr_ARYGET_t)-sizeof(void*))

#define OPCODE_ARYGETn ((knh_opcode_t)139)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_intptr_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_ARYGETn_t;

#define OPSIZE_ARYGETn  (sizeof(klr_ARYGETn_t)-sizeof(void*))

#define OPCODE_iARYGET ((knh_opcode_t)140)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iARYGET_t;

#define OPSIZE_iARYGET  (sizeof(klr_iARYGET_t)-sizeof(void*))

#define OPCODE_iARYGETn ((knh_opcode_t)141)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_intptr_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iARYGETn_t;

#define OPSIZE_iARYGETn  (sizeof(klr_iARYGETn_t)-sizeof(void*))

#define OPCODE_fARYGET ((knh_opcode_t)142)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fARYGET_t;

#define OPSIZE_fARYGET  (sizeof(klr_fARYGET_t)-sizeof(void*))

#define OPCODE_fARYGETn ((knh_opcode_t)143)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_intptr_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fARYGETn_t;

#define OPSIZE_fARYGETn  (sizeof(klr_fARYGETn_t)-sizeof(void*))

#define OPCODE_ARYSET ((knh_opcode_t)144)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_ARYSET_t;

#define OPSIZE_ARYSET  (sizeof(klr_ARYSET_t)-sizeof(void*))

#define OPCODE_ARYSETn ((knh_opcode_t)145)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_intptr_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_ARYSETn_t;

#define OPSIZE_ARYSETn  (sizeof(klr_ARYSETn_t)-sizeof(void*))

#define OPCODE_iARYSET ((knh_opcode_t)146)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iARYSET_t;

#define OPSIZE_iARYSET  (sizeof(klr_iARYSET_t)-sizeof(void*))

#define OPCODE_iARYSETn ((knh_opcode_t)147)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_intptr_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_iARYSETn_t;

#define OPSIZE_iARYSETn  (sizeof(klr_iARYSETn_t)-sizeof(void*))

#define OPCODE_fARYSET ((knh_opcode_t)148)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_sfpidx_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fARYSET_t;

#define OPSIZE_fARYSET  (sizeof(klr_fARYSET_t)-sizeof(void*))

#define OPCODE_fARYSETn ((knh_opcode_t)149)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	knh_sfpidx_t a2;
	knh_intptr_t a3;
	void *nextaddr;   /* sameas next codeaddr */
} klr_fARYSETn_t;

#define OPSIZE_fARYSETn  (sizeof(klr_fARYSETn_t)-sizeof(void*))

#define OPCODE_THCODE ((knh_opcode_t)150)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	knh_sfpidx_t a1;
	void *nextaddr;   /* sameas next codeaddr */
} klr_THCODE_t;

#define OPSIZE_THCODE  (sizeof(klr_THCODE_t)-sizeof(void*))

#define OPCODE_NOP ((knh_opcode_t)151)
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;
	void *nextaddr;   /* sameas next codeaddr */
} klr_NOP_t;

#define OPSIZE_NOP  (sizeof(klr_NOP_t)-sizeof(void*))
#endif /* KONOHA_CODE_H_ */
