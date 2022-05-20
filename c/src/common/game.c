#include <common/game.h>
#include <stdlib.h>
#include <common/maze.h>
#include <common/array.h>
#include <common/memory.h>
#include <common/string.h>
#include <common/message.h>

gl_ghost_t *gl_game_generate_ghosts(gl_maze_t *maze, uint8_t num_ghosts) {
    gl_ghost_t *ghosts = 0;
    
    uint32_t i = 0;
    while (gl_array_get_size(ghosts) < num_ghosts) {
        gl_pos_t pos;
        
        uint32_t j = 0;
        do {
            pos.x = rand() % gl_array_get_size(maze->grid[0]);
            pos.y = rand() % gl_array_get_size(maze->grid);
            
            if (j++ >= 100) {
                break;
            }
        } while (maze->grid[pos.y][pos.x] != GL_MAZE_ELEMENT_ROOM || gl_game_is_ghost_at_pos(ghosts, pos));
        
        if (j < 100) {
            gl_array_push(ghosts, ((gl_ghost_t) { .pos = pos }));
        } else if (i++ > 5) {
            break;
        }
    }
    
    return ghosts;
}

gl_player_t *gl_game_generate_players(struct gl_maze_t *maze, gl_player_t *players, gl_ghost_t *ghosts) {
    if (!players) {
        return 0;
    }
    
    for (uint32_t i = 0; i < gl_array_get_size(players); i++) {
        gl_pos_t pos;
    
        uint32_t j = 0;
        do {
            pos.x = rand() % gl_array_get_size(maze->grid[0]);
            pos.y = rand() % gl_array_get_size(maze->grid);
        
            if (j++ >= 100) {
                break;
            }
        } while (maze->grid[pos.y][pos.x] != GL_MAZE_ELEMENT_ROOM || gl_game_is_ghost_at_pos(ghosts, pos) || gl_game_is_player_at_pos(players, pos));
    
        players[i].pos = pos;
    }
    
    return players;
}

bool gl_game_is_ghost_at_pos(gl_ghost_t *ghosts, gl_pos_t pos) {
    if (!ghosts) {
        return false;
    }
    
    bool found = false;
    
    for (uint32_t i = 0; i < gl_array_get_size(ghosts); i++) {
        if (ghosts[i].pos.x == pos.x && ghosts[i].pos.y == pos.y) {
            found = true;
            break;
        }
    }
    
    return found;
}

bool gl_game_is_player_at_pos(gl_player_t *players, gl_pos_t pos) {
    if (!players) {
        return false;
    }
    
    bool found = false;
    
    for (uint32_t i = 0; i < gl_array_get_size(players); i++) {
        if (players[i].pos.x == pos.x && players[i].pos.y == pos.y) {
            found = true;
            break;
        }
    }
    
    return found;
}

void gl_game_free(gl_game_t *game) {
    if (game) {
        gl_free(game->multicast_ip);
        gl_free(game->multicast_port);
        gl_maze_free(game->maze);
        gl_array_free(game->ghosts);
        gl_array_free(game->players);
    }
}

void gl_game_free_all(gl_game_t *games) {
    gl_game_free_all_with_exception(games, -1);
}

void gl_game_free_all_with_exception(gl_game_t *games, int32_t exception) {
    if (games) {
        for (uint32_t i = 0; i < gl_array_get_size(games); i++) {
            if (games[i].id != exception) {
                gl_game_free(&games[i]);
                gl_array_remove(games, i);
            }
        }
    }
}

gl_pos_t gl_game_get_maze_size(gl_game_t *game) {
    if (game->maze && game->maze->grid) {
        return (gl_pos_t) { .x = gl_array_get_size(game->maze->grid[0]), .y = gl_array_get_size(game->maze->grid) };
    }
    
    return (gl_pos_t) { .x = 7 + 1 * gl_array_get_size(game->players), .y = 7 + 1 * gl_array_get_size(game->players) };
}

