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
//## @Const method Bytes! String.getBytes(String? enc);

static METHOD String_getBytes(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_Bytes_t *ba;
	if(IS_NULL(sfp[1].o)) {
		ba = new_Bytes(ctx, (sfp[0].s)->size + 1);
		knh_Bytes_write(ctx, ba, __tobytes(sfp[0].s));
	}
	else {
		knh_bytes_t t = __tobytes(sfp[0].s);
		knh_BytesConv_t *bc = new_BytesConv__out(ctx, __tochar(sfp[1].s));
		KNH_SETv(ctx, sfp[2].o, bc);
		ba = new_Bytes(ctx, t.len);
		bc->fbconv(ctx, bc, t, ba);
	}
	KNH_RETURN(ctx, sfp, ba);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean! String.equals(String! s);

static METHOD String_equals(Ctx *ctx, knh_sfp_t *sfp)
{
	KNH_RETURN_Boolean(ctx, sfp,
		((sfp[0].s)->size == (sfp[1].s)->size &&
		knh_bytes_strcmp(__tobytes(sfp[0].s), __tobytes(sfp[1].s)) == 0));
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean! String.equals:IgnoreCase(String! s);

static METHOD String_equals__IgnoreCase(Ctx *ctx, knh_sfp_t *sfp)
{
	KNH_RETURN_Boolean(ctx, sfp,
		((sfp[0].s)->size == (sfp[1].s)->size &&
		knh_bytes_strcasecmp(__tobytes(sfp[0].s), __tobytes(sfp[1].s))== 0));
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean! String.startsWith(String! s);

static METHOD String_startsWith(Ctx *ctx, knh_sfp_t *sfp)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_bytes_startsWith(__tobytes(sfp[0].s), __tobytes(sfp[1].s)));
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean! String.startsWith:IgnoreCase(String! s);

static METHOD String_startsWith__IgnoreCase(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_bytes_t base = __tobytes(sfp[0].s);
	knh_bytes_t expr = __tobytes(sfp[1].s);
	int res = 0;
	if(expr.len < base.len) {
		base = knh_bytes_first(base, expr.len);
		res = (knh_bytes_strcasecmp(base, expr) == 0);
	}
	KNH_RETURN_Boolean(ctx, sfp, res);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean! String.endsWith(String! s);

static METHOD String_endsWith(Ctx *ctx, knh_sfp_t *sfp)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_bytes_endsWith(__tobytes(sfp[0].s), __tobytes(sfp[1].s)));
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean! String.endsWith:IgnoreCase(String! s);

static METHOD String_endsWith__IgnoreCase(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_bytes_t base = __tobytes(sfp[0].s);
	knh_bytes_t expr = __tobytes(sfp[1].s);
	int res = 0;
	if(expr.len < base.len) {
		base = knh_bytes_last(base, base.len - expr.len);
		res = (knh_bytes_strcasecmp(base, expr) == 0);
	}
	KNH_RETURN_Boolean(ctx, sfp, res);
}

/* ------------------------------------------------------------------------ */
//## @Const method Int! String.indexOf(String! s);

static METHOD String_indexOf(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_bytes_t base = __tobytes(sfp[0].s);
	knh_bytes_t delim = __tobytes(sfp[1].s);
	int loc = knh_bytes_indexOf(base, delim);
	if (knh_String_isAscii(sfp[0].s) || loc == -1) {
		KNH_RETURN_Int(ctx, sfp, loc);
	} else {
		base.len = (size_t)loc;
		KNH_RETURN_Int(ctx, sfp, knh_bytes_mlen(base));
	}
}

/* ------------------------------------------------------------------------ */
//## @Const method Int! String.indexOf:IgnoreCase(String! s);

static METHOD String_indexOf__IgnoreCase(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_bytes_t base = __tobytes(sfp[0].s);
	knh_bytes_t delim = __tobytes(sfp[1].s);
	knh_intptr_t loc = base.len - delim.len;
	for(loc = 0; loc < base.len - delim.len; loc++) {
		if(base.buf[loc] != delim.buf[loc]) continue;
		knh_bytes_t sub = knh_bytes_offlen(base, loc, delim.len);
		if(knh_bytes_strcasecmp(sub, delim) == 0) break;
	}
	if(loc == base.len - delim.len) loc = -1;
	if (loc >= 0 && !knh_String_isAscii(sfp[0].s)) {
		base.len = (size_t)loc;
		loc = knh_bytes_mlen(base);
	}
	KNH_RETURN_Int(ctx, sfp, loc);
}

/* ------------------------------------------------------------------------ */
//## @Const method Int! String.lastIndexOf(String! s);

static METHOD String_lastIndexOf(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_bytes_t base = __tobytes(sfp[0].s);
	knh_bytes_t delim = __tobytes(sfp[1].s);
	knh_intptr_t loc = base.len - delim.len;
	for(; loc >= 0; loc--) {
		if(base.buf[loc] != delim.buf[loc]) continue;
		knh_bytes_t sub = knh_bytes_offlen(base, loc, delim.len);
		if(knh_bytes_strcmp(sub, delim) == 0) break;
	}
	if (loc >= 0 && !knh_String_isAscii(sfp[0].s)) {
		base.len = (size_t)loc;
		loc = knh_bytes_mlen(base);
	}
	KNH_RETURN_Int(ctx, sfp, loc);
}

/* ------------------------------------------------------------------------ */
//## @Const method Int! String.lastIndexOf:IgnoreCase(String! s);

static METHOD String_lastIndexOf__IgnoreCase(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_bytes_t base = __tobytes(sfp[0].s);
	knh_bytes_t delim = __tobytes(sfp[1].s);
	knh_intptr_t loc = base.len - delim.len;
	for(; loc >= 0; loc--) {
		if(base.buf[loc] != delim.buf[loc]) continue;
		knh_bytes_t sub = knh_bytes_offlen(base, loc, delim.len);
		if(knh_bytes_strcasecmp(sub, delim) == 0) break;
	}
	if (loc >= 0 && !knh_String_isAscii(sfp[0].s)) {
		base.len = (size_t)loc;
		loc = knh_bytes_mlen(base);
	}
	KNH_RETURN_Int(ctx, sfp, loc);
}

/* ------------------------------------------------------------------------ */
//## @Const @NullBase method String! String.concat(Any? value, ...);

static METHOD String_concat(Ctx *ctx, knh_sfp_t *sfp)
{
	int i, ac = knh_stack_argc(ctx, sfp);
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_sfp_t *esp = KNH_LOCAL(ctx);
	for(i = 0; i < ac; i++) {
		if(IS_bString(sfp[i].o)) {
			knh_Bytes_write(ctx, cwb->ba, __tobytes(sfp[i].s));
		}
		else {
			KNH_ASSERT(esp == ctx->esp);
			KNH_SETv(ctx, esp[1].o, sfp[i].o); esp[1].data = sfp[i].data;
			knh_esp1_format(ctx, METHODN__s, cwb->w, KNH_NULL);
		}
	}
	KNH_RETURN(ctx, sfp, knh_cwb_newString(ctx, cwb));
}

/* ------------------------------------------------------------------------ */
//## @Const method String! String.times(Int! n);

static METHOD String_times(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_String_t *res;
	knh_intptr_t n = p_int(sfp[1]);
	if(n <= 0) {
		res = TS_EMPTY;
	}else if(n == 1) {
		res = sfp[0].s;
	}
	else {
		knh_bytes_t base = __tobytes(sfp[0].s);
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		knh_intptr_t i;
		for(i = 0; i < n; i++) {
			knh_Bytes_write(ctx, cwb->ba, base);
		}
		res = knh_cwb_newString(ctx, cwb);
	}
	KNH_RETURN(ctx, sfp, res);
}

/* ------------------------------------------------------------------------ */
//## @Const method PairSS String.twofold(String! s);

static METHOD String_twofold(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_bytes_t base = __tobytes(sfp[0].s);
	knh_bytes_t delim = __tobytes(sfp[1].s);
	knh_index_t index = knh_bytes_indexOf(base, delim);
	knh_String_t *first, *second;
	knh_Pair_t *p;
	if(index == -1) {
		first = sfp[0].s;
		second = TS_EMPTY;
	}
	else {
		if(index == 0) {
			first = TS_EMPTY;
		}
		else {
			first = new_String(ctx, knh_bytes_first(base, index), sfp[0].s);
		}
		second = new_String(ctx, knh_bytes_last(base, index + delim.len), sfp[0].s);
	}
	p = (knh_Pair_t*)new_hObject(ctx, FLAG_Pair, CLASS_Pair, CLASS_PairSS);
	KNH_INITv(p->first, first);
	KNH_INITv(p->second, second);
	KNH_RETURN(ctx, sfp, p);
}

/* ------------------------------------------------------------------------ */
//## method String! String.format(Any? value, ...);

static METHOD String_format(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_bytes_t fmt = __tobytes(sfp[0].s);
	knh_sfp_t *param = sfp + 1;
	int ac = knh_stack_argc(ctx, param);
	knh_bytes_t mt, expr, next;

	if(!knh_bytes_findMT(ctx, fmt, &mt, &expr, &next)) {
		KNH_RETURN(ctx, sfp, sfp[0].s);
	}

	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	int count;
	for(count = 0; ; count++) {
		if(mt.buf > fmt.buf + 1) {
			fmt.len = (mt.buf - fmt.buf) - 1;
			knh_Bytes_write(ctx, cwb->ba, fmt);
		}
		int index = count;
		if(expr.len > 0) {
			knh_int_t num;
			if(knh_bytes_parseint(expr, &num)) {
				index = (int)num;
			}
		}
		if(0 <= index && index < ac) {
			knh_sfp_t *esp = KNH_LOCAL(ctx);
			KNH_SETv(ctx, esp[1].o, param[index].o); esp[1].data = param[index].data;
			Object *m = KNH_NULL;
			if(knh_bytes_isOptionalMT(mt)) m = UP(new_String(ctx, mt, NULL));
			mt.buf = mt.buf - 1; mt.len++;   /* 's' == > '%s' */
			knh_methodn_t mn = knh_getmn(ctx, mt, METHODN__empty);
			knh_esp1_format(ctx, mn, cwb->w, m);
		}
		else {
			if(knh_Context_isDebug(ctx)) {
				KNH_THROW_OUTOFINDEX(ctx, index, ac);
			}
		}
		fmt.buf = next.buf; fmt.len = next.len;
		if(!knh_bytes_findMT(ctx, fmt, &mt, &expr, &next)) {
			break;
		}
	}
	if(fmt.len > 0) {
		knh_Bytes_write(ctx, cwb->ba, fmt);
	}
	KNH_RETURN(ctx, sfp, new_StringX__cwb(ctx, knh_Object_cid(sfp[0].o), cwb));
}

/* ------------------------------------------------------------------------ */

static
int knh_bytes_equals_(knh_bytes_t base, size_t s, knh_bytes_t target)
{
	size_t i;
	for(i = 1; i < target.len; i++) {
		if(base.buf[s+i] != target.buf[i]) return 0;
	}
	return 1;
}

/* ------------------------------------------------------------------------ */
//## @Const method String! String.replace(String! s, String! newone)
/* @author nakata */

static METHOD String_replace(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_bytes_t base = __tobytes(sfp[0].s);
	knh_bytes_t target = __tobytes(sfp[1].s);
	knh_bytes_t alt = __tobytes(sfp[2].s);
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	int search_flag= 0, ch = target.buf[0], i;

	if (base.len == 0 || target.len == 0) KNH_RETURN(ctx, sfp, sfp[0].o);
	for(i = 0; i < base.len - target.len+1; i++) {
		if(base.buf[i] == ch && knh_bytes_equals_(base, i, target)) {
		    knh_Bytes_write(ctx, cwb->ba, alt);
			i += target.len - 1;
			search_flag = 1;
		}else {
			knh_Bytes_putc(ctx, cwb->ba, base.buf[i]);
		}
	}
	if(search_flag == 0) {
		KNH_RETURN(ctx, sfp, sfp[0].o);
	}
	else {
		knh_bytes_t leftover = {base.buf + i, base.len - i};
		knh_Bytes_write(ctx, cwb->ba, leftover);
		KNH_RETURN(ctx, sfp, knh_cwb_newString(ctx, cwb));
	}
}

/* ------------------------------------------------------------------------ */
//## @Const method Int! String.getUCS4(Int? n);

static METHOD String_getUCS4(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_bytes_t base = __tobytes(sfp[0].s);
	knh_intptr_t index = IS_NULL(sfp[1].o) ? 0 : (knh_intptr_t)sfp[1].ivalue;
	if(knh_String_isAscii(sfp[0].s)) {
		size_t n = knh_array_index(ctx, index, knh_String_strlen(sfp[0].s));
		KNH_RETURN_Int(ctx, sfp, knh_uchar_toucs4(&base.buf[n]));
	}
	else {
		size_t off = knh_array_index(ctx, index, knh_bytes_mlen(base));
		knh_bytes_t sub = knh_bytes_mofflen(base, off, 1);
		KNH_RETURN_Int(ctx, sfp, knh_uchar_toucs4(&sub.buf[0]));
	}
}

/* ======================================================================== */

static
size_t bconv__toLower(Ctx *ctx, knh_BytesConv_t *o, knh_bytes_t t, knh_Bytes_t *ba)
{
	size_t i;
	for(i = 0; i < t.len; i++) {
		if(isupper(t.buf[i])) {
			knh_Bytes_putc(ctx, ba, tolower(t.buf[i]));
		}
		else {
			knh_Bytes_putc(ctx, ba, t.buf[i]);
		}
	}
	return t.len;
}

/* ------------------------------------------------------------------------ */
//## @Const method String! String.toLower();

static METHOD String_toLower(Ctx *ctx, knh_sfp_t *sfp)
{
	KNH_RETURN(ctx, sfp, new_String__fbcnv(ctx, sfp[0].s, bconv__toLower, (knh_BytesConv_t*)KNH_NULL));
}

/* ------------------------------------------------------------------------ */

static
size_t bconv__toUpper(Ctx *ctx, knh_BytesConv_t *o, knh_bytes_t t, knh_Bytes_t *ba)
{
	size_t i;
	for(i = 0; i < t.len; i++) {
		if(islower(t.buf[i])) {
			knh_Bytes_putc(ctx, ba, toupper(t.buf[i]));
		}
		else {
			knh_Bytes_putc(ctx, ba, t.buf[i]);
		}
	}
	return t.len;
}

/* ------------------------------------------------------------------------ */
//## @Const method String! String.toUpper();

static METHOD String_toUpper(Ctx *ctx, knh_sfp_t *sfp)
{
	KNH_RETURN(ctx, sfp, new_String__fbcnv(ctx, sfp[0].s, bconv__toUpper, (knh_BytesConv_t*)KNH_NULL));
}

/* ------------------------------------------------------------------------ */
//## @Const method String! String.trim();

static METHOD String_trim(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_bytes_t t = __tobytes(sfp[0].s);
	knh_bytes_t t2 = knh_bytes_trim(t);
	knh_String_t *s = sfp[0].s;
	if(t.len > t2.len) {
		s = new_String(ctx, t2, s);
	}
	KNH_RETURN(ctx, sfp, s);
}

/* ------------------------------------------------------------------------ */

static knh_Array_t *knh_String_toCharArray(Ctx *ctx, knh_String_t *bs, int istrim)
{
	knh_bytes_t base = __tobytes(bs);
	if(knh_String_isAscii(bs)) {
		size_t i, n = base.len;
		knh_Array_t *a = new_Array(ctx, CLASS_String, n);
		for(i = 0; i < n; i++) {
			if(istrim && isspace(base.buf[i])) continue;
			knh_bytes_t sub = { base.buf + i, 1};
			knh_Array_add(ctx, a, UP(new_String(ctx, sub, bs)));
		}
		return a;
	}
	else {
		size_t i, n = knh_bytes_mlen(base);
		knh_Array_t *a = new_Array(ctx, CLASS_String, n);
		for(i = 0; i < n; i++) {
			if(istrim && isspace(base.buf[i])) continue;
			knh_bytes_t sub = knh_bytes_mofflen(base, n, 1);
			knh_Array_add(ctx, a, UP(new_String(ctx, sub, bs)));
		}
		return a;
	}
}

/* ------------------------------------------------------------------------ */
//## @Const method String[] String.split(String delim, Int isTrim);

static METHOD String_split(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_Array_t *a = NULL;
	int istrim = IS_NULL(sfp[2].o) ? 0 : (int)sfp[2].ivalue;
	if(IS_NULL(sfp[1].o)) {
		a = knh_String_toCharArray(ctx, sfp[0].s, istrim);
	}
	else {
		knh_bytes_t delim = __tobytes(sfp[1].s);
		if(delim.len == 0) {
			a = knh_String_toCharArray(ctx, sfp[0].s, istrim);
		}
		else {
			knh_bytes_t base = __tobytes(sfp[0].s);
			a = new_Array(ctx, CLASS_String, 8);
			while(1) {
				knh_index_t loc = knh_bytes_indexOf(base, delim);
				if(loc == -1) {
					if(istrim) base = knh_bytes_trim(base);
					knh_Array_add(ctx, a, UP(new_String(ctx, base, sfp[0].s)));
					break;
				}
				else if(loc == 0) {
					knh_Array_add(ctx, a, UP(TS_EMPTY));
				}
				else {
					knh_bytes_t t = knh_bytes_first(base, loc);
					if(istrim) t = knh_bytes_trim(t);
					knh_Array_add(ctx, a, UP(new_String(ctx, t, sfp[0].s)));
				}
				base.buf = base.buf + loc + delim.len;
				base.len = base.len - loc - delim.len;
			}
		}
	}
	KNH_ASSERT(a != NULL);
	KNH_RETURN(ctx, sfp, a);
}

/* ======================================================================== */

/* ------------------------------------------------------------------------ */

static
int knh_String_opMatch(Ctx *ctx, knh_String_t *o, knh_Regex_t *re)
{
	char *str = __tochar(o);
	knh_regmatch_t pmatch[KNH_REGEX_NMATCH_SIZE];
	int res = re->df->regexec(ctx, re->reg, str, KNH_REGEX_NMATCH_SIZE, pmatch, 0);
	return (res == 0);
}

/* ------------------------------------------------------------------------ */
//## method Boolean! String.opMatch(Regex! re);

static
METHOD String_opMatch(Ctx *ctx, knh_sfp_t *sfp)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_String_opMatch(ctx, sfp[0].s, (knh_Regex_t*)sfp[1].o));
}

