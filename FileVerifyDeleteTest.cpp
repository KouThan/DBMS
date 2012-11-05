// FileVerifyDeleteTest.cpp

#include "Tests.h"

bool FileVerifyDeleteTest()
{
	char tb[1024];
	for (int i=0; i<BL_MAX_OPEN_FILES; i++)
	{
		sprintf(tb, "%d.dat", i);
		int file_handle = BL_OpenFile(tb);
		if (file_handle < 0)
		{
			printf("Failed opening file: %s, reason: %d\n", tb, file_handle);
			return false;
		}

		printf("Opened file %s (handle %d)\n", tb, file_handle);

		int *data = NULL;
		int result = BL_BeginBlock(file_handle, 1, (char**)&data);

		if (result < 0)
		{
			printf("BeginBlock 1 Failed on handle %d, reason: %d\n",
				file_handle, result);
			return false;
		}

		if (*data != i)
		{
			printf("Data verification failed : %d instead of %d\n",
				*data, i);
			return false;
		}

		printf("Data was verified: The block indeed contains the number %d\n", i);

		result = BL_EndBlock(file_handle, 1, 0);

		if (result < 0)
		{
			printf("EndBlock 1 Failed on handle %d, reason: %d\n",
				file_handle, result);
			return false;
		}

		result = BL_CloseFile(file_handle);
		if (result < 0)
		{
			printf("Failed closing file handle: %d, reason: %d\n", 
				file_handle, result);
			return false;
		}

		printf("File was closed\n");
	}

	return true;
}
