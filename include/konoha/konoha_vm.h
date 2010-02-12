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
 * (1) GNU Lesser General Public License 3.0 (with KONOHA_UNDER_LGPL3)
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
#define KLRAPI(T)         T  KNH_CC_FASTCALL
#endif

#define SFb(x)   sfp[x].bvalue
#define SFi(x)   sfp[x].ivalue
#define SFf(x)   sfp[x].fvalue

#define KNH_THROW__T(ctx, s) \
	DBG2_P(s); \
	knh_stack_throw(ctx, ctx->esp, new_Exception__T(ctx, s), NULL, 0); \

#define KNH_THROW_iZERODIV(n)  \
	if(unlikely(n == 0)) { \
		KNH_THROW__T(ctx, "Arithmetic!!: Zero Divided"); \
	}\

#define KNH_THROW_fZERODIV(n)  \
	if(unlikely(n == KNH_FLOAT_ZERO)) { \
		KNH_THROW__T(ctx, "Arithmetic!!: Zero Divided"); \
	}\

int knh_Method_pcline(knh_Method_t *mtd, knh_code_t *pc);
#define _HERE_    knh_Method_file(ctx, sfp[-1].mtd), knh_Method_pcline(sfp[-1].mtd, pc)

#define klr_throw__T(ctx, s) \
	knh_stack_throw(ctx, ctx->esp, new_Exception__T(ctx, s), _HERE_); \

#define klr_throw__iZERODIV(n)  \
	if(unlikely(n == 0)) { \
		klr_throw__T(ctx, "Arithmetic!!: Zero Divided"); \
	}\

#define klr_throw__fZERODIV(n)  \
	if(unlikely(n == KNH_FLOAT_ZERO)) { \
		klr_throw__T(ctx, "Arithmetic!!: Zero Divided"); \
	}\

/* [EXIT] */

#define KLR0_HALT(ctx) {\
		fprintf(stderr, "unexpected virtual machine termination\n");\
		return pc;\
	}\

/* [TR] */

typedef KLRAPI(void) (*klr_Ftr)(Ctx *, knh_sfp_t *, int, knh_class_t);

#define KLR_TR(Ctx, ftr, a, cid) { \
		ftr(ctx, sfp, a, cid);\
	}\

#define KLR_OTR(Ctx, ftr, a, cid) { \
		if(IS_NOTNULL(sfp[a].o)) {\
			ftr(ctx, sfp, a, cid);\
		}\
	}\

#define KLR_NULTR(Ctx, ftr, a, cid) { \
		if(IS_NULL(sfp[a].o)) {\
			ftr(ctx, sfp, a, cid);\
		}\
	}\

#define KLR_UNBOX(ctx, a) {\
		sfp[a].data = knh_Object_data(sfp[a].o);\
	}\

/**
#define KNH_SWAP(ctx, sfp, n, n2) {\
		knh_sfp_t temp = sfp[(n)];\
		sfp[(n)] = sfp[(n2)];\
		sfp[(n2)] = temp;\
	}\
*/

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

#define KLR_NSET(ctx, a, v) {\
		sfp[a].data = v;\
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

#define KLR_XIMOV(ctx, a, b)   SFXi(a) = sfp[b].ivalue
#define KLR_XFMOV(ctx, a, b)   SFXf(a) = sfp[b].fvalue
#define KLR_XBMOV(ctx, a, b)   SFXb(a) = sfp[b].bvalue

/* ------------------------------------------------------------------------ */

#define ENVo(x)   (sfp[0].cc)->envsfp[x].o
#define ENVn(x)   (sfp[0].cc)->envsfp[x].data

#define KLR_MOVe(ctx, a, b) {\
		klr_mov(ctx, sfp[a].o, ENVo(b));\
		sfp[a].data = ENVn(b);\
	}\

