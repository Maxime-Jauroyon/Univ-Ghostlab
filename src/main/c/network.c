#include "network.h"
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

int gl_socket_create(const char *ip, const char *port, gl_socket_type_t type) {
    struct sockaddr_in address;
    
    address.sin_family = AF_INET;
    address.sin_port = htons(strtol(port, NULL, 10));
    
    if (type == GL_SOCKET_TYPE_SERVER) {
        address.sin_addr.s_addr = htonl(INADDR_ANY);
    } else if (type == GL_SOCKET_TYPE_CLIENT) {
        inet_aton(ip, &address.sin_addr);
    }
    
    int fd = socket(PF_INET, SOCK_STREAM, 0);
    int r = 0;
    
    if (type == GL_SOCKET_TYPE_SERVER) {
        r = bind(fd, (struct sockaddr *)&address, sizeof(struct sockaddr_in));
    
        if (r == -1) {
            return r;
        }
    
        r = listen(fd, 0);
    } else if (type == GL_SOCKET_TYPE_CLIENT) {
        r = connect(fd, (struct sockaddr *)&address, sizeof(struct sockaddr_in));
    }
    
    if (r == -1) {
        return r;
    }
    
    return fd;
}

int gl_socket_server_accept_client(int server_fd) {
    struct sockaddr_in address;
    socklen_t size_client = sizeof(address);
    return accept(server_fd, (struct sockaddr *)&address, &size_client);
}

int gl_socket_close(int fd) {
    return close(fd);
}