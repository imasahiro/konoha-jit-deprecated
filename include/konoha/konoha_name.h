// THIS FILE WAS AUTOMATICALLY GENERATED

/* ======================================================================== */
/* MACROS */
#define K_REVISION                      1690
#define K_BUILDID                       952

/* ======================================================================== */
/* STRUCT */

/* ------------------------------------------------------------------------ */
/* Object */
#define CLASS_Object            ((knh_class_t)0)
#define IS_Object(o)            ((o)->h.cid == CLASS_Object)
#define TYPE_Object             CLASS_Object
#define CFLAG_Object            ((knh_flag_t)0)
#define FLAG_Object             FLAG_oflag(CFLAG_Object)

/* ------------------------------------------------------------------------ */
/* Boolean */
#define CLASS_Boolean           ((knh_class_t)1)
#define IS_Boolean(o)           ((o)->h.cid == CLASS_Boolean)
#define IS_bBoolean(o)          ((o)->h.bcid == CLASS_Boolean)
#define TYPE_Boolean            CLASS_Boolean
#define CFLAG_Boolean           ((knh_flag_t)FLAG_Class_Immutable)
#define FLAG_Boolean            FLAG_oflag(CFLAG_Boolean)

/* ------------------------------------------------------------------------ */
/* Number */
#define CLASS_Number            ((knh_class_t)2)
#define IS_Number(o)            ((o)->h.cid == CLASS_Number)
#define IS_bNumber(o)           ((o)->h.bcid == CLASS_Number)
#define TYPE_Number             CLASS_Number
#define CFLAG_Number            ((knh_flag_t)0)
#define FLAG_Number             FLAG_oflag(CFLAG_Number)

/* ------------------------------------------------------------------------ */
/* Int */
#define CLASS_Int               ((knh_class_t)3)
#define IS_Int(o)               ((o)->h.cid == CLASS_Int)
#define IS_bInt(o)              ((o)->h.bcid == CLASS_Int)
#define TYPE_Int                CLASS_Int
#define CFLAG_Int               ((knh_flag_t)FLAG_Class_Immutable)
#define FLAG_Int                FLAG_oflag(CFLAG_Int)

/* ------------------------------------------------------------------------ */
/* Float */
#define CLASS_Float             ((knh_class_t)4)
#define IS_Float(o)             ((o)->h.cid == CLASS_Float)
#define IS_bFloat(o)            ((o)->h.bcid == CLASS_Float)
#define TYPE_Float              CLASS_Float
#define CFLAG_Float             ((knh_flag_t)FLAG_Class_Immutable)
#define FLAG_Float              FLAG_oflag(CFLAG_Float)

/* ------------------------------------------------------------------------ */
/* String */
#define CLASS_String            ((knh_class_t)5)
#define IS_String(o)            ((o)->h.cid == CLASS_String)
#define IS_bString(o)           ((o)->h.bcid == CLASS_String)
#define TYPE_String             CLASS_String
#define CFLAG_String            ((knh_flag_t)FLAG_Class_Immutable)
#define FLAG_String             FLAG_oflag(CFLAG_String)

/* ------------------------------------------------------------------------ */
/* Bytes */
#define CLASS_Bytes             ((knh_class_t)6)
#define IS_Bytes(o)             ((o)->h.cid == CLASS_Bytes)
#define IS_bBytes(o)            ((o)->h.bcid == CLASS_Bytes)
#define TYPE_Bytes              CLASS_Bytes
#define CFLAG_Bytes             ((knh_flag_t)0)
#define FLAG_Bytes              FLAG_oflag(CFLAG_Bytes)

/* ------------------------------------------------------------------------ */
/* Any */
#define CLASS_Any               ((knh_class_t)7)
#define IS_Any(o)               ((o)->h.cid == CLASS_Any)
#define IS_bAny(o)              ((o)->h.bcid == CLASS_Any)
#define TYPE_Any                CLASS_Any
#define CFLAG_Any               ((knh_flag_t)0)
#define FLAG_Any                FLAG_oflag(CFLAG_Any)

/* ------------------------------------------------------------------------ */
/* Iterator */
#define CLASS_Iterator          ((knh_class_t)8)
#define IS_Iterator(o)          ((o)->h.cid == CLASS_Iterator)
#define IS_bIterator(o)         ((o)->h.bcid == CLASS_Iterator)
#define TYPE_Iterator           CLASS_Iterator
#define CFLAG_Iterator          ((knh_flag_t)FLAG_Class_Immutable)
#define FLAG_Iterator           FLAG_oflag(CFLAG_Iterator)

/* ------------------------------------------------------------------------ */
/* Tuple */
#define CLASS_Tuple             ((knh_class_t)9)
#define IS_Tuple(o)             ((o)->h.cid == CLASS_Tuple)
#define IS_bTuple(o)            ((o)->h.bcid == CLASS_Tuple)
#define TYPE_Tuple              CLASS_Tuple
#define CFLAG_Tuple             ((knh_flag_t)FLAG_Class_Immutable)
#define FLAG_Tuple              FLAG_oflag(CFLAG_Tuple)

/* ------------------------------------------------------------------------ */
/* Range */
#define CLASS_Range             ((knh_class_t)10)
#define IS_Range(o)             ((o)->h.cid == CLASS_Range)
#define IS_bRange(o)            ((o)->h.bcid == CLASS_Range)
#define TYPE_Range              CLASS_Range
#define CFLAG_Range             ((knh_flag_t)FLAG_Class_Immutable)
#define FLAG_Range              FLAG_oflag(CFLAG_Range)

/* ------------------------------------------------------------------------ */
/* Array */
#define CLASS_Array             ((knh_class_t)11)
#define IS_Array(o)             ((o)->h.cid == CLASS_Array)
#define IS_bArray(o)            ((o)->h.bcid == CLASS_Array)
#define TYPE_Array              CLASS_Array
#define CFLAG_Array             ((knh_flag_t)0)
#define FLAG_Array              FLAG_oflag(CFLAG_Array)

/* ------------------------------------------------------------------------ */
/* Map */
#define CLASS_Map               ((knh_class_t)12)
#define IS_Map(o)               ((o)->h.cid == CLASS_Map)
#define IS_bMap(o)              ((o)->h.bcid == CLASS_Map)
#define TYPE_Map                CLASS_Map
#define CFLAG_Map               ((knh_flag_t)0)
#define FLAG_Map                FLAG_oflag(CFLAG_Map)

/* ------------------------------------------------------------------------ */
/* Class */
#define CLASS_Class             ((knh_class_t)13)
#define IS_Class(o)             ((o)->h.cid == CLASS_Class)
#define IS_bClass(o)            ((o)->h.bcid == CLASS_Class)
#define TYPE_Class              CLASS_Class
#define CFLAG_Class             ((knh_flag_t)FLAG_Class_Immutable)
#define FLAG_Class              FLAG_oflag(CFLAG_Class)

/* ------------------------------------------------------------------------ */
/* ParamArray */
#define CLASS_ParamArray        ((knh_class_t)14)
#define IS_ParamArray(o)        ((o)->h.cid == CLASS_ParamArray)
#define IS_bParamArray(o)       ((o)->h.bcid == CLASS_ParamArray)
#define TYPE_ParamArray         CLASS_ParamArray
#define CFLAG_ParamArray        ((knh_flag_t)FLAG_Class_Private)
#define FLAG_ParamArray         FLAG_oflag(CFLAG_ParamArray)

/* ------------------------------------------------------------------------ */
/* Method */
#define CLASS_Method            ((knh_class_t)15)
#define IS_Method(o)            ((o)->h.cid == CLASS_Method)
#define IS_bMethod(o)           ((o)->h.bcid == CLASS_Method)
#define TYPE_Method             CLASS_Method
#define CFLAG_Method            ((knh_flag_t)0)
#define FLAG_Method             FLAG_oflag(CFLAG_Method)

/* ------------------------------------------------------------------------ */
/* Translator */
#define CLASS_Translator        ((knh_class_t)16)
#define IS_Translator(o)        ((o)->h.cid == CLASS_Translator)
#define IS_bTranslator(o)       ((o)->h.bcid == CLASS_Translator)
#define TYPE_Translator         CLASS_Translator
#define CFLAG_Translator        ((knh_flag_t)0)
#define FLAG_Translator         FLAG_oflag(CFLAG_Translator)

/* ------------------------------------------------------------------------ */
/* Func */
#define CLASS_Func              ((knh_class_t)17)
#define IS_Func(o)              ((o)->h.cid == CLASS_Func)
#define IS_bFunc(o)             ((o)->h.bcid == CLASS_Func)
#define TYPE_Func               CLASS_Func
#define CFLAG_Func              ((knh_flag_t)0)
#define FLAG_Func               FLAG_oflag(CFLAG_Func)

/* ------------------------------------------------------------------------ */
/* Thunk */
#define CLASS_Thunk             ((knh_class_t)18)
#define IS_Thunk(o)             ((o)->h.cid == CLASS_Thunk)
#define IS_bThunk(o)            ((o)->h.bcid == CLASS_Thunk)
#define TYPE_Thunk              CLASS_Thunk
#define CFLAG_Thunk             ((knh_flag_t)0)
#define FLAG_Thunk              FLAG_oflag(CFLAG_Thunk)

/* ------------------------------------------------------------------------ */
/* Exception */
#define CLASS_Exception         ((knh_class_t)19)
#define IS_Exception(o)         ((o)->h.cid == CLASS_Exception)
#define IS_bException(o)        ((o)->h.bcid == CLASS_Exception)
#define TYPE_Exception          CLASS_Exception
#define CFLAG_Exception         ((knh_flag_t)0)
#define FLAG_Exception          FLAG_oflag(CFLAG_Exception)

