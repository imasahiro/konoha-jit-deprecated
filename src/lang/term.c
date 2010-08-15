/****************************************************************************
 * KONOHA COPYRIGHT, LICENSE NOTICE, AND DISCRIMER
 *
 * Copyright (c) 2006-2010, Kimio Kuramitsu <kimio at ynu.ac.jp>
 *           (c) 2008-      Konoha Team konohaken@googlegroups.com
 * All rights reserved..
 *
 * You may choose one of the following two licenses when you use konoha.
 * See www.konohaware.org/license.html for further information.
 *
 * (1) GNU Lesser General Public License 3.0 (with K_UNDER_LGPL)
 * (2) Konoha Software Foundation License 1.0
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORSÄ
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
#define USE_B     1
#define USE_bytes_index       1
#define USE_bytes_first       1
#define USE_bytes_equals      1
#define USE_bytes_startsWith  1

#define USE_cwb_open      1
#define USE_cwb_tobytes   1
#define USE_cwb_size      1

#include"commons.h"
#define K_USING_TERMCODE
#include"../../include/konoha/konoha_grammar_.h"
#define TT_SIZE TT_LOR   /* |a| */

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	struct knh_Token_t** ts;
	int meta;
	int c;
	int e;
} tkitr_t;

/* ======================================================================== */
/* [term] */

knh_Token_t* new_Token(Ctx *ctx, knh_flag_t flag, knh_term_t tt)
{
	knh_Token_t *tk = new_(Token);
	SP(tk)->uri   =   0;
	SP(tk)->line  =   0;
	SP(tk)->tt    =   tt;
	DP(tk)->flag0 =   flag;
	return tk;
}

void knh_Stmt_swap(Ctx *ctx, knh_Stmt_t *stmt, size_t n, size_t m)
{
	DBG_ASSERT(n < DP(stmt)->size);
	DBG_ASSERT(m < DP(stmt)->size);
	knh_Term_t *tm = DP(stmt)->terms[n];
	DP(stmt)->terms[n] = DP(stmt)->terms[m];
	DP(stmt)->terms[m] = tm;
}

void knh_Stmt_trimToSize(Ctx *ctx, knh_Stmt_t *stmt, size_t n)
{
	size_t i;
	for(i = n; i < DP(stmt)->size; i++) {
		KNH_FINALv(ctx, DP(stmt)->terms[i]);
	}
	DBG_ASSERT(n < DP(stmt)->size);
	DP(stmt)->size = n;
}

void knh_Stmt_toERR(Ctx *ctx, knh_Stmt_t *stmt, knh_Term_t *tm)
{
	if(STT_(stmt) == STT_ERR) return;
	DBG_P("** %s => STT_ERR", TT__(STT_(stmt)));
	STT_(stmt) = STT_ERR;
	if(tm != NULL) {
		SP(stmt)->uri =  SP(tm)->uri;
		SP(stmt)->line =  SP(tm)->line;
	}
	{
		char buf[256];
		knh_snprintf(buf, sizeof(buf), _("Script!!: you'll have to fix bugs at %s:%d"),
			FILENAME__(SP(stmt)->uri), SP(stmt)->line);
		KNH_SETv(ctx, DP(stmt)->errMsg, new_S(ctx, B(buf)));
		if(DP(stmt)->nextNULL != NULL) {
			KNH_FINALv(ctx, DP(stmt)->nextNULL);
			DP(stmt)->nextNULL = NULL;
		}
	}
}

knh_Stmt_t* new_Stmt2(Ctx *ctx, knh_term_t stt, ...)
{
	knh_Stmt_t *stmt = new_(Stmt);
	knh_Term_t *tm = NULL;
	va_list ap;
	SP(stmt)->stt = stt;
	va_start(ap , stt);
	while((tm = (knh_Term_t*)va_arg(ap, knh_Term_t*)) != NULL) {
		DBG_ASSERT(IS_Token(tm) || IS_Stmt(tm));
		knh_Stmt_add_(ctx, stmt, tm, NULL);
	}
	va_end(ap);
	return stmt;
}

/* ------------------------------------------------------------------------ */

#define ITR_next(itr)        (itr)->c += 1;
#define ITR_hasNext(itr)     ((itr)->c < (itr)->e)
#define ITR_size(itr)        ((itr)->e - (itr)->c)

static tkitr_t* ITR_new(knh_Token_t *tk, tkitr_t *buf)
{
	buf->meta = -1;
	buf->c = 0;
	if(IS_Array(DP(tk)->data)) {
		buf->ts = (DP(tk)->list)->tokens;
		buf->e = knh_Array_size(DP(tk)->list);
	}
	else if(IS_Token(DP(tk)->data)) {
		buf->ts = &(DP(tk)->token);
		buf->e = 1;
	}
	else {
		buf->ts = NULL;
		buf->e = 0;
	}
	return buf;
}

static knh_term_t ITR_tt(tkitr_t *itr)
{
	DBG_ASSERT(itr->c < itr->e);
	return TT_(itr->ts[itr->c]);
}

#define ITR_is(itr, tt) ITR_isN(itr, 0, tt)

static int ITR_isN(tkitr_t *itr, int shift, knh_term_t tt)
{
	int c = itr->c + shift;
	return (c < itr->e && TT_(itr->ts[c]) == tt);
}

#define ITR_tk(itr)     (itr->ts[itr->c])

static knh_Token_t *ITR_nextTK(tkitr_t *itr)
{
	DBG_ASSERT(itr->c < itr->e);
	(itr)->c += 1;
	return (itr)->ts[(itr)->c - 1];
}

static void ITR_chop(tkitr_t *itr, knh_term_t tt)
{
	int e = itr->e - 1;
	if(itr->c <= e && TT_(itr->ts[e]) == tt) {
		itr->e = e;
	}
}

static int ITR_count(tkitr_t *tc, knh_term_t tt)
{
	knh_Token_t **ts = tc->ts;
	int i, cnt = 0;
	for(i = tc->c; i < tc->e; i++) {
		if(TT_(ts[i]) == tt) cnt++;
	}
	return cnt;
}

/* ------------------------------------------------------------------------ */

void knh_write_token(Ctx *ctx,  knh_OutputStream_t *w, knh_Token_t *tk)
{
	if(knh_Token_isDOT(tk)) knh_putc(ctx, w, '.');
	if(knh_Token_isGetter(tk)) knh_write(ctx, w, STEXT("get_"));
	if(knh_Token_isSetter(tk)) knh_write(ctx, w, STEXT("set_"));
	if(IS_bString(DP(tk)->text)) {
		if(TT_(tk) == TT_METAN) knh_putc(ctx, w, '@');
		if(TT_(tk) == TT_PROPN) knh_putc(ctx, w, '$');
		if(TT_(tk) == TT_STR) knh_putc(ctx, w, '"');
		if(TT_(tk) == TT_ESTR) knh_putc(ctx, w, '`');
		if(TT_(tk) == TT_TSTR) knh_putc(ctx, w, '\'');
		knh_write(ctx, w, S_tobytes(DP(tk)->text));
		if(TT_(tk) == TT_STR) knh_putc(ctx, w, '"');
		if(TT_(tk) == TT_ESTR) knh_putc(ctx, w, '`');
		if(TT_(tk) == TT_TSTR) knh_putc(ctx, w, '\'');
		if(knh_Token_isExceptionType(tk)) {
			knh_write(ctx, w, STEXT("!!"));
		}
	}
	else if(TT_(tk) == TT_CID) {
		knh_write_cid(ctx, w, DP(tk)->cid);
	}else if(TT_(tk) == TT_MN) {
		knh_write_mn(ctx, w, DP(tk)->mn);
	}else if(TT_(tk) == TT_LOCAL || TT_(tk) == TT_STACK) {
		knh_write(ctx, w, STEXT("sfp["));
		knh_write_ifmt(ctx, w, K_INT_FMT, DP(tk)->index);
		knh_putc(ctx, w, ']');
	}else if(TT_(tk) == TT_FIELD) {
		knh_write(ctx, w, STEXT("field["));
		knh_write_ifmt(ctx, w, K_INT_FMT, DP(tk)->index);
		knh_putc(ctx, w, ']');
	}else if(TT_(tk) == TT_ASIS) {
		knh_putc(ctx, w, '_');
	}else {
		knh_Method_t *mtd = knh_getSystemFormatter(ctx, knh_Object_cid(DP(tk)->data), MN__k);
		knh_write_Object(ctx, w, ctx->esp, &mtd, DP(tk)->data);
	}
}

