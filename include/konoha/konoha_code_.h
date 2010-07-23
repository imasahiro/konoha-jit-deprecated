#ifndef KONOHA_CODE_H_
#define KONOHA_CODE_H_
// THIS FILE WAS AUTOMATICALLY GENERATED

#define OPCODE_HALT ((knh_opcode_t)0)
typedef struct klr_HALT_t {
	KCODE_HEAD;
} klr_HALT_t;

#define OPCODE_ENTER ((knh_opcode_t)1)
typedef struct klr_ENTER_t {
	KCODE_HEAD;
} klr_ENTER_t;

#define OPCODE_FUNCCALL ((knh_opcode_t)2)
typedef struct klr_FUNCCALL_t {
	KCODE_HEAD;
} klr_FUNCCALL_t;

#define OPCODE_THCODE ((knh_opcode_t)3)
typedef struct klr_THCODE_t {
	KCODE_HEAD;
} klr_THCODE_t;

#define OPCODE_LABEL ((knh_opcode_t)4)
typedef struct klr_LABEL_t {
	KCODE_HEAD;
	knh_uintptr_t id;
	knh_String_t* msg;
} klr_LABEL_t;

#define OPCODE_PROBE ((knh_opcode_t)5)
typedef struct klr_PROBE_t {
	KCODE_HEAD;
	klr_Fprobe probe;
	knh_uintptr_t n;
} klr_PROBE_t;

#define OPCODE_VEXEC ((knh_opcode_t)6)
typedef struct klr_VEXEC_t {
	KCODE_HEAD;
} klr_VEXEC_t;

#define OPCODE_YEILD ((knh_opcode_t)7)
typedef struct klr_YEILD_t {
	KCODE_HEAD;
	knh_sfpidx_t n;
} klr_YEILD_t;

#define OPCODE_EXIT ((knh_opcode_t)8)
typedef struct klr_EXIT_t {
	KCODE_HEAD;
} klr_EXIT_t;

#define OPCODE_P ((knh_opcode_t)9)
typedef struct klr_P_t {
	KCODE_HEAD;
	klr_Fprint print;
	knh_uintptr_t flag;
	knh_String_t* msg;
	knh_Method_t* fmt;
	knh_sfpidx_t n;
} klr_P_t;

#define OPCODE_OSET ((knh_opcode_t)10)
typedef struct klr_OSET_t {
	KCODE_HEAD;
	knh_sfpidx_t a;
	knh_Object_t* o;
} klr_OSET_t;

#define OPCODE_NSET ((knh_opcode_t)11)
typedef struct klr_NSET_t {
	KCODE_HEAD;
	knh_sfpidx_t a;
	knh_int_t n;
} klr_NSET_t;

