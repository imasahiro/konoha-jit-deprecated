#undef KLR_JMP
#define KLR_JMP(ctx, cur, PC, JUMP) {\
		cur = PC;\
		goto JUMP; \
	}\

#undef KLR_SKIP
#define KLR_SKIP(ctx,cur,  PC, JUMP) \
	if(!knh_Context_isDebug(ctx)) { \
		KLR_JMP(ctx, cur, PC, JUMP); \
	} \

#undef KLR_bJIFT
#define KLR_bJIFT(ctx, cur, PC, JUMP, n) \
	if(sfp[n].bvalue) { \
		KLR_JMP(ctx, cur, PC, JUMP); \
	} \

#undef KLR_bJIFF
#define KLR_bJIFF(ctx, cur, PC, JUMP, n) \
	if(!sfp[n].bvalue) { \
		KLR_JMP(ctx, cur, PC, JUMP); \
	} \

#undef KLR_bJIFF_LOOP
#define KLR_bJIFF_LOOP(ctx, cur, PC, JUMP, n) \
	if(!sfp[n].bvalue) { \
		KLR_JMP(ctx, cur, PC, JUMP); \
	} \

#undef KLR_JIFNUL
#define KLR_JIFNUL(ctx, cur, PC, JUMP, n) \
	if(IS_NULL(sfp[n].o)) { \
		KLR_JMP(ctx, cur, PC, JUMP); \
	} \

#undef KLR_JIFNN
#define KLR_JIFNN(ctx, cur, PC, JUMP, n) \
	if(IS_NOTNULL(sfp[n].o)) { \
		KLR_JMP(ctx, cur, PC, JUMP); \
	} \

#undef KLR_TRY
#define KLR_TRY(ctx, cur, PC, JUMP, hn)  {\
		knh_ExceptionHandler_t* _hdr = sfp[hn].hdr; \
		if(!IS_ExceptionHandler(_hdr)) { \
			_hdr = new_ExceptionHandler(ctx); \
			KLR_MOV(ctx, sfp[hn].o, _hdr); \
		} \
		DBG2_SETESP(ctx, sfp + hn + 1); \
		if(!knh_ExceptionHandler_isJumpable(_hdr)) { \
			int jump = KNH_SETJUMP(_hdr); \
			knh_ExceptionHandler_setJumpable(_hdr, 1); \
			if(jump != 0) { \
				DBG2_SETESP(ctx, DP(_hdr)->esp); \
				pc = DP(_hdr)->pc; \
				KLR_JMP(ctx, pc, PC, JUMP); \
			} \
			DP(_hdr)->esp = sfp + hn + 1; \
			DP(_hdr)->pc  = pc; \
		} \
		knh_ExceptionHandler_setCatching(_hdr, 1); \
	} \

#undef KLR_CATCH
#define KLR_CATCH(ctx, cur, PC, JUMP, hn, en, emsg) { \
		DBG2_ASSERT(IS_ExceptionHandler(sfp[hn].hdr)); \
		knh_Exception_t* _e = DP(sfp[hn].hdr)->caught; \
		if(knh_Exception_isa(ctx, _e, (knh_String_t*)emsg)) { \
			DBG2_P("CATHING .."); \
			KLR_MOV(ctx, sfp[en].o, _e); \
			KNH_SETv(ctx, DP(sfp[hn].hdr)->caught, KNH_NULL);\
		} \
		else { \
			KLR_JMP(ctx, cur, PC, JUMP); \
		} \
	} \

#undef KLR_NEXT
#define KLR_NEXT(ctx, cur, PC, JUMP, na, ib) { \
		knh_sfp_t *itrsfp_ = sfp + ib; \
		DBG2_ASSERT(IS_bIterator(itrsfp_[0].it));\
		if(!((itrsfp_[0].it)->fnext_1(ctx, itrsfp_, na - ib))) { \
			KLR_JMP(ctx, cur, PC, JUMP); \
		} \
	} \

#undef KLR_INEXT
#define KLR_INEXT(ctx, cur, PC, JUMP, reqc, na, ib) { \
		knh_sfp_t *itrsfp_ = sfp + ib; \
		knh_class_t ncid_;\
		DBG2_ASSERT(IS_bIterator(itrsfp_[0].it));\
		do {\
			if(!((itrsfp_[0].it)->fnext_1(ctx, itrsfp_, na - ib))) { \
				if(knh_Object_cid(sfp[na].o) != reqc) {\
					KNH_SETv(ctx, sfp[na].o, KNH_DEF(ctx, reqc)); \
				}\
				KLR_JMP(ctx, cur, PC, JUMP); \
			} \
			ncid_ = knh_Object_cid(sfp[na].o);\
		}while(ncid_ != reqc && !knh_class_instanceof(ctx, reqc, ncid_));\
	} \


