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

#include"commons.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef K_USING_DEFAULTAPI

/* ------------------------------------------------------------------------ */
/* [format] */

static const char *newfmt(char *buf, size_t bufsiz, knh_bytes_t fmt, const char *t)
{
	char *p = buf + (fmt.len - 1);
	strncpy(buf, fmt.text, bufsiz);
	strncpy(p, t, bufsiz - fmt.len);
	DBG_P("FMT='%s'", buf);
	return (const char*)buf;
}

/* ------------------------------------------------------------------------ */
//## @Const @FastCall method String Int.format(String fmt);

static METHOD Int_format(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_sfp_t *arg = ctx->esp - 1;
	DBG_ASSERT(arg == sfp+1);
	knh_bytes_t fmt = S_tobytes(arg[0].s);
	L_RETRY:;
	int ch = fmt.ustr[fmt.len - 1];
	if(fmt.ustr[0] == '%' && (ch == 'u' || ch == 'd' || ch == 'x')) {
		char fmtbuf[40], buf[80];
		const char *ifmt = (ch == 'd') ? K_INT_FMT : ((ch == 'x') ? K_INT_XFMT : K_UINT_FMT);
		knh_snprintf(buf, sizeof(buf), newfmt(fmtbuf, sizeof(fmtbuf), fmt, ifmt + 1), sfp[0].ivalue);
		RETURN_(new_S(ctx, B(buf)));
	}
	if(fmt.len != 0) {
		KNH_SYSLOG(ctx, LOG_WARNING, "Format!!", "invalid format: %s", fmt.text);
	}
	fmt = STEXT("%d");
	goto L_RETRY;
}

/* ------------------------------------------------------------------------ */
//## @Const @FastCall method String Float.format(String fmt);

static METHOD Float_format(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_sfp_t *arg = ctx->esp - 1;
	DBG_ASSERT(arg == sfp+1);
	knh_bytes_t fmt = S_tobytes(arg[0].s);
	L_RETRY:;
	int ch = fmt.ustr[fmt.len - 1];
	if(fmt.ustr[0] == '%' && (ch == 'f' || ch == 'e')) {
		char buf[80];
		const char *ifmt = (ch == 'f') ? K_FLOAT_FMT : K_FLOAT_FMTE;
		knh_snprintf(buf, sizeof(buf), ifmt, sfp[0].fvalue);
		RETURN_(new_S(ctx, B(buf)));
	}
	if(fmt.len != 0) {
		KNH_SYSLOG(ctx, LOG_WARNING, "Format!!", "invalid format: %s", fmt.text);
	}
	fmt = STEXT("%f");
	goto L_RETRY;
}

/* ------------------------------------------------------------------------ */
//## @Const @FastCall method String String.format(String fmt);

static METHOD String_format(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_sfp_t *arg = ctx->esp - 1;
	DBG_ASSERT(arg == sfp+1);
	knh_bytes_t fmt = S_tobytes(arg[0].s);
	if(fmt.ustr[0] == '%' && fmt.ustr[fmt.len-1] == 's') {
		char buf[256];
		knh_snprintf(buf, sizeof(buf), fmt.text, ctx->api->tochar(ctx, sfp[0].s));
		RETURN_(new_S(ctx, B(buf)));
	}
	if(fmt.len != 0) {
		KNH_SYSLOG(ctx, LOG_WARNING, "Format!!", "invalid format: %s", fmt.text);
	}
	RETURN_(sfp[0].s);
}

/* ------------------------------------------------------------------------ */
/* [utils] */

static knh_bool_t _isRecuriveFormatting(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_sfp_t *sp = sfp - 3;
	while(ctx->stack < sp) {
		if(sp[0].callmtd == sfp[0].callmtd && sp[1].w == sfp[1].w && sp[2].o == sfp[2].o) {
			return 1;
		}
		sp--;
	}
	return 0;
}

static void knh_stack_reformat(Ctx *ctx, knh_sfp_t *sfp, knh_methodn_t fmt)
{
	knh_Method_t *mtd = knh_getSystemFormatter(ctx, knh_Object_cid(sfp[1].o), fmt);
	mtd->fcall_1(ctx, sfp, K_RTNIDX);
}

static knh_ndata_t knh_Object_ndata(void *p)
{
	if(sizeof(knh_bool_t) < sizeof(knh_ndata_t)) {
		return ((knh_bool_t*)p)[0];
	}
	if(sizeof(knh_int_t) < sizeof(knh_ndata_t)) {
		return ((knh_int_t*)p)[0];
	}
	return ((knh_ndata_t*)p)[0];
}

static void knh_write_ObjectField(Ctx *ctx, knh_OutputStream_t *w, Object **v, size_t i, knh_type_t type, knh_methodn_t mn)
{
	if(!knh_write_ndata(ctx, w, CLASS_type(type), knh_Object_ndata(v+i))) {
		knh_Method_t *mtd = knh_getSystemFormatter(ctx, CLASS_type(type), mn);
		knh_write_Object(ctx, w, ctx->esp, &mtd, v[i]);
	}
}

/* ======================================================================== */
/* [%empty] */

#define WWW sfp[0].w

//## method void Object.%empty();

static METHOD Object__empty(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	if(knh_Context_isDebug(ctx)) {
		KNH_SYSLOG(ctx, LOG_WARNING, "Format!!", "*%%empty(%s)", O__(sfp[1].o));
	}
}

/* ======================================================================== */
/* [%s] */

//## method void Object.%s();

