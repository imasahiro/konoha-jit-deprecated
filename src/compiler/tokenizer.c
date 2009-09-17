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

/* ======================================================================== */
/* [data] */

#define TKSTACK_MAXSIZ 32
#ifndef KONOHA_NAME_BUFSIZ
#define KONOHA_NAME_BUFSIZ  80
#endif

#ifndef KONOHA_NAME_MAXSIZ
#define KONOHA_NAME_MAXSIZ  80
#endif

/* ======================================================================== */
/* [PERROR] */

static
void knh_InputStream_perror(Ctx *ctx, knh_InputStream_t *in, int pe, char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	knh_vperror(ctx, DP(in)->uri, DP(in)->line, pe, fmt, ap);
	va_end(ap);
}

/* ------------------------------------------------------------------------ */
/* [SYMBOL] */

knh_String_t *new_StringSYMBOL(Ctx *ctx, knh_bytes_t t)
{
	knh_DictMap_t *symbolDictMap = DP((ctx)->abr)->symbolDictMap;
	knh_index_t idx = knh_DictMap_index(symbolDictMap, t);
	if(idx == -1) {
		knh_String_t *s = new_String(ctx, t, NULL);
		knh_DictMap_set(ctx, symbolDictMap, s, UP(s));
		return s;
	}
	else {
		return knh_DictMap_keyAt(symbolDictMap, idx);
	}
}

/* ------------------------------------------------------------------------ */

knh_Token_t *knh_cwb_newTokenSYMBOL(Ctx *ctx, knh_cwb_t *cwb, knh_flag_t flag, knh_InputStream_t *in)
{
	knh_bytes_t t = knh_cwb_tobytes(cwb);
	knh_token_t tt = TT_NAME;
	char namebuf[KONOHA_NAME_BUFSIZ];
	size_t i, size;
	if(t.buf[0] == '.' && islower(t.buf[1])) {   /* .name   => name */
		flag |= FLAG_Token_TopDot;
		t.buf++; t.len--;
	}
	if(t.len > 1 && t.buf[t.len-1] == '*' && t.buf[t.len-2] == '.') {
		/* name.* => name */
		flag |= FLAG_Token_TailWildCard;
		t.len -= 2;
	}
	if(t.len > 0 && t.buf[t.len-1] == '.') {
		/* name. => name */
		t.len--;
	}
	for(i = 0; i < KONOHA_NAME_BUFSIZ - 1; i++) {
		if(i < t.len) namebuf[i] = t.buf[i];
		else break;
	}
	size = i;
	namebuf[size] = 0;
	if(KONOHA_NAME_BUFSIZ < t.len) {
		knh_InputStream_perror(ctx, in, KERR_ERRATA, "too long name: '%B' => '%s'", t, namebuf);
	}
	knh_cwb_subclear(cwb, 0);
	{
		size_t s = 0, dots = 0;
		knh_token_t tt_prev = TT_EOT;
		while(s < size) {
			if(islower(namebuf[s]) || namebuf[s] == '_') {
				while(namebuf[s] == '_') {
					knh_cwb_putc(ctx, cwb, '_');
					s++;
				}
				for(i = s; i < size; i++) {
					if(namebuf[i] == '_' && isalnum(namebuf[i+1])) {
						i++;
						if(islower(namebuf[i])) {
							knh_cwb_putc(ctx, cwb, toupper(namebuf[i]));
							continue;
						}
					}
					knh_cwb_putc(ctx, cwb, namebuf[i]);
					if(namebuf[i] == '.') {
						dots++;
						break;
					}
				}
				tt_prev = tt; tt = TT_NAME;
				s = i + 1;
			}
			else {
				int hasLower = 0, hasScore = 0, alphac = 0;
				for(i = s; i < size; i++) {
					if(namebuf[i] == '_') {
						if(hasLower) continue;
						hasScore = 1;
					}
					else if(islower(namebuf[i])) {
						hasLower = 1;
						alphac++;
					}
					else if(isupper(namebuf[i])) {
						alphac++;
					}
					knh_cwb_putc(ctx, cwb, namebuf[i]);
					if(namebuf[i] == '.') {
						dots++;
						break;
					}
				}
				tt_prev = tt;
				tt = (hasLower || alphac == 1) ? TT_TYPEN : TT_CONSTN;
				s = i + 1;
			}
			if(s < size) {
				while(namebuf[s] == '_') s++;
			}
		}
		//DBG2_P("dots=%d, '%s'.'%s'", dots, knh_token_tochar(tt_prev), knh_token_tochar(tt));
		if(dots > 0) {
			if(tt_prev == TT_TYPEN) {
				if(tt == TT_NAME) tt = TT_CMETHODN; else tt = TT_CCONSTN;
			}
		}
		else {
			/* void, var, byte is a special name for type */
			if(tt == TT_NAME) {
				if(namebuf[0] == 'v' && namebuf[1] == 'o' && namebuf[2] == 'i' && namebuf[3] == 'd' && namebuf[4] == 0) {
					tt = TT_TYPEN;
				}
				if(namebuf[0] == 'v' && namebuf[1] == 'a' && namebuf[2] == 'r' && namebuf[3] == 0) {
					tt = TT_TYPEN;
				}
				if(namebuf[0] == 'b' && namebuf[1] == 'y' && namebuf[2] == 't' && namebuf[3] == 'e' && namebuf[4] == 0) {
					tt = TT_TYPEN;
				}
			}
		}
		{
			knh_Token_t *tk = new_Token(ctx, flag, DP(in)->uri, DP(in)->line, tt);
			knh_bytes_t t = knh_cwb_tobytes(cwb);
//		DBG2_(
//		if(!knh_bytes_equals(t, B(name))) {
//			DBG2_P("(%s) '%s' ==> '%s'", knh_token_tochar(tt), (char*)t.buf, name);
//		});
			KNH_SETv(ctx, DP(tk)->data, new_StringSYMBOL(ctx, t));
			knh_cwb_close(cwb);
			return tk;
		}
	}
}

