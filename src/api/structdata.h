// THIS FILE WAS AUTOMATICALLY GENERATED

/* ======================================================================== */
/* flag */

static METHOD Object_isRelease(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_Object_isRelease((knh_Object_t*)sfp[0].o));
}

static METHOD Object_isDebug(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, !(knh_Object_isRelease((knh_Object_t*)sfp[0].o)));
}

static METHOD Object_isImmutable(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_Object_isImmutable((knh_Object_t*)sfp[0].o));
}

static METHOD Object_isUndefined(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_Object_isUndefined((knh_Object_t*)sfp[0].o));
}

static METHOD Object_isModified(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_Object_isModified((knh_Object_t*)sfp[0].o));
}

static METHOD Object_setModified(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Object_setModified((knh_Object_t*)sfp[0].o, p_bool(sfp[1]));
	KNH_RETURN_void(ctx, sfp);
}

static METHOD Object_isShared(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_Object_isShared((knh_Object_t*)sfp[0].o));
}

static METHOD Object_setShared(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Object_setShared((knh_Object_t*)sfp[0].o, p_bool(sfp[1]));
	KNH_RETURN_void(ctx, sfp);
}

static METHOD String_isAscii(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_String_isAscii((knh_String_t*)sfp[0].o));
}

static METHOD DictMap_isIgnoreCase(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_DictMap_isIgnoreCase((knh_DictMap_t*)sfp[0].o));
}

static METHOD DictSet_isIgnoreCase(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_DictSet_isIgnoreCase((knh_DictSet_t*)sfp[0].o));
}

static METHOD Class_isRelease(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_class_isRelease(knh_Class_cid(sfp[0].c)));
}

static METHOD Class_isDebug(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, !(knh_class_isRelease(knh_Class_cid(sfp[0].c))));
}

static METHOD Class_isImmutable(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_class_isImmutable(knh_Class_cid(sfp[0].c)));
}

static METHOD Class_isMetaClass(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_class_isMetaClass(knh_Class_cid(sfp[0].c)));
}

static METHOD Class_isPrivate(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_class_isPrivate(knh_Class_cid(sfp[0].c)));
}

static METHOD Class_isPublic(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, !(knh_class_isPrivate(knh_Class_cid(sfp[0].c))));
}

static METHOD Class_isFinal(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_class_isFinal(knh_Class_cid(sfp[0].c)));
}

static METHOD Class_isSingleton(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_class_isSingleton(knh_Class_cid(sfp[0].c)));
}

static METHOD Class_isUnique(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_class_isUnique(knh_Class_cid(sfp[0].c)));
}

static METHOD Class_isInterface(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_class_isInterface(knh_Class_cid(sfp[0].c)));
}

static METHOD Method_isPrivate(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_Method_isPrivate((knh_Method_t*)sfp[0].o));
}

static METHOD Method_isPublic(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, !(knh_Method_isPrivate((knh_Method_t*)sfp[0].o)));
}

static METHOD Method_isVirtual(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_Method_isVirtual((knh_Method_t*)sfp[0].o));
}

static METHOD Method_isFinal(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, !(knh_Method_isVirtual((knh_Method_t*)sfp[0].o)));
}

static METHOD Method_isConst(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_Method_isConst((knh_Method_t*)sfp[0].o));
}

static METHOD Method_isStatic(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_Method_isStatic((knh_Method_t*)sfp[0].o));
}

static METHOD Method_isVarArgs(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_Method_isVarArgs((knh_Method_t*)sfp[0].o));
}

static METHOD Mapper_isInterface(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_Mapper_isInterface((knh_Mapper_t*)sfp[0].o));
}

static METHOD Mapper_isICast(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_Mapper_isICast((knh_Mapper_t*)sfp[0].o));
}

static METHOD Mapper_isSignificant(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_Mapper_isSignificant((knh_Mapper_t*)sfp[0].o));
}

static METHOD Mapper_isFinal(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_Mapper_isFinal((knh_Mapper_t*)sfp[0].o));
}

static METHOD Mapper_isSynonym(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_Mapper_isSynonym((knh_Mapper_t*)sfp[0].o));
}

static METHOD Mapper_isLossLess(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_Mapper_isLossLess((knh_Mapper_t*)sfp[0].o));
}

static METHOD Mapper_isTotal(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_Mapper_isTotal((knh_Mapper_t*)sfp[0].o));
}

static METHOD Mapper_isPartial(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, !(knh_Mapper_isTotal((knh_Mapper_t*)sfp[0].o)));
}

static METHOD Mapper_isConst(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_Mapper_isConst((knh_Mapper_t*)sfp[0].o));
}

static METHOD Mapper_isTemporal(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, !(knh_Mapper_isConst((knh_Mapper_t*)sfp[0].o)));
}

static METHOD Mapper_isLocal(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_Mapper_isLocal((knh_Mapper_t*)sfp[0].o));
}

static METHOD Mapper_isDerived(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_Mapper_isDerived((knh_Mapper_t*)sfp[0].o));
}

static METHOD OutputStream_isAutoFlush(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_OutputStream_isAutoFlush((knh_OutputStream_t*)sfp[0].o));
}

static METHOD OutputStream_setAutoFlush(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_OutputStream_setAutoFlush((knh_OutputStream_t*)sfp[0].o, p_bool(sfp[1]));
	KNH_RETURN_void(ctx, sfp);
}

static METHOD OutputStream_isStoringBuffer(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_OutputStream_isStoringBuffer((knh_OutputStream_t*)sfp[0].o));
}

static METHOD OutputStream_setStoringBuffer(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_OutputStream_setStoringBuffer((knh_OutputStream_t*)sfp[0].o, p_bool(sfp[1]));
	KNH_RETURN_void(ctx, sfp);
}

static METHOD Exception_isLogging(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_Exception_isLogging((knh_Exception_t*)sfp[0].o));
}

static METHOD Context_isStrict(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_Context_isStrict((knh_Context_t*)sfp[0].o));
}

static METHOD Context_setStrict(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Context_setStrict((knh_Context_t*)sfp[0].o, p_bool(sfp[1]));
	KNH_RETURN_void(ctx, sfp);
}

static METHOD Context_isDebug(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_Context_isDebug((knh_Context_t*)sfp[0].o));
}

static METHOD Context_setDebug(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Context_setDebug((knh_Context_t*)sfp[0].o, p_bool(sfp[1]));
	KNH_RETURN_void(ctx, sfp);
}

static METHOD Context_isVerbose(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_Context_isVerbose((knh_Context_t*)sfp[0].o));
}

static METHOD Context_setVerbose(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Context_setVerbose((knh_Context_t*)sfp[0].o, p_bool(sfp[1]));
	KNH_RETURN_void(ctx, sfp);
}

static METHOD Context_isInteractive(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_Context_isInteractive((knh_Context_t*)sfp[0].o));
}

/* ======================================================================== */
/* data */

static char * StringData0[] = {
	"", /* TS_EMPTY */
	"\t", /* TS_TAB */
	""KONOHA_OS_LINEFEED"", /* TS_EOL */
	"BEGIN", /* TS_BEGIN */
	"END", /* TS_END */
	"true", /* TS_true */
	"false", /* TS_false */
	"null", /* TS_null */
	"void", /* TS_void */
	"main", /* TS_main */
	""KONOHA_ENCODING"", /* TS_ENCODING */
	"/dev/null", /* TS_DEVNULL */
	"/dev/stdin", /* TS_DEVSTDIN */
	"/dev/stderr", /* TS_DEVSTDERR */
	"/dev/stdout", /* TS_DEVSTDOUT */
	"@label", /* TS_ATlabel */
	"Comment", /* TS_Comment */
	"Assertion!!", /* TS_AssertionException */
	"(", /* TS_LP */
	")", /* TS_RP */
	"{", /* TS_LB */
	"}", /* TS_RB */
	"[", /* TS_LS */
	"]", /* TS_RS */
	NULL
};

static char * StructNameData[] = {
	"Object",
	"Boolean",
	"Number",
	"Int",
	"Float",
	"String",
	"Bytes",
	"Any",
	"Iterator",
	"Pair",
	"Tuple",
	"Range",
	"Array",
	"IArray",
	"FArray",
	"DictMap",
	"DictSet",
	"HashMap",
	"HashSet",
	"DictIdx",
	"Class",
	"ClassField",
	"MethodField",
	"Method",
	"Mapper",
	"ClassMap",
	"Closure",
	"Thunk",
	"AffineConv",
	"Regex",
	"BytesConv",
	"ClassSpec",
	"InputStream",
	"OutputStream",
	"Socket",
	"Connection",
	"ResultSet",
	"Exception",
	"ExceptionHandler",
	"Script",
	"NameSpace",
	"System",
	"Context",
	"Thread",
	"ScriptEngine",
	"Token",
	"Stmt",
	"Gamma",
	"KLRInst",
	"KLRCode",
	NULL
};

static knh_StructData0_t StructData0[] = {
	{(knh_ObjectFunc_t*)&(ObjectFunc), CLASS_Object, FLAG_Object},
	{(knh_ObjectFunc_t*)&(BooleanFunc), CLASS_Boolean, FLAG_Boolean},
	{(knh_ObjectFunc_t*)&(NumberFunc), CLASS_Number, FLAG_Number},
	{(knh_ObjectFunc_t*)&(IntFunc), CLASS_Int, FLAG_Int},
	{(knh_ObjectFunc_t*)&(FloatFunc), CLASS_Float, FLAG_Float},
	{(knh_ObjectFunc_t*)&(StringFunc), CLASS_String, FLAG_String},
	{(knh_ObjectFunc_t*)&(BytesFunc), CLASS_Bytes, FLAG_Bytes},
	{(knh_ObjectFunc_t*)&(AnyFunc), CLASS_Any, FLAG_Any},
	{(knh_ObjectFunc_t*)&(IteratorFunc), CLASS_Iterator, FLAG_Iterator},
	{(knh_ObjectFunc_t*)&(PairFunc), CLASS_Pair, FLAG_Pair},
	{(knh_ObjectFunc_t*)&(TupleFunc), CLASS_Tuple, FLAG_Tuple},
	{(knh_ObjectFunc_t*)&(RangeFunc), CLASS_Range, FLAG_Range},
	{(knh_ObjectFunc_t*)&(ArrayFunc), CLASS_Array, FLAG_Array},
	{(knh_ObjectFunc_t*)&(IArrayFunc), CLASS_IArray, FLAG_IArray},
	{(knh_ObjectFunc_t*)&(FArrayFunc), CLASS_FArray, FLAG_FArray},
	{(knh_ObjectFunc_t*)&(DictMapFunc), CLASS_DictMap, FLAG_DictMap},
	{(knh_ObjectFunc_t*)&(DictSetFunc), CLASS_DictSet, FLAG_DictSet},
	{(knh_ObjectFunc_t*)&(HashMapFunc), CLASS_HashMap, FLAG_HashMap},
	{(knh_ObjectFunc_t*)&(HashSetFunc), CLASS_HashSet, FLAG_HashSet},
	{(knh_ObjectFunc_t*)&(DictIdxFunc), CLASS_DictIdx, FLAG_DictIdx},
	{(knh_ObjectFunc_t*)&(ClassFunc), CLASS_Class, FLAG_Class},
	{(knh_ObjectFunc_t*)&(ClassFieldFunc), CLASS_ClassField, FLAG_ClassField},
	{(knh_ObjectFunc_t*)&(MethodFieldFunc), CLASS_MethodField, FLAG_MethodField},
	{(knh_ObjectFunc_t*)&(MethodFunc), CLASS_Method, FLAG_Method},
	{(knh_ObjectFunc_t*)&(MapperFunc), CLASS_Mapper, FLAG_Mapper},
	{(knh_ObjectFunc_t*)&(ClassMapFunc), CLASS_ClassMap, FLAG_ClassMap},
	{(knh_ObjectFunc_t*)&(ClosureFunc), CLASS_Closure, FLAG_Closure},
	{(knh_ObjectFunc_t*)&(ThunkFunc), CLASS_Thunk, FLAG_Thunk},
	{(knh_ObjectFunc_t*)&(AffineConvFunc), CLASS_AffineConv, FLAG_AffineConv},
	{(knh_ObjectFunc_t*)&(RegexFunc), CLASS_Regex, FLAG_Regex},
	{(knh_ObjectFunc_t*)&(BytesConvFunc), CLASS_BytesConv, FLAG_BytesConv},
	{(knh_ObjectFunc_t*)&(ClassSpecFunc), CLASS_ClassSpec, FLAG_ClassSpec},
	{(knh_ObjectFunc_t*)&(InputStreamFunc), CLASS_InputStream, FLAG_InputStream},
	{(knh_ObjectFunc_t*)&(OutputStreamFunc), CLASS_OutputStream, FLAG_OutputStream},
	{(knh_ObjectFunc_t*)&(SocketFunc), CLASS_Socket, FLAG_Socket},
	{(knh_ObjectFunc_t*)&(ConnectionFunc), CLASS_Connection, FLAG_Connection},
	{(knh_ObjectFunc_t*)&(ResultSetFunc), CLASS_ResultSet, FLAG_ResultSet},
	{(knh_ObjectFunc_t*)&(ExceptionFunc), CLASS_Exception, FLAG_Exception},
	{(knh_ObjectFunc_t*)&(ExceptionHandlerFunc), CLASS_ExceptionHandler, FLAG_ExceptionHandler},
	{(knh_ObjectFunc_t*)&(ScriptFunc), CLASS_Script, FLAG_Script},
	{(knh_ObjectFunc_t*)&(NameSpaceFunc), CLASS_NameSpace, FLAG_NameSpace},
	{(knh_ObjectFunc_t*)&(SystemFunc), CLASS_System, FLAG_System},
	{(knh_ObjectFunc_t*)&(ContextFunc), CLASS_Context, FLAG_Context},
	{(knh_ObjectFunc_t*)&(ThreadFunc), CLASS_Thread, FLAG_Thread},
	{(knh_ObjectFunc_t*)&(ScriptEngineFunc), CLASS_ScriptEngine, FLAG_ScriptEngine},
	{(knh_ObjectFunc_t*)&(TokenFunc), CLASS_Token, FLAG_Token},
	{(knh_ObjectFunc_t*)&(StmtFunc), CLASS_Stmt, FLAG_Stmt},
	{(knh_ObjectFunc_t*)&(GammaFunc), CLASS_Gamma, FLAG_Gamma},
	{(knh_ObjectFunc_t*)&(KLRInstFunc), CLASS_KLRInst, FLAG_KLRInst},
	{(knh_ObjectFunc_t*)&(KLRCodeFunc), CLASS_KLRCode, FLAG_KLRCode},
	{NULL}
};

