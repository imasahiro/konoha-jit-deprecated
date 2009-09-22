#!/usr/bin/python
import os, sys
from pygenlib2 import *

# ------------------------------------------------------------------------------
# ------------------------------------------------------------------------------

CPROTO = []
TOKEN_LIST = []
TT_LIST = []
TT_MAP = {}
STT_LIST = []
STT_MAP  = {}
ADD_MAP = {}

#------------------------------------------------------------------------------

STMT = '''
done                     @stmt
namespace NSNAME:ns ;    @stmt @token @new @eos
import FURN:file ;        @stmt @token @new @decl @eos
pragma VARN:name pragma.. ANY:value ;       @stmt @token @new @eos

using uoption..            @token @stmt @eos
using_class NSCLASSN:ns ;  @stmt @decl @eos
using_alias  CLASSN:class  CURN:urn ;     @stmt @decl @eos
using_unit   CLASSTN:class CURN:urn ;     @stmt @decl @eos
using_enum   CLASSTN:class CURN:urn ;     @stmt @decl @eos
using_vocab  CLASSTN:class CURN:urn ;     @stmt @decl @eos
with @token @P99
using_func   CMETHODN:func ;   @stmt @decl @eos
using_mapmap CLASSTN:target CLASSTN:source CURN:urn ;  @stmt @decl @eos

class CLASSN:class extends.. CLASSN:superclass STT_IMPLEMENTS:interface {:instmt  @stmt @token @new @eos
extends @token
implements CLASSN*:class[] @stmt @token
mapmap CLASSTN*:paths[]   @stmt @token 

method TYPEN:rtype cmethod.. CLASSN:class METHODN:method STT_DECL:params {:instmt  @stmt @eos
function TYPEN:rtype param.. {:instmt @stmt @token @P99

format CLASSN:class MT:name PARAM*:params STR:format ;   @stmt  @token @eos
decl TYPEN:type VARN:name expr:value @stmt
return EXPR*:values   @stmt  @token @eos

weave ANY:aspect CMETHODN:method @stmt @token @new @eos
aspect TYPEN:type @stmt @token @eos

block {:block @stmt @new

if pexpr:bool {:truecase else.. {:falsecase  @stmt @token @eos
else @token

switch pexpr:bool {:case @token @stmt @eos
case expr:const {:instmt @token @eos

while pexpr:bool {:loop @token @stmt @eos
do {:loop pexpr:bool ; @stmt @token @eos
for pstmt3.. stmt:init expr:bool stmt:redo {:loop  @stmt @token @eos
foreach peach.. STT_DECL:decls expr:iter expr:where {:loop @stmt @token @eos

break    any_:label ; @stmt @token @eos
continue any_:label ;  @stmt @token @eos
#goto ANY:label ;  @stmt @token @eos

try {:try catch.. {:catch {:finally   @stmt  @token @eos
catch EXPTN:type VARN:name {:catch   @token @stmt
finally @token
throw expr:expt ;   @stmt  @token @new @name @cmpl @eos
err ANY:msg     @stmt @cmpl

let        VARN:lvalue expr:rvalue     @stmt  @expr  @P0
letm       expr*:vars @stmt
register   expr*:exprs[] ; @stmt @token @eos
separator  STT_DECL:decls expr:value   @stmt

@expr new     METHODN:new CLASSN:class expr*:args[]  @stmt @cmpl
@expr mapcast CLASSN:class expr:source expr:option   @stmt @cmpl
@expr call    METHODN:method expr:base expr*:args[]  @stmt @cmpl
@expr call1   expr:value @stmt @cmpl
@expr pcall   METHODN:method expr:base pcall.. expr*:args[] @stmt
@expr mt      MT:mt expr:target expr:option @stmt  @cmpl
@expr op      expr:first expr:second expr*:args[] @stmt  @cmpl
@expr proceed METHODN:method expr:base expr*args[]

@expr tri     expr:bool expr:tvalue expr:fvalue @stmt  @cmpl
@expr alt     expr*:bools[]  @stmt   @cmpl
@expr and     expr*:bools[]  @stmt   @cmpl
@expr or      expr*:bools[]  @stmt   @cmpl

print  expr*:values[] ;  @stmt @token @eos
assert pexpr:bool ;    @token @stmt @eos
utest  expr:test expr:spec @stmt
'''

