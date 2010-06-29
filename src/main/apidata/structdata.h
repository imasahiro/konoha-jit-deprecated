// THIS FILE WAS AUTOMATICALLY GENERATED

/* ======================================================================== */
/* flag */

static METHOD Object_isRelease(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_Object_isRelease((knh_Object_t*)sfp[0].o));
}

static METHOD Object_isDebug(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(!(knh_Object_isRelease((knh_Object_t*)sfp[0].o)));
}

static METHOD Object_isImmutable(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_Object_isImmutable((knh_Object_t*)sfp[0].o));
}

static METHOD Object_isModified(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_Object_isModified((knh_Object_t*)sfp[0].o));
}

static METHOD Object_setModified(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Object_setModified((knh_Object_t*)sfp[0].o, sfp[1].bvalue);
	RETURNb_(sfp[1].bvalue);
}

static METHOD Object_isShared(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_Object_isShared((knh_Object_t*)sfp[0].o));
}

static METHOD Object_setShared(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Object_setShared((knh_Object_t*)sfp[0].o, sfp[1].bvalue);
	RETURNb_(sfp[1].bvalue);
}

static METHOD String_isASCII(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_String_isASCII((knh_String_t*)sfp[0].o));
}

static METHOD DictMap_isIgnoreCase(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_DictMap_isIgnoreCase((knh_DictMap_t*)sfp[0].o));
}

static METHOD DictSet_isIgnoreCase(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_DictSet_isIgnoreCase((knh_DictSet_t*)sfp[0].o));
}

static METHOD Class_isRelease(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_class_isRelease(knh_Class_cid(sfp[0].c)));
}

static METHOD Class_isDebug(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(!(knh_class_isRelease(knh_Class_cid(sfp[0].c))));
}

static METHOD Class_isImmutable(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_class_isImmutable(knh_Class_cid(sfp[0].c)));
}

static METHOD Class_isMetaClass(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_class_isMetaClass(knh_Class_cid(sfp[0].c)));
}

static METHOD Class_isPrivate(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_class_isPrivate(knh_Class_cid(sfp[0].c)));
}

static METHOD Class_isPublic(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(!(knh_class_isPrivate(knh_Class_cid(sfp[0].c))));
}

static METHOD Class_isFinal(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_class_isFinal(knh_Class_cid(sfp[0].c)));
}

static METHOD Class_isSingleton(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_class_isSingleton(knh_Class_cid(sfp[0].c)));
}

static METHOD Class_isUnique(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_class_isUnique(knh_Class_cid(sfp[0].c)));
}

static METHOD Class_isInterface(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_class_isInterface(knh_Class_cid(sfp[0].c)));
}

static METHOD Method_isPrivate(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_Method_isPrivate((knh_Method_t*)sfp[0].o));
}

static METHOD Method_isPublic(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(!(knh_Method_isPrivate((knh_Method_t*)sfp[0].o)));
}

static METHOD Method_isVirtual(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_Method_isVirtual((knh_Method_t*)sfp[0].o));
}

static METHOD Method_isFinal(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(!(knh_Method_isVirtual((knh_Method_t*)sfp[0].o)));
}

static METHOD Method_isConst(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_Method_isConst((knh_Method_t*)sfp[0].o));
}

static METHOD Method_isStatic(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_Method_isStatic((knh_Method_t*)sfp[0].o));
}

static METHOD Translator_isInterface(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_Translator_isInterface((knh_Translator_t*)sfp[0].o));
}

static METHOD Translator_isSignificant(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_Translator_isSignificant((knh_Translator_t*)sfp[0].o));
}

static METHOD Translator_isSemantic(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_Translator_isSemantic((knh_Translator_t*)sfp[0].o));
}

static METHOD Translator_isTotal(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_Translator_isTotal((knh_Translator_t*)sfp[0].o));
}

static METHOD Translator_isPartial(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(!(knh_Translator_isTotal((knh_Translator_t*)sfp[0].o)));
}

static METHOD Translator_isLossLess(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_Translator_isLossLess((knh_Translator_t*)sfp[0].o));
}

static METHOD Translator_isFinal(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_Translator_isFinal((knh_Translator_t*)sfp[0].o));
}

static METHOD Translator_isConst(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_Translator_isConst((knh_Translator_t*)sfp[0].o));
}

static METHOD Translator_isTemporal(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(!(knh_Translator_isConst((knh_Translator_t*)sfp[0].o)));
}

static METHOD Exception_isLogging(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_Exception_isLogging((knh_Exception_t*)sfp[0].o));
}

static METHOD OutputStream_isAutoFlush(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_OutputStream_isAutoFlush((knh_OutputStream_t*)sfp[0].o));
}

static METHOD OutputStream_setAutoFlush(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_OutputStream_setAutoFlush((knh_OutputStream_t*)sfp[0].o, sfp[1].bvalue);
	RETURNb_(sfp[1].bvalue);
}

static METHOD OutputStream_isStoringBuffer(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_OutputStream_isStoringBuffer((knh_OutputStream_t*)sfp[0].o));
}

static METHOD OutputStream_setStoringBuffer(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_OutputStream_setStoringBuffer((knh_OutputStream_t*)sfp[0].o, sfp[1].bvalue);
	RETURNb_(sfp[1].bvalue);
}

static METHOD Context_isStrict(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_Context_isStrict((knh_Context_t*)sfp[0].o));
}

static METHOD Context_setStrict(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Context_setStrict((knh_Context_t*)sfp[0].o, sfp[1].bvalue);
	RETURNb_(sfp[1].bvalue);
}

static METHOD Context_isDebug(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_Context_isDebug((knh_Context_t*)sfp[0].o));
}

static METHOD Context_setDebug(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Context_setDebug((knh_Context_t*)sfp[0].o, sfp[1].bvalue);
	RETURNb_(sfp[1].bvalue);
}

static METHOD Context_isVerbose(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_Context_isVerbose((knh_Context_t*)sfp[0].o));
}

static METHOD Context_setVerbose(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_Context_setVerbose((knh_Context_t*)sfp[0].o, sfp[1].bvalue);
	RETURNb_(sfp[1].bvalue);
}

static METHOD Context_isInteractive(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNb_(knh_Context_isInteractive((knh_Context_t*)sfp[0].o));
}

/* ======================================================================== */
/* data */

static char * StringData0[] = {
	"", /* TS_EMPTY */
	"\t", /* TS_TAB */
	"1", /* TS_ONE */
	""KONOHA_OS_LINEFEED"", /* TS_EOL */
	"BEGIN", /* TS_BEGIN */
	"END", /* TS_END */
	"true", /* TS_true */
	"false", /* TS_false */
	"null", /* TS_null */
	"void", /* TS_void */
	"main", /* TS_main */
	""K_ENCODING"", /* TS_ENCODING */
	"/dev/null", /* TS_DEVNULL */
	"/dev/stdin", /* TS_DEVSTDIN */
	"/dev/stderr", /* TS_DEVSTDERR */
	"/dev/stdout", /* TS_DEVSTDOUT */
	"@label", /* TS_ATlabel */
	"Assertion!!", /* TS_AssertionException */
	"(", /* TS_LP */
	")", /* TS_RP */
	"{", /* TS_LB */
	"}", /* TS_RB */
	"[", /* TS_LS */
	"]", /* TS_RS */
	NULL
};

static knh_data_t StructData0[] = {
	DATA_STRUCT, CLASS_Object, _DATA(&ObjectSPI), FLAG_Object,
	DATA_STRUCT, CLASS_Boolean, _DATA(&BooleanSPI), FLAG_Boolean,
	DATA_STRUCT, CLASS_Number, _DATA(&NumberSPI), FLAG_Number,
	DATA_STRUCT, CLASS_Int, _DATA(&IntSPI), FLAG_Int,
	DATA_STRUCT, CLASS_Float, _DATA(&FloatSPI), FLAG_Float,
	DATA_STRUCT, CLASS_String, _DATA(&StringSPI), FLAG_String,
	DATA_STRUCT, CLASS_Bytes, _DATA(&BytesSPI), FLAG_Bytes,
	DATA_STRUCT, CLASS_Any, _DATA(&AnySPI), FLAG_Any,
	DATA_STRUCT, CLASS_Iterator, _DATA(&IteratorSPI), FLAG_Iterator,
	DATA_STRUCT, CLASS_Tuple, _DATA(&TupleSPI), FLAG_Tuple,
	DATA_STRUCT, CLASS_Range, _DATA(&RangeSPI), FLAG_Range,
	DATA_STRUCT, CLASS_Array, _DATA(&ArraySPI), FLAG_Array,
	DATA_STRUCT, CLASS_DictMap, _DATA(&DictMapSPI), FLAG_DictMap,
	DATA_STRUCT, CLASS_DictSet, _DATA(&DictSetSPI), FLAG_DictSet,
	DATA_STRUCT, CLASS_Map, _DATA(&MapSPI), FLAG_Map,
	DATA_STRUCT, CLASS_Class, _DATA(&ClassSPI), FLAG_Class,
	DATA_STRUCT, CLASS_ParamArray, _DATA(&ParamArraySPI), FLAG_ParamArray,
	DATA_STRUCT, CLASS_Method, _DATA(&MethodSPI), FLAG_Method,
	DATA_STRUCT, CLASS_Translator, _DATA(&TranslatorSPI), FLAG_Translator,
	DATA_STRUCT, CLASS_Func, _DATA(&FuncSPI), FLAG_Func,
	DATA_STRUCT, CLASS_Thunk, _DATA(&ThunkSPI), FLAG_Thunk,
	DATA_STRUCT, CLASS_Exception, _DATA(&ExceptionSPI), FLAG_Exception,
	DATA_STRUCT, CLASS_ExceptionHandler, _DATA(&ExceptionHandlerSPI), FLAG_ExceptionHandler,
	DATA_STRUCT, CLASS_Regex, _DATA(&RegexSPI), FLAG_Regex,
	DATA_STRUCT, CLASS_Converter, _DATA(&ConverterSPI), FLAG_Converter,
	DATA_STRUCT, CLASS_StringEncoder, _DATA(&StringEncoderSPI), FLAG_StringEncoder,
	DATA_STRUCT, CLASS_StringDecoder, _DATA(&StringDecoderSPI), FLAG_StringDecoder,
	DATA_STRUCT, CLASS_StringConverter, _DATA(&StringConverterSPI), FLAG_StringConverter,
	DATA_STRUCT, CLASS_Semantics, _DATA(&SemanticsSPI), FLAG_Semantics,
	DATA_STRUCT, CLASS_InputStream, _DATA(&InputStreamSPI), FLAG_InputStream,
	DATA_STRUCT, CLASS_OutputStream, _DATA(&OutputStreamSPI), FLAG_OutputStream,
	DATA_STRUCT, CLASS_Channel, _DATA(&ChannelSPI), FLAG_Channel,
	DATA_STRUCT, CLASS_Connection, _DATA(&ConnectionSPI), FLAG_Connection,
	DATA_STRUCT, CLASS_ResultSet, _DATA(&ResultSetSPI), FLAG_ResultSet,
	DATA_STRUCT, CLASS_Script, _DATA(&ScriptSPI), FLAG_Script,
	DATA_STRUCT, CLASS_NameSpace, _DATA(&NameSpaceSPI), FLAG_NameSpace,
	DATA_STRUCT, CLASS_Package, _DATA(&PackageSPI), FLAG_Package,
	DATA_STRUCT, CLASS_System, _DATA(&SystemSPI), FLAG_System,
	DATA_STRUCT, CLASS_Context, _DATA(&ContextSPI), FLAG_Context,
	DATA_STRUCT, CLASS_Term, _DATA(&TermSPI), FLAG_Term,
	DATA_STRUCT, CLASS_Token, _DATA(&TokenSPI), FLAG_Token,
	DATA_STRUCT, CLASS_Stmt, _DATA(&StmtSPI), FLAG_Stmt,
	DATA_STRUCT, CLASS_Gamma, _DATA(&GammaSPI), FLAG_Gamma,
	DATA_STRUCT, CLASS_KLRInst, _DATA(&KLRInstSPI), FLAG_KLRInst,
	DATA_STRUCT, CLASS_KLRCode, _DATA(&KLRCodeSPI), FLAG_KLRCode,
	0
};

