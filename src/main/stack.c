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

/* ************************************************************************ */

#include"commons.h"


/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ======================================================================== */
/* [argc] */

int knh_stack_argc(Ctx *ctx, knh_sfp_t *v)
{
	return (((knh_Context_t*)ctx)->esp - v);
}

/* ------------------------------------------------------------------------ */

knh_Array_t* knh_stack_toArray(Ctx *ctx, knh_sfp_t *sfp, knh_class_t cid)
{
	DBG2_ASSERT_cid(cid);
	if(cid == CLASS_Any) {
		knh_Array_t *a = new_Array0(ctx, ctx->esp - sfp);
		while(sfp < ctx->esp) {
			knh_stack_boxing(ctx, sfp);
			knh_Array_add(ctx, a, sfp->o);
			sfp++;
		}
		return a;
	}
	else if(cid == CLASS_Int || ClassTable(cid).bcid == CLASS_Int) {
		knh_IArray_t *a = new_IArray(ctx, cid, ctx->esp - sfp);
		while(sfp < ctx->esp) {
			if(IS_NOTNULL(sfp->o)) {
				knh_IArray_add(ctx, a, sfp->ivalue);
			}
			sfp++;
		}
		return (knh_Array_t*)a;
	}
	else if(cid == CLASS_Float || ClassTable(cid).bcid == CLASS_Float) {
		knh_FArray_t *a = new_FArray(ctx, cid, ctx->esp - sfp);
		while(sfp < ctx->esp) {
			if(IS_NOTNULL(sfp->o)) {
				knh_FArray_add(ctx, a, sfp->fvalue);
			}
			sfp++;
		}
		return (knh_Array_t*)a;
	}
	else {
		knh_Array_t *a = new_Array(ctx, cid, ctx->esp - sfp);
		while(sfp < ctx->esp) {
			if(IS_NOTNULL(sfp->o)) {
				knh_stack_boxing(ctx, sfp);
				knh_Array_add(ctx, a, sfp->o);
			}
			sfp++;
		}
		return a;
	}
}

/* ------------------------------------------------------------------------ */

KNHAPI(void) knh_stack_boxing(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_class_t bcid = (sfp[0].o)->h.bcid;
	if(CLASS_Boolean <= bcid && bcid <= CLASS_Float && sfp[0].data != knh_Object_data(sfp[0].o)) {
		KNH_MOV(ctx, sfp[0].o, new_Object_boxing(ctx, knh_Object_cid(sfp[0].o), sfp));
	}
}

/* ------------------------------------------------------------------------ */

KNHAPI(void) knh_stack_unboxing(Ctx *ctx, knh_sfp_t *sfp)
{
	sfp[0].data = knh_Object_data(sfp[0].o);
}

/* ------------------------------------------------------------------------ */

KNHAPI(void) knh_stack_w(Ctx *ctx, knh_sfp_t *sfp, knh_sfp_t *one, knh_methodn_t mn, knh_OutputStream_t *w, Any *m)
{
	knh_Method_t *mtd = knh_lookupFormatter(ctx, knh_Object_cid(one[0].o), mn);
	KNH_SETv(ctx, sfp[1].o, one[0].o);
	sfp[1].data = one[0].data;
	KNH_SETv(ctx, sfp[2].o, w);
	KNH_SETv(ctx, sfp[3].o, m);
	KNH_SCALL(ctx, sfp, 0, mtd, /*args*/2);
}

/* ======================================================================== */
/* [call] */

