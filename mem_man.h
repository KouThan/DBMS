#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

int BL_init(void);

int BL_DestroyFile(char *filename);

int BL_OpenFile(char *filename);

int BL_CloseFile(int openDesc);

int BL_GetFirstBlock(int openDesc); 

int BL_GetNextBlock(int openDesc, int blockNum);

int BL_BeginBlock(int openDesc, int blockNum, char **blockBuf);

int BL_AllocateBlock(int openDesc);

int BL_DisposeBlock(int openDesc, int blockNum);

int BL_EndBlock(int openDesc, int blockNum, int modified);

void BL_PrintError(char *errorString);

int BL_CleanUp(void);

#endif
