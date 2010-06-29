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

//#define USE_STEXT 1
//#define USE_B     1
#define USE_bytes_strcmp      1
#define USE_bytes_strcasecmp  1

#include"commons.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */
/* [Map] */

knh_Map_t *new_Map(Ctx *ctx, size_t init, char *path, const knh_MapDSPI_t *dspi)
{
	knh_Map_t *m = new_(Map);
	knh_class_t cid = knh_Object_cid(m);
	m->dspi = dspi->config(ctx, knh_class_p1(cid), knh_class_p2(cid));
	m->map = m->dspi->init(ctx, init, path, NULL);
	return m;
}

/* ------------------------------------------------------------------------ */
/* [HashMap] */

#define K_HMAP_DELTA          (sizeof(knh_hmap_t) / sizeof(void*))
#define knh_map_hmap(m)      ((knh_hmap_t*)m);
#define knh_map_hentry(m)    ((knh_hentry_t**)m) + K_HMAP_DELTA;

static knh_hashcode_t knh_sfp_hashCode(Ctx *ctx, knh_sfp_t *sfp)
{
	return (knh_hashcode_t)ClassTable(knh_Object_bcid(sfp[0].o)).cspi->hashkey(ctx, sfp, KNH_FOBJECT_HASH);
}

static knh_map_t *hmap_init(Ctx *ctx, size_t init, char *path, void *option)
{
	if(init < K_HASH_INITSIZE) {
		init = K_HASH_INITSIZE;
	}
	init += K_HMAP_DELTA;
	{
		knh_hmem_t *hmem = knh_hmalloc(ctx, sizeof(knh_hentry_t*), init);
		knh_hmap_t *hmap = (knh_hmap_t*)hmem;
		hmap->hmax = init - K_HMAP_DELTA;
		hmap->size = 0;
		hmap->factor = KNH_HASH_FACTOR(hmap->hmax);
		return (knh_map_t*)hmap;
	}
}

static knh_hmap_t *hmap_rehash(Ctx *ctx, knh_hmap_t *hmap)
{
	size_t i, newhmax = hmap->hmax * 2 + 1;
	knh_hmap_t *newhmap = (knh_hmap_t*)knh_hmalloc(ctx, sizeof(knh_hentry_t*), newhmax + K_HMAP_DELTA);
	knh_hentry_t **hentry = knh_map_hentry(hmap);
	knh_hentry_t **newhentry = knh_map_hentry(newhmap);
	newhmap->hmax = newhmax;
	newhmap->size = hmap->size;
	newhmap->factor = KNH_HASH_FACTOR(newhmax);
	for(i = 0; i < hmap->size; i++) {
		knh_hentry_t *e = hentry[i];
		while(e != NULL) {
			knh_hentry_t *p = e;
			knh_hashcode_t ni = p->hcode % newhmap->hmax;
			e = e->next;
			p->next = newhentry[ni];
			newhentry[ni] = p;
		}
	}
	knh_hfree(ctx, (knh_hmem_t*)hmap);
	return newhmap;
}

static void hmap_ftr(Ctx *ctx, knh_map_t *m, knh_Ftraverse ftr)
{
	knh_hmap_t *hmap = knh_map_hmap(m);
	knh_hentry_t **hentry = knh_map_hentry(m);
	size_t i;
	for(i = 0; i < hmap->hmax; i++) {
		knh_hentry_t *e = hentry[i];
		while(e != NULL) {
			KNH_FTR(ctx, ftr, e->key);
			KNH_FTR(ctx, ftr, e->value);
			e = e->next;
		}
	}
}

static void hmap_ftrdata(Ctx *ctx, knh_map_t *m, knh_Ftraverse ftr)
{
	knh_hmap_t *hmap = knh_map_hmap(m);
	knh_hentry_t **hentry = knh_map_hentry(m);
	size_t i;
	for(i = 0; i < hmap->hmax; i++) {
		knh_hentry_t *e = hentry[i];
		while(e != NULL) {
			KNH_FTR(ctx, ftr, e->key);
			e = e->next;
		}
	}
}

static void hmap_dftr(Ctx *ctx, knh_map_t *m, knh_Ftraverse ftr)
{
	knh_hmap_t *hmap = knh_map_hmap(m);
	knh_hentry_t **hentry = knh_map_hentry(m);
	size_t i;
	for(i = 0; i < hmap->hmax; i++) {
		knh_hentry_t *e = hentry[i];
		while(e != NULL) {
			KNH_FTR(ctx, ftr, e->value);
			e = e->next;
		}
	}
}

static void hmap_dftrdata(Ctx *ctx, knh_map_t *m, knh_Ftraverse ftr)
{
}

static void hmap_free(Ctx *ctx, knh_map_t *m)
{
	knh_hmap_t *hmap = knh_map_hmap(m);
	knh_hentry_t **hentry = knh_map_hentry(m);
	size_t i;
	for(i = 0; i < hmap->hmax; i++) {
		knh_hentry_t *e = hentry[i];
		while(e != NULL) {
			knh_hentry_t *p = e;
			e = e->next;
			KNH_FREE(ctx, p, sizeof(knh_hentry_t));
		}
	}
	knh_hfree(ctx, (knh_hmem_t*)m);
}

static knh_bool_t hmap_get(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp, knh_sfp_t *rsfp)
{
	knh_hmap_t *hmap = knh_map_hmap(m);
	knh_hentry_t **hentry = knh_map_hentry(m);
	knh_Object_t *key = ksfp[0].o;
	knh_hashcode_t hcode = knh_sfp_hashCode(ctx, ksfp);
	size_t idx = hcode % hmap->hmax;
	knh_hentry_t *e = hentry[idx];
	while(e != NULL) {
		if(e->hcode == hcode && knh_Object_compareTo(ctx, e->key, key) == 0) {
			KNH_SETv(ctx, rsfp[0].o, e->value);
			return 1;
		}
		e = e->next;
	}
	return 0;
}

static knh_bool_t hmap_dget(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp, knh_sfp_t *rsfp)
{
	knh_hmap_t *hmap = knh_map_hmap(m);
	knh_hentry_t **hentry = knh_map_hentry(m);
	knh_hashcode_t hcode = (knh_hashcode_t)ksfp[0].data;
	size_t idx = hcode % hmap->hmax;
	knh_hentry_t *e = hentry[idx];
	while(e != NULL) {
		if(ksfp[0].data == e->dkey) {
			KNH_SETv(ctx, rsfp[0].o, e->value);
			return 1;
		}
		e = e->next;
	}
	return 0;
}

