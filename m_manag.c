#include <stdio.h>
#include <stdlib.h>
#include "BL.h"

int BL_Init(void) {  //αρχικοποίηση
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

int compareStrings(char* fileName1,char* fileName2){  //σύγκριση χαρακτήρα-χαρακτήρα για τα ονόματα των αρχείων
     int j;
     int counter=0;  //μετρητής
     for(j=0;j<FileNameLength;j++){
            if((fileName1[j]!=fileName2[j])){  //έλεγχος και βλέπει αν δεν είναι ίδια
                counter++;  //αυξάνουμε το μετρητή
            }
            if((counter>0)||((fileName1[j]==EOF)&&(fileName1[j]==fileName2[j]))){
                break;
            }
            else if(((fileName2[j]==EOF)||(fileName1[j]==EOF))&&(fileName1[j]!=fileName2[j])){
                counter=2;  //εκχωρούμε μία τιμή διάφορη του 0
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

int UpdateFiles(int openFilesPointer){  //update του αρχείου
    int i,j;
    fclose(OpenFile[openFilesPointer].fileHandler);   
    FILE* filebuf=fopen(OpenFile[openFilesPointer].fileName,"rb+");   
    for(i=0;i<openingSize;i++){
        if(CacheArray[i].fileNamePointer==openFilesPointer){
            if(CacheArray[i].modified==TRUE){  //αν έχει γίνει modified
                    fseek(filebuf,(CacheArray[i].ID+1)*blockSize,SEEK_SET);
                    for(j=0;j<blockSize;j++){
                        fputc(CacheArray[i].data[j],OpenFile[CacheArray[i].fileNamePointer].fileHandler);
                    }
            }
            
        }
    }
    fclose(filebuf);
    OpenFile[openFilesPointer].fileHandler=fopen(OpenFile[openFilesPointer].fileName,"rb");
    return BLE_OK;  //επιστρέφουμε OK
}

int TraverseCacheForModify(int openFilesPointer){
//διασχίζει Cache και τσεκάρει αν τα block έχουν γίνει modified
    int i;
    for(i=0;i<openingSize;i++){
        if(CacheArray[i].fileNamePointer==openFilesPointer){  
            if(CacheArray[i].modified==TRUE){  //αν έχει γίνει modified
                    return TRUE;
                
            }
            
        }
    }
   
    return FALSE;

}
int TraverseCacheForEmptyBlock(){  //διασχίζει για να βρει άδειο block στην Cache
    int i,j;
    for(j=0;j<openingSize;j++){  
        if(CacheArray[j].ID==EMPTY){  //αν το αναγνωριστικό είναι EMPTY 
            return j;  //επιστρέφει τη θέση του block
        }
    }
    for(i=0;i<openingSize;i++){  //διασχίζουμε Cache
        
        if((CacheArray[i].fileNamePointer==EMPTY) ){  //αν fileNamePointer είναι EMPTY 
                                                                
           return i;  //επιστρέφει τη θέση του  block           
        }
    }   
    return EMPTY;
}

int TraverseCacheForClose(int openFilesPointer){  //διασχίζει και τσεκάρει αν τα block ενός αρχείου είναι σε χρήση
    int i,j;
    int counter=0;  //μετρητής
    for(i=0;i<openingSize;i++){  //διασχίζει CacheArray
        if(CacheArray[i].fileNamePointer==openFilesPointer){ 
            for(j=0;j<openingSize;j++){  //διασχίζει pins
                if(CacheArray[i].pins[j]==TRUE){  //αν είναι καρφιτσωμένο
                    return FALSE;
                }
            }
            
        }
    }
   
    return TRUE;

}

int TraverseCacheForBlock(int openFilesPointer,int ID,char* fileName){
//διασχίζει Cache Array και τσεκάρει αν ένα συγκεκριμένο block του αρχείου χρησιμοποιείται
    int i;
    for(i=0;i<openingSize;i++){  //διασχίζουμε CacheArray
        if(compareStrings(CacheArray[i].FileName,fileName)){  //συγκρίνουμε ονόματα 
           if((CacheArray[i].ID==ID)&&(CacheArray[i].fileNamePointer==openFilesPointer)){  //αν βρούμε το συγκεκριμλένο block
               return i;  //επιστρέφουμε τη θέση του
           }
            
        }
    }
   
    return EMPTY;

}

void removeFileFromCurentUse(int openDesc){  //κλέίνει επιλεγμένα αρχεία 
    int i;
    for(i=0;i<openingSize;i++){  //διασχίζουμε CacheArray
        if(CacheArray[i].fileNamePointer==appOpenings[openDesc]){  //όταν το block της CacheArray ανήκει στο αρχείο OpenFile[appOpenings[OpenDesc]] 
            CacheArray[i].fileNamePointer=EMPTY;  //το μηδενίζουμε 
        }
       
    }
    fclose(OpenFile[appOpenings[openDesc]].fileHandler);  //κλείνουμε το αρχείο 
    OpenFile[appOpenings[openDesc]].fileHandler=NULL;  //τον κάνουμε NULL
    for(i=0;i<blockSize;i++){  //διασχίζουμε byteMap
          if(i<openingSize){  //στις openingSize πρώτες επαναλήψεις
             OpenFile[appOpenings[openDesc]].fileName[i]=(char)NULL;  //"μηδενίζουμε" το όνομα                  
          }                         
          OpenFile[appOpenings[openDesc]].byteMap[i]=(char)NULL;  //μηδενίζουμε byteMap
    }
          
    appOpenings[openDesc]=EMPTY;  //μηδενίζουμε appOpenings
    
    
}

int LRU(){  //LRU για την εύρεση του λιγότερου χρησιμοποιημένου
    int i,j,k,position;
    int counter;
    for(i=0;i<openingSize;i++){  //διασχίζουμε την CacheArray
        counter=0;  //μετρητής
        for(j=0;j<openingSize;j++){
            if(CacheArray[i].pins[j]!=EMPTY){  //αν έχει "ξεκαρφιτσωθεί" | διασχίζουμε pins
                counter++;  //αυξάνουμε μετρητή
                break;  //και break
            }
        
        }
        if(counter==0){  //αν είναι 0
            long int minTimeStamp=timeCounter;  //εκχωρούμε την τιμή του timeCounter στο minTimeStamp
             
            for(k=0;k<openingSize;k++){  //διασχίζουμε CacheArray
                if(minTimeStamp>CacheArray[k].timeStamp){   //ψάχνουμε να βρούμε την μικρότερη τιμή του TimeStamp
                    minTimeStamp=CacheArray[k].timeStamp;  //αν βρεθεί την εκχωρούμε
                    position=k;  //επιστρέφουμε τη θέση της CahceArrayX που βρέθηκε ο μικροτερος χρόνος
                }
            
            }
            return position;
        }
        
    }
    BL_errorNUM=BLE_NOMEM;
    return BLE_NOMEM;  //επιστρέφουμε error
}

int WriteToAppOpenings(){  //επιστρέφει το πρώτο κενό που θα βρει στον appOpenings
    int i;
    for(i=0;i<openingSize;i++){  //διασχίζει appaOpenings
        if((appOpenings[i]==EMPTY)){  //αν βρει EMPTY 

            return i;  //επιστρέφει τη θέση που το βρήκε
        }
    }
    
    return EMPTY;
}
int WriteToOpenFile(){  //επιστρέφει το πρώτο κενό που θα βρει στον OpenFile
    int i;
    for(i=0;i<openingSize;i++){  //διασχίζει τον OpenFile
        if(OpenFile[i].fileHandler==NULL){  //αν βρει NULL 
            return i;  //επιστρέφει τη θέση όπου το βρήκε
        }
    }
    return EMPTY;
}

int FileExists(char* filename){  //έλεγχος ύπαρξης αρχείου
    FILE * file=fopen(filename,"rb");  //το ανοίγουμε για διάβασμα για να δούμε αν υπάρχει ή όχι
    if(file==NULL){  //αν είναι NULL 
        fclose(file);  //το κλέίνουμε
        return FALSE;
    }
        
    else{
        fclose(file);  //το κλέίνουμε
        return TRUE;   
    }
}


int GetFilenamPositon(char *filename){  //μας απιστρέφει τη θέση του αρχείου στον OpenFile
    int i,j;
    for(i=0;i<openingSize;i++){  //διασχίζει το OpenFile
        if(compareStrings(filename,OpenFile[i].fileName)){  //συγκρίνει τα ονόματα
                return i;  //επιστρέφει τη θέση του αρχείου
        }        
    }
    return EMPTY;

}

int FileIsOpen(char *filename){  //έλεγχος για ανοιχτό αρχείο
    int i,j;
    int counter; //μετρητής
    for(i=0;i<openingSize;i++){  //διασχίζουμε OpenFile
        counter=0;
        for(j=0;j<FileNameLength;j++){
            if((OpenFile[i].fileName[j]!=filename[j])){  //έλεγχος χαρακτήρα-χαρακτήρα | αν δεν είναι ίδια
              
                counter++;  //αυξάνουμε τον μετρητή
            }
            if((counter>0)||((filename[j]==EOF)&&(OpenFile[i].fileName[j]==filename[j]))){  //διάφοροι έλεγχοι
                break;
            }
            else if(((filename[j]==EOF)||(OpenFile[i].fileName[j]==EOF))&&(OpenFile[i].fileName[j]!=filename[j])){  
            //αν 1 από τα 2 ονόματα τελειώσει νωρίτερα και ο EOF δεν είναι ίδιος και στους 2 
                counter=2;  //απλώς βάζουμε μία τιμή διάφορη του 0 στον counter
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
    if(FileExists(filename)==FALSE){  //αν δεν υπάρχει το αρχείο
        file=fopen(filename,"wb");  //το ανοίγουμε για γράψιμο
        
        int i;
        for(i=0;i<blockSize;i++){  //διασχίζουμε byteMap
            fputc((char)NULL,file);  //αρχικοποίηση του block header
        }
        fclose(file);  //κλείνουμε το αρχείο
        return BLE_OK;  //επιστρέφουμε ΟΚ
    }
        
    else{  //αλλιώς
         BL_errorNUM=BLE_FILERROR;
        return BLE_FILERROR;  //επιστρέφουμε error
    }
}




int BL_DestroyFile(char *filename) {
    if(FileExists(filename)==TRUE){  //αν υπάρχει το αρχείο
            if(!remove(filename))  //καθώς η remove επιστρέφει 0 σε περίπτωση επιτυχίας, το NOT 0 θα είναι 1, άρα αν επιτύχει
                return BLE_OK;  //επιστρέφουμε ΟΚ
        
            else{
               BL_errorNUM=BLE_FILENERASE;
                return  BLE_FILENERASE;  //αλλιώς λάθος
            }                  
    }
    else{  //αλλιώς
        BL_errorNUM=BLE_FILENEXIST;
        return BLE_FILENEXIST;  //error
    }
}

int BL_OpenFile(char *filename) {//Function that opens a file the name of witch is given as a parameter if possible
    int i,j;
    FILE* fileHandler;
    if(FileExists(filename)){
        if(FileIsOpen(filename)){
            if((i=WriteToAppOpenings())!=EMPTY){  //αν υπάρχει χώρος στον πίνακα appOpenings                                           
                appOpenings[i]=GetFilenamPositon(filename);  //τοποθετούμε στη θέση i του appOp την θέση στον πίνακα OpenFile
                return i;  //επιστρέφει το αναγνωριστικό του χρήστη
            }        
            else{
                 BL_errorNUM=BLE_OFTABFULL;
                 return BLE_OFTABFULL;  //επιστρέφουμε error               
            }
        }
        else{  //αν το αρχείο δεν είναι ανοιχτό        
             if((i=WriteToOpenFile())!=EMPTY){  //αν υπάρχει χώρος στον πίνακα appOpenings 
                 fileHandler=fopen(filename,"rb");  //ανοίγουμε το αρχείο για ανάγνωση      
                 OpenFile[i].fileHandler=fileHandler;  //αποθηκεύουμε τον file handler του αρχείου στην fileHandler
                 for(j=0;j<FileNameLength;j++){      
                     if(filename[j]==EOF){  //αν το όνομα έχει τελειώσει
                         break;  //βγαίνουμε από το for
                     }          
                     OpenFile[i].fileName[j]=filename[j];  //αντιγράφουμε το όνομα στο fileName
                 }
                 fseek(fileHandler,0,SEEK_SET);  //τοποθετούμε τον file pointer στην αρχή του αρχείου
                 for(j=0;j<blockSize;j++){
                     OpenFile[i].byteMap[j]=fgetc(fileHandler);  //διαβάζουμε το header block
                 }     
             } 
             else{
                   BL_errorNUM=BLE_OFTABFULL;
                 return BLE_OFTABFULL;  //επιστρέφουμε error               
             } 
             if((i=WriteToAppOpenings())!=EMPTY){                                            
                 appOpenings[i]=GetFilenamPositon(filename);  //τοποθετούμε στη θέση i του appOp την θέση στον πίνακα OpenFile
                 return i;
             }
             else{
                   BL_errorNUM=BLE_OFTABFULL;
                 return BLE_OFTABFULL;  //επιστρέφουμε error              
            }     
        }   
    } 
    return BLE_OK;  //επιστρέφουμε OK
}

int BL_CloseFile(int openDesc) {
    int i,j,k;
    FILE* fileBuff;
    if(appOpenings[openDesc]==EMPTY){  //αν είναι άδειος ο πίνακας ανοιγμάτων στο συγκεκριμένο openDesc
        BL_errorNUM=BLE_FD;
        return BLE_FD;  //επιστρέφουμε error
    }
    else{
        for(i=0;i<openingSize;i++){  //διασχίζουμε CcacheArray
           if(CacheArray[i].fileNamePointer==appOpenings[openDesc]){   //όταν το block της CacheArray ανήκει στο αρχείο OpenFile[appOpenings[OpenDesc]]
                if((CacheArray[i].pins[openDesc]!=EMPTY)){  //αν τα pins δεν είναι "ξεκαρφιτσωμένα"
                    BL_errorNUM=BLE_BLOCKUNFIXED;
                     
                    return BLE_BLOCKUNFIXED;  //επιστρέφουμε error
                }
            }
        }
        if(TraverseCacheForClose(appOpenings[openDesc])){  
            fclose(OpenFile[appOpenings[openDesc]].fileHandler);  //κλείνουμε το αρχείο, το οποίο είχαμε ανοίξει για διάβασμα, καθώς επιθυμούμε να γράψουμε σε αυτό
            fileBuff=fopen(OpenFile[appOpenings[openDesc]].fileName,"rb+"); //ανοίγουμε το αρχείο για εγγραφή
            fseek(OpenFile[appOpenings[openDesc]].fileHandler,0,SEEK_SET);
            //μετακινούμε τον filePointer στην κατάλληλη θέση 
            for(k=0;k<blockSize;k++){
                fputc(OpenFile[appOpenings[openDesc]].byteMap[k],fileBuff);  //τοποθετεί μηδενικά bytes στο block αυτό 
            
            }
           
            fclose(fileBuff);  //κλείνουμε το αρχείο 
            OpenFile[appOpenings[openDesc]].fileHandler=fopen(OpenFile[appOpenings[openDesc]].fileName,"rb");  //και εδώ το ανοίγουμε για διάβασμα
            if(TraverseCacheForModify(appOpenings[openDesc])){  //ελέγχουμε αν έγιναν modify τα data
                UpdateFiles(appOpenings[openDesc]);  //κάνουμε update το κατάλληλο αρχείο σε περίπτωση που έγιναν modify τα data
            }
            removeFileFromCurentUse(openDesc);  //κλείνουμε το αρχείο
            
           
        }
    }
    return BLE_OK;  //επιστρέφουμε OK
    
}

int BL_GetFirstBlock(int openDesc) {
        
    return BL_GetNextBlock(openDesc,0);  //χρησιμοποιεί την get next block για να βρει το πρώτο έγκυρο
        
}
    
int BL_GetNextBlock(int openDesc, int blockNum) {
    while(TRUE) {  //while
        if (blockNum>=blockSize){  //αν το blockNum που μας δίνει είναι >= του 1024
            BL_errorNUM=BLE_NONEXTFILE;
            return BLE_NONEXTFILE;  //επιστρέφουμε error
        }
                   
        blockNum++;  //αυξάνουμε το blockNum για να πάμε στο επόμενο έγκυρο
        if(OpenFile[appOpenings[openDesc]].byteMap[blockNum]==ValidB){  //ελέγχουμε για να δούμε αν το block είναι έγκυρο
            return blockNum;  //επιστρέφουμε το αναγνωριστικό                      
            
            }      
    }
}
int CheckForValidBlock(int openDesc,int blockNum){
    int i,j,l;
    j=0;
    for(i=0;i<blockSize;i++){  //διασχίζουμε byteMap
        l=BL_GetNextBlock(openDesc,i);  //το l μας δίνει το επόμενο έγκυρο block
        if(l>0){  //αν είναι >0
            i=l;  //εκχωρούμε στο i το αναγνωριστικό του έγκυρου block
        }   
        if(l<EMPTY){  //αλλιώς
            return l;  //επιστρέφει τη θέση
        }     
        if(i!=BLE_NONEXTFILE){  //αλλιώς αν είναι διάφορο του error
            j++;//αυξάνεται κατά 1
            if(j==blockNum){  //αν ο μετρητής γίνει ίοσς με το αναγνωριστικό του block που ψάχνουμε
                return i;  //επιστρέφουμε το αναγνωνριστικό            
            }
           
        }   
    }
    return BLE_INVALIDBLOCK;  //επιστρέφουμε error
}

void initBlock(int openDesc, int blockNum,int cacheArrayPosition){
    int i;
     
    CacheArray[cacheArrayPosition].fileNamePointer=appOpenings[openDesc];
    CacheArray[cacheArrayPosition].ID=blockNum;
    for(i=0;i<openingSize;i++){
        CacheArray[cacheArrayPosition].FileName[i]=OpenFile[appOpenings[openDesc]].fileName[i];  
        //αντιγράφουμε το όνομα του αρχείου, του οποίου το blockNum έχουμε τοποθετήσει στη θέση CacheArrayPosition της Cache 
    }
    CacheArray[cacheArrayPosition].modified=FALSE;
    CacheArray[cacheArrayPosition].pins[openDesc]=TRUE; 
   
    fseek(OpenFile[appOpenings[openDesc]].fileHandler,(blockNum+1)*blockSize,SEEK_SET);
    //πάει στο συγκεκριμένο block του αρχείου 
    for(i=0;i<blockSize;i++){  //διασχίζουμε το byteMap
        CacheArray[cacheArrayPosition].data[i]=fgetc(OpenFile[appOpenings[openDesc]].fileHandler);
        //διαβάζουμε την πληροφορία του block και την τοποθετούμε στα data της CacheArray
    }
    CacheArray[cacheArrayPosition].timeStamp=timeCounter;  //εκχωρούμε την timeCounter η οποία κρατάει το χρόνο που έγινε load η σελίδα στην Cache
    ++timeCounter;  //και το αυξάνουμε
}

int BL_BeginBlock(int openDesc, int blockNum, char **blockBuf) {
    int i,j;
   
    i=TraverseCacheForBlock(appOpenings[openDesc],blockNum,OpenFile[appOpenings[openDesc]].fileName);
    if(i!=EMPTY){  //αν υπάρχει το block που θέλουμε στην Cache 
            if((CacheArray[i].pins[openDesc]==EMPTY)){  //αν ο χρήστης openDesc δεν έχει ανοίξει ήδη το συγκεκριμένο block
                CacheArray[i].pins[openDesc]=TRUE;  //το "καρφιτσώνουμε"
            }
            else{  //αλλιώς
                return BLE_BLOCKFIXED;  //επιστρέφουμε error
            }
            *blockBuf=CacheArray[i].data;  //δίνουμε πρόσβαση στο block πληροφορίας μέσα στην CacheArray στο χρήστη μέσω του blockBuf
            return BLE_OK;  //επιστρέφουμε OK
    }
    else{  //αν δεν υπάρχει 

       if(OpenFile[appOpenings[openDesc]].byteMap[blockNum]==ValidB){  //αν σύμφωνα με το byteMap, το block με αναγνωριστικό blockNum, είναι έγκυρο
                                                         
            if((j=TraverseCacheForEmptyBlock())>=FALSE){  //αν βρεθεί μία κενή θέση στην Cache

                initBlock(openDesc,blockNum,j);  //τοποθετούμε στη θέση αυτή τα data του συγκεκριμένου block
                *blockBuf=CacheArray[j].data;  //δίνουμε πρόσβαση στο block πληροφορίας μέσα στην CacheArray στο χρήστη μέσω του blockBuf

                return BLE_OK;  //επιστρέφουμε OK              
            }
            else{  //αλλιώς
                if((i=LRU())==BLE_NOMEM){  //αν υπάρχει λάθος με την LRU
                    return i;  //επιστρέφει error
                }
                else{
                     
                    initBlock(openDesc,blockNum,i);  //τοποθετούμε στο παλιότερο μη χρησιμοποιημένο block της Cache
                    *blockBuf=CacheArray[j].data;  //δίνουμε πρόσβαση στο block πληροφορίας μέσα στην CacheArray στο χρήστη μέσω του block
                    return  BLE_OK;  //επιστρέφουμε OK                  
                }
               
            }
        }
        else{
            
             BL_errorNUM=BLE_INVALIDBLOCK;
            return BLE_INVALIDBLOCK;  //αλλιώς επιστρέφουμε error
        }
        
    }
    
}

int BL_AllocateBlock(int openDesc) {
    int i,j,k;
    FILE* fileBuff;  
    //χρησιμοποιούμε το fileBuff για να ξανανοίξουμε τα αρχεία που βρίσκονται ανοιγμένα στο OpenFile, με σκοπό να ενεργοποιήσουμε ένα νέο block 
    //τοποθετώντας την τιμή NULL στην αντίστοιχη θέση του block στο αρχείο
    for(i=0;i<blockSize;i++){  //διασχίζουμε το byteMap
        if(OpenFile[appOpenings[openDesc]].byteMap[i]==(char)NULL){  //αν σύμφωνα με το byteMap, το block στην επανάληψη i είναι ανενεργό
            if(i!=(blockSize-1)){  //και δεν βρισκόμαστε στην τελευταία επανάληψη
                                
                if(OpenFile[appOpenings[openDesc]].byteMap[i+1]==ValidB){  //και το αμέσως επόμενο block, σύμφωνα με το byteMap, είναι έγκυρο
                   OpenFile[appOpenings[openDesc]].byteMap[i]=ValidB;  
                   //η σελίδα στη θέση i προέρχεται από ανακύκλωση, άρα το block αυτό υπάρχει ήδη στο αρχείο και αφού το κάνουμε έγκυρο 
                   return i;  //και επιστρέφουμε τη θέση του από το byteMap
                }
                else if(OpenFile[appOpenings[openDesc]].byteMap[i+1]==(char)NULL){  //αν το αμέσως επόμενο block, σύμφωνα με το byteMap, είναι άκυρο
                    
                    fclose(OpenFile[appOpenings[openDesc]].fileHandler);  //κλείνουμε το αρχείο, το οποίο είχαμε ανοίξει για διάβασμα, καθώς επιθυμούμε να γράψουμε σε αυτό
                    fileBuff=fopen(OpenFile[appOpenings[openDesc]].fileName,"rb+");  //ανοίγουμε το αρχείο για εγγραφή 
                    fseek(fileBuff,(i+1)*blockSize,SEEK_SET);  //μετακινούμε τον filePointer στην κατάλληλη θέση
                    //(to i+1 parakatw paei epidi praktika den metrame to header block stis xrisimes plirofories kai to arxio mas thane 1025 KBytes praktika)
                    for(j=0;j<blockSize;j++){  //διασχίζουμε το byteMap
                        fputc((char)NULL,OpenFile[appOpenings[openDesc]].fileHandler);  //τοποθετεί μηδενικά bytes στο block αυτό  
                    }
                    OpenFile[appOpenings[openDesc]].byteMap[i]=ValidB;  //κάνουμε έγκυρο το block
                    fclose(fileBuff);  //κλείνουμε το αρχείο 
                    OpenFile[appOpenings[openDesc]].fileHandler=fopen(OpenFile[appOpenings[openDesc]].fileName,"rb");  //και εδώ το ανοίγουμε για διάβασμα                      
        
                    return i;  //και επιστρέφουμε τον αριθμό της σελίδας
                }
            }
            else{  //αλλιώς όταν βρισκόμαστε στην τελευταία επανάληψη, δηλαδή βρισκόμαστε στο block 1023 
                 fclose(OpenFile[appOpenings[openDesc]].fileHandler);  
                 //κλείνουμε το αρχείο, το οποίο είχαμε ανοίξει για διάβασμα, καθώς επιθυμούμε να γράψουμε σε αυτό
                 fileBuff=fopen(OpenFile[appOpenings[openDesc]].fileName,"rb+");  //ανοίγουμε το αρχείο για εγγραφή
                 fseek(fileBuff,0,SEEK_END);  //μετακινούμε τον filePointer στο τέλος του αρχείου
                 for(j=0;j<blockSize;j++){  
                     fputc((char)NULL,OpenFile[openDesc].fileHandler);  //μεγαλώνει το αρχείο κατά 1 block 
                 }
                 OpenFile[appOpenings[openDesc]].byteMap[i]=ValidB;  //κάνουμε το block έγκυρο
                 fclose(fileBuff);  //κλείνουμε το αρχείο 
                 OpenFile[appOpenings[openDesc]].fileHandler=fopen(OpenFile[appOpenings[openDesc]].fileName,"rb");  //και εδώ το ανοίγουμε για διάβασμα  
                                   
                 return i;  //και επιστρέφουμε τον αριθμό της σελίδας
                
            }
            
        }
    }
    BL_errorNUM=BLE_FILEFULL;
    return BLE_FILEFULL;  //επιστρέφουμε error
    
}

int BL_DisposeBlock(int openDesc, int blockNum) {
    int i;
    for(i=0;i<openingSize;i++){  //διασχίζουμε την CacheArray για να βρούμε αν το block που θέλουμε να "αχρηστέψουμε" υπάρχει στη μνήμη
        if((CacheArray[i].fileNamePointer==appOpenings[openDesc])&&(compareStrings(OpenFile[appOpenings[openDesc]].fileName,CacheArray[i].FileName))){
        //αν τα block που ζητάει ο χρήστης openDesc και τα ονόματα των αρχείων είναι ίδια                                                                                                                                             
            if(CacheArray[i].ID==blockNum){  //αν το block είναι αυτό που ψάχνουμε
                BL_errorNUM=BLE_BLOCKFIXED;
                return BLE_BLOCKFIXED;  //επιστρέφουμε error
            }
        }
    }
  
    OpenFile[appOpenings[openDesc]].byteMap[blockNum]=(char)NULL;  //μηδενίζουμε το byteMap για το block που θέλουμε
    
    BL_errorNUM=BLE_OK;
    return BLE_OK;  //επιστρέφουμε ΟΚ
    
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
   
    for(i=0;i<openingSize;i++){  //διασχίζουμε τον πίνακα CacheArray για να κλείσουμε το άνοιγμα ενός block με αναγνωριστικό blockNum που έχει ζητήσει ο χρήστης openDesc
        if((compareStrings(OpenFile[CacheArray[i].fileNamePointer].fileName,OpenFile[appOpenings[openDesc]].fileName))&&(CacheArray[i].pins[openDesc]==EMPTY)&&(CacheArray[i].ID==blockNum)){
        //συγκρίνουμε τα ονόματα των αρχείων, το ID με το blockNum και ελέγχουμε αν τα pins είναι "ξεκαρφιτσωμένα" - EMPTY    
            BL_errorNUM=BLE_BLOCKUNFIXED; 
            return BLE_BLOCKUNFIXED;  //αν δεν είναι το συγκεκριμένο block, επιστρέφουμε error
        }
        if((CacheArray[i].pins[openDesc]==TRUE)&&(CacheArray[i].ID==blockNum)){  //αν τα pins είναι "καρφιτσωμένα" και το block είναι αυτό που θέλουμε
            CacheArray[i].pins[openDesc]=EMPTY;  //"αδειάζουμε" τα pins
            if(CacheArray[i].modified!=TRUE){  //αν δεν έχει τροποποιηθεί το block από πριν
                CacheArray[i].modified=modified;  //το modified παίρνει την τιμή που δίνει ο χρήστης
            }
            return BLE_OK;  //ΟΚ
        }    

    }
    return BLE_BLOCKNOTINBUF;  //επιστρέφουμε error
    
    
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
    for(i=0;i<openingSize;i++)  //διασχίζουμε τον πίνακα OpenFile για να βρούμε και να κλείσουμε τα ανοιχτά αρχεία
    {
        if((OpenFile[i].fileHandler!=NULL)&&(fclose(OpenFile[i].fileHandler)==EOF)){  //αν ο fileHandler δεν είναι NULL και κατά το κλείσιμο του αρχείου γίνει λάθος 
            j++;  //αυξάνουμε το j κατά 1 για κάθε λάθος
        }
    
    }
    if(j!=FALSE){  //αν το j έχει τιμή διάφορη του 0
        BL_errorNUM=BLE_FILENCLOSEPROPER;  
        return BLE_FILENCLOSEPROPER;  //επιστρέφουμε error
    }
    else{
         
        return BLE_OK;  //αλλιώς OK
   }
    

}
