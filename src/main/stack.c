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

/* ************************************************************************ */

//#define USE_STEXT 1
#define USE_bytes_first       1
#define USE_bytes_index       1
//#define USE_bytes_last        1
//#define USE_bytes_indexOf     1
//#define USE_bytes_startsWith  1
//#define USE_cwb_open      1
#define USE_cwb_tobytes   1

#include"commons.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */

knh_sfp_t* knh_stack_initexpand(Ctx *ctx, knh_sfp_t *sfp, size_t n)
{
	knh_Context_t *ctxo = (knh_Context_t*)ctx;
	size_t i, s;
	if(sfp == NULL) {
		DBG_ASSERT(ctxo->stacksize == 0);
		s = 0;
		ctxo->stacksize = n;
		ctxo->stack = (knh_sfp_t*)KNH_MALLOC(ctx, sizeof(knh_sfp_t) * ctxo->stacksize);
		ctxo->esp = ctxo->stack;
		ctxo->mtdCache = (knh_Method_t**)KNH_MALLOC(ctx, K_PAGESIZE * (sizeof(void*) / 4));
		knh_bzero(ctxo->mtdCache, K_PAGESIZE * (sizeof(void*) / 4));
		ctxo->fmtCache = ctxo->mtdCache + K_CACHESIZE;
		ctxo->trlCache = (knh_Translator_t**)(ctxo->fmtCache + K_CACHESIZE);
		KNH_INITv(ctxo->bufw, new_(OutputStream));
		knh_OutputStream_setBOL(ctxo->bufw, 1);
		knh_OutputStream_setStoringBuffer(ctxo->bufw, 1);
		KNH_INITv(ctxo->bufa, DP(ctxo->bufw)->ba);
	}
	else {
		knh_sfp_t **cstack_top = &sfp;
		knh_sfp_t *newstack;
		size_t size = ctxo->stacksize, newsize = ctxo->stacksize * 2;
		if(newsize < size + n) newsize = size + n;
		DBG_ASSERT(ctxo->stacksize != 0);
		newstack = (knh_sfp_t*)KNH_MALLOC(ctx, sizeof(knh_sfp_t) * newsize);
		knh_memcpy(newstack, ctxo->stack, sizeof(knh_sfp_t) * size);
		KNH_SYSLOG(ctx, LOG_NOTICE, "ExtendedStack", "newsize=%d, oldstack=%p-%p", (int)newsize, ctx->stack, ctx->stack+size);
		if(ctxo->cstack_bottom < (void*)cstack_top) {
			knh_sfp_t **p = (knh_sfp_t**)ctxo->cstack_bottom;
			while(p <= cstack_top) {
				knh_uintptr_t addr = (knh_uintptr_t)p[0];
				if((ctxo->stack <= p[0] && p[0] < ctxo->stack + size) && addr % sizeof(void*) == 0) {
					knh_sfp_t *newsfp = p[0] + (newstack - ctxo->stack);
					DBG_ASSERT(newstack <= newsfp && newsfp < newstack + size);
//					DBG_P("addr=%lld, sfp=%p[%d] => %p[%d]", (knh_int_t)(addr % sizeof(void*)), p[0], (int)(p[0]->ivalue), newsfp, (int)newsfp->ivalue);
					DBG_ASSERT((p[0])->data == newsfp->data);
					p[0] = newsfp;
				}
				p++;
			}
		}
		else {
			knh_sfp_t **p = (knh_sfp_t**)ctxo->cstack_bottom;
			while(cstack_top <= p) {
				knh_uintptr_t addr = (knh_uintptr_t)p[0];
				if((ctxo->stack <= p[0] && p[0] < ctxo->stack + size) && addr % sizeof(void*) == 0) {
					knh_sfp_t *newsfp = p[0] + (newstack - ctxo->stack);
					DBG_ASSERT(newstack <= newsfp && newsfp < newstack + size);
//					DBG_P("addr=%lld, sfp=%p[%d] => %p[%d]", (knh_int_t)(addr % sizeof(void*)), p[0], (int)(p[0]->ivalue), newsfp, (int)newsfp->ivalue);
					DBG_ASSERT((p[0])->data == newsfp->data);
					p[0] = newsfp;
				}
				p--;
			}
		}
		KNH_FREE(ctx, ctxo->stack, sizeof(knh_sfp_t) * size);
		ctxo->stack = newstack;
		ctxo->stacksize = newsize;
		s = size;
	}
	for(i = s; i < ctxo->stacksize; i++) {
		KNH_INITv(ctxo->stack[i].o, KNH_NULL);
		ctxo->stack[i].data = 0;
	}
	ctxo->stacktop = ctxo->stack + (ctxo->stacksize - K_GAMMASIZE);
	return sfp;
}