static knh_ClassData0_t ClassData0[] = {
	{"konoha.Object", CLASS_Object, FLAG_Object, 
	   CLASS_ObjectField, CLASS_Object, 28, 7, 2,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.Boolean", CLASS_Boolean, FLAG_Boolean, 
	   CLASS_Boolean, CLASS_Object, 1, 4, 1,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.Number", CLASS_Number, FLAG_Number, 
	   CLASS_Number, CLASS_Object, 0, 0, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.Int", CLASS_Int, FLAG_Int, 
	   CLASS_Int, CLASS_Number, 23, 10, 2,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.Float", CLASS_Float, FLAG_Float, 
	   CLASS_Float, CLASS_Number, 18, 7, 2,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.String", CLASS_String, FLAG_String, 
	   CLASS_String, CLASS_Object, 34, 3, 5,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.Bytes", CLASS_Bytes, FLAG_Bytes, 
	   CLASS_Bytes, CLASS_Object, 12, 3, 1,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.Any", CLASS_Any, FLAG_Any, 
	   CLASS_Any, CLASS_Object, 6, 2, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.Iterator", CLASS_Iterator, FLAG_Iterator, 
	   CLASS_Iterator, CLASS_Object, 0, 1, 1,
	   TYPE_void, TYPE_Any, TYPE_void, TYPE_void},
	{"konoha.Pair", CLASS_Pair, FLAG_Pair, 
	   CLASS_Pair, CLASS_Object, 6, 1, 0,
	   TYPE_void, TYPE_Any, TYPE_Any, TYPE_void},
	{"konoha.Tuple", CLASS_Tuple, FLAG_Tuple, 
	   CLASS_Tuple, CLASS_Object, 8, 1, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.Range", CLASS_Range, FLAG_Range, 
	   CLASS_Range, CLASS_Object, 6, 1, 0,
	   TYPE_void, TYPE_Any, TYPE_void, TYPE_void},
	{"konoha.Array", CLASS_Array, FLAG_Array, 
	   CLASS_Array, CLASS_Object, 34, 2, 1,
	   TYPE_void, TYPE_Any, TYPE_void, TYPE_void},
	{"konoha.Int[]", CLASS_IArray, FLAG_IArray, 
	   CLASS_IArray, CLASS_Object, 34, 2, 1,
	   TYPE_void, TYPE_Int, TYPE_void, TYPE_void},
	{"konoha.Float[]", CLASS_FArray, FLAG_FArray, 
	   CLASS_FArray, CLASS_Object, 33, 2, 1,
	   TYPE_void, TYPE_Float, TYPE_void, TYPE_void},
	{"konoha.DictMap", CLASS_DictMap, FLAG_DictMap, 
	   CLASS_DictMap, CLASS_Object, 13, 2, 2,
	   TYPE_void, TYPE_Any, TYPE_void, TYPE_void},
	{"konoha.DictSet", CLASS_DictSet, FLAG_DictSet, 
	   CLASS_DictSet, CLASS_Object, 1, 1, 0,
	   TYPE_void, TYPE_Any, TYPE_void, TYPE_void},
	{"konoha.HashMap", CLASS_HashMap, FLAG_HashMap, 
	   CLASS_HashMap, CLASS_Object, 7, 1, 1,
	   TYPE_void, TYPE_Any, TYPE_Any, TYPE_void},
	{"konoha.HashSet", CLASS_HashSet, FLAG_HashSet, 
	   CLASS_HashSet, CLASS_Object, 0, 0, 0,
	   TYPE_void, TYPE_Any, TYPE_void, TYPE_void},
	{"konoha.DictIdx", CLASS_DictIdx, FLAG_DictIdx, 
	   CLASS_DictIdx, CLASS_Object, 0, 1, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.Class", CLASS_Class, FLAG_Class, 
	   CLASS_Class, CLASS_Object, 11, 4, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.ClassField", CLASS_ClassField, FLAG_ClassField, 
	   CLASS_ClassField, CLASS_Object, 0, 0, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.MethodField", CLASS_MethodField, FLAG_MethodField, 
	   CLASS_MethodField, CLASS_Object, 0, 0, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.Method", CLASS_Method, FLAG_Method, 
	   CLASS_Method, CLASS_Object, 12, 3, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.Mapper", CLASS_Mapper, FLAG_Mapper, 
	   CLASS_Mapper, CLASS_Object, 12, 1, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.ClassMap", CLASS_ClassMap, FLAG_ClassMap, 
	   CLASS_ClassMap, CLASS_Object, 0, 1, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.Closure", CLASS_Closure, FLAG_Closure, 
	   CLASS_Closure, CLASS_Object, 2, 0, 0,
	   TYPE_Any, TYPE_Any, TYPE_Any, TYPE_Any},
	{"konoha.Thunk", CLASS_Thunk, FLAG_Thunk, 
	   CLASS_Thunk, CLASS_Object, 2, 0, 0,
	   TYPE_void, TYPE_Any, TYPE_void, TYPE_void},
	{"konoha.AffineConv", CLASS_AffineConv, FLAG_AffineConv, 
	   CLASS_AffineConv, CLASS_Object, 0, 0, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.Regex", CLASS_Regex, FLAG_Regex, 
	   CLASS_Regex, CLASS_Object, 2, 1, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.BytesConv", CLASS_BytesConv, FLAG_BytesConv, 
	   CLASS_BytesConv, CLASS_Object, 0, 0, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.ClassSpec", CLASS_ClassSpec, FLAG_ClassSpec, 
	   CLASS_ClassSpec, CLASS_Object, 0, 0, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.InputStream", CLASS_InputStream, FLAG_InputStream, 
	   CLASS_InputStream, CLASS_Object, 9, 1, 1,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.OutputStream", CLASS_OutputStream, FLAG_OutputStream, 
	   CLASS_OutputStream, CLASS_Object, 17, 1, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.Socket", CLASS_Socket, FLAG_Socket, 
	   CLASS_Socket, CLASS_Object, 5, 0, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.Connection", CLASS_Connection, FLAG_Connection, 
	   CLASS_Connection, CLASS_Object, 4, 0, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.ResultSet", CLASS_ResultSet, FLAG_ResultSet, 
	   CLASS_ResultSet, CLASS_Object, 8, 1, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.Exception", CLASS_Exception, FLAG_Exception, 
	   CLASS_Exception, CLASS_Object, 4, 4, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.ExceptionHandler", CLASS_ExceptionHandler, FLAG_ExceptionHandler, 
	   CLASS_ExceptionHandler, CLASS_Object, 0, 0, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.Script", CLASS_Script, FLAG_Script, 
	   CLASS_Script, CLASS_Object, 5, 1, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.NameSpace", CLASS_NameSpace, FLAG_NameSpace, 
	   CLASS_NameSpace, CLASS_Object, 1, 0, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.System", CLASS_System, FLAG_System, 
	   CLASS_System, CLASS_Object, 19, 0, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.Context", CLASS_Context, FLAG_Context, 
	   CLASS_Context, CLASS_Object, 14, 1, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.Thread", CLASS_Thread, FLAG_Thread, 
	   CLASS_Thread, CLASS_Object, 1, 0, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.ScriptEngine", CLASS_ScriptEngine, FLAG_ScriptEngine, 
	   CLASS_ScriptEngine, CLASS_Object, 0, 0, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.Token", CLASS_Token, FLAG_Token, 
	   CLASS_Token, CLASS_Object, 0, 3, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.Stmt", CLASS_Stmt, FLAG_Stmt, 
	   CLASS_Stmt, CLASS_Object, 0, 2, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.Gamma", CLASS_Gamma, FLAG_Gamma, 
	   CLASS_Gamma, CLASS_Object, 0, 0, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.KLRInst", CLASS_KLRInst, FLAG_KLRInst, 
	   CLASS_KLRInst, CLASS_Object, 0, 0, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.KLRCode", CLASS_KLRCode, FLAG_KLRCode, 
	   CLASS_KLRCode, CLASS_Object, 0, 0, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.This", CLASS_This, FLAG_This, 
	   CLASS_Any, CLASS_Any, 0, 0, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.Tvoid", CLASS_Tvoid, FLAG_Tvoid, 
	   CLASS_Any, CLASS_Any, 0, 0, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.T0", CLASS_T0, FLAG_T0, 
	   CLASS_Any, CLASS_Any, 0, 0, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.T1", CLASS_T1, FLAG_T1, 
	   CLASS_Any, CLASS_Any, 0, 0, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.T2", CLASS_T2, FLAG_T2, 
	   CLASS_Any, CLASS_Any, 0, 0, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.T3", CLASS_T3, FLAG_T3, 
	   CLASS_Any, CLASS_Any, 0, 0, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.Tx", CLASS_Tx, FLAG_Tx, 
	   CLASS_Any, CLASS_Any, 0, 0, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.Tvar", CLASS_Tvar, FLAG_Tvar, 
	   CLASS_Any, CLASS_Any, 0, 0, 0,
	   TYPE_void, TYPE_void, TYPE_void, TYPE_void},
	{"konoha.Pair<String,String>", CLASS_PairSS, FLAG_PairSS, 
	   CLASS_Pair, CLASS_Pair, 0, 0, 0,
	   TYPE_void, TYPE_String, TYPE_String, TYPE_void},
	{"konoha.Pair<String,T1>", CLASS_PairST1, FLAG_PairST1, 
	   CLASS_Pair, CLASS_Pair, 0, 0, 0,
	   TYPE_void, TYPE_String, TYPE_T1, TYPE_void},
	{"konoha.Pair<T1,T2>", CLASS_PairT1T2, FLAG_PairT1T2, 
	   CLASS_Pair, CLASS_Pair, 0, 0, 0,
	   TYPE_void, TYPE_T1, TYPE_T2, TYPE_void},
	{"konoha.int(T1,T1)", CLASS_Cmpr, FLAG_Cmpr, 
	   CLASS_Closure, CLASS_Closure, 0, 0, 0,
	   NNTYPE_Int, NNTYPE_T1, NNTYPE_T1, TYPE_void},
	{"konoha.String..", CLASS_String_Itr, FLAG_String_Itr, 
	   CLASS_Iterator, CLASS_Iterator, 0, 0, 0,
	   TYPE_void, TYPE_String, TYPE_void, TYPE_void},
	{"konoha.T1..", CLASS_T1_Itr, FLAG_T1_Itr, 
	   CLASS_Iterator, CLASS_Iterator, 0, 0, 0,
	   TYPE_void, TYPE_T1, TYPE_void, TYPE_void},
	{"konoha.Pair<String,T1>..", CLASS_PairST1_Itr, FLAG_PairST1_Itr, 
	   CLASS_Iterator, CLASS_Iterator, 0, 0, 0,
	   TYPE_void, TYPE_PairST1, TYPE_void, TYPE_void},
	{"konoha.Pair<T1,T2>..", CLASS_PairT1T2_Itr, FLAG_PairT1T2_Itr, 
	   CLASS_Iterator, CLASS_Iterator, 0, 0, 0,
	   TYPE_void, TYPE_PairT1T2, TYPE_void, TYPE_void},
	{"konoha.This..", CLASS_This_Itr, FLAG_This_Itr, 
	   CLASS_Iterator, CLASS_Iterator, 0, 0, 0,
	   TYPE_void, TYPE_This, TYPE_void, TYPE_void},
	{"konoha.void(Array)", CLASS_Thrd, FLAG_Thrd, 
	   CLASS_Closure, CLASS_Closure, 0, 0, 0,
	   TYPE_void, NNTYPE_Array, TYPE_void, TYPE_void},
	{"konoha.String[]", CLASS_String_Ary, FLAG_String_Ary, 
	   CLASS_Array, CLASS_Object, 0, 0, 0,
	   TYPE_void, TYPE_String, TYPE_void, TYPE_void},
	{NULL}
};

static knh_ExptData0_t ExptData0[] = {
	{"Exception", 0, EXPT_Exception, EXPT_Exception},
	{"Fatal", 0, EXPT_Fatal, EXPT_Exception},
	{"OutOfMemory", 0, EXPT_OutOfMemory, EXPT_Fatal},
	{"Security", 0, EXPT_Security, EXPT_Exception},
	{"Null", 0, EXPT_Null, EXPT_Exception},
	{"OutOfIndex", 0, EXPT_OutOfIndex, EXPT_Exception},
	{"IO", 0, EXPT_IO, EXPT_Exception},
	{"Script", 0, EXPT_Script, EXPT_Exception},
	{"Syntax", 0, EXPT_Syntax, EXPT_Script},
	{"Type", 0, EXPT_Type, EXPT_Script},
	{"Assertion", 0, EXPT_Assertion, EXPT_Script},
	{NULL}
};

