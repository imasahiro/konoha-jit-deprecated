#define PJIT
#define TJIT_DUMP

#if defined(PJIT)
#define USE_STEXT       1
#define USE_cwb_open    1
#define USE_cwb_tobytes 1
//#define USE_cwb_size    1
#define USE_NOPTABLE
#endif

#include"commons.h"
#if defined(PJIT)
#if defined(K_USING_DEBUG) || 0
#define PCODE_DUMP
#endif

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

void knh_BasicBlock_add_(Ctx *ctx, knh_BasicBlock_t *bb, int line, knh_opline_t *op);
/* ----------------------------------------------------------------- */
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
#define SFP  PTYPE_SFP
#define FUNC PTYPE_FUNC
#define IDX  PTYPE_IDX
#define SELF PTYPE_SELF
#define FLAT PTYPE_FLAT

#define RSFP r14
#define RCTX r15

#define PJIT_RELOCATION_SYMBOL 0x0abadc0de /* a bad code! must rewrite it. */
#define PJIT_RELOCATION_SYMBOL_FUNC 0x1abadc0de /* a bad code! must rewrite it. */
#define PJIT_RELOCATION_SYMBOL_SELF 0x2abadc0de /* a bad code! must rewrite it. */

static void pcode_dump(pcode_t *op);
static void pjit_addRelocationCode(struct pjit *pjit, size_t size, int type, knh_uintptr_t target);
#define _dump(op)  if(pjit->state == PJIT_EMIT) { pcode_dump(op); }
//#define pcode_setline(op, pjit) if(pjit->state == PJIT_INIT) { op->pos = pjit->cur_pos; } fprintf(stderr, "op[%x] ", op->pos); pcode_dump(op);
#define pcode_setline(op, pjit) if(pjit->state == PJIT_INIT) { op->pos = pjit->cur_pos; }

