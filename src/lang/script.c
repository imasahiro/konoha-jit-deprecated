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

#define USE_STEXT 1
#define USE_B     1
#define USE_bytes_first       1
#define USE_bytes_index       1
#define USE_bytes_last        1
#define USE_bytes_index       1
#define USE_bytes_rindex      1
#define USE_bytes_equals      1
#define USE_bytes_startsWith  1
#define USE_bytes_endsWith    1

#define USE_cwb_open      1
#define USE_cwb_tobytes   1
#define USE_cwb_write     1

#include"commons.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */
/* [namespace] */

knh_NameSpace_t* new_NameSpace(Ctx *ctx, knh_NameSpace_t *nsNULL)
{
	knh_NameSpace_t* ns = new_(NameSpace);
	if(nsNULL != NULL) {
		DBG_ASSERT(IS_NameSpace(nsNULL));
		KNH_INITv(DP(ns)->parentNULL, nsNULL);
		KNH_SETv(ctx, DP(ns)->nsname, DP(nsNULL)->nsname);
		DP(ns)->regexSPI = DP(nsNULL)->regexSPI;
	}
	return ns;
}

/* ------------------------------------------------------------------------ */

knh_class_t knh_NameSpace_getcid(Ctx *ctx, knh_NameSpace_t *ns, knh_bytes_t sname)
{
	if(knh_bytes_equals(sname, STEXT("Script"))) {
		return knh_Object_cid(knh_getGammaScript(ctx));
	}
	L_TAIL:
	if(DP(ns)->name2cidDictSetNULL != NULL) {
		knh_uintptr_t cid = knh_DictSet_get(ctx, DP(ns)->name2cidDictSetNULL, sname);
		if(cid > 0) return (knh_class_t)(cid-1);
	}
	if(DP(ns)->parentNULL != NULL) {
		ns = DP(ns)->parentNULL;
		goto L_TAIL;
	}
	return knh_getcid(ctx, sname);
}

/* ------------------------------------------------------------------------ */
/* [stmt] */

knh_flag_t knh_Stmt_flag_(Ctx *ctx, knh_Stmt_t *stmt, knh_bytes_t name, knh_flag_t flag)
{
	if(IS_Map(DP(stmt)->metaDictCaseMap)) {
		Object *v = knh_DictCaseMap_getNULL(ctx, DP(stmt)->metaDictCaseMap, name);
		return (v != NULL) ? flag : 0;
	}
	return 0;
}

/* ------------------------------------------------------------------------ */
/* [function] */

//static
//void knh_NameSpace_setFuncClass(Ctx *ctx, knh_NameSpace_t *ns, knh_methodn_t mn, knh_class_t c)
//{
//	if(!MN_isGETTER(mn) && !MN_isSETTER(mn)) {
//		if(!IS_DictSet(DP(ns)->func2cidDictSet)) {
//			KNH_SETv(ctx, DP(ns)->func2cidDictSet, new_DictSet(ctx, 0));
//		}
//		knh_DictSet_set(ctx, DP(ns)->func2cidDictSet, knh_getFieldName(ctx, MN_toFN(mn)), (knh_uintptr_t)(c+1));
//	}
//}

/* ------------------------------------------------------------------------ */

knh_class_t knh_NameSpace_getFuncClass(Ctx *ctx, knh_NameSpace_t *ns, knh_methodn_t mn)
{
	if(!MN_isGETTER(mn) && !MN_isSETTER(mn)) {
		knh_bytes_t name = S_tobytes(knh_getFieldName(ctx, MN_toFN(mn)));
		L_TAIL:
		if(DP(ns)->func2cidDictSetNULL != NULL) {
			knh_uintptr_t cid = knh_DictSet_get(ctx, DP(ns)->func2cidDictSetNULL, name);
			if(cid != 0) {
				return (knh_class_t)(cid-1);
			}
		}
		if(DP(ns)->parentNULL != NULL) {
			ns = DP(ns)->parentNULL;
			goto L_TAIL;
		}
	}
	return CLASS_unknown; /* if not found */
}

/* ------------------------------------------------------------------------ */

knh_type_t knh_NameSpace_gettype(Ctx *ctx, knh_NameSpace_t *ns, knh_bytes_t name)
{
	if(name.ustr[0] == 'v') {
		if(name.len == 4 && name.ustr[1] == 'o' &&
				name.ustr[2] == 'i' && name.ustr[3] == 'd') return TYPE_void;
		if(name.len == 3 && name.ustr[1] == 'a' && name.ustr[2] == 'r') {
			return TYPE_var;
		}
	}
	return knh_NameSpace_getcid(ctx, ns, name);
}

/* ------------------------------------------------------------------------ */

knh_type_t knh_NameSpace_tagcid(Ctx *ctx, knh_NameSpace_t *o, knh_class_t cid, knh_bytes_t tag)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_class_t bcid = ClassTBL(cid).bcid;
	knh_printf(ctx, cwb->w, "%C:%B", bcid, tag);
	cid = knh_NameSpace_getcid(ctx, o, knh_cwb_tobytes(cwb));
	knh_cwb_close(cwb);
	return cid;
}

