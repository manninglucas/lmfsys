#ifndef INODE_H
#define INODE_H

#define MAX_PTR_DEPTH 2

typedef struct {
    short mode;
    short uid;
    int size;
    int time;
    int ctime;
    int mtime;
    int dtime;
    short gid;
    short links_count;
    int blocks;
    int flags;
    int osd1;
    uint32_t data_block[13];
    uint32_t indr_ptr[2];
    int generation;
    //need this for virtual purposes
    FILE *disk;
} inode;

inode* new_inode(int inode_num, int block_num, FILE *disk);
block* new_indr_block(int *blk_ptrs, int blks_needed, uint32_t addr,
        int ptr_depth, FILE *disk);
void alloc_indr_blocks(int *blk_ptrs, int blks_needed, inode *in);

#endif
