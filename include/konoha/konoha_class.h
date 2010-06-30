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

#ifndef CLASS_KONOHA_H_
#define CLASS_KONOHA_H_

#include"konoha_t.h"
#include"konoha_name.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */
/* flag */
/* ------------------------------------------------------------------------ */
//## @Cyclic class Object Object knh_ObjectField_t;
//## flag Object Release!Debug 0 (%s)->h.flag is *  is *;
//## flag Object Immutable     1 (%s)->h.flag is *  is *;
//## flag Object NullObject    2 (%s)->h.flag is set * *;
//## flag Object Cyclic        3 (%s)->h.flag is set * *;
//## flag Object Modified      4 (%s)->h.flag is set is set;
//## flag Object Shared        5 (%s)->h.flag is set is set;
//## flag Object Local1       12 (%s)->h.flag is set *  *;
//## flag Object Local2       13 (%s)->h.flag is set *  *;
//## flag Object Local3       14 (%s)->h.flag is set *  *;
//## flag Object Local4       15 (%s)->h.flag is set *  *;

#ifndef K_FIELDSIZE
#define K_FIELDSIZE 64
#endif

typedef struct knh_ObjectField_t {
	knh_hObject_t h;
	Object  **fields;
	size_t  bsize;
} knh_ObjectField_t ;

#define IS_NULL(o)     (((o)->h.flag & FLAG_Object_NullObject) == FLAG_Object_NullObject)
#define IS_NOTNULL(o)  (((o)->h.flag & FLAG_Object_NullObject) != FLAG_Object_NullObject)

#define CLASS_ObjectField   CLASS_Object
#define knh_Object_cid(o)   (o)->h.cid
#define knh_Object_bcid(o)  (o)->h.bcid
#define knh_Object_p1(o)    knh_class_p1((o)->h.cid)

#define new_H(C)         (knh_##C##_t*)new_hObject_(ctx, 0, CLASS_##C, CLASS_##C)
#define new_(C)          (knh_##C##_t*)new_Object_init2(ctx, 0, CLASS_##C, CLASS_##C)
#define new_O(C,cid)     (knh_##C##_t*)new_Object_init2(ctx, 0, CLASS_##C, cid)

/* ------------------------------------------------------------------------ */
//## @Immutable class Boolean Object;

typedef knh_uint64_t   knh_ndata_t;

typedef struct {
	union {
		knh_bool_t    bvalue;
		knh_int_t     ivalue;
		knh_float_t   fvalue;
		knh_ndata_t   data;
	};
} knh_nObject_t ;

typedef struct knh_Boolean_t {
	knh_hObject_t h;
	knh_nObject_t n;
} knh_Boolean_t;

#define IS_TRUE(o)         ((o)->h.bcid == CLASS_Boolean && N_tobool(o))
#define IS_FALSE(o)        ((o)->h.bcid == CLASS_Boolean && N_tobool(o) == 0)
#define new_Boolean(ctx, c)    ((c) ? KNH_TRUE : KNH_FALSE)

/* ------------------------------------------------------------------------ */
//## class Number Object;

typedef struct knh_Number_t {
	knh_hObject_t h;
	knh_nObject_t n;
} knh_Number_t;

#define N_toint(o)         (((knh_Number_t*)o)->n.ivalue)
#define N_tofloat(o)       (((knh_Number_t*)o)->n.fvalue)
#define N_tobool(o)        (((knh_Number_t*)o)->n.bvalue)

/* ------------------------------------------------------------------------ */
//## @Immutable class Int Number;

typedef struct knh_Int_t {
	knh_hObject_t h;
	knh_nObject_t n;
} knh_Int_t;

/* ------------------------------------------------------------------------ */
//## @Immutable class Float Number;

typedef struct knh_Float_t {
	knh_hObject_t h;
	knh_nObject_t n;
} knh_Float_t;

/* ------------------------------------------------------------------------ */
//## @Immutable class String Object;
//## flag String TextSgm 1 - is set  *   *;
//## flag String ASCII   2 - is set  is  *;

typedef const char*   knh_TEXT_t;

typedef struct knh_String_t {
	knh_hObject_t h;
	knh_bytes_t str;
	struct knh_String_t *memoNULL;
} knh_String_t;

#define new_T(msg)          new_TEXT(ctx, CLASS_String, msg)
#define new_S(ctx, msg)     new_String_(ctx, CLASS_String, msg, NULL)

#define S_tobytes(s)          ((s)->str)
#define S_tochar(s)           (char*)((s)->str.buf)
#define S_size(s)             ((s)->str.len)
#define S_equals(s, b)        knh_bytes_equals(S_tobytes(s), b)
#define S_startsWith(s, b)    knh_bytes_startsWith(S_tobytes(s), b)
#define S_endsWith(s, b)      knh_bytes_endsWith(S_tobytes(s), b)

/* ------------------------------------------------------------------------ */
//## class Bytes Object;
//## flag Bytes Static 1 - is set * *;

typedef struct {
	size_t capacity;
	size_t wsize;
} knh_hmem_t;

typedef struct knh_Bytes_t {
	knh_hObject_t h;
	knh_bytes_t   bu;
	size_t capacity;
} knh_Bytes_t ;

//#define KNH_SIZE(v)     knh_size(v)
#define KNH_SIZE(v)     v

#define BA_tobytes(o)   (o)->bu
#define BA_size(o)      (o)->bu.len
#define BA_tochar(o)    ((char*)(o)->bu.buf)

#define B_equals(b, t)        (knh_bytes_strcmp(b, STEXT(t)) == 0)
#define B_startsWith(b, t)    knh_bytes_startsWith(b, STEXT(t))
#define B_endsWith(b, t)      knh_bytes_endsWith(b, STEXT(t))

/* ------------------------------------------------------------------------ */
//## @Cyclic class Any Object knh_Any_t;
//## @Cyclic @TypeVariable class Tvoid Any Any;
//## @Cyclic @TypeVariable class Tvar  Any Any;

struct knh_RawPtr_t;
typedef void (*knh_FfreeRawPtr)(Ctx *, struct knh_RawPtr_t *o);

