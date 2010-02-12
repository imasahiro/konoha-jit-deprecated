#ifndef IJT_VM_H_
#define IJT_VM_H_
// THIS FILE WAS AUTOMATICALLY GENERATED

void JIT_OP_HALT_(Ctx *ctx, knh_sfp_t* sfp)
{
	KLR_HALT(ctx);
}

void JIT_OP_ENTER_(Ctx *ctx, knh_sfp_t* sfp)
{
	KLR_ENTER(ctx);
}

void JIT_OP_EXEC_(Ctx *ctx, knh_sfp_t* sfp)
{
	KLR_EXEC(ctx);
}

void JIT_OP_YEILD_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1)
{
	KLR_YEILD(ctx,a1);
}

void JIT_OP_EXIT_(Ctx *ctx, knh_sfp_t* sfp)
{
	KLR_EXIT(ctx);
}

void JIT_OP_TR_(Ctx *ctx, knh_sfp_t* sfp,klr_Ftr a1,knh_sfpidx_t a2,knh_class_t a3)
{
	KLR_TR(ctx,a1,a2,a3);
}

void JIT_OP_OTR_(Ctx *ctx, knh_sfp_t* sfp,klr_Ftr a1,knh_sfpidx_t a2,knh_class_t a3)
{
	KLR_OTR(ctx,a1,a2,a3);
}

void JIT_OP_NULTR_(Ctx *ctx, knh_sfp_t* sfp,klr_Ftr a1,knh_sfpidx_t a2,knh_class_t a3)
{
	KLR_NULTR(ctx,a1,a2,a3);
}

void JIT_OP_UNBOX_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1)
{
	KLR_UNBOX(ctx,a1);
}

void JIT_OP_iCAST_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1)
{
	KLR_iCAST(ctx,a1);
}

void JIT_OP_fCAST_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1)
{
	KLR_fCAST(ctx,a1);
}

void JIT_OP_OSET_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_Object_t* a2)
{
	KLR_OSET(ctx,a1,a2);
}

void JIT_OP_NSET_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_int_t a2)
{
	KLR_NSET(ctx,a1,a2);
}

void JIT_OP_MOV_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2)
{
	KLR_MOV(ctx,a1,a2);
}

void JIT_OP_OMOV_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2)
{
	KLR_OMOV(ctx,a1,a2);
}

void JIT_OP_NMOV_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2)
{
	KLR_NMOV(ctx,a1,a2);
}

void JIT_OP_OMOVx_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfx_t a2)
{
	KLR_OMOVx(ctx,a1,a2);
}

void JIT_OP_iMOVx_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfx_t a2)
{
	KLR_iMOVx(ctx,a1,a2);
}

void JIT_OP_fMOVx_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfx_t a2)
{
	KLR_fMOVx(ctx,a1,a2);
}

void JIT_OP_bMOVx_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfx_t a2)
{
	KLR_bMOVx(ctx,a1,a2);
}

void JIT_OP_XMOV_(Ctx *ctx, knh_sfp_t* sfp,knh_sfx_t a1,knh_sfpidx_t a2)
{
	KLR_XMOV(ctx,a1,a2);
}

void JIT_OP_XMOVx_(Ctx *ctx, knh_sfp_t* sfp,knh_sfx_t a1,knh_sfx_t a2)
{
	KLR_XMOVx(ctx,a1,a2);
}

void JIT_OP_XOSET_(Ctx *ctx, knh_sfp_t* sfp,knh_sfx_t a1,knh_Object_t* a2)
{
	KLR_XOSET(ctx,a1,a2);
}

void JIT_OP_XIMOV_(Ctx *ctx, knh_sfp_t* sfp,knh_sfx_t a1,knh_sfpidx_t a2)
{
	KLR_XIMOV(ctx,a1,a2);
}

void JIT_OP_XFMOV_(Ctx *ctx, knh_sfp_t* sfp,knh_sfx_t a1,knh_sfpidx_t a2)
{
	KLR_XFMOV(ctx,a1,a2);
}

