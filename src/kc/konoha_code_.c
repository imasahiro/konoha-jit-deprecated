/****************************************************************************
 * KONOHA COPYRIGHT, LICENSE NOTICE, AND DISCRIMER  
 * 
 * Copyright (c) 2006-2010, Kimio Kuramitsu <kimio at ynu.ac.jp>
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
static void EXIT_traverse(Ctx *ctx, knh_inst_t *c, knh_Ftraverse ftr)
{
}
static void EXIT_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
}

static void OSET_traverse(Ctx *ctx, knh_inst_t *c, knh_Ftraverse ftr)
{
	klr_OSET_t *op = (klr_OSET_t*)c; 
	DBG2_ASSERT(op->opcode == 11);
	ftr(ctx, UP(op->a2));
}

static void XOSET_traverse(Ctx *ctx, knh_inst_t *c, knh_Ftraverse ftr)
{
	klr_XOSET_t *op = (klr_XOSET_t*)c; 
	DBG2_ASSERT(op->opcode == 22);
	ftr(ctx, UP(op->a2));
}

static void SCALL_traverse(Ctx *ctx, knh_inst_t *c, knh_Ftraverse ftr)
{
	klr_SCALL_t *op = (klr_SCALL_t*)c; 
	DBG2_ASSERT(op->opcode == 34);
	ftr(ctx, UP(op->a3));
}

static void VCALL_traverse(Ctx *ctx, knh_inst_t *c, knh_Ftraverse ftr)
{
	klr_VCALL_t *op = (klr_VCALL_t*)c; 
	DBG2_ASSERT(op->opcode == 35);
	ftr(ctx, UP(op->a3));
}

static void SMAP_traverse(Ctx *ctx, knh_inst_t *c, knh_Ftraverse ftr)
{
	klr_SMAP_t *op = (klr_SMAP_t*)c; 
	DBG2_ASSERT(op->opcode == 38);
	ftr(ctx, UP(op->a2));
}

static void SMAPnc_traverse(Ctx *ctx, knh_inst_t *c, knh_Ftraverse ftr)
{
	klr_SMAPnc_t *op = (klr_SMAPnc_t*)c; 
	DBG2_ASSERT(op->opcode == 39);
	ftr(ctx, UP(op->a2));
}

static void CATCH_traverse(Ctx *ctx, knh_inst_t *c, knh_Ftraverse ftr)
{
	klr_CATCH_t *op = (klr_CATCH_t*)c; 
	DBG2_ASSERT(op->opcode == 58);
	ftr(ctx, UP(op->a4));
}

static void THROWs_traverse(Ctx *ctx, knh_inst_t *c, knh_Ftraverse ftr)
{
	klr_THROWs_t *op = (klr_THROWs_t*)c; 
	DBG2_ASSERT(op->opcode == 60);
	ftr(ctx, UP(op->a3));
}

static void PMSG_traverse(Ctx *ctx, knh_inst_t *c, knh_Ftraverse ftr)
{
	klr_PMSG_t *op = (klr_PMSG_t*)c; 
	DBG2_ASSERT(op->opcode == 63);
	ftr(ctx, UP(op->a2));
}

static void LABEL_traverse(Ctx *ctx, knh_inst_t *c, knh_Ftraverse ftr)
{
	klr_LABEL_t *op = (klr_LABEL_t*)c; 
	DBG2_ASSERT(op->opcode == 115);
	ftr(ctx, UP(op->a2));
}

static void HALT_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_HALT_t *op = (klr_HALT_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
}

static void YEILD_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_YEILD_t *op = (klr_YEILD_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfpidx(ctx, w, (op->a1));
}

static void TR_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_TR_t *op = (klr_TR_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__func(ctx, w, (op->a1));
	knh_write__sfpidx(ctx, w, (op->a2));
	knh_write__cid(ctx, w, (op->a3));
}

static void OSET_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_OSET_t *op = (klr_OSET_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfpidx(ctx, w, (op->a1));
	knh_write__OBJ(ctx, w, UP((op->a2)));
}

static void NSET_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_NSET_t *op = (klr_NSET_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfpidx(ctx, w, (op->a1));
	knh_write__int(ctx, w, (op->a2));
}

static void MOV_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_MOV_t *op = (klr_MOV_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfpidx(ctx, w, (op->a1));
	knh_write__sfpidx(ctx, w, (op->a2));
}

static void OMOVx_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_OMOVx_t *op = (klr_OMOVx_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfpidx(ctx, w, (op->a1));
	knh_write__sfx(ctx, w, (op->a2));
}

static void XMOV_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_XMOV_t *op = (klr_XMOV_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfx(ctx, w, (op->a1));
	knh_write__sfpidx(ctx, w, (op->a2));
}

static void XMOVx_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_XMOVx_t *op = (klr_XMOVx_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfx(ctx, w, (op->a1));
	knh_write__sfx(ctx, w, (op->a2));
}

static void XOSET_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_XOSET_t *op = (klr_XOSET_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfx(ctx, w, (op->a1));
	knh_write__OBJ(ctx, w, UP((op->a2)));
}

static void MOVe_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_MOVe_t *op = (klr_MOVe_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfpidx(ctx, w, (op->a1));
	knh_write__ushort(ctx, w, (op->a2));
}

static void CHKTYPE_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_CHKTYPE_t *op = (klr_CHKTYPE_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfpidx(ctx, w, (op->a1));
	knh_write__cid(ctx, w, (op->a2));
}

static void CHKNULx_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_CHKNULx_t *op = (klr_CHKNULx_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfx(ctx, w, (op->a1));
}

static void CHKTYPEx_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_CHKTYPEx_t *op = (klr_CHKTYPEx_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfx(ctx, w, (op->a1));
	knh_write__cid(ctx, w, (op->a2));
}

static void LDMETHOD_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_LDMETHOD_t *op = (klr_LDMETHOD_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__func(ctx, w, (op->a1));
	knh_write__sfpidx(ctx, w, (op->a2));
	knh_write__mn(ctx, w, (op->a3));
}

static void SCALL_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_SCALL_t *op = (klr_SCALL_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__sfpidx(ctx, w, (op->a1));
	knh_write__ushort(ctx, w, (op->a2));
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

static void JMPT_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_JMPT_t *op = (klr_JMPT_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__addr(ctx, w, (op->a1));
	knh_write__sfpidx(ctx, w, (op->a2));
}

static void JMPchk_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_JMPchk_t *op = (klr_JMPchk_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__addr(ctx, w, (op->a1));
	knh_write__func(ctx, w, (op->a2));
	knh_write__sfpidx(ctx, w, (op->a3));
}

static void JMPcmp_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_JMPcmp_t *op = (klr_JMPcmp_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__addr(ctx, w, (op->a1));
	knh_write__func(ctx, w, (op->a2));
	knh_write__sfpidx(ctx, w, (op->a3));
	knh_write__sfpidx(ctx, w, (op->a4));
}

static void JMPcmpi_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_JMPcmpi_t *op = (klr_JMPcmpi_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__addr(ctx, w, (op->a1));
	knh_write__func(ctx, w, (op->a2));
	knh_write__sfpidx(ctx, w, (op->a3));
	knh_write__int(ctx, w, (op->a4));
}

static void JMPcmpf_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_JMPcmpf_t *op = (klr_JMPcmpf_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__addr(ctx, w, (op->a1));
	knh_write__func(ctx, w, (op->a2));
	knh_write__sfpidx(ctx, w, (op->a3));
	knh_write__float(ctx, w, (op->a4));
}

static void NEXT_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_NEXT_t *op = (klr_NEXT_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__addr(ctx, w, (op->a1));
	knh_write__sfpidx(ctx, w, (op->a2));
	knh_write__sfpidx(ctx, w, (op->a3));
}

static void NEXTf_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_NEXTf_t *op = (klr_NEXTf_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__addr(ctx, w, (op->a1));
	knh_write__func(ctx, w, (op->a2));
	knh_write__cid(ctx, w, (op->a3));
	knh_write__sfpidx(ctx, w, (op->a4));
	knh_write__sfpidx(ctx, w, (op->a5));
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

static void iDIVn_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_iDIVn_t *op = (klr_iDIVn_t*)c; 
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

static void OEVAL_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_OEVAL_t *op = (klr_OEVAL_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__func(ctx, w, (op->a1));
	knh_write__sfpidx(ctx, w, (op->a2));
	knh_write__sfpidx(ctx, w, (op->a3));
}

static void GETIDX_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_GETIDX_t *op = (klr_GETIDX_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__func(ctx, w, (op->a1));
	knh_write__sfpidx(ctx, w, (op->a2));
	knh_write__sfpidx(ctx, w, (op->a3));
	knh_write__sfpidx(ctx, w, (op->a4));
}

static void SETIDX_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_SETIDX_t *op = (klr_SETIDX_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__func(ctx, w, (op->a1));
	knh_write__sfpidx(ctx, w, (op->a2));
	knh_write__sfpidx(ctx, w, (op->a3));
	knh_write__sfpidx(ctx, w, (op->a4));
	knh_write__sfpidx(ctx, w, (op->a5));
}

static void GETIDXn_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_GETIDXn_t *op = (klr_GETIDXn_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__func(ctx, w, (op->a1));
	knh_write__sfpidx(ctx, w, (op->a2));
	knh_write__sfpidx(ctx, w, (op->a3));
	knh_write__intptr(ctx, w, (op->a4));
}

static void SETIDXn_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_SETIDXn_t *op = (klr_SETIDXn_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__func(ctx, w, (op->a1));
	knh_write__sfpidx(ctx, w, (op->a2));
	knh_write__sfpidx(ctx, w, (op->a3));
	knh_write__sfpidx(ctx, w, (op->a4));
	knh_write__intptr(ctx, w, (op->a5));
}

static void LABEL_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_LABEL_t *op = (klr_LABEL_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__intptr(ctx, w, (op->a1));
	knh_write__OBJ(ctx, w, UP((op->a2)));
}

static void PROBE_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	klr_PROBE_t *op = (klr_PROBE_t*)c; 
	knh_write_opcode(ctx, w, op->opcode);
	knh_write__func(ctx, w, (op->a1));
	knh_write__ushort(ctx, w, (op->a2));
}

static void YEILD_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_YEILD_t *op = (klr_YEILD_t*)c; 
	op->a1 += shift;
}

static void TR_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_TR_t *op = (klr_TR_t*)c; 
	op->a2 += shift;
}

static void OSET_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_OSET_t *op = (klr_OSET_t*)c; 
	op->a1 += shift;
}

static void NSET_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_NSET_t *op = (klr_NSET_t*)c; 
	op->a1 += shift;
}

static void MOV_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_MOV_t *op = (klr_MOV_t*)c; 
	op->a1 += shift;
	op->a2 += shift;
}

static void OMOVx_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_OMOVx_t *op = (klr_OMOVx_t*)c; 
	op->a1 += shift;
	op->a2.i += shift;
}

static void XMOV_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_XMOV_t *op = (klr_XMOV_t*)c; 
	op->a1.i += shift;
	op->a2 += shift;
}

static void XMOVx_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_XMOVx_t *op = (klr_XMOVx_t*)c; 
	op->a1.i += shift;
	op->a2.i += shift;
}

static void XOSET_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_XOSET_t *op = (klr_XOSET_t*)c; 
	op->a1.i += shift;
}

static void MOVe_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_MOVe_t *op = (klr_MOVe_t*)c; 
	op->a1 += shift;
}

static void CHKTYPE_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_CHKTYPE_t *op = (klr_CHKTYPE_t*)c; 
	op->a1 += shift;
}

static void CHKNULx_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_CHKNULx_t *op = (klr_CHKNULx_t*)c; 
	op->a1.i += shift;
}

static void CHKTYPEx_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_CHKTYPEx_t *op = (klr_CHKTYPEx_t*)c; 
	op->a1.i += shift;
}

static void LDMETHOD_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_LDMETHOD_t *op = (klr_LDMETHOD_t*)c; 
	op->a2 += shift;
}

static void SCALL_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_SCALL_t *op = (klr_SCALL_t*)c; 
	op->a1 += shift;
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

static void JMPT_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_JMPT_t *op = (klr_JMPT_t*)c; 
	knh_code_t *newpc = ((knh_code_t*)op->a1) + pcshift;
	op->a1 = (knh_KLRInst_t*)newpc;
	THREADEDCODE(op->codeaddr = NULL;)
	THREADEDCODE(op->jumpaddr = NULL;)
	op->a2 += shift;
}

static void JMPchk_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_JMPchk_t *op = (klr_JMPchk_t*)c; 
	knh_code_t *newpc = ((knh_code_t*)op->a1) + pcshift;
	op->a1 = (knh_KLRInst_t*)newpc;
	THREADEDCODE(op->codeaddr = NULL;)
	THREADEDCODE(op->jumpaddr = NULL;)
	op->a3 += shift;
}

static void JMPcmp_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_JMPcmp_t *op = (klr_JMPcmp_t*)c; 
	knh_code_t *newpc = ((knh_code_t*)op->a1) + pcshift;
	op->a1 = (knh_KLRInst_t*)newpc;
	THREADEDCODE(op->codeaddr = NULL;)
	THREADEDCODE(op->jumpaddr = NULL;)
	op->a3 += shift;
	op->a4 += shift;
}

static void JMPcmpi_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_JMPcmpi_t *op = (klr_JMPcmpi_t*)c; 
	knh_code_t *newpc = ((knh_code_t*)op->a1) + pcshift;
	op->a1 = (knh_KLRInst_t*)newpc;
	THREADEDCODE(op->codeaddr = NULL;)
	THREADEDCODE(op->jumpaddr = NULL;)
	op->a3 += shift;
}

static void JMPcmpf_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_JMPcmpf_t *op = (klr_JMPcmpf_t*)c; 
	knh_code_t *newpc = ((knh_code_t*)op->a1) + pcshift;
	op->a1 = (knh_KLRInst_t*)newpc;
	THREADEDCODE(op->codeaddr = NULL;)
	THREADEDCODE(op->jumpaddr = NULL;)
	op->a3 += shift;
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

static void NEXTf_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_NEXTf_t *op = (klr_NEXTf_t*)c; 
	knh_code_t *newpc = ((knh_code_t*)op->a1) + pcshift;
	op->a1 = (knh_KLRInst_t*)newpc;
	THREADEDCODE(op->codeaddr = NULL;)
	THREADEDCODE(op->jumpaddr = NULL;)
	op->a4 += shift;
	op->a5 += shift;
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

static void iDIVn_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_iDIVn_t *op = (klr_iDIVn_t*)c; 
	op->a1 += shift;
	op->a2 += shift;
}

static void fADDn_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_fADDn_t *op = (klr_fADDn_t*)c; 
	op->a1 += shift;
	op->a2 += shift;
}

static void OEVAL_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_OEVAL_t *op = (klr_OEVAL_t*)c; 
	op->a2 += shift;
	op->a3 += shift;
}

static void GETIDX_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_GETIDX_t *op = (klr_GETIDX_t*)c; 
	op->a2 += shift;
	op->a3 += shift;
	op->a4 += shift;
}

static void SETIDX_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_SETIDX_t *op = (klr_SETIDX_t*)c; 
	op->a2 += shift;
	op->a3 += shift;
	op->a4 += shift;
	op->a5 += shift;
}

static void GETIDXn_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_GETIDXn_t *op = (klr_GETIDXn_t*)c; 
	op->a2 += shift;
	op->a3 += shift;
}

static void SETIDXn_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	klr_SETIDXn_t *op = (klr_SETIDXn_t*)c; 
	op->a2 += shift;
	op->a3 += shift;
	op->a4 += shift;
}

/* ======================================================================== */
/* [data] */

