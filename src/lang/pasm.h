#define iprintf(fmt, ...) \
    fprintf(stdout, "[%s:%d] ",__FILE__,__LINE__); \
    fprintf(stdout, fmt, ##__VA_ARGS__);\
    fprintf(stdout, "\n");

#define __put(c) {\
    pjit->cur_pos += 1;\
    if (pjit->state == PJIT_EMIT) {\
        knh_Bytes_putc(pjit->ctx, pjit->cwb->ba, c);\
    }\
}
#define __pos(pjit) knh_cwb_size(pjit->cwb)
#define __write(code, size) {\
    pjit->cur_pos += size;\
    if (pjit->state == PJIT_EMIT) {\
        knh_Bytes_write(pjit->ctx, pjit->cwb->ba,new_bytes2((char*)code, size));\
    }\
}

typedef uint16_t reg_t;
struct pjit {
    int state;
#define PJIT_INIT 0
#define PJIT_EMIT 1
#define PJIT_EXIT 2
    Ctx *ctx;
    knh_cwb_t *cwb;
    size_t cur_pos;
    knh_Array_t *symbolList;
};

typedef struct {
    int type;
    union {
        knh_BasicBlock_t* bb;
        knh_Object_t* o;
        knh_sfpidx_t  index;
        knh_ndata_t   data;
        reg_t         reg;
    };
} pindex_t;

typedef struct pcode_t {
    int opcode;
    int pos;
    pindex_t a;
    pindex_t b;
} pcode_t;

typedef struct pdata {
    pcode_t *opbuf;
    int size;
    int capacity;
} pdata_t;



static const reg_t rnon = {-1};
static const reg_t rax  = {0};
static const reg_t rcx  = {1};
static const reg_t rdx  = {2};
static const reg_t rbx  = {3};
static const reg_t rsp  = {4};
static const reg_t rbp  = {5};
static const reg_t rsi  = {6};
static const reg_t rdi  = {7};
static const reg_t r8   = {8};
static const reg_t r9   = {9};
static const reg_t r10  = {10};
static const reg_t r11  = {11};
static const reg_t r12  = {12};
static const reg_t r13  = {13};
static const reg_t r14  = {14};
static const reg_t r15  = {15};
static const reg_t xmmx0 = {0};
static const reg_t xmmx1 = {1};
static const reg_t xmmx2 = {2};
static const reg_t xmmx3 = {3};
static const reg_t xmmx4 = {4};

#define PREG_NOP rnon
#define PREG0    rax
#define PREG1    rcx
#define PREG2    r8 
#define PREG3    r9 
#define PREG4    r10
#define PREG5    r11
#define PREG6    r12
#define PREG7    r13
#define PARG0    rdi
#define PARG1    rsi
#define PARG2    rdx
#define PARG3    rcx
#define PARG4    r8
#define PFREG0   xmmx0
#define PFREG1   xmmx1

static int mov1(struct pjit *pjit, int n, reg_t r1, reg_t r2)
{
    unsigned char suffix = 0xff;
    int ret = 3;
    if (r1 < 8) {
        if (r2 < 8) {
            __put(0x48);
        } else if (r2 >= 8) {
            __put(0x49);
            r2 = r2 - 8;
        }
    } else {
        if (r1 == r12) suffix = 0x24;
        if (r2 < 8) {
            __put(0x49);
        } else if (r2 >= 8) {
            __put(0x4d);
            r2 = r2 - 8;
        }
        r1 = r1 - 8;
    }
    __put(0x8b);
    if (n < 0x80) {
        __put(0x40 | (r2 << 3) | (r1 << 0));
    } else {
        __put(0x80 | (r2 << 3) | (r1 << 0));
    }
    if(suffix != 0xff) {
        __put(0x24);
        ret = 4;
    }
    __put(n);
    return ret;
}

