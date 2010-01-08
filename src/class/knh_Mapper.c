/****************************************************************************
 * KONOHA COPYRIGHT, LICENSE NOTICE, AND DISCRIMER
 *
 * Copyright (c) 2006-2010, Kimio Kuramitsu <kimio at ynu.ac.jp>
 *           (c) 2008-      Konoha Software Foundation
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

#include"commons.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ======================================================================== */
/* [func] */

KNHAPI(void) knh_addMapper(Ctx *ctx, knh_Mapper_t *mpr)
{
	knh_readyClassMap(ctx, DP(mpr)->scid);
	knh_ClassMap_add(ctx, ClassTable(DP(mpr)->scid).cmap, mpr);
}

/* ------------------------------------------------------------------------ */

void knh_addMapperFunc(Ctx *ctx, knh_flag_t flag, knh_type_t stype, knh_type_t ttype, knh_Fmapper fmap, Object *mapdata)
{
	knh_class_t cid = CLASS_type(stype);
	DBG2_ASSERT_cid(cid);
	knh_readyClassMap(ctx, cid);
	knh_ClassMap_add(ctx, ClassTable(cid).cmap,
		new_Mapper(ctx, flag, CLASS_type(stype), CLASS_type(ttype), fmap, mapdata));
}

/* ------------------------------------------------------------------------ */

static MAPPER knh_Fmapper_null(Ctx *ctx, knh_sfp_t *sfp MAPPERARG)
{
	DBG2_(
	knh_Mapper_t *mpr = sfp[1].mpr;
	DBG2_P("mpr: %s ==> %s", CLASSN(DP(mpr)->scid), CLASSN(DP(mpr)->tcid));
	);
	KNH_THROW__T(ctx, "ClassCast!!");
}

/* ------------------------------------------------------------------------ */

static MAPPER knh_Fmapper_method(Ctx *ctx, knh_sfp_t *sfp MAPPERARG)
{
	knh_Method_t *mtd = (knh_Method_t*)DP(sfp[1].mpr)->mapdata;
	KNH_ASSERT(IS_Method(mtd));
	KNH_SWAP(ctx, sfp, 1, 0);
	KNH_SCALL(ctx, sfp, 0, mtd, /*args */0);
}

/* ------------------------------------------------------------------------ */

KNHAPI(knh_Mapper_t*) new_Mapper(Ctx *ctx, knh_flag_t flag, knh_class_t scid, knh_class_t tcid, knh_Fmapper fmap, Object *mapdata)
{
	knh_Mapper_t* o = (knh_Mapper_t*)new_Object_bcid(ctx, CLASS_Mapper, 0);
	DP(o)->size = 0;
	if(knh_class_isFinal(scid)) {
		DP(o)->flag = flag | FLAG_Mapper_Final;
	}
	else {
		DP(o)->flag = flag;
	}
	DP(o)->scid = scid;
	DP(o)->tcid = tcid;
	if(fmap == NULL) {
		if(IS_Method(mapdata)) {
			fmap = knh_Fmapper_method;
		}
		else {
			fmap = knh_Fmapper_null;
		}
	}
	o->fmap_1 = fmap;
	KNH_SETv(ctx, DP(o)->mapdata, mapdata);
	return o;
}

/* ------------------------------------------------------------------------ */

KNHAPI(void) knh_loadMapperData(Ctx *ctx, knh_MapperData_t *data)
{
	knh_NameSpace_t *ns = DP(ctx->kc)->ns;
	KNH_ASSERT_CTX0(ctx);
	while(data->func != NULL) {
		knh_class_t scid = knh_NameSpace_getcid(ctx, ns, B(data->sname));
		knh_class_t tcid = knh_NameSpace_getcid(ctx, ns, B(data->tname));
		if(scid != CLASS_unknown && tcid != CLASS_unknown) {
			knh_Mapper_t *mpr = new_Mapper(ctx, data->flag, scid, tcid, data->func, KNH_NULL);
			knh_addMapper(ctx, mpr);
		}
		data++;
	}
}

