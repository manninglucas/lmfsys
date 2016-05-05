#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "util.h"
#include "block.h"
#include "inode.h"
#include "bitmap.h"

//max size checks
inode *new_inode(int size, int type, FILE *disk)
{
    inode *in = malloc(sizeof(inode));
    in->blocks = 0;
    in->num  = free_bm_addr(INODE, disk);
    in->ctime = time(NULL);
    in->mtime = time(NULL);
    in->dtime = time(NULL);
    in->disk = disk;
    in->size = 0;
    in->type = type;

    alloc_blocks(in, (size / BLOCK_SIZE) + 1);

    return in;
}

void alloc_blocks(inode *in, int blks_needed)
{
    for (; in->blocks < 13 && in->blocks != blks_needed; ++(in->blocks)) {
        in->data_block[in->blocks] = free_bm_addr(DATA, in->disk);         
    }
    //this loop takes care of single indirect pointers
    if (in->blocks != blks_needed)
        alloc_indr_blocks(in, blks_needed);
}

void alloc_indr_blocks(inode *in, int blks_needed)
{
    for (int ptr_depth = 0;
            in->blocks != blks_needed && ptr_depth != MAX_PTR_DEPTH; 
            ptr_depth++) {
        in->indr_ptr[ptr_depth] = free_bm_addr(DATA, in->disk);

        block *indr_blk = new_indr_block(in, blks_needed, 
                in->indr_ptr[ptr_depth], ptr_depth);

        write_block(indr_blk, in->disk);
        free(indr_blk);
    }
}

block* new_indr_block(inode *in, int blks_needed, uint32_t addr,
        int ptr_depth)
{
    block *indr_blk = new_block(addr);

    for (int used_ptrs = 0; 
            used_ptrs*sizeof(uint32_t) < BLOCK_SIZE; used_ptrs++) {
        uint32_t ptr2data = free_bm_addr(DATA, in->disk); 
        int offset = used_ptrs*sizeof(used_ptrs);
        write_ptr_to_block(indr_blk, ptr2data, offset);

        if (ptr_depth > 0) {
            block* ptr_blk = new_indr_block(in, blks_needed, ptr2data, 
                    --ptr_depth);
            write_block(ptr_blk, in->disk);
            free(ptr_blk);
        }
        if (++(in->blocks) == blks_needed) break;
    }
    return indr_blk;
}

inode *inode_at_num(int inum, FILE *disk)
{
    fseek(disk, INODE_START_ADDR + sizeof(inode)*inum, SEEK_SET);
    inode* inode = malloc(sizeof(inode)); 
    fread(inode, sizeof(inode), 1, disk);
    return inode;
}

void read_inode(int inode_num, FILE *disk)
{
    inode* inode = inode_at_num(inode_num, disk);
    printf("INODE #%i\nindirect pointer: %i\n", inode->num, inode->data_block[0]);
    free(inode);
}
