#ifndef KONOH_CTT_H
#define KONOH_CTT_H

#undef KLR_HALT
#define KLR_HALT JIT_ASM_HALT
#undef KLR_MOVa
#define KLR_MOVa JIT_ASM_MOVa
#undef KLR_MOVn
#define KLR_MOVn JIT_ASM_MOVn
#undef KLR_MOVo
#define KLR_MOVo JIT_ASM_MOVo
#undef KLR_MOVi
#define KLR_MOVi JIT_ASM_MOVi
#undef KLR_MOVx
#define KLR_MOVx JIT_ASM_MOVx
#undef KLR_MOVDEF
#define KLR_MOVDEF JIT_ASM_MOVDEF
#undef KLR_MOVSYS
#define KLR_MOVSYS JIT_ASM_MOVSYS
#undef KLR_XMOVs
#define KLR_XMOVs JIT_ASM_XMOVs
#undef KLR_XMOVo
#define KLR_XMOVo JIT_ASM_XMOVo
#undef KLR_XMOVx
#define KLR_XMOVx JIT_ASM_XMOVx
#undef KLR_XMOVDEF
#define KLR_XMOVDEF JIT_ASM_XMOVDEF
#undef KLR_XMOVSYS
#define KLR_XMOVSYS JIT_ASM_XMOVSYS
#undef KLR_MOVxi
#define KLR_MOVxi JIT_ASM_MOVxi
#undef KLR_XMOVsi
#define KLR_XMOVsi JIT_ASM_XMOVsi
#undef KLR_XMOVoi
#define KLR_XMOVoi JIT_ASM_XMOVoi
#undef KLR_XMOVxi
#define KLR_XMOVxi JIT_ASM_XMOVxi
#undef KLR_XMOVxio
#define KLR_XMOVxio JIT_ASM_XMOVxio
#undef KLR_XMOVxBXi
#define KLR_XMOVxBXi JIT_ASM_XMOVxBXi
#undef KLR_MOVxf
#define KLR_MOVxf JIT_ASM_MOVxf
#undef KLR_XMOVsf
#define KLR_XMOVsf JIT_ASM_XMOVsf
#undef KLR_XMOVof
#define KLR_XMOVof JIT_ASM_XMOVof
#undef KLR_XMOVxf
#define KLR_XMOVxf JIT_ASM_XMOVxf
#undef KLR_XMOVxfo
#define KLR_XMOVxfo JIT_ASM_XMOVxfo
#undef KLR_XMOVxBXf
#define KLR_XMOVxBXf JIT_ASM_XMOVxBXf
#undef KLR_MOVxb
#define KLR_MOVxb JIT_ASM_MOVxb
#undef KLR_XMOVsb
#define KLR_XMOVsb JIT_ASM_XMOVsb
#undef KLR_XMOVob
#define KLR_XMOVob JIT_ASM_XMOVob
#undef KLR_XMOVxb
#define KLR_XMOVxb JIT_ASM_XMOVxb
#undef KLR_SWAP
#define KLR_SWAP JIT_ASM_SWAP
#undef KLR_PARAMDEF
#define KLR_PARAMDEF JIT_ASM_PARAMDEF
#undef KLR_PARAMo
#define KLR_PARAMo JIT_ASM_PARAMo
#undef KLR_PARAMPROP
#define KLR_PARAMPROP JIT_ASM_PARAMPROP
#undef KLR_PARAMS
#define KLR_PARAMS JIT_ASM_PARAMS
#undef KLR_CHKESP
#define KLR_CHKESP JIT_ASM_CHKESP
#undef KLR_RET
#define KLR_RET JIT_ASM_RET
#undef KLR_YEILDBREAK
#define KLR_YEILDBREAK JIT_ASM_YEILDBREAK
#undef KLR_BOX
#define KLR_BOX JIT_ASM_BOX
#undef KLR_BOXnc
#define KLR_BOXnc JIT_ASM_BOXnc
#undef KLR_NNBOX
#define KLR_NNBOX JIT_ASM_NNBOX
#undef KLR_NNBOXnc
#define KLR_NNBOXnc JIT_ASM_NNBOXnc
#undef KLR_UNBOX
#define KLR_UNBOX JIT_ASM_UNBOX
#undef KLR_CHKNUL
#define KLR_CHKNUL JIT_ASM_CHKNUL
#undef KLR_CHKNULx
#define KLR_CHKNULx JIT_ASM_CHKNULx
#undef KLR_CHKTYPE
#define KLR_CHKTYPE JIT_ASM_CHKTYPE
#undef KLR_FCALL
#define KLR_FCALL JIT_ASM_FCALL
#undef KLR_RCALL
#define KLR_RCALL JIT_ASM_RCALL
#undef KLR_SCALL
#define KLR_SCALL JIT_ASM_SCALL
#undef KLR_AINVOKE
#define KLR_AINVOKE JIT_ASM_AINVOKE
#undef KLR_CALL
#define KLR_CALL JIT_ASM_CALL
#undef KLR_ACALL
#define KLR_ACALL JIT_ASM_ACALL
#undef KLR_NEW
#define KLR_NEW JIT_ASM_NEW
#undef KLR_COPYSFP
#define KLR_COPYSFP JIT_ASM_COPYSFP
#undef KLR_STR
#define KLR_STR JIT_ASM_STR
#undef KLR_SSTR
#define KLR_SSTR JIT_ASM_SSTR
#undef KLR_SMAP
#define KLR_SMAP JIT_ASM_SMAP
#undef KLR_SMAPnc
#define KLR_SMAPnc JIT_ASM_SMAPnc
#undef KLR_MAP
#define KLR_MAP JIT_ASM_MAP
#undef KLR_MAPnc
#define KLR_MAPnc JIT_ASM_MAPnc
#undef KLR_AMAP
#define KLR_AMAP JIT_ASM_AMAP
#undef KLR_NNMAP
#define KLR_NNMAP JIT_ASM_NNMAP
#undef KLR_JMP
#define KLR_JMP JIT_ASM_JMP
#undef KLR_SKIP
#define KLR_SKIP JIT_ASM_SKIP
#undef KLR_bJIFT
#define KLR_bJIFT JIT_ASM_bJIFT
#undef KLR_bJIFF
#define KLR_bJIFF JIT_ASM_bJIFF
#undef KLR_bJIFF_LOOP
#define KLR_bJIFF_LOOP JIT_ASM_bJIFF_LOOP
#undef KLR_JIFNUL
#define KLR_JIFNUL JIT_ASM_JIFNUL
#undef KLR_JIFNN
#define KLR_JIFNN JIT_ASM_JIFNN
#undef KLR_NEXT
#define KLR_NEXT JIT_ASM_NEXT
#undef KLR_INEXT
#define KLR_INEXT JIT_ASM_INEXT
#undef KLR_TRY
#define KLR_TRY JIT_ASM_TRY
#undef KLR_TRYEND
#define KLR_TRYEND JIT_ASM_TRYEND
#undef KLR_CATCH
#define KLR_CATCH JIT_ASM_CATCH
#undef KLR_PUSH
#define KLR_PUSH JIT_ASM_PUSH
#undef KLR_POP
#define KLR_POP JIT_ASM_POP
#undef KLR_THROW
#define KLR_THROW JIT_ASM_THROW
#undef KLR_THROWs
#define KLR_THROWs JIT_ASM_THROWs
#undef KLR_THROW_AGAIN
#define KLR_THROW_AGAIN JIT_ASM_THROW_AGAIN
#undef KLR_P
#define KLR_P JIT_ASM_P
#undef KLR_PMSG
#define KLR_PMSG JIT_ASM_PMSG
#undef KLR_iCAST
#define KLR_iCAST JIT_ASM_iCAST
#undef KLR_inCAST
#define KLR_inCAST JIT_ASM_inCAST
#undef KLR_fCAST
#define KLR_fCAST JIT_ASM_fCAST
#undef KLR_fnCAST
#define KLR_fnCAST JIT_ASM_fnCAST
#undef KLR_bNOT
#define KLR_bNOT JIT_ASM_bNOT
#undef KLR_iNEG
#define KLR_iNEG JIT_ASM_iNEG
#undef KLR_iADD
#define KLR_iADD JIT_ASM_iADD
#undef KLR_iADDn
#define KLR_iADDn JIT_ASM_iADDn
#undef KLR_iSUB
#define KLR_iSUB JIT_ASM_iSUB
#undef KLR_iSUBn
#define KLR_iSUBn JIT_ASM_iSUBn
#undef KLR_iMUL
#define KLR_iMUL JIT_ASM_iMUL
#undef KLR_iMULn
#define KLR_iMULn JIT_ASM_iMULn
#undef KLR_iDIV
#define KLR_iDIV JIT_ASM_iDIV
#undef KLR_iDIVn
#define KLR_iDIVn JIT_ASM_iDIVn
#undef KLR_iMOD
#define KLR_iMOD JIT_ASM_iMOD
#undef KLR_iMODn
#define KLR_iMODn JIT_ASM_iMODn
#undef KLR_iEQ
#define KLR_iEQ JIT_ASM_iEQ
#undef KLR_iEQn
#define KLR_iEQn JIT_ASM_iEQn
#undef KLR_iNEQ
#define KLR_iNEQ JIT_ASM_iNEQ
#undef KLR_iNEQn
#define KLR_iNEQn JIT_ASM_iNEQn
#undef KLR_iLT
#define KLR_iLT JIT_ASM_iLT
#undef KLR_iLTn
#define KLR_iLTn JIT_ASM_iLTn
#undef KLR_iLTE
#define KLR_iLTE JIT_ASM_iLTE
#undef KLR_iLTEn
#define KLR_iLTEn JIT_ASM_iLTEn
#undef KLR_iGT
#define KLR_iGT JIT_ASM_iGT
#undef KLR_iGTn
#define KLR_iGTn JIT_ASM_iGTn
#undef KLR_iGTE
#define KLR_iGTE JIT_ASM_iGTE
#undef KLR_iGTEn
#define KLR_iGTEn JIT_ASM_iGTEn
#undef KLR_fNEG
#define KLR_fNEG JIT_ASM_fNEG
#undef KLR_fADD
#define KLR_fADD JIT_ASM_fADD
#undef KLR_fADDn
#define KLR_fADDn JIT_ASM_fADDn
#undef KLR_fSUB
#define KLR_fSUB JIT_ASM_fSUB
#undef KLR_fSUBn
#define KLR_fSUBn JIT_ASM_fSUBn
#undef KLR_fMUL
#define KLR_fMUL JIT_ASM_fMUL
#undef KLR_fMULn
#define KLR_fMULn JIT_ASM_fMULn
#undef KLR_fDIV
#define KLR_fDIV JIT_ASM_fDIV
#undef KLR_fDIVn
#define KLR_fDIVn JIT_ASM_fDIVn
#undef KLR_fEQ
#define KLR_fEQ JIT_ASM_fEQ
#undef KLR_fEQn
#define KLR_fEQn JIT_ASM_fEQn
#undef KLR_fNEQ
#define KLR_fNEQ JIT_ASM_fNEQ
#undef KLR_fNEQn
#define KLR_fNEQn JIT_ASM_fNEQn
#undef KLR_fLT
#define KLR_fLT JIT_ASM_fLT
#undef KLR_fLTn
#define KLR_fLTn JIT_ASM_fLTn
#undef KLR_fLTE
#define KLR_fLTE JIT_ASM_fLTE
#undef KLR_fLTEn
#define KLR_fLTEn JIT_ASM_fLTEn
#undef KLR_fGT
#define KLR_fGT JIT_ASM_fGT
#undef KLR_fGTn
#define KLR_fGTn JIT_ASM_fGTn
#undef KLR_fGTE
#define KLR_fGTE JIT_ASM_fGTE
#undef KLR_fGTEn
#define KLR_fGTEn JIT_ASM_fGTEn
#undef KLR_ARYGET
#define KLR_ARYGET JIT_ASM_ARYGET
#undef KLR_ARYGETn
#define KLR_ARYGETn JIT_ASM_ARYGETn
#undef KLR_iARYGET
#define KLR_iARYGET JIT_ASM_iARYGET
#undef KLR_iARYGETn
#define KLR_iARYGETn JIT_ASM_iARYGETn
#undef KLR_fARYGET
#define KLR_fARYGET JIT_ASM_fARYGET
#undef KLR_fARYGETn
#define KLR_fARYGETn JIT_ASM_fARYGETn
#undef KLR_ARYSET
#define KLR_ARYSET JIT_ASM_ARYSET
#undef KLR_ARYSETn
#define KLR_ARYSETn JIT_ASM_ARYSETn
#undef KLR_iARYSET
#define KLR_iARYSET JIT_ASM_iARYSET
#undef KLR_iARYSETn
#define KLR_iARYSETn JIT_ASM_iARYSETn
#undef KLR_fARYSET
#define KLR_fARYSET JIT_ASM_fARYSET
#undef KLR_fARYSETn
#define KLR_fARYSETn JIT_ASM_fARYSETn
#undef KLR_THCODE
#define KLR_THCODE JIT_ASM_THCODE
#undef KLR_NOP
#define KLR_NOP JIT_ASM_NOP

#endif
