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

#define KNH_TOBJECT_C  1

#include"commons.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

#define KNH_STATMODE0   -1  /* @property */

/* ======================================================================== */
/* [malloc] */

#ifdef KNH_DEBUGMODE2
static int canFree = 1;
#define DBG_ENABLE_FREE()     canFree = 1
#define DBG_DISABLE_FREE()    canFree = 0
#define DBG_ASSERT_FREE()     KNH_ASSERT(canFree)
#else
#define DBG_ENABLE_FREE()
#define DBG_DISABLE_FREE()
#define DBG_ASSERT_FREE()
#endif

/* ------------------------------------------------------------------------ */

void *knh_malloc(Ctx *ctx, size_t size)
{
	KNH_ASSERT(size > 0);
	void *block = malloc(size);
	if (unlikely(block == NULL)) {
		DBG2_ABORT();
		KNH_EXIT("OutOfMemory!!: %d bytes used", (int)ctx->stat->usedMemorySize);
		//KNH_THROW__T(ctx, "OutOfMemory!!");
	}
	knh_stat_incUsedMemorySize(ctx, size);
	return block;
}

/* ------------------------------------------------------------------------ */

void knh_free(Ctx *ctx, void *block, size_t size)
{
	DBG_ASSERT_FREE();
	SECURE_bzero(block, size);
	free(block);
	knh_stat_dclUsedMemorySize(ctx, size);
}

/* ------------------------------------------------------------------------ */

void *DBG2_malloc(Ctx *ctx, size_t size LOGARG)
{
	DBG2_ASSERT(size != 0);
	size_t *block = (size_t*)malloc(size + sizeof(size_t));
	if (unlikely(block == NULL)) {
		DBG2_ABORT();
		KNH_EXIT("OutOfMemory!!: %d bytes used", (int)ctx->stat->usedMemorySize);
		//KNH_THROW__T(ctx, "OutOfMemory!!");
	}
	knh_stat_incUsedMemorySize(ctx, size);
	block[0] = size;
	return (void*)(block + 1);
}

/* ------------------------------------------------------------------------ */

void DBG2_free(Ctx *ctx, void *p, size_t size LOGARG)
{
	DBG_ASSERT_FREE();
	DBG2_ASSERT(size != 0);
	size_t *block = ((size_t*)p) - 1;
	if(unlikely(size != block[0])) {
		KNH_SYSLOG2(ctx, LOG_ERR, "free: ptr = %p, block.size = %d, free.size=%d", p, (int)block[0], (int)size)
		KNH_ASSERT(size == block[0]);
	}
	block[0]=0;
	SECURE_bzero(block, size + sizeof(size_t));
	free(block);
	knh_stat_dclUsedMemorySize(ctx, size);
}

/* ======================================================================== */
/* [tObject] */

#define KNH_CHECK_UNUSED_OBJECT(ctx) { \
		if(unlikely(ctx->unusedObject == NULL)) { \
			KNH_ASSERT(ctx->unusedObjectSize == 0); \
			((knh_Context_t*)ctx)->unusedObject = new_UnusedObject(ctx); \
		} \
		DBG2_checkObjectPage(ctx, ctx->unusedObject LOGDATA);\
	} \

#define KNH_OBJECT_REUSE(ctx, used) { \
		DBG2_checkObjectPage(ctx, used LOGDATA);\
		used->ref = ctx->unusedObject;\
		((knh_Context_t*)ctx)->unusedObject = used;\
		((knh_Context_t*)ctx)->unusedObjectSize += 1;\
	}\


/* ------------------------------------------------------------------------ */

static void DBG2_checkObjectPage(Ctx *ctx, void *used LOGARG)
{
	knh_share_t *ctxshare = (knh_share_t*)ctx->share;
	int pageindex;
	for(pageindex = 0; pageindex < ctxshare->ObjectPageTableSize; pageindex++) {
		void *p = ctxshare->ObjectPageTable[pageindex].opage;
		if(p < used && used < p + SIZEOF_OBJECTPAGE) return;
	}
	KNH_SYSLOG2(ctx, LOG_ERR, "not paged object %p", used);
}

