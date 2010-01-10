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
/* [prototype] */

knh_NameSpace_t* new_NameSpace(Ctx *ctx, knh_NameSpace_t *nsNULL)
{
	knh_NameSpace_t* ns = (knh_NameSpace_t*)new_Object_bcid(ctx, CLASS_NameSpace, 0);
	if(nsNULL != NULL) {
		DBG2_ASSERT(IS_NameSpace(nsNULL));
		KNH_INITv(DP(ns)->parent, nsNULL);
		KNH_SETv(ctx, DP(ns)->nsname, DP(nsNULL)->nsname);
	}
	return ns;
}

/* ------------------------------------------------------------------------ */

knh_NameSpace_t* knh_getGammaNameSpace(Ctx *ctx)
{
	return DP(ctx->kc)->ns;
}

/* ------------------------------------------------------------------------ */
/* [class] */

void knh_NameSpace_setcid(Ctx *ctx, knh_NameSpace_t *ns, knh_String_t *name, knh_class_t cid)
{
	if(!IS_DictSet(DP(ns)->name2cidDictSet)) {
		KNH_SETv(ctx, DP(ns)->name2cidDictSet, new_DictSet(ctx, 0));
	}
	knh_DictSet_set(ctx, DP(ns)->name2cidDictSet, name, (knh_uintptr_t)(cid+1));
}

/* ------------------------------------------------------------------------ */

void knh_NameSpace_useClass(Ctx *ctx, knh_NameSpace_t *ns, knh_bytes_t pkgname)
{
	size_t cid;
	if(pkgname.buf[pkgname.len-1] == '.') {
		pkgname.len--;
	}
	for(cid = 0; cid < ctx->share->ClassTableSize; cid++) {
		if(ClassTable(cid).sname == NULL) continue;
		if(knh_class_isPrivate(cid) && knh_class_isGenerics(cid)) continue;
		knh_bytes_t cname = __tobytes(ClassTable(cid).lname);
		if(knh_bytes_startsWith(cname, pkgname)
				&& cname.buf[pkgname.len] == '.' && isupper(cname.buf[pkgname.len+1])) {
			knh_NameSpace_setcid(ctx, ns, ClassTable(cid).sname, (knh_class_t)cid);
		}
	}
}

/* ------------------------------------------------------------------------ */

knh_class_t knh_NameSpace_getcid(Ctx *ctx, knh_NameSpace_t *ns, knh_bytes_t sname)
{
	L_TAIL:
	if(IS_DictSet(DP(ns)->name2cidDictSet)) {
		knh_uintptr_t cid = knh_DictSet_get__b(DP(ns)->name2cidDictSet, sname);
		if(cid > 0) return (knh_class_t)(cid-1);
	}
	if(IS_NameSpace(DP(ns)->parent)) {
		ns = DP(ns)->parent;
		goto L_TAIL;
	}
	return CLASS_unknown;
}

/* ------------------------------------------------------------------------ */
/* [const] */

Object *knh_NameSpace_getConstNULL(Ctx *ctx, knh_NameSpace_t *ns, knh_bytes_t name)
{
	knh_index_t idx = knh_bytes_rindex(name, '.');
	if(idx > 0) { /*ns.Class.CONST*/
		knh_class_t cid = knh_NameSpace_getcid(ctx, ns, knh_bytes_first(name, idx));
		if(cid == CLASS_unknown) cid = CLASS_Any;
		return knh_getClassConstNULL(ctx, cid, knh_bytes_last(name, idx+1));
	}
	L_TAIL:
	if(IS_DictMap(DP(ns)->lconstDictMap)) {
		idx = knh_DictMap_index(DP(ns)->lconstDictMap, name);
		if(idx != -1) return knh_DictMap_valueAt(DP(ns)->lconstDictMap, idx);
	}
	if(IS_NameSpace(DP(ns)->parent)) {
		ns = DP(ns)->parent;
		goto L_TAIL;
	}
	return NULL;
}

