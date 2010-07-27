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

#ifdef K_USING_ICONV
#include<iconv.h>
#endif

#define K_USING_LOADDATA
#include"../../include/konoha/konoha_grammar_.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

char *TT_tochar(knh_term_t tt);

/* ======================================================================== */
/* [ContextTable] */

#ifdef KNH_DBGMODE
#define FLAG_CTX(f)      (f|FLAG_Context_Verbose|FLAG_Context_Strict)
#else
#define FLAG_CTX(f)      (f|FLAG_Context_Strict)
#endif

/* ------------------------------------------------------------------------ */

volatile static size_t ctxid_counter = 1;

static void knh_Object_RCsweep(Ctx *ctx, Object *o)
{
	knh_Object_RCdec(o);
	if(knh_Object_isRC0(o)) {
		knh_Object_free(ctx, o);
	}
}

static knh_Context_t* new_hContext(Ctx *ctx0)
{
	knh_Context_t *ctx;
	if(ctx0 == NULL) {
		ctx = (knh_Context_t*)malloc(sizeof(knh_Context_t));
		knh_bzero(ctx, sizeof(knh_Context_t));
		ctx0 = ctx;
		ctx->ctxid = 0;
	}
	else {
		KNH_ASSERT_CTX0(ctx0);
		ctx = (knh_Context_t*)KNH_MALLOC(ctx0, sizeof(knh_Context_t));
		knh_bzero(ctx, sizeof(knh_Context_t));
		ctx->ctxid = ctxid_counter;
		ctxid_counter++;
	}
	ctx->h.magic = K_OBJECT_MAGIC;
	knh_Object_RCset(ctx, K_RCGC_INIT);
	ctx->h.flag = FLAG_CTX(FLAG_Context);
	ctx->h.bcid = CLASS_Context;
	ctx->h.cid  = CLASS_Context;
	ctx->unusedObject = NULL;
	ctx->unusedObjectSize = 0;
	ctx->parent = ctx0;
	ctx->fsweep = knh_Object_RCsweep;
//	ctx->unusedContext = NULL;
	ctx->api = knh_getExportsAPI();
	return ctx;
}

/* ------------------------------------------------------------------------ */
/* [ContextCommon] */

static void knh_CommonContext_init(Ctx *ctx, knh_Context_t *o)
{
	KNH_ASSERT_CTX0(ctx);
	//knh_stack_initexpand((Ctx*)o);
	DBG_ASSERT(o->sys != NULL);
	DBG_ASSERT(o->script != NULL);
	DBG_ASSERT(o->gma != NULL);
	//KNH_INITv(o->script, new_Script(ctx, ctx->share->mainns, new_ClassId(ctx), CLASS_Script));
	KNH_INITv(o->enc, DP(ctx->sys)->enc);
	KNH_INITv(o->in,  DP(ctx->sys)->in);
	KNH_INITv(o->out, DP(ctx->sys)->out);
	KNH_INITv(o->err, DP(ctx->sys)->err);
	KNH_INITv(o->e, KNH_NULL);
	o->ctxlock = knh_mutex_malloc(ctx);
}

static void knh_CommonContext_traverse(Ctx *ctx, knh_Context_t *ctxo, knh_Ftraverse ftr)
{
	KNH_FTR(ctx, ftr, ctxo->e);
	KNH_FTR(ctx, ftr, (ctxo->script));
	KNH_FTR(ctx, ftr, (ctxo->enc));
	KNH_FTR(ctx, ftr, (ctxo->in));
	KNH_FTR(ctx, ftr, (ctxo->out));
	KNH_FTR(ctx, ftr, (ctxo->err));
	KNH_FTR(ctx, ftr, ctxo->gma);
	// stack
	if(ctxo->stack != NULL) {
		size_t i;
		for(i = 0; i < ctxo->stacksize; i++) {
			KNH_FTR(ctx, ftr, ctxo->stack[i].o);
		}
	}
	KNH_FTR(ctx, ftr, (ctxo->bufa));
	KNH_FTR(ctx, ftr, (ctxo->bufw));
}

static void knh_CommonContext_free(Ctx *ctx, knh_Context_t *ctxo)
{
	KNH_FREE(ctx, ctxo->stack, sizeof(knh_sfp_t) * ctxo->stacksize);
	ctxo->stack = NULL;
	ctxo->esp = NULL;
	ctxo->stacktop = NULL;
	ctxo->stacksize = 0;
	KNH_FREE(ctx, ctxo->mtdCache, K_PAGESIZE * (sizeof(void*)/4));
	ctxo->mtdCache = NULL;
	ctxo->fmtCache = NULL;
	ctxo->trlCache = NULL;
	knh_mutex_free(ctxo, ctxo->ctxlock);
	ctxo->ctxlock = NULL;
	ctxo->bufa = NULL;
}

