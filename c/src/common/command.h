#ifndef GHOSTLAB_COMMAND_H
#define GHOSTLAB_COMMAND_H

#include <common/types.h>

// Defines a command (with its names and function to execute when called).
typedef struct gl_command_definition_t {
    void (*function)(void *);
    const char *aliases[];
} gl_command_definition_t;

// Returns the number of aliases of a command.
uint32_t gl_command_get_num_aliases(const gl_command_definition_t *cmd_def);

// Executes a command by searching for it in a list of definitions and callinh its function if it's not null.
void gl_execute_command(char *cmd, uint32_t buf_size, const gl_command_definition_t **cmd_defs, uint32_t cmd_defs_count, bool consume);

#endif /* GHOSTLAB_COMMAND_H */
