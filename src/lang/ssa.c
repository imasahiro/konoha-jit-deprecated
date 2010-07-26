/****************************************************************************
 * KONOHA COPYRIGHT, LICENSE NOTICE, AND DISCRIMER  
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

#ifdef K_USING_SSA

#include"commons.h"

#define NEXT            0
#define JUMP            1
#define vertex(o)       ((knh_BasicBlock_t*)knh_Array_n(bb->listNC, o))
#define parent(o)       knh_BasicBlock_getParent(bb, o)
#define ancestor(o)     (knh_Array_n(ancestor, DP(o)->id))
#define semi(o)         (knh_Array_n(semi, DP(o)->id))
#define dfnum(o)        (DP(o)->id)
#define best(o)         (knh_Array_n(best, DP(o)->id))
#define idom(o)         (knh_Array_n(idom, DP(o)->id))
#define samedom(o)      (knh_Array_n(samedom, DP(o)->id))
#define bucket(o, n)    ((knh_BasicBlock_t*)knh_Array_n((knh_Array_t*)knh_Array_n(bucket, DP(o)->id), n))
#define pred(o, n)      ((knh_BasicBlock_t*)knh_Array_n((knh_Array_t*)knh_Array_n(pred, DP(o)->id), n))
#define link(p, n)      ancestor(n) = (knh_Object_t*)p; best(n) = (knh_Object_t*)n

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

static void knh_DomTree_print(Ctx *ctx, knh_BasicBlock_t *bb, knh_Array_t *samedom, knh_Array_t *ancestor, knh_Array_t *semi, knh_Array_t *idom)
{
	int i;
	int size = knh_Array_size(bb->listNC);
	for (i = 0; i < size; i++) {
		fprintf(stderr, "[%02d] samedom=", i);
		if (samedom(vertex(i)) != NULL) {
			fprintf(stderr, "%02d", dfnum((knh_BasicBlock_t*)samedom(vertex(i))));
		} else {
			fprintf(stderr, "NN");
		}
		fprintf(stderr, ", ancestor=");
		if (ancestor(vertex(i)) != NULL) {
			fprintf(stderr, "%02d", dfnum((knh_BasicBlock_t*)ancestor(vertex(i))));
		} else {
			fprintf(stderr, "NN");
		}
		fprintf(stderr, ", semi=");
		if (semi(vertex(i)) != NULL) {
			fprintf(stderr, "%02d", dfnum((knh_BasicBlock_t*)semi(vertex(i))));
		} else {
			fprintf(stderr, "NN");
		}
		fprintf(stderr, ", idom=");
		if (knh_Array_n(idom, i) != NULL) {
			fprintf(stderr, "%02d\n", dfnum((knh_BasicBlock_t*)knh_Array_n(idom, i)));
		} else {
			fprintf(stderr, "NN\n");
		}
	}
}
#endif

/* ------------------------------------------------------------------------ */
/* [convert to SSA form] */

static void knh_BasicBlock_DFS(Ctx *ctx, knh_BasicBlock_t *p, knh_BasicBlock_t *n, knh_Array_t *pred, int *depth)
{
	knh_BasicBlock_t *bbN, *bbJ;
	knh_BasicBlock_setVisited(n, 1);
	DP(n)->id = *depth;
	knh_Array_n(n->listNC, *depth) = (knh_Object_t*)n;
	*depth += 1;
	bbN = n->nextNC;
	bbJ = n->jumpNC;
	if (p != NULL)
		knh_Array_add(ctx, (knh_Array_t*)knh_Array_n(pred, dfnum(n)), p);
	if (bbN != NULL) {
		if (!knh_BasicBlock_isVisited(bbN)) {
			knh_BasicBlock_DFS(ctx, n, bbN, pred, depth);
		} else {
			knh_Array_add(ctx, (knh_Array_t*)knh_Array_n(pred, dfnum(bbN)), n);
		}
	}
	if (bbJ != NULL) {
		if (!knh_BasicBlock_isVisited(bbJ)) {
			knh_BasicBlock_DFS(ctx, n, bbJ, pred, depth);
		} else {
			knh_Array_add(ctx, (knh_Array_t*)knh_Array_n(pred, dfnum(bbJ)), n);
		}
	}
}

static void knh_BasicBlock_setUnvisited(Ctx *ctx, knh_BasicBlock_t *bb)
{
	knh_BasicBlock_setVisited(bb, 0);
#ifdef K_USING_DEBUG
	knh_BasicBlock_print(bb);
#endif
	if (bb->nextNC != NULL && knh_BasicBlock_isVisited(bb->nextNC)) {
		knh_BasicBlock_setUnvisited(ctx, bb->nextNC);
	}
	if (bb->jumpNC != NULL && knh_BasicBlock_isVisited(bb->jumpNC)) {
		knh_BasicBlock_setUnvisited(ctx, bb->jumpNC);
	}
}

static knh_BasicBlock_t* getAncestorWLS(knh_BasicBlock_t *bb, knh_BasicBlock_t *v, knh_Array_t* ancestor, knh_Array_t* best, knh_Array_t* semi)
{
	knh_BasicBlock_t *a = (knh_BasicBlock_t*)ancestor(v);
	knh_BasicBlock_t *b = NULL;
	if (ancestor(a) != NULL) {
		b = getAncestorWLS(bb, a, ancestor, best, semi);
		ancestor(v) = ancestor(a);
		if (dfnum((knh_BasicBlock_t*)semi(b)) < dfnum((knh_BasicBlock_t*)semi((knh_BasicBlock_t*)best(v)))) {
			best(v) = (knh_Object_t*)b;
		}
	}
	return (knh_BasicBlock_t*)best(v);
}

