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
//## @Const method Boolean String.opEXISTS();

static METHOD String_opEXISTS(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_bytes_t path = S_tobytes(sfp[0].s);
	knh_PathDSPI_t *dspi = knh_getPathDSPINULL(ctx, path);
	knh_bool_t tf = 0;
	if(dspi != NULL) tf = dspi->exists(ctx, path, NULL) ? 1 : 0;
	RETURNb_(tf);
}

/* ------------------------------------------------------------------------ */
//## @Hidden @Private @ReadOnly method Any String.path(String qualifier, NameSpace ns, Class c);

static METHOD String_path(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_class_t cid = (sfp[3].c)->cid;
	knh_bytes_t path = S_tobytes(sfp[0].s);
	knh_bytes_t qpath = S_tobytes(sfp[1].s);
	knh_PathDSPI_t *dspi = knh_NameSpace_getPathDSPINULL(ctx, sfp[2].ns, qpath);
	knh_String_t* spath = sfp[0].s;
	Object *v = NULL;
	int isTRIM = 0;
	if(!knh_bytes_startsWith(path, qpath)) {
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		knh_Bytes_write(ctx, cwb->ba, qpath);
		knh_Bytes_write(ctx, cwb->ba, path);
		spath = knh_cwb_newString(ctx, cwb);
		KNH_SETv(ctx, sfp[rix].s, spath);
		path = S_tobytes(spath);
		isTRIM = 1;
	}
	KNH_ASSERT(dspi != NULL);
	if(cid == CLASS_Boolean) {
		sfp[rix].bvalue = dspi->exists(ctx, path, NULL);
		v = sfp[rix].bvalue ? KNH_TRUE : KNH_FALSE;
	}
	else if(dspi->newObjectNULL != NULL) {
		v = dspi->newObjectNULL(ctx, cid, spath);
		if(v == NULL && cid != CLASS_String) {
			if(!dspi->exists(ctx, path, NULL)) {
				KNH_SYSLOG(ctx, LOG_WARNING, "MissingPath", "qpath='%B', path='%B'", qpath, path);
			}
			else {
				KNH_SYSLOG(ctx, LOG_WARNING, "Type!!", "requested=%T, path='%B'", cid, path);
			}
		}
	}
	if(v == NULL) {
		if(cid != CLASS_String) {
			v = KNH_NULVAL(cid);
		}
		else if(isTRIM) {
			v = sfp[0].o;
		}
		else {
			path = S_tobytes(spath);
			v = (Object*)new_String_(ctx, CLASS_String, knh_bytes_last(path, qpath.len), spath);
		}
	}
	RETURN_(v);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean String.equals(String s);

static METHOD String_equals(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_((S_size(sfp[0].s) == S_size(sfp[1].s) &&
		knh_bytes_strcmp(S_tobytes(sfp[0].s), S_tobytes(sfp[1].s)) == 0));
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean String.startsWith(String s);

static METHOD String_startsWith(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_bytes_startsWith(S_tobytes(sfp[0].s), S_tobytes(sfp[1].s)));
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean String.endsWith(String s);

static METHOD String_endsWith(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_bytes_endsWith(S_tobytes(sfp[0].s), S_tobytes(sfp[1].s)));
}

///* ------------------------------------------------------------------------ */
////## @Const method String String.concat(Object value, ...);
////## @Const method String String.opADD(Any value);
//
//static METHOD String_concat(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	int i, ac = knh_stack_argc(ctx, sfp);
//	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
//	knh_Method_t *mtd = knh_getDefaultFormatter(ctx, MN__s);
//	for(i = 0; i < ac; i++) {
//		if(IS_bString(sfp[i].o)) {
//			knh_Bytes_write(ctx, cwb->ba, S_tobytes(sfp[i].s));
//		}
//		else if(!knh_write_ndata(ctx, cwb->w, knh_Object_bcid(sfp[i].o), sfp[i].data)) {
//			knh_write_Object(ctx, cwb->w, sfp+ac, &mtd, sfp[i].o);
//		}
//	}
//	RETURN_(knh_cwb_newString(ctx, cwb));
//}

/* ------------------------------------------------------------------------ */
//## @Const method Int String.indexOf(Regex re);

static METHOD String_indexOf(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Regex_t *re = sfp[1].re;
	knh_index_t loc = -1;
	if (!IS_NULL(re)) {
		const char *str = ctx->api->tochar(ctx, sfp[0].s);  // necessary
		knh_regmatch_t pmatch[K_REGEX_MATCHSIZE];
		int res = re->spi->regexec(ctx, re->reg, str, K_REGEX_MATCHSIZE, pmatch, 0);
		if(res == 0) {
			loc = pmatch[0].rm_so;
			if (!knh_String_isASCII(sfp[0].s) && loc != -1) {
				knh_bytes_t base = {{str}, loc};
				loc = knh_bytes_mlen(base);
			}
		}
		else {
			char ebuf[K_ERRBUFSIZE];
			re->spi->regerror(res, re->reg, ebuf, K_ERRBUFSIZE);
			ctx->api->ebilog(ctx, re->spi->name, "regexec", LOG_WARNING, "errmsg='%s'", ebuf);
		}

	}
	RETURNi_(loc);
}

/* ------------------------------------------------------------------------ */
//## @Const method Int String.lastIndexOf(Regex re);

static METHOD String_lastIndexOf(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	KNH_TODO("function, assigned to NAKATA!!");
}

/* ------------------------------------------------------------------------ */

static int knh_bytes_equals_(knh_bytes_t base, size_t s, knh_bytes_t target)
{
	size_t i;
	for(i = 1; i < target.len; i++) {
		if(base.ustr[s+i] != target.ustr[i]) return 0;
	}
	return 1;
}

/* ------------------------------------------------------------------------ */
//## @Const method String String.replace(Regex re, String s);

static METHOD String_replace(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	if(knh_Regex_isSTRREGEX(sfp[1].re)) { /* @author nakata */
		knh_bytes_t base = S_tobytes(sfp[0].s);
		knh_bytes_t target = S_tobytes((sfp[1].re)->pattern);
		knh_bytes_t alt = S_tobytes(sfp[2].s);
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		int search_flag= 0, ch = target.ustr[0];
		size_t i;
		if (base.len == 0 || target.len == 0 || base.len < target.len) {
			RETURN_(sfp[0].o);
		}
		for(i = 0; i < base.len - target.len+1; i++) {
			if(base.ustr[i] == ch && knh_bytes_equals_(base, i, target)) {
				knh_Bytes_write(ctx, cwb->ba, alt);
				i += target.len - 1;
				search_flag = 1;
			}else {
				knh_Bytes_putc(ctx, cwb->ba, base.ustr[i]);
			}
		}
		if(search_flag == 0) {
			RETURN_(sfp[0].o);
		}
		else {
			knh_bytes_t leftover = {{base.text + i}, base.len - i};
			knh_Bytes_write(ctx, cwb->ba, leftover);
			RETURN_(knh_cwb_newString(ctx, cwb));
		}
	}
	else {
		//knh_Regex_t *re = sfp[1].re;
		KNH_TODO("function, assigned to HIRA!!");
	}
}

/* ------------------------------------------------------------------------ */

static knh_Array_t *knh_String_toCharArray(Ctx *ctx, knh_String_t *bs, int istrim)
{
	knh_bytes_t base = S_tobytes(bs);
	if(knh_String_isASCII(bs)) {
		size_t i, n = base.len;
		knh_Array_t *a = new_Array(ctx, CLASS_String, n);
		for(i = 0; i < n; i++) {
			if(istrim && isspace(base.ustr[i])) continue;
			knh_bytes_t sub = {{base.text + i}, 1};
			knh_Array_add(ctx, a, new_String_(ctx, CLASS_String, sub, bs));
		}
		return a;
	}
	else {
		size_t i, n = knh_bytes_mlen(base);
		knh_Array_t *a = new_Array(ctx, CLASS_String, n);
		for(i = 0; i < n; i++) {
			if(istrim && isspace(base.ustr[i])) continue;
			knh_bytes_t sub = knh_bytes_mofflen(base, n, 1);
			knh_Array_add(ctx, a, new_String_(ctx, CLASS_String, sub, bs));
		}
		return a;
	}
}

/* ------------------------------------------------------------------------ */

static knh_bytes_t knh_bytes_trim(knh_bytes_t t)
{
	while(isspace(t.ustr[0])) {
		t.ustr++;
		t.len--;
	}
	if(t.len == 0) return t;
	while(isspace(t.ustr[t.len-1])) {
		t.len--;
		if(t.len == 0) return t;
	}
	return t;
}

/* ------------------------------------------------------------------------ */
//## @Const method String[] String.split(Regex re);

static METHOD String_split(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_String_t *s0 = sfp[0].s;
	knh_Regex_t *re = sfp[1].re;
	knh_Array_t *a = NULL;
	if(IS_NULL(re)) {
		a = knh_String_toCharArray(ctx, s0, 0);
	}
	else if(knh_Regex_isSTRREGEX(re)) {
		int istrim = 0;
		knh_bytes_t delim = S_tobytes((re)->pattern);
		if(delim.len == 0) {
			a = knh_String_toCharArray(ctx, s0, 0);
		}
		else {
			knh_bytes_t base = S_tobytes(s0);
			a = new_Array(ctx, CLASS_String, 8);
			while(1) {
				knh_index_t loc = knh_bytes_indexOf(base, delim);
				if(loc == -1) {
					if(istrim) base = knh_bytes_trim(base);
					knh_Array_add(ctx, a, new_String_(ctx, CLASS_String, base, s0));
					break;
				}
				else if(loc == 0) {
					knh_Array_add_(ctx, a, UPCAST(TS_EMPTY));
				}
				else {
					knh_bytes_t t = knh_bytes_first(base, loc);
					if(istrim) t = knh_bytes_trim(t);
					knh_Array_add(ctx, a, new_String_(ctx, CLASS_String, t, s0));
				}
				base.ustr = base.ustr + loc + delim.len;
				base.len = base.len - loc - delim.len;
			}
		}
	}
	else {
		const char *str = S_tochar(s0);
		knh_regmatch_t pmatch[K_REGEX_MATCHSIZE];
		int res = re->spi->regexec(ctx, re->reg, str, K_REGEX_MATCHSIZE, pmatch, 0);
		a = new_Array(ctx, CLASS_String, K_REGEX_MATCHSIZE);
		if(res == 0) {
			knh_bytes_t sub = S_tobytes(s0);
			int i;
			for(i = 0; i < K_REGEX_MATCHSIZE; i++) {
				if(pmatch[i].rm_so == -1) break;
				//DBG_P("[%d], rm_so=%d, rm_eo=%d", i, pmatch[i].rm_so, pmatch[i].rm_eo);
				sub.text = str + pmatch[i].rm_so;
				sub.len = pmatch[i].rm_eo - pmatch[i].rm_so;
				knh_Array_add(ctx, a, new_String_(ctx, CLASS_String, sub, s0));
			}
		}
		else {
			char ebuf[K_ERRBUFSIZE];
			re->spi->regerror(res, re->reg, ebuf, K_ERRBUFSIZE);
			ctx->api->ebilog(ctx, re->spi->name, "regexec", LOG_WARNING, "errmsg='%s'", ebuf);
		}
	}
	RETURN_(a);
}


/* ------------------------------------------------------------------------ */
//## @Const method Bytes Bytes.(Converter enc);

static METHOD Bytes_convert(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_Converter_t *c = sfp[1].conv;
	c->dspi->conv(ctx, c->conv, BA_tobytes(sfp[0].ba), cwb->ba);
	knh_Bytes_t *ba = new_Bytes(ctx, knh_cwb_size(cwb));
	knh_Bytes_write(ctx, ba, knh_cwb_tobytes(cwb));
	knh_cwb_close(cwb);
	RETURN_(ba);
}

/* ------------------------------------------------------------------------ */
//## @Const method Bytes String.encode(StringEncoder enc);

static METHOD String_encode(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_Converter_t *c = sfp[1].conv;
	c->dspi->enc(ctx, c->conv, S_tobytes(sfp[0].s), cwb->ba);
	knh_Bytes_t *ba = new_Bytes(ctx, knh_cwb_size(cwb));
	knh_Bytes_write(ctx, ba, knh_cwb_tobytes(cwb));
	knh_cwb_close(cwb);
	RETURN_(ba);
}

/* ------------------------------------------------------------------------ */
//## @Const method String Bytes.decode(StringDecoder c);

static METHOD Bytes_toString(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_Converter_t *c = sfp[1].conv;
	c->dspi->dec(ctx, c->conv, BA_tobytes(sfp[0].ba), cwb->ba);
	RETURN_(knh_cwb_newString(ctx, cwb));
}

/* ------------------------------------------------------------------------ */
//## @Const method String String.(StringConverter c);
//## @Const method String String.convert(StringConverter c);

static METHOD String_convert(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_Converter_t *c = sfp[1].conv;
	c->dspi->sconv(ctx, c->conv, S_tobytes(sfp[0].s), cwb->ba);
	RETURN_(knh_cwb_newString(ctx, cwb));
}

/* ------------------------------------------------------------------------ */
//## @Const method String String.trim();

static METHOD String_trim(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_bytes_t t = S_tobytes(sfp[0].s);
	knh_bytes_t t2 = knh_bytes_trim(t);
	knh_String_t *s = sfp[0].s;
	if(t.len > t2.len) {
		s = new_String_(ctx, CLASS_String, t2, s);
	}
	RETURN_(s);
}

/* ------------------------------------------------------------------------ */

static int knh_String_opCASE(Ctx *ctx, knh_String_t *s, knh_Regex_t *re)
{
	const char *str = ctx->api->tochar(ctx, s);
	knh_regmatch_t pmatch[K_REGEX_MATCHSIZE];
	int res = re->spi->regexec(ctx, re->reg, str, K_REGEX_MATCHSIZE, pmatch, 0);
	return (res == 0);
}

/* ------------------------------------------------------------------------ */
//## method Boolean String.opCASE(Regex! re);

static METHOD String_opCASE(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_String_opCASE(ctx, sfp[0].s, (knh_Regex_t*)sfp[1].o));
}

/* ------------------------------------------------------------------------ */
//## method Boolean Regex.opCASE(String s);

static METHOD Regex_opCASE(Ctx *ctx, knh_sfp_t *sfp, long rix)
{

	RETURNb_(knh_String_opCASE(ctx, sfp[1].s, (knh_Regex_t*)sfp[0].o));
}

/* ------------------------------------------------------------------------ */

#endif/*K_USING_DEFAULTAPI*/

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
