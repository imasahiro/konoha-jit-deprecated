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

#ifndef KONOHA_T_H_
#define KONOHA_T_H_

#include"konoha_config.h"

#ifdef K_USING_STDC
#include<stdio.h>
#include<limits.h>
#include<float.h>
#include<setjmp.h>

#define __STDC_LIMIT_MACROS
#include<stdint.h>
#define __STDC_FORMAT_MACROS
#include<inttypes.h>
#endif

#if defined(K_USING_PTHREAD)
	#include<pthread.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */

#define K_SYSTEMBIT       (sizeof(void*) * CHAR_BIT)

typedef int16_t           knh_int16_t;
typedef uint16_t          knh_uint16_t;
typedef int32_t           knh_int32_t;
typedef uint32_t          knh_uint32_t;
typedef int64_t           knh_int64_t;
typedef uint64_t          knh_uint64_t;

typedef intptr_t          knh_intptr_t;
typedef uintptr_t         knh_uintptr_t;
typedef knh_intptr_t      knh_index_t;

#define K_INTPTR_FMT      "%"PRIdPTR
#define K_INTPTR_UFMT     "%"PRIuPTR


#if defined(__LP64__) || defined(__WIN64__)
typedef int32_t           knh_short_t;
typedef uint32_t          knh_ushort_t;
#ifndef K_USING_NOFLOAT
typedef double            knh_floatptr_t;
#endif

/* virtual machine */
#define VMTX_INT
#define VMTSIZE_int 0
#define VMTX_FLOAT
#define VMTSIZE_float 0

#else/*not defined(__LP64__)*/

typedef int16_t           knh_short_t;
typedef uint16_t          knh_ushort_t;
#ifndef K_USING_NOFLOAT
typedef float             knh_floatptr_t;
#endif

/*virtual machine*/
#define VMTX_INT  ,VMT_VOID
#define VMTSIZE_int 1
#define VMTX_FLOAT  ,VMT_VOID
#define VMTSIZE_float 1

#endif/* defined(__LP64__) */

/* ------------------------------------------------------------------------ */
/* Bool(ean), knh_bool_t */
/* ------------------------------------------------------------------------ */

typedef knh_intptr_t      knh_bool_t;
typedef knh_intptr_t      knh_boolean_t;

/* ------------------------------------------------------------------------ */
/* Integer, knh_int_t */
/* ------------------------------------------------------------------------ */

#ifdef K_USING_INT32
typedef long                knh_int_t;
typedef unsigned long       knh_uint_t;

#define K_INT_MAX               LONG_MAX
#define K_INT_MIN               LONG_MIN
#define K_INT0                  0UL
#define K_INT_FMT               "%ld"
#define K_INT_XFMT              "%lx"
#define K_UINT_MAX              ULONG_MAX
#define K_UINT_MIN              0
#define K_UINT_FMT              "%lu"
#define knh_abs(n)              labs(n)
#define VMTX_INT
#define VMTSIZE_int 0

#else /*K_USING_INT32*/

typedef knh_int64_t        knh_int_t;
typedef knh_uint64_t       knh_uint_t;

#ifndef LLONG_MIN
#define LLONG_MIN -9223372036854775807LL
#define LLONG_MAX  9223372036854775807LL
#endif

#ifndef ULLONG_MAX
#define ULLONG_MAX 18446744073709551615ULL
#endif

#define K_INT_MAX               LLONG_MAX
#define K_INT_MIN               LLONG_MIN
#define K_INT_FMT               "%lld"
#define K_INT_XFMT              "%llx"
#define K_INT0                  0ULL
#define K_UINT_MAX              ULLONG_MAX
#define K_UINT_MIN              0ULL
#define K_UINT_FMT              "%llu"
#define knh_abs(n)              llabs(n)

#endif/*K_USING_INT32*/

#define K_INT_FMTSIZ            40

/* ------------------------------------------------------------------------ */
/* Float, knh_float_t */
/* ------------------------------------------------------------------------ */

#ifdef K_USING_NOFLOAT
/* dummy */
typedef int               knh_float_t;
typedef int               knh_floatptr_t;

#define K_FLOAT_MAX             0
#define K_FLOAT_MIN             0
#define K_FLOAT_STEP            0
#define K_FLOAT_FMT             "%d"
#define K_FLOAT_FMT1            "%d"
#define K_FLOAT_FMT2            "%d"
#define K_FLOAT_FMT3            "%d"
#define K_FLOAT_FMT4            "%d"
#define K_FLOAT_FMTE            "%d"
#define K_FLOAT_NAN             0
#define K_FLOAT_NULL            K_FLOAT_NAN
#define K_FLOAT_FMTSIZ          40
#define K_FLOAT_ZERO            0
#define K_FLOAT_ONE             1
#else
#define K_FLOAT_ZERO            0.0
#define K_FLOAT_ONE             1.0
#endif/*K_USING_NOFLOAT*/

