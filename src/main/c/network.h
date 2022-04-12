#ifndef GHOSTLAB_NETWORK_H
#define GHOSTLAB_NETWORK_H

#include "types.h"

typedef enum gl_socket_type_t {
    GL_SOCKET_TYPE_SERVER,
    GL_SOCKET_TYPE_CLIENT,
    GL_SOCKET_TYPE_MULTICAST
} gl_socket_type_t;

int gl_socket_create(const char *ip, const char *port, gl_socket_type_t type);

int gl_socket_server_accept_client(int server_fd);

int gl_socket_close(int fd);

#endif /* GHOSTLAB_NETWORK_H */
