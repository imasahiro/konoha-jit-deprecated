class Dentry {
    void dget();
    void instantiate(Inode inode);
}

class Inode {
    int mode;
    int uid;
    int gid;
    int atime;
    int mtime;
    int ctime;
}
Inode  Inode.new(Inode dir, Dentry entry, int mode);
Dentry Inode.lookup(Inode dir, String name);
int    Inode.mknod(Inode dir, Dentry entry, int mode);
int    Inode.link(Inode dir, Dentry entry, int mode);
int    Inode.unlink(Inode dir, Dentry entry, int mode);

class File extends Inode;
File File.new(Inode dir, Dentry entry, int mode);

int mknod(Inode dir, Dentry entry, int mode)
{
    try {
        inode = new Inode(dir, entry, mode);
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

