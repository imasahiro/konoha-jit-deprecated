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


/* ************************************************************************ */
// added by nakata
#define USE_cwb_write 1
#define K_PERROR_LIBNAME "stdc"

/* ************************************************************************ */

#include"commons.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif


/* ======================================================================== */
/* [ctxkey] */

#ifdef K_USING_THREAD
#ifdef K_CC_TLS
static K_CC_TLS  Ctx *curctx = NULL;
#else
static volatile knh_thread_key_t ctxkey;
static volatile Ctx *curctx = NULL;
#endif
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
#if defined(K_USING_THREAD) && !defined(K_CC_TLS)
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
	knh_mutex_lock(ctx->ctxlock);  // by yoan
#if !defined(K_CC_TLS)
	knh_thread_setspecific(ctxkey, ctx);
#endif
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
#if !defined(K_CC_TLS)
	knh_thread_setspecific(ctxkey, NULL);
#endif
	knh_mutex_unlock(ctx->ctxlock);  // by yoan
	curctx = NULL;
#else
	curctx = NULL;
#endif
	((knh_Context_t*)ctx)->cstack_bottom = NULL;
}

/* ------------------------------------------------------------------------ */

KNHAPI(Ctx*) knh_getCurrentContext(void)
{
#if defined(K_USING_THREAD) && !defined(K_CC_TLS)
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

KNHAPI(knh_bool_t) knh_isInteractiveMode(void)
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

/* ************************************************************************ */

static knh_bool_t toTestMode = 0;

KNHAPI(knh_bool_t) knh_isTestMode(void)
{
	return toTestMode;
}

static void knh_setTestMode(Ctx *ctx, int mode, const char *optstr)
{
	toTestMode = 1;
}

/* ************************************************************************ */

static void knh_setSyncURL(Ctx *ctx, int mode, const char *optstr)
{
	DBG_P("SYNC URL: %s", optstr);
	KNH_TODO("sync");
}

void knh_loadPackageList(Ctx *ctx, const char *pkglist)
{
	if(pkglist != NULL) {
		knh_bytes_t t = {{pkglist}, knh_strlen(pkglist)};
		char buf[256];
		size_t i = 0;
		int isExists = 0;
		L_NEXT:;
		isExists = 0;
		while(i < t.len + 1) {
			char *c = buf + 4;
			buf[0] = 'p'; buf[1] = 'k'; buf[2] = 'g'; buf[3] = ':';
			while(i < t.len + 1) {
				int ch = t.ubuf[i];
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
	{"-t", OPT_EMPTY,  knh_setTestMode},
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

knh_bytes_t knh_bytes_nsname(knh_bytes_t t)
{
	size_t i, s = 0;
	for(i = t.len - 1; i > 0; i--) {
		if(t.ustr[i] == '/' || t.ustr[i] == '\\') {
			s = i + 1;
			break;
		}
	}
	for(i = s; i < t.len; i++) {
		if(t.ustr[i] == '.') {
			t.ustr = t.ustr + s;
			t.len = i - s;
			return t;
		}
	}
	t.ustr = t.ustr + s;
	t.len = t.len - s;
	return t;
}

int konoha_parseopt(konoha_t konoha, int argc, const char **argv)
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
			s = new_S(ctx, knh_bytes_nsname(S_tobytes(s)));
			KNH_SETv(ctx, DP(ctx->share->mainns)->nsname, s);
		}
	}
	return n;
}

/* ------------------------------------------------------------------------ */

int konoha_runMain(konoha_t konoha, int argc, const char **argv)
{
	KONOHA_CHECK(konoha, -1);
	Ctx *ctx = KONOHA_BEGIN(konoha.ctx);
	knh_Method_t *mtd = knh_getMethodNULL(ctx, knh_Object_cid(ctx->script), MN_main);
	int res = 0;
	if(mtd != NULL) {
		int thisidx = 1 + K_CALLDELTA;
		BEGIN_LOCAL(ctx, lsfp, 5);
		lsfp[1].ivalue = 0;
		lsfp[thisidx+K_PCIDX].pc = NULL;
		klr_setcallmtd(ctx,lsfp[thisidx+K_MTDIDX], mtd);
		KNH_SETv(ctx, lsfp[thisidx].o, ctx->script);
		KNH_SETv(ctx, lsfp[thisidx+1].o, knh_getPropertyNULL(ctx, STEXT("script.argv")));
		klr_setesp(ctx, lsfp + thisidx+2);
		knh_VirtualMachine_run(ctx, lsfp + thisidx, CODE_LAUNCH);
		res = (int)lsfp[1].ivalue;
		END_LOCAL(ctx, lsfp);
	}
	KONOHA_END(ctx);
	return res;
}

/* ------------------------------------------------------------------------ */
/* [shell] */

static void knh_showWelcome(Ctx *ctx, knh_OutputStream_t *w)
{
	knh_printf(ctx, w, "Konoha %s(%s) %s (rev:%d, %s %s)\n",
		K_VERSION, K_CODENAME, K_DIST, ((knh_intptr_t)K_REVISION), __DATE__, __TIME__);
	knh_printf(ctx, w, "[%s] on %s (%d, %s)\n", K_CC, K_PLATFORM, (knh_intptr_t)(sizeof(void*) * 8), knh_getSystemEncoding());
	knh_write_text(ctx, w, "Options:");

#ifdef KNH_FASTMODE
	knh_write_text(ctx, w, " fastmode");
#endif
#ifdef K_USING_INT32
	knh_write_text(ctx, w, " int32");
#endif
#ifdef K_USING_RCGC
	knh_write_text(ctx, w, " rcgc");
#endif
#ifdef K_USING_THREAD
	knh_write_text(ctx, w, " thread");
#endif
	knh_printf(ctx, w, " used_memory:%d kb\n", (knh_intptr_t)(ctx->stat->usedMemorySize / 1024));
}

static int shell_checkstmt(knh_bytes_t t)
{
	size_t i = 0;
	int ch, quote = 0, nest = 0;
	L_NORMAL:
	for(; i < t.len; i++) {
		ch = t.ubuf[i];
		if(ch == '{' || ch == '[' || ch == '(') nest++;
		if(ch == '}' || ch == ']' || ch == ')') nest--;
		if(ch == '\'' || ch == '"' || ch == '`') {
			quote = ch; i++;
			goto L_QUOTE;
		}
	}
	return nest;
	L_QUOTE:
	DBG_ASSERT(i > 0);
	for(; i < t.len; i++) {
		ch = t.ubuf[i];
		if(t.ubuf[i-1] != '\\' && ch == quote) {
			i++;
			goto L_NORMAL;
		}
	}
	return 1;
}

static void shell_restart(Ctx *ctx)
{
	knh_NameSpace_t *ns = new_NameSpace(ctx, NULL);
	DBG_ASSERT(ns->b->aliasDictMapNULL == NULL);
	DP(ns)->aliasDictMapNULL = DP(ctx->share->mainns)->aliasDictMapNULL;
	DP(ctx->share->mainns)->aliasDictMapNULL = NULL;
	KNH_SETv(ctx, ((knh_share_t*)ctx->share)->mainns, ns);
	KNH_SETv(ctx, ((knh_Context_t*)ctx)->script, new_(Script));
	{
		knh_Gamma_t *kc = new_(Gamma);
		KNH_SETv(ctx, ((knh_Context_t*)ctx)->gma, kc);
		KNH_INITv(DP(kc)->symbolDictMap, new_DictMap0(ctx, 256));
		KNH_INITv(DP(kc)->constPools, new_Array0(ctx, 0));
		KNH_INITv(DP(kc)->script, ctx->script);
	}
}

static knh_bool_t shell_command(Ctx *ctx, knh_cwb_t *cwb)
{
	knh_bytes_t t = knh_cwb_tobytes(cwb);
	if(B_equals(t, "quit") || B_equals(t, "exit") || B_equals(t, "bye")) {
		knh_cwb_clear(cwb, 0);
		return 0;
	}
	if(B_equals(t, "restart")) {
		knh_cwb_clear(cwb, 0);
		fputs(
"=============================================================================="
"\n\n\n", stdout);
		shell_restart(ctx);
	}
	return 1;
}

static const knh_ShellAPI_t shellAPI = {
	shell_checkstmt,
	shell_command,
};

/* ------------------------------------------------------------------------ */
/* standard shell api */

static void* shell_init(Ctx *ctx, const char *msg, const char *optstr)
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
				goto L_TOP;
			}
		}
		knh_Bytes_putc(ctx, cwb->ba, ch);
	}
	if(ch == EOF) return 0;
	return api->command(ctx, cwb);
}