/* ------------------------------------------------------------------------ */
/* [include] */

static knh_bool_t knh_StmtINCLUDE_eval(Ctx *ctx, knh_Stmt_t *stmt, knh_type_t reqt, knh_Array_t *resultsNULL)
{
	int isCONTINUE = 1;
	BEGIN_LOCAL(ctx, lsfp, 1);
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_String_t *pathS = DP(DP(stmt)->tokens[0])->text;
	if(S_equals(pathS, STEXT("objectlink"))) {
		knh_bytes_t path = S_tobytes(knh_getURN(ctx, SP(stmt)->uri));
		if(path.ustr[0] != '(' && !knh_bytes_startsWith(path, STEXT("http://"))) {
			knh_index_t idx = knh_bytes_rindex(path, '.'); //'.'
			if(idx > 0) path = knh_bytes_first(path, idx);
			knh_cwb_write(ctx, cwb, path);
			knh_cwb_ospath(ctx, cwb);
			DP(ctx->gma)->dlhdr = knh_cwb_dlopen(ctx, cwb, 1/*isPERROR*/);
			if(DP(ctx->gma)->dlhdr != NULL) {
				knh_FcheckPKG f = (knh_FcheckPKG)knh_dlsym(ctx, DP(ctx->gma)->dlhdr, "kcheck", 1);
				knh_FsetupPKG f2 = (knh_FsetupPKG)knh_dlsym(ctx, DP(ctx->gma)->dlhdr, "init", 1);
				if(f != NULL && f() != K_BUILDID) {
					knh_Gamma_perror(ctx, KERR_DWARN, "objectfile might be incompatible: %B", knh_cwb_tobytes(cwb));
				}
				if(f2 != NULL) f2(ctx, knh_getPackageAPI(), NULL, 0/*isOVERRIDE*/);
				goto L_RETURN;
			}
		}
		isCONTINUE = 0;
		knh_Gamma_perror(ctx, KERR_ERR, "cannot open objectfile for %B", path);
	}
	else {
		knh_bytes_t path = S_tobytes(pathS);
		if(knh_bytes_index(path, ':') > 0) {
			isCONTINUE = knh_load(ctx, S_tobytes(pathS), reqt, resultsNULL);
		}
		else {
			knh_Bytes_write(ctx, cwb->ba, STEXT("script:"));
			knh_Bytes_write(ctx, cwb->ba, path);
			isCONTINUE = knh_load(ctx, knh_cwb_tobytes(cwb), reqt, resultsNULL);
		}
	}
	L_RETURN:;
	knh_cwb_close(cwb);
	END_LOCAL(ctx, lsfp);
	knh_Stmt_done(ctx, stmt);
	return isCONTINUE;
}

/* ------------------------------------------------------------------------ */
/* [using] */

static knh_Package_t *new_Package(Ctx *ctx, knh_String_t *nsname)
{
	knh_Package_t *pkg = new_(Package);
	KNH_INITv(pkg->ns, new_NameSpace(ctx, NULL));
	KNH_SETv(ctx, DP(pkg->ns)->nsname, nsname);
	KNH_INITv(pkg->script, new_(Script));
	return pkg;
}

static void knh_Gamma_swapPackage(Ctx *ctx, knh_Package_t *pkg)
{
	knh_NameSpace_t *ns = DP(ctx->gma)->ns;
	knh_Script_t *script = DP(ctx->gma)->script;
	DP(ctx->gma)->ns = pkg->ns;
	pkg->ns = ns;
	DP(ctx->gma)->script = pkg->script;
	pkg->script = script;
}

knh_bool_t knh_loadPackage(Ctx *ctx, knh_bytes_t path)
{
	int res = 1;
	knh_bytes_t name = knh_bytes_last(path, sizeof("pkg:") - 1);
	knh_Package_t *pkg = (knh_Package_t*)knh_DictMap_getNULL(ctx, DP(ctx->sys)->PackageDictMap, name);
	if(pkg == NULL) {
		knh_PathDSPI_t *dspi = knh_getPathDSPINULL(ctx, path);
		if(dspi->exists(ctx, path, NULL)) {
			knh_String_t *nameS = new_S(ctx, name);
			knh_Array_t * a = KNH_TNULL(Array);
			pkg = new_Package(ctx, nameS);
			knh_DictMap_set(ctx, DP(ctx->sys)->PackageDictMap, nameS, pkg);
			knh_Gamma_swapPackage(ctx, pkg);
			res = knh_load(ctx, path, TYPE_void, a);
			knh_Array_clear(ctx, a, 0);
			knh_Gamma_swapPackage(ctx, pkg);
		}
		else {
			res = 0;
		}
	}
	if(pkg != NULL) {
		knh_NameSpace_t *ns = knh_getGammaNameSpace(ctx);
		knh_NameSpace_t *pkgns = SP(pkg)->ns;
		if(DP(pkgns)->regexSPI != DP(pkgns)->strregexSPI) {
			DP(ns)->regexSPI = DP(pkgns)->regexSPI;
		}
	}
	return res;
}

/* ------------------------------------------------------------------------ */

