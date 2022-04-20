#ifndef GHOSTLAB_GAME_H
#define GHOSTLAB_GAME_H

#include <common/types.h>

struct gl_maze_t;

typedef struct gl_player_t {
    char id[9];
    uint8_t score;
    gl_pos_t pos;
    bool ready;
#if GHOSTLAB_SERVER
    char port[4];
    int32_t socket_id;
#endif
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

gl_ghost_t *gl_game_generate_ghosts(struct gl_maze_t *maze, uint8_t num_ghosts);

gl_player_t *gl_game_generate_players_pos(struct gl_maze_t *maze, gl_player_t *players, gl_ghost_t *ghosts);

bool gl_game_is_ghost_at_pos(gl_ghost_t *ghosts, gl_pos_t pos);

bool gl_game_is_player_at_pos(gl_player_t *players, gl_pos_t pos);

void gl_game_free(gl_game_t *game);

void gl_game_free_all(gl_game_t *games);

void gl_game_free_all_with_exception(gl_game_t *games, int32_t exception);

#endif /* GHOSTLAB_GAME_H */