#define OPCODE_OMOV ((knh_opcode_t)12)
typedef struct klr_OMOV_t {
	KCODE_HEAD;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_OMOV_t;

#define OPCODE_NMOV ((knh_opcode_t)13)
typedef struct klr_NMOV_t {
	KCODE_HEAD;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_NMOV_t;

#define OPCODE_SWAP ((knh_opcode_t)14)
typedef struct klr_SWAP_t {
	KCODE_HEAD;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_SWAP_t;

#define OPCODE_UNBOX ((knh_opcode_t)15)
typedef struct klr_UNBOX_t {
	KCODE_HEAD;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_UNBOX_t;

#define OPCODE_ONMOV ((knh_opcode_t)16)
typedef struct klr_ONMOV_t {
	KCODE_HEAD;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
	knh_sfpidx_t c;
	knh_sfpidx_t d;
} klr_ONMOV_t;

#define OPCODE_OOMOV ((knh_opcode_t)17)
typedef struct klr_OOMOV_t {
	KCODE_HEAD;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
	knh_sfpidx_t c;
	knh_sfpidx_t d;
} klr_OOMOV_t;

#define OPCODE_NNMOV ((knh_opcode_t)18)
typedef struct klr_NNMOV_t {
	KCODE_HEAD;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
	knh_sfpidx_t c;
	knh_sfpidx_t d;
} klr_NNMOV_t;

#define OPCODE_OMOVx ((knh_opcode_t)19)
typedef struct klr_OMOVx_t {
	KCODE_HEAD;
	knh_sfpidx_t a;
	knh_sfx_t b;
} klr_OMOVx_t;

#define OPCODE_iMOVx ((knh_opcode_t)20)
typedef struct klr_iMOVx_t {
	KCODE_HEAD;
	knh_sfpidx_t a;
	knh_sfx_t b;
} klr_iMOVx_t;

#define OPCODE_fMOVx ((knh_opcode_t)21)
typedef struct klr_fMOVx_t {
	KCODE_HEAD;
	knh_sfpidx_t a;
	knh_sfx_t b;
} klr_fMOVx_t;

#define OPCODE_bMOVx ((knh_opcode_t)22)
typedef struct klr_bMOVx_t {
	KCODE_HEAD;
	knh_sfpidx_t a;
	knh_sfx_t b;
} klr_bMOVx_t;

#define OPCODE_XMOV ((knh_opcode_t)23)
typedef struct klr_XMOV_t {
	KCODE_HEAD;
	knh_sfx_t a;
	knh_sfpidx_t b;
} klr_XMOV_t;

#define OPCODE_XMOVx ((knh_opcode_t)24)
typedef struct klr_XMOVx_t {
	KCODE_HEAD;
	knh_sfx_t a;
	knh_sfx_t b;
} klr_XMOVx_t;

#define OPCODE_XOSET ((knh_opcode_t)25)
typedef struct klr_XOSET_t {
	KCODE_HEAD;
	knh_sfx_t a;
	knh_Object_t* b;
} klr_XOSET_t;

#define OPCODE_XIMOV ((knh_opcode_t)26)
typedef struct klr_XIMOV_t {
	KCODE_HEAD;
	knh_sfx_t a;
	knh_sfpidx_t b;
} klr_XIMOV_t;

#define OPCODE_XFMOV ((knh_opcode_t)27)
typedef struct klr_XFMOV_t {
	KCODE_HEAD;
	knh_sfx_t a;
	knh_sfpidx_t b;
} klr_XFMOV_t;

#define OPCODE_XBMOV ((knh_opcode_t)28)
typedef struct klr_XBMOV_t {
	KCODE_HEAD;
	knh_sfx_t a;
	knh_sfpidx_t b;
} klr_XBMOV_t;

#define OPCODE_CHKSTACK ((knh_opcode_t)29)
typedef struct klr_CHKSTACK_t {
	KCODE_HEAD;
	knh_sfpidx_t n;
} klr_CHKSTACK_t;

#define OPCODE_LDMETHOD ((knh_opcode_t)30)
typedef struct klr_LDMETHOD_t {
	KCODE_HEAD;
	klr_Fmethod method;
	knh_sfpidx_t n;
	knh_Method_t* callmtd;
} klr_LDMETHOD_t;

#define OPCODE_CALL ((knh_opcode_t)31)
typedef struct klr_CALL_t {
	KCODE_HEAD;
	knh_sfpidx_t rtnidx;
	knh_sfpidx_t espshift;
} klr_CALL_t;

#define OPCODE_SCALL ((knh_opcode_t)32)
typedef struct klr_SCALL_t {
	KCODE_HEAD;
	knh_sfpidx_t rtnidx;
	knh_sfpidx_t espshift;
	knh_Method_t* callmtd;
} klr_SCALL_t;

#define OPCODE_VCALL ((knh_opcode_t)33)
typedef struct klr_VCALL_t {
	KCODE_HEAD;
	knh_sfpidx_t rtnidx;
	knh_sfpidx_t espshift;
	knh_Method_t* callmtd;
} klr_VCALL_t;

#define OPCODE_VCALL_ ((knh_opcode_t)34)
typedef struct klr_VCALL__t {
	KCODE_HEAD;
	knh_sfpidx_t rtnidx;
	knh_sfpidx_t espshift;
	knh_Method_t* callmtd;
} klr_VCALL__t;

#define OPCODE_RET ((knh_opcode_t)35)
typedef struct klr_RET_t {
	KCODE_HEAD;
} klr_RET_t;

#define OPCODE_SCAST ((knh_opcode_t)36)
typedef struct klr_SCAST_t {
	KCODE_HEAD;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
	knh_Translator_t* cast;
} klr_SCAST_t;

#define OPCODE_TCAST ((knh_opcode_t)37)
typedef struct klr_TCAST_t {
	KCODE_HEAD;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
	knh_Translator_t* cast;
} klr_TCAST_t;

#define OPCODE_ACAST ((knh_opcode_t)38)
typedef struct klr_ACAST_t {
	KCODE_HEAD;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
	knh_Translator_t* cast;
} klr_ACAST_t;

#define OPCODE_iCAST ((knh_opcode_t)39)
typedef struct klr_iCAST_t {
	KCODE_HEAD;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_iCAST_t;

#define OPCODE_fCAST ((knh_opcode_t)40)
typedef struct klr_fCAST_t {
	KCODE_HEAD;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_fCAST_t;

#define OPCODE_TR ((knh_opcode_t)41)
typedef struct klr_TR_t {
	KCODE_HEAD;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
	knh_intptr_t cid;
	klr_Ftr tr;
} klr_TR_t;

#define OPCODE_NUL ((knh_opcode_t)42)
typedef struct klr_NUL_t {
	KCODE_HEAD;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_NUL_t;

#define OPCODE_iNUL ((knh_opcode_t)43)
typedef struct klr_iNUL_t {
	KCODE_HEAD;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_iNUL_t;

#define OPCODE_fNUL ((knh_opcode_t)44)
typedef struct klr_fNUL_t {
	KCODE_HEAD;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_fNUL_t;

#define OPCODE_JMP ((knh_opcode_t)45)
typedef struct klr_JMP_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
} klr_JMP_t;

#define OPCODE_JMP_ ((knh_opcode_t)46)
typedef struct klr_JMP__t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
} klr_JMP__t;

#define OPCODE_ONCE ((knh_opcode_t)47)
typedef struct klr_ONCE_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
} klr_ONCE_t;

#define OPCODE_JMPF ((knh_opcode_t)48)
typedef struct klr_JMPF_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_sfpidx_t a;
} klr_JMPF_t;

#define OPCODE_DYJMP ((knh_opcode_t)49)
typedef struct klr_DYJMP_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_sfpidx_t a;
	klr_Fchk chk;
} klr_DYJMP_t;

#define OPCODE_NEXT ((knh_opcode_t)50)
typedef struct klr_NEXT_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_NEXT_t;

#define OPCODE_TRY ((knh_opcode_t)51)
typedef struct klr_TRY_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_sfpidx_t hn;
} klr_TRY_t;

#define OPCODE_THROW ((knh_opcode_t)52)
typedef struct klr_THROW_t {
	KCODE_HEAD;
	knh_sfpidx_t start;
} klr_THROW_t;

#define OPCODE_CATCH ((knh_opcode_t)53)
typedef struct klr_CATCH_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_sfpidx_t en;
	knh_String_t* msg;
} klr_CATCH_t;

#define OPCODE_bNOT ((knh_opcode_t)54)
typedef struct klr_bNOT_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
} klr_bNOT_t;

#define OPCODE_iINC ((knh_opcode_t)55)
typedef struct klr_iINC_t {
	KCODE_HEAD;
	knh_sfpidx_t a;
} klr_iINC_t;

#define OPCODE_iDEC ((knh_opcode_t)56)
typedef struct klr_iDEC_t {
	KCODE_HEAD;
	knh_sfpidx_t a;
} klr_iDEC_t;

#define OPCODE_iNEG ((knh_opcode_t)57)
typedef struct klr_iNEG_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
} klr_iNEG_t;

