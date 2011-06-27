#include "commons.h"
#include <konoha1/konoha_vm.h>

#include <llvm/LLVMContext.h>
#include <llvm/Module.h>
#include <llvm/Constants.h>
#include <llvm/GlobalVariable.h>
#include <llvm/Function.h>
#include <llvm/BasicBlock.h>
#include <llvm/Instructions.h>
#include <llvm/Support/ManagedStatic.h>
#include <llvm/Pass.h>
#include <llvm/PassManager.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/Analysis/Passes.h>
#include <llvm/Support/IRBuilder.h>
#include <llvm/ExecutionEngine/JIT.h>
#include <llvm/ExecutionEngine/Interpreter.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/Target/TargetSelect.h>
#include <llvm/Target/TargetData.h>
#include <llvm/DerivedTypes.h>
#include <llvm/Transforms/Scalar.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>

using namespace llvm;

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

#define PJIT
#define TJIT_DUMP
#if !defined(__amd64__)
#error "Your Architecture is not surpported."
#define ZERO 0UL
#else
#define ZERO 0ULL
#endif

#if defined(PJIT)
#define USE_NOPTABLE
#define K_INTERNAL
#endif

#include"commons.h"

#if defined(PJIT)
#if defined(K_USING_DEBUG) || 0
#define PCODE_DUMP
#endif

#define op_getmtd(op) (op)->mtdNC

#define SHIFT (0x08)

#undef TODO
#define TODO() asm volatile("int3");
#include "pasm.h"
#ifdef USE_NOPTABLE
#define ASM_NOP_MAX 7
static const unsigned char x86nops[] = {
    /* 1byte nop */ 0x90,
    /* 2byte nop */ 0x66, 0x90,
    /* 3byte nop */ 0x0f, 0x1f, 0x00,
    /* 4byte nop */ 0x0f, 0x1f, 0x40, 0x00,
    /* 5byte nop */ 0x0f, 0x1f, 0x44, 0x00, 0x00,
    /* 6byte nop */ 0x66, 0x0f, 0x1f, 0x44, 0x00, 0x00,
    /* 7byte nop */ 0x0f, 0x1f, 0x80, 0x00, 0x00, 0x00, 0x00
};

static const unsigned char * x86_noptable[ASM_NOP_MAX+1] = {
    NULL,
    x86nops,
    x86nops + 1,
    x86nops + 1 + 2,
    x86nops + 1 + 2 + 3,
    x86nops + 1 + 2 + 3 + 4,
    x86nops + 1 + 2 + 3 + 4 + 5,
    x86nops + 1 + 2 + 3 + 4 + 5 + 6,
};

static void add_nops(void *insns, unsigned int len)
{
    const unsigned char **noptable = x86_noptable;
    while (len > 0) {
        unsigned int noplen = len;
        if (noplen > ASM_NOP_MAX)
            noplen = ASM_NOP_MAX;
        memcpy(insns, noptable[noplen], noplen);
        insns = (void*)((intptr_t)insns + noplen);
        len -= noplen;
    }
}
#endif

#define BasicBlock_add_(ctx, bb, op) \
    knh_BasicBlock_add_(ctx, bb, 0, op, sizeof(knh_opline_t));

/* ----------------------------------------------------------------- */
#include <sys/mman.h>
static struct mem {
    void *block;
    size_t size;
    size_t pos;
} memblock[10] = {};
//static int _index = 0;
static int _cur = 0;

#define ONE_MB (1024 * 1024)
/* allocate executable memory */
void *__xmalloc(Ctx *ctx, size_t size)
{
    void *block = NULL;
    if (memblock[_cur].block == NULL) {
        block = mmap(NULL, ONE_MB, PROT_READ|PROT_WRITE|PROT_EXEC,
                MAP_PRIVATE | MAP_ANON, -1, 0);
        memblock[_cur].block = block;
        memblock[_cur].size  = ONE_MB;
        memblock[_cur].pos   = size;
    } else {
        if (memblock[_cur].pos + size > memblock[_cur].size) {
            TODO();
            exit(1);
        }
        block = (void*)((intptr_t)memblock[_cur].block + memblock[_cur].pos);
        memblock[_cur].pos += size;
        DBG_P("block=%p & size=%zd",block, size);
    }
    return block;
}
/* ----------------------------------------------------------------- */

void knh_xfree(Ctx *ctx, void *block, size_t size)
{
    munmap(block, size);
}


void jit_dump(void *mem, int size)
{
    int i = 0;
    unsigned char *code = (unsigned char*) mem;
    fprintf(stderr, "code start=%p\n", code);
    for (i = 0; i < size; i++) {
        fprintf(stderr, " %02x", code[i]);
        if (i%16 == 15) {
            fprintf(stderr, "\n");
        }
    }
    if (i%16) fprintf(stderr, "\n");
    fprintf(stderr, "code   end=%p\n", code + i);
    fprintf(stderr, "\n");
}

knh_bool_t knh_opcode_usedef(knh_opcode_t opcode, int i);
typedef knh_BasicBlock_t bb_t;

typedef union {
    knh_int_t i;
    knh_float_t f;
} ndata_t;

#define opcode(code) (code - OPCODE_MAX)
#define PTYPE_NOP  0
#define PTYPE_REG  1
#define PTYPE_OBJ  2
#define PTYPE_NDAT 3
#define PTYPE_DATA 4
#define PTYPE_OBJP 5
#define PTYPE_BB   6
#define PTYPE_COND 7
#define PTYPE_SFP  8
#define PTYPE_FUNC 9
#define PTYPE_IDX  10
#define PTYPE_SELF 11
#define PTYPE_FLAT 12
#define NOP  PTYPE_NOP
#define REG  PTYPE_REG
#define OBJ  PTYPE_OBJ
#define NDAT PTYPE_NDAT
#define DAT  PTYPE_DATA
#define OBJP PTYPE_OBJP
#define BB   PTYPE_BB
#define COND PTYPE_COND
//#define SFP_ PTYPE_SFP
#define FUNC PTYPE_FUNC
#define IDX  PTYPE_IDX
#define SELF PTYPE_SELF
#define FLAT PTYPE_FLAT

#define RSFP 80/*r14*/
#define RCTX 81/*r15*/

#define PJIT_RELOCATION_SYMBOL      (knh_intptr_t)0x0abadc0de /* a bad code! must rewrite it. */
#define PJIT_RELOCATION_SYMBOL_FUNC (knh_intptr_t)0x1abadc0de /* a bad code! must rewrite it. */
#define PJIT_RELOCATION_SYMBOL_SELF (knh_intptr_t)0x2abadc0de /* a bad code! must rewrite it. */

static void pcode_dump(pcode_t *op);
//static void pjit_addRelocationCode(struct pjit *pjit, size_t size, int type, knh_uintptr_t target);
#define _dump(op)  if(pjit->state == PJIT_EMIT) { pcode_dump(op); }
//#define pcode_setline(op, pjit) if(pjit->state == PJIT_INIT) { op->pos = pjit->cur_pos; } fprintf(stderr, "op[%x] ", op->pos); pcode_dump(op);
#define pcode_setline(op, pjit) if(pjit->state == PJIT_INIT) { op->pos = pjit->cur_pos; }
//static int isRetVal(knh_sfpidx_t a)
//{
//    if (a < 0) {
//        return 1;
//    }
//    return 0;
//}
static int reg_isUsed(reg_t r);
static IRBuilder<> *BUILDER();
static Module *MODULE();
static Function *FUNCTION();
static Value *VALUE_get(struct pjit *pjit, int idx);
static void VALUE_set(struct pjit *pjit, int idx, Value *v);
#define LLVM_CONTEXT() (llvm::getGlobalContext())
#define LLVMTYPE_Void  (Type::getVoidTy(LLVM_CONTEXT()))
#define LLVMTYPE_Int   (Type::getInt64Ty(LLVM_CONTEXT()))
#define LLVMTYPE_Bool  (Type::getInt64Ty(LLVM_CONTEXT()))
#define LLVMTYPE_Float (Type::getDoubleTy(LLVM_CONTEXT()))
static const Type *LLVMTYPE_ObjectField = NULL;
static const Type *LLVMTYPE_Object = NULL;
static const Type *LLVMTYPE_Array = NULL;
static const Type *LLVMTYPE_Method = NULL;
static const Type *LLVMTYPE_context = NULL;
static const Type *LLVMTYPE_fcall = NULL;
static const Type *LLVMTYPE_checkin = NULL;
static const Type *LLVMTYPE_checkout = NULL;
static const Type *LLVMTYPE_sfp    = NULL;
static const Type *LLVMTYPE_rbp    = NULL;
static const Type *LLVMTYPE_opline = NULL;
static const Type *LLVMTYPE_itr    = NULL;
static const Type *LLVMTYPE_Iterator = NULL;

static Value *getctx(struct pjit *pjit)
{
		Function::arg_iterator args = FUNCTION()->arg_begin();
		return args;
}
static Value *getrbp(struct pjit *pjit)
{
		Function::arg_iterator args = FUNCTION()->arg_begin();
		args++;/* ctx */
		return args;
}
static Value *getop(struct pjit *pjit)
{
		Function::arg_iterator args = FUNCTION()->arg_begin();
		args++;/* ctx */
		args++;/* rbp */
		return args;
}
static Value *VNAME_(Value *v, const char *name)
{
	v->setName(name);
	return v;
}


#define RSPARG2 0xd8
#define BASE_SP 0xe8
static Value *ToInt(Value *v)
{
    v = BUILDER()->CreatePtrToInt(v, LLVMTYPE_Int);
    //return BUILDER()->CreateBitCast(v, LLVMTYPE_Int);
    return v;
}
static Value *ToFloat(Value *v)
{
    v = BUILDER()->CreatePtrToInt(v, LLVMTYPE_Int);
    return BUILDER()->CreateBitCast(v, LLVMTYPE_Float);
}

#define EMITMODE(pjit) (pjit->state == PJIT_EMIT) 

static void pasm_init(struct pjit *pjit, pcode_t *op)
{
    int i = 0;
    pcode_setline(op, pjit);
    push(pjit, rbp);
    mov (pjit, rsp, rbp);
    mov2bp(pjit, rdx, RSPARG2, rbp);
    if (reg_isUsed(r12)) {
        mov2bp(pjit, r12, BASE_SP + i++ * 0x08, rbp);
    }
    if (reg_isUsed(r13)) {
        mov2bp(pjit, r13, BASE_SP + i++ * 0x08, rbp);
    }
    mov2bp(pjit, r14, BASE_SP + i++ * 0x08, rbp);
    mov2bp(pjit, r15, BASE_SP + i++ * 0x08, rbp);
    //subi(pjit, 0x40, rsp);
    subi(pjit, (i+1) * 0x10, rsp);
    mov(pjit, rsi, RSFP);
    mov(pjit, rdi, RCTX);
    _dump(op);
}
static void pasm_exit(struct pjit *pjit, pcode_t *op)
{
    pcode_setline(op, pjit);
#if 0
    int i = 0;
    if (reg_isUsed(r12)) {
        movbp1(pjit, BASE_SP + i++ * 0x08, rbp, r12);
    }
    if (reg_isUsed(r13)) {
        movbp1(pjit, BASE_SP + i++ * 0x08, rbp, r13);
    }
    movbp1(pjit, BASE_SP + i++ * 0x08, rbp, r14);
    movbp1(pjit, BASE_SP + i++ * 0x08, rbp, r15);
    movbp1(pjit, RSPARG2, rbp, rdx);
    leave(pjit);
    ret(pjit);
#else
    if (EMITMODE(pjit)) {
        int size = pjit->opsize;
#if 0
        if (VALUE_get(pjit, 2)) {
            IRBuilder<> *builder = BUILDER();
            Module *m = MODULE();
            std::vector<const Type*>args_list;
            args_list.push_back(LLVMTYPE_sfp);
            args_list.push_back(LLVMTYPE_Int);
            args_list.push_back(LLVMTYPE_Int);
            FunctionType* fnTy = FunctionType::get(LLVMTYPE_Void, args_list, false);
            Function *func = cast<Function>(m->getOrInsertFunction("check", fnTy));

            std::vector<Value*> params;
            params.push_back(getrbp(pjit));
            params.push_back(ToInt(VALUE_get(pjit, 1)));
            params.push_back(ToInt(VALUE_get(pjit, 2)));
            builder->CreateCall(func, params.begin(), params.end());
        }
#endif
        Value *opline = getop(pjit);
        Value *v = BUILDER()->CreateConstGEP1_32(opline, size+1, "gep");
        BUILDER()->CreateRet(v);
    }
#endif
    _dump(op);
}
static void pasm_nop(struct pjit *pjit, pcode_t *op)
{
    pcode_setline(op, pjit);
    nop_(pjit);
    _dump(op);
}
static void pasm_halt(struct pjit *pjit, pcode_t *op)
{
    pcode_setline(op, pjit);
    nop_(pjit);
    int_(pjit);
    _dump(op);
}

static Value *create_loadsfp(IRBuilder<> *builder, Value *v, int idx0, int idx1, bool isNotFloat=true)
{
	v = builder->CreateConstGEP2_32(v, (unsigned)idx0, (unsigned)idx1, "gep");
    if (idx1 && isNotFloat) {
        v = builder->CreateBitCast(v, PointerType::get(LLVMTYPE_Int, 0), "cast");
    }
	return v;
}

static void pasm_load_r(struct pjit *pjit, pcode_t *op)
{
    reg_t r = op->a.reg;
    knh_sfpidx_t b = op->b.index;
    pcode_setline(op, pjit);
    int shift = b * SHIFT;
#if 0
    mov1(pjit, shift, RSFP, r);
#else
    if (EMITMODE(pjit)) {
        int sfpidx = b/2;
        int validx = (op->b.type == PTYPE_NDAT);
        IRBuilder<> *builder = BUILDER();
        Value *v = getrbp(pjit);
        v = create_loadsfp(builder, v, sfpidx, validx);
        v = builder->CreateLoad(v);
        VALUE_set(pjit, r, v);
    }
#endif
    _dump(op);
}

static void pasm_load_n(struct pjit *pjit, pcode_t *op)
{
    reg_t r = op->a.reg;
    knh_int_t data = op->b.data;
    pcode_setline(op, pjit);
#if 0
    if (data == 0) {
        xor_(pjit, r, r);
    }
    else {
        movi(pjit, r, data);
    }
#else
    if (EMITMODE(pjit)) {
        int sfpidx = r/2;
        int validx = (op->a.type == PTYPE_NDAT);
        Value *v;
        if (op->b.type == FLAT) {
            v = ConstantFP::get(LLVMTYPE_Float, op->b.fval);
        } else if (op->b.type == OBJP) {
            v = ConstantInt::get(LLVMTYPE_Int, data);
            v = BUILDER()->CreateIntToPtr(v, LLVMTYPE_Object);
        } else {
            v = ConstantInt::get(LLVMTYPE_Int, data);
        }
        VALUE_set(pjit, r, v);

    }
#endif
    _dump(op);
}

