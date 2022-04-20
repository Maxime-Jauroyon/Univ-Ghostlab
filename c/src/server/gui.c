#include <server/gui.h>
#include <cimgui/cimgui.h>
#include <common/gui.h>
#include <common/maze.h>
#include <common/game.h>
#include <common/array.h>
#include <server/shared.h>
#include <server/command.h>

static bool g_console_window_visible = true;

void gl_server_draw() {
    while (!g_should_quit) {
        gl_gui_start_render(&g_should_quit);
        gl_server_draw_main_window();
        gl_gui_end_render();
    }
}

void gl_server_draw_main_window() {
    gl_igBegin("Ghostlab Server", g_console_window_visible ? 0.6f : 1.0f);
    
    gl_server_draw_main_window_menu_bar();
    
    if (gl_array_get_size(g_games) > 0) {
        if (igCollapsingHeaderTreeNodeFlags("Available Games", 0)) {
            for (uint32_t i = 0; i < gl_array_get_size(g_games); i++) {
                if (igCollapsingHeaderTreeNodeFlags(g_games[i].name, 0)) {
                    if (g_games[i].started) {
                        igText("Game has started.");
                    } else {
                        igText("Waiting for all players to be ready to start the game.");
                    }
                    
                    char buf1[128] = { 0 };
                    sprintf(buf1, "Players (%d connected)###PlayersConnected", (uint32_t)gl_array_get_size(g_games[i].players));
                    if (igCollapsingHeaderTreeNodeFlags(buf1, 0)) {
                        for (uint32_t j = 0; j <  gl_array_get_size(g_games[i].players); j++) {
                            igText("- %s%s", g_games[i].players[j].id, !g_games[i].started && g_games[i].players[j].ready ? " (ready)" : "");
                        }
                    }
                    
                    if (g_games[i].maze) {
                        if (igCollapsingHeaderTreeNodeFlags("Maze", 0)) {
                            ImGuiIO *io = igGetIO();
                            
                            igPushFont(io->Fonts->Fonts.Data[io->Fonts->Fonts.Size - 1]);
                            igPushStyleVarVec2(ImGuiStyleVar_ItemSpacing, (ImVec2) { 0, 0 });
                            
                            for (uint32_t y = 0; y < gl_array_get_size(g_games[i].maze->grid); y++) {
                                char buf2[128] = { 0 };
                                uint32_t buf2_idx = 0;
                                
                                for (uint32_t x = 0; x < gl_array_get_size(g_games[i].maze->grid[y]); x++) {
                                    if (g_games[i].maze->grid[y][x] == GL_MAZE_ELEMENT_PILLAR || g_games[i].maze->grid[y][x] == GL_MAZE_ELEMENT_WALL_CLOSED) {
                                        buf2[buf2_idx++] = '#';
                                    } else if (g_games[i].maze->grid[y][x] == GL_MAZE_ELEMENT_WALL_OPENED || g_games[i].maze->grid[y][x] == GL_MAZE_ELEMENT_ROOM) {
                                        buf2[buf2_idx++] = ' ';
                                    }
                                }
                                
                                igTextUnformatted(buf2, 0);
                            }
                            
                            igPopStyleVar(1);
                            igPopFont();
                        }
                    }
                }
            }
        }
    } else {
        igText("There are currently no games.");
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
