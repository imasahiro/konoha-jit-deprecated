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

/* ------------------------------------------------------------------------ */
//## method This Connection.new(String urn);

static METHOD Connection_new(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Connection_t *o = (knh_Connection_t*)sfp[0].o;
	knh_Connection_open(ctx, o, sfp[1].s);
	RETURN_(sfp[0].o);
}

/* ------------------------------------------------------------------------ */
//## method ResultSet! Connection.query(String query);

static METHOD Connection_query(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Connection_t *c = (knh_Connection_t*)sfp[0].o;
	knh_String_t *query = (knh_String_t*)sfp[1].o;
	knh_ResultSet_t *rs = new_(ResultSet);
	KNH_RCSETv(ctx, sfp[2].o, rs);
	knh_qcur_t *qcur = (c)->dspi->qexec(ctx, (c)->conn, S_tobytes(query), rs);
	if(qcur != NULL) {
		DP(rs)->qcur = qcur;
		DP(rs)->qcurfree = (c)->dspi->qcurfree;
	}
	else {
		DP(rs)->qcur = NULL;
		DP(rs)->qcurfree = knh_getQueryDSPI(ctx, K_DEFAULT_DSPI)->qcurfree;
	}
	KNH_SETv(ctx, DP(rs)->conn, c);
	RETURN_(rs);
}

/* ------------------------------------------------------------------------ */
//## method void Connection.exec(String query);

static METHOD Connection_exec(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Connection_t *c = (knh_Connection_t*)sfp[0].o;
	knh_String_t *query = sfp[1].s;
	(c)->dspi->qexec(ctx, (c)->conn, S_tobytes(query), NULL);
	RETURNvoid_();
}

/* ------------------------------------------------------------------------ */
//## method void Connection.close();

static METHOD Connection_close(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Connection_close(ctx, (knh_Connection_t*)sfp[0].o);
	RETURNvoid_();
}

/* ------------------------------------------------------------------------ */
//## method Int ResultSet.getSize();

static METHOD ResultSet_getSize(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_ResultSet_t *o = (knh_ResultSet_t*)sfp[0].o;
	RETURNi_(DP(o)->column_size);
}

/* ------------------------------------------------------------------------ */
//## method Boolean ResultSet.next();

static METHOD ResultSet_next(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_ResultSet_next(ctx, (knh_ResultSet_t*)sfp[0].o));
}

/* ------------------------------------------------------------------------ */
//## method String ResultSet.getName(Int n);

static METHOD ResultSet_getName(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_ResultSet_t *o = (knh_ResultSet_t*)sfp[0].o;
	size_t n = Int_to(size_t, sfp[1]);
	knh_String_t *v = TS_EMPTY;
	if(n < DP(o)->column_size) {
		v = knh_ResultSet_getName(ctx, o, n);
	}
	else {
		KNH_THROW_OUTOFINDEX(ctx, sfp[1].ivalue, DP(o)->column_size);
	}
	RETURN_(v);
}

/* ------------------------------------------------------------------------ */

static int knh_ResultSet_indexof_(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_ResultSet_t *o = (knh_ResultSet_t*)sfp[0].o;
	if(IS_bInt(sfp[1].o)) {
		size_t n = Int_to(size_t, sfp[1]);
		if(!(n < DP(o)->column_size)) {
			KNH_THROW_OUTOFINDEX(ctx, sfp[1].ivalue, DP(o)->column_size);
			return -1;
		}
		return n;
	}
	else if(IS_bString(sfp[1].o)) {
		int loc = knh_ResultSet_findColumn(ctx, o, S_tobytes(sfp[1].s));
		if(loc == -1) {
			TODO();
			//KNH_STUPID(ctx, o, STUPID_NOTFOUND);
		}
		return loc;
	}
	TODO();
	//KNH_STUPID(ctx, o, STUPID_NOTFOUND);
	return -1;
}

/* ------------------------------------------------------------------------ */
//## method Int ResultSet.getInt(Any n);

static METHOD ResultSet_getInt(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	int n = knh_ResultSet_indexof_(ctx, sfp);
	knh_int_t res = 0;
	if(n >= 0) {
		knh_ResultSet_t *o = (knh_ResultSet_t*)sfp[0].o;
		char *p = BA_tochar(DP(o)->databuf) + DP(o)->column[n].start;
		switch(DP(o)->column[n].ctype) {
		case knh_ResultSet_CTYPE__integer :
			res = *((knh_int_t*)p); break;
		case knh_ResultSet_CTYPE__float :
			res = (knh_int_t)(*((knh_float_t*)p)); break;
		case knh_ResultSet_CTYPE__null :
		default:
			KNH_SETv(ctx, sfp[rix].o, KNH_NULVAL(CLASS_Int));
		}
	}
	RETURNi_(res);
}

/* ------------------------------------------------------------------------ */
//## method Float ResultSet.getFloat(Any n);

