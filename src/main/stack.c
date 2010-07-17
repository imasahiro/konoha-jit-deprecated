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

#define USE_STEXT 1
#define USE_bytes_first       1
#define USE_bytes_index       1
#define USE_bytes_last        1
#define USE_bytes_indexOf     1
//#define USE_bytes_startsWith  1
#define USE_cwb_open      1
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
//		for(i = 0; i < K_CACHESIZE; i++) {
//			ctxo->mtdCache[i] = NULL /*mtdInit*/;
//			ctxo->fmtCache[i] = NULL /*fmtInit*/;
//			ctxo->trlCache[i] = NULL /* mprInit*/;
//		}
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

KNHAPI(void) knh_stack_boxing(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_class_t bcid = (sfp[0].o)->h.bcid;
	if(CLASS_Boolean <= bcid && bcid <= CLASS_Float && sfp[0].data != knh_Object_data(sfp[0].o)) {
		klr_mov(ctx, sfp[0].o, new_Object_boxing(ctx, knh_Object_cid(sfp[0].o), sfp));
	}
}

///* ------------------------------------------------------------------------ */
//
//KNHAPI(void) knh_stack_w(Ctx *ctx, knh_sfp_t *sfp, knh_sfp_t *one, knh_methodn_t mn, knh_OutputStream_t *w)
//{
//	knh_Method_t *mtd = knh_getSystemFormatter(ctx, knh_Object_cid(one[0].o), mn);
//	KNH_SETv(ctx, sfp[3].o, one[0].o); sfp[3].data = one[0].data;
//	KNH_SETv(ctx, sfp[4].o, w);
//	KNH_SCALL(ctx, sfp, 0, mtd, 1);
//}

/* ======================================================================== */
/* [call] */

void knh_stack_typecheck(Ctx *ctx, knh_sfp_t *sfp, knh_Method_t *mtd, knh_opline_t *pc)
{
	const char *emsg;
	knh_class_t this_cid = knh_Object_cid(sfp[0].o);
	int i, argc;
	DBG_ASSERT(IS_Method(sfp[K_MTDIDX].callmtd));
	argc = knh_ParamArray_isVARGs(DP(mtd)->mp) ? (ctx->esp - sfp) : knh_Method_psize(mtd);
	for(i = 1; i < argc; i++) {
		knh_type_t type = knh_Method_ptype(ctx, mtd, this_cid, i - 1);
		knh_class_t reqc = CLASS_type(type);
		if(!knh_class_instanceof(ctx, knh_Object_cid(sfp[i].o), reqc)) {
			emsg = "Type!!: the parameter %d of %M";
			goto L_THROWERR;
		}
	}
	return;

	L_THROWERR:
	{
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		knh_printf(ctx, cwb->w, emsg, (knh_intptr_t)i, DP(mtd)->mn);
		knh_stack_throw(ctx, ctx->esp, knh_cwb_newException(ctx, cwb));
	}
}

/* ======================================================================== */
/* [throw] */

static knh_bool_t knh_stack_isCalledMethod(Ctx *ctx, knh_sfp_t *sfp)
{
	//IS_Method(sfp[0].mtd)
	return 0;
}

knh_sfp_t* knh_stack_callee(Ctx *ctx, knh_sfp_t *sfp, char **file, int *linenum)
{
	while(ctx->stack <= sfp) {
		if(knh_stack_isCalledMethod(ctx, sfp)) {
			knh_opline_t *pc = sfp[0].pc;
			//DBG_P("sfp[%d] pc=%p callee yes=%d", sfp - ctx->stack, pc, sfp[0].data != knh_Object_data(sfp[0].mtd));
			if(pc == NULL) {
				return sfp;
			}
//			if(sfp[0].data != knh_Object_data(sfp[0].mtd)) {
//				*linenum = knh_Method_pcline(sfp[0].mtd, pc);
//				*file = knh_Method_file(ctx, sfp[0].mtd);
//				return sfp;
//			}
		}
		sfp--;
	}
	return ctx->stack;
}

