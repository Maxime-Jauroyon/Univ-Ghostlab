#include "message.h"
#include "common/message.h"
#include "common/log.h"

void message_iquit(gl_message_t *msg, int32_t socket_id, void *user_data) {
    gl_log_push("hello world!\n");
}

void gl_message_add_functions() {
    gl_message_definitions()[GL_MESSAGE_TYPE_IQUIT]->function = message_iquit;
}