#define OPCODE_iADD ((knh_opcode_t)58)
typedef struct klr_iADD_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_iADD_t;

#define OPCODE_iSUB ((knh_opcode_t)59)
typedef struct klr_iSUB_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_iSUB_t;

#define OPCODE_iMUL ((knh_opcode_t)60)
typedef struct klr_iMUL_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_iMUL_t;

#define OPCODE_iDIV ((knh_opcode_t)61)
typedef struct klr_iDIV_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_iDIV_t;

#define OPCODE_iMOD ((knh_opcode_t)62)
typedef struct klr_iMOD_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_iMOD_t;

#define OPCODE_iEQ ((knh_opcode_t)63)
typedef struct klr_iEQ_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_iEQ_t;

#define OPCODE_iNEQ ((knh_opcode_t)64)
typedef struct klr_iNEQ_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_iNEQ_t;

#define OPCODE_iLT ((knh_opcode_t)65)
typedef struct klr_iLT_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_iLT_t;

#define OPCODE_iLTE ((knh_opcode_t)66)
typedef struct klr_iLTE_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_iLTE_t;

#define OPCODE_iGT ((knh_opcode_t)67)
typedef struct klr_iGT_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_iGT_t;

#define OPCODE_iGTE ((knh_opcode_t)68)
typedef struct klr_iGTE_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_iGTE_t;