static knh_data_t ClassData0[] = {
	DATA_CLASS, _DATA("konoha.Object"), FLAG_Object, CLASS_ObjectField, CLASS_Object, 25 + 7 /*1*/,
	DATA_CLASS, _DATA("konoha.Boolean"), FLAG_Boolean, CLASS_Boolean, CLASS_Object, 1 + 4 /*0*/,
	DATA_CLASS, _DATA("konoha.Number"), FLAG_Number, CLASS_Number, CLASS_Object, 0 + 0 /*0*/,
	DATA_CLASS, _DATA("konoha.Int"), FLAG_Int, CLASS_Int, CLASS_Number, 20 + 10 /*1*/,
	DATA_CLASS, _DATA("konoha.Float"), FLAG_Float, CLASS_Float, CLASS_Number, 15 + 7 /*1*/,
	DATA_CLASS, _DATA("konoha.String"), FLAG_String, CLASS_String, CLASS_Object, 26 + 3 /*4*/,
	DATA_CLASS, _DATA("konoha.Bytes"), FLAG_Bytes, CLASS_Bytes, CLASS_Object, 13 + 3 /*1*/,
	DATA_CLASS, _DATA("konoha.Any"), FLAG_Any, CLASS_Any, CLASS_Object, 6 + 2 /*0*/,
	DATA_CLASS, _DATA("konoha.Iterator"), FLAG_Iterator, CLASS_Iterator, CLASS_Object, 0 + 1 /*1*/,
	DATA_CLASS, _DATA("konoha.Tuple"), FLAG_Tuple, CLASS_Tuple, CLASS_Object, 5 + 1 /*0*/,
	DATA_CLASS, _DATA("konoha.Range"), FLAG_Range, CLASS_Range, CLASS_Object, 5 + 1 /*1*/,
	DATA_CLASS, _DATA("konoha.Array"), FLAG_Array, CLASS_Array, CLASS_Object, 14 + 2 /*1*/,
	DATA_CLASS, _DATA("konoha.DictMap"), FLAG_DictMap, CLASS_DictMap, CLASS_Object, 14 + 2 /*2*/,
	DATA_CLASS, _DATA("konoha.DictSet"), FLAG_DictSet, CLASS_DictSet, CLASS_Object, 1 + 1 /*0*/,
	DATA_CLASS, _DATA("konoha.Map"), FLAG_Map, CLASS_Map, CLASS_Object, 0 + 0 /*0*/,
	DATA_CLASS, _DATA("konoha.Class"), FLAG_Class, CLASS_Class, CLASS_Object, 12 + 4 /*0*/,
	DATA_CLASS, _DATA("konoha.ParamArray"), FLAG_ParamArray, CLASS_ParamArray, CLASS_Object, 0 + 0 /*0*/,
	DATA_CLASS, _DATA("konoha.Method"), FLAG_Method, CLASS_Method, CLASS_Object, 10 + 3 /*0*/,
	DATA_CLASS, _DATA("konoha.Translator"), FLAG_Translator, CLASS_Translator, CLASS_Object, 9 + 1 /*0*/,
	DATA_CLASS, _DATA("konoha.Func"), FLAG_Func, CLASS_Func, CLASS_Object, 2 + 1 /*0*/,
	DATA_CLASS, _DATA("konoha.Thunk"), FLAG_Thunk, CLASS_Thunk, CLASS_Object, 0 + 0 /*0*/,
	DATA_CLASS, _DATA("konoha.Exception"), FLAG_Exception, CLASS_Exception, CLASS_Object, 3 + 4 /*0*/,
	DATA_CLASS, _DATA("konoha.ExceptionHandler"), FLAG_ExceptionHandler, CLASS_ExceptionHandler, CLASS_Object, 0 + 0 /*0*/,
	DATA_CLASS, _DATA("konoha.Regex"), FLAG_Regex, CLASS_Regex, CLASS_Object, 2 + 1 /*0*/,
	DATA_CLASS, _DATA("konoha.Converter"), FLAG_Converter, CLASS_Converter, CLASS_Object, 0 + 0 /*0*/,
	DATA_CLASS, _DATA("konoha.StringEncoder"), FLAG_StringEncoder, CLASS_StringEncoder, CLASS_Object, 0 + 0 /*0*/,
	DATA_CLASS, _DATA("konoha.StringDecoder"), FLAG_StringDecoder, CLASS_StringDecoder, CLASS_Object, 0 + 0 /*0*/,
	DATA_CLASS, _DATA("konoha.StringConverter"), FLAG_StringConverter, CLASS_StringConverter, CLASS_Object, 0 + 0 /*0*/,
	DATA_CLASS, _DATA("konoha.Semantics"), FLAG_Semantics, CLASS_Semantics, CLASS_Object, 0 + 0 /*0*/,
	DATA_CLASS, _DATA("konoha.InputStream"), FLAG_InputStream, CLASS_InputStream, CLASS_Object, 7 + 1 /*1*/,
	DATA_CLASS, _DATA("konoha.OutputStream"), FLAG_OutputStream, CLASS_OutputStream, CLASS_Object, 16 + 1 /*0*/,
	DATA_CLASS, _DATA("konoha.Channel"), FLAG_Channel, CLASS_Channel, CLASS_Object, 0 + 0 /*0*/,
	DATA_CLASS, _DATA("konoha.Connection"), FLAG_Connection, CLASS_Connection, CLASS_Object, 4 + 0 /*0*/,
	DATA_CLASS, _DATA("konoha.ResultSet"), FLAG_ResultSet, CLASS_ResultSet, CLASS_Object, 8 + 0 /*0*/,
	DATA_CLASS, _DATA("konoha.Script"), FLAG_Script, CLASS_Script, CLASS_Object, 0 + 1 /*0*/,
	DATA_CLASS, _DATA("konoha.NameSpace"), FLAG_NameSpace, CLASS_NameSpace, CLASS_Object, 1 + 0 /*0*/,
	DATA_CLASS, _DATA("konoha.Package"), FLAG_Package, CLASS_Package, CLASS_Object, 0 + 0 /*0*/,
	DATA_CLASS, _DATA("konoha.System"), FLAG_System, CLASS_System, CLASS_Object, 11 + 0 /*0*/,
	DATA_CLASS, _DATA("konoha.Context"), FLAG_Context, CLASS_Context, CLASS_Object, 10 + 0 /*0*/,
	DATA_CLASS, _DATA("konoha.Term"), FLAG_Term, CLASS_Term, CLASS_Object, 0 + 0 /*0*/,
	DATA_CLASS, _DATA("konoha.Token"), FLAG_Token, CLASS_Token, CLASS_Term, 0 + 0 /*0*/,
	DATA_CLASS, _DATA("konoha.Stmt"), FLAG_Stmt, CLASS_Stmt, CLASS_Term, 0 + 0 /*0*/,
	DATA_CLASS, _DATA("konoha.Gamma"), FLAG_Gamma, CLASS_Gamma, CLASS_Object, 0 + 0 /*0*/,
	DATA_CLASS, _DATA("konoha.KLRInst"), FLAG_KLRInst, CLASS_KLRInst, CLASS_Object, 0 + 0 /*0*/,
	DATA_CLASS, _DATA("konoha.KLRCode"), FLAG_KLRCode, CLASS_KLRCode, CLASS_Object, 0 + 0 /*0*/,
	DATA_CLASS, _DATA("konoha.Tvoid"), FLAG_Tvoid, CLASS_Any, CLASS_Any, 0 + 0 /*0*/,
	DATA_CLASS, _DATA("konoha.Tvar"), FLAG_Tvar, CLASS_Any, CLASS_Any, 0 + 0 /*0*/,
	DATA_GENERICS/*PairSS*/, CLASS_Tuple, 2, 0, TYPE_String, FN_, TYPE_String, FN_,
	DATA_GENERICS/*PairST1*/, CLASS_Tuple, 2, 0, TYPE_String, FN_, TYPE_T1, FN_,
	DATA_GENERICS/*PairT1T2*/, CLASS_Tuple, 2, 0, TYPE_T1, FN_, TYPE_T2, FN_,
	DATA_GENERICS/*Cmpr*/, CLASS_Func, 2, 1, TYPE_T1, FN_, TYPE_T1, FN_, TYPE_Int, FN_,
	DATA_GENERICS/*StringARRAY*/, CLASS_Array, 1, 0, TYPE_String, FN_,
	DATA_GENERICS/*T1ARRAY*/, CLASS_Array, 1, 0, TYPE_T1, FN_,
	DATA_GENERICS/*StringITR*/, CLASS_Iterator, 1, 0, TYPE_String, FN_,
	DATA_GENERICS/*T1ITR*/, CLASS_Iterator, 1, 0, TYPE_T1, FN_,
	DATA_GENERICS/*ThisITR*/, CLASS_Iterator, 1, 0, TYPE_This, FN_,
	DATA_EXPT, _DATA("Exception"), 0, EXPT_Exception, EXPT_Exception, 
	DATA_EXPT, _DATA("Fatal"), 0, EXPT_Fatal, EXPT_Exception, 
	DATA_EXPT, _DATA("OutOfMemory"), 0, EXPT_OutOfMemory, EXPT_Fatal, 
	DATA_EXPT, _DATA("Security"), 0, EXPT_Security, EXPT_Exception, 
	DATA_EXPT, _DATA("Null"), 0, EXPT_Null, EXPT_Exception, 
	DATA_EXPT, _DATA("OutOfIndex"), 0, EXPT_OutOfIndex, EXPT_Exception, 
	DATA_EXPT, _DATA("IO"), 0, EXPT_IO, EXPT_Exception, 
	DATA_EXPT, _DATA("Script"), 0, EXPT_Script, EXPT_Exception, 
	DATA_EXPT, _DATA("Syntax"), 0, EXPT_Syntax, EXPT_Script, 
	DATA_EXPT, _DATA("Type"), 0, EXPT_Type, EXPT_Script, 
	DATA_EXPT, _DATA("Assertion"), 0, EXPT_Assertion, EXPT_Script, 
	0
};

