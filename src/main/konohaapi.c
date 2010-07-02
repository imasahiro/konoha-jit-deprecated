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

#define USE_STEXT 1
#define USE_B     1
#define USE_bytes_strcmp    1
#define USE_bytes_parseint  1
#define USE_cwb_open      1
#define USE_cwb_tobytes   1
#define USE_cwb_size      1

#include"commons.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ======================================================================== */
/* [ctxkey] */

#ifdef K_USING_THREAD
static volatile knh_thread_key_t ctxkey;
static volatile Ctx *curctx = NULL;
#else
static volatile Ctx *curctx = NULL;
#endif

/* ------------------------------------------------------------------------ */

void knh_opcode_check(void);

void konoha_init(void)
{
	static int isInit = 0;
	if(isInit==0) {
		knh_opcode_check();
#ifdef K_USING_THREAD
		knh_thread_key_create((knh_thread_key_t*)&ctxkey);
#endif
		knh_srand(0);
	}
	isInit = 1;
}

/* ------------------------------------------------------------------------ */
/* [Context] */

KNHAPI(Ctx*) knh_beginContext(Ctx *ctx, Ctx **bottom)
{
	((knh_Context_t*)ctx)->cstack_bottom = (void**)bottom;
#ifdef K_USING_THREAD
	knh_mutex_init(ctx->ctxlock);
	knh_thread_setspecific(ctxkey, ctx);
	curctx = ctx;
#else
	curctx = ctx;
#endif
	return ctx;
}

/* ------------------------------------------------------------------------ */

KNHAPI(void) knh_endContext(Ctx *ctx)
{
#ifdef K_USING_THREAD
	knh_thread_setspecific(ctxkey, NULL);
	knh_mutex_destroy(ctx->ctxlock);
	curctx = NULL;
#else
	curctx = NULL;
#endif
	((knh_Context_t*)ctx)->cstack_bottom = NULL;
}

/* ------------------------------------------------------------------------ */

KNHAPI(Ctx*) knh_getCurrentContext(void)
{
#ifdef K_USING_THREAD
	Ctx *ctx = (Ctx*)knh_thread_getspecific(ctxkey);
	if(ctx == NULL) {
		ctx = (Ctx*)curctx;
	}
#else
	Ctx *ctx = (Ctx*)curctx;
#endif
	if(ctx == NULL) {
		fprintf(stderr, "NOT IN THE CONTEXT OF KONOHA\n");
		exit(1);
	}
	return ctx;
}

/* ======================================================================== */
/* [option] */

#if defined(K_USING_DEBUG)
static int systemVerbose = 1;
#else
static int systemVerbose = 0;
#endif

knh_bool_t knh_isSystemVerbose(void)
{
	return systemVerbose;
}

static void knh_setVerboseMode(Ctx *ctx, int mode, const char *optstr)
{
	if(mode == 0) {
		systemVerbose = 1;
		KNH_SYSLOG(ctx, LOG_DEBUG, "Memory", "*sizeof(knh_intptr_t)=%d, sizeof(void*)=%d", sizeof(knh_intptr_t), sizeof(void*));
		KNH_ASSERT(sizeof(knh_intptr_t) == sizeof(void*));
		KNH_SYSLOG(ctx, LOG_DEBUG, "Memory", "*sizeof(knh_int_t)=%d, sizeof(knh_float_t)=%d", sizeof(knh_int_t), sizeof(knh_float_t));
		KNH_ASSERT(sizeof(knh_int_t) <= sizeof(knh_float_t));
		KNH_SYSLOG(ctx, LOG_DEBUG, "Memory", "*sizeof(knh_sfp_t)=%d, sizeof(Ctx)=%d", sizeof(knh_sfp_t), sizeof(knh_Context_t));
		KNH_SYSLOG(ctx, LOG_DEBUG, "Memory", "*sizeof(Object)=%d FASTMALLOC=%d", sizeof(knh_Object_t), K_FASTMALLOC_SIZE);
		KNH_SYSLOG(ctx, LOG_DEBUG, "Memory", "*sizeof(knh_opline_t)=%d FASTMALLOC=%d", sizeof(knh_opline_t), K_FASTMALLOC_SIZE);
	}
	else {
		knh_Context_setVerbose(ctx, 1);
	}
}

