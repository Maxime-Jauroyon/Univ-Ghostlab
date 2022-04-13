#ifndef GHOSTLAB_COMMAND_H
#define GHOSTLAB_COMMAND_H

#include "types.h"

typedef struct gl_command_definition_t {
    void (*function)(void *);
    const char *aliases[];
} gl_command_definition_t;

uint32_t gl_command_get_num_aliases(const gl_command_definition_t *cmd_def);

uint8_t gl_command_get_max_name_size(const gl_command_definition_t **cmd_defs);

#endif /* GHOSTLAB_COMMAND_H */
