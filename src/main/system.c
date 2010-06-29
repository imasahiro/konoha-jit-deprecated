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

#define USE_STEXT 1
#define USE_B     1
#define USE_bytes_first       1
#define USE_bytes_index       1
#define USE_bytes_last        1
#define USE_bytes_startsWith  1
#define USE_bytes_endsWith    1

#define USE_cwb_open      1
#define USE_cwb_tobytes   1

#include"commons.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ======================================================================== */
/* [properties] */

static void knh_cwb_nzenvkey(Ctx *ctx, knh_cwb_t *cwb, knh_bytes_t t)
{
	size_t i;
	for(i = 0; i < t.len; i++) {
		knh_Bytes_putc(ctx, (cwb->ba), toupper(t.buf[i]));
	}
}

/* ------------------------------------------------------------------------ */

knh_String_t* knh_getPropertyNULL(Ctx *ctx, knh_bytes_t key)
{
	if(knh_bytes_startsWith(key, STEXT("env."))) {
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		knh_cwb_nzenvkey(ctx, cwb, knh_bytes_last(key, 4));
		char *v = knh_getenv(knh_cwb_tochar(ctx, cwb));
		knh_cwb_close(cwb);
		if(v == NULL) return NULL;
		return new_S(ctx, B(v));
	}
	return (knh_String_t*)knh_DictMap_getNULL(ctx,  DP(ctx->sys)->props, key);
}

/* ------------------------------------------------------------------------ */

void knh_setProperty(Ctx *ctx, knh_String_t *key, Any *value)
{
	knh_DictMap_set_(ctx, DP(ctx->sys)->props, key, value);
}

/* ------------------------------------------------------------------------ */

KNHAPI(void) knh_setPropertyText(Ctx *ctx, char *key, char *value)
{
	knh_String_t *k = new_T(key);
	knh_String_t *v = new_T(value);
	knh_DictMap_set_(ctx, DP(ctx->sys)->props, k, UP(v));
}

/* ======================================================================== */
/* [Constant Value] */

Object *knh_getClassConstNULL(Ctx *ctx, knh_class_t cid, knh_bytes_t name)
{
	DBG_ASSERT_cid(cid);
	if(ClassTable(cid).constDictMap == NULL) return NULL;
	knh_DictMap_t *cmap = ClassTable(cid).constDictMap;
	Object *value = NULL;
	KNH_LOCK(ctx, LOCK_SYSTBL, NULL);
	int res = knh_DictMap_index(cmap, name);
	if(res != -1) {
		value = knh_DictMap_valueAt(cmap, res);
	}
	KNH_UNLOCK(ctx, LOCK_SYSTBL, NULL);
	return value;
}

/* ------------------------------------------------------------------------ */

int knh_addClassConst(Ctx *ctx, knh_class_t cid, knh_String_t* name, Object *value)
{
	int ret;
	knh_DictMap_t *cmap = ClassTable(cid).constDictMap;
	DBG_ASSERT_cid(cid);
	if(cmap == NULL) {
		knh_ClassTable_t *t = pClassTable(ctx, cid);
		cmap = new_DictMap0(ctx, 0);
		KNH_INITv(t->constDictMap, cmap);
	}
	DBG_ASSERT(IS_DictMap(cmap));
	KNH_LOCK(ctx, LOCK_SYSTBL, NULL);
	int idx = knh_DictMap_index(cmap, S_tobytes(name));
	if(idx != -1) {
		ret =  0;
		goto L_UNLOCK;
	}
	knh_DictMap_append(ctx, cmap, name, value);
	ret = 1;

	L_UNLOCK:
	KNH_UNLOCK(ctx, LOCK_SYSTBL, NULL);
	return ret;
}


/* ======================================================================== */
/* [SystemConst] */

Object *knh_getSystemConst(Ctx *ctx, int n)
{
	switch(n) {
		case KNH_SYS_CTX:     return (Object*)ctx;
		case KNH_SYS_CTXIN:   return (Object*)((ctx)->in);
		case KNH_SYS_CTXOUT:  return (Object*)((ctx)->out);
		case KNH_SYS_CTXERR:  return (Object*)((ctx)->err);
		case KNH_SYS_STDIN:   return (Object*)(DP((ctx)->sys)->in);
		case KNH_SYS_STDOUT:  return (Object*)(DP((ctx)->sys)->out);
		case KNH_SYS_STDERR:  return (Object*)(DP((ctx)->sys)->err);
		case KNH_SYS_OS:      return (Object*)knh_getClassDefaultValue(ctx, CLASS_System);
		case KNH_SYS_SCRIPT:  return (Object*)ctx->script;
	}
	DBG_P("unknown system const n=%d", n);
	return KNH_NULL;
}