#define OPCODE_iADDn ((knh_opcode_t)69)
typedef struct klr_iADDn_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_int_t n;
} klr_iADDn_t;

#define OPCODE_iSUBn ((knh_opcode_t)70)
typedef struct klr_iSUBn_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_int_t n;
} klr_iSUBn_t;

#define OPCODE_iMULn ((knh_opcode_t)71)
typedef struct klr_iMULn_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_int_t n;
} klr_iMULn_t;

#define OPCODE_iDIVn ((knh_opcode_t)72)
typedef struct klr_iDIVn_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_int_t n;
} klr_iDIVn_t;

#define OPCODE_iMODn ((knh_opcode_t)73)
typedef struct klr_iMODn_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_int_t n;
} klr_iMODn_t;

#define OPCODE_iEQn ((knh_opcode_t)74)
typedef struct klr_iEQn_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_int_t n;
} klr_iEQn_t;

#define OPCODE_iNEQn ((knh_opcode_t)75)
typedef struct klr_iNEQn_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_int_t n;
} klr_iNEQn_t;

#define OPCODE_iLTn ((knh_opcode_t)76)
typedef struct klr_iLTn_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_int_t n;
} klr_iLTn_t;

#define OPCODE_iLTEn ((knh_opcode_t)77)
typedef struct klr_iLTEn_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_int_t n;
} klr_iLTEn_t;

#define OPCODE_iGTn ((knh_opcode_t)78)
typedef struct klr_iGTn_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_int_t n;
} klr_iGTn_t;

#define OPCODE_iGTEn ((knh_opcode_t)79)
typedef struct klr_iGTEn_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_int_t n;
} klr_iGTEn_t;

#define OPCODE_fNEG ((knh_opcode_t)80)
typedef struct klr_fNEG_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
} klr_fNEG_t;

#define OPCODE_fADD ((knh_opcode_t)81)
typedef struct klr_fADD_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_fADD_t;

#define OPCODE_fSUB ((knh_opcode_t)82)
typedef struct klr_fSUB_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_fSUB_t;

#define OPCODE_fMUL ((knh_opcode_t)83)
typedef struct klr_fMUL_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_fMUL_t;

#define OPCODE_fDIV ((knh_opcode_t)84)
typedef struct klr_fDIV_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_fDIV_t;

#define OPCODE_fEQ ((knh_opcode_t)85)
typedef struct klr_fEQ_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_fEQ_t;

#define OPCODE_fNEQ ((knh_opcode_t)86)
typedef struct klr_fNEQ_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_fNEQ_t;

#define OPCODE_fLT ((knh_opcode_t)87)
typedef struct klr_fLT_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_fLT_t;

#define OPCODE_fLTE ((knh_opcode_t)88)
typedef struct klr_fLTE_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_fLTE_t;

#define OPCODE_fGT ((knh_opcode_t)89)
typedef struct klr_fGT_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_fGT_t;

