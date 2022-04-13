#ifndef GHOSTLAB_NETWORK_H
#define GHOSTLAB_NETWORK_H

#include "types.h"

typedef enum gl_socket_type_t {
    GL_SOCKET_TYPE_SERVER,
    GL_SOCKET_TYPE_CLIENT,
    GL_SOCKET_TYPE_MULTICAST_SERVER,
    GL_SOCKET_TYPE_MULTICAST_CLIENT
} gl_socket_type_t;

int32_t gl_socket_create(const char *ip, const char *port, gl_socket_type_t type);

int32_t gl_socket_server_accept_client(int32_t server_fd);

int32_t gl_socket_close(int32_t fd);

#endif /* GHOSTLAB_NETWORK_H */
