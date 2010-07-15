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
 * (1) GNU Lesser General Public License 3.0 (with K_UNDER_LGPL)
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

#include "konoha_t.h"

#ifndef KONOHA_VM_H_
#define KONOHA_VM_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ======================================================================== */
/* KCODE */
/* ======================================================================== */

#ifndef KLRAPI
#define KLRAPI(T)         T  CC_FASTCALL
#endif

#define SFi(x)   sfp[x].ivalue
#define SFf(x)   sfp[x].fvalue
#define SFb(x)   sfp[x].bvalue
#define	REGb(x)  sfp[x].bvalue

#define KNH_THROW__T(ctx, s) \
	knh_stack_throw(ctx, ctx->esp, new_Exception__T(ctx, s)); \

#define KNH_THROW_iZERODIV(n)  \
	if(unlikely(n == 0)) { \
		KNH_THROW__T(ctx, "Arithmetic!!: Zero Divided"); \
	}\

#define KNH_THROW_fZERODIV(n)  \
	if(unlikely(n == K_FLOAT_ZERO)) { \
		KNH_THROW__T(ctx, "Arithmetic!!: Zero Divided"); \
	}\

int knh_Method_pcline(knh_Method_t *mtd, knh_opline_t *pc);
#define _HERE_    knh_Method_file(ctx, sfp[K_MTDIDX].callmtd), knh_Method_pcline(sfp[K_MTDIDX].callmtd, pc-1)

#define klr_throw__T(ctx, s) \
	knh_stack_throw(ctx, ctx->esp, new_Exception__T(ctx, s)); \

#define klr_throw__iZERODIV(n)  \
	if(unlikely(n == 0)) { \
		klr_throw__T(ctx, "Arithmetic!!: Zero Divided"); \
	}\

#define klr_throw__fZERODIV(n)  \
	if(unlikely(n == K_FLOAT_ZERO)) { \
		klr_throw__T(ctx, "Arithmetic!!: Zero Divided"); \
	}\

/* [HALT] */
// knh_stack_pushesp(ctx, sfp) is meaningless but necessary for generating
// higly-optimized assemble virtual machince code. by ide and kimio

#define KLR0_HALT(ctx) {\
		knh_intptr_t thisidx_ = knh_stack_pushesp(ctx, sfp);\
		pc--;\
		KNH_SYSLOG(ctx, LOG_CRIT, "HaltVirtualMachine", "file=%s, line=%d thisidx=%d", \
			knh_Method_file(ctx, sfp[K_MTDIDX].callmtd), pc->line, (int)thisidx_);\
		goto L_RETURN;\
	}\

/* [MOV, SET] */

#define klr_mov(ctx, v1, v2) {\
		Object *v2_ = (Object*)v2;\
		knh_Object_RCinc(v2_);\
		knh_Object_RCdec(v1);\
		if(knh_Object_isRC0(v1)) {\
			knh_Object_free(ctx, v1);\
		}\
		v1 = v2_;\
	}\

/* DEL
 #define klr_qmov(ctx, v1, v2) {\
		Object *v2_ = (Object*)v2;\
		knh_Object_RCinc(v2_); \
		knh_Object_RCdec(v1); \
		v1 = v2_; \
	}\
*/

#define KLR_OSET(ctx, a, v) {\
		knh_Object_t *v_ = v;\
		klr_mov(ctx, sfp[a].o, v_);\
	}\

#define KLR_NSET(ctx, a, n) {\
		sfp[a].data = n;\
	}\

#define KLR_MOV(ctx, a, b) { \
		knh_Object_RCinc(sfp[b].o);\
		knh_Object_RCdec(sfp[a].o);\
		if(knh_Object_isRC0(sfp[a].o)) {\
			knh_Object_free(ctx, sfp[a].o);\
		} \
		sfp[a] = sfp[b];\
	}\

#define KLR_OMOV(ctx, a, b) { \
		klr_mov(ctx, sfp[a].o, sfp[b].o);\
	}\

#define KLR_NMOV(ctx, a, b) {\
		sfp[a].data = sfp[b].data;\
	}\

#define KLR_ONMOV(ctx, a, b, c, d) {\
		klr_mov(ctx, sfp[a].o, sfp[b].o);\
		sfp[c].data = sfp[d].data;\
	}\

#define KLR2_OOMOV(ctx, a, b, c, d) {\
		klr_mov(ctx, sfp[a].o, sfp[b].o);\
		klr_mov(ctx, sfp[c].o, sfp[d].o);\
	}\

#define KLR2_NNMOV(ctx, a, b, c, d) {\
		sfp[a].data = sfp[b].data;\
		sfp[c].data = sfp[d].data;\
	}\