//static void knh_stack_writeStackTrace(Ctx *ctx, knh_sfp_t *sfp, knh_OutputStream_t *w)
//{
//	knh_Method_t *mtd = sfp[K_MTDIDX].callmtd;
//	knh_write_sname(ctx, w, DP(mtd)->cid);
//	if(DP(mtd)->mn != MN_LAMBDA) {
//		int i = 0, psize = knh_Method_psize(mtd);
//		knh_putc(ctx, w, '.');
//		knh_write_mn(ctx, w, DP(mtd)->mn);
//		knh_putc(ctx, w, '(');
//		for(i = 0; i < psize; i++) {
//			knh_param_t *p = knh_ParamArray_get(DP(mtd)->mp, i);
//			knh_type_t type = knh_type_tocid(ctx, p->type, knh_Object_cid(sfp[0].o));
//			if(i > 0) {
//				knh_putc(ctx, w, ',');
//			}
//			knh_write_fn(ctx, w, p->fn);
//			knh_putc(ctx, w, '=');
//			if(IS_Tunbox(type)) {
//				knh_write_ifmt(ctx, w, K_INT_FMT, sfp[i+1].ivalue);
//			}
//			else if (IS_Tfloat(type)) {
//				knh_write_ifmt(ctx, w, K_FLOAT_FMT, sfp[i+1].fvalue);
//			}
//			else if(type == TYPE_Boolean){
//				knh_write_bool(ctx, w, sfp[i+1].bvalue);
//			}
//			else {
//				knh_stack_w(ctx, ctx->esp, &sfp[i+1], MN__k, w, KNH_NULL);
//			}
//		}
//		knh_putc(ctx, w, ')');
//	}
//}


//static void knh_stack_writeReturnValue(Ctx *ctx, knh_sfp_t *sfp, knh_OutputStream_t *w, knh_type_t rtype)
//{
//	if(rtype == TYPE_void) {
//		knh_write(ctx, w, STEXT("void"));
//	}
//	if(IS_Tunbox(rtype)) {
//		knh_write_ifmt(ctx, w, K_INT_FMT, sfp[-1].ivalue);
//	}
//	else if (IS_Tfloat(rtype)) {
//		knh_write_ifmt(ctx, w, K_FLOAT_FMT, sfp[-1].fvalue);
//	}
//	else if(rtype == TYPE_Boolean){
//		knh_write_bool(ctx, w, sfp[-1].bvalue);
//	}
//	else {
//		knh_stack_w(ctx, ctx->esp, &sfp[-1], MN__k, w, KNH_NULL);
//	}
//}

/* ------------------------------------------------------------------------ */

