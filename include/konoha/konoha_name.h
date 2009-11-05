// THIS FILE WAS AUTOMATICALLY GENERATED

/* ======================================================================== */
/* MACROS */
#define KONOHA_REVISION                 949
#define KONOHA_BUILDID                  754
#define KONOHA_EXPIRE                   1257720708LL

/* ======================================================================== */
/* STRUCT */

/* ------------------------------------------------------------------------ */
/* Object */
#define CLASS_Object            ((knh_class_t)0)
#define STRUCT_Object           ((knh_struct_t)0)
#define IS_Object(o)            ((o)->h.cid == CLASS_Object)
#define TYPE_Object             CLASS_Object
#define NNTYPE_Object           NNTYPE_cid(CLASS_Object)
#define NATYPE_Object           NATYPE_cid(CLASS_Object)
#define CFLAG_Object            ((knh_flag_t)0)
#define FLAG_Object             knh_flag_oflag(CFLAG_Object)

/* ------------------------------------------------------------------------ */
/* Boolean */
#define CLASS_Boolean           ((knh_class_t)1)
#define STRUCT_Boolean          ((knh_struct_t)1)
#define IS_Boolean(o)           ((o)->h.cid == CLASS_Boolean)
#define IS_bBoolean(o)          ((o)->h.bcid == CLASS_Boolean)
#define TYPE_Boolean            CLASS_Boolean
#define NNTYPE_Boolean          NNTYPE_cid(CLASS_Boolean)
#define NATYPE_Boolean          NATYPE_cid(CLASS_Boolean)
#define CFLAG_Boolean           ((knh_flag_t)FLAG_Class_Immutable)
#define FLAG_Boolean            knh_flag_oflag(CFLAG_Boolean)

/* ------------------------------------------------------------------------ */
/* Number */
#define CLASS_Number            ((knh_class_t)2)
#define STRUCT_Number           ((knh_struct_t)2)
#define IS_Number(o)            ((o)->h.cid == CLASS_Number)
#define IS_bNumber(o)           ((o)->h.bcid == CLASS_Number)
#define TYPE_Number             CLASS_Number
#define NNTYPE_Number           NNTYPE_cid(CLASS_Number)
#define NATYPE_Number           NATYPE_cid(CLASS_Number)
#define CFLAG_Number            ((knh_flag_t)0)
#define FLAG_Number             knh_flag_oflag(CFLAG_Number)

/* ------------------------------------------------------------------------ */
/* Int */
#define CLASS_Int               ((knh_class_t)3)
#define STRUCT_Int              ((knh_struct_t)3)
#define IS_Int(o)               ((o)->h.cid == CLASS_Int)
#define IS_bInt(o)              ((o)->h.bcid == CLASS_Int)
#define TYPE_Int                CLASS_Int
#define NNTYPE_Int              NNTYPE_cid(CLASS_Int)
#define NATYPE_Int              NATYPE_cid(CLASS_Int)
#define CFLAG_Int               ((knh_flag_t)FLAG_Class_Immutable)
#define FLAG_Int                knh_flag_oflag(CFLAG_Int)

/* ------------------------------------------------------------------------ */
/* Float */
#define CLASS_Float             ((knh_class_t)4)
#define STRUCT_Float            ((knh_struct_t)4)
#define IS_Float(o)             ((o)->h.cid == CLASS_Float)
#define IS_bFloat(o)            ((o)->h.bcid == CLASS_Float)
#define TYPE_Float              CLASS_Float
#define NNTYPE_Float            NNTYPE_cid(CLASS_Float)
#define NATYPE_Float            NATYPE_cid(CLASS_Float)
#define CFLAG_Float             ((knh_flag_t)FLAG_Class_Immutable)
#define FLAG_Float              knh_flag_oflag(CFLAG_Float)

/* ------------------------------------------------------------------------ */
/* String */
#define CLASS_String            ((knh_class_t)5)
#define STRUCT_String           ((knh_struct_t)5)
#define IS_String(o)            ((o)->h.cid == CLASS_String)
#define IS_bString(o)           ((o)->h.bcid == CLASS_String)
#define TYPE_String             CLASS_String
#define NNTYPE_String           NNTYPE_cid(CLASS_String)
#define NATYPE_String           NATYPE_cid(CLASS_String)
#define CFLAG_String            ((knh_flag_t)FLAG_Class_Immutable)
#define FLAG_String             knh_flag_oflag(CFLAG_String)

/* ------------------------------------------------------------------------ */
/* Bytes */
#define CLASS_Bytes             ((knh_class_t)6)
#define STRUCT_Bytes            ((knh_struct_t)6)
#define IS_Bytes(o)             ((o)->h.cid == CLASS_Bytes)
#define IS_bBytes(o)            ((o)->h.bcid == CLASS_Bytes)
#define TYPE_Bytes              CLASS_Bytes
#define NNTYPE_Bytes            NNTYPE_cid(CLASS_Bytes)
#define NATYPE_Bytes            NATYPE_cid(CLASS_Bytes)
#define CFLAG_Bytes             ((knh_flag_t)0)
#define FLAG_Bytes              knh_flag_oflag(CFLAG_Bytes)

/* ------------------------------------------------------------------------ */
/* Any */
#define CLASS_Any               ((knh_class_t)7)
#define STRUCT_Any              ((knh_struct_t)7)
#define IS_Any(o)               ((o)->h.cid == CLASS_Any)
#define IS_bAny(o)              ((o)->h.bcid == CLASS_Any)
#define TYPE_Any                NATYPE_cid(CLASS_Any)
#define NNTYPE_Any              NATYPE_cid(CLASS_Any)
#define NATYPE_Any              NATYPE_cid(CLASS_Any)
#define CFLAG_Any               ((knh_flag_t)0)
#define FLAG_Any                knh_flag_oflag(CFLAG_Any)

/* ------------------------------------------------------------------------ */
/* Iterator */
#define CLASS_Iterator          ((knh_class_t)8)
#define STRUCT_Iterator         ((knh_struct_t)8)
#define IS_Iterator(o)          ((o)->h.cid == CLASS_Iterator)
#define IS_bIterator(o)         ((o)->h.bcid == CLASS_Iterator)
#define TYPE_Iterator           CLASS_Iterator
#define NNTYPE_Iterator         NNTYPE_cid(CLASS_Iterator)
#define NATYPE_Iterator         NATYPE_cid(CLASS_Iterator)
#define CFLAG_Iterator          ((knh_flag_t)FLAG_Class_Immutable)
#define FLAG_Iterator           knh_flag_oflag(CFLAG_Iterator)

/* ------------------------------------------------------------------------ */
/* Pair */
#define CLASS_Pair              ((knh_class_t)9)
#define STRUCT_Pair             ((knh_struct_t)9)
#define IS_Pair(o)              ((o)->h.cid == CLASS_Pair)
#define IS_bPair(o)             ((o)->h.bcid == CLASS_Pair)
#define TYPE_Pair               CLASS_Pair
#define NNTYPE_Pair             NNTYPE_cid(CLASS_Pair)
#define NATYPE_Pair             NATYPE_cid(CLASS_Pair)
#define CFLAG_Pair              ((knh_flag_t)FLAG_Class_Immutable)
#define FLAG_Pair               knh_flag_oflag(CFLAG_Pair)

/* ------------------------------------------------------------------------ */
/* Tuple */
#define CLASS_Tuple             ((knh_class_t)10)
#define STRUCT_Tuple            ((knh_struct_t)10)
#define IS_Tuple(o)             ((o)->h.cid == CLASS_Tuple)
#define IS_bTuple(o)            ((o)->h.bcid == CLASS_Tuple)
#define TYPE_Tuple              CLASS_Tuple
#define NNTYPE_Tuple            NNTYPE_cid(CLASS_Tuple)
#define NATYPE_Tuple            NATYPE_cid(CLASS_Tuple)
#define CFLAG_Tuple             ((knh_flag_t)FLAG_Class_Immutable)
#define FLAG_Tuple              knh_flag_oflag(CFLAG_Tuple)

/* ------------------------------------------------------------------------ */
/* Range */
#define CLASS_Range             ((knh_class_t)11)
#define STRUCT_Range            ((knh_struct_t)11)
#define IS_Range(o)             ((o)->h.cid == CLASS_Range)
#define IS_bRange(o)            ((o)->h.bcid == CLASS_Range)
#define TYPE_Range              CLASS_Range
#define NNTYPE_Range            NNTYPE_cid(CLASS_Range)
#define NATYPE_Range            NATYPE_cid(CLASS_Range)
#define CFLAG_Range             ((knh_flag_t)FLAG_Class_Immutable)
#define FLAG_Range              knh_flag_oflag(CFLAG_Range)

/* ------------------------------------------------------------------------ */
/* Array */
#define CLASS_Array             ((knh_class_t)12)
#define STRUCT_Array            ((knh_struct_t)12)
#define IS_Array(o)             ((o)->h.cid == CLASS_Array)
#define IS_bArray(o)            ((o)->h.bcid == CLASS_Array)
#define TYPE_Array              CLASS_Array
#define NNTYPE_Array            NNTYPE_cid(CLASS_Array)
#define NATYPE_Array            NATYPE_cid(CLASS_Array)
#define CFLAG_Array             ((knh_flag_t)0)
#define FLAG_Array              knh_flag_oflag(CFLAG_Array)

/* ------------------------------------------------------------------------ */
/* IArray */
#define CLASS_IArray            ((knh_class_t)13)
#define STRUCT_IArray           ((knh_struct_t)13)
#define IS_IArray(o)            ((o)->h.cid == CLASS_IArray)
#define IS_bIArray(o)           ((o)->h.bcid == CLASS_IArray)
#define TYPE_IArray             CLASS_IArray
#define NNTYPE_IArray           NNTYPE_cid(CLASS_IArray)
#define NATYPE_IArray           NATYPE_cid(CLASS_IArray)
#define CFLAG_IArray            ((knh_flag_t)0)
#define FLAG_IArray             knh_flag_oflag(CFLAG_IArray)

/* ------------------------------------------------------------------------ */
/* FArray */
#define CLASS_FArray            ((knh_class_t)14)
#define STRUCT_FArray           ((knh_struct_t)14)
#define IS_FArray(o)            ((o)->h.cid == CLASS_FArray)
#define IS_bFArray(o)           ((o)->h.bcid == CLASS_FArray)
#define TYPE_FArray             CLASS_FArray
#define NNTYPE_FArray           NNTYPE_cid(CLASS_FArray)
#define NATYPE_FArray           NATYPE_cid(CLASS_FArray)
#define CFLAG_FArray            ((knh_flag_t)0)
#define FLAG_FArray             knh_flag_oflag(CFLAG_FArray)

/* ------------------------------------------------------------------------ */
/* DictMap */
#define CLASS_DictMap           ((knh_class_t)15)
#define STRUCT_DictMap          ((knh_struct_t)15)
#define IS_DictMap(o)           ((o)->h.cid == CLASS_DictMap)
#define IS_bDictMap(o)          ((o)->h.bcid == CLASS_DictMap)
#define TYPE_DictMap            CLASS_DictMap
#define NNTYPE_DictMap          NNTYPE_cid(CLASS_DictMap)
#define NATYPE_DictMap          NATYPE_cid(CLASS_DictMap)
#define CFLAG_DictMap           ((knh_flag_t)0)
#define FLAG_DictMap            knh_flag_oflag(CFLAG_DictMap)

/* ------------------------------------------------------------------------ */
/* DictSet */
#define CLASS_DictSet           ((knh_class_t)16)
#define STRUCT_DictSet          ((knh_struct_t)16)
#define IS_DictSet(o)           ((o)->h.cid == CLASS_DictSet)
#define IS_bDictSet(o)          ((o)->h.bcid == CLASS_DictSet)
#define TYPE_DictSet            CLASS_DictSet
#define NNTYPE_DictSet          NNTYPE_cid(CLASS_DictSet)
#define NATYPE_DictSet          NATYPE_cid(CLASS_DictSet)
#define CFLAG_DictSet           ((knh_flag_t)0)
#define FLAG_DictSet            knh_flag_oflag(CFLAG_DictSet)

/* ------------------------------------------------------------------------ */
/* HashMap */
#define CLASS_HashMap           ((knh_class_t)17)
#define STRUCT_HashMap          ((knh_struct_t)17)
#define IS_HashMap(o)           ((o)->h.cid == CLASS_HashMap)
#define IS_bHashMap(o)          ((o)->h.bcid == CLASS_HashMap)
#define TYPE_HashMap            CLASS_HashMap
#define NNTYPE_HashMap          NNTYPE_cid(CLASS_HashMap)
#define NATYPE_HashMap          NATYPE_cid(CLASS_HashMap)
#define CFLAG_HashMap           ((knh_flag_t)0)
#define FLAG_HashMap            knh_flag_oflag(CFLAG_HashMap)

/* ------------------------------------------------------------------------ */
/* HashSet */
#define CLASS_HashSet           ((knh_class_t)18)
#define STRUCT_HashSet          ((knh_struct_t)18)
#define IS_HashSet(o)           ((o)->h.cid == CLASS_HashSet)
#define IS_bHashSet(o)          ((o)->h.bcid == CLASS_HashSet)
#define TYPE_HashSet            CLASS_HashSet
#define NNTYPE_HashSet          NNTYPE_cid(CLASS_HashSet)
#define NATYPE_HashSet          NATYPE_cid(CLASS_HashSet)
#define CFLAG_HashSet           ((knh_flag_t)0)
#define FLAG_HashSet            knh_flag_oflag(CFLAG_HashSet)

