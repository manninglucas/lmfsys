#ifndef BITMAP_H
#define BITMAP_H
/*
 * BITMAP.H
 *
 * PURPOSE: Data and functions to operate on the bitmap sections of
 * the filesystem. Bitmaps keep track of which blocks on the disk are free
 * and which ones are used. A 0 marks the number of a free section of data and 
 * a 1 marks the number of a used one
 *
 */
#include "util.h"

#define BM_START_ADDR(x)  ((x == BM_INODE) ? INODE_BM_ADDR : DATA_BM_ADDR)

enum bitmap_type {
    BM_INODE,
    BM_DATA
};

typedef struct {
    enum bitmap_type type;
    u8 data[BLOCK_SIZE];
} bitmap;

int empty_bit_in_map(enum bitmap_type type);
void flip_bit_in_map(u32 bitpos, enum bitmap_type type);

#endif
