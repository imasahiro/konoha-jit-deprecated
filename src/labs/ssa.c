/**********************************************************************
 * Copyright (c) 2010, Masahiro Ide <masahiro at users.sourceforge.jp>
 * All rights reserved.
 *
 **********************************************************************/

#include <konoha.h>
#include <konoha/konoha_code_.h>
#include"../kc/commons.h"

#ifdef __cplusplus
extern "C" {
#endif

#define Method_isSetter(mtd) (METHODN_IS_SETTER(DP(mtd)->mn))
#define Method_isGetter(mtd) (METHODN_IS_GETTER(DP(mtd)->mn))
#define sfpidx_t knh_sfpidx_t 
struct opt_t {
    // 最大操作するsfpの数
    int max_sfp_idx;
    // 引数の数 (klr_THCODE_t->a1)
    int arg_num;
    //struct sfpidx_t *sfpinfo;
};

struct sfpinfo_t {
    // stack 変数の生存領域
    struct {
        // 開始地点
        sfpidx_t start;
        // 終了地点
        sfpidx_t end;
    } livearea;

    bool isLoaded;
    sfpidx_t loadedArea;
    // オブジェクトからフィールドとの差分
    short delta;
    bool  is_env;
    bool  is_field;
    bool  is_array;
} sfpinfo_t;

//void sfpinfo_update(sfarray_t *a, int i, sfpidx_t idx) {
//   sfpinfo_t *info = (sfpinfo_t *) knh_Array_n((knh_IArray_t*) a, idx);
//   info->livearea.end = i;
//}
//
//void sfpinfo_expire(sfarray_t *a, int i, sfpidx_t idx) {
//    knh_IArray_t *ia = (knh_IArray_t *) a;
//    int i, size = knh_Array_size(ia);
//
//    for (i = 0; i < size; i++) {
//        
//    }
//}
static inline void KLRInst_updateInst(Ctx *ctx, knh_Array_t *insts, int i, knh_inst_t *op)
{
    knh_KLRInst_t *inst = new_KLRInst(ctx, op);
    KNH_SETv(ctx, knh_Array_n(insts, i), inst);
}
void knh_KLRCode_optimize(Ctx *ctx, knh_Array_t *insts)
{
#if 0
    size_t i, inst_size = knh_Array_size(insts);
    knh_KLRInst_t *inst;

    for(i = 0; i < inst_size; i++) {
        inst = (knh_KLRInst_t*)knh_Array_n(insts, i);

        /* Peephole Optimization */
        if(inst->opcode == OPCODE_FCALL) {
            klr_FCALL_t *o = (klr_FCALL_t *) inst->op;
            knh_Method_t *mtd = o->a4;
            knh_String_t *name = knh_Method_getName(ctx, mtd);
            if (knh_bytes_strcmp(knh_String_tobytes(name), B("opNext")) == 0) {
                klr_iADDn_t op = {TADDR OPCODE_iADDn, o->a1, o->a3, 1};
                KLRInst_updateInst(ctx, insts, i, (knh_inst_t *)&op);
            }
            else if (Method_isSetter(mtd)) {
                knh_class_t cid_arg1 = knh_MethodField_pztype(DP(mtd)->mf, 1);
                if (cid_arg1 == CLASS_Float) {
                    knh_sfx_t ax = {o->a3 , DP(mtd)->delta};
                    klr_XMOVsf_t op = {TADDR OPCODE_XMOVsf, ax, o->a1 + 2};
                    KLRInst_updateInst(ctx, insts, i, (knh_inst_t *)&op);
                }
            }
            else if (Method_isGetter(mtd)) {
                knh_class_t cid_ret = knh_Method_rztype(mtd);
                if (cid_ret == CLASS_Float) {
                    knh_sfx_t ax = {o->a3, DP(mtd)->delta};
                    klr_MOVxf_t op = {TADDR OPCODE_MOVxf, o->a1 , ax};
                    KLRInst_updateInst(ctx, insts, i, (knh_inst_t *)&op);
                }
            }
        }
        else if(inst->opcode == OPCODE_SCALL) {
            klr_SCALL_t *o = (klr_SCALL_t *) inst->op;
            knh_Method_t *mtd = o->a3;
            if (Method_isSetter(mtd)) {
                knh_class_t cid_arg1 = knh_MethodField_pztype(DP(mtd)->mf, 1);
                if (cid_arg1 == CLASS_Float) {
                    knh_sfx_t ax = {o->a1 + 1, DP(mtd)->delta};
                    klr_XMOVsf_t op = {TADDR OPCODE_XMOVsf, ax, o->a1 + 2};
                    KLRInst_updateInst(ctx, insts, i, (knh_inst_t *)&op);
                }
            }
            else if (Method_isGetter(mtd)) {
                knh_class_t cid_ret = knh_Method_rztype(mtd);
                if (cid_ret == CLASS_Float) {
                    knh_sfx_t ax = {o->a1 + 1, DP(mtd)->delta};
                    klr_MOVxf_t op = {TADDR OPCODE_MOVxf, o->a1 , ax};
                    KLRInst_updateInst(ctx, insts, i, (knh_inst_t *)&op);
                }
            }
        }

        if(inst->opcode == OPCODE_RET) {
            break;
        }
    }
#endif
}

#ifdef __cplusplus
}
#endif
