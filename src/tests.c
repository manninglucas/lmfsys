#include "tests.h"
#include "util.h"
#include "superblock.h"
#include "bitmap.h"
#include "inode.h"
#include "block.h"
#include "directory.h"
#include "file.h"
#include "assert.h"

void run_tests()
{
    test_bitmap();
}

void test_bitmap()
{
    //bitmap automatically uses the 0 position for the root dir
    printf("testing bitmap...\n");

    FILE *disk = fopen("disk", "rb+");
    make_filesystem(file_size("disk"), disk);
    
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

    fclose(disk);

    printf("bitmap tests successful.\n");
}
