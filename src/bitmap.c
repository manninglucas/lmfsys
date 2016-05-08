#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "bitmap.h"

int free_bm_addr(BM_TYPE type)
{
    int free_bm_addr = -1;
    int offset = 0;
    
    bitmap *bm = malloc(sizeof(bitmap)); 
    bm->type = type;
    fseek(sb->disk, bm_start_addr(type), SEEK_SET);
    fread(bm->data, sizeof(bm->data), 1, sb->disk);

    while (free_bm_addr == -1) {
        u8 bm_section = bm->data[offset]; 

        int bit_pos = empty_bit_pos(bm_section);

        if (bit_pos == -1) {
            offset++;
         } else {
            free_bm_addr = bit_pos + (sizeof(u8)*offset);
            flip_bit(&bm_section, bit_pos);
            bm->data[offset] = bm_section;
         }
    }       
    write_disk((void *)bm->data, sizeof(bm->data), bm_start_addr(type));
    free(bm);
    return free_bm_addr;
}

int bm_start_addr(BM_TYPE type)
{
    if (type == INODE) return INODE_BM_START;
    else return DATA_BM_START;
}

void flip_bit(u8 *bitmap_section, int pos)
{
   (*bitmap_section) ^= (1 << pos); 
}

int empty_bit_pos(u8 bitmap_section)
{
    for (int i = 0; i < 8; i++) {
        if ((~bitmap_section >> i) & 1) return i;
    } 
    return -1;
}

void mark_empty_at_addr(u32 addr, BM_TYPE type)
{
    int free_bm_addr = -1;
    int offset = 0;
    
    bitmap *bm = malloc(sizeof(bitmap)); 
    bm->type = type;
    fseek(sb->disk, bm_start_addr(type), SEEK_SET);
    fread(bm->data, sizeof(bm->data), 1, sb->disk);
    
    flip_bit(&bm->data[addr / 8], addr % 8);
    write_disk((void *)bm->data, sizeof(bm->data), bm_start_addr(type));
    free(bm);
}
