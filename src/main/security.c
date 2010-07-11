/****************************************************************************
 * KONOHA COPYRIGHT, LICENSE NOTICE, AND DISCRIMER
 *
 * Copyright (c) 2006-2010, Kimio Kuramitsu <kimio at ynu.ac.jp>
 *           (c) 2008-      Konoha Team konohaken@googlegroups.com
 * All rights reserved.
 *
 * You may choose one of the following two licenses when you use konoha.
 * See www.konohaware.org/license.html for further information.
 *
 * (1) GNU Lesser General Public License 3.0 (with K_UNDER_LGPL)
 * (2) Konoha Software Foundation License 1.0
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/* ************************************************************************ */

#include"commons.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/param.h> //
#include <sys/uio.h>
#include <sys/sysctl.h>
#include <unistd.h>

#ifdef KONOHA_ON_MACOSX
#include <sys/sysctl.h>
#endif /* Def: KONOHA_ON_MACOSX */

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

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ======================================================================== */
/* [security alert] */

void knh_checkSecurityAlert()
{
	int fd;
#ifdef KONOHA_ON_MACOSX
	int sels[2] = { CTL_HW, HW_PHYSMEM };
	long mem = 0;
	size_t length = sizeof(mem);
	sysctl(sels, 2, &mem, &length, NULL, 0);
	//fprintf(stderr, "mem %ld\n", mem);
	int sels2[2] = { CTL_HW, HW_CPU_FREQ };
	long clock;
	size_t len = sizeof(clock);
	sysctl(sels2, 2, &clock, &length, NULL, 0);
	//fprintf(stderr, "clock %ld\n", clock);
#endif
#if defined( KONOHA_ON_LINUX ) || defined( KONOHA_ON_WINDOWS )
	FILE *fp;
	int n = 0, m = 0 ;
	long clock;
	long mem;
	char buf[64];
	char cclock[64];
	char cmem[64];
	
	size_t len = strlen("cpu MHz");
	fp = fopen("/proc/cpuinfo","r");
	while(fgets( buf, 64, fp ) != NULL){
	  if( strncmp(buf, "cpu MHz", len) == 0){
		//fprintf(stderr,buf);
	 	while( buf[n] != '\0'){
		  //		  fprintf(stderr, "%d,%c\n", buf[n], buf[n]);
		  
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
		//fprintf(stderr,buf);
	 	while( buf[n] != '\0'){
		  //		  fprintf(stderr, "%d,%c\n", buf[n], buf[n]);
		  
		  while( buf[n] >= 48 && buf[n] < 58 ){
			//fprintf(stdout, "%d\n",buf[n]);
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
	mem = atoi(cmem);
	mem = mem * 1000;
	close(fp);
#endif /* Def: KONOHA_ON_LINUX */
	char path[BUF_LEN] = {0};
	knh_snprintf(path, 512, UPDATE_PATH,
				 K_DIST, K_VERSION, K_PLATFORM,
				 CPU_NAME, K_REVISION, clock, mem);
	//fprintf(stderr, "\n\n%s\n\n", path);
	struct hostent *servhost;
	struct sockaddr_in server;

	char send_buf[BUF_LEN];
	char host[BUF_LEN] = UPDATE_HOST;
	unsigned short port = 80;
	
	servhost = gethostbyname(host);
	if ( servhost == NULL ){
		//fprintf(stderr, "[%s] Failed to convert Name to IPAdress \n", host);
		return ;
	}
	
	memset(&server, 0, sizeof(server));
	
	server.sin_family = AF_INET;
	
	memcpy(&server.sin_addr, servhost->h_addr,  servhost->h_length);
	server.sin_port = htons(port);
	
	if ( ( fd = socket(AF_INET, SOCK_STREAM, 0) ) < 0 ){
		//fprintf(stderr, "Failed to make Socket \n");
		return ;
	}
	
	if ( connect(fd, (struct sockaddr *)&server, sizeof(server)) == -1 ){
		//fprintf(stderr, "Failed to connect \n");
		return ;
	}
	
	snprintf(send_buf, 256, "GET %s HTTP/1.0\r\n", path);
	write(fd, send_buf, strlen(send_buf));
	snprintf(send_buf, 256, "Host: %s:%d\r\n\r\n", host, port);
	write(fd, send_buf, strlen(send_buf));
	if (fd != -1) {
		char buf[BUF_LEN];
		read(fd, buf ,BUF_LEN);
		//write(1, buf, BUF_LEN);
		char *version_str;
		if((version_str = strstr(buf, "\r\n\r\nMESSAGE:")) != 0 ){
		version_str += sizeof("\r\n\r\nMESSAGE");
		int n = 0;
		while( version_str[n] != '\n'){
			n++;
		}
		n++;
		version_str[n] = '\0';
		CHANGE_COLOR(RED);
		fprintf(stderr, "%s",version_str);
		CHANGE_COLOR(WHITE);
		}
		close(fd);
		return;
	}
}


/* ======================================================================== */
/* [password] */

static int secureMode = 0;

/* ------------------------------------------------------------------------ */

void knh_setSecureMode(void)
{
	secureMode = 1;
}

/* ------------------------------------------------------------------------ */

knh_bool_t knh_isTrustedPath(Ctx *ctx, knh_bytes_t path)
{
//	DBG_P("check: %s", (char*)path.buf);
//	if(knh_bytes_startsWith(path, STEXT("http:"))) {
//		return 0;
//	}
	return (secureMode != 1);
}

/* ======================================================================== */
/* [password] */

KNHAPI(char*) knh_getPassword(Ctx *ctx, knh_bytes_t url)
{
	return "password";
}

/* ======================================================================== */
/* [Trusted] */

KNHAPI(void) knh_stack_checkSecurityManager(Ctx *ctx, knh_sfp_t *sfp)
{
//	/* VERY SLOW */
//	knh_sfp_t *sp = sfp - 2;
//	while(ctx->stack < sp) {
//		if(IS_Method(sp[0].mtd)) {
//			if(!URI_ISTRUSTED(DP(sp[0].mtd)->uri)) {
//				char buf[FILEPATH_BUFSIZ];
//				knh_snprintf(buf, sizeof(buf), "Security!!: untrusted domain='%s'", URIDN(DP(sp[0].mtd)->uri));
//				KNH_THROW__T(ctx, buf);
//			}
//		}
//		sp--;
//	}
}

/* ------------------------------------------------------------------------ */

knh_bool_t knh_isTrustedHost(Ctx *ctx, knh_bytes_t host)
{
	TODO();
//	KNH_LOCK(ctx, LOCK_SYSTBL, NULL);
//	int res = knh_DictMap_index__b(DP(ctx->sys)->trustedHostDictMap, host);
//	if(res != -1) return 1;
//	KNH_UNLOCK(ctx, LOCK_SYSTBL, NULL);
	return 0;
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
