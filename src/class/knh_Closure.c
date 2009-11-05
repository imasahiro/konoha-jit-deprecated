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
#include"../../include/konoha/konoha_code_.h"

size_t knh_opcode_size(int opcode);

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ======================================================================== */

int
knh_stack_vpush(Ctx *ctx, knh_sfp_t *sfp, const char *fmt, va_list args)
{
	char *p = (char*)fmt;
	int ch, n = 2;
	while((ch = *p) != '\0') {
		p++;
		switch(ch) {
		case '%':
			break;
		case 'U': case 'D':
			KNH_SETv(ctx, sfp[n].o, KNH_INT0); // NONNULL
		case 'u': case 'd':
			sfp[n].ivalue = (knh_int_t)va_arg(args, knh_intptr_t);
			n++;
			break;

		case 'I': case 'L':
			KNH_SETv(ctx, sfp[n].o, KNH_INT0); // NONNULL
		case 'i': case 'l':
			sfp[n].ivalue = (knh_int_t)va_arg(args, knh_int_t);
			n++;
			break;

		case 'F':
			KNH_SETv(ctx, sfp[n].o, KNH_FLOAT0);  // NONNULL
		case 'f':
			sfp[n].fvalue = (knh_float_t)va_arg(args, knh_float_t);
			n++;
			break;

		case 'B': case 'b':
			sfp[n].bvalue = (knh_intptr_t)va_arg(args, knh_intptr_t);
			KNH_SETv(ctx, sfp[n].o, new_Boolean(ctx, sfp[n].bvalue));
			break;

		case 's':
		{
			char *s = (char*)va_arg(args, char*);
			Object *o = NULL;
			if(s == NULL) {
				o = KNH_NULL;
			}
			else {
				o = (Object*)new_String(ctx, B(s), NULL);
			}
			KNH_SETv(ctx, sfp[n].o, o);
			n++;
		}
		break;

		case 'o':
		{
			Object *o = (Object*)va_arg(args, knh_Object_t*);
			KNH_SETv(ctx, sfp[n].o, o);
			sfp[n].data = knh_Object_data(o);
			n++;
		}
		break;
		}/*switch*/
	} /* while */
	return n;
}

/* ------------------------------------------------------------------------ */

KNHAPI(void) knh_putsfp(Ctx *ctx, knh_sfp_t *lsfp, int n, Object *obj)
{
	KNH_SETv(ctx, lsfp[n].o, obj);
	lsfp[n].data = knh_Object_data(obj);
}

/* ------------------------------------------------------------------------ */

KNHAPI(void) knh_Closure_invokesfp(Ctx *ctx, knh_Closure_t *cc, knh_sfp_t *lsfp, int argc)
{
	KNH_MOV(ctx, lsfp[1].o, (cc)->base);
	KNH_SCALL(ctx, lsfp, 0, (cc)->mtd, argc);
	KNH_LOCALBACK(ctx, lsfp);
}

/* ------------------------------------------------------------------------ */

// knh_Closure_finvoke(ctx, c, "oo", sfp[1].o, sfp[2].o)
// knh_Closure_finvoke(ctx, c, "ii", 1, 2)

KNHAPI(knh_sfp_t*) knh_Closure_invokef(Ctx *ctx, knh_Closure_t *c, const char *fmt, ...)
{
	knh_sfp_t *lsfp = KNH_LOCAL(ctx);
	int n;
	va_list args;
	va_start(args , fmt);
	n = knh_stack_vpush(ctx, lsfp, fmt, args);
	va_end(args);
	knh_Closure_invokesfp(ctx, c, lsfp, n - 2);
	return lsfp;
}

/* ------------------------------------------------------------------------ */

KNHAPI(void) knh_Closure_invoke(Ctx *ctx, knh_Closure_t *c, const char *fmt, ...)
{
	knh_sfp_t *lsfp = KNH_LOCAL(ctx);
	int n;
	va_list args;
	va_start(args , fmt);
	n = knh_stack_vpush(ctx, lsfp, fmt, args);
	va_end(args);
	knh_Closure_invokesfp(ctx, c, lsfp, n - 2);
}

