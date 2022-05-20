#ifndef GHOSTLAB_SERVER_SHARED_H
#define GHOSTLAB_SERVER_SHARED_H

#include <common/types.h>

struct gl_game_t;
struct gl_player_t;
struct gl_message_t;
struct gl_ip_socket_t;
enum gl_movement_t;

extern const char *g_help[12];
extern bool g_should_quit;
extern bool g_use_legacy_protocol;
extern char *g_server_ip;
extern char *g_server_port;
extern char *g_multicast_ip;
extern char *g_multicast_port;
extern struct gl_ip_socket_t *g_ip_sockets;
extern int32_t g_tcp_acceptor_socket;
extern int32_t *g_tcp_listener_sockets;
extern void *g_ghost_handler_thread;
extern void *g_tcp_acceptor_thread;
extern void **g_tcp_listener_threads;
extern void *g_main_mutex;
extern struct gl_game_t *g_games;
extern uint32_t g_game_id;

// Adds a game to the list of games.
struct gl_game_t *gl_server_add_game(uint32_t id);

// Adds a player to the list of players of a game.
struct gl_player_t *gl_server_add_player(struct gl_game_t *game, const char *id, const char *port, int32_t socket_id);

// Returns a game with a given id.
struct gl_game_t *gl_server_get_game(uint32_t id);

// Returns a game where a client is playing with its socket.
struct gl_game_t *gl_server_get_game_with_socket(int32_t socket_id);

// Returns a player associated with a given socket.
struct gl_player_t *gl_server_get_player_with_socket(int32_t socket_id);

// Removes a player associated with a given socket.
bool gl_server_remove_player_with_socket(int32_t socket_id, uint32_t *game_id);

// Checks if all players of a game are ready.
bool gl_server_all_players_ready(struct gl_game_t *game);

// Starts a game if every players' are ready.
void gl_server_start_game_if_ready(struct gl_game_t *game);

// Starts a game.
void gl_server_start_game(struct gl_game_t *game);

// Sends the list of games message to a given socket.
void gl_server_send_game_list(int32_t socket_id);

// Sends a move message to a given socket.
void gl_server_send_move(int32_t socket_id, struct gl_message_t *msg, enum gl_movement_t movement);

// Frees all listeners socket and thread.
void gl_server_free_listeners();

#endif /* GHOSTLAB_SERVER_SHARED_H */
