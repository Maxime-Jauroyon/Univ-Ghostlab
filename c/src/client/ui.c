#include <client/ui.h>
#include <string.h>
#include <cimgui/cimgui.h>
#include <common/ui.h>
#include <common/game.h>
#include <common/array.h>
#include <common/string.h>
#include <common/message.h>
#include <client/shared.h>
#include <client/command.h>

static bool g_main_window_reload_games = true;
static bool g_console_window_visible = true;
static bool g_create_game_popup_visible = false;
static bool g_join_game_popup_visible = false;
static bool g_game_over_popup_visible = false;
static uint32_t g_join_game_popup_game_id = 0;
static uint32_t g_error = 0;
static char g_main_window_movement[4] = { '1', 0, 0, 0 };
static char g_main_window_message[201] = { 0 };
static int32_t g_main_window_message_target = 0;

void gl_client_draw() {
    while (!g_should_quit) {
        gl_ui_start_render(&g_should_quit);
    
        gl_client_main_window_draw();
        
        if (g_create_game_popup_visible) {
            gl_client_create_game_popup_draw();
        } else if (g_join_game_popup_visible) {
            gl_client_join_game_popup_draw();
        } else if (g_game_over_popup_visible) {
            gl_client_game_over_popup_draw();
        }
    
        gl_ui_end_render();
    }
}