void JIT_OP_XBMOV_(Ctx *ctx, knh_sfp_t* sfp,knh_sfx_t a1,knh_sfpidx_t a2)
{
	KLR_XBMOV(ctx,a1,a2);
}

void JIT_OP_MOVe_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_ushort_t a2)
{
	KLR_MOVe(ctx,a1,a2);
}

void JIT_OP_SWAP_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2)
{
	KLR_SWAP(ctx,a1,a2);
}

void JIT_OP_CHKNUL_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1)
{
	KLR_CHKNUL(ctx,a1);
}

void JIT_OP_CHKTYPE_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_class_t a2)
{
	KLR_CHKTYPE(ctx,a1,a2);
}

void JIT_OP_CHKNULx_(Ctx *ctx, knh_sfp_t* sfp,knh_sfx_t a1)
{
	KLR_CHKNULx(ctx,a1);
}

void JIT_OP_CHKTYPEx_(Ctx *ctx, knh_sfp_t* sfp,knh_sfx_t a1,knh_class_t a2)
{
	KLR_CHKTYPEx(ctx,a1,a2);
}

void JIT_OP_LDMETHOD_(Ctx *ctx, knh_sfp_t* sfp,klr_Fmethod a1,knh_sfpidx_t a2,knh_methodn_t a3)
{
	KLR_LDMETHOD(ctx,a1,a2,a3);
}

void JIT_OP_CALL_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_ushort_t a2)
{
	KLR_CALL(ctx,a1,a2);
}

void JIT_OP_SCALL_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_ushort_t a2,knh_Method_t* a3)
{
	KLR_SCALL(ctx,a1,a2,a3);
}

void JIT_OP_VCALL_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_ushort_t a2,knh_Method_t* a3)
{
	KLR_VCALL(ctx,a1,a2,a3);
}

void JIT_OP_RET_(Ctx *ctx, knh_sfp_t* sfp)
{
	KLR_RET(ctx);
}

void JIT_OP_FUNCCALL_(Ctx *ctx, knh_sfp_t* sfp)
{
	KLR_FUNCCALL(ctx);
}

void JIT_OP_SMAP_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_Mapper_t* a2)
{
	KLR_SMAP(ctx,a1,a2);
}

void JIT_OP_SMAPnc_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_Mapper_t* a2)
{
	KLR_SMAPnc(ctx,a1,a2);
}

void JIT_OP_MAP_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_class_t a2)
{
	KLR_MAP(ctx,a1,a2);
}

void JIT_OP_MAPnc_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_class_t a2)
{
	KLR_MAPnc(ctx,a1,a2);
}

void JIT_OP_AMAP_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_class_t a2)
{
	KLR_AMAP(ctx,a1,a2);
}

void JIT_OP_JMP_(Ctx *ctx, knh_sfp_t* sfp,knh_KLRInst_t* a1)
{
	KLR_JMP(ctx,a1);
}

void JIT_OP_NOPJMP_(Ctx *ctx, knh_sfp_t* sfp,knh_KLRInst_t* a1)
{
	KLR_NOPJMP(ctx,a1);
}

void JIT_OP_JMPT_(Ctx *ctx, knh_sfp_t* sfp,knh_KLRInst_t* a1,knh_sfpidx_t a2)
{
	KLR_JMPT(ctx,a1,a2);
}

void JIT_OP_JMPF_(Ctx *ctx, knh_sfp_t* sfp,knh_KLRInst_t* a1,knh_sfpidx_t a2)
{
	KLR_JMPF(ctx,a1,a2);
}

void JIT_OP_JMPF_LOOP_(Ctx *ctx, knh_sfp_t* sfp,knh_KLRInst_t* a1,knh_sfpidx_t a2)
{
	KLR_JMPF_LOOP(ctx,a1,a2);
}

