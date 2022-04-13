#include "server_thread_client_listener.h"
#include "message.h"
#include "server_common.h"
#include "print.h"
#include "network.h"

void *gl_thread_client_listener_main(void *arg) {
    uint32_t id = *(uint32_t *)arg;
    
    gl_printf("client listener %d thread started.\n", id);
    
    while (1) {
        gl_message_t msg = {0};
        int32_t r = gl_message_read(g_client_sockets[id], &msg);
        
        if (r > 0) {
            gl_message_printf(&msg);
            gl_message_free(&msg);
        } else {
            break;
        }
    }
    
    gl_socket_close(g_client_sockets[id]);
    
    gl_printf("client listener %d thread stopped.\n", id);
    
    return 0;
}