static knh_FieldNameData0_t FieldNameData0[] = {
	{"", FIELDN_},
	{"2D", FIELDN_2D},
	{"3D", FIELDN_3D},
	{"abstract", FIELDN_abstract},
	{"add", FIELDN_add},
	{"addHistory", FIELDN_addHistory},
	{"all", FIELDN_all},
	{"anno", FIELDN_anno},
	{"args", FIELDN_args},
	{"ascii", FIELDN_ascii},
	{"autoFlush", FIELDN_autoFlush},
	{"bag", FIELDN_bag},
	{"base", FIELDN_base},
	{"bits", FIELDN_bits},
	{"buf", FIELDN_buf},
	{"bytes", FIELDN_bytes},
	{"c", FIELDN_c},
	{"cc", FIELDN_cc},
	{"ch", FIELDN_ch},
	{"char", FIELDN_char},
	{"class", FIELDN_class},
	{"clear", FIELDN_clear},
	{"clearBuffer", FIELDN_clearBuffer},
	{"close", FIELDN_close},
	{"closed", FIELDN_closed},
	{"cmd", FIELDN_cmd},
	{"concat", FIELDN_concat},
	{"const", FIELDN_const},
	{"d", FIELDN_d},
	{"data", FIELDN_data},
	{"debug", FIELDN_debug},
	{"default", FIELDN_default},
	{"defined", FIELDN_defined},
	{"delegate", FIELDN_delegate},
	{"delim", FIELDN_delim},
	{"derived", FIELDN_derived},
	{"domain", FIELDN_domain},
	{"dump", FIELDN_dump},
	{"e", FIELDN_e},
	{"empty", FIELDN_empty},
	{"enc", FIELDN_enc},
	{"encoding", FIELDN_encoding},
	{"end", FIELDN_end},
	{"endsWith", FIELDN_endsWith},
	{"endsWith:IgnoreCase", FIELDN_endsWith__IgnoreCase},
	{"equals", FIELDN_equals},
	{"equals:IgnoreCase", FIELDN_equals__IgnoreCase},
	{"err", FIELDN_err},
	{"eval", FIELDN_eval},
	{"exec", FIELDN_exec},
	{"exit", FIELDN_exit},
	{"f", FIELDN_f},
	{"final", FIELDN_final},
	{"first", FIELDN_first},
	{"flag", FIELDN_flag},
	{"float", FIELDN_float},
	{"floatToIntBits", FIELDN_floatToIntBits},
	{"flush", FIELDN_flush},
	{"fmt", FIELDN_fmt},
	{"format", FIELDN_format},
	{"func", FIELDN_func},
	{"gc", FIELDN_gc},
	{"hasDir", FIELDN_hasDir},
	{"hasFile", FIELDN_hasFile},
	{"hashCode", FIELDN_hashCode},
	{"hasLib", FIELDN_hasLib},
	{"host", FIELDN_host},
	{"iCast", FIELDN_iCast},
	{"ignoreCase", FIELDN_ignoreCase},
	{"immutable", FIELDN_immutable},
	{"in", FIELDN_in},
	{"indexOf", FIELDN_indexOf},
	{"indexOf:IgnoreCase", FIELDN_indexOf__IgnoreCase},
	{"init", FIELDN_init},
	{"initCapacity", FIELDN_initCapacity},
	{"inputStream", FIELDN_inputStream},
	{"insert", FIELDN_insert},
	{"int", FIELDN_int},
	{"interactive", FIELDN_interactive},
	{"interface", FIELDN_interface},
	{"intToFloatBits", FIELDN_intToFloatBits},
	{"invoke", FIELDN_invoke},
	{"isInclusive", FIELDN_isInclusive},
	{"isTrim", FIELDN_isTrim},
	{"k", FIELDN_k},
	{"key", FIELDN_key},
	{"lastIndexOf", FIELDN_lastIndexOf},
	{"lastIndexOf:IgnoreCase", FIELDN_lastIndexOf__IgnoreCase},
	{"length", FIELDN_length},
	{"lib", FIELDN_lib},
	{"likely", FIELDN_likely},
	{"line", FIELDN_line},
	{"listDir", FIELDN_listDir},
	{"listProperties", FIELDN_listProperties},
	{"local", FIELDN_local},
	{"logging", FIELDN_logging},
	{"lossLess", FIELDN_lossLess},
	{"m", FIELDN_m},
	{"main", FIELDN_main},
	{"man", FIELDN_man},
	{"match", FIELDN_match},
	{"memcpy", FIELDN_memcpy},
	{"metaClass", FIELDN_metaClass},
	{"method", FIELDN_method},
	{"methodCompilationListener", FIELDN_methodCompilationListener},
	{"methodTypingListener", FIELDN_methodTypingListener},
	{"mkdir", FIELDN_mkdir},
	{"mode", FIELDN_mode},
	{"modified", FIELDN_modified},
	{"msg", FIELDN_msg},
	{"n", FIELDN_n},
	{"name", FIELDN_name},
	{"new", FIELDN_new},
	{"new:array", FIELDN_new__array},
	{"new:array2D", FIELDN_new__array2D},
	{"new:array3D", FIELDN_new__array3D},
	{"new:dictmap", FIELDN_new__dictmap},
	{"new:init", FIELDN_new__init},
	{"new:range", FIELDN_new__range},
	{"newone", FIELDN_newone},
	{"newpath", FIELDN_newpath},
	{"next", FIELDN_next},
	{"notNull", FIELDN_notNull},
	{"null", FIELDN_null},
	{"o", FIELDN_o},
	{"offset", FIELDN_offset},
	{"op0", FIELDN_op0},
	{"op1", FIELDN_op1},
	{"opAdd", FIELDN_opAdd},
	{"opAddr", FIELDN_opAddr},
	{"opAppend", FIELDN_opAppend},
	{"opAs", FIELDN_opAs},
	{"opCase", FIELDN_opCase},
	{"opDiv", FIELDN_opDiv},
	{"opEq", FIELDN_opEq},
	{"opGt", FIELDN_opGt},
	{"opGte", FIELDN_opGte},
	{"opHas", FIELDN_opHas},
	{"opInstanceof", FIELDN_opInstanceof},
	{"opInTo", FIELDN_opInTo},
	{"opIs", FIELDN_opIs},
	{"opItr", FIELDN_opItr},
	{"opItr:2", FIELDN_opItr__2},
	{"opLand", FIELDN_opLand},
	{"opLnot", FIELDN_opLnot},
	{"opLor", FIELDN_opLor},
	{"opLshift", FIELDN_opLshift},
	{"opLt", FIELDN_opLt},
	{"opLte", FIELDN_opLte},
	{"opMatch", FIELDN_opMatch},
	{"opMod", FIELDN_opMod},
	{"opMul", FIELDN_opMul},
	{"opN", FIELDN_opN},
	{"opNeg", FIELDN_opNeg},
	{"opNeq", FIELDN_opNeq},
	{"opNext", FIELDN_opNext},
	{"opNot", FIELDN_opNot},
	{"opPrev", FIELDN_opPrev},
	{"opRangeTo", FIELDN_opRangeTo},
	{"opRangeUntil", FIELDN_opRangeUntil},
	{"opRshift", FIELDN_opRshift},
	{"opSub", FIELDN_opSub},
	{"option", FIELDN_option},
	{"opXor", FIELDN_opXor},
	{"out", FIELDN_out},
	{"outputStream", FIELDN_outputStream},
	{"p", FIELDN_p},
	{"partial", FIELDN_partial},
	{"path", FIELDN_path},
	{"pattern", FIELDN_pattern},
	{"pop", FIELDN_pop},
	{"port", FIELDN_port},
	{"print", FIELDN_print},
	{"println", FIELDN_println},
	{"private", FIELDN_private},
	{"profCount", FIELDN_profCount},
	{"profTime", FIELDN_profTime},
	{"prompt", FIELDN_prompt},
	{"property", FIELDN_property},
	{"public", FIELDN_public},
	{"putc", FIELDN_putc},
	{"putChar", FIELDN_putChar},
	{"query", FIELDN_query},
	{"random", FIELDN_random},
	{"randomSeed", FIELDN_randomSeed},
	{"re", FIELDN_re},
	{"read", FIELDN_read},
	{"readData", FIELDN_readData},
	{"readLine", FIELDN_readLine},
	{"release", FIELDN_release},
	{"remove", FIELDN_remove},
	{"rename", FIELDN_rename},
	{"replace", FIELDN_replace},
	{"result", FIELDN_result},
	{"reverse", FIELDN_reverse},
	{"s", FIELDN_s},
	{"script", FIELDN_script},
	{"second", FIELDN_second},
	{"seed", FIELDN_seed},
	{"shared", FIELDN_shared},
	{"shuffle", FIELDN_shuffle},
	{"significant", FIELDN_significant},
	{"singleton", FIELDN_singleton},
	{"size", FIELDN_size},
	{"sort", FIELDN_sort},
	{"sortByClosure", FIELDN_sortByClosure},
	{"split", FIELDN_split},
	{"start", FIELDN_start},
	{"startsWith", FIELDN_startsWith},
	{"startsWith:IgnoreCase", FIELDN_startsWith__IgnoreCase},
	{"static", FIELDN_static},
	{"status", FIELDN_status},
	{"stmt", FIELDN_stmt},
	{"storingBuffer", FIELDN_storingBuffer},
	{"strict", FIELDN_strict},
	{"string", FIELDN_string},
	{"substring", FIELDN_substring},
	{"super", FIELDN_super},
	{"swap", FIELDN_swap},
	{"synonym", FIELDN_synonym},
	{"system", FIELDN_system},
	{"temporal", FIELDN_temporal},
	{"test", FIELDN_test},
	{"this", FIELDN_this},
	{"tickCount", FIELDN_tickCount},
	{"times", FIELDN_times},
	{"toLower", FIELDN_toLower},
	{"total", FIELDN_total},
	{"toUpper", FIELDN_toUpper},
	{"trace", FIELDN_trace},
	{"trim", FIELDN_trim},
	{"twofold", FIELDN_twofold},
	{"typeof", FIELDN_typeof},
	{"u", FIELDN_u},
	{"uCS4", FIELDN_uCS4},
	{"undefined", FIELDN_undefined},
	{"unique", FIELDN_unique},
	{"unlikely", FIELDN_unlikely},
	{"unlink", FIELDN_unlink},
	{"urn", FIELDN_urn},
	{"v", FIELDN_v},
	{"value", FIELDN_value},
	{"varArgs", FIELDN_varArgs},
	{"vargs", FIELDN_vargs},
	{"verbose", FIELDN_verbose},
	{"virtual", FIELDN_virtual},
	{"w", FIELDN_w},
	{"write", FIELDN_write},
	{"writeData", FIELDN_writeData},
	{"x", FIELDN_x},
	{"y", FIELDN_y},
	{"z", FIELDN_z},
	{NULL}
};

