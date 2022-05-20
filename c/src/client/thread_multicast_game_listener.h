#ifndef GHOSTLAB_CLIENT_THREAD_MULTICAST_GAME_LISTENER_H
#define GHOSTLAB_CLIENT_THREAD_MULTICAST_GAME_LISTENER_H

#include <common/types.h>

// Main function of the thread.
void *gl_client_thread_multicast_game_listener_main(void *user_data);

// Starts the thread.
void gl_client_thread_multicast_game_listener_start();

// Stops the thread.
void gl_client_thread_multicast_game_listener_close();

#endif /* GHOSTLAB_CLIENT_THREAD_MULTICAST_GAME_LISTENER_H */
