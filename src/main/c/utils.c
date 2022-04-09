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

#define gl_tcp_terminator '*'
#define gl_udp_terminator '+'
#define gl_separator ' '
#define gl_is_tcp_terminator(c) ((c) == gl_tcp_terminator)
#define gl_is_udp_terminator(c) ((c) == gl_udp_terminator)
#define gl_is_terminator(c) (gl_is_tcp_terminator(c) || gl_is_udp_terminator(c))
#define gl_is_separator(c) ((c) == gl_separator)
#define gl_is_separator_or_terminator(c) (gl_is_separator(c) || gl_is_terminator(c))

static uint8_t g_max_message_identifier_size = 0;

static uint8_t gl_calculate_max_message_identifier_size() {
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

static uint16_t gl_uint8_to_uint16(const uint8_t *n) {
    uint16_t r =
        (uint16_t)n[1] << 8 |
        (uint16_t)n[0];
    
    return r;
}

static uint32_t gl_uint8_to_uint32(const uint8_t *n) {
    uint32_t r =
        (uint32_t)n[3] << 24 | // Je met le dernier uint8_t sur les bits 24 à 32 du uint32_t
        (uint32_t)n[2] << 16 | // Je met le 3ème uint8_t sur les bits 16 à 24 du uint32_t
        (uint32_t)n[1] << 8 | // Je met le 2ème uint8_t sur les bits 8 à 16 du uint32_t
        (uint32_t)n[0]; // Je met le premier uint8_t sur les bits 0 à 8 du uint32_t
    
    return r;
}

static uint64_t gl_uint8_to_uint64(const uint8_t *n) {
    uint64_t r =
        (uint64_t)n[7] << 56 |
        (uint64_t)n[6] << 48 |
        (uint64_t)n[5] << 40 |
        (uint64_t)n[4] << 32 |
        (uint64_t)n[3] << 24 |
        (uint64_t)n[2] << 16 |
        (uint64_t)n[1] << 8 |
        (uint64_t)n[0];
    
    return r;
}

int gl_get_error() {
    if (errno != 0) {
        perror(GHOSTLAB_EXECUTABLE_NAME);
    }
    
    return EXIT_FAILURE;
}

uint8_t *gl_string_from_cstring(const char *src) {
    uint8_t *dst = 0;
    
    for (uint64_t i = 0; i < strlen(src); i++) {
        gl_array_push(dst, (uint8_t)(src)[i]);
    }
    
    return dst;
}

static int gl_write_uint8(uint8_t **buf, const uint8_t *n) {
    gl_array_push(*buf, *n);
    
    return 1;
}

static int gl_write_uint16(uint8_t **buf, const uint16_t *n, gl_message_parameter_endian_conversion_t conversion_type) {
    uint16_t v;
    
    if (conversion_type == GL_MESSAGE_PARAMETER_ENDIAN_CONVERSION_AS_LITTLE) {
        v = htole16(*n);
    } else if (conversion_type == GL_MESSAGE_PARAMETER_ENDIAN_CONVERSION_AS_BIG) { // NOLINT
        v = htobe16(*n);
    } else {
        v = htons(*n);
    }
    
    uint8_t *vp = (uint8_t *)&v;
    gl_array_push(*buf, vp[0]);
    gl_array_push(*buf, vp[1]);
    
    return 2;
}

static int gl_write_uint32(uint8_t **buf, const uint32_t *n, gl_message_parameter_endian_conversion_t conversion_type) {
    uint32_t v;
    
    if (conversion_type == GL_MESSAGE_PARAMETER_ENDIAN_CONVERSION_AS_LITTLE) {
        v = htole32(*n);
    } else if (conversion_type == GL_MESSAGE_PARAMETER_ENDIAN_CONVERSION_AS_BIG) { // NOLINT
        v = htobe32(*n);
    } else {
        v = htonl(*n);
    }
    
    uint8_t *vp = (uint8_t *)&v;
    gl_array_push(*buf, vp[0]);
    gl_array_push(*buf, vp[1]);
    gl_array_push(*buf, vp[2]);
    gl_array_push(*buf, vp[3]);
    
    return 4;
}

static int gl_write_uint64(uint8_t **buf, const uint64_t *n, gl_message_parameter_endian_conversion_t conversion_type) {
    uint64_t v;
    
    if (conversion_type == GL_MESSAGE_PARAMETER_ENDIAN_CONVERSION_AS_LITTLE) {
        v = htole64(*n);
    } else if (conversion_type == GL_MESSAGE_PARAMETER_ENDIAN_CONVERSION_AS_BIG) { // NOLINT
        v = htobe64(*n);
    } else {
        v = htonll(*n);
    }
    
    uint8_t *vp = (uint8_t *)&v;
    gl_array_push(*buf, vp[0]);
    gl_array_push(*buf, vp[1]);
    gl_array_push(*buf, vp[2]);
    gl_array_push(*buf, vp[3]);
    gl_array_push(*buf, vp[4]);
    gl_array_push(*buf, vp[5]);
    gl_array_push(*buf, vp[6]);
    gl_array_push(*buf, vp[7]);
    
    return 8;
}

static int gl_write_string(uint8_t **buf, const uint8_t **n) {
    for (uint64_t i = 0; i < gl_array_get_size(*n); i++) {
        gl_write_uint8(buf, &(*n)[i]);
    }
    
    return gl_array_get_size(*n);
}

static int gl_write_cstring(uint8_t **buf, const char **n) {
    for (uint64_t i = 0; i < strlen(*n); i++) {
        gl_write_uint8(buf, (const uint8_t *)&(*n)[i]);
    }
    
    return (int)strlen(*n);
}

int gl_write_message(int fd, struct gl_message_t *msg) {
    uint8_t *buf = 0;
    const gl_message_definition_t *msg_def = gl_message_definitions()[msg->type];
    
    gl_write_cstring(&buf, (const char **)&msg_def->identifier);
    
    for (uint8_t i = 0; i < msg_def->num_parameters; i++) {
        char separator = gl_separator;
        gl_write_uint8(&buf, (uint8_t *)&separator);
        
        const gl_message_parameter_definition_t *msg_param_def = gl_message_parameter_definitions()[msg_def->parameters[i]];
    
        if (msg_param_def->value_type == GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT8) {
            gl_write_uint8(&buf, &msg->parameters_value[i].uint8_value);
        } else if (msg_param_def->value_type == GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT16) {
            gl_write_uint16(&buf, &msg->parameters_value[i].uint16_value, msg_param_def->endian_conversion);
        } else if (msg_param_def->value_type == GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT32) {
            gl_write_uint32(&buf, &msg->parameters_value[i].uint32_value, msg_param_def->endian_conversion);
        } else if (msg_param_def->value_type == GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT64) {
            gl_write_uint64(&buf, &msg->parameters_value[i].uint64_value, msg_param_def->endian_conversion);
        } else {
            gl_write_string(&buf, (const uint8_t **)&msg->parameters_value[i].string_value);
        }
    }
    
    if (msg_def->protocol == GL_MESSAGE_PROTOCOL_UDP) {
        char terminator = gl_udp_terminator;
        gl_write_uint8(&buf, (uint8_t *)&terminator);
        gl_write_uint8(&buf, (uint8_t *)&terminator);
        gl_write_uint8(&buf, (uint8_t *)&terminator);
    } else {
        char terminator = gl_tcp_terminator;
        gl_write_uint8(&buf, (uint8_t *)&terminator);
        gl_write_uint8(&buf, (uint8_t *)&terminator);
        gl_write_uint8(&buf, (uint8_t *)&terminator);
    }
    
    int size = gl_array_get_size(buf);
    write(fd, buf, size);
    
    gl_array_free(buf);
    
    return size;
}

static int gl_read_uint8(int fd, uint8_t *n) {
    return (int)read(fd, n, sizeof(uint8_t));
}

static int gl_read_uint8_until_separator(int fd, uint8_t **dst, uint8_t *last_c, uint16_t max_size, bool precise_size, bool allow_spaces) {
    uint8_t c = 0;
    
    while (allow_spaces ? !gl_is_terminator(c) : !gl_is_separator_or_terminator(c)) {
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
    gl_assert(gl_read_uint8_until_separator(fd, &identifier_buf, &last_c, gl_calculate_max_message_identifier_size(), false, false) != 0);
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
    
    // Reads all parameters.
    for (uint32_t i = 0; i < msg_def->num_parameters; i++) {
        gl_assert(!gl_is_terminator(last_c));
    
        const gl_message_parameter_definition_t  *msg_param_def = gl_message_parameter_definitions()[msg_def->parameters[i]];
        uint8_t *parameter_value_buf = 0;
    
        // Reads the parameter value.
        gl_assert(gl_read_uint8_until_separator(fd, &parameter_value_buf, &last_c, msg_param_def->length, msg_param_def->precise_length, msg_param_def->allow_space) != 0);
        total_size += gl_array_get_header(parameter_value_buf)->size + 1;
    
        gl_message_parameter_t parameter;
    
        // Convert if necessary.
        if (msg_param_def->value_type == GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT8) {
            parameter = (gl_message_parameter_t) { .uint8_value = parameter_value_buf[0] };
        } else if (msg_param_def->value_type == GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT16) {
            uint16_t n = gl_uint8_to_uint16(parameter_value_buf);
            if (msg_param_def->endian_conversion == GL_MESSAGE_PARAMETER_ENDIAN_CONVERSION_AS_LITTLE) {
                n = le16toh(n);
            } else if (msg_param_def->endian_conversion == GL_MESSAGE_PARAMETER_ENDIAN_CONVERSION_AS_BIG) { // NOLINT
                n = be16toh(n);
            } else {
                n = ntohs(n);
            }
            parameter = (gl_message_parameter_t) { .uint16_value = msg_param_def->has_max_value && n > (uint16_t )msg_param_def->max_value ? (uint16_t )msg_param_def->max_value : n };
        } else if (msg_param_def->value_type == GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT32) {
            uint32_t n = gl_uint8_to_uint32(parameter_value_buf);
            if (msg_param_def->endian_conversion == GL_MESSAGE_PARAMETER_ENDIAN_CONVERSION_AS_LITTLE) {
                n = le32toh(n);
            } else if (msg_param_def->endian_conversion == GL_MESSAGE_PARAMETER_ENDIAN_CONVERSION_AS_BIG) { // NOLINT
                n = be32toh(n);
            } else {
                n = ntohl(n);
            }
            parameter = (gl_message_parameter_t) { .uint32_value = msg_param_def->has_max_value && n > (uint32_t )msg_param_def->max_value ? (uint32_t )msg_param_def->max_value : n };
        } else if (msg_param_def->value_type == GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT64) {
            uint64_t n = gl_uint8_to_uint64(parameter_value_buf);
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
    gl_assert(msg_def->protocol != GL_MESSAGE_PROTOCOL_TCP || gl_is_tcp_terminator(last_c));
    gl_assert(msg_def->protocol != GL_MESSAGE_PROTOCOL_UDP || gl_is_udp_terminator(last_c));
    
    // Reads ending characters.
    uint8_t c;
    gl_assert(gl_read_uint8(fd, &c) != 0);
    gl_assert(gl_read_uint8(fd, &c) != 0);
    total_size += 2;
    
    return total_size;
}

int gl_printf_string(uint8_t **str) {
    for (uint64_t i = 0; i < gl_array_get_size(*str); i++) {
        printf("%c", (*str)[i]);
    }
}

int gl_printf_message(struct gl_message_t *msg) {
    const gl_message_definition_t *msg_def = gl_message_definitions()[msg->type];
    
    gl_assert(gl_array_get_size(msg->parameters_value) == msg_def->num_parameters);
    
    printf("%s", msg_def->identifier);
    
    for (uint8_t i = 0; i < msg_def->num_parameters; i++) {
        const gl_message_parameter_definition_t *msg_param_def = gl_message_parameter_definitions()[msg_def->parameters[i]];
        
        printf(" ");
        
        if (msg_param_def->value_type == GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT8) {
            printf("%u", msg->parameters_value[i].uint8_value);
        } else if (msg_param_def->value_type == GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT16) {
            printf("%hu", msg->parameters_value[i].uint16_value);
        } else if (msg_param_def->value_type == GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT32) {
            printf("%u", msg->parameters_value[i].uint32_value);
        } else if (msg_param_def->value_type == GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT64) {
            printf("%llu", msg->parameters_value[i].uint64_value);
        } else if (msg_param_def->value_type == GL_MESSAGE_PARAMETER_VALUE_TYPE_STRING) {
            gl_printf_string(&msg->parameters_value[i].string_value);
        }
    }
    
    if (msg_def->protocol == GL_MESSAGE_PROTOCOL_UDP) {
        printf("%c", gl_udp_terminator);
        printf("%c", gl_udp_terminator);
        printf("%c", gl_udp_terminator);
    } else {
        printf("%c", gl_tcp_terminator);
        printf("%c", gl_tcp_terminator);
        printf("%c", gl_tcp_terminator);
    }
    
    printf("\n");
    
    return 0;
}

int gl_add_parameter_to_message(struct gl_message_t *msg, struct gl_message_parameter_t msg_param) {
    gl_array_push(msg->parameters_value, msg_param);
    
    return 0;
}

int gl_free_message(struct gl_message_t *msg) {
    const gl_message_definition_t *msg_def = gl_message_definitions()[msg->type];
    
    for (uint8_t i = 0; i < msg_def->num_parameters; i++) {
        const gl_message_parameter_definition_t *msg_param_def = gl_message_parameter_definitions()[msg_def->parameters[i]];
    
        if (msg_param_def->value_type == GL_MESSAGE_PARAMETER_VALUE_TYPE_STRING) {
            gl_array_free(msg->parameters_value[i].string_value);
        }
    }
    
    if (msg_def->num_parameters > 0) {
        gl_array_free(msg->parameters_value);
    }
    
    return 0;
}