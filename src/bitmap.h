#ifndef BITMAP_H
#define BITMAP_H

typedef enum {
    INODE,
    DATA,
} BM_TYPE;

typedef struct {
    BM_TYPE type;
    uint8_t data[BLOCK_SIZE];
} bitmap;

#define INODE_BM_START BLOCK_SIZE * 1
#define DATA_BM_START BLOCK_SIZE * 2

int free_bm_addr(BM_TYPE type, FILE *disk);
int empty_bit_pos(uint8_t bitmap_section);
void fill_bit(uint8_t *bitmap_section, int pos);
int bm_start_addr(BM_TYPE type);

#endif
