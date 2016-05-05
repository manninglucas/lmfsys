#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "inode.h"
#include "block.h"
#include "file.h"

void append_data_to_file(int inum, void *data, size_t size, FILE *disk)
{
    inode *in = inode_at_num(inum, disk);
    int new_blk_count = (size / BLOCK_SIZE) + 1 - in->blocks;
    int file_end_pos = (in->blocks * BLOCK_SIZE) - in->size;
    uint32_t eblock_addr = end_block_addr(in);
    if (new_blk_count > 0) {
        alloc_blocks(in, new_blk_count);
    }
    
   //need to actually write to file and handle writes larger 
   //than rest of current block
     
    free(in); 
}

void write_data_to_file(int inum, void *data, size_t size, FILE *disk)
{

}
//eventually make this recursive... somehow....
uint32_t end_block_addr(inode *in)
{
    if (in->blocks < 13) return in->data_block[in->blocks - 1];

    //indirect pointer
    if (in->blocks < 13 + MAX_INDIR_PTRS) {
        block *indr_block = block_at_addr(in->indr_ptr[0], in->disk);
        uint32_t addr = (uint32_t)indr_block->data[
            (in->blocks - 1 - 13)*sizeof(uint32_t)];
        free(indr_block);
        return addr;
    }  
    
    int blocknum = (in->blocks - 13 - 1 - MAX_INDIR_PTRS) / MAX_INDIR_PTRS;
    int blockpos = (in->blocks - 13 - 1 - MAX_INDIR_PTRS) % MAX_INDIR_PTRS;

    block *doub_indr_block = block_at_addr(in->indr_ptr[1], in->disk);
    uint32_t indr_blockpos = 
        (uint32_t)doub_indr_block->data[blocknum*sizeof(uint32_t)];

    block *indr_block = block_at_addr(indr_blockpos, in->disk);
    uint32_t addr = (uint32_t)indr_block->data[blockpos*sizeof(uint32_t)];

    free(indr_block);
    free(doub_indr_block);
    return addr;
}
