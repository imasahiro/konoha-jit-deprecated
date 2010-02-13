#!/usr/bin/python
import os, sys
import copy
from pygenlib import *

#------------------------------------------------------------------------------

INSTRUCTIONS = """
HALT  @0
ENTER @0
EXEC  @0
YEILD sfpidx  @0
EXIT  @0

TR   f_tr sfpidx cid
OTR  f_tr sfpidx cid
NULTR f_tr sfpidx cid
UNBOX sfpidx
iCAST sfpidx
fCAST sfpidx

OSET  sfpidx Object
NSET  sfpidx int

MOV  sfpidx sfpidx
OMOV sfpidx sfpidx
NMOV sfpidx sfpidx

#MOVx  sfpidx sfx
OMOVx sfpidx sfx
iMOVx sfpidx sfx
fMOVx  sfpidx sfx
bMOVx  sfpidx sfx

XMOV  sfx sfpidx
XMOVx sfx sfx
XOSET sfx Object
XIMOV sfx sfpidx
XFMOV sfx sfpidx
XBMOV sfx sfpidx
MOVe  sfpidx ushort
SWAP   sfpidx sfpidx

CHKNUL sfpidx
CHKTYPE sfpidx cid
CHKNULx sfx
CHKTYPEx sfx cid

LDMETHOD f_method sfpidx mn
CALL  sfpidx  ushort
SCALL  sfpidx ushort Method    @2
VCALL  sfpidx ushort Method    @2

#CHKESP sfpidx
RET
FUNCCALL  @0

#COPYSFP sfpidx
#STR     sfpidx sfpidx mn String
#SSTR    sfpidx sfpidx Method String

SMAP   sfpidx Mapper
SMAPnc   sfpidx Mapper
MAP    sfpidx cid
MAPnc   sfpidx cid
AMAP   sfpidx cid

JMP       addr
NOPJMP    addr
JMPT      addr sfpidx
JMPF      addr sfpidx
JMPF_LOOP addr sfpidx @2
JMPNUL    addr sfpidx
JMPNN     addr sfpidx
JMPchk    addr f_chk sfpidx
JMPcmp    addr f_cmp sfpidx sfpidx    @2
JMPcmpi   addr f_cmpi sfpidx int      @2
JMPcmpf   addr f_cmpf sfpidx float    @2

NEXT  addr sfpidx sfpidx
NEXTf addr f_next cid sfpidx sfpidx 

TRY     addr sfpidx
TRYEND  sfpidx
CATCH   addr sfpidx sfpidx String

#PUSH   sfpidx
#POP    sfpidx

THROW   sfpidx sfpidx 
THROWs  sfpidx sfpidx String
THROW_AGAIN sfpidx

P  flag mn sfpidx
PMSG flag String

bNOT sfpidx sfpidx
iNEG sfpidx sfpidx

iADD sfpidx sfpidx sfpidx
iSUB sfpidx sfpidx sfpidx
iMUL sfpidx sfpidx sfpidx
iDIV  sfpidx sfpidx sfpidx
iMOD  sfpidx sfpidx sfpidx
iDIVn  sfpidx sfpidx int
iADDn sfpidx sfpidx int
iSUBn sfpidx sfpidx int
iMULn sfpidx sfpidx int
iMODn  sfpidx sfpidx int

iEQ   sfpidx sfpidx sfpidx
iNEQ   sfpidx sfpidx sfpidx
iLT   sfpidx sfpidx sfpidx
iLTE  sfpidx sfpidx sfpidx
iGT   sfpidx sfpidx sfpidx
iGTE  sfpidx sfpidx sfpidx
iEQn  sfpidx sfpidx int
iNEQn  sfpidx sfpidx int
iLTn   sfpidx sfpidx int
iLTEn  sfpidx sfpidx int
iGTn  sfpidx sfpidx int
iGTEn  sfpidx sfpidx int

fNEG sfpidx sfpidx
fADD sfpidx sfpidx sfpidx
fSUB sfpidx sfpidx sfpidx
fMUL sfpidx sfpidx sfpidx
fDIV  sfpidx sfpidx sfpidx
fADDn sfpidx sfpidx float
fSUBn sfpidx sfpidx float
fMULn sfpidx sfpidx float
fDIVn  sfpidx sfpidx float

fEQ   sfpidx sfpidx sfpidx
fNEQ   sfpidx sfpidx sfpidx
fEQn  sfpidx sfpidx float
fNEQn  sfpidx sfpidx float

fLT   sfpidx sfpidx sfpidx
fLTE  sfpidx sfpidx sfpidx
fGT   sfpidx sfpidx sfpidx
fGTE  sfpidx sfpidx sfpidx
fLTn   sfpidx sfpidx float
fLTEn  sfpidx sfpidx float
fGTn  sfpidx sfpidx  float
fGTEn  sfpidx sfpidx float

OEVAL     f_oeval    sfpidx sfpidx
GETIDX    f_getidx sfpidx sfpidx sfpidx
SETIDX    f_setidx sfpidx sfpidx sfpidx sfpidx
GETIDXn   f_getidx sfpidx sfpidx intptr
SETIDXn   f_setidx sfpidx sfpidx sfpidx intptr

THCODE  @0
LABEL   intptr String
PROBE   f_probe ushort  @0

NOP
"""