/* ------------------------------------------------------------------------ */
/* DictIdx */
#define CLASS_DictIdx           ((knh_class_t)19)
#define STRUCT_DictIdx          ((knh_struct_t)19)
#define IS_DictIdx(o)           ((o)->h.cid == CLASS_DictIdx)
#define IS_bDictIdx(o)          ((o)->h.bcid == CLASS_DictIdx)
#define TYPE_DictIdx            CLASS_DictIdx
#define NNTYPE_DictIdx          NNTYPE_cid(CLASS_DictIdx)
#define NATYPE_DictIdx          NATYPE_cid(CLASS_DictIdx)
#define CFLAG_DictIdx           ((knh_flag_t)FLAG_Class_Private)
#define FLAG_DictIdx            knh_flag_oflag(CFLAG_DictIdx)

/* ------------------------------------------------------------------------ */
/* Class */
#define CLASS_Class             ((knh_class_t)20)
#define STRUCT_Class            ((knh_struct_t)20)
#define IS_Class(o)             ((o)->h.cid == CLASS_Class)
#define IS_bClass(o)            ((o)->h.bcid == CLASS_Class)
#define TYPE_Class              CLASS_Class
#define NNTYPE_Class            NNTYPE_cid(CLASS_Class)
#define NATYPE_Class            NATYPE_cid(CLASS_Class)
#define CFLAG_Class             ((knh_flag_t)FLAG_Class_Immutable)
#define FLAG_Class              knh_flag_oflag(CFLAG_Class)

/* ------------------------------------------------------------------------ */
/* ClassField */
#define CLASS_ClassField        ((knh_class_t)21)
#define STRUCT_ClassField       ((knh_struct_t)21)
#define IS_ClassField(o)        ((o)->h.cid == CLASS_ClassField)
#define IS_bClassField(o)       ((o)->h.bcid == CLASS_ClassField)
#define TYPE_ClassField         CLASS_ClassField
#define NNTYPE_ClassField       NNTYPE_cid(CLASS_ClassField)
#define NATYPE_ClassField       NATYPE_cid(CLASS_ClassField)
#define CFLAG_ClassField        ((knh_flag_t)FLAG_Class_Private)
#define FLAG_ClassField         knh_flag_oflag(CFLAG_ClassField)

/* ------------------------------------------------------------------------ */
/* MethodField */
#define CLASS_MethodField       ((knh_class_t)22)
#define STRUCT_MethodField      ((knh_struct_t)22)
#define IS_MethodField(o)       ((o)->h.cid == CLASS_MethodField)
#define IS_bMethodField(o)      ((o)->h.bcid == CLASS_MethodField)
#define TYPE_MethodField        CLASS_MethodField
#define NNTYPE_MethodField      NNTYPE_cid(CLASS_MethodField)
#define NATYPE_MethodField      NATYPE_cid(CLASS_MethodField)
#define CFLAG_MethodField       ((knh_flag_t)FLAG_Class_Private)
#define FLAG_MethodField        knh_flag_oflag(CFLAG_MethodField)

/* ------------------------------------------------------------------------ */
/* Method */
#define CLASS_Method            ((knh_class_t)23)
#define STRUCT_Method           ((knh_struct_t)23)
#define IS_Method(o)            ((o)->h.cid == CLASS_Method)
#define IS_bMethod(o)           ((o)->h.bcid == CLASS_Method)
#define TYPE_Method             CLASS_Method
#define NNTYPE_Method           NNTYPE_cid(CLASS_Method)
#define NATYPE_Method           NATYPE_cid(CLASS_Method)
#define CFLAG_Method            ((knh_flag_t)0)
#define FLAG_Method             knh_flag_oflag(CFLAG_Method)

/* ------------------------------------------------------------------------ */
/* Mapper */
#define CLASS_Mapper            ((knh_class_t)24)
#define STRUCT_Mapper           ((knh_struct_t)24)
#define IS_Mapper(o)            ((o)->h.cid == CLASS_Mapper)
#define IS_bMapper(o)           ((o)->h.bcid == CLASS_Mapper)
#define TYPE_Mapper             CLASS_Mapper
#define NNTYPE_Mapper           NNTYPE_cid(CLASS_Mapper)
#define NATYPE_Mapper           NATYPE_cid(CLASS_Mapper)
#define CFLAG_Mapper            ((knh_flag_t)0)
#define FLAG_Mapper             knh_flag_oflag(CFLAG_Mapper)

/* ------------------------------------------------------------------------ */
/* ClassMap */
#define CLASS_ClassMap          ((knh_class_t)25)
#define STRUCT_ClassMap         ((knh_struct_t)25)
#define IS_ClassMap(o)          ((o)->h.cid == CLASS_ClassMap)
#define IS_bClassMap(o)         ((o)->h.bcid == CLASS_ClassMap)
#define TYPE_ClassMap           CLASS_ClassMap
#define NNTYPE_ClassMap         NNTYPE_cid(CLASS_ClassMap)
#define NATYPE_ClassMap         NATYPE_cid(CLASS_ClassMap)
#define CFLAG_ClassMap          ((knh_flag_t)FLAG_Class_Private)
#define FLAG_ClassMap           knh_flag_oflag(CFLAG_ClassMap)

/* ------------------------------------------------------------------------ */
/* Closure */
#define CLASS_Closure           ((knh_class_t)26)
#define STRUCT_Closure          ((knh_struct_t)26)
#define IS_Closure(o)           ((o)->h.cid == CLASS_Closure)
#define IS_bClosure(o)          ((o)->h.bcid == CLASS_Closure)
#define TYPE_Closure            CLASS_Closure
#define NNTYPE_Closure          NNTYPE_cid(CLASS_Closure)
#define NATYPE_Closure          NATYPE_cid(CLASS_Closure)
#define CFLAG_Closure           ((knh_flag_t)0)
#define FLAG_Closure            knh_flag_oflag(CFLAG_Closure)

/* ------------------------------------------------------------------------ */
/* Thunk */
#define CLASS_Thunk             ((knh_class_t)27)
#define STRUCT_Thunk            ((knh_struct_t)27)
#define IS_Thunk(o)             ((o)->h.cid == CLASS_Thunk)
#define IS_bThunk(o)            ((o)->h.bcid == CLASS_Thunk)
#define TYPE_Thunk              CLASS_Thunk
#define NNTYPE_Thunk            NNTYPE_cid(CLASS_Thunk)
#define NATYPE_Thunk            NATYPE_cid(CLASS_Thunk)
#define CFLAG_Thunk             ((knh_flag_t)0)
#define FLAG_Thunk              knh_flag_oflag(CFLAG_Thunk)

/* ------------------------------------------------------------------------ */
/* AffineConv */
#define CLASS_AffineConv        ((knh_class_t)28)
#define STRUCT_AffineConv       ((knh_struct_t)28)
#define IS_AffineConv(o)        ((o)->h.cid == CLASS_AffineConv)
#define IS_bAffineConv(o)       ((o)->h.bcid == CLASS_AffineConv)
#define TYPE_AffineConv         CLASS_AffineConv
#define NNTYPE_AffineConv       NNTYPE_cid(CLASS_AffineConv)
#define NATYPE_AffineConv       NATYPE_cid(CLASS_AffineConv)
#define CFLAG_AffineConv        ((knh_flag_t)FLAG_Class_Immutable|FLAG_Class_Private)
#define FLAG_AffineConv         knh_flag_oflag(CFLAG_AffineConv)

/* ------------------------------------------------------------------------ */
/* Regex */
#define CLASS_Regex             ((knh_class_t)29)
#define STRUCT_Regex            ((knh_struct_t)29)
#define IS_Regex(o)             ((o)->h.cid == CLASS_Regex)
#define IS_bRegex(o)            ((o)->h.bcid == CLASS_Regex)
#define TYPE_Regex              CLASS_Regex
#define NNTYPE_Regex            NNTYPE_cid(CLASS_Regex)
#define NATYPE_Regex            NATYPE_cid(CLASS_Regex)
#define CFLAG_Regex             ((knh_flag_t)FLAG_Class_Immutable)
#define FLAG_Regex              knh_flag_oflag(CFLAG_Regex)

/* ------------------------------------------------------------------------ */
/* BytesConv */
#define CLASS_BytesConv         ((knh_class_t)30)
#define STRUCT_BytesConv        ((knh_struct_t)30)
#define IS_BytesConv(o)         ((o)->h.cid == CLASS_BytesConv)
#define IS_bBytesConv(o)        ((o)->h.bcid == CLASS_BytesConv)
#define TYPE_BytesConv          CLASS_BytesConv
#define NNTYPE_BytesConv        NNTYPE_cid(CLASS_BytesConv)
#define NATYPE_BytesConv        NATYPE_cid(CLASS_BytesConv)
#define CFLAG_BytesConv         ((knh_flag_t)FLAG_Class_Private)
#define FLAG_BytesConv          knh_flag_oflag(CFLAG_BytesConv)

/* ------------------------------------------------------------------------ */
/* ClassSpec */
#define CLASS_ClassSpec         ((knh_class_t)31)
#define STRUCT_ClassSpec        ((knh_struct_t)31)
#define IS_ClassSpec(o)         ((o)->h.cid == CLASS_ClassSpec)
#define IS_bClassSpec(o)        ((o)->h.bcid == CLASS_ClassSpec)
#define TYPE_ClassSpec          CLASS_ClassSpec
#define NNTYPE_ClassSpec        NNTYPE_cid(CLASS_ClassSpec)
#define NATYPE_ClassSpec        NATYPE_cid(CLASS_ClassSpec)
#define CFLAG_ClassSpec         ((knh_flag_t)FLAG_Class_Private)
#define FLAG_ClassSpec          knh_flag_oflag(CFLAG_ClassSpec)

/* ------------------------------------------------------------------------ */
/* InputStream */
#define CLASS_InputStream       ((knh_class_t)32)
#define STRUCT_InputStream      ((knh_struct_t)32)
#define IS_InputStream(o)       ((o)->h.cid == CLASS_InputStream)
#define IS_bInputStream(o)      ((o)->h.bcid == CLASS_InputStream)
#define TYPE_InputStream        CLASS_InputStream
#define NNTYPE_InputStream      NNTYPE_cid(CLASS_InputStream)
#define NATYPE_InputStream      NATYPE_cid(CLASS_InputStream)
#define CFLAG_InputStream       ((knh_flag_t)0)
#define FLAG_InputStream        knh_flag_oflag(CFLAG_InputStream)

/* ------------------------------------------------------------------------ */
/* OutputStream */
#define CLASS_OutputStream      ((knh_class_t)33)
#define STRUCT_OutputStream     ((knh_struct_t)33)
#define IS_OutputStream(o)      ((o)->h.cid == CLASS_OutputStream)
#define IS_bOutputStream(o)     ((o)->h.bcid == CLASS_OutputStream)
#define TYPE_OutputStream       CLASS_OutputStream
#define NNTYPE_OutputStream     NNTYPE_cid(CLASS_OutputStream)
#define NATYPE_OutputStream     NATYPE_cid(CLASS_OutputStream)
#define CFLAG_OutputStream      ((knh_flag_t)0)
#define FLAG_OutputStream       knh_flag_oflag(CFLAG_OutputStream)

/* ------------------------------------------------------------------------ */
/* Socket */
#define CLASS_Socket            ((knh_class_t)34)
#define STRUCT_Socket           ((knh_struct_t)34)
#define IS_Socket(o)            ((o)->h.cid == CLASS_Socket)
#define IS_bSocket(o)           ((o)->h.bcid == CLASS_Socket)
#define TYPE_Socket             CLASS_Socket
#define NNTYPE_Socket           NNTYPE_cid(CLASS_Socket)
#define NATYPE_Socket           NATYPE_cid(CLASS_Socket)
#define CFLAG_Socket            ((knh_flag_t)0)
#define FLAG_Socket             knh_flag_oflag(CFLAG_Socket)

/* ------------------------------------------------------------------------ */
/* Connection */
#define CLASS_Connection        ((knh_class_t)35)
#define STRUCT_Connection       ((knh_struct_t)35)
#define IS_Connection(o)        ((o)->h.cid == CLASS_Connection)
#define IS_bConnection(o)       ((o)->h.bcid == CLASS_Connection)
#define TYPE_Connection         CLASS_Connection
#define NNTYPE_Connection       NNTYPE_cid(CLASS_Connection)
#define NATYPE_Connection       NATYPE_cid(CLASS_Connection)
#define CFLAG_Connection        ((knh_flag_t)0)
#define FLAG_Connection         knh_flag_oflag(CFLAG_Connection)

/* ------------------------------------------------------------------------ */
/* ResultSet */
#define CLASS_ResultSet         ((knh_class_t)36)
#define STRUCT_ResultSet        ((knh_struct_t)36)
#define IS_ResultSet(o)         ((o)->h.cid == CLASS_ResultSet)
#define IS_bResultSet(o)        ((o)->h.bcid == CLASS_ResultSet)
#define TYPE_ResultSet          CLASS_ResultSet
#define NNTYPE_ResultSet        NNTYPE_cid(CLASS_ResultSet)
#define NATYPE_ResultSet        NATYPE_cid(CLASS_ResultSet)
#define CFLAG_ResultSet         ((knh_flag_t)0)
#define FLAG_ResultSet          knh_flag_oflag(CFLAG_ResultSet)

