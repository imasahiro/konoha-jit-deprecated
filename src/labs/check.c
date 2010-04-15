#include "commons.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__x86_64__)
#define CPU_NAME "x86_64"
#elif defined(__i386__)
#define CPU_NAME "i386"
#elif defined(__amd64__)
#define CPU_NAME "amd64"
#else
#define CPU_NAME "unknown"
#endif
#define UPDATE_SERVER "http://www.ubicg.ynu.ac.jp/cgi-bin/konoha-userdata/update_server"

/*--------------------------------------------------------------*/
static void knh_check_exec(void *arg)
{
	Ctx *ctx = (Ctx*)arg;
	char url[512] = {0};
	knh_snprintf(url, 512, "%s?var=%s&arch=%s(%s)&rev=%d",
			UPDATE_SERVER, KONOHA_VERSION, KONOHA_PLATFORM,
			CPU_NAME, KONOHA_REVISION);

	knh_iodrv_t *d = knh_getIODriver(ctx, STEXT("http"));
	knh_io_t fd = d->fopen(ctx, B(url), "r", 0);
	if (fd != -1) {
		d->fread(ctx, fd, url, 512);
		d->fclose(ctx, fd);
	}
}

/*--------------------------------------------------------------*/
void knh_check_update(Ctx *ctx)
{
	knh_io_t fp;
	knh_iodrv_t *d;
	knh_thread_t th;
	threadfunc_t func = (threadfunc_t) knh_check_exec;
	knh_thread_create(ctx, &th, NULL, func, (void*)ctx);
	knh_thread_detach(ctx, th);
}
/*--------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
