#include <client/message.h>
#include <common/game.h>
#include <common/string.h>
#include <common/network.h>
#include <common/message.h>
#include <client/shared.h>

static void message_games(gl_message_t *msg, int32_t socket_id, void *user_data) {
    gl_client_free_games();
    
    for (uint32_t i = 0; i < msg->parameters_value[0].uint8_value; i++) {
        gl_message_wait_and_execute_no_lock(socket_id, GL_MESSAGE_PROTOCOL_TCP);
    }
}

static void message_ogames(gl_message_t *msg, int32_t socket_id, void *user_data) {
    gl_client_add_game(msg->parameters_value[0].uint8_value);
}

static void message_regok(gl_message_t *msg, int32_t socket_id, void *user_data) {
    g_game_id = msg->parameters_value[0].uint8_value;
    
    if (gl_client_get_game() == 0) {
        gl_client_add_game(g_game_id);
    }
    
    gl_client_add_player(gl_client_get_game(), g_player_id);
}

static void message_unrok(gl_message_t *msg, int32_t socket_id, void *user_data) {
    g_game_id = -1;
    
    if (g_should_quit) {
        gl_socket_close(&g_server_socket);
    }
}

static void message_gobye(gl_message_t *msg, int32_t socket_id, void *user_data) {
    g_game_id = -1;
    
    gl_socket_close(&g_server_socket);
    
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
}

static void message_shutd(gl_message_t *msg, int32_t socket_id, void *user_data) {
    g_is_server_down = true;
    g_should_quit = true;
}

void gl_client_message_add_functions() {
    gl_message_definitions()[GL_MESSAGE_TYPE_GAMES]->function = message_games;
    gl_message_definitions()[GL_MESSAGE_TYPE_OGAMES]->function = message_ogames;
    gl_message_definitions()[GL_MESSAGE_TYPE_REGOK]->function = message_regok;
    gl_message_definitions()[GL_MESSAGE_TYPE_UNROK]->function = message_unrok;
    gl_message_definitions()[GL_MESSAGE_TYPE_GOBYE]->function = message_gobye;
    gl_message_definitions()[GL_MESSAGE_TYPE_MULTI]->function = message_multi;
    gl_message_definitions()[GL_MESSAGE_TYPE_SHUTD]->function = message_shutd;
}