/* ======================================================================== */
/* [parse] */

static
knh_bool_t knh_Bytes_isTupleQuote(knh_Bytes_t *o, knh_intptr_t ch)
{
	if(o->size > 2 &&
		o->buf[o->size-1] == ch &&
		o->buf[o->size-2] == ch &&
		o->buf[o->size-3] == ch) return 1;
	return 0;
}

/* ------------------------------------------------------------------------ */
/* [Token] */

static
knh_Token_t *knh_cwb_newToken(Ctx *ctx, knh_cwb_t *cwb, knh_token_t tt, knh_InputStream_t *in)
{
	knh_Token_t *tk = new_Token(ctx, 0, DP(in)->uri, DP(in)->line, tt);
	knh_bytes_t t = knh_cwb_tobytes(cwb);
	//DBG2_P("tt=%s, '%s', len=%d", knh_token_tochar(tt), t.buf, t.len);
	if(tt == TT_METAN && islower(t.buf[0])) {
		t.buf[0] = toupper(t.buf[0]);
	}
	else if(t.len == 0) {
		KNH_SETv(ctx, DP(tk)->data, TS_EMPTY);
	}
	else {
		KNH_SETv(ctx, DP(tk)->data, new_String__cwbconv(ctx, cwb, DP(in)->bconv));
	}
	return tk;
}

/* ------------------------------------------------------------------------ */

static
void knh_Token_padd(Ctx *ctx, knh_Token_t *tkP, int *BOL, knh_Token_t *tk)
{
	if(*BOL == 1) {
		knh_Token_setBOL(tk, 1); *BOL = 0;
	}
	if(TT_(tk) == TT_SEMICOLON || knh_token_isBeginOfStmt(TT_(tk))) {
		knh_Token_setBOL(tk, 1);
	}
	knh_Token_tokens_add(ctx, tkP, tk);
}

/* ------------------------------------------------------------------------ */

static
void knh_Token_add_space(Ctx *ctx, knh_Token_t *tk, int *BOL, knh_cwb_t* cwb, knh_InputStream_t *in)
{
	if(knh_cwb_size(cwb) == 0) return;
	knh_Token_padd(ctx, tk, BOL, knh_cwb_parseToken(ctx, cwb, 0, in));
	knh_cwb_close(cwb);
}

/* ------------------------------------------------------------------------ */
/* [join] */

static
void knh_write_TokenTYPEN(Ctx *ctx, knh_OutputStream_t *w, knh_Token_t *tk)
{
	if(knh_Token_isExceptionType(tk)) {
		knh_write(ctx, w, STEXT("Exception"));
	}
	else {
		knh_write(ctx, w, knh_Token_tobytes(ctx, tk));
	}
	if(knh_Token_isNotNullType(tk)) {
		knh_putc(ctx, w, '!');
	}
	if(knh_Token_isNullableType(tk)) {
		knh_putc(ctx, w, '?');
	}
}

/* ------------------------------------------------------------------------ */

static
int knh_Token_toClosureType(Ctx *ctx, knh_Token_t *tk0, knh_Token_t *tk1)
{
	knh_tkc_t tcbuf, *tc = knh_Token_tc(ctx, tk1, &tcbuf);
	int i, c = 0;
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	if(tc->e == 0) {
		return 0;
	}
	knh_write_TokenTYPEN(ctx, cwb->w, tk0);
	knh_putc(ctx, cwb->w, '(');
	for(i = 0; i < tc->e; i++) {
		if(TT_(tc->ts[i]) == TT_COMMA) continue;
		if(TT_(tc->ts[i]) != TT_TYPEN) {
			knh_cwb_close(cwb);
			return 0;
		}
		if(c > 0) knh_putc(ctx, cwb->w, ',');
		knh_write_TokenTYPEN(ctx, cwb->w, tc->ts[i]);
		c++;
	}
	if(c > 3) {
		KNH_SETv(ctx, DP(tk0)->data, ctx->share->ClassTable[CLASS_Closure].sname);
		knh_cwb_close(cwb);
		return 1;
	}
	knh_putc(ctx, cwb->w, ')');
	KNH_SETv(ctx, DP(tk0)->data, knh_cwb_newString(ctx, cwb));
	return 1;
}

/* ------------------------------------------------------------------------ */

static
int knh_Token_joinT(Ctx *ctx, knh_Array_t *a, knh_Token_t *tkT, int n)
{
	size_t asize = knh_Array_size(a);
	for(;n < asize; n++) {
		knh_Token_t *tkN = (knh_Token_t*)knh_Array_n(a, n);
		if(TT_(tkN) == TT_LT) { /* @TEST T<T, T> */
			size_t i, s = n;
			for(i = s; i < asize; i++) {
				knh_Token_t *tk = (knh_Token_t*)knh_Array_n(a, i);
				if(TT_(tk) == TT_GT) {
					n = i;
					break;
				}
			}
			if(i < asize) { /* found > */
				int comma = 0;
				knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
				knh_write(ctx, cwb->w, knh_Token_tobytes(ctx, tkT));
				knh_putc(ctx, cwb->w, '<');
				for(i = s + 1 ; i < n; i++) {
					knh_Token_t *tk = (knh_Token_t*)knh_Array_n(a, i);
					if(TT_(tk) == TT_TYPEN) {
						i = knh_Token_joinT(ctx, a, tk, i+1);
						knh_write(ctx, cwb->w, knh_Token_tobytes(ctx, tk));
						comma = 0;
					}
					else if(TT_(tk) == TT_COMMA) {
						if(comma == 0) knh_putc(ctx, cwb->w, ',');
						comma = 1;
					}
					else {
						knh_Token_perror(ctx, tk, KERR_DWARN, "not type: %s", sToken(tk));
					}
				}
				knh_putc(ctx, cwb->w, '>');
				KNH_SETv(ctx, DP(tkT)->data, knh_cwb_newString(ctx, cwb));
				continue;
			}
			break;
		}
		if(TT_(tkN) == TT_PARENTHESIS && knh_Token_toClosureType(ctx, tkT, tkN)) {
			/* T(T) */
			continue;
		}
		if(TT_(tkN) == TT_BRANCET && IS_NULL(DP(tkN)->data)) { /* T[] */
			knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
			knh_Bytes_write(ctx, cwb->ba, knh_String_tobytes(DP(tkT)->text));
			knh_Bytes_write(ctx, cwb->ba, STEXT("[]"));
			KNH_SETv(ctx, DP(tkT)->data, knh_cwb_newString(ctx, cwb));
			continue;
		}
		if(TT_(tkN) == TT_ITR) {  /* T.. */
			knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
			knh_Bytes_write(ctx, cwb->ba, knh_String_tobytes(DP(tkT)->text));
			knh_Bytes_write(ctx, cwb->ba, STEXT(".."));
			KNH_SETv(ctx, DP(tkT)->data, knh_cwb_newString(ctx, cwb));
			continue;
		}
		if(TT_(tkN) == TT_NOT) {  /* T! */
			if(TTn_(tkN) == TT_NOT) { /* T!! */
				knh_Token_setExceptionType(tkT, 1);
				n++;
			}
			else {
				knh_Token_setNotNullType(tkT, 1);
			}
			continue;
		}
		if(TT_(tkN) == TT_QUESTION) {  /* T? */
			knh_Token_setNullableType(tkT, 1);
			continue;
		}
		break;
	}
	return n-1;
}

