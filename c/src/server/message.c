#include <server/message.h>
#include <string.h>
#include <common/game.h>
#include <common/maze.h>
#include <common/array.h>
#include <common/message.h>
#include <common/network.h>
#include <server/shared.h>
#include "common/string.h"

void message_newpl(gl_message_t *msg, int32_t socket_id, void *user_data) {
    if (g_game_id >= 255) {
        gl_message_t response = { .type = GL_MESSAGE_TYPE_REGNO, 0 };
        gl_message_send_tcp(socket_id, &response);
        return;
    }
    
    char *player_id = (char *)msg->parameters_value[0].string_value;
    char *player_port = (char *)msg->parameters_value[1].string_value;
    
    gl_server_add_game(g_game_id);
    gl_server_add_player(&gl_array_get_last(g_games), player_id, player_port, socket_id);
    
    gl_message_t response = { .type = GL_MESSAGE_TYPE_REGOK, 0 };
    gl_message_push_parameter(&response, (gl_message_parameter_t) { .uint8_value = g_game_id });
    gl_message_send_tcp(socket_id, &response);
    
    g_game_id++;
}

void message_regis(gl_message_t *msg, int32_t socket_id, void *user_data) {
    uint8_t game_id = msg->parameters_value[2].uint8_value;
    
    if (!gl_server_get_game(game_id)) {
        gl_message_t response = { .type = GL_MESSAGE_TYPE_REGNO, 0 };
        gl_message_send_tcp(socket_id, &response);
        return;
    }
    
    gl_game_t *game = gl_server_get_game(game_id);
    char *player_id = (char *)msg->parameters_value[0].string_value;
    char *player_port = (char *)msg->parameters_value[1].string_value;
    
    for (uint32_t i = 0; i < gl_array_get_size(game->players); i++) {
        if (strcmp(g_games[game_id].players[i].id, player_id) == 0) {
            gl_message_t response = { .type = GL_MESSAGE_TYPE_REGNO, 0 };
            gl_message_send_tcp(socket_id, &response);
            return;
        }
    }
    
    gl_server_add_player(gl_server_get_game(game_id), player_id, player_port, socket_id);
    
    gl_message_t response = { .type = GL_MESSAGE_TYPE_REGOK, 0 };
    gl_message_push_parameter(&response, (gl_message_parameter_t) { .uint8_value = game_id });
    gl_message_send_tcp(socket_id, &response);
}

void message_size_req(gl_message_t *msg, int32_t socket_id, void *user_data) {
    uint32_t game_id = msg->parameters_value[0].uint8_value;
    gl_game_t *game = gl_server_get_game(game_id);
    
    if (!game) {
        gl_message_t response = { .type = GL_MESSAGE_TYPE_DUNNO, 0 };
        gl_message_send_tcp(socket_id, &response);
        return;
    }
    
    gl_pos_t size = gl_game_get_maze_size(game);
    gl_message_t response = { .type = GL_MESSAGE_TYPE_SIZE_RES, 0 };
    gl_message_push_parameter(&response, (gl_message_parameter_t) { .uint8_value = game_id });
    gl_message_push_parameter(&response, (gl_message_parameter_t) { .uint16_value = 2 * size.x + 1 }); // TODO: Move to maze.c
    gl_message_push_parameter(&response, (gl_message_parameter_t) { .uint16_value = 2 * size.y + 1 }); // TODO: Move to maze.c
    gl_message_send_tcp(socket_id, &response);
}

void message_list_req(gl_message_t *msg, int32_t socket_id, void *user_data) {
    uint32_t game_id = msg->parameters_value[0].uint8_value;
    gl_game_t *game = gl_server_get_game(game_id);
    
    if (!game) {
        gl_message_t response = { .type = GL_MESSAGE_TYPE_DUNNO, 0 };
        gl_message_send_tcp(socket_id, &response);
        return;
    }
    
    gl_message_t response = {.type = GL_MESSAGE_TYPE_LIST_RES, 0};
    gl_message_push_parameter(&response, (gl_message_parameter_t) {.uint8_value = game_id});
    gl_message_push_parameter(&response, (gl_message_parameter_t) {.uint8_value = gl_array_get_size(game->players)});
    gl_message_send_tcp(socket_id, &response);
    
    for (uint32_t i = 0; i < gl_array_get_size(game->players); i++) {
        response = (gl_message_t) { .type = GL_MESSAGE_TYPE_PLAYR, 0 };
        gl_message_push_parameter(&response, (gl_message_parameter_t) { .string_value = gl_string_create_from_cstring(game->players[i].id) });
        gl_message_send_tcp(socket_id, &response);
    }
}

void message_game_req(gl_message_t *msg, int32_t socket_id, void *user_data) {
    gl_server_send_game_list(socket_id);
}

