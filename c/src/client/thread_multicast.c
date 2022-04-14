#include <client/thread_multicast.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <printf.h>
#include "common/log.h"
#include "shared.h"
#include "common/message.h"

void *gl_thread_multicast_main(void *user_data) {
    gl_log_push("multicast thread started.\n");
    
    g_multicast_socket = socket(PF_INET,SOCK_DGRAM,0);
    int ok=1;
    int r=setsockopt(g_multicast_socket,SOL_SOCKET,SO_REUSEPORT,&ok,sizeof(ok));
    struct sockaddr_in address_sock;
    address_sock.sin_family=AF_INET;
    address_sock.sin_port=htons(strtol(GHOSTLAB_DEFAULT_MULTICAST_PORT, 0, 10));
    address_sock.sin_addr.s_addr=htonl(INADDR_ANY);
    r=bind(g_multicast_socket,(struct sockaddr *)&address_sock,sizeof(struct sockaddr_in));
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr=inet_addr(GHOSTLAB_DEFAULT_MULTICAST_IP);
    mreq.imr_interface.s_addr=htonl(INADDR_ANY);
    r=setsockopt(g_multicast_socket,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq));
    
    while (!g_quit) {
        if (gl_message_wait_and_execute(g_multicast_socket, GL_MESSAGE_PROTOCOL_UDP) < 0) {
            break;
        }
    }
    
    gl_log_push("multicast thread stopped.\n");
    
    return 0;
}