#ifndef KDB_IOCTL_H
#define KDB_IOCTL_H

struct msg_set {
	char data[256];
};

struct msg_next {
	char data[256];
};


struct msg_print {
	char data[256];
	//knh_class_t cid;
	//int sfpidx;
};

#define KDB_IOC_MAGIC 'k'
#define KDB_IOC_RESET  _IO(  KDB_IOC_MAGIC, 0)
#define KDB_IOC_RUN    _IOR( KDB_IOC_MAGIC, 1, struct msg_next)
#define KDB_IOC_SET    _IOW( KDB_IOC_MAGIC, 2, struct msg_set)
#define KDB_IOC_PRINT  _IOWR(KDB_IOC_MAGIC, 3, struct msg_print)
#define KDB_IOC_NEXT   _IOR( KDB_IOC_MAGIC, 4, struct msg_next)

#endif
