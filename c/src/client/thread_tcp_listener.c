#include <client/thread_tcp_listener.h>
#include <common/log.h>
#include <common/message.h>
#include <client/shared.h>

void *gl_client_thread_tcp_listener_main(void *user_data) {
    gl_log_push("tcp listener thread started.\n");
    
    while (!g_should_quit) {
        if (gl_message_wait_and_execute(g_tcp_listener_socket, GL_MESSAGE_PROTOCOL_TCP) == -1) {
            break;
        }
    }
    
    gl_log_push("tcp listener thread stopped.\n");
    
    if (g_use_legacy_protocol) {
        g_should_quit = true;
        g_is_server_down = true;
    }
    
    return 0;
}
