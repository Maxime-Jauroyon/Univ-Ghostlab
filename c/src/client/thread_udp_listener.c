#include <client/thread_udp_listener.h>
#include <sys/socket.h>
#include <common/log.h>
#include <common/message.h>
#include <common/network.h>
#include <client/shared.h>

void *gl_client_thread_udp_listener_main(void *user_data) {
    gl_log_push("udp listener thread started.\n");
    
    g_udp_socket = gl_socket_create(GHOSTLAB_DEFAULT_UDP_IP, g_udp_port, GL_SOCKET_TYPE_UDP_RECEIVER, 0);
    
    while (!g_should_quit) {
        if (gl_message_wait_and_execute(g_udp_socket, GL_MESSAGE_PROTOCOL_UDP) < 0) {
            break;
        }
    }
    
    gl_socket_close(&g_udp_socket);
    
    gl_log_push("udp listener thread stopped.\n");
    
    return 0;
}
