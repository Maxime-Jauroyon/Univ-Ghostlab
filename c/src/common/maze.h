#ifndef GHOSTLAB_MAZE_H
#define GHOSTLAB_MAZE_H

#include <common/types.h>

typedef enum gl_maze_element_t {
    GL_MAZE_ELEMENT_PILLAR,
    GL_MAZE_ELEMENT_WALL_CLOSED,
    GL_MAZE_ELEMENT_WALL_OPENED,
    GL_MAZE_ELEMENT_ROOM,
    
    GL_MAZE_ELEMENT_COUNT
} gl_maze_element_t;

typedef struct gl_maze_t {
    gl_maze_element_t **grid;
} gl_maze_t;

gl_maze_t *gl_maze_create(uint8_t base_width, uint8_t base_height, uint8_t num_ghosts);

void gl_maze_free(gl_maze_t *maze);

void gl_color_to_maze_element_from_color(gl_maze_element_t **maze, uint8_t **grid, uint8_t width, uint8_t height, const gl_maze_element_t *element_per_color, uint8_t size_element_per_color);

gl_pos_t gl_initial_room_random(gl_maze_element_t **maze, uint8_t width, uint8_t height);

uint32_t gl_wall_random(gl_pos_t *walls, uint32_t size);

uint8_t **gl_maze_create_color_grid(uint8_t width, uint8_t height, uint8_t *scheme, uint8_t scheme_width, uint8_t scheme_height);

gl_maze_element_t **gl_maze_color_grid_to_maze_grid(uint8_t **grid, void (*gl_color_to_maze_comparator)(gl_maze_element_t **, uint8_t**, uint8_t, uint8_t, const gl_maze_element_t*, uint8_t), const gl_maze_element_t *element_per_color, uint8_t size_element_per_color);

gl_maze_element_t **gl_maze_generate_from_grid_with_seed(gl_maze_element_t **maze, uint64_t seed, gl_pos_t (* initial_room_comparator)(gl_maze_element_t **, uint8_t, uint8_t), uint32_t (* wall_comparator)(gl_pos_t *, uint32_t));

gl_maze_element_t **gl_maze_generate_from_grid(gl_maze_element_t **maze, gl_pos_t (* initial_room_comparator)(gl_maze_element_t **, uint8_t, uint8_t), uint32_t ( *wall_comparator)(gl_pos_t *, uint32_t));

#endif /* GHOSTLAB_MAZE_H */
