#ifndef GHOSTLAB_SERVER_THREAD_MULTICAST_H
#define GHOSTLAB_SERVER_THREAD_MULTICAST_H

#include <common/types.h>

struct gl_message_t;

void *gl_thread_multicast_main(void *user_data);

void gl_thread_multicast_wake_up();

void gl_thread_multicast_send(struct gl_message_t *msg);

#endif /* GHOSTLAB_SERVER_THREAD_MULTICAST_H */
