#include <client/gui.h>
#include <cimgui/cimgui.h>
#include <common/gui.h>
#include <common/game.h>
#include <common/array.h>
#include <common/string.h>
#include <common/message.h>
#include <client/shared.h>
#include <client/command.h>

static bool g_console_window_visible = true;
static bool g_create_game_popup_visible = false;
static uint32_t g_create_game_popup_error_type = 0;
static bool g_join_game_popup_visible = false;
static uint32_t g_join_game_popup_game_id = 0;
static uint32_t g_join_game_popup_error_type = 0;

void gl_client_draw() {
    while (!g_should_quit) {
        gl_gui_start_render(&g_should_quit);
    
        if (g_create_game_popup_visible) {
            gl_client_draw_create_game_popup();
        } else if (g_join_game_popup_visible) {
            gl_client_draw_join_game_popup();
        } else {
            gl_client_draw_main_window();
        }
    
        gl_gui_end_render();
    }
}

void gl_client_draw_main_window() {
    gl_igBegin("Ghostlab Client", g_console_window_visible ? 0.6f : 1.0f);
    
    gl_client_draw_main_window_menu_bar();
    
    if (!gl_client_get_game()) {
        if (igButton("Create Game", (ImVec2) {0, 0})) {
            g_create_game_popup_visible = true;
        }
    }
    
    if (gl_client_get_game() && igCollapsingHeaderTreeNodeFlags(gl_client_get_game()->name, 0)) {
        if (!gl_client_get_player()->ready) {
            if (igButton("Ready To Start", (ImVec2) { 0, 0 })) {
                gl_message_t msg = { .type = GL_MESSAGE_TYPE_START, .parameters_value = 0 };
                gl_message_send_tcp(g_server_socket, &msg);
                gl_message_wait_and_execute(g_server_socket, GL_MESSAGE_PROTOCOL_TCP);
            }
        } else {
            // TODO: When game started
        }
        
        if (!gl_client_get_player()->ready || gl_client_get_game()->started) {
            if (igButton("Disconnect", (ImVec2) { 0, 0 })) {
                gl_client_disconnect(false);
                gl_message_t msg = {.type = GL_MESSAGE_TYPE_GAME_REQ, .parameters_value = 0};
                gl_message_send_tcp(g_server_socket, &msg);
                gl_message_wait_and_execute(g_server_socket, GL_MESSAGE_PROTOCOL_TCP);
            }
        }
    }
    
    if (!gl_client_get_game() || !gl_client_get_game()->started) {
        if (igCollapsingHeaderTreeNodeFlags("Available Games", 0)) {
            if (igButton("Reload", (ImVec2) {0, 0})) {
                gl_message_t msg = {.type = GL_MESSAGE_TYPE_GAME_REQ, .parameters_value = 0};
                gl_message_send_tcp(g_server_socket, &msg);
                gl_message_wait_and_execute(g_server_socket, GL_MESSAGE_PROTOCOL_TCP);
            }
            
            if (gl_array_get_size(g_games) > 0) {
                for (uint32_t i = 0; i < gl_array_get_size(g_games); i++) {
                    if ((!gl_client_get_game() || gl_client_get_game()->id != g_games[i].id) && igCollapsingHeaderTreeNodeFlags(g_games[i].name, 0)) {
                        if (igButton("Join", (ImVec2) {0, 0})) {
                            g_join_game_popup_visible = true;
                            g_create_game_popup_visible = false;
                            g_join_game_popup_game_id = g_games[i].id;
                        }
                    }
                }
            }
        }
    }
    
    igEnd();
    
    gl_client_draw_console_window();
}

