struct test_data;
typedef int (*ftest_t) (struct test_data *d, int fd);

struct test_data {
    char *name;
    int cmd;
    ftest_t test;
    void *data;
    void *ans;
};

int test(int fd, struct test_data* d)
{
    while(d->name != NULL) {
        fprintf(stderr, "test='%s', cmd=%d, data=%s\n", d->name, d->cmd, (char*)d->data);
        if(d->test(d, fd) == 0) break;
        d++;
    }
    return 0;
}
int test_reset(struct test_data *d, int fd)
{
    int ret;
    if ((ret = ioctl(fd, d->cmd)) < 0) {
        NG();
    }
    OK();
}
int test_run(struct test_data *d, int fd)
{
    struct msg_next msg;
    int ret;
    if ((ret=ioctl(fd, d->cmd, &msg)) < 0) {
        NG();
    }
    if(strncmp(msg.data, (char *)d->ans, 128) != 0) {
        NG();
    }
    OK();
}

int test_set(struct test_data *d, int fd)
{
    struct msg_set msg = {{0}};
    int ret;
    strncpy(msg.data, d->data, 128);
    if ((ret=ioctl(fd, d->cmd, d->data)) < 0) {
        NG();
    }
    OK();
}

int test_print(struct test_data *d, int fd)
{
    struct msg_print msg;
    int ret;
    strncpy(msg.data, d->data, 128);
    fprintf(stderr, "msg='%s',data='%s'\n",msg.data,(char *)d->data);
    if ((ret=ioctl(fd, d->cmd, &msg)) < 0) {
        NG();
    }
    if(strncmp(msg.data, (char *)d->ans, 128) != 0) {
        fprintf(stderr, "cmp err\n");
        NG();
    }
    OK();
}

int test_next(struct test_data *d, int fd)
{
    struct msg_print msg;
    int ret;
    if ((ret=ioctl(fd, d->cmd, &msg)) < 0) {
        NG();
    }
    if(strncmp(msg.data, (char *)d->ans, 128) != 0) {
        NG();
    }
    OK();
}

