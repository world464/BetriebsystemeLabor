#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int nChildProcess(int pNumber){
        for(int i = 1; pNumber >= i; i++) {
            pid_t pid = fork();
            if (pid == 0) {
                printf("This is the child process %d\n", i);
                sleep(5);
                printf("End child process %d\n", i);
                exit(0);
            }
            sleep(1);
        }

        for(int i = 1; pNumber >= i; i++){
            wait(NULL);
        }
        printf("Parent process finished.\n");
        exit(0);
}
/*
int main(int argc, char *argv[]){
    int number = atoi(argv[1]);
    nChildProcess(number);
}*/