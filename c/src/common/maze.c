#include <common/maze.h>
#include <time.h>
#include <stdlib.h>
#include <common/array.h>
#include <common/memory.h>

gl_maze_t *gl_maze_generate(uint8_t base_width, uint8_t base_height) {
    uint8_t width = 2 * base_width + 1;
    uint8_t height = 2 * base_height + 1;
    uint8_t color_scheme[2][2] = {
        {0, 1},
        {2, 3}
    };
    gl_maze_element_t element_per_color[4] = {GL_MAZE_ELEMENT_PILLAR, GL_MAZE_ELEMENT_WALL_CLOSED, GL_MAZE_ELEMENT_WALL_CLOSED, GL_MAZE_ELEMENT_ROOM};
    uint8_t **grid = gl_maze_create_color_grid(width, height, (uint8_t *) color_scheme, 2, 2);
    gl_maze_element_t **maze_grid = gl_maze_color_to_grid(grid, gl_maze_default_color_to_grid,
                                                          element_per_color, 4);
    gl_maze_generate_elements(maze_grid, gl_maze_default_random_initial_room_position,
                              gl_default_choose_random_wall);

    gl_maze_t *maze = gl_malloc(sizeof(*maze));
    maze->grid = maze_grid;
  
    return maze;
}

void gl_maze_free(gl_maze_t *maze) {
    if (maze) {
        for (uint32_t y = 0; y < gl_array_get_size(maze->grid); y++) {
            gl_array_free(maze->grid[y]);
        }
        gl_array_free(maze->grid);
    }
    
    gl_free(maze);
}

void gl_maze_default_color_to_grid(gl_maze_element_t **maze, uint8_t **grid, uint8_t width, uint8_t height, const gl_maze_element_t *element_per_color, uint8_t size_element_per_color) {
    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            maze[y][x] = element_per_color[grid[y][x]];
        }
    }
}

gl_pos_t gl_maze_default_random_initial_room_position(gl_maze_element_t **maze, uint8_t width, uint8_t height) {
    gl_pos_t initial_pos;
    
    do {
        initial_pos.x = rand() % width;
        initial_pos.y = rand() % height;
    } while (maze[initial_pos.y][initial_pos.x] != GL_MAZE_ELEMENT_ROOM);
    
    return initial_pos;
}

uint32_t gl_default_choose_random_wall(gl_pos_t *walls, uint32_t size) {
    return rand() % size;
}

uint8_t **gl_maze_create_color_grid(uint8_t width, uint8_t height, uint8_t *scheme, uint8_t scheme_width, uint8_t scheme_height) {
    if (width < 2 || height < 2 || !scheme || scheme_width < 1 || scheme_height < 1) {
        return 0;
    }

    uint8_t **grid = gl_array_create(uint8_t*, height);
    for (uint32_t y = 0; y < height; y++) {
        grid[y] = gl_array_create(uint8_t, width);
        for (uint32_t x = 0; x < width; x++) {
            grid[y][x] = scheme[y % scheme_height * scheme_width + x % scheme_width];
        }
    }

    return grid;
}

gl_maze_element_t **gl_maze_color_to_grid(uint8_t **grid, void (*gl_color_to_maze_comparator)(gl_maze_element_t **, uint8_t**, uint8_t, uint8_t, const gl_maze_element_t*, uint8_t), const gl_maze_element_t* element_per_color, uint8_t size_element_per_color) {
    if (!grid) {
        return 0;
    }
    
    uint32_t height = gl_array_get_size(grid);
    uint32_t width = gl_array_get_size(grid[0]);
    
    gl_maze_element_t **maze = gl_array_create(gl_maze_element_t*, height);
    for (uint32_t y = 0; y < height; y++) {
        maze[y] = gl_array_create(gl_maze_element_t, width);
        for (uint32_t x = 0; x < width; x++) {
            maze[y][x] = GL_MAZE_ELEMENT_COUNT;
        }
    }

    gl_color_to_maze_comparator(maze, grid, width, height, element_per_color, size_element_per_color);

    for (uint32_t y = 0; y < gl_array_get_size(grid); y++) {
        gl_array_free(grid[y]);
    }
    gl_array_free(grid);
    
    return maze;
}

