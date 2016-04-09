# lmfsys

A toy virtual UNIX filesystem.

## Design

- Simple, and easy to follow design.
- Structured into blocks, inodes, superblocks, and bitmaps (for both inodes and blocks)
- Roughly 7/8 space for blocks, 1/8 space for inodes. (A little less for inodes to make room for bitmap and superblock)
- Multi-level indexing with
- No extents
- caching of reads and buffering of writes

#$ Goals

- The goal is to have the above design finished in by mid may, 2016. Thus, the filesystem will only attempt to implement the features described above and no more. 

## Note

As this is a virtual filesystem and not a real one, a large file must be allocated to act as the disk.

