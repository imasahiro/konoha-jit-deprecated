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
#if defined(HAVE_MMAP)
	munmap(block, size);
#elif defined(K_USING_WINDOWS)
	VirtualFree(block, 0, MEM_RELEASE);
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
		KNH_SYSLOG(ctx, LOG_INFO, "NEW_Arena", "*id=%d region=(%p-%p), %d objects", (int)pageindex, t->head, t->bottom, (int)t->slot_size);
	}
	return ctx->unusedObject;
}

/* ------------------------------------------------------------------------ */
/* [fastmalloc] */

//static int knh_isFastMallocMemory(void *p)
//{
//	knh_uintptr_t *b = (knh_uintptr_t*)((((knh_uintptr_t)p) / K_PAGESIZE) * K_PAGESIZE);
//	int n = ((Object*)p - (Object*)b);
//	int x = n / (sizeof(knh_uintptr_t)*8);
//	knh_uintptr_t mask = ((knh_uintptr_t)1) << (n % (sizeof(knh_uintptr_t)*8));
//	return ((b[x] & mask) == mask);
//}

static void knh_setFastMallocMemory(void *p)
{
	knh_uintptr_t *b = (knh_uintptr_t*)((((knh_uintptr_t)p) / K_PAGESIZE) * K_PAGESIZE);
	int n = ((Object*)p - (Object*)b);
	int x = n / (sizeof(knh_uintptr_t)*8);
	knh_uintptr_t mask = ((knh_uintptr_t)1) << (n % (sizeof(knh_uintptr_t)*8));
	DBG_ASSERT((b[x] & mask) != mask);
	b[x] = b[x] | mask;
}

static void knh_unsetFastMallocMemory(void *p)
{
	knh_uintptr_t *b = (knh_uintptr_t*)((((knh_uintptr_t)p) / K_PAGESIZE) * K_PAGESIZE);
	int n = ((Object*)p - (Object*)b);
	int x = n / (sizeof(knh_uintptr_t)*8);
	knh_uintptr_t mask = ((knh_uintptr_t)1) << (n % (sizeof(knh_uintptr_t)*8));
	DBG_ASSERT((b[x] & mask) == mask);
	b[x] = b[x] & ~(mask);
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
	return UP(o);
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
		t->cspi->traverse(ctx, o, knh_Object_sweep);
		t->cspi->free(ctx, o);
		KNH_OBJECT_REUSE(ctx, o);
		knh_unuseObject(ctx, 1);
#if defined(K_USING_STATCLASS)
		t->count -= 1;
#endif
	}
}

/* ------------------------------------------------------------------------ */

//void knh_Object_traverse(Ctx *ctx, knh_Object_t *o, knh_Ftraverse ftr)
//{
//	if(IS_SWEEP(ftr)) {
//		knh_Object_free(ctx, o);
//	}
//	else {
//		ClassTable(o->h.bcid).cspi->traverse(ctx, o, knh_Object_sweep);
//	}
//}

/* ========================================================================= */

#define BSHIFT ((K_PAGESIZE / sizeof(knh_Object_t)) / (sizeof(knh_uintptr_t) * 8))

volatile static size_t markedObjectSize = 0;

/* ------------------------------------------------------------------------ */

//static void knh_Object_mark1(Ctx *ctx, Object *o)
//{
//	if(IS_Context(o)) {
//		//DBG_P("marked %p, cid=%d,%s", o, knh_Object_cid(o), CLASSN(knh_Object_cid(o)));
//		TODO();
//		knh_Context_traverse(ctx, (knh_Context_t*)o, knh_Object_mark1);
//	}
//	else {
//		DBG_ASSERT(o->h.magic == K_OBJECT_MAGIC);
//		DBG_ASSERT(!knh_isFastMallocMemory((void*)o));
//		knh_uintptr_t *b = (knh_uintptr_t*)((((knh_uintptr_t)o) / K_PAGESIZE) * K_PAGESIZE);
//		int n = (o - (Object*)b);
//		b = b + BSHIFT;
//		DBG_ASSERT(n < K_PAGESIZE / sizeof(knh_Object_t));
//		int x = n / (sizeof(knh_uintptr_t)*8);
//		knh_uintptr_t mask = ((knh_uintptr_t)1) << (n % (sizeof(knh_uintptr_t)*8));
//		if(!((b[x] & mask) == mask)) {
//			//DBG_P("marking %p(%d,%d), cid=%d, %s", o, x, n % (sizeof(knh_uintptr_t)*8), knh_Object_cid(o), CLASSN(knh_Object_cid(o)));
//			markedObjectSize++;
//			b[x] = b[x] | mask;
//			knh_Object_traverse(ctx, o, knh_Object_mark1);
//		}
//	}
//}


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
//				"ClassCounter", "*name=%s cid=%d count=%ld total=%ld", S_tochar(t->sname), i, t->count, t->total);
//		}
//	}
//#endif
	KNH_SYSLOG(ctx, LOG_DEBUG, "MemoryUsage", "*memory usage=%ld/%ld, object usage=%ld/%ld",
		stat->usedMemorySize, stat->maxMemoryUsage, stat->usedObjectSize, stat->maxObjectUsage);
	KNH_SYSLOG(ctx, LOG_DEBUG, "MemoryUsage", "*object generation=%ld", stat->countObjectGeneration);
	KNH_SYSLOG(ctx, LOG_DEBUG, "MemoryUsage", "*memory counter x1=%ld, x2=%ld, x4=%ld, x8=%ld, large=%ld",
		stat->countMemorySize1, stat->countMemorySize2, stat->countMemorySize4,
		stat->countMemorySize8, stat->countMemorySizeN);
}

