#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include "block.h"
#include "inode.h"
#include "bitmap.h"
#include "superblock.h"

#define BLOCK_SIZE 4096

static unsigned file_size(const char *file_name)
{
    struct stat sb;
    
    if (stat(file_name, &sb) != 0) {
        fprintf(stderr, "'stat' failed for '%s': %s.\n", file_name,
               strerror(errno)); 
        exit(EXIT_FAILURE);
    }
    return sb.st_size;
}

static void write_disk(const void *data, uint32_t size, FILE *disk, uint32_t offset)
{ 
    fseek(disk, offset, SEEK_SET);
    fwrite(data, size, 1, disk);
}

void make_filesystem(char *disk_name, size_t disk_sz)
{
    //opening for w zeroes out the file first 
    FILE *disk = fopen(disk_name, "rb+");

    superblock *sb = new_superblock(disk_sz);
    
    write_disk((void *)sb, sizeof(superblock), disk, 0);

    free(sb);

    fclose(disk);
}

static void write_file(FILE *f, uint32_t size, const char *path)
{
    //loop through inode bitmap
    

    //write to inode bitmap

    //allocate inodes
    int block_count = (size / BLOCK_SIZE) + 1;
    int inode_count = (block_count / 15) + 1;

    //allocate blocks
    //
    //write bitmaps
    //
    //write blocks  
    //
}

static void create_file(FILE *f, uint32_t size, const char *path)
{
    //find an unallcated inode
    int in_block_loc = free_bm_addr();

    //allocate the inode
    int block_count = (size / BLOCK_SIZE) + 1;
    inode *in = new_inode(in_block_loc, block_count);

    //write the inode
}

static void read_file(const void *filename)
{

}

int main(int argc, char *argv[])
{
    make_filesystem("disk", file_size("disk"));

    read_sb("disk");
    
    return 0;
}
