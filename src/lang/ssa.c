/****************************************************************************
 * KONOHA COPYRIGHT, LICENSE NOTICE, knh_Array_nD DISCRIMER  
 * 
 *           (c) 2006-2010, Kimio Kuramitsu <kimio at ynu.ac.jp>
 *           (c) 2008-      Konoha Team konohaken@googlegroups.com  
 * All rights reserved.
 * 
 * You may choose one of the following two licenses when you use konoha. 
 * See www.konohaware.org/license.html for further information.
 * 
 * (1) GNU General Public License 2.0      (with    KONOHA_UNDER_GPL2)
 * (2) Konoha Software Foundation License 1.0
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS knh_Array_nD CONTRIBUTORS
 * "AS IS" knh_Array_nD knh_Array_nY EXPRESS OR IMPLIED WARRknh_Array_nTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRknh_Array_nTIES OF MERCHknh_Array_nTABILITY knh_Array_nD FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER 
 * OR CONTRIBUTORS BE LIABLE FOR knh_Array_nY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED knh_Array_nD ON knh_Array_nY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN knh_Array_nY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *  
 ****************************************************************************/

#ifdef K_USING_SSA

#include"commons.h"

#define VERTEX(n)              ((knh_BasicBlock_t*)knh_Array_n(vertex, n))
#define PARENT(o)              knh_BasicBlock_getParent(vertex, o)
#define ANCESTOR(o)            (knh_Array_n(ancestor, DP(o)->id))
#define SEMI(o)                (knh_Array_n(semi, DP(o)->id))
#define DFNUM(o)               (DP(o)->id)
#define BEST(o)                (knh_Array_n(best, DP(o)->id))
#define IDOM(o)                (knh_Array_n(idom, DP(o)->id))
#define SAMEDOM(o)             (knh_Array_n(samedom, DP(o)->id))
#define BUCKET(o)              ((knh_Array_t*)knh_Array_n(bucket, DP(o)->id))
#define PRED(o)                (knh_Array_n(pred, DP(o)->id))
#define LINK(o1, o2)           KNH_SETv(ctx, ANCESTOR(o2), o1); KNH_SETv(ctx, BEST(o2), o2)
#define DF(o)                  (knh_Array_n(df, DP(o)->id))
#define DEFSITES(n)            ((knh_Array_t*)knh_Array_n(defsites, n))
#define APHI(n)                ((knh_Array_t*)knh_Array_n(Aphi, n))

#if defined(K_USING_THREADEDCODE)
#define TADDR   NULL, 0/*counter*/
#else
#define TADDR   0/*counter*/
#endif/*K_USING_THREADEDCODE*/
#define ASMLINE  0