typedef void (*codeftr)(Ctx *, knh_inst_t*, knh_Ftraverse);
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
	{"HALT", OPSIZE_HALT, 0, EXIT_traverse, HALT_dump, EXIT_shift}, 
	{"ENTER", OPSIZE_ENTER, 0, EXIT_traverse, HALT_dump, EXIT_shift}, 
	{"EXEC", OPSIZE_EXEC, 0, EXIT_traverse, HALT_dump, EXIT_shift}, 
	{"YEILD", OPSIZE_YEILD, 0, EXIT_traverse, YEILD_dump, YEILD_shift}, 
	{"EXIT", OPSIZE_EXIT, 0, EXIT_traverse, HALT_dump, EXIT_shift}, 
	{"TR", OPSIZE_TR, 0, EXIT_traverse, TR_dump, TR_shift}, 
	{"OTR", OPSIZE_OTR, 0, EXIT_traverse, TR_dump, TR_shift}, 
	{"NULTR", OPSIZE_NULTR, 0, EXIT_traverse, TR_dump, TR_shift}, 
	{"UNBOX", OPSIZE_UNBOX, 0, EXIT_traverse, YEILD_dump, YEILD_shift}, 
	{"iCAST", OPSIZE_iCAST, 0, EXIT_traverse, YEILD_dump, YEILD_shift}, 
	{"fCAST", OPSIZE_fCAST, 0, EXIT_traverse, YEILD_dump, YEILD_shift}, 
	{"OSET", OPSIZE_OSET, 0, OSET_traverse, OSET_dump, OSET_shift}, 
	{"NSET", OPSIZE_NSET, 0, EXIT_traverse, NSET_dump, NSET_shift}, 
	{"MOV", OPSIZE_MOV, 0, EXIT_traverse, MOV_dump, MOV_shift}, 
	{"OMOV", OPSIZE_OMOV, 0, EXIT_traverse, MOV_dump, MOV_shift}, 
	{"NMOV", OPSIZE_NMOV, 0, EXIT_traverse, MOV_dump, MOV_shift}, 
	{"OMOVx", OPSIZE_OMOVx, 0, EXIT_traverse, OMOVx_dump, OMOVx_shift}, 
	{"iMOVx", OPSIZE_iMOVx, 0, EXIT_traverse, OMOVx_dump, OMOVx_shift}, 
	{"fMOVx", OPSIZE_fMOVx, 0, EXIT_traverse, OMOVx_dump, OMOVx_shift}, 
	{"bMOVx", OPSIZE_bMOVx, 0, EXIT_traverse, OMOVx_dump, OMOVx_shift}, 
	{"XMOV", OPSIZE_XMOV, 0, EXIT_traverse, XMOV_dump, XMOV_shift}, 
	{"XMOVx", OPSIZE_XMOVx, 0, EXIT_traverse, XMOVx_dump, XMOVx_shift}, 
	{"XOSET", OPSIZE_XOSET, 0, XOSET_traverse, XOSET_dump, XOSET_shift}, 
	{"XIMOV", OPSIZE_XIMOV, 0, EXIT_traverse, XMOV_dump, XMOV_shift}, 
	{"XFMOV", OPSIZE_XFMOV, 0, EXIT_traverse, XMOV_dump, XMOV_shift}, 
	{"XBMOV", OPSIZE_XBMOV, 0, EXIT_traverse, XMOV_dump, XMOV_shift}, 
	{"MOVe", OPSIZE_MOVe, 0, EXIT_traverse, MOVe_dump, MOVe_shift}, 
	{"SWAP", OPSIZE_SWAP, 0, EXIT_traverse, MOV_dump, MOV_shift}, 
	{"CHKNUL", OPSIZE_CHKNUL, 0, EXIT_traverse, YEILD_dump, YEILD_shift}, 
	{"CHKTYPE", OPSIZE_CHKTYPE, 0, EXIT_traverse, CHKTYPE_dump, CHKTYPE_shift}, 
	{"CHKNULx", OPSIZE_CHKNULx, 0, EXIT_traverse, CHKNULx_dump, CHKNULx_shift}, 
	{"CHKTYPEx", OPSIZE_CHKTYPEx, 0, EXIT_traverse, CHKTYPEx_dump, CHKTYPEx_shift}, 
	{"LDMETHOD", OPSIZE_LDMETHOD, 0, EXIT_traverse, LDMETHOD_dump, LDMETHOD_shift}, 
	{"CALL", OPSIZE_CALL, 0, EXIT_traverse, MOVe_dump, MOVe_shift}, 
	{"SCALL", OPSIZE_SCALL, 0, SCALL_traverse, SCALL_dump, SCALL_shift}, 
	{"VCALL", OPSIZE_VCALL, 0, VCALL_traverse, SCALL_dump, SCALL_shift}, 
	{"RET", OPSIZE_RET, 0, EXIT_traverse, HALT_dump, EXIT_shift}, 
	{"FUNCCALL", OPSIZE_FUNCCALL, 0, EXIT_traverse, HALT_dump, EXIT_shift}, 
	{"SMAP", OPSIZE_SMAP, 0, SMAP_traverse, SMAP_dump, SMAP_shift}, 
	{"SMAPnc", OPSIZE_SMAPnc, 0, SMAPnc_traverse, SMAP_dump, SMAP_shift}, 
	{"MAP", OPSIZE_MAP, 0, EXIT_traverse, CHKTYPE_dump, CHKTYPE_shift}, 
	{"MAPnc", OPSIZE_MAPnc, 0, EXIT_traverse, CHKTYPE_dump, CHKTYPE_shift}, 
	{"AMAP", OPSIZE_AMAP, 0, EXIT_traverse, CHKTYPE_dump, CHKTYPE_shift}, 
	{"JMP", OPSIZE_JMP, 1, EXIT_traverse, JMP_dump, JMP_shift}, 
	{"NOPJMP", OPSIZE_NOPJMP, 1, EXIT_traverse, JMP_dump, JMP_shift}, 
	{"JMPT", OPSIZE_JMPT, 1, EXIT_traverse, JMPT_dump, JMPT_shift}, 
	{"JMPF", OPSIZE_JMPF, 1, EXIT_traverse, JMPT_dump, JMPT_shift}, 
	{"JMPF_LOOP", OPSIZE_JMPF_LOOP, 1, EXIT_traverse, JMPT_dump, JMPT_shift}, 
	{"JMPNUL", OPSIZE_JMPNUL, 1, EXIT_traverse, JMPT_dump, JMPT_shift}, 
	{"JMPNN", OPSIZE_JMPNN, 1, EXIT_traverse, JMPT_dump, JMPT_shift}, 
	{"JMPchk", OPSIZE_JMPchk, 1, EXIT_traverse, JMPchk_dump, JMPchk_shift}, 
	{"JMPcmp", OPSIZE_JMPcmp, 1, EXIT_traverse, JMPcmp_dump, JMPcmp_shift}, 
	{"JMPcmpi", OPSIZE_JMPcmpi, 1, EXIT_traverse, JMPcmpi_dump, JMPcmpi_shift}, 
	{"JMPcmpf", OPSIZE_JMPcmpf, 1, EXIT_traverse, JMPcmpf_dump, JMPcmpf_shift}, 
	{"NEXT", OPSIZE_NEXT, 1, EXIT_traverse, NEXT_dump, NEXT_shift}, 
	{"NEXTf", OPSIZE_NEXTf, 1, EXIT_traverse, NEXTf_dump, NEXTf_shift}, 
	{"TRY", OPSIZE_TRY, 1, EXIT_traverse, JMPT_dump, JMPT_shift}, 
	{"TRYEND", OPSIZE_TRYEND, 0, EXIT_traverse, YEILD_dump, YEILD_shift}, 
	{"CATCH", OPSIZE_CATCH, 1, CATCH_traverse, CATCH_dump, CATCH_shift}, 
	{"THROW", OPSIZE_THROW, 0, EXIT_traverse, MOV_dump, MOV_shift}, 
	{"THROWs", OPSIZE_THROWs, 0, THROWs_traverse, THROWs_dump, THROWs_shift}, 
	{"THROW_AGAIN", OPSIZE_THROW_AGAIN, 0, EXIT_traverse, YEILD_dump, YEILD_shift}, 
	{"P", OPSIZE_P, 0, EXIT_traverse, P_dump, P_shift}, 
	{"PMSG", OPSIZE_PMSG, 0, PMSG_traverse, PMSG_dump, EXIT_shift}, 
	{"bNOT", OPSIZE_bNOT, 0, EXIT_traverse, MOV_dump, MOV_shift}, 
	{"iNEG", OPSIZE_iNEG, 0, EXIT_traverse, MOV_dump, MOV_shift}, 
	{"iADD", OPSIZE_iADD, 0, EXIT_traverse, iADD_dump, iADD_shift}, 
	{"iSUB", OPSIZE_iSUB, 0, EXIT_traverse, iADD_dump, iADD_shift}, 
	{"iMUL", OPSIZE_iMUL, 0, EXIT_traverse, iADD_dump, iADD_shift}, 
	{"iDIV", OPSIZE_iDIV, 0, EXIT_traverse, iADD_dump, iADD_shift}, 
	{"iMOD", OPSIZE_iMOD, 0, EXIT_traverse, iADD_dump, iADD_shift}, 
	{"iDIVn", OPSIZE_iDIVn, 0, EXIT_traverse, iDIVn_dump, iDIVn_shift}, 
	{"iADDn", OPSIZE_iADDn, 0, EXIT_traverse, iDIVn_dump, iDIVn_shift}, 
	{"iSUBn", OPSIZE_iSUBn, 0, EXIT_traverse, iDIVn_dump, iDIVn_shift}, 
	{"iMULn", OPSIZE_iMULn, 0, EXIT_traverse, iDIVn_dump, iDIVn_shift}, 
	{"iMODn", OPSIZE_iMODn, 0, EXIT_traverse, iDIVn_dump, iDIVn_shift}, 
	{"iEQ", OPSIZE_iEQ, 0, EXIT_traverse, iADD_dump, iADD_shift}, 
	{"iNEQ", OPSIZE_iNEQ, 0, EXIT_traverse, iADD_dump, iADD_shift}, 
	{"iLT", OPSIZE_iLT, 0, EXIT_traverse, iADD_dump, iADD_shift}, 
	{"iLTE", OPSIZE_iLTE, 0, EXIT_traverse, iADD_dump, iADD_shift}, 
	{"iGT", OPSIZE_iGT, 0, EXIT_traverse, iADD_dump, iADD_shift}, 
	{"iGTE", OPSIZE_iGTE, 0, EXIT_traverse, iADD_dump, iADD_shift}, 
	{"iEQn", OPSIZE_iEQn, 0, EXIT_traverse, iDIVn_dump, iDIVn_shift}, 
	{"iNEQn", OPSIZE_iNEQn, 0, EXIT_traverse, iDIVn_dump, iDIVn_shift}, 
	{"iLTn", OPSIZE_iLTn, 0, EXIT_traverse, iDIVn_dump, iDIVn_shift}, 
	{"iLTEn", OPSIZE_iLTEn, 0, EXIT_traverse, iDIVn_dump, iDIVn_shift}, 
	{"iGTn", OPSIZE_iGTn, 0, EXIT_traverse, iDIVn_dump, iDIVn_shift}, 
	{"iGTEn", OPSIZE_iGTEn, 0, EXIT_traverse, iDIVn_dump, iDIVn_shift}, 
	{"fNEG", OPSIZE_fNEG, 0, EXIT_traverse, MOV_dump, MOV_shift}, 
	{"fADD", OPSIZE_fADD, 0, EXIT_traverse, iADD_dump, iADD_shift}, 
	{"fSUB", OPSIZE_fSUB, 0, EXIT_traverse, iADD_dump, iADD_shift}, 
	{"fMUL", OPSIZE_fMUL, 0, EXIT_traverse, iADD_dump, iADD_shift}, 
	{"fDIV", OPSIZE_fDIV, 0, EXIT_traverse, iADD_dump, iADD_shift}, 
	{"fADDn", OPSIZE_fADDn, 0, EXIT_traverse, fADDn_dump, fADDn_shift}, 
	{"fSUBn", OPSIZE_fSUBn, 0, EXIT_traverse, fADDn_dump, fADDn_shift}, 
	{"fMULn", OPSIZE_fMULn, 0, EXIT_traverse, fADDn_dump, fADDn_shift}, 
	{"fDIVn", OPSIZE_fDIVn, 0, EXIT_traverse, fADDn_dump, fADDn_shift}, 
	{"fEQ", OPSIZE_fEQ, 0, EXIT_traverse, iADD_dump, iADD_shift}, 
	{"fNEQ", OPSIZE_fNEQ, 0, EXIT_traverse, iADD_dump, iADD_shift}, 
	{"fEQn", OPSIZE_fEQn, 0, EXIT_traverse, fADDn_dump, fADDn_shift}, 
	{"fNEQn", OPSIZE_fNEQn, 0, EXIT_traverse, fADDn_dump, fADDn_shift}, 
	{"fLT", OPSIZE_fLT, 0, EXIT_traverse, iADD_dump, iADD_shift}, 
	{"fLTE", OPSIZE_fLTE, 0, EXIT_traverse, iADD_dump, iADD_shift}, 
	{"fGT", OPSIZE_fGT, 0, EXIT_traverse, iADD_dump, iADD_shift}, 
	{"fGTE", OPSIZE_fGTE, 0, EXIT_traverse, iADD_dump, iADD_shift}, 
	{"fLTn", OPSIZE_fLTn, 0, EXIT_traverse, fADDn_dump, fADDn_shift}, 
	{"fLTEn", OPSIZE_fLTEn, 0, EXIT_traverse, fADDn_dump, fADDn_shift}, 
	{"fGTn", OPSIZE_fGTn, 0, EXIT_traverse, fADDn_dump, fADDn_shift}, 
	{"fGTEn", OPSIZE_fGTEn, 0, EXIT_traverse, fADDn_dump, fADDn_shift}, 
	{"OEVAL", OPSIZE_OEVAL, 0, EXIT_traverse, OEVAL_dump, OEVAL_shift}, 
	{"GETIDX", OPSIZE_GETIDX, 0, EXIT_traverse, GETIDX_dump, GETIDX_shift}, 
	{"SETIDX", OPSIZE_SETIDX, 0, EXIT_traverse, SETIDX_dump, SETIDX_shift}, 
	{"GETIDXn", OPSIZE_GETIDXn, 0, EXIT_traverse, GETIDXn_dump, GETIDXn_shift}, 
	{"SETIDXn", OPSIZE_SETIDXn, 0, EXIT_traverse, SETIDXn_dump, SETIDXn_shift}, 
	{"THCODE", OPSIZE_THCODE, 0, EXIT_traverse, HALT_dump, EXIT_shift}, 
	{"LABEL", OPSIZE_LABEL, 0, LABEL_traverse, LABEL_dump, EXIT_shift}, 
	{"PROBE", OPSIZE_PROBE, 0, EXIT_traverse, PROBE_dump, EXIT_shift}, 
	{"NOP", OPSIZE_NOP, 0, EXIT_traverse, HALT_dump, EXIT_shift}, 
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