static void knh_setInsert(Ctx *ctx, knh_Array_t* bucket, knh_BasicBlock_t *n)
{
	int i = 0;
	while (knh_Array_n(bucket, i) != NULL) {
		if (knh_Array_n(bucket, i) == (knh_Object_t*)n) return;
		i++;
	}
	knh_Array_add(ctx, bucket, n);
}

static knh_BasicBlock_t* knh_BasicBlock_getParent(knh_BasicBlock_t *bb, knh_BasicBlock_t *n)
{
	int i;
	for (i = 0; i < knh_Array_size(bb->listNC); i++) {
		if (vertex(i)->nextNC == n || vertex(i)->jumpNC == n)
			return vertex(i);
	}
	return NULL;
}

static void knh_setIdom(Ctx *ctx, knh_BasicBlock_t *bb, knh_Array_t *idom, knh_Array_t *pred)
{
	size_t i, j;
	size_t size = knh_Array_size(bb->listNC);
	knh_Array_t *bucket = new_Array(ctx, CLASS_Array, size);
	knh_Array_t *semi = new_Array(ctx, CLASS_BasicBlock, size);
	knh_Array_t *ancestor = new_Array(ctx, CLASS_BasicBlock, size);
	knh_Array_t *samedom = new_Array(ctx, CLASS_BasicBlock, size);
	knh_Array_t *best = new_Array(ctx, CLASS_BasicBlock, size);
	knh_BasicBlock_t *n;
	knh_BasicBlock_t *p;
	knh_BasicBlock_t *s;
	knh_BasicBlock_t *_s;
	knh_BasicBlock_t *v;
	knh_BasicBlock_t *y;

	for (i = 0; i < size; i++) {
		knh_Array_add(ctx, bucket, new_Array(ctx, CLASS_BasicBlock, size));
	}

	for (i = size - 1; i > 0; i--) {
		//DBG_P("loop[%d]", i);
		n = vertex(i);
		p = parent(n);
		s = p;
		j = 0;
		for (v = (knh_BasicBlock_t*)pred(n, j); v != NULL; v = pred(n, ++j)) {
			//DBG_P("pred[%d]=%d", j, dfnum(pred(n, j)));
			if (dfnum(v) <= dfnum(n)) {
				_s = v;
			} else {
				_s = (knh_BasicBlock_t*)semi(getAncestorWLS(bb, v, ancestor, best, semi));
			}
			if (dfnum(_s) < dfnum(s)) {
				s = _s;
			}
		}
		semi(n) = (knh_Object_t*)s;
		knh_setInsert(ctx, (knh_Array_t*)knh_Array_n(bucket, DP(s)->id), n);
		link(p, n);
		j = 0;
		for (v = bucket(p, j); v != NULL; v = bucket(p, ++j)) {
			//DBG_P("bucket[%d]=%d", j, dfnum(bucket(p, j)));
			y = getAncestorWLS(bb, v, ancestor, best, semi);
			if (semi(y) == semi(v)) {
				idom(v) = (knh_Object_t*)p;
			} else {
				samedom(v) = (knh_Object_t*)y;
			}
		}
		if (bucket(p, 0) != NULL) {
			knh_Array_clear(ctx, (knh_Array_t*)bucket(p, 0), 0);
		}
	}
	for (i = 1; i < size; i++) {
		n = vertex(i);
		if (samedom(n) != NULL) {
			idom(n) = idom((knh_BasicBlock_t*)samedom(n));
		}
	}
#ifdef K_USING_DEBUG
	knh_DomTree_print(ctx, bb, samedom, ancestor, semi, idom);
#endif
	knh_Array_clear(ctx, bucket, 0);
	knh_Array_clear(ctx, semi, 0);
	knh_Array_clear(ctx, ancestor, 0);
	knh_Array_clear(ctx, samedom, 0);
	knh_Array_clear(ctx, best, 0);
}

//static void knh_BasicBlock_computeDF(knh_BasicBlock_t *bb, knh_BasicBlock_t *n, knh_BasicBlock_t* idom[], knh_BasicBlock_t* df[], int size)
//{
//	knh_BasicBlock_t *y;
//	y = n->nextNC;
//	if (y != NULL) {
//		if (idom(y) != n)
//	}
//	y = n->jumpNC;
//	if (y != NULL) {
//	}
//}

static void knh_BasicBlock_convert(Ctx *ctx, knh_BasicBlock_t *bb)
{
	int depth = 0;
	int i;
	size_t size = knh_Array_size(bb->listNC);
	knh_Array_t *df = new_Array(ctx, CLASS_Array, size);
	knh_Array_t *pred = new_Array(ctx, CLASS_Array, size);
	knh_Array_t *idom = new_Array(ctx, CLASS_BasicBlock, size);
	for (i = 0; i < size; i++) {
		knh_Array_add(ctx, df, new_Array(ctx, CLASS_BasicBlock, size));
		knh_Array_add(ctx, pred, new_Array(ctx, CLASS_BasicBlock, size));
	}

	knh_BasicBlock_DFS(ctx, NULL, bb, pred, &depth);
	knh_setIdom(ctx, bb, idom, pred);
//	for (i = 0; i < depth; i++) {
//		knh_BasicBlock_computeDF(bb, vertex(i), idom, &df[i][0], depth);
//	}
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

#endif
