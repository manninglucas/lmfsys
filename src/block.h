#ifndef BLOCK_H
#define BLOCK_H
/*
 * BLOCK.H
 *
 * PURPOSE: Data structures and functions for the CRUD of blocks within the 
 * filesystem. Blocks are 4k sections that help to organize the filesysten
 * into manageable chunks.
 *
 */
#include "util.h"

typedef struct {
    u32 num;
    u8 data[BLOCK_SIZE];
} block;

#include "inode.h"

//CREATE
block *new_block(u32 addr);
void write_block_to_disk(block *blk);

//READ
block *block_from_num(u32 num);
block *block_from_index(u32 index, inode *in);

//UPDATE
void write_data_to_block(block *blk, void *data, u32 size, u32 offset);

//DELETE
void erase_block(block *blk);
#endif
