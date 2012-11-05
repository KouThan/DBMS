// BlockAllocationTest.cpp

#include "Tests.h"

bool WriteStringToBlock(int file_desc, int block, const char *str)
{
	char *buffer = 0;
	if (BL_BeginBlock(file_desc, block, &buffer) != BLE_OK)	return false;
	strcpy(buffer, str);
	if (BL_EndBlock(file_desc, block, 1) != BLE_OK)	return false;

	return true;
}

bool BlockAllocationTest()
{
	int file_desc = BL_OpenFile("RandomNumbers.dat");
	if (file_desc < 0)
	{
		BL_PrintError("Failed to open file RandomNumbers.dat");
		return false;
	}

	printf("Opened file RandomNumbers.dat\n");

	int block1 = BL_AllocateBlock(file_desc);
	if (block1 <= 0)
	{
		BL_PrintError("Failed to allocate a new block in file RandomNumbers.dat");
		BL_CloseFile(file_desc);
		return false;
	}

	printf("Allocated block %d\n", block1);

	int block2 = BL_AllocateBlock(file_desc);
	if (block2 <= 0)
	{
		BL_PrintError("Failed to allocate a new block in file RandomNumbers.dat");
		BL_CloseFile(file_desc);
		return false;
	}

	printf("Allocated block %d\n", block2);

	// see if the allocated blocks are 3 and 5 (the ones we disposed in the previews test)
	if ((block1 == 3 && block2 == 5) || (block1 == 5 && block2 == 3))
	{
		printf("Newly allocated blocks are %d and %d as expected!\n", block1, block2);
	}
	else
	{
		printf("Error: Newly allocated blocks are %d and %d while in the previews test we have disposed blocks 3 and 5\n",
			block1, block2);
		BL_CloseFile(file_desc);
		return false;
	}

	if (!WriteStringToBlock(file_desc, 3, test_string1))
	{
		printf("Failed to write to newly allocated block 3\n");
		BL_CloseFile(file_desc);
		return false;
	}

	printf("Written the following string to newly allocated block 3: \"%s\"\n", test_string1);

	char *str2 = "Goodnight!";
	if (!WriteStringToBlock(file_desc, 5, test_string2))
	{
		printf("Failed to write to newly allocated block 5\n");
		BL_CloseFile(file_desc);
		return false;
	}

	printf("Written the following string to newly allocated block 5:\n%s\n", test_string2);

	if (BL_CloseFile(file_desc) != BLE_OK)
	{
		BL_PrintError("Failed to close file RandomNumbers.dat");
		return false;
	}

	printf("Closed file RandomNumbers.dat\n");

	return true;
}