CTYPE = {
	'sfpidx' : 'knh_sfpidx_t', 
	'sfx' : 'knh_sfx_t',
	'sfe' : 'knh_sfe_t', 
	'bool': 'knh_bool_t',
	'int': 'knh_int_t',
	'float': 'knh_float_t',
	'intptr': 'knh_intptr_t',
	'cid': 'knh_class_t',
	'type': 'knh_type_t',
	'mn': 'knh_methodn_t',
	'flag': 'knh_flag_t',
	'addr': 'knh_KLRInst_t*',
	'short': 'knh_short_t',
	'ushort': 'knh_ushort_t',
}

def getctype(a):
	if CTYPE.has_key(a): return CTYPE[a]
	if a.startswith("f_"): return 'klr_F%s' % (a.replace('f_', ''))
	return 'knh_%s_t*' % a

def getfmt(a):
	if CTYPE.has_key(a): return "knh_write__%s(ctx, w, %%s);" % a
	if a.startswith("f_"): return "knh_write__func(ctx, w, %s);"
	return "knh_write__OBJ(ctx, w, UP(%s));"

class KCODE:
	def __init__(self, opcode, line):
		self.tokens = line.split()
		self.name = self.tokens[0]
		self.NAME = self.name.upper()
		self.opcode = opcode
		self.OPCODE = 'OPCODE_%s' % self.name
		self.SIZE = 'OPSIZE_%s' % self.name
		self.OPLABEL = 'L_%s' % self.name
		self.ctype = 'klr_%s_t' % self.name
		
		self.level=''
		if '@2' in self.tokens:
			self.level='2'
			self.tokens.remove('@2')
		if '@0' in self.tokens:
			self.level='0'
			self.tokens.remove('@0')
		
		self.args = '('
		self.targs = ''
		self.hasLabel = False
		self.label = '0'
		self.hasObject = False
		self.ftr = 'EXIT_traverse'
		self.struct = ''
		self.shift = 'EXIT_shift'
		c = 1
		for a in self.tokens[1:]:
			ctype = getctype(a)
			if a[0].isupper(): 
				self.hasObject = True 
				self.ftr = '%s_traverse' % (self.name)
			if a == 'addr': 
				self.hasLabel = True
				self.label = '1'
			self.targs = self.targs + ',%s a%d' % (ctype, c)
			self.args = self.args + 'a%d,' % (c)
			self.struct = self.struct + a
			c += 1
		if 'addr' in self.struct or 'sfpidx' in self.struct or 'sfx' in self.struct:
			self.shift = None 
		if self.args[len(self.args) - 1] == ',':
			self.args = self.args[:len(self.args)-1] + ')'

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
	if KCODE_STRUCT.has_key(kc.struct):
		kc.dump = '%s_dump' % KCODE_STRUCT[kc.struct].name
		if kc.shift is None:
			kc.shift = '%s_shift' % KCODE_STRUCT[kc.struct].name
	else:
		KCODE_STRUCT[kc.struct] = kc
		KSTRUCT_LIST.append(kc)
		kc.dump = '%s_dump' % kc.name
		if kc.shift is None:
			kc.shift = '%s_shift' % kc.name
	c += 1