/* ------------------------------------------------------------------------ */

void knh_NameSpace_addConst(Ctx *ctx, knh_NameSpace_t *ns, knh_String_t *name, Object *value)
{
	knh_bytes_t t = __tobytes(name);
	knh_index_t loc = knh_bytes_rindex(t, '.');
	if(loc > 0) {
		knh_class_t cid = knh_NameSpace_getcid(ctx, ns, knh_bytes_first(t, loc));
		if(cid == CLASS_unknown) cid = CLASS_Any;
		knh_addClassConst(ctx, cid, new_String(ctx, knh_bytes_last(t, loc+1), NULL), value);
	}
	else {
		if(!IS_DictMap(DP(ns)->lconstDictMap)) {
			KNH_SETv(ctx, DP(ns)->lconstDictMap, new_DictMap0(ctx, 0));
		}
		knh_DictMap_set(ctx, DP(ns)->lconstDictMap, name, value);
	}
}

/* ------------------------------------------------------------------------ */
/* [function] */

void knh_NameSpace_setFuncClass(Ctx *ctx, knh_NameSpace_t *ns, knh_methodn_t mn, knh_class_t c)
{
	if(!METHODN_IS_GETTER(mn) && !METHODN_IS_SETTER(mn)) {
		if(!IS_DictSet(DP(ns)->func2cidDictSet)) {
			KNH_SETv(ctx, DP(ns)->func2cidDictSet, new_DictSet(ctx, 0));
		}
		knh_DictSet_set(ctx, DP(ns)->func2cidDictSet, knh_getFieldName(ctx, METHODN_TOFIELDN(mn)), (knh_uintptr_t)(c+1));
	}
}

/* ------------------------------------------------------------------------ */

knh_class_t knh_NameSpace_getFuncClass(Ctx *ctx, knh_NameSpace_t *ns, knh_methodn_t mn)
{
	if(!METHODN_IS_GETTER(mn) && !METHODN_IS_SETTER(mn)) {
		knh_bytes_t name = __tobytes(knh_getFieldName(ctx, METHODN_TOFIELDN(mn)));
		L_TAIL:
		if(IS_DictSet(DP(ns)->func2cidDictSet)) {
			knh_uintptr_t cid = knh_DictSet_get__b(DP(ns)->func2cidDictSet, name);
			if(cid != 0) {
				return (knh_class_t)(cid-1);
			}
		}
		if(IS_NameSpace(DP(ns)->parent)) {
			ns = DP(ns)->parent;
			goto L_TAIL;
		}
	}
	return CLASS_unknown; /* if not found */
}

/* ======================================================================== */
/* [class] */

knh_class_t knh_getcid(Ctx *ctx, knh_bytes_t lname)
{
	knh_uintptr_t cid = knh_DictSet_get__b(DP(ctx->sys)->ClassNameDictSet, lname);
	if(cid > 0) return (knh_class_t)(cid-1);
	if(lname.buf[lname.len-1] == '}') {
		return knh_findcidx(ctx, lname);
	}
	return CLASS_unknown;
}

/* ------------------------------------------------------------------------ */

static
knh_bytes_t knh_NameSpace_firstType(Ctx *ctx, knh_NameSpace_t *o, knh_bytes_t name, knh_type_t *type)
{
	int i, level = 1;
	for(i = 0; i < (int)name.len; i++) {
		int ch = name.buf[i];
		if(ch == '(' || ch == '<') level++;
		if(ch == ')' || ch == '>') level--;
		if((ch == ',' && level == 1) ||((ch == '>' || ch == ')') && level == 0)) {
			knh_bytes_t t = {name.buf, i};
			*type = knh_NameSpace_gettype(ctx, o, t, 0);
			if(*type == CLASS_unknown) {
				*type = TYPE_Any;
			}
			name.buf = name.buf+i+1;
			name.len = name.len-(i+1);
			return name;
		}
	}
	return STEXT("");
}

