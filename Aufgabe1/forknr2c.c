#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

//Nur manchmal in der richtigen Reinfolge

int forkExample2(int argc, char *argv[], int n) {


    for(int i = 1; n >= i; i++) {

        pid_t pid = fork();
        if (pid == 0) {
            printf("This is child process %d\n", i);
            sleep(5);
            exit(0);

        }
    }

    wait(NULL);
    printf("Parent process finished");
    exit(0);







}


