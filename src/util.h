#ifndef UTIL_H
#define UTIL_H
/*
 * UTIL
 *
 * PURPOSE: miscellaneous constants and functions for general usage across
 * all files
 *
 */
#include <stdio.h>

#define BLOCK_SIZE          0x1000
#define SUPERBLOCK_ADDR     0
#define INODE_BM_ADDR       BLOCK_SIZE * 1
#define DATA_BM_ADDR        BLOCK_SIZE * 2
#define INODE_TABLE_ADDR    BLOCK_SIZE * 3
#define DATA_START_ADDR     BLOCK_SIZE * 8 
#define MAX_INDIR_PTRS      BLOCK_SIZE / 4
#define MAX_PTR_DEPTH       2
#define DIR_SIZE            BLOCK_SIZE / 8

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

enum inode_type {
    FILE_INODE,
    DIR_INODE
};

unsigned file_size(const char *file_name);
void write_disk(const void *data, u32 size, u32 offset);
void make_filesystem(u32 size, FILE *disk);

#endif