/* ------------------------------------------------------------------------ */

static
void knh_Token_join(Ctx *ctx, knh_Token_t *o)
{
	knh_Array_t *a = (knh_Array_t*)DP(o)->data, *newa = NULL;
	if(!IS_Array(a) || knh_Array_size(a) == 1) return;
	int i, size = knh_Array_size(a) - 1;
	for(i = 0; i < size ; i++) {
		knh_Token_t *tk0 =  (knh_Token_t*)knh_Array_n(a, i);
		knh_Token_t *tk1 = (knh_Token_t*)knh_Array_n(a, i+1);
		TTn_(tk0) = TT_(tk1);
	}

	newa = new_Array0(ctx, size + 1);
	for(i = 0; i < size ; i++) {
		knh_Token_t *tk0 =  (knh_Token_t*)knh_Array_n(a, i);
		if(TT_(tk0) == TT_TYPEN) {
			/* T[][] */
			i = knh_Token_joinT(ctx, a, tk0, i+1);
		}
		else if(TT_(tk0) == TT_CONSTN) {
			knh_Token_t *tkN = (knh_Token_t*)knh_Array_n(a, i+1);
			if(TT_(tkN) == TT_NOT) {  /* T! */
				if(TTn_(tkN) == TT_NOT) { /* T!! */
					TT_(tk0) = TT_TYPEN;
					knh_Token_setExceptionType(tk0, 1);
					i += 2;
				}
			}
		}
		else if(TT_(tk0) == TT_STR || TT_(tk0) == TT_TSTR || TT_(tk0) == TT_TSTR) {
			/* @TEST "hello" "world" */
			knh_Token_t *tkP = tk0, *tkN = (knh_Token_t*)knh_Array_n(a, i+1);
			int c = 0;
			knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
			while(TT_(tkN) == TT_STR || TT_(tkN) == TT_TSTR || TT_(tkN) == TT_ESTR) {
				if(c == 0) {
					knh_Bytes_write(ctx, cwb->ba, knh_String_tobytes(DP(tk0)->text));
					c++;
				}
				if(SP(tkP)->line < SP(tkN)->line) {
					knh_Bytes_putc(ctx, cwb->ba, '\n');
				}
				knh_Bytes_write(ctx, cwb->ba, knh_String_tobytes(DP(tkN)->text));
				tkP = tkN;
				i++;
				if(i+1 < knh_Array_size(a)) {
					tkN =  (knh_Token_t*)knh_Array_n(a, i+1);
				}
				else {
					break;
				}
			}
			if(knh_cwb_size(cwb) > 0) {
				KNH_SETv(ctx, DP(tk0)->data, knh_cwb_newString(ctx, cwb));
			}
		}
		else if(TT_(tk0) == TT_FORMAT && TTn_(tk0) == TT_PARENTHESIS) {
			TT_(tk0) = TT_NAME;
			knh_Token_setBOL(tk0, 0);
			KNH_SETv(ctx, DP(tk0)->data, new_StringSYMBOL(ctx, STEXT("format")));
		}
		else if(TT_(tk0) == TT_MOD) {
			knh_Token_t *tkN =  (knh_Token_t*)knh_Array_n(a, i+1);
			if(TTn_(tkN) == TT_PARENTHESIS &&
				(TT_(tkN) == TT_NAME || TT_(tkN) == TT_TYPEN || TT_(tkN) == TT_CONSTN)) {
				TT_(tkN) = TT_MT;
				tk0 = tkN;
				i++;
			}
		}
		knh_Array_add(ctx, newa, UP(tk0));
	}
	if(i < knh_Array_size(a)) {
		knh_Array_add(ctx, newa, knh_Array_n(a, i));
	}

	size = knh_Array_size(newa) - 1;
	for(i = 0; i < size ; i++) {
		knh_Token_t *tk0 =  (knh_Token_t*)knh_Array_n(newa, i);
		knh_Token_t *tk1 = (knh_Token_t*)knh_Array_n(newa, i+1);
		TTn_(tk0) = TT_(tk1);
	}
	TTn_((knh_Token_t*)knh_Array_n(newa, size)) = TT_EOT;
	KNH_SETv(ctx, DP(o)->data, newa);
}

/* ------------------------------------------------------------------------ */

