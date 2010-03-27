//
///* ************************************************************************ */
//
//#include"commons.h"
////#include "ijt/convert.h"
////#include "ijt/imem.h"
////#include "ijt/vm.h"
//
///* ************************************************************************ */
//
//#ifdef __cplusplus
//extern "C" {
//#endif
//
//
///* ======================================================================== */

//static void knh_ctt_patch(imem_t* mem);
//
//METHOD knh_KLRCode_toCttCode(Ctx *ctx, knh_sfp_t *sfp)
//{
//    imem_t* mem = imem_new(1);
//    knh_Method_t *mtd = (knh_Method_t *) sfp[-1].o;
//    register knh_code_t *pc = sfp[-1].pc;
//    if(!pc) return;
//    jit_start(mem);
//
//    L_HEAD:;
//    switch(((knh_kode_t*)pc)->opcode) {
//
//    case OPCODE_HALT : {
//        const klr_halt_t* op = (klr_halt_t*)pc;
//        JIT_ASM_HALT(ctx);
//        pc += OPSIZE_HALT;
//        goto L_HEAD;
//    }
//
//    case OPCODE_MOVA : {
//        const klr_mova_t* op = (klr_mova_t*)pc;
//        JIT_ASM_MOVa(ctx, op->a1, op->a2);
//        pc += OPSIZE_MOVA;
//        goto L_HEAD;
//    }
//
//    case OPCODE_MOVN : {
//        const klr_movn_t* op = (klr_movn_t*)pc;
//        JIT_ASM_MOVn(ctx, op->a1, op->a2);
//        pc += OPSIZE_MOVN;
//        goto L_HEAD;
//    }
//
//    case OPCODE_MOVO : {
//        const klr_movo_t* op = (klr_movo_t*)pc;
//        JIT_ASM_MOVo(ctx, op->a1, op->a2);
//        pc += OPSIZE_MOVO;
//        goto L_HEAD;
//    }
//
//    case OPCODE_MOVX : {
//        const klr_movx_t* op = (klr_movx_t*)pc;
//        JIT_ASM_MOVx(ctx, op->a1, op->a2);
//        pc += OPSIZE_MOVX;
//        goto L_HEAD;
//    }
//
//    case OPCODE_MOVDEF : {
//        const klr_movdef_t* op = (klr_movdef_t*)pc;
//        JIT_ASM_MOVDEF(ctx, op->a1, op->a2);
//        pc += OPSIZE_MOVDEF;
//        goto L_HEAD;
//    }
//
//    case OPCODE_MOVSYS : {
//        const klr_movsys_t* op = (klr_movsys_t*)pc;
//        JIT_ASM_MOVSYS(ctx, op->a1, op->a2);
//        pc += OPSIZE_MOVSYS;
//        goto L_HEAD;
//    }
//
//    case OPCODE_XMOVS : {
//        const klr_xmovs_t* op = (klr_xmovs_t*)pc;
//        JIT_ASM_XMOVs(ctx, op->a1, op->a2);
//        pc += OPSIZE_XMOVS;
//        goto L_HEAD;
//    }
//
//    case OPCODE_XMOVO : {
//        const klr_xmovo_t* op = (klr_xmovo_t*)pc;
//        JIT_ASM_XMOVo(ctx, op->a1, op->a2);
//        pc += OPSIZE_XMOVO;
//        goto L_HEAD;
//    }
//
//    case OPCODE_XMOVX : {
//        const klr_xmovx_t* op = (klr_xmovx_t*)pc;
//        JIT_ASM_XMOVx(ctx, op->a1, op->a2);
//        pc += OPSIZE_XMOVX;
//        goto L_HEAD;
//    }
//
//    case OPCODE_XMOVDEF : {
//        const klr_xmovdef_t* op = (klr_xmovdef_t*)pc;
//        JIT_ASM_XMOVDEF(ctx, op->a1, op->a2);
//        pc += OPSIZE_XMOVDEF;
//        goto L_HEAD;
//    }
//
//    case OPCODE_XMOVSYS : {
//        const klr_xmovsys_t* op = (klr_xmovsys_t*)pc;
//        JIT_ASM_XMOVSYS(ctx, op->a1, op->a2);
//        pc += OPSIZE_XMOVSYS;
//        goto L_HEAD;
//    }
//
//    case OPCODE_MOVXI : {
//        const klr_movxi_t* op = (klr_movxi_t*)pc;
//        JIT_ASM_MOVxi(ctx, op->a1, op->a2);
//        pc += OPSIZE_MOVXI;
//        goto L_HEAD;
//    }
//
//    case OPCODE_XMOVSI : {
//        const klr_xmovsi_t* op = (klr_xmovsi_t*)pc;
//        JIT_ASM_XMOVsi(ctx, op->a1, op->a2);
//        pc += OPSIZE_XMOVSI;
//        goto L_HEAD;
//    }
//
//    case OPCODE_XMOVOI : {
//        const klr_xmovoi_t* op = (klr_xmovoi_t*)pc;
//        JIT_ASM_XMOVoi(ctx, op->a1, op->a2);
//        pc += OPSIZE_XMOVOI;
//        goto L_HEAD;
//    }
//
//    case OPCODE_XMOVXI : {
//        const klr_xmovxi_t* op = (klr_xmovxi_t*)pc;
//        JIT_ASM_XMOVxi(ctx, op->a1, op->a2);
//        pc += OPSIZE_XMOVXI;
//        goto L_HEAD;
//    }
//
//    case OPCODE_XMOVXIO : {
//        const klr_xmovxio_t* op = (klr_xmovxio_t*)pc;
//        JIT_ASM_XMOVxio(ctx, op->a1, op->a2);
//        pc += OPSIZE_XMOVXIO;
//        goto L_HEAD;
//    }
//
//    case OPCODE_XMOVXBXI : {
//        const klr_xmovxbxi_t* op = (klr_xmovxbxi_t*)pc;
//        JIT_ASM_XMOVxBXi(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_XMOVXBXI;
//        goto L_HEAD;
//    }
//
//    case OPCODE_MOVXF : {
//        const klr_movxf_t* op = (klr_movxf_t*)pc;
//        JIT_ASM_MOVxf(ctx, op->a1, op->a2);
//        pc += OPSIZE_MOVXF;
//        goto L_HEAD;
//    }
//
//    case OPCODE_XMOVSF : {
//        const klr_xmovsf_t* op = (klr_xmovsf_t*)pc;
//        JIT_ASM_XMOVsf(ctx, op->a1, op->a2);
//        pc += OPSIZE_XMOVSF;
//        goto L_HEAD;
//    }
//
//    case OPCODE_XMOVOF : {
//        const klr_xmovof_t* op = (klr_xmovof_t*)pc;
//        JIT_ASM_XMOVof(ctx, op->a1, op->a2);
//        pc += OPSIZE_XMOVOF;
//        goto L_HEAD;
//    }
//
//    case OPCODE_XMOVXF : {
//        const klr_xmovxf_t* op = (klr_xmovxf_t*)pc;
//        JIT_ASM_XMOVxf(ctx, op->a1, op->a2);
//        pc += OPSIZE_XMOVXF;
//        goto L_HEAD;
//    }
//
//    case OPCODE_XMOVXFO : {
//        const klr_xmovxfo_t* op = (klr_xmovxfo_t*)pc;
//        JIT_ASM_XMOVxfo(ctx, op->a1, op->a2);
//        pc += OPSIZE_XMOVXFO;
//        goto L_HEAD;
//    }
//
//    case OPCODE_XMOVXBXF : {
//        const klr_xmovxbxf_t* op = (klr_xmovxbxf_t*)pc;
//        JIT_ASM_XMOVxBXf(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_XMOVXBXF;
//        goto L_HEAD;
//    }
//
//    case OPCODE_MOVXB : {
//        const klr_movxb_t* op = (klr_movxb_t*)pc;
//        JIT_ASM_MOVxb(ctx, op->a1, op->a2);
//        pc += OPSIZE_MOVXB;
//        goto L_HEAD;
//    }
//
//    case OPCODE_XMOVSB : {
//        const klr_xmovsb_t* op = (klr_xmovsb_t*)pc;
//        JIT_ASM_XMOVsb(ctx, op->a1, op->a2);
//        pc += OPSIZE_XMOVSB;
//        goto L_HEAD;
//    }
//
//    case OPCODE_XMOVOB : {
//        const klr_xmovob_t* op = (klr_xmovob_t*)pc;
//        JIT_ASM_XMOVob(ctx, op->a1, op->a2);
//        pc += OPSIZE_XMOVOB;
//        goto L_HEAD;
//    }
//
//    case OPCODE_XMOVXB : {
//        const klr_xmovxb_t* op = (klr_xmovxb_t*)pc;
//        JIT_ASM_XMOVxb(ctx, op->a1, op->a2);
//        pc += OPSIZE_XMOVXB;
//        goto L_HEAD;
//    }
//
//    case OPCODE_EMOVS : {
//        const klr_emovs_t* op = (klr_emovs_t*)pc;
//        JIT_ASM_EMOVs(ctx, op->a1, op->a2);
//        pc += OPSIZE_EMOVS;
//        goto L_HEAD;
//    }
//
//    case OPCODE_EMOVO : {
//        const klr_emovo_t* op = (klr_emovo_t*)pc;
//        JIT_ASM_EMOVo(ctx, op->a1, op->a2);
//        pc += OPSIZE_EMOVO;
//        goto L_HEAD;
//    }
//
//    case OPCODE_EMOVE : {
//        const klr_emove_t* op = (klr_emove_t*)pc;
//        JIT_ASM_EMOVe(ctx, op->a1, op->a2);
//        pc += OPSIZE_EMOVE;
//        goto L_HEAD;
//    }
//
//    case OPCODE_EMOVDEF : {
//        const klr_emovdef_t* op = (klr_emovdef_t*)pc;
//        JIT_ASM_EMOVDEF(ctx, op->a1, op->a2);
//        pc += OPSIZE_EMOVDEF;
//        goto L_HEAD;
//    }
//
//    case OPCODE_EMOVSYS : {
//        const klr_emovsys_t* op = (klr_emovsys_t*)pc;
//        JIT_ASM_EMOVSYS(ctx, op->a1, op->a2);
//        pc += OPSIZE_EMOVSYS;
//        goto L_HEAD;
//    }
//
//    case OPCODE_SWAP : {
//        const klr_swap_t* op = (klr_swap_t*)pc;
//        JIT_ASM_SWAP(ctx, op->a1, op->a2);
//        pc += OPSIZE_SWAP;
//        goto L_HEAD;
//    }
//
//    case OPCODE_PINIDEF : {
//        const klr_pinidef_t* op = (klr_pinidef_t*)pc;
//        JIT_ASM_PINIDEF(ctx, op->a1, op->a2);
//        pc += OPSIZE_PINIDEF;
//        goto L_HEAD;
//    }
//
//    case OPCODE_PINIO : {
//        const klr_pinio_t* op = (klr_pinio_t*)pc;
//        JIT_ASM_PINIo(ctx, op->a1, op->a2);
//        pc += OPSIZE_PINIO;
//        goto L_HEAD;
//    }
//
//    case OPCODE_PARAMS : {
//        const klr_params_t* op = (klr_params_t*)pc;
//        JIT_ASM_PARAMS(ctx, op->a1, op->a2);
//        pc += OPSIZE_PARAMS;
//        goto L_HEAD;
//    }
//
//    case OPCODE_SETESP : {
//        const klr_setesp_t* op = (klr_setesp_t*)pc;
//        JIT_ASM_SETESP(ctx, op->a1);
//        pc += OPSIZE_SETESP;
//        goto L_HEAD;
//    }
//
//    case OPCODE_CHECKESP : {
//        const klr_checkesp_t* op = (klr_checkesp_t*)pc;
//        JIT_ASM_CHECKESP(ctx, op->a1);
//        pc += OPSIZE_CHECKESP;
//        goto L_HEAD;
//    }
//
//    case OPCODE_RET : {
//        const klr_ret_t* op = (klr_ret_t*)pc;
//        JIT_ASM_RET(ctx);
//        pc += OPSIZE_RET;
//        goto L_HEAD;
//    }
//
//    case OPCODE_RETN : {
//        const klr_retn_t* op = (klr_retn_t*)pc;
//        JIT_ASM_RETn(ctx, op->a1, op->a2);
//        pc += OPSIZE_RETN;
//        goto L_HEAD;
//    }
//
//    case OPCODE_RETA : {
//        const klr_reta_t* op = (klr_reta_t*)pc;
//        JIT_ASM_RETa(ctx, op->a1, op->a2);
//        pc += OPSIZE_RETA;
//        goto L_HEAD;
//    }
//
//    case OPCODE_RETO : {
//        const klr_reto_t* op = (klr_reto_t*)pc;
//        JIT_ASM_RETo(ctx, op->a1, op->a2);
//        pc += OPSIZE_RETO;
//        goto L_HEAD;
//    }
//
//    case OPCODE_RETX : {
//        const klr_retx_t* op = (klr_retx_t*)pc;
//        JIT_ASM_RETx(ctx, op->a1, op->a2);
//        pc += OPSIZE_RETX;
//        goto L_HEAD;
//    }
//
//    case OPCODE_YEILDBREAK : {
//        const klr_yeildbreak_t* op = (klr_yeildbreak_t*)pc;
//        JIT_ASM_YEILDBREAK(ctx);
//        pc += OPSIZE_YEILDBREAK;
//        goto L_HEAD;
//    }
//
//    case OPCODE_BOX : {
//        const klr_box_t* op = (klr_box_t*)pc;
//        JIT_ASM_BOX(ctx, op->a1, op->a2);
//        pc += OPSIZE_BOX;
//        goto L_HEAD;
//    }
//
//    case OPCODE_BOXNC : {
//        const klr_boxnc_t* op = (klr_boxnc_t*)pc;
//        JIT_ASM_BOXnc(ctx, op->a1, op->a2);
//        pc += OPSIZE_BOXNC;
//        goto L_HEAD;
//    }
//
//    case OPCODE_NNBOX : {
//        const klr_nnbox_t* op = (klr_nnbox_t*)pc;
//        JIT_ASM_NNBOX(ctx, op->a1, op->a2);
//        pc += OPSIZE_NNBOX;
//        goto L_HEAD;
//    }
//
//    case OPCODE_NNBOXNC : {
//        const klr_nnboxnc_t* op = (klr_nnboxnc_t*)pc;
//        JIT_ASM_NNBOXnc(ctx, op->a1, op->a2);
//        pc += OPSIZE_NNBOXNC;
//        goto L_HEAD;
//    }
//
//    case OPCODE_UNBOX : {
//        const klr_unbox_t* op = (klr_unbox_t*)pc;
//        JIT_ASM_UNBOX(ctx, op->a1);
//        pc += OPSIZE_UNBOX;
//        goto L_HEAD;
//    }
//
//    case OPCODE_CHECKNULL : {
//        const klr_checknull_t* op = (klr_checknull_t*)pc;
//        JIT_ASM_CHECKNULL(ctx, op->a1);
//        pc += OPSIZE_CHECKNULL;
//        goto L_HEAD;
//    }
//
//    case OPCODE_CHECKNULLX : {
//        const klr_checknullx_t* op = (klr_checknullx_t*)pc;
//        JIT_ASM_CHECKNULLx(ctx, op->a1);
//        pc += OPSIZE_CHECKNULLX;
//        goto L_HEAD;
//    }
//
//    case OPCODE_CHECKTYPE : {
//        const klr_checktype_t* op = (klr_checktype_t*)pc;
//        JIT_ASM_CHECKTYPE(ctx, op->a1, op->a2);
//        pc += OPSIZE_CHECKTYPE;
//        goto L_HEAD;
//    }
//
//    case OPCODE_CHECKNNTYPE : {
//        const klr_checknntype_t* op = (klr_checknntype_t*)pc;
//        JIT_ASM_CHECKNNTYPE(ctx, op->a1, op->a2);
//        pc += OPSIZE_CHECKNNTYPE;
//        goto L_HEAD;
//    }
//
//    case OPCODE_FCALL : {
//        const klr_fcall_t* op = (klr_fcall_t*)pc;
//        JIT_ASM_FCALL(ctx, op->a1, op->a2, op->a3, op->a4);
//        pc += OPSIZE_FCALL;
//        goto L_HEAD;
//    }
//
//    case OPCODE_SCALL : {
//        const klr_scall_t* op = (klr_scall_t*)pc;
//        JIT_ASM_SCALL(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_SCALL;
//        goto L_HEAD;
//    }
//
//    case OPCODE_AINVOKE : {
//        const klr_ainvoke_t* op = (klr_ainvoke_t*)pc;
//        JIT_ASM_AINVOKE(ctx, op->a1, op->a2);
//        pc += OPSIZE_AINVOKE;
//        goto L_HEAD;
//    }
//
//    case OPCODE_CALL : {
//        const klr_call_t* op = (klr_call_t*)pc;
//        JIT_ASM_CALL(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_CALL;
//        goto L_HEAD;
//    }
//
//    case OPCODE_ACALL : {
//        const klr_acall_t* op = (klr_acall_t*)pc;
//        JIT_ASM_ACALL(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_ACALL;
//        goto L_HEAD;
//    }
//
//    case OPCODE_NEW : {
//        const klr_new_t* op = (klr_new_t*)pc;
//        JIT_ASM_NEW(ctx, op->a1, op->a2, op->a3, op->a4, op->a5);
//        pc += OPSIZE_NEW;
//        goto L_HEAD;
//    }
//
//    case OPCODE_TOSTR : {
//        const klr_tostr_t* op = (klr_tostr_t*)pc;
//        JIT_ASM_TOSTR(ctx, op->a1, op->a2);
//        pc += OPSIZE_TOSTR;
//        goto L_HEAD;
//    }
//
//    case OPCODE_TOSTRF : {
//        const klr_tostrf_t* op = (klr_tostrf_t*)pc;
//        JIT_ASM_TOSTRf(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_TOSTRF;
//        goto L_HEAD;
//    }
//
//    case OPCODE_SMAP : {
//        const klr_smap_t* op = (klr_smap_t*)pc;
//        JIT_ASM_SMAP(ctx, op->a1, op->a2);
//        pc += OPSIZE_SMAP;
//        goto L_HEAD;
//    }
//
//    case OPCODE_SMAPNC : {
//        const klr_smapnc_t* op = (klr_smapnc_t*)pc;
//        JIT_ASM_SMAPnc(ctx, op->a1, op->a2);
//        pc += OPSIZE_SMAPNC;
//        goto L_HEAD;
//    }
//
//    case OPCODE_MAP : {
//        const klr_map_t* op = (klr_map_t*)pc;
//        JIT_ASM_MAP(ctx, op->a1, op->a2);
//        pc += OPSIZE_MAP;
//        goto L_HEAD;
//    }
//
//    case OPCODE_MAPNC : {
//        const klr_mapnc_t* op = (klr_mapnc_t*)pc;
//        JIT_ASM_MAPnc(ctx, op->a1, op->a2);
//        pc += OPSIZE_MAPNC;
//        goto L_HEAD;
//    }
//
//    case OPCODE_AMAP : {
//        const klr_amap_t* op = (klr_amap_t*)pc;
//        JIT_ASM_AMAP(ctx, op->a1, op->a2);
//        pc += OPSIZE_AMAP;
//        goto L_HEAD;
//    }
//
//    case OPCODE_NNMAP : {
//        const klr_nnmap_t* op = (klr_nnmap_t*)pc;
//        JIT_ASM_NNMAP(ctx, op->a1, op->a2);
//        pc += OPSIZE_NNMAP;
//        goto L_HEAD;
//    }
//
//    case OPCODE_JMP : {
//        const klr_jmp_t* op = (klr_jmp_t*)pc;
//        JIT_ASM_JMP(ctx, op->jumppc, 0 /* L_HEAD */);
//        pc += OPSIZE_JMP;
//        goto L_HEAD;
//    }
//
//    case OPCODE_SKIP : {
//        const klr_skip_t* op = (klr_skip_t*)pc;
//        JIT_ASM_SKIP(ctx, op->jumppc, 0 /* L_HEAD */);
//        pc += OPSIZE_SKIP;
//        goto L_HEAD;
//    }
//
//    case OPCODE_BJIFT : {
//        const klr_bjift_t* op = (klr_bjift_t*)pc;
//        JIT_ASM_bJIFT(ctx, op->jumppc, 0 /* L_HEAD */, op->a2);
//        pc += OPSIZE_BJIFT;
//        goto L_HEAD;
//    }
//
//    case OPCODE_BJIFF : {
//        const klr_bjiff_t* op = (klr_bjiff_t*)pc;
//        JIT_ASM_bJIFF(ctx, op->jumppc, 0 /* L_HEAD */, op->a2);
//        pc += OPSIZE_BJIFF;
//        goto L_HEAD;
//    }
//
//    case OPCODE_BJIFF_LOOP : {
//        const klr_bjiff_loop_t* op = (klr_bjiff_loop_t*)pc;
//        JIT_ASM_bJIFF_LOOP(ctx, op->jumppc, 0 /* L_HEAD */, op->a2);
//        pc += OPSIZE_BJIFF_LOOP;
//        goto L_HEAD;
//    }
//
//    case OPCODE_JIFNUL : {
//        const klr_jifnul_t* op = (klr_jifnul_t*)pc;
//        JIT_ASM_JIFNUL(ctx, op->jumppc, 0 /* L_HEAD */, op->a2);
//        pc += OPSIZE_JIFNUL;
//        goto L_HEAD;
//    }
//
//    case OPCODE_JIFNN : {
//        const klr_jifnn_t* op = (klr_jifnn_t*)pc;
//        JIT_ASM_JIFNN(ctx, op->jumppc, 0 /* L_HEAD */, op->a2);
//        pc += OPSIZE_JIFNN;
//        goto L_HEAD;
//    }
//
//    case OPCODE_NEXT : {
//        const klr_next_t* op = (klr_next_t*)pc;
//        JIT_ASM_NEXT(ctx, op->jumppc, 0 /* L_HEAD */, op->a2, op->a3);
//        pc += OPSIZE_NEXT;
//        goto L_HEAD;
//    }
//
//    case OPCODE_INEXT : {
//        const klr_inext_t* op = (klr_inext_t*)pc;
//        JIT_ASM_INEXT(ctx, op->jumppc, 0 /* L_HEAD */, op->a2, op->a3, op->a4);
//        pc += OPSIZE_INEXT;
//        goto L_HEAD;
//    }
//
//    case OPCODE_MAPNEXT : {
//        const klr_mapnext_t* op = (klr_mapnext_t*)pc;
//        JIT_ASM_MAPNEXT(ctx, op->jumppc, 0 /* L_HEAD */, op->a2, op->a3, op->a4);
//        pc += OPSIZE_MAPNEXT;
//        goto L_HEAD;
//    }
//
//    case OPCODE_SMAPNEXT : {
//        const klr_smapnext_t* op = (klr_smapnext_t*)pc;
//        JIT_ASM_SMAPNEXT(ctx, op->jumppc, 0 /* L_HEAD */, op->a2, op->a3, op->a4);
//        pc += OPSIZE_SMAPNEXT;
//        goto L_HEAD;
//    }
//
//    case OPCODE_TRY : {
//        const klr_try_t* op = (klr_try_t*)pc;
//        JIT_ASM_TRY(ctx, op->jumppc, 0 /* L_HEAD */, op->a2);
//        pc += OPSIZE_TRY;
//        goto L_HEAD;
//    }
//
//    case OPCODE_TRYEND : {
//        const klr_tryend_t* op = (klr_tryend_t*)pc;
//        JIT_ASM_TRYEND(ctx, op->a1);
//        pc += OPSIZE_TRYEND;
//        goto L_HEAD;
//    }
//
//    case OPCODE_CATCH : {
//        const klr_catch_t* op = (klr_catch_t*)pc;
//        JIT_ASM_CATCH(ctx, op->jumppc, 0 /* L_HEAD */, op->a2, op->a3, op->a4);
//        pc += OPSIZE_CATCH;
//        goto L_HEAD;
//    }
//
//    case OPCODE_PUSH : {
//        const klr_push_t* op = (klr_push_t*)pc;
//        JIT_ASM_PUSH(ctx, op->a1);
//        pc += OPSIZE_PUSH;
//        goto L_HEAD;
//    }
//
//    case OPCODE_POP : {
//        const klr_pop_t* op = (klr_pop_t*)pc;
//        JIT_ASM_POP(ctx, op->a1);
//        pc += OPSIZE_POP;
//        goto L_HEAD;
//    }
//
//    case OPCODE_THROW : {
//        const klr_throw_t* op = (klr_throw_t*)pc;
//        JIT_ASM_THROW(ctx, op->a1, op->a2);
//        pc += OPSIZE_THROW;
//        goto L_HEAD;
//    }
//
//    case OPCODE_THROWS : {
//        const klr_throws_t* op = (klr_throws_t*)pc;
//        JIT_ASM_THROWs(ctx, op->a1, op->a2);
//        pc += OPSIZE_THROWS;
//        goto L_HEAD;
//    }
//
//    case OPCODE_THROW_AGAIN : {
//        const klr_throw_again_t* op = (klr_throw_again_t*)pc;
//        JIT_ASM_THROW_AGAIN(ctx, op->a1);
//        pc += OPSIZE_THROW_AGAIN;
//        goto L_HEAD;
//    }
//
//    case OPCODE_P : {
//        const klr_p_t* op = (klr_p_t*)pc;
//        JIT_ASM_P(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_P;
//        goto L_HEAD;
//    }
//
//    case OPCODE_PMSG : {
//        const klr_pmsg_t* op = (klr_pmsg_t*)pc;
//        JIT_ASM_PMSG(ctx, op->a1, op->a2);
//        pc += OPSIZE_PMSG;
//        goto L_HEAD;
//    }
//
//    case OPCODE_ICAST : {
//        const klr_icast_t* op = (klr_icast_t*)pc;
//        JIT_ASM_iCAST(ctx, op->a1);
//        pc += OPSIZE_ICAST;
//        goto L_HEAD;
//    }
//
//    case OPCODE_INCAST : {
//        const klr_incast_t* op = (klr_incast_t*)pc;
//        JIT_ASM_inCAST(ctx, op->a1);
//        pc += OPSIZE_INCAST;
//        goto L_HEAD;
//    }
//
//    case OPCODE_FCAST : {
//        const klr_fcast_t* op = (klr_fcast_t*)pc;
//        JIT_ASM_fCAST(ctx, op->a1);
//        pc += OPSIZE_FCAST;
//        goto L_HEAD;
//    }
//
//    case OPCODE_FNCAST : {
//        const klr_fncast_t* op = (klr_fncast_t*)pc;
//        JIT_ASM_fnCAST(ctx, op->a1);
//        pc += OPSIZE_FNCAST;
//        goto L_HEAD;
//    }
//
//    case OPCODE_BNOT : {
//        const klr_bnot_t* op = (klr_bnot_t*)pc;
//        JIT_ASM_bNOT(ctx, op->a1, op->a2);
//        pc += OPSIZE_BNOT;
//        goto L_HEAD;
//    }
//
//    case OPCODE_INEG : {
//        const klr_ineg_t* op = (klr_ineg_t*)pc;
//        JIT_ASM_iNEG(ctx, op->a1, op->a2);
//        pc += OPSIZE_INEG;
//        goto L_HEAD;
//    }
//
//    case OPCODE_IADD : {
//        const klr_iadd_t* op = (klr_iadd_t*)pc;
//        JIT_ASM_iADD(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_IADD;
//        goto L_HEAD;
//    }
//
//    case OPCODE_IADDN : {
//        const klr_iaddn_t* op = (klr_iaddn_t*)pc;
//        JIT_ASM_iADDn(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_IADDN;
//        goto L_HEAD;
//    }
//
//    case OPCODE_ISUB : {
//        const klr_isub_t* op = (klr_isub_t*)pc;
//        JIT_ASM_iSUB(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_ISUB;
//        goto L_HEAD;
//    }
//
//    case OPCODE_ISUBN : {
//        const klr_isubn_t* op = (klr_isubn_t*)pc;
//        JIT_ASM_iSUBn(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_ISUBN;
//        goto L_HEAD;
//    }
//
//    case OPCODE_IMUL : {
//        const klr_imul_t* op = (klr_imul_t*)pc;
//        JIT_ASM_iMUL(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_IMUL;
//        goto L_HEAD;
//    }
//
//    case OPCODE_IMULN : {
//        const klr_imuln_t* op = (klr_imuln_t*)pc;
//        JIT_ASM_iMULn(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_IMULN;
//        goto L_HEAD;
//    }
//
//    case OPCODE_IDIV : {
//        const klr_idiv_t* op = (klr_idiv_t*)pc;
//        JIT_ASM_iDIV(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_IDIV;
//        goto L_HEAD;
//    }
//
//    case OPCODE_IDIVN : {
//        const klr_idivn_t* op = (klr_idivn_t*)pc;
//        JIT_ASM_iDIVn(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_IDIVN;
//        goto L_HEAD;
//    }
//
//    case OPCODE_IMOD : {
//        const klr_imod_t* op = (klr_imod_t*)pc;
//        JIT_ASM_iMOD(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_IMOD;
//        goto L_HEAD;
//    }
//
//    case OPCODE_IMODN : {
//        const klr_imodn_t* op = (klr_imodn_t*)pc;
//        JIT_ASM_iMODn(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_IMODN;
//        goto L_HEAD;
//    }
//
//    case OPCODE_IEQ : {
//        const klr_ieq_t* op = (klr_ieq_t*)pc;
//        JIT_ASM_iEQ(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_IEQ;
//        goto L_HEAD;
//    }
//
//    case OPCODE_IEQN : {
//        const klr_ieqn_t* op = (klr_ieqn_t*)pc;
//        JIT_ASM_iEQn(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_IEQN;
//        goto L_HEAD;
//    }
//
//    case OPCODE_INEQ : {
//        const klr_ineq_t* op = (klr_ineq_t*)pc;
//        JIT_ASM_iNEQ(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_INEQ;
//        goto L_HEAD;
//    }
//
//    case OPCODE_INEQN : {
//        const klr_ineqn_t* op = (klr_ineqn_t*)pc;
//        JIT_ASM_iNEQn(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_INEQN;
//        goto L_HEAD;
//    }
//
//    case OPCODE_ILT : {
//        const klr_ilt_t* op = (klr_ilt_t*)pc;
//        JIT_ASM_iLT(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_ILT;
//        goto L_HEAD;
//    }
//
//    case OPCODE_ILTN : {
//        const klr_iltn_t* op = (klr_iltn_t*)pc;
//        JIT_ASM_iLTn(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_ILTN;
//        goto L_HEAD;
//    }
//
//    case OPCODE_ILTE : {
//        const klr_ilte_t* op = (klr_ilte_t*)pc;
//        JIT_ASM_iLTE(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_ILTE;
//        goto L_HEAD;
//    }
//
//    case OPCODE_ILTEN : {
//        const klr_ilten_t* op = (klr_ilten_t*)pc;
//        JIT_ASM_iLTEn(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_ILTEN;
//        goto L_HEAD;
//    }
//
//    case OPCODE_IGT : {
//        const klr_igt_t* op = (klr_igt_t*)pc;
//        JIT_ASM_iGT(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_IGT;
//        goto L_HEAD;
//    }
//
//    case OPCODE_IGTN : {
//        const klr_igtn_t* op = (klr_igtn_t*)pc;
//        JIT_ASM_iGTn(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_IGTN;
//        goto L_HEAD;
//    }
//
//    case OPCODE_IGTE : {
//        const klr_igte_t* op = (klr_igte_t*)pc;
//        JIT_ASM_iGTE(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_IGTE;
//        goto L_HEAD;
//    }
//
//    case OPCODE_IGTEN : {
//        const klr_igten_t* op = (klr_igten_t*)pc;
//        JIT_ASM_iGTEn(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_IGTEN;
//        goto L_HEAD;
//    }
//
//    case OPCODE_FNEG : {
//        const klr_fneg_t* op = (klr_fneg_t*)pc;
//        JIT_ASM_fNEG(ctx, op->a1, op->a2);
//        pc += OPSIZE_FNEG;
//        goto L_HEAD;
//    }
//
//    case OPCODE_FADD : {
//        const klr_fadd_t* op = (klr_fadd_t*)pc;
//        JIT_ASM_fADD(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_FADD;
//        goto L_HEAD;
//    }
//
//    case OPCODE_FADDN : {
//        const klr_faddn_t* op = (klr_faddn_t*)pc;
//        JIT_ASM_fADDn(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_FADDN;
//        goto L_HEAD;
//    }
//
//    case OPCODE_FSUB : {
//        const klr_fsub_t* op = (klr_fsub_t*)pc;
//        JIT_ASM_fSUB(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_FSUB;
//        goto L_HEAD;
//    }
//
//    case OPCODE_FSUBN : {
//        const klr_fsubn_t* op = (klr_fsubn_t*)pc;
//        JIT_ASM_fSUBn(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_FSUBN;
//        goto L_HEAD;
//    }
//
//    case OPCODE_FMUL : {
//        const klr_fmul_t* op = (klr_fmul_t*)pc;
//        JIT_ASM_fMUL(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_FMUL;
//        goto L_HEAD;
//    }
//
//    case OPCODE_FMULN : {
//        const klr_fmuln_t* op = (klr_fmuln_t*)pc;
//        JIT_ASM_fMULn(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_FMULN;
//        goto L_HEAD;
//    }
//
//    case OPCODE_FDIV : {
//        const klr_fdiv_t* op = (klr_fdiv_t*)pc;
//        JIT_ASM_fDIV(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_FDIV;
//        goto L_HEAD;
//    }
//
//    case OPCODE_FDIVN : {
//        const klr_fdivn_t* op = (klr_fdivn_t*)pc;
//        JIT_ASM_fDIVn(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_FDIVN;
//        goto L_HEAD;
//    }
//
//    case OPCODE_FEQ : {
//        const klr_feq_t* op = (klr_feq_t*)pc;
//        JIT_ASM_fEQ(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_FEQ;
//        goto L_HEAD;
//    }
//
//    case OPCODE_FEQN : {
//        const klr_feqn_t* op = (klr_feqn_t*)pc;
//        JIT_ASM_fEQn(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_FEQN;
//        goto L_HEAD;
//    }
//
//    case OPCODE_FNEQ : {
//        const klr_fneq_t* op = (klr_fneq_t*)pc;
//        JIT_ASM_fNEQ(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_FNEQ;
//        goto L_HEAD;
//    }
//
//    case OPCODE_FNEQN : {
//        const klr_fneqn_t* op = (klr_fneqn_t*)pc;
//        JIT_ASM_fNEQn(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_FNEQN;
//        goto L_HEAD;
//    }
//
//    case OPCODE_FLT : {
//        const klr_flt_t* op = (klr_flt_t*)pc;
//        JIT_ASM_fLT(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_FLT;
//        goto L_HEAD;
//    }
//
//    case OPCODE_FLTN : {
//        const klr_fltn_t* op = (klr_fltn_t*)pc;
//        JIT_ASM_fLTn(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_FLTN;
//        goto L_HEAD;
//    }
//
//    case OPCODE_FLTE : {
//        const klr_flte_t* op = (klr_flte_t*)pc;
//        JIT_ASM_fLTE(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_FLTE;
//        goto L_HEAD;
//    }
//
//    case OPCODE_FLTEN : {
//        const klr_flten_t* op = (klr_flten_t*)pc;
//        JIT_ASM_fLTEn(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_FLTEN;
//        goto L_HEAD;
//    }
//
//    case OPCODE_FGT : {
//        const klr_fgt_t* op = (klr_fgt_t*)pc;
//        JIT_ASM_fGT(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_FGT;
//        goto L_HEAD;
//    }
//
//    case OPCODE_FGTN : {
//        const klr_fgtn_t* op = (klr_fgtn_t*)pc;
//        JIT_ASM_fGTn(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_FGTN;
//        goto L_HEAD;
//    }
//
//    case OPCODE_FGTE : {
//        const klr_fgte_t* op = (klr_fgte_t*)pc;
//        JIT_ASM_fGTE(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_FGTE;
//        goto L_HEAD;
//    }
//
//    case OPCODE_FGTEN : {
//        const klr_fgten_t* op = (klr_fgten_t*)pc;
//        JIT_ASM_fGTEn(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_FGTEN;
//        goto L_HEAD;
//    }
//
//    case OPCODE_AGET : {
//        const klr_aget_t* op = (klr_aget_t*)pc;
//        JIT_ASM_AGET(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_AGET;
//        goto L_HEAD;
//    }
//
//    case OPCODE_AGETN : {
//        const klr_agetn_t* op = (klr_agetn_t*)pc;
//        JIT_ASM_AGETn(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_AGETN;
//        goto L_HEAD;
//    }
//
//    case OPCODE_IAGET : {
//        const klr_iaget_t* op = (klr_iaget_t*)pc;
//        JIT_ASM_IAGET(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_IAGET;
//        goto L_HEAD;
//    }
//
//    case OPCODE_IAGETN : {
//        const klr_iagetn_t* op = (klr_iagetn_t*)pc;
//        JIT_ASM_IAGETn(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_IAGETN;
//        goto L_HEAD;
//    }
//
//    case OPCODE_FAGET : {
//        const klr_faget_t* op = (klr_faget_t*)pc;
//        JIT_ASM_FAGET(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_FAGET;
//        goto L_HEAD;
//    }
//
//    case OPCODE_FAGETN : {
//        const klr_fagetn_t* op = (klr_fagetn_t*)pc;
//        JIT_ASM_FAGETn(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_FAGETN;
//        goto L_HEAD;
//    }
//
//    case OPCODE_ASET : {
//        const klr_aset_t* op = (klr_aset_t*)pc;
//        JIT_ASM_ASET(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_ASET;
//        goto L_HEAD;
//    }
//
//    case OPCODE_ASETN : {
//        const klr_asetn_t* op = (klr_asetn_t*)pc;
//        JIT_ASM_ASETn(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_ASETN;
//        goto L_HEAD;
//    }
//
//    case OPCODE_IASET : {
//        const klr_iaset_t* op = (klr_iaset_t*)pc;
//        JIT_ASM_IASET(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_IASET;
//        goto L_HEAD;
//    }
//
//    case OPCODE_IASETN : {
//        const klr_iasetn_t* op = (klr_iasetn_t*)pc;
//        JIT_ASM_IASETn(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_IASETN;
//        goto L_HEAD;
//    }
//
//    case OPCODE_FASET : {
//        const klr_faset_t* op = (klr_faset_t*)pc;
//        JIT_ASM_FASET(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_FASET;
//        goto L_HEAD;
//    }
//
//    case OPCODE_FASETN : {
//        const klr_fasetn_t* op = (klr_fasetn_t*)pc;
//        JIT_ASM_FASETn(ctx, op->a1, op->a2, op->a3);
//        pc += OPSIZE_FASETN;
//        goto L_HEAD;
//    }
//
//    case OPCODE_INITCODE : {
//        const klr_initcode_t* op = (klr_initcode_t*)pc;
//        JIT_ASM_INITCODE(ctx, op->a1);
//        pc += OPSIZE_INITCODE;
//        goto L_HEAD;
//    }
//
//    case OPCODE_NOP : {
//        const klr_nop_t* op = (klr_nop_t*)pc;
//        JIT_ASM_NOP(ctx);
//        pc += OPSIZE_NOP;
//        goto L_HEAD;
//    }
//
//    }
//    KNH_WARNING(ctx, "unknown opcode=%d", ((knh_kode_t*)pc)->opcode);
//    goto L_HEAD;
//
//CTT_FIN:
//    {
//        jit_complete(mem);
//        knh_fmethod f = (METHOD (*) (Ctx *, knh_sfp_t *)) mem->addr;
//        knh_ctt_patch(mem);
//        imem_complete(mem);
//        knh_Method_syncFunc(mtd,f);
//    }
//}
//
///* ------------------------------------------------------------------------ */
//
//static void knh_ctt_patch(imem_t* mem)
//{
//    lstack_t* head = mem->lhead;
//    lstack_t* stack = NULL;
//
//    imem_setLabel(mem, 255, 255, 255);
//
//    for(stack = head;stack->opcode != 255; stack += 1) {
//        /*
//        fprintf(stderr, "pos=%3x,opcode=%3d,opsize=%d,jmpaddr=%d\n",
//                stack->pos,stack->opcode,stack->size,stack->addr);
//                */
//        if(stack->addr > 0) {
//            lstack_t* _s = NULL;
//            for(_s = stack; _s->opcode != 255; _s += 1) {
//                stack->addr -= _s->size;
//                //fprintf(stderr, "%d\n",stack->addr);
//                /*
//                fprintf(stderr, "\tpos=%3x,opcode=%3x,opsize=%3x,\n",
//                        _s->pos,_s->opcode,_s->size);
//                */
//                if(stack->addr <= 0) {
//                    _s = _s + 1;
//                    /*
//                    fprintf(stderr, "ok ps=%x,sp=%x l=%x\n",_s->pos,
//                            stack->pos, _s->pos - stack->pos - 2);
//                    fprintf(stderr, "p=%x\n",mem->addr[stack->pos + 1]);
//                    */
//                    mem->addr[stack->pos+1] = _s->pos - stack->pos - 2;
//                    /*
//                    fprintf(stderr, "p=%x\n",mem->addr[stack->pos + 1]);
//                    */
//                    break;
//                }
//            }
//        }
//    }
//}
//
///* ------------------------------------------------------------------------ */
//
//
//#ifdef __cplusplus
//}
//#endif