#------------------------------------------------------------------------------

def write_KCODE_h(f, kc):
	f.write('''
#define %s ((knh_opcode_t)%d)''' % (kc.OPCODE, kc.opcode))

	f.write('''
typedef struct {
	THREADEDCODE(void *codeaddr;)
	knh_opcode_t opcode;''')
	c = 0
	for a in kc.tokens[1:]:
		c += 1
		if a == "addr" : 
			f.write('''
	THREADEDCODE(void *jumpaddr;)
	knh_KLRInst_t* a1;  /* label */''')
		else: 
			f.write('''
	%s a%d;''' % (getctype(a), c))
	
	f.write('''
	void *nextaddr;   /* sameas next codeaddr */
} %s;
''' % kc.ctype)
	f.write('''
#define %s  (sizeof(%s)-sizeof(void*))
''' % (kc.SIZE, kc.ctype))	
	#proto = 'knh_code_t* KNH_ASM_%s_(Ctx *ctx, knh_Gamma_t *o%s)' % (kc.name, kc.args())

def write_define_h(f):
	for p in CPROTO:
		f.write(p + ';\n')
	for kc in KCODE_LIST:
		write_KCODE_h(f,kc)

def write_jit_def_h(f):
	for kc in KCODE_LIST:
		m = 'KLR_%s(ctx' % kc.name;
		a = "," + kc.args[1:]
		if len(a) == 1:
			a = ")"
		f.write('''
void JIT_OP_%s_(Ctx *ctx, knh_sfp_t* sfp%s)
{
	%s%s;
}
''' % (kc.name,kc.targs, m,a))


#------------------------------------------------------------------------------


def write_common_c(f):
	write_chapter(f, '[common]')
	f.write('''
#define knh_write__flag(ctx, w, f)  knh_write_flag(ctx, w, f)	
static void knh_write_opcode(Ctx *ctx, knh_OutputStream_t* w, int opcode)
{
	knh_printf(ctx, w, "%s(%d)", knh_opcode_tochar(opcode), (knh_intptr_t)opcode);
}
static void knh_write__addr(Ctx *ctx, knh_OutputStream_t* w, void *a)
{
	knh_putc(ctx, w, ' '); knh_printf(ctx, w, "%p", a);
}
static void knh_write__cid(Ctx *ctx, knh_OutputStream_t* w, knh_class_t cid)
{
	knh_putc(ctx, w, ' '); knh_write_cid(ctx, w, cid);
}
static void knh_write__float(Ctx *ctx, knh_OutputStream_t* w, knh_float_t a)
{
	knh_putc(ctx, w, ' '); knh_write_ffmt(ctx, w, KNH_FLOAT_FMT, a);
}
static void knh_write__int(Ctx *ctx, knh_OutputStream_t* w, knh_int_t a)
{
	knh_putc(ctx, w, ' '); knh_write_ifmt(ctx, w, KNH_INT_FMT, a);
}
static void knh_write__intptr(Ctx *ctx, knh_OutputStream_t* w, knh_intptr_t a)
{
	knh_putc(ctx, w, ' '); knh_write_dfmt(ctx, w, KNH_INTPTR_FMT, a);
}
static void knh_write__OBJ(Ctx *ctx, knh_OutputStream_t* w, Object *a)
{
	knh_putc(ctx, w, ' ');knh_format(ctx, w, METHODN__k, a, KNH_NULL);
}
static void knh_write__sfpidx(Ctx *ctx, knh_OutputStream_t* w, knh_sfpidx_t a)
{
	knh_printf(ctx, w, " sfp[%d]", (knh_intptr_t)a);
}
static void knh_write__sfx(Ctx *ctx, knh_OutputStream_t* w, knh_sfx_t a)
{
	knh_printf(ctx, w, " sfx[%d]+%d", (knh_intptr_t)a.i, (knh_intptr_t)a.n);
}
static void knh_write__ushort(Ctx *ctx, knh_OutputStream_t* w, knh_ushort_t a)
{
	knh_putc(ctx, w, ' ');
	knh_write_dfmt(ctx, w, KNH_INTPTR_FMT, (knh_intptr_t)a);
}
static void knh_write__mn(Ctx *ctx, knh_OutputStream_t* w, knh_methodn_t a)
{
	knh_putc(ctx, w, ' '); knh_write_mn(ctx, w, TYPE_void, (knh_methodn_t)a);
}
//static void knh_write__type(Ctx *ctx, knh_OutputStream_t* w, knh_type_t a)
//{
//	knh_putc(ctx, w, ' '); knh_write_type(ctx, w, (knh_type_t)a);
//}
static void EXIT_traverse(Ctx *ctx, knh_inst_t *c, knh_Ftraverse ftr)
{
}
static void EXIT_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
}
''')

