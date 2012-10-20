#include <stdio.h>
#include <stdlib.h>
#include "mem_man.h"



int BL_init(void) { 
    int i,j;
    for(i=0;i<openingSize;i++){ 
        appOpenings[i]=EMPTY; 
        OpenFile[i].fileHandler=NULL;   
        CacheArray[i].modified=NULL;
        CacheArray[i].fileNamePointer=EMPTY;
        CacheArray[i].ID=EMPTY;
        for(j=0;j<openingSize;j++){
            CacheArray[i].pins[j]=EMPTY;
        }                            
    }
}

int FileExists(char* filename){  //ELEGXOS IPARXIS ARXEIOU
    FILE * file=fopen(filename,"r"); 
    if(file==NULL){
        fclose(file);
        return FALSE;
    }
        
    else{
        fclose(file);
        return TRUE;   
    }
}

int FileIsOpen(char *filename){  //ELEGXOS GIA ANOIXTO ARXEIO
    int i,j;
    int counter; //METRITIS GIA DIEUKOLINSI
    for(i=0;i<openingSize;i++){
        counter=0;
        for(j=0;j<FileNameLength;j++){
            if((OpenFile[i].fileName[j]!=filename[j])){  //ELEGXOS XARAKTIRA-XARAKTIRA
              
                counter++;
            }
            if((counter>0)||((filename[j]==EOF)&&(OpenFile[i].fileName[j]==filename[j])){
                break;
            }
            else if(((filename[j]==EOF)||(OpenFile[i].fileName[j]==EOF))&&(OpenFile[i].fileName[j]!=filename[j]){
                counter=2; 
                break;    
            }
            
        }
        if(counter==0){
                return TRUE;
        }        
    }
    return FALSE;
}


int BL_CreateFile(char *filename) {
    FILE * file;
    if(FileExists(filename)==FALSE){
        file=fopen(filename,"w");
        int i;
        for(i=0;i<blockSize;i++){
            fputc(NULL,file);  //ARXIKOPOISI TOU BLOCK HEADER
        }
        fclose(file);
        return BLE_OK;
    }
        
    else{
        return BLE_FILERROR;
    }
}




int BL_DestroyFile(char *filename) {
    if(FileExists(filename)==TRUE){
        if(FileIsOpen()){
            if(!remove(filename))  //DIAGRAFI ARXEIOU
                return BLE_OK;
        
            else
                return  BLE_FILENERASE; 
        } 
        else
            return BLE_FILENOPEN;                      
    }
    else
        return BLE_FILENEXIST;
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
     

