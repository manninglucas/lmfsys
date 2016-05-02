#include <stdio.h>
#include <stdlib.h>
#include "util.h"

void write_disk(const void *data, uint32_t size, FILE *disk, uint32_t offset)
{ 
    fseek(disk, offset, SEEK_SET);
    fwrite(data, size, 1, disk);
}