#define KLR_SWAP(ctx, a, b) {\
		knh_sfp_t temp = sfp[(a)];\
		sfp[(a)] = sfp[(b)];\
		sfp[(b)] = temp;\
	}\

/* ------------------------------------------------------------------------ */

#define KLR_CHKNUL(ctx, n) {\
		if(unlikely(IS_NULL(sfp[n].o))) { \
			klr_throw__T(ctx, "Null!!"); \
		} \
	} \

#define KLR_CHKNULx(ctx, n) \
	if(unlikely(IS_NULL(SFXo(n)))) { \
		klr_throw__T(ctx, "Null!!"); \
	} \

#define KLR_CHKTYPE(ctx, n, cid) \
	if(!knh_Object_opTypeOf(ctx, sfp[n].o, cid)) { \
		klr_throw__T(ctx, "ClassCast!!"); \
	} \

#define KLR_CHKTYPEx(ctx, n, cid) \
	if(!knh_Object_opTypeOf(ctx, SFXo(n), cid)) { \
		klr_throw__T(ctx, "ClassCast!!"); \
	} \

/* [CALL] */

#define KNH_SETESP(ctx, newesp)  \
		((knh_Context_t*)ctx)->esp = newesp; \

#define klr_setesp(ctx, newesp)  \
		((knh_Context_t*)ctx)->esp = newesp; \

#define klr_ncall(ctx, Fcall, lsfp) { \
		Fcall(ctx, lsfp);\
		sfp = ctx->stack + sfpidx;\
	}\

#define klr_vcall(ctx, mtd, n1, n2) { \
		sfp[n1].ci.shift = n2;\
		sfp[-2].pc = pc;\
		sfp = sfp + n2;\
		if(unlikely(sfp > ctx->stacktop)) {\
			klr_throw__T(ctx, "StackOverflow!!"); \
		}\
		pc = (mtd)->pc_start;\
		goto L_KONOHACALL;\
	}

knh_code_t* knh_VirtualMachine_run(Ctx *, knh_sfp_t *, knh_code_t *);
#define knh_Method_isKLRCode(mtd) ((mtd)->fcall_1 == NULL)
#define knh_Gamma_hasJIT(ctx)     0
#define CODE_RUN       (ctx->share->PC_RUN)
#define CODE_LAUNCH    (ctx->share->PC_LAUNCH)
#define CODE_ABSTRACT  (ctx->share->PC_ABSTRACT)
#define CODE_FUNCCALL  (ctx->share->PC_FUNCCALL)

#define klr_hcall(ctx, mtd, n1, n2) { \
		if(knh_Method_isKLRCode(mtd)) {\
			klr_vcall(ctx, mtd, n1, n2);\
		}else {\
			klr_ncall(ctx, (mtd)->fcall_1, sfp + n2);\
		}\
	}\

#define KNH_SCALL0(ctx, lsfp, rtnidx) { \
		int n1_ = rtnidx+1, n2_ = rtnidx+2;\
		if(knh_Method_isKLRCode(lsfp[n1_].mtd)) {\
			knh_VirtualMachine_run(ctx, lsfp + n2_, CODE_RUN);\
		}else {\
			(lsfp[n1_].mtd)->fcall_1(ctx, lsfp + n2_);\
		}\
	} \

#define KNH_SCALL(ctx, lsfp, rtnidx, argc) { \
		int n1_ = rtnidx+1, n2_ = rtnidx+2;\
		klr_setesp(ctx, lsfp + (argc+3));\
		if(knh_Method_isKLRCode(lsfp[n1_].mtd)) {\
			knh_VirtualMachine_run(ctx, lsfp + n2_, CODE_RUN);\
		}else {\
			(lsfp[n1_].mtd)->fcall_1(ctx, lsfp + n2_);\
		}\
	} \

typedef KLRAPI(void) (*klr_Fmethod)(Ctx *, knh_sfp_t *, int, knh_methodn_t);