void knh_dump_token(Ctx *ctx, knh_OutputStream_t *w, knh_Token_t *tk)
{
	if(tk != NULL && IS_Token(tk)) {
		tkitr_t tcbuf, *itr = ITR_new(tk, &tcbuf);
		knh_write(ctx, w, STEXT("tt=")); knh_write_text(ctx, w, TT__(TT_(tk)));
		if(SP(tk)->line != 0) {
			knh_putc(ctx, w, ':');
			knh_write_ifmt(ctx, w, K_INT_FMT, SP(tk)->line);
		}
		else {
			knh_putc(ctx, w, '*');
		}
		if(SP(tk)->type != TYPE_var) {
			knh_write(ctx, w, STEXT(" type=")); knh_write_type(ctx, w, SP(tk)->type);
		}
		if(ITR_hasNext(itr)) {
			knh_write_begin(ctx, w, 0);
			while(ITR_hasNext(itr)) {
				knh_write_BOL(ctx, w);
				knh_write_ifmt(ctx, w, K_INT_FMT, itr->c);
				knh_write(ctx, w, STEXT("::\t"));
				knh_dump_token(ctx, w, ITR_nextTK(itr));
			}
			knh_write_end(ctx, w, 0);
		}
		else {
			knh_putc(ctx, w, ' ');
			if(IS_bString(DP(tk)->text)) {
				if(knh_Token_isDOT(tk)) knh_putc(ctx, w, '.');
				if(knh_Token_isGetter(tk)) knh_write(ctx, w, STEXT("get_"));
				if(knh_Token_isSetter(tk)) knh_write(ctx, w, STEXT("set_"));
				knh_putc(ctx, w, '"');
				knh_write(ctx, w, S_tobytes(DP(tk)->text));
				knh_putc(ctx, w, '"');
				if(knh_Token_isExceptionType(tk)) {
					knh_write(ctx, w, STEXT("!!"));
				}
			}
			else if(TT_(tk) == TT_CID) {
				knh_write_cid(ctx, w, DP(tk)->cid);
			}else if(TT_(tk) == TT_MN) {
				knh_write_mn(ctx, w, DP(tk)->mn);
			}else if(TT_(tk) == TT_LOCAL) {
				knh_write(ctx, w, STEXT("local="));
				knh_write_ifmt(ctx, w, K_INT_FMT, DP(tk)->index);
			}else if(TT_(tk) == TT_ASIS) {
				knh_putc(ctx, w, '_');
			}else {
				knh_Method_t *mtd = knh_getSystemFormatter(ctx, knh_Object_cid(DP(tk)->data), MN__k);
				knh_write_Object(ctx, w, ctx->esp, &mtd, DP(tk)->data);
			}
		}
	}
	else {
		knh_write(ctx, w, STEXT("not token"));
	}
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */

void knh_dump_stmt(Ctx *ctx, knh_OutputStream_t *w, knh_Stmt_t *stmt, int isNEXT)
{
	L_RESTART:;
	knh_intptr_t i, size;
	knh_putc(ctx, w, '(');
	if(IS_Map(DP(stmt)->metaDictCaseMap)) {
		size = knh_DictCaseMap_size(DP(stmt)->metaDictCaseMap);
		for(i = 0; i < size; i++) {
			knh_String_t *k = knh_DictCaseMap_keyAt(DP(stmt)->metaDictCaseMap, i);
			knh_String_t *v = (knh_String_t*)knh_DictCaseMap_valueAt(DP(stmt)->metaDictCaseMap, i);
			if(k == v) {
				knh_printf(ctx, w, "@%s ", S_tochar(k));
			}
			else {
				knh_printf(ctx, w, "@%s(%O) ", S_tochar(k), v);
			}
		}
	}
	knh_write_text(ctx, w, TT__(STT_(stmt)));
	if(DP(stmt)->size > 0) knh_putc(ctx, w, ' ');
	for(i = 0; i < DP(stmt)->size; i++) {
		if(i > 0) knh_putc(ctx, w, ' ');
		if(IS_Token(DP(stmt)->terms[i])) {
			knh_write_token(ctx, w, DP(stmt)->tokens[i]);
		}else if(IS_Stmt(DP(stmt)->terms[i])) {
			knh_dump_stmt(ctx, w, DP(stmt)->stmts[i], 0/*isNEXT*/);
		}
	}
	knh_putc(ctx, w, ')');
	if(SP(stmt)->type != TYPE_var) {
		knh_write(ctx, w, STEXT(" : "));
		knh_write_type(ctx, w, SP(stmt)->type);
	}

	if(DP(stmt)->nextNULL != NULL) {
		if(isNEXT) {
			knh_write_EOL(ctx, w);
			stmt = DP(stmt)->nextNULL;
			goto L_RESTART;
		}
		else {
			knh_write_dots(ctx, w);
		}
	}
}

/* ------------------------------------------------------------------------ */
/* [tokenizer] */

static void knh_Token_add(Ctx *ctx, knh_Token_t *tk, knh_Token_t *tkc);

static void knh_InputStream_perror(Ctx *ctx, knh_InputStream_t *in, int pe, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	knh_vperror(ctx, DP(in)->uri, DP(in)->line, pe, fmt, ap);
	va_end(ap);
}

static knh_term_t TT_ch(int ch)
{
	switch(ch) {
		case '{': case '}': return TT_BRACE;
		case '(': case ')': return TT_PARENTHESIS;
		case '[': case ']': return TT_BRANCET;
#ifdef TT_SIZE
		case '|': return TT_SIZE;
#endif
//		case '@': return TT_METAN;
//		case '$': return TT_PROPN;
//		case ';': return TT_SEMICOLON;
		case ',': return TT_COMMA;
		case '"': return TT_STR;
		case '\'': return TT_TSTR;
		case '`': return TT_ESTR;
		case '/': return TT_REGEX;
	}
	DBG_P("unknown ch=%c", ch);
	return TT_ERR;
}

static int isTYPE(knh_Token_t *tk)
{
	return (TT_(tk) == TT_TYPE || TT_(tk) == TT_UNAME
		|| TT_(tk) == TT_VOID || TT_(tk) == TT_VAR);
}

knh_Token_t *new_TokenCID(Ctx *ctx, knh_class_t cid)
{
	knh_Token_t *tk = new_(Token);
	TT_(tk) = TT_CID;
	DP(tk)->cid = cid;
	KNH_SETv(ctx, DP(tk)->text, ClassTBL(cid).sname);
	return tk;
}

static knh_Token_t *new_TokenPTYPE(Ctx *ctx, knh_class_t cid, knh_Token_t *tk)
{
	knh_Token_t *tkT = new_Token(ctx, 0, TT_TYPE);
	knh_Term_setLine(tkT, tk);
	knh_Token_add(ctx, tkT, new_TokenCID(ctx, cid));
	knh_Token_add(ctx, tkT, tk);
	return tkT;
}

static int ITR_findPTYPE(tkitr_t *itr)
{
	int c, e = 0, f = 0;
	for(c = itr->e-2; 0 <= c; c--) {
		knh_Token_t *tk = itr->ts[c];
		if(!isTYPE(tk)) break;
		if(e == 1) {
			itr->c = c; return 1;
		}
		c--; tk = itr->ts[c];
		if(TT_(tk) == TT_COMMA) continue;
		if(TT_(tk) == TT_LT) { e = 1; continue; }
		if(f == 0 && TT_(tk) == TT_FUNCARROW) {
			f = 1; continue;
		}
		break;
	}
	return 0;
}

static void knh_Token_toStmtTYPEOF(Ctx *ctx, knh_Token_t *tk);

static void knh_Token_add(Ctx *ctx, knh_Token_t *tk, knh_Token_t *tkc)
{
	knh_Array_t *a;
	knh_Token_t *tkPREV;
	int prev = 0;
	if(SP(tkc)->line == 0) {
		knh_Term_setLine(tkc, tk);
	}
	DBG_ASSERT(SP(tk)->line != 0 && SP(tkc)->line != 0);
	if(TT_(tkc) == TT_SEMICOLON) {
		knh_Token_setBOL(tkc, 1);
	}
	if(IS_NULL(DP(tk)->data)) {
		KNH_SETv(ctx, DP(tk)->data, tkc); return;
	}
	if(IS_Token(DP(tk)->data)) {
		a = new_Array(ctx, CLASS_Any, 2);
		tkPREV = DP(tk)->token;
		knh_Array_add(ctx, a, DP(tk)->data);
		KNH_SETv(ctx, DP(tk)->data, a);
	}
	else {
		DBG_ASSERT(IS_Array(DP(tk)->data));
		a = DP(tk)->list;
		DBG_ASSERT(knh_Array_size(a) > 0);
		prev = knh_Array_size(a)-1;
		tkPREV = knh_TOKENs_n(a, prev);
	}
	knh_Array_add(ctx, a, tkc);

	/* join part */
	if(TT_isSTR(TT_(tkc))) {
		if(TT_isSTR(TT_(tkPREV)) || TT_(tkPREV) == TT_FMTSTR) {
			knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
			knh_Bytes_write(ctx, cwb->ba, S_tobytes(DP(tkPREV)->text));
			if(SP(tkc)->line > SP(tkPREV)->line) {
				knh_Bytes_putc(ctx, cwb->ba, '\n');
			}
			knh_Bytes_write(ctx, cwb->ba, S_tobytes(DP(tkc)->text));
			KNH_SETv(ctx, DP(tkPREV)->data, knh_cwb_newString(ctx, cwb));
			if(TT_(tkc) == TT_ESTR) TT_(tkPREV) = TT_ESTR;
			knh_Array_trimSize(ctx, a, prev+1); return;
		}
		if(TT_(tkPREV) == TT_NAME && ISB(S_tobytes(DP(tkPREV)->text), "%%")) {
			TT_(tkc) = TT_FMTSTR;
			KNH_SETv(ctx, a->list[prev], tkc);
			knh_Array_trimSize(ctx, a, prev+1); return;
		}
	}
	if(TT_isSTR(TT_(tkc)) && TT_(tkPREV) == TT_URN) {
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		knh_Bytes_write(ctx, cwb->ba, S_tobytes(DP(tkPREV)->text));
		knh_Bytes_write(ctx, cwb->ba, S_tobytes(DP(tkc)->text));
		KNH_SETv(ctx, DP(tkPREV)->data, knh_cwb_newString(ctx, cwb));
		knh_Array_trimSize(ctx, a, prev+1); return;
	}
	if(SP(tkc)->line > SP(tkPREV)->line && TT_(tk) == TT_BRACE) {
		knh_Token_setBOL(tkc, 1); return;
	}
	if(TT_(tkc) == TT_EXPT && TT_(tkPREV) == TT_UNAME) {
		knh_Token_setExceptionType(tkPREV, 1);
		knh_Array_trimSize(ctx, a, prev+1); return;
	}
	if(isTYPE(tkPREV)) {
		if(TT_(tkc) == TT_ITR) {  // String..
			KNH_SETv(ctx, a->list[prev], new_TokenPTYPE(ctx, CLASS_Iterator, tkPREV));
			knh_Array_trimSize(ctx, a, prev+1); return;
		}
		else if(TT_(tkc) == TT_BRANCET && IS_NULL(DP(tkc)->data)) { // String[]
			KNH_SETv(ctx, a->list[prev], new_TokenPTYPE(ctx, CLASS_Array, tkPREV));
			knh_Array_trimSize(ctx, a, prev+1); return;
		}
		else if(TT_(tkc) == TT_GT) { // String>
			tkitr_t itrbuf, *itr = ITR_new(tk, &itrbuf);
			if(ITR_findPTYPE(itr)) {
				knh_Token_t *tkT = new_Token(ctx, 0, TT_TYPE);
				knh_Term_setLine(tkT, tkc);
				prev = itr->c;
				while(ITR_hasNext(itr)) {
					knh_Token_t *tkPT = ITR_nextTK(itr);
					knh_Token_add(ctx, tkT, tkPT);
					if(ITR_is(itr, TT_FUNCARROW)) {
						knh_Token_add(ctx, tkT, ITR_tk(itr));
					}
					ITR_next(itr);
				}
				KNH_SETv(ctx, a->list[prev], tkT);
				knh_Array_trimSize(ctx, a, prev+1); return;
			}
		}
	}
	if(TT_(tkc) == TT_PARENTHESIS) {
		if(TT_(tkPREV) == TT_TYPEOF) {
			knh_Token_t *tkT = new_Token(ctx, 0, TT_TYPE);
			knh_Term_setLine(tkT, tkc);
			knh_Token_add(ctx, tkT, tkPREV);
			TT_(tkc) = TT_BRACE; /* stop recursion */
			knh_Token_add(ctx, tkT, tkc);
			TT_(tkc) = TT_PARENTHESIS;
			KNH_SETv(ctx, a->list[prev], tkT);
			knh_Array_trimSize(ctx, a, prev+1);
			knh_Token_toStmtTYPEOF(ctx, tkT);
			return;
		}
		else if(TT_(tkPREV) == TT_DOT) {
			KNH_SETv(ctx, DP(tkPREV)->text, TS_EMPTY);
			knh_Token_setDOT(tkPREV, 1);
			TT_(tkPREV) = TT_FUNCNAME;
		}
		else if(TT_(tkPREV) == TT_NAME || TT_(tkPREV) == TT_UNAME) {
			TT_(tkPREV) = TT_FUNCNAME;
		}
		else if(TT_(tkPREV) == TT_STR || TT_(tkPREV) == TT_TSTR) {
			//DP(tkPREV)->mn = knh_bytes_parsemn(ctx, S_tobytes(DP(tkPREV)->text));
			TT_(tkPREV) = TT_FMT;
		}
	}
	if(TT_(tkPREV) == TT_BYTE) {
		TT_(tkPREV) = TT_UNAME;
		if(TT_(tkc) == TT_BRANCET) {
			knh_Token_setBYTE(tkPREV, 1);
			KNH_SETv(ctx, DP(tkPREV)->text, ClassTBL(CLASS_Bytes).sname);
			if(IS_NULL(DP(tkc)->data)) { // byte[] => Bytes
				knh_Array_trimSize(ctx, a, prev+1);
			}
		}
		else{
			KNH_SETv(ctx, DP(tkPREV)->text, ClassTBL(CLASS_Int).sname);
		}
	}
}

static knh_Token_t *knh_Token_lastChildNULL(knh_Token_t *tk)
{
	if(IS_Array(DP(tk)->data)) {
		size_t n = knh_Array_size(DP(tk)->list);
		DBG_ASSERT(n>0);
		return knh_TOKENs_n(DP(tk)->list, n-1);
	}
	else if(IS_Token(DP(tk)->data)) {
		return DP(tk)->token;
	}
	return NULL;
}

static int knh_Token_startsWithExpr(Ctx *ctx, knh_Token_t *tk)
{
	tk = knh_Token_lastChildNULL(tk);
	if(tk != NULL) {
		knh_term_t tt = TT_(tk);
		if(tt == TT_COMMA || tt == TT_SEMICOLON || tt == TT_COLON) return 1;
		if(tt == TT_CASE || tt == TT_RETURN || tt == TT_YEILD || tt == TT_PRINT) return 1;
		if(TT_LET <= tt && tt <= TT_TSUB) return 1;
		return 0;
	}
	return 1;
}

static knh_String_t *new_StringSYMBOL(Ctx *ctx, knh_bytes_t t)
{
	knh_DictMap_t *symbolDictMap = DP(ctx->gma)->symbolDictMap;
	knh_index_t idx = knh_DictMap_index(symbolDictMap, t);
	if(idx == -1) {
		knh_String_t *s = new_S(ctx, t);
		knh_DictMap_set(ctx, symbolDictMap, s, s);
		return s;
	}
	else {
		return knh_DictMap_keyAt(symbolDictMap, idx);
	}
}

static knh_String_t* knh_NameSpace_getAliasNULL(Ctx *ctx, knh_NameSpace_t* ns, knh_bytes_t t)
{
	knh_String_t *s = NULL;
	L_TAIL:;
	if(DP(ns)->aliasDictMapNULL != NULL) {
		s = (knh_String_t*)knh_DictMap_getNULL(ctx, DP(ns)->aliasDictMapNULL, t);
	}
	if(s == NULL) {
		if(DP(ns)->parentNULL != NULL) {
			ns = DP(ns)->parentNULL;
			goto L_TAIL;
		}
		if(ns != ctx->share->mainns) {
			ns = ctx->share->mainns;
			goto L_TAIL;
		}
	}
	return s;
}

static void knh_Token_setNAME(Ctx *ctx, knh_Token_t *tk, knh_cwb_t *cwb)
{
	knh_bytes_t t = knh_cwb_tobytes(cwb);
	if(t.ustr[0] == '.') {
		knh_Token_setDOT(tk, 1);
	}
	if(t.ustr[0] == '@') {  /* alias */
		t.ustr = t.ustr + 1; t.len = t.len - 1;
		TT_(tk) = TT_METAN;
		KNH_SETv(ctx, DP(tk)->data, new_StringSYMBOL(ctx, t));
	}
	else if(isupper(t.ustr[0]) || (t.ustr[0] == '.' && isupper(t.ustr[1]))) {
		TT_(tk) = TT_UNAME;
		if(t.ustr[0] == '.') {
			t.ustr = t.ustr + 1; t.len = t.len - 1;
		}
		KNH_SETv(ctx, DP(tk)->data, new_StringSYMBOL(ctx, t));
	}
	else {
		size_t i, u = 0;
		knh_cwb_t cwbbuf2, *cwb2 = knh_cwb_open(ctx, &cwbbuf2);
		TT_(tk) = TT_NAME;
		for(i = cwb->pos; i < cwb->pos + t.len; i++) {
			const char *p = BA_tochar(cwb->ba);
			int ch = p[i];
			if(ch == '.') continue;
			if(ch == '_') {
				if(!(knh_cwb_size(cwb2) < 2)) { /* _name, __name */
					u = 1;
					continue;
				}
			}
			if(u == 1) {
				ch = toupper(ch);
				u = 0;
			}
			knh_Bytes_putc(ctx, cwb2->ba, ch);
		}
		KNH_SETv(ctx, DP(tk)->data, new_StringSYMBOL(ctx, knh_cwb_tobytes(cwb2)));
	}
}

/* ------------------------------------------------------------------------ */
/* [tokenizer] */

#define K_POOLSIZE 64
#define K_POOLHALFSIZE (K_POOLSIZE/2)

static size_t phit = 0;
static size_t pmiss = 0;
static size_t presize = 0;

Object* knh_getConstPools(Ctx *ctx, void *p)
{
	knh_Object_t *v = UPCAST(p), *result = NULL;
	knh_Array_t *a = DP(ctx->gma)->constPools;
	long i;
	if(a == NULL) {
		KNH_INITv(DP(ctx->gma)->constPools, new_Array0(ctx, K_POOLSIZE));
		a = DP(ctx->gma)->constPools;
	}
	for(i = knh_Array_size(a) - 1; i >= 0; i--) {
		knh_Object_t *o = a->list[i];
		if(knh_Object_cid(o) == knh_Object_cid(v) && knh_Object_compareTo(ctx, o, v) == 0) {
			knh_Array_add(ctx, a, o); result = o;
			KNH_SETv(ctx, a->list[i], v); // TO AVOID RCGC
			phit++;
			goto L_POOLMGM;
		}
	}
	knh_Array_add(ctx, a, v); result = v;
	pmiss++;
	L_POOLMGM:;
	if(a->size  == K_POOLSIZE) {
		for(i = 0; i < K_POOLHALFSIZE; i++) {
			KNH_FINALv(ctx, a->list[i]);
		}
		knh_memcpy(&(a->list[0]), &(a->list[K_POOLHALFSIZE]), sizeof(Object*) * K_POOLHALFSIZE);
		knh_bzero(&(a->list[K_POOLHALFSIZE]), sizeof(Object*) * K_POOLHALFSIZE);
		a->size -= K_POOLHALFSIZE;
		presize++;
	}
	//DBG_P("ConstPool hit=%ld, miss=%ld resize=%ld", phit, pmiss, presize);
	return result;
}

static void knh_Token_setTEXT(Ctx *ctx, knh_Token_t *tk, knh_cwb_t *cwb)
{
	knh_bytes_t t = knh_cwb_tobytes(cwb);
	if(TT_(tk) == TT_CODE) {
		knh_String_t *text = knh_NameSpace_getAliasNULL(ctx, knh_getGammaNameSpace(ctx), t);
		if(text != NULL) {
			t = S_tobytes(text);
			KNH_SETv(ctx, DP(tk)->data, text);
			knh_cwb_clear(cwb, 0);
			knh_Bytes_write(ctx, cwb->ba, t);  // alias
		}
		knh_DictSet_t *tokenDictSet = DP(ctx->sys)->tokenDictSet;
		knh_index_t idx = knh_DictSet_index(tokenDictSet, t);
		if(idx != -1) {
			knh_term_t tt = (knh_term_t)knh_DictSet_valueAt(tokenDictSet, idx);
			TT_(tk) = TT_UNMASK(tt);
			if(IS_NULL(DP(tk)->data)) {
				KNH_SETv(ctx, DP(tk)->data, knh_DictSet_keyAt(tokenDictSet, idx));
			}
		}
		else {
			knh_Token_setNAME(ctx, tk, cwb);
		}
	}
	else if(TT_(tk) == TT_METAN) {
		KNH_SETv(ctx, DP(tk)->data, new_StringSYMBOL(ctx, t));
	}
	else {
		knh_Array_t *a = DP(ctx->gma)->constPools;
		knh_String_t *s = NULL;
		if(a != NULL) {
			long i;
			for(i = knh_Array_size(a) - 1; i >= 0; i--) {
				knh_Object_t *o = a->list[i];
				if(knh_Object_cid(o) == CLASS_String) {
					if(S_equals((knh_String_t*)o, t)) {
						knh_cwb_clear(cwb, 0);
						if(isdigit(t.ustr[0]) && TT_(tk) == TT_NUM) {
							KNH_SETv(ctx, DP(tk)->data, o);
						}
						s = (knh_String_t*)o;
						break;
					}
				}
			}
		}
		if(s == NULL) {
			s = knh_cwb_newString(ctx, cwb);
		}
		KNH_SETv(ctx, DP(tk)->data, knh_getConstPools(ctx, s));
	}
}

static void knh_Token_addBuf(Ctx *ctx, knh_Token_t *tk, knh_cwb_t *cwb, knh_term_t tt, int ch)
{
	if(knh_cwb_size(cwb) != 0) {
		knh_flag_t flag = 0;
		knh_Token_t *newtk = new_Token(ctx, flag, tt);
		knh_Token_setTEXT(ctx, newtk, cwb);
		knh_Token_add(ctx, tk, newtk);
		knh_cwb_clear(cwb, 0);
	}
	else if(tt == TT_BLOCK || TT_isSTR(tt) || tt == TT_REGEX) {
		knh_Token_t *newtk = new_Token(ctx, 0, tt);
		KNH_SETv(ctx, DP(newtk)->data, TS_EMPTY);
		knh_Token_add(ctx, tk, newtk);
	}
}

static void knh_InputStream_skipLINECOMMENT(Ctx *ctx, knh_InputStream_t *in)
{
	int ch;
	while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
		if(ch == '\n') break;
	}
}

static void knh_InputStream_skipBLOCKCOMMENT(Ctx *ctx, knh_InputStream_t *in, knh_Bytes_t *ba)
{
	int ch, prev = '/', level = 1;
	while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
		if(prev == '*' && ch == '/') {
			level--;
			if(level == 0) break;
		}else if(prev == '/' && ch == '*') {
			level++;
		}
//		else if(ch == '\n') {
			if(ba != NULL) knh_Bytes_putc(ctx, ba, ch);
//		}
		prev = ch;
	}
}

static knh_bool_t knh_Bytes_isTupleQuote(knh_Bytes_t *ba, int quote)
{
	if(BA_size(ba) > 2 &&
		ba->bu.ustr[BA_size(ba)-1] == quote
		&& ba->bu.ustr[BA_size(ba)-2] == quote
		/*&& ba->bu.ustr[BA_size(ba)-3] == quote*/) return 1;
	return 0;
}

