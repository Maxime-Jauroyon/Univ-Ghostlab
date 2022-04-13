#include <common/types.h>
#include <errno.h>
#include <stdlib.h>
#include <getopt.h>
#include <cimgui/cimgui.h>
#include <common/log.h>
#include <common/gui.h>
#include <common/utils.h>
#include <common/memory.h>
#include <common/network.h>
#include <common/command.h>
#include <client/shared.h>
#include <client/command.h>
#include "common/message.h"
#include "common/string.h"
#include "message.h"

static void draw_main_gui();

static int exit_code = EXIT_SUCCESS;
static char *server_ip = 0;
static char *server_port = 0;
static char *player_name = 0;
static char *udp_port = 0;
static bool g_show_console = true;

int main(int argc, char **argv) {
    errno = 0;
    
    int32_t server_fd = -1;
    
    static struct option opts[] = {
        { "ip", required_argument, 0, 'i' },
        { "port", required_argument, 0, 'p' },
        { "name", required_argument, 0, 'n' },
        { "udp-port", required_argument, 0, 'u' },
        { "help", no_argument, 0, 'h' },
        { "version", no_argument, 0, 'v' },
        {0, 0, 0, 0}
    };
    int32_t used_unknown_opt = 0;
    int32_t opt;
    while ((opt = getopt_long(argc, argv, "i:p:n:u:hv", opts, 0)) != -1) {
        switch (opt) {
        case 'i':
            // TODO: Check if ip is valid, if invalid use default
            server_ip = gl_strdup(optarg);
            break;
        case 'p':
            // TODO: Check if port is valid, if invalid use default
            server_port = gl_strdup(optarg);
            break;
        case 'n':
            // TODO: Check if name is valid, if invalid use default
            player_name = gl_strdup(optarg);
            break;
        case 'u':
            // TODO: Check if port is valid, if invalid use default
            udp_port = gl_strdup(optarg);
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
    
    if (!server_ip) {
        server_ip = gl_strdup(GHOSTLAB_DEFAULT_SERVER_IP);
    }
    if (!server_port) {
        server_port = gl_strdup(GHOSTLAB_DEFAULT_SERVER_PORT);
    }
    if (!udp_port) {
        udp_port = gl_strdup(GHOSTLAB_DEFAULT_UDP_PORT);
    }
    
    gl_message_add_functions();
    
    server_fd = gl_socket_create(GHOSTLAB_DEFAULT_SERVER_IP, GHOSTLAB_DEFAULT_SERVER_PORT, GL_SOCKET_TYPE_CLIENT);
    
    if (server_fd == -1) {
        gl_log_push("server not connected!\n");
        goto error;
    }
    
    gl_message_wait_and_execute(server_fd);
    
    gl_gui_create("Ghostlab Client");
    
    while (!g_quit) {
        gl_gui_start_render(&g_quit);
        draw_main_gui();
        gl_gui_end_render();
    }
    
    goto cleanup;
    
    error:
    exit_code = gl_error_get(errno);
    
    cleanup:
    if (server_fd != -1) {
        gl_socket_close(server_fd);
    }
    
    gl_free(server_ip);
    gl_free(server_port);
    gl_free(player_name);
    gl_free(udp_port);
    
    gl_logs_free();
    
    gl_memory_check_for_leaks();
    
    gl_gui_free();
    
    return exit_code;
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
