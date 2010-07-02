#!/usr/bin/python
import os, sys
import copy
from pygenlib2 import *

#------------------------------------------------------------------------------

INSTRUCTIONS = """
# level 0 instruction 
HALT      @0
ENTER     @0
FUNCCALL  @0
THCODE  @0
LABEL   id:u msg:String  @0
PROBE   probe:f n:u  @0
VEXEC     @0
YEILD   n:sfpidx   @0
EXIT    @0

P       print:f flag:u msg:String  fmt:mtd n:sfpidx

OSET   a:sfpidx o:Object
NSET   a:sfpidx n:int
OMOV   a:sfpidx b:sfpidx
NMOV   a:sfpidx b:sfpidx

SWAP   a:sfpidx b:sfpidx
UNBOX  a:sfpidx b:sfpidx

ONMOV  a:sfpidx b:sfpidx c:sfpidx d:sfpidx
OOMOV  a:sfpidx b:sfpidx c:sfpidx d:sfpidx  @2
NNMOV  a:sfpidx b:sfpidx c:sfpidx d:sfpidx  @2

OMOVx  a:sfpidx b:sfx
iMOVx  a:sfpidx b:sfx
fMOVx  a:sfpidx b:sfx
bMOVx  a:sfpidx b:sfx

XMOV   a:sfx b:sfpidx
XMOVx  a:sfx b:sfx
XOSET  a:sfx b:Object
XIMOV  a:sfx b:sfpidx
XFMOV  a:sfx b:sfpidx
XBMOV  a:sfx b:sfpidx
#MOVe   a:sfpidx xspidx:u

CHKSTACK n:sfpidx
LDMETHOD method:f n:sfpidx callmtd:mtd
CALL   rtnidx:sfpidx espshift:sfpidx
SCALL  rtnidx:sfpidx espshift:sfpidx callmtd:mtd    @2
VCALL  rtnidx:sfpidx espshift:sfpidx callmtd:mtd    @2
VCALL_  rtnidx:sfpidx espshift:sfpidx callmtd:mtd    @2
RET

SCAST   a:sfpidx b:sfpidx cast:trl
TCAST   a:sfpidx b:sfpidx cast:trl
ACAST   a:sfpidx b:sfpidx cast:trl
iCAST   a:sfpidx b:sfpidx
fCAST   a:sfpidx b:sfpidx
TR      a:sfpidx b:sfpidx cid:cid tr:f

NUL     a:sfpidx b:sfpidx
iNUL    a:sfpidx b:sfpidx
fNUL    a:sfpidx b:sfpidx

JMP     addr:addr
JMP_    addr:addr   @2
ONCE    addr:addr
JMPF    addr:addr a:sfpidx
DYJMP   addr:addr a:sfpidx chk:f

NEXT    addr:addr a:sfpidx b:sfpidx
#NEXTf   addr:addr a:sfpidx b:sfpidx cid:cid next:f

TRY     addr:addr hn:sfpidx
THROW   start:sfpidx
CATCH   addr:addr en:sfpidx msg:String

#PUSH   sfpidx
#POP    sfpidx

bNOT  c:sfpidx a:sfpidx

iINC   a:sfpidx  @3
iDEC   a:sfpidx  @3

iNEG  c:sfpidx a:sfpidx
iADD  c:sfpidx a:sfpidx b:sfpidx
iSUB  c:sfpidx a:sfpidx b:sfpidx
iMUL  c:sfpidx a:sfpidx b:sfpidx
iDIV  c:sfpidx a:sfpidx b:sfpidx
iMOD  c:sfpidx a:sfpidx b:sfpidx
iEQ   c:sfpidx a:sfpidx b:sfpidx
iNEQ  c:sfpidx a:sfpidx b:sfpidx
iLT   c:sfpidx a:sfpidx b:sfpidx
iLTE  c:sfpidx a:sfpidx b:sfpidx
iGT   c:sfpidx a:sfpidx b:sfpidx
iGTE  c:sfpidx a:sfpidx b:sfpidx

iADDn  c:sfpidx a:sfpidx n:int
iSUBn  c:sfpidx a:sfpidx n:int
iDIVn  c:sfpidx a:sfpidx n:int
iMULn  c:sfpidx a:sfpidx n:int
iMODn  c:sfpidx a:sfpidx n:int
iEQn   c:sfpidx a:sfpidx n:int
iNEQn  c:sfpidx a:sfpidx n:int
iLTn   c:sfpidx a:sfpidx n:int
iLTEn  c:sfpidx a:sfpidx n:int
iGTn   c:sfpidx a:sfpidx n:int
iGTEn  c:sfpidx a:sfpidx n:int

fNEG   c:sfpidx a:sfpidx
fADD   c:sfpidx a:sfpidx b:sfpidx
fSUB   c:sfpidx a:sfpidx b:sfpidx
fMUL   c:sfpidx a:sfpidx b:sfpidx
fDIV   c:sfpidx a:sfpidx b:sfpidx
fEQ    c:sfpidx a:sfpidx b:sfpidx
fNEQ   c:sfpidx a:sfpidx b:sfpidx
fLT    c:sfpidx a:sfpidx b:sfpidx
fLTE   c:sfpidx a:sfpidx b:sfpidx
fGT    c:sfpidx a:sfpidx b:sfpidx
fGTE   c:sfpidx a:sfpidx b:sfpidx

fADDn  c:sfpidx a:sfpidx n:float
fSUBn  c:sfpidx a:sfpidx n:float
fMULn  c:sfpidx a:sfpidx n:float
fDIVn  c:sfpidx a:sfpidx n:float
fEQn   c:sfpidx a:sfpidx n:float
fNEQn  c:sfpidx a:sfpidx n:float
fLTn   c:sfpidx a:sfpidx n:float
fLTEn  c:sfpidx a:sfpidx n:float
fGTn   c:sfpidx a:sfpidx n:float
fGTEn  c:sfpidx a:sfpidx n:float

OGETIDX    c:sfpidx a:sfpidx n:sfpidx
OSETIDX    c:sfpidx a:sfpidx n:sfpidx v:sfpidx 
OGETIDXn   c:sfpidx a:sfpidx n:u
OSETIDXn   c:sfpidx a:sfpidx n:u v:sfpidx
NGETIDX    c:sfpidx a:sfpidx n:sfpidx
NSETIDX    c:sfpidx a:sfpidx v:sfpidx n:sfpidx
NGETIDXn   c:sfpidx a:sfpidx n:u
NSETIDXn   c:sfpidx a:sfpidx v:sfpidx n:u

# level 3 instruction

bJNOT  addr:addr a:sfpidx @3
iJEQ   addr:addr a:sfpidx b:sfpidx  @3
iJNEQ  addr:addr a:sfpidx b:sfpidx  @3
iJLT   addr:addr a:sfpidx b:sfpidx  @3
iJLTE  addr:addr a:sfpidx b:sfpidx  @3
iJGT   addr:addr a:sfpidx b:sfpidx  @3
iJGTE  addr:addr a:sfpidx b:sfpidx  @3
iJEQn  addr:addr a:sfpidx n:int     @3
iJNEQn addr:addr a:sfpidx n:int     @3
iJLTn  addr:addr a:sfpidx n:int     @3
iJLTEn addr:addr a:sfpidx n:int     @3
iJGTn  addr:addr a:sfpidx n:int     @3
iJGTEn addr:addr a:sfpidx n:int     @3

fJEQ   addr:addr a:sfpidx b:sfpidx  @3
fJNEQ  addr:addr a:sfpidx b:sfpidx  @3
fJLT   addr:addr a:sfpidx b:sfpidx  @3
fJLTE  addr:addr a:sfpidx b:sfpidx  @3
fJGT   addr:addr a:sfpidx b:sfpidx  @3
fJGTE  addr:addr a:sfpidx b:sfpidx  @3
fJEQn  addr:addr a:sfpidx n:float   @3
fJNEQn addr:addr a:sfpidx n:float   @3
fJLTn  addr:addr a:sfpidx n:float   @3
fJLTEn addr:addr a:sfpidx n:float   @3
fJGTn  addr:addr a:sfpidx n:float   @3
fJGTEn addr:addr a:sfpidx n:float   @3

NOP
"""

