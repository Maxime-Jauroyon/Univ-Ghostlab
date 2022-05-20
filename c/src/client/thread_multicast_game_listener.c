#include <client/thread_multicast_game_listener.h>
#include <pthread.h>
#include <sys/socket.h>
#include <common/log.h>
#include <common/game.h>
#include <common/memory.h>
#include <common/message.h>
#include <common/network.h>
#include <client/shared.h>

void *gl_client_thread_multicast_game_listener_main(void *user_data) {
    gl_log_push("multicast game thread started.\n");
    
    g_multicast_game_socket = gl_socket_create(gl_client_get_game()->multicast_ip, gl_client_get_game()->multicast_port, GL_SOCKET_TYPE_MULTICAST_RECEIVER, 0);
    
    while (!g_should_quit) {
        if (gl_message_wait_and_execute(g_multicast_game_socket, GL_MESSAGE_PROTOCOL_UDP) < 0) {
            break;
        }
    }
    
    gl_socket_close(&g_multicast_game_socket);
    
    gl_log_push("multicast game thread stopped.\n");
    
    return 0;
}

void gl_client_thread_multicast_game_listener_start() {
    gl_client_thread_multicast_game_listener_close();
    g_multicast_game_listener_thread = gl_malloc(sizeof(pthread_t));
    pthread_create(g_multicast_game_listener_thread, 0, gl_client_thread_multicast_game_listener_main, 0);
}

void gl_client_thread_multicast_game_listener_close() {
    if (g_multicast_game_listener_thread) {
        gl_socket_close(&g_multicast_game_socket);
        pthread_join(*(pthread_t *)g_multicast_game_listener_thread, 0);
        gl_free(g_multicast_game_listener_thread);
        g_multicast_game_listener_thread = 0;
    }
}