/* ------------------------------------------------------------------------ */
/* ExceptionHandler */
#define CLASS_ExceptionHandler  ((knh_class_t)20)
#define IS_ExceptionHandler(o)  ((o)->h.cid == CLASS_ExceptionHandler)
#define IS_bExceptionHandler(o) ((o)->h.bcid == CLASS_ExceptionHandler)
#define TYPE_ExceptionHandler   CLASS_ExceptionHandler
#define CFLAG_ExceptionHandler  ((knh_flag_t)FLAG_Class_Private)
#define FLAG_ExceptionHandler   FLAG_oflag(CFLAG_ExceptionHandler)

/* ------------------------------------------------------------------------ */
/* Regex */
#define CLASS_Regex             ((knh_class_t)21)
#define IS_Regex(o)             ((o)->h.cid == CLASS_Regex)
#define IS_bRegex(o)            ((o)->h.bcid == CLASS_Regex)
#define TYPE_Regex              CLASS_Regex
#define CFLAG_Regex             ((knh_flag_t)FLAG_Class_Immutable)
#define FLAG_Regex              FLAG_oflag(CFLAG_Regex)

/* ------------------------------------------------------------------------ */
/* Converter */
#define CLASS_Converter         ((knh_class_t)22)
#define IS_Converter(o)         ((o)->h.cid == CLASS_Converter)
#define IS_bConverter(o)        ((o)->h.bcid == CLASS_Converter)
#define TYPE_Converter          CLASS_Converter
#define CFLAG_Converter         ((knh_flag_t)0)
#define FLAG_Converter          FLAG_oflag(CFLAG_Converter)

/* ------------------------------------------------------------------------ */
/* StringEncoder */
#define CLASS_StringEncoder     ((knh_class_t)23)
#define IS_StringEncoder(o)     ((o)->h.cid == CLASS_StringEncoder)
#define IS_bStringEncoder(o)    ((o)->h.bcid == CLASS_StringEncoder)
#define TYPE_StringEncoder      CLASS_StringEncoder
#define CFLAG_StringEncoder     ((knh_flag_t)0)
#define FLAG_StringEncoder      FLAG_oflag(CFLAG_StringEncoder)

/* ------------------------------------------------------------------------ */
/* StringDecoder */
#define CLASS_StringDecoder     ((knh_class_t)24)
#define IS_StringDecoder(o)     ((o)->h.cid == CLASS_StringDecoder)
#define IS_bStringDecoder(o)    ((o)->h.bcid == CLASS_StringDecoder)
#define TYPE_StringDecoder      CLASS_StringDecoder
#define CFLAG_StringDecoder     ((knh_flag_t)0)
#define FLAG_StringDecoder      FLAG_oflag(CFLAG_StringDecoder)

/* ------------------------------------------------------------------------ */
/* StringConverter */
#define CLASS_StringConverter   ((knh_class_t)25)
#define IS_StringConverter(o)   ((o)->h.cid == CLASS_StringConverter)
#define IS_bStringConverter(o)  ((o)->h.bcid == CLASS_StringConverter)
#define TYPE_StringConverter    CLASS_StringConverter
#define CFLAG_StringConverter   ((knh_flag_t)0)
#define FLAG_StringConverter    FLAG_oflag(CFLAG_StringConverter)

/* ------------------------------------------------------------------------ */
/* Semantics */
#define CLASS_Semantics         ((knh_class_t)26)
#define IS_Semantics(o)         ((o)->h.cid == CLASS_Semantics)
#define IS_bSemantics(o)        ((o)->h.bcid == CLASS_Semantics)
#define TYPE_Semantics          CLASS_Semantics
#define CFLAG_Semantics         ((knh_flag_t)FLAG_Class_Private)
#define FLAG_Semantics          FLAG_oflag(CFLAG_Semantics)

/* ------------------------------------------------------------------------ */
/* InputStream */
#define CLASS_InputStream       ((knh_class_t)27)
#define IS_InputStream(o)       ((o)->h.cid == CLASS_InputStream)
#define IS_bInputStream(o)      ((o)->h.bcid == CLASS_InputStream)
#define TYPE_InputStream        CLASS_InputStream
#define CFLAG_InputStream       ((knh_flag_t)0)
#define FLAG_InputStream        FLAG_oflag(CFLAG_InputStream)

/* ------------------------------------------------------------------------ */
/* OutputStream */
#define CLASS_OutputStream      ((knh_class_t)28)
#define IS_OutputStream(o)      ((o)->h.cid == CLASS_OutputStream)
#define IS_bOutputStream(o)     ((o)->h.bcid == CLASS_OutputStream)
#define TYPE_OutputStream       CLASS_OutputStream
#define CFLAG_OutputStream      ((knh_flag_t)0)
#define FLAG_OutputStream       FLAG_oflag(CFLAG_OutputStream)

/* ------------------------------------------------------------------------ */
/* Channel */
#define CLASS_Channel           ((knh_class_t)29)
#define IS_Channel(o)           ((o)->h.cid == CLASS_Channel)
#define IS_bChannel(o)          ((o)->h.bcid == CLASS_Channel)
#define TYPE_Channel            CLASS_Channel
#define CFLAG_Channel           ((knh_flag_t)0)
#define FLAG_Channel            FLAG_oflag(CFLAG_Channel)

/* ------------------------------------------------------------------------ */
/* Connection */
#define CLASS_Connection        ((knh_class_t)30)
#define IS_Connection(o)        ((o)->h.cid == CLASS_Connection)
#define IS_bConnection(o)       ((o)->h.bcid == CLASS_Connection)
#define TYPE_Connection         CLASS_Connection
#define CFLAG_Connection        ((knh_flag_t)0)
#define FLAG_Connection         FLAG_oflag(CFLAG_Connection)

/* ------------------------------------------------------------------------ */
/* ResultSet */
#define CLASS_ResultSet         ((knh_class_t)31)
#define IS_ResultSet(o)         ((o)->h.cid == CLASS_ResultSet)
#define IS_bResultSet(o)        ((o)->h.bcid == CLASS_ResultSet)
#define TYPE_ResultSet          CLASS_ResultSet
#define CFLAG_ResultSet         ((knh_flag_t)0)
#define FLAG_ResultSet          FLAG_oflag(CFLAG_ResultSet)

/* ------------------------------------------------------------------------ */
/* Script */
#define CLASS_Script            ((knh_class_t)32)
#define IS_Script(o)            ((o)->h.cid == CLASS_Script)
#define IS_bScript(o)           ((o)->h.bcid == CLASS_Script)
#define TYPE_Script             CLASS_Script
#define CFLAG_Script            ((knh_flag_t)FLAG_Class_Singleton|FLAG_Class_Private)
#define FLAG_Script             FLAG_oflag(CFLAG_Script)

/* ------------------------------------------------------------------------ */
/* NameSpace */
#define CLASS_NameSpace         ((knh_class_t)33)
#define IS_NameSpace(o)         ((o)->h.cid == CLASS_NameSpace)
#define IS_bNameSpace(o)        ((o)->h.bcid == CLASS_NameSpace)
#define TYPE_NameSpace          CLASS_NameSpace
#define CFLAG_NameSpace         ((knh_flag_t)0)
#define FLAG_NameSpace          FLAG_oflag(CFLAG_NameSpace)

/* ------------------------------------------------------------------------ */
/* Package */
#define CLASS_Package           ((knh_class_t)34)
#define IS_Package(o)           ((o)->h.cid == CLASS_Package)
#define IS_bPackage(o)          ((o)->h.bcid == CLASS_Package)
#define TYPE_Package            CLASS_Package
#define CFLAG_Package           ((knh_flag_t)0)
#define FLAG_Package            FLAG_oflag(CFLAG_Package)

/* ------------------------------------------------------------------------ */
/* System */
#define CLASS_System            ((knh_class_t)35)
#define IS_System(o)            ((o)->h.cid == CLASS_System)
#define IS_bSystem(o)           ((o)->h.bcid == CLASS_System)
#define TYPE_System             CLASS_System
#define CFLAG_System            ((knh_flag_t)FLAG_Class_Singleton)
#define FLAG_System             FLAG_oflag(CFLAG_System)

/* ------------------------------------------------------------------------ */
/* Context */
#define CLASS_Context           ((knh_class_t)36)
#define IS_Context(o)           ((o)->h.cid == CLASS_Context)
#define IS_bContext(o)          ((o)->h.bcid == CLASS_Context)
#define TYPE_Context            CLASS_Context
#define CFLAG_Context           ((knh_flag_t)0)
#define FLAG_Context            FLAG_oflag(CFLAG_Context)

/* ------------------------------------------------------------------------ */
/* Term */
#define CLASS_Term              ((knh_class_t)37)
#define IS_Term(o)              ((o)->h.cid == CLASS_Term)
#define IS_bTerm(o)             ((o)->h.bcid == CLASS_Term)
#define TYPE_Term               CLASS_Term
#define CFLAG_Term              ((knh_flag_t)0)
#define FLAG_Term               FLAG_oflag(CFLAG_Term)

/* ------------------------------------------------------------------------ */
/* Token */
#define CLASS_Token             ((knh_class_t)38)
#define IS_Token(o)             ((o)->h.cid == CLASS_Token)
#define IS_bToken(o)            ((o)->h.bcid == CLASS_Token)
#define TYPE_Token              CLASS_Token
#define CFLAG_Token             ((knh_flag_t)0)
#define FLAG_Token              FLAG_oflag(CFLAG_Token)

/* ------------------------------------------------------------------------ */
/* Stmt */
#define CLASS_Stmt              ((knh_class_t)39)
#define IS_Stmt(o)              ((o)->h.cid == CLASS_Stmt)
#define IS_bStmt(o)             ((o)->h.bcid == CLASS_Stmt)
#define TYPE_Stmt               CLASS_Stmt
#define CFLAG_Stmt              ((knh_flag_t)0)
#define FLAG_Stmt               FLAG_oflag(CFLAG_Stmt)

