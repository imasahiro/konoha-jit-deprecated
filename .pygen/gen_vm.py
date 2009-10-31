#!/usr/bin/python
import os, sys
import copy
from pygenlib import *

#------------------------------------------------------------------------------

INSTRUCTIONS = """
HALT

##MOV
MOVa sfpidx sfpidx
MOVn sfpidx sfpidx
MOVo sfpidx Object
MOVi sfpidx int
MOVx sfpidx sfx
MOVDEF sfpidx cid
MOVSYS sfpidx ushort 

XMOVs sfx sfpidx
XMOVo sfx Object
XMOVx sfx sfx
XMOVDEF sfx cid
XMOVSYS sfx ushort

MOVxi sfpidx sfx
XMOVsi sfx sfpidx
XMOVoi sfx Object
XMOVxi sfx sfx
XMOVxio sfx sfx
XMOVxBXi sfx sfx cid

MOVxf  sfpidx sfx
XMOVsf sfx sfpidx
XMOVof sfx Object
XMOVxf sfx sfx
XMOVxfo sfx sfx
XMOVxBXf sfx sfx cid

MOVxb  sfpidx sfx
XMOVsb sfx sfpidx
XMOVob sfx Object
XMOVxb sfx sfx

EMOVs sfe sfe
EMOVo sfe Object
EMOVe sfe sfe
EMOVDEF sfe cid
EMOVSYS sfe ushort

SWAP  sfpidx sfpidx

PARAMDEF sfpidx cid
PARAMo sfpidx Object
PARAMPROP sfpidx sfpidx
PARAMS sfpidx cid

CHKESP sfpidx

RET
RETn sfpidx sfpidx
RETa sfpidx sfpidx
RETo sfpidx Object
RETx sfpidx sfx
YEILDBREAK

BOX sfpidx cid
BOXnc sfpidx cid
NNBOX sfpidx cid
NNBOXnc sfpidx cid
UNBOX sfpidx

CHKNUL sfpidx
CHKNULx sfx
CHKTYPE sfpidx cid
#CHECKNNTYPE sfpidx type

#NCALL 
FCALL sfpidx ushort sfpidx Method
RCALL sfpidx ushort
SCALL sfpidx ushort Method
AINVOKE sfpidx ushort
CALL  sfpidx ushort mn 
ACALL sfpidx ushort mn
NEW   sfpidx flag cid ushort Method

COPYSFP sfpidx

STR     sfpidx sfpidx mn String
SSTR    sfpidx sfpidx Method String

SMAP   sfpidx Mapper
SMAPnc   sfpidx Mapper
MAP    sfpidx cid
MAPnc   sfpidx cid
AMAP   sfpidx cid
NNMAP   sfpidx cid

JMP     addr
SKIP    addr
bJIFT    addr sfpidx
bJIFF    addr sfpidx
bJIFF_LOOP addr sfpidx
JIFNUL  addr sfpidx
JIFNN   addr sfpidx

NEXT addr sfpidx sfpidx
INEXT addr cid sfpidx sfpidx 
#MAPNEXT addr cid sfpidx sfpidx 
#SMAPNEXT addr sfpidx sfpidx OBJ

TRY     addr sfpidx
TRYEND  sfpidx
CATCH   addr sfpidx sfpidx String

PUSH   sfpidx
POP    sfpidx

THROW   ushort sfpidx 
THROWs  ushort String
THROW_AGAIN sfpidx

P  flag mn sfpidx
PMSG flag String

iCAST sfpidx
inCAST sfpidx
fCAST sfpidx
fnCAST sfpidx

bNOT sfpidx sfpidx

iNEG sfpidx sfpidx
iADD sfpidx sfpidx sfpidx
iADDn sfpidx sfpidx int
iSUB sfpidx sfpidx sfpidx
iSUBn sfpidx sfpidx int
iMUL sfpidx sfpidx sfpidx
iMULn sfpidx sfpidx int
iDIV  sfpidx sfpidx sfpidx
iDIVn  sfpidx sfpidx int
iMOD  sfpidx sfpidx sfpidx
iMODn  sfpidx sfpidx int

iEQ   sfpidx sfpidx sfpidx
iEQn  sfpidx sfpidx int
iNEQ   sfpidx sfpidx sfpidx
iNEQn  sfpidx sfpidx int

iLT   sfpidx sfpidx sfpidx
iLTn   sfpidx sfpidx int
iLTE  sfpidx sfpidx sfpidx
iLTEn  sfpidx sfpidx int
iGT   sfpidx sfpidx sfpidx
iGTn  sfpidx sfpidx int
iGTE  sfpidx sfpidx sfpidx
iGTEn  sfpidx sfpidx int

fNEG sfpidx sfpidx
fADD sfpidx sfpidx sfpidx
fADDn sfpidx sfpidx float
fSUB sfpidx sfpidx sfpidx
fSUBn sfpidx sfpidx float
fMUL sfpidx sfpidx sfpidx
fMULn sfpidx sfpidx float
fDIV  sfpidx sfpidx sfpidx
fDIVn  sfpidx sfpidx float

fEQ   sfpidx sfpidx sfpidx
fEQn  sfpidx sfpidx float
fNEQ   sfpidx sfpidx sfpidx
fNEQn  sfpidx sfpidx float

fLT   sfpidx sfpidx sfpidx
fLTn   sfpidx sfpidx float
fLTE  sfpidx sfpidx sfpidx
fLTEn  sfpidx sfpidx float
fGT   sfpidx sfpidx sfpidx
fGTn  sfpidx sfpidx float
fGTE  sfpidx sfpidx sfpidx
fGTEn  sfpidx sfpidx float

ARYGET    sfpidx sfpidx sfpidx
ARYGETn   sfpidx sfpidx intptr
iARYGET   sfpidx sfpidx sfpidx
iARYGETn  sfpidx sfpidx intptr
fARYGET   sfpidx sfpidx sfpidx
fARYGETn  sfpidx sfpidx intptr

ARYSET    sfpidx sfpidx sfpidx
ARYSETn   sfpidx sfpidx intptr
iARYSET   sfpidx sfpidx sfpidx
iARYSETn  sfpidx sfpidx intptr
fARYSET   sfpidx sfpidx sfpidx
fARYSETn  sfpidx sfpidx intptr

THCODE  sfpidx

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
	return 'knh_%s_t*' % a

def getfmt(a):
	if CTYPE.has_key(a): return "knh_write__%s(ctx, w, %%s);" % a
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

		self.args = '('
		self.targs = ''
		self.hasLabel = False
		self.label = '0'
		self.hasObject = False
		self.ftr = 'HALT_traverse'
		self.struct = ''
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
	else:
		KCODE_STRUCT[kc.struct] = kc
		KSTRUCT_LIST.append(kc)
		kc.dump = '%s_dump' % kc.name
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
static void knh_write__sfe(Ctx *ctx, knh_OutputStream_t* w, knh_sfe_t a)
{
	knh_printf(ctx, w, " sfe[%d]", (knh_intptr_t)a);
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
static void HALT_traverse(Ctx *ctx, knh_inst_t *c, knh_ftraverse ftr)
{
}
''')

def write_data_c(f):
	write_chapter(f, '[data]')
	f.write('''
typedef void (*codeftr)(Ctx *, knh_inst_t*, knh_ftraverse);
typedef void (*codedump)(Ctx *, knh_inst_t*, knh_OutputStream_t*);

typedef struct knh_OPDATA_t {
	char *name;
	size_t size;
	knh_bool_t hasjump;
	codeftr cftr;
	codedump cdump;
} knh_OPDATA_t;

static knh_OPDATA_t OPDATA[] = {''')
	for kc in KCODE_LIST:
		f.write('''
	{"%s", %s, %s, %s, %s}, ''' % (kc.name, kc.SIZE, kc.label, kc.ftr, kc.dump))
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

void knh_opcode_traverse(Ctx *ctx, knh_inst_t *c, knh_ftraverse ftr)
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

''')


	
def write_kcftr(f, kc):
	f.write('''
static void %s_traverse(Ctx *ctx, knh_inst_t *c, knh_ftraverse ftr)
{
	%s *op = (%s*)c; 
	DBG2_ASSERT(op->opcode == %s);''' % (kc.name, kc.ctype, kc.ctype, kc.opcode))
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

def write_inst_c(f):
	write_common_c(f)
	for kc in KCODE_LIST:
		if not kc.hasObject: continue
		write_kcftr(f, kc)
	for kc in KSTRUCT_LIST:
		write_kcdump(f, kc)
	write_data_c(f)

#############################################################################

def getmacro(kc, label):
	m = 'KLR_%s(ctx' % kc.name
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
#define CASE(L, OP)   L:
#define NEXT  *(op->nextaddr)
#define JUMP  *(op->jumpaddr)
#define TC(c)   c
#else
#define OPJUMP NULL
#define CASE(L, OP)   case OP :
#define NEXT L_HEAD
#define JUMP L_HEAD
#define TC(c)    
#endif/*KNH_USING_THREADEDCODE*/

METHOD knh_KLRCode_exec(Ctx *ctx, knh_sfp_t *sfp)
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
	register knh_code_t *pc = (sfp[-1].mtd)->pc_start;
	goto *OPJUMP[KNH_OPCODE(pc)]; /* this is needed to init */
#else
	register knh_code_t *pc = (sfp[-1].mtd)->pc_start;
	L_HEAD:;
	switch(KNH_OPCODE(pc)) {
#endif
''')
	for kc in KCODE_LIST:
		LB = ''
		LE = ''
		if len(kc.tokens) == 1: 
			LB = 'TC('
			LE = ')'
		f.write('''
	CASE(%s, %s) {
		%sconst %s *op = (%s*)pc;%s
		%s;
		pc += %s;
		goto NEXT;
	} ''' % (kc.OPLABEL, kc.OPCODE, LB, kc.ctype, kc.ctype, LE, getmacro(kc, 'JUMP'), kc.SIZE))

	f.write('''
#ifndef KNH_USING_THREADEDCODE
	}
	KNH_WARNING(ctx, "unknown opcode=%d", KNH_OPCODE(pc));
	goto L_HEAD;
#endif
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

	f = open('include/ijt/vm.h', 'w')
	f.write('#ifndef %s\n' % 'ijt_vm_h_'.upper());
	f.write('#define %s\n' % 'ijt_vm_h_'.upper());
	f.write('''// THIS FILE WAS AUTOMATICALLY GENERATED
''')
	write_jit_def_h(f)
	f.write('#endif /* %s */\n' % 'ijt_vm_h_'.upper());
	f.close()


#------------------------------------------------------------------------------

if __name__ == '__main__':
	bdir = '.'
	gen_vm_c(".")