#define OPCODE_fGTE ((knh_opcode_t)90)
typedef struct klr_fGTE_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_fGTE_t;

#define OPCODE_fADDn ((knh_opcode_t)91)
typedef struct klr_fADDn_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_float_t n;
} klr_fADDn_t;

#define OPCODE_fSUBn ((knh_opcode_t)92)
typedef struct klr_fSUBn_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_float_t n;
} klr_fSUBn_t;

#define OPCODE_fMULn ((knh_opcode_t)93)
typedef struct klr_fMULn_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_float_t n;
} klr_fMULn_t;

#define OPCODE_fDIVn ((knh_opcode_t)94)
typedef struct klr_fDIVn_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_float_t n;
} klr_fDIVn_t;

#define OPCODE_fEQn ((knh_opcode_t)95)
typedef struct klr_fEQn_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_float_t n;
} klr_fEQn_t;

#define OPCODE_fNEQn ((knh_opcode_t)96)
typedef struct klr_fNEQn_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_float_t n;
} klr_fNEQn_t;

#define OPCODE_fLTn ((knh_opcode_t)97)
typedef struct klr_fLTn_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_float_t n;
} klr_fLTn_t;

#define OPCODE_fLTEn ((knh_opcode_t)98)
typedef struct klr_fLTEn_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_float_t n;
} klr_fLTEn_t;

#define OPCODE_fGTn ((knh_opcode_t)99)
typedef struct klr_fGTn_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_float_t n;
} klr_fGTn_t;

#define OPCODE_fGTEn ((knh_opcode_t)100)
typedef struct klr_fGTEn_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_float_t n;
} klr_fGTEn_t;

#define OPCODE_OGETIDX ((knh_opcode_t)101)
typedef struct klr_OGETIDX_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_sfpidx_t n;
} klr_OGETIDX_t;

#define OPCODE_OSETIDX ((knh_opcode_t)102)
typedef struct klr_OSETIDX_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_sfpidx_t n;
	knh_sfpidx_t v;
} klr_OSETIDX_t;

#define OPCODE_OGETIDXn ((knh_opcode_t)103)
typedef struct klr_OGETIDXn_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_uintptr_t n;
} klr_OGETIDXn_t;

#define OPCODE_OSETIDXn ((knh_opcode_t)104)
typedef struct klr_OSETIDXn_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_uintptr_t n;
	knh_sfpidx_t v;
} klr_OSETIDXn_t;

#define OPCODE_NGETIDX ((knh_opcode_t)105)
typedef struct klr_NGETIDX_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_sfpidx_t n;
} klr_NGETIDX_t;

#define OPCODE_NSETIDX ((knh_opcode_t)106)
typedef struct klr_NSETIDX_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_sfpidx_t v;
	knh_sfpidx_t n;
} klr_NSETIDX_t;

#define OPCODE_NGETIDXn ((knh_opcode_t)107)
typedef struct klr_NGETIDXn_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_uintptr_t n;
} klr_NGETIDXn_t;

#define OPCODE_NSETIDXn ((knh_opcode_t)108)
typedef struct klr_NSETIDXn_t {
	KCODE_HEAD;
	knh_sfpidx_t c;
	knh_sfpidx_t a;
	knh_sfpidx_t v;
	knh_uintptr_t n;
} klr_NSETIDXn_t;

#define OPCODE_bJNOT ((knh_opcode_t)109)
typedef struct klr_bJNOT_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_sfpidx_t a;
} klr_bJNOT_t;

#define OPCODE_iJEQ ((knh_opcode_t)110)
typedef struct klr_iJEQ_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_iJEQ_t;

#define OPCODE_iJNEQ ((knh_opcode_t)111)
typedef struct klr_iJNEQ_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_iJNEQ_t;

#define OPCODE_iJLT ((knh_opcode_t)112)
typedef struct klr_iJLT_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_iJLT_t;

#define OPCODE_iJLTE ((knh_opcode_t)113)
typedef struct klr_iJLTE_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_iJLTE_t;

#define OPCODE_iJGT ((knh_opcode_t)114)
typedef struct klr_iJGT_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_iJGT_t;

