#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE 6
#define RET 0xC3 // source: https://pdos.csail.mit.edu/6.828/2005/readings/i386/RET.htm

typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    unsigned char* sig;
} virus;

typedef struct link {
    struct link* nextVirus;
    virus *vir;
} myLink;

typedef struct fun_desc{
    char* name;
    void (*fun)(void);
}fun_desc;


//function prototypes
char* readMagicBytes(FILE* file);
void parseMagicBytes(char* magicBytesBuffer);

void SetSigFileName();
void loadSigFileName();
void printSigFile();

virus* readVirus(FILE* file);
void printVirus(virus* virus);
void printVirusToStream(virus* virus, FILE* file);

void virusDestructor(virus* v);

void list_print(myLink *virus_list, FILE* file);
myLink* list_append(myLink* virus_list, virus* data);
void list_free(myLink *virus_list);


void detectVirusWrapper();
void detect_virus(char *buffer, unsigned int size, myLink *virus_list, int isFix);

void fixFIle();
void neutralize_virus(char *fileName, int signatureOffset);

void performQuit();


//global variables
char* suspectedFileName = NULL;
FILE* susFile = NULL;

char* sigFileName = "signatures-L";
FILE* sigFile;
myLink* sig_list;

int isNeedToFix = 0;    //0 - no    1 - yes


#define LITTLE_ENDIAN_MODE 0
#define BIG_ENDIAN_MODE 1
int endian = LITTLE_ENDIAN_MODE; 

int main(int argc, char **argv){
    /*          --- Pre-menu main ---
    SetSigFileName();
    FILE* file = loadSigFileName(sigFileName);

    char* magicBytesBuffer = readMagicBytes(file);
    parseMagicBytes(magicBytesBuffer);
    
    
    virus* v = readVirus(file);
    while (v != NULL){
        printVirus(v);
        virusDestructor(v);

        v = readVirus(file);
    }

    free(magicBytesBuffer);
    */

    if (argc < 2)
    {
        printf("Usage: %s <file> \n", argv[0]);
        return 1;
    }

    suspectedFileName = argv[1];

    fun_desc menuItems [] = {
    {"0) Set signatures file name", SetSigFileName},
    {"1) Load signatures", loadSigFileName},
    {"2) Print signatures", printSigFile},
    {"3) Detect viruses", detectVirusWrapper},
    {"4) Fix file", fixFIle},
    {"5) Quit", performQuit},
    {NULL, NULL}};

    while (1){ 

      printf("\nSelect operation from the following menu:\n");
        for(int i=0; i < MAX_LINE; i++){
            printf("%s \n", menuItems[i].name);  
        }

        printf("\noption: ");
        char cindex[MAX_LINE];
        
        if(fgets(cindex, MAX_LINE, stdin) == NULL)
          exit(0);

        int index = atoi(cindex);
      
        //check bounds
        if(0 <= index && index <= MAX_LINE)
          printf("\nWithin bounds\n");
        else{
            printf("\nNot within bounds, try again.\n");
            continue;
        }

        menuItems[index].fun();
    }
    
    exit(0);
}


void SetSigFileName(){

    printf("\nEnter signature file: ");

    int ARBITRARY_SIZE = 100;
    char* requestedFile = (char *)malloc(ARBITRARY_SIZE * sizeof(char));
    if (requestedFile == NULL){
        printf("Memory error: malloc failed. the program will now exit.\n");
        exit(1);
    }

    fgets(requestedFile, ARBITRARY_SIZE ,stdin);
    requestedFile[strlen(requestedFile)-1] = '\0'; // strip the newline character


    //check if such file really exists
    if ( access(requestedFile, F_OK) != 0){
        printf("Error! no such file. please try again.\n");
        return;
    } else {
        sigFileName = requestedFile;
        printf("A new signature file was loaded sucessfuly: %s \n", requestedFile);
    }
}


void loadSigFileName(){

    FILE* file = fopen(sigFileName, "r");
    if(file == NULL){
        printf("ERROR! can't load file %s  the program will now exit.", sigFileName);
        exit(1);
    }

    printf("The file %s was loaded sucessfuly.\n", sigFileName);
    sigFile = file;


    //read the magic bytes
    char* magicBytesBuffer = readMagicBytes(sigFile);
    parseMagicBytes(magicBytesBuffer);
}