static int mov2(struct pjit *pjit, reg_t r1, int n, reg_t r2)
{
    unsigned char suffix = 0xff;
    int ret = 3, over80 = 0;
    union {
        unsigned char code[sizeof(int32_t)];
        int32_t ival;
    } code;

    if (r1 < 8) {
        if (r2 < 8) {
            __put(0x48);
        } else if (r2 >= 8) {
            __put(0x49);
            r2 = r2 - 8;
        }
    } else {
        if (r2 == r12) {
            suffix = 0x24;
            asm volatile("int3");
        }
        if (r2 < 8) {
            __put(0x4c);
        } else if (r2 >= 8) {
            __put(0x4d);
            r2 = r2 - 8;
        }
        r1 = r1 - 8;
    }
    __put(0x89);
    if (n < 0x80) {
        __put(0x40 | (r1 << 3) | (r2 << 0));
    } else {
        __put(0x80 | (r1 << 3) | (r2 << 0));
        over80 = 1;
    }
    if(suffix != 0xff) {
        __put(0x24);
        ret += 1;
    }
    if (over80) {
        ret += 4;
        code.ival = n;
        __write(code.code, sizeof(code));
    } else {
        __put(n);
    }
    return ret;
}

static int push(struct pjit *pjit, reg_t r1)
{
    if (r1 >= 8) {
        __put(0x41);
        r1 -= 0x08;
    }
    __put(0x50 | r1);
    return 2;
}

//static int pop(struct pjit *pjit, reg_t r1)
//{
//    if (r1 >= 8) {
//        __put(0x41);
//        r1 -= 0x08;
//    }
//    __put(0x50 | r1);
//    return 2;
//}

//movq    %r14,%rsi:0x4c 0x89 0xf6
//movq    %r15,%rdx:0x4c 0x89 0xfa
static int mov(struct pjit *pjit, reg_t r1, reg_t r2)
{
    if (r1 < 8) {
        if (r2 < 8) {
            __put(0x48);
        }
        else {
            __put(0x49);
            r2 = r2 - 8;
        }
    } else {
        if (r1 == r12) asm volatile("int3");
        if (r2 < 8) {
            __put(0x4c);
        }
        else {
            __put(0x4d);
            r2 = r2 - 8;
        }
        r1 = r1 - 8;
    }
    __put(0x89);
    __put(0xc0 | (r1 << 3) | (r2 << 0));
    return 3;
}
static int _add_sub(struct pjit *pjit, unsigned char opcode, reg_t r1, reg_t r2)
{
    unsigned char op = 0x48;
    if (r1 < 8) {
        if (r2 >= 8) {
            r2 = r2 - 8;
            op = 0x49;
        }
    }
    else {
        r1 = r1 - 8;
        if (r2 < 8) {
            op = 0x4c;
        } else {
            r2 = r2 - 8;
            op = 0x4d;
        }
    }
    __put(op);
    __put(opcode);
    __put((0xc0 | r1 << 3 | r2));
    return 3;
}

#define addr(pjit, r1, r2) _add_sub(pjit, 0x01, r1, r2)
#define subr(pjit, r1, r2) _add_sub(pjit, 0x29, r1, r2)

static int _addi_subi(struct pjit *pjit, unsigned char opcode1, unsigned char opcode2, knh_int_t data, reg_t r1)
{
    unsigned char op = 0x48;
    union {
        unsigned char code[sizeof(int32_t)];
        int32_t ival;
    } code;
    int ret = 2 + sizeof(code);
    int isRax = (r1 == rax);
    if (r1 >= 8) {
        op = 0x49;
        r1 = r1 - 8;
    }
    __put(op);
    if (isRax) {
        __put(opcode2);
    } else {
        __put(0x81);
        __put((opcode1 | r1));
        ret += 1;
    }
    code.ival = data;
    __write(code.code, sizeof(code));
    return ret;
}

#define addi(pjit, data, r1) _addi_subi(pjit, 0xc0, 0x05, data, r1)
#define subi(pjit, data, r1) _addi_subi(pjit, 0xe8, 0x2d, data, r1)

