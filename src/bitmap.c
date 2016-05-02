#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "bitmap.h"

int free_bm_addr(BM_TYPE type, FILE *disk)
{
    int free_bm_addr = -1;
    int offset = 0;
    
    bitmap *bm = malloc(sizeof(bitmap)); 
    bm->type = type;
    fseek(disk, bm_start_addr(type), SEEK_SET);
    fread(bm->data, sizeof(bm->data), 1, disk);

    while (free_bm_addr != -1) {
        uint8_t bm_section = bm->data[offset]; 

        int bit_pos = empty_bit_pos(bm_section);

        if (bit_pos == -1) {
            offset++;
         } else {
            free_bm_addr = bit_pos + (sizeof(uint8_t)*offset);
            fill_bit(&bm_section, bit_pos);
         }
    }        
    write_disk((void *)bm->data, sizeof(bm->data), disk, bm_start_addr(type));
    free(bm);
    return free_bm_addr;
}

int bm_start_addr(BM_TYPE type)
{
    if (type == INODE) return INODE_BM_START;
    else return DATA_BM_START;
}

void fill_bit(uint8_t *bitmap_section, int pos)
{
   (*bitmap_section) |= (1 << pos); 
}

int empty_bit_pos(uint8_t bitmap_section)
{
    for (int i = 0; i < sizeof(bitmap_section); i++) {
        if (~bitmap_section & 1) return i;
        bitmap_section >>= 1; 
    } 
    return -1;
}
