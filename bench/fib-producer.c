#include <sys/socket.h>
#include <sys/types.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>

void producer_thread_func(void *args) {
    int sock_fd;
    int comm_fd;
    struct sockaddr_storage client_addr;
    struct sockaddr_in servaddr;

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(9001); 

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        printf("ERR: could not create server socket (%s)\n", strerror(errno));
        exit(1);
    }

    int err = bind(sock_fd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    if (err == -1) {
        printf("ERR: could not bind server socket (%s)\n", strerror(errno));
        exit(1);
    }

    err = listen(sock_fd, 1);
    if (err == -1) {
        printf("ERR: error in listen() (%s)\n", strerror(errno));
        exit(1);
    }

    socklen_t addr_size = sizeof(client_addr);
    comm_fd = accept(sock_fd, (struct sockaddr *)&client_addr, &addr_size);
    close(sock_fd);

    if (comm_fd == -1) {
        printf("ERR: error in accept() (%s)\n", strerror(errno));
        exit(1);
    }

    write(comm_fd, "start", strlen("start"));
    close(comm_fd);
}