TOKEN = '''
{ BRACE          @array     @top @P99
( PARENTHESIS    @array     @top @P99
[ BRANCET        @array     @top @P99
... DOTS         @top

; SEMICOLON      @stmt     @eos
, COMMA          @P0
=> FUNCMAP       @P99
==>  Test        @op   @P1   @A2
it   IT          @P99
= LET            @P2   @A2
<<= lshifte      
>>= rshifte       
+= adde           
-= sube            
/= dive          
%= mode          
*= mule          
&= bitande       
|= bitore        
?\?= ALTLET      

? QUESTION       @P3
: COLON          @P99
.. Itr           @op @P3 @A1

?\? ALT          @op    @P3
|| OR            @P5
or TT_OR         @alias @P5
&& AND           @P6
and TT_AND       @alias  @P6
!  Not           @top  @op  @P7 @A1
not TT_NOT       @alias  @P7  @A1

instanceof Instanceof  @op    @P9  @A2
as  As                 @op  @P9  @A2
in Has                 @op  @P9  @A2
into Into              @op  @P9  @A2
is  Is                 @op  @P9  @A2

=== Same               @op @A2   @P9  
== Eq                  @op @A2   @P9
!= Neq                 @op @A2   @P9
< Lt                   @op @A2   @P9
<= Lte                 @op @A2   @P9
> Gt                   @op @A2   @P9
>= Gte                 @op @A2   @P9
=~  Match              @op @A2   @P9

from From              @op @A2   @top @P15
where Where            @op @A2   @P15
to To                  @op @A2   @P15
until Until            @op @A2   @P15

<< Lshift              @op @2    @P15
>> Rshift              @op @2    @P15
+ Add                  @op @top  @P20
- Sub                  @op @top  @P20  @A2
/ Div                  @op @2    @P24  @A2
% Mod                  @op @2    @P24  @A2
* Mul                  @op @2    @P24  @A2

& Land                 @op @top      @P27
|  Lor                  @op @top     @P27
~ Lnot                 @op @A1 @top  @P27
^ Xor                  @op @A2 @top  @P27
++ Next                @op @A1 @top  @P28
-- Prev                @op @A1 @top  @P28
--- Neg                @op @A1 @top  @P28

NUM                    @system @top @const @P99
STR                    @system @top @const @P99
TSTR                   @system @top @const @P99
ESTR                   @system @top @P99
LABEL                  @system @top
DOC                    @system @top
METAN                  @system @top @eos
PROPN                  @system @top @P99
URN                    @system @top @const @P99
MT                     @system @top @P99
CONSTN                 @system @top @const @P99
TYPEN                  @system @top @const @P99
CMETHODN               @system @top @const @P99
CCONSTN                @system @top @const @P99
NAME                   @system @top @const @P99
ERR                    @system 
MN                     @system @top @P99
FN                     @system @top @P99
CID                    @system @top @P99
MPR                    @system @top @P99
EOT                    @system @eos

#Typed Token
CONST                  @system @P99
DEFVAL                 @system @P99
SYSVAL                 @system @P99
LOCAL                  @system @P99
STACK                  @system @P99
FIELD                  @system @P99
SCRIPT                 @system @P99
MEMBER                 @system @P99
CLASSID                @system @P99
CLOSURE                @system @P99
_ ASIS                 @system @P99
NOP                    @system @P99

public  TT_METAN        @alias
private TT_METAN        @alias
final   TT_METAN        @alias
virtual TT_METAN        @alias
static  TT_METAN        @alias
const   TT_METAN        @alias

mod     TT_MOD          @alias

char    Int             @alias
short   Int             @alias
int     Int             @alias
long    Int             @alias
float   Float           @alias
double  Float           @alias
boolean Boolean         @alias
string  String          @alias
def     var             @alias
ArrayList Array         @alias
Integer Int             @alias

TRUE    true            @alias
FALSE   false           @alias
NULL    null            @alias
True    true            @alias
False   false           @alias
'''

