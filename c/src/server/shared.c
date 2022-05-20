#include <server/shared.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <common/game.h>
#include <common/maze.h>
#include <common/array.h>
#include <common/memory.h>
#include <common/string.h>
#include <common/message.h>
#include <common/network.h>

static pthread_mutex_t internal_g_main_mutex = PTHREAD_MUTEX_INITIALIZER;

const char *g_help[12] = {
    "usage: " GHOSTLAB_EXECUTABLE_NAME " [options]\n",
    "\n",
    "ghostlab is an online matchmaking based game where you take upon yourself to become the best ghost hunter!\n",
    "\n",
    "options:\n",
    "\t-i, --ip <server ip>                       defines the ip to use (" GHOSTLAB_DEFAULT_SERVER_IP " by default).\n",
    "\t-p, --port <server port>                   defines the port to use (" GHOSTLAB_DEFAULT_SERVER_PORT " by default).\n",
    "\t-I, --multi-ip <server multicast ip>       defines the multicast ip to use (" GHOSTLAB_DEFAULT_MULTICAST_IP " by default).\n",
    "\t-P, --multi-port <server multicast port>   defines the multicast port to use (" GHOSTLAB_DEFAULT_MULTICAST_PORT " by default).\n",
    "\t-l, --legacy-protocol                      sets the protocol version to legacy (the program will not run any extensions).\n",
    "\t-h, --help                                 displays this help message and terminates.\n",
    "\t-v, --version                              displays the program's version and terminates.\n"
};
bool g_should_quit = false;
bool g_use_legacy_protocol = false;
char *g_server_ip = 0;
char *g_server_port = 0;
char *g_multicast_ip = 0;
char *g_multicast_port = 0;
int32_t g_tcp_acceptor_socket = -1;
int32_t *g_tcp_listener_sockets = 0;
gl_ip_socket_t *g_ip_sockets = 0;
void *g_ghost_handler_thread = 0;
void *g_tcp_acceptor_thread = 0;
void **g_tcp_listener_threads = 0;
void *g_main_mutex = &internal_g_main_mutex;
gl_game_t *g_games = 0;
uint32_t g_game_id = 0;

gl_game_t *gl_server_add_game(uint32_t id) {
    gl_game_t game = { 0 };
    
    game.id = id;
    sprintf(game.name, "Game %d", game.id);
    game.multicast_ip = gl_calloc(16, 1);
    game.multicast_port = gl_calloc(5, 1);
    memcpy(game.multicast_ip, g_multicast_ip, 15);
    sprintf(game.multicast_port, "%d", (uint32_t)(strtol(g_multicast_port, 0, 10) + id + 1));
    
    gl_array_push(g_games, game);
    
    return &gl_array_get_last(g_games);
}

gl_player_t *gl_server_add_player(struct gl_game_t *game, const char *id, const char *port, int32_t socket_id) {
    if (!game) {
        return 0;
    }
    
    gl_player_t player = { 0 };
    player.socket_id = socket_id;
    memcpy(player.id, id, 8);
    
    bool already_exists = false;
    for (uint32_t i = 0; i < gl_array_get_size(game->players); i++) {
        if (player.socket_id != socket_id && strcmp(game->players[i].udp_port, port) == 0) {
            already_exists = true;
            // TODO: Send message to inform that UDP is already used.
        }
    }
    
    if (!already_exists) {
        memcpy(player.udp_port, port, 4);
    }
    
    gl_array_push(game->players, player);
    
    return &gl_array_get_last(game->players);
}

gl_game_t *gl_server_get_game(uint32_t id) {
    for (uint32_t i = 0; i < gl_array_get_size(g_games); i++) {
        if (g_games[i].id == id) {
            return &g_games[i];
        }
    }
    
    return 0;
}

gl_game_t *gl_server_get_game_with_socket(int32_t socket_id) {
    for (uint32_t i = 0; i < gl_array_get_size(g_games); i++) {
        for (uint32_t j = 0; j < gl_array_get_size(g_games[i].players); j++) {
            if (g_games[i].players[j].socket_id == socket_id) {
                return &g_games[i];
            }
        }
    }
    
    return 0;
}

gl_player_t *gl_server_get_player_with_socket(int32_t socket_id) {
    for (uint32_t i = 0; i < gl_array_get_size(g_games); i++) {
        for (uint32_t j = 0; j < gl_array_get_size(g_games[i].players); j++) {
            if (g_games[i].players[j].socket_id == socket_id) {
                return &g_games[i].players[j];
            }
        }
    }
    
    return 0;
}

bool gl_server_remove_player_with_socket(int32_t socket_id, uint32_t *game_id) {
    bool removed = false;
    
    for (uint32_t i = 0; i < gl_array_get_size(g_games); i++) {
        for (uint32_t j = 0; j < gl_array_get_size(g_games[i].players); j++) {
            if (g_games[i].players[j].socket_id == socket_id) {
                if (game_id) {
                    *game_id = g_games[i].id;
                }
    
                removed = true;
                
                gl_array_remove(g_games[i].players, j);
                
                if (gl_array_get_size(g_games[i].players) == 0) {
                    gl_game_free(&g_games[i]);
                    gl_array_remove(g_games, i);
                }
                
                break;
            }
        }
        
        if (removed) {
            break;
        }
    }
    
    return removed;
}

bool gl_server_all_players_ready(struct gl_game_t *game) {
    if (!game) {
        return false;
    }
    
    for (uint32_t i = 0; i < gl_array_get_size(game->players); i++) {
        if (!game->players[i].ready) {
            return false;
        }
    }
    
    return true;
}

