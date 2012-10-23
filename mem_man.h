#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

#define BLE_OK 0
#define BLE_NOMEM -1
#define BLE_NOBUF -2
#define BLE_BLOCKFIXED -3
#define BLE_BLOCKNOTINBUF -4
#define BLE_BLOCKINBUF -5
#define BLE_OS -6
#define BLE_INVALIDBLOCK -7
#define BLE_FILEOPEN -8
#define BLE_FTABFULL -9
#define BLE_OFTABFULL -10
#define BLE_FD -11
#define BLE_EOF -12
#define BLE_BLOCKFREE -13
#define BLE_BLOCKUNFIXED -14
#define BLE_FILERROR -15  //EXTRA DIKA MAS DEFINE
#define BLE_FILENERASE -16 //"
#define BLE_FILENOPEN -17  //"
#define BLE_FILENEXIST -18  //"
#define BLE_NONEXTFILE -19
#define FileNameLength 20
#define blockSize 1024
#define openingSize 20
#define TRUE 1
#define FALSE 0
#define EMPTY -1 //gia na doume an enas pinakas einai kenos i oxi|einai swsto i oxi?

typedef struct cacheBlock cacheBlock;
typedef struct openFiles openFilesInfo;

struct cacheBlock{

       char data[blockSize];  //array me 1024 xaraktires gia block
       int pins[openingSize];  //array gia pins
       int ID;  //anagnwristiko gia to block
       int fileNamePointer; //gia onoma arxeiou
       int modified; //modified int gia 0 i 1

};// struct gia ta block tis cache

struct openFiles{
       
       char fileName[FileNameLength];  //filename
       char byteMap[blockSize]; //data gia header
       FILE *fileHandler;  //handler 

};// struct gia ta openfiles

int appOpenings[openingSize]; //app-openings array

openFilesInfo OpenFile[blockSize]; //open files array (alaxte to megethos)

cacheBlock CacheArray[openingSize];


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