static knh_bool_t hmap_getdata(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp, knh_sfp_t *rsfp)
{
	knh_hmap_t *hmap = knh_map_hmap(m);
	knh_hentry_t **hentry = knh_map_hentry(m);
	knh_Object_t *key = ksfp[0].o;
	knh_hashcode_t hcode = knh_sfp_hashCode(ctx, ksfp);
	size_t idx = hcode % hmap->hmax;
	knh_hentry_t *e = hentry[idx];
	while(e != NULL) {
		if(e->hcode == hcode && knh_Object_compareTo(ctx, e->key, key) == 0) {
			rsfp[0].data = e->dvalue;
			return 1;
		}
		e = e->next;
	}
	rsfp[0].data = 0;
	return 0;
}

static knh_bool_t hmap_dgetdata(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp, knh_sfp_t *rsfp)
{
	knh_hmap_t *hmap = knh_map_hmap(m);
	knh_hentry_t **hentry = knh_map_hentry(m);
	knh_hashcode_t hcode = (knh_hashcode_t)ksfp[0].data;
	size_t idx = hcode % hmap->hmax;
	knh_hentry_t *e = hentry[idx];
	while(e != NULL) {
		if(ksfp[0].data == e->dkey) {
			rsfp[0].data = e->dvalue;
			return 1;
		}
		e = e->next;
	}
	return 0;
}

static void hmap_set(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp, knh_sfp_t *vsfp)
{
	knh_hmap_t *hmap = knh_map_hmap(m);
	knh_hentry_t **hentry = knh_map_hentry(m);
	knh_Object_t *key = ksfp[0].o;
	knh_hashcode_t hcode = knh_sfp_hashCode(ctx, ksfp);
	size_t idx = hcode % hmap->hmax;
	knh_hentry_t *e = hentry[idx];
	while(e != NULL) {
		if(e->hcode == hcode && knh_Object_compareTo(ctx, e->key, key) == 0) {
			KNH_SETv(ctx, e->value, vsfp[0].o);
			return;
		}
		e = e->next;
	}
	if(hmap->size > hmap->factor) {
		hmap = hmap_rehash(ctx, hmap);
		hentry = knh_map_hentry(hmap);
	}
	e = (knh_hentry_t*)KNH_MALLOC(ctx, sizeof(knh_hentry_t));
	e->hcode = hcode;
	KNH_INITv(e->key, key);
	KNH_INITv(e->value, vsfp[0].o);
	e->next = hentry[idx];
	hentry[idx] = e;
	hmap->size += 1;
}

static void hmap_setdata(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp, knh_sfp_t *vsfp)
{
	knh_hmap_t *hmap = knh_map_hmap(m);
	knh_hentry_t **hentry = knh_map_hentry(m);
	knh_Object_t *key = ksfp[0].o;
	knh_hashcode_t hcode = knh_sfp_hashCode(ctx, ksfp);
	size_t idx = hcode % hmap->hmax;
	knh_hentry_t *e = hentry[idx];
	while(e != NULL) {
		if(e->hcode == hcode && knh_Object_compareTo(ctx, e->key, key) == 0) {
			e->dvalue = vsfp[0].data;
			return;
		}
		e = e->next;
	}
	if(hmap->size > hmap->factor) {
		hmap = hmap_rehash(ctx, hmap);
		hentry = knh_map_hentry(hmap);
	}
	e = (knh_hentry_t*)KNH_MALLOC(ctx, sizeof(knh_hentry_t));
	e->hcode = hcode;
	KNH_INITv(e->key, key);
	e->dvalue = vsfp[0].data;
	e->next = hentry[idx];
	hentry[idx] = e;
	hmap->size += 1;
}

static void hmap_dset(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp, knh_sfp_t *vsfp)
{
	knh_hmap_t *hmap = knh_map_hmap(m);
	knh_hentry_t **hentry = knh_map_hentry(m);
	knh_hashcode_t hcode = (knh_hashcode_t)ksfp[0].data;
	size_t idx = hcode % hmap->hmax;
	knh_hentry_t *e = hentry[idx];
	while(e != NULL) {
		if(e->dkey == ksfp[0].data) {
			KNH_SETv(ctx, e->value, vsfp[0].o);
			return;
		}
		e = e->next;
	}
	if(hmap->size > hmap->factor) {
		hmap = hmap_rehash(ctx, hmap);
		hentry = knh_map_hentry(hmap);
	}
	e = (knh_hentry_t*)KNH_MALLOC(ctx, sizeof(knh_hentry_t));
	e->hcode = hcode;
	e->dkey = ksfp[0].data;
	KNH_INITv(e->value, vsfp[0].o);
	e->next = hentry[idx];
	hentry[idx] = e;
	hmap->size += 1;
}

static void hmap_dsetdata(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp, knh_sfp_t *vsfp)
{
	knh_hmap_t *hmap = knh_map_hmap(m);
	knh_hentry_t **hentry = knh_map_hentry(m);
	knh_hashcode_t hcode = (knh_hashcode_t)ksfp[0].data;
	size_t idx = hcode % hmap->hmax;
	knh_hentry_t *e = hentry[idx];
	while(e != NULL) {
		if(e->dkey == ksfp[0].data) {
			e->dvalue = vsfp[0].data;
			return;
		}
		e = e->next;
	}
	if(hmap->size > hmap->factor) {
		hmap = hmap_rehash(ctx, hmap);
		hentry = knh_map_hentry(hmap);
	}
	e = (knh_hentry_t*)KNH_MALLOC(ctx, sizeof(knh_hentry_t));
	e->hcode = hcode;
	e->dkey = ksfp[0].data;
	e->dvalue = vsfp[0].data;
	e->next = hentry[idx];
	hentry[idx] = e;
	hmap->size += 1;
}

static void hmap_remove(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp)
{
	KNH_TODO("HashMap.remove");
}

static void hmap_dremove(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp)
{
	KNH_TODO("HashMap.remove");
}

static size_t hmap_size(Ctx *ctx, knh_map_t* m)
{
	knh_hmap_t *hmap = knh_map_hmap(m);
	return hmap->size;
}

static knh_bool_t hmap_setIterator(Ctx *ctx, knh_map_t* m, knh_Iterator_t *it)
{
	KNH_TODO("hmap_setIterator");
	return 0;
}


static const knh_MapDSPI_t* hmap_config(Ctx *ctx, knh_class_t p1, knh_class_t p2);

static const struct knh_MapDSPI_t MAP_ObjectObject = {
	K_MAP_DSPI, "hash",
	hmap_config,
	hmap_init, hmap_ftr, hmap_free,
	hmap_get, hmap_set, hmap_remove, hmap_size,
	hmap_setIterator,
};