static void knh_Bytes_addEscapeSequence(Ctx *ctx, knh_Bytes_t *ba, knh_InputStream_t *in)
{
	int ch = knh_InputStream_getc(ctx, in);
	if(ch == 'n') {
		ch = '\n';
	}else if(ch == 't') {
		ch = '\t';
	}
	knh_Bytes_putc(ctx, ba, ch);
}

static void knh_Bytes_addQUOTE(Ctx *ctx, knh_Bytes_t *ba, knh_InputStream_t *in, int quote, int isRAW, int isTQUOTE)
{
	if(isRAW == 1) {
		int ch, prev = quote;
		while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
			if(ch == quote && prev != '\\') {
				return;
			}
			knh_Bytes_putc(ctx, ba, ch);
			prev = ch;
		}
	}
	else {
		int ch;
		while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
			if(ch == quote) {
				if(isTQUOTE == 1) {
					if(knh_Bytes_isTupleQuote(ba, quote)) {
						knh_Bytes_unputc(ba);
						knh_Bytes_unputc(ba);
						return;
					}
				}
				else {
					return;
				}
			}
			else if(ch == '\\') {
				knh_Bytes_addEscapeSequence(ctx, ba, in);
				continue;
			}
			knh_Bytes_putc(ctx, ba, ch);
		}
	}
}

static int knh_Token_addQUOTE(Ctx *ctx, knh_Token_t *tk, knh_cwb_t *cwb, knh_InputStream_t *in, int quote, int isRAW)
{
	int ch;
	if(quote != '/') {
		knh_Token_addBuf(ctx, tk, cwb, TT_CODE, quote);
	}
	ch = knh_InputStream_getc(ctx, in);
	if(quote == '#') {
		if(ch == '#') {
			knh_Bytes_addQUOTE(ctx, cwb->ba, in, '\n', 1/*isRAW*/, 0/*isTQUOTE*/);
			if(knh_cwb_tobytes(cwb).ustr[0] == ' ') {
				cwb->pos += 1;
				knh_Token_addBuf(ctx, tk, cwb, TT_ESTR, '\n');
				cwb->pos -= 1;
				knh_cwb_clear(cwb, 0);
			}
			else {
				knh_Token_addBuf(ctx, tk, cwb, TT_ESTR, '\n');
			}
			return '\n';
		}
		else if(ch == '!') {
			knh_InputStream_skipLINECOMMENT(ctx, in);
			return '\n';
		}
	}
	if(ch != quote) {
		if(ch == '\\' && !isRAW) {
			knh_Bytes_addEscapeSequence(ctx, cwb->ba, in);
		}
		else {
			knh_Bytes_putc(ctx, cwb->ba, ch);
		}
		knh_Bytes_addQUOTE(ctx, cwb->ba, in, quote, isRAW, 0/*isTQUOTE*/);
		ch = knh_InputStream_getc(ctx, in);
		knh_Token_addBuf(ctx, tk, cwb, TT_ch(quote), ch);
		return ch;
	}
	ch = knh_InputStream_getc(ctx, in);
	if(quote != ch) { /* "" */
		knh_Token_addBuf(ctx, tk, cwb, TT_ch(quote), ch);
	}
	else { /* triple quote */
		knh_term_t tt = TT_ch(quote);
		if(tt == TT_TSTR) tt = TT_STR;
		knh_Bytes_addQUOTE(ctx, cwb->ba, in, quote, isRAW, 1/*isTQUOTE*/);
		ch = knh_InputStream_getc(ctx, in);
		if(knh_cwb_tobytes(cwb).ustr[0] == '\n') {
			cwb->pos += 1;
			knh_Token_addBuf(ctx, tk, cwb, tt, ch);
			cwb->pos -= 1;
			knh_cwb_clear(cwb, 0);
		}
		else {
			knh_Token_addBuf(ctx, tk, cwb, tt, ch);
		}
	}
	return ch;
}

#define ISB1(t, c)  (t.len == 1 && t.ustr[0] == c)
#define ISB1_(t, c)  (t.ustr[0] == c)
#define ISB2(t, c, c2)  (t.len == 2 && t.ustr[0] == c && t.ustr[1] == c2)
#define ISB2_(t, c, c2)  (t.ustr[0] == c && t.ustr[1] == c2)

static int knh_bytes_isOPR(knh_bytes_t t, int ch)
{
	if(t.len==3) return 0;
	switch(ch) {
	case '%':
		if(ISB1(t, '%')) return 1; /* %% */
		return 0;
	case '&':
		if(ISB1(t, '&')) return 1; /* && */
		return 0;
	case '+':
		if(ISB1(t, '+')) return 1; /* ++ */
		return 0;
	case '-':
		if(ISB1(t, '-')) return 1; /* -- */
		if(ISB2(t, '-', '-')) return 1;
		return 0;
	case '.':
		if(ISB1(t, '.')) return 1;  /* .. */
		if(ISB2(t, '.', '.')) return 1; /* ... */
		return 0;
	case '*':
		if(ISB1(t, '*')) return 1; /* ** */
		return 0;
	case '<':
		if(ISB1(t, '<')) return 1;  /* << */
		if(ISB2(t, '<', '<')) return 1;  /* <<< */
		return 0;
	case '=':
		if(t.len == 1) {
			if(ISB1_(t, '+')) return 1;   /* += */
			if(ISB1_(t, '-')) return 1;   /* -= */
			if(ISB1_(t, '/')) return 1;   /* /= */
			if(ISB1_(t, '%')) return 1;   /* %= */
			if(ISB1_(t, '*')) return 1;   /* *= */
			if(ISB1_(t, '&')) return 1;   /* &= */
			if(ISB1_(t, '|')) return 1;   /* |= */
			if(ISB1_(t, '=')) return 1;   /* == */
			if(ISB1_(t, '!')) return 1;   /* != */
			if(ISB1_(t, '<')) return 1;   /* >= */
			if(ISB1_(t, '>')) return 1;   /* >= */
		}
		if(t.len == 2) {
			if(ISB2_(t, '<', '<')) return 1;  /* <<= */
			if(ISB2_(t, '>', '>')) return 1;  /* >>= */
			if(ISB2_(t, '?', '?')) return 1;   /* ??= */
			if(ISB2_(t, '=', '=')) return 1;   /* === */
		}
		return 0;
	case '>':
		if(t.len == 1) {
			if(ISB1_(t, '-')) return 1; /* -> */
			if(ISB1_(t, '=')) return 1;  /* => */
			if(ISB1_(t, '>')) return 1;  /* >> */
		}
		if(ISB2(t, '>', '>')) return 1;  /* >>> */
		return 0;
	case '?':
		if(ISB1(t, '?')) return 1;  /* ?? */
		return 0;
	case '!':
		if(ISB1(t, '!')) return 1;  /* ?? */
		return 0;
	case '|':
		if(ISB1(t, '|')) return 1;  /* || */
		return 0;
	case '~':
		if(ISB1(t, '=')) return 1;  /* ~= */
		return 0;
		/* built-in */
	}
	return 0;
}

static int knh_Token_addOPR(Ctx *ctx, knh_Token_t *tk, knh_cwb_t *cwb, knh_InputStream_t *in, int ch)
{
	if(ch == '/') {
		ch = knh_InputStream_getc(ctx, in);
		if(ch == '*') {
			knh_InputStream_skipBLOCKCOMMENT(ctx, in, NULL);
			return knh_InputStream_getc(ctx, in);
		}else if(ch == '/') {
			knh_InputStream_skipLINECOMMENT(ctx, in);
			return knh_InputStream_getc(ctx, in);
		}
		if(knh_Token_startsWithExpr(ctx, tk)) {
			knh_Bytes_putc(ctx, cwb->ba, ch);
			return knh_Token_addQUOTE(ctx, tk, cwb, in, '/', 1);
		}
		knh_Bytes_putc(ctx, cwb->ba, '/');
		goto L_INLOOP;
	}
	knh_Bytes_putc(ctx, cwb->ba, ch);
	while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
		L_INLOOP:;
		knh_bytes_t top = knh_cwb_tobytes(cwb);
		if(knh_bytes_isOPR(top, ch)) {
			knh_Bytes_putc(ctx, cwb->ba, ch);
		}
		else {
			int isHEAD = knh_Token_startsWithExpr(ctx, tk);
			knh_Token_addBuf(ctx, tk, cwb, TT_CODE, ch);
			if(isHEAD) {
				knh_Token_t *ctk = knh_Token_lastChildNULL(tk);
				DBG_ASSERT(ctk != NULL);
				if(TT_(ctk) == TT_SUB) TT_(ctk) = TT_TSUB;
				if(TT_(ctk) == TT_ADD) TT_(ctk) = TT_TADD;
				if(TT_(ctk) == TT_MUL) TT_(ctk) = TT_TMUL;
				if(TT_(ctk) == TT_LAND) TT_(ctk) = TT_ADDR;
			}
			break;
		}
	}
	return ch;
}

static int knh_Token_addMETAN(Ctx *ctx, knh_Token_t *tk, knh_cwb_t *cwb, knh_InputStream_t *in)
{
	int ch;
	knh_Token_addBuf(ctx, tk, cwb, TT_CODE, '@');
	ch = knh_InputStream_getc(ctx, in);
	if(ch == '"' || ch == '\'' || ch == '`') {
		ch = knh_Token_addQUOTE(ctx, tk, cwb, in, ch, /*RAW*/1);
	}
	do {
		if(ch == ';') {
			knh_InputStream_perror(ctx, in, KERR_DWARN, _("no effect ;"));
			continue;
		}
		if(!isalnum(ch) && ch != '_' && ch != '.') {
			break;
		}
		knh_Bytes_putc(ctx, cwb->ba, ch);
	}
	while((ch = knh_InputStream_getc(ctx, in)) != EOF);
	knh_Token_addBuf(ctx, tk, cwb, TT_METAN, ch);
	return ch;
}

static int knh_Token_addPROPN(Ctx *ctx, knh_Token_t *tk, knh_cwb_t *cwb, knh_InputStream_t *in)
{
	int ch;
	knh_Token_addBuf(ctx, tk, cwb, TT_CODE, '$');
	while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
		if(!isalnum(ch) && ch != '_' && ch != '.' && ch != '*') {
			break;
		}
		knh_Bytes_putc(ctx, cwb->ba, ch);
	}
	knh_Token_addBuf(ctx, tk, cwb, TT_PROPN, ch);
	return ch;
}

static int knh_Token_addURN(Ctx *ctx, knh_Token_t *tk, knh_cwb_t *cwb, knh_InputStream_t *in)
{
	int ch = knh_InputStream_getc(ctx, in);
	if(ch == ':') {
		knh_Bytes_putc(ctx, cwb->ba, ':');
		ch = knh_InputStream_getc(ctx, in);
		knh_Token_addBuf(ctx, tk, cwb, TT_QPATH, ch);
		return ch;
	}
	if(isspace(ch) || ch == ';' || ch == '"' || ch == '\'') {
		knh_Token_addBuf(ctx, tk, cwb, TT_CODE, ':');
		knh_Bytes_putc(ctx, cwb->ba, ':');
		knh_Token_addBuf(ctx, tk, cwb, TT_CODE, ch);
		return ch;
	}
	knh_Bytes_putc(ctx, cwb->ba, ':');
	knh_Bytes_putc(ctx, cwb->ba, ch);
	while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
		switch(ch) {
			case '\n': case '\r': case ' ':  case '\t': case ';': case ',':
			case '\'': case '"': case '`':
			case ')': case '}': case ']':
			case '(': case '{': case '[':
				goto L_NEWURN;
		}
		knh_Bytes_putc(ctx, cwb->ba, ch);
	}
	L_NEWURN: {
		knh_bytes_t t = knh_cwb_tobytes(cwb);
		knh_term_t tt = TT_URN;
#ifdef K_USING_SEMANTICS
		if(knh_bytes_startsWith(t, STEXT("int:")) || knh_bytes_startsWith(t, STEXT("float:"))) {
			t.ubuf[0] = toupper(t.ustr[0]);
		}
#endif
		if(knh_bytes_startsWith(t, STEXT("new:"))) {
			tt = TT_NEW;
		}
		knh_Token_addBuf(ctx, tk, cwb, (isupper(t.ustr[0])) ? TT_UNAME : TT_URN, ch);
	}
	return ch;
}

static int knh_Token_addNUM(Ctx *ctx, knh_Token_t *tk, knh_cwb_t *cwb, knh_InputStream_t *in, int ch)
{
	int prev = 0, dot = 0;
	L_ADD:;
	knh_Bytes_putc(ctx, cwb->ba, ch);
	prev = ch;
	while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
		if(isalnum(ch)) goto L_ADD;
		if(ch == '_') continue; // nothing
		if(prev == '.' && ch == '.') {  /* 1.. => 1 .. */
			knh_Bytes_unputc(cwb->ba);
			knh_Token_addBuf(ctx, tk, cwb, TT_NUM, ch);
			knh_Bytes_putc(ctx, cwb->ba, '.');
			knh_Bytes_putc(ctx, cwb->ba, '.');
			ch = knh_InputStream_getc(ctx, in);
			knh_Token_addBuf(ctx, tk, cwb, TT_CODE, ch);
			return ch;
		}
		if(ch == '.') {
			dot++;
			if(dot == 1) goto L_ADD;
			else if(dot == 2) {
				knh_InputStream_perror(ctx, in, KERR_DWARN, _("too many . in number"));
			}
			continue;
		}
		if((ch == '-' || ch == '+') && (prev == 'e' || prev =='E')) {
			goto L_ADD;
		}
		break;
	}
	knh_Token_addBuf(ctx, tk, cwb, TT_NUM, ch);
	return ch;
}

#define IBUFSIZ 80

//static void knh_dump_ibuf(char *ibuf)
//{
//	fprintf(stderr, "ibuf: '");
//	while(*ibuf != 0) {
//		if(*ibuf == ' ') fputc('s', stderr); else fputc('t', stderr);
//		ibuf++;
//	}
//	fprintf(stderr, "'\n");
//}

static void knh_Token_addBLOCK(Ctx *ctx, knh_Token_t *tk, knh_cwb_t *cwb, knh_InputStream_t *in, char *begin_ibuf)
{
	int ch, prev = '{', indent_pos = -1, level = 1;
	char indent_buf[IBUFSIZ];
	knh_Token_addBuf(ctx, tk, cwb, TT_CODE, '{');
//	knh_dump_ibuf(begin_ibuf);
	goto L_START;

	L_NEWLINE:
	indent_pos = 0;
	knh_bzero(indent_buf, IBUFSIZ);
	while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
		if(ch == '\t' || ch == ' ' || ch == '\v') {
			indent_buf[indent_pos] = ch;
			if(indent_pos < IBUFSIZ - 1) indent_pos++;
			continue;
		}
		break;
	}
//	knh_dump_ibuf(indent_buf);
	/*if(ch != '}')*/ { /* indent normization */
		int i, isize = level - ((ch != '}') ? 1 : 2);
		for(i = 0; i < isize; i++) {
			knh_Bytes_putc(ctx, cwb->ba, '\t');
		}
	}
	goto L_STARTLINE;
	L_START:;
	while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
		L_STARTLINE:;
		if(ch == '}' && prev != '\\') {
			level--;
			if(level == 0) break;
//			if(indent_pos != -1) {
//				int i;
//				for(i = 0; i < indent_pos; i++) {
//					//if(begin_ibuf[i] == 0) goto L_OK;
//					DBG_P("i=%d, '%c', '%c'", i, _S(begin_ibuf[i]), _S(indent_buf[i]));
//					if(begin_ibuf[i] != indent_buf[i]) {
//						goto L_OK;
//					}
//				}
//				knh_InputStream_perror(ctx, in, KERR_DWARN, _("mismatch indent"));
//				break;
//			}
		}
