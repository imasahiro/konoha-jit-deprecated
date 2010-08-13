#define USE_STEXT 1
#define USE_cwb_open      1
#define USE_cwb_tobytes 1
#define USE_cwb_size
#include"commons.h"

#define TJIT
//#define TJIT_DUMP

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

#if defined(TJIT)
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
static int canJIT[] = {
    0 /*HALT*/, 0 /*THCODE*/, 1 /*FUNCCALL*/, 1 /*ENTER*/,
    1 /*PROBE*/, 1 /*VEXEC*/, 0 /*YEILD*/, 0 /*EXIT*/,
    1 /*P*/, 1 /*OSET*/, 1 /*NSET*/, 1 /*OMOV*/,
    1 /*NMOV*/, 1 /*SWAP*/, 0 /*UNBOX*/, 0 /*ONMOV*/,
    0 /*OOMOV*/, 0 /*NNMOV*/, 0 /*OMOVx*/, 1 /*iMOVx*/,
    1 /*fMOVx*/, 1 /*bMOVx*/, 0 /*XMOV*/, 0 /*XMOVx*/,
    0 /*XOSET*/, 1 /*XIMOV*/, 0 /*XFMOV*/, 0 /*XBMOV*/,
    0 /*CHKSTACK*/, 0 /*LOADMTD*/, 0 /*CALL*/, 1 /*SCALL*/,
    1 /*VCALL*/, 1 /*VCALL_*/, 0 /*FASTCALL*/, 1 /*RET*/,
    0 /*SCAST*/, 0 /*TCAST*/, 0 /*ACAST*/, 0 /*iCAST*/,
    1 /*fCAST*/, 1 /*TR*/, 0 /*NUL*/, 1 /*JMP*/,
    0 /*JMP2*/, 1 /*JMP_*/, 0 /*JMPF*/, 1 /*DYJMP*/,
    0 /*NEXT*/, 0 /*TRY*/, 0 /*TRYEND*/, 0 /*THROW*/,
    0 /*CATCH*/, 0 /*bNOT*/, 1 /*iINC*/, 1 /*iDEC*/,
    1 /*iNEG*/, 1 /*iADD*/, 1 /*iSUB*/, 1 /*iMUL*/,
    1 /*iDIV*/, 1 /*iMOD*/, 1 /*iEQ*/, 1 /*iNEQ*/,
    1 /*iLT*/, 1 /*iLTE*/, 1 /*iGT*/, 1 /*iGTE*/,
    1 /*iADDn*/, 1 /*iSUBn*/, 1 /*iMULn*/, 1 /*iDIVn*/,
    1 /*iMODn*/, 1 /*iEQn*/, 1 /*iNEQn*/, 1 /*iLTn*/,
    1 /*iLTEn*/, 1 /*iGTn*/, 1 /*iGTEn*/, 1 /*fNEG*/,
    1 /*fADD*/, 1 /*fSUB*/, 1 /*fMUL*/, 1 /*fDIV*/,
    1 /*fEQ*/, 1 /*fNEQ*/, 1 /*fLT*/, 1 /*fLTE*/,
    1 /*fGT*/, 1 /*fGTE*/, 1 /*fADDn*/, 1 /*fSUBn*/,
    1 /*fMULn*/, 1 /*fDIVn*/, 1 /*fEQn*/, 1 /*fNEQn*/,
    1 /*fLTn*/, 1 /*fLTEn*/, 1 /*fGTn*/, 1 /*fGTEn*/,
    0 /*OGETIDX*/, 0 /*OSETIDX*/, 0 /*OGETIDXn*/, 0 /*OSETIDXn*/,
    0 /*NGETIDX*/, 0 /*NSETIDX*/, 0 /*NGETIDXn*/, 0 /*NSETIDXn*/,
    1 /*bJNOT*/, 1 /*iJEQ*/, 1 /*iJNEQ*/, 1 /*iJLT*/,
    1 /*iJLTE*/, 1 /*iJGT*/, 1 /*iJGTE*/, 1 /*iJEQn*/,
    1 /*iJNEQn*/, 1 /*iJLTn*/, 1 /*iJLTEn*/, 1 /*iJGTn*/,
    1 /*iJGTEn*/, 0 /*fJEQ*/, 0 /*fJNEQ*/, 0 /*fJLT*/,
    0 /*fJLTE*/, 0 /*fJGT*/, 0 /*fJGTE*/, 0 /*fJEQn*/,
    0 /*fJNEQn*/, 0 /*fJLTn*/, 0 /*fJLTEn*/, 0 /*fJGTn*/,
    0 /*fJGTEn*/, 0 /*NOP*/,
};