static METHOD ResultSet_getFloat(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	
	int n = knh_ResultSet_indexof_(ctx, sfp);
	knh_float_t res = K_FLOAT_ZERO;
	if(n >= 0) {
		knh_ResultSet_t *o = (knh_ResultSet_t*)sfp[0].o;
		char *p = BA_tochar(DP(o)->databuf) + DP(o)->column[n].start;
		switch(DP(o)->column[n].ctype) {
		case knh_ResultSet_CTYPE__integer :
			res = (knh_float_t)(*((knh_int_t*)p)); break;
		case knh_ResultSet_CTYPE__float :
			res = (*((knh_float_t*)p)); break;
		case knh_ResultSet_CTYPE__null :
		default:
			KNH_SETv(ctx, sfp[rix].o, KNH_NULVAL(CLASS_Float));
		}
	}
	RETURNf_(res);
}

/* ------------------------------------------------------------------------ */
//## method String ResultSet.getString(Any n);

static METHOD ResultSet_getString(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	
	int n = knh_ResultSet_indexof_(ctx, sfp);
	Object *v = KNH_NULL;
	if(n >= 0) {
		v = UP(knh_ResultSet_getString(ctx, (knh_ResultSet_t*)sfp[0].o, n));
	}
	RETURN_(v);
}

/* ------------------------------------------------------------------------ */
//## method Any ResultSet.get(Any n);

static METHOD ResultSet_get(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	
	int n = knh_ResultSet_indexof_(ctx, sfp);
	Object *v = KNH_NULL;
	if(n >= 0) {
		knh_ResultSet_t *o = (knh_ResultSet_t*)sfp[0].o;
		char *p = BA_tochar(DP(o)->databuf) + DP(o)->column[n].start;
		switch(DP(o)->column[n].ctype) {
		case knh_ResultSet_CTYPE__integer :
			KNH_SETv(ctx, sfp[rix].o, KNH_INT0);
			RETURNi_((*((knh_int_t*)p)));
		case knh_ResultSet_CTYPE__float :
			KNH_SETv(ctx, sfp[rix].o, KNH_FLOAT0);
			RETURNf_((*((knh_float_t*)p)));
		case knh_ResultSet_CTYPE__text :
			v = UP(new_S(ctx, B2(BA_tochar(DP(o)->databuf) + DP(o)->column[n].start, DP(o)->column[n].len)));
			break;
		case knh_ResultSet_CTYPE__bytes :
			{
				knh_Bytes_t *ba = new_Bytes(ctx, DP(o)->column[n].len);
				knh_Bytes_write(ctx, ba, B2(BA_tochar(DP(o)->databuf) + DP(o)->column[n].start, DP(o)->column[n].len));
				v = UP(ba);
			}
			break;
		default:
			v = KNH_NULL;
		}
	}
	RETURN_(v);
}

///* ------------------------------------------------------------------------ */
////## method void ResultSet.%dump(OutputStream w, String m);
//
//static void knh_ResultSet__dump(Ctx *ctx, knh_ResultSet_t *o, knh_OutputStream_t *w, knh_String_t *m)
//{
//	knh_putc(ctx, w, '{');
//	size_t n;
//	for(n = 0; n < DP(o)->column_size; n++) {
//		if(n > 0) {
//			knh_write_delim(ctx,w);
//		}
//		knh_write(ctx, w, S_tobytes(DP(o)->column[n].name));
//		knh_printf(ctx, w, "(%d): ", n);
//		char *p = BA_tochar(DP(o)->databuf) + DP(o)->column[n].start;
//		switch(DP(o)->column[n].ctype) {
//			case knh_ResultSet_CTYPE__null :
//				knh_write(ctx, w, STEXT("null"));
//				break;
//			case knh_ResultSet_CTYPE__integer :
//				knh_write_ifmt(ctx, w, K_INT_FMT, (*((knh_int_t*)p)));
//				break;
//			case knh_ResultSet_CTYPE__float :
//				knh_write_ffmt(ctx, w, K_FLOAT_FMT, (*((knh_float_t*)p)));
//				break;
//			case knh_ResultSet_CTYPE__text :
//				knh_write(ctx, w, B2(p, DP(o)->column[n].len));
//				break;
//			case knh_ResultSet_CTYPE__bytes :
//				knh_printf(ctx, w, "BLOB(%dbytes)", DP(o)->column[n].len);
//				break;
//		}
//	}
//	knh_putc(ctx, w, '}');
//}

/* ------------------------------------------------------------------------ */
//## method void ResultSet.close();

static METHOD ResultSet_close(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	
	knh_ResultSet_close(ctx, (knh_ResultSet_t*)sfp[0].o);
	RETURNvoid_();
}

/* ------------------------------------------------------------------------ */

#endif

#ifdef __cplusplus
}
#endif