static void shell_display(Ctx *ctx, void *status, const char *msg, const knh_ShellAPI_t *api)
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

#ifdef K_PREVIEW
static const char* SecurityAlertMessage =  "** FOR EVALUATION/DEVELOPMENT USE ONLY **";
#else
static const char* SecurityAlertMessage = NULL;
#endif

void knh_setSecurityAlertMessage(const char *msg, int isNeedFree)
{
	static int isSecurityAlertNeedFree = 0;
	if(SecurityAlertMessage != NULL && isSecurityAlertNeedFree) {
		free((void*)SecurityAlertMessage);
	}
	SecurityAlertMessage = msg;
	isSecurityAlertNeedFree = isNeedFree;
}

static void knh_showSecurityAlert(Ctx *ctx, knh_OutputStream_t *w)
{
	if(SecurityAlertMessage != NULL /*&& knh_Context_isInteractive(ctx)*/) {
		knh_write_text(ctx, w, "SECURITY ALERT: ");
		knh_write_text(ctx, w, SecurityAlertMessage);
		knh_write_EOL(ctx, w); knh_write_EOL(ctx, w);
		knh_setSecurityAlertMessage(NULL, 0);
	}
}

static void knh_shell(Ctx *ctx, const char *filename, const knh_ShellSPI_t *spi, const knh_ShellAPI_t *api)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	void *shell_status = NULL;
	BEGIN_LOCAL(ctx, lsfp, 2);
	LOCAL_NEW(ctx, lsfp, 0, knh_Array_t *, results, new_Array0(ctx, 0));
	LOCAL_NEW(ctx, lsfp, 1, knh_InputStream_t *, bin, new_BytesInputStream(ctx, new_Bytes(ctx, K_PAGESIZE)));
	if(spi == NULL) spi = &shellSPI;
	if(api == NULL) api = &shellAPI;
	knh_showWelcome(ctx, cwb->w);
	knh_showSecurityAlert(ctx, cwb->w);
	shell_status = spi->shell_init(ctx, knh_cwb_tochar(ctx, cwb), filename);
	knh_cwb_clear(cwb, 0);
	while(spi->shell_readstmt(ctx, shell_status, cwb, api)) {
		size_t i;
		knh_Bytes_clear(DP(bin)->ba, 0);
		knh_Bytes_write(ctx, DP(bin)->ba, knh_cwb_tobytes(cwb));
		knh_BytesInputStream_setpos(ctx, bin, 0, BA_size(DP(bin)->ba));
		knh_cwb_clear(cwb, 0);
		DP(bin)->uri = URI_EVAL;
		DP(bin)->line = 1; // always line1
		knh_eval(ctx, bin, TYPE_Any, results);
		knh_OutputStream_flush(ctx, ctx->out);
		if(ctx->out != DP(ctx->sys)->out) {
			knh_Bytes_t *outbuf = DP(ctx->out)->ba;
			knh_write(ctx, cwb->w, outbuf->bu);
			knh_Bytes_clear(outbuf, 0);
		}
		for(i = 0; i < knh_Array_size(results); i++) {
			knh_Object_t *o = results->list[i];
			knh_Method_t *mtdf = knh_getSystemFormatter(ctx, knh_Object_cid(o), MN__k);
			knh_write_Object(ctx, cwb->w, lsfp+1, &mtdf, o);
			knh_write_EOL(ctx, cwb->w);
		}
		knh_showSecurityAlert(ctx, cwb->w);
		if(knh_cwb_size(cwb) !=0) {
			spi->shell_display(ctx, shell_status, knh_cwb_tochar(ctx, cwb), api);
			knh_cwb_clear(cwb, 0);
		}
		knh_Array_clear(ctx, results, 0);
		knh_cwb_clear(cwb, 0);
	}
	spi->shell_cleanup(ctx, shell_status);
	knh_cwb_close(cwb);
	knh_stack_clear(ctx, lsfp);
	END_LOCAL(ctx, lsfp);
}

