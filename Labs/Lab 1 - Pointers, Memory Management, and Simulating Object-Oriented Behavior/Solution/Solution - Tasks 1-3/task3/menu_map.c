#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE 5

typedef struct{
    char *name;
    char (*fun)(char);
}fun_desc;

void printArr(char* arr, int size);

char my_get(char c){
  char ch = fgetc(stdin);
  return ch;
}

char cprt(char c){
  if(0x20 <= c && c <= 0x7E)
    printf("%c\n", c);
  else 
    printf(". \n");

  return c;
}

char encrypt(char c){
  if (0x20 <= c && c <= 0x4E)
    return c + 0x20;
  return c;

}

char decrypt(char c){

  if (0x40 <= c && c <= 0x7E)
    return c - 0x20;
  return c ;
}

char xoprt(char c){
  printf("%x %o \n", c,c);
  return c;
}

char* map(char *array, int array_length, char (*f) (char)){
  char* mapped_array = (char*)(malloc(array_length*sizeof(char)));
  /* Task 2.a */

  for(int i=0; i < array_length; i++){
    mapped_array[i] = f(array[i]);
  }

  return mapped_array;
}

int main(int argc, char **argv){
    char* carray = (char*)(malloc(5*sizeof(char)));
    fun_desc menuItems [] = {{"Get string", my_get},{"cprt", cprt},{"encrypt", encrypt},{"decrypt", decrypt},{"xoprt", xoprt},{NULL, NULL}};

    while (1){ 
      printf("\nSelect operation from the following menu:");
      for(int i=0; i < 5; i++){
        printf("\n%i) %s",i, menuItems[i].name);  
      }

        printf("\noption: ");
        char cindex[MAX_LINE];
        
        if(fgets(cindex, MAX_LINE, stdin) == NULL)
          exit(0);

        int index = atoi(cindex);
      
        //check bounds
        if(0 <= index && index <= 4)
          printf("\nWithin bounds\n");
        else {
          printf("\nNot within bounds");
          exit(1);
        }
      
      char* tmp = map(carray, 5, menuItems[index].fun);
      free(carray);
      carray = tmp;

      
      //clearing the input stream, source: https://stackoverflow.com/questions/4573457/how-to-flush-the-console-buffer
      //char c1 ;
      //while (c1 != EOF && (c1 = getchar()) != '\n');
      
      
    }

    printf("\n");
    exit(0);
}


void printArr(char* arr, int size){
  printf("\n");
  for(int i=0; i<size; i++)
    printf(" %c",arr[i]);
}