#ifdef __cplusplus 
extern "C" {
#endif

/* ------------------------------------------------------------------------ */
/* [debug] */

#ifdef K_USING_DEBUG
static void knh_BasicBlock_print(knh_BasicBlock_t *bb)
{
	int i;
	fprintf(stderr, "+------BB[%02d]------+\n", DP(bb)->id);
	//fprintf(stderr, "|    incoming=%02d   |\n", DP(bb)->incoming);
	//fprintf(stderr, "| size=%02d, capa=%02d |\n", DP(bb)->size, DP(bb)->capacity);
	for (i = 0; i < DP(bb)->size; i++) {
		knh_opline_t *op = DP(bb)->opbuf + i;
		fprintf(stderr, "|   opcode=%-6s  |\n", knh_opcode_tochar(op->opcode));
	}
	if (bb->nextNC != NULL) {
		fprintf(stderr, "| next=%02d, jump=", DP(bb->nextNC)->id);
	} else {
		fprintf(stderr, "| next=NN, jump=");
	}
	if (bb->jumpNC != NULL) {
		fprintf(stderr, "%02d |\n", DP(bb->jumpNC)->id);
	} else {
		fprintf(stderr, "NN |\n");
	}
}

static void knh_BasicBlock_printIDOM(Ctx *ctx, knh_Array_t *vertex, knh_Array_t *samedom, knh_Array_t *ancestor, knh_Array_t *semi, knh_Array_t *idom)
{
	size_t i;
	for (i = 0; i < knh_Array_size(vertex); i++) {
		fprintf(stderr, "[%02lu] samedom=", i);
		if (knh_Array_n(samedom, i) != KNH_NULL) {
			fprintf(stderr, "%02d", DFNUM((knh_BasicBlock_t*)knh_Array_n(samedom, i)));
		} else {
			fprintf(stderr, "NN");
		}
		fprintf(stderr, ", ancestor=");
		if (knh_Array_n(ancestor, i) != KNH_NULL) {
			fprintf(stderr, "%02d", DFNUM((knh_BasicBlock_t*)knh_Array_n(ancestor, i)));
		} else {
			fprintf(stderr, "NN");
		}
		fprintf(stderr, ", semi=");
		if (knh_Array_n(semi, i) != KNH_NULL) {
			fprintf(stderr, "%02d", DFNUM((knh_BasicBlock_t*)knh_Array_n(semi, i)));
		} else {
			fprintf(stderr, "NN");
		}
		fprintf(stderr, ", idom=");
		if (knh_Array_n(idom, i) != KNH_NULL) {
			fprintf(stderr, "%02d\n", DFNUM((knh_BasicBlock_t*)knh_Array_n(idom, i)));
		} else {
			fprintf(stderr, "NN\n");
		}
	}
}

static void knh_BasicBlock_printDF(Ctx *ctx, knh_Array_t *vertex, knh_Array_t* df)
{
	size_t i, j;
	knh_BasicBlock_t *n;
	for (i = 0; i < knh_Array_size(vertex); i++) {
		fprintf(stderr, "[%02lu] DF={", i);
		for (j = 0; j < knh_Array_size((knh_Array_t*)knh_Array_n(df, i)); j++) {
			n = (knh_BasicBlock_t*)knh_Array_n((knh_Array_t*)knh_Array_n(df, i), j);
			fprintf(stderr, "%02d, ", DFNUM(n));
		}
		fprintf(stderr, "}\n");
	}
}

#endif

/* ------------------------------------------------------------------------ */
/* [convert to SSA form] */

static void knh_BasicBlock_DFS(Ctx *ctx, knh_BasicBlock_t *p, knh_BasicBlock_t *n, knh_Array_t *vertex, knh_Array_t *pred)
{
	knh_BasicBlock_t *bbN, *bbJ;
	knh_BasicBlock_setVisited(n, 1);
	DP(n)->id = knh_Array_size(vertex);
	knh_Array_add(ctx, vertex, n);
	bbN = n->nextNC;
	bbJ = n->jumpNC;
	if (p != NULL)
		knh_Array_add(ctx, (knh_Array_t*)PRED(n), p);
	if (bbN != NULL) {
		if (!knh_BasicBlock_isVisited(bbN)) {
			knh_BasicBlock_DFS(ctx, n, bbN, vertex, pred);
		} else {
			knh_Array_add(ctx, (knh_Array_t*)PRED(bbN), n);
		}
	}
	if (bbJ != NULL) {
		if (!knh_BasicBlock_isVisited(bbJ)) {
			knh_BasicBlock_DFS(ctx, n, bbJ, vertex, pred);
		} else {
			knh_Array_add(ctx, (knh_Array_t*)PRED(bbJ), n);
		}
	}
}

static void knh_BasicBlock_setUnvisited(Ctx *ctx, knh_BasicBlock_t *bb)
{
	knh_BasicBlock_setVisited(bb, 0);
#ifdef K_USING_DEBUG
	knh_BasicBlock_print(bb);
#endif
	if (bb->nextNC != NULL && knh_BasicBlock_isVisited(bb->nextNC))
		knh_BasicBlock_setUnvisited(ctx, bb->nextNC);
	if (bb->jumpNC != NULL && knh_BasicBlock_isVisited(bb->jumpNC))
		knh_BasicBlock_setUnvisited(ctx, bb->jumpNC);
}

static knh_BasicBlock_t* getAncestorWLS(Ctx *ctx, knh_Array_t *vertex, knh_BasicBlock_t *v, knh_Array_t* ancestor, knh_Array_t* best, knh_Array_t* semi)
{
	knh_BasicBlock_t *a = (knh_BasicBlock_t*)ANCESTOR(v);
	knh_BasicBlock_t *b = NULL;
	if (ANCESTOR(a) != KNH_NULL) {
		b = getAncestorWLS(ctx, vertex, a, ancestor, best, semi);
		KNH_SETv(ctx, ANCESTOR(v), ANCESTOR(a));
		if (DFNUM((knh_BasicBlock_t*)SEMI(b)) < DFNUM((knh_BasicBlock_t*)SEMI((knh_BasicBlock_t*)BEST(v))))
			KNH_SETv(ctx, BEST(v), b);
	}
	return (knh_BasicBlock_t*)BEST(v);
}

static void knh_setInsert(Ctx *ctx, knh_Array_t *s, knh_BasicBlock_t *n)
{
	int i;
	for (i = 0; i < knh_Array_size(s); i++) {
		if (knh_Array_n(s, i) == (knh_Object_t*)n)
			return;
	}
	knh_Array_add(ctx, s, n);
}

static knh_BasicBlock_t* knh_BasicBlock_getParent(knh_Array_t *vertex, knh_BasicBlock_t *n)
{
	int i;
	for (i = 0; i < knh_Array_size(vertex); i++) {
		if (VERTEX(i)->nextNC == n || VERTEX(i)->jumpNC == n)
			return VERTEX(i);
	}
	return NULL;
}

static void knh_setIdom(Ctx *ctx, knh_Array_t *vertex, knh_Array_t* idom, knh_Array_t* pred)
{
	size_t i, j;
	size_t size = knh_Array_size(vertex);
	knh_Array_t *bucket = new_Array(ctx, CLASS_Array, 0);
	DBG_P("bucket = %p", bucket);
	knh_Array_t *semi = new_Array(ctx, CLASS_BasicBlock, 0);
	DBG_P("semi = %p", semi);
	knh_Array_t *ancestor = new_Array(ctx, CLASS_BasicBlock, 0);
	DBG_P("ancestor = %p", ancestor);
	knh_Array_t *samedom = new_Array(ctx, CLASS_BasicBlock, 0);
	DBG_P("samedom = %p", samedom);
	knh_Array_t *best = new_Array(ctx, CLASS_BasicBlock, 0);
	DBG_P("best = %p", best);
	knh_BasicBlock_t *n;
	knh_BasicBlock_t *p;
	knh_BasicBlock_t *s;
	knh_BasicBlock_t *_s;
	knh_BasicBlock_t *v;
	knh_BasicBlock_t *y;

	for (i = 0; i < size; i++) {
		knh_Array_add(ctx, bucket, new_Array(ctx, CLASS_BasicBlock, 0));
		DBG_P("bucket[%02d] = %p", i, knh_Array_n(bucket, i));
//		DBG_P("bucketsize = %lu", knh_Array_size(bucket));
		knh_Array_add(ctx, semi, KNH_NULL);
//		DBG_P("semi[%d] = %p", i, knh_Array_n(semi, i));
		knh_Array_add(ctx, ancestor, KNH_NULL);
//		DBG_P("ancestor[%d] = %p", i, knh_Array_n(ancestor, i));
		knh_Array_add(ctx, samedom, KNH_NULL);
//		DBG_P("samedom[%d] = %p", i, knh_Array_n(samedom, i));
		knh_Array_add(ctx, best, KNH_NULL);
//		DBG_P("best[%d] = %p", i, knh_Array_n(best, i));
	}

	for (i = size - 1; i > 0; i--) {
		//DBG_P("loop[%d]", i);
		n = VERTEX(i);
		p = PARENT(n);
		s = p;
		for (j = 0; j < knh_Array_size((knh_Array_t*)PRED(n)); j++) {
			v = (knh_BasicBlock_t*)knh_Array_n((knh_Array_t*)PRED(n), j);
			//DBG_P("pred[%d]=%d", j, DFNUM(PRED(n, j)));
			if (DFNUM(v) <= DFNUM(n)) {
				_s = v;
			} else {
				_s = (knh_BasicBlock_t*)SEMI(getAncestorWLS(ctx, vertex, v, ancestor, best, semi));
			}
			if (DFNUM(_s) < DFNUM(s))
				s = _s;
		}
		KNH_SETv(ctx, SEMI(n), s);
		knh_setInsert(ctx, BUCKET(s), n);
		//DBG_P("bucketsize[%02d] = %02d", DFNUM(s), knh_Array_size(BUCKET(s)));
		LINK(p, n);
		for (j = 0; j < knh_Array_size(BUCKET(p)); j++) {
			v = (knh_BasicBlock_t*)knh_Array_n(BUCKET(p), j);
			//DBG_P("bucket[%d]=%d", j, DFNUM(BUCKET(p, j)));
			y = getAncestorWLS(ctx, vertex, v, ancestor, best, semi);
			if (SEMI(y) == SEMI(v)) {
				KNH_SETv(ctx, IDOM(v), p);
			} else {
				KNH_SETv(ctx, SAMEDOM(v), y);
			}
		}
		knh_Array_clear(ctx, BUCKET(p), 0);
		//DBG_P("bucketsize[%02d] = %02d", DFNUM(p), knh_Array_size(BUCKET(p)));
	}
	for (i = 1; i < size; i++) {
		n = VERTEX(i);
		if (SAMEDOM(n) != KNH_NULL)
			KNH_SETv(ctx, IDOM(n), IDOM((knh_BasicBlock_t*)SAMEDOM(n)));
	}
	//DBG_P("bucketsize=%lu", knh_Array_size(bucket));
//	for (i = 0; i < size; i++) {
//		DBG_P("bucketsize[%d]=%lu", i, knh_Array_size((knh_Array_t*)knh_Array_n(bucket, i)));
//		knh_Array_clear(ctx, (knh_Array_t*)knh_Array_n(bucket, i), 0);
//	}
#ifdef K_USING_DEBUG
	knh_BasicBlock_printIDOM(ctx, vertex, samedom, ancestor, semi, idom);
#endif
	knh_Array_clear(ctx, bucket, 0);
	knh_Array_clear(ctx, semi, 0);
	knh_Array_clear(ctx, ancestor, 0);
	knh_Array_clear(ctx, samedom, 0);
	knh_Array_clear(ctx, best, 0);
}

static int isDominated(Ctx *ctx, knh_BasicBlock_t *n, knh_BasicBlock_t *w, knh_Array_t* idom)
{
	knh_BasicBlock_t *tmp;
	for (tmp = (knh_BasicBlock_t*)IDOM(w); tmp != (knh_BasicBlock_t*)KNH_NULL; tmp = (knh_BasicBlock_t*)IDOM(tmp)) {
		if (n == tmp)
			return 1;
	}
	return 0;
}

static void knh_Array_copy(Ctx *ctx, knh_Array_t *a, knh_Array_t *b)
{
	int i = 0;
	for (i = 0; i < knh_Array_size(b); i++) {
		knh_Array_add(ctx, a, (knh_BasicBlock_t*)knh_Array_n(b, i));
	}
}

static void knh_BasicBlock_computeDF(Ctx *ctx, knh_Array_t *vertex, knh_BasicBlock_t *n, knh_Array_t *df, knh_Array_t *idom)
{
	size_t i, j;
	size_t size = knh_Array_size(vertex);
	knh_BasicBlock_t *w, *c;
	knh_Array_t *S = new_Array(ctx, CLASS_BasicBlock, 0);
	DBG_P("S = %p", S);
	knh_BasicBlock_t *y;
	y = n->nextNC;
	if (y != NULL) {
		if (IDOM(y) != (knh_Object_t*)n)
			knh_setInsert(ctx, S, y);
	}
	y = n->jumpNC;
	if (y != NULL) {
		if (IDOM(y) != (knh_Object_t*)n)
			knh_setInsert(ctx, S, y);
	}
	for (i = 0; i < size; i++) {
		if (IDOM(c = (knh_BasicBlock_t*)VERTEX(i)) == (knh_Object_t*)n) {
			knh_BasicBlock_computeDF(ctx, vertex, c, df, idom);
			for (j = 0; j < knh_Array_size((knh_Array_t*)DF(c)); j++) {
				w = (knh_BasicBlock_t*)knh_Array_n((knh_Array_t*)DF(c), j);
				if (!isDominated(ctx, n, w, idom))
					knh_setInsert(ctx, S, w);
			}
		}
	}
	knh_Array_copy(ctx, (knh_Array_t*)DF(n), S);
	knh_Array_clear(ctx, S, 0);
}

static void knh_setInsertInt(Ctx *ctx, int *a, int n)
{
	int j = 0;
	while (a[j] != -1) {
		if (a[j] == n)
			return;
		j++;
	}
	a[j] = n;
	a[j + 1] = -1;
}

static void knh_setOrig(Ctx *ctx, int *Aorig, knh_BasicBlock_t *n)
{
	int i, size;
	knh_opline_t *op;
	size = DP(n)->size;
	for (i = 0; i < size; i++) {
		op = DP(n)->opbuf + i;
		if (op->opcode == OPCODE_NSET || op->opcode == OPCODE_NMOV || op->opcode == OPCODE_iINC)
			knh_setInsertInt(ctx, Aorig, op->data[0]);
	}
}

static int isContained(Ctx *ctx, knh_Array_t *a, knh_BasicBlock_t *n)
{
	int i;
	knh_BasicBlock_t *p;
	for (i = 0; i < knh_Array_size(a); i++) {
		p = (knh_BasicBlock_t*)knh_Array_n(a, i);
		if (p == n)
			return 1;
	}
	return 0;
}

static int isContainedInt(Ctx *ctx, int *a, int n)
{
	int i = 0;
	int p = a[i];
	while (p != -1) {
		if (p == n)
			return 1;
		i++;
		p = a[i];
	}
	return 0;
}

static void insertPhi(Ctx *ctx, knh_BasicBlock_t *n, int idx, size_t argc)
{
	int i;
	knh_opline_t *op1, *op2, tmp;
	DBG_P("insert phifunc of sfp[%02d] into bb[%02d]", idx, DFNUM(n));
	// at this point insert NOP instead of phifunc
	klr_NOP_t nop = {TADDR, OPCODE_NOP, ASMLINE};
	knh_BasicBlock_add_(ctx, n, 0, (knh_opline_t*)(&nop));
	for (i = DP(n)->size - 1; i > 0; i--) {
		op1 = (DP(n)->opbuf + i);
		op2 = (DP(n)->opbuf + i - 1);
		tmp = *op1;
		*op1 = *op2;
		*op2 = tmp;
	}
}

static void knh_BasicBlock_putPhifunc(Ctx *ctx, knh_Array_t *vertex, knh_Array_t* df)
{
	int i, j, a;
	int max = 0;
	knh_BasicBlock_t *n, *y;
	size_t size = knh_Array_size(vertex);
	int **Aorig = KNH_MALLOC(ctx, size * sizeof(int*));
	knh_Array_t *Aphi = new_Array(ctx, CLASS_Array, 0);
	//DBG_P("Aphi = %p", Aphi);
	knh_Array_t *defsites = new_Array(ctx, CLASS_Array, 0);
	//DBG_P("defsites = %p", defsites);
	knh_Array_t *W = new_Array(ctx, CLASS_BasicBlock, 0);
	//DBG_P("W = %p", W);
	for (i = 0; i < size; i++) {
		n = VERTEX(i);
		Aorig[i] = (int*)KNH_MALLOC(ctx, (DP(n)->size == 0 ? 1 : DP(n)->size) * sizeof(int));
		*Aorig[i] = -1;
		if (DP(n)->size > 0) {
			knh_setOrig(ctx, Aorig[i], n);
			for (j = 0; (a = *(Aorig[i] + j)) != -1; j++) {
				while (knh_Array_size(defsites) <= a) {
					knh_Array_add(ctx, defsites, new_Array(ctx, CLASS_BasicBlock, 0));
					max = a;
				}
				knh_setInsert(ctx, DEFSITES(a), n);
			}
		}
	}
	for (a = 0; a <= max; a++) {
		if (DEFSITES(a) != NULL) {
			knh_Array_add(ctx, Aphi, new_Array(ctx, CLASS_BasicBlock, 0));
			knh_Array_copy(ctx, W, DEFSITES(a));
			knh_Array_clear(ctx, DEFSITES(a), 0);
			while (knh_Array_size(W) > 0) {
				n = (knh_BasicBlock_t*)knh_Array_n(W, knh_Array_size(W) - 1);
				knh_Array_clear(ctx, W, knh_Array_size(W) - 1);
				//DBG_P("n=%p", n);
				for (i = 0; i < knh_Array_size((knh_Array_t*)DF(n)); i++) {
					y = (knh_BasicBlock_t*)knh_Array_n((knh_Array_t*)DF(n), i);
					if (!isContained(ctx, APHI(a), y)) {
						insertPhi(ctx, y, a, DP(y)->incoming);
						knh_setInsert(ctx, APHI(a), y);
						knh_setOrig(ctx, Aorig[DFNUM(y)], y);
						if (!isContainedInt(ctx, Aorig[DFNUM(y)], a))
							knh_setInsert(ctx, W, y);
					}
				}
			}
		}
	}
	for (i = 0; i < size; i++) {
		n = VERTEX(i);
		KNH_FREE(ctx, Aorig[i], (DP(n)->size == 0 ? 1 : DP(n)->size) * sizeof(int));
	}
	for (a = 1; a <= max; a++) {
		if (APHI(a) != NULL)
			knh_Array_clear(ctx, APHI(a), 0);
		if (DEFSITES(a) != NULL)
			knh_Array_clear(ctx, DEFSITES(a), 0);
	}
	knh_Array_clear(ctx, Aphi, 0);
	knh_Array_clear(ctx, defsites, 0);
	knh_Array_clear(ctx, W, 0);
	KNH_FREE(ctx, Aorig, size * sizeof(int*));
}

static void knh_BasicBlock_convert(Ctx *ctx, knh_BasicBlock_t *bb)
{
	int i;
	size_t size = knh_Array_size(bb->listNC);
	knh_Array_t *df = new_Array(ctx, CLASS_Array, 0);
	DBG_P("DF = %p", df);
	knh_Array_t *pred = new_Array(ctx, CLASS_Array, 0);
	knh_Array_t *idom = new_Array(ctx, CLASS_BasicBlock, 0);
	knh_Array_t *vertex = new_Array(ctx, CLASS_BasicBlock, 0);
	for (i = 0; i < size; i++) {
		knh_Array_add(ctx, df, new_Array(ctx, CLASS_BasicBlock, 0));
		knh_Array_add(ctx, pred, new_Array(ctx, CLASS_BasicBlock, 0));
		knh_Array_add(ctx, idom, KNH_NULL);
	}

	knh_BasicBlock_DFS(ctx, NULL, bb, vertex, pred);
	knh_setIdom(ctx, vertex, idom, pred);
	knh_BasicBlock_computeDF(ctx, vertex, bb, df, idom);
#ifdef K_USING_DEBUG
	knh_BasicBlock_printDF(ctx, vertex, df);
#endif
	knh_BasicBlock_putPhifunc(ctx, vertex, df);
	for (i = 0; i < size; i++) {
		knh_Array_clear(ctx, (knh_Array_t*)knh_Array_n(df, i), 0);
		knh_Array_clear(ctx, (knh_Array_t*)knh_Array_n(pred, i), 0);
	}
	knh_Array_clear(ctx, df, 0);
	knh_Array_clear(ctx, pred, 0);
	knh_Array_clear(ctx, idom, 0);
	knh_Array_clear(ctx, vertex, 0);
	knh_BasicBlock_setUnvisited(ctx, bb);
}

/* ------------------------------------------------------------------------ */
/* [convert out of SSA form] */

static void knh_BasicBlock_convertOut(Ctx *ctx, knh_BasicBlock_t *bb)
{
	// TODO
}

/* ------------------------------------------------------------------------ */
/* [SSA optimize] */
void knh_BasicBlock_optimize(Ctx *ctx, knh_BasicBlock_t *bb)
{
	knh_BasicBlock_convert(ctx, bb);
	knh_BasicBlock_convertOut(ctx, bb);
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif // K_USING_SSA