//		L_OK:;
		knh_Bytes_putc(ctx, cwb->ba, ch);
		if(ch == '\n') {
			goto L_NEWLINE;
		}
		else if(ch == '{' && prev != '\\') {
			level++;
		}
		else if(ch == '\'' || ch == '"' || ch == '`') {
			knh_Bytes_addQUOTE(ctx, cwb->ba, in, ch, 1/*isRAW*/, 0);
			knh_Bytes_putc(ctx, cwb->ba, ch);
		}
		else if(prev == '/' && ch == '/') {
			knh_InputStream_skipLINECOMMENT(ctx, in);
			knh_Bytes_putc(ctx, cwb->ba, '\n');
		}
		else if(prev == '/' && ch == '*') {
			knh_InputStream_skipBLOCKCOMMENT(ctx, in, cwb->ba);
			knh_Bytes_putc(ctx, cwb->ba, '*');
			knh_Bytes_putc(ctx, cwb->ba, '/');
		}
		prev = ch;
	}
	knh_Token_addBuf(ctx, tk, cwb, TT_BLOCK, ch);
}

#define knh_Token_setLine(tk, in) \
	SP(tk)->uri  = DP(in)->uri; \
	SP(tk)->line = DP(in)->line; \

static void knh_InputStream_parseToken(Ctx *ctx, knh_InputStream_t *in, knh_Token_t *tk)
{
	int ch, indent_pos;
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	char indent_buf[IBUFSIZ];
	DBG_ASSERT(DP(in)->line != 0);

	L_NEWLINE:;
	knh_Token_setLine(tk, in);
	indent_pos = 0;
	knh_bzero(indent_buf, IBUFSIZ);
	while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
		if(ch == '\t' || ch == ' ' || ch == '\v') {
			indent_buf[indent_pos] = ch;
			if(indent_pos < IBUFSIZ - 1) indent_pos++;
			continue;
		}
		goto L_AGAIN;
	}
	L_NEWTOKEN:;
	knh_cwb_clear(cwb, 0);
	knh_Token_setLine(tk, in);
	while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
		L_AGAIN:;
		switch(ch) {
		/* grammatic structure */
		case '\n':
			knh_Token_addBuf(ctx, tk, cwb, TT_CODE, ch);
			goto L_NEWLINE;

		case ' ': case '\t': case '\v': case '\r':
			knh_Token_addBuf(ctx, tk, cwb, TT_CODE, ch);
			goto L_NEWTOKEN;

		case ';': case ',':
			knh_Token_addBuf(ctx, tk, cwb, TT_CODE, ch);
			knh_Bytes_putc(ctx, cwb->ba, ch);
			knh_Token_addBuf(ctx, tk, cwb, TT_CODE, ch);
			goto L_NEWTOKEN;

		case '{':
			knh_Token_addBLOCK(ctx, tk, cwb, in, indent_buf);
			goto L_NEWTOKEN;

		case '[': case '(':
			knh_Token_addBuf(ctx, tk, cwb, TT_CODE, ch);
			{
				knh_Token_t *ctk = new_Token(ctx, 0, TT_ch(ch));
				ctk->h.meta = tk;
				tk = ctk;
			}
			goto L_NEWTOKEN;

		case ')': case ']':
			knh_Token_addBuf(ctx, tk, cwb, TT_CODE, ch);
			if(TT_ch(ch) == TT_(tk)) {
				knh_Token_t *ctk = tk;
				tk = (knh_Token_t*)(ctk)->h.meta;
				(ctk)->h.meta = NULL;
				knh_Token_add(ctx, tk, ctk);
				knh_Term_setLine(ctk, tk);
				knh_Token_setLine(tk, in);
				goto L_NEWTOKEN;
			}
		case '}':
			knh_Token_addBuf(ctx, tk, cwb, TT_CODE, ch);
			knh_InputStream_perror(ctx, in, KERR_ERR, "mismatched %c", ch);
			goto L_NEWTOKEN;

		/* token */
		case '"': case '\'': case '`' :
		case '#':
			ch = knh_Token_addQUOTE(ctx, tk, cwb, in, ch, 0/*isRAW*/);
			goto L_AGAIN;

		case '$':
			ch = knh_Token_addPROPN(ctx, tk, cwb, in);
			goto L_AGAIN;
		case '@':
			ch = knh_Token_addMETAN(ctx, tk, cwb, in);
			goto L_AGAIN;
		case '|':
#ifdef TT_SIZE
			knh_Token_addBuf(ctx, tk, cwb, TT_CODE, ch);
			if(TT_ch(ch) == TT_(tk)) {
				knh_Token_t *ctk = tk;
				tk = (knh_Token_t*)(ctk)->h.meta;
				(ctk)->h.meta = NULL;
				knh_Token_add(ctx, tk, ctk);
				knh_Term_setLine(ctk, tk);
				knh_Token_setLine(tk, in);
				TT_(ctk) = TT_PARENTHESIS;
				knh_Bytes_write(ctx, cwb->ba, STEXT(".size"));
				knh_Token_addBuf(ctx, tk, cwb, TT_CODE, ' ');
				goto L_NEWTOKEN;
			}
			if(knh_Token_startsWithExpr(ctx, tk)) {
				knh_Token_t *ctk = new_Token(ctx, 0, TT_ch(ch));
				ctk->h.meta = tk;
				tk = ctk;
				goto L_NEWTOKEN;
			}
#endif
		case '?': {
			knh_bytes_t t = knh_cwb_tobytes(cwb);
			if(ISB(t, "in") || ISB(t, "isa") || ISB(t, "is")) {
				knh_Bytes_putc(ctx, cwb->ba, ch);
				knh_Token_addBuf(ctx, tk, cwb, TT_CODE, ' ');
				break;
			}
		}
		case '+': case '-': case '*': case '/': case '%':
		case '=': case '&':
		case '<': case '>': case '^': case '!': case '~':
			knh_Token_addBuf(ctx, tk, cwb, TT_CODE, ch);
			ch = knh_Token_addOPR(ctx, tk, cwb, in, ch);
			goto L_AGAIN;

		case '.':
			knh_Token_addBuf(ctx, tk, cwb, TT_CODE, '.');
			ch = knh_InputStream_getc(ctx, in);
			if(ch == '.') {
				knh_Bytes_putc(ctx, cwb->ba, '.');
				ch = knh_Token_addOPR(ctx, tk, cwb, in, ch);
			}
			else if(isdigit(ch)) {
				knh_Bytes_putc(ctx, cwb->ba, '0');
				knh_Bytes_putc(ctx, cwb->ba, '.');
				ch = knh_Token_addNUM(ctx, tk, cwb, in, ch);
			}
			else {
				knh_Bytes_putc(ctx, cwb->ba, '.');
			}
			goto L_AGAIN;

		case ':':
			ch = knh_Token_addURN(ctx, tk, cwb, in);
			goto L_AGAIN;

		case EOF :
			break;

		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			if(knh_cwb_size(cwb) == 0) {
				ch = knh_Token_addNUM(ctx, tk, cwb, in, ch);
				goto L_AGAIN;
			}

		default:
			if(ch > 127) {
				knh_InputStream_perror(ctx, in, KERR_DWARN, _("unexpected multi-byte character"));
				ch = ' ';
			}
			knh_Bytes_putc(ctx, cwb->ba, ch);
		} /* switch */

	}/*while*/
	knh_Token_addBuf(ctx, tk, cwb, TT_CODE, EOF);
}

/* ------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------ */

static void knh_Token_toBLOCK(Ctx *ctx, knh_Token_t *tk)
{
	BEGIN_LOCAL(ctx, lsfp, 1);
	knh_InputStream_t *in = new_StringInputStream(ctx, DP(tk)->text, 0, S_size(DP(tk)->text));
	KNH_SETv(ctx, lsfp[0].o, in);
	DP(in)->uri = SP(tk)->uri; DP(in)->line = SP(tk)->line;
	KNH_SETv(ctx, DP(tk)->data, KNH_NULL);
	TT_(tk) = TT_BRACE;
	knh_InputStream_parseToken(ctx, in, tk);
	if(knh_isSystemVerbose() && DP(in)->uri == URI_EVAL) {
		knh_dump_token(ctx, KNH_STDOUT, tk);
	}
	END_LOCAL(ctx, lsfp);
}

/* ------------------------------------------------------------------------ */

static void knh_Stmt_expand(Ctx *ctx, knh_Stmt_t *o)
{
	knh_Term_t**newterms = 0;
	size_t i, newcapacity = DP(o)->capacity * 2;
	if(newcapacity == 0) {
		newcapacity = (K_FASTMALLOC_SIZE / sizeof(knh_Object_t*));
	}
	newterms = (knh_Term_t**)KNH_MALLOC(ctx, sizeof(Object*) * newcapacity);
	knh_memcpy(newterms, DP(o)->terms, sizeof(Object*) * DP(o)->capacity);
	for(i = DP(o)->capacity; i < newcapacity; i++) {
		newterms[i] = NULL;
	}
	if(DP(o)->capacity != 0) {
		KNH_FREE(ctx, DP(o)->terms, sizeof(Object*) * DP(o)->capacity);
	}
	DP(o)->terms = newterms;
	DP(o)->capacity = newcapacity;
}

static void knh_Stmt_setLine(knh_Stmt_t *stmt, knh_Term_t *tm)
{
	size_t i;
	DBG_ASSERT(SP(tm)->line != 0);
	SP(stmt)->line = SP(tm)->line; SP(stmt)->uri = SP(tm)->uri;
	for(i = 0; i < DP(stmt)->size; i++) {
		knh_Term_t *tmN = DP(stmt)->terms[i];
		if(SP(tmN)->line != 0) continue;
		if(IS_Stmt(tmN)) {
			knh_Stmt_setLine((knh_Stmt_t*)tmN, tm);
		}
		else {
			knh_Term_setLine(tmN, tm);
		}
	}
}

knh_Term_t *knh_Stmt_add_(Ctx *ctx, knh_Stmt_t *stmt, ...)
{
	va_list ap;
	knh_Term_t *tm = NULL;
	va_start(ap, stmt);
	while((tm = (knh_Term_t*)va_arg(ap, knh_Term_t*)) != NULL) {
		DBG_ASSERT(IS_Token(tm) || IS_Stmt(tm));
		if(!(DP(stmt)->size < DP(stmt)->capacity)) {
			knh_Stmt_expand(ctx, stmt);
		}
		DBG_ASSERT(DP(stmt)->terms[DP(stmt)->size] == NULL);
		KNH_INITv(DP(stmt)->terms[DP(stmt)->size], tm);
		DP(stmt)->size++;
		if(SP(stmt)->line == 0 && SP(tm)->line != 0) {
			knh_Stmt_setLine(stmt, tm);
		}
		if(SP(tm)->line == 0 && SP(stmt)->line != 0) {
			knh_Term_setLine(tm, stmt);
		}
		if(TT_(tm) == STT_ERR || TT_(tm) == TT_ERR) {
			STT_(stmt) = STT_ERR;
			knh_Stmt_toERR(ctx, stmt, tm);
		}
	}
	va_end(ap);
	return TM(stmt);
}

knh_Token_t *new_TokenMN(Ctx *ctx, knh_methodn_t mn)
{
	knh_Token_t *tk = new_(Token);
	TT_(tk) = TT_MN;
	DP(tk)->mn = mn;
	if(MN_isGETTER(mn)) {knh_Token_setGetter(tk, 1);};
	if(MN_isSETTER(mn)) {knh_Token_setSetter(tk, 1);}
	return tk;
}

/* ------------------------------------------------------------------------ */

#define ADD(stmt, FUNC) \
	if(STT_(stmt) != STT_ERR) {\
		FUNC;\
	}\

static void _EXPR(Ctx *ctx, knh_Stmt_t *stmt, tkitr_t *itr);
static knh_Stmt_t *new_StmtSTMT1(Ctx *ctx, tkitr_t *itr);

static void knh_Token_perror(Ctx *ctx, knh_Token_t *tk, int pe, const char *fmt, ...)
{
	if(TT_(tk) != TT_ERR) {
		va_list ap;
		va_start(ap, fmt);
		knh_vperror(ctx, SP(tk)->uri, SP(tk)->line, pe, fmt, ap);
		va_end(ap);
		if(pe < KERR_EWARN) TT_(tk) = TT_ERR;
	}
}

static int ITR_indexTT(tkitr_t *itr, knh_term_t tt, int def)
{
	int i;
	for(i = itr->c; i < itr->e; i++) {
		if(TT_(itr->ts[i]) == tt) return i;
	}
	return def;
}

static void ITR_replace(tkitr_t *itr, knh_term_t tt, knh_term_t tt1)
{
	int i;
	for(i = itr->c; i < itr->e; i++) {
		if(TT_(itr->ts[i]) == tt) TT_(itr->ts[i]) = tt1;
	}
}

static tkitr_t *ITR_copy(tkitr_t *itr, tkitr_t *buf, int shift)
{
	*buf = *itr;
	buf->c = itr->c + shift;
	return buf;
}

static void _SEMICOLON(Ctx *ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	if(ITR_is(itr, TT_SEMICOLON)) {
		ITR_next(itr);
	}
	else {
		knh_perror(ctx, SP(stmt)->uri, SP(stmt)->line, KERR_BAD, "needs ;");
	}
}

static void ITR_skipSEMICOLON(tkitr_t *itr)
{
	while(ITR_hasNext(itr)) {
		if(TT_(ITR_tk(itr)) != TT_SEMICOLON) break;
		ITR_next(itr);
	}
}

static void ITR_nextStmt(tkitr_t *itr)
{
	int i;
	knh_Token_t **ts = itr->ts;
	for(i = itr->c; i < itr->e; i++) {
		if(knh_Token_isBOL(ts[i])) {
			itr->c = i;
			ITR_skipSEMICOLON(itr);
			return;
		}
	}
	itr->c = itr->e;
}

static tkitr_t *ITR_stmt(Ctx *ctx, tkitr_t *itr, int pos, tkitr_t *buf, int isNeedSemicolon)
{
	int i;
	*buf = *itr;
	for(i = itr->c + 1 + pos; i < itr->e; i++) {
		if(knh_Token_isBOL(itr->ts[i])) {
			if(isNeedSemicolon && TT_(itr->ts[i]) != TT_SEMICOLON) {
				knh_Token_perror(ctx, itr->ts[i-1], KERR_BAD, "needs ;");
			}
			buf->e = i;
			itr->c = i;
			ITR_skipSEMICOLON(itr);
			return buf;
		}
	}
	buf->e = itr->e;
	itr->c = itr->e;
	if(isNeedSemicolon && itr->e -1 >= 0) {
		knh_Token_perror(ctx, itr->ts[itr->e - 1], KERR_BAD, "needs ;");
	}
	return buf;
}

static tkitr_t *ITR_first(tkitr_t *tc, int idx, tkitr_t *buf, int shift)
{
	DBG_ASSERT(idx != -1 && idx <= tc->e);
	int i;
	*buf = *tc;
	buf->meta = -1;
	for(i = tc->c; i < idx; i++) {
		knh_term_t tt = TT_(tc->ts[i]);
		if(tt == TT_METAN) {
			if(buf->meta == -1) buf->meta = i;
			tc->c = i + 1;
		}
		else {
			break;
		}
	}
	buf->e = idx;
	tc->c = idx + shift;
	tc->meta = -1;
	return buf;
}

static void ITR_skipUNTIL(tkitr_t *itr, knh_term_t tt)
{
	int i;
	for(i = itr->c; i < itr->e; i++) {
		if(TT_(itr->ts[i]) == tt) {
			itr->e = i;
			return;
		}
	}
}

static void TT_skipMETA(tkitr_t *itr)
{
	itr->meta = -1;
	while(ITR_hasNext(itr)) {
		knh_term_t tt = ITR_tt(itr);
		if(ITR_isN(itr, +1, TT_COLON) && (tt == TT_NAME || tt == TT_UNAME)) {
			if(itr->meta == -1) itr->meta = itr->c;
			ITR_next(itr);
		}
		else if(tt == TT_METAN || tt == TT_DOC) {
			if(itr->meta == -1) itr->meta = itr->c;
		}
		else {
			return;
		}
		ITR_next(itr);
	}
}

static knh_Token_t* ITR_curTK(tkitr_t *itr)
{
	int c = itr->c;
	while(!(c < itr->e)) {
		if(c == 0) break;
		c--;
	}
	itr->c = c + 1;
	return itr->ts[c];
}

static void ITR_perror(Ctx *ctx, tkitr_t *itr, knh_Stmt_t *stmt, const char *fmt)
{
	knh_Token_t *tk = ITR_curTK(itr);
	knh_Token_perror(ctx, tk, KERR_ERR, fmt, tk);
	knh_Stmt_add(ctx, stmt, tk);
	if(TT_(tk) == TT_ERR) {
		ITR_nextStmt(itr);
		DBG_ASSERT(SP(stmt)->stt == STT_ERR);
	}
}

