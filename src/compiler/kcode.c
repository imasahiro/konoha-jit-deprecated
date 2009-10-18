/****************************************************************************
 * KONOHA COPYRIGHT, LICENSE NOTICE, AND DISCRIMER
 *
 * Copyright (c) 2005-2009, Kimio Kuramitsu <kimio at ynu.ac.jp>
 *           (c) 2008-      Konoha Software Foundation
 * All rights reserved.
 *
 * You may choose one of the following two licenses when you use konoha.
 * See www.konohaware.org/license.html for further information.
 *
 * (1) GNU Lesser General Public License 3.0 (with KONOHA_UNDER_LGPL3)
 * (2) Konoha Software Foundation License 1.0
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/* ************************************************************************ */

#include"commons.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ======================================================================== */
/* [kcode] */

static
void knh_ftraverse_inc(Ctx* ctx, Object *o)
{
	knh_Object_RCinc(o);
}

/* ------------------------------------------------------------------------ */

knh_KLRInst_t* new_KLRInst(Ctx *ctx, knh_inst_t *op)
{
	knh_KLRInst_t *inst
		= (knh_KLRInst_t*)new_Object_init(ctx, FLAG_KLRInst, CLASS_KLRInst, op->opcode);
	knh_memcpy(inst->op, op, knh_opcode_size(inst->opcode));
	knh_opcode_traverse(ctx, inst->op, knh_ftraverse_inc);
	return inst;
}

/* ------------------------------------------------------------------------ */

knh_KLRInst_t* knh_InstList_lastNULL(knh_Array_t *insts)
{
	size_t size = knh_Array_size(insts);
	if(size > 0) {
		return (knh_KLRInst_t*)knh_Array_n(insts, size-1);
	}
	return NULL;
}

/* ------------------------------------------------------------------------ */

void knh_KLRInst_setopcode(knh_KLRInst_t *inst, knh_opcode_t opcode)
{
	inst->opcode = opcode;
	inst->op->opcode = opcode;
}

/* ------------------------------------------------------------------------ */

knh_KLRCode_t* knh_InstList_newKLRCode(Ctx *ctx, knh_Array_t *insts)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	size_t i, inst_size = knh_Array_size(insts), elf_size = 0, dwarf_size, dwarf2_size = 0;
	int last_line = 0;
	knh_KLRInst_t *inst = knh_InstList_lastNULL(insts);
	if(inst != NULL && inst->opcode != OPCODE_HALT) {
		KNH_ASM(HALT);
		inst_size += 1;
	}
	for(i = 0; i < inst_size; i++) {
		inst = (knh_KLRInst_t*)knh_Array_n(insts, i);
		if(inst->opcode == OPCODE_LABEL) continue;
		if(inst->opcode == OPCODE_HALT) {
			inst->line = last_line + 1;
		}
		//DBG2_P("offset=%d, inst->line=%d, last=%d", elf_size, inst->line, last_line);
		if(inst->line > last_line) {
			knh_dwarf_t dw = {elf_size, inst->line};
			//DBG2_P("\tline=%d at=%d", dw.line, dw.offset);
			knh_Bytes_write(ctx, cwb->ba, B2((char*)(&dw), sizeof(knh_dwarf_t)));
			last_line = inst->line;
		}
		elf_size += knh_opcode_size(inst->opcode);
	}
	dwarf_size = knh_cwb_size(cwb);
	knh_KLRCode_t *kcode = (knh_KLRCode_t*)new_Object_bcid(ctx, CLASS_KLRCode, 0);
	knh_code_t *pc = NULL;
	knh_bytes_t dwarf2 = knh_cwb_tobytes(cwb);
	DP(kcode)->uri = SP(ctx->kc)->uri;
	DP(kcode)->size = elf_size + dwarf2.len;
	DP(kcode)->code = (knh_code_t*)KNH_MALLOC(ctx, DP(kcode)->size);
	pc = DP(kcode)->code;
	for(i = 0; i < inst_size; i++) {
		inst = (knh_KLRInst_t*)knh_Array_n(insts, i);
		inst->code_pos = pc;
		if(inst->opcode != OPCODE_LABEL) {
			pc += knh_opcode_size(inst->opcode);
		}
	}
	for(i = 0; i < inst_size; i++) {
		knh_KLRInst_t *inst = (knh_KLRInst_t*)knh_Array_n(insts, i);
		if(inst->opcode == OPCODE_LABEL) continue;
		if(knh_opcode_hasjump(inst->opcode)) {
			volatile knh_inst_t *op = (inst->op);
			(op)->jumppc = (op->a1)->code_pos;
		}
	}
	pc = DP(kcode)->code;
	for(i = 0; i < inst_size; i++) {
		inst = (knh_KLRInst_t*)knh_Array_n(insts, i);
		if(inst->opcode != OPCODE_LABEL) {
			size_t size = knh_opcode_size(inst->opcode);
			knh_memcpy(pc, inst->op, size);
			knh_opcode_traverse(ctx, (knh_inst_t*)pc, knh_ftraverse_inc);
			pc += size;
		}
	}
	DP(kcode)->dwarf_size = dwarf_size;
	DP(kcode)->dwarf2_size = dwarf2_size;
	knh_memcpy(DP(kcode)->code + elf_size, dwarf2.buf, dwarf2.len);
	knh_cwb_close(cwb);
	return kcode;
}