CTYPE = {
	'sfpidx' : 'knh_sfpidx_t', 
	'sfx' :    'knh_sfx_t',
	'int':     'knh_int_t',
	'float':   'knh_float_t',
	'cid':     'knh_intptr_t',
	'mtd':     'knh_Method_t*',
	'trl':     'knh_Translator_t*',
	'addr':    'knh_KLRInst_t*',
	'u':       'knh_uintptr_t',
}

def getctype(t, v):
	if CTYPE.has_key(t): return CTYPE[t]
	if t == 'f': return 'klr_F%s' % (v)
	return 'knh_%s_t*' % t

def getVMT(t):
	tt = ', VMT_%s' % (t.upper())
	if t == 'sfx': tt = tt + ', VMT_VOID'
	if t == 'int': tt = tt + ' VMTX_INT'
	if t == 'float': tt = tt + ' VMTX_FLOAT'
	return tt

def getsize(t):
	if t == 'sfx': return '+1'
	if t == 'int': return '+VMTSIZE_int'
	if t == 'float': return '+VMTSIZE_float'
	return ''

class KCODE:
	def __init__(self, opcode, line):
		self.tokens = line.split()
		self.name = self.tokens[0]
		self.NAME = self.name.upper()
		self.opcode = opcode
		self.OPCODE = 'OPCODE_%s' % self.name
		self.OPLABEL = 'L_%s' % self.name
		self.ctype = 'klr_%s_t' % self.name
		
		self.level=''
		if '@2' in self.tokens:
			self.level='2'
			self.tokens.remove('@2')
		if '@3' in self.tokens:
			self.level='3'
			self.tokens.remove('@3')
		if '@0' in self.tokens:
			self.level='0'
			self.tokens.remove('@0')
		
		self.size = '%d' % len(self.tokens[1:])
		self.struct = '{'
		for a in self.tokens[1:]:
			if len(a.split(':')) == 1: print line
			n, t = a.split(':')
			ctype = getctype(t, n)
			self.struct = self.struct + getVMT(t)
			self.size = self.size + getsize(t)
		self.struct += ', VMT_VOID}'
		self.struct = self.struct.replace('{,', '{')

#####################################################################

CPROTO = []

KCODE_LIST = []
KSTRUCT_LIST = []
KCODE_STRUCT ={}

c = 0
for line in INSTRUCTIONS.split('\n'):
	if line.startswith("#") or len(line) == 0: continue
	kc = KCODE(c, line)
	KCODE_LIST.append(kc)
	c += 1

#------------------------------------------------------------------------------

def write_KCODE_h(f, kc):
	f.write('''
#define %s ((knh_opcode_t)%d)''' % (kc.OPCODE, kc.opcode))
	f.write('''
typedef struct %s {
	KCODE_HEAD;''' % kc.ctype)
	for a in kc.tokens[1:]:
		n, t = a.split(':')
		if t == "addr" : 
			f.write('''
	union {
		knh_KLRInst_t* lb; /* label*/
		knh_opline_t  *jumppc;
	}; ''')
		else: 
			f.write('''
	%s %s;''' % (getctype(t, n), n))
	f.write('''
} %s;
''' % kc.ctype)

def write_define_h(f):
	for p in CPROTO:
		f.write(p + ';\n')
	for kc in KCODE_LIST:
		write_KCODE_h(f,kc)
	n = len(KCODE_LIST)
	f.write('''
#define OPCODE_MAX ((knh_opcode_t)%d)

#define VMT_VOID     0
#define VMT_ADDR     1
#define VMT_SFPIDX   2
#define VMT_SFX      3
#define VMT_U        4
#define VMT_F        5
#define VMT_CID      6
#define VMT_MTD      7
#define VMT_TRL      8
#define VMT_OBJECT   9
#define VMT_STRING   10
#define VMT_INT      11
#define VMT_FLOAT    12

''' % (n))

