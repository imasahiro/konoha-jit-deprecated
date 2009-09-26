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
//## flag Object Release!Debug 0 (%s)->h.flag is *   is *;
//## flag Object Immutable     1 (%s)->h.flag is *   is *;
//## flag Object Cyclic        2 (%s)->h.flag is set *  *;
//## flag Object Undefined     3 (%s)->h.flag is *   is *;
//## flag Object Modified      4 (%s)->h.flag is set is set;
//## flag Object Shared        5 (%s)->h.flag is set is set;
//## flag Object Local4       11 (%s)->h.flag is set *  *;
//## flag Object Local3       12 (%s)->h.flag is set *  *;
//## flag Object Local2       13 (%s)->h.flag is set *  *;
//## flag Object Local1       14 (%s)->h.flag is set *  *;

#ifndef KNH_OBJECT_FIELDSIZE
#define KNH_OBJECT_FIELDSIZE 64
#endif

typedef struct knh_ObjectField_t {
	knh_hObject_t h;
	Object  **fields;
	size_t  bsize;
} knh_ObjectField_t ;

#define CLASS_ObjectField           CLASS_Object
#define knh_Object_cid(o)           (o)->h.cid
#define knh_Object_bcid(o)          (o)->h.bcid
#define knh_Object_p1(o)            (ClassTable((o)->h.cid).p1)
#define knh_Object_p2(o)            (ClassTable((o)->h.cid).p2)

/* ------------------------------------------------------------------------ */
//## @Immutable class Boolean Object;

typedef struct {
	union {
		knh_bool_t bvalue;
		knh_int_t ivalue;
		knh_float_t   fvalue;
		knh_uint64_t  data;
	};
} knh_nObject_t ;

typedef struct knh_Boolean_t {
	knh_hObject_t h;
	knh_nObject_t n;
} knh_Boolean_t;

#define IS_TRUE(o)         ((o)->h.bcid == CLASS_Boolean && tobool(o))
#define IS_FALSE(o)        ((o)->h.bcid == CLASS_Boolean && tobool(o) == 0)
#define new_Boolean(ctx, c)    ((c) ? KNH_TRUE : KNH_FALSE)

/* ------------------------------------------------------------------------ */
//## class Number Object;

typedef struct knh_Number_t {
	knh_hObject_t h;
	knh_nObject_t n;
} knh_Number_t;

#define toint(o)         (((knh_Number_t*)o)->n.ivalue)
#define tofloat(o)       (((knh_Number_t*)o)->n.fvalue)
#define tobool(o)        (((knh_Number_t*)o)->n.bvalue)

/* ------------------------------------------------------------------------ */
//## @Immutable class Int Number;

typedef struct knh_Int_t {
	knh_hObject_t h;
	knh_nObject_t n;
} knh_Int_t;

//typedef knh_Int_t knh_IntX_t;

/* ------------------------------------------------------------------------ */
//## @Immutable class Float Number;

typedef struct knh_Float_t {
	knh_hObject_t h;
	knh_nObject_t n;
} knh_Float_t;

//typedef knh_Float_t knh_FloatX_t;

/* ------------------------------------------------------------------------ */
//## @Immutable class String Object;
//## flag String TextSgm 1 - is set  *   *;
//## flag String Ascii   2 - is set  is  *;

typedef struct knh_String_t {
	knh_hObject_t h;
	knh_uchar_t *str;
	size_t size;
	struct knh_String_t *orign;
} knh_String_t;

//typedef knh_String_t   knh_StringX_t;

#define knh_String_value(o)   ((o)->str)
#define knh_String_tochar(o)  (char*)((o)->str)
#define knh_String_strlen(o)  (o)->size

/* ------------------------------------------------------------------------ */
//## class Bytes Object;

typedef struct knh_Bytes_t {
	knh_hObject_t h;
	knh_uchar_t *buf;
	size_t size;
	size_t capacity;
} knh_Bytes_t ;

#define knh_Bytes_size(o)      (o)->size
#define knh_Bytes_value(o)     (o)->buf
#define knh_Bytes_last(o)      ((o)->buf + (o)->size)
#define knh_Bytes_tochar(b)  (char*)knh_Bytes_value(b)
#define KNH_SIZE(v)         knh_size(v)

/* ------------------------------------------------------------------------ */
//## @Cyclic class Any Object knh_Any_t;
//## @Cyclic @TypeVariable class This  Any Any;
//## @Cyclic @TypeVariable class Tvoid Any Any;
//## @Cyclic @TypeVariable class T0    Any Any;
//## @Cyclic @TypeVariable class T1    Any Any;
//## @Cyclic @TypeVariable class T2    Any Any;
//## @Cyclic @TypeVariable class T3    Any Any;
//## @Cyclic @TypeVariable class Tx    Any Any;
//## @Cyclic @TypeVariable class Tvar  Any Any;

struct knh_Glue_t;
typedef void (*knh_fgfree)(Ctx *, struct knh_Glue_t *o);

typedef struct knh_Glue_t {
	knh_hObject_t h;
	void *ptr;
	knh_fgfree gfree;
} knh_Glue_t ;

#define CLASS_Glue    CLASS_Any

typedef knh_Glue_t knh_Any_t;
typedef knh_Glue_t knh_This_t;
typedef knh_Glue_t knh_T0_t;
typedef knh_Glue_t knh_T1_t;
typedef knh_Glue_t knh_T2_t;
typedef knh_Glue_t knh_T3_t;

#define IS_NULL(o)            ((ctx)->share->constNull == UP(o))
#define IS_NOTNULL(o)         ((ctx)->share->constNull != UP(o))