/* ----------------------------------------------------------------------- */

static knh_bool_t toInteractiveMode = 0;

knh_bool_t knh_isInteractiveMode(void)
{
	return toInteractiveMode;
}

static void knh_setInteractiveMode(Ctx *ctx, int mode, const char *optstr)
{
	toInteractiveMode = 1;
}

static void knh_setCompilingMode(Ctx *ctx, int mode, const char *optstr)
{
	knh_Context_setCompiling(ctx, 1);
}

static void knh_setDebugMode(Ctx *ctx, int mode, const char *optstr)
{
	knh_Context_setDebug(ctx, 1);
}

static void knh_setOptimizationMode(Ctx *ctx, int mode, const char *optstr)
{
	DBG_P("OPTIMIZATION %d", mode);
	if(mode == 0) {
		DP(ctx->gma)->cflag = 0; // NOTHING
	}
}

static void knh_setTestFile(Ctx *ctx, int mode, const char *optstr)
{
	DBG_P("TESTMODE: %s", optstr);
	KNH_TODO("test");
}

static void knh_setSyncURL(Ctx *ctx, int mode, const char *optstr)
{
	DBG_P("SYNC URL: %s", optstr);
	KNH_TODO("sync");
}

void knh_loadPackageList(Ctx *ctx, const char *pkglist)
{
	if(pkglist != NULL) {
		knh_bytes_t t = {{(knh_uchar_t*)pkglist}, knh_strlen(pkglist)+1};
		char buf[256];
		size_t i = 0;
		int isExists = 0;
		L_NEXT:;
		isExists = 0;
		while(i < t.len) {
			char *c = buf + 8;
			buf[0] = 'p'; buf[1] = 'a'; buf[2] = 'c'; buf[3] = 'k';
			buf[4] = 'a'; buf[5] = 'g'; buf[6] = 'e'; buf[7] = ':';
			while(i < t.len) {
				int ch = t.buf[i];
				i++;
				if(ch ==':' || ch == ';' || ch == ',' || ch == 0) {
					*c = 0;
					DBG_P("loading '%s'", buf);
					if(!knh_loadPackage(ctx, B(buf)) && isExists == 0) {
						KNH_SYSLOG(ctx, LOG_WARNING, "PackageNotFound", "package=%s", buf+8);
					}
					goto L_NEXT;
				}
				else {
					if(ch == '?') {
						isExists = 1; continue;
					}
					*c = ch;
				}
				c++;
				if(!(c - buf < 256)) {
					KNH_SYSLOG(ctx, LOG_WARNING, "TooLongPackage", "%s", pkglist);
					return ;
				}
			}
		}
	}
}

static void knh_setStartUpPackage(Ctx *ctx, int mode, const char *optstr)
{
	if(optstr != NULL) {
		DBG_P("package loading='%s'", optstr);
		knh_loadPackageList(ctx, optstr);
	}
}

/* ----------------------------------------------------------------------- */

typedef void (*knh_Fsetopt)(Ctx *, int, const char *);

#define OPT_EMPTY    0
#define OPT_NUMBER   1
#define OPT_STRING   2

typedef struct {
	const char *name;
	int type;
	knh_Fsetopt setopt;
} knh_optdata_t ;

static knh_optdata_t optdata[] = {
	{"-c", OPT_EMPTY, knh_setCompilingMode},
	{"-i", OPT_EMPTY, knh_setInteractiveMode},
	{"--version", OPT_EMPTY, knh_setVerboseMode},
	{"-g", OPT_NUMBER, knh_setDebugMode},
	{"-v", OPT_NUMBER, knh_setVerboseMode},
	{"-O", OPT_NUMBER, knh_setOptimizationMode},
	{"-P", OPT_STRING, knh_setStartUpPackage},
	{"-u", OPT_STRING, knh_setSyncURL},
	{"-t", OPT_STRING, knh_setTestFile},
	{NULL, OPT_EMPTY, NULL}, // END
};

static knh_optdata_t *knh_getoptdata(const char *name)
{
	knh_optdata_t *d = optdata;
	while(d->name != NULL) {
		size_t len = knh_strlen(d->name);
		if(knh_strncmp(d->name, name, len) == 0) {
			return d;
		}
		d++;
	}
	return NULL;
}

