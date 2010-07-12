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

#ifdef KONOHA_ON_MACOSX
#include <sys/sysctl.h>
#endif


#ifdef KONOHA_ON_WINDOWS

#include <winsock2.h>
#define UPDATE_PATH "/cgi-bin/security-alert/update_server?dist=%s&ver=%s&arch=%s(%s)&rev=%d&clock=%ld&mem=%lu"
#define _WIN32_DCOM
#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")

#else

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/param.h> //
#include <sys/uio.h>
#include <sys/sysctl.h>
#include <unistd.h>
#define UPDATE_PATH "/cgi-bin/security-alert/update_server?dist=%s&ver=%s&arch=%s(%s)&rev=%d&clock=%ld&mem=%ld"
#endif

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

#define BUF_LEN 512

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ======================================================================== */
/* [security alert] */

static void getpath(char* path)
{
#ifdef KONOHA_ON_MACOSX
	long mem, clock = 0;
	int sels[2] = { CTL_HW, HW_PHYSMEM };
	size_t length = sizeof(mem);
	sysctl(sels, 2, &mem, &length, NULL, 0);
	mem = mem / 1024 / 1024;
	//fprintf(stderr, "mem %ld\n", mem);
	sels[0] = CTL_HW;
	sels[1] = HW_CPU_FREQ;
	size_t len = sizeof(clock);
	sysctl(sels, 2, &clock, &len, NULL, 0);
	clock = clock / 1000 / 1000;
	//fprintf(stderr, "clock %ld\n", clock);

#elif defined(KONOHA_ON_WINDOWS)
	int clock;
	HRESULT hres;
	hres = CoInitializeEx(0, COINIT_MULTITHREADED);
	hres =  CoInitializeSecurity(
        NULL, 
        -1,                          // COM authentication
        NULL,                        // Authentication services
        NULL,                        // Reserved
        RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication 
        RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation  
        NULL,                        // Authentication info
        EOAC_NONE,                   // Additional capabilities 
        NULL                         // Reserved
        );
    IWbemLocator *pLoc = NULL;
    hres = CoCreateInstance(
        CLSID_WbemLocator,             
        0, 
        CLSCTX_INPROC_SERVER, 
        IID_IWbemLocator, (LPVOID *) &pLoc);
    IWbemServices *pSvc = NULL;
    hres = pLoc->ConnectServer(
         _bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
         NULL,                    // User name. NULL = current user
         NULL,                    // User password. NULL = current
         0,                       // Locale. NULL indicates current
         NULL,                    // Security flags.
         0,                       // Authority (e.g. Kerberos)
         0,                       // Context object 
         &pSvc                    // pointer to IWbemServices proxy
         );
     hres = CoSetProxyBlanket(
       pSvc,                        // Indicates the proxy to set
       RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
       RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
       NULL,                        // Server principal name 
       RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
       RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
       NULL,                        // client identity
       EOAC_NONE                    // proxy capabilities 
    );
    IEnumWbemClassObject* pEnumerator = NULL;
    hres = pSvc->ExecQuery(
        bstr_t("WQL"), 
        bstr_t("SELECT * FROM Win32_Processor"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, 
        NULL,
        &pEnumerator);
    IWbemClassObject *pclsObj;
    ULONG uReturn = 0;
	
    while (pEnumerator)
    {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, 
									   &pclsObj, &uReturn);
        if(0 == uReturn) break;
        VARIANT vtProp;
        // Get the value of the Name property
        hr = pclsObj->Get(L"MaxClockSpeed", 0, &vtProp, 0, 0);
        clock = (int)vtProp.bstrVal ;
        VariantClear(&vtProp);
        pclsObj->Release();
    }

    pSvc->Release();
    pLoc->Release();
    pEnumerator->Release();
    CoUninitialize();

	unsigned long mem = 0;
	MEMORYSTATUSEX stat;
	GlobalMemoryStatusEx(&stat);
	mem = (unsigned long)stat.ullTotalPhys;
	mem /= 1024*1024;
#elif defined(KONOHA_ON_LINUX)
	FILE *fp;
	long mem = 0;
	long clock = 0;
	int n = 0, m = 0 ;
	char buf[64] = {'\0'};
	char cclock[64] = {'\0'};
	char cmem[64] = {'\0'};
	
	size_t len = strlen("cpu MHz");
	fp = fopen("/proc/cpuinfo","r");
	while(fgets( buf, 64, fp ) != NULL){
		if( strncmp(buf, "cpu MHz", len) == 0){
			//fprintf(stderr,buf);
			while( buf[n] != '\0'){
				// fprintf(stderr, "%d,%c\n", buf[n], buf[n]);
				while( '0' <= buf[n]  && buf[n] <= '9' ){
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
	clock = atoi(cclock) / 1000;
	n =0, m =0;
	len = strlen("MemTotal");
	fp = fopen("/proc/meminfo","r");
	while(fgets( buf, 64, fp ) != NULL){
		if( strncmp(buf, "MemTotal", len) == 0){
			//fprintf(stderr,buf);
			while( buf[n] != '\0'){
				// fprintf(stderr, "%d,%c\n", buf[n], buf[n]);
				while( '0' <= buf[n]  && buf[n] <= '9' ){
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
	mem = atoi(cmem) / 1024;
	fclose(fp);
#endif	
	knh_snprintf(path, 512, UPDATE_PATH,
				 K_DIST, K_VERSION, K_PLATFORM,
				 CPU_NAME, K_REVISION, clock, mem);

}

/* ------------------------------------------------------------------------ */
void knh_checkSecurityAlert()
{
	int fd;
	char path[BUF_LEN] = { '\0' };
	getpath(path);
	fprintf(stderr, "\n\n%s\n\n", path);
	struct hostent *servhost;
	struct sockaddr_in server;

	char send_buf[BUF_LEN] = { '\0' };
	char host[BUF_LEN] = UPDATE_HOST;
	unsigned short port = 80;
	
#ifdef KONOHA_ON_WINDOWS
	WSADATA data;
	WSAStartup(MAKEWORD(2,0), &data);
#endif /* Def: KONOHA_ON_WINDOWS */

	servhost = gethostbyname(host);
	if ( servhost == NULL ){
		//fprintf(stderr, "[%s] Failed to convert Name to IPAdress \n", host);
		return ;
	}
	
	memset(&server, 0, sizeof(server));
	
	server.sin_family = AF_INET;
	
	memcpy(&server.sin_addr, servhost->h_addr,  servhost->h_length);
	server.sin_port = htons(port);
	
	if (( fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		//fprintf(stderr, "Failed to make Socket \n");
		return ;
	}
	
	if ( connect(fd, (struct sockaddr *)&server, sizeof(server)) == -1 ){
		//fprintf(stderr, "Failed to connect \n");
		return ;
	}
	
	knh_snprintf(send_buf, 256, "GET %s HTTP/1.0\r\n", path);
	send(fd, send_buf, strlen(send_buf), 0);
	knh_snprintf(send_buf, 256, "Host: %s:%d\r\n\r\n", host, port);
	send(fd, send_buf, strlen(send_buf), 0);
	if (fd != -1) {
		char buf[BUF_LEN] =  {'\0'};
		recv(fd, buf, BUF_LEN, 0);
		//write(1, buf, BUF_LEN);
		char *version_str;
		if((version_str = strstr(buf, "\r\n\r\nMESSAGE:")) != 0 ){
			version_str += sizeof("\r\n\r\nMESSAGE:");
			//int n = 0;
			CHANGE_COLOR(RED);
			fprintf(stderr, "%s",version_str);
			CHANGE_COLOR(WHITE);
		}

#ifdef KONOHA_ON_WINDOWS
		closesocket(fd);
		WSACleanup();
#else 
		close(fd);		
#endif

		return;
	}

}

/* ------------------------------------------------------------------------ */

/* ======================================================================== */
/* [password] */

//static int secureMode = 0;

/* ------------------------------------------------------------------------ */

//void knh_setSecureMode(void)
//{
//	secureMode = 1;
//}

/* ------------------------------------------------------------------------ */

//knh_bool_t knh_isTrustedPath(Ctx *ctx, knh_bytes_t path)
//{
//	DBG_P("check: %s", (char*)path.buf);
//	if(knh_bytes_startsWith(path, STEXT("http:"))) {
//		return 0;
//	}
//	return (secureMode != 1);
//}

/* ======================================================================== */
/* [password] */

//const char* knh_getPassword(Ctx *ctx, knh_bytes_t url)
//{
//	return "password";
//}

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
//				knh_snprintf(buf, sizeof(buf), "Security!!: untrusted domain='%s'", URI__(DP(sp[0].mtd)->uri));
//				KNH_THROW__T(ctx, buf);
//			}
//		}
//		sp--;
//	}
}

/* ------------------------------------------------------------------------ */

//knh_bool_t knh_isTrustedHost(Ctx *ctx, knh_bytes_t host)
//{
//	TODO();
//	OLD_LOCK(ctx, LOCK_SYSTBL, NULL);
//	int res = knh_DictMap_index__b(DP(ctx->sys)->trustedHostDictMap, host);
//	if(res != -1) return 1;
//	OLD_UNLOCK(ctx, LOCK_SYSTBL, NULL);
//	return 0;
//}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