void gl_client_main_window_draw() {
    gl_igBegin("Ghostlab Client", g_console_window_visible ? 0.6f : 1.0f);
    
    gl_client_main_window_menu_bar_draw();
    
    if (!gl_client_get_game()) {
        if (igButton("Create Game", (ImVec2) {0, 0})) {
            g_create_game_popup_visible = true;
        }
    }
    
    if (gl_client_get_game() && igCollapsingHeaderTreeNodeFlags(gl_client_get_game()->name, 0)) {
        if (gl_client_get_game()->started) {
            igText("Game has started.");
        } else {
            igText("Waiting for all players to be ready...");
        }
        
        if (!gl_client_get_player()->ready) {
            if (igButton("Ready", (ImVec2) { 0, 0 })) {
                gl_message_t msg = { .type = GL_MESSAGE_TYPE_START, .parameters_value = 0 };
                gl_message_send_tcp(g_tcp_listener_socket, &msg);
                gl_client_get_player()->ready = true;
            }
        }
        
        if (!gl_client_get_player()->ready || gl_client_get_game()->started) {
            if (igButton("Disconnect", (ImVec2) { 0, 0 })) {
                gl_client_disconnect(false, false);
            }
        }
        
        if (gl_client_get_game()->started) {
            if (igCollapsingHeaderTreeNodeFlags("Movements", 0)) {
                igText("Enter a number:");
    
                igSameLine(0, -1);
    
                igInputText("###NumberInput", g_main_window_movement, 4, ImGuiInputTextFlags_CallbackCharFilter, gl_igFilterNumbers, 0);

                bool cannot_move = strlen(g_main_window_movement) == 0;
                
                if (cannot_move) {
                    igPushItemFlag(ImGuiItemFlags_Disabled, true);
                    igPushStyleVarFloat(ImGuiStyleVar_Alpha, igGetStyle()->Alpha * 0.5f);
                }
                
#if GHOSTLAB_GUI
                if (igButton("Move Up", (ImVec2) { 208, 0 })) {
#else
                if (igButton("Move Up", (ImVec2) { 0, 0 })) {
#endif
                    gl_message_t msg = { .type = GL_MESSAGE_TYPE_UPMOV, .parameters_value = 0 };
                    gl_message_push_parameter(&msg, (gl_message_parameter_t) { .string_value = gl_string_create_from_number(g_main_window_movement, 3) });
                    gl_message_send_tcp(g_tcp_listener_socket, &msg);
                }

#if GHOSTLAB_GUI
                if (igButton("Move Left", (ImVec2) { 100, 0 })) {
#else
                if (igButton("Move Left", (ImVec2) { 0, 0 })) {
#endif
                    gl_message_t msg = { .type = GL_MESSAGE_TYPE_LEMOV, .parameters_value = 0 };
                    gl_message_push_parameter(&msg, (gl_message_parameter_t) { .string_value = gl_string_create_from_number(g_main_window_movement, 3) });
                    gl_message_send_tcp(g_tcp_listener_socket, &msg);
                }
    
                igSameLine(0, -1);

#if GHOSTLAB_GUI
                if (igButton("Move Right", (ImVec2) { 100, 0 })) {
#else
                if (igButton("Move Right", (ImVec2) { 0, 0 })) {
#endif
                    gl_message_t msg = { .type = GL_MESSAGE_TYPE_RIMOV, .parameters_value = 0 };
                    gl_message_push_parameter(&msg, (gl_message_parameter_t) { .string_value = gl_string_create_from_number(g_main_window_movement, 3) });
                    gl_message_send_tcp(g_tcp_listener_socket, &msg);
                }

#if GHOSTLAB_GUI
                if (igButton("Move Down", (ImVec2) { 208, 0 })) {
#else
                if (igButton("Move Down", (ImVec2) { 0, 0 })) {
#endif
                    gl_message_t msg = { .type = GL_MESSAGE_TYPE_DOMOV, .parameters_value = 0 };
                    gl_message_push_parameter(&msg, (gl_message_parameter_t) { .string_value = gl_string_create_from_number(g_main_window_movement, 3) });
                    gl_message_send_tcp(g_tcp_listener_socket, &msg);
                }
    
                if (cannot_move) {
                    igPopStyleVar(1);
                    igPopItemFlag();
                }
            }
    
    
            if (igCollapsingHeaderTreeNodeFlags("Messages", 0)) {
                gl_game_t *game = gl_client_get_game();
        
                if (igButton("Reload Players Data###ReloadPlayersData2", (ImVec2) {0, 0}) || game->reload_players_data_2) {
                    gl_client_reload_game_players_data(game->id);
                    game->reload_players_data_2 = false;
                }
        
                if (gl_array_get_size(gl_client_get_game()->players) > 1) {
                    igText("Enter a message:");
            
                    igInputText("###SendMessage", g_main_window_message, 201, ImGuiInputTextFlags_CallbackCharFilter, gl_igFilterWithSpaces, 0);
            
                    igText("Target:");
            
                    igComboStr_arr("###Targets", &g_main_window_message_target, (const char *const *)g_players_message_list, gl_array_get_size(g_players_message_list), -1);
            
                    if (igButton("Send", (ImVec2) { 0, 0 })) {
                        if (g_main_window_message_target == 0) {
                            gl_message_t msg = { .type = GL_MESSAGE_TYPE_MALL_REQ, .parameters_value = 0 };
                            gl_message_push_parameter(&msg, (gl_message_parameter_t) { .string_value = gl_string_create_from_cstring(g_main_window_message) });
                            gl_message_send_tcp(g_tcp_listener_socket, &msg);
                        } else {
                            gl_message_t msg = { .type = GL_MESSAGE_TYPE_SEND_REQ, .parameters_value = 0 };
                            gl_message_push_parameter(&msg, (gl_message_parameter_t) { .string_value = gl_string_create_from_cstring(g_players_message_list[g_main_window_message_target]) });
                            gl_message_push_parameter(&msg, (gl_message_parameter_t) { .string_value = gl_string_create_from_cstring(g_main_window_message) });
                            gl_message_send_tcp(g_tcp_listener_socket, &msg);
                            memcpy(g_last_sent_message, g_main_window_message, 201);
                        }
    
                        bzero(g_main_window_message, 201);
                    }
                } else {
                    igText("Not enough players.");
                }
            } else {
                gl_client_get_game()->reload_players_data_2 = true;
            }
        }
    
        if (!gl_client_get_player()->ready || gl_client_get_game()->started) {
            gl_client_main_window_game_data_draw(gl_client_get_game(), true);
        }
    }
    
    if ((!gl_client_get_game() || !gl_client_get_game()->started) && (!gl_client_get_player() || !gl_client_get_player()->ready)) {
        if (igCollapsingHeaderTreeNodeFlags("Available Games", 0)) {
            if (g_main_window_reload_games) {
                gl_client_reload_games();
                g_main_window_reload_games = false;
            }
            
            if (igButton("Reload Games Data", (ImVec2) {0, 0})) {
                gl_client_reload_games();
            }
            
            if (gl_array_get_size(g_games) > 0) {
                for (uint32_t i = 0; i < gl_array_get_size(g_games); i++) {
                    if ((!gl_client_get_game() || gl_client_get_game()->id != g_games[i].id)) {
                        if (igCollapsingHeaderTreeNodeFlags(g_games[i].name, 0)) {
                            if (igButton("Join", (ImVec2) {0, 0})) {
                                g_join_game_popup_visible = true;
                                g_join_game_popup_game_id = g_games[i].id;
                            }
    
                            gl_client_main_window_game_data_draw(&g_games[i], false);
                        }
                    }
                }
            } else {
                igText("There are currently no games.");
            }
        } else {
            g_main_window_reload_games = true;
        }
    }
    
    igEnd();
    
    gl_client_console_window_draw();
}

void gl_client_main_window_menu_bar_draw() {
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

void gl_client_main_window_game_data_draw(struct gl_game_t *game, bool show_player) {
    if (igCollapsingHeaderTreeNodeFlags("Players", 0)) {
        if (igButton("Reload Players Data###ReloadPlayersData1", (ImVec2) {0, 0}) || game->reload_players_data_1) {
            gl_client_reload_game_players_data(game->id);
            game->reload_players_data_1 = false;
        }
        
        if (show_player) {
            if (game->started) {
                gl_player_t *player = gl_client_get_player();
                igText("- %s (you) [x: %d, y: %d, score: %d]", g_player_id, player->pos.x, player->pos.y, player->score);
            } else {
                igText("- %s (you)", g_player_id);
            }
        }
        
        for (uint32_t j = 0; j <  gl_array_get_size(game->players); j++) {
            if (strcmp(game->players[j].id, g_player_id) != 0) {
                if (game->started) {
                    igText("- %s [score: %d]", game->players[j].id, game->players[j].score);
                } else {
                    igText("- %s", game->players[j].id);
                }
            }
        }
    } else {
        game->reload_players_data_1 = true;
    }
    
    if (igCollapsingHeaderTreeNodeFlags("Maze", 0)) {
        if (!game->started && (game->reload_maze_data || igButton("Reload Maze Data", (ImVec2) {0, 0}))) {
            gl_client_reload_game_maze_size(game->id);
            game->reload_maze_data = false;
        }
    
        igText("Size: %dx%d", game->maze_size.x, game->maze_size.y);
        
        if (game->started) {
            ImGuiIO *io = igGetIO();

#if GHOSTLAB_GUI
            igPushFont(io->Fonts->Fonts.Data[io->Fonts->Fonts.Size - 1]);
            igPushStyleVarVec2(ImGuiStyleVar_ItemSpacing, (ImVec2) { 0, 0 });
#endif
    
            bool found = false;
            for (uint32_t y = 0; y < game->maze_size.y; y++) {
                char buf2[128] = { 0 };
                uint32_t buf2_idx = 0;
                
                for (uint32_t x = 0; x < game->maze_size.x; x++) {
                    if (!found) {
                        for (uint32_t j = 0; j < gl_array_get_size(game->players); j++) {
                            if (game->players[j].pos.x == x && game->players[j].pos.y == y && strcmp(game->players[j].id, g_player_id) == 0) {
                                found = true;
                                buf2[buf2_idx++] = '@';
                                break;
                            }
                        }
                        
                        if (found) {
                            continue;
                        } else {
                            buf2[buf2_idx++] = '?';
                        }
                    } else {
                        buf2[buf2_idx++] = '?';
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
    } else if (!game->started) {
        game->reload_maze_data = true;
    }
}

void gl_client_console_window_draw() {
    if (g_console_window_visible) {
        gl_igConsole(gl_client_command_definitions(), GL_COMMAND_TYPE_COUNT);
    }
}

void gl_client_create_game_popup_draw() {
    igOpenPopup("###CreateGame", 0);
    
    if (igBeginPopupModal("Create Game###CreateGame", 0, ImGuiWindowFlags_AlwaysAutoResize) | ImGuiWindowFlags_NoMove) {
        igText("Choose a name:");
        
        igSameLine(0, -1);
        
        igInputText("###PlayerName", g_temp_player_id, 9, ImGuiInputTextFlags_CallbackCharFilter, gl_igFilter, 0);
        
        if (igButton("Back", (ImVec2) { 0, 0 })) {
            gl_client_create_game_popup_close();
        }
        
        igSameLine(0, -1);
    
        bool cannot_create = strlen(g_temp_player_id) != 8;
        
        if (cannot_create) {
            igPushItemFlag(ImGuiItemFlags_Disabled, true);
            igPushStyleVarFloat(ImGuiStyleVar_Alpha, igGetStyle()->Alpha * 0.5f);
        }
        
        if (igButton("Create", (ImVec2) { 0, 0 })) {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_NEWPL, .parameters_value = 0 };
            gl_message_push_parameter(&msg, (gl_message_parameter_t) { .string_value = gl_string_create_from_cstring(g_temp_player_id) });
            gl_message_push_parameter(&msg, (gl_message_parameter_t) { .string_value = gl_string_create_from_number(g_udp_port, 4) });
            gl_message_send_tcp(g_tcp_listener_socket, &msg);
        }
    
        if (cannot_create) {
            igPopItemFlag();
            igPopStyleVar(1);
        }
        
        if (g_error) {
            igText("Error(s):");
            igText("- This name is already used or the game is unavailable!");
        }
        
        igEndPopup();
    }
}

void gl_client_join_game_popup_draw() {
    igOpenPopup("###JoinGame", 0);
    
    char title[512] = { 0 };
    sprintf(title, "Join Game %d###JoinGame", g_join_game_popup_game_id);
    if (igBeginPopupModal(title, 0, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {
        igText("Choose a name:");
        
        igSameLine(0, -1);
        
        igInputText("###PlayerName", g_temp_player_id, 9, ImGuiInputTextFlags_CallbackCharFilter, gl_igFilter, 0);
        
        if (igButton("Back", (ImVec2) { 0, 0 })) {
            gl_client_join_game_popup_close();
        }
        
        igSameLine(0, -1);
    
        bool cannot_join = strlen(g_temp_player_id) != 8;
        
        if (cannot_join) {
            igPushItemFlag(ImGuiItemFlags_Disabled, true);
            igPushStyleVarFloat(ImGuiStyleVar_Alpha, igGetStyle()->Alpha * 0.5f);
        }
        
        if (igButton("Join", (ImVec2) { 0, 0 })) {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_REGIS, .parameters_value = 0 };
            gl_message_push_parameter(&msg, (gl_message_parameter_t) { .string_value = gl_string_create_from_cstring(g_temp_player_id) });
            gl_message_push_parameter(&msg, (gl_message_parameter_t) { .string_value = gl_string_create_from_number(g_udp_port, 4) });
            gl_message_push_parameter(&msg, (gl_message_parameter_t) { .uint8_value = g_join_game_popup_game_id });
            gl_message_send_tcp(g_tcp_listener_socket, &msg);
        }
    
        if (cannot_join) {
            igPopItemFlag();
            igPopStyleVar(1);
        }
        
        if (g_error) {
            igText("Error(s):");
            igText("- This name is already used or the game is unavailable!");
        }
        
        igEndPopup();
    }
}

void gl_client_server_down_popup_draw() {
    bool quit = false;
    while (!quit) {
        gl_ui_start_render(&quit);
        
        igOpenPopup("###ServerDown", 0);
        
        if (igBeginPopupModal("Server Down###ServerDown", 0, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {
            igText("The server is down!");
            igText("Press any key to continue.");
            
            for (int i = 0; i < 512; ++i) {
                if (igIsKeyReleased(i) || igIsMouseDown(0)) {
                    igCloseCurrentPopup();
                    quit = true;
                    break;
                }
            }
            
            igEndPopup();
        }
    
        gl_ui_end_render();
    }
}

void gl_client_game_over_popup_draw() {
    igOpenPopup("###GameOver", 0);
    
    char title[512] = { 0 };
    if (gl_client_get_player()->won) {
        sprintf(title, "You Won###GameOver");
    } else {
        sprintf(title, "Game Over###GameOver");
    }
    if (igBeginPopupModal(title, 0, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {
        if (gl_client_get_player()->won) {
            igText("Congratulations! You won!");
        } else {
            igText("Too bad... You lost! You'll do better next time!");
        }
        
        if (igButton("Back", (ImVec2) { 0, 0 })) {
            gl_client_disconnect(false, true);
            g_game_over_popup_visible = false;
        }
        
        igEndPopup();
    }
}

void gl_client_game_over_popup_show() {
    g_game_over_popup_visible = true;
}

void gl_client_create_game_popup_close() {
    g_create_game_popup_visible = false;
    gl_client_ui_error(0);
}

void gl_client_join_game_popup_close() {
    g_join_game_popup_visible = false;
    g_join_game_popup_game_id = 0;
    gl_client_ui_error(0);
}

void gl_client_ui_error(uint32_t error) {
    g_error = error;
}
