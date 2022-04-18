#include <common/types.h>
#include <errno.h>
#include <stdlib.h>
#include <getopt.h>
#include <cimgui/cimgui.h>
#include <common/log.h>
#include <common/gui.h>
#include <common/memory.h>
#include <common/network.h>
#include <common/command.h>
#include <client/shared.h>
#include <client/command.h>
#include <pthread.h>
#include <string.h>
#include <ctype.h>
#include "common/message.h"
#include "message.h"
#include "thread_multicast_server.h"
#include "thread_udp.h"
#include "common/array.h"
#include "common/maze.h"
#include "common/game.h"
#include "common/string.h"

static void draw_main_gui();
static void draw_join_game_popup(bool create);
static void draw_server_down_popup();

static int32_t g_exit_code = EXIT_SUCCESS;
static bool g_show_console = true;
static bool g_join_game_popup = false;
static bool g_create_game_popup = false;
static char join_game_player_id_buf[9] = {0 };
static bool join_game_error = false;
static uint8_t join_game_id = 0;

int main(int argc, char **argv) {
    errno = 0;

    static struct option opts[] = {
        { "ip", required_argument, 0, 'i' },
        { "port", required_argument, 0, 'p' },
        { "name", required_argument, 0, 'n' },
        { "udp-port", required_argument, 0, 'u' },
        { "legacy-protocol", required_argument, 0, 'l' },
        { "help", no_argument, 0, 'h' },
        { "version", no_argument, 0, 'v' },
        {0, 0, 0, 0}
    };
    int32_t used_unknown_opt = 0;
    int32_t opt;
    while ((opt = getopt_long(argc, argv, "i:p:n:u:lhv", opts, 0)) != -1) {
        switch (opt) {
        case 'i':
            // TODO: Check if ip is valid, if invalid use default
            g_server_ip = gl_strdup(optarg);
            break;
        case 'p':
            // TODO: Check if port is valid, if invalid use default
            g_server_port = gl_strdup(optarg);
            break;
        case 'n':
            // TODO: Check if name is valid, if invalid use default
            g_player_id = gl_strdup(optarg);
            break;
        case 'u':
            // TODO: Check if port is valid, if invalid use default
            g_udp_port = gl_strdup(optarg);
            break;
        case 'l':
            g_legacy_protocol = true;
            break;
        case 'h':
            gl_log_push("%s", g_help);
            goto cleanup;
        case 'v':
            gl_log_push("version: " GHOSTLAB_VERSION);
            goto cleanup;
        case '?':
            used_unknown_opt = 1;
            gl_log_push_error("use `-h` for more informations.\n");
            break;
        default:
            used_unknown_opt = 1;
            gl_log_push_error("option not yet implemented `%c`!\n", opt);
            gl_log_push_error("use `-h` for more informations.\n");
        }
    }

    if (used_unknown_opt) {
        goto error;
    }

    if (!g_server_ip) {
        g_server_ip = gl_strdup(GHOSTLAB_DEFAULT_SERVER_IP);
    }
    if (!g_server_port) {
        g_server_port = gl_strdup(GHOSTLAB_DEFAULT_SERVER_PORT);
    }
    if (!g_udp_port) {
        g_udp_port = gl_strdup(GHOSTLAB_DEFAULT_UDP_PORT);
    }
    
    gl_message_set_mutex(g_gameplay_mutex);
    gl_message_add_functions();
    
    gl_gui_create("Ghostlab Client");
    
    // TODO: use gl_connect_to_server();
    g_server_tcp_socket = gl_socket_create(g_server_ip, g_server_port, GL_SOCKET_TYPE_TCP_CLIENT, 0);
    if (g_server_tcp_socket == -1) {
        draw_server_down_popup();
        goto error;
    }
    gl_log_push("connection to server established.");
    if (!g_legacy_protocol) {
        gl_message_wait_and_execute(g_server_tcp_socket, GL_MESSAGE_PROTOCOL_TCP);
        g_multicast_server_thread = gl_malloc(sizeof(pthread_t));
        pthread_create(g_multicast_server_thread, 0, gl_thread_multicast_server_main, 0);
    }
    gl_message_wait_and_execute(g_server_tcp_socket, GL_MESSAGE_PROTOCOL_TCP);
    
    g_udp_thread = gl_malloc(sizeof(pthread_t));
    pthread_create(g_udp_thread, 0, gl_thread_udp_main, 0);
    
    while (!g_quit) {
        gl_gui_start_render(&g_quit);
        
        if (g_join_game_popup) {
            draw_join_game_popup(g_create_game_popup);
        } else {
            draw_main_gui();
        }
        
        gl_gui_end_render();
    }

    goto cleanup;

    error:
    g_exit_code = 1;
    
    cleanup:
    if (g_udp_thread) {
        gl_socket_close(&g_udp_socket);
        pthread_join(*(pthread_t *)g_udp_thread, 0);
        gl_free(g_udp_thread);
    }
    
    if (!g_legacy_protocol && g_multicast_server_thread) {
        gl_socket_close(&g_multicast_server_socket);
        pthread_join(*(pthread_t *)g_multicast_server_thread, 0);
        gl_free(g_multicast_server_thread);
    }

    if (g_server_tcp_socket) {
        gl_message_t msg = {.type = GL_MESSAGE_TYPE_IQUIT, .parameters_value = 0};
        gl_message_send_tcp(g_server_tcp_socket, &msg);
        gl_message_wait_and_execute(g_server_tcp_socket, GL_MESSAGE_PROTOCOL_TCP);
    }
    
    if (g_server_down) {
        draw_server_down_popup();
    }
    
    gl_free_games();
    
    gl_free(g_server_ip);
    gl_free(g_server_port);
    gl_free(g_player_id);
    gl_free(g_udp_port);
    gl_free(g_multicast_ip);
    gl_free(g_multicast_port);
    
    
    gl_log_free();
    
    gl_memory_check_for_leaks();

    gl_gui_free();

    return g_exit_code;
}

