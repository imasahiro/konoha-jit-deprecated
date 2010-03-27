#include "./client.h"
static const char *CopyRight = ""
"Konoha Script Debugger (KDB) 0.0.1\n"
"Copyright (C) 2010 Masahiro Ide.\n";
//"For bug reporting instructions, please see:"
//"<http://www.gnu.org/software/gdb/bugs/>..."


int cmd(Ctx *ctx, knh_sfp_t *sfp, knh_OutputStream_t *out, char *fmt);

char *client_gets(Ctx *ctx, char *buf, size_t size)
{
        knh_printf(ctx, ctx->out, "(kdb) ");
        return fgets(buf, size, stdin);
}

int dev_open()
{
    int fd;
    if ((fd = open("/dev/kdb", O_RDONLY)) < 0) {
        perror("kdb");
        exit(errno);
    }
    return fd;
}



int main (int argc, char **argv)
{
    char buf[128];
    fprintf(stderr, "%s\n", CopyRight);
    int fd = dev_open();
    konoha_t konoha = konoha_open(4096);
    Ctx *ctx = knh_beginContext(konoha.ctx);
    knh_sfp_t *sfp = KNH_LOCAL(ctx);
    p_integer(sfp[0]) = fd;

    while(client_gets(ctx, buf, sizeof(buf)) != NULL){
        buf[strlen(buf)-1] = '\0';
        cmd(ctx, sfp, ctx->out, buf);
    }
    return 0;
}

static int client_quit(Ctx *ctx, knh_sfp_t *sfp, knh_OutputStream_t *out, knh_bytes_t cmd)
{
    knh_endContext(ctx);
    exit(0);
    return 0;
}

static int client_null(Ctx *ctx, knh_sfp_t *sfp, knh_OutputStream_t *out, knh_bytes_t cmd)
{
    knh_printf(ctx, out, "Unknown Command\n");
    return 0;
}

static int client_next(Ctx *ctx, knh_sfp_t *sfp, knh_OutputStream_t *out, knh_bytes_t cmd)
{
    int fd = p_int(sfp[0]);
    struct msg_next msg;
    int ret;
    if ((ret=ioctl(fd, KDB_IOC_RUN, &msg)) < 0) {
        knh_printf(ctx, out,"ioctl error\n");
    } else {
        knh_printf(ctx, out, "%s\n",msg.data);
    }
    return 0;
}

static int client_print(Ctx *ctx, knh_sfp_t *sfp, knh_OutputStream_t *out, knh_bytes_t cmd)
{
    int fd = p_int(sfp[0]);
    //fprintf(stderr, "print:fd=%d,cmd='%s'\n", fd, cmd.buf);
    struct msg_print msg;
    strncpy(msg.data, (char*)cmd.buf, 128);
    int ret;
    if ((ret=ioctl(fd, KDB_IOC_PRINT, &msg)) < 0) {
        knh_printf(ctx, out, "ioctl error\n");
    }
    knh_printf(ctx, out, "%s\n",msg.data);
    return 0;
}

static int client_bt(Ctx *ctx, knh_sfp_t *sfp, knh_OutputStream_t *out, knh_bytes_t cmd)
{
    // TODO
    knh_printf(ctx, out, "bt:cmd='%s'\n", cmd.buf);
    return 3;
}


static struct op opr[] = {
    {"null"     , 4, 0, client_null},
    {"next"     , 4, 1, client_next},
    {"n"        , 1, 1, client_next},
    {"print"    , 5, 0, client_print},
    {"p"        , 1, 0, client_print},
    {"bt"       , 2, 0, client_bt},
    {"backtrace", 9, 0, client_bt},
    {"quit"     , 4, 0, client_quit},
    {"exit"     , 4, 0, client_quit},
    {NULL}
};

static fopr_t parse_opr(knh_bytes_t cmd)
{
    int i;
    for (i = 0; i < OPR_SIZE; i++) {
        opr_t *o = &opr[i];
        knh_bytes_t oname = B2(o->name, o->len);
        if (knh_bytes_strcmp(oname, cmd) == 0) {
            return o->fopr;
        }
    }
    return opr[0].fopr;
}

int cmd(Ctx *ctx, knh_sfp_t *sfp, knh_OutputStream_t *out, char *fmt)
{
    if(fmt == NULL) {
        return 1;
    }
    knh_bytes_t b = B(fmt);
    knh_bytes_t sp = B(" ");
    int loc = knh_bytes_indexOf(b, sp);
    knh_bytes_t c;
    if(loc == -1) {
        c = b;
    }
    else {
        c = knh_bytes_first(b, loc);
    }
    fopr_t func = parse_opr(c);
    return func(ctx, sfp, out, b);
}