/* ------------------------------------------------------------------------ */
/* Gamma */
#define CLASS_Gamma             ((knh_class_t)40)
#define IS_Gamma(o)             ((o)->h.cid == CLASS_Gamma)
#define IS_bGamma(o)            ((o)->h.bcid == CLASS_Gamma)
#define TYPE_Gamma              CLASS_Gamma
#define CFLAG_Gamma             ((knh_flag_t)FLAG_Class_Private)
#define FLAG_Gamma              FLAG_oflag(CFLAG_Gamma)

/* ------------------------------------------------------------------------ */
/* BasicBlock */
#define CLASS_BasicBlock        ((knh_class_t)41)
#define IS_BasicBlock(o)        ((o)->h.cid == CLASS_BasicBlock)
#define IS_bBasicBlock(o)       ((o)->h.bcid == CLASS_BasicBlock)
#define TYPE_BasicBlock         CLASS_BasicBlock
#define CFLAG_BasicBlock        ((knh_flag_t)0)
#define FLAG_BasicBlock         FLAG_oflag(CFLAG_BasicBlock)

/* ------------------------------------------------------------------------ */
/* KLRCode */
#define CLASS_KLRCode           ((knh_class_t)42)
#define IS_KLRCode(o)           ((o)->h.cid == CLASS_KLRCode)
#define IS_bKLRCode(o)          ((o)->h.bcid == CLASS_KLRCode)
#define TYPE_KLRCode            CLASS_KLRCode
#define CFLAG_KLRCode           ((knh_flag_t)0)
#define FLAG_KLRCode            FLAG_oflag(CFLAG_KLRCode)

/* ======================================================================== */
/* CLASS */

/* ------------------------------------------------------------------------ */
/* Tvoid */
#define CLASS_Tvoid             ((knh_class_t)43)
#define IS_Tvoid(o)             ((o)->h.cid == CLASS_Tvoid)
#define TYPE_Tvoid              CLASS_Tvoid
#define CFLAG_Tvoid             ((knh_flag_t)FLAG_Class_TypeVariable)
#define FLAG_Tvoid              FLAG_oflag(CFLAG_Tvoid)

/* ------------------------------------------------------------------------ */
/* Tvar */
#define CLASS_Tvar              ((knh_class_t)44)
#define IS_Tvar(o)              ((o)->h.cid == CLASS_Tvar)
#define TYPE_Tvar               CLASS_Tvar
#define CFLAG_Tvar              ((knh_flag_t)FLAG_Class_TypeVariable)
#define FLAG_Tvar               FLAG_oflag(CFLAG_Tvar)

/* ======================================================================== */
/* CLASS */

/* ------------------------------------------------------------------------ */
/* PairSS */
#define CLASS_PairSS            ((knh_class_t)45)
#define IS_PairSS(o)            ((o)->h.cid == CLASS_PairSS)
#define TYPE_PairSS             CLASS_PairSS

/* ------------------------------------------------------------------------ */
/* PairST1 */
#define CLASS_PairST1           ((knh_class_t)46)
#define IS_PairST1(o)           ((o)->h.cid == CLASS_PairST1)
#define TYPE_PairST1            CLASS_PairST1

/* ------------------------------------------------------------------------ */
/* PairT1T2 */
#define CLASS_PairT1T2          ((knh_class_t)47)
#define IS_PairT1T2(o)          ((o)->h.cid == CLASS_PairT1T2)
#define TYPE_PairT1T2           CLASS_PairT1T2

/* ------------------------------------------------------------------------ */
/* Cmpr */
#define CLASS_Cmpr              ((knh_class_t)48)
#define IS_Cmpr(o)              ((o)->h.cid == CLASS_Cmpr)
#define TYPE_Cmpr               CLASS_Cmpr

/* ------------------------------------------------------------------------ */
/* StringITR */
#define CLASS_StringITR         ((knh_class_t)49)
#define IS_StringITR(o)         ((o)->h.cid == CLASS_StringITR)
#define TYPE_StringITR          CLASS_StringITR

/* ------------------------------------------------------------------------ */
/* T1ITR */
#define CLASS_T1ITR             ((knh_class_t)50)
#define IS_T1ITR(o)             ((o)->h.cid == CLASS_T1ITR)
#define TYPE_T1ITR              CLASS_T1ITR

/* ------------------------------------------------------------------------ */
/* ThisITR */
#define CLASS_ThisITR           ((knh_class_t)51)
#define IS_ThisITR(o)           ((o)->h.cid == CLASS_ThisITR)
#define TYPE_ThisITR            CLASS_ThisITR

/* ------------------------------------------------------------------------ */
/* StringARRAY */
#define CLASS_StringARRAY       ((knh_class_t)52)
#define IS_StringARRAY(o)       ((o)->h.cid == CLASS_StringARRAY)
#define TYPE_StringARRAY        CLASS_StringARRAY
#define K_CLASS_INITSIZE                54

/* ======================================================================== */
/* FLAG */
#define FLAG_Object_Release             (knh_flag_t)(1<<0)

#define knh_Object_isRelease(o)  (((o)->h.flag & FLAG_Object_Release) == FLAG_Object_Release)

#define knh_Object_isDebug(o)  (((o)->h.flag & FLAG_Object_Release) != FLAG_Object_Release)
#define FLAG_Object_Immutable           (knh_flag_t)(1<<1)

#define knh_Object_isImmutable(o)  (((o)->h.flag & FLAG_Object_Immutable) == FLAG_Object_Immutable)
#define FLAG_Object_NullObject          (knh_flag_t)(1<<2)

#define knh_Object_isNullObject(o)  (((o)->h.flag & FLAG_Object_NullObject) == FLAG_Object_NullObject)

#define knh_Object_setNullObject(o,b) if(b) (o)->h.flag |= FLAG_Object_NullObject; else (o)->h.flag &= ~(FLAG_Object_NullObject);
#define FLAG_Object_Cyclic              (knh_flag_t)(1<<3)

#define knh_Object_isCyclic(o)  (((o)->h.flag & FLAG_Object_Cyclic) == FLAG_Object_Cyclic)

#define knh_Object_setCyclic(o,b) if(b) (o)->h.flag |= FLAG_Object_Cyclic; else (o)->h.flag &= ~(FLAG_Object_Cyclic);
#define FLAG_Object_Modified            (knh_flag_t)(1<<4)

#define knh_Object_isModified(o)  (((o)->h.flag & FLAG_Object_Modified) == FLAG_Object_Modified)

#define knh_Object_setModified(o,b) if(b) (o)->h.flag |= FLAG_Object_Modified; else (o)->h.flag &= ~(FLAG_Object_Modified);
#define FLAG_Object_Shared              (knh_flag_t)(1<<5)

#define knh_Object_isShared(o)  (((o)->h.flag & FLAG_Object_Shared) == FLAG_Object_Shared)

#define knh_Object_setShared(o,b) if(b) (o)->h.flag |= FLAG_Object_Shared; else (o)->h.flag &= ~(FLAG_Object_Shared);
#define FLAG_Object_Local1              (knh_flag_t)(1<<12)

#define knh_Object_isLocal1(o)  (((o)->h.flag & FLAG_Object_Local1) == FLAG_Object_Local1)

#define knh_Object_setLocal1(o,b) if(b) (o)->h.flag |= FLAG_Object_Local1; else (o)->h.flag &= ~(FLAG_Object_Local1);
#define FLAG_Object_Local2              (knh_flag_t)(1<<13)

#define knh_Object_isLocal2(o)  (((o)->h.flag & FLAG_Object_Local2) == FLAG_Object_Local2)

#define knh_Object_setLocal2(o,b) if(b) (o)->h.flag |= FLAG_Object_Local2; else (o)->h.flag &= ~(FLAG_Object_Local2);
#define FLAG_Object_Local3              (knh_flag_t)(1<<14)

#define knh_Object_isLocal3(o)  (((o)->h.flag & FLAG_Object_Local3) == FLAG_Object_Local3)

#define knh_Object_setLocal3(o,b) if(b) (o)->h.flag |= FLAG_Object_Local3; else (o)->h.flag &= ~(FLAG_Object_Local3);
#define FLAG_Object_Local4              (knh_flag_t)(1<<15)

#define knh_Object_isLocal4(o)  (((o)->h.flag & FLAG_Object_Local4) == FLAG_Object_Local4)

#define knh_Object_setLocal4(o,b) if(b) (o)->h.flag |= FLAG_Object_Local4; else (o)->h.flag &= ~(FLAG_Object_Local4);
#define FLAG_String_TextSgm             FLAG_Object_Local1

#define knh_String_isTextSgm(o)  (((o)->h.flag & FLAG_String_TextSgm) == FLAG_String_TextSgm)

#define knh_String_setTextSgm(o,b) if(b) (o)->h.flag |= FLAG_String_TextSgm; else (o)->h.flag &= ~(FLAG_String_TextSgm);
#define FLAG_String_ASCII               FLAG_Object_Local2

#define knh_String_isASCII(o)  (((o)->h.flag & FLAG_String_ASCII) == FLAG_String_ASCII)

#define knh_String_setASCII(o,b) if(b) (o)->h.flag |= FLAG_String_ASCII; else (o)->h.flag &= ~(FLAG_String_ASCII);
#define FLAG_Bytes_Static               FLAG_Object_Local1

#define knh_Bytes_isStatic(o)  (((o)->h.flag & FLAG_Bytes_Static) == FLAG_Bytes_Static)

#define knh_Bytes_setStatic(o,b) if(b) (o)->h.flag |= FLAG_Bytes_Static; else (o)->h.flag &= ~(FLAG_Bytes_Static);
#define FLAG_Tuple_Triple               FLAG_Object_Local1