typedef struct knh_RawPtr_t {
	knh_hObject_t h;
	void *ptr;
	knh_FfreeRawPtr pfree;
} knh_RawPtr_t ;

#define CLASS_RawPtr    CLASS_Any

typedef knh_RawPtr_t knh_Any_t;

/* ------------------------------------------------------------------------ */
//## @Immutable @Struct @Param1(Any) class Iterator Object;
//## cparam Iterator 0 T Any;

typedef void (*knh_Ffree)(void *ptr);

typedef struct {
	knh_Fitrnext fnext;
	Object* source;
	knh_int_t  pos;
	union {
		void*   ref;
		struct knh_opset_t *pc; /* @see(Generator) */
	};
	knh_Ffree freffree;
} knh_IteratorEX_t;

typedef struct knh_Iterator_t {
	knh_hObject_t h;
	knh_IteratorEX_t *b;
	knh_Fitrnext fnext_1;
} knh_Iterator_t;

/* ------------------------------------------------------------------------ */
//## @Immutable class Tuple Object;
//## cparam Tuple 0 T Any U Any;
//## flag Tuple Triple 1 - is set * *;
//## type PairSS Tuple 0 String String;
//## type PairST1 Tuple 0 String T1;
//## type PairT1T2 Tuple 0 T1 T2;

typedef struct knh_Tuple_t {
	knh_hObject_t h;
	union {
		knh_Object_t  *first;
		knh_Object_t  **list;
	};
	union {
		knh_Object_t  *second;
		size_t         size;
	};
	knh_Object_t  *thirdNULL;
} knh_Tuple_t;

/* ------------------------------------------------------------------------ */
//## @Immutable class Range Object;
//## cparam Range 0 T Any;
//## flag Range Inclusive 1 - is set * *;

typedef struct knh_Range_t {
	knh_hObject_t h;
	knh_Object_t  *start;
	knh_Object_t  *end;
} knh_Range_t;

/* ------------------------------------------------------------------------ */
//## @Cyclic class Array Object;
//## cparam Array 0 T Any;
//## flag Array NDATA     1 - is set * *;
//## flag Array SORTED    2 - is set * *;
//## flag Array Dimension 3 - has set * *;
//## type Cmpr Func 1 T1! T1! Int;

typedef METHOD   (*knh_Farray_get)(Ctx *ctx, knh_sfp_t *sfp, long rix);
typedef METHOD   (*knh_Farray_set)(Ctx *ctx, knh_sfp_t *sfp, long rix);
typedef void     (*knh_Farray_add)(Ctx *ctx, struct knh_Array_t *, knh_sfp_t *sfp);

typedef struct {
	knh_Farray_get get;
	knh_Farray_add add;
	knh_Farray_set set;
} knh_ArrayAPI_t;

typedef struct knh_Array_t {
	knh_hObject_t h;
	union {
		knh_hmem_t             *hmem;   // header pointer
		knh_ndata_t            *nlist;
		knh_int_t              *ilist;
		knh_float_t            *flist;
		struct knh_Object_t       **list;
		struct knh_Method_t       **methods;
		struct knh_Translator_t   **trans;
		struct knh_Token_t        **tokens;
		struct knh_KLRInst_t      **insts;
	};
	size_t size;
	knh_ArrayAPI_t *api;
} knh_Array_t;

#define knh_Array_n(a,n)      (a)->list[(n)]
#define knh_Array_size(a)     (a)->size
#define knh_Array_trimSize(ctx, a, newsize)  knh_Array_clear(ctx, a, newsize)
#define knh_Array_add(ctx, a, o)    knh_Array_add_(ctx, a, UP(o))

#define knh_TOKENs_n(a, n)    ((knh_Token_t*)(a)->list[(n)])
#define knh_KLRINSTs_n(a, n)  ((knh_KLRInst_t*)(a)->list[(n)])

///* ------------------------------------------------------------------------ */
//## @Cyclic class Map Object;
//## cparam DictMap 0 V Any;

typedef struct knh_dentry_t {
	union {
		knh_String_t  *key;
		knh_intptr_t   ikey;
		knh_floatptr_t fkey;
	};
	union {
		Object *value;
		knh_intptr_t ivalue;
		knh_floatptr_t fvalue;
	};
} knh_dentry_t;

typedef struct knh_dmap_t {
	size_t size;
	size_t sorted;
} knh_dmap_t ;

typedef struct knh_hentry_t {
	knh_hashcode_t hcode;
	union {
		Object *key;
		knh_ndata_t dkey;
	};
	union {
		Object *value;
		knh_ndata_t dvalue;
	};
	struct knh_hentry_t *next;
} knh_hentry_t;

typedef struct knh_hmap_t {
	size_t hmax;
	size_t size;
	size_t factor;
} knh_hmap_t;

typedef void  knh_map_t;

typedef struct knh_Map_t {
	knh_hObject_t h;
	union {
		knh_map_t     *map;
		knh_dmap_t    *dmap;
		knh_hmap_t    *hmap;
	};
	const struct knh_MapDSPI_t *dspi;
} knh_Map_t;

#define K_HASH_INITSIZE 83
#define KNH_HASH_FACTOR(i)     ((i * 4) / 3)
#define knh_Map_get(ctx, m, k)     m->dspi->getkey(ctx, m, k)

// DictMap, DictSet are old names of Map;

typedef struct knh_DictMap_t {
	knh_hObject_t h;
	union {
		knh_map_t     *map;
		knh_dmap_t    *dmap;
		knh_dentry_t  *dlistO;
	};
	const struct knh_MapDSPI_t *dspi;
} knh_DictMap_t;

typedef struct knh_DictCaseMap_t {
	knh_hObject_t h;
	union {
		knh_map_t     *map;
		knh_dmap_t    *dmap;
		knh_dentry_t  *dlistO;
	};
	const struct knh_MapDSPI_t *dspi;
} knh_DictCaseMap_t;

#define knh_DictMap_size(m)    ((m)->dmap)->size
#define knh_DictCaseMap_size(m)    ((m)->dmap)->size
#define knh_DictMap_set(ctx, m, k, v)      knh_DictMap_set_(ctx, m, k, UP(v))
#define knh_DictCaseMap_set(ctx, m, k, v)  knh_DictCaseMap_set_(ctx, m, k, UP(v))

