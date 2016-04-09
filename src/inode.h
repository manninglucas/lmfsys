#ifndef INODE_H
#define INODE_H

typedef struct {
    short mode;
    short uid;
    int size;
    int time;
    int ctime;
    int mtime;
    int dtime;
    short gid;
    short links_count;
    int blocks;
    int flags;
    int osd1;
    block *block[15];
    int generation;
} inode;

#endif