#------------------------------------------------------------------------------

def write_common_c(f):
	write_chapter(f, '[common]')
	f.write('''
#define K_USING_VMINLINE
#include "vminline.c"
''')

def write_data_c(f):
	write_chapter(f, '[data]')
	f.write('''

typedef struct {
	char *name;
	size_t size;
	knh_ushort_t types[6];
} knh_OPDATA_t;

static knh_OPDATA_t OPDATA[] = {''')
	for kc in KCODE_LIST:
		n = kc.name
		if n.endswith("_"): n = n[:-1]
		f.write('''
	{"%s", %s, %s}, ''' % (n, kc.size, kc.struct))
	f.write('''
};

void knh_opcode_check(void)
{''')
	for kc in KCODE_LIST:
		f.write('''
	KNH_ASSERT(sizeof(%s) <= sizeof(knh_opline_t));''' % (kc.ctype))
	f.write('''
}

/* ------------------------------------------------------------------------ */
#ifdef K_USING_VMCOUNT
static int opcode_count[OPCODE_MAX] = {0};

/* ------------------------------------------------------------------------ */
void knh_opcode_stat(Ctx *ctx)
{
	int i;
	for(i = 0; i < OPCODE_MAX; i++) {
		KNH_SYSLOG(ctx, LOG_INFO,
				"[VMCODE COUNTER] %s count=%d",
				knh_opcode_tochar(i), opcode_count[i]);
	}
}

/* ------------------------------------------------------------------------ */

void knh_opcode_count(Ctx *ctx, knh_opline_t *c)
{
	int count = c->count;
	opcode_count[c->opcode] += count;
}

#endif
/* ------------------------------------------------------------------------ */

char *knh_opcode_tochar(knh_opcode_t opcode)
{
	if(opcode < OPCODE_MAX) {
		return OPDATA[opcode].name;
	}
	else {
		DBG_P("opcode=%d", (int)opcode);
		return "OPCODE_??";
	}
}

/* ------------------------------------------------------------------------ */
size_t knh_opcode_size(knh_opcode_t opcode)
{
	return OPDATA[opcode].size;
}

/* ------------------------------------------------------------------------ */
knh_bool_t knh_opcode_hasjump(knh_opcode_t opcode)
{
	return (OPDATA[opcode].types[0] == VMT_ADDR);
}
/* ------------------------------------------------------------------------ */
void knh_opcode_traverse(Ctx *ctx, knh_opline_t *c, knh_Ftraverse ftr)
{
	size_t i, size = OPDATA[c->opcode].size;
	for(i = 0; i < size; i++) {
		knh_ushort_t vtype = OPDATA[c->opcode].types[i];
		if(vtype == VMT_OBJECT || vtype == VMT_STRING) {
			KNH_FTR(ctx, ftr, UP(c->p[i]));
		}
	}
}
/* ------------------------------------------------------------------------ */
void knh_opcode_dump(Ctx *ctx, knh_opline_t *c, knh_OutputStream_t *w, knh_opline_t *pc_start)
{
	size_t i, size = OPDATA[c->opcode].size;
	knh_ushort_t *vmt = OPDATA[c->opcode].types;
	if(pc_start == NULL) {
		knh_printf(ctx, w, "[%p:%d] %s(%d)", c, c->line, knh_opcode_tochar(c->opcode), (knh_intptr_t)c->opcode);
	}
	else {
		knh_printf(ctx, w, "L%d(%d): %s(%d)", c - pc_start, c->line, knh_opcode_tochar(c->opcode), (knh_intptr_t)c->opcode);
	}
	for(i = 0; i < size; i++) {
		knh_putc(ctx, w, ' '); 
		switch(vmt[i]) {
		case VMT_VOID: break;
		case VMT_ADDR: 
			if(pc_start == NULL) {
				knh_printf(ctx, w, "%p", c->p[i]); break;
			}
			else {
				knh_printf(ctx, w, "L%d", (knh_opline_t*)c->p[i] - pc_start); break;
			}
		case VMT_SFPIDX:
			knh_printf(ctx, w, "sfp[%d]", c->data[i]); break;
		case VMT_SFX:
			knh_printf(ctx, w, "sfx[%d]+%d", c->data[i], c->data[i+1]); break;
		case VMT_U:
			knh_write_dfmt(ctx, w, KNH_INTPTR_FMT, c->data[i]); break;
		case VMT_F:
			knh_write_vmfunc(ctx, w, c->p[i]); break;
		case VMT_CID:
			knh_write_cid(ctx, w, (knh_class_t)c->data[i]); break;
		case VMT_MTD: if(c->p[i] != NULL) {
			knh_Method_t *mtd = (knh_Method_t*)c->p[i];
			knh_write_cid(ctx, w, DP(mtd)->cid); knh_putc(ctx, w, '.');
			knh_write_mn(ctx, w, DP(mtd)->mn); 
		}
		break;
		case VMT_TRL:
		case VMT_OBJECT:
		case VMT_STRING: {
			knh_Method_t *mtd = knh_lookupFormatter(ctx, CLASS_String, MN__k);
			knh_write_Object(ctx, w, ctx->esp, &mtd, UP(c->p[i]));
			break;
		}
		case VMT_INT: {
			knh_int_t n = ((knh_int_t*)(&(c->p[i])))[0];
			knh_write_ifmt(ctx, w, K_INT_FMT, n); break;
		}
		case VMT_FLOAT:
			knh_write_ffmt(ctx, w, K_FLOAT_FMT, *((knh_float_t*)&(c->p[i]))); break;
		}
	}
	knh_write_EOL(ctx, w);
}

/* ------------------------------------------------------------------------ */
void knh_opcode_idxshift(knh_opline_t *c, int idxshift)
{
	size_t i, size = OPDATA[c->opcode].size;
	knh_ushort_t *vmt = OPDATA[c->opcode].types;
	for(i = 0; i < size; i++) {
		switch(vmt[i]) {
			case VMT_SFPIDX:
			case VMT_SFX: c->data[i] = c->data[i] + idxshift; break;
		}
	}
}

''')