void JIT_OP_JMPNUL_(Ctx *ctx, knh_sfp_t* sfp,knh_KLRInst_t* a1,knh_sfpidx_t a2)
{
	KLR_JMPNUL(ctx,a1,a2);
}

void JIT_OP_JMPNN_(Ctx *ctx, knh_sfp_t* sfp,knh_KLRInst_t* a1,knh_sfpidx_t a2)
{
	KLR_JMPNN(ctx,a1,a2);
}

void JIT_OP_JMPchk_(Ctx *ctx, knh_sfp_t* sfp,knh_KLRInst_t* a1,klr_Fchk a2,knh_sfpidx_t a3)
{
	KLR_JMPchk(ctx,a1,a2,a3);
}

void JIT_OP_JMPcmp_(Ctx *ctx, knh_sfp_t* sfp,knh_KLRInst_t* a1,klr_Fcmp a2,knh_sfpidx_t a3,knh_sfpidx_t a4)
{
	KLR_JMPcmp(ctx,a1,a2,a3,a4);
}

void JIT_OP_JMPcmpi_(Ctx *ctx, knh_sfp_t* sfp,knh_KLRInst_t* a1,klr_Fcmpi a2,knh_sfpidx_t a3,knh_int_t a4)
{
	KLR_JMPcmpi(ctx,a1,a2,a3,a4);
}

void JIT_OP_JMPcmpf_(Ctx *ctx, knh_sfp_t* sfp,knh_KLRInst_t* a1,klr_Fcmpf a2,knh_sfpidx_t a3,knh_float_t a4)
{
	KLR_JMPcmpf(ctx,a1,a2,a3,a4);
}

void JIT_OP_NEXT_(Ctx *ctx, knh_sfp_t* sfp,knh_KLRInst_t* a1,knh_sfpidx_t a2,knh_sfpidx_t a3)
{
	KLR_NEXT(ctx,a1,a2,a3);
}

void JIT_OP_NEXTf_(Ctx *ctx, knh_sfp_t* sfp,knh_KLRInst_t* a1,klr_Fnext a2,knh_class_t a3,knh_sfpidx_t a4,knh_sfpidx_t a5)
{
	KLR_NEXTf(ctx,a1,a2,a3,a4,a5);
}

void JIT_OP_TRY_(Ctx *ctx, knh_sfp_t* sfp,knh_KLRInst_t* a1,knh_sfpidx_t a2)
{
	KLR_TRY(ctx,a1,a2);
}

void JIT_OP_TRYEND_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1)
{
	KLR_TRYEND(ctx,a1);
}

void JIT_OP_CATCH_(Ctx *ctx, knh_sfp_t* sfp,knh_KLRInst_t* a1,knh_sfpidx_t a2,knh_sfpidx_t a3,knh_String_t* a4)
{
	KLR_CATCH(ctx,a1,a2,a3,a4);
}

void JIT_OP_THROW_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2)
{
	KLR_THROW(ctx,a1,a2);
}

void JIT_OP_THROWs_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_String_t* a3)
{
	KLR_THROWs(ctx,a1,a2,a3);
}

void JIT_OP_THROW_AGAIN_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1)
{
	KLR_THROW_AGAIN(ctx,a1);
}

void JIT_OP_P_(Ctx *ctx, knh_sfp_t* sfp,knh_flag_t a1,knh_methodn_t a2,knh_sfpidx_t a3)
{
	KLR_P(ctx,a1,a2,a3);
}

void JIT_OP_PMSG_(Ctx *ctx, knh_sfp_t* sfp,knh_flag_t a1,knh_String_t* a2)
{
	KLR_PMSG(ctx,a1,a2);
}

void JIT_OP_bNOT_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2)
{
	KLR_bNOT(ctx,a1,a2);
}

void JIT_OP_iNEG_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2)
{
	KLR_iNEG(ctx,a1,a2);
}

void JIT_OP_iADD_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_sfpidx_t a3)
{
	KLR_iADD(ctx,a1,a2,a3);
}