METHOD knh_Fmethod_stackTrace(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
//	knh_Method_t *mtd = sfp[K_MTDIDX].callmtd;
//	knh_OutputStream_t *w = KNH_STDERR;
//	knh_intptr_t count = DP(mtd)->prof_count;
//	DBG_(knh_sfp_t *esp = ctx->esp;)
//	knh_type_t rtype = knh_Method_rztype(mtd);
//	knh_putc(ctx, w, '#');
//	knh_write_ifmt(ctx, w, K_INT_FMT, (knh_int_t)count);
//	knh_putc(ctx, w, ':');
//	knh_stack_writeStackTrace(ctx, sfp, w);
//	knh_write_EOL(ctx, w);
//	DBG_ASSERT(ctx->esp == esp);
//	DP(mtd)->prof_count += 1;
//	DP(mtd)->fproceed(ctx, sfp);
//	knh_putc(ctx, w, '#');
//	knh_write_ifmt(ctx, w, K_INT_FMT, (knh_int_t)count);
//	knh_write(ctx, w, STEXT(":-> "));
//	knh_stack_writeReturnValue(ctx, sfp, w, rtype);
//	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

//static knh_sfp_t *knh_stack_addStackTrace(Ctx *ctx, knh_sfp_t *sfp, knh_Exception_t *e)
//{
//	char *file = "-";
//	int  line = 0;
//	knh_sfp_t *callee = knh_stack_callee(ctx, sfp, &file, &line);
//	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
//	knh_write_fline(ctx, cwb->w, file, line);
//	knh_putc(ctx, cwb->w, ':');
//	knh_stack_writeStackTrace(ctx, sfp, cwb->w);
//	{
//		knh_String_t *msg = knh_cwb_newString(ctx, cwb);
//		if(IS_NULL(DP(e)->traces)) {
//			KNH_SETv(ctx, DP(e)->traces, new_Array(ctx, CLASS_String, 16));
//		}
//		knh_Array_add_(ctx, DP(e)->traces, UPCAST(msg));
//	}
//	return callee;
//}

/* ------------------------------------------------------------------------ */

KNHAPI(void) knh_stack_throw(Ctx *ctx, knh_sfp_t *sfp, knh_Exception_t *e)
{
	knh_sfp_t *sp = (sfp == NULL) ? ctx->esp : sfp;
//	if(DP(e)->line != 0) {
//		if(line == 0) {
//			knh_stack_callee(ctx, sp, &file, &line);
//		}
//		DP(e)->file = file;
//		DP(e)->line = line;
//	}
	while(ctx->stack <= sp) {
		if(IS_ExceptionHandler(sp[0].hdr) && knh_ExceptionHandler_isCatching(sp[0].hdr)) {
			TODO();
		}
//		else if(IS_Method(sp[0].o) && sp[0].data != knh_Object_data(sp[0].mtd)) {
//			knh_stack_addStackTrace(ctx, sp+1, e);
//		}
		sp--;
	}
	fprintf(stderr, "********** USE STACKTRACE IN YOUR C/C++ DEBUGGER ************\n");
	fprintf(stderr, "Uncaught Exception: %s\n", S_tochar(DP(e)->msg));
	fprintf(stderr, "*************************************************************\n");
	exit(0);
}

/* ------------------------------------------------------------------------ */

knh_ExceptionHandler_t* knh_stack_findExceptionHandler(Ctx *ctx, knh_sfp_t *sfp, long start)
{
	knh_sfp_t *sp = (sfp == NULL) ? ctx->esp : sfp + start;
//	if(DP(e)->line != 0) {
//		if(line == 0) {
//			knh_stack_callee(ctx, sp, &file, &line);
//		}
//		DP(e)->file = file;
//		DP(e)->line = line;
//	}
	while(ctx->stack <= sp) {
		if(IS_ExceptionHandler(sp[0].hdr) && knh_ExceptionHandler_isCatching(sp[0].hdr)) {
			return sp[0].hdr;
		}
//		else if(IS_Method(sp[0].o) && sp[0].data != knh_Object_data(sp[0].mtd)) {
//			knh_stack_addStackTrace(ctx, sp+1, e);
//		}
		sp--;
	}
	fprintf(stderr, "********** USE STACKTRACE IN YOUR C/C++ DEBUGGER ************\n");
	fprintf(stderr, "Uncaught Exception: %s\n", S_tochar(DP(ctx->e)->msg));
	fprintf(stderr, "*************************************************************\n");
	exit(0);
	return NULL;
}

///* ------------------------------------------------------------------------ */
//
//KNHAPI(void) konoha_throwSecurityException(void)
//{
//	Ctx *ctx = knh_getCurrentContext();
//	knh_stack_throw(ctx, new_Exception(ctx, knh_getExptName(ctx, EBI_Security)), NULL, 0);
//}

/* ------------------------------------------------------------------------ */
/* [EventTBL] */

static knh_ebi_t new_ExptId(Ctx *ctx)
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

/* ------------------------------------------------------------------------ */

int knh_expt_isa(Ctx *ctx, knh_ebi_t eid, knh_ebi_t parent)
{
	ASSERT_ebi(eid);
	KNH_ASSERT(parent <= ctx->share->EventTBLSize);
	if(eid == parent || parent == 1) return 1;
	if(eid == 1) return 0;
	while((eid = ctx->share->EventTBL[eid-1].parent) != 1) {
		if(eid == parent) return 1;
	}
	return 0;
}

/* ------------------------------------------------------------------------ */

knh_String_t *knh_getExptName(Ctx *ctx, knh_ebi_t eid)
{
	ASSERT_ebi(eid);
	return ctx->share->EventTBL[eid-1].name;
}

/* ------------------------------------------------------------------------ */
/* [forname] */

knh_ebi_t knh_geteid(Ctx *ctx, knh_bytes_t msg, knh_ebi_t def)
{
	knh_ebi_t eid = EBI_Exception;
	knh_intptr_t loc = knh_bytes_index(msg, '!');
	if(loc != -1) {
		if(msg.ustr[loc+1] != '!') {
			return eid;
		}
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
			return knh_addException(ctx, 0, EBI_newid, new_S(ctx, msg), EBI_Exception);
		}
	}
	return def;
}

