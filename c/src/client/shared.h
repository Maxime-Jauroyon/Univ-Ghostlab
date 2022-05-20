#ifndef GHOSTLAB_CLIENT_SHARED_H
#define GHOSTLAB_CLIENT_SHARED_H

#include <common/types.h>

struct gl_game_t;
struct gl_player_t;

extern const char *g_help[11];
extern bool g_should_quit;
extern bool g_is_server_down;
extern bool g_use_legacy_protocol;
extern char *g_server_ip;
extern char *g_server_port;
extern char *g_udp_port;
extern char *g_multicast_ip;
extern char *g_multicast_port;
extern int32_t g_tcp_listener_socket;
extern int32_t g_multicast_general_socket;
extern int32_t g_multicast_game_socket;
extern int32_t g_udp_socket;
extern void *g_tcp_listener_thread;
extern void *g_multicast_general_listener_thread;
extern void *g_multicast_game_listener_thread;
extern void *g_udp_listener_thread;
extern void *g_main_mutex;
extern struct gl_game_t *g_games;
extern int32_t g_game_id;
extern char g_player_id[9];
extern char g_temp_player_id[9];
extern char **g_players_message_list;
extern char g_everyone[];
extern char g_last_sent_message[201];

// Connects the client to the server.
int32_t gl_client_connect();

// Sends a disconnect message to the server (and closes the socket if needed).
void gl_client_disconnect(bool close_socket, bool game_req);

// Adds a game to the list of games.
struct gl_game_t *gl_client_add_game(uint8_t id);

// Adds a player to the list of players of a game.
struct gl_player_t *gl_client_add_player(struct gl_game_t *game, const char *id);

// Returns the game in progress.
struct gl_game_t *gl_client_get_game();

// Returns the game with a given id.
struct gl_game_t *gl_client_get_game_with_id(uint32_t id);

// Returns the client's player.
struct gl_player_t *gl_client_get_player();

// Reloads the list of games.
void gl_client_reload_games();

// Reloads the maze data of a game.
void gl_client_reload_game_maze_size(uint32_t game_id);

// Reloads the players' data of a game.
void gl_client_reload_game_players_data(uint32_t game_id);

#endif /* GHOSTLAB_CLIENT_SHARED_H */