void JIT_OP_iSUB_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_sfpidx_t a3)
{
	KLR_iSUB(ctx,a1,a2,a3);
}

void JIT_OP_iMUL_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_sfpidx_t a3)
{
	KLR_iMUL(ctx,a1,a2,a3);
}

void JIT_OP_iDIV_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_sfpidx_t a3)
{
	KLR_iDIV(ctx,a1,a2,a3);
}

void JIT_OP_iMOD_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_sfpidx_t a3)
{
	KLR_iMOD(ctx,a1,a2,a3);
}

void JIT_OP_iDIVn_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_int_t a3)
{
	KLR_iDIVn(ctx,a1,a2,a3);
}

void JIT_OP_iADDn_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_int_t a3)
{
	KLR_iADDn(ctx,a1,a2,a3);
}

void JIT_OP_iSUBn_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_int_t a3)
{
	KLR_iSUBn(ctx,a1,a2,a3);
}

void JIT_OP_iMULn_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_int_t a3)
{
	KLR_iMULn(ctx,a1,a2,a3);
}

void JIT_OP_iMODn_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_int_t a3)
{
	KLR_iMODn(ctx,a1,a2,a3);
}

void JIT_OP_iEQ_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_sfpidx_t a3)
{
	KLR_iEQ(ctx,a1,a2,a3);
}

void JIT_OP_iNEQ_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_sfpidx_t a3)
{
	KLR_iNEQ(ctx,a1,a2,a3);
}

void JIT_OP_iLT_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_sfpidx_t a3)
{
	KLR_iLT(ctx,a1,a2,a3);
}

void JIT_OP_iLTE_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_sfpidx_t a3)
{
	KLR_iLTE(ctx,a1,a2,a3);
}

void JIT_OP_iGT_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_sfpidx_t a3)
{
	KLR_iGT(ctx,a1,a2,a3);
}

void JIT_OP_iGTE_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_sfpidx_t a3)
{
	KLR_iGTE(ctx,a1,a2,a3);
}

void JIT_OP_iEQn_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_int_t a3)
{
	KLR_iEQn(ctx,a1,a2,a3);
}

void JIT_OP_iNEQn_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_int_t a3)
{
	KLR_iNEQn(ctx,a1,a2,a3);
}

void JIT_OP_iLTn_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_int_t a3)
{
	KLR_iLTn(ctx,a1,a2,a3);
}

void JIT_OP_iLTEn_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_int_t a3)
{
	KLR_iLTEn(ctx,a1,a2,a3);
}

void JIT_OP_iGTn_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_int_t a3)
{
	KLR_iGTn(ctx,a1,a2,a3);
}

void JIT_OP_iGTEn_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_int_t a3)
{
	KLR_iGTEn(ctx,a1,a2,a3);
}

void JIT_OP_fNEG_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2)
{
	KLR_fNEG(ctx,a1,a2);
}

void JIT_OP_fADD_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_sfpidx_t a3)
{
	KLR_fADD(ctx,a1,a2,a3);
}

void JIT_OP_fSUB_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_sfpidx_t a3)
{
	KLR_fSUB(ctx,a1,a2,a3);
}

void JIT_OP_fMUL_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_sfpidx_t a3)
{
	KLR_fMUL(ctx,a1,a2,a3);
}

void JIT_OP_fDIV_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_sfpidx_t a3)
{
	KLR_fDIV(ctx,a1,a2,a3);
}

void JIT_OP_fADDn_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_float_t a3)
{
	KLR_fADDn(ctx,a1,a2,a3);
}

void JIT_OP_fSUBn_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_float_t a3)
{
	KLR_fSUBn(ctx,a1,a2,a3);
}

void JIT_OP_fMULn_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_float_t a3)
{
	KLR_fMULn(ctx,a1,a2,a3);
}

void JIT_OP_fDIVn_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_float_t a3)
{
	KLR_fDIVn(ctx,a1,a2,a3);
}

