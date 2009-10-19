#ifndef COMMONS_H_
#define COMMONS_H_

#define KONOHA_MACROS
#include"../../include/konoha.h"
#include"../../include/konoha/konoha_stmt_.h"
#include"../../include/konoha/konoha_code_.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */

#undef IS_Token
#define IS_Token(o)      (((knh_Token_t*)o)->h.cid == CLASS_Token)
#define TOKENNo(o)        knh_token_tochar(SP(o)->tt)

#undef IS_Stmt
#define IS_Stmt(o)       (((knh_Stmt_t*)o)->h.cid == CLASS_Stmt)
//#define STMTNo(o)         knh_stmt_tochar(SP(o)->stt)
#define cSTT_(o)          knh_stmt_tochar(SP(o)->stt)


#define knh_token_isNested(tt)  \
	(tt == TT_BRACE || tt == TT_PARENTHESIS || tt == TT_BRANCET)

#define TT_(tk)      SP(tk)->tt
#define STT_(stmt)   SP(stmt)->stt

#define IS_EMPTY(tc)   ((tc)->c >= (tc)->e)
#define HAS_TOKEN(tc)    ((tc)->c < (tc)->e)

#define TK0(tc)          (tc)->ts[(tc)->c]
#define TK1(tc)          (tc)->ts[((tc)->c)+1]
#define knh_Stmt_addT(ctx, stmt, tk)    knh_Stmt_add(ctx, stmt, TM(tk))
#define IST_(tk, t)   knh_String_equals(DP(tk)->text, STEXT(t))
#define TT_IN TT_HAS
#define TTn_(tk)         DP(tk)->tt_next

/* ------------------------------------------------------------------------ */

#define FL(o)            ((Object*)o)
#define TM(o)            ((Term*)o)

/* error */
#define KERR_ERROR     0
#define KERR_TERROR    1
/* warning */
#define KERR_EWARN     2
#define KERR_DWARN     3
#define KERR_ERRATA    4
/* info */
#define KERR_BAD       5
#define KERR_TINFO     6
#define KERR_NOTICE    7
#define KERR_INFO      8

#define _(s)           s

/* ------------------------------------------------------------------------ */

#define TWARN_  0
#define TCHECK_ 1
#define TCONV_  2
#define TITERCONV_ 3


#define SWITCH_IT       2
#define SEPARATOR_IT    2
#define FOREACH_ITR     (FOREACH_loop+1)
#define FOREACH_SPR     (FOREACH_loop+2)
#define TRY_HDR         3

/* ------------------------------------------------------------------------ */

#define OPCODE_LABEL  (OPCODE_NOP+1)

/* ------------------------------------------------------------------------ */

#if defined(KNH_USING_THREADEDCODE)
#define TADDR   NULL,
#else
#define TADDR   ""
#endif/*KNH_USING_THREADEDCODE*/

#define KNH_ASM(T, ...) { \
		klr_##T##_t op_ = {TADDR OPCODE_##T, ## __VA_ARGS__, NULL}; \
		knh_asmop(ctx, (knh_inst_t*)(&op_)); \
	}\

/* ------------------------------------------------------------------------ */


#ifdef __cplusplus
}
#endif

#endif /*COMMONS_H_*/
