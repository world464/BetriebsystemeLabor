#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#define BUFSIZE 1024

int receive(int argc, char* argv[]) {
    int fd = open("my_pipe",O_RDONLY);
    char input_buffer[BUFSIZE];
    for(int i = 100; i >= 0; i--) {
        read(fd, input_buffer, sizeof(BUFSIZE));
        printf("%s \n", input_buffer);
    }
    return 0;
}