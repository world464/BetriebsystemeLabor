#include <unistd.h>
#include <stdio.h>

int simpleFork(int arcg, char *argv[]) {
    fork();//2x4 Hello = 8
    fork();//2x2 Hello = 4 Hello
    fork();//2xHello

    printf("Hello!\n");
    sleep(1);
}