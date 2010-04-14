#include "commons.h"
#if defined(KNH_USING_POSIX)
#include <uuid/uuid.h>
#endif

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
#define UUID_FILE  ".konoha/knh.uuid"
/*--------------------------------------------------------------*/
static void setUUID(char *buffer)
{
#if defined(KNH_USING_POSIX)
	uuid_t uuid;
	uuid_generate(uuid);
	uuid_unparse(uuid, buffer);
#elif defined(KNH_USING_WINDOWS)
	/* TODO  Imprement uuid for Windows */
	knh_snprintf(buffer, 64, "I_like_Windows");
#else
	knh_snprintf(buffer, 64, "unknown");
#endif
}

/*--------------------------------------------------------------*/
static void send_user_record(Ctx *ctx, char *uuid)
{
	char url[512] = {0};
	knh_snprintf(url, 512, "%s?arch=%s(%s)&rev=%d&user_id=%s", 
			UPDATE_SERVER, KONOHA_PLATFORM,
			CPU_NAME, KONOHA_REVISION, uuid);

	knh_iodrv_t *d = knh_getIODriver(ctx, STEXT("http"));
	knh_io_t fd = d->fopen(ctx, B(url), "r", 0);
	if (fd != -1) {
		d->fread(ctx, fd, url, 512);
	}
	d->fclose(ctx, fd);
}

/*--------------------------------------------------------------*/
static void knh_check_exec(void *arg)
{
	Ctx *ctx = (Ctx*)arg;
	char filename[FILEPATH_BUFSIZ] = {0};
	char uuid[64] = {0};
	knh_iodrv_t *d;
	knh_io_t fp;

	knh_snprintf(filename, FILEPATH_BUFSIZ, 
			"%s/%s", knh_getenv("HOME"), UUID_FILE);

	d = knh_getIODriver(ctx, STEXT("file"));
	if ((fp = d->fopen(ctx, B(filename), "r", 0)) != -1) {
		d->fread(ctx, fp, uuid, 64);
	} else {
		setUUID(uuid);
		fp = d->fopen(ctx, B(filename), "w", 0);
		// write uuid to file
		d->fwrite(ctx, fp, uuid, 64);
	}
	d->fclose(ctx, fp);
	send_user_record(ctx, uuid);
}

/*--------------------------------------------------------------*/
void knh_check_update(Ctx *ctx)
{
	char filename[FILEPATH_BUFSIZ] = {0};
	knh_io_t fp;
	knh_iodrv_t *d;
	knh_thread_t th;

	knh_snprintf(filename, FILEPATH_BUFSIZ,
			"%s/%s", knh_getenv("HOME"), UUID_FILE);

	d = knh_getIODriver(ctx, STEXT("file"));
	if ((fp = d->fopen(ctx, B(filename), "r", 0)) != -1) {
		d->fclose(ctx, fp);
	} else {
		if (!knh_ask(ctx, 
					"Do you mind to send your environment information "
					"to update_server? [y/N]", 0)) {
			return;
		}
	}

	knh_thread_create(ctx, &th, NULL, knh_check_exec, (void*)ctx);
	knh_thread_detach(ctx, th);
}

#ifdef __cplusplus
}
#endif
