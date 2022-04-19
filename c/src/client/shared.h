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

extern void *g_gameplay_mutex;
extern struct gl_game_t *g_games;
extern int32_t g_current_game_id;
extern bool g_current_game_player_ready;
extern char g_current_player_id[9];

struct gl_game_t *gl_get_current_game();

void gl_connect_to_server();

void gl_free_games();

#endif /* GHOSTLAB_CLIENT_SHARED_H */
