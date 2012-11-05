// Tests.h

#ifndef _TESTS_H_
#define _TESTS_H_

#include "BL.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// definitions
#define MAX_RANDOM_NUMBERS_PER_BLOCK (BL_BLOCK_SIZE / sizeof(int))

// globals
extern char *test_string1;
extern char *test_string2;

// test prototypes
bool FileCreateWriteTest();
bool FileVerifyDeleteTest();
bool RandomWriteTest();
bool RandomVerifyTest();
bool BlockDisposalTest();
bool BlockAllocationTest();
bool AllocatedVerifyTest();

// utilities
typedef bool (*TestFunc)();
int RunTest(TestFunc func, const char *test_name);
int GetNthBlock(int file_desc, int n);

#endif // ndef _TESTS_H_