static void knh_NameSpace_setcid(Ctx *ctx, knh_NameSpace_t *ns, knh_String_t *name, knh_class_t cid, int isOVERRIDE)
{
	if(DP(ns)->name2cidDictSetNULL == NULL) {
		KNH_INITv(DP(ns)->name2cidDictSetNULL, new_DictSet0(ctx, 0));
	}
	else {
		knh_class_t oldcid = knh_NameSpace_getcid(ctx, ns, S_tobytes(name));
		if(oldcid != CLASS_unknown && cid != oldcid) {
			knh_Gamma_perror(ctx, KERR_DWARN, _("renaming.. %s => %s"), CLASS__(oldcid), CLASS__(cid));
			if(!isOVERRIDE) return;
		}
	}
	knh_DictSet_set(ctx, DP(ns)->name2cidDictSetNULL, name, (knh_uintptr_t)(cid+1));
}

static int knh_StmtUSINGCLASS_eval(Ctx *ctx, knh_Stmt_t *stmt, size_t n)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_Bytes_write(ctx, cwb->ba, STEXT("pkg:"));
	knh_Token_t *tkPKG = DP(stmt)->tokens[n], *tkN;
	knh_Bytes_write(ctx, cwb->ba, S_tobytes(DP(tkPKG)->text));
	while(1) {
		tkN = DP(stmt)->tokens[++n];
		if(TT_(tkN) == TT_ASIS) break;
		if(TT_(tkN) == TT_DOT) continue;
		if(TT_(tkN) == TT_NAME) {
			knh_Bytes_putc(ctx, cwb->ba, '.');
			knh_Bytes_write(ctx, cwb->ba, S_tobytes(DP(tkN)->text));
			continue;
		}
		break;
	}
	KNH_SETv(ctx, DP(tkPKG)->data, knh_cwb_newString(ctx, cwb));
	if(knh_loadPackage(ctx, S_tobytes(DP(tkPKG)->text))) {
		knh_NameSpace_t *ns = knh_getGammaNameSpace(ctx);
		int isOVERRIDE = knh_Stmt_flag(ctx, stmt, "Override", 1);
		if(TT_(tkN) == TT_MUL) {
			knh_bytes_t pkgname = knh_bytes_last(S_tobytes(DP(tkPKG)->text), 8);
			size_t cid;
			for(cid = 0; cid < ctx->share->ClassTBLSize; cid++) {
				if(ClassTBL(cid).lname == NULL) continue;
				if(knh_class_isPrivate(cid) && knh_class_isGenerics(ctx, cid)) continue;
				knh_bytes_t cname = S_tobytes(ClassTBL(cid).lname);
				if(knh_bytes_startsWith(cname, pkgname)
						&& cname.ustr[pkgname.len] == '.' && isupper(cname.ustr[pkgname.len+1])) {
					knh_NameSpace_setcid(ctx, ns, ClassTBL(cid).sname, (knh_class_t)cid, isOVERRIDE);
				}
			}
		}
		else if(TT_(tkN) == TT_UNAME) {
			knh_class_t newcid;
			knh_String_t* cname = DP(tkN)->text;
			knh_Bytes_write(ctx, cwb->ba, knh_bytes_last(S_tobytes(DP(tkPKG)->text), sizeof("pkg")));
			knh_Bytes_putc(ctx, cwb->ba, '.');
			knh_Bytes_write(ctx, cwb->ba, S_tobytes(cname));
			newcid = knh_getcid(ctx, knh_cwb_tobytes(cwb));
			if(newcid == CLASS_unknown) {
				KNH_SETv(ctx, DP(tkPKG)->data, knh_cwb_newString(ctx, cwb));
				goto L_ERROR;
			}
			else {
#ifdef TT_AS
				if(n+2 < DP(stmt)->size &&
					TT_(DP(stmt)->tokens[n+1]) == TT_AS && TT_(DP(stmt)->tokens[n+2]) == TT_UNAME) {
					cname = DP(DP(stmt)->tokens[n+2])->text;
				}
#endif
				knh_NameSpace_setcid(ctx, ns, cname, newcid, isOVERRIDE);
			}
			knh_cwb_clear(cwb, 0);
		}
		return 1;
	}
	L_ERROR:;
	while(n + 1 < DP(stmt)->size) {
		if(TT_(DP(stmt)->tokens[n]) == TT_OR) return 0;
		n++;
	}
	knh_Gamma_perror(ctx, KERR_ERR, _("not found: %B"), S_tobytes(DP(tkPKG)->text));
	return 0;
}

/* ------------------------------------------------------------------------ */

//static int knh_StmtUALIAS_decl(Ctx *ctx, knh_Stmt_t *stmt)
//{
//	DBG_DUMP(ctx, stmt, KNH_NULL, "decl");
//	TODO();
//	return 1;
//}