typedef struct knh_DictSet_t {
	knh_hObject_t h;
	union {
		knh_map_t     *map;
		knh_dmap_t    *dmap;
		knh_dentry_t  *dlistI;
	};
	const struct knh_MapDSPI_t *dspi;
} knh_DictSet_t;

typedef struct knh_DictCaseSet_t {
	knh_hObject_t h;
	union {
		knh_map_t     *map;
		knh_dmap_t    *dmap;
		knh_dentry_t  *dlistI;
	};
	const struct knh_MapDSPI_t *dspi;
} knh_DictCaseSet_t;

typedef void (*knh_Fdictset)(Ctx *, knh_DictCaseSet_t*, knh_String_t *k, knh_uintptr_t);
#define knh_DictSet_size(m)      ((m)->dmap)->size
#define knh_DictCaseSEt_size(m)  ((m)->dmap)->size

/* ------------------------------------------------------------------------ */
//## @Immutable class Class Object;
//## flag Class Release!Debug  0 (pClassTable(ctx,%s))->cflag is * is *;
//## flag Class Immutable      1 (pClassTable(ctx,%s))->cflag is * is *;
//## flag Class Cyclic         2 (pClassTable(ctx,%s))->cflag is set * *;
//## flag Class MetaClass      3 (pClassTable(ctx,%s))->cflag is * is *;
//## flag Class Private!Public 4 (pClassTable(ctx,%s))->cflag is * is *;
//## flag Class Final          5 (pClassTable(ctx,%s))->cflag  is * is *;
//## flag Class Singleton      6 (pClassTable(ctx,%s))->cflag  is * is *;
//## flag Class Unique         7 (pClassTable(ctx,%s))->cflag  is * is *;
//## flag Class Interface      8 (pClassTable(ctx,%s))->cflag  is * is *;
//## flag Class TypeVariable   9 (pClassTable(ctx,%s))->cflag  is set * *;

#define FLAG_Field_Hidden          (knh_flag_t)(1<<0)
#define FLAG_Field_Protected       (knh_flag_t)(1<<1)
#define FLAG_Field_Getter          (knh_flag_t)(1<<2)
#define FLAG_Field_Setter          (knh_flag_t)(1<<3)
#define FLAG_Field_Key             (knh_flag_t)(1<<4)
#define FLAG_Field_Volatile        (knh_flag_t)(1<<5)
#define FLAG_Field_ReadOnly        (knh_flag_t)(1<<6)
#define FLAG_Field_Property        (knh_flag_t)(1<<7)
#define FLAG_Field_Principle       (knh_flag_t)(1<<8)

#define FLAG_GAMMA_Register    FLAG_Field_Getter
#define FLAG_GAMMA_FuncScope   FLAG_Field_Setter

typedef struct knh_Class_t {
	knh_hObject_t h;
	knh_class_t   cid;
	knh_type_t    type;
} knh_Class_t;

/* class */

#define KNH_NULVAL(cid)  knh_getClassDefaultValue(ctx, cid)
#define KNH_TNULL(T)      (knh_##T##_t*)knh_getClassDefaultValue(ctx, CLASS_##T)

#define knh_Class_cid(c)     (knh_class_t)(c)->cid
#define FLAG_oflag(f)        (f)

typedef void (*knh_Fwritecid)(Ctx *ctx, struct knh_OutputStream_t *w, knh_class_t cid);

/* ------------------------------------------------------------------------ */
//## @Private class ParamArray Object;
//## flag ParamArray VARGs  1 - is set * *;
//## flag ParamArray RVARGs 2 - is set * *;

typedef struct {
	knh_type_t    type;  knh_fieldn_t  fn;
} knh_param_t ;

typedef struct knh_ParamArray_t {
	knh_hObject_t h;
	knh_ushort_t psize; knh_ushort_t rsize;
	union {
		knh_param_t* params;
		knh_param_t  p0;
	};
	union {
		knh_param_t p1;
		size_t capacity;
	};
	knh_param_t p2;
} knh_ParamArray_t;

#define new_ParamArray(ctx)         new_(ParamArray);
#define knh_param_hash(h, t, fn)    (fn + (((h * 31) + t) * 7))
#define knh_ParamArray_rget(pa, n)  knh_ParamArray_get(pa, (pa->psize) + n)

/* ------------------------------------------------------------------------ */
//## @Struct class Method Object;
//## flag Method Private!Public  0 DP(%s)->flag is set is *;
//## flag Method Virtual!Final   1 DP(%s)->flag is set is *;
//## flag Method Debug           2 DP(%s)->flag is set * *;
//## flag Method Const           3 DP(%s)->flag is * is *;
//## flag Method Static          4 DP(%s)->flag is * is *;
//## flag Method ObjectCode      5 DP(%s)->flag is set * *;
//## flag Method Hidden          6 DP(%s)->flag is set * *;
//## flag Method Dynamic         7 DP(%s)->flag is set * *;
//## flag Method Smart           8 DP(%s)->flag is set * *;
//## flag Method Tricky          9 DP(%s)->flag is set * *;

typedef struct {
	knh_flag_t     flag;  knh_ushort_t   delta;
	knh_class_t    cid;   knh_methodn_t  mn;
	knh_uri_t      uri;   knh_uri_t      domain;
	struct knh_ParamArray_t* mp;
	knh_Fmethod       fproceed;
	union {
		void*             code;
		struct knh_String_t  *source;
		struct knh_KLRCode_t *kcode;
	};
} knh_MethodEX_t;

typedef struct knh_Method_t {
	knh_hObject_t h;
	knh_MethodEX_t *b;
	knh_Fmethod fcall_1;
	struct knh_opset_t *pc_start;
} knh_Method_t;

#define knh_Method_mn(mtd)    DP(mtd)->mn
#define knh_Method_mf(mtd)    DP(mtd)->mp
#define knh_ParamArray_psize(mf)    ((mp)->psize)
#define knh_Method_rsize(mtd)       ((DP(mtd)->mp)->rsize)
#define knh_Method_psize(mtd)       ((DP(mtd)->mp)->psize)

