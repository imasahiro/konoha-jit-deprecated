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

/* ======================================================================== */
/* [common] */

	
static void knh_write_opcode(Ctx *ctx, knh_OutputStream_t* w, int opcode)
{
	knh_printf(ctx, w, "%s(%d)", knh_opcode_tochar(opcode), (knh_intptr_t)opcode);
}
static void knh_write__addr(Ctx *ctx, knh_OutputStream_t* w, void *a)
{
	knh_putc(ctx, w, ' '); knh_printf(ctx, w, "%p", a);
}
static void knh_write__cid(Ctx *ctx, knh_OutputStream_t* w, knh_class_t cid)
{
	knh_putc(ctx, w, ' '); knh_write_cid(ctx, w, cid);
}
static void knh_write__float(Ctx *ctx, knh_OutputStream_t* w, knh_float_t a)
{
	knh_putc(ctx, w, ' '); knh_write_ffmt(ctx, w, KNH_FLOAT_FMT, a);
}
static void knh_write__int(Ctx *ctx, knh_OutputStream_t* w, knh_int_t a)
{
	knh_putc(ctx, w, ' '); knh_write_ifmt(ctx, w, KNH_INT_FMT, a);
}
static void knh_write__intptr(Ctx *ctx, knh_OutputStream_t* w, knh_intptr_t a)
{
	knh_putc(ctx, w, ' '); knh_write_dfmt(ctx, w, KNH_INTPTR_FMT, a);
}
static void knh_write__OBJ(Ctx *ctx, knh_OutputStream_t* w, Object *a)
{
	knh_putc(ctx, w, ' ');knh_format(ctx, w, METHODN__k, a, KNH_NULL);
}
static void knh_write__sfpidx(Ctx *ctx, knh_OutputStream_t* w, knh_sfpidx_t a)
{
	knh_printf(ctx, w, " sfp[%d]", (knh_intptr_t)a);
}
static void knh_write__sfe(Ctx *ctx, knh_OutputStream_t* w, knh_sfe_t a)
{
	knh_printf(ctx, w, " sfe[%d]", (knh_intptr_t)a);
}
static void knh_write__sfx(Ctx *ctx, knh_OutputStream_t* w, knh_sfx_t a)
{
	knh_printf(ctx, w, " sfx[%d]+%d", (knh_intptr_t)a.i, (knh_intptr_t)a.n);
}
static void knh_write__ushort(Ctx *ctx, knh_OutputStream_t* w, knh_ushort_t a)
{
	knh_putc(ctx, w, ' ');
	knh_write_dfmt(ctx, w, KNH_INTPTR_FMT, (knh_intptr_t)a);
}
static void knh_write__mn(Ctx *ctx, knh_OutputStream_t* w, knh_methodn_t a)
{
	knh_putc(ctx, w, ' '); knh_write_mn(ctx, w, TYPE_void, (knh_methodn_t)a);
}
//static void knh_write__type(Ctx *ctx, knh_OutputStream_t* w, knh_type_t a)
//{
//	knh_putc(ctx, w, ' '); knh_write_type(ctx, w, (knh_type_t)a);
//}
static void HALT_traverse(Ctx *ctx, knh_inst_t *c, knh_ftraverse ftr)
{
}
static void HALT_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
}

static void MOVo_traverse(Ctx *ctx, knh_inst_t *c, knh_ftraverse ftr)
{
	klr_MOVo_t *op = (klr_MOVo_t*)c; 
	DBG2_ASSERT(op->opcode == 3);
	ftr(ctx, UP(op->a2));
}

static void XMOVo_traverse(Ctx *ctx, knh_inst_t *c, knh_ftraverse ftr)
{
	klr_XMOVo_t *op = (klr_XMOVo_t*)c; 
	DBG2_ASSERT(op->opcode == 13);
	ftr(ctx, UP(op->a2));
}

static void PARAMo_traverse(Ctx *ctx, knh_inst_t *c, knh_ftraverse ftr)
{
	klr_PARAMo_t *op = (klr_PARAMo_t*)c; 
	DBG2_ASSERT(op->opcode == 22);
	ftr(ctx, UP(op->a2));
}

static void FCALL_traverse(Ctx *ctx, knh_inst_t *c, knh_ftraverse ftr)
{
	klr_FCALL_t *op = (klr_FCALL_t*)c; 
	DBG2_ASSERT(op->opcode == 36);
	ftr(ctx, UP(op->a4));
}

static void SCALL_traverse(Ctx *ctx, knh_inst_t *c, knh_ftraverse ftr)
{
	klr_SCALL_t *op = (klr_SCALL_t*)c; 
	DBG2_ASSERT(op->opcode == 37);
	ftr(ctx, UP(op->a3));
}

static void NEW_traverse(Ctx *ctx, knh_inst_t *c, knh_ftraverse ftr)
{
	klr_NEW_t *op = (klr_NEW_t*)c; 
	DBG2_ASSERT(op->opcode == 41);
	ftr(ctx, UP(op->a5));
}

static void STR_traverse(Ctx *ctx, knh_inst_t *c, knh_ftraverse ftr)
{
	klr_STR_t *op = (klr_STR_t*)c; 
	DBG2_ASSERT(op->opcode == 43);
	ftr(ctx, UP(op->a4));
}

static void SSTR_traverse(Ctx *ctx, knh_inst_t *c, knh_ftraverse ftr)
{
	klr_SSTR_t *op = (klr_SSTR_t*)c; 
	DBG2_ASSERT(op->opcode == 44);
	ftr(ctx, UP(op->a3));
	ftr(ctx, UP(op->a4));
}

static void SMAP_traverse(Ctx *ctx, knh_inst_t *c, knh_ftraverse ftr)
{
	klr_SMAP_t *op = (klr_SMAP_t*)c; 
	DBG2_ASSERT(op->opcode == 45);
	ftr(ctx, UP(op->a2));
}