def write_kcftr(f, kc):
	pass
	
def write_kcdump(f, kc):
	##########
	f.write('''
static void %s_dump(Ctx *ctx, knh_opline_t *c, knh_OutputStream_t *w)
{
	%s *op = (%s*)c; 
	knh_write_opcode(ctx, w, op->opcode);''' % (kc.name, kc.ctype, kc.ctype))
	c = 1
	for a in kc.tokens[1:]:
		f.write('''
	''')
		f.write(getfmt(a) % ('(op->a%d)' % c))
		c += 1	
	f.write('''
}
''')

def write_kcshift(f, kc):
	##########
	f.write('''
static void %s_shift(Ctx *ctx, knh_opline_t *c, int shift, int pcshift)
{
	%s *op = (%s*)c; ''' % (kc.name, kc.ctype, kc.ctype))
	c = 1
	for a in kc.tokens[1:]:
		if a == 'addr':
			f.write('''
	char *newpc = ((char*)op->a%d) + pcshift;
	op->a%d = (knh_KLRInst_t*)newpc;
	THREADEDCODE(op->codeaddr = NULL;)''' % (c, c))
		if a == 'sfpidx':
			f.write('''
	op->a%d += shift;''' % c)
		if a == 'sfx':
			f.write('''
	op->a%d.i += shift;''' % c)
		c += 1	
	f.write('''
}
''')