static void pasm_store_r(struct pjit *pjit, pcode_t *op)
{
    knh_sfpidx_t a = op->a.index;
    reg_t r = op->b.reg;
    pcode_setline(op, pjit);
#if 0
//#ifdef OPCODE_FASTCALL0
//    //TODO();
//    if (isRetVal(a)) {
//        // XXX care about "rix"
//        int shift = 0;
//        assert(r != PREG0);
//        movbp1(pjit, RSPARG2, rbp, PREG0);
//        if (op->a.type == NDAT) {
//            inc(pjit, PREG0);
//        }
//        movrr(pjit, r, RSFP, PREG0);
//        mov2(pjit, r, shift, RSFP);
//    }
//    else {
//        int shift = a * SHIFT;
//        mov2(pjit, r, shift, RSFP);
//    }
//#else
    int shift = a * SHIFT;
    mov2(pjit, r, shift, RSFP);
//#endif
#else
    if (EMITMODE(pjit)) {
        int sfpidx = a/2;
        int validx = (op->a.type == PTYPE_NDAT);
        Value *v = VALUE_get(pjit, r);
        IRBuilder<> *builder = BUILDER();
        //fprintf(stderr, "reg=%d, sfpidx=%d, validx=%d\n", r, sfpidx, validx);
        bool isNotFloat = (v->getType() != LLVMTYPE_Float);
        if (validx == 1) {
            if (isNotFloat && v->getType() != LLVMTYPE_Int) {
                v = ToInt(v);
            }
            else if (!isNotFloat && v->getType() != LLVMTYPE_Float) {
                v = ToFloat(v);
            }
        }
        Value *ptr = create_loadsfp(builder, getrbp(pjit), sfpidx, validx, isNotFloat);
        builder->CreateStore(v, ptr, false);
    }
#endif
    _dump(op);
}

static void pasm_store_n(struct pjit *pjit, pcode_t *op)
{
    knh_sfpidx_t a = op->a.index;
    knh_int_t data = op->b.data;

    int shift = a * SHIFT;
    pcode_setline(op, pjit);
    if ((knh_uint_t)data >= 0x80000000) {
        movi(pjit, PREG0, data);
        mov2(pjit, PREG0, shift, RSFP);
    }
    else {
        movi2(pjit, data, shift, RSFP);
    }
    _dump(op);
}
static void pasm_store_m(struct pjit *pjit, pcode_t *op)
{
    pcode_setline(op, pjit);
    TODO();
    _dump(op);
}
static void pasm_getidx(struct pjit *pjit, pcode_t *op)
{
    reg_t r = op->a.reg;
    pcode_setline(op, pjit);
#if 0
    if (op->b.type == REG) {
        reg_t r2 = op->b.reg;
        movrr_get(pjit, r, r2, PREG0);
    }
    else if (op->b.type == DAT) {
        knh_int_t data = op->b.data;
        mov1(pjit, data, r, PREG0);
    } else {
        abort();
    }
#else
    if (EMITMODE(pjit)) {
        if (op->b.type == REG) {
            reg_t r2 = op->b.reg;
            const Type *PtrTy = PointerType::get(LLVMTYPE_Object, 0);
            Value *v, *v2;
            v  = VALUE_get(pjit, r);
            v2 = VALUE_get(pjit, r2);
            v = BUILDER()->CreateBitCast(v, PtrTy);
            v = BUILDER()->CreateGEP(v, v2);
            v = BUILDER()->CreateLoad(v);
        }
        else if (op->b.type == DAT) {
            knh_int_t data = op->b.data;
            const Type *PtrTy = PointerType::get(LLVMTYPE_Object, 0);
            Value *v;
            v = VALUE_get(pjit, r);
            v = BUILDER()->CreateBitCast(v, PtrTy);
            v = BUILDER()->CreateConstGEP1_32(v, data/sizeof(void*));
            v = BUILDER()->CreateLoad(v);
            VALUE_set(pjit, 0, v);
        } else {
            fprintf(stderr, "%p\n", op);
            asm volatile("int3");
        }
    }

#endif
    _dump(op);
}

static void pasm_setidx(struct pjit *pjit, pcode_t *op)
{
    reg_t r = op->a.reg;
#if 0
    // movq    %r1,(%r2,%r3,8
    if (op->b.type == REG) {
        reg_t r2 = op->b.reg;
        movrr(pjit, r, PREG0, r2);
    }
    else if (op->b.type == DAT) {
        knh_int_t data = op->b.data;
        mov2(pjit, r, data, PREG0);
    }
#else
    if (EMITMODE(pjit)) {
        Value *v = VALUE_get(pjit, 0);
        if (op->b.type == REG) {
            reg_t r2 = op->b.reg;
            asm volatile("int3");
        }
        else if (op->b.type == DAT) {
            IRBuilder<> *builder = BUILDER();
            knh_int_t data = op->b.data;
            v = builder->CreateBitCast(v, PointerType::get(LLVMTYPE_Int, 0));
            Value *ptr = builder->CreateConstGEP1_32(v, data/sizeof(void*));
            Value *val = VALUE_get(pjit, r);
            builder->CreateStore(val, ptr);
        }
#if 0
        {
            asm volatile("int3");
            IRBuilder<> *builder = BUILDER();
            Module *m = MODULE();
            std::vector<const Type*>args_list;
            args_list.push_back(LLVMTYPE_sfp);
            args_list.push_back(LLVMTYPE_Int);
            args_list.push_back(LLVMTYPE_Int);
            FunctionType* fnTy = FunctionType::get(LLVMTYPE_Void, args_list, false);
            Function *func = cast<Function>(m->getOrInsertFunction("check", fnTy));

            std::vector<Value*> params;
            params.push_back(getrbp(pjit));
            params.push_back(ToInt(VALUE_get(pjit, 1)));
            params.push_back(ConstantInt::get(LLVMTYPE_Int, 10));
            builder->CreateCall(func, params.begin(), params.end());
        }
#endif
    }
#endif
    _dump(op);
}
static void pasm_cmp_n(struct pjit *pjit, pcode_t *op)
{
    reg_t r = op->a.reg;
    knh_int_t data = op->b.data;
    pcode_setline(op, pjit);
    if (data == 0) {
        testq(pjit, r, r);
    }
    else {
        cmpn(pjit, r, data);
    }
    _dump(op);
}

static void pasm_cmp_m(struct pjit *pjit, pcode_t *op)
{
    pcode_setline(op, pjit);
    TODO();
    _dump(op);
}

static void pasm_cmp_r(struct pjit *pjit, pcode_t *op)
{
    pcode_setline(op, pjit);
    reg_t r1 = op->a.reg;
    reg_t r2 = op->b.reg;
    cmpr(pjit, r1, r2);
    if (op->flag != 0) {
        TODO();
        if (op->isFloat) {
        } else {
        }
    }
    _dump(op);
}
static void pasm_cond(struct pjit *pjit, pcode_t *op)
{
    knh_BasicBlock_t *bb = op->a.bb;
    knh_int_t idx = op->a.index;
    knh_int_t opcode = op->b.data;
    pcode_setline(op, pjit);
    if (op->a.type == BB) {
        cond(pjit, opcode, bb, 0);
    } else if (op->a.type == IDX) {
        cond(pjit, opcode, NULL, op + idx);
    }
    _dump(op);
}
static void pasm_jump(struct pjit *pjit, pcode_t *op)
{
    knh_BasicBlock_t *bb = op->a.bb;
    pcode_setline(op, pjit);
    jump(pjit, bb, 0);
    _dump(op);
}
static void pasm_call(struct pjit *pjit, pcode_t *op)
{
    knh_intptr_t func = op->a.data;
    knh_intptr_t flag = op->b.data;
    pcode_setline(op, pjit);
#if 0
    movi(pjit, PREG0, func);
    if (flag == PJIT_RELOCATION_SYMBOL_FUNC && pjit->state == PJIT_EMIT) {
        pjit_addRelocationCode(pjit, sizeof(void*), FUNC, func);
    }
    else if (flag == PJIT_RELOCATION_SYMBOL_SELF && pjit->state == PJIT_EMIT) {
        pjit_addRelocationCode(pjit, sizeof(void*), SELF, 0);
    }
    call_r(pjit);
    //mov(pjit, RSFP, PARG1);
    //mov(pjit, RCTX, PARG0);
#else
    if (EMITMODE(pjit)) {
        Value *vfunc;
        IRBuilder<> *builder = BUILDER();
        std::vector<Value*> params;
        vfunc = ConstantInt::get(LLVMTYPE_Int, func);
        vfunc = builder->CreateIntToPtr(vfunc, LLVMTYPE_fcall);
        params.push_back(getctx(pjit));
        params.push_back(VALUE_get(pjit, PARG1));
        params.push_back(VALUE_get(pjit, PARG2));
        builder->CreateCall(vfunc, params.begin(), params.end());
    }

#endif
    _dump(op);
}
static void pasm_incref(struct pjit *pjit, pcode_t *op)
{
    pcode_setline(op, pjit);
#if 0
    reg_t r = op->a.reg;
    if (op->b.type == OBJ) {
        asm volatile("int3");
    } else {
    }
    inc2(pjit, r, 0x10);
#else
#endif
    _dump(op);
}
static void pasm_decref(struct pjit *pjit, pcode_t *op)
{
    pcode_setline(op, pjit);
#if 0
    reg_t r = op->a.reg;
    if (op->b.type == OBJ) {
        knh_int_t b = op->b.data;
        int shift = b * SHIFT;
        mov1(pjit, shift, RSFP, r);
    } else {
        asm volatile("int3");
    }
    dec2(pjit, r, 0x10);
#else
#endif
    _dump(op);
}
static void pasm_inc(struct pjit *pjit, pcode_t *op)
{
    reg_t r = op->a.reg;
    pcode_setline(op, pjit);
#if 0
    inc(pjit, r);
#else
    if (EMITMODE(pjit)) {
        Value *v = VALUE_get(pjit, r);
        Value *one = (ConstantInt::get(LLVMTYPE_Int, 1));
        v = BUILDER()->CreateAdd(v, one, "add");
        VALUE_set(pjit, r, v);
    }
#endif
    _dump(op);
}
static void pasm_dec(struct pjit *pjit, pcode_t *op)
{
    reg_t r = op->a.reg;
    pcode_setline(op, pjit);
    dec(pjit, r);
    _dump(op);
}
static void pasm_mov_rr(struct pjit *pjit, pcode_t *op)
{
    reg_t r1 = op->a.reg;
    reg_t r2 = op->b.reg;
    pcode_setline(op, pjit);
#if 0
    mov(pjit, r2, r1);
#else
    if (EMITMODE(pjit)) {
        Value *v = VALUE_get(pjit, r2);
        if (!v) {
            if (PARG0 == r2)
                v = getctx(pjit);
            if (PARG1 == r2)
                v = getrbp(pjit);
            if (RCTX == r2)
                v = getctx(pjit);
            if (RSFP == r2)
                v = getrbp(pjit);
        }
        VALUE_set(pjit, r1, v);
    }
#endif
    _dump(op);
}
static void pasm_add_rm(struct pjit *pjit, pcode_t *op)
{
    pcode_setline(op, pjit);
    TODO();
    _dump(op);
}
static void pasm_add_mm(struct pjit *pjit, pcode_t *op)
{
    pcode_setline(op, pjit);
    TODO();
    _dump(op);
}
static void pasm_add_rn(struct pjit *pjit, pcode_t *op)
{
    reg_t r1 = op->a.reg;
    knh_int_t data = op->b.data;
    pcode_setline(op, pjit);
#if 0
    addi(pjit, data, r1);
#else
    if (EMITMODE(pjit)) {
        Value *v;
        Value *v1 = VALUE_get(pjit, r1);
        if (PointerType::classof(v1->getType())) {
            data = data / 0x10;
            v = BUILDER()->CreateConstGEP1_32(v1, data);
        } else {
            Value *v2 = ConstantInt::get(LLVMTYPE_Int, data);
            v = BUILDER()->CreateAdd(v1, v2);
        }
        VALUE_set(pjit, r1, v);
    }
#endif
    _dump(op);
}