static knh_Object_t *new_UnusedObject(Ctx *ctx)
{
	KNH_LOCK(ctx, LOCK_MEMORY, NULL);
	knh_share_t *ctxshare = (knh_share_t*)ctx->share;
	int pageindex = ctxshare->ObjectPageTableSize;
	char *opage, *opage_max;
	if(unlikely(!(pageindex < ctxshare->ObjectPageTableMax))) {
		size_t newsize = ctxshare->ObjectPageTableMax * 2;
		knh_ObjectPageTable_t *newpage
			= (knh_ObjectPageTable_t*)KNH_MALLOC(ctx, sizeof(knh_ObjectPageTable_t) * newsize);
		knh_bzero(newpage, sizeof(knh_ObjectPageTable_t) * newsize);
		knh_memcpy(newpage, ctxshare->ObjectPageTable, ctxshare->ObjectPageTableMax);
		KNH_FREE(ctx, ctxshare->ObjectPageTable, ctxshare->ObjectPageTableMax);
		ctxshare->ObjectPageTable = newpage;
		ctxshare->ObjectPageTableMax = newsize;
	}
	ctxshare->ObjectPageTableSize += 1;
	KNH_UNLOCK(ctx, LOCK_MEMORY, NULL);

	opage = (char*)KNH_MALLOC(ctx, SIZEOF_OBJECTPAGE);
	opage_max = opage + SIZEOF_OBJECTPAGE;
	knh_bzero(opage, SIZEOF_OBJECTPAGE);
	ctxshare->ObjectPageTable[pageindex].ctxid = ctx->ctxid;
	ctxshare->ObjectPageTable[pageindex].opage = opage;

	if((knh_uintptr_t)opage % KNH_PAGESIZE != 0) {
		char *t2 = (char*)((((knh_uintptr_t)opage / KNH_PAGESIZE) + 1) * KNH_PAGESIZE);
		DBG2_P("KNH_PAGESIZE=%d, shift=%d", (int)KNH_PAGESIZE, (int)(t2 - opage));
		opage = t2;
		KNH_ASSERT((knh_uintptr_t)opage % KNH_PAGESIZE == 0);
	}
	{
		int cnt = 0;
		DBG2_ASSERT(ctx->unusedObject == NULL);
		((knh_Context_t*)ctx)->unusedObject = ((knh_Object_t*)opage) + 1;
		while(opage + KNH_PAGESIZE < opage_max) {
			size_t i;
			knh_Object_t *o = (knh_Object_t*)opage;
			for(i = 1; i < (KNH_PAGESIZE / sizeof(knh_Object_t)) - 1; i++) {
				DBG2_({
				void *p = (void*)&(o[i]);
				knh_uintptr_t *b = (knh_uintptr_t*)((((knh_uintptr_t)p) / KNH_PAGESIZE) * KNH_PAGESIZE);
				DBG2_ASSERT((void*)b == (void*)o);});
				o[i].ref = &(o[i+1]);
				cnt++;
			}
			opage += KNH_PAGESIZE;
			if(opage + KNH_PAGESIZE < opage_max) {
				o[KNH_PAGESIZE / sizeof(knh_Object_t) - 1].ref = ((knh_Object_t*)opage) + 1;
			}
			else {
				o[KNH_PAGESIZE / sizeof(knh_Object_t) - 1].ref = NULL;
			}
			cnt++;
		}
		((knh_Context_t*)ctx)->unusedObjectSize += cnt;
	}
	KNH_SYSLOG(ctx, LOG_INFO, "new opage tindex=%d(%p-%p), ctxid=%d size=%d",
			pageindex, ctxshare->ObjectPageTable[pageindex].opage, opage_max, ctx->ctxid, (int)ctx->unusedObjectSize);
	return ctx->unusedObject;
}

/* ------------------------------------------------------------------------ */
/* [fastmalloc] */

