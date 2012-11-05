#include <stdio.h>
#include <stdlib.h>
#include "BL.h"

int BL_Init(void) { 
    int i,j;
    for(i=0;i<openingSize;i++){ 
        appOpenings[i]=EMPTY; 
        OpenFile[i].fileHandler=NULL;   
        CacheArray[i].modified=FALSE;
        CacheArray[i].fileNamePointer=EMPTY;
        CacheArray[i].ID=EMPTY;
        CacheArray[i].timeStamp=EMPTY;
        for(j=0;j<openingSize;j++){
            CacheArray[i].pins[j]=EMPTY;
        }                            
    }
    timeCounter=FALSE;
    return BLE_OK;
}

int compareStrings(char* fileName1,char* fileName2){
     int j;
     int counter=0;
     for(j=0;j<FileNameLength;j++){
            if((fileName1[j]!=fileName2[j])){  //ELEGXOS XARAKTIRA-XARAKTIRA
              
                counter++;
            }
            if((counter>0)||((fileName1[j]==EOF)&&(fileName1[j]==fileName2[j]))){
                break;
            }
            else if(((fileName2[j]==EOF)||(fileName1[j]==EOF))&&(fileName1[j]!=fileName2[j])){
                counter=2; 
                break;    
            }            
        }
        if(counter!=0){
            return FALSE;
        }
        else{
            return TRUE;
        }
}

int UpdateFiles(int openFilesPointer){//kanei afto pou leei
    int i,j;
    for(i=0;i<openingSize;i++){
        if(CacheArray[i].fileNamePointer==openFilesPointer){
            if(CacheArray[i].modified==TRUE){
                    fseek(OpenFile[CacheArray[i].fileNamePointer].fileHandler,(/*OpenFile[*/CacheArray[i]/*.fileNamePointer]*/.ID+1)*blockSize,SEEK_SET);
                    for(j=0;j<blockSize;j++){
                        fputc(CacheArray[i].data[j],OpenFile[CacheArray[i].fileNamePointer].fileHandler);
                    }
            }
            
        }
    }
    
    return BLE_OK;
}
int TraverseCacheForModify(int openFilesPointer){//Function that traverses Cache Array and Checks if a specific file's blocks have been modified
    int i;
    for(i=0;i<openingSize;i++){
        if(CacheArray[i].fileNamePointer==openFilesPointer){
            if(CacheArray[i].modified==TRUE){
                    return TRUE;
                
            }
            
        }
    }
   
    return FALSE;

}
int TraverseCacheForEmptyBlock(){
    int i;
    for(i=0;i<openingSize;i++){
        if(CacheArray[i].ID==EMPTY){
           return i;            
        }
    }
   
    return EMPTY;

}

int TraverseCacheForClose(int openFilesPointer){//Function that traverses Cache Array and Checks if a specific file's blocks are in use
    int i,j;
    int counter=0;
    for(i=0;i<openingSize;i++){
        if(CacheArray[i].fileNamePointer==openFilesPointer){
            for(j=0;j<openingSize;j++){
                if(CacheArray[i].pins[j]==TRUE){
                    return FALSE;
                }
            }
            
        }
    }
   
    return TRUE;

}

int TraverseCacheForBlock(int openFilesPointer,int ID,char* fileName){//Function that traverses Cache Array and Checks if a specific file's blocks are in use
    int i;
    //int counter=0;
    for(i=0;i<openingSize;i++){
        if(compareStrings(CacheArray[i].FileName,fileName)){
           if(CacheArray[i].ID==ID){
               return i;
           }
            
        }
    }
   
    return EMPTY;

}
void removeFileFromCurentUse(int openFilesPointer){//Function that removes a files blocks and 
    int i;
    for(i=0;i<openingSize;i++){
        if(CacheArray[i].fileNamePointer==openFilesPointer){
            CacheArray[i].fileNamePointer=EMPTY;
            CacheArray[i].ID=EMPTY;
        }
    }
    fclose(OpenFile[openFilesPointer].fileHandler);
    OpenFile[openFilesPointer].fileHandler=NULL;
    
}

int LRU(){
    int i,j,k,position;
    int counter;
    for(i=0;i<openingSize;i++){
        counter=0;
        for(j=0;j<openingSize;j++){
            if(CacheArray[i].pins[j]!=EMPTY){
                counter++;
                break;
            }
        
        }
        if(counter==0){
            long int minTimeStamp=timeCounter;
             
            for(k=0;k<openingSize;k++){
                if(minTimeStamp>CacheArray[k].timeStamp){
                    minTimeStamp=CacheArray[k].timeStamp;
                    position=k;
                }
            
            }
            return position;
        }
        
    }
    BL_errorNUM=BLE_NOMEM;
    return BLE_NOMEM;
}

