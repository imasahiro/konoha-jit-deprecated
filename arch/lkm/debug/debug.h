#define KONOHA_MACROS
#include <konoha.h>
#include <konoha/konoha_code_.h>
typedef int (*fopr_t)(Ctx *ctx, knh_sfp_t *sfp, knh_OutputStream_t *out, knh_bytes_t cmd);

struct op {
	char *name;
	int len;
	int next;
	fopr_t fopr;
};

#ifdef KONOHA_ON_LKM
#include "konoha/lkm.h"
#define IDEBUG(fmt, ...) \
	printk("DBG2[%d/%s] ",  __LINE__, __func__); \
	printk(fmt, ## __VA_ARGS__); \
	printk("\n"); \


#endif
#define __

#ifndef METHODOPT
#define METHODOPT 
#endif

#include "vm.h"

int op_null(Ctx *ctx, knh_sfp_t *sfp, knh_OutputStream_t *out, knh_bytes_t cmd);
int op_next(Ctx *ctx, knh_sfp_t *sfp, knh_OutputStream_t *out, knh_bytes_t cmd);

int op_print(Ctx *ctx, knh_sfp_t *sfp, knh_OutputStream_t *out, knh_bytes_t cmd);
int op_bt(Ctx *ctx, knh_sfp_t *sfp, knh_OutputStream_t *out, knh_bytes_t cmd);


typedef struct op opr_t;
#define OPR_SIZE ((sizeof(opr) / sizeof((opr)[0])) - 1)

#define KNH_BUF_MAX 256

METHOD System_debug(Ctx *ctx, knh_sfp_t *sfp METHODOPT);
METHOD Method_setDebugMode(Ctx *ctx, knh_sfp_t *sfp METHODOPT);