/* ------------------------------------------------------------------------ */
//## @Immutable @Struct @Param1(Any) class Iterator Object;

#ifdef _MSC_VER
typedef int   (KNH_CC_FASTCALL *knh_fitrnext)(Ctx *, knh_sfp_t *, int n);
#else
typedef ITRNEXT (*knh_fitrnext)(Ctx *, knh_sfp_t *, int n);
#endif

typedef void (*knh_ffree)(void *ptr);

typedef struct knh_Iterator {
	knh_fitrnext fnext;
	Object* source;
	knh_int_t  pos;
	union {
		void*   ref;
		knh_code_t *pc; /* @see(Generator) */
	};
	knh_ffree freffree;
} knh_Iterator_struct;

typedef struct knh_Iterator_t {
	knh_hObject_t h;
	knh_Iterator_struct *b;
	knh_fitrnext fnext_1;
} knh_Iterator_t;

/* ------------------------------------------------------------------------ */
//## @Immutable @Param1(Any) @Param2(Any) class Pair Object;
//## type Pair<String,String> PairSS Pair void String String;
//## type Pair<String,T1> PairST1 Pair void String T1;
//## type Pair<T1,T2> PairT1T2 Pair void T1 T2;

typedef struct knh_Pair_t {
	knh_hObject_t h;
	knh_Object_t  *first;
	knh_Object_t  *second;
} knh_Pair_t;

/* ------------------------------------------------------------------------ */
//## @Immutable class Tuple Object;
//## flag Tuple Triple 1 - is set * *;

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
	knh_Object_t  *third;
} knh_Tuple_t;

/* ------------------------------------------------------------------------ */
//## @Immutable @Param1(Any) class Range Object;
//## flag Range Inclusive 1 - is set * *;

typedef struct knh_Range_t {
	knh_hObject_t h;
	knh_Object_t  *start;
	knh_Object_t  *end;
} knh_Range_t;

/* ------------------------------------------------------------------------ */
//## @Cyclic @Param1(Any) class Array Object;
//## flag Array Dimension 1 - has set * *;
//## type int(T1,T1) Cmpr Closure Int! T1! T1! void;

#define KNH_ARRAY_INITSIZE  (KNH_FASTMALLOC_SIZE / sizeof(knh_Object_t*))

typedef struct knh_Array_t {
	knh_hObject_t h;
	struct knh_Object_t **list;
	size_t size;
	size_t capacity;
} knh_Array_t;

#define knh_Array_n(a,n)      (a)->list[(n)]
#define knh_Array_size(a)     (a)->size

/* ------------------------------------------------------------------------ */
//## @Cyclic @Param1(Int) class Int[] IArray Object knh_IArray_t;
//## flag IArray Dimension 1 - has set * *;

#define KNH_IARRAY_INITSIZE  (KONOHA_SMALLPAGESIZE / sizeof(knh_int_t))

typedef struct knh_IArray_t {
	knh_hObject_t h;
	knh_int_t *ilist;
	size_t size;
	size_t capacity;
} knh_IArray_t;

/* ------------------------------------------------------------------------ */
//## @Cyclic @Param1(Float) class Float[] FArray Object knh_FArray_t;
//## flag FArray Dimension 1 - has set * *;

#define KNH_FARRAY_INITSIZE  (KONOHA_SMALLPAGESIZE / sizeof(knh_float_t))

typedef struct knh_FArray_t {
	knh_hObject_t h;
	knh_float_t *flist;
	size_t size;
	size_t capacity;
} knh_FArray_t;

/* ------------------------------------------------------------------------ */
//## @Cyclic @Param1(Any) class DictMap Object;
//## @Cyclic @Param1(Any) class DictSet Object;
//## flag DictMap IgnoreCase 1 - is set is *;
//## flag DictSet IgnoreCase 1 - is set is *;

#define new_DictSet(ctx, c)   new_DictSet0(ctx, c)

#define KNH_DICT_INITSIZE (KNH_FASTMALLOC_SIZE/(sizeof(void*)*2))

typedef int (*knh_fbytescmp)(knh_bytes_t, knh_bytes_t);

typedef struct knh_dict_t {
	struct knh_String_t *key;
	union {
		Object *_unused;
		knh_uintptr_t _unused_int;
	};
} knh_dict_t;

typedef struct knh_dictmape_t {
	struct knh_String_t *key;
	Object *value;
} knh_dictmape_t;

typedef struct knh_DictMap_t {
	knh_hObject_t h;
	union {
		knh_dictmape_t *list;
		knh_dict_t *_list;  // To avoid stupid casting
	};
	size_t size;
	knh_fbytescmp fcmp;
} knh_DictMap_t;

/* ------------------------------------------------------------------------ */

typedef struct knh_dictsete_t {
	struct knh_String_t *key;
	knh_uintptr_t value;
} knh_dictsete_t;

typedef struct knh_DictSet_t {
	knh_hObject_t h;
	union {
		knh_dictsete_t *list;
		knh_dict_t *_list;  // To avoid stupid casting
	};
	size_t size;
	knh_fbytescmp fcmp;
} knh_DictSet_t;

/* ------------------------------------------------------------------------ */
//## @Struct @Param1(Any) @Param2(Any) @Cyclic class HashMap Object;
//## @Struct @Param1(Any) @Cyclic class HashSet Object;

typedef struct knh_hashentry_t {
	knh_hashcode_t hcode;
	struct knh_hashentry_t *next;
	Object *key;
	union {
		Object *value;
		knh_uintptr_t uvalue;
	};
} knh_hashentry_t;

typedef knh_hashentry_t* (*knh_fhashentry_init)(Ctx *);
typedef void (*knh_fhashentry_traverse)(Ctx *, knh_hashentry_t*, knh_ftraverse);