#define KLR_SWAP(ctx, a, b) {\
		knh_sfp_t temp = sfp[(a)];\
		sfp[(a)] = sfp[(b)];\
		sfp[(b)] = temp;\
	}\

#define KLR_BOX(ctx, a, b, cid) {\
		klr_mov(ctx, sfp[a].o new_Object_boxing(ctx, sfp+b, cid));\
	}\

#define KLR_UNBOX(ctx, a, b) {\
		sfp[a].data = knh_Object_data(sfp[a].o);\
	}\

#define SFXo(x)   (sfp[x.i].ox)->fields[x.n]
#define SFXi(x)   (*((knh_int_t*)(&(sfp[x.i].ox)->fields[x.n])))
#define SFXf(x)   (*((knh_float_t*)(&(sfp[x.i].ox)->fields[x.n])))
#define SFXb(x)   (*((knh_bool_t*)(&(sfp[x.i].ox)->fields[x.n])))

#define KLR_OMOVx(ctx, a, b) {\
		knh_Object_t *v_ = SFXo(b);\
		klr_mov(ctx, sfp[a].o, v_);\
	}\

#define KLR_iMOVx(ctx, a, b)    sfp[a].ivalue = SFXi(b)
#define KLR_fMOVx(ctx, a, b)    sfp[a].fvalue = SFXf(b)
#define KLR_bMOVx(ctx, a, b)    sfp[a].bvalue = SFXb(b)

#define KLR_XMOV(ctx, a, b)    klr_mov(ctx, SFXo(a), sfp[b].o)
#define KLR_XMOVx(ctx, a, b)    klr_mov(ctx, SFXo(a), SFXo(b))
#define KLR_XOSET(ctx, a, b)    klr_mov(ctx, SFXo(a), b)

#define KLR_XIMOV(ctx, a, b)   {\
		SFXi(a) = sfp[b].ivalue;\
	}\

#define KLR_XFMOV(ctx, a, b)   SFXf(a) = sfp[b].fvalue
#define KLR_XBMOV(ctx, a, b)   SFXb(a) = sfp[b].bvalue

/* ------------------------------------------------------------------------ */

#define ENVo(x)   (sfp[0].cc)->envsfp[x].o
#define ENVn(x)   (sfp[0].cc)->envsfp[x].data

#define KLR_MOVe(ctx, a, b) {\
		klr_mov(ctx, sfp[a].o, ENVo(b));\
		sfp[a].data = ENVn(b);\
	}\

/* ------------------------------------------------------------------------ */
/* [CALL] */

#define K_CALLDELTA   3
#define K_SHIFTIDX  (-3)
#define K_PCIDX     (-2)
#define K_MTDIDX    (-1)
#define K_RTNIDX    (-3)
#define K_TRLIDX    0
#define K_SELFIDX   0

#define klr_setesp(ctx, newesp)  ((knh_Context_t*)ctx)->esp = (newesp);
#define klr_setcallmtd(ctx, sfpA, mtdO)   sfpA.callmtd = mtdO

#define KLR_CHKSTACK(ctx, n) \
		if(unlikely(sfp + n > ctx->stacktop)) {\
			sfp = knh_stack_initexpand(ctx, sfp, n);\
		}\

#define klr_ncall(ctx, Fcall, lsfp, rix) { \
		Fcall(ctx, lsfp, rix);\
	}\

#define klr_vcall(ctx, mtd, thisidx) { \
		vshift = thisidx;\
		vpc = pc;\
		pc = (mtd)->pc_start;\
		sfp = sfp + vshift;\
		if(unlikely(sfp > ctx->stacktop)) {\
			sfp = knh_stack_initexpand(ctx, sfp, 0);\
		}\
		sfp[K_SHIFTIDX].shift = vshift;\
		sfp[K_PCIDX].pc = vpc;\
		GOTO_PC(pc); \
	}

#define klr_vcall2(ctx, mtd, thisidx) { \
		vshift = thisidx;\
		vpc = pc;\
		pc = (mtd)->pc_start;\
		sfp = sfp + vshift;\
		sfp[K_SHIFTIDX].shift = vshift;\
		sfp[K_PCIDX].pc = vpc;\
		GOTO_PC(pc); \
	}

#define KLR_RET(ctx) {\
		sfp = sfp - vshift; \
		pc = vpc; \
		vshift = sfp[K_SHIFTIDX].shift;\
		vpc = sfp[K_PCIDX].pc;\
		GOTO_PC(pc);\
	}\

#define KLR0_VEXEC(ctx) {\
		vpc = pc;\
		pc = (sfp[K_MTDIDX].callmtd)->pc_start;\
		sfp[K_SHIFTIDX].shift = 0;\
		sfp[K_PCIDX].pc = vpc;\
		GOTO_PC(pc); \
	}\

