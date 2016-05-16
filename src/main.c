#include "util.h"
#include "superblock.h"
#include "inode.h"
#include "file.h"
#include "tests.h"
#include <stdlib.h>
#include <string.h>

superblock *sb = NULL;

int main(int argc, char *argv[])
{
    if (argc > 1 && strcmp(argv[1], "test") == 0) {
        run_tests();
        exit(EXIT_SUCCESS);
    }

    FILE *disk = fopen("disk", "rb+");
    make_filesystem(file_size("disk"), disk);

    /* inode *in = new_inode(50000, FILE_INODE); */
    /* printf("inode: num: %i, size: %i, blocks: %i, end addr: %i\n", */
    /*         in->num, in->size, in->blocks, in->end_block); */
    /* write_inode_to_disk(in); */
    /* int num = in->num; */
    /* free(in); */
    /* in = inode_at_num(num); */
    /* printf("inode: num: %i, size: %i, blocks: %i, end addr: %i\n",  */
    /*         in->num, in->size, in->blocks, in->end_block); */
    create_file_from_file(sb->root_inum, "test.py");
    //read_file_into_file(sb->root_inum, "test.py", "output.txt");
     
    fclose(disk);
    free(sb);
    
    return 0;
}