#------------------------------------------------------------------------------

class Token:
    def __init__(self, line):
        self.line = line
        self.options = []
        self.alias = None
        
        c = 0
        for t in line.split():
            if t.startswith('@'):
                self.options.append(t)
                continue
            if c == 0:
                c += 1
                if t == "\\#": t = '#'
                self.token = t
                self.name = t
            elif c == 1:
                c += 1
                self.name = t
        
        if not '@system' in self.options:
            TOKEN_LIST.append(self)            
        if '@alias' in self.options:
            if self.name.startswith('TT_'):
                self.TT = self.name
            else:
                self.alias = self.name
        else:
            self.TT = 'TT_%s' % self.name.upper()
            TT_LIST.append(self)
            TT_MAP[self.name] = self
        self.opFunc = 'METHODN_NONAME'
        if '@op' in self.options:
            self.opFunc = 'METHODN_op' + self.name
            
    def isf(self, op):
        return op in self.options
    
    def array(self, op):
        if op == '@P':
            for op in self.options:
                if op.startswith('@P'): return op[2:]
            return '0'
        if op == '@A':
            for op in self.options:
                if op.startswith('@A'): return op[2:]
            return '0'
        if op == '@name': return '"%s"' % self.token
        if op == '@op': return self.opFunc
        if op == '@eos': 
            if op in self.options: return '1';
            return '0'
        if op in self.options: return '1'
        return '0'

class Param:
    def __init__(self, type, name):
        #print type, name
        self.type = type
        self.name = name
        self.cast = 'terms'
        self.check = 'knh_Term_isExpr(%s)'
        if type == '{' :
            self.cast = 'stmts'
            self.check = 'IS_Stmt(%s)'
        if type.startswith('STT_'):
            self.cast = 'stmts'
            self.check = '(SP((knh_Stmt_t*)(%%s))->stt = %s)' % type
            return
        if type[0].isupper():
            self.cast = 'tokens'
            self.check = 'knh_Token_is%s(%%s)' % type

    def __k__(self):
        if self.type == self.name:
            return self.type
        return '%s:%s' % (self.type, self.name)


class Stmt:
    def __init__(self, line):
        self.line = line
        self.options = []
        self.structs = []
        self.grammars = []
        self.size = 0
        self.has_args = False
        
        c = 0
        e = 0
        for t in line.split():
            if t.startswith('@'): 
                self.options.append(t)
                continue
            if c == 0:
                if t.startswith('_'): t = t[1:]
                
                self.name = t.replace('_', ' ')
                self.id = '_' + t.replace('using_', 'u').replace('pragma_', 'p')
                self.macro = self.id[1:].upper()
                self.STT = 'STT%s' % self.id.upper()
                c += 1
            else:
                type = t
                if t.find(':') > 0:
                    type, name = t.split(':')
                    self.structs.append(Param(type, name))
                    if not ADD_MAP.has_key(type): 
                        ADD_MAP[type] = type
                if e == 0:
                    if type.endswith('..'): 
                        e = 1
                        type = type.replace('..', '')
                    self.grammars.append(type)
                if type.endswith('*'): self.size += 4
        if self.isf('@token'):
            line = '%s' % self.id.replace('_', '')
            if self.isf('@stmt'): line += ' @stmt'
            if self.isf('@eos'): line += ' @eos'
            if self.isf('@P99'): line += ' @P99'
            Token(line)
                
        self.funcbase = 'Stmt' + self.STT.replace('STT_', '')
        self.size += len(self.structs)

    def isf(self, op):
        return op in self.options

    def array(self, op):
        if op == '@size': return '%d' % self.size
        if op == '@tochar': return '"%s"' % self.name
        if op in ['@decl', '@name', '@cmpl']:
            if op in self.options:
                return 'knh_%s_%s' % (self.funcbase, op[1:])
            else:
                return 'knh_StmtDONE_%s' % (op[1:])
        if op in self.options: return '1'
        return '0'

#------------------------------------------------------------------------------

