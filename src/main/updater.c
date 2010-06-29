
#define USE_STEXT 1
#define USE_B     1

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

static void* knh_check_exec(void *arg)
{
	Ctx *ctx = (Ctx*)arg;
	char url[512] = {0};
	knh_snprintf(url, 512, "%s?dist=%s&ver=%s&arch=%s(%s)&rev=%d",
			UPDATE_SERVER, K_DIST, K_VERSION, K_PLATFORM,
			CPU_NAME, K_REVISION);
	knh_StreamDSPI_t *d = knh_getStreamDSPI(ctx, STEXT("http"));
	knh_io_t fd = d->fopen(ctx, B(url), "r");
	if (fd != -1) {
		d->fread(ctx, fd, url, 512);
		// parse if its update
		char *version_str = NULL;
		if ((version_str = strstr(url, "\r\n\r\n<!--")) != 0) {
		  version_str += sizeof("\r\n\r\n<!--");
		  // we want to write cwb, but we need lock.
		  version_str[sizeof("x.x.x")] = '\0';
		  fprintf(stdout, "[info]Please update Konoha to %s \n>>>", version_str);
		  fflush(stdout);
		}
		d->fclose(ctx, fd);
	}
	return NULL; // check out pthread_create interface
}

/*--------------------------------------------------------------*/

void knh_check_update(Ctx *ctx)
{
	knh_thread_t th;
	knh_thread_create(ctx, &th, NULL, knh_check_exec, (void*)ctx);
	knh_thread_detach(ctx, th);
}

/*--------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