#define KLR0_VEXEC_OLD(ctx) {\
		knh_intptr_t thisidx_ = knh_stack_pushesp(ctx, sfp);\
		klr_vcall2(ctx, sfp[K_MTDIDX].callmtd, thisidx_);\
	}\

#define KLR2_JMP_(ctx, PC, JUMP)   KLR_RET(ctx)

knh_opline_t* knh_VirtualMachine_run(Ctx *, knh_sfp_t *, knh_opline_t *);
//METHOD knh_Fmethod_runVM(Ctx *ctx, knh_sfp_t *sfp, long rix);
#define knh_Method_isKLRCode(mtd) ((mtd)->fcall_1 == knh_Fmethod_runVM)

#define knh_Gamma_hasJIT(ctx)     0
#define CODE_VEXEC     (ctx->share->PC_VEXEC)
#define CODE_LAUNCH    (ctx->share->PC_LAUNCH)
#define CODE_FUNCCALL  (ctx->share->PC_FUNCCALL)
//#define CODE_ABSTRACT  (ctx->share->PC_ABSTRACT)

#define klr_hcall(ctx, mtd, thisidx, rix) { \
		if(knh_Method_isKLRCode(mtd)) {\
			klr_vcall(ctx, mtd, thisidx);\
		}else {\
			klr_ncall(ctx, (mtd)->fcall_1, sfp + thisidx, rix);\
		}\
	}\

#define KNH_SCALL(ctx, lsfp, rtnidx, mtdO, argc) { \
		knh_intptr_t thisidx_ = rtnidx+ K_CALLDELTA;\
		DBG_ASSERT(IS_Method(mtdO));\
		klr_setcallmtd(ctx, lsfp[thisidx_+K_MTDIDX], mtdO);\
		klr_setesp(ctx, lsfp + thisidx_ + argc + 1);\
		if(knh_Method_isKLRCode(mtdO)) {\
			knh_VirtualMachine_run(ctx, lsfp + thisidx_, CODE_VEXEC);\
		}else {\
			(mtdO)->fcall_1(ctx, lsfp + thisidx_, K_RTNIDX);\
		}\
		klr_setesp(ctx, lsfp + rtnidx);\
	} \

#define KNH_SELFCALL(ctx, sfp, mtd, rix) { \
		(mtd)->fcall_1(ctx, sfp, rix);\
	} \

typedef KLRAPI(knh_Method_t*) (*klr_Fmethod)(Ctx *, knh_sfp_t *, int, knh_Method_t*);

#define KLR_LDMETHOD(ctx, fmtd, thisidx, mtdO) { \
		knh_Method_t *mtd_ = fmtd(ctx, sfp, thisidx, mtdO);\
		if(mtd_ != mtdO) { \
			((klr_LDMETHOD_t*)op)->callmtd = mtd_;\
		}\
		klr_setcallmtd(ctx, sfp[thisidx+K_MTDIDX], mtd_);\
	} \

#define KLR_CALL(ctx, rtnidx, espshift) { \
		knh_intptr_t thisidx_ = rtnidx + K_CALLDELTA;\
		knh_intptr_t mtdidx_ = thisidx_ + K_MTDIDX;\
		klr_setesp(ctx, sfp + espshift);\
		klr_hcall(ctx, sfp[mtdidx_].callmtd, thisidx_, K_RTNIDX);\
	} \

#define KLR2_SCALL(ctx, rtnidx, espshift, mtdO) { \
		knh_Method_t *mtd_ = mtdO;\
		knh_intptr_t thisidx_ = rtnidx + K_CALLDELTA;\
		knh_intptr_t mtdidx_ = thisidx_ + K_MTDIDX;\
		klr_setcallmtd(ctx, sfp[mtdidx_], mtd_);\
		klr_setesp(ctx, sfp + espshift);\
		klr_ncall(ctx, (mtd_)->fcall_1, sfp + thisidx_, K_RTNIDX); \
	} \

#define KLR3_FASTCALL(ctx, fcall, c, a) { \
		fcall(ctx, sfp + a, c - a);\
	} \

#define KLR3_FASTCALLn(ctx, fcall, c, a, espshift, data) { \
		klr_setesp(ctx, sfp + espshift);\
		sfp[espshift-1].data = data;\
		fcall(ctx, sfp + a, c - a);\
	} \

#define KLR3_FASTCALLo(ctx, fcall, c, a, espshift, obj) { \
		klr_setesp(ctx, sfp + espshift);\
		klr_mov(ctx, sfp[espshift-1].o, obj);\
		fcall(ctx, sfp + a, c - a);\
	} \