/* ------------------------------------------------------------------------ */
/* Exception */
#define CLASS_Exception         ((knh_class_t)37)
#define STRUCT_Exception        ((knh_struct_t)37)
#define IS_Exception(o)         ((o)->h.cid == CLASS_Exception)
#define IS_bException(o)        ((o)->h.bcid == CLASS_Exception)
#define TYPE_Exception          CLASS_Exception
#define NNTYPE_Exception        NNTYPE_cid(CLASS_Exception)
#define NATYPE_Exception        NATYPE_cid(CLASS_Exception)
#define CFLAG_Exception         ((knh_flag_t)0)
#define FLAG_Exception          knh_flag_oflag(CFLAG_Exception)

/* ------------------------------------------------------------------------ */
/* ExceptionHandler */
#define CLASS_ExceptionHandler  ((knh_class_t)38)
#define STRUCT_ExceptionHandler ((knh_struct_t)38)
#define IS_ExceptionHandler(o)  ((o)->h.cid == CLASS_ExceptionHandler)
#define IS_bExceptionHandler(o) ((o)->h.bcid == CLASS_ExceptionHandler)
#define TYPE_ExceptionHandler   CLASS_ExceptionHandler
#define NNTYPE_ExceptionHandler NNTYPE_cid(CLASS_ExceptionHandler)
#define NATYPE_ExceptionHandler NATYPE_cid(CLASS_ExceptionHandler)
#define CFLAG_ExceptionHandler  ((knh_flag_t)FLAG_Class_Private)
#define FLAG_ExceptionHandler   knh_flag_oflag(CFLAG_ExceptionHandler)

/* ------------------------------------------------------------------------ */
/* Script */
#define CLASS_Script            ((knh_class_t)39)
#define STRUCT_Script           ((knh_struct_t)39)
#define IS_Script(o)            ((o)->h.cid == CLASS_Script)
#define IS_bScript(o)           ((o)->h.bcid == CLASS_Script)
#define TYPE_Script             CLASS_Script
#define NNTYPE_Script           NNTYPE_cid(CLASS_Script)
#define NATYPE_Script           NATYPE_cid(CLASS_Script)
#define CFLAG_Script            ((knh_flag_t)FLAG_Class_Singleton|FLAG_Class_Private)
#define FLAG_Script             knh_flag_oflag(CFLAG_Script)

/* ------------------------------------------------------------------------ */
/* NameSpace */
#define CLASS_NameSpace         ((knh_class_t)40)
#define STRUCT_NameSpace        ((knh_struct_t)40)
#define IS_NameSpace(o)         ((o)->h.cid == CLASS_NameSpace)
#define IS_bNameSpace(o)        ((o)->h.bcid == CLASS_NameSpace)
#define TYPE_NameSpace          CLASS_NameSpace
#define NNTYPE_NameSpace        NNTYPE_cid(CLASS_NameSpace)
#define NATYPE_NameSpace        NATYPE_cid(CLASS_NameSpace)
#define CFLAG_NameSpace         ((knh_flag_t)0)
#define FLAG_NameSpace          knh_flag_oflag(CFLAG_NameSpace)

/* ------------------------------------------------------------------------ */
/* System */
#define CLASS_System            ((knh_class_t)41)
#define STRUCT_System           ((knh_struct_t)41)
#define IS_System(o)            ((o)->h.cid == CLASS_System)
#define IS_bSystem(o)           ((o)->h.bcid == CLASS_System)
#define TYPE_System             CLASS_System
#define NNTYPE_System           NNTYPE_cid(CLASS_System)
#define NATYPE_System           NATYPE_cid(CLASS_System)
#define CFLAG_System            ((knh_flag_t)FLAG_Class_Singleton)
#define FLAG_System             knh_flag_oflag(CFLAG_System)

/* ------------------------------------------------------------------------ */
/* Context */
#define CLASS_Context           ((knh_class_t)42)
#define STRUCT_Context          ((knh_struct_t)42)
#define IS_Context(o)           ((o)->h.cid == CLASS_Context)
#define IS_bContext(o)          ((o)->h.bcid == CLASS_Context)
#define TYPE_Context            CLASS_Context
#define NNTYPE_Context          NNTYPE_cid(CLASS_Context)
#define NATYPE_Context          NATYPE_cid(CLASS_Context)
#define CFLAG_Context           ((knh_flag_t)0)
#define FLAG_Context            knh_flag_oflag(CFLAG_Context)

/* ------------------------------------------------------------------------ */
/* Token */
#define CLASS_Token             ((knh_class_t)43)
#define STRUCT_Token            ((knh_struct_t)43)
#define IS_Token(o)             ((o)->h.cid == CLASS_Token)
#define IS_bToken(o)            ((o)->h.bcid == CLASS_Token)
#define TYPE_Token              CLASS_Token
#define NNTYPE_Token            NNTYPE_cid(CLASS_Token)
#define NATYPE_Token            NATYPE_cid(CLASS_Token)
#define CFLAG_Token             ((knh_flag_t)0)
#define FLAG_Token              knh_flag_oflag(CFLAG_Token)

/* ------------------------------------------------------------------------ */
/* Stmt */
#define CLASS_Stmt              ((knh_class_t)44)
#define STRUCT_Stmt             ((knh_struct_t)44)
#define IS_Stmt(o)              ((o)->h.cid == CLASS_Stmt)
#define IS_bStmt(o)             ((o)->h.bcid == CLASS_Stmt)
#define TYPE_Stmt               CLASS_Stmt
#define NNTYPE_Stmt             NNTYPE_cid(CLASS_Stmt)
#define NATYPE_Stmt             NATYPE_cid(CLASS_Stmt)
#define CFLAG_Stmt              ((knh_flag_t)0)
#define FLAG_Stmt               knh_flag_oflag(CFLAG_Stmt)

/* ------------------------------------------------------------------------ */
/* Gamma */
#define CLASS_Gamma             ((knh_class_t)45)
#define STRUCT_Gamma            ((knh_struct_t)45)
#define IS_Gamma(o)             ((o)->h.cid == CLASS_Gamma)
#define IS_bGamma(o)            ((o)->h.bcid == CLASS_Gamma)
#define TYPE_Gamma              CLASS_Gamma
#define NNTYPE_Gamma            NNTYPE_cid(CLASS_Gamma)
#define NATYPE_Gamma            NATYPE_cid(CLASS_Gamma)
#define CFLAG_Gamma             ((knh_flag_t)FLAG_Class_Private)
#define FLAG_Gamma              knh_flag_oflag(CFLAG_Gamma)

/* ------------------------------------------------------------------------ */
/* KLRInst */
#define CLASS_KLRInst           ((knh_class_t)46)
#define STRUCT_KLRInst          ((knh_struct_t)46)
#define IS_KLRInst(o)           ((o)->h.cid == CLASS_KLRInst)
#define IS_bKLRInst(o)          ((o)->h.bcid == CLASS_KLRInst)
#define TYPE_KLRInst            CLASS_KLRInst
#define NNTYPE_KLRInst          NNTYPE_cid(CLASS_KLRInst)
#define NATYPE_KLRInst          NATYPE_cid(CLASS_KLRInst)
#define CFLAG_KLRInst           ((knh_flag_t)0)
#define FLAG_KLRInst            knh_flag_oflag(CFLAG_KLRInst)

/* ------------------------------------------------------------------------ */
/* KLRCode */
#define CLASS_KLRCode           ((knh_class_t)47)
#define STRUCT_KLRCode          ((knh_struct_t)47)
#define IS_KLRCode(o)           ((o)->h.cid == CLASS_KLRCode)
#define IS_bKLRCode(o)          ((o)->h.bcid == CLASS_KLRCode)
#define TYPE_KLRCode            CLASS_KLRCode
#define NNTYPE_KLRCode          NNTYPE_cid(CLASS_KLRCode)
#define NATYPE_KLRCode          NATYPE_cid(CLASS_KLRCode)
#define CFLAG_KLRCode           ((knh_flag_t)0)
#define FLAG_KLRCode            knh_flag_oflag(CFLAG_KLRCode)

/* ======================================================================== */
/* CLASS */

/* ------------------------------------------------------------------------ */
/* This */
#define CLASS_This              ((knh_class_t)KNH_TCLASS_SIZE-(0+1))
#define STRUCT_This             STRUCT_Any
#define IS_This(o)              ((o)->h.cid == CLASS_This)
#define TYPE_This               CLASS_This
#define NNTYPE_This             NNTYPE_cid(CLASS_This)
#define NATYPE_This             NATYPE_cid(CLASS_This)
#define CFLAG_This              ((knh_flag_t)FLAG_Class_TypeVariable)
#define FLAG_This               knh_flag_oflag(CFLAG_This)

/* ------------------------------------------------------------------------ */
/* Tvoid */
#define CLASS_Tvoid             ((knh_class_t)KNH_TCLASS_SIZE-(1+1))
#define STRUCT_Tvoid            STRUCT_Any
#define IS_Tvoid(o)             ((o)->h.cid == CLASS_Tvoid)
#define TYPE_Tvoid              CLASS_Tvoid
#define NNTYPE_Tvoid            NNTYPE_cid(CLASS_Tvoid)
#define NATYPE_Tvoid            NATYPE_cid(CLASS_Tvoid)
#define CFLAG_Tvoid             ((knh_flag_t)FLAG_Class_TypeVariable)
#define FLAG_Tvoid              knh_flag_oflag(CFLAG_Tvoid)

/* ------------------------------------------------------------------------ */
/* T0 */
#define CLASS_T0                ((knh_class_t)KNH_TCLASS_SIZE-(2+1))
#define STRUCT_T0               STRUCT_Any
#define IS_T0(o)                ((o)->h.cid == CLASS_T0)
#define TYPE_T0                 CLASS_T0
#define NNTYPE_T0               NNTYPE_cid(CLASS_T0)
#define NATYPE_T0               NATYPE_cid(CLASS_T0)
#define CFLAG_T0                ((knh_flag_t)FLAG_Class_TypeVariable)
#define FLAG_T0                 knh_flag_oflag(CFLAG_T0)

/* ------------------------------------------------------------------------ */
/* T1 */
#define CLASS_T1                ((knh_class_t)KNH_TCLASS_SIZE-(3+1))
#define STRUCT_T1               STRUCT_Any
#define IS_T1(o)                ((o)->h.cid == CLASS_T1)
#define TYPE_T1                 CLASS_T1
#define NNTYPE_T1               NNTYPE_cid(CLASS_T1)
#define NATYPE_T1               NATYPE_cid(CLASS_T1)
#define CFLAG_T1                ((knh_flag_t)FLAG_Class_TypeVariable)
#define FLAG_T1                 knh_flag_oflag(CFLAG_T1)

/* ------------------------------------------------------------------------ */
/* T2 */
#define CLASS_T2                ((knh_class_t)KNH_TCLASS_SIZE-(4+1))
#define STRUCT_T2               STRUCT_Any
#define IS_T2(o)                ((o)->h.cid == CLASS_T2)
#define TYPE_T2                 CLASS_T2
#define NNTYPE_T2               NNTYPE_cid(CLASS_T2)
#define NATYPE_T2               NATYPE_cid(CLASS_T2)
#define CFLAG_T2                ((knh_flag_t)FLAG_Class_TypeVariable)
#define FLAG_T2                 knh_flag_oflag(CFLAG_T2)

/* ------------------------------------------------------------------------ */
/* T3 */
#define CLASS_T3                ((knh_class_t)KNH_TCLASS_SIZE-(5+1))
#define STRUCT_T3               STRUCT_Any
#define IS_T3(o)                ((o)->h.cid == CLASS_T3)
#define TYPE_T3                 CLASS_T3
#define NNTYPE_T3               NNTYPE_cid(CLASS_T3)
#define NATYPE_T3               NATYPE_cid(CLASS_T3)
#define CFLAG_T3                ((knh_flag_t)FLAG_Class_TypeVariable)
#define FLAG_T3                 knh_flag_oflag(CFLAG_T3)

/* ------------------------------------------------------------------------ */
/* Tx */
#define CLASS_Tx                ((knh_class_t)KNH_TCLASS_SIZE-(6+1))
#define STRUCT_Tx               STRUCT_Any
#define IS_Tx(o)                ((o)->h.cid == CLASS_Tx)
#define TYPE_Tx                 CLASS_Tx
#define NNTYPE_Tx               NNTYPE_cid(CLASS_Tx)
#define NATYPE_Tx               NATYPE_cid(CLASS_Tx)
#define CFLAG_Tx                ((knh_flag_t)FLAG_Class_TypeVariable)
#define FLAG_Tx                 knh_flag_oflag(CFLAG_Tx)

/* ------------------------------------------------------------------------ */
/* Tvar */
#define CLASS_Tvar              ((knh_class_t)KNH_TCLASS_SIZE-(7+1))
#define STRUCT_Tvar             STRUCT_Any
#define IS_Tvar(o)              ((o)->h.cid == CLASS_Tvar)
#define TYPE_Tvar               CLASS_Tvar
#define NNTYPE_Tvar             NNTYPE_cid(CLASS_Tvar)
#define NATYPE_Tvar             NATYPE_cid(CLASS_Tvar)
#define CFLAG_Tvar              ((knh_flag_t)FLAG_Class_TypeVariable)
#define FLAG_Tvar               knh_flag_oflag(CFLAG_Tvar)

