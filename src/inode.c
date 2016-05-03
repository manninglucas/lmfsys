#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "util.h"
#include "block.h"
#include "inode.h"
#include "bitmap.h"

//max size checks
inode *new_inode(int inode_num, int size, const char *name, FILE *disk)
{
    inode *in = malloc(sizeof(inode));
    in->blocks = (size / BLOCK_SIZE) + 1;
    in->uid = inode_num;
    in->ctime = time(NULL);
    in->mtime = time(NULL);
    in->dtime = time(NULL);
    in->disk = disk;
    in->size = size;
    strcpy(in->name, name);

    int blk_ptrs = 0;

    for (; blk_ptrs < 12 && blk_ptrs != in->blocks; ++blk_ptrs) {
        in->data_block[blk_ptrs] = free_bm_addr(DATA, disk);         
    }
    //this loop takes care of single indirect pointers
    if (blk_ptrs != in->blocks)
        alloc_indr_blocks(&blk_ptrs, in->blocks, in);

    return in;
}

void alloc_indr_blocks(int *blk_ptrs, int blks_needed, inode *in)
{
    for (int ptr_depth = 0;
            *blk_ptrs != blks_needed && ptr_depth != MAX_PTR_DEPTH; 
            ptr_depth++) {
        in->indr_ptr[ptr_depth] = free_bm_addr(DATA, in->disk);

        block *indr_blk = new_indr_block(blk_ptrs, blks_needed, 
                in->indr_ptr[ptr_depth], ptr_depth, in->disk);

        write_block(indr_blk, in->disk);
        free(indr_blk);
    }
}

block* new_indr_block(int *blk_ptrs, int blks_needed, uint32_t addr,
        int ptr_depth, FILE *disk)
{
    block *indr_blk = new_block(addr);

    for (int used_ptrs = 0; 
            used_ptrs*sizeof(uint32_t) < BLOCK_SIZE; used_ptrs++) {
        uint32_t ptr2data = free_bm_addr(DATA, disk); 
        int offset = used_ptrs*sizeof(used_ptrs);
        write_ptr_to_block(indr_blk, ptr2data, offset);

        if (ptr_depth > 0) {
            block* ptr_blk = new_indr_block(blk_ptrs, blks_needed, ptr2data, 
                    --ptr_depth, disk);
            write_block(ptr_blk, disk);
            free(ptr_blk);
        }
        if (++(*blk_ptrs) == blks_needed) break;
    }
    return indr_blk;
}

void read_inode(int inode_num, FILE *disk)
{
    fseek(disk, INODE_START_ADDR + sizeof(inode)*inode_num, SEEK_SET);
    inode* inode = malloc(sizeof(inode)); 
    fread(inode, sizeof(inode), 1, disk);
    printf("INODE #%i:\nName: %s\nindirect pointer: %i\n", inode->uid, inode->name, inode->data_block[0]);
    free(inode);
}