#define KLR2_VCALL(ctx, rtnidx, espshift, mtdO) { \
		knh_Method_t *mtd_ = mtdO;\
		knh_intptr_t thisidx_ = rtnidx + K_CALLDELTA;\
		knh_intptr_t mtdidx_ = thisidx_ + K_MTDIDX;\
		klr_setcallmtd(ctx, sfp[mtdidx_], mtd_);\
		klr_setesp(ctx, sfp + espshift);\
		klr_vcall(ctx, mtd_, thisidx_); \
	} \

#define KLR2_VCALL_(ctx, rtnidx, espshift, mtdO) { \
		knh_Method_t *mtd_ = mtdO;\
		knh_intptr_t thisidx_ = rtnidx + K_CALLDELTA;\
		knh_intptr_t mtdidx_ = thisidx_ + K_MTDIDX;\
		klr_setcallmtd(ctx, sfp[mtdidx_], mtd_);\
		klr_setesp(ctx, sfp + espshift);\
		klr_vcall2(ctx, mtd_, thisidx_);\
	} \

#define KLR0_FUNCCALL(ctx) { \
		knh_opline_t *pc2 = sfp[K_PCIDX].pc;\
		klr_ncall(ctx, (sfp[K_MTDIDX].callmtd)->fcall_1, 0, K_RTNIDX);\
		pc = pc2; \
		GOTO_PC(pc);\
	} \

#define KLR0_YEILD(ctx, espidx) {\
		klr_setesp(ctx, sfp+espidx);\
		goto L_RETURN;\
	}\

#define KLR0_ENTER(ctx) {\
		knh_intptr_t thisidx_ = knh_stack_pushesp(ctx, sfp);\
		DBG_ASSERT(sfp[thisidx_ + K_MTDIDX].callmtd == sfp[K_MTDIDX].callmtd);\
		klr_hcall(ctx, sfp[K_MTDIDX].callmtd, thisidx_, K_RTNIDX);\
	}\

#define KLR0_EXIT(ctx) {\
		pc = NULL; goto L_RETURN;\
	}\

#define KLR0_THCODE(ctx) { \
		knh_code_thread(ctx, pc-1, OPJUMP); \
		goto L_RETURN; \
	}\

#define KLR0_LABEL(ctx, id, msg)

/* ------------------------------------------------------------------------- */

#define klr_setcasttrl(ctx, sfpA, trlO)   sfpA.casttrl = trlO

#define KLR_iCAST(ctx, c, a) {\
		sfp[c].ivalue = (knh_int_t)SFf(a); \
	}\

#define KLR_fCAST(ctx, c, a) {\
		sfp[c].fvalue = (knh_float_t)SFi(a); \
	}\

#define KLR_SCAST(ctx, rtnidx, trlidx, trl)  { \
		knh_Translator_t *trl_ = trl;\
		klr_setcasttrl(ctx, sfp[trlidx], trl_);\
		(trl_)->ftcast_1(ctx, sfp+trlidx, trlidx - rtnidx); \
	} \

#define KNH_SCAST(ctx, lsfp, rtnidx, trl)  { \
		knh_Translator_t *trl_ = trl;\
		klr_setcasttrl(ctx, lsfp[rtnidx], trl_);\
		(trl_)->ftcast_1(ctx, lsfp + rtnidx, 0); \
	} \

#define KLR_TCAST(ctx, rtnidx, trlidx, trl)  { \
		knh_Translator_t *trl_ = trl; \
		knh_class_t scid = SP(trl_)->scid, this_cid = knh_Object_cid(sfp[trlidx].o);\
		if(this_cid != scid) {\
			trl_ = knh_findTranslator(ctx, scid, SP(trl)->tcid);\
			KNH_SETv(ctx, ((klr_TCAST_t*)op)->cast, trl_);\
		}\
		klr_setcasttrl(ctx, sfp[trlidx], trl_);\
		(trl_)->ftcast_1(ctx, sfp+trlidx, trlidx - rtnidx);\
	} \

#define KLR_ACAST(ctx, rtnidx, trlidx, trl)  { \
		knh_Translator_t *trl_ = trl; \
		knh_class_t tcid = SP(trl_)->tcid, this_cid = knh_Object_cid(sfp[trlidx].o);\
		if(!tcid != this_cid && !knh_class_instanceof(ctx, this_cid, tcid)) {\
			knh_class_t scid = SP(trl_)->scid;\
			if(this_cid != scid) {\
				trl_ = knh_findTranslator(ctx, scid, tcid);\
				KNH_SETv(ctx, ((klr_ACAST_t*)op)->cast, trl_);\
			}\
			klr_setcasttrl(ctx, sfp[trlidx], trl_);\
			(trl_)->ftcast_1(ctx, sfp+trlidx, trlidx - rtnidx); \
		}\
	} \

