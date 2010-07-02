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

/* ======================================================================== */
/* [func] */

static void knh_readyTransMapList(Ctx *ctx, knh_class_t cid)
{
	DBG_ASSERT_cid(cid);
	knh_ClassTable_t *t = pClassTable(ctx, cid);
	if(t->tmaps == KNH_EMPTYLIST) {
		KNH_ASSERT(knh_Array_size(t->tmaps) == 0);
		KNH_SETv(ctx, t->tmaps, new_Array0(ctx, 1));
		//KNH_SYSLOG(ctx, LOG_INFO, "[NEW_TMAPLIST] cid=%s", CLASSN(cid));
	}
}

/* ------------------------------------------------------------------------ */

KNHAPI(void) knh_addTranslator(Ctx *ctx, knh_Translator_t *trl)
{
	knh_class_t cid = SP(trl)->scid;
	knh_readyTransMapList(ctx, cid);
	knh_Array_add(ctx, ClassTable(cid).tmaps, trl);
}

/* ------------------------------------------------------------------------ */

void knh_addTranslatorFunc(Ctx *ctx, knh_flag_t flag, knh_type_t stype, knh_type_t ttype, knh_Ftranslator ftcast, Object *mapdata)
{
	knh_class_t cid = CLASS_type(stype);
	DBG_ASSERT_cid(cid);
	knh_readyTransMapList(ctx, cid);
	knh_Array_add(ctx, ClassTable(cid).tmaps,
		new_Translator(ctx, flag, CLASS_type(stype), CLASS_type(ttype), ftcast));
}

/* ------------------------------------------------------------------------ */

static TCAST knh_Ftranslator_null(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	DBG_(
	knh_Translator_t *trl = sfp[0].casttrl;
	DBG_P("trl: %s ==> %s", CLASSN(SP(trl)->scid), CLASSN(SP(trl)->tcid));
	);
	KNH_THROW__T(ctx, "ClassCast!!");
}

///* ------------------------------------------------------------------------ */
//
//static TCAST knh_Ftranslator_method(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	knh_Method_t *mtd = DP(sfp[0].casttrl)->mtd;
//	DBG_ASSERT(IS_Method(mtd));
//	KNH_SETv(ctx, sfp[1+K_CALLDELTA].o, sfp[0].o);
//	KNH_SCALL(ctx, sfp, 1, mtd, /*argc*/, 0);
//	RETURN(ctx, sfp, rtnidx, sfp[1+K_CALLDELTA].o);
//}

/* ------------------------------------------------------------------------ */

KNHAPI(knh_Translator_t*) new_Translator(Ctx *ctx, knh_flag_t flag, knh_class_t scid, knh_class_t tcid, knh_Ftranslator ftcast)
{
	knh_Translator_t* trl = new_(Translator);
	if(knh_class_isFinal(scid)) {
		DP(trl)->flag = flag | FLAG_Translator_Final;
	}
	else {
		DP(trl)->flag = flag;
	}
	SP(trl)->scid = scid;
	SP(trl)->tcid = tcid;
	if(ftcast == NULL) ftcast = knh_Ftranslator_null;
	trl->ftcast_1 = ftcast;
	KNH_SETv(ctx, DP(trl)->mapdata, KNH_NULL);
	return trl;
}

///* ======================================================================== */
///* [MapMap] */
//
//static TCAST knh_Ftranslator_mapmap(Ctx *ctx, knh_sfp_t *sfp, long rix)
//{
//	knh_Translator_t *trl = sfp[K_TRLIDX].casttrl;
//	DBG_ASSERT(IS_Translator(trl));
//	knh_Translator_t *m1 = DP(trl)->m1, *m2 = DP(trl)->m2;
//	DBG_ASSERT(IS_Translator(m1));
//	klr_mov(ctx, sfp[1].o, m1);
//	(m1)->ftcast_1(ctx, sfp);
//	if(IS_NOTNULL(sfp[0].o)) {
//		DBG_ASSERT(IS_Translator(m2));
//		klr_mov(ctx, sfp[1].o, m2);
//		(m2)->ftcast_1(ctx, sfp);
//	}
//}
//
///* ------------------------------------------------------------------------ */
//
static knh_Translator_t* new_MapMap(Ctx *ctx, knh_Translator_t *m1, knh_Translator_t *m2)
{
//	knh_Translator_t* trl = (knh_Translator_t*)new_Object_bcid(ctx, CLASS_Translator, 0);
//	KNH_SYSLOG(ctx, LOG_NOTICE, "generated mapper: %C => %C => %C", DP(m1)->scid, DP(m1)->tcid, DP(m2)->tcid);
//	DP(trl)->size = 0;
//	DP(trl)->flag = DP(m1)->flag | DP(m2)->flag;
//	SP(trl)->scid = DP(m1)->scid;
//	SP(trl)->tcid = DP(m2)->tcid;
//	DBG_ASSERT(IS_Translator(m1));
//	KNH_SETv(ctx, DP(trl)->m1, m1);
//	DBG_ASSERT(IS_Translator(m2));
//	KNH_SETv(ctx, DP(trl)->m2, m2);
//	trl->h.flag = m1->h.flag & m2->h.flag;
//	trl->ftcast_1 = knh_Ftranslator_mapmap;
//	return trl;
	TODO(); DBG_ABORT();
	return NULL;
}

