#!/usr/bin/python
import os, sys
from pygenlib2 import *

# ------------------------------------------------------------------------------
# ------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# TOKEN

TOKEN = '''
pragma - @keyword @stmt
namespace - @keyword @stmt
script - @keyword @stmt
include - @keyword @stmt
import - @keyword @stmt
using - @keyword @stmt
class - @keyword @stmt
method - @stmt
format - @stmt
return - @keyword @stmt
yeild - @keyword @stmt
if - @keyword @stmt
switch - @keyword @stmt
case - @keyword @stmt
while - @keyword @stmt
do - @keyword @stmt
for - @keyword @stmt
foreach - @keyword @stmt
break    - @keyword @stmt
continue - @keyword @stmt
goto     - @keyword @stmt
try - @keyword @stmt
catch - @keyword @stmt
throw - @keyword @stmt
lsend -  @stmt
print  - @keyword @stmt
assert - @keyword @stmt
checkin - @keyword @stmt
register - @keyword @stmt
select - @keyword @stmt
go - @keyword @stmt

### using
#alias - @keyword
#unit
#enum
#vocab
### class
extends - @keyword
implements - @keyword
### if, switch
else - @keyword
default - @keyword
### if
finally - @keyword

# @array TT_BRACE TT_DOT

... TT_DOTS         @top   @op
!! TT_EXPT
,  TT_COMMA          @P0
;  TT_SEMICOLON      @top
_  TT_ASIS                 @system @P99
# in expr
function - @keyword @stmt
block -  @stmt

new   - @keyword
null  - @keyword
void  - @keyword
var   - @keyword
true  - @keyword
false - @keyword
byte  - @keyword
typeof - @keyword
{ TT_BRACE          @top
( TT_PARENTHESIS    @top
[ TT_BRANCET        @top
. TT_DOT            @top
:  TT_COLON       @P99
-> TT_ARROW            @P99
=> TT_FUNCARROW        @P99  

<<=   TT_LSFTE      
>>=   TT_RSFTE       
+=    TT_ADDE           
-=    TT_SUBE            
/=    TT_DIVE          
%=    TT_MODE          
*=    TT_MULE          
&=    TT_LANDE       
|=    TT_LORE        
?\?=  TT_ALTLET      

'''

OP = '''
=   TT_LET          @opLET        @P3
<<  TT_SEND         @opSEND       @P2

?\? TT_ALT          @opALT @op    @P4
?  TT_QUESTION            @opQ  @P4
or TT_OR            @opOR  @P5
and TT_AND          @opAND @P6
not TT_NOT          @opNOT @P7  @A1
exists  TT_EXISTS      @P8  @opEXISTS

#begin TT_isBINARY
is?  TT_IS             @opIS    @P9  @A2
instanceof TT_OF       @opOF    @P9  @A2
isa? TT_ISA            @opISA   @P9  @A2
in? TT_IN              @opHAS    @P9  @A2
=~ TT_MATCH            @opCASE @A2   @P9

== TT_EQ               @opEQ    @A2   @P9
!= TT_NEQ              @opNOTEQ  @A2   @P9
<  TT_LT               @opLT    @A2   @P9
<= TT_LTE              @opLTE   @A2   @P9
>  TT_GT               @opGT    @A2   @P9
>= TT_GTE              @opGTE   @A2   @P9

from -                  @opFROM  @A2   @top @P15
where -                 @opWHERE @A2   @P15
to -                    @opTO    @A2   @P15
until -                 @opUNTIL @A2   @P15

<<< TT_LSFT              @opLSFT   @P15  @A2
>> TT_RSFT              @opRSFT   @P15  @A2
+ TT_ADD                @opADD    @P20
- TT_SUB                @opSUB    @P20  @A2
/ TT_DIV                @opDIV    @P24  @A2
% TT_MOD                @opMOD    @P24  @A2
* TT_MUL                @opMUL    @P24  @A2
& TT_LAND                 @opLAND @P26
|  TT_LOR                 @opLOR  @P27
^  TT_XOR                 @opLXOR @P27
#end TT_isBINARY

~  TT_LNOT                @opLNOT @A1 @top  @P28
++ TT_NEXT                @opNEXT @A1 @top  @P28
-- TT_PREV                @opPREV @A1 @top  @P28
.. TT_ITR                 @opITR  @A1 @top  @P28
*** TT_TMUL               @opEXPAND    @A1 @top  @P28
+++ TT_TADD               @opPLUS    @A1 @top  @P28
&&& TT_ADDR               @opADDR @A1 @top  @P28
--- TT_TSUB               @opNEG  @A1 @top  @P28
'''

ALIAS = '''
public  @Public
private @Private
final   @Final
virtual @Virtual
static  @Static

mod     %
||      or
&&      and
!       not
===     is

char    Int
short   Int
int     Int
long    Int
float   Float
double  Float
boolean Boolean
string  String
def     var
ArrayList Array
Integer Int

TRUE    true
FALSE   false
NULL    null
True    true
False   false
'''