#if defined(K_USING_LONGDOUBLE) && !defined(K_FLOAT_FMT)
typedef long double               knh_float_t;
#define K_FLOAT_MAX             LDBL_MAX
#define K_FLOAT_MIN             (-(LDBL_MAX))
#define K_FLOAT_STEP            LDBL_MIN
#define K_FLOAT_FMT             "%.5Lf"
#define K_FLOAT_FMT1            "%.1Lf"
#define K_FLOAT_FMT2            "%.2Lf"
#define K_FLOAT_FMT3            "%.3Lf"
#define K_FLOAT_FMT4            "%.4Lf"
#define K_FLOAT_FMTE            "%Le"
#endif

#if defined(K_USING_FLOAT) && !defined(K_FLOAT_FMT)
typedef float                     knh_float_t;
#define K_FLOAT_MAX             FLT_MAX
#define K_FLOAT_MIN             (-(FLT_MAX))
#define K_FLOAT_STEP            FLT_MIN
#define K_FLOAT_FMT             "%.5f"
#define K_FLOAT_FMT1            "%.1f"
#define K_FLOAT_FMT2            "%.2f"
#define K_FLOAT_FMT3            "%.3f"
#define K_FLOAT_FMT4            "%.4f"
#define K_FLOAT_FMTE            "%e"
#endif

#ifndef K_FLOAT_FMT
typedef double                    knh_float_t;
#define K_FLOAT_MAX             DBL_MAX
#define K_FLOAT_MIN             (-(DBL_MAX))
#define K_FLOAT_STEP            DBL_MIN
#define K_FLOAT_FMT             "%.5f"
#define K_FLOAT_FMT1            "%.1f"
#define K_FLOAT_FMT2            "%.2f"
#define K_FLOAT_FMT3            "%.3f"
#define K_FLOAT_FMT4            "%.4f"
#define K_FLOAT_FMTE            "%e"
#endif

#ifndef K_FLOAT_NAN
#define K_FLOAT_NAN             (0.0/0.0)
#define K_FLOAT_NULL            K_FLOAT_NAN
#define K_FLOAT_FMTSIZ          80
#endif /*K_FLOAT_NAN*/

/* ------------------------------------------------------------------------ */
/* String, knh_uchar_t */
/* ------------------------------------------------------------------------ */

typedef unsigned char           knh_uchar_t;    /* byte */
typedef const char              knh_text_t;
typedef const unsigned char     knh_ustr_t;

typedef struct {
	union {
		const char *text;
		const unsigned char *ustr;
		knh_uchar_t *ubuf;    // deprecated in the future
	};
	size_t       len;
} knh_bytes_t;

#define ismulti(c)             (((knh_uchar_t)c)>127)

#define B(c)      new_bytes((char*)c)
#define STEXT(c)  new_bytes2(c,sizeof(c)-1)
#define ISB(t,c) (t.len == (sizeof(c)-1) && knh_strncmp(t.text, c, t.len) == 0)

/* ------------------------------------------------------------------------ */
/* knh_flag_t */
/* ------------------------------------------------------------------------ */

typedef knh_ushort_t              knh_flag_t;    /* flag field */

#define K_FLAG_T0     ((knh_flag_t)(1 << 15))
#define K_FLAG_T1     ((knh_flag_t)(1 << 14))
#define K_FLAG_T2     ((knh_flag_t)(1 << 13))
#define K_FLAG_T3     ((knh_flag_t)(1 << 12))
#define K_FLAG_T4     ((knh_flag_t)(1 << 11))
#define K_FLAG_T5     ((knh_flag_t)(1 << 10))
#define K_FLAG_T6     ((knh_flag_t)(1 << 9))
#define K_FLAG_T7     ((knh_flag_t)(1 << 8))
#define K_FLAG_T8     ((knh_flag_t)(1 << 7))
#define K_FLAG_T9     ((knh_flag_t)(1 << 6))
#define K_FLAG_T10    ((knh_flag_t)(1 << 5))
#define K_FLAG_T11    ((knh_flag_t)(1 << 4))
#define K_FLAG_T12    ((knh_flag_t)(1 << 3))
#define K_FLAG_T13    ((knh_flag_t)(1 << 2))
#define K_FLAG_T14    ((knh_flag_t)(1 << 1))
#define K_FLAG_T15    ((knh_flag_t)(1 << 0))

