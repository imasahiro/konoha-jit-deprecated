#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include "./internal.h"
#define KFS_MAGIC 0x12345

static int kfs_setattr(struct dentry *dentry, struct iattr *ia)
{
    struct inode *inode = dentry->d_inode;
    unsigned int old_ia_valid = ia->ia_valid;
    int ret = 0;
    printk("%s:%d\n", __func__, __LINE__);

    /* POSIX UID/GID verification for setting inode attributes */
    ret = inode_change_ok(inode, ia);
    if (ret)
        return ret;

    /* pick out size-changing events */
    if (ia->ia_valid & ATTR_SIZE) {
        loff_t size = i_size_read(inode);
        if (ia->ia_size != size) {
            //ret = ramfs_nommu_resize(inode, ia->ia_size, size);
            ret = -1;
            if (ret < 0 || ia->ia_valid == ATTR_SIZE)
                goto out;
        } else {
            /* we skipped the truncate but must still update
             * timestamps
             */
            ia->ia_valid |= ATTR_MTIME|ATTR_CTIME;
        }
    }

    ret = inode_setattr(inode, ia);
out:
    ia->ia_valid = old_ia_valid;
    return ret;
}

static ssize_t kfs_file_read(struct file *filp, char __user *buf, size_t len, loff_t *ppos)
{
    struct dentry *dentry = filp->f_path.dentry;
    struct inode *inode = dentry->d_inode;
    struct kfs_inode *i = (struct kfs_inode *) inode;
    char *data;
    size_t size;
    int l;
    printk("%s:%d, data='%s'\n", __func__, __LINE__,(char*)i->data);
    if(*ppos > 0) return 0;
    data = (char *)i->data;
    size = strlen(data);
    l = copy_to_user(buf, (char*)i->data, size);
    printk("%s:%d, size=%d,l=%d,ppos=%d\n", __func__, __LINE__, size, l, (int)*ppos);
    *ppos += size;
    return size;
    //return do_sync_read(filp, buf, len, ppos);
    //loff_t offs = *ppos;
    //
    //if(!count) return 0;
    //return 0;
}

static ssize_t kfs_file_write(struct file *filp, const char __user *buf, size_t len, loff_t *ppos)
{
    struct dentry *dentry = filp->f_path.dentry;
    struct inode *inode = dentry->d_inode;
    struct kfs_inode *i = (struct kfs_inode *) inode;
    char ubuf[128];
    int l = copy_from_user(ubuf, buf,len);
    char* mem;
    ubuf[len] = '\0';
    printk("%s:%d, data='%s'\n", __func__, __LINE__,(char*)i->data);
    printk("%s:%d, ubuf='%s',l=%d,len=%d\n", __func__, __LINE__,(char*)ubuf,l,len);
    kfree(i->data);
    mem = kzalloc(len + 1, GFP_KERNEL);
    memcpy(mem, ubuf, len + 1);
    i->data = mem;
    return len;
    //return do_sync_write(filp, buf, len, ppos);
}

const struct inode_operations kfs_file_inode_operations = {
    .setattr = kfs_setattr,
    .getattr = simple_getattr,
};

const struct file_operations kfs_file_operations = {
    .read      = kfs_file_read,
    .write     = kfs_file_write,
    .aio_read  = generic_file_aio_read,
    .aio_write = generic_file_aio_write,
    .fsync     = simple_sync_file,
    //.llseek    = generic_file_llseek,
};


