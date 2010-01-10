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

#define KNH_TSTRUCT_C

#include"commons.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ======================================================================== */
/* [ContextTable] */

void knh_loadSystemString(Ctx *o); /* konoha_data.c */
void knh_loadSystemStructData(Ctx *o);
void knh_loadSystemData(Ctx *o);

#ifdef KNH_DBGMODE
#define FLAG_CTX(f)      (f|FLAG_Context_Verbose|FLAG_Context_Strict)
#else
#define FLAG_CTX(f)      (f|FLAG_Context_Strict)
#endif

/* ------------------------------------------------------------------------ */

static Ctx *new_hContext(Ctx *pctx)
{
	knh_Context_t *ctx;
	if(pctx == NULL) {
		ctx = (knh_Context_t*)malloc(sizeof(knh_Context_t));
		pctx = ctx;
	}
	else {
		ctx = (knh_Context_t*)KNH_MALLOC(pctx, sizeof(knh_Context_t));
	}
	knh_bzero(ctx, sizeof(knh_Context_t));
	ctx->h.magic = KNH_OBJECT_MAGIC;
#ifdef KNH_HOBJECT_REFC
	ctx->h.refc = KNH_RCGC_INIT;
#endif
	ctx->h.flag = 	FLAG_CTX(FLAG_Context);
	ctx->h.bcid = CLASS_Context;
	ctx->h.cid  = CLASS_Context;
	ctx->h.ctxid = 0;
	ctx->h.lock  = LOCK_NOP;
	ctx->ctxid = 0;
	ctx->unusedObject = NULL;
	ctx->unusedObjectSize = 0;
	ctx->parent = pctx;
	ctx->fsweep = knh_getDefaultSweepFunc();
	ctx->unusedContext = NULL;
	return (Ctx*)ctx;
}

/* ------------------------------------------------------------------------ */

static Object *new_Null(Ctx *ctx)
{
	knh_Glue_t *glue = (knh_Glue_t*)new_hObject(ctx, FLAG_Object_Immutable, CLASS_Any, CLASS_Any);
	knh_Glue_init(ctx, glue, NULL, NULL);
	return (Object*)glue;
}

static knh_Object_t *new_Boolean0(Ctx *ctx, knh_bool_t tf)
{
	knh_Boolean_t *o = (knh_Boolean_t*)new_hObject(ctx, FLAG_Boolean, CLASS_Boolean, CLASS_Boolean);
	o->n.bvalue = tf;
	return (knh_Object_t*)o;
}

/* ------------------------------------------------------------------------ */
/* [Shared Data] */

static void knh_initClassTable(knh_ClassTable_t *t, size_t s, size_t e)
{
	size_t i;
	knh_bzero(&t[s], SIZEOF_TCLASS(e-s));
	for(i = s; i < e; i++) {
		t[i].p1     = CLASS_Tvoid;
		t[i].p2       = CLASS_Tvoid;
		t[i].keyidx   = -1;
		t[i].keyidx2   = -1;
		t[i].fdefault = knh_fdefault__NEWVALUE;
	}
}

void knh_expandClassTable(Ctx *ctx)
{
	size_t s = ctx->share->ClassTableSize, max = ctx->share->ClassTableMax * 2;
	knh_ClassTable_t *newt = (knh_ClassTable_t*)KNH_MALLOC(ctx, SIZEOF_TCLASS(max));
	knh_memcpy(newt, ctx->share->ClassTable, SIZEOF_TCLASS(s));
	knh_initClassTable(newt, s, max);
	((knh_share_t*)ctx->share)->ClassTable = newt;
	((knh_share_t*)ctx->share)->ClassTableMax = max;
}

/* ------------------------------------------------------------------------ */

knh_class_t new_ClassId(Ctx *ctx)
{
	knh_class_t newid;
	KNH_LOCK(ctx, LOCK_SYSTBL, NULL);
	newid = ctx->share->ClassTableSize;
	if(ctx->share->ClassTableSize == ctx->share->ClassTableMax) {
		knh_expandClassTable(ctx);
	}
	((knh_share_t*)ctx->share)->ClassTableSize = newid + 1;
	KNH_UNLOCK(ctx, LOCK_SYSTBL, NULL);
	return newid;
}


