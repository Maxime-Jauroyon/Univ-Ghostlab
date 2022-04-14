#ifndef GHOSTLAB_CLIENT_SHARED_H
#define GHOSTLAB_CLIENT_SHARED_H

#include <common/types.h>

extern const char g_help[];
extern bool g_quit;
extern bool g_server_down;
extern char *server_ip;
extern char *server_port;
extern char *player_name;
extern char *udp_port;
extern int32_t g_multicast_socket;
extern void *g_thread_multicast;

#endif /* GHOSTLAB_CLIENT_SHARED_H */