static
knh_token_t tt_(int ch)
{
	switch(ch) {
		case '{': case '}': return TT_BRACE;
		case '(': case ')': return TT_PARENTHESIS;
		case '[': case ']': return TT_BRANCET;
		case '@': return TT_METAN;
		case '$': return TT_PROPN;
		case '%': return TT_MT;
		case ';': return TT_SEMICOLON;
		case ',': return TT_COMMA;
		case '"': return TT_STR;
		case '\'': return TT_TSTR;
		case '`': return TT_ESTR;
	}
	DBG2_P("unknown ch=%c", ch);
	return TT_ERR;
}

/* ------------------------------------------------------------------------ */

static
int knh_InputStream_getc__urlencoded(Ctx *ctx, knh_InputStream_t *in)
{
	int num, ch = knh_InputStream_getc(ctx, in);
	if(isdigit(ch)) {
		num = (ch - '0') * 16;
	}else if('a' <= ch && ch <= 'f') {
		num = (ch - 'a' + 10) * 16;
	}else if('A' <= ch && ch <= 'F') {
		num = (ch - 'A' + 10) * 16;
	}else {
		if(ch == '\n') return EOF;
		return ch;
	}
	ch = knh_InputStream_getc(ctx, in);
	if(isdigit(ch)) {
		num += (ch - '0');
	}else if('a' <= ch && ch <= 'f') {
		num += (ch - 'a' + 10);
	}else if('A' <= ch && ch <= 'F') {
		num += (ch - 'A' + 10);
	}else {
		if(ch == '\n') return EOF;
		return ch;
	}
	return num;
}

/* ------------------------------------------------------------------------ */

static
int knh_InputStream_skipBLOCK(Ctx *ctx, knh_InputStream_t *in, int prev, knh_Token_t *tkROOT, knh_Token_t **tks, int tkslevel)
{
	int ch, endch = '}';
	if(tkROOT == NULL) {
		return tkslevel;
	}
	if(TT_(tkROOT) == TT_BRACE) endch = ']';
	TT_(tkROOT) = TT_ERR;
	KNH_SETv(ctx, DP(tkROOT)->data, KNH_NULL);

	MAIN_PART:
	while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
		if(prev == '\n' && ch == endch) {
			knh_InputStream_perror(ctx, in,
				KERR_DWARN, _("ignored this block from %d to %d lines"), (int)SP(tkROOT)->line, (int)DP(in)->line);
			goto STACK_POP;
		}
		if(prev == '/' && ch == '*') {
			prev = 0;
			goto BLOCK_COMMENT;
		}
		prev = ch;
	}
	goto STACK_POP;

	BLOCK_COMMENT:
	{
		int nest = 1;
		while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
			if(prev == '*' && ch == '/') {
				nest--;
				if(nest == 0) goto MAIN_PART;
			}else if(prev == '/' && ch == '*') {
				nest++;
			}
			prev = ch;
		}
	}

	STACK_POP:;
	while(tkslevel > 0) {
		if(tks[tkslevel] == tkROOT) {
			break;
		}
		tks[tkslevel] = NULL;
		tkslevel--;
	}
	tks[tkslevel] = NULL;
	tkslevel--;
	return tkslevel;
}

/* ======================================================================== */

static knh_token_t knh_Token_lastTT(knh_Token_t *tk)
{
	if(IS_Array(DP(tk)->list)) {
		return TT_((knh_Token_t*)knh_Array_last(DP(tk)->list));
	}
	else if(IS_Token(DP(tk)->data)) {
		return TT_((knh_Token_t*)DP(tk)->data);
	}
	return TT_EOT;
}

/* ======================================================================== */
/* [parse] */