/* ------------------------------------------------------------------------ */

void knh_expandExptTable(Ctx *ctx)
{
	size_t s = ctx->share->ExptTableSize, max = ctx->share->ExptTableMax * 2;
	knh_ExptTable_t *newt = (knh_ExptTable_t*)KNH_MALLOC(ctx, SIZEOF_TEXPT(max));
	knh_bzero(newt, SIZEOF_TEXPT(max));
	knh_memcpy(newt, ctx->share->ExptTable, SIZEOF_TEXPT(s));
	((knh_share_t*)ctx->share)->ExptTable = newt;
	((knh_share_t*)ctx->share)->ExptTableMax = max;
}

/* ------------------------------------------------------------------------ */

static void knh_initSharedData(knh_Context_t *ctx)
{
	size_t i;
	knh_share_t *share = (knh_share_t*)malloc(sizeof(knh_share_t) + sizeof(knh_stat_t));
	ctx->share = share;
	knh_bzero(share, sizeof(knh_share_t) + sizeof(knh_stat_t));
	share->threadCounter = 1;
	ctx->stat = (knh_stat_t*)((share+1));

	share->ObjectPageTableMax = KNH_TOBJECTPAGE_INITSIZE;
	share->ObjectPageTable =
		(knh_ObjectPageTable_t*)KNH_MALLOC(ctx,
			share->ObjectPageTableMax * sizeof(knh_ObjectPageTable_t));
	knh_bzero(share->ObjectPageTable,
		share->ObjectPageTableMax * sizeof(knh_ObjectPageTable_t));
	share->ObjectPageTableSize = 0;

	share->LockTable = (knh_LockTable_t*)KNH_MALLOC(ctx, SIZEOF_TLOCK);
	knh_bzero(share->LockTable, SIZEOF_TLOCK);
	for(i = 0; i < KNH_TLOCK_SIZE; i++) {
		share->LockTable[i].mutex = (knh_mutex_t*)KNH_MALLOC(ctx, sizeof(knh_mutex_t));
		knh_mutex_init(share->LockTable[i].mutex);
		if(LOCK_UNUSED <= i && i < KNH_TLOCK_SIZE - 1) {
			share->LockTable[i].unused = &(share->LockTable[i+1]);
		}
	}
	share->unusedLockTable = &(share->LockTable[LOCK_UNUSED]);

	share->ClassTable = (knh_ClassTable_t*)KNH_MALLOC((Ctx*)ctx, SIZEOF_TCLASS(KNH_CLASSTABLE_INIT));
	knh_initClassTable((knh_ClassTable_t*)share->ClassTable, 0, KNH_CLASSTABLE_INIT);
	share->ClassTableSize = 0;
	share->ClassTableMax  = KNH_CLASSTABLE_INIT;

	share->ExptTable = (knh_ExptTable_t*)KNH_MALLOC(ctx, SIZEOF_TEXPT(KNH_EXPTTABLE_INIT));
	knh_bzero((void*)share->ExptTable, SIZEOF_TEXPT(KNH_EXPTTABLE_INIT));
	share->ExptTableSize = 0;
	share->ExptTableMax  = KNH_EXPTTABLE_INIT;

	knh_loadSystemStructData(ctx);
	KNH_INITv(share->constNull, new_Null(ctx));
	KNH_INITv(share->constTrue, new_Boolean0(ctx, 1));
	KNH_INITv(share->constFalse, new_Boolean0(ctx, 0));
	{
		knh_Int_t *io = (knh_Int_t*)new_hObject(ctx, FLAG_Float, CLASS_Int, CLASS_Int);
		(io)->n.ivalue = 0;
		KNH_INITv(share->constInt0, io);
	}
	{
		knh_Float_t *fo = (knh_Float_t*)new_hObject(ctx, FLAG_Float, CLASS_Float, CLASS_Float);
		(fo)->n.fvalue = KNH_FLOAT_ZERO;
		KNH_INITv(share->constFloat0, fo);
	}

	KNH_ASSERT(share->tString == NULL);
	share->tString = (knh_String_t**)KNH_MALLOC(ctx, SIZEOF_TSTRING);
	knh_bzero(share->tString, SIZEOF_TSTRING);
	knh_loadSystemString(ctx);

	/* These are not shared, but needed to initialize System*/
	KNH_INITv(ctx->bufa, new_Bytes(ctx, KNH_PAGESIZE));
	KNH_INITv(ctx->bufw, new_BytesOutputStream(ctx, ctx->bufa));
	KNH_INITv(ctx->sys, new_Object_bcid(ctx, CLASS_System, 0));
	knh_loadSystemData(ctx);

	KNH_INITv(share->mainns, new_NameSpace(ctx, NULL));
	KNH_INITv(ctx->script, new_Script(ctx, share->mainns, CLASS_Script, CLASS_Object));
	share->ctx0 = ctx;
}

