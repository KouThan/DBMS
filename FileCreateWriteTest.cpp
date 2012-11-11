// FileCreateWriteTest.h

#include "Tests.h"

bool FileCreateWriteTest()
{
	printf("Trying to create %d files in disk\n", BL_MAX_OPEN_FILES);
	char tb[1024];
	int file_handles[BL_MAX_OPEN_FILES];
	for (int i=0; i<BL_MAX_OPEN_FILES; i++)
	{
		sprintf(tb, "%d.dat", i);
		int result = BL_CreateFile(tb);
		if (result)
		{
			printf("Failed to create file %s. Reason: %d\n", tb, result);
			return false;
		}

		printf("Created file %s\n", tb);

		// Verify the file
		file_handles[i] = BL_OpenFile(tb);
		if (file_handles[i] < 0)
		{
			printf("Failed to open newly created file : %s, Reason: %d\n",
				tb, file_handles[i]);
			return false;
		}

		printf("Opened file %s (handle %d)\n", tb, file_handles[i]);

		// write to the file
		int block = BL_AllocateBlock(file_handles[i]);
		if (block < 0)
		{
			printf("Failed to allocate a block to file %s, reason: %d\n",
				tb, block);
			return false;
		}

		printf("Allocated block %d in file %s\n", block, tb);

		char *data;
		result = BL_BeginBlock(file_handles[i], block, &data);
		if (result < 0)
		{
			printf("BeginBlock failed on file %s, reason: %d\n", 
				tb, result);
			return false;
		}

		// write data
		int *idata = (int*) data;
		*idata = i;

		printf("Written number %d to that block\n", i);

		result = BL_EndBlock(file_handles[i], block, 1);
		if (result < 0)
		{
			printf("EndBlock failed on file %s, reason: %d\n", 
				tb, result);
			return false;
		}
	}

	// Close file handles
	for (int i=0; i<BL_MAX_OPEN_FILES; i++)
	{
		int result = BL_CloseFile(file_handles[i]);
		if (result < 0)
		{
			printf("CloseFile failed on file handle %d, reason: %d\n", 
				file_handles[i], result);
			return false;
		}

		printf("Closed file handle %d\n", file_handles[i]);
	}
	return true;
}