def write_inst_c(f):
	write_common_c(f)
	write_data_c(f)

#############################################################################

def getmacro(kc, label):
	m = 'KLR%s_%s(ctx' % (kc.level, kc.name)
	for a in kc.tokens[1:]:
		v, t = a.split(':')
		if t == 'addr':
			m += ', pc = op->jumppc, %s' % (label)
		else:
			m += ', op->%s' % (v)
	m += ')'
	return m

def write_exec(f):
	write_chapter(f, '[exec]')
	f.write('''
#ifdef K_USING_VMCOUNT
#define VMCOUNT(op)    ((op)->count)++;
#else
#define VMCOUNT(op)
#endif

#ifdef K_USING_THREADEDCODE
#define CASE(x)   L_##x :
#define NEXT_OP  *(pc->codeaddr)
#define JUMP  *(pc->codeaddr)
#define TC(c) 
#define DISPATCH_START(pc) goto *OPJUMP[pc->opcode];
#define DISPATCH_END(pc)
#define GOTO_PC(pc)        goto *(pc->codeaddr);
#else
#define OPJUMP NULL
#define CASE(x)   case OPCODE_##x :
#define NEXT_OP L_HEAD
#define JUMP L_HEAD
#define TC(c)
#define DISPATCH_START(pc) L_HEAD:;switch(pc->opcode) {
#define DISPATCH_END(pc) 	}	KNH_SYSLOG(ctx, LOG_CRIT, "VM", "unknown opcode=%d", pc->opcode); 
#define GOTO_PC(pc)         goto L_HEAD;
#endif/*K_USING_THREADEDCODE*/

knh_opline_t* knh_VirtualMachine_run(Ctx *ctx, knh_sfp_t *sfp, knh_opline_t *pc)
{
#ifdef K_USING_THREADEDCODE
	static void *OPJUMP[] = {''')
	c = 0
	for kc in KCODE_LIST:
		if c % 4 == 0: f.write('\n\t\t')
		f.write('''&&%s, ''' % kc.OPLABEL)
		c += 1
	f.write('''
	};
#endif
	knh_intptr_t vshift = 0;
	knh_opline_t* vpc = NULL;
	DISPATCH_START(pc);
''')
	for kc in KCODE_LIST:
		LB = ''
		LE = ''
		if len(kc.tokens) == 1: 
			LB = 'TC('
			LE = ')'
# DBG_P("%%p %%s", pc-1, knh_opcode_tochar((pc-1)->opcode));
		f.write('''
	CASE(%s) {
		%s/*const*/ %s *op = (%s*)pc;%s; VMCOUNT(pc); pc++;
		%s;  goto NEXT_OP;
	} ''' % (kc.name, LB, kc.ctype, kc.ctype, LE, getmacro(kc, 'JUMP')))
	f.write('''
	DISPATCH_END(pc);
	L_RETURN:;
	return pc;
}

''')

#------------------------------------------------------------------------------

def gen_vm_c(bdir):
	fname = '%s/src/lang/vmcodelibs.c' % bdir
	f = open_c(fname, ['"commons.h"'])
	write_inst_c(f)
	write_exec(f)
	close_c(f, fname)
	
	f = open('include/konoha/konoha_code_.h', 'w')
	f.write('#ifndef %s\n' % 'konoha_code_h_'.upper());
	f.write('#define %s\n' % 'konoha_code_h_'.upper());
	f.write('''// THIS FILE WAS AUTOMATICALLY GENERATED
''')
	write_define_h(f)
	f.write('#endif /* %s */\n' % 'konoha_code_h_'.upper());
	f.close()


#------------------------------------------------------------------------------

if __name__ == '__main__':
	bdir = '.'
	gen_vm_c(".")