typedef KLRAPI(void) (*klr_Ftr)(Ctx *, knh_sfp_t *, knh_sfpidx_t, knh_class_t);

#define KLR_TR(Ctx, c, a, cid, f) { \
		f(ctx, sfp + a, c - a, (knh_class_t)cid);\
	}\

/* ======================================================================== */

#define KLR_JMP(ctx, PC, JUMP) {\
		PC; \
		goto JUMP; \
	}\

#define KLR_ONCE(ctx, PC, JUMP) { \
		((klr_ONCE_t*)op)->opcode = OPCODE_JMP;\
	}\

#define KLR_NUL(ctx, c, a)  REGb(c) = IS_NULL(sfp[a].o)
#define KLR_iNUL(ctx, c, a) REGb(c) = (IS_NULL(sfp[a].o) && sfp[a].ivalue == 0)
#define KLR_fNUL(ctx, c, a) REGb(c) = (IS_NULL(sfp[a].o) && sfp[a].ivalue == 0)

#define KLR_JMPF(ctx, PC, JUMP, n) \
	if(REGb(n)) {\
	}else{ \
		KLR_JMP(ctx, PC, JUMP); \
	} \

typedef KLRAPI(int) (*klr_Fchk)(Ctx *, knh_sfp_t *, int n);

#define KLR_DYJMP(ctx, PC, JUMP, n, fcheck) \
	if(fcheck(ctx, sfp, n)) { \
		KLR_JMP(ctx, PC, JUMP); \
	} \

/* ------------------------------------------------------------------------- */

#define KLR_NEXT(ctx, PC, JUMP, na, ib) { \
		knh_sfp_t *itrsfp_ = sfp + ib; \
		DBG_ASSERT(IS_bIterator(itrsfp_[0].it));\
		if(!((itrsfp_[0].it)->fnext_1(ctx, itrsfp_, na - ib))) { \
			KLR_JMP(ctx, PC, JUMP); \
		} \
	} \

typedef KLRAPI(int) (*klr_Fnext)(Ctx *, knh_sfp_t *, int, knh_class_t);

#define KLR_NEXTf(ctx, PC, JUMP, fnext, reqc, na, ib) { \
		knh_sfp_t *itrsfp_ = sfp + ib; \
		if(!fnext(ctx, itrsfp_, na - ib, reqc)) { \
			KLR_JMP(ctx, PC, JUMP); \
		} \
	} \

/* ------------------------------------------------------------------------- */

#define NPC  /* for KNH_TRY */

#define KLR_THROW(ctx, start) { \
		if(IS_Exception(ctx->e)) {\
			knh_ExceptionHandler_t* _hdr = knh_stack_findExceptionHandler(ctx, sfp, start); \
			pc = SP(_hdr)->pc; \
			sfp = ctx->stack + SP(_hdr)->sfpidx;\
			klr_setesp(ctx, (ctx->stack + SP(_hdr)->espidx));\
			vpc = SP(_hdr)->pc; \
			vshift = SP(_hdr)->vshift; \
			GOTO_PC(pc);\
		}\
	} \

#define KNH_SETJUMP(hdlr) knh_setjmp(DP(hdlr)->jmpbuf)

#define KLR_TRY(ctx, PC, JUMP, hn)  {\
		knh_ExceptionHandler_t* _hdr = sfp[hn].hdr; \
		if(!IS_ExceptionHandler(_hdr)) { \
			_hdr = new_(ExceptionHandler); \
			klr_mov(ctx, sfp[hn].o, _hdr); \
		} \
		SP(_hdr)->pc  = pc; \
		SP(_hdr)->sfpidx = (sfp - ctx->stack); \
		SP(_hdr)->espidx = (ctx->esp - ctx->stack); \
		SP(_hdr)->vshift = vshift; \
		SP(_hdr)->vpc = vpc; \
	} \

#define KLR_CATCH(ctx, PC, JUMP, en, emsg) { \
		if(knh_Exception_isa(ctx, ctx->e, emsg)) { \
			DBG_P("CATHING .."); \
			klr_mov(ctx, sfp[en].o, ctx->e); \
			KNH_SETv(ctx, ((knh_Context_t*)ctx)->e, KNH_NULL);\
		} \
		else { \
			KLR_JMP(ctx, PC, JUMP); \
		} \
	} \

/* ------------------------------------------------------------------------ */

struct klr_P_t;
typedef KLRAPI(void) (*klr_Fprint)(Ctx *, knh_sfp_t*, struct klr_P_t*);
#define KLR_P(ctx, fprint, flag, msg, fmt, n) fprint(ctx, sfp, op)

