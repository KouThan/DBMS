// Tests.cpp

#include "Tests.h"

char *test_string1 = "Hello, world! This is a test string for part 1 (Block Level) for the course Systems Programming";
char *test_string2 = "Department of Informatics and Telematics, Harokopio University of Athens";

int GetNthBlock(int file_desc, int n)
{
	int ret = BL_GetFirstBlock(file_desc);

	if (ret < 0) return ret;

	for (int i=1; i<=n; i++)
	{
		ret = BL_GetNextBlock(file_desc, ret);
	}

	return ret;
}


int RunTest(TestFunc func, const char *test_name)
{
	printf("\n**********************************************************\n");
	printf("Running test: %s\n", test_name);
	printf("**********************************************************\n\n");

	if (BL_Init())
	{
		printf("Failed to initialize BL\n");
		printf("\n*** Test Failed! ***\a\n");
		return 0;
	}
	else
	{
		printf("BL initialized\n\n");
	}

	if (func)
	{
		if (!func()) 
		{
			printf("\n*** Test Failed! ***\a\n");
			return 0;
		}
	}

	if (BL_CleanUp())
	{
		printf("Failed to deinitialize BL\n");
		printf("\n*** Test Failed ***!\a\n");
		return 0;
	}
	else
	{
		printf("\nBL deinitialized\n");
	}

	printf("\n*** Test Succeeded! ***\n\n");
	return 1;
}
