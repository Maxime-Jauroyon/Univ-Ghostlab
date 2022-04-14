#include <client/message.h>
#include "common/message.h"
#include "common/log.h"
#include "shared.h"

void message_games(gl_message_t *msg, int32_t socket_id, void *user_data) {
    gl_log_push("hello world: %d!\n", msg->parameters_value[0].uint8_value);
}

void message_shutd(gl_message_t *msg, int32_t socket_id, void *user_data) {
    g_server_down = true;
    g_quit = true;
}

void gl_message_add_functions() {
    gl_message_definitions()[GL_MESSAGE_TYPE_GAMES]->function = message_games;
    gl_message_definitions()[GL_MESSAGE_TYPE_SHUTD]->function = message_shutd;
}