static knh_MethodFieldData0_t MethodFieldData0[] = {
	{0, 0, TYPE_void},
	{1, 1, TYPE_void, NNTYPE_Int, FIELDN_char},
	{2, 3, TYPE_void, NNTYPE_Bytes, FIELDN_buf, NATYPE_Int, FIELDN_offset, NATYPE_Int, FIELDN_length},
	{3, 4, TYPE_void, TYPE_Int, FIELDN_offset, NNTYPE_Bytes, FIELDN_buf, NATYPE_Int, FIELDN_offset, NATYPE_Int, FIELDN_length},
	{4, 1, TYPE_void, TYPE_T1, FIELDN_value},
	{5, 1, TYPE_void, NNTYPE_Int, FIELDN_value},
	{6, 1, TYPE_void, NNTYPE_Float, FIELDN_value},
	{7, 2, TYPE_void, NNTYPE_Int, FIELDN_n, NNTYPE_T1, FIELDN_value},
	{8, 2, TYPE_void, NNTYPE_Int, FIELDN_n, NNTYPE_Int, FIELDN_value},
	{9, 2, TYPE_void, NNTYPE_Int, FIELDN_n, NNTYPE_Float, FIELDN_value},
	{10, 1, TYPE_void, NNTYPE_Int, FIELDN_n},
	{11, 0, NNTYPE_T1},
	{12, 1, NNTYPE_Int, TYPE_T1, FIELDN_value},
	{13, 1, NNTYPE_Int, NNTYPE_Int, FIELDN_value},
	{14, 1, NNTYPE_Int, NNTYPE_Float, FIELDN_value},
	{15, 2, TYPE_void, NNTYPE_Int, FIELDN_m, NNTYPE_Int, FIELDN_n},
	{16, 2, NNTYPE_This, NNTYPE_Int, FIELDN_x, NNTYPE_Int, FIELDN_y},
	{17, 3, NNTYPE_This, NNTYPE_Int, FIELDN_x, NNTYPE_Int, FIELDN_y, NNTYPE_Int, FIELDN_z},
	{18, 2, TYPE_T1, NNTYPE_Int, FIELDN_x, NNTYPE_Int, FIELDN_y},
	{19, 3, TYPE_T1, NNTYPE_Int, FIELDN_x, NNTYPE_Int, FIELDN_y, NNTYPE_Int, FIELDN_z},
	{20, 3, TYPE_void, NNTYPE_Int, FIELDN_x, NNTYPE_Int, FIELDN_y, TYPE_T1, FIELDN_value},
	{21, 4, TYPE_void, NNTYPE_Int, FIELDN_x, NNTYPE_Int, FIELDN_y, NNTYPE_Int, FIELDN_z, TYPE_T1, FIELDN_value},
	{22, 2, TYPE_void, TYPE_OutputStream, FIELDN_w, TYPE_String, FIELDN_m},
	{23, 1, NNTYPE_This, NNTYPE_String, FIELDN_urn},
	{24, 1, NNTYPE_ResultSet, NNTYPE_String, FIELDN_query},
	{25, 1, TYPE_void, NNTYPE_String, FIELDN_query},
	{26, 0, NNTYPE_Int},
	{27, 0, NNTYPE_Boolean},
	{28, 1, NNTYPE_String, NNTYPE_Int, FIELDN_n},
	{29, 1, TYPE_Int, TYPE_Any, FIELDN_n},
	{30, 1, TYPE_Float, TYPE_Any, FIELDN_n},
	{31, 1, TYPE_String, TYPE_Any, FIELDN_n},
	{32, 1, TYPE_Any, TYPE_Any, FIELDN_n},
	{33, 2, TYPE_void, TYPE_OutputStream, FIELDN_w, NATYPE_String, FIELDN_m},
	{34, 1, NNTYPE_Boolean, NNTYPE_String, FIELDN_key},
	{35, 1, NATYPE_T1, NNTYPE_String, FIELDN_key},
	{36, 2, TYPE_void, NNTYPE_String, FIELDN_key, NATYPE_T1, FIELDN_value},
	{37, 1, TYPE_void, NNTYPE_String, FIELDN_key},
	{38, 2, TYPE_void, TYPE_OutputStream, FIELDN_w, NATYPE_String, FIELDN_fmt},
	{39, 1, NNTYPE_This, NATYPE_Int, FIELDN_initCapacity},
	{40, 1, TYPE_T2, TYPE_T1, FIELDN_key},
	{41, 1, NNTYPE_Boolean, TYPE_T1, FIELDN_key},
	{42, 1, TYPE_void, TYPE_T1, FIELDN_key},
	{43, 2, TYPE_void, NNTYPE_T1, FIELDN_key, TYPE_T2, FIELDN_value},
	{44, 0, TYPE_String_Itr},
	{45, 0, TYPE_T1_Itr},
	{46, 0, TYPE_PairST1_Itr},
	{47, 0, TYPE_PairT1T2_Itr},
	{48, 0, TYPE_This_Itr},
	{49, 1, NNTYPE_Boolean, NNTYPE_String, FIELDN_script},
	{50, 1, TYPE_void, TYPE_String, FIELDN_script},
	{51, 1, TYPE_String, TYPE_String, FIELDN_prompt},
	{52, 1, TYPE_void, NNTYPE_String, FIELDN_line},
	{53, 2, TYPE_void, TYPE_Closure, FIELDN_c, TYPE_String, FIELDN_anno},
	{54, 0, TYPE_Int},
	{55, 1, TYPE_void, TYPE_Cmpr, FIELDN_cc},
	{56, 0, NNTYPE_This},
	{57, 1, NNTYPE_This, TYPE_Any, FIELDN_value},
	{58, 1, NNTYPE_This, NATYPE_Int, FIELDN_init},
	{59, 1, NNTYPE_Bytes, TYPE_Int, FIELDN_init},
	{60, 2, NNTYPE_String, NNTYPE_Bytes, FIELDN_buf, NATYPE_String, FIELDN_enc},
	{61, 2, TYPE_Regex, NNTYPE_String, FIELDN_pattern, NATYPE_String, FIELDN_option},
	{62, 2, NNTYPE_This, TYPE_T1, FIELDN_first, TYPE_T2, FIELDN_second},
	{63, 1, NNTYPE_This, NATYPE_Any, FIELDN_value},
	{64, 2, NNTYPE_This, NNTYPE_T1, FIELDN_start, NNTYPE_T1, FIELDN_end},
	{65, 1, NNTYPE_This, TYPE_Int, FIELDN_size},
	{66, 1, NNTYPE_This, NATYPE_T1, FIELDN_value},
	{67, 1, NNTYPE_This, NATYPE_Int, FIELDN_value},
	{68, 3, NNTYPE_This, NNTYPE_Int, FIELDN_start, NNTYPE_Int, FIELDN_end, NATYPE_Int, FIELDN_isInclusive},
	{69, 1, NNTYPE_This, NATYPE_Float, FIELDN_value},
	{70, 1, NNTYPE_This, TYPE_T1, FIELDN_value},
	{71, 2, NNTYPE_Exception, NATYPE_String, FIELDN_msg, NATYPE_Any, FIELDN_bag},
	{72, 3, NNTYPE_This, NATYPE_String, FIELDN_e, NATYPE_String, FIELDN_msg, NATYPE_Object, FIELDN_bag},
	{73, 2, NNTYPE_This, NNTYPE_Any, FIELDN_base, NNTYPE_Method, FIELDN_method},
	{74, 2, NNTYPE_Thread, NNTYPE_Thrd, FIELDN_c, NNTYPE_Array, FIELDN_args},
	{75, 1, TYPE_void, TYPE_Int, FIELDN_seed},
	{76, 1, NNTYPE_Int, NATYPE_Int, FIELDN_n},
	{77, 0, NNTYPE_Float},
	{78, 1, NNTYPE_Int, NNTYPE_Float, FIELDN_n},
	{79, 1, NNTYPE_Float, NNTYPE_Int, FIELDN_n},
	{80, 0, NNTYPE_Class},
	{81, 0, NNTYPE_String},
	{82, 0, NNTYPE_Array},
	{83, 1, TYPE_void, TYPE_Int, FIELDN_trace},
	{84, 3, TYPE_T0, TYPE_T1, FIELDN_x, TYPE_T2, FIELDN_y, TYPE_T3, FIELDN_z},
	{85, 1, TYPE_Boolean, TYPE_Class, FIELDN_c},
	{86, 1, NNTYPE_Boolean, TYPE_Any, FIELDN_value},
	{87, 1, NNTYPE_Boolean, NNTYPE_Int, FIELDN_value},
	{88, 1, NNTYPE_Boolean, TYPE_Float, FIELDN_value},
	{89, 1, NNTYPE_Boolean, NNTYPE_String, FIELDN_s},
	{90, 1, NNTYPE_Boolean, TYPE_Any, FIELDN_v},
	{91, 1, NNTYPE_Boolean, TYPE_Any, FIELDN_o},
	{92, 1, NNTYPE_Int, NNTYPE_Int, FIELDN_v},
	{93, 1, NNTYPE_Int, NNTYPE_Int, FIELDN_n},
	{94, 1, NNTYPE_Float, NNTYPE_Float, FIELDN_v},
	{95, 1, NNTYPE_Float, NNTYPE_Float, FIELDN_n},
	{96, 1, NNTYPE_String, TYPE_Any, FIELDN_value},
	{97, 1, NNTYPE_String, NNTYPE_String, FIELDN_s},
	{98, 1, TYPE_Any, TYPE_Any, FIELDN_value},
	{99, 0, TYPE_Any},
	{100, 2, TYPE_void, NNTYPE_Int, FIELDN_n, NNTYPE_Int, FIELDN_c},
	{101, 1, TYPE_void, NNTYPE_Int, FIELDN_c},
	{102, 1, NNTYPE_T1, NNTYPE_Int, FIELDN_n},
	{103, 1, TYPE_Any, NNTYPE_Int, FIELDN_n},
	{104, 2, TYPE_void, NNTYPE_Int, FIELDN_n, NNTYPE_T1, FIELDN_v},
	{105, 2, TYPE_void, NNTYPE_Int, FIELDN_n, NNTYPE_Int, FIELDN_v},
	{106, 2, TYPE_void, NNTYPE_Int, FIELDN_n, NNTYPE_Float, FIELDN_v},
	{107, 1, TYPE_void, TYPE_T1, FIELDN_v},
	{108, 1, TYPE_void, NNTYPE_Int, FIELDN_v},
	{109, 1, TYPE_void, NNTYPE_Float, FIELDN_v},
	{110, 2, NNTYPE_Bytes, NATYPE_Int, FIELDN_s, NATYPE_Int, FIELDN_e},
	{111, 2, NNTYPE_String, NATYPE_Int, FIELDN_offset, NATYPE_Int, FIELDN_length},
	{112, 2, NNTYPE_String, NATYPE_Int, FIELDN_s, NATYPE_Int, FIELDN_e},
	{113, 2, NNTYPE_This, NATYPE_Int, FIELDN_s, NATYPE_Int, FIELDN_e},
	{114, 2, NNTYPE_This, NATYPE_String, FIELDN_s, NATYPE_String, FIELDN_e},
	{115, 0, NNTYPE_T2},
	{116, 2, TYPE_InputStream, NNTYPE_String, FIELDN_urn, NATYPE_String, FIELDN_mode},
	{117, 3, NNTYPE_Int, NNTYPE_Bytes, FIELDN_buf, NATYPE_Int, FIELDN_offset, NATYPE_Int, FIELDN_length},
	{118, 1, TYPE_void, NATYPE_String, FIELDN_enc},
	{119, 0, NATYPE_String},
	{120, 0, NATYPE_Any},
	{121, 2, TYPE_OutputStream, NNTYPE_String, FIELDN_urn, NATYPE_String, FIELDN_mode},
	{122, 1, TYPE_void, NNTYPE_Int, FIELDN_ch},
	{123, 1, TYPE_void, NATYPE_Any, FIELDN_value},
	{124, 1, TYPE_void, NATYPE_Any, FIELDN_data},
	{125, 2, NNTYPE_Socket, NNTYPE_String, FIELDN_host, NNTYPE_Int, FIELDN_port},
	{126, 0, NNTYPE_InputStream},
	{127, 0, NNTYPE_OutputStream},
	{128, 1, NNTYPE_Bytes, NATYPE_String, FIELDN_enc},
	{129, 1, NNTYPE_Int, NNTYPE_String, FIELDN_s},
	{130, 1, NNTYPE_String, NATYPE_Any, FIELDN_value},
	{131, 1, TYPE_PairSS, NNTYPE_String, FIELDN_s},
	{132, 2, NNTYPE_String, NNTYPE_String, FIELDN_s, NNTYPE_String, FIELDN_newone},
	{133, 2, TYPE_String_Ary, TYPE_String, FIELDN_delim, NATYPE_Int, FIELDN_isTrim},
	{134, 1, NNTYPE_Boolean, NNTYPE_Regex, FIELDN_re},
	{135, 1, TYPE_String_Ary, TYPE_Regex, FIELDN_pattern},
	{136, 1, NATYPE_Any, NNTYPE_String, FIELDN_key},
	{137, 2, TYPE_void, NNTYPE_String, FIELDN_key, NATYPE_Any, FIELDN_value},
	{138, 2, TYPE_void, TYPE_Boolean, FIELDN_result, NATYPE_String, FIELDN_msg},
	{139, 1, TYPE_Int, TYPE_String, FIELDN_cmd},
	{140, 2, NNTYPE_Boolean, NNTYPE_String, FIELDN_lib, NATYPE_String, FIELDN_func},
	{141, 1, TYPE_void, TYPE_Int, FIELDN_status},
	{142, 1, TYPE_String_Ary, NATYPE_String, FIELDN_path},
	{143, 1, NNTYPE_Boolean, NNTYPE_String, FIELDN_path},
	{144, 2, NNTYPE_Boolean, TYPE_String, FIELDN_path, TYPE_String, FIELDN_newpath},
	{145, 1, TYPE_Any, NNTYPE_String, FIELDN_key},
	{146, 1, TYPE_String_Ary, TYPE_String, FIELDN_key},
	{147, 1, TYPE_void, NATYPE_InputStream, FIELDN_in},
	{148, 1, TYPE_void, NATYPE_OutputStream, FIELDN_out},
	{149, 1, NNTYPE_Boolean, TYPE_Any, FIELDN_msg},
	{150, 2, TYPE_void, NNTYPE_String, FIELDN_name, TYPE_Any, FIELDN_value},
	{151, 1, TYPE_void, NNTYPE_Boolean, FIELDN_flag},
	{-1}
};
#define KNH_TMETHODFIELD_SIZE           152

