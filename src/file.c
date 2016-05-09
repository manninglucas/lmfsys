#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "inode.h"
#include "block.h"
#include "file.h"

void append_data_to_file(int inum, void *data, size_t size)
{
    inode *in = inode_at_num(inum, sb->disk);

    block *cur_blk = block_at_addr(in->end_block); 
    int pos = in->size % BLOCK_SIZE;

    for (u8 *byte = data; size != 0; byte++, size--) {
        write_data_to_block(cur_blk, byte, sizeof(u8), pos++);
        in->size += 1;
        if (pos / BLOCK_SIZE != 0) {
            pos = 0;
            alloc_blocks(in, 1);
            cur_blk = block_at_addr(in->end_block);
        }
    }
    
    free(in); 
    free(cur_blk);
}

void write_data_to_file(int inum, void *data, size_t size, int offset)
{
    resize_file(inum, offset);
    append_data_to_file(inum, data, size);
}

void read_file(int inum, FILE *out)
{
    inode *in = inode_at_num(inum);   
    block *b = block_at_addr(in->data_block[0]);
    int blocknum = 0;
    for (int bytepos = 0; bytepos != in->size; bytepos++) {
        if (bytepos % BLOCK_SIZE == 0 && bytepos != 0)
            b = block_at_num(++blocknum, in);
        fputc(b->data[bytepos], out); 
    }
}

void write_file(int inum, FILE *inf, int size, int offset)
{
    inode *in = inode_at_num(inum);
    resize_file(inum, offset);
    for (int i = 0; i < size; i++) {
        u8 byte = fgetc(inf);
        append_data_to_file(inum, &byte, 1);
    }
}

void append_file(int inum, FILE *inf, int size)
{
    inode *in = inode_at_num(inum);
    for (int i = 0; i < size; i++) {
        u8 byte = fgetc(inf);
        append_data_to_file(inum, &byte, 1);
    }
}

//this can only be used to shrink files
void resize_file(int inum, int new_size)
{
    inode *in = inode_at_num(inum);
    //remove 1 less block than we need to 
    int blks2remove = in->blocks - (new_size / BLOCK_SIZE + 1); 
    dealloc_blocks(in, blks2remove);
   
    //pos within the block 
    block *endblock = block_at_addr(in->end_block);
    int endpos = in->size - new_size;
    for (int i = in->size - 1 % BLOCK_SIZE ; i > endpos; ++i)
        endblock->data[i] = 0;
    in->size = new_size;
    
    write_block_to_disk(endblock);
    free(endblock);
} 