/* ------------------------------------------------------------------------ */
/* PairSS */
#define CLASS_PairSS            ((knh_class_t)KNH_TCLASS_SIZE-(8+1))
#define STRUCT_PairSS           STRUCT_Pair
#define IS_PairSS(o)            ((o)->h.cid == CLASS_PairSS)
#define TYPE_PairSS             CLASS_PairSS
#define NNTYPE_PairSS           NNTYPE_cid(CLASS_PairSS)
#define NATYPE_PairSS           NATYPE_cid(CLASS_PairSS)
#define CFLAG_PairSS            ((knh_flag_t)0)
#define FLAG_PairSS             knh_flag_oflag(CFLAG_PairSS)

/* ------------------------------------------------------------------------ */
/* PairST1 */
#define CLASS_PairST1           ((knh_class_t)KNH_TCLASS_SIZE-(9+1))
#define STRUCT_PairST1          STRUCT_Pair
#define IS_PairST1(o)           ((o)->h.cid == CLASS_PairST1)
#define TYPE_PairST1            CLASS_PairST1
#define NNTYPE_PairST1          NNTYPE_cid(CLASS_PairST1)
#define NATYPE_PairST1          NATYPE_cid(CLASS_PairST1)
#define CFLAG_PairST1           ((knh_flag_t)0)
#define FLAG_PairST1            knh_flag_oflag(CFLAG_PairST1)

/* ------------------------------------------------------------------------ */
/* PairT1T2 */
#define CLASS_PairT1T2          ((knh_class_t)KNH_TCLASS_SIZE-(10+1))
#define STRUCT_PairT1T2         STRUCT_Pair
#define IS_PairT1T2(o)          ((o)->h.cid == CLASS_PairT1T2)
#define TYPE_PairT1T2           CLASS_PairT1T2
#define NNTYPE_PairT1T2         NNTYPE_cid(CLASS_PairT1T2)
#define NATYPE_PairT1T2         NATYPE_cid(CLASS_PairT1T2)
#define CFLAG_PairT1T2          ((knh_flag_t)0)
#define FLAG_PairT1T2           knh_flag_oflag(CFLAG_PairT1T2)

/* ------------------------------------------------------------------------ */
/* Cmpr */
#define CLASS_Cmpr              ((knh_class_t)KNH_TCLASS_SIZE-(11+1))
#define STRUCT_Cmpr             STRUCT_Closure
#define IS_Cmpr(o)              ((o)->h.cid == CLASS_Cmpr)
#define TYPE_Cmpr               CLASS_Cmpr
#define NNTYPE_Cmpr             NNTYPE_cid(CLASS_Cmpr)
#define NATYPE_Cmpr             NATYPE_cid(CLASS_Cmpr)
#define CFLAG_Cmpr              ((knh_flag_t)0)
#define FLAG_Cmpr               knh_flag_oflag(CFLAG_Cmpr)

/* ------------------------------------------------------------------------ */
/* String_Itr */
#define CLASS_String_Itr        ((knh_class_t)KNH_TCLASS_SIZE-(12+1))
#define STRUCT_String_Itr       STRUCT_Iterator
#define IS_String_Itr(o)        ((o)->h.cid == CLASS_String_Itr)
#define TYPE_String_Itr         CLASS_String_Itr
#define NNTYPE_String_Itr       NNTYPE_cid(CLASS_String_Itr)
#define NATYPE_String_Itr       NATYPE_cid(CLASS_String_Itr)
#define CFLAG_String_Itr        ((knh_flag_t)0)
#define FLAG_String_Itr         knh_flag_oflag(CFLAG_String_Itr)

/* ------------------------------------------------------------------------ */
/* T1_Itr */
#define CLASS_T1_Itr            ((knh_class_t)KNH_TCLASS_SIZE-(13+1))
#define STRUCT_T1_Itr           STRUCT_Iterator
#define IS_T1_Itr(o)            ((o)->h.cid == CLASS_T1_Itr)
#define TYPE_T1_Itr             CLASS_T1_Itr
#define NNTYPE_T1_Itr           NNTYPE_cid(CLASS_T1_Itr)
#define NATYPE_T1_Itr           NATYPE_cid(CLASS_T1_Itr)
#define CFLAG_T1_Itr            ((knh_flag_t)0)
#define FLAG_T1_Itr             knh_flag_oflag(CFLAG_T1_Itr)

/* ------------------------------------------------------------------------ */
/* PairST1_Itr */
#define CLASS_PairST1_Itr       ((knh_class_t)KNH_TCLASS_SIZE-(14+1))
#define STRUCT_PairST1_Itr      STRUCT_Iterator
#define IS_PairST1_Itr(o)       ((o)->h.cid == CLASS_PairST1_Itr)
#define TYPE_PairST1_Itr        CLASS_PairST1_Itr
#define NNTYPE_PairST1_Itr      NNTYPE_cid(CLASS_PairST1_Itr)
#define NATYPE_PairST1_Itr      NATYPE_cid(CLASS_PairST1_Itr)
#define CFLAG_PairST1_Itr       ((knh_flag_t)0)
#define FLAG_PairST1_Itr        knh_flag_oflag(CFLAG_PairST1_Itr)

/* ------------------------------------------------------------------------ */
/* PairT1T2_Itr */
#define CLASS_PairT1T2_Itr      ((knh_class_t)KNH_TCLASS_SIZE-(15+1))
#define STRUCT_PairT1T2_Itr     STRUCT_Iterator
#define IS_PairT1T2_Itr(o)      ((o)->h.cid == CLASS_PairT1T2_Itr)
#define TYPE_PairT1T2_Itr       CLASS_PairT1T2_Itr
#define NNTYPE_PairT1T2_Itr     NNTYPE_cid(CLASS_PairT1T2_Itr)
#define NATYPE_PairT1T2_Itr     NATYPE_cid(CLASS_PairT1T2_Itr)
#define CFLAG_PairT1T2_Itr      ((knh_flag_t)0)
#define FLAG_PairT1T2_Itr       knh_flag_oflag(CFLAG_PairT1T2_Itr)

/* ------------------------------------------------------------------------ */
/* This_Itr */
#define CLASS_This_Itr          ((knh_class_t)KNH_TCLASS_SIZE-(16+1))
#define STRUCT_This_Itr         STRUCT_Iterator
#define IS_This_Itr(o)          ((o)->h.cid == CLASS_This_Itr)
#define TYPE_This_Itr           CLASS_This_Itr
#define NNTYPE_This_Itr         NNTYPE_cid(CLASS_This_Itr)
#define NATYPE_This_Itr         NATYPE_cid(CLASS_This_Itr)
#define CFLAG_This_Itr          ((knh_flag_t)0)
#define FLAG_This_Itr           knh_flag_oflag(CFLAG_This_Itr)

/* ------------------------------------------------------------------------ */
/* String_Ary */
#define CLASS_String_Ary        ((knh_class_t)KNH_TCLASS_SIZE-(17+1))
#define STRUCT_String_Ary       STRUCT_Array
#define IS_String_Ary(o)        ((o)->h.cid == CLASS_String_Ary)
#define TYPE_String_Ary         CLASS_String_Ary
#define NNTYPE_String_Ary       NNTYPE_cid(CLASS_String_Ary)
#define NATYPE_String_Ary       NATYPE_cid(CLASS_String_Ary)
#define CFLAG_String_Ary        ((knh_flag_t)0)
#define FLAG_String_Ary         knh_flag_oflag(CFLAG_String_Ary)

/* ======================================================================== */
/* FLAG */
#define FLAG_Object_Release             (knh_flag_t)(1<<0)

#define knh_Object_isRelease(o)  (((o)->h.flag & FLAG_Object_Release) == FLAG_Object_Release)

#define knh_Object_isDebug(o)  (((o)->h.flag & FLAG_Object_Release) != FLAG_Object_Release)
#define FLAG_Object_Immutable           (knh_flag_t)(1<<1)

#define knh_Object_isImmutable(o)  (((o)->h.flag & FLAG_Object_Immutable) == FLAG_Object_Immutable)
#define FLAG_Object_Cyclic              (knh_flag_t)(1<<2)

#define knh_Object_isCyclic(o)  (((o)->h.flag & FLAG_Object_Cyclic) == FLAG_Object_Cyclic)

#define knh_Object_setCyclic(o,b) if(b) (o)->h.flag |= FLAG_Object_Cyclic; else (o)->h.flag &= ~(FLAG_Object_Cyclic);
#define FLAG_Object_Undefined           (knh_flag_t)(1<<3)

#define knh_Object_isUndefined(o)  (((o)->h.flag & FLAG_Object_Undefined) == FLAG_Object_Undefined)
#define FLAG_Object_Modified            (knh_flag_t)(1<<4)

#define knh_Object_isModified(o)  (((o)->h.flag & FLAG_Object_Modified) == FLAG_Object_Modified)

#define knh_Object_setModified(o,b) if(b) (o)->h.flag |= FLAG_Object_Modified; else (o)->h.flag &= ~(FLAG_Object_Modified);
#define FLAG_Object_Shared              (knh_flag_t)(1<<5)

#define knh_Object_isShared(o)  (((o)->h.flag & FLAG_Object_Shared) == FLAG_Object_Shared)

#define knh_Object_setShared(o,b) if(b) (o)->h.flag |= FLAG_Object_Shared; else (o)->h.flag &= ~(FLAG_Object_Shared);
#define FLAG_Object_Local4              (knh_flag_t)(1<<11)

#define knh_Object_isLocal4(o)  (((o)->h.flag & FLAG_Object_Local4) == FLAG_Object_Local4)

#define knh_Object_setLocal4(o,b) if(b) (o)->h.flag |= FLAG_Object_Local4; else (o)->h.flag &= ~(FLAG_Object_Local4);
#define FLAG_Object_Local3              (knh_flag_t)(1<<12)

#define knh_Object_isLocal3(o)  (((o)->h.flag & FLAG_Object_Local3) == FLAG_Object_Local3)

#define knh_Object_setLocal3(o,b) if(b) (o)->h.flag |= FLAG_Object_Local3; else (o)->h.flag &= ~(FLAG_Object_Local3);
#define FLAG_Object_Local2              (knh_flag_t)(1<<13)

#define knh_Object_isLocal2(o)  (((o)->h.flag & FLAG_Object_Local2) == FLAG_Object_Local2)

#define knh_Object_setLocal2(o,b) if(b) (o)->h.flag |= FLAG_Object_Local2; else (o)->h.flag &= ~(FLAG_Object_Local2);
#define FLAG_Object_Local1              (knh_flag_t)(1<<14)

#define knh_Object_isLocal1(o)  (((o)->h.flag & FLAG_Object_Local1) == FLAG_Object_Local1)

#define knh_Object_setLocal1(o,b) if(b) (o)->h.flag |= FLAG_Object_Local1; else (o)->h.flag &= ~(FLAG_Object_Local1);
#define FLAG_String_TextSgm             FLAG_Object_Local1

#define knh_String_isTextSgm(o)  (((o)->h.flag & FLAG_String_TextSgm) == FLAG_String_TextSgm)

#define knh_String_setTextSgm(o,b) if(b) (o)->h.flag |= FLAG_String_TextSgm; else (o)->h.flag &= ~(FLAG_String_TextSgm);
#define FLAG_String_Ascii               FLAG_Object_Local2

#define knh_String_isAscii(o)  (((o)->h.flag & FLAG_String_Ascii) == FLAG_String_Ascii)

#define knh_String_setAscii(o,b) if(b) (o)->h.flag |= FLAG_String_Ascii; else (o)->h.flag &= ~(FLAG_String_Ascii);
#define FLAG_Tuple_Triple               FLAG_Object_Local1

#define knh_Tuple_isTriple(o)  (((o)->h.flag & FLAG_Tuple_Triple) == FLAG_Tuple_Triple)

#define knh_Tuple_setTriple(o,b) if(b) (o)->h.flag |= FLAG_Tuple_Triple; else (o)->h.flag &= ~(FLAG_Tuple_Triple);
#define FLAG_Range_Inclusive            FLAG_Object_Local1

#define knh_Range_isInclusive(o)  (((o)->h.flag & FLAG_Range_Inclusive) == FLAG_Range_Inclusive)

#define knh_Range_setInclusive(o,b) if(b) (o)->h.flag |= FLAG_Range_Inclusive; else (o)->h.flag &= ~(FLAG_Range_Inclusive);
#define FLAG_Array_Dimension            FLAG_Object_Local1

#define knh_Array_hasDimension(o)  (((o)->h.flag & FLAG_Array_Dimension) == FLAG_Array_Dimension)

#define knh_Array_setDimension(o,b) if(b) (o)->h.flag |= FLAG_Array_Dimension; else (o)->h.flag &= ~(FLAG_Array_Dimension);
#define FLAG_IArray_Dimension           FLAG_Object_Local1

#define knh_IArray_hasDimension(o)  (((o)->h.flag & FLAG_IArray_Dimension) == FLAG_IArray_Dimension)

#define knh_IArray_setDimension(o,b) if(b) (o)->h.flag |= FLAG_IArray_Dimension; else (o)->h.flag &= ~(FLAG_IArray_Dimension);
#define FLAG_FArray_Dimension           FLAG_Object_Local1

#define knh_FArray_hasDimension(o)  (((o)->h.flag & FLAG_FArray_Dimension) == FLAG_FArray_Dimension)

#define knh_FArray_setDimension(o,b) if(b) (o)->h.flag |= FLAG_FArray_Dimension; else (o)->h.flag &= ~(FLAG_FArray_Dimension);
#define FLAG_DictMap_IgnoreCase         FLAG_Object_Local1

