#ifndef BITMAP_H
#define BITMAP_H

#include "util.h"

#define INODE_BM_START BLOCK_SIZE * 1
#define DATA_BM_START BLOCK_SIZE * 2

typedef enum {
    INODE,
    DATA,
} BM_TYPE;

typedef struct {
    BM_TYPE type;
    u8 data[BLOCK_SIZE];
} bitmap;

int free_bm_addr(BM_TYPE type);
int empty_bit_pos(u8 bitmap_section);
void flip_bit(u8 *bitmap_section, int pos);
int bm_start_addr(BM_TYPE type);
void mark_empty_at_addr(u32 addr, BM_TYPE type);

#endif