static knh_MethodData0_t MethodData0[] = {
	{Bytes_putc, 0, CLASS_Bytes, METHODN_putc, 0, 1, NULL},
	{Bytes_write, 0, CLASS_Bytes, METHODN_write, 0, 2, NULL},
	{Bytes_write, 0, CLASS_Bytes, METHODN_add, 0, 2, NULL},
	{Bytes_memcpy, 0, CLASS_Bytes, METHODN_memcpy, 0, 3, NULL},
	{Array_add, 0, CLASS_Array, METHODN_add, 0, 4, NULL},
	{IArray_add, 0, CLASS_IArray, METHODN_add, 0, 5, NULL},
	{FArray_add, 0, CLASS_FArray, METHODN_add, 0, 6, NULL},
	{Array_opAppend, FLAG_Method_VarArgs, CLASS_Array, METHODN_opAppend, 0, 4, NULL},
	{IArray_opAppend, FLAG_Method_VarArgs, CLASS_IArray, METHODN_opAppend, 0, 5, NULL},
	{FArray_opAppend, FLAG_Method_VarArgs, CLASS_FArray, METHODN_opAppend, 0, 6, NULL},
	{Array_insert, 0, CLASS_Array, METHODN_insert, 0, 7, NULL},
	{IArray_insert, 0, CLASS_IArray, METHODN_insert, 0, 8, NULL},
	{FArray_insert, 0, CLASS_FArray, METHODN_insert, 0, 9, NULL},
	{Array_clear, 0, CLASS_Array, METHODN_clear, 0, 0, NULL},
	{IArray_clear, 0, CLASS_IArray, METHODN_clear, 0, 0, NULL},
	{FArray_clear, 0, CLASS_FArray, METHODN_clear, 0, 0, NULL},
	{Array_remove, 0, CLASS_Array, METHODN_remove, 0, 10, NULL},
	{IArray_remove, 0, CLASS_IArray, METHODN_remove, 0, 10, NULL},
	{FArray_remove, 0, CLASS_FArray, METHODN_remove, 0, 10, NULL},
	{Array_pop, 0, CLASS_Array, METHODN_pop, 0, 11, NULL},
	{IArray_pop, 0, CLASS_IArray, METHODN_pop, 0, 11, NULL},
	{FArray_pop, 0, CLASS_FArray, METHODN_pop, 0, 11, NULL},
	{Array_indexOf, 0, CLASS_Array, METHODN_indexOf, 0, 12, NULL},
	{IArray_indexOf, 0, CLASS_IArray, METHODN_indexOf, 0, 13, NULL},
	{FArray_indexOf, 0, CLASS_FArray, METHODN_indexOf, 0, 14, NULL},
	{Array_lastIndexOf, 0, CLASS_Array, METHODN_lastIndexOf, 0, 12, NULL},
	{IArray_lastIndexOf, 0, CLASS_IArray, METHODN_lastIndexOf, 0, 13, NULL},
	{FArray_lastIndexOf, 0, CLASS_FArray, METHODN_lastIndexOf, 0, 14, NULL},
	{Array_sort, 0, CLASS_Array, METHODN_sort, 0, 0, NULL},
	{IArray_sort, 0, CLASS_IArray, METHODN_sort, 0, 0, NULL},
	{FArray_sort, 0, CLASS_FArray, METHODN_sort, 0, 0, NULL},
	{Array_reverse, 0, CLASS_Array, METHODN_reverse, 0, 0, NULL},
	{IArray_reverse, 0, CLASS_IArray, METHODN_reverse, 0, 0, NULL},
	{FArray_reverse, 0, CLASS_FArray, METHODN_reverse, 0, 0, NULL},
	{Array_swap, 0, CLASS_Array, METHODN_swap, 0, 15, NULL},
	{IArray_swap, 0, CLASS_IArray, METHODN_swap, 0, 15, NULL},
	{FArray_swap, 0, CLASS_FArray, METHODN_swap, 0, 15, NULL},
	{Array_shuffle, 0, CLASS_Array, METHODN_shuffle, 0, 0, NULL},
	{IArray_shuffle, 0, CLASS_IArray, METHODN_shuffle, 0, 0, NULL},
	{FArray_shuffle, 0, CLASS_FArray, METHODN_shuffle, 0, 0, NULL},
	{Array_new__array2D, FLAG_Method_Private, CLASS_Array, METHODN_new__array2D, 0, 16, NULL},
	{Array_new__array3D, FLAG_Method_Private, CLASS_Array, METHODN_new__array3D, 0, 17, NULL},
	{Array_get2D, FLAG_Method_Hidden, CLASS_Array, METHODN_get2D, 0, 18, NULL},
	{Array_get3D, FLAG_Method_Hidden, CLASS_Array, METHODN_get3D, 0, 19, NULL},
	{Array_set2D, FLAG_Method_Hidden, CLASS_Array, METHODN_set2D, 0, 20, NULL},
	{Array_set3D, FLAG_Method_Hidden, CLASS_Array, METHODN_set3D, 0, 21, NULL},
	{IArray_new__array2D, FLAG_Method_Private, CLASS_IArray, METHODN_new__array2D, 0, 16, NULL},
	{IArray_new__array3D, FLAG_Method_Private, CLASS_IArray, METHODN_new__array3D, 0, 17, NULL},
	{IArray_get2D, FLAG_Method_Hidden, CLASS_IArray, METHODN_get2D, 0, 18, NULL},
	{IArray_get3D, FLAG_Method_Hidden, CLASS_IArray, METHODN_get3D, 0, 19, NULL},
	{IArray_set2D, FLAG_Method_Hidden, CLASS_IArray, METHODN_set2D, 0, 20, NULL},
	{IArray_set3D, FLAG_Method_Hidden, CLASS_IArray, METHODN_set3D, 0, 21, NULL},
	{FArray_new__array2D, FLAG_Method_Private, CLASS_FArray, METHODN_new__array2D, 0, 16, NULL},
	{FArray_new__array3D, FLAG_Method_Private, CLASS_FArray, METHODN_new__array3D, 0, 17, NULL},
	{FArray_get2D, FLAG_Method_Hidden, CLASS_FArray, METHODN_get2D, 0, 18, NULL},
	{FArray_get3D, FLAG_Method_Hidden, CLASS_FArray, METHODN_get3D, 0, 19, NULL},
	{FArray_set2D, FLAG_Method_Hidden, CLASS_FArray, METHODN_set2D, 0, 20, NULL},
	{FArray_set3D, FLAG_Method_Hidden, CLASS_FArray, METHODN_set3D, 0, 21, NULL},
	{knh_fmethod_formatter, 0, CLASS_Token, METHODN__s, 0, 22, (void*)knh_Token__s},
	{knh_fmethod_formatter, 0, CLASS_Token, METHODN__k, 0, 22, (void*)knh_Token__k},
	{knh_fmethod_formatter, 0, CLASS_Token, METHODN__dump, 0, 22, (void*)knh_Token__dump},
	{knh_fmethod_formatter, 0, CLASS_Stmt, METHODN__s, 0, 22, (void*)knh_Stmt__s},
	{knh_fmethod_formatter, 0, CLASS_Stmt, METHODN__dump, 0, 22, (void*)knh_Stmt__dump},
	{Connection_new, 0, CLASS_Connection, METHODN_new, 0, 23, NULL},
	{Connection_query, 0, CLASS_Connection, METHODN_query, 0, 24, NULL},
	{Connection_exec, 0, CLASS_Connection, METHODN_exec, 0, 25, NULL},
	{Connection_close, 0, CLASS_Connection, METHODN_close, 0, 0, NULL},
	{ResultSet_getSize, 0, CLASS_ResultSet, METHODN_getSize, 0, 26, NULL},
	{ResultSet_next, 0, CLASS_ResultSet, METHODN_next, 0, 27, NULL},
	{ResultSet_getName, 0, CLASS_ResultSet, METHODN_getName, 0, 28, NULL},
	{ResultSet_getInt, 0, CLASS_ResultSet, METHODN_getInt, 0, 29, NULL},
	{ResultSet_getFloat, 0, CLASS_ResultSet, METHODN_getFloat, 0, 30, NULL},
	{ResultSet_getString, 0, CLASS_ResultSet, METHODN_getString, 0, 31, NULL},
	{ResultSet_get, 0, CLASS_ResultSet, METHODN_get, 0, 32, NULL},
	{knh_fmethod_formatter, 0, CLASS_ResultSet, METHODN__dump, 0, 33, (void*)knh_ResultSet__dump},
	{ResultSet_close, 0, CLASS_ResultSet, METHODN_close, 0, 0, NULL},
	{DictMap_opHas, 0, CLASS_DictMap, METHODN_opHas, 0, 34, NULL},
	{DictMap_get, 0, CLASS_DictMap, METHODN_get, 0, 35, NULL},
	{DictMap_set, 0, CLASS_DictMap, METHODN_set, 0, 36, NULL},
	{DictMap_remove, 0, CLASS_DictMap, METHODN_remove, 0, 37, NULL},
	{DictMap_clear, 0, CLASS_DictMap, METHODN_clear, 0, 0, NULL},
	{Object__empty, 0, CLASS_Object, METHODN__empty, 0, 38, NULL},
	{Object__s, 0, CLASS_Object, METHODN__s, 0, 38, NULL},
	{Boolean__s, 0, CLASS_Boolean, METHODN__s, 0, 38, NULL},
	{Boolean__s, 0, CLASS_Boolean, METHODN__k, 0, 38, NULL},
	{Boolean__s, 0, CLASS_Boolean, METHODN__data, 0, 38, NULL},
	{Boolean__s, 0, CLASS_Boolean, METHODN__dump, 0, 38, NULL},
	{Int__s, 0, CLASS_Int, METHODN__s, 0, 38, NULL},
	{String__s, 0, CLASS_String, METHODN__s, 0, 38, NULL},
	{Bytes__k, 0, CLASS_Bytes, METHODN__s, 0, 38, NULL},
	{Regex__s, 0, CLASS_Regex, METHODN__s, 0, 38, NULL},
	{Class__s, 0, CLASS_Class, METHODN__s, 0, 38, NULL},
	{Method__s, 0, CLASS_Method, METHODN__s, 0, 38, NULL},
	{Exception__s, 0, CLASS_Exception, METHODN__s, 0, 38, NULL},
	{Object__k, 0, CLASS_Object, METHODN__k, 0, 38, NULL},
	{Int__k, 0, CLASS_Int, METHODN__k, 0, 38, NULL},
	{Int__k, 0, CLASS_Int, METHODN__data, 0, 38, NULL},
	{Int__k, 0, CLASS_Int, METHODN__dump, 0, 38, NULL},
	{Float__k, 0, CLASS_Float, METHODN__k, 0, 38, NULL},
	{Float__k, 0, CLASS_Float, METHODN__data, 0, 38, NULL},
	{Float__k, 0, CLASS_Float, METHODN__dump, 0, 38, NULL},
	{String__k, 0, CLASS_String, METHODN__k, 0, 38, NULL},
	{Iterator__k, 0, CLASS_Iterator, METHODN__k, 0, 38, NULL},
	{Pair__k, 0, CLASS_Pair, METHODN__k, 0, 38, NULL},
	{Tuple__k, 0, CLASS_Tuple, METHODN__k, 0, 38, NULL},
	{Range__k, 0, CLASS_Range, METHODN__k, 0, 38, NULL},
	{Array__k, 0, CLASS_Array, METHODN__k, 0, 38, NULL},
	{IArray__k, 0, CLASS_IArray, METHODN__k, 0, 38, NULL},
	{FArray__k, 0, CLASS_FArray, METHODN__k, 0, 38, NULL},
	{DictMap__k, 0, CLASS_DictMap, METHODN__k, 0, 38, NULL},
	{DictSet__k, 0, CLASS_DictSet, METHODN__k, 0, 38, NULL},
	{DictIdx__k, 0, CLASS_DictIdx, METHODN__k, 0, 38, NULL},
	{HashMap__k, 0, CLASS_HashMap, METHODN__k, 0, 38, NULL},
	{Class__k, 0, CLASS_Class, METHODN__k, 0, 38, NULL},
	{Method__k, 0, CLASS_Method, METHODN__k, 0, 38, NULL},
	{Mapper__k, 0, CLASS_Mapper, METHODN__k, 0, 38, NULL},
	{Exception__k, 0, CLASS_Exception, METHODN__k, 0, 38, NULL},
	{InputStream__k, 0, CLASS_InputStream, METHODN__k, 0, 38, NULL},
	{OutputStream__k, 0, CLASS_OutputStream, METHODN__k, 0, 38, NULL},
	{Script__k, 0, CLASS_Script, METHODN__k, 0, 38, NULL},
	{Any__k, 0, CLASS_Any, METHODN__s, 0, 38, NULL},
	{Any__k, 0, CLASS_Any, METHODN__k, 0, 38, NULL},
	{Object__dump, 0, CLASS_Object, METHODN__dump, 0, 38, NULL},
	{Bytes__dump, 0, CLASS_Bytes, METHODN__dump, 0, 38, NULL},
	{Class__dump, 0, CLASS_Class, METHODN__dump, 0, 38, NULL},
	{ClassMap__dump, 0, CLASS_ClassMap, METHODN__dump, 0, 38, NULL},
	{Method__dump, 0, CLASS_Method, METHODN__dump, 0, 38, NULL},
	{Exception__dump, 0, CLASS_Exception, METHODN__dump, 0, 38, NULL},
	{Object__data, 0, CLASS_Object, METHODN__data, 0, 38, NULL},
	{String__data, 0, CLASS_String, METHODN__data, 0, 38, NULL},
	{Bytes__data, 0, CLASS_Bytes, METHODN__data, 0, 38, NULL},
	{Array__data, 0, CLASS_Array, METHODN__data, 0, 38, NULL},
	{IArray__data, 0, CLASS_IArray, METHODN__data, 0, 38, NULL},
	{FArray__data, 0, CLASS_FArray, METHODN__data, 0, 38, NULL},
	{DictMap__data, 0, CLASS_DictMap, METHODN__data, 0, 38, NULL},
	{Exception__data, 0, CLASS_Exception, METHODN__data, 0, 38, NULL},
	{Class__man, 0, CLASS_Class, METHODN__man, 0, 38, NULL},
	{Class__man, 0, CLASS_Object, METHODN__man, 0, 38, NULL},
	{Object__p, 0, CLASS_Object, METHODN__p, 0, 38, NULL},
	{Int__c, 0, CLASS_Int, METHODN__c, 0, 38, NULL},
	{Int__d, 0, CLASS_Int, METHODN__d, 0, 38, NULL},
	{Int__u, 0, CLASS_Int, METHODN__u, 0, 38, NULL},
	{Int__f, 0, CLASS_Int, METHODN__f, 0, 38, NULL},
	{Int__x, 0, CLASS_Int, METHODN__x, 0, 38, NULL},
	{Int__bits, 0, CLASS_Int, METHODN__bits, 0, 38, NULL},
	{Float__s, 0, CLASS_Float, METHODN__s, 0, 38, NULL},
	{Float__d, 0, CLASS_Float, METHODN__d, 0, 38, NULL},
	{Float__f, 0, CLASS_Float, METHODN__f, 0, 38, NULL},
	{Float__bits, 0, CLASS_Float, METHODN__bits, 0, 38, NULL},
	{HashMap_new, 0, CLASS_HashMap, METHODN_new, 0, 39, NULL},
	{HashMap_get, 0, CLASS_HashMap, METHODN_get, 0, 40, NULL},
	{HashMap_opHas, 0, CLASS_HashMap, METHODN_opHas, 0, 41, NULL},
	{HashMap_remove, 0, CLASS_HashMap, METHODN_remove, 0, 42, NULL},
	{HashMap_set, 0, CLASS_HashMap, METHODN_set, 0, 43, NULL},
	{String_opItr, 0, CLASS_String, METHODN_opItr, 0, 44, NULL},
	{Range_opItr, 0, CLASS_Range, METHODN_opItr, 0, 45, NULL},
	{Array_opItr, 0, CLASS_Array, METHODN_opItr, 0, 45, NULL},
	{IArray_opItr, 0, CLASS_IArray, METHODN_opItr, 0, 45, NULL},
	{FArray_opItr, 0, CLASS_FArray, METHODN_opItr, 0, 45, NULL},
	{DictMap_opItr, 0, CLASS_DictMap, METHODN_opItr, 0, 44, NULL},
	{DictMap_opItr__2, 0, CLASS_DictMap, METHODN_opItr__2, 0, 46, NULL},
	{HashMap_opItr, 0, CLASS_HashMap, METHODN_opItr, 0, 45, NULL},
	{HashMap_opItr__2, 0, CLASS_HashMap, METHODN_opItr__2, 0, 47, NULL},
	{Object_opItr, 0, CLASS_Object, METHODN_opItr, 0, 48, NULL},
	{Script_isStmt, FLAG_Method_Static|FLAG_Method_Hidden, CLASS_Script, METHODN_isStmt, 0, 49, NULL},
	{Script_eval, FLAG_Method_Static|FLAG_Method_Hidden, CLASS_Script, METHODN_eval, 0, 50, NULL},
	{Script_readLine, FLAG_Method_Static, CLASS_Script, METHODN_readLine, 0, 51, NULL},
	{Script_addHistory, FLAG_Method_Static, CLASS_Script, METHODN_addHistory, 0, 52, NULL},
	{System_setMethodTypingListener, FLAG_Method_Static, CLASS_System, METHODN_setMethodTypingListener, 0, 53, NULL},
	{System_setMethodCompilationListener, FLAG_Method_Static, CLASS_System, METHODN_setMethodCompilationListener, 0, 53, NULL},
	{Method_getProfTime, 0, CLASS_Method, METHODN_getProfTime, 0, 54, NULL},
	{Method_getProfCount, 0, CLASS_Method, METHODN_getProfCount, 0, 54, NULL},
	{Array_sortByClosure, 0, CLASS_Array, METHODN_sortByClosure, 0, 55, NULL},
	{System_getTimeMilliSecond, 0, CLASS_System, METHODN_getTickCount, 0, 54, NULL},
	{Object_new, FLAG_Method_Virtual, CLASS_Object, METHODN_new, 0, 56, NULL},
	{Object_new__dictmap, FLAG_Method_VarArgs|FLAG_Method_Hidden, CLASS_Object, METHODN_new__dictmap, 0, 57, NULL},
	{Bytes_new, 0, CLASS_Bytes, METHODN_new, 0, 58, NULL},
	{Bytes_new__array, 0, CLASS_Bytes, METHODN_new__array, 0, 59, NULL},
	{String_new, 0, CLASS_String, METHODN_new, 0, 60, NULL},
	{Regex_new, FLAG_Method_Const, CLASS_Regex, METHODN_new, 0, 61, NULL},
	{Pair_new__init, FLAG_Method_Const|FLAG_Method_Hidden, CLASS_Pair, METHODN_new__init, 0, 62, NULL},
	{Tuple_new__init, FLAG_Method_Const|FLAG_Method_VarArgs|FLAG_Method_Hidden, CLASS_Tuple, METHODN_new__init, 0, 63, NULL},
	{Range_new__init, FLAG_Method_Const|FLAG_Method_Hidden, CLASS_Range, METHODN_new__init, 0, 64, NULL},
	{Range_new, FLAG_Method_Const|FLAG_Method_Hidden, CLASS_Range, METHODN_new, 0, 64, NULL},
	{Array_new, 0, CLASS_Array, METHODN_new, 0, 39, NULL},
	{Array_new__array, FLAG_Method_Hidden, CLASS_Array, METHODN_new__array, 0, 65, NULL},
	{Array_new__init, FLAG_Method_VarArgs|FLAG_Method_Hidden, CLASS_Array, METHODN_new__init, 0, 66, NULL},
	{IArray_new, FLAG_Method_Hidden, CLASS_IArray, METHODN_new, 0, 39, NULL},
	{IArray_new__array, FLAG_Method_Hidden, CLASS_IArray, METHODN_new__array, 0, 65, NULL},
	{IArray_new__init, FLAG_Method_VarArgs|FLAG_Method_Hidden, CLASS_IArray, METHODN_new__init, 0, 67, NULL},
	{IArray_new__range, FLAG_Method_Const|FLAG_Method_Hidden, CLASS_IArray, METHODN_new__range, 0, 68, NULL},
	{FArray_new, FLAG_Method_Hidden, CLASS_FArray, METHODN_new, 0, 39, NULL},
	{FArray_new__array, FLAG_Method_Hidden, CLASS_FArray, METHODN_new__array, 0, 65, NULL},
	{FArray_new__init, FLAG_Method_VarArgs|FLAG_Method_Hidden, CLASS_FArray, METHODN_new__init, 0, 69, NULL},
	{DictMap_new, 0, CLASS_DictMap, METHODN_new, 0, 39, NULL},
	{DictMap_new__dictmap, FLAG_Method_VarArgs|FLAG_Method_Hidden, CLASS_DictMap, METHODN_new__dictmap, 0, 70, NULL},
	{Exception_new, 0, CLASS_Exception, METHODN_new, 0, 71, NULL},
	{Exception_new__init, FLAG_Method_Hidden, CLASS_Exception, METHODN_new__init, 0, 72, NULL},
	{Closure_new, 0, CLASS_Closure, METHODN_new, 0, 73, NULL},
	{Thread_new, 0, CLASS_Thread, METHODN_new, 0, 74, NULL},
	{System_setRandomSeed, FLAG_Method_Static, CLASS_System, METHODN_setRandomSeed, 0, 75, NULL},
	{Int_random, FLAG_Method_Static, CLASS_Int, METHODN_random, 0, 76, NULL},
	{Float_random, FLAG_Method_Static, CLASS_Float, METHODN_random, 0, 77, NULL},
	{Float_floatToIntBits, FLAG_Method_Static, CLASS_Float, METHODN_floatToIntBits, 0, 78, NULL},
	{Float_intToFloatBits, FLAG_Method_Static, CLASS_Float, METHODN_intToFloatBits, 0, 79, NULL},
	{Object_getClass, FLAG_Method_Const|FLAG_Method_NullBase, CLASS_Object, METHODN_getClass, 0, 80, NULL},
	{Object_hashCode, FLAG_Method_NullBase, CLASS_Object, METHODN_hashCode, 0, 26, NULL},
	{Object_isNull, FLAG_Method_Const|FLAG_Method_Hidden|FLAG_Method_NullBase, CLASS_Object, METHODN_isNull, 0, 27, NULL},
	{Object_isNotNull, FLAG_Method_Const|FLAG_Method_Hidden|FLAG_Method_NullBase, CLASS_Object, METHODN_isNotNull, 0, 27, NULL},
	{Object_getKey, FLAG_Method_Const|FLAG_Method_Virtual, CLASS_Object, METHODN_getKey, 0, 81, NULL},
	{Class_domain, FLAG_Method_Hidden, CLASS_Class, METHODN_domain, 0, 82, NULL},
	{Method_isAbstract, FLAG_Method_Const, CLASS_Method, METHODN_isAbstract, 0, 27, NULL},
	{Method_getName, FLAG_Method_Const, CLASS_Method, METHODN_getName, 0, 81, NULL},
	{Method_setTrace, FLAG_Method_Const, CLASS_Method, METHODN_setTrace, 0, 83, NULL},
	{Closure_invoke, FLAG_Method_VarArgs, CLASS_Closure, METHODN_invoke, 0, 84, NULL},
	{Thunk_eval, 0, CLASS_Thunk, METHODN_eval, 0, 11, NULL},
	{Thunk_value, 0, CLASS_Thunk, METHODN_value, 0, 11, NULL},
	{Object_opAddr, FLAG_Method_NullBase, CLASS_Object, METHODN_opAddr, 0, 26, NULL},
	{Object_opInstanceof, FLAG_Method_NullBase, CLASS_Object, METHODN_opInstanceof, 0, 85, NULL},
	{Object_opEq, FLAG_Method_Const|FLAG_Method_NullBase, CLASS_Object, METHODN_opEq, 0, 86, NULL},
	{Object_opNeq, FLAG_Method_Const|FLAG_Method_NullBase, CLASS_Object, METHODN_opNeq, 0, 86, NULL},
	{Object_opLt, FLAG_Method_Const|FLAG_Method_NullBase, CLASS_Object, METHODN_opLt, 0, 86, NULL},
	{Object_opLte, FLAG_Method_Const|FLAG_Method_NullBase, CLASS_Object, METHODN_opLte, 0, 86, NULL},
	{Object_opGt, FLAG_Method_Const|FLAG_Method_NullBase, CLASS_Object, METHODN_opGt, 0, 86, NULL},
	{Object_opGte, FLAG_Method_Const|FLAG_Method_NullBase, CLASS_Object, METHODN_opGte, 0, 86, NULL},
	{Int_opEq, FLAG_Method_Const, CLASS_Int, METHODN_opEq, 0, 87, NULL},
	{Int_opNeq, FLAG_Method_Const, CLASS_Int, METHODN_opNeq, 0, 87, NULL},
	{Int_opLt, FLAG_Method_Const, CLASS_Int, METHODN_opLt, 0, 87, NULL},
	{Int_opLte, FLAG_Method_Const, CLASS_Int, METHODN_opLte, 0, 87, NULL},
	{Int_opGt, FLAG_Method_Const, CLASS_Int, METHODN_opGt, 0, 87, NULL},
	{Int_opGte, FLAG_Method_Const, CLASS_Int, METHODN_opGte, 0, 87, NULL},
	{Float_opEq, FLAG_Method_Const|FLAG_Method_NullBase, CLASS_Float, METHODN_opEq, 0, 88, NULL},
	{Float_opNeq, FLAG_Method_Const|FLAG_Method_NullBase, CLASS_Float, METHODN_opNeq, 0, 88, NULL},
	{Float_opLt, FLAG_Method_Const, CLASS_Float, METHODN_opLt, 0, 88, NULL},
	{Float_opLte, FLAG_Method_Const, CLASS_Float, METHODN_opLte, 0, 88, NULL},
	{Float_opGt, FLAG_Method_Const, CLASS_Float, METHODN_opGt, 0, 88, NULL},
	{Float_opGte, FLAG_Method_Const, CLASS_Float, METHODN_opGte, 0, 88, NULL},
	{String_opHas, FLAG_Method_Const, CLASS_String, METHODN_opHas, 0, 89, NULL},
	{Pair_opHas, 0, CLASS_Pair, METHODN_opHas, 0, 90, NULL},
	{Pair_opHas, 0, CLASS_Pair, METHODN_opCase, 0, 90, NULL},
	{Range_opHas, 0, CLASS_Range, METHODN_opHas, 0, 90, NULL},
	{Range_opHas, 0, CLASS_Range, METHODN_opCase, 0, 90, NULL},
	{Array_opHas, 0, CLASS_Array, METHODN_opHas, 0, 90, NULL},
	{Array_opHas, 0, CLASS_Array, METHODN_opCase, 0, 90, NULL},
	{Tuple_opHas, 0, CLASS_Tuple, METHODN_opHas, 0, 90, NULL},
	{Tuple_opHas, 0, CLASS_Tuple, METHODN_opCase, 0, 90, NULL},
	{IArray_opHas, 0, CLASS_IArray, METHODN_opHas, 0, 90, NULL},
	{IArray_opHas, 0, CLASS_IArray, METHODN_opCase, 0, 90, NULL},
	{FArray_opHas, 0, CLASS_FArray, METHODN_opHas, 0, 90, NULL},
	{FArray_opHas, 0, CLASS_FArray, METHODN_opCase, 0, 90, NULL},
	{Object_opCase, FLAG_Method_Const|FLAG_Method_NullBase, CLASS_Object, METHODN_opCase, 0, 90, NULL},
	{Int_opCase, FLAG_Method_Const|FLAG_Method_NullBase, CLASS_Int, METHODN_opCase, 0, 90, NULL},
	{Float_opCase, FLAG_Method_Const|FLAG_Method_NullBase, CLASS_Float, METHODN_opCase, 0, 90, NULL},
	{Object_opIs, FLAG_Method_Const, CLASS_Object, METHODN_opIs, 0, 90, NULL},
	{Object_opAs, FLAG_Method_Const, CLASS_Object, METHODN_opAs, 0, 91, NULL},
	{Object_opInTo, FLAG_Method_Const, CLASS_Object, METHODN_opInTo, 0, 85, NULL},
	{Int_opAdd, FLAG_Method_Const, CLASS_Int, METHODN_opAdd, 0, 92, NULL},
	{Int_opNeg, FLAG_Method_Const, CLASS_Int, METHODN_opNeg, 0, 26, NULL},
	{Int_opSub, FLAG_Method_Const, CLASS_Int, METHODN_opSub, 0, 93, NULL},
	{Int_opMul, FLAG_Method_Const, CLASS_Int, METHODN_opMul, 0, 93, NULL},
	{Int_opDiv, FLAG_Method_Const, CLASS_Int, METHODN_opDiv, 0, 93, NULL},
	{Int_opMod, FLAG_Method_Const, CLASS_Int, METHODN_opMod, 0, 93, NULL},
	{Float_opAdd, FLAG_Method_Const, CLASS_Float, METHODN_opAdd, 0, 94, NULL},
	{Float_opNeg, FLAG_Method_Const, CLASS_Float, METHODN_opNeg, 0, 77, NULL},
	{Float_opSub, FLAG_Method_Const, CLASS_Float, METHODN_opSub, 0, 95, NULL},
	{Float_opMul, FLAG_Method_Const, CLASS_Float, METHODN_opMul, 0, 95, NULL},
	{Float_opDiv, FLAG_Method_Const, CLASS_Float, METHODN_opDiv, 0, 95, NULL},
	{String_opAdd, FLAG_Method_Const|FLAG_Method_NullBase, CLASS_String, METHODN_opAdd, 0, 96, NULL},
	{String_opSub, FLAG_Method_Const, CLASS_String, METHODN_opSub, 0, 97, NULL},
	{Any_opAdd, 0, CLASS_Any, METHODN_opAdd, 0, 98, NULL},
	{Any_opAdd, 0, CLASS_Any, METHODN_opSub, 0, 98, NULL},
	{Any_opAdd, 0, CLASS_Any, METHODN_opMul, 0, 98, NULL},
	{Any_opAdd, 0, CLASS_Any, METHODN_opDiv, 0, 98, NULL},
	{Any_opAdd, 0, CLASS_Any, METHODN_opMod, 0, 98, NULL},
	{Any_opNeg, 0, CLASS_Any, METHODN_opNeg, 0, 99, NULL},
	{Boolean_opNot, FLAG_Method_Const, CLASS_Boolean, METHODN_opNot, 0, 27, NULL},
	{Int_opLand, FLAG_Method_Const|FLAG_Method_VarArgs, CLASS_Int, METHODN_opLand, 0, 93, NULL},
	{Int_opLor, FLAG_Method_Const|FLAG_Method_VarArgs, CLASS_Int, METHODN_opLor, 0, 93, NULL},
	{Int_opXor, FLAG_Method_Const, CLASS_Int, METHODN_opXor, 0, 93, NULL},
	{Int_opLnot, FLAG_Method_Const, CLASS_Int, METHODN_opLnot, 0, 26, NULL},
	{Int_opLshift, FLAG_Method_Const, CLASS_Int, METHODN_opLshift, 0, 93, NULL},
	{Int_opRshift, FLAG_Method_Const, CLASS_Int, METHODN_opRshift, 0, 93, NULL},
	{Int_opNext, FLAG_Method_Const, CLASS_Int, METHODN_opNext, 0, 26, NULL},
	{Int_opPrev, FLAG_Method_Const, CLASS_Int, METHODN_opPrev, 0, 26, NULL},
	{Float_opNext, FLAG_Method_Const, CLASS_Float, METHODN_opNext, 0, 77, NULL},
	{Float_opPrev, FLAG_Method_Const, CLASS_Float, METHODN_opPrev, 0, 77, NULL},
	{Int_getSize, FLAG_Method_Const|FLAG_Method_NullBase, CLASS_Int, METHODN_getSize, 0, 26, NULL},
	{Float_getSize, FLAG_Method_Const|FLAG_Method_NullBase, CLASS_Float, METHODN_getSize, 0, 26, NULL},
	{Bytes_getSize, FLAG_Method_NullBase, CLASS_Bytes, METHODN_getSize, 0, 26, NULL},
	{String_getSize, FLAG_Method_Const|FLAG_Method_NullBase, CLASS_String, METHODN_getSize, 0, 26, NULL},
	{Pair_getSize, FLAG_Method_NullBase, CLASS_Pair, METHODN_getSize, 0, 26, NULL},
	{Tuple_getSize, FLAG_Method_NullBase, CLASS_Tuple, METHODN_getSize, 0, 26, NULL},
	{Range_getSize, FLAG_Method_NullBase, CLASS_Range, METHODN_getSize, 0, 26, NULL},
	{Array_getSize, FLAG_Method_NullBase, CLASS_Array, METHODN_getSize, 0, 26, NULL},
	{Array_getSize, FLAG_Method_NullBase, CLASS_IArray, METHODN_getSize, 0, 26, NULL},
	{Array_getSize, FLAG_Method_NullBase, CLASS_FArray, METHODN_getSize, 0, 26, NULL},
	{DictMap_getSize, FLAG_Method_NullBase, CLASS_DictMap, METHODN_getSize, 0, 26, NULL},
	{Bytes_get, 0, CLASS_Bytes, METHODN_get, 0, 93, NULL},
	{Bytes_set, 0, CLASS_Bytes, METHODN_set, 0, 100, NULL},
	{Bytes_setAll, 0, CLASS_Bytes, METHODN_setAll, 0, 101, NULL},
	{String_get, FLAG_Method_Const, CLASS_String, METHODN_get, 0, 28, NULL},
	{Array_get, 0, CLASS_Array, METHODN_get, 0, 102, NULL},
	{Tuple_get, 0, CLASS_Tuple, METHODN_get, 0, 103, NULL},
	{IArray_get, 0, CLASS_IArray, METHODN_get, 0, 93, NULL},
	{FArray_get, 0, CLASS_FArray, METHODN_get, 0, 79, NULL},
	{Array_set, 0, CLASS_Array, METHODN_set, 0, 104, NULL},
	{IArray_set, 0, CLASS_IArray, METHODN_set, 0, 105, NULL},
	{FArray_set, 0, CLASS_FArray, METHODN_set, 0, 106, NULL},
	{Array_setAll, 0, CLASS_Array, METHODN_setAll, 0, 107, NULL},
	{IArray_setAll, 0, CLASS_IArray, METHODN_setAll, 0, 108, NULL},
	{FArray_setAll, 0, CLASS_FArray, METHODN_setAll, 0, 109, NULL},
	{Bytes_opRangeUntil, 0, CLASS_Bytes, METHODN_opRangeUntil, 0, 110, NULL},
	{Bytes_opRangeTo, 0, CLASS_Bytes, METHODN_opRangeTo, 0, 110, NULL},
	{String_substring, FLAG_Method_Const, CLASS_String, METHODN_substring, 0, 111, NULL},
	{String_opRangeUntil, FLAG_Method_Const, CLASS_String, METHODN_opRangeUntil, 0, 112, NULL},
	{String_opRangeTo, FLAG_Method_Const, CLASS_String, METHODN_opRangeTo, 0, 112, NULL},
	{Array_opRangeUntil, 0, CLASS_Array, METHODN_opRangeUntil, 0, 113, NULL},
	{Array_opRangeTo, 0, CLASS_Array, METHODN_opRangeTo, 0, 113, NULL},
	{IArray_opRangeUntil, 0, CLASS_IArray, METHODN_opRangeUntil, 0, 113, NULL},
	{IArray_opRangeTo, 0, CLASS_IArray, METHODN_opRangeTo, 0, 113, NULL},
	{FArray_opRangeUntil, 0, CLASS_FArray, METHODN_opRangeUntil, 0, 113, NULL},
	{FArray_opRangeTo, 0, CLASS_FArray, METHODN_opRangeTo, 0, 113, NULL},
	{DictMap_opRangeTo, 0, CLASS_DictMap, METHODN_opRangeTo, 0, 114, NULL},
	{DictMap_opRangeUntil, 0, CLASS_DictMap, METHODN_opRangeUntil, 0, 114, NULL},
	{Pair_op0, 0, CLASS_Pair, METHODN_op0, 0, 11, NULL},
	{Pair_op1, 0, CLASS_Pair, METHODN_op1, 0, 115, NULL},
	{Tuple_op0, 0, CLASS_Tuple, METHODN_op0, 0, 99, NULL},
	{Tuple_op1, 0, CLASS_Tuple, METHODN_op1, 0, 99, NULL},
	{Tuple_opN, 0, CLASS_Tuple, METHODN_opN, 0, 103, NULL},
	{Array_op0, 0, CLASS_Array, METHODN_op0, 0, 11, NULL},
	{Array_op1, 0, CLASS_Array, METHODN_op1, 0, 11, NULL},
	{Array_opN, 0, CLASS_Array, METHODN_opN, 0, 102, NULL},
	{IArray_op0, 0, CLASS_IArray, METHODN_op0, 0, 26, NULL},
	{IArray_op1, 0, CLASS_IArray, METHODN_op1, 0, 26, NULL},
	{IArray_opN, 0, CLASS_IArray, METHODN_opN, 0, 93, NULL},
	{FArray_op0, 0, CLASS_FArray, METHODN_op0, 0, 77, NULL},
	{FArray_op1, 0, CLASS_FArray, METHODN_op1, 0, 77, NULL},
	{FArray_opN, 0, CLASS_FArray, METHODN_opN, 0, 79, NULL},
	{InputStream_new, 0, CLASS_InputStream, METHODN_new, 0, 116, NULL},
	{InputStream_getChar, 0, CLASS_InputStream, METHODN_getChar, 0, 26, NULL},
	{InputStream_read, 0, CLASS_InputStream, METHODN_read, 0, 117, NULL},
	{InputStream_getEncoding, 0, CLASS_InputStream, METHODN_getEncoding, 0, 81, NULL},
	{InputStream_setEncoding, 0, CLASS_InputStream, METHODN_setEncoding, 0, 118, NULL},
	{InputStream_isClosed, 0, CLASS_InputStream, METHODN_isClosed, 0, 27, NULL},
	{InputStream_close, 0, CLASS_InputStream, METHODN_close, 0, 0, NULL},
	{InputStream_readLine, 0, CLASS_InputStream, METHODN_readLine, 0, 119, NULL},
	{InputStream_readData, 0, CLASS_InputStream, METHODN_readData, 0, 120, NULL},
	{OutputStream_new, 0, CLASS_OutputStream, METHODN_new, 0, 121, NULL},
	{OutputStream_putChar, 0, CLASS_OutputStream, METHODN_putChar, 0, 122, NULL},
	{OutputStream_write, 0, CLASS_OutputStream, METHODN_write, 0, 2, NULL},
	{OutputStream_isClosed, 0, CLASS_OutputStream, METHODN_isClosed, 0, 27, NULL},
	{OutputStream_print, FLAG_Method_VarArgs, CLASS_OutputStream, METHODN_print, 0, 123, NULL},
	{OutputStream_print, FLAG_Method_VarArgs, CLASS_OutputStream, METHODN_opAppend, 0, 123, NULL},
	{OutputStream_println, FLAG_Method_VarArgs, CLASS_OutputStream, METHODN_println, 0, 123, NULL},
	{OutputStream_writeData, FLAG_Method_VarArgs, CLASS_OutputStream, METHODN_writeData, 0, 124, NULL},
	{OutputStream_flush, 0, CLASS_OutputStream, METHODN_flush, 0, 0, NULL},
	{OutputStream_clearBuffer, 0, CLASS_OutputStream, METHODN_clearBuffer, 0, 0, NULL},
	{OutputStream_close, 0, CLASS_OutputStream, METHODN_close, 0, 0, NULL},
	{OutputStream_getEncoding, 0, CLASS_OutputStream, METHODN_getEncoding, 0, 81, NULL},
	{OutputStream_setEncoding, 0, CLASS_OutputStream, METHODN_setEncoding, 0, 118, NULL},
	{Socket_new, 0, CLASS_Socket, METHODN_new, 0, 125, NULL},
	{Socket_getInputStream, 0, CLASS_Socket, METHODN_getInputStream, 0, 126, NULL},
	{Socket_getOutputStream, 0, CLASS_Socket, METHODN_getOutputStream, 0, 127, NULL},
	{Socket_close, 0, CLASS_Socket, METHODN_close, 0, 0, NULL},
	{Socket_isClosed, 0, CLASS_Socket, METHODN_isClosed, 0, 27, NULL},
	{String_getBytes, FLAG_Method_Const, CLASS_String, METHODN_getBytes, 0, 128, NULL},
	{String_equals, FLAG_Method_Const, CLASS_String, METHODN_equals, 0, 89, NULL},
	{String_equals__IgnoreCase, FLAG_Method_Const, CLASS_String, METHODN_equals__IgnoreCase, 0, 89, NULL},
	{String_startsWith, FLAG_Method_Const, CLASS_String, METHODN_startsWith, 0, 89, NULL},
	{String_startsWith__IgnoreCase, FLAG_Method_Const, CLASS_String, METHODN_startsWith__IgnoreCase, 0, 89, NULL},
	{String_endsWith, FLAG_Method_Const, CLASS_String, METHODN_endsWith, 0, 89, NULL},
	{String_endsWith__IgnoreCase, FLAG_Method_Const, CLASS_String, METHODN_endsWith__IgnoreCase, 0, 89, NULL},
	{String_indexOf, FLAG_Method_Const, CLASS_String, METHODN_indexOf, 0, 129, NULL},
	{String_indexOf__IgnoreCase, FLAG_Method_Const, CLASS_String, METHODN_indexOf__IgnoreCase, 0, 129, NULL},
	{String_lastIndexOf, FLAG_Method_Const, CLASS_String, METHODN_lastIndexOf, 0, 129, NULL},
	{String_lastIndexOf__IgnoreCase, FLAG_Method_Const, CLASS_String, METHODN_lastIndexOf__IgnoreCase, 0, 129, NULL},
	{String_concat, FLAG_Method_Const|FLAG_Method_VarArgs|FLAG_Method_NullBase, CLASS_String, METHODN_concat, 0, 130, NULL},
	{String_times, FLAG_Method_Const, CLASS_String, METHODN_times, 0, 28, NULL},
	{String_twofold, FLAG_Method_Const, CLASS_String, METHODN_twofold, 0, 131, NULL},
	{String_format, FLAG_Method_VarArgs, CLASS_String, METHODN_format, 0, 130, NULL},
	{String_replace, FLAG_Method_Const, CLASS_String, METHODN_replace, 0, 132, NULL},
	{String_getUCS4, FLAG_Method_Const, CLASS_String, METHODN_getUCS4, 0, 76, NULL},
	{String_toLower, FLAG_Method_Const, CLASS_String, METHODN_toLower, 0, 81, NULL},
	{String_toUpper, FLAG_Method_Const, CLASS_String, METHODN_toUpper, 0, 81, NULL},
	{String_trim, FLAG_Method_Const, CLASS_String, METHODN_trim, 0, 81, NULL},
	{String_split, FLAG_Method_Const, CLASS_String, METHODN_split, 0, 133, NULL},
	{String_opMatch, 0, CLASS_String, METHODN_opMatch, 0, 134, NULL},
	{Regex_opMatch, 0, CLASS_Regex, METHODN_opMatch, 0, 89, NULL},
	{String_match, FLAG_Method_Const, CLASS_String, METHODN_match, 0, 135, NULL},
	{System_getIn, FLAG_Method_Static, CLASS_System, METHODN_getIn, 0, 126, NULL},
	{System_getOut, FLAG_Method_Static, CLASS_System, METHODN_getOut, 0, 127, NULL},
	{System_getErr, FLAG_Method_Static, CLASS_System, METHODN_getErr, 0, 127, NULL},
	{System_getProperty, FLAG_Method_Static, CLASS_System, METHODN_getProperty, 0, 136, NULL},
	{System_setProperty, FLAG_Method_Static, CLASS_System, METHODN_setProperty, 0, 137, NULL},
	{System_gc, FLAG_Method_Static, CLASS_System, METHODN_gc, 0, 0, NULL},
	{System_test, FLAG_Method_Hidden, CLASS_System, METHODN_test, 0, 138, NULL},
	{Script_system, 0, CLASS_Script, METHODN_system, 0, 139, NULL},
	{System_hasLib, FLAG_Method_Static, CLASS_System, METHODN_hasLib, 0, 140, NULL},
	{System_exit, FLAG_Method_Static, CLASS_System, METHODN_exit, 0, 141, NULL},
	{System_listDir, 0, CLASS_System, METHODN_listDir, 0, 142, NULL},
	{System_hasDir, 0, CLASS_System, METHODN_hasDir, 0, 143, NULL},
	{System_hasFile, 0, CLASS_System, METHODN_hasFile, 0, 143, NULL},
	{System_mkdir, 0, CLASS_System, METHODN_mkdir, 0, 143, NULL},
	{System_unlink, 0, CLASS_System, METHODN_unlink, 0, 143, NULL},
	{System_rename, 0, CLASS_System, METHODN_rename, 0, 144, NULL},
	{Context_getProperty, FLAG_Method_Static, CLASS_Context, METHODN_getProperty, 0, 145, NULL},
	{Context_setProperty, FLAG_Method_Static, CLASS_Context, METHODN_setProperty, 0, 137, NULL},
	{Context_setEncoding, FLAG_Method_Static, CLASS_Context, METHODN_setEncoding, 0, 118, NULL},
	{Context_listProperties, FLAG_Method_Hidden, CLASS_Context, METHODN_listProperties, 0, 146, NULL},
	{Context_setIn, FLAG_Method_Static, CLASS_Context, METHODN_setIn, 0, 147, NULL},
	{Context_setOut, FLAG_Method_Static, CLASS_Context, METHODN_setOut, 0, 148, NULL},
	{Context_setErr, FLAG_Method_Static, CLASS_Context, METHODN_setErr, 0, 148, NULL},
	{knh_fmethod_formatter, 0, CLASS_Context, METHODN__dump, 0, 22, (void*)knh_Context__dump},
	{Exception_opInstanceof, 0, CLASS_Exception, METHODN_opInstanceof, 0, 149, NULL},
	{NameSpace_setConst, FLAG_Method_Const|FLAG_Method_Hidden, CLASS_NameSpace, METHODN_setConst, 0, 150, NULL},
	{Object_isRelease, 0, CLASS_Object, METHODN_isRelease, 0, 27, NULL},
	{Object_isDebug, 0, CLASS_Object, METHODN_isDebug, 0, 27, NULL},
	{Object_isImmutable, 0, CLASS_Object, METHODN_isImmutable, 0, 27, NULL},
	{Object_isUndefined, 0, CLASS_Object, METHODN_isUndefined, 0, 27, NULL},
	{Object_isModified, 0, CLASS_Object, METHODN_isModified, 0, 27, NULL},
	{Object_setModified, 0, CLASS_Object, METHODN_setModified, 0, 151, NULL},
	{Object_isShared, 0, CLASS_Object, METHODN_isShared, 0, 27, NULL},
	{Object_setShared, 0, CLASS_Object, METHODN_setShared, 0, 151, NULL},
	{String_isAscii, 0, CLASS_String, METHODN_isAscii, 0, 27, NULL},
	{DictMap_isIgnoreCase, 0, CLASS_DictMap, METHODN_isIgnoreCase, 0, 27, NULL},
	{DictSet_isIgnoreCase, 0, CLASS_DictSet, METHODN_isIgnoreCase, 0, 27, NULL},
	{Class_isRelease, 0, CLASS_Class, METHODN_isRelease, 0, 27, NULL},
	{Class_isDebug, 0, CLASS_Class, METHODN_isDebug, 0, 27, NULL},
	{Class_isImmutable, 0, CLASS_Class, METHODN_isImmutable, 0, 27, NULL},
	{Class_isMetaClass, 0, CLASS_Class, METHODN_isMetaClass, 0, 27, NULL},
	{Class_isPrivate, 0, CLASS_Class, METHODN_isPrivate, 0, 27, NULL},
	{Class_isPublic, 0, CLASS_Class, METHODN_isPublic, 0, 27, NULL},
	{Class_isFinal, 0, CLASS_Class, METHODN_isFinal, 0, 27, NULL},
	{Class_isSingleton, 0, CLASS_Class, METHODN_isSingleton, 0, 27, NULL},
	{Class_isUnique, 0, CLASS_Class, METHODN_isUnique, 0, 27, NULL},
	{Class_isInterface, 0, CLASS_Class, METHODN_isInterface, 0, 27, NULL},
	{Method_isPrivate, 0, CLASS_Method, METHODN_isPrivate, 0, 27, NULL},
	{Method_isPublic, 0, CLASS_Method, METHODN_isPublic, 0, 27, NULL},
	{Method_isVirtual, 0, CLASS_Method, METHODN_isVirtual, 0, 27, NULL},
	{Method_isFinal, 0, CLASS_Method, METHODN_isFinal, 0, 27, NULL},
	{Method_isConst, 0, CLASS_Method, METHODN_isConst, 0, 27, NULL},
	{Method_isStatic, 0, CLASS_Method, METHODN_isStatic, 0, 27, NULL},
	{Method_isVarArgs, 0, CLASS_Method, METHODN_isVarArgs, 0, 27, NULL},
	{Mapper_isInterface, 0, CLASS_Mapper, METHODN_isInterface, 0, 27, NULL},
	{Mapper_isICast, 0, CLASS_Mapper, METHODN_isICast, 0, 27, NULL},
	{Mapper_isSignificant, 0, CLASS_Mapper, METHODN_isSignificant, 0, 27, NULL},
	{Mapper_isFinal, 0, CLASS_Mapper, METHODN_isFinal, 0, 27, NULL},
	{Mapper_isSynonym, 0, CLASS_Mapper, METHODN_isSynonym, 0, 27, NULL},
	{Mapper_isLossLess, 0, CLASS_Mapper, METHODN_isLossLess, 0, 27, NULL},
	{Mapper_isTotal, 0, CLASS_Mapper, METHODN_isTotal, 0, 27, NULL},
	{Mapper_isPartial, 0, CLASS_Mapper, METHODN_isPartial, 0, 27, NULL},
	{Mapper_isConst, 0, CLASS_Mapper, METHODN_isConst, 0, 27, NULL},
	{Mapper_isTemporal, 0, CLASS_Mapper, METHODN_isTemporal, 0, 27, NULL},
	{Mapper_isLocal, 0, CLASS_Mapper, METHODN_isLocal, 0, 27, NULL},
	{Mapper_isDerived, 0, CLASS_Mapper, METHODN_isDerived, 0, 27, NULL},
	{OutputStream_isAutoFlush, 0, CLASS_OutputStream, METHODN_isAutoFlush, 0, 27, NULL},
	{OutputStream_setAutoFlush, 0, CLASS_OutputStream, METHODN_setAutoFlush, 0, 151, NULL},
	{OutputStream_isStoringBuffer, 0, CLASS_OutputStream, METHODN_isStoringBuffer, 0, 27, NULL},
	{OutputStream_setStoringBuffer, 0, CLASS_OutputStream, METHODN_setStoringBuffer, 0, 151, NULL},
	{Exception_isLogging, 0, CLASS_Exception, METHODN_isLogging, 0, 27, NULL},
	{Context_isStrict, 0, CLASS_Context, METHODN_isStrict, 0, 27, NULL},
	{Context_setStrict, 0, CLASS_Context, METHODN_setStrict, 0, 151, NULL},
	{Context_isDebug, 0, CLASS_Context, METHODN_isDebug, 0, 27, NULL},
	{Context_setDebug, 0, CLASS_Context, METHODN_setDebug, 0, 151, NULL},
	{Context_isVerbose, 0, CLASS_Context, METHODN_isVerbose, 0, 27, NULL},
	{Context_setVerbose, 0, CLASS_Context, METHODN_setVerbose, 0, 151, NULL},
	{Context_isInteractive, 0, CLASS_Context, METHODN_isInteractive, 0, 27, NULL},
	{NULL}
};

