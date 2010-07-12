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

#define KNH_TOBJECT_C  1

#include"commons.h"
#ifdef HAVE_MMAP
#include <sys/mman.h>
#endif

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef K_USING_DEBUG
static int canFree = 1;
#define DBG_ENABLE_FREE()     canFree = 1
#define DBG_DISABLE_FREE()    canFree = 0
#define DBG_ASSERT_FREE()     KNH_ASSERT(canFree)
#else
#define DBG_ENABLE_FREE()
#define DBG_DISABLE_FREE()
#define DBG_ASSERT_FREE()
#endif

//#define GC_DEBUG 

/* ======================================================================== */
/* [malloc] */

static void SYSLOG_OutOfMemory(Ctx *ctx, size_t size)
{
	KNH_SYSLOG(ctx, LOG_CRIT, "OutOfMemory",
		"*requested=%dbytes, used=%dbytes", size, ctx->stat->usedMemorySize);
}

static void SYSLOG_FreeLargeMemory(Ctx *ctx, void *ptr, size_t size)
{
	KNH_SYSLOG(ctx, LOG_DEBUG, "FREE_LargeMemory", "*ptr=%p, size=%ldkb", ptr, (size/1024));
}

/* ------------------------------------------------------------------------ */

void *knh_malloc(Ctx *ctx, size_t size)
{
	DBG_ASSERT(size > 0);
	void *block = malloc(size);
	if (unlikely(block == NULL)) {
		SYSLOG_OutOfMemory(ctx, size);
	}
	knh_useMemory(ctx, size);
	return block;
}

void knh_free(Ctx *ctx, void *block, size_t size)
{
	SECURE_bzero(block, size);
	if(unlikely(size > K_PAGESIZE)) {
		SYSLOG_FreeLargeMemory(ctx, block, size);
	}
	free(block);
	knh_unuseMemory(ctx, size);
}

void *knh_valloc(Ctx *ctx, size_t size)
{
	DBG_ASSERT(size > 0);
	void *block = NULL;
#if defined(HAVE_POSIX_MEMALIGN)
	posix_memalign(&block, K_PAGESIZE, size);
//#elif defined(HAVE_MMAP)
//	block = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
#elif defined(HAVE_MEMALIGN)
	block = memalign(K_PAGESIZE,size);
#elif defined(K_USING_WINDOWS)
	block = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
#else
	block = malloc(size);
#endif
	if (unlikely(block == NULL)) {
		SYSLOG_OutOfMemory(ctx, size);
	}
	knh_useMemory(ctx, size);
	return block;
}

void knh_vfree(Ctx *ctx, void *block, size_t size)
{
	SECURE_bzero(block, size);
	if(unlikely(size > K_PAGESIZE)) {
		SYSLOG_FreeLargeMemory(ctx, block, size);
	}
#if defined(K_USING_WINDOWS)
	VirtualFree(block, 0, MEM_RELEASE);
//#elif defined(HAVE_MMAP)
//	munmap(block, size);
#else
	free(block);
#endif
	knh_unuseMemory(ctx, size);
}

#if defined(K_USING_TRACEMALLOC)

void *TRACE_malloc(Ctx *ctx, size_t size TRACEARG)
{
	DBG_ASSERT(size != 0);
	size_t *block = (size_t*)malloc(size + sizeof(size_t));
	if (unlikely(block == NULL)) {
		SYSLOG_OutOfMemory(ctx, size);
	}
	knh_useMemory(ctx, size);
	block[0] = size;
	return (void*)(block + 1);
}

void TRACE_free(Ctx *ctx, void *p, size_t size TRACEARG)
{
	DBG_ASSERT_FREE();
	DBG_ASSERT(size != 0);
	size_t *block = ((size_t*)p) - 1;
	if(unlikely(size != block[0])) {
		fprintf(stderr, "%s:%d(%s) ptr = %p, block.size = %d, free.size=%d\n",
				_file, _line, _func, p, (int)block[0], (int)size);
		DBG_ABORT();
	}
	block[0]=0;
	SECURE_bzero(block, size + sizeof(size_t));
	free(block);
	knh_unuseMemory(ctx, size);
}
#endif/*TRACE_MALLOC*/

/* ======================================================================== */
/* [tObject] */

