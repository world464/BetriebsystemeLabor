#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

#define BUFSIZE 1024

int unnamedPipes() {
    int fd[2], ret = 0;
    char output_buffer[2];
    char input_buffer[BUFSIZE];
    pid_t pid;
    // create a oneway communication channel, an unnamed pipe
    pipe(fd);
    int i;
    for (i = 10; i >= 0; i--) {
        pid = fork();

        if (pid == 0) {
// child process:
            sleep(1);
            sprintf(output_buffer, "%d", i);
// child process writes to the write end of the pipe
            write(fd[1], output_buffer, sizeof(output_buffer));
            exit(0);
        } else {
        // parent process:


// parent proces reads from the read end of the pipe
            ret = read(fd[0], input_buffer, sizeof(input_buffer));
            printf("%s \n", input_buffer);
        }
    }
    close(fd[0]);
    close(fd[1]);
    return 0;
}