#define mul(r1, r2) {\
    reg_t _r1 = r1;\
    reg_t _r2 = r2;\
    unsigned char op = 0x48;\
    if (r1 < 8) {\
        if (_r2 >= 8) {\
            _r2 = _r2 - 8;\
            op = 0x4c;\
        }\
    }\
    else {\
        _r1 = _r1 - 8;\
        if (_r2 < 8) {\
            op = 0x49;\
        } else {\
            _r2 = _r2 - 8;\
            op = 0x4d;\
        }\
    }\
    f[i++] = op;\
    f[i++] = 0x0f;\
    f[i++] = 0xaf;\
    f[i++] = (0xc0 | _r2 << 3 | _r1);\
}
static int int_(struct pjit *pjit)
{
    __put(0xcc);
    //fprintf(stderr,"[%d]int:%x\n",__LINE__,(unsigned int)__pos(pjit));
    return 1;
}
static int nop_(struct pjit *pjit)
{
    __put(0x90);
    //fprintf(stderr,"[%d]nop:%x\n",__LINE__,(unsigned int)__pos(pjit));
    return 1;
}
static int leave(struct pjit *pjit)
{
    __put(0xc9);
    return 1;
}
static int ret(struct pjit *pjit)
{
    __put(0xc3);
    return 1;
}
#define epilog(f) {\
    /*pop(rbx);*/\
    /*addi(rsp, 0x10);*/\
    leave();\
    ret();\
}

static int call_r(struct pjit *pjit)
{
    __put(0xff);
    __put(0xd0);
    return 2;
}

#define call(f, func) {\
    union { \
        unsigned char code[sizeof(intptr_t)];\
        intptr_t ival;\
    } op;\
    op.ival = (intptr_t) func - (intptr_t) (f + i + 4);\
    __put(op.code[0]);\
    __put(op.code[1]);\
    __put(op.code[2]);\
    __put(op.code[3]);\
}

static int movi(struct pjit *pjit, reg_t r1, knh_int_t data)
{
    unsigned char op = 0x48;
    union {
        unsigned char code[sizeof(knh_int_t)];
        knh_int_t ival;
    } code;

    if (r1 >= 8) {
        r1 -= 0x08;
        op = 0x49;
    }
    __put(op);
    __put(0xb8 | r1);
    code.ival = data;
    __write(code.code, sizeof(code));
    return 0;
}
static int movi2(struct pjit *pjit, knh_int_t data, int n, reg_t r1)
{
    unsigned char op = 0x48;
    int suffix = 0x00;
    union {
        unsigned char code[sizeof(int32_t)];
        int32_t ival;
    } code;
    if (r1 >= 8) {
        op = 0x49;
        if (r1 == r12) suffix = 0x24;
        r1 = r1 - 8;
    }
    code.ival = data;
    __put(op);
    __put(0xc7);
    __put(0x40 | r1);
    __put(n);
    if (suffix) 
        __put(suffix);
    __write(code.code, sizeof(code));
    return 0;
}

static int mov2sp(struct pjit *pjit, reg_t r1, int n, reg_t r2)
{
    if (r1 < 8) {
        __put(0x48);
    } else {
        iprintf("not surpport yet");
        exit(1);
    }
    if (r2 != rsp) {
        iprintf("not surpport yet");
        exit(1);
    }
    __put(0x89);
    if (n == 0) {
        __put(0x04);
    } else {
        __put(0x44);
    }
    //if (r1 == rax) {
    //    __put(0x04);
    //    //__put((r1 << 3) | rbp);
    //} else {
    //    TODO();
    //}
    __put(0x24);
    if (n != 0) {
        __put(n);
    }
    return 0;
}
static int mov2bp(struct pjit *pjit, reg_t r1, int n, reg_t r2)
{
    if (r1 < 8) {
        iprintf("not surpport yet");
        exit(1);
    } else {
        __put(0x4c);
    }
    if (r2 != rbp) {
        iprintf("not surpport yet");
        exit(1);
    }
    __put(0x89);
    __put((r1 << 3) | rbp);
    __put(n);
    return 0;
}

