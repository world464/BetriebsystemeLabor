#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

//Nur manchmal in der richtigen Reinfolge

int forkExample2(int argc, char *argv[]) {

    pid_t pid = 1;
    for(int i = 1; 5> i; i++) {
        pid_t pid = fork();
        if (pid == 0){
            pid = getpid();
            pid_t ppid = getppid();
            printf("This is child process %d\n", i);
            sleep(5);
            exit(0);
        }


    }
    if (pid > 0) {
        pid = wait(NULL);
        printf("Parent process finished");
        exit(0);
    }






}