static void pasm_add_rr(struct pjit *pjit, pcode_t *op)
{
    reg_t r1 = op->a.reg;
    reg_t r2 = op->b.reg;
    pcode_setline(op, pjit);
#if 0
    addr(pjit, r2, r1);
#else
    if (EMITMODE(pjit)) {
        Value *v1 = VALUE_get(pjit, r1);
        Value *v2 = VALUE_get(pjit, r2);
        v1 = ToInt(v1);
        v2 = ToInt(v2);
        Value *v  = BUILDER()->CreateAdd(v1, v2);
        VALUE_set(pjit, r1, v);
    }
#endif
    _dump(op);
}
static void pasm_sub_rn(struct pjit *pjit, pcode_t *op)
{
    reg_t r1 = op->a.reg;
    knh_int_t data = op->b.data;
    pcode_setline(op, pjit);
#if 0
    subi(pjit, data, r1);
#else
    if (EMITMODE(pjit)) {
        Value *v1 = VALUE_get(pjit, r1);
        Value *v2 = (ConstantInt::get(LLVMTYPE_Int, data));
        Value *v  = BUILDER()->CreateSub(v1, v2);
        VALUE_set(pjit, r1, v);
    }
#endif
    _dump(op);
}
static void pasm_sub_rr(struct pjit *pjit, pcode_t *op)
{
    reg_t r1 = op->a.reg;
    reg_t r2 = op->b.reg;
    pcode_setline(op, pjit);
#if 0
    TODO();
#else
    if (EMITMODE(pjit)) {
        Value *v1 = VALUE_get(pjit, r1);
        Value *v2 = VALUE_get(pjit, r2);
        v1 = ToInt(v1);
        v2 = ToInt(v2);
        Value *v  = BUILDER()->CreateSub(v1, v2);
        VALUE_set(pjit, r1, v);
    }
#endif

    _dump(op);
}
static void pasm_mul_rn(struct pjit *pjit, pcode_t *op)
{
    reg_t r1 = op->a.reg;
    knh_int_t data = op->b.data;
    pcode_setline(op, pjit);
    //subi(pjit, data, r1);
#if 0
    TODO();
#else
    if (EMITMODE(pjit)) {
        Value *v1 = VALUE_get(pjit, r1);
        Value *v2 = (ConstantInt::get(LLVMTYPE_Int, data));
        Value *v  = BUILDER()->CreateMul(v1, v2);
        VALUE_set(pjit, r1, v);
    }
#endif
    _dump(op);
}
static void pasm_mul_rr(struct pjit *pjit, pcode_t *op)
{
    reg_t r1 = op->a.reg;
    reg_t r2 = op->b.reg;
    pcode_setline(op, pjit);
#if 0
    mulr(pjit, r1, r2);
#else
    if (EMITMODE(pjit)) {
        Value *v1 = VALUE_get(pjit, r1);
        Value *v2 = VALUE_get(pjit, r2);
        Value *v  = BUILDER()->CreateMul(v1, v2);
        VALUE_set(pjit, r1, v);
    }
#endif
    _dump(op);
}
static void pasm_store_f(struct pjit *pjit, pcode_t *op)
{
    reg_t r1 = op->a.reg;
    knh_int_t data = op->b.data;
    pcode_setline(op, pjit);
#if 0
    fmov2(pjit, PFREG0, data, r1);
#else
    if (EMITMODE(pjit)) {
        int sfpidx = (data/sizeof(void*))/sizeof(knh_sfp_t*);
        int validx = 1;
        Value *v = VALUE_get(pjit, 0);
        IRBuilder<> *builder = BUILDER();
        bool isNotFloat = (v->getType() != (LLVMTYPE_Float));
        if (isNotFloat) {
            v = ToFloat(v);
        }
        Value *ptr = create_loadsfp(builder, getrbp(pjit), sfpidx, validx, isNotFloat);
        builder->CreateStore(v, ptr, false);

    }

#endif
    _dump(op);
}
static void pasm_load_fn(struct pjit *pjit, pcode_t *op)
{
    reg_t r1 = op->a.reg;
    knh_int_t data = op->b.data;
    pcode_setline(op, pjit);
#if 0
    movi(pjit, PREG0, data);
    if (r1 == rsp) {
        mov2sp(pjit, PREG0, 0x00, r1);
    } else {
        TODO();
    }
    fmov1(pjit, 0x00, r1, PFREG0);
#else
    if (EMITMODE(pjit)) {
        //int sfpidx = r1/2;
        //int validx = (op->a.type == PTYPE_NDAT);
        Value *v;
        if (op->b.type == FLAT) {
            v = ConstantFP::get(LLVMTYPE_Float, op->b.fval);
        } else if (op->b.type == OBJP) {
            v = ConstantInt::get(LLVMTYPE_Int, data);
            v = BUILDER()->CreateIntToPtr(v, LLVMTYPE_Object);
        } else {
            v = ConstantInt::get(LLVMTYPE_Int, data);
        }
        VALUE_set(pjit, r1, v);
        asm volatile("int3");
    }
#endif
    _dump(op);
}
static void pasm_load_fr(struct pjit *pjit, pcode_t *op)
{
    reg_t r1 = op->a.reg;
    knh_int_t idx = op->b.data;
    pcode_setline(op, pjit);
#if 0
    fmov1(pjit, idx, r1, PFREG0);
#else
    if (EMITMODE(pjit)) {
        int sfpidx = idx/0x10;
        int validx = 1;
        IRBuilder<> *builder = BUILDER();
        Value *v = getrbp(pjit);
        v = create_loadsfp(builder, v, sfpidx, validx, false);
        v = builder->CreateLoad(v);
        VALUE_set(pjit, 0/*tmp*/, v);
    }
#endif
    _dump(op);
}
#define asm_f(pjit, op, func) {\
    reg_t r1 = op->a.reg;\
    knh_int_t data = op->b.data;\
    func(pjit, data, r1, PFREG0);\
}
static void pasm_add_f(struct pjit *pjit, pcode_t *op)
{
    pcode_setline(op, pjit);
    asm_f(pjit, op, fadd1);
    _dump(op);
}
static void pasm_sub_f(struct pjit *pjit, pcode_t *op)
{
    pcode_setline(op, pjit);
    asm_f(pjit, op, fsub1);
    _dump(op);
}
static void pasm_mul_f(struct pjit *pjit, pcode_t *op)
{
    pcode_setline(op, pjit);
    asm_f(pjit, op, fmul1);
    _dump(op);
}
static void pasm_div_f(struct pjit *pjit, pcode_t *op)
{
    pcode_setline(op, pjit);
    asm_f(pjit, op, fdiv1);
    _dump(op);
}
#define asm_fn(pjit, op, func) {\
    reg_t r1 = op->a.reg;\
    knh_int_t data = op->b.data;\
    movi(pjit, PREG0, data);\
    if (r1 == rsp) {\
        mov2sp(pjit, PREG0, 0x00, r1);\
    } else {\
        TODO();\
    }\
    func(pjit, 0x00, r1, PFREG0);\
}
static void pasm_add_fn(struct pjit *pjit, pcode_t *op)
{
    pcode_setline(op, pjit);
#if 0
    asm_fn(pjit, op, fadd1);
#else
    if (EMITMODE(pjit)) {
        reg_t r1 = op->a.reg;
        Value *f = ConstantFP::get(LLVMTYPE_Float, op->b.fval);
        IRBuilder<> *builder = BUILDER();
        Value *v = VALUE_get(pjit, 0/*tmp*/);
        v = builder->CreateFAdd(v, f);
        VALUE_set(pjit, r1, v);
    }
#endif
    _dump(op);
}
static void pasm_sub_fn(struct pjit *pjit, pcode_t *op)
{
    pcode_setline(op, pjit);
#if 0
    asm_fn(pjit, op, fsub1);
#else
    if (EMITMODE(pjit)) {
        reg_t r1 = op->a.reg;
        Value *f = ConstantFP::get(LLVMTYPE_Float, op->b.fval);
        IRBuilder<> *builder = BUILDER();
        Value *v = VALUE_get(pjit, 0/*tmp*/);
        v = builder->CreateFSub(v, f);
        VALUE_set(pjit, r1, v);
    }

#endif
    _dump(op);
}
static void pasm_mul_fn(struct pjit *pjit, pcode_t *op)
{
    pcode_setline(op, pjit);
#if 0
    asm_fn(pjit, op, fmul1);
#else
    if (EMITMODE(pjit)) {
        reg_t r1 = op->a.reg;
        Value *f = ConstantFP::get(LLVMTYPE_Float, op->b.fval);
        IRBuilder<> *builder = BUILDER();
        Value *v = VALUE_get(pjit, 0/*tmp*/);
        v = builder->CreateFMul(v, f);
        VALUE_set(pjit, r1, v);
    }

#endif
    _dump(op);
}
static void pasm_div_fn(struct pjit *pjit, pcode_t *op)
{
    pcode_setline(op, pjit);
#if 0
    asm_fn(pjit, op, fdiv1);
#else
    if (EMITMODE(pjit)) {
        reg_t r1 = op->a.reg;
        Value *f = ConstantFP::get(LLVMTYPE_Float, op->b.fval);
        IRBuilder<> *builder = BUILDER();
        Value *v = VALUE_get(pjit, 0/*tmp*/);
        v = builder->CreateFDiv(v, f);
        VALUE_set(pjit, r1, v);
    }

#endif
    _dump(op);
}
static void pasm_fcast_r(struct pjit *pjit, pcode_t *op)
{
    reg_t r1 = op->a.reg;
    pcode_setline(op, pjit);
#if 0
    fcast(pjit, r1, PFREG0);
#else
    if (EMITMODE(pjit)) {
        Value *v = VALUE_get(pjit, r1);
        v = BUILDER()->CreateSIToFP(v, LLVMTYPE_Float, "fcast");
        VALUE_set(pjit, 0, v);
    }
#endif
    _dump(op);
}
static void pasm_icast_r(struct pjit *pjit, pcode_t *op)
{
    reg_t r1 = op->a.reg;
    pcode_setline(op, pjit);
    icast(pjit, PFREG0, r1);
    _dump(op);
}
typedef void (*pemit_t)(struct pjit *, pcode_t *);
#include "opcode.h"
//static const pemit_t PCODE_EMIT[] = {
//#include "opcode_emit.c"
//};
#define EMIT(pjit, op) PCODEDATA[opcode(op->opcode)].emit(pjit, op)

#define JE  0x74
#define JNE 0x75
#define JBE 0x76
#define JA  0x77
#define JS  0x78
#define JNS 0x79
#define JP  0x7a
#define JNP 0x7b
#define JLE 0x7c
#define JGE 0x7d
#define JL  0x7e
#define JG  0x7f
#define nop()   {NOP,  {(0)}}
#define reg(r)  {REG,  {(void*)(ZERO + (knh_intptr_t)r)}}
#define obj(m)  {OBJ,  {(void*)(ZERO + (knh_intptr_t)m)}}
#define ndat(m) {NDAT, {(void*)(ZERO + (knh_intptr_t)m)}}
#define data(n) {DAT,  {(void*)(ZERO + (knh_intptr_t)n)}}
#define objp(p) {OBJP, {(void*)p}}
#define bb(b,i) {BB,   {(b)}}
#define cond(c) {COND, {(void*)(ZERO + c)}}
#define func(f) {FUNC, {(void*)(f)}}
#define idx(i)  {IDX,  {(void*)(ZERO + i)}}
#define flat(f) {FLAT, {(void*)(ZERO + (knh_ndata_t)f)}}

static void pcode_dump(pcode_t *op)
{
#ifdef PCODE_DUMP
    int i;
    pindex_t p[2];
    struct pcode_data data = PCODEDATA[opcode(op->opcode)];
    fprintf(stderr, "%s ", data.name);
    p[0] = op->a;
    p[1] = op->b;
    for (i = 0; i < 2; i++) {
        switch (p[i].type) {
            case PTYPE_REG:
                fprintf(stderr, "r%d ", p[i].reg);
                break;
            case PTYPE_OBJ:
                fprintf(stderr, "sfp[%ld].o ", p[i].index);
                break;
            case PTYPE_NDAT:
                fprintf(stderr, "sfp[%ld].i ", p[i].index);
                break;
            case PTYPE_DATA:
                fprintf(stderr, "0x%llx ", (long long unsigned int) p[i].data);
                break;
            case PTYPE_OBJP:
                fprintf(stderr, "obj(%p) ", p[i].o);
                break;
            case PTYPE_BB:
                fprintf(stderr, "bb[%d] ", DP(p[i].bb)->id);
                break;
            case PTYPE_FLAT:
                {
                    ndata_t v;
                    v.i = p[i].data;
                    fprintf(stderr, "%f ", v.f);
                    break;
                }
            case PTYPE_IDX:
                fprintf(stderr, "op[+%ld] ", p[i].index);
                break;
            case PTYPE_COND:
            case PTYPE_FUNC:
                fprintf(stderr, "%p ", (void *)p[i].data);
                break;
            case PTYPE_SFP:
                if (p[i].type == OBJ) {
                    fprintf(stderr, "sfp[%ld].o ", p[i].index);
                } else if (p[i].type == NDAT) {
                    fprintf(stderr, "sfp[%ld].i ", p[i].index);
                } else {
                    fprintf(stderr, "sfp[%ld] ", p[i].index);
                }
                break;
        }
    }
    fprintf(stderr, "\n");
#endif
}
static void pcode_expand(CTX ctx, knh_BasicBlock_t *bb, size_t newsize)
{
    pcode_t *newbuf = (pcode_t*)KNH_MALLOC(ctx, sizeof(pcode_t) * newsize);
    pdata_t *pdata = (pdata_t*) DP(bb)->code;
    knh_memcpy(newbuf, pdata->opbuf, pdata->capacity * sizeof(pcode_t));
    KNH_FREE(ctx, pdata->opbuf, pdata->capacity * sizeof(pcode_t));
    pdata->opbuf    = newbuf;
    pdata->capacity = newsize;
}

static void pcode_add_(CTX ctx, knh_BasicBlock_t *bb, pcode_t *op)
{
    pcode_t *pc;
    pdata_t *pdata = (pdata_t*) DP(bb)->code;

    if(pdata->capacity == 0) {
        pdata->opbuf = (pcode_t*)KNH_MALLOC(ctx, sizeof(pcode_t));
        pdata->capacity = 1;
    }
    else if(pdata->capacity == 1) {
        pcode_expand(ctx, bb, 4);
    }
    else if(pdata->size == pdata->capacity) {
        pcode_expand(ctx, bb, pdata->capacity * 2);
    }
    pc = pdata->opbuf + pdata->size;
    knh_memcpy(pc, op, sizeof(pcode_t));
    pdata->size += 1;
}

static void pdata_delete(CTX ctx, pdata_t *pdata)
{
    KNH_FREE(ctx, pdata->opbuf, sizeof(pcode_t) * pdata->capacity);
    pdata->opbuf = (pcode_t*)0xdeadbeaf;
    KNH_FREE(ctx, pdata, sizeof(pdata_t));
}

static pdata_t *pdata_new(CTX ctx)
{
    pdata_t *pdata = (pdata_t*)KNH_MALLOC(ctx, sizeof(pdata_t));
    pdata->opbuf = NULL;
    pdata->size = 0;
    pdata->capacity = 0;
    return pdata;
}

static knh_BasicBlock_t* new_BasicBlock(CTX ctx)
{
    knh_BasicBlock_t *bb = new_(BasicBlock);
    DP(bb)->id = 0;
    DP(bb)->code = (knh_opline_t *) pdata_new(ctx);
    return bb;
}

static void BasicBlock_setOriginal(CTX ctx, knh_BasicBlock_t *bb, knh_opline_t *op)
{
    BasicBlock_add_(ctx, bb, op);
    knh_opline_t *op_ = DP(bb)->opbuf + DP(bb)->size - 1;
    op_->codeaddr = op;
}

static void init_regtable(void);
#define regtable_size(_table) ((size_t)_table[-1].data)
static pindex_t *createRegisterTable(CTX ctx, int size)
{
    int size_ = size + 1;
    init_regtable();
    pindex_t *table = (pindex_t*)KNH_MALLOC(ctx, sizeof(pindex_t) * size_);
    int i;
    table[0].data = size;
    for (i = 1; i < size_; i++) {
        table[i].type = NOP;
        table[i].reg = PREG_NOP;
    }
    return (table + 1);
}

static void deleteRegisterTable(CTX ctx, pindex_t *regTable)
{
    int size = regtable_size(regTable) + 1;
    pindex_t *table = (regTable - 1);
    KNH_FREE(ctx, table, size * sizeof(pindex_t));
}

//static void register_allocation(kvc_t *kvc)
//static void expire_old_intervals(CTX ctx, knh_Array_t *interval, knh_Array_t *active, int i)
//static void *spill_at_interval(kvc_t *kvc, knh_Array_t *interval, knh_Array_t *active, int i)

