//#include "commons.h"
//#include <uuid/uuid.h>
//
//#define UPDATE_SERVER_URL "http://www.ubicg.ynu.ac.jp/konoha-userdata/"
//
//#ifdef __cplusplus
//extern "C" {
//#endif
//
///*===============================================================*/
//// [knh_user_record]
//
//typedef struct _knh_user_record {
//	char uuid[37];
//	char arch[11];
//	char platform[16];
//} knh_user_record;
//
///*--------------------------------------------------------------*/
//// new
//static knh_user_record* knh_new_user_record()
//{
//	knh_user_record *record;
//	record = (knh_user_record*) malloc(sizeof(knh_user_record));
//	knh_bzero(record, sizeof(*record));
//
//	return record;
//}
//
///*--------------------------------------------------------------*/
//// dump to FILE*
//static void knh_dump_user_record(knh_io_t file, knh_iodrv_t *d, knh_user_record* user)
//{
//	d->fwrite(fp, "uuid=%s arch=%s platform=%s",
//			user->uuid,
//			user->arch,
//			user->platform);
//}
//
///*--------------------------------------------------------------*/
//// read from FILE*
//	static
//void knh_read_user_record(FILE* fp, knh_user_record* user)
//{
//	fscanf(fp, "uuid=%s arch=%s platform=%s",
//			&(user->uuid),
//			&(user->arch),
//			&(user->platform));
//}
//
///*--------------------------------------------------------------*/
//// send to UPDATE_SERVER
//	static
//int knh_send_user_record(Ctx *ctx,knh_user_record *user)
//{
//	int ret = 0;
//	char url[512] = {0};
//	sprintf(url, "%s?arch=%s(%s)&rev=%d&user_id=%s", 
//			UPDATE_SERVER_URL,
//			user->platform,
//			user->arch,
//			(size_t)KONOHA_REVISION, 
//			user->uuid);
//	knh_iodrv_t *d = knh_getIODriver(ctx, STEXT("http"));
//	knh_io_t http = d->fopen(ctx, B(url), "r", 0);
//	if (http != -1) {
//		d->fread(ctx, http, url, 512);
//
//		ret = 1; // success
//	}
//	d->fclose(ctx, http);
//	return ret;
//}
//
///*===============================================================*/
//// [records]
//
///*--------------------------------------------------------------*/
//// property (konoha.platform)
//static void setPlatform(knh_user_record *trac)
//{
//	sprintf(trac->platform, KONOHA_PLATFORM);
//}
//
///*--------------------------------------------------------------*/
//// uuid
//	static
//void setUUID(knh_user_record *trac)
//{
//	char *ret = trac->uuid;
//#if defined(KNH_USING_POSIX)
//	uuid_t uuid;
//	uuid_generate(uuid);
//	uuid_unparse(uuid, ret);
//#elif defined(KNH_USING_WINDOWS)
//
//#else
//	sprintf(ret, "unknown");
//#endif
//}
//
///*--------------------------------------------------------------*/
//// architecture
//static
//void setArch(knh_user_record *trac)
//{
//	const char *cpu_name = 
//#if defined(__x86_64__)
//		"x86_64";
//#elif defined(__i386__)
//	"i386";
//#elif defined(__amd64__)
//	"amd64";
//#else
//	"unknown";
//#endif
//	sprintf(trac->arch, cpu_name);
//}
//
///*===============================================================*/
//// [main]
//
//#define UPDATE_FLAG_FILE  ".konoha/knh_check.out"
///*--------------------------------------------------------------*/
//// main routine
//static
//void knh_check_exec(void *arg)
//{
//	Ctx *ctx = (Ctx*)arg;
//	char filename[FILEPATH_BUFSIZ] = {0};
//	sprintf(filename, "%s/%s", knh_getenv("HOME"), UPDATE_FLAG_FILE);
//
//	/* new & set members */
//	knh_user_record *rec = knh_new_user_record();
//
//	knh_iodrv_t *d = knh_getIODriver(ctx, STEXT("file"));
//	knh_io_t file = d->fopen(ctx, B(url), "r", 0);
//	if ((fp = d->fopen(filename, "r")) != NULL) {
//		knh_read_user_record(fp, rec);
//	} else {
//		setUUID(rec);
//		setPlatform(rec);
//		setArch(rec);
//		if ((file = f->fopen(filename, "w")) != NULL) {
//			knh_dump_user_record(fp, rec);
//		}
//	}
//	fclose(fp);
//	knh_send_user_record(ctx, rec);
//	free(rec);
//}
//
///*--------------------------------------------------------------*/
//// create thread
//
//void knh_check_update(Ctx *ctx)
//{
//	char filename[FILEPATH_BUFSIZ] = {0};
//	sprintf(filename, "%s/%s", knh_getenv("HOME"), UPDATE_FLAG_FILE);
//
//	FILE *fp;
//	if ((fp = fopen(filename, "r")) != NULL) {
//		fclose(fp);
//	} else {
//		if (!knh_ask(ctx, "Do you mind to send your environment information to update_server? [y/N]", 0)) {
//			return;
//		}
//	}
//	knh_thread_t th;
//	knh_thread_create(ctx, &th, NULL, knh_check_exec, (void*)ctx);
//	knh_thread_detach(ctx, th);
//}
//
///*===============================================================*/
//
//#ifdef __cplusplus
//}
//#endif