///* ------------------------------------------------------------------------ */
//
//static
//char *knh_format_classurn(Ctx *ctx, char *buf, size_t bufsiz, knh_class_t bcid, knh_bytes_t urn)
//{
//	knh_snprintf(buf, bufsiz, KNH_SEMANTICS_FMT, CLASS__(bcid), urn.buf);
//	return buf;
//}
//
///* ------------------------------------------------------------------------ */
//
//static
//void knh_NameSpace_setTagName(Ctx *ctx, knh_NameSpace_t *o, knh_String_t *name, knh_class_t cid)
//{
//	KNH_ASSERT(IS_NameSpace(o));
//	DBG_ASSERT_cid(cid);
//	knh_DictSet_set(ctx, DP(o)->name2cidDictSet, name, (knh_uintptr_t)(cid+1));
////	knh_bytes_t n = S_tobytes(name);
////	knh_index_t loc = knh_bytes_index(n, ':');
////	if(loc != -1) {
////		n = knh_bytes_last(n, loc+1);
////		if(isupper(n.ustr[0])) {
////			knh_DictSet_set(ctx, DP(o)->name2cidDictSet, new_String(ctx, n, name), (knh_uintptr_t)(cid+1));
////		}
////	}
//}

/* ------------------------------------------------------------------------ */

//static int knh_StmtXCLASS_decl(Ctx *ctx, knh_Stmt_t *stmt, knh_class_t bcid)
//{
//
//	knh_NameSpace_t *ns = knh_getGammaNameSpace(ctx);
//	knh_Token_t *tkclassn = DP(stmt)->tokens[0];
//	knh_Token_t *tkurn = DP(stmt)->tokens[1];
//
//	char bufcu[CLASS__AME_BUFSIZ];
//	knh_format_classurn(ctx, bufcu, sizeof(bufcu), bcid, knh_getURNAlias(ctx, knh_Token_tobytes(ctx, tkurn)));
//
//	knh_class_t cid = knh_findcid(ctx, B(bufcu));
//	if(cid == bcid || cid == CLASS_unknown) {
//		knh_Token_perror(ctx, tkurn, KERR_EWARN, _("resource not found: %L"), tkurn);
//		cid = bcid;
//	}
//
//	knh_class_t oldcid = knh_NameSpace_findcid(ctx, ns, knh_Token_tobytes(ctx, tkclassn));
//	if(oldcid != CLASS_unknown && cid != oldcid) {
//		KNH_SYSLOG(ctx, LOG_WARNING, _("overriding %s"), knh_Token_tobytes(ctx, tkclassn));
//	}
//
//	KNH_ASSERT(IS_String(DP(tkclassn)->data));
//	knh_NameSpace_setTagName(ctx, ns, DP(tkclassn)->text, cid);
//	TODO();
//	return 1;
//}

/* ------------------------------------------------------------------------ */

//static int knh_StmtUVOCAB_decl(Ctx *ctx, knh_Stmt_t *stmt)
//{
//	return knh_StmtXCLASS_decl(ctx, stmt, CLASS_String);
//}

/* ------------------------------------------------------------------------ */

//static int knh_StmtUENUM_decl(Ctx *ctx, knh_Stmt_t *stmt)
//{
//	return knh_StmtXCLASS_decl(ctx, stmt, CLASS_Int);
//}

/* ------------------------------------------------------------------------ */

//static int knh_StmtUUNIT_decl(Ctx *ctx, knh_Stmt_t *stmt)
//{
//	return knh_StmtXCLASS_decl(ctx, stmt, CLASS_Float);
//}

/* ------------------------------------------------------------------------ */

//static int knh_StmtUFUNC_decl(Ctx *ctx, knh_Stmt_t *stmt)
//{
//	knh_NameSpace_t *ns = knh_getGammaNameSpace(ctx);
//	knh_Token_t *tk = DP(stmt)->tokens[0];
//	knh_bytes_t name = knh_Token_tobytes(ctx, tk);
//	if(SP(tk)->tt == TT_LONGNAME) {
//		knh_index_t loc = knh_bytes_rindex(name, '.');
//		knh_class_t cid = knh_NameSpace_findcid(ctx, ns, knh_bytes_first(name, loc));
//		if(cid == CLASS_unknown) {
//			knh_Token_perror(ctx, tk, KERR_ERR, _("unknown class: %L"), tk);
//			return 0;
//		}
//		knh_methodn_t mn = knh_getmn(ctx, knh_bytes_last(name, loc+1), MN_NONAME);
//		if(mn == MN_NONAME) {
//			knh_Token_perror(ctx, tk, KERR_ERR, _("unknown method: %C.%L"), cid, tk);
//			return 0;
//		}
//		knh_NameSpace_setFuncClass(ctx, ns, mn, cid);
//	}
//	else {
//		knh_class_t cid = knh_NameSpace_findcid(ctx, ns, name);
//		if(cid == CLASS_unknown) {
//			knh_Token_perror(ctx, tk, KERR_ERR, _("unknown class: %L"), tk);
//			return 0;
//		}
//		else {
//			DBG_ASSERT_cid(cid);
//			knh_Array_t *a = ClassTBL(cid).methods;
//			size_t i;
//			for(i = 0; i < knh_Array_size(a); i++) {
//				knh_Method_t *mtd = (knh_Method_t*)knh_Array_n(a, i);
//				KNH_ASSERT(IS_Method(mtd));
//				if(!knh_Method_isStatic(mtd)) continue;
//				knh_NameSpace_setFuncClass(ctx, ns, DP(mtd)->mn, cid);
//				if(knh_Context_isVerbose(ctx)) {
//					KNH_SYSLOG(ctx, LOG_NOTICE, _("using %C.%M"), cid, DP(mtd)->mn);
//				}
//			}
//		}
//	}
//	return 1;
//}

