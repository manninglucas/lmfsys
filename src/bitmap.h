#ifndef BITMAP_H
#define BITMAP_H

#define INODE_BM_START BLOCK_SIZE * 1
#define DATA_BM_START BLOCK_SIZE * 2

typedef enum {
    INODE,
    DATA,
} BM_TYPE;

typedef struct {
    BM_TYPE type;
    uint8_t data[BLOCK_SIZE];
} bitmap;

int free_bm_addr(BM_TYPE type);
int empty_bit_pos(uint8_t bitmap_section);
void flip_bit(uint8_t *bitmap_section, int pos);
int bm_start_addr(BM_TYPE type);
void mark_empty_at_addr(u32 addr, BM_TYPE type);

#endif
