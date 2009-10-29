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
/* [tokens] */

static knh_Stmt_t *new_StmtSTMT1(Ctx *ctx, knh_tkc_t *tc, int isData);
static knh_Stmt_t *new_StmtDECL(Ctx *ctx, knh_Token_t *tkT, knh_tkc_t *tc);
static Term *new_TermEXPR(Ctx *ctx, knh_tkc_t *tc, int isData);
static knh_Stmt_t *new_StmtMETA(Ctx *ctx,  knh_tkc_t *tc, knh_stmt_t stt);
static void knh_Stmt_addMETA(Ctx *ctx, knh_Stmt_t *stmt, knh_tkc_t *tc);
static void knh_Stmt_add_STMT1(Ctx *ctx, knh_Stmt_t *stmt, knh_tkc_t *tc);
static void knh_Stmt_add_SEMICOLON(Ctx *ctx, knh_Stmt_t *stmt, knh_tkc_t *tc);

/* ======================================================================== */
/* [tokens] */

static int knh_tokens_count(knh_tkc_t *tc, knh_token_t tt)
{
	knh_Token_t **ts = tc->ts;
	int i, cnt = 0;
	for(i = tc->c; i < tc->e; i++) {
		if(TT_(ts[i]) == tt) cnt++;
	}
	return cnt;
}

static knh_tkc_t *knh_tokens_prev(knh_tkc_t *tc)
{
	tc->c -= 1;
	return tc;
}

static void knh_tokens_skipSEMICOLON(knh_tkc_t *tc)
{
	while(HAS_TOKEN(tc)) {
		if(TT_(TK0(tc)) != TT_SEMICOLON) break;
		tc->c += 1;
	}
}

static void knh_tokens_nextStmt(knh_tkc_t *tc)
{
	int i;
	knh_Token_t **ts = tc->ts;
	for(i = tc->c; i < tc->e; i++) {
		if(knh_Token_isBOL(ts[i])) {
			tc->c = i;
			knh_tokens_skipSEMICOLON(tc);
			return;
		}
	}
	tc->c = tc->e;
}

static
knh_tkc_t *knh_tokens_firstSTMT(Ctx *ctx, knh_tkc_t *tc, int pos, knh_tkc_t *buf, int isNeedSemicolon)
{
	int i;
	*buf = *tc;
	for(i = tc->c + 1 + pos; i < tc->e; i++) {
		if(knh_Token_isBOL(tc->ts[i])) {
			if(isNeedSemicolon && TT_(tc->ts[i]) != TT_SEMICOLON) {
				knh_Token_perror(ctx, tc->ts[i-1], KERR_BAD, "needs ;");
			}
			buf->e = i;
			tc->c = i;
			knh_tokens_skipSEMICOLON(tc);
			return buf;
		}
	}
	buf->e = tc->e;
	tc->c = tc->e;
	if(isNeedSemicolon && tc->e -1 >= 0) {
		knh_Token_perror(ctx, tc->ts[tc->e - 1], KERR_BAD, "needs ;");
	}
	return buf;
}

/* ------------------------------------------------------------------------ */

static
int knh_tokens_findTTIDX(knh_tkc_t *tc, knh_token_t tt, int def)
{
	int i;
	for(i = tc->c; i < tc->e; i++) {
		if(TT_(tc->ts[i]) == tt) return i;
	}
	return def;
}

static
knh_tkc_t *knh_tokens_firstEXPR(knh_tkc_t *tc, int idx, knh_tkc_t *sub)
{
	DBG2_ASSERT(idx != -1 && idx <= tc->e);
	int i;
	*sub = *tc;
	sub->meta = -1;
	for(i = tc->c; i < idx; i++) {
		knh_token_t tt = TT_(tc->ts[i]);
		if(tt == TT_METAN) {
			if(sub->meta == -1) sub->meta = i;
			tc->c = i + 1;
		}
		else {
			break;
		}
	}
	sub->e = idx;
	tc->c = idx + 1;
	tc->meta = -1;
	return sub;
}

/* ------------------------------------------------------------------------ */

static
void knh_tokens_ignoreLast(Ctx *ctx, knh_tkc_t *tc)
{
	if(HAS_TOKEN(tc)) {
		knh_Token_t *tkS = TK0(tc), *tkE = tc->ts[tc->e-1];
		if(tkS != tkE) {
			knh_Token_perror(ctx, tkS, KERR_DWARN, _("ignored %s .. %s"), sToken(tkS), sToken(tkE));
		}
		else {
			knh_Token_perror(ctx, tkS, KERR_DWARN, _("ignored %s"), sToken(tkS));
		}
		tc->c = tc->e;
	}
}

static knh_Token_t* knh_tokens_curToken(knh_tkc_t *tc)
{
	int c = tc->c;
	while(!(c < tc->e)) {
		if(c == 0) break;
		c--;
	}
	tc->c = c + 1;
	return tc->ts[c];
}

/* ------------------------------------------------------------------------ */

static
void knh_Stmt_tokens_perror(Ctx *ctx, knh_Stmt_t *o, knh_tkc_t *tc, char *fmt)
{
	knh_Token_t *tk = knh_tokens_curToken(tc);
	knh_Token_perror(ctx, tk, KERR_ERROR, fmt, sToken(tk));
	knh_Stmt_addT(ctx, o, tk);
	if(TT_(tk) == TT_ERR) {
		knh_tokens_nextStmt(tc);
		KNH_ASSERT(SP(o)->stt == STT_ERR);
	}
}

/* ------------------------------------------------------------------------ */

static void knh_Stmt_add_ASIS(Ctx *ctx, knh_Stmt_t *o)
{
	if(STT_(o) == STT_ERR) return;
	knh_Stmt_addT(ctx, o, new_TokenASIS(ctx, FL(o)));
}

/* ------------------------------------------------------------------------ */

typedef int (*knh_ftchk)(Ctx *ctx, knh_Token_t *tk);

static void knh_Stmt_tadd(Ctx *ctx, knh_Stmt_t *o, knh_tkc_t *tc, knh_ftchk f, char *emsg)
{
	if(SP(o)->stt == STT_ERR) return;
	if(HAS_TOKEN(tc) && f(ctx, TK0(tc))) {
		knh_Stmt_add(ctx, o, UP(TK0(tc)));
		tc->c += 1;
	}
	else {
		if(emsg == NULL) {
			knh_Stmt_add_ASIS(ctx, o);
		}
		else {
			knh_Stmt_tokens_perror(ctx, o, tc, emsg);
		}
	}
}

/* ======================================================================== */
/* [knh_Token_is] */

static int isPATH(Ctx *ctx, knh_Token_t *tk)
{
	if(TT_(tk) == TT_URN) return 1;
	return IS_bString(DP(tk)->text);
}

/* ------------------------------------------------------------------------ */

static int isTYPEN(Ctx *ctx, knh_Token_t *tk)
{
	return (TT_(tk) == TT_TYPEN);
}

/* ------------------------------------------------------------------------ */

static int isCLASSN(Ctx *ctx, knh_Token_t *tk)
{
	if(TT_(tk) == TT_TYPEN) {
		knh_bytes_t t = knh_Token_tobytes(ctx, tk);
		if(knh_bytes_index(t, '.') == -1) {
			return isupper(t.buf[0]);
		}
		return 1;
	}
	return 0;
}

/* ------------------------------------------------------------------------ */

static int isVARN(Ctx *ctx, knh_Token_t *tk)
{
	if(TT_(tk) == TT_NAME) {
		knh_bytes_t t = __tobytes(DP(tk)->text);
		size_t i;
		for(i = 0; i < t.len; i++) {
			if(t.buf[i] == '.' || t.buf[i] == ':') return 0;
		}
		return 1;
	}
	return 0;
}

/* ======================================================================== */
/* [COMMONS STMT] */

static
knh_Token_t *new_TokenSYMBOL(Ctx *ctx, Any *fln, knh_token_t tt, knh_String_t *t)
{
	knh_Token_t *o = (knh_Token_t*)new_Object_bcid(ctx, CLASS_Token, 0);
	SP(o)->flag = 0;
	TT_(o) = tt;
	knh_Token_setFL(o, fln);
	KNH_SETv(ctx, DP(o)->data, t);
	return o;
}

/* ------------------------------------------------------------------------ */

static
void knh_Stmt_add_SYMBOL(Ctx *ctx, knh_Stmt_t *o, knh_token_t tt, knh_String_t *ts)
{
	if(STT_(o) == STT_ERR) return;
	knh_Stmt_addT(ctx, o, new_TokenSYMBOL(ctx, FL(o), tt, ts));
}

/* ------------------------------------------------------------------------ */

static
knh_bool_t knh_Token_checkKeyword(Ctx *ctx, knh_Token_t *tk)
{
	if(knh_token_isKeyword(TT_(tk))) {
		knh_Token_perror(ctx, tk, KERR_ERROR, _("reserved keyword: %s"), sToken(tk));
		return 0;
	}
	return 1;
}

/* ------------------------------------------------------------------------ */

static
void knh_Stmt_add_DECL(Ctx *ctx, knh_Stmt_t *o, knh_Token_t *tkT, knh_tkc_t *tc)
{
	if(HAS_TOKEN(tc)) {
		if(tkT == NULL) {
			if(knh_Token_isTYPEN(TK0(tc))) {
				knh_Stmt_add(ctx, o, TM(TK0(tc)));
				tc->c += 1;
				knh_Token_checkKeyword(ctx, TK0(tc));
				knh_Stmt_tadd(ctx, o, tc, isVARN, _("not variable: %s"));
				goto L_INIT;
			}
		}
		knh_Token_checkKeyword(ctx, TK0(tc));
		if(isVARN(ctx, TK0(tc))) {
			if(tkT == NULL) {
				knh_Stmt_add_ASIS(ctx, o);
			}
			else {
				knh_Stmt_addT(ctx, o, tkT);
			}
			knh_Stmt_add(ctx, o, TM(TK0(tc)));
			tc->c += 1;
			goto L_INIT;
		}
	}
	knh_Stmt_tokens_perror(ctx, o, tc, _("not variable: %s"));
	return;

	L_INIT:;
	if(tc->c + 1 < tc->e && TT_(TK0(tc)) == TT_LET) {
		tc->c += 1;
		knh_Stmt_add(ctx, o, new_TermEXPR(ctx, tc, /*isData*/0));
	}
	else {
		//knh_tokens_ignoreLast(ctx, &tc);
		knh_Stmt_add_ASIS(ctx, o);
	}
}

/* ------------------------------------------------------------------------ */

static
void knh_Stmt_add_PARAMs(Ctx *ctx, knh_Stmt_t *o, knh_tkc_t *tc)
{
	knh_Stmt_t *stmtPARAMs = NULL;
	int hasERR = 0;
	while(HAS_TOKEN(tc)) {
		int idx = knh_tokens_findTTIDX(tc, TT_COMMA, tc->e);
		knh_tkc_t tcbuf, *expr = knh_tokens_firstEXPR(tc, idx, &tcbuf);
		if(IS_EMPTY(expr)) continue;
		knh_Token_t *tkT = expr->ts[expr->c];
		if(TT_(tkT) == TT_DOTS) {
			/* @TEST (T1 v1, T2 v2, ...) */
			if(stmtPARAMs != NULL) {
				knh_Stmt_setVARARGS(o, 1);
			}
			break;
		}
		if(expr->c + 1 == expr->e && TT_(tkT) == TT_TYPEN) {
			/* void */
			knh_Token_perror(ctx, tkT, KERR_DWARN, "no needs: %s", sToken(tkT));
			continue;
		}
		else {
			knh_Stmt_t *stmtPARAM = new_StmtMETA(ctx, expr, STT_DECL);
			knh_Stmt_add_DECL(ctx, stmtPARAM, NULL, expr);
			if(STT_(stmtPARAM) == STT_ERR) hasERR = 1;
			stmtPARAMs = knh_StmtNULL_tail_append(ctx, stmtPARAMs, stmtPARAM);
		}
	}
	if(stmtPARAMs == NULL) {
		stmtPARAMs = new_Stmt(ctx, 0, STT_DONE);
	}
	knh_Stmt_add(ctx, o, TM(stmtPARAMs));
	if(hasERR == 1) knh_Stmt_toERR(ctx, o, NULL);
}

/* ------------------------------------------------------------------------ */

static
void knh_Stmt_add_PPARAMs(Ctx *ctx, knh_Stmt_t *o, knh_tkc_t *tc)
{
	if(STT_(o) == STT_ERR) return;
	if(HAS_TOKEN(tc) && TT_(TK0(tc)) == TT_PARENTHESIS) {
		knh_tkc_t tcbuf, *params = knh_Token_tc(ctx, TK0(tc), &tcbuf);
		knh_Stmt_add_PARAMs(ctx, o, params);
		tc->c += 1;
	}
	else {
		knh_Stmt_tokens_perror(ctx, o, tc, _("needs ()"));
	}
}

/* ------------------------------------------------------------------------ */

static
int knh_Stmt_add_EXPRs(Ctx *ctx, knh_Stmt_t *stmt, knh_tkc_t *tc, int isData)
{
	int c = 0;
	do {
		int idx = knh_tokens_findTTIDX(tc, TT_COMMA, tc->e);
		knh_tkc_t tcbuf, *expr = knh_tokens_firstEXPR(tc, idx, &tcbuf);
		if(!(IS_EMPTY(expr))) {
			if(STT_(stmt) == STT_PRINT) {
				/* @TEST print a */
				if(expr->c + 1 == expr->e) {
					knh_Token_t *tkN = expr->ts[expr->c];
					if(TT_(tkN) == TT_NAME ||
							TT_(tkN) == TT_CONSTN ||
							TT_(tkN) == TT_CCONSTN) {
						tkN = new_TokenCONST(ctx, FL(tkN), DP(tkN)->data);
						knh_Token_setPNAME(tkN, 1);
						knh_Stmt_addT(ctx, stmt, tkN);
					}
				}
			}
			knh_Stmt_add(ctx, stmt, new_TermEXPR(ctx, expr, isData));
			c++;
		}
	}
	while(HAS_TOKEN(tc));
	return c;
}

/* ------------------------------------------------------------------------ */

static void knh_Stmt_add_STMT1(Ctx *ctx, knh_Stmt_t *stmt, knh_tkc_t *tc)
{
	if(STT_(stmt) == STT_ERR) return;
	if(HAS_TOKEN(tc) && TT_(TK0(tc)) == TT_BRACE) {
		knh_tkc_t tcbuf, *stmts = knh_Token_tc(ctx, TK0(tc), &tcbuf);
		knh_Stmt_add(ctx, stmt, TM(new_StmtSTMTBLOCK(ctx, stmts, 0/*isData*/)));
		tc->c += 1;
	}
	else {
		knh_Stmt_add(ctx, stmt, TM(new_StmtSTMT1(ctx, tc, 0/*isData*/)));
	}
}

/* ------------------------------------------------------------------------ */

static void knh_Stmt_add_SEMICOLON(Ctx *ctx, knh_Stmt_t *stmt, knh_tkc_t *tc)
{
	if(STT_(stmt) == STT_ERR) return;
	if(HAS_TOKEN(tc) && TT_(TK0(tc)) == TT_SEMICOLON) {
		tc->c += 1;
	}
	else {
		knh_perror(ctx, SP(stmt)->uri, SP(stmt)->line, KERR_BAD, _("needs ;"));
	}
}

/* ======================================================================== */
/* [VALUE] */

static knh_Stmt_t *new_StmtERR(Ctx *ctx, knh_tkc_t *tc)
{
	knh_Stmt_t *stmt = new_Stmt(ctx, 0, STT_DONE);
	knh_Stmt_toERR(ctx, stmt, TM(knh_tokens_curToken(tc)));
	knh_tokens_nextStmt(tc);
	return stmt;
}

/* ======================================================================== */
/* [VALUE] */

static knh_Stmt_t *new_StmtCALLSTMT(Ctx *ctx, knh_Stmt_t *o)
{
	knh_Stmt_t *stmt = new_Stmt(ctx, 0, STT_CALL);
	knh_Stmt_add(ctx, stmt, TM(o));
	return stmt;
}

