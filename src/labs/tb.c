/*
 * T-Board LED 
 * written by goccy54 (at) gmail.com
 */
#include "commons.h"
#ifdef KONOHA_ON_TB
#include <tk/syslib.h>
#include <btron/tVCALL.h>
#include <tstring.h>
#define DN_LECHIP -200
#define DN_LEMode  -200
#define DN_LEDEC  -203
#endif

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

char **knh_tcstoeucs(int argc, char **argv)
{
#ifdef KONOHA_ON_TB
	char buf[256];
	char** args = malloc(sizeof(char*) * (argc + 1));
	int i, len;

	for (i = 0; i < argc; i++) {
		len = tcstoeucs(buf, (TC*)argv[i]);
		if (len >= 0) {
			args[i] = malloc(len + 1);
			knh_memcpy(args[i], buf, len);
		}
	}
	args[argc] = NULL;
#else
	char **args = argv;
#endif /* KONOHA_ON_TB */
	return args;
}
///* -------------------------------------------------------------------------*/
////## method void System.chipLed(Int! x);
//
//static METHOD System_chipLed(Ctx *ctx, knh_sfp_t *sfp)
//{
//	int x = p_int(sfp[1]);
//	int dd, asize;
//	dd = tk_opn_dev("chip_led", TD_UPDATE);
//	tk_swri_dev(dd, DN_LECHIP, &x, sizeof(x), &asize);
//	KNH_RETURN_void(ctx, sfp);
//}
///* -------------------------------------------------------------------------*/
////## method void System.segLed(Int! x);
//
//static METHOD System_segLed(Ctx *ctx, knh_sfp_t *sfp)
//{
//	int x = p_int(sfp[1]);
//	int dd, asize;
//	dd = tk_opn_dev("led", TD_UPDATE);
//	tk_swri_dev(dd, DN_LEDEC, &x, sizeof(x), &asize);
//	KNH_RETURN_void(ctx, sfp);
//}
//
///* -------------------------------------------------------------------------*/
////## method void System.led(Int! x);
//
//static METHOD System_led(Ctx *ctx, knh_sfp_t *sfp)
//{
//	int v = p_int(sfp[1]);
//	int ddl, asize;
//	ddl = tk_opn_dev("led_test",TD_UPDATE);
//	tk_swri_dev(ddl, DN_LEMode, &v, sizeof(v), &asize);
//	KNH_RETURN_void(ctx, sfp);
//}
///* -------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