/* ======================================================================== */
/* [tfieldn, tmethodn] */

knh_fieldn_t knh_addname(Ctx *ctx, knh_String_t *s, knh_Fdictset f)
{
	knh_SystemEX_t *b = DP(ctx->sys);
	size_t n = knh_DictSet_size(b->nameDictSet);
	if(n == b->hnameinfo[-1].capacity) {
		b->hnameinfo = knh_hrealloc(ctx, b->hnameinfo, n * 2);
	}
	DBG_ASSERT(n < b->hnameinfo[-1].capacity);
	KNH_INITv(b->nameinfo[n].name, s);
	if(unlikely(!(n+1 < K_FLAG_MN_SETTER))) {  /* Integer overflowed */
		KNH_SYSLOG(ctx, LOG_CRIT, "TooManyNames", "last nameid(fn)=%d < %d", (int)(n+1), (int)K_FLAG_MN_SETTER);
	}
	f(ctx, b->nameDictSet, s, n + 1);
	return (knh_fieldn_t)(n);
}

static knh_fieldn_t knh_getname(Ctx *ctx, knh_bytes_t n, knh_fieldn_t def)
{
	KNH_LOCK(ctx, LOCK_SYSTBL, NULL);
	knh_index_t idx = knh_DictSet_index(DP(ctx->sys)->nameDictSet, n);
	if(idx == -1) {
		if(def == FN_NEWID) {
			idx = knh_addname(ctx, new_S(ctx, n), knh_DictSet_set);
		}
		else {
			idx = def - MN_OPSIZE;
		}
	}
	else {
		idx = knh_DictSet_valueAt(DP(ctx->sys)->nameDictSet, idx) - 1;
	}
	KNH_UNLOCK(ctx, LOCK_SYSTBL, NULL);
	return (knh_fieldn_t)idx + MN_OPSIZE;
}

knh_NameInfo_t *knh_getnameinfo(Ctx *ctx, knh_fieldn_t fn)
{
	size_t n = (FN_UNMASK(fn) - MN_OPSIZE);
	DBG_(
		size_t size = knh_DictSet_size(DP(ctx->sys)->nameDictSet);
		DBG_ASSERT(n < size);
	);
	return DP(ctx->sys)->nameinfo + n;
}

///* ------------------------------------------------------------------------ */
//
//int knh_bytes_isOptionalMT(knh_bytes_t t)
//{
//	DBG_ASSERT(t.buf[0] != '%');
//	return (isdigit(t.buf[0]) || knh_bytes_index(t, '%') > 0);
//}
//
///* ------------------------------------------------------------------------ */
//
//knh_bytes_t knh_bytes_mtoption(knh_bytes_t t)
//{
//	size_t i = 0;
//	while(isdigit(t.buf[i]) || t.buf[i] == '.') i++;
//	t.len = i;
//	return t;
//}

/* ======================================================================== */
/* [fn] */

knh_String_t *knh_getFieldName(Ctx *ctx, knh_fieldn_t fn)
{
	return knh_getnameinfo(ctx, fn)->name;
}

/* ------------------------------------------------------------------------ */

knh_fieldn_t knh_getfnq(Ctx *ctx, knh_bytes_t tname, knh_fieldn_t def)
{
	knh_fieldn_t mask = 0;
	knh_index_t idx = knh_bytes_index(tname, ':');
	if(idx > 0) {
		tname = knh_bytes_first(tname, idx);
	}
	else if(knh_bytes_startsWith(tname, STEXT("super."))) {
		mask = (def == FN_NONAME) ? 0 : K_FLAG_FN_SUPER;
		tname = knh_bytes_last(tname, 6);
	}
	else if(!knh_bytes_endsWith(tname, STEXT("__"))) {
		if(tname.buf[0] == '_' && def != FN_NONAME) {
			mask = K_FLAG_FN_U1;
			tname = knh_bytes_last(tname, 1);
		}
		if(tname.buf[0] == '_' && def != FN_NONAME) {
			mask = K_FLAG_FN_U2;
			tname = knh_bytes_last(tname, 1);
		}
		while(tname.buf[0] == '_') {
			tname = knh_bytes_last(tname, 1);
		}
	}
	return knh_getname(ctx, tname, def) | mask;
}

/* ------------------------------------------------------------------------ */
/* [methodn] */

static knh_bytes_t knh_bytes_skipFMTOPT(knh_bytes_t t)
{
	size_t i;
	for(i = 1; i < t.len; i++) {
		if(isalnum(t.buf[i])) break;
	}
	t.buf = t.buf + i;
	t.len = t.len - i;
	return t;
}

