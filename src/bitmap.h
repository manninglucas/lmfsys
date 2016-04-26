#ifndef BITMAP_H
#define BITMAP_H

typedef enum {
    INODE,
    DATA,
} BM_TYPE;


int free_bm_addr();
static int empty_bit_pos(uint32_t bitmap);
#endif