static knh_FieldNameData0_t FieldNameData0[] = {
	{"", FN_},
	{"abstract", FN_abstract},
	{"add", FN_add},
	{"all", FN_all},
	{"aSCII", FN_aSCII},
	{"autoFlush", FN_autoFlush},
	{"bag", FN_bag},
	{"base", FN_base},
	{"bits", FN_bits},
	{"buf", FN_buf},
	{"c", FN_c},
	{"ch", FN_ch},
	{"char", FN_char},
	{"charset", FN_charset},
	{"class", FN_class},
	{"clear", FN_clear},
	{"clearBuffer", FN_clearBuffer},
	{"close", FN_close},
	{"closed", FN_closed},
	{"concat", FN_concat},
	{"const", FN_const},
	{"convert", FN_convert},
	{"d", FN_d},
	{"data", FN_data},
	{"debug", FN_debug},
	{"decode", FN_decode},
	{"defined", FN_defined},
	{"delegate", FN_delegate},
	{"domain", FN_domain},
	{"dump", FN_dump},
	{"e", FN_e},
	{"empty", FN_empty},
	{"enc", FN_enc},
	{"encode", FN_encode},
	{"end", FN_end},
	{"endsWith", FN_endsWith},
	{"equals", FN_equals},
	{"err", FN_err},
	{"exec", FN_exec},
	{"exit", FN_exit},
	{"f", FN_f},
	{"final", FN_final},
	{"flag", FN_flag},
	{"float", FN_float},
	{"floatToIntBits", FN_floatToIntBits},
	{"flush", FN_flush},
	{"format", FN_format},
	{"format:FINAL", FN_format__FINAL},
	{"gc", FN_gc},
	{"hashCode", FN_hashCode},
	{"ignoreCase", FN_ignoreCase},
	{"immutable", FN_immutable},
	{"in", FN_in},
	{"indexOf", FN_indexOf},
	{"init", FN_init},
	{"initCapacity", FN_initCapacity},
	{"int", FN_int},
	{"interactive", FN_interactive},
	{"interface", FN_interface},
	{"intToFloatBits", FN_intToFloatBits},
	{"invoke", FN_invoke},
	{"it", FN_it},
	{"k", FN_k},
	{"key", FN_key},
	{"keys", FN_keys},
	{"lastIndexOf", FN_lastIndexOf},
	{"length", FN_length},
	{"likely", FN_likely},
	{"listProperties", FN_listProperties},
	{"logging", FN_logging},
	{"lossLess", FN_lossLess},
	{"main", FN_main},
	{"man", FN_man},
	{"metaClass", FN_metaClass},
	{"method", FN_method},
	{"mode", FN_mode},
	{"modified", FN_modified},
	{"msg", FN_msg},
	{"n", FN_n},
	{"name", FN_name},
	{"new", FN_new},
	{"new:ARRAY", FN_new__ARRAY},
	{"new:LIST", FN_new__LIST},
	{"new:MAP", FN_new__MAP},
	{"new:RANGE", FN_new__RANGE},
	{"new:TUPLE", FN_new__TUPLE},
	{"next", FN_next},
	{"notNull", FN_notNull},
	{"ns", FN_ns},
	{"null", FN_null},
	{"offset", FN_offset},
	{"option", FN_option},
	{"out", FN_out},
	{"p", FN_p},
	{"partial", FN_partial},
	{"pattern", FN_pattern},
	{"print", FN_print},
	{"println", FN_println},
	{"private", FN_private},
	{"property", FN_property},
	{"public", FN_public},
	{"push", FN_push},
	{"putc", FN_putc},
	{"query", FN_query},
	{"random", FN_random},
	{"randomSeed", FN_randomSeed},
	{"re", FN_re},
	{"read", FN_read},
	{"readLine", FN_readLine},
	{"release", FN_release},
	{"remove", FN_remove},
	{"replace", FN_replace},
	{"s", FN_s},
	{"seed", FN_seed},
	{"semantic", FN_semantic},
	{"shared", FN_shared},
	{"significant", FN_significant},
	{"singleton", FN_singleton},
	{"size", FN_size},
	{"split", FN_split},
	{"start", FN_start},
	{"startsWith", FN_startsWith},
	{"static", FN_static},
	{"status", FN_status},
	{"storingBuffer", FN_storingBuffer},
	{"strict", FN_strict},
	{"string", FN_string},
	{"substring", FN_substring},
	{"super", FN_super},
	{"temporal", FN_temporal},
	{"this", FN_this},
	{"time", FN_time},
	{"total", FN_total},
	{"trim", FN_trim},
	{"u", FN_u},
	{"unique", FN_unique},
	{"unlikely", FN_unlikely},
	{"urn", FN_urn},
	{"v", FN_v},
	{"value", FN_value},
	{"values", FN_values},
	{"vargs", FN_vargs},
	{"verbose", FN_verbose},
	{"virtual", FN_virtual},
	{"w", FN_w},
	{"write", FN_write},
	{"writeChar", FN_writeChar},
	{"writeData", FN_writeData},
	{"x", FN_x},
	{NULL}
};

