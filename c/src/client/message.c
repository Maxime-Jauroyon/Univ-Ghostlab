#include <client/message.h>
#include <printf.h>
#include <string.h>
#include <unistd.h>
#include "common/message.h"
#include "common/log.h"
#include "shared.h"
#include "common/string.h"
#include "common/game.h"
#include "common/array.h"
#include "common/network.h"

void message_games(gl_message_t *msg, int32_t socket_id, void *user_data) {
    gl_free_games();
    for (uint32_t i = 0; i < msg->parameters_value[0].uint8_value; i++) {
        gl_message_wait_and_execute_no_lock(socket_id, GL_MESSAGE_PROTOCOL_TCP);
    }
}

void message_ogames(gl_message_t *msg, int32_t socket_id, void *user_data) {
    gl_game_t game = { 0 };
    game.id = msg->parameters_value[0].uint8_value;
    sprintf(game.name, "Game %d", game.id);
    for (uint32_t i = 0; i < msg->parameters_value[1] .uint8_value; i++) {
        gl_player_t player = { 0 };
        gl_array_push(game.players, player);
    }
    gl_array_push(g_games, game);
}

void message_regok(gl_message_t *msg, int32_t socket_id, void *user_data) {
    g_current_game_id = msg->parameters_value[0].uint8_value;
}

void message_regno(gl_message_t *msg, int32_t socket_id, void *user_data) {
    gl_log_push("there was an issue joining the game\n");
    g_current_game_id = -1;
}

void message_unrok(gl_message_t *msg, int32_t socket_id, void *user_data) {
    g_current_game_id = -1;
}

void message_gobye(gl_message_t *msg, int32_t socket_id, void *user_data) {
    gl_socket_close(&g_server_tcp_socket);
    g_current_game_id = -1;
    
    if (g_quit) {
        return;
    }
    
    gl_connect_to_server();
}

void message_multi(gl_message_t *msg, int32_t socket_id, void *user_data) {
    if (!g_multicast_ip && !g_multicast_port) {
        g_multicast_ip = gl_cstring_create_from_ip(msg->parameters_value[0].string_value);
        g_multicast_port = gl_cstring_create_from_string(msg->parameters_value[1].string_value);
    }
}

void message_shutd(gl_message_t *msg, int32_t socket_id, void *user_data) {
    g_server_down = true;
    g_quit = true;
}

void gl_message_add_functions() {
    gl_message_definitions()[GL_MESSAGE_TYPE_GAMES]->function = message_games;
    gl_message_definitions()[GL_MESSAGE_TYPE_OGAMES]->function = message_ogames;
    gl_message_definitions()[GL_MESSAGE_TYPE_REGOK]->function = message_regok;
    gl_message_definitions()[GL_MESSAGE_TYPE_REGNO]->function = message_regno;
    gl_message_definitions()[GL_MESSAGE_TYPE_UNROK]->function = message_unrok;
    gl_message_definitions()[GL_MESSAGE_TYPE_GOBYE]->function = message_gobye;
    gl_message_definitions()[GL_MESSAGE_TYPE_MULTI]->function = message_multi;
    gl_message_definitions()[GL_MESSAGE_TYPE_SHUTD]->function = message_shutd;
}
