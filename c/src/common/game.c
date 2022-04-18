#include <common/game.h>
#include <stdlib.h>
#include "array.h"
#include "maze.h"

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

bool gl_game_is_ghost_at_pos(gl_ghost_t *ghosts, gl_pos_t pos) {
    bool found = false;
    
    for (uint32_t i = 0; i < gl_array_get_size(ghosts); i++) {
        if (ghosts[i].pos.x == pos.x && ghosts[i].pos.y == pos.y) {
            found = true;
            break;
        }
    }
    
    return found;
}