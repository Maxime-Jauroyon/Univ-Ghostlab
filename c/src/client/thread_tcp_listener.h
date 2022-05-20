#ifndef GHOSTLAB_SERVER_TCP_LISTENER_H
#define GHOSTLAB_SERVER_TCP_LISTENER_H

#include <common/types.h>

// Main function of the thread.
void *gl_client_thread_tcp_listener_main(void *user_data);

#endif /* GHOSTLAB_SERVER_TCP_LISTENER_H */