static int knh_isFastMallocMemory(void *p)
{
	knh_uintptr_t *b = (knh_uintptr_t*)((((knh_uintptr_t)p) / KNH_PAGESIZE) * KNH_PAGESIZE);
	int n = ((Object*)p - (Object*)b);
	int x = n / (sizeof(knh_uintptr_t)*8);
	knh_uintptr_t mask = ((knh_uintptr_t)1) << (n % (sizeof(knh_uintptr_t)*8));
	return ((b[x] & mask) == mask);
}

static void knh_setFastMallocMemory(void *p)
{
	knh_uintptr_t *b = (knh_uintptr_t*)((((knh_uintptr_t)p) / KNH_PAGESIZE) * KNH_PAGESIZE);
	int n = ((Object*)p - (Object*)b);
	int x = n / (sizeof(knh_uintptr_t)*8);
	knh_uintptr_t mask = ((knh_uintptr_t)1) << (n % (sizeof(knh_uintptr_t)*8));
	KNH_ASSERT((b[x] & mask) != mask);
	b[x] = b[x] | mask;
	KNH_ASSERT((b[x] & mask) == mask);
}

static void knh_unsetFastMallocMemory(void *p)
{
	knh_uintptr_t *b = (knh_uintptr_t*)((((knh_uintptr_t)p) / KNH_PAGESIZE) * KNH_PAGESIZE);
	int n = ((Object*)p - (Object*)b);
	int x = n / (sizeof(knh_uintptr_t)*8);
	knh_uintptr_t mask = ((knh_uintptr_t)1) << (n % (sizeof(knh_uintptr_t)*8));
	KNH_ASSERT((b[x] & mask) == mask);
	b[x] = b[x] & ~(mask);
	KNH_ASSERT((b[x] & mask) != mask);
}

/* ------------------------------------------------------------------------ */

void *knh_fastmalloc(Ctx *ctx, size_t size)
{
	DBG2_ASSERT(size != 0);
	if(size <= KNH_FASTMALLOC_SIZE) {
		KNH_CHECK_UNUSED_OBJECT(ctx);
		Object *o = ctx->unusedObject;
		((knh_Context_t*)ctx)->unusedObject = (knh_Object_t*)o->ref;
		((knh_Context_t*)ctx)->unusedObjectSize -= 1;
		knh_setFastMallocMemory((void*)o);
		return (void*)o;
	}
	else {
		void *block = malloc(size);
		if (unlikely(block == NULL)) {
			DBG2_ABORT();
			KNH_EXIT("OutOfMemory!!: %d bytes used", (int)ctx->stat->usedMemorySize);
			//KNH_THROW__T(ctx, "OutOfMemory!!");
		}
		knh_stat_incUsedMemorySize(ctx, size);
		return block;
	}
}

/* ------------------------------------------------------------------------ */

void knh_fastfree(Ctx *ctx, void *block, size_t size)
{
	DBG_ASSERT_FREE();
	if(size <= KNH_FASTMALLOC_SIZE) {
		knh_Object_t *o = (knh_Object_t*)block;
		DBG2_checkObjectPage(ctx, block LOGDATA);
		o->ref = ctx->unusedObject;
		((knh_Context_t*)ctx)->unusedObject = o;
		((knh_Context_t*)ctx)->unusedObjectSize += 1;
		o->h.magic = 0;
		knh_unsetFastMallocMemory(block);
	}
	else {
		SECURE_bzero(block, size);
		free(block);
		knh_stat_dclUsedMemorySize(ctx, size);
	}
}

//void UTEST_fastmalloc(Ctx *ctx)
//{
//	int i = 0;
//	void *a[10000] = {NULL};
//	for(i = 0; i < 10000; i++) {
//		a[i] = knh_fastmalloc(ctx, KNH_FASTMALLOC_SIZE);
//		memset(a[i], i % 128, KNH_FASTMALLOC_SIZE);
//	}
//
//	for(i = 0; i < 10000; i++) {
//		knh_bzero(a[i], KNH_FASTMALLOC_SIZE);
//		knh_fastfree(ctx, a[i], KNH_FASTMALLOC_SIZE);
//	}
//}