/* ======================================================================== */
/* [MapMap] */

static MAPPER knh_Fmapper_mapmap(Ctx *ctx, knh_sfp_t *sfp MAPPERARG)
{
	knh_Mapper_t *mpr = KNH_GETMAPPER(ctx, sfp);
	DBG2_ASSERT(IS_Mapper(mpr));
	knh_Mapper_t *m1 = DP(mpr)->m1, *m2 = DP(mpr)->m2;
	DBG2_ASSERT(IS_Mapper(m1));
	KNH_MOV(ctx, sfp[1].o, m1);
	(m1)->fmap_1(ctx, sfp);
	if(IS_NOTNULL(sfp[0].o)) {
		DBG2_ASSERT(IS_Mapper(m2));
		KNH_MOV(ctx, sfp[1].o, m2);
		(m2)->fmap_1(ctx, sfp);
	}
}

/* ------------------------------------------------------------------------ */

knh_Mapper_t* new_MapMap(Ctx *ctx, knh_Mapper_t *m1, knh_Mapper_t *m2)
{
	knh_Mapper_t* mpr = (knh_Mapper_t*)new_Object_bcid(ctx, CLASS_Mapper, 0);
	KNH_NOTICE(ctx, "generated mapper: %C => %C => %C", DP(m1)->scid, DP(m1)->tcid, DP(m2)->tcid);
	DP(mpr)->size = 0;
	DP(mpr)->flag = DP(m1)->flag | DP(m2)->flag;
	DP(mpr)->scid = DP(m1)->scid;
	DP(mpr)->tcid = DP(m2)->tcid;
	DBG2_ASSERT(IS_Mapper(m1));
	KNH_SETv(ctx, DP(mpr)->m1, m1);
	DBG2_ASSERT(IS_Mapper(m2));
	KNH_SETv(ctx, DP(mpr)->m2, m2);
	mpr->h.flag = m1->h.flag & m2->h.flag;
	mpr->fmap_1 = knh_Fmapper_mapmap;
	return mpr;
}

/* ======================================================================== */
/* [cache] */

static
knh_Mapper_t *knh_Context_getMapperCache(Ctx *ctx, knh_class_t scid, knh_class_t tcid)
{
	knh_hashcode_t h = ((((knh_hashcode_t)scid) << (sizeof(knh_class_t) * 8)) + tcid) % ctx->cachesize;
	knh_Mapper_t *mpr = ctx->mprCache[h];
	if(mpr != NULL) {
		if(DP(mpr)->scid == scid && DP(mpr)->tcid == tcid) {
			knh_stat_mprCacheHit(ctx);
			return mpr;
		}
		knh_stat_fmtCacheMiss(ctx);
	}
	return NULL;
}

/* ------------------------------------------------------------------------ */

static
knh_Mapper_t *knh_Context_setMapperCache(Ctx *ctx, knh_class_t scid, knh_class_t tcid, knh_Mapper_t *mpr)
{
	knh_hashcode_t h = ((((knh_hashcode_t)scid) << (sizeof(knh_class_t) * 8)) + tcid) % ctx->cachesize;
	ctx->mprCache[h] = mpr;
	return mpr;
}

/* ------------------------------------------------------------------------ */
/* [Mapper] */

MAPPER knh_Fmapper_asis(Ctx *ctx, knh_sfp_t *sfp MAPPERARG)
{
}

/* ------------------------------------------------------------------------ */

static knh_Mapper_t* new_Mapper__asis(Ctx *ctx, knh_class_t scid, knh_class_t tcid)
{
	return new_Mapper(ctx, 0, scid, tcid, knh_Fmapper_asis, KNH_NULL);
}

/* ------------------------------------------------------------------------ */

static knh_Mapper_t* new_Mapper__NoSuchMapping(Ctx *ctx, knh_class_t scid, knh_class_t tcid)
{
	return new_Mapper(ctx, 0, scid, tcid, knh_Fmapper_null, KNH_NULL);
}