for line in STMT.split('\n'):
    if line == '' or line.startswith('#'):
        continue
    st = Stmt(line)
    STT_LIST.append(st)
    STT_MAP[st.id] = st

for line in TOKEN.split('\n'):
    if line == '' or line.startswith('#'):
        continue
    Token(line)

#------------------------------------------------------------------------------

def write_token_h(f):
    write_chapter(f, 'TT_MACRO knh_token_t')
    tn = 0        
    for tk in TT_LIST:
        write_define(f, tk.TT, '((knh_token_t)%d)' % tn, 40)
#        if tk.opFunc != None:
#            write_ifndef(f, 'METHODN_%s' % tk.opFunc, 'METHODN_NONAME', 40)
        tn += 1
        if tk.isf('@system'):
            write_define(f, 'knh_Token_is%s(tk_)' % tk.name, '(SP(tk_)->tt == %s)' % tk.TT, 40)
    write_define(f, 'KNH_TOKEN_MAXSIZ',  '((knh_token_t)%d)' % tn, 40)
    write_line(f)


def write_stmt_h(f):
    write_chapter(f, 'STT_MACRO knh_stmt_t')
    tn = 0
    for st in STT_LIST:
        write_define(f, st.STT, '((knh_stmt_t)%d)' % tn, 40)
        tn += 1
    write_define(f, 'KNH_STMT_MAXSIZ',  '((knh_stmt_t)%d)' % tn, 40)

    write_line(f)

    for st in STT_LIST:
        write_chapter(f, '[%s]' % st.name)
        loc = -1
        for p in st.structs:
            loc += 1
            if p.name.endswith('[]'):
                write_define(f, 'Stmt%s_%s_size(stmt)' % (st.macro, p.name[:-2]), '(DP(stmt)->size - %d)' % loc, 40)
                write_define(f, 'Stmt%s_%s(stmt,n)' % (st.macro, p.name[:-2]), 'DP(stmt)->%s[n-%d]' % (p.cast, loc), 40)
                continue
            if p.name.endswith('[-1]'):
                write_define(f, 'Stmt%s_%s(stmt)' % (st.macro, p.name[:-4]), 'DP(stmt)->%s[DP(stmt)->size - 1]' % (p.cast), 40)
            else:
                write_define(f, 'Stmt%s_%s(stmt)' % (st.macro, p.name), 'DP(stmt)->%s[%d]' % (p.cast, loc), 40)
                write_define(f, '%s_%s' % (st.macro, p.name), '%d' % loc, 40)
    
    ###
    
#------------------------------------------------------------------------------
# common
#------------------------------------------------------------------------------

def write_array(f, ctype, ttype, fn, list, op):
    f.write('''
static %s knh_%s_array_%s[] = {''' % (ctype, ttype, fn))
    for t in list:
        f.write('''
    %s,  /* %s */ ''' % (t.array(op), t.name))
    f.write('''
    //(%s)0
};
''' % (ctype))
    
    func = '%s knh_%s_%s(knh_%s_t t)' % (ctype, ttype, fn, ttype)
    CPROTO.append(func+';')
    f.write('''
%s
{
    DBG2_ASSERT(t < KNH_%s_MAXSIZ);
    return knh_%s_array_%s[t];
}
''' % (func, ttype.upper(), ttype, fn))
    write_line(f)

def write_knh_StmtDONE(f):
    f.write('''
''')

#------------------------------------------------------------------------------
# parser
#------------------------------------------------------------------------------

def write_chktoken_c(f):
    write_chapter(f, '[token checker]')
    CPROTO.append('int knh_bytes_istoken(knh_bytes_t t, int ch);')
    f.write('''
int knh_bytes_istoken(knh_bytes_t t, int ch)
{
    switch(ch) {
''')
    DC = {}
    for tk in TOKEN_LIST:
        ch = tk.token[-1]
        if ch.isalnum() or len(tk.token) == 1: continue
        if not DC.has_key(ch):
            DC[ch] = []
        DC[ch].append(tk)

    fcl = DC.keys()
    fcl.sort()
    
    for fc in fcl:
        f.write('''
    case '%s':''' % fc)
        for tk in DC[fc]:
            f.write('''
        if(ISB(t, "%s")) { return 1; }''' % (tk.token[:-1].replace("\\", "")))
        f.write('''
        return 0; ''')
    
    f.write('''
    /* built-in */
    }
    return 0;
}
''')