//#define knh_Method_pztype(mtd, n) (knh_ParamArray_get(DP(mtd)->mp, n))->type

#define knh_Method_ptype(ctx, mtd, cid, n) \
	knh_type_tocid(ctx, (knh_ParamArray_get(DP(mtd)->mp, n))->type, cid)

#define knh_getMethodNULL(ctx, c, mn)    knh_findMethodNULL(ctx, c, mn, 0)
#define knh_findMethod(ctx, c, mn)   knh_findMethodNULL(ctx, c, mn, 1)
#define knh_getDefaultFormatter(ctx, mn)   knh_lookupFormatter(ctx, CLASS_Tvoid, mn)

#define knh_stack_argc(ctx, sfp)      (ctx->esp - (sfp))

#define KNH_NOTRACE         0
#define KNH_SECURITYTRACE   1
#define KNH_AUDITTRACE      2
#define KNH_PROFILER        3
#define KNH_STACKTRACE      4
#define KNH_FUNCTIONTRACE   5

/* ------------------------------------------------------------------------ */
//## @Struct class Translator Object;
//## flag Translator Interface        0 DP(%s)->flag is set is *;
//## flag Translator Significant      1 DP(%s)->flag is set is *;
//## flag Translator Semantic         2 DP(%s)->flag is * is *;
//## flag Translator Total!Partial    3 DP(%s)->flag is set is *;
//## flag Translator LossLess         4 DP(%s)->flag is set is *;
//## flag Translator Final            6 DP(%s)->flag is set is *;
//## flag Translator Const!Temporal   7 DP(%s)->flag is set is *;
//## flag Translator MapMap           8 DP(%s)->flag is set * *;

typedef struct {
	knh_flag_t   flag;
	union {
		Object* mapdata;
		struct knh_Method_t *mtd;
		struct knh_Translator_t *trl1;
	};
	struct knh_Translator_t *trl2;
} knh_TranslatorEX_t;

typedef struct knh_Translator_t {
	knh_hObject_t h;
	knh_TranslatorEX_t *b;
	knh_Ftranslator ftcast_1;
	knh_class_t  scid; knh_class_t  tcid;
} knh_Translator_t;

#define knh_findTranslator(ctx, scid, tcid)     knh_findTranslatorNULL(ctx, scid, tcid, 1)
#define knh_getTranslatorNULL(ctx, scid, tcid)  knh_findTranslatorNULL(ctx, scid, tcid, 0)

/* ------------------------------------------------------------------------ */
//## class Func Object;
//## cparam Func 1 R Any P Any;
//## flag Func StoredEnv 1 - is set * *;

typedef struct knh_Func_t {
	knh_hObject_t h;
	Object* baseNULL;
	struct knh_Method_t* mtd;
	void *reserved;
//	union {
//		knh_sfp_t *envsfp;
//		size_t *hstacksize;
//	};
} knh_Func_t ;

/* ------------------------------------------------------------------------ */
//## @Param1(Any) class Thunk Object;
//## flag Thunk Evaluated 1 - is set * *;

typedef struct knh_Thunk_t {
	knh_hObject_t h;
	knh_sfp_t *envsfp;
	size_t envsize;
} knh_Thunk_t;

/* ------------------------------------------------------------------------ */
//## @Struct class Exception Object;
//## flag Exception Logging 0 DP(%s)->flag is * is *;
//## flag Exception Caught  1 DP(%s)->flag is set * *;

//## expt Exception!! -;
//## expt Fatal!! -;
//## expt OutOfMemory!! Fatal!!;
//## expt Security!! -;
//## expt Null!! -;
//## expt OutOfIndex!! -;
//## expt IO!! -;
//## expt Script!! -;
//## expt Syntax!! Script!!;
//## expt Type!! Script!!;
//## expt Assertion!! Script!!;

typedef struct {
	knh_flag_t flag;
	knh_expt_t eid;
	knh_String_t* msg;
	Object* bag;
	knh_Array_t* traces;
	char*     file;
	char*     Cfile;
	int       line;
	int       Cline;
} knh_ExceptionEX_t;

typedef struct knh_Exception_t {
	knh_hObject_t h;
	knh_ExceptionEX_t *b;
} knh_Exception_t;

#define new_Exception__s(ctx, s)     new_Exception__T(ctx, B(s))

/* ------------------------------------------------------------------------ */
//## @Private class ExceptionHandler Object;
//## flag ExceptionHandler Jumpable   1 - is set * *;
//## flag ExceptionHandler Catching   2 - is set * *;

#ifdef K_USING_POSIX
typedef sigjmp_buf knh_jmpbuf_t;
#define knh_setjmp(buf)         sigsetjmp(buf, 1)
#define knh_longjmp(buf, val)   siglongjmp(buf, val)
#elif defined(KONOHA_ON_LKM)
typedef jmp_buf knh_jmpbuf_t;
#else
typedef jmp_buf knh_jmpbuf_t;
#define knh_setjmp(buf)         setjmp(buf)
#define knh_longjmp(buf, val)   longjmp(buf, val)
#endif

typedef struct knh_ExceptionHandler_t {
	knh_hObject_t h;
	struct knh_opset_t *pc;
	knh_ushort_t sfpidx;     knh_ushort_t espidx;
	struct knh_opset_t *vpc;
	knh_intptr_t vshift;
} knh_ExceptionHandler_t;

#define KNH_NOAPI(ctx, cwb, isThrowable) { \
		knh_throw_Unsupported(ctx, cwb, __FUNCTION__, __FILE__, __LINE__, isThrowable); \
	}\

#define KNH_THROW_OUTOFINDEX(ctx, n, max) { \
		knh_throw_OutOfIndex(ctx, n, max, __FILE__, __LINE__); \
	}\

/* ------------------------------------------------------------------------ */
//## @Immutable class Regex Object;

typedef void knh_regex_t;

typedef struct knh_Regex_t {
	knh_hObject_t h;
	knh_regex_t *reg;
	int eflags;
	const struct knh_RegexSPI_t *spi;
	struct knh_String_t *pattern;
} knh_Regex_t;

/* ------------------------------------------------------------------------ */
//## class Converter Object;

typedef void knh_conv_t;