/* ------------------------------------------------------------------------ */

static void knh_ObjectPageTable_free(Ctx *ctx, char *opage)
{
	char *t = opage, *max = opage + SIZEOF_OBJECTPAGE;
	if((knh_uintptr_t)t % KNH_PAGESIZE != 0) {
		t = (char*)((((knh_uintptr_t)t / KNH_PAGESIZE) + 1) * KNH_PAGESIZE);
		KNH_ASSERT((knh_uintptr_t)t % KNH_PAGESIZE == 0);
	}
	while(t + KNH_PAGESIZE < max) {
		size_t i;
		for(i = 1; i < (KNH_PAGESIZE / sizeof(knh_Object_t)); i++) {
			Object *o = ((knh_Object_t*)t) + i;
			if(o->h.magic != KNH_OBJECT_MAGIC) continue;
#ifdef KNH_HOBJECT_REFC
			DBG_(
			{
				switch(o->h.bcid) {
				case CLASS_Class:
					fprintf(stderr, "async mem cid=%d, refc=%d\n", (int)((knh_Class_t*)o)->cid, (int)o->h.refc);
				break;
				case CLASS_Int:
					fprintf(stderr, "async mem cid=%s(%d), refc=%d ivalue='%d'\n", STRUCTN(o->h.bcid), (int)o->h.cid, (int)o->h.refc, (int)((knh_Int_t*)o)->n.ivalue);
				break;
				case CLASS_String:
					fprintf(stderr, "async mem cid=%s(%d), refc=%d str='%s'\n", STRUCTN(o->h.bcid), (int)o->h.cid, (int)o->h.refc, (char*)((knh_String_t*)o)->str);
				break;
				case CLASS_Stmt:
					fprintf(stderr, "async mem stmt='%s', refc=%d\n", knh_stmt_tochar((SP(knh_Stmt_t*)o)->stt), (int)o->h.refc);
				break;
				default:
					fprintf(stderr, "async mem cid=%s(%d), refc=%d\n", STRUCTN(o->h.bcid), (int)o->h.cid, (int)o->h.refc);
				}
			} );
			o->h.refc = 0;
#endif
			knh_Object_free(ctx, o);
		}
		t += KNH_PAGESIZE;
	}
}

