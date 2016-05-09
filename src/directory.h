#ifndef DIRECTORY_H
#define DIRECTORY_H

typedef struct {
    char name[32];
    uint32_t inum;
    uint32_t strlen;
    uint32_t blocks; //num of blocks allocated to file 
} entry;
    
void create_dir_entry(const char *name, int dir_inum, int data_inum);
void read_entries_into_arr(inode *in, entry **mem);
int inum_from_name(int dir_inum, const char *name);
#endif