#define KLR_LDMETHOD(ctx, fmtd, n, mn) { \
		fmtd(ctx, sfp, n, mn);\
	} \

#define KLR_CALL(ctx, n, espshift) { \
		int n1_ = n+1, n2_ = n+2;\
		klr_setesp(ctx, sfp + espshift);\
		klr_hcall(ctx, sfp[n1_].mtd, n1_, n2_);\
	} \

#define KLR2_SCALL(ctx, n, espshift, m) { \
		knh_Method_t *mtd_ = m;\
		int n1_ = n+1, n2_ = n+2;\
		klr_mov(ctx, sfp[n1_].o, mtd_);\
		klr_setesp(ctx, sfp + espshift);\
		klr_ncall(ctx, (mtd_)->fcall_1, sfp + n2_); \
	} \

#define KLR2_VCALL(ctx, n, espshift, m) { \
		knh_Method_t *mtd_ = m;\
		int n1_ = n+1, n2_ = n+2;\
		klr_mov(ctx, sfp[n1_].o, mtd_); \
		klr_vcall(ctx, mtd_, n1_, n2_); \
	} \

#define KLR_RET(ctx) {\
		sfp = sfp - sfp[-1].ci.shift;\
		pc = sfp[-2].pc; \
	}\

#define KLR0_FUNCCALL(ctx) { \
		klr_ncall(ctx, (sfp[-1].mtd)->fcall_1, 0);\
		KLR_RET(ctx);\
	} \

#define KLR0_YEILD(ctx, espidx) {\
		klr_setesp(ctx, sfp+espidx);\
		return pc + OPSIZE_YEILD;\
	}\

#define KLR0_ENTER(ctx) {\
		klr_hcall(ctx, sfp[-1].mtd, -1, 0);\
	}\

#define KLR0_EXEC(ctx) {\
		klr_vcall(ctx, sfp[-1].mtd, -1, 0);\
		return NULL;\
	}\

#define KLR0_EXIT(ctx) {\
		knh_stack_clear(ctx, sfp);\
		return NULL;\
	}\

/**
#define KLR_COPYSFP(ctx, n) { \
		DBG2_ASSERT(IS_bClosure(sfp[n].cc));\
		knh_Closure_copyEnv(ctx, sfp[n].cc, sfp);\
	} \
**/

/* ------------------------------------------------------------------------- */

#define KLR_SMAP(ctx, n, m)  { \
		klr_mov(ctx, sfp[n+1].o, m); \
		(sfp[n+1].mpr)->fmap_1(ctx, sfp + n); \
	} \

#define KLR_SMAPnc(ctx, n, m)  { \
		if(likely(IS_NOTNULL(sfp[n].o))) {\
			klr_mov(ctx, sfp[n+1].o, m);\
			(sfp[n+1].mpr)->fmap_1(ctx, sfp + n); \
		}\
	} \

#define KLR_MAP(ctx, n, tcid)  { \
		klr_mov(ctx, sfp[n+1].o, knh_findMapper(ctx, knh_Object_cid(sfp[n].o), tcid)); \
		(sfp[n+1].mpr)->fmap_1(ctx, sfp + n); \
	} \

#define KLR_MAPnc(ctx, n, tcid)  { \
		if(likely(IS_NOTNULL(sfp[n].o))) {\
			klr_mov(ctx, sfp[n+1].o, knh_findMapper(ctx, knh_Object_cid(sfp[n].o), tcid)); \
			(sfp[n+1].mpr)->fmap_1(ctx, sfp + n); \
		}\
	}\

#define KLR_AMAP(ctx, n, tcid)  { \
		knh_class_t scid = knh_Object_cid(sfp[n].o);\
		if(scid != ((knh_class_t)tcid) && !knh_class_instanceof(ctx, scid, tcid) && IS_NOTNULL(sfp[n].o)) { \
			klr_mov(ctx, sfp[n+1].o, knh_findMapper(ctx, scid, tcid)); \
			(sfp[n+1].mpr)->fmap_1(ctx, sfp + n); \
		} \
	} \

