#include <unistd.h>
#include <stdio.h>

int simpleFork(int arcg, char *argv[]) {
    fork();
    fork();
    fork();//2x4 Hello = 8

    printf("Hello!\n");
    sleep(1);
}