#define KNH_CHECK_UNUSED_OBJECT(ctx) { \
		/*DBG_P("size = %ld %p", ctx->unusedObjectSize, ctx->unusedObject);*/\
		if(unlikely(ctx->unusedObject == NULL)) { \
			DBG_ASSERT(ctx->unusedObjectSize == 0); \
			((knh_Context_t*)ctx)->unusedObject = new_UnusedObject(ctx); \
		} \
		DBG_checkOnArena(ctx, ctx->unusedObject TRACEDATA);\
	} \


#define KNH_OBJECT_REUSE(ctx, used) { \
		knh_intptr_t *p = (knh_intptr_t*)used;\
		p[0] = K_INT0; p[1] = K_INT0; p[2] = K_INT0; p[3] = K_INT0;\
		p[4] = K_INT0; p[5] = K_INT0; p[6] = K_INT0; p[7] = K_INT0;\
		DBG_checkOnArena(ctx, used TRACEDATA);\
		used->ref = ctx->unusedObject;\
		((knh_Context_t*)ctx)->unusedObject = used;\
		((knh_Context_t*)ctx)->unusedObjectSize += 1;\
	}\

/* ------------------------------------------------------------------------ */

KNHAPI(knh_bool_t) knh_isObject(Ctx *ctx, void *p)
{
	knh_uintptr_t n = (knh_uintptr_t)p;
	if(p != NULL && n % sizeof(knh_Object_t) == 0) {
		size_t i, size = ctx->share->ArenaSetSize;
		knh_ArenaSet_t *t = ctx->share->ArenaSet;
		for(i = 0; i < size; i++) {
			if(t->head <= p && p < t->bottom) return 1;
			t++;
		}
	}
	return 0;
}

static void DBG_checkOnArena(Ctx *ctx, void *used TRACEARG)
{
	size_t i, size = ctx->share->ArenaSetSize;
	knh_ArenaSet_t *t = ctx->share->ArenaSet;
	for(i = 0; i < size; i++) {
		if(t->head <= used && used < t->bottom) return;
		t++;
	}
	TRACE_P("not paged object %p", used);
}

static void knh_extendArenaSet(Ctx *ctx, knh_share_t *ctxshare)
{
	size_t newsize = ctxshare->ArenaSetMax * 2;
	knh_ArenaSet_t *newpage = (knh_ArenaSet_t*)KNH_MALLOC(ctx, sizeof(knh_ArenaSet_t) * newsize);
	knh_bzero(newpage, sizeof(knh_ArenaSet_t) * newsize);
	knh_memcpy(newpage, ctxshare->ArenaSet, sizeof(knh_ArenaSet_t) * ctxshare->ArenaSetMax);
	KNH_FREE(ctx, ctxshare->ArenaSet, sizeof(knh_ArenaSet_t) * ctxshare->ArenaSetMax);
	ctxshare->ArenaSet = newpage;
	ctxshare->ArenaSetMax = newsize;
	KNH_SYSLOG(ctx, LOG_INFO, "EXTENDED_ArenaSet", "*newsize=%d", newsize);
}