static void SMAPnc_traverse(Ctx *ctx, knh_inst_t *c, knh_ftraverse ftr)
{
	klr_SMAPnc_t *op = (klr_SMAPnc_t*)c; 
	DBG2_ASSERT(op->opcode == 46);
	ftr(ctx, UP(op->a2));
}

static void CATCH_traverse(Ctx *ctx, knh_inst_t *c, knh_ftraverse ftr)
{
	klr_CATCH_t *op = (klr_CATCH_t*)c; 
	DBG2_ASSERT(op->opcode == 62);
	ftr(ctx, UP(op->a4));
}

static void THROWs_traverse(Ctx *ctx, knh_inst_t *c, knh_ftraverse ftr)
{
	klr_THROWs_t *op = (klr_THROWs_t*)c; 
	DBG2_ASSERT(op->opcode == 66);
	ftr(ctx, UP(op->a3));
}

static void PMSG_traverse(Ctx *ctx, knh_inst_t *c, knh_ftraverse ftr)
{
	klr_PMSG_t *op = (klr_PMSG_t*)c; 
	DBG2_ASSERT(op->opcode == 69);
	ftr(ctx, UP(op->a2));
}

static void HALT_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_HALT_t *op = (klr_HALT_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
}

static void MOVa_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_MOVa_t *op = (klr_MOVa_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfpidx(ctx, w, (op->a1));
	knh_write__sfpidx(ctx, w, (op->a2));
}

static void MOVo_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_MOVo_t *op = (klr_MOVo_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfpidx(ctx, w, (op->a1));
	knh_write__OBJ(ctx, w, UP((op->a2)));
}

static void MOVi_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_MOVi_t *op = (klr_MOVi_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfpidx(ctx, w, (op->a1));
	knh_write__int(ctx, w, (op->a2));
}

static void MOVx_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_MOVx_t *op = (klr_MOVx_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfpidx(ctx, w, (op->a1));
	knh_write__sfx(ctx, w, (op->a2));
}

static void MOVDEF_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_MOVDEF_t *op = (klr_MOVDEF_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfpidx(ctx, w, (op->a1));
	knh_write__cid(ctx, w, (op->a2));
}

static void MOVSYS_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_MOVSYS_t *op = (klr_MOVSYS_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfpidx(ctx, w, (op->a1));
	knh_write__ushort(ctx, w, (op->a2));
}

static void XMOVs_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_XMOVs_t *op = (klr_XMOVs_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfx(ctx, w, (op->a1));
	knh_write__sfpidx(ctx, w, (op->a2));
}

static void XMOVo_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_XMOVo_t *op = (klr_XMOVo_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfx(ctx, w, (op->a1));
	knh_write__OBJ(ctx, w, UP((op->a2)));
}

static void XMOVx_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_XMOVx_t *op = (klr_XMOVx_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfx(ctx, w, (op->a1));
	knh_write__sfx(ctx, w, (op->a2));
}

static void XMOVDEF_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_XMOVDEF_t *op = (klr_XMOVDEF_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfx(ctx, w, (op->a1));
	knh_write__cid(ctx, w, (op->a2));
}

static void XMOVSYS_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_XMOVSYS_t *op = (klr_XMOVSYS_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfx(ctx, w, (op->a1));
	knh_write__ushort(ctx, w, (op->a2));
}

static void CHKESP_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_CHKESP_t *op = (klr_CHKESP_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfpidx(ctx, w, (op->a1));
}

static void CHKNULx_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_CHKNULx_t *op = (klr_CHKNULx_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfx(ctx, w, (op->a1));
}

static void FCALL_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_FCALL_t *op = (klr_FCALL_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfpidx(ctx, w, (op->a1));
	knh_write__ushort(ctx, w, (op->a2));
	knh_write__sfpidx(ctx, w, (op->a3));
	knh_write__OBJ(ctx, w, UP((op->a4)));
}

static void SCALL_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_SCALL_t *op = (klr_SCALL_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfpidx(ctx, w, (op->a1));
	knh_write__ushort(ctx, w, (op->a2));
	knh_write__OBJ(ctx, w, UP((op->a3)));
}

static void CALL_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_CALL_t *op = (klr_CALL_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfpidx(ctx, w, (op->a1));
	knh_write__ushort(ctx, w, (op->a2));
	knh_write__mn(ctx, w, (op->a3));
}

static void NEW_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_NEW_t *op = (klr_NEW_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfpidx(ctx, w, (op->a1));
	knh_write__flag(ctx, w, (op->a2));
	knh_write__cid(ctx, w, (op->a3));
	knh_write__ushort(ctx, w, (op->a4));
	knh_write__OBJ(ctx, w, UP((op->a5)));
}

static void STR_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_STR_t *op = (klr_STR_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfpidx(ctx, w, (op->a1));
	knh_write__sfpidx(ctx, w, (op->a2));
	knh_write__mn(ctx, w, (op->a3));
	knh_write__OBJ(ctx, w, UP((op->a4)));
}

static void SSTR_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_SSTR_t *op = (klr_SSTR_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfpidx(ctx, w, (op->a1));
	knh_write__sfpidx(ctx, w, (op->a2));
	knh_write__OBJ(ctx, w, UP((op->a3)));
	knh_write__OBJ(ctx, w, UP((op->a4)));
}

static void SMAP_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_SMAP_t *op = (klr_SMAP_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfpidx(ctx, w, (op->a1));
	knh_write__OBJ(ctx, w, UP((op->a2)));
}

static void JMP_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_JMP_t *op = (klr_JMP_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__addr(ctx, w, (op->a1));
}

static void bJIFT_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_bJIFT_t *op = (klr_bJIFT_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__addr(ctx, w, (op->a1));
	knh_write__sfpidx(ctx, w, (op->a2));
}

