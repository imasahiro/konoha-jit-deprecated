class Dir extends Inode;
int mkdir(Inode dir, Dentry entry, int mode);
int rmdir(Inode dir, Dentry entry);

int mkdir(Inode dir, Dentry entry, int mode)
{
    try {
        inode = new Inode(dir, mode);
        if (dir.mode & S_ISGID) {
            inode.gid = dir.gid;
            if (S_ISDIR(mode)) {
                inode.mode = inode.mode | S_ISGID;
            }
        }
        entry.instantiate(inode);
        entry.get();
        dir.mtime = dir.ctime = CURRENT_TIME;
        return 0;
    } catch(Exception e) {
        return -1;
    }
}


