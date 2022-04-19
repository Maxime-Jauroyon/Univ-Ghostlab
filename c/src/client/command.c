#include <client/command.h>
#include <common/log.h>
#include <common/command.h>
#include <client/shared.h>

static void command_help(void *user_data) {
    gl_log_push("commands:\n");
    gl_log_push("\tq, e, quit, exit   terminates the program.\n");
    gl_log_push("\th, help            displays this help message.\n");
    gl_log_push("\tv, version         display the program's version.\n");
}

static const gl_command_definition_t g_command_help = {
    .function = command_help,
    .aliases = {
        "help",
        "h",
        0
    }
};

static void command_version(void *user_data) {
    gl_log_push("version: " GHOSTLAB_VERSION "\n");
}

static const gl_command_definition_t g_command_version = {
    .function = command_version,
    .aliases = {
        "version",
        "v",
        0
    }
};

static void command_quit(void *user_data) {
    g_should_quit = true;
}

static const gl_command_definition_t g_command_quit = {
    .function = command_quit,
    .aliases = {
        "quit",
        "exit",
        "q",
        "e",
        0
    }
};

static const gl_command_definition_t *gl_command_definitions_array[] = {
    [GL_COMMAND_TYPE_HELP] = &g_command_help,
    [GL_COMMAND_TYPE_VERSION] = &g_command_version,
    [GL_COMMAND_TYPE_QUIT] = &g_command_quit,
    
    [GL_COMMAND_TYPE_COUNT] = 0
};

const gl_command_definition_t **gl_client_command_definitions() {
    return gl_command_definitions_array;
}
