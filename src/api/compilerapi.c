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
#define knh_token_isNested(tt)  (tt == TT_BRACE || tt == TT_PARENTHESIS || tt == TT_BRANCET)

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef KNH_CC_METHODAPI

/* ======================================================================== */
/* [Stmt] */

/* ------------------------------------------------------------------------ */
//## method void Token.%s(OutputStream w, String m);

static
void knh_Token__s(Ctx *ctx, knh_Token_t *o, knh_OutputStream_t *w, knh_String_t *m)
{
	KNH_ASSERT(IS_Token(o));
	if(SP(o)->tt < TT_NUM || SP(o)->tt == TT_ASIS) {
		knh_write_char(ctx, w, knh_token_tochar(SP(o)->tt));
	}else if(SP(o)->tt == TT_CID) {
		knh_write_char(ctx, w, CLASSN(DP(o)->cid));
	}else if(SP(o)->tt == TT_FN) {
		knh_write_char(ctx, w, FIELDN(DP(o)->fn));
	}else if(SP(o)->tt == TT_MN) {
		knh_write_mn(ctx, w, TYPE_void, DP(o)->mn);
	}else if(IS_String(DP(o)->text)) {
		if(SP(o)->tt == TT_STR) {
			knh_putc(ctx, w, '"');
			knh_write_char(ctx, w, __tochar(DP(o)->text));
			knh_putc(ctx, w, '"');
		}
		else if(SP(o)->tt == TT_TSTR) {
			knh_putc(ctx, w, '\'');
			knh_write_char(ctx, w, __tochar(DP(o)->text));
			knh_putc(ctx, w, '\'');
		}
		else {
			knh_write_char(ctx, w, __tochar(DP(o)->text));
		}
	}else if(IS_NULL(DP(o)->data)) {
		knh_write_char(ctx, w, "null");
	}else {
		knh_format(ctx, w, METHODN__k, DP(o)->data, KNH_NULL);
	}
}

/* ------------------------------------------------------------------------ */
//## method void Token.%k(OutputStream w, String m);

static
void knh_Token__k(Ctx *ctx, knh_Token_t *o, knh_OutputStream_t *w, knh_String_t *m)
{
	KNH_ASSERT(IS_Token(o));
	if(knh_token_isNested(SP(o)->tt)) {
		int i;
		knh_tkc_t tc;
		knh_Token_tc(ctx, o, &tc);
		if(SP(o)->tt == TT_PARENTHESIS) {
			knh_write(ctx, w, STEXT("("));
		}
		else if(SP(o)->tt == TT_BRANCET) {
			knh_write(ctx, w, STEXT("["));
		}
		else {
			KNH_ASSERT(SP(o)->tt == TT_BRACE);
			knh_write(ctx, w, STEXT("{"));
		}
		for(i = 0; i < tc.e; i++) {
			knh_Token__k(ctx, tc.ts[i], w, m);
		}
		if(SP(o)->tt == TT_PARENTHESIS) {
			knh_write(ctx, w, STEXT("("));
		}
		else if(SP(o)->tt == TT_BRANCET) {
			knh_write(ctx, w, STEXT("["));
		}
		else {
			KNH_ASSERT(SP(o)->tt == TT_BRACE);
			knh_write(ctx, w, STEXT("{"));
		}
		return;
	}
	/* PREFIX */
	if(knh_Token_isTopDot(o)) {
		knh_putc(ctx, w, '.');
	}
	if(SP(o)->tt == TT_METAN) {
		knh_putc(ctx, w, '@');
	}
	else if(SP(o)->tt == TT_PROPN) {
		knh_putc(ctx, w, '$');
	}
	else if(SP(o)->tt == TT_MT) {
		knh_putc(ctx, w, '%');
	}
	else if(SP(o)->tt == TT_STR) {
		knh_putc(ctx, w, '"');
	}
	else if(SP(o)->tt == TT_TSTR) {
		knh_putc(ctx, w, '\'');
	}

	/* BODY */
	if(SP(o)->tt == TT_CID) {
		knh_write_char(ctx, w, CLASSN(DP(o)->cid));
	}else if(SP(o)->tt == TT_FN) {
		knh_write_char(ctx, w, FIELDN(DP(o)->fn));
	}else if(SP(o)->tt == TT_MN) {
		knh_write_mn(ctx, w, TYPE_void, DP(o)->mn);
	}else {
		knh_write_char(ctx, w, sToken(o));
	}
	/* POSTFIX */
	if(SP(o)->tt == TT_LABEL) {
		knh_putc(ctx, w, ':'); knh_putc(ctx, w, ' ');
	}
	else if(SP(o)->tt == TT_METAN) {
		knh_putc(ctx, w, ' ');
	}
	else if(SP(o)->tt == TT_STR) {
		knh_putc(ctx, w, '"');
	}
	else if(SP(o)->tt == TT_TSTR) {
		knh_putc(ctx, w, '\'');
	}
	if(knh_Token_isTailWildCard(o)) {
		knh_write(ctx, w, STEXT(".*"));
	}
	if(knh_Token_isExceptionType(o)) {
		knh_write(ctx, w, STEXT("!!"));
	}
	else if(knh_Token_isNotNullType(o)) {
		knh_write(ctx, w, STEXT("!"));
	}
}

