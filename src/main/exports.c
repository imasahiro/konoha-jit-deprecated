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

#define USE_B 1
#define USE_bytes_rindex 1
#define USE_bytes_first  1

#include"commons.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */

static void knh_setsfp(Ctx *ctx, knh_sfp_t *sfp, void *v)
{
	knh_Object_t *o = (knh_Object_t*)v;
	DBG_ASSERT_ISOBJECT(o);
	knh_Object_RCinc(o);
	knh_Object_RCdec(sfp[0].o);
	if(knh_Object_isRC0(sfp[0].o)) {
		knh_Object_free(ctx, sfp[0].o);
	}
	sfp[0].o = o;
}

static void knh_cwb_clearAPI(knh_cwb_t *cwb, size_t n)
{
	knh_Bytes_clear(cwb->ba, cwb->pos + n);
}

static char *knh_cwb_tocharAPI(Ctx *ctx, knh_cwb_t *cwb)
{
	knh_bytes_t t = {{(cwb->ba)->bu.buf + cwb->pos}, (cwb->ba)->bu.len - cwb->pos};
	knh_Bytes_ensureZero(ctx, cwb->ba);
	return (char*)t.buf;
}


static knh_String_t *new_StringAPI(Ctx *ctx, char *str)
{
	if(str == NULL) {
		return TS_EMPTY;
	}
	else {
		return new_String_(ctx, CLASS_String, B(str), NULL);
	}
}

static char* knh_String_text(Ctx *ctx, knh_String_t *s)
{
	if(s->str.buf[s->str.len] != '\0') {
		knh_uchar_t *newstr = (knh_uchar_t*)KNH_MALLOC(ctx, KNH_SIZE(s->str.len+1));
		knh_memcpy(newstr, s->str.buf, s->str.len);
		newstr[s->str.len] = '\0';
		s->str.buf = newstr;
		DBG_ASSERT(s->memoNULL != NULL);
		KNH_FINALv(ctx, s->memoNULL);
		s->memoNULL = NULL;
	}
	return (char*)s->str.buf;
}

/* ------------------------------------------------------------------------ */
/* [RawPtr] */

static void knh_FfreeRawPtr_NOP(Ctx *ctx, knh_RawPtr_t *p)
{
	p->ptr = NULL;
	p->pfree = knh_FfreeRawPtr_NOP;
}

static void knh_RawPtr_init(Ctx *ctx, knh_RawPtr_t *p, void *ptr, knh_FfreeRawPtr pfree)
{
	DBG_ASSERT(IS_bAny(p));
	p->ptr = ptr;
	if(pfree == NULL) {
		pfree = knh_FfreeRawPtr_NOP;
	}
	p->pfree = pfree;
}

static knh_RawPtr_t* new_RawPtr(Ctx *ctx, void *ptr, knh_FfreeRawPtr pfree, knh_class_t cid, const char *lname)
{
	knh_RawPtr_t *p = new_(RawPtr);
	if(knh_class_bcid(cid) != CLASS_Any) {
		knh_bytes_t t = {{(knh_uchar_t*)lname}, knh_strlen(lname)};
		cid = knh_getcid(ctx, t);
		if(cid == CLASS_unknown) {
			KNH_SYSLOG(ctx, LOG_WARNING, "UnknownRawPtrClass", "name=%s", lname);
			cid = CLASS_Any;
		}
	}
	p->h.cid = cid;
	knh_RawPtr_init(ctx, p, ptr, pfree);
	return p;
}

static knh_InputStream_t *new_InputStreamNULL(Ctx *ctx, knh_String_t *s, char *mode)
{
	knh_bytes_t path = S_tobytes(s);
	knh_StreamDSPI_t *dspi = knh_getStreamDSPI(ctx, path);
	knh_io_t fd = dspi->fopen(ctx, path, mode);
	if(fd != IO_NULL) {
		knh_InputStream_t *in = new_InputStreamDSPI(ctx, fd, dspi);
		KNH_SETv(ctx, DP(in)->urn, s);
		return in;
	}
	return NULL;
}

static knh_OutputStream_t *new_OutputStreamNULL(Ctx *ctx, knh_String_t *s, char *mode)
{
	knh_bytes_t path = S_tobytes(s);
	knh_StreamDSPI_t *dspi = knh_getStreamDSPI(ctx, path);
	knh_io_t fd = dspi->fopen(ctx, path, mode);
	if(fd != IO_NULL) {
		knh_OutputStream_t *in = new_OutputStreamDSPI(ctx, fd, dspi);
		KNH_SETv(ctx, DP(in)->urn, s);
		return in;
	}
	return NULL;
}