uint32_t gl_game_move_player(gl_game_t *game, gl_player_t *player, uint32_t quantity, gl_movement_t movement) {
    uint32_t removed = 0;
    
    while (quantity > 0) {
        gl_pos_t pos;
        
        if (movement == GL_MOVEMENT_UP) {
            pos = (gl_pos_t) { player->pos.x, player->pos.y - 1 };
        } else if (movement == GL_MOVEMENT_RIGHT) {
            pos = (gl_pos_t) { player->pos.x + 1, player->pos.y };
        } else if (movement == GL_MOVEMENT_DOWN) {
            pos = (gl_pos_t) { player->pos.x, player->pos.y + 1 };
        } else {
            pos = (gl_pos_t) { player->pos.x - 1, player->pos.y };
        }
    
        gl_maze_element_t element = game->maze->grid[pos.y][pos.x];
    
        if (element == GL_MAZE_ELEMENT_ROOM || element == GL_MAZE_ELEMENT_WALL_OPENED) {
            player->pos = pos;
            
            for (uint32_t i = 0; i < gl_array_get_size(game->ghosts); i++) {
                if (game->ghosts[i].pos.x == pos.x && game->ghosts[i].pos.y == pos.y) {
                    player->score++;
                    
                    gl_array_remove(game->ghosts, i);
                    i--;
                    removed++;
    
                    gl_message_t response = { .type = GL_MESSAGE_TYPE_SCORE, 0 };
                    gl_message_push_parameter(&response, (gl_message_parameter_t) { .string_value = gl_string_create_from_cstring(player->id) });
                    gl_message_push_parameter(&response, (gl_message_parameter_t) { .string_value = gl_string_create_from_uint32(player->score, 4) });
                    gl_message_push_parameter(&response, (gl_message_parameter_t) { .string_value = gl_string_create_from_uint32(player->pos.x, 3) });
                    gl_message_push_parameter(&response, (gl_message_parameter_t) { .string_value = gl_string_create_from_uint32(player->pos.y, 3) });
                    gl_message_send_multicast(game->multicast_ip, game->multicast_port, &response);
    
                    uint32_t score_max = 0;
                    uint32_t k = 0;
                    for (uint32_t j = 0; j < gl_array_get_size(game->players); j++) {
                        if (game->players[j].score > score_max) {
                            score_max = game->players[j].score;
                            k = j;
                        }
                    }
                    
                    if (gl_array_get_size(game->ghosts) == 0) {
                        response = (gl_message_t) { .type = GL_MESSAGE_TYPE_ENDGA, 0 };
                        gl_message_push_parameter(&response, (gl_message_parameter_t) { .string_value = gl_string_create_from_cstring(game->players[k].id) });
                        gl_message_push_parameter(&response, (gl_message_parameter_t) { .string_value = gl_string_create_from_uint32(game->players[k].score, 4) });
                        gl_message_send_multicast(game->multicast_ip, game->multicast_port, &response);
                        game->over = true;
                    }
                }
            }
        } else {
            break;
        }
    
        quantity--;
    }
    
    return removed;
}

bool gl_game_move_ghost(gl_game_t *game, uint32_t ghost_id) {
    gl_pos_t pos;
    
    uint32_t j = 0;
    do {
        pos.x = rand() % gl_array_get_size(game->maze->grid[0]);
        pos.y = rand() % gl_array_get_size(game->maze->grid);
        
        if (j++ >= 100) {
            break;
        }
    } while (game->maze->grid[pos.y][pos.x] != GL_MAZE_ELEMENT_ROOM || gl_game_is_ghost_at_pos(game->ghosts, pos) || gl_game_is_player_at_pos(game->players, pos));
    
    if (j >= 100) {
        return false;
    }
    
    game->ghosts[ghost_id].pos.x = pos.x;
    game->ghosts[ghost_id].pos.y = pos.y;
    
    gl_message_t response = { .type = GL_MESSAGE_TYPE_GHOST, 0 };
    gl_message_push_parameter(&response, (gl_message_parameter_t) { .string_value = gl_string_create_from_uint32(game->ghosts[ghost_id].pos.x, 3) });
    gl_message_push_parameter(&response, (gl_message_parameter_t) { .string_value = gl_string_create_from_uint32(game->ghosts[ghost_id].pos.y, 3) });
    gl_message_send_multicast(game->multicast_ip, game->multicast_port, &response);
    
    return true;
}
