#include <printf.h>
#include <stdlib.h>
#include "common/types.h"
#include "common/array.h"
typedef enum gl_maze_element_t {
    GL_MAZE_ELEMENT_PILLAR,
    GL_MAZE_ELEMENT_WALL_CLOSED,
    GL_MAZE_ELEMENT_WALL_OPENED,
    GL_MAZE_ELEMENT_ROOM
} gl_maze_element_t;

typedef struct gl_maze_t {
    gl_maze_element_t *grid;
} gl_maze_t;

typedef struct gl_pos_t {
    uint32_t x;
    uint32_t y;
} gl_pos_t;

uint8_t **grid_create(uint8_t width, uint8_t height, uint8_t num_colors_per_line, uint8_t num_distinc_lines, uint8_t num_shift_per_line, uint8_t *num_colors) {
    if (width < 5 || height < 5 || num_colors_per_line == 0 || num_distinc_lines == 0) {
        return 0;
    }
    
    uint8_t **grid = gl_array_create(uint8_t*, height);
    
    for (uint32_t y = 0; y < width; y++) {
        grid[y] = gl_array_create(uint8_t, width);
        
        for (uint32_t x = 0; x < height; x++) {
            grid[y][x] = ((x + y * (num_shift_per_line)) % num_colors_per_line) + num_colors_per_line * (y % num_distinc_lines);
        }
    }
    
    if (num_colors) {
        *num_colors = num_colors_per_line * num_distinc_lines;
    }
    
    return grid;
}