/* ======================================================================== */
/* [NAME] */

static knh_Token_t *new_TokenTHIS(Ctx *ctx, knh_Token_t *tk)
{
	return new_TokenASIS(ctx, FL(tk));
}

/* ------------------------------------------------------------------------ */

static
knh_Token_t *new_TokenFUNCNAME(Ctx *ctx, knh_Token_t *tk, knh_bytes_t name, int isFunc)
{
	if(isFunc) {
		knh_methodn_t mn = knh_getmn(ctx, name, METHODN_NEWID);
		knh_Token_t *tkM = new_TokenMN(ctx, FL(tk), mn);
		knh_Token_setTopDot(tkM, 1);
		return tkM;
	}
	else {
		knh_fieldn_t fn = knh_getfnq(ctx, name, FIELDN_NEWID);
		knh_Token_t *tkF = new_TokenFN(ctx, FL(tk), fn);
		knh_Token_setGetter(tkF, 1);
		knh_Token_setTopDot(tkF, 1);
		return tkF;
	}
}

/* ------------------------------------------------------------------------ */

static
void knh_Stmt_add_FIRSTNAME(Ctx *ctx, knh_Stmt_t *stmt, knh_Token_t *tk, knh_bytes_t name)
{
	knh_bool_t isFunc = (TTn_(tk) == TT_PARENTHESIS) ? 1 : 0;
	DBG2_ASSERT(DP(stmt)->size == 0);
	while(1) {
		knh_index_t idx = knh_bytes_rindex(name, '.');
		knh_bytes_t fname = knh_bytes_first(name, idx);
		knh_bytes_t lname = knh_bytes_last(name,  idx+1);
		DBG2_ASSERT(idx != -1);
		knh_Stmt_addT(ctx, stmt, new_TokenFUNCNAME(ctx, tk, lname, isFunc));

		isFunc = 0;
		name = fname;

		if(isupper(name.buf[0])) {
			int i, has_ = 0, hasLOWER = 0, hasDOT = 0;
			for(i = 1; i < name.len; i++) {
				if(name.buf[i] == '_') {has_ = 0; continue; }
				if(islower(name.buf[i])) { hasLOWER = 1; continue; }
				if(name.buf[i] == '.') { hasDOT = 1; break; }
			}
			if(hasDOT == 0) {
				knh_token_t tt = TT_TYPEN;
				if(has_ == 1 || hasLOWER == 0) {
					tt = TT_CONSTN;
				}
				if(name.len == 1) tt = TT_TYPEN;
				knh_Stmt_add(ctx, stmt, TM(new_TokenSYMBOL(ctx, FL(tk), tt, new_StringSYMBOL(ctx, name))));
				return;
			}
		}
		else {
			int i, hasDOT = 0;
			for(i = 1; i < name.len; i++) {
				if(name.buf[i] == '.') { hasDOT = 1; break; }
			}
			if(hasDOT == 0) {
				knh_fieldn_t fn = knh_getfnq(ctx, name, FIELDN_NEWID);
				knh_Stmt_add(ctx, stmt, TM(new_TokenFN(ctx, FL(tk), fn)));
				return;
			}
		}
		/* push */ {
			knh_Stmt_t *stmtIN = new_Stmt(ctx, 0, STT_CALL);
			knh_Stmt_add(ctx, stmt, TM(stmtIN));
			stmt = stmtIN;
		}
	}
}

/* ------------------------------------------------------------------------ */

static
knh_Stmt_t* knh_Stmt_add_SECONDNAME(Ctx *ctx, knh_Stmt_t *stmt, knh_Token_t *tk)
{
	KNH_ASSERT(SP(tk)->tt == TT_NAME);
	KNH_ASSERT(DP(stmt)->size == 1);
	{
		knh_Stmt_t *stmt_head = stmt;
		knh_bytes_t name = knh_Token_tobytes(ctx, tk);
		knh_index_t idx = knh_bytes_index(name, '.');
		while(idx > 0) {
			knh_Stmt_add(ctx, stmt_head, TM(new_TokenFUNCNAME(ctx, tk, knh_bytes_first(name, idx), 0)));
			knh_Stmt_swap(ctx, stmt_head, 0, 1);
			stmt_head = new_StmtCALLSTMT(ctx, stmt_head);
			name = knh_bytes_last(name, idx+1);
			idx = knh_bytes_index(name, '.');
		}
		knh_Stmt_add(ctx, stmt_head, TM(new_TokenFUNCNAME(ctx, tk, name, TTn_(tk) == TT_PARENTHESIS)));
		knh_Stmt_swap(ctx, stmt_head, 0, 1);
		return stmt_head;
	}
}

/* ------------------------------------------------------------------------ */

static Term *new_TermNAME(Ctx *ctx, knh_Token_t *tk)
{
	KNH_ASSERT(TT_(tk) == TT_NAME);
	{
		knh_bytes_t t = knh_Token_tobytes(ctx, tk);
		if(knh_bytes_rindex(t, '.') == -1) {
			return TM(tk);
		}
		else {
			knh_Stmt_t* stmt = new_Stmt(ctx, 0, STT_CALL);
			knh_Stmt_add_FIRSTNAME(ctx, stmt, tk, t);
			return TM(stmt);
		}
	}
}

/* ------------------------------------------------------------------------ */

static
knh_Stmt_t *new_StmtNAME1(Ctx *ctx, knh_Token_t *tk)
{
	KNH_ASSERT(TT_(tk) == TT_NAME);
	{
		knh_bytes_t t = knh_Token_tobytes(ctx, tk);
		if(knh_bytes_rindex(t, '.') == -1) {
			knh_Stmt_t *stmt = new_Stmt(ctx, 0, STT_CALL);
			knh_Stmt_add(ctx, stmt, TM(tk));
			return stmt;
		}
		else {
			knh_Stmt_t* stmt = new_Stmt(ctx, 0, STT_CALL);
			knh_Stmt_add_FIRSTNAME(ctx, stmt, tk, t);
			return new_StmtCALLSTMT(ctx, stmt);
		}
	}
}

/* ------------------------------------------------------------------------ */

static
knh_Stmt_t *new_StmtNAME2(Ctx *ctx, knh_Token_t *tk)
{
	DBG2_ASSERT(TT_(tk) == TT_NAME || TT_(tk) == TT_CMETHODN);
	knh_bytes_t t = knh_Token_tobytes(ctx, tk);
	if(knh_bytes_rindex(t, '.') == -1) {
		knh_Stmt_t *stmt = new_Stmt(ctx, 0, STT_CALL);
		knh_Stmt_add(ctx, stmt, TM(tk));
		knh_Stmt_add(ctx, stmt, TM(new_TokenTHIS(ctx, tk)));
		return stmt;
	}
	else {
		knh_Stmt_t* stmt = new_Stmt(ctx, 0, STT_CALL);
		knh_Stmt_add_FIRSTNAME(ctx, stmt, tk, t);
		return stmt;
	}
}

/* ======================================================================== */
/* [Array, Tuple, Range] */

static knh_index_t knh_tokens_findRANGEIDX(knh_tkc_t *tc)
{
	knh_index_t i;
	for(i = tc->c; i < tc->e; i++) {
		knh_token_t tt = TT_(tc->ts[i]);
		if(tt == TT_TO || tt == TT_UNTIL) {
			return i;
		}
	}
	return -1;
}

/* ------------------------------------------------------------------------ */

static
void knh_Stmt_add_RANGE(Ctx *ctx, knh_Stmt_t *stmt, knh_tkc_t *tc, knh_index_t idx, int isData)
{
	knh_Token_t *tkM = NULL;
	if(DP(stmt)->size == 1) {
		tkM = new_TokenMN(ctx, FL(stmt), METHODN_get);
		knh_Stmt_addT(ctx, stmt, tkM);
		knh_Stmt_swap(ctx, stmt, 0, 1);
	}
	if(idx == -1) {
		int c = knh_tokens_count(tc, TT_COMMA);
		c = knh_Stmt_add_EXPRs(ctx, stmt, tc, /*isData*/0);
		if(c == 0) { /* @TEST a[] = 0 */
			DP(tkM)->mn = METHODN_setAll;
		}
		else if(c == 1) {
			DP(tkM)->mn = METHODN_get;
		}
		if(c == 2) {
			/* @TEST a[1,2] */
			DP(tkM)->mn = METHODN_get2D;
		}
		else if(c == 3) {
			/* @TEST a[1,2,3] */
			DP(tkM)->mn = METHODN_get3D;
		}
	}
	else {
		knh_tkc_t tcbuf, *expr = knh_tokens_firstEXPR(tc, idx, &tcbuf);
		if(tkM != NULL) {
			knh_methodn_t mn = (TT_(tc->ts[idx]) == TT_TO) ? METHODN_opRangeTo : METHODN_opRangeUntil;
			DP(tkM)->mn = mn;
		}
		if(IS_EMPTY(expr)) {
			if(tkM == NULL) {
				tc->c = idx;
				knh_Stmt_tokens_perror(ctx, stmt, tc, _("empty expression before '%s'"));
				return;
			}
			knh_Stmt_addT(ctx, stmt, new_TokenCONST(ctx, FL(tkM), KNH_NULL));
		}
		else {
			knh_Stmt_add(ctx, stmt, new_TermEXPR(ctx, expr, isData));
		}
		expr = knh_tokens_firstEXPR(tc, tc->e, &tcbuf);
		if(IS_EMPTY(expr)) {
			if(tkM == NULL) {
				tc->c = idx;
				knh_Stmt_tokens_perror(ctx, stmt, tc, _("empty expression after '%s'"));
				return;
			}
			knh_Stmt_addT(ctx, stmt, new_TokenCONST(ctx, FL(tkM), KNH_NULL));
		}
		else {
			knh_Stmt_add(ctx, stmt, new_TermEXPR(ctx, expr, isData));
		}
	}
}

/* ------------------------------------------------------------------------ */

static
Term *new_TermTUPLE(Ctx *ctx, knh_Token_t *tk, int isData)
{
	knh_tkc_t tcbuf, *tc = knh_Token_tc(ctx, tk, &tcbuf);
	int idx = knh_tokens_findRANGEIDX(tc);
	Term *tm = NULL;
	if(idx == -1) {
		int c = knh_tokens_count(tc, TT_COMMA);
		if(c == 0) {
			if(tc->e == 0) {
				/* @TEST () :=> null */
				return new_TermCONST(ctx, FL(tk), KNH_NULL);
			}
			/* @TEST (1) :=> 1*/
			return new_TermEXPR(ctx, tc, isData);
		}
		else {
			/* @TEST (1,2,3) :=> new Tuple(1,2,3) */
			knh_Stmt_t *stmt = new_Stmt(ctx, FLAG_Stmt_Literal, STT_NEW);
			knh_Token_t *tkC = new_TokenCID(ctx, FL(tk), CLASS_Tuple);
			knh_Stmt_addT(ctx, stmt, new_TokenMN(ctx, FL(tk), METHODN_new__init));
			knh_Stmt_addT(ctx, stmt, tkC);
			c = knh_Stmt_add_EXPRs(ctx, stmt, tc, isData);
			if(c == 2) { /* @TEST (1,2,) :=> new Pair(1,2) */
				KNH_ASSERT(DP(tkC)->cid == CLASS_Tuple);
				DP(tkC)->cid = CLASS_Pair;
			}
			tm = TM(stmt);
		}
	}
	else {
		/* @TEST (1 to 3) :=> new Range(1,3) */
		knh_Stmt_t *stmt = new_Stmt(ctx, FLAG_Stmt_Literal, STT_NEW);
		knh_methodn_t mn = (TT_(tc->ts[idx]) == TT_UNTIL) ? METHODN_new__init : METHODN_new;
		knh_Stmt_addT(ctx, stmt, new_TokenMN(ctx, FL(tk), mn));
		knh_Stmt_addT(ctx, stmt, new_TokenCID(ctx, FL(tk), CLASS_Range));
		knh_Stmt_add_RANGE(ctx, stmt, tc, idx, isData);
		tm = TM(stmt);
	}
	knh_Token_tokens_empty(ctx, tk);
	DBG2_ASSERT(tm != NULL);
	return tm;
}

/* ------------------------------------------------------------------------ */

static
Term *new_TermARRAY(Ctx *ctx, knh_Token_t *tk, int isData)
{
	knh_tkc_t tcbuf, *tc = knh_Token_tc(ctx, tk, &tcbuf);
	knh_Stmt_t *stmt = new_Stmt(ctx, FLAG_Stmt_Literal, STT_NEW);
	knh_index_t idx = knh_tokens_findRANGEIDX(tc);
	if(idx == -1) {
		/* @TEST [1,2, 3] :=> new Array(1,2,3) */
		knh_Stmt_addT(ctx, stmt, new_TokenMN(ctx, FL(tk), METHODN_new__init));
		knh_Stmt_addT(ctx, stmt, new_TokenCID(ctx, FL(tk), CLASS_Array));
		knh_Stmt_add_EXPRs(ctx, stmt, tc, isData);
	}
	else {
		/* @TEST [1 to 3] :=> new Int(1,2,3) */
		knh_Stmt_addT(ctx, stmt, new_TokenMN(ctx, FL(tk), METHODN_new__range));
		knh_Stmt_addT(ctx, stmt, new_TokenCID(ctx, FL(tk), CLASS_IArray));
		knh_Stmt_add_RANGE(ctx, stmt, tc, idx, isData);
		if(TT_(tc->ts[idx]) == TT_UNTIL) {
			knh_Stmt_addT(ctx, stmt, new_TokenCONST(ctx, FL(tk), UP(KNH_INT0)));
		}
	}
	knh_Token_tokens_empty(ctx, tk);
	return TM(stmt);
}

/* ======================================================================== */
/* [DictMap] */

static
int knh_tokens_findKEYIDX(Ctx *ctx, knh_tkc_t *tc)
{
	int i;
	knh_Token_t **ts = tc->ts;
	for(i = tc->c; i < tc->e - 1; i++) { /* BEGIN */
		if(TT_(ts[i]) == TT_LABEL) { /* key: */
			tc->c = i; 	i++;
			goto L_NEXT;
		}
		if(TT_(ts[i+1]) == TT_URN) {
			char *p = sToken(ts[i+1]);
			if(IS_String(DP(ts[i])->data) && p[0] == ':') {
				knh_Token_perror(ctx, ts[i+1], KERR_EWARN, _("ignored :"));
			}
		}
		if(TTn_(ts[i]) == TT_LET) { /* key = */
			if(IS_bString(DP(ts[i])->text)) {
				knh_Token_perror(ctx, ts[i+1], KERR_ERRATA, _("= ==> :"));
				TT_(ts[i+1]) = TT_COLON;
			}
		}
		if(TTn_(ts[i]) == TT_COLON) { /* "key" : */
			if(IS_bString(DP(ts[i])->text)) {
				TT_(ts[i+1]) = TT_LABEL;
				KNH_SETv(ctx, DP(ts[i+1])->data, DP(ts[i])->data);
				tc->c = i+1; i+=2;
				goto L_NEXT;
			}
			else {
				knh_Token_perror(ctx, ts[i+1], KERR_DWARN, _("ignored :"));
			}
		}
	}
	return -1;

	L_NEXT:;
	for(; i < tc->e; i++) { /* NEXT */
		if(TT_(ts[i]) == TT_LABEL) {
			return i;
		}
		if(TTn_(ts[i]) == TT_COLON) {
			if(IS_bString(DP(ts[i])->text)) {
				return i;
			}
			else {
				knh_Token_perror(ctx, ts[i+1], KERR_DWARN, _("ignored :"));
			}
		}
	}
	return tc->e;
}

/* ------------------------------------------------------------------------ */

