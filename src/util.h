#ifndef UTIL_H
#define UTIL_H

#define BLOCK_SIZE 4096
#define DATA_START_ADDR BLOCK_SIZE * 8 
#define INODE_START_ADDR BLOCK_SIZE * 3
#define FILE_INODE 0
#define DIRECTORY_INODE 1
#define MAX_INDIR_PTRS BLOCK_SIZE / 4

extern FILE *disk;

void write_disk(const void *data, uint32_t size, FILE *disk, uint32_t offset);

#endif