typedef KLRAPI(void) (*klr_Fprobe)(Ctx *, knh_sfp_t*, knh_sfpidx_t n, knh_opline_t *pc);

#define KLR0_PROBE(ctx, fprobe, n) { \
		fprobe(ctx, sfp, n, pc-1);\
	}\

/* ------------------------------------------------------------------------ */

#define KLR_bNOT(ctx, c, a)     REGb(c) = !(REGb(a))

#define KLR3_iINC(ctx, a)       SFi(a)++
#define KLR3_iDEC(ctx, a)       SFi(a)--

#define KLR_iNEG(ctx, c, a)     SFi(c) = -(SFi(a))
#define KLR_iADD(ctx, c, a, b)  SFi(c) = (SFi(a) + SFi(b))
#define KLR_iADDn(ctx, c, a, n) SFi(c) = (SFi(a) + n)
#define KLR_iSUB(ctx, c, a, b)  SFi(c) = (SFi(a) - SFi(b))
#define KLR_iSUBn(ctx, c, a, n) SFi(c) = (SFi(a) - n)
#define KLR_iMUL(ctx, c, a, b)  SFi(c) = (SFi(a) * SFi(b))
#define KLR_iMULn(ctx, c, a, n) SFi(c) = (SFi(a) * n)
#define KLR_iDIV(ctx, c, a, b)  { \
		klr_throw__iZERODIV(SFi(b)); \
		SFi(c) = (SFi(a) / SFi(b)); \
	} \

#define KLR_iDIVn(ctx, c, a, n)  SFi(c) = (SFi(a) / n)
#define KLR_iMOD(ctx, c, a, b)  { \
		klr_throw__iZERODIV(SFi(b)); \
		SFi(c) = (SFi(a) % SFi(b)); \
	} \

#define KLR_iMODn(ctx, c, a, n)  SFi(c) = (SFi(a) % n)

#define KLR_iEQ(ctx, c, a, b)  REGb(c) = (SFi(a) == SFi(b));
#define KLR_iEQn(ctx, c, a, n)  REGb(c) = (SFi(a) == n);
#define KLR_iNEQ(ctx, c, a, b)  REGb(c) = (SFi(a) != SFi(b));
#define KLR_iNEQn(ctx, c, a, n)  REGb(c) = (SFi(a) != n);
#define KLR_iLT(ctx, c, a, b)  REGb(c) = (SFi(a) < SFi(b));
#define KLR_iLTn(ctx, c, a, n)  REGb(c) = (SFi(a) < n);
#define KLR_iLTE(ctx, c, a, b)  REGb(c) = (SFi(a) <= SFi(b));
#define KLR_iLTEn(ctx, c, a, n)  REGb(c) = (SFi(a) <= n);
#define KLR_iGT(ctx, c, a, b)  REGb(c) = (SFi(a) > SFi(b));
#define KLR_iGTn(ctx, c, a, n)  REGb(c) = (SFi(a) > n);
#define KLR_iGTE(ctx, c, a, b)  REGb(c) = (SFi(a) >= SFi(b));
#define KLR_iGTEn(ctx, c, a, n)  REGb(c) = (SFi(a) >= n);

#define BR_(EXPR, PC, JUMP) if(EXPR) {} else {KLR_JMP(ctx, PC, JUMP); }

#define KLR3_bJNOT(ctx, PC, JUMP, a)     BR_(!REGb(a), PC, JUMP)
#define KLR3_iJEQ(ctx, PC, JUMP, a, b)   BR_((SFi(a) == SFi(b)), PC, JUMP)
#define KLR3_iJEQn(ctx, PC, JUMP, a, n)  BR_((SFi(a) == n), PC, JUMP)
#define KLR3_iJNEQ(ctx, PC, JUMP, a, b)  BR_((SFi(a) != SFi(b)), PC, JUMP)
#define KLR3_iJNEQn(ctx, PC, JUMP, a, n) BR_((SFi(a) != n), PC, JUMP)
#define KLR3_iJLT(ctx, PC, JUMP, a, b)   BR_((SFi(a) < SFi(b)), PC, JUMP)
#define KLR3_iJLTn(ctx, PC, JUMP, a, n)  BR_((SFi(a) < n), PC, JUMP)
#define KLR3_iJLTE(ctx, PC, JUMP, a, b)  BR_((SFi(a) <= SFi(b)), PC, JUMP)
#define KLR3_iJLTEn(ctx, PC, JUMP, a, n) BR_((SFi(a) <= n), PC, JUMP)
#define KLR3_iJGT(ctx, PC, JUMP, a, b)   BR_((SFi(a) > SFi(b)), PC, JUMP)
#define KLR3_iJGTn(ctx, PC, JUMP, a, n)  BR_((SFi(a) > n), PC, JUMP)
#define KLR3_iJGTE(ctx, PC, JUMP, a, b)  BR_((SFi(a) >= SFi(b)), PC, JUMP)
#define KLR3_iJGTEn(ctx, PC, JUMP, a, n) BR_((SFi(a) >= n), PC, JUMP)