static knh_MapperData0_t MapperData0[] = {
	{String_Iterator, FLAG_Mapper_Total, CLASS_String, CLASS_Iterator},
	{String_Iterator, FLAG_Mapper_Total, CLASS_String, CLASS_String_Itr},
	{Array_Iterator, FLAG_Mapper_Total, CLASS_Array, CLASS_Iterator},
	{knh_IArray_Iterator, FLAG_Mapper_Total, CLASS_IArray, CLASS_Iterator},
	{knh_FArray_Iterator, FLAG_Mapper_Total, CLASS_FArray, CLASS_Iterator},
	{knh_DictMap_String__, FLAG_Mapper_Total, CLASS_DictMap, CLASS_Iterator},
	{knh_DictMap_String__, FLAG_Mapper_Total, CLASS_DictMap, CLASS_String_Itr},
	{knh_HashMap_Iterator, FLAG_Mapper_Total|FLAG_Mapper_Final, CLASS_HashMap, CLASS_Iterator},
	{Float_Int, FLAG_Mapper_Const|FLAG_Mapper_Total|FLAG_Mapper_Final, CLASS_Float, CLASS_Int},
	{String_Int, FLAG_Mapper_Const|FLAG_Mapper_Final, CLASS_String, CLASS_Int},
	{Int_Float, FLAG_Mapper_Const|FLAG_Mapper_Total|FLAG_Mapper_Final, CLASS_Int, CLASS_Float},
	{String_Float, FLAG_Mapper_Const|FLAG_Mapper_Final, CLASS_String, CLASS_Float},
	{Object_String, FLAG_Mapper_Total, CLASS_Object, CLASS_String},
	{Boolean_String, FLAG_Mapper_Const|FLAG_Mapper_Total|FLAG_Mapper_Final, CLASS_Boolean, CLASS_String},
	{Int_String, FLAG_Mapper_Const|FLAG_Mapper_Total|FLAG_Mapper_Final, CLASS_Int, CLASS_String},
	{Float_String, FLAG_Mapper_Const|FLAG_Mapper_Total|FLAG_Mapper_Final, CLASS_Float, CLASS_String},
	{Bytes_String, FLAG_Mapper_Const|FLAG_Mapper_Total|FLAG_Mapper_Final, CLASS_Bytes, CLASS_String},
	{String_Bytes, FLAG_Mapper_Const|FLAG_Mapper_Total|FLAG_Mapper_Final, CLASS_String, CLASS_Bytes},
	{Object_Iterator, FLAG_Mapper_Const|FLAG_Mapper_Total, CLASS_Object, CLASS_Iterator},
	{Iterator_Array, FLAG_Mapper_Total, CLASS_Iterator, CLASS_Array},
	{knh_InputStream_String__, FLAG_Mapper_Total|FLAG_Mapper_Final, CLASS_InputStream, CLASS_String_Itr},
	{NULL}
};