#define FLAG_set(f,op)     (f) = ((f)|(op))
#define FLAG_unset(f,op)   (f) = ((f)&(~(op)))
#define FLAG_is(f,op)      (((f) & (op)) == (op))

/* ------------------------------------------------------------------------ */
/* Struct, Class, Type  */
/* ------------------------------------------------------------------------ */

struct knh_Context_t;
typedef const struct knh_Context_t    Ctx;

typedef knh_ushort_t       knh_class_t;  /* class id */
typedef knh_ushort_t       knh_type_t;   /* extended knh_type_t */
typedef knh_ushort_t       knh_ebi_t;    /* knh_ebi_t */

/* knh_class_t */
#define CLASS_newid                ((knh_class_t)-1)
#define CLASS_unknown              ((knh_class_t)-2)

#define DBG_ASSERT_cid(cid)        DBG_ASSERT(cid < ctx->share->ClassTBLSize)
#define CLASS__(cid)               knh_ClassTBL_CLASS__(ctx, cid)
#define bcid__(bcid)               knh_getStructName(ctx, bcid)
#define O__(o)                     knh_ClassTBL_CLASS__(ctx, knh_Object_cid(o))

/* knh_type_t */
#define TYPE_var            CLASS_Tvar
#define TYPE_void           CLASS_Tvoid
#define TYPE_T0             ((knh_type_t)30000)
#define TYPE_This           TYPE_T0
#define TYPE_T(n)           (TYPE_T0+(n))
#define TYPE_T1             TYPE_T(1)
#define TYPE_T2             TYPE_T(2)
#define TYPE_T3             TYPE_T(3)
#define TYPE_T4             TYPE_T(4)

#define CLASS_type(t)       (((t) > TYPE_T0) ? TYPE_var : (t))
#define TYPE_cid(t)         (t)

#define IS_Tint(t)        (t == CLASS_Int || ClassTBL(CLASS_type(t)).bcid == CLASS_Int)
#define IS_Tfloat(t)      (t == CLASS_Float || ClassTBL(CLASS_type(t)).bcid == CLASS_Float)
#define IS_Tbool(t)       (TYPE_Boolean == t)
#define IS_Tunbox(t)      (IS_Tint(t) || IS_Tfloat(t) || IS_Tbool(t))
#define IS_Tfunc(t)       (ClassTBL(CLASS_type(t)).bcid == CLASS_Func)
#define IS_Tstr(t)        (t == CLASS_String || ClassTBL(CLASS_type(t)).bcid == CLASS_String)

#define knh_Method_isPoly(mtd, T) \
	(DP(mtd)->cid == T || knh_class_bcid(T) == DP(mtd)->cid)

#define knh_Method_isPoly0(mtd, T) \
	(DP(mtd)->cid == T || knh_class_bcid(T) == DP(mtd)->cid || knh_class_instanceof(ctx, cid, DP(mtd)->cid))

#define TYPE__(type)      knh_TYPE__(ctx,type)

/* knh_ebi_t */
#define EBI_unknown            ((knh_ebi_t)-1)
#define EBI_newid              ((knh_ebi_t)0)
#define ASSERT_ebi(eid)        DBG_ASSERT(eid < ctx->share->EventTBLSize + 1)
#define EBI__(eid)             S_tochar(knh_getExptName(ctx, eid))

/* ------------------------------------------------------------------------ */

typedef knh_ushort_t          knh_fieldn_t;
typedef knh_ushort_t          knh_methodn_t;

#define FN_NONAME    ((knh_fieldn_t)-1)
#define FN_NEWID     ((knh_fieldn_t)-2)
#define FN_return    FN_
#define MN_NONAME   ((knh_methodn_t)-1)
#define MN_NEWID    ((knh_methodn_t)-2)

#define FN_K FN_k
#define FN_V FN_v
#define FN_T FN_t
#define FN_U FN_u
#define FN_P FN_p
#define FN_R FN_r

#define K_FLAG_FN_U1         K_FLAG_T0
#define K_FLAG_FN_U2         K_FLAG_T1
#define K_FLAG_FN_SUPER      (K_FLAG_T0|K_FLAG_T1)

#define FN_isSUPER(fnq)      ((fnq & K_FLAG_FN_SUPER) == K_FLAG_FN_SUPER)
#define FN_isU1(fnq)         ((fnq & K_FLAG_FN_U1) == K_FLAG_FN_U1)
#define FN_isU2(fnq)         ((fnq & K_FLAG_FN_U2) == K_FLAG_FN_U2)

#define FN_UNMASK(fnq)       (fnq & (~(K_FLAG_FN_SUPER|K_FLAG_FN_U1|K_FLAG_FN_U2)))