/* ------------------------------------------------------------------------ */

#define KLR_fNEG(ctx, c, a)     SFf(c) = -(SFf(a))
#define KLR_fADD(ctx, c, a, b)  SFf(c) = (SFf(a) + SFf(b))
#define KLR_fADDn(ctx, c, a, n) SFf(c) = (SFf(a) + n)
#define KLR_fSUB(ctx, c, a, b)  SFf(c) = (SFf(a) - SFf(b))
#define KLR_fSUBn(ctx, c, a, n) SFf(c) = (SFf(a) - n)
#define KLR_fMUL(ctx, c, a, b)  SFf(c) = (SFf(a) * SFf(b))
#define KLR_fMULn(ctx, c, a, n) SFf(c) = (SFf(a) * n)
#define KLR_fDIV(ctx, c, a, b)  { \
		klr_throw__fZERODIV(SFf(b)); \
		SFf(c) = (SFf(a) / SFf(b)); \
	} \

#define KLR_fDIVn(ctx, c, a, n)  SFf(c) = (SFf(a) / n)

#define KLR_fEQ(ctx, c, a, b) REGb(c) = (SFf(a) == SFf(b))
#define KLR_fEQn(ctx, c, a, n) REGb(c) = (SFf(a) == n)
#define KLR_fNEQ(ctx, c, a, b)  REGb(c) = (SFf(a) != SFf(b))
#define KLR_fNEQn(ctx, c, a, n)  REGb(c) = (SFf(a) != n)
#define KLR_fLT(ctx, c, a, b)  REGb(c) = (SFf(a) < SFf(b))
#define KLR_fLTn(ctx, c, a, n)  REGb(c) = (SFf(a) < n)
#define KLR_fLTE(ctx, c, a, b)  REGb(c) = (SFf(a) <= SFf(b))
#define KLR_fLTEn(ctx, c, a, n) REGb(c) = (SFf(a) <= n)
#define KLR_fGT(ctx, c, a, b)  REGb(c) = (SFf(a) > SFf(b))
#define KLR_fGTn(ctx, c, a, n)  REGb(c) = (SFf(a) > n)
#define KLR_fGTE(ctx, c, a, b)  REGb(c) = (SFf(a) >= SFf(b))
#define KLR_fGTEn(ctx, c, a, n)  REGb(c) = (SFf(a) >= n)

#define KLR3_fJEQ(ctx, PC, JUMP, a, b)   BR_((SFf(a) == SFf(b)), PC, JUMP)
#define KLR3_fJEQn(ctx, PC, JUMP, a, n)  BR_((SFf(a) == n), PC, JUMP)
#define KLR3_fJNEQ(ctx, PC, JUMP, a, b)  BR_((SFf(a) != SFf(b)), PC, JUMP)
#define KLR3_fJNEQn(ctx, PC, JUMP, a, n) BR_((SFf(a) != n), PC, JUMP)
#define KLR3_fJLT(ctx, PC, JUMP, a, b)   BR_((SFf(a) < SFf(b)), PC, JUMP)
#define KLR3_fJLTn(ctx, PC, JUMP, a, n)  BR_((SFf(a) < n), PC, JUMP)
#define KLR3_fJLTE(ctx, PC, JUMP, a, b)  BR_((SFf(a) <= SFf(b)), PC, JUMP)
#define KLR3_fJLTEn(ctx, PC, JUMP, a, n) BR_((SFf(a) <= n), PC, JUMP)
#define KLR3_fJGT(ctx, PC, JUMP, a, b)   BR_((SFf(a) > SFf(b)), PC, JUMP)
#define KLR3_fJGTn(ctx, PC, JUMP, a, n)  BR_((SFf(a) > n), PC, JUMP)
#define KLR3_fJGTE(ctx, PC, JUMP, a, b)  BR_((SFf(a) >= SFf(b)), PC, JUMP)
#define KLR3_fJGTEn(ctx, PC, JUMP, a, n) BR_((SFf(a) >= n), PC, JUMP)

/* ------------------------------------------------------------------------ */

#define klr_array_index(ctx, n, size)   (size_t)n
#define klr_array_check(n, size) \
	if(unlikely(n >= size)) knh_throw_OutOfIndex(ctx, n, size, NULL, 0)

