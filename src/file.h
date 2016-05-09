#ifndef FILE_H
#define FILE_H


void write_data_to_file(int inum, void *data, size_t size, int offset);
void append_data_to_file(int inum, void *data, size_t size);
uint32_t find_block_addr(inode *in, int block_offset);
void resize_file(int inum, int new_size);
void read_file(int inum, int startpos, int endpos, FILE *out);
void write_file(int inum, FILE *inf, int size, int offset);
void append_file(int inum, FILE *inf, int size);

#endif