static void _putc(Ctx *ctx, void *p, int ch)
{
	knh_Bytes_t *ba = (knh_Bytes_t*)p;
	if(p == ctx->bufw) {
		ba = ctx->bufa;
	}
	if(IS_OutputStream(ba)) {
		knh_OutputStream_t *w = (knh_OutputStream_t*)ba;
		knh_OutputStream_putc(ctx, w, ch);
	}
	else if(IS_Bytes(ba)) {
		knh_Bytes_putc(ctx, ba, ch);
	}
}

static void _write(Ctx *ctx, void *p, char *buf, size_t i)
{
	knh_Bytes_t *ba = (knh_Bytes_t*)p;
	knh_bytes_t t = {{(knh_uchar_t*)buf}, i};
	if(p == ctx->bufw) {
		ba = ctx->bufa;
	}
	if(IS_Bytes(ba)) {
		knh_Bytes_write(ctx, ba, t);
	}
	else if(IS_OutputStream(ba)) {
		knh_OutputStream_t *w = (knh_OutputStream_t*)ba;
		knh_OutputStream_write(ctx, w, t);
	}
}

const knh_ExportsAPI_t *knh_getExportsAPI(void)
{
	static knh_ExportsAPI_t exports = {
		knh_fastmalloc, knh_fastfree, /* memory.c */
		knh_setsfp, knh_Iterator_close,
		knh_makeEvidence, knh_stack_perror, dbg_p, todo_p,
		knh_cwb_clearAPI, knh_cwb_tocharAPI,
		new_StringAPI, knh_String_text,
		new_RawPtr, knh_RawPtr_init,
		new_InputStreamNULL, new_OutputStreamNULL,
		_putc, _write,
	};
	return &exports;
}

/* ------------------------------------------------------------------------ */
/* [CONST/PROPERTY DATA] */

static void knh_addConstData(Ctx *ctx, char *dname, Object *value)
{
	if(dname[0] == '$') {
		knh_String_t *n = new_T(dname + 1);
		knh_DictMap_set_(ctx, DP(ctx->sys)->props, n, value);
	}
	else {
		knh_bytes_t n = B(dname);
		knh_index_t loc = knh_bytes_rindex(n, '.');
		knh_String_t *name = new_T(dname + (loc+1));
		knh_class_t cid = CLASS_Any;
		if(loc != -1) {
			if(ctx->gma != NULL && IS_Gamma(ctx->gma)) {
				cid = knh_NameSpace_getcid(ctx, knh_getGammaNameSpace(ctx), knh_bytes_first(n, loc));
			}
			else {
				cid = knh_getcid(ctx, knh_bytes_first(n, loc));
			}
			if(cid == CLASS_unknown) {
				DBG_P("unknown class const: %s", dname);
				cid = CLASS_Any;
			}
		}
		knh_addClassConst(ctx, cid, name, value);
	}
}

static void knh_loadIntData(Ctx *ctx, knh_IntData_t *data)
{
	while(data->name != NULL) {
		Object *value = UP(new_Int(ctx, CLASS_Int, data->ivalue));
		knh_addConstData(ctx, data->name, value);
		data++;
	}
}

static void knh_loadFloatData(Ctx *ctx, knh_FloatData_t *data)
{
	while(data->name != NULL) {
		Object *value = UP(new_Float(ctx, CLASS_Float, data->fvalue));
		knh_addConstData(ctx, data->name, value);
		data++;
	}
}

static void knh_loadStringData(Ctx *ctx, knh_StringData_t *data)
{
	while(data->name != NULL) {
		Object *value = UP(new_T(data->value));
		knh_addConstData(ctx, data->name, value);
		data++;
	}
}

/* ------------------------------------------------------------------------ */

#define _MAX 1024

static knh_bytes_t knh_data_tobytes(knh_data_t data)
{
	char *p = (char*)data;
	return B(p);
}

