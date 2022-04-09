#include "types.h"
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <printf.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>
#include <getopt.h>
#include <errno.h>
#include "utils.h"
#include "message.h"

static const char g_help[] =
    "usage: " GHOSTLAB_EXECUTABLE_NAME " [options]\n"
    "\n"
    "ghostlab is an online matchmaking based game where you take upon yourself to become the best ghost hunter!\n"
    "\n"
    "options:\n"
    "\t-p, --port <server port>                   defines the port to use (" GHOSTLAB_DEFAULT_SERVER_PORT " by default).\n"
    "\t-I, --multi-ip <server multicast ip>       defines the multicast ip to use (" GHOSTLAB_DEFAULT_MULTICAST_IP " by default).\n"
    "\t-P, --multi-port <server multicast port>   defines the multicast port to use (" GHOSTLAB_DEFAULT_MULTICAST_PORT " by default).\n"
    "\t-h, --help                                 displays this help message and exits.\n"
    "\t-v, --version                              displays the program's version and exits.\n";

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
            server_port = strdup(optarg);
            break;
        case 'I':
            // TODO: Check if ip is valid, if invalid use default
            multicast_ip = strdup(optarg);
            break;
        case 'P':
            // TODO: Check if port is valid, if invalid use default
            multicast_port = strdup(optarg);
            break;
        case 'h':
            printf("%s", g_help);
            goto cleanup;
        case 'v':
            printf("version: " GHOSTLAB_VERSION);
            goto cleanup;
        case '?':
            used_unknown_opt = 1;
            break;
        default:
            fprintf(stderr, "option not yet implemented `%c`!\n", opt);
        }
    }
    
    if (used_unknown_opt) {
        error("use `-h` for more informations.\n");
    }
    
    if (!server_port) {
        server_port = strdup(GHOSTLAB_DEFAULT_SERVER_PORT);
    }
    if (!multicast_ip) {
        multicast_ip = strdup(GHOSTLAB_DEFAULT_MULTICAST_IP);
    }
    if (!multicast_port) {
        multicast_port = strdup(GHOSTLAB_DEFAULT_MULTICAST_PORT);
    }
    
    int sock = socket(PF_INET,SOCK_STREAM,0);
    struct sockaddr_in address_sock;
    address_sock.sin_family = AF_INET;
    address_sock.sin_port = htons(4243);
    address_sock.sin_addr.s_addr = htonl(INADDR_ANY);
    
    int r = bind(sock, (struct sockaddr *)&address_sock, sizeof(struct sockaddr_in));
    if (!r) {
        r = listen(sock, 0);
        if (r < 0) {
            goto error;
        }
    
        struct sockaddr_in client;
        socklen_t size_client = sizeof(client);
        int fd = accept(sock, (struct sockaddr *)&client, &size_client);
        
        if (fd >= 0) {
            for (uint8_t i = 0; i < 39; i++) {
                {
                    gl_message_t msg = {0};
                    gl_read_message(fd, &msg);
                    gl_printf_message(&msg);
                    gl_write_message(fd, &msg);
                    gl_free_message(&msg);
                }
            }
        }
    
        close(fd);
    }
    
    goto cleanup;
    
    error:
    exit_code = gl_get_error();
    
    cleanup:
    if (server_port) {
        free(server_port);
    }
    if (multicast_ip) {
        free(multicast_ip);
    }
    if (multicast_port) {
        free(multicast_port);
    }
    
    return exit_code;
}
