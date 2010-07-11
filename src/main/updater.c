
#define USE_STEXT 1
#define USE_B     1

#include "commons.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/param.h> //
#include <sys/uio.h>
#ifdef KONOHA_ON_MACOSX
#include <sys/sysctl.h>
#endif /* Def: KONOHA_ON_MACOSX */
#ifdef KONOHA_ON_WINDOWS
#include "atrstl.h"
#endif /* Def: KONOHA_ON_WINDOWS */

#include <sys/sysctl.h>
#include <unistd.h>

#define CHANGE_COLOR(color) fprintf(stderr, "\x1b[%dm", color)

enum {
  BLACK = 30,
  RED,
  GREEN,
  YELLOW,
  BLUE,
  MAGENTA,
  SYAN,
  GRAY,
  DEFAULT,
  WHITE,
};

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
#define UPDATE_HOST "192.168.59.5"     //"localhost"
#define UPDATE_PATH "/cgi-bin/security-alert/update_server?dist=%s&ver=%s&arch=%s(%s)&rev=%d&clock=%ld&mem=%ld"
#define BUF_LEN 512

/*--------------------------------------------------------------*/

static void* knh_check_exec(void *arg)
{
	int fd;
	FILE *fp; //
	char str[512], *ptr; //
	Ctx *ctx = (Ctx*)arg;
	char path[BUF_LEN] = {0};
	long clock, mem;
#ifdef KONOHA_ON_MACOSX
	int sels[2] = { CTL_HW, HW_PHYSMEM };
	size_t length = sizeof(mem);
	sysctl(sels, 2, &mem, &length, NULL, 0);
	//fprintf(stderr, "mem %ld\n", mem);
	int sels2[2] = { CTL_HW, HW_CPU_FREQ };
	size_t len = sizeof(clock);
	sysctl(sels2, 2, &clock, &length, NULL, 0);
	//fprintf(stderr, "clock %ld\n", clock);
#endif
#if defined( KONOHA_ON_LINUX ) || defined( KONOHA_ON_WINDOWS )
	FILE *fp;
	int n = 0, m = 0 ;
	char buf[64];
	char cclock[64];
	char cmem[64];
	
	size_t len = strlen("cpu MHz");
	fp = fopen("/proc/cpuinfo","r");
	while(fgets( buf, 64, fp ) != NULL){
		if( strncmp(buf, "cpu MHz", len) == 0){
			//fprintf(stderr,buf);
			while( buf[n] != '\0'){
				while( buf[n] >= 48 && buf[n] < 58 ){
					//fprintf(stdout, "%d\n",buf[n]);
					cclock[m] = buf[n];			
					n++;
					m++;
				}
				n++;
			}
		}
	}
	n++;
	cclock[n] = '\0';
	clock = atoi(cclock);
	clock = clock * 1000;
	n =0, m =0;
	len = strlen("MemTotal");
	fp = fopen("/proc/meminfo","r");
	while(fgets( buf, 64, fp ) != NULL){
		if( strncmp(buf, "MemTotal", len) == 0){
			while( buf[n] != '\0'){
				while( buf[n] >= 48 && buf[n] < 58 ){
					cmem[m] = buf[n];			
					n++;
					m++;
				}
				n++;
			}
		}
	}
	n++;
	cmem[n] = '\0';
	fprintf(stderr,"%s\n",cmem);
	mem = atoi(cmem);
	mem = mem * 1000;
	fprintf(stderr, "%ld\n", mem);
	close(fp);
}		
	mem = atoi(cmem);
	mem = mem*1000;*/
