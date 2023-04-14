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
                exit(0);
            }
        }

        while(wait(NULL)>0);
        printf("Parent process finished.\n");
        exit(0);
}
/*
int main(int argc, char *argv[]){
    int number = atoi(argv[1]);
    nChildProcess(number);
}*/