#define K_FLAG_MN_ISBOOL     K_FLAG_T0
#define K_FLAG_MN_GETTER     K_FLAG_T1
#define K_FLAG_MN_SETTER     K_FLAG_T2
#define K_FLAG_MN_FMT        (K_FLAG_T0|K_FLAG_T1|K_FLAG_T2)
#define K_FLAG_MN_FIELDN     (~K_FLAG_MN_FMT)

#define MN_isISBOOL(mn)   ((mn & K_FLAG_MN_FMT) == K_FLAG_MN_ISBOOL)
#define MN_toISBOOL(mn)   (mn | K_FLAG_MN_ISBOOL)
#define MN_isGETTER(mn)   ((mn & K_FLAG_MN_FMT) == K_FLAG_MN_GETTER)
#define MN_toGETTER(mn)   (mn | K_FLAG_MN_GETTER)
#define MN_isSETTER(mn)   ((mn & K_FLAG_MN_FMT) == K_FLAG_MN_SETTER)
#define MN_toSETTER(mn)   (mn | K_FLAG_MN_SETTER)
#define MN_isFMT(mn)      ((mn & K_FLAG_MN_FMT) == K_FLAG_MN_FMT)
#define MN_toFMT(mn)      (mn | K_FLAG_MN_FMT)

#define MN_toFN(mn)       (mn & K_FLAG_MN_FIELDN)

#define MN_LAMBDA          FN_

const char *knh_getopname(knh_methodn_t mn);
#define FN__(fn)          S_tochar(knh_getFieldName(ctx, fn))
#define MN__(mn)          knh_getmnname(ctx, mn)

/* ------------------------------------------------------------------------ */
/* Object */
/* ------------------------------------------------------------------------ */

#define K_OBJECT_MAGIC         578

#ifdef K_USING_RCGC
#define KNH_HOBJECT_REFC      1
#endif

#ifdef K_OBJECT_MAGIC
	#define DBG_ASSERT_ISOBJECT(o)        DBG_ASSERT((o)->h.magic == K_OBJECT_MAGIC)
#else/*KONOHA_OBJECT_MAGIC*/
	#define DBG_ASSERT_ISOBJECT(o)
#endif/*KONOHA_OBJECT_MAGIC*/

typedef struct knh_hObject_t {
	knh_flag_t  flag;   knh_ushort_t magic;
	knh_class_t  bcid;  knh_class_t cid;
	union {
		knh_uintptr_t refc;
		void *gcinfo;
	};
	void *meta;   // reserved for traits
} knh_hObject_t ;

typedef struct knh_Object_t {
	knh_hObject_t h;
	void *ref;
	void *ref2_unused;
	void *ref3_unused;
	void *ref4_unused;
} knh_Object_t ;

#define K_FASTMALLOC_SIZE     sizeof(knh_Object_t)

#define SP(o)               (o)
#define DP(o)               ((o)->b)
#define _(s)          s

#define knh_bodycpy(o, s) \
	o->ref = s->ref;\
	o->ref2_unused = s->ref2_unused;\
	o->ref3_unused = s->ref3_unused;\
	o->ref4_unused = s->ref4_unused;\

#define knh_bodyextcpy(o, s) \
	o->ref2_unused = s->ref2_unused;\
	o->ref3_unused = s->ref3_unused;\
	o->ref4_unused = s->ref4_unused;\

/* types of basic objects (not type-checked) */

#define Object          knh_Object_t
#define ObjectNULL      Object
#define Any             knh_Object_t
#define This            knh_Object_t
#define T1              knh_Object_t
#define T2              knh_Object_t
#define UPCAST(o)       (Object*)(o)

#define knh_Object_toNULL(ctx, o)   knh_Object_toNULL_(ctx, UPCAST(o))
#define knh_Object_data(o)       (((knh_Int_t*)(o))->n.data)

/* ------------------------------------------------------------------------ */
/* Common Object Structure */

typedef knh_ushort_t              knh_uri_t;
#define URI_EVAL                  ((knh_uri_t)0)
#define URI_UNMASK(uri)           (uri)

//#define K_FLAG_URI_UNTRUSTED      K_FLAG_T0
//#define URI_UNMASK(uri)           (uri & (~(K_FLAG_URI_UNTRUSTED)))
//#define URI_TRUSTED(uri)          (uri & (~(K_FLAG_URI_UNTRUSTED)))
//#define URI_UNTRUSTED(uri)        (uri | K_FLAG_URI_UNTRUSTED)
//#define URI_ISTRUSTED(uri)        ((uri & K_FLAG_URI_UNTRUSTED) == 0)
//#define NSN_main           0
//#define FILENAME___unknown      0

