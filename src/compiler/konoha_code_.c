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
static void knh_write__type(Ctx *ctx, knh_OutputStream_t* w, knh_type_t a)
{
	knh_putc(ctx, w, ' '); knh_write_type(ctx, w, (knh_type_t)a);
}

static void HALT_traverse(Ctx *ctx, knh_inst_t *c, knh_ftraverse ftr)
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
	DBG2_ASSERT(op->opcode == 8);
	ftr(ctx, UP(op->a2));
}

static void XMOVoi_traverse(Ctx *ctx, knh_inst_t *c, knh_ftraverse ftr)
{
	klr_XMOVoi_t *op = (klr_XMOVoi_t*)c; 
	DBG2_ASSERT(op->opcode == 14);
	ftr(ctx, UP(op->a2));
}

static void XMOVof_traverse(Ctx *ctx, knh_inst_t *c, knh_ftraverse ftr)
{
	klr_XMOVof_t *op = (klr_XMOVof_t*)c; 
	DBG2_ASSERT(op->opcode == 20);
	ftr(ctx, UP(op->a2));
}

static void XMOVob_traverse(Ctx *ctx, knh_inst_t *c, knh_ftraverse ftr)
{
	klr_XMOVob_t *op = (klr_XMOVob_t*)c; 
	DBG2_ASSERT(op->opcode == 26);
	ftr(ctx, UP(op->a2));
}

static void EMOVo_traverse(Ctx *ctx, knh_inst_t *c, knh_ftraverse ftr)
{
	klr_EMOVo_t *op = (klr_EMOVo_t*)c; 
	DBG2_ASSERT(op->opcode == 29);
	ftr(ctx, UP(op->a2));
}

static void PARAMo_traverse(Ctx *ctx, knh_inst_t *c, knh_ftraverse ftr)
{
	klr_PARAMo_t *op = (klr_PARAMo_t*)c; 
	DBG2_ASSERT(op->opcode == 35);
	ftr(ctx, UP(op->a2));
}

static void RETo_traverse(Ctx *ctx, knh_inst_t *c, knh_ftraverse ftr)
{
	klr_RETo_t *op = (klr_RETo_t*)c; 
	DBG2_ASSERT(op->opcode == 42);
	ftr(ctx, UP(op->a2));
}

static void FCALL_traverse(Ctx *ctx, knh_inst_t *c, knh_ftraverse ftr)
{
	klr_FCALL_t *op = (klr_FCALL_t*)c; 
	DBG2_ASSERT(op->opcode == 54);
	ftr(ctx, UP(op->a4));
}

static void SCALL_traverse(Ctx *ctx, knh_inst_t *c, knh_ftraverse ftr)
{
	klr_SCALL_t *op = (klr_SCALL_t*)c; 
	DBG2_ASSERT(op->opcode == 55);
	ftr(ctx, UP(op->a3));
}

static void NEW_traverse(Ctx *ctx, knh_inst_t *c, knh_ftraverse ftr)
{
	klr_NEW_t *op = (klr_NEW_t*)c; 
	DBG2_ASSERT(op->opcode == 59);
	ftr(ctx, UP(op->a5));
}

static void TOSTRf_traverse(Ctx *ctx, knh_inst_t *c, knh_ftraverse ftr)
{
	klr_TOSTRf_t *op = (klr_TOSTRf_t*)c; 
	DBG2_ASSERT(op->opcode == 62);
	ftr(ctx, UP(op->a3));
}

static void SMAP_traverse(Ctx *ctx, knh_inst_t *c, knh_ftraverse ftr)
{
	klr_SMAP_t *op = (klr_SMAP_t*)c; 
	DBG2_ASSERT(op->opcode == 63);
	ftr(ctx, UP(op->a2));
}

static void SMAPnc_traverse(Ctx *ctx, knh_inst_t *c, knh_ftraverse ftr)
{
	klr_SMAPnc_t *op = (klr_SMAPnc_t*)c; 
	DBG2_ASSERT(op->opcode == 64);
	ftr(ctx, UP(op->a2));
}

static void CATCH_traverse(Ctx *ctx, knh_inst_t *c, knh_ftraverse ftr)
{
	klr_CATCH_t *op = (klr_CATCH_t*)c; 
	DBG2_ASSERT(op->opcode == 80);
	ftr(ctx, UP(op->a4));
}

static void THROWs_traverse(Ctx *ctx, knh_inst_t *c, knh_ftraverse ftr)
{
	klr_THROWs_t *op = (klr_THROWs_t*)c; 
	DBG2_ASSERT(op->opcode == 84);
	ftr(ctx, UP(op->a2));
}

