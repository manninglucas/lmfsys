#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H

typedef struct {
    uint16 id; //How the "OS" identifies the fs
    uint32 inode_count;
    uint32 block_count;
    uint32 res_block_count;
    uint32 block_size;
    uint32 first_inode;
    uint32 inode_size;
    uint32 first_block;
    uint32 inode_bmap;
    uint32 block_bmap;
} superblock;

superblock *new_superblock(uint32 disk_sz);

#endif