#endif /* Def: KONOHA_ON_LINUX */		
	knh_snprintf(path, 512, UPDATE_PATH,
				 K_DIST, K_VERSION, K_PLATFORM,
				 CPU_NAME, K_REVISION, clock, mem);
	fd = getStreamHTTP(path);
	
	if (fd != -1) {
		char buf[BUF_LEN];
		read(fd, buf ,BUF_LEN);
		//write(1, buf, BUF_LEN);
		char *version_str = NULL;
		//fprintf(stderr,">>>");
		if((version_str = strstr(buf, "\r\n\r\nMESSAGE:")) != 0 ){
			version_str += sizeof("\r\n\r\nMESSAGE");
			version_str[128] = '\0';
			CHANGE_COLOR(RED);
			fprintf(stderr, "%s",version_str);
			CHANGE_COLOR(WHITE);
		}
		fprintf(stderr,">>> ");
		close(fd);
		return;
	}
}
	
/*--------------------------------------------------------------*/
	
void knh_check_update(Ctx *ctx)
{
	FILE *fp;
	knh_thread_t th;
	if ((fp = fopen(".user.txt", "r")) != NULL) {
		char c;
		c = getc(fp);
		if(c == 'N') {
			return;
		}else if(c == 'y') {
			knh_thread_create(ctx, &th, NULL, knh_check_exec, (void*)ctx);
			return;
		}
	}
	fclose(fp);
	knh_check_makefile(ctx);
	knh_thread_detach(ctx, th);
	return ;
	
}

/*--------------------------------------------------------------*/

void knh_check_makefile(Ctx *ctx)
{
	FILE *fp;
	char buf[16];
	int len = 0;
	while(1){
		fprintf(stdout,
				"IMPORTANT: For improving Konoha experience and delivering security updates,\n"
				"Konoha development team is collecting the following information:\n"
				"\tversion: version=%s distribution=%s revision=%d\n"
				"\tsystem: %s %dbits LANG=%s\n"
				"DO YOU ALLOW? [y/N]: ", K_VERSION, K_DIST, (int)K_REVISION, K_PLATFORM, (int)(sizeof(void*) * 8), knh_getenv("LANG"));
		fgets( buf, 16, stdin );
		len = strlen(buf);
		if((buf[1] == '\n' || buf[1] == '\r' || buf[1] == 0)  &&  (buf[0] == 'y' || buf[0] == 'Y' || buf[0] == 'n' || buf[0] =='N') ) {
			if( buf[0] == 'y' ){
				knh_thread_t th;
				knh_thread_create(ctx, &th, NULL, knh_check_exec, (void*)ctx);
				knh_thread_detach(ctx, th);
			}
			fp = fopen(".user.txt", "w");
			fprintf(fp, "%s", buf);
			fclose(fp);
			fprintf(stdout, "Thank you for using Konoha!!\n");
			break;
		}
	}
	return;
}
/*--------------------------------------------------------------*/
int getStreamHTTP(char *path)
{
	FILE *fp;
	int s;
	struct hostent *servhost;
	struct sockaddr_in server;
	struct servent *service;
	
	char send_buf[BUF_LEN];
	char host[BUF_LEN] = UPDATE_HOST;
	unsigned short port = 80;
	
	servhost = gethostbyname(host);
	if ( servhost == NULL ){
		fprintf(stderr, "[%s] Failed to convert Name to IPAdress \n", host);
		return 0;
	}
	
	memset(&server, 0, sizeof(server));
	
	server.sin_family = AF_INET;
	
	memcpy(&server.sin_addr, servhost->h_addr,  servhost->h_length);
	server.sin_port = htons(port);
	
	if ( ( s = socket(AF_INET, SOCK_STREAM, 0) ) < 0 ){
		//fprintf(stderr, "Failed to make Socket \n");
		return 1;
	}
	
	if ( connect(s, (struct sockaddr *)&server, sizeof(server)) == -1 ){
		//fprintf(stderr, "Failed to connect \n");
		return 1;
	}
	
	snprintf(send_buf, 256, "GET %s HTTP/1.0\r\n", path);
	write(s, send_buf, strlen(send_buf));
	snprintf(send_buf, 256, "Host: %s:%d\r\n\r\n", host, port);
	write(s, send_buf, strlen(send_buf));
	
	return s;
	
}

/*--------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif





















