#include "util.h"
#include "superblock.h"
#include "inode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

void write_disk(const void *data, u32 size, u32 offset)
{ 
    fseek(sb->disk, offset, SEEK_SET);
    fwrite(data, size, 1, sb->disk);
}

unsigned file_size(const char *file_name)
{
    struct stat supb;
    
    if (stat(file_name, &supb) != 0) {
        fprintf(stderr, "'stat' failed for '%s': %s.\n", file_name,
               strerror(errno)); 
        exit(EXIT_FAILURE);
    }
    return supb.st_size;
}

void make_filesystem(u32 size, FILE *disk)
{
    sb = new_superblock(size);
    sb->disk = disk;

    inode *root_inode = new_inode(DIR_SIZE, DIR_INODE);
    write_inode_to_disk(root_inode);

    sb->root_inum = root_inode->num;
    
    write_disk((void *)sb, sizeof(superblock), 0);

    free(root_inode);
}
