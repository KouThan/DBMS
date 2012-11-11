// RandomWriteTest.cpp

#include "Tests.h"

bool RandomWriteTest()
{
	// make sure the file does not exist
	BL_DestroyFile("RandomNumbers.dat");

	if (BL_CreateFile("RandomNumbers.dat") != BLE_OK)
	{
		BL_PrintError("Failed to create file RandomNumbers.dat");
		return false;
	}

	int file_desc = BL_OpenFile("RandomNumbers.dat");
	if (file_desc < 0)
	{
		BL_PrintError("Failed to open file RandomNumbers.dat");
		return false;
	}

	// allocate 10 blocks 
	for (int i=0; i<10; i++)
	{
		if (BL_AllocateBlock(file_desc) < 0)
		{
			BL_CloseFile(file_desc);
			BL_PrintError("Failed to allocate a block on file RandomNumbers.dat");
			return false;
		}
	}
	printf("Allocated 10 blocks in file RandomNumbers.dat\n");

	srand(0);

	int random_counter = 0;
    
	for (int i=0; i<10; i++)
	{
		int curr_block = GetNthBlock(file_desc, i);
		
		if (curr_block < 0)
		{
			BL_CloseFile(file_desc);
			BL_PrintError("RandomNumbers.dat does not contain 10 writeable blocks as expected");
			return false;
		}
		int *buffer;
		if (BL_BeginBlock(file_desc, curr_block, (char**)&buffer) != BLE_OK)
		{
			BL_CloseFile(file_desc);
			BL_PrintError("Failed to pin a block on file RandomNumbers.dat");
			return false;
		}

		for (int j=0; j<MAX_RANDOM_NUMBERS_PER_BLOCK; j++)
		{
			buffer[j] = rand() % 100;
			random_counter ++;
		}
		 
		if (BL_EndBlock(file_desc, curr_block, 1) != BLE_OK)
		{
			BL_CloseFile(file_desc);
			BL_PrintError("Failed to unpin a block on file RandomNumbers.dat");
			return false;
		}

		printf("Successfully written %d random numbers to block %d\n", MAX_RANDOM_NUMBERS_PER_BLOCK,
			curr_block);
	}

	if (BL_CloseFile(file_desc) != BLE_OK)
	{
		BL_PrintError("Failed to close file RandomNumbers.dat");
		return false;
	}

	printf("Closed file RandomNumbers.dat");

	return true;
}
