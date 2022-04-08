#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/errno.h>
#include "array.h"
#include "message.h"

#ifdef __APPLE__
#include <sys/syslimits.h>
#else
#include <limits.h>
#include <endian.h>
#endif

#define gl_is_tcp_eol(c) ((c) == '*')
#define gl_is_udp_eol(c) ((c) == '+')
#define gl_is_eol(c) (gl_is_tcp_eol(c) || gl_is_udp_eol(c))
#define gl_is_separator(c) ((c) == ' ' || gl_is_eol(c))

static uint8_t g_max_message_identifier_size = 0;
static uint16_t g_max_message_parameter_value_size = 0;

static uint8_t calculate_max_message_identifier_size() {
    if (g_max_message_identifier_size != 0) {
        return g_max_message_identifier_size;
    }
    
    uint8_t max = 0;
    
    for (uint32_t i = 0; i < GL_MESSAGE_TYPE_COUNT; i++) {
        uint8_t name_size = strlen(gl_message_definitions()[i]->identifier);
        
        if (name_size > max) {
            max = name_size;
        }
    }
    
    g_max_message_identifier_size = max;
    
    return g_max_message_identifier_size;
}

static uint16_t calculate_max_message_parameter_value_size() {
    if (g_max_message_parameter_value_size != 0) {
        return g_max_message_parameter_value_size;
    }
    
    uint16_t max = 0;
    
    for (uint32_t i = 0; i < GL_MESSAGE_PARAMETER_TYPE_COUNT; i++) {
        uint16_t max_length = gl_message_parameter_definitions()[i]->length;
        
        if (max_length > max) {
            max = max_length;
        }
    }
    
    g_max_message_parameter_value_size = max;
    
    return g_max_message_parameter_value_size;
}

int gl_get_error() {
    if (errno != 0) {
        perror(GHOSTLAB_EXECUTABLE_NAME);
    }
    
    return EXIT_FAILURE;
}

int gl_write_uint8(gl_buffer_t *buf, const uint8_t *n) {
    buf->data = realloc(buf->data, buf->length + sizeof(uint8_t));
    gl_assert(buf->data);
    gl_assert(memcpy(buf->data + buf->length, n, sizeof(uint8_t)) != NULL);
    buf->length += sizeof(uint8_t);
    
    return 0;
}

int gl_read_uint8(int fd, uint8_t *n) {
    return (int)read(fd, n, sizeof(uint8_t));
}

int gl_read_uint16(int fd, uint16_t *n) {
    uint16_t be_n;
    gl_assert(read(fd, &be_n, sizeof(uint16_t)) != -1);
    *n = ntohs(be_n);
    
    return 0;
}

int gl_read_string(int fd, uint8_t **dst, uint16_t *dst_i, uint8_t *last_c, uint16_t max_size, bool precise_size) {
    uint8_t c = 0;
    uint16_t i = 0;
    
    while (!gl_is_separator(c)) {
        if (c != 0) {
            (*dst)[(*dst_i)++] = c;
            gl_assert(i++ <= max_size);
        }
    
        gl_assert(gl_read_uint8(fd, &c) != 0);
    }
    
    gl_assert(!precise_size || i == max_size);
    
    *last_c = c;
    
    return i;
}

int gl_read_message(int fd, struct gl_message_t *dst) {
    uint16_t total_length = 0;
    
    uint8_t last_c = 0;
    uint8_t identifier_buf[calculate_max_message_identifier_size() + 1];
    uint16_t identifier_length = 0;
    
    // Reads the message type name.
    gl_assert(gl_read_string(fd, (uint8_t **)&identifier_buf, &identifier_length, &last_c, calculate_max_message_identifier_size(), false) != 0);
    identifier_buf[identifier_length] = 0;
    total_length += identifier_length;
    
    // Finds the message type.
    const gl_message_definition_t *msg_def;
    for (uint32_t i = 0; i < GL_MESSAGE_TYPE_COUNT; i++) {
        msg_def = gl_message_definitions()[i];
        if (strcmp((const char *)identifier_buf, msg_def->identifier) == 0) {
            dst->type = i;
            break;
        }
    }
    
    gl_assert(msg_def);
    
    // Reads the parameters.
    for (uint32_t i = 0; i < msg_def->num_parameters; i++) {
        gl_assert(!gl_is_eol(last_c));
    
        const gl_message_parameter_definition_t  *msg_param_def = gl_message_parameter_definitions()[msg_def->parameters[i]];
        uint8_t parameter_value_buf[calculate_max_message_parameter_value_size()];
        uint16_t parameter_value_length = 0;
    
        gl_assert(gl_read_string(fd, (uint8_t **)&parameter_value_buf, &parameter_value_length, &last_c, msg_param_def->length, msg_param_def->precise_length) != 0);
        total_length += parameter_value_length;
        
        gl_array_push(dst->parameters_data, gl_array_create_from_carray(parameter_value_buf, parameter_value_length));
    }
    
    if (msg_def->protocol != GL_MESSAGE_PROTOCOL_TYPE_BOTH) {
        gl_assert(msg_def->protocol != GL_MESSAGE_PROTOCOL_TYPE_TCP || gl_is_tcp_eol(last_c));
        gl_assert(msg_def->protocol != GL_MESSAGE_PROTOCOL_TYPE_UDP || gl_is_udp_eol(last_c));
    }
    
    // Read ending characters
    gl_assert(gl_read_uint8(fd, 0) != 0);
    gl_assert(gl_read_uint8(fd, 0) != 0);
    
    return total_length;
}
