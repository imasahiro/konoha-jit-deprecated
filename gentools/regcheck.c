/*
 * regcheck.c
 * Copyright (c) 2010       Masahiro Ide <imasahiro9@gmail.com>
 *
 * Checking what register and dispach method Konoha Virtual Machine use.
 * how to use;
 * $ gcc regcheck.c -o regcheck -lkonoha 
 * $ ./regcheck
 */


#include <konoha.h>
#include <konoha/konoha_code_.h>
#if !defined(KONOHA_ON_LINUX) && (defined(__i386__) || !defined(__x86_64__))
#error please compile with x86_64
#endif

#if defined(K_USING_THREADEDCODE)
#define TADDR   NULL, 0/*counter*/
#else
#define TADDR   0/*counter*/
#endif/*K_USING_THREADEDCODE*/

#define ASMLINE  0

#define ARRAY_SIZE(x) (int)(sizeof(x) / sizeof((x)[0]))
#define REGION_SIZE   0x40
#ifdef DEBUG
#define rcheck(a, b) {\
    const char *name = "   ";\
    if (a >= 0) {\
        name=curname[a];\
        curregs[a] = b;\
    }\
    fprintf(stdout, "%-3s  %-3s = env[%02u] = %p\n", name, b, i, reg);\
}
#else
#define rcheck(a, b) curregs[a] = b;
#endif

static void code_dump(knh_uchar_t *mem, int size)
{
#if defined(DEBUG) && 0
    int i = 0;
    knh_uchar_t *code = mem;
    fprintf(stdout, "code start=%p\n", code);
    for (i = 0; i < size; i++) {
        fprintf(stdout, " %02x", code[i]);
        if (i%16 == 15) {
            fprintf(stdout, "\n");
        }
    }
    if (i%16) 
        fprintf(stdout, "\n");
    fprintf(stdout, "code   end=%p\n", code + i);
    fprintf(stdout, "\n");
#endif
}

static Ctx *gctx = NULL;
static knh_sfp_t *gsfp = NULL;
static knh_opline_t *gpc = NULL;

static void check_dispatch(void *address)
{
    static knh_uchar_t jmp_r[] = {0xff ,0xe0};
    static knh_uchar_t jmpcodes[2][5] = {
        {0xeb, 0x00, 0x00, 0x00, 0x00}, // Jump short JMP rel8
        {0xe9, 0x00, 0x00, 0x00, 0x00}, // Jump near  JMP rel16/rel32
    };
    knh_uchar_t *code = (knh_uchar_t *) address;

    int i;
    char *msg = "invalid jump code";
    for (i = 0; i < REGION_SIZE; i++) {
        if (code[i] == jmp_r[0] && code[i+1] == jmp_r[1]) {
            msg = "detect direct threading jump code";
            break;
        }
        int j;
        for (j = 0; j < ARRAY_SIZE(jmpcodes); j++) {
            knh_uchar_t *jcode = jmpcodes[j];
            if (code[0] == jcode[0]) {
                msg = "detect switch-case jump code";
                break;
            }
        }
    }
    fprintf(stdout, "checking dispach: \n\t%s\n", msg);
    code_dump(code, REGION_SIZE);
}

void check_register(Ctx *ctx, knh_opline_t *pc, void **codeaddr)
{
    jmp_buf env;
    const char *curname[] = {"ctx", "sfp", "pc"};
    const char *curregs[] = {NULL, NULL, NULL};
    int i;
    static const char *regs[] = {
#if defined(KONOHA_ON_LINUX)
#if defined(__i386__) 
        "ebx", "esi", "edi", "ebp", "esp", "pc",
#elif  defined(__x86_64__)
        "rbx", "rbp", "r12", "r13", "r14", "r15", "rsp", "pc", 
#endif
#elif defined(KONOHA_ON_MACOSX)
        //"ebx", "esi", "edi", "ebx", "???", "???", "edi", "esi", "ebp", "esp",
        "rbx", "rbp", "rsp", "r12", "r13", "r14", "r15",
#endif
    };
    setjmp(env);
#if defined(DEBUG2)
    asm volatile("int3");
    for (i = 0; i < 10; i++) {
        void *reg = ((void**)env)[i];
        fprintf(stderr, "env[%2d] = %p\n", i , reg);
    }
    asm volatile("int3");
#endif
    for (i = 0; i < ARRAY_SIZE(regs); i++) {
        void *reg = ((void**)env)[i];
        if (reg == gctx) {
            rcheck(0, regs[i]);
        }
        else if (reg == gsfp) {
            rcheck(1, regs[i]);
        }
        else if (reg == gpc) {
            rcheck(2, regs[i]);
        }
#ifdef DEBUG
        else 
            rcheck(-1, regs[i]);
#endif
    }

    fprintf(stdout, "checking register: \n");
    for (i = 0; i < 3; i++) {
        fprintf(stdout, "\t%-3s = %s\n", curname[i], curregs[i]);
    }
    check_dispatch(codeaddr[OPCODE_iADD]);
}

void save_data(Ctx **pctx, knh_sfp_t **psfp, knh_opline_t **ppc)
{
    gctx = *pctx;
    gsfp = *psfp;
    gpc  = *ppc;
#ifdef DEBUG
    fprintf(stdout, "ctx = %p\n", *pctx);
    fprintf(stdout, "sfp = %p\n", *psfp);
    fprintf(stdout, "pc  = %p\n", *ppc);
#endif
    asm volatile("" :: "r"(pctx), "r"(psfp), "r"(ppc));
}

int main(int argc, char **argv)
{
    konoha_t konoha = konoha_open(4096);
    Ctx *ctx = KONOHA_BEGIN(konoha.ctx);
    BEGIN_LOCAL(ctx, sfp, 5);
    klr_THCODE_t op[] = {
        {TADDR, OPCODE_THCODE, ASMLINE, check_register},
        {TADDR, OPCODE_RET   , ASMLINE, NULL}
    };
    knh_opline_t *pc = (knh_opline_t *) malloc(sizeof(knh_opline_t) * 2);

    memcpy(pc, op, sizeof(knh_opline_t) * 2);
    save_data(&ctx, &sfp, &pc);
    knh_VirtualMachine_run(ctx, sfp, pc);
    END_LOCAL(ctx, sfp);
    KONOHA_END(ctx);
    konoha_close(konoha);
    return 0;
}