#define ITR_ignore(ctx, itr, n)

/* ------------------------------------------------------------------------ */

static void _ASIS(Ctx *ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	knh_Stmt_add(ctx, stmt, new_Token(ctx, 0, TT_ASIS));
}

static void _VAR(Ctx *ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	knh_Stmt_add(ctx, stmt, new_Token(ctx, 0, TT_VAR));
}

static void _DONE(Ctx *ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	knh_Stmt_add(ctx, stmt, new_Stmt2(ctx, STT_DONE, NULL));
}

typedef int (*knh_FisToken)(knh_Token_t *tk);

static int ITR_isT(tkitr_t *itr, knh_FisToken f)
{
	return (ITR_hasNext(itr) && f(ITR_tk(itr)));
}

static void knh_Stmt_tadd(Ctx *ctx, knh_Stmt_t *stmt, tkitr_t *itr, knh_FisToken f, const char *emsg)
{
	if(SP(stmt)->stt == STT_ERR) return;
	if(ITR_hasNext(itr) && f(ITR_tk(itr))) {
		knh_Stmt_add(ctx, stmt, ITR_nextTK(itr));
	}
	else if(emsg == NULL) {
		_ASIS(ctx, stmt, itr);
	}
	else {
		ITR_perror(ctx, itr, stmt, emsg);
	}
}

static int isVARN(knh_Token_t* tk)
{
	return (TT_(tk) == TT_NAME && !knh_Token_isDOT(tk));
}

static void _PNAME(Ctx *ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	knh_String_t *s = NULL;
	if(itr->c + 1 == itr->e) {
		knh_Token_t *tk = itr->ts[itr->c];
		if(TT_(tk) == TT_NAME || TT_(tk) == TT_UNAME) {
			s = DP(tk)->text;
		}
	}
	if(s != NULL) {
		knh_Token_t *tkN = new_Token(ctx, 0, TT_CONST);
		KNH_SETv(ctx, DP(tkN)->data, s);
		SP(tkN)->type = TYPE_String;
		knh_Token_setPNAME(tkN, 1);
		knh_Stmt_add(ctx, stmt, tkN);
	}
}

static void _EXPRs(Ctx *ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	if(ITR_is(itr, TT_SEMICOLON)) return;
	do {
		int idx = ITR_indexTT(itr, TT_COMMA, itr->e);
		tkitr_t ebuf, *eitr = ITR_first(itr, idx, &ebuf, +1);
		if(ITR_size(eitr) != 0) {
			if(STT_(stmt) == STT_PRINT) { /* @TEST print a */
				_PNAME(ctx, stmt, eitr);
			}
			_EXPR(ctx, stmt, eitr);
		}
	}
	while(ITR_hasNext(itr));
}

static knh_index_t ITR_indexTOUNTIL(tkitr_t *itr)
{
	knh_index_t i;
	for(i = itr->c; i < itr->e; i++) {
		knh_term_t tt = TT_(itr->ts[i]);
		if(tt == TT_TO || tt == TT_UNTIL) return i;
		if(tt == TT_ITR) { /* a[1..n] as a [1 until n] */
			TT_(itr->ts[i]) = TT_UNTIL;
			return i;
		}

	}
	return -1;
}

static void _ASISEXPRs(Ctx *ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	do {
		int idx = ITR_indexTT(itr, TT_COMMA, itr->e);
		tkitr_t ebuf, *eitr = ITR_first(itr, idx, &ebuf, +1);
		if(ITR_size(eitr) == 0) {
			_ASIS(ctx, stmt, eitr);
		}
		else {
			_EXPR(ctx, stmt, eitr);
		}
	}
	while(ITR_hasNext(itr));
	(itr)->c = (itr)->e - 1;
	if(ITR_is(itr, TT_COMMA)) {
		_ASIS(ctx, stmt, itr);
	}
}

static int ITR_indexKEY(tkitr_t *itr, int shift)
{
	int i;
	knh_Token_t **ts = itr->ts;
	for(i = itr->c + shift; i < itr->e - 1; i++) { /* BEGIN */
		if(TT_(ts[i+1]) == TT_COLON && IS_bString(DP(ts[i])->text)) {
			return i;
		}
	}
	return itr->e;
}

static void _ARRAY(Ctx *ctx, knh_Stmt_t *stmt, knh_methodn_t mn, knh_class_t cid, tkitr_t *itr)
{
	DBG_ASSERT(STT_(stmt) == STT_NEW);
	knh_Stmt_add(ctx, stmt, new_TokenMN(ctx, mn));
	knh_Stmt_add(ctx, stmt, new_TokenCID(ctx, cid));
	_EXPRs(ctx, stmt, itr);
}

static knh_Stmt_t *knh_Stmt_addNewStmt(Ctx *ctx, knh_Stmt_t *stmt, knh_term_t stt)
{
	knh_Stmt_t *newstmt = new_Stmt2(ctx, stt, NULL);
	knh_Stmt_add(ctx, stmt, newstmt);
	return newstmt;
}

static void _KEYVALUEs(Ctx *ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	int p = ITR_indexKEY(itr, +0);
	while(p < itr->e) {
		tkitr_t ebuf, *eitr = ITR_copy(itr, &ebuf, 0);
		eitr->c = p;
		eitr->e = ITR_indexKEY(eitr, +1);
		if(ITR_size(eitr) > 0) {
			knh_Token_t *tk = ITR_nextTK(eitr);
			TT_(tk) = TT_STR;
			knh_Stmt_add(ctx, stmt, tk);
			DBG_ASSERT(ITR_is(eitr, TT_COLON));
			ITR_next(eitr);
			ITR_chop(eitr, TT_COMMA);
			ITR_chop(eitr, TT_SEMICOLON);
			if(!ITR_hasNext(eitr)) {
				knh_Stmt_add(ctx, stmt, new_Token(ctx, 0, TT_NULL));
				return;
			}
			if(ITR_count(eitr, TT_COMMA) == 0) {
				_EXPR(ctx, stmt, eitr);
			}
			else {
				knh_Stmt_t *stmtARRAY = knh_Stmt_addNewStmt(ctx, stmt, STT_NEW);
				_ARRAY(ctx, stmtARRAY, MN_new__LIST, CLASS_Array, eitr);
			}
		}
		p = eitr->e;
	}
}

static void _DICT(Ctx *ctx, knh_Stmt_t *stmt, knh_Token_t *tkC, knh_Token_t *tkB)
{
	DBG_ASSERT(STT_(stmt) == STT_NEW);
	knh_Stmt_add(ctx, stmt, tkC);
	if(TT_(tkB) == TT_BLOCK) {
		knh_Token_toBLOCK(ctx, tkB);
	}
	DBG_ASSERT(TT_(tkB) == TT_BRACE);
	{
		tkitr_t pbuf, *pitr = ITR_new(tkB, &pbuf);
		_KEYVALUEs(ctx, stmt, pitr);
	}
}

static void knh_Token_toStmtTYPEOF(Ctx *ctx, knh_Token_t *tk)
{
	if(TT_(tk) == TT_TYPE) {
		tkitr_t tbuf, *titr = ITR_new(tk, &tbuf);
		if(ITR_is(titr, TT_TYPEOF)) {
			knh_Stmt_t *stmt = new_Stmt2(ctx, STT_CALL1, NULL);
			ITR_next(titr);
			_EXPR(ctx, stmt, titr);
			KNH_SETv(ctx, DP(tk)->data, stmt);
		}
	}
}

/* ------------------------------------------------------------------------ */
/* EXPR */

static int ITR_indexLET(tkitr_t *itr)
{
	knh_Token_t **ts = itr->ts; int i;
	for(i = itr->c; i < itr->e; i++) {
		knh_term_t tt = TT_(ts[i]);
		if(tt == TT_LET) return i;
		if(tt == TT_ALTLET) {
			TT_(ts[i]) = TT_ALT; return i;
		}
		if(TT_LSFTE <= tt && tt < TT_ALTLET) {
			TT_(ts[i]) += (TT_ADD - TT_ADDE);
			DBG_P("REWRITE OP %s => %s", TT__(tt), TT__(TT_(ts[i])));
			return i;
		}
	}
	return -1;
}

static int isLVALUE(knh_Token_t *tk)
{
	knh_term_t tt = TT_(tk);
	if(tt == TT_UNAME || tt == TT_PROPN) {
		return 1;
	}
	if(tt == TT_NAME) {
//		knh_Token_t *tkL = itr->ts[itr->c];
		//return (!knh_Token_checkKeyword(ctx, tkL));
		return 1;
	}
	return 0;
}

static int ITR_indexOPR(Ctx *ctx, tkitr_t *itr)
{
	int i, oppri = 98, idx = -1;
	knh_term_t tt_found = TT_EOT;
	knh_Token_t **ts = itr->ts;
	{
		int isTEXTPARAM = 0;
		for(i = itr->c; i < itr->e; i++) {
			if(TT_(ts[i]) == TT_NAME || TT_(ts[i]) == TT_UNAME) {
				isTEXTPARAM = 1;
				continue;
			}
			if(isTEXTPARAM == 1 && TT_isSTR(TT_(ts[i]))) {
				return -1;
			}
			break;
		}
	}
	for(i = itr->c; i < itr->e; i++) {
		int p = knh_term_getOPRpriority(TT_(ts[i]));
		if(p == 0) {
			knh_Token_perror(ctx, ts[i], KERR_ERR, _("don't use %L in an expression"), ts[i]);
			itr->e = i;
			return i;
		}
		if(p <= oppri) { /* p <= oppri is binary*/
			oppri = p;
			tt_found = TT_(ts[i]);
		}
	}
	if(tt_found != TT_EOT) {
		for(i = itr->c; i < itr->e; i++) {
			if(TT_(ts[i]) == tt_found) {
				idx = i; /* last idx */
				if(!TT_isBINARY(tt_found)) break; /* fisrt idx */
			}
		}
	}
	return idx;
}

static int ITR_isDOTNAME(tkitr_t *itr, int shift)
{
	int c = itr->c + shift;
	if(c < itr->e) {
		knh_Token_t *tk = itr->ts[c];
		if(!knh_Token_isDOT(tk)) return 0;
		if(TT_(tk) == TT_NAME || TT_(tk) == TT_UNAME || TT_(tk) == TT_FUNCNAME) return 1;
	}
	return 0;
}

static int ITR_isCAST(tkitr_t *itr)
{
	if(ITR_is(itr, TT_PARENTHESIS) && !ITR_isDOTNAME(itr, 1)) {
		tkitr_t cbuf, *citr = ITR_new(ITR_tk(itr), &cbuf);
		if(ITR_is(citr, TT_TO)) {
			ITR_next(citr)
		}
		if(ITR_isT(citr, isTYPE) && ITR_size(citr) == 1) {
			return 1;
		}
	}
	return 0;
}

static void ITR_toLET(Ctx *ctx, tkitr_t *itr, knh_Token_t *tk)
{
	int i;
	knh_Token_t *tkC = NULL;
	KNH_SETv(ctx, DP(tk)->data, new_Array0(ctx, 0));
	for(i = itr->c; i < itr->e; i++) {
		tkC = new_Token(ctx, 0, TT_(itr->ts[i]));
		KNH_SETv(ctx, DP(tkC)->data, DP(itr->ts[i])->data);
		DP(tkC)->index = DP(itr->ts[i])->index;
		knh_Array_add(ctx, DP(tk)->list, tkC);
	}
	knh_Array_add(ctx, DP(tk)->list, new_Token(ctx, 0, TT_LET));
	for(i = itr->c; i < itr->e; i++) {
		tkC = new_Token(ctx, 0, TT_(itr->ts[i]));
		KNH_SETv(ctx, DP(tkC)->data, DP(itr->ts[i])->data);
		DP(tkC)->index = DP(itr->ts[i])->index;
		knh_Array_add(ctx, DP(tk)->list, tkC);
	}
	tkC = new_Token(ctx, 0, (TT_(tk)==TT_NEXT) ? TT_ADD : TT_SUB);
	knh_Array_add(ctx, DP(tk)->list, tkC);
	tkC = new_Token(ctx, 0, TT_NUM);
	KNH_SETv(ctx, DP(tkC)->data, TS_ONE);
	knh_Array_add(ctx, DP(tk)->list, tkC);
}

static knh_Stmt_t *knh_Stmt_addFUNC(Ctx *ctx, knh_Stmt_t *stmt, knh_Token_t *tkF)
{
	size_t size = DP(stmt)->size;
	if(TT_(tkF) == TT_NAME || TT_(tkF) == TT_UNAME) {
		knh_Token_setGetter(tkF, 1);
	}
	if(size == 0) {
		DBG_ASSERT(STT_(stmt) == STT_CALL);
		knh_Stmt_add(ctx, stmt, tkF);
	}
	else if(DP(stmt)->size == 1) {
		DBG_ASSERT(STT_(stmt) == STT_CALL);
		knh_Stmt_add(ctx, stmt, tkF);
		knh_Stmt_swap(ctx, stmt, 0, 1);
	}
	else {
		knh_Stmt_t *stmtIN = new_Stmt2(ctx, STT_(stmt), NULL);
		size_t i;
		for(i = 0; i < DP(stmt)->size; i++) {
			knh_Stmt_add(ctx, stmtIN, DP(stmt)->terms[i], NULL);
		}
		knh_Stmt_done(ctx, stmt);
		STT_(stmt) = STT_CALL;
		knh_Stmt_add(ctx, stmt, tkF, stmtIN);
	}
	return stmt;
}

#define knh_Stmt_addMN(ctx, stmt, mn) knh_Stmt_addFUNC(ctx, stmt, new_TokenMN(ctx, mn))

static knh_Stmt_t *new_StmtREUSE(Ctx *ctx, knh_Stmt_t *stmt, knh_term_t stt)
{
	if(STT_(stmt) == STT_CALL1 || (STT_(stmt) == STT_CALL && DP(stmt)->size == 0)) {
		STT_(stmt) = stt;
		return stmt;
	}
	else {
		return knh_Stmt_addNewStmt(ctx, stmt, stt);
	}
}

static void _REGEX(Ctx *ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	knh_Token_t *tk = ITR_nextTK(itr);
	knh_NameSpace_t *ns = knh_getGammaNameSpace(ctx);
	knh_Regex_t *re = new_H(Regex);
	const char *opt = "";
	int cflags = 0;
	KNH_INITv(re->pattern, DP(tk)->text);
	re->spi = DP(ns)->regexSPI;
	if(re->spi == DP(ns)->strregexSPI) {
		knh_Token_perror(ctx, tk, KERR_EWARN, _("regex is not installed"));
	}
	re->reg = re->spi->regmalloc(ctx, DP(tk)->text);
	if(ITR_is(itr, TT_NAME)) {
		opt = S_tochar(DP(ITR_nextTK(itr))->text);
	}
	cflags = re->spi->parse_cflags(ctx, opt);
	if(re->spi->regcomp(ctx, re->reg, S_tochar(re->pattern), cflags) != 0) {
		knh_Token_perror(ctx, tk, KERR_ERR, _("%s compile error: /%s/"), re->spi->name, S_tochar(DP(tk)->text));
	}
	re->eflags = re->spi->parse_eflags(ctx, opt);
	KNH_SETv(ctx, DP(tk)->data, re);
	knh_Stmt_add(ctx, stmt, tk);
	ITR_ignore(ctx, itr, +0);
}

static void _PARAM(Ctx *ctx, knh_Stmt_t *stmt, tkitr_t *itr);
static void _STMT1(Ctx *ctx, knh_Stmt_t *stmt, tkitr_t *itr);