static knh_data_t ParamArrayData0[] = {
	DATA_PARAM/*0*/, 0, 0, 0, 
	DATA_PARAM/*1*/, 0, 1, 0, TYPE_Int, FN_char,
	DATA_PARAM/*2*/, 0, 3, 0, TYPE_Bytes, FN_buf, TYPE_Int, FN_offset, TYPE_Int, FN_length,
	DATA_PARAM/*3*/, 0, 1, 1, TYPE_Int, FN_n, TYPE_T1, FN_return,
	DATA_PARAM/*4*/, 0, 2, 1, TYPE_Int, FN_n, TYPE_T1, FN_v, TYPE_T1, FN_return,
	DATA_PARAM/*5*/, FLAG_ParamArray_VARGs, 1, 0, TYPE_T1, FN_value,
	DATA_PARAM/*6*/, 0, 1, 1, TYPE_String, FN_urn, TYPE_This, FN_return,
	DATA_PARAM/*7*/, 0, 1, 1, TYPE_String, FN_query, TYPE_ResultSet, FN_return,
	DATA_PARAM/*8*/, 0, 1, 0, TYPE_String, FN_query,
	DATA_PARAM/*9*/, 0, 0, 1, TYPE_Int, FN_return,
	DATA_PARAM/*10*/, 0, 0, 1, TYPE_Boolean, FN_return,
	DATA_PARAM/*11*/, 0, 1, 1, TYPE_Int, FN_n, TYPE_String, FN_return,
	DATA_PARAM/*12*/, 0, 1, 1, TYPE_Any, FN_n, TYPE_Int, FN_return,
	DATA_PARAM/*13*/, 0, 1, 1, TYPE_Any, FN_n, TYPE_Float, FN_return,
	DATA_PARAM/*14*/, 0, 1, 1, TYPE_Any, FN_n, TYPE_String, FN_return,
	DATA_PARAM/*15*/, 0, 1, 1, TYPE_Any, FN_n, TYPE_Any, FN_return,
	DATA_PARAM/*16*/, 0, 1, 1, TYPE_String, FN_key, TYPE_Boolean, FN_return,
	DATA_PARAM/*17*/, 0, 1, 1, TYPE_String, FN_key, TYPE_T1, FN_return,
	DATA_PARAM/*18*/, 0, 2, 0, TYPE_String, FN_key, TYPE_T1, FN_value,
	DATA_PARAM/*19*/, 0, 1, 0, TYPE_String, FN_key,
	DATA_PARAM/*20*/, 0, 0, 1, TYPE_StringARRAY, FN_return,
	DATA_PARAM/*21*/, 0, 0, 1, TYPE_T1ARRAY, FN_return,
	DATA_PARAM/*22*/, 0, 1, 0, TYPE_OutputStream, FN_w,
	DATA_PARAM/*23*/, 0, 0, 1, TYPE_StringITR, FN_return,
	DATA_PARAM/*24*/, 0, 0, 1, TYPE_T1ITR, FN_return,
	DATA_PARAM/*25*/, 0, 0, 1, TYPE_ThisITR, FN_return,
	DATA_PARAM/*26*/, 0, 0, 1, TYPE_This, FN_return,
	DATA_PARAM/*27*/, FLAG_ParamArray_VARGs, 1, 1, TYPE_Any, FN_value, TYPE_This, FN_return,
	DATA_PARAM/*28*/, 0, 1, 1, TYPE_Int, FN_init, TYPE_Bytes, FN_return,
	DATA_PARAM/*29*/, 0, 2, 1, TYPE_Bytes, FN_buf, TYPE_StringDecoder, FN_c, TYPE_String, FN_return,
	DATA_PARAM/*30*/, 0, 3, 1, TYPE_String, FN_pattern, TYPE_String, FN_option, TYPE_NameSpace, FN_ns, TYPE_Regex, FN_return,
	DATA_PARAM/*31*/, FLAG_ParamArray_VARGs, 3, 1, TYPE_T1, FN_k, TYPE_T2, FN_v, TYPE_Any, FN_value, TYPE_This, FN_return,
	DATA_PARAM/*32*/, 0, 2, 1, TYPE_T1, FN_start, TYPE_T1, FN_end, TYPE_This, FN_return,
	DATA_PARAM/*33*/, 0, 1, 1, TYPE_Int, FN_initCapacity, TYPE_This, FN_return,
	DATA_PARAM/*34*/, 0, 1, 1, TYPE_Int, FN_size, TYPE_This, FN_return,
	DATA_PARAM/*35*/, FLAG_ParamArray_VARGs, 1, 1, TYPE_T1, FN_value, TYPE_This, FN_return,
	DATA_PARAM/*36*/, 0, 2, 1, TYPE_String, FN_msg, TYPE_Any, FN_bag, TYPE_Exception, FN_return,
	DATA_PARAM/*37*/, 0, 2, 1, TYPE_Any, FN_base, TYPE_Method, FN_method, TYPE_This, FN_return,
	DATA_PARAM/*38*/, 0, 1, 0, TYPE_Int, FN_seed,
	DATA_PARAM/*39*/, 0, 1, 1, TYPE_Int, FN_n, TYPE_Int, FN_return,
	DATA_PARAM/*40*/, 0, 0, 1, TYPE_Float, FN_return,
	DATA_PARAM/*41*/, 0, 1, 1, TYPE_Float, FN_n, TYPE_Int, FN_return,
	DATA_PARAM/*42*/, 0, 1, 1, TYPE_Int, FN_n, TYPE_Float, FN_return,
	DATA_PARAM/*43*/, 0, 0, 1, TYPE_Class, FN_return,
	DATA_PARAM/*44*/, 0, 0, 1, TYPE_String, FN_return,
	DATA_PARAM/*45*/, 0, 0, 1, TYPE_Array, FN_return,
	DATA_PARAM/*46*/, FLAG_ParamArray_VARGs, 1, 1, TYPE_Any, FN_v, TYPE_String, FN_return,
	DATA_PARAM/*47*/, FLAG_ParamArray_VARGs, 1, 1, TYPE_Object, FN_v, TYPE_String, FN_return,
	DATA_PARAM/*48*/, 0, 1, 1, TYPE_Any, FN_x, TYPE_Any, FN_return,
	DATA_PARAM/*49*/, 0, 1, 1, TYPE_Class, FN_c, TYPE_Boolean, FN_return,
	DATA_PARAM/*50*/, 0, 1, 1, TYPE_Any, FN_value, TYPE_Boolean, FN_return,
	DATA_PARAM/*51*/, 0, 1, 1, TYPE_Int, FN_value, TYPE_Boolean, FN_return,
	DATA_PARAM/*52*/, 0, 1, 1, TYPE_Float, FN_value, TYPE_Boolean, FN_return,
	DATA_PARAM/*53*/, 0, 1, 1, TYPE_String, FN_s, TYPE_Boolean, FN_return,
	DATA_PARAM/*54*/, 0, 1, 1, TYPE_Any, FN_v, TYPE_Boolean, FN_return,
	DATA_PARAM/*55*/, 0, 1, 1, TYPE_Int, FN_v, TYPE_Int, FN_return,
	DATA_PARAM/*56*/, 0, 1, 1, TYPE_Float, FN_v, TYPE_Float, FN_return,
	DATA_PARAM/*57*/, 0, 1, 1, TYPE_Float, FN_n, TYPE_Float, FN_return,
	DATA_PARAM/*58*/, 0, 1, 1, TYPE_String, FN_s, TYPE_String, FN_return,
	DATA_PARAM/*59*/, 0, 1, 1, TYPE_Any, FN_value, TYPE_Any, FN_return,
	DATA_PARAM/*60*/, 0, 0, 1, TYPE_Any, FN_return,
	DATA_PARAM/*61*/, FLAG_ParamArray_VARGs, 1, 1, TYPE_Int, FN_n, TYPE_Int, FN_return,
	DATA_PARAM/*62*/, 0, 2, 1, TYPE_Int, FN_n, TYPE_Int, FN_c, TYPE_Int, FN_return,
	DATA_PARAM/*63*/, 0, 1, 0, TYPE_Int, FN_c,
	DATA_PARAM/*64*/, 0, 2, 1, TYPE_Int, FN_s, TYPE_Int, FN_e, TYPE_Bytes, FN_return,
	DATA_PARAM/*65*/, 0, 2, 1, TYPE_Int, FN_offset, TYPE_Int, FN_length, TYPE_String, FN_return,
	DATA_PARAM/*66*/, 0, 2, 1, TYPE_Int, FN_s, TYPE_Int, FN_e, TYPE_String, FN_return,
	DATA_PARAM/*67*/, 0, 2, 1, TYPE_Int, FN_s, TYPE_Int, FN_e, TYPE_This, FN_return,
	DATA_PARAM/*68*/, 0, 2, 1, TYPE_String, FN_s, TYPE_String, FN_e, TYPE_This, FN_return,
	DATA_PARAM/*69*/, 0, 0, 1, TYPE_T1, FN_return,
	DATA_PARAM/*70*/, 0, 2, 1, TYPE_String, FN_urn, TYPE_String, FN_mode, TYPE_InputStream, FN_return,
	DATA_PARAM/*71*/, 0, 3, 1, TYPE_Bytes, FN_buf, TYPE_Int, FN_offset, TYPE_Int, FN_length, TYPE_Int, FN_return,
	DATA_PARAM/*72*/, 0, 1, 1, TYPE_StringDecoder, FN_c, TYPE_StringDecoder, FN_return,
	DATA_PARAM/*73*/, 0, 2, 1, TYPE_String, FN_urn, TYPE_String, FN_mode, TYPE_OutputStream, FN_return,
	DATA_PARAM/*74*/, 0, 1, 0, TYPE_Int, FN_ch,
	DATA_PARAM/*75*/, FLAG_ParamArray_VARGs, 1, 0, TYPE_Object, FN_value,
	DATA_PARAM/*76*/, FLAG_ParamArray_VARGs, 1, 0, TYPE_Any, FN_value,
	DATA_PARAM/*77*/, FLAG_ParamArray_VARGs, 1, 0, TYPE_Object, FN_data,
	DATA_PARAM/*78*/, 0, 1, 1, TYPE_StringEncoder, FN_c, TYPE_StringEncoder, FN_return,
	DATA_PARAM/*79*/, 0, 1, 1, TYPE_Class, FN_c, TYPE_Any, FN_return,
	DATA_PARAM/*80*/, FLAG_ParamArray_VARGs, 1, 1, TYPE_Object, FN_value, TYPE_String, FN_return,
	DATA_PARAM/*81*/, 0, 1, 1, TYPE_Any, FN_value, TYPE_String, FN_return,
	DATA_PARAM/*82*/, 0, 1, 1, TYPE_Regex, FN_re, TYPE_Int, FN_return,
	DATA_PARAM/*83*/, 0, 2, 1, TYPE_Regex, FN_re, TYPE_String, FN_s, TYPE_String, FN_return,
	DATA_PARAM/*84*/, 0, 1, 1, TYPE_Regex, FN_re, TYPE_StringARRAY, FN_return,
	DATA_PARAM/*85*/, 0, 1, 1, TYPE_Converter, FN_enc, TYPE_Bytes, FN_return,
	DATA_PARAM/*86*/, 0, 1, 1, TYPE_StringEncoder, FN_enc, TYPE_Bytes, FN_return,
	DATA_PARAM/*87*/, 0, 1, 1, TYPE_StringDecoder, FN_c, TYPE_String, FN_return,
	DATA_PARAM/*88*/, 0, 1, 1, TYPE_StringConverter, FN_c, TYPE_String, FN_return,
	DATA_PARAM/*89*/, 0, 1, 1, TYPE_Regex, FN_re, TYPE_Boolean, FN_return,
	DATA_PARAM/*90*/, 0, 0, 1, TYPE_InputStream, FN_return,
	DATA_PARAM/*91*/, 0, 0, 1, TYPE_OutputStream, FN_return,
	DATA_PARAM/*92*/, 0, 1, 1, TYPE_String, FN_key, TYPE_Any, FN_return,
	DATA_PARAM/*93*/, 0, 2, 1, TYPE_String, FN_key, TYPE_Any, FN_value, TYPE_Any, FN_return,
	DATA_PARAM/*94*/, 0, 1, 1, TYPE_String, FN_key, TYPE_StringARRAY, FN_return,
	DATA_PARAM/*95*/, 0, 1, 0, TYPE_Int, FN_status,
	DATA_PARAM/*96*/, 0, 1, 1, TYPE_InputStream, FN_in, TYPE_InputStream, FN_return,
	DATA_PARAM/*97*/, 0, 1, 1, TYPE_OutputStream, FN_out, TYPE_OutputStream, FN_return,
	DATA_PARAM/*98*/, 0, 1, 1, TYPE_Any, FN_msg, TYPE_Boolean, FN_return,
	DATA_PARAM/*99*/, 0, 2, 1, TYPE_String, FN_name, TYPE_Object, FN_value, TYPE_Any, FN_return,
	DATA_PARAM/*100*/, 0, 1, 1, TYPE_Boolean, FN_flag, TYPE_Boolean, FN_return,
	0
};
#define K_PARAM0_SIZE                   101