typedef struct knh_Converter_t {
	knh_hObject_t h;
	knh_conv_t *conv;
	const struct knh_ConverterDSPI_t *dspi;
} knh_Converter_t;

/* ------------------------------------------------------------------------ */
//## class StringEncoder Object;

typedef struct knh_StringEncoder_t {
	knh_hObject_t h;
	knh_conv_t *conv;
	const struct knh_ConverterDSPI_t *dspi;
} knh_StringEncoder_t;

/* ------------------------------------------------------------------------ */
//## class StringDecoder Object;

typedef struct knh_StringDecoder_t {
	knh_hObject_t h;
	knh_conv_t *conv;
	const struct knh_ConverterDSPI_t *dspi;
} knh_StringDecoder_t;

/* ------------------------------------------------------------------------ */
//## class StringConverter Object;

typedef struct knh_StringConveter_t {
	knh_hObject_t h;
	knh_conv_t *conv;
	const struct knh_ConverterDSPI_t *dspi;
} knh_StringConverter_t;

/* ------------------------------------------------------------------------ */
//## @Struct @Private class Semantics Object;

#define KNH_SEMANTICS_FMT    "%s{%s}"
struct  knh_Semantics_t;

typedef int (*knh_Fichk)(struct knh_Semantics_t *, knh_int_t v);
typedef int (*knh_Ficmp)(struct knh_Semantics_t *, knh_int_t v1, knh_int_t v2);

typedef int (*knh_Ffchk)(struct knh_Semantics_t *, knh_float_t v);
typedef int (*knh_Ffcmp)(struct knh_Semantics_t *, knh_float_t v1, knh_float_t v2);

typedef knh_String_t *(*knh_Fsnew)(Ctx *, knh_class_t cid, knh_bytes_t, struct knh_String_t *, int *);
typedef int (*knh_Fscmp)(struct knh_Semantics_t *, knh_bytes_t, knh_bytes_t);

typedef struct {
	knh_flag_t  flag;
	knh_class_t ucid;
	knh_class_t ubcid;
	knh_String_t* urn;
	knh_String_t* tag;
	struct knh_Int_t    *ivalue;
	struct knh_Float_t  *fvalue;
	struct knh_String_t *svalue;

	// int
	union {
		knh_int_t imin;
		knh_uint_t umin;
	};
	union {
		knh_int_t imax;
		knh_uint_t umax;
	};
	knh_Fichk fichk;
	knh_Ficmp ficmp;

	// float
	knh_float_t fmin;
	knh_float_t fmax;
	knh_float_t fstep;
	knh_Ffchk   ffchk;
	knh_Ffcmp   ffcmp;

	// String
	size_t bytelen;
	size_t charlen;
	Object* pattern;
//	struct knh_DictIdx_t* vocabDictIdx;
	struct knh_StringEncoder_t *bconv;
	knh_Fsnew    fsnew;
	knh_Fscmp    fscmp;
} knh_SemanticsEX_t;

typedef struct knh_Semantics_t* (*knh_fspec)(Ctx *ctx, knh_bytes_t urn);

typedef struct knh_Semantics_t {
	knh_hObject_t h;
	knh_SemanticsEX_t *b;
} knh_Semantics_t;

/* ------------------------------------------------------------------------ */
/* InputStream, OutputStream */

#define KNH_STDIN          (ctx->in)
#define KNH_STDOUT         (ctx->out)
#define KNH_STDERR         (ctx->err)

/* ------------------------------------------------------------------------ */
//## @Struct class InputStream Object;
//## flag InputStream FILE 1 - is set * *;

typedef knh_intptr_t knh_io_t;
#define IO_NULL   ((knh_io_t)NULL)

typedef struct {
	union {
		knh_io_t fd;
		FILE *fp;
	};
	union {
		struct knh_Bytes_t *ba;
		struct knh_String_t *str;
	};
	char *buf;
	size_t bufpos;
	size_t bufend;
	size_t bufsiz;
	struct knh_StringDecoder_t* decNULL;
	knh_String_t*  urn;
	knh_uri_t  uri;
	knh_ushort_t  prev;
	size_t line;
	size_t size;
} knh_InputStreamEX_t;

typedef struct knh_InputStream_t {
	knh_hObject_t h;
	knh_InputStreamEX_t *b;
	const struct knh_StreamDSPI_t *dspi;
} knh_InputStream_t;

/* ------------------------------------------------------------------------ */
//## @Struct class OutputStream Object;
//## flag OutputStream BOL            1 - is set * *;
//## flag OutputStream AutoFlush      2 - is set is set;
//## flag OutputStream StoringBuffer  3 - is set is set;

typedef struct {
	knh_io_t fd;
	union {
		struct knh_Bytes_t *ba;
//		struct knh_String_t *str;
	};
	struct knh_StringEncoder_t* encNULL;
	knh_String_t*  urn;
	size_t size;
	size_t line;
	knh_String_t*  NEWLINE;
	knh_String_t*  TAB;
	knh_short_t indent;
	knh_uri_t   uri;
} knh_OutputStreamEX_t;

typedef struct knh_OutputStream_t {
	knh_hObject_t h;
	knh_OutputStreamEX_t *b;
	const struct knh_StreamDSPI_t *dspi;
} knh_OutputStream_t;

#ifdef K_EXPORTS
#define knh_putc(ctx, w, ch)     ctx->api->putc(ctx, w, ch)
#define knh_write(ctx, w, s, len)  ctx->api->write(ctx, w, s, len)
#else
#define knh_putc(ctx, w, ch)    knh_OutputStream_putc(ctx, w, ch)
#define knh_write(ctx, w, s)    knh_OutputStream_write(ctx, w, s)
#define knh_flush(ctx, w)       knh_OutputStream_flush(ctx, w)
#define knh_print(ctx, w, s)    knh_OutputStream_writeLine(ctx, w, s, 0)
#define knh_println(ctx, w, s)  knh_OutputStream_writeLine(ctx, w, s, 1)

#define knh_write_char(ctx, w, s) knh_write(ctx, w, B(s))