#define URI__(uri) S_tochar(knh_getURN(ctx, uri))
#define FILENAME__(uri) knh_sfile(URI__(uri))

/* ------------------------------------------------------------------------ */
/* Thread */
/* ------------------------------------------------------------------------ */

#if defined(K_USING_PTHREAD)
#define knh_thread_t pthread_t
#define knh_thread_key_t pthread_key_t
#define knh_mutex_t pthread_mutex_t
#elif defined(K_USING_BTRON)
#define knh_thread_t W
#define knh_thread_key_t W
#define knh_mutex_t W
#else
typedef knh_intptr_t knh_thread_t;
typedef knh_intptr_t knh_thread_key_t;
typedef knh_intptr_t knh_mutex_t;
#endif

typedef void *(*knh_Fthread)(void *);

#define LOCK_NOP          ((knh_lock_t)0)
#define LOCK_MEMORY       ((knh_lock_t)1)
#define LOCK_SYSTBL       ((knh_lock_t)2)
#define LOCK_UNUSED       3
#define OLD_LOCK(ctx, lockid, o)
#define OLD_UNLOCK(ctx, lockid, o)

/* ------------------------------------------------------------------------ */
/* Stack Frame Pointer */
/* ------------------------------------------------------------------------ */

typedef struct knh_sfp_t {
	union {
		knh_boolean_t bvalue;
		knh_int_t     ivalue;
		knh_uint_t    uvalue;
		knh_float_t   fvalue;
		knh_uint64_t  data;
		knh_uintptr_t shift;
		struct knh_opline_t  * pc;
		struct knh_Method_t * callmtd;
		struct knh_Translator_t * casttrl;
	};
	union {
		void   *ref;
		Object *o;
		struct knh_Int_t    *i;
		struct knh_Float_t  *f;
		struct knh_Class_t  *c;
		struct knh_String_t *s;
		struct knh_Bytes_t  *ba;
		struct knh_Regex_t  *re;
		struct knh_Tuple_t  *tuple;
		struct knh_Range_t  *range;
		struct knh_Array_t  *a;
		struct knh_Iterator_t *it;
		struct knh_Map_t    *m;
		struct knh_Func_t         *fo;
		struct knh_InputStream_t  *in;
		struct knh_OutputStream_t *w;
		struct knh_Method_t *mtdOBJ;
		struct knh_Translator_t *trlOBJ;
		struct knh_Exception_t *e;
		struct knh_ExceptionHandler_t *hdr;
		struct knh_NameSpace_t *ns;
		struct knh_RawPtr_t   *p;
		struct knh_ObjectField_t *ox;
		struct knh_Converter_t *conv;
	};
} knh_sfp_t;

/* ------------------------------------------------------------------------ */
/* [Context] */
/* ------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------ */
/* [ObjectFunc] */

typedef void (*knh_Ftraverse)(Ctx *ctx, Object *);
#define KNH_FTR(ctx, ftr, p)       ftr(ctx, UPCAST(p))
#define KNH_NULLFTR(ctx, ftr, p)   if(p != NULL) ftr(ctx, UPCAST(p))

typedef knh_uintptr_t                knh_hashcode_t;  /* knh_hashcode_t */
#define knh_hcode_join(s1,s2)	   ((knh_hashcode_t)s1 << (sizeof(knh_short_t)*8)) + s2;

#define KNH_FOBJECT_HASH  0    /*knh_fobject_hashkey */
#define KNH_FOBJECT_KEY   1    /*knh_fobject_hashkey */

typedef FASTAPI(void) (*knh_Fobject_init)(Ctx *, Object *);
typedef FASTAPI(void) (*knh_Fobject_initcopy)(Ctx *, Object *, Object *);
typedef FASTAPI(void) (*knh_Fobject_traverse)(Ctx *, Object*, knh_Ftraverse);
typedef FASTAPI(void) (*knh_Fobject_free)(Ctx *, Object *);
typedef FASTAPI(void) (*knh_Fobject_checkout)(Ctx *, Object*, int);
typedef int     (*knh_Fobject_compareTo)(Ctx *ctx, Object*, Object*);
typedef FASTAPI(void*) (*knh_Fstack_hashkey)(Ctx *, knh_sfp_t *, int);
typedef struct  knh_Translator_t* (*knh_Fgenmap)(Ctx *, knh_class_t, knh_class_t);

typedef struct {
	const char             *name;
	knh_ushort_t            size;
	knh_flag_t              cflag;
	knh_Fobject_init        init;
	knh_Fobject_initcopy    initcopy;
	knh_Fobject_traverse    traverse;
	knh_Fobject_free        free;
	knh_Fobject_checkout    checkout;
	knh_Fobject_compareTo   compareTo;
	knh_Fstack_hashkey      hashkey;
	knh_Fgenmap             genmap;
} knh_ObjectCSPI_t ;

