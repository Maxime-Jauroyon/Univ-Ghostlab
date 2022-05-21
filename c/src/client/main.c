#include <common/types.h>
#include <errno.h>
#include <getopt.h>
#include <stdlib.h>
#include <pthread.h>
#include <common/ui.h>
#include <common/log.h>
#include <common/game.h>
#include <common/array.h>
#include <common/utils.h>
#include <common/memory.h>
#include <common/message.h>
#include <common/network.h>
#include <client/ui.h>
#include <client/shared.h>
#include <client/message.h>
#include <client/thread_tcp_listener.h>
#include <client/thread_udp_listener.h>

static int32_t g_exit_code = EXIT_SUCCESS;

static int32_t gl_client_init(int argc, char **argv);
static int32_t gl_client_handle_args(int argc, char **argv);
static void gl_client_free();

int main(int argc, char **argv) {
    int32_t r = gl_client_init(argc, argv);
    if (r == -1) {
        goto error;
    } else if (r == 1) {
        goto cleanup;
    }
    
    gl_client_draw();
    
    goto cleanup;

    error:
    g_exit_code = 1;
    
    cleanup:
    gl_client_free();
    
    return g_exit_code;
}

static int32_t gl_client_init(int argc, char **argv) {
    errno = 0;
    
    int32_t r = gl_client_handle_args(argc, argv);
    if (r != 0) {
        return r;
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
    
    gl_message_set_mutex(g_main_mutex);
    gl_client_message_add_functions();
    
    gl_ui_create("Ghostlab Client");
    
    if (gl_client_connect() == -1) {
        gl_client_server_down_popup_draw();
        return -1;
    }
    
    g_tcp_listener_thread = gl_malloc(sizeof(pthread_t));
    pthread_create(g_tcp_listener_thread, 0, gl_client_thread_tcp_listener_main, 0);
    g_udp_listener_thread = gl_malloc(sizeof(pthread_t));
    pthread_create(g_udp_listener_thread, 0, gl_client_thread_udp_listener_main, 0);
    
    return 0;
}

static int32_t gl_client_handle_args(int argc, char **argv) {
    opterr = 0;
    
    struct option opts[] = {
        { "ip", required_argument, 0, 'i' },
        { "port", required_argument, 0, 'p' },
        { "udp-port", required_argument, 0, 'u' },
        { "legacy-protocol", required_argument, 0, 'l' },
        { "help", no_argument, 0, 'h' },
        { "version", no_argument, 0, 'v' },
        {0, 0, 0, 0}
    };
    int32_t used_unknown_or_undefined_opt = 0;
    int32_t opt;
    while ((opt = getopt_long(argc, argv, "i:p:n:u:lhv", opts, 0)) != -1) {
        switch (opt) {
        case 'i':
            if (gl_is_ip_valid(optarg)) {
                g_server_ip = gl_strdup(optarg);
            } else {
                gl_log_push_error("invalid server ip, reverting to default.\n");
            }
            break;
        case 'p':
            if (gl_is_number_valid(optarg, 4)) {
                g_server_port = gl_strdup(optarg);
            } else {
                gl_log_push_error("invalid server port, reverting to default.\n");
            }
            break;
        case 'u':
            if (gl_is_number_valid(optarg, 4)) {
                g_udp_port = gl_strdup(optarg);
            } else {
                gl_log_push_error("invalid udp port, reverting to default.\n");
            }
            break;
        case 'l':
            g_use_legacy_protocol = true;
            break;
        case 'h':
            gl_log_push_array(g_help, sizeof(g_help) / sizeof(g_help[0]));
            return 1;
        case 'v':
            gl_log_push("version: " GHOSTLAB_VERSION);
            return 1;
        case '?':
            used_unknown_or_undefined_opt = 1;
            gl_log_push_error("option unknown '%c'!\n", optopt);
            break;
        default:
            used_unknown_or_undefined_opt = 1;
            gl_log_push_error("option not yet implemented `%c`!\n", opt);
        }
    }
    
    if (used_unknown_or_undefined_opt) {
        gl_log_push_error("use `-h` for more informations.\n");
    }
    
    return 0;
}

static void gl_client_free() {
    if (g_udp_listener_thread) {
        gl_socket_close(&g_udp_socket);
        pthread_join(*(pthread_t *)g_udp_listener_thread, 0);
        gl_free(g_udp_listener_thread);
    }
    
    if (!g_use_legacy_protocol && g_multicast_general_listener_thread) {
        gl_socket_close(&g_multicast_general_socket);
        pthread_join(*(pthread_t *)g_multicast_general_listener_thread, 0);
        gl_free(g_multicast_general_listener_thread);
    }
    
    if (g_multicast_game_listener_thread) {
        gl_socket_close(&g_multicast_game_socket);
        pthread_join(*(pthread_t *)g_multicast_game_listener_thread, 0);
        gl_free(g_multicast_game_listener_thread);
    }
    
    if (g_tcp_listener_socket) {
        gl_client_disconnect(true, false);
    }
    
    if (g_tcp_listener_thread) {
        pthread_join(*(pthread_t *)g_tcp_listener_thread, 0);
        gl_free(g_tcp_listener_thread);
    }
    
    if (g_is_server_down) {
        gl_client_server_down_popup_draw();
    }
    
    gl_game_free_all(g_games);
    gl_array_free(g_games);
    gl_free(g_server_ip);
    gl_free(g_server_port);
    gl_free(g_udp_port);
    gl_free(g_multicast_ip);
    gl_free(g_multicast_port);
    gl_array_free(g_players_message_list);
    
    gl_log_free();
    
    gl_memory_check_for_leaks();
    gl_ui_free();
}
