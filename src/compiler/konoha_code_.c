/****************************************************************************
 * KONOHA COPYRIGHT, LICENSE NOTICE, AND DISCRIMER  
 * 
 * Copyright (c) 2005-2009, Kimio Kuramitsu <kimio at ynu.ac.jp>
 *           (c) 2008-      Konoha Software Foundation  
 * All rights reserved.
 * 
 * You may choose one of the following two licenses when you use konoha. 
 * See www.konohaware.org/license.html for further information.
 * 
 * (1) GNU General Public License 2.0      (with    KONOHA_UNDER_GPL2)
 * (2) Konoha Software Foundation License 1.0
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER 
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *  
 ****************************************************************************/

#include"commons.h"


#ifdef __cplusplus 
extern "C" {
#endif

/* ======================================================================== */

static size_t OPSIZE[] = {
	OPSIZE_HALT,
	OPSIZE_MOVA,
	OPSIZE_MOVN,
	OPSIZE_MOVO,
	OPSIZE_MOVX,
	OPSIZE_MOVDEF,
	OPSIZE_MOVSYS,
	OPSIZE_XMOVS,
	OPSIZE_XMOVO,
	OPSIZE_XMOVX,
	OPSIZE_XMOVDEF,
	OPSIZE_XMOVSYS,
	OPSIZE_MOVXI,
	OPSIZE_XMOVSI,
	OPSIZE_XMOVOI,
	OPSIZE_XMOVXI,
	OPSIZE_XMOVXIO,
	OPSIZE_XMOVXBXI,
	OPSIZE_MOVXF,
	OPSIZE_XMOVSF,
	OPSIZE_XMOVOF,
	OPSIZE_XMOVXF,
	OPSIZE_XMOVXFO,
	OPSIZE_XMOVXBXF,
	OPSIZE_MOVXB,
	OPSIZE_XMOVSB,
	OPSIZE_XMOVOB,
	OPSIZE_XMOVXB,
	OPSIZE_EMOVS,
	OPSIZE_EMOVO,
	OPSIZE_EMOVE,
	OPSIZE_EMOVDEF,
	OPSIZE_EMOVSYS,
	OPSIZE_SWAP,
	OPSIZE_PINIDEF,
	OPSIZE_PINIO,
	OPSIZE_PARAMS,
	OPSIZE_SETESP,
	OPSIZE_CHECKESP,
	OPSIZE_RET,
	OPSIZE_RETN,
	OPSIZE_RETA,
	OPSIZE_RETO,
	OPSIZE_RETX,
	OPSIZE_YEILDBREAK,
	OPSIZE_BOX,
	OPSIZE_BOXNC,
	OPSIZE_NNBOX,
	OPSIZE_NNBOXNC,
	OPSIZE_UNBOX,
	OPSIZE_CHECKNULL,
	OPSIZE_CHECKNULLX,
	OPSIZE_CHECKTYPE,
	OPSIZE_CHECKNNTYPE,
	OPSIZE_FCALL,
	OPSIZE_SCALL,
	OPSIZE_AINVOKE,
	OPSIZE_CALL,
	OPSIZE_ACALL,
	OPSIZE_NEW,
	OPSIZE_COPYSFP,
	OPSIZE_TOSTR,
	OPSIZE_TOSTRF,
	OPSIZE_SMAP,
	OPSIZE_SMAPNC,
	OPSIZE_MAP,
	OPSIZE_MAPNC,
	OPSIZE_AMAP,
	OPSIZE_NNMAP,
	OPSIZE_JMP,
	OPSIZE_SKIP,
	OPSIZE_BJIFT,
	OPSIZE_BJIFF,
	OPSIZE_BJIFF_LOOP,
	OPSIZE_JIFNUL,
	OPSIZE_JIFNN,
	OPSIZE_NEXT,
	OPSIZE_INEXT,
	OPSIZE_MAPNEXT,
	OPSIZE_SMAPNEXT,
	OPSIZE_TRY,
	OPSIZE_TRYEND,
	OPSIZE_CATCH,
	OPSIZE_PUSH,
	OPSIZE_POP,
	OPSIZE_THROW,
	OPSIZE_THROWS,
	OPSIZE_THROW_AGAIN,
	OPSIZE_P,
	OPSIZE_PMSG,
	OPSIZE_ICAST,
	OPSIZE_INCAST,
	OPSIZE_FCAST,
	OPSIZE_FNCAST,
	OPSIZE_BNOT,
	OPSIZE_INEG,
	OPSIZE_IADD,
	OPSIZE_IADDN,
	OPSIZE_ISUB,
	OPSIZE_ISUBN,
	OPSIZE_IMUL,
	OPSIZE_IMULN,
	OPSIZE_IDIV,
	OPSIZE_IDIVN,
	OPSIZE_IMOD,
	OPSIZE_IMODN,
	OPSIZE_IEQ,
	OPSIZE_IEQN,
	OPSIZE_INEQ,
	OPSIZE_INEQN,
	OPSIZE_ILT,
	OPSIZE_ILTN,
	OPSIZE_ILTE,
	OPSIZE_ILTEN,
	OPSIZE_IGT,
	OPSIZE_IGTN,
	OPSIZE_IGTE,
	OPSIZE_IGTEN,
	OPSIZE_FNEG,
	OPSIZE_FADD,
	OPSIZE_FADDN,
	OPSIZE_FSUB,
	OPSIZE_FSUBN,
	OPSIZE_FMUL,
	OPSIZE_FMULN,
	OPSIZE_FDIV,
	OPSIZE_FDIVN,
	OPSIZE_FEQ,
	OPSIZE_FEQN,
	OPSIZE_FNEQ,
	OPSIZE_FNEQN,
	OPSIZE_FLT,
	OPSIZE_FLTN,
	OPSIZE_FLTE,
	OPSIZE_FLTEN,
	OPSIZE_FGT,
	OPSIZE_FGTN,
	OPSIZE_FGTE,
	OPSIZE_FGTEN,
	OPSIZE_AGET,
	OPSIZE_AGETN,
	OPSIZE_IAGET,
	OPSIZE_IAGETN,
	OPSIZE_FAGET,
	OPSIZE_FAGETN,
	OPSIZE_ASET,
	OPSIZE_ASETN,
	OPSIZE_IASET,
	OPSIZE_IASETN,
	OPSIZE_FASET,
	OPSIZE_FASETN,
	OPSIZE_INITCODE,
	OPSIZE_NOP,
};

size_t knh_code_opsize(int opcode)
{
	return OPSIZE[opcode];
}


knh_code_t* KNH_ASM_HALT_(Ctx *ctx, knh_Gamma_t *o)
{
	klr_halt_t *op = NULL;
	if(1) {
		op = (klr_halt_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_HALT);
		op->opcode = 0;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_MOVa_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2)
{
	klr_mova_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_mova_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_MOVA);
		op->opcode = 1;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_MOVn_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2)
{
	klr_movn_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_movn_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_MOVN);
		op->opcode = 2;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_MOVo_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_Object_t* a2)
{
	klr_movo_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_movo_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_MOVO);
		op->opcode = 3;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_Object_t*)*/a2;
		knh_Object_RCinc(op->a2);
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_MOVx_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfx_t a2)
{
	klr_movx_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_movx_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_MOVX);
		op->opcode = 4;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfx_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_MOVDEF_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_class_t a2)
{
	klr_movdef_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_movdef_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_MOVDEF);
		op->opcode = 5;
		op->a1 = /*(knh_sfi_t)*/a1;
		DBG2_ASSERT_cid(a2);
		op->a2 = /*(knh_class_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_MOVSYS_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_ushort_t a2)
{
	klr_movsys_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_movsys_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_MOVSYS);
		op->opcode = 6;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_ushort_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_XMOVs_(Ctx *ctx, knh_Gamma_t *o,knh_sfx_t a1,knh_sfi_t a2)
{
	klr_xmovs_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_xmovs_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_XMOVS);
		op->opcode = 7;
		op->a1 = /*(knh_sfx_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_XMOVo_(Ctx *ctx, knh_Gamma_t *o,knh_sfx_t a1,knh_Object_t* a2)
{
	klr_xmovo_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_xmovo_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_XMOVO);
		op->opcode = 8;
		op->a1 = /*(knh_sfx_t)*/a1;
		op->a2 = /*(knh_Object_t*)*/a2;
		knh_Object_RCinc(op->a2);
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_XMOVx_(Ctx *ctx, knh_Gamma_t *o,knh_sfx_t a1,knh_sfx_t a2)
{
	klr_xmovx_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_xmovx_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_XMOVX);
		op->opcode = 9;
		op->a1 = /*(knh_sfx_t)*/a1;
		op->a2 = /*(knh_sfx_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_XMOVDEF_(Ctx *ctx, knh_Gamma_t *o,knh_sfx_t a1,knh_class_t a2)
{
	klr_xmovdef_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_xmovdef_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_XMOVDEF);
		op->opcode = 10;
		op->a1 = /*(knh_sfx_t)*/a1;
		DBG2_ASSERT_cid(a2);
		op->a2 = /*(knh_class_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_XMOVSYS_(Ctx *ctx, knh_Gamma_t *o,knh_sfx_t a1,knh_ushort_t a2)
{
	klr_xmovsys_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_xmovsys_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_XMOVSYS);
		op->opcode = 11;
		op->a1 = /*(knh_sfx_t)*/a1;
		op->a2 = /*(knh_ushort_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_MOVxi_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfx_t a2)
{
	klr_movxi_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_movxi_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_MOVXI);
		op->opcode = 12;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfx_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_XMOVsi_(Ctx *ctx, knh_Gamma_t *o,knh_sfx_t a1,knh_sfi_t a2)
{
	klr_xmovsi_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_xmovsi_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_XMOVSI);
		op->opcode = 13;
		op->a1 = /*(knh_sfx_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_XMOVoi_(Ctx *ctx, knh_Gamma_t *o,knh_sfx_t a1,knh_Object_t* a2)
{
	klr_xmovoi_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_xmovoi_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_XMOVOI);
		op->opcode = 14;
		op->a1 = /*(knh_sfx_t)*/a1;
		op->a2 = /*(knh_Object_t*)*/a2;
		knh_Object_RCinc(op->a2);
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_XMOVxi_(Ctx *ctx, knh_Gamma_t *o,knh_sfx_t a1,knh_sfx_t a2)
{
	klr_xmovxi_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_xmovxi_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_XMOVXI);
		op->opcode = 15;
		op->a1 = /*(knh_sfx_t)*/a1;
		op->a2 = /*(knh_sfx_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_XMOVxio_(Ctx *ctx, knh_Gamma_t *o,knh_sfx_t a1,knh_sfx_t a2)
{
	klr_xmovxio_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_xmovxio_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_XMOVXIO);
		op->opcode = 16;
		op->a1 = /*(knh_sfx_t)*/a1;
		op->a2 = /*(knh_sfx_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_XMOVxBXi_(Ctx *ctx, knh_Gamma_t *o,knh_sfx_t a1,knh_sfx_t a2,knh_class_t a3)
{
	klr_xmovxbxi_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_xmovxbxi_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_XMOVXBXI);
		op->opcode = 17;
		op->a1 = /*(knh_sfx_t)*/a1;
		op->a2 = /*(knh_sfx_t)*/a2;
		DBG2_ASSERT_cid(a3);
		op->a3 = /*(knh_class_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_MOVxf_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfx_t a2)
{
	klr_movxf_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_movxf_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_MOVXF);
		op->opcode = 18;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfx_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_XMOVsf_(Ctx *ctx, knh_Gamma_t *o,knh_sfx_t a1,knh_sfi_t a2)
{
	klr_xmovsf_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_xmovsf_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_XMOVSF);
		op->opcode = 19;
		op->a1 = /*(knh_sfx_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_XMOVof_(Ctx *ctx, knh_Gamma_t *o,knh_sfx_t a1,knh_Object_t* a2)
{
	klr_xmovof_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_xmovof_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_XMOVOF);
		op->opcode = 20;
		op->a1 = /*(knh_sfx_t)*/a1;
		op->a2 = /*(knh_Object_t*)*/a2;
		knh_Object_RCinc(op->a2);
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_XMOVxf_(Ctx *ctx, knh_Gamma_t *o,knh_sfx_t a1,knh_sfx_t a2)
{
	klr_xmovxf_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_xmovxf_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_XMOVXF);
		op->opcode = 21;
		op->a1 = /*(knh_sfx_t)*/a1;
		op->a2 = /*(knh_sfx_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_XMOVxfo_(Ctx *ctx, knh_Gamma_t *o,knh_sfx_t a1,knh_sfx_t a2)
{
	klr_xmovxfo_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_xmovxfo_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_XMOVXFO);
		op->opcode = 22;
		op->a1 = /*(knh_sfx_t)*/a1;
		op->a2 = /*(knh_sfx_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_XMOVxBXf_(Ctx *ctx, knh_Gamma_t *o,knh_sfx_t a1,knh_sfx_t a2,knh_class_t a3)
{
	klr_xmovxbxf_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_xmovxbxf_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_XMOVXBXF);
		op->opcode = 23;
		op->a1 = /*(knh_sfx_t)*/a1;
		op->a2 = /*(knh_sfx_t)*/a2;
		DBG2_ASSERT_cid(a3);
		op->a3 = /*(knh_class_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_MOVxb_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfx_t a2)
{
	klr_movxb_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_movxb_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_MOVXB);
		op->opcode = 24;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfx_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_XMOVsb_(Ctx *ctx, knh_Gamma_t *o,knh_sfx_t a1,knh_sfi_t a2)
{
	klr_xmovsb_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_xmovsb_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_XMOVSB);
		op->opcode = 25;
		op->a1 = /*(knh_sfx_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_XMOVob_(Ctx *ctx, knh_Gamma_t *o,knh_sfx_t a1,knh_Object_t* a2)
{
	klr_xmovob_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_xmovob_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_XMOVOB);
		op->opcode = 26;
		op->a1 = /*(knh_sfx_t)*/a1;
		op->a2 = /*(knh_Object_t*)*/a2;
		knh_Object_RCinc(op->a2);
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_XMOVxb_(Ctx *ctx, knh_Gamma_t *o,knh_sfx_t a1,knh_sfx_t a2)
{
	klr_xmovxb_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_xmovxb_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_XMOVXB);
		op->opcode = 27;
		op->a1 = /*(knh_sfx_t)*/a1;
		op->a2 = /*(knh_sfx_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_EMOVs_(Ctx *ctx, knh_Gamma_t *o,knh_sfe_t a1,knh_sfe_t a2)
{
	klr_emovs_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_emovs_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_EMOVS);
		op->opcode = 28;
		op->a1 = /*(knh_sfe_t)*/a1;
		op->a2 = /*(knh_sfe_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_EMOVo_(Ctx *ctx, knh_Gamma_t *o,knh_sfe_t a1,knh_Object_t* a2)
{
	klr_emovo_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_emovo_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_EMOVO);
		op->opcode = 29;
		op->a1 = /*(knh_sfe_t)*/a1;
		op->a2 = /*(knh_Object_t*)*/a2;
		knh_Object_RCinc(op->a2);
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_EMOVe_(Ctx *ctx, knh_Gamma_t *o,knh_sfe_t a1,knh_sfe_t a2)
{
	klr_emove_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_emove_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_EMOVE);
		op->opcode = 30;
		op->a1 = /*(knh_sfe_t)*/a1;
		op->a2 = /*(knh_sfe_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_EMOVDEF_(Ctx *ctx, knh_Gamma_t *o,knh_sfe_t a1,knh_class_t a2)
{
	klr_emovdef_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_emovdef_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_EMOVDEF);
		op->opcode = 31;
		op->a1 = /*(knh_sfe_t)*/a1;
		DBG2_ASSERT_cid(a2);
		op->a2 = /*(knh_class_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_EMOVSYS_(Ctx *ctx, knh_Gamma_t *o,knh_sfe_t a1,knh_ushort_t a2)
{
	klr_emovsys_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_emovsys_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_EMOVSYS);
		op->opcode = 32;
		op->a1 = /*(knh_sfe_t)*/a1;
		op->a2 = /*(knh_ushort_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_SWAP_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2)
{
	klr_swap_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_swap_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_SWAP);
		op->opcode = 33;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_PINIDEF_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_class_t a2)
{
	klr_pinidef_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_pinidef_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_PINIDEF);
		op->opcode = 34;
		op->a1 = /*(knh_sfi_t)*/a1;
		DBG2_ASSERT_cid(a2);
		op->a2 = /*(knh_class_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_PINIo_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_Object_t* a2)
{
	klr_pinio_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_pinio_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_PINIO);
		op->opcode = 35;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_Object_t*)*/a2;
		knh_Object_RCinc(op->a2);
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_PARAMS_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_class_t a2)
{
	klr_params_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_params_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_PARAMS);
		op->opcode = 36;
		op->a1 = /*(knh_sfi_t)*/a1;
		DBG2_ASSERT_cid(a2);
		op->a2 = /*(knh_class_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_SETESP_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1)
{
	klr_setesp_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_setesp_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_SETESP);
		op->opcode = 37;
		op->a1 = /*(knh_sfi_t)*/a1;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_CHECKESP_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1)
{
	klr_checkesp_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_checkesp_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_CHECKESP);
		op->opcode = 38;
		op->a1 = /*(knh_sfi_t)*/a1;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_RET_(Ctx *ctx, knh_Gamma_t *o)
{
	klr_ret_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_ret_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_RET);
		op->opcode = 39;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_RETn_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2)
{
	klr_retn_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_retn_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_RETN);
		op->opcode = 40;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_RETa_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2)
{
	klr_reta_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_reta_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_RETA);
		op->opcode = 41;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_RETo_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_Object_t* a2)
{
	klr_reto_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_reto_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_RETO);
		op->opcode = 42;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_Object_t*)*/a2;
		knh_Object_RCinc(op->a2);
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_RETx_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfx_t a2)
{
	klr_retx_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_retx_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_RETX);
		op->opcode = 43;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfx_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_YEILDBREAK_(Ctx *ctx, knh_Gamma_t *o)
{
	klr_yeildbreak_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_yeildbreak_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_YEILDBREAK);
		op->opcode = 44;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_BOX_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_class_t a2)
{
	klr_box_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_box_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_BOX);
		op->opcode = 45;
		op->a1 = /*(knh_sfi_t)*/a1;
		DBG2_ASSERT_cid(a2);
		op->a2 = /*(knh_class_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_BOXnc_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_class_t a2)
{
	klr_boxnc_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_boxnc_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_BOXNC);
		op->opcode = 46;
		op->a1 = /*(knh_sfi_t)*/a1;
		DBG2_ASSERT_cid(a2);
		op->a2 = /*(knh_class_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_NNBOX_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_class_t a2)
{
	klr_nnbox_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_nnbox_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_NNBOX);
		op->opcode = 47;
		op->a1 = /*(knh_sfi_t)*/a1;
		DBG2_ASSERT_cid(a2);
		op->a2 = /*(knh_class_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_NNBOXnc_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_class_t a2)
{
	klr_nnboxnc_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_nnboxnc_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_NNBOXNC);
		op->opcode = 48;
		op->a1 = /*(knh_sfi_t)*/a1;
		DBG2_ASSERT_cid(a2);
		op->a2 = /*(knh_class_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_UNBOX_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1)
{
	klr_unbox_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_unbox_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_UNBOX);
		op->opcode = 49;
		op->a1 = /*(knh_sfi_t)*/a1;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_CHECKNULL_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1)
{
	klr_checknull_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_checknull_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_CHECKNULL);
		op->opcode = 50;
		op->a1 = /*(knh_sfi_t)*/a1;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_CHECKNULLx_(Ctx *ctx, knh_Gamma_t *o,knh_sfx_t a1)
{
	klr_checknullx_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_checknullx_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_CHECKNULLX);
		op->opcode = 51;
		op->a1 = /*(knh_sfx_t)*/a1;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_CHECKTYPE_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_class_t a2)
{
	klr_checktype_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_checktype_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_CHECKTYPE);
		op->opcode = 52;
		op->a1 = /*(knh_sfi_t)*/a1;
		DBG2_ASSERT_cid(a2);
		op->a2 = /*(knh_class_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_CHECKNNTYPE_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_type_t a2)
{
	klr_checknntype_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_checknntype_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_CHECKNNTYPE);
		op->opcode = 53;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_type_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_FCALL_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_ushort_t a2,knh_sfi_t a3,knh_Object_t* a4)
{
	klr_fcall_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_fcall_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_FCALL);
		op->opcode = 54;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_ushort_t)*/a2;
		op->a3 = /*(knh_sfi_t)*/a3;
		op->a4 = /*(knh_Object_t*)*/a4;
		knh_Object_RCinc(op->a4);
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_SCALL_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_ushort_t a2,knh_Object_t* a3)
{
	klr_scall_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_scall_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_SCALL);
		op->opcode = 55;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_ushort_t)*/a2;
		op->a3 = /*(knh_Object_t*)*/a3;
		knh_Object_RCinc(op->a3);
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_AINVOKE_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_ushort_t a2)
{
	klr_ainvoke_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_ainvoke_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_AINVOKE);
		op->opcode = 56;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_ushort_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_CALL_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_ushort_t a2,knh_methodn_t a3)
{
	klr_call_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_call_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_CALL);
		op->opcode = 57;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_ushort_t)*/a2;
		op->a3 = /*(knh_methodn_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_ACALL_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_ushort_t a2,knh_methodn_t a3)
{
	klr_acall_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_acall_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_ACALL);
		op->opcode = 58;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_ushort_t)*/a2;
		op->a3 = /*(knh_methodn_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_NEW_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_flag_t a2,knh_class_t a3,knh_ushort_t a4,knh_Object_t* a5)
{
	klr_new_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_new_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_NEW);
		op->opcode = 59;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_flag_t)*/a2;
		DBG2_ASSERT_cid(a3);
		op->a3 = /*(knh_class_t)*/a3;
		op->a4 = /*(knh_ushort_t)*/a4;
		op->a5 = /*(knh_Object_t*)*/a5;
		knh_Object_RCinc(op->a5);
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_COPYSFP_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1)
{
	klr_copysfp_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_copysfp_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_COPYSFP);
		op->opcode = 60;
		op->a1 = /*(knh_sfi_t)*/a1;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_TOSTR_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_methodn_t a2)
{
	klr_tostr_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_tostr_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_TOSTR);
		op->opcode = 61;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_methodn_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_TOSTRf_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_methodn_t a2,knh_Object_t* a3)
{
	klr_tostrf_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_tostrf_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_TOSTRF);
		op->opcode = 62;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_methodn_t)*/a2;
		op->a3 = /*(knh_Object_t*)*/a3;
		knh_Object_RCinc(op->a3);
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_SMAP_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_Object_t* a2)
{
	klr_smap_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_smap_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_SMAP);
		op->opcode = 63;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_Object_t*)*/a2;
		knh_Object_RCinc(op->a2);
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_SMAPnc_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_Object_t* a2)
{
	klr_smapnc_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_smapnc_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_SMAPNC);
		op->opcode = 64;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_Object_t*)*/a2;
		knh_Object_RCinc(op->a2);
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_MAP_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_class_t a2)
{
	klr_map_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_map_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_MAP);
		op->opcode = 65;
		op->a1 = /*(knh_sfi_t)*/a1;
		DBG2_ASSERT_cid(a2);
		op->a2 = /*(knh_class_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_MAPnc_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_class_t a2)
{
	klr_mapnc_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_mapnc_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_MAPNC);
		op->opcode = 66;
		op->a1 = /*(knh_sfi_t)*/a1;
		DBG2_ASSERT_cid(a2);
		op->a2 = /*(knh_class_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_AMAP_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_class_t a2)
{
	klr_amap_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_amap_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_AMAP);
		op->opcode = 67;
		op->a1 = /*(knh_sfi_t)*/a1;
		DBG2_ASSERT_cid(a2);
		op->a2 = /*(knh_class_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_NNMAP_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_class_t a2)
{
	klr_nnmap_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_nnmap_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_NNMAP);
		op->opcode = 68;
		op->a1 = /*(knh_sfi_t)*/a1;
		DBG2_ASSERT_cid(a2);
		op->a2 = /*(knh_class_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_JMP_(Ctx *ctx, knh_Gamma_t *o,knh_labelid_t a1)
{
	klr_jmp_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_jmp_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_JMP);
		op->opcode = 69;
		op->a1 = /*(knh_labelid_t)*/a1;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_SKIP_(Ctx *ctx, knh_Gamma_t *o,knh_labelid_t a1)
{
	klr_skip_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_skip_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_SKIP);
		op->opcode = 70;
		op->a1 = /*(knh_labelid_t)*/a1;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_bJIFT_(Ctx *ctx, knh_Gamma_t *o,knh_labelid_t a1,knh_sfi_t a2)
{
	klr_bjift_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_bjift_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_BJIFT);
		op->opcode = 71;
		op->a1 = /*(knh_labelid_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_bJIFF_(Ctx *ctx, knh_Gamma_t *o,knh_labelid_t a1,knh_sfi_t a2)
{
	klr_bjiff_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_bjiff_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_BJIFF);
		op->opcode = 72;
		op->a1 = /*(knh_labelid_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_bJIFF_LOOP_(Ctx *ctx, knh_Gamma_t *o,knh_labelid_t a1,knh_sfi_t a2)
{
	klr_bjiff_loop_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_bjiff_loop_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_BJIFF_LOOP);
		op->opcode = 73;
		op->a1 = /*(knh_labelid_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_JIFNUL_(Ctx *ctx, knh_Gamma_t *o,knh_labelid_t a1,knh_sfi_t a2)
{
	klr_jifnul_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_jifnul_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_JIFNUL);
		op->opcode = 74;
		op->a1 = /*(knh_labelid_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_JIFNN_(Ctx *ctx, knh_Gamma_t *o,knh_labelid_t a1,knh_sfi_t a2)
{
	klr_jifnn_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_jifnn_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_JIFNN);
		op->opcode = 75;
		op->a1 = /*(knh_labelid_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_NEXT_(Ctx *ctx, knh_Gamma_t *o,knh_labelid_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
	klr_next_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_next_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_NEXT);
		op->opcode = 76;
		op->a1 = /*(knh_labelid_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_sfi_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_INEXT_(Ctx *ctx, knh_Gamma_t *o,knh_labelid_t a1,knh_class_t a2,knh_sfi_t a3,knh_sfi_t a4)
{
	klr_inext_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_inext_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_INEXT);
		op->opcode = 77;
		op->a1 = /*(knh_labelid_t)*/a1;
		DBG2_ASSERT_cid(a2);
		op->a2 = /*(knh_class_t)*/a2;
		op->a3 = /*(knh_sfi_t)*/a3;
		op->a4 = /*(knh_sfi_t)*/a4;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_MAPNEXT_(Ctx *ctx, knh_Gamma_t *o,knh_labelid_t a1,knh_class_t a2,knh_sfi_t a3,knh_sfi_t a4)
{
	klr_mapnext_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_mapnext_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_MAPNEXT);
		op->opcode = 78;
		op->a1 = /*(knh_labelid_t)*/a1;
		DBG2_ASSERT_cid(a2);
		op->a2 = /*(knh_class_t)*/a2;
		op->a3 = /*(knh_sfi_t)*/a3;
		op->a4 = /*(knh_sfi_t)*/a4;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_SMAPNEXT_(Ctx *ctx, knh_Gamma_t *o,knh_labelid_t a1,knh_sfi_t a2,knh_sfi_t a3,knh_Object_t* a4)
{
	klr_smapnext_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_smapnext_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_SMAPNEXT);
		op->opcode = 79;
		op->a1 = /*(knh_labelid_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_sfi_t)*/a3;
		op->a4 = /*(knh_Object_t*)*/a4;
		knh_Object_RCinc(op->a4);
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_TRY_(Ctx *ctx, knh_Gamma_t *o,knh_labelid_t a1,knh_sfi_t a2)
{
	klr_try_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_try_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_TRY);
		op->opcode = 80;
		op->a1 = /*(knh_labelid_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_TRYEND_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1)
{
	klr_tryend_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_tryend_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_TRYEND);
		op->opcode = 81;
		op->a1 = /*(knh_sfi_t)*/a1;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_CATCH_(Ctx *ctx, knh_Gamma_t *o,knh_labelid_t a1,knh_sfi_t a2,knh_sfi_t a3,knh_Object_t* a4)
{
	klr_catch_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_catch_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_CATCH);
		op->opcode = 82;
		op->a1 = /*(knh_labelid_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_sfi_t)*/a3;
		op->a4 = /*(knh_Object_t*)*/a4;
		knh_Object_RCinc(op->a4);
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_PUSH_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1)
{
	klr_push_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_push_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_PUSH);
		op->opcode = 83;
		op->a1 = /*(knh_sfi_t)*/a1;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_POP_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1)
{
	klr_pop_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_pop_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_POP);
		op->opcode = 84;
		op->a1 = /*(knh_sfi_t)*/a1;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_THROW_(Ctx *ctx, knh_Gamma_t *o,knh_ushort_t a1,knh_sfi_t a2)
{
	klr_throw_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_throw_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_THROW);
		op->opcode = 85;
		op->a1 = /*(knh_ushort_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_THROWs_(Ctx *ctx, knh_Gamma_t *o,knh_ushort_t a1,knh_Object_t* a2)
{
	klr_throws_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_throws_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_THROWS);
		op->opcode = 86;
		op->a1 = /*(knh_ushort_t)*/a1;
		op->a2 = /*(knh_Object_t*)*/a2;
		knh_Object_RCinc(op->a2);
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_THROW_AGAIN_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1)
{
	klr_throw_again_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_throw_again_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_THROW_AGAIN);
		op->opcode = 87;
		op->a1 = /*(knh_sfi_t)*/a1;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_P_(Ctx *ctx, knh_Gamma_t *o,knh_flag_t a1,knh_methodn_t a2,knh_sfi_t a3)
{
	klr_p_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_p_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_P);
		op->opcode = 88;
		op->a1 = /*(knh_flag_t)*/a1;
		op->a2 = /*(knh_methodn_t)*/a2;
		op->a3 = /*(knh_sfi_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_PMSG_(Ctx *ctx, knh_Gamma_t *o,knh_flag_t a1,knh_Object_t* a2)
{
	klr_pmsg_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_pmsg_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_PMSG);
		op->opcode = 89;
		op->a1 = /*(knh_flag_t)*/a1;
		op->a2 = /*(knh_Object_t*)*/a2;
		knh_Object_RCinc(op->a2);
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_iCAST_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1)
{
	klr_icast_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_icast_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_ICAST);
		op->opcode = 90;
		op->a1 = /*(knh_sfi_t)*/a1;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_inCAST_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1)
{
	klr_incast_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_incast_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_INCAST);
		op->opcode = 91;
		op->a1 = /*(knh_sfi_t)*/a1;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_fCAST_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1)
{
	klr_fcast_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_fcast_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_FCAST);
		op->opcode = 92;
		op->a1 = /*(knh_sfi_t)*/a1;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_fnCAST_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1)
{
	klr_fncast_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_fncast_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_FNCAST);
		op->opcode = 93;
		op->a1 = /*(knh_sfi_t)*/a1;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_bNOT_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2)
{
	klr_bnot_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_bnot_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_BNOT);
		op->opcode = 94;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_iNEG_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2)
{
	klr_ineg_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_ineg_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_INEG);
		op->opcode = 95;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_iADD_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
	klr_iadd_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_iadd_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_IADD);
		op->opcode = 96;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_sfi_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_iADDn_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_int_t a3)
{
	klr_iaddn_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_iaddn_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_IADDN);
		op->opcode = 97;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_int_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_iSUB_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
	klr_isub_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_isub_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_ISUB);
		op->opcode = 98;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_sfi_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_iSUBn_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_int_t a3)
{
	klr_isubn_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_isubn_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_ISUBN);
		op->opcode = 99;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_int_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_iMUL_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
	klr_imul_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_imul_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_IMUL);
		op->opcode = 100;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_sfi_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_iMULn_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_int_t a3)
{
	klr_imuln_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_imuln_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_IMULN);
		op->opcode = 101;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_int_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_iDIV_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
	klr_idiv_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_idiv_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_IDIV);
		op->opcode = 102;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_sfi_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_iDIVn_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_int_t a3)
{
	klr_idivn_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_idivn_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_IDIVN);
		op->opcode = 103;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_int_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_iMOD_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
	klr_imod_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_imod_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_IMOD);
		op->opcode = 104;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_sfi_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_iMODn_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_int_t a3)
{
	klr_imodn_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_imodn_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_IMODN);
		op->opcode = 105;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_int_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_iEQ_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
	klr_ieq_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_ieq_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_IEQ);
		op->opcode = 106;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_sfi_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_iEQn_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_int_t a3)
{
	klr_ieqn_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_ieqn_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_IEQN);
		op->opcode = 107;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_int_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_iNEQ_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
	klr_ineq_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_ineq_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_INEQ);
		op->opcode = 108;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_sfi_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_iNEQn_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_int_t a3)
{
	klr_ineqn_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_ineqn_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_INEQN);
		op->opcode = 109;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_int_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_iLT_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
	klr_ilt_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_ilt_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_ILT);
		op->opcode = 110;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_sfi_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_iLTn_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_int_t a3)
{
	klr_iltn_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_iltn_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_ILTN);
		op->opcode = 111;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_int_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_iLTE_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
	klr_ilte_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_ilte_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_ILTE);
		op->opcode = 112;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_sfi_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_iLTEn_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_int_t a3)
{
	klr_ilten_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_ilten_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_ILTEN);
		op->opcode = 113;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_int_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_iGT_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
	klr_igt_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_igt_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_IGT);
		op->opcode = 114;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_sfi_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_iGTn_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_int_t a3)
{
	klr_igtn_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_igtn_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_IGTN);
		op->opcode = 115;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_int_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_iGTE_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
	klr_igte_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_igte_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_IGTE);
		op->opcode = 116;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_sfi_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_iGTEn_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_int_t a3)
{
	klr_igten_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_igten_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_IGTEN);
		op->opcode = 117;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_int_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_fNEG_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2)
{
	klr_fneg_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_fneg_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_FNEG);
		op->opcode = 118;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_fADD_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
	klr_fadd_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_fadd_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_FADD);
		op->opcode = 119;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_sfi_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_fADDn_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_float_t a3)
{
	klr_faddn_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_faddn_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_FADDN);
		op->opcode = 120;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_float_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_fSUB_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
	klr_fsub_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_fsub_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_FSUB);
		op->opcode = 121;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_sfi_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_fSUBn_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_float_t a3)
{
	klr_fsubn_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_fsubn_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_FSUBN);
		op->opcode = 122;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_float_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_fMUL_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
	klr_fmul_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_fmul_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_FMUL);
		op->opcode = 123;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_sfi_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_fMULn_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_float_t a3)
{
	klr_fmuln_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_fmuln_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_FMULN);
		op->opcode = 124;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_float_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_fDIV_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
	klr_fdiv_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_fdiv_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_FDIV);
		op->opcode = 125;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_sfi_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_fDIVn_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_float_t a3)
{
	klr_fdivn_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_fdivn_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_FDIVN);
		op->opcode = 126;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_float_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_fEQ_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
	klr_feq_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_feq_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_FEQ);
		op->opcode = 127;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_sfi_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_fEQn_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_float_t a3)
{
	klr_feqn_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_feqn_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_FEQN);
		op->opcode = 128;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_float_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_fNEQ_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
	klr_fneq_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_fneq_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_FNEQ);
		op->opcode = 129;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_sfi_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_fNEQn_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_float_t a3)
{
	klr_fneqn_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_fneqn_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_FNEQN);
		op->opcode = 130;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_float_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_fLT_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
	klr_flt_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_flt_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_FLT);
		op->opcode = 131;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_sfi_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_fLTn_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_float_t a3)
{
	klr_fltn_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_fltn_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_FLTN);
		op->opcode = 132;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_float_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_fLTE_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
	klr_flte_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_flte_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_FLTE);
		op->opcode = 133;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_sfi_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_fLTEn_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_float_t a3)
{
	klr_flten_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_flten_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_FLTEN);
		op->opcode = 134;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_float_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_fGT_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
	klr_fgt_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_fgt_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_FGT);
		op->opcode = 135;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_sfi_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_fGTn_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_float_t a3)
{
	klr_fgtn_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_fgtn_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_FGTN);
		op->opcode = 136;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_float_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_fGTE_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
	klr_fgte_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_fgte_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_FGTE);
		op->opcode = 137;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_sfi_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_fGTEn_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_float_t a3)
{
	klr_fgten_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_fgten_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_FGTEN);
		op->opcode = 138;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_float_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_AGET_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
	klr_aget_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_aget_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_AGET);
		op->opcode = 139;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_sfi_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_AGETn_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_intptr_t a3)
{
	klr_agetn_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_agetn_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_AGETN);
		op->opcode = 140;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_intptr_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_IAGET_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
	klr_iaget_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_iaget_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_IAGET);
		op->opcode = 141;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_sfi_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_IAGETn_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_intptr_t a3)
{
	klr_iagetn_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_iagetn_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_IAGETN);
		op->opcode = 142;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_intptr_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_FAGET_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
	klr_faget_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_faget_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_FAGET);
		op->opcode = 143;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_sfi_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_FAGETn_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_intptr_t a3)
{
	klr_fagetn_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_fagetn_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_FAGETN);
		op->opcode = 144;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_intptr_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_ASET_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
	klr_aset_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_aset_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_ASET);
		op->opcode = 145;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_sfi_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_ASETn_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_intptr_t a3)
{
	klr_asetn_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_asetn_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_ASETN);
		op->opcode = 146;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_intptr_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_IASET_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
	klr_iaset_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_iaset_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_IASET);
		op->opcode = 147;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_sfi_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_IASETn_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_intptr_t a3)
{
	klr_iasetn_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_iasetn_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_IASETN);
		op->opcode = 148;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_intptr_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_FASET_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_sfi_t a3)
{
	klr_faset_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_faset_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_FASET);
		op->opcode = 149;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_sfi_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_FASETn_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1,knh_sfi_t a2,knh_intptr_t a3)
{
	klr_fasetn_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_fasetn_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_FASETN);
		op->opcode = 150;
		op->a1 = /*(knh_sfi_t)*/a1;
		op->a2 = /*(knh_sfi_t)*/a2;
		op->a3 = /*(knh_intptr_t)*/a3;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_INITCODE_(Ctx *ctx, knh_Gamma_t *o,knh_sfi_t a1)
{
	klr_initcode_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_initcode_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_INITCODE);
		op->opcode = 151;
		op->a1 = /*(knh_sfi_t)*/a1;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

