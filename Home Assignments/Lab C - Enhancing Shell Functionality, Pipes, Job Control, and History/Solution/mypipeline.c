#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>

  #include <sys/types.h>
  #include <sys/wait.h>

void execute();

int main(int argc, char **argv){
    execute();
    return 0;
}

void execute(){
    //create and open a pipe
    int buf[2]; //buf[0] is for reading, buf[1] is for writing
    if( pipe(buf) == -1){
        perror("pipe error"); 
        exit(1);
    }

    fprintf(stderr, "parent_process>forking child\n");

    int childPID = fork();

    if(childPID != 0)
        fprintf(stderr, "parent_process>created process with id: %d\n", childPID);
    

    //only child will enter
    if(childPID == 0) {

        //closing standart output
        close(STDOUT_FILENO);
        
        //duplicating the write-end of the pipe
        dup(buf[1]);

        //closing the original file desc
        close(buf[1]);
        fprintf(stderr,"child1>redirecting stdout to the write end of the pipe…\n");

        //execute ls -l command
        char* command = "ls";
        char* args[] = {"ls", "-l", NULL};
        fprintf(stderr,"child1>going to execute cmd: ls -l \n");
        execvp(command, args);
        
        //close the child process if the exec failed
        perror("execv failed on first child! \n");
        _exit(1); 
    }


    //closing the write-end of the pipe
    fprintf(stderr, "parent_process>closing the write end of the pipe…\n");
    close(buf[1]);

    //forking the second child
    int childPID2 = fork();

    //only child will enter
    if(childPID2 == 0) {

        //closing standart input
        close(STDIN_FILENO);

        //duplicate the read-end of the pipe
        //int copy_fd = dup(buf[0]);
        dup(buf[0]);
        
        //closing the original file desc
        close(buf[0]);
        fprintf(stderr,"child2>redirecting stdin to the write end of the pipe…\n");

        //execute tail -n 2 command
        char* command = "tail";
        char* args[] = {"tail", "-n" "2", NULL};
        fprintf(stderr,"child2>going to execute cmd: grep loop \n");
        execvp(command, args);
        
        //close the child process if the exec failed
        perror("execv failed on second child! \n");
        _exit(1); 
    }

    //closing the read-end of the pipe
    fprintf(stderr, "parent_process>closing the read end of the pipe… \n");
    close(buf[0]);


    //waiting for child processes 
    fprintf(stderr, "parent_process>waiting for child processes to terminate… \n");
    waitpid(childPID,  NULL, 0);
    waitpid(childPID2, NULL, 0);

    //exiting
    fprintf(stderr, "parent_process>exiting… \n");
    exit(0);
}