static void NEXT_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_NEXT_t *op = (klr_NEXT_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__addr(ctx, w, (op->a1));
	knh_write__sfpidx(ctx, w, (op->a2));
	knh_write__sfpidx(ctx, w, (op->a3));
}

static void INEXT_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_INEXT_t *op = (klr_INEXT_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__addr(ctx, w, (op->a1));
	knh_write__cid(ctx, w, (op->a2));
	knh_write__sfpidx(ctx, w, (op->a3));
	knh_write__sfpidx(ctx, w, (op->a4));
}

static void CATCH_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_CATCH_t *op = (klr_CATCH_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__addr(ctx, w, (op->a1));
	knh_write__sfpidx(ctx, w, (op->a2));
	knh_write__sfpidx(ctx, w, (op->a3));
	knh_write__OBJ(ctx, w, UP((op->a4)));
}

static void THROWs_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_THROWs_t *op = (klr_THROWs_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfpidx(ctx, w, (op->a1));
	knh_write__sfpidx(ctx, w, (op->a2));
	knh_write__OBJ(ctx, w, UP((op->a3)));
}

static void P_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_P_t *op = (klr_P_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__flag(ctx, w, (op->a1));
	knh_write__mn(ctx, w, (op->a2));
	knh_write__sfpidx(ctx, w, (op->a3));
}

static void PMSG_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_PMSG_t *op = (klr_PMSG_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__flag(ctx, w, (op->a1));
	knh_write__OBJ(ctx, w, UP((op->a2)));
}

static void iADD_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_iADD_t *op = (klr_iADD_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfpidx(ctx, w, (op->a1));
	knh_write__sfpidx(ctx, w, (op->a2));
	knh_write__sfpidx(ctx, w, (op->a3));
}

static void iADDn_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_iADDn_t *op = (klr_iADDn_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfpidx(ctx, w, (op->a1));
	knh_write__sfpidx(ctx, w, (op->a2));
	knh_write__int(ctx, w, (op->a3));
}

static void fADDn_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_fADDn_t *op = (klr_fADDn_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfpidx(ctx, w, (op->a1));
	knh_write__sfpidx(ctx, w, (op->a2));
	knh_write__float(ctx, w, (op->a3));
}

static void ARYGETn_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_ARYGETn_t *op = (klr_ARYGETn_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfpidx(ctx, w, (op->a1));
	knh_write__sfpidx(ctx, w, (op->a2));
	knh_write__intptr(ctx, w, (op->a3));
}

static void MOVa_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_MOVa_t *op = (klr_MOVa_t*)c; 
	op->a1 += shift;
	op->a2 += shift;
}

static void MOVo_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_MOVo_t *op = (klr_MOVo_t*)c; 
	op->a1 += shift;
}

static void MOVi_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_MOVi_t *op = (klr_MOVi_t*)c; 
	op->a1 += shift;
}

static void MOVx_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_MOVx_t *op = (klr_MOVx_t*)c; 
	op->a1 += shift;
	op->a2.i += shift;
}

static void MOVDEF_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_MOVDEF_t *op = (klr_MOVDEF_t*)c; 
	op->a1 += shift;
}

static void MOVSYS_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_MOVSYS_t *op = (klr_MOVSYS_t*)c; 
	op->a1 += shift;
}

static void XMOVs_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_XMOVs_t *op = (klr_XMOVs_t*)c; 
	op->a1.i += shift;
	op->a2 += shift;
}

static void XMOVo_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_XMOVo_t *op = (klr_XMOVo_t*)c; 
	op->a1.i += shift;
}

static void XMOVx_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_XMOVx_t *op = (klr_XMOVx_t*)c; 
	op->a1.i += shift;
	op->a2.i += shift;
}

static void XMOVDEF_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_XMOVDEF_t *op = (klr_XMOVDEF_t*)c; 
	op->a1.i += shift;
}

static void XMOVSYS_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_XMOVSYS_t *op = (klr_XMOVSYS_t*)c; 
	op->a1.i += shift;
}

static void CHKESP_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_CHKESP_t *op = (klr_CHKESP_t*)c; 
	op->a1 += shift;
}

static void CHKNULx_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_CHKNULx_t *op = (klr_CHKNULx_t*)c; 
	op->a1.i += shift;
}

static void FCALL_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_FCALL_t *op = (klr_FCALL_t*)c; 
	op->a1 += shift;
	op->a3 += shift;
}

static void SCALL_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_SCALL_t *op = (klr_SCALL_t*)c; 
	op->a1 += shift;
}

static void CALL_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_CALL_t *op = (klr_CALL_t*)c; 
	op->a1 += shift;
}

static void NEW_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_NEW_t *op = (klr_NEW_t*)c; 
	op->a1 += shift;
}

static void STR_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_STR_t *op = (klr_STR_t*)c; 
	op->a1 += shift;
	op->a2 += shift;
}

static void SSTR_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_SSTR_t *op = (klr_SSTR_t*)c; 
	op->a1 += shift;
	op->a2 += shift;
}

static void SMAP_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_SMAP_t *op = (klr_SMAP_t*)c; 
	op->a1 += shift;
}

static void JMP_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_JMP_t *op = (klr_JMP_t*)c; 
	knh_code_t *newpc = ((knh_code_t*)op->a1) + pcshift;
	op->a1 = (knh_KLRInst_t*)newpc;
	THREADEDCODE(op->codeaddr = NULL;)
	THREADEDCODE(op->jumpaddr = NULL;)
}

static void bJIFT_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_bJIFT_t *op = (klr_bJIFT_t*)c; 
	knh_code_t *newpc = ((knh_code_t*)op->a1) + pcshift;
	op->a1 = (knh_KLRInst_t*)newpc;
	THREADEDCODE(op->codeaddr = NULL;)
	THREADEDCODE(op->jumpaddr = NULL;)
	op->a2 += shift;
}