#ifdef K_USING_SECURITY_ALERT
static void knh_askSecurityAlert(Ctx *ctx)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_Bytes_write(ctx, cwb->ba, S_tobytes(knh_getPropertyNULL(ctx, STEXT("user.path"))));
	knh_Bytes_putc(ctx, cwb->ba, '/');
	knh_Bytes_write(ctx, cwb->ba, STEXT("ALLOWED_SECURITY_ALERT"));
	if(!knh_cwb_isfile(ctx, cwb)) {
		char buf[80];
		fprintf(stdout,
"IMPORTANT: For improving Konoha experience and delivering security updates,\n"
"Konoha development team is collecting the following information:\n"
"\tversion: version=%s distribution=%s revision=%d\n"
"\tsystem: %s %dbits LANG=%s\n"
"DO YOU ALLOW? [y/N]: ",
		K_VERSION, K_DIST, (int)K_REVISION, K_PLATFORM, (int)(sizeof(void*) * 8), knh_getenv("LANG"));
		if(fgets(buf, sizeof(80), stdin) != NULL) {
			if((buf[0] == 'y' || buf[0] == 'Y') && (buf[1] == 0 || buf[1] == '\n' || buf[1] == '\r')) {
				FILE *fp = fopen(knh_cwb_tochar(ctx, cwb), "a");
				if(fp != NULL) {
					fclose(fp);
				}
				fprintf(stdout, "Thank you for using Konoha!!\n");
				goto L_CHECK;
			}
		}
		knh_cwb_close(cwb);
		return;
	}
	L_CHECK:;
	knh_cwb_close(cwb);