void message_start(gl_message_t *msg, int32_t socket_id, void *user_data) {
    gl_player_t *player = gl_server_get_player_with_socket(socket_id);
    
    if (!player) {
        gl_message_t response = { .type = GL_MESSAGE_TYPE_DUNNO, 0 };
        gl_message_send_tcp(socket_id, &response);
        return;
    }
    
    player->ready = true;
    
    gl_game_t *game = gl_server_get_game_with_socket(socket_id);
    
    if (!game) {
        gl_message_t response = { .type = GL_MESSAGE_TYPE_DUNNO, 0 };
        gl_message_send_tcp(socket_id, &response);
        return;
    }
    
    gl_server_start_game_if_ready(game);
}

void message_unreg(gl_message_t *msg, int32_t socket_id, void *user_data) {
    uint32_t game_id;
    bool removed = gl_server_remove_player_with_socket(socket_id, &game_id);
    
    if (!removed) {
        gl_message_t response = { .type = GL_MESSAGE_TYPE_DUNNO, 0 };
        gl_message_send_tcp(socket_id, &response);
        return;
    }
    
    gl_message_t response = { .type = GL_MESSAGE_TYPE_UNROK, 0 };
    gl_message_push_parameter(&response, (gl_message_parameter_t) { .uint8_value = game_id });
    gl_message_send_tcp(socket_id, &response);
    
    gl_server_start_game_if_ready(gl_server_get_game(game_id));
}

void message_iquit(gl_message_t *msg, int32_t socket_id, void *user_data) {
    bool removed = gl_server_remove_player_with_socket(socket_id, 0);
    
    if (!removed) {
        gl_message_t response = { .type = GL_MESSAGE_TYPE_DUNNO, 0 };
        gl_message_send_tcp(socket_id, &response);
        return;
    }
    
    gl_message_t response = { .type = GL_MESSAGE_TYPE_GOBYE, 0 };
    gl_message_send_tcp(socket_id, &response);
    gl_socket_close(&socket_id);
}

void message_upmov(gl_message_t *msg, int32_t socket_id, void *user_data) {

}

void message_domov(gl_message_t *msg, int32_t socket_id, void *user_data) {

}

void message_lemov(gl_message_t *msg, int32_t socket_id, void *user_data) {

}

void message_rimov(gl_message_t *msg, int32_t socket_id, void *user_data) {

}

void message_glis_req(gl_message_t *msg, int32_t socket_id, void *user_data) {
    gl_game_t *game = gl_server_get_game_with_socket(socket_id);
    
    if (game->over) {
        gl_message_t response = {.type = GL_MESSAGE_TYPE_GOBYE, 0};
        gl_message_send_tcp(socket_id, &response);
    }
    
    gl_message_t response = {.type = GL_MESSAGE_TYPE_GLIS_RES, 0};
    gl_message_push_parameter(&response, (gl_message_parameter_t) {.uint8_value = gl_array_get_size(game->players)});
    gl_message_send_tcp(socket_id, &response);
    
    for (uint32_t i = 0; i < gl_array_get_size(game->players); i++) {
        response = (gl_message_t) { .type = GL_MESSAGE_TYPE_GPLYR, 0 };
        gl_message_push_parameter(&response, (gl_message_parameter_t) { .string_value = gl_string_create_from_cstring(game->players[i].id) });
        gl_message_push_parameter(&response, (gl_message_parameter_t) { .string_value = gl_string_create_from_uint32(game->players[i].pos.x, 3) });
        gl_message_push_parameter(&response, (gl_message_parameter_t) { .string_value = gl_string_create_from_uint32(game->players[i].pos.y, 3) });
        gl_message_push_parameter(&response, (gl_message_parameter_t) { .string_value = gl_string_create_from_uint32(game->players[i].score, 4) });
        gl_message_send_tcp(socket_id, &response);
    }
}

void gl_server_message_add_functions() {
    gl_message_definitions()[GL_MESSAGE_TYPE_NEWPL]->function = message_newpl;
    gl_message_definitions()[GL_MESSAGE_TYPE_REGIS]->function = message_regis;
    gl_message_definitions()[GL_MESSAGE_TYPE_SIZE_REQ]->function = message_size_req;
    gl_message_definitions()[GL_MESSAGE_TYPE_LIST_REQ]->function = message_list_req;
    gl_message_definitions()[GL_MESSAGE_TYPE_GAME_REQ]->function = message_game_req;
    gl_message_definitions()[GL_MESSAGE_TYPE_START]->function = message_start;
    gl_message_definitions()[GL_MESSAGE_TYPE_UNREG]->function = message_unreg;
    gl_message_definitions()[GL_MESSAGE_TYPE_IQUIT]->function = message_iquit;
    gl_message_definitions()[GL_MESSAGE_TYPE_UPMOV]->function = message_upmov;
    gl_message_definitions()[GL_MESSAGE_TYPE_DOMOV]->function = message_domov;
    gl_message_definitions()[GL_MESSAGE_TYPE_LEMOV]->function = message_lemov;
    gl_message_definitions()[GL_MESSAGE_TYPE_RIMOV]->function = message_rimov;
    gl_message_definitions()[GL_MESSAGE_TYPE_GLIS_REQ]->function = message_glis_req;
}