/* ----------------------------------------------------------------- */
static void jit_dump(void *mem, int size)
{
#if defined(TJIT_DUMP)
    int i = 0;
    unsigned char *code = (unsigned char*) mem;
    fprintf(stderr, "code start=%p\n", code);
    for (i = 0; i < size; i++) {
        fprintf(stderr, " %02x", code[i]);
        if (i%16 == 15) {
            fprintf(stderr, "\n");
        }
    }
    if (i%16) 
        fprintf(stderr, "\n");
    fprintf(stderr, "code   end=%p\n", code + i);
    fprintf(stderr, "\n");
#endif
}
/* ----------------------------------------------------------------- */
struct code_stack {
    unsigned char *code;
    unsigned char *code_addr;
    unsigned char *code_pos;
    unsigned char *target;
    unsigned char *jmp_pos;
    int target_size;
} cstack[64] = {};
int cindex = 0;

#define cstack_push(cstack, c, p, t, j, s) {\
    cstack[cindex].code_addr   = c; \
    cstack[cindex].code_pos    = p; \
    cstack[cindex].target      = t; \
    cstack[cindex].jmp_pos     = j; \
    cstack[cindex].target_size = s; \
    cindex++;\
}
#define cstack_pop(cstack, c, p, t, j, s) {\
    p = cstack[cindex].code_pos;    \
    t = cstack[cindex].target;      \
    j = cstack[cindex].jmp_pos;     \
    s = cstack[cindex].target_size; \
    cindex--;\
}
#define cstack_clear(data) {\
    data->code        = (unsigned char *) 0xffffffff;\
    data->code_addr   = (unsigned char *) NULL;  \
    data->code_pos    = (unsigned char *) NULL;  \
    data->target      = (unsigned char *) NULL;  \
    data->jmp_pos     = (unsigned char *) NULL;  \
    data->target_size = 0;     \
}

static knh_intptr_t get_target_template(unsigned char *code, unsigned char **jmp_target)
{
    static unsigned char jmp_r[] = {0xff,0xe0};
    static unsigned char jmp[]   = {0xe9,0x00,0x00,0x00,0x00};
    assert(sizeof(jmp_r) == 2);
    assert(sizeof(jmp) == 5);
    int i, size = -1;

    for (i = 0; i < 128; i++) {
        /* check jump instruction with register */
        if (code[i] == jmp_r[0] && code[i+1] == jmp_r[1]) {
            size = (i + sizeof(jmp_r));
            break;
        }

        /* check jump instruction */
        if (code[i] == jmp[0]) {
            int32_t pos = ((int32_t*) &code[i+1])[0];
            unsigned char *target = code + i + 5 + pos;
            DBG_(
                    jit_dump(&code[i], 8);
                    jit_dump(target, 16);
                );
            size = (i + sizeof(jmp));
            *jmp_target = target;
            break;
        }
    }
    return size;
}
/* ----------------------------------------------------------------- */
static int fix_code_template(unsigned char *code, knh_intptr_t *size)
{
    static unsigned char jmp_r[] = {0xff,0xe0};
    static unsigned char cond[]  = {0x0f,0x80, 0x00, 0x00, 0x00, 0x00};
    int i, need_rewrite = 0;
    assert(sizeof(jmp_r) == 2);
    assert(sizeof(cond) == 6);

    for (i = 0; i < 128; i++) {
        /* check jump instruction with register */
        if (code[i] == jmp_r[0] && code[i+1] == jmp_r[1]) {
            *size = i + sizeof(jmp_r);
            break;
        }
        /* check conditional branch instruction */
        if (code[i] == cond[0] && 
                (0x84 <= code[i+1] && code[i+1] <= 0x8d)) {
            /* 0x0f 0x87 0xe8 0x08 0x00 0x00 */
            /* 0xe8 0x08 0x00 0x00 => 0x000008e8 */
            knh_intptr_t tsize;
            unsigned char *cur_pos = &code[i+2];
            int32_t pos = ((int32_t*) cur_pos)[0];
            unsigned char *target = code + i + sizeof(cond) + pos;
            unsigned char *jmp_target = NULL;
            //jit_dump(&code[i], 8);
            //jit_dump(target, 16);
            tsize = get_target_template(target, &jmp_target);
            cstack_push(cstack, code, cur_pos, target, jmp_target, tsize);
            DBG_P("code=%p, cur_pos=%p diff=%p",
                    code, cur_pos, cur_pos - (intptr_t) code);
            need_rewrite = 1;
        }
    }
    return need_rewrite;
}

/* ----------------------------------------------------------------- */
static knh_intptr_t knh_jit_write(Ctx *ctx, knh_cwb_t *cwb, knh_opcode_t op, void **s, void **e)
{
    unsigned char *start = (unsigned char *) s[op];
    unsigned char *end   = (unsigned char *) e[op];
    int need_rewrite = 0;

    knh_intptr_t size = end - start;
    DBG_P("%s %p - %p, %d", knh_opcode_tochar(op),
            start, end, size);
    if (size < 0) {
        DBG_P("%s(%d)", knh_opcode_tochar(op), op);
        need_rewrite = fix_code_template(start, &size);
        if (size < 0) {
            abort();
        }
    }
    if (need_rewrite) {
        intptr_t len = knh_cwb_size(cwb);
        DBG_P("size=%x\n", (unsigned int) len);
        cstack[cindex-1].code = (unsigned char*) len;
    }
    knh_Bytes_write(ctx, cwb->ba, new_bytes2((char*)start, size));
    return size;
}

