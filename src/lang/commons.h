#ifndef COMMONS_H_
#define COMMONS_H_

#define K_INTERNAL
#include"../../include/konoha.h"
#include"../../include/konoha/konoha_code_.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ======================================================================== */

#define TT_CODE TT_EOT

#define knh_stmt_isExpr(stt)   (STT_LET <= stt && stt <= STT_CALL1)
char* TT_tochar(knh_term_t tt);

#define knh_Term_setLine(tm, tm2) \
	((knh_Term_t*)tm)->line = ((knh_Term_t*)tm2)->line;\
	((knh_Term_t*)tm)->uri  = ((knh_Term_t*)tm2)->uri;\

#define TK_tobytes(tk)           S_tobytes(DP(tk)->text)

#define TK_typing(tk, tt0, type0, index0) { \
		DBG_ASSERT(type0 != TYPE_var);\
		TT_(tk) = tt0;\
		SP(tk)->type = type0;\
		DP(tk)->index = index0;\
	}\

#define _NOTWARN     K_FLAG_T0
#define _NOTCHECK    K_FLAG_T1
#define _BCHECK      K_FLAG_T2
#define _NOTCAST     K_FLAG_T3
#define _ICAST       K_FLAG_T4

#define TYPING(ctx, stmt, n, reqt, mode) \
	if(!TERMs_typing(ctx, stmt, n, reqt, mode)) {\
		return NULL;\
	}\

#define ADD_FLAG(flag, T, F) \
	if(knh_DictMap_getNULL(ctx, DP(o)->metaDictMap, STEXT(T)) != NULL) {\
		flag |= F;\
	}\

#define isSINGLEFOREACH(stmt)   (DP(stmt)->size == 3)

#define TERMs_isCONST(stmt,n)   (TT_(DP(stmt)->terms[n]) == TT_CONST)
#define TERMs_isASIS(stmt,n)    (TT_(DP(stmt)->terms[n]) == TT_ASIS)
#define TERMs_isCID(stmt,n)     (TT_(DP(stmt)->terms[n]) == TT_CID)
#define TERMs_const(stmt, n)    DP(DP(stmt)->tokens[n])->data
#define TERMs_isNULL(stmt, n)   (TERMs_isCONST(stmt, n) && IS_NULL(TERMs_const(stmt, n)))
#define TERMs_isTRUE(stmt, n)   (TERMs_isCONST(stmt, n) && IS_TRUE(TERMs_const(stmt, n)))
#define TERMs_isFALSE(stmt, n)  (TERMs_isCONST(stmt, n) && IS_FALSE(TERMs_const(stmt, n)))
#define TERMs_gettype(stmt, n)  SP(DP(stmt)->terms[n])->type
#define TERMs_getcid(stmt, n)   CLASS_type(TERMs_gettype(stmt, n))
#define TERMs_getbcid(stmt, n)  ctx->share->ClassTable[TERMs_getcid(stmt,n)].bcid

#define knh_Gamma_setURI(ctx, uri)  SP(ctx->gma)->uri  = uri
#define knh_Gamma_setLine(ctx, n)   SP(ctx->gma)->line = n
#define knh_Stmt_flag(ctx, stmt, text, flag) knh_Stmt_flag_(ctx, stmt, STEXT(text), flag)

knh_Token_t* new_Token(Ctx *ctx, knh_flag_t flag, knh_term_t tt);
knh_Token_t *new_TokenCID(Ctx *ctx, knh_class_t cid);
knh_Token_t *new_TokenMN(Ctx *ctx, knh_methodn_t mn);
knh_class_t knh_Token_getcid(Ctx *ctx, knh_Token_t *tk, knh_class_t defc);

knh_Stmt_t* new_Stmt2(Ctx *ctx, knh_term_t stt, ...);
knh_Term_t* knh_Stmt_add_(Ctx *ctx, knh_Stmt_t *o, ...);
void knh_Stmt_swap(Ctx *ctx, knh_Stmt_t *o, size_t n, size_t m);
knh_bool_t knh_StmtMETA_is(Ctx *ctx, knh_Stmt_t *stmt, knh_bytes_t name); /* typing.c*/

knh_Term_t* knh_Token_toTYPED(Ctx *ctx, knh_Token_t *tk, knh_term_t tt, knh_type_t type, knh_short_t nn);
knh_Token_t* new_TokenTYPED(Ctx *ctx, knh_term_t tt, knh_type_t type, knh_short_t nn);

knh_uintptr_t UnitTest_newId(void);
void UnitTest_print(Ctx *ctx, knh_uintptr_t unitid, knh_bytes_t msg, int result);
void UnitTestToken(Ctx *ctx, char *stmt, int size, ...);
void TestToken(Ctx *ctx);

