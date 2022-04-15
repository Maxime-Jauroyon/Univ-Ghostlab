#ifndef GHOSTLAB_NETWORK_H
#define GHOSTLAB_NETWORK_H

#include <common/types.h>

struct sockaddr;
struct gl_message_t;

typedef enum gl_socket_type_t {
    GL_SOCKET_TYPE_TCP_SERVER,
    GL_SOCKET_TYPE_TCP_CLIENT,
    GL_SOCKET_TYPE_UDP_SENDER,
    GL_SOCKET_TYPE_UDP_RECEIVER,
    GL_SOCKET_TYPE_MULTICAST_SENDER,
    GL_SOCKET_TYPE_MULTICAST_RECEIVER
} gl_socket_type_t;

int32_t gl_socket_create(const char *ip, const char *port, gl_socket_type_t type, struct sockaddr **address);

int32_t gl_socket_close(int32_t *fd);

int32_t gl_tcp_accept(int32_t server_fd);

#endif /* GHOSTLAB_NETWORK_H */
