#ifndef BLOCK_H
#define BLOCK_H

typedef struct {
    uint32_t addr;
    uint8_t data[BLOCK_SIZE];
} block;

block *new_block(uint32_t addr);
void write_ptr_to_block(block *blk, uint32_t ptr, int offset);
void write_block_to_disk(block *blk);
block *block_at_addr(uint32_t addr);
void write_byte_to_block(block *blk, uint8_t byte, int offset)
#endif
