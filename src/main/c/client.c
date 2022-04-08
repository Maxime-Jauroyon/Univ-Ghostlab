#include "types.h"
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <printf.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv) {
    struct sockaddr_in adress_sock;
    adress_sock.sin_family = AF_INET;
    adress_sock.sin_port = htons(4243);
    inet_aton("127.0.0.1",&adress_sock.sin_addr);
    int fd = socket(PF_INET, SOCK_STREAM, 0);
    int r=connect(fd, (struct sockaddr *)&adress_sock, sizeof(struct sockaddr_in));
    if(r != -1) {
        while (1) {
            char buf[219];
            printf("command: ");
            fflush(stdout);
            char *line = 0;
            size_t max_size = 0;
            ssize_t size = getline(&line, &max_size, stdin);
            send(fd, line, size - 1, 0);
            free(line);
        }
        
        close(fd);
    }
    
    return EXIT_SUCCESS;
}