static const struct knh_MapDSPI_t MAP_ObjectData = {
	K_MAP_DSPI, "hash",
	hmap_config, hmap_init, hmap_ftrdata, hmap_free,
	hmap_getdata, hmap_setdata, hmap_remove, hmap_size,
	hmap_setIterator,
};

static const struct knh_MapDSPI_t MAP_DataObject = {
	K_MAP_DSPI, "hash",
	hmap_config, hmap_init, hmap_dftr, hmap_free,
	hmap_dget, hmap_dset, hmap_dremove, hmap_size,
	hmap_setIterator,
};

static const struct knh_MapDSPI_t MAP_DataData = {
	K_MAP_DSPI, "hash",
	hmap_config, hmap_init, hmap_dftrdata, hmap_free,
	hmap_dgetdata, hmap_dsetdata, hmap_dremove, hmap_size,
	hmap_setIterator,
};

static const knh_MapDSPI_t* hmap_config(Ctx *ctx, knh_class_t p1, knh_class_t p2)
{
	if(IS_Tunbox(p1)) {
		if(IS_Tunbox(p2)) {
			return &MAP_DataData;
		}
		return &MAP_DataObject;
	}
	else {
		if(IS_Tunbox(p2)) {
			return &MAP_ObjectData;
		}
		return &MAP_ObjectObject;
	}
}

