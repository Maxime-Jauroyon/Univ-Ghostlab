#include "common/types.h"
#include "common/array.h"
#include "common/memory.h"
#include "common/maze.h"
#include "common/log.h"

// 0, 3: blue
// 1, 2: yellow
// 4: white
void gl_color_to_maze_test(gl_maze_element_t **maze, uint8_t **grid, uint8_t width, uint8_t height, const gl_maze_element_t *element_per_color, uint8_t size_element_per_color) {
    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            if (x < width - 1 && grid[y][x] == 1 && grid[y][x + 1] == 2 && maze[y][x] == GL_MAZE_ELEMENT_COUNT && maze[y][x + 1] == GL_MAZE_ELEMENT_COUNT) {
                maze[y][x] = GL_MAZE_ELEMENT_WALL_CLOSED;
                maze[y][x + 1] = GL_MAZE_ELEMENT_WALL_CLOSED;
            } else if (y < height - 1 && grid[y][x] == 0 && grid[y + 1][x] == 3 && maze[y][x] == GL_MAZE_ELEMENT_COUNT && maze[y + 1][x] == GL_MAZE_ELEMENT_COUNT) {
                maze[y][x] = GL_MAZE_ELEMENT_WALL_CLOSED;
                maze[y + 1][x] = GL_MAZE_ELEMENT_WALL_CLOSED;
            } else if (grid[y][x] == 4 && maze[y][x] == GL_MAZE_ELEMENT_COUNT) {
                maze[y][x] = GL_MAZE_ELEMENT_ROOM;
            } else if (maze[y][x] == GL_MAZE_ELEMENT_COUNT) {
                maze[y][x] = GL_MAZE_ELEMENT_PILLAR;
            }
        }
    }
}

int main() {
    gl_maze_t *maze = gl_maze_create(6, 6, 3);
    gl_maze_free(maze);
    
    gl_log_free();
    
    gl_memory_check_for_leaks();
    
    return 0;
}