virus* readVirus(FILE* file){
    if(feof(file) != 0)
        return NULL;

    
    //read length
    int LENGTH_SIZE = 1;
    unsigned short* lengthBuffer = (unsigned short *)malloc(LENGTH_SIZE * sizeof(unsigned short));
    if (lengthBuffer == NULL){
        printf("Memory error: malloc failed. the program will now exit.");
        free(lengthBuffer);
        exit(1);
    }
    

    //if the file is empty
    if(lengthBuffer == NULL || fread(lengthBuffer, sizeof(unsigned short), LENGTH_SIZE, file) <= 0)
        return NULL;

    //read name
    int NAME_SIZE = 16;
    char* nameBuffer = (char *) malloc(NAME_SIZE * sizeof(char));
    if (nameBuffer == NULL){
        printf("Memory error: malloc failed. the program will now exit.");
        exit(1);
    }
    fread(nameBuffer, sizeof(char), NAME_SIZE, file);

    
    //read signature
    int SIG_SIZE = (int)lengthBuffer[0];

    unsigned char* sigBuffer = (unsigned char *)malloc(SIG_SIZE * sizeof(unsigned char));
    if (sigBuffer == NULL){
        printf("Memory error: malloc failed. the program will now exit.");
        free(lengthBuffer);
        free(nameBuffer);
        exit(1);
    }
    fread(sigBuffer, sizeof(char), SIG_SIZE, file);

    //create a new virus struct
    virus* v = malloc(sizeof(virus));
    if (v == NULL){
        printf("Memory error: malloc failed. the program will now exit.");
        exit(1);
    }
    
    (*v).SigSize = lengthBuffer[0];
    memcpy((*v).virusName, nameBuffer, NAME_SIZE); 

    (*v).sig = (unsigned char *)malloc(SIG_SIZE * sizeof(unsigned char));
    memcpy((*v).sig, sigBuffer, SIG_SIZE);

    free(nameBuffer);
    free(lengthBuffer);
    free(sigBuffer);

    return v;
}

void printSigFile(){
    virus* v = readVirus(sigFile);

    while (v != NULL){
        printVirus(v);
        virusDestructor(v);

        v = readVirus(sigFile);
    }

    //go to the start of the signatueres list
    fseek(sigFile, 0 , SEEK_SET);
    readMagicBytes(sigFile);
}

void printVirus(virus* virus){
    printVirusToStream(virus, stdout);
}

void printVirusToStream(virus* virus, FILE* file){
    //name
    fprintf(file,"name: %s \n",(*virus).virusName);

    //sig size
    unsigned short swapped = (*virus).SigSize;
    //if(endian == 0)
    //   swapped = (swapped>>8) | (swapped<<8);

    fprintf(file, "signature length: %d \n",swapped);


    //signature
    if(endian == 0){
        unsigned char* sig = (unsigned char *)malloc(swapped * sizeof(unsigned char));
        if (sig == NULL){
            fprintf(file, "Memory error: malloc failed. the program will now exit.");
            exit(1);
        }

        memcpy(sig, (*virus).sig, swapped);
        fprintf(file, "signature: \n");
        for(int i = 0; i < (*virus).SigSize; i++)
            fprintf(file, "%02X ", sig[i]);

        free(sig);
    }
    else {
        fprintf(file, "signature: \n");
        for(int i = 0; i < (*virus).SigSize; i++)
            fprintf(file, "%02X ", (*virus).sig[i]);

    }
    fprintf(file,"\n\n");
}

void virusDestructor(virus* v){
    if(v != NULL){
        free((*v).sig);
        free(v);
    }
}

char* readMagicBytes(FILE* file){
    //read the magic bytes 
    const int MAGIC_BYTES_LENGTH = 4;

    char* magicBytesBuffer = (char *)malloc(MAGIC_BYTES_LENGTH * sizeof(char));
    if (magicBytesBuffer == NULL){
        printf("Memory error: malloc failed. the program will now exit.");
        exit(1);
    }
    fread(magicBytesBuffer, sizeof(char), MAGIC_BYTES_LENGTH, file);

    return magicBytesBuffer;
}
void parseMagicBytes(char* magicBytesBuffer){
    if(strcmp(magicBytesBuffer, "VIRL") == 0){
        endian = LITTLE_ENDIAN_MODE;
        printf("Working in Little Endian mode.\n");
    } else if (strcmp(magicBytesBuffer, "VIRB") == 0){
        endian = BIG_ENDIAN_MODE;
        printf("Working in Big Endian mode.\n");
    }
    else {
        printf("ERROR: bad signature file %s\nThe program will now exit.\n", sigFileName);
        free(magicBytesBuffer);
        exit(1);
    }
}

