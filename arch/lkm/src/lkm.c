/* ------------------------------------------------------------------------
 * konohadev - char device for konoha
 * Masahiro Ide
 *
 * ------------------------------------------------------------------------ */

#ifdef KONOHA_ON_LKM

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/init.h>
#include <asm/uaccess.h>
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,25))
#include <linux/semaphore.h>
#else
#define down_interruptible(ptr) (0)
#define up(ptr);
#define init_MUTEX(ptr);
#endif

#include <konoha.h>
#include <konoha/konoha_api.h>
#include "konoha/konoha_lkm_api.h"
#include "konoha/lkm.h"

void init_device_mtd(Ctx *ctx);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("masahiro ide");

enum {
	MAXCOPYBUF =256 
};

struct konohadev_t {
	dev_t id;
	struct cdev cdev;
	konoha_t  konoha;
	char* buffer;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,25))
	struct semaphore sem;
#endif
};

static const char *msg = "konohadev";
static struct konohadev_t *konohadev_p;

static int knh_dev_open (struct inode *inode , struct file *filp);
static ssize_t knh_dev_read(struct file *filp, char __user *user_buf,
		size_t count, loff_t *offset);

static ssize_t knh_dev_write(struct file *file,const char __user *buf,
		size_t count,loff_t *offp) ;

static struct file_operations knh_fops = {
	.owner = THIS_MODULE,
	.open  = knh_dev_open,
	.read  = knh_dev_read,
	.write = knh_dev_write,
};

static int knh_dev_open (struct inode* inode, struct file *filp)
{
	filp->private_data = container_of(inode->i_cdev,
			struct konohadev_t,cdev);
	return 0;
}

static ssize_t knh_dev_read (struct file* filp, char __user *user_buf,
		size_t count, loff_t *offset)
{
	struct konohadev_t *dev = filp->private_data;
	char buf[MAXCOPYBUF];
	int len;

	if(*offset > 0) return 0;

	if(down_interruptible(&dev->sem)){
		return -ERESTARTSYS;
	}

	len = snprintf(buf,MAXCOPYBUF,"%s\n",dev->buffer);

	if(copy_to_user(user_buf,buf,len)){
		up(&dev->sem);
		printk(KERN_ALERT "%s: copy_to_user failed\n",msg);
		return -EFAULT;
	}

	up(&dev->sem);
	*offset += len;

	return len;
}

static ssize_t knh_dev_write(struct file *filp,const char __user *user_buf,
		size_t count,loff_t *offset) {
	char buf[MAXCOPYBUF];
	struct konohadev_t *dev = filp->private_data;
	long len;

	if(down_interruptible(&dev->sem)){
		return -ERESTARTSYS;
	}

	len = copy_from_user(buf,user_buf,count);
	memset(dev->buffer,0,sizeof(char)*MAXCOPYBUF);
	buf[count] = '\0';
	printk("[%s][user_buf='%s']\n", __FUNCTION__,buf);
	konoha_evalScript(dev->konoha,buf);
	snprintf(dev->buffer,MAXCOPYBUF,"%s",konoha_getStdOutBufferText(dev->konoha));
	printk(KERN_DEBUG "[%s][dev->buffer='%s']\n",__func__ ,dev->buffer);
	up(&dev->sem);
	return count;
}

void Ctx_evalScript(Ctx *ctx, char *data)
{
	// Don't use cwb->ba for new_BytesInputStream (by kimio)

//	knh_sfp_t *lsfp = KNH_LOCAL(ctx);
//	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
//	knh_cwb_write(ctx, cwb, B(data));
//	knh_cwb_putc(ctx, cwb, '\n');
//	knh_InputStream_t *in = new_BytesInputStream(ctx, cwb->ba, cwb->pos, knh_Bytes_size(cwb->ba));
//	KNH_LPUSH(ctx, in);
//	DP(in)->uri = URI_EVAL;
//	DP(in)->line = 0;
//	knh_NameSpace_load(ctx, ctx->share->mainns, in, 1/*isEval*/,0/*isThrowable*/);
//	knh_cwb_close(cwb);
//	KNH_LOCALBACK(ctx, lsfp);
}
EXPORT_SYMBOL(Ctx_evalScript);

static char *device_script = ""
"class Chardev;\n"
"class Task;\n"
"class File;\n"
"\n"
"Chardev! Chardev.new(String! devname);\n"
"void Chardev.open(File! file);\n"
"@Virtual void Chardev.read(File! file, OutputStream ous);\n"
"@Virtual void Chardev.write(File! file, InputStream ins);\n"
"Boolean Chardev.register();\n"
"Boolean Chardev.unregister();\n"
"Int File.getMode();\n"
"Int Task.getPid();\n"
"\n"
"class MyDev extends Chardev {};\n"
"void MyDev.read(File! file, OutputStream ous)\n"
"{\n"
"	ous << \"hello world\" << EOL;\n"
"}\n"
"\n"
"void MyDev.write(File! file, InputStream ins)\n"
"{\n"
"	OUT << ins.readLine() << EOL;\n"
"}\n";


static void knh_dev_setup(struct konohadev_t *dev)
{
	int err = alloc_chrdev_region(&dev->id, 0, 1, msg);
	if(err){
		printk(KERN_ALERT "%s: alloc_chrdev_region() failed (%d)\n",
				msg,err);
		return;
	}
	cdev_init(&dev->cdev,&knh_fops);
	dev->cdev.owner = THIS_MODULE;
	dev->konoha = konoha_open(MAXCOPYBUF);
	konoha_setOutputStreamBuffer(dev->konoha, MAXCOPYBUF,MAXCOPYBUF);
	dev->buffer = kmalloc(sizeof(char)*MAXCOPYBUF,GFP_KERNEL);
	memset(dev->buffer,0,sizeof(char)*MAXCOPYBUF);
	init_MUTEX(&dev->sem);

	err = cdev_add(&dev->cdev, dev->id, 1);
	if(err){
		printk(KERN_ALERT "%s:cdev_add() failed(%d)\n",msg,err);
		return;
	}
	Ctx *ctx = knh_beginContext(dev->konoha.ctx);
	Ctx_evalScript(ctx, device_script);
	init_device_mtd(ctx);
}

// Start/Init function
static int __init konohadev_init(void) {
	konohadev_p = kmalloc(sizeof(struct konohadev_t),GFP_KERNEL);
	if(!konohadev_p){
		printk(KERN_ALERT "%s:kmalloc() failed\n",msg);
		return -ENOMEM;
	}
	memset(konohadev_p,0,sizeof(struct konohadev_t));
	printk(KERN_INFO "konoha init!\n");
	knh_dev_setup(konohadev_p);
	return 0;
}

// End/Cleanup function
static void __exit konohadev_exit(void) {
	knh_endContext(konohadev_p->konoha.ctx);
	konoha_close(konohadev_p->konoha);
	kfree(konohadev_p->buffer);
	cdev_del(&konohadev_p->cdev);
	unregister_chrdev_region(konohadev_p->id,1);
	printk(KERN_INFO "konoha exit\n");
	kfree(konohadev_p);
}

module_init(konohadev_init);
module_exit(konohadev_exit);

#endif /*KONOHA_ON_LKM*/

