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

    for (u8 *addr = data; size != 0; addr++, size--) {
        write_byte_to_block(cur_blk, *addr, pos++);
        in->size += 1;
        if (pos / BLOCK_SIZE != 0) {
            pos = 0;
            alloc_blocks(in, 1);
            cur_blk = block_at_addr(in->end_block);
        }
    }
    
    free(in); 
    free(cur_blk);
}

void write_data_to_file(int inum, void *data, size_t size, int offset)
{
    resize_file(inum, offset);
    append_data_to_file(inum, data, size);
}

void read_file(int inum)
{
    inode *in = inode_at_num(inum);   
    block *b = block_at_addr(in->data_block[0]);
    int blocknum = 0;
    for (int bytepos = 0; bytepos != in->size; bytepos++) {
        if (bytepos % BLOCK_SIZE == 0 && bytepos != 0) {
            b = next_block_in_file(blocknum++, in);
        }
    }
}

//eventually make this recursive... somehow....
//This function is useless but I spent a lot of time on it. :(
u32 find_block_addr(inode *in, int block_offset)
{
    if (block_offset < 13) return in->data_block[block_offset];

    //indirect pointer
    if (block_offset < 13 + MAX_INDIR_PTRS) {
        block *indr_block = block_at_addr(in->indr_ptr[0]);
        u32 addr = (u32)indr_block->data[
            (block_offset - 13)*sizeof(u32)];
        free(indr_block);
        return addr;
    }
    
    int blocknum = (block_offset - 13 - MAX_INDIR_PTRS) / MAX_INDIR_PTRS;
    int blockpos = (block_offset - 13 - MAX_INDIR_PTRS) % MAX_INDIR_PTRS;

    block *doub_indr_block = block_at_addr(in->indr_ptr[1], sb->disk);
    u32 indr_blockpos = 
        (u32)doub_indr_block->data[blocknum*sizeof(u32)];

    block *indr_block = block_at_addr(indr_blockpos, sb->disk);
    u32 addr = (u32)indr_block->data[blockpos*sizeof(u32)];

    free(indr_block);
    free(doub_indr_block);
    return addr;
}

//this can only be used to shrink files
void resize_file(int inum, int new_size)
{
    inode *in = inode_at_num(inum);
    //remove 1 less block than we need to 
    int blks2remove = in->blocks - (new_size / BLOCK_SIZE + 1); 
    dealloc_blocks(in, blks2remove);
   
    //pos within the block 
    block *endblock = block_at_addr(in->end_block);
    int endpos = in->size - new_size;
    for (int i = in->size - 1 % BLOCK_SIZE ; i > endpos; ++i)
        endblock->data[i] = 0;
    in->size = new_size;
    
    write_block_to_disk(endblock);
    free(endblock);
} 
