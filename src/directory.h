#ifndef DIRECTORY_H
#define DIRECTORY_H

typedef struct {
    char name[32];
    uint32_t inum;
    uint32_t strlen;
    uint32_t blocks; //num of blocks allocated to file 
} entry;
    
void create_dir_entry(const char *name, int dir_inum, int data_inum, FILE *disk);
#endif
