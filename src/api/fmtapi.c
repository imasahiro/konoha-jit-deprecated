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

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef KNH_CC_METHODAPI

/* ------------------------------------------------------------------------ */
/* [utils] */

static
int knh_stack_isRecuriveFormatting(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_sfp_t *sp = sfp - 3;
	while(ctx->stack < sp) {
		if(sp[0].mtd == sfp[0].mtd && sp[1].o == sfp[1].o && sp[2].w == sfp[2].w) {
			return 1;
		}
		sp--;
	}
	return 0;
}

static
void knh_stack_reformat(Ctx *ctx, knh_sfp_t *sfp, knh_methodn_t fmt)
{
	knh_Method_t *mtd = knh_lookupFormatter(ctx, knh_Object_cid(sfp[0].o), fmt);
	//KNH_SETv(ctx, sfp[-1].mtd, mtd);
	mtd->fcall_1(ctx, sfp);
}

static
void knh_write_ObjectField(Ctx *ctx, knh_OutputStream_t *w, Object **v, size_t i, knh_type_t type, knh_methodn_t mn)
{
#ifdef KNH_USING_UNBOXFIELD
	if(IS_ubxint(type)) {
		knh_int_t *data = (knh_int_t*)(v + i);
		knh_write_ifmt(ctx, w, KNH_INT_FMT, data[0]);
	}
	else if(IS_ubxfloat(type)) {
		knh_float_t *data = (knh_float_t*)(v + i);
		knh_write_ffmt(ctx, w, KNH_FLOAT_FMT, data[0]);
	}
	else if(IS_ubxboolean(type)) {
		knh_bool_t *data = (knh_bool_t*)(v + i);
		knh_write_bool(ctx, w, data[0]);
	}
	else
#endif
	{
		knh_format(ctx, w, mn, v[i], KNH_NULL);
	}
}

/* ======================================================================== */
/* [%empty] */

/* ------------------------------------------------------------------------ */
//## method void Object.%empty(OutputStream w, String? fmt);

static METHOD Object__empty(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	if(knh_Context_isDebug(ctx)) {
		knh_printf(ctx, sfp[1].w, "@Debug %%empty(%s)", CLASSNo(sfp[0].o));
	}
}

/* ======================================================================== */
/* [%s] */

/* ------------------------------------------------------------------------ */
//## method void Object.%s(OutputStream w, String? fmt);

static METHOD Object__s(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
//	if(IS_NULL(b)) {
//		knh_write(ctx, w, STEXT("null"));
//	}
//	else {
		knh_write_char(ctx, sfp[1].w, CLASSN(knh_Object_cid(sfp[0].o)));
		knh_putc(ctx, sfp[1].w, ':');
		knh_write__p(ctx, sfp[1].w, (void*)sfp[0].o);
//	}
}

/* ------------------------------------------------------------------------ */
//## method void Boolean.%s(OutputStream w, String? fmt);
//## method void Boolean.%k(OutputStream w, String? fmt);
//## method void Boolean.%data(OutputStream w, String? fmt);
//## method void Boolean.%dump(OutputStream w, String? fmt);

static METHOD Boolean__s(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_write_bool(ctx, sfp[1].w, p_bool(sfp[0]));
}

/* ------------------------------------------------------------------------ */
//## method void Int.%s(OutputStream w, String? fmt);

static METHOD Int__s(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_write_ifmt(ctx, sfp[1].w, KNH_INT_FMT, sfp[0].ivalue);
}

/* ------------------------------------------------------------------------ */

