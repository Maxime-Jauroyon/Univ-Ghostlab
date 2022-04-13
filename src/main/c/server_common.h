#ifndef GHOSTLAB_SERVER_COMMON_H
#define GHOSTLAB_SERVER_COMMON_H

#include "types.h"

extern const char g_help[];
extern bool g_quit;
extern int32_t g_server_socket;
extern int32_t *g_client_sockets;
extern void *g_thread_client_acceptors;
extern void **g_threads_client_listener;

#endif /* GHOSTLAB_SERVER_COMMON_H */
