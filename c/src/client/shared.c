#include <client/shared.h>

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
    "\t-h, --help                       displays this help message and exits.\n"
    "\t-v, --version                    displays the program's version and exits.\n";
bool g_quit = false;
bool g_server_down = false;
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