static void NEXT_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_NEXT_t *op = (klr_NEXT_t*)c; 
	knh_code_t *newpc = ((knh_code_t*)op->a1) + pcshift;
	op->a1 = (knh_KLRInst_t*)newpc;
	THREADEDCODE(op->codeaddr = NULL;)
	THREADEDCODE(op->jumpaddr = NULL;)
	op->a2 += shift;
	op->a3 += shift;
}

static void INEXT_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_INEXT_t *op = (klr_INEXT_t*)c; 
	knh_code_t *newpc = ((knh_code_t*)op->a1) + pcshift;
	op->a1 = (knh_KLRInst_t*)newpc;
	THREADEDCODE(op->codeaddr = NULL;)
	THREADEDCODE(op->jumpaddr = NULL;)
	op->a3 += shift;
	op->a4 += shift;
}

static void CATCH_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_CATCH_t *op = (klr_CATCH_t*)c; 
	knh_code_t *newpc = ((knh_code_t*)op->a1) + pcshift;
	op->a1 = (knh_KLRInst_t*)newpc;
	THREADEDCODE(op->codeaddr = NULL;)
	THREADEDCODE(op->jumpaddr = NULL;)
	op->a2 += shift;
	op->a3 += shift;
}

static void THROWs_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_THROWs_t *op = (klr_THROWs_t*)c; 
	op->a1 += shift;
	op->a2 += shift;
}

static void P_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_P_t *op = (klr_P_t*)c; 
	op->a3 += shift;
}

static void iADD_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_iADD_t *op = (klr_iADD_t*)c; 
	op->a1 += shift;
	op->a2 += shift;
	op->a3 += shift;
}

static void iADDn_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_iADDn_t *op = (klr_iADDn_t*)c; 
	op->a1 += shift;
	op->a2 += shift;
}

static void fADDn_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_fADDn_t *op = (klr_fADDn_t*)c; 
	op->a1 += shift;
	op->a2 += shift;
}

static void ARYGETn_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_ARYGETn_t *op = (klr_ARYGETn_t*)c; 
	op->a1 += shift;
	op->a2 += shift;
}

/* ======================================================================== */
/* [data] */

typedef void (*codeftr)(Ctx *, knh_inst_t*, knh_ftraverse);
typedef void (*codedump)(Ctx *, knh_inst_t*, knh_OutputStream_t*);
typedef void (*codeshift)(Ctx *, knh_inst_t*, int, int);

typedef struct knh_OPDATA_t {
	char *name;
	size_t size;
	knh_bool_t hasjump;
	codeftr cftr;
	codedump cdump;
	codeshift cshift;
} knh_OPDATA_t;

