/****************************************************************************
 * KONOHA COPYRIGHT, LICENSE NOTICE, AND DISCRIMER
 *
 * Copyright (c) 2006-2010, Kimio Kuramitsu <kimio at ynu.ac.jp>
 *           (c) 2008-      Konoha Team konohaken@googlegroups.com
 * All rights reserved.
 *
 * You may choose one of the following two licenses when you use konoha.
 * See www.konohaware.org/license.html for further information.
 *
 * (1) GNU Lesser General Public License 3.0 (with K_UNDER_LGPL)
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

#ifdef K_USING_DEFAULTAPI

/* ======================================================================== */
/* [Common] */

/* ------------------------------------------------------------------------ */
//## @Const method Int Object.opADDR();

static METHOD Object_opADDR(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	void *p = NULL;
	if(IS_Boolean(sfp[0].o)) {
		p = (sfp[0].bvalue) ? (void*)(KNH_TRUE) : (void*)(KNH_FALSE);
	}
	else {
		p = (void*)sfp[0].o;
	}
	RETURNi_((knh_uintptr_t)p);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Object.opOF(Class c);

static METHOD Object_opOF(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	DBG_P("cid=%s", CLASSNo(sfp[0].o));
	RETURNb_(knh_class_instanceof(ctx, knh_Object_cid(sfp[0].o), Class_tocid(sfp[1])));
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Object.opEQ(Any value);

static METHOD Object_opEQ(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_Object_compareTo(ctx, sfp[0].o, sfp[1].o) == 0);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Object.opNOTEQ(Any value);

static METHOD Object_opNOTEQ(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_Object_compareTo(ctx, sfp[0].o, sfp[1].o) != 0);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Object.opLT(Any value);

static METHOD Object_opLT(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_Object_compareTo(ctx, sfp[0].o, sfp[1].o) < 0);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Object.opLTE(Any value);

static METHOD Object_opLTE(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_Object_compareTo(ctx, sfp[0].o, sfp[1].o) <= 0);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Object.opGT(Any value);

static METHOD Object_opGT(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_Object_compareTo(ctx, sfp[0].o, sfp[1].o) > 0);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Object.opGTE(Any value);

static METHOD Object_opGTE(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_Object_compareTo(ctx, sfp[0].o, sfp[1].o) >= 0);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Int.opEQ(Int value);

static METHOD Int_opEQ(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(sfp[0].ivalue == sfp[1].ivalue);
}

/* ------------------------------------------------------------------------ */
//## @Const  method Boolean Int.opNOTEQ(Int value);

static METHOD Int_opNOTEQ(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(sfp[0].ivalue != sfp[1].ivalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Int.opLT(Int value);

static METHOD Int_opLT(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(sfp[0].ivalue < sfp[1].ivalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Int.opLTE(Int value);

static METHOD Int_opLTE(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(sfp[0].ivalue <= sfp[1].ivalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Int.opGT(Int value);

static METHOD Int_opGT(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(sfp[0].ivalue > sfp[1].ivalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Int.opGTE(Int value);

static METHOD Int_opGTE(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(sfp[0].ivalue >= sfp[1].ivalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Float.opEQ(Float value);

static METHOD Float_opEQ(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(sfp[0].fvalue == sfp[1].fvalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Float.opNOTEQ(Float value);

static METHOD Float_opNOTEQ(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(sfp[0].fvalue != sfp[1].fvalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Float.opLT(Float! value);

static METHOD Float_opLT(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(sfp[0].fvalue < sfp[1].fvalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Float.opLTE(Float! value);

static METHOD Float_opLTE(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(sfp[0].fvalue <= sfp[1].fvalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Float.opGT(Float! value);

static METHOD Float_opGT(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(sfp[0].fvalue > sfp[1].fvalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Float.opGTE(Float! value);

static METHOD Float_opGTE(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(sfp[0].fvalue >= sfp[1].fvalue);
}

/* ======================================================================== */
/* [opHAS] */

/* ------------------------------------------------------------------------ */
//## @Const method Boolean String.opHAS(String s);

static METHOD String_opHAS(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_bytes_indexOf(S_tobytes(sfp[0].s), S_tobytes(sfp[1].s)) != -1);
}

/* ------------------------------------------------------------------------ */
//## method Boolean Range.opHAS(Any v);
//## method Boolean Range.opCASE(Any v);

static METHOD Range_opHAS(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	int res = 0/*NotFound*/;
	knh_Range_t *o = sfp[0].range;
	if(IS_Int((o)->start) && IS_Float(sfp[1].o)) {
		KNH_SETv(ctx, sfp[1].o, new_Int(ctx, CLASS_Int, (knh_int_t)sfp[1].fvalue));
	}
	else if(IS_Float((o)->start) && IS_Int(sfp[1].o)) {
		KNH_SETv(ctx, sfp[1].o, new_Float(ctx, CLASS_Float, (knh_float_t)sfp[1].ivalue));
	}
	else {
		knh_stack_boxing(ctx, sfp + 1);
	}
	if(knh_Object_compareTo(ctx, o->start, sfp[1].o) <= 0) {
		if(knh_Range_isInclusive(o)) {
			if(knh_Object_compareTo(ctx, sfp[1].o, o->end) <= 0) {
				res = 1;
			}
		}
		else {
			if(knh_Object_compareTo(ctx, sfp[1].o, o->end) < 0) {
				res = 1;
			}
		}
	}
	RETURNb_(res);
}

/* ------------------------------------------------------------------------ */
//## method Boolean Array.opHAS(Any v);
//## method Boolean Array.opCASE(Any v);

static METHOD Array_opHAS(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Array_t *a = (knh_Array_t*)sfp[0].o;
	size_t i, res = 0/*NotFound*/;
	if(knh_Array_isNDATA(a)) {
		knh_ndata_t d = knh_Object_data(sfp[1].o);
		for(i = 0; i < knh_Array_size(a); i++) {
			if(d == a->nlist[i]) { res = 1; break; }
		}
	}
	else {
		for(i = 0; i < knh_Array_size(a); i++) {
			if(knh_Object_compareTo(ctx, a->list[i], sfp[1].o) == 0) {
				res = 1; break;
			}
		}
	}
	RETURNb_(res);
}

/* ------------------------------------------------------------------------ */
//## method Boolean Tuple.opHAS(Any v);
//## method Boolean Tuple.opCASE(Any v);

static METHOD Tuple_opHAS(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	if(knh_Tuple_isTriple(sfp[0].tuple)) {
		knh_Tuple_t *t = sfp[0].tuple;
		int res;
		knh_stack_boxing(ctx, sfp + 1);
		res = ((knh_Object_compareTo(ctx, t->first, sfp[1].o) == 0)
				|| (knh_Object_compareTo(ctx, t->second, sfp[1].o) == 0)
				|| (t->thirdNULL != NULL && (knh_Object_compareTo(ctx, t->thirdNULL, sfp[1].o) == 0)));
		RETURNb_(res);
	}
	else {
		Array_opHAS(ctx, sfp, rix); // reuse
	}
}

/* ======================================================================== */
/* [Case] */

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Object.opCASE(Any v);

static METHOD Object_opCASE(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_Object_compareTo(ctx, sfp[0].o, sfp[1].o) == 0);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Int.opCASE(Any v);

static METHOD Int_opCASE(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	int res = 0;
	if(IS_bInt(sfp[1].o)) {
		knh_int_t n = sfp[1].ivalue;
		if(knh_Object_cid(sfp[0].o) == CLASS_Int ||
				knh_Object_bcid(sfp[0].o) == knh_Object_bcid(sfp[0].o)) {
			res = (n == sfp[0].ivalue);
		}
	}
	RETURNb_(res);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Float.opCASE(Any v);

static METHOD Float_opCASE(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	int res = 0;
	if(IS_bFloat(sfp[1].o)) {
		knh_float_t n = sfp[1].fvalue;
		if(knh_Object_cid(sfp[0].o) == CLASS_Float ||
				knh_Object_bcid(sfp[0].o) == knh_Object_bcid(sfp[0].o)) {
			res = (n == sfp[0].fvalue);
		}
	}
	RETURNb_(res);
}

/* ======================================================================== */
/* [Semantic Comparator] */

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Object.opIS(Any v);

static METHOD Object_opIS(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
//	knh_class_t scid = knh_Object_cid(sfp[0].o);
//	knh_class_t tcid = knh_Object_cid(sfp[1].o);
//	DBG_P("Semantic Matching %s === %s", CLASSN(scid), CLASSN(tcid));
//	if(scid == tcid) {
//		Object_opEQ(ctx, sfp. rix);
//	}
//	else if(IS_NULL(sfp[1].o)) {
//		RETURNb_(IS_NULL(sfp[0].o));
//	}
//	else
	{
		TODO();
		RETURNb_(0);
//		BEGIN_LOCAL(ctx, lsfp);
//		KNH_LPUSH(ctx, sfp[1].o);
//		VM_MAP(ctx, scid);
//		KNH_SETv(ctx, sfp[1].o, ctx->esp[0].o);
//		END_LOCAL(ctx, lsfp);
//		if(IS_NULL(sfp[1].o)) {
//			RETURNb_(0);
//		}
//		else {
//			Object_opEQ(ctx, sfp);
//		}
	}
}

/* ======================================================================== */
/* [Arithemetic] */

/* ------------------------------------------------------------------------ */
/* [Int] */
//## @Const method Int Int.opADD(Int v);

static METHOD Int_opADD(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNi_(sfp[0].ivalue + sfp[1].ivalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Int Int.opNEG();

static METHOD Int_opNEG(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNi_(-(sfp[0].ivalue));
}

/* ------------------------------------------------------------------------ */
//## @Const method Int Int.opSUB(Int n);

static METHOD Int_opSUB(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNi_(sfp[0].ivalue - sfp[1].ivalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Int Int.opMUL(Int n);

static METHOD Int_opMUL(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNi_(sfp[0].ivalue * sfp[1].ivalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Int Int.opDIV(Int n);

static METHOD Int_opDIV(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	KNH_THROW_iZERODIV(sfp[1].ivalue);
	RETURNi_(sfp[0].ivalue / sfp[1].ivalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Int Int.opMOD(Int n);

static METHOD Int_opMOD(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	KNH_THROW_iZERODIV(sfp[1].ivalue);
	RETURNi_(sfp[0].ivalue % sfp[1].ivalue);
}

/* ------------------------------------------------------------------------ */
/* [Float] */
//## @Const method Float! Float.opADD(Float! v);

static METHOD Float_opADD(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNf_(sfp[0].fvalue + sfp[1].fvalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Float! Float.opNEG();

static METHOD Float_opNEG(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNf_(-(sfp[0].fvalue));
}

/* ------------------------------------------------------------------------ */
//## @Const method Float! Float.opSUB(Float! n);

static METHOD Float_opSUB(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNf_(sfp[0].fvalue - sfp[1].fvalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Float! Float.opMUL(Float! n);

static METHOD Float_opMUL(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNf_(sfp[0].fvalue * sfp[1].fvalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Float! Float.opDIV(Float! n);

static METHOD Float_opDIV(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	KNH_THROW_fZERODIV(sfp[1].fvalue);
	RETURNf_(sfp[0].fvalue / sfp[1].fvalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method String String.opSUB(String s);

static METHOD String_opSUB(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_bytes_t base = S_tobytes(sfp[0].s);
	knh_bytes_t t = S_tobytes(sfp[1].s);
	knh_uchar_t c = t.buf[0];
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	size_t i;
	for(i = 0; i < base.len; i++) {
		if(base.buf[i] == c) {
			size_t j;
			for(j = 1; j < t.len; j++) {
				if(base.buf[i+j] != t.buf[j]) break;
			}
			if(j == t.len) {
				i += t.len - 1;
				continue;
			}
		}
		knh_Bytes_putc(ctx, cwb->ba, base.buf[i]);
	}
	if(base.len == knh_cwb_size(cwb)) {
		knh_cwb_close(cwb);
		RETURN_(sfp[0].o);
	}
	else {
		RETURN_(knh_cwb_newString(ctx, cwb));
	}
}

/* ------------------------------------------------------------------------ */
/* [Any] */
//## method Any Any.opADD(Any value);
//## method Any Any.opSUB(Any value);
//## method Any Any.opMUL(Any value);
//## method Any Any.opDIV(Any value);
//## method Any Any.opMOD(Any value);
//## method Any Any.opNEG();

static METHOD Any_opADD(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Method_t *mtd = sfp[K_MTDIDX].callmtd;
	mtd = knh_lookupMethod(ctx, knh_Object_cid(sfp[0].o), DP(mtd)->mn);
	klr_setcallmtd(ctx, sfp[K_MTDIDX], mtd);
	knh_stack_typecheck(ctx, sfp, mtd, NULL);
	(sfp[K_MTDIDX].callmtd)->fcall_1(ctx, sfp, rix);
}

/* ======================================================================== */
/* [Logical] */

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Boolean.opNOT();

static METHOD Boolean_opNOT(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(!sfp[0].bvalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Int Int.opLAND(Int n, ...);

static METHOD Int_opLAND(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	size_t i, ac = knh_stack_argc(ctx, sfp);
	knh_int_t n = sfp[0].ivalue;
	for(i = 1; i < ac; i++) {
		n = n & sfp[i].ivalue;
	}
	RETURNi_(n);
}

/* ------------------------------------------------------------------------ */
//## @Const method Int Int.opLOR(Int n, ...);

static METHOD Int_opLOR(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	size_t i, ac = knh_stack_argc(ctx, sfp);
	knh_int_t n = sfp[0].ivalue;
	for(i = 1; i < ac; i++) {
		n = n | sfp[i].ivalue;
	}
	RETURNi_(n);
}

/* ------------------------------------------------------------------------ */
//## @Const method Int Int.opLXOR(Int n);

static METHOD Int_opLXOR(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNi_(sfp[0].ivalue ^ sfp[1].ivalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Int Int.opLNOT();

static METHOD Int_opLNOT(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNi_(~(sfp[0].ivalue));
}

/* ------------------------------------------------------------------------ */
//## @Const method Int Int.opLSFT(Int n);

static METHOD Int_opLSFT(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNi_(sfp[0].ivalue << sfp[1].ivalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Int Int.opRSFT(Int n);

static METHOD Int_opRSFT(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNi_(sfp[0].ivalue >> sfp[1].ivalue);
}

///* ======================================================================== */
///* [Enum] */
//
///* ------------------------------------------------------------------------ */
////## @Const method Int Int.opNEXT();
//
//static METHOD Int_opNEXT(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	RETURNi_(sfp[0].ivalue+1);
//}
//
///* ------------------------------------------------------------------------ */
////## @Const method Int Int.opPREV();
//
//static METHOD Int_opPREV(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	RETURNi_(sfp[0].ivalue-1);
//}
//
///* ------------------------------------------------------------------------ */
////## @Const method Float! Float.opNEXT();
//
//static METHOD Float_opNEXT(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	RETURNf_((sfp[0].fvalue)+K_FLOAT_ONE);
//}
//
///* ------------------------------------------------------------------------ */
////## @Const method Float! Float.opPREV();
//
//static METHOD Float_opPREV(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	RETURNf_((sfp[0].fvalue)-K_FLOAT_ONE);
//}

/* ======================================================================== */
/* [getSize] */

///* ------------------------------------------------------------------------ */
////## @Const method Int Int.getSize();
//
//static METHOD Int_getSize(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	knh_int_t res = 0;
//	if(IS_NOTNULL(sfp[0].o)) {
//		knh_int_t res = sfp[0].ivalue;
//		if(res < 0) res = -(res);
//	}
//	RETURNi_(res);
//}
//
///* ------------------------------------------------------------------------ */
////## @Const method Int Float.getSize();
//
//static METHOD Float_getSize(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	knh_int_t res = 0;
//	if(IS_NOTNULL(sfp[0].o)) {
//		res = (knh_uint_t)sfp[0].fvalue;
//	}
//	RETURNi_(res);
//}

/* ------------------------------------------------------------------------ */
//## method Int Bytes.getSize();
//## method Int Array.getSize();
//## method Int DictMap.getSize();

METHOD Bytes_getSize(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNi_((sfp[0].ba)->bu.len);
}

/* ------------------------------------------------------------------------ */
//## @Const method Int String.getSize();

static METHOD String_getSize(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	size_t size = IS_bString(sfp[0].s) ? S_size(sfp[0].s) : 0;
	if(!knh_String_isASCII(sfp[0].s)) {
		size = knh_bytes_mlen(S_tobytes(sfp[0].s));
	}
	RETURNi_(size);
}

/* ------------------------------------------------------------------------ */
//## method Int Tuple.getSize();

static METHOD Tuple_getSize(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	size_t size = (IS_NULL(sfp[0].o)) ? 0 : 2;
	if(!knh_Tuple_isTriple(sfp[0].tuple)) size = (sfp[0].tuple)->size;
	RETURNi_(size);
}

/* ======================================================================== */
/* [get,set] */

/* ------------------------------------------------------------------------ */
//## method Int Bytes.get(Int n);

static METHOD Bytes_get(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Bytes_t *ba = sfp[0].ba;
	size_t n2 = knh_array_index(ctx, Int_to(size_t, sfp[1]), ba->bu.len);
	RETURNi_(ba->bu.buf[n2]);
}

/* ------------------------------------------------------------------------ */
//## method Int Bytes.set(Int n, Int c);

static METHOD Bytes_set(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Bytes_t *ba = sfp[0].ba;
	size_t n2 = knh_array_index(ctx, Int_to(size_t, sfp[1]), ba->bu.len);
	ba->bu.buf[n2] = Int_to(knh_uchar_t, sfp[2]);
	RETURNi_(ba->bu.buf[n2]);
}

/* ------------------------------------------------------------------------ */
//## method void Bytes.setAll(Int c);

static METHOD Bytes_setAll(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Bytes_t *ba = sfp[0].ba;
	size_t i, n = Int_to(size_t, sfp[1]);
	for(i = 0; i < ba->bu.len; i++) {
		ba->bu.buf[i] = n;
	}
	RETURNvoid_();
}

/* ------------------------------------------------------------------------ */
//## @Const method String String.get(Int n);

static METHOD String_get(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_bytes_t base = S_tobytes(sfp[0].s);
	knh_String_t *s;
	if(knh_String_isASCII(sfp[0].s)) {
		size_t n = knh_array_index(ctx, Int_to(size_t, sfp[1]), S_size(sfp[0].s));
		base.buf = base.buf + n;
		base.len = 1;
		s = new_String_(ctx, CLASS_String, base, sfp[0].s);
	}
	else {
		size_t off = knh_array_index(ctx, Int_to(size_t, sfp[1]), knh_bytes_mlen(base));
		knh_bytes_t sub = knh_bytes_mofflen(base, off, 1);
		s = new_String_(ctx, CLASS_String, sub, sfp[0].s);
	}
	RETURN_(s);
}

///* ------------------------------------------------------------------------ */
////## method Any Tuple.get(Int n);
//
//static METHOD Tuple_get(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	if(knh_Tuple_isTriple(sfp[0].tuple)) {
//		Object *v = KNH_NULL;
//		switch(Int_to(size_t, sfp[1])) {
//		case 0: v = (sfp[0].tuple)->first; break;
//		case 1: v = (sfp[1].tuple)->first; break;
//		case 2: v = (sfp[2].tuple)->first; break;
//		default: knh_array_index(ctx, Int_to(size_t, sfp[1]), 3);
//		}
//		RETURN_(v);
//	}
//	else {
//		Array_get(ctx, sfp, rix); /* reuse */
//	}
//}


/* ======================================================================== */
/* [opRange] */

static void knh_stack_rangeUntil(Ctx *ctx, knh_sfp_t *sfp, size_t size, size_t *s, size_t *e)
{
	*s = knh_array_index(ctx, Int_to(size_t, sfp[1]), size);
	*e = sfp[2].ivalue == 0 ? (size) : knh_array_index(ctx, Int_to(size_t, sfp[2]), size);
}

/* ------------------------------------------------------------------------ */

static void knh_stack_rangeTo(Ctx *ctx, knh_sfp_t *sfp, size_t size, size_t *s, size_t *e)
{
	*s = IS_NULL(sfp[1].o) ? 0 : knh_array_index(ctx, Int_to(size_t, sfp[1]), size);
	*e = IS_NULL(sfp[2].o) ? (size) : knh_array_index(ctx, Int_to(size_t, sfp[2]) + 1, size);
}

/* ------------------------------------------------------------------------ */

static knh_Bytes_t *new_Bytes__range(Ctx *ctx, knh_Bytes_t *ba, size_t s, size_t e)
{
	knh_Bytes_t *newa = new_(Bytes);
	if(e < s) {
		size_t t = s; s = e; e = t;
	}
	DBG_ASSERT(e <= BA_size(ba));
	if(s < e) {
		size_t newsize = e - s;
		size_t capacity = newsize;
		if(newsize > 0) {
			if(capacity < 256) capacity = 256;
			newa->bu.buf = (knh_uchar_t*)KNH_MALLOC(ctx, capacity);
			knh_bzero(newa->bu.buf, capacity);
			knh_memcpy(newa->bu.buf, &ba->bu.buf[s], newsize);
		}
		else {
			newa->bu.buf = (knh_uchar_t*)"";
		}
		newa->capacity = capacity;
		newa->bu.len = newsize;
	}
	return newa;
}

/* ------------------------------------------------------------------------ */
//## method Bytes Bytes.opUNTIL(Int s, Int e);

static METHOD Bytes_opUNTIL(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	size_t s, e;
	knh_stack_rangeUntil(ctx, sfp, (sfp[0].ba)->bu.len, &s, &e);
	RETURN_(new_Bytes__range(ctx, sfp[0].ba, s, e));
}

/* ------------------------------------------------------------------------ */
//## method Bytes Bytes.opTO(Int s, Int e);

static METHOD Bytes_opTO(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	size_t s, e;
	knh_stack_rangeTo(ctx, sfp, (sfp[0].ba)->bu.len, &s, &e);
	RETURN_(new_Bytes__range(ctx, sfp[0].ba, s, e));
}

/* ------------------------------------------------------------------------ */
//## @Const method String String.substring(Int offset, Int length);

static METHOD String_substring(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_String_t *s;
	knh_bytes_t base = S_tobytes(sfp[0].s);
	knh_bytes_t sub;
	if(knh_String_isASCII(sfp[0].s)) {
		size_t offset = knh_array_index(ctx, sfp[1].ivalue, base.len);
		sub = knh_bytes_last(base, offset);
		if(sfp[2].ivalue != 0) {
			size_t len = (size_t)sfp[2].ivalue;
			if(len < sub.len) sub = knh_bytes_first(sub, len);
		}
	}
	else { // multibytes
		size_t mlen = knh_bytes_mlen(base);
		size_t offset = knh_array_index(ctx, sfp[1].ivalue, mlen);
		size_t length = sfp[2].ivalue == 0  ? (mlen - offset) : (size_t)sfp[2].ivalue;
		sub = knh_bytes_mofflen(base, offset, length);
	}
	if(sub.len == 0) {
		s = TS_EMPTY;
	}
	else if(sub.len == base.len) {
		s = sfp[0].s;
	}
	else {
		s = new_String_(ctx, CLASS_String, sub, sfp[0].s);
	}
	RETURN_(s);
}

/* ------------------------------------------------------------------------ */
//## @Const method String String.opUNTIL(Int s, Int e);

static METHOD String_opUNTIL(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	if(sfp[2].ivalue != 0) {
		size_t offset = IS_NULL(sfp[1].o) ? 0 : (size_t)sfp[1].ivalue;
		sfp[2].ivalue = sfp[2].ivalue - offset;
	}
	String_substring(ctx, sfp, rix);
}

/* ------------------------------------------------------------------------ */
//## @Const method String String.opTO(Int s, Int e);

static METHOD String_opTO(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	if(sfp[2].ivalue != 0) {
		size_t offset = IS_NULL(sfp[1].o) ? 0 : (size_t)sfp[1].ivalue;
		sfp[2].ivalue = sfp[2].ivalue - offset + 1;
	}
	String_substring(ctx, sfp, rix);
}

/* ------------------------------------------------------------------------ */

static knh_Array_t *new_Array__range(Ctx *ctx, knh_Array_t *a, size_t s, size_t e /*until*/)
{
	knh_Array_t *newa = new_O(Array, a->h.cid);
	if(e < s) {
		size_t t = s; s = e; e = t;
	}
	if(s < e) {
		DBG_ASSERT(e <= a->size);
		size_t newsize = e - s;
		size_t sizeofdata = a->hmem[-1].wsize;
		if(newsize > 0) {
			char *p = (char*)a->list;
			knh_Array_grow(ctx, newa, newsize, newsize);
			knh_memcpy(newa->list, p + (s * sizeofdata), newsize * sizeofdata);
			if(!knh_Array_isNDATA(a)) {
				size_t i;
				for(i = 0; i < newsize; i++) {
					knh_Object_RCinc(newa->list[i]);
				}
			}
		}
		newa->size = newsize;
	}
	return newa;
}

/* ------------------------------------------------------------------------ */
//## method This Array.opUNTIL(Int s, Int e);

static METHOD Array_opUNTIL(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	size_t s, e;
	knh_stack_rangeUntil(ctx, sfp, knh_Array_size(sfp[0].a), &s, &e);
	RETURN_(new_Array__range(ctx, sfp[0].a, s, e));
}

/* ------------------------------------------------------------------------ */
//## method This Array.opTO(Int s, Int e);

static METHOD Array_opTO(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	size_t s, e;
	knh_stack_rangeTo(ctx, sfp, knh_Array_size(sfp[0].a), &s, &e);
	RETURN_(new_Array__range(ctx, sfp[0].a, s, e));
}

/* ------------------------------------------------------------------------ */
/* [DictMap] */

static
int knh_fbytescmp_1(knh_bytes_t t1, knh_bytes_t t2)
{
	return 1;
}

static
knh_DictMap_t *knh_DictMap_subset(Ctx *ctx, knh_DictMap_t *d, knh_String_t* ss, knh_String_t *es, int isInclusive)
{
	knh_bytes_t s = IS_bString(ss) ? S_tobytes(ss) : STEXT("");
	knh_bytes_t e = IS_bString(es) ? S_tobytes(es) : STEXT("");
	size_t i, dsize = d->size;
	knh_DictMap_t *newd = new_DictMap(ctx, knh_Object_p1(d), 0);
	int zero = (isInclusive) ?  0 : 1;
	knh_Fbytescmp f1 = (s.len > 0) ? d->fcmp : knh_fbytescmp_1;
	knh_Fbytescmp f2 = (e.len > 0) ? d->fcmp : knh_fbytescmp_1;
	for(i = 0; i < dsize; i++) {
		knh_bytes_t key = S_tobytes(knh_DictMap_keyAt(d, i));
		if(f1(key, s) <= 0 && f2(e, key) >= zero) {
			knh_DictMap_append(ctx, newd, knh_DictMap_keyAt(d, i), knh_DictMap_valueAt(d, i));
		}
	}
	return newd;
}

/* ------------------------------------------------------------------------ */
//## method This DictMap.opTO(String s, String e);

static METHOD DictMap_opTO(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURN_(knh_DictMap_subset(ctx, sfp[0].dmap, sfp[1].s, sfp[2].s, 1));
}

/* ------------------------------------------------------------------------ */
//## method This DictMap.opUNTIL(String s, String e);

static METHOD DictMap_opUNTIL(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURN_(knh_DictMap_subset(ctx, sfp[0].dmap, sfp[1].s, sfp[2].s, 1));
}
/* ------------------------------------------------------------------------ */
//## method Any Tuple.opEXPAND();

static METHOD Tuple_opEXPAND(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	Object *res;
	if(knh_Tuple_isTriple(sfp[0].tuple)) {
		res = (sfp[0].tuple)->first;
	}
	else {
		knh_array_index(ctx, 0, (sfp[0].tuple)->size);
		res = (sfp[0].tuple)->list[0];
	}
	RETURN_(res);
}

/* ------------------------------------------------------------------------ */
//## method T1! Array.opEXPAND();

static METHOD Array_opEXPAND(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Array_t *a = sfp[0].a;
	RETURN_(knh_Array_n(a, knh_array_index(ctx, 0, knh_Array_size(a))));
}

/* ------------------------------------------------------------------------ */


#endif /*K_USING_DEFAULTAPI*/

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
