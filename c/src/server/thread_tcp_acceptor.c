#include <server/thread_tcp_acceptor.h>
#include <pthread.h>
#include <common/log.h>
#include <common/array.h>
#include <common/memory.h>
#include <common/network.h>
#include <common/message.h>
#include <server/shared.h>
#include <server/thread_tcp_listener.h>

void *gl_thread_tcp_acceptor_main(void *user_data) {
    gl_log_push("tcp thread started.\n");
    
    g_server_socket = gl_socket_create(g_server_ip, g_server_port, GL_SOCKET_TYPE_TCP_SERVER, 0);
    
    while (!g_should_quit) {
        int32_t client_fd = gl_tcp_accept(g_server_socket);
        
        if (g_server_socket == -1) {
            break;
        }
        
        if (client_fd == -1) {
            continue;
        }
    
        gl_array_push(g_client_sockets, client_fd);
        uint32_t thread_id = gl_array_get_size(g_threads_client_listener);
        gl_array_push(g_threads_client_listener, gl_malloc(sizeof(pthread_t)));
        pthread_create(g_threads_client_listener[thread_id], 0, gl_thread_tcp_listener_main, &thread_id);
    }
    
    for (uint32_t i = 0; i < gl_array_get_size(g_threads_client_listener); i++) {
        gl_socket_close(&g_client_sockets[i]);
        pthread_join(*(pthread_t *)g_threads_client_listener[i], 0);
        gl_free(g_threads_client_listener[i]);
    }
    gl_array_free(g_client_sockets);
    gl_array_free(g_threads_client_listener);
    gl_socket_close(&g_server_socket);
    
    gl_log_push("tcp thread stopped.\n");
    
    return 0;
}