static METHOD Object__s(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	if(IS_NULL(sfp[1].o)) {
		knh_write(ctx, WWW, STEXT("null"));
	}
	else {
		knh_write_text(ctx, WWW, CLASS__(knh_Object_cid(sfp[1].o)));
		knh_putc(ctx, WWW, ':');
		knh_write__p(ctx, WWW, (void*)sfp[1].o);
	}
}

/* ------------------------------------------------------------------------ */
//## method void Boolean.%s();

static METHOD Boolean__s(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_write_bool(ctx, WWW, sfp[1].bvalue);
}

/* ------------------------------------------------------------------------ */
//## method void Int.%s();
//## method void Int.%d();

static METHOD Int__s(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_write_ifmt(ctx, WWW, K_INT_FMT, sfp[1].ivalue);
}

/* ------------------------------------------------------------------------ */
//## method void Float.%s();
//## method void Float.%f();

static METHOD Float__s(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_write_ffmt(ctx, WWW, K_FLOAT_FMT, sfp[1].fvalue);
}

/* ------------------------------------------------------------------------ */

static void knh_write_quote(Ctx *ctx, knh_OutputStream_t *w, knh_bytes_t t, int quote)
{
	knh_bytes_t sub = t;
	size_t i, s = 0;
	knh_putc(ctx, w, quote);
	for(i = 0; i < t.len; i++) {
		int ch = t.ustr[i];
		if(ch == '\t' || ch == '\n' || ch == '\r' || ch == '\\' || ch == quote) {
			sub.ustr = t.ustr + s;
			sub.len = i - s;
			knh_print(ctx, w, sub); s = i + 1;
			knh_putc(ctx, w, '\\');
			if(ch == '\t') {
				knh_putc(ctx, w, 't');
			}
			else if(ch == '\n') {
				knh_putc(ctx, w, 'n');
			}
			else if(ch == '\r') {
				knh_putc(ctx, w, 'r');
			}
			else {
				knh_putc(ctx, w, ch);
			}
		}
	}
	if (s < t.len) {
		sub.ustr = t.ustr + s;
		sub.len = t.len - s;
		knh_print(ctx, w, sub);
	}
	knh_putc(ctx, w, quote);
}

/* ------------------------------------------------------------------------ */
//## method void String.%s();

static METHOD String__s(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_print(ctx, WWW, S_tobytes(sfp[1].s));
}

/* ------------------------------------------------------------------------ */
//## method void Bytes.%s();

static METHOD Bytes__s(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_printf(ctx, WWW, "byte[%d]", (sfp[1].ba)->bu.len);
}

/* ------------------------------------------------------------------------ */
//## method void Regex.%s();

static METHOD Regex__s(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_write_quote(ctx, WWW, S_tobytes((sfp[1].re)->pattern), '/');
}

/* ------------------------------------------------------------------------ */
//## method void Class.%s();

static METHOD Class__s(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_write_ltype(ctx, WWW, knh_Class_cid(sfp[1].c));
}

/* ------------------------------------------------------------------------ */
//## method void Method.%s();

static METHOD Method__s(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_write_sname(ctx, WWW, DP(sfp[1].mtdOBJ)->cid);
	knh_putc(ctx, WWW, '.');
	knh_write_mn(ctx, WWW, DP(sfp[1].mtdOBJ)->mn);
}

/* ------------------------------------------------------------------------ */
//## method void Exception.%s();

static METHOD Exception__s(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_write_text(ctx, WWW, EBI__(DP(sfp[1].e)->eid));
	knh_write_text(ctx, WWW, "!!");
}

/* ======================================================================== */
/* [%k] */

/* ------------------------------------------------------------------------ */
//## method void Object.%k();

static METHOD Object__k(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	Object *o = sfp[1].o;
	knh_OutputStream_t *w = WWW;
	if(knh_Object_isNullObject(o)) {
		knh_write(ctx, w, STEXT("null"));
	}
	else if(o->h.cid == CLASS_Object || o->h.bcid != CLASS_Object) {
		knh_stack_reformat(ctx, sfp, MN__s);
	}
	else if(_isRecuriveFormatting(ctx, sfp)) {
		knh_write_dots(ctx, w);
	}
	else {
		size_t bsize = ClassTBL(o->h.cid).size / sizeof(Object*);
		knh_write_sname(ctx, w, o->h.cid);
		if(bsize > 0) {
			size_t i;
			Object **v = (Object**)o->ref;
			knh_putc(ctx, w, '{');
			for(i = 0; i < bsize; i++) {
				knh_fields_t *cf = knh_Class_fieldAt(ctx, knh_Object_cid(o), i);
				if(cf->fn == FN_NONAME) break;
				if(cf->fn == FN_/*register*/) continue;
				if(i > 0) {
					knh_write_delim(ctx, w);
				}
				knh_write_fn(ctx, w, cf->fn);
				knh_write(ctx, w, STEXT(": "));
				knh_write_ObjectField(ctx, w, v, i, cf->type, MN__k);
			}
			knh_putc(ctx, w, '}');
		}
	}
}

/* ------------------------------------------------------------------------ */
//## method void Int.%k();

static METHOD Int__k(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
#if defined(K_USING_SEMANTICS)
	knh_Semantics_t *u = knh_getSemantics(ctx, knh_Object_cid(sfp[1].o));
	knh_write_intx(ctx, WWW, u, sfp[1].ivalue);
#else
	knh_write_ifmt(ctx, WWW, K_INT_FMT, sfp[1].ivalue);
#endif
}

/* ------------------------------------------------------------------------ */
//## method void Float.%k();