///* ------------------------------------------------------------------------ */
///* dmap */
//
//#define K_DMAP_DELTA    2
//#define knh_map_dmap(m)        ((knh_dmap_t*)m)
//#define knh_map_dentry(m)      (&(((knh_dentry_t)m)+K_DMAP_DELTA))
//
//static knh_map_t *dmap_init(Ctx *ctx, size_t init, char *path, void *option)
//{
//	knh_dmap_t *dmap = (knh_dmap_t*)knh_hmalloc(ctx, sizeof(knh_dentry_t), init + K_DMAP_DELTA);
//	dmap->size = 0;
//	dmap->sorted = 0;
//	dmap->fcmp = knh_bytes_strcmp;
//	return dmap;
//}
//
//static knh_dmap_t *dmap_resize(Ctx *ctx, knh_dmap_t *dmap)
//{
//}
//
//static void dmap_ftr(Ctx *ctx, knh_map_t *m, knh_Ftraverse ftr)
//{
//	knh_dmap_t *dmap = knh_map_dmap(m);
//	knh_dentry_t *dentry = knh_map_dentry(m);
//	size_t i;
//	for(i = 0; i < dmap->size; i++) {
//		KNH_FTR(ctx, ftr, dentry[i].okey);
//		KNH_FTR(ctx, ftr, dentry[i].value);
//	}
//}
//
//static void dmap_ftrdata(Ctx *ctx, knh_map_t *m, knh_Ftraverse ftr)
//{
//	knh_dmap_t *dmap = knh_map_dmap(m);
//	knh_dentry_t *dentry = knh_map_dentry(m);
//	size_t i;
//	for(i = 0; i < dmap->size; i++) {
//		KNH_FTR(ctx, ftr, dentry[i].okey);
//	}
//}
//
//static void dmap_dftr(Ctx *ctx, knh_map_t *m, knh_Ftraverse ftr)
//{
//	knh_dmap_t *dmap = knh_map_dmap(m);
//	knh_dentry_t *dentry = knh_map_dentry(m);
//	size_t i;
//	for(i = 0; i < dmap->size; i++) {
//		KNH_FTR(ctx, ftr, dentry[i].value);
//	}
//}
//
//static void dmap_dftrdata(Ctx *ctx, knh_map_t *m, knh_Ftraverse ftr)
//{
//}
//
//static void dmap_free(Ctx *ctx, knh_map_t *m)
//{
//	knh_hfree(ctx, (knh_hmem_t*)m);
//}
//
///* ------------------------------------------------------------------------ */
//
//#define UNSORTED_MAXSIZ 8
//
//static knh_index_t knh_dentry_index_strcmp(knh_dentry_t *a, size_t sp, size_t ep, knh_bytes_t key)
//{
//	L_TAIL:;
//	if(ep - sp < UNSORTED_MAXSIZ) {
//		size_t i;
//		for(i = sp; i < ep; i++) {
//			if(fcmp(S_tobytes(a[i].key), key) == 0) return i;
//		}
//		return -1;
//	}
//	else {
//		size_t cp = KNH_MID(sp, ep);
//		int i = fcmp(key, S_tobytes(a[cp].key));
//		if(i == 0) {
//			return cp;
//		}
//		else if(i < 0) {
//			ep = cp;
//		}else {
//			sp = cp + 1;
//		}
//		goto L_TAIL;
//	}
//}
//
///* ------------------------------------------------------------------------ */
//
//static knh_index_t knh_dentry_index(knh_dentry_t *a, size_t sorted, size_t size, knh_Fbytescmp fcmp, knh_bytes_t key)
//{
//	DBG_ASSERT(a != NULL);
//	size_t sorted = knh_dict_sorted(a);
//	if(size - sorted >= UNSORTED_MAXSIZ) {
//		knh_dict_sort(a, size);
//		return knh_dict_findb_between(a, fcmp, key, 0, size);
//	}
//	else {
//		knh_index_t loc = knh_dict_findb_between(a, fcmp, key, 0, sorted);
//		return (loc == -1) ? knh_dict_findb_between(a, fcmp, key, sorted, size) : loc;
//	}
//}
//
//
//
//static knh_bool_t dmap_sget(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp, knh_sfp_t *rsfp)
//{
//	knh_dmap_t *dmap = knh_map_dmap(m);
//	knh_dentry_t *dentry = knh_map_dentry(m);
//	knh_String_t *key = ksfp[0].s;
//
//}
//
//static knh_bool_t dmap_dget(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp, knh_sfp_t *rsfp)
//{
//	knh_dmap_t *dmap = knh_map_dmap(m);
//	knh_dentry_t **dentry = knh_map_dentry(m);
//	knh_hashcode_t hcode = (knh_hashcode_t)ksfp[0].data;
//	size_t idx = hcode % dmap->hmax;
//	knh_dentry_t *e = dentry[idx];
//	while(e != NULL) {
//		if(ksfp[0].data == e->dkey) {
//			KNH_SETv(ctx, rsfp[0].o, e->value);
//			return 1;
//		}
//		e = e->next;
//	}
//	return 0;
//}
//
//static knh_bool_t dmap_getdata(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp, knh_sfp_t *rsfp)
//{
//	knh_dmap_t *dmap = knh_map_dmap(m);
//	knh_dentry_t **dentry = knh_map_dentry(m);
//	knh_Object_t *key = ksfp[0].o;
//	knh_hashcode_t hcode = knh_sfp_hashCode(ctx, ksfp);
//	size_t idx = hcode % dmap->hmax;
//	knh_dentry_t *e = dentry[idx];
//	while(e != NULL) {
//		if(e->hcode == hcode && knh_Object_compareTo(ctx, e->key, key) == 0) {
//			rsfp[0].data = e->dvalue;
//			return 1;
//		}
//		e = e->next;
//	}
//	rsfp[0].data = 0;
//	return 0;
//}
//
//static knh_bool_t dmap_dgetdata(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp, knh_sfp_t *rsfp)
//{
//	knh_dmap_t *dmap = knh_map_dmap(m);
//	knh_dentry_t **dentry = knh_map_dentry(m);
//	knh_hashcode_t hcode = (knh_hashcode_t)ksfp[0].data;
//	size_t idx = hcode % dmap->hmax;
//	knh_dentry_t *e = dentry[idx];
//	while(e != NULL) {
//		if(ksfp[0].data == e->dkey) {
//			rsfp[0].data = e->dvalue;
//			return 1;
//		}
//		e = e->next;
//	}
//	return 0;
//}
//
//static void dmap_set(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp, knh_sfp_t *vsfp)
//{
//	knh_dmap_t *dmap = knh_map_dmap(m);
//	knh_dentry_t **dentry = knh_map_dentry(m);
//	knh_Object_t *key = ksfp[0].o;
//	knh_hashcode_t hcode = knh_sfp_hashCode(ctx, ksfp);
//	size_t idx = hcode % dmap->hmax;
//	knh_dentry_t *e = dentry[idx];
//	while(e != NULL) {
//		if(e->hcode == hcode && knh_Object_compareTo(ctx, e->key, key) == 0) {
//			KNH_SETv(ctx, e->value, vsfp[0].o);
//			return;
//		}
//		e = e->next;
//	}
//	if(dmap->size > dmap->factor) {
//		dmap = dmap_rehash(ctx, dmap);
//		dentry = knh_map_dentry(dmap);
//	}
//	e = (knh_dentry_t*)KNH_MALLOC(ctx, sizeof(knh_dentry_t));
//	e->hcode = hcode;
//	KNH_INITv(e->key, key);
//	KNH_INITv(e->value, vsfp[0].o);
//	e->next = dentry[idx];
//	dentry[idx] = e;
//	dmap->size += 1;
//}
//
//static void dmap_setdata(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp, knh_sfp_t *vsfp)
//{
//	knh_dmap_t *dmap = knh_map_dmap(m);
//	knh_dentry_t **dentry = knh_map_dentry(m);
//	knh_Object_t *key = ksfp[0].o;
//	knh_hashcode_t hcode = knh_sfp_hashCode(ctx, ksfp);
//	size_t idx = hcode % dmap->hmax;
//	knh_dentry_t *e = dentry[idx];
//	while(e != NULL) {
//		if(e->hcode == hcode && knh_Object_compareTo(ctx, e->key, key) == 0) {
//			e->dvalue = vsfp[0].data;
//			return;
//		}
//		e = e->next;
//	}
//	if(dmap->size > dmap->factor) {
//		dmap = dmap_rehash(ctx, dmap);
//		dentry = knh_map_dentry(dmap);
//	}
//	e = (knh_dentry_t*)KNH_MALLOC(ctx, sizeof(knh_dentry_t));
//	e->hcode = hcode;
//	KNH_INITv(e->key, key);
//	e->dvalue = vsfp[0].data;
//	e->next = dentry[idx];
//	dentry[idx] = e;
//	dmap->size += 1;
//}
//
//static void dmap_dset(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp, knh_sfp_t *vsfp)
//{
//	knh_dmap_t *dmap = knh_map_dmap(m);
//	knh_dentry_t **dentry = knh_map_dentry(m);
//	knh_hashcode_t hcode = (knh_hashcode_t)ksfp[0].data;
//	size_t idx = hcode % dmap->hmax;
//	knh_dentry_t *e = dentry[idx];
//	while(e != NULL) {
//		if(e->dkey == ksfp[0].data) {
//			KNH_SETv(ctx, e->value, vsfp[0].o);
//			return;
//		}
//		e = e->next;
//	}
//	if(dmap->size > dmap->factor) {
//		dmap = dmap_rehash(ctx, dmap);
//		dentry = knh_map_dentry(dmap);
//	}
//	e = (knh_dentry_t*)KNH_MALLOC(ctx, sizeof(knh_dentry_t));
//	e->hcode = hcode;
//	e->dkey = ksfp[0].data;
//	KNH_INITv(e->value, vsfp[0].o);
//	e->next = dentry[idx];
//	dentry[idx] = e;
//	dmap->size += 1;
//}
//
//static void dmap_dsetdata(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp, knh_sfp_t *vsfp)
//{
//	knh_dmap_t *dmap = knh_map_dmap(m);
//	knh_dentry_t **dentry = knh_map_dentry(m);
//	knh_hashcode_t hcode = (knh_hashcode_t)ksfp[0].data;
//	size_t idx = hcode % dmap->hmax;
//	knh_dentry_t *e = dentry[idx];
//	while(e != NULL) {
//		if(e->dkey == ksfp[0].data) {
//			e->dvalue = vsfp[0].data;
//			return;
//		}
//		e = e->next;
//	}
//	if(dmap->size > dmap->factor) {
//		dmap = dmap_rehash(ctx, dmap);
//		dentry = knh_map_dentry(dmap);
//	}
//	e = (knh_dentry_t*)KNH_MALLOC(ctx, sizeof(knh_dentry_t));
//	e->hcode = hcode;
//	e->dkey = ksfp[0].data;
//	e->dvalue = vsfp[0].data;
//	e->next = dentry[idx];
//	dentry[idx] = e;
//	dmap->size += 1;
//}
//
//static void dmap_remove(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp)
//{
//	KNH_TODO("HashMap.remove");
//}
//
//static void dmap_dremove(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp)
//{
//	KNH_TODO("HashMap.remove");
//}
//
//static size_t dmap_size(Ctx *ctx, knh_map_t* m)
//{
//	knh_dmap_t *dmap = knh_map_dmap(m);
//	return dmap->size;
//}
//
//static knh_bool_t dmap_setIterator(Ctx *ctx, knh_map_t* m, knh_Iterator_t *it)
//{
//	KNH_TODO("dmap_setIterator");
//	return 0;
//}
//
//
//static const knh_MapDSPI_t* dmap_config(Ctx *ctx, knh_class_t p1, knh_class_t p2);
//
//static const struct knh_MapDSPI_t MAP_ObjectObject = {
//	K_MAP_DSPI, "hash",
//	dmap_config,
//	dmap_init, dmap_ftr, dmap_free,
//	dmap_get, dmap_set, dmap_remove, dmap_size,
//	dmap_setIterator,
//};
//
//static const struct knh_MapDSPI_t MAP_ObjectData = {
//	K_MAP_DSPI, "hash",
//	dmap_config, dmap_init, dmap_ftrdata, dmap_free,
//	dmap_getdata, dmap_setdata, dmap_remove, dmap_size,
//	dmap_setIterator,
//};
//
//static const struct knh_MapDSPI_t MAP_DataObject = {
//	K_MAP_DSPI, "hash",
//	dmap_config, dmap_init, dmap_dftr, dmap_free,
//	dmap_dget, dmap_dset, dmap_dremove, dmap_size,
//	dmap_setIterator,
//};
//
//static const struct knh_MapDSPI_t MAP_DataData = {
//	K_MAP_DSPI, "hash",
//	dmap_config, dmap_init, dmap_dftrdata, dmap_free,
//	dmap_dgetdata, dmap_dsetdata, dmap_dremove, dmap_size,
//	dmap_setIterator,
//};
//
//static const knh_MapDSPI_t* dmap_config(Ctx *ctx, knh_class_t p1, knh_class_t p2)
//{
//	if(IS_Tunbox(p1)) {
//		if(IS_Tunbox(p2)) {
//			return &MAP_DataData;
//		}
//		return &MAP_DataObject;
//	}
//	else {
//		if(IS_Tunbox(p2)) {
//			return &MAP_ObjectData;
//		}
//		return &MAP_ObjectObject;
//	}
//}