/* ------------------------------------------------------------------------ */

static void _vsyslog(int p, const char *fmt, va_list ap)
{
	fprintf(stderr, "%s ", LOG_tochar(p));
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
}

static void _syslog(int p, const char *fmt, ...)
{
	va_list ap;
	va_start(ap , fmt);
	fprintf(stderr, "%s ", LOG_tochar(p));
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	va_end(ap);
}

static const knh_EvidenceSPI_t EBISPI = {
	"stderr", _syslog, _vsyslog,
};

static knh_bool_t _isOPCODE(knh_opcode_t op)
{
	return 1;
}

static knh_Fmethod _compile(Ctx *ctx, knh_opline_t *op)
{
	return NULL;
}

static const knh_CompilerSPI_t JITSPI = {
	"Unsupported", _isOPCODE, _compile,
};

static int _lock(knh_mutex_t *m DBG_TRACE)
{
	TRACE_P("UNLOCK mutex=%p", m);
	return 0;
};


static int _unlock(knh_mutex_t *m DBG_TRACE)
{
	TRACE_P("UNLOCK mutex=%p", m);
	return 0;
};


static const knh_SyncSPI_t SYNCSPI = {
	"Synchronized(NO Thread)",
	_lock, _unlock,
};


/* ------------------------------------------------------------------------ */
/* [RootContext] */

static void knh_initClassTBL(knh_ClassTBL_t *t, size_t s, size_t e)
{
	size_t i;
	knh_bzero(&t[s], SIZEOF_TCLASS(e-s));
	for(i = s; i < e; i++) {
		t[i].keyidx   = -1;
	}
}

static void knh_expandClassTBL(Ctx *ctx)
{
	size_t s = ctx->share->ClassTBLSize, max = ctx->share->ClassTBLMax * 2;
	knh_ClassTBL_t *newt = (knh_ClassTBL_t*)KNH_MALLOC(ctx, SIZEOF_TCLASS(max));
	knh_memcpy(newt, ctx->share->ClassTBL, SIZEOF_TCLASS(s));
	knh_initClassTBL(newt, s, max);
	((knh_share_t*)ctx->share)->ClassTBL = newt;
	((knh_share_t*)ctx->share)->ClassTBLMax = max;
	KNH_SYSLOG(ctx, LOG_NOTICE, "ExtendedClassTBL", "*size=%ld", max);
}

/* ------------------------------------------------------------------------ */

knh_class_t new_ClassId(Ctx *ctx)
{
	knh_class_t newid = ctx->share->ClassTBLSize;
	KNH_ASSERT_CTX0(ctx);
	if(ctx->share->ClassTBLSize == ctx->share->ClassTBLMax) {
		knh_expandClassTBL(ctx);
	}
	((knh_share_t*)ctx->share)->ClassTBLSize = newid + 1;
	return newid;
}

/* ------------------------------------------------------------------------ */

void knh_expandEventTBL(Ctx *ctx)
{
	size_t s = ctx->share->EventTBLSize, max = ctx->share->EventTBLMax * 2;
	knh_EventTBL_t *newt = (knh_EventTBL_t*)KNH_MALLOC(ctx, SIZEOF_TEXPT(max));
	knh_bzero(newt, SIZEOF_TEXPT(max));
	knh_memcpy(newt, ctx->share->EventTBL, SIZEOF_TEXPT(s));
	((knh_share_t*)ctx->share)->EventTBL = newt;
	((knh_share_t*)ctx->share)->EventTBLMax = max;
}

#ifndef HMEM_SAFEZERO
static knh_hmem_t HMEM_ZERO = {0, 0};
#define HMEM_SAFEZERO         (&HMEM_ZERO) + 1
#endif