/* ------------------------------------------------------------------------ */
//## method void Token.%dump(OutputStream w, String m);

static
void knh_Token__dump(Ctx *ctx, knh_Token_t *o, knh_OutputStream_t *w, knh_String_t *m)
{
	KNH_ASSERT(IS_Token(o));
	knh_write_BOL(ctx, w);
	knh_printf(ctx, w, "%s[%d]", knh_token_tochar(SP(o)->tt), (knh_intptr_t)SP(o)->line);
	knh_putc(ctx, w, ' ');
	knh_Token__s(ctx, o, w, m);
	knh_printf(ctx, w, " (>>%s) ", knh_token_tochar(DP(o)->tt_next));
	if(SP(o)->flag != 0)
		knh_write__flag(ctx, w, SP(o)->flag);
	knh_println(ctx, w, STEXT(""));
	if(knh_token_isNested(SP(o)->tt)) {
		int i;
		knh_tkc_t tc;
		knh_Token_tc(ctx, o, &tc);
		knh_write_begin(ctx, w, 0);
		for(i = 0; i < tc.e; i++) {
			knh_Token__dump(ctx, tc.ts[i], w, m);
		}
		knh_write_end(ctx, w, 0);
	}
}


/* ------------------------------------------------------------------------ */
//## method void Stmt.%s(OutputStream w, String m);

static
void knh_Stmt__s(Ctx *ctx, knh_Stmt_t *o, knh_OutputStream_t *w, knh_String_t *m)
{
	knh_intptr_t i;
	knh_putc(ctx, w, '(');
	if(SP(o)->stt != STT_OP && SP(o)->stt != STT_NEW && SP(o)->stt != STT_CALL ) {
		knh_write_char(ctx, w, knh_stmt_tochar(SP(o)->stt));
		if(DP(o)->size > 0) {
			knh_putc(ctx, w, ' ');
		}
	}
	for(i = 0; i < DP(o)->size; i++) {
		if(i > 0) knh_putc(ctx, w, ' ');
		if(IS_Token(DP(o)->terms[i])) {
			knh_Token__s(ctx, DP(o)->tokens[i], w, m);
		}else {
			KNH_ASSERT(IS_Stmt(DP(o)->terms[i]));
			knh_Stmt__s(ctx, DP(o)->stmts[i], w, m);
			if(IS_NOTNULL(DP(DP(o)->stmts[i])->next)) {
				knh_write_dots(ctx, w);
			}
		}
	}
	knh_putc(ctx, w, ')');
}

/* ------------------------------------------------------------------------ */

static
void knh_StmtMETA_dump(Ctx *ctx, knh_Stmt_t *o, knh_OutputStream_t *w, knh_String_t *m)
{
	if(!IS_DictMap(DP(o)->metaDictMap)) return ;
	int i = 0, size = (DP(o)->metaDictMap)->size;
	for(i = 0; i < size; i++) {
		knh_String_t *k = (knh_String_t*)knh_DictMap_keyAt(DP(o)->metaDictMap, i);
		knh_String_t *v = (knh_String_t*)knh_DictMap_valueAt(DP(o)->metaDictMap, i);
		if(k == v) {
			knh_printf(ctx, w, "@%s ", __tochar(k));
		}
		else {
			knh_printf(ctx, w, "@%s(%O) ", __tochar(k), v);
		}
	}
	if(size > 0) {
		knh_println(ctx, w, STEXT(""));
	}
}

/* ------------------------------------------------------------------------ */
//## method void Stmt.%dump(OutputStream w, String m)

static
void knh_Stmt__dump(Ctx *ctx, knh_Stmt_t *o, knh_OutputStream_t *w, knh_String_t *m)
{
	L_RESTART:;
	if(IS_DictMap(DP(o)->metaDictMap)) {
		knh_StmtMETA_dump(ctx, o, w, m);
	}
	knh_printf(ctx, w, "%s\t", knh_stmt_tochar(SP(o)->stt));
	knh_Stmt__s(ctx, o, w, m);
	if(IS_NOTNULL(DP(o)->next)) {
		knh_putc(ctx, w, '\n');
		o = DP(o)->next;
		goto L_RESTART;
	}
}


/* ------------------------------------------------------------------------ */

#endif/*KNH_CC_METHODAPI*/

#ifdef __cplusplus
}
#endif