static void pasm_init(struct pjit *pjit, pcode_t *op)
{
    pcode_setline(op, pjit);
    push(pjit, rbp);
    mov (pjit, rsp, rbp);
    mov2bp(pjit, r12, 0xe0, rbp);
    mov2bp(pjit, r13, 0xe8, rbp);
    mov2bp(pjit, r14, 0xf0, rbp);
    mov2bp(pjit, r15, 0xf8, rbp);
    subi(pjit, 0x40, rsp);
    mov(pjit, rsi, RSFP);
    mov(pjit, rdi, RCTX);
    _dump(op);
}
static void pasm_exit(struct pjit *pjit, pcode_t *op)
{
    pcode_setline(op, pjit);
    movbp1(pjit, 0xe0, rbp, r12);
    movbp1(pjit, 0xe8, rbp, r13);
    movbp1(pjit, 0xf0, rbp, r14);
    movbp1(pjit, 0xf8, rbp, r15);
    leave(pjit);
    ret(pjit);
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

static void pasm_load_r(struct pjit *pjit, pcode_t *op)
{
    reg_t r = op->a.reg;
    knh_sfpidx_t b = op->b.index;
    pcode_setline(op, pjit);
    int shift = b * 0x10;
    if (op->b.type == OBJ) {
        shift += 0x08;
    }
    mov1(pjit, shift, RSFP, r);
    _dump(op);
}

static void pasm_load_n(struct pjit *pjit, pcode_t *op)
{
    reg_t r = op->a.reg;
    knh_int_t data = op->b.data;
    pcode_setline(op, pjit);
    if (data == 0) {
        xor_(pjit, r, r);
    }
    else {
        movi(pjit, r, data);
    }
    _dump(op);
}

static void pasm_store_r(struct pjit *pjit, pcode_t *op)
{
    knh_sfpidx_t a = op->a.index;
    reg_t r = op->b.reg;
    pcode_setline(op, pjit);
    int shift = a * 0x10;
    if (op->a.type == OBJ) {
        shift += 0x08;
    }
    mov2(pjit, r, shift, RSFP);
    _dump(op);
}

static void pasm_store_n(struct pjit *pjit, pcode_t *op)
{
    knh_sfpidx_t a = op->a.index;
    knh_int_t data = op->b.data;
    int shift = a * 0x10;
    if (op->b.type == OBJP) {
        shift += 0x08;
    }
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
    knh_int_t data = op->b.data;
    pcode_setline(op, pjit);
    mov1(pjit, data, r, PREG0);
    _dump(op);
}

static void pasm_setidx(struct pjit *pjit, pcode_t *op)
{
    reg_t r = op->a.reg;
    knh_int_t data = op->b.data;
    pcode_setline(op, pjit);
    mov2(pjit, r, data, PREG0);
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
    TODO();
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
    movi(pjit, PREG0, func);
    if (flag == PJIT_RELOCATION_SYMBOL_FUNC && pjit->state == PJIT_EMIT) {
        pjit_addRelocationCode(pjit, sizeof(void*), FUNC, func);
    }
    else if (flag == PJIT_RELOCATION_SYMBOL_SELF && pjit->state == PJIT_EMIT) {
        pjit_addRelocationCode(pjit, sizeof(void*), SELF, 0);
    }
    call_r(pjit);
    _dump(op);
}
static void pasm_incref(struct pjit *pjit, pcode_t *op)
{
    reg_t r = op->a.reg;
    pcode_setline(op, pjit);
    if (op->b.type == OBJ) {
        asm volatile("int3");
    } else {
    }
    inc2(pjit, r, 0x10);
    _dump(op);
}
static void pasm_decref(struct pjit *pjit, pcode_t *op)
{
    reg_t r = op->a.reg;
    pcode_setline(op, pjit);
    if (op->b.type == OBJ) {
        knh_int_t b = op->b.data;
        int shift = b * 0x10 + 0x08;
        mov1(pjit, shift, RSFP, r);
    } else {
        asm volatile("int3");
    }
    dec2(pjit, r, 0x10);
    _dump(op);
}
static void pasm_inc(struct pjit *pjit, pcode_t *op)
{
    reg_t r = op->a.reg;
    pcode_setline(op, pjit);
    inc(pjit, r);
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
    mov(pjit, r2, r1);
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
    addi(pjit, data, r1);
    _dump(op);
}
static void pasm_add_rr(struct pjit *pjit, pcode_t *op)
{
    reg_t r1 = op->a.reg;
    reg_t r2 = op->b.reg;
    pcode_setline(op, pjit);
    addr(pjit, r1, r2);
    _dump(op);
}
static void pasm_sub_rn(struct pjit *pjit, pcode_t *op)
{
    reg_t r1 = op->a.reg;
    knh_int_t data = op->b.data;
    pcode_setline(op, pjit);
    subi(pjit, data, r1);
    _dump(op);
}
static void pasm_sub_rr(struct pjit *pjit, pcode_t *op)
{
    pcode_setline(op, pjit);
    TODO();
    _dump(op);
}
static void pasm_mul_rn(struct pjit *pjit, pcode_t *op)
{
    //reg_t r1 = op->a.reg;
    //knh_int_t data = op->b.data;
    pcode_setline(op, pjit);
    //subi(pjit, data, r1);
    TODO();
    _dump(op);
}
static void pasm_mul_rr(struct pjit *pjit, pcode_t *op)
{
    pcode_setline(op, pjit);
    TODO();
    _dump(op);
}
static void pasm_store_f(struct pjit *pjit, pcode_t *op)
{
    reg_t r1 = op->a.reg;
    knh_int_t data = op->b.data;
    pcode_setline(op, pjit);
    fmov2(pjit, PFREG0, data, r1);
    _dump(op);
}
static void pasm_load_fn(struct pjit *pjit, pcode_t *op)
{
    reg_t r1 = op->a.reg;
    knh_int_t data = op->b.data;
    pcode_setline(op, pjit);
    movi(pjit, PREG0, data);
    if (r1 == rsp) {
        mov2sp(pjit, PREG0, 0x00, r1);
    } else {
        TODO();
    }
    fmov1(pjit, 0x00, r1, PFREG0);
    _dump(op);
}
static void pasm_load_fr(struct pjit *pjit, pcode_t *op)
{
    reg_t r1 = op->a.reg;
    knh_int_t idx = op->b.data;
    pcode_setline(op, pjit);
    fmov1(pjit, idx, r1, PFREG0);
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
    asm_fn(pjit, op, fadd1);
    _dump(op);
}
static void pasm_sub_fn(struct pjit *pjit, pcode_t *op)
{
    pcode_setline(op, pjit);
    asm_fn(pjit, op, fsub1);
    _dump(op);
}
static void pasm_mul_fn(struct pjit *pjit, pcode_t *op)
{
    pcode_setline(op, pjit);
    asm_fn(pjit, op, fmul1);
    _dump(op);
}
static void pasm_div_fn(struct pjit *pjit, pcode_t *op)
{
    pcode_setline(op, pjit);
    asm_fn(pjit, op, fdiv1);
    _dump(op);
}
static void pasm_fcast_r(struct pjit *pjit, pcode_t *op)
{
    reg_t r1 = op->a.reg;
    pcode_setline(op, pjit);
    fcast(pjit, r1, PFREG0);
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
#define reg(r)  {REG,  {(void*)(0ULL + r)}}
#define obj(m)  {OBJ,  {(void*)(0ULL + m)}}
#define ndat(m) {NDAT, {(void*)(0ULL + m)}}
#define data(n) {DAT,  {(void*)(0ULL + n)}}
#define objp(p) {OBJP, {(void*)p}}
#define bb(b,i) {BB,   {(b)}}
#define cond(c) {COND, {(void*)(0ULL + c)}}
#define func(f) {FUNC, {(void*)(f)}}
#define idx(i)  {IDX,  {(void*)(0ULL + i)}}
#define flat(f) {FLAT, {(void*)(0ULL + (knh_ndata_t)f)}}

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
            fprintf(stderr, "0x%llx ", p[i].data);
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
static void pcode_expand(Ctx *ctx, knh_BasicBlock_t *bb, size_t newsize)
{
    pcode_t *newbuf = (pcode_t*)KNH_MALLOC(ctx, sizeof(pcode_t) * newsize);
    pdata_t *pdata = (pdata_t*) DP(bb)->code;
    knh_memcpy(newbuf, pdata->opbuf, pdata->capacity * sizeof(pcode_t));
    KNH_FREE(ctx, pdata->opbuf, pdata->capacity * sizeof(pcode_t));
    pdata->opbuf    = newbuf;
    pdata->capacity = newsize;
}

static void pcode_add_(Ctx *ctx, knh_BasicBlock_t *bb, pcode_t *op)
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

static void pdata_delete(Ctx *ctx, pdata_t *pdata)
{
    KNH_FREE(ctx, pdata->opbuf, sizeof(pcode_t) * pdata->capacity);
    pdata->opbuf = (pcode_t*)0xdeadbeaf;
    KNH_FREE(ctx, pdata, sizeof(pdata_t));
}

static pdata_t *pdata_new(Ctx *ctx)
{
    pdata_t *pdata = KNH_MALLOC(ctx, sizeof(pdata_t));
    pdata->opbuf = NULL;
    pdata->size = 0;
    pdata->capacity = 0;
    return pdata;
}

static knh_BasicBlock_t* new_BasicBlock(Ctx *ctx, knh_Array_t *list)
{
    knh_BasicBlock_t *bb = new_(BasicBlock);
    SP(bb)->listNC = list;
    DP(bb)->id = knh_Array_size(list);
    knh_Array_add(ctx, list, bb);
    DP(bb)->code = (knh_opline_t *) pdata_new(ctx);
    return bb;
}

static knh_bool_t knh_opset_isHeadOfBasicBlock(knh_opline_t *codeblock, void *cur)
{
    knh_opline_t *pc = codeblock;
    while(pc->opcode != OPCODE_RET) {
        if(knh_opcode_hasjump(pc->opcode)) {
            if(pc->p[0] == cur) {
                DBG_P("%s jumppc=%p, cur=%p", knh_opcode_tochar(pc->opcode), pc->p[0], cur);
                return 1;
            }
        }
        pc++;
    }
    return 0;
}

static void BasicBlock_setOriginal(Ctx *ctx, knh_BasicBlock_t *bb, knh_opline_t *op)
{
    knh_BasicBlock_add_(ctx, bb, 0, op);
    knh_opline_t *op_ = DP(bb)->opbuf + DP(bb)->size - 1;
    op_->codeaddr = op;
}

static void init_regtable(void);
#define regtable_size(_table) ((size_t)_table[-1].data)
static pindex_t *createRegisterTable(Ctx *ctx, int size)
{
    int size_ = size + 1;
    init_regtable();
    pindex_t *table = KNH_MALLOC(ctx, sizeof(pindex_t) * size_);
    int i;
    table[0].data = size;
    for (i = 1; i < size_; i++) {
        table[i].type = NOP;
        table[i].reg = PREG_NOP;
    }
    return (table + 1);
}

static void deleteRegisterTable(Ctx *ctx, pindex_t *regTable)
{
    int size = regtable_size(regTable) + 1;
    pindex_t *table = (regTable - 1);
    KNH_FREE(ctx, table, size * sizeof(pindex_t));
}

#define COUNT_MAX(op, i) {\
    int j;\
    for (j = 0; j < knh_opcode_size(op->opcode); j++) {\
        if (knh_opcode_usedef(op->opcode, j)) {\
            _max = KNH_MAX(_max, (int)op->data[j]);\
        }\
    }\
}

static knh_Array_t *KLRCode_toBasicBlock(Ctx *ctx, knh_KLRCode_t *kcode, int *max)
{
    knh_opline_t *op, *opHEAD = SP(kcode)->code;
    int i = 0, _max = 0;
    knh_Array_t * list = new_Array(ctx, CLASS_BasicBlock, 0);
    knh_BasicBlock_t* bb_, *bb = new_BasicBlock(ctx, list);

    for (op = opHEAD + 1; op->opcode != OPCODE_RET; op++) {
        bb = (knh_BasicBlock_t*) knh_Array_n(list, i);
        COUNT_MAX(op, i);
        if (knh_opset_isHeadOfBasicBlock(opHEAD, op)) {
            bb_ = new_BasicBlock(ctx, list);
            i = DP(bb_)->id;
            //fprintf(stderr, "new bb(%p,%d)\n", bb_, i);
            KNH_INITv(SP(bb)->nextNC, bb_);
            BasicBlock_setOriginal(ctx, bb_, op);
            bb = bb_;
        }
        else {
            BasicBlock_setOriginal(ctx, bb, op);
        }
        //fprintf(stderr, "opcode=%6s\n", knh_opcode_tochar(op->opcode));
    }
    bb_ = new_BasicBlock(ctx, list);
    BasicBlock_setOriginal(ctx, bb_, op);
    KNH_INITv(SP(bb)->nextNC, bb_);
    //knh_BasicBlock_add_(ctx, bb, 0, op); /* add ret op */
    //for (i = 0; i < knh_Array_size(list); i++) {
    //    dumpBB(knh_Array_n(list, i));
    //}
    *max = _max;
    return list;
}

//static void register_allocation(kvc_t *kvc)
//static void expire_old_intervals(Ctx *ctx, knh_Array_t *interval, knh_Array_t *active, int i)
//static void *spill_at_interval(kvc_t *kvc, knh_Array_t *interval, knh_Array_t *active, int i)

static void pcode_add_(Ctx *ctx, knh_BasicBlock_t *bb, pcode_t *op);
#define PASM_(opcode, a, b) { \
    pcode_t __op = {opcode, 0, a, b};\
    pcode_add_(ctx, bb, (&__op));\
}

