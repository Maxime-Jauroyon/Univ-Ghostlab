#include <client/message.h>
#include <string.h>
#include <pthread.h>
#include <common/log.h>
#include <common/game.h>
#include <common/array.h>
#include <common/memory.h>
#include <common/string.h>
#include <common/message.h>
#include <common/network.h>
#include <client/ui.h>
#include <client/shared.h>
#include <client/thread_multicast_game_listener.h>
#include <client/thread_multicast_general_listener.h>

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
    memcpy(g_player_id, g_temp_player_id, 8);
    bzero(g_temp_player_id, 8);
    
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
    gl_client_ui_error(2);
}

static void message_unrok(gl_message_t *msg, int32_t socket_id, void *user_data) {
    uint32_t game_id = g_game_id;
    
    g_game_id = -1;
    
    for (uint32_t i = 0; i < gl_array_get_size(g_games); i++) {
        if (g_games[i].id == game_id) {
            gl_game_free(&g_games[i]);
            gl_array_remove(g_games, i);
            break;
        }
    }
    
    if (g_should_quit) {
        gl_socket_close(&g_tcp_listener_socket);
    }
}

static void message_dunno(gl_message_t *msg, int32_t socket_id, void *user_data) {
    gl_log_push_error("an error occured!\n");
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
        if (strcmp(game->players[i].id, g_player_id) != 0) {
            gl_array_remove(game->players, i);
            i--;
        }
    }
    
    g_message_list_res_game_id = game_id;
}

static void message_playr(gl_message_t *msg, int32_t socket_id, void *user_data) {
    gl_game_t *game = gl_client_get_game_with_id(g_message_list_res_game_id);
    
    char player_id[9] = { 0 };
    memcpy(player_id, msg->parameters_value[0].string_value, 8);
    
    if (strcmp(player_id, g_player_id) != 0) {
        gl_player_t player = { 0 };
        memcpy(player.id, msg->parameters_value[0].string_value, 8);
        gl_array_push(game->players, player);
    }
}

static void message_welco(gl_message_t *msg, int32_t socket_id, void *user_data) {
    gl_game_t *game = gl_client_get_game();
    
    game->maze_size.x = msg->parameters_value[1].uint16_value;
    game->maze_size.y = msg->parameters_value[2].uint16_value;
    
    uint32_t num_ghosts = msg->parameters_value[3].uint8_value;
    
    for (uint32_t i = 0; i < num_ghosts; i++) {
        gl_ghost_t ghost = { 0 };
        gl_array_push(game->ghosts, ghost);
    }
    
    game->multicast_ip = gl_cstring_create_from_ip(msg->parameters_value[4].string_value);
    game->multicast_port = gl_cstring_create_from_string(msg->parameters_value[5].string_value);
    
    game->started = true;
    
    gl_client_thread_multicast_game_listener_start();
}

static void message_posit(gl_message_t *msg, int32_t socket_id, void *user_data) {
    gl_player_t *player = gl_client_get_player();
    
    player->pos.x = gl_string_strtol(msg->parameters_value[1].string_value);
    player->pos.y = gl_string_strtol(msg->parameters_value[2].string_value);
    
    gl_client_get_game()->reload_players_data_1 = true;
}

static void message_move_res(gl_message_t *msg, int32_t socket_id, void *user_data) {
    gl_player_t *player = gl_client_get_player();
    player->pos.x = gl_string_strtol(msg->parameters_value[0].string_value);
    player->pos.y = gl_string_strtol(msg->parameters_value[1].string_value);
}

static void message_movef(gl_message_t *msg, int32_t socket_id, void *user_data) {
    gl_player_t *player = gl_client_get_player();
    player->pos.x = gl_string_strtol(msg->parameters_value[0].string_value);
    player->pos.y = gl_string_strtol(msg->parameters_value[1].string_value);
    player->score = gl_string_strtol(msg->parameters_value[2].string_value);
}

static void message_gobye(gl_message_t *msg, int32_t socket_id, void *user_data) {
    uint32_t game_id = g_game_id;
    
    g_game_id = -1;
    
    gl_socket_close(&g_tcp_listener_socket);
    
    gl_client_thread_multicast_game_listener_close();
    
    for (uint32_t i = 0; i < gl_array_get_size(g_games); i++) {
        if (g_games[i].id == game_id) {
            gl_game_free(&g_games[i]);
            gl_array_remove(g_games, i);
            break;
        }
    }
    
    if (!g_should_quit) {
        gl_client_connect();
    }
}

static void message_glis_res(gl_message_t *msg, int32_t socket_id, void *user_data) {
    gl_game_t *game = gl_client_get_game();
    
    for (uint32_t i = 0; i < gl_array_get_size(game->players); i++) {
        if (strcmp(game->players[i].id, g_player_id) != 0) {
            gl_array_remove(game->players, i);
            i--;
        }
    }
    
    g_message_list_res_game_id = game->id;
    
    if (g_players_message_list) {
        gl_array_free(g_players_message_list);
    }
    gl_array_push(g_players_message_list, g_everyone);
}

static void message_gplyr(gl_message_t *msg, int32_t socket_id, void *user_data) {
    gl_game_t *game = gl_client_get_game_with_id(g_message_list_res_game_id);
    
    char player_id[9] = { 0 };
    memcpy(player_id, msg->parameters_value[0].string_value, 8);
    
    if (strcmp(player_id, g_player_id) != 0) {
        gl_player_t player = { 0 };
        memcpy(player.id, msg->parameters_value[0].string_value, 8);
        player.pos.x = gl_string_strtol(msg->parameters_value[1].string_value);
        player.pos.y = gl_string_strtol(msg->parameters_value[2].string_value);
        player.score = gl_string_strtol(msg->parameters_value[3].string_value);
        gl_array_push(game->players, player);
        gl_array_push(g_players_message_list, gl_array_get_last(game->players).id);
    } else {
        gl_player_t *player = gl_client_get_player();
        player->pos.x = gl_string_strtol(msg->parameters_value[1].string_value);
        player->pos.y = gl_string_strtol(msg->parameters_value[2].string_value);
        player->score = gl_string_strtol(msg->parameters_value[3].string_value);
    }
}

