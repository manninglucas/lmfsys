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
    uint32_t data_block[13];
    uint32_t indr_ptr[2];
    //need this for virtual purposes
    FILE *disk;
} inode;

inode* new_inode(int size, int type, FILE *disk);
void read_inode(int inode_num, FILE *disk);
inode *inode_at_num(int inum, FILE *disk);

void alloc_blocks(inode *in, int blks_needed);
void alloc_indr_blocks(inode *in, int blks_needed);
block* new_indr_block(inode *in, int blks_needed, uint32_t addr,
        int ptr_depth);
#endif