static void knh_traverseSharedData(Ctx *ctx, knh_share_t *share, knh_Ftraverse ftr)
{
	int i;
	ftr(ctx, share->constNull);
	ftr(ctx, share->constTrue);
	ftr(ctx, share->constFalse);
	ftr(ctx, UP(share->constInt0));
	ftr(ctx, UP(share->constFloat0));
	ftr(ctx, UP(ctx->sys));
	ftr(ctx, UP(share->mainns));

	for(i = 0; i < KNH_TSTRING_SIZE; i++) {
		ftr(ctx, UP(share->tString[i]));
	}

	for(i = 0; i < KNH_TLOCK_SIZE; i++) {
		knh_mutex_destroy(share->LockTable[i].mutex);
		KNH_FREE(ctx, share->LockTable[i].mutex, sizeof(knh_mutex_t));
		if(share->LockTable[i].name != NULL) {
			ftr(ctx, UP(share->LockTable[i].name));
			if(share->LockTable[i].so != NULL) {
				ftr(ctx, share->LockTable[i].so);
			}
		}
	}

	for(i = 0; i < (int)(share->ExptTableSize); i++) {
		if(ExptTable(i).name != NULL) {
			ftr(ctx, UP(ExptTable(i).name));
		}
	}

	/* tclass */
	if(IS_SWEEP(ftr)) {
		knh_ClassTable_t *t = pClassTable(ctx, i);
		for(i = 0; i < share->ClassTableSize; i++) {
			size_t j;
			for(j = 0; j < knh_Array_size(t->methods); j++) {
				knh_Method_toAbstract(ctx, (knh_Method_t*)knh_Array_n(t->methods, j));
			}
		}
	}

	for(i = 0; i < share->ClassTableSize; i++) {
		knh_ClassTable_t *t = pClassTable(ctx, i);
		DBG2_ASSERT(t->sname != NULL);
		if(t->class_cid != NULL) ftr(ctx, UP(t->class_cid));
		if(t->class_natype != NULL) ftr(ctx, UP(t->class_natype));
		ftr(ctx, UP(t->methods));
		if(t->fields != NULL) {
			size_t j;
			for(j = 0; j < t->fsize; j++) {
				if(t->fields[j].value != NULL)
					ftr(ctx, t->fields[j].value);
			}
			DBG2_P("traverse cid=%d, t->fsize=%d, t->fields=%p\n", i, t->fsize, t->fields);
		}
		ftr(ctx, UP(t->cmap));
		if(t->cspec != NULL) {
			ftr(ctx, UP(t->cspec));
		}
		if(t->constDictMap != NULL) {
			ftr(ctx, UP(t->constDictMap));
		}
		ftr(ctx, UP(t->sname));
		ftr(ctx, UP(t->lname));
	}

	/* System Table */
	if(IS_SWEEP(ftr)) {
		KNH_FREE(ctx, (void*)share->ExptTable, SIZEOF_TEXPT(ctx->share->ExptTableMax));
		share->ExptTable = NULL;
		KNH_FREE(ctx, share->tString, SIZEOF_TSTRING);
		share->tString = NULL;

		((knh_Context_t*)ctx)->fsweep = knh_Object_finalSweep;
		KNH_ASSERT(share->ObjectPageTable != NULL);
		for(i = 0; i < (int)(share->ObjectPageTableSize); i++) {
			knh_ObjectPageTable_free(ctx, share->ObjectPageTable[i].opage);
		}

		for(i = 0; i < share->ClassTableSize; i++) {
			knh_ClassTable_t *t = pClassTable(ctx, i);
			if(t->fields != NULL) {
				KNH_FREE(ctx, t->fields, sizeof(knh_fields_t) * t->fsize);
				t->fields = NULL;
			}
		}
		KNH_FREE(ctx, (void*)share->ClassTable, SIZEOF_TCLASS(share->ClassTableMax));
		share->ClassTable = NULL;

		KNH_ASSERT(share->ObjectPageTable != NULL);
		for(i = 0; i < (int)(share->ObjectPageTableSize); i++) {
			KNH_FREE(ctx, share->ObjectPageTable[i].opage, SIZEOF_OBJECTPAGE);
			share->ObjectPageTable[i].opage = NULL;
		}

		KNH_FREE(ctx, share->ObjectPageTable,
				share->ObjectPageTableMax * sizeof(knh_ObjectPageTable_t));
		share->ObjectPageTable = NULL;

		KNH_ASSERT(share->LockTable != NULL);
		KNH_FREE(ctx, share->LockTable, SIZEOF_TLOCK);
		share->unusedLockTable = NULL;

		if(ctx->stat->usedMemorySize != 0) {
			KNH_SYSLOG0(ctx, LOG_WARNING, "memory leaks: %d bytes", (int)ctx->stat->usedMemorySize);
		}

		{
			knh_stat_t *stat = ctx->stat;
			KNH_SYSLOG0(ctx, LOG_INFO,
				"method cache hit/miss %d/%d", (int)stat->mtdCacheHit, (int)stat->mtdCacheMiss);
			KNH_SYSLOG0(ctx, LOG_INFO,
				"formatter cache hit/miss %d/%d", (int)stat->fmtCacheHit, (int)stat->fmtCacheMiss);
			KNH_SYSLOG0(ctx, LOG_INFO,
				"mapper cache hit/miss %d/%d", (int)stat->mprCacheHit, (int)stat->mprCacheMiss);
		}
		knh_bzero(share, sizeof(knh_share_t) + sizeof(knh_stat_t));
		free(share);
	}
}

/* ------------------------------------------------------------------------ */
/* [ContextCommon] */

