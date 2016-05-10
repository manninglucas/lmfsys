#ifndef BLOCK_H
#define BLOCK_H

#include "util.h"

typedef struct {
    u32 addr;
    u8 data[BLOCK_SIZE];
} block;

#include "inode.h"

block *new_block(u32 addr);
void write_block_to_disk(block *blk);
block *block_at_addr(u32 addr);
void write_data_to_block(block *blk, void *data, int size, int offset);

void erase_block(block *blk);
block *block_at_num(int ptrnum, inode *in);
#endif