static knh_ParamArray_t *knh_loadParamArray(Ctx *ctx, knh_data_t **d, knh_flag_t hflag, int step)
{
	knh_ParamArray_t *pa = new_ParamArray(ctx);
	knh_data_t *data = (*d) + step;
	long i, psize = (long)data[0];
	long rsize = (long)data[1];
	data += 2;
	for(i = 0; i < psize+rsize; i++) {
		knh_type_t type = (data[0] < _MAX || (TYPE_This <= data[0] && data[0] <= TYPE_T3)) ?
			(knh_type_t)data[0] : knh_NameSpace_gettype(ctx, knh_getGammaNameSpace(ctx), knh_data_tobytes(data[0]));
		knh_fieldn_t fn = (data[1] < _MAX) ?
			(knh_fieldn_t)data[1] : knh_getfnq(ctx, knh_data_tobytes(data[1]), FN_NEWID);
		knh_param_t p = {type, fn};
		knh_ParamArray_add(ctx, pa, p);
		data += 2;
	}
	pa->psize = (knh_ushort_t)psize;
	pa->rsize = (knh_ushort_t)rsize;
	*d = data;
	pa->h.flag |= hflag;
	return pa;
}

#define _CID(d)  (d < _MAX) ? (knh_class_t)(d) : knh_NameSpace_getcid(ctx, knh_getGammaNameSpace(ctx), knh_data_tobytes(d))
#define _EXPTID(d)  (d < _MAX) ? (knh_expt_t)(d) : knh_geteid(ctx, knh_data_tobytes(d), EXPT_newid)

static void knh_loadData(Ctx *ctx, knh_data_t *data, knh_ParamArray_t **buf)
{
	size_t c = 0;
	while(1) {
		long datatype = (long)data[0]; data++;
		switch(datatype) {
		case DATA_END: return;
		case DATA_STRUCT: {
			knh_class_t cid = _CID(data[0]);
			knh_ObjectCSPI_t *cspi = (knh_ObjectCSPI_t*)data[1];
			knh_ClassTable_t *t = pClassTable(ctx, cid);
			t->cspi = cspi;
			t->cflag = (knh_flag_t)data[2];
			t->oflag = FLAG_oflag(t->cflag);
			t->size = cspi->size;
			data += 3;
			break;
		}
		//{"Object", FLAG_Object, CLASS_ObjectField, CLASS_Object, a},
		case DATA_CLASS: {
			char *name = (char*)data[0];
			knh_class_t cid = new_ClassId(ctx);
			knh_ClassTable_t *t = pClassTable(ctx, cid);
			t->cflag = (knh_flag_t)data[1];
			t->oflag = FLAG_oflag(t->cflag);
			t->bcid = _CID(data[2]);
			if(cid != t->bcid) {
				t->cspi = ClassTable(t->bcid).cspi;
				t->size =  ClassTable(t->bcid).size;
			}
			t->supcid = _CID(data[3]);
			t->offset = 0;
			knh_setClassName(ctx, cid, new_T(name), NULL);
			DBG_ASSERT(t->methods == NULL);
			if(data[4] > 0) {
				KNH_INITv(t->methods, new_Array0(ctx, data[4]));
			}
			else {
				KNH_INITv(t->methods, KNH_EMPTYLIST);
			}
			if(t->tmaps == NULL) {
				KNH_INITv(t->tmaps, KNH_EMPTYLIST);
			}
			data += 5;
			break;
		}
		case DATA_CPARAM: {
			knh_class_t cid = _CID(data[0]);
			knh_ClassTable_t *t = pClassTable(ctx, cid);
			knh_ParamArray_t *mp = knh_loadParamArray(ctx, &data, 0/*hflag*/, +1);
			KNH_ASSERT(t->cparam == NULL);
			KNH_INITv(t->cparam, mp);
			break;
		}
		case DATA_GENERICS: {
			knh_class_t bcid = _CID(data[0]);
			knh_ParamArray_t *mp = knh_loadParamArray(ctx, &data, 0/*hflag*/, +1);
			knh_addGenericsClass(ctx, CLASS_newid, bcid, mp);
			break;
		}
		case DATA_EXPT: {
			char *name = (char*)data[0];
			knh_flag_t flag = (knh_flag_t)data[1];
			knh_expt_t eid = _EXPTID(data[2]);
			knh_expt_t pid = _EXPTID(data[3]);
			knh_addException(ctx, flag, eid, new_T(name), pid);
			data += 4;
			break;
		}
		//{Int_opPREV, FLAG_Method_Const, CLASS_Int, MN_opPREV, 0, 20, NULL},
		//{cid, mn, flag, func, mpidx, psize, rsize}
		case DATA_METHOD0:
		case DATA_METHOD : {
			knh_class_t cid = _CID(data[0]);
			knh_methodn_t mn = (datatype == DATA_METHOD)
				? knh_getmn(ctx, knh_data_tobytes(data[1]), MN_NEWID) : (knh_methodn_t)data[1];
			knh_flag_t flag = (knh_flag_t)data[2];
			knh_Fmethod func = (knh_Fmethod)data[3];
			knh_Method_t *mtd;
			if(knh_class_isSingleton(cid)) {
				flag = flag | FLAG_Method_Static;
			}
			mtd = new_Method(ctx, flag, cid, mn, func);
			knh_Array_add(ctx, ClassTable(cid).methods, mtd);
			data += 4;
			if(datatype == DATA_METHOD0) {
				KNH_SETv(ctx, DP(mtd)->mp, buf[data[0]]);
				DBG_ASSERT(IS_ParamArray(DP(mtd)->mp));
				data++;
			}
			else {
				knh_ParamArray_t *mp = knh_loadParamArray(ctx, &data, 0/*hflag*/, +0);
				KNH_SETv(ctx, DP(mtd)->mp, mp);
			}
			break;
		}
		case DATA_TCAST: { //{scid, tcid, flag, func},
			knh_class_t scid = _CID(data[0]);
			knh_class_t tcid = _CID(data[1]);
			knh_flag_t  flag = (knh_flag_t)data[2];
			knh_Ftranslator func = (knh_Ftranslator)data[3];
			knh_addTranslatorFunc(ctx, flag, scid, tcid, func, KNH_NULL);
			data += 4;
			break;
		}
		case DATA_PARAM: {
			knh_flag_t hflag = (knh_flag_t)data[0];
			data++;
			buf[c] = knh_loadParamArray(ctx, &data, hflag, +0);
			c++;
			break;
		}/*case DATA_PARAM*/
		default :
			KNH_SYSLOG(ctx, LOG_EMERG, "LoadingDataPanic", "unknown dataype=%d", (int)datatype);
		}/*switch*/
	}
}

