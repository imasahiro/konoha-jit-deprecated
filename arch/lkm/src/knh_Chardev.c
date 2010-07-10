#include <linux/module.h>
#include <linux/security.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <konoha.h>
#include <konoha/konoha_api.h>
#include "konoha/lkm.h"

//## class Chardev RawPtr;
typedef struct {
	knh_Object_t* self;
	dev_t id;
	knh_String_t* name;
	struct cdev cdev;
	knh_Method_t *read;
	knh_Method_t *write;
} knh_device_t;

//## class File RawPtr;
typedef struct {
	knh_Object_t* self;
	struct file *file;
} knh_file_t;

//## class Inode RawPtr;
typedef struct {
	knh_Object_t* self;
	struct inode *inode;
} knh_inode_t;

//## class Task Object;
typedef struct {
	knh_Object_t* self;
	struct task_struct *task;
} knh_task_t;


/* ======================================================================== */
static knh_device_t* new_device(Ctx *ctx, knh_String_t *str)
{
	knh_device_t *d = kmalloc(sizeof(knh_device_t), GFP_KERNEL);
	printk("%s:%d,%d\n",__func__,__LINE__,sizeof(knh_device_t));
	KNH_INITv(d->name, str);
	KNH_INITv(d->read,  KNH_NULL);
	KNH_INITv(d->write, KNH_NULL);
	printk("%s\n",__func__);
	return d;
}

static void device_gfree(Ctx *ctx, knh_RawPtr_t *g)
{
	knh_device_t *d = (knh_device_t *) g->ptr;
	printk("%s:%d\n",__func__,__LINE__);
	KNH_FINALv(ctx,d->name);
	KNH_FINALv(ctx,d->read);
	KNH_FINALv(ctx,d->write);
	kfree(d);
}

/* ------------------------------------------------------------------------ */
//## method Chardev! Chardev.new(String! devname);

METHOD Chardev_new(Ctx *ctx, knh_sfp_t *sfp)
{
	printk("%s:%d\n",__func__,__LINE__);
	knh_device_t *dev = new_device(ctx, sfp[1].s);
	knh_RawPtr_init(ctx, sfp[0].glue, dev, device_gfree);

	// set ref for object.
	// but we do not set ref because it becomes cyclic refalence.
	dev->self = sfp[0].o;
	printk("%s:%d, cid=%d\n",__func__,__LINE__, knh_Object_cid(sfp[0].o));
	KNH_RETURN(ctx, sfp, sfp[0].o);
}

/* ------------------------------------------------------------------------ */
#ifdef KONOHA_ON_LKM

static int script_open (struct inode *inode , struct file *filp);
static ssize_t script_read(struct file *filp, char __user *user_buf,
		size_t count, loff_t *offset);
static ssize_t script_write(struct file *filp,const char __user *user_buf,
		size_t count,loff_t *offset);

static struct file_operations script_fops = {
	.owner = THIS_MODULE,
	.open  = script_open,
	.read  = script_read,
	.write = script_write,
};

//## method void Chardev.open(File! file);
static int script_open (struct inode* inode, struct file *filp)
{
	filp->private_data = container_of(inode->i_cdev, knh_device_t,cdev);
	printk("%s:%d\n",__func__,__LINE__);
	//printk("%s at %d\n",__FUNCTION__,__LINE__);
	//knh_Chardev_t *self = filp->private_data;

	//Ctx *ctx = knh_getCurrentContext();
	//knh_sfp_t *sfp = KNH_LOCAL(ctx);
	//knh_bytes_t fname = B("open");
	//knh_methodn_t mn = knh_getmn(ctx, fname, METHODN_NONAME);
	//knh_Method_t *mtd = knh_lookupMethod(ctx, knh_Object_cid(self), mn);

	//if (IS_Method(mtd)) {
	//	KNH_MOV(ctx, sfp[0].o, new_ExceptionHandler(ctx));
	//	KNH_TRY(ctx, L_CATCH, sfp, 0);
	//	KNH_MOV(ctx, sfp[1].o, self);
	//	KNH_SCALL(ctx, sfp, 0, mtd, 0);
	//	printk(KERN_DEBUG "[%s:%d][out='%s']\n",__func__ ,__LINE__ ,(ctx)->out->b->ba->buf);
	//	printk(KERN_DEBUG "[%s:%d][err='%s']\n",__func__ ,__LINE__ ,(ctx)->err->b->ba->buf);
	//	knh_Context_clearstack(ctx);
	//	return 0;
	//	/* catch */
	//	L_CATCH:;
	//	KNH_PRINT_STACKTRACE(ctx, sfp, 0);
	//}
	return 0;
}

