#include "types.h"
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include "utils.h"
#include "message.h"
#include "network.h"
#include "memory.h"
#include "print.h"

static const char g_help[] =
    "usage: " GHOSTLAB_EXECUTABLE_NAME " [options]\n"
    "\n"
    "ghostlab is an online matchmaking based game where you take upon yourself to become the best ghost hunter!\n"
    "\n"
    "options:\n"
    "\t-p, --port <server port>                   defines the port to use (" GHOSTLAB_DEFAULT_SERVER_PORT " by default).\n"
    "\t-I, --multi-ip <server multicast ip>       defines the multicast ip to use (" GHOSTLAB_DEFAULT_MULTICAST_IP " by default).\n"
    "\t-P, --multi-port <server multicast port>   defines the multicast port to use (" GHOSTLAB_DEFAULT_MULTICAST_PORT " by default).\n"
    "\t-h, --help                                 displays this help message and terminates.\n"
    "\t-v, --version                              displays the program's version and terminates.\n";

void *main_player_thread(void *arg) {
    int server_fd = gl_socket_create(GHOSTLAB_DEFAULT_SERVER_IP, GHOSTLAB_DEFAULT_SERVER_PORT, GL_SOCKET_TYPE_SERVER);
    
    int client_fd = gl_socket_server_accept_client(server_fd);
    
    if (client_fd >= 0) {
    
    }
    
    gl_socket_close(server_fd);
}

void command_help() {
    gl_printf("commands:\n");
    gl_printf("\tq, quit, exit   terminates the server.\n");
    gl_printf("\th, help         displays this help message.\n");
    gl_printf("\tv, version      display the program's version.\n");
}

void command_version() {
    gl_printf("version: " GHOSTLAB_VERSION "\n");
}

void *main_tcp_thread(void *arg) {
    int server_fd = gl_socket_create(GHOSTLAB_DEFAULT_SERVER_IP, GHOSTLAB_DEFAULT_SERVER_PORT, GL_SOCKET_TYPE_SERVER);
    
    int client_fd = gl_socket_server_accept_client(server_fd);
    
    if (client_fd >= 0) {
        for (uint8_t i = 0; i < 39; i++) {
            gl_message_t msg = {0};
            gl_message_read(client_fd, &msg);
            gl_message_printf(&msg);
            gl_message_write(client_fd, &msg);
            gl_message_free(&msg);
        }
    }
    
    gl_socket_close(server_fd);
}

int main(int argc, char **argv) {
    errno = 0;
    int exit_code = EXIT_SUCCESS;
    
    char *server_port = 0;
    char *multicast_ip = 0;
    char *multicast_port = 0;
    
    static struct option opts[] =
        {
            { "port", required_argument, 0, 'p' },
            { "multi-ip", required_argument, 0, 'I' },
            { "multi-port", required_argument, 0, 'P' },
            { "help", no_argument, 0, 'h' },
            { "version", no_argument, 0, 'v' },
            {0, 0, 0, 0}
        };
    int used_unknown_opt = 0;
    int opt;
    while ((opt = getopt_long(argc, argv, "p:I:P:hv", opts, 0)) != -1) {
        switch (opt) {
        case 'p':
            // TODO: Check if port is valid, if invalid use default
            server_port = gl_strdup(optarg);
            break;
        case 'I':
            // TODO: Check if ip is valid, if invalid use default
            multicast_ip = gl_strdup(optarg);
            break;
        case 'P':
            // TODO: Check if port is valid, if invalid use default
            multicast_port = gl_strdup(optarg);
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
    
    if (!server_port) {
        server_port = gl_strdup(GHOSTLAB_DEFAULT_SERVER_PORT);
    }
    if (!multicast_ip) {
        multicast_ip = gl_strdup(GHOSTLAB_DEFAULT_MULTICAST_IP);
    }
    if (!multicast_port) {
        multicast_port = gl_strdup(GHOSTLAB_DEFAULT_MULTICAST_PORT);
    }
    
    pthread_t tcp_thread;
    pthread_create(&tcp_thread, 0, main_tcp_thread, 0);
    
    gl_printf("you can now enter commands.\n");
    command_help();
    
    bool quit = false;
    while (!quit) {
        char command[512] = { 0 };
        gl_gets(0, command);
    
        if (strcmp(command, "quit") == 0 || strcmp(command, "exit") == 0 || strcmp(command, "q") == 0) {
            quit = true;
        } else if (strcmp(command, "h") == 0 || strcmp(command, "help") == 0) {
            command_help();
        } else if (strcmp(command, "v") == 0 || strcmp(command, "version") == 0) {
            command_version();
        }
    }
    
    goto cleanup;
    
    error:
    exit_code = gl_error_get(errno);
    
    cleanup:
    pthread_join(tcp_thread, 0);
    
    gl_free(server_port);
    gl_free(multicast_ip);
    gl_free(multicast_port);
    
    gl_memory_check_for_leaks();
    
    return exit_code;
}
