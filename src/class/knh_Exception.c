/****************************************************************************
 * KONOHA COPYRIGHT, LICENSE NOTICE, AND DISCRIMER
 *
 * Copyright (c) 2005-2009, Kimio Kuramitsu <kimio at ynu.ac.jp>
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

#ifdef KONOHA_ON_LKM
#include<linux/errno.h>
static int errno;
#else

#ifdef KNH_USING_POSIX
#include<errno.h>
#else
static int errno;
#endif
#endif

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ======================================================================== */
/* [ExptTable] */

static
knh_expt_t knh_ExptTable_newId(Ctx *ctx)
{
	knh_class_t newid = 0;
	KNH_LOCK(ctx, LOCK_SYSTBL, NULL);
	if(!(ctx->share->ExptTableSize < KNH_TEXPT_SIZE)) {
		KNH_EXIT("Enlarge KNH_TEXPT_SIZE %d", KNH_TEXPT_SIZE);
		goto L_UNLOCK;
	}
	((knh_SharedData_t*)ctx->share)->ExptTableSize += 1;
	newid = ctx->share->ExptTableSize;
	L_UNLOCK:
	KNH_UNLOCK(ctx, LOCK_SYSTBL, NULL);
	return newid;
}

/* ------------------------------------------------------------------------ */

int knh_expt_isa(Ctx *ctx, knh_expt_t eid, knh_expt_t parent)
{
	KNH_ASSERT_eid(eid);
	KNH_ASSERT(parent <= ctx->share->ExptTableSize);
	if(eid == parent || parent == 1) return 1;
	if(eid == 1) return 0;
	while((eid = ctx->share->ExptTable[eid-1].parent) != 1) {
		if(eid == parent) return 1;
	}
	return 0;
}

/* ------------------------------------------------------------------------ */

knh_String_t *knh_getExptName(Ctx *ctx, knh_expt_t eid)
{
	KNH_ASSERT_eid(eid);
	return ctx->share->ExptTable[eid-1].name;
}

/* ------------------------------------------------------------------------ */
/* [forname] */

knh_expt_t knh_geteid(Ctx *ctx, knh_bytes_t msg, knh_expt_t def)
{
	knh_intptr_t loc = knh_bytes_index(msg, '!');
	if(loc != -1) {
		if(msg.buf[loc+1] != '!') {
			return EXPT_Exception;
		}
		msg = knh_bytes_first(msg, loc);
	}

	if(msg.len == 0) return EXPT_Exception; /* '!!' */

	knh_expt_t eid = (knh_expt_t)knh_DictSet_get__b(DP(ctx->sys)->ExptNameDictSet, msg);
	if(eid != 0) return eid;
	if(def == EXPT_newid) {
		return knh_addException(ctx, 0, EXPT_newid, new_String(ctx, msg, NULL), EXPT_Exception);
	}
	return def;
}

/* ------------------------------------------------------------------------ */
/* [TABLE] */

knh_expt_t
knh_addException(Ctx *ctx, knh_flag_t flag, knh_class_t eid, knh_String_t *name, knh_class_t peid)
{
	if(eid == EXPT_newid) {
		eid = knh_ExptTable_newId(ctx);
	}else {
		((knh_SharedData_t*)ctx->share)->ExptTableSize += 1;
		DBG2_ASSERT(eid == ctx->share->ExptTableSize);
	}
	KNH_ASSERT_eid(eid);

	knh_ExptTable_t *t = pExptTable(eid-1);
	DBG2_ASSERT(t->name == NULL);
	t->flag = flag;
	t->parent = peid;
	KNH_INITv(t->name, name);
	knh_DictSet_set(ctx, DP(ctx->sys)->ExptNameDictSet, name, eid);
	return eid;
}

/* ======================================================================== */
/* [Exception.new] */

/* @method Exception! Exception.new:init(String e, String msg, Object bag) */
// new Exception("Security!!", "hogehoge");
// new Exception("Security!!: hogehoge", null);

knh_Exception_t* knh_Exception_new__init(Ctx *ctx, knh_Exception_t *o, knh_String_t *e, knh_String_t *msg, Object *bag)
{
	knh_expt_t eid = EXPT_Exception;
	if(IS_NOTNULL(e)) {
		KNH_RCSETv(ctx, DP(o)->bag, e);
		eid = knh_geteid(ctx, knh_String_tobytes(e), EXPT_newid /*EXPT_unknown*/);
	}
	if(eid == EXPT_unknown) {
		KNH_WARNING(ctx, "unknown exception: %s", knh_String_tochar(e));
		DP(o)->eid = EXPT_Exception;
	}
	else {
		DP(o)->eid = eid;
	}
	KNH_ASSERT_eid(eid);
	DP(o)->flag = ctx->share->ExptTable[DP(o)->eid].flag;
	{
		if(IS_NOTNULL(msg)) {
			KNH_RCSETv(ctx, DP(o)->bag, msg);
			knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
			knh_write_char(ctx, cwb->w, EXPTN(DP(o)->eid));
			knh_write_char(ctx, cwb->w, "!!: ");
			knh_write(ctx, cwb->w, knh_String_tobytes(msg));
			KNH_SETv(ctx, DP(o)->msg, knh_cwb_newString(ctx, cwb));
		}
		else {
			int loc = knh_bytes_indexOf(knh_String_tobytes(e), STEXT("!!:"));
			if(loc > 0 && eid != EXPT_unknown) {
				KNH_SETv(ctx, DP(o)->msg, e);
				return o;
			}
			knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
			knh_write_char(ctx, cwb->w, EXPTN(DP(o)->eid));
			knh_write_char(ctx, cwb->w, "!!");
			if(loc > 0) {
				knh_write_char(ctx, cwb->w, ": ");
				knh_write(ctx, cwb->w, knh_bytes_last(knh_String_tobytes(e), loc+3));
			}
			KNH_SETv(ctx, DP(o)->msg, knh_cwb_newString(ctx, cwb));
		}
	}
	KNH_SETv(ctx, DP(o)->bag, bag);
	return o;
}

