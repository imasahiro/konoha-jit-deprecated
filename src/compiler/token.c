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
/* [constructors] */

knh_Token_t* new_Token(Ctx *ctx, knh_flag_t flag, knh_uri_t uri, knh_sline_t line, knh_token_t tt)
{
	knh_Token_t *o = (knh_Token_t*)new_Object_bcid(ctx, CLASS_Token, 0);
	SP(o)->flag =   flag;
	SP(o)->uri =    uri;
	SP(o)->line =   line;
	SP(o)->tt   =   tt;
	return o;
}

/* ------------------------------------------------------------------------ */

void knh_Token_setFL(knh_Token_t *o, Any *fln)
{
	if(IS_Token(fln)) {
		knh_Token_t *tk = (knh_Token_t*)fln;
		SP(o)->uri =  SP(tk)->uri;
		SP(o)->line =  SP(tk)->line;
	}
	else if(IS_Gamma((Object*)fln)) {
		knh_Gamma_t *abr = (knh_Gamma_t*)fln;
		SP(o)->uri =  SP(abr)->uri;
		SP(o)->line =  SP(abr)->line;
	}
	else {
		knh_Stmt_t *stmt = (knh_Stmt_t*)fln;
		KNH_ASSERT(IS_Stmt(stmt));
		SP(o)->uri =  SP(stmt)->uri;
		SP(o)->line =  SP(stmt)->line;
	}
}

/* ------------------------------------------------------------------------ */

knh_Token_t *new_TokenASIS(Ctx *ctx, Any *fln)
{
	knh_Token_t *o = (knh_Token_t*)new_Object_bcid(ctx, CLASS_Token, 0);
	SP(o)->flag = 0;
	TT_(o) = TT_ASIS;
	knh_Token_setFL(o, fln);
	return o;
}

/* ------------------------------------------------------------------------ */

knh_Token_t *new_TokenCID(Ctx *ctx, Any *fln, knh_class_t cid)
{
	knh_Token_t *o = (knh_Token_t*)new_Object_bcid(ctx, CLASS_Token, 0);
	SP(o)->flag = 0;
	SP(o)->tt = TT_CID;
	knh_Token_setFL(o, fln);
	DP(o)->cid = cid;
	return o;
}

/* ------------------------------------------------------------------------ */

knh_Token_t *new_TokenMN(Ctx *ctx, Any *fln, knh_methodn_t mn)
{
	knh_Token_t *o = (knh_Token_t*)new_Object_bcid(ctx, CLASS_Token, 0);
	SP(o)->flag = 0;
	SP(o)->tt = TT_MN;
	knh_Token_setFL(o, fln);
	DP(o)->mn = mn;
	return o;
}

/* ------------------------------------------------------------------------ */

knh_Token_t *new_TokenFN(Ctx *ctx, Any *fln, knh_fieldn_t fn)
{
	knh_Token_t *o = (knh_Token_t*)new_Object_bcid(ctx, CLASS_Token, 0);
	SP(o)->flag = 0;
	SP(o)->tt = TT_FN;
	knh_Token_setFL(o, fln);
	DP(o)->fn = fn;
	return o;
}

/* ======================================================================== */
/* [Array] */

#define knh_token_isNested(tt)  (tt == TT_BRACE || tt == TT_PARENTHESIS || tt == TT_BRANCET)

/* ------------------------------------------------------------------------ */

knh_tkc_t* knh_Token_tc(Ctx *ctx, knh_Token_t *o, knh_tkc_t *tc)
{
	DBG2_ASSERT(knh_token_isNested(SP(o)->tt));
	tc->meta = 0;
	tc->c = 0;
	if(IS_Array(DP(o)->data)) {
		tc->ts = (knh_Token_t**)knh_Array_list((knh_Array_t*)DP(o)->data);
		tc->e = knh_Array_size(((knh_Array_t*)DP(o)->data));
	}
	else if(IS_NULL(DP(o)->data)) {
		tc->ts = NULL;
		tc->e = 0;
	}
	else{
		KNH_ASSERT(IS_Token(DP(o)->data));
		tc->ts = &(DP(o)->token);
		tc->e = 1;
	}
	return tc;
}

/* ------------------------------------------------------------------------ */

void knh_Token_tokens_add(Ctx *ctx, knh_Token_t *o, knh_Token_t *tk)
{
	KNH_ASSERT(knh_token_isNested(SP(o)->tt));
	KNH_ASSERT(IS_Token(tk));
	if(IS_NULL(DP(o)->data)) {
		KNH_SETv(ctx, DP(o)->data, tk);
		return ;
	}
	if(IS_Token(DP(o)->data)) {
		knh_Array_t *a = new_Array(ctx, CLASS_Any, 2);
		knh_Array_add(ctx, a, DP(o)->data);
		KNH_SETv(ctx, DP(o)->data, a);
	}
	KNH_ASSERT(IS_Array(DP(o)->data));
	knh_Array_add(ctx, (knh_Array_t*)DP(o)->data, UP(tk));
}

/* ------------------------------------------------------------------------ */

void knh_Token_tokens_empty(Ctx *ctx, knh_Token_t *o)
{
	KNH_ASSERT(knh_token_isNested(SP(o)->tt));
	KNH_SETv(ctx, DP(o)->data, KNH_NULL);
}


/* ======================================================================== */
/* [movabletext] */

char *knh_Token__tochar(Ctx *ctx, knh_Token_t *o)
{
	KNH_ASSERT(IS_Token(o));
	if(SP(o)->tt == TT_ASIS) { return "_"; }
	if(SP(o)->tt < TT_NUM) {
		return knh_token_tochar(SP(o)->tt);
	}
	if(IS_String(DP(o)->text)) {
		return __tochar(DP(o)->text);
	}
	if(SP(o)->tt == TT_CID) {
		return CLASSN(DP(o)->cid);
	}
	if(SP(o)->tt == TT_FN) {
		return FIELDN(DP(o)->fn);
	}
	if(IS_NULL(DP(o)->data)) {
		return "null";
	}
	return knh_token_tochar(TT_(o));
}

/* ------------------------------------------------------------------------ */

knh_bytes_t knh_Token_tobytes(Ctx *ctx, knh_Token_t *o)
{
	return new_bytes(sToken(o));
}


/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