/* ======================================================================== */
/* [cache] */

static knh_Translator_t *knh_getTranslatorCacheNULL(Ctx *ctx, knh_class_t scid, knh_class_t tcid)
{
	knh_hashcode_t h = ((((knh_hashcode_t)scid) << (sizeof(knh_class_t) * 8)) + tcid) % K_CACHESIZE;
	knh_Translator_t *trl = ctx->trlCache[h];
	if(trl != NULL) {
		if(SP(trl)->scid == scid && SP(trl)->tcid == tcid) {
			knh_stat_trlCacheHit(ctx);
			return trl;
		}
		knh_stat_fmtCacheMiss(ctx);
	}
	return NULL;
}

/* ------------------------------------------------------------------------ */

static knh_Translator_t *knh_setTranslatorCache(Ctx *ctx, knh_class_t scid, knh_class_t tcid, knh_Translator_t *trl)
{
	knh_hashcode_t h = ((((knh_hashcode_t)scid) << (sizeof(knh_class_t) * 8)) + tcid) % K_CACHESIZE;
	ctx->trlCache[h] = trl;
	return trl;
}

/* ------------------------------------------------------------------------ */
/* [Translator] */

static TCAST knh_Ftranslator_asis(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	KNH_SWAPMOV(ctx, sfp, rix, 0);
}

/* ------------------------------------------------------------------------ */

static knh_Translator_t* new_TranslatorASIS(Ctx *ctx, knh_class_t scid, knh_class_t tcid)
{
	return new_Translator(ctx, 0, scid, tcid, knh_Ftranslator_asis);
}

/* ------------------------------------------------------------------------ */

static knh_Translator_t* new_Translator__NoSuchMapping(Ctx *ctx, knh_class_t scid, knh_class_t tcid)
{
	return new_Translator(ctx, 0, scid, tcid, NULL);
}

/* ------------------------------------------------------------------------ */

knh_bool_t knh_Translator_isNoSuchMapping(knh_Translator_t *trl)
{
	return ((trl)->ftcast_1 == knh_Ftranslator_null);
}

/* ======================================================================== */

knh_Translator_t *knh_findTranslatorNULL(Ctx *ctx, knh_class_t scid, knh_class_t tcid, int isGEN)
{
	knh_Translator_t *trl = knh_getTranslatorCacheNULL(ctx, scid, tcid);
	if(trl != NULL) return trl;
	DBG_P("finding.. %s ==> %s",CLASSN(scid), CLASSN(tcid));
	DBG_ASSERT_cid(scid); DBG_ASSERT_cid(tcid);
	{
		knh_class_t sbcid = scid;
		while(1) {
			size_t i;
			knh_Array_t *a = ClassTable(sbcid).tmaps;
			for(i = 0; i < (a)->size; i++) {
				trl = (a)->trans[i];
				if(SP(trl)->tcid == tcid) {
					goto L_SETCACHE;
				}
			}
			for(i = 0; i < (a)->size; i++) {
				trl = (a)->trans[i];
				if(knh_class_instanceof(ctx, SP(trl)->tcid, tcid)) {
					goto L_SETCACHE;
				}
			}
			if(sbcid == CLASS_Object) break;
			for(i = 0; i < (a)->size; i++) {
				size_t j;
				knh_class_t mcid = SP((a)->trans[i])->tcid;
				DBG_ASSERT_cid(mcid);
				if(mcid <= CLASS_String) {
					DBG_P("forbid lowlevel transitivity %s", CLASSN(mcid));
					continue;   /* Stop lowlevel inference */
				}
				knh_Array_t *a2 = ClassTable(mcid).tmaps;
				for(j = 0; j < (a2)->size; j++) {
					trl = (a2)->trans[j];
					if(SP(trl)->tcid == tcid) {
						trl = new_MapMap(ctx, (a)->trans[i], trl);
						goto L_ADDTRL;
					}
				}
				for(j = 0; j < (a2)->size; j++) {
					trl = (a2)->trans[j];
					if(knh_class_instanceof(ctx, SP(trl)->tcid, tcid)) {
						trl = new_MapMap(ctx, (a)->trans[i], trl);
						goto L_ADDTRL;
					}
				}
			}
			if(knh_class_isGenerics(ctx, sbcid) && ClassTable(sbcid).bcid != sbcid) {
				sbcid = ClassTable(sbcid).bcid;
			}
			else {
				sbcid = ClassTable(sbcid).supcid;
			}
		}
	}
	/* GENERATIVE PART */
	if(scid == tcid || knh_class_instanceof(ctx, scid, tcid)) {  /* default */
		trl = new_TranslatorASIS(ctx, scid, tcid);
		goto L_ADDTRL;
	}

	trl = ClassTable(scid).cspi->genmap(ctx, scid, tcid);
	if(trl != NULL) {
		goto L_ADDTRL;
	}

	if(isGEN) {
		trl = new_Translator__NoSuchMapping(ctx, scid, tcid);
		goto L_ADDTRL;
	}
	return NULL;

	L_ADDTRL:
	knh_addTranslator(ctx, trl);
	L_SETCACHE:
	return knh_setTranslatorCache(ctx, scid, tcid, trl);
}

/* ------------------------------------------------------------------------ */


#ifdef __cplusplus
}
#endif