static knh_OPDATA_t OPDATA[] = {
	{"HALT", OPSIZE_HALT, 0, HALT_traverse, HALT_dump, HALT_shift}, 
	{"MOVa", OPSIZE_MOVa, 0, HALT_traverse, MOVa_dump, MOVa_shift}, 
	{"MOVn", OPSIZE_MOVn, 0, HALT_traverse, MOVa_dump, MOVa_shift}, 
	{"MOVo", OPSIZE_MOVo, 0, MOVo_traverse, MOVo_dump, MOVo_shift}, 
	{"MOVi", OPSIZE_MOVi, 0, HALT_traverse, MOVi_dump, MOVi_shift}, 
	{"MOVx", OPSIZE_MOVx, 0, HALT_traverse, MOVx_dump, MOVx_shift}, 
	{"MOVDEF", OPSIZE_MOVDEF, 0, HALT_traverse, MOVDEF_dump, MOVDEF_shift}, 
	{"MOVSYS", OPSIZE_MOVSYS, 0, HALT_traverse, MOVSYS_dump, MOVSYS_shift}, 
	{"MOVxi", OPSIZE_MOVxi, 0, HALT_traverse, MOVx_dump, MOVx_shift}, 
	{"MOVxf", OPSIZE_MOVxf, 0, HALT_traverse, MOVx_dump, MOVx_shift}, 
	{"MOVxb", OPSIZE_MOVxb, 0, HALT_traverse, MOVx_dump, MOVx_shift}, 
	{"MOVe", OPSIZE_MOVe, 0, HALT_traverse, MOVSYS_dump, MOVSYS_shift}, 
	{"XMOVs", OPSIZE_XMOVs, 0, HALT_traverse, XMOVs_dump, XMOVs_shift}, 
	{"XMOVo", OPSIZE_XMOVo, 0, XMOVo_traverse, XMOVo_dump, XMOVo_shift}, 
	{"XMOVx", OPSIZE_XMOVx, 0, HALT_traverse, XMOVx_dump, XMOVx_shift}, 
	{"XMOVDEF", OPSIZE_XMOVDEF, 0, HALT_traverse, XMOVDEF_dump, XMOVDEF_shift}, 
	{"XMOVSYS", OPSIZE_XMOVSYS, 0, HALT_traverse, XMOVSYS_dump, XMOVSYS_shift}, 
	{"XMOVsi", OPSIZE_XMOVsi, 0, HALT_traverse, XMOVs_dump, XMOVs_shift}, 
	{"XMOVsf", OPSIZE_XMOVsf, 0, HALT_traverse, XMOVs_dump, XMOVs_shift}, 
	{"XMOVsb", OPSIZE_XMOVsb, 0, HALT_traverse, XMOVs_dump, XMOVs_shift}, 
	{"SWAP", OPSIZE_SWAP, 0, HALT_traverse, MOVa_dump, MOVa_shift}, 
	{"PARAMDEF", OPSIZE_PARAMDEF, 0, HALT_traverse, MOVDEF_dump, MOVDEF_shift}, 
	{"PARAMo", OPSIZE_PARAMo, 0, PARAMo_traverse, MOVo_dump, MOVo_shift}, 
	{"PARAMPROP", OPSIZE_PARAMPROP, 0, HALT_traverse, MOVa_dump, MOVa_shift}, 
	{"PARAMS", OPSIZE_PARAMS, 0, HALT_traverse, MOVDEF_dump, MOVDEF_shift}, 
	{"CHKESP", OPSIZE_CHKESP, 0, HALT_traverse, CHKESP_dump, CHKESP_shift}, 
	{"RET", OPSIZE_RET, 0, HALT_traverse, HALT_dump, HALT_shift}, 
	{"YEILDBREAK", OPSIZE_YEILDBREAK, 0, HALT_traverse, HALT_dump, HALT_shift}, 
	{"BOX", OPSIZE_BOX, 0, HALT_traverse, MOVDEF_dump, MOVDEF_shift}, 
	{"BOXnc", OPSIZE_BOXnc, 0, HALT_traverse, MOVDEF_dump, MOVDEF_shift}, 
	{"NNBOX", OPSIZE_NNBOX, 0, HALT_traverse, MOVDEF_dump, MOVDEF_shift}, 
	{"NNBOXnc", OPSIZE_NNBOXnc, 0, HALT_traverse, MOVDEF_dump, MOVDEF_shift}, 
	{"UNBOX", OPSIZE_UNBOX, 0, HALT_traverse, CHKESP_dump, CHKESP_shift}, 
	{"CHKNUL", OPSIZE_CHKNUL, 0, HALT_traverse, CHKESP_dump, CHKESP_shift}, 
	{"CHKNULx", OPSIZE_CHKNULx, 0, HALT_traverse, CHKNULx_dump, CHKNULx_shift}, 
	{"CHKTYPE", OPSIZE_CHKTYPE, 0, HALT_traverse, MOVDEF_dump, MOVDEF_shift}, 
	{"FCALL", OPSIZE_FCALL, 0, FCALL_traverse, FCALL_dump, FCALL_shift}, 
	{"SCALL", OPSIZE_SCALL, 0, SCALL_traverse, SCALL_dump, SCALL_shift}, 
	{"AINVOKE", OPSIZE_AINVOKE, 0, HALT_traverse, MOVSYS_dump, MOVSYS_shift}, 
	{"CALL", OPSIZE_CALL, 0, HALT_traverse, CALL_dump, CALL_shift}, 
	{"ACALL", OPSIZE_ACALL, 0, HALT_traverse, CALL_dump, CALL_shift}, 
	{"NEW", OPSIZE_NEW, 0, NEW_traverse, NEW_dump, NEW_shift}, 
	{"COPYSFP", OPSIZE_COPYSFP, 0, HALT_traverse, CHKESP_dump, CHKESP_shift}, 
	{"STR", OPSIZE_STR, 0, STR_traverse, STR_dump, STR_shift}, 
	{"SSTR", OPSIZE_SSTR, 0, SSTR_traverse, SSTR_dump, SSTR_shift}, 
	{"SMAP", OPSIZE_SMAP, 0, SMAP_traverse, SMAP_dump, SMAP_shift}, 
	{"SMAPnc", OPSIZE_SMAPnc, 0, SMAPnc_traverse, SMAP_dump, SMAP_shift}, 
	{"MAP", OPSIZE_MAP, 0, HALT_traverse, MOVDEF_dump, MOVDEF_shift}, 
	{"MAPnc", OPSIZE_MAPnc, 0, HALT_traverse, MOVDEF_dump, MOVDEF_shift}, 
	{"AMAP", OPSIZE_AMAP, 0, HALT_traverse, MOVDEF_dump, MOVDEF_shift}, 
	{"NNMAP", OPSIZE_NNMAP, 0, HALT_traverse, MOVDEF_dump, MOVDEF_shift}, 
	{"JMP", OPSIZE_JMP, 1, HALT_traverse, JMP_dump, JMP_shift}, 
	{"SKIP", OPSIZE_SKIP, 1, HALT_traverse, JMP_dump, JMP_shift}, 
	{"bJIFT", OPSIZE_bJIFT, 1, HALT_traverse, bJIFT_dump, bJIFT_shift}, 
	{"bJIFF", OPSIZE_bJIFF, 1, HALT_traverse, bJIFT_dump, bJIFT_shift}, 
	{"bJIFF_LOOP", OPSIZE_bJIFF_LOOP, 1, HALT_traverse, bJIFT_dump, bJIFT_shift}, 
	{"JIFNUL", OPSIZE_JIFNUL, 1, HALT_traverse, bJIFT_dump, bJIFT_shift}, 
	{"JIFNN", OPSIZE_JIFNN, 1, HALT_traverse, bJIFT_dump, bJIFT_shift}, 
	{"NEXT", OPSIZE_NEXT, 1, HALT_traverse, NEXT_dump, NEXT_shift}, 
	{"INEXT", OPSIZE_INEXT, 1, HALT_traverse, INEXT_dump, INEXT_shift}, 
	{"TRY", OPSIZE_TRY, 1, HALT_traverse, bJIFT_dump, bJIFT_shift}, 
	{"TRYEND", OPSIZE_TRYEND, 0, HALT_traverse, CHKESP_dump, CHKESP_shift}, 
	{"CATCH", OPSIZE_CATCH, 1, CATCH_traverse, CATCH_dump, CATCH_shift}, 
	{"PUSH", OPSIZE_PUSH, 0, HALT_traverse, CHKESP_dump, CHKESP_shift}, 
	{"POP", OPSIZE_POP, 0, HALT_traverse, CHKESP_dump, CHKESP_shift}, 
	{"THROW", OPSIZE_THROW, 0, HALT_traverse, MOVa_dump, MOVa_shift}, 
	{"THROWs", OPSIZE_THROWs, 0, THROWs_traverse, THROWs_dump, THROWs_shift}, 
	{"THROW_AGAIN", OPSIZE_THROW_AGAIN, 0, HALT_traverse, CHKESP_dump, CHKESP_shift}, 
	{"P", OPSIZE_P, 0, HALT_traverse, P_dump, P_shift}, 
	{"PMSG", OPSIZE_PMSG, 0, PMSG_traverse, PMSG_dump, HALT_shift}, 
	{"iCAST", OPSIZE_iCAST, 0, HALT_traverse, CHKESP_dump, CHKESP_shift}, 
	{"inCAST", OPSIZE_inCAST, 0, HALT_traverse, CHKESP_dump, CHKESP_shift}, 
	{"fCAST", OPSIZE_fCAST, 0, HALT_traverse, CHKESP_dump, CHKESP_shift}, 
	{"fnCAST", OPSIZE_fnCAST, 0, HALT_traverse, CHKESP_dump, CHKESP_shift}, 
	{"bNOT", OPSIZE_bNOT, 0, HALT_traverse, MOVa_dump, MOVa_shift}, 
	{"iNEG", OPSIZE_iNEG, 0, HALT_traverse, MOVa_dump, MOVa_shift}, 
	{"iADD", OPSIZE_iADD, 0, HALT_traverse, iADD_dump, iADD_shift}, 
	{"iADDn", OPSIZE_iADDn, 0, HALT_traverse, iADDn_dump, iADDn_shift}, 
	{"iSUB", OPSIZE_iSUB, 0, HALT_traverse, iADD_dump, iADD_shift}, 
	{"iSUBn", OPSIZE_iSUBn, 0, HALT_traverse, iADDn_dump, iADDn_shift}, 
	{"iMUL", OPSIZE_iMUL, 0, HALT_traverse, iADD_dump, iADD_shift}, 
	{"iMULn", OPSIZE_iMULn, 0, HALT_traverse, iADDn_dump, iADDn_shift}, 
	{"iDIV", OPSIZE_iDIV, 0, HALT_traverse, iADD_dump, iADD_shift}, 
	{"iDIVn", OPSIZE_iDIVn, 0, HALT_traverse, iADDn_dump, iADDn_shift}, 
	{"iMOD", OPSIZE_iMOD, 0, HALT_traverse, iADD_dump, iADD_shift}, 
	{"iMODn", OPSIZE_iMODn, 0, HALT_traverse, iADDn_dump, iADDn_shift}, 
	{"iEQ", OPSIZE_iEQ, 0, HALT_traverse, iADD_dump, iADD_shift}, 
	{"iEQn", OPSIZE_iEQn, 0, HALT_traverse, iADDn_dump, iADDn_shift}, 
	{"iNEQ", OPSIZE_iNEQ, 0, HALT_traverse, iADD_dump, iADD_shift}, 
	{"iNEQn", OPSIZE_iNEQn, 0, HALT_traverse, iADDn_dump, iADDn_shift}, 
	{"iLT", OPSIZE_iLT, 0, HALT_traverse, iADD_dump, iADD_shift}, 
	{"iLTn", OPSIZE_iLTn, 0, HALT_traverse, iADDn_dump, iADDn_shift}, 
	{"iLTE", OPSIZE_iLTE, 0, HALT_traverse, iADD_dump, iADD_shift}, 
	{"iLTEn", OPSIZE_iLTEn, 0, HALT_traverse, iADDn_dump, iADDn_shift}, 
	{"iGT", OPSIZE_iGT, 0, HALT_traverse, iADD_dump, iADD_shift}, 
	{"iGTn", OPSIZE_iGTn, 0, HALT_traverse, iADDn_dump, iADDn_shift}, 
	{"iGTE", OPSIZE_iGTE, 0, HALT_traverse, iADD_dump, iADD_shift}, 
	{"iGTEn", OPSIZE_iGTEn, 0, HALT_traverse, iADDn_dump, iADDn_shift}, 
	{"fNEG", OPSIZE_fNEG, 0, HALT_traverse, MOVa_dump, MOVa_shift}, 
	{"fADD", OPSIZE_fADD, 0, HALT_traverse, iADD_dump, iADD_shift}, 
	{"fADDn", OPSIZE_fADDn, 0, HALT_traverse, fADDn_dump, fADDn_shift}, 
	{"fSUB", OPSIZE_fSUB, 0, HALT_traverse, iADD_dump, iADD_shift}, 
	{"fSUBn", OPSIZE_fSUBn, 0, HALT_traverse, fADDn_dump, fADDn_shift}, 
	{"fMUL", OPSIZE_fMUL, 0, HALT_traverse, iADD_dump, iADD_shift}, 
	{"fMULn", OPSIZE_fMULn, 0, HALT_traverse, fADDn_dump, fADDn_shift}, 
	{"fDIV", OPSIZE_fDIV, 0, HALT_traverse, iADD_dump, iADD_shift}, 
	{"fDIVn", OPSIZE_fDIVn, 0, HALT_traverse, fADDn_dump, fADDn_shift}, 
	{"fEQ", OPSIZE_fEQ, 0, HALT_traverse, iADD_dump, iADD_shift}, 
	{"fEQn", OPSIZE_fEQn, 0, HALT_traverse, fADDn_dump, fADDn_shift}, 
	{"fNEQ", OPSIZE_fNEQ, 0, HALT_traverse, iADD_dump, iADD_shift}, 
	{"fNEQn", OPSIZE_fNEQn, 0, HALT_traverse, fADDn_dump, fADDn_shift}, 
	{"fLT", OPSIZE_fLT, 0, HALT_traverse, iADD_dump, iADD_shift}, 
	{"fLTn", OPSIZE_fLTn, 0, HALT_traverse, fADDn_dump, fADDn_shift}, 
	{"fLTE", OPSIZE_fLTE, 0, HALT_traverse, iADD_dump, iADD_shift}, 
	{"fLTEn", OPSIZE_fLTEn, 0, HALT_traverse, fADDn_dump, fADDn_shift}, 
	{"fGT", OPSIZE_fGT, 0, HALT_traverse, iADD_dump, iADD_shift}, 
	{"fGTn", OPSIZE_fGTn, 0, HALT_traverse, fADDn_dump, fADDn_shift}, 
	{"fGTE", OPSIZE_fGTE, 0, HALT_traverse, iADD_dump, iADD_shift}, 
	{"fGTEn", OPSIZE_fGTEn, 0, HALT_traverse, fADDn_dump, fADDn_shift}, 
	{"SIZE", OPSIZE_SIZE, 0, HALT_traverse, MOVa_dump, MOVa_shift}, 
	{"ARYGET", OPSIZE_ARYGET, 0, HALT_traverse, iADD_dump, iADD_shift}, 
	{"ARYGETn", OPSIZE_ARYGETn, 0, HALT_traverse, ARYGETn_dump, ARYGETn_shift}, 
	{"iARYGET", OPSIZE_iARYGET, 0, HALT_traverse, iADD_dump, iADD_shift}, 
	{"iARYGETn", OPSIZE_iARYGETn, 0, HALT_traverse, ARYGETn_dump, ARYGETn_shift}, 
	{"fARYGET", OPSIZE_fARYGET, 0, HALT_traverse, iADD_dump, iADD_shift}, 
	{"fARYGETn", OPSIZE_fARYGETn, 0, HALT_traverse, ARYGETn_dump, ARYGETn_shift}, 
	{"ARYSET", OPSIZE_ARYSET, 0, HALT_traverse, iADD_dump, iADD_shift}, 
	{"ARYSETn", OPSIZE_ARYSETn, 0, HALT_traverse, ARYGETn_dump, ARYGETn_shift}, 
	{"iARYSET", OPSIZE_iARYSET, 0, HALT_traverse, iADD_dump, iADD_shift}, 
	{"iARYSETn", OPSIZE_iARYSETn, 0, HALT_traverse, ARYGETn_dump, ARYGETn_shift}, 
	{"fARYSET", OPSIZE_fARYSET, 0, HALT_traverse, iADD_dump, iADD_shift}, 
	{"fARYSETn", OPSIZE_fARYSETn, 0, HALT_traverse, ARYGETn_dump, ARYGETn_shift}, 
	{"THCODE", OPSIZE_THCODE, 0, HALT_traverse, CHKESP_dump, CHKESP_shift}, 
	{"NOP", OPSIZE_NOP, 0, HALT_traverse, HALT_dump, HALT_shift}, 
};