gl_maze_element_t **gl_maze_generate_elements_with_seed(gl_maze_element_t **maze, uint64_t seed, gl_pos_t (* initial_room_comparator)(gl_maze_element_t **, uint8_t, uint8_t), uint32_t (* wall_comparator)(gl_pos_t *, uint32_t)) {
    srand(seed);
    
    if (!maze) {
        return 0;
    }
    
    uint32_t height = gl_array_get_size(maze);
    uint32_t width = gl_array_get_size(maze[0]);
    
    if (width < 5 || height < 5) {
        return 0;
    }
    
    gl_pos_t initial_pos = initial_room_comparator(maze, width, height);
    gl_pos_t *path = 0;
    gl_array_push(path, initial_pos);
    
    gl_pos_t *walls = 0;
    gl_array_push(walls, ((gl_pos_t) { initial_pos.x, initial_pos.y - 1 }));
    gl_array_push(walls, ((gl_pos_t) { initial_pos.x + 1, initial_pos.y }));
    gl_array_push(walls, ((gl_pos_t) { initial_pos.x, initial_pos.y + 1 }));
    gl_array_push(walls, ((gl_pos_t) { initial_pos.x - 1, initial_pos.y }));
    
    while (!gl_array_is_empty(walls)) {
        uint32_t wall_idx = wall_comparator(walls, gl_array_get_size(walls));
        gl_pos_t wall_pos = walls[wall_idx];
        gl_array_remove(walls, wall_idx);
        
        if (maze[wall_pos.y][wall_pos.x] == GL_MAZE_ELEMENT_WALL_OPENED) {
            continue;
        }
        
        gl_pos_t *rooms = 0;
        
        if (wall_pos.y - 1 >= 0 && wall_pos.y - 1 < height && wall_pos.x >= 0 && wall_pos.x < width && maze[wall_pos.y - 1][wall_pos.x] == GL_MAZE_ELEMENT_ROOM) {
            gl_array_push(rooms, ((gl_pos_t) { wall_pos.x, wall_pos.y - 1 }));
        }
        if (wall_pos.y >= 0 && wall_pos.y < height && wall_pos.x + 1 >= 0 && wall_pos.x + 1 < width && maze[wall_pos.y][wall_pos.x + 1] == GL_MAZE_ELEMENT_ROOM) {
            gl_array_push(rooms, ((gl_pos_t) { wall_pos.x + 1, wall_pos.y }));
        }
        if (wall_pos.y + 1 >= 0 && wall_pos.y + 1 < height && wall_pos.x >= 0 && wall_pos.x < width && maze[wall_pos.y + 1][wall_pos.x] == GL_MAZE_ELEMENT_ROOM) {
            gl_array_push(rooms, ((gl_pos_t) { wall_pos.x, wall_pos.y + 1 }));
        }
        if (wall_pos.y >= 0 && wall_pos.y < height && wall_pos.x - 1 >= 0 && wall_pos.x - 1 < width && maze[wall_pos.y][wall_pos.x - 1] == GL_MAZE_ELEMENT_ROOM) {
            gl_array_push(rooms, ((gl_pos_t) { wall_pos.x - 1, wall_pos.y }));
        }
        
        if (gl_array_get_size(rooms) == 2) {
            uint32_t found = 0;
            gl_pos_t room;
            
            for (uint32_t i = 0; i < gl_array_get_size(path); i++) {
                if (path[i].x == rooms[0].x && path[i].y == rooms[0].y) {
                    found++;
                    room = rooms[1];
                }
                if (path[i].x == rooms[1].x && path[i].y == rooms[1].y) {
                    found++;
                    room = rooms[0];
                }
            }
            
            if (found == 1) {
                maze[wall_pos.y][wall_pos.x] = GL_MAZE_ELEMENT_WALL_OPENED;
                
                gl_array_push(path, room);
                
                if (room.y - 1 >= 0 && room.y - 1 < height && room.x >= 0 && room.x < width && maze[room.y - 1][room.x] == GL_MAZE_ELEMENT_WALL_CLOSED) {
                    gl_array_push(walls, ((gl_pos_t) { room.x, room.y - 1 }));
                }
                if (room.y >= 0 && room.y < height && room.x + 1 >= 0 && room.x + 1 < width && maze[room.y][room.x + 1] == GL_MAZE_ELEMENT_WALL_CLOSED) {
                    gl_array_push(walls, ((gl_pos_t) { room.x + 1, room.y }));
                }
                if (room.y + 1 >= 0 && room.y + 1 < height && room.x >= 0 && room.x < width && maze[room.y + 1][room.x] == GL_MAZE_ELEMENT_WALL_CLOSED) {
                    gl_array_push(walls, ((gl_pos_t) { room.x, room.y + 1 }));
                }
                if (room.y >= 0 && room.y < height && room.x - 1 >= 0 && room.x - 1 < width && maze[room.y][room.x - 1] == GL_MAZE_ELEMENT_WALL_CLOSED) {
                    gl_array_push(walls, ((gl_pos_t) { room.x - 1, room.y }));
                }
            }
        }
        
        gl_array_free(rooms);
    }
    
    gl_array_free(walls);
    gl_array_free(path);

    return maze;
}

gl_maze_element_t **gl_maze_generate_elements(gl_maze_element_t **maze, gl_pos_t (* initial_room_comparator)(gl_maze_element_t **, uint8_t, uint8_t), uint32_t ( *wall_comparator)(gl_pos_t *, uint32_t)) {
    return gl_maze_generate_elements_with_seed(maze, time(0), initial_room_comparator, wall_comparator);
}
