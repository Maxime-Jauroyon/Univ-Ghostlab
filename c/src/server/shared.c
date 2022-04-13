#include "shared.h"

const char g_help[] =
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
bool g_quit = false;
int32_t g_server_socket = 0;
int32_t *g_client_sockets = 0;
void *g_thread_tcp = 0;
void **g_threads_client_listener = 0;