EXPR = '''
done  - @stmt
decl  - @stmt
letm  - @stmt
pushm - @stmt
cmd   - @stmt
extra - @stmt
err   - @stmt

# @expr: let - call1
let     - @stmt  @expr
new     - @stmt  @expr
tcast   - @stmt  @expr
qcast   - @stmt  @expr
func    - @stmt  @expr
call    - @stmt  @expr
pcall   - @stmt  @expr
mt      - @stmt  @expr
fmt     - @stmt  @expr
op      - @stmt  @expr
tri     - @stmt  @expr
alt     - @stmt  @expr
and     - @stmt  @expr @keyword
or      - @stmt  @expr @keyword
w       - @stmt
call1   - @stmt  @expr
'''

TOKENT = '''
NUM - 
STR - 
TSTR - 
ESTR - 
FMTSTR -
REGEX - 
DOC - 
METAN - 
PROPN - 
URN - 
QPATH -
NAME - 
UNAME - 
FUNCNAME - 
TYPE  - 
CID - 
MN - 
FMT - 
MT - 

#Typed Token
CONST -                 
STMT  -                
SYSVAL -                
LOCAL -                 
STACK -                 
FIELD -                 

ERR -                    
EOT -                    
'''

#------------------------------------------------------------------------------

TOKENMAP = {}
TT_LIST = []
DTT_LIST = []
OP_LIST = []
MN_LIST = []
STT_LIST = []
DSTT_LIST = []
ALIASMAP = {}

def get_op(tokens):
	for t in tokens:
		if t.startswith('@op'): 
			return t.replace('@op', 'MN_op')
	return '/*ERROR*/'

def get_p(tokens):
	for t in tokens:
		if t.startswith('@P'): 
			return t.replace('@P', '')
	return '100'

def keyword(tokens, TT):
	for t in tokens:
		if t.startswith('@keyword'): 
			return 'TT_KEYWORD(%s)' % TT
	return TT

def readData():
	for line in TOKEN.split('\n'):
		if line == '' or line.startswith('#'): continue
		t = line.split()
		if not len(t) > 1: continue
		name = t[0]
		TT = t[1]
		if TT == '-' : TT = 'TT_%s' % name.upper()
		TOKENMAP[name] = keyword(t, TT)
		TT_LIST.append(TT)
		DTT_LIST.append('"%s", /* %s */' % (name, TT))
		if '@stmt' in line:
			STT_LIST.append(TT.replace('TT_', 'STT_'))
			DSTT_LIST.append('"%s", /* S%s */' % (name, TT))

	for line in TOKENT.split('\n'):
		if line == '' or line.startswith('#'): continue
		t = line.split()
		if not len(t) > 1: continue
		name = t[0]
		TT = t[1]
		if TT == '-' : TT = 'TT_%s' % name.upper()
		TT_LIST.append(TT)
		DTT_LIST.append('"%s", /* %s */' % (TT, TT))

	for line in OP.split('\n'):
		if line == '' or line.startswith('#'): continue
		t = line.split()
		if not len(t) > 1: continue
		name = t[0]
		TT = t[1]
		if TT == '-' : TT = 'TT_%s' % name.upper()
		TOKENMAP[name] = keyword(t, TT)
		TT_LIST.append(TT)
		DTT_LIST.append('"%s", /* %s */' % (name, TT))
		MN_LIST.append(get_op(t))
		OP_LIST.append('"%s", %s /* %s */' % (name, get_p(t), TT))

	for line in EXPR.split('\n'):
		if line == '' or line.startswith('#'): continue
		t = line.split()
		if not len(t) > 1: continue
		name = t[0]
		TT = t[1]
		if TT == '-' : TT = 'STT_%s' % name.upper()
		STT_LIST.append(TT)
		DSTT_LIST.append('"%s", /* %s */' % (TT, TT))

	for line in ALIAS.split('\n'):
		if line == '' or line.startswith('#'): continue
		t = line.split()
		if not len(t) > 1: continue
		name = t[0]
		TT = t[1]
		ALIASMAP[name] = TT

