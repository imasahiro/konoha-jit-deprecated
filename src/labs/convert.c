#include <konoha.h>
#include <konoha/konoha_code_.h>

/* ======================================================================== */
static void knh_jit_nop_init(Ctx *ctx, knh_sfp_t *sfp, void **ref)
{
	/* direct threading */
	knh_KLRCode_exec(ctx, sfp);
}

static void knh_jit_nop_exit(Ctx *ctx, knh_sfp_t *sfp, void **data)
{
	/* do nothing */
}

/* ------------------------------------------------------------------------ */
static knh_code_t *knh_jit_nop(Ctx *ctx, knh_code_t *pc, void *data)
{
	return pc + knh_opcode_size(KNH_OPCODE(pc));
}

/* ------------------------------------------------------------------------ */
knh_jitDSPI_t JIT__NOP = {
	KNH_DRVAPI_TYPE__JIT,
	NULL,
	knh_jit_nop_init,
	knh_jit_nop_exit,
	{
		knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop,
		knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop,
		knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop,
		knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop,
		knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop,
		knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop,
		knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop,
		knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop,
		knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop,
		knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop,
		knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop,
		knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop,
		knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop,
		knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop,
		knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop,
		knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop,
		knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop,
		knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop,
		knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop,
		knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop,
		knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop,
		knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop,
		knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop,
		knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop,
		knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop,
		knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop,
		knh_jit_nop, knh_jit_nop, knh_jit_nop, knh_jit_nop
	}
};


static knh_jitDSPI_t *defaultDriver = &JIT__NOP;
/* ------------------------------------------------------------------------ */
knh_jitDSPI_t *knh_System_getDefaultJITDriver(void)
{
	return defaultDriver;
}

/* ------------------------------------------------------------------------ */
void knh_System_setDefaultJITDriver(knh_jitDSPI_t *driver)
{
	if (driver->type == KNH_DRVAPI_TYPE__JIT) {
		defaultDriver = driver;
	}
}


/* ------------------------------------------------------------------------ */

knh_jitDSPI_t *knh_System_getJITDriver(Ctx *ctx, knh_bytes_t name)
{
	if(ctx == NULL) {
		return knh_System_getDefaultJITDriver();
	}
	else {
			knh_jitDSPI_t *p = (knh_jitDSPI_t *)knh_getDriverAPI(ctx, KNH_DRVAPI_TYPE__JIT, name);
			if(p == NULL) {
				//KNH_WARNING(ctx, "OPT: unsupported optimizer '%s'", name);
				p = knh_System_getDefaultJITDriver();
			}
			return p;
	}
}

/* ------------------------------------------------------------------------ */

METHOD knh_KLRCode_optimize(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_loadDefaultOPTDriver(ctx);
	knh_jitDSPI_t *opt = knh_System_getJITDriver(ctx, 
#ifdef KNH_KLR_LLVM
			B("llvm")
#elif defined(KNH_KLR_CONVERT)
			B("ctt")
#else
			B("nop")
#endif
			);

	knh_code_t *pc = (sfp[-1].mtd)->pc_start;
	DBG2_P("optimizer='%s'", opt->name);
	void *data = NULL;
	opt->init(ctx, sfp, &data);
	while(KNH_OPCODE(pc) != OPCODE_RET) {
		pc = opt->func[KNH_OPCODE(pc)](ctx, pc, data);
	}
	// return inst
	opt->func[KNH_OPCODE(pc)](ctx, pc, data);
	opt->exit(ctx, sfp, &data);
}

/* ------------------------------------------------------------------------ */

KNHAPI(void) knh_addJITDriver(Ctx *ctx, char *alias, knh_jitDSPI_t *d)
{
	knh_addDriverAPI(ctx, alias, (knh_drvapi_t*)d);
}

void knh_loadDefaultOPTDriver(Ctx *ctx)
{
	knh_addJITDriver(ctx, NULL, &JIT__NOP);
	knh_addJITDriver(ctx, "nop", &JIT__NOP);
#ifdef KNH_KLR_CONVERT
	knh_addJITDriver(ctx, NULL, &JIT__CTT);
	knh_addJITDriver(ctx, "ctt", &JIT__CTT);
#endif
}