void list_print(myLink *virus_list, FILE* file){
    if(virus_list->vir == NULL)
        return;

    printVirusToStream(((*virus_list).vir), file);
    list_print(virus_list->nextVirus, file);
}

myLink* list_append(myLink* virus_list, virus* data){
    myLink* head = (myLink *) malloc(1 * sizeof(myLink));
    head->vir = data;
    head->nextVirus = virus_list;

    return head;
}

void list_free(myLink *virus_list){
    if(virus_list->vir == NULL)
        return;

    free(virus_list->vir); 
    list_free(virus_list->nextVirus);
}

void performQuit (){
    printf("The program will now quit.\n");
    exit(0);
}

void detectVirusWrapper(){
    //check pre conditions
    if(sigFile == NULL){
        printf("no signature file is loaded! please load a signature file first\n");
        return;
    }

    //load the suspected file into memory.
    if(susFile != NULL)
        fclose(susFile);

    susFile = fopen(suspectedFileName, "r");
    if(susFile == NULL){
        printf("Error loading the file into memory. the program will now exit.\n");
        exit(1);
    }

    int fileSize ;

    fseek(susFile, 0, SEEK_END);
    fileSize = ftell(susFile);
    fseek(susFile, 0, SEEK_SET);

    char* susFileBuffer = (char*) malloc((fileSize + 1) * sizeof(char));

    if(susFileBuffer == NULL){
        printf("ERROR, can't allocate memory (malloc failed). the program will now exit.\n");
        free(susFileBuffer);
        fclose(susFile);
        exit(1);
    }

    //int readBytes = fread(susFileBuffer, fileSize+1, 1, susFile);
    int readBytes = fread(susFileBuffer, 1, fileSize, susFile);
    

    if(readBytes < fileSize){
        printf("Error loading the file %s to memory, the program will exit.", suspectedFileName);
        exit(1);
    }

    myLink* sig_list = NULL;
    virus* v = readVirus(sigFile);

    while (v != NULL){
        sig_list = list_append(sig_list, v);
        v = readVirus(sigFile);
    }  

    if(sig_list == NULL){
        printf("no signatures were loaded from %s, please re-load another signature file.\n", sigFileName);
        return;
    }

    //call the wrapped function
    detect_virus(susFileBuffer, fileSize, sig_list, isNeedToFix);
}

void detect_virus(char *buffer, unsigned int size, myLink *virus_list, int isFix){

    printf("Scanning %s ...\n", suspectedFileName+2);
    int virusCount = 0;

    while(virus_list != NULL && virus_list->vir != NULL){
        
        //check the head of the list
        int min = virus_list->vir->SigSize;
        if(min > size) 
            min = size;

        for(int i=0; i < size-min; i++){
            if(memcmp(buffer + i, virus_list->vir->sig, min) == 0) {
                //print mode: isNeedToFix = 0
                if(isNeedToFix == 0){
                    virusCount++;
                    printf("Virus #%d detected at position %d.\nVirus name: %s, signature size:%d\n", 
                    virusCount, i, (*virus_list).vir->virusName, (virus_list)->vir->SigSize);
                } 
                
                //fix mode: isNeedToFix = 1
                else if (isNeedToFix == 1){
                    printf("Neutralizing: %s, at offset %d\n",(*virus_list).vir->virusName, i);
                    neutralize_virus(suspectedFileName, i);
                }
            }
        }

        virus_list = virus_list->nextVirus;
    }

    printf("%d virus(es) were found.\n", virusCount);
}

void neutralize_virus(char *fileName, int signatureOffset)
{
    FILE *file = fopen(fileName, "rb+");
    if (!file)
    {
        fprintf(stderr, "Error: cannot open file\n");
        return;
    }

    if (fseek(file, signatureOffset, SEEK_SET))
    {
        fprintf(stderr, "Error: cannot seek to offset\n");
        fclose(file);
        return;
    }

    if (fputc(RET, file) == EOF)
    {
        fprintf(stderr, "Error: cannot write to file\n");
        fclose(file);
        return;
    }

    fclose(file);
}


void fixFIle(){
    isNeedToFix = 1;
    detectVirusWrapper();        
    isNeedToFix = 0;    
}


//---- menu functionality 
char* map(char *array, int array_length, char (*f) (char)){
  char* mapped_array = (char*)(malloc(array_length*sizeof(char)));

  for(int i=0; i < array_length; i++){
    mapped_array[i] = f(array[i]);
  }

  return mapped_array;
}

void printArr(char* arr, int size){
  printf("\n");
  for(int i=0; i<size; i++)
    printf(" %c",arr[i]);
}

