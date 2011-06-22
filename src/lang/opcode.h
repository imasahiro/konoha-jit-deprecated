//#include "../../pcode_opcode.txt"
#define PCODE_INIT (OPCODE_MAX + 0)
#define PCODE_EXIT (OPCODE_MAX + 1)
#define PCODE_NOP (OPCODE_MAX + 2)
#define PCODE_HALT (OPCODE_MAX + 3)
#define PCODE_LOADR (OPCODE_MAX + 4)
#define PCODE_LOADN (OPCODE_MAX + 5)
#define PCODE_STORER (OPCODE_MAX + 6)
#define PCODE_STOREN (OPCODE_MAX + 7)
#define PCODE_STOREM (OPCODE_MAX + 8)
#define PCODE_GETIDX (OPCODE_MAX + 9)
#define PCODE_SETIDX (OPCODE_MAX + 10)
#define PCODE_CMPN (OPCODE_MAX + 11)
#define PCODE_CMPM (OPCODE_MAX + 12)
#define PCODE_CMPR (OPCODE_MAX + 13)
#define PCODE_COND (OPCODE_MAX + 14)
#define PCODE_JMP (OPCODE_MAX + 15)
#define PCODE_CALL (OPCODE_MAX + 16)
#define PCODE_INCREF (OPCODE_MAX + 17)
#define PCODE_DECREF (OPCODE_MAX + 18)
#define PCODE_INC (OPCODE_MAX + 19)
#define PCODE_DEC (OPCODE_MAX + 20)
#define PCODE_MOVRR (OPCODE_MAX + 21)
#define PCODE_ADDMM (OPCODE_MAX + 22)
#define PCODE_ADDRM (OPCODE_MAX + 23)
#define PCODE_ADDRN (OPCODE_MAX + 24)
#define PCODE_ADDRR (OPCODE_MAX + 25)
#define PCODE_SUBRN (OPCODE_MAX + 26)
#define PCODE_SUBRR (OPCODE_MAX + 27)
#define PCODE_MULRN (OPCODE_MAX + 28)
#define PCODE_MULRR (OPCODE_MAX + 29)
#define PCODE_STOREF (OPCODE_MAX + 30)
#define PCODE_LOADFN (OPCODE_MAX + 31)
#define PCODE_LOADFR (OPCODE_MAX + 32)
#define PCODE_ADDF (OPCODE_MAX + 33)
#define PCODE_SUBF (OPCODE_MAX + 34)
#define PCODE_MULF (OPCODE_MAX + 35)
#define PCODE_DIVF (OPCODE_MAX + 36)
#define PCODE_ADDFN (OPCODE_MAX + 37)
#define PCODE_SUBFN (OPCODE_MAX + 38)
#define PCODE_MULFN (OPCODE_MAX + 39)
#define PCODE_DIVFN (OPCODE_MAX + 40)
#define PCODE_FCASTR (OPCODE_MAX + 41)
#define PCODE_ICASTR (OPCODE_MAX + 42)
struct pcode_data {
    int opcode;
    const char *name;
    int types[2];
    void (*emit)(struct pjit *pjit, pcode_t *op);
} PCODEDATA[] = {
    // init
    {PCODE_INIT, "init", {PTYPE_NOP, PTYPE_NOP}, pasm_init},
    // exit
    {PCODE_EXIT, "exit", {PTYPE_NOP, PTYPE_NOP}, pasm_exit},
    // nop
    {PCODE_NOP, "nop", {PTYPE_NOP, PTYPE_NOP}, pasm_nop},
    // halt
    {PCODE_HALT, "halt", {PTYPE_NOP, PTYPE_NOP}, pasm_halt},
    // load memory data to r1 
    {PCODE_LOADR, "load_r", {PTYPE_REG, PTYPE_SFP}, pasm_load_r},
    // load ndata to r1
    {PCODE_LOADN, "load_n", {PTYPE_REG, PTYPE_DATA}, pasm_load_n},
    // store data on register to memory
    {PCODE_STORER, "store_r", {PTYPE_SFP, PTYPE_REG}, pasm_store_r},
    // store ndata to memory
    {PCODE_STOREN, "store_n", {PTYPE_SFP, PTYPE_DATA}, pasm_store_n},
    // store memory to memory
    {PCODE_STOREM, "store_m", {PTYPE_SFP, PTYPE_SFP}, pasm_store_m},
    // load data
    {PCODE_GETIDX, "getidx", {PTYPE_REG, PTYPE_DATA}, pasm_getidx},
    // store data
    {PCODE_SETIDX, "setidx", {PTYPE_REG, PTYPE_DATA}, pasm_setidx},
    // compare r1 to ndata
    {PCODE_CMPN, "cmp_n", {PTYPE_REG, PTYPE_DATA}, pasm_cmp_n},
    // compare r1 to memory
    {PCODE_CMPM, "cmp_m", {PTYPE_REG, PTYPE_SFP}, pasm_cmp_m},
    // compare r1 to r2
    {PCODE_CMPR, "cmp_r", {PTYPE_REG, PTYPE_SFP}, pasm_cmp_r},
    // conditional jump(op=i1) to b1
    {PCODE_COND, "cond", {PTYPE_BB, PTYPE_COND}, pasm_cond},
    // jump to b1
    {PCODE_JMP, "jump", {PTYPE_BB, PTYPE_NOP}, pasm_jump},
    // call mtd1
    {PCODE_CALL, "call", {PTYPE_FUNC, PTYPE_NOP}, pasm_call},
    // increment r1's ref
    {PCODE_INCREF, "incref", {PTYPE_REG, PTYPE_OBJ}, pasm_incref},
    // decrement r1's ref and free if r1's ref == 0.
    {PCODE_DECREF, "decref", {PTYPE_REG, PTYPE_OBJ}, pasm_decref},
    // increment r1
    {PCODE_INC, "inc", {PTYPE_REG, PTYPE_NOP}, pasm_inc},
    // decrement r1
    {PCODE_DEC, "dec", {PTYPE_REG, PTYPE_NOP}, pasm_dec},
    // copy r2 to r1
    {PCODE_MOVRR, "mov_rr", {PTYPE_REG, PTYPE_REG}, pasm_mov_rr},
    // add memory data to memory data
    {PCODE_ADDMM, "add_mm", {PTYPE_NDAT, PTYPE_NDAT}, pasm_add_mm},
    // add memory data to r1
    {PCODE_ADDRM, "add_rm", {PTYPE_REG, PTYPE_NDAT}, pasm_add_rm},
    // add ndata to r1
    {PCODE_ADDRN, "add_rn", {PTYPE_REG, PTYPE_DATA}, pasm_add_rn},
    // add r2 to r1
    {PCODE_ADDRR, "add_rr", {PTYPE_REG, PTYPE_REG}, pasm_add_rr},
    // sub ndata to r1
    {PCODE_SUBRN, "sub_rn", {PTYPE_REG, PTYPE_DATA}, pasm_sub_rn},
    // sub r2 to r1
    {PCODE_SUBRR, "sub_rr", {PTYPE_REG, PTYPE_REG}, pasm_sub_rr},
    // mul ndata to r1
    {PCODE_MULRN, "mul_rn", {PTYPE_REG, PTYPE_DATA}, pasm_mul_rn},
    // mul r2 to r1
    {PCODE_MULRR, "mul_rr", {PTYPE_REG, PTYPE_REG}, pasm_mul_rr},
    // store float data on float tmp register to memory
    {PCODE_STOREF, "store_f", {PTYPE_SFP, PTYPE_DATA}, pasm_store_f},
    // load float data
    {PCODE_LOADFN, "load_fn", {PTYPE_REG, PTYPE_DATA}, pasm_load_fn},
    // load float data from r1
    {PCODE_LOADFR, "load_fr", {PTYPE_REG, PTYPE_DATA}, pasm_load_fr},
    // add memory data to float tmp register
    {PCODE_ADDF, "add_f", {PTYPE_REG, PTYPE_DATA}, pasm_add_f},
    {PCODE_SUBF, "sub_f", {PTYPE_REG, PTYPE_DATA}, pasm_sub_f},
    {PCODE_MULF, "mul_f", {PTYPE_REG, PTYPE_DATA}, pasm_mul_f},
    {PCODE_DIVF, "div_f", {PTYPE_REG, PTYPE_DATA}, pasm_div_f},

    // add memory data to float tmp register
    {PCODE_ADDFN, "add_fn", {PTYPE_REG, PTYPE_DATA}, pasm_add_fn},
    {PCODE_SUBFN, "sub_fn", {PTYPE_REG, PTYPE_DATA}, pasm_sub_fn},
    {PCODE_MULFN, "mul_fn", {PTYPE_REG, PTYPE_DATA}, pasm_mul_fn},
    {PCODE_DIVFN, "div_fn", {PTYPE_REG, PTYPE_DATA}, pasm_div_fn},

    // cast int data to float data
    {PCODE_FCASTR, "fcast_r", {PTYPE_REG, PTYPE_REG}, pasm_fcast_r},
    // cast float data to int data
    {PCODE_ICASTR, "icast_r", {PTYPE_REG, PTYPE_REG}, pasm_icast_r},
};

