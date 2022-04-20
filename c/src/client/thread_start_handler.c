#include <client/thread_start_handler.h>
#include <common/log.h>
#include <common/message.h>
#include <client/shared.h>

void *gl_client_thread_start_handler_main(void *user_data) {
    gl_log_push("start handler thread started.\n");
    
    while (!g_should_quit) {
        if (gl_message_wait_and_execute(g_server_socket, GL_MESSAGE_PROTOCOL_TCP) == -1) {
            break;
        }
    }
    
    gl_log_push("start handler thread stopped.\n");
    
    return 0;
}