typedef struct {
	size_t size;
	knh_fhashentry_init finit;
	knh_fhashentry_traverse ftraverse;
} knh_hash_op ;

/* ------------------------------------------------------------------------ */

#define KNH_HASH_INITSIZE 83
#define KNH_HASH_TABLESIZE  KONOHA_PAGESIZE
#define KNH_HASHENTRY(T,ESIZE,i)  (knh_hashentry_t*)&((T)[(ESIZE) * (i)])

typedef struct {
	knh_hashentry_t** array;
	size_t hmax;
	size_t capacity;
	size_t size;
	knh_hashentry_t*  unused_first;
	char** tables;
	size_t tables_capacity;
	size_t tables_size;
	char*  tables_first;
	char *DBG_name;
} knh_Hash_struct;

typedef struct {
	knh_hObject_t h;
	knh_Hash_struct *b;
	knh_fstruct_compareTo fcmp;
	knh_hash_op *hashop;
} knh_Hash_t ;

typedef knh_Hash_struct knh_HashMap_struct;
typedef knh_Hash_struct knh_HashSet_struct;

typedef struct knh_HashMap_t {
	knh_hObject_t h;
	knh_HashMap_struct *b;
} knh_HashMap_t;

typedef struct knh_HashSet_t {
	knh_hObject_t h;
	knh_HashSet_struct *b;
} knh_HashSet_t;

/* ------------------------------------------------------------------------ */
//## @Private class DictIdx Object;

typedef struct knh_DictIdx_t {
	knh_hObject_t h;
	struct knh_Array_t *terms;
	struct knh_DictSet_t *termsDictSet;
	knh_intptr_t offset;
} knh_DictIdx_t;

/* ------------------------------------------------------------------------ */
//## @Immutable class Class Object;
//## flag Class Release!Debug  0 (pClassTable(%s))->cflag is * is *;
//## flag Class Immutable      1 (pClassTable(%s))->cflag is * is *;
//## flag Class Cyclic         2 (pClassTable(%s))->cflag is set * *;
//## flag Class MetaClass      3 (pClassTable(%s))->cflag is * is *;
//## flag Class Private!Public 4 (pClassTable(%s))->cflag is * is *;
//## flag Class Final          5 (pClassTable(%s))->cflag  is * is *;
//## flag Class Singleton      6 (pClassTable(%s))->cflag  is * is *;
//## flag Class Unique         7 (pClassTable(%s))->cflag  is * is *;
//## flag Class Interface      8 (pClassTable(%s))->cflag  is * is *;
//## flag Class TypeVariable   9 (pClassTable(%s))->cflag  is set * *;

typedef struct knh_Class_t {
	knh_hObject_t h;
	knh_class_t   cid;
	knh_type_t    type;
} knh_Class_t;

/* class */

#define KNH_DEF(ctx, cid)  knh_getClassDefaultValue(ctx, cid)

#define knh_Class_cid(c)     (knh_class_t)(c)->cid
#define knh_flag_oflag(f)        (f)
#define knh_class_isGenerics(cid)    (ClassTable(cid).p1 != TYPE_Tvoid)

/* ------------------------------------------------------------------------ */
//## @Private class ClassStruct Object;
//## flag ClassStruct Hidden     0 (%s)->fields[n].flag is set * *;
//## flag ClassStruct Protected  1 (%s)->fields[n].flag is set * *;
//## flag ClassStruct Getter     2 (%s)->fields[n].flag is set * *;
//## flag ClassStruct Setter     3 (%s)->fields[n].flag is set * *;
//## flag ClassStruct Key        4 (%s)->fields[n].flag is set * *;
//## flag ClassStruct Volatile   5 (%s)->fields[n].flag is set * *;
//## flag ClassStruct ReadOnly   6 (%s)->fields[n].flag is set * *;

typedef struct knh_cfield_t {
	knh_flag_t    flag  ;
	knh_type_t    type  ;
	knh_fieldn_t  fn    ;
	Object        *value;
} knh_cfield_t ;

typedef struct knh_ClassStruct_t {
	knh_hObject_t h;
	knh_cfield_t* fields;
	size_t fsize;
	knh_Array_t*  methods;
} knh_ClassStruct_t;

/* ------------------------------------------------------------------------ */
//## @Private class MethodField Object;

typedef struct {
	knh_type_t    type;
	knh_fieldn_t  fn;
} knh_mparam_t ;

#define KNH_METHODFIELD_2  2

typedef struct knh_MethodField_t {
	knh_hObject_t h;
	knh_type_t rtype; 	knh_ushort_t psize;
	union {
		knh_mparam_t* mparams;
		knh_mparam_t  p0;
	};
	knh_mparam_t p1;
} knh_MethodField_t;

/* ------------------------------------------------------------------------ */
//## @Struct class Method Object;
//## flag Method Private!Public  0 DP(%s)->flag is set is *;
//## flag Method Virtual!Final   1 DP(%s)->flag is set is *;
//## flag Method Release!Debug   2 DP(%s)->flag is set * *;
//## flag Method Const           3 DP(%s)->flag is * is *;
//## flag Method Static          4 DP(%s)->flag is * is *;
//## flag Method NullBase        5 DP(%s)->flag is * * *;
//## flag Method VarArgs         6 DP(%s)->flag is set is *;
//## flag Method Generator       7 DP(%s)->flag is set * *;
//## flag Method Aspect          8 DP(%s)->flag is set * *;
//## flag Method ObjectCode      9 DP(%s)->flag is set * *;
//## flag Method Hidden          10 DP(%s)->flag is set * *;
//## flag Method Tricky          11 DP(%s)->flag is set * *;