/* ------------------------------------------------------------------------ */

//static int knh_StmtUMAPMAP_decl(Ctx *ctx, knh_Stmt_t *stmt)
//{
//	DBG_DUMP(ctx, stmt, KNH_NULL, "decl");
//	return 1;
//}


static knh_bool_t knh_StmtUSING_eval(Ctx *ctx, knh_Stmt_t *stmt)
{
	int isCONTINUE = 1;
	size_t n = 0;
	L_TRYAGAIN:; {
		knh_Token_t *tkF = DP(stmt)->tokens[n];
		if(TT_(tkF) == TT_NAME) {
			if(knh_StmtUSINGCLASS_eval(ctx, stmt, n)) {
				goto L_RETURN;
			}
		}
	}
	while(n + 1 < DP(stmt)->size) {  // find next or
		knh_Token_t *tkF = DP(stmt)->tokens[n];
		n++;
		if(TT_(tkF) == TT_OR) goto L_TRYAGAIN;
	}
	isCONTINUE = 0;
	L_RETURN:;
	knh_Stmt_done(ctx, stmt);
	return isCONTINUE;
}

/* ------------------------------------------------------------------------ */
/* [CLASS] */

static knh_flag_t knh_StmtCLASS_flag(Ctx *ctx, knh_Stmt_t *stmt)
{
	knh_flag_t flag = 0;
	if(IS_Map(DP(stmt)->metaDictCaseMap)) {
		flag |= knh_Stmt_flag(ctx, stmt, "Final",     FLAG_Class_Final);
		flag |= knh_Stmt_flag(ctx, stmt, "Private",   FLAG_Class_Private);
		flag |= knh_Stmt_flag(ctx, stmt, "Interface", FLAG_Class_Interface);
		flag |= knh_Stmt_flag(ctx, stmt, "Singleton", FLAG_Class_Singleton);
		flag |= knh_Stmt_flag(ctx, stmt, "Release",   FLAG_Class_Release);
	}
	return flag;
}

/* ------------------------------------------------------------------------ */

static int knh_StmtCLASS_decl(Ctx *ctx, knh_Stmt_t *stmt)
{
	knh_class_t cid;
	knh_Token_t *tkC = DP(stmt)->tokens[0/*CNAME*/];
	knh_Token_t *tkE = DP(stmt)->tokens[2/*extends D*/];
	knh_ClassTBL_t *t = NULL;
	knh_NameSpace_t *ns = knh_getGammaNameSpace(ctx);
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_Bytes_write(ctx, cwb->ba, S_tobytes(DP(ns)->nsname));
	knh_Bytes_putc(ctx, cwb->ba, '.');
	knh_Bytes_write(ctx, cwb->ba, TK_tobytes(tkC));
	cid = knh_getcid(ctx, knh_cwb_tobytes(cwb));
	if(cid == CLASS_unknown) {
		cid = new_ClassId(ctx);
		t = pClassTBL(ctx, cid);
	}
	else {
		knh_cwb_close(cwb);
		t = pClassTBL(ctx, cid);
		if(!(t->bcid == CLASS_Object && t->fields == NULL)) {
			knh_Gamma_perror(ctx, KERR_ERR, _("re-definition of %B(%d)"), S_tobytes(DP(tkC)->text), cid);
			goto L_ERROR;
		}
	}

	DP(tkC)->cid = cid;
	if(t->supcid == 0) {
		t->supcid = knh_Token_getcid(ctx, tkE, CLASS_Object);
		if(knh_class_isFinal(t->supcid)) {
			knh_Gamma_perror(ctx, KERR_ERR, _("cannot extends final class %C"), t->supcid);
			goto L_ERROR;
		}
		t->offset = 0; /* will be extended in CLASS_typing */
		t->keyidx = ClassTBL(t->supcid).keyidx;
	}
	if(t->lname == NULL) {
		t->cflag  = knh_StmtCLASS_flag(ctx, stmt);
		t->oflag  = FLAG_oflag(t->cflag);
		if(DP(stmt)->size == 5 && DP(ctx->gma)->dlhdr != NULL) {
			knh_Fclass f = (knh_Fclass)knh_dlsym(ctx, DP(ctx->gma)->dlhdr, S_tochar(DP(tkC)->text), 0/*isRequired*/);
			if(f != NULL) {
				knh_ClassData_t *csetup = f();
				if(csetup->cspi != NULL) {
					t->bcid = cid;
					t->cspi = csetup->cspi;
					t->cflag = t->cflag | t->cspi->cflag;
					t->oflag = FLAG_oflag(t->cflag);
				}
				else {
					t->bcid = CLASS_RawPtr;
					t->cspi = ClassTBL(CLASS_RawPtr).cspi;
				}
				if(csetup->fdefault != NULL) {
					knh_setClassDefaultValue(ctx, cid, NULL, csetup->fdefault);
				}
				else {
					knh_Object_t *o = new_hObject_(ctx, t->oflag | FLAG_Object_NullObject, t->bcid, cid);
					t->cspi->init(ctx, o);
					knh_setClassDefaultValue(ctx, cid, o, NULL);
				}
				goto L_NAME;
			}
		}
		t->bcid = CLASS_Object;
		t->cspi = ClassTBL(CLASS_Object).cspi;
		knh_setClassDefaultValue(ctx, cid, new_hObject_(ctx, t->oflag | FLAG_Object_NullObject, t->bcid, cid), NULL);
		DBG_ASSERT(t->fields == NULL);
		DBG_ASSERT(t->fsize  == 0);
	}
	L_NAME:;
	if(t->lname == NULL) {
		knh_setClassName(ctx, cid, knh_cwb_newString(ctx, cwb), DP(tkC)->text);
		KNH_INITv(t->methods, KNH_EMPTYLIST);
		KNH_INITv(t->tmaps, KNH_EMPTYLIST);
		knh_NameSpace_setcid(ctx, ns, DP(tkC)->text, cid, 1);
		KNH_SYSLOG(ctx, LOG_NOTICE, "NEW_CLASS", "*cid=%d, name='%s'", cid, CLASS__(cid));
	}
	if(DP(stmt)->size == 5) {
		knh_Stmt_done(ctx, stmt);
	}
	return 1;

	L_ERROR:;
	knh_Stmt_done(ctx, stmt);
	return 0;
}

