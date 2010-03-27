#include "debug.h"

METHOD Method_setDebugMode(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Method_t *mtd = sfp[0].mtd;
	IDEBUG("mtd=%p",mtd);
	//knh_code_t *pc = (mtd)->pc_start;
	//code_init(pc);
	if(mtd->fcall_1 == knh_KLRCode_exec) {
		(mtd)->fcall_1 = System_debug;
	} else {
		knh_printf(ctx, ctx->err, "Native Method can not debug\n");
	}
	KNH_RETURN_void(ctx, sfp);
}

void print_break(Ctx *ctx, knh_Method_t *mtd, knh_OutputStream_t *out)
{
	knh_code_t *pc = mtd->pc_start;
	char *file = knh_Method_file(ctx, mtd);
	int line   = knh_Method_pcline(mtd, pc);
	IDEBUG("file=%s,line=%d",file,line);
	knh_printf(ctx, out, "BreakPoing: ");
	knh_format(ctx, out, METHODN__k, UP(mtd), KNH_NULL);
	knh_printf(ctx, out, " at %s:%d\n",file, line);
}

static struct op opr[] = {
	{"null"     , 4, 1, op_null},
	{"next"     , 4, 1, op_next},
	{"n"        , 1, 1, op_next},
	{"print"    , 5, 0, op_print},
	{"p"        , 1, 0, op_print},
	{"bt"       , 2, 0, op_bt},
	{"backtrace", 9, 0, op_bt},
};

static fopr_t parse_opr(knh_bytes_t cmd)
{
    int i;
    for (i = 0; i < OPR_SIZE; i++) {
        opr_t *o = &opr[i];
        knh_bytes_t oname = B2(o->name, o->len);
        if (knh_bytes_strcmp(oname, cmd) == 0) {
            return o->fopr;
        }
    }
    return opr[0].fopr;
}


int cmd(Ctx *ctx, knh_sfp_t *sfp, knh_OutputStream_t *out, char *fmt)
{
	IDEBUG("fmt=%s",fmt);
	if(fmt == NULL) {
		return 1;
	}
	knh_bytes_t b = B(fmt);
	knh_bytes_t sp = B(" ");
	int loc = knh_bytes_indexOf(b, sp);
	knh_bytes_t c;
	if(loc == -1) {
		c = b;
	}
	else {
		c = knh_bytes_first(b, loc);
	}
	fopr_t func = parse_opr(c);
	return func(ctx, sfp, out, b);
}

#define OPJUMP NULL
#define CASE(x)   case OPCODE_##x :
#define NEXT_OP L_HEAD
#define JUMP L_HEAD
#define TC(c)    
#define DISPATCH_START(pc) switch(KNH_OPCODE(pc)) {
#define DISPATCH_END(pc) 	}	KNH_WARNING(ctx, "unknown opcode=%d", KNH_OPCODE(pc)); 	goto L_HEAD;
char *kdb_readline(void);
void kdb_write(Ctx *ctx, knh_OutputStream_t *out);