/* ------------------------------------------------------------------------ */
/* [hObject] */

knh_Object_t *new_hObject(Ctx *ctx, knh_flag_t flag, knh_class_t bcid, knh_class_t cid)
{
	DBG2_ASSERT(bcid != CLASS_Context);
	KNH_CHECK_UNUSED_OBJECT(ctx);
	{
		knh_Object_t *o = ctx->unusedObject;
		((knh_Context_t*)ctx)->unusedObject = (knh_Object_t*)o->ref;
		((knh_Context_t*)ctx)->unusedObjectSize -= 1;
		knh_stat_incUsedObjectSize(ctx, 1);
		o->h.magic = KNH_OBJECT_MAGIC;
		knh_Object_RCset(o, KNH_RCGC_INIT);
		o->h.flag = flag;
		o->h.bcid = bcid;
		o->h.cid  = cid;
		o->h.ctxid = ctx->ctxid;
		o->h.lock  = LOCK_NOP;
		DBG2_({o->ref = NULL;});
		return o;
	}
}

/* ------------------------------------------------------------------------ */

knh_Object_t *new_Object_bcid(Ctx *ctx, /*knh_flag_t flag,*/ knh_class_t bcid, int init)
{
	DBG2_ASSERT(bcid != CLASS_Context);
	KNH_CHECK_UNUSED_OBJECT(ctx);
	{
		knh_Object_t *o = ctx->unusedObject;
		((knh_Context_t*)ctx)->unusedObject = (knh_Object_t*)o->ref;
		((knh_Context_t*)ctx)->unusedObjectSize -= 1;
		knh_stat_incUsedObjectSize(ctx, 1);
		o->h.magic = KNH_OBJECT_MAGIC;
		knh_Object_RCset(o, KNH_RCGC_INIT);
		o->h.flag = ClassTable(bcid).oflag;
		o->h.bcid = bcid;
		o->h.cid  = bcid;
		o->h.ctxid = ctx->ctxid;
		o->h.lock  = LOCK_NOP;
		size_t size = ClassTable(bcid).size;
		if(size > 0) {
			o->ref = KNH_MALLOC(ctx, size);
		}
		else {
			o->ref = NULL;
		}
		ClassTable(bcid).cspi->init(ctx, o, init);
		return o;
	}
}

/* ------------------------------------------------------------------------ */

knh_Object_t *new_Object_init(Ctx *ctx, knh_flag_t flag, knh_class_t cid, int init)
{
	DBG2_ASSERT(cid != CLASS_Context);
	KNH_CHECK_UNUSED_OBJECT(ctx);
	{
		knh_Object_t *o = ctx->unusedObject;
		((knh_Context_t*)ctx)->unusedObject = (knh_Object_t*)o->ref;
		((knh_Context_t*)ctx)->unusedObjectSize -= 1;
		knh_stat_incUsedObjectSize(ctx, 1);
		o->h.magic = KNH_OBJECT_MAGIC;
		knh_Object_RCset(o, KNH_RCGC_INIT);
		DBG2_ASSERT_cid(cid);
		knh_class_t bcid = ClassTable(cid).bcid;
		o->h.bcid = bcid;
		o->h.flag = ClassTable(cid).oflag | flag;
		o->h.cid  = cid;
		o->h.ctxid = ctx->ctxid;
		o->h.lock  = LOCK_NOP;
		size_t size = ClassTable(cid).size;
		if(size > 0) {
			o->ref = KNH_MALLOC(ctx, size);
		}
		else {
			o->ref = NULL;
		}
		ClassTable(bcid).cspi->init(ctx, o, init);
		return o;
	}
}

/* ------------------------------------------------------------------------ */