static void PMSG_traverse(Ctx *ctx, knh_inst_t *c, knh_ftraverse ftr)
{
	klr_PMSG_t *op = (klr_PMSG_t*)c; 
	DBG2_ASSERT(op->opcode == 87);
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

static void XMOVxBXi_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_XMOVxBXi_t *op = (klr_XMOVxBXi_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfx(ctx, w, (op->a1));
	knh_write__sfx(ctx, w, (op->a2));
	knh_write__cid(ctx, w, (op->a3));
}

static void EMOVs_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_EMOVs_t *op = (klr_EMOVs_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfe(ctx, w, (op->a1));
	knh_write__sfe(ctx, w, (op->a2));
}

static void EMOVo_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_EMOVo_t *op = (klr_EMOVo_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfe(ctx, w, (op->a1));
	knh_write__OBJ(ctx, w, UP((op->a2)));
}

static void EMOVDEF_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_EMOVDEF_t *op = (klr_EMOVDEF_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfe(ctx, w, (op->a1));
	knh_write__cid(ctx, w, (op->a2));
}

static void EMOVSYS_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_EMOVSYS_t *op = (klr_EMOVSYS_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfe(ctx, w, (op->a1));
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

static void TOSTR_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_TOSTR_t *op = (klr_TOSTR_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfpidx(ctx, w, (op->a1));
	knh_write__mn(ctx, w, (op->a2));
}

static void TOSTRf_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_TOSTRf_t *op = (klr_TOSTRf_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfpidx(ctx, w, (op->a1));
	knh_write__mn(ctx, w, (op->a2));
	knh_write__OBJ(ctx, w, UP((op->a3)));
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

static void THROW_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_THROW_t *op = (klr_THROW_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__ushort(ctx, w, (op->a1));
	knh_write__sfpidx(ctx, w, (op->a2));
}

static void THROWs_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_THROWs_t *op = (klr_THROWs_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__ushort(ctx, w, (op->a1));
	knh_write__OBJ(ctx, w, UP((op->a2)));
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

/* ======================================================================== */
/* [data] */

typedef void (*codeftr)(Ctx *, knh_inst_t*, knh_ftraverse);
typedef void (*codedump)(Ctx *, knh_inst_t*, knh_OutputStream_t*);

typedef struct knh_OPDATA_t {
	char *name;
	size_t size;
	knh_bool_t hasjump;
	codeftr cftr;
	codedump cdump;
} knh_OPDATA_t;

static knh_OPDATA_t OPDATA[] = {
	{"HALT", OPSIZE_HALT, 0, HALT_traverse, HALT_dump}, 
	{"MOVa", OPSIZE_MOVa, 0, HALT_traverse, MOVa_dump}, 
	{"MOVn", OPSIZE_MOVn, 0, HALT_traverse, MOVa_dump}, 
	{"MOVo", OPSIZE_MOVo, 0, MOVo_traverse, MOVo_dump}, 
	{"MOVx", OPSIZE_MOVx, 0, HALT_traverse, MOVx_dump}, 
	{"MOVDEF", OPSIZE_MOVDEF, 0, HALT_traverse, MOVDEF_dump}, 
	{"MOVSYS", OPSIZE_MOVSYS, 0, HALT_traverse, MOVSYS_dump}, 
	{"XMOVs", OPSIZE_XMOVs, 0, HALT_traverse, XMOVs_dump}, 
	{"XMOVo", OPSIZE_XMOVo, 0, XMOVo_traverse, XMOVo_dump}, 
	{"XMOVx", OPSIZE_XMOVx, 0, HALT_traverse, XMOVx_dump}, 
	{"XMOVDEF", OPSIZE_XMOVDEF, 0, HALT_traverse, XMOVDEF_dump}, 
	{"XMOVSYS", OPSIZE_XMOVSYS, 0, HALT_traverse, XMOVSYS_dump}, 
	{"MOVxi", OPSIZE_MOVxi, 0, HALT_traverse, MOVx_dump}, 
	{"XMOVsi", OPSIZE_XMOVsi, 0, HALT_traverse, XMOVs_dump}, 
	{"XMOVoi", OPSIZE_XMOVoi, 0, XMOVoi_traverse, XMOVo_dump}, 
	{"XMOVxi", OPSIZE_XMOVxi, 0, HALT_traverse, XMOVx_dump}, 
	{"XMOVxio", OPSIZE_XMOVxio, 0, HALT_traverse, XMOVx_dump}, 
	{"XMOVxBXi", OPSIZE_XMOVxBXi, 0, HALT_traverse, XMOVxBXi_dump}, 
	{"MOVxf", OPSIZE_MOVxf, 0, HALT_traverse, MOVx_dump}, 
	{"XMOVsf", OPSIZE_XMOVsf, 0, HALT_traverse, XMOVs_dump}, 
	{"XMOVof", OPSIZE_XMOVof, 0, XMOVof_traverse, XMOVo_dump}, 
	{"XMOVxf", OPSIZE_XMOVxf, 0, HALT_traverse, XMOVx_dump}, 
	{"XMOVxfo", OPSIZE_XMOVxfo, 0, HALT_traverse, XMOVx_dump}, 
	{"XMOVxBXf", OPSIZE_XMOVxBXf, 0, HALT_traverse, XMOVxBXi_dump}, 
	{"MOVxb", OPSIZE_MOVxb, 0, HALT_traverse, MOVx_dump}, 
	{"XMOVsb", OPSIZE_XMOVsb, 0, HALT_traverse, XMOVs_dump}, 
	{"XMOVob", OPSIZE_XMOVob, 0, XMOVob_traverse, XMOVo_dump}, 
	{"XMOVxb", OPSIZE_XMOVxb, 0, HALT_traverse, XMOVx_dump}, 
	{"EMOVs", OPSIZE_EMOVs, 0, HALT_traverse, EMOVs_dump}, 
	{"EMOVo", OPSIZE_EMOVo, 0, EMOVo_traverse, EMOVo_dump}, 
	{"EMOVe", OPSIZE_EMOVe, 0, HALT_traverse, EMOVs_dump}, 
	{"EMOVDEF", OPSIZE_EMOVDEF, 0, HALT_traverse, EMOVDEF_dump}, 
	{"EMOVSYS", OPSIZE_EMOVSYS, 0, HALT_traverse, EMOVSYS_dump}, 
	{"SWAP", OPSIZE_SWAP, 0, HALT_traverse, MOVa_dump}, 
	{"PARAMDEF", OPSIZE_PARAMDEF, 0, HALT_traverse, MOVDEF_dump}, 
	{"PARAMo", OPSIZE_PARAMo, 0, PARAMo_traverse, MOVo_dump}, 
	{"PARAMPROP", OPSIZE_PARAMPROP, 0, HALT_traverse, MOVa_dump}, 
	{"PARAMS", OPSIZE_PARAMS, 0, HALT_traverse, MOVDEF_dump}, 
	{"CHKESP", OPSIZE_CHKESP, 0, HALT_traverse, CHKESP_dump}, 
	{"RET", OPSIZE_RET, 0, HALT_traverse, HALT_dump}, 
	{"RETn", OPSIZE_RETn, 0, HALT_traverse, MOVa_dump}, 
	{"RETa", OPSIZE_RETa, 0, HALT_traverse, MOVa_dump}, 
	{"RETo", OPSIZE_RETo, 0, RETo_traverse, MOVo_dump}, 
	{"RETx", OPSIZE_RETx, 0, HALT_traverse, MOVx_dump}, 
	{"YEILDBREAK", OPSIZE_YEILDBREAK, 0, HALT_traverse, HALT_dump}, 
	{"BOX", OPSIZE_BOX, 0, HALT_traverse, MOVDEF_dump}, 
	{"BOXnc", OPSIZE_BOXnc, 0, HALT_traverse, MOVDEF_dump}, 
	{"NNBOX", OPSIZE_NNBOX, 0, HALT_traverse, MOVDEF_dump}, 
	{"NNBOXnc", OPSIZE_NNBOXnc, 0, HALT_traverse, MOVDEF_dump}, 
	{"UNBOX", OPSIZE_UNBOX, 0, HALT_traverse, CHKESP_dump}, 
	{"CHKNUL", OPSIZE_CHKNUL, 0, HALT_traverse, CHKESP_dump}, 
	{"CHKNULx", OPSIZE_CHKNULx, 0, HALT_traverse, CHKNULx_dump}, 
	{"CHKTYPE", OPSIZE_CHKTYPE, 0, HALT_traverse, MOVDEF_dump}, 
	{"NCALL", OPSIZE_NCALL, 0, HALT_traverse, HALT_dump}, 
	{"FCALL", OPSIZE_FCALL, 0, FCALL_traverse, FCALL_dump}, 
	{"SCALL", OPSIZE_SCALL, 0, SCALL_traverse, SCALL_dump}, 
	{"AINVOKE", OPSIZE_AINVOKE, 0, HALT_traverse, MOVSYS_dump}, 
	{"CALL", OPSIZE_CALL, 0, HALT_traverse, CALL_dump}, 
	{"ACALL", OPSIZE_ACALL, 0, HALT_traverse, CALL_dump}, 
	{"NEW", OPSIZE_NEW, 0, NEW_traverse, NEW_dump}, 
	{"COPYSFP", OPSIZE_COPYSFP, 0, HALT_traverse, CHKESP_dump}, 
	{"TOSTR", OPSIZE_TOSTR, 0, HALT_traverse, TOSTR_dump}, 
	{"TOSTRf", OPSIZE_TOSTRf, 0, TOSTRf_traverse, TOSTRf_dump}, 
	{"SMAP", OPSIZE_SMAP, 0, SMAP_traverse, SMAP_dump}, 
	{"SMAPnc", OPSIZE_SMAPnc, 0, SMAPnc_traverse, SMAP_dump}, 
	{"MAP", OPSIZE_MAP, 0, HALT_traverse, MOVDEF_dump}, 
	{"MAPnc", OPSIZE_MAPnc, 0, HALT_traverse, MOVDEF_dump}, 
	{"AMAP", OPSIZE_AMAP, 0, HALT_traverse, MOVDEF_dump}, 
	{"NNMAP", OPSIZE_NNMAP, 0, HALT_traverse, MOVDEF_dump}, 
	{"JMP", OPSIZE_JMP, 1, HALT_traverse, JMP_dump}, 
	{"SKIP", OPSIZE_SKIP, 1, HALT_traverse, JMP_dump}, 
	{"bJIFT", OPSIZE_bJIFT, 1, HALT_traverse, bJIFT_dump}, 
	{"bJIFF", OPSIZE_bJIFF, 1, HALT_traverse, bJIFT_dump}, 
	{"bJIFF_LOOP", OPSIZE_bJIFF_LOOP, 1, HALT_traverse, bJIFT_dump}, 
	{"JIFNUL", OPSIZE_JIFNUL, 1, HALT_traverse, bJIFT_dump}, 
	{"JIFNN", OPSIZE_JIFNN, 1, HALT_traverse, bJIFT_dump}, 
	{"NEXT", OPSIZE_NEXT, 1, HALT_traverse, NEXT_dump}, 
	{"INEXT", OPSIZE_INEXT, 1, HALT_traverse, INEXT_dump}, 
	{"TRY", OPSIZE_TRY, 1, HALT_traverse, bJIFT_dump}, 
	{"TRYEND", OPSIZE_TRYEND, 0, HALT_traverse, CHKESP_dump}, 
	{"CATCH", OPSIZE_CATCH, 1, CATCH_traverse, CATCH_dump}, 
	{"PUSH", OPSIZE_PUSH, 0, HALT_traverse, CHKESP_dump}, 
	{"POP", OPSIZE_POP, 0, HALT_traverse, CHKESP_dump}, 
	{"THROW", OPSIZE_THROW, 0, HALT_traverse, THROW_dump}, 
	{"THROWs", OPSIZE_THROWs, 0, THROWs_traverse, THROWs_dump}, 
	{"THROW_AGAIN", OPSIZE_THROW_AGAIN, 0, HALT_traverse, CHKESP_dump}, 
	{"P", OPSIZE_P, 0, HALT_traverse, P_dump}, 
	{"PMSG", OPSIZE_PMSG, 0, PMSG_traverse, PMSG_dump}, 
	{"iCAST", OPSIZE_iCAST, 0, HALT_traverse, CHKESP_dump}, 
	{"inCAST", OPSIZE_inCAST, 0, HALT_traverse, CHKESP_dump}, 
	{"fCAST", OPSIZE_fCAST, 0, HALT_traverse, CHKESP_dump}, 
	{"fnCAST", OPSIZE_fnCAST, 0, HALT_traverse, CHKESP_dump}, 
	{"bNOT", OPSIZE_bNOT, 0, HALT_traverse, MOVa_dump}, 
	{"iNEG", OPSIZE_iNEG, 0, HALT_traverse, MOVa_dump}, 
	{"iADD", OPSIZE_iADD, 0, HALT_traverse, iADD_dump}, 
	{"iADDn", OPSIZE_iADDn, 0, HALT_traverse, iADDn_dump}, 
	{"iSUB", OPSIZE_iSUB, 0, HALT_traverse, iADD_dump}, 
	{"iSUBn", OPSIZE_iSUBn, 0, HALT_traverse, iADDn_dump}, 
	{"iMUL", OPSIZE_iMUL, 0, HALT_traverse, iADD_dump}, 
	{"iMULn", OPSIZE_iMULn, 0, HALT_traverse, iADDn_dump}, 
	{"iDIV", OPSIZE_iDIV, 0, HALT_traverse, iADD_dump}, 
	{"iDIVn", OPSIZE_iDIVn, 0, HALT_traverse, iADDn_dump}, 
	{"iMOD", OPSIZE_iMOD, 0, HALT_traverse, iADD_dump}, 
	{"iMODn", OPSIZE_iMODn, 0, HALT_traverse, iADDn_dump}, 
	{"iEQ", OPSIZE_iEQ, 0, HALT_traverse, iADD_dump}, 
	{"iEQn", OPSIZE_iEQn, 0, HALT_traverse, iADDn_dump}, 
	{"iNEQ", OPSIZE_iNEQ, 0, HALT_traverse, iADD_dump}, 
	{"iNEQn", OPSIZE_iNEQn, 0, HALT_traverse, iADDn_dump}, 
	{"iLT", OPSIZE_iLT, 0, HALT_traverse, iADD_dump}, 
	{"iLTn", OPSIZE_iLTn, 0, HALT_traverse, iADDn_dump}, 
	{"iLTE", OPSIZE_iLTE, 0, HALT_traverse, iADD_dump}, 
	{"iLTEn", OPSIZE_iLTEn, 0, HALT_traverse, iADDn_dump}, 
	{"iGT", OPSIZE_iGT, 0, HALT_traverse, iADD_dump}, 
	{"iGTn", OPSIZE_iGTn, 0, HALT_traverse, iADDn_dump}, 
	{"iGTE", OPSIZE_iGTE, 0, HALT_traverse, iADD_dump}, 
	{"iGTEn", OPSIZE_iGTEn, 0, HALT_traverse, iADDn_dump}, 
	{"fNEG", OPSIZE_fNEG, 0, HALT_traverse, MOVa_dump}, 
	{"fADD", OPSIZE_fADD, 0, HALT_traverse, iADD_dump}, 
	{"fADDn", OPSIZE_fADDn, 0, HALT_traverse, fADDn_dump}, 
	{"fSUB", OPSIZE_fSUB, 0, HALT_traverse, iADD_dump}, 
	{"fSUBn", OPSIZE_fSUBn, 0, HALT_traverse, fADDn_dump}, 
	{"fMUL", OPSIZE_fMUL, 0, HALT_traverse, iADD_dump}, 
	{"fMULn", OPSIZE_fMULn, 0, HALT_traverse, fADDn_dump}, 
	{"fDIV", OPSIZE_fDIV, 0, HALT_traverse, iADD_dump}, 
	{"fDIVn", OPSIZE_fDIVn, 0, HALT_traverse, fADDn_dump}, 
	{"fEQ", OPSIZE_fEQ, 0, HALT_traverse, iADD_dump}, 
	{"fEQn", OPSIZE_fEQn, 0, HALT_traverse, fADDn_dump}, 
	{"fNEQ", OPSIZE_fNEQ, 0, HALT_traverse, iADD_dump}, 
	{"fNEQn", OPSIZE_fNEQn, 0, HALT_traverse, fADDn_dump}, 
	{"fLT", OPSIZE_fLT, 0, HALT_traverse, iADD_dump}, 
	{"fLTn", OPSIZE_fLTn, 0, HALT_traverse, fADDn_dump}, 
	{"fLTE", OPSIZE_fLTE, 0, HALT_traverse, iADD_dump}, 
	{"fLTEn", OPSIZE_fLTEn, 0, HALT_traverse, fADDn_dump}, 
	{"fGT", OPSIZE_fGT, 0, HALT_traverse, iADD_dump}, 
	{"fGTn", OPSIZE_fGTn, 0, HALT_traverse, fADDn_dump}, 
	{"fGTE", OPSIZE_fGTE, 0, HALT_traverse, iADD_dump}, 
	{"fGTEn", OPSIZE_fGTEn, 0, HALT_traverse, fADDn_dump}, 
	{"ARYGET", OPSIZE_ARYGET, 0, HALT_traverse, iADD_dump}, 
	{"ARYGETn", OPSIZE_ARYGETn, 0, HALT_traverse, ARYGETn_dump}, 
	{"iARYGET", OPSIZE_iARYGET, 0, HALT_traverse, iADD_dump}, 
	{"iARYGETn", OPSIZE_iARYGETn, 0, HALT_traverse, ARYGETn_dump}, 
	{"fARYGET", OPSIZE_fARYGET, 0, HALT_traverse, iADD_dump}, 
	{"fARYGETn", OPSIZE_fARYGETn, 0, HALT_traverse, ARYGETn_dump}, 
	{"ARYSET", OPSIZE_ARYSET, 0, HALT_traverse, iADD_dump}, 
	{"ARYSETn", OPSIZE_ARYSETn, 0, HALT_traverse, ARYGETn_dump}, 
	{"iARYSET", OPSIZE_iARYSET, 0, HALT_traverse, iADD_dump}, 
	{"iARYSETn", OPSIZE_iARYSETn, 0, HALT_traverse, ARYGETn_dump}, 
	{"fARYSET", OPSIZE_fARYSET, 0, HALT_traverse, iADD_dump}, 
	{"fARYSETn", OPSIZE_fARYSETn, 0, HALT_traverse, ARYGETn_dump}, 
	{"THCODE", OPSIZE_THCODE, 0, HALT_traverse, CHKESP_dump}, 
	{"NOP", OPSIZE_NOP, 0, HALT_traverse, HALT_dump}, 
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


/* ======================================================================== */
/* [exec] */

#ifdef KNH_USING_THREADEDCODE
#define CASE(L, OP)   L:
#define NEXT  *(op->nextaddr)
#define JUMP  *(op->jumpaddr)
#define TC(c)   c
#else
#define OPJUMP NULL
#define CASE(L, OP)   case OP :
#define NEXT L_HEAD
#define JUMP L_HEAD
#define TC(c)    
#endif/*KNH_USING_THREADEDCODE*/

METHOD knh_KLRCode_exec(Ctx *ctx, knh_sfp_t *sfp)
{
#ifdef KNH_USING_THREADEDCODE
	static void *OPJUMP[] = {
		&&L_HALT, &&L_MOVa, &&L_MOVn, &&L_MOVo, 
		&&L_MOVx, &&L_MOVDEF, &&L_MOVSYS, &&L_XMOVs, 
		&&L_XMOVo, &&L_XMOVx, &&L_XMOVDEF, &&L_XMOVSYS, 
		&&L_MOVxi, &&L_XMOVsi, &&L_XMOVoi, &&L_XMOVxi, 
		&&L_XMOVxio, &&L_XMOVxBXi, &&L_MOVxf, &&L_XMOVsf, 
		&&L_XMOVof, &&L_XMOVxf, &&L_XMOVxfo, &&L_XMOVxBXf, 
		&&L_MOVxb, &&L_XMOVsb, &&L_XMOVob, &&L_XMOVxb, 
		&&L_EMOVs, &&L_EMOVo, &&L_EMOVe, &&L_EMOVDEF, 
		&&L_EMOVSYS, &&L_SWAP, &&L_PARAMDEF, &&L_PARAMo, 
		&&L_PARAMPROP, &&L_PARAMS, &&L_CHKESP, &&L_RET, 
		&&L_RETn, &&L_RETa, &&L_RETo, &&L_RETx, 
		&&L_YEILDBREAK, &&L_BOX, &&L_BOXnc, &&L_NNBOX, 
		&&L_NNBOXnc, &&L_UNBOX, &&L_CHKNUL, &&L_CHKNULx, 
		&&L_CHKTYPE, &&L_NCALL, &&L_FCALL, &&L_SCALL, 
		&&L_AINVOKE, &&L_CALL, &&L_ACALL, &&L_NEW, 
		&&L_COPYSFP, &&L_TOSTR, &&L_TOSTRf, &&L_SMAP, 
		&&L_SMAPnc, &&L_MAP, &&L_MAPnc, &&L_AMAP, 
		&&L_NNMAP, &&L_JMP, &&L_SKIP, &&L_bJIFT, 
		&&L_bJIFF, &&L_bJIFF_LOOP, &&L_JIFNUL, &&L_JIFNN, 
		&&L_NEXT, &&L_INEXT, &&L_TRY, &&L_TRYEND, 
		&&L_CATCH, &&L_PUSH, &&L_POP, &&L_THROW, 
		&&L_THROWs, &&L_THROW_AGAIN, &&L_P, &&L_PMSG, 
		&&L_iCAST, &&L_inCAST, &&L_fCAST, &&L_fnCAST, 
		&&L_bNOT, &&L_iNEG, &&L_iADD, &&L_iADDn, 
		&&L_iSUB, &&L_iSUBn, &&L_iMUL, &&L_iMULn, 
		&&L_iDIV, &&L_iDIVn, &&L_iMOD, &&L_iMODn, 
		&&L_iEQ, &&L_iEQn, &&L_iNEQ, &&L_iNEQn, 
		&&L_iLT, &&L_iLTn, &&L_iLTE, &&L_iLTEn, 
		&&L_iGT, &&L_iGTn, &&L_iGTE, &&L_iGTEn, 
		&&L_fNEG, &&L_fADD, &&L_fADDn, &&L_fSUB, 
		&&L_fSUBn, &&L_fMUL, &&L_fMULn, &&L_fDIV, 
		&&L_fDIVn, &&L_fEQ, &&L_fEQn, &&L_fNEQ, 
		&&L_fNEQn, &&L_fLT, &&L_fLTn, &&L_fLTE, 
		&&L_fLTEn, &&L_fGT, &&L_fGTn, &&L_fGTE, 
		&&L_fGTEn, &&L_ARYGET, &&L_ARYGETn, &&L_iARYGET, 
		&&L_iARYGETn, &&L_fARYGET, &&L_fARYGETn, &&L_ARYSET, 
		&&L_ARYSETn, &&L_iARYSET, &&L_iARYSETn, &&L_fARYSET, 
		&&L_fARYSETn, &&L_THCODE, &&L_NOP, 
	};
	register knh_code_t *pc = (sfp[-1].mtd)->pc_start;
	goto *OPJUMP[KNH_OPCODE(pc)]; /* this is needed to init */
#else
	register knh_code_t *pc = (sfp[-1].mtd)->pc_start;
	L_HEAD:;
	switch(KNH_OPCODE(pc)) {
#endif

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
	CASE(L_MOVxi, OPCODE_MOVxi) {
		const klr_MOVxi_t *op = (klr_MOVxi_t*)pc;
		KLR_MOVxi(ctx, op->a1, op->a2);
		pc += OPSIZE_MOVxi;
		goto NEXT;
	} 
	CASE(L_XMOVsi, OPCODE_XMOVsi) {
		const klr_XMOVsi_t *op = (klr_XMOVsi_t*)pc;
		KLR_XMOVsi(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVsi;
		goto NEXT;
	} 
	CASE(L_XMOVoi, OPCODE_XMOVoi) {
		const klr_XMOVoi_t *op = (klr_XMOVoi_t*)pc;
		KLR_XMOVoi(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVoi;
		goto NEXT;
	} 
	CASE(L_XMOVxi, OPCODE_XMOVxi) {
		const klr_XMOVxi_t *op = (klr_XMOVxi_t*)pc;
		KLR_XMOVxi(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVxi;
		goto NEXT;
	} 
	CASE(L_XMOVxio, OPCODE_XMOVxio) {
		const klr_XMOVxio_t *op = (klr_XMOVxio_t*)pc;
		KLR_XMOVxio(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVxio;
		goto NEXT;
	} 
	CASE(L_XMOVxBXi, OPCODE_XMOVxBXi) {
		const klr_XMOVxBXi_t *op = (klr_XMOVxBXi_t*)pc;
		KLR_XMOVxBXi(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_XMOVxBXi;
		goto NEXT;
	} 
	CASE(L_MOVxf, OPCODE_MOVxf) {
		const klr_MOVxf_t *op = (klr_MOVxf_t*)pc;
		KLR_MOVxf(ctx, op->a1, op->a2);
		pc += OPSIZE_MOVxf;
		goto NEXT;
	} 
	CASE(L_XMOVsf, OPCODE_XMOVsf) {
		const klr_XMOVsf_t *op = (klr_XMOVsf_t*)pc;
		KLR_XMOVsf(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVsf;
		goto NEXT;
	} 
	CASE(L_XMOVof, OPCODE_XMOVof) {
		const klr_XMOVof_t *op = (klr_XMOVof_t*)pc;
		KLR_XMOVof(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVof;
		goto NEXT;
	} 
	CASE(L_XMOVxf, OPCODE_XMOVxf) {
		const klr_XMOVxf_t *op = (klr_XMOVxf_t*)pc;
		KLR_XMOVxf(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVxf;
		goto NEXT;
	} 
	CASE(L_XMOVxfo, OPCODE_XMOVxfo) {
		const klr_XMOVxfo_t *op = (klr_XMOVxfo_t*)pc;
		KLR_XMOVxfo(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVxfo;
		goto NEXT;
	} 
	CASE(L_XMOVxBXf, OPCODE_XMOVxBXf) {
		const klr_XMOVxBXf_t *op = (klr_XMOVxBXf_t*)pc;
		KLR_XMOVxBXf(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_XMOVxBXf;
		goto NEXT;
	} 
	CASE(L_MOVxb, OPCODE_MOVxb) {
		const klr_MOVxb_t *op = (klr_MOVxb_t*)pc;
		KLR_MOVxb(ctx, op->a1, op->a2);
		pc += OPSIZE_MOVxb;
		goto NEXT;
	} 
	CASE(L_XMOVsb, OPCODE_XMOVsb) {
		const klr_XMOVsb_t *op = (klr_XMOVsb_t*)pc;
		KLR_XMOVsb(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVsb;
		goto NEXT;
	} 
	CASE(L_XMOVob, OPCODE_XMOVob) {
		const klr_XMOVob_t *op = (klr_XMOVob_t*)pc;
		KLR_XMOVob(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVob;
		goto NEXT;
	} 
	CASE(L_XMOVxb, OPCODE_XMOVxb) {
		const klr_XMOVxb_t *op = (klr_XMOVxb_t*)pc;
		KLR_XMOVxb(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVxb;
		goto NEXT;
	} 
	CASE(L_EMOVs, OPCODE_EMOVs) {
		const klr_EMOVs_t *op = (klr_EMOVs_t*)pc;
		KLR_EMOVs(ctx, op->a1, op->a2);
		pc += OPSIZE_EMOVs;
		goto NEXT;
	} 
	CASE(L_EMOVo, OPCODE_EMOVo) {
		const klr_EMOVo_t *op = (klr_EMOVo_t*)pc;
		KLR_EMOVo(ctx, op->a1, op->a2);
		pc += OPSIZE_EMOVo;
		goto NEXT;
	} 
	CASE(L_EMOVe, OPCODE_EMOVe) {
		const klr_EMOVe_t *op = (klr_EMOVe_t*)pc;
		KLR_EMOVe(ctx, op->a1, op->a2);
		pc += OPSIZE_EMOVe;
		goto NEXT;
	} 
	CASE(L_EMOVDEF, OPCODE_EMOVDEF) {
		const klr_EMOVDEF_t *op = (klr_EMOVDEF_t*)pc;
		KLR_EMOVDEF(ctx, op->a1, op->a2);
		pc += OPSIZE_EMOVDEF;
		goto NEXT;
	} 
	CASE(L_EMOVSYS, OPCODE_EMOVSYS) {
		const klr_EMOVSYS_t *op = (klr_EMOVSYS_t*)pc;
		KLR_EMOVSYS(ctx, op->a1, op->a2);
		pc += OPSIZE_EMOVSYS;
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
	CASE(L_RETn, OPCODE_RETn) {
		const klr_RETn_t *op = (klr_RETn_t*)pc;
		KLR_RETn(ctx, op->a1, op->a2);
		pc += OPSIZE_RETn;
		goto NEXT;
	} 
	CASE(L_RETa, OPCODE_RETa) {
		const klr_RETa_t *op = (klr_RETa_t*)pc;
		KLR_RETa(ctx, op->a1, op->a2);
		pc += OPSIZE_RETa;
		goto NEXT;
	} 
	CASE(L_RETo, OPCODE_RETo) {
		const klr_RETo_t *op = (klr_RETo_t*)pc;
		KLR_RETo(ctx, op->a1, op->a2);
		pc += OPSIZE_RETo;
		goto NEXT;
	} 
	CASE(L_RETx, OPCODE_RETx) {
		const klr_RETx_t *op = (klr_RETx_t*)pc;
		KLR_RETx(ctx, op->a1, op->a2);
		pc += OPSIZE_RETx;
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
	CASE(L_NCALL, OPCODE_NCALL) {
		TC(const klr_NCALL_t *op = (klr_NCALL_t*)pc;)
		KLR_NCALL(ctx);
		pc += OPSIZE_NCALL;
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
	CASE(L_TOSTR, OPCODE_TOSTR) {
		const klr_TOSTR_t *op = (klr_TOSTR_t*)pc;
		KLR_TOSTR(ctx, op->a1, op->a2);
		pc += OPSIZE_TOSTR;
		goto NEXT;
	} 
	CASE(L_TOSTRf, OPCODE_TOSTRf) {
		const klr_TOSTRf_t *op = (klr_TOSTRf_t*)pc;
		KLR_TOSTRf(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_TOSTRf;
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
		KLR_JMP(ctx, pc = ((knh_inst_t*)op)->jumppc, JUMP);
		pc += OPSIZE_JMP;
		goto NEXT;
	} 
	CASE(L_SKIP, OPCODE_SKIP) {
		const klr_SKIP_t *op = (klr_SKIP_t*)pc;
		KLR_SKIP(ctx, pc = ((knh_inst_t*)op)->jumppc, JUMP);
		pc += OPSIZE_SKIP;
		goto NEXT;
	} 
	CASE(L_bJIFT, OPCODE_bJIFT) {
		const klr_bJIFT_t *op = (klr_bJIFT_t*)pc;
		KLR_bJIFT(ctx, pc = ((knh_inst_t*)op)->jumppc, JUMP, op->a2);
		pc += OPSIZE_bJIFT;
		goto NEXT;
	} 
	CASE(L_bJIFF, OPCODE_bJIFF) {
		const klr_bJIFF_t *op = (klr_bJIFF_t*)pc;
		KLR_bJIFF(ctx, pc = ((knh_inst_t*)op)->jumppc, JUMP, op->a2);
		pc += OPSIZE_bJIFF;
		goto NEXT;
	} 
	CASE(L_bJIFF_LOOP, OPCODE_bJIFF_LOOP) {
		const klr_bJIFF_LOOP_t *op = (klr_bJIFF_LOOP_t*)pc;
		KLR_bJIFF_LOOP(ctx, pc = ((knh_inst_t*)op)->jumppc, JUMP, op->a2);
		pc += OPSIZE_bJIFF_LOOP;
		goto NEXT;
	} 
	CASE(L_JIFNUL, OPCODE_JIFNUL) {
		const klr_JIFNUL_t *op = (klr_JIFNUL_t*)pc;
		KLR_JIFNUL(ctx, pc = ((knh_inst_t*)op)->jumppc, JUMP, op->a2);
		pc += OPSIZE_JIFNUL;
		goto NEXT;
	} 
	CASE(L_JIFNN, OPCODE_JIFNN) {
		const klr_JIFNN_t *op = (klr_JIFNN_t*)pc;
		KLR_JIFNN(ctx, pc = ((knh_inst_t*)op)->jumppc, JUMP, op->a2);
		pc += OPSIZE_JIFNN;
		goto NEXT;
	} 
	CASE(L_NEXT, OPCODE_NEXT) {
		const klr_NEXT_t *op = (klr_NEXT_t*)pc;
		KLR_NEXT(ctx, pc = ((knh_inst_t*)op)->jumppc, JUMP, op->a2, op->a3);
		pc += OPSIZE_NEXT;
		goto NEXT;
	} 
	CASE(L_INEXT, OPCODE_INEXT) {
		const klr_INEXT_t *op = (klr_INEXT_t*)pc;
		KLR_INEXT(ctx, pc = ((knh_inst_t*)op)->jumppc, JUMP, op->a2, op->a3, op->a4);
		pc += OPSIZE_INEXT;
		goto NEXT;
	} 
	CASE(L_TRY, OPCODE_TRY) {
		const klr_TRY_t *op = (klr_TRY_t*)pc;
		KLR_TRY(ctx, pc = ((knh_inst_t*)op)->jumppc, JUMP, op->a2);
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
		KLR_CATCH(ctx, pc = ((knh_inst_t*)op)->jumppc, JUMP, op->a2, op->a3, op->a4);
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
		KLR_THROWs(ctx, op->a1, op->a2);
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
#ifndef KNH_USING_THREADEDCODE
	}
	KNH_WARNING(ctx, "unknown opcode=%d", KNH_OPCODE(pc));
	goto L_HEAD;
#endif
}

#ifdef __cplusplus
}
#endif