/* ======================================================================== */
/* [hdict] */
/* @data */

typedef struct {
	size_t capacity;
	size_t sorted;
	void (*fdict_init)(Ctx *ctx, knh_dict_t *);
	void (*fdict_traverse)(Ctx *ctx, knh_dict_t *, knh_Ftraverse ftr);
	int  (*fdict_compar)(const void *, const void *);
	void *dummy;
} knh_hdict_t ;

/* ------------------------------------------------------------------------ */

size_t knh_dict_capacity(knh_dict_t *a)
{
	if(a == NULL) {
		return 0;
	}
	else {
		knh_hdict_t *h = ((knh_hdict_t*)a);
		return h[-1].capacity;
	}
}

static size_t knh_dict_sorted(knh_dict_t *a)
{
	if(a == NULL) {
		return 0;
	}
	else {
		knh_hdict_t *h = ((knh_hdict_t*)a);
		return h[-1].sorted;
	}
}

static void knh_dict_setsorted(knh_dict_t *a, size_t size)
{
	if(a != NULL) {
		knh_hdict_t *h = ((knh_hdict_t*)a);
		h[-1].sorted = size;
	}
}

static knh_dict_t *knh_dict_malloc(Ctx *ctx, size_t capacity,
		void (*fdict_init)(Ctx *, knh_dict_t*), void (*fdict_traverse)(Ctx*, knh_dict_t*, knh_Ftraverse),
		int (*fdict_compar)(const void*, const void*))
{
	knh_hdict_t *h;
	knh_dict_t *a;
	size_t i;
	if(!(capacity > 0)) {
		capacity = (K_SMALLPAGESIZE - sizeof(knh_hdict_t)) / sizeof(knh_dict_t);
	}
	h = (knh_hdict_t*)KNH_MALLOC(ctx, (capacity * sizeof(knh_dict_t)) + sizeof(knh_hdict_t));
	h->capacity = capacity;
	h->sorted = 0;
	h->fdict_init = fdict_init;
	h->fdict_traverse = fdict_traverse;
	h->fdict_compar = fdict_compar;
	a = (knh_dict_t*)(h+1);
	for(i = 0; i < capacity; i++) {
		fdict_init(ctx, a + i);
	}
	return a;
}

knh_dict_t* knh_dict_resize(Ctx *ctx, knh_dict_t *a, size_t newsize)
{
	knh_hdict_t *h = ((knh_hdict_t*)a) - 1, *newh = NULL;
	knh_dict_t *newa = NULL;
	size_t i, size = h->capacity;
	if (size < newsize) {
		newh = (knh_hdict_t*)KNH_MALLOC(ctx, (sizeof(knh_dict_t) * newsize) + sizeof(knh_hdict_t));
		newh->capacity = newsize;
		newh->fdict_init = h->fdict_init;
		newh->fdict_traverse = h->fdict_traverse;
		newh->sorted = h->sorted;
		newh->fdict_compar = h->fdict_compar;
		newa = (knh_dict_t*)(newh + 1);
		if(size > 0) {
			knh_memcpy(newa, a, (sizeof(knh_dict_t) * size));
		}
		for(i = size; i < newsize; i++) {
			newh->fdict_init(ctx, newa + i);
		}
		if(size > 0) {
			KNH_FREE(ctx, h, (sizeof(knh_dict_t) * size) + sizeof(knh_hdict_t));
		}
		return newa;
	}
	else if(newsize < size) {
		if(newsize > 0) {
			newh = (knh_hdict_t*)KNH_MALLOC(ctx, (sizeof(knh_dict_t) * newsize) + sizeof(knh_hdict_t));
			newh->capacity = newsize;
			newh->fdict_init = h->fdict_init;
			newh->fdict_traverse = h->fdict_traverse;
			newh->sorted = h->sorted;
			newh->fdict_compar = h->fdict_compar;
			newa = (knh_dict_t*)(newh + 1);
			knh_memcpy(newa, a, (sizeof(knh_dict_t) * size));
			for(i = newsize; i < size; i++) {
				newh->fdict_traverse(ctx, a + i, knh_Object_sweep);
			}
		}
		else {
			DBG_P("resize to 0");
			newa = NULL;
		}

		KNH_FREE(ctx, h, (sizeof(knh_dict_t) * size) + sizeof(knh_hdict_t));
		return newa;
	}
	else { /* size == newssiz */
		return a;
	}
}

/* ------------------------------------------------------------------------ */

void knh_dict_traverse(Ctx *ctx, knh_dict_t *a, knh_Ftraverse ftr)
{
	if(a != NULL) {
		knh_hdict_t *h = ((knh_hdict_t*)a) - 1;
		size_t i = 0;
		for(i = 0; i < h->capacity; i++) {
			h->fdict_traverse(ctx, a + i, ftr);
		}
	}
}

