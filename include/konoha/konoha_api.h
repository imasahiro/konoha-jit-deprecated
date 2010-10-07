// THIS FILE WAS AUTOMATICALLY GENERATED

#ifdef __cplusplus
extern "C" {
#endif



/* ======================================================================== */
/* PUBLIC */
KNHAPI(knh_Array_t*) new_Array(Ctx *ctx, knh_class_t p1, size_t capacity);
KNHAPI(void) knh_Array_add_(Ctx *ctx, knh_Array_t *a, Any *value);
KNHAPI(konoha_t) konoha_open(size_t stacksize);
KNHAPI(void) konoha_close(konoha_t konoha);
KNHAPI(knh_Iterator_t*) new_Iterator(Ctx *ctx, knh_class_t p1, Any *source, knh_Fitrnext fnext);
KNHAPI(Object*) new_Object_boxing(Ctx *ctx, knh_class_t cid, knh_sfp_t *sfp);
KNHAPI(knh_Int_t*) new_Int(Ctx *ctx, knh_class_t cid, knh_int_t value);
KNHAPI(knh_Float_t*) new_Float(Ctx *ctx, knh_class_t cid, knh_float_t value);
KNHAPI(void) knh_ResultSet_initColumn(Ctx *ctx, knh_ResultSet_t *o, size_t column_size);
KNHAPI(void) knh_ResultSet_setName(Ctx *ctx, knh_ResultSet_t *o, size_t n, knh_String_t *name);
KNHAPI(void) knh_ResultSet_setInt(Ctx *ctx, knh_ResultSet_t *rs, size_t n, knh_int_t value);
KNHAPI(void) knh_ResultSet_setFloat(Ctx *ctx, knh_ResultSet_t *rs, size_t n, knh_float_t value);
KNHAPI(void) knh_ResultSet_setText(Ctx *ctx, knh_ResultSet_t *o, size_t n, knh_bytes_t t);
KNHAPI(void) knh_ResultSet_setBlob(Ctx *ctx, knh_ResultSet_t *o, size_t n, knh_bytes_t t);
KNHAPI(void) knh_ResultSet_setNULL(Ctx *ctx, knh_ResultSet_t *o, size_t n);
KNHAPI(Ctx*) knh_beginContext(Ctx *ctx, Ctx **bottom);
KNHAPI(void) knh_endContext(Ctx *ctx);
KNHAPI(Ctx*) knh_getCurrentContext(void);
KNHAPI(knh_bool_t) knh_isInteractiveMode(void);
KNHAPI(knh_bool_t) knh_isTestMode(void);
KNHAPI(void) konoha_shell(konoha_t konoha, char *optstr);
KNHAPI(knh_Int_t*) new_IntX(Ctx *ctx, knh_class_t cid, knh_int_t value);
KNHAPI(knh_Float_t*) new_FloatX(Ctx *ctx, knh_class_t cid, knh_float_t value);
KNHAPI(knh_Semantics_t*) new_Enum(Ctx *ctx, char *tag, knh_bytes_t urn, knh_int_t min, knh_int_t max);
KNHAPI(knh_Semantics_t*) new_Unit(Ctx *ctx, char *tag, knh_bytes_t urn, knh_float_t min, knh_float_t max, knh_float_t step);
KNHAPI(knh_Semantics_t*) new_Vocab(Ctx *ctx, char *tag, knh_bytes_t urn, int base, char **terms);
KNHAPI(void) knh_loadURNAliasData(Ctx *ctx, knh_StringData_t *data);
KNHAPI(void) knh_loadSemanticsFuncData(Ctx *ctx, knh_NamedPointerData_t *data);
KNHAPI(knh_OutputStream_t*) new_BytesOutputStream(Ctx *ctx, knh_Bytes_t *ba);
KNHAPI(void) knh_OutputStream_putc(Ctx *ctx, knh_OutputStream_t *w, int ch);
KNHAPI(void) knh_OutputStream_write(Ctx *ctx, knh_OutputStream_t *w, knh_bytes_t buf);
KNHAPI(void) knh_OutputStream_flush(Ctx *ctx, knh_OutputStream_t *w);
KNHAPI(void) knh_OutputStream_writeLine(Ctx *ctx, knh_OutputStream_t *w, knh_bytes_t t, knh_bool_t isNEWLINE);
KNHAPI(void) knh_write_EOL(Ctx *ctx, knh_OutputStream_t *w);
KNHAPI(void) knh_write_TAB(Ctx *ctx, knh_OutputStream_t *w);
KNHAPI(void) knh_write_BOL(Ctx *ctx, knh_OutputStream_t *w);
KNHAPI(void) knh_printf(Ctx *ctx, knh_OutputStream_t *w, const char *fmt, ...);
KNHAPI(knh_String_t*) new_String_(Ctx *ctx, knh_class_t cid, knh_bytes_t t, knh_String_t *memoNULL);
KNHAPI(void) knh_setPropertyText(Ctx *ctx, char *key, char *value);
KNHAPI(void) knh_addTranslator(Ctx *ctx, knh_Translator_t *trl);
KNHAPI(knh_Translator_t*) new_Translator(Ctx *ctx, knh_flag_t flag, knh_class_t scid, knh_class_t tcid, knh_Ftranslator ftcast);

/* ======================================================================== */
/* DEVELOPERS API */
void knh_BasicBlock_add_(Ctx *ctx, knh_BasicBlock_t *bb, int line, knh_opline_t *op);
METHOD knh_Fmethod_runVM(Ctx *ctx, knh_sfp_t *sfp, long rix);
void knh_NameSpace_addFormatter(Ctx *ctx, knh_NameSpace_t *ns, knh_Method_t *mtd);
void knh_Method_asm(Ctx *ctx, knh_Method_t *mtd, knh_Stmt_t *stmtP, knh_Stmt_t *stmtB, knh_type_t reqt, knh_Ftyping typing);
void knh_loadSystemKLRCode(Ctx *ctx);
void knh_write_vmfunc(Ctx *ctx, knh_OutputStream_t *w, void *f);
void jit_dump(void *mem, int size);
void pjit_compile(Ctx *ctx, knh_Method_t *mtd);
knh_NameSpace_t* new_NameSpace(Ctx *ctx, knh_NameSpace_t *nsNULL);
knh_class_t knh_NameSpace_getcid(Ctx *ctx, knh_NameSpace_t *ns, knh_bytes_t sname);
knh_flag_t knh_Stmt_flag_(Ctx *ctx, knh_Stmt_t *stmt, knh_bytes_t name, knh_flag_t flag);
knh_class_t knh_NameSpace_getFuncClass(Ctx *ctx, knh_NameSpace_t *ns, knh_methodn_t mn);
knh_type_t knh_NameSpace_gettype(Ctx *ctx, knh_NameSpace_t *ns, knh_bytes_t name);
knh_type_t knh_NameSpace_tagcid(Ctx *ctx, knh_NameSpace_t *o, knh_class_t cid, knh_bytes_t tag);
knh_bool_t knh_loadPackage(Ctx *ctx, knh_bytes_t path);
knh_bool_t knh_eval(Ctx *ctx, knh_InputStream_t *in, knh_type_t reqt, knh_Array_t *resultsNULL);
knh_bool_t knh_load(Ctx *ctx, knh_bytes_t path, knh_type_t reqt, knh_Array_t *resultsNULL);
int konoha_load(konoha_t konoha, knh_bytes_t path, int isCompileOnly);
void knh_BasicBlock_optimize(Ctx *ctx, knh_BasicBlock_t *bb);
knh_Token_t* new_Token(Ctx *ctx, knh_flag_t flag, knh_term_t tt);
void knh_Stmt_swap(Ctx *ctx, knh_Stmt_t *stmt, size_t n, size_t m);
void knh_Stmt_trimToSize(Ctx *ctx, knh_Stmt_t *stmt, size_t n);
void knh_Stmt_toERR(Ctx *ctx, knh_Stmt_t *stmt, knh_Term_t *tm);
knh_Stmt_t* new_Stmt2(Ctx *ctx, knh_term_t stt, ...);
void knh_write_token(Ctx *ctx,  knh_OutputStream_t *w, knh_Token_t *tk);
void knh_dump_token(Ctx *ctx, knh_OutputStream_t *w, knh_Token_t *tk);
void knh_dump_stmt(Ctx *ctx, knh_OutputStream_t *w, knh_Stmt_t *stmt, int isNEXT);
knh_Token_t *new_TokenCID(Ctx *ctx, knh_class_t cid);
Object* knh_getConstPools(Ctx *ctx, void *p);
knh_Term_t *knh_Stmt_add_(Ctx *ctx, knh_Stmt_t *stmt, ...);
knh_Token_t *new_TokenMN(Ctx *ctx, knh_methodn_t mn);
knh_Stmt_t *knh_Stmt_tail(Ctx *ctx, knh_Stmt_t *o);
knh_Stmt_t *knh_InputStream_parseStmt(Ctx *ctx, knh_InputStream_t *in);
knh_Stmt_t *knh_Token_parseStmt(Ctx *ctx, knh_Token_t *tk);
knh_bool_t knh_String_parseFMT(Ctx *ctx, knh_String_t *fmt, knh_Array_t *a, knh_uri_t uri, int line);
void knh_code_thread(Ctx *ctx, knh_opline_t *pc, void **codeaddr, void **codeend);
void knh_code_thread(Ctx *ctx, knh_opline_t *pc, void **codeaddr, void **codeend);
void knh_Gamma_perror(Ctx *ctx, int pe, const char *fmt, ...);
knh_Term_t* knh_Token_toTYPED(Ctx *ctx, knh_Token_t *tk, knh_term_t tt, knh_type_t type, knh_short_t nn);
knh_Token_t* new_TokenTYPED(Ctx *ctx, knh_term_t tt, knh_type_t type, knh_short_t nn);
knh_bool_t knh_StmtMETA_is(Ctx *ctx, knh_Stmt_t *stmt, knh_bytes_t name);
knh_class_t knh_Token_getcid(Ctx *ctx, knh_Token_t *tk, knh_class_t defc);
knh_class_t knh_class_parent(Ctx *ctx, knh_class_t c1, knh_class_t c2);
int knh_StmtITR_scriptTyping(Ctx *ctx, knh_Stmt_t *stmtITR, knh_type_t reqt);
knh_bool_t knh_Method_typing(Ctx *ctx, knh_Method_t *mtd, knh_Stmt_t *stmtP, knh_Stmt_t *stmtB);
knh_bool_t knh_Formatter_typing(Ctx *ctx, knh_Method_t *mtd, knh_Stmt_t *stmtP, knh_Stmt_t *stmtB);
void knh_Stmt_scriptAsm(Ctx *ctx, knh_Stmt_t *stmt);
void knh_opcode_check(void);
void knh_opcode_stat(Ctx *ctx);
void knh_opcode_count(Ctx *ctx, knh_opline_t *c);
const char *knh_opcode_tochar(knh_opcode_t opcode);
knh_bool_t knh_opcode_usedef(knh_opcode_t opcode, int i);
size_t knh_opcode_size(knh_opcode_t opcode);
knh_bool_t knh_opcode_hasjump(knh_opcode_t opcode);
void knh_opline_traverse(Ctx *ctx, knh_opline_t *c, knh_Ftraverse ftr);
void knh_opcode_dump(Ctx *ctx, knh_opline_t *c, knh_OutputStream_t *w, knh_opline_t *pc_start);
void knh_opcode_idxshift(knh_opline_t *c, int idxshift);
knh_opline_t* knh_VirtualMachine_run(Ctx *ctx, knh_sfp_t *sfp0, knh_opline_t *pc0);
knh_bytes_t knh_bytes_skipPATHHEAD(knh_bytes_t path);
knh_StreamDSPI_t *knh_getStreamDSPI(Ctx *ctx, knh_bytes_t path);
knh_QueryDSPI_t *knh_getQueryDSPI(Ctx *ctx, knh_bytes_t path);
void knh_loadDriver(Ctx *ctx);
METHOD Bytes_getSize(Ctx *ctx, knh_sfp_t *sfp, long rix);
METHOD Map_getSize(Ctx *ctx, knh_sfp_t *sfp, long rix);
void DEFAULT_init(Ctx *ctx, Object *o);
void ABSTRACT_init(Ctx *ctx, Object *o);
void DEFAULT_initcopy(Ctx *ctx, Object *dst, Object *src);
void DEFAULT_traverse(Ctx *ctx, Object *o, knh_Ftraverse ftr);
void DEFAULT_free(Ctx *ctx, Object *o);
int DEFAULT_compareTo(Ctx *ctx, Object *o1, Object *o2);
knh_String_t* DEFAULT_getkey(Ctx *ctx, knh_sfp_t *sfp);
knh_hashcode_t DEFAULT_hashCode(Ctx *ctx, knh_sfp_t *sfp);
knh_Translator_t* DEFAULT_findTransNULL(Ctx *ctx, knh_class_t scid, knh_class_t tcid);
void DEFAULT_checkin(Ctx *ctx, Object *o);
void DEFAULT_checkout(Ctx *ctx, Object *o);
void DEFAULT_lock(Ctx *ctx, Object *o);
void DEFAULT_unlock(Ctx *ctx, Object *o);
knh_bool_t knh_Regex_isSTRREGEX(knh_Regex_t *re);
knh_Term_t *knh_Stmt_done(Ctx *ctx, knh_Stmt_t *o);
void knh_loadSystemStructData(Ctx *ctx, const knh_PackageLoaderAPI_t *kapi);
void knh_loadSystemString(Ctx *ctx);
void knh_loadSystemData(Ctx *ctx, const knh_PackageLoaderAPI_t *kapi);
void knh_loadSystemMethod(Ctx *ctx, const knh_PackageLoaderAPI_t *kapi);
size_t knh_good_entrysize(size_t hsize, size_t wsize, size_t n);
knh_hmem_t* knh_hmalloc(Ctx *ctx, size_t wsize, size_t n);
knh_hmem_t* knh_hrealloc(Ctx *ctx, knh_hmem_t *p, size_t newn);
knh_hmem_t* knh_hgrow(Ctx *ctx, knh_hmem_t *p);
void knh_hfree(Ctx *ctx, knh_hmem_t *p);
void knh_Array_grow(Ctx *ctx, knh_Array_t *a, size_t newsize, size_t reqsize);
void knh_Array_initAPI(Ctx *ctx, knh_Array_t *a);
knh_Array_t* new_Array0(Ctx *ctx, size_t capacity);
void knh_Array_clear(Ctx *ctx, knh_Array_t *a, size_t n);
size_t knh_good_size(size_t ss);
knh_Bytes_t* new_Bytes(Ctx *ctx, size_t capacity);
void knh_Bytes_clear(knh_Bytes_t *ba, size_t pos);
void knh_Bytes_ensureSize(Ctx *ctx, knh_Bytes_t *ba, size_t len);
const char *knh_Bytes_ensureZero(Ctx *ctx, knh_Bytes_t *ba);
void knh_Bytes_putc(Ctx *ctx, knh_Bytes_t *ba, int ch);
void knh_Bytes_unputc(knh_Bytes_t *ba);
void knh_Bytes_write(Ctx *ctx, knh_Bytes_t *ba, knh_bytes_t t);
knh_String_t *knh_cwb_newString(Ctx *ctx, knh_cwb_t *cwb);
const char *knh_getStructName(Ctx *ctx, knh_class_t bcid);
knh_Class_t *new_Type(Ctx *ctx, knh_type_t type);
void knh_write_sname(Ctx *ctx, knh_OutputStream_t *w, knh_class_t cid);
void knh_write_cid(Ctx *ctx, knh_OutputStream_t *w, knh_class_t cid);
void knh_setClassName(Ctx *ctx, knh_class_t cid, knh_String_t *lname, knh_String_t *snameNULL);
knh_class_t knh_getcid(Ctx *ctx, knh_bytes_t lname);
const char *knh_ClassTBL_CLASS__(Ctx *ctx, knh_class_t cid);
void knh_setClassDefaultValue_(Ctx *ctx, knh_class_t cid, Object *value, knh_Fdefnull f);
Object *knh_getClassDefaultValue(Ctx *ctx, knh_class_t cid);
knh_index_t knh_Class_indexOfField(Ctx *ctx, knh_class_t cid, knh_fieldn_t fn);
knh_index_t knh_Class_queryField(Ctx *ctx, knh_class_t cid, knh_fieldn_t fnq);
knh_fields_t *knh_Class_fieldAt(Ctx *ctx, knh_class_t cid, size_t n);
knh_bool_t knh_class_instanceof(Ctx *ctx, knh_class_t scid, knh_class_t tcid);
int knh_class_isGenerics(Ctx *ctx, knh_class_t cid);
knh_class_t knh_class_p(Ctx *ctx, knh_class_t cid, size_t n);
void knh_write_typeF(Ctx *ctx, knh_OutputStream_t *w, knh_type_t type, knh_Fwritecid f);
knh_class_t knh_addGenericsClass(Ctx *ctx, knh_class_t cid, knh_class_t bcid, knh_ParamArray_t *pa);
knh_class_t knh_class_Generics(Ctx *ctx, knh_class_t bcid, knh_ParamArray_t *pa);
knh_class_t knh_class_P1(Ctx *ctx, knh_class_t bcid, knh_type_t p1);
knh_bool_t knh_ParamArray_hasTypeVar(knh_ParamArray_t *pa);
void knh_ParamArray_tocid(Ctx *ctx, knh_ParamArray_t *pa, knh_class_t this_cid, knh_ParamArray_t *npa);
knh_class_t knh_type_tocid(Ctx *ctx, knh_type_t ptype, knh_class_t this_cid);
const char *knh_TYPE__(Ctx *ctx, knh_type_t type);
knh_class_t new_ClassId(Ctx *ctx);
void knh_expandEventTBL(Ctx *ctx);
void knh_Object_finalSweep(Ctx *ctx, Object *o);
void knh_Context_traverse(Ctx *ctx, knh_Context_t *o, knh_Ftraverse ftr);
void knh_Context_free(Ctx *ctx, knh_Context_t* ctxo);
void knh_traverseAll(Ctx* ctx, knh_Ftraverse ftr);
const char* knh_sfile(const char *file);
const char* LOG__(int p);
void dbg_p(const char *file, const char *func, int line, const char *fmt, ...);
void todo_p(const char *file, const char *func, int line, const char *fmt, ...);
void knh_write_uline(Ctx *ctx, knh_OutputStream_t *w, knh_uri_t uri, long line);
void knh_exit(Ctx *ctx, int status);
void knh_throw(Ctx *ctx, knh_sfp_t *sfp, long start);
void knh_makeEvidence(Ctx *ctx, const char *ns, const char *event, int p, const char *fmt, ...);
void knh_stack_perror(Ctx *ctx, knh_sfp_t *sfp, const char *ns, const char *event);
void SYSLOG_Halt(Ctx *ctx, knh_sfp_t *sfp, const char *msg);
void SYSLOG_Arithmetic(Ctx *ctx, knh_sfp_t *sfp, const char *msg);
void SYSLOG_OutOfIndex(Ctx *ctx, knh_sfp_t *sfp, knh_int_t n, size_t max);
void SYSLOG_NoSuchMethod(Ctx *ctx, knh_sfp_t *sfp, knh_class_t cid, knh_methodn_t mn);
void SYSLOG_ParamTypeError(Ctx *ctx, knh_sfp_t *sfp, size_t n, knh_methodn_t mn, knh_class_t reqt, knh_class_t cid);
void knh_foundKonohaStyle(Ctx *ctx, size_t score);
void knh_vperror(Ctx *ctx, knh_uri_t uri, int line, int pe, const char *fmt, va_list ap);
void knh_perror(Ctx *ctx, knh_uri_t uri, int line, int pe, const char *fmt, ...);
const knh_ExportsAPI_t *knh_getExportsAPI(void);
const knh_PackageLoaderAPI_t* knh_getPackageAPI(void);
void knh_Iterator_close(Ctx *ctx, knh_Iterator_t *it);
knh_DictMap_t* new_DictMap0(Ctx *ctx, size_t capacity);
knh_DictCaseMap_t* new_DictCaseMap0(Ctx *ctx, size_t capacity);
knh_DictSet_t* new_DictSet0(Ctx *ctx, size_t capacity);
knh_DictCaseSet_t* new_DictCaseSet0(Ctx *ctx, size_t capacity);
knh_String_t* knh_DictMap_keyAt(knh_DictMap_t *m, size_t n);
Object* knh_DictMap_valueAt(knh_DictMap_t *m, size_t n);
knh_String_t* knh_DictCaseMap_keyAt(knh_DictCaseMap_t *m, size_t n);
Object* knh_DictCaseMap_valueAt(knh_DictCaseMap_t *m, size_t n);
knh_String_t* knh_DictSet_keyAt(knh_DictSet_t *m, size_t n);
knh_uintptr_t knh_DictSet_valueAt(knh_DictSet_t *m, size_t n);
knh_String_t* knh_DictCaseSet_keyAt(knh_DictCaseSet_t *m, size_t n);
knh_uintptr_t knh_DictCaseSet_valueAt(knh_DictCaseSet_t *m, size_t n);
knh_index_t knh_DictMap_index(knh_DictMap_t *m, knh_bytes_t key);
knh_index_t knh_DictCaseMap_index(knh_DictCaseMap_t *m, knh_bytes_t key);
knh_index_t knh_DictSet_index(knh_DictSet_t *m, knh_bytes_t key);
knh_index_t knh_DictCaseSet_index(knh_DictCaseSet_t *m, knh_bytes_t key);
Object *knh_DictMap_getNULL(Ctx *ctx, knh_DictMap_t *m, knh_bytes_t key);
Object *knh_DictCaseMap_getNULL(Ctx *ctx, knh_DictCaseMap_t *m, knh_bytes_t key);
knh_uintptr_t knh_DictSet_get(Ctx *ctx, knh_DictSet_t *m, knh_bytes_t key);
knh_uintptr_t knh_DictCaseSet_get(Ctx *ctx, knh_DictCaseSet_t *m, knh_bytes_t key);
void knh_DictMap_set_(Ctx *ctx, knh_DictMap_t *m, knh_String_t *key, Any *v);
void knh_DictCaseMap_set_(Ctx *ctx, knh_DictCaseMap_t *m, knh_String_t *key, Any *v);
void knh_DictSet_append(Ctx *ctx, knh_DictSet_t *m, knh_String_t *key, knh_uintptr_t n);
void knh_DictSet_set(Ctx *ctx, knh_DictSet_t *m, knh_String_t *key, knh_uintptr_t n);
void knh_DictCaseSet_set(Ctx *ctx, knh_DictCaseSet_t *m, knh_String_t *key, knh_uintptr_t n);
void knh_DictCaseSet_append(Ctx *ctx, knh_DictCaseSet_t *m, knh_String_t *key, knh_uintptr_t n);
void knh_loadDefaultMapDSPI(Ctx *ctx);
void *knh_malloc(Ctx *ctx, size_t size);
void knh_free(Ctx *ctx, void *block, size_t size);
void *knh_valloc(Ctx *ctx, size_t size);
void knh_vfree(Ctx *ctx, void *block, size_t size);
void *knh_xmalloc(Ctx *ctx, size_t size);
void knh_xfree(Ctx *ctx, void *block, size_t size);
void *TRACE_malloc(Ctx *ctx, size_t size TRACEARG0);
void TRACE_free(Ctx *ctx, void *p, size_t size TRACEARG0);
knh_bool_t knh_isObject(Ctx *ctx, void *p);
void *knh_fastmalloc(Ctx *ctx, size_t size);
void knh_fastfree(Ctx *ctx, void *block, size_t size);
knh_Object_t *new_hObject_(Ctx *ctx, knh_flag_t flag, knh_class_t bcid, knh_class_t cid);
knh_Object_t *new_Object_init2(Ctx *ctx, knh_flag_t flag, knh_class_t bcid, knh_class_t cid);
void knh_Object_free(Ctx *ctx, knh_Object_t *o);
void knh_showMemoryStat(Ctx *ctx);
void knh_System_gc(Ctx *ctx);
knh_ParamArray_t *new_ParamArrayR0(Ctx *ctx, knh_type_t t);
knh_ParamArray_t *new_ParamArrayP1(Ctx *ctx, knh_type_t rtype, knh_type_t p1, knh_fieldn_t fn1);
void knh_ParamArray_add(Ctx *ctx, knh_ParamArray_t *pa, knh_param_t p);
void knh_ParamArray_radd(Ctx *ctx, knh_ParamArray_t *pa, knh_param_t p);
knh_param_t* knh_ParamArray_get(knh_ParamArray_t *pa, size_t n);
knh_type_t knh_ParamArray_getptype(knh_ParamArray_t *pa, size_t n);
knh_bool_t knh_ParamArray_equalsType(knh_ParamArray_t *pa, knh_ParamArray_t *pa2);
knh_type_t knh_ParamArray_rtype(knh_ParamArray_t *pa);
void knh_write_mn(Ctx *ctx, knh_OutputStream_t *w, knh_methodn_t mn);
knh_bool_t knh_Method_isAbstract(knh_Method_t *o);
void knh_Method_toAbstract(Ctx *ctx, knh_Method_t *mtd);
void knh_Method_setFunc(Ctx *ctx, knh_Method_t *mtd, knh_Fmethod func);
knh_Method_t* new_Method(Ctx *ctx, knh_flag_t flag, knh_class_t cid, knh_methodn_t mn, knh_Fmethod func);
void knh_Class_addMethod(Ctx *ctx, knh_class_t cid, knh_Method_t *mtd);
knh_index_t knh_Method_indexOfGetterField(knh_Method_t *o);
knh_index_t knh_Method_indexOfSetterField(knh_Method_t *o);
knh_Method_t* knh_Array_findMethodNULL(Ctx *ctx, knh_Array_t *a, knh_methodn_t mn);
knh_Method_t* knh_findMethodNULL(Ctx *ctx, knh_class_t this_cid, knh_methodn_t mn, int isGEN);
knh_Method_t *knh_lookupMethod(Ctx *ctx, knh_class_t cid, knh_methodn_t mn);
knh_Method_t *knh_getSystemFormatter(Ctx *ctx, knh_class_t cid, knh_methodn_t mn0);
const char *knh_Method_file(Ctx *ctx, knh_Method_t *mtd);
void knh_srand(knh_uint_t seed);
knh_uint_t knh_rand(void);
knh_float_t knh_float_rand(void);
int knh_Object_compareTo(Ctx *ctx, Object *o1, Object *o2);
void knh_Object_toNULL_(Ctx *ctx, Object *o);
const char* knh_cwb_ospath(Ctx *ctx, knh_cwb_t* cwb);
const char* knh_cwb_realpath(Ctx *ctx, knh_cwb_t *cwb);
knh_bool_t knh_cwb_isfile(Ctx *ctx, knh_cwb_t *cwb);
knh_bool_t knh_cwb_isdir(Ctx *ctx, knh_cwb_t *cwb);
knh_bool_t knh_cwb_parentpath(Ctx *ctx, knh_cwb_t *cwb, char *subbuf);
knh_bool_t knh_mkdir(Ctx *ctx, knh_bytes_t path);
void knh_System_initPath(Ctx *ctx, knh_System_t *o);
void *knh_cwb_dlopen(Ctx *ctx, knh_cwb_t *cwb, int isPERROR);
void *knh_dlsym(Ctx *ctx, void* hdr, const char* symbol, int isPERROR);
int knh_dlclose(Ctx *ctx, void* hdr);
const char *knh_getSystemEncoding(void);
void knh_Connection_open(Ctx *ctx, knh_Connection_t *c, knh_String_t *urn);
knh_Connection_t* new_Connection(Ctx *ctx, knh_String_t *urn);
void knh_Connection_close(Ctx *ctx, knh_Connection_t *c);
knh_bool_t knh_ResultSet_next(Ctx *ctx, knh_ResultSet_t *o);
void knh_ResultSet_close(Ctx *ctx, knh_ResultSet_t *o);
void knh_ResultSet_initTargetClass(knh_ResultSet_t *o, knh_class_t tcid);
knh_String_t *knh_ResultSet_getName(Ctx *ctx, knh_ResultSet_t *o, size_t n);
int knh_ResultSet_findColumn(Ctx *ctx, knh_ResultSet_t *o, knh_bytes_t name);
knh_type_t knh_ResultSet_get_type(Ctx *ctx, knh_ResultSet_t *o, size_t n);
void knh_ResultSet_initData(Ctx *ctx, knh_ResultSet_t *rs);
knh_int_t knh_ResultSet_getInt(Ctx *ctx, knh_ResultSet_t *o, size_t n);
knh_float_t knh_ResultSet_getFloat(Ctx *ctx, knh_ResultSet_t *o, size_t n);
knh_String_t* knh_ResultSet_getString(Ctx *ctx, knh_ResultSet_t *o, size_t n);
void konoha_init(void);
knh_bool_t knh_isSystemVerbose(void);
void knh_loadPackageList(Ctx *ctx, const char *pkglist);
knh_bytes_t knh_bytes_nsname(knh_bytes_t t);
int konoha_parseopt(konoha_t konoha, int argc, const char **argv);
int konoha_runMain(konoha_t konoha, int argc, const char **argv);
void knh_setSecurityAlertMessage(const char *msg, int isNeedFree);
void konoha_runTest(konoha_t konoha, int argc, const char **argv);
void knh_checkSecurityAlert(void);
void knh_checkSecurityManager(Ctx *ctx, knh_sfp_t *sfp);
void knh_write_intx(Ctx *ctx, knh_OutputStream_t *w, knh_Semantics_t *u, knh_int_t v);
void knh_write_floatx(Ctx *ctx, knh_OutputStream_t *w, knh_Semantics_t *u, knh_float_t v);
void knh_Semantics_reuse(Ctx *ctx, knh_Semantics_t *u, knh_class_t cid);
knh_bytes_t knh_getURNAlias(Ctx *ctx, knh_bytes_t aurn);
knh_Semantics_t *knh_getSemantics(Ctx *ctx, knh_class_t cid);
knh_class_t knh_findcidx(Ctx *ctx, knh_bytes_t lname);
knh_ExceptionHandler_t* knh_ExceptionHandler_setjmp(Ctx *ctx, knh_ExceptionHandler_t *hdr);
knh_ExceptionHandler_t *knh_ExceptionHandler_longjmp(Ctx *ctx, knh_ExceptionHandler_t *hdr);
knh_sfp_t* knh_stack_initexpand(Ctx *ctx, knh_sfp_t *sfp, size_t n);
void knh_stack_clear(Ctx *ctx, knh_sfp_t *sfp);
knh_sfp_t* knh_stack_local(Ctx *ctx, size_t n);
void knh_stack_typecheck(Ctx *ctx, knh_sfp_t *sfp, knh_Method_t *mtd, knh_opline_t *pc);
int knh_expt_isa(Ctx *ctx, knh_ebi_t eid, knh_ebi_t parent);
knh_String_t *knh_getEventName(Ctx *ctx, knh_ebi_t eid);
knh_ebi_t knh_addEvent(Ctx *ctx, knh_flag_t flag, knh_class_t eid, knh_String_t *name, knh_class_t peid);
knh_ebi_t knh_geteid(Ctx *ctx, knh_bytes_t msg, knh_ebi_t def);
knh_Exception_t* knh_Exception_setup(Ctx *ctx, knh_Exception_t *e, knh_String_t *event, knh_String_t *msg, Object *bag);
knh_Exception_t* new_Exception(Ctx *ctx, knh_String_t *msg);
void knh_Context_setThrowingException(Ctx *ctx, knh_Exception_t *e);
knh_InputStream_t* new_InputStreamDSPI(Ctx *ctx, knh_io_t fd, const knh_StreamDSPI_t *dspi);
knh_InputStream_t* new_BytesInputStream(Ctx *ctx, knh_Bytes_t *ba);
void knh_BytesInputStream_setpos(Ctx *ctx, knh_InputStream_t *in, size_t s, size_t e);
knh_InputStream_t* new_StringInputStream(Ctx *ctx, knh_String_t *str, size_t s, size_t e);
int knh_InputStream_getc(Ctx *ctx, knh_InputStream_t *in);
size_t knh_InputStream_read(Ctx *ctx, knh_InputStream_t *in, char *buf, size_t bufsiz);
knh_String_t* knh_InputStream_readLine(Ctx *ctx, knh_InputStream_t *in);
void knh_InputStream_close(Ctx *ctx, knh_InputStream_t *in);
int knh_InputStream_isClosed(Ctx *ctx, knh_InputStream_t *in);
void knh_InputStream_setCharset(Ctx *ctx, knh_InputStream_t *in, knh_StringDecoder_t *c);
knh_OutputStream_t* new_OutputStreamDSPI(Ctx *ctx, knh_io_t fd, const knh_StreamDSPI_t *dspi);
void knh_OutputStream_clear(Ctx *ctx, knh_OutputStream_t *w);
void knh_OutputStream_close(Ctx *ctx, knh_OutputStream_t *w);
int knh_OutputStream_isClosed(knh_OutputStream_t *w);
void knh_OutputStream_setCharset(Ctx *ctx, knh_OutputStream_t *w, knh_StringEncoder_t *c);
void knh_write_begin(Ctx *ctx, knh_OutputStream_t *w, int ch);
void knh_write_end(Ctx *ctx, knh_OutputStream_t *w, int ch);
void knh_write_bool(Ctx *ctx, knh_OutputStream_t *w, int b);
void knh_write__p(Ctx *ctx, knh_OutputStream_t *w, void *ptr);
void knh_write_dfmt(Ctx *ctx, knh_OutputStream_t *w, const char *fmt, knh_intptr_t n);
void knh_write_ifmt(Ctx *ctx, knh_OutputStream_t *w, const char *fmt, knh_int_t n);
void knh_write_ffmt(Ctx *ctx, knh_OutputStream_t *w, const char *fmt, knh_float_t n);
void knh_write_text(Ctx *ctx, knh_OutputStream_t *w, const char *text);
void knh_write_flag(Ctx *ctx, knh_OutputStream_t *w, knh_flag_t flag);
void knh_write_cap(Ctx *ctx, knh_OutputStream_t *w, knh_bytes_t t);
knh_bool_t knh_write_ndata(Ctx *ctx, knh_OutputStream_t *w, knh_class_t cid, knh_ndata_t d);
void knh_write_Object(Ctx *ctx, knh_OutputStream_t *w, knh_sfp_t *esp, knh_Method_t **mtdP, knh_Object_t *o);
void knh_vprintf(Ctx *ctx, knh_OutputStream_t *w, const char *fmt, va_list ap);
knh_bool_t knh_bytes_checkENCODING(knh_bytes_t s);
size_t knh_bytes_mlen(knh_bytes_t s);
knh_bytes_t knh_bytes_mofflen(knh_bytes_t m, size_t moff, size_t mlen);
knh_int_t knh_uchar_toucs4(knh_ustr_t *utf8)   /* utf8 -> ucs4 */;
char *knh_format_utf8(char *buf, size_t bufsiz, knh_uint_t ucs4);
knh_String_t *new_TEXT(Ctx *ctx, knh_class_t cid, knh_TEXT_t text);
int knh_bytes_strcasecmp(knh_bytes_t v1, knh_bytes_t v2);
knh_StringDecoder_t* new_StringDecoderNULL(Ctx *ctx, knh_bytes_t t, knh_NameSpace_t *ns);
knh_StringEncoder_t* new_StringEncoderNULL(Ctx *ctx, knh_bytes_t t, knh_NameSpace_t *ns);
knh_String_t *knh_cwb_newStringDECODE(Ctx *ctx, knh_cwb_t *cwb, knh_StringDecoder_t *c);
knh_String_t* knh_getPropertyNULL(Ctx *ctx, knh_bytes_t key);
void knh_setProperty(Ctx *ctx, knh_String_t *key, Any *value);
Object *knh_getClassConstNULL(Ctx *ctx, knh_class_t cid, knh_bytes_t name);
int knh_addClassConst(Ctx *ctx, knh_class_t cid, knh_String_t* name, Object *value);
Object *knh_getSystemConst(Ctx *ctx, int n);
knh_fieldn_t knh_addname(Ctx *ctx, knh_String_t *s, knh_Fdictset f);
knh_NameInfo_t *knh_getnameinfo(Ctx *ctx, knh_fieldn_t fn);
knh_String_t *knh_getFieldName(Ctx *ctx, knh_fieldn_t fn);
knh_fieldn_t knh_getfnq(Ctx *ctx, knh_bytes_t tname, knh_fieldn_t def);
knh_methodn_t knh_getmn(Ctx *ctx, knh_bytes_t tname, knh_methodn_t def);
const char* knh_getmnname(Ctx *ctx, knh_methodn_t mn);
knh_uri_t knh_getURI(Ctx *ctx, knh_bytes_t t);
knh_String_t *knh_getURN(Ctx *ctx, knh_uri_t uri);
void knh_addDSPI(Ctx *ctx, const char *scheme, const knh_DSPI_t* p);
const knh_DSPI_t *knh_getDSPINULL(Ctx *ctx, int type, knh_bytes_t path);
knh_PathDSPI_t *knh_NameSpace_getPathDSPINULL(Ctx *ctx, knh_NameSpace_t *ns, knh_bytes_t path);
knh_ConvDSPI_t *knh_NameSpace_getConvTODSPINULL(Ctx *ctx, knh_NameSpace_t *ns, knh_bytes_t path);
knh_ConvDSPI_t *knh_NameSpace_getConvFROMDSPINULL(Ctx *ctx, knh_NameSpace_t *ns, knh_bytes_t path);
knh_thread_t knh_thread_self(void);
int knh_thread_create(Ctx *ctx, knh_thread_t *thread, void *attr, knh_Fthread fgo, void * arg);
int knh_thread_detach(Ctx *ctx, knh_thread_t th);
int knh_thread_join(Ctx *ctx, knh_thread_t thread, void **ret);
int knh_mutex_init(knh_mutex_t *m);
int knh_mutex_lock(knh_mutex_t *m);
int knh_mutex_trylock(knh_mutex_t *m);
int knh_mutex_unlock(knh_mutex_t *m);
int knh_mutex_destroy(knh_mutex_t *m);
knh_mutex_t *knh_mutex_malloc(Ctx *ctx);
void knh_mutex_free(Ctx *ctx, knh_mutex_t *m);
int knh_thread_key_create(knh_thread_key_t *key);
int knh_thread_setspecific(knh_thread_key_t key, const void *data);
void* knh_thread_getspecific(knh_thread_key_t key);
int knh_thread_key_delete(knh_thread_key_t key);
void knh_addTranslatorFunc(Ctx *ctx, knh_flag_t flag, knh_type_t stype, knh_type_t ttype, knh_Ftranslator ftcast, Object *mapdata);
knh_bool_t knh_Translator_isNoSuchMapping(knh_Translator_t *trl);
knh_Translator_t *knh_findTranslatorNULL(Ctx *ctx, knh_class_t scid, knh_class_t tcid, int isGEN);

#ifdef __cplusplus
}
#endif

// THIS FILE WAS AUTOMATICALLY GENERATED. DON'T EDIT.

