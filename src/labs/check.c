#include "commons.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__x86_64__) || defined(_M_X64)
#define CPU_NAME "x86_64"
#elif defined(__i386__) || defined(_M_IX86)
#define CPU_NAME "i386"
#elif defined(__amd64__) || defined(_M_AMD64)
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
	knh_snprintf(url, 512, "%s?dist=%s&ver=%s&arch=%s(%s)&rev=%d",
			UPDATE_SERVER, KONOHA_DIST, KONOHA_VERSION, KONOHA_PLATFORM,
			CPU_NAME, KONOHA_REVISION);

	knh_iodrv_t *d = knh_getIODriver(ctx, STEXT("http"));
	knh_io_t fd = d->fopen(ctx, B(url), "r", 0);
	if (fd != -1) {
		d->fread(ctx, fd, url, 512);
		// parse if its update
		char *comment_in = NULL;
		if ((comment_in = strstr(url, "\r\n\r\n<!--")) != 0) {
		  comment_in += sizeof("\r\n\r\n<!--");
		  // we want to write cwb, but we need lock.
		  char *comment_out = strstr(url, "-->");
		  if (comment_out) {
			comment_in[comment_out - comment_in] = '\0';
			fprintf(stdout, "%s \n>>>", comment_in);
			fflush(stdout);
		  }
		}
		d->fclose(ctx, fd);
	}
}

/*--------------------------------------------------------------*/
void knh_check_update(Ctx *ctx)
{
	knh_thread_t th;
	threadfunc_t func = (threadfunc_t) knh_check_exec;
	knh_thread_create(ctx, &th, NULL, func, (void*)ctx);
	knh_thread_detach(ctx, th);
}
/*--------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
