#include <stdlib.h>
#include <time.h>
#include "block.h"
#include "inode.h"

inode *new_inode(int inode_num, int block_count)
{
    inode *in = malloc(sizeof(inode));
    in->blocks = block_count;
    in->uid = 1111;
    in->ctime = time(NULL);
    in->mtime = time(NULL);
    in->dtime = time(NULL);

    return in;
}