//## method @Virtual void Chardev.read(File! file, OutputStream ous);
static ssize_t script_read (struct file* filp, char __user *user_buf,
		size_t count, loff_t *offset)
{
	printk("%s at %d\n",__func__,__LINE__);
	knh_device_t *dev = filp->private_data;
	knh_Object_t *self = dev->self;

	if(*offset > 0) return 0;
	Ctx *ctx = knh_getCurrentContext();
	knh_sfp_t *sfp = KNH_LOCAL(ctx);
	knh_Method_t *mtd = dev->read;
	if (!IS_Method(mtd) || knh_Method_isAbstract(mtd)) {
		knh_bytes_t fname = B("read");
		knh_methodn_t mn = knh_getmn(ctx, fname, METHODN_NONAME);
		mtd = knh_Class_getMethod(ctx, knh_Object_cid(self), mn);
		KNH_SETv(ctx, dev->read, mtd);
	}
	//knh_File_t *f = (knh_File_t*)new_Object_bcid(ctx, CLASS_File, 0);
	//f->file = filp;

	if (IS_Method(mtd)) {
		KNH_MOV(ctx, sfp[0].o, new_ExceptionHandler(ctx));
		KNH_TRY(ctx, L_CATCH, sfp, 0);
		{
			knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
			// THIS IS DANGEROUS. DON'T USED by KIMIO
//			knh_OutputStream_t *o = new_BytesOutputStream(ctx, cwb->ba);
//			KNH_MOV(ctx, sfp[1].o, self);
//			//KNH_MOV(ctx, sfp[2].o, f);
//			KNH_MOV(ctx, sfp[2].o, KNH_NULL);
//			KNH_MOV(ctx, sfp[3].o, o);
//			KNH_SCALL(ctx, sfp, 0, mtd, 2);
//			{
//				size_t len  = knh_cwb_size(cwb);
//				char   *buf = knh_cwb_tochar(ctx, cwb);
//				if(copy_to_user(user_buf,buf,len)){
//					printk(KERN_ALERT "%s: copy_to_user failed\n",__func__);
//					return -EFAULT;
//				}
//				knh_cwb_close(cwb);
//				knh_Context_clearstack(ctx);
//				*offset += len;
//				return len;
//			}
		}
		/* catch */
		L_CATCH:;
		KNH_PRINT_STACKTRACE(ctx, sfp, 0);
	}
	return 0;
}

//## method @Virtual void Chardev.write(File! file, InputStream ins);
static ssize_t script_write(struct file *filp,const char __user *user_buf,
		size_t count,loff_t *offset)
{
	printk("%s at %d\n",__FUNCTION__,__LINE__);
	knh_device_t *dev = filp->private_data;
	knh_Object_t *self = dev->self;

	Ctx *ctx = knh_getCurrentContext();
	knh_sfp_t *sfp = KNH_LOCAL(ctx);
	knh_Method_t *mtd = dev->write;
	//knh_bytes_t fname = B("write");
	//knh_methodn_t mn = knh_getmn(ctx, fname, METHODN_NONAME);
	//knh_Method_t *mtd = knh_lookupMethod(ctx, knh_Object_cid(self), mn);
	
	printk(KERN_DEBUG "[%s:%d][mtd='%p',%d]\n",__func__ ,__LINE__ ,mtd, IS_Method(mtd));
	if (IS_Method(mtd)) {
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		size_t len = copy_from_user(cwb->ba->buf,user_buf,count);
		cwb->ba->size += count;
		KNH_MOV(ctx, sfp[0].o, new_ExceptionHandler(ctx));
		KNH_TRY(ctx, L_CATCH, sfp, 0);
		knh_InputStream_t *o = new_BytesInputStream(ctx, cwb->ba, 0, knh_cwb_size(cwb));
		KNH_MOV(ctx, sfp[1].o, self);
		KNH_MOV(ctx, sfp[2].o, o);
		KNH_SCALL(ctx, sfp, 0, mtd, 1);
		{
		printk(KERN_DEBUG "[%s:%d][out='%s']\n",__func__ ,__LINE__ ,(ctx)->out->b->ba->buf);
		printk(KERN_DEBUG "[%s:%d][err='%s']\n",__func__ ,__LINE__ ,(ctx)->err->b->ba->buf);
		knh_cwb_close(cwb);
		knh_Context_clearstack(ctx);
		return count;
		}
		/* catch */
		L_CATCH:;
		KNH_PRINT_STACKTRACE(ctx, sfp, 0);
	}
	return count;
}