#define K_NUMBERCSPI_MAGIC   ((size_t)1234567)
typedef FASTAPI(knh_int_t) (*knh_Fnumber_toint)(Ctx *, knh_sfp_t *);
typedef FASTAPI(knh_float_t) (*knh_Fnumber_tofloat)(Ctx *, knh_sfp_t *);

typedef struct {
	knh_ObjectCSPI_t        common;
	size_t                  magic;
	knh_Fnumber_toint       to_int;
	knh_Fnumber_tofloat     to_float;
} knh_NumberCSPI_t ;

/* ------------------------------------------------------------------------ */

#define K_CLASSTABLE_INIT 128
#define SIZEOF_TCLASS(n)  ((n) * sizeof(knh_ClassTBL_t))
typedef knh_Object_t* (*knh_Fdefnull)(Ctx *ctx, knh_class_t cid);

typedef struct {
	knh_flag_t    flag  ;
	knh_short_t   idx   ;
	knh_type_t    type  ;
	knh_fieldn_t  fn    ;
	Object        *value;
	union {
		knh_int_t   ivalue;
		knh_float_t fvalue;
		knh_bool_t  bvalue;
		void       *ref;
	};
} knh_fields_t ;

typedef struct {
	knh_flag_t    cflag;   knh_flag_t    oflag;
	knh_class_t   bcid;    knh_short_t   keyidx;
	knh_class_t   supcid;  knh_ushort_t  offset;
	union {
		knh_ObjectCSPI_t *cspi;
		knh_NumberCSPI_t *numcspi;
	};
	size_t size;
	struct knh_String_t       *lname;
	struct knh_String_t       *sname;
	struct knh_Class_t        *typeNULL;
	struct knh_ParamArray_t   *cparam;
	knh_fields_t              *fields;
	size_t                     fsize;
	struct knh_Array_t        *methods;
	struct knh_Array_t        *tmaps;
	union {
		struct knh_Object_t       *defnull;
		struct knh_Func_t         *deffunc;
//		struct knh_Semantics_t    *cspec;
	};
	knh_Fdefnull                   fdefnull;
	struct knh_DictCaseMap_t      *constDictCaseMapNULL;
#if defined(K_USING_STATCLASS)
	size_t count;
	size_t total;
#endif
} knh_ClassTBL_t;

#define knh_class_bcid(c)   ClassTBL(c).bcid
#define knh_class_p1(c)     knh_class_p(ctx, c, 0)
#define knh_class_p2(c)     knh_class_p(ctx, c, 1)

/* ------------------------------------------------------------------------ */

#ifndef K_EVENTTBL_INIT
#define K_EVENTTBL_INIT 64
#endif

#define SIZEOF_TEXPT(n)  (n * sizeof(knh_EventTBL_t))

typedef struct {
	knh_flag_t   flag;
	knh_ebi_t   parent;
	struct knh_String_t     *name;
} knh_EventTBL_t;

/* ------------------------------------------------------------------------ */
/* [SystemStat] */

typedef struct {
	size_t usedMemorySize;
	size_t maxMemoryUsage;
	size_t usedObjectSize;
	size_t maxObjectUsage;
	size_t countObjectGeneration;
	size_t countMemorySize1;
	size_t countMemorySize2;
	size_t countMemorySize4;
	size_t countMemorySize8;
	size_t countMemorySizeN;
	size_t mtdCacheHit;
	size_t mtdCacheMiss;
	size_t fmtCacheHit;
	size_t fmtCacheMiss;
	size_t trlCacheHit;
	size_t trlCacheMiss;
} knh_stat_t;

#define knh_useMemory(ctx, n) \
	(ctx->stat)->usedMemorySize += (n);\
	if(ctx->stat->usedMemorySize > ctx->stat->maxMemoryUsage) ctx->stat->maxMemoryUsage = ctx->stat->usedMemorySize;\

#define knh_unuseMemory(ctx, n)   (ctx->stat)->usedMemorySize -= (n)

#define knh_useObject(ctx, n)\
	(ctx->stat)->usedObjectSize += (n);\
	if(ctx->stat->usedObjectSize > ctx->stat->maxObjectUsage) ctx->stat->maxObjectUsage = ctx->stat->usedObjectSize;\

#define knh_unuseObject(ctx, n)   (ctx->stat)->usedObjectSize -= (n)

#define knh_countObjectGeneration(ctx)   (ctx->stat)->countObjectGeneration += 1
#define knh_countMemorySize(ctx, SIZE)   (ctx->stat)->countMemorySize##SIZE = ctx->stat->countMemorySize##SIZE + 1

