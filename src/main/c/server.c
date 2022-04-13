#include "types.h"
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include "utils.h"
#include "network.h"
#include "memory.h"
#include "print.h"
#include "server_common.h"
#include "server_command.h"
#include "command.h"
#include "server_thread_clients_acceptor.h"

static int32_t g_exit_code = EXIT_SUCCESS;
static char *g_server_port = 0;
static char *g_multicast_ip = 0;
static char *g_multicast_port = 0;

int main(int argc, char **argv) {
    errno = 0;
    
    static struct option opts[] = {
        { "port", required_argument, 0, 'p' },
        { "multi-ip", required_argument, 0, 'I' },
        { "multi-port", required_argument, 0, 'P' },
        { "help", no_argument, 0, 'h' },
        { "version", no_argument, 0, 'v' },
        {0, 0, 0, 0}
    };
    int32_t used_unknown_opt = 0;
    int32_t opt;
    while ((opt = getopt_long(argc, argv, "p:I:P:hv", opts, 0)) != -1) {
        switch (opt) {
        case 'p':
            // TODO: Check if port is valid, if invalid use default
            g_server_port = gl_strdup(optarg);
            break;
        case 'I':
            // TODO: Check if ip is valid, if invalid use default
            g_multicast_ip = gl_strdup(optarg);
            break;
        case 'P':
            // TODO: Check if port is valid, if invalid use default
            g_multicast_port = gl_strdup(optarg);
            break;
        case 'h':
            gl_printf_no_indicator("%s", g_help);
            goto cleanup;
        case 'v':
            gl_printf_no_indicator("version: " GHOSTLAB_VERSION);
            goto cleanup;
        case '?':
            used_unknown_opt = 1;
            break;
        default:
            gl_printf_warning("option not yet implemented `%c`!\n", opt);
        }
    }
    
    if (used_unknown_opt) {
        gl_printf_warning("use `-h` for more informations.\n");
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
    
    g_thread_client_acceptors = gl_malloc(sizeof(pthread_t));
    pthread_create(g_thread_client_acceptors, 0, gl_thread_clients_acceptor_main, 0);
    
    gl_printf("you can now enter commands.\n");
    gl_command_definitions()[GL_COMMAND_TYPE_HELP]->function(0);
    
    while (!g_quit) {
        char command[gl_command_get_max_name_size(gl_command_definitions())];
        gl_gets(0, command);
    
        uint32_t found_pos = GL_COMMAND_TYPE_COUNT;
        
        for (uint32_t i = 0; i < GL_COMMAND_TYPE_COUNT; i++) {
            const gl_command_definition_t *cmd_def = gl_command_definitions()[i];
    
            for (uint32_t j = 0; j < gl_command_get_num_aliases(cmd_def); j++) {
                if (strcmp(cmd_def->aliases[j], command) == 0) {
                    found_pos = i;
                }
            }
            
            if (found_pos != GL_COMMAND_TYPE_COUNT) {
                break;
            }
        }
    
        if (found_pos != GL_COMMAND_TYPE_COUNT) {
            gl_command_definitions()[found_pos]->function(0);
        } else {
            gl_printf_warning("invalid option -- %s\n", command);
            gl_printf_warning("use `h` for more informations.\n");
        }
    }
    
    goto cleanup;
    
    error:
    g_exit_code = gl_error_get(errno);
    
    cleanup:
    gl_socket_close(g_server_socket);
    
    if (g_thread_client_acceptors) {
        pthread_join(g_thread_client_acceptors, 0);
        gl_free(g_thread_client_acceptors);
    }
    
    gl_free(g_server_port);
    gl_free(g_multicast_ip);
    gl_free(g_multicast_port);
    
    gl_memory_check_for_leaks();
    
    return g_exit_code;
}
