#ifndef GHOSTLAB_CLIENT_SHARED_H
#define GHOSTLAB_CLIENT_SHARED_H

#include <common/types.h>

struct gl_game_t;
struct gl_player_t;

extern const char g_help[];
extern bool g_should_quit;
extern bool g_is_server_down;
extern bool g_use_legacy_protocol;
extern char *g_server_ip;
extern char *g_server_port;
extern char *g_udp_port;
extern char *g_multicast_ip;
extern char *g_multicast_port;
extern int32_t g_server_socket;
extern int32_t g_multicast_server_socket;
extern int32_t g_udp_socket;
extern void *g_multicast_server_thread;
extern void *g_udp_thread;
extern void *g_main_mutex;
extern struct gl_game_t *g_games;
extern int32_t g_game_id;
extern char g_player_id[9];

int32_t gl_client_connect();

void gl_client_disconnect(bool close_socket);

bool gl_client_is_player_id_valid(char *player_id);

struct gl_game_t *gl_client_add_game(uint8_t id);

struct gl_player_t *gl_client_add_player(struct gl_game_t *game, const char *id);

struct gl_game_t *gl_client_get_game();

struct gl_player_t *gl_client_get_player();

#endif /* GHOSTLAB_CLIENT_SHARED_H */
