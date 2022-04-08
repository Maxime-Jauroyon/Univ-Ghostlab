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
#include <libkern/OSByteOrder.h>
#define htobe16(x) OSSwapHostToBigInt16(x)
#define htobe32(x) OSSwapHostToBigInt32(x)
#define htobe64(x) OSSwapHostToBigInt64(x)
#define htole16(x) OSSwapHostToLittleInt16(x)
#define htole32(x) OSSwapHostToLittleInt32(x)
#define htole64(x) OSSwapHostToLittleInt64(x)
#define be16toh(x) OSSwapBigToHostInt16(x)
#define be32toh(x) OSSwapBigToHostInt32(x)
#define be64toh(x) OSSwapBigToHostInt64(x)
#define le16toh(x) OSSwapLittleToHostInt16(x)
#define le32toh(x) OSSwapLittleToHostInt32(x)
#define le64toh(x) OSSwapLittleToHostInt64(x)
#else
#include <limits.h>
#include <endian.h>
#endif

#define gl_is_tcp_eol(c) ((c) == '*')
#define gl_is_udp_eol(c) ((c) == '+')
#define gl_is_eol(c) (gl_is_tcp_eol(c) || gl_is_udp_eol(c))
#define gl_is_separator(c) ((c) == ' ' || gl_is_eol(c))

static uint8_t g_max_message_identifier_size = 0;

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

static uint16_t uint8_to_uint16(const uint8_t *n) {
    uint16_t r = 0;
    
    r |= (uint16_t)n[0] << 8;
    r |= (uint16_t)n[1] << 0;
    
    return r;
}

static uint32_t uint8_to_uint32(const uint8_t *n) {
    uint32_t r = 0;
    
    r |= (uint32_t)n[0] << 16;
    r |= (uint32_t)n[1] << 8;
    r |= (uint32_t)n[2] << 0;
    
    return r;
}

static uint64_t uint8_to_uint64(const uint8_t *n) {
    uint64_t r = 0;
    
    r |= (uint64_t)n[0] << 32;
    r |= (uint64_t)n[1] << 16;
    r |= (uint64_t)n[2] << 8;
    r |= (uint64_t)n[3] << 0;
    
    return r;
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

int gl_read_string(int fd, uint8_t **dst, uint8_t *last_c, uint16_t max_size, bool precise_size) {
    uint8_t c = 0;
    
    while (!gl_is_separator(c)) {
        if (c != 0) {
            gl_array_push(*dst, c);
            gl_assert(gl_array_get_header(*dst)->size <= max_size);
        }
    
        gl_assert(gl_read_uint8(fd, &c) != 0);
    }
    
    gl_assert(!precise_size || gl_array_get_header(*dst)->size == max_size);
    
    *last_c = c;
    
    return gl_array_get_header(*dst)->size;
}

int gl_read_message(int fd, struct gl_message_t *dst) {
    uint16_t total_size = 0;
    
    uint8_t last_c = 0;
    uint8_t *identifier_buf = 0;
    
    // Reads the message type name.
    gl_assert(gl_read_string(fd, &identifier_buf, &last_c, calculate_max_message_identifier_size(), false) != 0);
    gl_array_push(identifier_buf, 0);
    total_size += gl_array_get_header(identifier_buf)->size;
    
    // Finds the message type.
    const gl_message_definition_t *msg_def;
    for (uint32_t i = 0; i < GL_MESSAGE_TYPE_COUNT; i++) {
        msg_def = gl_message_definitions()[i];
        if (strcmp((const char *)identifier_buf, msg_def->identifier) == 0) {
            dst->type = i;
            break;
        }
    }
    
    gl_array_free(identifier_buf);
    gl_assert(msg_def);
    
    // Reads the parameters.
    for (uint32_t i = 0; i < msg_def->num_parameters; i++) {
        gl_assert(!gl_is_eol(last_c));
    
        const gl_message_parameter_definition_t  *msg_param_def = gl_message_parameter_definitions()[msg_def->parameters[i]];
        uint8_t *parameter_value_buf = 0;
    
        gl_assert(gl_read_string(fd, &parameter_value_buf, &last_c, msg_param_def->length, msg_param_def->precise_length) != 0);
        total_size += gl_array_get_header(parameter_value_buf)->size + 1;
    
        gl_message_parameter_t parameter;
    
        if (msg_param_def->value_type == GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT8) {
            parameter = (gl_message_parameter_t) { .uint8_value = parameter_value_buf[0] };
        } else if (msg_param_def->value_type == GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT16) {
            uint16_t n = uint8_to_uint16(parameter_value_buf);
            if (msg_param_def->endian_conversion == GL_MESSAGE_PARAMETER_ENDIAN_CONVERSION_AS_LITTLE) {
                n = le16toh(n);
            } else if (msg_param_def->endian_conversion == GL_MESSAGE_PARAMETER_ENDIAN_CONVERSION_AS_BIG) { // NOLINT
                n = be16toh(n);
            } else {
                n = ntohs(n);
            }
            parameter = (gl_message_parameter_t) { .uint16_value = msg_param_def->has_max_value && n > (uint16_t )msg_param_def->max_value ? (uint16_t )msg_param_def->max_value : n };
        } else if (msg_param_def->value_type == GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT32) {
            uint32_t n = uint8_to_uint32(parameter_value_buf);
            if (msg_param_def->endian_conversion == GL_MESSAGE_PARAMETER_ENDIAN_CONVERSION_AS_LITTLE) {
                n = le32toh(n);
            } else if (msg_param_def->endian_conversion == GL_MESSAGE_PARAMETER_ENDIAN_CONVERSION_AS_BIG) { // NOLINT
                n = be32toh(n);
            } else {
                n = ntohl(n);
            }
            parameter = (gl_message_parameter_t) { .uint32_value = msg_param_def->has_max_value && n > (uint32_t )msg_param_def->max_value ? (uint32_t )msg_param_def->max_value : n };
        } else if (msg_param_def->value_type == GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT64) {
            uint64_t n = uint8_to_uint64(parameter_value_buf);
            if (msg_param_def->endian_conversion == GL_MESSAGE_PARAMETER_ENDIAN_CONVERSION_AS_LITTLE) {
                n = le64toh(n);
            } else if (msg_param_def->endian_conversion == GL_MESSAGE_PARAMETER_ENDIAN_CONVERSION_AS_BIG) { // NOLINT
                n = be64toh(n);
            } else {
                n = ntohll(n);
            }
            parameter = (gl_message_parameter_t) { .uint64_value = msg_param_def->has_max_value && n > (uint64_t )msg_param_def->max_value ? (uint64_t )msg_param_def->max_value : n };
        } else {
            parameter = (gl_message_parameter_t) { .string_value = parameter_value_buf };
        }
        
        gl_array_push(dst->parameters_value, parameter);
    }
    
    // Checks if the message was sent using the right protocol.
    if (msg_def->protocol != GL_MESSAGE_PROTOCOL_BOTH) {
        gl_assert(msg_def->protocol != GL_MESSAGE_PROTOCOL_TCP || gl_is_tcp_eol(last_c));
        gl_assert(msg_def->protocol != GL_MESSAGE_PROTOCOL_UDP || gl_is_udp_eol(last_c));
    }
    
    // Reads ending characters.
    uint8_t c;
    gl_assert(gl_read_uint8(fd, &c) != 0);
    gl_assert(gl_read_uint8(fd, &c) != 0);
    total_size += 2;
    
    return total_size;
}