static
void knh_Stmt_add_KEYVALUE(Ctx *ctx, knh_Stmt_t *stmt, knh_tkc_t *tc, int isData)
{
	knh_Token_t **ts = tc->ts;
	int c;
	DBG2_ASSERT(TT_(ts[tc->c]) == TT_LABEL);
	knh_Stmt_add(ctx, stmt, TM(knh_Token_toCONST(ctx, ts[tc->c])));
	tc->c += 1;
	if(tc->e - 1 > tc->c &&
		(TT_(ts[tc->e - 1]) == TT_COMMA || TT_(ts[tc->e - 1]) == TT_SEMICOLON)) {
		tc->e -= 1;
	}
	if(IS_EMPTY(tc)) {
		knh_Stmt_add(ctx, stmt, new_TermCONST(ctx, FL(ts[tc->c]), KNH_NULL));
		return;
	}
	c = knh_tokens_count(tc, TT_COMMA);
	if(c == 0) {
		knh_Stmt_add(ctx, stmt, new_TermEXPR(ctx, tc, isData));
	}
	else {
		knh_Stmt_t *stmtARRAY = new_Stmt(ctx, FLAG_Stmt_Literal, STT_NEW);
		knh_Stmt_addT(ctx, stmtARRAY, new_TokenMN(ctx, FL(ts[tc->c]), METHODN_new__init));
		knh_Stmt_addT(ctx, stmtARRAY, new_TokenCID(ctx, FL(ts[tc->c]), CLASS_Array));
		knh_Stmt_add_EXPRs(ctx, stmtARRAY, tc, isData);
		knh_Stmt_add(ctx, stmt, TM(stmtARRAY));
	}
}

/* ------------------------------------------------------------------------ */

static
void knh_Stmt_add_KEYVALUEs(Ctx *ctx, knh_Stmt_t *stmt, knh_tkc_t *tc, int isData)
{
	while(HAS_TOKEN(tc)) {
		int idx = knh_tokens_findKEYIDX(ctx, tc);
		if(idx == -1) break;
		knh_tkc_t tcbuf, *expr = knh_tokens_firstEXPR(tc, idx, &tcbuf);
		if(!IS_EMPTY(expr)) {
			knh_Stmt_add_KEYVALUE(ctx, stmt, expr, isData);
		}
		knh_tokens_prev(tc);
	}
}

static int knh_Token_isDictMap(Ctx *ctx, knh_Token_t *tk, int def)
{
	knh_tkc_t tcbuf, *tc = knh_Token_tc(ctx, tk, &tcbuf);
	int i, c = 0;
	for(i = 0; i < tc->e; i++) {
		if(TT_(tc->ts[i]) < TT_BRACE || TT_(tc->ts[i]) == TT_FUNCMAP) {
			return 0;
		}
		if(TT_(tc->ts[i]) == TT_LABEL || TT_(tc->ts[i]) == TT_COLON || TT_(tc->ts[i]) == TT_URN) {
			c++;
		}
	}
	if(c > 0) return 1;
	return def;
}

/* ======================================================================== */
/* [FUNCEXPR] */

static int isMETHODN(Ctx *ctx, knh_Token_t *tk)
{
	if(TT_(tk) == TT_NAME) {
		knh_bytes_t t = knh_Token_tobytes(ctx, tk);
		return (knh_bytes_index(t, '.') == -1);
	}
	return (TT_(tk) == TT_CMETHODN);
}

static int isMT(Ctx *ctx, knh_Token_t *tk)
{
	return (TT_(tk) == TT_MT);
}

static
knh_Stmt_t *new_StmtMETHOD(Ctx *ctx, knh_tkc_t *tc)
{
	knh_Stmt_t *o = new_StmtMETA(ctx, tc, STT_METHOD);
	if(HAS_TOKEN(tc) && TT_(TK0(tc)) == TT_FUNCTION) {
		knh_Stmt_add_ASIS(ctx, o); /* TYPE is var */
		knh_Stmt_add_ASIS(ctx, o); /* Class is _ */
		tc->c += 1;
		knh_Stmt_tadd(ctx, o, tc, isMETHODN, _("function name is needed: %s"));
		goto L_PARAMS;
	}
	else {
		knh_Stmt_tadd(ctx, o, tc, isTYPEN, _("invalid return type: %s"));
		if(HAS_TOKEN(tc)) {
			knh_Token_t *tkN = TK0(tc);
			if(TT_(tkN) == TT_PARENTHESIS) {
				knh_Stmt_add_ASIS(ctx, o); /* CLASS */
				knh_Stmt_addT(ctx, o, new_TokenMN(ctx, FL(tkN), METHODN_new));
				goto L_PARAMS;
			}
			if(isCLASSN(ctx, tkN) && TTn_(tkN) == TT_MT) {
				knh_Stmt_tadd(ctx, o, tc, isCLASSN, NULL); /* CLASS */
				knh_Stmt_tadd(ctx, o, tc, isMT, NULL); /* MT */
				goto L_PARAMS;
			}
			if(TT_(tkN) == TT_MT) {
				knh_Stmt_add_ASIS(ctx, o); /* CLASS */
				knh_Stmt_tadd(ctx, o, tc, isMT, NULL); /* MT */
				goto L_PARAMS;
			}
			if(TT_(tkN) == TT_CMETHODN) {
				knh_Stmt_add(ctx, o, TM(tkN)); /* CLASS */
				knh_Stmt_tadd(ctx, o, tc, isMETHODN, NULL);
				goto L_PARAMS;
			}
		}
		knh_Stmt_add_ASIS(ctx, o); /* CLASS */
		knh_Stmt_tadd(ctx, o, tc, isMETHODN, _("method name is needed: %s"));
	}

	L_PARAMS:;
	if(STT_(o) == STT_ERR) return o;

	knh_Stmt_add_PPARAMs(ctx, o, tc);
	if(HAS_TOKEN(tc) && TT_(TK0(tc)) == TT_BRACE) {
		knh_Stmt_add_STMT1(ctx, o, tc);
	}
	else {
		knh_Stmt_add_SEMICOLON(ctx, o, tc);
	}
	return o;
}

/* ------------------------------------------------------------------------ */

static
knh_Stmt_t *new_StmtFUNCTION(Ctx *ctx, knh_tkc_t *tc)
{
	/* @TEST function(a, b) {....} */
	knh_Stmt_t *o = new_StmtMETA(ctx, tc, STT_FUNCTION);
	if(TT_(TK0(tc)) == TT_FUNCTION) tc->c += 1;
	if(TT_(TK0(tc)) == TT_NAME) {
		knh_Token_perror(ctx, TK0(tc), KERR_ERRATA, _("%s => lambda"), sToken(TK0(tc)));
		tc->c += 1;
	}
	knh_Stmt_add_ASIS(ctx, o);  /* TYPEN */
	knh_Stmt_add_PPARAMs(ctx, o, tc);
	knh_Stmt_add_STMT1(ctx, o, tc);
	return o;
}

/* ======================================================================== */
/* [DECL] */

static
knh_Stmt_t *new_StmtDECL(Ctx *ctx, knh_Token_t *tkT, knh_tkc_t *tc)
{
	knh_Stmt_t *stmt = new_StmtMETA(ctx, tc, STT_DECL);
	int idx = knh_tokens_findTTIDX(tc, TT_COMMA, tc->e);
	knh_tkc_t tcbuf, *expr = knh_tokens_firstEXPR(tc, idx, &tcbuf);
	knh_Stmt_add_DECL(ctx, stmt, tkT, expr);
	if(HAS_TOKEN(tc)) {
		knh_Stmt_t *newstmt = new_StmtDECL(ctx, tkT, tc);
		KNH_SETv(ctx, DP(newstmt)->metaDictMap, DP(stmt)->metaDictMap);
		KNH_SETv(ctx, DP(stmt)->next, newstmt);
	}
	return stmt;
}

/* ------------------------------------------------------------------------ */

static
Term *new_TermBLOCK(Ctx *ctx, knh_Token_t *tkC, knh_Token_t *tk, int isData)
{
	knh_tkc_t tcbuf, *tc = knh_Token_tc(ctx, tk, &tcbuf);
	if(knh_Token_isDictMap(ctx, tk, 1)) {
		/* @TEST {} :=> DictMap */
		knh_Stmt_t *stmt = new_Stmt(ctx, FLAG_Stmt_Literal, STT_NEW);
		knh_Stmt_addT(ctx, stmt, new_TokenMN(ctx, FL(tk), METHODN_new__dictmap));
		if(tkC == NULL) {
			tkC = new_TokenCID(ctx, FL(tk), CLASS_DictMap);
		}
		knh_Stmt_add(ctx, stmt, TM(tkC));
		knh_Stmt_add_KEYVALUEs(ctx, stmt, tc, 1/*isData*/);
		return TM(stmt);
	}
	else {
		/* @TEST {a => a} :=> Closure */
		knh_Stmt_t *o = new_StmtMETA(ctx, tc, STT_FUNCTION);
		knh_tkc_t tcbuf, *tc = knh_Token_tc(ctx, tk, &tcbuf);
		int idx = knh_tokens_findTTIDX(tc, TT_FUNCMAP, -1);
		if(tkC == NULL) {
			knh_Stmt_add_ASIS(ctx, o);
		}
		else {
			knh_Stmt_add(ctx, o, TM(tkC));
		}
		if(idx != -1) {
			knh_tkc_t tcbuf2, *params = knh_tokens_firstEXPR(tc, idx, &tcbuf2);
			knh_Stmt_add_PARAMs(ctx, o, params);
		}
		else {
			knh_Stmt_add(ctx, o, TM(new_Stmt(ctx, 0, STT_DONE)));
		}
		knh_Stmt_add(ctx, o, TM(new_StmtSTMTBLOCK(ctx, tc, isData)));
		return TM(o);
	}
}

/* ======================================================================== */
/* [PROPN] */

static
Term* new_TermPROPN(Ctx *ctx, knh_Token_t *tk, int isData)
{
	DBG2_ASSERT(IS_String(DP(tk)->text));
	knh_bytes_t t = __tobytes(DP(tk)->text);
	if(isData) {
		Object *o = knh_getProperty(ctx, t);
		knh_Token_setCONST(ctx, tk, o);
		return TM(tk);
	}
	else {
		knh_Stmt_t *stmt = new_Stmt(ctx, 0, STT_CALL);
		knh_methodn_t mn = (knh_bytes_index(t, '*') > 0) ?
				METHODN_listProperties : METHODN_getProperty;
		knh_Stmt_addT(ctx, stmt, new_TokenMN(ctx, FL(tk), mn));
		knh_Stmt_addT(ctx, stmt, new_TokenCID(ctx, FL(tk), CLASS_Context));
		knh_Token_toCONST(ctx, tk);
		knh_Stmt_addT(ctx, stmt, tk);
		if(mn != METHODN_listProperties) {
			knh_Stmt_setPROPN(stmt, 1);
		}
		return TM(stmt);
	}
}

/* ------------------------------------------------------------------------ */

knh_String_t* knh_Stmt_getPropertyNameNULL(Ctx *ctx, knh_Stmt_t *stmt)
{
	knh_Token_t *tkPROPN = DP(stmt)->tokens[2];
	if(IS_Token(tkPROPN) && TT_(tkPROPN) == TT_CONST) {
		DBG2_ASSERT(IS_String(DP(tkPROPN)->text));
		return DP(tkPROPN)->text;
	}
	return NULL;
}

/* ======================================================================== */
/* [VALUE] */

static
Term *new_TermVALUE(Ctx *ctx, knh_Token_t *tk, int isData)
{
	switch(TT_(tk)) {
		case TT_NAME:
		{
			knh_bytes_t t = knh_Token_tobytes(ctx, tk);
			if(ISB(t, "null")) {
				knh_Token_setCONST(ctx, tk, KNH_NULL);
				return TM(tk);
			}
			else if(ISB(t, "true")) {
				knh_Token_setCONST(ctx, tk, KNH_TRUE);
				return TM(tk);
			}
			else if(ISB(t, "false")) {
				knh_Token_setCONST(ctx, tk, KNH_FALSE);
				return TM(tk);
			}
			if(isData) {
				DBG2_P("TO CONST name=%s", sToken(tk));
				//DBG2_ASSERT(isData == 0);
				knh_Token_toCONST(ctx, tk);
				return TM(tk);
			}
			else {
				return new_TermNAME(ctx, tk);
			}
		}
		case TT_PARENTHESIS:
			return new_TermTUPLE(ctx, tk, isData);
		case TT_BRANCET:
			return new_TermARRAY(ctx, tk, isData);
		case TT_BRACE:
			return new_TermBLOCK(ctx, NULL, tk, isData);
		case TT_PROPN:
			return new_TermPROPN(ctx, tk, isData);
		case TT_TYPEN:
		case TT_CONSTN:
		case TT_CCONSTN:
		case TT_CMETHODN:
			if(isData) {
				knh_Token_toCONST(ctx, tk);
				return TM(tk);
			}
		case TT_ESTR:
		case TT_STR:
		case TT_NUM:
		case TT_TSTR:
		case TT_URN:
		case TT_ERR:
			return TM(tk);
	}
	if(!isData) {
		knh_Token_perror(ctx, tk, KERR_ERROR, _("unknown token"), sToken(tk));
	}
	return TM(tk);
}

/* ======================================================================== */
/* [LET] */

static int knh_tokens_findLETIDX(knh_tkc_t *tc)
{
	int i;
	knh_Token_t **ts = tc->ts;
	for(i = tc->c; i < tc->e; i++) {
		if(TT_LET <= TT_(ts[i]) && TT_(ts[i]) <= TT_ALTLET) return i;
	}
	return -1;
}

/* ------------------------------------------------------------------------ */

static
int knh_Term_checkLVALUE(Ctx *ctx, Term *tm)
{
	if(IS_Token(tm)) {
		knh_Token_t *tkL = (knh_Token_t*)tm;
		if(knh_Token_checkKeyword(ctx, tkL)) {
			if(isVARN(ctx, tkL) || TT_(tkL) == TT_CONSTN || TT_(tkL) == TT_CCONSTN) {
				return 1;
			}
			knh_Token_perror(ctx, tkL, KERR_ERROR, _("not l-value"));
		}
		return 0;
	}
	else {
		knh_Stmt_t *stmt = (knh_Stmt_t*)tm;
		if(STT_(stmt) == STT_CALL) {
			knh_Token_t *tk = (knh_Token_t*)DP(stmt)->tokens[0];
			DBG2_ASSERT(IS_Token(tk));
			if(knh_Token_isGetter(tk)) {
				knh_Token_setGetter(tk, 0);
				knh_Token_setSetter(tk, 1);
				return 1;
			}
			else if(TT_(tk) == TT_MN) {
				knh_methodn_t mn = DP(tk)->mn;
//				if(METHODN_IS_SETTER(mn)) {
//					return 1;
//				}
//				else
				if(METHODN_IS_GETTER(mn)) {
					mn = METHODN_TOFIELDN(mn);
					DP(tk)->mn = METHODN_TO_SETTER(mn);
					return 1;
				}
			}
			else if(TT_(tk) == TT_FN) {
				TODO();
			}
			else {
				TODO();
			}
		}
		knh_perror(ctx, SP(stmt)->uri, SP(stmt)->line, KERR_ERROR, _("not l-value"));
		DBG2_P("stt=%s", cSTT_((stmt)));
		STT_(stmt) = STT_ERR;

	}
	return 0;
}

static
knh_Stmt_t *new_StmtLET(Ctx *ctx, knh_tkc_t *expr, int idx, int isData)
{
	knh_tkc_t tcbuf, *lexpr = knh_tokens_firstEXPR(expr, idx, &tcbuf);
	Term *ltm = new_TermEXPR(ctx, lexpr, isData);
	knh_Stmt_t *stmt;
	if(TT_(expr->ts[idx]) != TT_LET) {
		TT_(expr->ts[idx]) = TT_(expr->ts[idx]) - (TT_ADDE - TT_ADD);
		expr->c = lexpr->c;
	}
	if(IS_Token(ltm)) {
		stmt = new_Stmt(ctx, 0, STT_LET);
		knh_Stmt_add(ctx, stmt, ltm);
		knh_Stmt_add(ctx, stmt, new_TermEXPR(ctx, expr, 0/*isData*/));
	}
	else {
		stmt = (knh_Stmt_t*)ltm;
		DBG2_ASSERT(IS_Stmt(stmt));
		knh_Stmt_add(ctx, stmt, new_TermEXPR(ctx, expr, 0/*isData*/));
	}
	if(!knh_Term_checkLVALUE(ctx, ltm)) {
		knh_Stmt_toERR(ctx, stmt, ltm);
	}
	return stmt;
}

