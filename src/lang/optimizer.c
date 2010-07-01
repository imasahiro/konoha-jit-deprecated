/****************************************************************************
 * KONOHA COPYRIGHT, LICENSE NOTICE, AND DISCRIMER  
 * 
 * Copyright (c) 2010       Masahiro Ide <imasahiro9@gmail.com>
 *           (c) 2006-2010, Kimio Kuramitsu <kimio at ynu.ac.jp>
 *           (c) 2008-      Konoha Team konohaken@googlegroups.com  
 * All rights reserved.
 * 
 * You may choose one of the following two licenses when you use konoha. 
 * See www.konohaware.org/license.html for further information.
 * 
 * (1) GNU General Public License 2.0      (with    KONOHA_UNDER_GPL2)
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

#include"commons.h"


#ifdef __cplusplus 
extern "C" {
#endif

//#define BF(bb) ((bb)->f)
//typedef struct BasicBlock {
//    knh_hObject_t h;
//    knh_Array_t *insts;
//    Ctx *ctx;
//    struct bb_api *f;
//} BasicBlock;
//#define bb_foreach_inst(bb, i, inst)
//    for (i = 0; i < knh_Array_size((bb)->insts) &&
//            ({inst = knh_KLRINSTs_n(bb->insts, i); 1;});
//            i++)
//
//#define CASE(L) case OPCODE_##L:
//static void BB_add(BasicBlock *bb, knh_KLRInst_t *inst)
//{
//    Ctx *ctx = bb->ctx;
//    knh_opline_t *op = inst->op;
//    knh_opcode_t opcode = op->opcode;
//    switch (opcode) {
//#define omov(v)   ((klr_OMOV_t*)(v))
//#define movx(v)   ((klr_iMOVx_t*)(v))
//#define xmov(v)   ((klr_XIMOV_t*)(v))
//#define iadd(v)   ((klr_iADD_t*)(v))
//#define tr(v)     ((klr_TR_t*)(v))
//#define call(v)   ((klr_SCALL_t*)(v))
//        CASE(THCODE) CASE(LABEL) CASE(RET)
//            break;
//        CASE(OMOV) CASE(SWAP)
//            break;
//        CASE(iMOVx)
//            break;
//        CASE(XIMOV)
//            break;
//        CASE(SCALL) CASE(VCALL)
//            break;
//        CASE(TR)
//            break;
//        CASE(iADD)
//            break;
//    }
//    knh_Array_add(bb->ctx, bb->insts, inst);
//}
//
//static void knh_Array_swap(Ctx *ctx, knh_Array_t *a, knh_Array_t *o);
//static knh_Array_t *BB_dump(BasicBlock *bb, knh_Array_t *insts)
//{
//    Ctx *ctx = bb->ctx;
//    knh_Array_clear(ctx, insts, 0);
//    knh_Array_swap(ctx, insts, bb->insts);
//    KNH_FINALv(ctx, bb->insts);
//    KNH_FINALv(ctx, bb);
//    return insts;
//}
//
//struct bb_api {
//    void (*add) (struct BasicBlock *bb, knh_KLRInst_t *insts);
//    knh_Array_t *(*dump) (struct BasicBlock *bb, knh_Array_t *insts);
//} BB_api = {
//    BB_add,
//    BB_dump,
//};
//
//static BasicBlock *BB_create(Ctx *ctx, void *data)
//{
//    BasicBlock *bb;
//    KNH_INITv(bb, (BasicBlock *) new_Object_init(ctx, 0, 0, 0));
//    KNH_INITv(bb->insts, new_Array0(ctx, 0));
//    bb->ctx = ctx;
//    BF(bb) = &BB_api;
//    return bb;
//}
//
//knh_Array_t *knh_KLRInst_optimize(Ctx *ctx, knh_Array_t *insts)
//{
//    size_t i, inst_size = knh_Array_size(insts);
//    BasicBlock *bb = BB_create(ctx, NULL);
//    for(i = 0; i < inst_size; i++) {
//        knh_KLRInst_t *inst = knh_KLRINSTs_n(insts, i);
//        BF(bb)->add(bb, inst);
//    }
//    BF(bb)->dump(bb, insts);
//    return insts;
//}
//
//static void knh_Array_swap(Ctx *ctx, knh_Array_t *a, knh_Array_t *o)
//{
//    knh_Array_t _tmp;
//    _tmp.list = a->list;
//    _tmp.size = a->size;
//    _tmp.api  = a->api;
//    a->list = o->list;
//    a->size = o->size;
//    a->api  = o->api;
//    o->list = _tmp.list;
//    o->size = _tmp.size;
//    o->api  = _tmp.api;
//}

#ifdef __cplusplus
}
#endif