static Ctx* new_RootContext(void)
{
	knh_Context_t *ctx = (knh_Context_t*)new_hContext(NULL);
	const knh_PackageLoaderAPI_t *kapi = knh_getPackageAPI();
	knh_share_t *share = (knh_share_t*)malloc(sizeof(knh_share_t) + sizeof(knh_stat_t));
	ctx->share = share;
	knh_bzero(share, sizeof(knh_share_t) + sizeof(knh_stat_t));
	share->threadCounter = 1;
	ctx->stat = (knh_stat_t*)((share+1));

	share->ebiSPI  = &EBISPI;
	share->jitSPI  = &JITSPI;
	share->syncSPI = &SYNCSPI;

	share->ArenaSet = (knh_ArenaSet_t*)KNH_MALLOC(ctx, K_ARENASET_INITSIZE * sizeof(knh_ArenaSet_t));
	knh_bzero(share->ArenaSet, K_ARENASET_INITSIZE * sizeof(knh_ArenaSet_t));
	share->ArenaSetSize = 0;
	share->ArenaSetMax = K_ARENASET_INITSIZE;

	share->ClassTBL = (knh_ClassTBL_t*)KNH_MALLOC((Ctx*)ctx, SIZEOF_TCLASS(K_CLASSTABLE_INIT));
	knh_initClassTBL((knh_ClassTBL_t*)share->ClassTBL, 0, K_CLASSTABLE_INIT);
	share->ClassTBLSize = 0;
	share->ClassTBLMax  = K_CLASSTABLE_INIT;

	share->EventTBL = (knh_EventTBL_t*)KNH_MALLOC(ctx, SIZEOF_TEXPT(K_EVENTTBL_INIT));
	knh_bzero((void*)share->EventTBL, SIZEOF_TEXPT(K_EVENTTBL_INIT));
	share->EventTBLSize = 0;
	share->EventTBLMax  = K_EVENTTBL_INIT;
	knh_loadSystemStructData(ctx, kapi);
	{
		knh_RawPtr_t *p = (knh_RawPtr_t*)new_hObject_(ctx, FLAG_Object_Immutable|FLAG_Object_NullObject, CLASS_Any, CLASS_Any);
		ctx->api->RawPtr_init(ctx, p, NULL, NULL);
		KNH_INITv(share->constNull, p);
	}
	{
		knh_Boolean_t *o = new_H(Boolean);
		o->n.bvalue = 1;
		KNH_INITv(share->constTrue, o);
	}
	{
		knh_Boolean_t *o = new_H(Boolean);
		o->n.bvalue = 0;
		KNH_INITv(share->constFalse, o);
	}
	{
		knh_Int_t *io = new_H(Int);
		(io)->n.ivalue = 0;
		KNH_INITv(share->constInt0, io);
	}
	{
		knh_Float_t *fo = new_H(Float);
		(fo)->n.fvalue = K_FLOAT_ZERO;
		KNH_INITv(share->constFloat0, fo);
	}
	{
		knh_Array_t *a = new_H(Array);
		(a)->hmem = HMEM_SAFEZERO;
		(a)->size = 0;
		KNH_INITv(share->emptyArray, a);
	}

	DBG_ASSERT(share->tString == NULL);
	share->tString = (knh_String_t**)KNH_MALLOC(ctx, SIZEOF_TSTRING);
	knh_bzero(share->tString, SIZEOF_TSTRING);
	knh_loadSystemString(ctx);
	/* These are not shared, but needed to initialize System*/
	knh_stack_initexpand(ctx, NULL, K_STACKSIZE);
	KNH_INITv(ctx->sys, new_(System));
	knh_loadSystemData(ctx, kapi);
	KNH_INITv(share->mainns, new_NameSpace(ctx, NULL));
	KNH_INITv(ctx->script, new_(Script));
	{
		knh_Gamma_t *kc = new_(Gamma);
		KNH_INITv(ctx->gma, kc);
		KNH_INITv(DP(kc)->symbolDictMap, new_DictMap0(ctx, 256));
		KNH_INITv(DP(kc)->constPools, new_Array0(ctx, 0));
		KNH_INITv(DP(kc)->script, ctx->script);
	}
	knh_loadSystemKLRCode(ctx);
	knh_loadSystemMethod(ctx, kapi);
	share->ctx0 = ctx;
	knh_CommonContext_init(ctx, ctx);
	knh_System_initPath(ctx, ctx->sys);
	knh_loadTokenData(ctx);
	knh_loadAliasTokenData(ctx);
	return (Ctx*)ctx;
}

/* ------------------------------------------------------------------------ */