/* ----------------------------------------------------------------- */
static unsigned char *write_target_code(unsigned char *code, int len, unsigned char *tcode , int tsize)
{
    unsigned char *target = code + len;
    DBG_P("target=%p, code=%p, len=%d", target, code, len);
    memcpy(target, tcode, tsize);
    return target;
}
#define icast(a) (intptr_t)(a)
#define toaddr(a) (void *)(a)
#define intadd(a,b) (knh_intptr_t)(a) + (knh_intptr_t)(b)
static void rewrite_jmp_target(unsigned char *code, int *len, struct code_stack* cdata)
{
    int size = *len;
    intptr_t diff = icast(cdata->code_pos) - icast(cdata->code_addr);
    unsigned char *p = code + icast(cdata->code) + diff;
    unsigned char *target = write_target_code(code, size, cdata->target, cdata->target_size);
    intptr_t jmp = (intptr_t)target - (intptr_t)p - 4;
    ((int32_t*) p)[0] = jmp;
    //if (cdata->jmp_pos) 
    //    jit_dump(cdata->jmp_pos, 16);

    *len += cdata->target_size;

    DBG_(jit_dump(code, *len));

    /* clear used code info */
    cstack_clear(cdata);
}

#endif /* TJIT */

void knh_code_thread(Ctx *ctx, knh_opline_t *pc, void **codeaddr, void **codeend)
{
#if defined(TJIT)
#ifdef K_USING_THREADEDCODE
    knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
    knh_opline_t *op = pc;
    intptr_t curpos = 0;
L_TRUE:
    while(1) {
        DBG_P("%s %d %d\n",knh_opcode_tochar(pc->opcode)
                ,pc->opcode,canJIT[pc->opcode]);
        if (!canJIT[pc->opcode]) {
            /* we can not jit compile because current pc's template
               is broken */
            DBG_P("jmp to %s(%d)\n",knh_opcode_tochar(pc->opcode),pc->opcode);
            knh_intptr_t size = knh_jit_write(ctx, cwb, OPCODE_JMP2, codeaddr, codeend);
            curpos += size;
            goto L_FALSE;
        } else {
            knh_intptr_t size = knh_jit_write(ctx, cwb, pc->opcode, codeaddr, codeend);
            pc->codeaddr = toaddr(curpos);
            curpos += size;
        }
        if(pc->opcode == OPCODE_RET) break;
        pc++;
    }
    goto L_FINAL;
L_FALSE:
    /*pc = op;*/
    while(1) {
        DBG_ASSERT_OPCODE(pc->opcode);
        pc->codeaddr = codeaddr[pc->opcode];
        if(pc->opcode == OPCODE_RET) break;
        pc++;
        goto L_TRUE;
    }
L_FINAL:
    {
        knh_bytes_t codes = knh_cwb_tobytes(cwb);
        unsigned char *mem;
        int i, len, maxlen, align;
        len = maxlen = codes.len;
        for (i = 0; i < cindex; i++) 
            maxlen += cstack[i].target_size;
        align = (int) ((uint16_t)(0x8 - maxlen % 0x8));
        maxlen += align;
        mem = (unsigned char *) knh_xmalloc(ctx, maxlen);
        memcpy(mem, codes.ubuf, len);
        DBG_P("len=%d, maxlen=%d, mem=%p align=%d",
                len, maxlen, mem, align);

        /* reindexing codeaddr */
        pc = op;
        while(1) {
            if (canJIT[pc->opcode]) {
                void *pos = pc->codeaddr;
                pc->codeaddr = toaddr(intadd(mem, pc->codeaddr));
                int max = cindex;
                for (i = 0; i < max; i++) {
                    DBG_P("pos=%p, c[%d].p=%p size=%x",
                            pos, i, cstack[i].code,
                            cstack[i].target_size);
                    if (pos == cstack[i].code)
                        rewrite_jmp_target(mem, &len, &cstack[i]);
                }
            }
            if(pc->opcode == OPCODE_RET) break;
            pc++;
        }

        add_nops((void*)((intptr_t)mem + len), align);
        len += align;

        DBG_P("len=%d, maxlen=%d, mem=%p align=%d",
                len, maxlen, mem, align);
        KNH_ASSERT(len == maxlen);
        jit_dump(mem, len);
        /* reset code stack */
        cindex = 0;
        return;
    }
#endif
#else
#ifdef K_USING_THREADEDCODE
    while(1) {
        DBG_ASSERT_OPCODE(pc->opcode);
        pc->codeaddr = codeaddr[pc->opcode];
        if(pc->opcode == OPCODE_RET) break;
        pc++;
    }
#endif
#endif /* TJIT */
}

/* ----------------------------------------------------------------- */
#ifdef __cplusplus
}
#endif