static
knh_Stmt_t* new_StmtLETEXPR(Ctx *ctx, knh_tkc_t *tc, int isData)
{
	knh_tkc_t tcbuf, *expr = knh_tokens_firstSTMT(ctx, tc, /*pos*/0, &tcbuf, 0/*needs;*/);
	int idx = knh_tokens_findLETIDX(expr);
	if(idx > 0) {
		knh_tkc_t tcbuf2, *lexpr = knh_tokens_firstEXPR(expr, idx, &tcbuf2);
		int lc = knh_tokens_count(lexpr, TT_COMMA);
		int rc = knh_tokens_count(expr, TT_COMMA);
		if((lc > 0 || rc > 0) && TT_(expr->ts[idx]) != TT_LET) {
			knh_Token_perror(ctx, expr->ts[idx],
				KERR_ERRATA, _("%s => ="), sToken(expr->ts[idx]));
		}
		if(lc > 0) {
			knh_Stmt_t *stmt;
			if(rc == 0) {
				/* a, b = s */
				stmt = new_Stmt(ctx, 0, STT_SEPARATOR);
				knh_Stmt_add_PARAMs(ctx, stmt, lexpr);
				knh_Stmt_add(ctx, stmt, new_TermEXPR(ctx, expr, 0/*isData*/));
			}
			else {
				/* a, b = 1, 2 */
				stmt = new_Stmt(ctx, 0, STT_LETM);
				while(1) {
					int lidx = knh_tokens_findTTIDX(lexpr, TT_COMMA, lexpr->e);
					int ridx = knh_tokens_findTTIDX(expr, TT_COMMA, expr->e);
					knh_tkc_t ltcbuf, *ltc = knh_tokens_firstEXPR(lexpr, lidx, &ltcbuf);
					knh_tkc_t rtcbuf, *rtc = knh_tokens_firstEXPR(expr, ridx, &rtcbuf);
					if(IS_EMPTY(ltc) || IS_EMPTY(rtc)) break;
					{
						Term *ltm = new_TermEXPR(ctx, ltc, isData);
						knh_Stmt_add(ctx, stmt, ltm);
						knh_Stmt_add(ctx, stmt, new_TermEXPR(ctx, rtc, isData));
						if(!knh_Term_checkLVALUE(ctx, ltm)) {
							knh_Stmt_toERR(ctx, stmt, ltm);
							break;
						}
					}
				}
			}
			return stmt;
		}
		expr->c = lexpr->c;
	}
	if(IS_EMPTY(expr)) {
		return new_Stmt(ctx, 0, STT_DONE);
	}
	else {
		Term *term = new_TermEXPR(ctx, expr, isData);
		if(IS_Token(term)) {
			knh_Stmt_t *stmt = new_Stmt(ctx, 0, STT_CALL1);
			knh_Stmt_add(ctx, stmt, term);
			return stmt;
		}
		return (knh_Stmt_t*)term;
	}
}

/* ======================================================================== */
/* [EXPR] */

static
Term *new_TermPEXPR(Ctx *ctx, knh_Token_t *tk, int isData)
{
	knh_tkc_t tcbuf, *tc = knh_Token_tc(ctx, tk, &tcbuf);
	if(IS_EMPTY(tc)) {
		if(!isData) {
			knh_Token_perror(ctx, tk, KERR_ERRATA, _("empty: () ==> false"));
		}
		return TM(new_TokenCONST(ctx, FL(tk), KNH_FALSE));
	}
	return new_TermEXPR(ctx, tc, isData);
}

static
void knh_Stmt_add_PEXPR(Ctx *ctx, knh_Stmt_t *stmt, knh_tkc_t *tc)
{
	if(STT_(stmt) == STT_ERR) return;
	if(HAS_TOKEN(tc)) {
		if(TT_(TK0(tc)) == TT_WHILE) {
			tc->c += 1;  /* @ADHOC do {} while() */
		}
	}
	if(HAS_TOKEN(tc)) {
		if(TT_(TK0(tc)) == TT_PARENTHESIS) {
			knh_Stmt_add(ctx, stmt, new_TermPEXPR(ctx, TK0(tc), 0/*isData*/));
			tc->c += 1;
			return ;
		}
	}
	knh_Stmt_tokens_perror(ctx, stmt, tc, _("needs ()"));
}

/* ======================================================================== */
/* [OP] */

static int knh_tokens_findOPRIDX(Ctx *ctx, knh_tkc_t *tc, int isData)
{
	knh_Token_t **ts = tc->ts;
	int i, oppri = 98, idx = -1;
	knh_token_t tt_found = TT_EOT;
	if(TT_(ts[tc->c]) == TT_ADD) tc->c += 1;
	if(TT_(ts[tc->c]) == TT_SUB) TT_(ts[tc->c]) = TT_NEG;

	for(i = tc->c; i < tc->e; i++) {
		int p = knh_token_getOpPriority(TT_(ts[i]));
		if(p == 0) {  /* token, disallowd in expression */
			if(!isData) {
				knh_Token_perror(ctx, ts[i], KERR_ERROR, _("not in expression: %s"), sToken(ts[i]));
			}
			tc->e = i;
			return i;
		}
		/* p <= oppri is binary*/
		if(p <= oppri) {
			oppri = p;
			tt_found = TT_(ts[i]);
			//idx = i;
		}
	}
	if(tt_found != TT_EOT) {
		if(knh_token_getOpSize(tt_found) == 2) {
			for(i = tc->c; i < tc->e; i++) {
				if(TT_(ts[i]) == tt_found) idx = i; /* last one */
			}
		}
		else {
			for(i = tc->c; i < tc->e; i++) {
				if(TT_(ts[i]) == tt_found) {
					idx = i;
					break; /* first one */
				}
			}
		}
	}
	return idx;
}

/* ------------------------------------------------------------------------ */

static
knh_Stmt_t *new_StmtNEXT(Ctx *ctx, knh_Token_t *op, knh_tkc_t *tc)
{
	knh_Stmt_t *stmt = new_Stmt(ctx, 0, STT_CALL);
	knh_methodn_t mn = (TT_(op) == TT_NEXT) ? METHODN_opNext : METHODN_opPrev;
	knh_Stmt_addT(ctx, stmt, new_TokenMN(ctx, FL(op), mn));
	knh_Stmt_add(ctx, stmt, new_TermEXPR(ctx, tc, 0/*isData*/));
	return stmt;
}

/* ------------------------------------------------------------------------ */

static
Term *new_TermOPR(Ctx *ctx, knh_tkc_t *tc, int idx)
{
	knh_Token_t *op = tc->ts[idx];
	if(TT_(op) == TT_QUESTION) {
		/* @TEST expr ? $expr : $expr */
		knh_Stmt_t *stmt = new_Stmt(ctx, 0, STT_TRI);
		knh_tkc_t tcbuf, *expr = knh_tokens_firstEXPR(tc, idx, &tcbuf);
		knh_Stmt_add(ctx, stmt, new_TermEXPR(ctx, expr, 0/*isData*/));
		idx = knh_tokens_findTTIDX(tc, TT_COLON, -1);
		if(idx == -1) {
			knh_Stmt_tokens_perror(ctx, stmt, expr, _("trinary ? needs :"));
		}
		else {
			expr = knh_tokens_firstEXPR(tc, idx, &tcbuf);
			knh_Stmt_add(ctx, stmt, new_TermEXPR(ctx, expr, 0/*isData*/));
			knh_Stmt_add(ctx, stmt, new_TermEXPR(ctx, tc, 0/*isData*/));
		}
		return TM(stmt);
	}
	else if(TT_(op) == TT_NEXT || TT_(op) == TT_PREV) {
		int isPrePosition = 0;
		knh_Stmt_t *stmt;
		if(TK0(tc) == op) {
			isPrePosition = 1;
			tc->c += 1;
		}
		else {
			tc->e = idx;
		}
		Term *lval = new_TermEXPR(ctx, tc, 0/*isData*/);
		if(IS_Token(lval)) {
			stmt = new_Stmt(ctx, 0, STT_LET);
			knh_Stmt_add(ctx, stmt, lval);
			knh_Stmt_add(ctx, stmt, TM(new_StmtNEXT(ctx, op, tc)));
		}
		else {
			stmt = (knh_Stmt_t*)lval;
			knh_Stmt_add(ctx, stmt, TM(new_StmtNEXT(ctx, op, tc)));
		}
		if(!knh_Term_checkLVALUE(ctx, lval)) {
			knh_Stmt_toERR(ctx, stmt, lval);
		}
		return TM(stmt);
	}
	else {
		knh_Stmt_t *stmtOPR;
		knh_stmt_t stt = STT_OP;
		switch(TT_(op)) {
			case TT_AND: stt = STT_AND; break;
			case TT_OR:  stt = STT_OR; break;
			case TT_ALT: stt = STT_ALT; break;
		}
		stmtOPR = new_Stmt(ctx, 0, stt);
		if(stt == STT_OP) {
			knh_Stmt_add(ctx, stmtOPR, TM(op));
		}
		if(TT_(TK0(tc)) == TT_(op)) {
			/* @TEST -1 PrePosition */
			tc->c += 1;
			idx = knh_tokens_findTTIDX(tc, TT_(op), tc->e);
		}
		while(HAS_TOKEN(tc)) {
			knh_tkc_t tcbuf, *expr = knh_tokens_firstEXPR(tc, idx, &tcbuf);
			knh_Stmt_add(ctx, stmtOPR, new_TermEXPR(ctx, expr, 0/*isData*/));
			idx = knh_tokens_findTTIDX(tc, TT_(op), tc->e);
		}
		return TM(stmtOPR);
	}
}

/* ======================================================================== */
/* [CAST] */

static knh_Token_t *knh_Token_findCASTNULL(Ctx *ctx, knh_Token_t *tk, knh_Token_t *ntk, int isData)
{
	if(TT_(tk) == TT_PARENTHESIS && !knh_Token_isTopDot(ntk)) {
		knh_tkc_t tcbuf, *tc = knh_Token_tc(ctx, tk, &tcbuf);
		if(tc->e == 0) return NULL;
		if((tc->e > 2 && (tc->ts[1])->tt != TT_WITH)  || tc->e == 2) {
			if(!isData) {
				knh_Token_perror(ctx, tc->ts[1], KERR_ERROR, _("cast??"));
			}
			TT_(tk) = TT_ERR;
			return NULL;
		}
		if(TT_(tc->ts[0]) == TT_MUL) {
			TT_(tc->ts[0]) = TT_CID;
			DP(tc->ts[0])->cid = CLASS_Any;
			return tk; //tc->ts[0];
		}
		if(isCLASSN(ctx, tc->ts[0])) {
			return tk; //tc->ts[0];
		}
	}
	return NULL;
}

/* ------------------------------------------------------------------------ */

static
Term *new_TermCAST(Ctx *ctx, knh_Token_t *tk, knh_tkc_t *tc, int isData)
{
	knh_Stmt_t *stmt = new_Stmt(ctx, 0, STT_MAPCAST);
	DBG2_ASSERT(TT_(tk) == TT_PARENTHESIS);
	knh_tkc_t tcbuf, *cast = knh_Token_tc(ctx, tk, &tcbuf);
	knh_Stmt_add(ctx, stmt, TM(cast->ts[0]));
	knh_Stmt_add(ctx, stmt, new_TermEXPR(ctx, tc, isData));
	if(cast->e > 2) {
		DBG2_ASSERT(TT_(cast->ts[1]) == TT_WITH);
		cast->c = 2;
		knh_Stmt_add(ctx, stmt, new_TermEXPR(ctx, cast, isData));
	}
	return TM(stmt);
}

/* ------------------------------------------------------------------------ */

static
int knh_Token_isNEW(Ctx *ctx, knh_Token_t *tk)
{
	if(TT_(tk) == TT_NAME) {
		char *p = sToken(tk);
		return (p[0]=='n' && p[1] == 'e' && p[2] == 'w' && (p[3]==0 || p[3]==':'));
	}
	return 0;
}

/* ------------------------------------------------------------------------ */

