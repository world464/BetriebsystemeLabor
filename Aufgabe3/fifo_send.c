#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
    char output[3];
    mkfifo("my_pipe",0777);
    int fd = open("my_pipe",O_WRONLY);
    for(int i = 100; i >= 0; i--){
        sleep(1);
        sprintf(output, "%d", i);
        write(fd, output, sizeof(output));
    }


    return 0;
}