/* ------------------------------------------------------------------------ */
/* [TABLE] */

knh_ebi_t knh_addException(Ctx *ctx, knh_flag_t flag, knh_class_t eid, knh_String_t *name, knh_class_t peid)
{
	if(eid == EBI_newid) {
		eid = new_ExptId(ctx);
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

/* ======================================================================== */
/* [Exception.new] */

/* @method Exception! Exception.new:init(String e, String msg, Object bag) */
// new Exception("Security!!", "hogehoge");
// new Exception("Security!!: hogehoge", null);

knh_Exception_t* knh_Exception_new__init(Ctx *ctx, knh_Exception_t *e, knh_String_t *ename, knh_String_t *msg, Object *bag)
{
	knh_ebi_t eid = EBI_Exception;
	if(IS_NOTNULL(ename)) {
		KNH_RCSETv(ctx, DP(e)->bag, ename);
		eid = knh_geteid(ctx, S_tobytes(ename), EBI_newid /*EBI_unknown*/);
	}
	if(eid == EBI_unknown) {
		KNH_SYSLOG(ctx, LOG_WARNING, "unknown exception: %s", S_tochar(ename));
		DP(e)->eid = EBI_Exception;
	}
	else {
		DP(e)->eid = eid;
	}
	ASSERT_ebi(eid);
	DP(e)->flag = ctx->share->EventTBL[DP(e)->eid].flag;
	{
		if(IS_NOTNULL(msg)) {
			KNH_RCSETv(ctx, DP(e)->bag, msg);
			knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
			knh_write_text(ctx, cwb->w, EBI__(DP(e)->eid));
			knh_write_text(ctx, cwb->w, "!!: ");
			knh_write(ctx, cwb->w, S_tobytes(msg));
			KNH_SETv(ctx, DP(e)->msg, knh_cwb_newString(ctx, cwb));
		}
		else {
			int loc = knh_bytes_indexOf(S_tobytes(ename), STEXT("!!:"));
			if(loc > 0 && eid != EBI_unknown) {
				KNH_SETv(ctx, DP(e)->msg, ename);
				return e;
			}
			knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
			knh_write_text(ctx, cwb->w, EBI__(DP(e)->eid));
			knh_write_text(ctx, cwb->w, "!!");
			if(loc > 0) {
				knh_write_text(ctx, cwb->w, ": ");
				knh_write(ctx, cwb->w, knh_bytes_last(S_tobytes(ename), loc+3));
			}
			KNH_SETv(ctx, DP(e)->msg, knh_cwb_newString(ctx, cwb));
		}
	}
	KNH_SETv(ctx, DP(e)->bag, bag);
	return e;
}

/* ------------------------------------------------------------------------ */

KNHAPI(knh_Exception_t*) new_Exception(Ctx *ctx, knh_String_t *msg)
{
	knh_Exception_t* e = new_(Exception);
	knh_ebi_t eid = knh_geteid(ctx, S_tobytes(msg), EBI_newid);
	DP(e)->eid = eid;
	DP(e)->flag = ctx->share->EventTBL[eid].flag;
	KNH_SETv(ctx, DP(e)->msg, msg);
	return e;
}

/* ------------------------------------------------------------------------ */

KNHAPI(knh_Exception_t*) knh_cwb_newException(Ctx *ctx, knh_cwb_t *cwb)
{
	knh_Exception_t* e = new_(Exception);
	knh_ebi_t eid = knh_geteid(ctx, knh_cwb_tobytes(cwb), EBI_newid);
	DP(e)->eid = eid;
	DP(e)->flag = ctx->share->EventTBL[eid].flag;
	KNH_SETv(ctx, DP(e)->msg, knh_cwb_newString(ctx, cwb));
	return e;
}

/* ------------------------------------------------------------------------ */

KNHAPI(knh_Exception_t*) new_Exception__T(Ctx *ctx, const char *msg)
{
	knh_Exception_t* e = new_(Exception);
	knh_bytes_t t = {{msg}, knh_strlen(msg)};
	knh_ebi_t eid = knh_geteid(ctx, t, EBI_newid);
	DP(e)->eid = eid;
	DP(e)->flag = ctx->share->EventTBL[eid].flag;
	KNH_SETv(ctx, DP(e)->msg, new_T(msg));
	return e;
}

/* ------------------------------------------------------------------------ */

int knh_Exception_isa(Ctx *ctx, knh_Exception_t *o, knh_String_t *msg)
{
	int res = 0;
	knh_ebi_t eid = knh_geteid(ctx, S_tobytes(msg), EBI_unknown);
	if(eid != EBI_unknown) {
		res = knh_expt_isa(ctx, DP(o)->eid, eid);
	}
	return res;
}

/* ------------------------------------------------------------------------ */

void knh_Exception_setCInfo(Ctx *ctx, knh_Exception_t *o, const char *file, int line)
{
	DP(o)->Cfile = knh_sfile(file);
	DP(o)->Cline = line;
}

/* ------------------------------------------------------------------------ */

void knh_Context_setThrowingException(Ctx *ctx, knh_Exception_t *e)
{
	KNH_SETv(ctx, ((knh_Context_t*)ctx)->e, e);
}

/* ------------------------------------------------------------------------ */

void knh_throw_OutOfIndex(Ctx *ctx, knh_int_t n, size_t max, const char *file, int line)
{
	knh_Exception_t *e;
	knh_cwb_t cwbbuf2, *cwb2 = knh_cwb_open(ctx, &cwbbuf2);
	knh_printf(ctx, cwb2->w, "OutOfIndex!!: %i not < %i", n, (knh_int_t)max);
	e = knh_cwb_newException(ctx, cwb2);
	knh_stack_throw(ctx, ctx->esp, e);
}

/* ======================================================================== */
/* [ExceptionHandler] */
//
//knh_ExceptionHandler_t* new_ExceptionHandler(Ctx *ctx)
//{
//	return (knh_ExceptionHandler_t*)new_Object_bcid(ctx, CLASS_ExceptionHandler, 0);
//}
//
///* ------------------------------------------------------------------------ */
//
//void knh_ExceptionHandler_longjmp(Ctx *ctx, knh_ExceptionHandler_t *o, knh_Exception_t *e)
//{
//	KNH_ASSERT(IS_Exception(e));
//	KNH_SETv(ctx, DP(o)->caught, e);
//	knh_longjmp(DP(o)->jmpbuf, ((int)DP(e)->eid));
//}
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
