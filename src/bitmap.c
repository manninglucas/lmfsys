#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"

int free_bm_addr(BM_TYPE bitmap)
{
    int free_bm_addr = -1;
    if (bitmap == INODE) {
        FILE *disk = fopen("disk", "rb+");  
        
        int uint32_offset = 0;
        while (free_bm_addr != -1) {
            uint32_t *bm_section; 
            fseek(disk, sizeof(uint32_t), SEEK_CUR);
            fread(bm_section, sizeof(uint32_t), 1, disk);

            int bit_pos = empty_bit_pos(*bm_section);
            if (bit_pos == -1)
                uint32_offset++;
            else
                return bit_pos + (32*uint32_offset);
        }
            
    } else {
    }

    return -1;
}

static int empty_bit_pos(uint32_t bitmap)
{
    for (int i = 0; i < 32; i++) {
        if (~bitmap & 1) return i;
        bitmap >>= 1; 
    } 
    return -1;
}