static METHOD Float__k(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
#if defined(K_USING_SEMANTICS)
	knh_Semantics_t *u = knh_getSemantics(ctx, knh_Object_cid(sfp[1].o));
	knh_write_floatx(ctx, WWW, u, sfp[1].fvalue);
#else
	knh_write_ffmt(ctx, WWW, K_FLOAT_FMT, sfp[1].fvalue);
#endif
}

/* ------------------------------------------------------------------------ */
//## method void String.%k();

static METHOD String__k(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_write_quote(ctx, WWW, S_tobytes(sfp[1].s), '"');
}

/* ------------------------------------------------------------------------ */
//## method void Iterator.%k();

static METHOD Iterator__k(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_OutputStream_t *w = WWW;
	knh_Iterator_t *it = sfp[1].it;
	knh_ParamArray_t *pa = ClassTBL(knh_Object_cid(it)).cparam;
	knh_class_t p1 = knh_Object_p1(it);
	knh_Method_t *mtdf = knh_getSystemFormatter(ctx, p1, MN__k);
	if(pa->psize > 1) {
		TODO();
	}
	if(pa->psize == 1) {
		size_t c = 0;
		while(it->fnext_1(ctx, sfp+1, /*1+*/1)) {
			if(c > 0) {
				knh_write_EOL(ctx, w);
			}
			if(IS_Tint(p1)) {
				knh_write_ifmt(ctx, w, K_INT_FMT, sfp[2].ivalue);
			}
			else if(IS_Tfloat(p1)) {
				knh_write_ffmt(ctx, w, K_FLOAT_FMT, sfp[2].fvalue);
			}
			else {
				knh_write_Object(ctx, w, sfp+4, &mtdf, sfp[2].o);
			}
			c++;
		}
	}
}

///* ------------------------------------------------------------------------ */
////## method void Tuple.%k();
//
//static METHOD Tuple__k(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
////	knh_Tuple_t *o = sfp[1].tuple;
//	knh_OutputStream_t *w = WWW;
//	knh_putc(ctx, w, '(');
//	if(_isRecuriveFormatting(ctx, sfp)) {
//		knh_write_dots(ctx,w);
//	}
//	knh_putc(ctx, w, ')');
//}
//
///* ------------------------------------------------------------------------ */
////## method void Range.%k();
//
//static METHOD Range__k(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
////	knh_Range_t *o = sfp[1].range;
//	knh_OutputStream_t *w = WWW;
//	knh_putc(ctx, w, '(');
//	TODO();
//	knh_putc(ctx, w, ')');
//}

/* ------------------------------------------------------------------------ */
//## method void Array.%k();

static METHOD Array__k(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_OutputStream_t *w = WWW;
	knh_putc(ctx, w, '[');
	if(_isRecuriveFormatting(ctx, sfp)) {
		knh_write_dots(ctx,w);
	}
	else {
		knh_Array_t *a = sfp[1].a;
		knh_class_t p1 = knh_Object_p1(a);
		size_t c;
		if(IS_Tint(p1)) {
			for(c = 0; c < knh_Array_size(a); c++) {
				if(c > 0) knh_write_delim(ctx, w);
				knh_write_ifmt(ctx, w, K_INT_FMT, a->ilist[c]);
			}
		}
		else if(IS_Tfloat(p1)) {
			for(c = 0; c < knh_Array_size(a); c++) {
				if(c > 0) knh_write_delim(ctx, w);
				knh_write_ffmt(ctx, w, K_FLOAT_FMT, a->flist[c]);
			}
		}
		else if(knh_Array_size(a) > 0) {
			knh_Method_t *mtd = knh_getSystemFormatter(ctx, knh_Object_cid(a->list[0]), MN__k);
			for(c = 0; c < knh_Array_size(a); c++) {
				if(c > 0) knh_write_delim(ctx, w);
				knh_write_Object(ctx, w, ctx->esp, &mtd, a->list[c]);
			}
		}
	}
	knh_putc(ctx, w, ']');
}

/* ------------------------------------------------------------------------ */
//## method void Map.%k();

static METHOD Map__k(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	KNH_TODO(__FUNCTION__);
//	knh_OutputStream_t *w = WWW;
//	knh_putc(ctx, w, '{');
//	if(knh_stack_isRecuriveFormatting(ctx, sfp)) {
//		knh_write_dots(ctx, w);
//	}
//	else {
//		knh_DictMap_t *o = sfp[1].dmap;
//		knh_DictMap_sort(o);
//		if(knh_DictMap_size(o) > 0) {
//			size_t c;
//			knh_Method_t *mtd = knh_getSystemFormatter(ctx, knh_Object_cid(o->list[0].value), MN__k);
//			for(c = 0; c < o->size; c++) {
//				if(c > 0) knh_write_delim(ctx, w);
//				knh_write(ctx, w, S_tobytes(o->list[c].key));
//				knh_putc(ctx, w, ':');	knh_putc(ctx, w, ' ');
//				knh_write_Object(ctx, w, ctx->esp, &mtd, o->list[c].value);
//			}
//		}
//	}
//	knh_putc(ctx, w, '}');
}

/* ------------------------------------------------------------------------ */
//## method void Class.%k();

static METHOD Class__k(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_write_cid(ctx, WWW, ((knh_Class_t*)sfp[1].o)->type);
}

/* ------------------------------------------------------------------------ */
//## method void Method.%k();