/* ------------------------------------------------------------------------ */

knh_class_t knh_NameSpace_findcid(Ctx *ctx, knh_NameSpace_t *ns, knh_bytes_t name)
{
	knh_class_t cid, p1;
//	if(knh_bytes_equals(name, STEXT("This"))) {
//		return CLASS_This;
//	}
//	if(knh_bytes_equals(name, STEXT("Script"))) {
//		return knh_Object_cid(knh_NameSpace_getScript(ctx, o));
//	}
//	if(knh_bytes_equals(name, STEXT("byte[]"))) {
//		return CLASS_Bytes;
//	}

	cid = knh_NameSpace_getcid(ctx, ns, name);
	if(cid != CLASS_unknown) {
		return cid;
	}

	cid = knh_getcid(ctx, name);
	if(cid != CLASS_unknown) {
		knh_NameSpace_setcid(ctx, ns, ClassTable(cid).sname, cid);
	}

	if(name.buf[name.len-1] == ']' && name.buf[name.len-2] == '[') {
		name.len -= 2;
		p1 = knh_NameSpace_findcid(ctx, ns, name);
		if(p1 == CLASS_unknown) return CLASS_Array;
		cid = knh_class_Array(ctx, p1);
		if(cid != CLASS_unknown) {
			name.len += 2;
			knh_NameSpace_setcid(ctx, ns, new_String(ctx, name, NULL), cid);
		}
		return cid;
	}

	if(name.buf[name.len-1] == '.' && name.buf[name.len-2] == '.') {
		name.len -= 2;
		p1 = knh_NameSpace_findcid(ctx, ns, name);
		if(p1 == CLASS_unknown) return CLASS_Iterator;
		cid = knh_class_Iterator(ctx, p1);
		if(cid != CLASS_unknown) {
			name.len += 2;
			knh_NameSpace_setcid(ctx, ns, new_String(ctx, name, NULL), cid);
		}
		return cid;
	}

	if(name.buf[name.len-1] == '>') {
		knh_index_t loc = knh_bytes_index(name, '<');
		if(loc > 0) {
			knh_class_t bcid = knh_NameSpace_findcid(ctx, ns, knh_bytes_first(name, loc));
			if(bcid == CLASS_unknown) return CLASS_unknown;
			DBG2_ASSERT_cid(bcid);
			if(!knh_class_isGenerics(bcid)) return bcid;
			knh_class_t p1 = CLASS_Any, p2 = ClassTable(bcid).p2;
			knh_bytes_t nsub = knh_NameSpace_firstType(ctx, ns, knh_bytes_last(name, loc+1), &p1);
			knh_NameSpace_firstType(ctx, ns, nsub, &p2);
			cid = knh_class_Generics(ctx, bcid, CLASS_type(p1), CLASS_type(p2));
			if(cid != CLASS_unknown) {
				knh_NameSpace_setcid(ctx, ns, new_String(ctx, name, NULL), cid);
			}
			else {
				cid = bcid;
			}
			return cid;
		}
	}
	if(name.buf[name.len-1] == ')') {
		knh_index_t loc = knh_bytes_index(name, '(');
		if(loc > 0) {
			knh_type_t r0 = knh_NameSpace_gettype(ctx, ns, knh_bytes_first(name, loc), 0);
			if(CLASS_type(r0) == CLASS_unknown) {
				return CLASS_Closure;
			}
			knh_type_t t1 = TYPE_void, t2 = TYPE_void, t3 = TYPE_void;
			knh_bytes_t nsub = knh_NameSpace_firstType(ctx, ns, knh_bytes_last(name, loc+1), &t1);
			nsub = knh_NameSpace_firstType(ctx, ns, nsub, &t2);
			nsub = knh_NameSpace_firstType(ctx, ns, nsub, &t3);
			knh_class_t cid = knh_class_Closure(ctx, r0, t1, t2, t3);
			if(cid != CLASS_unknown) {
				knh_NameSpace_setcid(ctx, ns, new_String(ctx, name, NULL), cid);
			}
			else {
				cid = CLASS_Closure;
			}
			return cid;
		}
	}
	return CLASS_unknown;
}

