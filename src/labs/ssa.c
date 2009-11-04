/**********************************************************************
 * Copyright (c) 2009, Masahiro Ide <masahiro at users.sourceforge.jp>
 * All rights reserved.
 *
 **********************************************************************/

/* ******************************************************************* */

#define KNH_DBGMODE2
#include"../kc/commons.h"

/* ******************************************************************* */

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------- */

//Array<SSAGraph>
// ssa[0]
//    label
//    use
//    def

void knh_SSAGraph_new(Ctx *ctx, knh_Array_t *insts)
{
	size_t i, inst_size = knh_Array_size(insts);
	int last_line = 0;
	int label = 0;
	knh_KLRInst_t *inst;
	knh_OutputStream_t *w = DP(ctx->sys)->out;

	// count opcode size
	for(i = 0; i < inst_size; i++) {
		inst = (knh_KLRInst_t*)knh_Array_n(insts, i);
		knh_inst_t *op = (inst->op);
		knh_printf(ctx, w, "[%p,%d] ", inst,label);
		if(inst->opcode == OPCODE_LABEL) {
			label++;
		}
		else {
			knh_opcode_dump(ctx,inst->op, w);
			if(knh_opcode_hasjump(inst->opcode)) {
				//(op)->jumppc = (op->a1)->code_pos;
				int jopcode = ((knh_inst_t*)(op->jumppc))->opcode;
				//DBG2_P("%p(%d) GOTO %p(%d)", op, op->opcode, op->jumppc, jopcode);
			}
		}
		if(inst->opcode == OPCODE_HALT) {
			knh_write_EOL(ctx, w);
			break;
		}
		knh_write_EOL(ctx, w);
	}
	knh_write_EOL(ctx, w);
}


/* ------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif
