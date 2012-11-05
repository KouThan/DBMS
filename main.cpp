// main.cpp - test suite for Block Level

#include "Tests.h"

long int timeCounter;

int appOpenings[openingSize]; //app-openings array

openFilesInfo OpenFile[openingSize]; 

cacheBlock CacheArray[openingSize];

int BL_errorNUM;


void ClearTestFiles()
{
	// remove all test files
	char tb[1024];
	for (int i=0; i<BL_MAX_OPEN_FILES; i++)
	{
		sprintf(tb, "%d.dat", i);
		remove(tb);
	}
	remove("RandomNumbers.dat");
}

int main()
{
	ClearTestFiles();

	int total_tests = 8;
	int tests_succeeded = 0;
	tests_succeeded += RunTest(0, "BL initialization and cleanup");
	tests_succeeded += RunTest(FileCreateWriteTest, 
		"File creation and writing");
	tests_succeeded += RunTest(FileVerifyDeleteTest, 
		"File content verification and deletion");
	tests_succeeded += RunTest(RandomWriteTest, 
		"Writing random numbers to file");
	tests_succeeded += RunTest(RandomVerifyTest, 
		"Verifying random numbers written to file");
	tests_succeeded += RunTest(BlockDisposalTest, 
		"Block Disposal Test");
	tests_succeeded += RunTest(BlockAllocationTest, 
		"Block Allocation Test");
	tests_succeeded += RunTest(AllocatedVerifyTest, 
		"Reallocated block integrity check");

	printf("%d tests succeeded, %d failed, %d total\n", tests_succeeded,
		total_tests - tests_succeeded, total_tests);

	ClearTestFiles();
	
	
    system("PAUSE");
	return 0;
}