#define knh_Tuple_isTriple(o)  (((o)->h.flag & FLAG_Tuple_Triple) == FLAG_Tuple_Triple)

#define knh_Tuple_setTriple(o,b) if(b) (o)->h.flag |= FLAG_Tuple_Triple; else (o)->h.flag &= ~(FLAG_Tuple_Triple);
#define FLAG_Range_Inclusive            FLAG_Object_Local1

#define knh_Range_isInclusive(o)  (((o)->h.flag & FLAG_Range_Inclusive) == FLAG_Range_Inclusive)

#define knh_Range_setInclusive(o,b) if(b) (o)->h.flag |= FLAG_Range_Inclusive; else (o)->h.flag &= ~(FLAG_Range_Inclusive);
#define FLAG_Array_NDATA                FLAG_Object_Local1

#define knh_Array_isNDATA(o)  (((o)->h.flag & FLAG_Array_NDATA) == FLAG_Array_NDATA)

#define knh_Array_setNDATA(o,b) if(b) (o)->h.flag |= FLAG_Array_NDATA; else (o)->h.flag &= ~(FLAG_Array_NDATA);
#define FLAG_Array_SORTED               FLAG_Object_Local2

#define knh_Array_isSORTED(o)  (((o)->h.flag & FLAG_Array_SORTED) == FLAG_Array_SORTED)

#define knh_Array_setSORTED(o,b) if(b) (o)->h.flag |= FLAG_Array_SORTED; else (o)->h.flag &= ~(FLAG_Array_SORTED);
#define FLAG_Array_Dimension            FLAG_Object_Local3

#define knh_Array_hasDimension(o)  (((o)->h.flag & FLAG_Array_Dimension) == FLAG_Array_Dimension)

#define knh_Array_setDimension(o,b) if(b) (o)->h.flag |= FLAG_Array_Dimension; else (o)->h.flag &= ~(FLAG_Array_Dimension);
#define FLAG_Class_Release              (knh_flag_t)(1<<0)

#define knh_class_isRelease(o)  (((pClassTBL(ctx,o))->cflag & FLAG_Class_Release) == FLAG_Class_Release)

#define knh_class_isDebug(o)  (((pClassTBL(ctx,o))->cflag & FLAG_Class_Release) != FLAG_Class_Release)
#define FLAG_Class_Immutable            (knh_flag_t)(1<<1)

#define knh_class_isImmutable(o)  (((pClassTBL(ctx,o))->cflag & FLAG_Class_Immutable) == FLAG_Class_Immutable)
#define FLAG_Class_Cyclic               (knh_flag_t)(1<<2)

#define knh_class_isCyclic(o)  (((pClassTBL(ctx,o))->cflag & FLAG_Class_Cyclic) == FLAG_Class_Cyclic)

#define knh_class_setCyclic(o,b) if(b) (pClassTBL(ctx,o))->cflag |= FLAG_Class_Cyclic; else (pClassTBL(ctx,o))->cflag &= ~(FLAG_Class_Cyclic);
#define FLAG_Class_MetaClass            (knh_flag_t)(1<<3)

#define knh_class_isMetaClass(o)  (((pClassTBL(ctx,o))->cflag & FLAG_Class_MetaClass) == FLAG_Class_MetaClass)
#define FLAG_Class_Private              (knh_flag_t)(1<<4)

#define knh_class_isPrivate(o)  (((pClassTBL(ctx,o))->cflag & FLAG_Class_Private) == FLAG_Class_Private)

#define knh_class_isPublic(o)  (((pClassTBL(ctx,o))->cflag & FLAG_Class_Private) != FLAG_Class_Private)
#define FLAG_Class_Final                (knh_flag_t)(1<<5)

#define knh_class_isFinal(o)  (((pClassTBL(ctx,o))->cflag & FLAG_Class_Final) == FLAG_Class_Final)
#define FLAG_Class_Singleton            (knh_flag_t)(1<<6)

#define knh_class_isSingleton(o)  (((pClassTBL(ctx,o))->cflag & FLAG_Class_Singleton) == FLAG_Class_Singleton)
#define FLAG_Class_Unique               (knh_flag_t)(1<<7)

#define knh_class_isUnique(o)  (((pClassTBL(ctx,o))->cflag & FLAG_Class_Unique) == FLAG_Class_Unique)
#define FLAG_Class_Interface            (knh_flag_t)(1<<8)

#define knh_class_isInterface(o)  (((pClassTBL(ctx,o))->cflag & FLAG_Class_Interface) == FLAG_Class_Interface)
#define FLAG_Class_TypeVariable         (knh_flag_t)(1<<9)

#define knh_class_isTypeVariable(o)  (((pClassTBL(ctx,o))->cflag & FLAG_Class_TypeVariable) == FLAG_Class_TypeVariable)

#define knh_class_setTypeVariable(o,b) if(b) (pClassTBL(ctx,o))->cflag |= FLAG_Class_TypeVariable; else (pClassTBL(ctx,o))->cflag &= ~(FLAG_Class_TypeVariable);
#define FLAG_ParamArray_VARGs           FLAG_Object_Local1

#define knh_ParamArray_isVARGs(o)  (((o)->h.flag & FLAG_ParamArray_VARGs) == FLAG_ParamArray_VARGs)

#define knh_ParamArray_setVARGs(o,b) if(b) (o)->h.flag |= FLAG_ParamArray_VARGs; else (o)->h.flag &= ~(FLAG_ParamArray_VARGs);
#define FLAG_ParamArray_RVARGs          FLAG_Object_Local2

#define knh_ParamArray_isRVARGs(o)  (((o)->h.flag & FLAG_ParamArray_RVARGs) == FLAG_ParamArray_RVARGs)

#define knh_ParamArray_setRVARGs(o,b) if(b) (o)->h.flag |= FLAG_ParamArray_RVARGs; else (o)->h.flag &= ~(FLAG_ParamArray_RVARGs);
#define FLAG_Method_Private             (knh_flag_t)(1<<0)

#define knh_Method_isPrivate(o)  ((DP(o)->flag & FLAG_Method_Private) == FLAG_Method_Private)

#define knh_Method_isPublic(o)  ((DP(o)->flag & FLAG_Method_Private) != FLAG_Method_Private)

#define knh_Method_setPrivate(o,b) if(b) DP(o)->flag |= FLAG_Method_Private; else DP(o)->flag &= ~(FLAG_Method_Private);

#define knh_Method_setPublic(o,b)  if(b) DP(o)->flag &= ~(FLAG_Method_Private); else DP(o)->flag |= FLAG_Method_Private;
#define FLAG_Method_Virtual             (knh_flag_t)(1<<1)

#define knh_Method_isVirtual(o)  ((DP(o)->flag & FLAG_Method_Virtual) == FLAG_Method_Virtual)

#define knh_Method_isFinal(o)  ((DP(o)->flag & FLAG_Method_Virtual) != FLAG_Method_Virtual)

#define knh_Method_setVirtual(o,b) if(b) DP(o)->flag |= FLAG_Method_Virtual; else DP(o)->flag &= ~(FLAG_Method_Virtual);

#define knh_Method_setFinal(o,b)  if(b) DP(o)->flag &= ~(FLAG_Method_Virtual); else DP(o)->flag |= FLAG_Method_Virtual;
#define FLAG_Method_Debug               (knh_flag_t)(1<<2)

#define knh_Method_isDebug(o)  ((DP(o)->flag & FLAG_Method_Debug) == FLAG_Method_Debug)

#define knh_Method_setDebug(o,b) if(b) DP(o)->flag |= FLAG_Method_Debug; else DP(o)->flag &= ~(FLAG_Method_Debug);
#define FLAG_Method_Const               (knh_flag_t)(1<<3)

#define knh_Method_isConst(o)  ((DP(o)->flag & FLAG_Method_Const) == FLAG_Method_Const)
#define FLAG_Method_Static              (knh_flag_t)(1<<4)

#define knh_Method_isStatic(o)  ((DP(o)->flag & FLAG_Method_Static) == FLAG_Method_Static)
#define FLAG_Method_ObjectCode          (knh_flag_t)(1<<5)

#define knh_Method_isObjectCode(o)  ((DP(o)->flag & FLAG_Method_ObjectCode) == FLAG_Method_ObjectCode)

#define knh_Method_setObjectCode(o,b) if(b) DP(o)->flag |= FLAG_Method_ObjectCode; else DP(o)->flag &= ~(FLAG_Method_ObjectCode);
#define FLAG_Method_Hidden              (knh_flag_t)(1<<6)

#define knh_Method_isHidden(o)  ((DP(o)->flag & FLAG_Method_Hidden) == FLAG_Method_Hidden)

#define knh_Method_setHidden(o,b) if(b) DP(o)->flag |= FLAG_Method_Hidden; else DP(o)->flag &= ~(FLAG_Method_Hidden);
#define FLAG_Method_Dynamic             (knh_flag_t)(1<<7)

#define knh_Method_isDynamic(o)  ((DP(o)->flag & FLAG_Method_Dynamic) == FLAG_Method_Dynamic)

#define knh_Method_setDynamic(o,b) if(b) DP(o)->flag |= FLAG_Method_Dynamic; else DP(o)->flag &= ~(FLAG_Method_Dynamic);
#define FLAG_Method_Smart               (knh_flag_t)(1<<8)

#define knh_Method_isSmart(o)  ((DP(o)->flag & FLAG_Method_Smart) == FLAG_Method_Smart)

#define knh_Method_setSmart(o,b) if(b) DP(o)->flag |= FLAG_Method_Smart; else DP(o)->flag &= ~(FLAG_Method_Smart);
#define FLAG_Method_Tricky              (knh_flag_t)(1<<9)

#define knh_Method_isTricky(o)  ((DP(o)->flag & FLAG_Method_Tricky) == FLAG_Method_Tricky)

