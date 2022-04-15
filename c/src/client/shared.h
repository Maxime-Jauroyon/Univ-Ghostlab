#ifndef GHOSTLAB_CLIENT_SHARED_H
#define GHOSTLAB_CLIENT_SHARED_H

#include <common/types.h>

extern const char g_help[];
extern bool g_quit;
extern bool g_server_down;
extern bool g_legacy_protocol;
extern char *g_server_ip;
extern char *g_server_port;
extern char *g_player_id;
extern char *g_udp_port;
extern int32_t g_server_tcp_socket;
extern char *g_multicast_ip;
extern char *g_multicast_port;
extern int32_t g_multicast_server_socket;
extern void *g_multicast_server_thread;
extern int32_t g_udp_socket;
extern void *g_udp_thread;

#endif /* GHOSTLAB_CLIENT_SHARED_H */