#define knh_stat_mtdCacheHit(ctx)   (ctx->stat)->mtdCacheHit += 1
#define knh_stat_mtdCacheMiss(ctx)  (ctx->stat)->mtdCacheMiss += 1
#define knh_stat_fmtCacheHit(ctx)   (ctx->stat)->fmtCacheHit += 1
#define knh_stat_fmtCacheMiss(ctx)  (ctx->stat)->fmtCacheMiss += 1
#define knh_stat_trlCacheHit(ctx)   (ctx->stat)->trlCacheHit += 1
#define knh_stat_trlCacheMiss(ctx)  (ctx->stat)->trlCacheMiss += 1

/* ------------------------------------------------------------------------ */
/* [SystemShare] */

#define SIZEOF_TSTRING (sizeof(knh_Object_t*) * K_TSTRING_SIZE)

#define KNH_ENC             DP(ctx->sys)->enc

#define KNH_NULL            (ctx->share->constNull)
#define KNH_TRUE            (ctx->share->constTrue)
#define KNH_FALSE           (ctx->share->constFalse)
#define KNH_INT0            (ctx->share->constInt0)
#define KNH_FLOAT0          (ctx->share->constFloat0)
#define KNH_EMPTYLIST       (ctx->share->emptyArray)
#define KNH_SYSTEM          (ctx->sys)
#define knh_Object_sweep    (ctx)->fsweep

/* ------------------------------------------------------------------------ */
/* Arena */

#define K_OARENASIZE ((K_PAGESIZE / sizeof(knh_Object_t)) - 1)

typedef struct {
	knh_uintptr_t bitmap[sizeof(knh_Object_t)/(sizeof(knh_uintptr_t))];
} knh_hOArena_t;

#define knh_Object_getArena(o)  (knh_hOArena_t*)((((knh_uintptr_t)(o)) / K_PAGESIZE) * K_PAGESIZE)

typedef struct {
	knh_hOArena_t head;
	knh_Object_t slots[K_OARENASIZE];
} knh_OArena_t;

#define KNH_ARENA_MAX(p, size)   ((char*)p + size)

typedef struct {
	knh_flag_t   flag;  // used by nakata GC team
	knh_ushort_t isObjectArena;
	Ctx *ctx;           // context, using this arena
	void *head;         // allocated head;
	void *bottom;       // head <= p < bottom
	struct {
		knh_OArena_t  *oslot;
		char          *mslot32[32];
		char          *mslot64[64];
		char          *mslot128[128];
		char          *mslot256[256];
	};
	size_t            arena_size;
	size_t            slot_size;
	void *meta;       // used by nakata GC team
} knh_ArenaSet_t ;

#define K_ARENASET_INITSIZE     (K_PAGESIZE / sizeof(knh_ArenaSet_t))
#define K_ARENASIZE             (sizeof(knh_Object_t) * 4096)

typedef struct {
	/* system table */
	knh_ArenaSet_t           *ArenaSet;
	size_t ArenaSetSize;
	size_t ArenaSetMax;
	const knh_ClassTBL_t   *ClassTBL;
	size_t ClassTBLSize;
	size_t ClassTBLMax;
	const knh_EventTBL_t    *EventTBL;
	size_t EventTBLSize;
	size_t EventTBLMax;

	/* system shared const */
	knh_Object_t         *constNull;
	knh_Object_t         *constTrue;
	knh_Object_t         *constFalse;
	struct knh_Int_t     *constInt0;
	struct knh_Float_t   *constFloat0;
	struct knh_String_t  **tString;
	struct knh_Array_t   *emptyArray;
	struct knh_Context_t     *ctx0;
	struct knh_NameSpace_t   *mainns;
	struct knh_Script_t      *script;
	struct knh_opline_t      *PC_LAUNCH;
	struct knh_opline_t      *PC_FUNCCALL;
	struct knh_opline_t      *PC_VEXEC;
	struct knh_opline_t      *PC_ABSTRACT;

	/* spi */
	const struct knh_ShellSPI_t       *shellSPI;
	const struct knh_ConverterDSPI_t  *iconvDSPI;
	const struct knh_EvidenceSPI_t    *ebiSPI;
	const struct knh_CompilerSPI_t    *jitSPI;
	const struct knh_SyncSPI_t      *syncSPI;

	/* thread */
	size_t              contextCounter;
	size_t              threadCounter;
} knh_share_t ;

#define KNH_ASSERT_CTX0(ctx)   KNH_ASSERT((ctx)->ctxid == 0)

