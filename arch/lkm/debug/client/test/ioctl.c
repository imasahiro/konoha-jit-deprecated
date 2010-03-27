#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include "../debug.h"
#include "../ioctl.h"

#define OK() {\
    fprintf(stderr, "%s:OK!!(cmd=%d)\n",d->name, d->cmd);\
    return 1;\
}

#define NG() {\
    fprintf(stderr, "%s:NG!!(cmd=%d)\n",d->name, d->cmd);\
    return 0;\
}

static fopr_t parse_opr(knh_bytes_t cmd);


#include "cmd.c"

#define TEST(name, cmd, func, data, ans) {name, cmd, func, (void*)data, (void*)ans}
struct test_data data[] = {
    TEST("KDB_IOC_RESET", KDB_IOC_RESET, test_reset, 0,0), 
    TEST("KDB_IOC_RUN"  , KDB_IOC_RUN  , test_run  , 0,"run"), 
    TEST("KDB_IOC_SET"  , KDB_IOC_SET  , test_set  , "set Device.read","ok"), 
    TEST("KDB_IOC_PRINT", KDB_IOC_PRINT, test_print, "print %k(sfp[2])","print"), 
    TEST("KDB_IOC_NEXT" , KDB_IOC_NEXT , test_next , 0,"next"), 
};
int main (int argc, char **argv) 
{
    int fd;
    fprintf(stderr, "open /dev/kdb:");
    if ((fd = open("/dev/kdb", O_RDONLY)) < 0) {
        fprintf(stderr, "NG!!\n");
        perror(argv[0]);
        exit(errno);
    }
    fprintf(stderr, "ok\n");

    test(fd, data);
    close(fd);

    return 0;
}

int cmd(Ctx *ctx, knh_sfp_t *sfp, char *fmt)
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
    return func(ctx, sfp, b);
}

void exec_fmt(Ctx *ctx, knh_sfp_t *sfp, char *fmt);
int op_null(Ctx *ctx, knh_sfp_t *sfp, knh_bytes_t cmd)
{
    return 1;
}
int op_next(Ctx *ctx, knh_sfp_t *sfp, knh_bytes_t cmd)
{
    return 0;
}
int op_print(Ctx *ctx, knh_sfp_t *sfp, knh_bytes_t cmd)
{
    //fprintf(stderr, "%s\n",cmd.buf);
    knh_bytes_t sp = B(" ");
    int loc = knh_bytes_indexOf(cmd, sp);
    if(loc == -1) {
        //fprintf(stderr, "???\n");
        return 1;
    }
    else {
        knh_bytes_t c = knh_bytes_last(cmd, loc + 1);
        if(c.len > 4) { // strlen("%s()")
            //exec_fmt(ctx, sfp, (char*)c.buf);
        }
    }
    return 1;
}
int op_bt(Ctx *ctx, knh_sfp_t *sfp, knh_bytes_t cmd)
{
    // TODO
    // we have to imprement backtrace
    //knh_stack_writeStackTrace(ctx, sfp, ctx->out);
    //knh_write_EOL(ctx, ctx->out);
    return 1;
}

static struct op opr[] = {
    {"next"     , 4, 1, op_next},
    {"n"        , 1, 1, op_next},
    {"print"    , 5, 0, op_print},
    {"p"        , 1, 0, op_print},
    {"bt"       , 2, 0, op_bt},
    {"backtrace", 9, 0, op_bt},
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
    return op_null;
}

