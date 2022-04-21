#ifndef GHOSTLAB_NETWORK_H
#define GHOSTLAB_NETWORK_H

#include <common/types.h>

struct sockaddr;
struct gl_message_t;
struct sockaddr_in;

typedef enum gl_socket_type_t {
    GL_SOCKET_TYPE_TCP_SERVER,
    GL_SOCKET_TYPE_TCP_CLIENT,
    GL_SOCKET_TYPE_UDP_SENDER,
    GL_SOCKET_TYPE_UDP_RECEIVER,
    GL_SOCKET_TYPE_MULTICAST_SENDER,
    GL_SOCKET_TYPE_MULTICAST_RECEIVER
} gl_socket_type_t;

typedef struct gl_ip_socket_t {
    int32_t socket_id;
    char ip[16];
} gl_ip_socket_t;

int32_t gl_socket_create(const char *ip, const char *port, gl_socket_type_t type, struct sockaddr **address);

int32_t gl_socket_close(int32_t *fd);

int32_t gl_tcp_accept(int32_t server_fd, struct sockaddr_in *addr);

#endif /* GHOSTLAB_NETWORK_H */
