#ifndef GHOSTLAB_SERVER_SHARED_H
#define GHOSTLAB_SERVER_SHARED_H

#include <common/types.h>

struct gl_game_t;
struct gl_player_t;

extern const char g_help[];
extern bool g_should_quit;
extern bool g_use_legacy_protocol;
extern char *g_server_ip;
extern char *g_server_port;
extern char *g_multicast_ip;
extern char *g_multicast_port;
extern int32_t g_server_socket;
extern int32_t *g_client_sockets;
extern void *g_tcp_acceptor_thread;
extern void **g_tcp_listener_threads;
extern void *g_main_mutex;
extern struct gl_game_t *g_games;
extern uint32_t g_game_id;

struct gl_game_t *gl_server_add_game(uint32_t id);

struct gl_player_t *gl_server_add_player(struct gl_game_t *game, const char *id, const char *port, int32_t socket_id);

struct gl_game_t *gl_server_get_game(uint32_t id);

struct gl_game_t *gl_server_get_game_with_socket(int32_t socket_id);

struct gl_player_t *gl_server_get_player_with_socket(int32_t socket_id);

bool gl_server_remove_player_with_socket(int32_t socket_id, uint32_t *game_id);

bool gl_server_all_players_ready(struct gl_game_t *game);

void gl_server_start_game_if_ready(struct gl_game_t *game);

void gl_server_start_game(struct gl_game_t *game);

void gl_server_send_game_list(int32_t socket_id);

void gl_server_free_listeners();

#endif /* GHOSTLAB_SERVER_SHARED_H */