/* ------------------------------------------------------------------------ */
/* class */
/* ------------------------------------------------------------------------ */

static
METHOD knh_fmethod_closureDEFAULT(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_type_t rtype = knh_Method_rztype(sfp[-1].mtd);
	if(rtype == TYPE_void) {
		KNH_RETURN_void(ctx, sfp);
	}
	KNH_RETURN(ctx, sfp, KNH_DEF(ctx, CLASS_type(rtype)));
}

/* ------------------------------------------------------------------------ */

knh_Object_t *knh_fdefault__NEWCLOSURE(Ctx *ctx, knh_class_t cid)
{
	knh_ClassTable_t *t = pClassTable(cid);
	knh_Closure_t *cc = (knh_Closure_t*)new_Object_init(ctx, FLAG_Closure, cid, 0);
	knh_Method_t *mtd = new_Method(ctx, 0, cid, METHODN_LAMBDA, knh_fmethod_closureDEFAULT);
	KNH_INITv((cc)->mtd, mtd);
	KNH_SETv(ctx, DP(mtd)->mf, knh_findMethodField0(ctx, t->r0));
	knh_Method_setVarArgs(mtd, 1);
	KNH_INITv((cc)->base, KNH_NULL);
	(cc)->envsfp = NULL;

	DBG2_P("create new default value of %s", CLASSN(cid));
	KNH_ASSERT(t->cspec == NULL);
	KNH_INITv(t->cspec, cc);
	t->fdefault = knh_fdefault__CONST;
	return t->cspec;
}

/* ------------------------------------------------------------------------ */

knh_class_t
knh_addClosureClass(Ctx *ctx, knh_class_t cid, knh_String_t *name, knh_type_t r0, knh_type_t p1, knh_type_t p2, knh_type_t p3)
{
	knh_ClassTable_t *t = NULL;
	knh_flag_t mask = 0;
	if(cid == CLASS_newid) {
		cid = knh_ClassTable_newId(ctx);
	} else {
		((knh_SharedData_t*)ctx->share)->ClassTableSize = cid;
	}
	/* knh_ClassTable_t */ t = pClassTable(cid);
	KNH_ASSERT(ClassTable(cid).sname == NULL);
	if(knh_class_isTypeVariable(CLASS_type(r0)) ||
			knh_class_isTypeVariable(CLASS_type(p1)) ||
			knh_class_isTypeVariable(CLASS_type(p2)) ||
			knh_class_isTypeVariable(CLASS_type(p3))) {
		mask = FLAG_Class_TypeVariable;
		DBG2_P("TypeVarable: %s", __tochar(name));
	}

	knh_setClassName(ctx, cid, name);
	t->cflag  = ClassTable(CLASS_Closure).cflag | mask;
	t->oflag  = ClassTable(CLASS_Closure).oflag;
	t->sid    = ClassTable(CLASS_Closure).sid;

	t->bcid   = CLASS_Closure;
	t->supcid = ClassTable(CLASS_Closure).supcid;
	t->offset = ClassTable(CLASS_Closure).offset;

	t->size = ClassTable(CLASS_Closure).size;
	t->bsize  = ClassTable(CLASS_Closure).bsize;

	KNH_INITv(t->cstruct, ClassTable(CLASS_Closure).cstruct);
	KNH_INITv(t->cmap, ClassTable(CLASS_Closure).cmap);
	t->r0 = r0;
	t->p1 = p1;
	t->p2 = p2;
	t->p3 = p3;
	StructTable(CLASS_Closure).fnewClass(ctx, cid);
	return cid;
}

/* ------------------------------------------------------------------------ */

void knh_Closure_newClass(Ctx *ctx, knh_class_t cid)
{
	knh_setClassDefaultValue(ctx, cid, NULL, knh_fdefault__NEWCLOSURE);
}


/* ------------------------------------------------------------------------ */

