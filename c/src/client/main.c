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
#include "common/message.h"
#include "message.h"
#include "thread_multicast_server.h"
#include "thread_udp.h"

static void draw_main_gui();
static void draw_server_down_popup();

static int32_t g_exit_code = EXIT_SUCCESS;
static bool g_show_console = true;

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

    gl_message_add_functions();
    
    gl_gui_create("Ghostlab Client");
    
    g_server_tcp_socket = gl_socket_create(g_server_ip, g_server_port, GL_SOCKET_TYPE_TCP_CLIENT, 0);
    
    if (g_server_tcp_socket == -1) {
        draw_server_down_popup();
        goto error;
    }
    
    if (!g_legacy_protocol) {
        // MULTI
        gl_message_wait_and_execute(g_server_tcp_socket, GL_MESSAGE_PROTOCOL_TCP);
        g_multicast_server_thread = gl_malloc(sizeof(pthread_t));
        pthread_create(g_multicast_server_thread, 0, gl_thread_multicast_server_main, 0);
    }
    
    // GAMES
    gl_message_wait_and_execute(g_server_tcp_socket, GL_MESSAGE_PROTOCOL_TCP);
    
    g_udp_thread = gl_malloc(sizeof(pthread_t));
    pthread_create(g_udp_thread, 0, gl_thread_udp_main, 0);
    
    while (!g_quit) {
        gl_gui_start_render(&g_quit);
        draw_main_gui();
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

    if (g_server_tcp_socket != -1) {
        gl_socket_close(&g_server_tcp_socket);
    }
    
    if (g_server_down) {
        draw_server_down_popup();
    }
    
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
            if (igBeginMenu("Options", true)) {
                igMenuItemBoolPtr("Show Logs ", 0, &g_show_console, true);
                igMenuItemBoolPtr("Quit", 0, &g_quit, true);
                igEndMenu();
            }
            igEndMenuBar();
        }

        if (igCollapsingHeaderTreeNodeFlags("Informations", 0)) {
            igText("TCP socket connected:");
            igText("- IP: 127.0.0.1");
            igText("- Port: 4750");
        }

        if (igCollapsingHeaderTreeNodeFlags("Statistics", 0)) {
            igText("Number of clients connected: 0");
            igText("Number of matches: 0");
        }

        igEnd();
    }

    if (g_show_console) {
        gl_igConsole(gl_command_definitions(), GL_COMMAND_TYPE_COUNT);
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