//  USE knh_checkSecurityAlert();
//	knh_thread_t th;
//	knh_thread_create(ctx, &th, NULL, knh_checkSecurityAlert, (void*)ctx);
//	knh_thread_detach(ctx, th);
//	knh_checkSecurityAlert();
}
#endif

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
#ifdef K_USING_SECURITY_ALERT
	knh_askSecurityAlert(ctx);
#endif
	knh_shell(ctx, optstr, ctx->share->shellSPI, NULL);
	KONOHA_END(ctx);
}

/* ------------------------------------------------------------------------ */
/* [ktest lang] */
// the following is added by nakata 
// modified by kimio

static volatile knh_bool_t isTestVerbose = 0;


#define KTEST_LINE_MAX 512
#define IS_D(L, ch) (L[0] == ch && L[1] == ch)
#define IS_T(L, ch) (L[0] == ch && L[1] == ch && L[2] == ch && L[3] == ' ')

typedef struct _kt_stmt_t {
	knh_bytes_t testBody;
	knh_bytes_t testResult;
	knh_uint_t  isPassed;   // by kimio isFail => isFailed
	struct _kt_stmt_t *next;
} kt_stmt_t;

typedef struct _kt_unit_t {
	knh_bytes_t testTitle;
	knh_uint_t stmtsize;
	kt_stmt_t *shead;
	kt_stmt_t *current;
	struct _kt_unit_t *next;
} kt_unit_t;

typedef struct _kt_status_t {
	FILE *in;
	FILE *out;
	knh_bytes_t filename;
	size_t unitsize;
	kt_unit_t *uhead;
	kt_unit_t *current;
	size_t     lineno;
	size_t     sumOfPassed;
	size_t     sumOfFailed;
} kt_status_t ;

static char *kt_fgets(Ctx *ctx, kt_status_t *kt, char *line, int max)
{
	char *ret;
	if ((ret = fgets(line, max - 1, kt->in)) != NULL) {
		line[strlen(line)] = '\0';
	}
	return ret;
}

static void kt_truncate_line(char* str)
{
	size_t len = knh_strlen(str);
	str[len - 1] = '\0';
}

static kt_unit_t* new_kt_unit(Ctx *ctx, const char* title)
{
	kt_unit_t* ret = (kt_unit_t*)KNH_MALLOC(ctx, sizeof(kt_unit_t));
	size_t len = knh_strlen(title);
	if (len == 0) title = "(Undefined)";
	ret->testTitle.ubuf = (knh_uchar_t *)KNH_MALLOC(ctx, len + 1);
	knh_memcpy(ret->testTitle.ubuf, title, len + 1);
	ret->testTitle.len = len;
	ret->shead = NULL;
	ret->current = NULL;
	ret->stmtsize = 0;
	ret->next = NULL;
	return ret;
}