/* ----------------------------------------------------------------------- */

KNHAPI(int) konoha_parseopt(konoha_t konoha, int argc, char **argv)
{
	KONOHA_CHECK(konoha, 1);
	Ctx *ctx = konoha.ctx;
	int n;
	for(n = 1; n < argc; n++) {
		const char *t = argv[n];
		if(t[0] != '-') {
			goto L_SET_KONOHA_ARGV;
		}
		else {
			knh_optdata_t *d = knh_getoptdata(t);
			int optnum = 1;     // default
			const char* optstr = NULL;  // default
			if(d == NULL) {
				KNH_SYSLOG(ctx, LOG_WARNING, "UnknownCommandLineOption", "option='%s'", t);
				continue;
			}
			if(d->type == OPT_NUMBER) {
				t += knh_strlen(d->name);
				if(t[0] == '=') t++;
				if(isalnum(t[0])) {
					knh_int_t v = 0;
					knh_bytes_parseint(B((char*)t), &v);
					optnum = (int)v;
				}
			}
			else if(d->type == OPT_STRING) {
				t += knh_strlen(d->name);
				if(t[0] == '=') {
					optstr = t + 1;
				}
				else if(t[0] != 0) {
					optstr = t;
				}
				else if(n + 1 < argc) {
					n++;
					optstr = argv[n];
					if(optstr[0] == '-') {
						n--; optstr = NULL;
					}
				}
			}
			d->setopt(ctx, optnum, optstr);
		}
	}
	L_SET_KONOHA_ARGV:;
#if defined(K_USING_DEBUG)
	knh_Context_setVerbose(ctx, 1);
#endif
	argc = argc - n;
	argv = argv + n;
	{
		int i;
		knh_Array_t *a = new_Array(ctx, CLASS_String, argc);
		for(i = 1; i < argc; i++) {
			knh_Array_add(ctx, a, new_T(argv[i]));
		}
		knh_DictMap_set(ctx, DP(ctx->sys)->props, new_T("script.argv"), a);
		if(argc > 0) {
			knh_String_t *s = new_T(argv[0]);
			knh_DictMap_set(ctx, DP(ctx->sys)->props, new_T("script.name"), s);
			KNH_SETv(ctx, DP(ctx->share->mainns)->nsname, s);
		}
	}
	return n;
}

/* ------------------------------------------------------------------------ */

KNHAPI(int) konoha_runMain(konoha_t konoha, int argc, char **argv)
{
	KONOHA_CHECK(konoha, -1);
	Ctx *ctx = KONOHA_BEGIN(konoha.ctx);
	knh_Method_t *mtd = knh_getMethodNULL(ctx, knh_Object_cid(ctx->script), MN_main);
	int res = 0;
	if(mtd != NULL) {
		BEGIN_LOCAL(ctx, lsfp, 5);
		klr_setcallmtd(ctx,lsfp[2], mtd);
		KNH_SETv(ctx, lsfp[3].o, ctx->script);
		KNH_SETv(ctx, lsfp[4].o, knh_getPropertyNULL(ctx, STEXT("script.argv")));
		knh_VirtualMachine_run(ctx, lsfp + 3, CODE_LAUNCH);
		res = (int)lsfp[2].ivalue;
		END_LOCAL(ctx, lsfp);
	}
	KONOHA_END(ctx);
	return res;
}

/* ======================================================================== */
/* [shell] */

static void knh_showWelcome(Ctx *ctx, knh_OutputStream_t *w)
{
	knh_printf(ctx, w, "Konoha %s(%s) %s (rev:%d, %s %s)\n",
		K_VERSION, K_CODENAME, K_DIST, ((knh_intptr_t)K_REVISION), __DATE__, __TIME__);
	knh_printf(ctx, w, "[%s] on %s (%d, %s)\n", K_CC, K_PLATFORM, (knh_intptr_t)(sizeof(void*) * 8), knh_getSystemEncoding());
	knh_write_char(ctx, w, "Options:");

#ifdef KNH_FASTMODE
	knh_write_char(ctx, w, " fastmode");
#endif
#ifdef K_USING_INT32
	knh_write_char(ctx, w, " int32");
#endif
#ifdef K_USING_RCGC
	knh_write_char(ctx, w, " rcgc");
#endif
#ifdef K_USING_THREAD
	knh_write_char(ctx, w, " thread");
#endif
	knh_printf(ctx, w, " used_memory:%d kb\n", (knh_intptr_t)(ctx->stat->usedMemorySize / 1024));
#ifdef K_PREVIEW
	knh_printf(ctx, w, "SECURITY ALERT: ** FOR EVALUATION USE ONLY IN DEVELOPMENT **\n\n");
#endif
}