#define knh_write_delim(ctx, w)    knh_write(ctx, w, STEXT(", "))
#define knh_write_dots(ctx, w)     knh_write(ctx, w, STEXT("..."))
#define knh_write_fn(ctx, w, fn)   knh_write(ctx, w, B(FN_tochar(fn)))
#define knh_write_type(ctx, w, type)   knh_write_cid(ctx, w, type)
#define knh_write_ltype(ctx, w, type)  knh_write_cid(ctx, w, type)
#define knh_write__O(ctx, w, o)    knh_write_Object(ctx, w, MN__k, o)
#endif

/* ------------------------------------------------------------------------ */
//## @Struct class Channel Object;

typedef struct knh_Channel_t {
	knh_hObject_t h;
	knh_intptr_t sd;
	struct knh_String_t *urn;
	struct knh_InputStream_t  *in;
	struct knh_OutputStream_t *out;
} knh_Channel_t;

/* ======================================================================== */
//## class Connection Object;
//## @Struct class ResultSet Object;

typedef void   knh_qconn_t;
typedef void   knh_qcur_t;

typedef struct knh_Connection_t {
	knh_hObject_t h;
	knh_qconn_t              *conn;
	struct knh_QueryDPI_t    *dspi;
	knh_String_t             *urn;
} knh_Connection_t;

/* ------------------------------------------------------------------------ */

#define knh_ResultSet_CTYPE__null    0
#define knh_ResultSet_CTYPE__integer 1
#define knh_ResultSet_CTYPE__float   2
#define knh_ResultSet_CTYPE__text    3  /* UTF-8*/
#define knh_ResultSet_CTYPE__bytes   4
#define knh_ResultSet_CTYPE__Object  5

typedef struct {
	knh_type_t type;
	knh_ushort_t ctype;
	knh_String_t *name;
	size_t start;
	size_t len;
	int dbtype;
} knh_dbschema_t;

typedef struct {
	struct knh_Connection_t *conn;
	knh_qcur_t              *qcur;
	void   (*qcurfree)(knh_qcur_t *); /* necessary if conn is closed before */
	knh_String_t            *tableName;
	knh_class_t              tcid;
	knh_ushort_t             column_size;
	knh_dbschema_t          *column;
	knh_Bytes_t             *databuf;
	size_t                   count;
} knh_ResultSetEX_t;

typedef struct knh_ResultSet_t {
	knh_hObject_t h;
	knh_ResultSetEX_t *b;
} knh_ResultSet_t;


/* ------------------------------------------------------------------------ */
//## @Private @Singleton class Script Object;

typedef struct knh_Script_t {
	knh_hObject_t h;
	Object **fields;
	size_t fsizeUSED;
} knh_Script_t ;

//#define new_Script(ctx)   (knh_Script_t*)new_Object_init2(ctx, FLAG_Script, CLASS_Script, CLASS_Script)

/* ------------------------------------------------------------------------ */
//## @Struct class NameSpace Object;

#define KNH_PATH_TRUSTED      1
#define KNH_PATH_UNTRUSTED    2

typedef struct knh_NameSpace {
	knh_String_t*           nsname;
	struct knh_NameSpace_t  *parentNULL;
	struct knh_DictMap_t*   aliasDictMapNULL;
	struct knh_DictSet_t*   name2cidDictSetNULL;
	struct knh_DictSet_t*   func2cidDictSetNULL;
	struct knh_DictMap_t*   lconstDictCaseMapNULL;
	const struct knh_RegexSPI_t  *strregexSPI;
	const struct knh_RegexSPI_t  *regexSPI;
/*	struct knh_DictSet_t*   pathTrustDictSet; */
/*	struct knh_DictMap_t*   tag2urnDictMap; */
} knh_NameSpaceEX_t;

typedef struct knh_NameSpace_t {
	knh_hObject_t h;
	knh_NameSpaceEX_t *b;
} knh_NameSpace_t;

/* ------------------------------------------------------------------------ */
//## @Struct class Package Object;

typedef struct knh_Package_t {
	knh_hObject_t h;
	struct knh_NameSpace_t    *ns;
	struct knh_Script_t       *script;
	void *hdlr;
} knh_Package_t;

/* ------------------------------------------------------------------------ */
//## @Singleton @Struct class System Object;

typedef struct {
	knh_String_t *name;
	knh_methodn_t parent_mn;
	knh_methodn_t parentMF;
	knh_class_t   principle_mtd;
	knh_class_t   mtdfCID;
} 	knh_NameInfo_t ;

typedef struct {
	knh_uintptr_t sysid;
	size_t     ctxcount;

	struct knh_DictMap_t*      props;
	struct knh_InputStream_t*  in;
	struct knh_OutputStream_t* out;
	struct knh_OutputStream_t* err;
	struct knh_String_t*       enc;

	struct knh_DictSet_t       *tokenDictSet;
	struct knh_DictCaseSet_t   *nameDictCaseSet;
	union {
		knh_hmem_t            *hnameinfo;
		knh_NameInfo_t        *nameinfo;
	};
	struct knh_DictSet_t      *urnDictSet;
	struct knh_Array_t        *urns;

	struct knh_DictSet_t      *ClassNameDictSet;
	struct knh_DictCaseSet_t  *EventDictCaseSet;
	struct knh_DictMap_t      *PackageDictMap;
	struct knh_DictMap_t      *URNAliasDictMap;
	struct knh_DictSet_t      *dspiDictSet;
//	struct knh_DictSet_t *SpecFuncDictSet;
//	struct knh_Array_t   *UsingResources;
//	struct knh_DictMap_t *listenerDictMap;
//	struct knh_DictMap_t *trustedPathDictSet;
} knh_SystemEX_t;

typedef struct knh_System_t {
	knh_hObject_t h;
	knh_SystemEX_t *b;
} knh_System_t;

/* ------------------------------------------------------------------------ */
//## class Context Object;
//## flag Context Strict       0 ((knh_Context_t*)%s)->flag is set is set;
//## flag Context Debug        1 ((knh_Context_t*)%s)->flag is set is set;
//## flag Context Verbose      2 ((knh_Context_t*)%s)->flag is set is set;
//## flag Context Interactive  3 ((knh_Context_t*)%s)->flag is set is *;
//## flag Context Compiling    4 ((knh_Context_t*)%s)->flag is set * *;