static
void knh_Context_initCommon(Ctx *ctx, knh_Context_t *o, size_t stacksize)
{
	o->stacksize = (stacksize < 128) ? KONOHA_STACKSIZE : stacksize;
	o->stack = (knh_sfp_t*)KNH_MALLOC(ctx, sizeof(knh_sfp_t) * o->stacksize);
	o->esp = o->stack;
	{
		size_t i;
		for(i = 0; i < o->stacksize; i++) {
			KNH_INITv(o->stack[i].o, KNH_NULL);
			o->stack[i].data = 0;
		}
	}

	o->cachesize = 509 /* prime number */;
	o->mtdCache = (knh_Method_t**)KNH_MALLOC(ctx, sizeof(knh_Method_t*) * o->cachesize);
	o->fmtCache = (knh_Method_t**)KNH_MALLOC(ctx, sizeof(knh_Method_t*) * o->cachesize);
	o->mprCache = (knh_Mapper_t**)KNH_MALLOC(ctx, sizeof(knh_Mapper_t*) * o->cachesize);
	{
		size_t i;
		knh_Method_t *mtdInit = NULL;
		knh_Method_t *fmtInit = NULL;
		knh_Mapper_t *mprInit = NULL;
		for(i = 0; i < o->cachesize; i++) {
			o->mtdCache[i] = mtdInit;
			o->fmtCache[i] = fmtInit;
			o->mprCache[i] = mprInit;
		}
	}
	if(o->bufa == NULL) {
		KNH_INITv(o->bufa, new_Bytes(ctx, KNH_PAGESIZE));
		KNH_INITv(o->bufw, new_BytesOutputStream(ctx, o->bufa));
	}

	KNH_INITv(o->bconvbuf, new_Bytes(ctx, KNH_PAGESIZE));
	KNH_INITv(o->props, new_DictMap0(ctx, 16));

	DBG2_ASSERT(ctx->sys != NULL);
	if(o->script == NULL) {
		KNH_INITv(o->script, new_Script(ctx, ctx->share->mainns, new_ClassId(ctx), CLASS_Script));
	}
	KNH_INITv(o->enc, DP(ctx->sys)->enc);
	KNH_INITv(o->in,  DP(ctx->sys)->in);
	KNH_INITv(o->out, DP(ctx->sys)->out);
	KNH_INITv(o->err, DP(ctx->sys)->err);

	o->ctxlock = (knh_mutex_t*)KNH_MALLOC(ctx, sizeof(knh_mutex_t));
	knh_bzero(o->ctxlock, sizeof(knh_mutex_t));
	knh_mutex_init(o->ctxlock);

	if(ctx->ctxid == 0) {
		knh_Gamma_t *kc = (knh_Gamma_t*)new_Object_bcid(ctx, CLASS_Gamma, 0);
		KNH_INITv(o->kc, kc);
		KNH_INITv(DP(kc)->script, o->script);
		KNH_INITv(DP(kc)->symbolDictMap, new_DictMap0(ctx, 256));
		KNH_INITv(DP(kc)->constPools, new_Array0(ctx, 0));
	}
	else {
		KNH_INITv(o->kc, ctx->kc);
	}
	KNH_INITv(o->msgError, UP(TS_EMPTY));
}

/* ------------------------------------------------------------------------ */

static
void knh_Context_traverseCommon(Ctx *ctx, knh_Context_t *o, knh_Ftraverse ftr)
{
	size_t i;
	for(i = 0; i < o->stacksize; i++) {
		ftr(ctx, o->stack[i].o);
	}

	ftr(ctx, UP(o->script));
	ftr(ctx, UP(o->bufa));
	ftr(ctx, UP(o->bufw));
	ftr(ctx, UP(o->bconvbuf));
	ftr(ctx, UP(o->props));

	ftr(ctx, UP(o->enc));
	ftr(ctx, UP(o->in));
	ftr(ctx, UP(o->out));
	ftr(ctx, UP(o->err));
	ftr(ctx, UP(o->kc));
	ftr(ctx, UP(o->msgError));

	if(IS_SWEEP(ftr)) {
		DBG2_P("freeing stack (ctxid=%d)...", o->ctxid);
		KNH_FREE(ctx, o->stack, sizeof(knh_sfp_t) * o->stacksize);
		o->stack = NULL;
		o->esp = NULL;
		KNH_FREE(ctx, o->mtdCache, sizeof(knh_Method_t*) * o->cachesize);
		KNH_FREE(ctx, o->fmtCache, sizeof(knh_Method_t*) * o->cachesize);
		KNH_FREE(ctx, o->mprCache, sizeof(knh_Mapper_t*) * o->cachesize);
		o->mtdCache = NULL;
		o->fmtCache = NULL;
		o->mprCache = NULL;
		knh_mutex_destroy(o->ctxlock);
		KNH_FREE(ctx, o->ctxlock, sizeof(knh_mutex_t));
		o->ctxlock = NULL;
	} else {
		ftr(ctx, UP(o->lines));
	}
}