knh_methodn_t knh_getmn(Ctx *ctx, knh_bytes_t tname, knh_methodn_t def)
{
	knh_fieldn_t mask = 0;
	if(tname.buf[0] == '%') {
		tname = knh_bytes_skipFMTOPT(tname);
		if(def != MN_NONAME) mask = K_FLAG_MN_FMT;
	}
	else if(tname.buf[0] == 'i' && tname.buf[1] == 's') { /* is => get */
		tname = knh_bytes_last(tname, 2);
		if(def != MN_NONAME) mask = K_FLAG_MN_ISBOOL;
	}
	else if(tname.buf[0] == 'g' && tname.buf[1] == 'e' && tname.buf[2] == 't') {
		tname = knh_bytes_last(tname, 3);
		if(def != MN_NONAME) mask = K_FLAG_MN_GETTER;
	}
	else if(tname.buf[0] == 's' && tname.buf[1] == 'e' && tname.buf[2] == 't') {
		tname = knh_bytes_last(tname, 3);
		if(def != MN_NONAME) mask = K_FLAG_MN_SETTER;
	}
	return knh_getname(ctx, tname, def) | mask;
}

/* ------------------------------------------------------------------------ */

char* knh_getmnname(Ctx *ctx, knh_methodn_t mn)
{
	mn = MN_toFN(mn);
	if(mn < MN_OPSIZE) {
		return knh_getopname(mn);
	}
	return S_tochar(knh_getFieldName(ctx, mn));
}

/* ======================================================================== */
/* [uri] */

knh_uri_t knh_getURI(Ctx *ctx, knh_bytes_t t)
{
	KNH_LOCK(ctx, LOCK_SYSTBL, NULL);
	knh_index_t idx = knh_DictSet_index(DP(ctx->sys)->urnDictSet, t);
	if(idx == -1) {
		knh_String_t *s = new_S(ctx, t);
		idx = knh_Array_size(DP(ctx->sys)->urns);
		knh_DictSet_set(ctx, DP(ctx->sys)->urnDictSet, s, idx);
		knh_Array_add(ctx, DP(ctx->sys)->urns, s);
		KNH_SYSLOG(ctx, LOG_INFO, "NEW_URI", "URI=%d, URN=%s", idx, S_tobytes(s));
	}
	KNH_UNLOCK(ctx, LOCK_SYSTBL, NULL);
	return (knh_uri_t)idx;
}

/* ------------------------------------------------------------------------ */

knh_String_t *knh_getURN(Ctx *ctx, knh_uri_t uri)
{
	size_t n = URI_UNMASK(uri);
	knh_Array_t *a = DP(ctx->sys)->urns;
	DBG_ASSERT(n < knh_Array_size(a));
	return (knh_String_t*)(a)->list[n];
}

/* ======================================================================== */
/* [Driver] */

void knh_addDriverSPI(Ctx *ctx, const char *scheme, knh_DriverSPI_t* p)
{
	char *name = (scheme == NULL) ? p->name : (char*)scheme;
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_write(ctx, cwb->w, B(name));
	knh_putc(ctx, cwb->w, ':');
	knh_write_ifmt(ctx, cwb->w, K_INT_FMT, p->type);
	KNH_LOCK(ctx, LOCK_SYSTBL, NULL);
	knh_DictSet_set(ctx, DP(ctx->sys)->dspiDictSet, knh_cwb_newString(ctx, cwb), (knh_uintptr_t)p);
	KNH_UNLOCK(ctx, LOCK_SYSTBL, NULL);
}

/* ------------------------------------------------------------------------ */

knh_DriverSPI_t *knh_getDriverSPI(Ctx *ctx, int type, knh_bytes_t path)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_index_t idx = knh_bytes_index(path, ':');
	if(idx == -1) {
		knh_write(ctx, cwb->w, path);
	}
	else {
		knh_write(ctx, cwb->w, knh_bytes_first(path, idx));
	}
	knh_putc(ctx, cwb->w, ':');
	knh_write_ifmt(ctx, cwb->w, K_INT_FMT, type);
	KNH_LOCK(ctx, LOCK_SYSTBL, NULL);
	knh_DriverSPI_t *p = (knh_DriverSPI_t*)knh_DictSet_get(DP(ctx->sys)->dspiDictSet, knh_cwb_tobytes(cwb));
	KNH_UNLOCK(ctx, LOCK_SYSTBL, NULL);
	knh_cwb_close(cwb);
	return p;
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
