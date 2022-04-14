#ifndef GHOSTLAB_SERVER_SHARED_H
#define GHOSTLAB_SERVER_SHARED_H

#include <common/types.h>

struct gl_message_t;

extern const char g_help[];
extern bool g_quit;
extern int32_t g_server_socket;
extern int32_t *g_client_sockets;
extern int32_t g_multicast_socket;
extern void *g_thread_tcp;
extern void *g_thread_multicast;
extern void **g_threads_client_listener;
extern char *g_server_port;
extern char *g_multicast_ip;
extern char *g_multicast_port;

#endif /* GHOSTLAB_SERVER_SHARED_H */