static knh_Object_t *new_UnusedObject(Ctx *ctx)
{
	KNH_LOCK(ctx, LOCK_MEMORY, NULL);
	knh_share_t *ctxshare = (knh_share_t*)ctx->share;
	size_t pageindex = ctxshare->ArenaSetSize;
	if(unlikely(!(pageindex < ctxshare->ArenaSetMax))) {
		knh_extendArenaSet(ctx, ctxshare);
	}
	ctxshare->ArenaSetSize += 1;
	KNH_UNLOCK(ctx, LOCK_MEMORY, NULL);
	{
		long cnt = 0;
		int c = 0;
		knh_ArenaSet_t *t = ctxshare->ArenaSet + pageindex;
		knh_OArena_t *oslot = (knh_OArena_t*)KNH_VALLOC(ctx, K_ARENASIZE);
		knh_bzero(oslot, K_ARENASIZE);
		t->ctx = ctx;
		t->head = (void*)oslot;
		t->bottom = (void*)KNH_ARENA_MAX(oslot, K_ARENASIZE);
		t->arena_size = K_ARENASIZE;
		t->isObjectArena = 1;
		if((knh_uintptr_t)oslot % K_PAGESIZE != 0) {
			char *t2 = (char*)((((knh_uintptr_t)oslot / K_PAGESIZE) + 1) * K_PAGESIZE);
			oslot = (knh_OArena_t*)t2;
			DBG_ASSERT((knh_uintptr_t)oslot % K_PAGESIZE == 0);
			KNH_SYSLOG(ctx, LOG_DEBUG, "SHIFT_Arena", "*shift %p=>%p, UNUSED MEMORY=%d", t->head, oslot, (int)(t2 - (char*)t->head));
		}
		t->oslot = oslot;
		DBG_ASSERT(ctx->unusedObject == NULL);
		((knh_Context_t*)ctx)->unusedObject = oslot->slots;
		while(oslot + 1 <= (knh_OArena_t*)t->bottom) {
			size_t i;
			for(i = 0; i < K_OARENASIZE - 1; i++) {
				DBG_ASSERT(knh_Object_getArena(oslot->slots + i) == &oslot->head);
				oslot->slots[i].ref = &(oslot->slots[i+1]);
				c++;
			}
			if(oslot + 2 <= (knh_OArena_t*)t->bottom) {
				oslot->slots[K_OARENASIZE - 1].ref = oslot[1].slots;
				c++;
			}
			else {
				oslot->slots[K_OARENASIZE - 1].ref = NULL;
				c++;
			}
			oslot += 1;
			cnt++;
		}
		t->slot_size = (cnt * K_OARENASIZE);
		//DBG_P("K_OARENASIZE=%d, cnt=%d, sizeof(knh_OArena_t)=%d", K_OARENASIZE, cnt, sizeof(knh_OArena_t));
		DBG_ASSERT(c == t->slot_size);
		DBG_ASSERT(ctx->unusedObjectSize == 0);
		((knh_Context_t*)ctx)->unusedObjectSize = t->slot_size;
		KNH_SYSLOG(ctx, LOG_INFO, "NEW_Arena", "*id=%zd region=(%p-%p), %zd objects", pageindex, t->head, t->bottom, t->slot_size);
	}
	return ctx->unusedObject;
}

/* ------------------------------------------------------------------------ */
/* [bitop] */

/* added by ide */
#define UINTPTR8 (sizeof(knh_uintptr_t) * 8)
#define INDEX2OFFSET(index_) (index_ / UINTPTR8)
#define INDEX2MASK(n) (((knh_uintptr_t)1) << (n % UINTPTR8))

static int bit_test(knh_uintptr_t *b, int offset)
{
	int x = INDEX2OFFSET(offset);
	knh_uintptr_t mask = INDEX2MASK(offset);
	return ((b[x] & mask) == mask);
}
static void bit_set(knh_uintptr_t *b, int offset)
{
	int x = INDEX2OFFSET(offset);
	knh_uintptr_t mask = INDEX2MASK(offset);
	DBG_ASSERT((b[x] & mask) != mask);
	b[x] = b[x] | mask;
}
static void bit_unset(knh_uintptr_t *b, int offset)
{
	int x = INDEX2OFFSET(offset);
	knh_uintptr_t mask = INDEX2MASK(offset);
	DBG_ASSERT((b[x] & mask) == mask);
	b[x] = b[x] & ~(mask);
}

/* ------------------------------------------------------------------------ */
/* [fastmalloc] */

/* fixed by ide */

#ifdef K_USING_DEBUG
static int knh_isFastMallocMemory(void *p)
{
	knh_uintptr_t *b = (knh_uintptr_t*) knh_Object_getArena(p);
	int n = ((Object*)p - (Object*)b);
	return bit_test(b, n);
}
#endif

static void knh_setFastMallocMemory(void *p)
{
	knh_uintptr_t *b = (knh_uintptr_t*) knh_Object_getArena(p);
	int n = ((Object*)p - (Object*)b);
	bit_set(b, n);
}

static void knh_unsetFastMallocMemory(void *p)
{
	knh_uintptr_t *b = (knh_uintptr_t*) knh_Object_getArena(p);
	int n = ((Object*)p - (Object*)b);
	bit_unset(b, n);
}

/* ------------------------------------------------------------------------ */

