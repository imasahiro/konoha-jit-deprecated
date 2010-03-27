#include <linux/module.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include "./internal.h"
#define KFS_MAGIC 0x12345

struct kmem_cache *kfs_inode_cache;
static const struct super_operations kfs_ops;
struct kfs_mount_opts {
    umode_t mode;
};

struct kfs_fs_info {
    struct kfs_mount_opts mount_opts;
};

struct inode *kfs_new_inode(struct inode *dir, int mode)
{
    struct super_block *sb = dir->i_sb;
    struct inode* inode;

    inode = new_inode(sb);
    if (!inode) return ERR_PTR(-ENOMEM);
    inode->i_mode  = mode;
    inode->i_uid = current_fsuid();
    inode->i_gid = current_fsgid();
    inode->i_atime = inode->i_mtime = inode->i_ctime = CURRENT_TIME;
    return inode;
}

struct inode *kfs_iget(struct super_block *sb, ino_t ino)
{
    struct inode *inode = iget_locked(sb, ino);
    printk("%s:%d\n", __func__, __LINE__);
    if (!inode) {
        return ERR_PTR(-ENOMEM);
    }
    if (!(inode->i_state & I_NEW)) {
        return inode;
    }
    inode->i_uid = current_fsuid();
    inode->i_gid = current_fsgid();
    inode->i_atime = inode->i_mtime = inode->i_ctime = CURRENT_TIME;
    unlock_new_inode(inode);
    return inode;
}

static struct inode *kfs_get_root(struct super_block *sb)
{
    struct inode *inode = kfs_iget(sb, 1);
    if (inode) {
        inode->i_mode = S_IFDIR;
        inode->i_op  = &kfs_dir_inode_operations;
        inode->i_fop = &simple_dir_operations;
        inc_nlink(inode);
    }
    return inode;
}


static int kfs_fill_super(struct super_block * sb, void * data, int silent)
{
    struct kfs_fs_info *fsi;
    struct inode *inode = NULL;
    struct dentry *root;
    int err;

    printk("%s:%d\n", __func__, __LINE__);
    save_mount_options(sb, data);

    fsi = kzalloc(sizeof(struct kfs_fs_info), GFP_KERNEL);
    sb->s_fs_info = fsi;
    if (!fsi) {
        err = -ENOMEM;
        goto fail;
    }

    sb->s_magic  = KFS_MAGIC;
    sb->s_op     = &kfs_ops;
    sb->s_time_gran = 1;

    inode = kfs_get_root(sb);
    if (!inode) {
        err = -ENOMEM;
        goto fail;
    }

    root = d_alloc_root(inode);
    sb->s_root = root;
    if (!root) {
        err = -ENOMEM;
        goto fail;
    }

    return 0;
fail:
    kfree(fsi);
    sb->s_fs_info = NULL;
    iput(inode);
    return err;
}

static void init_once(void *data)
{
    struct kfs_inode *i = (struct kfs_inode *) data;
    inode_init_once(&i->inode);
}

static struct inode *kfs_inode_alloc(struct super_block *sb)
{
    static int count = 0;
    struct kfs_inode *inode;
    char *data;
    Ctx *ctx = knh_getCurrentContext();

    inode = kmem_cache_alloc(kfs_inode_cache, GFP_KERNEL);
    if (!inode)
        return NULL;
    data = kzalloc(10, GFP_KERNEL);
    snprintf(data, 10, "hi%d", count++);
    inode->data = (void *) data;
    KNH_INITv(inode->entry_table, new_DictSet0(ctx, 16));
    return (struct inode *)inode;
}

static void kfs_inode_destroy(struct inode *_inode)
{
    struct kfs_inode *inode = (struct kfs_inode *)_inode;
    Ctx *ctx = knh_getCurrentContext();
    KNH_FINALv(ctx, inode->entry_table);
    kmem_cache_free(kfs_inode_cache, (inode));
}

int kfs_get_sb(struct file_system_type *fs_type,
        int flags, const char *dev_name, void *data, struct vfsmount *mnt)
{
    printk("%s:%d\n", __func__, __LINE__);
    return get_sb_nodev(fs_type, flags, data, kfs_fill_super, mnt);
}


static void kfs_kill_sb(struct super_block *sb)
{
    kfree(sb->s_fs_info);
    kill_litter_super(sb);
}
static const struct super_operations kfs_ops = {
    .statfs       = simple_statfs,
    .alloc_inode  = kfs_inode_alloc,
    .destroy_inode= kfs_inode_destroy,
    .show_options = generic_show_options,
};

static struct file_system_type kfs_fs_type = {
    .name      = "kfs",
    .get_sb    = kfs_get_sb,
    .kill_sb   = kfs_kill_sb,
};

konoha_t konoha  = {KONOHA_MAGIC};
static int __init init_kfs_fs(void)
{
    konoha_t k = konoha_open(128);
    Ctx *ctx;
    konoha.magic = k.magic;
    konoha.ctx   = k.ctx;
    konoha_setOutputStreamBuffer(konoha, 128, 128);
    ctx = knh_beginContext(konoha.ctx);
    printk("%s:%d,ctx=%p\n", __func__, __LINE__,ctx);
    kfs_inode_cache = kmem_cache_create("kfs_inode_cache", 
            sizeof(struct kfs_inode), 0, 
            SLAB_RECLAIM_ACCOUNT | SLAB_MEM_SPREAD, init_once);
    return register_filesystem(&kfs_fs_type);
}

static void __exit exit_kfs_fs(void)
{
    printk("%s:%d,ctx=%p\n", __func__, __LINE__,konoha.ctx);
    unregister_filesystem(&kfs_fs_type);
    kmem_cache_destroy(kfs_inode_cache);
    knh_endContext(konoha.ctx);
    konoha_close(konoha);
}

module_init(init_kfs_fs);
module_exit(exit_kfs_fs);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("masahiro ide");
MODULE_VERSION("0.1");