static void pcode_add_(CTX ctx, knh_BasicBlock_t *bb, pcode_t *op);
#define PASM_(opcode, a, b) { \
    pcode_t __op = {opcode, 0, a, b};\
    pcode_add_(ctx, bb, (&__op));\
}

#define PASM(T, a, b) { \
    pcode_t __op = {PCODE_##T, 0, a, b};\
    pcode_add_(ctx, bb, (&__op));\
}
#define PASM_CMP(T, a, b, flag, isFloat) {\
    pcode_t __op = {PCODE_##T, 0, a, b, flag, isFloat};\
    pcode_add_(ctx, bb, (&__op));\
}

#define OPCASE(op) case OPCODE_##op : 
//static void table_tochar(pindex_t *t)
//{
//    int type = t->type;
//    switch (type) {
//    case REG:
//        fprintf(stderr, "r%d(%d)", (t->reg), type);
//        break;
//    case OBJ:
//    case NDAT:
//        fprintf(stderr, "%4ld(%d)",(t->index), type);
//        break;
//    case NOP:
//        fprintf(stderr, "nop(%d)", type);
//    }
//}
struct regtable {
    reg_t r;
    int type;
    knh_sfpidx_t use;
    int generation;
};

#define REG_UNUSE 0xff
static struct regtable reg_table[] = {
    {-1, NOP, REG_UNUSE, 0},
    {-1, NOP, REG_UNUSE, 0},
    {-1, NOP, REG_UNUSE, 0},
    {-1, NOP, REG_UNUSE, 0},
    {-1, NOP, REG_UNUSE, 0},
    {-1, NOP, REG_UNUSE, 0},
    {-1, NOP, REG_UNUSE, 0},
    {-1, NOP, REG_UNUSE, 0},
    /* */
    {-1, NOP, REG_UNUSE, 0},
    {-1, NOP, REG_UNUSE, 0},
    {-1, NOP, REG_UNUSE, 0},
    {-1, NOP, REG_UNUSE, 0},
};

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
static void regtable_dump(void)
{
#if 0
    size_t i;
    for (i = 0; i < ARRAY_SIZE(reg_table); ++i) {
        struct regtable *t = &reg_table[i];
        int type = t->type;
        switch (type) {
            case REG:
                fprintf(stderr, "r%d(%d), ", (t->r), type);
                break;
            case OBJ:
            case NDAT:
                fprintf(stderr, "%4ld(%d), ",(t->use), type);
                break;
            case NOP:
                fprintf(stderr, "nop(%d), ", type);
        }
    }
    fprintf(stderr, "\n");
#endif
}
static void init_regtable(void)
{
    size_t i;
    for (i = 0; i < ARRAY_SIZE(reg_table); ++i) {
        reg_table[i].r = PREG0+i;
    }
}
static int reg_isUsed(reg_t r)
{
    size_t i;
    for (i = 1; i < ARRAY_SIZE(reg_table); i++) {
        struct regtable *t = &reg_table[i];
        if (t->r == r) {
            if (t->generation > 0) return 1;
            return 0;
        }
    }
    return 0;
}

static reg_t regalloc(pindex_t *regTable, int type, knh_sfpidx_t a)
{
    size_t i;
    for (i = 1; i < ARRAY_SIZE(reg_table); i++) {
        if (reg_table[i].use == a) return reg_table[i].r;
    }

    for (i = 1; i < ARRAY_SIZE(reg_table); i++) {
        if (reg_table[i].use == REG_UNUSE)
            break;
    }
    assert(i < ARRAY_SIZE(reg_table));
    reg_table[i].use  = a;
    reg_table[i].type = type;
    reg_table[i].generation++;
    regtable_dump();
    //size_t size = regtable_size(regTable);
    //fprintf(stderr, "a=%ld\n", a);
    //if (0 <= a && a <= regtable_size(regTable)) {
    //    regTable[a].type  = type;
    //    regTable[a].index = a;
    //}
    return reg_table[i].r;
}
#define CLEAR_REG(r) {\
    r.use  = REG_UNUSE;\
    r.type = NOP;\
}
static void clear_reg(reg_t r)
{
    size_t i;
    for (i = 1; i < ARRAY_SIZE(reg_table); i++) {
        if (reg_table[i].r == r) {
            CLEAR_REG(reg_table[i]);
        }
    }
}

static knh_sfpidx_t search_source(knh_BasicBlock_t *bb, reg_t r)
{
    if (r == PREG0) {
        return -1;
    } else {
        int i;
        pdata_t *pdata = (pdata_t*) DP(bb)->code;
        pcode_t *op;
        for (i = pdata->size - 1; i >= 0; i--) {
            op = pdata->opbuf + i;
            if (op->opcode == PCODE_GETIDX) {
                continue;
            }
            if (op->a.type == REG && op->a.reg == r) {
                return op->b.index;
            }
        }
    }
    return -1;
}

static void store_reg_ndat(CTX ctx, knh_BasicBlock_t *bb, pindex_t *regTable, int i)
{
    reg_t r  = reg_table[i].r;
    PASM(STORER, ndat(reg_table[i].use), reg(r));
    //int size = regtable_size(regTable);
}
static void store_reg_obj(CTX ctx, knh_BasicBlock_t *bb, pindex_t *regTable, int i)
{
    reg_t r  = reg_table[i].r;
    knh_sfpidx_t use = reg_table[i].use;
    knh_sfpidx_t source = search_source(bb, r);
    if (source == use)
        return;
#ifdef K_USING_RCGC
    PASM(INCREF, reg(r), nop());
    PASM(DECREF, reg(PREG0), obj(use));
    PASM(MOVRR , reg(PARG1), reg(PREG0));
#endif
    PASM(STORER, obj(use), reg(r));
#ifdef K_USING_RCGC
    //fprintf(stderr, "use=%d source=%d r=%d\n", use, source, r);
    /* obj(reg_table[i].use) is loaded to PREG0 
     * by DECREF op. */
    PASM(GETIDX, reg(PARG1), data(0x10));
    PASM_CMP(CMPN, reg(PREG0), data(0), 0/*default*/);
    PASM(COND, idx(2), cond(JNE));
    /* TODO write fast path Object_free */ 
    PASM(CALL , func(knh_Object_free), nop());
    PASM(NOP , nop(), nop());
#endif
}
static void store_reg_objp(CTX ctx, knh_BasicBlock_t *bb, pindex_t *regTable, int i)
{
    asm volatile("int3");
}
static void store_reg_(CTX ctx, knh_BasicBlock_t *bb, pindex_t *regTable, int i)
{
    int type = reg_table[i].type;
    if (type == NDAT) {
        store_reg_ndat(ctx, bb, regTable, i);
    } else if (type == OBJ) {
        store_reg_obj(ctx, bb, regTable, i);
    } else if (type == OBJP) {
        store_reg_objp(ctx, bb, regTable, i);
    }

}

#define BLOCKEND 0
#define MTDEND   1
#define FUNCCALL 2
static void store_regs(CTX ctx, knh_BasicBlock_t *bb, pindex_t *regTable, knh_sfpidx_t this_, int end)
{
    int i;
    for (i = ARRAY_SIZE(reg_table) -1; i >= 0;i--) {
        int type = reg_table[i].type;
        knh_sfpidx_t use = reg_table[i].use;
        if (use != REG_UNUSE && type == NDAT) {
            if (end == BLOCKEND || end == FUNCCALL) {
                store_reg_ndat(ctx, bb, regTable, i);
            }
            else if (end == MTDEND && this_ > use) {
                store_reg_ndat(ctx, bb, regTable, i);
            }
            CLEAR_REG(reg_table[i]);
        }
    }
    for (i = ARRAY_SIZE(reg_table) -1; i >= 0;i--) {
        int type = reg_table[i].type;
        knh_sfpidx_t use = reg_table[i].use;
        if (use != REG_UNUSE && type == OBJ) {
            store_reg_obj(ctx, bb, regTable, i);
            CLEAR_REG(reg_table[i]);
        }
    }
    for (i = ARRAY_SIZE(reg_table) -1; i >= 0;i--) {
        int type = reg_table[i].type;
        knh_sfpidx_t use = reg_table[i].use;
        if (use != REG_UNUSE && type == OBJP) {
            store_reg_objp(ctx, bb, regTable, i);
            CLEAR_REG(reg_table[i]);
        }
    }
}

static void reg_store_(CTX ctx, knh_BasicBlock_t *bb, pindex_t *regTable, knh_sfpidx_t a)
{
    size_t i;
    for (i = 0; i < ARRAY_SIZE(reg_table); i++) {
        if (reg_table[i].use == a)
            store_reg_(ctx, bb, regTable, i);
    }

}
static int reg_isLoaded(pindex_t *regTable, knh_sfpidx_t a)
{
    size_t i;
    for (i = 0; i < ARRAY_SIZE(reg_table); i++) {
        if (reg_table[i].use == a)
            return 1;
    }
    return 0;
}

static void asm_nmov(CTX ctx, knh_BasicBlock_t *bb, pindex_t *regTable, knh_sfpidx_t a, knh_sfpidx_t b)
{
    reg_t r, r2;
    if (reg_isLoaded(regTable, b)) {
        r2 = regalloc(regTable, NDAT, b);
        r  = regalloc(regTable, NDAT, a);
        if (r2 == PREG_NOP) {
            TODO();
        }
        PASM(MOVRR, reg(r), reg(r2));
    } else {
        r  = regalloc(regTable, NDAT, a);
        PASM(LOADR, reg(r), ndat(b));
    }
}
static void asm_omov(CTX ctx, knh_BasicBlock_t *bb, pindex_t *regTable, knh_sfpidx_t a, knh_sfpidx_t b)
{
    reg_t r, r2;
    if (reg_isLoaded(regTable, b)) {
        r2 = regalloc(regTable, OBJ, b);
        r  = regalloc(regTable, OBJ, a);
        if (r2 == PREG_NOP) {
            TODO();
        }
        PASM(MOVRR, reg(r), reg(r2));
    } else {
        r  = regalloc(regTable, OBJ, a);
        PASM(LOADR , reg(r), obj(b));
    }
}
static void asm_nset(CTX ctx, knh_BasicBlock_t *bb, pindex_t *regTable, knh_sfpidx_t a, knh_int_t n)
{
    reg_t r = regalloc(regTable, NDAT, a);
    if (r == PREG_NOP) {
        TODO();
        PASM(LOADN, reg(r), data(n));
        asm volatile("int3");
    } else {
        PASM(LOADN, reg(r), data(n));
    }
}

static void asm_oset(CTX ctx, knh_BasicBlock_t *bb, pindex_t *regTable, knh_sfpidx_t a, knh_Object_t *o)
{
    reg_t r = regalloc(regTable, OBJ, a);
    if (r == PREG_NOP) {
        TODO();
    }
    PASM(LOADN, reg(r), objp(o));
    //PASM(INCREF, reg(r), nop());
}

//static void setesp(CTX ctx, knh_rbp_t *rbp, size_t size)
//{
//    //klr_setesp(ctx, SFP(rshift(rbp, size)));
//    //((knh_context_t*)ctx)->esp += size;
//}


static void PASM_SETESP(CTX ctx, knh_BasicBlock_t *bb, size_t espshift)
{
#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif
    //PASM(GETIDX, reg(PREG0), data(offsetof(knh_context_t, esp)));
    //PASM(ADDRN,  reg(PREG0), data(op_->espshift));
    //PASM(SETIDX, reg(PREG0), data(offsetof(knh_context_t, esp)));
    PASM(MOVRR, reg(PARG1), reg(RSFP));
    PASM(LOADN,  reg(PARG2), data(espshift));
    //PASM(CALL , func(setesp), nop());
}
static void __throw_outofrange(CTX ctx, knh_rbp_t *rbp, knh_Array_t *a, size_t n)
{
    size_t size = a->size;
    if (unlikely(n >= size)) {
        THROW_OutOfRange(ctx, SFP(rbp), n, size);
    }
}
static void PASM_CHKIDX(CTX ctx, knh_BasicBlock_t *bb, reg_t raobj, reg_t rn)
{
    //PASM(MOVRR, reg(PARG1), reg(RSFP));
    //PASM(MOVRR, reg(PARG2), reg(raobj));
    //PASM(MOVRR, reg(PARG3), reg(rn));
    //PASM(CALL , func(__throw_outofrange), nop());
}
#if 0
static void __throw_outofrange2(CTX ctx, knh_rbp_t *rbp, knh_Array_t *a, size_t x, size_t y)
{
    asm volatile("int3");
}


static void PASM_CHKIDX2(CTX ctx, knh_BasicBlock_t *bb, reg_t raobj, reg_t rx, reg_t ry)
{
    PASM(MOVRR, reg(PARG1), reg(RSFP));
    PASM(MOVRR, reg(PARG2), reg(raobj));
    PASM(MOVRR, reg(PARG3), reg(rx));
    PASM(MOVRR, reg(PARG4), reg(ry));
    PASM(CALL , func(__throw_outofrange2), nop());
}
#endif