/* ------------------------------------------------------------------------ */

void knh_code_thread(Ctx *ctx, knh_code_t *pc, void **codeaddr)
{
//#ifdef KNH_USING_THREADEDCODE
	while(KNH_OPCODE(pc) != OPCODE_HALT) {
		knh_inst_t *op = (knh_inst_t*)pc;
		DBG2_ASSERT_OPCODE(op->opcode);
		op->codeaddr = codeaddr[op->opcode];
		if(knh_opcode_hasjump(op->opcode)) {
			int jopcode = ((knh_inst_t*)(op->jumppc))->opcode;
			//DBG2_P("%p(%d) GOTO %p(%d)", op, op->opcode, op->jumppc, jopcode);
			op->jumpaddr = codeaddr[jopcode];
		}
		pc += knh_opcode_size(op->opcode);
	}
//#endif/*KNH_USING_THREADEDCODE*/
}

/* ======================================================================== */
/* [methods] */

void knh_Method_setKLRCode(Ctx *ctx, knh_Method_t *mtd, knh_KLRCode_t *code)
{
	KNH_ASSERT(IS_KLRCode(code));
	if(knh_Method_isObjectCode(mtd)) {
		KNH_SETv(ctx, DP(mtd)->code, code);
	}else {
		KNH_INITv(DP(mtd)->code, code);
		knh_Method_setObjectCode(mtd, 1);
	}
	knh_Method_syncFunc(mtd, knh_KLRCode_exec);
	mtd->pc_start = DP(code)->code;
}

/* ------------------------------------------------------------------------ */

knh_code_t* knh_Method_pcstartNULL(knh_Method_t *mtd)
{
	if(knh_Method_isObjectCode(mtd)) {
		knh_KLRCode_t *o = (knh_KLRCode_t*)DP(mtd)->code;
		if(IS_KLRCode(o)) {
			return DP(o)->code;
		}
	}
	return NULL;
}

/* ------------------------------------------------------------------------ */

knh_bytes_t knh_KLRCode_tobytes(knh_KLRCode_t *o)
{
	KNH_ASSERT(IS_KLRCode(o));
	return new_bytes__2((char*)DP(o)->code, DP(o)->size);
}

/* ------------------------------------------------------------------------ */

char *knh_Method_file(Ctx *ctx, knh_Method_t *mtd)
{
	if(knh_Method_isObjectCode(mtd) && IS_KLRCode(DP(mtd)->kcode)) {
		knh_KLRCode_t *kcode = DP(mtd)->kcode;
		return FILEN(DP(kcode)->uri);
	}
	return "(naitive)";
}

/* ------------------------------------------------------------------------ */

int knh_Method_pcline(knh_Method_t *mtd, knh_code_t *pc)
{
	if(knh_Method_isObjectCode(mtd) && IS_KLRCode(DP(mtd)->kcode)) {
		knh_KLRCode_t *kcode = DP(mtd)->kcode;
		size_t offset = pc - DP(kcode)->code;
		size_t csize = DP(kcode)->size - (DP(kcode)->dwarf_size + DP(kcode)->dwarf2_size);
		if(offset <= csize) {
			int i, dsize = ((DP(kcode)->dwarf_size / sizeof(knh_dwarf_t))-1);
			knh_dwarf_t *dwarf = (knh_dwarf_t*)(DP(kcode)->code + csize);
			for(i = 0; i < dsize; i++) {
				//DBG2_P("offset=%d, dwarf[%d].offset=%d,line=%d", offset, i, dwarf[i].offset, dwarf[i].line);
				if(dwarf[i].offset <= offset && offset < dwarf[i+1].offset) {
					return dwarf[i].line;
				}
			}
			return dwarf[dsize].line;
		}
	}
	return 0;
}

/* ------------------------------------------------------------------------ */

knh_fieldn_t knh_Method_sfpfn(knh_Method_t *mtd, size_t sfpidx)
{
	if(knh_Method_isObjectCode(mtd) && IS_KLRCode(DP(mtd)->kcode)) {
		knh_KLRCode_t *kcode = DP(mtd)->kcode;
		size_t d2size = (DP(kcode)->dwarf2_size / sizeof(knh_dwarf2_t));
		if(sfpidx < d2size) {
			size_t csize = DP(kcode)->size - DP(kcode)->dwarf2_size;
			knh_dwarf2_t *dwarf2 = (knh_dwarf2_t*)(DP(kcode)->code + csize);
			return dwarf2[sfpidx].fn;
		}
	}
	return FIELDN_NONAME;
}

/* ------------------------------------------------------------------------ */

void knh_Gamma_loadCompiledMethod(Ctx *ctx)
{

}

/* ------------------------------------------------------------------------ */

knh_fmethod knh_Gamma_getCompiledMethod(Ctx *ctx, knh_bytes_t cname, knh_bytes_t mname)
{
	return NULL;
}


/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