#define knh_DictMap_isIgnoreCase(o)  (((o)->h.flag & FLAG_DictMap_IgnoreCase) == FLAG_DictMap_IgnoreCase)

#define knh_DictMap_setIgnoreCase(o,b) if(b) (o)->h.flag |= FLAG_DictMap_IgnoreCase; else (o)->h.flag &= ~(FLAG_DictMap_IgnoreCase);
#define FLAG_DictSet_IgnoreCase         FLAG_Object_Local1

#define knh_DictSet_isIgnoreCase(o)  (((o)->h.flag & FLAG_DictSet_IgnoreCase) == FLAG_DictSet_IgnoreCase)

#define knh_DictSet_setIgnoreCase(o,b) if(b) (o)->h.flag |= FLAG_DictSet_IgnoreCase; else (o)->h.flag &= ~(FLAG_DictSet_IgnoreCase);
#define FLAG_Class_Release              (knh_flag_t)(1<<0)

#define knh_class_isRelease(o)  (((pClassTable(o))->cflag & FLAG_Class_Release) == FLAG_Class_Release)

#define knh_class_isDebug(o)  (((pClassTable(o))->cflag & FLAG_Class_Release) != FLAG_Class_Release)
#define FLAG_Class_Immutable            (knh_flag_t)(1<<1)

#define knh_class_isImmutable(o)  (((pClassTable(o))->cflag & FLAG_Class_Immutable) == FLAG_Class_Immutable)
#define FLAG_Class_Cyclic               (knh_flag_t)(1<<2)

#define knh_class_isCyclic(o)  (((pClassTable(o))->cflag & FLAG_Class_Cyclic) == FLAG_Class_Cyclic)

#define knh_class_setCyclic(o,b) if(b) (pClassTable(o))->cflag |= FLAG_Class_Cyclic; else (pClassTable(o))->cflag &= ~(FLAG_Class_Cyclic);
#define FLAG_Class_MetaClass            (knh_flag_t)(1<<3)

#define knh_class_isMetaClass(o)  (((pClassTable(o))->cflag & FLAG_Class_MetaClass) == FLAG_Class_MetaClass)
#define FLAG_Class_Private              (knh_flag_t)(1<<4)

#define knh_class_isPrivate(o)  (((pClassTable(o))->cflag & FLAG_Class_Private) == FLAG_Class_Private)

#define knh_class_isPublic(o)  (((pClassTable(o))->cflag & FLAG_Class_Private) != FLAG_Class_Private)
#define FLAG_Class_Final                (knh_flag_t)(1<<5)

#define knh_class_isFinal(o)  (((pClassTable(o))->cflag & FLAG_Class_Final) == FLAG_Class_Final)
#define FLAG_Class_Singleton            (knh_flag_t)(1<<6)

#define knh_class_isSingleton(o)  (((pClassTable(o))->cflag & FLAG_Class_Singleton) == FLAG_Class_Singleton)
#define FLAG_Class_Unique               (knh_flag_t)(1<<7)

#define knh_class_isUnique(o)  (((pClassTable(o))->cflag & FLAG_Class_Unique) == FLAG_Class_Unique)
#define FLAG_Class_Interface            (knh_flag_t)(1<<8)

#define knh_class_isInterface(o)  (((pClassTable(o))->cflag & FLAG_Class_Interface) == FLAG_Class_Interface)
#define FLAG_Class_TypeVariable         (knh_flag_t)(1<<9)

#define knh_class_isTypeVariable(o)  (((pClassTable(o))->cflag & FLAG_Class_TypeVariable) == FLAG_Class_TypeVariable)

#define knh_class_setTypeVariable(o,b) if(b) (pClassTable(o))->cflag |= FLAG_Class_TypeVariable; else (pClassTable(o))->cflag &= ~(FLAG_Class_TypeVariable);
#define FLAG_ClassField_Hidden          (knh_flag_t)(1<<0)

#define knh_ClassField_isHidden(o,n)  (((o)->fields[n].flag & FLAG_ClassField_Hidden) == FLAG_ClassField_Hidden)

#define knh_ClassField_setHidden(o,n,b) if(b) (o)->fields[n].flag |= FLAG_ClassField_Hidden; else (o)->fields[n].flag &= ~(FLAG_ClassField_Hidden);
#define FLAG_ClassField_Protected       (knh_flag_t)(1<<1)

#define knh_ClassField_isProtected(o,n)  (((o)->fields[n].flag & FLAG_ClassField_Protected) == FLAG_ClassField_Protected)

#define knh_ClassField_setProtected(o,n,b) if(b) (o)->fields[n].flag |= FLAG_ClassField_Protected; else (o)->fields[n].flag &= ~(FLAG_ClassField_Protected);
#define FLAG_ClassField_Getter          (knh_flag_t)(1<<2)

#define knh_ClassField_isGetter(o,n)  (((o)->fields[n].flag & FLAG_ClassField_Getter) == FLAG_ClassField_Getter)

#define knh_ClassField_setGetter(o,n,b) if(b) (o)->fields[n].flag |= FLAG_ClassField_Getter; else (o)->fields[n].flag &= ~(FLAG_ClassField_Getter);
#define FLAG_ClassField_Setter          (knh_flag_t)(1<<3)

#define knh_ClassField_isSetter(o,n)  (((o)->fields[n].flag & FLAG_ClassField_Setter) == FLAG_ClassField_Setter)

#define knh_ClassField_setSetter(o,n,b) if(b) (o)->fields[n].flag |= FLAG_ClassField_Setter; else (o)->fields[n].flag &= ~(FLAG_ClassField_Setter);
#define FLAG_ClassField_Key             (knh_flag_t)(1<<4)

#define knh_ClassField_isKey(o,n)  (((o)->fields[n].flag & FLAG_ClassField_Key) == FLAG_ClassField_Key)

#define knh_ClassField_setKey(o,n,b) if(b) (o)->fields[n].flag |= FLAG_ClassField_Key; else (o)->fields[n].flag &= ~(FLAG_ClassField_Key);
#define FLAG_ClassField_Volatile        (knh_flag_t)(1<<5)

#define knh_ClassField_isVolatile(o,n)  (((o)->fields[n].flag & FLAG_ClassField_Volatile) == FLAG_ClassField_Volatile)

#define knh_ClassField_setVolatile(o,n,b) if(b) (o)->fields[n].flag |= FLAG_ClassField_Volatile; else (o)->fields[n].flag &= ~(FLAG_ClassField_Volatile);
#define FLAG_ClassField_ReadOnly        (knh_flag_t)(1<<6)

#define knh_ClassField_isReadOnly(o,n)  (((o)->fields[n].flag & FLAG_ClassField_ReadOnly) == FLAG_ClassField_ReadOnly)

#define knh_ClassField_setReadOnly(o,n,b) if(b) (o)->fields[n].flag |= FLAG_ClassField_ReadOnly; else (o)->fields[n].flag &= ~(FLAG_ClassField_ReadOnly);
#define FLAG_ClassField_Property        (knh_flag_t)(1<<7)

#define knh_ClassField_isProperty(o,n)  (((o)->fields[n].flag & FLAG_ClassField_Property) == FLAG_ClassField_Property)

#define knh_ClassField_setProperty(o,n,b) if(b) (o)->fields[n].flag |= FLAG_ClassField_Property; else (o)->fields[n].flag &= ~(FLAG_ClassField_Property);
#define FLAG_ClassField_Principle       (knh_flag_t)(1<<8)

#define knh_ClassField_isPrinciple(o,n)  (((o)->fields[n].flag & FLAG_ClassField_Principle) == FLAG_ClassField_Principle)

#define knh_ClassField_setPrinciple(o,n,b) if(b) (o)->fields[n].flag |= FLAG_ClassField_Principle; else (o)->fields[n].flag &= ~(FLAG_ClassField_Principle);
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
#define FLAG_Method_NullBase            (knh_flag_t)(1<<5)

#define knh_Method_isNullBase(o)  ((DP(o)->flag & FLAG_Method_NullBase) == FLAG_Method_NullBase)
#define FLAG_Method_VarArgs             (knh_flag_t)(1<<6)

#define knh_Method_isVarArgs(o)  ((DP(o)->flag & FLAG_Method_VarArgs) == FLAG_Method_VarArgs)

#define knh_Method_setVarArgs(o,b) if(b) DP(o)->flag |= FLAG_Method_VarArgs; else DP(o)->flag &= ~(FLAG_Method_VarArgs);
#define FLAG_Method_Inline              (knh_flag_t)(1<<7)

#define knh_Method_isInline(o)  ((DP(o)->flag & FLAG_Method_Inline) == FLAG_Method_Inline)

#define knh_Method_setInline(o,b) if(b) DP(o)->flag |= FLAG_Method_Inline; else DP(o)->flag &= ~(FLAG_Method_Inline);
#define FLAG_Method_Generator           (knh_flag_t)(1<<8)

#define knh_Method_isGenerator(o)  ((DP(o)->flag & FLAG_Method_Generator) == FLAG_Method_Generator)

#define knh_Method_setGenerator(o,b) if(b) DP(o)->flag |= FLAG_Method_Generator; else DP(o)->flag &= ~(FLAG_Method_Generator);
#define FLAG_Method_ObjectCode          (knh_flag_t)(1<<9)

#define knh_Method_isObjectCode(o)  ((DP(o)->flag & FLAG_Method_ObjectCode) == FLAG_Method_ObjectCode)

#define knh_Method_setObjectCode(o,b) if(b) DP(o)->flag |= FLAG_Method_ObjectCode; else DP(o)->flag &= ~(FLAG_Method_ObjectCode);
#define FLAG_Method_Hidden              (knh_flag_t)(1<<10)

#define knh_Method_isHidden(o)  ((DP(o)->flag & FLAG_Method_Hidden) == FLAG_Method_Hidden)

#define knh_Method_setHidden(o,b) if(b) DP(o)->flag |= FLAG_Method_Hidden; else DP(o)->flag &= ~(FLAG_Method_Hidden);
#define FLAG_Method_Tricky              (knh_flag_t)(1<<11)

#define knh_Method_isTricky(o)  ((DP(o)->flag & FLAG_Method_Tricky) == FLAG_Method_Tricky)

#define knh_Method_setTricky(o,b) if(b) DP(o)->flag |= FLAG_Method_Tricky; else DP(o)->flag &= ~(FLAG_Method_Tricky);
#define FLAG_Mapper_Interface           (knh_flag_t)(1<<0)

#define knh_Mapper_isInterface(o)  ((DP(o)->flag & FLAG_Mapper_Interface) == FLAG_Mapper_Interface)

#define knh_Mapper_setInterface(o,b) if(b) DP(o)->flag |= FLAG_Mapper_Interface; else DP(o)->flag &= ~(FLAG_Mapper_Interface);
#define FLAG_Mapper_ICast               (knh_flag_t)(1<<1)

#define knh_Mapper_isICast(o)  ((DP(o)->flag & FLAG_Mapper_ICast) == FLAG_Mapper_ICast)
#define FLAG_Mapper_Significant         (knh_flag_t)(1<<2)

#define knh_Mapper_isSignificant(o)  ((DP(o)->flag & FLAG_Mapper_Significant) == FLAG_Mapper_Significant)

#define knh_Mapper_setSignificant(o,b) if(b) DP(o)->flag |= FLAG_Mapper_Significant; else DP(o)->flag &= ~(FLAG_Mapper_Significant);
#define FLAG_Mapper_Final               (knh_flag_t)(1<<3)

#define knh_Mapper_isFinal(o)  ((DP(o)->flag & FLAG_Mapper_Final) == FLAG_Mapper_Final)

#define knh_Mapper_setFinal(o,b) if(b) DP(o)->flag |= FLAG_Mapper_Final; else DP(o)->flag &= ~(FLAG_Mapper_Final);
#define FLAG_Mapper_Synonym             (knh_flag_t)(1<<4)

#define knh_Mapper_isSynonym(o)  ((DP(o)->flag & FLAG_Mapper_Synonym) == FLAG_Mapper_Synonym)

#define knh_Mapper_setSynonym(o,b) if(b) DP(o)->flag |= FLAG_Mapper_Synonym; else DP(o)->flag &= ~(FLAG_Mapper_Synonym);
#define FLAG_Mapper_LossLess            (knh_flag_t)(1<<5)

#define knh_Mapper_isLossLess(o)  ((DP(o)->flag & FLAG_Mapper_LossLess) == FLAG_Mapper_LossLess)

#define knh_Mapper_setLossLess(o,b) if(b) DP(o)->flag |= FLAG_Mapper_LossLess; else DP(o)->flag &= ~(FLAG_Mapper_LossLess);
#define FLAG_Mapper_Total               (knh_flag_t)(1<<6)

#define knh_Mapper_isTotal(o)  ((DP(o)->flag & FLAG_Mapper_Total) == FLAG_Mapper_Total)

#define knh_Mapper_isPartial(o)  ((DP(o)->flag & FLAG_Mapper_Total) != FLAG_Mapper_Total)

#define knh_Mapper_setTotal(o,b) if(b) DP(o)->flag |= FLAG_Mapper_Total; else DP(o)->flag &= ~(FLAG_Mapper_Total);

#define knh_Mapper_setPartial(o,b)  if(b) DP(o)->flag &= ~(FLAG_Mapper_Total); else DP(o)->flag |= FLAG_Mapper_Total;
#define FLAG_Mapper_Const               (knh_flag_t)(1<<7)

