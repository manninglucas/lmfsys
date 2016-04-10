#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include "types.h"
#include "inode.h"
#include "bitmap.h"
#include "superblock.h"
#include "block.h"

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

static void write(const void *data, uint32 size, FILE *disk)
{
    for (uint8 *byte = (uint8*) &data; size--; ++byte) {
        fputc(*byte, disk);
    }
}

void make_filesystem(char *disk_name, size_t disk_sz)
{
    //opening for w+ zeroes out the file first 
    FILE *disk = fopen(disk_name, "w+");
    int total_blocks = disk_sz / BLOCK_SIZE;

    superblock *sb = new_superblock(disk_sz);
    uint32 sb_size = sizeof(superblock);
    
    write((void *)sb, sizeof(superblock), disk);
}

void write_file(const char *file_path, char* data)
{

}

void read_file(const char *file_path, const char *path)
{

}

int main(int argc, char *argv[])
{
    make_filesystem("disk", file_size("disk"));
    
    return 0;
}
