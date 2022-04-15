#include <server/thread_tcp_connection.h>
#include <common/log.h>
#include <common/message.h>
#include <common/network.h>
#include <server/shared.h>
#include "common/string.h"

void *gl_thread_tcp_connection_main(void *user_data) {
    uint32_t id = *(uint32_t *)user_data;
    int32_t socket_id = g_client_sockets[id];
    
    gl_log_push("connection %d thread started.\n", id);
    
    {
        gl_message_t msg = {.type = GL_MESSAGE_TYPE_MULTI, 0};
        gl_message_push_parameter(&msg, (gl_message_parameter_t) {.string_value = gl_string_create_from_ip(g_multicast_ip) });
        gl_message_push_parameter(&msg, (gl_message_parameter_t) {.string_value = gl_string_create_from_number(g_multicast_port, 4) });
        gl_message_send_tcp(socket_id, &msg);
    }
    
    {
        gl_message_t msg = {.type = GL_MESSAGE_TYPE_GAMES, 0};
        gl_message_push_parameter(&msg, (gl_message_parameter_t) {.uint8_value = 0}); // TODO: Change value
        gl_message_send_tcp(socket_id, &msg);
    }
    
    // TODO: OGAMES
    
    while (!g_quit) {
        if (gl_message_wait_and_execute(socket_id, GL_MESSAGE_PROTOCOL_TCP) == -1) {
            break;
        }
    }
    
    gl_socket_close(&socket_id);
    
    gl_log_push("connection %d thread stopped.\n", id);
    
    return 0;
}