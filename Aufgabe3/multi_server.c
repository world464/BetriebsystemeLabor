#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>

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
sendto(sock, message, sizeof(message), 0, (struct sockaddr *) &addr,sizeof(addr));
 sleep(5);
}
}