static int movbp1(struct pjit *pjit, int n, reg_t r1, reg_t r2)
{
    if (r1 != rbp) {
        iprintf("not surpport yet");
        exit(1);
    }
    if (r2 < 8) {
        iprintf("not surpport yet");
        exit(1);
    } else {
        __put(0x4c);
    }
    __put(0x8b);
    __put((r2 << 3) | rbp);
    __put(n);
    return 0;
}

static int cmpn(struct pjit *pjit, reg_t r1, int n)
{
    unsigned char op;
    int isRax = 0;
    int ret = 1;
    union {
        unsigned char code[sizeof(int32_t)];
        int32_t ival;
    } code;
    if (r1 < 8) {
        op = 0x48;
        if (r1 == rax) {
            isRax = 1;
        }
    } else {
        op = 0x49;
        r1 = r1 - 8;
    }
    __put(op);
    if (isRax) {
        __put(0x3d);
        ret += 1;
    } else {
        __put(0x81);
        __put(0xf8 | r1);
        ret += 2;
    }
    code.ival = n;
    __write(code.code, sizeof(code));
    ret += sizeof(code);
    return ret;
}

#define lea(r1,r2, imm8) {\
    reg_t r1 = r1;\
    reg_t r2 = r2;\
    if (r1 < 8) {\
        if (r2 < 8) {\
            __put(0x48);\
        }\
        else {\
            iprintf("not surpport yet");\
            exit(1);\
        }\
    } else {\
        r1 = r1 - 8;\
        if (r2 < 8) {\
            __put(0x49);\
        }\
        else {\
            iprintf("not surpport yet");\
            exit(1);\
        }\
    }\
    __put(0x8d);\
    __put(0x40 | (r2 << 3) | (r1 << 0));\
    if (r1 == r12) {\
        __put(0x24);\
    }\
    __put(imm8);\
}

// 0x0f 0x8f 0x00 0x00 0x00 0x00
static int cond(struct pjit *pjit, int op, knh_BasicBlock_t *bb, pcode_t *target)
{
    union {
        unsigned char code[sizeof(int32_t)];
        int32_t diff;
    } code;
    int pos;
    if (bb) {
        pos = (((pdata_t*)DP(bb)->code)->opbuf)->pos;
    } else {
        pos = target->pos;
    }
    if (pos == 0 && pjit->state == PJIT_EMIT) {
        abort();
    }
    int diff = 2 + sizeof(code);
    code.diff = pos - (pjit->cur_pos + diff);
    //fprintf(stderr, "pos=%x, cur=%x, code.diff=%x %d\n", pos, (int)pjit->cur_pos, code.diff, shift);
    __put(0x0f);
    __put(0x10 + op);
    __write(code.code, sizeof(code));
    return 2 + sizeof(code);
}
static int jump(struct pjit *pjit, knh_BasicBlock_t *bb, pcode_t *target)
{
    union {
        unsigned char code[sizeof(int32_t)];
        int32_t diff;
    } code;
    int pos;
    if (bb) {
        pos = (((pdata_t*)DP(bb)->code)->opbuf)->pos;
    } else {
        pos = target->pos;
    }
    if (pos == 0 && pjit->state == PJIT_EMIT) {
        abort();
    }
    int diff = 1 + sizeof(code);
    code.diff = pos - (pjit->cur_pos + diff);
    //fprintf(stderr, "pos=%x, cur=%x, code.diff=%x(-%x) \n", pos, (int)pjit->cur_pos, code.diff, shift);
    __put(0xe9);
    __write(code.code, sizeof(code));
    return 1 + sizeof(code);
}
static int xor_(struct pjit *pjit, reg_t r1, reg_t r2)
{
    if (r1 < 8) {
        if (r2 < 8) {
            __put(0x48);
        } else if (r2 >= 8) {
            iprintf("not surpport yet");
            exit(1);
        }
    } else {
        if (r2 < 8) {
            iprintf("not surpport yet");
            exit(1);
        } else if (r2 >= 8) {
            __put(0x4d);
            r2 = r2 - 8;
        }
        r1 = r1 - 8;
    }
    __put(0x31);
    __put(0xc0 | (r1 << 3) | (r2 << 0));
    return 3;
}