static kt_stmt_t* new_kt_stmt(Ctx *ctx, char *body)
{
	kt_stmt_t *ret = (kt_stmt_t *)KNH_MALLOC(ctx, sizeof(kt_stmt_t));
	size_t len = knh_strlen(body);
	if (len > 0) {
		ret->testBody.ubuf = (knh_uchar_t *)KNH_MALLOC(ctx, len + 1);
		knh_memcpy(ret->testBody.ubuf, body, len+1);
	}
	ret->testBody.len = len;
	ret->testResult.ubuf = NULL;
	ret->testResult.len = 0;
	ret->isPassed = 0;
	ret->next = NULL;
	return ret;

}

static void* test_init(Ctx *ctx, const char *msg, const char *filename)
{
	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		KNH_SYSLOG(ctx, LOG_ERR, "TestFile", "cannot open: %s", filename);
		return NULL;
	}
	else {
		size_t len = knh_strlen(filename);
		kt_status_t *kt = (kt_status_t*)KNH_MALLOC(ctx, sizeof(kt_status_t));
		kt->in = fp;
		kt->out = stdout;
		kt->filename.ubuf = (knh_uchar_t *)KNH_MALLOC(ctx, len+1);
		knh_memcpy(kt->filename.ubuf, filename, len + 1);
		kt->filename.len = len;
		kt->current = NULL;
		kt->uhead = NULL;
		kt->unitsize = 0;
		kt->lineno = 0;
		kt->sumOfPassed = 0;
		kt->sumOfFailed = 0;
		return kt;
	}
}

static void add_kt_stmt_body(Ctx *ctx, kt_stmt_t* ks, char *body)
{
	// realloc
	size_t len = knh_strlen(body);
	DBG_ASSERT(len > 0);
	knh_uchar_t *tmp = (knh_uchar_t*)KNH_MALLOC(ctx, ks->testBody.len + len + 1);
	knh_memcpy(tmp, ks->testBody.ubuf, ks->testBody.len);
	knh_memcpy(tmp + ks->testBody.len, body, len);
	tmp[ks->testBody.len + len] = '\0';
	KNH_FREE(ctx, ks->testBody.ubuf, ks->testBody.len + 1);
	ks->testBody.ubuf = tmp;
	ks->testBody.len += len;
}

static void add_kt_stmt_result(Ctx *ctx, kt_stmt_t *ks, const char *result)
{
	size_t len = knh_strlen(result);
 	DBG_ASSERT(len > 0);
	if (ks != NULL) {
		if (ks->testResult.len == 0) {
			ks->testResult.ubuf = (knh_uchar_t *)KNH_MALLOC(ctx, len + 1);
			knh_memcpy(ks->testResult.ubuf, result, len + 1);
			ks->testResult.len = len;
		} else {
			//realloc;
			knh_uchar_t *tmp = (knh_uchar_t *)KNH_MALLOC(ctx, ks->testResult.len + len + 1);
			knh_memcpy(tmp, ks->testResult.ubuf, ks->testResult.len);
			knh_memcpy(tmp + ks->testResult.len, result, len);
			tmp[ks->testResult.len + len] = '\0';
			KNH_FREE(ctx, ks->testResult.ubuf, ks->testResult.len + 1);
			ks->testResult.ubuf = tmp;
			ks->testResult.len += len;
		}
	} else {
		// adding result before ks instance created.
		// invalid syntax.
		KNH_SYSLOG(ctx, LOG_ERR, "TestFile", "please add '>>> ' statement before writing result : '%s", result);
	}
}


