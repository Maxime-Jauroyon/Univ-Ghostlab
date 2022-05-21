#include <common/network.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <common/message.h>

static pthread_mutex_t g_close_mutex = PTHREAD_MUTEX_INITIALIZER;

static int32_t gl_socket_create_tcp(const char *ip, const char *port, gl_socket_type_t type) {
    int32_t fd = socket(PF_INET, SOCK_STREAM, 0);
    
    int32_t port_int;
    if ((port_int = (int32_t)strtol(port, NULL, 10)) == 0) {
        goto error;
    }
    
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(port_int);
    
    if (type == GL_SOCKET_TYPE_TCP_SERVER) {
        address.sin_addr.s_addr = htonl(INADDR_ANY);
    
        int ok = 1;
        if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &ok, sizeof(ok)) == -1) {
            goto error;
        }
    
        if (bind(fd, (struct sockaddr *)&address, sizeof(struct sockaddr_in)) == -1) {
            goto error;
        }
    
        if (listen(fd, 0) == -1) {
            goto error;
        }
    } else {
        inet_aton(ip, &address.sin_addr);
    
        if (connect(fd, (struct sockaddr *)&address, sizeof(struct sockaddr_in)) == -1) {
            goto error;
        }
    }
    
    return fd;
    
    error:
    gl_socket_close(&fd);
    return -1;
}

static int32_t gl_socket_create_udp(const char *ip, const char *port, gl_socket_type_t type, struct sockaddr **address) {
    int32_t fd = socket(PF_INET, SOCK_DGRAM, 0);
    
    if (type == GL_SOCKET_TYPE_UDP_SENDER || type == GL_SOCKET_TYPE_MULTICAST_SENDER) {
        struct addrinfo *first_info;
        struct addrinfo hints;
        bzero(&hints, sizeof(struct addrinfo));
        
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_DGRAM;
        
        if (getaddrinfo(ip, port, &hints, &first_info) == -1 || first_info == NULL) {
            goto error;
        }
        
        *address = first_info->ai_addr;
    } else {
        int32_t port_int;
        if ((port_int = (int32_t)strtol(port, NULL, 10)) == 0) {
            goto error;
        }
        
        if (type == GL_SOCKET_TYPE_MULTICAST_RECEIVER) {
            int ok = 1;
            if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &ok, sizeof(ok)) == -1) {
                goto error;
            }
        }
        
        struct sockaddr_in address_sock;
        address_sock.sin_family = AF_INET;
        address_sock.sin_port = htons(port_int);
        address_sock.sin_addr.s_addr = htonl(INADDR_ANY);
        if (bind(fd, (struct sockaddr *)&address_sock, sizeof(struct sockaddr_in)) == -1) {
            goto error;
        }
        
        if (type == GL_SOCKET_TYPE_MULTICAST_RECEIVER) {
            struct ip_mreq mreq;
            mreq.imr_multiaddr.s_addr = inet_addr(ip);
            mreq.imr_interface.s_addr = htonl(INADDR_ANY);
            if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) == -1) {
                goto error;
            }
        }
    }
    
    return fd;
    
    error:
    gl_socket_close(&fd);
    return -1;
}

int32_t gl_socket_create(const char *ip, const char *port, gl_socket_type_t type, struct sockaddr **address) {
    if (type == GL_SOCKET_TYPE_TCP_CLIENT || type == GL_SOCKET_TYPE_TCP_SERVER) {
        return gl_socket_create_tcp(ip, port, type);
    }
    
    return gl_socket_create_udp(ip, port, type, address);
}

int32_t gl_socket_close(int32_t *fd) {
    pthread_mutex_lock(&g_close_mutex);
    
    if (*fd != -1) {
        shutdown(*fd, SHUT_RD);
        int32_t r = close(*fd);
        *fd = -1;
        pthread_mutex_unlock(&g_close_mutex);
        return r;
    }
    
    pthread_mutex_unlock(&g_close_mutex);
    return 0;
}

int32_t gl_tcp_accept(int32_t server_fd, struct sockaddr_in *addr) {
    struct sockaddr_in address;
    socklen_t size_client = sizeof(address);
    int32_t r = accept(server_fd, (struct sockaddr *)&address, &size_client);
    if (addr) {
        *addr = address;
    }
    return r;
}