#define PASM(T, a, b) { \
    pcode_t __op = {PCODE_##T, 0, a, b};\
    pcode_add_(ctx, bb, (&__op));\
}


#define OPCASE(op) case OPCODE_##op : 
//static void table_tochar(pindex_t *t)
//{
//    int type = t->type;
//    switch (type) {
//    case REG:
//        fprintf(stderr, "r%d", (t->reg));
//        break;
//    case OBJ:
//    case NDAT:
//    case SFP:
//        fprintf(stderr, "%4ld",(t->index));
//        break;
//    case NOP:
//        fprintf(stderr, "nop");
//    }
//}
struct regtable {
    reg_t r;
    int type;
    knh_sfpidx_t use;
};

#define REG_UNUSE -1
static struct regtable reg_table[] = {
    {-1, NOP, REG_UNUSE},
    {-1, NOP, REG_UNUSE},
    {-1, NOP, REG_UNUSE},
    {-1, NOP, REG_UNUSE},
    {-1, NOP, REG_UNUSE},
    {-1, NOP, REG_UNUSE},
    {-1, NOP, REG_UNUSE},
};

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static void init_regtable(void)
{
    reg_table[0].r = PREG0;
    reg_table[1].r = PREG1;
    reg_table[2].r = PREG2;
    reg_table[3].r = PREG3;
    reg_table[4].r = PREG4;
    reg_table[5].r = PREG5;
    reg_table[6].r = PREG6;
    reg_table[7].r = PREG7;
}

