#include <stdio.h>
#include <stdlib.h>
#include "memory_management.h"

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

int BL_init(void) {

}

int BL_CreateFile(char *filename) {
    
    
}

int BL_DestroyFile(char *filename) {

}

int BL_OpenFile(char *filename) {
    
}

int BL_CloseFile(int openDesc) {
    
}

int BL_GetFirstBlock(int openDesc) {
    
}

int BL_GetNextBlock(int openDesc, int blockNum) {
    
}

int BL_BeginBlock(int openDesc, int blockNum, char **blockBuf) {
    
}

int BL_AllocateBlock(int openDesc) {
    
}

int BL_DisposeBlock(int openDesc, int blockNum) {
    
}

int BL_EndBlock(int openDesc, int blockNum, int modified) {
    
}

void BL_PrintError(char *errorString) {
     
}

int BL_CleanUp(void) {
    
}
     