uint8_t **create_maze(uint8_t **grid, gl_maze_element_t* elements_per_color, uint8_t num_colors) {
    gl_pos_t initial_pos;
    gl_pos_t *path = 0;
    gl_pos_t *walls = 0;
    uint32_t height = gl_array_get_size(grid);
    uint32_t width = gl_array_get_size(grid[0]);
    
    /*or (uint32_t y = 0; y < width; y++) {
        for (uint32_t x = 0; x < height; x++) {
            printf("%d", grid[y][x]);
        }
        printf("\n");
    }
    printf("\n");*/
    
    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            grid[y][x] = elements_per_color[grid[y][x]];
        }
    }
    
    do {
        initial_pos.x = rand() % height;
        initial_pos.y = rand() % width;
    } while (grid[initial_pos.y][initial_pos.x] != GL_MAZE_ELEMENT_ROOM);
    
    gl_array_push(path, initial_pos);
    gl_array_push(walls, ((gl_pos_t) { initial_pos.x, initial_pos.y - 1 }));
    gl_array_push(walls, ((gl_pos_t) { initial_pos.x + 1, initial_pos.y }));
    gl_array_push(walls, ((gl_pos_t) { initial_pos.x, initial_pos.y + 1 }));
    gl_array_push(walls, ((gl_pos_t) { initial_pos.x - 1, initial_pos.y }));
    
    while (!gl_array_is_empty(walls)) {
        uint32_t wall_idx = rand() % gl_array_get_size(walls);
        gl_pos_t wall_pos = walls[wall_idx];
    
        /*for (uint32_t y = 0; y < width; y++) {
            for (uint32_t x = 0; x < height; x++) {
                if (x == initial_pos.x && y == initial_pos.y) {
                    printf("\033[0;31m");
                    printf("@");
                    printf("\033[0m");
                } else if (x == wall_pos.x && y == wall_pos.y) {
                    printf("\033[0;32m");
                    printf("C");
                    printf("\033[0m");
                } else if (grid[y][x] == GL_MAZE_ELEMENT_PILLAR) {
                    printf("#");
                } else if (grid[y][x] == GL_MAZE_ELEMENT_WALL_CLOSED) {
                    printf("C");
                } else if (grid[y][x] == GL_MAZE_ELEMENT_WALL_OPENED) {
                    printf("O");
                } else if (grid[y][x] == GL_MAZE_ELEMENT_ROOM) {
                    printf(".");
                }
            }
            printf("\n");
        }
        printf("\n");*/
    
        gl_pos_t *rooms = 0;
        
        if (wall_pos.y - 1 >= 0 && wall_pos.y - 1 < height && wall_pos.x >= 0 && wall_pos.x < width && grid[wall_pos.y - 1][wall_pos.x] == GL_MAZE_ELEMENT_ROOM) {
            gl_array_push(rooms, ((gl_pos_t) { wall_pos.x, wall_pos.y - 1 }));
        }
        if (wall_pos.y - 1 >= 0 && wall_pos.y - 1 < height && wall_pos.x + 1 >= 0 && wall_pos.x + 1 < width && grid[wall_pos.y - 1][wall_pos.x + 1] == GL_MAZE_ELEMENT_ROOM) {
            gl_array_push(rooms, ((gl_pos_t) { wall_pos.x + 1, wall_pos.y - 1 }));
        }
        if (wall_pos.y >= 0 && wall_pos.y < height && wall_pos.x + 1 >= 0 && wall_pos.x + 1 < width && grid[wall_pos.y][wall_pos.x + 1] == GL_MAZE_ELEMENT_ROOM) {
            gl_array_push(rooms, ((gl_pos_t) { wall_pos.x + 1, wall_pos.y }));
        }
        if (wall_pos.y + 1 >= 0 && wall_pos.y + 1 < height && wall_pos.x + 1 >= 0 && wall_pos.x + 1 < width && grid[wall_pos.y + 1][wall_pos.x + 1] == GL_MAZE_ELEMENT_ROOM) {
            gl_array_push(rooms, ((gl_pos_t) { wall_pos.x + 1, wall_pos.y + 1 }));
        }
        if (wall_pos.y + 1 >= 0 && wall_pos.y + 1 < height && wall_pos.x >= 0 && wall_pos.x < width && grid[wall_pos.y + 1][wall_pos.x] == GL_MAZE_ELEMENT_ROOM) {
            gl_array_push(rooms, ((gl_pos_t) { wall_pos.x, wall_pos.y + 1 }));
        }
        if (wall_pos.y + 1 >= 0 && wall_pos.y + 1 < height && wall_pos.x - 1 >= 0 && wall_pos.x - 1 < width && grid[wall_pos.y + 1][wall_pos.x - 1] == GL_MAZE_ELEMENT_ROOM) {
            gl_array_push(rooms, ((gl_pos_t) { wall_pos.x - 1, wall_pos.y + 1 }));
        }
        if (wall_pos.y >= 0 && wall_pos.y < height && wall_pos.x - 1 >= 0 && wall_pos.x - 1 < width && grid[wall_pos.y][wall_pos.x - 1] == GL_MAZE_ELEMENT_ROOM) {
            gl_array_push(rooms, ((gl_pos_t) { wall_pos.x - 1, wall_pos.y }));
        }
        if (wall_pos.y - 1 >= 0 && wall_pos.y - 1 < height && wall_pos.x - 1 >= 0 && wall_pos.x - 1 < width && grid[wall_pos.y - 1][wall_pos.x - 1] == GL_MAZE_ELEMENT_ROOM) {
            gl_array_push(rooms, ((gl_pos_t) { wall_pos.x - 1, wall_pos.y - 1 }));
        }
        
        if (gl_array_get_size(rooms) == 2) {
            uint32_t found = 0;
            bool found_0 = false;
            bool found_1 = false;
    
            for (uint32_t i = 0; i < gl_array_get_size(path); i++) {
                if (path[i].x == rooms[0].x && path[i].y == rooms[0].y) {
                    found++;
                    found_0 = true;
                }
                if (path[i].x == rooms[1].x && path[i].y == rooms[1].y) {
                    found++;
                    found_1 = true;
                }
            }
            
            if (found == 1) {
                grid[wall_pos.y][wall_pos.x] = GL_MAZE_ELEMENT_WALL_OPENED;
                gl_pos_t room;
                if (found_0) {
                    room = rooms[1];
                } else {
                    room = rooms[0];
                }
    
                gl_array_push(path, room);
    
                if (room.y - 1 >= 0 && room.y - 1 < height && room.x >= 0 && room.x < width && grid[room.y - 1][room.x] == GL_MAZE_ELEMENT_WALL_CLOSED) {
                    gl_array_push(walls, ((gl_pos_t) { room.x, room.y - 1 }));
                }
                if (room.y - 1 >= 0 && room.y - 1 < height && room.x + 1 >= 0 && room.x + 1 < width && grid[room.y - 1][room.x + 1] == GL_MAZE_ELEMENT_WALL_CLOSED) {
                    gl_array_push(walls, ((gl_pos_t) { room.x + 1, room.y - 1 }));
                }
                if (room.y >= 0 && room.y < height && room.x + 1 >= 0 && room.x + 1 < width && grid[room.y][room.x + 1] == GL_MAZE_ELEMENT_WALL_CLOSED) {
                    gl_array_push(walls, ((gl_pos_t) { room.x + 1, room.y }));
                }
                if (room.y + 1 >= 0 && room.y + 1 < height && room.x + 1 >= 0 && room.x + 1 < width && grid[room.y + 1][room.x + 1] == GL_MAZE_ELEMENT_WALL_CLOSED) {
                    gl_array_push(walls, ((gl_pos_t) { room.x + 1, room.y + 1 }));
                }
                if (room.y + 1 >= 0 && room.y + 1 < height && room.x >= 0 && room.x < width && grid[room.y + 1][room.x] == GL_MAZE_ELEMENT_WALL_CLOSED) {
                    gl_array_push(walls, ((gl_pos_t) { room.x, room.y + 1 }));
                }
                if (room.y + 1 >= 0 && room.y + 1 < height && room.x - 1 >= 0 && room.x - 1 < width && grid[room.y + 1][room.x - 1] == GL_MAZE_ELEMENT_WALL_CLOSED) {
                    gl_array_push(walls, ((gl_pos_t) { room.x - 1, room.y + 1 }));
                }
                if (room.y >= 0 && room.y < height && room.x - 1 >= 0 && room.x - 1 < width && grid[room.y][room.x - 1] == GL_MAZE_ELEMENT_WALL_CLOSED) {
                    gl_array_push(walls, ((gl_pos_t) { room.x - 1, room.y }));
                }
                if (room.y - 1 >= 0 && room.y - 1 < height && room.x - 1 >= 0 && room.x - 1 < width && grid[room.y - 1][room.x - 1] == GL_MAZE_ELEMENT_WALL_CLOSED) {
                    gl_array_push(walls, ((gl_pos_t) { room.x - 1, room.y - 1 }));
                }
            }
        }
    
        gl_array_remove(walls, wall_idx);
        gl_array_free(rooms);
    }
    
    gl_array_free(walls);
    gl_array_free(path);
    
    return grid;
}

int main() {
    srand(time(0));
    
    uint8_t width = 5;
    uint8_t height = 5;
    width = 2 * width + 1;
    height = 2 * height + 1;
    gl_maze_element_t elements[4] = { GL_MAZE_ELEMENT_PILLAR, GL_MAZE_ELEMENT_WALL_CLOSED, GL_MAZE_ELEMENT_WALL_CLOSED, GL_MAZE_ELEMENT_ROOM };
    uint8_t num_colors;
    uint8_t **grid = grid_create(width, height, 2, 2, 0, &num_colors);
    grid = create_maze(grid, elements, num_colors);
    
    for (uint32_t y = 0; y < width; y++) {
        for (uint32_t x = 0; x < height; x++) {
            if (grid[y][x] == GL_MAZE_ELEMENT_PILLAR) {
                printf("#");
            } else if (grid[y][x] == GL_MAZE_ELEMENT_WALL_CLOSED) {
                printf("C");
            } else if (grid[y][x] == GL_MAZE_ELEMENT_WALL_OPENED) {
                printf("O");
            } else {
                printf(".");
            }
        }
        
        printf("\n");
    }
    
    return 0;
}