#define knh_Method_setTricky(o,b) if(b) DP(o)->flag |= FLAG_Method_Tricky; else DP(o)->flag &= ~(FLAG_Method_Tricky);
#define FLAG_Translator_Interface       (knh_flag_t)(1<<0)

#define knh_Translator_isInterface(o)  ((DP(o)->flag & FLAG_Translator_Interface) == FLAG_Translator_Interface)

#define knh_Translator_setInterface(o,b) if(b) DP(o)->flag |= FLAG_Translator_Interface; else DP(o)->flag &= ~(FLAG_Translator_Interface);
#define FLAG_Translator_Significant     (knh_flag_t)(1<<1)

#define knh_Translator_isSignificant(o)  ((DP(o)->flag & FLAG_Translator_Significant) == FLAG_Translator_Significant)

#define knh_Translator_setSignificant(o,b) if(b) DP(o)->flag |= FLAG_Translator_Significant; else DP(o)->flag &= ~(FLAG_Translator_Significant);
#define FLAG_Translator_Semantic        (knh_flag_t)(1<<2)

#define knh_Translator_isSemantic(o)  ((DP(o)->flag & FLAG_Translator_Semantic) == FLAG_Translator_Semantic)
#define FLAG_Translator_Total           (knh_flag_t)(1<<3)

#define knh_Translator_isTotal(o)  ((DP(o)->flag & FLAG_Translator_Total) == FLAG_Translator_Total)

#define knh_Translator_isPartial(o)  ((DP(o)->flag & FLAG_Translator_Total) != FLAG_Translator_Total)

#define knh_Translator_setTotal(o,b) if(b) DP(o)->flag |= FLAG_Translator_Total; else DP(o)->flag &= ~(FLAG_Translator_Total);

#define knh_Translator_setPartial(o,b)  if(b) DP(o)->flag &= ~(FLAG_Translator_Total); else DP(o)->flag |= FLAG_Translator_Total;
#define FLAG_Translator_LossLess        (knh_flag_t)(1<<4)

#define knh_Translator_isLossLess(o)  ((DP(o)->flag & FLAG_Translator_LossLess) == FLAG_Translator_LossLess)

#define knh_Translator_setLossLess(o,b) if(b) DP(o)->flag |= FLAG_Translator_LossLess; else DP(o)->flag &= ~(FLAG_Translator_LossLess);
#define FLAG_Translator_Final           (knh_flag_t)(1<<6)

#define knh_Translator_isFinal(o)  ((DP(o)->flag & FLAG_Translator_Final) == FLAG_Translator_Final)

#define knh_Translator_setFinal(o,b) if(b) DP(o)->flag |= FLAG_Translator_Final; else DP(o)->flag &= ~(FLAG_Translator_Final);
#define FLAG_Translator_Const           (knh_flag_t)(1<<7)

#define knh_Translator_isConst(o)  ((DP(o)->flag & FLAG_Translator_Const) == FLAG_Translator_Const)

#define knh_Translator_isTemporal(o)  ((DP(o)->flag & FLAG_Translator_Const) != FLAG_Translator_Const)

#define knh_Translator_setConst(o,b) if(b) DP(o)->flag |= FLAG_Translator_Const; else DP(o)->flag &= ~(FLAG_Translator_Const);

#define knh_Translator_setTemporal(o,b)  if(b) DP(o)->flag &= ~(FLAG_Translator_Const); else DP(o)->flag |= FLAG_Translator_Const;
#define FLAG_Translator_MapMap          (knh_flag_t)(1<<8)

#define knh_Translator_isMapMap(o)  ((DP(o)->flag & FLAG_Translator_MapMap) == FLAG_Translator_MapMap)

#define knh_Translator_setMapMap(o,b) if(b) DP(o)->flag |= FLAG_Translator_MapMap; else DP(o)->flag &= ~(FLAG_Translator_MapMap);
#define FLAG_Func_StoredEnv             FLAG_Object_Local1

#define knh_Func_isStoredEnv(o)  (((o)->h.flag & FLAG_Func_StoredEnv) == FLAG_Func_StoredEnv)

#define knh_Func_setStoredEnv(o,b) if(b) (o)->h.flag |= FLAG_Func_StoredEnv; else (o)->h.flag &= ~(FLAG_Func_StoredEnv);
#define FLAG_Thunk_Evaluated            FLAG_Object_Local1

#define knh_Thunk_isEvaluated(o)  (((o)->h.flag & FLAG_Thunk_Evaluated) == FLAG_Thunk_Evaluated)

#define knh_Thunk_setEvaluated(o,b) if(b) (o)->h.flag |= FLAG_Thunk_Evaluated; else (o)->h.flag &= ~(FLAG_Thunk_Evaluated);
#define FLAG_Exception_Logging          (knh_flag_t)(1<<0)

#define knh_Exception_isLogging(o)  ((DP(o)->flag & FLAG_Exception_Logging) == FLAG_Exception_Logging)
#define FLAG_Exception_Caught           (knh_flag_t)(1<<1)

#define knh_Exception_isCaught(o)  ((DP(o)->flag & FLAG_Exception_Caught) == FLAG_Exception_Caught)

#define knh_Exception_setCaught(o,b) if(b) DP(o)->flag |= FLAG_Exception_Caught; else DP(o)->flag &= ~(FLAG_Exception_Caught);
#define FLAG_ExceptionHandler_Jumpable  FLAG_Object_Local1

#define knh_ExceptionHandler_isJumpable(o)  (((o)->h.flag & FLAG_ExceptionHandler_Jumpable) == FLAG_ExceptionHandler_Jumpable)

#define knh_ExceptionHandler_setJumpable(o,b) if(b) (o)->h.flag |= FLAG_ExceptionHandler_Jumpable; else (o)->h.flag &= ~(FLAG_ExceptionHandler_Jumpable);
#define FLAG_ExceptionHandler_Catching  FLAG_Object_Local2

#define knh_ExceptionHandler_isCatching(o)  (((o)->h.flag & FLAG_ExceptionHandler_Catching) == FLAG_ExceptionHandler_Catching)

#define knh_ExceptionHandler_setCatching(o,b) if(b) (o)->h.flag |= FLAG_ExceptionHandler_Catching; else (o)->h.flag &= ~(FLAG_ExceptionHandler_Catching);
#define FLAG_InputStream_FILE           FLAG_Object_Local1

#define knh_InputStream_isFILE(o)  (((o)->h.flag & FLAG_InputStream_FILE) == FLAG_InputStream_FILE)

#define knh_InputStream_setFILE(o,b) if(b) (o)->h.flag |= FLAG_InputStream_FILE; else (o)->h.flag &= ~(FLAG_InputStream_FILE);
#define FLAG_OutputStream_BOL           FLAG_Object_Local1

#define knh_OutputStream_isBOL(o)  (((o)->h.flag & FLAG_OutputStream_BOL) == FLAG_OutputStream_BOL)

#define knh_OutputStream_setBOL(o,b) if(b) (o)->h.flag |= FLAG_OutputStream_BOL; else (o)->h.flag &= ~(FLAG_OutputStream_BOL);
#define FLAG_OutputStream_AutoFlush     FLAG_Object_Local2

#define knh_OutputStream_isAutoFlush(o)  (((o)->h.flag & FLAG_OutputStream_AutoFlush) == FLAG_OutputStream_AutoFlush)

#define knh_OutputStream_setAutoFlush(o,b) if(b) (o)->h.flag |= FLAG_OutputStream_AutoFlush; else (o)->h.flag &= ~(FLAG_OutputStream_AutoFlush);
#define FLAG_OutputStream_StoringBuffer FLAG_Object_Local3

#define knh_OutputStream_isStoringBuffer(o)  (((o)->h.flag & FLAG_OutputStream_StoringBuffer) == FLAG_OutputStream_StoringBuffer)

#define knh_OutputStream_setStoringBuffer(o,b) if(b) (o)->h.flag |= FLAG_OutputStream_StoringBuffer; else (o)->h.flag &= ~(FLAG_OutputStream_StoringBuffer);
#define FLAG_Context_Strict             (knh_flag_t)(1<<0)

#define knh_Context_isStrict(o)  ((((knh_Context_t*)o)->flag & FLAG_Context_Strict) == FLAG_Context_Strict)

#define knh_Context_setStrict(o,b) if(b) ((knh_Context_t*)o)->flag |= FLAG_Context_Strict; else ((knh_Context_t*)o)->flag &= ~(FLAG_Context_Strict);
#define FLAG_Context_Debug              (knh_flag_t)(1<<1)

#define knh_Context_isDebug(o)  ((((knh_Context_t*)o)->flag & FLAG_Context_Debug) == FLAG_Context_Debug)

#define knh_Context_setDebug(o,b) if(b) ((knh_Context_t*)o)->flag |= FLAG_Context_Debug; else ((knh_Context_t*)o)->flag &= ~(FLAG_Context_Debug);
#define FLAG_Context_Verbose            (knh_flag_t)(1<<2)

#define knh_Context_isVerbose(o)  ((((knh_Context_t*)o)->flag & FLAG_Context_Verbose) == FLAG_Context_Verbose)

#define knh_Context_setVerbose(o,b) if(b) ((knh_Context_t*)o)->flag |= FLAG_Context_Verbose; else ((knh_Context_t*)o)->flag &= ~(FLAG_Context_Verbose);
#define FLAG_Context_Interactive        (knh_flag_t)(1<<3)

#define knh_Context_isInteractive(o)  ((((knh_Context_t*)o)->flag & FLAG_Context_Interactive) == FLAG_Context_Interactive)

#define knh_Context_setInteractive(o,b) if(b) ((knh_Context_t*)o)->flag |= FLAG_Context_Interactive; else ((knh_Context_t*)o)->flag &= ~(FLAG_Context_Interactive);
#define FLAG_Context_Compiling          (knh_flag_t)(1<<4)