/* ======================================================================== */

#define KLR_JMP(ctx, PC, JUMP) {\
		PC; \
		goto JUMP; \
	}\

#define KLR_NOPJMP(ctx, PC, JUMP) { \
		((knh_inst_t*)pc)->opcode = OPCODE_JMP;\
	}\

#define KLR_JMPT(ctx, PC, JUMP, n) \
	if(sfp[n].bvalue) { \
		KLR_JMP(ctx, PC, JUMP); \
	} \

#define KLR_JMPF(ctx, PC, JUMP, n) \
	if(!sfp[n].bvalue) { \
		KLR_JMP(ctx, PC, JUMP); \
	} \

#define KLR2_JMPF_LOOP(ctx, PC, JUMP, n) \
	if(!sfp[n].bvalue) { \
		KLR_JMP(ctx, PC, JUMP); \
	} \

#define KLR_JMPNUL(ctx, PC, JUMP, n) \
	if(IS_NULL(sfp[n].o)) { \
		KLR_JMP(ctx, PC, JUMP); \
	} \

#define KLR_JMPNN(ctx, PC, JUMP, n) \
	if(IS_NOTNULL(sfp[n].o)) { \
		KLR_JMP(ctx, PC, JUMP); \
	} \

typedef KLRAPI(int) (*klr_Fchk)(Ctx *, knh_sfp_t *, int n);
typedef KLRAPI(int) (*klr_Fcmp)(Ctx *, knh_sfp_t *, int n, int n2);
typedef KLRAPI(int) (*klr_Fcmpi)(Ctx *, knh_sfp_t *, int n, knh_int_t v);
typedef KLRAPI(int) (*klr_Fcmpf)(Ctx *, knh_sfp_t *, int n, knh_float_t v);

#define KLR_JMPchk(ctx, PC, JUMP, fcheck, n) \
	if(fcheck(ctx, sfp, n)) { \
		KLR_JMP(ctx, PC, JUMP); \
	} \

#define KLR2_JMPcmp(ctx, PC, JUMP, fcmp, n, n2) \
	if(fcmp(ctx, sfp, n, n2)) { \
		KLR_JMP(ctx, PC, JUMP); \
	} \

#define KLR2_JMPcmpi(ctx, PC, JUMP, fcmpi, n, v) \
	if(fcmpi(ctx, sfp, n, v)) { \
		KLR_JMP(ctx, PC, JUMP); \
	} \

#define KLR2_JMPcmpf(ctx, PC, JUMP, fcmpf, n, v) \
	if(fcmpf(ctx, sfp, n, v)) { \
		KLR_JMP(ctx, PC, JUMP); \
	} \

/* ------------------------------------------------------------------------- */

#define KLR_NEXT(ctx, PC, JUMP, na, ib) { \
		knh_sfp_t *itrsfp_ = sfp + ib; \
		DBG2_ASSERT(IS_bIterator(itrsfp_[0].it));\
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

#define KLR_THROW(ctx, start, espn) { \
		DBG2_ASSERT(IS_Exception(sfp[espn].e)); \
		KNH_SETESP(ctx, sfp+espn);\
		knh_stack_throw(ctx, sfp + start, sfp[espn].e, _HERE_); \
	}\

#define KLR_THROWs(ctx, start, espn, msg) {\
		DBG2_ASSERT(IS_bString(msg)); \
		KNH_SETESP(ctx, sfp+espn);\
		knh_stack_throw(ctx, sfp + start, new_Exception(ctx, msg), _HERE_); \
	}

#define KNH_SETJUMP(hdlr) knh_setjmp(DP(hdlr)->jmpbuf)