#define knh_Mapper_isConst(o)  ((DP(o)->flag & FLAG_Mapper_Const) == FLAG_Mapper_Const)

#define knh_Mapper_isTemporal(o)  ((DP(o)->flag & FLAG_Mapper_Const) != FLAG_Mapper_Const)

#define knh_Mapper_setConst(o,b) if(b) DP(o)->flag |= FLAG_Mapper_Const; else DP(o)->flag &= ~(FLAG_Mapper_Const);

#define knh_Mapper_setTemporal(o,b)  if(b) DP(o)->flag &= ~(FLAG_Mapper_Const); else DP(o)->flag |= FLAG_Mapper_Const;
#define FLAG_Mapper_Local               (knh_flag_t)(1<<8)

#define knh_Mapper_isLocal(o)  ((DP(o)->flag & FLAG_Mapper_Local) == FLAG_Mapper_Local)

#define knh_Mapper_setLocal(o,b) if(b) DP(o)->flag |= FLAG_Mapper_Local; else DP(o)->flag &= ~(FLAG_Mapper_Local);
#define FLAG_Mapper_Derived             (knh_flag_t)(1<<9)

#define knh_Mapper_isDerived(o)  ((DP(o)->flag & FLAG_Mapper_Derived) == FLAG_Mapper_Derived)

#define knh_Mapper_setDerived(o,b) if(b) DP(o)->flag |= FLAG_Mapper_Derived; else DP(o)->flag &= ~(FLAG_Mapper_Derived);
#define FLAG_Mapper_MapMap              (knh_flag_t)(1<<10)

#define knh_Mapper_isMapMap(o)  ((DP(o)->flag & FLAG_Mapper_MapMap) == FLAG_Mapper_MapMap)

#define knh_Mapper_setMapMap(o,b) if(b) DP(o)->flag |= FLAG_Mapper_MapMap; else DP(o)->flag &= ~(FLAG_Mapper_MapMap);
#define FLAG_ClassMap_Sorted            FLAG_Object_Local1

#define knh_ClassMap_isSorted(o)  (((o)->h.flag & FLAG_ClassMap_Sorted) == FLAG_ClassMap_Sorted)

#define knh_ClassMap_setSorted(o,b) if(b) (o)->h.flag |= FLAG_ClassMap_Sorted; else (o)->h.flag &= ~(FLAG_ClassMap_Sorted);
#define FLAG_Closure_StoredEnv          FLAG_Object_Local1

#define knh_Closure_isStoredEnv(o)  (((o)->h.flag & FLAG_Closure_StoredEnv) == FLAG_Closure_StoredEnv)

#define knh_Closure_setStoredEnv(o,b) if(b) (o)->h.flag |= FLAG_Closure_StoredEnv; else (o)->h.flag &= ~(FLAG_Closure_StoredEnv);
#define FLAG_Thunk_Evaluated            FLAG_Object_Local1

#define knh_Thunk_isEvaluated(o)  (((o)->h.flag & FLAG_Thunk_Evaluated) == FLAG_Thunk_Evaluated)

#define knh_Thunk_setEvaluated(o,b) if(b) (o)->h.flag |= FLAG_Thunk_Evaluated; else (o)->h.flag &= ~(FLAG_Thunk_Evaluated);
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

#define knh_Token_isBOL(o)  ((SP(o)->flag & FLAG_Token_BOL) == FLAG_Token_BOL)

#define knh_Token_setBOL(o,b) if(b) SP(o)->flag |= FLAG_Token_BOL; else SP(o)->flag &= ~(FLAG_Token_BOL);
#define FLAG_Token_TailWildCard         (knh_flag_t)(1<<1)

#define knh_Token_isTailWildCard(o)  ((SP(o)->flag & FLAG_Token_TailWildCard) == FLAG_Token_TailWildCard)

#define knh_Token_setTailWildCard(o,b) if(b) SP(o)->flag |= FLAG_Token_TailWildCard; else SP(o)->flag &= ~(FLAG_Token_TailWildCard);
#define FLAG_Token_TopDot               (knh_flag_t)(1<<2)

#define knh_Token_isTopDot(o)  ((SP(o)->flag & FLAG_Token_TopDot) == FLAG_Token_TopDot)

#define knh_Token_setTopDot(o,b) if(b) SP(o)->flag |= FLAG_Token_TopDot; else SP(o)->flag &= ~(FLAG_Token_TopDot);
#define FLAG_Token_Getter               (knh_flag_t)(1<<3)

#define knh_Token_isGetter(o)  ((SP(o)->flag & FLAG_Token_Getter) == FLAG_Token_Getter)

#define knh_Token_setGetter(o,b) if(b) SP(o)->flag |= FLAG_Token_Getter; else SP(o)->flag &= ~(FLAG_Token_Getter);
#define FLAG_Token_Setter               (knh_flag_t)(1<<4)

#define knh_Token_isSetter(o)  ((SP(o)->flag & FLAG_Token_Setter) == FLAG_Token_Setter)

#define knh_Token_setSetter(o,b) if(b) SP(o)->flag |= FLAG_Token_Setter; else SP(o)->flag &= ~(FLAG_Token_Setter);
#define FLAG_Token_ExceptionType        (knh_flag_t)(1<<5)

#define knh_Token_isExceptionType(o)  ((SP(o)->flag & FLAG_Token_ExceptionType) == FLAG_Token_ExceptionType)

#define knh_Token_setExceptionType(o,b) if(b) SP(o)->flag |= FLAG_Token_ExceptionType; else SP(o)->flag &= ~(FLAG_Token_ExceptionType);
#define FLAG_Token_NotNullType          (knh_flag_t)(1<<6)

#define knh_Token_isNotNullType(o)  ((SP(o)->flag & FLAG_Token_NotNullType) == FLAG_Token_NotNullType)

#define knh_Token_setNotNullType(o,b) if(b) SP(o)->flag |= FLAG_Token_NotNullType; else SP(o)->flag &= ~(FLAG_Token_NotNullType);
#define FLAG_Token_NullableType         (knh_flag_t)(1<<7)

#define knh_Token_isNullableType(o)  ((SP(o)->flag & FLAG_Token_NullableType) == FLAG_Token_NullableType)

#define knh_Token_setNullableType(o,b) if(b) SP(o)->flag |= FLAG_Token_NullableType; else SP(o)->flag &= ~(FLAG_Token_NullableType);
#define FLAG_Stmt_Adposition            (knh_flag_t)(1<<0)

#define knh_Stmt_isAdposition(o)  ((SP(o)->flag & FLAG_Stmt_Adposition) == FLAG_Stmt_Adposition)

#define knh_Stmt_setAdposition(o,b) if(b) SP(o)->flag |= FLAG_Stmt_Adposition; else SP(o)->flag &= ~(FLAG_Stmt_Adposition);
#define FLAG_Stmt_Literal               (knh_flag_t)(1<<1)

#define knh_Stmt_isLiteral(o)  ((SP(o)->flag & FLAG_Stmt_Literal) == FLAG_Stmt_Literal)

#define knh_Stmt_setLiteral(o,b) if(b) SP(o)->flag |= FLAG_Stmt_Literal; else SP(o)->flag &= ~(FLAG_Stmt_Literal);
#define FLAG_Stmt_AutoReturn            (knh_flag_t)(1<<2)

#define knh_Stmt_isAutoReturn(o)  ((SP(o)->flag & FLAG_Stmt_AutoReturn) == FLAG_Stmt_AutoReturn)

#define knh_Stmt_setAutoReturn(o,b) if(b) SP(o)->flag |= FLAG_Stmt_AutoReturn; else SP(o)->flag &= ~(FLAG_Stmt_AutoReturn);
#define FLAG_Stmt_Untyped               (knh_flag_t)(1<<3)

#define knh_Stmt_isUntyped(o)  ((SP(o)->flag & FLAG_Stmt_Untyped) == FLAG_Stmt_Untyped)

#define knh_Stmt_setUntyped(o,b) if(b) SP(o)->flag |= FLAG_Stmt_Untyped; else SP(o)->flag &= ~(FLAG_Stmt_Untyped);
#define FLAG_Stmt_Expr                  (knh_flag_t)(1<<4)

#define knh_Stmt_isExpr(o)  ((SP(o)->flag & FLAG_Stmt_Expr) == FLAG_Stmt_Expr)

#define knh_Stmt_setExpr(o,b) if(b) SP(o)->flag |= FLAG_Stmt_Expr; else SP(o)->flag &= ~(FLAG_Stmt_Expr);
#define FLAG_Stmt_Memo1                 (knh_flag_t)(1<<5)

#define knh_Stmt_isMemo1(o)  ((SP(o)->flag & FLAG_Stmt_Memo1) == FLAG_Stmt_Memo1)

#define knh_Stmt_setMemo1(o,b) if(b) SP(o)->flag |= FLAG_Stmt_Memo1; else SP(o)->flag &= ~(FLAG_Stmt_Memo1);
#define FLAG_Stmt_Memo2                 (knh_flag_t)(1<<6)

#define knh_Stmt_isMemo2(o)  ((SP(o)->flag & FLAG_Stmt_Memo2) == FLAG_Stmt_Memo2)

#define knh_Stmt_setMemo2(o,b) if(b) SP(o)->flag |= FLAG_Stmt_Memo2; else SP(o)->flag &= ~(FLAG_Stmt_Memo2);
#define FLAG_Gamma_Cancelled            (knh_flag_t)(1<<0)

#define knh_Gamma_isCancelled(o)  ((DP(o)->flag & FLAG_Gamma_Cancelled) == FLAG_Gamma_Cancelled)

#define knh_Gamma_setCancelled(o,b) if(b) DP(o)->flag |= FLAG_Gamma_Cancelled; else DP(o)->flag &= ~(FLAG_Gamma_Cancelled);
#define FLAG_Gamma_PROCEED              (knh_flag_t)(1<<1)

#define knh_Gamma_hasPROCEED(o)  ((DP(o)->flag & FLAG_Gamma_PROCEED) == FLAG_Gamma_PROCEED)

#define knh_Gamma_foundPROCEED(o,b) if(b) DP(o)->flag |= FLAG_Gamma_PROCEED; else DP(o)->flag &= ~(FLAG_Gamma_PROCEED);
#define FLAG_Gamma_RETURN               (knh_flag_t)(1<<2)

#define knh_Gamma_hasRETURN(o)  ((DP(o)->flag & FLAG_Gamma_RETURN) == FLAG_Gamma_RETURN)

#define knh_Gamma_foundRETURN(o,b) if(b) DP(o)->flag |= FLAG_Gamma_RETURN; else DP(o)->flag &= ~(FLAG_Gamma_RETURN);
#define FLAG_Gamma_YEILD                (knh_flag_t)(1<<3)

#define knh_Gamma_hasYEILD(o)  ((DP(o)->flag & FLAG_Gamma_YEILD) == FLAG_Gamma_YEILD)

#define knh_Gamma_foundYEILD(o,b) if(b) DP(o)->flag |= FLAG_Gamma_YEILD; else DP(o)->flag &= ~(FLAG_Gamma_YEILD);
#define FLAG_Gamma_FIELD                (knh_flag_t)(1<<4)

#define knh_Gamma_hasFIELD(o)  ((DP(o)->flag & FLAG_Gamma_FIELD) == FLAG_Gamma_FIELD)

#define knh_Gamma_foundFIELD(o,b) if(b) DP(o)->flag |= FLAG_Gamma_FIELD; else DP(o)->flag &= ~(FLAG_Gamma_FIELD);
#define FLAG_Gamma_STACK                (knh_flag_t)(1<<5)

#define knh_Gamma_hasSTACK(o)  ((DP(o)->flag & FLAG_Gamma_STACK) == FLAG_Gamma_STACK)

#define knh_Gamma_foundSTACK(o,b) if(b) DP(o)->flag |= FLAG_Gamma_STACK; else DP(o)->flag &= ~(FLAG_Gamma_STACK);
#define FLAG_Gamma_SCRIPT               (knh_flag_t)(1<<6)

#define knh_Gamma_hasSCRIPT(o)  ((DP(o)->flag & FLAG_Gamma_SCRIPT) == FLAG_Gamma_SCRIPT)

#define knh_Gamma_foundSCRIPT(o,b) if(b) DP(o)->flag |= FLAG_Gamma_SCRIPT; else DP(o)->flag &= ~(FLAG_Gamma_SCRIPT);
#define FLAG_Gamma_Data                 (knh_flag_t)(1<<0)

#define knh_Gamma_isData(o)  ((DP(o)->pflag & FLAG_Gamma_Data) == FLAG_Gamma_Data)

#define knh_Gamma_setData(o,b) if(b) DP(o)->pflag |= FLAG_Gamma_Data; else DP(o)->pflag &= ~(FLAG_Gamma_Data);

/* ======================================================================== */
/* EXPT */
#define EXPT_Exception          1
#define EXPT_Fatal              2
#define EXPT_OutOfMemory        3
#define EXPT_Security           4
#define EXPT_Null               5
#define EXPT_OutOfIndex         6
#define EXPT_IO                 7
#define EXPT_Script             8
#define EXPT_Syntax             9
#define EXPT_Type               10
#define EXPT_Assertion          11

