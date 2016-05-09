#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "util.h"
#include "inode.h"
#include "directory.h"
#include "block.h"
#include "file.h"

void create_dir(const char *name, int parent_dir)
{

}

void create_dir_entry(const char *name, int dir_inum, int data_inum)
{
    inode *in = inode_at_num(data_inum);
    entry *dir_entry = malloc(sizeof(entry));

    strcpy(dir_entry->name, name);
    dir_entry->strlen = strlen(name) + 1;
    dir_entry->blocks = in->blocks;

    append_data_to_file(dir_inum, (void *)dir_entry, sizeof(dir_entry));

    free(in);
    free(dir_entry);
}

int inum_from_name(int dir_inum, const char *name)
{
    inode *in = inode_at_num(dir_inum); 
    entry *entries[in->size / sizeof(entry)];
    read_entries_into_arr(in, entries);
    int beg = 0;
    int end = in->size / sizeof(entry);
    int mid = end / 2; 
    int res;
    //binary search dirs are sorted alphabeticall
    while ((res = strcmp(entries[mid]->name, name))) {
        if (res > 0) end = mid;
        else if (res < 0) beg = mid;
        //should throw not found error here
        if (beg == end) break;
    }
    return entries[mid]->inum;
}

void read_entries_into_arr(inode *in, entry *entries[])
{
    block *b = block_at_addr(in->data_block[0]);
    int i = 0;
    int blocknum = 0;
    for (int bytepos = 0; bytepos != in->size; 
            bytepos+=sizeof(entry)) {
        if (bytepos % BLOCK_SIZE == 0 && bytepos != 0)
            b = block_at_num(++blocknum, in);
        if (i++ % sizeof(entry) == 0 && i != 0) 
            entries[i/sizeof(entry)] = (entry *)&b->data[bytepos % BLOCK_SIZE]; 
    }
    free(in);
    free(b);
}