static int _inc_dec(struct pjit *pjit, reg_t r1, int shift)
{
    unsigned char op;
    if (r1 < 8) {
        op = 0x48;
    } else {
        op = 0x49;
        r1 = r1 - 8;
    }
    __put(op);
    __put(0xff);
    __put(0xc0 | (r1 + shift));
    return 3;
}

#define inc(pjit, r1) _inc_dec(pjit, r1, 0)
#define dec(pjit, r1) _inc_dec(pjit, r1, 8)

//static int incr(struct pjit *pjit, reg_t r1, reg_t r2)
//{
//    unsigned char op;
//    int suffix = 0x04;
//    int ret = 4;
//    if (r1 < 8) {
//        fprintf(stderr, "error1\n");
//        exit(1);
//    } else {
//        op = 0x49;
//        if (r1 == r13) suffix = 0x44;
//        r1 = r1 - 8;
//        if (r2 != rax) {
//            fprintf(stderr, "error2\n");
//            exit(1);
//        }
//    }
//    __put(op);
//    __put(0xff);
//    __put(suffix);
//    __put(r1);
//    if (suffix == 0x44) {
//        __put(0x00);
//        ret += 1;
//    }
//    return ret;
//}

#define inc2(pjit, r1, offset) _inc2_dec2(pjit, 0x40, r1, offset)
#define dec2(pjit, r1, offset) _inc2_dec2(pjit, 0x48, r1, offset)
static int _inc2_dec2(struct pjit *pjit, unsigned char opcode, reg_t r1, int offset)
{
    unsigned char op = 0x48;
    int ret = 4, suffix = 0x00;
    if (r1 >= 8) {
        op = 0x49;
        if (r1 == r12) suffix = 0x24;
        r1 = r1 - 8;
    }
    __put(op);
    __put(0xff);
    __put(opcode | r1);
    if (suffix == 0x24) {
        __put(suffix);
        ret += 1;
    }
    __put(offset);
    return ret;
}
static int testq(struct pjit *pjit, reg_t r1, reg_t r2)
{
    unsigned char op = 0x48;
    if (r1 != r2) {
        iprintf("not surpport yet");
        exit(1);
    } else {
        if (r1 >= 8) {
            op = 0x4d;
            r2 = r2 - 8;
        }
    }
    __put(op);
    __put(0x85);
    __put(0xc0 | (r1 << 3) | (r2 << 0));
    return 3;
}
#define IS_FloatReg(r) (1)
// f2 0f 10 44 24 10
// f2 0f 10 84 24 80 00 00 00
static int fmov1(struct pjit *pjit, int32_t n, reg_t r1, reg_t r2)
{
    union {
        unsigned char code[sizeof(int32_t)];
        int32_t ival;
    } code;
    int ret = 4;
    int over80 = (n >= 0x80)? 1 : 0;
    assert(IS_FloatReg(r2));
    __put(0xf2);
    if (r1 >= 8) {
        __put(0x41);
        r1 = r1 - 8;
        ret += 1;
    }
    __put(0x0f);
    __put(0x10);
    if (r1 == rsp) {
        if (!over80) {
            __put((0x00 | (r2 << 3) | r1));
        } else {
            __put((0x80 | (r2 << 3) | r1));
        }
        ret += 1;
        __put(0x24);
    } else {
        __put((0x80 | (r2 << 3) | r1));
        code.ival = n;
        __write(code.code, sizeof(code));
        ret += 1 + sizeof(code);
    }
    return ret;
}
//static int fadd1(struct pjit *pjit, int32_t n, reg_t r1, reg_t r2)
//{
//    reg_t _r1 = r1;
//    union {
//        unsigned char code[sizeof(int32_t)];
//        int32_t ival;
//    } code;
//    int ret = 5 + sizeof(code);
//    assert(IS_FloatReg(r2));
//    __put(0xf2);
//    if (r1 >= 8) {
//        __put(0x41);
//        _r1 = _r1 - 8;
//        ret += 1;
//    }
//    __put(0x0f);
//    __put(0x58);
//    __put((0x80 | (r2 << 3) | _r1));
//    code.ival = n;
//    __write(code.code, sizeof(code));
//    return ret;
//}
static int fcalc1(struct pjit *pjit, unsigned char op, int32_t n, reg_t r1, reg_t r2)
{
    reg_t _r1 = r1;
    union {
        unsigned char code[sizeof(int32_t)];
        int32_t ival;
    } code;
    int ret = 5;
    assert(IS_FloatReg(r2));
    __put(0xf2);
    if (r1 >= 8) {
        __put(0x41);
        _r1 = _r1 - 8;
        ret += 1;
    }
    __put(0x0f);
    __put(op);
    if (n == 0) {
        __put((0x00 | (r2 << 3) | _r1));
        __put(0x24);
        // f2 0f 59 04 24
        ret += 1;
    } else {
        __put((0x80 | (r2 << 3) | _r1));
        code.ival = n;
        __write(code.code, sizeof(code));
        ret += sizeof(code);
    }
    return ret;
}
#define fadd1(pjit, n, r1, r2) fcalc1(pjit, 0x58, n, r1, r2)
#define fsub1(pjit, n, r1, r2) fcalc1(pjit, 0x5c, n, r1, r2)
#define fmul1(pjit, n, r1, r2) fcalc1(pjit, 0x59, n, r1, r2)
#define fdiv1(pjit, n, r1, r2) fcalc1(pjit, 0x5e, n, r1, r2)