/* ------------------------------------------------------------------------ */

void knh_Stmt_scriptAsm(Ctx *ctx, knh_Stmt_t *stmtITR);

static knh_Method_t *knh_Script_getEvalMethod(Ctx *ctx, knh_Script_t *scr)
{
	knh_Method_t *mtd = knh_getMethodNULL(ctx, knh_Object_cid(scr), MN_LAMBDA);
	if(mtd == NULL) {
		mtd = new_Method(ctx, 0, knh_Object_cid(scr), MN_LAMBDA, NULL);
		KNH_SETv(ctx, DP(mtd)->mp, new_ParamArrayR0(ctx, TYPE_Any));
		knh_Class_addMethod(ctx, knh_Object_cid(scr), mtd);
	}
	return mtd;
}

static knh_bool_t knh_Stmt_eval(Ctx *ctx, knh_Stmt_t *stmtITR, knh_type_t reqt, knh_Array_t *resultsNULL)
{
	knh_bool_t isCONTINUE = 1;
	BEGIN_LOCAL(ctx, lsfp, 4);
	knh_Stmt_t *stmt = NULL;
	KNH_SETv(ctx, lsfp[0].o, stmtITR);
	SP(ctx->gma)->uri = SP(stmtITR)->uri;
	stmt = stmtITR;
	while(stmt != NULL) {
		knh_Gamma_setLine(ctx, SP(stmt)->line);
		switch(STT_(stmt)) {
		case STT_NAMESPACE:
		{
			knh_NameSpace_t *ns = new_NameSpace(ctx, DP(ctx->gma)->ns);
			KNH_SETv(ctx, DP(ctx->gma)->ns, ns);
			isCONTINUE = knh_Stmt_eval(ctx, DP(stmt)->stmts[0], reqt, resultsNULL);
			DBG_ASSERT(DP(ctx->gma)->ns == ns);
			DBG_ASSERT(DP(ns)->parentNULL != NULL);
			KNH_SETv(ctx, DP(ctx->gma)->ns, DP(ns)->parentNULL);
			knh_Stmt_done(ctx, stmt);
			break;
		}
		case STT_SCRIPT:
		{
			knh_Script_t *pscript = knh_getGammaScript(ctx);
			knh_Script_t *script = new_(Script);
			KNH_SETv(ctx, DP(ctx->gma)->script, script);
			isCONTINUE = knh_Stmt_eval(ctx, DP(stmt)->stmts[0], reqt, resultsNULL);
			KNH_SETv(ctx, DP(ctx->gma)->script, pscript);
			knh_Stmt_done(ctx, stmt);
			break;
		}
//		case STT_IF: /* Conditional Compilation */
//		if(knh_Stmt_flag(ctx, stmt, "Static", 1)) {
//			knh_Gamma_initThisScript(ctx);
//			stmt = knh_StmtIF_decl(ctx, stmt);
//			isCONTINUE = 0;
//			if(stmt != NULL) {
//				isCONTINUE = knh_Stmt_eval(ctx, stmt, reqt, resultsNULL);
//			}
//			knh_Stmt_done(ctx, stmt);
//			break;
//		}
		case STT_INCLUDE:
			isCONTINUE = knh_StmtINCLUDE_eval(ctx, stmt, reqt, resultsNULL);
			break;
		case STT_USING:
			isCONTINUE = knh_StmtUSING_eval(ctx, stmt);
			break;
		case STT_CLASS:
			isCONTINUE = knh_StmtCLASS_decl(ctx, stmt);
			break;
		case STT_RETURN:
			knh_Gamma_perror(ctx, KERR_ERRATA, _("'break' is much better"));
		case STT_BREAK:
			isCONTINUE = 0;
		}
		if(isCONTINUE == 0) goto L_BREAK;
		stmt = DP(stmt)->nextNULL;
	}
	if(knh_StmtITR_scriptTyping(ctx, stmtITR, reqt) == 0 && resultsNULL != NULL) {
		isCONTINUE = 0; goto L_BREAK;
	}
	stmt = stmtITR;
	while(stmt != NULL) {
		knh_Gamma_setLine(ctx, SP(stmt)->line);
		knh_Stmt_scriptAsm(ctx, stmt);
		if(STT_(stmt) != STT_DONE) {
			knh_Script_t *scr = knh_getGammaScript(ctx);
			knh_Method_t *mtd = knh_Script_getEvalMethod(ctx, scr);
			int isExpr = knh_stmt_isExpr(STT_(stmt));
			if(STT_(stmt) == STT_ERR) {
				isCONTINUE = 0; goto L_BREAK;
			}
			knh_Stmt_setEveryLine(stmt, 1);
			knh_Method_asm(ctx, mtd, NULL, stmt, isExpr ? reqt : TYPE_void, knh_Method_typing);
			if(knh_Method_isAbstract(mtd) || STT_(stmt) == STT_ERR) {
				isCONTINUE = 0; goto L_BREAK;
			}
			if(resultsNULL != NULL) {
				DP(mtd)->uri = SP(stmt)->uri;
				KNH_SETv(ctx, lsfp[1].o, DP(mtd)->kcode);  // put KCode to avoid GC
				klr_setcallmtd(ctx, lsfp[2], mtd);
				KNH_SETv(ctx, lsfp[3].o, scr);
				klr_setesp(ctx, lsfp + 4);
				if(knh_VirtualMachine_run(ctx, lsfp + 3, CODE_LAUNCH) == NULL) {
					int rtnidx=4;
					knh_class_t cid = knh_Object_cid(lsfp[rtnidx].o);
					//DBG_P("returning sfpidx=%d, rtnidx=%d, %s %lld %ld %f", sfpidx_, sfpidx_ + rtnidx, O__(lsfp[rtnidx].o), lsfp[rtnidx].ivalue, lsfp[rtnidx].bvalue, lsfp[rtnidx].fvalue);
					if(isExpr && SP(stmt)->type != TYPE_void) {
						if(cid == reqt || reqt == TYPE_Any || knh_class_instanceof(ctx, cid, reqt)) {
							knh_Array_add(ctx, resultsNULL, lsfp[rtnidx].o);
						}
					}
				}
				else {
					isCONTINUE = 0; goto L_BREAK;
				}
			}
		}
		stmt = DP(stmt)->nextNULL;
	}
	L_BREAK:;
	END_LOCAL(ctx, lsfp);
	knh_Gamma_setLine(ctx, 0);
	return isCONTINUE;
}

