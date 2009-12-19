
/* ************************************************************************ */

#include "../kc/commons.h"
#include <ijt/konoha_ctt.h>

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif


/* ======================================================================== */
#ifdef KNH_KLR_CONVERT

/* copy from ../kc/konoha_code_.c */
#define OPJUMP NULL
#define CASE(L, OP)   case OP :
#define NEXT L_HEAD
#define JUMP L_HEAD
#define TC(c)    c


static void knh_ctt_patch(imem_t* mem);

METHOD knh_KLRCode_toCttCode(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	imem_t* mem = imem_new(1);
	knh_Method_t *mtd = (knh_Method_t *) sfp[-1].o;
	register knh_code_t *pc = (sfp[-1].mtd)->pc_start;
	jit_start(mem);
	L_HEAD:;
	switch(KNH_OPCODE(pc)) {

	CASE(L_HALT, OPCODE_HALT) {
		TC(const klr_HALT_t *op = (klr_HALT_t*)pc;)
		KLR_HALT(ctx);
		pc += OPSIZE_HALT;
		goto NEXT;
	} 
	CASE(L_MOVa, OPCODE_MOVa) {
		const klr_MOVa_t *op = (klr_MOVa_t*)pc;
		KLR_MOVa(ctx, op->a1, op->a2);
		pc += OPSIZE_MOVa;
		goto NEXT;
	} 
	CASE(L_MOVn, OPCODE_MOVn) {
		const klr_MOVn_t *op = (klr_MOVn_t*)pc;
		KLR_MOVn(ctx, op->a1, op->a2);
		pc += OPSIZE_MOVn;
		goto NEXT;
	} 
	CASE(L_MOVo, OPCODE_MOVo) {
		const klr_MOVo_t *op = (klr_MOVo_t*)pc;
		KLR_MOVo(ctx, op->a1, op->a2);
		pc += OPSIZE_MOVo;
		goto NEXT;
	} 
	CASE(L_MOVi, OPCODE_MOVi) {
		const klr_MOVi_t *op = (klr_MOVi_t*)pc;
		KLR_MOVi(ctx, op->a1, op->a2);
		pc += OPSIZE_MOVi;
		goto NEXT;
	} 
	CASE(L_MOVx, OPCODE_MOVx) {
		const klr_MOVx_t *op = (klr_MOVx_t*)pc;
		KLR_MOVx(ctx, op->a1, op->a2);
		pc += OPSIZE_MOVx;
		goto NEXT;
	} 
	CASE(L_MOVDEF, OPCODE_MOVDEF) {
		const klr_MOVDEF_t *op = (klr_MOVDEF_t*)pc;
		KLR_MOVDEF(ctx, op->a1, op->a2);
		pc += OPSIZE_MOVDEF;
		goto NEXT;
	} 
	CASE(L_MOVSYS, OPCODE_MOVSYS) {
		const klr_MOVSYS_t *op = (klr_MOVSYS_t*)pc;
		KLR_MOVSYS(ctx, op->a1, op->a2);
		pc += OPSIZE_MOVSYS;
		goto NEXT;
	} 
	CASE(L_MOVxi, OPCODE_MOVxi) {
		const klr_MOVxi_t *op = (klr_MOVxi_t*)pc;
		KLR_MOVxi(ctx, op->a1, op->a2);
		pc += OPSIZE_MOVxi;
		goto NEXT;
	} 
	CASE(L_MOVxf, OPCODE_MOVxf) {
		const klr_MOVxf_t *op = (klr_MOVxf_t*)pc;
		KLR_MOVxf(ctx, op->a1, op->a2);
		pc += OPSIZE_MOVxf;
		goto NEXT;
	} 
	CASE(L_MOVxb, OPCODE_MOVxb) {
		const klr_MOVxb_t *op = (klr_MOVxb_t*)pc;
		KLR_MOVxb(ctx, op->a1, op->a2);
		pc += OPSIZE_MOVxb;
		goto NEXT;
	} 
	CASE(L_MOVe, OPCODE_MOVe) {
		const klr_MOVe_t *op = (klr_MOVe_t*)pc;
		KLR_MOVe(ctx, op->a1, op->a2);
		pc += OPSIZE_MOVe;
		goto NEXT;
	} 
	CASE(L_XMOVs, OPCODE_XMOVs) {
		const klr_XMOVs_t *op = (klr_XMOVs_t*)pc;
		KLR_XMOVs(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVs;
		goto NEXT;
	} 
	CASE(L_XMOVo, OPCODE_XMOVo) {
		const klr_XMOVo_t *op = (klr_XMOVo_t*)pc;
		KLR_XMOVo(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVo;
		goto NEXT;
	} 
	CASE(L_XMOVx, OPCODE_XMOVx) {
		const klr_XMOVx_t *op = (klr_XMOVx_t*)pc;
		KLR_XMOVx(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVx;
		goto NEXT;
	} 
	CASE(L_XMOVDEF, OPCODE_XMOVDEF) {
		const klr_XMOVDEF_t *op = (klr_XMOVDEF_t*)pc;
		KLR_XMOVDEF(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVDEF;
		goto NEXT;
	} 
	CASE(L_XMOVSYS, OPCODE_XMOVSYS) {
		const klr_XMOVSYS_t *op = (klr_XMOVSYS_t*)pc;
		KLR_XMOVSYS(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVSYS;
		goto NEXT;
	} 
	CASE(L_XMOVsi, OPCODE_XMOVsi) {
		const klr_XMOVsi_t *op = (klr_XMOVsi_t*)pc;
		KLR_XMOVsi(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVsi;
		goto NEXT;
	} 
	CASE(L_XMOVsf, OPCODE_XMOVsf) {
		const klr_XMOVsf_t *op = (klr_XMOVsf_t*)pc;
		KLR_XMOVsf(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVsf;
		goto NEXT;
	} 
	CASE(L_XMOVsb, OPCODE_XMOVsb) {
		const klr_XMOVsb_t *op = (klr_XMOVsb_t*)pc;
		KLR_XMOVsb(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVsb;
		goto NEXT;
	} 
	CASE(L_SWAP, OPCODE_SWAP) {
		const klr_SWAP_t *op = (klr_SWAP_t*)pc;
		KLR_SWAP(ctx, op->a1, op->a2);
		pc += OPSIZE_SWAP;
		goto NEXT;
	} 
	CASE(L_PARAMDEF, OPCODE_PARAMDEF) {
		const klr_PARAMDEF_t *op = (klr_PARAMDEF_t*)pc;
		KLR_PARAMDEF(ctx, op->a1, op->a2);
		pc += OPSIZE_PARAMDEF;
		goto NEXT;
	} 
	CASE(L_PARAMo, OPCODE_PARAMo) {
		const klr_PARAMo_t *op = (klr_PARAMo_t*)pc;
		KLR_PARAMo(ctx, op->a1, op->a2);
		pc += OPSIZE_PARAMo;
		goto NEXT;
	} 
	CASE(L_PARAMPROP, OPCODE_PARAMPROP) {
		const klr_PARAMPROP_t *op = (klr_PARAMPROP_t*)pc;
		KLR_PARAMPROP(ctx, op->a1, op->a2);
		pc += OPSIZE_PARAMPROP;
		goto NEXT;
	} 
	CASE(L_PARAMS, OPCODE_PARAMS) {
		const klr_PARAMS_t *op = (klr_PARAMS_t*)pc;
		KLR_PARAMS(ctx, op->a1, op->a2);
		pc += OPSIZE_PARAMS;
		goto NEXT;
	} 
	CASE(L_CHKESP, OPCODE_CHKESP) {
		const klr_CHKESP_t *op = (klr_CHKESP_t*)pc;
		KLR_CHKESP(ctx, op->a1);
		pc += OPSIZE_CHKESP;
		goto NEXT;
	} 
	CASE(L_RET, OPCODE_RET) {
		TC(const klr_RET_t *op = (klr_RET_t*)pc;)
		KLR_RET(ctx);
		pc += OPSIZE_RET;
		goto NEXT;
	} 
	CASE(L_YEILDBREAK, OPCODE_YEILDBREAK) {
		TC(const klr_YEILDBREAK_t *op = (klr_YEILDBREAK_t*)pc;)
		KLR_YEILDBREAK(ctx);
		pc += OPSIZE_YEILDBREAK;
		goto NEXT;
	} 
	CASE(L_BOX, OPCODE_BOX) {
		const klr_BOX_t *op = (klr_BOX_t*)pc;
		KLR_BOX(ctx, op->a1, op->a2);
		pc += OPSIZE_BOX;
		goto NEXT;
	} 
	CASE(L_BOXnc, OPCODE_BOXnc) {
		const klr_BOXnc_t *op = (klr_BOXnc_t*)pc;
		KLR_BOXnc(ctx, op->a1, op->a2);
		pc += OPSIZE_BOXnc;
		goto NEXT;
	} 
	CASE(L_NNBOX, OPCODE_NNBOX) {
		const klr_NNBOX_t *op = (klr_NNBOX_t*)pc;
		KLR_NNBOX(ctx, op->a1, op->a2);
		pc += OPSIZE_NNBOX;
		goto NEXT;
	} 
	CASE(L_NNBOXnc, OPCODE_NNBOXnc) {
		const klr_NNBOXnc_t *op = (klr_NNBOXnc_t*)pc;
		KLR_NNBOXnc(ctx, op->a1, op->a2);
		pc += OPSIZE_NNBOXnc;
		goto NEXT;
	} 
	CASE(L_UNBOX, OPCODE_UNBOX) {
		const klr_UNBOX_t *op = (klr_UNBOX_t*)pc;
		KLR_UNBOX(ctx, op->a1);
		pc += OPSIZE_UNBOX;
		goto NEXT;
	} 
	CASE(L_CHKNUL, OPCODE_CHKNUL) {
		const klr_CHKNUL_t *op = (klr_CHKNUL_t*)pc;
		KLR_CHKNUL(ctx, op->a1);
		pc += OPSIZE_CHKNUL;
		goto NEXT;
	} 
	CASE(L_CHKNULx, OPCODE_CHKNULx) {
		const klr_CHKNULx_t *op = (klr_CHKNULx_t*)pc;
		KLR_CHKNULx(ctx, op->a1);
		pc += OPSIZE_CHKNULx;
		goto NEXT;
	} 
	CASE(L_CHKTYPE, OPCODE_CHKTYPE) {
		const klr_CHKTYPE_t *op = (klr_CHKTYPE_t*)pc;
		KLR_CHKTYPE(ctx, op->a1, op->a2);
		pc += OPSIZE_CHKTYPE;
		goto NEXT;
	} 
	CASE(L_FCALL, OPCODE_FCALL) {
		const klr_FCALL_t *op = (klr_FCALL_t*)pc;
		KLR_FCALL(ctx, op->a1, op->a2, op->a3, op->a4);
		pc += OPSIZE_FCALL;
		goto NEXT;
	} 
	CASE(L_SCALL, OPCODE_SCALL) {
		const klr_SCALL_t *op = (klr_SCALL_t*)pc;
		KLR_SCALL(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_SCALL;
		goto NEXT;
	} 
	CASE(L_AINVOKE, OPCODE_AINVOKE) {
		const klr_AINVOKE_t *op = (klr_AINVOKE_t*)pc;
		KLR_AINVOKE(ctx, op->a1, op->a2);
		pc += OPSIZE_AINVOKE;
		goto NEXT;
	} 
	CASE(L_CALL, OPCODE_CALL) {
		const klr_CALL_t *op = (klr_CALL_t*)pc;
		KLR_CALL(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_CALL;
		goto NEXT;
	} 
	CASE(L_ACALL, OPCODE_ACALL) {
		const klr_ACALL_t *op = (klr_ACALL_t*)pc;
		KLR_ACALL(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_ACALL;
		goto NEXT;
	} 
	CASE(L_NEW, OPCODE_NEW) {
		const klr_NEW_t *op = (klr_NEW_t*)pc;
		KLR_NEW(ctx, op->a1, op->a2, op->a3, op->a4, op->a5);
		pc += OPSIZE_NEW;
		goto NEXT;
	} 
	CASE(L_COPYSFP, OPCODE_COPYSFP) {
		const klr_COPYSFP_t *op = (klr_COPYSFP_t*)pc;
		KLR_COPYSFP(ctx, op->a1);
		pc += OPSIZE_COPYSFP;
		goto NEXT;
	} 
	CASE(L_STR, OPCODE_STR) {
		const klr_STR_t *op = (klr_STR_t*)pc;
		KLR_STR(ctx, op->a1, op->a2, op->a3, op->a4);
		pc += OPSIZE_STR;
		goto NEXT;
	} 
	CASE(L_SSTR, OPCODE_SSTR) {
		const klr_SSTR_t *op = (klr_SSTR_t*)pc;
		KLR_SSTR(ctx, op->a1, op->a2, op->a3, op->a4);
		pc += OPSIZE_SSTR;
		goto NEXT;
	} 
	CASE(L_SMAP, OPCODE_SMAP) {
		const klr_SMAP_t *op = (klr_SMAP_t*)pc;
		KLR_SMAP(ctx, op->a1, op->a2);
		pc += OPSIZE_SMAP;
		goto NEXT;
	} 
	CASE(L_SMAPnc, OPCODE_SMAPnc) {
		const klr_SMAPnc_t *op = (klr_SMAPnc_t*)pc;
		KLR_SMAPnc(ctx, op->a1, op->a2);
		pc += OPSIZE_SMAPnc;
		goto NEXT;
	} 
	CASE(L_MAP, OPCODE_MAP) {
		const klr_MAP_t *op = (klr_MAP_t*)pc;
		KLR_MAP(ctx, op->a1, op->a2);
		pc += OPSIZE_MAP;
		goto NEXT;
	} 
	CASE(L_MAPnc, OPCODE_MAPnc) {
		const klr_MAPnc_t *op = (klr_MAPnc_t*)pc;
		KLR_MAPnc(ctx, op->a1, op->a2);
		pc += OPSIZE_MAPnc;
		goto NEXT;
	} 
	CASE(L_AMAP, OPCODE_AMAP) {
		const klr_AMAP_t *op = (klr_AMAP_t*)pc;
		KLR_AMAP(ctx, op->a1, op->a2);
		pc += OPSIZE_AMAP;
		goto NEXT;
	} 
	CASE(L_NNMAP, OPCODE_NNMAP) {
		const klr_NNMAP_t *op = (klr_NNMAP_t*)pc;
		KLR_NNMAP(ctx, op->a1, op->a2);
		pc += OPSIZE_NNMAP;
		goto NEXT;
	} 
	CASE(L_JMP, OPCODE_JMP) {
		const klr_JMP_t *op = (klr_JMP_t*)pc;
		KLR_JMP(ctx, ((knh_inst_t*)op)->jumppc/*, JUMP*/);
		pc += OPSIZE_JMP;
		goto NEXT;
	} 
	CASE(L_SKIP, OPCODE_SKIP) {
		const klr_SKIP_t *op = (klr_SKIP_t*)pc;
		KLR_SKIP(ctx, ((knh_inst_t*)op)->jumppc/*, JUMP*/);
		pc += OPSIZE_SKIP;
		goto NEXT;
	} 
	CASE(L_bJIFT, OPCODE_bJIFT) {
		const klr_bJIFT_t *op = (klr_bJIFT_t*)pc;
		KLR_bJIFT(ctx, ((knh_inst_t*)op)->jumppc/*, JUMP*/, op->a2);
		pc += OPSIZE_bJIFT;
		goto NEXT;
	} 
	CASE(L_bJIFF, OPCODE_bJIFF) {
		const klr_bJIFF_t *op = (klr_bJIFF_t*)pc;
		KLR_bJIFF(ctx, ((knh_inst_t*)op)->jumppc/*, JUMP*/, op->a2);
		pc += OPSIZE_bJIFF;
		goto NEXT;
	} 
	CASE(L_bJIFF_LOOP, OPCODE_bJIFF_LOOP) {
		const klr_bJIFF_LOOP_t *op = (klr_bJIFF_LOOP_t*)pc;
		KLR_bJIFF_LOOP(ctx, ((knh_inst_t*)op)->jumppc/*, JUMP*/, op->a2);
		pc += OPSIZE_bJIFF_LOOP;
		goto NEXT;
	} 
	CASE(L_JIFNUL, OPCODE_JIFNUL) {
		const klr_JIFNUL_t *op = (klr_JIFNUL_t*)pc;
		KLR_JIFNUL(ctx, ((knh_inst_t*)op)->jumppc/*, JUMP*/, op->a2);
		pc += OPSIZE_JIFNUL;
		goto NEXT;
	} 
	CASE(L_JIFNN, OPCODE_JIFNN) {
		const klr_JIFNN_t *op = (klr_JIFNN_t*)pc;
		KLR_JIFNN(ctx, ((knh_inst_t*)op)->jumppc/*, JUMP*/, op->a2);
		pc += OPSIZE_JIFNN;
		goto NEXT;
	} 
	CASE(L_NEXT, OPCODE_NEXT) {
		const klr_NEXT_t *op = (klr_NEXT_t*)pc;
		KLR_NEXT(ctx, ((knh_inst_t*)op)->jumppc/*, JUMP*/, op->a2, op->a3);
		pc += OPSIZE_NEXT;
		goto NEXT;
	} 
	CASE(L_INEXT, OPCODE_INEXT) {
		const klr_INEXT_t *op = (klr_INEXT_t*)pc;
		KLR_INEXT(ctx, ((knh_inst_t*)op)->jumppc/*, JUMP*/, op->a2, op->a3, op->a4);
		pc += OPSIZE_INEXT;
		goto NEXT;
	} 
	CASE(L_TRY, OPCODE_TRY) {
		const klr_TRY_t *op = (klr_TRY_t*)pc;
		KLR_TRY(ctx, ((knh_inst_t*)op)->jumppc/*, JUMP*/, op->a2);
		pc += OPSIZE_TRY;
		goto NEXT;
	} 
	CASE(L_TRYEND, OPCODE_TRYEND) {
		const klr_TRYEND_t *op = (klr_TRYEND_t*)pc;
		KLR_TRYEND(ctx, op->a1);
		pc += OPSIZE_TRYEND;
		goto NEXT;
	} 
	CASE(L_CATCH, OPCODE_CATCH) {
		const klr_CATCH_t *op = (klr_CATCH_t*)pc;
		KLR_CATCH(ctx, ((knh_inst_t*)op)->jumppc/*, JUMP*/, op->a2, op->a3, op->a4);
		pc += OPSIZE_CATCH;
		goto NEXT;
	} 
	CASE(L_PUSH, OPCODE_PUSH) {
		const klr_PUSH_t *op = (klr_PUSH_t*)pc;
		KLR_PUSH(ctx, op->a1);
		pc += OPSIZE_PUSH;
		goto NEXT;
	} 
	CASE(L_POP, OPCODE_POP) {
		const klr_POP_t *op = (klr_POP_t*)pc;
		KLR_POP(ctx, op->a1);
		pc += OPSIZE_POP;
		goto NEXT;
	} 
	CASE(L_THROW, OPCODE_THROW) {
		const klr_THROW_t *op = (klr_THROW_t*)pc;
		KLR_THROW(ctx, op->a1, op->a2);
		pc += OPSIZE_THROW;
		goto NEXT;
	} 
	CASE(L_THROWs, OPCODE_THROWs) {
		const klr_THROWs_t *op = (klr_THROWs_t*)pc;
		KLR_THROWs(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_THROWs;
		goto NEXT;
	} 
	CASE(L_THROW_AGAIN, OPCODE_THROW_AGAIN) {
		const klr_THROW_AGAIN_t *op = (klr_THROW_AGAIN_t*)pc;
		KLR_THROW_AGAIN(ctx, op->a1);
		pc += OPSIZE_THROW_AGAIN;
		goto NEXT;
	} 
	CASE(L_P, OPCODE_P) {
		const klr_P_t *op = (klr_P_t*)pc;
		KLR_P(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_P;
		goto NEXT;
	} 
	CASE(L_PMSG, OPCODE_PMSG) {
		const klr_PMSG_t *op = (klr_PMSG_t*)pc;
		KLR_PMSG(ctx, op->a1, op->a2);
		pc += OPSIZE_PMSG;
		goto NEXT;
	} 
	CASE(L_iCAST, OPCODE_iCAST) {
		const klr_iCAST_t *op = (klr_iCAST_t*)pc;
		KLR_iCAST(ctx, op->a1);
		pc += OPSIZE_iCAST;
		goto NEXT;
	} 
	CASE(L_inCAST, OPCODE_inCAST) {
		const klr_inCAST_t *op = (klr_inCAST_t*)pc;
		KLR_inCAST(ctx, op->a1);
		pc += OPSIZE_inCAST;
		goto NEXT;
	} 
	CASE(L_fCAST, OPCODE_fCAST) {
		const klr_fCAST_t *op = (klr_fCAST_t*)pc;
		KLR_fCAST(ctx, op->a1);
		pc += OPSIZE_fCAST;
		goto NEXT;
	} 
	CASE(L_fnCAST, OPCODE_fnCAST) {
		const klr_fnCAST_t *op = (klr_fnCAST_t*)pc;
		KLR_fnCAST(ctx, op->a1);
		pc += OPSIZE_fnCAST;
		goto NEXT;
	} 
	CASE(L_bNOT, OPCODE_bNOT) {
		const klr_bNOT_t *op = (klr_bNOT_t*)pc;
		KLR_bNOT(ctx, op->a1, op->a2);
		pc += OPSIZE_bNOT;
		goto NEXT;
	} 
	CASE(L_iNEG, OPCODE_iNEG) {
		const klr_iNEG_t *op = (klr_iNEG_t*)pc;
		KLR_iNEG(ctx, op->a1, op->a2);
		pc += OPSIZE_iNEG;
		goto NEXT;
	} 
	CASE(L_iADD, OPCODE_iADD) {
		const klr_iADD_t *op = (klr_iADD_t*)pc;
		KLR_iADD(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iADD;
		goto NEXT;
	} 
	CASE(L_iADDn, OPCODE_iADDn) {
		const klr_iADDn_t *op = (klr_iADDn_t*)pc;
		KLR_iADDn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iADDn;
		goto NEXT;
	} 
	CASE(L_iSUB, OPCODE_iSUB) {
		const klr_iSUB_t *op = (klr_iSUB_t*)pc;
		KLR_iSUB(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iSUB;
		goto NEXT;
	} 
	CASE(L_iSUBn, OPCODE_iSUBn) {
		const klr_iSUBn_t *op = (klr_iSUBn_t*)pc;
		KLR_iSUBn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iSUBn;
		goto NEXT;
	} 
	CASE(L_iMUL, OPCODE_iMUL) {
		const klr_iMUL_t *op = (klr_iMUL_t*)pc;
		KLR_iMUL(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iMUL;
		goto NEXT;
	} 
	CASE(L_iMULn, OPCODE_iMULn) {
		const klr_iMULn_t *op = (klr_iMULn_t*)pc;
		KLR_iMULn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iMULn;
		goto NEXT;
	} 
	CASE(L_iDIV, OPCODE_iDIV) {
		const klr_iDIV_t *op = (klr_iDIV_t*)pc;
		KLR_iDIV(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iDIV;
		goto NEXT;
	} 
	CASE(L_iDIVn, OPCODE_iDIVn) {
		const klr_iDIVn_t *op = (klr_iDIVn_t*)pc;
		KLR_iDIVn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iDIVn;
		goto NEXT;
	} 
	CASE(L_iMOD, OPCODE_iMOD) {
		const klr_iMOD_t *op = (klr_iMOD_t*)pc;
		KLR_iMOD(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iMOD;
		goto NEXT;
	} 
	CASE(L_iMODn, OPCODE_iMODn) {
		const klr_iMODn_t *op = (klr_iMODn_t*)pc;
		KLR_iMODn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iMODn;
		goto NEXT;
	} 
	CASE(L_iEQ, OPCODE_iEQ) {
		const klr_iEQ_t *op = (klr_iEQ_t*)pc;
		KLR_iEQ(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iEQ;
		goto NEXT;
	} 
	CASE(L_iEQn, OPCODE_iEQn) {
		const klr_iEQn_t *op = (klr_iEQn_t*)pc;
		KLR_iEQn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iEQn;
		goto NEXT;
	} 
	CASE(L_iNEQ, OPCODE_iNEQ) {
		const klr_iNEQ_t *op = (klr_iNEQ_t*)pc;
		KLR_iNEQ(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iNEQ;
		goto NEXT;
	} 
	CASE(L_iNEQn, OPCODE_iNEQn) {
		const klr_iNEQn_t *op = (klr_iNEQn_t*)pc;
		KLR_iNEQn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iNEQn;
		goto NEXT;
	} 
	CASE(L_iLT, OPCODE_iLT) {
		const klr_iLT_t *op = (klr_iLT_t*)pc;
		KLR_iLT(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iLT;
		goto NEXT;
	} 
	CASE(L_iLTn, OPCODE_iLTn) {
		const klr_iLTn_t *op = (klr_iLTn_t*)pc;
		KLR_iLTn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iLTn;
		goto NEXT;
	} 
	CASE(L_iLTE, OPCODE_iLTE) {
		const klr_iLTE_t *op = (klr_iLTE_t*)pc;
		KLR_iLTE(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iLTE;
		goto NEXT;
	} 
	CASE(L_iLTEn, OPCODE_iLTEn) {
		const klr_iLTEn_t *op = (klr_iLTEn_t*)pc;
		KLR_iLTEn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iLTEn;
		goto NEXT;
	} 
	CASE(L_iGT, OPCODE_iGT) {
		const klr_iGT_t *op = (klr_iGT_t*)pc;
		KLR_iGT(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iGT;
		goto NEXT;
	} 
	CASE(L_iGTn, OPCODE_iGTn) {
		const klr_iGTn_t *op = (klr_iGTn_t*)pc;
		KLR_iGTn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iGTn;
		goto NEXT;
	} 
	CASE(L_iGTE, OPCODE_iGTE) {
		const klr_iGTE_t *op = (klr_iGTE_t*)pc;
		KLR_iGTE(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iGTE;
		goto NEXT;
	} 
	CASE(L_iGTEn, OPCODE_iGTEn) {
		const klr_iGTEn_t *op = (klr_iGTEn_t*)pc;
		KLR_iGTEn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iGTEn;
		goto NEXT;
	} 
	CASE(L_fNEG, OPCODE_fNEG) {
		const klr_fNEG_t *op = (klr_fNEG_t*)pc;
		KLR_fNEG(ctx, op->a1, op->a2);
		pc += OPSIZE_fNEG;
		goto NEXT;
	} 
	CASE(L_fADD, OPCODE_fADD) {
		const klr_fADD_t *op = (klr_fADD_t*)pc;
		KLR_fADD(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fADD;
		goto NEXT;
	} 
	CASE(L_fADDn, OPCODE_fADDn) {
		const klr_fADDn_t *op = (klr_fADDn_t*)pc;
		KLR_fADDn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fADDn;
		goto NEXT;
	} 
	CASE(L_fSUB, OPCODE_fSUB) {
		const klr_fSUB_t *op = (klr_fSUB_t*)pc;
		KLR_fSUB(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fSUB;
		goto NEXT;
	} 
	CASE(L_fSUBn, OPCODE_fSUBn) {
		const klr_fSUBn_t *op = (klr_fSUBn_t*)pc;
		KLR_fSUBn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fSUBn;
		goto NEXT;
	} 
	CASE(L_fMUL, OPCODE_fMUL) {
		const klr_fMUL_t *op = (klr_fMUL_t*)pc;
		KLR_fMUL(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fMUL;
		goto NEXT;
	} 
	CASE(L_fMULn, OPCODE_fMULn) {
		const klr_fMULn_t *op = (klr_fMULn_t*)pc;
		KLR_fMULn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fMULn;
		goto NEXT;
	} 
	CASE(L_fDIV, OPCODE_fDIV) {
		const klr_fDIV_t *op = (klr_fDIV_t*)pc;
		KLR_fDIV(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fDIV;
		goto NEXT;
	} 
	CASE(L_fDIVn, OPCODE_fDIVn) {
		const klr_fDIVn_t *op = (klr_fDIVn_t*)pc;
		KLR_fDIVn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fDIVn;
		goto NEXT;
	} 
	CASE(L_fEQ, OPCODE_fEQ) {
		const klr_fEQ_t *op = (klr_fEQ_t*)pc;
		KLR_fEQ(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fEQ;
		goto NEXT;
	} 
	CASE(L_fEQn, OPCODE_fEQn) {
		const klr_fEQn_t *op = (klr_fEQn_t*)pc;
		KLR_fEQn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fEQn;
		goto NEXT;
	} 
	CASE(L_fNEQ, OPCODE_fNEQ) {
		const klr_fNEQ_t *op = (klr_fNEQ_t*)pc;
		KLR_fNEQ(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fNEQ;
		goto NEXT;
	} 
	CASE(L_fNEQn, OPCODE_fNEQn) {
		const klr_fNEQn_t *op = (klr_fNEQn_t*)pc;
		KLR_fNEQn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fNEQn;
		goto NEXT;
	} 
	CASE(L_fLT, OPCODE_fLT) {
		const klr_fLT_t *op = (klr_fLT_t*)pc;
		KLR_fLT(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fLT;
		goto NEXT;
	} 
	CASE(L_fLTn, OPCODE_fLTn) {
		const klr_fLTn_t *op = (klr_fLTn_t*)pc;
		KLR_fLTn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fLTn;
		goto NEXT;
	} 
	CASE(L_fLTE, OPCODE_fLTE) {
		const klr_fLTE_t *op = (klr_fLTE_t*)pc;
		KLR_fLTE(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fLTE;
		goto NEXT;
	} 
	CASE(L_fLTEn, OPCODE_fLTEn) {
		const klr_fLTEn_t *op = (klr_fLTEn_t*)pc;
		KLR_fLTEn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fLTEn;
		goto NEXT;
	} 
	CASE(L_fGT, OPCODE_fGT) {
		const klr_fGT_t *op = (klr_fGT_t*)pc;
		KLR_fGT(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fGT;
		goto NEXT;
	} 
	CASE(L_fGTn, OPCODE_fGTn) {
		const klr_fGTn_t *op = (klr_fGTn_t*)pc;
		KLR_fGTn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fGTn;
		goto NEXT;
	} 
	CASE(L_fGTE, OPCODE_fGTE) {
		const klr_fGTE_t *op = (klr_fGTE_t*)pc;
		KLR_fGTE(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fGTE;
		goto NEXT;
	} 
	CASE(L_fGTEn, OPCODE_fGTEn) {
		const klr_fGTEn_t *op = (klr_fGTEn_t*)pc;
		KLR_fGTEn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fGTEn;
		goto NEXT;
	} 
	CASE(L_ARYGET, OPCODE_ARYGET) {
		const klr_ARYGET_t *op = (klr_ARYGET_t*)pc;
		KLR_ARYGET(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_ARYGET;
		goto NEXT;
	} 
	CASE(L_ARYGETn, OPCODE_ARYGETn) {
		const klr_ARYGETn_t *op = (klr_ARYGETn_t*)pc;
		KLR_ARYGETn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_ARYGETn;
		goto NEXT;
	} 
	CASE(L_iARYGET, OPCODE_iARYGET) {
		const klr_iARYGET_t *op = (klr_iARYGET_t*)pc;
		KLR_iARYGET(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iARYGET;
		goto NEXT;
	} 
	CASE(L_iARYGETn, OPCODE_iARYGETn) {
		const klr_iARYGETn_t *op = (klr_iARYGETn_t*)pc;
		KLR_iARYGETn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iARYGETn;
		goto NEXT;
	} 
	CASE(L_fARYGET, OPCODE_fARYGET) {
		const klr_fARYGET_t *op = (klr_fARYGET_t*)pc;
		KLR_fARYGET(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fARYGET;
		goto NEXT;
	} 
	CASE(L_fARYGETn, OPCODE_fARYGETn) {
		const klr_fARYGETn_t *op = (klr_fARYGETn_t*)pc;
		KLR_fARYGETn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fARYGETn;
		goto NEXT;
	} 
	CASE(L_ARYSET, OPCODE_ARYSET) {
		const klr_ARYSET_t *op = (klr_ARYSET_t*)pc;
		KLR_ARYSET(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_ARYSET;
		goto NEXT;
	} 
	CASE(L_ARYSETn, OPCODE_ARYSETn) {
		const klr_ARYSETn_t *op = (klr_ARYSETn_t*)pc;
		KLR_ARYSETn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_ARYSETn;
		goto NEXT;
	} 
	CASE(L_iARYSET, OPCODE_iARYSET) {
		const klr_iARYSET_t *op = (klr_iARYSET_t*)pc;
		KLR_iARYSET(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iARYSET;
		goto NEXT;
	} 
	CASE(L_iARYSETn, OPCODE_iARYSETn) {
		const klr_iARYSETn_t *op = (klr_iARYSETn_t*)pc;
		KLR_iARYSETn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iARYSETn;
		goto NEXT;
	} 
	CASE(L_fARYSET, OPCODE_fARYSET) {
		const klr_fARYSET_t *op = (klr_fARYSET_t*)pc;
		KLR_fARYSET(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fARYSET;
		goto NEXT;
	} 
	CASE(L_fARYSETn, OPCODE_fARYSETn) {
		const klr_fARYSETn_t *op = (klr_fARYSETn_t*)pc;
		KLR_fARYSETn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fARYSETn;
		goto NEXT;
	} 
	CASE(L_THCODE, OPCODE_THCODE) {
		const klr_THCODE_t *op = (klr_THCODE_t*)pc;
		KLR_THCODE(ctx, op->a1);
		pc += OPSIZE_THCODE;
		goto NEXT;
	} 
	CASE(L_NOP, OPCODE_NOP) {
		TC(const klr_NOP_t *op = (klr_NOP_t*)pc;)
		KLR_NOP(ctx);
		pc += OPSIZE_NOP;
		goto NEXT;
	} 
	}
	KNH_WARNING(ctx, "unknown opcode=%d", KNH_OPCODE(pc));
	goto L_HEAD;

CTT_FIN:
	{
		jit_complete(mem);
		knh_ctt_patch(mem);
		imem_complete(mem);
		knh_Method_syncFunc(mtd,(knh_Fmethod) mem->addr);
	}
}

/* ------------------------------------------------------------------------ */
//static void knh_arr_dump(unsigned char* addr,int n)
//{
//	int i;
//	for (i = 0; i < n; i++) {
//		fprintf(stderr, "%x ",addr[i]);
//	}
//	fprintf(stderr, "\n");
//}

/* ------------------------------------------------------------------------ */

static void knh_ctt_patch(imem_t* mem)
{
	lstack_t* head = mem->lhead;
	lstack_t* stack = NULL;

	imem_setLabel(mem, 255, 255, 255);

	for(stack = head;stack->opcode != 255; stack += 1) {
		//fprintf(stderr, "pos=%3x,opcode=%3d,opsize=%d,jmpaddr=%d\n",
		//stack->pos,stack->opcode,stack->size,stack->addr);
		if(stack->addr > 0) {
			lstack_t* _s = NULL;
			for(_s = stack; _s->opcode != 255; _s += 1) {
				stack->addr -= _s->size;
				//fprintf(stderr, "%d\n",stack->addr);
				//fprintf(stderr, "\tpos=%3x,opcode=%3x,opsize=%3x,\n",
				//_s->pos,_s->opcode,_s->size);
				if(stack->addr <= 0) {
					_s = _s + 1;
					//fprintf(stderr, "ok ps=%x,sp=%x l=%x\n",_s->pos,
					//stack->pos, _s->pos - stack->pos - 2);
					//fprintf(stderr, "p=%x\n",mem->addr[stack->pos + 1]);
					if(stack->opcode == OPCODE_JMP) {
						if(mem->addr[stack->pos] == 0xe9) {
							int32_t *addr = (int32_t*)(&mem->addr[stack->pos+1]);
							addr[0] = (int32_t)(_s->pos - stack->pos - 2);
						} else {
							mem->addr[stack->pos+1] = _s->pos - stack->pos - 2;
						}
					}
					else {
						mem->addr[stack->pos+1] = _s->pos - stack->pos - 2;
					}
					//fprintf(stderr, "p=%x\n",mem->addr[stack->pos + 1]);
					break;
				}
			}
		}
	}
}

/* ------------------------------------------------------------------------ */

#endif

#ifdef __cplusplus
}
#endif