typedef struct {
	knh_flag_t     flag;  knh_ushort_t   delta;
	knh_class_t    cid;   knh_methodn_t  mn;
	knh_uri_t     uri;  knh_uri_t    domain;
	struct knh_MethodField_t* mf;
	knh_fmethod       fproceed;
	void*             code;
	knh_uintptr_t    prof_count;  /*recode how many times called */
	knh_uintptr_t    prof_time;   /*recode how long spending */
} knh_Method_struct;

typedef struct knh_Method_t {
	knh_hObject_t h;
	knh_Method_struct *b;
	knh_fmethod fcall_1;
	knh_code_t *pc_start;
} knh_Method_t;

#define knh_Method_mn(mtd)    DP(mtd)->mn
#define knh_Method_mf(mtd)    DP(mtd)->mf
#define knh_MethodField_rztype(mf)   ((mf)->rtype)
#define knh_MethodField_psize(mf)    ((mf)->psize)
#define knh_Method_rztype(mtd)      ((DP(mtd)->mf)->rtype)
#define knh_Method_psize(mtd)       ((DP(mtd)->mf)->psize)
#define knh_Method_pztype(mtd,n)    knh_MethodField_pztype(DP(mtd)->mf,n)

#define knh_Class_getMethod(ctx, c, mn)    knh_Class_getMethod__(ctx, c, mn, 0)
#define knh_Class_findMethod(ctx, c, mn)   knh_Class_getMethod__(ctx, c, mn, 1)

/* ------------------------------------------------------------------------ */
//## @Struct class Mapper Object;
//## flag Mapper Interface     0 DP(%s)->flag is set is *;
//## flag Mapper ICast         1 DP(%s)->flag is * is *;
//## flag Mapper Significant   2 DP(%s)->flag is set is *;
//## flag Mapper Final         3 DP(%s)->flag is set is *;
//## flag Mapper Synonym       4 DP(%s)->flag is set is *;
//## flag Mapper LossLess      5 DP(%s)->flag is set is *;
//## flag Mapper Total!Partial 6 DP(%s)->flag is set is *;
//## flag Mapper Const!Temporal 7 DP(%s)->flag is set is *;
//## flag Mapper Local         8 DP(%s)->flag is set is *;
//## flag Mapper Derived       9 DP(%s)->flag is set is *;
//## flag Mapper MapMap       10 DP(%s)->flag is set * *;

typedef struct knh_Mapper {
	knh_flag_t   flag; knh_ushort_t size;
	knh_class_t  scid; knh_class_t  tcid;
	//knh_fmapper     fmap;
	union {
		Object*      mapdata;
		struct knh_Mapper_t *m1;
	};
	struct knh_Mapper_t *m2;
} knh_Mapper_struct;

typedef struct knh_Mapper_t {
	knh_hObject_t h;
	knh_Mapper_struct *b;
	knh_fmapper fmap_1;
} knh_Mapper_t;

#define knh_findMapper(ctx, scid, tcid) knh_findMapper_(ctx, scid, tcid, 1)
#define knh_Class_getMapper(ctx, scid, tcid)  knh_findMapper_(ctx, scid, tcid, 0)

/* ------------------------------------------------------------------------ */
//## @Private class ClassMap Object;
//## flag ClassMap Sorted 1 - is set * *;

typedef struct knh_ClassMap_t {
	knh_hObject_t h;
	struct knh_Mapper_t**   maplist;
	knh_ushort_t   size;
	knh_ushort_t   capacity;
} knh_ClassMap_t ;

/* ------------------------------------------------------------------------ */
//## class Closure Object;
//## flag Closure StoredEnv 1 - is set * *;

typedef struct knh_Closure_t {
	knh_hObject_t h;
	union {
		Object* base;
		struct knh_Closure_t *self;
	};
	struct knh_Method_t* mtd;
	union {
		knh_sfp_t *envsfp;
		size_t *hstacksize;
	};
} knh_Closure_t ;


#define KNH_INVOKE(ctx, lsfp) {\
		knh_Closure_t* cc_ = sfp[0].cc;\
		int argc_ = (ctx->esp - sfp) - 1; \
		KNH_MOV(ctx, sfp[0].o, (cc_)->base);\
		KNH_SCALL(ctx, sfp, -1, (cc_)->mtd, argc_);\
	}\

/* ------------------------------------------------------------------------ */
//## @Private @Immutable class AffineConv Object;

#ifdef KNH_USING_NOFLOAT
typedef long knh_affinefloat_t;
#else
typedef float knh_affinefloat_t;
#endif

typedef struct knh_AffineConv_t {
	knh_hObject_t h;
	knh_affinefloat_t scale;
	knh_affinefloat_t shift;
} knh_AffineConv_t;

#define FLAG_Mapper_Affine (FLAG_Mapper_ICast|FLAG_Mapper_Synonym|FLAG_Mapper_Const)

/* ------------------------------------------------------------------------ */
//## @Immutable class Regex Object;

typedef struct {
	knh_hObject_t h;
	knh_regex_t *reg;
	knh_regex_drvapi_t *df;
	struct knh_String_t *pattern;
} knh_Regex_t;

/* ------------------------------------------------------------------------ */
//## @Private class BytesConv Object;

#ifdef KNH_USING_ICONV
	#include<iconv.h>
#else
typedef knh_intptr_t iconv_t;
#endif

typedef struct knh_BytesConv_t {
	knh_hObject_t h;
	knh_fbyteconv      fbconv;
	knh_fbyteconvfree  fbconvfree;
	union {
		iconv_t iconv_d;
		void *convp;
	};
} knh_BytesConv_t;

