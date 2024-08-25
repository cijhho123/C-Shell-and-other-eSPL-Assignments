#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int addr5;
int addr6;

int foo()
{
    return -1;
}
void point_at(void *p);
void foo1();
char g = 'g';
void foo2();

int secondary(int x)
{
    int addr2;
    int addr3;
    char *yos = "ree";
    int *addr4 = (int *)(malloc(50));
	int iarray[3];
    float farray[3];
    double darray[3];
    char carray[3]; 
	int iarray2[] = {1,2,3};
    char carray2[] = {'a','b','c'};
    int* iarray2Ptr;
    char* carray2Ptr; 
    
	printf("- &addr2: %p\n", &addr2);
    printf("- &addr3: %p\n", &addr3);
    printf("- foo: %p\n", &foo);
    printf("- &addr5: %p\n", &addr5);
	printf("Print distances:\n");
    point_at(&addr5);

    printf("Print more addresses:\n");
    printf("- &addr6: %p\n", &addr6);
    printf("- yos: %p\n", yos);
    printf("- gg: %p\n", &g);
    printf("- addr4: %p\n", addr4);
    printf("- &addr4: %p\n", &addr4);

    printf("- &foo1: %p\n", &foo1);
    printf("- &foo1: %p\n", &foo2);
    
    printf("Print another distance:\n");
    printf("- &foo2 - &foo1: %ld\n", (long) (&foo2 - &foo1));

   
    printf("Arrays Mem Layout (T1b):\n");

    printf("iarray: %0x \n", iarray);
    printf("iarray+1: %0x \n", iarray+1);
    printf("&iarray+1: %0x \n", &iarray+1);

    printf("farray: %0x \n", farray);
    printf("farray+1: %0x \n", farray+1);

    printf("darray: %0x \n", darray);
    printf("darray+1: %0x \n", darray+1);

    printf("carray: %0x \n", carray);
    printf("carray+1: %0x \n", carray+1);

    // task 1c
    //The first cell is the address of the array, and each concecutive cell is at address of the 
    //pevious one + the size of the data type
    
    printf("Pointers and arrays (T1d): ");
    //Initialize the pointers
    iarray2Ptr = iarray2;
    carray2Ptr = carray2;

    //print the arrays using the folder
    printf("\nPrinting array iarray:\n");
    int arrSize = sizeof(iarray2) / sizeof(int);
    for(int i = 0; i < arrSize; i++){   //int array need to have a size, they don't have a null terminating string
        printf("%i, ", iarray2Ptr[i]);
    }

    printf("\nPrinting array carray:\n");   
    int arrSize2 = sizeof(carray2) / sizeof(char);
    for(int i = 0; i < arrSize2; i++){ //char array behave like strings
        printf("%i, ", carray2Ptr[i]);
    }
     printf("\n\n");

}

int main(int argc, char **argv)
{ 

    printf("Print function argument addresses:\n");

    printf("- &argc %p\n", &argc);
    printf("- argv %p\n", argv);
    printf("- &argv %p\n", &argv);
	
	secondary(0);
    
    printf("\nCommand line arg addresses (T1e):\n");
    for(int i=1; i<argc; i++){
        printf("argv[%i] is at address %0x, with content: %s \n", i, argv[i], argv[i]);
    }
    // the memory addresses are higher than the local variables
    
    return 0;
}

void point_at(void *p)
{
    int local;
    static int addr0 = 2;
    static int addr1;

    printf("%p",&local);
    long dist1 = (size_t)&addr6 - (size_t)p;
    long dist2 = (size_t)&local - (size_t)p;
    long dist3 = (size_t)&foo - (size_t)p;

    printf("- dist1: (size_t)&addr6 - (size_t)p: %ld\n", dist1);
    printf("- dist2: (size_t)&local - (size_t)p: %ld\n", dist2);
    printf("- dist3: (size_t)&foo - (size_t)p:  %ld\n", dist3);
    
    printf("Check long type mem size (T1a):\n");
    printf("long size is: %d \n", sizeof(long));    
    //print 4

    printf("- addr0: %p\n", &addr0);
    printf("- addr1: %p\n", &addr1);
}

void foo1()
{
    printf("foo1\n");
}

void foo2()
{
    printf("foo2\n");
}
