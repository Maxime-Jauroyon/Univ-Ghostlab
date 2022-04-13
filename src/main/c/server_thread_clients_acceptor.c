#include <pthread.h>
#include "server_thread_clients_acceptor.h"
#include "server_common.h"
#include "network.h"
#include "print.h"
#include "message.h"
#include "array.h"
#include "memory.h"
#include "server_thread_client_listener.h"

void *gl_thread_clients_acceptor_main(void *arg) {
    gl_printf("clients acceptor thread started.\n");
    
    g_server_socket = gl_socket_create(GHOSTLAB_DEFAULT_SERVER_IP, GHOSTLAB_DEFAULT_SERVER_PORT, GL_SOCKET_TYPE_SERVER);
    
    while (1) {
        int32_t client_fd = gl_socket_server_accept_client(g_server_socket);
        
        if (client_fd >= 0) {
            gl_array_push(g_client_sockets, client_fd);
            uint32_t thread_id = gl_array_get_size(g_threads_client_listener);
            gl_array_push(g_threads_client_listener, gl_malloc(sizeof(pthread_t)));
            pthread_create(g_threads_client_listener[thread_id], 0, gl_thread_client_listener_main, &thread_id);
        } else {
            break;
        }
    }
    
    for (uint32_t i = 0; i < gl_array_get_size(g_threads_client_listener); i++) {
        gl_socket_close(g_client_sockets[i]);
        pthread_join(g_threads_client_listener[i], 0);
        gl_free(g_threads_client_listener[i]);
    }
    gl_array_free(g_client_sockets);
    gl_array_free(g_threads_client_listener);
    gl_socket_close(g_server_socket);
    
    gl_printf("clients acceptor thread stopped.\n");
    
    return 0;
}