void knh_dict_free(Ctx *ctx, knh_dict_t *a)
{
	if(a != NULL) {
		knh_hdict_t *h = ((knh_hdict_t*)a) - 1;
		if(h->capacity > 0) {
			KNH_FREE(ctx, h, (h->capacity * sizeof(knh_dict_t)) + sizeof(knh_hdict_t));
		}
	}
}

/* ======================================================================== */
/* [sort] */

static void knh_dict_sort(knh_dict_t *a, size_t size)
{
	if(a != NULL) {
		knh_hdict_t *h = ((knh_hdict_t*)a) - 1;
		DBG_ASSERT(h->fdict_compar != NULL);
		knh_qsort(a, size, sizeof(knh_dict_t), h->fdict_compar);
		h->sorted = size;
	}
}

/* ======================================================================== */

#define UNSORTED_MAXSIZ 8

static
knh_index_t knh_dict_findb_between(knh_dict_t *a, knh_Fbytescmp fcmp, knh_bytes_t key, size_t sp, size_t ep)
{
	L_TAIL:;
	if(ep - sp < UNSORTED_MAXSIZ) {
		size_t i;
		for(i = sp; i < ep; i++) {
			//DBG_P("%p %d %s %s %d", a, i, S_tochar(a[i].key), key.buf, fcmp(S_tobytes(a[i].key), key));
			if(fcmp(S_tobytes(a[i].key), key) == 0) return i;
		}
		return -1;
	}
	else {
		size_t cp = KNH_MID(sp, ep);
		int i = fcmp(key, S_tobytes(a[cp].key));
		if(i == 0) {
			return cp;
		}
		else if(i < 0) {
			ep = cp;
		}else {
			sp = cp + 1;
		}
		goto L_TAIL;
	}
}

/* ------------------------------------------------------------------------ */

static knh_index_t knh_dict_index(knh_dict_t *a, size_t size, knh_Fbytescmp fcmp, knh_bytes_t key)
{
	DBG_ASSERT(a != NULL);
	size_t sorted = knh_dict_sorted(a);
	if(size - sorted >= UNSORTED_MAXSIZ) {
		knh_dict_sort(a, size);
		return knh_dict_findb_between(a, fcmp, key, 0, size);
	}
	else {
		knh_index_t loc = knh_dict_findb_between(a, fcmp, key, 0, sorted);
		return (loc == -1) ? knh_dict_findb_between(a, fcmp, key, sorted, size) : loc;
	}
}


/* ------------------------------------------------------------------------ */

void knh_DictMap_sort(knh_DictMap_t *o)
{
	if(knh_dict_sorted(o->_list) != o->size) {
		knh_dict_sort(o->_list, o->size);
	}
}

/* ------------------------------------------------------------------------ */

void knh_DictSet_sort(knh_DictSet_t *o)
{
	if(knh_dict_sorted(o->_list) != o->size) {
		knh_dict_sort(o->_list, o->size);
	}
}

/* ------------------------------------------------------------------------ */

static void knh_dictmap_finit(Ctx *ctx, knh_dict_t *p)
{
	knh_dictmape_t *e = (knh_dictmape_t*)p;
	KNH_INITv(e->key, KNH_NULL);
	KNH_INITv(e->value, KNH_NULL);
}

static void knh_dictmap_ftraverse(Ctx *ctx, knh_dict_t *p, knh_Ftraverse ftr)
{
	knh_dictmape_t *e = (knh_dictmape_t*)p;
	KNH_FTR(ctx, ftr, (e->key));
	KNH_FTR(ctx, ftr, e->value);
}

static void knh_dictset_finit(Ctx *ctx, knh_dict_t *p)
{
	knh_dictsete_t *e = (knh_dictsete_t*)p;
	KNH_INITv(e->key, KNH_NULL);
	e->value = 0;
}

static void knh_dictset_ftraverse(Ctx *ctx, knh_dict_t *p, knh_Ftraverse ftr)
{
	knh_dictsete_t *e = (knh_dictsete_t*)p;
	KNH_FTR(ctx, ftr, (e->key));
}

static int knh_dict_fcmp(const void *p, const void *p2)
{
	knh_dict_t *e = (knh_dict_t*)p;
	knh_dict_t *e2 = (knh_dict_t*)p2;
	return knh_bytes_strcmp(S_tobytes(e->key), S_tobytes(e2->key));
}

static int knh_dict_fcasecmp(const void *p, const void *p2)
{
	knh_dict_t *e = (knh_dict_t*)p;
	knh_dict_t *e2 = (knh_dict_t*)p2;
	return knh_bytes_strcasecmp(S_tobytes(e->key), S_tobytes(e2->key));
}

/* ------------------------------------------------------------------------ */

static void knh_dict_tocase(knh_dict_t *a)
{
	if(a != NULL) {
		knh_hdict_t *h = ((knh_hdict_t*)a);
		h[-1].fdict_compar = knh_dict_fcasecmp;
	}
}

/* ------------------------------------------------------------------------ */

void knh_DictMap_toIgnoreCase(knh_DictMap_t *o)
{
	knh_dict_tocase(o->_list);
	o->fcmp = knh_bytes_strcasecmp;
	knh_DictMap_setIgnoreCase(o, 1);
}

/* ======================================================================== */
/* [DictMap] */

knh_dict_t *knh_dictmap_malloc(Ctx *ctx, size_t init)
{
	return knh_dict_malloc(ctx, init, knh_dictmap_finit, knh_dictmap_ftraverse, knh_dict_fcmp);
}

/* ------------------------------------------------------------------------ */

knh_DictMap_t* new_DictMap0(Ctx *ctx, knh_intptr_t capacity)
{
	knh_DictMap_t *o = new_H(DictMap);
	o->_list = knh_dict_malloc(ctx, capacity, knh_dictmap_finit, knh_dictmap_ftraverse, knh_dict_fcmp);
	o->size = 0;
	o->fcmp = knh_bytes_strcmp;
	return o;
}

/* ------------------------------------------------------------------------ */

knh_DictMap_t* new_DictMap(Ctx *ctx, knh_class_t p1, knh_intptr_t capacity)
{
	knh_class_t cid = knh_class_P1(ctx, CLASS_DictMap, p1);
	knh_DictMap_t *o = (knh_DictMap_t*)new_hObject_(ctx, FLAG_DictMap, CLASS_DictMap, cid);
	o->_list = knh_dict_malloc(ctx, capacity, knh_dictmap_finit, knh_dictmap_ftraverse, knh_dict_fcmp);
	o->size = 0;
	o->fcmp = knh_bytes_strcmp;
	return o;
}

/* ======================================================================== */
/* [methods] */

size_t knh_DictMap_size(knh_DictMap_t *o)
{
	return o->size;
}

/* ------------------------------------------------------------------------ */

knh_String_t* knh_DictMap_keyAt(knh_DictMap_t *o, size_t n)
{
	DBG_ASSERT(n < o->size);
	return o->list[n].key;
}

/* ------------------------------------------------------------------------ */

Object* knh_DictMap_valueAt(knh_DictMap_t *o, size_t n)
{
	DBG_ASSERT(n < o->size);
	return o->list[n].value;
}

/* ------------------------------------------------------------------------ */

void knh_DictMap_removeAt(Ctx *ctx, knh_DictMap_t *o, size_t n)
{
	DBG_ASSERT(n < o->size);
	KNH_SETv(ctx, o->list[n].value, KNH_NULL);
}

/* ------------------------------------------------------------------------ */

knh_index_t knh_DictMap_index(knh_DictMap_t *o, knh_bytes_t key)
{
	return knh_dict_index(o->_list, o->size, o->fcmp, key);
}

/* ------------------------------------------------------------------------ */

static
knh_index_t knh_dict_first(knh_dict_t *a, knh_Fbytescmp fcmp, knh_bytes_t key, size_t sp, size_t ep)
{
	L_TAIL:;
	if(ep - sp < UNSORTED_MAXSIZ) {
		size_t i;
		for(i = sp; i < ep; i++) {
			int res = fcmp(S_tobytes(a[i].key), key);
			if(res >= 0) return i;
		}
		return -1;
	}
	else {
		size_t cp = KNH_MID(sp, ep);
		int i = fcmp(key, S_tobytes(a[cp].key));
		if(i == 0) {
			return cp;
		}
		else if(i < 0) {
			ep = cp;
		}else {
			sp = cp + 1;
		}
		goto L_TAIL;
	}
}

/* ------------------------------------------------------------------------ */

knh_index_t knh_DictMap_firstIndex(knh_DictMap_t *o, knh_bytes_t key)
{
	DBG_ASSERT(IS_bDictMap(o));
	knh_DictMap_sort(o);
	return knh_dict_first(o->_list, o->fcmp, key, 0, o->size);
}

/* ------------------------------------------------------------------------ */

knh_String_t *new_String__DictMap(Ctx *ctx, knh_DictMap_t *o, knh_bytes_t key)
{
	DBG_ASSERT(IS_bDictMap(o));
	knh_index_t loc = knh_dict_index(o->_list, o->size, o->fcmp, key);
	if(loc == -1) {
		return new_S(ctx, key);
	}
	else {
		return o->list[loc].key;
	}
}

///* ------------------------------------------------------------------------ */
//
//Object *knh_DictMap_get(Ctx *ctx, knh_DictMap_t *o, knh_bytes_t key)
//{
//	DBG_ASSERT(IS_bDictMap(o));
//	knh_intptr_t loc = knh_dict_index(o->_list, o->size, o->fcmp, key);
//	return (loc == -1) ? KNH_NULL : o->list[loc].value;
//}

/* ------------------------------------------------------------------------ */

Object *knh_DictMap_getNULL(Ctx *ctx, knh_DictMap_t *o, knh_bytes_t key)
{
	DBG_ASSERT(IS_bDictMap(o));
	knh_intptr_t loc = knh_dict_index(o->_list, o->size, o->fcmp, key);
	return (loc == -1) ? NULL : o->list[loc].value;
}

/* ------------------------------------------------------------------------ */

void knh_DictMap_append(Ctx *ctx, knh_DictMap_t *o, knh_String_t *key, Any *value)
{
	DBG_ASSERT(IS_bDictMap(o));
	if(o->_list == NULL) {
		o->_list = knh_dict_malloc(ctx, 8, knh_dictmap_finit, knh_dictmap_ftraverse, knh_dict_fcmp);
		o->size = 0;
	}
	else {
		size_t capacity = knh_dict_capacity(o->_list);
		if(o->size == capacity) {
			o->_list = knh_dict_resize(ctx, o->_list, capacity * 2);
		}
	}
	KNH_SETv(ctx, o->list[o->size].key, key);
	KNH_SETv(ctx, o->list[o->size].value, value);
	o->size++;
}

/* ------------------------------------------------------------------------ */

void knh_DictMap_set__b(Ctx *ctx, knh_DictMap_t *o, knh_bytes_t key, Any *v)
{
	knh_Object_t *value = v;
	int loc = knh_dict_index(o->_list, o->size, o->fcmp,key);
	knh_String_t *k = new_S(ctx, key);
	if(loc != -1) {
		KNH_SETv(ctx, o->list[loc].key, k);  /* To avoid losing key */
		KNH_SETv(ctx, o->list[loc].value, value);
	}
	else {
		knh_DictMap_append(ctx, o, k, value);
	}
}

/* ------------------------------------------------------------------------ */

void knh_DictMap_set_(Ctx *ctx, knh_DictMap_t *o, knh_String_t *key, Any *v)
{
	knh_Object_t *value = v;
	int loc = knh_dict_index(o->_list, o->size, o->fcmp, S_tobytes(key));
	if(loc != -1) {
		KNH_SETv(ctx, o->list[loc].key, key);  /* To avoid losing key */
		KNH_SETv(ctx, o->list[loc].value, value);
	}
	else {
		knh_DictMap_append(ctx, o, key, value);
	}
}

/* ------------------------------------------------------------------------ */

void knh_DictMap_remove(Ctx *ctx, knh_DictMap_t *d, knh_bytes_t key)
{
	knh_index_t loc = knh_dict_index(d->_list, d->size, d->fcmp, key);
	if(loc != -1) {
		// move removed entry to the tail
		knh_String_t *keyRemoved = d->list[loc].key;
		knh_Object_t *valueRemoved = d->list[loc].value;
		knh_memmove(d->list + loc, d->list + (loc+1), sizeof(knh_dict_t) * (d->size - loc));
		KNH_SETv(ctx, d->list[d->size-1].key,   keyRemoved);
		KNH_SETv(ctx, d->list[d->size-1].value, valueRemoved);
		d->size--;
		{
			knh_hdict_t *h = ((knh_hdict_t*)d->_list);
			if(loc < h[-1].sorted) h[-1].sorted -= 1;
		}
	}
}

/* ------------------------------------------------------------------------ */

