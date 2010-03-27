/* internal.h: ramfs internal definitions
 *
 * Copyright (C) 2005 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */


#include <konoha.h>
extern const struct inode_operations kfs_dir_inode_operations;
extern const struct file_operations kfs_file_operations;
extern const struct inode_operations kfs_file_inode_operations;
extern konoha_t konoha;

struct kfs_inode {
    struct inode inode;
    ino_t ino;
    void *data;
    knh_DictSet_t *entry_table;
//    struct timer_list timer;
//    wait_queue_head_t wait;
//    atomic_t flagged;
};


struct inode *kfs_new_inode(struct inode *dir, int ino);
struct inode *kfs_iget(struct super_block *sb, ino_t ino);
