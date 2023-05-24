#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#define BUFSIZE 1024 * 10




int main(int argc, char* argv[]) {
    mkfifo(argv[1], 0777);

    int fd = open(argv[1],O_RDONLY);
    char input_buffer[BUFSIZE];

    //Prints out data
    for(int i = 100; i >= 0; i--) {
        read(fd, input_buffer, sizeof(BUFSIZE));
        printf("%s \n", input_buffer);
    }
    return 0;
}