#include "./debug.h"
#include "./ioctl.h"
#ifdef KONOHA_ON_LKM
#include <linux/module.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/semaphore.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#endif

KNHAPI(int) init(Ctx *ctx)
{
    KNH_NOTICE(ctx, "loading debug...");
    knh_class_t cid = CLASS_Method;
    knh_bytes_t b = B("setDebugMode");
    knh_methodn_t mn = knh_getmn(ctx, b, METHODN_NEWID);
    knh_MethodField_t *mf = new_MethodField(ctx, TYPE_void, 0);
    knh_Method_t *mtd = new_Method(ctx, 0, cid, mn, Method_setDebugMode);
    knh_ClassField_t *cs = ClassTable(cid).cstruct;
    knh_Array_add(ctx, cs->methods, UP(mtd));
    knh_addMethodFieldTable(ctx, mf);
    return 1;
}

#ifdef KONOHA_ON_LKM
static int kdb_open (struct inode *inode, struct file *filp);
static int kdb_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg);

static struct file_operations kdb_fops = {
    .owner = THIS_MODULE,
    .open  = kdb_open,
    .ioctl = kdb_ioctl,
};

struct kdb_t {
    dev_t id;
    struct cdev cdev;
    knh_Method_t *mtd;
};
static struct kdb_t *dev_ptr = NULL;


static int kdb_open (struct inode *inode, struct file *filp) 
{
    filp->private_data = container_of(inode->i_cdev, struct kdb_t, cdev);
    printk("[%s]\n",__func__);return 0;
}

#define SIZE_MSG_SET sizeof(struct msg_set)
#define SIZE_MSG_PRINT sizeof(struct msg_print)
#define SIZE_MSG_NEXT sizeof(struct msg_next)

static char *kdb_buf = NULL;
static int  kdb_len  = 0;
static int wait_flag = 0;
static int exec_flag = 0;
#if 0
#define DBG() printk("%s:%d w=%d,e=%d\n",__func__,__LINE__, wait_flag,exec_flag);
#else
#define DBG()
#endif
static DECLARE_WAIT_QUEUE_HEAD(wait);
static DECLARE_WAIT_QUEUE_HEAD(exec);

void kdb_write(Ctx *ctx, knh_OutputStream_t *out)
{
	printk("%s:%d (size=%d)\n",__func__, __LINE__, out->b->ba->size);
	if(kdb_buf) {
		knh_Bytes_t *ba = out->b->ba;
		size_t len = ba->size;
		char *buf  = ba->buf;
		printk("w\n");
		//IDEBUG("buf='%s'(%d)", buf,len);
		//IDEBUG("%s", kdb_buf);
		strncpy(kdb_buf, buf, len);
		kdb_buf[len-1] = '\0';
		kdb_len = len;
		//IDEBUG("%s", kdb_buf);
		knh_Bytes_clear(ba, 0);
	}
}
char *kdb_readline(void)
{
    //struct msg_next msg = {"run"};
    DBG();
    if(waitqueue_active(&wait)) { // if debugger is stopped
        DBG();
        wait_flag = 1;
        wake_up(&wait);
    }
    DBG();
    wait_event_interruptible(exec,exec_flag!=0);
    DBG();
    if(waitqueue_active(&wait)) {
        DBG();
        wait_flag = 1;
        wake_up(&wait);
    }
    exec_flag = 0;
    IDEBUG("cmd='%s'(%d)", kdb_buf, kdb_len);
    DBG();
    return kdb_buf;
}