static void knh_share_traverse(Ctx *ctx, knh_share_t *share, knh_Ftraverse ftr)
{
	size_t i;
	KNH_FTR(ctx, ftr, share->constNull);
	KNH_FTR(ctx, ftr, share->constTrue);
	KNH_FTR(ctx, ftr, share->constFalse);
	KNH_FTR(ctx, ftr, (share->constInt0));
	KNH_FTR(ctx, ftr, (share->constFloat0));
	KNH_FTR(ctx, ftr, (share->emptyArray));
	KNH_FTR(ctx, ftr, (ctx->sys));
	KNH_FTR(ctx, ftr, (share->mainns));
	for(i = 0; i < K_TSTRING_SIZE; i++) {
		KNH_FTR(ctx, ftr, (share->tString[i]));
	}
	for(i = 0; i < share->EventTBLSize; i++) {
		if(EventTBL(i).name != NULL) {
			KNH_FTR(ctx, ftr, (EventTBL(i).name));
		}
	}
	/* tclass */
	for(i = 0; i < share->ClassTBLSize; i++) {
		knh_ClassTBL_t *t = pClassTBL(ctx, i);
		DBG_ASSERT(t->lname != NULL);
		KNH_NULLFTR(ctx, ftr, (t->typeNULL));
		KNH_FTR(ctx, ftr, (t->methods));
		if(t->fields != NULL) {
			size_t j;
			for(j = 0; j < t->fsize; j++) {
				KNH_NULLFTR(ctx, ftr, t->fields[j].value);
			}
		}
		KNH_FTR(ctx, ftr, t->tmaps);
		KNH_NULLFTR(ctx, ftr, t->cparam);
		KNH_NULLFTR(ctx, ftr, t->defnull);
		KNH_NULLFTR(ctx, ftr, t->constDictCaseMapNULL);
		KNH_FTR(ctx, ftr, t->sname);
		KNH_FTR(ctx, ftr, t->lname);
	}
}

static void knh_ObjectArenaSet_free(Ctx *ctx, knh_ArenaSet_t *t)
{
	knh_OArena_t *oslot = t->oslot;
	while(oslot + 1 <= (knh_OArena_t*)t->bottom) {
		size_t i;
		for(i = 0; i < K_OARENASIZE; i++) {
			Object *o = oslot->slots + i;
			if(o->h.magic != K_OBJECT_MAGIC) continue;
#ifdef KNH_HOBJECT_REFC
			DBG_({
				fprintf(stderr, "async object %p cid=%s(%d), ref=%d ", o, bcid__(o->h.bcid), (int)o->h.cid, (int)o->h.refc);
				switch(o->h.bcid) {
				case CLASS_Boolean:
					fprintf(stderr, "bvalue='%ld' true?=%d false?=%d\n", (int)((knh_Int_t*)o)->n.bvalue, (o == KNH_TRUE), (o == KNH_FALSE));
				break;
				case CLASS_Int:
					fprintf(stderr, "ivalue='%lld'\n", (long long int)((knh_Int_t*)o)->n.ivalue);
				break;
				case CLASS_Class:
					fprintf(stderr, "o->cid=%d\n", (int)((knh_Class_t*)o)->cid);
				break;
				case CLASS_String:
					fprintf(stderr, "str='%s'\n", S_tochar((knh_String_t*)o));
				break;
				case CLASS_Method: {
					knh_Method_t *mtd = (knh_Method_t*)o;
					fprintf(stderr, "DP(mtd)->cid=%s, DP(mtd)->mn=%d\n", bcid__(DP(mtd)->cid), DP(mtd)->mn);
					break;
				}
				case CLASS_Stmt:
					fprintf(stderr, "stmt='%s'\n", TT_tochar((SP(knh_Stmt_t*)o)->stt));
				break;
				default:
					fprintf(stderr, "\n");
				}
			});
			o->h.refc = 0;
#endif
			knh_Object_free(ctx, o);
		}
		oslot++;
	}
}

void knh_Object_finalSweep(Ctx *ctx, Object *o)
{
}