knh_class_t knh_class_Closure(Ctx *ctx, knh_type_t r0, knh_type_t p1, knh_type_t p2, knh_type_t p3)
{
	knh_class_t cid = CLASS_unknown;
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	if(r0 == CLASS_unknown || p1 == CLASS_unknown || p2 == CLASS_unknown|| p3 == CLASS_unknown) {
		return cid;
	}
	knh_write_ltype(ctx, cwb->w, r0);
	knh_putc(ctx, cwb->w, '(');
	knh_write_ltype(ctx, cwb->w, p1);
	if(p2 == TYPE_void) {
		goto L_CLOSE;
	}
	knh_putc(ctx, cwb->w, ',');
	knh_write_ltype(ctx, cwb->w, p2);
	if(p3 == TYPE_void) {
		goto L_CLOSE;
	}
	knh_putc(ctx, cwb->w, ',');
	knh_write_ltype(ctx, cwb->w, p3);
	L_CLOSE:;
	knh_putc(ctx, cwb->w, ')');

	/* knh_class_t*/ cid = knh_getcid(ctx, knh_cwb_tobytes(cwb));
	if(cid == CLASS_unknown) {
		cid = knh_addClosureClass(ctx, CLASS_newid, knh_cwb_newString(ctx, cwb), r0, p1, p2, p3);
	} else {
		knh_cwb_close(cwb);
	}
	return cid;
}

/* ------------------------------------------------------------------------ */

knh_class_t knh_class_MethodClosure(Ctx *ctx, knh_class_t cid, knh_Method_t *mtd)
{
	if(knh_Method_psize(mtd) > 3) return CLASS_Closure;
	knh_type_t r0 = knh_Method_rtype(ctx, mtd, cid);
	knh_type_t p1 = TYPE_void;
	if(0 < knh_Method_psize(mtd)) p1 = knh_Method_ptype(ctx, mtd, cid, 0);
	knh_type_t p2 = TYPE_void;
	if(1 < knh_Method_psize(mtd)) p2 = knh_Method_ptype(ctx, mtd, cid, 1);
	knh_type_t p3 = TYPE_void;
	if(2 < knh_Method_psize(mtd)) p3 = knh_Method_ptype(ctx, mtd, cid, 2);
	return knh_class_Closure(ctx, r0, p1, p2, p3);
}

/* ------------------------------------------------------------------------ */
/* [Closure] */
/* ------------------------------------------------------------------------ */

static
void knh_sfp_copy(Ctx *ctx, knh_sfp_t *dst, knh_sfp_t *src, size_t size)
{
	size_t i;
	for(i = 0; i < size; i++) {
		dst[i].data = src[i].data;
		KNH_MOV(ctx, dst[i].o, src[i].o);
	}
}

///* ------------------------------------------------------------------------ */
//
//knh_Closure_t* new_Closure(Ctx *ctx, knh_sfp_t *sfp, knh_Method_t *mtd)
//{
//	knh_class_t cid = knh_class_MethodClosure(ctx, knh_Object_cid(sfp[0].o), mtd);
//	knh_Closure_t *cc = (knh_Closure_t*)new_Object_init(ctx, FLAG_Closure, cid, 0);
//	KNH_INITv((cc)->mtd, mtd);
//	(cc)->self = cc;   // TO AVOID CYCLIC REFERENCE
//	(cc)->envsfp = sfp;
//	return cc;
//}

/* ------------------------------------------------------------------------ */

void knh_Closure_copyEnv(Ctx *ctx, knh_Closure_t *cc, knh_sfp_t *sfp)
{
	size_t stacksize = ctx->esp - sfp;
	size_t* hstack = (size_t*)KNH_MALLOC(ctx, (sizeof(knh_sfp_t) * stacksize) + sizeof(size_t));
	knh_sfp_t *envsfp = (knh_sfp_t*)(&hstack[1]);
	hstack[0] = stacksize;
	knh_sfp_copy(ctx, sfp, envsfp, stacksize);
	(cc)->envsfp = envsfp;
	knh_Closure_setStoredEnv(cc, 1);
	DBG2_P("STORED %d", (int)stacksize);
}

/* ------------------------------------------------------------------------ */
/* [Generator] */
/* ------------------------------------------------------------------------ */

