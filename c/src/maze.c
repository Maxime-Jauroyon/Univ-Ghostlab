#include <printf.h>
#include <stdlib.h>
#include "common/types.h"
#include "common/array.h"
#include "common/memory.h"

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

uint8_t **gl_grid_create(uint8_t width, uint8_t height, uint8_t num_colors_per_line, uint8_t num_distinc_lines, uint8_t num_shift_per_line, const gl_maze_element_t* elements_per_color) {
    printf("grid width = 2 * width + 1 = 2 * %d + 1 = %d\n", width, 2 * width + 1);
    printf("grid height = 2 * height + 1 = 2 * %d + 1 = %d\n", height, 2 * height + 1);
    printf("num colors per line: %d\n", num_colors_per_line);
    printf("num distinct lines: %d\n", num_distinc_lines);
    printf("num shifting after end of line: %d\n", num_shift_per_line);
    printf("color - maze element:\n");
    for (uint32_t i = 0; i < num_colors_per_line * num_distinc_lines; i++) {
        printf("> %d - ", i);
        
        if (elements_per_color[i] == GL_MAZE_ELEMENT_PILLAR) {
            printf("GL_MAZE_ELEMENT_PILLAR (#)");
        } else if (elements_per_color[i] == GL_MAZE_ELEMENT_WALL_CLOSED) {
            printf("GL_MAZE_ELEMENT_WALL_CLOSED (C)");
        } else if (elements_per_color[i] == GL_MAZE_ELEMENT_WALL_OPENED) {
            printf("GL_MAZE_ELEMENT_WALL_OPENED (O)");
        } else if (elements_per_color[i] == GL_MAZE_ELEMENT_ROOM) {
            printf("GL_MAZE_ELEMENT_ROOM (.)");
        }
        
        printf("\n");
    }
    printf("\n");
    width = 2 * width + 1;
    height = 2 * height + 1;
    
    if (width < 2 || height < 2 || num_colors_per_line == 0 || num_distinc_lines == 0) {
        return 0;
    }
    
    uint8_t **grid = gl_array_create(uint8_t*, height);
    
    for (uint32_t y = 0; y < width; y++) {
        grid[y] = gl_array_create(uint8_t, width);
        
        for (uint32_t x = 0; x < height; x++) {
            grid[y][x] = ((x + y * (num_shift_per_line)) % num_colors_per_line) + num_colors_per_line * (y % num_distinc_lines);
        }
    }
    
    printf("base scheme with color:\n");
    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            printf("%d", grid[y][x]);
        }
        
        printf("\n");
    }
    printf("\n");
    
    printf("convert colors to maze elements:\n");
    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            grid[y][x] = elements_per_color[grid[y][x]];
        }
    }
    
    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
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
    printf("\n");
    
    return grid;
}

void gl_maze_generate_from_grid_with_seed(uint8_t **grid, uint64_t seed) {
#ifdef __APPLE__
    printf("seed: %llu\n\n", seed);
#else
    printf("seed: %lu\n\n", seed);
#endif
    srand(seed);
    
    if (!grid) {
        return;
    }
    
    uint32_t height = gl_array_get_size(grid);
    uint32_t width = gl_array_get_size(grid[0]);
    
    if (width < 5 || height < 5) {
        return;
    }
    
    gl_pos_t initial_pos;
    do {
        initial_pos.x = rand() % height;
        initial_pos.y = rand() % width;
    } while (grid[initial_pos.y][initial_pos.x] != GL_MAZE_ELEMENT_ROOM);
    
    gl_pos_t *path = 0;
    gl_array_push(path, initial_pos);
    
    gl_pos_t *walls = 0;
    gl_array_push(walls, ((gl_pos_t) { initial_pos.x, initial_pos.y - 1 }));
    gl_array_push(walls, ((gl_pos_t) { initial_pos.x + 1, initial_pos.y }));
    gl_array_push(walls, ((gl_pos_t) { initial_pos.x, initial_pos.y + 1 }));
    gl_array_push(walls, ((gl_pos_t) { initial_pos.x - 1, initial_pos.y }));
    
    while (!gl_array_is_empty(walls)) {
        uint32_t wall_idx = rand() % gl_array_get_size(walls);
        gl_pos_t wall_pos = walls[wall_idx];
    
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
    
    printf("maze after generating with prim algorithm:\n");
    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
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
}

void gl_maze_generate_from_grid(uint8_t **grid) {
    gl_maze_generate_from_grid_with_seed(grid, time(0));
}

void gl_maze_free(uint8_t **grid) {
    for (uint32_t y = 0; y < gl_array_get_size(grid); y++) {
        gl_array_free(grid[y]);
    }
    gl_array_free(grid);
}

int main() {
    printf("1. create grid:\n\n");
    uint8_t **grid = gl_grid_create(3, 3, 2, 2, 0,
                                    (gl_maze_element_t[]) {GL_MAZE_ELEMENT_PILLAR, GL_MAZE_ELEMENT_WALL_CLOSED,
                                                           GL_MAZE_ELEMENT_WALL_CLOSED, GL_MAZE_ELEMENT_ROOM});
    printf("2. generate:\n\n");
    gl_maze_generate_from_grid(grid);
    gl_maze_free(grid);
    
    gl_memory_check_for_leaks();
    
    return 0;
}