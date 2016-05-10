#include "block.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "superblock.h"
#include "bitmap.h"

block *new_block(u32 addr)
{
    block *blk = malloc(sizeof(block));
    blk->addr = addr;

    return blk;
}

void write_data_to_block(block *blk, void *data, int size, int offset)
{
    int i = 0;
    for (u8 *byte = (u8*)&data; i < size; ++byte, ++i)
        blk->data[offset+i] = *byte;

    write_block_to_disk(blk);
}

void write_block_to_disk(block *blk)
{
    int block_offset = DATA_START_ADDR + (BLOCK_SIZE * blk->addr);
    write_disk((void *)blk->data, sizeof(blk->data), block_offset);
}

block *block_at_addr(u32 addr)
{
    fseek(sb->disk, DATA_START_ADDR + BLOCK_SIZE*addr, SEEK_SET);
    block* blk = new_block(addr);
    fread(blk->data, sizeof(blk->data), 1, sb->disk);
    return blk;
}

void erase_block(block *blk)
{
    for (int i = 0; i < BLOCK_SIZE; ++i)
       blk->data[i] = 0; 

    mark_empty_at_addr(blk->addr, DATA);
    
    write_block_to_disk(blk);
}

block *block_at_num(int ptrnum, inode *in)
{
    //first parts easy enough just grab the next data block
    if (ptrnum < 13) 
        return block_at_addr(in->data_block[ptrnum]);
    //subtract the blocks that arent in the block of pointers
    ptrnum -= 13;
    //maximum amount of pointers on a depth level. on the second level its 
    //MAX_INDIR_PTRS^2
    int max_ptrs = MAX_INDIR_PTRS;
    int depth;
    for (depth = 0; depth != MAX_PTR_DEPTH; depth++, max_ptrs*=MAX_INDIR_PTRS)
        if (ptrnum / max_ptrs == 0) break;
    
    //now that we have the depth we can loop through the pointer depth levels
    block *indr_blk = block_at_addr(in->indr_ptr[depth]);
    //the position within the block of pointers of the next data block
    if (depth)
        ptrnum -= pow(MAX_INDIR_PTRS, depth);
    for (int i = 0; i < depth; i++){
        int indr_ptrnum = ptrnum / pow(MAX_INDIR_PTRS, MAX_PTR_DEPTH - (i+1));
        indr_blk = block_at_addr(*(u32*)&indr_blk->data[indr_ptrnum*sizeof(u32)]);
    }
    int pos = ptrnum % MAX_INDIR_PTRS;
    u32 addr = *(u32*)&indr_blk->data[pos*sizeof(u32)];
    free(indr_blk);
    return block_at_addr(addr);
}
