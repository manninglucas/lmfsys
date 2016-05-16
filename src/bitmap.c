#include "bitmap.h"
#include "superblock.h"
#include <stdio.h>
#include <stdlib.h>

/* NAME: EMPTY BIT IN MAP
 * PURPOSE: Find the first 0 in the bitmap and return it's position
 * PARAMS:
 *      bitmap_type type: type of bitmap -- inode or data. 
 * RETURN:
 *      position in the empty bit in the bitmap. 
 *      If no empty bit is found, the funcion returns -1.
 */
int empty_bit_in_map(enum bitmap_type type)
{
    bitmap *bm = (bitmap *) malloc(sizeof(bitmap)); 
    bm->type = type;
    fseek(sb->disk, BM_START_ADDR(type), SEEK_SET);
    fread(bm->data, sizeof(u8), sizeof(bm->data), sb->disk);

    for (int bytepos = 0; bytepos != BLOCK_SIZE; bytepos++) {
        u8 byte = bm->data[bytepos]; 
        //if the byte is completely full we can just skip it
        if (byte == 0xFF) continue;
        int bitpos;
        //this operation first masks the byte with the bit at pos 0-7
        //then shifts the desired bit back to the 1 position. If it's
        //0 we want that pos.
        for (bitpos = 0; bitpos < 8; bitpos++)
            if (!((byte & (1 << bitpos)) >> bitpos)) break;
        free(bm);
        return bytepos * 8 + bitpos;
    }
    free(bm);
    return -1;
}

/* NAME: FLIP BIT IN MAP
 * PURPOSE: flip a bit within a bitmap at selected pos.
 * PARAMS:
 *      bitmap_type type: type of bitmap -- inode or data. 
 *      u32 bitpos: position of bit within bitmap
 */
void flip_bit_in_map(u32 bitpos, enum bitmap_type type)
{
    bitmap *bm = malloc(sizeof(bitmap)); 
    bm->type = type;
    fseek(sb->disk, BM_START_ADDR(type), SEEK_SET);
    fread(bm->data, sizeof(u8), sizeof(bm->data), sb->disk);
    
    (bm->data[bitpos / 8]) ^= 1 << (bitpos % 8);
    write_disk(bm->data, sizeof(bm->data), BM_START_ADDR(type));
    free(bm);
}

