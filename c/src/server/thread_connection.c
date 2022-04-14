#include <server/thread_connection.h>
#include <common/log.h>
#include <common/message.h>
#include <common/network.h>
#include <server/shared.h>

void *gl_thread_connection_main(void *user_data) {
    uint32_t id = *(uint32_t *)user_data;
    int32_t socket_id = g_client_sockets[id];
    
    gl_log_push("connection %d thread started.\n", id);
    
    {
        gl_message_t msg = {.type = GL_MESSAGE_TYPE_GAMES, 0};
        gl_message_push_parameter(&msg, (gl_message_parameter_t) {.uint8_value = 0});
        gl_message_send(socket_id, &msg);
    }
    
    while (!g_quit) {
        if (gl_message_wait_and_execute(socket_id, GL_MESSAGE_PROTOCOL_TCP) < 0) {
            break;
        }
    }
    
    gl_socket_close(&socket_id);
    
    gl_log_push("connection %d thread stopped.\n", id);
    
    return 0;
}