/* ------------------------------------------------------------------------ */
//## @Struct @Private class ClassSpec Object;

#define KNH_CLASSSPEC_FMT    "%s{%s}"
struct  knh_ClassSpec_t;

typedef int (*knh_fichk)(struct knh_ClassSpec_t *, knh_int_t v);
typedef int (*knh_ficmp)(struct knh_ClassSpec_t *, knh_int_t v1, knh_int_t v2);

typedef int (*knh_ffchk)(struct knh_ClassSpec_t *, knh_float_t v);
typedef int (*knh_ffcmp)(struct knh_ClassSpec_t *, knh_float_t v1, knh_float_t v2);

typedef knh_String_t *(*knh_fsnew)(Ctx *, knh_class_t cid, knh_bytes_t, struct knh_String_t *, int *);
typedef int (*knh_fscmp)(struct knh_ClassSpec_t *, knh_bytes_t, knh_bytes_t);

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
	knh_fichk fichk;
	knh_ficmp ficmp;

	// float
	knh_float_t fmin;
	knh_float_t fmax;
	knh_float_t fstep;
	knh_ffchk   ffchk;
	knh_ffcmp   ffcmp;

	// String
	size_t bytelen;
	size_t charlen;
	Object* pattern;
	struct knh_DictIdx_t* vocabDictIdx;
	struct knh_BytesConv_t *bconv;
	knh_fsnew    fsnew;
	knh_fscmp    fscmp;
} knh_ClassSpec_struct;

typedef struct knh_ClassSpec_t* (*knh_fspec)(Ctx *ctx, knh_bytes_t urn);

typedef struct knh_ClassSpec_t {
	knh_hObject_t h;
	knh_ClassSpec_struct *b;
} knh_ClassSpec_t;

/* ------------------------------------------------------------------------ */
/* InputStream, OutputStream */

#define KNH_STDIN          (ctx->in)
#define KNH_STDOUT         (ctx->out)
#define KNH_STDERR         (ctx->err)

/* ------------------------------------------------------------------------ */
//## @Struct class InputStream Object;
//## flag InputStream FILE 1 - is set * *;

typedef struct knh_InputStream {
	union {
		knh_io_t fd;
		FILE *fp;
	};
	knh_iodrv_t *driver;
	union {
		struct knh_Bytes_t *ba;
		struct knh_String_t *str;
	};
	char *buf;
	size_t bufpos;
	size_t bufend;
	size_t bufsiz;
	knh_String_t *enc;
	struct knh_BytesConv_t* bconv;
	knh_String_t*  urn;
	knh_uri_t  uri;
	knh_ushort_t  prev;
	size_t line;
	size_t size;
} knh_InputStream_struct;

typedef struct knh_InputStream_t {
	knh_hObject_t h;
	knh_InputStream_struct *b;
} knh_InputStream_t;

/* ------------------------------------------------------------------------ */
//## @Struct class OutputStream Object;
//## flag OutputStream BOL            1 - is set * *;
//## flag OutputStream AutoFlush      2 - is set is set;
//## flag OutputStream StoringBuffer  3 - is set is set;

typedef struct {
	knh_io_t fd;
	knh_iodrv_t *driver;
	knh_Bytes_t* ba;
	knh_String_t *enc;
	struct knh_BytesConv_t* bconv;
	knh_String_t*  urn;
	size_t size;
	size_t line;
	knh_String_t*  NEWLINE;
	knh_String_t*  TAB;
	knh_short_t indent;
	knh_flag_t flag;  /*unused */
} knh_OutputStream_struct;

typedef struct knh_OutputStream_t {
	knh_hObject_t h;
	knh_OutputStream_struct *b;
} knh_OutputStream_t;

#define knh_write_delim(ctx, w)    knh_write(ctx, w, STEXT(", "))
#define knh_write_dots(ctx, w)     knh_write(ctx, w, STEXT("..."))
#define knh_write_fn(ctx, w, fn)   knh_write(ctx, w, B(FIELDN(fn)))
#define knh_write_type(ctx, w, type)   knh_write_type_(ctx, w, type, 0)
#define knh_write_ltype(ctx, w, type)  knh_write_type_(ctx, w, type, 1)
#define knh_write__O(ctx, w, o)    knh_format(ctx, w, METHODN__k, o, KNH_NULL)

/* ------------------------------------------------------------------------ */
//## @Struct class Socket Object;

typedef struct {
	knh_intptr_t sd;
	knh_intptr_t port;
	struct knh_String_t *urn;
	struct knh_InputStream_t  *in;
	struct knh_OutputStream_t *out;
} knh_Socket_struct;

typedef struct knh_Socket_t {
	knh_hObject_t h;
	knh_Socket_struct *b;
} knh_Socket_t;

/* ======================================================================== */
//## class Connection Object;
//## @Struct class ResultSet Object;

