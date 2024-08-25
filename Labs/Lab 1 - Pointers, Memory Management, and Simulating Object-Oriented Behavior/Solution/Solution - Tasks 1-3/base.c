#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char my_get(char c);
char cprt(char c);
char encrypt(char c);
char decrypt(char c);
char xoprt(char c);

void printArr(char* arr, int size);

//just for testing
char upper (char ch){
  return ch + 1;
}

char my_get(char c){
  return (char)fgetc(stdin);
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
  c += 0x020;

  if (0x20 <= c && c <= 0x4E)
    return c;
  return c - 0x20;
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
  /*
  //Task 1 test
  char arr1[] = {'a', 'b', 'c', '0','1', '9', '@'};

  char* arr2 = map(arr1, sizeof(arr1)/sizeof(char), *upper);
  printf("%s\n", arr2);
  free(arr2);
  */ 

  //task 2 test

  int base_len = sizeof(5);
  char arr1[base_len];

  char* arr2 = map(arr1, base_len, my_get);
  char* arr3 = map(arr2, base_len, cprt);
  char* arr4 = map(arr3, base_len, xoprt);
  char* arr5 = map(arr4, base_len, encrypt);

  /*
  printArr(arr2, base_len);
  printArr(arr3, base_len);
  printArr(arr4, base_len);
  printArr(arr5, base_len);
  */

  free(arr2);
  free(arr3);
  free(arr4);
  free(arr5);

  exit(0);
}

void printArr(char* arr, int size){
  printf("\n");
  for(int i=0; i<size; i++)
    printf(" %c",arr[i]);
}