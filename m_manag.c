#include <stdio.h>
#include <stdlib.h>
#include "BL.h"

int BL_Init(void) {  //������������
    int i,j,k;
    for(i=0;i<openingSize;i++){   
        appOpenings[i]=EMPTY; 
        OpenFile[i].fileHandler=NULL;   
        CacheArray[i].modified=FALSE;
        CacheArray[i].fileNamePointer=EMPTY;
        CacheArray[i].ID=EMPTY;
        CacheArray[i].timeStamp=EMPTY;
         for(k=0;k<blockSize;k++){
            OpenFile[i].byteMap[k]=(char)NULL;
        }
        for(j=0;j<openingSize;j++){
            CacheArray[i].pins[j]=EMPTY;
            
        }                            
    }
    timeCounter=FALSE;
    return BLE_OK;
}

int compareStrings(char* fileName1,char* fileName2){  //�������� ���������-��������� ��� �� ������� ��� �������
     int j;
     int counter=0;  //��������
     for(j=0;j<FileNameLength;j++){
            if((fileName1[j]!=fileName2[j])){  //������� ��� ������ �� ��� ����� ����
                counter++;  //��������� �� �������
            }
            if((counter>0)||((fileName1[j]==EOF)&&(fileName1[j]==fileName2[j]))){
                break;
            }
            else if(((fileName2[j]==EOF)||(fileName1[j]==EOF))&&(fileName1[j]!=fileName2[j])){
                counter=2;  //��������� ��� ���� ������� ��� 0
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

int UpdateFiles(int openFilesPointer){  //update ��� �������
    int i,j;
    fclose(OpenFile[openFilesPointer].fileHandler);   
    FILE* filebuf=fopen(OpenFile[openFilesPointer].fileName,"rb+");   
    for(i=0;i<openingSize;i++){
        if(CacheArray[i].fileNamePointer==openFilesPointer){
            if(CacheArray[i].modified==TRUE){  //�� ���� ����� modified
                    fseek(filebuf,(CacheArray[i].ID+1)*blockSize,SEEK_SET);
                    for(j=0;j<blockSize;j++){
                        fputc(CacheArray[i].data[j],OpenFile[CacheArray[i].fileNamePointer].fileHandler);
                    }
            }
            
        }
    }
    fclose(filebuf);
    OpenFile[openFilesPointer].fileHandler=fopen(OpenFile[openFilesPointer].fileName,"rb");
    return BLE_OK;  //������������ OK
}

int TraverseCacheForModify(int openFilesPointer){
//��������� Cache ��� �������� �� �� block ����� ����� modified
    int i;
    for(i=0;i<openingSize;i++){
        if(CacheArray[i].fileNamePointer==openFilesPointer){  
            if(CacheArray[i].modified==TRUE){  //�� ���� ����� modified
                    return TRUE;
                
            }
            
        }
    }
   
    return FALSE;

}
int TraverseCacheForEmptyBlock(){  //��������� ��� �� ���� ����� block ���� Cache
    int i,j;
    for(j=0;j<openingSize;j++){  
        if(CacheArray[j].ID==EMPTY){  //�� �� ������������� ����� EMPTY 
            return j;  //���������� �� ���� ��� block
        }
    }
    for(i=0;i<openingSize;i++){  //����������� Cache
        
        if((CacheArray[i].fileNamePointer==EMPTY) ){  //�� fileNamePointer ����� EMPTY 
                                                                
           return i;  //���������� �� ���� ���  block           
        }
    }   
    return EMPTY;
}

int TraverseCacheForClose(int openFilesPointer){  //��������� ��� �������� �� �� block ���� ������� ����� �� �����
    int i,j;
    int counter=0;  //��������
    for(i=0;i<openingSize;i++){  //��������� CacheArray
        if(CacheArray[i].fileNamePointer==openFilesPointer){ 
            for(j=0;j<openingSize;j++){  //��������� pins
                if(CacheArray[i].pins[j]==TRUE){  //�� ����� ������������
                    return FALSE;
                }
            }
            
        }
    }
   
    return TRUE;

}

int TraverseCacheForBlock(int openFilesPointer,int ID,char* fileName){
//��������� Cache Array ��� �������� �� ��� ������������ block ��� ������� ���������������
    int i;
    for(i=0;i<openingSize;i++){  //����������� CacheArray
        if(compareStrings(CacheArray[i].FileName,fileName)){  //����������� ������� 
           if((CacheArray[i].ID==ID)&&(CacheArray[i].fileNamePointer==openFilesPointer)){  //�� ������ �� ������������� block
               return i;  //������������ �� ���� ���
           }
            
        }
    }
   
    return EMPTY;

}

void removeFileFromCurentUse(int openDesc){  //������� ���������� ������ 
    int i;
    for(i=0;i<openingSize;i++){  //����������� CacheArray
        if(CacheArray[i].fileNamePointer==appOpenings[openDesc]){  //���� �� block ��� CacheArray ������ ��� ������ OpenFile[appOpenings[OpenDesc]] 
            CacheArray[i].fileNamePointer=EMPTY;  //�� ����������� 
        }
       
    }
    fclose(OpenFile[appOpenings[openDesc]].fileHandler);  //��������� �� ������ 
    OpenFile[appOpenings[openDesc]].fileHandler=NULL;  //��� ������� NULL
    for(i=0;i<blockSize;i++){  //����������� byteMap
          if(i<openingSize){  //���� openingSize ������ �����������
             OpenFile[appOpenings[openDesc]].fileName[i]=(char)NULL;  //"�����������" �� �����                  
          }                         
          OpenFile[appOpenings[openDesc]].byteMap[i]=(char)NULL;  //����������� byteMap
    }
          
    appOpenings[openDesc]=EMPTY;  //����������� appOpenings
    
    
}

int LRU(){  //LRU ��� ��� ������ ��� ��������� ����������������
    int i,j,k,position;
    int counter;
    for(i=0;i<openingSize;i++){  //����������� ��� CacheArray
        counter=0;  //��������
        for(j=0;j<openingSize;j++){
            if(CacheArray[i].pins[j]!=EMPTY){  //�� ���� "�������������" | ����������� pins
                counter++;  //��������� �������
                break;  //��� break
            }
        
        }
        if(counter==0){  //�� ����� 0
            long int minTimeStamp=timeCounter;  //��������� ��� ���� ��� timeCounter ��� minTimeStamp
             
            for(k=0;k<openingSize;k++){  //����������� CacheArray
                if(minTimeStamp>CacheArray[k].timeStamp){   //�������� �� ������ ��� ��������� ���� ��� TimeStamp
                    minTimeStamp=CacheArray[k].timeStamp;  //�� ������ ��� ���������
                    position=k;  //������������ �� ���� ��� CahceArrayX ��� ������� � ���������� ������
                }
            
            }
            return position;
        }
        
    }
    BL_errorNUM=BLE_NOMEM;
    return BLE_NOMEM;  //������������ error
}

int WriteToAppOpenings(){  //���������� �� ����� ���� ��� �� ���� ���� appOpenings
    int i;
    for(i=0;i<openingSize;i++){  //��������� appaOpenings
        if((appOpenings[i]==EMPTY)){  //�� ���� EMPTY 

            return i;  //���������� �� ���� ��� �� �����
        }
    }
    
    return EMPTY;
}
int WriteToOpenFile(){  //���������� �� ����� ���� ��� �� ���� ���� OpenFile
    int i;
    for(i=0;i<openingSize;i++){  //��������� ��� OpenFile
        if(OpenFile[i].fileHandler==NULL){  //�� ���� NULL 
            return i;  //���������� �� ���� ���� �� �����
        }
    }
    return EMPTY;
}

int FileExists(char* filename){  //������� ������� �������
    FILE * file=fopen(filename,"rb");  //�� ��������� ��� �������� ��� �� ����� �� ������� � ���
    if(file==NULL){  //�� ����� NULL 
        fclose(file);  //�� ���������
        return FALSE;
    }
        
    else{
        fclose(file);  //�� ���������
        return TRUE;   
    }
}


int GetFilenamPositon(char *filename){  //��� ���������� �� ���� ��� ������� ���� OpenFile
    int i,j;
    for(i=0;i<openingSize;i++){  //��������� �� OpenFile
        if(compareStrings(filename,OpenFile[i].fileName)){  //��������� �� �������
                return i;  //���������� �� ���� ��� �������
        }        
    }
    return EMPTY;

}

int FileIsOpen(char *filename){  //������� ��� ������� ������
    int i,j;
    int counter; //��������
    for(i=0;i<openingSize;i++){  //����������� OpenFile
        counter=0;
        for(j=0;j<FileNameLength;j++){
            if((OpenFile[i].fileName[j]!=filename[j])){  //������� ���������-��������� | �� ��� ����� ����
              
                counter++;  //��������� ��� �������
            }
            if((counter>0)||((filename[j]==EOF)&&(OpenFile[i].fileName[j]==filename[j]))){  //�������� �������
                break;
            }
            else if(((filename[j]==EOF)||(OpenFile[i].fileName[j]==EOF))&&(OpenFile[i].fileName[j]!=filename[j])){  
            //�� 1 ��� �� 2 ������� ��������� �������� ��� � EOF ��� ����� ����� ��� ����� 2 
                counter=2;  //����� ������� ��� ���� ������� ��� 0 ���� counter
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
    if(FileExists(filename)==FALSE){  //�� ��� ������� �� ������
        file=fopen(filename,"wb");  //�� ��������� ��� �������
        
        int i;
        for(i=0;i<blockSize;i++){  //����������� byteMap
            fputc((char)NULL,file);  //������������ ��� block header
        }
        fclose(file);  //��������� �� ������
        return BLE_OK;  //������������ ��
    }
        
    else{  //������
         BL_errorNUM=BLE_FILERROR;
        return BLE_FILERROR;  //������������ error
    }
}




int BL_DestroyFile(char *filename) {
    if(FileExists(filename)==TRUE){  //�� ������� �� ������
            if(!remove(filename))  //����� � remove ���������� 0 �� ��������� ���������, �� NOT 0 �� ����� 1, ��� �� ��������
                return BLE_OK;  //������������ ��
        
            else{
               BL_errorNUM=BLE_FILENERASE;
                return  BLE_FILENERASE;  //������ �����
            }                  
    }
    else{  //������
        BL_errorNUM=BLE_FILENEXIST;
        return BLE_FILENEXIST;  //error
    }
}

int BL_OpenFile(char *filename) {//Function that opens a file the name of witch is given as a parameter if possible
    int i,j;
    FILE* fileHandler;
    if(FileExists(filename)){
        if(FileIsOpen(filename)){
            if((i=WriteToAppOpenings())!=EMPTY){  //�� ������� ����� ���� ������ appOpenings                                           
                appOpenings[i]=GetFilenamPositon(filename);  //����������� ��� ���� i ��� appOp ��� ���� ���� ������ OpenFile
                return i;  //���������� �� ������������� ��� ������
            }        
            else{
                 BL_errorNUM=BLE_OFTABFULL;
                 return BLE_OFTABFULL;  //������������ error               
            }
        }
        else{  //�� �� ������ ��� ����� �������        
             if((i=WriteToOpenFile())!=EMPTY){  //�� ������� ����� ���� ������ appOpenings 
                 fileHandler=fopen(filename,"rb");  //��������� �� ������ ��� ��������      
                 OpenFile[i].fileHandler=fileHandler;  //������������ ��� file handler ��� ������� ���� fileHandler
                 for(j=0;j<FileNameLength;j++){      
                     if(filename[j]==EOF){  //�� �� ����� ���� ���������
                         break;  //��������� ��� �� for
                     }          
                     OpenFile[i].fileName[j]=filename[j];  //������������ �� ����� ��� fileName
                 }
                 fseek(fileHandler,0,SEEK_SET);  //����������� ��� file pointer ���� ���� ��� �������
                 for(j=0;j<blockSize;j++){
                     OpenFile[i].byteMap[j]=fgetc(fileHandler);  //���������� �� header block
                 }     
             } 
             else{
                   BL_errorNUM=BLE_OFTABFULL;
                 return BLE_OFTABFULL;  //������������ error               
             } 
             if((i=WriteToAppOpenings())!=EMPTY){                                            
                 appOpenings[i]=GetFilenamPositon(filename);  //����������� ��� ���� i ��� appOp ��� ���� ���� ������ OpenFile
                 return i;
             }
             else{
                   BL_errorNUM=BLE_OFTABFULL;
                 return BLE_OFTABFULL;  //������������ error              
            }     
        }   
    } 
    return BLE_OK;  //������������ OK
}

int BL_CloseFile(int openDesc) {
    int i,j,k;
    FILE* fileBuff;
    if(appOpenings[openDesc]==EMPTY){  //�� ����� ������ � ������� ���������� ��� ������������ openDesc
        BL_errorNUM=BLE_FD;
        return BLE_FD;  //������������ error
    }
    else{
        for(i=0;i<openingSize;i++){  //����������� CcacheArray
           if(CacheArray[i].fileNamePointer==appOpenings[openDesc]){   //���� �� block ��� CacheArray ������ ��� ������ OpenFile[appOpenings[OpenDesc]]
                if((CacheArray[i].pins[openDesc]!=EMPTY)){  //�� �� pins ��� ����� "��������������"
                    BL_errorNUM=BLE_BLOCKUNFIXED;
                     
                    return BLE_BLOCKUNFIXED;  //������������ error
                }
            }
        }
        if(TraverseCacheForClose(appOpenings[openDesc])){  
            fclose(OpenFile[appOpenings[openDesc]].fileHandler);  //��������� �� ������, �� ����� ������ ������� ��� ��������, ����� ���������� �� �������� �� ����
            fileBuff=fopen(OpenFile[appOpenings[openDesc]].fileName,"rb+"); //��������� �� ������ ��� �������
            fseek(OpenFile[appOpenings[openDesc]].fileHandler,0,SEEK_SET);
            //����������� ��� filePointer ���� ��������� ���� 
            for(k=0;k<blockSize;k++){
                fputc(OpenFile[appOpenings[openDesc]].byteMap[k],fileBuff);  //��������� �������� bytes ��� block ���� 
            
            }
           
            fclose(fileBuff);  //��������� �� ������ 
            OpenFile[appOpenings[openDesc]].fileHandler=fopen(OpenFile[appOpenings[openDesc]].fileName,"rb");  //��� ��� �� ��������� ��� ��������
            if(TraverseCacheForModify(appOpenings[openDesc])){  //��������� �� ������ modify �� data
                UpdateFiles(appOpenings[openDesc]);  //������� update �� ��������� ������ �� ��������� ��� ������ modify �� data
            }
            removeFileFromCurentUse(openDesc);  //��������� �� ������
            
           
        }
    }
    return BLE_OK;  //������������ OK
    
}

int BL_GetFirstBlock(int openDesc) {
        
    return BL_GetNextBlock(openDesc,0);  //������������ ��� get next block ��� �� ���� �� ����� ������
        
}
    
int BL_GetNextBlock(int openDesc, int blockNum) {
    while(TRUE) {  //while
        if (blockNum>=blockSize){  //�� �� blockNum ��� ��� ����� ����� >= ��� 1024
            BL_errorNUM=BLE_NONEXTFILE;
            return BLE_NONEXTFILE;  //������������ error
        }
                   
        blockNum++;  //��������� �� blockNum ��� �� ���� ��� ������� ������
        if(OpenFile[appOpenings[openDesc]].byteMap[blockNum]==ValidB){  //��������� ��� �� ����� �� �� block ����� ������
            return blockNum;  //������������ �� �������������                      
            
            }      
    }
}
int CheckForValidBlock(int openDesc,int blockNum){
    int i,j,l;
    j=0;
    for(i=0;i<blockSize;i++){  //����������� byteMap
        l=BL_GetNextBlock(openDesc,i);  //�� l ��� ����� �� ������� ������ block
        if(l>0){  //�� ����� >0
            i=l;  //��������� ��� i �� ������������� ��� ������� block
        }   
        if(l<EMPTY){  //������
            return l;  //���������� �� ����
        }     
        if(i!=BLE_NONEXTFILE){  //������ �� ����� ������� ��� error
            j++;//��������� ���� 1
            if(j==blockNum){  //�� � �������� ����� ���� �� �� ������������� ��� block ��� ��������
                return i;  //������������ �� ��������������            
            }
           
        }   
    }
    return BLE_INVALIDBLOCK;  //������������ error
}

void initBlock(int openDesc, int blockNum,int cacheArrayPosition){
    int i;
     
    CacheArray[cacheArrayPosition].fileNamePointer=appOpenings[openDesc];
    CacheArray[cacheArrayPosition].ID=blockNum;
    for(i=0;i<openingSize;i++){
        CacheArray[cacheArrayPosition].FileName[i]=OpenFile[appOpenings[openDesc]].fileName[i];  
        //������������ �� ����� ��� �������, ��� ������ �� blockNum ������ ����������� ��� ���� CacheArrayPosition ��� Cache 
    }
    CacheArray[cacheArrayPosition].modified=FALSE;
    CacheArray[cacheArrayPosition].pins[openDesc]=TRUE; 
   
    fseek(OpenFile[appOpenings[openDesc]].fileHandler,(blockNum+1)*blockSize,SEEK_SET);
    //���� ��� ������������ block ��� ������� 
    for(i=0;i<blockSize;i++){  //����������� �� byteMap
        CacheArray[cacheArrayPosition].data[i]=fgetc(OpenFile[appOpenings[openDesc]].fileHandler);
        //���������� ��� ���������� ��� block ��� ��� ����������� ��� data ��� CacheArray
    }
    CacheArray[cacheArrayPosition].timeStamp=timeCounter;  //��������� ��� timeCounter � ����� ������� �� ����� ��� ����� load � ������ ���� Cache
    ++timeCounter;  //��� �� ���������
}

int BL_BeginBlock(int openDesc, int blockNum, char **blockBuf) {
    int i,j;
   
    i=TraverseCacheForBlock(appOpenings[openDesc],blockNum,OpenFile[appOpenings[openDesc]].fileName);
    if(i!=EMPTY){  //�� ������� �� block ��� ������� ���� Cache 
            if((CacheArray[i].pins[openDesc]==EMPTY)){  //�� � ������� openDesc ��� ���� ������� ��� �� ������������ block
                CacheArray[i].pins[openDesc]=TRUE;  //�� "�������������"
            }
            else{  //������
                return BLE_BLOCKFIXED;  //������������ error
            }
            *blockBuf=CacheArray[i].data;  //������� �������� ��� block ����������� ���� ���� CacheArray ��� ������ ���� ��� blockBuf
            return BLE_OK;  //������������ OK
    }
    else{  //�� ��� ������� 

       if(OpenFile[appOpenings[openDesc]].byteMap[blockNum]==ValidB){  //�� ������� �� �� byteMap, �� block �� ������������� blockNum, ����� ������
                                                         
            if((j=TraverseCacheForEmptyBlock())>=FALSE){  //�� ������ ��� ���� ���� ���� Cache

                initBlock(openDesc,blockNum,j);  //����������� ��� ���� ���� �� data ��� ������������� block
                *blockBuf=CacheArray[j].data;  //������� �������� ��� block ����������� ���� ���� CacheArray ��� ������ ���� ��� blockBuf

                return BLE_OK;  //������������ OK              
            }
            else{  //������
                if((i=LRU())==BLE_NOMEM){  //�� ������� ����� �� ��� LRU
                    return i;  //���������� error
                }
                else{
                     
                    initBlock(openDesc,blockNum,i);  //����������� ��� ��������� �� ��������������� block ��� Cache
                    *blockBuf=CacheArray[j].data;  //������� �������� ��� block ����������� ���� ���� CacheArray ��� ������ ���� ��� block
                    return  BLE_OK;  //������������ OK                  
                }
               
            }
        }
        else{
            
             BL_errorNUM=BLE_INVALIDBLOCK;
            return BLE_INVALIDBLOCK;  //������ ������������ error
        }
        
    }
    
}

int BL_AllocateBlock(int openDesc) {
    int i,j,k;
    FILE* fileBuff;  
    //�������������� �� fileBuff ��� �� ������������ �� ������ ��� ���������� ��������� ��� OpenFile, �� ����� �� ��������������� ��� ��� block 
    //������������ ��� ���� NULL ���� ���������� ���� ��� block ��� ������
    for(i=0;i<blockSize;i++){  //����������� �� byteMap
        if(OpenFile[appOpenings[openDesc]].byteMap[i]==(char)NULL){  //�� ������� �� �� byteMap, �� block ���� ��������� i ����� ��������
            if(i!=(blockSize-1)){  //��� ��� ����������� ���� ��������� ���������
                                
                if(OpenFile[appOpenings[openDesc]].byteMap[i+1]==ValidB){  //��� �� ������ ������� block, ������� �� �� byteMap, ����� ������
                   OpenFile[appOpenings[openDesc]].byteMap[i]=ValidB;  
                   //� ������ ��� ���� i ���������� ��� ����������, ��� �� block ���� ������� ��� ��� ������ ��� ���� �� ������� ������ 
                   return i;  //��� ������������ �� ���� ��� ��� �� byteMap
                }
                else if(OpenFile[appOpenings[openDesc]].byteMap[i+1]==(char)NULL){  //�� �� ������ ������� block, ������� �� �� byteMap, ����� �����
                    
                    fclose(OpenFile[appOpenings[openDesc]].fileHandler);  //��������� �� ������, �� ����� ������ ������� ��� ��������, ����� ���������� �� �������� �� ����
                    fileBuff=fopen(OpenFile[appOpenings[openDesc]].fileName,"rb+");  //��������� �� ������ ��� ������� 
                    fseek(fileBuff,(i+1)*blockSize,SEEK_SET);  //����������� ��� filePointer ���� ��������� ����
                    //(to i+1 parakatw paei epidi praktika den metrame to header block stis xrisimes plirofories kai to arxio mas thane 1025 KBytes praktika)
                    for(j=0;j<blockSize;j++){  //����������� �� byteMap
                        fputc((char)NULL,OpenFile[appOpenings[openDesc]].fileHandler);  //��������� �������� bytes ��� block ����  
                    }
                    OpenFile[appOpenings[openDesc]].byteMap[i]=ValidB;  //������� ������ �� block
                    fclose(fileBuff);  //��������� �� ������ 
                    OpenFile[appOpenings[openDesc]].fileHandler=fopen(OpenFile[appOpenings[openDesc]].fileName,"rb");  //��� ��� �� ��������� ��� ��������                      
        
                    return i;  //��� ������������ ��� ������ ��� �������
                }
            }
            else{  //������ ���� ����������� ���� ��������� ���������, ������ ����������� ��� block 1023 
                 fclose(OpenFile[appOpenings[openDesc]].fileHandler);  
                 //��������� �� ������, �� ����� ������ ������� ��� ��������, ����� ���������� �� �������� �� ����
                 fileBuff=fopen(OpenFile[appOpenings[openDesc]].fileName,"rb+");  //��������� �� ������ ��� �������
                 fseek(fileBuff,0,SEEK_END);  //����������� ��� filePointer ��� ����� ��� �������
                 for(j=0;j<blockSize;j++){  
                     fputc((char)NULL,OpenFile[openDesc].fileHandler);  //��������� �� ������ ���� 1 block 
                 }
                 OpenFile[appOpenings[openDesc]].byteMap[i]=ValidB;  //������� �� block ������
                 fclose(fileBuff);  //��������� �� ������ 
                 OpenFile[appOpenings[openDesc]].fileHandler=fopen(OpenFile[appOpenings[openDesc]].fileName,"rb");  //��� ��� �� ��������� ��� ��������  
                                   
                 return i;  //��� ������������ ��� ������ ��� �������
                
            }
            
        }
    }
    BL_errorNUM=BLE_FILEFULL;
    return BLE_FILEFULL;  //������������ error
    
}

int BL_DisposeBlock(int openDesc, int blockNum) {
    int i;
    for(i=0;i<openingSize;i++){  //����������� ��� CacheArray ��� �� ������ �� �� block ��� ������� �� "������������" ������� ��� �����
        if((CacheArray[i].fileNamePointer==appOpenings[openDesc])&&(compareStrings(OpenFile[appOpenings[openDesc]].fileName,CacheArray[i].FileName))){
        //�� �� block ��� ������ � ������� openDesc ��� �� ������� ��� ������� ����� ����                                                                                                                                             
            if(CacheArray[i].ID==blockNum){  //�� �� block ����� ���� ��� ��������
                BL_errorNUM=BLE_BLOCKFIXED;
                return BLE_BLOCKFIXED;  //������������ error
            }
        }
    }
  
    OpenFile[appOpenings[openDesc]].byteMap[blockNum]=(char)NULL;  //����������� �� byteMap ��� �� block ��� �������
    
    BL_errorNUM=BLE_OK;
    return BLE_OK;  //������������ ��
    
}
int NotifyModifiedBlockInFile(int openDesc, int blockNum, int modified){ //elegxei gia modified 
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
   
    for(i=0;i<openingSize;i++){  //����������� ��� ������ CacheArray ��� �� ��������� �� ������� ���� block �� ������������� blockNum ��� ���� ������� � ������� openDesc
        if((compareStrings(OpenFile[CacheArray[i].fileNamePointer].fileName,OpenFile[appOpenings[openDesc]].fileName))&&(CacheArray[i].pins[openDesc]==EMPTY)&&(CacheArray[i].ID==blockNum)){
        //����������� �� ������� ��� �������, �� ID �� �� blockNum ��� ��������� �� �� pins ����� "��������������" - EMPTY    
            BL_errorNUM=BLE_BLOCKUNFIXED; 
            return BLE_BLOCKUNFIXED;  //�� ��� ����� �� ������������ block, ������������ error
        }
        if((CacheArray[i].pins[openDesc]==TRUE)&&(CacheArray[i].ID==blockNum)){  //�� �� pins ����� "������������" ��� �� block ����� ���� ��� �������
            CacheArray[i].pins[openDesc]=EMPTY;  //"����������" �� pins
            if(CacheArray[i].modified!=TRUE){  //�� ��� ���� ������������ �� block ��� ����
                CacheArray[i].modified=modified;  //�� modified ������� ��� ���� ��� ����� � �������
            }
            return BLE_OK;  //��
        }    

    }
    return BLE_BLOCKNOTINBUF;  //������������ error
    
    
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
    for(i=0;i<openingSize;i++)  //����������� ��� ������ OpenFile ��� �� ������ ��� �� ��������� �� ������� ������
    {
        if((OpenFile[i].fileHandler!=NULL)&&(fclose(OpenFile[i].fileHandler)==EOF)){  //�� � fileHandler ��� ����� NULL ��� ���� �� �������� ��� ������� ����� ����� 
            j++;  //��������� �� j ���� 1 ��� ���� �����
        }
    
    }
    if(j!=FALSE){  //�� �� j ���� ���� ������� ��� 0
        BL_errorNUM=BLE_FILENCLOSEPROPER;  
        return BLE_FILENCLOSEPROPER;  //������������ error
    }
    else{
         
        return BLE_OK;  //������ OK
   }
    

}