static Term *new_TermEXPR(Ctx *ctx, knh_tkc_t *tc, int isData)
{
	knh_Token_t **ts = tc->ts;
	int oc = tc->c, e = tc->e;
	int fc = 0, pc = 0;

	knh_Stmt_t *stmt = NULL;
	if(!(oc < e)) {
		knh_Token_t *tke = new_Token(ctx, 0, SP(tc->ts[e-1])->uri, SP(tc->ts[e-1])->line, TT_ERR);
		if(!isData) {
			knh_perror(ctx, SP(tc->ts[e-1])->uri, SP(tc->ts[e-1])->line, KERR_ERROR, _("syntax error"));
		}
		DBG2_P("empty??: tc->c=%d, tc->e=%d", (int)oc, (int)e);
		return TM(tke);
	}

	if (oc + 1 == e) {
		return new_TermVALUE(ctx, ts[oc], isData);
	}

	if(TT_(ts[oc]) == TT_SUB) {
		TT_(ts[oc]) = TT_NEG;
	}

	for(pc = oc; pc < e; pc++) {
		if(TT_(ts[pc]) == TT_ERR) {
			knh_tokens_ignoreLast(ctx, tc);
			tc->c = tc->e;
			return TM(ts[pc]);
		}
	}

	pc = 0;
	{
		knh_token_t tt0 = TT_(ts[oc]), tt1 = TTn_(ts[oc]);
		/* @TEST a.f "text" + 1 */
		if(tt0 == TT_NAME || tt0 == TT_CMETHODN || tt0 == TT_CONSTN || tt0 == TT_TYPEN) {
			if(tt1 == TT_STR || tt1 == TT_TSTR || tt1 == TT_ESTR) {
				TTn_(ts[oc]) = TT_PARENTHESIS; // needed
				if(tt0 == TT_CONSTN || tt0 == TT_TYPEN) TT_(ts[oc]) = TT_NAME;
				stmt = new_StmtNAME2(ctx, ts[oc]);
				tc->c = oc + 1;
				knh_Stmt_add(ctx, stmt, new_TermEXPR(ctx, tc, isData));
				return TM(stmt);
			}
		}
		/* @TEXT (a).f "text" + 1 */
		if(tt0 == TT_PARENTHESIS
				&& (tt1 == TT_NAME || tt1 == TT_CONSTN || tt1 == TT_TYPEN)
				&& knh_Token_isTopDot(ts[oc+1])) {
			knh_token_t tt2 = TTn_(ts[oc+1]);
			if(tt2 == TT_STR || tt2 == TT_TSTR || tt2 == TT_TSTR) {
				TTn_(ts[oc]) = TT_PARENTHESIS;
				stmt = new_Stmt(ctx, 0, STT_CALL);
				if(tt1 == TT_CONSTN || tt1 == TT_TYPEN) TT_(ts[oc+1]) = TT_NAME;
				knh_Stmt_add(ctx, stmt, new_TermVALUE(ctx, ts[oc], isData));
				stmt = knh_Stmt_add_SECONDNAME(ctx, stmt, ts[oc+1]);
				tc->c = oc + 2;
				knh_Stmt_add(ctx, stmt, new_TermEXPR(ctx, tc, isData));
				return TM(stmt);
			}
		}
	}

	/* LET */ {
		int idx = knh_tokens_findLETIDX(tc);
		if(idx != -1) {
			return TM(new_StmtLET(ctx, tc, idx, isData));
		}
	}

	/* OPERATOR */ {
		int idx = knh_tokens_findOPRIDX(ctx, tc, isData);
		if(idx != -1) {
			if(TT_(ts[idx]) == TT_ERR) return TM(ts[idx]);
			return new_TermOPR(ctx, tc, idx);
		}
	}

	/* cast operator (C)a */ {
		knh_Token_t *cast_op = knh_Token_findCASTNULL(ctx, ts[oc], ts[oc+1], isData);
		if(cast_op != NULL) {
			tc->c += 1;
			return TM(new_TermCAST(ctx, cast_op, tc, isData));
		}
	}

	if(knh_Token_isNEW(ctx, ts[oc])) {  /* @TEST new:gc Person("naruto") */
		if(TTn_(ts[oc]) == TT_TYPEN) {
			if(TTn_(ts[oc+1]) == TT_PARENTHESIS) {
				stmt = new_Stmt(ctx, 0, STT_NEW);
				knh_Stmt_add(ctx, stmt, TM(ts[oc]));
				knh_Stmt_add(ctx, stmt, TM(ts[oc+1]));
				pc = oc + 2;
				goto L_PARAM;
			}
			else if(TTn_(ts[oc+1]) == TT_BRANCET) {   /* @TEST new Int[10] */
				stmt = new_Stmt(ctx, 0, STT_NEW);
				knh_tkc_t cma_tc;
				knh_Token_tc(ctx, ts[oc+2], &cma_tc);
				knh_Stmt_add(ctx, stmt, TM(new_TokenMN(ctx, FL(ts[oc]), METHODN_new__array)));
				knh_Stmt_add(ctx, stmt, TM(ts[oc+1]));
				{
					knh_Token_t *tk0 = ts[oc+1];
					knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
					knh_Bytes_write(ctx, cwb->ba, __tobytes(DP(tk0)->text));
					knh_Bytes_write(ctx, cwb->ba, STEXT("[]"));
					KNH_SETv(ctx, DP(tk0)->data, knh_cwb_newString(ctx, cwb));
				}
				pc = oc + 2;
				KNH_ASSERT(TT_(ts[pc]) == TT_BRANCET);
				TT_(ts[pc]) = TT_PARENTHESIS;
				goto L_PARAM;
			}
			else {
				knh_Token_t *tke = ts[oc];
				knh_Token_perror(ctx, tke, KERR_ERROR, _("needs Class"));
				TT_(tke) = TT_ERR;
				tc->c = oc + 2; knh_tokens_nextStmt(tc);
				return TM(tke);
			}
		}
		if(TTn_(ts[oc]) == TT_PARENTHESIS) {
			stmt = new_Stmt(ctx, 0, STT_NEW);
			knh_Stmt_add(ctx, stmt, TM(ts[oc]));
			knh_Stmt_add_ASIS(ctx, stmt);
			pc = oc + 1;
			goto L_PARAM;
		}
	}

	/* function */
	if(TT_(ts[oc]) == TT_FUNCTION) {
		tc->c = oc + 1;
		stmt = new_StmtFUNCTION(ctx, tc);
		fc = tc->c;
		goto L_NEXTFUNC;
	}

	/* Closure Class (a, b) { ... } */
	if(knh_Token_isTYPEN(ts[oc])) {
		if(TTn_(ts[oc]) == TT_PARENTHESIS) {
			if(TTn_(ts[fc]) == TT_BRACE) {  /* @TEST Class (a,b) { ... } */
				tc->c = oc;
				stmt = new_Stmt(ctx, 0, STT_FUNCTION);
				knh_Stmt_tadd(ctx, stmt, tc, isTYPEN, _("not type: %s"));
				knh_Stmt_add_PPARAMs(ctx, stmt, tc);
				knh_Stmt_add_STMT1(ctx, stmt, tc);
				fc = tc->c;
				goto L_NEXTFUNC;
			}
		}
	}

	/** first **/
	KNH_ASSERT(fc < e);
	{
		knh_token_t tt0 = TT_(ts[oc]), tt1 = TTn_(ts[oc]);
		if((tt0 == TT_CONSTN || tt0 == TT_TYPEN) && tt1 == TT_PARENTHESIS) {
			TT_(ts[oc]) = TT_NAME;
			tt0 = TT_NAME;
		}
		/* NAME */
		if(tt0 == TT_NAME) {
			if(tt1 == TT_PARENTHESIS) {   /* @TEST func(a) */
				stmt = new_StmtNAME2(ctx, ts[oc]);
				pc = oc + 1;
				goto L_PARAM;
			}
			else if(tt1 == TT_BRANCET) {    /* @TEST d[a] */
				stmt = new_StmtNAME1(ctx, ts[oc]);
				fc = oc + 1;
				goto L_FUNC;
			}
			else {
				tc->c = oc + 1;
				knh_tokens_ignoreLast(ctx, tc);
				return new_TermVALUE(ctx, ts[oc], isData);
			}
		}
		else if(tt0 == TT_CMETHODN) {
			if(tt1 == TT_PARENTHESIS) {   /* @TEST C.func(a) */
				stmt = new_StmtNAME2(ctx, ts[oc]);
				pc = oc + 1;
				goto L_PARAM;
			}
			else {
				tc->c = oc + 1;
				knh_tokens_ignoreLast(ctx, tc);
				return new_TermVALUE(ctx, ts[oc], isData);
			}
		}/* TT_NAME */
		else if(tt0 == TT_MT) {  /* @TEST %s(a) */
			if(tt1 != TT_PARENTHESIS) {
				tc->c = oc + 1;
				knh_Token_perror(ctx, ts[oc], KERR_ERROR, _("syntax error"));
				return TM(new_StmtERR(ctx, tc));
			}
			stmt = new_Stmt(ctx, 0, STT_MT);
			knh_Stmt_add(ctx, stmt, TM(ts[oc]));
			pc = oc + 1;
			goto L_PARAM;
		} /* DP(ts[oc])->TT == TT_MT */
		else if(knh_Token_isTYPEN(ts[oc])) {
			if(TTn_(ts[oc]) == TT_BRACE) {  /* @TEST Class {..} */
				Term *tm = new_TermBLOCK(ctx, ts[oc], ts[oc+1], isData);
				tc->c = oc + 2;
				knh_tokens_ignoreLast(ctx, tc);
				return tm;
			}
			tc->c = oc + 1;
			knh_tokens_ignoreLast(ctx, tc);
			return TM(ts[oc]);
		}
		else {
			DBG2_ASSERT(stmt == NULL);
			//DBG2_P("FIRST TOKEN=%s", knh_token_tochar(tt0));
			Term *tm = new_TermVALUE(ctx, ts[oc], isData);
			if(TT_((knh_Token_t*)tm) == TT_ERR) {
				return tm;
			}
			stmt = new_Stmt(ctx, 0, STT_CALL);
			knh_Stmt_add(ctx, stmt, tm);
			fc = oc + 1;
		}
	}

	L_FUNC:; /* function name */
	KNH_ASSERT(stmt != NULL);
	KNH_ASSERT(DP(stmt)->size == 1);
	KNH_ASSERT(fc > 0);

	if(TT_(ts[fc]) == TT_NAME && knh_Token_isTopDot(ts[fc])) {
		stmt = knh_Stmt_add_SECONDNAME(ctx, stmt, ts[fc]);
		if(TTn_(ts[fc]) == TT_PARENTHESIS) {  /* @TEST (a).f(a) */
			pc = fc + 1;
			goto L_PARAM;
		}
		else if(TTn_(ts[fc]) == TT_BRANCET) { /* @TEST (a).f[a] */
			KNH_ASSERT(DP(stmt)->size == 2);
			stmt = new_StmtCALLSTMT(ctx, stmt);
			fc = fc + 1;
			goto L_FUNC;
		}
		else if(fc + 1 < tc->e) {
			knh_Token_perror(ctx, ts[fc], KERR_BAD, _("%s wants ()"), sToken(ts[fc]));
			tc->c = fc + 1;
			knh_Stmt_add(ctx, stmt, new_TermEXPR(ctx, tc, 0/*isData*/));
		}
		/* (a).f */
		return TM(stmt);
	}
	else if(TT_(ts[fc]) == TT_BRANCET) {
		knh_tkc_t tcbuf, *expr = knh_Token_tc(ctx, ts[fc], &tcbuf);
		int idx = knh_tokens_findRANGEIDX(expr);
		knh_Stmt_add_RANGE(ctx, stmt, expr, idx, isData);
		fc = fc + 1;
		goto L_NEXTFUNC;
	}
	else {
		DBG2_P("** Unexpected funcname %s **", knh_token_tochar(TT_(ts[fc])));
		knh_Token_perror(ctx, ts[fc], KERR_ERROR, _("not method call %s"), sToken(ts[fc]));
		knh_Stmt_add(ctx, stmt, TM(ts[fc]));
		tc->c = tc->e;
	}
	return TM(stmt);

	/** third *************************************************************/

	L_PARAM:;
	DBG2_ASSERT(stmt != NULL);
	DBG2_ASSERT(pc > 0);
	//DBG2_P("oc=%d, fc=%d, pc=%d %s", oc, fc, pc, knh_token_tochar(TT_(ts[pc])));
	DBG2_ASSERT(TT_(ts[pc]) == TT_PARENTHESIS);
	{
		knh_tkc_t ptc;
		knh_Token_tc(ctx, ts[pc], &ptc);
		//DBG2_P("size=%d, ptc=%d", DP(stmt)->size, ptc->e);
		knh_Stmt_add_EXPRs(ctx, stmt, &ptc, isData);
	}
	if(DP(stmt)->size == 1) {
        //DBG2_P("size=%d", DP(stmt)->size);
		knh_perror(ctx, SP(ts[pc])->uri, SP(ts[pc])->line, KERR_ERRATA, _("empty: () == > (void)"));
		knh_Stmt_add(ctx, stmt, TM(new_TokenCONST(ctx, FL(ts[oc]), KNH_NULL)));
	}
	fc = pc + 1; pc = 0;

	L_NEXTFUNC:;
	if(fc < e) {
		stmt = new_StmtCALLSTMT(ctx, stmt);
		goto L_FUNC;
	}
	return TM(stmt);
}

/* ======================================================================== */
/* [StmtMETA] */

static
void knh_StmtMETA_addLabel(Ctx *ctx, knh_Stmt_t *stmt, knh_Token_t *tkL)
{
	if(IS_NULL(DP(stmt)->metaDictMap)) {
		KNH_SETv(ctx, DP(stmt)->metaDictMap, new_DictMap0(ctx, 2));
	}
	if(IS_DictMap(DP(stmt)->metaDictMap)) {
		knh_DictMap_set(ctx, DP(stmt)->metaDictMap, TS_ATlabel, UP(tkL));
	}
}

/* ------------------------------------------------------------------------ */

static
void knh_StmtMETA_addComment(Ctx *ctx, knh_Stmt_t *stmt, knh_String_t *comment)
{
	if(IS_NULL(DP(stmt)->metaDictMap)) {
		KNH_SETv(ctx, DP(stmt)->metaDictMap, new_DictMap0(ctx, 2));
	}
	if(IS_DictMap(DP(stmt)->metaDictMap)) {
		knh_DictMap_set(ctx, DP(stmt)->metaDictMap, TS_Comment, UP(comment));
	}
}

/* ------------------------------------------------------------------------ */

static
void knh_StmtMETA_addMeta(Ctx *ctx, knh_Stmt_t *stmt, knh_String_t *k, knh_Token_t *tkP)
{
	if(IS_NULL(DP(stmt)->metaDictMap)) {
		KNH_SETv(ctx, DP(stmt)->metaDictMap, new_DictMap0(ctx, 2));
	}
	if(IS_DictMap(DP(stmt)->metaDictMap)) {
		if(tkP == NULL) {
			knh_DictMap_set(ctx, DP(stmt)->metaDictMap, k, UP(k));
		}
		else {
			knh_tkc_t tcbuf, *tc = knh_Token_tc(ctx, tkP, &tcbuf);
			knh_Token_t **ts = tc->ts;
			if(tc->e == 0) {
				knh_DictMap_set(ctx, DP(stmt)->metaDictMap, k, UP(k));
			}
			else if(tc->e == 1) {
				if(IS_String(DP(tc->ts[0])->data)) {
					knh_DictMap_set(ctx, DP(stmt)->metaDictMap, k, DP(ts[0])->data);
				}
				else {
					knh_bytes_t t = knh_Token_tobytes(ctx, tc->ts[0]);
					knh_DictMap_set(ctx, DP(stmt)->metaDictMap, k, UP(new_String(ctx, t, NULL)));
				}
			}
			else {
				knh_Array_t *a = new_Array(ctx, CLASS_String, tc->e);
				int c = 0;
				for(c = 0; c < tc->e; c++) {
					if(TT_(ts[c]) == TT_COMMA) continue;
					if(IS_String(DP(tc->ts[0])->data)) {
						knh_Array_add(ctx, a, DP(ts[0])->data);
					}
					else {
						knh_bytes_t t = knh_Token_tobytes(ctx, tc->ts[0]);
						knh_Array_add(ctx, a, UP(new_String(ctx, t, NULL)));
					}
				}
				knh_DictMap_set(ctx, DP(stmt)->metaDictMap, k, UP(a));
			}
		}
	}
}

/* ------------------------------------------------------------------------ */

static
void knh_Stmt_addMETA(Ctx *ctx, knh_Stmt_t *stmt, knh_tkc_t *tc)
{
	int i, e;
	knh_Token_t **ts = tc->ts;
	if(tc->meta == -1 || STT_(stmt) == STT_DONE || STT_(stmt) == STT_ERR) {
		return ;
	}
	e = (tc->meta < tc->c) ? tc->c : tc->e;
	for(i = tc->meta; i < e; i++) {
		if(TT_(ts[i]) == TT_LABEL) {
			knh_StmtMETA_addLabel(ctx, stmt, ts[i]);
		}
		if(TT_(ts[i]) == TT_DOC) {
			knh_StmtMETA_addComment(ctx, stmt, DP(ts[i])->text);
		}
		else if(TT_(ts[i]) == TT_METAN) {
			if(TTn_(ts[i]) == TT_PARENTHESIS) {
				knh_StmtMETA_addMeta(ctx, stmt, DP(ts[i])->text, ts[i+1]);
				i++;
			}
			else {
				knh_StmtMETA_addMeta(ctx, stmt, DP(ts[i])->text, NULL);
			}
		}
	}
	if(e == tc->e) tc->c = i;
	tc->meta = -1;
}

/* ------------------------------------------------------------------------ */

static
knh_Stmt_t *new_StmtMETA(Ctx *ctx,  knh_tkc_t *tc, knh_stmt_t stt)
{
	knh_Stmt_t *stmt = new_Stmt(ctx, 0, stt);
	knh_Stmt_addMETA(ctx, stmt, tc);
	int c = tc->c - 1;
	if(c < 0) c = 0;
	SP(stmt)->line = SP(tc->ts[c])->line;
	SP(stmt)->uri = SP(tc->ts[c])->uri;
	return stmt;
}


/* ======================================================================== */
/* [new_Stmt] */

static knh_Stmt_t *new_StmtNAMESPACE(Ctx *ctx, knh_tkc_t *tc)
{
	knh_Token_perror(ctx, tc->ts[tc->c-1], KERR_EWARN, _("namespace will be supported soon or later"));
	knh_tokens_nextStmt(tc);
	return new_Stmt(ctx, 0, STT_DONE);
}

/* ------------------------------------------------------------------------ */
/* [import] */

static knh_Stmt_t *new_StmtIMPORT(Ctx *ctx, knh_tkc_t *tc)
{
	knh_Stmt_t *o = new_StmtMETA(ctx, tc, STT_IMPORT);
	knh_Stmt_tadd(ctx, o, tc, isPATH, _("not path: %s")); /* PATH */
	knh_Stmt_add_SEMICOLON(ctx, o, tc); /* ; */
	return o;
}

/* ------------------------------------------------------------------------ */
/* [USING] */

/* ------------------------------------------------------------------------ */
/* [using class] */

static int isUCLASSN(Ctx *ctx, knh_Token_t *tk)
{
	return ((knh_Token_isTYPEN(tk) && !knh_Token_isTailWildCard(tk)) || knh_Token_isNAME(tk));
}

static
knh_Stmt_t *new_StmtUCLASS(Ctx *ctx, knh_tkc_t *tc)
{
	knh_Stmt_t *o = new_StmtMETA(ctx, tc, STT_UCLASS);
	knh_Stmt_tadd(ctx, o, tc, isUCLASSN, _("not class %s"));
	knh_Stmt_add_SEMICOLON(ctx, o, tc); /* ; */
	return o;
}

/* ------------------------------------------------------------------------ */
/* [using unit] */