def write_tparser_c(f):
    write_chapter(f, '[isKeyword]')
    CPROTO.append('int knh_token_isKeyword(knh_token_t tt);')
    f.write('''
int knh_token_isKeyword(knh_token_t tt)
{
    char *keyword = knh_token_tochar(tt);
    return islower(keyword[0]);
}

''')
    write_chapter(f, '[token parser]')
    CPROTO.append('knh_Token_t *knh_cwb_parseToken(Ctx *ctx, knh_cwb_t *cwb, knh_flag_t flag, knh_InputStream_t *in);')
    f.write('''

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

    switch(t.buf[0]) {''')
    DC = {}
    for tk in TOKEN_LIST:
        if not DC.has_key(tk.token[0]):
            DC[tk.token[0]] = []
        DC[tk.token[0]].append(tk)

    fcl = DC.keys()
    fcl.sort()

    #print al
    for fc in fcl:
        f.write('''
    case '%s':''' % fc)
        for tk in DC[fc]:
            if tk.alias is None :
                f.write('''
        if(ISB(t, "%s")) { tt = %s; break; }''' % (tk.token, tk.TT));
            else:
                f.write('''
        if(ISB(t, "%s")) { t = STEXT("%s"); goto L_TAIL; }''' % (tk.token, tk.alias));
        f.write('''
    break;''')
    f.write('''
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
''')

#------------------------------------------------------------------------------
# parser
#------------------------------------------------------------------------------

def write_stmt_add(f, token):
    write_section(f, '[%s]' % token)
    func = 'void knh_Stmt_add_%s(Ctx *ctx, Stmt *o, knh_tokens_t *tc)' % token
    #CPROTO.append('%s;' % func)
    f.write('''
static %s
{
\tif(SP(o)->stt == STT_ERR) return;
\tif(tc->c < tc->e && knh_Token_is%s(tc->ts[tc->c])) {
\t\tknh_Stmt_add(ctx, o, UP(tc->ts[tc->c]));
\t\ttc->c += 1;
\t}
\telse {
\t\tknh_Stmt_tokens_perror(ctx, o, tc, KMSG_T%s);
\t}
}
''' % (func, token, token))
         
#------------------------------------------------------------------------------

def gen_stmt(bdir):
    fname = 'src/compiler/misc_.c'
    f = open_c(fname, ['"commons.h"'])
    
    write_chapter(f,'[token]')
    write_array(f, 'char*', 'token', 'tochar', TT_LIST, '@name')
    write_array(f, 'knh_methodn_t', 'token', 'tomethodn', TT_LIST, '@op')
    write_array(f, 'int', 'token', 'getOpPriority', TT_LIST, '@P')
    write_array(f,  'int', 'token', 'getOpSize', TT_LIST, '@A')
    write_array(f, 'knh_bool_t', 'token', 'isBeginOfStmt', TT_LIST, '@eos')
    write_tparser_c(f)
    write_chktoken_c(f)
        
    write_chapter(f,'[stmt]')
    write_array(f, 'char*', 'stmt', 'tochar', STT_LIST, '@tochar')
    #write_array(f, 'size_t', 'stmt', 'size', STT_LIST, '@size')
    #write_sparser_c(f)

    write_knh_StmtDONE(f)
    write_array(f, 'int', 'stmt', 'isExpr', STT_LIST, '@expr')
    
    close_c(f, fname)
    ##
    f = open('include/konoha/konoha_stmt_.h', 'w')
    f.write('''// THIS FILE WAS AUTOMATICALLY GENERATED
''')
    write_token_h(f)
    write_stmt_h(f)
    write_dline(f)
    for p in CPROTO: f.write(p+'\n')
    f.close()
    
#------------------------------------------------------------------------------

if __name__ == '__main__':
    bdir = '..'
    gen_stmt(bdir)
