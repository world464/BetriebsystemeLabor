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
    mkfifo("my_pipe", 0777);

    int fd = open("my_pipe",O_RDONLY);
    char input_buffer[BUFSIZE];

    //Wait until receives data
    while(read(fd, input_buffer, sizeof(BUFSIZE)) <= 0){
        printf("%s \n", "Wartet!");
    }

    // Reset the file pointer to the start of the pixel data
    lseek(fd, 0, SEEK_SET);

    //Prints out data
    for(int i = 100; i >= 0; i--) {
        read(fd, input_buffer, sizeof(BUFSIZE));
        printf("%s \n", input_buffer);
    }
    return 0;
}