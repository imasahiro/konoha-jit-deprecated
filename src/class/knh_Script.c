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
/* [constructors] */

knh_Script_t *new_Script(Ctx *ctx, knh_NameSpace_t *ns, knh_class_t cid, knh_class_t supcid)
{
	size_t i;
	knh_Script_t *script = (knh_Script_t*)new_Object_init(ctx, FLAG_Script, CLASS_Script, 0);
	knh_ClassTable_t *t = (knh_ClassTable_t*)(&ClassTable(cid));
	if(cid != CLASS_Script) {
		char buf[CLASSNAME_BUFSIZ];
		script->h.cid = cid;
		DBG2_ASSERT(ClassTable(cid).cspi == NULL);
		DBG2_ASSERT(t->cmap == NULL);
		knh_snprintf(buf, sizeof(buf), "%s%d.Script", __tochar(DP(ns)->nsname), (int)cid);
		knh_setClassName(ctx, cid, new_String(ctx, B(buf), NULL));
		t->cflag  = CFLAG_Script;
		t->oflag  = FLAG_Script;
		t->bcid   = CLASS_Script; /* CLASS_Object */
		t->supcid = supcid;
		t->offset = 0;
		t->cspi = ClassTable(CLASS_Script).cspi;
		KNH_INITv(t->methods, new_Array0(ctx, 0));
		KNH_INITv(t->cmap, ClassTable(CLASS_Script).cmap);
	}

	DBG2_ASSERT(t->fsize == 0);
	DBG2_ASSERT(t->fields == NULL);
	t->bsize  = KNH_SCRIPT_FIELDSIZE;
	t->size = t->bsize * sizeof(knh_Object_t*);
	t->fsize = KNH_SCRIPT_FIELDSIZE;
	t->fields = KNH_MALLOC(ctx, sizeof(knh_fields_t) * KNH_SCRIPT_FIELDSIZE);
	knh_bzero(t->fields, sizeof(knh_fields_t) * KNH_SCRIPT_FIELDSIZE);
	for(i=0; i < KNH_SCRIPT_FIELDSIZE; i++) {
		t->fields[i].flag  = 0;
		t->fields[i].type  = TYPE_void;
		t->fields[i].fn    = FIELDN_NONAME;
		t->fields[i].value = NULL;
	}

	DBG2_ASSERT(t->cspec == NULL);
	knh_setClassDefaultValue(ctx, cid, UP(script), NULL);
	{
		knh_Method_t *mtd = new_Method(ctx, 0, cid, METHODN_LAMBDA, NULL);
		KNH_SETv(ctx, DP(mtd)->mf, knh_findMethodField0(ctx, TYPE_Any));
		knh_Class_addMethod(ctx, cid, mtd);
	}
	return script;
}

/* ------------------------------------------------------------------------ */

knh_bool_t knh_hasScriptFunc(Ctx *ctx, char *fmt)
{
	knh_bytes_t fname = B(fmt);
	knh_index_t loc = knh_bytes_index(fname, '(');
	if(loc > 0) {
		fname = knh_bytes_first(fname, loc);
		fmt = fmt + loc + 1;
	}
	{
		knh_methodn_t mn = knh_getmn(ctx, fname, METHODN_NONAME);
		knh_Method_t *mtd = knh_Class_getMethod(ctx, CLASS_Script, mn);
		return IS_Method(mtd);
	}
}

/* ------------------------------------------------------------------------ */

knh_sfp_t *knh_invokeScriptFunc(Ctx *ctx, char *fmt, va_list args)
{
	knh_bytes_t fname = B(fmt);
	knh_index_t loc = knh_bytes_index(fname, '(');
	knh_sfp_t *lsfp = KNH_LOCAL(ctx);
	if(loc == -1) {
		knh_setRuntimeError(ctx, T__("needs ()"));
		return lsfp+1;
	}
	else {
		fname = knh_bytes_first(fname, loc);
		fmt = fmt + loc + 1;
	}

	KNH_MOV(ctx, lsfp[0].o, new_ExceptionHandler(ctx));
	KNH_TRY(ctx, L_CATCH, lsfp, 0);
	{
		knh_methodn_t mn = knh_getmn(ctx, fname, METHODN_NONAME);
		knh_Method_t *mtd = knh_lookupMethod(ctx, knh_Object_cid(ctx->script), mn);
		int n = knh_stack_vpush(ctx, lsfp + 1, fmt, args);
		KNH_MOV(ctx, lsfp[2].o, ctx->script);
		KNH_SCALL(ctx, lsfp, 1, mtd, /* args*/ n);
	}
	knh_Context_clearstack(ctx);
	((knh_Context_t*)ctx)->hasError = 0;
	return lsfp + 1;

	/* catch */
	L_CATCH:;
	KNH_PRINT_STACKTRACE(ctx, lsfp, 0);
	return lsfp + 1;
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
