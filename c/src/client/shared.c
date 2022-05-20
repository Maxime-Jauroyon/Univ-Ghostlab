#include <client/shared.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <common/log.h>
#include <common/maze.h>
#include <common/game.h>
#include <common/array.h>
#include <common/message.h>
#include <common/network.h>

static pthread_mutex_t internal_g_main_mutex = PTHREAD_MUTEX_INITIALIZER;

const char *g_help[11] = {
    "usage: " GHOSTLAB_EXECUTABLE_NAME " [options]\n",
    "\n",
    "ghostlab is an online matchmaking based game where you take upon yourself to become the best ghost hunter!\n",
    "\n",
    "options:\n",
    "\t-i, --ip <server ip>             defines the ip to connect to (" GHOSTLAB_DEFAULT_SERVER_IP " by default).\n",
    "\t-p, --port <server port>         defines the port to connect to (" GHOSTLAB_DEFAULT_SERVER_PORT " by default).\n",
    "\t-u, --udp-port,  <client port>   defines the udp port to use to communicate with other players (" GHOSTLAB_DEFAULT_UDP_PORT " used by default).\n",
    "\t-l, --legacy-protocol            sets the protocol version to legacy (the program will not run any extensions).\n",
    "\t-h, --help                       displays this help message and exits.\n",
    "\t-v, --version                    displays the program's version and exits.\n"
};
bool g_should_quit = false;
bool g_is_server_down = false;
bool g_use_legacy_protocol = false;
char *g_server_ip = 0;
char *g_server_port = 0;
char *g_udp_port = 0;
char *g_multicast_ip = 0;
char *g_multicast_port = 0;
int32_t g_tcp_listener_socket = -1;
int32_t g_multicast_general_socket = -1;
int32_t g_multicast_game_socket = -1;
int32_t g_udp_socket = -1;
void *g_tcp_listener_thread = 0;
void *g_multicast_general_listener_thread = 0;
void *g_multicast_game_listener_thread = 0;
void *g_udp_listener_thread = 0;
void *g_main_mutex = &internal_g_main_mutex;
gl_game_t *g_games = 0;
int32_t g_game_id = -1;
char g_player_id[9] = { 0 };
char g_temp_player_id[9] = { 0 };
char **g_players_message_list = { 0 };
char g_everyone[] = "Everyone";
char g_last_sent_message[201] = { 0 };

int32_t gl_client_connect() {
    g_tcp_listener_socket = gl_socket_create(g_server_ip, g_server_port, GL_SOCKET_TYPE_TCP_CLIENT, 0);
    
    if (g_tcp_listener_socket == -1) {
        return -1;
    }
    
    gl_log_push("connection to server established.\n");
    
    return 0;
}

void gl_client_disconnect(bool close_socket, bool game_req) {
    gl_game_t *game = gl_client_get_game();
    
    if (game && game->started) {
        gl_message_t msg = { .type = GL_MESSAGE_TYPE_IQUIT, .parameters_value = 0 };
        gl_message_send_tcp(g_tcp_listener_socket, &msg);
    } else if (game && !gl_client_get_player()->ready) {
        gl_message_t msg = { .type = GL_MESSAGE_TYPE_UNREG, .parameters_value = 0 };
        gl_message_send_tcp(g_tcp_listener_socket, &msg);
    }
    
    if (close_socket) {
        gl_socket_close(&g_tcp_listener_socket);
    }
    
    if (game_req) {
        gl_message_t msg = { .type = GL_MESSAGE_TYPE_GAME_REQ, .parameters_value = 0 };
        gl_message_send_tcp(g_tcp_listener_socket, &msg);
    }
}

gl_game_t *gl_client_add_game(uint8_t id) {
    gl_game_t game = { 0 };
    
    game.id = id;
    sprintf(game.name, "Game %d", game.id);
    
    gl_array_push(g_games, game);
    
    return &gl_array_get_last(g_games);
}

gl_player_t *gl_client_add_player(struct gl_game_t *game, const char *id) {
    if (!game) {
        return 0;
    }
    
    gl_player_t player = { 0 };
    memcpy(player.id, id, 9);
    
    gl_array_push(game->players, player);
    
    return &gl_array_get_last(game->players);
}

gl_game_t *gl_client_get_game() {
    if (g_game_id == -1) {
        return 0;
    }
    
    return gl_client_get_game_with_id(g_game_id);
}

struct gl_game_t *gl_client_get_game_with_id(uint32_t id) {
    for (uint32_t i = 0; i < gl_array_get_size(g_games); i++) {
        if (g_games[i].id == id) {
            return &g_games[i];
        }
    }
    
    return 0;
}

gl_player_t *gl_client_get_player() {
    gl_game_t *game = gl_client_get_game();
    
    if (!game) {
        return 0;
    }
    
    for (uint32_t i = 0; i < gl_array_get_size(game->players); i++) {
        if (strcmp(game->players[i].id, g_player_id) == 0) {
            return &game->players[i];
        }
    }
    
    return 0;
}

void gl_client_reload_games() {
    gl_message_t msg = { .type = GL_MESSAGE_TYPE_GAME_REQ, .parameters_value = 0 };
    gl_message_send_tcp(g_tcp_listener_socket, &msg);
}

void gl_client_reload_game_maze_size(uint32_t game_id) {
    gl_message_t msg = { .type = GL_MESSAGE_TYPE_SIZE_REQ, .parameters_value = 0 };
    gl_message_push_parameter(&msg, (gl_message_parameter_t) { .uint8_value = game_id });
    gl_message_send_tcp(g_tcp_listener_socket, &msg);
}

void gl_client_reload_game_players_data(uint32_t game_id) {
    if  (gl_client_get_game_with_id(game_id)->started) {
        gl_message_t msg = { .type = GL_MESSAGE_TYPE_GLIS_REQ, .parameters_value = 0 };
        gl_message_send_tcp(g_tcp_listener_socket, &msg);
    } else {
        gl_message_t msg = { .type = GL_MESSAGE_TYPE_LIST_REQ, .parameters_value = 0 };
        gl_message_push_parameter(&msg, (gl_message_parameter_t) { .uint8_value = game_id });
        gl_message_send_tcp(g_tcp_listener_socket, &msg);
    }
}