KNHAPI(Object*) new_Object_boxing(Ctx *ctx, knh_class_t cid, knh_sfp_t *sfp)
{
	KNH_ASSERT(ctx != NULL);
	KNH_CHECK_UNUSED_OBJECT(ctx);
	{
		knh_Object_t *o = ctx->unusedObject;
		((knh_Context_t*)ctx)->unusedObject = (knh_Object_t*)o->ref;
		((knh_Context_t*)ctx)->unusedObjectSize -= 1;
		knh_stat_incUsedObjectSize(ctx, 1);
		o->h.magic = KNH_OBJECT_MAGIC;
		knh_Object_RCset(o, KNH_RCGC_INIT);
		o->h.flag = FLAG_Float;
		o->h.bcid = ClassTable(cid).bcid;
		o->h.cid  = cid;
		o->h.ctxid = ctx->ctxid;
		o->h.lock  = LOCK_NOP;
		knh_Float_t *v = (knh_Float_t*)o;
		v->n.data = sfp[0].data;
		return o;
	}
}

/* ------------------------------------------------------------------------ */

FASTAPI(void) knh_Object_free(Ctx *ctx, knh_Object_t *o)
{
	DBG_ASSERT_FREE();
#ifdef KNH_USING_RCGC
	DBG2_ASSERT(knh_Object_isRC0(o));
#endif
	if(unlikely(o->h.magic == 0)) return;
	if(unlikely(o->h.bcid == CLASS_Context)) return;
	o->h.magic = 0;
	ClassTable(o->h.bcid).cspi->traverse(ctx, o, knh_Object_sweep);
	size_t size = ClassTable(o->h.cid).size;
	if(size > 0) {
		KNH_FREE(ctx, o->ref, size);
	}
	KNH_OBJECT_REUSE(ctx, o);
	knh_stat_dclUsedObjectSize(ctx, 1);
}

/* ------------------------------------------------------------------------ */

void knh_Object_traverse(Ctx *ctx, knh_Object_t *o, knh_Ftraverse ftr)
{
	if(IS_SWEEP(ftr)) {
		knh_Object_free(ctx, o);
	}
	else {
		ClassTable(o->h.bcid).cspi->traverse(ctx, o, knh_Object_sweep);
	}
}

/* ========================================================================= */

#define BSHIFT ((KNH_PAGESIZE / sizeof(knh_Object_t)) / (sizeof(knh_uintptr_t) * 8))

volatile static size_t markedObjectSize = 0;

/* ------------------------------------------------------------------------ */

static
void knh_Object_mark1(Ctx *ctx, Object *o)
{
	if(IS_Context(o)) {
		//DBG2_P("marked %p, cid=%d,%s", o, knh_Object_cid(o), CLASSN(knh_Object_cid(o)));
		TODO();
		knh_Context_traverse(ctx, (knh_Context_t*)o, knh_Object_mark1);
	}
	else {
		DBG2_ASSERT(o->h.magic == KNH_OBJECT_MAGIC);
		DBG2_ASSERT(!knh_isFastMallocMemory((void*)o));
		knh_uintptr_t *b = (knh_uintptr_t*)((((knh_uintptr_t)o) / KNH_PAGESIZE) * KNH_PAGESIZE);
		int n = (o - (Object*)b);
		b = b + BSHIFT;
		DBG2_ASSERT(n < KNH_PAGESIZE / sizeof(knh_Object_t));
		int x = n / (sizeof(knh_uintptr_t)*8);
		knh_uintptr_t mask = ((knh_uintptr_t)1) << (n % (sizeof(knh_uintptr_t)*8));
		if(!((b[x] & mask) == mask)) {
			//DBG2_P("marking %p(%d,%d), cid=%d, %s", o, x, n % (sizeof(knh_uintptr_t)*8), knh_Object_cid(o), CLASSN(knh_Object_cid(o)));
			markedObjectSize++;
			b[x] = b[x] | mask;
			knh_Object_traverse(ctx, o, knh_Object_mark1);
		}
	}
}

/* ------------------------------------------------------------------------ */

void knh_Object_finalSweep(Ctx *ctx, Object *o)
{
	// DO Nothing;
}

/* ------------------------------------------------------------------------ */