/* ======================================================================== */

//#define StmtCLASS_class(stmt)           DP(stmt)->tokens[0]
//#define CLASS_class                     0
//#define StmtCLASS_superclass(stmt)      DP(stmt)->tokens[1]
//#define CLASS_superclass                1
//#define StmtCLASS_interface(stmt)       DP(stmt)->stmts[2]
//#define CLASS_interface                 2
//#define StmtCLASS_instmt(stmt)          DP(stmt)->stmts[3]
//#define CLASS_instmt                    3
//
//#define StmtMETHOD_rtype(stmt)          DP(stmt)->tokens[0]
//#define METHOD_rtype                    0
//#define StmtMETHOD_class(stmt)          DP(stmt)->tokens[1]
//#define METHOD_class                    1
//#define DP(stmt)->stmts[2/*method*/]         DP(stmt)->tokens[2]
//#define METHOD_method                   2
//#define StmtMETHOD_params(stmt)         DP(stmt)->stmts[3]
//#define METHOD_params                   3
//#define StmtMETHOD_instmt(stmt)         DP(stmt)->stmts[4]
//#define METHOD_instmt                   4
//
//#define StmtFOR_init(stmt)              DP(stmt)->terms[0]
//#define FOR_init                        0
//#define StmtFOR_bool(stmt)              DP(stmt)->terms[1]
//#define FOR_bool                        1
//#define StmtFOR_redo(stmt)              DP(stmt)->terms[2]
//#define FOR_redo                        2
//#define StmtFOR_loop(stmt)              DP(stmt)->stmts[3]
//#define FOR_loop                        3
//
///* ======================================================================== */
///* [foreach] */
//#define StmtFOREACH_decls(stmt)         DP(stmt)->stmts[0]
//#define FOREACH_decls                   0
//#define StmtFOREACH_iter(stmt)          DP(stmt)->terms[1]
//#define FOREACH_iter                    1
//#define StmtFOREACH_where(stmt)         DP(stmt)->terms[2]
//#define FOREACH_where                   2
//#define StmtFOREACH_loop(stmt)          DP(stmt)->stmts[3]
//#define FOREACH_loop                    3
//
//#define StmtTRY_try(stmt)               DP(stmt)->stmts[0]
//#define TRY_try                         0
//#define StmtTRY_catch(stmt)             DP(stmt)->stmts[1]
//#define TRY_catch                       1
//#define StmtTRY_finally(stmt)           DP(stmt)->stmts[2]
//#define TRY_finally                     2

/* ------------------------------------------------------------------------ */

#define TM(o)             ((knh_Term_t*)o)
#define cSTT_(o)          TT_tochar(STT_(o))

#define knh_token_isNested(tt)  \
	(tt == TT_BRACE || tt == TT_PARENTHESIS || tt == TT_BRANCET)

#define TT_(tk)        SP(tk)->tt
#define TT_isSTR(tt)   (tt == TT_STR || tt == TT_TSTR || tt == TT_ESTR)
#define TT_isBINARY(tt) (TT_EXISTS < (tt) && (tt) <= TT_LNOT)

#define STT_(stmt)   SP(stmt)->stt
#define knh_Stmt_add(ctx, stmt, ...)   knh_Stmt_add_(ctx, stmt, ## __VA_ARGS__, NULL)

#define TK1(tc)          (tc)->ts[((tc)->c)+1]
#define IST_(tk, t)   S_equals(DP(tk)->text, STEXT(t))
#define TTn_(tk)         DP(tk)->tt_next

/* ------------------------------------------------------------------------ */

#define FL(o)            ((Object*)o)

#define knh_style(ctx, score)    knh_foundKonohaStyle(ctx, score)

#define _(s)           s

/* ------------------------------------------------------------------------ */

#define SEPARATOR_IT    2
#define FOREACH_ITR     (FOREACH_loop+1)
#define FOREACH_SPR     (FOREACH_loop+2)
#define TRY_HDR         3
#define TEST_IT         2

/* ------------------------------------------------------------------------ */

#define METHOD_getSize  MN_toGETTER(FN_size)

#if defined(K_USING_THREADEDCODE)
#define TADDR   NULL, 0/*counter*/,
#else
#define TADDR   0/*counter*/,
#endif/*K_USING_THREADEDCODE*/

#define ASMLINE  ,0

#define KNH_ASM(T, ...) { \
		klr_##T##_t op_ = {TADDR OPCODE_##T ASMLINE, ## __VA_ARGS__}; \
		knh_asmop(ctx, (knh_opset_t*)(&op_), sizeof(klr_##T##_t)); \
	}\

/* ------------------------------------------------------------------------ */


#ifdef __cplusplus
}
#endif

#endif /*COMMONS_H_*/
