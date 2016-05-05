#ifndef FILE_H
#define FILE_H


void write_data_to_file(int inum, void *data, size_t size, FILE *disk);
void append_data_to_file(int inum, void *data, size_t size, FILE *disk);
uint32_t end_block_addr(inode *in);

#endif