static void message_send_res(gl_message_t *msg, int32_t socket_id, void *user_data) {
    gl_log_push_info("[you] whispered: %s\n", g_last_sent_message);
}

static void message_ghost(gl_message_t *msg, int32_t socket_id, void *user_data) {
    gl_log_push_info("[server] says: a ghost just moved!\n");
}

static void message_score(gl_message_t *msg, int32_t socket_id, void *user_data) {
    gl_game_t *game = gl_client_get_game();
    
    for (uint32_t i = 0; i < gl_array_get_size(game->players); i++) {
        char player_id[9] = { 0 };
        memcpy(player_id, msg->parameters_value[0].string_value, 8);
        
        if (strcmp(game->players[i].id, player_id) == 0) {
            game->players[i].score = gl_string_strtol(msg->parameters_value[1].string_value);
            game->players[i].pos.x = gl_string_strtol(msg->parameters_value[2].string_value);
            game->players[i].pos.y = gl_string_strtol(msg->parameters_value[3].string_value);
        }
    }
}

static void message_messa(gl_message_t *msg, int32_t socket_id, void *user_data) {
    char player_id[9] = { 0 };
    memcpy(player_id, msg->parameters_value[0].string_value, 8);
    
    char *buf = gl_cstring_create_from_ip(msg->parameters_value[1].string_value);
    
    if (strcmp(g_player_id, player_id) != 0) {
        gl_log_push_info("[%s] says: %s\n", player_id, buf);
    } else {
        gl_log_push_info("[you] said: %s\n", buf);
    }
    
    gl_free(buf);
}

static void message_endga(gl_message_t *msg, int32_t socket_id, void *user_data) {
    char player_id[9] = { 0 };
    memcpy(player_id, msg->parameters_value[0].string_value, 8);
    
    if (strcmp(g_player_id, player_id) == 0) {
        gl_client_get_player()->won = true;
    }
    
    gl_client_game_over_popup_show();
}

static void message_messp(gl_message_t *msg, int32_t socket_id, void *user_data) {
    char player_id[9] = { 0 };
    memcpy(player_id, msg->parameters_value[0].string_value, 8);
    
    char *buf = gl_cstring_create_from_ip(msg->parameters_value[1].string_value);
    
    if (strcmp(g_player_id, player_id) != 0) {
        gl_log_push_info("[%s] whispers: %s\n", player_id, buf);
    }
    
    gl_free(buf);
}

static void message_multi(gl_message_t *msg, int32_t socket_id, void *user_data) {
    if (!g_multicast_ip && !g_multicast_port) {
        g_multicast_ip = gl_cstring_create_from_ip(msg->parameters_value[0].string_value);
        g_multicast_port = gl_cstring_create_from_string(msg->parameters_value[1].string_value);
    }
    
    if (!g_multicast_general_listener_thread) {
        g_multicast_general_listener_thread = gl_malloc(sizeof(pthread_t));
        pthread_create(g_multicast_general_listener_thread, 0, gl_client_thread_multicast_general_listener_main, 0);
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
    gl_message_definitions()[GL_MESSAGE_TYPE_REGNO]->function = message_regno;
    gl_message_definitions()[GL_MESSAGE_TYPE_UNROK]->function = message_unrok;
    gl_message_definitions()[GL_MESSAGE_TYPE_DUNNO]->function = message_dunno;
    gl_message_definitions()[GL_MESSAGE_TYPE_SIZE_RES]->function = message_size_res;
    gl_message_definitions()[GL_MESSAGE_TYPE_LIST_RES]->function = message_list_res;
    gl_message_definitions()[GL_MESSAGE_TYPE_PLAYR]->function = message_playr;
    gl_message_definitions()[GL_MESSAGE_TYPE_WELCO]->function = message_welco;
    gl_message_definitions()[GL_MESSAGE_TYPE_POSIT]->function = message_posit;
    gl_message_definitions()[GL_MESSAGE_TYPE_MOVE_RES]->function = message_move_res;
    gl_message_definitions()[GL_MESSAGE_TYPE_MOVEF]->function = message_movef;
    gl_message_definitions()[GL_MESSAGE_TYPE_GOBYE]->function = message_gobye;
    gl_message_definitions()[GL_MESSAGE_TYPE_GLIS_RES]->function = message_glis_res;
    gl_message_definitions()[GL_MESSAGE_TYPE_GPLYR]->function = message_gplyr;
    gl_message_definitions()[GL_MESSAGE_TYPE_SEND_RES]->function = message_send_res;
    
    gl_message_definitions()[GL_MESSAGE_TYPE_GHOST]->function = message_ghost;
    gl_message_definitions()[GL_MESSAGE_TYPE_GHOST]->hide_when_received = true;
    gl_message_definitions()[GL_MESSAGE_TYPE_SCORE]->function = message_score;
    gl_message_definitions()[GL_MESSAGE_TYPE_MESSA]->function = message_messa;
    gl_message_definitions()[GL_MESSAGE_TYPE_ENDGA]->function = message_endga;
    gl_message_definitions()[GL_MESSAGE_TYPE_MESSP]->function = message_messp;
    
    gl_message_definitions()[GL_MESSAGE_TYPE_MULTI]->function = message_multi;
    gl_message_definitions()[GL_MESSAGE_TYPE_SHUTD]->function = message_shutd;
}