#define FIELD_OFFSET (offsetof(knh_ObjectField_t, fields))
#define LIST_OFFSET (offsetof(knh_Array_t, list))
static void BasicBlock_setPcode(CTX ctx, knh_BasicBlock_t *bb, pindex_t *regTable, int *oplen)
{
    size_t j;
    /* EQ, NEQ, LT< LTE, GT, GTE */
    static int condop_c[] = {JNE, JE, JGE, JG, JLE, JL};
    static int condop[]   = {JE, JNE, JL, JLE, JGE, JNE};
    PASM(INIT, nop(), nop());
    knh_opline_t *op;
    for (j = 0; j < DP(bb)->size; j++) {
        op = DP(bb)->opbuf + j;
#ifdef PCODE_DUMP
        fprintf(stderr, "bb=%d (%p)", DP(bb)->id, op);
        knh_opcode_dump(ctx, op, KNH_STDERR, NULL);
#endif
        switch (op->opcode) {
            OPCASE(NSET) {
                klr_NSET_t *op_ = (klr_NSET_t*) op;
                asm_nset(ctx, bb, regTable, op_->a, op_->n);
                break;
            }
            OPCASE(NSET2) {
                klr_NSET2_t *op_ = (klr_NSET2_t*) op;
                asm_nset(ctx, bb, regTable, op_->a, op_->n);
                asm_nset(ctx, bb, regTable, op_->a + K_NEXTIDX, op_->n2);
                break;
            }
            OPCASE(NSET3) {
                klr_NSET3_t *op_ = (klr_NSET3_t*) op;
                asm_nset(ctx, bb, regTable, op_->a, op_->n);
                asm_nset(ctx, bb, regTable, op_->a + K_NEXTIDX * 1, op_->n2);
                asm_nset(ctx, bb, regTable, op_->a + K_NEXTIDX * 2, op_->n3);
                break;
            }
            OPCASE(NSET4) {
                klr_NSET4_t *op_ = (klr_NSET4_t*) op;
                asm_nset(ctx, bb, regTable, op_->a, op_->n);
                asm_nset(ctx, bb, regTable, op_->a + K_NEXTIDX * 1, op_->n2);
                asm_nset(ctx, bb, regTable, op_->a + K_NEXTIDX * 2, op_->n3);
                asm_nset(ctx, bb, regTable, op_->a + K_NEXTIDX * 3, op_->n4);
                break;
            }
            OPCASE(OSET) {
                klr_OSET_t *op_ = (klr_OSET_t*) op;
                asm_oset(ctx, bb, regTable, op_->a + K_NEXTIDX * 0, op_->o);
                break;
            }
            OPCASE(OSET2) {
                klr_OSET2_t *op_ = (klr_OSET2_t*) op;
                asm_oset(ctx, bb, regTable, op_->a + K_NEXTIDX * 0, op_->v);
                asm_oset(ctx, bb, regTable, op_->a + K_NEXTIDX * 1, op_->v2);
                break;
            }
            OPCASE(OSET3) {
                klr_OSET3_t *op_ = (klr_OSET3_t*) op;
                asm_oset(ctx, bb, regTable, op_->a + K_NEXTIDX * 0, op_->v);
                asm_oset(ctx, bb, regTable, op_->a + K_NEXTIDX * 1, op_->v2);
                asm_oset(ctx, bb, regTable, op_->a + K_NEXTIDX * 2, op_->v3);
                break;
            }
            OPCASE(OSET4) {
                klr_OSET4_t *op_ = (klr_OSET4_t*) op;
                asm_oset(ctx, bb, regTable, op_->a + K_NEXTIDX * 0, op_->v);
                asm_oset(ctx, bb, regTable, op_->a + K_NEXTIDX * 1, op_->v2);
                asm_oset(ctx, bb, regTable, op_->a + K_NEXTIDX * 2, op_->v3);
                asm_oset(ctx, bb, regTable, op_->a + K_NEXTIDX * 3, op_->v4);
                break;
            }
            OPCASE(iEQ)OPCASE(iNEQ)OPCASE(iLT)OPCASE(iLTE)OPCASE(iGT)OPCASE(iGTE) {
                klr_iLT_t *op_ = (klr_iLT_t*) op;
                int isLoaded  = reg_isLoaded(regTable, op_->a);
                int isLoaded2 = reg_isLoaded(regTable, op_->b);
                int opcode = op_->opcode - OPCODE_iEQC;
                reg_t r  = regalloc(regTable, NDAT, op_->a);
                reg_t r2 = regalloc(regTable, NDAT, op_->b);
                if (!isLoaded) {
                    if (r == PREG_NOP) { TODO(); }
                    PASM(LOADR, reg(r), ndat(op_->a));
                }
                if (!isLoaded2) {
                    if (r2 == PREG_NOP) { TODO(); }
                    PASM(LOADR, reg(r2), ndat(op_->b));
                }
                TODO();
                PASM_CMP(CMPR , reg(r), reg(r2), condop[opcode], 0);
                break;
            }
            OPCASE(iEQC)OPCASE(iNEQC)OPCASE(iLTC)OPCASE(iLTEC)OPCASE(iGTC)OPCASE(iGTEC) {
                klr_iLTC_t *op_ = (klr_iLTC_t*) op;
                int isLoaded = reg_isLoaded(regTable, op_->a);
                int opcode = op_->opcode - OPCODE_iEQC;
                reg_t r = regalloc(regTable, NDAT, op_->a);
                knh_int_t n = op_->n;
                if (!isLoaded) {
                    if (r == PREG_NOP) { TODO(); }
                    PASM(LOADR, reg(r), ndat(op_->a));
                }
                if (n >= 0x80000000) { // n is too big to compare directly.
                    TODO();
                    PASM(LOADR, reg(PREG0), data(n));
                    PASM_CMP(CMPR , reg(r), reg(PREG0), condop_c[opcode], 0);
                } else {
                    PASM_CMP(CMPN, reg(r), data(n), condop_c[opcode], 0);
                }
                TODO();
                break;
            }
            OPCASE(iJEQ)OPCASE(iJNEQ)OPCASE(iJLT)OPCASE(iJLTE)OPCASE(iJGT)OPCASE(iJGTE)
                OPCASE(iJEQC)OPCASE(iJNEQC)OPCASE(iJLTC)OPCASE(iJLTEC)OPCASE(iJGTC)OPCASE(iJGTEC) {
                    fprintf(stderr, "non reachable\n");
                    KNH_ASSERT(0);
                    break;
                }
            OPCASE(iINC) {
                klr_iINC_t *op_ = (klr_iINC_t*) op;
                int isLoaded = reg_isLoaded(regTable, op_->a);
                reg_t r = regalloc(regTable, NDAT, op_->a);
                if (!isLoaded) {
                    PASM(LOADR, reg(r), ndat(op_->a));
                }
                PASM(INC, reg(r), nop());
                break;
            }
            OPCASE(iDEC) {
                klr_iDEC_t *op_ = (klr_iDEC_t*) op;
                int isLoaded = reg_isLoaded(regTable, op_->a);
                reg_t r = regalloc(regTable, NDAT, op_->a);
                if (!isLoaded) {
                    PASM(LOADR, reg(r), ndat(op_->a));
                }
                PASM(DEC, reg(r), nop());
                break;
            }
            OPCASE(JMP) {
                fprintf(stderr, "non reachable\n");
                KNH_ASSERT(0);
                break;
            }
            OPCASE(NMOV) {
                klr_NMOV_t *op_ = (klr_NMOV_t*) op;
                asm_nmov(ctx, bb, regTable, op_->a, op_->b);
                break;
            }
            OPCASE(NNMOV) {
                klr_NNMOV_t *op_ = (klr_NNMOV_t*) op;
                asm_nmov(ctx, bb, regTable, op_->a, op_->b);
                asm_nmov(ctx, bb, regTable, op_->c, op_->d);
                break;
            }
            OPCASE(OMOV) {
                klr_OMOV_t *op_ = (klr_OMOV_t*) op;
                asm_omov(ctx, bb, regTable, op_->a, op_->b);
                break;
            }
            OPCASE(ONMOV) {
                klr_ONMOV_t *op_ = (klr_ONMOV_t*) op;
                asm_omov(ctx, bb, regTable, op_->a, op_->b);
                asm_nmov(ctx, bb, regTable, op_->c, op_->d);
                break;
            }

            OPCASE(OOMOV) {
                klr_OOMOV_t *op_ = (klr_OOMOV_t*) op;
                asm_omov(ctx, bb, regTable, op_->a, op_->b);
                asm_omov(ctx, bb, regTable, op_->c, op_->d);
                break;
            }
            OPCASE(JMP_) OPCASE(RET) {
                store_regs(ctx, bb, regTable, 0, MTDEND);
                PASM(EXIT, nop(), nop());
                //fprintf(stderr, "bb bb=(%p,id:%d)\n", bb, DP(bb)->id);
                break;
            }
            OPCASE(VCALL) OPCASE(VCALL_) {
                klr_VCALL_t *op_ = (klr_VCALL_t*) op;
                knh_sfpidx_t thisidx  = op_->thisidx;
                //knh_sfpidx_t espshift = op_->espshift;
                goto L_end;
#if 0
                knh_Method_t *callmtd = op_getmtd(op_);
                store_regs(ctx, bb, regTable, thisidx+K_RTNIDX, FUNCCALL);
                PASM(MOVRR, reg(PARG0), reg(RCTX));
                PASM_SETESP(ctx, bb, op_->espshift);
                /* set args */
                PASM(MOVRR, reg(PARG1), reg(RSFP));
                PASM(ADDRN, reg(PARG1), data(thisidx * SHIFT));
                PASM(LOADN, reg(PARG2), data(K_RTNIDX));
                /* TODO */
                //if (mtd == callmtd) {
                //    PASM(CALL , func(NULL), data(PJIT_RELOCATION_SYMBOL_SELF));
                //} else {
                //    PASM(STOREN, obj(thisidx + K_MTDIDX), data(callmtd));
                //    PASM(CALL , func(callmtd->fcall_1), nop());
                //}
                PASM(STOREN, obj(thisidx + K_MTDIDX), data(callmtd));
                PASM(CALL , func(callmtd->fcall_1), nop());
#endif
                break;
            }
            OPCASE(SCALL) {
                klr_SCALL_t *op_ = (klr_SCALL_t*) op;
                goto L_end;
#if 0
                knh_sfpidx_t thisidx  = op_->thisidx;
                //knh_sfpidx_t espshift = op_->espshift;
                knh_Method_t *callmtd = op_getmtd(op_);
                store_regs(ctx, bb, regTable, thisidx+K_RTNIDX, FUNCCALL);
                /* set esp */
                PASM(MOVRR, reg(PARG0), reg(RCTX));
                PASM_SETESP(ctx, bb, op_->espshift);
                /* set args */
                PASM(MOVRR, reg(PARG1), reg(RSFP));
                PASM(ADDRN, reg(PARG1), data(thisidx * SHIFT));
                PASM(LOADN, reg(PARG2), data(K_RTNIDX));
                /* TODO sfp_[K_MTDIDX].callmtd = mtd_ */
                PASM(CALL , func(callmtd->fcall_1), nop());
#endif
                break;
            }
            OPCASE(FASTCALL0) {
                klr_FASTCALL0_t *op_ = (klr_FASTCALL0_t*) op;
                knh_sfpidx_t thisidx  = op_->thisidx;
                knh_intptr_t rix = op_->rix;
                klr_Ffcall fcall = op_->fcall;
                //goto L_end;
                //knh_sfpidx_t espshift = op_->espshift;
                store_regs(ctx, bb, regTable, thisidx+K_RTNIDX, FUNCCALL);
                PASM(MOVRR, reg(RCTX), reg(PARG0));
                PASM(MOVRR, reg(PARG1), reg(RSFP));
                PASM(ADDRN, reg(PARG1), data(thisidx * SHIFT));
                PASM(LOADN, reg(PARG2), data(rix));
                /* TODO sfp_[K_MTDIDX].callmtd = mtd_ */
                PASM(CALL , func(fcall), nop());
                break;
            }
            OPCASE(TR) {
                klr_TR_t *op_ = (klr_TR_t*) op;
                //knh_sfpidx_t diff = (op_->a - op_->b) / 2;
                knh_sfpidx_t diff = op_->rix;
                /* XXX check out store_regs, op_->a, op_->b */
                store_regs(ctx, bb, regTable, op_->a, FUNCCALL);
                PASM(MOVRR, reg(RCTX), reg(PARG0));
                PASM(MOVRR, reg(PARG1), reg(RSFP));
                PASM(ADDRN, reg(PARG1), data(op_->b * SHIFT));
                PASM(LOADN, reg(PARG2), data(diff));
                PASM(LOADN, reg(PARG3), data(op_->cid));
                PASM(CALL , func(op_->tr), nop());
                break;
            }
            OPCASE(OMOVx) {
                klr_OMOVx_t *op_ = (klr_OMOVx_t*) op;
                reg_t r, r2;
                int pos = sizeof(knh_int_t) * op_->b.n;
                if (reg_isLoaded(regTable, op_->b.i)) {
                    r2 = regalloc(regTable, OBJ, op_->b.i);
                } else {
                    r2 = regalloc(regTable, OBJ, op_->b.i);
                    //r2 = PREG0;
                    PASM(LOADR , reg(r2), obj(op_->b.i)); /* sfp[b.i].ox */
                }
                r = regalloc(regTable, NDAT, op_->a);
                //assert(r != PREG0);
                //if (r2 != PREG0) {
                //    PASM(MOVRR, reg(PREG0), reg(r2));
                //}
                PASM(GETIDX, reg(r2), data(FIELD_OFFSET));  /* PREG0 = sfp[b.i].ox->fields */
                PASM(GETIDX, reg(PREG0), data(pos));/* PREG0 = sfp[b.i].ox->fields[a] */
                PASM(MOVRR , reg(r), reg(PREG0));
                break;
            }
            OPCASE(NMOVx) {
                klr_NMOVx_t *op_ = (klr_NMOVx_t*) op;
                reg_t r, r2;
                int pos = sizeof(knh_int_t) * op_->b.n;
                if (reg_isLoaded(regTable, op_->b.i)) {
                    r2 = regalloc(regTable, OBJ, op_->b.i);
                } else {
                    r2 = regalloc(regTable, OBJ, op_->b.i);
                    //r2 = PREG0;
                    PASM(LOADR , reg(r2), obj(op_->b.i)); /* sfp[b.i].ox */
                }
                r = regalloc(regTable, NDAT, op_->a);
                //assert(r != PREG0);
                //if (r2 != PREG0) {
                //    PASM(MOVRR, reg(PREG0), reg(r2));
                //}
                PASM(GETIDX, reg(r2), data(FIELD_OFFSET));  /* PREG0 = sfp[b.i].ox->fields */
                PASM(GETIDX, reg(PREG0), data(pos));/* PREG0 = sfp[b.i].ox->fields[a] */
                PASM(MOVRR , reg(r), reg(PREG0));
                break;
            }
            OPCASE(XNMOV) {
                klr_XNMOV_t *op_ = (klr_XNMOV_t*) op;
                reg_t r, r2;
                int pos = sizeof(knh_int_t) * op_->a.n;
                int isLoadedR1 = reg_isLoaded(regTable, op_->a.i);
                int isLoadedR2 = reg_isLoaded(regTable, op_->b);
                r2 = regalloc(regTable, NDAT, op_->b);
                if (!isLoadedR2) {
                    PASM(LOADR , reg(r2), ndat(op_->b));
                }
                if (isLoadedR1) {
                    r = regalloc(regTable, OBJ, op_->a.i);
                } else {
                    r = PREG0;
                    PASM(LOADR, reg(r), obj(op_->a.i)); /* sfp[b.i].ox */
                }
                assert(r != PREG_NOP);
                /* TODO remove loading object fields */
                PASM(GETIDX, reg(r), data(FIELD_OFFSET));  /* sfp[b.i].ox->fields */
                PASM(SETIDX, reg(r2), data(pos));/* sfp[b.i].ox->fields[a] */
                break;
            }
            OPCASE(XMOV) {
                klr_XMOV_t *op_ = (klr_XMOV_t*) op;
                reg_t r, r2;
                int pos = sizeof(knh_int_t) * op_->a.n;
                int isLoadedR1 = reg_isLoaded(regTable, op_->a.i);
                int isLoadedR2 = reg_isLoaded(regTable, op_->b);
                r2 = regalloc(regTable, OBJ, op_->b);
                if (!isLoadedR2) {
                    PASM(LOADR , reg(r2), obj(op_->b));
                }
                if (isLoadedR1) {
                    r = regalloc(regTable, OBJ, op_->a.i);
                } else {
                    r = PREG0;
                    PASM(LOADR, reg(r), obj(op_->a.i)); /* sfp[b.i].ox */
                }
                assert(r != PREG_NOP);
                /* TODO remove loading object fields */
                PASM(GETIDX, reg(r), data(FIELD_OFFSET));  /* sfp[b.i].ox->fields */
                PASM(SETIDX, reg(r2), data(pos));/* sfp[b.i].ox->fields[a] */
                break;
            }
            OPCASE(XNSET) {
                klr_XNSET_t *op_ = (klr_XNSET_t*) op;
                reg_t r, r2;
                int pos = sizeof(knh_int_t) * op_->a.n;
                int isLoadedR1 = reg_isLoaded(regTable, op_->a.i);
                //int isLoadedR2 = reg_isLoaded(regTable, op_->b);
                r2 = regalloc(regTable, NDAT, op_->b);
                PASM(LOADN, reg(r2), data(op_->b));
                if (isLoadedR1) {
                    r = regalloc(regTable, OBJ, op_->a.i);
                } else {
                    r = PREG0;//regalloc(regTable, OBJ, op_->a.i);
                    PASM(LOADR, reg(r), obj(op_->a.i)); /* sfp[b.i].ox */
                }
                assert(r != PREG_NOP);
                /* TODO remove loading object fields */
                PASM(GETIDX, reg(r), data(FIELD_OFFSET));  /* sfp[b.i].ox->fields */
                PASM(SETIDX, reg(r2), data(pos));/* sfp[b.i].ox->fields[a] */
                clear_reg(r2);
                break;
            }
            OPCASE(XOSET) {
                klr_XOSET_t *op_ = (klr_XOSET_t*) op;
                reg_t r, r2;
                int pos = sizeof(knh_int_t) * op_->a.n;
                int isLoadedR1 = reg_isLoaded(regTable, op_->a.i);
                //int isLoadedR2 = reg_isLoaded(regTable, op_->b);
                r2 = regalloc(regTable, OBJP, -1);
                PASM(LOADN, reg(r2), objp(op_->b));
                if (isLoadedR1) {
                    r = regalloc(regTable, OBJ, op_->a.i);
                } else {
                    r = PREG0;
                    PASM(LOADR, reg(r), obj(op_->a.i)); /* sfp[b.i].ox */
                }
                clear_reg(r2);
                assert(r != PREG_NOP);
                /* TODO remove loading object fields */
                PASM(GETIDX, reg(r), data(FIELD_OFFSET));  /* sfp[b.i].ox->fields */
                PASM(SETIDX, reg(r2), data(pos));/* sfp[b.i].ox->fields[a] */
                break;
            }
            OPCASE(iNEG) {
                klr_iNEG_t *op_ = (klr_iNEG_t*) op;
                reg_t r0, r1;
                int isLoaded1 = reg_isLoaded(regTable, op_->a);
                r0 = regalloc(regTable, NDAT, op_->c);
                if (isLoaded1) {
                    r1 = regalloc(regTable, NDAT, op_->a);
                    PASM(MOVRR, reg(r0), reg(r1));
                    ///* TODO register allocation */
                    reg_store_(ctx, bb, regTable, op_->a);
                    clear_reg(r1);
                } else {
                    PASM(LOADR, reg(r0), ndat(op_->a));
                }
                PASM(MULRN, reg(r0), data(-1));
                break;
            }

            OPCASE(iADD)OPCASE(iSUB)OPCASE(iMUL) {
                klr_iADD_t *op_ = (klr_iADD_t*) op;
                reg_t r0, r1, r2;
                //int isLoaded0 = reg_isLoaded(regTable, op_->c);
                int isLoaded1 = reg_isLoaded(regTable, op_->a);
                int isLoaded2 = reg_isLoaded(regTable, op_->b);
                r0 = regalloc(regTable, NDAT, op_->c);
                //asm volatile("int3");
                if (isLoaded1) {
                    r1 = regalloc(regTable, NDAT, op_->a);
                    PASM(MOVRR, reg(r0), reg(r1));
                    ///* TODO register allocation */
                    reg_store_(ctx, bb, regTable, op_->a);
                    clear_reg(r1);
                } else {
                    PASM(LOADR, reg(r0), ndat(op_->a));
                }

                r2 = regalloc(regTable, NDAT, op_->b);
                if (!isLoaded2) {
                    PASM(LOADR, reg(r2), ndat(op_->b));
                }

                //PASM_iOP(ctx, bb, op_->opcode, r, r3);
                if (op_->opcode == OPCODE_iADD) {
                    PASM(ADDRR, reg(r0), reg(r2));
                } else if (op_->opcode == OPCODE_iSUB) {
                    PASM(SUBRR, reg(r0), reg(r2));
                } else if (op_->opcode == OPCODE_iMUL) {
                    PASM(MULRR, reg(r0), reg(r2));
                }
                //reg_store_(ctx, bb, regTable, op_->b);
                //clear_reg(r2);
                //reg_store_(ctx, bb, regTable, op_->c);
                //clear_reg(r0);

                break;
            }
            OPCASE(iADDC)OPCASE(iSUBC) {
                klr_iADDC_t *op_ = (klr_iADDC_t*) op;
                reg_t r, r2;
                if (reg_isLoaded(regTable, op_->a)) {
                    // c = a + n
                    r2 = regalloc(regTable, NDAT, op_->a);
                    r  = regalloc(regTable, NDAT, op_->c);
                    if (r2 == PREG_NOP) {
                        TODO();
                    }
                    PASM(MOVRR, reg(r), reg(r2));
                    reg_store_(ctx, bb, regTable, op_->a);
                    clear_reg(r2);
                } else {
                    r  = regalloc(regTable, NDAT, op_->c);
                    PASM(LOADR, reg(r), ndat(op_->a));
                }
                if (op_->opcode == OPCODE_iADDC) {
                    PASM(ADDRN, reg(r), data(op_->n));
                } else if (op_->opcode == OPCODE_iSUBC) {
                    PASM(SUBRN, reg(r), data(op_->n));
                } else if (op_->opcode == OPCODE_iMULC) {
                    PASM(MULRN, reg(r), data(op_->n));
                }
                break;
            }
            OPCASE(fADD) OPCASE(fSUB) OPCASE(fMUL) OPCASE(fDIV) {
                klr_fADD_t *op_ = (klr_fADD_t*) op;
                if (reg_isLoaded(regTable, op_->a)) {
                    reg_t r = regalloc(regTable, NDAT, op_->a);
                    reg_store_(ctx, bb, regTable, op_->a);
                    clear_reg(r);
                }
                if (reg_isLoaded(regTable, op_->b)) {
                    reg_t r = regalloc(regTable, NDAT, op_->b);
                    reg_store_(ctx, bb, regTable, op_->b);
                    clear_reg(r);
                }
                PASM(LOADFR, reg(RSFP), data(op_->a * SHIFT));
                int opcode = op_->opcode - OPCODE_fADD + PCODE_ADDF;
                PASM_(opcode, reg(RSFP), data(op_->b * SHIFT));
                // store 0x00(%r) = %ftmp0
                PASM(STOREF, reg(RSFP), data(op_->c * SHIFT));
                break;
            }
            OPCASE(fADDC) OPCASE(fSUBC) OPCASE(fMULC) OPCASE(fDIVC) {
                klr_fADDC_t *op_ = (klr_fADDC_t*) op;
                reg_t r0, r1;
                int isLoaded1 = reg_isLoaded(regTable, op_->a);
                r0 = regalloc(regTable, NDAT, op_->c);

                if (isLoaded1) {
                    r1 = regalloc(regTable, NDAT, op_->a);
                    PASM(MOVRR, reg(r0), reg(r1));
                    DBG_ASSERT(1);
                } else {
                    PASM(LOADFR, reg(r0), data(op_->a * SHIFT));
                }

                ndata_t v;
                v.f = op_->n;
                int opcode = op_->opcode - OPCODE_fADDC + PCODE_ADDFN;
                PASM_(opcode, reg(r0), flat(v.i));

                // ORIGINAL VERSION
                //PASM(LOADFR, reg(RSFP), data(op_->a * SHIFT));
                //PASM_(opcode, reg(rsp), flat(v.i));
                //// store 0x00(%r) = %ftmp0
                //PASM(STOREF, reg(RSFP), data(op_->c * SHIFT));

                break;
            }
            OPCASE(fEQ)OPCASE(fNEQ)OPCASE(fLT)OPCASE(fLTE)OPCASE(fGT)OPCASE(fGTE)
                OPCASE(fEQC)OPCASE(fNEQC)OPCASE(fLTC)OPCASE(fLTEC)OPCASE(fGTC)OPCASE(fGTEC) {
                    fprintf(stderr, "non reachable\n");
                    KNH_ASSERT(0);
                    break;
                }
            OPCASE(fCAST) {
                klr_fCAST_t *op_ = (klr_fCAST_t*) op;
                reg_t r;
                if (!reg_isLoaded(regTable, op_->b)) {
                    r = regalloc(regTable, NDAT, op_->b);
                    PASM(LOADR, reg(r), ndat(op_->b));
                } else {
                    r  = regalloc(regTable, NDAT, op_->a);
                }
                PASM(FCASTR, reg(r), nop());
                PASM(STOREF, reg(RSFP), data(op_->a * SHIFT));
                clear_reg(r);
                break;
            }
            OPCASE(iCAST) {
                klr_iCAST_t *op_ = (klr_iCAST_t*) op;
                reg_t r;
                if (reg_isLoaded(regTable, op_->b)) {
                    reg_store_(ctx, bb, regTable, op_->b);
                }
                r = regalloc(regTable, NDAT, op_->a);
                PASM(LOADFR, reg(RSFP), data(op_->b * SHIFT));
                PASM(ICASTR, reg(r), nop());
                break;
            }
            OPCASE(CHKIDX) {
                klr_CHKIDX_t *op_ = (klr_CHKIDX_t*) op;
                reg_t r, r2;
                if (!reg_isLoaded(regTable, op_->a)) {
                    r = regalloc(regTable, OBJ, op_->a);
                    PASM(LOADR, reg(r), obj(op_->a));
                } else {
                    r = regalloc(regTable, OBJ, op_->a);
                }
                if (!reg_isLoaded(regTable, op_->n)) {
                    r2 = regalloc(regTable, NDAT, op_->n);
                    PASM(LOADR, reg(r2), ndat(op_->n));
                } else {
                    r2 = regalloc(regTable, NDAT, op_->n);
                }
                store_regs(ctx, bb, regTable, op_->a, FUNCCALL);
                PASM_CHKIDX(ctx, bb, r, r2);
                break;
            }
            OPCASE(CHKIDXC) {
                klr_CHKIDXC_t *op_ = (klr_CHKIDXC_t*) op;
                reg_t r, r2;
                if (!reg_isLoaded(regTable, op_->a)) {
                    r = regalloc(regTable, OBJ, op_->a);
                    PASM(LOADR, reg(r), obj(op_->a));
                } else {
                    r = regalloc(regTable, OBJ, op_->a);
                }
                if (!reg_isLoaded(regTable, op_->n)) {
                    r2 = regalloc(regTable, NDAT, op_->n);
                    PASM(LOADN, reg(r2), ndat(op_->n));
                } else {
                    r2 = regalloc(regTable, NDAT, op_->n);
                }
                store_regs(ctx, bb, regTable, op_->a, FUNCCALL);
                PASM_CHKIDX(ctx, bb, r, r2);
                break;
            }
            OPCASE(NGETIDX) {
                klr_NGETIDX_t *op_ = (klr_NGETIDX_t*) op;
                reg_t r, r2, r3;
                if (reg_isLoaded(regTable, op_->a)) {
                    r2 = regalloc(regTable, OBJ, op_->a);
                } else {
                    r2 = regalloc(regTable, OBJ, op_->a);
                    //r2 = PREG0;
                    PASM(LOADR , reg(r2), obj(op_->a)); /* sfp[a].a */
                }
                if (reg_isLoaded(regTable, op_->n)) {
                    r3 = regalloc(regTable, NDAT, op_->n);
                } else {
                    r3 = regalloc(regTable, NDAT, op_->n);
                    PASM(LOADR , reg(r3), ndat(op_->n)); /* n */
                }
                r = regalloc(regTable, NDAT, op_->c);
                //assert(r != PREG0);
                PASM(GETIDX, reg(r2), data(LIST_OFFSET));  /* PREG0 = sfp[a].a->list */
                PASM(GETIDX, reg(PREG0), reg(r3));/* PREG0 = sfp[a].a->list[n] */
                PASM(MOVRR , reg(r), reg(PREG0));
                break;
            }
            OPCASE(NGETIDXC) {
                klr_NGETIDXC_t *op_ = (klr_NGETIDXC_t*) op;
                reg_t r, r2, r3;
                if (reg_isLoaded(regTable, op_->a)) {
                    r2 = regalloc(regTable, OBJ, op_->a);
                } else {
                    r2 = regalloc(regTable, OBJ, op_->a);
                    //r2 = PREG0;
                    PASM(LOADR , reg(r2), obj(op_->a)); /* sfp[a].a */
                }
                if (reg_isLoaded(regTable, op_->n)) {
                    r3 = regalloc(regTable, NDAT, op_->n);
                } else {
                    r3 = regalloc(regTable, NDAT, op_->n);
                    PASM(LOADN , reg(r3), data(op_->n)); /* n */
                }
                r = regalloc(regTable, NDAT, op_->c);
                assert(r != PREG0);
                PASM(GETIDX, reg(r2), data(LIST_OFFSET));  /* PREG0 = sfp[a].a->list */
                PASM(GETIDX, reg(PREG0), reg(r3));/* PREG0 = sfp[a].a->list[n] */
                PASM(MOVRR , reg(r), reg(PREG0));
                break;
            }
            OPCASE(NSETIDX) {
                klr_NSETIDX_t *op_ = (klr_NSETIDX_t*) op;
                reg_t r, r2, r3;
                int isLoaded = reg_isLoaded(regTable, op_->v);
                if (reg_isLoaded(regTable, op_->a)) {
                    r2 = regalloc(regTable, OBJ, op_->a);
                } else {
                    r2 = regalloc(regTable, OBJ, op_->a);
                    PASM(LOADR , reg(r2), obj(op_->a)); /* sfp[a].a */
                }
                if (reg_isLoaded(regTable, op_->n)) {
                    r3 = regalloc(regTable, NDAT, op_->n);
                } else {
                    r3 = regalloc(regTable, NDAT, op_->n);
                    PASM(LOADN , reg(r3), data(op_->n)); /* n */
                }
                r = regalloc(regTable, NDAT, op_->v);
                if (!isLoaded) {
                    PASM(LOADR , reg(r), ndat(op_->v));
                }
                assert(r != PREG0);
                PASM(GETIDX, reg(r2), data(LIST_OFFSET));  /* PREG0 = sfp[a].a->list */
                //PASM(GETIDX, reg(PREG0), reg(r3));/* PREG0 = sfp[a].a->list[n] */
                PASM(SETIDX, reg(r), reg(r3));/* sfp[a].a->list[n] = r */
                break;
            }
            OPCASE(OGETIDXC) {
                break;
            }
            OPCASE(OGETIDX) {
                break;
            }
            OPCASE(fNEG) {
                break;
            }
            OPCASE(HALT) OPCASE(THCODE) {
                iprintf("JIT compile error at %s(%d) at %d",
                        OPCODE__(op->opcode),
                        op->opcode, op->line);
                asm volatile("int3");
                break;
            }
            default :
            iprintf("not surpport yet %s(%d)",
                    OPCODE__(op->opcode), op->opcode);
            asm volatile("int3");
            break;
            //case OPCODE_:
            //    break;
        }
    }
L_end:;
    store_regs(ctx, bb, regTable, 0, BLOCKEND);
    //PASM(HALT, nop(), nop());
    PASM(EXIT, nop(), nop());
    *oplen = j;
}

