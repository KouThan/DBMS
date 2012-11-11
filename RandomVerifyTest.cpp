// RandomVerifyTest.cpp

#include "Tests.h"

bool RandomVerifyTest()
{
	int file_desc = BL_OpenFile("RandomNumbers.dat");
	if (file_desc < 0)
	{
		BL_PrintError("Failed to open file RandomNumbers.dat");
		return false;
	}

	int file_desc2 = BL_OpenFile("RandomNumbers.dat");
	if (file_desc2 < 0)
	{
		BL_PrintError("Failed to open file RandomNumbers.dat a second time");
		return false;
	}

	printf("Opened file RandomNumbers.dat twice\n");

	srand(0);

	int random_counter = 0;

	for (int i=0; i<10; i++)
	{
		int curr_block = GetNthBlock(file_desc, i);
		if (curr_block < 0)
		{
			BL_CloseFile(file_desc);
			BL_CloseFile(file_desc2);
			BL_PrintError("RandomNumbers.dat does not contain 10 writeable blocks as expected");
			return false;
		}
		int *buffer;
		if (BL_BeginBlock(file_desc, curr_block, (char**)&buffer) != BLE_OK)
		{
			BL_CloseFile(file_desc);
			BL_CloseFile(file_desc2);
			BL_PrintError("Failed to pin a block on file RandomNumbers.dat");
			return false;
		}

		int *buffer2;
		if (BL_BeginBlock(file_desc2, curr_block, (char**)&buffer2) != BLE_OK)
		{
			BL_CloseFile(file_desc);
			BL_CloseFile(file_desc2);
			BL_PrintError("Failed to pin a block on file RandomNumbers.dat");
			return false;
		}

		// verify that both buffers are the same
		if (buffer != buffer2)
		{
			BL_CloseFile(file_desc);
			BL_CloseFile(file_desc2);
			BL_PrintError("Requesting the same block from two openings of RandomNumbers.dat results in different buffers in memory");
			return false;
		}

		for (int j=0; j<MAX_RANDOM_NUMBERS_PER_BLOCK; j++)
		{
			int r = rand() % 100;
			if (buffer[j] != r)
			{
				char tb[1024];
				sprintf(tb, "Verification failed: block %d must contain %d instead of %d in %dth integer position, random_counter = %d",
					curr_block, r, buffer[j], j, random_counter);
				BL_CloseFile(file_desc);
				BL_CloseFile(file_desc2);
				BL_PrintError(tb);
				return false;
			}
			random_counter ++;
		}

		if (BL_EndBlock(file_desc2, curr_block, 1) != BLE_OK)
		{
			BL_CloseFile(file_desc);
			BL_CloseFile(file_desc2);
			BL_PrintError("Failed to unpin a block on second opening of file RandomNumbers.dat");
			return false;
		}

		if (BL_EndBlock(file_desc, curr_block, 1) != BLE_OK)
		{
			BL_CloseFile(file_desc);
			BL_CloseFile(file_desc2);
			BL_PrintError("Failed to unpin a block on file RandomNumbers.dat");
			return false;
		}

		printf("Verified all %d random numbers in block %d\n", MAX_RANDOM_NUMBERS_PER_BLOCK,
			curr_block);
	}

	if (BL_CloseFile(file_desc2) != BLE_OK)
	{
		BL_PrintError("Failed to close second opening of file RandomNumbers.dat");
		return false;
	}

	if (BL_CloseFile(file_desc) != BLE_OK)
	{
		BL_PrintError("Failed to close file RandomNumbers.dat");
		return false;
	}

	return true;
}
