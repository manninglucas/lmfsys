#ifndef UTIL_H
#define UTIL_H

#define BLOCK_SIZE 4096
#define DATA_START_ADDR BLOCK_SIZE * 8 
#define INODE_START_ADDR BLOCK_SIZE * 3
#define FILE_INODE 0
#define DIRECTORY_INODE 1
#define MAX_INDIR_PTRS BLOCK_SIZE / 4

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

extern superblock *sb;

void write_disk(const void *data, u32 size, u32 offset);

#endif
