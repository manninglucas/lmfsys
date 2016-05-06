#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "inode.h"
#include "block.h"
#include "file.h"

void append_data_to_file(int inum, void *data, size_t size)
{
    inode *in = inode_at_num(inum, sb->disk);

    block *cur_blk = block_at_addr(in->end_block); 
    int pos = in->size % BLOCK_SIZE;

    for (uint8_t *addr = data; size != 0; addr++, size--) {
        write_byte_to_block(cur_blk, *addr, pos++);
        if (pos / BLOCK_SIZE != 0) {
            pos = 0;
            alloc_blocks(in, 1);
            cur_blk = block_at_addr(in->end_block);
        }
    }
    in->size += size;
    
    free(in); 
}

void write_data_to_file(int inum, void *data, size_t size)
{

}

//eventually make this recursive... somehow....
//This function is useless but I spent a lot of time on it. :(
uint32_t end_block_addr(inode *in)
{
    if (in->blocks < 13) return in->data_block[in->blocks - 1];

    //indirect pointer
    if (in->blocks < 13 + MAX_INDIR_PTRS) {
        block *indr_block = block_at_addr(in->indr_ptr[0], sb->disk);
        uint32_t addr = (uint32_t)indr_block->data[
            (in->blocks - 1 - 13)*sizeof(uint32_t)];
        free(indr_block);
        return addr;
    }  
    
    int blocknum = (in->blocks - 13 - 1 - MAX_INDIR_PTRS) / MAX_INDIR_PTRS;
    int blockpos = (in->blocks - 13 - 1 - MAX_INDIR_PTRS) % MAX_INDIR_PTRS;

    block *doub_indr_block = block_at_addr(in->indr_ptr[1], sb->disk);
    uint32_t indr_blockpos = 
        (uint32_t)doub_indr_block->data[blocknum*sizeof(uint32_t)];

    block *indr_block = block_at_addr(indr_blockpos, sb->disk);
    uint32_t addr = (uint32_t)indr_block->data[blockpos*sizeof(uint32_t)];

    free(indr_block);
    free(doub_indr_block);
    return addr;
}