/* ------------------------------------------------------------------------ */
/* [LOCKTABLE] */

void knh_lockID(Ctx *ctx, knh_lock_t lockid, Object *ref, char *filename, int lineno)
{
	knh_LockTable_t *t = pLockTable(lockid);
	if(t->mutex != NULL) {
		//DBG2_(fprintf(stderr, "LOCK[%d] count=%d at %s:%d\n", (int)lockid, (int)(t->count + 1), filename, lineno));
		knh_mutex_lock(t->mutex);
		t->count += 1;
		DBG2_({
			t->filename = filename;
			t->lineno = lineno;
		});
	}
}

/* ------------------------------------------------------------------------ */

void knh_unlockID(Ctx *ctx, knh_lock_t lockid, char *filename, int lineno)
{
	knh_LockTable_t *t = pLockTable(lockid);
	if(t->mutex != NULL) {
		//DBG2_(fprintf(stderr, "UNLOCK[%d] count=%d at %s:%d\n", (int)lockid, (int)t->count, filename, lineno));
		t->count -= 1;
		knh_mutex_unlock(t->mutex);
	}
}

/* ------------------------------------------------------------------------ */

static void knh_traverseUnusedContext(Ctx *ctx, knh_Ftraverse ftr)
{
	if(ctx->unusedContext != NULL) {
		knh_traverseUnusedContext(ctx->unusedContext, ftr);
	}
	knh_Context_traverseCommon(ctx, (knh_Context_t*)ctx, ftr);
	if(IS_SWEEP(ftr)) {
		knh_bzero((void*)ctx, sizeof(knh_Context_t));
		knh_free(ctx, (void*)ctx, sizeof(knh_Context_t));
	}
}

/* ------------------------------------------------------------------------ */

void knh_Context_traverse(Ctx *ctx, knh_Context_t *o, knh_Ftraverse ftr)
{
	knh_Context_traverseCommon(ctx, (knh_Context_t*)o, ftr);
	if(o->parent == (knh_Context_t*)o) {
		if(o->unusedContext != NULL) {
			knh_traverseUnusedContext(o->unusedContext, ftr);
		}
		knh_traverseSharedData(ctx, (knh_share_t*)o->share, ftr);
		if(IS_SWEEP(ftr)) {
			knh_bzero((void*)o, sizeof(knh_Context_t));
			free((void*)o);
		}
	}
	else {
		if(IS_SWEEP(ftr)) {
			knh_bzero((void*)o, sizeof(knh_Context_t));
			knh_free(o, (void*)o, sizeof(knh_Context_t));
		}
	}
}

/* ------------------------------------------------------------------------ */

static Ctx *knh_getRootContext(Ctx *ctx)
{
	if(ctx->parent == ctx) return ctx;
	return knh_getRootContext(ctx->parent);
}