static
void knh_write_quote(Ctx *ctx, knh_OutputStream_t *w, knh_bytes_t t, int quote)
{
	knh_bytes_t sub = t;
	size_t i, s = 0;
	knh_putc(ctx, w, quote);
	for(i = 0; i < t.len; i++) {
		int ch = t.buf[i];
		if(ch == '\t' || ch == '\n' || ch == '\r' || ch == '\\' || ch == quote) {
			sub.buf = t.buf + s;
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
		sub.buf = t.buf + s;
		sub.len = t.len - s;
		knh_print(ctx, w, sub);
	}
	knh_putc(ctx, w, quote);
}

/* ------------------------------------------------------------------------ */
//## method void String.%s(OutputStream w, String? fmt);

static METHOD String__s(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_print(ctx, sfp[1].w, __tobytes(sfp[0].s));
}

/* ------------------------------------------------------------------------ */
//## method void Bytes.%s(OutputStream w, String? fmt);

static METHOD Bytes__k(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_printf(ctx, sfp[1].w, "byte[%d]", (sfp[0].ba)->size);
}

/* ------------------------------------------------------------------------ */
//## method void Regex.%s(OutputStream w, String? fmt);

static METHOD Regex__s(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_write_quote(ctx, sfp[1].w, __tobytes(((knh_Regex_t*)sfp[0].o)->pattern), '/');
}

/* ------------------------------------------------------------------------ */
//## method void Class.%s(OutputStream w, String? fmt);

static METHOD Class__s(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_write_ltype(ctx, sfp[1].w, knh_Object_cid(sfp[0].o));
}

/* ------------------------------------------------------------------------ */
//## method void Method.%s(OutputStream w, String? fmt);

static METHOD Method__s(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_write_cid(ctx, sfp[1].w, DP(sfp[0].mtd)->cid);
	knh_putc(ctx, sfp[1].w, '.');
	knh_write_mn(ctx, sfp[1].w, knh_Method_rztype(sfp[0].mtd), DP(sfp[0].mtd)->mn);
}

/* ------------------------------------------------------------------------ */
//## method void Exception.%s(OutputStream w, String? fmt);

static METHOD Exception__s(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_write_char(ctx, sfp[1].w, EXPTN(DP(sfp[0].e)->eid));
	knh_write_char(ctx, sfp[1].w, "!!");
}

/* ======================================================================== */
/* [%k] */

/* ------------------------------------------------------------------------ */
//## method void Object.%k(OutputStream w, String? fmt);

static METHOD Object__k(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	Object *o = sfp[0].o;
	knh_OutputStream_t *w = sfp[1].w;
	if(knh_Object_isUndefined(o)) {
		knh_write(ctx, w, STEXT("undefined"));
	}
	else if(o->h.cid == CLASS_Object || o->h.bcid != CLASS_Object) {
		knh_stack_reformat(ctx, sfp, METHODN__s);
	}
	else if(knh_stack_isRecuriveFormatting(ctx, sfp)) {
		knh_write_dots(ctx, w);
	}
	else {
		size_t bsize = ClassTable(o->h.cid).bsize;
		knh_write_cid(ctx, w, o->h.cid);
		if(bsize > 0) {
			size_t i;
			Object **v = (Object**)o->ref;
			knh_putc(ctx, w, '{');
			for(i = 0; i < bsize; i++) {
				knh_cfield_t *cf = knh_Class_fieldAt(ctx, knh_Object_cid(o), i);
				if(cf->fn == FIELDN_NONAME) break;
				if(cf->fn == FIELDN_/*register*/) continue;
				if(i > 0) {
					knh_write_delim(ctx, w);
				}
				knh_write_fn(ctx, w, cf->fn);
				knh_write(ctx, w, STEXT(": "));
				knh_write_ObjectField(ctx, w, v, i, cf->type, METHODN__k);
			}
			knh_putc(ctx, w, '}');
		}
	}
}

/* ------------------------------------------------------------------------ */
//## method void Int.%k(OutputStream w, String? fmt);
//## method void Int.%data(OutputStream w, String? fmt);
//## method void Int.%dump(OutputStream w, String? fmt);

static METHOD Int__k(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Semantics_t *u = knh_getSemantics(ctx, knh_Object_cid(sfp[0].o));
	knh_write_intx(ctx, sfp[1].w, u, sfp[0].ivalue);
}

/* ------------------------------------------------------------------------ */
//## method void Float.%k(OutputStream w, String? fmt);
//## method void Float.%data(OutputStream w, String? fmt);
//## method void Float.%dump(OutputStream w, String? fmt);

static METHOD Float__k(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Semantics_t *u = knh_getSemantics(ctx, knh_Object_cid(sfp[0].o));
	knh_write_floatx(ctx, sfp[1].w, u, p_float(sfp[0]));
}

/* ------------------------------------------------------------------------ */
//## method void String.%k(OutputStream w, String? fmt);

static METHOD String__k(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_write_quote(ctx, sfp[1].w, __tobytes(sfp[0].s), '"');
}

/* ------------------------------------------------------------------------ */
//## method void Iterator.%k(OutputStream w, String? fmt);

static METHOD Iterator__k(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	size_t c = 0;
	knh_Iterator_t *it = sfp[0].it;
	knh_OutputStream_t *w = sfp[1].w;
	int isInline = IS_NULL(sfp[2].o) ? 1 : 0;
	KNH_MOV(ctx, sfp[4].o, sfp[1].w);
	KNH_MOV(ctx, sfp[5].o, KNH_NULL);
	while(it->fnext_1(ctx, sfp, 3)) {
		if(c > 0) {
			if(isInline && c > 7) {
				knh_write_dots(ctx, w);
				break;
			}
			knh_write_EOL(ctx, sfp[1].w);
		}
		KNH_SETESP(ctx, sfp+6);
		KNH_MOV(ctx, sfp[2].o, knh_lookupFormatter(ctx, knh_Object_cid(sfp[3].o), METHODN__k));
		(sfp[2].mtd)->fcall_1(ctx, sfp+3);
		c++;
	}
}

/* ------------------------------------------------------------------------ */
//## method void Pair.%k(OutputStream w, String? fmt);

static METHOD Pair__k(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_OutputStream_t *w = sfp[1].w;
	knh_putc(ctx, w, '(');
	if(knh_stack_isRecuriveFormatting(ctx, sfp)) {
		knh_write_dots(ctx, w);
	}
	else {
		knh_Pair_t *o = sfp[0].pair;
		knh_format(ctx, w, METHODN__k, o->first, KNH_NULL);
		knh_write_delim(ctx, w);
		knh_format(ctx, w, METHODN__k, o->second, KNH_NULL);
	}
	knh_putc(ctx, w, ')');
}

/* ------------------------------------------------------------------------ */
//## method void Tuple.%k(OutputStream w, String? fmt);

static METHOD Tuple__k(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Tuple_t *o = sfp[0].tuple;
	knh_OutputStream_t *w = sfp[1].w;
	knh_putc(ctx, w, '(');
	if(knh_stack_isRecuriveFormatting(ctx, sfp)) {
		knh_write_dots(ctx,w);
	}
	else if(knh_Tuple_isTriple(o)) {
		knh_format(ctx, w, METHODN__k, o->first, KNH_NULL);
		knh_write_delim(ctx,w);
		knh_format(ctx, w, METHODN__k, o->second, KNH_NULL);
		knh_write_delim(ctx,w);
		knh_format(ctx, w, METHODN__k, o->third, KNH_NULL);
	}
	else {
		size_t i;
		for(i = 0; i < o->size; i++) {
			if(i > 0) {
				knh_write_delim(ctx,w);
			}
			knh_format(ctx, w, METHODN__k, o->list[i], KNH_NULL);
		}
	}
	knh_putc(ctx, w, ')');
}

/* ------------------------------------------------------------------------ */
//## method void Range.%k(OutputStream w, String? fmt);

static METHOD Range__k(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Range_t *o = sfp[0].range;
	knh_OutputStream_t *w = sfp[1].w;
	knh_putc(ctx, w, '(');
	knh_format(ctx, w, METHODN__k, o->start, KNH_NULL);
	if(knh_Range_isInclusive(o)) {
		knh_write(ctx, w, STEXT(" to "));
	}
	else {
		knh_write(ctx, w, STEXT(" until "));
	}
	knh_format(ctx, w, METHODN__k, o->end, KNH_NULL);
	knh_putc(ctx, w, ')');
}

/* ------------------------------------------------------------------------ */
//## method void Array.%k(OutputStream w, String? fmt);

static METHOD Array__k(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_OutputStream_t *w = sfp[1].w;
	knh_putc(ctx, w, '[');
	if(knh_stack_isRecuriveFormatting(ctx, sfp)) {
		knh_write_dots(ctx,w);
	}
	else {
		knh_Array_t *o = sfp[0].a;
		int isInline = IS_NULL(sfp[2].o) ? 1 : 0;
		size_t c;
		for(c = 0; c < o->size; c++) {
			if(c > 0) {
				if(isInline && c > 7) {
					knh_write_dots(ctx, w);
					break;
				}
				knh_write_delim(ctx,w);
			}
			knh_format(ctx, w, METHODN__k, o->list[c], KNH_NULL);
		}
	}
	knh_putc(ctx, w, ']');
}

/* ------------------------------------------------------------------------ */
//## method void IArray.%k(OutputStream w, String? fmt);

static METHOD IArray__k(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_IArray_t *o = sfp[0].ia;
	knh_OutputStream_t *w = sfp[1].w;
	int isInline = IS_NULL(sfp[2].o) ? 1 : 0;
	size_t c;
	knh_putc(ctx, w, '[');
	for(c = 0; c < o->size; c++) {
		if(c > 0) {
			if(isInline && c > 7) {
				knh_write_dots(ctx, w);
				break;
			}
			knh_write_delim(ctx, w);
		}
		knh_write_ifmt(ctx, w, KNH_INT_FMT, o->ilist[c]);
	}
	knh_putc(ctx, w, ']');
}

/* ------------------------------------------------------------------------ */
//## method void FArray.%k(OutputStream w, String? fmt);

static METHOD FArray__k(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_FArray_t *o = sfp[0].fa;
	knh_OutputStream_t *w = sfp[1].w;
	int isInline = IS_NULL(sfp[2].o) ? 1 : 0;
	size_t c;
	knh_putc(ctx, w, '[');
	for(c = 0; c < o->size; c++) {
		if(c > 0) {
			if(isInline && c > 7) {
				knh_write_dots(ctx, w);
				break;
			}
			knh_write_delim(ctx,w);
		}
		knh_write_ffmt(ctx, w, KNH_FLOAT_FMT, o->flist[c]);
	}
	knh_putc(ctx, w, ']');
}

/* ------------------------------------------------------------------------ */
//## method void DictMap.%k(OutputStream w, String? fmt);

static METHOD DictMap__k(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_OutputStream_t *w = sfp[1].w;
	knh_putc(ctx, w, '{');
	if(knh_stack_isRecuriveFormatting(ctx, sfp)) {
		knh_write_dots(ctx, w);
	}
	else {
		knh_DictMap_t *o = sfp[0].dmap;
		int isInline = IS_NULL(sfp[2].o) ? 1 : 0;
		size_t c;
		knh_DictMap_sort(o);
		for(c = 0; c < o->size; c++) {
			if(c > 0) {
				if(isInline && c > 3) {
					knh_write_dots(ctx, w);
					break;
				}
				knh_write_delim(ctx, w);
			}
			knh_write(ctx, w, __tobytes(o->list[c].key));
			knh_putc(ctx, w, ':');	knh_putc(ctx, w, ' ');
			knh_format(ctx, w, METHODN__k, o->list[c].value, KNH_NULL);
		}
	}
	knh_putc(ctx, w, '}');
}

/* ------------------------------------------------------------------------ */
//## method void DictSet.%k(OutputStream w, String? fmt);

static
METHOD DictSet__k(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_DictSet_t *o = sfp[0].dset;
	knh_OutputStream_t *w = sfp[1].w;
	int isInline = IS_NULL(sfp[2].o) ? 1 : 0;
	size_t c;
	knh_DictSet_sort(o);
	knh_putc(ctx, w, '{');
	for(c = 0; c < o->size; c++) {
		if(c > 0) {
			if(isInline && c > 3) {
				knh_write_dots(ctx, w);
				break;
			}
			knh_write_delim(ctx, w);
		}
		knh_write(ctx, w, __tobytes(o->list[c].key));
	}
	knh_putc(ctx, w, '}');
}

/* ------------------------------------------------------------------------ */
//## method void DictIdx.%k(OutputStream w, String? fmt);

static METHOD DictIdx__k(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_SETv(ctx, sfp[0].o, ((knh_DictIdx_t*)sfp[0].o)->terms);
	Array__k(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## method void HashMap.%k(OutputStream w, String? fmt);

static METHOD HashMap__k(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Hash_t *o = (knh_Hash_t*)sfp[0].o;
	knh_OutputStream_t *w = sfp[1].w;
	size_t pos = 0, c = 0;
	size_t max = (KNH_HASH_TABLESIZE / o->hashop->size) * DP(o)->tables_size;
	int isInline = IS_NULL(sfp[2].o) ? 1 : 0;
	knh_putc(ctx, w, '{');
	while(pos < max) {
		knh_hashentry_t *e = knh_hashentry_at((knh_Hash_t*)o, pos);
		if(e != NULL) {
			if(c > 0) {
				if(isInline && c > 7) {
					knh_write_dots(ctx, w);
					break;
				}
				knh_write_delim(ctx,w);
			}
			knh_format(ctx, w, METHODN__k, e->key, KNH_NULL);
			knh_putc(ctx, w, ':');
			knh_putc(ctx, w, ' ');
			knh_format(ctx, w, METHODN__k, e->value, KNH_NULL);
			c++;
		}
		pos++;
	}
	knh_putc(ctx, w, '}');
}

/* ------------------------------------------------------------------------ */
//## method void Class.%k(OutputStream w, String? fmt);

static METHOD Class__k(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_write_type(ctx, sfp[1].w, ((knh_Class_t*)sfp[0].o)->type);
}

/* ------------------------------------------------------------------------ */
//## method void Method.%k(OutputStream w, String? fmt);

static METHOD Method__k(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Method_t *o = sfp[0].mtd;
	knh_OutputStream_t *w = sfp[1].w;
	int isInline = IS_NULL(sfp[2].o) ? 1 : 0;
	if(!isInline && knh_Method_isAbstract(o)) {
		knh_write(ctx, w, STEXT("@Abstract"));
		knh_putc(ctx, w, ' ');
	}
	if(knh_Method_rztype(o) == TYPE_void) {
		knh_write(ctx, w, __tobytes(TS_void));
	}else{
		knh_write_type(ctx, w, knh_Method_rztype(o));
	}
	knh_putc(ctx, w, ' ');
	Method__s(ctx, sfp);
	knh_putc(ctx, w, '(');
	size_t i;
	for(i = 0; i < knh_Method_psize(o); i++) {
		knh_mparam_t mf = knh_Method_param(o, i);
		if(i > 0) {
			knh_write_delim(ctx, w);
		}
		knh_write_type(ctx, w, mf.type);
		if(!isInline) {
			knh_putc(ctx, w, ' ');
			knh_write(ctx, w, B(FIELDN(mf.fn)));
		}
	}
	if(knh_Method_isVarArgs(o)) {
		knh_write_delim(ctx, w);
		knh_write_dots(ctx, w);
	}
	knh_putc(ctx, w, ')');
}

/* ------------------------------------------------------------------------ */
//## method void Mapper.%k(OutputStream w, String? fmt);

static METHOD Mapper__k(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Mapper_t *o = sfp[0].mpr;
	knh_OutputStream_t *w = sfp[1].w;
	//int isInline = IS_NULL(sfp[2].o) ? 1 : 0;
	knh_write_cid(ctx, w, DP(o)->scid);
	if(knh_Mapper_isTotal(o)) {
		knh_write(ctx, w, STEXT("==>"));
	}else{
		knh_write(ctx, w, STEXT("-->"));
	}
	knh_write_cid(ctx, w, DP(o)->tcid);
}

/* ------------------------------------------------------------------------ */
//## method void Exception.%k(OutputStream w, String? fmt);

static METHOD Exception__k(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Exception_t *o = sfp[0].e;
	knh_OutputStream_t *w = sfp[1].w;
	knh_write_quote(ctx, w, __tobytes(DP(o)->msg), '\'');
}

/* ------------------------------------------------------------------------ */
//## method void InputStream.%k(OutputStream w, String? fmt);

static METHOD InputStream__k(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_write_quote(ctx, sfp[1].w, __tobytes(DP((knh_InputStream_t*)sfp[0].o)->urn), '\'');
}

/* ------------------------------------------------------------------------ */
//## method void OutputStream.%k(OutputStream w, String? fmt);

static METHOD OutputStream__k(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_write_quote(ctx, sfp[1].w, __tobytes(DP(sfp[0].w)->urn), '\'');
}

/* ------------------------------------------------------------------------ */
//## method void Script.%k(OutputStream w, String? fmt);

METHOD Script__k(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	int isInline = IS_NULL(sfp[2].o) ? 1 : 0;
	if(isInline) {
		knh_write(ctx, sfp[1].w, STEXT("(Script)"));
	}
	else {
		size_t i;
		knh_Script_t *o = (knh_Script_t*)sfp[0].o;
		knh_OutputStream_t *w = sfp[1].w;
		for(i = 0; i < KNH_SCRIPT_FIELDSIZE; i++) {
			knh_cfield_t *cf = knh_Class_fieldAt(ctx, knh_Object_cid(o), i);
			if(cf == NULL) break;
			if(cf->fn == FIELDN_NONAME) break;
			{
				knh_type_t type = knh_pmztype_totype(ctx, cf->type, knh_Object_cid(o));
				knh_printf(ctx, w, "[%d] %T %s=", i, type, FIELDN(cf->fn));
				knh_write_ObjectField(ctx, w, (Object**)o->fields, i, cf->type, METHODN__k);
				knh_println(ctx, w, STEXT(""));
			}
		}
	}
}

/* ------------------------------------------------------------------------ */
//## method void Any.%s(OutputStream w, String? fmt);
//## method void Any.%k(OutputStream w, String? fmt);

static METHOD Any__k(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	if(IS_NULL(sfp[0].o)) {
		knh_write(ctx, sfp[1].w, __tobytes(TS_null));
	}
	else {
		knh_write_cid(ctx, sfp[1].w, knh_Object_cid(sfp[0].o));
		knh_putc(ctx, sfp[1].w, ':');
		knh_write__p(ctx, sfp[1].w, (sfp[0].glue)->ptr);
	}
}

/* ======================================================================== */
/* [dump] */

/* ------------------------------------------------------------------------ */
//## method void Object.%dump(OutputStream w, String? fmt);

static METHOD Object__dump(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_SETv(ctx, sfp[2].o, TS_EMPTY);
	knh_stack_reformat(ctx, sfp, METHODN__k);
}

/* ------------------------------------------------------------------------ */
//## method void Bytes.%dump(OutputStream w, String? fmt);

static METHOD Bytes__dump(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Bytes_t *o = sfp[0].ba;
	knh_OutputStream_t *w = sfp[1].w;
	size_t i, j, n;
	char buf[40];
	for(j = 0; j * 16 < o->size; j++) {
		knh_snprintf(buf, sizeof(buf), "%08x", (int)(j*16));
		knh_write(ctx, w, B(buf));
		for(i = 0; i < 16; i++) {
			n = j * 16 + i;
			if(n < o->size) {
				knh_snprintf(buf, sizeof(buf), " %2x", (int)o->buf[n]);
				knh_write(ctx, w, B(buf));
			}
			else {
				knh_write(ctx, w, STEXT("   "));
			}
		}
		knh_write(ctx, w, STEXT("    "));
		for(i = 0; i < 16; i++) {
			n = j * 16 + i;
			if(n < o->size && isprint(o->buf[n])) {
				knh_snprintf(buf, sizeof(buf), "%c", (int)o->buf[n]);
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
//## method void Class.%dump(OutputStream w, String? fmt);

static
METHOD Class__dump(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	TODO();
}

/* ------------------------------------------------------------------------ */
//## method void ClassMap.%dump(OutputStream w, String? fmt);

static METHOD ClassMap__dump(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_ClassMap_t *cmap = (knh_ClassMap_t*)sfp[0].o;
	knh_OutputStream_t *w = sfp[1].w;
	size_t i;
	for(i = 0; i < cmap->size; i++) {
		knh_printf(ctx, w, "[%d]\t", i);
		KNH_SETv(ctx, sfp[0].o, cmap->maplist[i]);
		Mapper__k(ctx, sfp);
		knh_write_EOL(ctx, w);
	}
}

/* ------------------------------------------------------------------------ */
//## method void Method.%dump(OutputStream w, String? fmt);

static METHOD Method__dump(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Method_t *mtd = sfp[0].mtd;
	knh_OutputStream_t *w = sfp[1].w;
	KNH_SETv(ctx, sfp[2].o, TS_EMPTY);
	Method__k(ctx, sfp);
	knh_write_EOL(ctx, w);
	if(knh_Method_isObjectCode(mtd)) {
		if(IS_KLRCode(DP(mtd)->kcode)) {
			knh_code_t *pc = SP(mtd)->pc_start;
			while(KNH_OPCODE(pc) != OPCODE_RET) {
				knh_printf(ctx, w, "[%p:%d] ", pc, knh_Method_pcline(mtd, pc));
				knh_opcode_dump(ctx, (knh_inst_t*)pc, w);
				knh_write_EOL(ctx, w);
				pc += knh_opcode_size(KNH_OPCODE(pc));
			}
			knh_printf(ctx, w, "[%p:%d] ", pc, knh_Method_pcline(mtd, pc));
			knh_opcode_dump(ctx, (knh_inst_t*)pc, w);
			knh_write_EOL(ctx, w);
		}
	}
}

/* ------------------------------------------------------------------------ */
//## method void Exception.%dump(OutputStream w, String? fmt);

static METHOD Exception__dump(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Exception_t *o = sfp[0].e;
	knh_OutputStream_t *w = sfp[1].w;
	DBG2_ASSERT(IS_Exception(o));
	knh_write_EOL(ctx, w);
	if(DP(o)->line != 0) {
		knh_printf(ctx, w, "[%s:%d] ", DP(o)->file, DP(o)->line);
	}
	knh_print(ctx, w, __tobytes(DP(o)->msg));
	if(IS_bArray(DP(o)->traces)) {
		size_t i, size = knh_Array_size(DP(o)->traces), c = 0;
		knh_bytes_t prev = STEXT("?");
		for(i = 0; i < size; i++) {
			knh_String_t *s = (knh_String_t*)knh_Array_n(DP(o)->traces, i);
			if(knh_String_startsWith(s, prev)) {
				c++;
				continue;
			}
			if(c > 0) {
				knh_printf(ctx, w, "\n    ** called %d times recursively **", c);
				c = 0;
			}
			knh_printf(ctx, w, "\n  at %s", __tochar(s));
			prev = __tobytes(s);
			prev = knh_bytes_first(prev, knh_bytes_rindex(prev, '('));
		}
	}
	knh_write_EOL(ctx, w);
}

/* ======================================================================== */
/* [data] */

static int knh_Object_isFieldObject(Ctx *ctx, Object *o)
{
	return knh_Object_bcid(o) == CLASS_Script
		|| ClassTable(knh_Object_cid(o)).cspi == ClassTable(CLASS_Object).cspi;
}

/* ------------------------------------------------------------------------ */
//## method void Object.%data(OutputStream w, String? fmt);

static METHOD Object__data(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Object_t *o = sfp[0].o;
	knh_OutputStream_t *w = sfp[1].w;
	if(knh_Object_isUndefined(o)) {
		knh_write(ctx, w, STEXT("null/*undefined*/"));
	}
	else if(knh_Object_isFieldObject(ctx, o)) {
		knh_intptr_t i;
		knh_class_t cid = knh_Object_cid(o);
		Object **v = (Object**)o->ref;
		knh_write_cid(ctx, w, cid);
		knh_putc(ctx, w, ' ');
		knh_write_begin(ctx, w, '{');
		for(i = 0; i < ClassTable(cid).bsize; i++) {
			knh_cfield_t *cf = knh_Class_fieldAt(ctx, cid, i);
			if(cf->fn == FIELDN_/*register*/) continue;
			if(cf->fn == FIELDN_NONAME
				|| KNH_FLAG_IS(cf->flag, FLAG_ClassField_Volatile)) continue;
			knh_write_BOL(ctx, w);
			knh_printf(ctx, w, "\"%s\": ", FIELDN(cf->fn));
			knh_write_ObjectField(ctx, w, v, i, cf->type, METHODN__data);
			knh_putc(ctx, w, ',');
		}
		knh_write_end(ctx, w, '}');
	}
	else{
		knh_printf(ctx, w, "null/*%C: Not Serializable*/", knh_Object_cid(o));
	}
}

/* ------------------------------------------------------------------------ */
//## method void String.%data(OutputStream w, String? fmt);

static METHOD String__data(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_write_quote(ctx, sfp[1].w, __tobytes(sfp[0].s), '"');
}

/* ------------------------------------------------------------------------ */
//## method void Bytes.%data(OutputStream w, String? fmt);

static METHOD Bytes__data(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	TODO();
	// 'base64:.....'
}

/* ------------------------------------------------------------------------ */
//## method void Array.%data(OutputStream w, String? fmt);

static METHOD Array__data(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Array_t *a = sfp[0].a;
	knh_OutputStream_t *w = sfp[1].w;
	knh_intptr_t i, size = knh_Array_size(a);
	knh_write_begin(ctx, w, '[');
	for(i = 0; i < size; i++) {
		Object *v = knh_Array_n(a, i);
		knh_write_BOL(ctx, w);
		knh_format(ctx, w, METHODN__data, v, KNH_NULL);
		knh_putc(ctx, w, ',');
	}
	knh_write_end(ctx, w, ']');
}

/* ------------------------------------------------------------------------ */
//## method void IArray.%data(OutputStream w, String? fmt);

static METHOD IArray__data(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_intptr_t i, size = knh_Array_size(sfp[0].ia);
	knh_OutputStream_t *w = sfp[1].w;
	KNH_SETv(ctx, sfp[-1].o, sfp[0].o);
	KNH_SETv(ctx, sfp[0].o, KNH_DEF(ctx, knh_Object_p1(sfp[-1].ia)));
	knh_write_begin(ctx, w, '[');
	for(i = 0; i < size; i++) {
		sfp[0].ivalue = (sfp[-1].ia)->ilist[i];
		knh_write_BOL(ctx, w);
		Int__k(ctx, sfp);
		knh_putc(ctx, w, ',');
	}
	knh_write_end(ctx, w, ']');
}

/* ------------------------------------------------------------------------ */
//## method void FArray.%data(OutputStream w, String? fmt);

static METHOD FArray__data(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_intptr_t i, size = knh_Array_size(sfp[0].ia);
	knh_OutputStream_t *w = sfp[1].w;
	KNH_SETv(ctx, sfp[-1].o, sfp[0].o);
	KNH_SETv(ctx, sfp[0].o, KNH_DEF(ctx, knh_Object_p1(sfp[-1].ia)));
	knh_write_begin(ctx, w, '[');
	for(i = 0; i < size; i++) {
		knh_write_BOL(ctx, w);
		sfp[0].fvalue = (sfp[-1].fa)->flist[i];
		Float__k(ctx, sfp);
	}
	knh_write_end(ctx, w, ']');
}

/* ------------------------------------------------------------------------ */
//## method void DictMap.%data(OutputStream w, String? fmt);

static METHOD DictMap__data(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_DictMap_t *o = sfp[0].dmap;
	knh_OutputStream_t *w = sfp[1].w;
	knh_intptr_t i, size = knh_DictMap_size(o);
	knh_DictMap_sort(o);
	knh_write_begin(ctx, w, '{');
	for(i = 0; i < size; i++) {
		Object *v = knh_DictMap_valueAt(o, i);
		if(IS_NOTNULL(v)) {
			knh_write_BOL(ctx, w);
			knh_format(ctx, w, METHODN__k, UP(knh_DictMap_keyAt(o, i)), KNH_NULL);
			knh_putc(ctx, w, ':'); knh_putc(ctx, w, ' ');
			knh_format(ctx, w, METHODN__data, v, KNH_NULL);
			knh_putc(ctx, w, ',');
		}
	}
	knh_write_end(ctx, w, '}');
}

/* ------------------------------------------------------------------------ */
//## method void Exception.%data(OutputStream w, String? fmt);

static METHOD Exception__data(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_write_quote(ctx, sfp[1].w, __tobytes(DP(sfp[0].e)->msg), '\'');
}

/* ======================================================================== */
/* [man] */

static char *knh_methodop__tochar(knh_methodn_t mn)
{
	switch(mn) {
	case METHODN_opNot: return "!x";
	case METHODN_opInstanceof: return "x instanceof T";

	case METHODN_opHas:  return "y in x";
	case METHODN_opIs:  return "x is y";
	case METHODN_opAs:  return "x as y";
	case METHODN_opInto:  return "x into y";

	case METHODN_opEq:  return "x == y";
	case METHODN_opNeq:  return "x != y";
	case METHODN_opLt:  return "x < y";
	case METHODN_opLte:  return "x <= y";
	case METHODN_opGt:  return "x > y";
	case METHODN_opGte:  return "x >= y";

	case METHODN_opLshift:  return "x << y";
	case METHODN_opAppend:  return "x << y";
	case METHODN_opRshift:  return "x >> y";

	case METHODN_opMod:  return "x mod y";

	case METHODN_opAdd:  return "x + y";
	case METHODN_opNeg:  return "-x";
	case METHODN_opSub:  return "x - y";
	case METHODN_opDiv:  return "x / y";
	case METHODN_opMul:  return "x * y";

	case METHODN_opLor:        return "x & y";
	case METHODN_opLand:       return "x | y";
	case METHODN_opLnot:       return "~x";
	case METHODN_opXor:        return "x ^ y";
	case METHODN_opNext:       return "x++";
	case METHODN_opPrev:       return "x--";
	case METHODN_opItr:        return "x..";
	case METHODN_getSize:      return "|x|";
	case METHODN_get:          return "x[n]";
	case METHODN_set:          return "x[n]=y";
	case METHODN_setAll:       return "x[]=y";
	case METHODN_opRangeTo:    return "x[m to n]";
	case METHODN_opRangeUntil: return "x[m until n]";
	case METHODN_op0:          return "a,b = x";
	case METHODN_opN :         return "a,b,c = x";
	case METHODN_opCase :      return "case x:";
	}
	return NULL;
}

static
void knh_ClassNAME__man(Ctx *ctx, knh_class_t cid, knh_OutputStream_t *w)
{
	knh_write(ctx, w, STEXT("Class"));
	knh_write_EOL(ctx, w);

	knh_write_TAB(ctx, w);
	knh_write_ltype(ctx, w, cid);
	knh_write_EOL(ctx, w);

	while(ClassTable(cid).supcid != CLASS_Object) {
		cid = ClassTable(cid).supcid;
		knh_write_TAB(ctx, w);
		knh_write(ctx, w, STEXT("extends "));
		knh_write_ltype(ctx, w, cid);
		knh_write_EOL(ctx, w);
	}
}

static
void knh_ClassCONST__man(Ctx *ctx, knh_class_t cid, knh_OutputStream_t *w)
{
	DBG2_ASSERT_cid(cid);
	if(ClassTable(cid).constDictMap == NULL) return ;
	knh_DictMap_t *tcmap = ClassTable(cid).constDictMap;
	size_t i, size = knh_DictMap_size(tcmap);
	KNH_LOCK(ctx, LOCK_SYSTBL, NULL);
	int hasCaption = 0;
	for(i = 0; i < size; i++) {
		if(!hasCaption) {
			knh_write_char(ctx, w, _("Const"));
			knh_write_EOL(ctx, w);
			hasCaption = 1;
		}
		knh_printf(ctx, w, "\t%s.%s: %O\n", CLASSN(cid),
				__tochar(knh_DictMap_keyAt(tcmap, i)),
				knh_DictMap_valueAt(tcmap, i));
	}
	KNH_UNLOCK(ctx, LOCK_SYSTBL, NULL);
}


static
void knh_Method__man(Ctx *ctx, knh_Method_t *o, knh_OutputStream_t *w, knh_class_t cid)
{
	if(!knh_Context_isVerbose(ctx)) {
		if(knh_Method_isPrivate(o)) return;
	}

	if(knh_Method_isAbstract(o)) {
		knh_write(ctx, w, STEXT("@Abstract"));
		knh_putc(ctx, w, ' ');
	}

	knh_write_type(ctx, w, knh_pmztype_totype(ctx, knh_Method_rztype(o), cid));
	knh_putc(ctx, w, ' ');

	if(knh_Method_isStatic(o)) {
		knh_write_char(ctx, w, CTXCLASSN(cid));
		knh_putc(ctx, w, '.');
	}
	knh_write_mn(ctx, w, knh_Method_rztype(o), DP(o)->mn);

	knh_putc(ctx, w, '(');
	size_t i;
	for(i = 0; i < knh_Method_psize(o); i++) {
		if(i > 0) {
			knh_write_delim(ctx, w);
		}
		knh_mparam_t mf = knh_Method_param(o, i);
		knh_write_type(ctx, w, knh_pmztype_totype(ctx, mf.type, cid));
		knh_putc(ctx, w, ' ');
		knh_write(ctx, w, B(FIELDN(mf.fn)));
	}
	if(knh_Method_isVarArgs(o)) {
		knh_write_delim(ctx, w);
		knh_write_dots(ctx, w);
	}
	knh_putc(ctx, w, ')');
}


static
void knh_ClassMap__man(Ctx *ctx, knh_ClassMap_t *cmap, knh_OutputStream_t *w, knh_class_t cid)
{
	int i;
	int hasCaption = 0, from = 0;
	L_TAIL:;
	knh_ClassMap_sort(ctx, cmap);
	for(i = 0; i < cmap->size; i++) {
		if(hasCaption == 0) {
			knh_write_char(ctx, w, _("Mapper"));
			knh_write_EOL(ctx, w);
			hasCaption = 1;
		}
		if(from == 0) {
			knh_write_TAB(ctx, w);
			knh_write_cid(ctx, w, cid);
			knh_write_EOL(ctx, w);
			from = 1;
		}
		knh_Mapper_t *mpr = cmap->maplist[i];
		knh_write_TAB(ctx, w); knh_write_TAB(ctx, w);
		if(knh_Mapper_isSynonym(mpr)) {
			knh_write_char(ctx, w, "=== ");
		}
		else if(knh_Mapper_isTotal(mpr)) {
			knh_write_char(ctx, w, "==> ");
		}
		else {
			knh_write_char(ctx, w, "--> ");
		}
		knh_write_cid(ctx, w, DP(mpr)->tcid);
		knh_write_EOL(ctx, w);
	}
	if(ClassTable(cid).supcid != CLASS_Object) {
		cid = ClassTable(cid).supcid;
		cmap = ClassTable(cid).cmap;
		from = 0;
		goto L_TAIL;
	}
}

/* ------------------------------------------------------------------------ */
//## method void Class.%man(OutputStream w, String? fmt);
//## method void Object.%man(OutputStream w, String? fmt);

static METHOD Class__man(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_class_t cid, this_cid;
	knh_OutputStream_t *w = sfp[1].w;
	size_t i = 0;
	knh_DictMap_t *dm = new_DictMap0(ctx, 128);
	KNH_SETv(ctx, sfp[2].o, dm);
	if(IS_Class(sfp[0].o)) {
		this_cid = ((knh_Class_t*)sfp[0].o)->cid;
		DBG2_ASSERT_cid(this_cid);
	}
	else {
		this_cid = knh_Object_cid(sfp[0].o);
	}
	cid = this_cid;
	while(1) {
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		knh_Array_t *a = ClassTable(cid).cstruct->methods;
		for(i = 0; i < knh_Array_size(a); i++) {
			knh_Method_t *mtd = (knh_Method_t*)knh_Array_n(a, i);
			char *op = knh_methodop__tochar(DP(mtd)->mn);
			if(op == NULL) {
				knh_cwb_subclear(cwb, 0);
				knh_write_mn(ctx, cwb->w, knh_Method_rztype(mtd), DP(mtd)->mn);
				if(IS_NULL(knh_DictMap_get__b(ctx, dm, knh_cwb_tobytes(cwb)))) {
					knh_DictMap_set(ctx, dm, knh_cwb_newString(ctx, cwb), UP(mtd));
				}
			}
			else {
				knh_bytes_t name = B(op);
				if(IS_NULL(knh_DictMap_get__b(ctx,  dm, name))) {
					knh_DictMap_set(ctx, dm, T__(op), UP(mtd));
				}
			}
		}
		if(cid == CLASS_Object) {
			knh_cwb_close(cwb);
			break;
		}
		cid = ClassTable(cid).supcid;
	}

	knh_ClassNAME__man(ctx, this_cid, w);
	knh_ClassCONST__man(ctx, this_cid, w);

	int cnt = 0;
	int hasCaption = 0;
	int isBOL = 1;
	char buf[40];
	for(i = 0; i < knh_DictMap_size(dm); i++) {
		knh_Method_t *mtd = (knh_Method_t*)knh_DictMap_valueAt(dm, i);
		if(IS_Method(mtd)) {
			char *op = knh_methodop__tochar(DP(mtd)->mn);
			if(op == NULL) continue;
			knh_DictMap_removeAt(ctx, dm, i);
			if(DP(mtd)->cid == CLASS_Object && this_cid != CLASS_Object) continue;
			if(hasCaption == 0) {
				knh_write_char(ctx, w, _("Operator"));
				knh_write_EOL(ctx, w);
				hasCaption = 1;
			}
			if(isBOL == 1) {
				knh_write_TAB(ctx, w);
				isBOL = 0;
			}
			knh_snprintf(buf, sizeof(buf), "%10s  ", op);
			knh_write_char(ctx, w, buf);
			if(cnt % 5 == 4) {
				knh_write_EOL(ctx, w);
				isBOL = 1;
			}
			cnt++;
		}
	}
	if(isBOL != 1) {
		knh_write_EOL(ctx, w);
	}

	hasCaption = 0;
	isBOL = 1;
	knh_DictMap_sort(dm);

	for(i = 0; i < knh_DictMap_size(dm); i++) {
		knh_Method_t *mtd = (knh_Method_t*)knh_DictMap_valueAt(dm, i);
		if(IS_Method(mtd)) {
			if(METHODN_IS_MOVTEXT(DP(mtd)->mn)) continue;
			if(DP(mtd)->cid == CLASS_Object && this_cid != CLASS_Object) continue;
			if(hasCaption == 0) {
				knh_write_char(ctx, w, _("Method"));
				knh_write_EOL(ctx, w);
				hasCaption = 1;
			}
#if defined(KNH_DBGMODE2)
			if(1) {
#else
			if(!knh_Method_isPrivate(mtd) || !knh_Method_isHidden(mtd)) {
#endif
				knh_write_TAB(ctx, w);
				knh_Method__man(ctx, mtd, w, this_cid);
				knh_write_EOL(ctx, w);
			}
			knh_DictMap_removeAt(ctx, dm, i);
		}
	}

	hasCaption = 0;
	cnt = 8;
	for(i = 0; i < knh_DictMap_size(dm); i++) {
		knh_Method_t *mtd = (knh_Method_t*)knh_DictMap_valueAt(dm, i);
		if(IS_Method(mtd)) {
			if(!METHODN_IS_MOVTEXT(DP(mtd)->mn)) continue;
			if(DP(mtd)->cid == CLASS_Object && this_cid != CLASS_Object) continue;
			if(hasCaption == 0) {
				knh_write_char(ctx, w, _("Formatter"));
				knh_write_EOL(ctx, w);
				knh_write_TAB(ctx, w);
				hasCaption = 1;
			}
			knh_bytes_t k = __tobytes(knh_DictMap_keyAt(dm, i));
			if(cnt + k.len > 72) {
				knh_write_EOL(ctx, w);
				knh_write_TAB(ctx, w);
				cnt = 8;
			}
			knh_write(ctx, w, k); knh_putc(ctx, w, ' ');
			cnt += (k.len + 1);
		}
	}
	knh_write_EOL(ctx, w);
	knh_ClassMap__man(ctx, ClassTable(this_cid).cmap, w, this_cid);
}


/* ======================================================================== */
/* [C Compatible] */

/* ------------------------------------------------------------------------ */
//## method void Object.%p(OutputStream w, String? fmt);

static METHOD Object__p(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	void *p = NULL;
	if(IS_bInt(sfp[0].o) || IS_bFloat(sfp[0].o)) {
		p = (void*)(&sfp[0] + sizeof(void*));
	}
	else if(IS_Boolean(sfp[0].o)) {
		p = (sfp[0].bvalue) ? (void*)(KNH_TRUE) : (void*)(KNH_FALSE);
	}
	else if(IS_NOTNULL(sfp[0].o)) {
		p = (void*)sfp[0].o;
	}
	knh_write__p(ctx, sfp[1].w, p);
}

/* ------------------------------------------------------------------------ */
//## method void Int.%c(OutputStream w, String? fmt);

static METHOD Int__c(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	char buf[16];
	knh_format_utf8(buf, sizeof(buf), sfp[0].ivalue);
	knh_write(ctx, sfp[1].w, B(buf));
}

/* ======================================================================== */
/* [number] */

/* ------------------------------------------------------------------------ */
//## method void Int.%d(OutputStream w, String? fmt);

static METHOD Int__d(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	if(IS_String(sfp[2].s)) {
		char fmt[40];
		knh_format_newFMT(fmt, sizeof(fmt), __tobytes(sfp[2].s), 0, KNH_INT_FMT);
		knh_write_ifmt(ctx, sfp[1].w, fmt, sfp[0].ivalue);
	}
	else {
		knh_write_ifmt(ctx, sfp[1].w, KNH_INT_FMT, sfp[0].ivalue);
	}
}

/* ------------------------------------------------------------------------ */
//## method void Int.%u(OutputStream w, String? fmt);

static METHOD Int__u(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	if(IS_String(sfp[2].s)) {
		char fmt[40];
		knh_format_newFMT(fmt, sizeof(fmt), __tobytes(sfp[2].s), 0, KNH_UINT_FMT);
		knh_write_ifmt(ctx, sfp[1].w, fmt, sfp[0].ivalue);
	}
	else {
		knh_write_ifmt(ctx, sfp[1].w, KNH_UINT_FMT, sfp[0].ivalue);
	}
}

/* ------------------------------------------------------------------------ */
//## method void Int.%f(OutputStream w, String? fmt);

static METHOD Int__f(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	if(IS_String(sfp[2].s)) {
		char fmt[40];
		knh_format_newFMT(fmt, sizeof(fmt), __tobytes(sfp[2].s), 1, KNH_FLOAT_FMT);
		knh_write_ffmt(ctx, sfp[1].w, fmt, (knh_float_t)sfp[0].ivalue);
	}
	else {
		knh_write_ffmt(ctx, sfp[1].w, KNH_FLOAT_FMT, (knh_float_t)sfp[0].ivalue);
	}
}

/* ------------------------------------------------------------------------ */
//## method void Int.%x(OutputStream w, String? fmt);

static METHOD Int__x(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
    if(IS_String(sfp[2].s)) {
        char fmt[40];
        knh_format_newFMT(fmt, sizeof(fmt), __tobytes(sfp[2].s), 1, KNH_INT_XFMT);
        knh_write_ifmt(ctx, sfp[1].w, fmt, sfp[0].ivalue);
    }
    else {
        knh_write_ifmt(ctx, sfp[1].w, KNH_INT_XFMT, sfp[0].ivalue);
    }
}

/* ------------------------------------------------------------------------ */

static
void knh_write_bits(Ctx *ctx, knh_OutputStream_t *w, knh_uint64_t n, size_t bits)
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
//## method void Int.%bits(OutputStream w, String? fmt);

static METHOD Int__bits(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_write_bits(ctx, sfp[1].w, sfp[0].ivalue, sizeof(knh_int_t) * 8);
}

/* ------------------------------------------------------------------------ */
//## method void Float.%s(OutputStream w, String? fmt);

static METHOD Float__s(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_write_ffmt(ctx, sfp[1].w, KNH_FLOAT_FMT, p_float(sfp[0]));
}

/* ------------------------------------------------------------------------ */
//## method void Float.%d(OutputStream w, String? fmt);

static METHOD Float__d(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	if(IS_String(sfp[2].s)) {
		char fmt[40];
		knh_format_newFMT(fmt, sizeof(fmt), __tobytes(sfp[2].s), 0, KNH_INT_FMT);
		//DBG_P("fmt='%s'", fmt);
		knh_write_ifmt(ctx, sfp[1].w, fmt, (knh_int_t)p_float(sfp[0]));
	}
	else {
		knh_write_ifmt(ctx, sfp[1].w, KNH_INT_FMT, (knh_int_t)p_float(sfp[0]));
	}
}

/* ------------------------------------------------------------------------ */
//## method void Float.%f(OutputStream w, String? fmt);

static METHOD Float__f(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	if(IS_String(sfp[2].s)) {
		char fmt[40];
		knh_format_newFMT(fmt, sizeof(fmt), __tobytes(sfp[2].s), 1, KNH_FLOAT_FMT);
		//DBG_P("fmt='%s'", fmt);
		knh_write_ffmt(ctx, sfp[1].w, fmt, p_float(sfp[0]));
	}
	else {
		knh_write_ffmt(ctx, sfp[1].w, KNH_FLOAT_FMT, p_float(sfp[0]));
	}
}

/* ------------------------------------------------------------------------ */
//## method void Float.%bits(OutputStream w, String? fmt);

static METHOD Float__bits(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_write_bits(ctx, sfp[1].w, sfp[0].data, sizeof(knh_float_t) * 8);
}

#endif/* KNH_CC_METHODAPI*/

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
