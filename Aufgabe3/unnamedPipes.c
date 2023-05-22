#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define BUFSIZE 1024

int main() {
    int fd[2], ret = 0;
    char output_buffer[] = "my message";
    char input_buffer[BUFSIZ];
    pid_t pid;
    // create a oneway communication channel, an unnamed pipe
    pipe(fd);
    pid = fork();

    if (pid == 0) {
// child process:
// close the read end of the pipe
        close(fd[0]);
        printf("Child intiates communication by writing data to the write end of the pipe.\n");
// child process writes to the write end of the pipe
        write(fd[1], output_buffer, sizeof(output_buffer));
    } else {
// parent process:
// close the write end of the pipe
        close(fd[1]);
// parent proces reads from the read end of the pipe
        ret = read(fd[0], input_buffer, sizeof(input_buffer));
        printf("The data read from parent process is: \"%s\" \n", input_buffer);
    }
    return 0;
}