typedef struct knh_Connection_t {
	knh_hObject_t h;
	knh_db_t           *conn;
	knh_db_drvapi_t    *df;
	knh_String_t       *urn;
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

typedef struct knh_ResultSet_struct {
	struct knh_Connection_t *conn;
	knh_dbcur_t             *dbcur;
	knh_fdbcurfree           dbcur_free;  /* necessary if conn is closed before */
	knh_String_t            *tableName;
	knh_class_t              tcid;
	knh_ushort_t             column_size;
	knh_dbschema_t          *column;
	knh_Bytes_t             *databuf;
	size_t                   count;
} knh_ResultSet_struct;

typedef struct knh_ResultSet_t {
	knh_hObject_t h;
	knh_ResultSet_struct *b;
} knh_ResultSet_t;

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

typedef struct knh_Exception_struct {
	knh_flag_t flag;
	knh_expt_t eid;
	knh_String_t* msg;
	Object* bag;
	knh_Array_t* traces;
	char*     file;
	size_t    line;
} knh_Exception_struct;

typedef struct knh_Exception_t {
	knh_hObject_t h;
	knh_Exception_struct *b;
} knh_Exception_t;

#define new_Exception__s(ctx, s)     new_Exception__b(ctx, B(s))

/* ------------------------------------------------------------------------ */
//## @Private @Struct class ExceptionHandler Object;
//## flag ExceptionHandler Jumpable   1 - is set * *;
//## flag ExceptionHandler Catching   2 - is set * *;

#ifdef KNH_USING_POSIX
typedef sigjmp_buf knh_jmpbuf_t;
#define knh_setjmp(buf)         sigsetjmp(buf, 1)
#define knh_longjmp(buf, val)   siglongjmp(buf, val)
#else
typedef jmp_buf knh_jmpbuf_t;
#define knh_setjmp(buf)         setjmp(buf)
#define knh_longjmp(buf, val)   longjmp(buf, val)
#endif

typedef struct knh_ExceptionHandler {
	knh_sfp_t *esp;
	knh_code_t *pc;
	struct knh_Exception_t* caught;
	knh_jmpbuf_t jmpbuf;
} knh_ExceptionHandler_struct;

typedef struct knh_ExceptionHandler_t {
	knh_hObject_t h;
	knh_ExceptionHandler_struct *b;
} knh_ExceptionHandler_t;

#define KNH_THROW(ctx, e) knh_throw(ctx, UP(e), knh_safefile(__FILE__), __LINE__)
#define KNH_THROWs(ctx, s) \
	knh_throwException(ctx, new_Exception__b(ctx, B(s)), knh_safefile(__FILE__), __LINE__); \


#define KNH_THROWf(ctx, fmt, ...) \
	char throcwb_[256]; \
	knh_snprintf(throcwb_, sizeof(throcwb_), fmt, ## __VA_ARGS__); \
	knh_throwException(ctx, new_Exception__b(ctx, B(throcwb_)), knh_safefile(__FILE__), __LINE__); \

#define TODO_THROW(ctx) knh_throw_TODO(ctx, (char*)__FILE__, __LINE__, (char*)__FUNCTION__)

#define KNH_PERRNO(ctx, cwb, emsg, func, isThrowable) { \
		knh_cwb_perrno(ctx, cwb, emsg, func, __FILE__, __LINE__, isThrowable); \
	}\


#define KNH_NOAPI(ctx, cwb, isThrowable) { \
		knh_throw_Unsupported(ctx, cwb, __FUNCTION__, __FILE__, __LINE__, isThrowable); \
	}\

#define KNH_THROW_OUTOFINDEX(ctx, n, max) { \
		knh_throw_OutOfIndex(ctx, n, max, __FILE__, __LINE__); \
	}\


/* ------------------------------------------------------------------------ */
//## @Private @Singleton class Script Object;

#define KNH_SCRIPT_FIELDSIZE   128

typedef struct knh_Script_t {
	knh_hObject_t h;
	Object **fields;
} knh_Script_t ;

/* ------------------------------------------------------------------------ */
//## @Struct class NameSpace Object;

#define KNH_PATH_TRUSTED      1
#define KNH_PATH_UNTRUSTED    2

typedef struct knh_NameSpace {
	knh_String_t*           nsname;
	struct knh_Script_t*    script;
	knh_Array_t*            importedNameSpaces;
	struct knh_DictSet_t*   name2cidDictSet;
	struct knh_DictSet_t*   func2cidDictSet;
	struct knh_DictSet_t*   pathTrustDictSet;
	struct knh_DictMap_t*   lconstDictMap;
	struct knh_DictMap_t*   tag2urnDictMap;
} knh_NameSpace_struct;

typedef struct knh_NameSpace_t {
	knh_hObject_t h;
	knh_NameSpace_struct *b;
} knh_NameSpace_t;

/* ------------------------------------------------------------------------ */
//## @Singleton @Struct class System Object;

typedef struct {
	knh_uintptr_t sysid;
	size_t     ctxcount;

	struct knh_DictMap_t*      props;
	struct knh_InputStream_t*  in;
	struct knh_OutputStream_t* out;
	struct knh_OutputStream_t* err;
	struct knh_String_t*       enc;
//	knh_String_t              *homeDir;

	struct knh_DictIdx_t *ResourceDictIdx;
	struct knh_DictIdx_t *FieldNameDictIdx;
	struct knh_HashMap_t* MethodFieldHashMap;

	struct knh_DictSet_t *ClassNameDictSet;
	struct knh_DictSet_t *ExptNameDictSet;

	struct knh_DictMap_t* NameSpaceTableDictMap;
	struct knh_DictMap_t* URNAliasDictMap;
	struct knh_DictSet_t *DriversTableDictSet;
	struct knh_DictSet_t *SpecFuncDictSet;
	struct knh_Array_t   *UsingResources;

	struct knh_DictMap_t *listenerDictMap;
	struct knh_DictMap_t *trustedPathDictSet;

} knh_System_struct;

typedef struct knh_System_t {
	knh_hObject_t h;
	knh_System_struct *b;
} knh_System_t;

/* ------------------------------------------------------------------------ */
//## class Context Object;
//## flag Context Strict       0 ((knh_Context_t*)%s)->flag is set is set;
//## flag Context Debug        1 ((knh_Context_t*)%s)->flag is set is set;
//## flag Context Verbose      2 ((knh_Context_t*)%s)->flag is set is set;
//## flag Context Interactive  3 ((knh_Context_t*)%s)->flag is set is *;
//## flag Context Compiling    4 ((knh_Context_t*)%s)->flag is set * *;

