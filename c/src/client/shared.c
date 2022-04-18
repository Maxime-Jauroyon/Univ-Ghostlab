#include <client/shared.h>
#include <pthread.h>
#include "common/maze.h"
#include "common/array.h"
#include "common/game.h"
#include "common/network.h"
#include "common/log.h"
#include "common/message.h"

static pthread_mutex_t internal_g_gameplay_mutex = PTHREAD_MUTEX_INITIALIZER;

const char g_help[] =
    "usage: " GHOSTLAB_EXECUTABLE_NAME " [options]\n"
    "\n"
    "ghostlab is an online matchmaking based game where you take upon yourself to become the best ghost hunter!\n"
    "\n"
    "options:\n"
    "\t-i, --ip <server ip>             defines the ip to connect to (" GHOSTLAB_DEFAULT_SERVER_IP " by default).\n"
    "\t-p, --port <server port>         defines the port to connect to (" GHOSTLAB_DEFAULT_SERVER_PORT " by default).\n"
    "\t-n, --name <player name>         defines the name to use when connected to a match (will be asked later if not provided).\n"
    "\t-u, --udp-port,  <client port>   defines the udp port to use to communicate with other players (" GHOSTLAB_DEFAULT_UDP_PORT " used by default).\n"
    "\t-l, --legacy-protocol            sets the protocol version to legacy (the program will not run any extensions).\n"
    "\t-h, --help                       displays this help message and exits.\n"
    "\t-v, --version                    displays the program's version and exits.\n";
bool g_quit = false;
bool g_server_down = false;
bool g_legacy_protocol = false;
char *g_server_ip = 0;
char *g_server_port = 0;
char *g_player_id = 0;
char *g_udp_port = 0;
int32_t g_server_tcp_socket = -1;
char *g_multicast_ip = 0;
char *g_multicast_port = 0;
int32_t g_multicast_server_socket = -1;
void *g_multicast_server_thread = 0;
int32_t g_udp_socket = -1;
void *g_udp_thread = 0;
void *g_gameplay_mutex = &internal_g_gameplay_mutex;
struct gl_game_t *g_games = 0;
int32_t g_current_game_id = -1;

void gl_connect_to_server() {
    g_server_tcp_socket = gl_socket_create(g_server_ip, g_server_port, GL_SOCKET_TYPE_TCP_CLIENT, 0);
    gl_log_push("connection to server established.");
    if (!g_legacy_protocol) {
        gl_message_wait_and_execute_no_lock(g_server_tcp_socket, GL_MESSAGE_PROTOCOL_TCP);
    }
    gl_message_wait_and_execute_no_lock(g_server_tcp_socket, GL_MESSAGE_PROTOCOL_TCP);
}

void gl_free_games() {
    for (uint32_t i = 0; i < gl_array_get_size(g_games); i++) {
        gl_maze_free(g_games[i].maze);
        gl_array_free(g_games[i].ghosts);
        gl_array_free(g_games[i].players);
    }
    gl_array_free(g_games);
}