#ifndef TESTS_H
#define TESTS_H
/* TESTS
 * 
 * PURPOSE: Unit tests for filesystem functions to ensure they work
 * after modifications
 *
 *
 */
#include "block.h"

void run_tests();
/******************************************************************************/
void test_superblock();
/******************************************************************************/
void test_bitmap();
/******************************************************************************/
void test_inode();
/******************************************************************************/
void test_block();
void test_block_write(block *blk);
void test_block_contents(block *blk);
/******************************************************************************/
void test_directory();
/******************************************************************************/
void test_file();

#endif