static int fmov2(struct pjit *pjit, reg_t r1, int32_t n, reg_t r2)
{
    reg_t _r2 = r2;
    union {
        unsigned char code[sizeof(int32_t)];
        int32_t ival;
    } code;
    int ret = 5 + sizeof(code);
    assert(IS_FloatReg(r1));
    __put(0xf2);
    if (r2 >= 8) {
        __put(0x41);
        _r2 = _r2 - 8;
        ret += 1;
    }
    __put(0x0f);
    __put(0x11);
    __put((0x80 | (r1 << 3) | _r2));
    code.ival = n;
    __write(code.code, sizeof(code));
    return ret;
}

static int cast_(struct pjit *pjit, reg_t r1, reg_t r2, unsigned char opcode)
{
    unsigned char op = 0x48;
    int ret = 5;
    if (r1 >= 8) {
        op = 0x49;
        r1 = r1 - 8;
    }
    __put(0xf2);
    __put(op);
    __put(0x0f);
    __put(opcode);
    __put((0xc0 | (r2 << 3) | (r1)));
    return ret;
}

#define fcast(pjit, r1, r2) cast_(pjit, r1, r2, 0x2a)
#define icast(pjit, r1, r2) cast_(pjit, r1, r2, 0x2c)

#if 0
int main(int argc, char **argv)
{
#define CODEMAX 0x38
    unsigned char f[CODEMAX] = {0};
    int i = 0;
    push(rbp);
    mov(rsp, rbp);
    push(r15);
    push(r14);
    push(r13);
    push(r12);
    mov2(rdi,0xd8, rbp);
    mov2(rsi,0xd0, rbp);
    mov1(0xd8, rbp, rsi);
    mov1(0xd0, rbp, rdx);
    mov(rsi, r14);
    mov(rdx, r15);
    mov(r14, rsi);
    mov(r15, rdx);
    nop_();
    pop(r12);
    pop(r13);
    pop(r14);
    pop(r15);
    leave();
    ret();
    //mov2(rdx,0x08, r13);
    //mov2(r14,0x08, r15);
    //mov2(r14,0x08, rcx);
    //mov2(r8 ,0x08, rcx);
    //mov2(r2, 0x08, r2);
    jit_dump((void*)f, CODEMAX);
    return 0;
}
#endif
