#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "util.h"
#include "inode.h"
#include "directory.h"
#include "block.h"
#include "file.h"

void create_dir(const char *name, block *parent_dir, FILE *disk)
{

}

void create_dir_entry(const char *name, int dir_inum, int data_inum, FILE *disk)
{
    inode *in = inode_at_num(data_inum, disk);
    inode *dir_in = inode_at_num(dir_inum, disk);
    entry *dir_entry = malloc(sizeof(entry));

    strcpy(dir_entry->name, name);
    dir_entry->strlen = strlen(name) + 1;
    dir_entry->blocks = in->blocks;

    append_data_to_file(dir_in->num, (void *)dir_entry, sizeof(dir_entry), disk);

    free(in);
    free(dir_in);
    free(dir_entry);
}