void knh_opcode_traverse(Ctx *ctx, knh_inst_t *c, knh_Ftraverse ftr)
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

knh_code_t* knh_VirtualMachine_run(Ctx *ctx, knh_sfp_t *sfp, knh_code_t *pc)
{
#ifdef KNH_USING_THREADEDCODE
	static void *OPJUMP[] = {
		&&L_HALT, &&L_ENTER, &&L_EXEC, &&L_YEILD, 
		&&L_EXIT, &&L_TR, &&L_OTR, &&L_NULTR, 
		&&L_UNBOX, &&L_iCAST, &&L_fCAST, &&L_OSET, 
		&&L_NSET, &&L_MOV, &&L_OMOV, &&L_NMOV, 
		&&L_OMOVx, &&L_iMOVx, &&L_fMOVx, &&L_bMOVx, 
		&&L_XMOV, &&L_XMOVx, &&L_XOSET, &&L_XIMOV, 
		&&L_XFMOV, &&L_XBMOV, &&L_MOVe, &&L_SWAP, 
		&&L_CHKNUL, &&L_CHKTYPE, &&L_CHKNULx, &&L_CHKTYPEx, 
		&&L_LDMETHOD, &&L_CALL, &&L_SCALL, &&L_VCALL, 
		&&L_RET, &&L_FUNCCALL, &&L_SMAP, &&L_SMAPnc, 
		&&L_MAP, &&L_MAPnc, &&L_AMAP, &&L_JMP, 
		&&L_NOPJMP, &&L_JMPT, &&L_JMPF, &&L_JMPF_LOOP, 
		&&L_JMPNUL, &&L_JMPNN, &&L_JMPchk, &&L_JMPcmp, 
		&&L_JMPcmpi, &&L_JMPcmpf, &&L_NEXT, &&L_NEXTf, 
		&&L_TRY, &&L_TRYEND, &&L_CATCH, &&L_THROW, 
		&&L_THROWs, &&L_THROW_AGAIN, &&L_P, &&L_PMSG, 
		&&L_bNOT, &&L_iNEG, &&L_iADD, &&L_iSUB, 
		&&L_iMUL, &&L_iDIV, &&L_iMOD, &&L_iDIVn, 
		&&L_iADDn, &&L_iSUBn, &&L_iMULn, &&L_iMODn, 
		&&L_iEQ, &&L_iNEQ, &&L_iLT, &&L_iLTE, 
		&&L_iGT, &&L_iGTE, &&L_iEQn, &&L_iNEQn, 
		&&L_iLTn, &&L_iLTEn, &&L_iGTn, &&L_iGTEn, 
		&&L_fNEG, &&L_fADD, &&L_fSUB, &&L_fMUL, 
		&&L_fDIV, &&L_fADDn, &&L_fSUBn, &&L_fMULn, 
		&&L_fDIVn, &&L_fEQ, &&L_fNEQ, &&L_fEQn, 
		&&L_fNEQn, &&L_fLT, &&L_fLTE, &&L_fGT, 
		&&L_fGTE, &&L_fLTn, &&L_fLTEn, &&L_fGTn, 
		&&L_fGTEn, &&L_OEVAL, &&L_GETIDX, &&L_SETIDX, 
		&&L_GETIDXn, &&L_SETIDXn, &&L_THCODE, &&L_LABEL, 
		&&L_PROBE, &&L_NOP, 
	};
#endif
#if defined(OPCODE_VCALL)
	L_KONOHACALL:;
#endif
	int sfpidx = sfp - ctx->stack;
	//register knh_code_t *pc = (sfp[-1].mtd)->pc_start;
	DBG2_ASSERT(IS_Method(sfp[-1].mtd));
	DISPATCH_START(pc);

	CASE(HALT) {
		TC(const klr_HALT_t *op = (klr_HALT_t*)pc;)
		KLR0_HALT(ctx);
		pc += OPSIZE_HALT;
		goto NEXT_OP;
	} 
	CASE(ENTER) {
		TC(const klr_ENTER_t *op = (klr_ENTER_t*)pc;)
		KLR0_ENTER(ctx);
		pc += OPSIZE_ENTER;
		goto NEXT_OP;
	} 
	CASE(EXEC) {
		TC(const klr_EXEC_t *op = (klr_EXEC_t*)pc;)
		KLR0_EXEC(ctx);
		pc += OPSIZE_EXEC;
		goto NEXT_OP;
	} 
	CASE(YEILD) {
		const klr_YEILD_t *op = (klr_YEILD_t*)pc;
		KLR0_YEILD(ctx, op->a1);
		pc += OPSIZE_YEILD;
		goto NEXT_OP;
	} 
	CASE(EXIT) {
		TC(const klr_EXIT_t *op = (klr_EXIT_t*)pc;)
		KLR0_EXIT(ctx);
		pc += OPSIZE_EXIT;
		goto NEXT_OP;
	} 
	CASE(TR) {
		const klr_TR_t *op = (klr_TR_t*)pc;
		KLR_TR(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_TR;
		goto NEXT_OP;
	} 
	CASE(OTR) {
		const klr_OTR_t *op = (klr_OTR_t*)pc;
		KLR_OTR(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_OTR;
		goto NEXT_OP;
	} 
	CASE(NULTR) {
		const klr_NULTR_t *op = (klr_NULTR_t*)pc;
		KLR_NULTR(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_NULTR;
		goto NEXT_OP;
	} 
	CASE(UNBOX) {
		const klr_UNBOX_t *op = (klr_UNBOX_t*)pc;
		KLR_UNBOX(ctx, op->a1);
		pc += OPSIZE_UNBOX;
		goto NEXT_OP;
	} 
	CASE(iCAST) {
		const klr_iCAST_t *op = (klr_iCAST_t*)pc;
		KLR_iCAST(ctx, op->a1);
		pc += OPSIZE_iCAST;
		goto NEXT_OP;
	} 
	CASE(fCAST) {
		const klr_fCAST_t *op = (klr_fCAST_t*)pc;
		KLR_fCAST(ctx, op->a1);
		pc += OPSIZE_fCAST;
		goto NEXT_OP;
	} 
	CASE(OSET) {
		const klr_OSET_t *op = (klr_OSET_t*)pc;
		KLR_OSET(ctx, op->a1, op->a2);
		pc += OPSIZE_OSET;
		goto NEXT_OP;
	} 
	CASE(NSET) {
		const klr_NSET_t *op = (klr_NSET_t*)pc;
		KLR_NSET(ctx, op->a1, op->a2);
		pc += OPSIZE_NSET;
		goto NEXT_OP;
	} 
	CASE(MOV) {
		const klr_MOV_t *op = (klr_MOV_t*)pc;
		KLR_MOV(ctx, op->a1, op->a2);
		pc += OPSIZE_MOV;
		goto NEXT_OP;
	} 
	CASE(OMOV) {
		const klr_OMOV_t *op = (klr_OMOV_t*)pc;
		KLR_OMOV(ctx, op->a1, op->a2);
		pc += OPSIZE_OMOV;
		goto NEXT_OP;
	} 
	CASE(NMOV) {
		const klr_NMOV_t *op = (klr_NMOV_t*)pc;
		KLR_NMOV(ctx, op->a1, op->a2);
		pc += OPSIZE_NMOV;
		goto NEXT_OP;
	} 
	CASE(OMOVx) {
		const klr_OMOVx_t *op = (klr_OMOVx_t*)pc;
		KLR_OMOVx(ctx, op->a1, op->a2);
		pc += OPSIZE_OMOVx;
		goto NEXT_OP;
	} 
	CASE(iMOVx) {
		const klr_iMOVx_t *op = (klr_iMOVx_t*)pc;
		KLR_iMOVx(ctx, op->a1, op->a2);
		pc += OPSIZE_iMOVx;
		goto NEXT_OP;
	} 
	CASE(fMOVx) {
		const klr_fMOVx_t *op = (klr_fMOVx_t*)pc;
		KLR_fMOVx(ctx, op->a1, op->a2);
		pc += OPSIZE_fMOVx;
		goto NEXT_OP;
	} 
	CASE(bMOVx) {
		const klr_bMOVx_t *op = (klr_bMOVx_t*)pc;
		KLR_bMOVx(ctx, op->a1, op->a2);
		pc += OPSIZE_bMOVx;
		goto NEXT_OP;
	} 
	CASE(XMOV) {
		const klr_XMOV_t *op = (klr_XMOV_t*)pc;
		KLR_XMOV(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOV;
		goto NEXT_OP;
	} 
	CASE(XMOVx) {
		const klr_XMOVx_t *op = (klr_XMOVx_t*)pc;
		KLR_XMOVx(ctx, op->a1, op->a2);
		pc += OPSIZE_XMOVx;
		goto NEXT_OP;
	} 
	CASE(XOSET) {
		const klr_XOSET_t *op = (klr_XOSET_t*)pc;
		KLR_XOSET(ctx, op->a1, op->a2);
		pc += OPSIZE_XOSET;
		goto NEXT_OP;
	} 
	CASE(XIMOV) {
		const klr_XIMOV_t *op = (klr_XIMOV_t*)pc;
		KLR_XIMOV(ctx, op->a1, op->a2);
		pc += OPSIZE_XIMOV;
		goto NEXT_OP;
	} 
	CASE(XFMOV) {
		const klr_XFMOV_t *op = (klr_XFMOV_t*)pc;
		KLR_XFMOV(ctx, op->a1, op->a2);
		pc += OPSIZE_XFMOV;
		goto NEXT_OP;
	} 
	CASE(XBMOV) {
		const klr_XBMOV_t *op = (klr_XBMOV_t*)pc;
		KLR_XBMOV(ctx, op->a1, op->a2);
		pc += OPSIZE_XBMOV;
		goto NEXT_OP;
	} 
	CASE(MOVe) {
		const klr_MOVe_t *op = (klr_MOVe_t*)pc;
		KLR_MOVe(ctx, op->a1, op->a2);
		pc += OPSIZE_MOVe;
		goto NEXT_OP;
	} 
	CASE(SWAP) {
		const klr_SWAP_t *op = (klr_SWAP_t*)pc;
		KLR_SWAP(ctx, op->a1, op->a2);
		pc += OPSIZE_SWAP;
		goto NEXT_OP;
	} 
	CASE(CHKNUL) {
		const klr_CHKNUL_t *op = (klr_CHKNUL_t*)pc;
		KLR_CHKNUL(ctx, op->a1);
		pc += OPSIZE_CHKNUL;
		goto NEXT_OP;
	} 
	CASE(CHKTYPE) {
		const klr_CHKTYPE_t *op = (klr_CHKTYPE_t*)pc;
		KLR_CHKTYPE(ctx, op->a1, op->a2);
		pc += OPSIZE_CHKTYPE;
		goto NEXT_OP;
	} 
	CASE(CHKNULx) {
		const klr_CHKNULx_t *op = (klr_CHKNULx_t*)pc;
		KLR_CHKNULx(ctx, op->a1);
		pc += OPSIZE_CHKNULx;
		goto NEXT_OP;
	} 
	CASE(CHKTYPEx) {
		const klr_CHKTYPEx_t *op = (klr_CHKTYPEx_t*)pc;
		KLR_CHKTYPEx(ctx, op->a1, op->a2);
		pc += OPSIZE_CHKTYPEx;
		goto NEXT_OP;
	} 
	CASE(LDMETHOD) {
		const klr_LDMETHOD_t *op = (klr_LDMETHOD_t*)pc;
		KLR_LDMETHOD(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_LDMETHOD;
		goto NEXT_OP;
	} 
	CASE(CALL) {
		const klr_CALL_t *op = (klr_CALL_t*)pc;
		KLR_CALL(ctx, op->a1, op->a2);
		pc += OPSIZE_CALL;
		goto NEXT_OP;
	} 
	CASE(SCALL) {
		const klr_SCALL_t *op = (klr_SCALL_t*)pc;
		KLR2_SCALL(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_SCALL;
		goto NEXT_OP;
	} 
	CASE(VCALL) {
		const klr_VCALL_t *op = (klr_VCALL_t*)pc;
		KLR2_VCALL(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_VCALL;
		goto NEXT_OP;
	} 
	CASE(RET) {
		TC(const klr_RET_t *op = (klr_RET_t*)pc;)
		KLR_RET(ctx);
		pc += OPSIZE_RET;
		goto NEXT_OP;
	} 
	CASE(FUNCCALL) {
		TC(const klr_FUNCCALL_t *op = (klr_FUNCCALL_t*)pc;)
		KLR0_FUNCCALL(ctx);
		pc += OPSIZE_FUNCCALL;
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
	CASE(JMP) {
		const klr_JMP_t *op = (klr_JMP_t*)pc;
		KLR_JMP(ctx, pc = ((knh_inst_t*)op)->jumppc, JUMP);
		pc += OPSIZE_JMP;
		goto NEXT_OP;
	} 
	CASE(NOPJMP) {
		const klr_NOPJMP_t *op = (klr_NOPJMP_t*)pc;
		KLR_NOPJMP(ctx, pc = ((knh_inst_t*)op)->jumppc, JUMP);
		pc += OPSIZE_NOPJMP;
		goto NEXT_OP;
	} 
	CASE(JMPT) {
		const klr_JMPT_t *op = (klr_JMPT_t*)pc;
		KLR_JMPT(ctx, pc = ((knh_inst_t*)op)->jumppc, JUMP, op->a2);
		pc += OPSIZE_JMPT;
		goto NEXT_OP;
	} 
	CASE(JMPF) {
		const klr_JMPF_t *op = (klr_JMPF_t*)pc;
		KLR_JMPF(ctx, pc = ((knh_inst_t*)op)->jumppc, JUMP, op->a2);
		pc += OPSIZE_JMPF;
		goto NEXT_OP;
	} 
	CASE(JMPF_LOOP) {
		const klr_JMPF_LOOP_t *op = (klr_JMPF_LOOP_t*)pc;
		KLR2_JMPF_LOOP(ctx, pc = ((knh_inst_t*)op)->jumppc, JUMP, op->a2);
		pc += OPSIZE_JMPF_LOOP;
		goto NEXT_OP;
	} 
	CASE(JMPNUL) {
		const klr_JMPNUL_t *op = (klr_JMPNUL_t*)pc;
		KLR_JMPNUL(ctx, pc = ((knh_inst_t*)op)->jumppc, JUMP, op->a2);
		pc += OPSIZE_JMPNUL;
		goto NEXT_OP;
	} 
	CASE(JMPNN) {
		const klr_JMPNN_t *op = (klr_JMPNN_t*)pc;
		KLR_JMPNN(ctx, pc = ((knh_inst_t*)op)->jumppc, JUMP, op->a2);
		pc += OPSIZE_JMPNN;
		goto NEXT_OP;
	} 
	CASE(JMPchk) {
		const klr_JMPchk_t *op = (klr_JMPchk_t*)pc;
		KLR_JMPchk(ctx, pc = ((knh_inst_t*)op)->jumppc, JUMP, op->a2, op->a3);
		pc += OPSIZE_JMPchk;
		goto NEXT_OP;
	} 
	CASE(JMPcmp) {
		const klr_JMPcmp_t *op = (klr_JMPcmp_t*)pc;
		KLR2_JMPcmp(ctx, pc = ((knh_inst_t*)op)->jumppc, JUMP, op->a2, op->a3, op->a4);
		pc += OPSIZE_JMPcmp;
		goto NEXT_OP;
	} 
	CASE(JMPcmpi) {
		const klr_JMPcmpi_t *op = (klr_JMPcmpi_t*)pc;
		KLR2_JMPcmpi(ctx, pc = ((knh_inst_t*)op)->jumppc, JUMP, op->a2, op->a3, op->a4);
		pc += OPSIZE_JMPcmpi;
		goto NEXT_OP;
	} 
	CASE(JMPcmpf) {
		const klr_JMPcmpf_t *op = (klr_JMPcmpf_t*)pc;
		KLR2_JMPcmpf(ctx, pc = ((knh_inst_t*)op)->jumppc, JUMP, op->a2, op->a3, op->a4);
		pc += OPSIZE_JMPcmpf;
		goto NEXT_OP;
	} 
	CASE(NEXT) {
		const klr_NEXT_t *op = (klr_NEXT_t*)pc;
		KLR_NEXT(ctx, pc = ((knh_inst_t*)op)->jumppc, JUMP, op->a2, op->a3);
		pc += OPSIZE_NEXT;
		goto NEXT_OP;
	} 
	CASE(NEXTf) {
		const klr_NEXTf_t *op = (klr_NEXTf_t*)pc;
		KLR_NEXTf(ctx, pc = ((knh_inst_t*)op)->jumppc, JUMP, op->a2, op->a3, op->a4, op->a5);
		pc += OPSIZE_NEXTf;
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
	CASE(iSUB) {
		const klr_iSUB_t *op = (klr_iSUB_t*)pc;
		KLR_iSUB(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iSUB;
		goto NEXT_OP;
	} 
	CASE(iMUL) {
		const klr_iMUL_t *op = (klr_iMUL_t*)pc;
		KLR_iMUL(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iMUL;
		goto NEXT_OP;
	} 
	CASE(iDIV) {
		const klr_iDIV_t *op = (klr_iDIV_t*)pc;
		KLR_iDIV(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iDIV;
		goto NEXT_OP;
	} 
	CASE(iMOD) {
		const klr_iMOD_t *op = (klr_iMOD_t*)pc;
		KLR_iMOD(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iMOD;
		goto NEXT_OP;
	} 
	CASE(iDIVn) {
		const klr_iDIVn_t *op = (klr_iDIVn_t*)pc;
		KLR_iDIVn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iDIVn;
		goto NEXT_OP;
	} 
	CASE(iADDn) {
		const klr_iADDn_t *op = (klr_iADDn_t*)pc;
		KLR_iADDn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iADDn;
		goto NEXT_OP;
	} 
	CASE(iSUBn) {
		const klr_iSUBn_t *op = (klr_iSUBn_t*)pc;
		KLR_iSUBn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iSUBn;
		goto NEXT_OP;
	} 
	CASE(iMULn) {
		const klr_iMULn_t *op = (klr_iMULn_t*)pc;
		KLR_iMULn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iMULn;
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
	CASE(iNEQ) {
		const klr_iNEQ_t *op = (klr_iNEQ_t*)pc;
		KLR_iNEQ(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iNEQ;
		goto NEXT_OP;
	} 
	CASE(iLT) {
		const klr_iLT_t *op = (klr_iLT_t*)pc;
		KLR_iLT(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iLT;
		goto NEXT_OP;
	} 
	CASE(iLTE) {
		const klr_iLTE_t *op = (klr_iLTE_t*)pc;
		KLR_iLTE(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iLTE;
		goto NEXT_OP;
	} 
	CASE(iGT) {
		const klr_iGT_t *op = (klr_iGT_t*)pc;
		KLR_iGT(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iGT;
		goto NEXT_OP;
	} 
	CASE(iGTE) {
		const klr_iGTE_t *op = (klr_iGTE_t*)pc;
		KLR_iGTE(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iGTE;
		goto NEXT_OP;
	} 
	CASE(iEQn) {
		const klr_iEQn_t *op = (klr_iEQn_t*)pc;
		KLR_iEQn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iEQn;
		goto NEXT_OP;
	} 
	CASE(iNEQn) {
		const klr_iNEQn_t *op = (klr_iNEQn_t*)pc;
		KLR_iNEQn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iNEQn;
		goto NEXT_OP;
	} 
	CASE(iLTn) {
		const klr_iLTn_t *op = (klr_iLTn_t*)pc;
		KLR_iLTn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iLTn;
		goto NEXT_OP;
	} 
	CASE(iLTEn) {
		const klr_iLTEn_t *op = (klr_iLTEn_t*)pc;
		KLR_iLTEn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iLTEn;
		goto NEXT_OP;
	} 
	CASE(iGTn) {
		const klr_iGTn_t *op = (klr_iGTn_t*)pc;
		KLR_iGTn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_iGTn;
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
	CASE(fSUB) {
		const klr_fSUB_t *op = (klr_fSUB_t*)pc;
		KLR_fSUB(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fSUB;
		goto NEXT_OP;
	} 
	CASE(fMUL) {
		const klr_fMUL_t *op = (klr_fMUL_t*)pc;
		KLR_fMUL(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fMUL;
		goto NEXT_OP;
	} 
	CASE(fDIV) {
		const klr_fDIV_t *op = (klr_fDIV_t*)pc;
		KLR_fDIV(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fDIV;
		goto NEXT_OP;
	} 
	CASE(fADDn) {
		const klr_fADDn_t *op = (klr_fADDn_t*)pc;
		KLR_fADDn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fADDn;
		goto NEXT_OP;
	} 
	CASE(fSUBn) {
		const klr_fSUBn_t *op = (klr_fSUBn_t*)pc;
		KLR_fSUBn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fSUBn;
		goto NEXT_OP;
	} 
	CASE(fMULn) {
		const klr_fMULn_t *op = (klr_fMULn_t*)pc;
		KLR_fMULn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fMULn;
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
	CASE(fNEQ) {
		const klr_fNEQ_t *op = (klr_fNEQ_t*)pc;
		KLR_fNEQ(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fNEQ;
		goto NEXT_OP;
	} 
	CASE(fEQn) {
		const klr_fEQn_t *op = (klr_fEQn_t*)pc;
		KLR_fEQn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fEQn;
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
	CASE(fLTE) {
		const klr_fLTE_t *op = (klr_fLTE_t*)pc;
		KLR_fLTE(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fLTE;
		goto NEXT_OP;
	} 
	CASE(fGT) {
		const klr_fGT_t *op = (klr_fGT_t*)pc;
		KLR_fGT(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fGT;
		goto NEXT_OP;
	} 
	CASE(fGTE) {
		const klr_fGTE_t *op = (klr_fGTE_t*)pc;
		KLR_fGTE(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fGTE;
		goto NEXT_OP;
	} 
	CASE(fLTn) {
		const klr_fLTn_t *op = (klr_fLTn_t*)pc;
		KLR_fLTn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fLTn;
		goto NEXT_OP;
	} 
	CASE(fLTEn) {
		const klr_fLTEn_t *op = (klr_fLTEn_t*)pc;
		KLR_fLTEn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fLTEn;
		goto NEXT_OP;
	} 
	CASE(fGTn) {
		const klr_fGTn_t *op = (klr_fGTn_t*)pc;
		KLR_fGTn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fGTn;
		goto NEXT_OP;
	} 
	CASE(fGTEn) {
		const klr_fGTEn_t *op = (klr_fGTEn_t*)pc;
		KLR_fGTEn(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_fGTEn;
		goto NEXT_OP;
	} 
	CASE(OEVAL) {
		const klr_OEVAL_t *op = (klr_OEVAL_t*)pc;
		KLR_OEVAL(ctx, op->a1, op->a2, op->a3);
		pc += OPSIZE_OEVAL;
		goto NEXT_OP;
	} 
	CASE(GETIDX) {
		const klr_GETIDX_t *op = (klr_GETIDX_t*)pc;
		KLR_GETIDX(ctx, op->a1, op->a2, op->a3, op->a4);
		pc += OPSIZE_GETIDX;
		goto NEXT_OP;
	} 
	CASE(SETIDX) {
		const klr_SETIDX_t *op = (klr_SETIDX_t*)pc;
		KLR_SETIDX(ctx, op->a1, op->a2, op->a3, op->a4, op->a5);
		pc += OPSIZE_SETIDX;
		goto NEXT_OP;
	} 
	CASE(GETIDXn) {
		const klr_GETIDXn_t *op = (klr_GETIDXn_t*)pc;
		KLR_GETIDXn(ctx, op->a1, op->a2, op->a3, op->a4);
		pc += OPSIZE_GETIDXn;
		goto NEXT_OP;
	} 
	CASE(SETIDXn) {
		const klr_SETIDXn_t *op = (klr_SETIDXn_t*)pc;
		KLR_SETIDXn(ctx, op->a1, op->a2, op->a3, op->a4, op->a5);
		pc += OPSIZE_SETIDXn;
		goto NEXT_OP;
	} 
	CASE(THCODE) {
		TC(const klr_THCODE_t *op = (klr_THCODE_t*)pc;)
		KLR0_THCODE(ctx);
		pc += OPSIZE_THCODE;
		goto NEXT_OP;
	} 
	CASE(LABEL) {
		const klr_LABEL_t *op = (klr_LABEL_t*)pc;
		KLR_LABEL(ctx, op->a1, op->a2);
		pc += OPSIZE_LABEL;
		goto NEXT_OP;
	} 
	CASE(PROBE) {
		const klr_PROBE_t *op = (klr_PROBE_t*)pc;
		KLR0_PROBE(ctx, op->a1, op->a2);
		pc += OPSIZE_PROBE;
		goto NEXT_OP;
	} 
	CASE(NOP) {
		TC(const klr_NOP_t *op = (klr_NOP_t*)pc;)
		KLR_NOP(ctx);
		pc += OPSIZE_NOP;
		goto NEXT_OP;
	} 
	DISPATCH_END(pc);
	return NULL;
}


#ifdef __cplusplus
}
#endif