static knh_bool_t test_readstmt(Ctx *ctx, void *status, knh_cwb_t *cwb, const knh_ShellAPI_t *api)
{
	kt_status_t *kt = (kt_status_t*)status;
	kt_unit_t *ku = NULL;
	kt_stmt_t *ks = NULL;
	if (kt == NULL) return 0;
	if (kt->unitsize != 0) ku = kt->current;
	if (ku != NULL && ku->stmtsize != 0) ks = ku->current;

	/* if its in middle of unittest, return current */
	if (ks != NULL && ks->next != NULL) {
		ks = ks->next;
		ku->current = ks;
		if(ks->testBody.len == 1 && ks->testBody.text[0] == '\n') {
			/* assured as passed */
			kt->sumOfPassed++;
			ks->isPassed = 1;
			if (ks->next != NULL) {
				ks = ks->next;
				ku->current = ks;
			} else {
				return 1;
			}
		}
		knh_cwb_write(ctx, cwb, ks->testBody);
		return 1;
	}
	char line[KTEST_LINE_MAX] = {0};
	int comment_nest = 0;
	char *search_ptr = NULL;
	char *line_ptr = NULL;
	int isUnitStarted = 0, isStmtContinue = 0;
	int isResultReady = 0;
	while(kt_fgets(ctx, kt, line, KTEST_LINE_MAX) != NULL) {
		kt->lineno += 1;
		line_ptr = line;
		search_ptr = NULL;
		/* ignore comment first */
		if (comment_nest == 0) {
			if ((search_ptr = strstr(line_ptr, "//")) != NULL) {
				/* only a single appearance is enough */
				// ignore after searchptr
				if (search_ptr - line_ptr >= 1) {
					if (search_ptr[-1] == '"' || search_ptr[-1] == '\'') { /* its might be literal */
						// do nothing.
					} else {
						line_ptr[search_ptr - line_ptr] = '\0'; // terminate line before // appear.
					}
				} else {
					line_ptr[search_ptr - line_ptr] = '\0'; // terminate line before // appear.
				}
			}
			// get the first comment opener
			if ((search_ptr = strstr(line_ptr, "/*")) != NULL) {
				comment_nest += 1;
				line_ptr[search_ptr - line_ptr] = '\0';
				search_ptr += 2;
				char *tmp_ptr = search_ptr;
				/* detect all comment opener */
				while ((tmp_ptr = strstr(tmp_ptr, "/*")) != NULL) {
					comment_nest += 1;
					tmp_ptr += 2;
				}
				/* detect the last comment closer */
				while ((search_ptr = strstr(search_ptr, "*/")) != NULL) {
					comment_nest -= 1;
					search_ptr += 2;
				}
			}
		} else {
			// its in comment nest, get last */
			while ((search_ptr = strstr(line_ptr, "*/")) != NULL) {
				comment_nest -= 1;
				line_ptr = search_ptr + 2;
			}
			if (comment_nest > 0) {
				// ignore
				continue;
			} else if (comment_nest == 0) {
				// do nothing
			}
		}
		if (comment_nest < 0) {
			fprintf(stderr, "COMMENT ERROR!\n");
			KNH_SYSLOG(ctx, LOG_ERR, "parsing testcode", "invalid comment nesting");
			return 0;
		}
		/* now, we parse per unit */
		if (IS_T(line_ptr, '#')) {
			if (isUnitStarted) {
				if (ku->stmtsize != 0) {
				// next unit test. we need to rewind fp;
					// TODO: alternate fseek for windows implementation
					long len = (long)knh_strlen(line);
					fseek(kt->in, -(len + 1), SEEK_CUR);
					kt->lineno -= 1;
					break;
				} else {
					// empty unit. ignore this.
				}
			}
			line_ptr += 4;
			size_t title_len = knh_strlen(line_ptr);

			kt_truncate_line(line_ptr);
			if (kt->unitsize == 0) {
				kt->uhead = new_kt_unit(ctx, line_ptr);
				ku = kt->uhead;
			} else {
				ku->next = new_kt_unit(ctx, line_ptr);
				ku = ku->next;
			}
			kt->unitsize++;
			kt->current = ku;
			isUnitStarted = 1;
			isResultReady = 0;
			if (title_len >= 4 && line_ptr[0] == 'T' && line_ptr[1] == 'O' && line_ptr[2] == 'D' && line_ptr[3] == 'O') {
				//TODO : ignore this unit;
			}
			continue;
		}

		if (IS_T(line_ptr, '>')) {
			line_ptr += 4;
			if (ku == NULL) continue; // invalid test stmt
//			kt_truncate_line(line_ptr);
			if (ku->stmtsize == 0) {
				ku->shead = new_kt_stmt(ctx, line_ptr);
				ks = ku->shead;
				ku->current = ks;
			} else {
				if (isResultReady == 1) {
					ks->isPassed = 2; /* including both case; ignore intentionally, or no result */
					isResultReady = 0;
				}
				ks->next = new_kt_stmt(ctx, line_ptr);
				ks = ks->next;
			}
			ku->stmtsize++;
			int check;
			if ((check = api->checkstmt(ks->testBody)) == 0) {
				// stmt is valid. exec first stmt
				isResultReady = 1;
				continue;
			}
			if (check < 0) {
				KNH_SYSLOG(ctx, LOG_WARNING, "KTEST", "invalid statement at line=%d", kt->lineno);
				knh_cwb_clear(cwb, 0);
				continue;
			} else {
				// stmt is continuing;
				isStmtContinue = 1;
				continue;
			}
		}

		if (IS_T(line_ptr, '.')) {
			if (!isStmtContinue) continue; // invalid stmt.
			if (ku == NULL) continue; //invalid stmt.
			if (ks == NULL) continue; // invalid stmt
//			kt_truncate_line(line_ptr);
			add_kt_stmt_body(ctx, ks, line_ptr + 4);
			int check;
			if ((check = api->checkstmt(ks->testBody)) == 0) {
				isStmtContinue = 0;
				isResultReady = 1;
				continue;
			}
			if (check < 0) {
				KNH_SYSLOG(ctx, LOG_WARNING, "KTEST", "invalid statement at line=%d", kt->lineno);
				knh_cwb_clear(cwb, 0);
			}
			continue;
		}
		// handle result
		if (isResultReady) {
			if (line_ptr[0] != '\n' && line_ptr[0] != '\0') {
				add_kt_stmt_result(ctx, ks, line_ptr);
			}

		}
	}
	if (isUnitStarted && ku->current != NULL) {
		ks = ku->current;
		knh_cwb_write(ctx, cwb, ks->testBody);
		return 1;
	}
	return 0;
}