static void draw_main_gui() {
    {
        gl_igBegin("Ghostlab Client", g_show_console ? 0.6f : 1.0f);

        if (igBeginMenuBar()) {
            if (igBeginMenu("File", true)) {
                igMenuItemBoolPtr("Quit", 0, &g_quit, true);
                igEndMenu();
            }
            if (igBeginMenu("View", true)) {
                igMenuItemBoolPtr("Show Logs", 0, &g_show_console, true);
                igEndMenu();
            }
            igEndMenuBar();
        }
        
        if (g_current_game_id == -1) {
            if (igButton("Create Game", (ImVec2) { 0, 0 })) {
                g_join_game_popup = true;
                g_create_game_popup = true;
            }
            igSameLine(0, -1);
            if (igButton("Reload Available Games", (ImVec2) { 0, 0 })) {
                gl_message_t msg = { .type = GL_MESSAGE_TYPE_GAME_REQ, .parameters_value = 0 };
                gl_message_send_tcp(g_server_tcp_socket, &msg);
                gl_message_wait_and_execute(g_server_tcp_socket, GL_MESSAGE_PROTOCOL_TCP);
            }
            igSameLine(0, -1);
            if (igButton("Quit", (ImVec2) { 0, 0 })) {
                g_quit = true;
            }
            
            if (gl_array_get_size(g_games) > 0) {
                if (igCollapsingHeaderTreeNodeFlags("Available Games", 0)) {
                    for (uint32_t i = 0; i < gl_array_get_size(g_games); i++) {
                        char buf[512] = { 0 };
                        sprintf(buf, "Join %s", g_games[i].name);
                        if (igButton(buf, (ImVec2) { 0, 0 })) {
                            g_join_game_popup = true;
                            g_create_game_popup = false;
                            join_game_id = g_games[i].id;
                        }
                    }
                }
            }
        } else {
            if (igButton("Ready To Start", (ImVec2) { 0, 0 })) {
                gl_message_t msg = { .type = GL_MESSAGE_TYPE_START, .parameters_value = 0 };
                gl_message_send_tcp(g_server_tcp_socket, &msg);
            }
            igSameLine(0, -1);
            if (igButton("Disconnect", (ImVec2) { 0, 0 })) {
                gl_message_t msg = { .type = GL_MESSAGE_TYPE_IQUIT, .parameters_value = 0 };
                gl_message_send_tcp(g_server_tcp_socket, &msg);
                gl_message_wait_and_execute(g_server_tcp_socket, GL_MESSAGE_PROTOCOL_TCP);
            }
            igSameLine(0, -1);
            if (igButton("Quit", (ImVec2) { 0, 0 })) {
                g_quit = true;
            }
        }

        igEnd();
    }

    if (g_show_console) {
        gl_igConsole(gl_command_definitions(), GL_COMMAND_TYPE_COUNT);
    }
}

