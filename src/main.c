#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include "superblock.h"
#include "inode.h"
#include "directory.h"
#include "file.h"

superblock *sb = NULL;

static unsigned file_size(const char *file_name)
{
    struct stat supb;
    
    if (stat(file_name, &supb) != 0) {
        fprintf(stderr, "'stat' failed for '%s': %s.\n", file_name,
               strerror(errno)); 
        exit(EXIT_FAILURE);
    }
    return supb.st_size;
}

void make_filesystem(size_t disk_sz, FILE *disk)
{
    sb = new_superblock(disk_sz);
    sb->disk = disk;

    inode *root_in = new_inode(BLOCK_SIZE / 8, DIRECTORY_INODE);
    int offset = INODE_START_ADDR + (root_in->num * sizeof(inode));
    write_disk((void *)root_in, sizeof(inode), offset);

    sb->root_inum = root_in->num;
    
    write_disk((void *)sb, sizeof(superblock), 0);

    free(root_in);
}

static void create_file(u32 size, int dir_inum,
        const char *name)
{
    //allocate the inode
    inode *in = new_inode(size, FILE_INODE);
    create_dir_entry(name, dir_inum, in->num);

    //write the inode
    int offset = INODE_START_ADDR + (in->num * sizeof(inode));
    write_disk((void *)in, sizeof(inode), offset);
    free(in);
}

void create_file_from_file(int dir_inum, const char *name)
{
    FILE *infile = fopen(name, "rb+");
    inode *in = new_inode(file_size(name), FILE_INODE);
    create_dir_entry(name, dir_inum, in->num);
    append_file(in->num, infile, file_size(name));
    free(in);
    fclose(infile);
}

void read_file_into_file(int dir_inum, const char *name)
{
    FILE *outfile = fopen(name, "rb+");
    int inum = inum_from_name(dir_inum, name);
    inode *in = inode_at_num(inum);
    read_file(inum, 0, in->size, outfile);
    free(in);
    fclose(outfile);
}

int main(int argc, char *argv[])
{
    FILE *disk = fopen("disk", "rb+");
    make_filesystem(file_size("disk"), disk);
    
    disk = fopen("disk", "rb+");
    create_file_from_file(sb->root_inum, "test.py");
    read_file_into_file(sb->root_inum, "output.txt");
    
    fclose(disk);
    free(sb);
    
    return 0;
}