#define ClassTBL(cid)   ctx->share->ClassTBL[cid]
#define pClassTBL(ctx, cid)  (knh_ClassTBL_t*)((ctx)->share->ClassTBL + (cid))
#define EventTBL(eid)    ctx->share->EventTBL[eid]
#define pEventTBL(eid)   (knh_EventTBL_t*)(ctx->share->EventTBL + (eid))

#define knh_setClassDefaultValue(ctx, cid, v, f) knh_setClassDefaultValue_(ctx, cid, UPCAST(v), f)

/* ------------------------------------------------------------------------ */

typedef struct knh_Context_t {
	knh_hObject_t h;
	/* stack */
	knh_sfp_t*                   stack;
	knh_sfp_t*                   esp;
	size_t                       stacksize;
	knh_sfp_t*                   stacktop;
	void*                        cstack_bottom;
	struct knh_Exception_t      *e;

	/* memory */
	knh_Object_t                *unusedObject;
	size_t                       unusedObjectSize;
	knh_Ftraverse                fsweep;

	/* cache */
	struct knh_Method_t         **mtdCache;
	struct knh_Method_t         **fmtCache;
	struct knh_Translator_t     **trlCache;

	/* shared table */
	const knh_share_t           *share;
	knh_stat_t                  *stat;
	struct knh_System_t*         sys;
	struct knh_Script_t*         script;

	struct knh_String_t*         enc;
	struct knh_InputStream_t*    in;
	struct knh_OutputStream_t*   out;
	struct knh_OutputStream_t*   err;
	struct knh_Bytes_t*          bufa;
	struct knh_OutputStream_t*   bufw;
	struct knh_Gamma_t          *gma;

	knh_flag_t                   flag;
	knh_ushort_t                 ctxid;
	const struct knh_Context_t   *parent;
	knh_mutex_t                  *ctxlock;

	const struct knh_ExportsAPI_t *api;
} knh_Context_t ;


/* ------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------ */

#define KONOHA_MAGIC        314159

typedef struct konoha_t {
	knh_uintptr_t  magic;
	Ctx *ctx;
} konoha_t ;

/* ------------------------------------------------------------------------ */

#define KONOHA_CHECK_(konoha) \
	if(konoha.magic != KONOHA_MAGIC) { \
		fprintf(stderr, "This is not a Konoha Scripting Engine\n"); \
		return; \
	}\

#define KONOHA_CHECK(konoha, value) \
	if(konoha.magic != KONOHA_MAGIC) { \
		fprintf(stderr, "This is not a Konoha Scripting Engine\n"); \
		return value; \
	}\

#define KONOHA_BEGIN(v)   knh_beginContext(v, &(v))
#define KONOHA_END(v)     knh_endContext(v)

/* ------------------------------------------------------------------------ */
/* cwb */
/* ------------------------------------------------------------------------ */

typedef struct {
	struct knh_Bytes_t          *ba;
	struct knh_OutputStream_t   *w;
	size_t pos;
} knh_cwb_t;

#define knh_cwb_tochar(ctx, cwb)      ctx->api->cwb_tochar(ctx, cwb)
#ifdef K_EXPORTS
#define knh_cwb_clear(cwb,len)        ctx->api->cwb_clear(cwb, cwb->pos+len)
#define knh_cwb_close(cwb)            ctx->api->cwb_clear(cwb->ba, cwb->pos)
#else
#define knh_cwb_clear(cwb,len)        knh_Bytes_clear(cwb->ba, cwb->pos+len)
#define knh_cwb_close(cwb)            knh_Bytes_clear(cwb->ba, cwb->pos)
#endif

/* ------------------------------------------------------------------------ */
/* Functions */
/* ------------------------------------------------------------------------ */

#ifdef KONOHA_ON_WINDOWS
#define METHOD  void K_CC_FASTCALL
#define TCAST   METHOD
#define ITRNEXT int   K_CC_FASTCALL
typedef void (K_CC_FASTCALL *knh_Fmethod)(Ctx *, knh_sfp_t*, long rix);
typedef void (K_CC_FASTCALL *knh_Ftranslator)(Ctx *, knh_sfp_t *, long rix);
typedef int  (K_CC_FASTCALL *knh_Fitrnext)(Ctx *, knh_sfp_t *, long rtnidx);
#else
#define METHOD  void  K_CC_FASTCALL
#define TCAST   METHOD
#define ITRNEXT int   K_CC_FASTCALL
typedef METHOD (*knh_Fmethod)(Ctx*, knh_sfp_t*, long rix);
typedef TCAST (*knh_Ftranslator)(Ctx *, knh_sfp_t *, long rix);
typedef ITRNEXT (*knh_Fitrnext)(Ctx *, knh_sfp_t *, long rtnidx);
#endif

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif /*KONOHA_T_H_*/