static int shell_checkstmt(knh_bytes_t t)
{
	size_t i = 0;
	int ch, quote = 0, nest = 0;
	L_NORMAL:
	for(; i < t.len; i++) {
		ch = t.buf[i];
		if(ch == '{' || ch == '[' || ch == '(') nest++;
		if(ch == '}' || ch == ']' || ch == ')') nest--;
		if(ch == '\'' || ch == '"' || ch == '`' || ch == '/') {
			quote = ch; i++;
			goto L_QUOTE;
		}
	}
	return nest;
	L_QUOTE:
	DBG_ASSERT(i > 0);
	for(; i < t.len; i++) {
		ch = t.buf[i];
		if(t.buf[i-1] != '\\' && ch == quote) {
			i++;
			goto L_NORMAL;
		}
	}
	return 1;
}

static knh_bool_t shell_command(Ctx *ctx, knh_cwb_t *cwb)
{
	knh_bytes_t t = knh_cwb_tobytes(cwb);
	if(B_equals(t, "quit") || B_equals(t, "exit") || B_equals(t, "bye")) {
		knh_cwb_clear(cwb, 0);
		return 0;
	}
	return 1;
}

static const knh_ShellAPI_t shellAPI = {
	shell_checkstmt,
	shell_command,
};

/* ------------------------------------------------------------------------ */
/* standard shell api */

static void* shell_init(Ctx *ctx, char *msg, char *optstr)
{
	fputs(msg, stdout);
	knh_Context_setInteractive(ctx, 1);
	knh_Context_setDebug(ctx, 1);
	return NULL; // nostatus
}

static knh_bool_t shell_readstmt(Ctx *ctx, void *status, knh_cwb_t *cwb, const knh_ShellAPI_t *api)
{
	int line = 1;
	int ch;
	L_TOP:
	fputs((line == 1) ? ">>> " : "... ", stdout);
	while((ch = fgetc(stdin)) != EOF) {
		if(ch == '\r') continue;
		if(ch == 27) {
			/* ^[[A */ fgetc(stdin); fgetc(stdin); continue;
		}
		if(ch == '\n') {
			int check;
			if((check = api->checkstmt(knh_cwb_tobytes(cwb))) == 0) break;
			if(check < 0) {
				fputs("(Cancelled)...\n", stdout);
				knh_cwb_clear(cwb, 0); line = 1;
				goto L_TOP;
			}
			else {
				line++;
			}
		}
		knh_Bytes_putc(ctx, cwb->ba, ch);
	}
	if(ch == EOF) return 0;
	return api->command(ctx, cwb);
}

static void shell_display(Ctx *ctx, void *status, char *msg, const knh_ShellAPI_t *api)
{
	fputs(msg, stdout);
}

static void shell_cleanup(Ctx *ctx, void *status)
{
	//cleanup status
	knh_Context_setInteractive(ctx, 0);
}

static const knh_ShellSPI_t shellSPI = {
	"konoha standard shell",
	shell_init,
	shell_readstmt,
	shell_display,
	shell_cleanup,
};

