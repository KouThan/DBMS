// BlockDisposalTest

#include "Tests.h"

bool BlockDisposalTest()
{
	int file_desc = BL_OpenFile("RandomNumbers.dat");
	if (file_desc < 0)
	{
		BL_PrintError("Failed to open file RandomNumbers.dat");
		return false;
	}

	printf("Opened file RandomNumbers.dat\n");

	// dispose blocks 3 and 5
	if (BL_DisposeBlock(file_desc, 3) != BLE_OK)
	{
		BL_PrintError("Failed to dispose block 3 of file RandomNumbers.dat");
		BL_CloseFile(file_desc);
		return false;
	}

	printf("Disposed block 3\n");

	if (BL_DisposeBlock(file_desc, 5) != BLE_OK)
	{
		BL_PrintError("Failed to dispose block 5 of file RandomNumbers.dat");
		BL_CloseFile(file_desc);
		return false;
	}

	printf("Disposed block 5\n");

	// try to begin working with block 3
	char *buffer = 0;
	if (BL_BeginBlock(file_desc, 3, &buffer) == BLE_OK)
	{
		BL_PrintError("Block Level pinned block 3 which was previewsly disposed");
		BL_EndBlock(file_desc, 3, 0);
		BL_CloseFile(file_desc);
		return false;
	}

	printf("Pinning block 3 failed as expected\n");

	// try to begin working with block 5
	if (BL_BeginBlock(file_desc, 5, &buffer) == BLE_OK)
	{
		BL_PrintError("Block Level pinned block 5 which was previewsly disposed");
		BL_EndBlock(file_desc, 5, 0);
		BL_CloseFile(file_desc);
		return false;
	}

	printf("Pinning block 5 failed as expected\n");

	if (BL_CloseFile(file_desc) != BLE_OK)
	{
		BL_PrintError("Failed to close file RandomNumbers.dat");
		return false;
	}

	printf("Closed file RandomNumbers.dat\n");

	return true;
}