static METHOD Method__k(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Method_t *o = sfp[1].mtdOBJ;
	knh_OutputStream_t *w = WWW;
	int isInline = IS_NULL(sfp[2].o) ? 1 : 0;
	if(!isInline && knh_Method_isAbstract(o)) {
		knh_write(ctx, w, STEXT("@Abstract"));
		knh_putc(ctx, w, ' ');
	}
	if(knh_ParamArray_rtype(DP(o)->mp) == TYPE_void) {
		knh_write(ctx, w, S_tobytes(TS_void));
	}else{
		knh_write_type(ctx, w, knh_ParamArray_rtype(DP(o)->mp));
	}
	knh_putc(ctx, w, ' ');
	Method__s(ctx, sfp, rix);
	knh_putc(ctx, w, '(');
	size_t i;
	for(i = 0; i < knh_Method_psize(o); i++) {
		knh_param_t *p = knh_ParamArray_get(DP(o)->mp, i);
		if(i > 0) {
			knh_write_delim(ctx, w);
		}
		knh_write_type(ctx, w, p->type);
		if(!isInline) {
			knh_putc(ctx, w, ' ');
			knh_write(ctx, w, B(FN__(p->fn)));
		}
	}
	if(knh_ParamArray_isVARGs(DP(o)->mp)) {
		knh_write_delim(ctx, w);
		knh_write_dots(ctx, w);
	}
	knh_putc(ctx, w, ')');
}

/* ------------------------------------------------------------------------ */
//## method void Translator.%k();

static METHOD Translator__k(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Translator_t *o = sfp[1].trlOBJ;
	knh_OutputStream_t *w = WWW;
	knh_write_sname(ctx, w, SP(o)->scid);
	knh_write(ctx, w, STEXT("=>"));
	knh_write_sname(ctx, w, SP(o)->tcid);
}

///* ------------------------------------------------------------------------ */
////## method void Exception.%k();
//
//static METHOD Exception__k(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	knh_Exception_t *o = sfp[1].e;
//	knh_OutputStream_t *w = WWW;
//	knh_write
//	knh_write_quote(ctx, w, S_tobytes(DP(o)->msg), '\'');
//}

/* ------------------------------------------------------------------------ */
//## method void InputStream.%k();

static METHOD InputStream__k(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_write_quote(ctx, WWW, S_tobytes(DP((knh_InputStream_t*)sfp[1].o)->urn), '\'');
}

/* ------------------------------------------------------------------------ */
//## method void OutputStream.%k();

static METHOD OutputStream__k(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_write_quote(ctx, WWW, S_tobytes(DP(sfp[1].w)->urn), '\'');
}

/* ------------------------------------------------------------------------ */
//## method void Script.%k();

static METHOD Script__k(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	int isInline = IS_NULL(sfp[2].o) ? 1 : 0;
	if(isInline) {
		knh_write(ctx, WWW, STEXT("(Script)"));
	}
	else {
		size_t i;
		knh_Script_t *o = (knh_Script_t*)sfp[1].o;
		knh_OutputStream_t *w = WWW;
		for(i = 0; i < o->fsizeUSED; i++) {
			knh_fields_t *cf = knh_Class_fieldAt(ctx, knh_Object_cid(o), i);
			if(cf == NULL) break;
			if(cf->fn == FN_NONAME) break;
			{
				knh_type_t type = knh_type_tocid(ctx, cf->type, knh_Object_cid(o));
				knh_printf(ctx, w, "[%d] %T %s=", i, type, FN__(cf->fn));
				knh_write_ObjectField(ctx, w, (Object**)o->fields, i, cf->type, MN__k);
				knh_println(ctx, w, STEXT(""));
			}
		}
	}
}

/* ------------------------------------------------------------------------ */
//## method void Any.%s();

static METHOD Any__k(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	if(IS_NULL(sfp[1].o)) {
		knh_write(ctx, WWW, S_tobytes(TS_null));
	}
	else {
		knh_write_sname(ctx, WWW, knh_Object_cid(sfp[1].o));
		knh_putc(ctx, WWW, ':');
		knh_write__p(ctx, WWW, (sfp[1].p)->ptr);
	}
}

/* ======================================================================== */
/* [data] */

