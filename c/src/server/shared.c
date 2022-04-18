#include <pthread.h>
#include "shared.h"

static pthread_mutex_t internal_g_gameplay_mutex = PTHREAD_MUTEX_INITIALIZER;

const char g_help[] =
    "usage: " GHOSTLAB_EXECUTABLE_NAME " [options]\n"
    "\n"
    "ghostlab is an online matchmaking based game where you take upon yourself to become the best ghost hunter!\n"
    "\n"
    "options:\n"
    "\t-i, --ip <server ip>                       defines the ip to use (" GHOSTLAB_DEFAULT_SERVER_IP " by default).\n"
    "\t-p, --port <server port>                   defines the port to use (" GHOSTLAB_DEFAULT_SERVER_PORT " by default).\n"
    "\t-I, --multi-ip <server multicast ip>       defines the multicast ip to use (" GHOSTLAB_DEFAULT_MULTICAST_IP " by default).\n"
    "\t-P, --multi-port <server multicast port>   defines the multicast port to use (" GHOSTLAB_DEFAULT_MULTICAST_PORT " by default).\n"
    "\t-l, --legacy-protocol                      sets the protocol version to legacy (the program will not run any extensions).\n"
    "\t-h, --help                                 displays this help message and terminates.\n"
    "\t-v, --version                              displays the program's version and terminates.\n";

bool g_quit = false;
bool g_legacy_protocol = false;

int32_t g_server_socket = -1;
int32_t *g_client_sockets = 0;
void *g_thread_tcp = 0;
void **g_threads_client_listener = 0;

char *g_server_ip = 0;
char *g_server_port = 0;
char *g_multicast_ip = 0;
char *g_multicast_port = 0;

void *g_gameplay_mutex = &internal_g_gameplay_mutex;
struct gl_game_t *g_games = 0;