/* ------------------------------------------------------------------------ */
//## method Boolean! Regex.opMatch(String! s);

static
METHOD Regex_opMatch(Ctx *ctx, knh_sfp_t *sfp)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_String_opMatch(ctx, sfp[1].s, (knh_Regex_t*)sfp[0].o));
}

/* ------------------------------------------------------------------------ */

knh_Array_t *knh_Regex_split(Ctx *ctx, knh_Regex_t *o, knh_String_t *s)
{
	char *str = __tochar(s);
	knh_regmatch_t pmatch[KNH_REGEX_NMATCH_SIZE];
	int res = o->df->regexec(ctx, o->reg, str, KNH_REGEX_NMATCH_SIZE, pmatch, 0);
	if(res == 0) {
		knh_Array_t *a = new_Array(ctx, CLASS_String, KNH_REGEX_NMATCH_SIZE);
		knh_bytes_t sub = __tobytes(s);
		int i;
		for(i = 0; i < KNH_REGEX_NMATCH_SIZE; i++) {
			if(pmatch[i].rm_so == -1) break;
			DBG2_P("[%d], rm_so=%d, rm_eo=%d", i, pmatch[i].rm_so, pmatch[i].rm_eo);
			sub.buf = (knh_uchar_t*)str + pmatch[i].rm_so;
			sub.len = pmatch[i].rm_eo - pmatch[i].rm_so;
			knh_Array_add(ctx, a, UP(new_String(ctx, sub, s)));
		}
		return a;
	}
	else {
		return (knh_Array_t*)knh_getClassDefaultValue(ctx, CLASS_String_Ary);
		//return (Array*)KNH_NULL;
	}
}

/* ------------------------------------------------------------------------ */
//## @Const method String[] String.match(Regex pattern);

static METHOD String_match(Ctx *ctx, knh_sfp_t *sfp)
{
	KNH_RETURN(ctx, sfp, knh_Regex_split(ctx, (knh_Regex_t*)sfp[1].o, sfp[0].s));
}


/* ------------------------------------------------------------------------ */

#endif/*KNH_CC_METHODAPI*/

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