//static int knh_Object_isFieldObject(Ctx *ctx, Object *o)
//{
//	return knh_Object_bcid(o) == CLASS_Script
//		|| ClassTBL(knh_Object_cid(o)).cspi == ClassTBL(CLASS_Object).cspi;
//}
//
///* ------------------------------------------------------------------------ */
////## method void Object.%data();
//
//static METHOD Object__data(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	knh_Object_t *o = sfp[1].o;
//	knh_OutputStream_t *w = WWW;
//	if(knh_Object_isNullObject(o)) {
//		knh_write(ctx, w, STEXT("null"));
//	}
//	else if(knh_Object_isFieldObject(ctx, o)) {
//		knh_intptr_t i;
//		knh_class_t cid = knh_Object_cid(o);
//		Object **v = (Object**)o->ref;
//		knh_intptr_t bsize = ClassTBL(cid).size / sizeof(Object*);
//		knh_write_sname(ctx, w, cid);
//		knh_putc(ctx, w, ' ');
//		knh_write_begin(ctx, w, '{');
//		for(i = 0; i < bsize; i++) {
//			knh_fields_t *cf = knh_Class_fieldAt(ctx, cid, i);
//			if(cf->fn == FN_/*register*/) continue;
//			if(cf->fn == FN_NONAME
//					|| FLAG_is(cf->flag, FLAG_Field_Volatile)) continue;
//			knh_write_BOL(ctx, w);
//			knh_printf(ctx, w, "\"%s\": ", FN__(cf->fn));
//			knh_write_ObjectField(ctx, w, v, i, cf->type, MN__data);
//			knh_putc(ctx, w, ',');
//		}
//		knh_write_end(ctx, w, '}');
//	}
//	else{
//		knh_printf(ctx, w, "null/*%C: Not Serializable*/", knh_Object_cid(o));
//	}
//}
//
///* ------------------------------------------------------------------------ */
////## method void String.%data();
//
//static METHOD String__data(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	knh_write_quote(ctx, WWW, S_tobytes(sfp[1].s), '"');
//}
//
///* ------------------------------------------------------------------------ */
////## method void Bytes.%data();
//
//static METHOD Bytes__data(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	TODO();
//	// 'base64:.....'
//}
//
///* ------------------------------------------------------------------------ */
////## method void Array.%data();
//
//static METHOD Array__data(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	knh_Array_t *a = sfp[1].a;
//	knh_OutputStream_t *w = WWW;
//	knh_class_t p1 = knh_Object_p1(a);
//	knh_intptr_t i, size = knh_Array_size(a);
//	knh_write_begin(ctx, w, '[');
//	if(IS_Tint(p1)) {
//		for(i = 0; i < size; i++) {
//			knh_write_BOL(ctx, w);
//			knh_write_ifmt(ctx, w, K_INT_FMT ",", a->ilist[i]);
//		}
//	}
//	else if(IS_Tfloat(p1)) {
//		for(i = 0; i < size; i++) {
//			knh_write_BOL(ctx, w);
//			knh_write_ffmt(ctx, w, K_FLOAT_FMT ",", a->flist[i]);
//		}
//	}
//	else if(size > 0) {
//		knh_Method_t *mtd = knh_getSystemFormatter(ctx, knh_Object_cid(a->list[0]), MN__data);
//		for(i = 0; i < size; i++) {
//			knh_write_BOL(ctx, w);
//			knh_write_Object(ctx, w, ctx->esp, &mtd, a->list[i]);
//			knh_putc(ctx, w, ',');
//		}
//	}
//	knh_write_end(ctx, w, ']');
//}
//
//
///* ------------------------------------------------------------------------ */
////## method void Map.%data();
//
//static METHOD Map__data(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	KNH_TODO(__FUNCTION__);
////	knh_DictMap_t *o = sfp[1].dmap;
////	knh_OutputStream_t *w = WWW;
////	knh_intptr_t i = 0, size = knh_DictMap_size(o);
////	knh_DictMap_sort(o);
////	knh_write_begin(ctx, w, '{');
////	if(size > 0) {
////		knh_Method_t *mtd = knh_getSystemFormatter(ctx, knh_Object_cid(knh_DictMap_valueAt(o, i)), MN__data);
////		for(; i < size; i++) {
////			knh_write_BOL(ctx, w);
////			knh_write(ctx, w, S_tobytes(knh_DictMap_keyAt(o, i)));
////			knh_putc(ctx, w, ':'); knh_putc(ctx, w, ' ');
////			knh_write_Object(ctx, w, ctx->esp, &mtd, knh_DictMap_valueAt(o, i));
////			knh_putc(ctx, w, ',');
////		}
////	}
////	knh_write_end(ctx, w, '}');
//}
//
///* ------------------------------------------------------------------------ */
////## method void Exception.%data();
//
//static METHOD Exception__data(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	knh_write_quote(ctx, WWW, S_tobytes(DP(sfp[1].e)->msg), '\'');
//}

/* ------------------------------------------------------------------------ */
//## method void Func.%data();

static METHOD Func__data(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	TODO();
}

/* ======================================================================== */
/* [dump] */

/* ------------------------------------------------------------------------ */
//## method void Bytes.%dump();

static METHOD Bytes__dump(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Bytes_t *ba = sfp[1].ba;
	knh_OutputStream_t *w = WWW;
	size_t i, j, n;
	char buf[40];
	for(j = 0; j * 16 < ba->bu.len; j++) {
		knh_snprintf(buf, sizeof(buf), "%08x", (int)(j*16));
		knh_write(ctx, w, B(buf));
		for(i = 0; i < 16; i++) {
			n = j * 16 + i;
			if(n < ba->bu.len) {
				knh_snprintf(buf, sizeof(buf), " %2x", (int)ba->bu.ustr[n]);
				knh_write(ctx, w, B(buf));
			}
			else {
				knh_write(ctx, w, STEXT("   "));
			}
		}
		knh_write(ctx, w, STEXT("    "));
		for(i = 0; i < 16; i++) {
			n = j * 16 + i;
			if(n < ba->bu.len && isprint(ba->bu.ustr[n])) {
				knh_snprintf(buf, sizeof(buf), "%c", (int)ba->bu.ustr[n]);
				knh_write(ctx, w, B(buf));
			}
			else {
				knh_write(ctx, w, STEXT(" "));
			}
		}
		knh_write_EOL(ctx, w);
	}
}

/* ------------------------------------------------------------------------ */
//## method void Method.%dump();

static METHOD Method__dump(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Method_t *mtd = sfp[1].mtdOBJ;
	knh_OutputStream_t *w = WWW;
	KNH_SETv(ctx, sfp[2].o, TS_EMPTY);
	Method__k(ctx, sfp, rix);
	knh_write_EOL(ctx, w);
	if(knh_Method_isObjectCode(mtd)) {
		if(IS_KLRCode(DP(mtd)->kcode)) {
			knh_opline_t *pc = SP(mtd)->pc_start;
			while(1) {
				knh_opcode_dump(ctx, pc, w, SP(mtd)->pc_start);
				if(pc->opcode == OPCODE_RET) break;
				pc++;
			}
		}
	}
}

/* ------------------------------------------------------------------------ */
//## method void Exception.%dump();