static void pcode_optimize(CTX ctx, knh_BasicBlock_t *bb, pindex_t *regTable)
{
#if 0
    int j;
    pcode_t *op, *opP;
    pdata_t *pdata = (pdata_t*) DP(bb)->code;
    opP = pdata->opbuf;
    for (j = 1; j < pdata->size; j++) {
        op = pdata->opbuf + j;
        if (opP->opcode == PCODE_LOADN && op->opcode == PCODE_STORER) {
            if (opP->a.index == op->b.index) {
                opP->opcode  = PCODE_STOREN;
                op->opcode = PCODE_NOP;
                opP->a.type = op->a.type;
                opP->a.data = op->a.data;
                op->a.type = NOP;
                op->b.type = NOP;
            }
        }
        if (opP->opcode == PCODE_LOADN && op->opcode == PCODE_LOADN) {
            if (opP->a.index == op->a.index && opP->a.type == op->a.type) {
                opP->opcode = PCODE_NOP;
                opP->a.type = NOP;
                opP->b.type = NOP;
            }
        }

        if (opP->opcode == PCODE_ADDRN) {
            if (opP->b.data == 0) {
                opP->opcode = PCODE_NOP;
            }
        }
        if (opP->opcode == PCODE_MOVRR) {
            if (opP->a.reg == opP->b.reg) {
                opP->opcode = PCODE_NOP;
            }
        }

        opP = op;
    }
#endif
}

