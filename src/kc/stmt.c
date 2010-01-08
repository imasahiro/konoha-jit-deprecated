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

knh_Stmt_t* new_Stmt(Ctx *ctx, knh_flag_t flag, knh_stmt_t stt)
{
	knh_Stmt_t *o = (knh_Stmt_t*)new_Object_bcid(ctx, CLASS_Stmt, 0);
	SP(o)->uri = 0;
	SP(o)->line = 0;
	SP(o)->flag = flag;
	SP(o)->stt  = stt;
	return o;
}

/* ------------------------------------------------------------------------ */

knh_Stmt_t* knh_Stmt_copy(Ctx *ctx, knh_Stmt_t *stmt)
{
	size_t i;
	knh_Stmt_t *o = new_Stmt(ctx, SP(stmt)->flag, SP(stmt)->stt);
	SP(o)->uri = SP(stmt)->uri;
	SP(o)->line = SP(stmt)->line;
	DP(o)->type = DP(stmt)->type;
	DP(o)->size = 0;
	for(i = 0; i < DP(stmt)->size; i++) {
		knh_Stmt_add(ctx, o, DP(stmt)->terms[i]);
	}
	//KNH_SETv(ctx, DP(o)->metaDictMap, DP(stmt)->metaDictMap);
	return o;
}

/* ======================================================================== */
/* [ERR] */

void knh_Stmt_toERR(Ctx *ctx, knh_Stmt_t *stmt, Term *tm)
{
	if(SP(stmt)->stt == STT_ERR) return;
	SP(stmt)->stt = STT_ERR;
	if(tm == NULL) {

	}
	else if(IS_Token(tm)) {
		SP(stmt)->uri =  SP((knh_Token_t*)tm)->uri;
		SP(stmt)->line =  SP((knh_Token_t*)tm)->line;
	}
	else if(IS_Stmt(tm)) {
		SP(stmt)->uri =  SP((knh_Stmt_t*)tm)->uri;
		SP(stmt)->line =  SP((knh_Stmt_t*)tm)->line;
	}
	{
		char buf[256];
		knh_snprintf(buf, sizeof(buf), _("Script!!: you'll have to fix bugs at %s:%d"),
			FILEN(SP(stmt)->uri), SP(stmt)->line);
		KNH_SETv(ctx, DP(stmt)->errMsg, new_String(ctx, B(buf), NULL));
		KNH_SETv(ctx, DP(stmt)->next, KNH_NULL);
	}
}

/* ======================================================================== */
/* [terms] */

static void knh_Stmt_terms_expand(Ctx *ctx, knh_Stmt_t *o)
{
	size_t i, newcapacity = DP(o)->capacity * 2;
	if(newcapacity == 0) newcapacity = (KNH_FASTMALLOC_SIZE / sizeof(knh_Object_t*));
	Object **newterms = (Object**)KNH_MALLOC(ctx, sizeof(Object*) * newcapacity);
	knh_memcpy(newterms, DP(o)->terms, sizeof(Object*) * DP(o)->capacity);
	for(i = DP(o)->capacity; i < newcapacity; i++) {
		KNH_INITv(newterms[i], KNH_NULL);
	}
	if(DP(o)->capacity != 0) {
		KNH_FREE(ctx, DP(o)->terms, sizeof(Object*) * DP(o)->capacity);
	}
	DP(o)->terms = newterms;
	DP(o)->capacity = newcapacity;
}

/* ------------------------------------------------------------------------ */

void knh_Stmt_add(Ctx *ctx, knh_Stmt_t *o, Term *tm)
{
	DBG2_ASSERT(IS_Token(tm) || IS_Stmt(tm));
	DBG2_ASSERT(SP(o)->stt != STT_DONE);
	if(!(DP(o)->size < DP(o)->capacity)) {
		knh_Stmt_terms_expand(ctx, o);
	}
	KNH_SETv(ctx, DP(o)->terms[DP(o)->size], tm);
	DP(o)->size++;
	if(IS_Stmt(tm)) {
		knh_Stmt_t *stmt = (knh_Stmt_t*)tm;
		if(SP(o)->line == 0) {
			SP(o)->line = SP(stmt)->line;
			SP(o)->uri = SP(stmt)->uri;
		}
		if(SP(stmt)->stt == STT_ERR) {
			knh_Stmt_toERR(ctx, o, tm);
		}
	}
	else {
		knh_Token_t *tk = (knh_Token_t*)tm;
		DBG2_ASSERT(IS_Token(tk));
		if(SP(o)->line == 0) {
			SP(o)->line = SP(tk)->line;
			SP(o)->uri = SP(tk)->uri;
		}
		if(SP(tk)->tt == TT_ERR) {
			knh_Stmt_toERR(ctx, o, tm);
		}
	}
}

/* ------------------------------------------------------------------------ */

void knh_Stmt_resize(Ctx *ctx, knh_Stmt_t *o, size_t n)
{
	size_t i;
	DBG2_ASSERT(n <= DP(o)->size);
	for(i = n; i < DP(o)->size; i++) {
		KNH_SETv(ctx, DP(o)->terms[n], KNH_NULL);
	}
	DP(o)->size = n;
}