static void knh_shell(Ctx *ctx, char *optstr, const knh_ShellSPI_t *spi, const knh_ShellAPI_t *api)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	void *shell_status = NULL;
	BEGIN_LOCAL(ctx, lsfp, 2);
	knh_Array_t *results = new_Array0(ctx, 0);
	KNH_SETv(ctx, lsfp[0].o, results);
	if(spi == NULL) spi = &shellSPI;
	if(api == NULL) api = &shellAPI;
	knh_showWelcome(ctx, cwb->w);
	shell_status = spi->shell_init(ctx, knh_cwb_tochar(ctx, cwb), optstr);
	knh_cwb_clear(cwb, 0);
	while(spi->shell_readstmt(ctx, shell_status, cwb, api)) {
		size_t i;
		knh_InputStream_t *in = new_BytesInputStream(ctx, cwb->ba, cwb->pos, BA_size(cwb->ba));
		KNH_SETv(ctx, lsfp[1].o, in);
		DP(in)->uri = URI_EVAL;
		DP(in)->line = 1; // always line1
		knh_eval(ctx, in, TYPE_Any, results);
		knh_cwb_clear(cwb, 0);
		for(i = 0; i < knh_Array_size(results); i++) {
			knh_Object_t *o = results->list[i];
			knh_Method_t *mtdf = knh_lookupFormatter(ctx, knh_Object_cid(o), MN__k);
			knh_write_Object(ctx, cwb->w, lsfp+1, &mtdf, o);
			knh_write_EOL(ctx, cwb->w);
			spi->shell_display(ctx, shell_status, knh_cwb_tochar(ctx, cwb), api);
			knh_cwb_clear(cwb, 0);
		}
		knh_Array_clear(ctx, results, 0);
	}
	spi->shell_cleanup(ctx, shell_status);
	knh_cwb_close(cwb);
	knh_stack_clear(ctx, lsfp);
	END_LOCAL(ctx, lsfp);
}

KNHAPI(void) konoha_shell(konoha_t konoha, char *optstr)
{
	KONOHA_CHECK_(konoha);
	Ctx *ctx = KONOHA_BEGIN(konoha.ctx);
	char *path = knh_getenv("KONOHA_SHELL");
	if(path != NULL) {
		knh_loadPackageList(ctx, path);
	}
	else {
#if defined(K_USING_POSIX)
		knh_loadPackageList(ctx, "lib.readline?");
#endif
	}
	knh_shell(ctx, optstr, ctx->share->shellSPI, NULL);
	KONOHA_END(ctx);
}

/* ------------------------------------------------------------------------ */

//typedef struct {
//	FILE *fp;
//	char *testTitle;
//	char *testBody;
//	char *testResult;
//} ktest_t ;
//
//static void* test_init(Ctx *ctx, char *msg, char *optstr)
//{
//	FILE *fp = knh_fopen(ctx, )
//	ktest_t *statms = (ktest_t)KNH_MALLOC(ctx, sizeof(ktest_t));
//	DBG_P("test=%s", opstr);
//
//}
//
//#define IS_D(L, ch) (L[0] == ch && L[1] == ch && L[3] == ' ')
//#define IS_T(L, ch) (L[0] == ch && L[1] == ch && L[3] == ch && L[4] == ' ')
//
//static char* test_readstmt(Ctx *ctx, void *status)
//{
//	ktest_t *kt = (ktest_t*)status;
//	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
//	char line[LINE_MAX];
//	while(kt_fgets(ctx, kt, line, LINE_MAX) != NULL) {
//		if(IS_D(line, '#'))) {
//			KNH_SETv(ctx, kt->title, new_S(ctx, B(line + 4))); continue;
//		}
//		if(IS_T(line, '>') || IS_T(line, '.')) {
//			knh_Bytes_write(ctx, cwb->ba, B(line+5));
//			continue;
//		}
//		kt_setline(ctx, kt, line);
//		break;
//	}
//
//	return knh_cwb_tochar(ctx, cwb);
//}
//
//static void shell_display(Ctx *ctx, void *status, char *msg)
//{
//	ktest_t *kt = (ktest_t*)status;
//	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
//	char line[LINE_MAX];
//	while(kt_fgets(ctx, kt, line, LINE_MAX) != NULL) {
//		if(IS_D(line, '#') || IS_T(line, '>') || IS_T(line, '.'))) {
//			kt_setline(ctx, kt, line);
//			break;
//		}
//		knh_Bytes_write(ctx, cwb->ba, B(line));
//	}
//	if(strncmp(msg, status, n)) {
//
//	}
//}
//
//static void shell_cleanup(Ctx *ctx, void *status)
//{
//	//cleanup status
//	knh_Context_setInteractive(ctx, 0);
//}
//
//static knh_ShellSPI_t SimpleShell = {
//	"konoha standard shell",
//	shell_init,
//	shell_readstmt,
//	shell_display,
//	shell_cleanup,
//};

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