static void _EXPR(Ctx *ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	int idx = ITR_indexLET(itr);
	knh_Token_t *tkCUR = NULL;
	if(idx != -1) { /* a = b */
		tkitr_t lbuf, *litr = ITR_first(itr, idx, &lbuf, +1);
		tkCUR = itr->ts[idx];
		if(TT_(tkCUR) != TT_LET) { /* i += 1 ==> i = i + 1 */
			itr->c = litr->c;
		}
		if(ITR_size(litr) == 1) {
			if(ITR_isT(litr, isLVALUE)) {
				stmt = new_StmtREUSE(ctx, stmt, STT_LET);
				_VAR(ctx, stmt, litr);
				knh_Stmt_add(ctx, stmt, ITR_nextTK(litr));
				_EXPR(ctx, stmt, itr); return;
			}
		}
		else {
			if(STT_(stmt) != STT_CALL1) {
				knh_Stmt_t *stmt1 = new_Stmt2(ctx, STT_CALL1, NULL);
				knh_Stmt_add(ctx, stmt, stmt1);
				stmt = stmt1;
			}
			_EXPR(ctx, stmt, litr);
			if(STT_(stmt) == STT_CALL) {
				knh_Token_t *tkS = DP(stmt)->tokens[0];
				knh_Token_t *tkM = new_Token(ctx, DP(tkS)->flag0, TT_(tkS));
				KNH_SETv(ctx, DP(tkM)->data, DP(tkS)->data);
				DP(tkM)->mn = DP(tkS)->mn;
				KNH_SETv(ctx, DP(stmt)->tokens[0], tkM);
				DBG_ASSERT(IS_Token(tkM));
				if(knh_Token_isGetter(tkM)) {
					knh_Token_setGetter(tkM, 0);
					knh_Token_setSetter(tkM, 1);
					_EXPR(ctx, stmt, itr); return;
				}
			}
		}
		ITR_perror(ctx, itr, stmt, _("not l-value"));
		goto L_ERROR;
	}

	idx = ITR_indexOPR(ctx, itr);
	if(idx != -1) {
		knh_term_t stt = STT_OP;
		tkCUR = itr->ts[idx];
		knh_term_t tt = TT_(tkCUR);
		if(ITR_size(itr) == 1) {
			knh_Token_perror(ctx, tkCUR, KERR_ERR, _("syntax error"));
			goto L_ERROR;
		}
		switch(tt){
		case TT_QUESTION: {/* @TEST expr ? $expr : $expr */
			int idx2 = ITR_indexTT(itr, TT_COLON, -1);
			if(idx2 != -1) {
				stmt = new_StmtREUSE(ctx, stmt, STT_TRI);
				TT_(tkCUR) = TT_COMMA;
				TT_(itr->ts[idx2]) = TT_COMMA;
				_EXPRs(ctx, stmt, itr); return;
			}
			knh_Token_perror(ctx, tkCUR, KERR_ERR, _("trinary ? needs :"));
			goto L_ERROR;
		}
		case TT_NEXT: case TT_PREV: {
			tkitr_t lbuf, *litr;
			if(ITR_is(itr, tt)) { // ++ a;
				itr->c += 1;
				ITR_toLET(ctx, itr, tkCUR);
				litr = ITR_new(tkCUR, &lbuf);
				_EXPR(ctx, stmt, litr);
			}
			else { // a++;
				int c = itr->c, e = itr->e -1;
				itr->e -= 1;
				if(STT_(stmt) != STT_CALL1) {
					stmt = knh_Stmt_addNewStmt(ctx, stmt, STT_CALL1);
				}
				_EXPR(ctx, stmt, itr);
				if(STT_(stmt) != STT_ERR) {
					knh_Stmt_t *stmtPOST = new_Stmt2(ctx, STT_CALL1, NULL);
					KNH_SETv(ctx, DP(stmt)->stmtPOST, stmtPOST);
					itr->c = c; itr->e = e;
					ITR_toLET(ctx, itr, tkCUR);
					litr = ITR_new(tkCUR, &lbuf);
					_EXPR(ctx, stmtPOST, litr);
				}
			}
			return;
		}
		case TT_TMUL: case TT_EXISTS:
		case TT_LNOT: case TT_NOT:
		case TT_TSUB: case TT_TADD: case TT_ADDR: {
			if(itr->c == idx) {
				knh_Stmt_t *stmtOPR = new_StmtREUSE(ctx, stmt, stt);
				knh_Stmt_add(ctx, stmtOPR, tkCUR);
				ITR_next(itr);
				_EXPR(ctx, stmtOPR, itr); return;
			}
			else {
				if(itr->c + 1 == idx && ITR_isCAST(itr)) {
					goto L_CAST;
				}
				knh_Token_perror(ctx, tkCUR, KERR_ERR, "%s needs next", TT__(TT_(tkCUR)));
				goto L_ERROR;
			}
		}
		case TT_AND: stt = STT_AND; goto L_OPR;
		case TT_OR:  stt = STT_OR; goto L_OPR;
		case TT_ALT: stt = STT_ALT; goto L_OPR;
		default: {
			L_OPR:;
			knh_Stmt_t *stmtOPR = new_StmtREUSE(ctx, stmt, stt);
			if(stt == STT_OP) {
				knh_Stmt_add(ctx, stmtOPR, tkCUR);
			}
			if(TT_isBINARY(tt)) {
				TT_(tkCUR) = TT_COMMA;
			}
			else {
				ITR_replace(itr, tt, TT_COMMA);
			}
			DBG_ASSERT(!ITR_is(itr, TT_COMMA));
			_EXPRs(ctx, stmtOPR, itr);
			TT_(tkCUR) = tt;
			return;
		}
		}/*switch*/
		goto L_ERROR;
	}

	if(ITR_is(itr, TT_QPATH) && itr->c + 1 < itr->e) {
		knh_Token_t *tkCUR = ITR_nextTK(itr);
		knh_Stmt_t *stmtCAST = new_StmtREUSE(ctx, stmt, STT_QCAST);
		_ASIS(ctx, stmtCAST, itr);
		_EXPR(ctx, stmtCAST, itr);
		knh_Stmt_add(ctx, stmtCAST, tkCUR);
		return;
	}
	if(ITR_isCAST(itr)) {
		L_CAST:;
		knh_Stmt_t *stmtCAST = new_StmtREUSE(ctx, stmt, STT_TCAST);
		tkitr_t cbuf, *citr = ITR_new(ITR_nextTK(itr), &cbuf);
		if(ITR_is(citr, TT_TO)) {
			TODO();
			ITR_next(citr)
		}
		if(ITR_isT(citr, isTYPE)) {
			if(ITR_is(citr, TT_TYPE)) {
				knh_Token_toStmtTYPEOF(ctx, ITR_tk(citr));
			}
			knh_Stmt_add(ctx, stmtCAST, ITR_nextTK(citr));
		}
		_EXPR(ctx, stmtCAST, itr); return;
	}

	if(!ITR_hasNext(itr)) {
		_ASIS(ctx, stmt, itr); return;
	}
	else {
		knh_term_t tt = ITR_tt(itr);
		if(tt == TT_REGEX) {
			_REGEX(ctx, stmt, itr); return;
		}
		tkCUR = ITR_nextTK(itr);
		//DBG_P("FIRST ************* tt = %s", TT__(tt));
		if(ITR_hasNext(itr)) {
			stmt = new_StmtREUSE(ctx, stmt, STT_CALL);
		}
		switch(tt) {
		case TT_FMT:
			STT_(stmt) = STT_FMT;
		case TT_FUNCNAME: // f()
		case TT_UNAME:
		case TT_NAME:  // a .f .c
			knh_Stmt_add(ctx, stmt, tkCUR);
			goto L_PARAM;
		case TT_NEW: {
			DBG_ASSERT(STT_(stmt) == STT_CALL);
			STT_(stmt) = STT_NEW;
			knh_Stmt_add(ctx, stmt, tkCUR);
			if(ITR_is(itr, TT_FUNCNAME)) { /* new T() */
				knh_Stmt_add(ctx, stmt, ITR_nextTK(itr));
				goto L_PARAM;
			}
			if(ITR_is(itr, TT_PARENTHESIS)) { /* new () */
				_ASIS(ctx, stmt, itr); goto L_PARAM;
			}
			if(ITR_isT(itr, isTYPE) && ITR_isN(itr, +1, TT_BRANCET)) {
				knh_Token_t *tkC = ITR_nextTK(itr);
				if(!knh_Token_isBYTE(tkC)) {
					tkC = new_TokenPTYPE(ctx, CLASS_Array, tkC);
				}
				TT_(tkCUR) = TT_MN; /* new C[10] */
				DP(tkCUR)->mn = MN_new__ARRAY;
				knh_Stmt_add(ctx, stmt, tkC);
				TT_(itr->ts[itr->c]) = TT_PARENTHESIS;
				goto L_PARAM;
			}
			if(ITR_is(itr, TT_BRANCET)) { /* new [] */
				TT_(itr->ts[itr->c]) = TT_PARENTHESIS;
				_ASIS(ctx, stmt, itr); goto L_PARAM;
			}
			if(ITR_isT(itr, isTYPE) && ITR_isN(itr, +1, TT_BLOCK)) {
				knh_Token_t *tkC = ITR_nextTK(itr);
				DP(tkCUR)->mn = MN_new__MAP;
				_DICT(ctx, stmt, tkC, ITR_nextTK(itr));
				goto L_FUNC;
			}
			if(ITR_is(itr, TT_BLOCK)) {
				DP(tkCUR)->mn = MN_new__MAP;
				_DICT(ctx, stmt, new_Token(ctx, 0, TT_ASIS), ITR_nextTK(itr));
				goto L_FUNC;
			}
			knh_Token_perror(ctx, tkCUR, KERR_ERR, _("new operator needs some parameter(s)"));
			goto L_ERROR;
		}/*TT_NEW*/
		case TT_NULL:
		case TT_TRUE:
		case TT_FALSE:
			knh_Stmt_add(ctx, stmt, tkCUR);
			break;
		case TT_PARENTHESIS: {
			tkitr_t pbuf, *pitr = ITR_new(tkCUR, &pbuf);
			int c = ITR_count(pitr, TT_COMMA);
			if(c == 0) {
				if(ITR_hasNext(pitr)) {
					_EXPR(ctx, stmt, pitr);
				}
				else {
					TT_(tkCUR) = TT_NULL;
					knh_Stmt_add(ctx, stmt, tkCUR);
				}
			}
			else {
#ifdef MN_new__TUPLE
				stmt = new_StmtREUSE(ctx, stmt, STT_NEW);
				_ARRAY(ctx, stmt, MN_new__TUPLE, CLASS_Tuple, pitr);
#endif
				KNH_TODO("new:TUPLE");
			}
			goto L_FUNC;
		}
		case TT_BRANCET: {
			tkitr_t pbuf, *pitr = ITR_new(tkCUR, &pbuf);
			stmt = new_StmtREUSE(ctx, stmt, STT_NEW);
			_ARRAY(ctx, stmt, MN_new__LIST, CLASS_Array, pitr);
			goto L_FUNC;
		}
		case TT_BLOCK:
		case TT_BRACE: {
			stmt = new_StmtREUSE(ctx, stmt, STT_NEW);
			knh_Stmt_add(ctx, stmt, new_TokenMN(ctx, MN_new__MAP));
			_DICT(ctx, stmt, new_TokenCID(ctx, CLASS_Map), tkCUR);
			goto L_FUNC;
		}
		case TT_FUNCTION: { /* function () */
			if(ITR_is(itr, TT_FUNCNAME)) {
				knh_Token_perror(ctx, tkCUR, KERR_DWARN, _("ignored function name: %L"), ITR_nextTK(itr));
			}
			if(ITR_is(itr, TT_PARENTHESIS) && ITR_isN(itr, +1, TT_BLOCK)) {
				tkCUR = new_Token(ctx, 0, TT_DOC);
				STT_(stmt) = STT_FUNCTION;
				_PARAM(ctx, stmt, itr);
				KNH_SETv(ctx, DP(tkCUR)->data, DP(ITR_tk(itr))->text);
				knh_Stmt_add(ctx, stmt, tkCUR);
				_STMT1(ctx, stmt, itr);
				goto L_PARAM;
			}
			knh_Token_perror(ctx, tkCUR, KERR_ERR, _("function must take param and block"));
			goto L_ERROR;
		}
		case TT_TYPE:
		case TT_PROPN:
		case TT_STR:
		case TT_TSTR:
		case TT_ESTR:
		case TT_NUM:
		case TT_QPATH:
		case TT_URN:
			knh_Stmt_add(ctx, stmt, tkCUR); break;
		case TT_ERR: goto L_ERROR;
		default:
			knh_Token_perror(ctx, tkCUR, KERR_ERR, _("unknown token: %L"), tkCUR);
			goto L_ERROR;
		}
	}

	L_FUNC:;
	if(!ITR_hasNext(itr)) return;
	if(ITR_isDOTNAME(itr, 0)) {
		stmt = knh_Stmt_addFUNC(ctx, stmt, ITR_nextTK(itr));
	}
	else if(ITR_is(itr, TT_BRANCET)) {
		tkitr_t pbuf, *pitr = ITR_new(ITR_nextTK(itr), &pbuf);
		int idx = ITR_indexTOUNTIL(pitr);
		if(idx == -1) {
			stmt = knh_Stmt_addMN(ctx, stmt, MN_get);
			_EXPRs(ctx, stmt, pitr);
		}
		else {
			knh_methodn_t mn = (TT_(pitr->ts[idx]) == TT_TO) ? MN_opTO : MN_opUNTIL;
			stmt = knh_Stmt_addMN(ctx, stmt, mn);
			TT_((pitr)->ts[idx]) = TT_COMMA;
			_ASISEXPRs(ctx, stmt, pitr);
		}
		goto L_FUNC;
	}
	else {
		if(TT_(tkCUR) == TT_NAME) {

		}
		tkCUR = ITR_tk(itr);
		DBG_P("** Unexpected funcname %s **", TT__(TT_(tkCUR)));
		knh_Token_perror(ctx, tkCUR, KERR_ERR, _("syntax error: function? %L"), tkCUR);
		goto L_ERROR;
	}

	L_PARAM:
	if(!ITR_hasNext(itr)) return;
	if(ITR_is(itr, TT_PARENTHESIS)) {
		tkitr_t pbuf, *pitr = ITR_new(ITR_nextTK(itr), &pbuf);
		if(DP(stmt)->size == 1) _ASIS(ctx, stmt, itr);
		_EXPRs(ctx, stmt, pitr);
	}
	else if(TT_isSTR(ITR_tt(itr))) {
		if(DP(stmt)->size == 1) _ASIS(ctx, stmt, itr);
		_EXPR(ctx, stmt, itr);
		return;
	}
	goto L_FUNC;

	L_ERROR:;
	if(tkCUR != NULL) {
		DBG_P("TO ERROR stt=%s", TT__(STT_(stmt)));
		knh_Stmt_toERR(ctx, stmt, TM(tkCUR));
	}
	return;
}

typedef void (*knh_Fstmt)(Ctx *, knh_Stmt_t *stmt, tkitr_t *itr);

static void _PEXPR(Ctx *ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	if(ITR_is(itr, TT_PARENTHESIS)) {
		knh_term_t stt = STT_(stmt); (void)stt;
		tkitr_t pbuf, *pitr = ITR_new(ITR_nextTK(itr), &pbuf);
		_EXPR(ctx, stmt, pitr);
		DBG_P("stt=%s => %s", TT__(stt), TT__(STT_(stmt)));
	}
	else {
		ITR_perror(ctx, itr, stmt, _("needs ()"));
	}
}

/* ------------------------------------------------------------------------ */

static void _DECL(Ctx *ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	DBG_ASSERT(ITR_isT(itr, isTYPE));
	knh_Token_t *tkT = ITR_nextTK(itr);
	tkitr_t declbuf, *declitr = ITR_stmt(ctx, itr, +0, &declbuf, 0/*need;*/);
	while(ITR_hasNext(declitr)) {
		int idx = ITR_indexTT(declitr, TT_COMMA, declitr->e);
		tkitr_t abuf, *aitr = ITR_first(declitr, idx, &abuf, +1);
		if(isVARN(ITR_tk(aitr))) {
			knh_Stmt_add(ctx, stmt, tkT);
			knh_Stmt_add(ctx, stmt, ITR_nextTK(aitr));
		}
		else {
			ITR_perror(ctx, declitr, stmt, _("not variable: %L"));
			return;
		}
		if(ITR_is(aitr, TT_LET)) {
			ITR_next(aitr);
			_EXPR(ctx, stmt, aitr);
		}
		else {
			_ASIS(ctx, stmt, aitr);
		}
		if(ITR_hasNext(declitr)) {
			knh_Stmt_t *stmtDECL = new_Stmt2(ctx, STT_DECL, NULL);
			KNH_INITv(DP(stmt)->nextNULL, stmtDECL);
			stmt = stmtDECL;
		}
	}
}