static knh_data_t APIData0[] = {
	DATA_METHOD0, CLASS_Bytes, MN_putc, 0, _DATA(Bytes_putc), /*P=*/1,
	DATA_METHOD0, CLASS_Bytes, MN_write, 0, _DATA(Bytes_write), /*P=*/2,
	DATA_METHOD0, CLASS_Bytes, MN_add, 0, _DATA(Bytes_write), /*P=*/2,
	DATA_METHOD0, CLASS_Array, MN_get, 0, _DATA(Array_get), /*P=*/3,
	DATA_METHOD0, CLASS_Array, MN_set, 0, _DATA(Array_set), /*P=*/4,
	DATA_METHOD0, CLASS_Array, MN_add, 0, _DATA(Array_add), /*P=*/5,
	DATA_METHOD0, CLASS_Array, MN_opSEND, 0, _DATA(Array_add), /*P=*/5,
	DATA_METHOD0, CLASS_Connection, MN_new, 0, _DATA(Connection_new), /*P=*/6,
	DATA_METHOD0, CLASS_Connection, MN_query, 0, _DATA(Connection_query), /*P=*/7,
	DATA_METHOD0, CLASS_Connection, MN_exec, 0, _DATA(Connection_exec), /*P=*/8,
	DATA_METHOD0, CLASS_Connection, MN_close, 0, _DATA(Connection_close), /*P=*/0,
	DATA_METHOD0, CLASS_ResultSet, MN_getSize, 0, _DATA(ResultSet_getSize), /*P=*/9,
	DATA_METHOD0, CLASS_ResultSet, MN_next, 0, _DATA(ResultSet_next), /*P=*/10,
	DATA_METHOD0, CLASS_ResultSet, MN_getName, 0, _DATA(ResultSet_getName), /*P=*/11,
	DATA_METHOD0, CLASS_ResultSet, MN_getInt, 0, _DATA(ResultSet_getInt), /*P=*/12,
	DATA_METHOD0, CLASS_ResultSet, MN_getFloat, 0, _DATA(ResultSet_getFloat), /*P=*/13,
	DATA_METHOD0, CLASS_ResultSet, MN_getString, 0, _DATA(ResultSet_getString), /*P=*/14,
	DATA_METHOD0, CLASS_ResultSet, MN_get, 0, _DATA(ResultSet_get), /*P=*/15,
	DATA_METHOD0, CLASS_ResultSet, MN_close, 0, _DATA(ResultSet_close), /*P=*/0,
	DATA_METHOD0, CLASS_DictMap, MN_opHAS, 0, _DATA(DictMap_opHAS), /*P=*/16,
	DATA_METHOD0, CLASS_DictMap, MN_get, 0, _DATA(DictMap_get), /*P=*/17,
	DATA_METHOD0, CLASS_DictMap, MN_set, 0, _DATA(DictMap_set), /*P=*/18,
	DATA_METHOD0, CLASS_DictMap, MN_remove, 0, _DATA(DictMap_remove), /*P=*/19,
	DATA_METHOD0, CLASS_DictMap, MN_clear, 0, _DATA(DictMap_clear), /*P=*/0,
	DATA_METHOD0, CLASS_DictMap, MN_keys, 0, _DATA(DictMap_keys), /*P=*/20,
	DATA_METHOD0, CLASS_DictMap, MN_values, 0, _DATA(DictMap_values), /*P=*/21,
	DATA_METHOD0, CLASS_Object, MN__empty, 0, _DATA(Object__empty), /*P=*/22,
	DATA_METHOD0, CLASS_Object, MN__s, 0, _DATA(Object__s), /*P=*/22,
	DATA_METHOD0, CLASS_Boolean, MN__s, 0, _DATA(Boolean__s), /*P=*/22,
	DATA_METHOD0, CLASS_Boolean, MN__k, 0, _DATA(Boolean__s), /*P=*/22,
	DATA_METHOD0, CLASS_Boolean, MN__data, 0, _DATA(Boolean__s), /*P=*/22,
	DATA_METHOD0, CLASS_Boolean, MN__dump, 0, _DATA(Boolean__s), /*P=*/22,
	DATA_METHOD0, CLASS_Int, MN__s, 0, _DATA(Int__s), /*P=*/22,
	DATA_METHOD0, CLASS_Float, MN__s, 0, _DATA(Float__s), /*P=*/22,
	DATA_METHOD0, CLASS_String, MN__s, 0, _DATA(String__s), /*P=*/22,
	DATA_METHOD0, CLASS_Bytes, MN__s, 0, _DATA(Bytes__s), /*P=*/22,
	DATA_METHOD0, CLASS_Regex, MN__s, 0, _DATA(Regex__s), /*P=*/22,
	DATA_METHOD0, CLASS_Class, MN__s, 0, _DATA(Class__s), /*P=*/22,
	DATA_METHOD0, CLASS_Method, MN__s, 0, _DATA(Method__s), /*P=*/22,
	DATA_METHOD0, CLASS_Exception, MN__s, 0, _DATA(Exception__s), /*P=*/22,
	DATA_METHOD0, CLASS_Object, MN__k, 0, _DATA(Object__k), /*P=*/22,
	DATA_METHOD0, CLASS_Int, MN__k, 0, _DATA(Int__k), /*P=*/22,
	DATA_METHOD0, CLASS_Int, MN__data, 0, _DATA(Int__k), /*P=*/22,
	DATA_METHOD0, CLASS_Int, MN__dump, 0, _DATA(Int__k), /*P=*/22,
	DATA_METHOD0, CLASS_Float, MN__k, 0, _DATA(Float__k), /*P=*/22,
	DATA_METHOD0, CLASS_Float, MN__data, 0, _DATA(Float__k), /*P=*/22,
	DATA_METHOD0, CLASS_Float, MN__dump, 0, _DATA(Float__k), /*P=*/22,
	DATA_METHOD0, CLASS_String, MN__k, 0, _DATA(String__k), /*P=*/22,
	DATA_METHOD0, CLASS_Iterator, MN__k, 0, _DATA(Iterator__k), /*P=*/22,
	DATA_METHOD0, CLASS_Tuple, MN__k, 0, _DATA(Tuple__k), /*P=*/22,
	DATA_METHOD0, CLASS_Range, MN__k, 0, _DATA(Range__k), /*P=*/22,
	DATA_METHOD0, CLASS_Array, MN__k, 0, _DATA(Array__k), /*P=*/22,
	DATA_METHOD0, CLASS_DictMap, MN__k, 0, _DATA(DictMap__k), /*P=*/22,
	DATA_METHOD0, CLASS_DictSet, MN__k, 0, _DATA(DictSet__k), /*P=*/22,
	DATA_METHOD0, CLASS_Class, MN__k, 0, _DATA(Class__k), /*P=*/22,
	DATA_METHOD0, CLASS_Method, MN__k, 0, _DATA(Method__k), /*P=*/22,
	DATA_METHOD0, CLASS_Translator, MN__k, 0, _DATA(Translator__k), /*P=*/22,
	DATA_METHOD0, CLASS_Exception, MN__k, 0, _DATA(Exception__k), /*P=*/22,
	DATA_METHOD0, CLASS_InputStream, MN__k, 0, _DATA(InputStream__k), /*P=*/22,
	DATA_METHOD0, CLASS_OutputStream, MN__k, 0, _DATA(OutputStream__k), /*P=*/22,
	DATA_METHOD0, CLASS_Script, MN__k, 0, _DATA(Script__k), /*P=*/22,
	DATA_METHOD0, CLASS_Any, MN__s, 0, _DATA(Any__k), /*P=*/22,
	DATA_METHOD0, CLASS_Any, MN__k, 0, _DATA(Any__k), /*P=*/22,
	DATA_METHOD0, CLASS_Object, MN__dump, 0, _DATA(Object__dump), /*P=*/22,
	DATA_METHOD0, CLASS_Bytes, MN__dump, 0, _DATA(Bytes__dump), /*P=*/22,
	DATA_METHOD0, CLASS_Class, MN__dump, 0, _DATA(Class__dump), /*P=*/22,
	DATA_METHOD0, CLASS_Method, MN__dump, 0, _DATA(Method__dump), /*P=*/22,
	DATA_METHOD0, CLASS_Exception, MN__dump, 0, _DATA(Exception__dump), /*P=*/22,
	DATA_METHOD0, CLASS_Object, MN__data, 0, _DATA(Object__data), /*P=*/22,
	DATA_METHOD0, CLASS_String, MN__data, 0, _DATA(String__data), /*P=*/22,
	DATA_METHOD0, CLASS_Bytes, MN__data, 0, _DATA(Bytes__data), /*P=*/22,
	DATA_METHOD0, CLASS_Array, MN__data, 0, _DATA(Array__data), /*P=*/22,
	DATA_METHOD0, CLASS_DictMap, MN__data, 0, _DATA(DictMap__data), /*P=*/22,
	DATA_METHOD0, CLASS_Exception, MN__data, 0, _DATA(Exception__data), /*P=*/22,
	DATA_METHOD0, CLASS_Func, MN__data, 0, _DATA(Func__data), /*P=*/22,
	DATA_METHOD0, CLASS_Class, MN__man, 0, _DATA(Class__man), /*P=*/22,
	DATA_METHOD0, CLASS_Object, MN__man, 0, _DATA(Class__man), /*P=*/22,
	DATA_METHOD0, CLASS_Object, MN__p, 0, _DATA(Object__p), /*P=*/22,
	DATA_METHOD0, CLASS_Int, MN__c, 0, _DATA(Int__c), /*P=*/22,
	DATA_METHOD0, CLASS_Int, MN__d, 0, _DATA(Int__d), /*P=*/22,
	DATA_METHOD0, CLASS_Int, MN__u, 0, _DATA(Int__u), /*P=*/22,
	DATA_METHOD0, CLASS_Int, MN__f, 0, _DATA(Int__f), /*P=*/22,
	DATA_METHOD0, CLASS_Int, MN__x, 0, _DATA(Int__x), /*P=*/22,
	DATA_METHOD0, CLASS_Int, MN__bits, 0, _DATA(Int__bits), /*P=*/22,
	DATA_METHOD0, CLASS_Float, MN__d, 0, _DATA(Float__d), /*P=*/22,
	DATA_METHOD0, CLASS_Float, MN__f, 0, _DATA(Float__f), /*P=*/22,
	DATA_METHOD0, CLASS_Float, MN__bits, 0, _DATA(Float__bits), /*P=*/22,
	DATA_METHOD0, CLASS_String, MN_opITR, 0, _DATA(String_Iterator), /*P=*/23,
	DATA_METHOD0, CLASS_Range, MN_opITR, 0, _DATA(Range_Iterator), /*P=*/24,
	DATA_METHOD0, CLASS_Array, MN_opITR, 0, _DATA(Array_Iterator), /*P=*/24,
	DATA_METHOD0, CLASS_DictMap, MN_opITR, 0, _DATA(DictMap_String__), /*P=*/23,
	DATA_METHOD0, CLASS_Object, MN_opITR, 0, _DATA(Object_Iterator), /*P=*/25,
	DATA_METHOD0, CLASS_Object, MN_new, FLAG_Method_Virtual, _DATA(Object_new), /*P=*/26,
	DATA_METHOD0, CLASS_Object, MN_new__MAP, FLAG_Method_Hidden, _DATA(Object_new__MAP), /*P=*/27,
	DATA_METHOD0, CLASS_Bytes, MN_new, 0, _DATA(Bytes_new), /*P=*/28,
	DATA_METHOD0, CLASS_Bytes, MN_new__ARRAY, 0, _DATA(Bytes_new__ARRAY), /*P=*/28,
	DATA_METHOD0, CLASS_String, MN_new, 0, _DATA(String_new), /*P=*/29,
	DATA_METHOD0, CLASS_Regex, MN_new, FLAG_Method_Const, _DATA(Regex_new), /*P=*/30,
	DATA_METHOD0, CLASS_Tuple, MN_new__TUPLE, FLAG_Method_Const|FLAG_Method_Hidden, _DATA(Tuple_new__TUPLE), /*P=*/31,
	DATA_METHOD0, CLASS_Range, MN_new__LIST, FLAG_Method_Const|FLAG_Method_Hidden, _DATA(Range_new__LIST), /*P=*/32,
	DATA_METHOD0, CLASS_Range, MN_new__RANGE, FLAG_Method_Const|FLAG_Method_Hidden, _DATA(Range_new__RANGE), /*P=*/32,
	DATA_METHOD0, CLASS_Array, MN_new, 0, _DATA(Array_new), /*P=*/33,
	DATA_METHOD0, CLASS_Array, MN_new__ARRAY, FLAG_Method_Hidden, _DATA(Array_new__ARRAY), /*P=*/34,
	DATA_METHOD0, CLASS_Array, MN_new__LIST, FLAG_Method_Hidden, _DATA(Array_new__LIST), /*P=*/35,
	DATA_METHOD0, CLASS_DictMap, MN_new, 0, _DATA(DictMap_new), /*P=*/33,
	DATA_METHOD0, CLASS_DictMap, MN_new__MAP, FLAG_Method_Hidden, _DATA(DictMap_new__MAP), /*P=*/35,
	DATA_METHOD0, CLASS_Exception, MN_new, 0, _DATA(Exception_new), /*P=*/36,
	DATA_METHOD0, CLASS_Func, MN_new, 0, _DATA(Func_new), /*P=*/37,
	DATA_METHOD0, CLASS_System, MN_setRandomSeed, FLAG_Method_Static, _DATA(System_setRandomSeed), /*P=*/38,
	DATA_METHOD0, CLASS_Int, MN_random, FLAG_Method_Static, _DATA(Int_random), /*P=*/39,
	DATA_METHOD0, CLASS_Float, MN_random, FLAG_Method_Static, _DATA(Float_random), /*P=*/40,
	DATA_METHOD0, CLASS_Float, MN_floatToIntBits, FLAG_Method_Static, _DATA(Float_floatToIntBits), /*P=*/41,
	DATA_METHOD0, CLASS_Float, MN_intToFloatBits, FLAG_Method_Static, _DATA(Float_intToFloatBits), /*P=*/42,
	DATA_METHOD0, CLASS_Object, MN_getClass, FLAG_Method_Const, _DATA(Object_getClass), /*P=*/43,
	DATA_METHOD0, CLASS_Object, MN_hashCode, 0, _DATA(Object_hashCode), /*P=*/9,
	DATA_METHOD0, CLASS_Object, MN_isNull, FLAG_Method_Const|FLAG_Method_Hidden, _DATA(Object_isNull), /*P=*/10,
	DATA_METHOD0, CLASS_Object, MN_isNotNull, FLAG_Method_Const|FLAG_Method_Hidden, _DATA(Object_isNotNull), /*P=*/10,
	DATA_METHOD0, CLASS_Object, MN_getKey, FLAG_Method_Const|FLAG_Method_Virtual, _DATA(Object_getKey), /*P=*/44,
	DATA_METHOD0, CLASS_Class, MN_domain, FLAG_Method_Hidden, _DATA(Class_domain), /*P=*/45,
	DATA_METHOD0, CLASS_Method, MN_isAbstract, FLAG_Method_Const, _DATA(Method_isAbstract), /*P=*/10,
	DATA_METHOD0, CLASS_Method, MN_getName, FLAG_Method_Const, _DATA(Method_getName), /*P=*/44,
	DATA_METHOD0, CLASS_Method, MN_format, FLAG_Method_Const|FLAG_Method_Hidden, _DATA(Method_format), /*P=*/46,
	DATA_METHOD0, CLASS_Method, MN_format__FINAL, FLAG_Method_Const|FLAG_Method_Hidden, _DATA(Method_format__FINAL), /*P=*/47,
	DATA_METHOD0, CLASS_Func, MN_invoke, FLAG_Method_Hidden|FLAG_Method_Private, _DATA(Func_invoke), /*P=*/48,
	DATA_METHOD0, CLASS_Object, MN_opADDR, FLAG_Method_Const, _DATA(Object_opADDR), /*P=*/9,
	DATA_METHOD0, CLASS_Object, MN_opOF, FLAG_Method_Const, _DATA(Object_opOF), /*P=*/49,
	DATA_METHOD0, CLASS_Object, MN_opEQ, FLAG_Method_Const, _DATA(Object_opEQ), /*P=*/50,
	DATA_METHOD0, CLASS_Object, MN_opNOTEQ, FLAG_Method_Const, _DATA(Object_opNOTEQ), /*P=*/50,
	DATA_METHOD0, CLASS_Object, MN_opLT, FLAG_Method_Const, _DATA(Object_opLT), /*P=*/50,
	DATA_METHOD0, CLASS_Object, MN_opLTE, FLAG_Method_Const, _DATA(Object_opLTE), /*P=*/50,
	DATA_METHOD0, CLASS_Object, MN_opGT, FLAG_Method_Const, _DATA(Object_opGT), /*P=*/50,
	DATA_METHOD0, CLASS_Object, MN_opGTE, FLAG_Method_Const, _DATA(Object_opGTE), /*P=*/50,
	DATA_METHOD0, CLASS_Int, MN_opEQ, FLAG_Method_Const, _DATA(Int_opEQ), /*P=*/51,
	DATA_METHOD0, CLASS_Int, MN_opNOTEQ, FLAG_Method_Const, _DATA(Int_opNOTEQ), /*P=*/51,
	DATA_METHOD0, CLASS_Int, MN_opLT, FLAG_Method_Const, _DATA(Int_opLT), /*P=*/51,
	DATA_METHOD0, CLASS_Int, MN_opLTE, FLAG_Method_Const, _DATA(Int_opLTE), /*P=*/51,
	DATA_METHOD0, CLASS_Int, MN_opGT, FLAG_Method_Const, _DATA(Int_opGT), /*P=*/51,
	DATA_METHOD0, CLASS_Int, MN_opGTE, FLAG_Method_Const, _DATA(Int_opGTE), /*P=*/51,
	DATA_METHOD0, CLASS_Float, MN_opEQ, FLAG_Method_Const, _DATA(Float_opEQ), /*P=*/52,
	DATA_METHOD0, CLASS_Float, MN_opNOTEQ, FLAG_Method_Const, _DATA(Float_opNOTEQ), /*P=*/52,
	DATA_METHOD0, CLASS_Float, MN_opLT, FLAG_Method_Const, _DATA(Float_opLT), /*P=*/52,
	DATA_METHOD0, CLASS_Float, MN_opLTE, FLAG_Method_Const, _DATA(Float_opLTE), /*P=*/52,
	DATA_METHOD0, CLASS_Float, MN_opGT, FLAG_Method_Const, _DATA(Float_opGT), /*P=*/52,
	DATA_METHOD0, CLASS_Float, MN_opGTE, FLAG_Method_Const, _DATA(Float_opGTE), /*P=*/52,
	DATA_METHOD0, CLASS_String, MN_opHAS, FLAG_Method_Const, _DATA(String_opHAS), /*P=*/53,
	DATA_METHOD0, CLASS_Range, MN_opHAS, 0, _DATA(Range_opHAS), /*P=*/54,
	DATA_METHOD0, CLASS_Range, MN_opCASE, 0, _DATA(Range_opHAS), /*P=*/54,
	DATA_METHOD0, CLASS_Array, MN_opHAS, 0, _DATA(Array_opHAS), /*P=*/54,
	DATA_METHOD0, CLASS_Array, MN_opCASE, 0, _DATA(Array_opHAS), /*P=*/54,
	DATA_METHOD0, CLASS_Tuple, MN_opHAS, 0, _DATA(Tuple_opHAS), /*P=*/54,
	DATA_METHOD0, CLASS_Tuple, MN_opCASE, 0, _DATA(Tuple_opHAS), /*P=*/54,
	DATA_METHOD0, CLASS_Object, MN_opCASE, FLAG_Method_Const, _DATA(Object_opCASE), /*P=*/54,
	DATA_METHOD0, CLASS_Int, MN_opCASE, FLAG_Method_Const, _DATA(Int_opCASE), /*P=*/54,
	DATA_METHOD0, CLASS_Float, MN_opCASE, FLAG_Method_Const, _DATA(Float_opCASE), /*P=*/54,
	DATA_METHOD0, CLASS_Object, MN_opIS, FLAG_Method_Const, _DATA(Object_opIS), /*P=*/54,
	DATA_METHOD0, CLASS_Int, MN_opADD, FLAG_Method_Const, _DATA(Int_opADD), /*P=*/55,
	DATA_METHOD0, CLASS_Int, MN_opNEG, FLAG_Method_Const, _DATA(Int_opNEG), /*P=*/9,
	DATA_METHOD0, CLASS_Int, MN_opSUB, FLAG_Method_Const, _DATA(Int_opSUB), /*P=*/39,
	DATA_METHOD0, CLASS_Int, MN_opMUL, FLAG_Method_Const, _DATA(Int_opMUL), /*P=*/39,
	DATA_METHOD0, CLASS_Int, MN_opDIV, FLAG_Method_Const, _DATA(Int_opDIV), /*P=*/39,
	DATA_METHOD0, CLASS_Int, MN_opMOD, FLAG_Method_Const, _DATA(Int_opMOD), /*P=*/39,
	DATA_METHOD0, CLASS_Float, MN_opADD, FLAG_Method_Const, _DATA(Float_opADD), /*P=*/56,
	DATA_METHOD0, CLASS_Float, MN_opNEG, FLAG_Method_Const, _DATA(Float_opNEG), /*P=*/40,
	DATA_METHOD0, CLASS_Float, MN_opSUB, FLAG_Method_Const, _DATA(Float_opSUB), /*P=*/57,
	DATA_METHOD0, CLASS_Float, MN_opMUL, FLAG_Method_Const, _DATA(Float_opMUL), /*P=*/57,
	DATA_METHOD0, CLASS_Float, MN_opDIV, FLAG_Method_Const, _DATA(Float_opDIV), /*P=*/57,
	DATA_METHOD0, CLASS_String, MN_opSUB, FLAG_Method_Const, _DATA(String_opSUB), /*P=*/58,
	DATA_METHOD0, CLASS_Any, MN_opADD, 0, _DATA(Any_opADD), /*P=*/59,
	DATA_METHOD0, CLASS_Any, MN_opSUB, 0, _DATA(Any_opADD), /*P=*/59,
	DATA_METHOD0, CLASS_Any, MN_opMUL, 0, _DATA(Any_opADD), /*P=*/59,
	DATA_METHOD0, CLASS_Any, MN_opDIV, 0, _DATA(Any_opADD), /*P=*/59,
	DATA_METHOD0, CLASS_Any, MN_opMOD, 0, _DATA(Any_opADD), /*P=*/59,
	DATA_METHOD0, CLASS_Any, MN_opNEG, 0, _DATA(Any_opADD), /*P=*/60,
	DATA_METHOD0, CLASS_Boolean, MN_opNOT, FLAG_Method_Const, _DATA(Boolean_opNOT), /*P=*/10,
	DATA_METHOD0, CLASS_Int, MN_opLAND, FLAG_Method_Const, _DATA(Int_opLAND), /*P=*/61,
	DATA_METHOD0, CLASS_Int, MN_opLOR, FLAG_Method_Const, _DATA(Int_opLOR), /*P=*/61,
	DATA_METHOD0, CLASS_Int, MN_opLXOR, FLAG_Method_Const, _DATA(Int_opLXOR), /*P=*/39,
	DATA_METHOD0, CLASS_Int, MN_opLNOT, FLAG_Method_Const, _DATA(Int_opLNOT), /*P=*/9,
	DATA_METHOD0, CLASS_Int, MN_opLSFT, FLAG_Method_Const, _DATA(Int_opLSFT), /*P=*/39,
	DATA_METHOD0, CLASS_Int, MN_opRSFT, FLAG_Method_Const, _DATA(Int_opRSFT), /*P=*/39,
	DATA_METHOD0, CLASS_Bytes, MN_getSize, 0, _DATA(Bytes_getSize), /*P=*/9,
	DATA_METHOD0, CLASS_Array, MN_getSize, 0, _DATA(Bytes_getSize), /*P=*/9,
	DATA_METHOD0, CLASS_DictMap, MN_getSize, 0, _DATA(Bytes_getSize), /*P=*/9,
	DATA_METHOD0, CLASS_String, MN_getSize, FLAG_Method_Const, _DATA(String_getSize), /*P=*/9,
	DATA_METHOD0, CLASS_Tuple, MN_getSize, 0, _DATA(Tuple_getSize), /*P=*/9,
	DATA_METHOD0, CLASS_Bytes, MN_get, 0, _DATA(Bytes_get), /*P=*/39,
	DATA_METHOD0, CLASS_Bytes, MN_set, 0, _DATA(Bytes_set), /*P=*/62,
	DATA_METHOD0, CLASS_Bytes, MN_setAll, 0, _DATA(Bytes_setAll), /*P=*/63,
	DATA_METHOD0, CLASS_String, MN_get, FLAG_Method_Const, _DATA(String_get), /*P=*/11,
	DATA_METHOD0, CLASS_Bytes, MN_opUNTIL, 0, _DATA(Bytes_opUNTIL), /*P=*/64,
	DATA_METHOD0, CLASS_Bytes, MN_opTO, 0, _DATA(Bytes_opTO), /*P=*/64,
	DATA_METHOD0, CLASS_String, MN_substring, FLAG_Method_Const, _DATA(String_substring), /*P=*/65,
	DATA_METHOD0, CLASS_String, MN_opUNTIL, FLAG_Method_Const, _DATA(String_opUNTIL), /*P=*/66,
	DATA_METHOD0, CLASS_String, MN_opTO, FLAG_Method_Const, _DATA(String_opTO), /*P=*/66,
	DATA_METHOD0, CLASS_Array, MN_opUNTIL, 0, _DATA(Array_opUNTIL), /*P=*/67,
	DATA_METHOD0, CLASS_Array, MN_opTO, 0, _DATA(Array_opTO), /*P=*/67,
	DATA_METHOD0, CLASS_DictMap, MN_opTO, 0, _DATA(DictMap_opTO), /*P=*/68,
	DATA_METHOD0, CLASS_DictMap, MN_opUNTIL, 0, _DATA(DictMap_opUNTIL), /*P=*/68,
	DATA_METHOD0, CLASS_Tuple, MN_opEXPAND, 0, _DATA(Tuple_opEXPAND), /*P=*/60,
	DATA_METHOD0, CLASS_Array, MN_opEXPAND, 0, _DATA(Array_opEXPAND), /*P=*/69,
	DATA_METHOD0, CLASS_InputStream, MN_new, 0, _DATA(InputStream_new), /*P=*/70,
	DATA_METHOD0, CLASS_InputStream, MN_getChar, 0, _DATA(InputStream_getChar), /*P=*/9,
	DATA_METHOD0, CLASS_InputStream, MN_read, 0, _DATA(InputStream_read), /*P=*/71,
	DATA_METHOD0, CLASS_InputStream, MN_isClosed, 0, _DATA(InputStream_isClosed), /*P=*/10,
	DATA_METHOD0, CLASS_InputStream, MN_close, 0, _DATA(InputStream_close), /*P=*/0,
	DATA_METHOD0, CLASS_InputStream, MN_setCharset, 0, _DATA(InputStream_setCharset), /*P=*/72,
	DATA_METHOD0, CLASS_InputStream, MN_readLine, 0, _DATA(InputStream_readLine), /*P=*/44,
	DATA_METHOD0, CLASS_OutputStream, MN_new, 0, _DATA(OutputStream_new), /*P=*/73,
	DATA_METHOD0, CLASS_OutputStream, MN_writeChar, 0, _DATA(OutputStream_writeChar), /*P=*/74,
	DATA_METHOD0, CLASS_OutputStream, MN_write, 0, _DATA(OutputStream_write), /*P=*/2,
	DATA_METHOD0, CLASS_OutputStream, MN_isClosed, 0, _DATA(OutputStream_isClosed), /*P=*/10,
	DATA_METHOD0, CLASS_OutputStream, MN_print, 0, _DATA(OutputStream_print), /*P=*/75,
	DATA_METHOD0, CLASS_OutputStream, MN_opSEND, 0, _DATA(OutputStream_opSEND), /*P=*/75,
	DATA_METHOD0, CLASS_OutputStream, MN_println, 0, _DATA(OutputStream_println), /*P=*/76,
	DATA_METHOD0, CLASS_OutputStream, MN_writeData, 0, _DATA(OutputStream_writeData), /*P=*/77,
	DATA_METHOD0, CLASS_OutputStream, MN_flush, 0, _DATA(OutputStream_flush), /*P=*/0,
	DATA_METHOD0, CLASS_OutputStream, MN_clearBuffer, 0, _DATA(OutputStream_clearBuffer), /*P=*/0,
	DATA_METHOD0, CLASS_OutputStream, MN_close, 0, _DATA(OutputStream_close), /*P=*/0,
	DATA_METHOD0, CLASS_OutputStream, MN_setCharset, 0, _DATA(OutputStream_setCharaset), /*P=*/78,
	DATA_METHOD0, CLASS_String, MN_opEXISTS, FLAG_Method_Const, _DATA(String_opEXISTS), /*P=*/10,
	DATA_METHOD0, CLASS_String, MN_opPATH, FLAG_Method_Hidden|FLAG_Method_Private, _DATA(String_opPATH), /*P=*/79,
	DATA_METHOD0, CLASS_String, MN_equals, FLAG_Method_Const, _DATA(String_equals), /*P=*/53,
	DATA_METHOD0, CLASS_String, MN_startsWith, FLAG_Method_Const, _DATA(String_startsWith), /*P=*/53,
	DATA_METHOD0, CLASS_String, MN_endsWith, FLAG_Method_Const, _DATA(String_endsWith), /*P=*/53,
	DATA_METHOD0, CLASS_String, MN_concat, FLAG_Method_Const, _DATA(String_concat), /*P=*/80,
	DATA_METHOD0, CLASS_String, MN_opADD, FLAG_Method_Const, _DATA(String_concat), /*P=*/81,
	DATA_METHOD0, CLASS_String, MN_indexOf, FLAG_Method_Const, _DATA(String_indexOf), /*P=*/82,
	DATA_METHOD0, CLASS_String, MN_lastIndexOf, FLAG_Method_Const, _DATA(String_lastIndexOf), /*P=*/82,
	DATA_METHOD0, CLASS_String, MN_replace, FLAG_Method_Const, _DATA(String_replace), /*P=*/83,
	DATA_METHOD0, CLASS_String, MN_split, FLAG_Method_Const, _DATA(String_split), /*P=*/84,
	DATA_METHOD0, CLASS_Bytes, MN_, FLAG_Method_Const, _DATA(Bytes_convert), /*P=*/85,
	DATA_METHOD0, CLASS_String, MN_encode, FLAG_Method_Const, _DATA(String_encode), /*P=*/86,
	DATA_METHOD0, CLASS_Bytes, MN_decode, FLAG_Method_Const, _DATA(Bytes_toString), /*P=*/87,
	DATA_METHOD0, CLASS_String, MN_, FLAG_Method_Const, _DATA(String_convert), /*P=*/88,
	DATA_METHOD0, CLASS_String, MN_convert, FLAG_Method_Const, _DATA(String_convert), /*P=*/88,
	DATA_METHOD0, CLASS_String, MN_trim, FLAG_Method_Const, _DATA(String_trim), /*P=*/44,
	DATA_METHOD0, CLASS_String, MN_opCASE, 0, _DATA(String_opCASE), /*P=*/89,
	DATA_METHOD0, CLASS_Regex, MN_opCASE, 0, _DATA(Regex_opCASE), /*P=*/53,
	DATA_METHOD0, CLASS_System, MN_getIn, FLAG_Method_Static, _DATA(System_getIn), /*P=*/90,
	DATA_METHOD0, CLASS_System, MN_getOut, FLAG_Method_Static, _DATA(System_getOut), /*P=*/91,
	DATA_METHOD0, CLASS_System, MN_getErr, FLAG_Method_Static, _DATA(System_getErr), /*P=*/91,
	DATA_METHOD0, CLASS_System, MN_getProperty, FLAG_Method_Static, _DATA(System_getProperty), /*P=*/92,
	DATA_METHOD0, CLASS_System, MN_setProperty, FLAG_Method_Static, _DATA(System_setProperty), /*P=*/93,
	DATA_METHOD0, CLASS_System, MN_listProperties, FLAG_Method_Hidden, _DATA(System_listProperties), /*P=*/94,
	DATA_METHOD0, CLASS_System, MN_gc, FLAG_Method_Static, _DATA(System_gc), /*P=*/0,
	DATA_METHOD0, CLASS_System, MN_push, FLAG_Method_Static|FLAG_Method_Hidden, _DATA(System_push), /*P=*/75,
	DATA_METHOD0, CLASS_System, MN_getTime, 0, _DATA(System_getTime), /*P=*/9,
	DATA_METHOD0, CLASS_System, MN_exit, FLAG_Method_Static, _DATA(System_exit), /*P=*/95,
	DATA_METHOD0, CLASS_Context, MN_setIn, FLAG_Method_Static, _DATA(Context_setIn), /*P=*/96,
	DATA_METHOD0, CLASS_Context, MN_setOut, FLAG_Method_Static, _DATA(Context_setOut), /*P=*/97,
	DATA_METHOD0, CLASS_Context, MN_setErr, FLAG_Method_Static, _DATA(Context_setErr), /*P=*/97,
	DATA_METHOD0, CLASS_Exception, MN_opOF, 0, _DATA(Exception_opOF), /*P=*/98,
	DATA_METHOD0, CLASS_NameSpace, MN_setConst, FLAG_Method_Const|FLAG_Method_Hidden, _DATA(NameSpace_setConst), /*P=*/99,
	DATA_METHOD0, CLASS_Class, MN_setConst, FLAG_Method_Const|FLAG_Method_Hidden, _DATA(Class_setConst), /*P=*/99,
	DATA_METHOD0, CLASS_Object, MN_isRelease, 0, _DATA(Object_isRelease), /*P=*/10,
	DATA_METHOD0, CLASS_Object, MN_isDebug, 0, _DATA(Object_isDebug), /*P=*/10,
	DATA_METHOD0, CLASS_Object, MN_isImmutable, 0, _DATA(Object_isImmutable), /*P=*/10,
	DATA_METHOD0, CLASS_Object, MN_isModified, 0, _DATA(Object_isModified), /*P=*/10,
	DATA_METHOD0, CLASS_Object, MN_setModified, 0, _DATA(Object_setModified), /*P=*/100,
	DATA_METHOD0, CLASS_Object, MN_isShared, 0, _DATA(Object_isShared), /*P=*/10,
	DATA_METHOD0, CLASS_Object, MN_setShared, 0, _DATA(Object_setShared), /*P=*/100,
	DATA_METHOD0, CLASS_String, MN_isASCII, 0, _DATA(String_isASCII), /*P=*/10,
	DATA_METHOD0, CLASS_DictMap, MN_isIgnoreCase, 0, _DATA(DictMap_isIgnoreCase), /*P=*/10,
	DATA_METHOD0, CLASS_DictSet, MN_isIgnoreCase, 0, _DATA(DictSet_isIgnoreCase), /*P=*/10,
	DATA_METHOD0, CLASS_Class, MN_isRelease, 0, _DATA(Class_isRelease), /*P=*/10,
	DATA_METHOD0, CLASS_Class, MN_isDebug, 0, _DATA(Class_isDebug), /*P=*/10,
	DATA_METHOD0, CLASS_Class, MN_isImmutable, 0, _DATA(Class_isImmutable), /*P=*/10,
	DATA_METHOD0, CLASS_Class, MN_isMetaClass, 0, _DATA(Class_isMetaClass), /*P=*/10,
	DATA_METHOD0, CLASS_Class, MN_isPrivate, 0, _DATA(Class_isPrivate), /*P=*/10,
	DATA_METHOD0, CLASS_Class, MN_isPublic, 0, _DATA(Class_isPublic), /*P=*/10,
	DATA_METHOD0, CLASS_Class, MN_isFinal, 0, _DATA(Class_isFinal), /*P=*/10,
	DATA_METHOD0, CLASS_Class, MN_isSingleton, 0, _DATA(Class_isSingleton), /*P=*/10,
	DATA_METHOD0, CLASS_Class, MN_isUnique, 0, _DATA(Class_isUnique), /*P=*/10,
	DATA_METHOD0, CLASS_Class, MN_isInterface, 0, _DATA(Class_isInterface), /*P=*/10,
	DATA_METHOD0, CLASS_Method, MN_isPrivate, 0, _DATA(Method_isPrivate), /*P=*/10,
	DATA_METHOD0, CLASS_Method, MN_isPublic, 0, _DATA(Method_isPublic), /*P=*/10,
	DATA_METHOD0, CLASS_Method, MN_isVirtual, 0, _DATA(Method_isVirtual), /*P=*/10,
	DATA_METHOD0, CLASS_Method, MN_isFinal, 0, _DATA(Method_isFinal), /*P=*/10,
	DATA_METHOD0, CLASS_Method, MN_isConst, 0, _DATA(Method_isConst), /*P=*/10,
	DATA_METHOD0, CLASS_Method, MN_isStatic, 0, _DATA(Method_isStatic), /*P=*/10,
	DATA_METHOD0, CLASS_Translator, MN_isInterface, 0, _DATA(Translator_isInterface), /*P=*/10,
	DATA_METHOD0, CLASS_Translator, MN_isSignificant, 0, _DATA(Translator_isSignificant), /*P=*/10,
	DATA_METHOD0, CLASS_Translator, MN_isSemantic, 0, _DATA(Translator_isSemantic), /*P=*/10,
	DATA_METHOD0, CLASS_Translator, MN_isTotal, 0, _DATA(Translator_isTotal), /*P=*/10,
	DATA_METHOD0, CLASS_Translator, MN_isPartial, 0, _DATA(Translator_isPartial), /*P=*/10,
	DATA_METHOD0, CLASS_Translator, MN_isLossLess, 0, _DATA(Translator_isLossLess), /*P=*/10,
	DATA_METHOD0, CLASS_Translator, MN_isFinal, 0, _DATA(Translator_isFinal), /*P=*/10,
	DATA_METHOD0, CLASS_Translator, MN_isConst, 0, _DATA(Translator_isConst), /*P=*/10,
	DATA_METHOD0, CLASS_Translator, MN_isTemporal, 0, _DATA(Translator_isTemporal), /*P=*/10,
	DATA_METHOD0, CLASS_Exception, MN_isLogging, 0, _DATA(Exception_isLogging), /*P=*/10,
	DATA_METHOD0, CLASS_OutputStream, MN_isAutoFlush, 0, _DATA(OutputStream_isAutoFlush), /*P=*/10,
	DATA_METHOD0, CLASS_OutputStream, MN_setAutoFlush, 0, _DATA(OutputStream_setAutoFlush), /*P=*/100,
	DATA_METHOD0, CLASS_OutputStream, MN_isStoringBuffer, 0, _DATA(OutputStream_isStoringBuffer), /*P=*/10,
	DATA_METHOD0, CLASS_OutputStream, MN_setStoringBuffer, 0, _DATA(OutputStream_setStoringBuffer), /*P=*/100,
	DATA_METHOD0, CLASS_Context, MN_isStrict, 0, _DATA(Context_isStrict), /*P=*/10,
	DATA_METHOD0, CLASS_Context, MN_setStrict, 0, _DATA(Context_setStrict), /*P=*/100,
	DATA_METHOD0, CLASS_Context, MN_isDebug, 0, _DATA(Context_isDebug), /*P=*/10,
	DATA_METHOD0, CLASS_Context, MN_setDebug, 0, _DATA(Context_setDebug), /*P=*/100,
	DATA_METHOD0, CLASS_Context, MN_isVerbose, 0, _DATA(Context_isVerbose), /*P=*/10,
	DATA_METHOD0, CLASS_Context, MN_setVerbose, 0, _DATA(Context_setVerbose), /*P=*/100,
	DATA_METHOD0, CLASS_Context, MN_isInteractive, 0, _DATA(Context_isInteractive), /*P=*/10,
	DATA_TCAST, CLASS_String, CLASS_Iterator, FLAG_Translator_Total, _DATA(String_Iterator),
	DATA_TCAST, CLASS_String, CLASS_StringITR, FLAG_Translator_Total, _DATA(String_Iterator),
	DATA_TCAST, CLASS_Range, CLASS_Iterator, FLAG_Translator_Total, _DATA(Range_Iterator),
	DATA_TCAST, CLASS_Array, CLASS_Iterator, FLAG_Translator_Total, _DATA(Array_Iterator),
	DATA_TCAST, CLASS_DictMap, CLASS_Iterator, FLAG_Translator_Total, _DATA(DictMap_String__),
	DATA_TCAST, CLASS_DictMap, CLASS_StringITR, FLAG_Translator_Total, _DATA(DictMap_String__),
	DATA_TCAST, CLASS_Float, CLASS_Int, FLAG_Translator_Const|FLAG_Translator_Total|FLAG_Translator_Final, _DATA(Float_Int),
	DATA_TCAST, CLASS_String, CLASS_Int, FLAG_Translator_Const|FLAG_Translator_Total|FLAG_Translator_Final, _DATA(String_Int),
	DATA_TCAST, CLASS_Int, CLASS_Float, FLAG_Translator_Const|FLAG_Translator_Total|FLAG_Translator_Final, _DATA(Int_Float),
	DATA_TCAST, CLASS_Object, CLASS_Iterator, FLAG_Translator_Const|FLAG_Translator_Total, _DATA(Object_Iterator),
	DATA_TCAST, CLASS_Iterator, CLASS_Array, FLAG_Translator_Total, _DATA(Iterator_Array),
	DATA_TCAST, CLASS_Bytes, CLASS_InputStream, FLAG_Translator_Total, _DATA(Bytes_InputStream),
	DATA_TCAST, CLASS_String, CLASS_Regex, FLAG_Translator_Const|FLAG_Translator_Total, _DATA(String_Regex),
	DATA_TCAST, CLASS_InputStream, CLASS_StringITR, FLAG_Translator_Total|FLAG_Translator_Final, _DATA(knh_InputStream_String__),
	0
};
