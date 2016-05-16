#ifndef FILE_H
#define FILE_H

#include "inode.h"
#include <stdio.h>

void write_data_to_file(int inum, void *data, size_t size, int offset);
void append_data_to_file(int inum, u8 *data, size_t size);
u32 find_block_addr(inode *in, int block_offset);
void resize_file(int inum, int new_size);
void read_file(int inum, int startpos, int endpos, FILE *out);
void write_file(int inum, FILE *inf, int size, int offset);
void append_file(int inum, FILE *inf, int size);
void create_file_from_file(int dir_inum, const char *name);
void read_file_into_file(int dir_inum, const char *name, const char *outf);

#endif
