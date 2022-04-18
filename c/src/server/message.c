#include <stdio.h>
#include <string.h>
#include "message.h"
#include "common/message.h"
#include "common/log.h"
#include "common/game.h"
#include "common/array.h"
#include "shared.h"
#include "common/string.h"
#include "common/network.h"

void message_newpl(gl_message_t *msg, int32_t socket_id, void *user_data) {
    if (gl_array_get_size(g_games) == 255) {
        gl_message_t new_msg = {.type = GL_MESSAGE_TYPE_REGNO, 0};
        gl_message_send_tcp(socket_id, &new_msg);
        return;
    }
    gl_game_t game = { 0 };
    game.id = gl_array_get_size(g_games);
    sprintf(game.name, "Game %d", game.id);
    gl_player_t player = { 0 };
    memcpy(player.id, msg->parameters_value[0].string_value, 8);
    memcpy(player.port, msg->parameters_value[1].string_value, 4);
    player.socket_id = socket_id;
    gl_array_push(game.players, player);
    gl_array_push(g_games, game);
    gl_message_t new_msg = {.type = GL_MESSAGE_TYPE_REGOK, 0};
    gl_message_push_parameter(&new_msg, (gl_message_parameter_t) {.uint8_value = game.id});
    gl_message_send_tcp(socket_id, &new_msg);
}

void message_regis(gl_message_t *msg, int32_t socket_id, void *user_data) {
    if (gl_array_get_size(g_games) < msg->parameters_value[3].uint8_value) {
        gl_message_t new_msg = {.type = GL_MESSAGE_TYPE_REGNO, 0};
        gl_message_send_tcp(socket_id, &new_msg);
        return;
    }
    uint8_t game_id = msg->parameters_value[3].uint8_value;
    gl_player_t player = { 0 };
    memcpy(player.id, msg->parameters_value[0].string_value, 8);
    memcpy(player.port, msg->parameters_value[1].string_value, 4);
    player.socket_id = socket_id;
    gl_array_push(g_games[game_id].players, player);
    gl_message_t new_msg = {.type = GL_MESSAGE_TYPE_REGOK, 0};
    gl_message_push_parameter(&new_msg, (gl_message_parameter_t) {.uint8_value = game_id});
    gl_message_send_tcp(socket_id, &new_msg);
}

void message_game_req(gl_message_t *msg, int32_t socket_id, void *user_data) {
    {
        gl_message_t new_msg = {.type = GL_MESSAGE_TYPE_GAMES, 0};
        gl_message_push_parameter(&new_msg, (gl_message_parameter_t) {.uint8_value = gl_array_get_size(g_games)});
        gl_message_send_tcp(socket_id, &new_msg);
    }
    for (uint32_t i = 0; i < gl_array_get_size(g_games); i++) {
        gl_message_t new_msg = {.type = GL_MESSAGE_TYPE_OGAMES, 0};
        gl_message_push_parameter(&new_msg, (gl_message_parameter_t) {.uint8_value = g_games[i].id});
        gl_message_push_parameter(&new_msg, (gl_message_parameter_t) {.uint8_value = gl_array_get_size(g_games[i].players)});
        gl_message_send_tcp(socket_id, &new_msg);
    }
}

void message_iquit(gl_message_t *msg, int32_t socket_id, void *user_data) {
    gl_message_t new_msg = {.type = GL_MESSAGE_TYPE_GOBYE, 0};
    gl_message_send_tcp(socket_id, &new_msg);
    bool found = false;
    for (uint32_t i = 0; i < gl_array_get_size(g_games); i++) {
        for (uint32_t j = 0; j < gl_array_get_size(g_games[i].players); j++) {
            if (g_games[i].players[j].socket_id == socket_id) {
                found = true;
                gl_array_remove(g_games[i].players, j);
                if (gl_array_get_size(g_games[i].players) == 0) {
                    gl_array_remove(g_games, i);
                }
                break;
            }
        }
        if (found) {
            break;
        }
    }
    gl_socket_close(&socket_id);
}

void message_multi(gl_message_t *msg, int32_t socket_id, void *user_data) {
    if (!g_legacy_protocol) {
        gl_message_t new_msg = {.type = GL_MESSAGE_TYPE_MULTI, 0};
        gl_message_push_parameter(&new_msg, (gl_message_parameter_t) {.string_value = gl_string_create_from_ip(g_multicast_ip) });
        gl_message_push_parameter(&new_msg, (gl_message_parameter_t) {.string_value = gl_string_create_from_number(g_multicast_port, 4) });
        gl_message_send_tcp(socket_id, &new_msg);
    }
}

void message_shutd(gl_message_t *msg, int32_t socket_id, void *user_data) {
    if (!g_legacy_protocol) {
        gl_message_t new_msg = {.type = GL_MESSAGE_TYPE_SHUTD, 0};
        gl_message_send_multicast(g_multicast_ip, g_multicast_port, &new_msg);
    }
}

void gl_message_add_functions() {
    gl_message_definitions()[GL_MESSAGE_TYPE_NEWPL]->function = message_newpl;
    gl_message_definitions()[GL_MESSAGE_TYPE_REGIS]->function = message_regis;
    gl_message_definitions()[GL_MESSAGE_TYPE_GAME_REQ]->function = message_game_req;
    gl_message_definitions()[GL_MESSAGE_TYPE_IQUIT]->function = message_iquit;
    gl_message_definitions()[GL_MESSAGE_TYPE_MULTI]->function = message_multi;
    gl_message_definitions()[GL_MESSAGE_TYPE_SHUTD]->function = message_shutd;
}