void gl_client_draw_main_window_menu_bar() {
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

void gl_client_draw_console_window() {
    if (g_console_window_visible) {
        gl_igConsole(gl_client_command_definitions(), GL_COMMAND_TYPE_COUNT);
    }
}

void gl_client_draw_create_game_popup() {
    igOpenPopup("###CreateGame", 0);
    
    if (igBeginPopupModal("Create Game###CreateGame", 0, ImGuiWindowFlags_AlwaysAutoResize)) {
        igText("Choose a name:");
        
        igSameLine(0, -1);
        
        igInputText("###PlayerName", g_player_id, 9, 0, 0, 0);
        
        if (igButton("Back", (ImVec2) { 0, 0 })) {
            g_create_game_popup_visible = false;
            g_create_game_popup_error_type = 0;
        }
        
        igSameLine(0, -1);
        
        if (igButton("Create", (ImVec2) { 0, 0 })) {
            if (gl_client_is_player_id_valid(g_player_id)) {
                gl_message_t msg = { .type = GL_MESSAGE_TYPE_NEWPL, .parameters_value = 0 };
                gl_message_push_parameter(&msg, (gl_message_parameter_t) {.string_value = gl_string_create_from_cstring(g_player_id)});
                gl_message_push_parameter(&msg, (gl_message_parameter_t) {.string_value = gl_string_create_from_number(g_udp_port, 4)});
                gl_message_send_tcp(g_server_socket, &msg);
                gl_message_wait_and_execute_no_lock(g_server_socket, GL_MESSAGE_PROTOCOL_TCP);
                
                if (g_game_id != -1) {
                    g_create_game_popup_visible = false;
                    g_create_game_popup_error_type = 0;
                } else {
                    g_create_game_popup_error_type = 2;
                }
            } else {
                g_create_game_popup_error_type = 1;
            }
        }
        
        if (g_create_game_popup_error_type) {
            igText("Error(s):");
            
            if (g_create_game_popup_error_type == 1) {
                igText("- This name format is invalid, it should contain 8 characters in the range [a-zA-Z0-9]!");
            } else {
                // TODO: Separate these two errors
                igText("- This name is already used or the game is unavailable!");
            }
        }
        
        igEndPopup();
    }
}

void gl_client_draw_join_game_popup() {
    igOpenPopup("###JoinGame", 0);
    
    char title[512] = { 0 };
    sprintf(title, "Join Game %d###JoinGame", g_join_game_popup_game_id);
    if (igBeginPopupModal(title, 0, ImGuiWindowFlags_AlwaysAutoResize)) {
        igText("Choose a name:");
        
        igSameLine(0, -1);
        
        igInputText("###PlayerName", g_player_id, 9, 0, 0, 0);
        
        if (igButton("Back", (ImVec2) { 0, 0 })) {
            g_join_game_popup_visible = false;
            g_join_game_popup_game_id = 0;
            g_join_game_popup_error_type = 0;
        }
        
        igSameLine(0, -1);
        
        if (igButton("Join", (ImVec2) { 0, 0 })) {
            if (gl_client_is_player_id_valid(g_player_id)) {
                gl_message_t msg = { .type = GL_MESSAGE_TYPE_REGIS, .parameters_value = 0 };
                gl_message_push_parameter(&msg, (gl_message_parameter_t) {.string_value = gl_string_create_from_cstring(g_player_id)});
                gl_message_push_parameter(&msg, (gl_message_parameter_t) {.string_value = gl_string_create_from_number(g_udp_port, 4)});
                gl_message_push_parameter(&msg, (gl_message_parameter_t) {.uint8_value = g_join_game_popup_game_id});
                gl_message_send_tcp(g_server_socket, &msg);
                gl_message_wait_and_execute_no_lock(g_server_socket, GL_MESSAGE_PROTOCOL_TCP);
                
                if (g_game_id != -1) {
                    g_join_game_popup_visible = false;
                    g_join_game_popup_game_id = 0;
                    g_join_game_popup_error_type = 0;
                } else {
                    g_join_game_popup_error_type = 2;
                }
            } else {
                g_join_game_popup_error_type = 1;
            }
        }
        
        if (g_join_game_popup_error_type) {
            igText("Error(s):");
            
            if (g_join_game_popup_error_type == 1) {
                igText("- This name format is invalid, it should contain 8 characters in the range [a-zA-Z0-9]!");
            } else {
                // TODO: Separate these two errors
                igText("- This name is already used or the game is unavailable!");
            }
        }
        
        igEndPopup();
    }
}

void gl_client_draw_server_down_popup() {
    bool quit = false;
    while (!quit) {
        gl_gui_start_render(&quit);
        
        igOpenPopup("###ServerDown", 0);
        
        if (igBeginPopupModal("Server Down###ServerDown", 0, ImGuiWindowFlags_AlwaysAutoResize)) {
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
        
        gl_gui_end_render();
    }
}