#define KLR_OGETIDXn(ctx, cidx, aidx, N) {\
		knh_Array_t *a_ = sfp[aidx].a;\
		size_t n_ = klr_array_index(ctx, N, knh_Array_size(a_));\
		klr_array_check(n_, knh_Array_size(a_));\
		Object *v_ = (a_)->list[n_];\
		klr_mov(ctx, sfp[cidx].o, v_);\
	}\

#define KLR_OGETIDX(ctx, cidx, aidx, nidx) KLR_OGETIDXn(ctx, cidx, aidx, sfp[nidx].ivalue)

#define KLR_OSETIDXn(ctx, cidx, aidx, N, vidx) {\
		knh_Array_t *a_ = sfp[aidx].a;\
		size_t n_ = klr_array_index(ctx, N, knh_Array_size(a_));\
		klr_array_check(n_, knh_Array_size(a_));\
		klr_mov(ctx, (a_)->list[n_], sfp[vidx].o);\
		klr_mov(ctx, sfp[cidx].o, sfp[vidx].o);\
	}\

#define KLR_OSETIDX(ctx, cidx, aidx, nidx, vidx) KLR_OSETIDXn(ctx, cidx, aidx, sfp[nidx].ivalue, vidx)

#define KLR_NGETIDXn(ctx, cidx, aidx, N) {\
		knh_Array_t *a_ = sfp[aidx].a;\
		size_t n_ = klr_array_index(ctx, N, knh_Array_size(a_));\
		klr_array_check(n_, knh_Array_size(a_));\
		sfp[cidx].data = (a_)->nlist[n_];\
	}\

#define KLR_NGETIDX(ctx, cidx, aidx, nidx) KLR_NGETIDXn(ctx, cidx, aidx, sfp[nidx].ivalue)

#define KLR_NSETIDXn(ctx, cidx, aidx, N, vidx) {\
		knh_Array_t *a_ = sfp[aidx].a;\
		size_t n_ = klr_array_index(ctx, N, knh_Array_size(a_));\
		klr_array_check(n_, knh_Array_size(a_));\
		sfp[cidx].data = (a_)->nlist[n_] = sfp[vidx].data;\
	}\

#define KLR_NSETIDX(ctx, cidx, aidx, nidx, vidx) KLR_NSETIDXn(ctx, cidx, aidx, sfp[nidx].ivalue, vidx)

/* ------------------------------------------------------------------------ */

#define KLR_LABEL(ctx, id, msg) DBG_P("LABEL: id=%d", (int)id)
#ifndef KONOHA_ON_WINDOWS
#define KLR_NOP(ctx)  asm("nop")
#else
#define KLR_NOP(ctx)
#endif
/* ------------------------------------------------------------------------ */

#define BEGIN_LOCAL(ctx, lsfp, n) \
		knh_sfp_t *lsfp = knh_stack_local(ctx, n);\
		int sfpidx_ = lsfp - ctx->stack;\

#define UPDATE_LOCAL(ctx, lsfp)   lsfp = ctx->stack + sfpidx_;
#define END_LOCAL(ctx, lsfp)      ((knh_Context_t*)ctx)->esp = ctx->stack + sfpidx_;

#define LOCAL_NEW(ctx, lsfp, n, T, V, O) \
	T V = O;\
	KNH_SETv(ctx, lsfp[n].o, V);\

/* ------------------------------------------------------------------------ */

typedef KLRAPI(void) (*klr_Foeval)(Ctx *ctx, knh_sfp_t *, int, int);

#define KLR_OEVAL(ctx, foeval, c, a)  {\
		foeval(ctx, sfp, c, a);\
	}\

typedef KLRAPI(void) (*klr_Fgetidx)(Ctx *, knh_sfp_t *, int, int, size_t);
typedef KLRAPI(void) (*klr_Fsetidx)(Ctx *, knh_sfp_t *, int, int, int, size_t);

#define KLR_GETIDX(ctx, fgetidx, b, a, n)  {\
		fgetidx(ctx, sfp, b, a, (size_t)sfp[n].ivalue);\
	}\

#define KLR_GETIDXn(ctx, fgetidx, b, a, n)  {\
		fgetidx(ctx, sfp, b, a, (size_t)n);\
	}\

#define KLR_SETIDX(ctx, fsetidx, c, a, v, n)  {\
		fsetidx(ctx, sfp, c, a, v, (size_t)sfp[n].ivalue);\
	}\

#define KLR_SETIDXn(ctx, fsetidx, c, a, v, n)  {\
		fsetidx(ctx, sfp, c, a, v, (size_t)n);\
	}\

/* ------------------------------------------------------------------------ */

#define KNH_SECURE(ctx, sfp) knh_stack_checkSecurityManager(ctx, sfp)

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif /*KONOHA_VM_H_*/
