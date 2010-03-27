#include <konoha.h>
#include <tk/syslib.h>
#include <btron/tkcall.h>
#include <tstring.h>
#define DN_LECHIP -200
#define DN_LEMode  -200
#define DN_LEDEC  -203

/* -------------------------------------------------------------------------*/
//## method void System.chipLed(Int! x);

METHOD System_chipLed(Ctx *ctx, knh_sfp_t *sfp)
{
	int x = p_int(sfp[1]);
	int dd, asize;
	dd = tk_opn_dev("chip_led", TD_UPDATE);
	tk_swri_dev(dd, DN_LECHIP, &x, sizeof(x), &asize);
	KNH_RETURN_void(ctx, sfp);
}
/* -------------------------------------------------------------------------*/
//## method void System.segLed(Int! x);

METHOD System_segLed(Ctx *ctx, knh_sfp_t *sfp)
{
	int x = p_int(sfp[1]);
	int dd, asize;
	dd = tk_opn_dev("led", TD_UPDATE);
	tk_swri_dev(dd, DN_LEDEC, &x, sizeof(x), &asize);
	KNH_RETURN_void(ctx, sfp);
}

/* -------------------------------------------------------------------------*/
//## method void System.led(Int! x);

METHOD System_led(Ctx *ctx, knh_sfp_t *sfp)
{
	int v = p_int(sfp[1]);
	int ddl, asize;
	ddl = tk_opn_dev("led_test",TD_UPDATE);
	tk_swri_dev(ddl, DN_LEMode, &v, sizeof(v), &asize);
	KNH_RETURN_void(ctx, sfp);
}



#endif /* KONOHA_ON_TB */
