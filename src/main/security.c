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

#ifdef KONOHA_ON_WINDOWS
#include <winsock2.h>
#define _WIN32_DCOM
#include <comdef.h>
#include <Wbemidl.h>
#include "atlstr.h"
#pragma comment(lib, "wbemuuid.lib")
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/param.h>
#include <sys/uio.h>
#include <sys/sysctl.h>
#include <unistd.h>
#endif
#ifdef KONOHA_ON_MACOSX
#include <sys/sysctl.h>
#endif
#ifdef KONOHA_ON_LINUX
#include <sys/sysinfo.h>
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

#define UPDATE_HOST "localhost"//"192.168.59.5"
#define UPDATE_PATH "/cgi-bin/security-alert/update_server?dist=%s&ver=%s&arch=%s(%s)&rev=%d&clock=%d&mem=%d&ncpu=%d"
#define PORT 80
#define BUF_LEN 512

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ======================================================================== */
/* [security alert] */

static void getmesseage(int fd, char* path);
static void serverconnect(char *path)
{
	struct hostent *servhost;
	struct sockaddr_in server = {0};
	int fd;
	
#ifdef KONOHA_ON_WINDOWS
	WSADATA data;
	WSAStartup(MAKEWORD(2,0), &data);
#endif

	servhost = gethostbyname(UPDATE_HOST);
	if (servhost == NULL) {
		DBG_P("[%s] Failed to convert Name to IPAdress \n", UPDATE_HOST);
		return;
	}
	
	server.sin_family = AF_INET;
	memcpy(&server.sin_addr, servhost->h_addr,  servhost->h_length);
	server.sin_port = htons(PORT);
	
	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		TODO(/* error message */);
		return;
	}
	if (connect(fd, (struct sockaddr *)&server, sizeof(server)) == -1) {
		TODO(/* error message */);
		return;
	}
	getmesseage(fd, path);

#ifdef KONOHA_ON_WINDOWS
		closesocket(fd);
		WSACleanup();
#else
		close(fd);
#endif
}

/* ------------------------------------------------------------------------ */
#define CHANGE_COLOR(out, color) fprintf(out, "\x1b[%dm", color)

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

static void getmesseage(int fd, char* path)
{
	char send_buf[BUF_LEN] = {'\0'};
	knh_snprintf(send_buf, BUF_LEN, "GET %s HTTP/1.0\r\n", path);
	send(fd, send_buf, strlen(send_buf), 0);
	knh_snprintf(send_buf, BUF_LEN, "Host: %s:%d\r\n\r\n", UPDATE_HOST, PORT);
	send(fd, send_buf, strlen(send_buf), 0);

	if (fd != -1) {
		char buf[BUF_LEN] = {'\0'};
		recv(fd, buf, BUF_LEN, 0);
		char *version_str;
		if((version_str = strstr(buf, "\r\n\r\nMESSAGE:")) != 0 ){
			version_str += strlen("\r\n\r\nMESSAGE:");
			CHANGE_COLOR(stderr, RED);
			fprintf(stderr, "%s", version_str);
			CHANGE_COLOR(stderr, WHITE);
		}
	}
}

/* ------------------------------------------------------------------------ */

static int getncpu(void)
{
	int ncpu;
#ifdef KONOHA_ON_MACOSX
	int mib[2] = { CTL_HW, HW_NCPU };
	size_t len = sizeof(int);
	if (sysctl(mib, 2, &ncpu, &len, NULL, 0) == -1) {
		/* Error */ ncpu = 1;
	}
#elif defined(KONOHA_ON_WINDOWS)
	CAtlString strMessage;
	SYSTEM_INFO sysInfo;
	GetNativeSystemInfo(&sysInfo);
	ncpu = sysInfo.dwNumberOfProcessors;
#elif defined(KONOHA_ON_LINUX)
	ncpu = sysconf(_SC_NPROCESSORS_ONLN);
#else
	TODO();
#endif
	return ncpu;
}

/* ------------------------------------------------------------------------ */

#define ONE_MB (1024 * 1024)
static int getmem(void)
{
	int mem = 0;
#ifdef KONOHA_ON_MACOSX
	int mem_sels[2] = { CTL_HW, HW_PHYSMEM };
	size_t length = sizeof(int);
	sysctl(mem_sels, 2, &mem, &length, NULL, 0);
	mem /= ONE_MB;
#elif defined(KONOHA_ON_WINDOWS)
	MEMORYSTATUSEX stat;
	GlobalMemoryStatusEx(&stat);
	mem = (int) stat.ullTotalPhys / ONE_MB;
#elif defined(KONOHA_ON_LINUX)
	mem = (int) getpagesize() * sysconf (_SC_PHYS_PAGES) / ONE_MB;
#else
	TODO();
#endif
	return mem;
}

/* ------------------------------------------------------------------------ */

static int getclock(void)
{
	int clock;
#ifdef KONOHA_ON_MACOSX
	int cpu_sels[2] = { CTL_HW, HW_CPU_FREQ };
	size_t len = sizeof(int);
	sysctl(cpu_sels, 2, &clock, &len, NULL, 0);
	clock = clock / (1000 * 1000);
#elif defined(KONOHA_ON_WINDOWS)
	HRESULT hres;
	hres = CoInitializeEx(0, COINIT_MULTITHREADED);
	hres = CoInitializeSecurity(
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
	
	while (pEnumerator) {
		HRESULT hr = pEnumerator->Next(
			WBEM_INFINITE,
			1,
			&pclsObj,
			&uReturn
			);
		if (uReturn == 0) { break; }
		VARIANT vtProp;
		hr = pclsObj->Get(L"MaxClockSpeed", 0, &vtProp, 0, 0);
		clock = (int)vtProp.bstrVal;
		VariantClear(&vtProp);
		pclsObj->Release();
	}
	pSvc->Release();
	pLoc->Release();
	pEnumerator->Release();
	CoUninitialize();
#elif defined(KONOHA_ON_LINUX)
	char buf[64] = {'\0'}, *data;
	int n = 0, m = 0;
	const char *cpumhz = "cpu MHz";
	size_t len = strlen(cpumhz);
	FILE *fp = fopen("/proc/cpuinfo","r");
	while(fgets( buf, 64, fp ) != NULL){
		if( strncmp(buf, cpumhz, len) == 0){
			data = strstr(buf, cpumhz);
			break;
		}
	}
	clock = atoi(data) / 1000;
	fclose(fp);
#else 
	TODO();
#endif
	return clock;
}

/* ------------------------------------------------------------------------ */

void knh_checkSecurityAlert(void)
{
	char path[BUF_LEN] = {'\0'};
	int clock = getmem();
	int mem   = getclock();
	int ncpu  = getncpu();
	knh_snprintf(path, 512, UPDATE_PATH,
				 K_DIST, K_VERSION, K_PLATFORM,
				 CPU_NAME, K_REVISION, clock, mem, ncpu);
	DBG_P("Path == [%s] \n", path);
	serverconnect(path);
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
