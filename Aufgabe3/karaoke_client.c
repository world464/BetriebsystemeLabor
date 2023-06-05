#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include "unistd.h"
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
    error(sock);
// prepare socket for multicast (reuse port for multiple clients)
    u_int val = 1;
    err = setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &val, sizeof(val));
    error(err);

// bind socket to multicast address
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
    bzero((char *)&addr, addrlen);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(MY_PORT);
    err = bind(sock, (struct sockaddr *) &addr, addrlen);
    error(err);

// add socket to multicast group
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(MY_GROUP);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    err = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
    error(err);

// receive messages from server
    printf("Waiting for messages from server...\n");
    while (1) {
        char message[100];
        err = recvfrom(sock, message, sizeof(message), 0, (struct sockaddr *) &addr,&addrlen);
        error(err);
        printf("%s", message);
        sleep(1);
    }


    /*while(read(fd, input_buffer, sizeof(BUFSIZE)) > 0) {
        printf("%s \n", input_buffer);
    }*/
}
