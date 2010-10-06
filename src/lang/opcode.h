#include "../../pcode_opcode.txt"

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

};

