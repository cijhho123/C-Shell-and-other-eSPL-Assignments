#include <stdio.h>
#include <stdlib.h>

void printHex(unsigned char* buffer, int length);

int main(int argc, char **argv){
    FILE* file = fopen(argv[1], "r");

    if(file == NULL){
        printf("No Such File, the program will exit.\n");
        exit(1);
    }

    unsigned char* buffer;
    int size = 1;
    buffer = (unsigned char *)malloc(size * sizeof(unsigned char *));

    while(1){
        fread(buffer, sizeof(char), size, file);

        if(feof(file))
            break;

        printHex(buffer, 1);
    }

    printf("\n");
    free(buffer);
    exit(0);
}

void printHex(unsigned char* buffer, int length){
    for(int i=0; i < length; i++){
        printf("%02X ",buffer[i]);
    }
}