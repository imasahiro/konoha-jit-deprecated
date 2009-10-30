#ifndef IJT_VM_H
#define IJT_VM_H
#include <konoha.h>
#include <konoha/konoha_api.h>
//#include <konoha/konoha_code_.h>

void JIT_OP_HALT_(Ctx* ctx,knh_sfp_t* sfp)
{
    KLR_HALT(ctx);
}
void JIT_OP_MOVa_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2)
{
    KLR_MOVa(ctx, a1, a2);
}
void JIT_OP_MOVn_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2)
{
    KLR_MOVn(ctx, a1, a2);
}
void JIT_OP_MOVo_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_Object_t* a2)
{
    KLR_MOVo(ctx, a1, a2);
}
void JIT_OP_MOVx_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfx_t a2)
{
    KLR_MOVx(ctx, a1, a2);
}
void JIT_OP_MOVDEF_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_class_t a2)
{
    KLR_MOVDEF(ctx, a1, a2);
}
void JIT_OP_MOVSYS_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_ushort_t a2)
{
    KLR_MOVSYS(ctx, a1, a2);
}
void JIT_OP_XMOVs_(Ctx* ctx,knh_sfp_t* sfp,knh_sfx_t a1,knh_sfi_t a2)
{
    KLR_XMOVs(ctx, a1, a2);
}
void JIT_OP_XMOVo_(Ctx* ctx,knh_sfp_t* sfp,knh_sfx_t a1,knh_Object_t* a2)
{
    KLR_XMOVo(ctx, a1, a2);
}
void JIT_OP_XMOVx_(Ctx* ctx,knh_sfp_t* sfp,knh_sfx_t a1,knh_sfx_t a2)
{
    KLR_XMOVx(ctx, a1, a2);
}
void JIT_OP_XMOVDEF_(Ctx* ctx,knh_sfp_t* sfp,knh_sfx_t a1,knh_class_t a2)
{
    KLR_XMOVDEF(ctx, a1, a2);
}
void JIT_OP_XMOVSYS_(Ctx* ctx,knh_sfp_t* sfp,knh_sfx_t a1,knh_ushort_t a2)
{
    KLR_XMOVSYS(ctx, a1, a2);
}
void JIT_OP_MOVxi_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfx_t a2)
{
    KLR_MOVxi(ctx, a1, a2);
}
void JIT_OP_XMOVsi_(Ctx* ctx,knh_sfp_t* sfp,knh_sfx_t a1,knh_sfi_t a2)
{
    KLR_XMOVsi(ctx, a1, a2);
}
void JIT_OP_XMOVoi_(Ctx* ctx,knh_sfp_t* sfp,knh_sfx_t a1,knh_Object_t* a2)
{
    KLR_XMOVoi(ctx, a1, a2);
}
void JIT_OP_XMOVxi_(Ctx* ctx,knh_sfp_t* sfp,knh_sfx_t a1,knh_sfx_t a2)
{
    KLR_XMOVxi(ctx, a1, a2);
}
void JIT_OP_XMOVxio_(Ctx* ctx,knh_sfp_t* sfp,knh_sfx_t a1,knh_sfx_t a2)
{
    KLR_XMOVxio(ctx, a1, a2);
}
void JIT_OP_XMOVxBXi_(Ctx* ctx,knh_sfp_t* sfp,knh_sfx_t a1,knh_sfx_t a2,knh_class_t a3)
{
    KLR_XMOVxBXi(ctx, a1, a2, a3);
}
void JIT_OP_MOVxf_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfx_t a2)
{
    KLR_MOVxf(ctx, a1, a2);
}
void JIT_OP_XMOVsf_(Ctx* ctx,knh_sfp_t* sfp,knh_sfx_t a1,knh_sfi_t a2)
{
    KLR_XMOVsf(ctx, a1, a2);
}
void JIT_OP_XMOVof_(Ctx* ctx,knh_sfp_t* sfp,knh_sfx_t a1,knh_Object_t* a2)
{
    KLR_XMOVof(ctx, a1, a2);
}
void JIT_OP_XMOVxf_(Ctx* ctx,knh_sfp_t* sfp,knh_sfx_t a1,knh_sfx_t a2)
{
    KLR_XMOVxf(ctx, a1, a2);
}
void JIT_OP_XMOVxfo_(Ctx* ctx,knh_sfp_t* sfp,knh_sfx_t a1,knh_sfx_t a2)
{
    KLR_XMOVxfo(ctx, a1, a2);
}
void JIT_OP_XMOVxBXf_(Ctx* ctx,knh_sfp_t* sfp,knh_sfx_t a1,knh_sfx_t a2,knh_class_t a3)
{
    KLR_XMOVxBXf(ctx, a1, a2, a3);
}
void JIT_OP_MOVxb_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfx_t a2)
{
    KLR_MOVxb(ctx, a1, a2);
}
void JIT_OP_XMOVsb_(Ctx* ctx,knh_sfp_t* sfp,knh_sfx_t a1,knh_sfi_t a2)
{
    KLR_XMOVsb(ctx, a1, a2);
}
void JIT_OP_XMOVob_(Ctx* ctx,knh_sfp_t* sfp,knh_sfx_t a1,knh_Object_t* a2)
{
    KLR_XMOVob(ctx, a1, a2);
}
void JIT_OP_XMOVxb_(Ctx* ctx,knh_sfp_t* sfp,knh_sfx_t a1,knh_sfx_t a2)
{
    KLR_XMOVxb(ctx, a1, a2);
}
void JIT_OP_EMOVs_(Ctx* ctx,knh_sfp_t* sfp,knh_sfe_t a1,knh_sfe_t a2)
{
    KLR_EMOVs(ctx, a1, a2);
}
void JIT_OP_EMOVo_(Ctx* ctx,knh_sfp_t* sfp,knh_sfe_t a1,knh_Object_t* a2)
{
    KLR_EMOVo(ctx, a1, a2);
}
void JIT_OP_EMOVe_(Ctx* ctx,knh_sfp_t* sfp,knh_sfe_t a1,knh_sfe_t a2)
{
    KLR_EMOVe(ctx, a1, a2);
}
void JIT_OP_EMOVDEF_(Ctx* ctx,knh_sfp_t* sfp,knh_sfe_t a1,knh_class_t a2)
{
    KLR_EMOVDEF(ctx, a1, a2);
}
void JIT_OP_EMOVSYS_(Ctx* ctx,knh_sfp_t* sfp,knh_sfe_t a1,knh_ushort_t a2)
{
    KLR_EMOVSYS(ctx, a1, a2);
}
void JIT_OP_SWAP_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2)
{
    KLR_SWAP(ctx, a1, a2);
}
void JIT_OP_PINIDEF_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_class_t a2)
{
    KLR_PINIDEF(ctx, a1, a2);
}
void JIT_OP_PINIo_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_Object_t* a2)
{
    KLR_PINIo(ctx, a1, a2);
}
void JIT_OP_PARAMS_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_class_t a2)
{
    KLR_PARAMS(ctx, a1, a2);
}
void JIT_OP_SETESP_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1)
{
    KLR_SETESP(ctx, a1);
}
void JIT_OP_CHECKESP_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1)
{
    KLR_CHECKESP(ctx, a1);
}
void JIT_OP_RET_(Ctx* ctx,knh_sfp_t* sfp)
{
    knh_code_t* pc = NULL;
    KLR_RET(ctx);
}
void JIT_OP_RETn_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2)
{
    knh_code_t* pc = NULL;
    KLR_RETn(ctx, a1, a2);
}
void JIT_OP_RETa_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2)
{
    knh_code_t* pc = NULL;
    KLR_RETa(ctx, a1, a2);
}
void JIT_OP_RETo_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_Object_t* a2)
{
    knh_code_t* pc = NULL;
    KLR_RETo(ctx, a1, a2);
}
void JIT_OP_RETx_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfx_t a2)
{
    knh_code_t* pc = NULL;
    KLR_RETx(ctx, a1, a2);
}
void JIT_OP_YEILDBREAK_(Ctx* ctx,knh_sfp_t* sfp)
{
    KLR_YEILDBREAK(ctx);
}
void JIT_OP_BOX_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_class_t a2)
{
    KLR_BOX(ctx, a1, a2);
}
void JIT_OP_BOXnc_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_class_t a2)
{
    KLR_BOXnc(ctx, a1, a2);
}
void JIT_OP_NNBOX_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_class_t a2)
{
    KLR_NNBOX(ctx, a1, a2);
}
void JIT_OP_NNBOXnc_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_class_t a2)
{
    KLR_NNBOXnc(ctx, a1, a2);
}
void JIT_OP_UNBOX_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1)
{
    KLR_UNBOX(ctx, a1);
}
void JIT_OP_CHECKNULL_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1)
{
    knh_code_t* pc = NULL;
    KLR_CHECKNULL(ctx, a1);
}
void JIT_OP_CHECKNULLx_(Ctx* ctx,knh_sfp_t* sfp,knh_sfx_t a1)
{
    knh_code_t* pc = NULL;
    KLR_CHECKNULLx(ctx, a1);
}
void JIT_OP_CHECKTYPE_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_class_t a2)
{
    knh_code_t* pc = NULL;
    KLR_CHECKTYPE(ctx, a1, a2);
}
void JIT_OP_CHECKNNTYPE_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_type_t a2)
{
    knh_code_t* pc = NULL;
    KLR_CHECKNNTYPE(ctx, a1, a2);
}
void JIT_OP_FCALL_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_ushort_t a2,knh_sfi_t a3,knh_Object_t* a4)
{
    knh_code_t* pc = NULL;
    KLR_FCALL(ctx, a1, a2, a3, a4);
}
void JIT_OP_SCALL_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_ushort_t a2,knh_Object_t* a3)
{
    knh_code_t* pc = NULL;
    KLR_SCALL(ctx, a1, a2, a3);
}
void JIT_OP_AINVOKE_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_ushort_t a2)
{
    knh_code_t* pc = NULL;
    KLR_AINVOKE(ctx, a1, a2);
}
void JIT_OP_CALL_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_ushort_t a2,knh_methodn_t a3)
{
    knh_code_t* pc = NULL;
    KLR_CALL(ctx, a1, a2, a3);
}
void JIT_OP_ACALL_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_ushort_t a2,knh_methodn_t a3)
{
    knh_code_t* pc = NULL;
    KLR_ACALL(ctx, a1, a2, a3);
}
void JIT_OP_NEW_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_flag_t a2,knh_class_t a3,knh_ushort_t a4,knh_Object_t* a5)
{
    knh_code_t* pc = NULL;
    KLR_NEW(ctx, a1, a2, a3, a4, a5);
}
void JIT_OP_TOSTR_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_methodn_t a2)
{
    knh_code_t* pc = NULL;
    KLR_TOSTR(ctx, a1, a2);
}
void JIT_OP_TOSTRf_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_methodn_t a2,knh_Object_t* a3)
{
    knh_code_t* pc = NULL;
    KLR_TOSTRf(ctx, a1, a2, a3);
}
void JIT_OP_SMAP_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_Object_t* a2)
{
    KLR_SMAP(ctx, a1, a2);
}
void JIT_OP_SMAPnc_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_Object_t* a2)
{
    KLR_SMAPnc(ctx, a1, a2);
}
void JIT_OP_MAP_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_class_t a2)
{
    KLR_MAP(ctx, a1, a2);
}
void JIT_OP_MAPnc_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_class_t a2)
{
    KLR_MAPnc(ctx, a1, a2);
}
void JIT_OP_AMAP_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_class_t a2)
{
    KLR_AMAP(ctx, a1, a2);
}
void JIT_OP_NNMAP_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_class_t a2)
{
    KLR_NNMAP(ctx, a1, a2);
}
void JIT_OP_JMP_(Ctx* ctx,knh_sfp_t* sfp,knh_labelid_t a1)
{
    //KLR_JMP(ctx, 0, a1);
}
void JIT_OP_SKIP_(Ctx* ctx,knh_sfp_t* sfp,knh_labelid_t a1)
{
    //KLR_SKIP(ctx, 0, a1);
}
void JIT_OP_bJIFT_(Ctx* ctx,knh_sfp_t* sfp,knh_labelid_t a1,knh_sfi_t a2)
{
    //KLR_bJIFT(ctx, 0, a1, a2);
}
void JIT_OP_bJIFF_(Ctx* ctx,knh_sfp_t* sfp,knh_labelid_t a1,knh_sfi_t a2)
{
    //KLR_bJIFF(ctx, 0, a1, a2);
}
void JIT_OP_bJIFF_LOOP_(Ctx* ctx,knh_sfp_t* sfp,knh_labelid_t a1,knh_sfi_t a2)
{
    //KLR_bJIFF_LOOP(ctx, 0, a1, a2);
}
void JIT_OP_JIFNUL_(Ctx* ctx,knh_sfp_t* sfp,knh_labelid_t a1,knh_sfi_t a2)
{
    //KLR_JIFNUL(ctx, 0, a1, a2);
}
void JIT_OP_JIFNN_(Ctx* ctx,knh_sfp_t* sfp,knh_labelid_t a1,knh_sfi_t a2)
{
    //KLR_JIFNN(ctx, 0, a1, a2);
}
void JIT_OP_NEXT_(Ctx* ctx,knh_sfp_t* sfp,knh_labelid_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
    //KLR_NEXT(ctx, a1, a2, a3);
}
void JIT_OP_INEXT_(Ctx* ctx,knh_sfp_t* sfp,knh_labelid_t a1,knh_class_t a2,knh_sfi_t a3,knh_sfi_t a4)
{
    //KLR_INEXT(ctx, a1, a2, a3, a4);
}
void JIT_OP_MAPNEXT_(Ctx* ctx,knh_sfp_t* sfp,knh_labelid_t a1,knh_class_t a2,knh_sfi_t a3,knh_sfi_t a4)
{
    //KLR_MAPNEXT(ctx, a1, a2, a3, a4);
}
void JIT_OP_SMAPNEXT_(Ctx* ctx,knh_sfp_t* sfp,knh_labelid_t a1,knh_sfi_t a2,knh_sfi_t a3,knh_Object_t* a4)
{
    //KLR_SMAPNEXT(ctx, a1, a2, a3, a4);
}
void JIT_OP_TRY_(Ctx* ctx,knh_sfp_t* sfp,knh_labelid_t a1,knh_sfi_t a2)
{
    //KLR_TRY(ctx, a1, a2);
}
void JIT_OP_TRYEND_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1)
{
    KLR_TRYEND(ctx, a1);
}
void JIT_OP_CATCH_(Ctx* ctx,knh_sfp_t* sfp,knh_labelid_t a1,knh_sfi_t a2,knh_sfi_t a3,knh_Object_t* a4)
{
    //KLR_CATCH(ctx, a1, a2, a3, a4);
}
void JIT_OP_PUSH_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1)
{
    KLR_PUSH(ctx, a1);
}
void JIT_OP_POP_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1)
{
    KLR_POP(ctx, a1);
}
void JIT_OP_THROW_(Ctx* ctx,knh_sfp_t* sfp,knh_ushort_t a1,knh_sfi_t a2)
{
    knh_code_t* pc = NULL;
    KLR_THROW(ctx, a1, a2);
}
void JIT_OP_THROWs_(Ctx* ctx,knh_sfp_t* sfp,knh_ushort_t a1,knh_Object_t* a2)
{
    knh_code_t* pc = NULL;
    KLR_THROWs(ctx, a1, a2);
}
void JIT_OP_THROW_AGAIN_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1)
{
    KLR_THROW_AGAIN(ctx, a1);
}
void JIT_OP_P_(Ctx* ctx,knh_sfp_t* sfp,knh_flag_t a1,knh_methodn_t a2,knh_sfi_t a3)
{
    KLR_P(ctx, a1, a2, a3);
}
void JIT_OP_PMSG_(Ctx* ctx,knh_sfp_t* sfp,knh_flag_t a1,knh_Object_t* a2)
{
    KLR_PMSG(ctx, a1, a2);
}
void JIT_OP_iCAST_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1)
{
    KLR_iCAST(ctx, a1);
}
void JIT_OP_inCAST_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1)
{
    KLR_inCAST(ctx, a1);
}
void JIT_OP_fCAST_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1)
{
    KLR_fCAST(ctx, a1);
}
void JIT_OP_fnCAST_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1)
{
    KLR_fnCAST(ctx, a1);
}
void JIT_OP_bNOT_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2)
{
    KLR_bNOT(ctx, a1, a2);
}
void JIT_OP_iNEG_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2)
{
    KLR_iNEG(ctx, a1, a2);
}
void JIT_OP_iADD_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
    KLR_iADD(ctx, a1, a2, a3);
}
void JIT_OP_iADDn_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_int_t a3)
{
    KLR_iADDn(ctx, a1, a2, a3);
}
void JIT_OP_iSUB_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
    KLR_iSUB(ctx, a1, a2, a3);
}
void JIT_OP_iSUBn_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_int_t a3)
{
    KLR_iSUBn(ctx, a1, a2, a3);
}
void JIT_OP_iMUL_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
    KLR_iMUL(ctx, a1, a2, a3);
}
void JIT_OP_iMULn_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_int_t a3)
{
    KLR_iMULn(ctx, a1, a2, a3);
}
void JIT_OP_iDIV_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
    KLR_iDIV(ctx, a1, a2, a3);
}
void JIT_OP_iDIVn_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_int_t a3)
{
    KLR_iDIVn(ctx, a1, a2, a3);
}
void JIT_OP_iMOD_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
    KLR_iMOD(ctx, a1, a2, a3);
}
void JIT_OP_iMODn_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_int_t a3)
{
    KLR_iMODn(ctx, a1, a2, a3);
}
void JIT_OP_iEQ_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
    KLR_iEQ(ctx, a1, a2, a3);
}
void JIT_OP_iEQn_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_int_t a3)
{
    KLR_iEQn(ctx, a1, a2, a3);
}
void JIT_OP_iNEQ_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
    KLR_iNEQ(ctx, a1, a2, a3);
}
void JIT_OP_iNEQn_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_int_t a3)
{
    KLR_iNEQn(ctx, a1, a2, a3);
}
void JIT_OP_iLT_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
    KLR_iLT(ctx, a1, a2, a3);
}
void JIT_OP_iLTn_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_int_t a3)
{
    KLR_iLTn(ctx, a1, a2, a3);
}
void JIT_OP_iLTE_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
    KLR_iLTE(ctx, a1, a2, a3);
}
void JIT_OP_iLTEn_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_int_t a3)
{
    KLR_iLTEn(ctx, a1, a2, a3);
}
void JIT_OP_iGT_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
    KLR_iGT(ctx, a1, a2, a3);
}
void JIT_OP_iGTn_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_int_t a3)
{
    KLR_iGTn(ctx, a1, a2, a3);
}
void JIT_OP_iGTE_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
    KLR_iGTE(ctx, a1, a2, a3);
}
void JIT_OP_iGTEn_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_int_t a3)
{
    KLR_iGTEn(ctx, a1, a2, a3);
}
void JIT_OP_fNEG_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2)
{
    KLR_fNEG(ctx, a1, a2);
}
void JIT_OP_fADD_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
    KLR_fADD(ctx, a1, a2, a3);
}
void JIT_OP_fADDn_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_float_t a3)
{
    KLR_fADDn(ctx, a1, a2, a3);
}
void JIT_OP_fSUB_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
    KLR_fSUB(ctx, a1, a2, a3);
}
void JIT_OP_fSUBn_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_float_t a3)
{
    KLR_fSUBn(ctx, a1, a2, a3);
}
void JIT_OP_fMUL_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
    KLR_fMUL(ctx, a1, a2, a3);
}
void JIT_OP_fMULn_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_float_t a3)
{
    KLR_fMULn(ctx, a1, a2, a3);
}
void JIT_OP_fDIV_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
    KLR_fDIV(ctx, a1, a2, a3);
}
void JIT_OP_fDIVn_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_float_t a3)
{
    KLR_fDIVn(ctx, a1, a2, a3);
}
void JIT_OP_fEQ_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
    KLR_fEQ(ctx, a1, a2, a3);
}
void JIT_OP_fEQn_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_float_t a3)
{
    KLR_fEQn(ctx, a1, a2, a3);
}
void JIT_OP_fNEQ_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
    KLR_fNEQ(ctx, a1, a2, a3);
}
void JIT_OP_fNEQn_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_float_t a3)
{
    KLR_fNEQn(ctx, a1, a2, a3);
}
void JIT_OP_fLT_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
    KLR_fLT(ctx, a1, a2, a3);
}
void JIT_OP_fLTn_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_float_t a3)
{
    KLR_fLTn(ctx, a1, a2, a3);
}
void JIT_OP_fLTE_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
    KLR_fLTE(ctx, a1, a2, a3);
}
void JIT_OP_fLTEn_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_float_t a3)
{
    KLR_fLTEn(ctx, a1, a2, a3);
}
void JIT_OP_fGT_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
    KLR_fGT(ctx, a1, a2, a3);
}
void JIT_OP_fGTn_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_float_t a3)
{
    KLR_fGTn(ctx, a1, a2, a3);
}
void JIT_OP_fGTE_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
    KLR_fGTE(ctx, a1, a2, a3);
}
void JIT_OP_fGTEn_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_float_t a3)
{
    KLR_fGTEn(ctx, a1, a2, a3);
}
void JIT_OP_AGET_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
    knh_code_t* pc = NULL;
    KLR_AGET(ctx, a1, a2, a3);
}
void JIT_OP_AGETn_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_intptr_t a3)
{
    knh_code_t* pc = NULL;
    KLR_AGETn(ctx, a1, a2, a3);
}
void JIT_OP_IAGET_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
    knh_code_t* pc = NULL;
    KLR_IAGET(ctx, a1, a2, a3);
}
void JIT_OP_IAGETn_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_intptr_t a3)
{
    knh_code_t* pc = NULL;
    KLR_IAGETn(ctx, a1, a2, a3);
}
void JIT_OP_FAGET_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
    knh_code_t* pc = NULL;
    KLR_FAGET(ctx, a1, a2, a3);
}
void JIT_OP_FAGETn_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_intptr_t a3)
{
    knh_code_t* pc = NULL;
    KLR_FAGETn(ctx, a1, a2, a3);
}
void JIT_OP_ASET_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
    knh_code_t* pc = NULL;
    KLR_ASET(ctx, a1, a2, a3);
}
void JIT_OP_ASETn_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_intptr_t a3)
{
    knh_code_t* pc = NULL;
    KLR_ASETn(ctx, a1, a2, a3);
}
void JIT_OP_IASET_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
    knh_code_t* pc = NULL;
    KLR_IASET(ctx, a1, a2, a3);
}
void JIT_OP_IASETn_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_intptr_t a3)
{
    knh_code_t* pc = NULL;
    KLR_IASETn(ctx, a1, a2, a3);
}
void JIT_OP_FASET_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
    knh_code_t* pc = NULL;
    KLR_FASET(ctx, a1, a2, a3);
}
void JIT_OP_FASETn_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1,knh_sfi_t a2,knh_intptr_t a3)
{
    knh_code_t* pc = NULL;
    KLR_FASETn(ctx, a1, a2, a3);
}
void JIT_OP_INITCODE_(Ctx* ctx,knh_sfp_t* sfp,knh_sfi_t a1)
{
    //KLR_INITCODE(ctx, a1);
}
void JIT_OP_NOP_(Ctx* ctx,knh_sfp_t* sfp)
{
    KLR_NOP(ctx);
}

#endif