/* ------------------------------------------------------------------------ */

void knh_stack_clear(Ctx *ctx, knh_sfp_t *sfp)
{
	if(!(ctx->stack <= sfp && sfp < ctx->stacktop)) {
		sfp = ctx->stack; // sometimes, rewriting pointer is failed.
	}
	while(sfp < ctx->stack + ctx->stacksize) {
		KNH_SETv(ctx, sfp[0].o, KNH_NULL);
		sfp[0].data = 0;
		sfp++;
	}
}

/* ------------------------------------------------------------------------ */

knh_sfp_t* knh_stack_local(Ctx *ctx, size_t n)
{
	knh_sfp_t *esp = ctx->esp;
	((knh_Context_t*)ctx)->esp = esp + n;
	return esp;
}

/* ------------------------------------------------------------------------ */
/* [call] */

void knh_stack_typecheck(Ctx *ctx, knh_sfp_t *sfp, knh_Method_t *mtd, knh_opline_t *pc)
{
	knh_class_t this_cid = knh_Object_cid(sfp[0].o);
	int i, argc;
	DBG_ASSERT(IS_Method(sfp[K_MTDIDX].callmtd));
	argc = knh_ParamArray_isVARGs(DP(mtd)->mp) ? (ctx->esp - sfp) : knh_Method_psize(mtd);
	for(i = 1; i < argc; i++) {
		knh_type_t type = knh_Method_ptype(ctx, mtd, this_cid, i - 1);
		knh_class_t reqc = CLASS_type(type);
		if(!knh_class_instanceof(ctx, knh_Object_cid(sfp[i].o), reqc)) {
			SYSLOG_ParamTypeError(ctx, sfp, DP(mtd)->mn, i, reqc, knh_Object_cid(sfp[i].o));
			break;
		}
	}
	return;
}


///* ------------------------------------------------------------------------ */
//
//KNHAPI(void) konoha_throwSecurityException(void)
//{
//	Ctx *ctx = knh_getCurrentContext();
//	knh_stack_throw(ctx, new_Exception(ctx, knh_getEventName(ctx, EBI_Security)), NULL, 0);
//}


/* ------------------------------------------------------------------------ */
/* [Event] */

int knh_expt_isa(Ctx *ctx, knh_ebi_t eid, knh_ebi_t parent)
{
	ASSERT_ebi(eid);
	DBG_ASSERT(parent <= ctx->share->EventTBLSize);
	if(eid == parent || parent == 1) return 1;
	if(eid == 1) return 0;
	while((eid = ctx->share->EventTBL[eid-1].parent) != 1) {
		if(eid == parent) return 1;
	}
	return 0;
}

/* ------------------------------------------------------------------------ */

knh_String_t *knh_getEventName(Ctx *ctx, knh_ebi_t eid)
{
	ASSERT_ebi(eid);
	return ctx->share->EventTBL[eid-1].name;
}

/* ------------------------------------------------------------------------ */
/* [TABLE] */

static knh_ebi_t new_EventId(Ctx *ctx)
{
	knh_class_t newid = 0;
	OLD_LOCK(ctx, LOCK_SYSTBL, NULL);
	if(ctx->share->EventTBLSize == ctx->share->EventTBLMax) {
		knh_expandEventTBL(ctx);
	}
	((knh_share_t*)ctx->share)->EventTBLSize += 1;
	newid = ctx->share->EventTBLSize;
	OLD_UNLOCK(ctx, LOCK_SYSTBL, NULL);
	return newid;
}

knh_ebi_t knh_addEvent(Ctx *ctx, knh_flag_t flag, knh_class_t eid, knh_String_t *name, knh_class_t peid)
{
	if(eid == EBI_newid) {
		eid = new_EventId(ctx);
	}else {
		((knh_share_t*)ctx->share)->EventTBLSize += 1;
		DBG_ASSERT(eid == ctx->share->EventTBLSize);
	}
	ASSERT_ebi(eid);
	{
		knh_EventTBL_t *t = pEventTBL(eid-1);
		DBG_ASSERT(t->name == NULL);
		t->flag = flag;
		t->parent = peid;
		KNH_INITv(t->name, name);
		OLD_LOCK(ctx, LOCK_SYSTBL, NULL);
		knh_DictCaseSet_set(ctx, DP(ctx->sys)->EventDictCaseSet, name, eid);
		OLD_UNLOCK(ctx, LOCK_SYSTBL, NULL);
	}
	return eid;
}

