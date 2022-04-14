#include <server/thread_multicast.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include "common/log.h"
#include "shared.h"
#include "common/array.h"
#include "common/message.h"
#include "common/network.h"

static pthread_cond_t g_cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t g_array_lock = PTHREAD_MUTEX_INITIALIZER;
static gl_message_t **g_queue = 0;

void *gl_thread_multicast_main(void *user_data) {
    gl_log_push("multicast thread started.\n");
    
    g_multicast_socket = socket(PF_INET, SOCK_DGRAM, 0);
    
    struct addrinfo *first_info;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    int r = getaddrinfo(GHOSTLAB_DEFAULT_MULTICAST_IP, GHOSTLAB_DEFAULT_MULTICAST_PORT, &hints, &first_info);
    struct sockaddr *saddr=first_info->ai_addr;
    
    pthread_mutex_lock(&g_array_lock);
    
    while (!g_quit) {
        if (gl_array_get_size(g_queue) == 0) {
            pthread_cond_wait(&g_cond, &g_array_lock);
        }
        
        if (g_multicast_socket < 0) {
            break;
        }
    
        if (gl_array_get_size(g_queue) > 0) {
            gl_message_t *msg = gl_array_get_first(g_queue);
            gl_array_remove(g_queue, 0); // NOLINT
    
            r = getaddrinfo(GHOSTLAB_DEFAULT_MULTICAST_IP, GHOSTLAB_DEFAULT_MULTICAST_PORT, &hints, &first_info);
    
            if(r == 0 && first_info != NULL) {
                r = gl_message_sendto(g_multicast_socket, msg, first_info->ai_addr);
        
                if (r == -1) {
                    break;
                }
            } else {
                break;
            }
        }
    }
    
    pthread_mutex_unlock(&g_array_lock);
    
    gl_socket_close(&g_multicast_socket);
    
    gl_array_free(g_queue);
    
    gl_log_push("multicast thread stopped.\n");
    
    return 0;
}

void gl_thread_multicast_wake_up() {
    pthread_mutex_lock(&g_array_lock);
    pthread_cond_signal(&g_cond);
    pthread_mutex_unlock(&g_array_lock);
}

void gl_thread_multicast_send(struct gl_message_t *msg) {
    pthread_mutex_lock(&g_array_lock);
    gl_array_push(g_queue, msg); // NOLINT
    pthread_cond_signal(&g_cond);
    pthread_mutex_unlock(&g_array_lock);
}
