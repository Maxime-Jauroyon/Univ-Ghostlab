#ifndef GHOSTLAB_GAME_H
#define GHOSTLAB_GAME_H

#include <common/types.h>

struct gl_maze_t;

typedef struct gl_player_t {
    char id[9];
    char port[4];
    uint8_t score;
    gl_pos_t pos;
    int32_t socket_id;
    bool ready;
} gl_player_t;

typedef struct gl_ghost_t {
    gl_pos_t pos;
} gl_ghost_t;

typedef struct gl_game_t {
    char name[128];
    uint8_t id;
    gl_player_t *players;
    gl_ghost_t *ghosts;
    struct gl_maze_t *maze;
    bool started;
} gl_game_t;

gl_player_t *gl_game_find_player_with_socket(gl_game_t *games, int32_t socket_id);

gl_game_t *gl_game_find_game_with_socket(gl_game_t *games, int32_t socket_id);

gl_ghost_t *gl_game_generate_ghosts(struct gl_maze_t *maze, uint8_t num_ghosts);

bool gl_game_is_ghost_at_pos(gl_ghost_t *ghosts, gl_pos_t pos);

#endif /* GHOSTLAB_GAME_H */