#define KNH_SYS(ctx, n)    knh_getSystemConst(ctx, n)
#define KNH_SYS_CTX    0
#define KNH_SYS_CTXIN  1
#define KNH_SYS_CTXOUT 2
#define KNH_SYS_CTXERR 3
#define KNH_SYS_STDIN  4
#define KNH_SYS_STDOUT 5
#define KNH_SYS_STDERR 6
#define KNH_SYS_OS     7
#define KNH_SYS_SCRIPT 8

#define K_FLAG_PF_STDERR      K_FLAG_T0
#define K_FLAG_PF_EOL         K_FLAG_T1
#define K_FLAG_PF_TIME        K_FLAG_T2
#define K_FLAG_PF_FUNC        K_FLAG_T3
#define K_FLAG_PF_LINE        K_FLAG_T4
#define K_FLAG_PF_NAME        K_FLAG_T8
#define K_FLAG_PF_BOL         K_FLAG_T9

/* ------------------------------------------------------------------------ */
/* konohac.h */

/* ------------------------------------------------------------------------ */
//## @Struct class Term Object;

typedef knh_ushort_t   knh_term_t;

#define K_FLAG_KEYWORD      K_FLAG_T0
#define TT_KEYWORD(tt)      ((tt) | K_FLAG_KEYWORD)
#define TT_ISKEYWORD(tt)    (((tt) & K_FLAG_KEYWORD) == K_FLAG_KEYWORD)
#define TT_UNMASK(tt)       ((tt) & ~(K_FLAG_KEYWORD))

typedef struct {
	knh_flag_t flag0;
	knh_ushort_t dummy;
} knh_TermEX_t;

typedef struct knh_Term_t {
	knh_hObject_t h;
	knh_TermEX_t *b;
	knh_uri_t  uri;  knh_ushort_t line;
	knh_type_t type; knh_term_t  tt;
} knh_Term_t;

#define knh_Term_isTyped(o)     (SP(o)->type != TYPE_var)

/* ------------------------------------------------------------------------ */
//## @Struct class Token Term;
//## flag Token BOL           0 DP(%s)->flag0 is  set * *;
//## flag Token DOT           1 DP(%s)->flag0 is  set * *;
//## flag Token LCASE         2 DP(%s)->flag0 has set * *;
//## flag Token USCORE        3 DP(%s)->flag0 has set * *;
//## flag Token Getter        4 DP(%s)->flag0 is set * *;
//## flag Token Setter        5 DP(%s)->flag0 is set * *;
//## flag Token ExceptionType 6 DP(%s)->flag0 is set * *;
//## flag Token MEMO1         7 DP(%s)->flag0 is set * *;

// TT_NAME
#define knh_Token_isPNAME(o)  knh_Token_isMEMO1(o)
#define knh_Token_setPNAME(o, b) knh_Token_setMEMO1(o, b)

// TT_LOCAL,
#define knh_Token_isSUPER(o)      knh_Token_isMEMO1(o)
#define knh_Token_setSUPER(o, b)  knh_Token_setMEMO1(o, b)

// TT_LOCAL, TT_FIELD, TT_SCRIPT
#define knh_Token_isReadOnly(tk)   knh_Token_isBOL(tk)
#define knh_Token_setReadOnly(tk, b)   knh_Token_setBOL(tk, b)

typedef struct {
	knh_flag_t   flag0;
	union {
		knh_short_t   index;
		knh_class_t   cid;
		knh_methodn_t mn;
	};
	union {
		Object* data;
		struct knh_Array_t*  list;
		struct knh_String_t* text;
		struct knh_Token_t*  token;
		struct knh_Stmt_t*   stmt;
		struct knh_Method_t* mtd;
		struct knh_Translator_t* mpr;
		struct knh_Int_t *num;
	};
} knh_TokenEX_t;

typedef struct knh_Token_t {
	knh_hObject_t h;
	knh_TokenEX_t *b;
	knh_uri_t uri;   knh_ushort_t line;
	knh_type_t type; knh_term_t  tt;
} knh_Token_t;

#define knh_Token_isTyped(o)    (SP(o)->type != TYPE_var)
#define new_TokenCONST(ctx, d)  new_TokenCONST_(ctx, UP(d))
#define new_TermCONST(ctx, d)   TM(new_TokenCONST_(ctx, UP(d)))
#define knh_Token_setCONST(ctx, tk, d) knh_Token_setCONST_(ctx, tk, UP(d))

/* ------------------------------------------------------------------------ */
//## @Struct class Stmt Term;
//## flag Stmt EveryLine  0 DP(%s)->flag0 is set * *;
//## flag Stmt TailReturn 1 DP(%s)->flag0 is set * *;
//## flag Stmt Memo1      5 DP(%s)->flag0 is set * *;
//## flag Stmt Memo2      6 DP(%s)->flag0 is set * *;

/* STT_METHOD*/
#define knh_Stmt_isVARGs(s)      knh_Stmt_isMemo1(s)
#define knh_Stmt_setVARGs(s,b)   knh_Stmt_setMemo1(s,b)

/* STT_TCAST*/
#define knh_Stmt_isDOWNCAST(s)       knh_Stmt_isMemo1(s)
#define knh_Stmt_setDOWNCAST(s,b)    knh_Stmt_setMemo1(s,b)

/* STT_CALL*/
#define knh_Stmt_isTAILRECURSION(s)       knh_Stmt_isMemo1(s)
#define knh_Stmt_setTAILRECURSION(s,b)    knh_Stmt_setMemo1(s,b)

///* STT_DECL */
//#define knh_Stmt_isFuncScope(s)       knh_Stmt_isMemo1(s)
//#define knh_Stmt_setFuncScope(s,b)    knh_Stmt_setMemo1(s,b)
///* TCAST */
///* STT_BLOCK {} */
//#define knh_Stmt_isDEBUG(s)       knh_Stmt_isMemo1(s)
//#define knh_Stmt_setDEBUG(s,b)    knh_Stmt_setMemo1(s,b)
///* STT_TCAST */
//#define knh_Stmt_isNNCAST(s)       knh_Stmt_isMemo1(s)
//#define knh_Stmt_setNNCAST(s,b)    knh_Stmt_setMemo1(s,b)
///* STT_FOREACH */
//#define knh_Stmt_isMAPNEXT(s)      knh_Stmt_isMemo1(s)
//#define knh_Stmt_setMAPNEXT(s,b)   knh_Stmt_setMemo1(s,b)
///* STT_CALL*/
//#define knh_Stmt_isPROPN(s)      knh_Stmt_isMemo1(s)
//#define knh_Stmt_setPROPN(s,b)   knh_Stmt_setMemo1(s,b)