int wait_for_dbg(void *buf, char *cmd)
{
    struct msg_next msg;//= {"run"};
    DBG();
    if(cmd != NULL) {
        kdb_len = strlen(cmd) + 1;
        kdb_buf = kzalloc(kdb_len, GFP_KERNEL);
        strncpy(kdb_buf, cmd, kdb_len);
        IDEBUG("cmd='%s', set->'%s'",cmd, kdb_buf);
    }
    DBG();
    if(waitqueue_active(&exec)) {
        DBG();
        exec_flag = 1;
        wake_up(&exec);
    }
    DBG();
    wait_event_interruptible(wait,wait_flag!=0);
    DBG();
    if(waitqueue_active(&exec)) {
        DBG();
        exec_flag = 1;
        wake_up(&exec);
    }

    IDEBUG("cmd='%s', kdb_buf='%s'", cmd, kdb_buf);
    if(kdb_buf != NULL) {
        strncpy(msg.data, kdb_buf, kdb_len);
        IDEBUG("cmd='%s', set->'%s', kdb_buf='%s'", cmd, msg.data, kdb_buf);
        if(copy_to_user(buf, &msg, SIZE_MSG_NEXT)) {
            printk("copy_to_user failed\n");
            return -EFAULT;
        }
        kfree(kdb_buf); kdb_buf = NULL;kdb_len = 0;
    }
    wait_flag = 0;
    DBG();
    return 0;
}

static int kdb_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
    int retval = 0;
    struct kdb_t *dev = filp->private_data;
    void __user *argp = (void __user*) arg;

    switch (cmd) {
    case KDB_IOC_RESET:
        IDEBUG("cmd=%d, reset",cmd);
        break;
    case KDB_IOC_RUN:
    {
        retval = wait_for_dbg(argp, "next");
        IDEBUG("cmd=%d, run",cmd);
        break;
    }
    case KDB_IOC_SET:
    {
        struct msg_set msg, *user_msg = argp;
        if(copy_from_user(&msg, user_msg, SIZE_MSG_SET)) {
            retval = -EFAULT;
        }
        retval = wait_for_dbg(argp, msg.data);
        IDEBUG("cmd=%d, set '%s'", cmd, msg.data);
        break;
    }
    case KDB_IOC_PRINT:
    {
        struct msg_print msg, *user_msg = argp;
        if(copy_from_user(&msg, user_msg, SIZE_MSG_PRINT)) {
            retval = -EFAULT;
        }
        retval = wait_for_dbg(argp, msg.data);
        break;
    }
    case KDB_IOC_NEXT:
    {
        retval = wait_for_dbg(argp, "next");
        break;
    }

    default:
        retval = -ENOTTY;
        break;
    }
    return retval;
}

static struct func_data debug_data[] = {
    {"Method", "setDebugMode", Method_setDebugMode},
    {NULL}
};

static int __init init_debug(void)
{
    Ctx *ctx = knh_getCurrentContext();
    init(ctx);
    dev_ptr = kmalloc(sizeof(struct kdb_t), GFP_KERNEL);
    struct kdb_t *dev = dev_ptr;
    alloc_chrdev_region(&dev->id, 0, 1, "kdb");
    cdev_init(&dev->cdev,&kdb_fops);
    dev->cdev.owner = THIS_MODULE;
    KNH_INITv(dev->mtd, KNH_NULL);
    cdev_add(&dev->cdev, dev->id, 1);
    //printk("%s:%d, d->mtd=%p,dp->mtd=%p\n", __func__, __LINE__,dev->mtd,dev_ptr->mtd);
    init_mtd(ctx, debug_data);
    printk(KERN_INFO "kdb init; ctx=%p\n",ctx);
    return 0;
}

static void __exit exit_debug(void)
{
    Ctx *ctx = knh_getCurrentContext();
    knh_class_t cid = CLASS_Method;
    knh_bytes_t b = B("setDebugMode");
    struct kdb_t *dev = dev_ptr;
    //printk("%s:%d, d=%p,dp=%p\n", __func__, __LINE__,dev,dev_ptr);
    knh_methodn_t mn = knh_getmn(ctx, b, METHODN_NEWID);
    knh_Method_t *mtd = knh_Class_getMethod(ctx, cid, mn);
    if(IS_Method(mtd) && mtd->fcall_1 == Method_setDebugMode) {
        knh_Method_toAbstract(ctx, mtd);
    }

    KNH_FINALv(ctx,dev->mtd);
    cdev_del(&dev->cdev);
    unregister_chrdev_region(dev->id,1);
    printk(KERN_INFO "kdb exit; ctx=%p\n",ctx);
    kfree(dev_ptr);

}


module_init(init_debug);
module_exit(exit_debug);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("masahiro ide");
MODULE_VERSION("0.1");
#endif
