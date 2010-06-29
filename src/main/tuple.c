/****************************************************************************
 *
 * Pair, Tuple, Range class API
 *
 * Copyright (c) 2009  Masahiro Ide
 *
 ****************************************************************************/

/* ************************************************************************ */

#include"commons.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ======================================================================== */

//KNHAPI(knh_Pair_t*) new_Pair(Ctx *ctx, knh_class_t p1, Object *first, Object *second)
//{
//	knh_class_t cid = knh_class_P1(ctx, CLASS_Pair, p1);
//	knh_Pair_t *o = (knh_Pair_t*)new_H(ctx, FLAG_Pair, CLASS_Pair, cid);
//	o->first  = first;
//	o->second = second;
//	return o;
//}
//
///* ------------------------------------------------------------------------ */
//
//KNHAPI(knh_Tuple_t*) new_Tuple(Ctx *ctx, knh_class_t p1, size_t capacity)
//{
//	knh_class_t cid = knh_class_Generics(ctx, CLASS_Tuple, p1, CLASS_Tuple);
//	knh_Tuple_t *t = (knh_Tuple_t*)new_H(ctx, FLAG_Tuple, CLASS_Tuple, cid);
//	if(capacity <= 3) {
//		knh_Tuple_setTriple(t, 1);
//		KNH_INITv(t->first,  KNH_NULL);
//		KNH_INITv(t->second, KNH_NULL);
//		KNH_INITv(t->third,  KNH_NULL);
//	} else {
//		int i;
//		t->size = capacity;
//		t->list = (knh_Object_t **) KNH_MALLOC(ctx, sizeof(void*) * t->size);
//		for (i = 0; i < t->size; i++) {
//			KNH_INITv(t->list[i], KNH_NULL);
//		}
//	}
//	return t;
//}

/* ------------------------------------------------------------------------ */

KNHAPI(knh_Range_t*) new_Range(Ctx *ctx, knh_class_t p1, Object *start, Object *end)
{
	knh_class_t cid = knh_class_P1(ctx, CLASS_Range, p1);
	knh_Range_t *o = (knh_Range_t*)new_hObject_(ctx, FLAG_Range, CLASS_Range, cid);
	o->start = start;
	o->end   = end;
	return o;
}

/* ------------------------------------------------------------------------ */


#ifdef __cplusplus
}
#endif
