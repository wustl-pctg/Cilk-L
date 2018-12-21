#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>

#include <arpa/inet.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fib-consumer.h"

int open_consumer(void) {
    int sock_fd;
    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));

    sock_fd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sock_fd == -1) {
        printf("ERR: could not create client socket (%s)\n", strerror(errno));
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(9001);
    
    int err = -1;

    for (int i = 0; err == -1 && i < 100; i++) {
        err = connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
        usleep(100);
    }

    if (err == -1) {
        printf("ERR: could not connect client to server socket (%s)\n", strerror(errno));
        exit(1);
    }

    /*
    char msg[512];

    err = read(sock_fd, msg, 512);
    if (err == -1) {
        printf("ERR: read error (%s)\n", strerror(errno));
        exit(1);
    }
    msg[err] = 0;
    printf("Received %s\n", msg);
    //close(sock_fd);
    */
    return sock_fd;
}