#define knh_Context_isCompiling(o)  ((((knh_Context_t*)o)->flag & FLAG_Context_Compiling) == FLAG_Context_Compiling)

#define knh_Context_setCompiling(o,b) if(b) ((knh_Context_t*)o)->flag |= FLAG_Context_Compiling; else ((knh_Context_t*)o)->flag &= ~(FLAG_Context_Compiling);
#define FLAG_Token_BOL                  (knh_flag_t)(1<<0)

#define knh_Token_isBOL(o)  ((DP(o)->flag0 & FLAG_Token_BOL) == FLAG_Token_BOL)

#define knh_Token_setBOL(o,b) if(b) DP(o)->flag0 |= FLAG_Token_BOL; else DP(o)->flag0 &= ~(FLAG_Token_BOL);
#define FLAG_Token_DOT                  (knh_flag_t)(1<<1)

#define knh_Token_isDOT(o)  ((DP(o)->flag0 & FLAG_Token_DOT) == FLAG_Token_DOT)

#define knh_Token_setDOT(o,b) if(b) DP(o)->flag0 |= FLAG_Token_DOT; else DP(o)->flag0 &= ~(FLAG_Token_DOT);
#define FLAG_Token_LCASE                (knh_flag_t)(1<<2)

#define knh_Token_hasLCASE(o)  ((DP(o)->flag0 & FLAG_Token_LCASE) == FLAG_Token_LCASE)

#define knh_Token_setLCASE(o,b) if(b) DP(o)->flag0 |= FLAG_Token_LCASE; else DP(o)->flag0 &= ~(FLAG_Token_LCASE);
#define FLAG_Token_USCORE               (knh_flag_t)(1<<3)

#define knh_Token_hasUSCORE(o)  ((DP(o)->flag0 & FLAG_Token_USCORE) == FLAG_Token_USCORE)

#define knh_Token_setUSCORE(o,b) if(b) DP(o)->flag0 |= FLAG_Token_USCORE; else DP(o)->flag0 &= ~(FLAG_Token_USCORE);
#define FLAG_Token_Getter               (knh_flag_t)(1<<4)

#define knh_Token_isGetter(o)  ((DP(o)->flag0 & FLAG_Token_Getter) == FLAG_Token_Getter)

#define knh_Token_setGetter(o,b) if(b) DP(o)->flag0 |= FLAG_Token_Getter; else DP(o)->flag0 &= ~(FLAG_Token_Getter);
#define FLAG_Token_Setter               (knh_flag_t)(1<<5)

#define knh_Token_isSetter(o)  ((DP(o)->flag0 & FLAG_Token_Setter) == FLAG_Token_Setter)

#define knh_Token_setSetter(o,b) if(b) DP(o)->flag0 |= FLAG_Token_Setter; else DP(o)->flag0 &= ~(FLAG_Token_Setter);
#define FLAG_Token_ExceptionType        (knh_flag_t)(1<<6)

#define knh_Token_isExceptionType(o)  ((DP(o)->flag0 & FLAG_Token_ExceptionType) == FLAG_Token_ExceptionType)

#define knh_Token_setExceptionType(o,b) if(b) DP(o)->flag0 |= FLAG_Token_ExceptionType; else DP(o)->flag0 &= ~(FLAG_Token_ExceptionType);
#define FLAG_Token_MEMO1                (knh_flag_t)(1<<7)

#define knh_Token_isMEMO1(o)  ((DP(o)->flag0 & FLAG_Token_MEMO1) == FLAG_Token_MEMO1)

#define knh_Token_setMEMO1(o,b) if(b) DP(o)->flag0 |= FLAG_Token_MEMO1; else DP(o)->flag0 &= ~(FLAG_Token_MEMO1);
#define FLAG_Stmt_EveryLine             (knh_flag_t)(1<<0)

#define knh_Stmt_isEveryLine(o)  ((DP(o)->flag0 & FLAG_Stmt_EveryLine) == FLAG_Stmt_EveryLine)

#define knh_Stmt_setEveryLine(o,b) if(b) DP(o)->flag0 |= FLAG_Stmt_EveryLine; else DP(o)->flag0 &= ~(FLAG_Stmt_EveryLine);
#define FLAG_Stmt_TailReturn            (knh_flag_t)(1<<1)

#define knh_Stmt_isTailReturn(o)  ((DP(o)->flag0 & FLAG_Stmt_TailReturn) == FLAG_Stmt_TailReturn)

#define knh_Stmt_setTailReturn(o,b) if(b) DP(o)->flag0 |= FLAG_Stmt_TailReturn; else DP(o)->flag0 &= ~(FLAG_Stmt_TailReturn);
#define FLAG_Stmt_Memo1                 (knh_flag_t)(1<<5)

#define knh_Stmt_isMemo1(o)  ((DP(o)->flag0 & FLAG_Stmt_Memo1) == FLAG_Stmt_Memo1)

#define knh_Stmt_setMemo1(o,b) if(b) DP(o)->flag0 |= FLAG_Stmt_Memo1; else DP(o)->flag0 &= ~(FLAG_Stmt_Memo1);
#define FLAG_Stmt_Memo2                 (knh_flag_t)(1<<6)

#define knh_Stmt_isMemo2(o)  ((DP(o)->flag0 & FLAG_Stmt_Memo2) == FLAG_Stmt_Memo2)

#define knh_Stmt_setMemo2(o,b) if(b) DP(o)->flag0 |= FLAG_Stmt_Memo2; else DP(o)->flag0 &= ~(FLAG_Stmt_Memo2);
#define FLAG_Gamma_Cancelled            (knh_flag_t)(1<<0)

#define knh_Gamma_isCancelled(o)  ((DP(o)->flag & FLAG_Gamma_Cancelled) == FLAG_Gamma_Cancelled)

#define knh_Gamma_setCancelled(o,b) if(b) DP(o)->flag |= FLAG_Gamma_Cancelled; else DP(o)->flag &= ~(FLAG_Gamma_Cancelled);
#define FLAG_Gamma_Quiet                (knh_flag_t)(1<<1)

#define knh_Gamma_isQuiet(o)  ((DP(o)->flag & FLAG_Gamma_Quiet) == FLAG_Gamma_Quiet)

#define knh_Gamma_setQuiet(o,b) if(b) DP(o)->flag |= FLAG_Gamma_Quiet; else DP(o)->flag &= ~(FLAG_Gamma_Quiet);
#define FLAG_Gamma_Throwable            (knh_flag_t)(1<<2)

#define knh_Gamma_isThrowable(o)  ((DP(o)->flag & FLAG_Gamma_Throwable) == FLAG_Gamma_Throwable)

#define knh_Gamma_setThrowable(o,b) if(b) DP(o)->flag |= FLAG_Gamma_Throwable; else DP(o)->flag &= ~(FLAG_Gamma_Throwable);
#define FLAG_Gamma_PROCEED              (knh_flag_t)(1<<3)

#define knh_Gamma_hasPROCEED(o)  ((DP(o)->flag & FLAG_Gamma_PROCEED) == FLAG_Gamma_PROCEED)

#define knh_Gamma_foundPROCEED(o,b) if(b) DP(o)->flag |= FLAG_Gamma_PROCEED; else DP(o)->flag &= ~(FLAG_Gamma_PROCEED);
#define FLAG_Gamma_RETURN               (knh_flag_t)(1<<4)

#define knh_Gamma_hasRETURN(o)  ((DP(o)->flag & FLAG_Gamma_RETURN) == FLAG_Gamma_RETURN)

#define knh_Gamma_foundRETURN(o,b) if(b) DP(o)->flag |= FLAG_Gamma_RETURN; else DP(o)->flag &= ~(FLAG_Gamma_RETURN);
#define FLAG_Gamma_YEILD                (knh_flag_t)(1<<5)

#define knh_Gamma_hasYEILD(o)  ((DP(o)->flag & FLAG_Gamma_YEILD) == FLAG_Gamma_YEILD)

#define knh_Gamma_foundYEILD(o,b) if(b) DP(o)->flag |= FLAG_Gamma_YEILD; else DP(o)->flag &= ~(FLAG_Gamma_YEILD);
#define FLAG_Gamma_FIELD                (knh_flag_t)(1<<6)

#define knh_Gamma_hasFIELD(o)  ((DP(o)->flag & FLAG_Gamma_FIELD) == FLAG_Gamma_FIELD)

#define knh_Gamma_foundFIELD(o,b) if(b) DP(o)->flag |= FLAG_Gamma_FIELD; else DP(o)->flag &= ~(FLAG_Gamma_FIELD);
#define FLAG_Gamma_STACK                (knh_flag_t)(1<<7)

#define knh_Gamma_hasSTACK(o)  ((DP(o)->flag & FLAG_Gamma_STACK) == FLAG_Gamma_STACK)

#define knh_Gamma_foundSTACK(o,b) if(b) DP(o)->flag |= FLAG_Gamma_STACK; else DP(o)->flag &= ~(FLAG_Gamma_STACK);
#define FLAG_Gamma_SCRIPT               (knh_flag_t)(1<<7)

#define knh_Gamma_hasSCRIPT(o)  ((DP(o)->flag & FLAG_Gamma_SCRIPT) == FLAG_Gamma_SCRIPT)

#define knh_Gamma_foundSCRIPT(o,b) if(b) DP(o)->flag |= FLAG_Gamma_SCRIPT; else DP(o)->flag &= ~(FLAG_Gamma_SCRIPT);
#define FLAG_Gamma_InlineFunction       (knh_flag_t)(1<<0)

#define knh_Gamma_isInlineFunction(o)  ((DP(o)->cflag & FLAG_Gamma_InlineFunction) == FLAG_Gamma_InlineFunction)

