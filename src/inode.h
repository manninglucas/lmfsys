#ifndef INODE_H
#define INODE_H

#define MAX_PTR_DEPTH 2

typedef struct {
    int type;
    short uid;
    int num;
    int size;
    int time;
    int ctime;
    int mtime;
    int dtime;
    short gid;
    short links_count;
    int blocks;
    int flags;
    u32 end_block;
    u32 data_block[13];
    u32 indr_ptr[MAX_PTR_DEPTH];
} inode;

inode* new_inode(int size, int type);
void read_inode(int inode_num);
inode *inode_at_num(int inum);

void alloc_blocks(inode *in, int blks_needed);
void alloc_indr_blocks(inode *in, int blks_needed);
block* new_indr_block(inode *in, int blks_needed, u32 addr,
        int ptr_depth);

void dealloc_blocks(inode *in, int blks2remove);
void dealloc_indr_blocks(inode *in, int blks_needed);
void remove_indr_block(inode *in, int blks_needed, u32 addr,
        int ptr_depth);
#endif
