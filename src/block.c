#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "block.h"
#include "bitmap.h"

block *new_block(uint32_t addr)
{
    block *blk = malloc(sizeof(block));
    blk->addr = addr;

    return blk;
}

void write_ptr_to_block(block *blk, uint32_t ptr, int offset)
{ 
    int i = 0;
    for (uint8_t *byte = &ptr; i < sizeof(uint32_t); ++byte, ++i)
        blk->data[offset+i] = *byte;
}

void write_byte_to_block(block *blk, uint8_t byte, int offset)
{

}

void write_block_to_disk(block *blk)
{
    int block_offset = DATA_START_ADDR + (BLOCK_SIZE * blk->addr);
    write_disk((void *)blk->data, sizeof(blk->data), sb->disk, block_offset);
}

block *block_at_addr(uint32_t addr)
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