void JIT_OP_fEQ_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_sfpidx_t a3)
{
	KLR_fEQ(ctx,a1,a2,a3);
}

void JIT_OP_fNEQ_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_sfpidx_t a3)
{
	KLR_fNEQ(ctx,a1,a2,a3);
}

void JIT_OP_fEQn_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_float_t a3)
{
	KLR_fEQn(ctx,a1,a2,a3);
}

void JIT_OP_fNEQn_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_float_t a3)
{
	KLR_fNEQn(ctx,a1,a2,a3);
}

void JIT_OP_fLT_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_sfpidx_t a3)
{
	KLR_fLT(ctx,a1,a2,a3);
}

void JIT_OP_fLTE_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_sfpidx_t a3)
{
	KLR_fLTE(ctx,a1,a2,a3);
}

void JIT_OP_fGT_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_sfpidx_t a3)
{
	KLR_fGT(ctx,a1,a2,a3);
}

void JIT_OP_fGTE_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_sfpidx_t a3)
{
	KLR_fGTE(ctx,a1,a2,a3);
}

void JIT_OP_fLTn_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_float_t a3)
{
	KLR_fLTn(ctx,a1,a2,a3);
}

void JIT_OP_fLTEn_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_float_t a3)
{
	KLR_fLTEn(ctx,a1,a2,a3);
}

void JIT_OP_fGTn_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_float_t a3)
{
	KLR_fGTn(ctx,a1,a2,a3);
}

void JIT_OP_fGTEn_(Ctx *ctx, knh_sfp_t* sfp,knh_sfpidx_t a1,knh_sfpidx_t a2,knh_float_t a3)
{
	KLR_fGTEn(ctx,a1,a2,a3);
}

void JIT_OP_OEVAL_(Ctx *ctx, knh_sfp_t* sfp,klr_Foeval a1,knh_sfpidx_t a2,knh_sfpidx_t a3)
{
	KLR_OEVAL(ctx,a1,a2,a3);
}

void JIT_OP_GETIDX_(Ctx *ctx, knh_sfp_t* sfp,klr_Fgetidx a1,knh_sfpidx_t a2,knh_sfpidx_t a3,knh_sfpidx_t a4)
{
	KLR_GETIDX(ctx,a1,a2,a3,a4);
}

void JIT_OP_SETIDX_(Ctx *ctx, knh_sfp_t* sfp,klr_Fsetidx a1,knh_sfpidx_t a2,knh_sfpidx_t a3,knh_sfpidx_t a4,knh_sfpidx_t a5)
{
	KLR_SETIDX(ctx,a1,a2,a3,a4,a5);
}

void JIT_OP_GETIDXn_(Ctx *ctx, knh_sfp_t* sfp,klr_Fgetidx a1,knh_sfpidx_t a2,knh_sfpidx_t a3,knh_intptr_t a4)
{
	KLR_GETIDXn(ctx,a1,a2,a3,a4);
}

void JIT_OP_SETIDXn_(Ctx *ctx, knh_sfp_t* sfp,klr_Fsetidx a1,knh_sfpidx_t a2,knh_sfpidx_t a3,knh_sfpidx_t a4,knh_intptr_t a5)
{
	KLR_SETIDXn(ctx,a1,a2,a3,a4,a5);
}

void JIT_OP_THCODE_(Ctx *ctx, knh_sfp_t* sfp)
{
	KLR_THCODE(ctx);
}

void JIT_OP_LABEL_(Ctx *ctx, knh_sfp_t* sfp,knh_intptr_t a1,knh_String_t* a2)
{
	KLR_LABEL(ctx,a1,a2);
}

void JIT_OP_PROBE_(Ctx *ctx, knh_sfp_t* sfp,klr_Fprobe a1,knh_ushort_t a2)
{
	KLR_PROBE(ctx,a1,a2);
}

void JIT_OP_NOP_(Ctx *ctx, knh_sfp_t* sfp)
{
	KLR_NOP(ctx);
}
#endif /* IJT_VM_H_ */