static void _PARAMs(Ctx *ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	knh_Stmt_t *stmtP = new_Stmt2(ctx, STT_DECL, NULL);
	int isRPARAM = 0, rsize = 0;
	int e = itr->e, ridx = ITR_indexTT(itr, TT_FUNCARROW, e);
	if(ridx != -1) {
		itr->e = ridx;
	}
	knh_Stmt_add(ctx, stmt, stmtP);
	L_PARAM:;
	while(ITR_hasNext(itr)) {
		int idx = ITR_indexTT(itr, TT_COMMA, itr->e);
		tkitr_t abuf, *aitr = ITR_first(itr, idx, &abuf, +1);
		if(ITR_is(aitr, TT_DOTS)) {
			knh_Stmt_setVARGs(stmtP, 1);
			break;
		}
		if(ITR_is(aitr, TT_VOID)) {
			break;
		}
		if(ITR_isT(aitr, isTYPE)) {
			knh_Stmt_add(ctx, stmtP, ITR_nextTK(aitr));
		}
		if(DP(stmtP)->size % 3 != 1) {
			_VAR(ctx, stmtP, aitr);
		}
		if(ITR_isT(aitr, isVARN)) {
			knh_Token_t *tkN = ITR_nextTK(aitr);
			knh_Token_setDOT(tkN, isRPARAM);
			knh_Stmt_add(ctx, stmtP, tkN);
		}
		else if(isRPARAM) {
			knh_Token_t *tkN = new_Token(ctx, FLAG_Token_DOT, TT_ASIS);
			knh_Stmt_add(ctx, stmtP, tkN);
		}
		else {
			ITR_perror(ctx, aitr, stmt, _("no variables"));
			return;
		}
		if(isRPARAM) {
			rsize++;
			if(rsize == 1 && STT_(stmt) == STT_METHOD) {
				KNH_SETv(ctx, DP(stmt)->terms[0], DP(stmtP)->tokens[DP(stmtP)->size - 2]);
			}
		}
		if(ITR_is(aitr, TT_LET)) {
			ITR_next(aitr);
			_EXPR(ctx, stmtP, aitr);
		}
		else {
			_ASIS(ctx, stmtP, aitr);
		}
	}
	if(e != itr->e) {
		itr->c = ridx + 1;
		itr->e = e;
		isRPARAM = 1;
		goto L_PARAM;
	}
}

static void _PARAM(Ctx *ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	if(ITR_is(itr, TT_PARENTHESIS)) {
		tkitr_t pbuf, *pitr = ITR_new(ITR_nextTK(itr), &pbuf);
		_PARAMs(ctx, stmt, pitr);
	}
	else {
		ITR_perror(ctx, itr, stmt, _("needs (parameters..)"));
	}
}

static void _STMTs(Ctx *ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	knh_Stmt_t *stmtHEAD = NULL, *stmtTAIL = NULL;
	while(ITR_hasNext(itr)) {
		knh_Stmt_t *stmtCUR = new_StmtSTMT1(ctx, itr);
		if(stmtHEAD == NULL) {
			stmtHEAD = stmtCUR;
		}
		else {
			KNH_INITv(DP(stmtTAIL)->nextNULL, stmtCUR);
		}
		stmtTAIL = stmtCUR;
		while(DP(stmtTAIL)->nextNULL != NULL) stmtTAIL = DP(stmtTAIL)->nextNULL;
	}
	if(stmtHEAD == NULL) {
		stmtHEAD = new_Stmt2(ctx, STT_DONE, NULL);
	}
	knh_Stmt_add(ctx, stmt, stmtHEAD);
}

static void _STMT1(Ctx *ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	if(ITR_is(itr, TT_BLOCK)) {
		knh_Token_toBLOCK(ctx, ITR_tk(itr));
	}
	if(ITR_is(itr, TT_BRACE)) {
		tkitr_t stmtbuf, *stmtitr = ITR_new(ITR_nextTK(itr), &stmtbuf);
		_STMTs(ctx, stmt, stmtitr);
	}
	else {
		knh_Stmt_add(ctx, stmt, new_StmtSTMT1(ctx, itr));
	}
}

static void _ELSE(Ctx *ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	if(ITR_is(itr, TT_ELSE)) {
		ITR_next(itr);
		_STMT1(ctx, stmt, itr);
	}
	else {
		knh_Stmt_add(ctx, stmt, new_Stmt2(ctx, STT_DONE, NULL));
	}
}

static void _DOWHILE(Ctx *ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	if(ITR_is(itr, TT_BLOCK)) {
		knh_Token_toBLOCK(ctx, ITR_tk(itr));
	}
	if(ITR_is(itr, TT_BRACE)) {
		tkitr_t stmtbuf, *stmtitr = ITR_new(ITR_nextTK(itr), &stmtbuf);
		_STMTs(ctx, stmt, stmtitr);
		if(ITR_is(itr, TT_WHILE)) {
			ITR_next(itr);
		}
		return;
	}
	ITR_perror(ctx, itr, stmt, _("do must take {} while"));
}

static void _CASEEXPR(Ctx *ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	int idx = ITR_indexTT(itr, TT_COLON, -1);
	if(idx == -1) {
		ITR_perror(ctx, itr, stmt, _("case needs :"));
	}
	else {
		itr->e = idx;
		_EXPR(ctx, stmt, itr);
		itr->c = idx + 1;
	}
}

static void _CASESTMT(Ctx *ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	int i;
	for(i = itr->c; i < itr->e; i++) {
		knh_term_t tt = TT_(itr->ts[i]);
		if(tt == TT_CASE || tt == TT_DEFAULT) {
			break;
		}
	}
	itr->e = i;
	_STMTs(ctx, stmt, itr);
}

static int ITR_indexINFROM(tkitr_t *itr)
{
	if(ITR_size(itr) > 2) {
		int i, c = itr->c + 1;
		if(TT_(itr->ts[c]) == TT_UNAME) c++;  // skip InputSteam in
		for(i = c; i < itr->e; i++) {
			knh_term_t tt = TT_(itr->ts[i]);
			if(tt == TT_FROM || tt == TT_COLON) {
				return i;
			}
			if(tt == TT_NAME && ISB(S_tobytes(DP(itr->ts[i])->text), "in")) {
				return i;
			}
		}
	}
	return -1;
}

static void _EACH(Ctx *ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	int idx = ITR_indexINFROM(itr);
	if(idx != -1) {
		tkitr_t pbuf, *pitr = ITR_first(itr, idx, &pbuf, +1);
		_PARAMs(ctx, stmt, pitr);
		_EXPR(ctx, stmt, itr);
	}
	else {
		ITR_perror(ctx, itr, stmt, _("needs from/in in foreach"));
	}
}

static void _PEACH(Ctx *ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	if(ITR_is(itr, TT_PARENTHESIS)) {
		tkitr_t pbuf, *pitr = ITR_new(ITR_nextTK(itr), &pbuf);
		_EACH(ctx, stmt, pitr);
	}
	else {
		int idx = ITR_indexTT(itr, TT_BLOCK, -1);
		if(idx != -1) {
			tkitr_t pbuf, *pitr = ITR_first(itr, idx, &pbuf, 0);
			_EACH(ctx, stmt, pitr);
		}
	}
}

static void _PSTMT3(Ctx *ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	if(ITR_is(itr, TT_PARENTHESIS)) {
		tkitr_t pbuf, *pitr = ITR_new(ITR_nextTK(itr), &pbuf);
		if(ITR_count(pitr, TT_SEMICOLON) == 2) {
			int idx = ITR_indexTT(pitr, TT_SEMICOLON, itr->e);
			tkitr_t stmtbuf, *stmtitr = ITR_first(pitr, idx, &stmtbuf, +1);
			stmtitr->e += 1; // including ";"
			knh_Stmt_add(ctx, stmt, new_StmtSTMT1(ctx, stmtitr));
			idx = ITR_indexTT(pitr, TT_SEMICOLON, itr->e);
			stmtitr = ITR_first(pitr, idx, &stmtbuf, +1);
			if(!ITR_hasNext(stmtitr)) {
				knh_Stmt_add(ctx, stmt, new_Token(ctx, 0, TT_TRUE));
			}
			else {
				_EXPR(ctx, stmt, stmtitr);
			}
			knh_Stmt_add(ctx, stmt, new_StmtSTMT1(ctx, pitr));
			return;
		}
		else if(ITR_indexINFROM(pitr) != -1) {
			_EACH(ctx, stmt, pitr); return;
		}
	}
	ITR_perror(ctx, itr, stmt, _("for statement needs (;;)"));
}

static int isLABEL(knh_Token_t *tk)
{
	knh_term_t tt = TT_(tk);
	return (tt == TT_UNAME || tt == TT_NAME);
}

static void _LABEL(Ctx *ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	knh_Stmt_tadd(ctx, stmt, itr, isLABEL, NULL);
}

static int isEVENT(knh_Token_t *tk)
{
	if(knh_Token_isExceptionType(tk)) return 1;
	return 0;
}

knh_Stmt_t *knh_Stmt_tail(Ctx *ctx, knh_Stmt_t *o)
{
	knh_Stmt_t *tail = o;
	while(DP(tail)->nextNULL != NULL) {
		tail = DP(tail)->nextNULL;
	}
	return tail;
}

static knh_Stmt_t* knh_StmtNULL_append(Ctx *ctx, knh_Stmt_t *o, knh_Stmt_t *stmt)
{
	if(o == NULL) {
		return stmt;
	}
	else {
		knh_Stmt_t *tail = knh_Stmt_tail(ctx, o);
		KNH_INITv(DP(tail)->nextNULL, stmt);
		return o;
	}
}

static void _CATCH(Ctx *ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	knh_Stmt_t *stmtHEAD = NULL;
	while(ITR_is(itr, TT_CATCH)) {
		ITR_next(itr);
		if(ITR_is(itr, TT_PARENTHESIS)) {
			tkitr_t pbuf, *pitr = ITR_new(ITR_nextTK(itr), &pbuf);
			knh_Stmt_t *stmtCATCH = new_Stmt2(ctx, STT_CATCH, NULL);
			knh_Stmt_tadd(ctx, stmtCATCH, pitr, isEVENT, _("not exception: %s"));
			knh_Stmt_tadd(ctx, stmtCATCH, pitr, isVARN, _("not variable: %s"));
			_STMT1(ctx, stmtCATCH, itr);
			stmtHEAD = knh_StmtNULL_append(ctx, stmtHEAD, stmtCATCH);
		}
		else {
			ITR_perror(ctx, itr, stmt, _("nothing to catch"));
			return;
		}
	}
	if(stmtHEAD == NULL) {
		stmtHEAD = new_Stmt2(ctx, STT_DONE, NULL);
	}
	knh_Stmt_add(ctx, stmt, stmtHEAD);
	if(ITR_is(itr, TT_FINALLY)) {
		ITR_next(itr);
		_STMT1(ctx, stmt, itr);
	}
	else {
		_DONE(ctx, stmt, itr);
	}
}

static void _PRAGMA(Ctx *ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	while(ITR_hasNext(itr)) {
		knh_Stmt_add(ctx, stmt, ITR_nextTK(itr));
	}
}

static void _USING(Ctx *ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	while(ITR_hasNext(itr)) {
		knh_Token_t *tk = ITR_nextTK(itr);
		if(TT_(tk) == TT_OR) _ASIS(ctx, stmt, itr);
		knh_Stmt_add(ctx, stmt, tk);
	}
	_ASIS(ctx, stmt, itr);
}

static int isCLASSAME(knh_Token_t* tk)
{
	if(TT_(tk) == TT_UNAME) {
		knh_bytes_t t = S_tobytes(DP(tk)->text);
		if(knh_bytes_index(t, ':') == -1) {
			return isupper(t.ustr[0]);
		}
		return 1;
	}
	return 0;
}

