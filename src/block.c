#include "block.h"
#include "superblock.h"
#include "bitmap.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

/* NAME: NEW BLOCK
 * PURPOSE: Allocate and create a block data structure that is easier
 * to use and pass around than a raw byte array
 * PARAMS: 
 *      u32 num: the block number we want this block to have. Used to 
 *      find the block.
 * RETURN:
 *      pointer to the block structure used to pass around and organize 
 *      block daata
 */
block *new_block(u32 num)
{
    block *blk = malloc(sizeof(block));
    blk->num = num;

    return blk;
}

/* NAME: WRITE BLOCK TO DISK
 * PURPOSE: Write data in a block struct to the disk at address specified by
 * the block's number
 * PARAMS: 
 *      block *blk: the block struct with the data to write and the block
 *      number to write to.
 */
void write_block_to_disk(block *blk)
{
    int block_offset = DATA_START_ADDR + (BLOCK_SIZE * blk->num);
    write_disk(blk->data, BLOCK_SIZE, block_offset);
}

/* NAME: BLOCK FROM NUM
 * PURPOSE: Retrieve block data from disk at block at number num 
 * and load it into a usable block struct. 
 * PARAMS:
 *      u32 num: number of block that data will be read from.
 * RETURN:
 *      block *blk: pointer to block struct created that is flled with the
 *      blocks data from the disk
 */
block *block_from_num(u32 num)
{
    fseek(sb->disk, DATA_START_ADDR + BLOCK_SIZE*num, SEEK_SET);
    block* blk = new_block(num);
    fread(blk->data, sizeof(u8), BLOCK_SIZE, sb->disk);
    return blk;
}

/* NAME: BLOCK FROM INDEX
 * PURPOSE: Retrieve and load block data from the index of a block within
 * an inode. This function can get the 5th or 100th block allocated to an
 * inode for example.
 * PARAMS;
 *      u32 index: index of the block within the inode, whether it be the 5th
 *      or 100th
 *      inode *in: inode that the block is allocated to.
 * RETURN:
 *      block *blk: pointer to block struct created that is flled with the
 *      blocks data from the disk
 */
block *block_from_index(u32 index, inode *in)
{
    //first parts easy enough just grab the next data block
    if (index < 13) 
        return block_from_num(in->data_block[index]);
    //subtract the blocks that arent in the block of pointers
    index -= 13;
    //maximum amount of pointers on a depth level. on the second level its 
    //MAX_INDIR_PTRS^2
    int max_ptrs = MAX_INDIR_PTRS;
    int depth;
    for (depth = 0; depth != MAX_PTR_DEPTH; depth++, max_ptrs*=MAX_INDIR_PTRS)
        if (index / max_ptrs == 0) break;
    
    //now that we have the depth we can loop through the pointer depth levels
    block *indr_blk = block_from_num(in->indr_ptr[depth]);
    //the position within the block of pointers of the next data block
    if (depth)
        index -= pow(MAX_INDIR_PTRS, depth);
    for (int i = 0; i < depth; i++){
        int indr_ptrnum = index / pow(MAX_INDIR_PTRS, MAX_PTR_DEPTH - (i+1));
        indr_blk = block_from_num(*(u32*)&indr_blk->data[indr_ptrnum*sizeof(u32)]);
    }
    int pos = index % MAX_INDIR_PTRS;
    u32 addr = *(u32*)&indr_blk->data[pos*sizeof(u32)];
    free(indr_blk);
    return block_from_num(addr);
}

/* NAME: WRITE DATA TO BLOCK
 * PURPOSE: Take in data and write it to the byte array 
 * within the block struct at the specified offset
 * PARAMS:
 *      block *blk: block to write the data to.
 *      void *data: data to write to block. Must not be bigger than block size
 *      u32 size: sizeof data to write to block.
 *      u32 offset: position within the byte array to write the data to.
 */
void write_data_to_block(block *blk, void *data, u32 size, u32 offset)
{
    u8 *bytes = (u8 *) data;
    for (int i = 0; i < size; i++)
        blk->data[offset+i] = *(bytes+i); 
}

/* NAME: ERASE BLOCK
 * PURPOSE: Erase all data in block, write it to the disk,
 * and free its bitmap number.
 * PARAMS:
 *      block *blk: block to erase.
 */
void erase_block(block *blk)
{
    for (int i = 0; i < BLOCK_SIZE; ++i)
       blk->data[i] = 0; 

    flip_bit_in_map(blk->num, BM_DATA);
    
    write_block_to_disk(blk);
}