#define KLR_TRY(ctx, PC, JUMP, hn)  {\
		knh_ExceptionHandler_t* _hdr = sfp[hn].hdr; \
		if(!IS_ExceptionHandler(_hdr)) { \
			_hdr = new_ExceptionHandler(ctx); \
			klr_mov(ctx, sfp[hn].o, _hdr); \
		} \
		klr_setesp(ctx, sfp + hn + 1); \
		if(!knh_ExceptionHandler_isJumpable(_hdr)) { \
			int jump = KNH_SETJUMP(_hdr); \
			knh_ExceptionHandler_setJumpable(_hdr, 1); \
			if(jump != 0) { \
				klr_setesp(ctx, DP(_hdr)->esp); \
				pc = DP(_hdr)->pc; \
				KLR_JMP(ctx, PC, JUMP); \
			} \
			DP(_hdr)->esp = sfp + hn + 1; \
			DP(_hdr)->pc  = pc; \
		} \
		knh_ExceptionHandler_setCatching(_hdr, 1); \
	} \

#define KLR_TRYEND(ctx, hn)  {\
		DBG2_ASSERT(IS_ExceptionHandler(sfp[hn].o)); \
		KNH_SETv(ctx, DP(sfp[hn].hdr)->caught, KNH_NULL);\
	} \

#define KLR_CATCH(ctx, PC, JUMP, hn, en, emsg) { \
		DBG2_ASSERT(IS_ExceptionHandler(sfp[hn].hdr)); \
		knh_Exception_t* _e = DP(sfp[hn].hdr)->caught; \
		if(knh_Exception_isa(ctx, _e, (knh_String_t*)emsg)) { \
			DBG2_P("CATHING .."); \
			klr_mov(ctx, sfp[en].o, _e); \
			KNH_SETv(ctx, DP(sfp[hn].hdr)->caught, KNH_NULL);\
		} \
		else { \
			KLR_JMP(ctx, PC, JUMP); \
		} \
	} \

#define KLR_THROW_AGAIN(ctx, hn) { \
		DBG2_ASSERT(IS_ExceptionHandler(sfp[hn].o)); \
		knh_Exception_t *_e = DP(sfp[hn].hdr)->caught; \
		if(IS_Exception(_e)) {\
			DBG2_P("THROW AGAIN ... ");\
			KNH_SETESP(ctx, sfp + hn + 1); \
			knh_stack_throw(ctx, sfp, _e, NULL, 0); \
		}\
	} \

/**
#define KLR_PUSH(ctx, n)   {\
		DBG_P("push %d", (int)n); \
	} \

#define KLR_POP(ctx, n)  {\
		DBG_P("pop %d", (int)n); \
	} \
**/

/* ------------------------------------------------------------------------ */

#define KLR_P(ctx, flag, mn, n) knh_stack_p(ctx, sfp + n, flag, mn)
#define KLR_PMSG(ctx, flag, msg)  knh_stack_pmsg(ctx, flag, msg)

/* ------------------------------------------------------------------------ */

#define KLR_iCAST(ctx, a) {\
		knh_int_t n_ = (knh_int_t)sfp[a].fvalue; \
		sfp[a].ivalue = n_; \
	}\

#define KLR_fCAST(ctx, a) {\
		knh_float_t n_ = (knh_float_t)sfp[a].ivalue; \
		sfp[a].fvalue = n_; \
	}\

#define KLR_inCAST(ctx, a) {\
		if(IS_NULL(sfp[a].o)) { \
			sfp[a].ivalue = 0; \
		}else{\
			knh_int_t n_ = (knh_int_t)sfp[a].fvalue; \
			sfp[a].ivalue = n_; \
		}\
	}\

#define KLR_fnCAST(ctx, a) {\
		if(IS_NULL(sfp[a].o)) { \
			sfp[a].fvalue = 0; \
		}else{\
			knh_float_t n_ = (knh_float_t)sfp[a].ivalue; \
			sfp[a].fvalue = n_; \
		}\
	}\

/* ------------------------------------------------------------------------ */

