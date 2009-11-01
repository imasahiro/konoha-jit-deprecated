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
 * (1) GNU General Public License 2.0      (with    KONOHA_UNDER_GPL2)
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

#include"commons.h"


#ifdef __cplusplus 
extern "C" {
#endif

/* ======================================================================== */

/* ======================================================================== */
/* [token] */

static char* knh_token_array_tochar[] = {
    "namespace",  /* namespace */ 
    "import",  /* import */ 
    "pragma",  /* pragma */ 
    "using",  /* using */ 
    "with",  /* with */ 
    "class",  /* class */ 
    "extends",  /* extends */ 
    "implements",  /* implements */ 
    "mapmap",  /* mapmap */ 
    "function",  /* function */ 
    "format",  /* format */ 
    "return",  /* return */ 
    "weave",  /* weave */ 
    "aspect",  /* aspect */ 
    "if",  /* if */ 
    "else",  /* else */ 
    "switch",  /* switch */ 
    "case",  /* case */ 
    "while",  /* while */ 
    "do",  /* do */ 
    "for",  /* for */ 
    "foreach",  /* foreach */ 
    "break",  /* break */ 
    "continue",  /* continue */ 
    "try",  /* try */ 
    "catch",  /* catch */ 
    "finally",  /* finally */ 
    "throw",  /* throw */ 
    "test",  /* test */ 
    "register",  /* register */ 
    "print",  /* print */ 
    "assert",  /* assert */ 
    "{",  /* BRACE */ 
    "(",  /* PARENTHESIS */ 
    "[",  /* BRANCET */ 
    "...",  /* DOTS */ 
    ";",  /* SEMICOLON */ 
    ",",  /* COMMA */ 
    "=>",  /* FUNCMAP */ 
    "it",  /* IT */ 
    "=",  /* LET */ 
    "<<=",  /* lshifte */ 
    ">>=",  /* rshifte */ 
    "+=",  /* adde */ 
    "-=",  /* sube */ 
    "/=",  /* dive */ 
    "%=",  /* mode */ 
    "*=",  /* mule */ 
    "&=",  /* bitande */ 
    "|=",  /* bitore */ 
    "?\?=",  /* ALTLET */ 
    "?",  /* QUESTION */ 
    ":",  /* COLON */ 
    "..",  /* Itr */ 
    "?\?",  /* ALT */ 
    "||",  /* OR */ 
    "&&",  /* AND */ 
    "!",  /* Not */ 
    "instanceof",  /* Instanceof */ 
    "as",  /* As */ 
    "in",  /* Has */ 
    "into",  /* Into */ 
    "is",  /* Is */ 
    "===",  /* Same */ 
    "==",  /* Eq */ 
    "!=",  /* Neq */ 
    "<",  /* Lt */ 
    "<=",  /* Lte */ 
    ">",  /* Gt */ 
    ">=",  /* Gte */ 
    "=~",  /* Match */ 
    "from",  /* From */ 
    "where",  /* Where */ 
    "to",  /* To */ 
    "until",  /* Until */ 
    "<<",  /* Lshift */ 
    ">>",  /* Rshift */ 
    "+",  /* Add */ 
    "-",  /* Sub */ 
    "/",  /* Div */ 
    "%",  /* Mod */ 
    "*",  /* Mul */ 
    "&",  /* Land */ 
    "|",  /* Lor */ 
    "~",  /* Lnot */ 
    "^",  /* Xor */ 
    "++",  /* Next */ 
    "--",  /* Prev */ 
    "---",  /* Neg */ 
    "NUM",  /* NUM */ 
    "STR",  /* STR */ 
    "TSTR",  /* TSTR */ 
    "ESTR",  /* ESTR */ 
    "LABEL",  /* LABEL */ 
    "DOC",  /* DOC */ 
    "METAN",  /* METAN */ 
    "PROPN",  /* PROPN */ 
    "URN",  /* URN */ 
    "MT",  /* MT */ 
    "CONSTN",  /* CONSTN */ 
    "TYPEN",  /* TYPEN */ 
    "CMETHODN",  /* CMETHODN */ 
    "CCONSTN",  /* CCONSTN */ 
    "NAME",  /* NAME */ 
    "ERR",  /* ERR */ 
    "MN",  /* MN */ 
    "FN",  /* FN */ 
    "CID",  /* CID */ 
    "MPR",  /* MPR */ 
    "EOT",  /* EOT */ 
    "CONST",  /* CONST */ 
    "DEFVAL",  /* DEFVAL */ 
    "SYSVAL",  /* SYSVAL */ 
    "LOCAL",  /* LOCAL */ 
    "STACK",  /* STACK */ 
    "FIELD",  /* FIELD */ 
    "SCRIPT",  /* SCRIPT */ 
    "MEMBER",  /* MEMBER */ 
    "CLASSID",  /* CLASSID */ 
    "CLOSURE",  /* CLOSURE */ 
    "_",  /* ASIS */ 
    "NOP",  /* NOP */ 
    //(char*)0
};

char* knh_token_tochar(knh_token_t t)
{
    DBG2_ASSERT(t < KNH_TOKEN_MAXSIZ);
    return knh_token_array_tochar[t];
}

/* ------------------------------------------------------------------------ */

static knh_methodn_t knh_token_array_tomethodn[] = {
    METHODN_NONAME,  /* namespace */ 
    METHODN_NONAME,  /* import */ 
    METHODN_NONAME,  /* pragma */ 
    METHODN_NONAME,  /* using */ 
    METHODN_NONAME,  /* with */ 
    METHODN_NONAME,  /* class */ 
    METHODN_NONAME,  /* extends */ 
    METHODN_NONAME,  /* implements */ 
    METHODN_NONAME,  /* mapmap */ 
    METHODN_NONAME,  /* function */ 
    METHODN_NONAME,  /* format */ 
    METHODN_NONAME,  /* return */ 
    METHODN_NONAME,  /* weave */ 
    METHODN_NONAME,  /* aspect */ 
    METHODN_NONAME,  /* if */ 
    METHODN_NONAME,  /* else */ 
    METHODN_NONAME,  /* switch */ 
    METHODN_NONAME,  /* case */ 
    METHODN_NONAME,  /* while */ 
    METHODN_NONAME,  /* do */ 
    METHODN_NONAME,  /* for */ 
    METHODN_NONAME,  /* foreach */ 
    METHODN_NONAME,  /* break */ 
    METHODN_NONAME,  /* continue */ 
    METHODN_NONAME,  /* try */ 
    METHODN_NONAME,  /* catch */ 
    METHODN_NONAME,  /* finally */ 
    METHODN_NONAME,  /* throw */ 
    METHODN_NONAME,  /* test */ 
    METHODN_NONAME,  /* register */ 
    METHODN_NONAME,  /* print */ 
    METHODN_NONAME,  /* assert */ 
    METHODN_NONAME,  /* BRACE */ 
    METHODN_NONAME,  /* PARENTHESIS */ 
    METHODN_NONAME,  /* BRANCET */ 
    METHODN_NONAME,  /* DOTS */ 
    METHODN_NONAME,  /* SEMICOLON */ 
    METHODN_NONAME,  /* COMMA */ 
    METHODN_NONAME,  /* FUNCMAP */ 
    METHODN_NONAME,  /* IT */ 
    METHODN_NONAME,  /* LET */ 
    METHODN_NONAME,  /* lshifte */ 
    METHODN_NONAME,  /* rshifte */ 
    METHODN_NONAME,  /* adde */ 
    METHODN_NONAME,  /* sube */ 
    METHODN_NONAME,  /* dive */ 
    METHODN_NONAME,  /* mode */ 
    METHODN_NONAME,  /* mule */ 
    METHODN_NONAME,  /* bitande */ 
    METHODN_NONAME,  /* bitore */ 
    METHODN_NONAME,  /* ALTLET */ 
    METHODN_NONAME,  /* QUESTION */ 
    METHODN_NONAME,  /* COLON */ 
    METHODN_opItr,  /* Itr */ 
    METHODN_opALT,  /* ALT */ 
    METHODN_NONAME,  /* OR */ 
    METHODN_NONAME,  /* AND */ 
    METHODN_opNot,  /* Not */ 
    METHODN_opInstanceof,  /* Instanceof */ 
    METHODN_opAs,  /* As */ 
    METHODN_opHas,  /* Has */ 
    METHODN_opInto,  /* Into */ 
    METHODN_opIs,  /* Is */ 
    METHODN_opSame,  /* Same */ 
    METHODN_opEq,  /* Eq */ 
    METHODN_opNeq,  /* Neq */ 
    METHODN_opLt,  /* Lt */ 
    METHODN_opLte,  /* Lte */ 
    METHODN_opGt,  /* Gt */ 
    METHODN_opGte,  /* Gte */ 
    METHODN_opMatch,  /* Match */ 
    METHODN_opFrom,  /* From */ 
    METHODN_opWhere,  /* Where */ 
    METHODN_opTo,  /* To */ 
    METHODN_opUntil,  /* Until */ 
    METHODN_opLshift,  /* Lshift */ 
    METHODN_opRshift,  /* Rshift */ 
    METHODN_opAdd,  /* Add */ 
    METHODN_opSub,  /* Sub */ 
    METHODN_opDiv,  /* Div */ 
    METHODN_opMod,  /* Mod */ 
    METHODN_opMul,  /* Mul */ 
    METHODN_opLand,  /* Land */ 
    METHODN_opLor,  /* Lor */ 
    METHODN_opLnot,  /* Lnot */ 
    METHODN_opXor,  /* Xor */ 
    METHODN_opNext,  /* Next */ 
    METHODN_opPrev,  /* Prev */ 
    METHODN_opNeg,  /* Neg */ 
    METHODN_NONAME,  /* NUM */ 
    METHODN_NONAME,  /* STR */ 
    METHODN_NONAME,  /* TSTR */ 
    METHODN_NONAME,  /* ESTR */ 
    METHODN_NONAME,  /* LABEL */ 
    METHODN_NONAME,  /* DOC */ 
    METHODN_NONAME,  /* METAN */ 
    METHODN_NONAME,  /* PROPN */ 
    METHODN_NONAME,  /* URN */ 
    METHODN_NONAME,  /* MT */ 
    METHODN_NONAME,  /* CONSTN */ 
    METHODN_NONAME,  /* TYPEN */ 
    METHODN_NONAME,  /* CMETHODN */ 
    METHODN_NONAME,  /* CCONSTN */ 
    METHODN_NONAME,  /* NAME */ 
    METHODN_NONAME,  /* ERR */ 
    METHODN_NONAME,  /* MN */ 
    METHODN_NONAME,  /* FN */ 
    METHODN_NONAME,  /* CID */ 
    METHODN_NONAME,  /* MPR */ 
    METHODN_NONAME,  /* EOT */ 
    METHODN_NONAME,  /* CONST */ 
    METHODN_NONAME,  /* DEFVAL */ 
    METHODN_NONAME,  /* SYSVAL */ 
    METHODN_NONAME,  /* LOCAL */ 
    METHODN_NONAME,  /* STACK */ 
    METHODN_NONAME,  /* FIELD */ 
    METHODN_NONAME,  /* SCRIPT */ 
    METHODN_NONAME,  /* MEMBER */ 
    METHODN_NONAME,  /* CLASSID */ 
    METHODN_NONAME,  /* CLOSURE */ 
    METHODN_NONAME,  /* ASIS */ 
    METHODN_NONAME,  /* NOP */ 
    //(knh_methodn_t)0
};

knh_methodn_t knh_token_tomethodn(knh_token_t t)
{
    DBG2_ASSERT(t < KNH_TOKEN_MAXSIZ);
    return knh_token_array_tomethodn[t];
}

/* ------------------------------------------------------------------------ */

static int knh_token_array_getOpPriority[] = {
    0,  /* namespace */ 
    0,  /* import */ 
    0,  /* pragma */ 
    0,  /* using */ 
    99,  /* with */ 
    0,  /* class */ 
    0,  /* extends */ 
    0,  /* implements */ 
    0,  /* mapmap */ 
    99,  /* function */ 
    0,  /* format */ 
    0,  /* return */ 
    0,  /* weave */ 
    0,  /* aspect */ 
    0,  /* if */ 
    0,  /* else */ 
    0,  /* switch */ 
    0,  /* case */ 
    0,  /* while */ 
    0,  /* do */ 
    0,  /* for */ 
    0,  /* foreach */ 
    0,  /* break */ 
    0,  /* continue */ 
    0,  /* try */ 
    0,  /* catch */ 
    0,  /* finally */ 
    0,  /* throw */ 
    0,  /* test */ 
    0,  /* register */ 
    0,  /* print */ 
    0,  /* assert */ 
    99,  /* BRACE */ 
    99,  /* PARENTHESIS */ 
    99,  /* BRANCET */ 
    0,  /* DOTS */ 
    0,  /* SEMICOLON */ 
    0,  /* COMMA */ 
    99,  /* FUNCMAP */ 
    99,  /* IT */ 
    2,  /* LET */ 
    0,  /* lshifte */ 
    0,  /* rshifte */ 
    0,  /* adde */ 
    0,  /* sube */ 
    0,  /* dive */ 
    0,  /* mode */ 
    0,  /* mule */ 
    0,  /* bitande */ 
    0,  /* bitore */ 
    0,  /* ALTLET */ 
    3,  /* QUESTION */ 
    99,  /* COLON */ 
    3,  /* Itr */ 
    3,  /* ALT */ 
    5,  /* OR */ 
    6,  /* AND */ 
    7,  /* Not */ 
    9,  /* Instanceof */ 
    9,  /* As */ 
    9,  /* Has */ 
    9,  /* Into */ 
    9,  /* Is */ 
    9,  /* Same */ 
    9,  /* Eq */ 
    9,  /* Neq */ 
    9,  /* Lt */ 
    9,  /* Lte */ 
    9,  /* Gt */ 
    9,  /* Gte */ 
    9,  /* Match */ 
    15,  /* From */ 
    15,  /* Where */ 
    15,  /* To */ 
    15,  /* Until */ 
    15,  /* Lshift */ 
    15,  /* Rshift */ 
    20,  /* Add */ 
    20,  /* Sub */ 
    24,  /* Div */ 
    24,  /* Mod */ 
    24,  /* Mul */ 
    27,  /* Land */ 
    27,  /* Lor */ 
    27,  /* Lnot */ 
    27,  /* Xor */ 
    28,  /* Next */ 
    28,  /* Prev */ 
    28,  /* Neg */ 
    99,  /* NUM */ 
    99,  /* STR */ 
    99,  /* TSTR */ 
    99,  /* ESTR */ 
    0,  /* LABEL */ 
    0,  /* DOC */ 
    0,  /* METAN */ 
    99,  /* PROPN */ 
    99,  /* URN */ 
    99,  /* MT */ 
    99,  /* CONSTN */ 
    99,  /* TYPEN */ 
    99,  /* CMETHODN */ 
    99,  /* CCONSTN */ 
    99,  /* NAME */ 
    0,  /* ERR */ 
    99,  /* MN */ 
    99,  /* FN */ 
    99,  /* CID */ 
    99,  /* MPR */ 
    0,  /* EOT */ 
    99,  /* CONST */ 
    99,  /* DEFVAL */ 
    99,  /* SYSVAL */ 
    99,  /* LOCAL */ 
    99,  /* STACK */ 
    99,  /* FIELD */ 
    99,  /* SCRIPT */ 
    99,  /* MEMBER */ 
    99,  /* CLASSID */ 
    99,  /* CLOSURE */ 
    99,  /* ASIS */ 
    99,  /* NOP */ 
    //(int)0
};

int knh_token_getOpPriority(knh_token_t t)
{
    DBG2_ASSERT(t < KNH_TOKEN_MAXSIZ);
    return knh_token_array_getOpPriority[t];
}

/* ------------------------------------------------------------------------ */

static int knh_token_array_getOpSize[] = {
    0,  /* namespace */ 
    0,  /* import */ 
    0,  /* pragma */ 
    0,  /* using */ 
    0,  /* with */ 
    0,  /* class */ 
    0,  /* extends */ 
    0,  /* implements */ 
    0,  /* mapmap */ 
    0,  /* function */ 
    0,  /* format */ 
    0,  /* return */ 
    0,  /* weave */ 
    0,  /* aspect */ 
    0,  /* if */ 
    0,  /* else */ 
    0,  /* switch */ 
    0,  /* case */ 
    0,  /* while */ 
    0,  /* do */ 
    0,  /* for */ 
    0,  /* foreach */ 
    0,  /* break */ 
    0,  /* continue */ 
    0,  /* try */ 
    0,  /* catch */ 
    0,  /* finally */ 
    0,  /* throw */ 
    0,  /* test */ 
    0,  /* register */ 
    0,  /* print */ 
    0,  /* assert */ 
    0,  /* BRACE */ 
    0,  /* PARENTHESIS */ 
    0,  /* BRANCET */ 
    0,  /* DOTS */ 
    0,  /* SEMICOLON */ 
    0,  /* COMMA */ 
    0,  /* FUNCMAP */ 
    0,  /* IT */ 
    2,  /* LET */ 
    0,  /* lshifte */ 
    0,  /* rshifte */ 
    0,  /* adde */ 
    0,  /* sube */ 
    0,  /* dive */ 
    0,  /* mode */ 
    0,  /* mule */ 
    0,  /* bitande */ 
    0,  /* bitore */ 
    0,  /* ALTLET */ 
    0,  /* QUESTION */ 
    0,  /* COLON */ 
    1,  /* Itr */ 
    0,  /* ALT */ 
    0,  /* OR */ 
    0,  /* AND */ 
    1,  /* Not */ 
    2,  /* Instanceof */ 
    2,  /* As */ 
    2,  /* Has */ 
    2,  /* Into */ 
    2,  /* Is */ 
    2,  /* Same */ 
    2,  /* Eq */ 
    2,  /* Neq */ 
    2,  /* Lt */ 
    2,  /* Lte */ 
    2,  /* Gt */ 
    2,  /* Gte */ 
    2,  /* Match */ 
    2,  /* From */ 
    2,  /* Where */ 
    2,  /* To */ 
    2,  /* Until */ 
    0,  /* Lshift */ 
    0,  /* Rshift */ 
    0,  /* Add */ 
    2,  /* Sub */ 
    2,  /* Div */ 
    2,  /* Mod */ 
    2,  /* Mul */ 
    0,  /* Land */ 
    0,  /* Lor */ 
    1,  /* Lnot */ 
    2,  /* Xor */ 
    1,  /* Next */ 
    1,  /* Prev */ 
    1,  /* Neg */ 
    0,  /* NUM */ 
    0,  /* STR */ 
    0,  /* TSTR */ 
    0,  /* ESTR */ 
    0,  /* LABEL */ 
    0,  /* DOC */ 
    0,  /* METAN */ 
    0,  /* PROPN */ 
    0,  /* URN */ 
    0,  /* MT */ 
    0,  /* CONSTN */ 
    0,  /* TYPEN */ 
    0,  /* CMETHODN */ 
    0,  /* CCONSTN */ 
    0,  /* NAME */ 
    0,  /* ERR */ 
    0,  /* MN */ 
    0,  /* FN */ 
    0,  /* CID */ 
    0,  /* MPR */ 
    0,  /* EOT */ 
    0,  /* CONST */ 
    0,  /* DEFVAL */ 
    0,  /* SYSVAL */ 
    0,  /* LOCAL */ 
    0,  /* STACK */ 
    0,  /* FIELD */ 
    0,  /* SCRIPT */ 
    0,  /* MEMBER */ 
    0,  /* CLASSID */ 
    0,  /* CLOSURE */ 
    0,  /* ASIS */ 
    0,  /* NOP */ 
    //(int)0
};

int knh_token_getOpSize(knh_token_t t)
{
    DBG2_ASSERT(t < KNH_TOKEN_MAXSIZ);
    return knh_token_array_getOpSize[t];
}

/* ------------------------------------------------------------------------ */

static knh_bool_t knh_token_array_isBeginOfStmt[] = {
    1,  /* namespace */ 
    1,  /* import */ 
    1,  /* pragma */ 
    1,  /* using */ 
    0,  /* with */ 
    1,  /* class */ 
    0,  /* extends */ 
    0,  /* implements */ 
    0,  /* mapmap */ 
    0,  /* function */ 
    1,  /* format */ 
    1,  /* return */ 
    1,  /* weave */ 
    1,  /* aspect */ 
    1,  /* if */ 
    0,  /* else */ 
    1,  /* switch */ 
    1,  /* case */ 
    1,  /* while */ 
    1,  /* do */ 
    1,  /* for */ 
    1,  /* foreach */ 
    1,  /* break */ 
    1,  /* continue */ 
    1,  /* try */ 
    0,  /* catch */ 
    0,  /* finally */ 
    1,  /* throw */ 
    1,  /* test */ 
    1,  /* register */ 
    1,  /* print */ 
    1,  /* assert */ 
    0,  /* BRACE */ 
    0,  /* PARENTHESIS */ 
    0,  /* BRANCET */ 
    0,  /* DOTS */ 
    1,  /* SEMICOLON */ 
    0,  /* COMMA */ 
    0,  /* FUNCMAP */ 
    0,  /* IT */ 
    0,  /* LET */ 
    0,  /* lshifte */ 
    0,  /* rshifte */ 
    0,  /* adde */ 
    0,  /* sube */ 
    0,  /* dive */ 
    0,  /* mode */ 
    0,  /* mule */ 
    0,  /* bitande */ 
    0,  /* bitore */ 
    0,  /* ALTLET */ 
    0,  /* QUESTION */ 
    0,  /* COLON */ 
    0,  /* Itr */ 
    0,  /* ALT */ 
    0,  /* OR */ 
    0,  /* AND */ 
    0,  /* Not */ 
    0,  /* Instanceof */ 
    0,  /* As */ 
    0,  /* Has */ 
    0,  /* Into */ 
    0,  /* Is */ 
    0,  /* Same */ 
    0,  /* Eq */ 
    0,  /* Neq */ 
    0,  /* Lt */ 
    0,  /* Lte */ 
    0,  /* Gt */ 
    0,  /* Gte */ 
    0,  /* Match */ 
    0,  /* From */ 
    0,  /* Where */ 
    0,  /* To */ 
    0,  /* Until */ 
    0,  /* Lshift */ 
    0,  /* Rshift */ 
    0,  /* Add */ 
    0,  /* Sub */ 
    0,  /* Div */ 
    0,  /* Mod */ 
    0,  /* Mul */ 
    0,  /* Land */ 
    0,  /* Lor */ 
    0,  /* Lnot */ 
    0,  /* Xor */ 
    0,  /* Next */ 
    0,  /* Prev */ 
    0,  /* Neg */ 
    0,  /* NUM */ 
    0,  /* STR */ 
    0,  /* TSTR */ 
    0,  /* ESTR */ 
    0,  /* LABEL */ 
    0,  /* DOC */ 
    1,  /* METAN */ 
    0,  /* PROPN */ 
    0,  /* URN */ 
    0,  /* MT */ 
    0,  /* CONSTN */ 
    0,  /* TYPEN */ 
    0,  /* CMETHODN */ 
    0,  /* CCONSTN */ 
    0,  /* NAME */ 
    0,  /* ERR */ 
    0,  /* MN */ 
    0,  /* FN */ 
    0,  /* CID */ 
    0,  /* MPR */ 
    1,  /* EOT */ 
    0,  /* CONST */ 
    0,  /* DEFVAL */ 
    0,  /* SYSVAL */ 
    0,  /* LOCAL */ 
    0,  /* STACK */ 
    0,  /* FIELD */ 
    0,  /* SCRIPT */ 
    0,  /* MEMBER */ 
    0,  /* CLASSID */ 
    0,  /* CLOSURE */ 
    0,  /* ASIS */ 
    0,  /* NOP */ 
    //(knh_bool_t)0
};

knh_bool_t knh_token_isBeginOfStmt(knh_token_t t)
{
    DBG2_ASSERT(t < KNH_TOKEN_MAXSIZ);
    return knh_token_array_isBeginOfStmt[t];
}

/* ------------------------------------------------------------------------ */

/* ======================================================================== */
/* [isKeyword] */

int knh_token_isKeyword(knh_token_t tt)
{
    char *keyword = knh_token_tochar(tt);
    return islower(keyword[0]);
}


/* ======================================================================== */
/* [token parser] */


knh_Token_t *
knh_cwb_parseToken(Ctx *ctx, knh_cwb_t *cwb, knh_flag_t flag, knh_InputStream_t *in)
{
    knh_bytes_t t = knh_cwb_tobytes(cwb);
    knh_token_t tt = ((knh_token_t)-1);
    L_TAIL:;
    if(isalpha(t.buf[0])) {
        if(t.buf[t.len-2]=='.' && t.buf[t.len-1]=='*') {
            flag |= FLAG_Token_TailWildCard;
            t.len -= 2;
        }
        if(isupper(t.buf[0]) && t.len > 9 && knh_bytes_endsWith(t, STEXT("Exception"))) {
            flag |= FLAG_Token_ExceptionType;
            t.len -= 9;
        }
    }

    switch(t.buf[0]) {
    case '!':
        if(ISB(t, "!")) { tt = TT_NOT; break; }
        if(ISB(t, "!=")) { tt = TT_NEQ; break; }
    break;
    case '%':
        if(ISB(t, "%=")) { tt = TT_MODE; break; }
        if(ISB(t, "%")) { tt = TT_MOD; break; }
    break;
    case '&':
        if(ISB(t, "&=")) { tt = TT_BITANDE; break; }
        if(ISB(t, "&&")) { tt = TT_AND; break; }
        if(ISB(t, "&")) { tt = TT_LAND; break; }
    break;
    case '(':
        if(ISB(t, "(")) { tt = TT_PARENTHESIS; break; }
    break;
    case '*':
        if(ISB(t, "*=")) { tt = TT_MULE; break; }
        if(ISB(t, "*")) { tt = TT_MUL; break; }
    break;
    case '+':
        if(ISB(t, "+=")) { tt = TT_ADDE; break; }
        if(ISB(t, "+")) { tt = TT_ADD; break; }
        if(ISB(t, "++")) { tt = TT_NEXT; break; }
    break;
    case ',':
        if(ISB(t, ",")) { tt = TT_COMMA; break; }
    break;
    case '-':
        if(ISB(t, "-=")) { tt = TT_SUBE; break; }
        if(ISB(t, "-")) { tt = TT_SUB; break; }
        if(ISB(t, "--")) { tt = TT_PREV; break; }
        if(ISB(t, "---")) { tt = TT_NEG; break; }
    break;
    case '.':
        if(ISB(t, "...")) { tt = TT_DOTS; break; }
        if(ISB(t, "..")) { tt = TT_ITR; break; }
    break;
    case '/':
        if(ISB(t, "/=")) { tt = TT_DIVE; break; }
        if(ISB(t, "/")) { tt = TT_DIV; break; }
    break;
    case ':':
        if(ISB(t, ":")) { tt = TT_COLON; break; }
    break;
    case ';':
        if(ISB(t, ";")) { tt = TT_SEMICOLON; break; }
    break;
    case '<':
        if(ISB(t, "<<=")) { tt = TT_LSHIFTE; break; }
        if(ISB(t, "<")) { tt = TT_LT; break; }
        if(ISB(t, "<=")) { tt = TT_LTE; break; }
        if(ISB(t, "<<")) { tt = TT_LSHIFT; break; }
    break;
    case '=':
        if(ISB(t, "=>")) { tt = TT_FUNCMAP; break; }
        if(ISB(t, "=")) { tt = TT_LET; break; }
        if(ISB(t, "===")) { tt = TT_SAME; break; }
        if(ISB(t, "==")) { tt = TT_EQ; break; }
        if(ISB(t, "=~")) { tt = TT_MATCH; break; }
    break;
    case '>':
        if(ISB(t, ">>=")) { tt = TT_RSHIFTE; break; }
        if(ISB(t, ">")) { tt = TT_GT; break; }
        if(ISB(t, ">=")) { tt = TT_GTE; break; }
        if(ISB(t, ">>")) { tt = TT_RSHIFT; break; }
    break;
    case '?':
        if(ISB(t, "?\?=")) { tt = TT_ALTLET; break; }
        if(ISB(t, "?")) { tt = TT_QUESTION; break; }
        if(ISB(t, "?\?")) { tt = TT_ALT; break; }
    break;
    case 'A':
        if(ISB(t, "ArrayList")) { t = STEXT("Array"); goto L_TAIL; }
    break;
    case 'F':
        if(ISB(t, "FALSE")) { t = STEXT("false"); goto L_TAIL; }
        if(ISB(t, "False")) { t = STEXT("false"); goto L_TAIL; }
    break;
    case 'I':
        if(ISB(t, "Integer")) { t = STEXT("Int"); goto L_TAIL; }
    break;
    case 'N':
        if(ISB(t, "NULL")) { t = STEXT("null"); goto L_TAIL; }
    break;
    case 'T':
        if(ISB(t, "TRUE")) { t = STEXT("true"); goto L_TAIL; }
        if(ISB(t, "True")) { t = STEXT("true"); goto L_TAIL; }
    break;
    case '[':
        if(ISB(t, "[")) { tt = TT_BRANCET; break; }
    break;
    case '^':
        if(ISB(t, "^")) { tt = TT_XOR; break; }
    break;
    case 'a':
        if(ISB(t, "aspect")) { tt = TT_ASPECT; break; }
        if(ISB(t, "assert")) { tt = TT_ASSERT; break; }
        if(ISB(t, "and")) { tt = TT_AND; break; }
        if(ISB(t, "as")) { tt = TT_AS; break; }
    break;
    case 'b':
        if(ISB(t, "break")) { tt = TT_BREAK; break; }
        if(ISB(t, "boolean")) { t = STEXT("Boolean"); goto L_TAIL; }
    break;
    case 'c':
        if(ISB(t, "class")) { tt = TT_CLASS; break; }
        if(ISB(t, "case")) { tt = TT_CASE; break; }
        if(ISB(t, "continue")) { tt = TT_CONTINUE; break; }
        if(ISB(t, "catch")) { tt = TT_CATCH; break; }
        if(ISB(t, "const")) { tt = TT_METAN; break; }
        if(ISB(t, "char")) { t = STEXT("Int"); goto L_TAIL; }
    break;
    case 'd':
        if(ISB(t, "do")) { tt = TT_DO; break; }
        if(ISB(t, "double")) { t = STEXT("Float"); goto L_TAIL; }
        if(ISB(t, "def")) { t = STEXT("var"); goto L_TAIL; }
    break;
    case 'e':
        if(ISB(t, "extends")) { tt = TT_EXTENDS; break; }
        if(ISB(t, "else")) { tt = TT_ELSE; break; }
    break;
    case 'f':
        if(ISB(t, "function")) { tt = TT_FUNCTION; break; }
        if(ISB(t, "format")) { tt = TT_FORMAT; break; }
        if(ISB(t, "for")) { tt = TT_FOR; break; }
        if(ISB(t, "foreach")) { tt = TT_FOREACH; break; }
        if(ISB(t, "finally")) { tt = TT_FINALLY; break; }
        if(ISB(t, "from")) { tt = TT_FROM; break; }
        if(ISB(t, "final")) { tt = TT_METAN; break; }
        if(ISB(t, "float")) { t = STEXT("Float"); goto L_TAIL; }
    break;
    case 'i':
        if(ISB(t, "import")) { tt = TT_IMPORT; break; }
        if(ISB(t, "implements")) { tt = TT_IMPLEMENTS; break; }
        if(ISB(t, "if")) { tt = TT_IF; break; }
        if(ISB(t, "it")) { tt = TT_IT; break; }
        if(ISB(t, "instanceof")) { tt = TT_INSTANCEOF; break; }
        if(ISB(t, "in")) { tt = TT_HAS; break; }
        if(ISB(t, "into")) { tt = TT_INTO; break; }
        if(ISB(t, "is")) { tt = TT_IS; break; }
        if(ISB(t, "int")) { t = STEXT("Int"); goto L_TAIL; }
    break;
    case 'l':
        if(ISB(t, "long")) { t = STEXT("Int"); goto L_TAIL; }
    break;
    case 'm':
        if(ISB(t, "mapmap")) { tt = TT_MAPMAP; break; }
        if(ISB(t, "mod")) { tt = TT_MOD; break; }
    break;
    case 'n':
        if(ISB(t, "namespace")) { tt = TT_NAMESPACE; break; }
        if(ISB(t, "not")) { tt = TT_NOT; break; }
    break;
    case 'o':
        if(ISB(t, "or")) { tt = TT_OR; break; }
    break;
    case 'p':
        if(ISB(t, "pragma")) { tt = TT_PRAGMA; break; }
        if(ISB(t, "print")) { tt = TT_PRINT; break; }
        if(ISB(t, "public")) { tt = TT_METAN; break; }
        if(ISB(t, "private")) { tt = TT_METAN; break; }
    break;
    case 'r':
        if(ISB(t, "return")) { tt = TT_RETURN; break; }
        if(ISB(t, "register")) { tt = TT_REGISTER; break; }
    break;
    case 's':
        if(ISB(t, "switch")) { tt = TT_SWITCH; break; }
        if(ISB(t, "static")) { tt = TT_METAN; break; }
        if(ISB(t, "short")) { t = STEXT("Int"); goto L_TAIL; }
        if(ISB(t, "string")) { t = STEXT("String"); goto L_TAIL; }
    break;
    case 't':
        if(ISB(t, "try")) { tt = TT_TRY; break; }
        if(ISB(t, "throw")) { tt = TT_THROW; break; }
        if(ISB(t, "test")) { tt = TT_TEST; break; }
        if(ISB(t, "to")) { tt = TT_TO; break; }
    break;
    case 'u':
        if(ISB(t, "using")) { tt = TT_USING; break; }
        if(ISB(t, "until")) { tt = TT_UNTIL; break; }
    break;
    case 'v':
        if(ISB(t, "virtual")) { tt = TT_METAN; break; }
    break;
    case 'w':
        if(ISB(t, "with")) { tt = TT_WITH; break; }
        if(ISB(t, "weave")) { tt = TT_WEAVE; break; }
        if(ISB(t, "while")) { tt = TT_WHILE; break; }
        if(ISB(t, "where")) { tt = TT_WHERE; break; }
    break;
    case '{':
        if(ISB(t, "{")) { tt = TT_BRACE; break; }
    break;
    case '|':
        if(ISB(t, "|=")) { tt = TT_BITORE; break; }
        if(ISB(t, "||")) { tt = TT_OR; break; }
        if(ISB(t, "|")) { tt = TT_LOR; break; }
    break;
    case '~':
        if(ISB(t, "~")) { tt = TT_LNOT; break; }
    break;
    /* built-in */
    }
    if(tt == ((knh_token_t)-1)) {
        if(t.buf != knh_cwb_tobytes(cwb).buf) {
            knh_cwb_subclear(cwb, 0);
            knh_cwb_write(ctx, cwb, t);
        }
        return knh_cwb_newTokenSYMBOL(ctx, cwb, flag, in);
    }
    else {
        knh_Token_t *tk = new_Token(ctx, flag, DP(in)->uri, DP(in)->line, tt);
        if(tt == TT_METAN) {
            KNH_SETv(ctx, DP(tk)->data, new_StringSYMBOL(ctx, t));
        }
        return  tk;
    }
}

/* ======================================================================== */
/* [token checker] */

int knh_bytes_istoken(knh_bytes_t t, int ch)
{
    switch(ch) {

    case '&':
        if(ISB(t, "&")) { return 1; }
        return 0; 
    case '+':
        if(ISB(t, "+")) { return 1; }
        return 0; 
    case '-':
        if(ISB(t, "-")) { return 1; }
        if(ISB(t, "--")) { return 1; }
        return 0; 
    case '.':
        if(ISB(t, "..")) { return 1; }
        if(ISB(t, ".")) { return 1; }
        return 0; 
    case '<':
        if(ISB(t, "<")) { return 1; }
        return 0; 
    case '=':
        if(ISB(t, "<<")) { return 1; }
        if(ISB(t, ">>")) { return 1; }
        if(ISB(t, "+")) { return 1; }
        if(ISB(t, "-")) { return 1; }
        if(ISB(t, "/")) { return 1; }
        if(ISB(t, "%")) { return 1; }
        if(ISB(t, "*")) { return 1; }
        if(ISB(t, "&")) { return 1; }
        if(ISB(t, "|")) { return 1; }
        if(ISB(t, "??")) { return 1; }
        if(ISB(t, "==")) { return 1; }
        if(ISB(t, "=")) { return 1; }
        if(ISB(t, "!")) { return 1; }
        if(ISB(t, "<")) { return 1; }
        if(ISB(t, ">")) { return 1; }
        return 0; 
    case '>':
        if(ISB(t, "=")) { return 1; }
        if(ISB(t, ">")) { return 1; }
        return 0; 
    case '?':
        if(ISB(t, "?")) { return 1; }
        return 0; 
    case '|':
        if(ISB(t, "|")) { return 1; }
        return 0; 
    case '~':
        if(ISB(t, "=")) { return 1; }
        return 0; 
    /* built-in */
    }
    return 0;
}

/* ======================================================================== */
/* [stmt] */

static char* knh_stmt_array_tochar[] = {
    "done",  /* done */ 
    "namespace",  /* namespace */ 
    "import",  /* import */ 
    "pragma",  /* pragma */ 
    "using",  /* using */ 
    "using class",  /* using class */ 
    "using alias",  /* using alias */ 
    "using unit",  /* using unit */ 
    "using enum",  /* using enum */ 
    "using vocab",  /* using vocab */ 
    "with",  /* with */ 
    "using func",  /* using func */ 
    "using mapmap",  /* using mapmap */ 
    "class",  /* class */ 
    "extends",  /* extends */ 
    "implements",  /* implements */ 
    "mapmap",  /* mapmap */ 
    "method",  /* method */ 
    "function",  /* function */ 
    "format",  /* format */ 
    "decl",  /* decl */ 
    "return",  /* return */ 
    "weave",  /* weave */ 
    "aspect",  /* aspect */ 
    "block",  /* block */ 
    "if",  /* if */ 
    "else",  /* else */ 
    "switch",  /* switch */ 
    "case",  /* case */ 
    "while",  /* while */ 
    "do",  /* do */ 
    "for",  /* for */ 
    "foreach",  /* foreach */ 
    "break",  /* break */ 
    "continue",  /* continue */ 
    "try",  /* try */ 
    "catch",  /* catch */ 
    "finally",  /* finally */ 
    "throw",  /* throw */ 
    "test",  /* test */ 
    "err",  /* err */ 
    "let",  /* let */ 
    "letm",  /* letm */ 
    "register",  /* register */ 
    "separator",  /* separator */ 
    "new",  /* new */ 
    "mapcast",  /* mapcast */ 
    "call",  /* call */ 
    "call1",  /* call1 */ 
    "pcall",  /* pcall */ 
    "mt",  /* mt */ 
    "op",  /* op */ 
    "proceed",  /* proceed */ 
    "tri",  /* tri */ 
    "alt",  /* alt */ 
    "and",  /* and */ 
    "or",  /* or */ 
    "print",  /* print */ 
    "assert",  /* assert */ 
    //(char*)0
};

char* knh_stmt_tochar(knh_stmt_t t)
{
    DBG2_ASSERT(t < KNH_STMT_MAXSIZ);
    return knh_stmt_array_tochar[t];
}

/* ------------------------------------------------------------------------ */


static int knh_stmt_array_isExpr[] = {
    0,  /* done */ 
    0,  /* namespace */ 
    0,  /* import */ 
    0,  /* pragma */ 
    0,  /* using */ 
    0,  /* using class */ 
    0,  /* using alias */ 
    0,  /* using unit */ 
    0,  /* using enum */ 
    0,  /* using vocab */ 
    0,  /* with */ 
    0,  /* using func */ 
    0,  /* using mapmap */ 
    0,  /* class */ 
    0,  /* extends */ 
    0,  /* implements */ 
    0,  /* mapmap */ 
    0,  /* method */ 
    0,  /* function */ 
    0,  /* format */ 
    0,  /* decl */ 
    0,  /* return */ 
    0,  /* weave */ 
    0,  /* aspect */ 
    0,  /* block */ 
    0,  /* if */ 
    0,  /* else */ 
    0,  /* switch */ 
    0,  /* case */ 
    0,  /* while */ 
    0,  /* do */ 
    0,  /* for */ 
    0,  /* foreach */ 
    0,  /* break */ 
    0,  /* continue */ 
    0,  /* try */ 
    0,  /* catch */ 
    0,  /* finally */ 
    0,  /* throw */ 
    0,  /* test */ 
    0,  /* err */ 
    1,  /* let */ 
    0,  /* letm */ 
    0,  /* register */ 
    0,  /* separator */ 
    1,  /* new */ 
    1,  /* mapcast */ 
    1,  /* call */ 
    1,  /* call1 */ 
    1,  /* pcall */ 
    1,  /* mt */ 
    1,  /* op */ 
    1,  /* proceed */ 
    1,  /* tri */ 
    1,  /* alt */ 
    1,  /* and */ 
    1,  /* or */ 
    0,  /* print */ 
    0,  /* assert */ 
    //(int)0
};

int knh_stmt_isExpr(knh_stmt_t t)
{
    DBG2_ASSERT(t < KNH_STMT_MAXSIZ);
    return knh_stmt_array_isExpr[t];
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif