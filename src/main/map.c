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

//#define USE_STEXT 1
//#define USE_B     1
#define USE_bytes_strcmp      1
#define USE_bytes_strcasecmp  1
#define USE_bytes_equals      1

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
	return (knh_hashcode_t)ClassTBL(knh_Object_bcid(sfp[0].o)).cspi->hashkey(ctx, sfp, KNH_FOBJECT_HASH);
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

static knh_map_t* hmap_set(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp)
{
	knh_hmap_t *hmap = knh_map_hmap(m);
	knh_hentry_t **hentry = knh_map_hentry(m);
	knh_Object_t *key = ksfp[0].o;
	knh_hashcode_t hcode = knh_sfp_hashCode(ctx, ksfp);
	size_t idx = hcode % hmap->hmax;
	knh_hentry_t *e = hentry[idx];
	while(e != NULL) {
		if(e->hcode == hcode && knh_Object_compareTo(ctx, e->key, key) == 0) {
			KNH_SETv(ctx, e->value, ksfp[1].o);
			return (knh_map_t*)hmap;
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
	KNH_INITv(e->value, ksfp[1].o);
	e->next = hentry[idx];
	hentry[idx] = e;
	hmap->size += 1;
	return (knh_map_t*)hmap;
}

static knh_map_t* hmap_setdata(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp)
{
	knh_hmap_t *hmap = knh_map_hmap(m);
	knh_hentry_t **hentry = knh_map_hentry(m);
	knh_Object_t *key = ksfp[0].o;
	knh_hashcode_t hcode = knh_sfp_hashCode(ctx, ksfp);
	size_t idx = hcode % hmap->hmax;
	knh_hentry_t *e = hentry[idx];
	while(e != NULL) {
		if(e->hcode == hcode && knh_Object_compareTo(ctx, e->key, key) == 0) {
			e->dvalue = ksfp[1].data;
			return (knh_map_t*)hmap;
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
	e->dvalue = ksfp[1].data;
	e->next = hentry[idx];
	hentry[idx] = e;
	hmap->size += 1;
	return (knh_map_t*)hmap;
}

static knh_map_t* hmap_dset(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp)
{
	knh_hmap_t *hmap = knh_map_hmap(m);
	knh_hentry_t **hentry = knh_map_hentry(m);
	knh_hashcode_t hcode = (knh_hashcode_t)ksfp[0].data;
	size_t idx = hcode % hmap->hmax;
	knh_hentry_t *e = hentry[idx];
	while(e != NULL) {
		if(e->dkey == ksfp[0].data) {
			KNH_SETv(ctx, e->value, ksfp[1].o);
			return (knh_map_t*)hmap;
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
	KNH_INITv(e->value, ksfp[1].o);
	e->next = hentry[idx];
	hentry[idx] = e;
	hmap->size += 1;
	return (knh_map_t*)hmap;
}

static knh_map_t* hmap_dsetdata(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp)
{
	knh_hmap_t *hmap = knh_map_hmap(m);
	knh_hentry_t **hentry = knh_map_hentry(m);
	knh_hashcode_t hcode = (knh_hashcode_t)ksfp[0].data;
	size_t idx = hcode % hmap->hmax;
	knh_hentry_t *e = hentry[idx];
	while(e != NULL) {
		if(e->dkey == ksfp[0].data) {
			e->dvalue = ksfp[1].data;
			return (knh_map_t*)hmap;
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
	e->dvalue = ksfp[1].data;
	e->next = hentry[idx];
	hentry[idx] = e;
	hmap->size += 1;
	return (knh_map_t*)hmap;
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

/* ------------------------------------------------------------------------ */
/* dmap */

#define UNSORTED 8
#define K_DMAP_DELTA           1
#define knh_map_dmap(m)        ((knh_dmap_t*)m)
#define knh_map_dentry(m)      (((knh_dentry_t*)m)+K_DMAP_DELTA)

#define DMAP_GET(m, KEY, index, SET) {\
		knh_dmap_t *dmap = knh_map_dmap(m);\
		knh_dentry_t *dentry = knh_map_dentry(m);\
		KEY;\
		knh_index_t loc = index(dentry, 0, dmap->sorted, key);\
		if(loc == -1) {\
			loc = index(dentry, dmap->sorted, dmap->size, key);\
			if(loc == -1) return 0;\
		}\
		SET;\
		return 1;\
	}\

#define DMAP_SET(m, KEY, index, addkey, SET) {\
		knh_dmap_t *dmap = knh_map_dmap(m);\
		knh_dentry_t *dentry = knh_map_dentry(m);\
		KEY;\
		knh_index_t loc = index(dentry, 0, dmap->sorted, key);\
		if(loc == -1) {\
			loc = index(dentry, dmap->sorted, dmap->size, key);\
			if(loc == -1) {\
				dmap = addkey(ctx, dmap, dentry, ksfp, index, &loc);\
				dentry = knh_map_dentry(dmap);\
			}\
		}\
		SET;\
		return (knh_map_t*)dmap;\
	}\

#define dmap_grow(dmap, dentry) {\
		knh_hmem_t *hmem = (knh_hmem_t*)dmap;\
		if(!(dmap->size + K_DMAP_DELTA < hmem[-1].capacity)) {\
			dmap = (knh_dmap_t*)knh_hgrow(ctx, hmem);\
			dentry = knh_map_dentry(dmap);\
		}\
	}\

#define dmap_sort(dmap, dentry, cmpr, index) \
	if(!(dmap->sorted - dmap->size < UNSORTED)) {\
		knh_qsort(dentry, dmap->size, sizeof(knh_dentry_t), cmpr);\
		dmap->sorted = dmap->size;\
		*loc = index(dentry, 0, dmap->sorted, key);\
	}\

#define DMAP_RM(m, KEY, index, rm) {\
		knh_dmap_t *dmap = knh_map_dmap(m);\
		knh_dentry_t *dentry = knh_map_dentry(m);\
		KEY;\
		knh_index_t loc = index(dentry, 0, dmap->sorted, key);\
		if(loc == -1) {\
			loc = index(dentry, dmap->sorted, dmap->size, key);\
			if(loc == -1) {\
				return;\
			}\
		}\
		rm(ctx, dmap, dentry, loc);\
	}\


static knh_map_t *dmap_init(Ctx *ctx, size_t init, char *path, void *option)
{
	knh_dmap_t *dmap = (knh_dmap_t*)knh_hmalloc(ctx, sizeof(knh_dentry_t), init + K_DMAP_DELTA);
	dmap->size = 0;
	dmap->sorted = 0;
	return dmap;
}

static void dmap_ftr(Ctx *ctx, knh_map_t *m, knh_Ftraverse ftr)
{
	knh_dmap_t *dmap = knh_map_dmap(m);
	knh_dentry_t *dentry = knh_map_dentry(m);
	size_t i;
	for(i = 0; i < dmap->size; i++) {
		KNH_FTR(ctx, ftr, dentry[i].key);
		KNH_FTR(ctx, ftr, dentry[i].value);
	}
}

static void dmap_ftrdata(Ctx *ctx, knh_map_t *m, knh_Ftraverse ftr)
{
	knh_dmap_t *dmap = knh_map_dmap(m);
	knh_dentry_t *dentry = knh_map_dentry(m);
	size_t i;
	for(i = 0; i < dmap->size; i++) {
		KNH_FTR(ctx, ftr, dentry[i].key);
	}
}

static void dmap_dftr(Ctx *ctx, knh_map_t *m, knh_Ftraverse ftr)
{
	knh_dmap_t *dmap = knh_map_dmap(m);
	knh_dentry_t *dentry = knh_map_dentry(m);
	size_t i;
	for(i = 0; i < dmap->size; i++) {
		KNH_FTR(ctx, ftr, dentry[i].value);
	}
}

static void dmap_dftrdata(Ctx *ctx, knh_map_t *m, knh_Ftraverse ftr)
{
}

static void rm_dremovedata(Ctx *ctx, knh_dmap_t *dmap, knh_dentry_t *dentry, size_t loc)
{
	KNH_TODO("remove");
}

static void rm_dremove(Ctx *ctx, knh_dmap_t *dmap, knh_dentry_t *dentry, size_t loc)
{
	KNH_FINALv(ctx, dentry[loc].value);
	rm_dremovedata(ctx, dmap, dentry, loc);
}

static void rm_removedata(Ctx *ctx, knh_dmap_t *dmap, knh_dentry_t *dentry, size_t loc)
{
	KNH_FINALv(ctx, dentry[loc].key);
	rm_dremovedata(ctx, dmap, dentry, loc);
}

static void rm_remove(Ctx *ctx, knh_dmap_t *dmap, knh_dentry_t *dentry, size_t loc)
{
	KNH_FINALv(ctx, dentry[loc].key);
	KNH_FINALv(ctx, dentry[loc].value);
	rm_dremovedata(ctx, dmap, dentry, loc);
}

static void dmap_free(Ctx *ctx, knh_map_t *m)
{
	knh_hfree(ctx, (knh_hmem_t*)m);
}

static size_t dmap_size(Ctx *ctx, knh_map_t* m)
{
	knh_dmap_t *dmap = knh_map_dmap(m);
	return dmap->size;
}

static knh_bool_t dmap_setIterator(Ctx *ctx, knh_map_t* m, knh_Iterator_t *it)
{
	KNH_TODO("dmap_setIterator");
	return 0;
}

/* ------------------------------------------------------------------------ */
/* int */

static knh_index_t iindex(knh_dentry_t *a, size_t sp, size_t ep, knh_int_t key)
{
	L_TAIL:;
	if(ep - sp < UNSORTED) {
		size_t i;
		for(i = sp; i < ep; i++) {
			if(a[i].ikey == key) return i;
		}
		return -1;
	}
	else {
		size_t cp = KNH_MID(sp, ep);
		if(a[cp].ikey == key) { return cp; }
		else if(a[cp].ikey < key) { ep = cp; }
		else { sp = cp + 1; }
		goto L_TAIL;
	}
}

static int dentry_icmp(const void *p, const void *p2)
{
	knh_dentry_t *e = (knh_dentry_t*)p;
	knh_dentry_t *e2 = (knh_dentry_t*)p2;
	if(e->ikey == e2->ikey) return 0;
	return (e->ikey < e2->ikey) ? -1 : 1;
}

static knh_bool_t dmap_iget(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp, knh_sfp_t *rsfp)
{
	DMAP_GET(m, knh_int_t key = ksfp[0].ivalue, iindex, KNH_SETv(ctx, rsfp[0].o, dentry[loc].value));
}

static knh_bool_t dmap_igetint(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp, knh_sfp_t *rsfp)
{
	DMAP_GET(m, knh_int_t key = ksfp[0].ivalue, iindex, rsfp[0].ivalue = dentry[loc].ivalue);
}

static knh_bool_t dmap_igetfloat(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp, knh_sfp_t *rsfp)
{
	DMAP_GET(m, knh_int_t key = ksfp[0].ivalue, iindex, rsfp[0].fvalue = dentry[loc].fvalue);
}

static knh_dmap_t* add_ikey(Ctx *ctx, knh_dmap_t *dmap, knh_dentry_t *dentry, knh_sfp_t *ksfp, knh_index_t (*iindex)(knh_dentry_t *, size_t, size_t, knh_int_t), knh_index_t *loc)
{
	knh_int_t key = ksfp[0].ivalue;
	*loc = dmap->size;
	dmap_grow(dmap, dentry);
	dentry[*loc].ikey = (knh_intptr_t)(ksfp[0].ivalue);
	dmap->size++;
	dmap_sort(dmap, dentry, dentry_icmp, iindex);
	return dmap;
}

static knh_map_t* dmap_iset(Ctx *ctx, knh_map_t* m, knh_sfp_t* ksfp)
{
	DMAP_SET(m, knh_int_t key = ksfp[0].ivalue, iindex, add_ikey, KNH_SETv(ctx, dentry[loc].value, ksfp[1].o));
}

static knh_map_t* dmap_isetint(Ctx *ctx, knh_map_t* m, knh_sfp_t* ksfp)
{
	DMAP_SET(m, knh_int_t key = ksfp[0].ivalue, iindex, add_ikey, dentry[loc].ivalue = (knh_intptr_t)(ksfp[1].ivalue));
}

static knh_map_t* dmap_isetfloat(Ctx *ctx, knh_map_t* m, knh_sfp_t* ksfp)
{
	DMAP_SET(m, knh_int_t key = ksfp[0].ivalue, iindex, add_ikey, dentry[loc].fvalue = (knh_floatptr_t)(ksfp[1].fvalue));
}

static void dmap_iremove(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp)
{
	DMAP_RM(m, knh_int_t key = ksfp[0].ivalue, iindex, rm_dremove);
}

static void dmap_iremovedata(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp)
{
	DMAP_RM(m, knh_int_t key = ksfp[0].ivalue, iindex, rm_dremovedata);
}

/* ------------------------------------------------------------------------ */
/* float */

static knh_index_t findex(knh_dentry_t *a, size_t sp, size_t ep, knh_float_t key)
{
	L_TAIL:;
	if(ep - sp < UNSORTED) {
		size_t i;
		for(i = sp; i < ep; i++) {
			if(a[i].fkey == key) return i;
		}
		return -1;
	}
	else {
		size_t cp = KNH_MID(sp, ep);
		if(a[cp].fkey == key) { return cp; }
		else if(a[cp].fkey < key) { ep = cp; }
		else { sp = cp + 1; }
		goto L_TAIL;
	}
}

static int dentry_fcmp(const void *p, const void *p2)
{
	knh_dentry_t *e = (knh_dentry_t*)p;
	knh_dentry_t *e2 = (knh_dentry_t*)p2;
	if(e->fkey == e2->fkey) return 0;
	return (e->fkey < e2->fkey) ? -1 : 1;
}

static knh_bool_t dmap_fget(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp, knh_sfp_t *rsfp)
{
	DMAP_GET(m, knh_float_t key = ksfp[0].fvalue, findex, KNH_SETv(ctx, rsfp[0].o, dentry[loc].value));
}

static knh_bool_t dmap_fgetint(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp, knh_sfp_t *rsfp)
{
	DMAP_GET(m, knh_float_t key = ksfp[0].fvalue, findex, rsfp[0].ivalue = dentry[loc].ivalue);
}

static knh_bool_t dmap_fgetfloat(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp, knh_sfp_t *rsfp)
{
	DMAP_GET(m, knh_int_t key = (knh_int_t)(ksfp[0].fvalue), iindex, rsfp[0].fvalue = dentry[loc].fvalue);
}

static knh_dmap_t* add_fkey(Ctx *ctx, knh_dmap_t *dmap, knh_dentry_t *dentry, knh_sfp_t *ksfp, knh_index_t (*findex)(knh_dentry_t *, size_t, size_t, knh_float_t), knh_index_t *loc)
{
	knh_float_t key = ksfp[0].fvalue;
	*loc = dmap->size;
	dmap_grow(dmap, dentry);
	dentry[*loc].fkey = (knh_floatptr_t)key;
	dmap->size++;
	dmap_sort(dmap, dentry, dentry_fcmp, findex);
	return dmap;
}

static knh_map_t* dmap_fset(Ctx *ctx, knh_map_t* m, knh_sfp_t* ksfp)
{
	DMAP_SET(m, knh_float_t key = ksfp[0].fvalue, findex, add_fkey, KNH_SETv(ctx, dentry[loc].value, ksfp[1].o));
}

static knh_map_t* dmap_fsetint(Ctx *ctx, knh_map_t* m, knh_sfp_t* ksfp)
{
	DMAP_SET(m, knh_float_t key = ksfp[0].fvalue, findex, add_fkey, dentry[loc].ivalue = (knh_intptr_t)(ksfp[1].ivalue));
}

static knh_map_t* dmap_fsetfloat(Ctx *ctx, knh_map_t* m, knh_sfp_t* ksfp)
{
	DMAP_SET(m, knh_float_t key = ksfp[0].fvalue, findex, add_fkey, dentry[loc].fvalue = (knh_floatptr_t)(ksfp[1].fvalue));
}

static void dmap_fremove(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp)
{
	DMAP_RM(m, knh_float_t key = ksfp[0].fvalue, findex, rm_dremove);
}

static void dmap_fremovedata(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp)
{
	DMAP_RM(m, knh_float_t key = ksfp[0].fvalue, findex, rm_dremovedata);
}

/* ------------------------------------------------------------------------ */
/* String */

//static void dump(knh_dentry_t *a, size_t sp, size_t ep, int res)
//{
//	size_t i;
//	for(i = sp; i < ep; i++) {
//		DBG_P("a[%ld] %s %ld", i, S_tochar(a[i].key), a[i].ivalue);
//	}
//	DBG_P("res = %d", res);
//}

static knh_index_t strcmp_sindex(knh_dentry_t *a, size_t sp, size_t ep, knh_bytes_t key)
{
	//dump(a, sp, ep, 0);
	L_TAIL:;
	if(ep - sp < UNSORTED) {
		size_t i;
		for(i = sp; i < ep; i++) {
			if(knh_bytes_equals(S_tobytes(a[i].key), key)) return i;
		}
		return -1;
	}
	else {
		size_t cp = KNH_MID(sp, ep);
		int i = knh_bytes_strcmp(key, S_tobytes(a[cp].key));
		if(i == 0) {
			return cp;
		}
		else if(i < 0) { ep = cp; }
		else { sp = cp + 1; }
		goto L_TAIL;
	}
}

static int dentry_strcmp(const void *p, const void *p2)
{
	knh_dentry_t *e = (knh_dentry_t*)p;
	knh_dentry_t *e2 = (knh_dentry_t*)p2;
	return knh_bytes_strcmp(S_tobytes(e->key), S_tobytes(e2->key));
}

static knh_bool_t dmap_sget(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp, knh_sfp_t *rsfp)
{
	DMAP_GET(m, knh_bytes_t key = S_tobytes(ksfp[0].s), strcmp_sindex, KNH_SETv(ctx, rsfp[0].o, dentry[loc].value));
}

static knh_bool_t dmap_sgetint(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp, knh_sfp_t *rsfp)
{
	DMAP_GET(m, knh_bytes_t key = S_tobytes(ksfp[0].s), strcmp_sindex, rsfp[0].ivalue = dentry[loc].ivalue);
}

static knh_bool_t dmap_sgetfloat(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp, knh_sfp_t *rsfp)
{
	DMAP_GET(m, knh_bytes_t key = S_tobytes(ksfp[0].s), strcmp_sindex, rsfp[0].fvalue = dentry[loc].fvalue);
}

static knh_dmap_t *add_skey(Ctx *ctx, knh_dmap_t *dmap, knh_dentry_t *dentry, knh_sfp_t *ksfp, knh_index_t (*sindex)(knh_dentry_t *, size_t, size_t, knh_bytes_t), knh_index_t *loc)
{
	knh_bytes_t key = S_tobytes(ksfp[0].s);
	*loc = dmap->size;
	dmap_grow(dmap, dentry);
	KNH_INITv(dentry[*loc].key, ksfp[0].s);
	KNH_INITv(dentry[*loc].value, KNH_NULL);
	dmap->size++;
	dmap_sort(dmap, dentry, dentry_strcmp, sindex);
	return dmap;
}

static knh_dmap_t *add_skeydata(Ctx *ctx, knh_dmap_t *dmap, knh_dentry_t *dentry, knh_sfp_t *ksfp, knh_index_t (*sindex)(knh_dentry_t *, size_t, size_t, knh_bytes_t), knh_index_t *loc)
{
	knh_bytes_t key = S_tobytes(ksfp[0].s);
	*loc = dmap->size;
	dmap_grow(dmap, dentry);
	KNH_INITv(dentry[*loc].key, ksfp[0].s);
	dmap->size++;
	dmap_sort(dmap, dentry, dentry_strcmp, sindex);
	return dmap;
}

static knh_map_t* dmap_sset(Ctx *ctx, knh_map_t* m, knh_sfp_t* ksfp)
{
	DMAP_SET(m, knh_bytes_t key = S_tobytes(ksfp[0].s), strcmp_sindex, add_skey, KNH_SETv(ctx, dentry[loc].value, ksfp[1].o));
}

static knh_map_t* dmap_ssetint(Ctx *ctx, knh_map_t* m, knh_sfp_t* ksfp)
{
	DMAP_SET(m, knh_bytes_t key = S_tobytes(ksfp[0].s), strcmp_sindex, add_skeydata, dentry[loc].ivalue = (knh_intptr_t)(ksfp[1].ivalue));
}

static knh_map_t* dmap_ssetfloat(Ctx *ctx, knh_map_t* m, knh_sfp_t* ksfp)
{
	DMAP_SET(m, knh_bytes_t key = S_tobytes(ksfp[0].s), strcmp_sindex, add_skeydata, dentry[loc].fvalue = (knh_floatptr_t)(ksfp[1].fvalue));
}

static void dmap_sremove(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp)
{
	DMAP_RM(m, knh_bytes_t key = S_tobytes(ksfp[0].s), strcmp_sindex, rm_remove);
}

static void dmap_sremovedata(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp)
{
	DMAP_RM(m, knh_bytes_t key = S_tobytes(ksfp[0].s), strcmp_sindex, rm_removedata);
}

static const knh_MapDSPI_t* dmap_config(Ctx *ctx, knh_class_t p1, knh_class_t p2);

static const struct knh_MapDSPI_t DMAP_IntObject = {
	K_MAP_DSPI, "dict",
	dmap_config,
	dmap_init, dmap_dftr, dmap_free,
	dmap_iget, dmap_iset, dmap_iremove,
	dmap_size, dmap_setIterator,
};

static const struct knh_MapDSPI_t DMAP_IntInt = {
	K_MAP_DSPI, "dict",
	dmap_config,
	dmap_init, dmap_dftrdata, dmap_free,
	dmap_igetint, dmap_isetint, dmap_iremovedata,
	dmap_size, dmap_setIterator,
};

static const struct knh_MapDSPI_t DMAP_IntFloat = {
	K_MAP_DSPI, "dict",
	dmap_config,
	dmap_init, dmap_dftrdata, dmap_free,
	dmap_igetfloat, dmap_isetfloat, dmap_iremovedata,
	dmap_size, dmap_setIterator,
};

static const struct knh_MapDSPI_t DMAP_FloatObject = {
	K_MAP_DSPI, "dict",
	dmap_config,
	dmap_init, dmap_dftr, dmap_free,
	dmap_fget, dmap_fset, dmap_fremove,
	dmap_size, dmap_setIterator,
};

static const struct knh_MapDSPI_t DMAP_FloatInt = {
	K_MAP_DSPI, "dict",
	dmap_config,
	dmap_init, dmap_dftrdata, dmap_free,
	dmap_fgetint, dmap_fsetint, dmap_fremovedata,
	dmap_size, dmap_setIterator,
};

static const struct knh_MapDSPI_t DMAP_FloatFloat = {
	K_MAP_DSPI, "dict",
	dmap_config,
	dmap_init, dmap_dftrdata, dmap_free,
	dmap_fgetfloat, dmap_fsetfloat, dmap_fremovedata,
	dmap_size, dmap_setIterator,
};

static const struct knh_MapDSPI_t DMAP_StringObject = {
	K_MAP_DSPI, "dict",
	dmap_config,
	dmap_init, dmap_ftr, dmap_free,
	dmap_sget, dmap_sset, dmap_sremove, dmap_size,
	dmap_setIterator,
};

static const struct knh_MapDSPI_t DMAP_StringInt = {
	K_MAP_DSPI, "dict",
	dmap_config,
	dmap_init, dmap_ftrdata, dmap_free,
	dmap_sgetint, dmap_ssetint, dmap_sremovedata, dmap_size,
	dmap_setIterator,
};

static const struct knh_MapDSPI_t DMAP_StringFloat = {
	K_MAP_DSPI, "dict",
	dmap_config,
	dmap_init, dmap_ftrdata, dmap_free,
	dmap_sgetfloat, dmap_ssetfloat, dmap_sremovedata, dmap_size,
	dmap_setIterator,
};

static const knh_MapDSPI_t* dmap_config(Ctx *ctx, knh_class_t p1, knh_class_t p2)
{
	if(IS_Tint(p1) || p1 == TYPE_Boolean) {
		if(IS_Tint(p2) || p2 == TYPE_Boolean) {
			return &DMAP_IntInt;
		}
		if(IS_Tfloat(p1)) {
			return &DMAP_IntFloat;
		}
		return &DMAP_IntObject;
	}
	else if(IS_Tfloat(p1)) {
		if(IS_Tint(p2) || p2 == TYPE_Boolean) {
			return &DMAP_FloatInt;
		}
		if(IS_Tfloat(p1)) {
			return &DMAP_FloatFloat;
		}
		return &DMAP_FloatObject;
	}
	else if(IS_Tstr(p1)) {
		if(IS_Tint(p2) || p2 == TYPE_Boolean) {
			return &DMAP_StringInt;
		}
		if(IS_Tfloat(p1)) {
			return &DMAP_StringFloat;
		}
		return &DMAP_StringObject;
	}
	return NULL;
}

/* ------------------------------------------------------------------------- */
/* [casecmp] */

static knh_index_t strcasecmp_sindex(knh_dentry_t *a, size_t sp, size_t ep, knh_bytes_t key)
{
	L_TAIL:;
	if(ep - sp < UNSORTED) {
		size_t i;
		for(i = sp; i < ep; i++) {
			if(knh_bytes_strcasecmp(S_tobytes(a[i].key), key) == 0) return i;
		}
		return -1;
	}
	else {
		size_t cp = KNH_MID(sp, ep);
		int i = knh_bytes_strcasecmp(key, S_tobytes(a[cp].key));
		if(i == 0) { return cp; }
		else if(i < 0) { ep = cp; }
		else { sp = cp + 1; }
		goto L_TAIL;
	}
}

static int dentry_strcasecmp(const void *p, const void *p2)
{
	knh_dentry_t *e = (knh_dentry_t*)p;
	knh_dentry_t *e2 = (knh_dentry_t*)p2;
	return knh_bytes_strcasecmp(S_tobytes(e->key), S_tobytes(e2->key));
}

static knh_bool_t casemap_sget(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp, knh_sfp_t *rsfp)
{
	DMAP_GET(m, knh_bytes_t key = S_tobytes(ksfp[0].s), strcasecmp_sindex, KNH_SETv(ctx, rsfp[0].o, dentry[loc].value));
}

static knh_bool_t casemap_sgetint(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp, knh_sfp_t *rsfp)
{
	DMAP_GET(m, knh_bytes_t key = S_tobytes(ksfp[0].s), strcasecmp_sindex, rsfp[0].ivalue = dentry[loc].ivalue);
}

static knh_bool_t casemap_sgetfloat(Ctx *ctx, knh_map_t* m, knh_sfp_t *ksfp, knh_sfp_t *rsfp)
{
	DMAP_GET(m, knh_bytes_t key = S_tobytes(ksfp[0].s), strcasecmp_sindex, rsfp[0].fvalue = dentry[loc].fvalue);
}

static knh_dmap_t *add_casekey(Ctx *ctx, knh_dmap_t *dmap, knh_dentry_t *dentry, knh_sfp_t *ksfp, knh_index_t (*sindex)(knh_dentry_t *, size_t, size_t, knh_bytes_t), knh_index_t *loc)
{
	knh_bytes_t key = S_tobytes(ksfp[0].s);
	*loc = dmap->size;
	dmap_grow(dmap, dentry);
	KNH_INITv(dentry[*loc].key, ksfp[0].s);
	KNH_INITv(dentry[*loc].key, KNH_NULL);
	dmap->size++;
	dmap_sort(dmap, dentry, dentry_strcasecmp, sindex);
	return dmap;
}

static knh_dmap_t *add_casekeydata(Ctx *ctx, knh_dmap_t *dmap, knh_dentry_t *dentry, knh_sfp_t *ksfp, knh_index_t (*sindex)(knh_dentry_t *, size_t, size_t, knh_bytes_t), knh_index_t *loc)
{
	knh_bytes_t key = S_tobytes(ksfp[0].s);
	*loc = dmap->size;
	dmap_grow(dmap, dentry);
	KNH_INITv(dentry[*loc].key, ksfp[0].s);
	dmap->size++;
	dmap_sort(dmap, dentry, dentry_strcasecmp, sindex);
	return dmap;
}

static knh_map_t* casemap_sset(Ctx *ctx, knh_map_t* m, knh_sfp_t* ksfp)
{
	DMAP_SET(m, knh_bytes_t key = S_tobytes(ksfp[0].s), strcasecmp_sindex, add_casekey, KNH_SETv(ctx, dentry[loc].value, ksfp[1].o));
}

static knh_map_t* casemap_ssetint(Ctx *ctx, knh_map_t* m, knh_sfp_t* ksfp)
{
	DMAP_SET(m, knh_bytes_t key = S_tobytes(ksfp[0].s), strcasecmp_sindex, add_casekeydata, dentry[loc].ivalue = (knh_intptr_t)(ksfp[1].ivalue));
}

static knh_map_t* casemap_ssetfloat(Ctx *ctx, knh_map_t* m, knh_sfp_t* ksfp)
{
	DMAP_SET(m, knh_bytes_t key = S_tobytes(ksfp[0].s), strcasecmp_sindex, add_casekeydata, dentry[loc].fvalue = (knh_floatptr_t)(ksfp[1].fvalue));
}

static const knh_MapDSPI_t* casemap_config(Ctx *ctx, knh_class_t p1, knh_class_t p2);

static const struct knh_MapDSPI_t CASEMAP_StringObject = {
	K_MAP_DSPI, "casedict",
	casemap_config,
	dmap_init, dmap_ftr, dmap_free,
	casemap_sget, casemap_sset, dmap_sremove,
	dmap_size, dmap_setIterator,
};

static const struct knh_MapDSPI_t CASEMAP_StringInt = {
	K_MAP_DSPI, "casedict",
	casemap_config,
	dmap_init, dmap_ftrdata, dmap_free,
	casemap_sgetint, casemap_ssetint, dmap_sremove,
	dmap_size, dmap_setIterator,
};

static const struct knh_MapDSPI_t CASEMAP_StringFloat = {
	K_MAP_DSPI, "casedict",
	casemap_config,
	dmap_init, dmap_ftrdata, dmap_free,
	casemap_sgetfloat, casemap_ssetfloat, dmap_sremove,
	dmap_size, dmap_setIterator,
};

static const knh_MapDSPI_t* casemap_config(Ctx *ctx, knh_class_t p1, knh_class_t p2)
{
	if(IS_Tstr(p1)) {
		if(IS_Tint(p2) || p2 == TYPE_Boolean) {
			return &CASEMAP_StringInt;
		}
		if(IS_Tfloat(p1)) {
			return &CASEMAP_StringFloat;
		}
		return &CASEMAP_StringObject;
	}
	return dmap_config(ctx, p1, p2);
}

/* ------------------------------------------------------------------------ */
/* DictMap */

knh_DictMap_t* new_DictMap0(Ctx *ctx, size_t capacity)
{
	knh_Map_t *m = new_H(Map);
	m->dspi = &DMAP_StringObject;
	m->map = m->dspi->init(ctx, capacity, NULL, NULL);
	return (knh_DictMap_t*)m;
}

knh_DictCaseMap_t* new_DictCaseMap0(Ctx *ctx, size_t capacity)
{
	knh_Map_t *m = new_H(Map);
	m->dspi = &DMAP_StringObject;
	m->map = m->dspi->init(ctx, capacity, NULL, NULL);
	return (knh_DictCaseMap_t*)m;
}

knh_DictSet_t* new_DictSet0(Ctx *ctx, size_t capacity)
{
	knh_Map_t *m = new_H(Map);
	m->dspi = &DMAP_StringInt;
	m->map = m->dspi->init(ctx, capacity, NULL, NULL);
	return (knh_DictSet_t*)m;
}

knh_DictCaseSet_t* new_DictCaseSet0(Ctx *ctx, size_t capacity)
{
	knh_Map_t *m = new_H(Map);
	m->dspi = &CASEMAP_StringInt;
	m->map = m->dspi->init(ctx, capacity, NULL, NULL);
	return (knh_DictCaseSet_t*)m;
}

///* ------------------------------------------------------------------------ */
//
//knh_DictMap_t* new_DictMap(Ctx *ctx, knh_class_t p1, knh_intptr_t capacity)
//{
//	knh_class_t cid = knh_class_P1(ctx, CLASS_DictMap, p1);
//	knh_DictMap_t *o = (knh_DictMap_t*)new_hObject_(ctx, FLAG_DictMap, CLASS_DictMap, cid);
//	o->_list = knh_dict_malloc(ctx, capacity, knh_dictmap_finit, knh_dictmap_ftraverse, knh_dict_fcmp);
//	o->size = 0;
//	o->fcmp = knh_bytes_strcmp;
//	return o;
//}
//
//
/* ------------------------------------------------------------------------ */

knh_String_t* knh_DictMap_keyAt(knh_DictMap_t *m, size_t n)
{
	DBG_ASSERT(n < knh_DictMap_size(m));
	return m->dlistO[n + K_DMAP_DELTA].key;
}

Object* knh_DictMap_valueAt(knh_DictMap_t *m, size_t n)
{
	DBG_ASSERT(n < knh_DictMap_size(m));
	return m->dlistO[n + K_DMAP_DELTA].value;
}

knh_String_t* knh_DictCaseMap_keyAt(knh_DictCaseMap_t *m, size_t n)
{
	DBG_ASSERT(n < knh_DictMap_size(m));
	return m->dlistO[n + K_DMAP_DELTA].key;
}

Object* knh_DictCaseMap_valueAt(knh_DictCaseMap_t *m, size_t n)
{
	DBG_ASSERT(n < knh_DictMap_size(m));
	return m->dlistO[n + K_DMAP_DELTA].value;
}

knh_String_t* knh_DictSet_keyAt(knh_DictSet_t *m, size_t n)
{
	DBG_ASSERT(n < knh_DictSet_size(m));
	return m->dlistI[n + K_DMAP_DELTA].key;
}

knh_uintptr_t knh_DictSet_valueAt(knh_DictSet_t *m, size_t n)
{
	DBG_ASSERT(n < knh_DictSet_size(m));
	return (knh_uintptr_t)m->dlistI[n + K_DMAP_DELTA].ivalue;
}

knh_String_t* knh_DictCaseSet_keyAt(knh_DictCaseSet_t *m, size_t n)
{
	DBG_ASSERT(n < knh_DictSet_size(m));
	return m->dlistI[n + K_DMAP_DELTA].key;
}

knh_uintptr_t knh_DictCaseSet_valueAt(knh_DictCaseSet_t *m, size_t n)
{
	DBG_ASSERT(n < knh_DictSet_size(m));
	return (knh_uintptr_t)m->dlistI[n + K_DMAP_DELTA].ivalue;
}

/* ------------------------------------------------------------------------ */

knh_index_t knh_DictMap_index(knh_DictMap_t *m, knh_bytes_t key)
{
	knh_dmap_t *dmap = knh_map_dmap(m->dmap);
	knh_dentry_t *dentry = knh_map_dentry(m->dmap);
	knh_index_t loc = strcmp_sindex(dentry, 0, dmap->sorted, key);
	if(loc == -1) {
		loc = strcmp_sindex(dentry, dmap->sorted, dmap->size, key);
	}
	return loc;
}

knh_index_t knh_DictCaseMap_index(knh_DictCaseMap_t *m, knh_bytes_t key)
{
	knh_dmap_t *dmap = knh_map_dmap(m->dmap);
	knh_dentry_t *dentry = knh_map_dentry(m->dmap);
	knh_index_t loc = strcasecmp_sindex(dentry, 0, dmap->sorted, key);
	if(loc == -1) {
		loc = strcasecmp_sindex(dentry, dmap->sorted, dmap->size, key);
	}
	return loc;
}

knh_index_t knh_DictSet_index(knh_DictSet_t *m, knh_bytes_t key)
{
	knh_dmap_t *dmap = knh_map_dmap(m->dmap);
	knh_dentry_t *dentry = knh_map_dentry(m->dmap);
	knh_index_t loc = strcmp_sindex(dentry, 0, dmap->sorted, key);
	if(loc == -1) {
		loc = strcmp_sindex(dentry, dmap->sorted, dmap->size, key);
	}
	return loc;
}

knh_index_t knh_DictCaseSet_index(knh_DictCaseSet_t *m, knh_bytes_t key)
{
	knh_dmap_t *dmap = knh_map_dmap(m->dmap);
	knh_dentry_t *dentry = knh_map_dentry(m->dmap);
	knh_index_t loc = strcasecmp_sindex(dentry, 0, dmap->sorted, key);
	if(loc == -1) {
		loc = strcasecmp_sindex(dentry, dmap->sorted, dmap->size, key);
	}
	return loc;
}

/* ------------------------------------------------------------------------ */

Object *knh_DictMap_getNULL(Ctx *ctx, knh_DictMap_t *m, knh_bytes_t key)
{
	knh_dmap_t *dmap = knh_map_dmap(m->dmap);
	knh_dentry_t *dentry = knh_map_dentry(m->dmap);
	knh_index_t loc = strcmp_sindex(dentry, 0, dmap->sorted, key);
	if(loc == -1) {
		loc = strcmp_sindex(dentry, dmap->sorted, dmap->size, key);
	}
	return (loc == -1) ? NULL : m->dlistO[loc + K_DMAP_DELTA].value;
}

Object *knh_DictCaseMap_getNULL(Ctx *ctx, knh_DictCaseMap_t *m, knh_bytes_t key)
{
	knh_dmap_t *dmap = knh_map_dmap(m->dmap);
	knh_dentry_t *dentry = knh_map_dentry(m->dmap);
	knh_index_t loc = strcasecmp_sindex(dentry, 0, dmap->sorted, key);
	if(loc == -1) {
		loc = strcasecmp_sindex(dentry, dmap->sorted, dmap->size, key);
	}
	return (loc == -1) ? NULL : m->dlistO[loc + K_DMAP_DELTA].value;
}

knh_uintptr_t knh_DictSet_get(Ctx *ctx, knh_DictSet_t *m, knh_bytes_t key)
{
	knh_dmap_t *dmap = knh_map_dmap(m->dmap);
	knh_dentry_t *dentry = knh_map_dentry(m->dmap);
	knh_index_t loc = strcmp_sindex(dentry, 0, dmap->sorted, key);
	if(loc == -1) {
		loc = strcmp_sindex(dentry, dmap->sorted, dmap->size, key);
	}
	return (loc == -1) ? 0 : (knh_uintptr_t)m->dlistI[loc+K_DMAP_DELTA].ivalue;
}

knh_uintptr_t knh_DictCaseSet_get(Ctx *ctx, knh_DictCaseSet_t *m, knh_bytes_t key)
{
	knh_dmap_t *dmap = knh_map_dmap(m->dmap);
	knh_dentry_t *dentry = knh_map_dentry(m->dmap);
	knh_index_t loc = strcasecmp_sindex(dentry, 0, dmap->sorted, key);
	if(loc == -1) {
		loc = strcasecmp_sindex(dentry, dmap->sorted, dmap->size, key);
	}
	return (loc == -1) ? 0 : (knh_uintptr_t)m->dlistI[loc+K_DMAP_DELTA].ivalue;
}

/* ------------------------------------------------------------------------ */

void knh_DictMap_set_(Ctx *ctx, knh_DictMap_t *m, knh_String_t *key, Any *v)
{
	knh_sfp_t* lsfp = ctx->esp;
	KNH_SETv(ctx, lsfp[0].o, key);
	KNH_SETv(ctx, lsfp[1].o, v);
	m->map = m->dspi->set(ctx, m->map, lsfp);
}

void knh_DictCaseMap_set_(Ctx *ctx, knh_DictCaseMap_t *m, knh_String_t *key, Any *v)
{
	knh_sfp_t* lsfp = ctx->esp;
	KNH_SETv(ctx, lsfp[0].o, key);
	KNH_SETv(ctx, lsfp[1].o, v);
	m->map = m->dspi->set(ctx, m->map, lsfp);
}

/* ------------------------------------------------------------------------ */

void knh_DictSet_append(Ctx *ctx, knh_DictSet_t *m, knh_String_t *key, knh_uintptr_t n)
{
	knh_sfp_t* lsfp = ctx->esp;
	KNH_SETv(ctx, lsfp[0].o, key);
	lsfp[1].ivalue = n;
	m->map = m->dspi->set(ctx, m->map, lsfp);
}

/* ------------------------------------------------------------------------ */

void knh_DictSet_set(Ctx *ctx, knh_DictSet_t *m, knh_String_t *key, knh_uintptr_t n)
{
	knh_sfp_t* lsfp = ctx->esp;
	KNH_SETv(ctx, lsfp[0].o, key);
	lsfp[1].ivalue = n;
	m->map = m->dspi->set(ctx, m->map, lsfp);
}

/* ------------------------------------------------------------------------ */

void knh_DictCaseSet_set(Ctx *ctx, knh_DictCaseSet_t *m, knh_String_t *key, knh_uintptr_t n)
{
	knh_sfp_t* lsfp = ctx->esp;
	KNH_SETv(ctx, lsfp[0].o, key);
	lsfp[1].ivalue = n;
	m->map = m->dspi->set(ctx, m->map, lsfp);
}

/* ------------------------------------------------------------------------ */

void knh_DictCaseSet_append(Ctx *ctx, knh_DictCaseSet_t *m, knh_String_t *key, knh_uintptr_t n)
{
	knh_sfp_t* lsfp = ctx->esp;
	KNH_SETv(ctx, lsfp[0].o, key);
	lsfp[1].ivalue = n;
	m->map = m->dspi->set(ctx, m->map, lsfp);
}

/* ------------------------------------------------------------------------ */

void knh_loadDefaultMapDSPI(Ctx *ctx, const knh_MapDSPI_t *dspi)
{
	knh_addDriverSPI(ctx, "hash", (knh_DriverSPI_t*)&MAP_ObjectObject);
	knh_addDriverSPI(ctx, "dict", (knh_DriverSPI_t*)&DMAP_StringObject);
	knh_addDriverSPI(ctx, "casedict", (knh_DriverSPI_t*)&CASEMAP_StringObject);
}

#ifdef __cplusplus
}
#endif