/* ------------------------------------------------------------------------ */

void knh_System_gc(Ctx *ctx)
{
	knh_showMemoryStat(ctx);
//	int tindex, size = (int)(ctx->share->ArenaSetSize);
//	DBG_P("** GC - Starting (used %d Kb) ***", (int)ctx->stat->usedMemorySize/1024);
//
//	for(tindex = 0; tindex < size; tindex++) {
//		char *t = ctx->share->ArenaSet[tindex].opage;
//		if((knh_uintptr_t)t % K_PAGESIZE != 0) {
//			t = (char*)((((knh_uintptr_t)t / K_PAGESIZE) + 1) * K_PAGESIZE);
//			KNH_ASSERT((knh_uintptr_t)t % K_PAGESIZE == 0);
//		}
//		char *max = ctx->share->ArenaSet[tindex].opage + K_ARENASIZE;
//		while(t + K_PAGESIZE < max) {
//			knh_memcpy(t + ((BSHIFT) * sizeof(knh_uintptr_t)), t, (BSHIFT) * sizeof(knh_uintptr_t));
//			//knh_bzero(t + ((BSHIFT) * sizeof(knh_uintptr_t)), (BSHIFT) * sizeof(knh_uintptr_t));
//			t += K_PAGESIZE;
//		}
//	}
//
//	DBG_DISABLE_FREE();
//	markedObjectSize = 0;
//	knh_traverseAll(ctx, knh_Object_mark1);
//	DBG_ENABLE_FREE();
//
//	DBG_P("** GC - Marked %d/%d object(s)", (int)markedObjectSize, (int)ctx->stat->usedObjectSize);
//
//	size_t cnt = 0;
//	knh_Ftraverse fsweep = ctx->fsweep;
//	((knh_Context_t*)ctx)->fsweep = knh_Object_finalSweep;
//	for(tindex = 0; tindex < size; tindex++) {
//		char *t = ctx->share->ArenaSet[tindex].opage;
//
//		if((knh_uintptr_t)t % K_PAGESIZE != 0) {
//			t = (char*)((((knh_uintptr_t)t / K_PAGESIZE) + 1) * K_PAGESIZE);
//			KNH_ASSERT((knh_uintptr_t)t % K_PAGESIZE == 0);
//		}
//		char *max = ctx->share->ArenaSet[tindex].opage + K_ARENASIZE;
//		while(t + K_PAGESIZE < max) {
//			knh_uintptr_t *b = ((knh_uintptr_t*)t) + BSHIFT;
//			int i;
//			for(i = 1; i < (K_PAGESIZE / sizeof(knh_Object_t)); i++) {
//				int x = i / (sizeof(knh_uintptr_t)*8);
//				knh_uintptr_t mask = ((knh_uintptr_t)1) << (i % (sizeof(knh_uintptr_t)*8));
//				if((b[x] & mask) == mask) continue;
//				knh_Object_t *o = ((Object*)t) + i;
//				DBG_ASSERT(!knh_isFastMallocMemory((void*)o));
//				if(o->h.magic == 0) continue;   // knh_Object_free
//				DBG_P("%p(%d,%d), magic=%d, cid=%s(%d)", o, i, x, o->h.magic, CLASSN(o->h.bcid), o->h.bcid);
//				knh_Object_RCset(o, 0);
//				knh_Object_free(ctx, o);
//				cnt++;
//			}
//			t += K_PAGESIZE;
//		}
//	}
//	((knh_Context_t*)ctx)->fsweep = fsweep;
//	DBG_P("** GC - Collected %d/%d object(s) used=%d Kbytes", (int)cnt, (int)ctx->stat->usedObjectSize, (int)ctx->stat->usedMemorySize/1024);
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