static
knh_Stmt_t *new_StmtUUNIT(Ctx *ctx, knh_tkc_t *tc)
{
	knh_Stmt_t *o = new_StmtMETA(ctx, tc, STT_UUNIT);
	knh_Stmt_tadd(ctx, o, tc, isCLASSN, _("not class: %s")); /* CLASSTN */
	knh_Stmt_tadd(ctx, o, tc, isPATH, _("not path: %s")); /* PATH */
	knh_Stmt_add_SEMICOLON(ctx, o, tc); /* ; */
	return o;
}

/* ------------------------------------------------------------------------ */
/* [using enum] */

static
knh_Stmt_t *new_StmtUENUM(Ctx *ctx, knh_tkc_t *tc)
{
	knh_Stmt_t *o = new_StmtMETA(ctx, tc, STT_UENUM);
	knh_Stmt_tadd(ctx, o, tc, isCLASSN, _("not class: %s")); /* CLASSTN */
	knh_Stmt_tadd(ctx, o, tc, isPATH, _("not path: %s")); /* PATH */
	knh_Stmt_add_SEMICOLON(ctx, o, tc); /* ; */
	return o;
}

/* ------------------------------------------------------------------------ */
/* [using vocab] */

static
knh_Stmt_t *new_StmtUVOCAB(Ctx *ctx, knh_tkc_t *tc)
{
	knh_Stmt_t *o = new_StmtMETA(ctx, tc, STT_UVOCAB);
	knh_Stmt_tadd(ctx, o, tc, isCLASSN, _("not class: %s")); /* CLASSTN */
	knh_Stmt_tadd(ctx, o, tc, isPATH, _("not path: %s")); /* PATH */
	knh_Stmt_add_SEMICOLON(ctx, o, tc); /* ; */
	return o;
}

/* ------------------------------------------------------------------------ */
/* [using func] */

static int isUFUNCN(Ctx *ctx, knh_Token_t *tk)
{
	return (knh_Token_isCMETHODN(tk) || (knh_Token_isTYPEN(tk) && knh_Token_isTailWildCard(tk)));
}

static
knh_Stmt_t *new_StmtUFUNC(Ctx *ctx, knh_tkc_t *tc)
{
	knh_Stmt_t *o = new_StmtMETA(ctx, tc, STT_UFUNC);
	knh_Stmt_tadd(ctx, o, tc, isUFUNCN, _("not func"));
	knh_Stmt_add_SEMICOLON(ctx, o, tc); /* ; */
	return o;
}

/* ------------------------------------------------------------------------ */
/* [using mapmap] */

//static
//Stmt *new_StmtUMAPMAP(Ctx *ctx, knh_tokens_t *tc)
//{
//	Stmt *o = new_StmtMETA(ctx, tc, STT_UMAPMAP);
//	knh_Stmt_add_CLASSTN(ctx, o, tc); /* CLASSTN */
//	knh_Stmt_add_CLASSTN(ctx, o, tc); /* CLASSTN */
//	knh_Stmt_tadd(ctx, o, tc, isPATH, _("not path: %s")); /* PATH */
//	knh_Stmt_add_SEMICOLON(ctx, o, tc); /* ; */
//	return o;
//}

/* ------------------------------------------------------------------------ */

static
knh_Stmt_t *new_StmtUSING(Ctx *ctx, knh_tkc_t *tc)
{
	if(HAS_TOKEN(tc)) {
		knh_Token_t *tk = TK0(tc);
		knh_bytes_t op = knh_Token_tobytes(ctx, tk);

		/* using naruto.* */
		if(TT_(TK0(tc)) == TT_CLASS) {
			tc->c += 1;
			return new_StmtUCLASS(ctx, tc);
		}

		/* using Math.* */
		if(TT_(TK0(tc)) == TT_FUNCTION) {
			tc->c += 1;
			return new_StmtUFUNC(ctx, tc);
		}
		if(isUFUNCN(ctx, tk)) {
			return new_StmtUFUNC(ctx, tc);
		}

		if(ISB(op, "unit")) {
			tc->c += 1;
			return new_StmtUUNIT(ctx, tc);
		}
		if(knh_bytes_startsWith(op, STEXT("Float:")) || knh_bytes_startsWith(op, STEXT("float:"))) {
			return new_StmtUUNIT(ctx, tc);
		}

		if(knh_bytes_startsWith(op, STEXT("vocab"))) {
			tc->c += 1;
			return new_StmtUVOCAB(ctx, tc);
		}
		if(knh_bytes_startsWith(op, STEXT("String:"))) {
			return new_StmtUVOCAB(ctx, tc);
		}

		if(ISB(op, "enum")) {
			tc->c += 1;
			return new_StmtUENUM(ctx, tc);
		}
		if(knh_bytes_startsWith(op, STEXT("Int:"))||knh_bytes_startsWith(op, STEXT("int:"))) {
			return new_StmtUENUM(ctx, tc);
		}

		if(isUCLASSN(ctx, tk)) {
			return new_StmtUCLASS(ctx, tc);
		}

		knh_Token_perror(ctx, tc->ts[tc->c-1], KERR_EWARN, _("unknown using options: %s"), sToken(tk));
	}
	knh_tokens_nextStmt(tc);
	return new_Stmt(ctx, 0, STT_DONE);
}

/* ------------------------------------------------------------------------ */
/* [class] */
/* ------------------------------------------------------------------------ */

static knh_Stmt_t *new_StmtIMPLEMENTS(Ctx *ctx, knh_tkc_t *tc)
{
	knh_Stmt_t *o = new_Stmt(ctx, 0, STT_IMPLEMENTS);
	for(;HAS_TOKEN(tc); tc->c += 1) {
		knh_Token_t *tk0 = TK0(tc);
		if(isCLASSN(ctx, tk0)) {
			knh_Stmt_addT(ctx, o, tk0);
		}
		if(TT_(tk0) == TT_BRACE || TT_(tk0) == TT_SEMICOLON) break;
	}
	return o;
}

int isSHORTCLASSN(Ctx *ctx, knh_Token_t *tk)
{
	if(TT_(tk) == TT_TYPEN) {
		knh_bytes_t t = knh_Token_tobytes(ctx, tk);
		size_t i;
		if(islower(t.buf[0])) return 0;
		for(i = 1; i < t.len; i++) {
			if(t.buf[i] == ':' || t.buf[i] == '<' || t.buf[i] == '[') return 0;
		}
		return 1;
	}
	return 0;

}

static knh_Stmt_t *new_StmtCLASS(Ctx *ctx, knh_tkc_t *tc)
{
	knh_Stmt_t *o = new_StmtMETA(ctx, tc, STT_CLASS);
	knh_Stmt_tadd(ctx, o, tc, isSHORTCLASSN, _("not class: %s")); /* CLASSN */
	if(STT_(o) == STT_ERR) return o;
	if(HAS_TOKEN(tc) && TT_(TK0(tc)) == TT_EXTENDS) {
		tc->c += 1;
		knh_Stmt_tadd(ctx, o, tc, isCLASSN, _("not class: %s"));
	}
	else { /* Object */
		knh_Stmt_add_SYMBOL(ctx, o, TT_TYPEN, ctx->share->ClassTable[CLASS_Object].sname);
	}
	if(HAS_TOKEN(tc) && TT_(TK0(tc)) == TT_IMPLEMENTS) {
		tc->c += 1;
		knh_Stmt_add(ctx, o, TM(new_StmtIMPLEMENTS(ctx, tc)));
	}
	else {
		knh_Stmt_add(ctx, o, TM(new_Stmt(ctx, 0, STT_DONE)));
	}
	knh_Stmt_add_STMT1(ctx, o, tc);
	return o;
}

/* ------------------------------------------------------------------------ */
/* [format] */

static int isTEXT(Ctx *ctx, knh_Token_t *tk)
{
	return (TT_(tk) == TT_STR || TT_(tk) == TT_TSTR || TT_(tk) == TT_ESTR);
}

static knh_Stmt_t *new_StmtFORMAT(Ctx *ctx, knh_tkc_t *tc)
{
	knh_Token_t **ts = tc->ts;
	if(HAS_TOKEN(tc) && TT_(ts[tc->c]) == TT_PARENTHESIS) {
		/* format("%s", a) */
		return new_StmtLETEXPR(ctx, knh_tokens_prev(tc), 0/*isData*/);
	}
	else {
		/* format String %MT (param) { }*/
		knh_Stmt_t *o = new_StmtMETA(ctx, tc, STT_FORMAT);
		knh_Stmt_tadd(ctx, o, tc, isCLASSN, NULL);
		knh_Stmt_tadd(ctx, o, tc, isMT, _("not formatter: %s")); /* CLASSN */
		if(STT_(o) == STT_ERR) return o;
		knh_Stmt_add_PPARAMs(ctx, o, tc);
		knh_Stmt_tadd(ctx, o, tc, isTEXT, _("not format: %s")); /* CLASSN */
		return o;
	}
}

/* ------------------------------------------------------------------------ */
/* [return] */

static knh_Stmt_t *new_StmtRETURN(Ctx *ctx, knh_tkc_t *tc)
{
	knh_Stmt_t *o = new_StmtMETA(ctx, tc, STT_RETURN);
	knh_tkc_t tcbuf, *exprs = knh_tokens_firstSTMT(ctx, tc, /*pos*/-1, &tcbuf, /*need;*/1);
	knh_Stmt_add_EXPRs(ctx, o, exprs, 0/*isData*/); /* EXPR* */
	return o;
}

/* ------------------------------------------------------------------------ */
/* [mapmap] */

static knh_Stmt_t *new_StmtMAPMAP(Ctx *ctx, knh_tkc_t *tc)
{
	knh_Stmt_t *o = new_StmtMETA(ctx, tc, STT_MAPMAP);
	knh_tkc_t tcbuf, *expr = knh_tokens_firstSTMT(ctx, tc, /*pos*/-1, &tcbuf, 1/*needs;*/);
	for(; HAS_TOKEN(expr); expr->c += 1) {
		if(isCLASSN(ctx, TK0(expr))) {
			knh_Stmt_addT(ctx, o, TK0(expr));
		}
	}
	if(DP(o)->size == 1) {  /* mapmap C */
		knh_Stmt_done(ctx, o);
	}
	return o;
}

/* ------------------------------------------------------------------------ */
/* [weave] */

static knh_Stmt_t *new_StmtWEAVE(Ctx *ctx, knh_tkc_t *tc)
{
	knh_Token_perror(ctx, tc->ts[tc->c-1], KERR_EWARN, _("weave will be supported soon or later"));
	knh_tokens_nextStmt(tc);
	return new_Stmt(ctx, 0, STT_DONE);
}

/* ------------------------------------------------------------------------ */
/* [aspect] */

static knh_Stmt_t *new_StmtASPECT(Ctx *ctx, knh_tkc_t *tc)
{
	knh_Token_perror(ctx, tc->ts[tc->c-1], KERR_EWARN, _("aspect will be supported soon or later"));
	knh_tokens_nextStmt(tc);
	return new_Stmt(ctx, 0, STT_DONE);
}

/* ------------------------------------------------------------------------ */
/* [block] */

static
int knh_Token_isBLOCK(Ctx *ctx, knh_Token_t *tk)
{
	knh_tkc_t tcbuf, *tc = knh_Token_tc(ctx, tk, &tcbuf);
	int i, c = 0;
	for(i = 0; i < tc->e; i++) {
		if(TT_(tc->ts[i]) == TT_FUNCMAP) return 0;
		if(TT_(tc->ts[i]) == TT_LABEL || TT_(tc->ts[i]) == TT_COLON) {
			c++;
		}
	}
	if(c > 0) return 0;
	return (tc->e == 0) ? 0 : 1;
}

static knh_Stmt_t *new_StmtBLOCK(Ctx *ctx, knh_tkc_t *tc)
{
	knh_Stmt_t *o = new_StmtMETA(ctx, tc, STT_BLOCK);
	knh_Stmt_add_STMT1(ctx, o, tc); /* { */
	return o;
}

/* ------------------------------------------------------------------------ */
/* [if] */
/* ------------------------------------------------------------------------ */

static knh_Stmt_t *new_StmtIF(Ctx *ctx, knh_tkc_t *tc)
{
	knh_Stmt_t *o = new_StmtMETA(ctx, tc, STT_IF);
	knh_Stmt_add_PEXPR(ctx, o, tc); /* pexpr */
	knh_Stmt_add_STMT1(ctx, o, tc); /* { */
	if(STT_(o) == STT_ERR) return o;
	if(HAS_TOKEN(tc) && TT_(TK0(tc)) == TT_ELSE) {
		tc->c += 1;
		knh_Stmt_add_STMT1(ctx, o, tc);
	}
	else {
		knh_Stmt_add(ctx, o, TM(new_Stmt(ctx, 0, STT_DONE)));
	}
	return o;
}

/* ------------------------------------------------------------------------ */
/* [switch] */

static knh_Stmt_t *new_StmtCASE(Ctx *ctx, knh_tkc_t *expr, knh_tkc_t *tc)
{
	knh_Stmt_t *stmt = new_Stmt(ctx, 0, STT_CASE);
	int i;
	if(IS_EMPTY(expr)) {
		knh_Stmt_add_ASIS(ctx, stmt);
	}
	else {
		knh_Stmt_add(ctx, stmt, new_TermEXPR(ctx, expr, /*isData*/0));
	}
	*expr = *tc;
	for(i = tc->c; i < tc->e; i++) {
		if(TT_(tc->ts[i]) == TT_CASE) {
			break;
		}
		if(TT_(tc->ts[i]) == TT_NAME && TTn_(tc->ts[i]) == TT_COLON && IST_(tc->ts[i], "default")) {
			break;
		}
		if(TT_(tc->ts[i]) == TT_LABEL && IST_(tc->ts[i], "default")) {
			break;
		}
	}
	expr->e = i;
	if(IS_EMPTY(expr)) {
		knh_Stmt_add(ctx, stmt, TM(new_Stmt(ctx, 0, STT_DONE)));
	}
	else {
		knh_Stmt_add(ctx, stmt, TM(new_StmtSTMTBLOCK(ctx, expr, 0/*isData*/)));
	}
	tc->c = i; // next
	return stmt;
}

static void knh_Stmt_add_CASEs(Ctx *ctx, knh_Stmt_t *o, knh_tkc_t *tc)
{
	knh_Stmt_t *stmt_head = NULL;
	while(HAS_TOKEN(tc)) {
		knh_Token_t *tk0 = TK0(tc); tc->c += 1;
		if(TT_(tk0) == TT_CASE) {
			knh_tkc_t tcbuf, *expr = NULL;
			if(TT_(TK0(tc)) == TT_LABEL) {
				expr = knh_tokens_firstEXPR(tc, tc->c, &tcbuf);
				DBG2_ASSERT(TT_(TK0(expr)) == TT_LABEL);
				if(knh_bytes_index(__tobytes(DP(TK0(expr))->text), '.') == -1) {
					TT_(TK0(expr)) = TT_CONSTN;
				}
				else {
					TT_(TK0(expr)) = TT_CCONSTN;
				}
				expr->e = expr->c + 1;
			}
			else {
				int idx = knh_tokens_findTTIDX(tc, TT_COLON, tc->e);
				expr = knh_tokens_firstEXPR(tc, idx, &tcbuf);
			}
			if(IS_EMPTY(expr)) {
				knh_Token_perror(ctx, tk0, KERR_DWARN, _("no condition in case"));
			}
			else {
				knh_Stmt_t *stmt = new_StmtCASE(ctx, expr, tc);
				stmt_head = knh_StmtNULL_tail_append(ctx, stmt_head, stmt);
			}
			continue;
		}
		if((TT_(tk0) == TT_NAME || TT_(tk0) == TT_LABEL) && IST_(tk0, "default")) {
			knh_tkc_t tcbuf = *tc, *expr = &tcbuf; expr->c = expr->e;
			if(TT_(tk0) == TT_NAME && TT_(TK0(tc)) == TT_COLON) {
					tc->c += 1;
			}
			knh_Stmt_t *stmt = new_StmtCASE(ctx, expr, tc);
			stmt_head = knh_StmtNULL_tail_append(ctx, stmt_head, stmt);
		}
	}
	if(stmt_head == NULL) { /* empty cases */
		knh_Stmt_done(ctx, o);
	}
	else {
		knh_Stmt_add(ctx, o, TM(stmt_head));
	}
}

