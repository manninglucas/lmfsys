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

void make_filesystem(FILE *disk, size_t disk_sz)
{
    superblock *sb = new_superblock(disk_sz);
    
    write_disk((void *)sb, sizeof(superblock), disk, 0);

    free(sb);
}

static void create_file(uint32_t size, const char *path,
        const char *name, FILE *disk)
{
    //find an unallcated inode
    int in_block_loc = free_bm_addr(INODE, disk);

    //allocate the inode
    inode *in = new_inode(in_block_loc, size, name, disk);

    //write the inode
    int offset = INODE_START_ADDR + (in_block_loc * sizeof(inode));
    write_disk((void *)in, sizeof(inode), disk, offset);
    free(in);
}

int main(int argc, char *argv[])
{
    FILE *disk = fopen("disk", "rb+");
    make_filesystem(disk, file_size("disk"));
    fclose(disk);
    
    disk = fopen("disk", "rb+");
    create_file(5000, "Asd/fa/s", "newfile", disk); 
    read_inode(0, disk);
    fclose(disk);

    read_sb("disk");
    
    return 0;
}