/* ------------------------------------------------------------------------ */

char *knh_opcode_tochar(int opcode)
{
	return OPDATA[opcode].name;
}

/* ------------------------------------------------------------------------ */

size_t knh_opcode_size(int opcode)
{
	return OPDATA[opcode].size;
}

/* ------------------------------------------------------------------------ */

knh_bool_t knh_opcode_hasjump(int opcode)
{
	return OPDATA[opcode].hasjump;
}

/* ------------------------------------------------------------------------ */

void knh_opcode_traverse(Ctx *ctx, knh_inst_t *c, knh_ftraverse ftr)
{
	int opcode = KNH_OPCODE(c);
	OPDATA[opcode].cftr(ctx, c, ftr);
}

/* ------------------------------------------------------------------------ */

void knh_opcode_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	int opcode = KNH_OPCODE(c);
	OPDATA[opcode].cdump(ctx, c, w);
}

/* ------------------------------------------------------------------------ */

void knh_opcode_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	int opcode = KNH_OPCODE(c);
	OPDATA[opcode].cshift(ctx, c, shift, pcshift);
}


/* ======================================================================== */
/* [exec] */

#ifdef KNH_USING_THREADEDCODE
#define CASE(x)   L_##x :
#define NEXT_OP  *(op->nextaddr)
#define JUMP  *(op->jumpaddr)
#define TC(c)   c
#define DISPATCH_START(pc) goto *OPJUMP[KNH_OPCODE(pc)];
#define DISPATCH_END(pc)
#else
#define OPJUMP NULL
#define CASE(x)   case OPCODE_##x :
#define NEXT_OP L_HEAD
#define JUMP L_HEAD
#define TC(c)    
#define DISPATCH_START(pc) L_HEAD:;switch(KNH_OPCODE(pc)) {
#define DISPATCH_END(pc) 	}	KNH_WARNING(ctx, "unknown opcode=%d", KNH_OPCODE(pc)); 	goto L_HEAD;
#endif/*KNH_USING_THREADEDCODE*/

