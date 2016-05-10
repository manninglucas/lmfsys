#include "inode.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "superblock.h"
#include "bitmap.h"

//max size checks
inode *new_inode(int size, int type)
{
    inode *in = malloc(sizeof(inode));
    in->num = free_bm_addr(INODE);
    in->ctime = time(NULL);
    in->mtime = time(NULL);
    in->dtime = time(NULL);
    in->type = type;
    
    alloc_blocks(in, (size / BLOCK_SIZE) + 1);
    in->size = size;
    in->blocks = (size / BLOCK_SIZE) + 1;

    return in;
}

void alloc_blocks(inode *in, int numblks)
{
    int blks_needed = numblks + in->blocks;
    for (; in->blocks <= 13 && in->blocks != blks_needed; ++(in->blocks)) {
        in->data_block[in->blocks] = free_bm_addr(DATA); 
    }
    //this loop takes care of single indirect pointers
    if (in->blocks != blks_needed)
        alloc_indr_blocks(in, blks_needed);
    else
        in->end_block = in->data_block[in->blocks];
}

void alloc_indr_blocks(inode *in, int blks_needed)
{
    for (int ptr_depth = 0;
            in->blocks != blks_needed && ptr_depth != MAX_PTR_DEPTH; 
            ptr_depth++) {
        in->indr_ptr[ptr_depth] = free_bm_addr(DATA);

        block *indr_blk = new_indr_block(in, blks_needed, 
                in->indr_ptr[ptr_depth], ptr_depth);

        write_block_to_disk(indr_blk);
        free(indr_blk);
    }
}

block *new_indr_block(inode *in, int blks_needed, u32 addr,
        int ptr_depth)
{
    block *indr_blk = new_block(addr);

    for (int used_ptrs = 0; 
            used_ptrs*sizeof(u32) < BLOCK_SIZE; used_ptrs++) {
        u32 ptr2data = free_bm_addr(DATA); 
        int offset = used_ptrs*sizeof(used_ptrs);
        write_data_to_block(indr_blk, &ptr2data, sizeof(u32), offset);

        if (ptr_depth > 0) {
            block* ptr_blk = new_indr_block(in, blks_needed, ptr2data, 
                    --ptr_depth);
            write_block_to_disk(ptr_blk);
            free(ptr_blk);
        }
        if (++(in->blocks) == blks_needed) {
            in->end_block = ptr2data;
            break;
        }
    }
    return indr_blk;
}

void dealloc_blocks(inode *in, int blks2remove)
{
    int blks_needed = in->blocks - blks2remove;

    if (in->blocks > 13)
        dealloc_indr_blocks(in, blks_needed);

    for (; in->blocks <= 13 && in->blocks != blks_needed; --(in->blocks)) {
        mark_empty_at_addr(in->data_block[in->blocks], DATA);
        in->data_block[in->blocks] = 0;
    }
}

void dealloc_indr_blocks(inode *in, int blks_needed)
{
    for (int ptr_depth = 0; 
            ptr_depth != MAX_PTR_DEPTH && blks_needed != in->blocks;
            ptr_depth++) {

        remove_indr_block(in, blks_needed, in->indr_ptr[ptr_depth], ptr_depth);
        
        if (blks_needed != in->blocks) {
            mark_empty_at_addr(in->indr_ptr[ptr_depth], DATA);
            in->indr_ptr[ptr_depth] = 0;
        }
    }
}

void remove_indr_block(inode *in, int blks_needed, u32 addr,
        int ptr_depth)
{
    block *indr_blk = block_at_addr(addr);
    int end_ptr_pos = 
        (in->blocks / (int)pow((double)MAX_INDIR_PTRS,(double)ptr_depth))
        % MAX_INDIR_PTRS;

    while (end_ptr_pos > 0) {
        u32 *ptr = (u32*)&indr_blk->data[end_ptr_pos];
       
        if (ptr_depth > 0)
            remove_indr_block(in, blks_needed, *ptr, --ptr_depth);
                
        block *data = block_at_addr(*ptr);
        erase_block(data);
        free(data);

        end_ptr_pos--;
        if(--(in->blocks) != blks_needed) {
            in->end_block = *((u32*)&indr_blk->data[end_ptr_pos]); 
            break;
        }
    }
    free(indr_blk);
}

inode *inode_at_num(int inum)
{
    fseek(sb->disk, INODE_START_ADDR + sizeof(inode)*inum, SEEK_SET);
    inode* inode = malloc(sizeof(inode)); 
    fread(inode, sizeof(inode), 1, sb->disk);
    return inode;
}

void read_inode(int inode_num)
{
    inode* inode = inode_at_num(inode_num);
    printf("INODE #%i\nindirect pointer: %i\n", inode->num, inode->data_block[0]);
    free(inode);
}