#define OPCODE_iJGTE ((knh_opcode_t)115)
typedef struct klr_iJGTE_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_iJGTE_t;

#define OPCODE_iJEQn ((knh_opcode_t)116)
typedef struct klr_iJEQn_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_sfpidx_t a;
	knh_int_t n;
} klr_iJEQn_t;

#define OPCODE_iJNEQn ((knh_opcode_t)117)
typedef struct klr_iJNEQn_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_sfpidx_t a;
	knh_int_t n;
} klr_iJNEQn_t;

#define OPCODE_iJLTn ((knh_opcode_t)118)
typedef struct klr_iJLTn_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_sfpidx_t a;
	knh_int_t n;
} klr_iJLTn_t;

#define OPCODE_iJLTEn ((knh_opcode_t)119)
typedef struct klr_iJLTEn_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_sfpidx_t a;
	knh_int_t n;
} klr_iJLTEn_t;

#define OPCODE_iJGTn ((knh_opcode_t)120)
typedef struct klr_iJGTn_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_sfpidx_t a;
	knh_int_t n;
} klr_iJGTn_t;

#define OPCODE_iJGTEn ((knh_opcode_t)121)
typedef struct klr_iJGTEn_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_sfpidx_t a;
	knh_int_t n;
} klr_iJGTEn_t;

#define OPCODE_fJEQ ((knh_opcode_t)122)
typedef struct klr_fJEQ_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_fJEQ_t;

#define OPCODE_fJNEQ ((knh_opcode_t)123)
typedef struct klr_fJNEQ_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_fJNEQ_t;

#define OPCODE_fJLT ((knh_opcode_t)124)
typedef struct klr_fJLT_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_fJLT_t;

#define OPCODE_fJLTE ((knh_opcode_t)125)
typedef struct klr_fJLTE_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_fJLTE_t;

#define OPCODE_fJGT ((knh_opcode_t)126)
typedef struct klr_fJGT_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_fJGT_t;

#define OPCODE_fJGTE ((knh_opcode_t)127)
typedef struct klr_fJGTE_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_sfpidx_t a;
	knh_sfpidx_t b;
} klr_fJGTE_t;

#define OPCODE_fJEQn ((knh_opcode_t)128)
typedef struct klr_fJEQn_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_sfpidx_t a;
	knh_float_t n;
} klr_fJEQn_t;

#define OPCODE_fJNEQn ((knh_opcode_t)129)
typedef struct klr_fJNEQn_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_sfpidx_t a;
	knh_float_t n;
} klr_fJNEQn_t;

#define OPCODE_fJLTn ((knh_opcode_t)130)
typedef struct klr_fJLTn_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_sfpidx_t a;
	knh_float_t n;
} klr_fJLTn_t;

#define OPCODE_fJLTEn ((knh_opcode_t)131)
typedef struct klr_fJLTEn_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_sfpidx_t a;
	knh_float_t n;
} klr_fJLTEn_t;

#define OPCODE_fJGTn ((knh_opcode_t)132)
typedef struct klr_fJGTn_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_sfpidx_t a;
	knh_float_t n;
} klr_fJGTn_t;

#define OPCODE_fJGTEn ((knh_opcode_t)133)
typedef struct klr_fJGTEn_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_sfpidx_t a;
	knh_float_t n;
} klr_fJGTEn_t;

#define OPCODE_NOP ((knh_opcode_t)134)
typedef struct klr_NOP_t {
	KCODE_HEAD;
} klr_NOP_t;

#define OPCODE_MAX ((knh_opcode_t)135)

#define VMT_VOID     0
#define VMT_ADDR     1
#define VMT_SFPIDX   2
#define VMT_SFX      3
#define VMT_U        4
#define VMT_F        5
#define VMT_CID      6
#define VMT_MTD      7
#define VMT_TRL      8
#define VMT_OBJECT   9
#define VMT_STRING   10
#define VMT_INT      11
#define VMT_FLOAT    12

#endif /* KONOHA_CODE_H_ */
