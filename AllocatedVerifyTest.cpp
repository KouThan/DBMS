// AllocatedVerifyTest.cpp

#include "Tests.h"

bool VerifyBlockString(int file_desc, int block, const char *str)
{
	char *buffer = 0;
	if (BL_BeginBlock(file_desc, block, &buffer) != BLE_OK)	return false;
	int result = strcmp(str, buffer);
	if (BL_EndBlock(file_desc, block, 1) != BLE_OK)	return false;

	return result == 0;
}

bool AllocatedVerifyTest()
{
	int file_desc = BL_OpenFile("RandomNumbers.dat");
	if (file_desc < 0)
	{
		BL_PrintError("Failed to open file RandomNumbers.dat");
		return false;
	}

	printf("Opened file RandomNumbers.dat\n");

	if (!VerifyBlockString(file_desc, 3, test_string1))
	{
		BL_PrintError("Failed to verify string on newly allocated block 3");
		BL_CloseFile(file_desc);
		return false;
	}

	printf("Block 3 indeed contains the string \"%s\"\n", test_string1);
	
	if (!VerifyBlockString(file_desc, 5, test_string2))
	{
		BL_PrintError("Failed to verify string on newly allocated block 5");
		BL_CloseFile(file_desc);
		return false;
	}

	printf("Block 5 indeed contains the string \"%s\"\n", test_string2);

	if (BL_CloseFile(file_desc) != BLE_OK)
	{
		BL_PrintError("Failed to close file RandomNumbers.dat");
		return false;
	}

	printf("Closed file RandomNumbers.dat\n");

	if (BL_DestroyFile("RandomNumbers.dat") != BLE_OK)
	{
		BL_PrintError("Failed to delete file RandomNumbers.dat");
		return false;
	}

	printf("Deleted file RandomNumbers.dat\n");

	return true;
}
