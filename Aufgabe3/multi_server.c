#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>

#include "common.h"

void error(int err){
    if(err == -1){
        perror("Error");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
 {
    int err = 0;
     // set up socket
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    error(err);
    // prepare socket for multicast (reuse port for multiple clients)
    u_int val = 1;
    err = setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &val, sizeof(val));
    error(err);

    // bind socket to multicast address
    struct sockaddr_in addr;
    bzero((char *)&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(MY_GROUP);
    addr.sin_port = htons(MY_PORT);

    // send message (current time) every 5 seconds
    printf("Server ready to send messages.\n");
    while (1) {
    time_t t = time(0);
    char message[50];
    sprintf(message, "time is %-24.24s", ctime(&t));

    printf("sending: %s\n", message);
    err = sendto(sock, message, sizeof(message), 0, (struct sockaddr *) &addr,sizeof(addr));
    error(err);
     sleep(5);
    }
}