void knh_stack_typecheck(Ctx *ctx, knh_sfp_t *sfp, knh_Method_t *mtd, knh_code_t *pc)
{
	DBG2_ASSERT(IS_Method(sfp[-1].mtd));
	char *emsg;
	knh_class_t this_cid = knh_Object_cid(sfp[0].o);
	int i, argc = knh_Method_isVarArgs(mtd) ? (ctx->esp - sfp) : knh_Method_psize(mtd);
	for(i = 1; i < argc; i++) {
		knh_type_t type = knh_Method_ptype(ctx, mtd, this_cid, i - 1);
		if(IS_NULL(sfp[i].o)) {
			if(IS_NNTYPE(type)) {
				emsg = "Null!!: the parameter %d of %M"; goto L_THROWERR;
			}
		}
		else {
			knh_class_t reqc = CLASS_type(type);
			if(!knh_class_instanceof(ctx, knh_Object_cid(sfp[i].o), reqc)) {
				emsg = "Type!!: the parameter %d of %M"; goto L_THROWERR;
			}
		}
	}
	return;

	L_THROWERR:
	{
		char *file = (pc != NULL) ? knh_Method_file(ctx, sfp[-1].mtd) : NULL;
		int line = (pc != NULL) ? knh_Method_pcline(sfp[-1].mtd, pc) : 0;
		knh_Exception_t *e;
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		knh_printf(ctx, cwb->w, emsg, (knh_intptr_t)i, DP(mtd)->mn);
		e = knh_cwb_newException(ctx, cwb);
		knh_stack_throw(ctx, ctx->esp, e, file, line);
	}
}

/* ======================================================================== */
/* [throw] */

knh_sfp_t* knh_stack_callee(Ctx *ctx, knh_sfp_t *sfp, char **file, int *linenum)
{
	while(ctx->stack <= sfp) {
		if(IS_Method(sfp[0].mtd)) {
			knh_code_t *pc = sfp[0].pc;
			DBG2_P("sfp[%d] pc=%p callee yes=%d", sfp - ctx->stack, pc, sfp[0].data != knh_Object_data(sfp[0].mtd));
			if(pc == NULL) {
				return sfp;
			}
			if(sfp[0].data != knh_Object_data(sfp[0].mtd)) {
				*linenum = knh_Method_pcline(sfp[0].mtd, pc);
				*file = knh_Method_file(ctx, sfp[0].mtd);
				return sfp;
			}
		}
		sfp--;
	}
	return ctx->stack;
}

static
void knh_stack_writeStackTrace(Ctx *ctx, knh_sfp_t *sfp, knh_OutputStream_t *w)
{
	knh_Method_t *mtd = sfp[-1].mtd;
	knh_write_cid(ctx, w, DP(mtd)->cid);
	if(DP(mtd)->mn != METHODN_LAMBDA) {
		int i = 0, psize = knh_Method_psize(mtd);
		knh_putc(ctx, w, '.');
		knh_write_mn(ctx, w, knh_Method_rztype(mtd), DP(mtd)->mn);
		knh_putc(ctx, w, '(');
		for(i = 0; i < psize; i++) {
			knh_mparam_t p = knh_Method_param(mtd, i);
			p.type = knh_Method_ptype(ctx, mtd, knh_Object_cid(sfp[0].o), i);
			if(i > 0) {
				knh_putc(ctx, w, ',');
			}
			knh_write_fn(ctx, w, p.fn);
			knh_putc(ctx, w, '=');
			if(IS_ubxtype(p.type)) {
				knh_write_ifmt(ctx, w, KNH_INT_FMT, sfp[i+1].ivalue);
			}
			else if (IS_ubxfloat(p.type)) {
				knh_write_ifmt(ctx, w, KNH_FLOAT_FMT, sfp[i+1].fvalue);
			}
			else if(p.type == NNTYPE_Boolean){
				knh_write_bool(ctx, w, sfp[i+1].bvalue);
			}
			else {
				knh_stack_w(ctx, ctx->esp, &sfp[i+1], METHODN__k, w, KNH_NULL);
			}
		}
		knh_putc(ctx, w, ')');
	}
}


static
void knh_stack_writeReturnValue(Ctx *ctx, knh_sfp_t *sfp, knh_OutputStream_t *w, knh_type_t rtype)
{
	if(rtype == TYPE_void) {
		knh_write(ctx, w, STEXT("void"));
	}
	if(IS_ubxtype(rtype)) {
		knh_write_ifmt(ctx, w, KNH_INT_FMT, sfp[-1].ivalue);
	}
	else if (IS_ubxfloat(rtype)) {
		knh_write_ifmt(ctx, w, KNH_FLOAT_FMT, sfp[-1].fvalue);
	}
	else if(rtype == NNTYPE_Boolean){
		knh_write_bool(ctx, w, sfp[-1].bvalue);
	}
	else {
		knh_stack_w(ctx, ctx->esp, &sfp[-1], METHODN__k, w, KNH_NULL);
	}
}

