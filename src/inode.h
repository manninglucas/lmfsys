#ifndef INODE_H
#define INODE_H

#include "util.h"

typedef struct  {
    int type;
    short uid;
    int num;
    int size;
    int time;
    int ctime;
    int mtime;
    int dtime;
    short gid;
    int blocks;
    u32 end_block;
    u32 data_block[13];
    u32 indr_ptr[MAX_PTR_DEPTH];
}__attribute__((__packed__)) inode;

#include "block.h"

inode *new_inode(int size, enum inode_type type);
void read_inode(int inode_num);
inode *inode_at_num(int inum);

void alloc_blocks(inode *in, int blks_needed);
void alloc_indr_blocks(inode *in, int blks_needed);
block *new_indr_block(inode *in, int blks_needed, u32 addr,
        int ptr_depth);

void dealloc_blocks(inode *in, int blks2remove);
void dealloc_indr_blocks(inode *in, int blks_needed);
void remove_indr_block(inode *in, int blks_needed, u32 addr,
        int ptr_depth);

void write_inode_to_disk(inode *in);
#endif
