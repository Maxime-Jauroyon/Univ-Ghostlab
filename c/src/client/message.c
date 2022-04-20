#include <client/message.h>
#include <common/game.h>
#include <common/string.h>
#include <common/network.h>
#include <common/message.h>
#include <client/shared.h>
#include <pthread.h>
#include <common/memory.h>
#include <client/thread_multicast_general_listener.h>
#include <string.h>
#include "gui.h"
#include "common/array.h"

static uint32_t g_message_list_res_game_id = 0;

static void message_games(gl_message_t *msg, int32_t socket_id, void *user_data) {
    gl_game_free_all_with_exception(g_games, g_game_id);
}

static void message_ogames(gl_message_t *msg, int32_t socket_id, void *user_data) {
    if (g_game_id != msg->parameters_value[0].uint8_value) {
        gl_client_add_game(msg->parameters_value[0].uint8_value);
    }
}

static void message_regok(gl_message_t *msg, int32_t socket_id, void *user_data) {
    g_game_id = msg->parameters_value[0].uint8_value;
    
    bool game_created = false;
    
    if (gl_client_get_game() == 0) {
        gl_client_add_game(g_game_id);
        game_created = true;
    }
    
    gl_client_add_player(gl_client_get_game(), g_player_id);
    
    if (game_created) {
        gl_client_create_game_popup_close();
    } else {
        gl_client_join_game_popup_close();
    }
}

static void message_regno(gl_message_t *msg, int32_t socket_id, void *user_data) {
    gl_client_error(2);
}

static void message_unrok(gl_message_t *msg, int32_t socket_id, void *user_data) {
    g_game_id = -1;
    
    if (g_should_quit) {
        gl_socket_close(&g_tcp_acceptor_socket);
    }
}

static void message_size_res(gl_message_t *msg, int32_t socket_id, void *user_data) {
    uint32_t game_id = msg->parameters_value[0].uint8_value;
    gl_game_t *game = gl_client_get_game_with_id(game_id);
    
    game->maze_size.x = msg->parameters_value[1].uint16_value;
    game->maze_size.y = msg->parameters_value[2].uint16_value;
}

static void message_list_res(gl_message_t *msg, int32_t socket_id, void *user_data) {
    uint32_t game_id = msg->parameters_value[0].uint8_value;
    gl_game_t *game = gl_client_get_game_with_id(game_id);
    
    for (uint32_t i = 0; i < gl_array_get_size(game->players); i++) {
        if (strcmp(game->players->id, g_player_id) != 0) {
            gl_array_remove(game->players, i);
        }
    }
    
    g_message_list_res_game_id = game_id;
}

static void message_playr(gl_message_t *msg, int32_t socket_id, void *user_data) {
    gl_game_t *game = gl_client_get_game_with_id(g_message_list_res_game_id);
    
    char *player_id = (char *)msg->parameters_value[0].string_value;
    
    if (strcmp(player_id, g_player_id) != 0) {
        gl_player_t player = { 0 };
        memcpy(player.id, msg->parameters_value[0].string_value, 8);
        gl_array_push(game->players, player);
    }
}

static void message_gobye(gl_message_t *msg, int32_t socket_id, void *user_data) {
    g_game_id = -1;
    
    gl_socket_close(&g_tcp_acceptor_socket);
    
    if (!g_should_quit) {
        gl_client_connect();
    }
}

static void message_multi(gl_message_t *msg, int32_t socket_id, void *user_data) {
    if (!g_multicast_ip) {
        g_multicast_ip = gl_cstring_create_from_ip(msg->parameters_value[0].string_value);
    }
    
    if (!g_multicast_port) {
        g_multicast_port = gl_cstring_create_from_string(msg->parameters_value[1].string_value);
    }
    
    g_multicast_general_listener_thread = gl_malloc(sizeof(pthread_t));
    pthread_create(g_multicast_general_listener_thread, 0, gl_client_thread_multicast_general_listener_main, 0);
}

static void message_shutd(gl_message_t *msg, int32_t socket_id, void *user_data) {
    g_is_server_down = true;
    g_should_quit = true;
}

void gl_client_message_add_functions() {
    gl_message_definitions()[GL_MESSAGE_TYPE_GAMES]->function = message_games;
    gl_message_definitions()[GL_MESSAGE_TYPE_OGAMES]->function = message_ogames;
    gl_message_definitions()[GL_MESSAGE_TYPE_REGOK]->function = message_regok;
    gl_message_definitions()[GL_MESSAGE_TYPE_REGNO]->function = message_regno;
    gl_message_definitions()[GL_MESSAGE_TYPE_UNROK]->function = message_unrok;
    gl_message_definitions()[GL_MESSAGE_TYPE_SIZE_RES]->function = message_size_res;
    gl_message_definitions()[GL_MESSAGE_TYPE_LIST_RES]->function = message_list_res;
    gl_message_definitions()[GL_MESSAGE_TYPE_PLAYR]->function = message_playr;
    gl_message_definitions()[GL_MESSAGE_TYPE_GOBYE]->function = message_gobye;
    gl_message_definitions()[GL_MESSAGE_TYPE_MULTI]->function = message_multi;
    gl_message_definitions()[GL_MESSAGE_TYPE_SHUTD]->function = message_shutd;
}