static knh_Stmt_t *new_StmtSWITCH(Ctx *ctx, knh_tkc_t *tc)
{
	knh_Stmt_t *o = new_StmtMETA(ctx, tc, STT_SWITCH);
	knh_Stmt_add_PEXPR(ctx, o, tc); /* pexpr */
	if(STT_(o) == STT_ERR) return o;
	if(HAS_TOKEN(tc) && TT_(TK0(tc)) == TT_BRACE) {
		knh_tkc_t tcbuf, *cases = knh_Token_tc(ctx, TK0(tc), &tcbuf);
		knh_Stmt_add_CASEs(ctx, o, cases);
	}
	else {
		knh_Stmt_tokens_perror(ctx, o, tc, _("needs {case ..}"));
		return o;
	}
	tc->c += 1;
	return o;
}

/* ------------------------------------------------------------------------ */
/* [while] */

static knh_Stmt_t *new_StmtWHILE(Ctx *ctx, knh_tkc_t *tc)
{
	knh_Stmt_t *o = new_StmtMETA(ctx, tc, STT_WHILE);
	knh_Stmt_add_PEXPR(ctx, o, tc); /* pexpr */
	knh_Stmt_add_STMT1(ctx, o, tc); /* { */
	return o;
}

/* ------------------------------------------------------------------------ */
/* [do] */

static knh_Stmt_t *new_StmtDO(Ctx *ctx, knh_tkc_t *tc)
{
	knh_Stmt_t *o = new_StmtMETA(ctx, tc, STT_DO);
	knh_Stmt_add_STMT1(ctx, o, tc); /* { */
	knh_Stmt_add_PEXPR(ctx, o, tc); /* pexpr */
	knh_Stmt_add_SEMICOLON(ctx, o, tc); /* ; */
	return o;
}

/* ------------------------------------------------------------------------ */
/* [foreach] */

static int knh_tokens_findFROMIDX(knh_tkc_t *tc, int def)
{
	int i;
	for(i = tc->c; i < tc->e; i++) {
		knh_token_t tt = TT_(tc->ts[i]);
		if(tt == TT_FROM || tt == TT_COLON || tt == TT_IN) {
			return i;
		}
	}
	return def;
}

static
void knh_Stmt_add_PEACH(Ctx *ctx, knh_Stmt_t *o, knh_tkc_t *tc)
{
	int idx = knh_tokens_findFROMIDX(tc, -1);
	if(idx == -1) {
		knh_Stmt_tokens_perror(ctx, o, tc, _("syntax error: needs in/from"));
		return;
	}
	else {
		knh_tkc_t tcbuf, *expr = knh_tokens_firstEXPR(tc, idx, &tcbuf);
#if defined(TT_SELECT)
		if(TT_(TK0(expr)) == TT_SELECT) {
			knh_Stmt_setSELECT(o, 1);
			expr->c += 1;
		}
#endif
		knh_Stmt_add_PARAMs(ctx, o, expr);
		if(STT_(o) == STT_ERR) return;
		idx = knh_tokens_findTTIDX(tc, TT_WHERE, tc->e);
		expr = knh_tokens_firstEXPR(tc, idx, &tcbuf);
		knh_Stmt_add(ctx, o, new_TermEXPR(ctx, expr, 0/*isData*/)); /* FROM */
		if(STT_(o) == STT_ERR) return;
		if(HAS_TOKEN(tc)) { /* WHERE */
			knh_Stmt_add(ctx, o, new_TermEXPR(ctx, tc, 0/*isData*/));
		}
		else {
			knh_Stmt_add(ctx, o, TM(new_TokenCONST(ctx, FL(o), KNH_TRUE)));
		}
	}
}

int knh_tokens_findPEACHBRACEIDX(knh_tkc_t *tc)
{
	int i, c = 0;
	for(i = tc->c; i < tc->e; i++) {
		knh_token_t tt = TT_(tc->ts[i]);
		if(tt == TT_BRACE) {
			return (c > 0) ? i: -1;
		}
		if(tt == TT_COLON || tt == TT_FROM || tt == TT_IN) {
			c = 1;
		}
	}
	return -1;
}

static knh_Stmt_t *new_StmtFOREACH(Ctx *ctx, knh_tkc_t *tc)
{
	knh_Stmt_t *o = new_StmtMETA(ctx, tc, STT_FOREACH);
	if(HAS_TOKEN(tc)) {
		if(TT_(TK0(tc)) == TT_PARENTHESIS) {
			knh_tkc_t tcbuf, *stmts = knh_Token_tc(ctx, TK0(tc), &tcbuf);
			knh_Stmt_add_PEACH(ctx, o, stmts); /* peach */
			tc->c += 1;
			knh_Stmt_add_STMT1(ctx, o, tc);
			return o;
		}
		else {
			/* @TEST foreach a in b {.. } */
			int idx = knh_tokens_findPEACHBRACEIDX(tc);
			if(idx != -1) {
				knh_tkc_t tcbuf, *stmts = knh_tokens_firstEXPR(tc, idx, &tcbuf);
				knh_Stmt_add_PEACH(ctx, o, stmts); /* peach */
				tc->c -= 1; DBG2_ASSERT(TT_(TK0(tc)) == TT_BRACE);
				knh_Stmt_add_STMT1(ctx, o, tc);
				return o;
			}
		}
	}
	knh_Stmt_tokens_perror(ctx, o, tc, _("needs ()"));
	return o;
}

/* ------------------------------------------------------------------------ */
/* [for] */

static
void knh_Stmt_add_PSTMT3(Ctx *ctx, knh_Stmt_t *o, knh_tkc_t *tc)
{
	int c = knh_tokens_count(tc, TT_SEMICOLON);
	if(c != 2) {
		knh_Stmt_tokens_perror(ctx, o, tc, _("needs ;; in for"));
	}
	else {
		int idx = knh_tokens_findTTIDX(tc, TT_SEMICOLON, tc->e);
		knh_tkc_t tcbuf, *expr = knh_tokens_firstEXPR(tc, idx, &tcbuf);
		if(IS_EMPTY(expr)) {
			knh_Stmt_add(ctx, o, TM(new_Stmt(ctx, 0, STT_DONE)));
		}
		else {
			knh_Token_t *tkT = TK0(expr);
			if(TT_(tkT) == TT_TYPEN && TTn_(tkT) == TT_NAME) {
				expr->c += 1;
				knh_Stmt_add(ctx, o, TM(new_StmtDECL(ctx, tkT, expr)));
			}
			else {
				knh_Stmt_add(ctx, o, TM(new_StmtLETEXPR(ctx, expr, 0/*isData*/)));
			}
		}
		if(STT_(o) == STT_ERR) return;
		idx = knh_tokens_findTTIDX(tc, TT_SEMICOLON, tc->e);
		expr = knh_tokens_firstEXPR(tc, idx, &tcbuf);

		if(IS_EMPTY(expr)) {
			knh_Stmt_addT(ctx, o, TM(new_TokenCONST(ctx, FL(tc->ts[idx]), KNH_TRUE)));
		}
		else {
			knh_Stmt_add(ctx, o, new_TermEXPR(ctx, expr, 0/*isData*/));
		}
		if(STT_(o) == STT_ERR) return;
		if(IS_EMPTY(tc)) {
			knh_Stmt_add(ctx, o, TM(new_Stmt(ctx, 0, STT_DONE)));
		}
		else {
			knh_Stmt_add(ctx, o, TM(new_StmtLETEXPR(ctx, tc, 0/*isData*/)));
		}
	}
}

static knh_Stmt_t *new_StmtFOR(Ctx *ctx, knh_tkc_t *tc)
{
	if(HAS_TOKEN(tc)) {
		if(TT_(TK0(tc)) == TT_PARENTHESIS) {
			knh_tkc_t tcbuf, *stmts = knh_Token_tc(ctx, TK0(tc), &tcbuf);
			if(knh_tokens_count(stmts, TT_COLON) > 0) {
				return new_StmtFOREACH(ctx, tc);
			}
			else {
				knh_Stmt_t *o = new_StmtMETA(ctx, tc, STT_FOR);
				knh_Stmt_add_PSTMT3(ctx, o, stmts); /* pstmt3 */
				tc->c += 1;
				knh_Stmt_add_STMT1(ctx, o, tc);
				return o;
			}
		}
		else {
			int idx = knh_tokens_findPEACHBRACEIDX(tc);
			if(idx != -1) {
				knh_tkc_t tcbuf, *stmts = knh_tokens_firstEXPR(tc, idx, &tcbuf);
				knh_Stmt_t *o = new_StmtMETA(ctx, tc, STT_FOREACH);
				knh_Stmt_add_PEACH(ctx, o, stmts); /* peach */
				tc->c -= 1; DBG2_ASSERT(TT_(TK0(tc)) == TT_BRACE);
				knh_Stmt_add_STMT1(ctx, o, tc);
				return o;
			}
		}
	}
	{
		knh_Stmt_t *o = new_StmtMETA(ctx, tc, STT_FOR);
		knh_Stmt_tokens_perror(ctx, o, tc, _("needs ()"));
		return o;
	}
}

/* ------------------------------------------------------------------------ */

static int isLABEL(Ctx *ctx, knh_Token_t *tk)
{
	return (TT_(tk) == TT_LABEL || TT_(tk) == TT_CONSTN || TT_(tk) == TT_NAME || TT_(tk) == TT_TYPEN);
}

static knh_Stmt_t *new_StmtBREAK(Ctx *ctx, knh_tkc_t *tc)
{
	knh_Stmt_t *o = new_StmtMETA(ctx, tc, STT_BREAK);
	knh_Stmt_tadd(ctx, o, tc, isLABEL, NULL);
	knh_Stmt_add_SEMICOLON(ctx, o, tc); /* ; */
	return o;
}

static knh_Stmt_t *new_StmtCONTINUE(Ctx *ctx, knh_tkc_t *tc)
{
	knh_Stmt_t *o = new_StmtMETA(ctx, tc, STT_CONTINUE);
	knh_Stmt_tadd(ctx, o, tc, isLABEL, NULL);
	knh_Stmt_add_SEMICOLON(ctx, o, tc); /* ; */
	return o;
}

/* ------------------------------------------------------------------------ */

static knh_Stmt_t *new_StmtCATCH(Ctx *ctx, knh_tkc_t *tc)
{
	knh_Stmt_t *stmt = NULL;
	if(HAS_TOKEN(tc) && TT_(TK0(tc)) == TT_PARENTHESIS) {
		knh_tkc_t tcbuf, *expr = knh_Token_tc(ctx, TK0(tc), &tcbuf);
		if(IS_EMPTY(expr)) {
			knh_Token_perror(ctx, TK0(tc), KERR_DWARN, _("nothing to catch"));
			return new_Stmt(ctx, 0, STT_DONE);
		}
		else {
			stmt = new_Stmt(ctx, 0, STT_CATCH);
			knh_Stmt_tadd(ctx, stmt, expr, isCLASSN, _("not exception: %s"));
			knh_Stmt_tadd(ctx, stmt, expr, isVARN, _("not variable: %s"));
			knh_tokens_ignoreLast(ctx, expr);
			tc->c += 1;
		}
		knh_Stmt_add_STMT1(ctx, stmt, tc);
	}
	else {
		knh_Stmt_tokens_perror(ctx, stmt, tc, _("needs ()"));
		return new_Stmt(ctx, 0, STT_DONE);
	}
	if(HAS_TOKEN(tc) && TT_(TK0(tc)) == TT_CATCH) {
		tc->c += 1;
		knh_StmtNULL_tail_append(ctx, stmt, new_StmtCATCH(ctx, tc));
	}
	return stmt;
}

static knh_Stmt_t *new_StmtTRY(Ctx *ctx, knh_tkc_t *tc)
{
	knh_Stmt_t *o = new_StmtMETA(ctx, tc, STT_TRY);
	knh_Stmt_add_STMT1(ctx, o, tc); /* { */
	if(STT_(o) == STT_ERR) return o;
	if(HAS_TOKEN(tc) && TT_(TK0(tc)) == TT_CATCH) {
		tc->c += 1;
		knh_Stmt_add(ctx, o, TM(new_StmtCATCH(ctx, tc)));
	}
	else {
		knh_Stmt_add(ctx, o, TM(new_Stmt(ctx, 0, STT_DONE)));
	}
	if(HAS_TOKEN(tc) && TT_(TK0(tc)) == TT_FINALLY) {
		tc->c += 1;
		knh_Stmt_add_STMT1(ctx, o, tc);
	}
	else {
		knh_Stmt_add(ctx, o, TM(new_Stmt(ctx, 0, STT_DONE)));
	}
	return o;
}

static knh_Stmt_t *new_StmtTHROW(Ctx *ctx, knh_tkc_t *tc)
{
	knh_Stmt_t *o = new_StmtMETA(ctx, tc, STT_THROW);
	knh_tkc_t tcbuf, *exprs = knh_tokens_firstSTMT(ctx, tc, /*pos*/-1, &tcbuf, /*isNeedSemicolon*/1);
	int c = knh_Stmt_add_EXPRs(ctx, o, exprs, 0/*isData*/); /* EXPR* */
	if(c == 0) {
		knh_Stmt_tokens_perror(ctx, o, tc, _("throw something"));
	}
	return o;
}

static knh_Stmt_t *new_StmtREGISTER(Ctx *ctx, knh_tkc_t *tc)
{
	knh_Stmt_t *o = new_StmtMETA(ctx, tc, STT_REGISTER);
	knh_tkc_t tcbuf, *exprs =
		knh_tokens_firstSTMT(ctx, tc, /*pos*/-1, &tcbuf, /*isNeedSemicolon*/1);
	knh_Stmt_add_EXPRs(ctx, o, exprs, 0/*isData*/); /* EXPR* */
	return o;
}

static knh_Stmt_t *new_StmtPRINT(Ctx *ctx, knh_tkc_t *tc)
{
	knh_Stmt_t *o = new_StmtMETA(ctx, tc, STT_PRINT);
	knh_tkc_t tcbuf, *exprs = knh_tokens_firstSTMT(ctx, tc, /*pos*/-1, &tcbuf, /*isNeedSemicolon*/1);
	knh_Stmt_add_EXPRs(ctx, o, exprs, 0/*isData*/); /* EXPR* */
	return o;
}

static knh_Stmt_t *new_StmtASSERT(Ctx *ctx, knh_tkc_t *tc)
{
	knh_Stmt_t *o = new_StmtMETA(ctx, tc, STT_ASSERT);
	knh_tkc_t tcbuf, *expr = knh_tokens_firstSTMT(ctx, tc, /*pos*/-1, &tcbuf, /*isNeedSemicolon*/1);
	int idx = knh_tokens_findTTIDX(expr, TT_BRACE, -1);
	if(idx == -1) {
		knh_Stmt_add(ctx, o, new_TermEXPR(ctx, expr, 0/*isData*/));
		knh_Stmt_add(ctx, o, TM(new_Stmt(ctx, 0, STT_DONE)));
	}
	else {
		knh_tkc_t tcbuf2, *expr2= knh_tokens_firstEXPR(expr, idx, &tcbuf2);
		knh_Stmt_add(ctx, o, new_TermEXPR(ctx, expr2, 0/*isData*/));
		expr2->c -= 1;
		DBG2_ASSERT(TT_(TK0(expr2)) == TT_BRACE);
		knh_Stmt_add_STMT1(ctx, o, expr2);
	}
	return o;
}

/* ------------------------------------------------------------------------ */
/* [assert] */

static knh_Stmt_t *new_StmtTEST(Ctx *ctx, knh_tkc_t *tc)
{
	knh_Stmt_t *o = new_StmtMETA(ctx, tc, STT_TEST);
	/* @TEST test hoge {.. } */
	int idx = knh_tokens_findTTIDX(tc, TT_BRACE, -1);
	if(idx != -1) {
		knh_tkc_t tcbuf, *expr = knh_tokens_firstEXPR(tc, idx, &tcbuf);
		knh_Stmt_add(ctx, o, new_TermEXPR(ctx, expr, 0/*isData*/));
		tc->c -= 1; DBG2_ASSERT(TT_(TK0(tc)) == TT_BRACE);
		knh_Stmt_add_STMT1(ctx, o, tc);
	}
	else  {
		knh_Stmt_tokens_perror(ctx, o, tc, _("syntax error"));
	}
	return o;
}