void knh_System_gc(Ctx *ctx)
{
	int tindex, size = (int)(ctx->share->ObjectPageTableSize);
	DBG_P("** GC - Starting (used %d Kb) ***", (int)ctx->stat->usedMemorySize/1024);

	for(tindex = 0; tindex < size; tindex++) {
		char *t = ctx->share->ObjectPageTable[tindex].opage;
		if((knh_uintptr_t)t % KNH_PAGESIZE != 0) {
			t = (char*)((((knh_uintptr_t)t / KNH_PAGESIZE) + 1) * KNH_PAGESIZE);
			KNH_ASSERT((knh_uintptr_t)t % KNH_PAGESIZE == 0);
		}
		char *max = ctx->share->ObjectPageTable[tindex].opage + SIZEOF_OBJECTPAGE;
		while(t + KNH_PAGESIZE < max) {
			knh_memcpy(t + ((BSHIFT) * sizeof(knh_uintptr_t)), t, (BSHIFT) * sizeof(knh_uintptr_t));
			//knh_bzero(t + ((BSHIFT) * sizeof(knh_uintptr_t)), (BSHIFT) * sizeof(knh_uintptr_t));
			t += KNH_PAGESIZE;
		}
	}

	DBG_DISABLE_FREE();
	markedObjectSize = 0;
	knh_traverseAll(ctx, knh_Object_mark1);
	DBG_ENABLE_FREE();

	DBG_P("** GC - Marked %d/%d object(s)", (int)markedObjectSize, (int)ctx->stat->usedObjectSize);

	size_t cnt = 0;
	knh_Ftraverse fsweep = ctx->fsweep;
	((knh_Context_t*)ctx)->fsweep = knh_Object_finalSweep;
	for(tindex = 0; tindex < size; tindex++) {
		char *t = ctx->share->ObjectPageTable[tindex].opage;

		if((knh_uintptr_t)t % KNH_PAGESIZE != 0) {
			t = (char*)((((knh_uintptr_t)t / KNH_PAGESIZE) + 1) * KNH_PAGESIZE);
			KNH_ASSERT((knh_uintptr_t)t % KNH_PAGESIZE == 0);
		}
		char *max = ctx->share->ObjectPageTable[tindex].opage + SIZEOF_OBJECTPAGE;
		while(t + KNH_PAGESIZE < max) {
			knh_uintptr_t *b = ((knh_uintptr_t*)t) + BSHIFT;
			int i;
			for(i = 1; i < (KNH_PAGESIZE / sizeof(knh_Object_t)); i++) {
				int x = i / (sizeof(knh_uintptr_t)*8);
				knh_uintptr_t mask = ((knh_uintptr_t)1) << (i % (sizeof(knh_uintptr_t)*8));
				if((b[x] & mask) == mask) continue;
				knh_Object_t *o = ((Object*)t) + i;
				DBG2_ASSERT(!knh_isFastMallocMemory((void*)o));
				if(o->h.magic == 0) continue;   // knh_Object_free
				DBG2_P("%p(%d,%d), magic=%d, cid=%s(%d)", o, i, x, o->h.magic, CLASSN(o->h.bcid), o->h.bcid);
				knh_Object_RCset(o, 0);
				knh_Object_free(ctx, o);
				cnt++;
			}
			t += KNH_PAGESIZE;
		}
	}
	((knh_Context_t*)ctx)->fsweep = fsweep;
	DBG_P("** GC - Collected %d/%d object(s) used=%d Kbytes", (int)cnt, (int)ctx->stat->usedObjectSize, (int)ctx->stat->usedMemorySize/1024);
}

/* ------------------------------------------------------------------------ */
/* [RCGC] */

void knh_Object_RCsweep(Ctx *ctx, Object *o)
{
	knh_Object_RCdec(o);
	if(knh_Object_isRC0(o)) {
		knh_Object_free(ctx, o);
	}
}

/* ------------------------------------------------------------------------ */

knh_Ftraverse knh_getDefaultSweepFunc(void)
{
	return knh_Object_RCsweep;
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