void *knh_fastmalloc(Ctx *ctx, size_t size)
{
	DBG_ASSERT(size != 0);
	if(size <= K_FASTMALLOC_SIZE) {
		KNH_CHECK_UNUSED_OBJECT(ctx);
		Object *o = ctx->unusedObject;
		((knh_Context_t*)ctx)->unusedObject = (knh_Object_t*)o->ref;
		((knh_Context_t*)ctx)->unusedObjectSize -= 1;
		knh_setFastMallocMemory((void*)o);
		knh_countMemorySize(ctx, 1);
		return (void*)o;
	}
	else {
		void *block = malloc(size);
		if (unlikely(block == NULL)) {
			SYSLOG_OutOfMemory(ctx, size);
		}
		knh_useMemory(ctx, size);
		if(size <= K_FASTMALLOC_SIZE * 4) {
			if(size <= K_FASTMALLOC_SIZE * 2) {
				knh_countMemorySize(ctx, 2);
			}
			else {
				knh_countMemorySize(ctx, 4);
			}
		}
		else {
			if(size <= K_FASTMALLOC_SIZE * 8) {
				knh_countMemorySize(ctx, 8);
			}
			else {
				knh_countMemorySize(ctx, N);
			}
		}
		return block;
	}
}

/* ------------------------------------------------------------------------ */

void knh_fastfree(Ctx *ctx, void *block, size_t size)
{
	if(size <= K_FASTMALLOC_SIZE) {
		knh_Object_t *o = (knh_Object_t*)block;
		DBG_checkOnArena(ctx, block TRACEDATA);
		o->ref = ctx->unusedObject;
		((knh_Context_t*)ctx)->unusedObject = o;
		((knh_Context_t*)ctx)->unusedObjectSize += 1;
		o->h.magic = 0;
		knh_unsetFastMallocMemory(block);
	}
	else {
		knh_free(ctx, block, size);
	}
}

/* ------------------------------------------------------------------------ */
/* [hObject] */

knh_Object_t *new_hObject_(Ctx *ctx, knh_flag_t flag, knh_class_t bcid, knh_class_t cid)
{
	knh_Object_t *o = NULL;
	DBG_ASSERT(bcid != CLASS_Context);
	KNH_CHECK_UNUSED_OBJECT(ctx);
	knh_countObjectGeneration(ctx);
	o = ctx->unusedObject;
	((knh_Context_t*)ctx)->unusedObject = (knh_Object_t*)o->ref;
	((knh_Context_t*)ctx)->unusedObjectSize -= 1;
	knh_useObject(ctx, 1);
	o->h.magic = K_OBJECT_MAGIC;
	knh_Object_RCset(o, K_RCGC_INIT);
	o->h.flag = flag;
	o->h.bcid = bcid;
	o->h.cid  = cid;
#if defined(K_USING_STATCLASS)
	{
		knh_ClassTable_t *t = pClassTable(ctx, cid);
		t->count += 1; t->total += 1;
	}
#endif
	return o;
}

/* ------------------------------------------------------------------------ */

knh_Object_t *new_Object_init2(Ctx *ctx, knh_flag_t flag, knh_class_t bcid, knh_class_t cid)
{
	knh_Object_t *o = NULL;
	knh_ClassTable_t *t = NULL;
	DBG_ASSERT(cid != CLASS_Context);
	KNH_CHECK_UNUSED_OBJECT(ctx);
	o = ctx->unusedObject;
	((knh_Context_t*)ctx)->unusedObject = (knh_Object_t*)o->ref;
	((knh_Context_t*)ctx)->unusedObjectSize -= 1;
	knh_useObject(ctx, 1);
	o->h.magic = K_OBJECT_MAGIC;
	knh_Object_RCset(o, K_RCGC_INIT);
	t = pClassTable(ctx, cid);
	o->h.bcid = bcid;
	o->h.flag = t->oflag | flag;
	o->h.cid  = cid;
	t->cspi->init(ctx, o);
	knh_countObjectGeneration(ctx);
#if defined(K_USING_STATCLASS)
	t->count += 1;
	t->total += 1;
#endif
	return o;
}

/* ------------------------------------------------------------------------ */

KNHAPI(Object*) new_Object_boxing(Ctx *ctx, knh_class_t cid, knh_sfp_t *sfp)
{
	knh_Float_t *o = NULL;
	knh_ClassTable_t *t = pClassTable(ctx, cid);
	KNH_CHECK_UNUSED_OBJECT(ctx);
	o = (knh_Float_t*)ctx->unusedObject;
	((knh_Context_t*)ctx)->unusedObject = (knh_Object_t*)((knh_Object_t*)o)->ref;
	((knh_Context_t*)ctx)->unusedObjectSize -= 1;
	knh_useObject(ctx, 1);
	o->h.magic = K_OBJECT_MAGIC;
	knh_Object_RCset(o, K_RCGC_INIT);
	o->h.flag = t->oflag;
	o->h.bcid = t->bcid;
	o->h.cid  = cid;
	o->n.data = sfp[0].data;
	knh_countObjectGeneration(ctx);
#if defined(K_USING_STATCLASS)
	t->count += 1;
	t->total += 1;
#endif
	return UPCAST(o);
}