void gl_server_start_game_if_ready(struct gl_game_t *game) {
    if (!game) {
        return;
    }
    
    if (gl_server_all_players_ready(game)) {
        gl_server_start_game(game);
    }
}

void gl_server_start_game(struct gl_game_t *game) {
    gl_pos_t size = gl_game_get_maze_size(game);
    
    game->maze = gl_maze_generate(size.x, size.y);
    game->ghosts = gl_game_generate_ghosts(game->maze, gl_array_get_size(game->players) + 1);
    game->players = gl_game_generate_players(game->maze, game->players, game->ghosts);
    game->started = true;
    
    for (uint32_t i = 0; i < gl_array_get_size(game->players); i++) {
        gl_message_t response = { .type = GL_MESSAGE_TYPE_WELCO, 0 };
        gl_message_push_parameter(&response, (gl_message_parameter_t) { .uint8_value = game->id });
        gl_message_push_parameter(&response, (gl_message_parameter_t) { .uint16_value = gl_array_get_size(game->maze->grid[0]) });
        gl_message_push_parameter(&response, (gl_message_parameter_t) { .uint16_value = gl_array_get_size(game->maze->grid) });
        gl_message_push_parameter(&response, (gl_message_parameter_t) { .uint8_value = gl_array_get_size(game->ghosts) });
        gl_message_push_parameter(&response, (gl_message_parameter_t) { .string_value = gl_string_create_from_ip(game->multicast_ip) });
        gl_message_push_parameter(&response, (gl_message_parameter_t) { .string_value = gl_string_create_from_number(game->multicast_port, 4) });
        gl_message_send_tcp(game->players[i].socket_id, &response);
    }
    
    for (uint32_t i = 0; i < gl_array_get_size(game->players); i++) {
        gl_message_t response = { .type = GL_MESSAGE_TYPE_POSIT, 0 };
        gl_message_push_parameter(&response, (gl_message_parameter_t) { .string_value = gl_string_create_from_cstring(game->players[i].id) });
        gl_message_push_parameter(&response, (gl_message_parameter_t) { .string_value = gl_string_create_from_uint32(game->players[i].pos.x, 3) });
        gl_message_push_parameter(&response, (gl_message_parameter_t) { .string_value = gl_string_create_from_uint32(game->players[i].pos.y, 3) });
        gl_message_send_tcp(game->players[i].socket_id, &response);
    }
}

void gl_server_send_game_list(int32_t socket_id) {
    uint32_t num_games_not_started = 0;
    
    for (uint32_t i = 0; i < gl_array_get_size(g_games); i++) {
        if (!g_games[i].started) {
            num_games_not_started++;
        }
    }
    
    gl_message_t response = { .type = GL_MESSAGE_TYPE_GAMES, 0 };
    gl_message_push_parameter(&response, (gl_message_parameter_t) { .uint8_value = num_games_not_started });
    gl_message_send_tcp(socket_id, &response);
    
    for (uint32_t i = 0; i < gl_array_get_size(g_games); i++) {
        if (!g_games[i].started) {
            response = (gl_message_t) { .type = GL_MESSAGE_TYPE_OGAMES, 0 };
            gl_message_push_parameter(&response, (gl_message_parameter_t) { .uint8_value = g_games[i].id });
            gl_message_push_parameter(&response, (gl_message_parameter_t) { .uint8_value = gl_array_get_size(g_games[i].players) });
            gl_message_send_tcp(socket_id, &response);
        }
    }
}

void gl_server_send_move(int32_t socket_id, struct gl_message_t *msg, enum gl_movement_t movement) {
    gl_game_t *game = gl_server_get_game_with_socket(socket_id);
    gl_player_t *player = gl_server_get_player_with_socket(socket_id);
    uint32_t quantity = gl_string_strtol(msg->parameters_value[0].string_value);
    uint32_t removed = gl_game_move_player(game, player, quantity, movement);
    
    if (removed > 0) {
        gl_message_t response = {.type = GL_MESSAGE_TYPE_MOVEF, 0};
        gl_message_push_parameter(&response, (gl_message_parameter_t) { .string_value = gl_string_create_from_uint32(player->pos.x, 3) });
        gl_message_push_parameter(&response, (gl_message_parameter_t) { .string_value = gl_string_create_from_uint32(player->pos.y, 3) });
        gl_message_push_parameter(&response, (gl_message_parameter_t) { .string_value = gl_string_create_from_uint32(player->score, 4) });
        gl_message_send_tcp(socket_id, &response);
    } else {
        gl_message_t response = {.type = GL_MESSAGE_TYPE_MOVE_RES, 0};
        gl_message_push_parameter(&response, (gl_message_parameter_t) { .string_value = gl_string_create_from_uint32(player->pos.x, 3) });
        gl_message_push_parameter(&response, (gl_message_parameter_t) { .string_value = gl_string_create_from_uint32(player->pos.y, 3) });
        gl_message_send_tcp(socket_id, &response);
    }
}

void gl_server_free_listeners() {
    for (uint32_t i = 0; i < gl_array_get_size(g_tcp_listener_threads); i++) {
        gl_socket_close(&g_tcp_listener_sockets[i]);
        pthread_join(*(pthread_t *)g_tcp_listener_threads[i], 0);
        gl_free(g_tcp_listener_threads[i]);
    }
    
    gl_array_free(g_tcp_listener_sockets);
    gl_array_free(g_tcp_listener_threads);
    
    gl_socket_close(&g_tcp_acceptor_socket);
}
