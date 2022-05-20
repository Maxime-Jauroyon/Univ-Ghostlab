#include <common/types.h>
#include <errno.h>
#include <stdlib.h>
#include <getopt.h>
#include <pthread.h>
#include <common/log.h>
#include <common/ui.h>
#include <common/game.h>
#include <common/maze.h>
#include <common/array.h>
#include <common/utils.h>
#include <common/memory.h>
#include <common/message.h>
#include <server/ui.h>
#include <server/shared.h>
#include <server/message.h>
#include <server/thread_tcp_acceptor.h>
#include <server/thread_ghosts_handler.h>

static int32_t g_exit_code = EXIT_SUCCESS;

static int32_t gl_server_init(int argc, char **argv);
static int32_t gl_server_handle_args(int argc, char **argv);
static void gl_server_free();

int main(int argc, char **argv) {
    int32_t r = gl_server_init(argc, argv);
    if (r == -1) {
        goto error;
    } else if (r == 1) {
        goto cleanup;
    }
    
    gl_server_draw();
    
    goto cleanup;
    
    error:
    g_exit_code = 1;
    
    cleanup:
    gl_server_free();
    
    return g_exit_code;
}

static int32_t gl_server_init(int argc, char **argv) {
    errno = 0;
    
    int32_t r = gl_server_handle_args(argc, argv);
    if (r != 0) {
        return r;
    }
    
    if (!g_server_ip) {
        g_server_ip = gl_strdup(GHOSTLAB_DEFAULT_SERVER_IP);
    }
    if (!g_server_port) {
        g_server_port = gl_strdup(GHOSTLAB_DEFAULT_SERVER_PORT);
    }
    if (!g_multicast_ip) {
        g_multicast_ip = gl_strdup(GHOSTLAB_DEFAULT_MULTICAST_IP);
    }
    if (!g_multicast_port) {
        g_multicast_port = gl_strdup(GHOSTLAB_DEFAULT_MULTICAST_PORT);
    }
    
    gl_message_set_mutex(g_main_mutex);
    gl_server_message_add_functions();
    
    gl_ui_create("Ghostlab Server");
    
    g_tcp_acceptor_thread = gl_malloc(sizeof(pthread_t));
    pthread_create(g_tcp_acceptor_thread, 0, gl_thread_tcp_acceptor_main, 0);
    
    g_ghost_handler_thread = gl_malloc(sizeof(pthread_t));
    pthread_create(g_ghost_handler_thread, 0, gl_thread_ghosts_handler_main, 0);
    
    return 0;
}

static int32_t gl_server_handle_args(int argc, char **argv) {
    opterr = 0;
    
    static struct option opts[] = {
        { "ip", required_argument, 0, 'i' },
        { "port", required_argument, 0, 'p' },
        { "multi-ip", required_argument, 0, 'I' },
        { "multi-port", required_argument, 0, 'P' },
        { "legacy-protocol", required_argument, 0, 'l' },
        { "help", no_argument, 0, 'h' },
        { "version", no_argument, 0, 'v' },
        {0, 0, 0, 0}
    };
    int32_t used_unknown_or_undefined_opt = 0;
    int32_t opt;
    while ((opt = getopt_long(argc, argv, "i:p:I:P:lhv", opts, 0)) != -1) {
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
        case 'I':
            if (gl_is_ip_valid(optarg)) {
                g_multicast_ip = gl_strdup(optarg);
            } else {
                gl_log_push_error("invalid multicast ip, reverting to default.\n");
            }
            break;
        case 'P':
            if (gl_is_number_valid(optarg, 4)) {
                g_multicast_port = gl_strdup(optarg);
            } else {
                gl_log_push_error("invalid multicast port, reverting to default.\n");
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

static void gl_server_free() {
    if (!g_use_legacy_protocol) {
        gl_message_t response = { .type = GL_MESSAGE_TYPE_SHUTD,  0 };
        gl_message_send_multicast(g_multicast_ip, g_multicast_port, &response);
    }
    
    gl_server_free_listeners();
    
    if (g_tcp_acceptor_thread) {
        pthread_join(*(pthread_t *)g_tcp_acceptor_thread, 0);
        gl_free(g_tcp_acceptor_thread);
    }
    
    pthread_mutex_lock(g_main_mutex);
    gl_game_free_all(g_games);
    gl_array_free(g_games);
    pthread_mutex_unlock(g_main_mutex);
    
    if (g_ghost_handler_thread) {
        pthread_cancel(*(pthread_t *)g_ghost_handler_thread);
        pthread_join(*(pthread_t *)g_ghost_handler_thread, 0);
        gl_free(g_ghost_handler_thread);
    }
    
    gl_array_free(g_ip_sockets);
    gl_free(g_server_ip);
    gl_free(g_server_port);
    gl_free(g_multicast_ip);
    gl_free(g_multicast_port);
    
    gl_log_free();
    
    gl_memory_check_for_leaks();
    gl_ui_free();
}