int WriteToAppOpenings(){//Function that returns the first empty part of appOpenings array
    int i;
    for(i=0;i<openingSize;i++){
        if(appOpenings[i]==-1){
            return i;
        }
    }
    return EMPTY;
}
int WriteToOpenFile(){//Function that returns the first empty part of appOpenings array
    int i;
    for(i=0;i<openingSize;i++){
        if(OpenFile[i].fileHandler==NULL){
            return i;
        }
    }
    return EMPTY;
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


int GetFilenamPositon(char *filename){
    int i,j;
  //  int counter; //METRITIS GIA DIEUKOLINSI
    for(i=0;i<openingSize;i++){
      //  counter=0;
       /* for(j=0;j<FileNameLength;j++){
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
            
        }*/
        
        if(compareStrings(filename,OpenFile[i].fileName)){
                return i;
        }        
    }
    return EMPTY;

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
            if((counter>0)||((filename[j]==EOF)&&(OpenFile[i].fileName[j]==filename[j]))){
                break;
            }
            else if(((filename[j]==EOF)||(OpenFile[i].fileName[j]==EOF))&&(OpenFile[i].fileName[j]!=filename[j])){
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
        file=fopen(filename,"a");//To ekana apo "w" se "a"
        int i;
        for(i=0;i<blockSize;i++){
            fputc((char)NULL,file);  //ARXIKOPOISI TOU BLOCK HEADER
        }
        fclose(file);
        return BLE_OK;
    }
        
    else{
         BL_errorNUM=BLE_FILERROR;
        return BLE_FILERROR;
    }
}




int BL_DestroyFile(char *filename) {
    if(FileExists(filename)==TRUE){
        if(FileIsOpen(filename)){
            if(!remove(filename))  //DIAGRAFI ARXEIOU
                return BLE_OK;
        
            else
            BL_errorNUM=BLE_FILENERASE;
                return  BLE_FILENERASE; 
        } 
        else
        BL_errorNUM=BLE_FILENOPEN;
            return BLE_FILENOPEN;                      
    }
    else
    BL_errorNUM=BLE_FILENEXIST;
        return BLE_FILENEXIST;
}

int BL_OpenFile(char *filename) {//Function that opens a file the name of witch is given as a parameter if possible
    int i,j;
    FILE* fileHandler;
    if(FileExists(filename)){
        if(FileIsOpen(filename)){
            if((i=WriteToAppOpenings())!=EMPTY){                                            
                appOpenings[i]=GetFilenamPositon(filename);
                return i;
            }        
            else{
                 BL_errorNUM=BLE_OFTABFULL;
                 return BLE_OFTABFULL;               
            }
        }
        else{        
             if((i=WriteToOpenFile())!=EMPTY){
                 fileHandler=fopen(filename,"r");        
                 OpenFile[i].fileHandler=fileHandler;
                 for(j=0;j<FileNameLength;j++){      
                     if(filename[j]==EOF){
                         break;
                     }          
                     OpenFile[i].fileName[j]=filename[j];
                 }
                 for(j=0;j<blockSize;j++){
                     OpenFile[i].byteMap[j]=fgetc(fileHandler);
                 }            
             } 
             else{
                   BL_errorNUM=BLE_OFTABFULL;
                 return BLE_OFTABFULL;               
             } 
             if((i=WriteToAppOpenings())!=EMPTY){                                            
                 appOpenings[i]=GetFilenamPositon(filename);
                 return i;
             }
             else{
                   BL_errorNUM=BLE_OFTABFULL;
                 return BLE_OFTABFULL;               
            }     
        }   
    } 
}

int BL_CloseFile(int openDesc) {
    int i;
    if(appOpenings[openDesc]==EMPTY){
        BL_errorNUM=BLE_FD;
        return BLE_FD;
    }
    else{
        for(i=0;i<openingSize;i++){
            if(CacheArray[i].fileNamePointer==appOpenings[openDesc]){
                if(CacheArray[i].pins[openDesc]==FALSE){
                    BL_errorNUM=BLE_BLOCKUNFIXED;
                    return BLE_BLOCKUNFIXED;
                }
                else if(CacheArray[i].pins[openDesc]==TRUE){
                    CacheArray[i].pins[openDesc]=FALSE;                  
                }
            }
        }
        if(TraverseCacheForClose(appOpenings[openDesc])){
            if(TraverseCacheForModify(appOpenings[openDesc])){//ELEGXOUME AN EGINAN MODIFY TA DATA
                UpdateFiles(appOpenings[openDesc]);//KANOUME UPDATE TO KATALILO FILE
            }
            removeFileFromCurentUse(appOpenings[openDesc]);
           
        }
    }
    return BLE_OK;
    
}

int BL_GetFirstBlock(int openDesc) {
        
    return BL_GetNextBlock(openDesc,0);  //uses get next block for finding the first one (isos na thelei -1)
        
}
    
int BL_GetNextBlock(int openDesc, int blockNum) {
        
        
    while(TRUE) {
        if (blockNum>=blockSize){
            BL_errorNUM=BLE_NONEXTFILE;
            return BLE_NONEXTFILE;   
        }
            
        blockNum++;
        if(OpenFile[appOpenings[openDesc]].byteMap[blockNum]==ValidB)  //elegxei to bytemap gia to arxeio
            return blockNum;                              //kai na doume an arxikopoioume kapou to bytemap.
            
        
        
    }
}

void initBlock(int openDesc, int blockNum,int cacheArrayPosition){
    int i;
    CacheArray[cacheArrayPosition].fileNamePointer=appOpenings[openDesc];
    CacheArray[cacheArrayPosition].ID=blockNum;
    for(i=0;i<openingSize;i++){
        CacheArray[cacheArrayPosition].FileName[i]=OpenFile[appOpenings[openDesc]].fileName[i];
    }
    CacheArray[cacheArrayPosition].modified=FALSE;
    CacheArray[cacheArrayPosition].pins[openDesc]=TRUE;
    fseek(OpenFile[appOpenings[openDesc]].fileHandler,(blockNum+1)*blockSize,SEEK_SET);
    for(i=0;i<blockSize;i++){
        CacheArray[cacheArrayPosition].data[i]=fgetc(OpenFile[appOpenings[openDesc]].fileHandler);
    }
    CacheArray[cacheArrayPosition].timeStamp=timeCounter;
    ++timeCounter; 
}

int BL_BeginBlock(int openDesc, int blockNum, char **blockBuf) {
    int i,j;
    if((i=TraverseCacheForBlock(/*OpenFile[*/openDesc/*]*/,blockNum,OpenFile[openDesc].fileName))!=EMPTY){
        *blockBuf=CacheArray[i].data;
        for(j=0;j<openingSize;j++){
            if(CacheArray[i].pins[j]==EMPTY){//RE MALAKES CHECK HERE!!!!!!!
                CacheArray[i].pins[j]=openDesc;
            }
        }
        return BLE_OK;
    }
    else{//RE MALAKES NA KANOUME ENIMEROSI TOU KATALLILOU ARXEIOU...
        if(OpenFile[appOpenings[openDesc]].byteMap[blockNum]==ValidB){
                                                         
            if((j=TraverseCacheForEmptyBlock())!=EMPTY){
                initBlock(openDesc,blockNum,j);
                *blockBuf=CacheArray[j].data;
                BL_errorNUM=BLE_OK;
                return BLE_OK;                
            }
            else{
                if((i=LRU())==BLE_NOMEM){
                    
                    return i;
                }
                else{
                     
                    initBlock(openDesc,blockNum,i);
                    *blockBuf=CacheArray[j].data;
                    BL_errorNUM=BLE_OK;
                    return  BLE_OK;                  
                }
               
            }
        }
        else{
             BL_errorNUM=BLE_INVALIDBLOCK;
            return BLE_INVALIDBLOCK;
        }
        
    }
    
}

int BL_AllocateBlock(int openDesc) {
    int i,j;
    for(i=0;i<blockSize;i++){
        if(OpenFile[openDesc].byteMap[i]==(char)NULL){
            if(i!=(blockSize-1)){
                                
                if(OpenFile[openDesc].byteMap[i+1]==ValidB){//check ti na valw dw anti gia true
                   OpenFile[openDesc].byteMap[i]=ValidB;
                   return i;
                }
                else if(OpenFile[openDesc].byteMap[i+1]==(char)NULL){//to i+1 parakatw paei epidi praktika den metrame to header block stis xrisimes plirofories kai to arxio mas thane 1025 KBytes praktika
                    fseek(OpenFile[openDesc].fileHandler,(i+1)*blockSize,SEEK_SET);//paizi na prepei na elegxoume kai gia lathei kai aftes edw...(tis fseek();
                    for(j=0;j<blockSize;j++){
                        fputc((char)NULL,OpenFile[openDesc].fileHandler);
                    }
                    OpenFile[openDesc].byteMap[i]=ValidB;
                    return i;
                }
            }
            else{
                 fseek(OpenFile[openDesc].fileHandler,0,SEEK_END);
                 for(j=0;j<blockSize;j++){
                     fputc((char)NULL,OpenFile[openDesc].fileHandler);
                 }
                 OpenFile[openDesc].byteMap[i]=ValidB;
                 return i;
                
            }
            
        }
    }
    BL_errorNUM=BLE_FILEFULL;
    return BLE_FILEFULL;
    
}

int BL_DisposeBlock(int openDesc, int blockNum) {
    int i;
    for(i=0;i<openingSize;i++){
        if(CacheArray[i].fileNamePointer==openDesc){
            if(CacheArray[i].ID==blockNum){
                BL_errorNUM=BLE_BLOCKFIXED;
                return BLE_BLOCKFIXED;
            }
        }
    }
    OpenFile[openDesc].byteMap[blockNum]=(char)NULL;
    BL_errorNUM=BLE_OK;
    return BLE_OK;
    
}
int NotifyModifiedBlockInFile(int openDesc, int blockNum, int modified){
    int i,k;
    int j=0;
    for(i=0;i<openingSize;i++){
        if((CacheArray[i].ID==blockNum)&&(compareStrings(CacheArray[i].FileName,OpenFile[appOpenings[openDesc]].fileName))){
            k=i;                           
            if(CacheArray[i].pins[openDesc]==TRUE){
                j++;
               break;
            } 
        }
    }
    if(j!=0){
        return EMPTY;
    }
    else{
         if(modified==TRUE){
                            
            fseek(OpenFile[appOpenings[openDesc]].fileHandler,(blockNum+1)*blockSize,SEEK_SET);
            for(i=0;i<blockSize;i++){
                fputc(CacheArray[k].data[i],OpenFile[appOpenings[openDesc]].fileHandler);
            }
            return TRUE;
         }
         else{
             return FALSE;
         }
    }
    


}

int BL_EndBlock(int openDesc, int blockNum, int modified) {
    int i,j;
    for(i=0;i<openingSize;i++){

        if((CacheArray[i].pins[openDesc]==TRUE)&&(CacheArray[i].ID==blockNum)){
            CacheArray[i].pins[openDesc]=FALSE;
            if(CacheArray[i].modified!=TRUE){
                CacheArray[i].modified=modified;
            }
            break;
        }    

    }
    
    
}

void BL_PrintError(char *errorString) {
     printf("%s",errorString);
     switch(BL_errorNUM){
         case BLE_NOBUF:
              printf("No buffer space!");
              
              break;
              
         case BLE_BLOCKFIXED:
              printf("Block already pined in Cache!");
              break;
              
         case BLE_BLOCKNOTINBUF:
              printf("Block is not pined in Cache!");
              break;
              
         case BLE_BLOCKINBUF:
              printf("Block already in Cache!");
              break;
              
         case BLE_OS:
              printf("Operating system general error!");
              break;
              
         case BLE_INVALIDBLOCK:
              printf("Non valid block ID!");
              break;
              
         case BLE_FILEOPEN:
              printf("File already open!");
              break;
         case BLE_FTABFULL:
              printf("Open file array full!");
              break;
              
         case BLE_OFTABFULL:
              printf("App openings array full!");
              break;
              
         case BLE_FD:
              printf("Non valid Open files position!");
              break;
              
         case BLE_EOF:
              printf("End Of File reached!");
              break;
              
         case BLE_BLOCKFREE:
              printf("Block already available!");
              break;
              
         case BLE_BLOCKUNFIXED:
              printf("Block already unpined!");
              break;
         case BLE_FILERROR:
              printf("General file error!");
              break;
              
         case BLE_FILENERASE:
              printf("File not erased!");
              break;
              
         case BLE_FILENOPEN:
              printf("File not open!");
              break;
              
         case BLE_FILENEXIST:
              printf("File does not exist!");
              break;
              
         case BLE_NONEXTFILE:
              printf("No next block!");
              break;
              
         case BLE_FILEFULL:
              printf("File is full!");
              break;
              
         case BLE_FILENCLOSEPROPER:
              printf("File did not close properly!");
              break;   
     
     }
     
     
}

int BL_CleanUp(void) {
    int i;
    int j=FALSE;
    for(i=0;i<openingSize;i++)
    {
        if((OpenFile[i].fileHandler!=NULL)&&(fclose(OpenFile[i].fileHandler)==EOF)){
            j++;
        }
    
    }
    if(j!=FALSE){
        BL_errorNUM=BLE_FILENCLOSEPROPER;
        return BLE_FILENCLOSEPROPER;
    }
    else{
         
        return BLE_OK;
   }
    

}