static METHOD Exception__dump(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Exception_t *e = sfp[1].e;
	knh_OutputStream_t *w = WWW;
	knh_write_EOL(ctx, w);
	if(DP(e)->line != 0) {
		knh_printf(ctx, w, "[%s:%d] ", DP(e)->file, DP(e)->line);
	}
	knh_printf(ctx, w, "%B: %B", S_tobytes(DP(e)->event), S_tobytes(DP(e)->msg));
	if(DP(e)->tracesNULL != NULL) {
		knh_Array_t *a = DP(e)->tracesNULL;
		size_t i, size = knh_Array_size(a), c = 0;
		knh_bytes_t prev = STEXT("?");
		for(i = 0; i < size; i++) {
			knh_String_t *s = a->strings[i];
			if(S_startsWith(s, prev)) {
				c++; continue;
			}
			if(c > 0) {
				knh_write_EOL(ctx, w);
				knh_printf(ctx, w, "  *** called %d times recursively ***", c);
				c = 0;
			}
			knh_write_EOL(ctx, w);
			knh_printf(ctx, w, "  at %s", S_tochar(s));
			prev = S_tobytes(s);
			prev = knh_bytes_first(prev, knh_bytes_rindex(prev, '('));
		}
	}
	knh_write_EOL(ctx, w);
}

///* ======================================================================== */
///* [man] */
//
//static char *knh_methodop__tochar(knh_methodn_t mn)
//{
//	switch(mn) {
//	case MN_opNOT: return "!x";
//	case MN_opOF: return "x instanceof T";
//
//	case MN_opHAS:  return "y in x";
//	case MN_opIS:  return "x is y";
////	case MN_opAs:  return "x as y";
////	case MN_opInto:  return "x into y";
//
//	case MN_opEQ:  return "x == y";
//	case MN_opNOTEQ:  return "x != y";
//	case MN_opLT:  return "x < y";
//	case MN_opLTE:  return "x <= y";
//	case MN_opGT:  return "x > y";
//	case MN_opGTE:  return "x >= y";
//
//	case MN_opLSFT:  return "x << y";
//	case MN_opSEND:  return "x << y";
//	case MN_opRSFT:  return "x >> y";
//
//	case MN_opMOD:  return "x mod y";
//
//	case MN_opADD:  return "x + y";
//	case MN_opNEG:  return "-x";
//	case MN_opSUB:  return "x - y";
//	case MN_opDIV:  return "x / y";
//	case MN_opMUL:  return "x * y";
//
//	case MN_opLOR:        return "x & y";
//	case MN_opLAND:       return "x | y";
//	case MN_opLNOT:       return "~x";
//	case MN_opLXOR:        return "x ^ y";
//	case MN_opNEXT:       return "x++";
//	case MN_opPREV:       return "x--";
//	case MN_opITR:        return "x..";
//	case MN_getSize:      return "|x|";
//	case MN_get:          return "x[n]";
//	case MN_set:          return "x[n]=y";
//	case MN_setAll:       return "x[]=y";
//	case MN_opTO:    return "x[m to n]";
//	case MN_opUNTIL: return "x[m until n]";
//	case MN_opEXPAND:          return "a,b = x";
//	}
//	return NULL;
//}
//
//static void knh_ClassNAME__man(Ctx *ctx, knh_class_t cid, knh_OutputStream_t *w)
//{
//	knh_write(ctx, w, STEXT("Class"));
//	knh_write_EOL(ctx, w);
//
//	knh_write_TAB(ctx, w);
//	knh_write_ltype(ctx, w, cid);
//	knh_write_EOL(ctx, w);
//
//	while(ClassTBL(cid).supcid != CLASS_Object) {
//		cid = ClassTBL(cid).supcid;
//		knh_write_TAB(ctx, w);
//		knh_write(ctx, w, STEXT("extends "));
//		knh_write_ltype(ctx, w, cid);
//		knh_write_EOL(ctx, w);
//	}
//}
//
//static void knh_ClassCONST__man(Ctx *ctx, knh_class_t cid, knh_OutputStream_t *w)
//{
//	DBG_ASSERT_cid(cid);
//	if(ClassTBL(cid).constDictMap == NULL) return ;
//	knh_DictMap_t *tcmap = ClassTBL(cid).constDictMap;
//	size_t i, size = knh_DictMap_size(tcmap);
//	OLD_LOCK(ctx, LOCK_SYSTBL, NULL);
//	int hasCaption = 0;
//	for(i = 0; i < size; i++) {
//		if(!hasCaption) {
//			knh_write_char(ctx, w, _("Const"));
//			knh_write_EOL(ctx, w);
//			hasCaption = 1;
//		}
//		knh_printf(ctx, w, "\t%s.%s: %O\n", CLASS__(cid),
//				S_tochar(knh_DictMap_keyAt(tcmap, i)),
//				knh_DictMap_valueAt(tcmap, i));
//	}
//	OLD_UNLOCK(ctx, LOCK_SYSTBL, NULL);
//}
//
//
//static void knh_Method__man(Ctx *ctx, knh_Method_t *o, knh_OutputStream_t *w, knh_class_t cid)
//{
//	if(!knh_Context_isVerbose(ctx)) {
//		if(knh_Method_isPrivate(o)) return;
//	}
//
//	if(knh_Method_isAbstract(o)) {
//		knh_write(ctx, w, STEXT("@Abstract"));
//		knh_putc(ctx, w, ' ');
//	}
//
//	knh_write_type(ctx, w, knh_type_tocid(ctx, knh_ParamArray_rtype(DP(o)->mp), cid));
//	knh_putc(ctx, w, ' ');
//
//	if(knh_Method_isStatic(o)) {
//		knh_write_char(ctx, w, CTXCLASS__(cid));
//		knh_putc(ctx, w, '.');
//	}
//	knh_write_mn(ctx, w, DP(o)->mn);
//
//	knh_putc(ctx, w, '(');
//	size_t i;
//	for(i = 0; i < knh_Method_psize(o); i++) {
//		knh_param_t *p = knh_ParamArray_get(DP(o)->mp, i);
//		if(i > 0) {
//			knh_write_delim(ctx, w);
//		}
//		knh_write_type(ctx, w, knh_type_tocid(ctx, p->type, cid));
//		knh_putc(ctx, w, ' ');
//		knh_write(ctx, w, B(FN__(p->fn)));
//	}
//	if(knh_ParamArray_isVARGs(DP(o)->mp)) {
//		knh_write_delim(ctx, w);
//		knh_write_dots(ctx, w);
//	}
//	knh_putc(ctx, w, ')');
//}
//
//static void knh_ClassMap__man(Ctx *ctx, knh_Array_t *tmaps, knh_OutputStream_t *w, knh_class_t cid)
//{
//	size_t i;
//	int hasCaption = 0, from = 0;
//	L_TAIL:;
//	for(i = 0; i < tmaps->size; i++) {
//		if(hasCaption == 0) {
//			knh_write_char(ctx, w, _("Translator"));
//			knh_write_EOL(ctx, w);
//			hasCaption = 1;
//		}
//		if(from == 0) {
//			knh_write_TAB(ctx, w);
//			knh_write_sname(ctx, w, cid);
//			knh_write_EOL(ctx, w);
//			from = 1;
//		}
//		knh_Translator_t *mpr = tmaps->trans[i];
//		knh_write_TAB(ctx, w); knh_write_TAB(ctx, w);
//		if(knh_Translator_isTotal(mpr)) {
//			knh_write_char(ctx, w, "==> ");
//		}
//		else {
//			knh_write_char(ctx, w, "--> ");
//		}
//		knh_write_sname(ctx, w, SP(mpr)->tcid);
//		knh_write_EOL(ctx, w);
//	}
//	if(ClassTBL(cid).supcid != CLASS_Object) {
//		cid = ClassTBL(cid).supcid;
//		tmaps = ClassTBL(cid).tmaps;
//		from = 0;
//		goto L_TAIL;
//	}
//}