knh_code_t* KNH_ASM_NOP_(Ctx *ctx, knh_Gamma_t *o)
{
	klr_nop_t *op = NULL;
	if(!knh_Gamma_isCancelled(o)) {
		op = (klr_nop_t*)knh_Gamma_asmmalloc(ctx, o, OPSIZE_NOP);
		op->opcode = 152;
		DP(o)->prev_op = (knh_kode_t*)op;
	}
	return (knh_code_t*)op;
}

/* ------------------------------------------------------------------------ */

/* ======================================================================== */
/* [label] */


typedef void (*frewrite)(Ctx *ctx, knh_Gamma_t *o, knh_code_t *pc_start, knh_kode_t *op);

void knh_rewrite_addr(Ctx *ctx, knh_Gamma_t *o, knh_code_t *pc_start, knh_kode_t *op)
{
	int offset = DP(o)->labels[op->a1].offset;
	if(offset == -1) {
		DBG_P("Gamma!!: unknown Labelid=%d", op->a1);
		knh_Gamma_setCancelled(o, 1);
	}else {
		op->jumppc = pc_start + offset;
		//DBG2_P("%p, op=%d, jmp=%p", op, op->opcode, op->jumpaddr);
	}
}

/* ------------------------------------------------------------------------ */

void knh_rewrite_NOP(Ctx *ctx, knh_Gamma_t *o, knh_code_t *pc_start, knh_kode_t *op)
{
	//DBG2_P("%p, op=%d", op, op->opcode);
}