def write_file(f):
	f.write('''
#ifndef GAMMA_MACRO
#define GAMMA_MACRO 1
''')
	write_chapter(f, 'TT_MACRO')
	n = 0
	for tt in TT_LIST:
		write_define(f, tt, '((knh_term_t)%d)' % n, 40)
		n += 1

	write_define(f, 'K_TOKEN_MAXSIZ',  '((knh_term_t)%d)' % n, 40)
	write_line(f)

	#n = 0
	for stt in STT_LIST:
		write_define(f, stt, '((knh_term_t)%d)' % n, 40)
		n += 1

	write_define(f, 'K_STMT_MAXSIZ',  '((knh_term_t)%d)' % n, 40)
	write_line(f)

	n = 0
	for mn in MN_LIST:
		write_define(f, mn, '((knh_methodn_t)%d)' % n, 40)
		n += 1

	write_define(f, 'MN_OPSIZE',  '((knh_methodn_t)%d)' % n, 40)
	write_line(f)
		
	f.write('''

#endif/*GAMMA_MACRO*/

#ifdef K_USING_LOADDATA

typedef struct {
	const char *name;
	knh_uintptr_t tt;
} knh_TokenData_t;

static knh_TokenData_t knh_TokenData[] = {''')
	for n in TOKENMAP.keys():
		f.write('''
	{"%s", %s}, ''' % (n, TOKENMAP[n]))
	f.write('''
	{NULL, 0}
};

static void knh_loadTokenData(Ctx *ctx)
{
	knh_TokenData_t *data = knh_TokenData;
	knh_DictSet_t *ds = DP(ctx->sys)->tokenDictSet;
	while(data->name != NULL) {
		knh_DictSet_set(ctx, ds, new_T(data->name), data->tt);
		data++;
	}
}

typedef struct {
	const char *name;
	const char *alias;
} knh_AliasData_t;

static knh_AliasData_t knh_AliasData[] = {''')
	for n in ALIASMAP.keys():
		f.write('''
	{"%s", "%s"}, ''' % (n, ALIASMAP[n]))
	f.write('''
	{NULL, NULL}
};

static void knh_loadAliasTokenData(Ctx *ctx)
{
	knh_AliasData_t *data = knh_AliasData;
	knh_DictMap_t *dm = new_DictMap0(ctx, 0);
	KNH_INITv(DP(ctx->share->mainns)->aliasDictMapNULL, dm);
	while(data->name != NULL) {
		knh_DictMap_set(ctx, dm, new_T(data->name), UPCAST(new_T(data->alias)));
		data++;
	}
}

#endif/*K_USING_LOADDATA*/

#ifdef K_USING_TERMCODE
/***********************************************
static knh_term_t knh_bytes_parseToken(Ctx *ctx, knh_bytes_t t)
{
	knh_uintptr_t n = knh_DictSet_get__b(DP(ctx->sys)->tokenDictSet, t);
	if(n > 0) {
		knh_term_t tt = (knh_term_t)(n);
		return TT_UNMASK(tt);
	}
	return TT_EOT;
}

static int knh_bytes_isKeyword(Ctx *ctx, knh_bytes_t t)
{
	knh_uintptr_t n = knh_DictSet_get__b(DP(ctx->sys)->tokenDictSet, t);
	if(n > 0) {
		knh_term_t tt = (knh_term_t)(n);
		return TT_ISKEYWORD(tt);
	}
	return TT_EOT;
}
********************************************/

#ifndef K_USING_SMALLFOOTPRINT
static const char* knh_tokens[] = {''')
	for tt in DTT_LIST:
		f.write('''
	%s''' % (tt))
	f.write('''
	NULL
};
#endif/*SMALLFOOTPRINT*/

#ifndef K_USING_SMALLFOOTPRINT
static const char* knh_stmts[] = {''')
	for stt in DSTT_LIST:
		f.write('''
	%s''' % (stt))
	f.write('''
	NULL
};
#endif/*SMALLFOOTPRINT*/

const char *TT__(knh_term_t tt)
{
	if(tt < K_TOKEN_MAXSIZ) {
#ifdef K_USING_SMALLFOOTPRINT
		return "TT";
#else
		return knh_tokens[tt];
#endif
	}
	else {
#ifdef K_USING_SMALLFOOTPRINT
		return "STT";
#else
		return knh_stmts[tt - K_TOKEN_MAXSIZ];
#endif
	}
}

typedef struct { 
	const char *opname;
	int priority;
} knh_opdata_t;

static knh_opdata_t knh_opdata[] = {''')
	for op in OP_LIST:
		f.write('''
	{%s}, ''' % (op))
	f.write('''
	{NULL, 0},
};

static int knh_term_getOPRpriority(knh_term_t tt)
{
	if(TT_LET <= tt && tt <= TT_TSUB) {
		return knh_opdata[tt-TT_LET].priority;
	}
	if(TT_ASIS <= tt && tt < TT_ERR) {
	    return 99;
	}
	return 0;
}

const char* knh_getopname(knh_methodn_t mn)
{
	DBG_ASSERT(mn + TT_LET <= TT_TSUB);
	return knh_opdata[mn].opname;
}
#endif/*K_USING_TERMCODE*/
''')

def gen_stmt(bdir):
    f = open('include/konoha/konoha_grammar_.h', 'w')
    f.write('''// THIS FILE WAS AUTOMATICALLY GENERATED
''')
    readData()
    write_file(f)
    f.close()


#------------------------------------------------------------------------------

if __name__ == '__main__':
    bdir = '..'
    readData()
    write_file(sys.stdout)
    #gen_stmt(bdir)