def write_data_c(f):
	write_chapter(f, '[data]')
	f.write('''
typedef void (*codeftr)(Ctx *, knh_inst_t*, knh_Ftraverse);
typedef void (*codedump)(Ctx *, knh_inst_t*, knh_OutputStream_t*);
typedef void (*codeshift)(Ctx *, knh_inst_t*, int, int);

typedef struct knh_OPDATA_t {
	char *name;
	size_t size;
	knh_bool_t hasjump;
	codeftr cftr;
	codedump cdump;
	codeshift cshift;
} knh_OPDATA_t;

static knh_OPDATA_t OPDATA[] = {''')
	for kc in KCODE_LIST:
		f.write('''
	{"%s", %s, %s, %s, %s, %s}, ''' % (kc.name, kc.SIZE, kc.label, kc.ftr, kc.dump, kc.shift))
	f.write('''
};

/* ------------------------------------------------------------------------ */

char *knh_opcode_tochar(int opcode)
{
	return OPDATA[opcode].name;
}

/* ------------------------------------------------------------------------ */

size_t knh_opcode_size(int opcode)
{
	return OPDATA[opcode].size;
}

/* ------------------------------------------------------------------------ */

knh_bool_t knh_opcode_hasjump(int opcode)
{
	return OPDATA[opcode].hasjump;
}

/* ------------------------------------------------------------------------ */

void knh_opcode_traverse(Ctx *ctx, knh_inst_t *c, knh_Ftraverse ftr)
{
	int opcode = KNH_OPCODE(c);
	OPDATA[opcode].cftr(ctx, c, ftr);
}

/* ------------------------------------------------------------------------ */

void knh_opcode_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
{
	int opcode = KNH_OPCODE(c);
	OPDATA[opcode].cdump(ctx, c, w);
}

/* ------------------------------------------------------------------------ */

void knh_opcode_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	int opcode = KNH_OPCODE(c);
	OPDATA[opcode].cshift(ctx, c, shift, pcshift);
}

''')

def write_kcftr(f, kc):
	f.write('''
static void %s_traverse(Ctx *ctx, knh_inst_t *c, knh_Ftraverse ftr)
{
	%s *op = (%s*)c; 
	DBG_ASSERT(op->opcode == %s);''' % (kc.name, kc.ctype, kc.ctype, kc.opcode))
	c = 1;
	for a in kc.tokens[1:]:
		if a[0].isupper(): 
			f.write('''
	ftr(ctx, UP(op->a%d));''' % c)
		c += 1
	f.write('''
}
''')