/* ------------------------------------------------------------------------ */

static frewrite OPREWRITE[] = {
	knh_rewrite_NOP, /* HALT */
	knh_rewrite_NOP, /* MOVa */
	knh_rewrite_NOP, /* MOVn */
	knh_rewrite_NOP, /* MOVo */
	knh_rewrite_NOP, /* MOVx */
	knh_rewrite_NOP, /* MOVDEF */
	knh_rewrite_NOP, /* MOVSYS */
	knh_rewrite_NOP, /* XMOVs */
	knh_rewrite_NOP, /* XMOVo */
	knh_rewrite_NOP, /* XMOVx */
	knh_rewrite_NOP, /* XMOVDEF */
	knh_rewrite_NOP, /* XMOVSYS */
	knh_rewrite_NOP, /* MOVxi */
	knh_rewrite_NOP, /* XMOVsi */
	knh_rewrite_NOP, /* XMOVoi */
	knh_rewrite_NOP, /* XMOVxi */
	knh_rewrite_NOP, /* XMOVxio */
	knh_rewrite_NOP, /* XMOVxBXi */
	knh_rewrite_NOP, /* MOVxf */
	knh_rewrite_NOP, /* XMOVsf */
	knh_rewrite_NOP, /* XMOVof */
	knh_rewrite_NOP, /* XMOVxf */
	knh_rewrite_NOP, /* XMOVxfo */
	knh_rewrite_NOP, /* XMOVxBXf */
	knh_rewrite_NOP, /* MOVxb */
	knh_rewrite_NOP, /* XMOVsb */
	knh_rewrite_NOP, /* XMOVob */
	knh_rewrite_NOP, /* XMOVxb */
	knh_rewrite_NOP, /* EMOVs */
	knh_rewrite_NOP, /* EMOVo */
	knh_rewrite_NOP, /* EMOVe */
	knh_rewrite_NOP, /* EMOVDEF */
	knh_rewrite_NOP, /* EMOVSYS */
	knh_rewrite_NOP, /* SWAP */
	knh_rewrite_NOP, /* PINIDEF */
	knh_rewrite_NOP, /* PINIo */
	knh_rewrite_NOP, /* PARAMS */
	knh_rewrite_NOP, /* SETESP */
	knh_rewrite_NOP, /* CHECKESP */
	knh_rewrite_NOP, /* RET */
	knh_rewrite_NOP, /* RETn */
	knh_rewrite_NOP, /* RETa */
	knh_rewrite_NOP, /* RETo */
	knh_rewrite_NOP, /* RETx */
	knh_rewrite_NOP, /* YEILDBREAK */
	knh_rewrite_NOP, /* BOX */
	knh_rewrite_NOP, /* BOXnc */
	knh_rewrite_NOP, /* NNBOX */
	knh_rewrite_NOP, /* NNBOXnc */
	knh_rewrite_NOP, /* UNBOX */
	knh_rewrite_NOP, /* CHECKNULL */
	knh_rewrite_NOP, /* CHECKNULLx */
	knh_rewrite_NOP, /* CHECKTYPE */
	knh_rewrite_NOP, /* CHECKNNTYPE */
	knh_rewrite_NOP, /* FCALL */
	knh_rewrite_NOP, /* SCALL */
	knh_rewrite_NOP, /* AINVOKE */
	knh_rewrite_NOP, /* CALL */
	knh_rewrite_NOP, /* ACALL */
	knh_rewrite_NOP, /* NEW */
	knh_rewrite_NOP, /* COPYSFP */
	knh_rewrite_NOP, /* TOSTR */
	knh_rewrite_NOP, /* TOSTRf */
	knh_rewrite_NOP, /* SMAP */
	knh_rewrite_NOP, /* SMAPnc */
	knh_rewrite_NOP, /* MAP */
	knh_rewrite_NOP, /* MAPnc */
	knh_rewrite_NOP, /* AMAP */
	knh_rewrite_NOP, /* NNMAP */
	knh_rewrite_addr, /* JMP */
	knh_rewrite_addr, /* SKIP */
	knh_rewrite_addr, /* bJIFT */
	knh_rewrite_addr, /* bJIFF */
	knh_rewrite_addr, /* bJIFF_LOOP */
	knh_rewrite_addr, /* JIFNUL */
	knh_rewrite_addr, /* JIFNN */
	knh_rewrite_addr, /* NEXT */
	knh_rewrite_addr, /* INEXT */
	knh_rewrite_addr, /* MAPNEXT */
	knh_rewrite_addr, /* SMAPNEXT */
	knh_rewrite_addr, /* TRY */
	knh_rewrite_NOP, /* TRYEND */
	knh_rewrite_addr, /* CATCH */
	knh_rewrite_NOP, /* PUSH */
	knh_rewrite_NOP, /* POP */
	knh_rewrite_NOP, /* THROW */
	knh_rewrite_NOP, /* THROWs */
	knh_rewrite_NOP, /* THROW_AGAIN */
	knh_rewrite_NOP, /* P */
	knh_rewrite_NOP, /* PMSG */
	knh_rewrite_NOP, /* iCAST */
	knh_rewrite_NOP, /* inCAST */
	knh_rewrite_NOP, /* fCAST */
	knh_rewrite_NOP, /* fnCAST */
	knh_rewrite_NOP, /* bNOT */
	knh_rewrite_NOP, /* iNEG */
	knh_rewrite_NOP, /* iADD */
	knh_rewrite_NOP, /* iADDn */
	knh_rewrite_NOP, /* iSUB */
	knh_rewrite_NOP, /* iSUBn */
	knh_rewrite_NOP, /* iMUL */
	knh_rewrite_NOP, /* iMULn */
	knh_rewrite_NOP, /* iDIV */
	knh_rewrite_NOP, /* iDIVn */
	knh_rewrite_NOP, /* iMOD */
	knh_rewrite_NOP, /* iMODn */
	knh_rewrite_NOP, /* iEQ */
	knh_rewrite_NOP, /* iEQn */
	knh_rewrite_NOP, /* iNEQ */
	knh_rewrite_NOP, /* iNEQn */
	knh_rewrite_NOP, /* iLT */
	knh_rewrite_NOP, /* iLTn */
	knh_rewrite_NOP, /* iLTE */
	knh_rewrite_NOP, /* iLTEn */
	knh_rewrite_NOP, /* iGT */
	knh_rewrite_NOP, /* iGTn */
	knh_rewrite_NOP, /* iGTE */
	knh_rewrite_NOP, /* iGTEn */
	knh_rewrite_NOP, /* fNEG */
	knh_rewrite_NOP, /* fADD */
	knh_rewrite_NOP, /* fADDn */
	knh_rewrite_NOP, /* fSUB */
	knh_rewrite_NOP, /* fSUBn */
	knh_rewrite_NOP, /* fMUL */
	knh_rewrite_NOP, /* fMULn */
	knh_rewrite_NOP, /* fDIV */
	knh_rewrite_NOP, /* fDIVn */
	knh_rewrite_NOP, /* fEQ */
	knh_rewrite_NOP, /* fEQn */
	knh_rewrite_NOP, /* fNEQ */
	knh_rewrite_NOP, /* fNEQn */
	knh_rewrite_NOP, /* fLT */
	knh_rewrite_NOP, /* fLTn */
	knh_rewrite_NOP, /* fLTE */
	knh_rewrite_NOP, /* fLTEn */
	knh_rewrite_NOP, /* fGT */
	knh_rewrite_NOP, /* fGTn */
	knh_rewrite_NOP, /* fGTE */
	knh_rewrite_NOP, /* fGTEn */
	knh_rewrite_NOP, /* AGET */
	knh_rewrite_NOP, /* AGETn */
	knh_rewrite_NOP, /* IAGET */
	knh_rewrite_NOP, /* IAGETn */
	knh_rewrite_NOP, /* FAGET */
	knh_rewrite_NOP, /* FAGETn */
	knh_rewrite_NOP, /* ASET */
	knh_rewrite_NOP, /* ASETn */
	knh_rewrite_NOP, /* IASET */
	knh_rewrite_NOP, /* IASETn */
	knh_rewrite_NOP, /* FASET */
	knh_rewrite_NOP, /* FASETn */
	knh_rewrite_NOP, /* INITCODE */
	knh_rewrite_NOP, /* NOP */
};

/* ------------------------------------------------------------------------ */

void knh_Gamma_writeAddress(Ctx *ctx, knh_Gamma_t *o, knh_code_t *pc_start)
{
	knh_code_t *pc = pc_start;
	while(1) {
		int opcode = KNH_OPCODE(pc);
		if(opcode == OPCODE_HALT) return ; 
		OPREWRITE[opcode](ctx, o, pc_start, (knh_kode_t*)pc);
		pc += OPSIZE[opcode];
	}
}

/* ======================================================================== */
/* [vmc] */

void knh_code_traverse(Ctx *ctx, knh_code_t *pc, knh_ftraverse ftr)
{
	L_NEXT:
	switch(KNH_OPCODE(pc)) {
		case OPCODE_HALT:
			return;
		case OPCODE_MOVO :
			ftr(ctx, ((klr_movo_t*)pc)->a2);
			break;
		case OPCODE_XMOVO :
			ftr(ctx, ((klr_xmovo_t*)pc)->a2);
			break;
		case OPCODE_XMOVOI :
			ftr(ctx, ((klr_xmovoi_t*)pc)->a2);
			break;
		case OPCODE_XMOVOF :
			ftr(ctx, ((klr_xmovof_t*)pc)->a2);
			break;
		case OPCODE_XMOVOB :
			ftr(ctx, ((klr_xmovob_t*)pc)->a2);
			break;
		case OPCODE_EMOVO :
			ftr(ctx, ((klr_emovo_t*)pc)->a2);
			break;
		case OPCODE_PINIO :
			ftr(ctx, ((klr_pinio_t*)pc)->a2);
			break;
		case OPCODE_RETO :
			ftr(ctx, ((klr_reto_t*)pc)->a2);
			break;
		case OPCODE_FCALL :
			ftr(ctx, ((klr_fcall_t*)pc)->a4);
			break;
		case OPCODE_SCALL :
			ftr(ctx, ((klr_scall_t*)pc)->a3);
			break;
		case OPCODE_NEW :
			ftr(ctx, ((klr_new_t*)pc)->a5);
			break;
		case OPCODE_TOSTRF :
			ftr(ctx, ((klr_tostrf_t*)pc)->a3);
			break;
		case OPCODE_SMAP :
			ftr(ctx, ((klr_smap_t*)pc)->a2);
			break;
		case OPCODE_SMAPNC :
			ftr(ctx, ((klr_smapnc_t*)pc)->a2);
			break;
		case OPCODE_SMAPNEXT :
			ftr(ctx, ((klr_smapnext_t*)pc)->a4);
			break;
		case OPCODE_CATCH :
			ftr(ctx, ((klr_catch_t*)pc)->a4);
			break;
		case OPCODE_THROWS :
			ftr(ctx, ((klr_throws_t*)pc)->a2);
			break;
		case OPCODE_PMSG :
			ftr(ctx, ((klr_pmsg_t*)pc)->a2);
			break;
	}
	pc += OPSIZE[KNH_OPCODE(pc)];
	goto L_NEXT;
}

/* ======================================================================== */
/* [dump] */

#define knh_write__addr(ctx, w, a)  knh_printf(ctx, w, "%p", a)
#define knh_write__boolean(ctx, w, a)  knh_write_char(ctx, w, (a) ? "true" : "false")
#define knh_write__cid(ctx, w, a) knh_write_cid(ctx, w, a)
#define knh_write__float(ctx, w, a) knh_write_ffmt(ctx, w, KNH_FLOAT_FMT, a)
#define knh_write__integer(ctx, w, a) knh_write_ifmt(ctx, w, KNH_INT_FMT, a)
#define knh_write__intptr(ctx, w, a)  knh_write_dfmt(ctx, w, KNH_INTPTR_FMT, a)
#define knh_write__OBJ(ctx, w, a)  knh_format(ctx, w, METHODN__k, a, KNH_NULL)
#define knh_write__sfi(ctx, w, a)  knh_printf(ctx, w, "sfp[%d]", (knh_intptr_t)a);
#define knh_write__sfe(ctx, w, a)  knh_printf(ctx, w, "sfe[%d]", (knh_intptr_t)a);
#define knh_write__sfx(ctx, w, a)  knh_printf(ctx, w, "sfp[%d]+%d", (knh_intptr_t)a.i, (knh_intptr_t)a.n)
#define knh_write__ushort(ctx, w, a)    knh_write_dfmt(ctx, w, KNH_INTPTR_FMT, (knh_intptr_t)a)
#define knh_write__mn(ctx, w, a)    knh_write_mn(ctx, w, TYPE_void, (knh_methodn_t)a)
#define knh_write__type(ctx, w, a)    knh_write_type(ctx, w, (knh_type_t)a)

static
void KNH_DUMP_OPCODE(Ctx *ctx, knh_code_t *pc, knh_OutputStream_t*w, knh_Method_t *mtd, char *name)
{
	knh_kode_t *op = (knh_kode_t*)pc;
	int line = 0;
	if(IS_Method(mtd)) {
		line = knh_Method_pctoline(mtd, pc);
	}
	if(line == 0) {
		knh_printf(ctx, w, " [%p] %s(%d)", pc, name, (knh_intptr_t)op->opcode);
	}else{
		knh_printf(ctx, w, " [%d:%p] %s(%d)", line, pc, name, (knh_intptr_t)op->opcode);
	}
}

/* ------------------------------------------------------------------------ */


