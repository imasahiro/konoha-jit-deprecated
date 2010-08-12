/****************************************************************************
 * KONOHA COPYRIGHT, LICENSE NOTICE, AND DISCRIMER  
 * 
 * Copyright (c) 2006-2010, Kimio Kuramitsu <kimio at ynu.ac.jp>
 *           (c) 2008-      Konoha Team konohaken@googlegroups.com  
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

#define K_USING_VMINLINE
#include "vminline.c"

/* ======================================================================== */
/* [data] */


typedef struct {
	const char *name;
	size_t size;
	knh_ushort_t types[6];
} knh_OPDATA_t;

static const knh_OPDATA_t OPDATA[] = {
	{"HALT", 0, { VMT_VOID}}, 
	{"THCODE", 0, { VMT_VOID}}, 
	{"FUNCCALL", 0, { VMT_VOID}}, 
	{"ENTER", 0, { VMT_VOID}}, 
	{"PROBE", 2, { VMT_F, VMT_U, VMT_VOID}}, 
	{"VEXEC", 0, { VMT_VOID}}, 
	{"YEILD", 1, { VMT_SFPIDX, VMT_VOID}}, 
	{"EXIT", 0, { VMT_VOID}}, 
	{"P", 5, { VMT_F, VMT_U, VMT_STRING, VMT_MTD, VMT_SFPIDX, VMT_VOID}}, 
	{"OSET", 2, { VMT_SFPIDX, VMT_OBJECT, VMT_VOID}}, 
	{"NSET", 2+VMTSIZE_int, { VMT_SFPIDX, VMT_INT VMTX_INT, VMT_VOID}}, 
	{"OMOV", 2, { VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"NMOV", 2, { VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"SWAP", 2, { VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"UNBOX", 2, { VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"ONMOV", 4, { VMT_SFPIDX, VMT_SFPIDX, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"OOMOV", 4, { VMT_SFPIDX, VMT_SFPIDX, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"NNMOV", 4, { VMT_SFPIDX, VMT_SFPIDX, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"OMOVx", 2+1, { VMT_SFPIDX, VMT_SFX, VMT_VOID, VMT_VOID}}, 
	{"iMOVx", 2+1, { VMT_SFPIDX, VMT_SFX, VMT_VOID, VMT_VOID}}, 
	{"fMOVx", 2+1, { VMT_SFPIDX, VMT_SFX, VMT_VOID, VMT_VOID}}, 
	{"bMOVx", 2+1, { VMT_SFPIDX, VMT_SFX, VMT_VOID, VMT_VOID}}, 
	{"XMOV", 2+1, { VMT_SFX, VMT_VOID, VMT_SFPIDX, VMT_VOID}}, 
	{"XMOVx", 2+1+1, { VMT_SFX, VMT_VOID, VMT_SFX, VMT_VOID, VMT_VOID}}, 
	{"XOSET", 2+1, { VMT_SFX, VMT_VOID, VMT_OBJECT, VMT_VOID}}, 
	{"XIMOV", 2+1, { VMT_SFX, VMT_VOID, VMT_SFPIDX, VMT_VOID}}, 
	{"XFMOV", 2+1, { VMT_SFX, VMT_VOID, VMT_SFPIDX, VMT_VOID}}, 
	{"XBMOV", 2+1, { VMT_SFX, VMT_VOID, VMT_SFPIDX, VMT_VOID}}, 
	{"CHKSTACK", 1, { VMT_SFPIDX, VMT_VOID}}, 
	{"LOADMTD", 3, { VMT_SFPIDX, VMT_F, VMT_MTD, VMT_VOID}}, 
	{"CALL", 2, { VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"SCALL", 3, { VMT_SFPIDX, VMT_SFPIDX, VMT_MTD, VMT_VOID}}, 
	{"VCALL", 3, { VMT_SFPIDX, VMT_SFPIDX, VMT_MTD, VMT_VOID}}, 
	{"VCALL", 3, { VMT_SFPIDX, VMT_SFPIDX, VMT_MTD, VMT_VOID}}, 
	{"FASTCALL", 3, { VMT_SFPIDX, VMT_SFPIDX, VMT_F, VMT_VOID}}, 
	{"RET", 0, { VMT_VOID}}, 
	{"SCAST", 3, { VMT_SFPIDX, VMT_SFPIDX, VMT_TRL, VMT_VOID}}, 
	{"TCAST", 3, { VMT_SFPIDX, VMT_SFPIDX, VMT_TRL, VMT_VOID}}, 
	{"ACAST", 3, { VMT_SFPIDX, VMT_SFPIDX, VMT_TRL, VMT_VOID}}, 
	{"iCAST", 2, { VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"fCAST", 2, { VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"TR", 4, { VMT_SFPIDX, VMT_SFPIDX, VMT_CID, VMT_F, VMT_VOID}}, 
	{"NUL", 2, { VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"JMP", 1, { VMT_ADDR, VMT_VOID}}, 
	{"JMP", 1, { VMT_ADDR, VMT_VOID}}, 
	{"JMPF", 2, { VMT_ADDR, VMT_SFPIDX, VMT_VOID}}, 
	{"DYJMP", 3, { VMT_ADDR, VMT_SFPIDX, VMT_F, VMT_VOID}}, 
	{"NEXT", 3, { VMT_ADDR, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"TRY", 2, { VMT_ADDR, VMT_SFPIDX, VMT_VOID}}, 
	{"TRYEND", 1, { VMT_SFPIDX, VMT_VOID}}, 
	{"THROW", 1, { VMT_SFPIDX, VMT_VOID}}, 
	{"CATCH", 3, { VMT_ADDR, VMT_SFPIDX, VMT_STRING, VMT_VOID}}, 
	{"bNOT", 2, { VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"iINC", 1, { VMT_SFPIDX, VMT_VOID}}, 
	{"iDEC", 1, { VMT_SFPIDX, VMT_VOID}}, 
	{"iNEG", 2, { VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"iADD", 3, { VMT_SFPIDX, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"iSUB", 3, { VMT_SFPIDX, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"iMUL", 3, { VMT_SFPIDX, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"iDIV", 3, { VMT_SFPIDX, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"iMOD", 3, { VMT_SFPIDX, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"iEQ", 3, { VMT_SFPIDX, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"iNEQ", 3, { VMT_SFPIDX, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"iLT", 3, { VMT_SFPIDX, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"iLTE", 3, { VMT_SFPIDX, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"iGT", 3, { VMT_SFPIDX, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"iGTE", 3, { VMT_SFPIDX, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"iADDn", 3+VMTSIZE_int, { VMT_SFPIDX, VMT_SFPIDX, VMT_INT VMTX_INT, VMT_VOID}}, 
	{"iSUBn", 3+VMTSIZE_int, { VMT_SFPIDX, VMT_SFPIDX, VMT_INT VMTX_INT, VMT_VOID}}, 
	{"iMULn", 3+VMTSIZE_int, { VMT_SFPIDX, VMT_SFPIDX, VMT_INT VMTX_INT, VMT_VOID}}, 
	{"iDIVn", 3+VMTSIZE_int, { VMT_SFPIDX, VMT_SFPIDX, VMT_INT VMTX_INT, VMT_VOID}}, 
	{"iMODn", 3+VMTSIZE_int, { VMT_SFPIDX, VMT_SFPIDX, VMT_INT VMTX_INT, VMT_VOID}}, 
	{"iEQn", 3+VMTSIZE_int, { VMT_SFPIDX, VMT_SFPIDX, VMT_INT VMTX_INT, VMT_VOID}}, 
	{"iNEQn", 3+VMTSIZE_int, { VMT_SFPIDX, VMT_SFPIDX, VMT_INT VMTX_INT, VMT_VOID}}, 
	{"iLTn", 3+VMTSIZE_int, { VMT_SFPIDX, VMT_SFPIDX, VMT_INT VMTX_INT, VMT_VOID}}, 
	{"iLTEn", 3+VMTSIZE_int, { VMT_SFPIDX, VMT_SFPIDX, VMT_INT VMTX_INT, VMT_VOID}}, 
	{"iGTn", 3+VMTSIZE_int, { VMT_SFPIDX, VMT_SFPIDX, VMT_INT VMTX_INT, VMT_VOID}}, 
	{"iGTEn", 3+VMTSIZE_int, { VMT_SFPIDX, VMT_SFPIDX, VMT_INT VMTX_INT, VMT_VOID}}, 
	{"fNEG", 2, { VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"fADD", 3, { VMT_SFPIDX, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"fSUB", 3, { VMT_SFPIDX, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"fMUL", 3, { VMT_SFPIDX, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"fDIV", 3, { VMT_SFPIDX, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"fEQ", 3, { VMT_SFPIDX, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"fNEQ", 3, { VMT_SFPIDX, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"fLT", 3, { VMT_SFPIDX, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"fLTE", 3, { VMT_SFPIDX, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"fGT", 3, { VMT_SFPIDX, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"fGTE", 3, { VMT_SFPIDX, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"fADDn", 3+VMTSIZE_float, { VMT_SFPIDX, VMT_SFPIDX, VMT_FLOAT VMTX_FLOAT, VMT_VOID}}, 
	{"fSUBn", 3+VMTSIZE_float, { VMT_SFPIDX, VMT_SFPIDX, VMT_FLOAT VMTX_FLOAT, VMT_VOID}}, 
	{"fMULn", 3+VMTSIZE_float, { VMT_SFPIDX, VMT_SFPIDX, VMT_FLOAT VMTX_FLOAT, VMT_VOID}}, 
	{"fDIVn", 3+VMTSIZE_float, { VMT_SFPIDX, VMT_SFPIDX, VMT_FLOAT VMTX_FLOAT, VMT_VOID}}, 
	{"fEQn", 3+VMTSIZE_float, { VMT_SFPIDX, VMT_SFPIDX, VMT_FLOAT VMTX_FLOAT, VMT_VOID}}, 
	{"fNEQn", 3+VMTSIZE_float, { VMT_SFPIDX, VMT_SFPIDX, VMT_FLOAT VMTX_FLOAT, VMT_VOID}}, 
	{"fLTn", 3+VMTSIZE_float, { VMT_SFPIDX, VMT_SFPIDX, VMT_FLOAT VMTX_FLOAT, VMT_VOID}}, 
	{"fLTEn", 3+VMTSIZE_float, { VMT_SFPIDX, VMT_SFPIDX, VMT_FLOAT VMTX_FLOAT, VMT_VOID}}, 
	{"fGTn", 3+VMTSIZE_float, { VMT_SFPIDX, VMT_SFPIDX, VMT_FLOAT VMTX_FLOAT, VMT_VOID}}, 
	{"fGTEn", 3+VMTSIZE_float, { VMT_SFPIDX, VMT_SFPIDX, VMT_FLOAT VMTX_FLOAT, VMT_VOID}}, 
	{"OGETIDX", 3, { VMT_SFPIDX, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"OSETIDX", 4, { VMT_SFPIDX, VMT_SFPIDX, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"OGETIDXn", 3, { VMT_SFPIDX, VMT_SFPIDX, VMT_U, VMT_VOID}}, 
	{"OSETIDXn", 4, { VMT_SFPIDX, VMT_SFPIDX, VMT_U, VMT_SFPIDX, VMT_VOID}}, 
	{"NGETIDX", 3, { VMT_SFPIDX, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"NSETIDX", 4, { VMT_SFPIDX, VMT_SFPIDX, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"NGETIDXn", 3, { VMT_SFPIDX, VMT_SFPIDX, VMT_U, VMT_VOID}}, 
	{"NSETIDXn", 4, { VMT_SFPIDX, VMT_SFPIDX, VMT_SFPIDX, VMT_U, VMT_VOID}}, 
	{"bJNOT", 2, { VMT_ADDR, VMT_SFPIDX, VMT_VOID}}, 
	{"iJEQ", 3, { VMT_ADDR, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"iJNEQ", 3, { VMT_ADDR, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"iJLT", 3, { VMT_ADDR, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"iJLTE", 3, { VMT_ADDR, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"iJGT", 3, { VMT_ADDR, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"iJGTE", 3, { VMT_ADDR, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"iJEQn", 3+VMTSIZE_int, { VMT_ADDR, VMT_SFPIDX, VMT_INT VMTX_INT, VMT_VOID}}, 
	{"iJNEQn", 3+VMTSIZE_int, { VMT_ADDR, VMT_SFPIDX, VMT_INT VMTX_INT, VMT_VOID}}, 
	{"iJLTn", 3+VMTSIZE_int, { VMT_ADDR, VMT_SFPIDX, VMT_INT VMTX_INT, VMT_VOID}}, 
	{"iJLTEn", 3+VMTSIZE_int, { VMT_ADDR, VMT_SFPIDX, VMT_INT VMTX_INT, VMT_VOID}}, 
	{"iJGTn", 3+VMTSIZE_int, { VMT_ADDR, VMT_SFPIDX, VMT_INT VMTX_INT, VMT_VOID}}, 
	{"iJGTEn", 3+VMTSIZE_int, { VMT_ADDR, VMT_SFPIDX, VMT_INT VMTX_INT, VMT_VOID}}, 
	{"fJEQ", 3, { VMT_ADDR, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"fJNEQ", 3, { VMT_ADDR, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"fJLT", 3, { VMT_ADDR, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"fJLTE", 3, { VMT_ADDR, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"fJGT", 3, { VMT_ADDR, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"fJGTE", 3, { VMT_ADDR, VMT_SFPIDX, VMT_SFPIDX, VMT_VOID}}, 
	{"fJEQn", 3+VMTSIZE_float, { VMT_ADDR, VMT_SFPIDX, VMT_FLOAT VMTX_FLOAT, VMT_VOID}}, 
	{"fJNEQn", 3+VMTSIZE_float, { VMT_ADDR, VMT_SFPIDX, VMT_FLOAT VMTX_FLOAT, VMT_VOID}}, 
	{"fJLTn", 3+VMTSIZE_float, { VMT_ADDR, VMT_SFPIDX, VMT_FLOAT VMTX_FLOAT, VMT_VOID}}, 
	{"fJLTEn", 3+VMTSIZE_float, { VMT_ADDR, VMT_SFPIDX, VMT_FLOAT VMTX_FLOAT, VMT_VOID}}, 
	{"fJGTn", 3+VMTSIZE_float, { VMT_ADDR, VMT_SFPIDX, VMT_FLOAT VMTX_FLOAT, VMT_VOID}}, 
	{"fJGTEn", 3+VMTSIZE_float, { VMT_ADDR, VMT_SFPIDX, VMT_FLOAT VMTX_FLOAT, VMT_VOID}}, 
	{"NOP", 0, { VMT_VOID}}, 
};

void knh_opcode_check(void)
{
	KNH_ASSERT(sizeof(klr_HALT_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_THCODE_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_FUNCCALL_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_ENTER_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_PROBE_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_VEXEC_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_YEILD_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_EXIT_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_P_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_OSET_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_NSET_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_OMOV_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_NMOV_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_SWAP_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_UNBOX_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_ONMOV_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_OOMOV_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_NNMOV_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_OMOVx_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iMOVx_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_fMOVx_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_bMOVx_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_XMOV_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_XMOVx_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_XOSET_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_XIMOV_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_XFMOV_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_XBMOV_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_CHKSTACK_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_LOADMTD_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_CALL_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_SCALL_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_VCALL_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_VCALL__t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_FASTCALL_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_RET_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_SCAST_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_TCAST_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_ACAST_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iCAST_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_fCAST_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_TR_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_NUL_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_JMP_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_JMP__t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_JMPF_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_DYJMP_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_NEXT_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_TRY_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_TRYEND_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_THROW_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_CATCH_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_bNOT_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iINC_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iDEC_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iNEG_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iADD_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iSUB_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iMUL_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iDIV_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iMOD_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iEQ_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iNEQ_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iLT_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iLTE_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iGT_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iGTE_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iADDn_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iSUBn_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iMULn_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iDIVn_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iMODn_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iEQn_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iNEQn_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iLTn_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iLTEn_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iGTn_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iGTEn_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_fNEG_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_fADD_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_fSUB_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_fMUL_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_fDIV_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_fEQ_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_fNEQ_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_fLT_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_fLTE_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_fGT_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_fGTE_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_fADDn_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_fSUBn_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_fMULn_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_fDIVn_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_fEQn_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_fNEQn_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_fLTn_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_fLTEn_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_fGTn_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_fGTEn_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_OGETIDX_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_OSETIDX_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_OGETIDXn_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_OSETIDXn_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_NGETIDX_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_NSETIDX_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_NGETIDXn_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_NSETIDXn_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_bJNOT_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iJEQ_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iJNEQ_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iJLT_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iJLTE_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iJGT_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iJGTE_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iJEQn_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iJNEQn_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iJLTn_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iJLTEn_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iJGTn_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_iJGTEn_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_fJEQ_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_fJNEQ_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_fJLT_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_fJLTE_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_fJGT_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_fJGTE_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_fJEQn_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_fJNEQn_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_fJLTn_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_fJLTEn_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_fJGTn_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_fJGTEn_t) <= sizeof(knh_opline_t));
	KNH_ASSERT(sizeof(klr_NOP_t) <= sizeof(knh_opline_t));
}

/* ------------------------------------------------------------------------ */
#ifdef K_USING_VMCOUNT
static int opcode_count[OPCODE_MAX] = {0};

/* ------------------------------------------------------------------------ */
void knh_opcode_stat(Ctx *ctx)
{
	int i;
	for(i = 0; i < OPCODE_MAX; i++) {
		KNH_SYSLOG(ctx, LOG_INFO,
				"[VMCODE COUNTER] %s count=%d",
				knh_opcode_tochar(i), opcode_count[i]);
	}
}

/* ------------------------------------------------------------------------ */

void knh_opcode_count(Ctx *ctx, knh_opline_t *c)
{
	int count = c->count;
	opcode_count[c->opcode] += count;
}

#endif
/* ------------------------------------------------------------------------ */

const char *knh_opcode_tochar(knh_opcode_t opcode)
{
	if(opcode < OPCODE_MAX) {
		return OPDATA[opcode].name;
	}
	else {
		DBG_P("opcode=%d", (int)opcode);
		return "OPCODE_??";
	}
}

/* ------------------------------------------------------------------------ */
size_t knh_opcode_size(knh_opcode_t opcode)
{
	return OPDATA[opcode].size;
}

/* ------------------------------------------------------------------------ */
knh_bool_t knh_opcode_hasjump(knh_opcode_t opcode)
{
	return (OPDATA[opcode].types[0] == VMT_ADDR);
}
/* ------------------------------------------------------------------------ */
void knh_opline_traverse(Ctx *ctx, knh_opline_t *c, knh_Ftraverse ftr)
{
	size_t i, size = OPDATA[c->opcode].size;
	for(i = 0; i < size; i++) {
		knh_ushort_t vtype = OPDATA[c->opcode].types[i];
		if(vtype == VMT_OBJECT || vtype == VMT_STRING) {
			KNH_FTR(ctx, ftr, UPCAST(c->p[i]));
		}
	}
}
/* ------------------------------------------------------------------------ */
void knh_opcode_dump(Ctx *ctx, knh_opline_t *c, knh_OutputStream_t *w, knh_opline_t *pc_start)
{
	size_t i, size = OPDATA[c->opcode].size;
	const knh_ushort_t *vmt = OPDATA[c->opcode].types;
	if(pc_start == NULL) {
		knh_printf(ctx, w, "[%p:%d] %s(%d)", c, c->line, knh_opcode_tochar(c->opcode), (knh_intptr_t)c->opcode);
	}
	else {
		knh_printf(ctx, w, "L%d(%d): %s(%d)", c - pc_start, c->line, knh_opcode_tochar(c->opcode), (knh_intptr_t)c->opcode);
	}
	for(i = 0; i < size; i++) {
		knh_putc(ctx, w, ' '); 
		switch(vmt[i]) {
		case VMT_VOID: break;
		case VMT_ADDR: 
			if(pc_start == NULL) {
				knh_printf(ctx, w, "%p", c->p[i]); break;
			}
			else {
				knh_printf(ctx, w, "L%d", (knh_opline_t*)c->p[i] - pc_start); break;
			}
		case VMT_SFPIDX:
			knh_printf(ctx, w, "sfp[%d]", c->data[i]); break;
		case VMT_SFX:
			knh_printf(ctx, w, "sfx[%d]+%d", c->data[i], c->data[i+1]); break;
		case VMT_U:
			knh_write_dfmt(ctx, w, K_INTPTR_FMT, c->data[i]); break;
		case VMT_F:
			knh_write_vmfunc(ctx, w, c->p[i]); break;
		case VMT_CID:
			knh_write_cid(ctx, w, (knh_class_t)c->data[i]); break;
		case VMT_MTD: if(c->p[i] != NULL) {
			knh_Method_t *mtd = (knh_Method_t*)c->p[i];
			knh_write_cid(ctx, w, DP(mtd)->cid); knh_putc(ctx, w, '.');
			knh_write_mn(ctx, w, DP(mtd)->mn); 
		}
		break;
		case VMT_TRL:
		case VMT_OBJECT:
		case VMT_STRING: {
			knh_Method_t *mtd = knh_getSystemFormatter(ctx, CLASS_String, MN__k);
			knh_write_Object(ctx, w, ctx->esp, &mtd, UPCAST(c->p[i]));
			break;
		}
		case VMT_INT: {
			knh_int_t n = ((knh_int_t*)(&(c->p[i])))[0];
			knh_write_ifmt(ctx, w, K_INT_FMT, n); break;
		}
		case VMT_FLOAT:
			knh_write_ffmt(ctx, w, K_FLOAT_FMT, *((knh_float_t*)&(c->p[i]))); break;
		}
	}
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */
void knh_opcode_idxshift(knh_opline_t *c, int idxshift)
{
	size_t i, size = OPDATA[c->opcode].size;
	const knh_ushort_t *vmt = OPDATA[c->opcode].types;
	for(i = 0; i < size; i++) {
		switch(vmt[i]) {
			case VMT_SFPIDX:
			case VMT_SFX: c->data[i] = c->data[i] + idxshift; break;
		}
	}
}


/* ======================================================================== */
/* [exec] */

#ifdef K_USING_VMCOUNT
#define VMCOUNT(op)    ((op)->count)++;
#else
#define VMCOUNT(op)
#endif

#ifdef K_USING_THREADEDCODE
#define CASE(x)   L_##x :
#define NEXT_OP   (pc->codeaddr)
#define JUMP      *(NEXT_OP)
#if (defined(__i386__) || defined(__x86_64__)) && (defined(__GNUC__) && __GNUC__ >= 3)
#define DISPATCH_ASM
#define GOTO_NEXT()  asm volatile("jmp *%0;": : "r"(NEXT_OP))
#else
#define GOTO_NEXT()  goto *(NEXT_OP)
#endif
#define TC(c) 
#if defined(DISPATCH_ASM) && defined(__i386__)
#define DISPATCH_INIT()    asm("" :: "r"(&vpc))
#else
#define DISPATCH_INIT()
#endif
#define DISPATCH_START(pc) goto *OPJUMP[pc->opcode]
#define DISPATCH_END(pc)
#define GOTO_PC(pc)        GOTO_NEXT()
#else
#define OPJUMP      NULL
#define CASE(x)     case OPCODE_##x :
#define NEXT_OP     L_HEAD
#define GOTO_NEXT() goto NEXT_OP
#define JUMP        L_HEAD
#define TC(c)
#define DISPATCH_INIT()
#define DISPATCH_START(pc) L_HEAD:;switch(pc->opcode) {
#define DISPATCH_END(pc)   } KNH_SYSLOG(ctx, LOG_CRIT, "VM", "unknown opcode=%d", pc->opcode); 
#define GOTO_PC(pc)         GOTO_NEXT()
#endif/*K_USING_THREADEDCODE*/

knh_opline_t* knh_VirtualMachine_run(Ctx *ctx, knh_sfp_t *sfp0, knh_opline_t *pc0)
{
#ifdef K_USING_THREADEDCODE
	static void *OPJUMP[] = {
		&&L_HALT, &&L_THCODE, &&L_FUNCCALL, &&L_ENTER, 
		&&L_PROBE, &&L_VEXEC, &&L_YEILD, &&L_EXIT, 
		&&L_P, &&L_OSET, &&L_NSET, &&L_OMOV, 
		&&L_NMOV, &&L_SWAP, &&L_UNBOX, &&L_ONMOV, 
		&&L_OOMOV, &&L_NNMOV, &&L_OMOVx, &&L_iMOVx, 
		&&L_fMOVx, &&L_bMOVx, &&L_XMOV, &&L_XMOVx, 
		&&L_XOSET, &&L_XIMOV, &&L_XFMOV, &&L_XBMOV, 
		&&L_CHKSTACK, &&L_LOADMTD, &&L_CALL, &&L_SCALL, 
		&&L_VCALL, &&L_VCALL_, &&L_FASTCALL, &&L_RET, 
		&&L_SCAST, &&L_TCAST, &&L_ACAST, &&L_iCAST, 
		&&L_fCAST, &&L_TR, &&L_NUL, &&L_JMP, 
		&&L_JMP_, &&L_JMPF, &&L_DYJMP, &&L_NEXT, 
		&&L_TRY, &&L_TRYEND, &&L_THROW, &&L_CATCH, 
		&&L_bNOT, &&L_iINC, &&L_iDEC, &&L_iNEG, 
		&&L_iADD, &&L_iSUB, &&L_iMUL, &&L_iDIV, 
		&&L_iMOD, &&L_iEQ, &&L_iNEQ, &&L_iLT, 
		&&L_iLTE, &&L_iGT, &&L_iGTE, &&L_iADDn, 
		&&L_iSUBn, &&L_iMULn, &&L_iDIVn, &&L_iMODn, 
		&&L_iEQn, &&L_iNEQn, &&L_iLTn, &&L_iLTEn, 
		&&L_iGTn, &&L_iGTEn, &&L_fNEG, &&L_fADD, 
		&&L_fSUB, &&L_fMUL, &&L_fDIV, &&L_fEQ, 
		&&L_fNEQ, &&L_fLT, &&L_fLTE, &&L_fGT, 
		&&L_fGTE, &&L_fADDn, &&L_fSUBn, &&L_fMULn, 
		&&L_fDIVn, &&L_fEQn, &&L_fNEQn, &&L_fLTn, 
		&&L_fLTEn, &&L_fGTn, &&L_fGTEn, &&L_OGETIDX, 
		&&L_OSETIDX, &&L_OGETIDXn, &&L_OSETIDXn, &&L_NGETIDX, 
		&&L_NSETIDX, &&L_NGETIDXn, &&L_NSETIDXn, &&L_bJNOT, 
		&&L_iJEQ, &&L_iJNEQ, &&L_iJLT, &&L_iJLTE, 
		&&L_iJGT, &&L_iJGTE, &&L_iJEQn, &&L_iJNEQn, 
		&&L_iJLTn, &&L_iJLTEn, &&L_iJGTn, &&L_iJGTEn, 
		&&L_fJEQ, &&L_fJNEQ, &&L_fJLT, &&L_fJLTE, 
		&&L_fJGT, &&L_fJGTE, &&L_fJEQn, &&L_fJNEQn, 
		&&L_fJLTn, &&L_fJLTEn, &&L_fJGTn, &&L_fJGTEn, 
		&&L_NOP, 
	};
#endif
	knh_sfp_t *sfp = sfp0;
	knh_opline_t* pc = pc0;
	volatile knh_opline_t* vpc = NULL;
	volatile knh_intptr_t vshift = 0;
	DISPATCH_INIT();
	DISPATCH_START(pc);

	CASE(HALT) {
		klr_HALT_t *op = (klr_HALT_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR0_HALT(ctx);
		GOTO_NEXT();
	} 
	CASE(THCODE) {
		klr_THCODE_t *op = (klr_THCODE_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR0_THCODE(ctx);
		GOTO_NEXT();
	} 
	CASE(FUNCCALL) {
		klr_FUNCCALL_t *op = (klr_FUNCCALL_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR0_FUNCCALL(ctx);
		GOTO_NEXT();
	} 
	CASE(ENTER) {
		klr_ENTER_t *op = (klr_ENTER_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR0_ENTER(ctx);
		GOTO_NEXT();
	} 
	CASE(PROBE) {
		klr_PROBE_t *op = (klr_PROBE_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR0_PROBE(ctx, op->probe, op->n);
		GOTO_NEXT();
	} 
	CASE(VEXEC) {
		klr_VEXEC_t *op = (klr_VEXEC_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR0_VEXEC(ctx);
		GOTO_NEXT();
	} 
	CASE(YEILD) {
		klr_YEILD_t *op = (klr_YEILD_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR0_YEILD(ctx, op->n);
		GOTO_NEXT();
	} 
	CASE(EXIT) {
		klr_EXIT_t *op = (klr_EXIT_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR0_EXIT(ctx);
		GOTO_NEXT();
	} 
	CASE(P) {
		klr_P_t *op = (klr_P_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_P(ctx, op->print, op->flag, op->msg, op->fmt, op->n);
		GOTO_NEXT();
	} 
	CASE(OSET) {
		klr_OSET_t *op = (klr_OSET_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_OSET(ctx, op->a, op->o);
		GOTO_NEXT();
	} 
	CASE(NSET) {
		klr_NSET_t *op = (klr_NSET_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_NSET(ctx, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(OMOV) {
		klr_OMOV_t *op = (klr_OMOV_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_OMOV(ctx, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(NMOV) {
		klr_NMOV_t *op = (klr_NMOV_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_NMOV(ctx, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(SWAP) {
		klr_SWAP_t *op = (klr_SWAP_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_SWAP(ctx, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(UNBOX) {
		klr_UNBOX_t *op = (klr_UNBOX_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_UNBOX(ctx, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(ONMOV) {
		klr_ONMOV_t *op = (klr_ONMOV_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_ONMOV(ctx, op->a, op->b, op->c, op->d);
		GOTO_NEXT();
	} 
	CASE(OOMOV) {
		klr_OOMOV_t *op = (klr_OOMOV_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR2_OOMOV(ctx, op->a, op->b, op->c, op->d);
		GOTO_NEXT();
	} 
	CASE(NNMOV) {
		klr_NNMOV_t *op = (klr_NNMOV_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR2_NNMOV(ctx, op->a, op->b, op->c, op->d);
		GOTO_NEXT();
	} 
	CASE(OMOVx) {
		klr_OMOVx_t *op = (klr_OMOVx_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_OMOVx(ctx, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(iMOVx) {
		klr_iMOVx_t *op = (klr_iMOVx_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_iMOVx(ctx, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(fMOVx) {
		klr_fMOVx_t *op = (klr_fMOVx_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_fMOVx(ctx, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(bMOVx) {
		klr_bMOVx_t *op = (klr_bMOVx_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_bMOVx(ctx, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(XMOV) {
		klr_XMOV_t *op = (klr_XMOV_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_XMOV(ctx, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(XMOVx) {
		klr_XMOVx_t *op = (klr_XMOVx_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_XMOVx(ctx, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(XOSET) {
		klr_XOSET_t *op = (klr_XOSET_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_XOSET(ctx, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(XIMOV) {
		klr_XIMOV_t *op = (klr_XIMOV_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_XIMOV(ctx, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(XFMOV) {
		klr_XFMOV_t *op = (klr_XFMOV_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_XFMOV(ctx, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(XBMOV) {
		klr_XBMOV_t *op = (klr_XBMOV_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_XBMOV(ctx, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(CHKSTACK) {
		klr_CHKSTACK_t *op = (klr_CHKSTACK_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_CHKSTACK(ctx, op->n);
		GOTO_NEXT();
	} 
	CASE(LOADMTD) {
		klr_LOADMTD_t *op = (klr_LOADMTD_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_LOADMTD(ctx, op->thisidx, op->method, op->callmtd);
		GOTO_NEXT();
	} 
	CASE(CALL) {
		klr_CALL_t *op = (klr_CALL_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_CALL(ctx, op->thisidx, op->espshift);
		GOTO_NEXT();
	} 
	CASE(SCALL) {
		klr_SCALL_t *op = (klr_SCALL_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR2_SCALL(ctx, op->thisidx, op->espshift, op->callmtd);
		GOTO_NEXT();
	} 
	CASE(VCALL) {
		klr_VCALL_t *op = (klr_VCALL_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR2_VCALL(ctx, op->thisidx, op->espshift, op->callmtd);
		GOTO_NEXT();
	} 
	CASE(VCALL_) {
		klr_VCALL__t *op = (klr_VCALL__t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR2_VCALL_(ctx, op->thisidx, op->espshift, op->callmtd);
		GOTO_NEXT();
	} 
	CASE(FASTCALL) {
		klr_FASTCALL_t *op = (klr_FASTCALL_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_FASTCALL(ctx, op->a, op->b, op->fcall);
		GOTO_NEXT();
	} 
	CASE(RET) {
		klr_RET_t *op = (klr_RET_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_RET(ctx);
		GOTO_NEXT();
	} 
	CASE(SCAST) {
		klr_SCAST_t *op = (klr_SCAST_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_SCAST(ctx, op->a, op->b, op->cast);
		GOTO_NEXT();
	} 
	CASE(TCAST) {
		klr_TCAST_t *op = (klr_TCAST_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_TCAST(ctx, op->a, op->b, op->cast);
		GOTO_NEXT();
	} 
	CASE(ACAST) {
		klr_ACAST_t *op = (klr_ACAST_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_ACAST(ctx, op->a, op->b, op->cast);
		GOTO_NEXT();
	} 
	CASE(iCAST) {
		klr_iCAST_t *op = (klr_iCAST_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_iCAST(ctx, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(fCAST) {
		klr_fCAST_t *op = (klr_fCAST_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_fCAST(ctx, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(TR) {
		klr_TR_t *op = (klr_TR_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_TR(ctx, op->a, op->b, op->cid, op->tr);
		GOTO_NEXT();
	} 
	CASE(NUL) {
		klr_NUL_t *op = (klr_NUL_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_NUL(ctx, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(JMP) {
		klr_JMP_t *op = (klr_JMP_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_JMP(ctx, pc = op->jumppc, JUMP);
		GOTO_NEXT();
	} 
	CASE(JMP_) {
		klr_JMP__t *op = (klr_JMP__t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR2_JMP_(ctx, pc = op->jumppc, JUMP);
		GOTO_NEXT();
	} 
	CASE(JMPF) {
		klr_JMPF_t *op = (klr_JMPF_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_JMPF(ctx, pc = op->jumppc, JUMP, op->a);
		GOTO_NEXT();
	} 
	CASE(DYJMP) {
		klr_DYJMP_t *op = (klr_DYJMP_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_DYJMP(ctx, pc = op->jumppc, JUMP, op->a, op->chk);
		GOTO_NEXT();
	} 
	CASE(NEXT) {
		klr_NEXT_t *op = (klr_NEXT_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_NEXT(ctx, pc = op->jumppc, JUMP, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(TRY) {
		klr_TRY_t *op = (klr_TRY_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_TRY(ctx, pc = op->jumppc, JUMP, op->hn);
		GOTO_NEXT();
	} 
	CASE(TRYEND) {
		klr_TRYEND_t *op = (klr_TRYEND_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_TRYEND(ctx, op->hn);
		GOTO_NEXT();
	} 
	CASE(THROW) {
		klr_THROW_t *op = (klr_THROW_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_THROW(ctx, op->start);
		GOTO_NEXT();
	} 
	CASE(CATCH) {
		klr_CATCH_t *op = (klr_CATCH_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_CATCH(ctx, pc = op->jumppc, JUMP, op->en, op->msg);
		GOTO_NEXT();
	} 
	CASE(bNOT) {
		klr_bNOT_t *op = (klr_bNOT_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_bNOT(ctx, op->c, op->a);
		GOTO_NEXT();
	} 
	CASE(iINC) {
		klr_iINC_t *op = (klr_iINC_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR3_iINC(ctx, op->a);
		GOTO_NEXT();
	} 
	CASE(iDEC) {
		klr_iDEC_t *op = (klr_iDEC_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR3_iDEC(ctx, op->a);
		GOTO_NEXT();
	} 
	CASE(iNEG) {
		klr_iNEG_t *op = (klr_iNEG_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_iNEG(ctx, op->c, op->a);
		GOTO_NEXT();
	} 
	CASE(iADD) {
		klr_iADD_t *op = (klr_iADD_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_iADD(ctx, op->c, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(iSUB) {
		klr_iSUB_t *op = (klr_iSUB_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_iSUB(ctx, op->c, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(iMUL) {
		klr_iMUL_t *op = (klr_iMUL_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_iMUL(ctx, op->c, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(iDIV) {
		klr_iDIV_t *op = (klr_iDIV_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_iDIV(ctx, op->c, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(iMOD) {
		klr_iMOD_t *op = (klr_iMOD_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_iMOD(ctx, op->c, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(iEQ) {
		klr_iEQ_t *op = (klr_iEQ_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_iEQ(ctx, op->c, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(iNEQ) {
		klr_iNEQ_t *op = (klr_iNEQ_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_iNEQ(ctx, op->c, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(iLT) {
		klr_iLT_t *op = (klr_iLT_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_iLT(ctx, op->c, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(iLTE) {
		klr_iLTE_t *op = (klr_iLTE_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_iLTE(ctx, op->c, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(iGT) {
		klr_iGT_t *op = (klr_iGT_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_iGT(ctx, op->c, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(iGTE) {
		klr_iGTE_t *op = (klr_iGTE_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_iGTE(ctx, op->c, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(iADDn) {
		klr_iADDn_t *op = (klr_iADDn_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_iADDn(ctx, op->c, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(iSUBn) {
		klr_iSUBn_t *op = (klr_iSUBn_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_iSUBn(ctx, op->c, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(iMULn) {
		klr_iMULn_t *op = (klr_iMULn_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_iMULn(ctx, op->c, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(iDIVn) {
		klr_iDIVn_t *op = (klr_iDIVn_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_iDIVn(ctx, op->c, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(iMODn) {
		klr_iMODn_t *op = (klr_iMODn_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_iMODn(ctx, op->c, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(iEQn) {
		klr_iEQn_t *op = (klr_iEQn_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_iEQn(ctx, op->c, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(iNEQn) {
		klr_iNEQn_t *op = (klr_iNEQn_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_iNEQn(ctx, op->c, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(iLTn) {
		klr_iLTn_t *op = (klr_iLTn_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_iLTn(ctx, op->c, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(iLTEn) {
		klr_iLTEn_t *op = (klr_iLTEn_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_iLTEn(ctx, op->c, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(iGTn) {
		klr_iGTn_t *op = (klr_iGTn_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_iGTn(ctx, op->c, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(iGTEn) {
		klr_iGTEn_t *op = (klr_iGTEn_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_iGTEn(ctx, op->c, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(fNEG) {
		klr_fNEG_t *op = (klr_fNEG_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_fNEG(ctx, op->c, op->a);
		GOTO_NEXT();
	} 
	CASE(fADD) {
		klr_fADD_t *op = (klr_fADD_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_fADD(ctx, op->c, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(fSUB) {
		klr_fSUB_t *op = (klr_fSUB_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_fSUB(ctx, op->c, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(fMUL) {
		klr_fMUL_t *op = (klr_fMUL_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_fMUL(ctx, op->c, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(fDIV) {
		klr_fDIV_t *op = (klr_fDIV_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_fDIV(ctx, op->c, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(fEQ) {
		klr_fEQ_t *op = (klr_fEQ_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_fEQ(ctx, op->c, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(fNEQ) {
		klr_fNEQ_t *op = (klr_fNEQ_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_fNEQ(ctx, op->c, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(fLT) {
		klr_fLT_t *op = (klr_fLT_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_fLT(ctx, op->c, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(fLTE) {
		klr_fLTE_t *op = (klr_fLTE_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_fLTE(ctx, op->c, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(fGT) {
		klr_fGT_t *op = (klr_fGT_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_fGT(ctx, op->c, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(fGTE) {
		klr_fGTE_t *op = (klr_fGTE_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_fGTE(ctx, op->c, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(fADDn) {
		klr_fADDn_t *op = (klr_fADDn_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_fADDn(ctx, op->c, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(fSUBn) {
		klr_fSUBn_t *op = (klr_fSUBn_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_fSUBn(ctx, op->c, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(fMULn) {
		klr_fMULn_t *op = (klr_fMULn_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_fMULn(ctx, op->c, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(fDIVn) {
		klr_fDIVn_t *op = (klr_fDIVn_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_fDIVn(ctx, op->c, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(fEQn) {
		klr_fEQn_t *op = (klr_fEQn_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_fEQn(ctx, op->c, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(fNEQn) {
		klr_fNEQn_t *op = (klr_fNEQn_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_fNEQn(ctx, op->c, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(fLTn) {
		klr_fLTn_t *op = (klr_fLTn_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_fLTn(ctx, op->c, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(fLTEn) {
		klr_fLTEn_t *op = (klr_fLTEn_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_fLTEn(ctx, op->c, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(fGTn) {
		klr_fGTn_t *op = (klr_fGTn_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_fGTn(ctx, op->c, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(fGTEn) {
		klr_fGTEn_t *op = (klr_fGTEn_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_fGTEn(ctx, op->c, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(OGETIDX) {
		klr_OGETIDX_t *op = (klr_OGETIDX_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_OGETIDX(ctx, op->c, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(OSETIDX) {
		klr_OSETIDX_t *op = (klr_OSETIDX_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_OSETIDX(ctx, op->c, op->a, op->n, op->v);
		GOTO_NEXT();
	} 
	CASE(OGETIDXn) {
		klr_OGETIDXn_t *op = (klr_OGETIDXn_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_OGETIDXn(ctx, op->c, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(OSETIDXn) {
		klr_OSETIDXn_t *op = (klr_OSETIDXn_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_OSETIDXn(ctx, op->c, op->a, op->n, op->v);
		GOTO_NEXT();
	} 
	CASE(NGETIDX) {
		klr_NGETIDX_t *op = (klr_NGETIDX_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_NGETIDX(ctx, op->c, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(NSETIDX) {
		klr_NSETIDX_t *op = (klr_NSETIDX_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_NSETIDX(ctx, op->c, op->a, op->v, op->n);
		GOTO_NEXT();
	} 
	CASE(NGETIDXn) {
		klr_NGETIDXn_t *op = (klr_NGETIDXn_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_NGETIDXn(ctx, op->c, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(NSETIDXn) {
		klr_NSETIDXn_t *op = (klr_NSETIDXn_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_NSETIDXn(ctx, op->c, op->a, op->v, op->n);
		GOTO_NEXT();
	} 
	CASE(bJNOT) {
		klr_bJNOT_t *op = (klr_bJNOT_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR3_bJNOT(ctx, pc = op->jumppc, JUMP, op->a);
		GOTO_NEXT();
	} 
	CASE(iJEQ) {
		klr_iJEQ_t *op = (klr_iJEQ_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR3_iJEQ(ctx, pc = op->jumppc, JUMP, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(iJNEQ) {
		klr_iJNEQ_t *op = (klr_iJNEQ_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR3_iJNEQ(ctx, pc = op->jumppc, JUMP, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(iJLT) {
		klr_iJLT_t *op = (klr_iJLT_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR3_iJLT(ctx, pc = op->jumppc, JUMP, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(iJLTE) {
		klr_iJLTE_t *op = (klr_iJLTE_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR3_iJLTE(ctx, pc = op->jumppc, JUMP, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(iJGT) {
		klr_iJGT_t *op = (klr_iJGT_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR3_iJGT(ctx, pc = op->jumppc, JUMP, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(iJGTE) {
		klr_iJGTE_t *op = (klr_iJGTE_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR3_iJGTE(ctx, pc = op->jumppc, JUMP, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(iJEQn) {
		klr_iJEQn_t *op = (klr_iJEQn_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR3_iJEQn(ctx, pc = op->jumppc, JUMP, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(iJNEQn) {
		klr_iJNEQn_t *op = (klr_iJNEQn_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR3_iJNEQn(ctx, pc = op->jumppc, JUMP, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(iJLTn) {
		klr_iJLTn_t *op = (klr_iJLTn_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR3_iJLTn(ctx, pc = op->jumppc, JUMP, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(iJLTEn) {
		klr_iJLTEn_t *op = (klr_iJLTEn_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR3_iJLTEn(ctx, pc = op->jumppc, JUMP, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(iJGTn) {
		klr_iJGTn_t *op = (klr_iJGTn_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR3_iJGTn(ctx, pc = op->jumppc, JUMP, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(iJGTEn) {
		klr_iJGTEn_t *op = (klr_iJGTEn_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR3_iJGTEn(ctx, pc = op->jumppc, JUMP, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(fJEQ) {
		klr_fJEQ_t *op = (klr_fJEQ_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR3_fJEQ(ctx, pc = op->jumppc, JUMP, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(fJNEQ) {
		klr_fJNEQ_t *op = (klr_fJNEQ_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR3_fJNEQ(ctx, pc = op->jumppc, JUMP, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(fJLT) {
		klr_fJLT_t *op = (klr_fJLT_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR3_fJLT(ctx, pc = op->jumppc, JUMP, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(fJLTE) {
		klr_fJLTE_t *op = (klr_fJLTE_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR3_fJLTE(ctx, pc = op->jumppc, JUMP, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(fJGT) {
		klr_fJGT_t *op = (klr_fJGT_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR3_fJGT(ctx, pc = op->jumppc, JUMP, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(fJGTE) {
		klr_fJGTE_t *op = (klr_fJGTE_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR3_fJGTE(ctx, pc = op->jumppc, JUMP, op->a, op->b);
		GOTO_NEXT();
	} 
	CASE(fJEQn) {
		klr_fJEQn_t *op = (klr_fJEQn_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR3_fJEQn(ctx, pc = op->jumppc, JUMP, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(fJNEQn) {
		klr_fJNEQn_t *op = (klr_fJNEQn_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR3_fJNEQn(ctx, pc = op->jumppc, JUMP, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(fJLTn) {
		klr_fJLTn_t *op = (klr_fJLTn_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR3_fJLTn(ctx, pc = op->jumppc, JUMP, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(fJLTEn) {
		klr_fJLTEn_t *op = (klr_fJLTEn_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR3_fJLTEn(ctx, pc = op->jumppc, JUMP, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(fJGTn) {
		klr_fJGTn_t *op = (klr_fJGTn_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR3_fJGTn(ctx, pc = op->jumppc, JUMP, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(fJGTEn) {
		klr_fJGTEn_t *op = (klr_fJGTEn_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR3_fJGTEn(ctx, pc = op->jumppc, JUMP, op->a, op->n);
		GOTO_NEXT();
	} 
	CASE(NOP) {
		klr_NOP_t *op = (klr_NOP_t*)pc;; (void)op; VMCOUNT(pc); pc++;
		KLR_NOP(ctx);
		GOTO_NEXT();
	} 
	DISPATCH_END(pc);
	L_RETURN:;
	return pc;
}


#ifdef __cplusplus
}
#endif
