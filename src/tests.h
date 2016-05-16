#ifndef TESTS_H
#define TESTS_H
/* TESTS
 * 
 * PURPOSE: Unit tests for filesystem functions to ensure they work
 * after modifications
 *
 *
 */

void run_tests();
/******************************************************************************/
void test_superblock();
/******************************************************************************/
void test_bitmap();
/******************************************************************************/
void test_inode();
/******************************************************************************/
void test_block();
/******************************************************************************/
void test_directory();
/******************************************************************************/
void test_file();

#endif
