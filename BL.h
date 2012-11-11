#ifndef BL_H
#define BL_H

#include<stdio.h>
#include<stdlib.h>
 
#define BL_BLOCK_SIZE 1024
#define BL_MAX_OPEN_FILES 20
#define BL_MAX_CACHE_BLOCKS 20
#define BL_MAX_FILENAME_LEN 256


#define BLE_OK 0  //OK
#define BLE_NOMEM -10  //elleipsi mnimis
#define BLE_NOBUF -20  //elleipsi xwrou endiamesis mnimis
#define BLE_BLOCKFIXED -30  //block idi karfwmeno sti mnimi
#define BLE_BLOCKNOTINBUF -40  //block gia xsekarfwma den einai sti mnimi
#define BLE_BLOCKINBUF -50  //block idi sti mnimi
#define BLE_OS -60  //geniko sfalma Leitourgikou
#define BLE_INVALIDBLOCK -70  //mi egkiro block ID
#define BLE_FILEOPEN -80  //arxeio idi anoixto
#define BLE_FTABFULL -90  //lista anoixtwn arxeiwn pliris
#define BLE_OFTABFULL -100  //lista anoigmatwn arxeiwn pliris
#define BLE_FD -110  //mi egkiro block ID
#define BLE_EOF -120  //telso arxeiou
#define BLE_BLOCKFREE -130  //block idi diathesimo
#define BLE_BLOCKUNFIXED -140  //block idi xsekarfwmeno
#define BLE_FILERROR -150  //dika mas define - extra
#define BLE_FILENERASE -160  // -- " --
#define BLE_FILENOPEN -170 // -- " --
#define BLE_FILENEXIST -180  // -- " --
#define BLE_NONEXTFILE -190  // -- " --
#define BLE_FILEFULL -200  //gia AllocateBlock
#define BLE_FILENCLOSEPROPER -210  // -- " --
#define FileNameLength 20
#define blockSize 1024
#define openingSize 20
#define TRUE 1
#define FALSE 0
#define EMPTY -1 //gia na doume an enas pinakas einai kenos i oxi
#define ValidB '*'

typedef struct cacheBlock cacheBlock;
typedef struct openFiles openFilesInfo;

struct cacheBlock{

       char data[blockSize];  //array me 1024 xaraktires gia block
       int pins[openingSize];  //array gia pins
       int ID;  //anagnwristiko gia to block
       int fileNamePointer; //gia onoma arxeiou
       int modified; //modified gia 0 i 1
       char FileName[openingSize];//Onoma arxeiou
       long int timeStamp;

};// struct gia ta block tis cache

struct openFiles{
       
       char fileName[FileNameLength];  //filename
       char byteMap[blockSize]; //data gia header
       FILE* fileHandler;  //handler 

};// struct gia ta openfiles

extern long int timeCounter;

extern int appOpenings[openingSize]; //app-openings array

extern openFilesInfo OpenFile[openingSize]; 

extern cacheBlock CacheArray[openingSize];

extern int BL_errorNUM;


int BL_Init(void);

int BL_CreateFile(char *filename);

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