/* ------------------------------------------------------------------------ */

METHOD knh_Fmethod_stackTrace(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	knh_Method_t *mtd = sfp[-1].mtd;
	knh_OutputStream_t *w = KNH_STDERR;
	knh_intptr_t count = DP(mtd)->prof_count;
	knh_sfp_t *esp = ctx->esp;
	knh_type_t rtype = knh_Method_rztype(mtd);
	knh_putc(ctx, w, '#');
	knh_write_ifmt(ctx, w, KNH_INT_FMT, (knh_int_t)count);
	knh_putc(ctx, w, ':');
	knh_stack_writeStackTrace(ctx, sfp, w);
	knh_write_EOL(ctx, w);
	DBG2_ASSERT(ctx->esp == esp);
	DP(mtd)->prof_count += 1;
	DP(mtd)->fproceed(ctx, sfp);
	knh_putc(ctx, w, '#');
	knh_write_ifmt(ctx, w, KNH_INT_FMT, (knh_int_t)count);
	knh_write(ctx, w, STEXT(":-> "));
	knh_stack_writeReturnValue(ctx, sfp, w, rtype);
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

static
knh_sfp_t *knh_stack_addStackTrace(Ctx *ctx, knh_sfp_t *sfp, knh_Exception_t *e)
{
	char *file = "-";
	int  line = 0;
	knh_sfp_t *callee = knh_stack_callee(ctx, sfp, &file, &line);
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_write_fline(ctx, cwb->w, file, line);
	knh_putc(ctx, cwb->w, ':');
	knh_stack_writeStackTrace(ctx, sfp, cwb->w);
	{
		knh_String_t *msg = knh_cwb_newString(ctx, cwb);
		if(IS_NULL(DP(e)->traces)) {
			KNH_SETv(ctx, DP(e)->traces, new_Array(ctx, CLASS_String, 16));
		}
		knh_Array_add(ctx, DP(e)->traces, UP(msg));
	}
	return callee;
}

/* ------------------------------------------------------------------------ */

KNHAPI(void) knh_stack_throw(Ctx *ctx, knh_sfp_t *sfp, knh_Exception_t *e, char *file, int line)
{
	DBG2_ASSERT(IS_Exception(e));
	knh_sfp_t *sp = (sfp == NULL) ? ctx->esp : sfp;
	DBG2_P("throwing %s at %d esp=%d", __tochar(DP(e)->msg), sp - ctx->stack, ctx->esp - ctx->stack);
	KNH_SETv(ctx, ctx->stack[ctx->stacksize-1].o, e);  // TO AVOID GC
	if(DP(e)->line != 0) {
		if(line == 0) {
			knh_stack_callee(ctx, sp, &file, &line);
		}
		DP(e)->file = file;
		DP(e)->line = line;
	}
	while(ctx->stack <= sp) {
		if(IS_ExceptionHandler(sp[0].hdr) && knh_ExceptionHandler_isCatching(sp[0].hdr)) {
			knh_ExceptionHandler_setCatching(sp[0].hdr, 0);
			knh_ExceptionHandler_longjmp(ctx, sp[0].hdr, e);
		}
		else if(IS_Method(sp[0].o) && sp[0].data != knh_Object_data(sp[0].mtd)) {
			knh_stack_addStackTrace(ctx, sp+1, e);
		}
		sp--;
	}
	fprintf(stderr, "********** USE STACKTRACE IN YOUR C/C++ DEBUGGER ************\n");
	fprintf(stderr, "Uncaught Exception: %s\n", __tochar(DP(e)->msg));
	fprintf(stderr, "*************************************************************\n");
	exit(0);
}

///* ------------------------------------------------------------------------ */
//
//KNHAPI(void) konoha_throwSecurityException(void)
//{
//	Ctx *ctx = knh_getCurrentContext();
//	knh_stack_throw(ctx, new_Exception(ctx, knh_getExptName(ctx, EXPT_Security)), NULL, 0);
//}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