/* ------------------------------------------------------------------------ */

int knh_Exception_isa(Ctx *ctx, knh_Exception_t *o, knh_String_t *msg)
{
	int res = 0;
	knh_expt_t eid = knh_geteid(ctx, knh_String_tobytes(msg), EXPT_unknown);
	if(eid != EXPT_unknown) {
		res = knh_expt_isa(ctx, DP(o)->eid, eid);
	}
	return res;
}

/* ------------------------------------------------------------------------ */

KNHAPI(knh_Exception_t*) new_Exception(Ctx *ctx, knh_String_t *msg)
{
	knh_Exception_t* o = (knh_Exception_t*)new_Object_bcid(ctx, CLASS_Exception, 0);
	knh_Exception_new__init(ctx, o, msg, (knh_String_t*)KNH_NULL, (Object*)KNH_NULL);
	return o;
}

/* ------------------------------------------------------------------------ */

KNHAPI(knh_Exception_t*) new_Exception__b(Ctx *ctx, knh_bytes_t msg)
{
	knh_Exception_t* o = (knh_Exception_t*)new_Object_bcid(ctx, CLASS_Exception, 0);
	knh_Exception_new__init(ctx, o, new_String(ctx, msg, NULL), (knh_String_t*)KNH_NULL, (Object*)KNH_NULL);
	return o;
}

/* ------------------------------------------------------------------------ */

knh_Exception_t* new_NullException (Ctx *ctx, Object *o)
{
	return new_Exception__b(ctx, STEXT("Null!!"));
}

/* ------------------------------------------------------------------------ */

KNHAPI(void) knh_cwb_perrno(Ctx *ctx, knh_cwb_t *cwb, char *emsg, char *func, char *file, int line, int isThrowable)
{
	if(emsg == NULL) emsg = "Exception!!";
	if(errno == 13) {
		emsg = "Security!!";
	}
#ifdef _MSC_VER
	char emsg2[256];
	strerror_s(emsg2, sizeof(emsg2), errno);
#else
	char *emsg2 = strerror(errno);
#endif
	if(isThrowable) {
		char buf[256];
		knh_snprintf(buf, sizeof(buf), "%s: %s at %s (errno=%d)", emsg, emsg2, func, errno);
		if(cwb != NULL) {
			knh_cwb_close(cwb);
		}
		knh_throwException(ctx, new_Exception__b(ctx, B(buf)), knh_safefile(file), line);
	}
	else {
		KNH_NOTICE(ctx, "%s() says %s (errno=%d)", func, emsg2, errno);
	}
}

/* ------------------------------------------------------------------------ */

KNHAPI(void) knh_throw_Unsupported(Ctx *ctx, knh_cwb_t *cwb, const char *func, char *file, int line, int isThrowable)
{
	if(isThrowable) {
		char buf[256];
		knh_snprintf(buf, sizeof(buf), "Unsupported!!: func=%s", func);
		if(cwb != NULL) {
			knh_cwb_close(cwb);
		}
		knh_throwException(ctx, new_Exception__b(ctx, B(buf)), knh_safefile(file), line);
	}
	else {
		KNH_NOTICE(ctx, "unsupported: %s", func);
	}
}

/* ------------------------------------------------------------------------ */

KNHAPI(void) knh_throw_OutOfIndex(Ctx *ctx, knh_int_t n, size_t max, char *file, int line)
{
	char buf[80];
	knh_snprintf(buf, sizeof(buf), "OutOfIndex!!: %ld <%ld", (long)n, (long)max);
	knh_throwException(ctx, new_Exception__b(ctx, B(buf)), knh_safefile(file), line);
}

/* ======================================================================== */
/* [ExceptionHandler] */

knh_ExceptionHandler_t* new_ExceptionHandler(Ctx *ctx)
{
	return (knh_ExceptionHandler_t*)new_Object_bcid(ctx, CLASS_ExceptionHandler, 0);
}

/* ------------------------------------------------------------------------ */

void knh_ExceptionHandler_longjmp(Ctx *ctx, knh_ExceptionHandler_t *o, knh_Exception_t *e)
{
	KNH_ASSERT(IS_Exception(e));
	KNH_SETv(ctx, DP(o)->caught, e);
	knh_longjmp(DP(o)->jmpbuf, ((int)DP(e)->eid));
}

/* ------------------------------------------------------------------------ */

knh_Exception_t* knh_ExceptionHandler_getCaughtException(knh_ExceptionHandler_t *o)
{
	KNH_ASSERT(IS_Exception(DP(o)->caught));
	return DP(o)->caught;
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