/* ======================================================================== */
/* FIELDN */
#define FIELDN_         0
#define FIELDN_2D       1
#define FIELDN_2d       1
#define FIELDN_3D       2
#define FIELDN_3d       2
#define FIELDN_abstract 3
#define FIELDN_add      4
#define FIELDN_addHistory 5
#define FIELDN_addhistory 5
#define FIELDN_all      6
#define FIELDN_anno     7
#define FIELDN_ascii    8
#define FIELDN_autoFlush 9
#define FIELDN_autoflush 9
#define FIELDN_bag      10
#define FIELDN_base     11
#define FIELDN_bits     12
#define FIELDN_buf      13
#define FIELDN_bytes    14
#define FIELDN_c        15
#define FIELDN_ch       16
#define FIELDN_char     17
#define FIELDN_class    18
#define FIELDN_clear    19
#define FIELDN_clearBuffer 20
#define FIELDN_clearbuffer 20
#define FIELDN_close    21
#define FIELDN_closed   22
#define FIELDN_cmd      23
#define FIELDN_concat   24
#define FIELDN_const    25
#define FIELDN_d        26
#define FIELDN_data     27
#define FIELDN_debug    28
#define FIELDN_default  29
#define FIELDN_defined  30
#define FIELDN_delegate 31
#define FIELDN_delim    32
#define FIELDN_derived  33
#define FIELDN_domain   34
#define FIELDN_dump     35
#define FIELDN_e        36
#define FIELDN_empty    37
#define FIELDN_enc      38
#define FIELDN_encoding 39
#define FIELDN_end      40
#define FIELDN_endsWith 41
#define FIELDN_endswith 41
#define FIELDN_endsWith__IgnoreCase 42
#define FIELDN_endswith__ignorecase 42
#define FIELDN_equals   43
#define FIELDN_equals__IgnoreCase 44
#define FIELDN_equals__ignorecase 44
#define FIELDN_err      45
#define FIELDN_eval     46
#define FIELDN_exec     47
#define FIELDN_exit     48
#define FIELDN_f        49
#define FIELDN_final    50
#define FIELDN_first    51
#define FIELDN_flag     52
#define FIELDN_float    53
#define FIELDN_floatToIntBits 54
#define FIELDN_floattointbits 54
#define FIELDN_flush    55
#define FIELDN_fmt      56
#define FIELDN_format   57
#define FIELDN_func     58
#define FIELDN_gc       59
#define FIELDN_hasDir   60
#define FIELDN_hasdir   60
#define FIELDN_hasFile  61
#define FIELDN_hasfile  61
#define FIELDN_hashCode 62
#define FIELDN_hashcode 62
#define FIELDN_hasLib   63
#define FIELDN_haslib   63
#define FIELDN_host     64
#define FIELDN_iCast    65
#define FIELDN_icast    65
#define FIELDN_ignoreCase 66
#define FIELDN_ignorecase 66
#define FIELDN_immutable 67
#define FIELDN_in       68
#define FIELDN_indexOf  69
#define FIELDN_indexof  69
#define FIELDN_indexOf__IgnoreCase 70
#define FIELDN_indexof__ignorecase 70
#define FIELDN_init     71
#define FIELDN_initCapacity 72
#define FIELDN_initcapacity 72
#define FIELDN_inputStream 73
#define FIELDN_inputstream 73
#define FIELDN_insert   74
#define FIELDN_int      75
#define FIELDN_interactive 76
#define FIELDN_interface 77
#define FIELDN_intToFloatBits 78
#define FIELDN_inttofloatbits 78
#define FIELDN_invoke   79
#define FIELDN_isInclusive 80
#define FIELDN_isinclusive 80
#define FIELDN_isTrim   81
#define FIELDN_istrim   81
#define FIELDN_k        82
#define FIELDN_key      83
#define FIELDN_lastIndexOf 84
#define FIELDN_lastindexof 84
#define FIELDN_lastIndexOf__IgnoreCase 85
#define FIELDN_lastindexof__ignorecase 85
#define FIELDN_length   86
#define FIELDN_lib      87
#define FIELDN_likely   88
#define FIELDN_line     89
#define FIELDN_listDir  90
#define FIELDN_listdir  90
#define FIELDN_listProperties 91
#define FIELDN_listproperties 91
#define FIELDN_local    92
#define FIELDN_logging  93
#define FIELDN_lossLess 94
#define FIELDN_lossless 94
#define FIELDN_m        95
#define FIELDN_main     96
#define FIELDN_man      97
#define FIELDN_match    98
#define FIELDN_memcpy   99
#define FIELDN_metaClass 100
#define FIELDN_metaclass 100
#define FIELDN_method   101
#define FIELDN_methodCompilationListener 102
#define FIELDN_methodcompilationlistener 102
#define FIELDN_methodTypingListener 103
#define FIELDN_methodtypinglistener 103
#define FIELDN_mkdir    104
#define FIELDN_mode     105
#define FIELDN_modified 106
#define FIELDN_msg      107
#define FIELDN_n        108
#define FIELDN_name     109
#define FIELDN_new      110
#define FIELDN_new__array 111
#define FIELDN_new__array 111
#define FIELDN_new__array2D 112
#define FIELDN_new__array2d 112
#define FIELDN_new__array3D 113
#define FIELDN_new__array3d 113
#define FIELDN_new__dictmap 114
#define FIELDN_new__dictmap 114
#define FIELDN_new__init 115
#define FIELDN_new__init 115
#define FIELDN_new__range 116
#define FIELDN_new__range 116
#define FIELDN_newone   117
#define FIELDN_newpath  118
#define FIELDN_next     119
#define FIELDN_notNull  120
#define FIELDN_notnull  120
#define FIELDN_null     121
#define FIELDN_o        122
#define FIELDN_offset   123
#define FIELDN_op0      124
#define FIELDN_op1      125
#define FIELDN_opAdd    126
#define FIELDN_opadd    126
#define FIELDN_opAddr   127
#define FIELDN_opaddr   127
#define FIELDN_opAppend 128
#define FIELDN_opappend 128
#define FIELDN_opAs     129
#define FIELDN_opas     129
#define FIELDN_opCase   130
#define FIELDN_opcase   130
#define FIELDN_opDiv    131
#define FIELDN_opdiv    131
#define FIELDN_opEq     132
#define FIELDN_opeq     132
#define FIELDN_opGt     133
#define FIELDN_opgt     133
#define FIELDN_opGte    134
#define FIELDN_opgte    134
#define FIELDN_opHas    135
#define FIELDN_ophas    135
#define FIELDN_opInstanceof 136
#define FIELDN_opinstanceof 136
#define FIELDN_opInTo   137
#define FIELDN_opinto   137
#define FIELDN_opIs     138
#define FIELDN_opis     138
#define FIELDN_opItr    139
#define FIELDN_opitr    139
#define FIELDN_opItr__2 140
#define FIELDN_opitr__2 140
#define FIELDN_opLand   141
#define FIELDN_opland   141
#define FIELDN_opLnot   142
#define FIELDN_oplnot   142
#define FIELDN_opLor    143
#define FIELDN_oplor    143
#define FIELDN_opLshift 144
#define FIELDN_oplshift 144
#define FIELDN_opLt     145
#define FIELDN_oplt     145
#define FIELDN_opLte    146
#define FIELDN_oplte    146
#define FIELDN_opMatch  147
#define FIELDN_opmatch  147
#define FIELDN_opMod    148
#define FIELDN_opmod    148
#define FIELDN_opMul    149
#define FIELDN_opmul    149
#define FIELDN_opN      150
#define FIELDN_opn      150
#define FIELDN_opNeg    151
#define FIELDN_opneg    151
#define FIELDN_opNeq    152
#define FIELDN_opneq    152
#define FIELDN_opNext   153
#define FIELDN_opnext   153
#define FIELDN_opNot    154
#define FIELDN_opnot    154
#define FIELDN_opPrev   155
#define FIELDN_opprev   155
#define FIELDN_opRangeTo 156
#define FIELDN_oprangeto 156
#define FIELDN_opRangeUntil 157
#define FIELDN_oprangeuntil 157
#define FIELDN_opRshift 158
#define FIELDN_oprshift 158
#define FIELDN_opSub    159
#define FIELDN_opsub    159
#define FIELDN_option   160
#define FIELDN_opXor    161
#define FIELDN_opxor    161
#define FIELDN_out      162
#define FIELDN_outputStream 163
#define FIELDN_outputstream 163
#define FIELDN_p        164
#define FIELDN_partial  165
#define FIELDN_path     166
#define FIELDN_pattern  167
#define FIELDN_pop      168
#define FIELDN_port     169
#define FIELDN_print    170
#define FIELDN_println  171
#define FIELDN_private  172
#define FIELDN_prompt   173
#define FIELDN_property 174
#define FIELDN_public   175
#define FIELDN_putc     176
#define FIELDN_putChar  177
#define FIELDN_putchar  177
#define FIELDN_query    178
#define FIELDN_random   179
#define FIELDN_randomSeed 180
#define FIELDN_randomseed 180
#define FIELDN_re       181
#define FIELDN_read     182
#define FIELDN_readData 183
#define FIELDN_readdata 183
#define FIELDN_readLine 184
#define FIELDN_readline 184
#define FIELDN_release  185
#define FIELDN_remove   186
#define FIELDN_rename   187
#define FIELDN_replace  188
#define FIELDN_result   189
#define FIELDN_reverse  190
#define FIELDN_s        191
#define FIELDN_script   192
#define FIELDN_second   193
#define FIELDN_seed     194
#define FIELDN_shared   195
#define FIELDN_shuffle  196
#define FIELDN_significant 197
#define FIELDN_singleton 198
#define FIELDN_size     199
#define FIELDN_sort     200
#define FIELDN_split    201
#define FIELDN_start    202
#define FIELDN_startsWith 203
#define FIELDN_startswith 203
#define FIELDN_startsWith__IgnoreCase 204
#define FIELDN_startswith__ignorecase 204
#define FIELDN_static   205
#define FIELDN_status   206
#define FIELDN_stmt     207
#define FIELDN_storingBuffer 208
#define FIELDN_storingbuffer 208
#define FIELDN_strict   209
#define FIELDN_string   210
#define FIELDN_substring 211
#define FIELDN_super    212
#define FIELDN_swap     213
#define FIELDN_synonym  214
#define FIELDN_system   215
#define FIELDN_temporal 216
#define FIELDN_test     217
#define FIELDN_this     218
#define FIELDN_times    219
#define FIELDN_toLower  220
#define FIELDN_tolower  220
#define FIELDN_total    221
#define FIELDN_toUpper  222
#define FIELDN_toupper  222
#define FIELDN_trim     223
#define FIELDN_twofold  224
#define FIELDN_typeof   225
#define FIELDN_u        226
#define FIELDN_uCS4     227
#define FIELDN_ucs4     227
#define FIELDN_undefined 228
#define FIELDN_unique   229
#define FIELDN_unlikely 230
#define FIELDN_unlink   231
#define FIELDN_urn      232
#define FIELDN_v        233
#define FIELDN_value    234
#define FIELDN_varArgs  235
#define FIELDN_varargs  235
#define FIELDN_vargs    236
#define FIELDN_verbose  237
#define FIELDN_virtual  238
#define FIELDN_w        239
#define FIELDN_write    240
#define FIELDN_writeData 241
#define FIELDN_writedata 241
#define FIELDN_x        242
#define FIELDN_y        243
#define FIELDN_z        244
#define KNH_TFIELDN_SIZE 245

