#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

//gcc fifo_recv.c -o fifo_recv && gcc fifo_send.c -o fifo_send
//./fifo_recv man muss es erst kompelieren!!!

int main(int argc, char* argv[]) {
    char output[3];

    int fd = open("my_pipe",O_WRONLY);
    for(int i = 100; i >= 0; i--){
        sleep(1);
        sprintf(output, "%d", i);
        write(fd, output, sizeof(output));
    }

    return 0;
}