static struct pjit *new_pjit(CTX ctx, knh_cwb_t *cwb)
{
    struct pjit *pjit = (struct pjit*)KNH_MALLOC(ctx, sizeof(struct pjit));
    pjit->state = PJIT_INIT;
    pjit->cwb   = cwb;
    pjit->ctx   = ctx;
    pjit->cur_pos = 0;
    KNH_INITv(pjit->symbolList,new_Array(ctx, CLASS_Int, 0));
    return pjit;
}

static void pjit_delete(CTX ctx, struct pjit *pjit)
{
    KNH_FINALv(ctx, pjit->symbolList);
    KNH_FREE(ctx, pjit, sizeof(struct pjit));
}
//static void IArray_add(CTX ctx, knh_Array_t *a, knh_int_t v)
//{
//    BEGIN_LOCAL(ctx, lsfp, 1);
//    lsfp[0].ivalue = v;
//    a->api->add(ctx, a, lsfp);
//    END_LOCAL_(ctx, lsfp);
//}
//static void pjit_addRelocationCode(struct pjit *pjit, size_t size, int type, knh_uintptr_t target)
//{
//    CTX ctx = pjit->ctx;
//    knh_Array_t *symbolList = pjit->symbolList;
//    IArray_add(ctx, symbolList, type);
//    IArray_add(ctx, symbolList, target);
//    IArray_add(ctx, symbolList, size);
//    IArray_add(ctx, symbolList, pjit->cur_pos);
//}
static void pjit_rewrite_symbols(struct pjit *pjit, unsigned char *mem)
{
#define IArray_get(a, i)             ((a)->ilist[i])
    knh_Array_t *symbolList = pjit->symbolList;
    size_t i;
    for (i = 0; i < knh_Array_size(symbolList); i+=4) {
        knh_int_t type = IArray_get(symbolList, i + 0);
        knh_int_t sym  = IArray_get(symbolList, i + 1);
        knh_int_t size = IArray_get(symbolList, i + 2);
        knh_int_t pos  = IArray_get(symbolList, i + 3);
        union {
            unsigned char code[sizeof(knh_int_t)];
            knh_int_t target;
        } code;
        if (type == FUNC) {
            code.target = sym;
        }
        else if (type == SELF) {
            code.target = (knh_int_t) mem; /* + sym */
        }
        __write(code.code, sizeof(code));
        assert(sizeof(code) == size);
        memcpy(mem + pos - size, code.code, size);
    }
}
static void *_gencode2(CTX ctx, knh_cwb_t *cwb, int *lenp)
{
    knh_bytes_t codes = knh_cwb_tobytes(cwb);
    unsigned char *mem;
    int len, align = (int) ((uint16_t)(0x10 - codes.len % 0x10));
    len = codes.len + align;
    mem = (unsigned char *) __xmalloc(ctx, len);
    memcpy(mem, codes.ubuf, len);
    add_nops((void*)((intptr_t)mem + len), align);
#ifdef PCODE_DUMP
    //jit_dump(mem, len);
#endif
    //knh_xfree(ctx, mem, len);
    //return NULL;
    *lenp = len;
    return mem;
}

static void *_gencode(CTX ctx, struct pjit *pjit)
{
    int len;
    unsigned char *mem = (unsigned char *)_gencode2(ctx, pjit->cwb, &len);
    pjit_rewrite_symbols(pjit, mem);
    pjit_delete(ctx, pjit);
#ifdef PCODE_DUMP
    jit_dump(mem, len);
#endif
    //knh_xfree(ctx, mem, len);
    //return NULL;
    return mem;
}

static const Type *getLongTy(Module *m)
{
#if defined(SIZEOF_VOIDP) && (SIZEOF_VOIDP == 4)
    return Type::getInt32Ty(m->getContext());
#else
    return Type::getInt64Ty(m->getContext());
#endif
}
static const Type *getShortTy(Module *m)
{
#if defined(SIZEOF_VOIDP) && (SIZEOF_VOIDP == 4)
    return Type::getInt16Ty(m->getContext());
#else
    return Type::getInt32Ty(m->getContext());
#endif
}

static Module *g_module = NULL;
static ExecutionEngine *g_engine = NULL;
static IRBuilder<> *g_builder = NULL;
static Function *g_function = NULL;

static Module *MODULE()
{
    return g_module;
}

static void VALUE_set(struct pjit *pjit, int index, Value *v);
static Value *VALUE_get(struct pjit *pjit, int index)
{
    knh_Array_t *lstacks = (knh_Array_t*) (pjit->data);
    knh_sfp_t lsfp = {};
    //index = index + (-1 * K_RTNIDX);
    lsfp.a = lstacks;
    lstacks->api->get(pjit->ctx, &lsfp, index, 0);
    return (Value*) lsfp.ndata;
}

#define knh_Array_capacity(a) ((a)->dim->capacity)
static void VALUE_set(struct pjit *pjit, int index, Value *v)
{
    knh_Array_t *lstacks = (knh_Array_t*) (pjit->data);
    knh_sfp_t lsfp = {};
    //index = index + (-1 * K_RTNIDX);
    if ((int)knh_Array_capacity(lstacks) < index) {
        asm volatile("int3");
        knh_Array_grow(pjit->ctx, lstacks, index, index);
    }
    lsfp.ndata = (knh_ndata_t) v;
    lstacks->api->set(pjit->ctx, lstacks, index, &lsfp);
}

static IRBuilder<> *BUILDER()
{
    return g_builder;
}
static Function *FUNCTION()
{
    return g_function;
}
static void ConstructObjectStruct(Module *m);

static Function *build_function(Module *m)
{
    std::vector<const Type*> argsTy;
    std::string name("op");

    argsTy.push_back(LLVMTYPE_context);
    argsTy.push_back(LLVMTYPE_sfp);
    argsTy.push_back(LLVMTYPE_opline);
    FunctionType *fnTy = FunctionType::get(LLVMTYPE_opline, argsTy, false);
    Function *func = Function::Create(fnTy, GlobalValue::ExternalLinkage, name, m);
    Function::arg_iterator args = func->arg_begin();
    VNAME_(args++, "ctx");
    VNAME_(args++, "rbp");
    VNAME_(args  , "op");
    g_function = func;
    return func;
}

static void pcode_gen1(struct pjit *pjit, pdata_t *pdata, knh_BasicBlock_t *bb, int oplen)
{
    int j;
    Function *func = NULL;
    pjit->opsize = oplen;
    if (g_module == NULL) {
        InitializeNativeTarget();
        g_module = new Module("test", LLVM_CONTEXT());
        g_engine = EngineBuilder(g_module).setEngineKind(EngineKind::JIT).create();
        ConstructObjectStruct(g_module);
    }
    if (pjit->state == PJIT_EMIT) {
        func = build_function(MODULE());
        BasicBlock *bb = BasicBlock::Create(LLVM_CONTEXT(), "EntryBlock", func);
        g_builder = new IRBuilder<>(bb);
        pjit->data = (void*)  new_Array(pjit->ctx, CLASS_Int, 128);
    }
    for (j = 0; j < pdata->size; j++) {
        pcode_t *op = pdata->opbuf + j;
#ifdef PCODE_DUMP
        static int opnum = 0;
        if (pjit->state == PJIT_EMIT) {
            if (op->opcode != PCODE_NOP) {
                opnum++;
            }
            fprintf(stderr, "[%2d] bb[%d] ", opnum, DP(bb)->id);
        }
#endif
        EMIT(pjit, op);
    }
}
static void *pcode_gencode(CTX ctx, knh_BasicBlock_t *bb, pindex_t *regTable, int oplen)
{
    knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
    struct pjit *pjit = new_pjit(ctx, cwb);
    {
        pdata_t *pdata = (pdata_t*) DP(bb)->code;
        pcode_gen1(pjit, pdata, bb, oplen);
    }

    pjit->cur_pos = 0;
    pjit->state = PJIT_EMIT;
    {
        pdata_t *pdata = (pdata_t*) DP(bb)->code;
        pcode_gen1(pjit, pdata, bb, oplen);
    }
    pjit->state = PJIT_EXIT;
    {
        pdata_t *pdata = (pdata_t*) DP(bb)->code;
        pdata_delete(ctx, pdata);
    }
#if 0
    return _gencode(ctx, pjit);
#else
    Function *func = FUNCTION();
    FunctionPassManager pm(g_module);
    pm.add(new TargetData(*(g_engine->getTargetData())));
    pm.add(createVerifierPass());
    pm.add(createInstructionCombiningPass()); // Cleanup for scalarrepl.
    pm.add(createIndVarSimplifyPass());       // Canonicalize indvars
    pm.add(createGVNPass());                  // Remove redundancies
    pm.add(createSCCPPass());                 // Constant prop with SCCP
    pm.add(createConstantPropagationPass());
    pm.add(createDeadStoreEliminationPass()); // Delete dead stores
    pm.add(createDemoteRegisterToMemoryPass());
    pm.doInitialization();
    pm.run(*func);

    void *ptr =  (void*) g_engine->getPointerToFunction(func);
#ifdef PCODE_DUMP
    (func)->dump();
#endif
    return ptr;
#endif
}

static knh_BasicBlock_t *KonohaCode_toBasicBlock(CTX ctx, knh_opline_t *opS, knh_opline_t *opE)
{
    knh_opline_t *op;
    knh_BasicBlock_t* bb = new_BasicBlock(ctx);

    for (op = opS; op < opE; op++) {
        BasicBlock_setOriginal(ctx, bb, op);
        //fprintf(stderr, "opcode=%6s\n", OPCODE__(op->opcode));
    }
    return bb;
}

void *pjit_compile(CTX ctx, knh_opline_t *opS, knh_opline_t *opE, int *oplen)
{
    void *func;
    //BEGIN_LOCAL(ctx, lsfp, 4);
    //LOCAL_NEW(ctx, lsfp, 0, knh_BasicBlock_t*, bb, KonohaCode_toBasicBlock(ctx, opS, opE));
    knh_BasicBlock_t* bb = KonohaCode_toBasicBlock(ctx, opS, opE);
    pindex_t *regTable = createRegisterTable(ctx, 12);
    BasicBlock_setPcode(ctx, bb, regTable, oplen);
    pcode_optimize(ctx, bb, regTable);
    func = pcode_gencode(ctx, bb, regTable, *oplen);
    deleteRegisterTable(ctx, regTable);
#ifdef PCODE_DUMP
    //asm volatile("int3");
#endif
    if(DP(bb)->capacity > 0) {
        KNH_FREE(ctx, DP(bb)->opbuf, DP(bb)->capacity * sizeof(knh_opline_t));
        DP(bb)->opbuf = NULL;
        DP(bb)->capacity = 0;
    }
    //fprintf(stderr, "*****%d\n", *oplen);
    //END_LOCAL_(ctx, lsfp);
    return func;
}

//static const unsigned char template[] = {
//    0x49, 0x81, 0xc4, 0x40, 0x00, 0x00, 0x00
//};
//static const unsigned char load_jmp[] = {
//    0x49, 0x8b, 0x04, 0x24, 0xff, 0xe0
//};
struct opint {
    union {
        int64_t  i;
        void    *p;
        unsigned char v[sizeof(int64_t)];
    };
};
/*
 * movq  %r12,%rdx
 * movq  %r13,%rsi
 * movq  0xc8(%rbp),%rdi
 * movq  $0xcafebabeaabbccdd,%rcx
 * call  *%rcx
 * movq  %rax,%r12
 * movq  (%rax),%rax
 * jmp   *%rax
 */
