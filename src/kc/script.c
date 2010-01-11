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

/* ------------------------------------------------------------------------ */
/* [namespace] */
/* ------------------------------------------------------------------------ */

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

static
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

static
knh_class_t knh_NameSpace_getcid(Ctx *ctx, knh_NameSpace_t *ns, knh_bytes_t sname)
{
	if(knh_bytes_equals(sname, STEXT("Script"))) {
		return knh_Object_cid(knh_getGammaScript(ctx));
	}
	if(knh_bytes_equals(sname, STEXT("byte[]"))) {
		return CLASS_Bytes;
	}
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

static
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

knh_class_t knh_NameSpace_findcid(Ctx *ctx, knh_NameSpace_t *ns, knh_bytes_t name)
{
	knh_class_t cid, p1;
	cid = knh_NameSpace_getcid(ctx, ns, name);
	if(cid != CLASS_unknown) {
		return cid;
	}

	cid = knh_getcid(ctx, name);
	if(cid != CLASS_unknown) {
		knh_NameSpace_setcid(ctx, ns, ClassTable(cid).sname, cid);
		return cid;
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

/* ------------------------------------------------------------------------ */
/* [script] */
/* ------------------------------------------------------------------------ */

knh_Script_t *new_Script(Ctx *ctx, knh_NameSpace_t *ns, knh_class_t cid, knh_class_t supcid)
{
	size_t i;
	knh_Script_t *script = (knh_Script_t*)new_Object_init(ctx, FLAG_Script, CLASS_Script, 0);
	knh_ClassTable_t *t = (knh_ClassTable_t*)(&ClassTable(cid));
	if(cid != CLASS_Script) {
		char buf[CLASSNAME_BUFSIZ];
		script->h.cid = cid;
		DBG2_ASSERT(ClassTable(cid).cspi == NULL);
		DBG2_ASSERT(t->cmap == NULL);
		knh_snprintf(buf, sizeof(buf), "%s%d.Script", __tochar(DP(ns)->nsname), (int)cid);
		knh_setClassName(ctx, cid, new_String(ctx, B(buf), NULL));
		t->cflag  = CFLAG_Script;
		t->oflag  = FLAG_Script;
		t->bcid   = CLASS_Script; /* CLASS_Object */
		t->supcid = supcid;
		t->offset = 0;
		t->cspi = ClassTable(CLASS_Script).cspi;
		KNH_INITv(t->methods, new_Array0(ctx, 0));
		KNH_INITv(t->cmap, ClassTable(CLASS_Script).cmap);
	}
	DBG2_ASSERT(t->fsize == 0);
	DBG2_ASSERT(t->fields == NULL);
	t->bsize  = KNH_SCRIPT_FIELDSIZE;
	t->size = t->bsize * sizeof(knh_Object_t*);
	t->fsize = KNH_SCRIPT_FIELDSIZE;
	t->fields = KNH_MALLOC(ctx, sizeof(knh_fields_t) * KNH_SCRIPT_FIELDSIZE);
	knh_bzero(t->fields, sizeof(knh_fields_t) * KNH_SCRIPT_FIELDSIZE);
	for(i=0; i < KNH_SCRIPT_FIELDSIZE; i++) {
		t->fields[i].flag  = 0;
		t->fields[i].type  = TYPE_void;
		t->fields[i].fn    = FIELDN_NONAME;
		t->fields[i].value = NULL;
	}
	DBG2_ASSERT(t->cspec == NULL);
	knh_setClassDefaultValue(ctx, cid, UP(script), NULL);
	{
		knh_Method_t *mtd = new_Method(ctx, 0, cid, METHODN_LAMBDA, NULL);
		KNH_SETv(ctx, DP(mtd)->mf, knh_findMethodField0(ctx, TYPE_Any));
		knh_Class_addMethod(ctx, cid, mtd);
	}
	return script;
}

/* ------------------------------------------------------------------------ */
/* [package] */
/* ------------------------------------------------------------------------ */

static char *knh_cwb_packageScript(Ctx *ctx, knh_cwb_t *cwb, knh_bytes_t rootdir, knh_bytes_t package)
{
	knh_cwb_subclear(cwb, 0);
	knh_cwb_write(ctx, cwb, rootdir);
	knh_cwb_putc(ctx, cwb, '/');
	knh_cwb_write(ctx, cwb, package);
	knh_cwb_putc(ctx, cwb, '/');
	knh_index_t loc = knh_bytes_index(package, '.');
	if(loc != -1) {
		package = knh_bytes_first(package, loc);
	}
	knh_cwb_write(ctx, cwb, package);
	knh_cwb_write(ctx, cwb, STEXT(".k"));
	knh_cwb_ospath(ctx, cwb);
	if(knh_cwb_isfile(ctx, cwb)) {
		return knh_cwb_tochar(ctx, cwb);
	}
	return NULL;
}

static char *knh_lookupPackageScript(Ctx *ctx, knh_cwb_t *cwb, knh_bytes_t pkgname)
{
	knh_String_t *spath;
	char *path = knh_getenv("KONOHA_PACKAGE");
	if(path != NULL) {
		path = knh_cwb_packageScript(ctx, cwb, B(path), pkgname);
		if(path != NULL) return path;
	}
	spath = (knh_String_t*)knh_getProperty(ctx, STEXT("konoha.package.path"));
	if(IS_bString(spath)) {
		path = knh_cwb_packageScript(ctx, cwb, __tobytes(spath), pkgname);
		if(path != NULL) return path;
	}
	spath = (knh_String_t*)knh_getProperty(ctx, STEXT("user.package.path"));
	if(IS_bString(spath)){
		path = knh_cwb_packageScript(ctx, cwb, __tobytes(spath), pkgname);
		if(path != NULL) return path;
	}
	return NULL;
}

static knh_Package_t *new_Package(Ctx *ctx, knh_String_t *nsname)
{
	knh_Package_t *pkg = (knh_Package_t*)new_Object_init(ctx, FLAG_Package, CLASS_Package, 0);
	KNH_SETv(ctx, DP(pkg)->name, nsname);
	return pkg;
}

static int knh_loadPackage(Ctx *ctx, knh_bytes_t pkgname)
{
	int res = 1;
	knh_Package_t *pkg = (knh_Package_t*)knh_DictMap_get__b(ctx, DP(ctx->sys)->PackageDictMap, pkgname);
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_InputStream_t *in = NULL;
	knh_sfp_t *lsfp = KNH_LOCAL(ctx);
	if(IS_NULL(pkg)) {
		knh_uri_t uri = knh_getResourceId(ctx, pkgname);
		knh_String_t *nsname = knh_getResourceName(ctx, uri);
		char *fpath = knh_lookupPackageScript(ctx, cwb, __tobytes(nsname));
		if(fpath == NULL) {
			KNH_SYSLOG0(ctx, LOG_NOTICE, "package not found: %s", __tochar(nsname));
			goto L_RETURN;
		}
		else {
			KNH_SYSLOG0(ctx, LOG_INFO, "package: %s, %s", __tochar(nsname), fpath);
		}
		pkg = new_Package(ctx, nsname);
		knh_DictMap_set(ctx, DP(ctx->sys)->PackageDictMap, nsname, UP(pkg));
		in = new_ScriptInputStream(ctx, knh_cwb_tobytes(cwb), cwb, 0);
		KNH_LPUSH(ctx, in);
	}
	if(!knh_InputStream_isClosed(ctx, in)) {
		knh_Gamma_t *gamma = ctx->kc;
		knh_NameSpace_t *ns = new_NameSpace(ctx, NULL);
		knh_NameSpace_t *oldns = DP(gamma)->ns;
		knh_Script_t *oldscr = DP(gamma)->script;
		KNH_LPUSH(ctx, ns);
		KNH_SETv(ctx, DP(gamma)->ns, ns);
		KNH_SETv(ctx, DP(ns)->nsname, DP(pkg)->name);
		KNH_SETv(ctx, DP(gamma)->script, new_Script(ctx, ns, new_ClassId(ctx), CLASS_Script));
		KNH_SETv(ctx, DP(pkg)->script, DP(gamma)->script);
		knh_Script_loadStream(ctx, DP(pkg)->script, in, 1/*isEval*/,0/*isThrowable*/);
		KNH_SETv(ctx, DP(gamma)->ns, oldns);
		KNH_SETv(ctx, DP(gamma)->script, oldscr);
	}
	L_RETURN:
	KNH_LOCALBACK(ctx, lsfp);
	knh_cwb_close(cwb);
	return res;
}

/* ------------------------------------------------------------------------ */
/* interpreter */
/* ------------------------------------------------------------------------ */

knh_flag_t knh_Stmt_flag(Ctx *ctx, knh_Stmt_t *stmt, knh_bytes_t name, knh_flag_t flag)
{
	Object *v = knh_DictMap_get__b(ctx, DP(stmt)->metaDictMap, name);
	return (IS_NOTNULL(v)) ? flag : 0;
}

/* ------------------------------------------------------------------------ */
/* [CLASS] */

static knh_flag_t knh_StmtCLASS_flag(Ctx *ctx, knh_Stmt_t *o)
{
	knh_flag_t flag = 0;
	if(IS_DictMap(DP(o)->metaDictMap)) {
		flag |= knh_Stmt_flag(ctx, o, STEXT("Final"), FLAG_Class_Final);
		flag |= knh_Stmt_flag(ctx, o, STEXT("Private"), FLAG_Class_Private);
		flag |= knh_Stmt_flag(ctx, o, STEXT("Interface"), FLAG_Class_Interface);
		flag |= knh_Stmt_flag(ctx, o, STEXT("Singleton"), FLAG_Class_Singleton);
		flag |= knh_Stmt_flag(ctx, o, STEXT("Release"), FLAG_Class_Release);
	}
	return flag;
}

/* ------------------------------------------------------------------------ */
/* [implements] */

static
knh_Mapper_t* new_Mapper__interface(Ctx *ctx, knh_class_t scid, knh_class_t tcid)
{
	return new_Mapper(ctx, FLAG_Mapper_Interface, scid, tcid, knh_Fmapper_asis, KNH_NULL);
}

/* ------------------------------------------------------------------------ */

static
void knh_class_addInterface(Ctx *ctx, knh_class_t cid, knh_class_t icid)
{
	KNH_ASSERT(!knh_class_isInterface(icid));
	if(knh_class_instanceof(ctx, cid, icid)) {
		return ;
	}
	else {
		knh_class_t isupcid = icid;
		int allchecked = 1;
		while(isupcid != CLASS_Object) {
			knh_ClassTable_t *t = pClassTable(ctx, isupcid);
			size_t i;
			for(i = 0; i < knh_Array_size(t->methods); i++) {
				knh_Method_t *imtd = (knh_Method_t*)knh_Array_n(t->methods, i);
				if(knh_Method_isPrivate(imtd)) {
					continue;
				}
				else {
					knh_Method_t *mtd = knh_Class_getMethod(ctx, cid, DP(imtd)->mn);
					if(IS_NOTNULL(mtd)) {
						if(!knh_MethodField_equalsType(DP(mtd)->mf, DP(imtd)->mf)) {
							DBG2_P("mismatch!! %s.%s", CLASSN(DP(imtd)->cid), METHODN(DP(imtd)->mn));
							allchecked = 0;
						}
					}
				}
			}
			isupcid = ctx->share->ClassTable[isupcid].supcid;
		}
		if(allchecked) {
			isupcid = icid;
			while(isupcid != CLASS_Object) {
				knh_ClassTable_t *t = pClassTable(ctx, isupcid);
				size_t i;
				for(i = 0; i < knh_Array_size(t->methods); i++) {
					knh_Method_t *imtd = (knh_Method_t*)knh_Array_n(t->methods, i);
					if(knh_Method_isPrivate(imtd)) {
						continue;
					}
					else {
						knh_Method_t *mtd = knh_Class_getMethod(ctx, cid, DP(imtd)->mn);
						if(IS_NULL(mtd)) {
							mtd = new_Method(ctx, 0, cid, DP(imtd)->mn, NULL);
							KNH_SETv(ctx, DP(mtd)->mf, DP(imtd)->mf);
							KNH_ASSERT(IS_NOTNULL(DP(mtd)->mf));
							knh_Class_addMethod(ctx, cid, mtd);
						}
					}
				}
				isupcid = ctx->share->ClassTable[isupcid].supcid;
			}
			DBG2_P("add interface %s to %s", CLASSN(icid), CLASSN(cid));
			knh_ClassMap_add(ctx, ClassTable(cid).cmap, new_Mapper__interface(ctx, cid, icid));
		}
	}
}

/* ------------------------------------------------------------------------ */

static int knh_StmtCLASS_decl(Ctx *ctx, knh_Stmt_t *stmt)
{
	knh_NameSpace_t *ns = knh_getGammaNameSpace(ctx);
	char bufn[CLASSNAME_BUFSIZ];
	knh_snprintf(bufn, sizeof(bufn), "%s.%s", __tochar(DP(ns)->nsname), sToken(StmtCLASS_class(stmt)));
	knh_class_t cid  = knh_NameSpace_findcid(ctx, ns, B(bufn));
	if(cid == CLASS_unknown) {
		cid = new_ClassId(ctx);
		KNH_ASSERT(ClassTable(cid).sname == NULL);
	}
	else {
		knh_Gamma_perror(ctx, KERR_ERROR, _("re-definition of %C"), cid);
		knh_Stmt_done(ctx, stmt);
		return 0;
	}

	knh_class_t supcid = CLASS_Object;
	if(SP(StmtCLASS_superclass(stmt))->tt != TT_ASIS) {
		supcid = knh_NameSpace_findcid(ctx, ns, knh_Token_tobytes(ctx, StmtCLASS_superclass(stmt)));
		if(supcid == CLASS_unknown) {
			supcid = CLASS_Object;
			knh_Gamma_perror(ctx, KERR_ERRATA, _("unknown class: %s ==> %C"), sToken(StmtCLASS_superclass(stmt)), supcid);
		}
		else if(knh_class_isFinal(supcid)) {
			knh_Gamma_perror(ctx, KERR_ERROR, _("cannot extends %C: this class is final"), supcid);
			knh_Stmt_done(ctx, stmt);
			return 0;
		}
	}

	DP(StmtCLASS_class(stmt))->cid = cid;
	{
		knh_ClassTable_t *t = pClassTable(ctx, cid);
		t->cflag  = knh_StmtCLASS_flag(ctx, stmt);
		t->oflag  = FLAG_oflag(t->cflag);
		if(SP(StmtCLASS_instmt(stmt))->stt == STT_DONE) {
			t->bcid   = CLASS_Any;    /* Glue */
		}
		else {
			t->bcid   = CLASS_Object;
		}
		t->supcid = supcid;
		t->offset = 0; /* will be extended in CLASS_typing */
		DBG2_ASSERT_cid(supcid);
		t->keyidx = ClassTable(supcid).keyidx;
		DBG2_ASSERT(t->sname == NULL);
		knh_setClassName(ctx, cid, new_String(ctx, B(bufn), NULL));
		DBG2_ASSERT(t->fields == NULL);
		DBG2_ASSERT(t->fsize  == 0);
		KNH_INITv(t->methods, new_Array0(ctx, 0));
		KNH_INITv(t->cmap, ctx->share->ClassTable[CLASS_Any].cmap);
		knh_setClassDefaultValue(ctx, cid, new_hObject(ctx, t->oflag | FLAG_Object_Undefined, t->bcid, cid), NULL);
		if(t->bcid == CLASS_Any) {
			knh_Glue_init(ctx, (knh_Glue_t*)t->cspec, NULL, NULL);
		}
		knh_NameSpace_setcid(ctx, ns, t->sname, cid);
		if(SP(StmtCLASS_interface(stmt))->stt != STT_DONE) {
			knh_Stmt_t *istmt = StmtCLASS_interface(stmt);
			int i, n = DP(istmt)->size;
			for(i = 0; i < n; i++) {
				knh_Token_t *tk = DP(stmt)->tokens[i];
				knh_class_t icid = knh_NameSpace_findcid(ctx, ns, knh_Token_tobytes(ctx, tk));
				if(icid == CLASS_unknown) {
					knh_Token_perror(ctx, tk, KERR_EWARN, _("unknown class: %s"), sToken(tk));
					continue;
				}
				if(!knh_class_isInterface(icid)) {
					knh_Token_perror(ctx, tk, KERR_EWARN, _("cannot implements %C: this class is not @Interface"), icid);
					continue;
				}
				knh_class_addInterface(ctx, cid, icid);
			}
		}
		if(SP(StmtCLASS_instmt(stmt))->stt == STT_DONE) {
			knh_Stmt_done(ctx, stmt);
		}
		KNH_NOTICE(ctx, "added new class: %s", CLASSN(cid));
	}
	return 1;
}

/* ------------------------------------------------------------------------ */

static int knh_StmtINCLUDE_decl(Ctx *ctx, knh_Stmt_t *stmt)
{
	int res = 1;
	knh_bytes_t path = __tobytes(DP(StmtINCLUDE_file(stmt))->text);
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	if(path.buf[0] != '/' && path.buf[0] != '\\' && !knh_bytes_startsWith(path, STEXT("http://"))) {
		knh_bytes_t t = __tobytes(knh_getResourceName(ctx, SP(stmt)->uri));
		if(t.buf[0] != '(') {  /* NOT (eval) */
			knh_cwb_write(ctx, cwb, t);
			knh_bool_t ret = knh_cwb_parentpath(ctx, cwb, NULL);
			if (ret) {
			  knh_cwb_putc(ctx, cwb, '/');
			}
		}
	}
	knh_cwb_write(ctx, cwb, path);
	{
		knh_sfp_t *lsfp = KNH_LOCAL(ctx);
		knh_InputStream_t *in = new_ScriptInputStream(ctx, path, cwb, 0);
		KNH_LPUSH(ctx, in);
		if(!knh_InputStream_isClosed(ctx, in)) {
			res = knh_Script_loadStream(ctx, ctx->script, in, 1/*isEval*/,0/*isThrowable*/);
		}
		else {
			res = 0;
		}
		KNH_LOCALBACK(ctx, lsfp);
	}
	knh_cwb_close(cwb);
	return res;
}

/* ------------------------------------------------------------------------ */

static int knh_StmtUCLASS_decl(Ctx *ctx, knh_Stmt_t *stmt)
{
	knh_NameSpace_t *ns = knh_getGammaNameSpace(ctx);
	knh_Token_t *tk = DP(stmt)->tokens[0];
	knh_bytes_t name = knh_Token_tobytes(ctx, tk);
	knh_index_t loc = knh_bytes_rindex(name, '.');
	if(loc != -1 && isupper(name.buf[loc+1])) { /* using math.Math */
		if(knh_loadPackage(ctx, knh_bytes_first(name, loc))) {
			knh_class_t newcid = knh_getcid(ctx, name);
			if(newcid == CLASS_unknown) {
				knh_Token_perror(ctx, tk, KERR_ERROR, _("unknown class: %s"), sToken(tk));
				return 0;
			}
//			knh_class_t oldcid = knh_NameSpace_getcid(ctx, ns, knh_bytes_last(name, loc+1));
//			if(oldcid != CLASS_unknown && newcid != oldcid) {
//				KNH_WARNING(ctx, _("ovrriding.. %s => %s"), CLASSN(oldcid), CLASSN(newcid));
//			}
			knh_NameSpace_setcid(ctx, ns, ClassTable(newcid).sname, newcid);
			return 1;
		}
	}else {
		if(knh_loadPackage(ctx, name)) {
			if(knh_Token_isTailWildCard(tk)) {
				knh_NameSpace_useClass(ctx, ns, name);
			}
			return 1;
		}
	}
	knh_Token_perror(ctx, tk, KERR_ERROR, _("package not found: %s"), sToken(tk));
	return 0;
}

/* ------------------------------------------------------------------------ */

static int knh_StmtUALIAS_decl(Ctx *ctx, knh_Stmt_t *stmt)
{
	DBG2_DUMP(ctx, stmt, KNH_NULL, "decl");
	TODO();
	return 1;
}

///* ------------------------------------------------------------------------ */
//
//static
//char *knh_format_classurn(Ctx *ctx, char *buf, size_t bufsiz, knh_class_t bcid, knh_bytes_t urn)
//{
//	knh_snprintf(buf, bufsiz, KNH_SEMANTICS_FMT, CLASSN(bcid), urn.buf);
//	return buf;
//}
//
///* ------------------------------------------------------------------------ */
//
//static
//void knh_NameSpace_setTagName(Ctx *ctx, knh_NameSpace_t *o, knh_String_t *name, knh_class_t cid)
//{
//	KNH_ASSERT(IS_NameSpace(o));
//	DBG2_ASSERT_cid(cid);
//	knh_DictSet_set(ctx, DP(o)->name2cidDictSet, name, (knh_uintptr_t)(cid+1));
////	knh_bytes_t n = __tobytes(name);
////	knh_index_t loc = knh_bytes_index(n, ':');
////	if(loc != -1) {
////		n = knh_bytes_last(n, loc+1);
////		if(isupper(n.buf[0])) {
////			knh_DictSet_set(ctx, DP(o)->name2cidDictSet, new_String(ctx, n, name), (knh_uintptr_t)(cid+1));
////		}
////	}
//}

/* ------------------------------------------------------------------------ */

static int knh_StmtXCLASS_decl(Ctx *ctx, knh_Stmt_t *stmt, knh_class_t bcid)
{
//
//	knh_NameSpace_t *ns = knh_getGammaNameSpace(ctx);
//	knh_Token_t *tkclassn = DP(stmt)->tokens[0];
//	knh_Token_t *tkurn = DP(stmt)->tokens[1];
//
//	char bufcu[CLASSNAME_BUFSIZ];
//	knh_format_classurn(ctx, bufcu, sizeof(bufcu), bcid, knh_getURNAlias(ctx, knh_Token_tobytes(ctx, tkurn)));
//
//	knh_class_t cid = knh_findcid(ctx, B(bufcu));
//	if(cid == bcid || cid == CLASS_unknown) {
//		knh_Token_perror(ctx, tkurn, KERR_EWARN, _("resource not found: %s"), sToken(tkurn));
//		cid = bcid;
//	}
//
//	knh_class_t oldcid = knh_NameSpace_findcid(ctx, ns, knh_Token_tobytes(ctx, tkclassn));
//	if(oldcid != CLASS_unknown && cid != oldcid) {
//		KNH_WARNING(ctx, _("overriding %s"), knh_Token_tobytes(ctx, tkclassn));
//	}
//
//	KNH_ASSERT(IS_String(DP(tkclassn)->data));
//	knh_NameSpace_setTagName(ctx, ns, DP(tkclassn)->text, cid);
	TODO();
	return 1;
}

/* ------------------------------------------------------------------------ */

static int knh_StmtUVOCAB_decl(Ctx *ctx, knh_Stmt_t *stmt)
{
	return knh_StmtXCLASS_decl(ctx, stmt, CLASS_String);
}

/* ------------------------------------------------------------------------ */

static int knh_StmtUENUM_decl(Ctx *ctx, knh_Stmt_t *stmt)
{
	return knh_StmtXCLASS_decl(ctx, stmt, CLASS_Int);
}

/* ------------------------------------------------------------------------ */

static int knh_StmtUUNIT_decl(Ctx *ctx, knh_Stmt_t *stmt)
{
	return knh_StmtXCLASS_decl(ctx, stmt, CLASS_Float);
}

/* ------------------------------------------------------------------------ */

static int knh_StmtUFUNC_decl(Ctx *ctx, knh_Stmt_t *stmt)
{
	knh_NameSpace_t *ns = knh_getGammaNameSpace(ctx);
	knh_Token_t *tk = DP(stmt)->tokens[0];
	knh_bytes_t name = knh_Token_tobytes(ctx, tk);
	if(SP(tk)->tt == TT_CMETHODN) {
		knh_index_t loc = knh_bytes_rindex(name, '.');
		knh_class_t cid = knh_NameSpace_findcid(ctx, ns, knh_bytes_first(name, loc));
		if(cid == CLASS_unknown) {
			knh_Token_perror(ctx, tk, KERR_ERROR, _("unknown class: %s"), sToken(tk));
			return 0;
		}
		knh_methodn_t mn = knh_getmn(ctx, knh_bytes_last(name, loc+1), METHODN_NONAME);
		if(mn == METHODN_NONAME) {
			knh_Token_perror(ctx, tk, KERR_ERROR, _("unknown method: %C.%s"), cid, sToken(tk));
			return 0;
		}
		knh_NameSpace_setFuncClass(ctx, ns, mn, cid);
	}
	else {
		knh_class_t cid = knh_NameSpace_findcid(ctx, ns, name);
		if(cid == CLASS_unknown) {
			knh_Token_perror(ctx, tk, KERR_ERROR, _("unknown class: %s"), sToken(tk));
			return 0;
		}
		else {
			DBG2_ASSERT_cid(cid);
			knh_Array_t *a = ClassTable(cid).methods;
			size_t i;
			for(i = 0; i < knh_Array_size(a); i++) {
				knh_Method_t *mtd = (knh_Method_t*)knh_Array_n(a, i);
				KNH_ASSERT(IS_Method(mtd));
				if(!knh_Method_isStatic(mtd)) continue;
				knh_NameSpace_setFuncClass(ctx, ns, DP(mtd)->mn, cid);
				if(knh_Context_isVerbose(ctx)) {
					KNH_NOTICE(ctx, _("using %C.%M"), cid, DP(mtd)->mn);
				}
			}
		}
	}
	return 1;
}

/* ------------------------------------------------------------------------ */

static int knh_StmtUMAPMAP_decl(Ctx *ctx, knh_Stmt_t *stmt)
{
	DBG2_DUMP(ctx, stmt, KNH_NULL, "decl");
	return 1;
}

/* ------------------------------------------------------------------------ */

static int knh_Stmt_eval(Ctx *ctx, knh_Stmt_t *stmt, int isEval)
{
	int res = 0;
	knh_sfp_t *lsfp = KNH_LOCAL(ctx);
	knh_Script_t *scr = knh_getGammaScript(ctx);
	knh_Method_t *mtd = knh_Class_getMethod(ctx, knh_Object_cid(scr), METHODN_LAMBDA);
	int isExpr = knh_stmt_isExpr(SP(stmt)->stt);
	knh_methodn_t mt = METHODN__k;

	if(SP(stmt)->stt == STT_MT) {
		knh_Token_t *tk0 = DP(stmt)->tokens[0];
		knh_methodn_t mn = knh_getmn(ctx, knh_Token_tobytes(ctx, tk0), METHODN_NEWID);
		char *name = FIELDN(METHODN_TOFIELDN(mn));
		if(name[1] != 0) mt = METHODN__s;
	}

	if(isExpr) {
		if(SP(stmt)->stt == STT_CALL1) {
			knh_Token_t *tk0 = DP(stmt)->tokens[0];
			if(SP(tk0)->tt == TT_STR) {
				if(isEval) {
					knh_write(ctx, KNH_STDOUT, __tobytes(DP(tk0)->text));
					knh_write_EOL(ctx, KNH_STDOUT);
				}
				return 1;
			}
			if(SP(tk0)->tt == TT_ESTR) {
				mt = METHODN__s;
			}
			SP(stmt)->stt = STT_RETURN;
		}
//		else if(SP(stmt)->stt == STT_LET) {
//			isExpr = 0;
//		}
		else {
			knh_Stmt_t *newstmt = new_Stmt(ctx, 0, STT_RETURN);
			knh_Stmt_add(ctx, newstmt, UP(stmt));
			stmt = newstmt;
			KNH_LPUSH(ctx, stmt);
		}
	}

	if(STT_(stmt) == STT_ERR) goto L_RETURN;
	KNH_ASM_METHOD(ctx, mtd, NULL, stmt, 0 /* isIteration */);
	if(knh_Method_isAbstract(mtd) || SP(stmt)->stt == STT_ERR) {
		goto L_RETURN;
	}

	int isVOID = knh_Stmt_isVOID(stmt);
	if(isEval) {
		knh_ExceptionHandler_t *hdr = new_ExceptionHandler(ctx);
		KNH_MOV(ctx, lsfp[0].o, hdr);
		KNH_TRY(ctx, L_CATCH, lsfp, 0);
		{
			KNH_MOV(ctx, lsfp[1].o, DP(mtd)->code); // TO AVOID RCGC
			KNH_MOV(ctx, lsfp[3].o, scr);
			KNH_SCALL(ctx, lsfp, 2, mtd, 0/*args*/);
			if(isExpr && !isVOID) {
				//DBG2_P("returning %p %s %lld %ld", lsfp+2, CLASSNo(lsfp[2].o), lsfp[2].ivalue, lsfp[2].bvalue);
				knh_stack_w(ctx, lsfp+3, lsfp+2, mt, KNH_STDOUT, UP(TS_EMPTY));
				knh_write_EOL(ctx, KNH_STDOUT);
			}
		}
		KNH_SETv(ctx, lsfp[0].o, KNH_NULL);
		res = 1;
		goto L_RETURN;

		/* catch */
		L_CATCH:;
		KNH_PRINT_STACKTRACE(ctx, lsfp, 0);
	}
	L_RETURN:
	KNH_LOCALBACK(ctx, lsfp);
	return res;
}

static void knh_Gamma_initThisScript(Ctx *ctx)
{
	knh_Gamma_initThis(ctx, knh_Object_cid(knh_getGammaScript(ctx)));
	DP(ctx->kc)->scope = SCOPE_SCRIPT;
}

static int knh_interpret(Ctx *ctx, knh_Stmt_t *stmt, int isEval)
{
	int res = 1;
	knh_sfp_t *lsfp = KNH_LOCAL(ctx);
	knh_Gamma_t *gamma = ctx->kc;
	knh_Stmt_t *cur = NULL;

	KNH_LPUSH(ctx, stmt);
	SP(gamma)->uri = SP(stmt)->uri;

	cur = stmt;
	while(IS_Stmt(cur)) {
		SP(gamma)->line = SP(cur)->line;
		DP(gamma)->scope = SCOPE_SCRIPT;
		switch(STT_(cur)) {
		case STT_NAMESPACE:
		{
			knh_NameSpace_t *ns = new_NameSpace(ctx, knh_getGammaNameSpace(ctx));
			KNH_SETv(ctx, DP(gamma)->ns, ns);
			res = knh_interpret(ctx, DP(cur)->stmts[0], isEval);
			KNH_SETv(ctx, DP(gamma)->ns, DP(ns)->parent);
			knh_Stmt_done(ctx, cur);
			break;
		}
		case STT_SCRIPT:
		{
			knh_Script_t *pscript = knh_getGammaScript(ctx);
			knh_Script_t *script = new_Script(ctx, DP(gamma)->ns, new_ClassId(ctx), knh_Object_cid(pscript));
			KNH_SETv(ctx, DP(gamma)->script, script);
			res = knh_interpret(ctx, DP(cur)->stmts[0], isEval);
			KNH_SETv(ctx, DP(gamma)->script, pscript);
			knh_Stmt_done(ctx, cur);
			break;
		}
		case STT_IF: /* Conditional Compilation */
		if(knh_Stmt_flag(ctx, cur, STEXT("@Script"), 1)) {
			knh_Gamma_initThisScript(ctx);
			cur = knh_StmtIF_decl(ctx, cur);
			res = 0;
			if(cur != NULL) {
				res = knh_interpret(ctx, cur, isEval);
			}
			knh_Stmt_done(ctx, cur);
			break;
		}
		case STT_CLASS:
			res = knh_StmtCLASS_decl(ctx, cur);
			break;
		case STT_INCLUDE:
			res = knh_StmtINCLUDE_decl(ctx, cur);
			knh_Stmt_done(ctx, cur);
			break;
		case STT_UCLASS:
			res = knh_StmtUCLASS_decl(ctx, cur);
			knh_Stmt_done(ctx, cur);
			break;
		case STT_UALIAS:
			res = knh_StmtUALIAS_decl(ctx, cur);
			knh_Stmt_done(ctx, cur);
			break;
		case STT_UVOCAB:
			res = knh_StmtUVOCAB_decl(ctx, cur);
			knh_Stmt_done(ctx, cur);
			break;
		case STT_UENUM:
			res = knh_StmtUENUM_decl(ctx, cur);
			knh_Stmt_done(ctx, cur);
			break;
		case STT_UUNIT:
			res = knh_StmtUUNIT_decl(ctx, cur);
			knh_Stmt_done(ctx, cur);
			break;
		case STT_UFUNC:
			res = knh_StmtUFUNC_decl(ctx, cur);
			knh_Stmt_done(ctx, cur);
			break;
		case STT_UMAPMAP:
			res = knh_StmtUMAPMAP_decl(ctx, cur);
			knh_Stmt_done(ctx, cur);
			break;
		case STT_RETURN:
			knh_Gamma_perror(ctx, KERR_ERRATA, _("'break' is much better"));
		case STT_BREAK:
			res = 0;
			goto L_BREAK;
		}
		if(res == 0) {
			goto L_BREAK;
		}
		cur = DP(cur)->next;
	}

	cur = stmt;
	while(IS_Stmt(cur)) {
		knh_stmt_t stt = SP(cur)->stt;
		SP(gamma)->line = SP(cur)->line;
		DP(gamma)->scope = SCOPE_SCRIPT;
		Term *tm = TM(cur);
		switch(stt) {
		case STT_CLASS:
			tm = knh_StmtCLASS_typing(ctx, cur);
			break;
		case STT_METHOD:
			tm = knh_StmtMETHOD_typing(ctx, cur);
			break;
//		case STT_FORMAT:
//			tm = knh_StmtFORMAT_typing(ctx, cur);
//			break;
		case STT_DECL: {
				knh_Gamma_initThisScript(ctx);
				tm = knh_StmtDECL_typing(ctx, cur);
				break;
			}
		case STT_LET: {
				knh_Gamma_initThisScript(ctx);
				tm = knh_StmtLET_typing(ctx, cur, TYPE_void);
				break;
			}
		case STT_LETM: {
				knh_Gamma_initThisScript(ctx);
				tm = knh_StmtLETM_typing(ctx, cur);
				break;
			}
		case STT_SEPARATOR: {
				knh_Gamma_initThisScript(ctx);
				tm = knh_StmtSEPARATOR_typing(ctx, cur);
				break;
			}
		case STT_FUNCTION: {
				knh_Gamma_initThisScript(ctx);
				tm = knh_StmtFUNCTION_typing(ctx, cur, TYPE_void);
				break;
			}
		}
		if(tm == NULL) {
			res = 0;
			goto L_BREAK;
		}
		cur = DP(cur)->next;
	}

	if(DP(gamma)->dlhdr != NULL) {
		knh_Fpkginit fpkginit = (knh_Fpkginit)knh_dlsym(ctx, DP(gamma)->dlhdr, "knh_pkginit");
		if(fpkginit != NULL) {
			knh_PackageData_t *pkgdata = fpkginit();
			if(pkgdata->setup != NULL) {
				DBG_P("loading %s-%s", __tochar(DP(DP(gamma)->ns)->nsname), pkgdata->version);
				pkgdata->setup(ctx);
			}
		}
		else {
			knh_Fscriptinit finit = (knh_Fscriptinit)knh_dlsym(ctx, DP(gamma)->dlhdr, "init");
			if(finit != NULL) {
				finit(ctx);
			}
		}
		//DP(kc)->dlhdr = NULL;
	}
	cur = stmt;
	while(IS_Stmt(cur)) {
		knh_stmt_t stt = SP(cur)->stt;
		DP(gamma)->scope = SCOPE_SCRIPT;
		switch(stt) {
		case STT_CLASS:
			knh_StmtCLASS_asm(ctx, cur); break;
		case STT_METHOD:
			knh_StmtMETHOD_asm(ctx, cur); break;
		case STT_FORMAT:
			knh_StmtFORMAT_asm(ctx, cur); break;
		case STT_DONE:
			break;
		default:
			{
				res = knh_Stmt_eval(ctx, cur, isEval);
				if(res == 0) goto L_BREAK;
			}
		}
		knh_Stmt_done(ctx, cur);
		cur = DP(cur)->next;
	}
	res = 1;
	L_BREAK:;
	KNH_LOCALBACK(ctx, lsfp);
	return res;
}

/* ======================================================================== */
/* [Data] */

int knh_bytes_checkStmtLine(knh_bytes_t line)
{
	char *ln = (char*)line.buf;
	size_t i = 0, len = line.len;
	int ch, quote = 0, nest =0;
	L_NORMAL:
	for(; i < len; i++) {
		ch = ln[i];
		if(ch == '{' || ch == '[' || ch == '(') nest++;
		if(ch == '}' || ch == ']' || ch == ')') nest--;
		if(ch == '\'' || ch == '"' || ch == '`') {
			quote = ch; i++;
			goto L_QUOTE;
		}
	}
	return nest;

	L_QUOTE:
	DBG2_ASSERT(i > 0);
	for(; i < len; i++) {
		ch = ln[i];
		if(ln[i-1] != '\\' && ch == quote) {
			i++;
			goto L_NORMAL;
		}
	}
	return 1;
}

/* ------------------------------------------------------------------------ */

static
Object *knh_InputStream_parseDataNULL(Ctx *ctx, knh_InputStream_t *in)
{
	knh_sfp_t *lsfp = KNH_LOCAL(ctx);
	Object *rVALUE = NULL;
	knh_Stmt_t *stmt = knh_InputStream_parseStmt(ctx, in, 1/*isData*/);
	KNH_LPUSH(ctx, in); //lsfp[0]
	KNH_LPUSH(ctx, stmt); // lsfp[1]
	if(knh_stmt_isExpr(STT_(stmt)) && STT_(stmt) != STT_LET) {
		knh_Script_t *scr = knh_getGammaScript(ctx);
		knh_Method_t *mtd = knh_Class_getMethod(ctx, knh_Object_cid(scr), METHODN_LAMBDA);
		KNH_ASM_METHOD(ctx, mtd, NULL, stmt, 0 /* isIteration */);
		if(knh_Method_isAbstract(mtd) || SP(stmt)->stt == STT_ERR) {
			goto L_RETURN;
		}
		DBG_P("rtype=%s%s", TYPEQN(DP(stmt)->type));
		KNH_MOV(ctx, lsfp[1].o, DP(mtd)->code); // TO AVOID RCGC
		KNH_MOV(ctx, lsfp[2].o, scr);
		KNH_SCALL(ctx, lsfp, 2, mtd, 0/*args*/);
		rVALUE = lsfp[4].o;
	}
	L_RETURN:
	KNH_LOCALBACK(ctx, lsfp);
	return rVALUE;
}

/* ------------------------------------------------------------------------ */

Object* knh_InputStream_readData(Ctx *ctx, knh_InputStream_t *in)
{
	int ch, linenum = DP(in)->line;
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	L_READLINE:;
	int quote = 0, prev = 0;
	knh_cwb_subclear(cwb, 0);
	while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
		if(ch == ';' && quote == 0) {
			if(knh_cwb_size(cwb) > 0) {
				goto L_STMT;
			}
		}
		knh_Bytes_putc(ctx, cwb->ba, ch);
		if(quote == ch && prev != '\\') {
			quote = 0;
			continue;
		}
		if(quote == 0) {
			if(ch == '"' || ch == '\'' || ch == '`') {
				quote = ch;
			}
			if(ch == '#') {
				quote = '\n';
			}
		}
	}
	return KNH_NULL;
	L_STMT:;
	{
		knh_InputStream_t *bin = new_BytesInputStream(ctx, cwb->ba, cwb->pos, knh_Bytes_size(cwb->ba));
		Object *value = NULL;
		DP(bin)->uri = DP(in)->uri;
		DP(bin)->line = linenum;
		knh_InputStream_setEncoding(ctx, bin, DP(in)->enc);
		value = knh_InputStream_parseDataNULL(ctx, bin);
		knh_cwb_close(cwb);
		if(value == NULL) goto L_READLINE;
		return value;
	}
}

/* ------------------------------------------------------------------------ */
/* [Extension] */

//static
//knh_Array_t* knh_DictMap_makeArray(Ctx *ctx, knh_DictMap_t *o, knh_bytes_t key)
//{
//	knh_index_t idx = knh_DictMap_firstIndex(o, key);
//	if(idx != -1) {
//		knh_Array_t *a = new_Array0(ctx, 0);
//		int i;
//		for(i = idx; i < knh_DictMap_size(o); i++) {
//			knh_String_t *k = knh_DictMap_keyAt(o, i);
//			Object *v = knh_DictMap_valueAt(o, i);
//			if(knh_String_startsWith(k, key) && IS_NOTNULL(v)) {
//				knh_Array_add(ctx, a, UP(k));
//				knh_Array_add(ctx, a, v);
//			}
//		}
//		return a;
//	}
//	return (knh_Array_t*)KNH_NULL;
//}
//
///* ------------------------------------------------------------------------ */
//
//static
//void knh_invokeMethodListener(Ctx *ctx, knh_DictMap_t *meta, knh_Method_t *mtd, knh_bytes_t key1, knh_bytes_t key2)
//{
//	knh_Closure_t *cc;
//	KNH_LOCK(ctx, LOCK_SYSTBL, NULL);
//	{
//		knh_DictMap_t *dm = DP(ctx->sys)->listenerDictMap;
//		cc = (knh_Closure_t*)knh_DictMap_get__b(ctx, dm, key1);
//	}
//	KNH_UNLOCK(ctx, LOCK_SYSTBL, NULL);
//	if(IS_NOTNULL(cc)) {
//		knh_sfp_t *lsfp = KNH_LOCAL(ctx);
//		if(IS_NOTNULL(meta)) {
//			KNH_SETv(ctx, lsfp[2].o, meta);   /* cc(DictMap, Method) */
//			KNH_SETv(ctx, lsfp[3].o, mtd);
//			knh_Closure_invokesfp(ctx, cc, lsfp, /*argc*/2);
//		}
//		else {
//			knh_DictMap_t *dmeta = (knh_DictMap_t*)KNH_DEF(ctx, CLASS_DictMap);
//			KNH_SETv(ctx, lsfp[2].o, dmeta);
//			KNH_SETv(ctx, lsfp[3].o, mtd);
//			knh_Closure_invokesfp(ctx, cc, lsfp, /*argc*/2);
//			knh_DictMap_clear(ctx, dmeta);
//		}
//	}
//	if(IS_NOTNULL(meta)) {
//		int i;
//		knh_Array_t *a;
//		knh_sfp_t *lsfp = KNH_LOCAL(ctx);
//		KNH_LOCK(ctx, LOCK_SYSTBL, NULL);
//		{
//			knh_DictMap_t *dm = DP(ctx->sys)->listenerDictMap;
//			a = knh_DictMap_makeArray(ctx, dm, key2);
//		}
//		KNH_UNLOCK(ctx, LOCK_SYSTBL, NULL);
//		if(IS_NULL(a)) return;
//		KNH_LPUSH(ctx, a); // TO AVOID GC;
//		for(i = 0; i < knh_Array_size(a); i+=2) {
//			knh_String_t *key = (knh_String_t*)knh_Array_n(a, i);
//			knh_bytes_t anno = knh_bytes_rmod(__tobytes(key), '@');
//			if(IS_NOTNULL(knh_DictMap_get__b(ctx, meta, anno))) {
//				cc = (knh_Closure_t*)knh_Array_n(a, i+1);
//				KNH_SETv(ctx, lsfp[3].o, meta);   /* cc(DictMap, Method) */
//				KNH_SETv(ctx, lsfp[4].o, mtd);
//				knh_Closure_invokesfp(ctx, cc, lsfp+1, /*argc*/2);
//			}
//		}
//		KNH_LOCALBACK(ctx, lsfp);
//	}
//}
//
///* ------------------------------------------------------------------------ */
//
//void knh_invokeMethodTypingListener(Ctx *ctx, knh_DictMap_t *meta, knh_Method_t *mtd)
//{
//	knh_invokeMethodListener(ctx, meta, mtd, STEXT("MethodT"), STEXT("MethodT@"));
//}
//
///* ------------------------------------------------------------------------ */
//
//void knh_invokeMethodCompilationListener(Ctx *ctx, knh_DictMap_t *meta, knh_Method_t *mtd)
//{
//	knh_invokeMethodListener(ctx, meta, mtd, STEXT("MethodC"), STEXT("MethodC@"));
//}

/* ------------------------------------------------------------------------ */
/* [import script] */

static knh_bool_t knh_bytes_isSystemScript(knh_bytes_t path)
{
	size_t i;
	for(i = 0; i < path.len; i++) {
		int ch = path.buf[i];
		if(ch == '/' || ch == '\\' || ch == '.') return 0;
	}
	return 1;
}

/* ------------------------------------------------------------------------ */

knh_InputStream_t* new_ScriptInputStream(Ctx *ctx, knh_bytes_t path, knh_cwb_t *cwb, int isThrowable)
{
	knh_uri_t uri = 0;
	knh_StreamDSPI_t *drv = NULL;
	knh_cwb_t cwbbuf;
	if(cwb == NULL) {
		cwb = knh_cwb_openinit(ctx, &cwbbuf, path);
	}
	if(knh_bytes_startsWith(path, STEXT("http://"))) {
		drv = knh_getIODriver(ctx, STEXT("http"));
	}
	else {
		drv = knh_getIODriver(ctx, STEXT("file"));
		knh_cwb_ospath(ctx, cwb);
		if(!knh_cwb_isfile(ctx, cwb) && knh_bytes_isSystemScript(path)) {
			knh_String_t *spath = (knh_String_t*)knh_getProperty(ctx, STEXT("konoha.script.path"));
			if(IS_bString(spath)) {
				knh_cwb_subclear(cwb, 0);
				knh_cwb_write(ctx, cwb, __tobytes(spath));
				knh_cwb_putc(ctx, cwb, '/');
				knh_cwb_write(ctx, cwb, path);
				knh_cwb_ospath(ctx, cwb);
				if(knh_cwb_isfile(ctx, cwb)) {
					uri = knh_cwb_getResourceId(ctx, cwb);
					DBG2_ASSERT(uri != 0);
					uri = URI_TRUSTED(uri);
				}
			}
			spath = (knh_String_t*)knh_getProperty(ctx, STEXT("user.script.path"));
			if(IS_bString(spath) && uri == 0) {
				knh_cwb_subclear(cwb, 0);
				knh_cwb_write(ctx, cwb, __tobytes(spath));
				knh_cwb_putc(ctx, cwb, '/');
				knh_cwb_write(ctx, cwb, path);
				knh_cwb_ospath(ctx, cwb);
				if(knh_cwb_isfile(ctx, cwb)) {
					uri = knh_cwb_getResourceId(ctx, cwb);
					DBG2_ASSERT(uri != 0);
					uri = URI_TRUSTED(uri);
				}
			}
			if(uri == 0) {
				knh_cwb_subclear(cwb, 0);
				knh_cwb_write(ctx, cwb, path);
			}
		}
	}
	if(uri == 0) {
		uri = knh_cwb_getResourceId(ctx, cwb);
	}
	{
		knh_String_t *spath = knh_getResourceName(ctx, uri);
		path = __tobytes(spath);
		KNH_NOTICE(ctx, "importing script: %s", path.buf);
		knh_io_t fd = drv->fopen(ctx, path, "r", isThrowable);
		knh_InputStream_t *in = new_InputStream__io(ctx, spath, fd, drv);
		if(!knh_InputStream_isClosed(ctx, in)) {
			DP(in)->uri = uri;
			if(!knh_bytes_startsWith(path, STEXT("http://"))) {
				knh_InputStream_setEncoding(ctx, in, KNH_ENC);
			}
		}
		return in;
	}
}

/* ------------------------------------------------------------------------ */

knh_Stmt_t *knh_InputStream_parseStmt(Ctx *ctx, knh_InputStream_t *in, int isData)
{
	knh_Token_t *tk = new_Token(ctx, 0, DP(in)->uri, 0, TT_BRACE);
	KNH_LPUSH(ctx, tk);
	KNH_LPUSH(ctx, in);
	knh_InputStream_parseToken(ctx, in, tk);
	DBG2_DUMP(ctx, tk, KNH_NULL, "tokens");
	if(TT_(tk) != TT_ERR) {
		knh_tkc_t tcbuf, *tc = knh_Token_tc(ctx, tk, &tcbuf);
		return new_StmtSTMTBLOCK(ctx, tc, isData);
	}
	else {
		knh_Stmt_t *stmt = new_Stmt(ctx, 0, STT_DONE);
		knh_Stmt_toERR(ctx, stmt, TM(tk));
		return stmt;
	}
}

/* ------------------------------------------------------------------------ */

static void* knh_openDynamicLinkLibrary(Ctx *ctx, knh_uri_t uri)
{
	void *dlhdr = NULL;
	knh_bytes_t path = __tobytes(knh_getResourceName(ctx, uri));
	if(path.buf[0] != '(' && !knh_bytes_startsWith(path, STEXT("http://"))) {  // check "(eval)", "(shell)", or o
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		knh_index_t idx = knh_bytes_rindex(path, '.');
		if(idx > 0) path = knh_bytes_first(path, idx);
		knh_cwb_write(ctx, cwb, path);
		knh_cwb_putc(ctx, cwb, '_');
		knh_cwb_write(ctx, cwb, STEXT(KONOHA_PLATFORM));
		knh_cwb_ospath(ctx, cwb);
		dlhdr =knh_cwb_dlopen(ctx, cwb);
		if(dlhdr == NULL) {
			KNH_SYSLOG0(ctx, LOG_NOTICE, "dynamic library: opened %s", knh_cwb_tochar(ctx, cwb));
		}
		else if(knh_cwb_isfile(ctx, cwb)) {
			KNH_SYSLOG0(ctx, LOG_WARNING, "dynamic library: error %s", knh_cwb_tochar(ctx, cwb));
		}
		knh_cwb_close(cwb);
	}
	return dlhdr;
}

/* ------------------------------------------------------------------------ */

int knh_Script_loadStream(Ctx *ctx, knh_Script_t *script, knh_InputStream_t *in, int isEval, int isThrowable)
{
	int res = 1;
	knh_sfp_t *lsfp = KNH_LOCAL(ctx);
	knh_Gamma_t *gamma = ctx->kc;
	knh_Script_t *gamma_script = DP(gamma)->script;
	knh_uri_t uri = DP(in)->uri;
	knh_Stmt_t *stmt = (knh_Stmt_t*)KNH_NULL;
	KNH_LPUSH(ctx, in); /* lsfp[0] */
	KNH_LPUSH(ctx, new_ExceptionHandler(ctx)); /* lsfp[1] */
	KNH_LPUSH(ctx, stmt);
	KNH_TRY(ctx, L_CATCH, lsfp, 1);
	KNH_SETv(ctx, DP(gamma)->script, script);
	if(isEval) {
		DP(ctx->kc)->dlhdr = knh_openDynamicLinkLibrary(ctx, uri);
	}
	while(!knh_InputStream_isClosed(ctx, in)){
		stmt = knh_InputStream_parseStmt(ctx, in, 0/*isData*/);
		KNH_SETv(ctx, lsfp[2].o, stmt);
		if(STT_(stmt) == STT_DONE) break;
		if(URI_UNMASK(uri) != 0) {
			DBG_DUMP(ctx, stmt, KNH_NULL, "stmt");
		}
		res = knh_interpret(ctx, stmt, isEval);
	}
	KNH_SETv(ctx, DP(gamma)->script, gamma_script);
	goto L_RETURN;

	L_CATCH:;
	res = 0;
	KNH_SETv(ctx, DP(gamma)->script, gamma_script);
	if(isThrowable) {
		KNH_THROW_AGAIN(ctx, lsfp, 1);
	}
	else {
		KNH_PRINT_STACKTRACE(ctx, lsfp, 1);
	}

	L_RETURN:
	KNH_SETv(ctx, DP(gamma)->script, gamma_script);
	KNH_LOCALBACK(ctx, lsfp);
	return res;
}

/* ------------------------------------------------------------------------ */
/* [konoha] */
/* ------------------------------------------------------------------------ */

static int knh_Script_eval(Ctx *ctx, knh_Script_t *script, knh_bytes_t data)
{
	int res;
	knh_sfp_t *lsfp = KNH_LOCAL(ctx);
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_Bytes_write(ctx, cwb->ba, data);
	knh_Bytes_putc(ctx, cwb->ba, '\n');
	{
		knh_InputStream_t *in = new_BytesInputStream(ctx, cwb->ba, cwb->pos, knh_Bytes_size(cwb->ba));
		DP(in)->uri = URI_EVAL;
		DP(in)->line = 0;
		res = knh_Script_loadStream(ctx, script, in, 1/*isEval*/, 0/*isThrowable*/);
	}
	knh_cwb_close(cwb);
	KNH_LOCALBACK(ctx, lsfp);
	return res;
}

/* ------------------------------------------------------------------------ */

static int knh_Script_load(Ctx *ctx, knh_Script_t *script, knh_bytes_t path)
{
	int res = 0;
	knh_Gamma_t *gamma = ctx->kc;
	knh_sfp_t *lsfp = KNH_LOCAL(ctx);
	knh_InputStream_t *in = new_ScriptInputStream(ctx, path, NULL, 0/*isThrowable*/);
	knh_NameSpace_t *gamma_ns = DP(gamma)->ns;
	KNH_SETv(ctx, DP(gamma)->ns, new_NameSpace(ctx, NULL));
	KNH_LPUSH(ctx, in);
	res = knh_Script_loadStream(ctx, script, in, 1/*isEval*/, 0/*isThrowable*/);
	KNH_SETv(ctx, DP(gamma)->ns, gamma_ns);
	KNH_LOCALBACK(ctx, lsfp);
	return res;
}

/* ------------------------------------------------------------------------ */

KNHAPI(int) konoha_eval(konoha_t konoha, char *script)
{
	KONOHA_CHECK(konoha, 0);
	Ctx *ctx = knh_beginContext(konoha.ctx);
	int res = knh_Script_eval(ctx, ctx->script, B(script));
	knh_endContext(ctx);
	return res;
}

/* ------------------------------------------------------------------------ */

KNHAPI(int) konoha_load(konoha_t konoha, char *fpath)
{
	KONOHA_CHECK(konoha, 0);
	Ctx *ctx = knh_beginContext(konoha.ctx);
	int res = knh_Script_load(ctx, ctx->script, B(fpath));
	knh_Context_clearstack(ctx);
	knh_endContext(ctx);
	return res;
}

/* ------------------------------------------------------------------------ */
//## @Hidden @Static method Boolean Script.eval(String script);

METHOD Script_eval(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	KNH_ASSERT(IS_Script(sfp[0].o));
	int res = knh_Script_eval(ctx, (knh_Script_t*)sfp[0].o, __tobytes(sfp[1].s));
	KNH_RETURN_Boolean(ctx, sfp, res);
}

/* ------------------------------------------------------------------------ */
//## @Hidden @Static method Boolean Script.load(String path);

METHOD Script_load(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	KNH_ASSERT(IS_Script(sfp[0].o));
	int res = knh_Script_load(ctx, (knh_Script_t*)sfp[0].o, __tobytes(sfp[1].s));
	KNH_RETURN_Boolean(ctx, sfp, res);
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