/* ------------------------------------------------------------------------ */
//## method void Class.%man();
//## method void Object.%man();

static METHOD Class__man(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	KNH_TODO("man Class");
//	knh_class_t cid, this_cid;
//	knh_OutputStream_t *w = WWW;
//	size_t i = 0;
//	knh_DictMap_t *dm = new_DictMap0(ctx, 128);
//	BEGIN_LOCAL(ctx, lsfp, 1);
//	KNH_SETv(ctx, lsfp[1].o, dm);
//
//	if(IS_Class(sfp[1].o)) {
//		this_cid = ((knh_Class_t*)sfp[1].o)->cid;
//		DBG_ASSERT_cid(this_cid);
//	}
//	else {
//		this_cid = knh_Object_cid(sfp[1].o);
//	}
//	cid = this_cid;
//	while(1) {
//		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
//		knh_Array_t *a = ClassTBL(cid).methods;
//		for(i = 0; i < knh_Array_size(a); i++) {
//			knh_Method_t *mtd = (knh_Method_t*)knh_Array_n(a, i);
//			char *op = knh_methodop__tochar(DP(mtd)->mn);
//			if(op == NULL) {
//				knh_cwb_clear(cwb, 0);
//				knh_write_mn(ctx, cwb->w, DP(mtd)->mn);
//				if(knh_DictMap_getNULL(ctx, dm, knh_cwb_tobytes(cwb)) == NULL) {
//					knh_DictMap_set(ctx, dm, knh_cwb_newString(ctx, cwb), mtd);
//				}
//			}
//			else {
//				knh_bytes_t name = B(op);
//				if(knh_DictMap_getNULL(ctx,  dm, name) == NULL) {
//					knh_DictMap_set(ctx, dm, new_T(op), mtd);
//				}
//			}
//		}
//		if(cid == CLASS_Object) {
//			knh_cwb_close(cwb);
//			break;
//		}
//		cid = ClassTBL(cid).supcid;
//		END_LOCAL(ctx, lsfp);
//	}
//
//	knh_ClassNAME__man(ctx, this_cid, w);
//	knh_ClassCONST__man(ctx, this_cid, w);
//
//	int cnt = 0;
//	int hasCaption = 0;
//	int isBOL = 1;
//	char buf[40];
//	for(i = 0; i < knh_DictMap_size(dm); i++) {
//		knh_Method_t *mtd = (knh_Method_t*)knh_DictMap_valueAt(dm, i);
//		if(IS_Method(mtd)) {
//			char *op = knh_methodop__tochar(DP(mtd)->mn);
//			if(op == NULL) continue;
//			knh_DictMap_removeAt(ctx, dm, i);
//			if(DP(mtd)->cid == CLASS_Object && this_cid != CLASS_Object) continue;
//			if(hasCaption == 0) {
//				knh_write_char(ctx, w, _("Operator"));
//				knh_write_EOL(ctx, w);
//				hasCaption = 1;
//			}
//			if(isBOL == 1) {
//				knh_write_TAB(ctx, w);
//				isBOL = 0;
//			}
//			knh_snprintf(buf, sizeof(buf), "%10s  ", op);
//			knh_write_char(ctx, w, buf);
//			if(cnt % 5 == 4) {
//				knh_write_EOL(ctx, w);
//				isBOL = 1;
//			}
//			cnt++;
//		}
//	}
//	if(isBOL != 1) {
//		knh_write_EOL(ctx, w);
//	}
//
//	hasCaption = 0;
//	isBOL = 1;
//	knh_DictMap_sort(dm);
//
//	for(i = 0; i < knh_DictMap_size(dm); i++) {
//		knh_Method_t *mtd = (knh_Method_t*)knh_DictMap_valueAt(dm, i);
//		if(IS_Method(mtd)) {
//			if(MN_isFMT(DP(mtd)->mn)) continue;
//			if(DP(mtd)->cid == CLASS_Object && this_cid != CLASS_Object) continue;
//			if(hasCaption == 0) {
//				knh_write_char(ctx, w, _("Method"));
//				knh_write_EOL(ctx, w);
//				hasCaption = 1;
//			}
//#if defined(K_USING_DEBUG)
//			if(1)
//#else
//			if(!knh_Method_isPrivate(mtd) || !knh_Method_isHidden(mtd))
//#endif
//			{
//				knh_write_TAB(ctx, w);
//				knh_Method__man(ctx, mtd, w, this_cid);
//				knh_write_EOL(ctx, w);
//			}
//			//knh_DictMap_removeAt(ctx, dm, i);
//		}
//	}
//
//	hasCaption = 0;
//	cnt = 8;
//	for(i = 0; i < knh_DictMap_size(dm); i++) {
//		knh_Method_t *mtd = (knh_Method_t*)knh_DictMap_valueAt(dm, i);
//		if(IS_Method(mtd)) {
//			if(!MN_isFMT(DP(mtd)->mn)) continue;
//			if(DP(mtd)->cid == CLASS_Object && this_cid != CLASS_Object) continue;
//			if(hasCaption == 0) {
//				knh_write_char(ctx, w, _("Formatter"));
//				knh_write_EOL(ctx, w);
//				knh_write_TAB(ctx, w);
//				hasCaption = 1;
//			}
//			knh_bytes_t k = S_tobytes(knh_DictMap_keyAt(dm, i));
//			if(cnt + k.len > 72) {
//				knh_write_EOL(ctx, w);
//				knh_write_TAB(ctx, w);
//				cnt = 8;
//			}
//			knh_write(ctx, w, k); knh_putc(ctx, w, ' ');
//			cnt += (k.len + 1);
//		}
//	}
//	knh_write_EOL(ctx, w);
//	knh_ClassMap__man(ctx, ClassTBL(this_cid).tmaps, w, this_cid);
}