static void test_dump(FILE *fp, const char *linehead, const char *body, const char *foot)
{
	const char *p = body;
	while(*p != 0) {
		int ch = *p;
		fputc(ch, fp);
		if(ch == '\n') {
			fputs(linehead, fp);
		}
		p++;
	}
	fputs(foot, fp);
}


static void test_display(Ctx *ctx, void *status, const char* result, const knh_ShellAPI_t *api)
{
	kt_status_t *kt = (kt_status_t*)status;
	kt_unit_t *ku;
	kt_stmt_t *ks;
	if (kt == NULL) return;
	ku = kt->current;
	if (ku == NULL) return;
	ks = ku->current;
	size_t len = knh_strlen(result);

	// we cannot know if there is a result or?
	if (ks->testResult.len == 0) {
		//ignoring result;
		ks->isPassed = 2; //ignore
		return;
	}
	knh_uchar_t *charResult = ks->testResult.ubuf;
	if (strncmp(result,(char*)charResult, len) == 0) {
		ks->isPassed = 1;
		kt->sumOfPassed++;
//		if(isTestVerbose) {
//			fprintf(kt->out, "[PASSED] %s \n", ku->testTitle.text);
//		}
	} else {
		ks->isPassed = 0;
		kt->sumOfFailed++;
		fprintf(kt->out, "[FAILED] %s\nTESTED:\n>>> ", ku->testTitle.text);
		test_dump(kt->out, "... ", ks->testBody.text, "\n\t");
		test_dump(kt->out, "\t", ks->testResult.text, "\nRESULTS:\n\t");
		test_dump(kt->out, "\t", result, "\n");
	}
	if (ks->next == NULL) {
		size_t i;
		ks = ku->shead;
		int isAllPassed = 1;
		int p = 0,  f = 0;
		for(i = 1; i <= ku->stmtsize; i++) {
			if (ks->isPassed == 0 /* failed */) {
				isAllPassed = 0;
				f++;
			} else if (ks->isPassed == 1){
				p++;
			}
			ks = ks->next;
		}
		if (isAllPassed) {
			fprintf(kt->out, "[PASSED] %s\n", ku->testTitle.text);
		}
		// if its failed, already alerted.
	}
}