static void _CLASS(Ctx *ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{	// CNAME:0 CPARAM:1 EXTENDS:2 IMPLEMENTS:3 STMT:4
	knh_Stmt_tadd(ctx, stmt, itr, isCLASSAME, _("not class: %s")); /*0*/
	_ASIS(ctx, stmt, itr); /* Generics for future : 1*/
	if(ITR_is(itr, TT_EXTENDS)) {
		ITR_next(itr);
		knh_Stmt_tadd(ctx, stmt, itr, isCLASSAME, _("not class: %s")); /*2*/
	}
	else { /* Object */
		knh_Stmt_add(ctx, stmt, new_TokenCID(ctx, CLASS_Object));
	}
	if(ITR_is(itr, TT_IMPLEMENTS)) {
		ITR_skipUNTIL(itr, TT_BLOCK);
		_ASIS(ctx, stmt, itr); /* implements for future 3*/
	}
	else {
		_ASIS(ctx, stmt, itr);
	}
	if(ITR_is(itr, TT_BLOCK)) {
		knh_Stmt_add(ctx, stmt, ITR_nextTK(itr));
	}
	else {
		_SEMICOLON(ctx, stmt, itr);
	}
}

static int isMN(knh_Token_t* tk)
{
	return (TT_(tk) == TT_FUNCNAME/* || TT_(tk) == TT_PROPN*/);
}

static void _METHOD(Ctx *ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	knh_Token_t *tkT = ITR_nextTK(itr);
	knh_Stmt_add(ctx, stmt, tkT);
	if(TT_(tkT) == TT_FUNCTION) TT_(tkT) = TT_ASIS;
	if(ITR_is(itr, TT_UNAME)) {
		knh_Stmt_tadd(ctx, stmt, itr, isCLASSAME, _("not class: %s"));
	}
	else {
		_ASIS(ctx, stmt, itr);
	}
	knh_Stmt_tadd(ctx, stmt, itr, isMN, NULL);
	ADD(stmt, _PARAM(ctx, stmt, itr));
	if(STT_(stmt) != STT_ERR) {
		if(ITR_is(itr, TT_BLOCK)) {
			tkT = new_Token(ctx, 0, TT_DOC);
			KNH_SETv(ctx, DP(tkT)->data, DP(ITR_tk(itr))->text);
			knh_Stmt_add(ctx, stmt, tkT);
			knh_Stmt_add(ctx, stmt, ITR_nextTK(itr));
		}
		else {
			_SEMICOLON(ctx, stmt, itr);
		}
	}
}

static void _CONSTRUCTOR(Ctx *ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	knh_Token_t *tkT = ITR_tk(itr);
	TT_(tkT) = TT_UNAME;
	knh_Stmt_tadd(ctx, stmt, itr, isCLASSAME, _("not class: %s"));
	_ASIS(ctx, stmt, itr);
	knh_Stmt_add(ctx, stmt, new_TokenMN(ctx, MN_new));
	ADD(stmt, _PARAM(ctx, stmt, itr));
	if(STT_(stmt) != STT_ERR) {
		if(ITR_is(itr, TT_BLOCK)) {
			tkT = new_Token(ctx, 0, TT_DOC);
			KNH_SETv(ctx, DP(tkT)->data, DP(ITR_tk(itr))->text);
			knh_Stmt_add(ctx, stmt, tkT);
			knh_Stmt_add(ctx, stmt, ITR_nextTK(itr));
		}
		else {
			_SEMICOLON(ctx, stmt, itr);
		}
	}
}

static void _FORMAT(Ctx *ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	knh_Token_t *tkT = ITR_nextTK(itr);
	knh_Stmt_add(ctx, stmt, tkT);  // void
	ITR_nextTK(itr); // skip %
	_ASIS(ctx, stmt, itr);  // class name
	knh_Stmt_add(ctx, stmt, ITR_nextTK(itr));
	_PARAM(ctx, stmt, itr);
	if(STT_(stmt) != STT_ERR) {
		if(ITR_is(itr, TT_BLOCK)) {
			knh_Token_t *tkT = new_Token(ctx, 0, TT_DOC);
			KNH_SETv(ctx, DP(tkT)->data, DP(ITR_tk(itr))->text);
			knh_Stmt_add(ctx, stmt, tkT);
			knh_Stmt_add(ctx, stmt, ITR_nextTK(itr));
		}
		else if(ITR_is(itr, TT_STR) || ITR_is(itr, TT_TSTR) || ITR_is(itr, TT_ESTR)) {
			knh_Stmt_add(ctx, stmt, ITR_nextTK(itr));
			//_SEMICOLON(ctx, stmt, itr);
		}
		else {
			_SEMICOLON(ctx, stmt, itr);
		}
	}
}

/* ------------------------------------------------------------------------ */
/* META */

static void knh_Stmt_addMETA(Ctx *ctx, knh_Stmt_t *stmt, tkitr_t *itr)
{
	int i, e;
	knh_Token_t **ts = itr->ts;
	if(itr->c < itr->e && SP(stmt)->line == 0) {
		knh_Term_setLine(stmt, itr->ts[itr->c]);
	}
	if(itr->meta == -1 || STT_(stmt) == STT_DONE || STT_(stmt) == STT_ERR) {
		return ;
	}
	e = (itr->meta < itr->c) ? itr->c : itr->e;
	if(IS_NULL(DP(stmt)->metaDictCaseMap)) {
		KNH_SETv(ctx, DP(stmt)->metaDictCaseMap, new_DictCaseMap0(ctx, 2));
	}
	for(i = itr->meta; i < e; i++) {
		if(TT_(ts[i]) == TT_NAME || TT_(ts[i]) == TT_UNAME) {
			knh_DictCaseMap_set(ctx, DP(stmt)->metaDictCaseMap, TS_ATlabel, ts[i]);
			i++;
			DBG_ASSERT(TT_(ts[i]) == TT_COLON);
		}
		else if(TT_(ts[i]) == TT_METAN) {
			knh_DictCaseMap_set(ctx, DP(stmt)->metaDictCaseMap, DP(ts[i])->text, UPCAST(ts[i]));
		}
	}
	if(e == itr->e) itr->c = i;
	itr->meta = -1;
}

static knh_Stmt_t *new_StmtMETA(Ctx *ctx, knh_term_t stt, tkitr_t *itr, int shift, ...)
{
	knh_Fstmt func = NULL;
	va_list ap;
	knh_Stmt_t *stmt = new_Stmt2(ctx, stt, NULL);
	knh_Stmt_addMETA(ctx, stmt, itr);
	itr->c = itr->c + shift;
	va_start(ap , shift);
	while((func = (knh_Fstmt)va_arg(ap, knh_Fstmt)) != NULL) {
		if(STT_(stmt) == STT_ERR) {
			return stmt;
		}
		func(ctx, stmt, itr);
	}
	va_end(ap);
	return stmt;
}

/* ------------------------------------------------------------------------ */
/* [STMT1] */

#define CASE_(XX, ...) case TT_##XX : { \
		stmt = new_StmtMETA(ctx, STT_##XX , itr, +1, ## __VA_ARGS__, NULL); \
		break;\
	}\

#define CASE_L(XX, ...) case TT_##XX : { \
		tkitr_t stmtbuf, *stmtitr = ITR_stmt(ctx, itr, 1, &stmtbuf, 1);\
		stmt = new_StmtMETA(ctx, STT_##XX , stmtitr, +1, ## __VA_ARGS__, NULL);\
		break;\
	}\

static int knh_Token_isMAP(Ctx *ctx, knh_Token_t *tk)
{
	if(TT_(tk) == TT_BRACE) {
		tkitr_t tbuf, *titr = ITR_new(tk, &tbuf);
		int i, c = 0;
		for(i = 0; i < titr->e; i++) {
			knh_Token_t *tkI = titr->ts[i];
			if(TT_(tkI) < TT_DOTS || TT_(tkI) == TT_LET) return 0;
			if(TT_(tkI) == TT_COLON) c++;
		}
		if(titr->e == 0 || c > 0) return 1;
	}
	return 0;
}

static knh_Stmt_t *new_StmtSTMT1(Ctx *ctx, tkitr_t *itr)
{
	knh_term_t tt;
	knh_Stmt_t *stmt = NULL;
	knh_Token_t *tkLINE = NULL;
	L_TAIL:;
	TT_skipMETA(itr);
	if(!ITR_hasNext(itr)) {
		stmt = new_Stmt2(ctx, STT_DONE, NULL);
		goto L_RETURN;
	}
	tkLINE = ITR_tk(itr);
	tt = ITR_tt(itr);
	switch(tt) {
		case TT_SEMICOLON: {
			ITR_next(itr);
			goto L_TAIL;
		}
		CASE_L(PRAGMA, _PRAGMA)
		CASE_(NAMESPACE, _STMT1);
		CASE_(SCRIPT, _STMT1);
		CASE_L(INCLUDE, _EXPRs);
		CASE_L(USING, _USING);
		CASE_(CLASS, _CLASS);
		CASE_L(BREAK, _LABEL);
		CASE_L(CONTINUE, _LABEL);
		CASE_L(RETURN, _EXPRs);
		CASE_L(THROW, _EXPRs);
		CASE_L(REGISTER, _EXPRs);
		CASE_L(PRINT, _EXPRs);
		CASE_L(YEILD, _EXPRs);
		CASE_(IF, _PEXPR, _STMT1, _ELSE);
		CASE_(SWITCH, _PEXPR, _STMT1, _ASIS);  /* it */
		CASE_(CASE, _CASEEXPR, _CASESTMT);
		CASE_(WHILE, _PEXPR, _STMT1);
		CASE_(DO, _DOWHILE, _PEXPR, _SEMICOLON);
		CASE_(FOR, _PSTMT3, _STMT1);
		CASE_(FOREACH, _PEACH, _STMT1, _ASIS);  /* it */
		CASE_(TRY, _STMT1, _CATCH, _ASIS);  /* it */
		CASE_(ASSERT, _PEXPR, _STMT1);

		case TT_BLOCK:
			knh_Token_toBLOCK(ctx, ITR_tk(itr));
		case TT_BRACE:
			if(!knh_Token_isMAP(ctx, ITR_tk(itr))) {
				tkitr_t pbuf, *pitr = ITR_new(ITR_nextTK(itr), &pbuf);
				stmt = new_StmtMETA(ctx, STT_BLOCK, pitr, 0, _STMTs, NULL);
			}
			break /*L_EXPR*/;
		case TT_VOID: {
			if(ITR_isN(itr, +1, TT_MOD) && ITR_isN(itr, +2, TT_FUNCNAME)) {
				stmt = new_StmtMETA(ctx, STT_FORMAT, itr, 0, _FORMAT, NULL);
				break;
			}
		}
		case TT_VAR:
		case TT_TYPE:
		case TT_UNAME: {
			tkitr_t mbuf, *mitr = ITR_copy(itr, &mbuf, +1);
			if(ITR_isT(mitr, isVARN) && tt != TT_VOID) {
				tkitr_t dbuf, *ditr = ITR_stmt(ctx, itr, /*pos*/0, &dbuf, 1/*needs;*/);
				stmt = new_StmtMETA(ctx, STT_DECL, ditr, 0, _DECL, NULL);
				break;
			}
			if(ITR_is(mitr, TT_UNAME)) {
				ITR_next(mitr);
				if(ITR_is(mitr, TT_FUNCNAME)) {
					stmt = new_StmtMETA(ctx, STT_METHOD, itr, 0, _METHOD, NULL);
					break;
				}
			}
			if(ITR_is(mitr, TT_FUNCNAME) && !knh_Token_isDOT(ITR_tk(mitr))) {
				stmt = new_StmtMETA(ctx, STT_METHOD, itr, 0, _METHOD, NULL);
				break;
			}
			break /*L_EXPR*/;
		}
		case TT_FUNCTION: {
			if(ITR_isN(itr, +1, TT_FUNCNAME)) {
				stmt = new_StmtMETA(ctx, STT_METHOD, itr, 0, _METHOD, NULL);
				break;
			}
			break /*L_EXPR*/;
		}
		case TT_PROPN:
		case TT_NAME: {
			tkitr_t sbuf, *sitr = ITR_stmt(ctx, itr, /*pos*/0, &sbuf, 0/*needs;*/);
			int comma = ITR_count(sitr, TT_COMMA);
			if(comma > 0) {
				int idx = ITR_indexTT(sitr, TT_LET, itr->e);
				tkitr_t lbuf, *litr = ITR_first(sitr, idx, &lbuf, +1);
				int lcomma = ITR_count(litr, TT_COMMA);
				if(lcomma == comma) {
					stmt = new_StmtMETA(ctx, STT_LETM, litr, 0, _EXPRs, NULL);
					if(STT_(stmt) != STT_ERR) {
						_EXPR(ctx, stmt, sitr);
					}
				}
				else if(lcomma * 2 == comma) {
					stmt = new_StmtMETA(ctx, STT_PUSHM, litr, 0, _EXPRs, NULL);
					if(STT_(stmt) != STT_ERR) {
						_EXPRs(ctx, stmt, sitr);
					}
				}
				else {
					stmt = new_StmtMETA(ctx, STT_ERR, sitr, 0, NULL);
					knh_Token_perror(ctx, sitr->ts[idx], KERR_ERR, _("different amount of assignment"));
				}
				break;
			}
			else {
				itr->c = sitr->c;
			}
		}
		case TT_FUNCNAME: {
			if(isupper(S_tobytes(DP(ITR_tk(itr))->text).ustr[0])) {
				if(ITR_isN(itr, +2, TT_BLOCK)) {
					stmt = new_StmtMETA(ctx, STT_METHOD, itr, 0, _CONSTRUCTOR, NULL);
					break;
				}
			}
		}
		case TT_FMT:
		case TT_PARENTHESIS:
		case TT_BRANCET:
		case TT_TRUE: case TT_FALSE: case TT_NULL:
		case TT_NOT: case TT_EXISTS: case TT_QPATH:
		case TT_LNOT:
		case TT_NEXT: case TT_PREV:  /* Prev */
		case TT_ITR: case TT_NEW:
		case TT_TADD: case TT_TMUL:
		case TT_ADDR: case TT_TSUB:
		case TT_NUM:
		case TT_STR: case TT_TSTR: case TT_FMTSTR:
		case TT_ESTR: case TT_REGEX:
		case TT_URN: break; // EXPR

		case TT_ERR:  default: {
			tkitr_t sbuf, *sitr = ITR_stmt(ctx, itr, /*pos*/0, &sbuf, 0/*needs;*/);
			stmt = new_StmtMETA(ctx, STT_ERR, sitr, 0, NULL);
			ITR_perror(ctx, sitr, stmt, _("not the begin of an expression"));
			break;
		}
	}
	if(stmt == NULL) {
		tkitr_t sbuf, *sitr = ITR_stmt(ctx, itr, /*pos*/0, &sbuf, 0/*needs;*/);
		stmt = new_StmtMETA(ctx, STT_CALL1, sitr, 0, _EXPR, NULL);
	}
	L_RETURN:;
	if(tkLINE != NULL && SP(stmt)->line == 0) {
		knh_Term_setLine(stmt, tkLINE);
	}
	return stmt;
}

/* ------------------------------------------------------------------------ */

knh_Stmt_t *knh_InputStream_parseStmt(Ctx *ctx, knh_InputStream_t *in)
{
	knh_Stmt_t *rVALUE = new_Stmt2(ctx, STT_BLOCK, NULL);
	knh_Token_t *tk = new_Token(ctx, 0, TT_BRACE);
	BEGIN_LOCAL(ctx, lsfp, 2);
	KNH_SETv(ctx, lsfp[0].o, tk);
	KNH_SETv(ctx, lsfp[1].o, rVALUE);
	DBG_ASSERT(DP(in)->line != 0);
	knh_Token_setLine(tk, in);
	knh_Term_setLine(rVALUE, tk);
	knh_InputStream_parseToken(ctx, in, tk);
	if(knh_isSystemVerbose() /*&& DP(in)->uri == URI_EVAL*/) {
		knh_dump_token(ctx, KNH_STDOUT, tk);
	}
	if(TT_(tk) != TT_ERR) {
		tkitr_t tbuf, *titr = ITR_new(tk, &tbuf);
		_STMTs(ctx, rVALUE, titr);
		DBG_ASSERT(DP(rVALUE)->size > 0);
		rVALUE = DP(rVALUE)->stmts[0];
	}
	else {
		knh_Stmt_toERR(ctx, rVALUE, TM(tk));
	}
	if(knh_isSystemVerbose() /*&& DP(in)->uri == URI_EVAL*/) {
		knh_dump_stmt(ctx, KNH_STDOUT, rVALUE, 1);
		knh_write_EOL(ctx, KNH_STDOUT);
	}
	END_LOCAL(ctx, lsfp);
	return rVALUE;
}

/* ------------------------------------------------------------------------ */

knh_Stmt_t *knh_Token_parseStmt(Ctx *ctx, knh_Token_t *tk)
{
	knh_Stmt_t *rVALUE = new_Stmt2(ctx, STT_BLOCK, NULL);
	knh_Token_toBLOCK(ctx, tk);
	knh_Term_setLine(rVALUE, tk);
	BEGIN_LOCAL(ctx, lsfp, 1);
	KNH_SETv(ctx, lsfp[0].o, rVALUE);
	if(TT_(tk) != TT_ERR) {
		tkitr_t tbuf, *titr = ITR_new(tk, &tbuf);
		_STMTs(ctx, rVALUE, titr);
		DBG_ASSERT(DP(rVALUE)->size > 0);
		rVALUE = DP(rVALUE)->stmts[0];
	}
	else {
		knh_Stmt_toERR(ctx, rVALUE, TM(tk));
	}
	END_LOCAL(ctx, lsfp);
	return rVALUE;
}

/* ------------------------------------------------------------------------ */

static void knh_Array_addTEXT(Ctx *ctx, knh_Array_t *a, knh_cwb_t *cwb)
{
	if(knh_cwb_size(cwb) != 0) {
		knh_Array_add(ctx, a, KNH_NULL);
		knh_Array_add(ctx, a, knh_cwb_newString(ctx, cwb));
	}
}

static knh_bool_t knh_Array_addEXPR(Ctx *ctx, knh_Array_t *a, knh_bytes_t mt, knh_bytes_t expr, knh_uri_t uri, int line)
{
	knh_Array_add(ctx, a, new_S(ctx, mt));
	if(expr.ustr[0] == '#') {
		knh_Array_add(ctx, a, new_S(ctx, expr));
		return 1;
	}
	else {
		knh_Stmt_t *stmt;
		BEGIN_LOCAL(ctx, lsfp, 1);
		LOCAL_NEW(ctx, lsfp, 0, knh_Token_t*, tk, new_Token(ctx, 0, TT_BLOCK));
		KNH_SETv(ctx, DP(tk)->data, new_S(ctx, expr));
		SP(tk)->uri = uri;
		SP(tk)->line = line;
		stmt = knh_Token_parseStmt(ctx, tk);
		knh_Array_add(ctx, a, stmt);
		END_LOCAL(ctx, lsfp);
		return (STT_(stmt) != STT_ERR);
	}
}

knh_bool_t knh_String_parseFMT(Ctx *ctx, knh_String_t *fmt, knh_Array_t *a, knh_uri_t uri, int line)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_bytes_t t = S_tobytes(fmt);
	size_t i = 0, s = 0;
	L_TEXT:;
	for(;i < t.len; i++) {
		if(t.ustr[i] == '%') {
			i++;
			if(t.ustr[i] == '%') {
				knh_Bytes_putc(ctx, cwb->ba, '%');
				continue;
			}
			goto L_FMT;
		}
		if(t.ustr[i] == '\n') line++;
		knh_Bytes_putc(ctx, cwb->ba, t.ustr[i]);
	}
	knh_Array_addTEXT(ctx, a, cwb);
	return 1;
	L_FMT:;
	s = i-1;
	for(;i < t.len; i++) {
		if(t.ustr[i] == '\n') line++;
		if(t.ustr[i] == '{') {
			knh_bytes_t mt = {{t.text + s}, i - s};
			knh_bytes_t t2 = {{t.text + (i+1)}, t.len - (i+1)};
			knh_index_t loc = knh_bytes_index(t2, '}');
			if(loc != -1) {
				knh_Array_addTEXT(ctx, a, cwb);
				if(!knh_Array_addEXPR(ctx, a, mt, knh_bytes_first(t2, loc), uri, line)) {
					return 0;
				}
				knh_cwb_close(cwb);
				i = i + 1 + loc + 1; s = i;
				//DBG_P("START='%c'", t.ustr[s]);
				if(s < t.len) goto L_TEXT;
				return 1;
			}
			break;
		}
	}
	KNH_SYSLOG(ctx, LOG_WARNING, "BrokenFormatter", "'%B'", t);
	knh_cwb_close(cwb);
	return 0;
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