///* ------------------------------------------------------------------------ */
//
//Ctx *new_ThreadContext(Ctx *parent)
//{
//	Ctx *root = knh_getRootContext(parent);
//	knh_Context_t *ctx = NULL;
//	if(root->unusedContext != NULL) {
//		KNH_LOCK(ctx, LOCK_SYSTBL, NULL);
//		ctx = (knh_Context_t*)root->unusedContext;
//		if(ctx != NULL) {
//			((knh_Context_t*)root)->unusedContext = ctx->unusedContext;
//		}
//		KNH_UNLOCK(ctx, LOCK_SYSTBL, NULL);
//		if(ctx != NULL) {
//			DBG2_P("reuseing Context: ctxid=%d", (int)ctx->ctxid);
//			ctx->parent = parent;
//			ctx->esp = ctx->stack;
//			ctx->fsweep = knh_getDefaultSweepFunc();
//			KNH_SETv(root, ctx->props, new_DictMap0(root, 16));
//			KNH_SETv(root, ctx->enc, DP(root->sys)->enc);
//			KNH_SETv(root, ctx->in,  DP(root->sys)->in);
//			KNH_SETv(root, ctx->out, DP(root->sys)->out);
//			KNH_SETv(root, ctx->err, DP(root->sys)->err);
//			KNH_SETv(root, ctx->kc, KNH_NULL);
//		}
//	}
//	if(ctx == NULL) {
//		ctx = (knh_Context_t*)new_hContext(parent);
//		ctx->share = parent->share;
//		ctx->stat = parent->stat;
//		ctx->sys  = parent->sys;
//		knh_Context_initCommon(root, ctx, parent->stacksize);
//		((knh_share_t*)ctx->share)->contextCounter += 1;
//		ctx->ctxid = (knh_ushort_t)ctx->share->contextCounter;
//	}
//	KNH_LOCK(ctx, LOCK_SYSTBL, NULL);
//	((knh_share_t*)ctx->share)->threadCounter += 1;
//	KNH_UNLOCK(ctx, LOCK_SYSTBL, NULL);
//	if(ctx->share->threadCounter == 2) {
//		DBG2_P("Activating multi-threading mode!!");
//
//	}
//	return (Ctx*)ctx;
//}
//
///* ------------------------------------------------------------------------ */
//
//void knh_ThreadContext_dispose(Ctx *ctx)
//{
//	KNH_ASSERT(ctx->parent != ctx);
//	Ctx *root = knh_getRootContext(ctx);
//	knh_mutex_lock((((knh_Context_t*)ctx)->ctxlock));  // checking using or not
//	KNH_LOCK(ctx, LOCK_SYSTBL, NULL);
//	((knh_Context_t*)ctx)->unusedContext = root->unusedContext;
//	((knh_Context_t*)root)->unusedContext = ctx;
//	((knh_share_t*)ctx->share)->threadCounter -= 1;
//	KNH_UNLOCK(ctx, LOCK_SYSTBL, NULL);
//	knh_mutex_unlock((((knh_Context_t*)ctx)->ctxlock));
//}

/* ======================================================================== */
/* [konohaapi] */

KNHAPI(konoha_t) konoha_open(size_t stacksize)
{
	konoha_t k = {KONOHA_MAGIC};
	konoha_init();  // harmless
	if(stacksize < KONOHA_STACKSIZE) stacksize = KONOHA_STACKSIZE;
	{
		Ctx *ctx = new_hContext(NULL);
		knh_initSharedData((knh_Context_t*)ctx);
		knh_Context_initCommon(ctx, (knh_Context_t*)ctx, stacksize);
		knh_System_initPath(ctx, ctx->sys);
		k.ctx = ctx;
	}
	return k;
}

/* ------------------------------------------------------------------------ */

void knh_traverseAll(Ctx* ctx, knh_Ftraverse ftr)
{
	knh_Context_traverse(ctx, (knh_Context_t*)ctx, ftr);
}

/* ------------------------------------------------------------------------ */

KNHAPI(void) konoha_close(konoha_t konoha)
{
	KONOHA_CHECK_(konoha);
	knh_Context_t *ctx = (knh_Context_t*)konoha.ctx;
	if(ctx->share->threadCounter > 1) {
		fprintf(stderr, "Many threads are still running... Found %d threads", (int)ctx->share->threadCounter);
		return;
	}
//	{
//		knh_ObjectField_t *scr = (knh_ObjectField_t*)ctx;
//		ClassTable(CLASS_ObjectField).cspi->traverse(ctx, UP(scr), ctx->fsweep);
//		(scr)->h.cid = CLASS_Object;
//		(scr)->h.bcid = CLASS_Object;
//		KNH_FREE(ctx, scr->fields, sizeof(Object*) * KNH_SCRIPT_FIELDSIZE);
//	}
	knh_Context_traverse(konoha.ctx, ctx, ctx->fsweep);
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