static void test_cleanup(Ctx *ctx, void *status)
{
	kt_status_t *kt = (kt_status_t*)status;
	if (kt == NULL) return;
	fclose(kt->in);
	if (kt->unitsize != 0)  {  // removed goto, by kimio
		kt_unit_t *ku = kt->uhead, *clean_ku;
		kt_stmt_t *ks = ku->shead, *clean_ks;
		size_t i, j;
		/* traverse test result, and sum up */
		size_t unit_passed = 0, isAllPassed = 1, line = 0;
		size_t total_stmts = 0, ignore_units = 0; /* for TODOs, and Empties */
		for (i = 1; i <= kt->unitsize; i++) {
			if (ku->stmtsize ==0) {
				clean_ku = ku;
				ignore_units++;
				if (i < kt->unitsize) {
					ku = ku->next;
					ks = ku->shead;
				}
				KNH_FREE(ctx, clean_ku->testTitle.ubuf, clean_ku->testTitle.len + 1);
				KNH_FREE(ctx, clean_ku, sizeof(kt_unit_t));
				continue;
			}// ignore
			total_stmts += ku->stmtsize;
			for (j = 1; j <= ku->stmtsize; j++) {
				if (ks->isPassed == 0 /*failed*/) {
					line = j;
					isAllPassed = 0;
				}
				clean_ks = ks;
				if (j < ku->stmtsize) {
					ks = ks->next;
				}
				// cleanup ks
				if (clean_ks->testBody.len > 0)
					KNH_FREE(ctx, clean_ks->testBody.ubuf, clean_ks->testBody.len + 1);
				if (clean_ks->testResult.len  > 0)
					KNH_FREE(ctx, clean_ks->testResult.ubuf, clean_ks->testResult.len + 1);
				KNH_FREE(ctx, clean_ks, sizeof(kt_stmt_t));
			}
			if (isAllPassed == 1 /* all passed*/ ) unit_passed++;
			isAllPassed = 1;
			clean_ku = ku;
			if (i < kt->unitsize) {
				ku = ku->next;
				ks = ku->shead;
			}
			// clean up ku
			KNH_FREE(ctx, clean_ku->testTitle.ubuf, clean_ku->testTitle.len + 1);
			KNH_FREE(ctx, clean_ku, sizeof(kt_unit_t));
		}
		if(kt->out != stderr && kt->out != stdout) {
			fclose(kt->out);
		}
		if(isTestVerbose) {
			fprintf(kt->out, "%s: %d of %d units, %d of %d tests have been passed\n",
					kt->filename.text, (int)unit_passed, (int)(kt->unitsize - ignore_units),
					(int)kt->sumOfPassed, (int)(kt->sumOfPassed + kt->sumOfFailed));
		}
		knh_statUnitTest(ctx, kt->sumOfPassed, kt->sumOfFailed);
	}
	KNH_FREE(ctx, kt->filename.ubuf, kt->filename.len + 1);
	KNH_FREE(ctx, kt, sizeof(kt_status_t));
}

static const knh_ShellSPI_t testSPI = {
		"konoha builtin test mode",
		test_init,
		test_readstmt,
		test_display,
		test_cleanup,
};

void konoha_runTest(konoha_t konoha, int argc, const char **argv)
{
	KONOHA_CHECK_(konoha);
	Ctx *ctx = KONOHA_BEGIN(konoha.ctx);
	int i = 0;
#ifndef K_USING_DEBUG
	KNH_SETv(ctx, ((knh_Context_t*)ctx)->out, new_BytesOutputStream(ctx, new_Bytes(ctx, K_PAGESIZE)));
	KNH_SETv(ctx, ((knh_Context_t*)ctx)->err, ctx->out);
#endif
	if(i + 1 == argc) isTestVerbose = 1;
	for(i = 0; i < argc; i++) {
		knh_shell(ctx, argv[i], &testSPI, NULL);
		shell_restart(ctx);
	}
	KNH_SETv(ctx, ((knh_Context_t*)ctx)->out, DP(ctx->sys)->out);
	KNH_SETv(ctx, ((knh_Context_t*)ctx)->err, DP(ctx->sys)->err);
	{
		size_t passed = ctx->stat->utestPassed;
		size_t failed = ctx->stat->utestFailed;
		if(failed != 0) {
			KNH_SYSLOG(ctx, LOG_ERR, "MiniTest", "*%ld of %ld tests were failed", failed, passed + failed);
		}
	}
	KONOHA_END(ctx);
}

#ifdef __cplusplus
}
#endif
