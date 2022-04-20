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
    gl_log_push("tcp acceptor thread started.\n");
    
    g_tcp_acceptor_socket = gl_socket_create(g_server_ip, g_server_port, GL_SOCKET_TYPE_TCP_SERVER, 0);
    
    while (!g_should_quit) {
        int32_t client_socket = gl_tcp_accept(g_tcp_acceptor_socket);
        
        if (g_tcp_acceptor_socket == -1) {
            break;
        }
        
        if (client_socket == -1) {
            continue;
        }
    
        gl_array_push(g_tcp_listener_sockets, client_socket);
        uint32_t thread_id = gl_array_get_size(g_tcp_listener_threads);
        gl_array_push(g_tcp_listener_threads, gl_malloc(sizeof(pthread_t)));
        pthread_create(g_tcp_listener_threads[thread_id], 0, gl_thread_tcp_listener_main, &thread_id);
    }
    
    gl_server_free_listeners();
    
    gl_log_push("tcp acceptor thread stopped.\n");
    
    return 0;
}
