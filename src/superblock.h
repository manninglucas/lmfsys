#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H

typedef struct {
    uint16_t id; //How the "OS" identifies the fs
    uint32_t inode_count;
    uint32_t block_count;
    uint32_t res_block_count;
    uint32_t block_size;
    uint32_t first_inode;
    uint32_t inode_size;
    uint32_t first_block;
    uint32_t inode_bmap;
    uint32_t block_bmap;
} superblock;

superblock *new_superblock(uint32_t disk_sz);

#endif