static reg_t regalloc(pindex_t *regTable, int type, knh_sfpidx_t a)
{
    int i;
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
    //int size = regtable_size(regTable);
    //fprintf(stderr, "regtable: ");
    //for (i = 0; i < size; i++) {
    //    table_tochar(&regTable[i]);
    //    fprintf(stderr, ", ");
    //}
    //fprintf(stderr, "\n");
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
    int i;
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

static void store_reg_ndat(Ctx *ctx, knh_BasicBlock_t *bb, pindex_t *regTable, int i)
{
    reg_t r  = reg_table[i].r;
    PASM(STORER, ndat(reg_table[i].use), reg(r));
    //int size = regtable_size(regTable);
}
static void store_reg_obj(Ctx *ctx, knh_BasicBlock_t *bb, pindex_t *regTable, int i)
{
    reg_t r  = reg_table[i].r;
    knh_sfpidx_t use = reg_table[i].use;
    knh_sfpidx_t source = search_source(bb, r);
    if (source == use)
        return;
    PASM(INCREF, reg(r), nop());
    PASM(DECREF, reg(PREG0), obj(reg_table[i].use));
    PASM(STORER, obj(reg_table[i].use), reg(r));
    /* obj(reg_table[i].use) is loaded to PREG0 
     * by DECREF op. */
    /* TODO */
    PASM(MOVRR , reg(PARG1), reg(PREG0));
    PASM(GETIDX, reg(PREG0), data(0x10));
    PASM(CMPN, reg(PREG0), data(0));
    PASM(COND, idx(2), cond(JNE));
    /* TODO write fast path Object_free */ 
    PASM(CALL , func(knh_Object_free), nop());
    PASM(NOP , nop(), nop());
}
static void store_reg_objp(Ctx *ctx, knh_BasicBlock_t *bb, pindex_t *regTable, int i)
{
    asm volatile("int3");
}
static void store_reg_(Ctx *ctx, knh_BasicBlock_t *bb, pindex_t *regTable, int i)
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
#define FUNCEND  1
static void store_regs(Ctx *ctx, knh_BasicBlock_t *bb, pindex_t *regTable, int end)
{
    int i;
    for (i = 1; i < ARRAY_SIZE(reg_table); i++) {
        int type = reg_table[i].type;
        knh_sfpidx_t use = reg_table[i].use;
        if (use != REG_UNUSE && type == NDAT) {
            if (end == BLOCKEND) {
                store_reg_ndat(ctx, bb, regTable, i);
            }
            CLEAR_REG(reg_table[i]);
        }
    }
    for (i = 1; i < ARRAY_SIZE(reg_table); i++) {
        int type = reg_table[i].type;
        knh_sfpidx_t use = reg_table[i].use;
        if (use != REG_UNUSE && type == OBJ) {
            store_reg_obj(ctx, bb, regTable, i);
            CLEAR_REG(reg_table[i]);
        }
    }
    for (i = 1; i < ARRAY_SIZE(reg_table); i++) {
        int type = reg_table[i].type;
        knh_sfpidx_t use = reg_table[i].use;
        if (use != REG_UNUSE && type == OBJP) {
            store_reg_objp(ctx, bb, regTable, i);
            CLEAR_REG(reg_table[i]);
        }
    }
}

static knh_BasicBlock_t *knh_opline_getTargetBB(knh_Array_t *bbList, knh_opline_t *target)
{
    int i , j;
    knh_BasicBlock_t* bb;
    for (i = 0; i < knh_Array_size(bbList); i++) {
        knh_opline_t *op;
        bb = (knh_BasicBlock_t*) knh_Array_n(bbList, i);
        for (j = 0; j < DP(bb)->size; j++) {
            op = DP(bb)->opbuf + j;
            if (op->codeaddr == target) {
                return bb;
            }
        }
    }
    KNH_ASSERT(1);
    return NULL;
}
static void reg_store_(Ctx *ctx, knh_BasicBlock_t *bb, pindex_t *regTable, knh_sfpidx_t a)
{
    int i;
    for (i = 0; i < ARRAY_SIZE(reg_table); i++) {
        if (reg_table[i].use == a)
            store_reg_(ctx, bb, regTable, i);
    }

}
static int reg_isLoaded(pindex_t *regTable, knh_sfpidx_t a)
{
    int i;
    for (i = 0; i < ARRAY_SIZE(reg_table); i++) {
        if (reg_table[i].use == a)
            return 1;
    }
    return 0;
}

static void BasicBlock_setPcode(Ctx *ctx, knh_Method_t *mtd, knh_Array_t *bbList, pindex_t *regTable)
{
    int i , j;
    knh_BasicBlock_t* bb = (knh_BasicBlock_t*) knh_Array_n(bbList, 0);
    PASM(INIT, nop(), nop());
    for (i = 0; i < knh_Array_size(bbList); i++) {
        knh_opline_t *op;
        bb = (knh_BasicBlock_t*) knh_Array_n(bbList, i);
        for (j = 0; j < DP(bb)->size; j++) {
            op = DP(bb)->opbuf + j;
#ifdef PCODE_DUMP
            fprintf(stderr, "bb=%d ", DP(bb)->id);
            knh_opcode_dump(ctx, op, KNH_STDERR, NULL);
#endif
            switch (op->opcode) {
            OPCASE(NSET) {
                klr_NSET_t *op_ = (klr_NSET_t*) op;
                reg_t r = regalloc(regTable, NDAT, op_->a);
                knh_int_t n = op_->n;
                if (r == PREG_NOP) {
                    TODO();
                    PASM(LOADN, ndat(r), data(n));
                    asm volatile("int3");
                } else {
                    PASM(LOADN, reg(r), data(n));
                }
                break;
            }
            OPCASE(OSET) {
                klr_OSET_t *op_ = (klr_OSET_t*) op;
                reg_t r = regalloc(regTable, OBJ, op_->a);
                knh_Object_t *o = op_->o;
                if (r == PREG_NOP) {
                    TODO();
                }
                PASM(LOADN, reg(r), objp(o));
                //PASM(INCREF, reg(r), nop());
                break;
            }
            OPCASE(iJEQn)OPCASE(iJNEQn)OPCASE(iJLTn)OPCASE(iJLTEn)OPCASE(iJGTn)OPCASE(iJGTEn) {
                klr_iJLTn_t *op_ = (klr_iJLTn_t*) op;
                int isLoaded = reg_isLoaded(regTable, op_->a);
                int opcode = op_->opcode - OPCODE_iJEQn;
                static int condop[] = {JNE, JE, JGE, JG, JLE, JE};
                reg_t r = regalloc(regTable, NDAT, op_->a);
                knh_int_t n = op_->n;
                knh_BasicBlock_t *targetBB = knh_opline_getTargetBB(bbList, op_->jumppc);
                if (!isLoaded) {
                    if (r == PREG_NOP) {
                        TODO();
                    }
                    PASM(LOADR, reg(r), ndat(op_->a));
                }
                if (n >= 0x80000000) { // n is too big to compare directly.
                    TODO();
                    PASM(LOADR, reg(PREG0), data(n));
                    PASM(CMPR , reg(r), reg(PREG0));
                } else {
                    PASM(CMPN, reg(r), data(n));
                }
                if (!isLoaded) {
                    clear_reg(r);
                }
                //fprintf(stderr, "targetBB bb=(%p,id:%d)\n", targetBB, DP(targetBB)->id);
                store_regs(ctx, bb, regTable, BLOCKEND);
                PASM(COND, bb(targetBB,0), cond(condop[opcode]));
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
                klr_JMP_t *op_ = (klr_JMP_t*) op;
                knh_BasicBlock_t *targetBB = knh_opline_getTargetBB(bbList, op_->jumppc);
                store_regs(ctx, bb, regTable, BLOCKEND);
                //fprintf(stderr, "targetBB bb=%d\n", DP(targetBB)->id);
                PASM(JMP, bb(targetBB,0), nop());
                break;
            }
            OPCASE(NMOV) {
                klr_NMOV_t *op_ = (klr_NMOV_t*) op;
                reg_t r, r2;
                if (reg_isLoaded(regTable, op_->b)) {
                    r2 = regalloc(regTable, NDAT, op_->b);
                    r  = regalloc(regTable, NDAT, op_->a);
                    if (r2 == PREG_NOP) {
                        TODO();
                    }
                    PASM(MOVRR, reg(r), reg(r2));
                } else {
                    r  = regalloc(regTable, NDAT, op_->a);
                    PASM(LOADR, reg(r), ndat(op_->b));
                }
                break;
            }
            OPCASE(SWAP) {
                klr_SWAP_t *op_ = (klr_SWAP_t*) op;
                reg_t r, r2;
                if (reg_isLoaded(regTable, op_->b)) {
                    r2 = regalloc(regTable, OBJ, op_->b);
                    r  = regalloc(regTable, OBJ, op_->a);
                    if (r2 == PREG_NOP) {
                        TODO();
                    }
                    PASM(MOVRR, reg(r), reg(r2));
                } else {
                    r  = regalloc(regTable, OBJ, op_->a);
                    PASM(LOADR , reg(r), obj(op_->b));
                }
                break;
            }

            OPCASE(OMOV) {
                klr_OMOV_t *op_ = (klr_OMOV_t*) op;
                reg_t r, r2;
                if (reg_isLoaded(regTable, op_->b)) {
                    r2 = regalloc(regTable, OBJ, op_->b);
                    r  = regalloc(regTable, OBJ, op_->a);
                    if (r2 == PREG_NOP) {
                        TODO();
                    }
                    PASM(MOVRR, reg(r), reg(r2));
                } else {
                    r  = regalloc(regTable, OBJ, op_->a);
                    PASM(LOADR , reg(r), obj(op_->b));
                    //PASM(INCREF, reg(r), nop());
                }
                break;
            }
            OPCASE(JMP_) OPCASE(RET) {
                store_regs(ctx, bb, regTable, FUNCEND);
                PASM(EXIT, nop(), nop());
                //fprintf(stderr, "bb bb=(%p,id:%d)\n", bb, DP(bb)->id);
                break;
            }
            OPCASE(VCALL) OPCASE(VCALL_) {
                klr_VCALL_t *op_ = (klr_VCALL_t*) op;
                knh_sfpidx_t thisidx  = op_->thisidx;
                //knh_sfpidx_t espshift = op_->espshift;
                knh_Method_t *callmtd = op_->callmtd;
                store_regs(ctx, bb, regTable, BLOCKEND);
                PASM(MOVRR, reg(PARG1), reg(RSFP));
                PASM(ADDRN, reg(PARG1), data(thisidx * 0x10));
                PASM(LOADN, reg(PARG2), data(K_RTNIDX));
                if (mtd == callmtd) {
                    PASM(CALL , func(NULL), data(PJIT_RELOCATION_SYMBOL_SELF));
                } else if (callmtd->fcall_1 == knh_Fmethod_runVM) {
                    /* TODO klr_setesp */
                    PASM(STOREN, obj(thisidx + K_MTDIDX), data(callmtd));
                    PASM(CALL , func(callmtd->fcall_1), nop());
                } else {
                    PASM(CALL , func(callmtd->fcall_1), nop());
                }
                break;
            }
            OPCASE(SCALL) {
                klr_SCALL_t *op_ = (klr_SCALL_t*) op;
                knh_sfpidx_t thisidx  = op_->thisidx;
                //knh_sfpidx_t espshift = op_->espshift;
                knh_Method_t *callmtd = op_->callmtd;
                store_regs(ctx, bb, regTable, BLOCKEND);
                PASM(MOVRR, reg(PARG1), reg(RSFP));
                PASM(ADDRN, reg(PARG1), data(thisidx * 0x10));
                PASM(LOADN, reg(PARG2), data(K_RTNIDX));
                /* TODO klr_setesp */
                /* TODO sfp_[K_MTDIDX].callmtd = mtd_ */
                PASM(CALL , func(callmtd->fcall_1), nop());
                break;
            }
            OPCASE(TR) {
                klr_TR_t *op_ = (klr_TR_t*) op;
                knh_sfpidx_t diff = op_->a - op_->b;
                store_regs(ctx, bb, regTable, BLOCKEND);
                PASM(MOVRR, reg(PARG1), reg(RSFP));
                PASM(ADDRN, reg(PARG1), data(op_->a * 0x10));
                PASM(LOADN, reg(PARG2), data(diff));
                PASM(LOADN, reg(PARG3), data(op_->cid));
                PASM(CALL , func(op_->tr), nop());
                break;
            }
            OPCASE(iMOVx) OPCASE(fMOVx) {
                klr_iMOVx_t *op_ = (klr_iMOVx_t*) op;
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
                assert(r != PREG0);
                //if (r2 != PREG0) {
                //    PASM(MOVRR, reg(PREG0), reg(r2));
                //}
                PASM(GETIDX, reg(r2), data(0x20));  /* PREG0 = sfp[b.i].ox->fields */
                PASM(GETIDX, reg(PREG0), data(pos));/* PREG0 = sfp[b.i].ox->fields[a] */
                PASM(MOVRR , reg(r), reg(PREG0));
                break;
            }
            OPCASE(XIMOV) OPCASE(XFMOV) {
                klr_XIMOV_t *op_ = (klr_XIMOV_t*) op;
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
                PASM(GETIDX, reg(r), data(0x20));  /* sfp[b.i].ox->fields */
                PASM(SETIDX, reg(r2), data(pos));/* sfp[b.i].ox->fields[a] */
                break;
            }
            OPCASE(iADD)OPCASE(iSUB)OPCASE(iMUL) {
                klr_iADD_t *op_ = (klr_iADD_t*) op;
                reg_t r, r2, r3;
                if (reg_isLoaded(regTable, op_->b)) {
                    r3 = regalloc(regTable, NDAT, op_->b);
                    r2 = regalloc(regTable, NDAT, op_->a);
                    r  = regalloc(regTable, NDAT, op_->c);
                    if (r3 == PREG_NOP) {
                        TODO();
                    }
                    if (r2 == PREG_NOP) {
                        TODO();
                    }
                    PASM(MOVRR, reg(r), reg(r2));
                } else {
                    r  = regalloc(regTable, NDAT, op_->c);
                    r3 =  PREG0;
                    PASM(LOADR, reg(r), ndat(op_->a));
                    PASM(LOADR, reg(r3), ndat(op_->b));
                }
                if (op_->opcode == OPCODE_iADD) {
                    PASM(ADDRR, reg(r3), reg(r));
                } else if (op_->opcode == OPCODE_iSUB) {
                    PASM(SUBRR, reg(r3), reg(r));
                } else if (op_->opcode == OPCODE_iMUL) {
                    PASM(MULRR, reg(r3), reg(r));
                }
                break;
            }
            OPCASE(iADDn)OPCASE(iSUBn) {
                klr_iADDn_t *op_ = (klr_iADDn_t*) op;
                reg_t r, r2;
                if (reg_isLoaded(regTable, op_->a)) {
                    // c = a + n
                    r2 = regalloc(regTable, NDAT, op_->a);
                    r  = regalloc(regTable, NDAT, op_->c);
                    if (r2 == PREG_NOP) {
                        TODO();
                    }
                    PASM(MOVRR, reg(r), reg(r2));
                } else {
                    r  = regalloc(regTable, NDAT, op_->c);
                    PASM(LOADR, reg(r), ndat(op_->a));
                }
                if (op_->opcode == OPCODE_iADDn) {
                    PASM(ADDRN, reg(r), data(op_->n));
                } else if (op_->opcode == OPCODE_iSUBn) {
                    PASM(SUBRN, reg(r), data(op_->n));
                } else if (op_->opcode == OPCODE_iMULn) {
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
                PASM(LOADFR, reg(RSFP), data(op_->a * 0x10));
                int opcode = op_->opcode - OPCODE_fADD + PCODE_ADDF;
                PASM_(opcode, reg(RSFP), data(op_->b * 0x10));
                // store 0x00(%r) = %ftmp0
                PASM(STOREF, reg(RSFP), data(op_->c * 0x10));
                break;
            }
            OPCASE(fADDn) OPCASE(fSUBn) OPCASE(fMULn) OPCASE(fDIVn) {
                klr_fADDn_t *op_ = (klr_fADDn_t*) op;
                ndata_t v;
                v.f = op_->n;
                int opcode = op_->opcode - OPCODE_fADDn + PCODE_ADDFN;
                PASM(LOADFR, reg(RSFP), data(op_->a * 0x10));
                PASM_(opcode, reg(rsp), flat(v.i));
                // store 0x00(%r) = %ftmp0
                PASM(STOREF, reg(RSFP), data(op_->c * 0x10));
                break;
            }
            OPCASE(fCAST){
                klr_fCAST_t *op_ = (klr_fCAST_t*) op;
                reg_t r;
                if (!reg_isLoaded(regTable, op_->b)) {
                    r = regalloc(regTable, NDAT, op_->b);
                    PASM(LOADR, reg(r), ndat(op_->b));
                } else {
                    r  = regalloc(regTable, NDAT, op_->a);
                }
                PASM(FCASTR, reg(r), nop());
                PASM(STOREF, reg(RSFP), data(op_->a * 0x10));
                clear_reg(r);
                break;
            }
            OPCASE(iCAST){
                klr_iCAST_t *op_ = (klr_iCAST_t*) op;
                reg_t r;
                if (reg_isLoaded(regTable, op_->b)) {
                    reg_store_(ctx, bb, regTable, op_->b);
                }
                r = regalloc(regTable, NDAT, op_->a);
                PASM(LOADFR, reg(RSFP), data(op_->b * 0x10));
                PASM(ICASTR, reg(r), nop());
                break;
            }


            OPCASE(HALT) OPCASE(THCODE) {
                iprintf("JIT compile error at %s(%d) at %d",
                        knh_opcode_tochar(op->opcode),
                        op->opcode, op->line);
                asm volatile("int3");
                break;

            }
            default :
                iprintf("not surpport yet %s(%d)",
                        knh_opcode_tochar(op->opcode), op->opcode);
                asm volatile("int3");
                break;
            //case OPCODE_:
            //    break;
            }
        }
        store_regs(ctx, bb, regTable, BLOCKEND);
        //PASM(HALT, nop(), nop());
    }
}

static void pcode_optimize(Ctx *ctx, knh_Array_t *bbList, pindex_t *regTable)
{
#if 1
    int i , j;
    pcode_t *op, *opP;
    knh_BasicBlock_t* bb;
    for (i = 0; i < knh_Array_size(bbList); i++) {
        bb = (knh_BasicBlock_t*) knh_Array_n(bbList, i);
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
            if (opP->opcode == PCODE_ADDRN) {
                if (opP->a.data == 0) {
                    opP->opcode = PCODE_NOP;
                }
            }
            opP = op;
        }
    }
#endif
}

static struct pjit *new_pjit(Ctx *ctx, knh_cwb_t *cwb)
{
    struct pjit *pjit = (struct pjit*)KNH_MALLOC(ctx, sizeof(struct pjit));
    pjit->state = PJIT_INIT;
    pjit->cwb   = cwb;
    pjit->ctx   = ctx;
    pjit->cur_pos = 0;
    pjit->symbolList = new_Array(ctx, CLASS_Int, 0);
    return pjit;
}

static void pjit_delete(Ctx *ctx, struct pjit *pjit)
{
    KNH_FINALv(ctx, pjit->symbolList);
    KNH_FREE(ctx, pjit, sizeof(struct pjit));
}
static void IArray_add(Ctx *ctx, knh_Array_t *a, knh_int_t v)
{
    BEGIN_LOCAL(ctx, lsfp, 1);
    lsfp[0].ivalue = v;
    a->api->add(ctx, a, lsfp);
    END_LOCAL(ctx, lsfp);
}
static void pjit_addRelocationCode(struct pjit *pjit, size_t size, int type, knh_uintptr_t target)
{
    Ctx *ctx = pjit->ctx;
    knh_Array_t *symbolList = pjit->symbolList;
    IArray_add(ctx, symbolList, type);
    IArray_add(ctx, symbolList, target);
    IArray_add(ctx, symbolList, size);
    IArray_add(ctx, symbolList, pjit->cur_pos);
}
static void pjit_rewrite_symbols(struct pjit *pjit, unsigned char *mem)
{
#define IArray_get(a, i)             ((a)->ilist[i])
    knh_Array_t *symbolList = pjit->symbolList;
    int i;
    for (i = 0; i < knh_Array_size(symbolList); i+=4) {
        knh_int_t type = IArray_get(symbolList, i + 0);
        knh_int_t sym  = IArray_get(symbolList, i + 1);
        knh_int_t size = IArray_get(symbolList, i + 2);
        knh_int_t pos  = IArray_get(symbolList, i + 3);
        //fprintf(stderr, "type=%lld\n", type);
        //fprintf(stderr, "sym =%lld\n", sym);
        //fprintf(stderr, "size=%lld\n", size);
        //fprintf(stderr, "pos =%lld\n", pos);
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
static void *_gencode(Ctx *ctx, struct pjit *pjit)
{
    knh_bytes_t codes = knh_cwb_tobytes(pjit->cwb);
    unsigned char *mem;
    int len, align = (int) ((uint16_t)(0x10 - codes.len % 0x10));
    len = codes.len + align;
    mem = (unsigned char *) knh_xmalloc(ctx, len);
    memcpy(mem, codes.ubuf, len);
    add_nops((void*)((intptr_t)mem + len), align);
    pjit_rewrite_symbols(pjit, mem);
    pjit_delete(ctx, pjit);
#ifdef PCODE_DUMP
    jit_dump(mem, len);
#endif
    //knh_xfree(ctx, mem, len);
    //return NULL;
    return mem;
}

static void pcode_gen1(struct pjit *pjit, pdata_t *pdata, knh_BasicBlock_t *bb)
{
    int j;
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
#else
        (void)bb;
#endif
        EMIT(pjit, op);
    }
}
static void *pcode_gencode(Ctx *ctx, knh_Array_t *bbList, pindex_t *regTable)
{
    int i;
    knh_BasicBlock_t *bb;
    knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
    struct pjit *pjit = new_pjit(ctx, cwb);
    for (i = 0; i < knh_Array_size(bbList); i++) {
        bb = (knh_BasicBlock_t*) knh_Array_n(bbList, i);
        pdata_t *pdata = (pdata_t*) DP(bb)->code;
        pcode_gen1(pjit, pdata, bb);
    }
    pjit->cur_pos = 0;
    pjit->state = PJIT_EMIT;
    for (i = 0; i < knh_Array_size(bbList); i++) {
        bb = (knh_BasicBlock_t*) knh_Array_n(bbList, i);
        pdata_t *pdata = (pdata_t*) DP(bb)->code;
        pcode_gen1(pjit, pdata, bb);
    }
    pjit->state = PJIT_EXIT;
    for (i = 0; i < knh_Array_size(bbList); i++) {
        bb = (knh_BasicBlock_t*) knh_Array_n(bbList, i);
        pdata_t *pdata = (pdata_t*) DP(bb)->code;
        if (SP(bb)->nextNC) 
            KNH_FINALv(ctx, (bb)->nextNC);
        pdata_delete(ctx, pdata);
    }
    return _gencode(ctx, pjit);
}

void pjit_compile(Ctx *ctx, knh_Method_t *mtd)
{
    if(!knh_Method_isObjectCode(mtd)) {
        return;
    }
    else {
        void *func;
        int max;
        BEGIN_LOCAL(ctx, lsfp, 1);
        knh_KLRCode_t *kcode = DP(mtd)->kcode;
        LOCAL_NEW(ctx, lsfp, 0, knh_Array_t *, bbList, KLRCode_toBasicBlock(ctx, kcode, &max));
        pindex_t *regTable = createRegisterTable(ctx, max);
        BasicBlock_setPcode(ctx, mtd, bbList, regTable);
        pcode_optimize(ctx, bbList, regTable);
        func = pcode_gencode(ctx, bbList, regTable);
        if (func)
            knh_Method_setFunc(ctx, mtd, (knh_Fmethod)func);
        deleteRegisterTable(ctx, regTable);
#ifdef PCODE_DUMP
        asm volatile("int3");
#endif
        END_LOCAL(ctx, lsfp);
    }
}
//static void checkrefc(knh_Object_t *o, knh_Object_t *o2, knh_Object_t *o3, knh_Object_t *o4)
//{
//    fprintf(stderr, "(o1=%p,refc=%ld,", o , o->h.refc);
//    if (o->h.cid == CLASS_String) {
//        fprintf(stderr, "'%s')\n", ((knh_String_t*)o)->str.text);
//    } else {
//        fprintf(stderr, ")\n");
//    }
//    fprintf(stderr, "(o2=%p,refc=%ld,", o2, o2->h.refc);
//    if (o2->h.cid == CLASS_String) {
//        fprintf(stderr, "'%s')\n", ((knh_String_t*)o2)->str.text);
//    } else {
//        fprintf(stderr, ")\n");
//    }
//
//    fprintf(stderr, "(o3=%p,refc=%ld,", o3, o3->h.refc);
//    if (o3->h.cid == CLASS_String) {
//        fprintf(stderr, "'%s')\n", ((knh_String_t*)o3)->str.text);
//    } else {
//        fprintf(stderr, ")\n");
//    }
//
//    fprintf(stderr, "(o4=%p,refc=%ld,", o4, o4->h.refc);
//    if (o4->h.cid == CLASS_String) {
//        fprintf(stderr, "'%s')\n", ((knh_String_t*)o4)->str.text);
//    } else {
//        fprintf(stderr, ")\n");
//    }
//
//}
#endif