static const unsigned char template1[] = {
    0x4c, 0x89, 0xe2,
    0x4c, 0x89, 0xee,
    0x48, 0x8b, 0x7d, 0xc8,
    0x48, 0xb9
};
/* 0xcafebabeaabbccdd */
static const unsigned char template2[] = {
    0xff, 0xd1,
    0x49, 0x89, 0xc4,
    0x48, 0x8b, 0x00,
    0xff, 0xe0,
};

static knh_opline_t *jit_nop1(CTX ctx, knh_rbp_t *rbp, knh_opline_t *op)
{
    return op + 1;
}
//static knh_opline_t *jit_nop2(CTX ctx, knh_rbp_t *rbp, knh_opline_t *op)
//{
//    return op + 2;
//}

knh_opline_t *check(CTX ctx, knh_rbp_t *rbp, knh_opline_t *op)
{
    asm volatile("int3");
    return op+1;
}

static void *write_wrapper(CTX ctx, knh_opline_t *opS, knh_opline_t *opE, void *ptr)
{
    knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
    int len = (opE - opS);// / sizeof(knh_opline_t);
    struct opint op;
    op.p = ptr;
    //fprintf(stderr, "size=%d\n", len);
    knh_Bytes_write(ctx, cwb->ba, new_bytes2((char*)template1, sizeof(template1)));
    knh_Bytes_write(ctx, cwb->ba, new_bytes2((char*)op.v, sizeof(op)));
    knh_Bytes_write(ctx, cwb->ba, new_bytes2((char*)template2, sizeof(template2)));
    return _gencode2(ctx, cwb, &len);
}
static void *compile(CTX ctx, knh_opline_t *opS, knh_opline_t *opE)
{
    int opsize = 0;
    void *ptr;
    if (opE - opS > 2) {
        void *func = pjit_compile(ctx, opS, opE, &opsize);
        ptr = write_wrapper(ctx, opS, opE, func);
    }
    else {
        ptr = (void*) jit_nop1;
    }

    return NULL;
}

void _TRACE(CTX ctx, knh_sfp_t *sfp, struct klr_PROBE_t *op)
{
    if (op->n++ == 10) {
        knh_opline_t *pc = ((knh_opline_t*) op) +1;
        knh_opline_t *opNext = pc;
        while (pc->opcode != OPCODE_RET) {
            if (knh_opcode_hasjump(pc->opcode)) {
                break;
            }
            pc++;
        }
        {
            void *ptr = compile(ctx, opNext, pc);
#ifndef K_USING_DEBUG
            if (ptr) {
                op->codeaddr = ptr;
            }
#endif
            //memcpy(op, opNext, sizeof(knh_opline_t));
        }
    }
}
static void ConstructObjectStruct(Module *m)
{
    // Type Definitions
    const Type *longTy , *shortTy;
    const Type *voidTy  = Type::getVoidTy(m->getContext());
    const Type *hObjectTy, *objectPtr;
    StructType* structTy;
    longTy  = getLongTy(m);
    shortTy = getShortTy(m);
    const Type *voidPtr = PointerType::get(longTy, 0);

    /* hObject */
    std::vector<const Type*>fields;
    fields.push_back(longTy);  /* magicflag */
    fields.push_back(voidPtr); /* cTBL */
    fields.push_back(longTy);  /* refc */
    fields.push_back(voidPtr); /* meta */

    structTy = StructType::get(m->getContext(), fields, /*isPacked=*/false);
    m->addTypeName("struct.hObject", structTy);
    hObjectTy = structTy;
    fields.clear();

    /* Object */
    fields.push_back(structTy);
    fields.push_back(voidPtr);
    fields.push_back(voidPtr);
    fields.push_back(voidPtr);
    fields.push_back(voidPtr);
    structTy = StructType::get(m->getContext(), fields, /*isPacked=*/false);
    objectPtr = PointerType::get(structTy, 0);
    LLVMTYPE_Object = objectPtr;
    m->addTypeName("Object", objectPtr);
    fields.clear();

    /* ObjectField */
    fields.push_back(hObjectTy);
    fields.push_back(objectPtr);
    fields.push_back(PointerType::get(objectPtr, 0));
    structTy = StructType::get(m->getContext(), fields, /*isPacked=*/false);
    LLVMTYPE_ObjectField = PointerType::get(structTy, 0);
    m->addTypeName("ObjectField", LLVMTYPE_ObjectField);
    fields.clear();

    /* sfp */
    fields.push_back(objectPtr);
    //fields.push_back(LLVMTYPE_Int);
    fields.push_back(LLVMTYPE_Float);
    structTy = StructType::get(m->getContext(), fields, /*isPacked=*/false);
    const Type *sfpPtr = PointerType::get(structTy, 0);
    m->addTypeName("struct.knh_sfp_t", structTy);
    m->addTypeName("knh_sfp_t_ptr", sfpPtr);
    LLVMTYPE_sfp = sfpPtr;
    fields.clear();

    /* InputStream */
    /* System */
    /* SystemEX */
    fields.push_back(longTy);  // knh_uintptr_t sysid;
    fields.push_back(longTy);  // size_t     ctxcount;
    fields.push_back(voidPtr); // struct knh_DictMap_t*      props;
    fields.push_back(objectPtr/*TODO*/);  // struct knh_InputStream_t*  in;
    fields.push_back(objectPtr/*TODO*/);  // struct knh_OutputStream_t* out;
    fields.push_back(objectPtr/*TODO*/);  // struct knh_OutputStream_t* err;
    fields.push_back(objectPtr/*TODO*/);  // struct knh_String_t*       enc;
    fields.push_back(voidPtr); // struct knh_DictSet_t       *tokenDictSet;
    fields.push_back(voidPtr); // struct knh_DictSet_t   *nameDictCaseSet;
    fields.push_back(longTy);  // size_t                      namecapacity;
    fields.push_back(voidPtr); // knh_nameinfo_t             *nameinfo;
    fields.push_back(voidPtr); // struct knh_DictSet_t      *urnDictSet;
    fields.push_back(voidPtr); // struct knh_Array_t        *urns;
    fields.push_back(voidPtr); // struct knh_DictSet_t      *ClassNameDictSet;
    fields.push_back(voidPtr); // struct knh_DictSet_t  *EventDictCaseSet;
    fields.push_back(voidPtr); // struct knh_DictMap_t      *PackageDictMap;
    fields.push_back(voidPtr); // struct knh_DictMap_t      *URNAliasDictMap;
    fields.push_back(voidPtr); // struct knh_DictSet_t      *dspiDictSet;

    structTy = StructType::get(m->getContext(), fields, /*isPacked=*/false);
    fields.clear();
    fields.push_back(hObjectTy);
    fields.push_back(structTy);
    structTy = StructType::get(m->getContext(), fields, /*isPacked=*/false);
    const Type *sysPtr = PointerType::get(structTy, 0);
    m->addTypeName("System", sysPtr);
    fields.clear();

    /* ctx */
    /* 00 */fields.push_back(voidPtr);   /* share */
    /* 01 */fields.push_back(voidPtr);   /* stat */
    /* 02 */fields.push_back(voidPtr);   /* spi */
    /* 03 */fields.push_back(voidPtr);   //const struct knh_api2_t        *api2;
    /* 04 */fields.push_back(sysPtr);    /* sys */
    /* 05 */fields.push_back(voidPtr); /* script */
    /* stack */
    /* 06 */fields.push_back(sfpPtr); //knh_sfp_t*                   stack;
    /* 07 */fields.push_back(sfpPtr); //knh_sfp_t*                   esp;
    /* 08 */fields.push_back(longTy); //size_t                       stacksize;
    /* 09 */fields.push_back(sfpPtr); //knh_sfp_t*                   stacktop;
    /* 10 */fields.push_back(voidPtr); //void*                        cstack_bottom;
    /* 11 */fields.push_back(objectPtr); //struct knh_Exception_t      *e;
    /* 12 */fields.push_back(objectPtr); //struct knh_Monitor_t        *mon;
    /* memory */
    /* 13 */fields.push_back(objectPtr); //knh_Object_t                *freeObjectList;
    /* 14 */fields.push_back(objectPtr); //knh_Object_t                *freeObjectTail;
    /* 15 */fields.push_back(longTy); //size_t                       freeObjectListSize;
    /* 16 */fields.push_back(longTy); //knh_uintptr_t                mscheck;
    /* 17 */fields.push_back(voidPtr); //knh_fastmem_t               *freeMemoryList;
    /* 18 */fields.push_back(voidPtr); //knh_fastmem_t               *freeMemoryTail;

    /* cache */
    /* 19 */fields.push_back(voidPtr); //knh_mtdcache_t              *mtdcache;
    /* 20 */fields.push_back(voidPtr); //knh_tmrcache_t             *tmrcache;
    /* 21 */fields.push_back(voidPtr); //struct knh_Object_t        **refs;
    /* 22 */fields.push_back(longTy); //size_t                       ref_size;
    /* 23 */fields.push_back(voidPtr); //struct knh_Object_t        **ref_buf;
    /* 24 */fields.push_back(longTy); //size_t                       ref_capacity;
    /* 25 */fields.push_back(voidPtr); //struct knh_Object_t        **queue;
    /* 26 */fields.push_back(longTy); //size_t                       queue_capacity;
    /* 27 */fields.push_back(longTy); //size_t                       queue_log2;

    /* 28 */fields.push_back(objectPtr); //struct knh_String_t*         enc;
    /* 29 */fields.push_back(objectPtr); //struct knh_InputStream_t*    in;
    /* 20 */fields.push_back(objectPtr); //struct knh_OutputStream_t*   out;
    /* 31 */fields.push_back(objectPtr); //struct knh_OutputStream_t*   err;
    /* 32 */fields.push_back(objectPtr); //struct knh_Bytes_t*          bufa;
    /* 33 */fields.push_back(objectPtr); //struct knh_OutputStream_t*   bufw;
    /* 34 */fields.push_back(objectPtr); //struct knh_Gamma_t*          gma;
    /* 35 */fields.push_back(objectPtr); //struct knh_DictMap_t*        symbolDictMap;
    /* 36 */fields.push_back(objectPtr); //struct knh_Array_t*          constPools;

    fields.push_back(shortTy);   //knh_flag_t                   flag;
    fields.push_back(shortTy);   //knh_ushort_t                 ctxid;
    fields.push_back(voidPtr);   //struct knh_Context_t        *ctxobjNC;
    fields.push_back(voidPtr);   //struct knh_context_t        *parent;
    fields.push_back(voidPtr);   //knh_mutex_t                 *ctxlock;

    fields.push_back(voidPtr);   //const struct _knh_ExportsAPI_t *api;
    fields.push_back(voidPtr);   //char                            trace[16];
    fields.push_back(voidPtr);   //knh_uint_t                      seq;
    // add here for new entry
    fields.push_back(voidPtr);   //struct knh_ExceptionHandler_t  *ehdrNC;
    fields.push_back(voidPtr);   //struct knh_Object_t            *evaled;


    structTy = StructType::get(m->getContext(), fields, /*isPacked=*/false);
    const Type *ctxPtr = PointerType::get(structTy, 0);
    m->addTypeName("struct.context", structTy);
    m->addTypeName("ctx", ctxPtr);
    LLVMTYPE_context = ctxPtr;
    fields.clear();

    // Function Type Definitions
    FunctionType* callTy;
    std::vector<const Type*>args;
    args.push_back(ctxPtr);
    args.push_back(sfpPtr);
    args.push_back(longTy);
    callTy = FunctionType::get(/*Result=*/voidTy,/*Params=*/args,/*isVarArg=*/false);
    LLVMTYPE_fcall = PointerType::get(callTy, 0);
    m->addTypeName("fcall", callTy);
    args.clear();

    /* checkin */
    args.push_back(ctxPtr);
    args.push_back(sfpPtr);
    args.push_back(objectPtr);
    callTy = FunctionType::get(/*Result=*/voidTy,/*Params=*/args,/*isVarArg=*/false);
    LLVMTYPE_checkin = PointerType::get(callTy, 0);
    m->addTypeName("checkin", callTy);
    args.clear();

    /* checkout */
    args.push_back(ctxPtr);
    args.push_back(objectPtr);
    args.push_back(longTy);
    FunctionType* callTy2 = FunctionType::get(/*Result=*/voidTy,/*Params=*/args,/*isVarArg=*/false);
    LLVMTYPE_checkout = PointerType::get(callTy2, 0);
    m->addTypeName("checkout", callTy2);

    /* Method */
    fields.push_back(hObjectTy);
    fields.push_back(voidPtr);
    fields.push_back(shortTy);
    fields.push_back(shortTy);
    fields.push_back(LLVMTYPE_fcall);
    structTy = StructType::get(m->getContext(), fields, /*isPacked=*/false);
    LLVMTYPE_Method = PointerType::get(structTy, 0);
    m->addTypeName("Method", LLVMTYPE_Method);
    fields.clear();

    /* Array */
    fields.push_back(hObjectTy);
    fields.push_back(voidPtr);
    fields.push_back(longTy); /* size */
    fields.push_back(voidPtr);/* dim */
    fields.push_back(voidPtr);/* api */
    structTy = StructType::get(LLVM_CONTEXT(), fields, false);
    LLVMTYPE_Array = PointerType::get(structTy, 0);
    m->addTypeName("Array", LLVMTYPE_Array);
    fields.clear();

    /* fitr */
    {
        args.clear();
        args.push_back(ctxPtr);
        args.push_back(sfpPtr);
        args.push_back(longTy);
        FunctionType* callTy = FunctionType::get(longTy, args, false);
        LLVMTYPE_itr = PointerType::get(callTy, 0);
        m->addTypeName("fitr", callTy);
    }

    /* Iterator */
    fields.push_back(hObjectTy);
    fields.push_back(voidPtr);
    fields.push_back(LLVMTYPE_itr);
    structTy = StructType::get(m->getContext(), fields, /*isPacked=*/false);
    LLVMTYPE_Iterator = PointerType::get(structTy, 0);
    m->addTypeName("Iterator", LLVMTYPE_Iterator);
    fields.clear();

    /* Array */
    fields.push_back(voidPtr); /* codeaddr */
    fields.push_back(longTy); /* codeaddr */
    fields.push_back(longTy); /* opcode/line */
    fields.push_back(longTy);/* d[0] */
    fields.push_back(longTy);/* d[1] */
    fields.push_back(longTy);/* d[2] */
    fields.push_back(longTy);/* d[3] */
    fields.push_back(longTy);/* d[4] */
    structTy = StructType::get(LLVM_CONTEXT(), fields, false);
    LLVMTYPE_opline = PointerType::get(structTy, 0);
    m->addTypeName("opline", LLVMTYPE_opline);
    fields.clear();


}


#ifdef __cplusplus
}
#endif
#endif
