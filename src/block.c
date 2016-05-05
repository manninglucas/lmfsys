#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "block.h"

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

void write_block(block *blk, FILE *disk)
{
    int block_offset = DATA_START_ADDR + (BLOCK_SIZE * blk->addr);
    write_disk((void *)blk->data, sizeof(blk->data), disk, block_offset);
}

block *block_at_addr(uint32_t addr, FILE *disk)
{
    fseek(disk, DATA_START_ADDR + BLOCK_SIZE*addr, SEEK_SET);
    block* blk = new_block(addr);
    fread(blk->data, sizeof(blk->data), 1, disk);
    return blk;
}
