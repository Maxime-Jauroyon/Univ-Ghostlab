#include <common/command.h>
#include <string.h>

static uint8_t g_max_name_size = 0;

uint32_t gl_command_get_num_aliases(const gl_command_definition_t *cmd_def) {
    uint32_t i;
    for (i = 0; cmd_def->aliases[i] != 0; i++) { }
    return i;
}

uint8_t gl_command_get_max_name_size(const gl_command_definition_t **cmd_defs) {
    if (g_max_name_size != 0) {
        return g_max_name_size;
    }
    
    uint8_t max = 0;
    
    for (uint32_t i = 0; cmd_defs[i] != 0; i++) {
        for (uint32_t j = 0; j < gl_command_get_num_aliases(cmd_defs[i]); j++) {
            uint32_t name_size = strlen(cmd_defs[i]->aliases[j]);
            
            if (name_size > max) {
                max = name_size;
            }
        }
    }
    
    g_max_name_size = max;
    
    return g_max_name_size;
}
