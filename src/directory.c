#include "directory.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "block.h"
#include "file.h"

void create_dir(const char *name, int parent_dir)
{

}

void create_dir_entry(const char *name, int dir_inum, int data_inum)
{
    inode *in = inode_at_num(data_inum);
    entry *dir_entry = malloc(sizeof(entry));

    strncpy(dir_entry->name, name, strlen(name));
    dir_entry->strlen = strlen(name) + 1;
    dir_entry->blocks = in->blocks;
    dir_entry->inum = data_inum;

    printf("dir entry inum: %i\n", dir_entry->inum);

    append_data_to_file(dir_inum, (u8 *)dir_entry, sizeof(entry));

    free(in);
    free(dir_entry);
}

int inum_from_name(int dir_inum, const char *name)
{
    inode *in = inode_at_num(dir_inum); 
    entry entries[in->size / sizeof(entry)];
    read_entries_into_arr(in, entries);
    int beg = 0;
    int end = sizeof(entries) / sizeof(entry);
    int mid = end / 2; 
    int res;
    free(in);
    printf("end: %i\n", end);    
    //binary search dirs are sorted alphabeticall
    while ((res = strncmp(entries[mid].name, name, entries[mid].strlen))) {
        if (res > 0)
            end = mid;
        else if (res < 0) 
            beg = mid;
        //should throw not found error here
        if (beg == end) return -1;
    }
    printf("%i\n", mid);
    return entries[mid].inum;
}

void read_entries_into_arr(inode *in, entry entries[])
{
    printf("read entries into arr\n");
    block *b = block_from_num(in->data_block[0]);
    int blocknum = 0;
    u8 bytes[in->size];
    for (int bytepos = 0; bytepos != in->size; bytepos++) {
        if (bytepos % BLOCK_SIZE == 0 && bytepos != 0)
            b = block_from_index(++blocknum, in);
        bytes[bytepos] = b->data[bytepos % BLOCK_SIZE];
    }
    entries = (entry *)bytes;
    free(b);   
}
