#include "./client.h"
// gcc ./run.c -lkonoha
void frun(char *msg)
{
	fprintf(stderr, "'%s'\n",msg);
	sleep(1);
}
typedef void (*fsuc_t)(char *msg);

int dev_open()
{
	int fd;
	//fprintf(stderr, "open /dev/kdb:");
	if ((fd = open("/dev/kdb", O_RDONLY)) < 0) {
		fprintf(stderr, "NG!!\n");
		//perror(argv[0]);
		exit(errno);
	}
	//fprintf(stderr, "ok\n");
	return fd;
}

void run(int fd, fsuc_t f)
{
	struct msg_next msg;
	int ret;
	if ((ret=ioctl(fd, KDB_IOC_RUN, &msg)) < 0) {
		fprintf(stderr, "ioctl error\n");
	}
	f(msg.data);
}

void print(int fd, int sfpidx, fsuc_t f)
{
	char *fmt = "print %k(sfp[%d])";
	struct msg_print msg;
	snprintf(msg.data, 128, fmt, sfpidx);
	int ret;
	//fprintf(stderr, "%s\n",msg.data);
	if ((ret=ioctl(fd, KDB_IOC_PRINT, &msg)) < 0) {
		fprintf(stderr, "ioctl error\n");
	}
	f(msg.data);
}
int main (int argc, char **argv)
{
	int fd = dev_open();
	fprintf(stderr, "%s\t","run");
	run(fd, frun);
	fprintf(stderr, "%s\t","run");
	run(fd, frun);
	fprintf(stderr, "%s\t","run");
	run(fd, frun);
	fprintf(stderr, "%s\t","print");
	print(fd, 5, frun);
	fprintf(stderr, "%s\t","run");
	run(fd, frun);
	fprintf(stderr, "%s\t","run");
	run(fd, frun);
	fprintf(stderr, "%s\t","run");
	run(fd, frun);
	close(fd);
	return 0;
}

