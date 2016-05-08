#ifndef BLOCK_H
#define BLOCK_H

typedef struct {
    u32 addr;
    u8 data[BLOCK_SIZE];
} block;

block *new_block(u32 addr);
void write_ptr_to_block(block *blk, u32 ptr, int offset);
void write_block_to_disk(block *blk);
block *block_at_addr(u32 addr);
void write_byte_to_block(block *blk, u8 byte, int offset);

void erase_block(block *blk);
#endif