static void draw_join_game_popup(bool create) {
    igOpenPopup("###Game", 0);
    
    if (igBeginPopupModal(create ? "Create Game###Game" : "Join Game###Game", 0, ImGuiWindowFlags_AlwaysAutoResize)) {
        igText(" ");
        igInputText("Choose a name.", join_game_player_id_buf, 9, 0, 0, 0);
        igText(" ");
        if (igButton("Back", (ImVec2) { 0, 0 })) {
            g_join_game_popup = false;
            g_create_game_popup = false;
            join_game_error = false;
        }
        igSameLine(0, -1);
        if (igButton(create ? "Create" : "Join", (ImVec2) { 0, 0 })) {
            bool error = strlen(join_game_player_id_buf) != 8;
            
            if (!error) {
                for (uint32_t i = 0; i < strlen(join_game_player_id_buf); i++) {
                    if (!isalpha(join_game_player_id_buf[i]) && !isdigit(join_game_player_id_buf[i])) {
                        error = true;
                        break;
                    }
                }
            }
            
            if (!error) {
                gl_message_t msg;
                if (create) {
                    msg = (gl_message_t) { .type = GL_MESSAGE_TYPE_NEWPL, .parameters_value = 0 };
                } else {
                    msg = (gl_message_t) { .type = GL_MESSAGE_TYPE_REGIS, .parameters_value = 0 };
                }
                gl_message_push_parameter(&msg, (gl_message_parameter_t) {.string_value = gl_string_create_from_cstring(join_game_player_id_buf) });
                gl_message_push_parameter(&msg, (gl_message_parameter_t) {.string_value = gl_string_create_from_number(g_udp_port, 4) });
                if (!create) {
                    gl_message_push_parameter(&msg, (gl_message_parameter_t) {.uint8_value = join_game_id });
                }
                gl_message_send_tcp(g_server_tcp_socket, &msg);
                g_join_game_popup = false;
                g_create_game_popup = false;
                bzero(join_game_player_id_buf, 9);
                gl_message_wait_and_execute_no_lock(g_server_tcp_socket, GL_MESSAGE_PROTOCOL_TCP);
            } else {
                join_game_error = true;
            }
        }
        igText(" ");
        if (join_game_error) {
            igText("Error:");
            igText("Your name must be made with 8 characters in the range [a-zA-Z0-9].");
        }
        igText(" ");
        
        igEndPopup();
    }
}

static void draw_server_down_popup() {
    bool quit = false;
    
    while (!quit) {
        gl_gui_start_render(&quit);
        
        igOpenPopup("###Down", 0);
        
        if (igBeginPopupModal("Server Down###Down", 0, ImGuiWindowFlags_AlwaysAutoResize)) {
            igText(" ");
            igText("The server is down!");
            igText("The client cannot continue!");
            igText("To know more, contact the server's owners.");
            igText(" ");
            igText("Press any key to continue.");
            igText(" ");
            
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