/* ------------------------------------------------------------------------ */

knh_bool_t knh_eval(Ctx *ctx, knh_InputStream_t *in, knh_type_t reqt, knh_Array_t *resultsNULL)
{
	int isCONTINUE = 1;
	BEGIN_LOCAL(ctx, lsfp, 3);
	KNH_SETv(ctx, lsfp[0].o, in);
	if(resultsNULL != NULL) {
		KNH_SETv(ctx, lsfp[1].o, resultsNULL);
	}
	LOCAL_NEW(ctx, lsfp, 2, knh_Stmt_t *, stmt, knh_InputStream_parseStmt(ctx, in));
	isCONTINUE = knh_Stmt_eval(ctx, stmt, reqt, resultsNULL);
	END_LOCAL(ctx, lsfp);
	return isCONTINUE;
}

/* ------------------------------------------------------------------------ */

static int knh_bytes_isempty(knh_bytes_t t)
{
	size_t i;
	for(i = 0; i < t.len; i++) {
		if(!isspace(t.ustr[i])) return 0;
	}
	return 1;
}

static void knh_Bytes_addQUOTE(Ctx *ctx, knh_Bytes_t *ba, knh_InputStream_t *in, int quote)
{
	int ch, prev = quote;
	while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
		knh_Bytes_putc(ctx, ba, ch);
		if(ch == quote && prev != '\\') {
			return;
		}
		prev = ch;
	}
}

static void knh_Bytes_addCOMMENT(Ctx *ctx, knh_Bytes_t *ba, knh_InputStream_t *in)
{
	int ch, prev = 0, level = 1;
	while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
		knh_Bytes_putc(ctx, ba, ch);
		if(prev == '*' && ch == '/') level--;
		if(prev == '/' && ch == '*') level++;
		if(level == 0) return;
		prev = ch;
	}
}

static void knh_Bytes_skipLCOMMENT(Ctx *ctx, knh_Bytes_t *ba, knh_InputStream_t *in)
{
	int ch;
	while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
		if(ch == '\n') {
			knh_Bytes_putc(ctx, ba, ch);
			break;
		}
	}
}

