#include "tests.h"
#include "util.h"
#include "superblock.h"
#include "bitmap.h"
#include "inode.h"
#include "directory.h"
#include "file.h"
#include "assert.h"
#include "unistd.h"
#include <stdlib.h>
#include <string.h>

void run_tests()
{
    printf("creating test env...\n");

    FILE *disk = fopen("test_disk", "wb");
    //1G test file
    fseek(disk, 0x40000000, SEEK_SET);
    fputc('\0', disk);
    fclose(disk);

    disk = fopen("test_disk", "rb+");
    make_filesystem(file_size("test_disk"), disk);

    test_bitmap();
    test_block();

    fclose(disk);
    unlink("test_disk");
}

void test_bitmap()
{
    //bitmap automatically uses the 0 position for the root dir
    printf("testing bitmap...\n");
        
    int bitpos1 = empty_bit_in_map(BM_INODE);
    
    assert(bitpos1 == 1);

    flip_bit_in_map(bitpos1, BM_INODE);
    int bitpos2 = empty_bit_in_map(BM_INODE);

    assert(bitpos2 == 2);

    bitpos1 = empty_bit_in_map(BM_DATA);

    assert(bitpos1 == 1);

    flip_bit_in_map(bitpos1, BM_DATA);
    bitpos2 = empty_bit_in_map(BM_DATA);

    assert(bitpos2 == 2);

    printf("bitmap tests successful.\n");
}

void test_block()
{
    printf("testing block...\n");
    
    block *blk = new_block(0);
    test_block_write(blk);

    inode *in = new_inode(0x9C4000, FILE_INODE);
    write_inode_to_disk(in);

    blk = block_from_index(10, in);
    test_block_write(blk);

    blk = block_from_index(52, in);
    test_block_write(blk);

    blk = block_from_index(2000, in);
    test_block_write(blk);
    free(blk);

    blk = block_from_index(10, in);
    test_block_contents(blk);
    blk = block_from_index(52, in);
    test_block_contents(blk);
    blk = block_from_index(2000, in);
    test_block_contents(blk);

    free(blk);
    printf("bitmap tests successful.\n");
}

void test_block_contents(block *blk)
{
    char test[100];
    strcpy(test, (char *)blk->data);
    assert(strcmp(test, "The quick brown fox jumped over the lazy dog.") == 0);
}

void test_block_write(block *blk)
{
    u32 num = blk->num;
    char *string = "The quick brown fox jumped over the lazy dog.";
    write_data_to_block(blk, string, 46, 0);
    write_block_to_disk(blk);

    blk = block_from_num(num);

    char test[100];
    strcpy(test, (char *)blk->data);

    assert(strcmp(test, string) == 0);
}

void test_inode()
{
    inode *in = new_inode(0x4000, FILE_INODE);
    printf("write inode to disk\n");
    write_inode_to_disk(in);

}