def write_kcdump(f, kc):
	##########
	f.write('''
static void %s_dump(Ctx *ctx, knh_inst_t *c, knh_OutputStream_t *w)
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
static void %s_shift(Ctx *ctx, knh_inst_t *c, int shift, int pcshift)
{
	%s *op = (%s*)c; ''' % (kc.name, kc.ctype, kc.ctype))
	c = 1
	for a in kc.tokens[1:]:
		if a == 'addr':
			f.write('''
	knh_code_t *newpc = ((knh_code_t*)op->a%d) + pcshift;
	op->a%d = (knh_KLRInst_t*)newpc;
	THREADEDCODE(op->codeaddr = NULL;)
	THREADEDCODE(op->jumpaddr = NULL;)''' % (c, c))
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
	for kc in KCODE_LIST:
		if not kc.hasObject: continue
		write_kcftr(f, kc)
	for kc in KSTRUCT_LIST:
		write_kcdump(f, kc)
	for kc in KSTRUCT_LIST:
		if 'addr' in kc.struct or 'sfpidx' in kc.struct or 'sfx' in kc.struct:
			write_kcshift(f, kc)
	write_data_c(f)

#############################################################################

def getmacro(kc, label):
	m = 'KLR%s_%s(ctx' % (kc.level, kc.name)
	c = 1
	for a in kc.tokens[1:]:
		if a == 'addr':
			m += ', pc = ((knh_inst_t*)op)->jumppc, %s' % (label)
		else:
			m += ', op->a%d' % (c)
		c += 1
	m += ')'
	return m

def write_exec(f):
	write_chapter(f, '[exec]')
	f.write('''
#ifdef KNH_USING_THREADEDCODE
#define CASE(x)   L_##x :
#define NEXT_OP  *(op->nextaddr)
#define JUMP  *(op->jumpaddr)
#define TC(c)   c
#define DISPATCH_START(pc) goto *OPJUMP[KNH_OPCODE(pc)];
#define DISPATCH_END(pc)
#else
#define OPJUMP NULL
#define CASE(x)   case OPCODE_##x :
#define NEXT_OP L_HEAD
#define JUMP L_HEAD
#define TC(c)    
#define DISPATCH_START(pc) L_HEAD:;switch(KNH_OPCODE(pc)) {
#define DISPATCH_END(pc) \
	}\
	KNH_SYSLOG(ctx, LOG_ERR, "unknown opcode=%d", KNH_OPCODE(pc)); \
	goto L_HEAD;
#endif/*KNH_USING_THREADEDCODE*/

knh_code_t* knh_VirtualMachine_run(Ctx *ctx, knh_sfp_t *sfp, knh_code_t *pc)
{
#ifdef KNH_USING_THREADEDCODE
	static void *OPJUMP[] = {''')
	c = 0
	for kc in KCODE_LIST:
		if c % 4 == 0: f.write('\n\t\t')
		f.write('''&&%s, ''' % kc.OPLABEL)
		c += 1
	f.write('''
	};
#endif
#if defined(OPCODE_VCALL)
	L_KONOHACALL:;
#endif
	int sfpidx = sfp - ctx->stack;
	//register knh_code_t *pc = (sfp[-1].mtd)->pc_start;
	//DBG_ASSERT(IS_Method(sfp[-1].mtd));
	DISPATCH_START(pc);
''')
	for kc in KCODE_LIST:
		LB = ''
		LE = ''
		if len(kc.tokens) == 1: 
			LB = 'TC('
			LE = ')'
		f.write('''
	CASE(%s) {
		%sconst %s *op = (%s*)pc;%s
		%s;
		pc += %s;
		goto NEXT_OP;
	} ''' % (kc.name, LB, kc.ctype, kc.ctype, LE, getmacro(kc, 'JUMP'), kc.SIZE))

	f.write('''
	DISPATCH_END(pc);
	return NULL;
}

''')

#------------------------------------------------------------------------------

def gen_vm_c(bdir):
	fname = '%s/src/kc/konoha_code_.c' % bdir
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

