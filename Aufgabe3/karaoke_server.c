#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>

#include "common.h"


#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
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

    //File: /home/user/Documents/BetriebsystemeLabor/Aufgabe3/lyrics-1.txt

    //open file

    FILE* fp;
    fp = fopen("/home/user/Documents/BetriebsystemeLabor/Aufgabe3/lyrics-1.txt", "r");
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
    //while (variable) {

    ///home/user/Documents/BetriebsystemeLabor/Aufgabe3/lyrics-1.txt

    size_t len = 0;
    ssize_t read;
    char* line = NULL;


    //reads a line

   /* while((read = getline(&line, &len, fp) != -1)){
        //printf("Retieved line of lenth %zu :\n", read);
        printf("%s", line);

        char massage[50] = "Hey ich bin Johannes Test";
        err = sendto(sock, massage, sizeof(massage), 0, (struct sockaddr *) &addr,sizeof(addr));
        error(err);
        sleep(1);

    }//lesen wir das gesamte fiel aus
*/
    char massage[50];
    while(fgets(massage, sizeof (massage), fp)){
        printf("%s", massage);
        err = sendto(sock, massage, sizeof(massage), 0, (struct sockaddr *) &addr,sizeof(addr));
        error(err);
        sleep(1);

    }

    fclose(fp);
    if(line){
        free(line);
        exit(EXIT_SUCCESS);
    }



    // Reset the file pointer to the start of the data
    //Zero bytes have been read
    //lseek(fd, 0, SEEK_SET);

    //}*/
}