METHOD System_debug(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Method_t *mtd = sfp[-1].mtd;
	IDEBUG("mtd=%p",mtd);
	register knh_code_t *pc = mtd->pc_start;
	char *ln;
	knh_OutputStream_t *out;
#ifdef KONOHA_ON_LKM
	knh_Bytes_t *ba;
	KNH_INITv(ba, new_Bytes(ctx, KNH_BUF_MAX));
	KNH_INITv(out, new_BytesOutputStream(ctx, ba));
#endif
	print_break(ctx, mtd, out);
L_HEAD:;
	knh_inst_t *inst = (knh_inst_t *) pc;
#ifdef KONOHA_ON_LKM
	if(out == NULL) {
		KNH_INITv(ba, new_Bytes(ctx, KNH_BUF_MAX));
		KNH_INITv(out, new_BytesOutputStream(ctx, ba));
	}
#else
	KNH_INITv(out, ctx->out);
#endif
	knh_printf(ctx, out, "%d:\t ", knh_Method_pcline(mtd, pc));
	knh_opcode_dump(ctx, inst, out);
	knh_write_EOL(ctx, out);
#ifdef KONOHA_ON_LKM
	kdb_write(ctx, out);
	ln = kdb_readline();
#else
	ln = knh_readline("(kdb) ");
#endif
	int ret = cmd(ctx, sfp, out, ln);
	if((ret == 0) && out) {
		//printk("%s:%d ba='%s'\n",__func__, __LINE__, ba->buf);
		KNH_FINALv(ctx, out);
		KNH_FINALv(ctx, ba);
	}
	if(ret) {
		goto L_HEAD;
	}
	DISPATCH_START(pc);

	CASE(HALT) {
		TC(const klr_HALT_t *op = (klr_HALT_t*)pc;)
		KLR_HALT(ctx);
		pc += OPSIZE_HALT;
		goto NEXT_OP;
	} 
	CASE(MOVa) {
		const klr_MOVa_t *op = (klr_MOVa_t*)pc;
		KLR_MOVa(ctx, op->a1, op->a2);
		pc += OPSIZE_MOVa;
		goto NEXT_OP;
	} 
	CASE(MOVn) {
		const klr_MOVn_t *op = (klr_MOVn_t*)pc;
		KLR_MOVn(ctx, op->a1, op->a2);
		pc += OPSIZE_MOVn;
		goto NEXT_OP;
	} 
	CASE(MOVo) {
		const klr_MOVo_t *op = (klr_MOVo_t*)pc;
		KLR_MOVo(ctx, op->a1, op->a2);
		pc += OPSIZE_MOVo;
		goto NEXT_OP;
	} 
	CASE(MOVi) {
		const klr_MOVi_t *op = (klr_MOVi_t*)pc;
		KLR_MOVi(ctx, op->a1, op->a2);
		pc += OPSIZE_MOVi;
		goto NEXT_OP;
	} 
	CASE(MOVx) {
		const klr_MOVx_t *op = (klr_MOVx_t*)pc;
		KLR_MOVx(ctx, op->a1, op->a2);
		pc += OPSIZE_MOVx;
		goto NEXT_OP;
	} 
	CASE(MOVDEF) {
		const klr_MOVDEF_t *op = (klr_MOVDEF_t*)pc;
		KLR_MOVDEF(ctx, op->a1, op->a2);
		pc += OPSIZE_MOVDEF;
		goto NEXT_OP;
	} 
	CASE(MOVSYS) {
		const klr_MOVSYS_t *op = (klr_MOVSYS_t*)pc;
		KLR_MOVSYS(ctx, op->a1, op->a2);
		pc += OPSIZE_MOVSYS;
		goto NEXT_OP;
	} 
	CASE(MOVxi) {
		const klr_MOVxi_t *op = (klr_MOVxi_t*)pc;
		KLR_MOVxi(ctx, op->a1, op->a2);
		pc += OPSIZE_MOVxi;
		goto NEXT_OP;
	} 
	CASE(MOVxf) {
		const klr_MOVxf_t *op = (klr_MOVxf_t*)pc;
		KLR_MOVxf(ctx, op->a1, op->a2);
		pc += OPSIZE_MOVxf;
		goto NEXT_OP;
	} 
	CASE(MOVxb) {
		const klr_MOVxb_t *op = (klr_MOVxb_t*)pc;
		KLR_MOVxb(ctx, op->a1, op->a2);
		pc += OPSIZE_MOVxb;
		goto NEXT_OP;
	} 
	CASE(MOVe) {
		const klr_MOVe_t *op = (klr_MOVe_t*)pc;
		KLR_MOVe(ctx, op->a1, op->a2);
		pc += OPSIZE_MOVe;
		goto NEXT_OP;
	} 
	CASE(XMOVs) {
		const klr_XMOVs_t *op = (klr_XMOVs_t*)pc;
		KLR_XMOVs(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVs;
		goto NEXT_OP;
	} 
	CASE(XMOVo) {
		const klr_XMOVo_t *op = (klr_XMOVo_t*)pc;
		KLR_XMOVo(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVo;
		goto NEXT_OP;
	} 
	CASE(XMOVx) {
		const klr_XMOVx_t *op = (klr_XMOVx_t*)pc;
		KLR_XMOVx(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVx;
		goto NEXT_OP;
	} 
	CASE(XMOVDEF) {
		const klr_XMOVDEF_t *op = (klr_XMOVDEF_t*)pc;
		KLR_XMOVDEF(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVDEF;
		goto NEXT_OP;
	} 
	CASE(XMOVSYS) {
		const klr_XMOVSYS_t *op = (klr_XMOVSYS_t*)pc;
		KLR_XMOVSYS(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVSYS;
		goto NEXT_OP;
	} 
	CASE(XMOVsi) {
		const klr_XMOVsi_t *op = (klr_XMOVsi_t*)pc;
		KLR_XMOVsi(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVsi;
		goto NEXT_OP;
	} 
	CASE(XMOVsf) {
		const klr_XMOVsf_t *op = (klr_XMOVsf_t*)pc;
		KLR_XMOVsf(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVsf;
		goto NEXT_OP;
	} 
	CASE(XMOVsb) {
		const klr_XMOVsb_t *op = (klr_XMOVsb_t*)pc;
		KLR_XMOVsb(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVsb;
		goto NEXT_OP;
	} 
	CASE(SWAP) {
		const klr_SWAP_t *op = (klr_SWAP_t*)pc;
		KLR_SWAP(ctx, op->a1, op->a2);
		pc += OPSIZE_SWAP;
		goto NEXT_OP;
	} 
	CASE(PARAMDEF) {
		const klr_PARAMDEF_t *op = (klr_PARAMDEF_t*)pc;
		KLR_PARAMDEF(ctx, op->a1, op->a2);
		pc += OPSIZE_PARAMDEF;
		goto NEXT_OP;
	} 
	CASE(PARAMo) {
		const klr_PARAMo_t *op = (klr_PARAMo_t*)pc;
		KLR_PARAMo(ctx, op->a1, op->a2);
		pc += OPSIZE_PARAMo;
		goto NEXT_OP;
	} 
	CASE(PARAMPROP) {
		const klr_PARAMPROP_t *op = (klr_PARAMPROP_t*)pc;
		KLR_PARAMPROP(ctx, op->a1, op->a2);
		pc += OPSIZE_PARAMPROP;
		goto NEXT_OP;
	} 
	CASE(PARAMS) {
		const klr_PARAMS_t *op = (klr_PARAMS_t*)pc;
		KLR_PARAMS(ctx, op->a1, op->a2);
		pc += OPSIZE_PARAMS;
		goto NEXT_OP;
	} 
	CASE(CHKESP) {
		//const klr_CHKESP_t *op = (klr_CHKESP_t*)pc;
		KLR_CHKESP(ctx, op->a1);
		pc += OPSIZE_CHKESP;
		goto NEXT_OP;
	} 
	CASE(RET) {
		TC(const klr_RET_t *op = (klr_RET_t*)pc;)
		KLR_RET(ctx);
		pc += OPSIZE_RET;
		goto NEXT_OP;
	} 
	CASE(YEILDBREAK) {
		TC(const klr_YEILDBREAK_t *op = (klr_YEILDBREAK_t*)pc;)
		KLR_YEILDBREAK(ctx);
		pc += OPSIZE_YEILDBREAK;
		goto NEXT_OP;
	} 
	CASE(BOX) {
		const klr_BOX_t *op = (klr_BOX_t*)pc;
		KLR_BOX(ctx, op->a1, op->a2);
		pc += OPSIZE_BOX;
		goto NEXT_OP;
	} 
	CASE(BOXnc) {
		const klr_BOXnc_t *op = (klr_BOXnc_t*)pc;
		KLR_BOXnc(ctx, op->a1, op->a2);
		pc += OPSIZE_BOXnc;
		goto NEXT_OP;
	} 
	CASE(NNBOX) {
		const klr_NNBOX_t *op = (klr_NNBOX_t*)pc;
		KLR_NNBOX(ctx, op->a1, op->a2);
		pc += OPSIZE_NNBOX;
		goto NEXT_OP;
	} 
	CASE(NNBOXnc) {
		const klr_NNBOXnc_t *op = (klr_NNBOXnc_t*)pc;
		KLR_NNBOXnc(ctx, op->a1, op->a2);
		pc += OPSIZE_NNBOXnc;
		goto NEXT_OP;
	} 
	CASE(UNBOX) {
		const klr_UNBOX_t *op = (klr_UNBOX_t*)pc;
		KLR_UNBOX(ctx, op->a1);
		pc += OPSIZE_UNBOX;
		goto NEXT_OP;
	} 
	CASE(CHKNUL) {
		const klr_CHKNUL_t *op = (klr_CHKNUL_t*)pc;
		KLR_CHKNUL(ctx, op->a1);
		pc += OPSIZE_CHKNUL;
		goto NEXT_OP;
	} 
	CASE(CHKNULx) {
		const klr_CHKNULx_t *op = (klr_CHKNULx_t*)pc;
		KLR_CHKNULx(ctx, op->a1);
		pc += OPSIZE_CHKNULx;
		goto NEXT_OP;
	} 
	CASE(CHKTYPE) {
		const klr_CHKTYPE_t *op = (klr_CHKTYPE_t*)pc;
		KLR_CHKTYPE(ctx, op->a1, op->a2);
		pc += OPSIZE_CHKTYPE;
		goto NEXT_OP;
	} 
	CASE(FCALL) {
		const klr_FCALL_t *op = (klr_FCALL_t*)pc;
		KLR_FCALL(ctx, op->a1, op->a2, op->a3, op->a4);
		pc += OPSIZE_FCALL;
		goto NEXT_OP;
	} 
	CASE(SCALL) {
		const klr_SCALL_t *op = (klr_SCALL_t*)pc;
		KLR_SCALL(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_SCALL;
		goto NEXT_OP;
	} 
	CASE(AINVOKE) {
		const klr_AINVOKE_t *op = (klr_AINVOKE_t*)pc;
		KLR_AINVOKE(ctx, op->a1, op->a2);
		pc += OPSIZE_AINVOKE;
		goto NEXT_OP;
	} 
	CASE(CALL) {
		const klr_CALL_t *op = (klr_CALL_t*)pc;
		KLR_CALL(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_CALL;
		goto NEXT_OP;
	} 
	CASE(ACALL) {
		const klr_ACALL_t *op = (klr_ACALL_t*)pc;
		KLR_ACALL(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_ACALL;
		goto NEXT_OP;
	} 
	CASE(NEW) {
		const klr_NEW_t *op = (klr_NEW_t*)pc;
		KLR_NEW(ctx, op->a1, op->a2, op->a3, op->a4, op->a5);
		pc += OPSIZE_NEW;
		goto NEXT_OP;
	} 
	CASE(COPYSFP) {
		const klr_COPYSFP_t *op = (klr_COPYSFP_t*)pc;
		KLR_COPYSFP(ctx, op->a1);
		pc += OPSIZE_COPYSFP;
		goto NEXT_OP;
	} 
	CASE(STR) {
		const klr_STR_t *op = (klr_STR_t*)pc;
		KLR_STR(ctx, op->a1, op->a2, op->a3, op->a4);
		pc += OPSIZE_STR;
		goto NEXT_OP;
	} 
	CASE(SSTR) {
		const klr_SSTR_t *op = (klr_SSTR_t*)pc;
		KLR_SSTR(ctx, op->a1, op->a2, op->a3, op->a4);
		pc += OPSIZE_SSTR;
		goto NEXT_OP;
	} 
	CASE(SMAP) {
		const klr_SMAP_t *op = (klr_SMAP_t*)pc;
		KLR_SMAP(ctx, op->a1, op->a2);
		pc += OPSIZE_SMAP;
		goto NEXT_OP;
	} 
	CASE(SMAPnc) {
		const klr_SMAPnc_t *op = (klr_SMAPnc_t*)pc;
		KLR_SMAPnc(ctx, op->a1, op->a2);
		pc += OPSIZE_SMAPnc;
		goto NEXT_OP;
	} 
	CASE(MAP) {
		const klr_MAP_t *op = (klr_MAP_t*)pc;
		KLR_MAP(ctx, op->a1, op->a2);
		pc += OPSIZE_MAP;
		goto NEXT_OP;
	} 
	CASE(MAPnc) {
		const klr_MAPnc_t *op = (klr_MAPnc_t*)pc;
		KLR_MAPnc(ctx, op->a1, op->a2);
		pc += OPSIZE_MAPnc;
		goto NEXT_OP;
	} 
	CASE(AMAP) {
		const klr_AMAP_t *op = (klr_AMAP_t*)pc;
		KLR_AMAP(ctx, op->a1, op->a2);
		pc += OPSIZE_AMAP;
		goto NEXT_OP;
	} 
	CASE(NNMAP) {
		const klr_NNMAP_t *op = (klr_NNMAP_t*)pc;
		KLR_NNMAP(ctx, op->a1, op->a2);
		pc += OPSIZE_NNMAP;
		goto NEXT_OP;
	} 
	CASE(JMP) {
		const klr_JMP_t *op = (klr_JMP_t*)pc;
		KLR_JMP(ctx, pc , ((knh_inst_t*)op)->jumppc, JUMP);
		pc += OPSIZE_JMP;
		goto NEXT_OP;
	} 
	CASE(SKIP) {
		const klr_SKIP_t *op = (klr_SKIP_t*)pc;
		KLR_SKIP(ctx, pc , ((knh_inst_t*)op)->jumppc, JUMP);
		pc += OPSIZE_SKIP;
		goto NEXT_OP;
	} 
	CASE(bJIFT) {
		const klr_bJIFT_t *op = (klr_bJIFT_t*)pc;
		KLR_bJIFT(ctx, pc , ((knh_inst_t*)op)->jumppc, JUMP, op->a2);
		pc += OPSIZE_bJIFT;
		goto NEXT_OP;
	} 
	CASE(bJIFF) {
		const klr_bJIFF_t *op = (klr_bJIFF_t*)pc;
		KLR_bJIFF(ctx, pc , ((knh_inst_t*)op)->jumppc, JUMP, op->a2);
		pc += OPSIZE_bJIFF;
		goto NEXT_OP;
	} 
	CASE(bJIFF_LOOP) {
		const klr_bJIFF_LOOP_t *op = (klr_bJIFF_LOOP_t*)pc;
		KLR_bJIFF_LOOP(ctx, pc , ((knh_inst_t*)op)->jumppc, JUMP, op->a2);
		pc += OPSIZE_bJIFF_LOOP;
		goto NEXT_OP;
	} 
	CASE(JIFNUL) {
		const klr_JIFNUL_t *op = (klr_JIFNUL_t*)pc;
		KLR_JIFNUL(ctx, pc , ((knh_inst_t*)op)->jumppc, JUMP, op->a2);
		pc += OPSIZE_JIFNUL;
		goto NEXT_OP;
	} 
	CASE(JIFNN) {
		const klr_JIFNN_t *op = (klr_JIFNN_t*)pc;
		KLR_JIFNN(ctx, pc , ((knh_inst_t*)op)->jumppc, JUMP, op->a2);
		pc += OPSIZE_JIFNN;
		goto NEXT_OP;
	} 
	CASE(NEXT) {
		const klr_NEXT_t *op = (klr_NEXT_t*)pc;
		KLR_NEXT(ctx, pc , ((knh_inst_t*)op)->jumppc, JUMP, op->a2, op->a3);
		pc += OPSIZE_NEXT;
		goto NEXT_OP;
	} 
	CASE(INEXT) {
		const klr_INEXT_t *op = (klr_INEXT_t*)pc;
		KLR_INEXT(ctx, pc , ((knh_inst_t*)op)->jumppc, JUMP, op->a2, op->a3, op->a4);
		pc += OPSIZE_INEXT;
		goto NEXT_OP;
	} 
	CASE(TRY) {
		const klr_TRY_t *op = (klr_TRY_t*)pc;
		KLR_TRY(ctx, pc , ((knh_inst_t*)op)->jumppc, JUMP, op->a2);
		pc += OPSIZE_TRY;
		goto NEXT_OP;
	} 
	CASE(TRYEND) {
		const klr_TRYEND_t *op = (klr_TRYEND_t*)pc;
		KLR_TRYEND(ctx, op->a1);
		pc += OPSIZE_TRYEND;
		goto NEXT_OP;
	} 
	CASE(CATCH) {
		const klr_CATCH_t *op = (klr_CATCH_t*)pc;
		KLR_CATCH(ctx, pc , ((knh_inst_t*)op)->jumppc, JUMP, op->a2, op->a3, op->a4);
		pc += OPSIZE_CATCH;
		goto NEXT_OP;
	} 
	CASE(PUSH) {
		const klr_PUSH_t *op = (klr_PUSH_t*)pc;
		KLR_PUSH(ctx, op->a1);
		pc += OPSIZE_PUSH;
		goto NEXT_OP;
	} 
	CASE(POP) {
		const klr_POP_t *op = (klr_POP_t*)pc;
		KLR_POP(ctx, op->a1);
		pc += OPSIZE_POP;
		goto NEXT_OP;
	} 
	CASE(THROW) {
		const klr_THROW_t *op = (klr_THROW_t*)pc;
		KLR_THROW(ctx, op->a1, op->a2);
		pc += OPSIZE_THROW;
		goto NEXT_OP;
	} 
	CASE(THROWs) {
		const klr_THROWs_t *op = (klr_THROWs_t*)pc;
		KLR_THROWs(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_THROWs;
		goto NEXT_OP;
	} 
	CASE(THROW_AGAIN) {
		const klr_THROW_AGAIN_t *op = (klr_THROW_AGAIN_t*)pc;
		KLR_THROW_AGAIN(ctx, op->a1);
		pc += OPSIZE_THROW_AGAIN;
		goto NEXT_OP;
	} 
	CASE(P) {
		const klr_P_t *op = (klr_P_t*)pc;
		KLR_P(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_P;
		goto NEXT_OP;
	} 
	CASE(PMSG) {
		const klr_PMSG_t *op = (klr_PMSG_t*)pc;
		KLR_PMSG(ctx, op->a1, op->a2);
		pc += OPSIZE_PMSG;
		goto NEXT_OP;
	} 
	CASE(iCAST) {
		const klr_iCAST_t *op = (klr_iCAST_t*)pc;
		KLR_iCAST(ctx, op->a1);
		pc += OPSIZE_iCAST;
		goto NEXT_OP;
	} 
	CASE(inCAST) {
		const klr_inCAST_t *op = (klr_inCAST_t*)pc;
		KLR_inCAST(ctx, op->a1);
		pc += OPSIZE_inCAST;
		goto NEXT_OP;
	} 
	CASE(fCAST) {
		const klr_fCAST_t *op = (klr_fCAST_t*)pc;
		KLR_fCAST(ctx, op->a1);
		pc += OPSIZE_fCAST;
		goto NEXT_OP;
	} 
	CASE(fnCAST) {
		const klr_fnCAST_t *op = (klr_fnCAST_t*)pc;
		KLR_fnCAST(ctx, op->a1);
		pc += OPSIZE_fnCAST;
		goto NEXT_OP;
	} 
	CASE(bNOT) {
		const klr_bNOT_t *op = (klr_bNOT_t*)pc;
		KLR_bNOT(ctx, op->a1, op->a2);
		pc += OPSIZE_bNOT;
		goto NEXT_OP;
	} 
	CASE(iNEG) {
		const klr_iNEG_t *op = (klr_iNEG_t*)pc;
		KLR_iNEG(ctx, op->a1, op->a2);
		pc += OPSIZE_iNEG;
		goto NEXT_OP;
	} 
	CASE(iADD) {
		const klr_iADD_t *op = (klr_iADD_t*)pc;
		KLR_iADD(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iADD;
		goto NEXT_OP;
	} 
	CASE(iADDn) {
		const klr_iADDn_t *op = (klr_iADDn_t*)pc;
		KLR_iADDn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iADDn;
		goto NEXT_OP;
	} 
	CASE(iSUB) {
		const klr_iSUB_t *op = (klr_iSUB_t*)pc;
		KLR_iSUB(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iSUB;
		goto NEXT_OP;
	} 
	CASE(iSUBn) {
		const klr_iSUBn_t *op = (klr_iSUBn_t*)pc;
		KLR_iSUBn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iSUBn;
		goto NEXT_OP;
	} 
	CASE(iMUL) {
		const klr_iMUL_t *op = (klr_iMUL_t*)pc;
		KLR_iMUL(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iMUL;
		goto NEXT_OP;
	} 
	CASE(iMULn) {
		const klr_iMULn_t *op = (klr_iMULn_t*)pc;
		KLR_iMULn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iMULn;
		goto NEXT_OP;
	} 
	CASE(iDIV) {
		const klr_iDIV_t *op = (klr_iDIV_t*)pc;
		KLR_iDIV(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iDIV;
		goto NEXT_OP;
	} 
	CASE(iDIVn) {
		const klr_iDIVn_t *op = (klr_iDIVn_t*)pc;
		KLR_iDIVn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iDIVn;
		goto NEXT_OP;
	} 
	CASE(iMOD) {
		const klr_iMOD_t *op = (klr_iMOD_t*)pc;
		KLR_iMOD(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iMOD;
		goto NEXT_OP;
	} 
	CASE(iMODn) {
		const klr_iMODn_t *op = (klr_iMODn_t*)pc;
		KLR_iMODn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iMODn;
		goto NEXT_OP;
	} 
	CASE(iEQ) {
		const klr_iEQ_t *op = (klr_iEQ_t*)pc;
		KLR_iEQ(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iEQ;
		goto NEXT_OP;
	} 
	CASE(iEQn) {
		const klr_iEQn_t *op = (klr_iEQn_t*)pc;
		KLR_iEQn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iEQn;
		goto NEXT_OP;
	} 
	CASE(iNEQ) {
		const klr_iNEQ_t *op = (klr_iNEQ_t*)pc;
		KLR_iNEQ(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iNEQ;
		goto NEXT_OP;
	} 
	CASE(iNEQn) {
		const klr_iNEQn_t *op = (klr_iNEQn_t*)pc;
		KLR_iNEQn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iNEQn;
		goto NEXT_OP;
	} 
	CASE(iLT) {
		const klr_iLT_t *op = (klr_iLT_t*)pc;
		KLR_iLT(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iLT;
		goto NEXT_OP;
	} 
	CASE(iLTn) {
		const klr_iLTn_t *op = (klr_iLTn_t*)pc;
		KLR_iLTn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iLTn;
		goto NEXT_OP;
	} 
	CASE(iLTE) {
		const klr_iLTE_t *op = (klr_iLTE_t*)pc;
		KLR_iLTE(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iLTE;
		goto NEXT_OP;
	} 
	CASE(iLTEn) {
		const klr_iLTEn_t *op = (klr_iLTEn_t*)pc;
		KLR_iLTEn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iLTEn;
		goto NEXT_OP;
	} 
	CASE(iGT) {
		const klr_iGT_t *op = (klr_iGT_t*)pc;
		KLR_iGT(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iGT;
		goto NEXT_OP;
	} 
	CASE(iGTn) {
		const klr_iGTn_t *op = (klr_iGTn_t*)pc;
		KLR_iGTn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iGTn;
		goto NEXT_OP;
	} 
	CASE(iGTE) {
		const klr_iGTE_t *op = (klr_iGTE_t*)pc;
		KLR_iGTE(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iGTE;
		goto NEXT_OP;
	} 
	CASE(iGTEn) {
		const klr_iGTEn_t *op = (klr_iGTEn_t*)pc;
		KLR_iGTEn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iGTEn;
		goto NEXT_OP;
	} 
	CASE(fNEG) {
		const klr_fNEG_t *op = (klr_fNEG_t*)pc;
		KLR_fNEG(ctx, op->a1, op->a2);
		pc += OPSIZE_fNEG;
		goto NEXT_OP;
	} 
	CASE(fADD) {
		const klr_fADD_t *op = (klr_fADD_t*)pc;
		KLR_fADD(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fADD;
		goto NEXT_OP;
	} 
	CASE(fADDn) {
		const klr_fADDn_t *op = (klr_fADDn_t*)pc;
		KLR_fADDn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fADDn;
		goto NEXT_OP;
	} 
	CASE(fSUB) {
		const klr_fSUB_t *op = (klr_fSUB_t*)pc;
		KLR_fSUB(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fSUB;
		goto NEXT_OP;
	} 
	CASE(fSUBn) {
		const klr_fSUBn_t *op = (klr_fSUBn_t*)pc;
		KLR_fSUBn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fSUBn;
		goto NEXT_OP;
	} 
	CASE(fMUL) {
		const klr_fMUL_t *op = (klr_fMUL_t*)pc;
		KLR_fMUL(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fMUL;
		goto NEXT_OP;
	} 
	CASE(fMULn) {
		const klr_fMULn_t *op = (klr_fMULn_t*)pc;
		KLR_fMULn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fMULn;
		goto NEXT_OP;
	} 
	CASE(fDIV) {
		const klr_fDIV_t *op = (klr_fDIV_t*)pc;
		KLR_fDIV(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fDIV;
		goto NEXT_OP;
	} 
	CASE(fDIVn) {
		const klr_fDIVn_t *op = (klr_fDIVn_t*)pc;
		KLR_fDIVn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fDIVn;
		goto NEXT_OP;
	} 
	CASE(fEQ) {
		const klr_fEQ_t *op = (klr_fEQ_t*)pc;
		KLR_fEQ(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fEQ;
		goto NEXT_OP;
	} 
	CASE(fEQn) {
		const klr_fEQn_t *op = (klr_fEQn_t*)pc;
		KLR_fEQn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fEQn;
		goto NEXT_OP;
	} 
	CASE(fNEQ) {
		const klr_fNEQ_t *op = (klr_fNEQ_t*)pc;
		KLR_fNEQ(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fNEQ;
		goto NEXT_OP;
	} 
	CASE(fNEQn) {
		const klr_fNEQn_t *op = (klr_fNEQn_t*)pc;
		KLR_fNEQn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fNEQn;
		goto NEXT_OP;
	} 
	CASE(fLT) {
		const klr_fLT_t *op = (klr_fLT_t*)pc;
		KLR_fLT(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fLT;
		goto NEXT_OP;
	} 
	CASE(fLTn) {
		const klr_fLTn_t *op = (klr_fLTn_t*)pc;
		KLR_fLTn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fLTn;
		goto NEXT_OP;
	} 
	CASE(fLTE) {
		const klr_fLTE_t *op = (klr_fLTE_t*)pc;
		KLR_fLTE(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fLTE;
		goto NEXT_OP;
	} 
	CASE(fLTEn) {
		const klr_fLTEn_t *op = (klr_fLTEn_t*)pc;
		KLR_fLTEn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fLTEn;
		goto NEXT_OP;
	} 
	CASE(fGT) {
		const klr_fGT_t *op = (klr_fGT_t*)pc;
		KLR_fGT(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fGT;
		goto NEXT_OP;
	} 
	CASE(fGTn) {
		const klr_fGTn_t *op = (klr_fGTn_t*)pc;
		KLR_fGTn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fGTn;
		goto NEXT_OP;
	} 
	CASE(fGTE) {
		const klr_fGTE_t *op = (klr_fGTE_t*)pc;
		KLR_fGTE(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fGTE;
		goto NEXT_OP;
	} 
	CASE(fGTEn) {
		const klr_fGTEn_t *op = (klr_fGTEn_t*)pc;
		KLR_fGTEn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fGTEn;
		goto NEXT_OP;
	} 
	CASE(SIZE) {
		const klr_SIZE_t *op = (klr_SIZE_t*)pc;
		KLR_SIZE(ctx, op->a1, op->a2);
		pc += OPSIZE_SIZE;
		goto NEXT_OP;
	} 
	CASE(ARYGET) {
		const klr_ARYGET_t *op = (klr_ARYGET_t*)pc;
		KLR_ARYGET(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_ARYGET;
		goto NEXT_OP;
	} 
	CASE(ARYGETn) {
		const klr_ARYGETn_t *op = (klr_ARYGETn_t*)pc;
		KLR_ARYGETn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_ARYGETn;
		goto NEXT_OP;
	} 
	CASE(iARYGET) {
		const klr_iARYGET_t *op = (klr_iARYGET_t*)pc;
		KLR_iARYGET(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iARYGET;
		goto NEXT_OP;
	} 
	CASE(iARYGETn) {
		const klr_iARYGETn_t *op = (klr_iARYGETn_t*)pc;
		KLR_iARYGETn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iARYGETn;
		goto NEXT_OP;
	} 
	CASE(fARYGET) {
		const klr_fARYGET_t *op = (klr_fARYGET_t*)pc;
		KLR_fARYGET(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fARYGET;
		goto NEXT_OP;
	} 
	CASE(fARYGETn) {
		const klr_fARYGETn_t *op = (klr_fARYGETn_t*)pc;
		KLR_fARYGETn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fARYGETn;
		goto NEXT_OP;
	} 
	CASE(ARYSET) {
		const klr_ARYSET_t *op = (klr_ARYSET_t*)pc;
		KLR_ARYSET(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_ARYSET;
		goto NEXT_OP;
	} 
	CASE(ARYSETn) {
		const klr_ARYSETn_t *op = (klr_ARYSETn_t*)pc;
		KLR_ARYSETn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_ARYSETn;
		goto NEXT_OP;
	} 
	CASE(iARYSET) {
		const klr_iARYSET_t *op = (klr_iARYSET_t*)pc;
		KLR_iARYSET(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iARYSET;
		goto NEXT_OP;
	} 
	CASE(iARYSETn) {
		const klr_iARYSETn_t *op = (klr_iARYSETn_t*)pc;
		KLR_iARYSETn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iARYSETn;
		goto NEXT_OP;
	} 
	CASE(fARYSET) {
		const klr_fARYSET_t *op = (klr_fARYSET_t*)pc;
		KLR_fARYSET(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fARYSET;
		goto NEXT_OP;
	} 
	CASE(fARYSETn) {
		const klr_fARYSETn_t *op = (klr_fARYSETn_t*)pc;
		KLR_fARYSETn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fARYSETn;
		goto NEXT_OP;
	} 
	CASE(THCODE) {
		//const klr_THCODE_t *op = (klr_THCODE_t*)pc;
		//KLR_THCODE(ctx, op->a1);
		pc += OPSIZE_THCODE;
		goto NEXT_OP;
	} 
	CASE(NOP) {
		TC(const klr_NOP_t *op = (klr_NOP_t*)pc;)
		KLR_NOP(ctx);
		pc += OPSIZE_NOP;
		goto NEXT_OP;
	} 
	DISPATCH_END(pc);
}

