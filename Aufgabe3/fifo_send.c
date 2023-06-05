#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

//gcc fifo_recv.c -o fifo_recv && gcc fifo_send.c -o fifo_send
//./fifo_recv man muss es erst kompelieren!!!

int main(int argc, char* argv[]) {
    char output[4];

    int fd = open(argv[1],O_WRONLY);
    if(fd < 0) {
        printf("%s /n", "Failed to open file.");
        exit(EXIT_FAILURE);
    }
    for(int i = 100; i >= 0; i--){
        sleep(1);
        sprintf(output, "%d", i);
        write(fd, output, sizeof(output));
        printf("%s %d\n", "Nachricht versandt: ", i);
    }

    return 0;
}
