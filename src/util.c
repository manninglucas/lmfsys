#include "util.h"
#include "superblock.h"
#include <stdio.h>
#include <stdlib.h>

void write_disk(const void *data, uint32_t size, uint32_t offset)
{ 
    fseek(sb->disk, offset, SEEK_SET);
    fwrite(data, size, 1, sb->disk);
}