#define knh_Gamma_setInlineFunction(o,b) if(b) DP(o)->cflag |= FLAG_Gamma_InlineFunction; else DP(o)->cflag &= ~(FLAG_Gamma_InlineFunction);
#define FLAG_Gamma_TailRecursion        (knh_flag_t)(1<<1)

#define knh_Gamma_isTailRecursion(o)  ((DP(o)->cflag & FLAG_Gamma_TailRecursion) == FLAG_Gamma_TailRecursion)

#define knh_Gamma_setTailRecursion(o,b) if(b) DP(o)->cflag |= FLAG_Gamma_TailRecursion; else DP(o)->cflag &= ~(FLAG_Gamma_TailRecursion);
#define FLAG_BasicBlock_Visited         FLAG_Object_Local1

#define knh_BasicBlock_isVisited(o)  (((o)->h.flag & FLAG_BasicBlock_Visited) == FLAG_BasicBlock_Visited)

#define knh_BasicBlock_setVisited(o,b) if(b) (o)->h.flag |= FLAG_BasicBlock_Visited; else (o)->h.flag &= ~(FLAG_BasicBlock_Visited);
#define FLAG_BasicBlock_StackChecked    FLAG_Object_Local2

#define knh_BasicBlock_isStackChecked(o)  (((o)->h.flag & FLAG_BasicBlock_StackChecked) == FLAG_BasicBlock_StackChecked)

#define knh_BasicBlock_setStackChecked(o,b) if(b) (o)->h.flag |= FLAG_BasicBlock_StackChecked; else (o)->h.flag &= ~(FLAG_BasicBlock_StackChecked);

/* ======================================================================== */
/* EXPT */
#define EBI_Exception           1
#define EBI_Fatal               2
#define EBI_OutOfMemory         3
#define EBI_Security            4
#define EBI_Null                5
#define EBI_OutOfIndex          6
#define EBI_IO                  7
#define EBI_Script              8
#define EBI_Syntax              9
#define EBI_Type                10
#define EBI_Assertion           11

/* ======================================================================== */
/* FIELDN */
#define FN_             (MN_OPSIZE+0)
#define FN_abstract     (MN_OPSIZE+1)
#define FN_add          (MN_OPSIZE+2)
#define FN_all          (MN_OPSIZE+3)
#define FN_aSCII        (MN_OPSIZE+4)
#define FN_ascii        (MN_OPSIZE+4)
#define FN_autoFlush    (MN_OPSIZE+5)
#define FN_autoflush    (MN_OPSIZE+5)
#define FN_bag          (MN_OPSIZE+6)
#define FN_base         (MN_OPSIZE+7)
#define FN_bits         (MN_OPSIZE+8)
#define FN_buf          (MN_OPSIZE+9)
#define FN_c            (MN_OPSIZE+10)
#define FN_ch           (MN_OPSIZE+11)
#define FN_char         (MN_OPSIZE+12)
#define FN_charset      (MN_OPSIZE+13)
#define FN_class        (MN_OPSIZE+14)
#define FN_clearBuffer  (MN_OPSIZE+15)
#define FN_clearbuffer  (MN_OPSIZE+15)
#define FN_close        (MN_OPSIZE+16)
#define FN_closed       (MN_OPSIZE+17)
#define FN_const        (MN_OPSIZE+18)
#define FN_convert      (MN_OPSIZE+19)
#define FN_d            (MN_OPSIZE+20)
#define FN_data         (MN_OPSIZE+21)
#define FN_debug        (MN_OPSIZE+22)
#define FN_decode       (MN_OPSIZE+23)
#define FN_defined      (MN_OPSIZE+24)
#define FN_delegate     (MN_OPSIZE+25)
#define FN_domain       (MN_OPSIZE+26)
#define FN_dump         (MN_OPSIZE+27)
#define FN_e            (MN_OPSIZE+28)
#define FN_empty        (MN_OPSIZE+29)
#define FN_enc          (MN_OPSIZE+30)
#define FN_encode       (MN_OPSIZE+31)
#define FN_end          (MN_OPSIZE+32)
#define FN_endsWith     (MN_OPSIZE+33)
#define FN_endswith     (MN_OPSIZE+33)
#define FN_equals       (MN_OPSIZE+34)
#define FN_err          (MN_OPSIZE+35)
#define FN_exec         (MN_OPSIZE+36)
#define FN_exit         (MN_OPSIZE+37)
#define FN_f            (MN_OPSIZE+38)
#define FN_final        (MN_OPSIZE+39)
#define FN_flag         (MN_OPSIZE+40)
#define FN_float        (MN_OPSIZE+41)
#define FN_floatToIntBits (MN_OPSIZE+42)
#define FN_floattointbits (MN_OPSIZE+42)
#define FN_flush        (MN_OPSIZE+43)
#define FN_fmt          (MN_OPSIZE+44)
#define FN_format       (MN_OPSIZE+45)
#define FN_gc           (MN_OPSIZE+46)
#define FN_hashCode     (MN_OPSIZE+47)
#define FN_hashcode     (MN_OPSIZE+47)
#define FN_immutable    (MN_OPSIZE+48)
#define FN_in           (MN_OPSIZE+49)
#define FN_indexOf      (MN_OPSIZE+50)
#define FN_indexof      (MN_OPSIZE+50)
#define FN_init         (MN_OPSIZE+51)
#define FN_initCapacity (MN_OPSIZE+52)
#define FN_initcapacity (MN_OPSIZE+52)
#define FN_int          (MN_OPSIZE+53)
#define FN_interactive  (MN_OPSIZE+54)
#define FN_interface    (MN_OPSIZE+55)
#define FN_intToFloatBits (MN_OPSIZE+56)
#define FN_inttofloatbits (MN_OPSIZE+56)
#define FN_invoke       (MN_OPSIZE+57)
#define FN_it           (MN_OPSIZE+58)
#define FN_k            (MN_OPSIZE+59)
#define FN_key          (MN_OPSIZE+60)
#define FN_lastIndexOf  (MN_OPSIZE+61)
#define FN_lastindexof  (MN_OPSIZE+61)
#define FN_length       (MN_OPSIZE+62)
#define FN_likely       (MN_OPSIZE+63)
#define FN_listProperties (MN_OPSIZE+64)
#define FN_listproperties (MN_OPSIZE+64)
#define FN_logging      (MN_OPSIZE+65)
#define FN_lossLess     (MN_OPSIZE+66)
#define FN_lossless     (MN_OPSIZE+66)
#define FN_main         (MN_OPSIZE+67)
#define FN_man          (MN_OPSIZE+68)
#define FN_metaClass    (MN_OPSIZE+69)
#define FN_metaclass    (MN_OPSIZE+69)
#define FN_method       (MN_OPSIZE+70)
#define FN_mode         (MN_OPSIZE+71)
#define FN_modified     (MN_OPSIZE+72)
#define FN_msg          (MN_OPSIZE+73)
#define FN_n            (MN_OPSIZE+74)
#define FN_name         (MN_OPSIZE+75)
#define FN_new          (MN_OPSIZE+76)
#define FN_new__ARRAY   (MN_OPSIZE+77)
#define FN_new__array   (MN_OPSIZE+77)
#define FN_new__LIST    (MN_OPSIZE+78)
#define FN_new__list    (MN_OPSIZE+78)
#define FN_new__MAP     (MN_OPSIZE+79)
#define FN_new__map     (MN_OPSIZE+79)
#define FN_new__RANGE   (MN_OPSIZE+80)
#define FN_new__range   (MN_OPSIZE+80)
#define FN_new__TUPLE   (MN_OPSIZE+81)
#define FN_new__tuple   (MN_OPSIZE+81)
#define FN_next         (MN_OPSIZE+82)
#define FN_notNull      (MN_OPSIZE+83)
#define FN_notnull      (MN_OPSIZE+83)
#define FN_ns           (MN_OPSIZE+84)
#define FN_null         (MN_OPSIZE+85)
#define FN_offset       (MN_OPSIZE+86)
#define FN_option       (MN_OPSIZE+87)
#define FN_out          (MN_OPSIZE+88)
#define FN_p            (MN_OPSIZE+89)
#define FN_partial      (MN_OPSIZE+90)
#define FN_path         (MN_OPSIZE+91)
#define FN_pattern      (MN_OPSIZE+92)
#define FN_print        (MN_OPSIZE+93)
#define FN_println      (MN_OPSIZE+94)
#define FN_private      (MN_OPSIZE+95)
#define FN_property     (MN_OPSIZE+96)
#define FN_public       (MN_OPSIZE+97)
#define FN_push         (MN_OPSIZE+98)
#define FN_putc         (MN_OPSIZE+99)
#define FN_qualifier    (MN_OPSIZE+100)
#define FN_query        (MN_OPSIZE+101)
#define FN_random       (MN_OPSIZE+102)
#define FN_randomSeed   (MN_OPSIZE+103)
#define FN_randomseed   (MN_OPSIZE+103)
#define FN_re           (MN_OPSIZE+104)
#define FN_read         (MN_OPSIZE+105)
#define FN_readLine     (MN_OPSIZE+106)
#define FN_readline     (MN_OPSIZE+106)
#define FN_release      (MN_OPSIZE+107)
#define FN_remove       (MN_OPSIZE+108)
#define FN_replace      (MN_OPSIZE+109)
#define FN_s            (MN_OPSIZE+110)
#define FN_seed         (MN_OPSIZE+111)
#define FN_semantic     (MN_OPSIZE+112)
#define FN_shared       (MN_OPSIZE+113)
#define FN_significant  (MN_OPSIZE+114)
#define FN_singleton    (MN_OPSIZE+115)
#define FN_size         (MN_OPSIZE+116)
#define FN_split        (MN_OPSIZE+117)
#define FN_start        (MN_OPSIZE+118)
#define FN_startsWith   (MN_OPSIZE+119)
#define FN_startswith   (MN_OPSIZE+119)
#define FN_static       (MN_OPSIZE+120)
#define FN_status       (MN_OPSIZE+121)
#define FN_storingBuffer (MN_OPSIZE+122)
#define FN_storingbuffer (MN_OPSIZE+122)
#define FN_strict       (MN_OPSIZE+123)
#define FN_string       (MN_OPSIZE+124)
#define FN_substring    (MN_OPSIZE+125)
#define FN_super        (MN_OPSIZE+126)
#define FN_temporal     (MN_OPSIZE+127)
#define FN_this         (MN_OPSIZE+128)
#define FN_time         (MN_OPSIZE+129)
#define FN_total        (MN_OPSIZE+130)
#define FN_trim         (MN_OPSIZE+131)
#define FN_u            (MN_OPSIZE+132)
#define FN_unique       (MN_OPSIZE+133)
#define FN_unlikely     (MN_OPSIZE+134)
#define FN_urn          (MN_OPSIZE+135)
#define FN_v            (MN_OPSIZE+136)
#define FN_value        (MN_OPSIZE+137)
#define FN_vargs        (MN_OPSIZE+138)
#define FN_verbose      (MN_OPSIZE+139)
#define FN_virtual      (MN_OPSIZE+140)
#define FN_write        (MN_OPSIZE+141)
#define FN_writeChar    (MN_OPSIZE+142)
#define FN_writechar    (MN_OPSIZE+142)
#define FN_writeData    (MN_OPSIZE+143)
#define FN_writedata    (MN_OPSIZE+143)
#define FN_x            (MN_OPSIZE+144)
#define K_TFIELD_SIZE   145