///* ------------------------------------------------------------------------ */
//
//KNHAPI(knh_class_t) knh_findcid(Ctx *ctx, knh_bytes_t lname)
//{
//	return knh_NameSpace_findcid(ctx, DP(ctx->kc)->ns, )
//	knh_class_t cid = knh_getcid(ctx, lname);
//	if(cid != CLASS_unknown) {
//		return cid;
//	}
//	if(knh_bytes_endsWith(lname, STEXT("[]"))) {
//		lname.len -= 2;
//		if(knh_bytes_equals(lname, STEXT("byte"))) {
//			return CLASS_Bytes;
//		}
//		cid = knh_getcid(ctx, lname);
//		if(cid == CLASS_unknown || knh_class_isPrivate(cid)) {
//			return CLASS_Array;
//		}
//		return knh_class_Array(ctx, cid);
//	}
//	if(knh_bytes_endsWith(lname, STEXT(".."))) {
//		lname.len -= 2;
//		cid = knh_getcid(ctx, lname);
//		if(cid == CLASS_unknown || knh_class_isPrivate(cid)) {
//			return CLASS_Iterator;
//		}
//		return knh_class_Iterator(ctx, cid);
//	}
//	return cid;
//}

/* ------------------------------------------------------------------------ */

char* knh_Context_CLASSN(Ctx *ctx, knh_class_t cid)
{
	DBG2_ASSERT_cid(cid);
	knh_String_t *name = ClassTable(cid).lname;
	if(knh_String_startsWith(name, STEXT("konoha."))) {
		return __tochar(ClassTable(cid).sname);
	}
	else {
		knh_uintptr_t cid2 =
			knh_DictSet_get__b(DP(ctx->share->mainns)->name2cidDictSet, __tobytes(ClassTable(cid).sname));
		if(cid2 > 0 && cid == cid2 - 1) {
			return __tochar(ClassTable(cid).sname);
		}
		return CLASSN(cid);
	}
}


/* ======================================================================== */
/* [type] */

knh_type_t knh_NameSpace_gettype(Ctx *ctx, knh_NameSpace_t *ns, knh_bytes_t name, int isNullable)
{
	if(name.buf[0] == 'v') {
		if(name.len == 4 && name.buf[1] == 'o' &&
				name.buf[2] == 'i' && name.buf[3] == 'd') return TYPE_void;
		if(name.len == 3 && name.buf[1] == 'a' && name.buf[2] == 'r') {
			return TYPE_var;
		}
	}

	if(name.len > 2 && name.buf[name.len-1] == '!') {
		name.len--;
		isNullable = 0;
	}

	if(name.len > 2 && name.buf[name.len-1] == '?') {
		name.len--;
		isNullable = 1;
	}
	{
		knh_class_t cid = knh_NameSpace_findcid(ctx, ns, name);
		if(cid == CLASS_Any) return cid;
		if(cid != CLASS_unknown && isNullable) return NATYPE_cid(cid);
		return cid;
	}
}


/* ------------------------------------------------------------------------ */
/* [tag] */

knh_type_t knh_NameSpace_tagcid(Ctx *ctx, knh_NameSpace_t *o, knh_class_t cid, knh_bytes_t tag)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_class_t bcid = ClassTable(cid).bcid;
	knh_printf(ctx, cwb->w, "%C:%B", bcid, tag);
	cid = knh_NameSpace_findcid(ctx, o, knh_cwb_tobytes(cwb));
	knh_cwb_close(cwb);
	return cid;
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