/* ------------------------------------------------------------------------ */

static
knh_Stmt_t* new_StmtTYPEEXPR(Ctx *ctx, knh_tkc_t *tc, int isData)
{
	knh_Token_t** ts = tc->ts;
	int c = tc->c;
	if(TT_(ts[c]) == TT_TYPEN && c+1 < tc->e) {
		if(TTn_(ts[c]) == TT_PARENTHESIS && TTn_(ts[c+1])== TT_BRACE) { /* @TEST Person (n) {...} */
			return new_StmtMETHOD(ctx, tc);
		}
		if(knh_token_isKeyword(TT_(ts[c+1]))) {
			tc->c = c + 1;
			knh_Token_perror(ctx, ts[c+1], KERR_ERROR, _("reserved keyword: %s"), sToken(ts[c+1]));
			return new_StmtERR(ctx, tc);
		}
		if(TTn_(ts[c+1]) == TT_PARENTHESIS) {
			knh_Token_t *tkN = ts[c+1];
			if(TT_(tkN) == TT_TYPEN || TT_(tkN) == TT_CONSTN) {
				knh_Token_perror(ctx, tkN, KERR_ERRATA, _("is really method name?: %s"), tkN);
				TT_(tkN) = TT_NAME;
			}
			if(TT_(tkN) == TT_CCONSTN) {
				knh_Token_perror(ctx, tkN, KERR_ERRATA, _("is truely class method name?: %s"), tkN);
				TT_(tkN)= TT_CMETHODN;
			}
			/* @TEST int func(n) { ... } */   /* @TEST float Math.abs() */
			return new_StmtMETHOD(ctx, tc);
		}
		if(TT_(ts[c+1]) == TT_NAME && isVARN(ctx, ts[c+1])) {  /* @TEST int name; */
			knh_Token_t *tkT = ts[c]; tc->c += 1;
			knh_tkc_t tcbuf, *decl = knh_tokens_firstSTMT(ctx, tc, /*pos*/0, &tcbuf, 1/*needs*/);
			return new_StmtDECL(ctx, tkT, decl);
		}
	}
	return new_StmtLETEXPR(ctx, tc, 0/*isData*/);
}

/* ------------------------------------------------------------------------ */
/* [pragma] */

static void konohac_pragma(Ctx *ctx, knh_tkc_t *tc)
{
	KNH_FUTURE("pargma");
	knh_tokens_nextStmt(tc);
}

/* ------------------------------------------------------------------------ */
/* [SPECIAL] */

static int knh_Token_isDEBUG(knh_Token_t *tk)
{
	if(TTn_(tk) == TT_BRACE) {
		knh_String_t *s = DP(tk)->text;
		if(IS_String(s)) {
			return (knh_bytes_strcasecmp(__tobytes(s), STEXT("DEBUG")) == 0);
		}
	}
	return 0;
}

/* ------------------------------------------------------------------------ */
/* [STMT1] */

static knh_Stmt_t *new_StmtNOTDATA(Ctx *ctx, knh_Token_t *tk)
{
	knh_Stmt_t *stmt = new_Stmt(ctx, 0, STT_CALL1);
	knh_Token_setCONST(ctx, tk, KNH_NULL);
	knh_Stmt_add(ctx, stmt, TM(tk));
	return stmt;
}

static knh_Stmt_t *new_StmtDATA(Ctx *ctx, knh_tkc_t *tc, int isData)
{
	knh_Token_t *tkc = NULL;
	int c = tc->c;
	tc->meta = -1;
	L_TAIL:;
	if(!(HAS_TOKEN(tc))) {
		DBG2_P("c = %d, e = %d", tc->c, tc->e);
		return new_StmtNOTDATA(ctx, tc->ts[c]);
	}

	tkc = TK0(tc);

	switch(TT_(tkc)) {
	case TT_LABEL:
	{
		if(tc->meta == -1) tc->meta = tc->c - 1;
		goto L_TAIL;
	}
	case TT_METAN:
	{
		if(tc->meta == -1) tc->meta = tc->c - 1;
		//DBG2_P("Found METAN '@%s', tc->meta=%d", sToken(tkc), tc->meta);
		if(TTn_(tkc) == TT_PARENTHESIS) {
			tc->c += 1;
		}
		if(TTn_(tkc) == TT_BRACE) {
			//DBG2_P("Found DATA '@%s', tc->meta=%d", sToken(tkc), tc->meta);
			tc->c += 1;
		}
		if(TTn_(tkc) == TT_TYPEN) {
			DBG2_ASSERT(IS_String(DP(TK0(tc))->text));
			if(knh_String_equals(DP(TK0(tc))->text, STEXT("Unsafe"))) {
				knh_Token_setUnsafeType(TK0(tc), 1);
			}
		}
		goto L_TAIL;
	}

	case TT_SEMICOLON:
		knh_tokens_nextStmt(tc);
		goto L_TAIL;

	case TT_BRACE:  /* BRACE */
	case TT_PARENTHESIS:  /* PARENTHESIS */
	case TT_BRANCET:  /* BRANCET */
	case TT_DOTS:  /* DOTS */
	case TT_NOT:  /* Not */
	case TT_LOR:  /* Lor */
	case TT_ADD:  /* Add */
	case TT_SUB:  /* Sub */
	case TT_LAND:  /* Land */
	case TT_LNOT:  /* Lnot */
	case TT_XOR:  /* Xor */
	case TT_NEXT:  /* Next */
	case TT_PREV:  /* Prev */
	case TT_NEG:  /* Neg */
	case TT_NUM:  /* NUM */
	case TT_STR:  /* STR */
	case TT_TSTR:  /* TSTR */
	case TT_ESTR:  /* EVAL */
	case TT_PROPN:  /* PROPN */
	case TT_URN:  /* URN */
	case TT_MT:  /* MT */
	case TT_CONSTN:  /* CONSTN */
	case TT_TYPEN:  /* TYPEN */
	case TT_CMETHODN:  /* CMETHODN */
	case TT_CCONSTN:
	case TT_NAME:  /* NAME */
	case TT_MN:  /* MN */
	case TT_FN:  /* FN */
	case TT_CID:  /* CID */
	case TT_MPR:  /* MPR */
		return new_StmtTYPEEXPR(ctx, tc, isData);
	}
	return new_StmtNOTDATA(ctx, tc->ts[c]);
}

/* ------------------------------------------------------------------------ */
/* [STMT1] */

static knh_Stmt_t *new_StmtSTMT1(Ctx *ctx, knh_tkc_t *tc, int isData)
{
	knh_Token_t *tk0 = NULL;
	tc->meta = -1;
	if(isData) return new_StmtDATA(ctx, tc, isData);

	L_TAIL:;
	if(!(HAS_TOKEN(tc))) {
		DBG2_P("c = %d, e = %d", tc->c, tc->e);
		return new_Stmt(ctx, 0, STT_DONE);
	}

	tk0 = TK0(tc);
	tc->c += 1;
	if(knh_Token_isDEBUG(tk0)) {
		knh_Stmt_t *stmt = new_StmtBLOCK(ctx, tc);
		knh_Stmt_setDEBUG(stmt, 1);
		return stmt;
	}

	switch(TT_(tk0)) {
	case TT_LABEL: case TT_DOC:
	{
		if(tc->meta == -1) tc->meta = tc->c - 1;
		goto L_TAIL;
	}
	case TT_METAN:
	{
		if(tc->meta == -1) tc->meta = tc->c - 1;
		//DBG2_P("Found METAN '@%s', tc->meta=%d", sToken(tkc), tc->meta);
		if(TTn_(tk0) == TT_PARENTHESIS) {
			tc->c += 1;
		}
		if(TTn_(tk0) == TT_BRACE) {
			//DBG2_P("Found DATA '@%s', tc->meta=%d", sToken(tkc), tc->meta);
			tc->c += 1;
		}
		goto L_TAIL;
	}

	case TT_NAMESPACE:
		return new_StmtNAMESPACE(ctx, tc);
	case TT_IMPORT:
		return new_StmtIMPORT(ctx, tc);
	case TT_PRAGMA:
		konohac_pragma(ctx, tc);
		return new_Stmt(ctx, 0, STT_DONE);
	case TT_USING:
		return new_StmtUSING(ctx, tc);
	case TT_WITH:
		break;  /* NOTHERE */
	case TT_CLASS:
		return new_StmtCLASS(ctx, tc);
	case TT_EXTENDS:
		break;  /* NOTHERE */
	case TT_IMPLEMENTS:
		break;  /* NOTHERE */
	case TT_FUNCTION:
		if(TT_(TK0(tc)) == TT_NAME) {
			return new_StmtMETHOD(ctx, knh_tokens_prev(tc));
		}
		else {
			return new_StmtFUNCTION(ctx, knh_tokens_prev(tc));
		}
	case TT_FORMAT:
		return new_StmtFORMAT(ctx, tc);
	case TT_RETURN:
		return new_StmtRETURN(ctx, tc);
	case TT_MAPMAP:
		return new_StmtMAPMAP(ctx, tc);
	case TT_WEAVE:
		return new_StmtWEAVE(ctx, tc);
	case TT_ASPECT:
		return new_StmtASPECT(ctx, tc);
	case TT_IF:
		return new_StmtIF(ctx, tc);
	case TT_ELSE:
		break;  /* NOTHERE */
	case TT_SWITCH:
		return new_StmtSWITCH(ctx, tc);
	case TT_CASE:
		break;   /* NOTHERE */
	case TT_WHILE:
		return new_StmtWHILE(ctx, tc);
	case TT_DO:
		return new_StmtDO(ctx, tc);
	case TT_FOR:
		return new_StmtFOR(ctx, tc);
	case TT_FOREACH:
		return new_StmtFOREACH(ctx, tc);
	case TT_BREAK:
		return new_StmtBREAK(ctx, tc);
	case TT_CONTINUE:
		return new_StmtCONTINUE(ctx, tc);
	case TT_TRY:
		return new_StmtTRY(ctx, tc);
	case TT_CATCH:
		break;  /* NOTHERE */
	case TT_FINALLY:
		break;  /* NOTHERE */
	case TT_THROW:
		return new_StmtTHROW(ctx, tc);
	case TT_REGISTER:
		return new_StmtREGISTER(ctx, tc);
	case TT_PRINT:
		return new_StmtPRINT(ctx, tc);
	case TT_ASSERT:
		return new_StmtASSERT(ctx, tc);
	case TT_TEST:
		return new_StmtTEST(ctx, tc);
	case TT_SEMICOLON:
		knh_tokens_nextStmt(tc);
		DBG2_P("EMPTY STATEMENT tc->c=%d, tc->e=%d", tc->c, tc->e);
		return new_Stmt(ctx, 0, STT_DONE);

	case TT_BRACE:  /* BRACE */
	{
		knh_tokens_prev(tc);
		if(knh_Token_isBLOCK(ctx, TK0(tc))) {
			return new_StmtBLOCK(ctx, tc);
		}
		else {
			return new_StmtLETEXPR(ctx, tc, isData);
		}
	}

	case TT_PARENTHESIS:  /* PARENTHESIS */
	case TT_BRANCET:  /* BRANCET */
	case TT_DOTS:  /* DOTS */
	case TT_NOT:  /* Not */
	case TT_LOR:  /* Lor */
	case TT_ADD:  /* Add */
	case TT_SUB:  /* Sub */
	case TT_LAND:  /* Land */
	case TT_LNOT:  /* Lnot */
	case TT_XOR:  /* Xor */
	case TT_NEXT:  /* Next */
	case TT_PREV:  /* Prev */
	case TT_NEG:  /* Neg */
	case TT_FROM:
	case TT_NUM:  /* NUM */
	case TT_STR:  /* STR */
	case TT_TSTR:  /* TSTR */
	case TT_ESTR:  /* EVAL */
	case TT_PROPN:  /* PROPN */
	case TT_URN:  /* URN */
	case TT_MT:  /* MT */
	case TT_CONSTN:  /* CONSTN */
	case TT_CMETHODN:  /* CMETHODN */
	case TT_CCONSTN:
	case TT_NAME:  /* NAME */
	case TT_MN:  /* MN */
	case TT_FN:  /* FN */
	case TT_CID:  /* CID */
	case TT_MPR:  /* MPR */
		return new_StmtLETEXPR(ctx, knh_tokens_prev(tc), isData);

	case TT_TYPEN:  /* TYPEN */
		return new_StmtTYPEEXPR(ctx, knh_tokens_prev(tc), isData);

	case TT_COMMA:
	case TT_LET:
	case TT_LSHIFTE:
	case TT_RSHIFTE:
	case TT_ADDE:
	case TT_SUBE:
	case TT_MULE:
	case TT_DIVE:
	case TT_MODE:
	case TT_BITORE:
	case TT_BITANDE:
	case TT_ALTLET:
	case TT_QUESTION:
	case TT_ITR:
	case TT_COLON:
	case TT_ALT:
	case TT_OR:
	case TT_AND:
	case TT_INSTANCEOF:
	case TT_AS:
	case TT_HAS:
	case TT_INTO:
	case TT_WHERE:
	case TT_TO:
	case TT_UNTIL:
	case TT_EQ:
	case TT_NEQ:
	case TT_LT:
	case TT_LTE:
	case TT_GT:
	case TT_GTE:
	case TT_MATCH:
	case TT_LSHIFT:
	case TT_RSHIFT:
	case TT_DIV:  /* Div */
	case TT_MOD:  /* Mod */
	case TT_MUL:  /* Mul */
		break; /* NOTHERE */

	case TT_ERR:
		return new_StmtERR(ctx, knh_tokens_prev(tc));

	case TT_EOT:  /* EOT */
	case TT_CONST:  /* CONST */
	case TT_DEFVAL:  /* DEFVAL */
	case TT_SYSVAL:  /* SYSVAL */
	case TT_LOCAL:
	case TT_STACK:  /* STACK */
	case TT_FIELD:  /* FIELD */
	case TT_SCRIPT:  /* GLOBAL */
	case TT_MEMBER:  /* MEMBER */
	case TT_CLASSID:  /* CLASSID */
	case TT_ASIS:  /* ASIS */
	case TT_NOP:  /* NOP */
		break;
	}
	tc->c -= 1;
	{
		knh_Token_t *tk = TK0(tc);
		knh_Token_perror(ctx, tk, KERR_ERROR, "don't use '%s' HERE", sToken(tk));
		return new_StmtERR(ctx, tc);
	}
}

/* ------------------------------------------------------------------------ */

knh_Stmt_t *new_StmtSTMTBLOCK(Ctx *ctx, knh_tkc_t *tc, int isData)
{
	knh_Stmt_t *stmtFIRST = NULL, *stmtLAST = NULL;
	int prev = 0;
	while(HAS_TOKEN(tc)) {
		//DBG2_P("S c = %d, e = %d", (int)tc->c, (int)tc->e);
		if(TT_(TK0(tc)) == TT_SEMICOLON) {
			tc->c += 1;
			continue;
		}
		prev = tc->c;
		if(stmtFIRST == NULL) {
			stmtFIRST = new_StmtSTMT1(ctx, tc, isData);
			stmtLAST = knh_Stmt_tail(ctx, stmtFIRST);
		}
		else {
			KNH_SETv(ctx, DP(stmtLAST)->next, new_StmtSTMT1(ctx, tc, isData));
			stmtLAST = knh_Stmt_tail(ctx, DP(stmtLAST)->next);
		}
		if(prev == tc->c) { /* infinate loop */
			DBG_P("Infinate loop? prev = %d, c = %d, e = %d", prev, tc->c, tc->e);
			DBG2_ABORT();
			break;
		}
	}
	if(stmtFIRST == NULL) {
		stmtFIRST = new_Stmt(ctx, 0, STT_DONE);
	}
	return stmtFIRST;
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