/* ------------------------------------------------------------------------ */

typedef struct {
	knh_flag_t   flag0;
	knh_ushort_t espidx;
	knh_ushort_t size;
	knh_ushort_t capacity;
	union {
		struct knh_Term_t**  terms;
		struct knh_Token_t** tokens;
		struct knh_Stmt_t**  stmts;
	};
	union {
		struct knh_DictCaseMap_t* metaDictCaseMap;
		struct knh_String_t*  errMsg;
		struct knh_Stmt_t*    stmtPOST;
	};
	struct knh_Stmt_t* nextNULL;
} knh_StmtEX_t;

typedef struct knh_Stmt_t {
	knh_hObject_t h;
	knh_StmtEX_t *b;
	knh_uri_t uri; knh_ushort_t line;
	knh_type_t type; knh_term_t  stt;
} knh_Stmt_t;

#define knh_Stmt_isTyped(o)        (SP(o)->type != TYPE_var)

/* ------------------------------------------------------------------------ */
//## @Struct @Private class Gamma Object;
//## flag Gamma Cancelled  0 DP(%s)->flag is  set   * *;
//## flag Gamma Quiet      1 DP(%s)->flag is  set   * *;
//## flag Gamma Throwable  2 DP(%s)->flag is  set   * *;
//## flag Gamma PROCEED    3 DP(%s)->flag has found * *;
//## flag Gamma RETURN     4 DP(%s)->flag has found * *;
//## flag Gamma YEILD      5 DP(%s)->flag has found * *;
//## flag Gamma FIELD      6 DP(%s)->flag has found * *;
//## flag Gamma STACK      7 DP(%s)->flag has found * *;
//## flag Gamma SCRIPT     7 DP(%s)->flag has found * *;

//## flag Gamma InlineFunction  0 DP(%s)->cflag is set * *;
//## flag Gamma TailRecursion   1 DP(%s)->cflag is set * *;

#ifndef K_GAMMASIZE
#define K_GAMMASIZE 64
#endif

#define knh_getGammaScript(ctx)   DP(ctx->gma)->script
#define knh_getGammaNameSpace(ctx)   DP(ctx->gma)->ns

#define SCOPE_PARAM       0
#define SCOPE_FOREACH     1
#define SCOPE_LOCAL       2
#define SCOPE_FIELD       3
#define SCOPE_SCRIPT      4

typedef struct {
	knh_type_t type;
	knh_short_t pn;
	union {
		knh_Token_t *token;
		knh_Stmt_t *stmt;
	};
} knh_typevars_t ;

typedef struct {
	knh_flag_t                 flag;
	knh_flag_t                 cflag;
	struct knh_NameSpace_t*    ns;
	struct knh_Script_t*       script;
	struct knh_Method_t*       mtd;
	knh_class_t                this_cid;
	knh_short_t                espidx;

	/*gamma*/
	knh_ushort_t               goffset;
	knh_short_t                psize; /* param size */
	knh_fields_t*              gf; /* type environment */
	knh_short_t                idxIT;
	knh_type_t                 typeIT;
	size_t                     tvsize;
	knh_typevars_t            *typevars;

	struct knh_Array_t    *constPools;
	struct knh_Array_t    *insts;
	struct knh_Array_t*   lstacks;
	struct knh_Stmt_t*    finallyStmt;

	void                  *dlhdr;
	struct knh_DictMap_t  *symbolDictMap;

	/*stat*/
	size_t statError;
	size_t statWarning;
	size_t statBadManner;
	size_t statKonohaStyle;
	size_t statStmt;
} knh_GammaEX_t;

typedef struct knh_Gamma_t {
	knh_hObject_t h;
	knh_GammaEX_t *b;
	knh_uri_t uri; knh_ushort_t line;
} knh_Gamma_t;

/* ------------------------------------------------------------------------ */
//## class KLRInst Object;

typedef knh_uintptr_t          knh_opcode_t;
typedef knh_intptr_t           knh_sfpidx_t;

typedef struct {
	knh_sfpidx_t i;
	/*size_t    n;*/
	knh_sfpidx_t n;
} knh_sfx_t;

#if defined(K_USING_THREADEDCODE)
#define KCODE_HEAD \
	void *codeaddr; \
	size_t count; \
	knh_ushort_t opcode; \
	knh_ushort_t line

#else
#define KCODE_HEAD \
	size_t count; \
	knh_opcode_t opcode; \
	knh_uintptr_t line \

#endif/*K_USING_THREADEDCODE*/

typedef struct knh_opset_t {
	KCODE_HEAD;
	union {
		knh_uintptr_t data[5];
		void *p[5];
		knh_uchar_t *u[5];
	};
} knh_opset_t;

#define DBG_ASSERT_OPCODE(c) \
	if(((knh_opcode_t)c) >= OPCODE_MAX) {\
		DBG_P("Invalid opcode=%d", (int)c); \
		DBG_ASSERT(c < OPCODE_MAX); \
	}\

typedef struct knh_KLRInst_t {
	knh_hObject_t h;
	knh_opset_t *op;
	knh_opset_t *code_pos;
	knh_ushort_t opcode;
	knh_ushort_t line;
} knh_KLRInst_t;

/* ------------------------------------------------------------------------ */
//## class KLRCode Object;

typedef struct knh_KLRCode_t {
	knh_hObject_t h;
	knh_opset_t*  code;
	size_t        codesize;
	knh_String_t *source;
	knh_uri_t     uri; knh_uri_t     domain;
} knh_KLRCode_t;

/* ======================================================================== */

#ifdef __cplusplus
}
#endif

#endif /*CLASS_KONOHA_H_*/

