#ifndef GHOSTLAB_MAZE_H
#define GHOSTLAB_MAZE_H

#include <common/types.h>

// A maze is made of a randomly generated two-dimensional grid.
// The system is made to be extensible:
// - The maze default layout can be customized.
// - Generation functions can be passed as parameters.
// Steps of the generation:
// - A color grid is created by following a given pattern.
// - Each color is changed to become a maze elements (given a list which contains a maze element for each color).
//   It will typically consist of pillars, closed walls and room (but it can contain opened rooms).
// - Generations functions are then applied on the maze elements grid to open some walls randomly.
// - This results in a perfect maze.
// - The ghosts are now randomly placed on the maze.
// - The players are also randomly placed on the maze.

// Defines the type of a maze element.
typedef enum gl_maze_element_t {
    GL_MAZE_ELEMENT_PILLAR,
    GL_MAZE_ELEMENT_WALL_CLOSED,
    GL_MAZE_ELEMENT_WALL_OPENED,
    GL_MAZE_ELEMENT_ROOM,
    
    GL_MAZE_ELEMENT_COUNT
} gl_maze_element_t;

// Defines a maze.
typedef struct gl_maze_t {
    gl_maze_element_t **grid;
} gl_maze_t;

// Generates a maze.
gl_maze_t *gl_maze_generate(uint8_t base_width, uint8_t base_height);

// Frees a maze.
void gl_maze_free(gl_maze_t *maze);

// Default function to use to convert a color grid to a maze grid.
void gl_maze_default_color_to_grid(gl_maze_element_t **maze, uint8_t **grid, uint8_t width, uint8_t height, const gl_maze_element_t *element_per_color, uint8_t size_element_per_color);

// Default function to use to get a randomly chosen initial room position.
gl_pos_t gl_maze_default_random_initial_room_position(gl_maze_element_t **maze, uint8_t width, uint8_t height);

// Default function to use to get a randomly chosen wall in a list of walls.
uint32_t gl_default_choose_random_wall(gl_pos_t *walls, uint32_t size);

// Creates a color grid.
uint8_t **gl_maze_create_color_grid(uint8_t width, uint8_t height, uint8_t *scheme, uint8_t scheme_width, uint8_t scheme_height);

// Converts a color grid to a maze grid.
gl_maze_element_t **gl_maze_color_to_grid(uint8_t **grid, void (*gl_color_to_maze_comparator)(gl_maze_element_t **, uint8_t**, uint8_t, uint8_t, const gl_maze_element_t*, uint8_t), const gl_maze_element_t *element_per_color, uint8_t size_element_per_color);

// Generates a list of maze elements with a given seed.
gl_maze_element_t **gl_maze_generate_elements_with_seed(gl_maze_element_t **maze, uint64_t seed, gl_pos_t (* initial_room_comparator)(gl_maze_element_t **, uint8_t, uint8_t), uint32_t (* wall_comparator)(gl_pos_t *, uint32_t));

// Generates a list of maze elements.
gl_maze_element_t **gl_maze_generate_elements(gl_maze_element_t **maze, gl_pos_t (* initial_room_comparator)(gl_maze_element_t **, uint8_t, uint8_t), uint32_t ( *wall_comparator)(gl_pos_t *, uint32_t));

#endif /* GHOSTLAB_MAZE_H */
