#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//was taken from https://www.w3schools.com/c/c_booleans.php to use boolean types (less wasteful than using a char)
#include <stdbool.h>

_Bool isInASCIIRange (unsigned char ch);

int main(int argc, char **argv) {
   
    //taken from the "main.c" example and tweaked a little
    FILE * output = stdout;
    FILE * input = stdin;
    FILE * debug = stderr;

    //  --- PART 1 ---

     _Bool isInDebug = true; //default: ON

    for(int i = 1; i < argc; i++){
        if(isInDebug)
            fprintf(debug, "%s \n" ,argv[i]);
        
        //check for a debug flag
        if(argv[i][2] == '\0' && argv[i][1]== 'D'){
            if(argv[i][0] == '+')
                isInDebug = true;
            else if (argv[i][0] == '-')
                isInDebug = false;
        }
            
    }

    //  --- PART 3 ---
    for(int i = 1; i < argc; i++){

        //check for a -I flag
        //I've read the manual for the function's arguments
        if(argv[i][0] == '-' && argv[i][1]== 'I'){
            const char * mode = "r";
            const char * path = (char *)argv[i] + 2;
            input = fopen(path, mode);

            if(input == NULL){
                fprintf(debug, "%s \n" ,"ERROR: No such input file was found!");
                exit(1);
            }
        }

        if(argv[i][0] == '-' && argv[i][1]== 'O'){
            const char* mode = "w";
            const char * path = (char *) argv[i] + 2;
            output = fopen(path, mode);
        }
    }


    //  --- PART 2 ---

    //find the encryption key flag and parse it
    _Bool isAddingKey = true;
    char * key = NULL;    //hold the key as string (will by "0" if wasn't provided by the user)

    for(int i = 1; i < argc; i++){
        if(argv[i][0] == '+' && argv[i][1]== 'e'){ 
            key = &argv[i][2];
            isAddingKey = true;
        } else if(argv[i][0] == '-' && argv[i][1]== 'e'){
            key = &argv[i][2];
            isAddingKey = false;
        }
    }

    //check if no encryption flag was provided
    if(key == NULL)
        key = "0";
    
    //iterate over incoming user input, encoding it and sending it to the screen
    int multiplier = 1;
    if(! isAddingKey)
        multiplier = -1;

    int index = 0;

    //I've used the manual for the fget, fput, feof - no copying was done tho.

    //-----test delete later


    ///---- end of test

    unsigned char nextChar = fgetc(input);
    
    while( feof(input) == 0){
        
        unsigned char processed = nextChar;

        if(isInASCIIRange(nextChar)){
            processed = nextChar + (key[index] - '0') * multiplier;

            //check for wrap
            if(! isInASCIIRange(processed)){
                //numbers 0-9
                if((unsigned char)('0' - 9) <= processed && processed < (unsigned char)('0'))
                    processed += 10;
                if((unsigned char)('9') < processed && processed <= (unsigned char)('9' + 9))
                    processed -= 10;
                
                //lower case letters a-z
                if((unsigned char)('a' - 9) <= processed && processed < (unsigned char)('a'))
                    processed += 26;
                if((unsigned char)('z') < processed && processed <= (unsigned char)('z' + 9))
                    processed -= 26
                    ;
            }
        }

        fputc(processed, output);
        
        //update the index
        index ++;

        if(key[index] == 0x00)
            index = 0;

        nextChar = fgetc(input);
    } 

    return 0;
}

//check if a certain chaacter is in range a-z 0-9, source: the assignment's hints
_Bool isInASCIIRange (unsigned char ch){
    if( (unsigned char)'a' <=  ch && ch <=  (unsigned char)'z')
        return true;
    if( (unsigned char)'0' <= ch && ch <= (unsigned char)'9')
        return true;
    
    return false;
}