/* ------------------------------------------------------------------------ */
/* konohac.h */

typedef knh_ushort_t   knh_token_t;
typedef knh_ushort_t   knh_stmt_t;
#define knh_perror0_t   int

#define KNH_SYS(ctx, n)    knh_getSystemConst(ctx, n)
#define KNH_SYS_CTX    0
#define KNH_SYS_STDIN  1
#define KNH_SYS_STDOUT 2
#define KNH_SYS_STDERR 3
#define KNH_SYS_OS     4
#define KNH_SYS_SCRIPT 5

#define KNH_FLAG_PF_STDERR      KNH_FLAG_T0
#define KNH_FLAG_PF_EOL         KNH_FLAG_T1
#define KNH_FLAG_PF_TIME        KNH_FLAG_T2
#define KNH_FLAG_PF_FUNC        KNH_FLAG_T3
#define KNH_FLAG_PF_NAME        KNH_FLAG_T4
#define KNH_FLAG_PF_BOL         KNH_FLAG_T7

/* ------------------------------------------------------------------------ */
//## @Struct class Token Object;
//## flag Token BOL           0 SP(%s)->flag is set * *;
//## flag Token TailWildCard  1 SP(%s)->flag is set * *;
//## flag Token TopDot        2 SP(%s)->flag is set * *;
//## flag Token Getter        3 SP(%s)->flag is set * *;
//## flag Token Setter        4 SP(%s)->flag is set * *;
//## flag Token ExceptionType 5 SP(%s)->flag is set * *;
//## flag Token NotNullType   6 SP(%s)->flag is set * *;
//## flag Token NullableType  7 SP(%s)->flag is set * *;

#define knh_Token_isUnsafeType(o)  knh_Token_isTailWildCard(o)
#define knh_Token_setUnsafeType(o, b) knh_Token_setTailWildCard(o, b)

#define knh_Token_isPNAME(o)  knh_Token_isTailWildCard(o)
#define knh_Token_setPNAME(o, b) knh_Token_setTailWildCard(o, b)

#define knh_Token_isSUPER(o)      knh_Token_isTailWildCard(o)
#define knh_Token_setSUPER(o, b)  knh_Token_setTailWildCard(o, b)

/* ------------------------------------------------------------------------ */

#define knh_Token_isTyped(o)    (DP(o)->type != TYPE_var)

typedef struct knh_Token {
	knh_type_t   type;
	union {
		knh_token_t   tt_next;
		knh_short_t   index;   /* STACK, FIELD, GLOBAL, SYSVAL, DEFVAL */
		knh_methodn_t mn;
		knh_token_t   tt_op;
		knh_fieldn_t  fn;
		knh_class_t   cid;
	};
	union {
		Object* data;
		struct knh_Array_t*  list;
		struct knh_String_t* text;
		struct knh_Token_t* token;
		struct knh_Method_t* mtd;
		struct knh_Int_t *num;
	};
} knh_Token_struct;

typedef struct knh_Token_t {
	knh_hObject_t h;
	knh_Token_struct *b;
	knh_uri_t uri;   knh_ushort_t line;
	knh_flag_t flag; knh_token_t  tt;
} knh_Token_t;

#define sToken(o)  knh_Token_tochar(ctx, o)

/* ------------------------------------------------------------------------ */

typedef struct {
	struct knh_Token_t** ts;
	int meta;
	int c;
	int e;
} knh_tkc_t;

/* ------------------------------------------------------------------------ */
//## @Struct class Stmt Object;
//## flag Stmt Adposition 0 SP(%s)->flag is set * *;
//## flag Stmt Literal    1 SP(%s)->flag is set * *;
//## flag Stmt AutoReturn   2 SP(%s)->flag is set * *;
//## flag Stmt Untyped    3 SP(%s)->flag is set * *;
//## flag Stmt Expr       4 SP(%s)->flag is set * *;
//## flag Stmt Memo1      5 SP(%s)->flag is set * *;

#define knh_Stmt_isTyped(o)        (DP(o)->type != TYPE_var)

#define knh_Stmt_isVOID(s)         knh_Stmt_isMemo1(s)
#define knh_Stmt_setVOID(s,b)      knh_Stmt_setMemo1(s,b)

/* MAPCAST */
/* DEBUG {} */
#define knh_Stmt_isDEBUG(s)       knh_Stmt_isMemo1(s)
#define knh_Stmt_setDEBUG(s,b)    knh_Stmt_setMemo1(s,b)
/* STT_MAPCAST */
#define knh_Stmt_isNNCAST(s)       knh_Stmt_isMemo1(s)
#define knh_Stmt_setNNCAST(s,b)    knh_Stmt_setMemo1(s,b)
/* STT_FOREACH */
#define knh_Stmt_isMAPNEXT(s)      knh_Stmt_isMemo1(s)
#define knh_Stmt_setMAPNEXT(s,b)   knh_Stmt_setMemo1(s,b)
/* STT_METHOD*/
#define knh_Stmt_isVARARGS(s)      knh_Stmt_isMemo1(s)
#define knh_Stmt_setVARARGS(s,b)   knh_Stmt_setMemo1(s,b)

/* ------------------------------------------------------------------------ */