knh_class_t knh_Method_gencid(Ctx *ctx, knh_Method_t *mtd, knh_class_t cid)
{
	knh_type_t rtype = knh_pmztype_totype(ctx, knh_Method_rztype(mtd), cid);
	cid = CLASS_type(rtype);
	DBG2_ASSERT_cid(cid);
	KNH_ASSERT(ClassTable(cid).bcid == CLASS_Iterator);
	return ClassTable(cid).p1;
}

/* ------------------------------------------------------------------------ */

static
knh_code_t *knh_KLRCode_yeildingNext(knh_code_t *pc)
{
	knh_inst_t *op = (knh_inst_t*)pc;
	if(op->opcode != OPCODE_NOP) return NULL;
	pc += knh_opcode_size(op->opcode);
	return pc;
}

/* ------------------------------------------------------------------------ */

static
ITRNEXT knh_Generator_fnext(Ctx *ctx, knh_sfp_t *sfp, int n)
{
	knh_Iterator_t *it = sfp[0].it;
	knh_Closure_t *cc = (knh_Closure_t*)DP(it)->source;
	knh_code_t *pc = knh_KLRCode_yeildingNext(DP(it)->pc);
	if(pc == NULL) {
		KNH_ITREND(ctx, sfp, n);
	}
	else {
		size_t stacksize = (cc)->hstacksize[-1];
		knh_sfp_copy(ctx, (cc)->envsfp, sfp+1, stacksize);
		KNH_CALLGEN(ctx, sfp, 1, mtd, pc, stacksize);  /* args is reset to esp size */
		if(((knh_inst_t*)pc)->opcode == OPCODE_YEILDBREAK) {
			KNH_ITREND(ctx, sfp, n);
		}
		DBG2_P("stacksize=%d, %d", (int)stacksize, (int)(cc)->hstacksize[-1]);
		knh_sfp_copy(ctx, sfp+1, (cc)->envsfp, (cc)->hstacksize[-1]);
		DP(it)->pc = pc;
		KNH_ITRNEXT_envsfp(ctx, sfp, n, (cc)->envsfp);
	}
}

/* ------------------------------------------------------------------------ */

static
knh_Iterator_t* new_Generator(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Closure_t *cc = (knh_Closure_t*)new_Object_init(ctx, FLAG_Closure, CLASS_Closure, 0);
	KNH_INITv((cc)->mtd, sfp[-1].mtd);
	KNH_INITv((cc)->base, sfp[0].o);
	{
		knh_class_t cid = knh_Method_gencid(ctx, sfp[-1].mtd, knh_Object_cid(sfp[0].o));
		knh_Iterator_t *it = new_Iterator(ctx, cid, UP(cc), knh_Generator_fnext);
		knh_code_t *pc = (sfp[-1].mtd)->pc_start;
		KNH_ASSERT(((klr_CHKESP_t*)pc)->opcode == OPCODE_CHKESP);
		size_t stacksize = 1 + ((klr_CHKESP_t*)pc)->a1;
		size_t* hstack = (size_t*)KNH_MALLOC(ctx, (sizeof(knh_sfp_t) * stacksize) + sizeof(size_t));
		knh_sfp_t *envsfp = (knh_sfp_t*)(&hstack[1]);
		hstack[0] = stacksize;
		knh_sfp_copy(ctx, sfp - 1, envsfp, stacksize);
		(cc)->envsfp = envsfp;
		knh_Closure_setStoredEnv(cc, 1);
		DBG2_P("STORED %d", (int)stacksize);
		DP(it)->pc = pc;
		return it;
	}
}

/* ------------------------------------------------------------------------ */

static
METHOD knh_fmethod_generator(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN(ctx, sfp, new_Generator(ctx, sfp));
}

///* ------------------------------------------------------------------------ */
//
//int knh_Method_isGenerator(Method *mtd)
//{
//	return (mtd->fcall_1 == knh_fmethod_generator);
//}

/* ------------------------------------------------------------------------ */

void knh_Method_toGenerator(knh_Method_t *mtd)
{
	mtd->fcall_1 = knh_fmethod_generator;
}

/* ======================================================================== */
/* [Thunk] */

