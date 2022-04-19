#ifndef GHOSTLAB_CLIENT_COMMAND_H
#define GHOSTLAB_CLIENT_COMMAND_H

#include <common/types.h>

struct gl_command_definition_t;

typedef enum gl_command_type_t {
    GL_COMMAND_TYPE_HELP,
    GL_COMMAND_TYPE_VERSION,
    GL_COMMAND_TYPE_QUIT,
    
    GL_COMMAND_TYPE_COUNT
} gl_command_type_t;

const struct gl_command_definition_t **gl_client_command_definitions();

#endif /* GHOSTLAB_CLIENT_COMMAND_H */