typedef struct knh_Stmt_struct {
	knh_type_t   type;
	knh_short_t  used_stack;
	knh_ushort_t size;
	knh_ushort_t capacity;
	union {
		Object** terms;
		struct knh_Token_t** tokens;
		struct knh_Stmt_t** stmts;
	};
	union {
		struct knh_DictMap_t* metaDictMap;
		struct knh_String_t*  errMsg;
	};
	struct knh_Stmt_t* next;
} knh_Stmt_struct;

typedef struct knh_Stmt_t {
	knh_hObject_t h;
	knh_Stmt_struct *b;
	knh_uri_t uri; knh_ushort_t line;
	knh_flag_t flag; knh_stmt_t  stt;
} knh_Stmt_t;

/* ------------------------------------------------------------------------ */

#define Term         Any
#define IS_Term(o)   (IS_Token((Object*)o)||IS_Stmt((Object*)o))

#define KNH_LOCALSIZE 64

#define new_TermCONST(ctx, fln, d)   TM(new_TokenCONST(ctx, fln, d))
#define TERMs_getcid(stmt, n)    CLASS_type(TERMs_gettype(stmt, n))
#define TERMs_getbcid(stmt, n)   ctx->share->ClassTable[TERMs_getcid(stmt,n)].bcid

/* ------------------------------------------------------------------------ */
/* KLRCode */
/* ------------------------------------------------------------------------ */

typedef unsigned char          knh_opcode_t;
typedef int                    knh_labelid_t;
typedef knh_short_t            knh_sfi_t;
typedef knh_short_t            knh_sfe_t;

typedef struct {
	knh_sfi_t i;
	size_t    n;
} knh_sfx_t;

typedef struct {
#ifdef KNH_USING_THREADEDCODE
	void *codeaddr;
#endif/*KNH_USING_THREADEDCODE*/
	knh_opcode_t opcode;
#ifdef KNH_USING_THREADEDCODE
	void *jumpaddr;
#endif/*KNH_USING_THREADEDCODE*/
	union {
		knh_labelid_t a1;  /* labelid */
		knh_code_t *jumppc;
	};
	void *nextaddr;
} knh_kode_t;

#define KNH_OPCODE(pc)          ((knh_kode_t*)pc)->opcode

#define KNH_ASSERT_OPCODE(c) \
	if(c > OPCODE_NOP) {\
		DBG_P("Invalid opcode=%d", (int)c); \
		KNH_ASSERT(c <= OPCODE_NOP); \
	}\

/* ------------------------------------------------------------------------ */
//## @Struct @Private class Asm Object;
//## flag Asm Cancelled  0 DP(%s)->flag is set * *;
//## flag Asm PROCEED    1 DP(%s)->flag has found * *;
//## flag Asm RETURN     2 DP(%s)->flag has found * *;
//## flag Asm YEILD      3 DP(%s)->flag has found * *;

#ifndef KONOHA_LOCALSIZE
#define KONOHA_LOCALSIZE 128
#endif

#define KNH_ASM_REGMAX 32

typedef struct {
	knh_short_t level;
	knh_short_t varidx;
	struct knh_Stmt_t *stmt;
} knh_asmreg_t ;

typedef struct {
	int offset;
	struct knh_Token_t *tklabel;
} knh_labeltbl_t ;

typedef knh_uint64_t  knh_rtti_t;

typedef struct knh_Asm {
	knh_flag_t             flag;
	knh_class_t            this_cid;
	struct knh_NameSpace_t *ns;
	struct knh_Method_t    *mtd;
	knh_type_t             rtype;
	knh_ushort_t           gamma_size;
	knh_cfield_t*          gamma;   /* type environment */

	knh_rtti_t             nnrtti0;
	knh_rtti_t             nnrtti;

	int                    level; /* 0.3 */
	int stack;
	int globalidx;
	int llstep;

	/* closure */
	knh_flag_t             xflag;
	knh_class_t            xthis_cid;
	struct knh_Method_t   *xmtd;
	knh_type_t             xrtype;
	knh_ushort_t           xstack_size;

	/* regisiter */
	knh_asmreg_t *regs;
	knh_ushort_t regs_size;
	knh_ushort_t regs_usedsize;

	/* label */
	struct knh_DictSet_t* name2labelIdDictSet;
	size_t labelmax;
	size_t labelcapacity;
	knh_labeltbl_t       *labels;
	knh_Array_t*          lstacks;
	struct knh_Stmt_t*    finallyStmt;

	knh_uri_t            uri;
	knh_sline_t          line;
	struct knh_Bytes_t*  elf;
	struct knh_Bytes_t*  dwarf;
	knh_kode_t          *prev_op;

	void *dlhdr;
	struct knh_DictMap_t  *symbolDictMap;
	struct knh_Array_t    *constPools;
	struct knh_Array_t    *exportsMethods;
} knh_Asm_struct;

typedef struct knh_Asm_t {
	knh_hObject_t h;
	knh_Asm_struct *b;
	knh_uri_t uri; knh_ushort_t line;
} knh_Asm_t;

#define knh_Context_getAsm(ctx)    (ctx)->abr

/* ------------------------------------------------------------------------ */
//## @Struct class KLRCode Object;

typedef struct {
	knh_ushort_t offset;
	knh_ushort_t line;
} knh_dwarf_t;

typedef struct knh_KLRCode {
	knh_code_t* code;
	size_t size;
	knh_uri_t uri;
	knh_uri_t domain;
	size_t dsize;
	knh_dwarf_t* dwarf;
} knh_KLRCode_struct;

typedef struct knh_KLRCode_t {
	knh_hObject_t h;
	knh_KLRCode_struct *b;
	knh_uri_t uri; knh_ushort_t line;
} knh_KLRCode_t;


/* ======================================================================== */

#ifdef __cplusplus
}
#endif

#endif /*CLASS_KONOHA_H_*/