static
knh_class_t knh_addThunkClass(Ctx *ctx, knh_class_t cid, knh_String_t *name, knh_type_t rtype)
{
	knh_ClassTable_t *t = NULL;
	knh_flag_t mask = 0;
	if(cid == CLASS_newid) {
		cid = knh_ClassTable_newId(ctx);
	} else {
		((knh_SharedData_t*)ctx->share)->ClassTableSize = cid;
	}
	/* knh_ClassTable_t */ t = pClassTable(cid);
	KNH_ASSERT(ClassTable(cid).sname == NULL);
	if(knh_class_isTypeVariable(CLASS_type(rtype))) {
		mask = FLAG_Class_TypeVariable;
		DBG2_P("TypeVarable: %s", __tochar(name));
	}
	knh_setClassName(ctx, cid, name);
	t->cflag  = ClassTable(CLASS_Thunk).cflag | mask;
	t->oflag  = ClassTable(CLASS_Thunk).oflag;
	t->sid    = ClassTable(CLASS_Thunk).sid;

	t->bcid   = CLASS_Thunk;
	t->supcid = ClassTable(CLASS_Thunk).supcid;
	t->offset = ClassTable(CLASS_Thunk).offset;

	t->size = ClassTable(CLASS_Thunk).size;
	t->bsize  = ClassTable(CLASS_Thunk).bsize;

	KNH_INITv(t->cstruct, ClassTable(CLASS_Thunk).cstruct);
	KNH_INITv(t->cmap, new_ClassMap0(ctx, 0));
	t->p1 = rtype;
	t->p2 = CLASS_Tvoid;
	StructTable(CLASS_Thunk).fnewClass(ctx, cid);
	return cid;
}

/* ------------------------------------------------------------------------ */

knh_class_t knh_class_Thunk(Ctx *ctx, knh_type_t rtype)
{
	knh_class_t cid = CLASS_Thunk;
	if(CLASS_type(rtype) != CLASS_Any) {
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		knh_write_cid(ctx, cwb->w, CLASS_Thunk);
		knh_putc(ctx, cwb->w, '<');
		knh_write_ltype(ctx, cwb->w, rtype);
		knh_putc(ctx, cwb->w, '>');
		/* knh_class_t*/ cid = knh_getcid(ctx, knh_cwb_tobytes(cwb));
		if(cid == CLASS_unknown) {
			cid = knh_addThunkClass(ctx, CLASS_newid, knh_cwb_newString(ctx, cwb), rtype);
		} else {
			knh_cwb_close(cwb);
		}
	}
	return cid;
}

/* ------------------------------------------------------------------------ */

knh_Thunk_t* new_Thunk(Ctx *ctx, knh_type_t rtype, knh_sfp_t *sfp, size_t size)
{
	knh_class_t cid = knh_class_Thunk(ctx, rtype);
	knh_Thunk_t *thk = (knh_Thunk_t*)new_Object_init(ctx, FLAG_Thunk, cid, 0);
	if(size == 1) {
		(thk)->envsize = 1;
		(thk)->envsfp = (knh_sfp_t*)KNH_MALLOC(ctx, sizeof(knh_sfp_t) * (thk)->envsize);
		KNH_INITv((thk)->envsfp[0].o, sfp[0].o);
		(thk)->envsfp[0].data = sfp[0].data;
		knh_Thunk_setEvaluated(thk, 1);
	}
	else {
		size_t i;
		DBG2_ASSERT(IS_Method(sfp[0].mtd));
		(thk)->envsize = 1 + size;
		(thk)->envsfp = (knh_sfp_t*)KNH_MALLOC(ctx, sizeof(knh_sfp_t) * (thk)->envsize);
		for(i = 1; i < (thk)->envsize; i++) {
			KNH_INITv((thk)->envsfp[i].o, sfp[i-1].o);
			(thk)->envsfp[i].data = sfp[i-1].data;
		}
		KNH_INITv((thk)->envsfp[0].o, KNH_NULL);
		(thk)->envsfp[0].data = 0;
	}
	return thk;
}

/* ------------------------------------------------------------------------ */


#ifdef __cplusplus
}
#endif
