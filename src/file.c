#include "file.h"
#include "util.h"
#include "inode.h"
#include "block.h"
#include "directory.h"
#include <stdlib.h>

void append_data_to_file(int inum, u8 *data, size_t size)
{
    printf("append data file at num %i\n", inum);
    inode *in = inode_at_num(inum);
    printf("inode: num: %i, size: %i, blocks: %i, end addr: %i\n",
            in->num, in->size, in->blocks, in->end_block);

    block *cur_blk = block_from_num(in->end_block); 
    printf("block addr: %i\n", in->end_block);
    int pos = in->size % BLOCK_SIZE;
    int blocks = (pos + size) / BLOCK_SIZE;
    int numbytes = BLOCK_SIZE - pos;
    write_data_to_block(cur_blk, data, size, pos);
    in->size += numbytes;
    //printf("%i:",pos);

    for (int i = 0; i < blocks; i++) {
        alloc_blocks(in, 1);
        cur_blk = block_from_num(in->end_block);
        int pos = in->size % BLOCK_SIZE;
        int numbytes = BLOCK_SIZE - pos;
        write_data_to_block(cur_blk, data, numbytes, pos);
        in->size += numbytes;
    }
    write_block_to_disk(cur_blk);
    in->size += size;
    write_inode_to_disk(in);
    
    free(in); 
    free(cur_blk);
}

void write_data_to_file(int inum, void *data, size_t size, int offset)
{
    resize_file(inum, offset);
    append_data_to_file(inum, data, size);
}

void read_file(int inum, int startpos, int endpos, FILE *out)
{
    inode *in = inode_at_num(inum);
    int blocknum = startpos / BLOCK_SIZE;
    block *b = block_from_index(blocknum, in);
    for (int bytepos = startpos % BLOCK_SIZE; bytepos != endpos; bytepos++) {
        if (bytepos % BLOCK_SIZE == 0 && bytepos != 0)
            b = block_from_index(++blocknum, in);
        fputc(b->data[bytepos % BLOCK_SIZE], out); 
    }
    free(in);
    free(b);
}

void write_file(int inum, FILE *inf, int size, int offset)
{
    resize_file(inum, offset);
    u8 bytes[size];
    for (int i = 0; i < size; i++) {
        u8 byte = fgetc(inf);
        bytes[i] = byte;
    }
    append_data_to_file(inum, bytes, size);
}

void append_file(int inum, FILE *inf, int size)
{
    printf("appen file\n");
    u8 bytes[size];
    for (int i = 0; i < size; i++) {
        u8 byte = fgetc(inf);
        bytes[i] = byte;
    }
    append_data_to_file(inum, bytes, size);
}

//this can only be used to shrink files
void resize_file(int inum, int new_size)
{
    inode *in = inode_at_num(inum);
    //remove 1 less block than we need to 
    int blks2remove = in->blocks - (new_size / BLOCK_SIZE + 1); 
    dealloc_blocks(in, blks2remove);
   
    //pos within the block 
    block *endblock = block_from_num(in->end_block);
    int endpos = in->size - new_size;
    for (int i = in->size - 1 % BLOCK_SIZE ; i > endpos; ++i)
        endblock->data[i] = 0;
    in->size = new_size;
    
    write_inode_to_disk(in); 
    write_block_to_disk(endblock);
    free(endblock);
    free(in);
}

void read_file_into_file(int dir_inum, const char *name, const char *outf)
{
    FILE *outfile = fopen(outf, "wb");
    int inum = inum_from_name(dir_inum, name);
    if (inum == -1) {
        printf("dir not found\n");
        exit(EXIT_FAILURE);
    }
    printf("file inum:%i\n",inum);
    inode *in = inode_at_num(inum);
    int size = in->size;
    printf("readfile\n");
    read_file(inum, 0, in->size, outfile);
    free(in);
    fclose(outfile);
}

void create_file_from_file(int dir_inum, const char *name)
{
    FILE *infile = fopen(name, "rb");
    inode *in = new_inode(file_size(name), FILE_INODE);
    write_inode_to_disk(in);
    create_dir_entry(name, dir_inum, in->num);
    append_file(in->num, infile, file_size(name));
    free(in);
    fclose(infile);
} 