void knh_InputStream_parseToken(Ctx *ctx, knh_InputStream_t *in, knh_Token_t *tk)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	int ch, prev=0, indent = 0, pindent, isBeginOfLine = 0;
	knh_Token_t *tks[TKSTACK_MAXSIZ] = {NULL};
	knh_Token_t *tkROOT = NULL;   // the first nested block { }
	int tkslevel = 0, equote = 1;
	tks[tkslevel] = tk;

	INDENT_PART:;
	pindent = indent;
	indent = 0;
	while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
		if(ch == '\t') {
			indent+=3;
		}
		else if(ch == ' ') {
			indent++;
		}
		else if(ch == '/' || ch == '\n' || ch == '\r' || ch == EOF) {
			goto MAIN_PART_INLOOP;
		}
		else {
			break;
		}
	}
	isBeginOfLine = 0;
	if(pindent == indent && TT_(tks[tkslevel]) == TT_BRACE) {
		isBeginOfLine = 1;
	}
	goto MAIN_PART_INLOOP;

	MAIN_PART:;
	while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
		MAIN_PART_INLOOP:;

		switch(ch) {
		case '\n':
			knh_Token_add_space(ctx, tks[tkslevel], &isBeginOfLine, cwb, in);
		goto INDENT_PART;

		case '\t': case ' ': case '\v': case '\r':
			knh_Token_add_space(ctx, tks[tkslevel], &isBeginOfLine, cwb, in);
			prev = ch;

		goto MAIN_PART;

		case '{': case '[': case '(':
			knh_Token_add_space(ctx, tks[tkslevel], &isBeginOfLine, cwb, in);
			if(tkslevel < (TKSTACK_MAXSIZ - 1)) {
				knh_Token_t *intk = new_Token(ctx,
						(equote == 2) ? FLAG_Token_TailWildCard : 0,
								DP(in)->uri, DP(in)->line, tt_(ch));
				knh_Token_padd(ctx, tks[tkslevel], &isBeginOfLine, intk);
				tkslevel++;
				tks[tkslevel] = intk;
				if(tkROOT == NULL && ch != '(') {
					tkROOT = intk;
				}
			}
			else {
				knh_InputStream_perror(ctx, in, KERR_ERROR, _("too many of nested blocks"));
				tkslevel = knh_InputStream_skipBLOCK(ctx, in, prev, tkROOT, tks, tkslevel);
				tkROOT = NULL;
			}
			equote= 1;
			goto MAIN_PART;

		case '}':
			knh_Token_add_space(ctx, tks[tkslevel], &isBeginOfLine, cwb, in);
			if(tkslevel > 0 && TT_(tks[tkslevel]) == TT_BRACE) {
				knh_Token_join(ctx, tks[tkslevel]);
				tkslevel--;
			}
			else {
				knh_InputStream_perror(ctx, in, KERR_ERROR, _("mismatched ...}"));
				TT_(tks[tkslevel]) = TT_ERR;
				tkslevel = knh_InputStream_skipBLOCK(ctx, in, prev, tkROOT, tks, tkslevel);
				tkROOT = NULL;
			}
			equote = 0;
			goto MAIN_PART;

		case ')':
			knh_Token_add_space(ctx, tks[tkslevel], &isBeginOfLine, cwb, in);
			if(tkslevel > 0 && TT_(tks[tkslevel]) == TT_PARENTHESIS) {
				knh_Token_join(ctx, tks[tkslevel]);
				tkslevel--;
			}
			else {
				knh_InputStream_perror(ctx, in, KERR_ERROR, _("mismatched ...)"));
				TT_(tks[tkslevel]) = TT_ERR;
				tkslevel = knh_InputStream_skipBLOCK(ctx, in, prev, tkROOT, tks, tkslevel);
				tkROOT = NULL;
			}
			equote = 0;
			goto MAIN_PART;

		case ']':
			knh_Token_add_space(ctx, tks[tkslevel], &isBeginOfLine, cwb, in);
			if(tkslevel > 0 && TT_(tks[tkslevel]) == TT_BRANCET) {
				knh_Token_join(ctx, tks[tkslevel]);
				tkslevel--;
			}
			else {
				knh_InputStream_perror(ctx, in, KERR_ERROR, _("mismatched ...]"));
				TT_(tks[tkslevel]) = TT_ERR;
				tkslevel = knh_InputStream_skipBLOCK(ctx, in, prev, tkROOT, tks, tkslevel);
				tkROOT = NULL;
			}
			equote = 0;
			goto MAIN_PART;

		case '"': case '\'': case '`' :
			knh_Token_add_space(ctx, tks[tkslevel], &isBeginOfLine, cwb, in);
			prev = ch;
			equote = 0;
			goto QUOTED_PART;

		case ';':
			knh_Token_add_space(ctx, tks[tkslevel], &isBeginOfLine, cwb, in);
			knh_Token_padd(ctx, tks[tkslevel], &isBeginOfLine, new_Token(ctx, 0, DP(in)->uri, DP(in)->line, tt_(ch)));
			equote = 1;
			break;

		case ',':
			knh_Token_add_space(ctx, tks[tkslevel], &isBeginOfLine, cwb, in);
			knh_Token_padd(ctx, tks[tkslevel], &isBeginOfLine, new_Token(ctx, 0, DP(in)->uri, DP(in)->line, tt_(ch)));
			equote = 1;
			break;

		case '$':
			knh_Token_add_space(ctx, tks[tkslevel], &isBeginOfLine, cwb, in);
			prev = ch;
			equote = 0;
			goto QNAME_PART;

		case '@':
			knh_Token_add_space(ctx, tks[tkslevel], &isBeginOfLine, cwb, in);
			ch = knh_InputStream_getc(ctx, in);
			if(ch == '"' || ch == '\'' || ch == '`') {
				prev = ch;
				equote = 0;
				goto RAWSTR_PART;
			}
			prev = '@';
			knh_Bytes_putc(ctx, cwb->ba, ch);
			equote = 1;
			goto QNAME_PART;

		case '%':
			knh_Token_add_space(ctx, tks[tkslevel], &isBeginOfLine, cwb, in);
			//DBG2_P("IS EXTENDED QUOTE: %s", (equote) ? "yes" : "no");
			if(equote == 0 && knh_Token_lastTT(tks[tkslevel]) < TT_NUM) {
				equote = 1;
			}

			ch = knh_InputStream_getc(ctx, in);
			prev = '%';
			if(equote == 0 || ch == ' ' || ch == '\t') {
				knh_Bytes_putc(ctx, cwb->ba, '%');
				equote = 1;
				goto OP_PART_INLOOP;
			}
			else if(ch == '(') {  /* %(..) => format(..) */
				knh_Bytes_write(ctx, cwb->ba, STEXT("format"));
				knh_Token_add_space(ctx, tks[tkslevel], &isBeginOfLine, cwb, in);
				equote = 0;
				goto MAIN_PART_INLOOP;
			}
			else {
				equote = 0;
				knh_Bytes_putc(ctx, cwb->ba, ch);
				goto QNAME_PART;
			}

		case '|':
			knh_Token_add_space(ctx, tks[tkslevel], &isBeginOfLine, cwb, in);
			if(tkslevel > 0 && TT_(tks[tkslevel]) == TT_PARENTHESIS
					&& knh_Token_isTailWildCard(tks[tkslevel])) {
				knh_Token_join(ctx, tks[tkslevel]);
				tkslevel--;
				knh_Bytes_write(ctx, cwb->ba, STEXT(".size"));
				knh_Token_add_space(ctx, tks[tkslevel], &isBeginOfLine, cwb, in);
				equote = 0;
				goto MAIN_PART;
			}

			//DBG2_P("IS EXTENDED QUOTE: %s", (equote) ? "yes" : "no");
			if(equote == 0 && knh_Token_lastTT(tks[tkslevel]) < TT_NUM) {
				equote = 1;
			}

			if(equote == 1) {
				equote = 2; /*very dirty orz */
				ch = '(';
				goto MAIN_PART_INLOOP;
			}

		case '+': case '-': case '*': case '=':
		case '&': case '<':
		case '>': case '^': case '!': case '~':
			knh_Token_add_space(ctx, tks[tkslevel], &isBeginOfLine, cwb, in);
			knh_Bytes_putc(ctx, cwb->ba, ch);
			prev = ch;
			equote = 1;
			goto OP_PART;

		case '?':
			equote = 1;
			knh_Token_add_space(ctx, tks[tkslevel], &isBeginOfLine, cwb, in);
			knh_Bytes_putc(ctx, cwb->ba, ch);
			prev = ch;
			goto OP_PART;

		case '/':
			knh_Token_add_space(ctx, tks[tkslevel], &isBeginOfLine, cwb, in);
			ch = knh_InputStream_getc(ctx, in);
			if(ch == '*') {
				goto BLOCK_COMMENT;
			}else if(ch == '/') {
				goto LINE_COMMENT;
			}
			//DBG2_P("IS EXTENDED QUOTE: %s", (equote) ? "yes" : "no");
			prev = '/';
			if(equote == 0 || ch == ' ' || ch == '\t') {
				knh_Bytes_putc(ctx, cwb->ba, '/');
				equote = 1;
				goto OP_PART_INLOOP;
			}
			else {
				equote = 0;
				knh_Bytes_write(ctx, cwb->ba, STEXT("re:"));
				knh_Bytes_putc(ctx, cwb->ba, ch);
				goto RESTR_PART;
			}

		case '#':
			knh_Token_add_space(ctx, tks[tkslevel], &isBeginOfLine, cwb, in);
			goto LINE_STRING;

		case '.':
			equote = 0;
			ch = knh_InputStream_getc(ctx, in);
			if(ch == '.') { /* .. */
				knh_Token_add_space(ctx, tks[tkslevel], &isBeginOfLine, cwb, in);
				knh_Bytes_putc(ctx, cwb->ba, '.');
				knh_Bytes_putc(ctx, cwb->ba, ch);
				prev = '.';
				goto OP_PART;
			}
			else if(ch == '*') {
				knh_Bytes_putc(ctx, cwb->ba, '.');
				knh_Bytes_putc(ctx, cwb->ba, '*');
				knh_Token_add_space(ctx, tks[tkslevel], &isBeginOfLine, cwb, in);
				prev = ' ';
				break;
			}
			else { /* a.b */
				knh_Bytes_putc(ctx, cwb->ba, '.');
				goto MAIN_PART_INLOOP;
			}
			break;

		case ':':
			ch = knh_InputStream_getc(ctx, in);
			equote = 1;
			if(knh_cwb_size(cwb) == 0) {
				if(ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r' || ch == ';') {
					equote = 1;
					knh_Bytes_putc(ctx, cwb->ba, ':');
					knh_Token_add_space(ctx, tks[tkslevel], &isBeginOfLine, cwb, in);
					break;
				}
				knh_Bytes_putc(ctx, cwb->ba, ':');
				goto URN_PART_INLOOP;
			}
			if(ch == '/') {
				knh_Bytes_putc(ctx, cwb->ba, ':');
				knh_Bytes_putc(ctx, cwb->ba, ch);
				goto URN_PART;
			}
			else if(ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r' || ch == ';') {
				knh_Token_padd(ctx, tks[tkslevel], &isBeginOfLine, knh_cwb_newToken(ctx, cwb, TT_LABEL, in));
				goto MAIN_PART_INLOOP;
			}
			else {
				knh_Bytes_putc(ctx, cwb->ba, ':');
				knh_Bytes_putc(ctx, cwb->ba, ch);
			}
			break;

		case '1': case '2': case '3': case '4': case '5':
		case '6': case '7': case '8': case '9': case '0':
			equote = 0;
			if(knh_cwb_size(cwb) == 0) {
				knh_Bytes_putc(ctx, cwb->ba, ch);
				goto NUM_PART;
			}
			else {
				knh_Bytes_putc(ctx, cwb->ba, ch);
			}

		case EOF :
			break;

		default:
			equote = 0;
			if(ch > 127) {
				knh_InputStream_perror(ctx, in, KERR_DWARN, _("unexpected multi-byte character"));
				ch = ' ';
			}
			knh_Bytes_putc(ctx, cwb->ba, ch);
		} /* switch */
		prev = ch;
	}/* while */
	goto L_JOIN;

	NUM_PART:;  /* 0x0000 */
	{
		int unit = 0;
		//DBG2_P("=> NUM_PART");
		while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
			NUM_PART_INLOOP:;
			switch(ch) {
				case '0': case '1': case '2': case '3': case '4':
				case '5': case '6': case '7': case '8': case '9':
				case '_':
					knh_Bytes_putc(ctx, cwb->ba, ch);
					prev = ch;
				break;

				case '.':
					ch = knh_InputStream_getc(ctx, in);
					if(ch == '.') {
						knh_Token_padd(ctx, tks[tkslevel], &isBeginOfLine, knh_cwb_newToken(ctx, cwb, TT_NUM, in));
						knh_Bytes_putc(ctx, cwb->ba, '.');
						goto OP_PART_INLOOP;
					}
					else {
						knh_Bytes_putc(ctx, cwb->ba, '.');
						prev = ch;
						goto NUM_PART_INLOOP;
					}
				break;

				case '[':
					unit++;
					knh_Bytes_putc(ctx, cwb->ba, ch);
				break;

				case ']':   /* 1.0[km/h] */
					unit--;
					if(unit < 0) {
						knh_Token_padd(ctx, tks[tkslevel], &isBeginOfLine, knh_cwb_newToken(ctx, cwb, TT_NUM, in));
						goto MAIN_PART_INLOOP;
					}
					else if(unit == 0) {
						prev = ch;
						knh_Bytes_putc(ctx, cwb->ba, ch);
						knh_Token_padd(ctx, tks[tkslevel], &isBeginOfLine, knh_cwb_newToken(ctx, cwb, TT_NUM, in));
						goto MAIN_PART;
					}
				break;

				default:
					if(unit > 0) {
						if(ch == '<' || ch == '(') {
							knh_InputStream_perror(ctx, in, KERR_DWARN, _("illegal character in tag"));
						}
						else {
							prev = ch;
							knh_Bytes_putc(ctx, cwb->ba, ch);
						}
					}
					else if(isalnum(ch) || ch > 127) {  /* 1.0a */
						prev = ch;
						knh_Bytes_putc(ctx, cwb->ba, ch);
					}
					else {
						if(ch == '-' && (prev == 'e' || prev == 'E')) {
							prev = ch;
							knh_Bytes_putc(ctx, cwb->ba, ch);
							break;
						}
						knh_Token_padd(ctx, tks[tkslevel], &isBeginOfLine, knh_cwb_newToken(ctx, cwb, TT_NUM, in));
						goto MAIN_PART_INLOOP;
					}
				/*break*/
			}
		}
		knh_Token_padd(ctx, tks[tkslevel], &isBeginOfLine, knh_cwb_newToken(ctx, cwb, TT_NUM, in));
	}
	goto MAIN_PART_INLOOP; /* EOF */

	QNAME_PART:;
	KNH_ASSERT(prev == '@' || prev == '$' || prev == '%');
	{
		while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
			if(isalnum(ch) || ch == '_' || ch == ':' || ch == '.') {
				knh_Bytes_putc(ctx, cwb->ba, ch);
				continue;
			}
			if(ch == '*' && prev == '$') {
				knh_Bytes_putc(ctx, cwb->ba, ch);
				continue;
			}
			break;
		}
		knh_Token_padd(ctx, tks[tkslevel], &isBeginOfLine, knh_cwb_newToken(ctx, cwb, tt_(prev), in));
	}
	goto MAIN_PART_INLOOP; /* EOF */

	OP_PART:;
	{
		while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
			OP_PART_INLOOP:;
			knh_bytes_t top = knh_cwb_tobytes(cwb);
			if(knh_bytes_istoken(top, ch)) {
				knh_Bytes_putc(ctx, cwb->ba, ch);
			}
			else {
				knh_Token_add_space(ctx, tks[tkslevel], &isBeginOfLine, cwb, in);
				goto MAIN_PART_INLOOP;
			}
		}
	}
	goto MAIN_PART_INLOOP; /* EOF */

	URN_PART:;
	{
		DBG2_P("=> URN_PART");
		while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
			URN_PART_INLOOP:;
			switch(ch) {
				case '\n': case '\r':
				case ' ':  case '\t':
				case ';':
				case '(': case ')':
				case '{': case '}':
				case '[': case ']':
					knh_Token_padd(ctx, tks[tkslevel], &isBeginOfLine, knh_cwb_newToken(ctx, cwb, TT_URN, in));
					goto MAIN_PART_INLOOP;
				case '%':
					ch = knh_InputStream_getc__urlencoded(ctx, in);
					if(ch == EOF) {
						ch = '\n';
						knh_Token_padd(ctx, tks[tkslevel], &isBeginOfLine, knh_cwb_newToken(ctx, cwb, TT_URN, in));
						goto MAIN_PART_INLOOP;
					}
					knh_Bytes_putc(ctx, cwb->ba, ch);
				default:
					knh_Bytes_putc(ctx, cwb->ba, ch);
			}
		}
		knh_Token_padd(ctx, tks[tkslevel], &isBeginOfLine, knh_cwb_newToken(ctx, cwb, TT_URN, in));
	}/*URN_PART*/
	goto L_EOF;

	RESTR_PART:;
	DBG2_ASSERT(prev == '/');
	{
		while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
			RESTR_PART_INLOOP:;
			if(ch == '\\') {
				ch = knh_InputStream_getc(ctx, in);
				if(ch == '/') {
					knh_Bytes_putc(ctx, cwb->ba, '/');
				}
				else if(ch == '\\') {
					knh_Bytes_putc(ctx, cwb->ba, ch);
					knh_Bytes_putc(ctx, cwb->ba, ch);
				}
				else {
					knh_Bytes_putc(ctx, cwb->ba, '\\');
					goto RESTR_PART_INLOOP;
				}
				continue;
			}
			if(ch == '\n' || ch == '\r' || ch == EOF) {
				knh_InputStream_perror(ctx, in, KERR_EWARN, _("suddenly ending?"));
				knh_Token_padd(ctx, tks[tkslevel], &isBeginOfLine, knh_cwb_newToken(ctx, cwb, tt_(prev), in));
				goto MAIN_PART_INLOOP;
			}
			if(ch == prev) {
				knh_Token_padd(ctx, tks[tkslevel], &isBeginOfLine, knh_cwb_newToken(ctx, cwb, TT_TSTR, in));
				goto MAIN_PART;
			}
			knh_Bytes_putc(ctx, cwb->ba, ch);
		}
		knh_Token_padd(ctx, tks[tkslevel], &isBeginOfLine, knh_cwb_newToken(ctx, cwb, TT_TSTR, in));
	}/*RESTR_PART*/
	goto L_EOF;

	RAWSTR_PART:;
	DBG2_ASSERT(prev == '"' || prev == '\'' || prev == '`');
	{
		//knh_Bytes_putc(ctx, tbuf.ba, prev);
		while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
			if(ch == prev) {
				knh_Token_padd(ctx, tks[tkslevel], &isBeginOfLine, knh_cwb_newToken(ctx, cwb, tt_(prev), in));
				goto MAIN_PART;
			}
			knh_Bytes_putc(ctx, cwb->ba, ch);
		}
		knh_Token_padd(ctx, tks[tkslevel], &isBeginOfLine, knh_cwb_newToken(ctx, cwb, tt_(prev), in));
	}/*RAWSTR_PART*/
	goto L_EOF;

	QUOTED_PART:;
	KNH_ASSERT(prev == '"' || prev == '\'' || prev == '`');
	{
		//knh_Bytes_putc(ctx, tbuf.ba, prev);
		ch = knh_InputStream_getc(ctx, in);
		if(ch == prev) {
			ch = knh_InputStream_getc(ctx, in);
			if(prev == ch) { /* triple quote */
				goto TRIPLE_QUOTED_PART;
			}else {
				knh_Token_padd(ctx, tks[tkslevel], &isBeginOfLine, knh_cwb_newToken(ctx, cwb, tt_(prev), in));
				goto MAIN_PART_INLOOP;
			}
		}
		else {
			goto QUOTED_PART_INLOOP;
		}
		while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
			QUOTED_PART_INLOOP:;
			if(ch == '\\') {
				ch = knh_InputStream_getc(ctx, in);
				switch(ch) {
				case 'n':
					knh_Bytes_putc(ctx, cwb->ba, '\n');
					break;
				case 't':
					knh_Bytes_putc(ctx, cwb->ba, '\t');
					break;
				case 'r':
					knh_Bytes_putc(ctx, cwb->ba, '\r');
					break;
				case '"': case '\'': case '\\':
					knh_Bytes_putc(ctx, cwb->ba, ch);
					break;
				case EOF:

				default:
					knh_Bytes_putc(ctx, cwb->ba, '\\');
					knh_Bytes_putc(ctx, cwb->ba, ch);
				}
				continue;
			}
			if(ch == '\n' || ch == '\r' || ch == EOF) {
				knh_InputStream_perror(ctx, in, KERR_EWARN, _("suddenly ending?"));
				knh_Token_padd(ctx, tks[tkslevel], &isBeginOfLine, knh_cwb_newToken(ctx, cwb, tt_(prev), in));
				goto MAIN_PART_INLOOP;
			}
			if(ch == prev) {
				knh_Token_padd(ctx, tks[tkslevel], &isBeginOfLine, knh_cwb_newToken(ctx, cwb, tt_(prev), in));
				goto MAIN_PART;
			}
			knh_Bytes_putc(ctx, cwb->ba, ch);
		}
	}
	knh_Token_padd(ctx, tks[tkslevel], &isBeginOfLine, knh_cwb_newToken(ctx, cwb, tt_(prev), in));
	goto L_EOF;

	TRIPLE_QUOTED_PART:;
	KNH_ASSERT(prev == '"' || prev == '\'' || prev == '`');
	{
		while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
			if(ch == '\\') {
				ch = knh_InputStream_getc(ctx, in);
				switch(ch) {
				case 'n':
					knh_Bytes_putc(ctx, cwb->ba, '\n');
					break;
				case 't':
					knh_Bytes_putc(ctx, cwb->ba, '\t');
					break;
				case 'r':
					knh_Bytes_putc(ctx, cwb->ba, '\r');
					break;
				case '"': case '\'': case '\\':
					knh_Bytes_putc(ctx, cwb->ba, ch);
					break;
				case EOF:

				default:
					knh_Bytes_putc(ctx, cwb->ba, '\\');
					knh_Bytes_putc(ctx, cwb->ba, ch);
				}
				continue;
			}
			if(ch == '\n' || ch == '\r') {
				if(knh_Bytes_size(cwb->ba) - cwb->pos == 0) {
					DBG2_P("remove first linefeed at triplequote");
					continue;
				}
			}
			knh_Bytes_putc(ctx, cwb->ba, ch);
			if(knh_Bytes_isTupleQuote(cwb->ba, prev)) {
				knh_Bytes_unputc(cwb->ba);
				knh_Bytes_unputc(cwb->ba);
				knh_Bytes_unputc(cwb->ba);
				knh_Token_padd(ctx, tks[tkslevel], &isBeginOfLine, knh_cwb_newToken(ctx, cwb, tt_(prev), in));
				goto MAIN_PART;
			}
		}
		knh_Token_padd(ctx, tks[tkslevel], &isBeginOfLine, knh_cwb_newToken(ctx, cwb, tt_(prev), in));
	}
	goto L_EOF; /* EOF */

	/** COMMENT **/
	LINE_STRING:
	{
		ch = knh_InputStream_getc(ctx, in);
		if(ch == '!') goto LINE_COMMENT; /* #! /usr/local/bin */
		if(ch != ' ') goto LINE_STRING_INLOOP;
		while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
			LINE_STRING_INLOOP:
			if(ch == '\n') {
				knh_Token_padd(ctx, tks[tkslevel], &isBeginOfLine, knh_cwb_newToken(ctx, cwb, TT_ESTR, in));
				goto MAIN_PART_INLOOP;
			}
			knh_Bytes_putc(ctx, cwb->ba, ch);
		}
	}
	goto L_EOF; /* EOF */

	/** COMMENT **/
	LINE_COMMENT:
	{
		while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
			if(ch == '\n') {
				goto MAIN_PART_INLOOP;
			}
		}
	}
	goto L_EOF; /* EOF */

	BLOCK_COMMENT:
	{
		int nest = 1, line = DP(in)->line;
		knh_cwb_write(ctx, cwb, STEXT("/*"));
		while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
			knh_cwb_putc(ctx, cwb, ch);
			if(prev == '*' && ch == '/') {
				nest--;
				if(nest == 0) {
					//DBG2_P("line=%d, isBeginOfLine=%d", DP(in)->line - line, isBeginOfLine);
					if(DP(in)->line - line > 2) {
						knh_Token_padd(ctx, tks[tkslevel], &isBeginOfLine, knh_cwb_newToken(ctx, cwb, TT_DOC, in));
					}
					knh_cwb_close(cwb);
					goto MAIN_PART;
				}
			}else if(prev == '/' && ch == '*') {
				nest++;
			}
			prev = ch;
		}
		knh_cwb_close(cwb);
	}
	goto L_EOF; /* EOF */

	L_EOF:;
	knh_InputStream_perror(ctx, in, KERR_EWARN, _("suddenly comes to end of file"));

	L_JOIN:
	knh_Token_add_space(ctx, tks[tkslevel], &isBeginOfLine, cwb, in);
	while(tkslevel >= 0) {
		knh_Token_join(ctx, tks[tkslevel]);
		tkslevel--;
	}
	return ;
}


/* ======================================================================== */

#ifdef __cplusplus
}
#endif