#endif

static knh_bool_t knh_Chardev_register(Ctx *ctx, knh_device_t *dev)
{
#ifdef KONOHA_ON_LKM
	char* name = dev->name->str;
	int err = alloc_chrdev_region(&dev->id, 0, 1, name);
	printk("%s:%d\n",__func__,__LINE__);
	if(err){
		printk(KERN_ALERT "%s: alloc_chrdev_region() failed (%d)\n",name,err);
		return 0; // false
	}
	cdev_init(&dev->cdev,&script_fops);
	dev->cdev.owner = THIS_MODULE;
	err = cdev_add(&dev->cdev, dev->id, 1);
	if(err){
		printk(KERN_ALERT "%s: cdev_add() failed (%d)\n",name,err);
		return 0; // false
	}
#endif
	return 1;
}

/* ------------------------------------------------------------------------ */
//## method Boolean Chardev.register();

METHOD Chardev_register(Ctx *ctx, knh_sfp_t *sfp)
{
	printk("%s:%d\n",__func__,__LINE__);
	knh_device_t *dev = p_cptr(sfp[0]);
	KNH_RETURN_Boolean(ctx, sfp, knh_Chardev_register(ctx, dev))
}

/* ------------------------------------------------------------------------ */
//## method Boolean Chardev.unregister();

METHOD Chardev_unregister(Ctx *ctx, knh_sfp_t *sfp)
{
	printk("%s:%d\n",__func__,__LINE__);
	knh_device_t *dev = p_cptr(sfp[0]);
#ifdef KONOHA_ON_LKM
	cdev_del(&dev->cdev);
	unregister_chrdev_region(dev->id,1);
#endif
	KNH_RETURN_Boolean(ctx, sfp, 1);
}

/* ------------------------------------------------------------------------ */
//## method Int File.getMode();

METHOD File_getMode(Ctx *ctx, knh_sfp_t *sfp)
{
	knh_file_t *o = (knh_file_t *) p_cptr(sfp[0]);
	printk("mode=%3x\n", o->file->f_mode);
	KNH_RETURN_Int(ctx, sfp, o->file->f_mode);
}

/* ------------------------------------------------------------------------ */
//## method Int Task.getPid();

METHOD Task_getPid(Ctx *ctx, knh_sfp_t *sfp)
{
	printk("%s:%d\n",__func__, __LINE__);
	const pid_t pid = current->pid;
	KNH_RETURN_Int(ctx, sfp,(int)pid);
}

/* ------------------------------------------------------------------------ */

static struct func_data func[] = {
    {"main.Chardev", "new"       , Chardev_new},
    {"main.Chardev", "open"      , (knh_fmethod )NULL},
    {"main.Chardev", "read"      , (knh_fmethod )NULL},
    {"main.Chardev", "write"     , (knh_fmethod )NULL},
    {"main.Chardev", "register"  , Chardev_register},
    {"main.Chardev", "unregister", Chardev_unregister},
    {"main.File"   , "getMode"   , File_getMode},
    {"main.Task"   , "getPid"    , Task_getPid},
    {NULL}
};

void init_mtd(Ctx *ctx, struct func_data *d)
{
    while(d->cname != NULL) {
        //printk("func '%s.%s', func=%p\n", d->cname, d->fname, d->func);
        knh_class_t cid = knh_findcid(ctx, B(d->cname));
        if(cid != CLASS_unknown) {
            //fprintf(stderr, "find cid=%d\n",cid);
            knh_bytes_t fname = B(d->fname);
            knh_methodn_t mn = knh_getmn(ctx, fname, METHODN_NONAME);
            knh_Method_t *mtd = knh_lookupMethod(ctx, cid, mn);
            if(IS_Method(mtd) && d->func != NULL) {
                //printk("sync func '%s(cid=%d).%s(mtd=%p)'\n", d->cname, cid, d->fname, mtd);
                knh_Method_syncFunc(mtd, d->func);
            }
        }
        d++;
    }
}

EXPORT_SYMBOL(init_mtd);

void init_device_mtd(Ctx *ctx)
{
    init_mtd(ctx, func);
}