/* ------------------------------------------------------------------------ */

FASTAPI(void) knh_Object_free(Ctx *ctx, knh_Object_t *o)
{
#ifdef K_USING_RCGC
	DBG_ASSERT(knh_Object_isRC0(o));
#endif
	if(unlikely(o->h.magic == 0)) return;
	if(unlikely(o->h.bcid == CLASS_Context)) return;
	o->h.magic = 0;
	{
		knh_ClassTable_t *t = pClassTable(ctx, o->h.cid);
#ifdef K_USING_RCGC
		/* fixed by ide */
		t->cspi->traverse(ctx, o, knh_Object_sweep);
#endif
		t->cspi->free(ctx, o);
		KNH_OBJECT_REUSE(ctx, o);
		knh_unuseObject(ctx, 1);
#if defined(K_USING_STATCLASS)
		t->count -= 1;
#endif
	}
}

/* ------------------------------------------------------------------------ */
/* added by ide */
static void knh_Object_traverse(Ctx *ctx, knh_Object_t *o, knh_Ftraverse ftr)
{
	ClassTable(o->h.bcid).cspi->traverse(ctx, o, ftr);
}

/* ========================================================================= */
#define BSHIFT ((K_PAGESIZE / sizeof(knh_Object_t)) / (sizeof(knh_uintptr_t) * 8))

static volatile size_t markedObjectSize = 0;

/* added by ide */
static void GC_DBG(Ctx *ctx, knh_Object_t *o, const char *msg) {
#if defined(GC_DEBUG)
	knh_class_t _cid = knh_Object_cid(o);
	knh_uintptr_t *b = (knh_uintptr_t*)knh_Object_getArena(o);
		int n = (o - (Object*)b);
		int x = INDEX2OFFSET(n);
	DBG_P("%s %p(%d,%d), cid=%s(%d)", msg, o, x, n % (sizeof(knh_uintptr_t)*8), CLASSN(_cid), _cid);
#else
	(void)ctx;(void)o;(void)msg;
#endif
}

/* ------------------------------------------------------------------------ */

/* added by ide */
static void knh_Object_mark1(Ctx *ctx, Object *o)
{
	if(IS_Context(o)) {
		TODO();
		knh_Context_traverse(ctx, (knh_Context_t*)o, knh_Object_mark1);
	}
	else {
		DBG_ASSERT(o->h.magic == K_OBJECT_MAGIC);
		DBG_ASSERT(!knh_isFastMallocMemory((void*)o));
		knh_uintptr_t *b = (knh_uintptr_t*)knh_Object_getArena(o);
		int n = (o - (Object*)b);
		b = b + BSHIFT;
		DBG_ASSERT(n < K_PAGESIZE / sizeof(knh_Object_t));
		if(!(bit_test(b, n))) {
			bit_set(b, n);
			GC_DBG(ctx, o,"marking");
			markedObjectSize++;
			knh_Object_traverse(ctx, o, knh_Object_mark1);
		}
	}
}

#if defined(K_USING_DEBUG) && defined(GC_DEBUG)
/* added by ide */
static int knh_Object_isMarked(Ctx *ctx, Object *o)
{
	if(!IS_Context(o)) {
		knh_uintptr_t *b = (knh_uintptr_t*)knh_Object_getArena(o);
		int n = (o - (Object*)b);
		b = b + BSHIFT;
		if (bit_test(b, n)) {
			return 1;
		}
	}
	return 0;
}
#endif

/* ------------------------------------------------------------------------ */

