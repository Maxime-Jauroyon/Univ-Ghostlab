#include <server/thread_tcp_listener.h>
#include <pthread.h>
#include <common/log.h>
#include <common/string.h>
#include <common/message.h>
#include <common/network.h>
#include <server/shared.h>

void *gl_thread_tcp_listener_main(void *user_data) {
    uint32_t id = *(uint32_t *)user_data;
    int32_t socket_id = g_tcp_listener_sockets[id];
    
    gl_log_push("tcp listener %d thread started.\n", id);
    
    if (!g_use_legacy_protocol) {
        gl_message_t response = {.type = GL_MESSAGE_TYPE_MULTI, 0};
        gl_message_push_parameter(&response, (gl_message_parameter_t) {.string_value = gl_string_create_from_ip(g_multicast_ip) });
        gl_message_push_parameter(&response, (gl_message_parameter_t) {.string_value = gl_string_create_from_number(g_multicast_port, 4) });
        gl_message_send_tcp(socket_id, &response);
    }
    
    gl_server_send_game_list(socket_id);
    
    while (!g_should_quit) {
        if (gl_message_wait_and_execute(socket_id, GL_MESSAGE_PROTOCOL_TCP) == -1) {
            break;
        }
    }
    
    pthread_mutex_lock(g_main_mutex);
    gl_server_remove_player_with_socket(socket_id, 0);
    pthread_mutex_unlock(g_main_mutex);
    
    gl_socket_close(&socket_id);
    
    gl_log_push("tcp listener %d thread stopped.\n", id);
    
    return 0;
}