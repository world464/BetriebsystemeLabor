#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int forkExample3(int argc, char *argv[], int n) {
//Hier wird ein Childprozess erzeugt, danach wartet der Vater (wait) und der Childprozess wird ausgefuehrt.
//Danach erstellt der Vater wieder ein Childprozess und wartet wieder. Das passiert fuer jede Interation.
    for(int i = 1; n >= i; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            printf("This is child process %d\n", i);
            sleep(5);
            exit(0);
        }
        else
            wait(NULL);
    }

    printf("This is the parent process with process .\n");
    exit(0);
}

int forkExample2(int argc, char *argv[], int n) {
    int x = 5;
    //Hier werden die Childprozesse nacheinander erzeugt und dann beginnt der erste,zweite, ...
    for(int i = 1; x >= i; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            printf("Start child process %d\n", i);
            sleep(5);
            printf("End child process %d\n", i);
            exit(0);
        }
        sleep(1);
    }

    for(int i = 1; x >= i; i++){
        wait(NULL);
    }
    printf("This is the parent process with process .\n");
    exit(0);
}