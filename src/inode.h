#ifndef INODE_H
#define INODE_H
/*
 * INODE.H
 * 
 * PURPOSE: Inodes are structures stored in a special partition of the filesystem.
 * They are used to store information about a particular file like it's size and the
 * last time the file was allocated. Functions in this file deal with the CRUD of
 * inodes as well as the the manipulation of the inode table as a whole.
 */
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

//CREATE
void alloc_blocks(inode *in, u32 blks_needed);
void alloc_indr_blocks(inode *in, u32 blks_needed);
block *new_indr_block(inode *in, u32 blks_needed, u32 addr, u32 ptr_depth);
inode *new_inode(u32 size, enum inode_type type);

//READ
inode *inode_at_num(u32 num);

//UPDATE
void write_inode_to_disk(inode *in);

//DELETE
void dealloc_blocks(inode *in, u32 blks2remove);
void dealloc_indr_blocks(inode *in, u32 blks_needed);
void remove_indr_block(inode *in, u32 blks_needed, u32 addr, u32 ptr_depth);

#endif
