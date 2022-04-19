#include <server/thread_tcp_connection.h>
#include <common/log.h>
#include <common/message.h>
#include <common/network.h>
#include <server/shared.h>
#include <pthread.h>
#include "common/string.h"
#include "common/array.h"
#include "common/game.h"

void *gl_thread_tcp_connection_main(void *user_data) {
    uint32_t id = *(uint32_t *)user_data;
    int32_t socket_id = g_client_sockets[id];
    
    gl_log_push("connection %d thread started.\n", id);
   
    if (!g_legacy_protocol) {
        gl_message_t msg = {.type = GL_MESSAGE_TYPE_MULTI, 0};
        gl_message_execute(&msg, socket_id, 0);
    }
    
    {
        gl_message_t msg = {.type = GL_MESSAGE_TYPE_GAME_REQ, 0};
        gl_message_execute(&msg, socket_id, 0);
    }
    
    while (!g_quit) {
        if (gl_message_wait_and_execute(socket_id, GL_MESSAGE_PROTOCOL_TCP) == -1) { // TODO: Pass socket id by pointer
            break;
        }
    }
    
    pthread_mutex_lock(g_gameplay_mutex);
    bool found = false;
    for (uint32_t i = 0; i < gl_array_get_size(g_games); i++) {
        for (uint32_t j = 0; j < gl_array_get_size(g_games[i].players); j++) {
            if (g_games[i].players[j].socket_id == socket_id) {
                g_games[i].players[j].has_quit = true;
                break;
            }
        }
        if (found) {
            break;
        }
    }
    pthread_mutex_unlock(g_gameplay_mutex);
    
    gl_socket_close(&socket_id);
    
    gl_log_push("connection %d thread stopped.\n", id);
    
    return 0;
}