/* ------------------------------------------------------------------------ */

void knh_Stmt_swap(Ctx *ctx, knh_Stmt_t *o, size_t n, size_t m)
{
	DBG2_ASSERT(n < DP(o)->size);
	DBG2_ASSERT(m < DP(o)->size);
	Term *tm = DP(o)->terms[n];
	DP(o)->terms[n] = DP(o)->terms[m];
	DP(o)->terms[m] = tm;
}

/* ------------------------------------------------------------------------ */

void knh_Stmt_insert(Ctx *ctx, knh_Stmt_t *o, size_t n, Term *tm)
{
	int i = DP(o)->size;
	DBG2_ASSERT(n <= DP(o)->size);
	knh_Stmt_add(ctx, o, tm); // DP(o)->size++;
	for(; n < i ; i--) {
		knh_Stmt_swap(ctx, o, i - 1, i);
	}
}

/* ======================================================================== */
/* [visit] */

knh_Stmt_t *knh_Stmt_tail(Ctx *ctx, knh_Stmt_t *o)
{
	knh_Stmt_t *tail = o;
	while(IS_NOTNULL(DP(tail)->next)) {
		tail = DP(tail)->next;
	}
	return tail;
}

/* ------------------------------------------------------------------------ */

knh_Stmt_t* knh_StmtNULL_tail_append(Ctx *ctx, knh_Stmt_t *o, knh_Stmt_t *stmt)
{
	if(o == NULL) {
		return stmt;
	}
	else {
		knh_Stmt_t *tail = knh_Stmt_tail(ctx, o);
		KNH_SETv(ctx, DP(tail)->next, stmt);
		return o;
	}
}

/* ------------------------------------------------------------------------ */
/* [META] */

knh_bool_t knh_Stmt_hasMeta(knh_Stmt_t *o)
{
	return (IS_bDictMap(DP(o)->metaDictMap));
}

/* ------------------------------------------------------------------------ */

knh_flag_t knh_StmtMETHOD_flag(Ctx *ctx, knh_Stmt_t *o)
{
	knh_flag_t flag = 0;
	if(IS_DictMap(DP(o)->metaDictMap)) {
		Object *v = knh_DictMap_get__b(ctx, DP(o)->metaDictMap, STEXT("Virtual"));
		if(IS_NOTNULL(v)) {
			flag |= FLAG_Method_Virtual;
		}
		v = knh_DictMap_get__b(ctx, DP(o)->metaDictMap, STEXT("Abstract"));
		if(IS_NOTNULL(v)) {
			flag |= FLAG_Method_Virtual;
		}
		v = knh_DictMap_get__b(ctx, DP(o)->metaDictMap, STEXT("Private"));
		if(IS_NOTNULL(v)) {
			flag |= FLAG_Method_Private;
		}
		v = knh_DictMap_get__b(ctx, DP(o)->metaDictMap, STEXT("Const"));
		if(IS_NOTNULL(v)) {
			flag |= FLAG_Method_Const;
		}
		v = knh_DictMap_get__b(ctx, DP(o)->metaDictMap, STEXT("Static"));
		if(IS_NOTNULL(v)) {
			flag |= FLAG_Method_Static;
		}
		v = knh_DictMap_get__b(ctx, DP(o)->metaDictMap, STEXT("Debug"));
		if(IS_NOTNULL(v)) {
			flag |= FLAG_Method_Debug;
		}
		v = knh_DictMap_get__b(ctx, DP(o)->metaDictMap, STEXT("Inline"));
		if(IS_NOTNULL(v)) {
			flag |= FLAG_Method_Inline;
		}
	}
	return flag;
}

/* ------------------------------------------------------------------------ */

knh_flag_t knh_StmtPRINT_flag(Ctx *ctx, knh_Stmt_t *o)
{
	knh_flag_t flag = 0;
	if(IS_bDictMap(DP(o)->metaDictMap)) {
		Object *v = knh_DictMap_get__b(ctx,  DP(o)->metaDictMap, STEXT("time"));
		if(IS_NOTNULL(v)) {
			flag |= KNH_FLAG_PF_TIME;
		}
	}
	return flag;
}

/* ======================================================================== */
/* [Annotation] */

knh_bool_t knh_StmtMETA_isOverride(Ctx *ctx, knh_Stmt_t *o)
{
	if(IS_DictMap(DP(o)->metaDictMap)) {
		Object *v = knh_DictMap_get__b(ctx, DP(o)->metaDictMap, STEXT("Override"));
		if(IS_NOTNULL(v)) return 1;
	}
	return 0;
}

/* ------------------------------------------------------------------------ */

int knh_StmtMETA_is(Ctx *ctx, knh_Stmt_t *stmt, knh_bytes_t name)
{
	if(IS_DictMap(DP(stmt)->metaDictMap)) {
		Object *v = knh_DictMap_get__b(ctx, DP(stmt)->metaDictMap, name);
		if(IS_NOTNULL(v)) return 1;
	}
	return 0;
}

/* ======================================================================== */
/* [movabletext] */


/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
