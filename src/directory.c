#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "util.h"
#include "inode.h"
#include "directory.h"
#include "block.h"
#include "file.h"

void create_dir(const char *name, int parent_dir)
{

}

void create_dir_entry(const char *name, int dir_inum, int data_inum)
{
    inode *in = inode_at_num(data_inum);
    entry *dir_entry = malloc(sizeof(entry));

    strcpy(dir_entry->name, name);
    dir_entry->strlen = strlen(name) + 1;
    dir_entry->blocks = in->blocks;

    append_data_to_file(dir_inum, (void *)dir_entry, sizeof(dir_entry));

    free(in);
    free(dir_entry);
}
