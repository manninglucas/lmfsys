#include "inode.h"
#include "superblock.h"
#include "bitmap.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>

//max size checks
inode *new_inode(int size, enum inode_type type)
{
    printf("new inode\n");
    inode *in = malloc(sizeof(inode));
    in->num = empty_bit_in_map(BM_INODE);
    flip_bit_in_map(in->num, BM_INODE);
    in->ctime = time(NULL);
    in->mtime = time(NULL);
    in->dtime = time(NULL);
    in->type = type;
    in->blocks = 0;
    
    alloc_blocks(in, (size / BLOCK_SIZE) + 1);
    in->size = size;
    in->blocks = (size / BLOCK_SIZE) + 1;

    return in;
}

void alloc_blocks(inode *in, int numblks)
{
    printf("alloc blocks\n");
    int blks_needed = numblks + in->blocks;
    while (in->blocks <= 13 && in->blocks != blks_needed) {
        in->data_block[in->blocks] = empty_bit_in_map(BM_DATA); 
        flip_bit_in_map(in->data_block[in->blocks], BM_DATA);
        in->end_block = in->data_block[in->blocks];
        ++(in->blocks);
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
        in->indr_ptr[ptr_depth] = empty_bit_in_map(BM_DATA);
        flip_bit_in_map(in->indr_ptr[ptr_depth], BM_DATA);

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
        u32 ptr2data = empty_bit_in_map(BM_DATA); 
        flip_bit_in_map(ptr2data, BM_DATA);
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
        flip_bit_in_map(in->data_block[in->blocks], BM_DATA);
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
            flip_bit_in_map(in->indr_ptr[ptr_depth], BM_DATA);
            in->indr_ptr[ptr_depth] = 0;
        }
    }
}

void remove_indr_block(inode *in, int blks_needed, u32 addr,
        int ptr_depth)
{
    block *indr_blk = block_from_num(addr);
    int end_ptr_pos = 
        (in->blocks / (int)pow((double)MAX_INDIR_PTRS,(double)ptr_depth))
        % MAX_INDIR_PTRS;

    while (end_ptr_pos > 0) {
        u32 *ptr = (u32*)&indr_blk->data[end_ptr_pos];
       
        if (ptr_depth > 0)
            remove_indr_block(in, blks_needed, *ptr, --ptr_depth);
                
        block *data = block_from_num(*ptr);
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
    fseek(sb->disk, INODE_TABLE_ADDR + sizeof(inode)*inum, SEEK_SET);
    inode* in = malloc(sizeof(inode)); 
    fread(in, sizeof(inode), 1, sb->disk);
    return in;
}

void read_inode(int inode_num)
{
    inode* inode = inode_at_num(inode_num);
    printf("INODE #%i\nindirect pointer: %i\n", inode->num, inode->data_block[0]);
    free(inode);
}

void write_inode_to_disk(inode *in)
{
    printf("write inode to disk\n");
    int offset = INODE_TABLE_ADDR + (in->num * sizeof(inode));
    write_disk(in, sizeof(inode), offset);
}