/* ------------------------------------------------------------------------ */

static void knh_setRegexSPI(Ctx *ctx, const knh_RegexSPI_t *spi)
{
	KNH_SYSLOG(ctx, LOG_NOTICE, "LoadedRegexSPI", "*name=%s", spi->name);
	DP(DP(ctx->gma)->ns)->regexSPI = spi;
}

static void knh_setShellSPI(Ctx *ctx, const knh_ShellSPI_t *spi, int isOVERRIDE)
{
	KNH_SYSLOG(ctx, LOG_NOTICE, "LoadedShellSPI", "*name=%s", spi->name);
	((knh_share_t*)ctx->share)->shellSPI = spi;
}

static void knh_setEbiSPI(Ctx *ctx, const knh_EvidenceSPI_t *spi, int isOVERRIDE)
{
	KNH_SYSLOG(ctx, LOG_NOTICE, "LoadedEvidenceSPI", "*name=%s", spi->name);
	((knh_share_t*)ctx->share)->ebiSPI = spi;
}

static void knh_addPathDSPI(Ctx *ctx, char *scheme, const knh_PathDSPI_t *d, int isOVERIDE)
{
	knh_addDriverSPI(ctx, scheme, (knh_DriverSPI_t*)d);
}

static void knh_addStreamDSPI(Ctx *ctx, char *scheme, const knh_StreamDSPI_t *d, int isOVERRIDE)
{
	knh_addDriverSPI(ctx, scheme, (knh_DriverSPI_t*)d);
}

static void knh_addQueryDSPI(Ctx *ctx, char *scheme, const knh_QueryDSPI_t *d, int isOVERRIDE)
{
	knh_addDriverSPI(ctx, scheme, (knh_DriverSPI_t*)d);
}

static void knh_addConverterDSPI(Ctx *ctx, char *scheme, const knh_ConverterDSPI_t *d, int isOVERRIDE)
{
	knh_addDriverSPI(ctx, scheme, (knh_DriverSPI_t*)d);
}

/* ------------------------------------------------------------------------ */

const knh_PackageLoaderAPI_t* knh_getPackageAPI(void)
{
	static knh_PackageLoaderAPI_t exports = {
		knh_loadData,
		knh_loadIntData, knh_loadFloatData, knh_loadStringData,
		knh_setRegexSPI,
		knh_setShellSPI,
		knh_setEbiSPI,
		knh_addPathDSPI, knh_addStreamDSPI, knh_addQueryDSPI,
		knh_addConverterDSPI,
	};
	return &exports;
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
