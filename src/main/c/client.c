#include "types.h"
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <printf.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include "utils.h"
#include "array.h"
#include "message.h"

static const char g_help[] =
    "usage: " GHOSTLAB_EXECUTABLE_NAME " [options]\n"
    "\n"
    "ghostlab is an online matchmaking based game where you take upon yourself to become the best ghost hunter!\n"
    "\n"
    "options:\n"
    "\t-i, --ip <server ip>                 defines the ip to connect to (" GHOSTLAB_DEFAULT_SERVER_IP " by default).\n"
    "\t-p, --port <server tcp port>         defines the port to connect to (" GHOSTLAB_DEFAULT_SERVER_PORT " by default).\n"
    "\t-n, --name <player name>             defines the name to use when connected to a match (will be asked later if not provided).\n"
    "\t-u, --udp-port,  <client udp port>   defines the udp port to use to communicate with other players (" GHOSTLAB_DEFAULT_UDP_PORT " used by default).\n"
    "\t-h, --help                           displays this help message and exits.\n"
    "\t-v, --version                        displays the program's version and exits.\n";

int main(int argc, char **argv) {
    errno = 0;
    int exit_code = EXIT_SUCCESS;
    
    char *server_ip = 0;
    char *server_port = 0;
    char *player_name = 0;
    char *udp_port = 0;
    
    static struct option opts[] =
        {
            { "ip", required_argument, 0, 'i' },
            { "port", required_argument, 0, 'p' },
            { "name", required_argument, 0, 'n' },
            { "udp-port", required_argument, 0, 'u' },
            { "help", no_argument, 0, 'h' },
            { "version", no_argument, 0, 'v' },
            {0, 0, 0, 0}
        };
    int used_unknown_opt = 0;
    int opt;
    while ((opt = getopt_long(argc, argv, "i:p:n:u:hv", opts, 0)) != -1) {
        switch (opt) {
        case 'i':
            // TODO: Check if ip is valid
            server_ip = strdup(optarg);
            break;
        case 'p':
            // TODO: Check if port is valid
            server_port = strdup(optarg);
            break;
        case 'n':
            // TODO: Check if name is valid
            player_name = strdup(optarg);
            break;
        case 'u':
            // TODO: Check if port is valid
            udp_port = strdup(optarg);
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
    
    if (!server_ip) {
        server_ip = strdup(GHOSTLAB_DEFAULT_SERVER_IP);
    }
    if (!server_port) {
        server_ip = strdup(GHOSTLAB_DEFAULT_SERVER_PORT);
    }
    if (!udp_port) {
        udp_port = strdup(GHOSTLAB_DEFAULT_UDP_PORT);
    }
    
    struct sockaddr_in adress_sock;
    adress_sock.sin_family = AF_INET;
    adress_sock.sin_port = htons(4243);
    inet_aton("127.0.0.1",&adress_sock.sin_addr);
    int fd = socket(PF_INET, SOCK_STREAM, 0);
    int r = connect(fd, (struct sockaddr *)&adress_sock, sizeof(struct sockaddr_in));
    if(r != -1) {
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_GAMES, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .uint8_value = 8 });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_OGAMES, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .uint8_value = 4 });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .uint8_value = 7 });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_NEWPL, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("hugoboss") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("4242") });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_REGIS, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("hugoboss") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("4242") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .uint8_value =  5 });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_REGOK, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .uint8_value =  5 });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_REGNO, 0 };
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_START, 0 };
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_UNREG, 0 };
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_UNROK, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .uint8_value =  5 });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_DUNNO, 0 };
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_SIZE_REQ, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .uint8_value =  5 });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_SIZE_RES, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .uint8_value =  5 });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .uint16_value =  265 });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .uint16_value =  345 });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_LIST_REQ, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .uint8_value =  5 });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_LIST_RES, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .uint8_value =  5 });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .uint16_value =  12 });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_PLAYR, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("hugoboss") });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_GAME_REQ, 0 };
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_WELCO, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .uint8_value = 4, });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .uint16_value = 543 });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .uint16_value = 654 });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .uint8_value = 12 });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("127.0.0.1######") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("4242") });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_POSIT, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("hugoboss") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("043") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("053") });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_UPMOV, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("023") });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_DOMOV, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("023") });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_LEMOV, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("023") });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_RIMOV, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("023") });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_MOVE_RES, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("023") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("023") });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_MOVEF, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("023") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("023") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("0023") });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_IQUIT, 0 };
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_GOBYE, 0 };
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_GLIS_REQ, 0 };
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_GLIS_RES, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .uint8_value = 4, });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_GPLYR, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("hugoboss") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("043") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("053") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("0053") });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_MALL_REQ, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("02dfgd gdfg dsfgdfg dfsdg dfg dsgdfg 3") });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_MALL_RES, 0 };
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_SEND_REQ, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("hugoboss") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("02dfgd gdfg dsfgdfg dfsdg dfg dsgdfg 3") });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_SEND_RES, 0 };
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_NSEND, 0 };
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_GHOST, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("023") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("023") });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_SCORE, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("hugoboss") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("0056") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("023") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("023") });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_MESSA, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("hugoboss") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("02dfgd gdfg dsfgdfg dfsdg dfg dsgdfg 3") });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_ENDGA, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("hugoboss") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("0054") });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_MESSP, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("hugoboss") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("02dfgd gdfg dsfgdfg dfsdg dfg dsgdfg 3") });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        for (uint8_t i = 0; i < 39; i++) {
            {
                gl_message_t msg = {0};
                gl_read_message(fd, &msg);
                gl_printf_message(&msg);
                gl_free_message(&msg);
            }
        }
        
        close(fd);
    }
    
    goto cleanup;
    
    error:
    exit_code = gl_get_error();
    
    cleanup:
    if (server_ip) {
        free(server_ip);
    }
    if (server_port) {
        free(server_port);
    }
    if (player_name) {
        free(player_name);
    }
    if (udp_port) {
        free(udp_port);
    }
    
    return exit_code;
}
