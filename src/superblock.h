#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H

#include <stdio.h>
#include "util.h"

typedef struct {
    u8 id; //How the "OS" identifies the fs
    char name[8];
    u32 inode_count;
    u32 block_count;
    u32 res_block_count;
    u32 block_size;
    u32 first_inode;
    u32 inode_size;
    u32 first_block;
    u32 inode_bmap;
    u32 block_bmap;
    u32 root_inum;
    FILE *disk;
} superblock;

extern superblock *sb;

superblock *new_superblock(u32 disk_sz);

void read_sb(const char *filename);
superblock *filesys_sb();
FILE *filesys_disk();

#endif
