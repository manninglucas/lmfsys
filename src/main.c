#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include "util.h"
#include "block.h"
#include "inode.h"
#include "bitmap.h"
#include "superblock.h"

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

void make_filesystem(char *disk_name, size_t disk_sz)
{
    //opening for w zeroes out the file first 
    FILE *disk = fopen(disk_name, "rb+");

    superblock *sb = new_superblock(disk_sz);
    
    write_disk((void *)sb, sizeof(superblock), disk, 0);

    free(sb);

    fclose(disk);
}

static void create_file(uint32_t size, const char *path, FILE *disk)
{
    //find an unallcated inode
    int in_block_loc = free_bm_addr(INODE, disk);

    //allocate the inode
    int block_count = (size / BLOCK_SIZE) + 1;
    inode *in = new_inode(in_block_loc, block_count, disk);

    //write the inode
    free(in);
}

int main(int argc, char *argv[])
{
    make_filesystem("disk", file_size("disk"));
    
    FILE *disk = fopen("disk", "rb+");
    create_file(5000, "Asd/fa/s", disk); 
    fclose(disk);

    read_sb("disk");
    
    return 0;
}
