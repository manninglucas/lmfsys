#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include "util.h"
#include "block.h"
#include "inode.h"
#include "bitmap.h"
#include "superblock.h"
#include "directory.h"

//TODO: replace all the disk vars with the global version

static unsigned file_size(const char *file_name)
{
    struct stat sb;
    
    if (stat(file_name, &sb) != 0) {
        fprintf(stderr, "'stat' failed for '%s': %s.\n", file_name,
               strerror(errno)); 
        exit(EXIT_FAILURE);
    }
    return sb.st_size;
}

void make_filesystem(size_t disk_sz)
{
    superblock *sb = new_superblock(disk_sz);

    inode *root_in = new_inode(BLOCK_SIZE / 8, DIRECTORY_INODE, disk);
    int offset = INODE_START_ADDR + (root_in->num * sizeof(inode));
    write_disk((void *)root_in, sizeof(inode), disk, offset);

    sb->root_inum = root_in->num;
    
    write_disk((void *)sb, sizeof(superblock), disk, 0);

    free(root_in);
    free(sb);
}

static void create_file(uint32_t size, int dir_inum,
        const char *name)
{
    //allocate the inode
    inode *in = new_inode(size, FILE_INODE, disk);
    create_dir_entry(name, dir_inum, in->num, disk);

    //write the inode
    int offset = INODE_START_ADDR + (in->num * sizeof(inode));
    write_disk((void *)in, sizeof(inode), disk, offset);
    free(in);
}

int main(int argc, char *argv[])
{
    disk = fopen("disk", "rb+");
    make_filesystem(file_size("disk"));

    superblock *sb = fs_sb(disk);
    
    disk = fopen("disk", "rb+");
    create_file(5000, sb->root_inum, "newfile");
    read_inode(0, disk);
    fclose(disk);

    read_sb("disk");
    
    return 0;
}