static knh_InputStream_t* knh_openPathNULL(Ctx *ctx, knh_bytes_t path)
{
	knh_StreamDSPI_t *sdspi = knh_getStreamDSPI(ctx, path);
	knh_io_t fd = sdspi->fopen(ctx, path, "r");
	if(fd == IO_NULL) {
		KNH_SYSLOG(ctx, LOG_ERR, "ScriptNotFound", "path='%B'", path);
		return NULL;
	}
	else {
		knh_intptr_t id = 0;
		knh_PathDSPI_t *pdspi = knh_getPathDSPINULL(ctx, path);
		if(pdspi != NULL) pdspi->exists(ctx, path, &id);
		knh_InputStream_t *in = new_InputStreamDSPI(ctx, fd, sdspi);
		if(id != 0) {
			DP(in)->uri = (knh_uri_t)id;
			KNH_SETv(ctx, DP(in)->urn, knh_getURN(ctx, DP(in)->uri));
		}
		return in;
	}
}

knh_bool_t knh_load(Ctx *ctx, knh_bytes_t path, knh_type_t reqt, knh_Array_t *resultsNULL)
{
	knh_bool_t res = 0;
	long linenum = 1;
	void *dlhdrSTACK = DP(ctx->gma)->dlhdr;
	BEGIN_LOCAL(ctx, lsfp, 2);
	DP(ctx->gma)->dlhdr = NULL;

	LOCAL_NEW(ctx, lsfp, 1, knh_InputStream_t *, bin, new_BytesInputStream(ctx, new_Bytes(ctx, K_PAGESIZE)));
	knh_Bytes_t *ba = DP(bin)->ba;

	knh_InputStream_t *in = knh_openPathNULL(ctx, path);
	if(in == NULL) {
		goto L_RETURN;
	}
	KNH_SETv(ctx, lsfp[0].o, in);

	L_READLINE:;
	knh_Bytes_clear(ba, 0);
	if(!knh_InputStream_isClosed(ctx, in)) {
		int ch;
		int isBLOCK = 0, prev = 0;
		linenum = DP(in)->line;
		while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
			if(ch == '\r') continue;
			knh_Bytes_putc(ctx, ba, ch);
			if(isBLOCK && (ch == '}' || ch == ']') && prev == 0) {
				isBLOCK = 0;
			}
			if(prev == '/' && ch == '*') {
				knh_Bytes_addCOMMENT(ctx, ba, in);
				continue;
			}
			if(prev == '/' && ch == '/') {
				knh_Bytes_skipLCOMMENT(ctx, ba, in);
				continue;
			}
			if(ch == '\'' || ch == '"' || ch == '`') {
				knh_Bytes_addQUOTE(ctx, ba, in, ch);
				continue;
			}
			if(ch == '\n') {
				if(prev == '{' || prev == '[') {
					isBLOCK = 1;
				}
				if(prev == '\\') continue;
				if(!isBLOCK) goto L_PARSE;
				prev = 0;
				continue;
			}
			if(prev != 0 && (ch == ' ' || ch == '\t')) continue;
			prev = ch;
		}
	}
	L_PARSE:;
	if(!knh_bytes_isempty(ba->bu)) {
		knh_BytesInputStream_setpos(ctx, bin, 0, BA_size(ba));
		DP(bin)->uri = DP(in)->uri;
		DP(bin)->line = linenum;
		//knh_InputStream_setCharset(ctx, bin, DP(in)->enc);
		DBG_(if(knh_isSystemVerbose()) {
			fprintf(stderr, "\n>>>--------------------------------\n");
			fprintf(stderr, "%s<<<--------------------------------\n", knh_Bytes_ensureZero(ctx, ba));
		});
		res = knh_eval(ctx, bin, reqt, resultsNULL);
		if(res == 0) goto L_RETURN;
		goto L_READLINE;
	}
	if(BA_size(ba) > 0) {
		goto L_READLINE;
	}
	L_RETURN:;
	if(DP(ctx->gma)->dlhdr != NULL) {
		knh_FsetupPKG f = (knh_FsetupPKG)knh_dlsym(ctx, DP(ctx->gma)->dlhdr, "setup", 0);
		if(f != NULL) f(ctx, knh_getPackageAPI(), NULL, 0/*isOVERRIDE*/);
		DP(ctx->gma)->dlhdr = NULL;
	}
	DP(ctx->gma)->dlhdr = dlhdrSTACK;
	END_LOCAL(ctx, lsfp);
	return res;
}

/* ------------------------------------------------------------------------ */

KNHAPI(int) konoha_load(konoha_t konoha, knh_bytes_t path, int isCompileOnly)
{
	KONOHA_CHECK(konoha, 0);
	int isCONTINUE;
	Ctx *ctx = KONOHA_BEGIN(konoha.ctx);
	char buf[256];
	knh_Array_t *resultsNULL = isCompileOnly ? NULL : KNH_TNULL(Array);
	if(B_endsWith(path, ".k")) {
		knh_snprintf(buf, sizeof(buf), "file:%s", path.text);
	}
	else {
		knh_snprintf(buf, sizeof(buf), "tool:%s", path.text);
	}
	isCONTINUE = knh_load(ctx, B(buf), TYPE_void, resultsNULL);
	if(resultsNULL != NULL) knh_Array_clear(ctx, resultsNULL, 0);
	knh_stack_clear(ctx, ctx->stack);
	KONOHA_END(ctx);
	return isCONTINUE;
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
