#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "inode.h"
#include "util.h"

typedef struct {
    char name[32];
    u32 inum;
    u32 strlen;
    u32 blocks; //num of blocks allocated to file 
}__attribute__((__packed__))  entry;

void create_dir_entry(const char *name, int dir_inum, int data_inum);
void read_entries_into_arr(inode *in, entry entries[]);
int inum_from_name(int dir_inum, const char *name);
#endif