void knh_DictMap_clear(Ctx *ctx, knh_DictMap_t *o)
{
	size_t i, capacity = knh_dict_capacity(o->_list);
	for(i = 0; i < capacity; i++) {
		KNH_SETv(ctx, o->list[i].key, KNH_NULL);
		KNH_SETv(ctx, o->list[i].value, KNH_NULL);
	}
	o->size = 0;
	knh_dict_setsorted(o->_list, 0);
}

/* ======================================================================== */
/* [DictSet] */

knh_dict_t *knh_dictset_malloc(Ctx *ctx, size_t init)
{
	return knh_dict_malloc(ctx, init, knh_dictset_finit, knh_dictset_ftraverse, knh_dict_fcmp);
}

/* ------------------------------------------------------------------------ */

void knh_DictSet_toIgnoreCase(knh_DictSet_t *o)
{
	knh_dict_tocase(o->_list);
	o->fcmp = knh_bytes_strcasecmp;
	knh_DictSet_setIgnoreCase(o, 1);
}


knh_DictSet_t* new_DictSet0(Ctx *ctx, knh_intptr_t capacity, int isIgnoreCase)
{
	knh_DictSet_t *o = new_H(DictSet);
	o->_list = knh_dict_malloc(ctx, capacity,
		knh_dictset_finit, knh_dictset_ftraverse, isIgnoreCase ? knh_dict_fcasecmp : knh_dict_fcmp);
	o->size = 0;
	o->fcmp = isIgnoreCase ? knh_bytes_strcasecmp : knh_bytes_strcmp ;
	return o;
}

/* ======================================================================== */
/* [methods] */

knh_index_t knh_DictSet_index(knh_DictSet_t *d, knh_bytes_t key)
{
	DBG_ASSERT(IS_bDictSet(d));
	return knh_dict_index(d->_list, d->size, d->fcmp, key);
}

/* ------------------------------------------------------------------------ */

knh_String_t* knh_DictSet_keyAt(knh_DictSet_t *o, size_t n)
{
	DBG_ASSERT(n < o->size);
	return o->list[n].key;
}

/* ------------------------------------------------------------------------ */

knh_uintptr_t knh_DictSet_valueAt(knh_DictSet_t *o, size_t n)
{
	DBG_ASSERT(n < o->size);
	return o->list[n].value;
}

///* ------------------------------------------------------------------------ */
//
//void knh_DictSet_removeAt(Ctx *ctx, knh_DictSet_t *o, size_t n)
//{
//	DBG_ASSERT(n < o->size);
//	o->list[n].value = 0;
//}

/* ------------------------------------------------------------------------ */

knh_String_t *new_String__DictSet(Ctx *ctx, knh_DictSet_t *o, knh_bytes_t key)
{
	knh_index_t loc = knh_dict_index(o->_list, o->size, o->fcmp, key);
	if(loc == -1) {
		return new_S(ctx, key);
	}
	else {
		return o->list[loc].key;
	}
}

/* ------------------------------------------------------------------------ */

knh_uintptr_t knh_DictSet_get(knh_DictSet_t *o, knh_bytes_t key)
{
	knh_intptr_t loc = knh_dict_index(o->_list, o->size, o->fcmp, key);
	return (loc == -1) ? 0 : o->list[loc].value;
}

/* ------------------------------------------------------------------------ */

void knh_DictSet_append(Ctx *ctx, knh_DictSet_t *o, knh_String_t *key, knh_uintptr_t n)
{
	if(o->_list == NULL) {
		o->_list = knh_dict_malloc(ctx, 8, knh_dictmap_finit, knh_dictmap_ftraverse, knh_dict_fcmp);
		o->size = 0;
	}
	else {
		size_t capacity = knh_dict_capacity(o->_list);
		if(o->size == capacity) {
			o->_list = knh_dict_resize(ctx, o->_list, capacity * 2);
		}
	}
	KNH_SETv(ctx, o->list[o->size].key, key);
	o->list[o->size].value = n;
	o->size++;
}

/* ------------------------------------------------------------------------ */

void knh_DictSet_set(Ctx *ctx, knh_DictSet_t *o, knh_String_t *key, knh_uintptr_t n)
{
	int loc = knh_dict_index(o->_list, o->size, o->fcmp, S_tobytes(key));
	if(loc != -1) {
		KNH_SETv(ctx, o->list[loc].key, key);  /* To avoid losing key */
		o->list[loc].value = n;
	}
	else {
		knh_DictSet_append(ctx, o, key, n);
	}
}

/* ------------------------------------------------------------------------ */

void knh_DictSet_add(Ctx *ctx, knh_DictSet_t *o, knh_String_t *key)
{
	int loc = knh_dict_index(o->_list, o->size, o->fcmp, S_tobytes(key));
	if(loc != -1) {
		KNH_SETv(ctx, o->list[loc].key, key);  /* To avoid losing key */
		o->list[loc].value += 1;
	}
	else {
		knh_DictSet_append(ctx, o, key, 1);
	}
}

///* ------------------------------------------------------------------------ */
//
//void knh_DictSet_remove(Ctx *ctx, knh_DictSet_t *d, knh_bytes_t key)
//{
//	knh_index_t loc = knh_dict_index(d->_list, d->size, d->fcmp, key);
//	if(loc != -1) {
//		// move removed entry to the tail
//		knh_String_t *keyRemoved = d->list[loc].key;
//		knh_memmove(d->list + loc, d->list + (loc+1), sizeof(knh_dict_t) * (d->size - loc));
//		KNH_SETv(ctx, d->list[d->size-1].key,   keyRemoved);
//		d->list[d->size-1].value = 0;
//		d->size--;
//		{
//			knh_hdict_t *h = ((knh_hdict_t*)d->_list);
//			if(loc < h[-1].sorted) h[-1].sorted -=1;
//		}
//	}
//}
//
///* ------------------------------------------------------------------------ */
//
//void knh_DictSet_clear(Ctx *ctx, knh_DictSet_t *o)
//{
//	size_t i, capacity = knh_dict_capacity(o->_list);
//	for(i = 0; i < capacity; i++) {
//		KNH_SETv(ctx, o->list[i].key, KNH_NULL);
//		o->list[i].value = 0;
//	}
//	o->size = 0;
//	knh_dict_setsorted(o->_list, 0);
//}

/* ------------------------------------------------------------------------ */

void knh_loadDefaultMapDSPI(Ctx *ctx, const knh_MapDSPI_t *dspi)
{
	knh_addDriverSPI(ctx, "hash", (knh_DriverSPI_t*)&MAP_ObjectObject);
}

#ifdef __cplusplus
}
#endif