METHOD knh_KLRCode_exec(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
#ifdef KNH_USING_THREADEDCODE
	static void *OPJUMP[] = {
		&&L_HALT, &&L_MOVa, &&L_MOVn, &&L_MOVo, 
		&&L_MOVi, &&L_MOVx, &&L_MOVDEF, &&L_MOVSYS, 
		&&L_MOVxi, &&L_MOVxf, &&L_MOVxb, &&L_MOVe, 
		&&L_XMOVs, &&L_XMOVo, &&L_XMOVx, &&L_XMOVDEF, 
		&&L_XMOVSYS, &&L_XMOVsi, &&L_XMOVsf, &&L_XMOVsb, 
		&&L_SWAP, &&L_PARAMDEF, &&L_PARAMo, &&L_PARAMPROP, 
		&&L_PARAMS, &&L_CHKESP, &&L_RET, &&L_YEILDBREAK, 
		&&L_BOX, &&L_BOXnc, &&L_NNBOX, &&L_NNBOXnc, 
		&&L_UNBOX, &&L_CHKNUL, &&L_CHKNULx, &&L_CHKTYPE, 
		&&L_FCALL, &&L_SCALL, &&L_AINVOKE, &&L_CALL, 
		&&L_ACALL, &&L_NEW, &&L_COPYSFP, &&L_STR, 
		&&L_SSTR, &&L_SMAP, &&L_SMAPnc, &&L_MAP, 
		&&L_MAPnc, &&L_AMAP, &&L_NNMAP, &&L_JMP, 
		&&L_SKIP, &&L_bJIFT, &&L_bJIFF, &&L_bJIFF_LOOP, 
		&&L_JIFNUL, &&L_JIFNN, &&L_NEXT, &&L_INEXT, 
		&&L_TRY, &&L_TRYEND, &&L_CATCH, &&L_PUSH, 
		&&L_POP, &&L_THROW, &&L_THROWs, &&L_THROW_AGAIN, 
		&&L_P, &&L_PMSG, &&L_iCAST, &&L_inCAST, 
		&&L_fCAST, &&L_fnCAST, &&L_bNOT, &&L_iNEG, 
		&&L_iADD, &&L_iADDn, &&L_iSUB, &&L_iSUBn, 
		&&L_iMUL, &&L_iMULn, &&L_iDIV, &&L_iDIVn, 
		&&L_iMOD, &&L_iMODn, &&L_iEQ, &&L_iEQn, 
		&&L_iNEQ, &&L_iNEQn, &&L_iLT, &&L_iLTn, 
		&&L_iLTE, &&L_iLTEn, &&L_iGT, &&L_iGTn, 
		&&L_iGTE, &&L_iGTEn, &&L_fNEG, &&L_fADD, 
		&&L_fADDn, &&L_fSUB, &&L_fSUBn, &&L_fMUL, 
		&&L_fMULn, &&L_fDIV, &&L_fDIVn, &&L_fEQ, 
		&&L_fEQn, &&L_fNEQ, &&L_fNEQn, &&L_fLT, 
		&&L_fLTn, &&L_fLTE, &&L_fLTEn, &&L_fGT, 
		&&L_fGTn, &&L_fGTE, &&L_fGTEn, &&L_SIZE, 
		&&L_ARYGET, &&L_ARYGETn, &&L_iARYGET, &&L_iARYGETn, 
		&&L_fARYGET, &&L_fARYGETn, &&L_ARYSET, &&L_ARYSETn, 
		&&L_iARYSET, &&L_iARYSETn, &&L_fARYSET, &&L_fARYSETn, 
		&&L_THCODE, &&L_NOP, 
	};
#endif
	register knh_code_t *pc = (sfp[-1].mtd)->pc_start;
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
		const klr_CHKESP_t *op = (klr_CHKESP_t*)pc;
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
		KLR_JMP(ctx, pc = ((knh_inst_t*)op)->jumppc, JUMP);
		pc += OPSIZE_JMP;
		goto NEXT_OP;
	} 
	CASE(SKIP) {
		const klr_SKIP_t *op = (klr_SKIP_t*)pc;
		KLR_SKIP(ctx, pc = ((knh_inst_t*)op)->jumppc, JUMP);
		pc += OPSIZE_SKIP;
		goto NEXT_OP;
	} 
	CASE(bJIFT) {
		const klr_bJIFT_t *op = (klr_bJIFT_t*)pc;
		KLR_bJIFT(ctx, pc = ((knh_inst_t*)op)->jumppc, JUMP, op->a2);
		pc += OPSIZE_bJIFT;
		goto NEXT_OP;
	} 
	CASE(bJIFF) {
		const klr_bJIFF_t *op = (klr_bJIFF_t*)pc;
		KLR_bJIFF(ctx, pc = ((knh_inst_t*)op)->jumppc, JUMP, op->a2);
		pc += OPSIZE_bJIFF;
		goto NEXT_OP;
	} 
	CASE(bJIFF_LOOP) {
		const klr_bJIFF_LOOP_t *op = (klr_bJIFF_LOOP_t*)pc;
		KLR_bJIFF_LOOP(ctx, pc = ((knh_inst_t*)op)->jumppc, JUMP, op->a2);
		pc += OPSIZE_bJIFF_LOOP;
		goto NEXT_OP;
	} 
	CASE(JIFNUL) {
		const klr_JIFNUL_t *op = (klr_JIFNUL_t*)pc;
		KLR_JIFNUL(ctx, pc = ((knh_inst_t*)op)->jumppc, JUMP, op->a2);
		pc += OPSIZE_JIFNUL;
		goto NEXT_OP;
	} 
	CASE(JIFNN) {
		const klr_JIFNN_t *op = (klr_JIFNN_t*)pc;
		KLR_JIFNN(ctx, pc = ((knh_inst_t*)op)->jumppc, JUMP, op->a2);
		pc += OPSIZE_JIFNN;
		goto NEXT_OP;
	} 
	CASE(NEXT) {
		const klr_NEXT_t *op = (klr_NEXT_t*)pc;
		KLR_NEXT(ctx, pc = ((knh_inst_t*)op)->jumppc, JUMP, op->a2, op->a3);
		pc += OPSIZE_NEXT;
		goto NEXT_OP;
	} 
	CASE(INEXT) {
		const klr_INEXT_t *op = (klr_INEXT_t*)pc;
		KLR_INEXT(ctx, pc = ((knh_inst_t*)op)->jumppc, JUMP, op->a2, op->a3, op->a4);
		pc += OPSIZE_INEXT;
		goto NEXT_OP;
	} 
	CASE(TRY) {
		const klr_TRY_t *op = (klr_TRY_t*)pc;
		KLR_TRY(ctx, pc = ((knh_inst_t*)op)->jumppc, JUMP, op->a2);
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
		KLR_CATCH(ctx, pc = ((knh_inst_t*)op)->jumppc, JUMP, op->a2, op->a3, op->a4);
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
		const klr_THCODE_t *op = (klr_THCODE_t*)pc;
		KLR_THCODE(ctx, op->a1);
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

#ifdef __cplusplus
}
#endif
