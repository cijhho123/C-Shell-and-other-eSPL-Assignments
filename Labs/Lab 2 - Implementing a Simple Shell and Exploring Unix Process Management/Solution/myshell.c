#include <linux/limits.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>

#include "LineParser.h"

int checkString(char* src, char* dst, int dstSize);
int checkFlag(int argc, char **argv, char* flag);

void execute(cmdLine *pCmdLine); 
void performCD(cmdLine *pCmdLine);
void performAlarm(cmdLine *pCmdLine);
void performBlast(cmdLine *pCmdLine);

//global variables
int isInDebug = 0;


int main(int argc, char **argv){
    isInDebug = checkFlag(argc, argv, "-d"); 

    while(1){
        char buf[PATH_MAX];
        if(getcwd(buf, PATH_MAX) == NULL)
            exit(1); 
        printf("%s\n", buf);
        
        int size = 2048;
        char input[size];
        if( fgets(input, size, stdin) == NULL)
            exit(1);

        if(strcmp(input, "quit\n") == 0)
            break;
        
        cmdLine* currLine = parseCmdLines(input);
        if( currLine == NULL)
            exit(1);
        
        if(strcmp((*currLine).arguments[0], "cd") == 0)
            performCD(currLine);
        else if(strcmp((*currLine).arguments[0], "alarm") == 0)
            performAlarm(currLine);
        else if(strcmp((*currLine).arguments[0], "blast") == 0)
            performBlast(currLine);
        else
            execute(currLine);
            

        freeCmdLines(currLine);
    }

    exit(0);
}

void execute(cmdLine *pCmdLine){
    int childPID = fork();

    if(childPID == 0) {
        //debug mode
        if(isInDebug)
            fprintf(stderr, "PID: %d\tCommand: %s\n", getpid(), (*pCmdLine).arguments[0]);

        
        //printf("input: %s\noutput: %s",(*pCmdLine).inputRedirect, (*pCmdLine).outputRedirect);
        //printf("output: %s\n",(*pCmdLine).outputRedirect);


        //Input redirection
        if((*pCmdLine).inputRedirect != NULL){
            printf("changing input stream\n");
            close(STDIN_FILENO);
            open((*pCmdLine).inputRedirect, O_RDONLY, 0777);
        }
        //Output redirection
        if((*pCmdLine).outputRedirect != NULL){
            printf("changing output stream\n");
            close(STDOUT_FILENO);
            open((*pCmdLine).outputRedirect, O_RDWR	 | O_CREAT, 0777);
        }

        execvp((*pCmdLine).arguments[0], (*pCmdLine).arguments);
        perror("execv failed! ");
        _exit(1); 
    }
    
    if((*pCmdLine).blocking != 0){
        printf("waiting....\n");
        waitpid(childPID, NULL, 0);
    }
}


//return 1 if the debug flag was provided, 0 otherwise.
int checkFlag(int argc, char **argv, char* flag){
    for(int i = 1; i < argc; i++){
        //check for a debug flag
        if(strcmp(argv[i], flag) == 0){
            printf("%s is on\n",flag);
            return 1;
        }    
    }

    printf("%s is off\n",flag);
    return 0;
}

//return 1 if src's initial string is equls to dst (assumes src is always larger than dst)
int checkString(char* src, char* dst, int dstSize){
    for(int i=0; i < dstSize; i++)
        if(src[i] != dst[i])
            return 1;    

    return 0;
}

void performCD(cmdLine *pCmdLine){
    //printf("doing CD command\n");

    /*
    char buf[PATH_MAX];
    getcwd(buf, PATH_MAX);
    strcat(buf, "/");
    strcat(buf, (*pCmdLine).arguments[1]);
    replaceCmdArg(pCmdLine,1, buf);
    */


    //printf("1:%s 2:%s 3:%s \n", (*pCmdLine).arguments[0], (*pCmdLine).arguments[1], (*pCmdLine).arguments[2]);

    if(chdir((*pCmdLine).arguments[1]) != 0){
        fprintf(stderr, "cd command failed with path %s with error ", (*pCmdLine).arguments[1]);
        perror("");
    }
}

void performAlarm(cmdLine *pCmdLine){
    int result = kill(atoi((*pCmdLine).arguments[1]), SIGCONT);
    if(result == 0)
        printf("Alarm succesed.\n");
    else
        printf("Alarm failed.\n");
}

void performBlast(cmdLine *pCmdLine){
    int result = kill(atoi((*pCmdLine).arguments[1]), SIGKILL);
    if(result == 0)
        printf("Blast succesed.\n");
    else
        printf("Blast failed.\n");
}