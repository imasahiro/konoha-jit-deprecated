#include <linux/fs.h>
#include "./internal.h"

static ino_t kfs_inode_by_name(struct inode *dir, struct qstr *child)
{
	//return (ino_t)((struct kfs_inode*) inode)->data;
	struct kfs_inode *inode = (struct kfs_inode*) dir;
	return 0;
}
static int kfs_dir_dentry_delete(struct dentry *dentry)
{
	return 1;
}
static struct dentry *kfs_dir_lookup(struct inode *dir, struct dentry *dentry, struct nameidata *nd)
{
	//static struct dentry_operations kfs_dir_dentry_operations = {
	//    .d_delete = kfs_dir_dentry_delete
	//};
	//unsigned long ino;
	//struct inode *inode = NULL;

	(void)nd; /* unused */

	printk("%s:%d,name='%s'\n", __func__, __LINE__,dentry->d_name.name);
	return simple_lookup(dir, dentry, nd);
	//if (dentry->d_name.len > NAME_MAX)
	//    return ERR_PTR(-ENAMETOOLONG);
	//dentry->d_op = &kfs_dir_dentry_operations;
	
	////ino = kfs_inode_by_name(dir, &dentry->d_name);
	//ino = 0;
	//if (!ino) {
	//    d_add(dentry, NULL);
	//    return NULL;
	//}
	//inode = kfs_iget(inode->i_sb, ino);
	//if (unlikely(IS_ERR(inode)))
	//    return ERR_PTR(-ENOENT);
	//return d_splice_alias(inode, dentry);
}

static int kfs_mknod(struct inode *dir, struct dentry *dentry, int mode, dev_t dev)
{
	struct inode * inode = kfs_new_inode(dir, mode);
	int error = -ENOSPC;

	printk("%s:%d\n", __func__, __LINE__);
	if (inode) {
		struct kfs_inode *kdir = (struct kfs_inode *) dir;
		Ctx *ctx = knh_getCurrentContext();
		if (dir->i_mode & S_ISGID) {
			inode->i_gid = dir->i_gid;
			if (S_ISDIR(mode))
				inode->i_mode |= S_ISGID;
		}
		d_instantiate(dentry, inode);
		dget(dentry); /* Extra count - pin the dentry in core */
		error = 0;
		dir->i_mtime = dir->i_ctime = CURRENT_TIME;
		knh_DictSet_t *o = kdir->entry_table;
		knh_bytes_t b = {dentry->d_name.name, dentry->d_name.len};
		knh_DictSet_append(ctx, o, new_String(ctx, b, NULL), (knh_uintptr_t)dentry);
	}
	return error;
}

static int kfs_mkdir(struct inode *dir, struct dentry *dentry, int mode)
{
	int err = -EMLINK;
	struct inode *inode = kfs_new_inode(dir, mode | S_IFDIR);

	err = PTR_ERR(inode);
	if (IS_ERR(inode))
		return err;
	printk("%s:%d\n", __func__, __LINE__);
	if (inode) {
		if (dir->i_mode & S_ISGID) {
			inode->i_gid = dir->i_gid;
			inode->i_mode |= S_ISGID;
		}
		d_instantiate(dentry, inode);
		dget(dentry);
		err = 0;
		dir->i_mtime = dir->i_ctime = CURRENT_TIME;
		inode->i_op = &kfs_dir_inode_operations;
		inode->i_fop = &simple_dir_operations;
	}

	if (!err)
		inc_nlink(dir);
	return err;
}

static int kfs_rmdir(struct inode * dir, struct dentry * dentry)
{
	//struct inode *inode = dentry->d_inode;
	int err = simple_rmdir(dir, dentry);
	printk("%s:%d\n", __func__, __LINE__);
	return err;
}

static int kfs_create(struct inode *dir, struct dentry *dentry, int mode, struct nameidata *nd)
{
	int err = -EMLINK;
	struct inode *inode = kfs_new_inode(dir, mode | S_IFREG);
	printk("%s:%d\n", __func__, __LINE__);
	err = PTR_ERR(inode);
	if (IS_ERR(inode))
		return err;
	if (inode) {
		if (dir->i_mode & S_ISGID) {
			inode->i_gid = dir->i_gid;
		}
		d_instantiate(dentry, inode);
		dget(dentry);
		inode->i_op  = &kfs_file_inode_operations;
		inode->i_fop = &kfs_file_operations;
		err = 0;
	}
	return err;
}

static int kfs_symlink(struct inode * dir, struct dentry *dentry, const char * symname)
{
	struct inode *inode;
	int error = -ENOSPC;
	printk("%s:%d\n", __func__, __LINE__);
	//inode = kfs_get_inode(dir->i_sb, S_IFLNK|S_IRWXUGO, 0);
	inode = kfs_new_inode(dir, S_IFLNK | S_IRWXUGO);
	inode->i_op = &page_symlink_inode_operations;
	if (inode) {
		int l = strlen(symname)+1;
		error = page_symlink(inode, symname, l);
		if (!error) {
			if (dir->i_mode & S_ISGID)
				inode->i_gid = dir->i_gid;
			d_instantiate(dentry, inode);
			dget(dentry);
			dir->i_mtime = dir->i_ctime = CURRENT_TIME;
		} else
			iput(inode);
	}
	return error;
}

const struct inode_operations kfs_dir_inode_operations = {
	.create  = kfs_create,
	.lookup  = kfs_dir_lookup,
	.link    = simple_link,
	.unlink  = simple_unlink,
	.symlink = kfs_symlink,
	.mkdir   = kfs_mkdir,
	.rmdir   = kfs_rmdir,
	.mknod   = kfs_mknod,
	.rename  = simple_rename,
};