/* ------------------------------------------------------------------------ */
//## method void Object.%p();

static METHOD Object__p(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	void *p = NULL;
	if(IS_bInt(sfp[1].o) || IS_bFloat(sfp[1].o)) {
		p = (void*)(&sfp[1] + sizeof(void*));
	}
	else if(IS_Boolean(sfp[1].o)) {
		p = (sfp[1].bvalue) ? (void*)(KNH_TRUE) : (void*)(KNH_FALSE);
	}
	else if(IS_NOTNULL(sfp[1].o)) {
		p = (void*)sfp[1].o;
	}
	knh_write__p(ctx, WWW, p);
}

/* ------------------------------------------------------------------------ */
//## method void Int.%c();

static METHOD Int__c(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	char buf[16];
	knh_uint_t c = (knh_uint_t) sfp[1].ivalue;
	knh_format_utf8(buf, sizeof(buf), c);
	knh_write(ctx, WWW, B(buf));
}

/* ======================================================================== */
/* [number] */

/* ------------------------------------------------------------------------ */
//## method void Int.%u();

static METHOD Int__u(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_write_ifmt(ctx, WWW, K_UINT_FMT, sfp[1].ivalue);
}

/* ------------------------------------------------------------------------ */
//## method void Int.%f();

static METHOD Int__f(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_write_ffmt(ctx, WWW, K_FLOAT_FMT, (knh_float_t)sfp[1].ivalue);
}

/* ------------------------------------------------------------------------ */
//## method void Int.%x();

static METHOD Int__x(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_write_ifmt(ctx, WWW, K_INT_XFMT, sfp[1].ivalue);
}

/* ------------------------------------------------------------------------ */

static void knh_write_bits(Ctx *ctx, knh_OutputStream_t *w, knh_uint64_t n, size_t bits)
{
	size_t i;
	knh_uint64_t flag = 1ULL << (bits - 1);
	for(i = 0; i < bits; i++) {
		if(i > 0 && i % 8 == 0) {
			knh_putc(ctx, w, ' ');
		}
		if((flag & n) == flag) {
			knh_putc(ctx, w, '1');
		}else{
			knh_putc(ctx, w, '0');
		}
		flag = flag >> 1;
	}
}

/* ------------------------------------------------------------------------ */
//## method void Int.%bits();

static METHOD Int__bits(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_write_bits(ctx, WWW, sfp[1].ivalue, sizeof(knh_int_t) * 8);
}

/* ------------------------------------------------------------------------ */
//## method void Float.%d();

static METHOD Float__d(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_write_ifmt(ctx, WWW, K_INT_FMT, (knh_int_t)sfp[1].fvalue);
}

/* ------------------------------------------------------------------------ */
//## method void Float.%bits();

static METHOD Float__bits(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_write_bits(ctx, WWW, sfp[1].data, sizeof(knh_float_t) * 8);
}

#endif/* K_USING_DEFAULTAPI*/

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
