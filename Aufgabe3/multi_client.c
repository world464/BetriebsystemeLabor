#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <strings.h>

#include "common.h"

int main(int argc, char *argv[])
{
 // set up socket
int sock = socket(AF_INET, SOCK_DGRAM, 0);

// prepare socket for multicast (reuse port for multiple clients)
u_int val = 1;
setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &val, sizeof(val));

// bind socket to multicast address
struct sockaddr_in addr;
socklen_t addrlen = sizeof(addr);
bzero((char *)&addr, addrlen);
addr.sin_family = AF_INET;
addr.sin_addr.s_addr = htonl(INADDR_ANY);
addr.sin_port = htons(MY_PORT);
bind(sock, (struct sockaddr *) &addr, addrlen);

// add socket to multicast group
struct ip_mreq mreq;
mreq.imr_multiaddr.s_addr = inet_addr(MY_GROUP);
mreq.imr_interface.s_addr = htonl(INADDR_ANY);
setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));

// receive messages from server
printf("Waiting for messages from server...\n");
while (1) {
char message[50];
recvfrom(sock, message, sizeof(message), 0, (struct sockaddr *) &addr,&addrlen);
printf("%s: message = \"%s\"\n", inet_ntoa(addr.sin_addr), message);
}
}
