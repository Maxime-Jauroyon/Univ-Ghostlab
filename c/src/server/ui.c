#include <server/ui.h>
#include <cimgui/cimgui.h>
#include <common/ui.h>
#include <common/maze.h>
#include <common/game.h>
#include <common/array.h>
#include <server/shared.h>
#include <server/command.h>

static bool g_console_window_visible = true;

void gl_server_draw() {
    while (!g_should_quit) {
        gl_ui_start_render(&g_should_quit);
        gl_server_draw_main_window();
        gl_ui_end_render();
    }
}

void gl_server_draw_main_window() {
    char buf[512] = { 0 };
    
    gl_igBegin("Ghostlab Server", g_console_window_visible ? 0.6f : 1.0f);
    
    gl_server_draw_main_window_menu_bar();
    
    if (igCollapsingHeaderTreeNodeFlags("Available Games", 0)) {
        if (gl_array_get_size(g_games) > 0) {
            for (uint32_t i = 0; i < gl_array_get_size(g_games); i++) {
                if (!g_games[i].over && igCollapsingHeaderTreeNodeFlags(g_games[i].name, 0)) {
                    if (g_games[i].started) {
                        igText("Game has started.");
                    } else {
                        igText("Waiting for all players to be ready...");
                    }
                    
                    sprintf(buf, "Players###Players%d", g_games[i].id);
                    if (igCollapsingHeaderTreeNodeFlags(buf, 0)) {
                        for (uint32_t j = 0; j <  gl_array_get_size(g_games[i].players); j++) {
                            if (!g_games[i].started) {
                                if (g_games[i].players[j].ready) {
                                    sprintf(buf, " (ready)");
                                } else {
                                    sprintf(buf, " (not ready)");
                                }
                            } else {
                                sprintf(buf, " [x: %d, y: %d, score: %d]", g_games[i].players[j].pos.x, g_games[i].players[j].pos.y, g_games[i].players[j].score);
                            }
                            igText("- %s%s", g_games[i].players[j].id, buf);
                        }
                    }
    
                    if (g_games[i].started) {
                        sprintf(buf, "Ghosts###Ghosts%d", g_games[i].id);
                        if (igCollapsingHeaderTreeNodeFlags(buf, 0)) {
                            for (uint32_t j = 0; j <  gl_array_get_size(g_games[i].ghosts); j++) {
                                igText("- Ghost %d [x: %d, y: %d]", j, g_games[i].ghosts[j].pos.x, g_games[i].ghosts[j].pos.y);
                            }
                        }
                    }
    
                    sprintf(buf, "Maze###Maze%d", g_games[i].id);
                    if (igCollapsingHeaderTreeNodeFlags(buf, 0)) {
                        gl_pos_t size = gl_game_get_maze_size(&g_games[i]);
                        
                        if  (g_games[i].started) {
                            igText("Size: %dx%d", size.x, size.y);
                        } else {
                            igText("Size: %dx%d", 2 * size.x + 1, 2 * size.y + 1);
                            igText("Not yet generated!");
                        }
                        
                        if (g_games[i].maze) {
                            ImGuiIO *io = igGetIO();

#if GHOSTLAB_GUI
                            igPushFont(io->Fonts->Fonts.Data[io->Fonts->Fonts.Size - 1]);
                            igPushStyleVarVec2(ImGuiStyleVar_ItemSpacing, (ImVec2) { 0, 0 });
#endif
                            
                            for (uint32_t y = 0; y < gl_array_get_size(g_games[i].maze->grid); y++) {
                                char buf2[128] = { 0 };
                                uint32_t buf2_idx = 0;
        
                                for (uint32_t x = 0; x < gl_array_get_size(g_games[i].maze->grid[y]); x++) {
                                    bool found = false;
                                    for (uint32_t j = 0; j < gl_array_get_size(g_games[i].ghosts); j++) {
                                        if (g_games[i].ghosts[j].pos.x == x && g_games[i].ghosts[j].pos.y == y) {
                                            found = true;
                                            buf2[buf2_idx++] = 'G';
                                            break;
                                        }
                                    }
                                    if (!found) {
                                        for (uint32_t j = 0; j < gl_array_get_size(g_games[i].players); j++) {
                                            if (g_games[i].players[j].pos.x == x && g_games[i].players[j].pos.y == y) {
                                                found = true;
                                                buf2[buf2_idx++] = 'P';
                                                break;
                                            }
                                        }
                                    }
                                    if (!found) {
                                        if (g_games[i].maze->grid[y][x] == GL_MAZE_ELEMENT_PILLAR || g_games[i].maze->grid[y][x] == GL_MAZE_ELEMENT_WALL_CLOSED) {
                                            buf2[buf2_idx++] = '#';
                                        } else if (g_games[i].maze->grid[y][x] == GL_MAZE_ELEMENT_WALL_OPENED || g_games[i].maze->grid[y][x] == GL_MAZE_ELEMENT_ROOM) {
                                            buf2[buf2_idx++] = ' ';
                                        }
                                    }
                                }

#if GHOSTLAB_GUI
                                if (y > 0) {
                                    igSetCursorPosY(igGetCursorPosY() - 6);
                                }
#endif
                                igTextUnformatted(buf2, 0);
                            }

#if GHOSTLAB_GUI
                            igPopStyleVar(1);
                            igPopFont();
#endif
                        }
                    }
                }
            }
        } else {
            igText("There are currently no games.");
        }
    }
    
    igEnd();
    
    gl_server_draw_console_window();
}

void gl_server_draw_main_window_menu_bar() {
    if (igBeginMenuBar()) {
        if (igBeginMenu("File", true)) {
            igMenuItemBoolPtr("Quit", 0, &g_should_quit, true);
            igEndMenu();
        }
        
        if (igBeginMenu("View", true)) {
            igMenuItemBoolPtr("Show Logs", 0, &g_console_window_visible, true);
            igEndMenu();
        }
        
        igEndMenuBar();
    }
}

void gl_server_draw_console_window() {
    if (g_console_window_visible) {
        gl_igConsole(gl_client_command_definitions(), GL_COMMAND_TYPE_COUNT);
    }
}