#define KLR_bNOT(ctx, c, a)     SFb(c) = !(SFb(a))

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

#define KLR_iEQ(ctx, c, a, b)  SFi(c) = (SFi(a) == SFi(b))
#define KLR_iEQn(ctx, c, a, n)  SFi(c) = (SFi(a) == n)
#define KLR_iNEQ(ctx, c, a, b)  SFi(c) = (SFi(a) != SFi(b))
#define KLR_iNEQn(ctx, c, a, n)  SFi(c) = (SFi(a) != n)

#define KLR_iLT(ctx, c, a, b)  SFb(c) = (SFi(a) < SFi(b))
#define KLR_iLTn(ctx, c, a, n)  SFb(c) = (SFi(a) < n)
#define KLR_iLTE(ctx, c, a, b)  SFb(c) = (SFi(a) <= SFi(b))
#define KLR_iLTEn(ctx, c, a, n)  SFb(c) = (SFi(a) <= n)
#define KLR_iGT(ctx, c, a, b)  SFb(c) = (SFi(a) > SFi(b))
#define KLR_iGTn(ctx, c, a, n)  SFb(c) = (SFi(a) > n)
#define KLR_iGTE(ctx, c, a, b)  SFb(c) = (SFi(a) >= SFi(b))
#define KLR_iGTEn(ctx, c, a, n)  SFb(c) = (SFi(a) >= n)

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

#define KLR_fEQ(ctx, c, a, b) SFb(c) = (SFf(a) == SFf(b))
#define KLR_fEQn(ctx, c, a, n) SFb(c) = (SFf(a) == n)
#define KLR_fNEQ(ctx, c, a, b)  SFb(c) = (SFf(a) != SFf(b))
#define KLR_fNEQn(ctx, c, a, n)  SFb(c) = (SFf(a) != n)
#define KLR_fLT(ctx, c, a, b)  SFb(c) = (SFf(a) < SFf(b))
#define KLR_fLTn(ctx, c, a, n)  SFb(c) = (SFf(a) < n)
#define KLR_fLTE(ctx, c, a, b)  SFb(c) = (SFf(a) <= SFf(b))
#define KLR_fLTEn(ctx, c, a, n)  SFb(c) = (SFf(a) <= n)
#define KLR_fGT(ctx, c, a, b)  SFb(c) = (SFf(a) > SFf(b))
#define KLR_fGTn(ctx, c, a, n)  SFb(c) = (SFf(a) > n)
#define KLR_fGTE(ctx, c, a, b)  SFb(c) = (SFf(a) >= SFf(b))
#define KLR_fGTEn(ctx, c, a, n)  SFb(c) = (SFf(a) >= n)

/* ------------------------------------------------------------------------ */

#define KLR_LABEL(ctx, id, msg)
#define KLR_NOP(ctx)

/* ------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------ */

#define BEGIN_LOCAL(ctx, n)   knh_stack_local(ctx, n)
#define END_LOCAL(ctx, lsfp)  ((knh_Context_t*)ctx)->esp = lsfp

#define KNH_SMAP(ctx, lsfp, n, m)  { \
		klr_mov(ctx, lsfp[n+1].o, m);\
		KNH_ASSERT(IS_Mapper(m)); \
		(lsfp[n+1].mpr)->fmap_1(ctx, lsfp + n); \
	} \

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

#define KLR0_THCODE(ctx) { \
		knh_code_thread(ctx, pc, OPJUMP); \
		return pc + OPSIZE_THCODE; \
	}\

typedef KLRAPI(void) (*klr_Fprobe)(Ctx *, knh_sfp_t*, int n, knh_code_t **pc);

#define KLR0_PROBE(ctx, fprobe, n) { \
		fprobe(ctx, sfp, n, &pc);\
	}\

#define KNH_SECURE(ctx, sfp) knh_stack_checkSecurityManager(ctx, sfp)

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif /*KONOHA_VM_H_*/
