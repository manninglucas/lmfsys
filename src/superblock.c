#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "superblock.h"

superblock *new_superblock(uint32_t disk_sz)
{
    superblock *sb = malloc(sizeof(superblock));

    if (!sb || disk_sz < 0x4000)
        goto error;

    sb->id = 1;

    strcpy(sb->name, "Lucas");
    
    //sizes stay constant for now
    sb->block_size = 0x1000;
    sb->inode_size = 0x80;

    //find addresses based on arbitrary disk size
    sb->block_count = disk_sz / sb->block_size;

    //1/8 of the disk is for inodes and bitmaps
    sb->res_block_count = sb->block_count / 8;

    //both bitmaps only take up 1 block. Should tweak to be more efficient.
    sb->inode_bmap = sb->block_size;
    sb->block_bmap = sb->block_size * 2;

    sb->first_inode = sb->block_size * 3; 

    sb->first_block = sb->res_block_count * sb->block_size;
    
    return sb;

error:
    //some failure stuff
    fprintf(stderr, "Could not initialize superblock: %s", strerror(errno));
    exit(EXIT_FAILURE);
}


void read_sb(const char *filename)
{
    FILE *disk = fopen("disk", "rb");
    superblock *sb = malloc(sizeof(superblock));
    fread(sb, sizeof(superblock), 1, disk);        
    printf("Name: %s \nBlock count: %i", sb->name, sb->block_count);
    free(sb);
    fclose(disk);
}