void knh_showMemoryStat(Ctx *ctx)
{
	knh_stat_t *stat = ctx->stat;
//#if defined(K_USING_STATCLASS)
//	size_t i;
//	for(i = 0; i < ctx->share->ClassTableSize; i++) {
//		knh_ClassTable_t *t = pClassTable(ctx, i);
//		if(t->total > 2) {
//			KNH_SYSLOG(ctx, LOG_INFO,
//				"ClassCounter", "*name=%s cid=%d count=%ld total=%ld",
//				S_tochar(t->sname), i, t->count, t->total);
//		}
//	}
//#endif
	KNH_SYSLOG(ctx, LOG_DEBUG, "MemoryUsage", "*memory usage=%ld/%ld, object usage=%ld/%ld",
		stat->usedMemorySize, stat->maxMemoryUsage, stat->usedObjectSize, stat->maxObjectUsage);
	KNH_SYSLOG(ctx, LOG_DEBUG, "MemoryUsage", "*object generation=%ld", stat->countObjectGeneration);
	KNH_SYSLOG(ctx, LOG_DEBUG, "MemoryUsage",
			"*memory counter x1=%ld, x2=%ld, x4=%ld, x8=%ld, large=%ld",
			stat->countMemorySize1, stat->countMemorySize2, stat->countMemorySize4,
			stat->countMemorySize8, stat->countMemorySizeN);
}

/* ------------------------------------------------------------------------ */

/* added by ide */
#define Object_isFreed(o) (((knh_Object_t*)(o))->h.magic == 0)
#define CHECK_ALIGN(ptr, size) \
	if((knh_uintptr_t)(ptr) % (size) != 0) {\
		TODO();\
		/*	ptr = (char*)((((knh_uintptr_t)ptr / size) + 1) * size);*/\
		KNH_ASSERT((knh_uintptr_t)(ptr) % (size) == 0);\
	}\

static int Object_free_(Ctx *ctx, knh_Object_t *o)
{
	DBG_ASSERT(!knh_isFastMallocMemory((void*)o));
	if (Object_isFreed(o)) return 0;
	GC_DBG(ctx, o, "freeing");
	knh_Object_RCset(o, 0);
	knh_Object_free(ctx, o);
	return 1;
}

/* added by ide */
#define BITMAP_SIZE ((BSHIFT) * sizeof(knh_uintptr_t))

/* ------------------------------------------------------------------------ */
void knh_System_gc(Ctx *ctx)
{
	/* added by ide */
	knh_showMemoryStat(ctx);
	knh_share_t *share = (knh_share_t *) ctx->share;
	int tindex, size = share->ArenaSetSize;
	DBG_P("** GC - Starting (used %zd Kb) ***", ctx->stat->usedMemorySize/1024);

	KNH_SYSLOG(ctx, LOG_DEBUG, "GC", "GC_ArenaSize=%d", size);

	for(tindex = 0; tindex < size; tindex++) {
		char *t   = (char*) share->ArenaSet[tindex].oslot;
		char *max = (char*) share->ArenaSet[tindex].oslot + K_ARENASIZE;
		CHECK_ALIGN(t, K_PAGESIZE);
		while(t + K_PAGESIZE <= max) {
			knh_memcpy(t + BITMAP_SIZE, t, BITMAP_SIZE);
			t += K_PAGESIZE;
		}
	}
	DBG_DISABLE_FREE();
	markedObjectSize = 0;
	knh_traverseAll(ctx, knh_Object_mark1);
	DBG_ENABLE_FREE();

	DBG_P("** GC - Marked %zd/%zd object(s)", markedObjectSize, ctx->stat->usedObjectSize);
	size_t cnt = 0;
	knh_Ftraverse fsweep = ctx->fsweep;
	((knh_Context_t*)ctx)->fsweep = knh_Object_finalSweep;
	for(tindex = 0; tindex < size; tindex++) {
		char *t   = (char*) share->ArenaSet[tindex].oslot;
		char *max = (char*) share->ArenaSet[tindex].oslot + K_ARENASIZE;
		CHECK_ALIGN(t, K_PAGESIZE);
		while(t + K_PAGESIZE <= max) {
			knh_uintptr_t *b = ((knh_uintptr_t*)t) + BSHIFT;
			size_t i;
			for(i = 1; i < (K_PAGESIZE / sizeof(knh_Object_t)); i++) {
				if (!bit_test(b, i)) {
					knh_Object_t *o = ((Object*)t) + i;
					cnt += Object_free_(ctx, o);
				}
			}
			t += K_PAGESIZE;
		}
	}
	((knh_Context_t*)ctx)->fsweep = fsweep;
	DBG_P("** GC - Collected %zd/%zd object(s) used=%zd Kbytes", cnt, ctx->stat->usedObjectSize, ctx->stat->usedMemorySize/1024);
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
