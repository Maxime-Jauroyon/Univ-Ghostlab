#ifndef GHOSTLAB_GAME_H
#define GHOSTLAB_GAME_H

#include <common/types.h>

struct gl_maze_t;

// Defines a player (entity of a game).
typedef struct gl_player_t {
    char id[9];
    uint8_t score;
    gl_pos_t pos;
    bool ready;
#if GHOSTLAB_SERVER
    char udp_port[5];
    int32_t socket_id;
#else
    bool won;
#endif
} gl_player_t;

// Defines a ghost (entity of a game).
typedef struct gl_ghost_t {
    gl_pos_t pos;
} gl_ghost_t;

// Defines a game.
typedef struct gl_game_t {
    char name[128];
    uint8_t id;
    gl_player_t *players;
    gl_ghost_t *ghosts;
    struct gl_maze_t *maze;
    bool started;
    char *multicast_ip;
    char *multicast_port;
    bool over;
#if GHOSTLAB_CLIENT
    gl_pos_t maze_size;
    bool reload_maze_data;
    bool reload_players_data_1;
    bool reload_players_data_2;
#endif
} gl_game_t;

// Defines the list of movements a player can do.
typedef enum gl_movement_t {
    GL_MOVEMENT_UP,
    GL_MOVEMENT_RIGHT,
    GL_MOVEMENT_DOWN,
    GL_MOVEMENT_LEFT
}gl_movement_t;

// Generates a list of ghosts for a given maze.
gl_ghost_t *gl_game_generate_ghosts(struct gl_maze_t *maze, uint8_t num_ghosts);

// Generates a list of players for a given maze.
gl_player_t *gl_game_generate_players(struct gl_maze_t *maze, gl_player_t *players, gl_ghost_t *ghosts);

// Checks if a ghost is at a given position.
bool gl_game_is_ghost_at_pos(gl_ghost_t *ghosts, gl_pos_t pos);

// Checks if a player is at a given position.
bool gl_game_is_player_at_pos(gl_player_t *players, gl_pos_t pos);

// Frees a game.
void gl_game_free(gl_game_t *game);

// Frees all games.
void gl_game_free_all(gl_game_t *games);

// Frees all game with a specific exception (generally the game in progress).
void gl_game_free_all_with_exception(gl_game_t *games, int32_t exception);

// Returns the size of a maze.
gl_pos_t gl_game_get_maze_size(gl_game_t *game);

// Moves a players as much as possible for a given length and movement in a given game.
uint32_t gl_game_move_player(gl_game_t *game, gl_player_t *player, uint32_t quantity, gl_movement_t movement);

// Moves a ghost in a given game.
bool gl_game_move_ghost(gl_game_t *game, uint32_t ghost_id);

#endif /* GHOSTLAB_GAME_H */