/* ------------------------------------------------------------------------ */

knh_bool_t knh_Mapper_isNoSuchMapping(knh_Mapper_t *mpr)
{
	return ((mpr)->fmap_1 == knh_Fmapper_null);
}

/* ======================================================================== */

knh_Mapper_t *knh_findMapperCommon(Ctx *ctx, knh_class_t scid, knh_class_t tcid, int isgen)
{
	knh_Mapper_t *mpr;
	DBG2_ASSERT_cid(scid); DBG2_ASSERT_cid(tcid);
//	if(scid == CLASS_Any) {
//		KNH_ASSERT(scid != CLASS_Any);
//		if(isgen == 0) return (knh_Mapper_t*)KNH_NULL;
//	}

	mpr = knh_Context_getMapperCache(ctx, scid, tcid);
	if(mpr != NULL) return mpr;

	DBG2_P("finding.. %s ==> %s",CLASSN(scid), CLASSN(tcid));

	{
		knh_class_t sbcid = scid;
		while(1) {
			int i;
			knh_ClassMap_t *cmap = ClassTable(sbcid).cmap;
			for(i = 0; i < (cmap)->size; i++) {
				mpr = (cmap)->maplist[i];
				if(DP(mpr)->tcid == tcid) {
					goto L_SETCACHE;
				}
			}
			for(i = 0; i < (cmap)->size; i++) {
				mpr = (cmap)->maplist[i];
				if(knh_class_instanceof(ctx, DP(mpr)->tcid, tcid)) {
					goto L_SETCACHE;
				}
			}
			if(sbcid == CLASS_Object) break;

			for(i = 0; i < (cmap)->size; i++) {
				int j;
				knh_class_t mcid = DP((cmap)->maplist[i])->tcid;
				DBG2_ASSERT_cid(mcid);
				if(mcid <= CLASS_String) {
					DBG_P("forbid lowlevel transitivity %s", CLASSN(mcid));
					continue;   /* Stop lowlevel inference */
				}
				knh_ClassMap_t *cmap2 = ClassTable(mcid).cmap;
				for(j = 0; j < (cmap2)->size; j++) {
					mpr = (cmap2)->maplist[j];
					if(DP(mpr)->tcid == tcid) {
						mpr = new_MapMap(ctx, (cmap)->maplist[i], mpr);
						goto L_ADDMAPPER;
					}
				}
				for(j = 0; j < (cmap2)->size; j++) {
					mpr = (cmap2)->maplist[j];
					if(knh_class_instanceof(ctx, DP(mpr)->tcid, tcid)) {
						mpr = new_MapMap(ctx, (cmap)->maplist[i], mpr);
						goto L_ADDMAPPER;
					}
				}
			}
			if(knh_class_isGenerics(sbcid) && ClassTable(sbcid).bcid != sbcid) {
				sbcid = ClassTable(sbcid).bcid;
			}
			else {
				sbcid = ClassTable(sbcid).supcid;
			}
		}
	}
	/* GENERATIVE PART */
	if(scid == tcid || knh_class_instanceof(ctx, scid, tcid)) {  /* default */
		mpr = new_Mapper__asis(ctx, scid, tcid);
		goto L_ADDMAPPER;
	}

	mpr = ClassTable(scid).cspi->genmap(ctx, scid, tcid);
	if(mpr != NULL) {
		goto L_ADDMAPPER;
	}

	if(isgen) {
		mpr = new_Mapper__NoSuchMapping(ctx, scid, tcid);
		goto L_ADDMAPPER;
	}
	return (knh_Mapper_t*)KNH_NULL;

	L_ADDMAPPER:
	knh_addMapper(ctx, mpr);
	L_SETCACHE:
	return knh_Context_setMapperCache(ctx, scid, tcid, mpr);
}

/* ------------------------------------------------------------------------ */


#ifdef __cplusplus
}
#endif