/* ======================================================================== */
/* METHODN */
#define METHODN_setAll  METHODN_TO_SETTER(FIELDN_all)
#define METHODN_unlikely FIELDN_unlikely
#define METHODN_format  FIELDN_format
#define METHODN_shuffle FIELDN_shuffle
#define METHODN_value   FIELDN_value
#define METHODN_opLte   FIELDN_oplte
#define METHODN_indexOf__IgnoreCase FIELDN_indexof__ignorecase
#define METHODN_isIgnoreCase METHODN_TO_GETTER(FIELDN_ignorecase)
#define METHODN_main    FIELDN_main
#define METHODN_opItr__2 FIELDN_opitr__2
#define METHODN_getBytes METHODN_TO_GETTER(FIELDN_bytes)
#define METHODN__data   METHODN_TO_MOVTEXT(FIELDN_data)
#define METHODN_flush   FIELDN_flush
#define METHODN_opAddr  FIELDN_opaddr
#define METHODN_new__init FIELDN_new__init
#define METHODN_memcpy  FIELDN_memcpy
#define METHODN_new__dictmap FIELDN_new__dictmap
#define METHODN_delegate FIELDN_delegate
#define METHODN_eval    FIELDN_eval
#define METHODN_isUndefined METHODN_TO_GETTER(FIELDN_undefined)
#define METHODN_opNot   FIELDN_opnot
#define METHODN_isClosed METHODN_TO_GETTER(FIELDN_closed)
#define METHODN_system  FIELDN_system
#define METHODN_addHistory FIELDN_addhistory
#define METHODN_getString METHODN_TO_GETTER(FIELDN_string)
#define METHODN_new     FIELDN_new
#define METHODN_floatToIntBits FIELDN_floattointbits
#define METHODN_getFloat METHODN_TO_GETTER(FIELDN_float)
#define METHODN__dump   METHODN_TO_MOVTEXT(FIELDN_dump)
#define METHODN_opXor   FIELDN_opxor
#define METHODN_opAppend FIELDN_opappend
#define METHODN_setStrict METHODN_TO_SETTER(FIELDN_strict)
#define METHODN_opNext  FIELDN_opnext
#define METHODN_isICast METHODN_TO_GETTER(FIELDN_icast)
#define METHODN_defined FIELDN_defined
#define METHODN_new__array3D FIELDN_new__array3d
#define METHODN_setErr  METHODN_TO_SETTER(FIELDN_err)
#define METHODN_opLnot  FIELDN_oplnot
#define METHODN_endsWith FIELDN_endswith
#define METHODN_add     FIELDN_add
#define METHODN_get3D   METHODN_TO_GETTER(FIELDN_3d)
#define METHODN_setMethodTypingListener METHODN_TO_SETTER(FIELDN_methodtypinglistener)
#define METHODN_equals__IgnoreCase FIELDN_equals__ignorecase
#define METHODN_readData FIELDN_readdata
#define METHODN_setStoringBuffer METHODN_TO_SETTER(FIELDN_storingbuffer)
#define METHODN_hasLib  FIELDN_haslib
#define METHODN_opN     FIELDN_opn
#define METHODN_read    FIELDN_read
#define METHODN_split   FIELDN_split
#define METHODN_getUCS4 METHODN_TO_GETTER(FIELDN_ucs4)
#define METHODN_lastIndexOf__IgnoreCase FIELDN_lastindexof__ignorecase
#define METHODN_print   FIELDN_print
#define METHODN_opMatch FIELDN_opmatch
#define METHODN_isTotal METHODN_TO_GETTER(FIELDN_total)
#define METHODN_isDerived METHODN_TO_GETTER(FIELDN_derived)
#define METHODN_setIn   METHODN_TO_SETTER(FIELDN_in)
#define METHODN_equals  FIELDN_equals
#define METHODN_isStmt  METHODN_TO_GETTER(FIELDN_stmt)
#define METHODN_opLt    FIELDN_oplt
#define METHODN_isMetaClass METHODN_TO_GETTER(FIELDN_metaclass)
#define METHODN_getSize METHODN_TO_GETTER(FIELDN_size)
#define METHODN_putc    FIELDN_putc
#define METHODN_gc      FIELDN_gc
#define METHODN_startsWith FIELDN_startswith
#define METHODN_super   FIELDN_super
#define METHODN_query   FIELDN_query
#define METHODN_likely  FIELDN_likely
#define METHODN_setOut  METHODN_TO_SETTER(FIELDN_out)
#define METHODN_opItr   FIELDN_opitr
#define METHODN_getName METHODN_TO_GETTER(FIELDN_name)
#define METHODN_sort    FIELDN_sort
#define METHODN_isAbstract METHODN_TO_GETTER(FIELDN_abstract)
#define METHODN_insert  FIELDN_insert
#define METHODN_opAdd   FIELDN_opadd
#define METHODN_isModified METHODN_TO_GETTER(FIELDN_modified)
#define METHODN_exec    FIELDN_exec
#define METHODN_getProperty METHODN_TO_GETTER(FIELDN_property)
#define METHODN_opMul   FIELDN_opmul
#define METHODN_isAscii METHODN_TO_GETTER(FIELDN_ascii)
#define METHODN_next    FIELDN_next
#define METHODN_substring FIELDN_substring
#define METHODN_new__array FIELDN_new__array
#define METHODN_opHas   FIELDN_ophas
#define METHODN_isLossLess METHODN_TO_GETTER(FIELDN_lossless)
#define METHODN_isInteractive METHODN_TO_GETTER(FIELDN_interactive)
#define METHODN_setProperty METHODN_TO_SETTER(FIELDN_property)
#define METHODN_this    FIELDN_this
#define METHODN_isSignificant METHODN_TO_GETTER(FIELDN_significant)
#define METHODN_new__array2D FIELDN_new__array2d
#define METHODN_setRandomSeed METHODN_TO_SETTER(FIELDN_randomseed)
#define METHODN_opLshift FIELDN_oplshift
#define METHODN_opEq    FIELDN_opeq
#define METHODN_setDebug METHODN_TO_SETTER(FIELDN_debug)
#define METHODN_isSynonym METHODN_TO_GETTER(FIELDN_synonym)
#define METHODN_isShared METHODN_TO_GETTER(FIELDN_shared)
#define METHODN_toLower FIELDN_tolower
#define METHODN_op0     FIELDN_op0
#define METHODN_op1     FIELDN_op1
#define METHODN_isPublic METHODN_TO_GETTER(FIELDN_public)
#define METHODN__c      METHODN_TO_MOVTEXT(FIELDN_c)
#define METHODN__d      METHODN_TO_MOVTEXT(FIELDN_d)
#define METHODN_getInt  METHODN_TO_GETTER(FIELDN_int)
#define METHODN_opDiv   FIELDN_opdiv
#define METHODN__k      METHODN_TO_MOVTEXT(FIELDN_k)
#define METHODN_hasFile FIELDN_hasfile
#define METHODN_get     METHODN_TO_GETTER(FIELDN_)
#define METHODN_intToFloatBits FIELDN_inttofloatbits
#define METHODN__p      METHODN_TO_MOVTEXT(FIELDN_p)
#define METHODN__s      METHODN_TO_MOVTEXT(FIELDN_s)
#define METHODN__u      METHODN_TO_MOVTEXT(FIELDN_u)
#define METHODN_indexOf FIELDN_indexof
#define METHODN_reverse FIELDN_reverse
#define METHODN_match   FIELDN_match
#define METHODN__x      METHODN_TO_MOVTEXT(FIELDN_x)
#define METHODN_getOut  METHODN_TO_GETTER(FIELDN_out)
#define METHODN_set2D   METHODN_TO_SETTER(FIELDN_2d)
#define METHODN_opLand  FIELDN_opland
#define METHODN_domain  FIELDN_domain
#define METHODN_isVerbose METHODN_TO_GETTER(FIELDN_verbose)
#define METHODN_getInputStream METHODN_TO_GETTER(FIELDN_inputstream)
#define METHODN_opRshift FIELDN_oprshift
#define METHODN_hashCode FIELDN_hashcode
#define METHODN_clear   FIELDN_clear
#define METHODN_set     METHODN_TO_SETTER(FIELDN_)
#define METHODN_getIn   METHODN_TO_GETTER(FIELDN_in)
#define METHODN_isStoringBuffer METHODN_TO_GETTER(FIELDN_storingbuffer)
#define METHODN_println FIELDN_println
#define METHODN_isImmutable METHODN_TO_GETTER(FIELDN_immutable)
#define METHODN_isRelease METHODN_TO_GETTER(FIELDN_release)
#define METHODN_invoke  FIELDN_invoke
#define METHODN_test    FIELDN_test
#define METHODN_typeof  FIELDN_typeof
#define METHODN_setModified METHODN_TO_SETTER(FIELDN_modified)
#define METHODN_opMod   FIELDN_opmod
#define METHODN_setVerbose METHODN_TO_SETTER(FIELDN_verbose)
#define METHODN_trim    FIELDN_trim
#define METHODN_isDebug METHODN_TO_GETTER(FIELDN_debug)
#define METHODN_getOutputStream METHODN_TO_GETTER(FIELDN_outputstream)
#define METHODN_setEncoding METHODN_TO_SETTER(FIELDN_encoding)
#define METHODN_swap    FIELDN_swap
#define METHODN_writeData FIELDN_writedata
#define METHODN_isFinal METHODN_TO_GETTER(FIELDN_final)
#define METHODN_opNeq   FIELDN_opneq
#define METHODN_random  FIELDN_random
#define METHODN__empty  METHODN_TO_MOVTEXT(FIELDN_empty)
#define METHODN_isUnique METHODN_TO_GETTER(FIELDN_unique)
#define METHODN_toUpper FIELDN_toupper
#define METHODN_opSub   FIELDN_opsub
#define METHODN_opGt    FIELDN_opgt
#define METHODN_opLor   FIELDN_oplor
#define METHODN_opInstanceof FIELDN_opinstanceof
#define METHODN_opNeg   FIELDN_opneg
#define METHODN_new__range FIELDN_new__range
#define METHODN_getErr  METHODN_TO_GETTER(FIELDN_err)
#define METHODN_getClass METHODN_TO_GETTER(FIELDN_class)
#define METHODN_isLogging METHODN_TO_GETTER(FIELDN_logging)
#define METHODN_opCase  FIELDN_opcase
#define METHODN_opPrev  FIELDN_opprev
#define METHODN_isVirtual METHODN_TO_GETTER(FIELDN_virtual)
#define METHODN_getEncoding METHODN_TO_GETTER(FIELDN_encoding)
#define METHODN_isLocal METHODN_TO_GETTER(FIELDN_local)
#define METHODN_isAutoFlush METHODN_TO_GETTER(FIELDN_autoflush)
#define METHODN_remove  FIELDN_remove
#define METHODN_listDir FIELDN_listdir
#define METHODN_unlink  FIELDN_unlink
#define METHODN_setShared METHODN_TO_SETTER(FIELDN_shared)
#define METHODN_isPrivate METHODN_TO_GETTER(FIELDN_private)
#define METHODN_opInTo  FIELDN_opinto
#define METHODN_concat  FIELDN_concat
#define METHODN_isStatic METHODN_TO_GETTER(FIELDN_static)
#define METHODN_isTemporal METHODN_TO_GETTER(FIELDN_temporal)
#define METHODN_readLine FIELDN_readline
#define METHODN_isConst METHODN_TO_GETTER(FIELDN_const)
#define METHODN_default FIELDN_default
#define METHODN_setConst METHODN_TO_SETTER(FIELDN_const)
#define METHODN_isPartial METHODN_TO_GETTER(FIELDN_partial)
#define METHODN_get2D   METHODN_TO_GETTER(FIELDN_2d)
#define METHODN_mkdir   FIELDN_mkdir
#define METHODN_opRangeUntil FIELDN_oprangeuntil
#define METHODN_isNotNull METHODN_TO_GETTER(FIELDN_notnull)
#define METHODN_opRangeTo FIELDN_oprangeto
#define METHODN_endsWith__IgnoreCase FIELDN_endswith__ignorecase
#define METHODN_pop     FIELDN_pop
#define METHODN_twofold FIELDN_twofold
#define METHODN_rename  FIELDN_rename
#define METHODN_hasDir  FIELDN_hasdir
#define METHODN_close   FIELDN_close
#define METHODN_isNull  METHODN_TO_GETTER(FIELDN_null)
#define METHODN_isSingleton METHODN_TO_GETTER(FIELDN_singleton)
#define METHODN_startsWith__IgnoreCase FIELDN_startswith__ignorecase
#define METHODN_isVarArgs METHODN_TO_GETTER(FIELDN_varargs)
#define METHODN__f      METHODN_TO_MOVTEXT(FIELDN_f)
#define METHODN_exit    FIELDN_exit
#define METHODN_setAutoFlush METHODN_TO_SETTER(FIELDN_autoflush)
#define METHODN__bits   METHODN_TO_MOVTEXT(FIELDN_bits)
#define METHODN_opGte   FIELDN_opgte
#define METHODN_set3D   METHODN_TO_SETTER(FIELDN_3d)
#define METHODN_times   FIELDN_times
#define METHODN_setMethodCompilationListener METHODN_TO_SETTER(FIELDN_methodcompilationlistener)
#define METHODN_putChar FIELDN_putchar
#define METHODN_lastIndexOf FIELDN_lastindexof
#define METHODN_opIs    FIELDN_opis
#define METHODN_isInterface METHODN_TO_GETTER(FIELDN_interface)
#define METHODN_clearBuffer FIELDN_clearbuffer
#define METHODN_getChar METHODN_TO_GETTER(FIELDN_char)
#define METHODN_isStrict METHODN_TO_GETTER(FIELDN_strict)
#define METHODN_listProperties FIELDN_listproperties
#define METHODN_write   FIELDN_write
#define METHODN_replace FIELDN_replace
#define METHODN__man    METHODN_TO_MOVTEXT(FIELDN_man)
#define METHODN_getKey  METHODN_TO_GETTER(FIELDN_key)
#define METHODN_opAs    FIELDN_opas

/* ======================================================================== */
/* TS */
#define TS_EMPTY        (ctx->share)->tString[0]
#define TS_TAB          (ctx->share)->tString[1]
#define TS_EOL          (ctx->share)->tString[2]
#define TS_BEGIN        (ctx->share)->tString[3]
#define TS_END          (ctx->share)->tString[4]
#define TS_true         (ctx->share)->tString[5]
#define TS_false        (ctx->share)->tString[6]
#define TS_null         (ctx->share)->tString[7]
#define TS_void         (ctx->share)->tString[8]
#define TS_main         (ctx->share)->tString[9]
#define TS_ENCODING     (ctx->share)->tString[10]
#define TS_DEVNULL      (ctx->share)->tString[11]
#define TS_DEVSTDIN     (ctx->share)->tString[12]
#define TS_DEVSTDERR    (ctx->share)->tString[13]
#define TS_DEVSTDOUT    (ctx->share)->tString[14]
#define TS_ATlabel      (ctx->share)->tString[15]
#define TS_Comment      (ctx->share)->tString[16]
#define TS_AssertionException (ctx->share)->tString[17]
#define TS_LP           (ctx->share)->tString[18]
#define TS_RP           (ctx->share)->tString[19]
#define TS_LB           (ctx->share)->tString[20]
#define TS_RB           (ctx->share)->tString[21]
#define TS_LS           (ctx->share)->tString[22]
#define TS_RS           (ctx->share)->tString[23]
#define KNH_TSTRING_SIZE 24
