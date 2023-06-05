#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>

#include "common.h"
#include <fcntl.h>
#define BUFSIZE 1024
#define MAXLINESIZE 1024

typedef struct unsinged unsinged;

void error(int err){
    if(err == -1){
        perror("Error");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {

    //open file

    FILE* fp;
    fp = fopen(PATH_LYRICS, "r");
    if (fp == NULL) {
        printf("%s /n", "Failed to open file.");
        exit(EXIT_FAILURE);
    }

    //error status
    int err = 0;

    // set up socket
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    // prepare socket for multicast (reuse port for multiple clients)
    u_int val = 1;
    err = setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &val, sizeof(val));
    error(err);

    // bind socket to multicast address
    struct sockaddr_in addr;
    bzero((char *) &addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(MY_GROUP);
    addr.sin_port = htons(MY_PORT);


    printf("Server ready to send messages.\n");


    char* line = NULL;


 char massage[50];
    while(1){
        fseek(fp, 0, SEEK_SET);
        while (fgets(massage, sizeof(massage), fp)) {
            printf("%s", massage);
            err = sendto(sock, massage, sizeof(massage), 0, (struct sockaddr *) &addr, sizeof(addr));
            error(err);
            sleep(1);

        }
    }
    fclose(fp);
    if(line){
        free(line);
        exit(EXIT_SUCCESS);
    }
}