/* ------------------------------------------------------------------------ */

knh_ebi_t knh_geteid(Ctx *ctx, knh_bytes_t msg, knh_ebi_t def)
{
	knh_ebi_t eid = EBI_Exception;
	knh_intptr_t loc = knh_bytes_index(msg, '!');
	if(loc != -1) {
		if(msg.ustr[loc+1] != '!') return eid;
		msg = knh_bytes_first(msg, loc);
	}
	if(msg.len == 0) return EBI_Exception; /* '!!' */
	{
		OLD_LOCK(ctx, LOCK_SYSTBL, NULL);
		eid = (knh_ebi_t)knh_DictCaseSet_get(ctx, DP(ctx->sys)->EventDictCaseSet, msg);
		OLD_UNLOCK(ctx, LOCK_SYSTBL, NULL);
		DBG_P("'%s', eid=%d", msg.text, eid);
		if(eid != 0) return eid;
		if(def == EBI_newid) {
			return knh_addEvent(ctx, 0, EBI_newid, new_S(ctx, msg), EBI_Exception);
		}
	}
	return def;
}

/* ======================================================================== */
/* [Exception.new] */

knh_Exception_t* knh_Exception_setup(Ctx *ctx, knh_Exception_t *e, knh_String_t *event, knh_String_t *msg, Object *bag)
{
	DBG_ASSERT(IS_Exception(e));
	knh_ebi_t eid = knh_geteid(ctx, S_tobytes(event), EBI_unknown/*newid*/);
	if(eid == EBI_unknown) {
		KNH_SYSLOG(ctx, LOG_WARNING, "ThrownException", "unknown exception: %s", S_tochar(event));
		DP(e)->eid = EBI_Exception;
	}
	else {
		DP(e)->eid = eid;
	}
	ASSERT_ebi(DP(e)->eid);
	DP(e)->flag = ctx->share->EventTBL[DP(e)->eid].flag;
	KNH_SETv(ctx, DP(e)->event, event);
	KNH_SETv(ctx, DP(e)->msg, msg);
	KNH_SETv(ctx, DP(e)->bag, bag);
	return e;
}

/* ------------------------------------------------------------------------ */

knh_Exception_t* new_Exception(Ctx *ctx, knh_String_t *msg)
{
	knh_Exception_t* e = new_(Exception);
	knh_ebi_t eid = knh_geteid(ctx, S_tobytes(msg), EBI_newid);
	DP(e)->eid = eid;
	DP(e)->flag = ctx->share->EventTBL[eid].flag;
	KNH_SETv(ctx, DP(e)->msg, msg);
	return e;
}

void knh_Context_setThrowingException(Ctx *ctx, knh_Exception_t *e)
{
	KNH_SETv(ctx, ((knh_Context_t*)ctx)->e, e);
}

///* ------------------------------------------------------------------------ */
//
//KNHAPI(knh_Exception_t*) knh_cwb_newException(Ctx *ctx, knh_cwb_t *cwb)
//{
//	knh_Exception_t* e = new_(Exception);
//	knh_ebi_t eid = knh_geteid(ctx, knh_cwb_tobytes(cwb), EBI_newid);
//	DP(e)->eid = eid;
//	DP(e)->flag = ctx->share->EventTBL[eid].flag;
//	KNH_SETv(ctx, DP(e)->msg, knh_cwb_newString(ctx, cwb));
//	return e;
//}
//
///* ------------------------------------------------------------------------ */
//
//KNHAPI(knh_Exception_t*) new_Exception__T(Ctx *ctx, const char *msg)
//{
//	knh_Exception_t* e = new_(Exception);
//	knh_bytes_t t = {{msg}, knh_strlen(msg)};
//	knh_ebi_t eid = knh_geteid(ctx, t, EBI_newid);
//	DP(e)->eid = eid;
//	DP(e)->flag = ctx->share->EventTBL[eid].flag;
//	KNH_SETv(ctx, DP(e)->msg, new_T(msg));
//	return e;
//}
//
///* ------------------------------------------------------------------------ */
//

/* ======================================================================== */
/* [ExceptionHandler] */
//
//knh_ExceptionHandler_t* new_ExceptionHandler(Ctx *ctx)
//{
//	return (knh_ExceptionHandler_t*)new_Object_bcid(ctx, CLASS_ExceptionHandler, 0);
//}
//
//
///* ------------------------------------------------------------------------ */
//
//knh_Exception_t* knh_ExceptionHandler_getCaughtException(knh_ExceptionHandler_t *o)
//{
//	KNH_ASSERT(IS_Exception(DP(o)->caught));
//	return DP(o)->caught;
//}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