/* ======================================================================== */
/* METHODN */
#define MN_exec         FN_exec
#define MN_isUnique     MN_toISBOOL(FN_unique)
#define MN_replace      FN_replace
#define MN_gc           FN_gc
#define MN_setDebug     MN_toSETTER(FN_debug)
#define MN_setAutoFlush MN_toSETTER(FN_autoflush)
#define MN_isConst      MN_toISBOOL(FN_const)
#define MN_isTotal      MN_toISBOOL(FN_total)
#define MN_getErr       MN_toGETTER(FN_err)
#define MN_floatToIntBits FN_floattointbits
#define MN_random       FN_random
#define MN_setProperty  MN_toSETTER(FN_property)
#define MN_add          FN_add
#define MN_path         FN_path
#define MN_isRelease    MN_toISBOOL(FN_release)
#define MN_isNotNull    MN_toISBOOL(FN_notnull)
#define MN_isStatic     MN_toISBOOL(FN_static)
#define MN_read         FN_read
#define MN_writeData    FN_writedata
#define MN__empty       MN_toFMT(FN_empty)
#define MN_split        FN_split
#define MN_likely       FN_likely
#define MN_setModified  MN_toSETTER(FN_modified)
#define MN__bits        MN_toFMT(FN_bits)
#define MN_setErr       MN_toSETTER(FN_err)
#define MN_isNull       MN_toISBOOL(FN_null)
#define MN_getName      MN_toGETTER(FN_name)
#define MN_clearBuffer  FN_clearbuffer
#define MN_flush        FN_flush
#define MN_setIn        MN_toSETTER(FN_in)
#define MN_getChar      MN_toGETTER(FN_char)
#define MN_setShared    MN_toSETTER(FN_shared)
#define MN_getSize      MN_toGETTER(FN_size)
#define MN_isStoringBuffer MN_toISBOOL(FN_storingbuffer)
#define MN_next         FN_next
#define MN_isVirtual    MN_toISBOOL(FN_virtual)
#define MN_main         FN_main
#define MN_indexOf      FN_indexof
#define MN_isPartial    MN_toISBOOL(FN_partial)
#define MN_super        FN_super
#define MN_getKey       MN_toGETTER(FN_key)
#define MN_push         FN_push
#define MN_putc         FN_putc
#define MN_isAutoFlush  MN_toISBOOL(FN_autoflush)
#define MN__x           MN_toFMT(FN_x)
#define MN_             FN_
#define MN__u           MN_toFMT(FN_u)
#define MN__p           MN_toFMT(FN_p)
#define MN__s           MN_toFMT(FN_s)
#define MN_new__LIST    FN_new__list
#define MN__k           MN_toFMT(FN_k)
#define MN__d           MN_toFMT(FN_d)
#define MN__f           MN_toFMT(FN_f)
#define MN_getOut       MN_toGETTER(FN_out)
#define MN__c           MN_toFMT(FN_c)
#define MN_getTime      MN_toGETTER(FN_time)
#define MN_getIn        MN_toGETTER(FN_in)
#define MN_close        FN_close
#define MN_setCharset   MN_toSETTER(FN_charset)
#define MN_isShared     MN_toISBOOL(FN_shared)
#define MN_setOut       MN_toSETTER(FN_out)
#define MN_listProperties FN_listproperties
#define MN_substring    FN_substring
#define MN_isModified   MN_toISBOOL(FN_modified)
#define MN_getFloat     MN_toGETTER(FN_float)
#define MN_readLine     FN_readline
#define MN_defined      FN_defined
#define MN_getProperty  MN_toGETTER(FN_property)
#define MN_isDebug      MN_toISBOOL(FN_debug)
#define MN_isInterface  MN_toISBOOL(FN_interface)
#define MN__dump        MN_toFMT(FN_dump)
#define MN_new__TUPLE   FN_new__tuple
#define MN_invoke       FN_invoke
#define MN_new__ARRAY   FN_new__array
#define MN_lastIndexOf  FN_lastindexof
#define MN_isClosed     MN_toISBOOL(FN_closed)
#define MN_setVerbose   MN_toSETTER(FN_verbose)
#define MN_setAll       MN_toSETTER(FN_all)
#define MN_isFinal      MN_toISBOOL(FN_final)
#define MN_write        FN_write
#define MN_isAbstract   MN_toISBOOL(FN_abstract)
#define MN_setConst     MN_toSETTER(FN_const)
#define MN_new__MAP     FN_new__map
#define MN_getClass     MN_toGETTER(FN_class)
#define MN_query        FN_query
#define MN_println      FN_println
#define MN_intToFloatBits FN_inttofloatbits
#define MN_isASCII      MN_toISBOOL(FN_ascii)
#define MN_delegate     FN_delegate
#define MN_unlikely     FN_unlikely
#define MN_isStrict     MN_toISBOOL(FN_strict)
#define MN_exit         FN_exit
#define MN_domain       FN_domain
#define MN_convert      FN_convert
#define MN_new__RANGE   FN_new__range
#define MN_isPrivate    MN_toISBOOL(FN_private)
#define MN_get          MN_toGETTER(FN_)
#define MN_trim         FN_trim
#define MN_isLogging    MN_toISBOOL(FN_logging)
#define MN_print        FN_print
#define MN_endsWith     FN_endswith
#define MN_new          FN_new
#define MN_set          MN_toSETTER(FN_)
#define MN_isTemporal   MN_toISBOOL(FN_temporal)
#define MN_isSemantic   MN_toISBOOL(FN_semantic)
#define MN_setStrict    MN_toSETTER(FN_strict)
#define MN_isMetaClass  MN_toISBOOL(FN_metaclass)
#define MN_remove       FN_remove
#define MN_this         FN_this
#define MN_encode       FN_encode
#define MN_hashCode     FN_hashcode
#define MN_equals       FN_equals
#define MN_isImmutable  MN_toISBOOL(FN_immutable)
#define MN__data        MN_toFMT(FN_data)
#define MN_writeChar    FN_writechar
#define MN_startsWith   FN_startswith
#define MN_isInteractive MN_toISBOOL(FN_interactive)
#define MN_getInt       MN_toGETTER(FN_int)
#define MN_setStoringBuffer MN_toSETTER(FN_storingbuffer)
#define MN_format       FN_format
#define MN__man         MN_toFMT(FN_man)
#define MN_setRandomSeed MN_toSETTER(FN_randomseed)
#define MN_isSignificant MN_toISBOOL(FN_significant)
#define MN_isPublic     MN_toISBOOL(FN_public)
#define MN_getString    MN_toGETTER(FN_string)
#define MN_isSingleton  MN_toISBOOL(FN_singleton)
#define MN_isVerbose    MN_toISBOOL(FN_verbose)
#define MN_decode       FN_decode
#define MN_isLossLess   MN_toISBOOL(FN_lossless)

/* ======================================================================== */
/* TS */
#define TS_EMPTY        (ctx->share)->tString[0]
#define TS_TAB          (ctx->share)->tString[1]
#define TS_ONE          (ctx->share)->tString[2]
#define TS_EOL          (ctx->share)->tString[3]
#define TS_BEGIN        (ctx->share)->tString[4]
#define TS_END          (ctx->share)->tString[5]
#define TS_true         (ctx->share)->tString[6]
#define TS_false        (ctx->share)->tString[7]
#define TS_null         (ctx->share)->tString[8]
#define TS_void         (ctx->share)->tString[9]
#define TS_main         (ctx->share)->tString[10]
#define TS_ENCODING     (ctx->share)->tString[11]
#define TS_DEVNULL      (ctx->share)->tString[12]
#define TS_DEVSTDIN     (ctx->share)->tString[13]
#define TS_DEVSTDERR    (ctx->share)->tString[14]
#define TS_DEVSTDOUT    (ctx->share)->tString[15]
#define TS_ATlabel      (ctx->share)->tString[16]
#define TS_AssertionException (ctx->share)->tString[17]
#define TS_LP           (ctx->share)->tString[18]
#define TS_RP           (ctx->share)->tString[19]
#define TS_LB           (ctx->share)->tString[20]
#define TS_RB           (ctx->share)->tString[21]
#define TS_LS           (ctx->share)->tString[22]
#define TS_RS           (ctx->share)->tString[23]
#define K_TSTRING_SIZE  24