static
void KNH_DUMP_HALT(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	//klr_halt_t *op = (klr_halt_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "HALT");
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_MOVa(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_mova_t *op = (klr_mova_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "MOVa");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_MOVn(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_movn_t *op = (klr_movn_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "MOVn");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_MOVo(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_movo_t *op = (klr_movo_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "MOVo");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__OBJ(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_MOVx(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_movx_t *op = (klr_movx_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "MOVx");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfx(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_MOVDEF(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_movdef_t *op = (klr_movdef_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "MOVDEF");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__cid(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_MOVSYS(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_movsys_t *op = (klr_movsys_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "MOVSYS");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__ushort(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_XMOVs(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_xmovs_t *op = (klr_xmovs_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "XMOVs");
	knh_putc(ctx, w, ' '); knh_write__sfx(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_XMOVo(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_xmovo_t *op = (klr_xmovo_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "XMOVo");
	knh_putc(ctx, w, ' '); knh_write__sfx(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__OBJ(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_XMOVx(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_xmovx_t *op = (klr_xmovx_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "XMOVx");
	knh_putc(ctx, w, ' '); knh_write__sfx(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfx(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_XMOVDEF(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_xmovdef_t *op = (klr_xmovdef_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "XMOVDEF");
	knh_putc(ctx, w, ' '); knh_write__sfx(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__cid(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_XMOVSYS(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_xmovsys_t *op = (klr_xmovsys_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "XMOVSYS");
	knh_putc(ctx, w, ' '); knh_write__sfx(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__ushort(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_MOVxi(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_movxi_t *op = (klr_movxi_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "MOVxi");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfx(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_XMOVsi(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_xmovsi_t *op = (klr_xmovsi_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "XMOVsi");
	knh_putc(ctx, w, ' '); knh_write__sfx(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_XMOVoi(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_xmovoi_t *op = (klr_xmovoi_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "XMOVoi");
	knh_putc(ctx, w, ' '); knh_write__sfx(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__OBJ(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_XMOVxi(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_xmovxi_t *op = (klr_xmovxi_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "XMOVxi");
	knh_putc(ctx, w, ' '); knh_write__sfx(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfx(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_XMOVxio(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_xmovxio_t *op = (klr_xmovxio_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "XMOVxio");
	knh_putc(ctx, w, ' '); knh_write__sfx(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfx(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_XMOVxBXi(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_xmovxbxi_t *op = (klr_xmovxbxi_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "XMOVxBXi");
	knh_putc(ctx, w, ' '); knh_write__sfx(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfx(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__cid(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_MOVxf(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_movxf_t *op = (klr_movxf_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "MOVxf");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfx(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_XMOVsf(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_xmovsf_t *op = (klr_xmovsf_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "XMOVsf");
	knh_putc(ctx, w, ' '); knh_write__sfx(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_XMOVof(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_xmovof_t *op = (klr_xmovof_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "XMOVof");
	knh_putc(ctx, w, ' '); knh_write__sfx(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__OBJ(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_XMOVxf(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_xmovxf_t *op = (klr_xmovxf_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "XMOVxf");
	knh_putc(ctx, w, ' '); knh_write__sfx(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfx(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_XMOVxfo(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_xmovxfo_t *op = (klr_xmovxfo_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "XMOVxfo");
	knh_putc(ctx, w, ' '); knh_write__sfx(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfx(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_XMOVxBXf(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_xmovxbxf_t *op = (klr_xmovxbxf_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "XMOVxBXf");
	knh_putc(ctx, w, ' '); knh_write__sfx(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfx(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__cid(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_MOVxb(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_movxb_t *op = (klr_movxb_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "MOVxb");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfx(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_XMOVsb(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_xmovsb_t *op = (klr_xmovsb_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "XMOVsb");
	knh_putc(ctx, w, ' '); knh_write__sfx(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_XMOVob(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_xmovob_t *op = (klr_xmovob_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "XMOVob");
	knh_putc(ctx, w, ' '); knh_write__sfx(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__OBJ(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_XMOVxb(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_xmovxb_t *op = (klr_xmovxb_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "XMOVxb");
	knh_putc(ctx, w, ' '); knh_write__sfx(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfx(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_EMOVs(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_emovs_t *op = (klr_emovs_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "EMOVs");
	knh_putc(ctx, w, ' '); knh_write__sfe(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfe(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_EMOVo(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_emovo_t *op = (klr_emovo_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "EMOVo");
	knh_putc(ctx, w, ' '); knh_write__sfe(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__OBJ(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_EMOVe(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_emove_t *op = (klr_emove_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "EMOVe");
	knh_putc(ctx, w, ' '); knh_write__sfe(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfe(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_EMOVDEF(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_emovdef_t *op = (klr_emovdef_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "EMOVDEF");
	knh_putc(ctx, w, ' '); knh_write__sfe(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__cid(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_EMOVSYS(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_emovsys_t *op = (klr_emovsys_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "EMOVSYS");
	knh_putc(ctx, w, ' '); knh_write__sfe(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__ushort(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_SWAP(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_swap_t *op = (klr_swap_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "SWAP");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_PINIDEF(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_pinidef_t *op = (klr_pinidef_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "PINIDEF");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__cid(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_PINIo(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_pinio_t *op = (klr_pinio_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "PINIo");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__OBJ(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_PARAMS(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_params_t *op = (klr_params_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "PARAMS");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__cid(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_SETESP(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_setesp_t *op = (klr_setesp_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "SETESP");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_CHECKESP(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_checkesp_t *op = (klr_checkesp_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "CHECKESP");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_RET(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	//klr_ret_t *op = (klr_ret_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "RET");
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_RETn(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_retn_t *op = (klr_retn_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "RETn");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_RETa(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_reta_t *op = (klr_reta_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "RETa");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_RETo(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_reto_t *op = (klr_reto_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "RETo");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__OBJ(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_RETx(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_retx_t *op = (klr_retx_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "RETx");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfx(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_YEILDBREAK(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	//klr_yeildbreak_t *op = (klr_yeildbreak_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "YEILDBREAK");
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_BOX(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_box_t *op = (klr_box_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "BOX");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__cid(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_BOXnc(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_boxnc_t *op = (klr_boxnc_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "BOXnc");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__cid(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_NNBOX(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_nnbox_t *op = (klr_nnbox_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "NNBOX");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__cid(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_NNBOXnc(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_nnboxnc_t *op = (klr_nnboxnc_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "NNBOXnc");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__cid(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_UNBOX(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_unbox_t *op = (klr_unbox_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "UNBOX");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_CHECKNULL(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_checknull_t *op = (klr_checknull_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "CHECKNULL");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_CHECKNULLx(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_checknullx_t *op = (klr_checknullx_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "CHECKNULLx");
	knh_putc(ctx, w, ' '); knh_write__sfx(ctx, w, (op->a1));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_CHECKTYPE(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_checktype_t *op = (klr_checktype_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "CHECKTYPE");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__cid(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_CHECKNNTYPE(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_checknntype_t *op = (klr_checknntype_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "CHECKNNTYPE");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__type(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_FCALL(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_fcall_t *op = (klr_fcall_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "FCALL");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__ushort(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a3));
	knh_putc(ctx, w, ' '); knh_write__OBJ(ctx, w, (op->a4));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_SCALL(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_scall_t *op = (klr_scall_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "SCALL");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__ushort(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__OBJ(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_AINVOKE(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_ainvoke_t *op = (klr_ainvoke_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "AINVOKE");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__ushort(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_CALL(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_call_t *op = (klr_call_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "CALL");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__ushort(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__mn(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_ACALL(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_acall_t *op = (klr_acall_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "ACALL");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__ushort(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__mn(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_NEW(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_new_t *op = (klr_new_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "NEW");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__flag(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__cid(ctx, w, (op->a3));
	knh_putc(ctx, w, ' '); knh_write__ushort(ctx, w, (op->a4));
	knh_putc(ctx, w, ' '); knh_write__OBJ(ctx, w, (op->a5));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_COPYSFP(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_copysfp_t *op = (klr_copysfp_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "COPYSFP");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_TOSTR(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_tostr_t *op = (klr_tostr_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "TOSTR");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__mn(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_TOSTRf(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_tostrf_t *op = (klr_tostrf_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "TOSTRf");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__mn(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__OBJ(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_SMAP(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_smap_t *op = (klr_smap_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "SMAP");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__OBJ(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_SMAPnc(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_smapnc_t *op = (klr_smapnc_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "SMAPnc");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__OBJ(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_MAP(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_map_t *op = (klr_map_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "MAP");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__cid(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_MAPnc(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_mapnc_t *op = (klr_mapnc_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "MAPnc");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__cid(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_AMAP(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_amap_t *op = (klr_amap_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "AMAP");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__cid(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_NNMAP(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_nnmap_t *op = (klr_nnmap_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "NNMAP");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__cid(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_JMP(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_jmp_t *op = (klr_jmp_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "JMP");
	knh_putc(ctx, w, ' '); knh_write__addr(ctx, w, (op->a1));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_SKIP(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_skip_t *op = (klr_skip_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "SKIP");
	knh_putc(ctx, w, ' '); knh_write__addr(ctx, w, (op->a1));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_bJIFT(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_bjift_t *op = (klr_bjift_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "bJIFT");
	knh_putc(ctx, w, ' '); knh_write__addr(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_bJIFF(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_bjiff_t *op = (klr_bjiff_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "bJIFF");
	knh_putc(ctx, w, ' '); knh_write__addr(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_bJIFF_LOOP(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_bjiff_loop_t *op = (klr_bjiff_loop_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "bJIFF_LOOP");
	knh_putc(ctx, w, ' '); knh_write__addr(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_JIFNUL(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_jifnul_t *op = (klr_jifnul_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "JIFNUL");
	knh_putc(ctx, w, ' '); knh_write__addr(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_JIFNN(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_jifnn_t *op = (klr_jifnn_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "JIFNN");
	knh_putc(ctx, w, ' '); knh_write__addr(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_NEXT(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_next_t *op = (klr_next_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "NEXT");
	knh_putc(ctx, w, ' '); knh_write__addr(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_INEXT(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_inext_t *op = (klr_inext_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "INEXT");
	knh_putc(ctx, w, ' '); knh_write__addr(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__cid(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a3));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a4));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_MAPNEXT(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_mapnext_t *op = (klr_mapnext_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "MAPNEXT");
	knh_putc(ctx, w, ' '); knh_write__addr(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__cid(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a3));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a4));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_SMAPNEXT(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_smapnext_t *op = (klr_smapnext_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "SMAPNEXT");
	knh_putc(ctx, w, ' '); knh_write__addr(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a3));
	knh_putc(ctx, w, ' '); knh_write__OBJ(ctx, w, (op->a4));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_TRY(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_try_t *op = (klr_try_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "TRY");
	knh_putc(ctx, w, ' '); knh_write__addr(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_TRYEND(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_tryend_t *op = (klr_tryend_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "TRYEND");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_CATCH(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_catch_t *op = (klr_catch_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "CATCH");
	knh_putc(ctx, w, ' '); knh_write__addr(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a3));
	knh_putc(ctx, w, ' '); knh_write__OBJ(ctx, w, (op->a4));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_PUSH(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_push_t *op = (klr_push_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "PUSH");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_POP(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_pop_t *op = (klr_pop_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "POP");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_THROW(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_throw_t *op = (klr_throw_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "THROW");
	knh_putc(ctx, w, ' '); knh_write__ushort(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_THROWs(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_throws_t *op = (klr_throws_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "THROWs");
	knh_putc(ctx, w, ' '); knh_write__ushort(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__OBJ(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_THROW_AGAIN(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_throw_again_t *op = (klr_throw_again_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "THROW_AGAIN");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_P(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_p_t *op = (klr_p_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "P");
	knh_putc(ctx, w, ' '); knh_write__flag(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__mn(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_PMSG(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_pmsg_t *op = (klr_pmsg_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "PMSG");
	knh_putc(ctx, w, ' '); knh_write__flag(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__OBJ(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_iCAST(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_icast_t *op = (klr_icast_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "iCAST");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_inCAST(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_incast_t *op = (klr_incast_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "inCAST");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_fCAST(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_fcast_t *op = (klr_fcast_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "fCAST");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_fnCAST(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_fncast_t *op = (klr_fncast_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "fnCAST");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_bNOT(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_bnot_t *op = (klr_bnot_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "bNOT");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_iNEG(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_ineg_t *op = (klr_ineg_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "iNEG");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_iADD(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_iadd_t *op = (klr_iadd_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "iADD");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_iADDn(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_iaddn_t *op = (klr_iaddn_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "iADDn");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__integer(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_iSUB(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_isub_t *op = (klr_isub_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "iSUB");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_iSUBn(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_isubn_t *op = (klr_isubn_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "iSUBn");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__integer(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_iMUL(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_imul_t *op = (klr_imul_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "iMUL");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_iMULn(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_imuln_t *op = (klr_imuln_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "iMULn");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__integer(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_iDIV(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_idiv_t *op = (klr_idiv_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "iDIV");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_iDIVn(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_idivn_t *op = (klr_idivn_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "iDIVn");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__integer(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_iMOD(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_imod_t *op = (klr_imod_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "iMOD");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_iMODn(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_imodn_t *op = (klr_imodn_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "iMODn");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__integer(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_iEQ(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_ieq_t *op = (klr_ieq_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "iEQ");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_iEQn(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_ieqn_t *op = (klr_ieqn_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "iEQn");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__integer(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_iNEQ(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_ineq_t *op = (klr_ineq_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "iNEQ");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_iNEQn(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_ineqn_t *op = (klr_ineqn_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "iNEQn");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__integer(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_iLT(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_ilt_t *op = (klr_ilt_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "iLT");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_iLTn(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_iltn_t *op = (klr_iltn_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "iLTn");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__integer(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_iLTE(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_ilte_t *op = (klr_ilte_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "iLTE");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_iLTEn(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_ilten_t *op = (klr_ilten_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "iLTEn");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__integer(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_iGT(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_igt_t *op = (klr_igt_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "iGT");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_iGTn(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_igtn_t *op = (klr_igtn_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "iGTn");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__integer(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_iGTE(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_igte_t *op = (klr_igte_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "iGTE");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_iGTEn(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_igten_t *op = (klr_igten_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "iGTEn");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__integer(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_fNEG(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_fneg_t *op = (klr_fneg_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "fNEG");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_fADD(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_fadd_t *op = (klr_fadd_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "fADD");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_fADDn(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_faddn_t *op = (klr_faddn_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "fADDn");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__float(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_fSUB(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_fsub_t *op = (klr_fsub_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "fSUB");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_fSUBn(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_fsubn_t *op = (klr_fsubn_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "fSUBn");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__float(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_fMUL(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_fmul_t *op = (klr_fmul_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "fMUL");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_fMULn(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_fmuln_t *op = (klr_fmuln_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "fMULn");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__float(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_fDIV(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_fdiv_t *op = (klr_fdiv_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "fDIV");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_fDIVn(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_fdivn_t *op = (klr_fdivn_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "fDIVn");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__float(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_fEQ(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_feq_t *op = (klr_feq_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "fEQ");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_fEQn(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_feqn_t *op = (klr_feqn_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "fEQn");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__float(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_fNEQ(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_fneq_t *op = (klr_fneq_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "fNEQ");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_fNEQn(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_fneqn_t *op = (klr_fneqn_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "fNEQn");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__float(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_fLT(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_flt_t *op = (klr_flt_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "fLT");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_fLTn(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_fltn_t *op = (klr_fltn_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "fLTn");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__float(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_fLTE(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_flte_t *op = (klr_flte_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "fLTE");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_fLTEn(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_flten_t *op = (klr_flten_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "fLTEn");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__float(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_fGT(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_fgt_t *op = (klr_fgt_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "fGT");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_fGTn(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_fgtn_t *op = (klr_fgtn_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "fGTn");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__float(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_fGTE(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_fgte_t *op = (klr_fgte_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "fGTE");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_fGTEn(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_fgten_t *op = (klr_fgten_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "fGTEn");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__float(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_AGET(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_aget_t *op = (klr_aget_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "AGET");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_AGETn(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_agetn_t *op = (klr_agetn_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "AGETn");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__intptr(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_IAGET(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_iaget_t *op = (klr_iaget_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "IAGET");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_IAGETn(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_iagetn_t *op = (klr_iagetn_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "IAGETn");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__intptr(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_FAGET(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_faget_t *op = (klr_faget_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "FAGET");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_FAGETn(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_fagetn_t *op = (klr_fagetn_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "FAGETn");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__intptr(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_ASET(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_aset_t *op = (klr_aset_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "ASET");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_ASETn(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_asetn_t *op = (klr_asetn_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "ASETn");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__intptr(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_IASET(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_iaset_t *op = (klr_iaset_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "IASET");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_IASETn(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_iasetn_t *op = (klr_iasetn_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "IASETn");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__intptr(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_FASET(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_faset_t *op = (klr_faset_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "FASET");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_FASETn(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_fasetn_t *op = (klr_fasetn_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "FASETn");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a2));
	knh_putc(ctx, w, ' '); knh_write__intptr(ctx, w, (op->a3));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_INITCODE(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	klr_initcode_t *op = (klr_initcode_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "INITCODE");
	knh_putc(ctx, w, ' '); knh_write__sfi(ctx, w, (op->a1));
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
void KNH_DUMP_NOP(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	//klr_nop_t *op = (klr_nop_t*)pc;
	KNH_DUMP_OPCODE(ctx, pc, w, mtd, "NOP");
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------ */

typedef void (*fdump)(Ctx *ctx, knh_code_t *pc, int flag, knh_OutputStream_t *w, knh_Method_t *mtd);

static fdump OPDUMP[] = {
	KNH_DUMP_HALT,
	KNH_DUMP_MOVa,
	KNH_DUMP_MOVn,
	KNH_DUMP_MOVo,
	KNH_DUMP_MOVx,
	KNH_DUMP_MOVDEF,
	KNH_DUMP_MOVSYS,
	KNH_DUMP_XMOVs,
	KNH_DUMP_XMOVo,
	KNH_DUMP_XMOVx,
	KNH_DUMP_XMOVDEF,
	KNH_DUMP_XMOVSYS,
	KNH_DUMP_MOVxi,
	KNH_DUMP_XMOVsi,
	KNH_DUMP_XMOVoi,
	KNH_DUMP_XMOVxi,
	KNH_DUMP_XMOVxio,
	KNH_DUMP_XMOVxBXi,
	KNH_DUMP_MOVxf,
	KNH_DUMP_XMOVsf,
	KNH_DUMP_XMOVof,
	KNH_DUMP_XMOVxf,
	KNH_DUMP_XMOVxfo,
	KNH_DUMP_XMOVxBXf,
	KNH_DUMP_MOVxb,
	KNH_DUMP_XMOVsb,
	KNH_DUMP_XMOVob,
	KNH_DUMP_XMOVxb,
	KNH_DUMP_EMOVs,
	KNH_DUMP_EMOVo,
	KNH_DUMP_EMOVe,
	KNH_DUMP_EMOVDEF,
	KNH_DUMP_EMOVSYS,
	KNH_DUMP_SWAP,
	KNH_DUMP_PINIDEF,
	KNH_DUMP_PINIo,
	KNH_DUMP_PARAMS,
	KNH_DUMP_SETESP,
	KNH_DUMP_CHECKESP,
	KNH_DUMP_RET,
	KNH_DUMP_RETn,
	KNH_DUMP_RETa,
	KNH_DUMP_RETo,
	KNH_DUMP_RETx,
	KNH_DUMP_YEILDBREAK,
	KNH_DUMP_BOX,
	KNH_DUMP_BOXnc,
	KNH_DUMP_NNBOX,
	KNH_DUMP_NNBOXnc,
	KNH_DUMP_UNBOX,
	KNH_DUMP_CHECKNULL,
	KNH_DUMP_CHECKNULLx,
	KNH_DUMP_CHECKTYPE,
	KNH_DUMP_CHECKNNTYPE,
	KNH_DUMP_FCALL,
	KNH_DUMP_SCALL,
	KNH_DUMP_AINVOKE,
	KNH_DUMP_CALL,
	KNH_DUMP_ACALL,
	KNH_DUMP_NEW,
	KNH_DUMP_COPYSFP,
	KNH_DUMP_TOSTR,
	KNH_DUMP_TOSTRf,
	KNH_DUMP_SMAP,
	KNH_DUMP_SMAPnc,
	KNH_DUMP_MAP,
	KNH_DUMP_MAPnc,
	KNH_DUMP_AMAP,
	KNH_DUMP_NNMAP,
	KNH_DUMP_JMP,
	KNH_DUMP_SKIP,
	KNH_DUMP_bJIFT,
	KNH_DUMP_bJIFF,
	KNH_DUMP_bJIFF_LOOP,
	KNH_DUMP_JIFNUL,
	KNH_DUMP_JIFNN,
	KNH_DUMP_NEXT,
	KNH_DUMP_INEXT,
	KNH_DUMP_MAPNEXT,
	KNH_DUMP_SMAPNEXT,
	KNH_DUMP_TRY,
	KNH_DUMP_TRYEND,
	KNH_DUMP_CATCH,
	KNH_DUMP_PUSH,
	KNH_DUMP_POP,
	KNH_DUMP_THROW,
	KNH_DUMP_THROWs,
	KNH_DUMP_THROW_AGAIN,
	KNH_DUMP_P,
	KNH_DUMP_PMSG,
	KNH_DUMP_iCAST,
	KNH_DUMP_inCAST,
	KNH_DUMP_fCAST,
	KNH_DUMP_fnCAST,
	KNH_DUMP_bNOT,
	KNH_DUMP_iNEG,
	KNH_DUMP_iADD,
	KNH_DUMP_iADDn,
	KNH_DUMP_iSUB,
	KNH_DUMP_iSUBn,
	KNH_DUMP_iMUL,
	KNH_DUMP_iMULn,
	KNH_DUMP_iDIV,
	KNH_DUMP_iDIVn,
	KNH_DUMP_iMOD,
	KNH_DUMP_iMODn,
	KNH_DUMP_iEQ,
	KNH_DUMP_iEQn,
	KNH_DUMP_iNEQ,
	KNH_DUMP_iNEQn,
	KNH_DUMP_iLT,
	KNH_DUMP_iLTn,
	KNH_DUMP_iLTE,
	KNH_DUMP_iLTEn,
	KNH_DUMP_iGT,
	KNH_DUMP_iGTn,
	KNH_DUMP_iGTE,
	KNH_DUMP_iGTEn,
	KNH_DUMP_fNEG,
	KNH_DUMP_fADD,
	KNH_DUMP_fADDn,
	KNH_DUMP_fSUB,
	KNH_DUMP_fSUBn,
	KNH_DUMP_fMUL,
	KNH_DUMP_fMULn,
	KNH_DUMP_fDIV,
	KNH_DUMP_fDIVn,
	KNH_DUMP_fEQ,
	KNH_DUMP_fEQn,
	KNH_DUMP_fNEQ,
	KNH_DUMP_fNEQn,
	KNH_DUMP_fLT,
	KNH_DUMP_fLTn,
	KNH_DUMP_fLTE,
	KNH_DUMP_fLTEn,
	KNH_DUMP_fGT,
	KNH_DUMP_fGTn,
	KNH_DUMP_fGTE,
	KNH_DUMP_fGTEn,
	KNH_DUMP_AGET,
	KNH_DUMP_AGETn,
	KNH_DUMP_IAGET,
	KNH_DUMP_IAGETn,
	KNH_DUMP_FAGET,
	KNH_DUMP_FAGETn,
	KNH_DUMP_ASET,
	KNH_DUMP_ASETn,
	KNH_DUMP_IASET,
	KNH_DUMP_IASETn,
	KNH_DUMP_FASET,
	KNH_DUMP_FASETn,
	KNH_DUMP_INITCODE,
	KNH_DUMP_NOP,
};

/* ------------------------------------------------------------------------ */

void knh_KLRCode__dumpmtd(Ctx *ctx, knh_KLRCode_t *o, knh_OutputStream_t *w, knh_Method_t *mtd)
{
	knh_bytes_t vmc = knh_KLRCode_tobytes(o);
	knh_code_t *pc = (knh_code_t*)vmc.buf;
	while(1) {
		int opcode = KNH_OPCODE(pc);
		if(opcode == OPCODE_HALT) return;
		KNH_ASSERT_OPCODE(opcode);
		OPDUMP[opcode](ctx, pc, 0, w, mtd);
		pc += OPSIZE[opcode];
	}
}

/* ------------------------------------------------------------------------ */


/* ======================================================================== */
/* [exec] */

/* ------------------------------------------------------------------------ */

#ifndef DBG2_OPDUMP
#define DBG2_OPDUMP(ctx, pc)
#endif

static
void knh_code_thread(Ctx *ctx, knh_code_t *pc, void **codeaddr)
{
#ifdef KNH_USING_THREADEDCODE
	while(1) {
		knh_kode_t *op = (knh_kode_t*)pc;
		KNH_ASSERT_OPCODE(op->opcode);
		//DBG2_P("%p opcode=%d next=%p", op, op->opcode, op->nextaddr);
		op->codeaddr = codeaddr[op->opcode];
		if(op->opcode == OPCODE_HALT) {
			return;
		}
		switch(op->opcode) {
		case OPCODE_JMP : 
		case OPCODE_SKIP : 
		case OPCODE_BJIFT : 
		case OPCODE_BJIFF : 
		case OPCODE_BJIFF_LOOP : 
		case OPCODE_JIFNUL : 
		case OPCODE_JIFNN : 
		case OPCODE_NEXT : 
		case OPCODE_INEXT : 
		case OPCODE_MAPNEXT : 
		case OPCODE_SMAPNEXT : 
		case OPCODE_TRY : 
		case OPCODE_CATCH : 
			DBG2_P("%p GOTO %p", op, op->jumppc);
			op->jumpaddr = codeaddr[((knh_kode_t*)(op->jumppc))->opcode];
			break;
		}
		pc += OPSIZE[op->opcode];
	}
#endif/*KNH_USING_THREADEDCODE*/
}


#ifndef KNH_USING_THREADEDCODE
#define OPJUMP NULL
METHOD knh_KLRCode_exec(Ctx *ctx, knh_sfp_t *sfp)
{
	static void **globalMachineToJump = NULL;
	register knh_code_t *pc = sfp[-1].pc;
	L_HEAD:;
	switch(((knh_kode_t*)pc)->opcode) {

	case OPCODE_HALT :
		{
			//const klr_halt_t* op = (klr_halt_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_HALT(ctx);
			pc += OPSIZE_HALT;
			goto L_HEAD;
		}

	case OPCODE_MOVA :
		{
			const klr_mova_t* op = (klr_mova_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_MOVa(ctx, op->a1, op->a2);
			pc += OPSIZE_MOVA;
			goto L_HEAD;
		}

	case OPCODE_MOVN :
		{
			const klr_movn_t* op = (klr_movn_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_MOVn(ctx, op->a1, op->a2);
			pc += OPSIZE_MOVN;
			goto L_HEAD;
		}

	case OPCODE_MOVO :
		{
			const klr_movo_t* op = (klr_movo_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_MOVo(ctx, op->a1, op->a2);
			pc += OPSIZE_MOVO;
			goto L_HEAD;
		}

	case OPCODE_MOVX :
		{
			const klr_movx_t* op = (klr_movx_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_MOVx(ctx, op->a1, op->a2);
			pc += OPSIZE_MOVX;
			goto L_HEAD;
		}

	case OPCODE_MOVDEF :
		{
			const klr_movdef_t* op = (klr_movdef_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_MOVDEF(ctx, op->a1, op->a2);
			pc += OPSIZE_MOVDEF;
			goto L_HEAD;
		}

	case OPCODE_MOVSYS :
		{
			const klr_movsys_t* op = (klr_movsys_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_MOVSYS(ctx, op->a1, op->a2);
			pc += OPSIZE_MOVSYS;
			goto L_HEAD;
		}

	case OPCODE_XMOVS :
		{
			const klr_xmovs_t* op = (klr_xmovs_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_XMOVs(ctx, op->a1, op->a2);
			pc += OPSIZE_XMOVS;
			goto L_HEAD;
		}

	case OPCODE_XMOVO :
		{
			const klr_xmovo_t* op = (klr_xmovo_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_XMOVo(ctx, op->a1, op->a2);
			pc += OPSIZE_XMOVO;
			goto L_HEAD;
		}

	case OPCODE_XMOVX :
		{
			const klr_xmovx_t* op = (klr_xmovx_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_XMOVx(ctx, op->a1, op->a2);
			pc += OPSIZE_XMOVX;
			goto L_HEAD;
		}

	case OPCODE_XMOVDEF :
		{
			const klr_xmovdef_t* op = (klr_xmovdef_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_XMOVDEF(ctx, op->a1, op->a2);
			pc += OPSIZE_XMOVDEF;
			goto L_HEAD;
		}

	case OPCODE_XMOVSYS :
		{
			const klr_xmovsys_t* op = (klr_xmovsys_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_XMOVSYS(ctx, op->a1, op->a2);
			pc += OPSIZE_XMOVSYS;
			goto L_HEAD;
		}

	case OPCODE_MOVXI :
		{
			const klr_movxi_t* op = (klr_movxi_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_MOVxi(ctx, op->a1, op->a2);
			pc += OPSIZE_MOVXI;
			goto L_HEAD;
		}

	case OPCODE_XMOVSI :
		{
			const klr_xmovsi_t* op = (klr_xmovsi_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_XMOVsi(ctx, op->a1, op->a2);
			pc += OPSIZE_XMOVSI;
			goto L_HEAD;
		}

	case OPCODE_XMOVOI :
		{
			const klr_xmovoi_t* op = (klr_xmovoi_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_XMOVoi(ctx, op->a1, op->a2);
			pc += OPSIZE_XMOVOI;
			goto L_HEAD;
		}

	case OPCODE_XMOVXI :
		{
			const klr_xmovxi_t* op = (klr_xmovxi_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_XMOVxi(ctx, op->a1, op->a2);
			pc += OPSIZE_XMOVXI;
			goto L_HEAD;
		}

	case OPCODE_XMOVXIO :
		{
			const klr_xmovxio_t* op = (klr_xmovxio_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_XMOVxio(ctx, op->a1, op->a2);
			pc += OPSIZE_XMOVXIO;
			goto L_HEAD;
		}

	case OPCODE_XMOVXBXI :
		{
			const klr_xmovxbxi_t* op = (klr_xmovxbxi_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_XMOVxBXi(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_XMOVXBXI;
			goto L_HEAD;
		}

	case OPCODE_MOVXF :
		{
			const klr_movxf_t* op = (klr_movxf_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_MOVxf(ctx, op->a1, op->a2);
			pc += OPSIZE_MOVXF;
			goto L_HEAD;
		}

	case OPCODE_XMOVSF :
		{
			const klr_xmovsf_t* op = (klr_xmovsf_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_XMOVsf(ctx, op->a1, op->a2);
			pc += OPSIZE_XMOVSF;
			goto L_HEAD;
		}

	case OPCODE_XMOVOF :
		{
			const klr_xmovof_t* op = (klr_xmovof_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_XMOVof(ctx, op->a1, op->a2);
			pc += OPSIZE_XMOVOF;
			goto L_HEAD;
		}

	case OPCODE_XMOVXF :
		{
			const klr_xmovxf_t* op = (klr_xmovxf_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_XMOVxf(ctx, op->a1, op->a2);
			pc += OPSIZE_XMOVXF;
			goto L_HEAD;
		}

	case OPCODE_XMOVXFO :
		{
			const klr_xmovxfo_t* op = (klr_xmovxfo_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_XMOVxfo(ctx, op->a1, op->a2);
			pc += OPSIZE_XMOVXFO;
			goto L_HEAD;
		}

	case OPCODE_XMOVXBXF :
		{
			const klr_xmovxbxf_t* op = (klr_xmovxbxf_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_XMOVxBXf(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_XMOVXBXF;
			goto L_HEAD;
		}

	case OPCODE_MOVXB :
		{
			const klr_movxb_t* op = (klr_movxb_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_MOVxb(ctx, op->a1, op->a2);
			pc += OPSIZE_MOVXB;
			goto L_HEAD;
		}

	case OPCODE_XMOVSB :
		{
			const klr_xmovsb_t* op = (klr_xmovsb_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_XMOVsb(ctx, op->a1, op->a2);
			pc += OPSIZE_XMOVSB;
			goto L_HEAD;
		}

	case OPCODE_XMOVOB :
		{
			const klr_xmovob_t* op = (klr_xmovob_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_XMOVob(ctx, op->a1, op->a2);
			pc += OPSIZE_XMOVOB;
			goto L_HEAD;
		}

	case OPCODE_XMOVXB :
		{
			const klr_xmovxb_t* op = (klr_xmovxb_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_XMOVxb(ctx, op->a1, op->a2);
			pc += OPSIZE_XMOVXB;
			goto L_HEAD;
		}

	case OPCODE_EMOVS :
		{
			const klr_emovs_t* op = (klr_emovs_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_EMOVs(ctx, op->a1, op->a2);
			pc += OPSIZE_EMOVS;
			goto L_HEAD;
		}

	case OPCODE_EMOVO :
		{
			const klr_emovo_t* op = (klr_emovo_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_EMOVo(ctx, op->a1, op->a2);
			pc += OPSIZE_EMOVO;
			goto L_HEAD;
		}

	case OPCODE_EMOVE :
		{
			const klr_emove_t* op = (klr_emove_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_EMOVe(ctx, op->a1, op->a2);
			pc += OPSIZE_EMOVE;
			goto L_HEAD;
		}

	case OPCODE_EMOVDEF :
		{
			const klr_emovdef_t* op = (klr_emovdef_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_EMOVDEF(ctx, op->a1, op->a2);
			pc += OPSIZE_EMOVDEF;
			goto L_HEAD;
		}

	case OPCODE_EMOVSYS :
		{
			const klr_emovsys_t* op = (klr_emovsys_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_EMOVSYS(ctx, op->a1, op->a2);
			pc += OPSIZE_EMOVSYS;
			goto L_HEAD;
		}

	case OPCODE_SWAP :
		{
			const klr_swap_t* op = (klr_swap_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_SWAP(ctx, op->a1, op->a2);
			pc += OPSIZE_SWAP;
			goto L_HEAD;
		}

	case OPCODE_PINIDEF :
		{
			const klr_pinidef_t* op = (klr_pinidef_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_PINIDEF(ctx, op->a1, op->a2);
			pc += OPSIZE_PINIDEF;
			goto L_HEAD;
		}

	case OPCODE_PINIO :
		{
			const klr_pinio_t* op = (klr_pinio_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_PINIo(ctx, op->a1, op->a2);
			pc += OPSIZE_PINIO;
			goto L_HEAD;
		}

	case OPCODE_PARAMS :
		{
			const klr_params_t* op = (klr_params_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_PARAMS(ctx, op->a1, op->a2);
			pc += OPSIZE_PARAMS;
			goto L_HEAD;
		}

	case OPCODE_SETESP :
		{
			const klr_setesp_t* op = (klr_setesp_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_SETESP(ctx, op->a1);
			pc += OPSIZE_SETESP;
			goto L_HEAD;
		}

	case OPCODE_CHECKESP :
		{
			const klr_checkesp_t* op = (klr_checkesp_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_CHECKESP(ctx, op->a1);
			pc += OPSIZE_CHECKESP;
			goto L_HEAD;
		}

	case OPCODE_RET :
		{
			//const klr_ret_t* op = (klr_ret_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_RET(ctx);
			pc += OPSIZE_RET;
			goto L_HEAD;
		}

	case OPCODE_RETN :
		{
			const klr_retn_t* op = (klr_retn_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_RETn(ctx, op->a1, op->a2);
			pc += OPSIZE_RETN;
			goto L_HEAD;
		}

	case OPCODE_RETA :
		{
			const klr_reta_t* op = (klr_reta_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_RETa(ctx, op->a1, op->a2);
			pc += OPSIZE_RETA;
			goto L_HEAD;
		}

	case OPCODE_RETO :
		{
			const klr_reto_t* op = (klr_reto_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_RETo(ctx, op->a1, op->a2);
			pc += OPSIZE_RETO;
			goto L_HEAD;
		}

	case OPCODE_RETX :
		{
			const klr_retx_t* op = (klr_retx_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_RETx(ctx, op->a1, op->a2);
			pc += OPSIZE_RETX;
			goto L_HEAD;
		}

	case OPCODE_YEILDBREAK :
		{
			//const klr_yeildbreak_t* op = (klr_yeildbreak_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_YEILDBREAK(ctx);
			pc += OPSIZE_YEILDBREAK;
			goto L_HEAD;
		}

	case OPCODE_BOX :
		{
			const klr_box_t* op = (klr_box_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_BOX(ctx, op->a1, op->a2);
			pc += OPSIZE_BOX;
			goto L_HEAD;
		}

	case OPCODE_BOXNC :
		{
			const klr_boxnc_t* op = (klr_boxnc_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_BOXnc(ctx, op->a1, op->a2);
			pc += OPSIZE_BOXNC;
			goto L_HEAD;
		}

	case OPCODE_NNBOX :
		{
			const klr_nnbox_t* op = (klr_nnbox_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_NNBOX(ctx, op->a1, op->a2);
			pc += OPSIZE_NNBOX;
			goto L_HEAD;
		}

	case OPCODE_NNBOXNC :
		{
			const klr_nnboxnc_t* op = (klr_nnboxnc_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_NNBOXnc(ctx, op->a1, op->a2);
			pc += OPSIZE_NNBOXNC;
			goto L_HEAD;
		}

	case OPCODE_UNBOX :
		{
			const klr_unbox_t* op = (klr_unbox_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_UNBOX(ctx, op->a1);
			pc += OPSIZE_UNBOX;
			goto L_HEAD;
		}

	case OPCODE_CHECKNULL :
		{
			const klr_checknull_t* op = (klr_checknull_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_CHECKNULL(ctx, op->a1);
			pc += OPSIZE_CHECKNULL;
			goto L_HEAD;
		}

	case OPCODE_CHECKNULLX :
		{
			const klr_checknullx_t* op = (klr_checknullx_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_CHECKNULLx(ctx, op->a1);
			pc += OPSIZE_CHECKNULLX;
			goto L_HEAD;
		}

	case OPCODE_CHECKTYPE :
		{
			const klr_checktype_t* op = (klr_checktype_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_CHECKTYPE(ctx, op->a1, op->a2);
			pc += OPSIZE_CHECKTYPE;
			goto L_HEAD;
		}

	case OPCODE_CHECKNNTYPE :
		{
			const klr_checknntype_t* op = (klr_checknntype_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_CHECKNNTYPE(ctx, op->a1, op->a2);
			pc += OPSIZE_CHECKNNTYPE;
			goto L_HEAD;
		}

	case OPCODE_FCALL :
		{
			const klr_fcall_t* op = (klr_fcall_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_FCALL(ctx, op->a1, op->a2, op->a3, op->a4);
			pc += OPSIZE_FCALL;
			goto L_HEAD;
		}

	case OPCODE_SCALL :
		{
			const klr_scall_t* op = (klr_scall_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_SCALL(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_SCALL;
			goto L_HEAD;
		}

	case OPCODE_AINVOKE :
		{
			const klr_ainvoke_t* op = (klr_ainvoke_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_AINVOKE(ctx, op->a1, op->a2);
			pc += OPSIZE_AINVOKE;
			goto L_HEAD;
		}

	case OPCODE_CALL :
		{
			const klr_call_t* op = (klr_call_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_CALL(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_CALL;
			goto L_HEAD;
		}

	case OPCODE_ACALL :
		{
			const klr_acall_t* op = (klr_acall_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_ACALL(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_ACALL;
			goto L_HEAD;
		}

	case OPCODE_NEW :
		{
			const klr_new_t* op = (klr_new_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_NEW(ctx, op->a1, op->a2, op->a3, op->a4, op->a5);
			pc += OPSIZE_NEW;
			goto L_HEAD;
		}

	case OPCODE_COPYSFP :
		{
			const klr_copysfp_t* op = (klr_copysfp_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_COPYSFP(ctx, op->a1);
			pc += OPSIZE_COPYSFP;
			goto L_HEAD;
		}

	case OPCODE_TOSTR :
		{
			const klr_tostr_t* op = (klr_tostr_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_TOSTR(ctx, op->a1, op->a2);
			pc += OPSIZE_TOSTR;
			goto L_HEAD;
		}

	case OPCODE_TOSTRF :
		{
			const klr_tostrf_t* op = (klr_tostrf_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_TOSTRf(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_TOSTRF;
			goto L_HEAD;
		}

	case OPCODE_SMAP :
		{
			const klr_smap_t* op = (klr_smap_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_SMAP(ctx, op->a1, op->a2);
			pc += OPSIZE_SMAP;
			goto L_HEAD;
		}

	case OPCODE_SMAPNC :
		{
			const klr_smapnc_t* op = (klr_smapnc_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_SMAPnc(ctx, op->a1, op->a2);
			pc += OPSIZE_SMAPNC;
			goto L_HEAD;
		}

	case OPCODE_MAP :
		{
			const klr_map_t* op = (klr_map_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_MAP(ctx, op->a1, op->a2);
			pc += OPSIZE_MAP;
			goto L_HEAD;
		}

	case OPCODE_MAPNC :
		{
			const klr_mapnc_t* op = (klr_mapnc_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_MAPnc(ctx, op->a1, op->a2);
			pc += OPSIZE_MAPNC;
			goto L_HEAD;
		}

	case OPCODE_AMAP :
		{
			const klr_amap_t* op = (klr_amap_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_AMAP(ctx, op->a1, op->a2);
			pc += OPSIZE_AMAP;
			goto L_HEAD;
		}

	case OPCODE_NNMAP :
		{
			const klr_nnmap_t* op = (klr_nnmap_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_NNMAP(ctx, op->a1, op->a2);
			pc += OPSIZE_NNMAP;
			goto L_HEAD;
		}

	case OPCODE_JMP :
		{
			const klr_jmp_t* op = (klr_jmp_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_JMP(ctx, pc = op->jumppc, L_HEAD);
			pc += OPSIZE_JMP;
			goto L_HEAD;
		}

	case OPCODE_SKIP :
		{
			const klr_skip_t* op = (klr_skip_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_SKIP(ctx, pc = op->jumppc, L_HEAD);
			pc += OPSIZE_SKIP;
			goto L_HEAD;
		}

	case OPCODE_BJIFT :
		{
			const klr_bjift_t* op = (klr_bjift_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_bJIFT(ctx, pc = op->jumppc, L_HEAD, op->a2);
			pc += OPSIZE_BJIFT;
			goto L_HEAD;
		}

	case OPCODE_BJIFF :
		{
			const klr_bjiff_t* op = (klr_bjiff_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_bJIFF(ctx, pc = op->jumppc, L_HEAD, op->a2);
			pc += OPSIZE_BJIFF;
			goto L_HEAD;
		}

	case OPCODE_BJIFF_LOOP :
		{
			const klr_bjiff_loop_t* op = (klr_bjiff_loop_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_bJIFF_LOOP(ctx, pc = op->jumppc, L_HEAD, op->a2);
			pc += OPSIZE_BJIFF_LOOP;
			goto L_HEAD;
		}

	case OPCODE_JIFNUL :
		{
			const klr_jifnul_t* op = (klr_jifnul_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_JIFNUL(ctx, pc = op->jumppc, L_HEAD, op->a2);
			pc += OPSIZE_JIFNUL;
			goto L_HEAD;
		}

	case OPCODE_JIFNN :
		{
			const klr_jifnn_t* op = (klr_jifnn_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_JIFNN(ctx, pc = op->jumppc, L_HEAD, op->a2);
			pc += OPSIZE_JIFNN;
			goto L_HEAD;
		}

	case OPCODE_NEXT :
		{
			const klr_next_t* op = (klr_next_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_NEXT(ctx, pc = op->jumppc, L_HEAD, op->a2, op->a3);
			pc += OPSIZE_NEXT;
			goto L_HEAD;
		}

	case OPCODE_INEXT :
		{
			const klr_inext_t* op = (klr_inext_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_INEXT(ctx, pc = op->jumppc, L_HEAD, op->a2, op->a3, op->a4);
			pc += OPSIZE_INEXT;
			goto L_HEAD;
		}

	case OPCODE_MAPNEXT :
		{
			const klr_mapnext_t* op = (klr_mapnext_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_MAPNEXT(ctx, pc = op->jumppc, L_HEAD, op->a2, op->a3, op->a4);
			pc += OPSIZE_MAPNEXT;
			goto L_HEAD;
		}

	case OPCODE_SMAPNEXT :
		{
			const klr_smapnext_t* op = (klr_smapnext_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_SMAPNEXT(ctx, pc = op->jumppc, L_HEAD, op->a2, op->a3, op->a4);
			pc += OPSIZE_SMAPNEXT;
			goto L_HEAD;
		}

	case OPCODE_TRY :
		{
			const klr_try_t* op = (klr_try_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_TRY(ctx, pc = op->jumppc, L_HEAD, op->a2);
			pc += OPSIZE_TRY;
			goto L_HEAD;
		}

	case OPCODE_TRYEND :
		{
			const klr_tryend_t* op = (klr_tryend_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_TRYEND(ctx, op->a1);
			pc += OPSIZE_TRYEND;
			goto L_HEAD;
		}

	case OPCODE_CATCH :
		{
			const klr_catch_t* op = (klr_catch_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_CATCH(ctx, pc = op->jumppc, L_HEAD, op->a2, op->a3, op->a4);
			pc += OPSIZE_CATCH;
			goto L_HEAD;
		}

	case OPCODE_PUSH :
		{
			const klr_push_t* op = (klr_push_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_PUSH(ctx, op->a1);
			pc += OPSIZE_PUSH;
			goto L_HEAD;
		}

	case OPCODE_POP :
		{
			const klr_pop_t* op = (klr_pop_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_POP(ctx, op->a1);
			pc += OPSIZE_POP;
			goto L_HEAD;
		}

	case OPCODE_THROW :
		{
			const klr_throw_t* op = (klr_throw_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_THROW(ctx, op->a1, op->a2);
			pc += OPSIZE_THROW;
			goto L_HEAD;
		}

	case OPCODE_THROWS :
		{
			const klr_throws_t* op = (klr_throws_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_THROWs(ctx, op->a1, op->a2);
			pc += OPSIZE_THROWS;
			goto L_HEAD;
		}

	case OPCODE_THROW_AGAIN :
		{
			const klr_throw_again_t* op = (klr_throw_again_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_THROW_AGAIN(ctx, op->a1);
			pc += OPSIZE_THROW_AGAIN;
			goto L_HEAD;
		}

	case OPCODE_P :
		{
			const klr_p_t* op = (klr_p_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_P(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_P;
			goto L_HEAD;
		}

	case OPCODE_PMSG :
		{
			const klr_pmsg_t* op = (klr_pmsg_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_PMSG(ctx, op->a1, op->a2);
			pc += OPSIZE_PMSG;
			goto L_HEAD;
		}

	case OPCODE_ICAST :
		{
			const klr_icast_t* op = (klr_icast_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_iCAST(ctx, op->a1);
			pc += OPSIZE_ICAST;
			goto L_HEAD;
		}

	case OPCODE_INCAST :
		{
			const klr_incast_t* op = (klr_incast_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_inCAST(ctx, op->a1);
			pc += OPSIZE_INCAST;
			goto L_HEAD;
		}

	case OPCODE_FCAST :
		{
			const klr_fcast_t* op = (klr_fcast_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_fCAST(ctx, op->a1);
			pc += OPSIZE_FCAST;
			goto L_HEAD;
		}

	case OPCODE_FNCAST :
		{
			const klr_fncast_t* op = (klr_fncast_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_fnCAST(ctx, op->a1);
			pc += OPSIZE_FNCAST;
			goto L_HEAD;
		}

	case OPCODE_BNOT :
		{
			const klr_bnot_t* op = (klr_bnot_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_bNOT(ctx, op->a1, op->a2);
			pc += OPSIZE_BNOT;
			goto L_HEAD;
		}

	case OPCODE_INEG :
		{
			const klr_ineg_t* op = (klr_ineg_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_iNEG(ctx, op->a1, op->a2);
			pc += OPSIZE_INEG;
			goto L_HEAD;
		}

	case OPCODE_IADD :
		{
			const klr_iadd_t* op = (klr_iadd_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_iADD(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_IADD;
			goto L_HEAD;
		}

	case OPCODE_IADDN :
		{
			const klr_iaddn_t* op = (klr_iaddn_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_iADDn(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_IADDN;
			goto L_HEAD;
		}

	case OPCODE_ISUB :
		{
			const klr_isub_t* op = (klr_isub_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_iSUB(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_ISUB;
			goto L_HEAD;
		}

	case OPCODE_ISUBN :
		{
			const klr_isubn_t* op = (klr_isubn_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_iSUBn(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_ISUBN;
			goto L_HEAD;
		}

	case OPCODE_IMUL :
		{
			const klr_imul_t* op = (klr_imul_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_iMUL(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_IMUL;
			goto L_HEAD;
		}

	case OPCODE_IMULN :
		{
			const klr_imuln_t* op = (klr_imuln_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_iMULn(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_IMULN;
			goto L_HEAD;
		}

	case OPCODE_IDIV :
		{
			const klr_idiv_t* op = (klr_idiv_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_iDIV(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_IDIV;
			goto L_HEAD;
		}

	case OPCODE_IDIVN :
		{
			const klr_idivn_t* op = (klr_idivn_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_iDIVn(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_IDIVN;
			goto L_HEAD;
		}

	case OPCODE_IMOD :
		{
			const klr_imod_t* op = (klr_imod_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_iMOD(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_IMOD;
			goto L_HEAD;
		}

	case OPCODE_IMODN :
		{
			const klr_imodn_t* op = (klr_imodn_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_iMODn(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_IMODN;
			goto L_HEAD;
		}

	case OPCODE_IEQ :
		{
			const klr_ieq_t* op = (klr_ieq_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_iEQ(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_IEQ;
			goto L_HEAD;
		}

	case OPCODE_IEQN :
		{
			const klr_ieqn_t* op = (klr_ieqn_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_iEQn(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_IEQN;
			goto L_HEAD;
		}

	case OPCODE_INEQ :
		{
			const klr_ineq_t* op = (klr_ineq_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_iNEQ(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_INEQ;
			goto L_HEAD;
		}

	case OPCODE_INEQN :
		{
			const klr_ineqn_t* op = (klr_ineqn_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_iNEQn(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_INEQN;
			goto L_HEAD;
		}

	case OPCODE_ILT :
		{
			const klr_ilt_t* op = (klr_ilt_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_iLT(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_ILT;
			goto L_HEAD;
		}

	case OPCODE_ILTN :
		{
			const klr_iltn_t* op = (klr_iltn_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_iLTn(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_ILTN;
			goto L_HEAD;
		}

	case OPCODE_ILTE :
		{
			const klr_ilte_t* op = (klr_ilte_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_iLTE(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_ILTE;
			goto L_HEAD;
		}

	case OPCODE_ILTEN :
		{
			const klr_ilten_t* op = (klr_ilten_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_iLTEn(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_ILTEN;
			goto L_HEAD;
		}

	case OPCODE_IGT :
		{
			const klr_igt_t* op = (klr_igt_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_iGT(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_IGT;
			goto L_HEAD;
		}

	case OPCODE_IGTN :
		{
			const klr_igtn_t* op = (klr_igtn_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_iGTn(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_IGTN;
			goto L_HEAD;
		}

	case OPCODE_IGTE :
		{
			const klr_igte_t* op = (klr_igte_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_iGTE(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_IGTE;
			goto L_HEAD;
		}

	case OPCODE_IGTEN :
		{
			const klr_igten_t* op = (klr_igten_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_iGTEn(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_IGTEN;
			goto L_HEAD;
		}

	case OPCODE_FNEG :
		{
			const klr_fneg_t* op = (klr_fneg_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_fNEG(ctx, op->a1, op->a2);
			pc += OPSIZE_FNEG;
			goto L_HEAD;
		}

	case OPCODE_FADD :
		{
			const klr_fadd_t* op = (klr_fadd_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_fADD(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_FADD;
			goto L_HEAD;
		}

	case OPCODE_FADDN :
		{
			const klr_faddn_t* op = (klr_faddn_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_fADDn(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_FADDN;
			goto L_HEAD;
		}

	case OPCODE_FSUB :
		{
			const klr_fsub_t* op = (klr_fsub_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_fSUB(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_FSUB;
			goto L_HEAD;
		}

	case OPCODE_FSUBN :
		{
			const klr_fsubn_t* op = (klr_fsubn_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_fSUBn(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_FSUBN;
			goto L_HEAD;
		}

	case OPCODE_FMUL :
		{
			const klr_fmul_t* op = (klr_fmul_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_fMUL(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_FMUL;
			goto L_HEAD;
		}

	case OPCODE_FMULN :
		{
			const klr_fmuln_t* op = (klr_fmuln_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_fMULn(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_FMULN;
			goto L_HEAD;
		}

	case OPCODE_FDIV :
		{
			const klr_fdiv_t* op = (klr_fdiv_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_fDIV(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_FDIV;
			goto L_HEAD;
		}

	case OPCODE_FDIVN :
		{
			const klr_fdivn_t* op = (klr_fdivn_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_fDIVn(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_FDIVN;
			goto L_HEAD;
		}

	case OPCODE_FEQ :
		{
			const klr_feq_t* op = (klr_feq_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_fEQ(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_FEQ;
			goto L_HEAD;
		}

	case OPCODE_FEQN :
		{
			const klr_feqn_t* op = (klr_feqn_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_fEQn(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_FEQN;
			goto L_HEAD;
		}

	case OPCODE_FNEQ :
		{
			const klr_fneq_t* op = (klr_fneq_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_fNEQ(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_FNEQ;
			goto L_HEAD;
		}

	case OPCODE_FNEQN :
		{
			const klr_fneqn_t* op = (klr_fneqn_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_fNEQn(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_FNEQN;
			goto L_HEAD;
		}

	case OPCODE_FLT :
		{
			const klr_flt_t* op = (klr_flt_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_fLT(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_FLT;
			goto L_HEAD;
		}

	case OPCODE_FLTN :
		{
			const klr_fltn_t* op = (klr_fltn_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_fLTn(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_FLTN;
			goto L_HEAD;
		}

	case OPCODE_FLTE :
		{
			const klr_flte_t* op = (klr_flte_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_fLTE(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_FLTE;
			goto L_HEAD;
		}

	case OPCODE_FLTEN :
		{
			const klr_flten_t* op = (klr_flten_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_fLTEn(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_FLTEN;
			goto L_HEAD;
		}

	case OPCODE_FGT :
		{
			const klr_fgt_t* op = (klr_fgt_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_fGT(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_FGT;
			goto L_HEAD;
		}

	case OPCODE_FGTN :
		{
			const klr_fgtn_t* op = (klr_fgtn_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_fGTn(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_FGTN;
			goto L_HEAD;
		}

	case OPCODE_FGTE :
		{
			const klr_fgte_t* op = (klr_fgte_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_fGTE(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_FGTE;
			goto L_HEAD;
		}

	case OPCODE_FGTEN :
		{
			const klr_fgten_t* op = (klr_fgten_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_fGTEn(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_FGTEN;
			goto L_HEAD;
		}

	case OPCODE_AGET :
		{
			const klr_aget_t* op = (klr_aget_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_AGET(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_AGET;
			goto L_HEAD;
		}

	case OPCODE_AGETN :
		{
			const klr_agetn_t* op = (klr_agetn_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_AGETn(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_AGETN;
			goto L_HEAD;
		}

	case OPCODE_IAGET :
		{
			const klr_iaget_t* op = (klr_iaget_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_IAGET(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_IAGET;
			goto L_HEAD;
		}

	case OPCODE_IAGETN :
		{
			const klr_iagetn_t* op = (klr_iagetn_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_IAGETn(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_IAGETN;
			goto L_HEAD;
		}

	case OPCODE_FAGET :
		{
			const klr_faget_t* op = (klr_faget_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_FAGET(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_FAGET;
			goto L_HEAD;
		}

	case OPCODE_FAGETN :
		{
			const klr_fagetn_t* op = (klr_fagetn_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_FAGETn(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_FAGETN;
			goto L_HEAD;
		}

	case OPCODE_ASET :
		{
			const klr_aset_t* op = (klr_aset_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_ASET(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_ASET;
			goto L_HEAD;
		}

	case OPCODE_ASETN :
		{
			const klr_asetn_t* op = (klr_asetn_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_ASETn(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_ASETN;
			goto L_HEAD;
		}

	case OPCODE_IASET :
		{
			const klr_iaset_t* op = (klr_iaset_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_IASET(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_IASET;
			goto L_HEAD;
		}

	case OPCODE_IASETN :
		{
			const klr_iasetn_t* op = (klr_iasetn_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_IASETn(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_IASETN;
			goto L_HEAD;
		}

	case OPCODE_FASET :
		{
			const klr_faset_t* op = (klr_faset_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_FASET(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_FASET;
			goto L_HEAD;
		}

	case OPCODE_FASETN :
		{
			const klr_fasetn_t* op = (klr_fasetn_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_FASETn(ctx, op->a1, op->a2, op->a3);
			pc += OPSIZE_FASETN;
			goto L_HEAD;
		}

	case OPCODE_INITCODE :
		{
			const klr_initcode_t* op = (klr_initcode_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_INITCODE(ctx, op->a1);
			pc += OPSIZE_INITCODE;
			goto L_HEAD;
		}

	case OPCODE_NOP :
		{
			//const klr_nop_t* op = (klr_nop_t*)pc;
			DBG2_OPDUMP(ctx, pc);
			KLR_NOP(ctx);
			pc += OPSIZE_NOP;
			goto L_HEAD;
		}

	}
	KNH_WARNING(ctx, "unknown opcode=%d", ((knh_kode_t*)pc)->opcode);
	goto L_HEAD;
}
#endif/*KNH_USING_THREADEDCODE*/

/* ------------------------------------------------------------------------ */

#ifdef KNH_USING_THREADEDCODE
METHOD knh_KLRCode_exec(Ctx *ctx, knh_sfp_t *sfp)
{
	static void *OPJUMP[] = {
		&&L_HALT,
		&&L_MOVA,
		&&L_MOVN,
		&&L_MOVO,
		&&L_MOVX,
		&&L_MOVDEF,
		&&L_MOVSYS,
		&&L_XMOVS,
		&&L_XMOVO,
		&&L_XMOVX,
		&&L_XMOVDEF,
		&&L_XMOVSYS,
		&&L_MOVXI,
		&&L_XMOVSI,
		&&L_XMOVOI,
		&&L_XMOVXI,
		&&L_XMOVXIO,
		&&L_XMOVXBXI,
		&&L_MOVXF,
		&&L_XMOVSF,
		&&L_XMOVOF,
		&&L_XMOVXF,
		&&L_XMOVXFO,
		&&L_XMOVXBXF,
		&&L_MOVXB,
		&&L_XMOVSB,
		&&L_XMOVOB,
		&&L_XMOVXB,
		&&L_EMOVS,
		&&L_EMOVO,
		&&L_EMOVE,
		&&L_EMOVDEF,
		&&L_EMOVSYS,
		&&L_SWAP,
		&&L_PINIDEF,
		&&L_PINIO,
		&&L_PARAMS,
		&&L_SETESP,
		&&L_CHECKESP,
		&&L_RET,
		&&L_RETN,
		&&L_RETA,
		&&L_RETO,
		&&L_RETX,
		&&L_YEILDBREAK,
		&&L_BOX,
		&&L_BOXNC,
		&&L_NNBOX,
		&&L_NNBOXNC,
		&&L_UNBOX,
		&&L_CHECKNULL,
		&&L_CHECKNULLX,
		&&L_CHECKTYPE,
		&&L_CHECKNNTYPE,
		&&L_FCALL,
		&&L_SCALL,
		&&L_AINVOKE,
		&&L_CALL,
		&&L_ACALL,
		&&L_NEW,
		&&L_COPYSFP,
		&&L_TOSTR,
		&&L_TOSTRF,
		&&L_SMAP,
		&&L_SMAPNC,
		&&L_MAP,
		&&L_MAPNC,
		&&L_AMAP,
		&&L_NNMAP,
		&&L_JMP,
		&&L_SKIP,
		&&L_BJIFT,
		&&L_BJIFF,
		&&L_BJIFF_LOOP,
		&&L_JIFNUL,
		&&L_JIFNN,
		&&L_NEXT,
		&&L_INEXT,
		&&L_MAPNEXT,
		&&L_SMAPNEXT,
		&&L_TRY,
		&&L_TRYEND,
		&&L_CATCH,
		&&L_PUSH,
		&&L_POP,
		&&L_THROW,
		&&L_THROWS,
		&&L_THROW_AGAIN,
		&&L_P,
		&&L_PMSG,
		&&L_ICAST,
		&&L_INCAST,
		&&L_FCAST,
		&&L_FNCAST,
		&&L_BNOT,
		&&L_INEG,
		&&L_IADD,
		&&L_IADDN,
		&&L_ISUB,
		&&L_ISUBN,
		&&L_IMUL,
		&&L_IMULN,
		&&L_IDIV,
		&&L_IDIVN,
		&&L_IMOD,
		&&L_IMODN,
		&&L_IEQ,
		&&L_IEQN,
		&&L_INEQ,
		&&L_INEQN,
		&&L_ILT,
		&&L_ILTN,
		&&L_ILTE,
		&&L_ILTEN,
		&&L_IGT,
		&&L_IGTN,
		&&L_IGTE,
		&&L_IGTEN,
		&&L_FNEG,
		&&L_FADD,
		&&L_FADDN,
		&&L_FSUB,
		&&L_FSUBN,
		&&L_FMUL,
		&&L_FMULN,
		&&L_FDIV,
		&&L_FDIVN,
		&&L_FEQ,
		&&L_FEQN,
		&&L_FNEQ,
		&&L_FNEQN,
		&&L_FLT,
		&&L_FLTN,
		&&L_FLTE,
		&&L_FLTEN,
		&&L_FGT,
		&&L_FGTN,
		&&L_FGTE,
		&&L_FGTEN,
		&&L_AGET,
		&&L_AGETN,
		&&L_IAGET,
		&&L_IAGETN,
		&&L_FAGET,
		&&L_FAGETN,
		&&L_ASET,
		&&L_ASETN,
		&&L_IASET,
		&&L_IASETN,
		&&L_FASET,
		&&L_FASETN,
		&&L_INITCODE,
		&&L_NOP,
	};
	register knh_code_t *pc = sfp[-1].pc;
	goto *OPJUMP[KNH_OPCODE(pc)]; /* this is needed to init */


	L_HALT:; {
		const klr_halt_t *op = (klr_halt_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_HALT(ctx);
		pc += OPSIZE_HALT;
		goto *(op->nextaddr);
	}

	L_MOVA:; {
		const klr_mova_t *op = (klr_mova_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_MOVa(ctx, op->a1, op->a2);
		pc += OPSIZE_MOVA;
		goto *(op->nextaddr);
	}

	L_MOVN:; {
		const klr_movn_t *op = (klr_movn_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_MOVn(ctx, op->a1, op->a2);
		pc += OPSIZE_MOVN;
		goto *(op->nextaddr);
	}

	L_MOVO:; {
		const klr_movo_t *op = (klr_movo_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_MOVo(ctx, op->a1, op->a2);
		pc += OPSIZE_MOVO;
		goto *(op->nextaddr);
	}

	L_MOVX:; {
		const klr_movx_t *op = (klr_movx_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_MOVx(ctx, op->a1, op->a2);
		pc += OPSIZE_MOVX;
		goto *(op->nextaddr);
	}

	L_MOVDEF:; {
		const klr_movdef_t *op = (klr_movdef_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_MOVDEF(ctx, op->a1, op->a2);
		pc += OPSIZE_MOVDEF;
		goto *(op->nextaddr);
	}

	L_MOVSYS:; {
		const klr_movsys_t *op = (klr_movsys_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_MOVSYS(ctx, op->a1, op->a2);
		pc += OPSIZE_MOVSYS;
		goto *(op->nextaddr);
	}

	L_XMOVS:; {
		const klr_xmovs_t *op = (klr_xmovs_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_XMOVs(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVS;
		goto *(op->nextaddr);
	}

	L_XMOVO:; {
		const klr_xmovo_t *op = (klr_xmovo_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_XMOVo(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVO;
		goto *(op->nextaddr);
	}

	L_XMOVX:; {
		const klr_xmovx_t *op = (klr_xmovx_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_XMOVx(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVX;
		goto *(op->nextaddr);
	}

	L_XMOVDEF:; {
		const klr_xmovdef_t *op = (klr_xmovdef_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_XMOVDEF(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVDEF;
		goto *(op->nextaddr);
	}

	L_XMOVSYS:; {
		const klr_xmovsys_t *op = (klr_xmovsys_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_XMOVSYS(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVSYS;
		goto *(op->nextaddr);
	}

	L_MOVXI:; {
		const klr_movxi_t *op = (klr_movxi_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_MOVxi(ctx, op->a1, op->a2);
		pc += OPSIZE_MOVXI;
		goto *(op->nextaddr);
	}

	L_XMOVSI:; {
		const klr_xmovsi_t *op = (klr_xmovsi_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_XMOVsi(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVSI;
		goto *(op->nextaddr);
	}

	L_XMOVOI:; {
		const klr_xmovoi_t *op = (klr_xmovoi_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_XMOVoi(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVOI;
		goto *(op->nextaddr);
	}

	L_XMOVXI:; {
		const klr_xmovxi_t *op = (klr_xmovxi_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_XMOVxi(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVXI;
		goto *(op->nextaddr);
	}

	L_XMOVXIO:; {
		const klr_xmovxio_t *op = (klr_xmovxio_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_XMOVxio(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVXIO;
		goto *(op->nextaddr);
	}

	L_XMOVXBXI:; {
		const klr_xmovxbxi_t *op = (klr_xmovxbxi_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_XMOVxBXi(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_XMOVXBXI;
		goto *(op->nextaddr);
	}

	L_MOVXF:; {
		const klr_movxf_t *op = (klr_movxf_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_MOVxf(ctx, op->a1, op->a2);
		pc += OPSIZE_MOVXF;
		goto *(op->nextaddr);
	}

	L_XMOVSF:; {
		const klr_xmovsf_t *op = (klr_xmovsf_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_XMOVsf(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVSF;
		goto *(op->nextaddr);
	}

	L_XMOVOF:; {
		const klr_xmovof_t *op = (klr_xmovof_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_XMOVof(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVOF;
		goto *(op->nextaddr);
	}

	L_XMOVXF:; {
		const klr_xmovxf_t *op = (klr_xmovxf_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_XMOVxf(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVXF;
		goto *(op->nextaddr);
	}

	L_XMOVXFO:; {
		const klr_xmovxfo_t *op = (klr_xmovxfo_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_XMOVxfo(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVXFO;
		goto *(op->nextaddr);
	}

	L_XMOVXBXF:; {
		const klr_xmovxbxf_t *op = (klr_xmovxbxf_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_XMOVxBXf(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_XMOVXBXF;
		goto *(op->nextaddr);
	}

	L_MOVXB:; {
		const klr_movxb_t *op = (klr_movxb_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_MOVxb(ctx, op->a1, op->a2);
		pc += OPSIZE_MOVXB;
		goto *(op->nextaddr);
	}

	L_XMOVSB:; {
		const klr_xmovsb_t *op = (klr_xmovsb_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_XMOVsb(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVSB;
		goto *(op->nextaddr);
	}

	L_XMOVOB:; {
		const klr_xmovob_t *op = (klr_xmovob_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_XMOVob(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVOB;
		goto *(op->nextaddr);
	}

	L_XMOVXB:; {
		const klr_xmovxb_t *op = (klr_xmovxb_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_XMOVxb(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVXB;
		goto *(op->nextaddr);
	}

	L_EMOVS:; {
		const klr_emovs_t *op = (klr_emovs_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_EMOVs(ctx, op->a1, op->a2);
		pc += OPSIZE_EMOVS;
		goto *(op->nextaddr);
	}

	L_EMOVO:; {
		const klr_emovo_t *op = (klr_emovo_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_EMOVo(ctx, op->a1, op->a2);
		pc += OPSIZE_EMOVO;
		goto *(op->nextaddr);
	}

	L_EMOVE:; {
		const klr_emove_t *op = (klr_emove_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_EMOVe(ctx, op->a1, op->a2);
		pc += OPSIZE_EMOVE;
		goto *(op->nextaddr);
	}

	L_EMOVDEF:; {
		const klr_emovdef_t *op = (klr_emovdef_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_EMOVDEF(ctx, op->a1, op->a2);
		pc += OPSIZE_EMOVDEF;
		goto *(op->nextaddr);
	}

	L_EMOVSYS:; {
		const klr_emovsys_t *op = (klr_emovsys_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_EMOVSYS(ctx, op->a1, op->a2);
		pc += OPSIZE_EMOVSYS;
		goto *(op->nextaddr);
	}

	L_SWAP:; {
		const klr_swap_t *op = (klr_swap_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_SWAP(ctx, op->a1, op->a2);
		pc += OPSIZE_SWAP;
		goto *(op->nextaddr);
	}

	L_PINIDEF:; {
		const klr_pinidef_t *op = (klr_pinidef_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_PINIDEF(ctx, op->a1, op->a2);
		pc += OPSIZE_PINIDEF;
		goto *(op->nextaddr);
	}

	L_PINIO:; {
		const klr_pinio_t *op = (klr_pinio_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_PINIo(ctx, op->a1, op->a2);
		pc += OPSIZE_PINIO;
		goto *(op->nextaddr);
	}

	L_PARAMS:; {
		const klr_params_t *op = (klr_params_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_PARAMS(ctx, op->a1, op->a2);
		pc += OPSIZE_PARAMS;
		goto *(op->nextaddr);
	}

	L_SETESP:; {
		const klr_setesp_t *op = (klr_setesp_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_SETESP(ctx, op->a1);
		pc += OPSIZE_SETESP;
		goto *(op->nextaddr);
	}

	L_CHECKESP:; {
		const klr_checkesp_t *op = (klr_checkesp_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_CHECKESP(ctx, op->a1);
		pc += OPSIZE_CHECKESP;
		goto *(op->nextaddr);
	}

	L_RET:; {
		const klr_ret_t *op = (klr_ret_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_RET(ctx);
		pc += OPSIZE_RET;
		goto *(op->nextaddr);
	}

	L_RETN:; {
		const klr_retn_t *op = (klr_retn_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_RETn(ctx, op->a1, op->a2);
		pc += OPSIZE_RETN;
		goto *(op->nextaddr);
	}

	L_RETA:; {
		const klr_reta_t *op = (klr_reta_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_RETa(ctx, op->a1, op->a2);
		pc += OPSIZE_RETA;
		goto *(op->nextaddr);
	}

	L_RETO:; {
		const klr_reto_t *op = (klr_reto_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_RETo(ctx, op->a1, op->a2);
		pc += OPSIZE_RETO;
		goto *(op->nextaddr);
	}

	L_RETX:; {
		const klr_retx_t *op = (klr_retx_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_RETx(ctx, op->a1, op->a2);
		pc += OPSIZE_RETX;
		goto *(op->nextaddr);
	}

	L_YEILDBREAK:; {
		const klr_yeildbreak_t *op = (klr_yeildbreak_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_YEILDBREAK(ctx);
		pc += OPSIZE_YEILDBREAK;
		goto *(op->nextaddr);
	}

	L_BOX:; {
		const klr_box_t *op = (klr_box_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_BOX(ctx, op->a1, op->a2);
		pc += OPSIZE_BOX;
		goto *(op->nextaddr);
	}

	L_BOXNC:; {
		const klr_boxnc_t *op = (klr_boxnc_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_BOXnc(ctx, op->a1, op->a2);
		pc += OPSIZE_BOXNC;
		goto *(op->nextaddr);
	}

	L_NNBOX:; {
		const klr_nnbox_t *op = (klr_nnbox_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_NNBOX(ctx, op->a1, op->a2);
		pc += OPSIZE_NNBOX;
		goto *(op->nextaddr);
	}

	L_NNBOXNC:; {
		const klr_nnboxnc_t *op = (klr_nnboxnc_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_NNBOXnc(ctx, op->a1, op->a2);
		pc += OPSIZE_NNBOXNC;
		goto *(op->nextaddr);
	}

	L_UNBOX:; {
		const klr_unbox_t *op = (klr_unbox_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_UNBOX(ctx, op->a1);
		pc += OPSIZE_UNBOX;
		goto *(op->nextaddr);
	}

	L_CHECKNULL:; {
		const klr_checknull_t *op = (klr_checknull_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_CHECKNULL(ctx, op->a1);
		pc += OPSIZE_CHECKNULL;
		goto *(op->nextaddr);
	}

	L_CHECKNULLX:; {
		const klr_checknullx_t *op = (klr_checknullx_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_CHECKNULLx(ctx, op->a1);
		pc += OPSIZE_CHECKNULLX;
		goto *(op->nextaddr);
	}

	L_CHECKTYPE:; {
		const klr_checktype_t *op = (klr_checktype_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_CHECKTYPE(ctx, op->a1, op->a2);
		pc += OPSIZE_CHECKTYPE;
		goto *(op->nextaddr);
	}

	L_CHECKNNTYPE:; {
		const klr_checknntype_t *op = (klr_checknntype_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_CHECKNNTYPE(ctx, op->a1, op->a2);
		pc += OPSIZE_CHECKNNTYPE;
		goto *(op->nextaddr);
	}

	L_FCALL:; {
		const klr_fcall_t *op = (klr_fcall_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_FCALL(ctx, op->a1, op->a2, op->a3, op->a4);
		pc += OPSIZE_FCALL;
		goto *(op->nextaddr);
	}

	L_SCALL:; {
		const klr_scall_t *op = (klr_scall_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_SCALL(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_SCALL;
		goto *(op->nextaddr);
	}

	L_AINVOKE:; {
		const klr_ainvoke_t *op = (klr_ainvoke_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_AINVOKE(ctx, op->a1, op->a2);
		pc += OPSIZE_AINVOKE;
		goto *(op->nextaddr);
	}

	L_CALL:; {
		const klr_call_t *op = (klr_call_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_CALL(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_CALL;
		goto *(op->nextaddr);
	}

	L_ACALL:; {
		const klr_acall_t *op = (klr_acall_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_ACALL(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_ACALL;
		goto *(op->nextaddr);
	}

	L_NEW:; {
		const klr_new_t *op = (klr_new_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_NEW(ctx, op->a1, op->a2, op->a3, op->a4, op->a5);
		pc += OPSIZE_NEW;
		goto *(op->nextaddr);
	}

	L_COPYSFP:; {
		const klr_copysfp_t *op = (klr_copysfp_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_COPYSFP(ctx, op->a1);
		pc += OPSIZE_COPYSFP;
		goto *(op->nextaddr);
	}

	L_TOSTR:; {
		const klr_tostr_t *op = (klr_tostr_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_TOSTR(ctx, op->a1, op->a2);
		pc += OPSIZE_TOSTR;
		goto *(op->nextaddr);
	}

	L_TOSTRF:; {
		const klr_tostrf_t *op = (klr_tostrf_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_TOSTRf(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_TOSTRF;
		goto *(op->nextaddr);
	}

	L_SMAP:; {
		const klr_smap_t *op = (klr_smap_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_SMAP(ctx, op->a1, op->a2);
		pc += OPSIZE_SMAP;
		goto *(op->nextaddr);
	}

	L_SMAPNC:; {
		const klr_smapnc_t *op = (klr_smapnc_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_SMAPnc(ctx, op->a1, op->a2);
		pc += OPSIZE_SMAPNC;
		goto *(op->nextaddr);
	}

	L_MAP:; {
		const klr_map_t *op = (klr_map_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_MAP(ctx, op->a1, op->a2);
		pc += OPSIZE_MAP;
		goto *(op->nextaddr);
	}

	L_MAPNC:; {
		const klr_mapnc_t *op = (klr_mapnc_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_MAPnc(ctx, op->a1, op->a2);
		pc += OPSIZE_MAPNC;
		goto *(op->nextaddr);
	}

	L_AMAP:; {
		const klr_amap_t *op = (klr_amap_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_AMAP(ctx, op->a1, op->a2);
		pc += OPSIZE_AMAP;
		goto *(op->nextaddr);
	}

	L_NNMAP:; {
		const klr_nnmap_t *op = (klr_nnmap_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_NNMAP(ctx, op->a1, op->a2);
		pc += OPSIZE_NNMAP;
		goto *(op->nextaddr);
	}

	L_JMP:; {
		const klr_jmp_t *op = (klr_jmp_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_JMP(ctx, pc = op->jumppc, *(op->jumpaddr));
		pc += OPSIZE_JMP;
		goto *(op->nextaddr);
	}

	L_SKIP:; {
		const klr_skip_t *op = (klr_skip_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_SKIP(ctx, pc = op->jumppc, *(op->jumpaddr));
		pc += OPSIZE_SKIP;
		goto *(op->nextaddr);
	}

	L_BJIFT:; {
		const klr_bjift_t *op = (klr_bjift_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_bJIFT(ctx, pc = op->jumppc, *(op->jumpaddr), op->a2);
		pc += OPSIZE_BJIFT;
		goto *(op->nextaddr);
	}

	L_BJIFF:; {
		const klr_bjiff_t *op = (klr_bjiff_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_bJIFF(ctx, pc = op->jumppc, *(op->jumpaddr), op->a2);
		pc += OPSIZE_BJIFF;
		goto *(op->nextaddr);
	}

	L_BJIFF_LOOP:; {
		const klr_bjiff_loop_t *op = (klr_bjiff_loop_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_bJIFF_LOOP(ctx, pc = op->jumppc, *(op->jumpaddr), op->a2);
		pc += OPSIZE_BJIFF_LOOP;
		goto *(op->nextaddr);
	}

	L_JIFNUL:; {
		const klr_jifnul_t *op = (klr_jifnul_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_JIFNUL(ctx, pc = op->jumppc, *(op->jumpaddr), op->a2);
		pc += OPSIZE_JIFNUL;
		goto *(op->nextaddr);
	}

	L_JIFNN:; {
		const klr_jifnn_t *op = (klr_jifnn_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_JIFNN(ctx, pc = op->jumppc, *(op->jumpaddr), op->a2);
		pc += OPSIZE_JIFNN;
		goto *(op->nextaddr);
	}

	L_NEXT:; {
		const klr_next_t *op = (klr_next_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_NEXT(ctx, pc = op->jumppc, *(op->jumpaddr), op->a2, op->a3);
		pc += OPSIZE_NEXT;
		goto *(op->nextaddr);
	}

	L_INEXT:; {
		const klr_inext_t *op = (klr_inext_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_INEXT(ctx, pc = op->jumppc, *(op->jumpaddr), op->a2, op->a3, op->a4);
		pc += OPSIZE_INEXT;
		goto *(op->nextaddr);
	}

	L_MAPNEXT:; {
		const klr_mapnext_t *op = (klr_mapnext_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_MAPNEXT(ctx, pc = op->jumppc, *(op->jumpaddr), op->a2, op->a3, op->a4);
		pc += OPSIZE_MAPNEXT;
		goto *(op->nextaddr);
	}

	L_SMAPNEXT:; {
		const klr_smapnext_t *op = (klr_smapnext_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_SMAPNEXT(ctx, pc = op->jumppc, *(op->jumpaddr), op->a2, op->a3, op->a4);
		pc += OPSIZE_SMAPNEXT;
		goto *(op->nextaddr);
	}

	L_TRY:; {
		const klr_try_t *op = (klr_try_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_TRY(ctx, pc = op->jumppc, *(op->jumpaddr), op->a2);
		pc += OPSIZE_TRY;
		goto *(op->nextaddr);
	}

	L_TRYEND:; {
		const klr_tryend_t *op = (klr_tryend_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_TRYEND(ctx, op->a1);
		pc += OPSIZE_TRYEND;
		goto *(op->nextaddr);
	}

	L_CATCH:; {
		const klr_catch_t *op = (klr_catch_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_CATCH(ctx, pc = op->jumppc, *(op->jumpaddr), op->a2, op->a3, op->a4);
		pc += OPSIZE_CATCH;
		goto *(op->nextaddr);
	}

	L_PUSH:; {
		const klr_push_t *op = (klr_push_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_PUSH(ctx, op->a1);
		pc += OPSIZE_PUSH;
		goto *(op->nextaddr);
	}

	L_POP:; {
		const klr_pop_t *op = (klr_pop_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_POP(ctx, op->a1);
		pc += OPSIZE_POP;
		goto *(op->nextaddr);
	}

	L_THROW:; {
		const klr_throw_t *op = (klr_throw_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_THROW(ctx, op->a1, op->a2);
		pc += OPSIZE_THROW;
		goto *(op->nextaddr);
	}

	L_THROWS:; {
		const klr_throws_t *op = (klr_throws_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_THROWs(ctx, op->a1, op->a2);
		pc += OPSIZE_THROWS;
		goto *(op->nextaddr);
	}

	L_THROW_AGAIN:; {
		const klr_throw_again_t *op = (klr_throw_again_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_THROW_AGAIN(ctx, op->a1);
		pc += OPSIZE_THROW_AGAIN;
		goto *(op->nextaddr);
	}

	L_P:; {
		const klr_p_t *op = (klr_p_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_P(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_P;
		goto *(op->nextaddr);
	}

	L_PMSG:; {
		const klr_pmsg_t *op = (klr_pmsg_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_PMSG(ctx, op->a1, op->a2);
		pc += OPSIZE_PMSG;
		goto *(op->nextaddr);
	}

	L_ICAST:; {
		const klr_icast_t *op = (klr_icast_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_iCAST(ctx, op->a1);
		pc += OPSIZE_ICAST;
		goto *(op->nextaddr);
	}

	L_INCAST:; {
		const klr_incast_t *op = (klr_incast_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_inCAST(ctx, op->a1);
		pc += OPSIZE_INCAST;
		goto *(op->nextaddr);
	}

	L_FCAST:; {
		const klr_fcast_t *op = (klr_fcast_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_fCAST(ctx, op->a1);
		pc += OPSIZE_FCAST;
		goto *(op->nextaddr);
	}

	L_FNCAST:; {
		const klr_fncast_t *op = (klr_fncast_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_fnCAST(ctx, op->a1);
		pc += OPSIZE_FNCAST;
		goto *(op->nextaddr);
	}

	L_BNOT:; {
		const klr_bnot_t *op = (klr_bnot_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_bNOT(ctx, op->a1, op->a2);
		pc += OPSIZE_BNOT;
		goto *(op->nextaddr);
	}

	L_INEG:; {
		const klr_ineg_t *op = (klr_ineg_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_iNEG(ctx, op->a1, op->a2);
		pc += OPSIZE_INEG;
		goto *(op->nextaddr);
	}

	L_IADD:; {
		const klr_iadd_t *op = (klr_iadd_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_iADD(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_IADD;
		goto *(op->nextaddr);
	}

	L_IADDN:; {
		const klr_iaddn_t *op = (klr_iaddn_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_iADDn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_IADDN;
		goto *(op->nextaddr);
	}

	L_ISUB:; {
		const klr_isub_t *op = (klr_isub_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_iSUB(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_ISUB;
		goto *(op->nextaddr);
	}

	L_ISUBN:; {
		const klr_isubn_t *op = (klr_isubn_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_iSUBn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_ISUBN;
		goto *(op->nextaddr);
	}

	L_IMUL:; {
		const klr_imul_t *op = (klr_imul_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_iMUL(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_IMUL;
		goto *(op->nextaddr);
	}

	L_IMULN:; {
		const klr_imuln_t *op = (klr_imuln_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_iMULn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_IMULN;
		goto *(op->nextaddr);
	}

	L_IDIV:; {
		const klr_idiv_t *op = (klr_idiv_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_iDIV(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_IDIV;
		goto *(op->nextaddr);
	}

	L_IDIVN:; {
		const klr_idivn_t *op = (klr_idivn_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_iDIVn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_IDIVN;
		goto *(op->nextaddr);
	}

	L_IMOD:; {
		const klr_imod_t *op = (klr_imod_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_iMOD(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_IMOD;
		goto *(op->nextaddr);
	}

	L_IMODN:; {
		const klr_imodn_t *op = (klr_imodn_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_iMODn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_IMODN;
		goto *(op->nextaddr);
	}

	L_IEQ:; {
		const klr_ieq_t *op = (klr_ieq_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_iEQ(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_IEQ;
		goto *(op->nextaddr);
	}

	L_IEQN:; {
		const klr_ieqn_t *op = (klr_ieqn_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_iEQn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_IEQN;
		goto *(op->nextaddr);
	}

	L_INEQ:; {
		const klr_ineq_t *op = (klr_ineq_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_iNEQ(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_INEQ;
		goto *(op->nextaddr);
	}

	L_INEQN:; {
		const klr_ineqn_t *op = (klr_ineqn_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_iNEQn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_INEQN;
		goto *(op->nextaddr);
	}

	L_ILT:; {
		const klr_ilt_t *op = (klr_ilt_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_iLT(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_ILT;
		goto *(op->nextaddr);
	}

	L_ILTN:; {
		const klr_iltn_t *op = (klr_iltn_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_iLTn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_ILTN;
		goto *(op->nextaddr);
	}

	L_ILTE:; {
		const klr_ilte_t *op = (klr_ilte_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_iLTE(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_ILTE;
		goto *(op->nextaddr);
	}

	L_ILTEN:; {
		const klr_ilten_t *op = (klr_ilten_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_iLTEn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_ILTEN;
		goto *(op->nextaddr);
	}

	L_IGT:; {
		const klr_igt_t *op = (klr_igt_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_iGT(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_IGT;
		goto *(op->nextaddr);
	}

	L_IGTN:; {
		const klr_igtn_t *op = (klr_igtn_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_iGTn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_IGTN;
		goto *(op->nextaddr);
	}

	L_IGTE:; {
		const klr_igte_t *op = (klr_igte_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_iGTE(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_IGTE;
		goto *(op->nextaddr);
	}

	L_IGTEN:; {
		const klr_igten_t *op = (klr_igten_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_iGTEn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_IGTEN;
		goto *(op->nextaddr);
	}

	L_FNEG:; {
		const klr_fneg_t *op = (klr_fneg_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_fNEG(ctx, op->a1, op->a2);
		pc += OPSIZE_FNEG;
		goto *(op->nextaddr);
	}

	L_FADD:; {
		const klr_fadd_t *op = (klr_fadd_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_fADD(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_FADD;
		goto *(op->nextaddr);
	}

	L_FADDN:; {
		const klr_faddn_t *op = (klr_faddn_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_fADDn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_FADDN;
		goto *(op->nextaddr);
	}

	L_FSUB:; {
		const klr_fsub_t *op = (klr_fsub_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_fSUB(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_FSUB;
		goto *(op->nextaddr);
	}

	L_FSUBN:; {
		const klr_fsubn_t *op = (klr_fsubn_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_fSUBn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_FSUBN;
		goto *(op->nextaddr);
	}

	L_FMUL:; {
		const klr_fmul_t *op = (klr_fmul_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_fMUL(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_FMUL;
		goto *(op->nextaddr);
	}

	L_FMULN:; {
		const klr_fmuln_t *op = (klr_fmuln_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_fMULn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_FMULN;
		goto *(op->nextaddr);
	}

	L_FDIV:; {
		const klr_fdiv_t *op = (klr_fdiv_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_fDIV(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_FDIV;
		goto *(op->nextaddr);
	}

	L_FDIVN:; {
		const klr_fdivn_t *op = (klr_fdivn_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_fDIVn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_FDIVN;
		goto *(op->nextaddr);
	}

	L_FEQ:; {
		const klr_feq_t *op = (klr_feq_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_fEQ(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_FEQ;
		goto *(op->nextaddr);
	}

	L_FEQN:; {
		const klr_feqn_t *op = (klr_feqn_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_fEQn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_FEQN;
		goto *(op->nextaddr);
	}

	L_FNEQ:; {
		const klr_fneq_t *op = (klr_fneq_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_fNEQ(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_FNEQ;
		goto *(op->nextaddr);
	}

	L_FNEQN:; {
		const klr_fneqn_t *op = (klr_fneqn_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_fNEQn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_FNEQN;
		goto *(op->nextaddr);
	}

	L_FLT:; {
		const klr_flt_t *op = (klr_flt_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_fLT(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_FLT;
		goto *(op->nextaddr);
	}

	L_FLTN:; {
		const klr_fltn_t *op = (klr_fltn_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_fLTn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_FLTN;
		goto *(op->nextaddr);
	}

	L_FLTE:; {
		const klr_flte_t *op = (klr_flte_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_fLTE(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_FLTE;
		goto *(op->nextaddr);
	}

	L_FLTEN:; {
		const klr_flten_t *op = (klr_flten_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_fLTEn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_FLTEN;
		goto *(op->nextaddr);
	}

	L_FGT:; {
		const klr_fgt_t *op = (klr_fgt_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_fGT(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_FGT;
		goto *(op->nextaddr);
	}

	L_FGTN:; {
		const klr_fgtn_t *op = (klr_fgtn_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_fGTn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_FGTN;
		goto *(op->nextaddr);
	}

	L_FGTE:; {
		const klr_fgte_t *op = (klr_fgte_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_fGTE(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_FGTE;
		goto *(op->nextaddr);
	}

	L_FGTEN:; {
		const klr_fgten_t *op = (klr_fgten_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_fGTEn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_FGTEN;
		goto *(op->nextaddr);
	}

	L_AGET:; {
		const klr_aget_t *op = (klr_aget_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_AGET(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_AGET;
		goto *(op->nextaddr);
	}

	L_AGETN:; {
		const klr_agetn_t *op = (klr_agetn_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_AGETn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_AGETN;
		goto *(op->nextaddr);
	}

	L_IAGET:; {
		const klr_iaget_t *op = (klr_iaget_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_IAGET(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_IAGET;
		goto *(op->nextaddr);
	}

	L_IAGETN:; {
		const klr_iagetn_t *op = (klr_iagetn_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_IAGETn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_IAGETN;
		goto *(op->nextaddr);
	}

	L_FAGET:; {
		const klr_faget_t *op = (klr_faget_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_FAGET(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_FAGET;
		goto *(op->nextaddr);
	}

	L_FAGETN:; {
		const klr_fagetn_t *op = (klr_fagetn_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_FAGETn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_FAGETN;
		goto *(op->nextaddr);
	}

	L_ASET:; {
		const klr_aset_t *op = (klr_aset_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_ASET(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_ASET;
		goto *(op->nextaddr);
	}

	L_ASETN:; {
		const klr_asetn_t *op = (klr_asetn_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_ASETn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_ASETN;
		goto *(op->nextaddr);
	}

	L_IASET:; {
		const klr_iaset_t *op = (klr_iaset_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_IASET(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_IASET;
		goto *(op->nextaddr);
	}

	L_IASETN:; {
		const klr_iasetn_t *op = (klr_iasetn_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_IASETn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_IASETN;
		goto *(op->nextaddr);
	}

	L_FASET:; {
		const klr_faset_t *op = (klr_faset_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_FASET(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_FASET;
		goto *(op->nextaddr);
	}

	L_FASETN:; {
		const klr_fasetn_t *op = (klr_fasetn_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_FASETn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_FASETN;
		goto *(op->nextaddr);
	}

	L_INITCODE:; {
		const klr_initcode_t *op = (klr_initcode_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_INITCODE(ctx, op->a1);
		pc += OPSIZE_INITCODE;
		goto *(op->nextaddr);
	}

	L_NOP:; {
		const klr_nop_t *op = (klr_nop_t*)pc;
		DBG2_OPDUMP(ctx, pc);
		KLR_NOP(ctx);
		pc += OPSIZE_NOP;
		goto *(op->nextaddr);
	}

}
#endif


#ifdef __cplusplus
}
#endif
