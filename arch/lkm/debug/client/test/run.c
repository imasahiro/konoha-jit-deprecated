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
// gcc ./run.c -lkonoha
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
    struct msg_next msg;
    int ret;
    if ((ret=ioctl(fd, KDB_IOC_RUN, &msg)) < 0) {
        fprintf(stderr, "ioctl error\n");
    }
    fprintf(stderr, "msg='%s'\n",msg.data);

    close(fd);

    return 0;
}