static void knh_share_free(Ctx *ctx, knh_share_t *share)
{
	size_t i;

	KNH_FREE(ctx, (void*)share->EventTBL, SIZEOF_TEXPT(ctx->share->EventTBLMax));
	share->EventTBL = NULL;
	KNH_FREE(ctx, share->tString, SIZEOF_TSTRING);
	share->tString = NULL;
	((knh_Context_t*)ctx)->fsweep = knh_Object_finalSweep;

	DBG_ASSERT(share->ArenaSet != NULL);
	for(i = 0; i < share->ArenaSetSize; i++) {
		knh_ArenaSet_t *t = share->ArenaSet + i;
		if(t->isObjectArena == 1) {
			knh_ObjectArenaSet_free(ctx, share->ArenaSet + i);
		}
	}
	for(i = 0; i < share->ClassTBLSize; i++) {
		knh_ClassTBL_t *t = pClassTBL(ctx, i);
		if(t->fields != NULL) {
			KNH_FREE(ctx, t->fields, sizeof(knh_fields_t) * t->fsize);
			t->fields = NULL;
		}
	}
	KNH_FREE(ctx, (void*)share->ClassTBL, SIZEOF_TCLASS(share->ClassTBLMax));
	share->ClassTBL = NULL;
	DBG_ASSERT(share->ArenaSet != NULL);

	for(i = 0; i < share->ArenaSetSize; i++) {
		knh_ArenaSet_t *t = share->ArenaSet + i;
		KNH_VFREE(ctx, t->head, t->arena_size);
	}
	KNH_FREE(ctx, share->ArenaSet, share->ArenaSetMax * sizeof(knh_ArenaSet_t));
	share->ArenaSet = NULL;
	DBG_({
		knh_stat_t *stat = ctx->stat;
		KNH_SYSLOG(ctx, LOG_DEBUG, "Statistics",
				"*method cache hit/miss %ld/%ld", stat->mtdCacheHit, stat->mtdCacheMiss);
		KNH_SYSLOG(ctx, LOG_DEBUG, "Statistics",
				"*formatter cache hit/miss %ld/%ld", stat->fmtCacheHit, stat->fmtCacheMiss);
		KNH_SYSLOG(ctx, LOG_DEBUG, "Statistics",
				"*translator cache hit/miss %ld/%ld", stat->trlCacheHit, stat->trlCacheMiss);
	})
	if(ctx->stat->usedMemorySize != 0) {
		KNH_SYSLOG(ctx, LOG_NOTICE, "MemoryLeak", "*size=%ld[bytes]", ctx->stat->usedMemorySize);
	}
	knh_bzero(share, sizeof(knh_share_t) + sizeof(knh_stat_t));
	free(share);
}

/* ------------------------------------------------------------------------ */

static Ctx *knh_getRootContext(Ctx *ctx)
{
	if(ctx->parent == ctx) return ctx;
	return knh_getRootContext(ctx->parent);
}

void knh_Context_traverse(Ctx *ctx, knh_Context_t *o, knh_Ftraverse ftr)
{
	if(knh_getRootContext(ctx) == (Ctx*)o) {
		knh_share_traverse(ctx, (knh_share_t*)o->share, ftr);
	}
	knh_CommonContext_traverse(ctx, (knh_Context_t*)o, ftr);
}

void knh_Context_free(Ctx *ctx, knh_Context_t* ctxo)
{
	knh_CommonContext_free(ctx, ctxo);
	if(knh_getRootContext(ctx) == (Ctx*)ctxo) {
		size_t i, j;
		for(i = 0; i < ctxo->share->ClassTBLSize; i++) {
			knh_ClassTBL_t *t = pClassTBL(ctx, i);
			knh_Array_t *a = t->methods;
			for(j = 0; j < knh_Array_size(a); j++) {
				knh_Method_toAbstract(ctx, a->methods[j]);
			}
		}
		knh_share_free(ctx, (knh_share_t*)ctxo->share);
		knh_bzero((void*)ctxo, sizeof(knh_Context_t));
		free((void*)ctxo);
	}
	else {
		knh_bzero((void*)ctxo, sizeof(knh_Context_t));
		knh_free(ctx, (void*)ctxo, sizeof(knh_Context_t));
	}
}

/* ======================================================================== */
/* [konohaapi] */

KNHAPI(konoha_t) konoha_open(size_t stacksize)
{
	konoha_t k = {KONOHA_MAGIC};
	konoha_init();
	{
		Ctx *ctx = new_RootContext();
		k.ctx = ctx;
	}
	return k;
}

/* ------------------------------------------------------------------------ */

void knh_traverseAll(Ctx* ctx, knh_Ftraverse ftr)
{
	ctx = knh_getRootContext(ctx);
	knh_Context_traverse(ctx, (knh_Context_t*)ctx, ftr);
}

/* ------------------------------------------------------------------------ */

KNHAPI(void) konoha_close(konoha_t konoha)
{
	Ctx* ctx = (knh_Context_t*)konoha.ctx;
	KONOHA_CHECK_(konoha);
	if(ctx->share->threadCounter > 1) {
		KNH_SYSLOG(ctx, LOG_ERR, "konoha_close", "*running_threads=%ld", ctx->share->threadCounter);
		return;
	}
	knh_showMemoryStat(ctx);
	knh_Context_traverse(ctx, (knh_Context_t*)ctx, knh_Object_RCsweep);
	((knh_Context_t*)ctx)->bufa = NULL; // necessary for KNH_SYSLOG
	knh_Context_free(ctx, (knh_Context_t*)ctx);
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
