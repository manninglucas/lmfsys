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
    for (uint8_t *byte = (uint8_t*) &data; size--; ++byte) {
        fputc(*byte, disk);
    }
}

void make_filesystem(char *disk_name, size_t disk_sz)
{
    //opening for w zeroes out the file first 
    FILE *disk = fopen(disk_name, "r+");

    superblock *sb = new_superblock(disk_sz);
    
    write_disk((void *)sb, sizeof(superblock), disk, 0);

    fclose(disk);
}

int main(int argc, char *argv[])
{
    make_filesystem("disk", file_size("disk"));
    
    return 0;
}
