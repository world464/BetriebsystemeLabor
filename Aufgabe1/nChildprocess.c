#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int ret(double pNumber){
int number = pNumber;
printf(" %d ", number);
int zaehler = 1;

// fork a process
pid_t pid = fork();
if (pid == 0) {
// child process
pid = getpid();
pid_t ppid = getppid();
printf("This is child process (pid=%d) with parent %d.\n", pid, ppid);

if(number > 0) {
ret(number - 1);
}
// child sleeps for 5 secods
    sleep(5);
exit(0);
} else if (pid > 0) {
// parent process
pid = getpid();
printf("This is the parent process with process id %d.\n", pid);
// wait for child to finish
pid = wait(NULL);
if (pid == -1) {
printf("Wait failed.\n");
exit(1);
}
printf("Child %d finished.\n", pid);
exit(0);

} else {

printf("Fork failed.\n");
exit(1);

}
}


int main(int argc, char *argv[]) {
    double zahl = atoi(argv[1]);//n childprocess;
    ret(zahl);
}
