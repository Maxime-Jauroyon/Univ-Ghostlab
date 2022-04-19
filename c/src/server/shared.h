#ifndef GHOSTLAB_SERVER_SHARED_H
#define GHOSTLAB_SERVER_SHARED_H

#include <common/types.h>

extern const char g_help[];

extern bool g_should_quit;
extern bool g_use_legacy_protocol;

extern int32_t g_server_socket;
extern int32_t *g_client_sockets;
extern void *g_thread_tcp;
extern void **g_threads_client_listener;

extern char *g_server_ip;
extern char *g_server_port;
extern char *g_multicast_ip;
extern char *g_multicast_port;

extern void *g_main_mutex;
extern struct gl_game_t *g_games;

#endif /* GHOSTLAB_SERVER_SHARED_H */
