#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

#define BLE_OK 0
#define BLE_NOMEM -10
#define BLE_NOBUF -20
#define BLE_BLOCKFIXED -30
#define BLE_BLOCKNOTINBUF -40
#define BLE_BLOCKINBUF -50
#define BLE_OS -60
#define BLE_INVALIDBLOCK -70
#define BLE_FILEOPEN -80
#define BLE_FTABFULL -90
#define BLE_OFTABFULL -100
#define BLE_FD -110
#define BLE_EOF -120
#define BLE_BLOCKFREE -130
#define BLE_BLOCKUNFIXED -140
#define BLE_FILERROR -150  //EXTRA DIKA MAS DEFINE
#define BLE_FILENERASE -160 //"
#define BLE_FILENOPEN -170 //"
#define BLE_FILENEXIST -180  //"
#define BLE_NONEXTFILE -190
#define BLE_FILEFULL -200 //Define gia tin AllocateBlock
#define BLE_FILENCLOSEPROPER -210
#define FileNameLength 20
#define blockSize 1024
#define openingSize 20
#define TRUE 1
#define FALSE 0
#define EMPTY -1 //gia na doume an enas pinakas einai kenos i oxi|einai swsto i oxi?
#define ValidB '*'

typedef struct cacheBlock cacheBlock;
typedef struct openFiles openFilesInfo;

struct cacheBlock{

       char data[blockSize];  //array me 1024 xaraktires gia block
       int pins[openingSize];  //array gia pins
       int ID;  //anagnwristiko gia to block
       int fileNamePointer; //gia onoma arxeiou
       int modified; //modified int gia 0 i 1
       char FileName[openingSize];//Onoma arxeiou
       long int timeStamp;

};// struct gia ta block tis cache

struct openFiles{
       
       char fileName[FileNameLength];  //filename
       char byteMap[blockSize]; //data gia header
       FILE *fileHandler;  //handler 

};// struct gia ta openfiles

long int timeCounter;

int appOpenings[openingSize]; //app-openings array

openFilesInfo OpenFile[openingSize]; 

cacheBlock CacheArray[openingSize];

int BL_errorNUM;


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
