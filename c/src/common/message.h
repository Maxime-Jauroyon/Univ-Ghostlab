#ifndef GHOSTLAB_MESSAGE_H
#define GHOSTLAB_MESSAGE_H

#include <common/types.h>

struct sockaddr;
struct gl_message_t;
struct socklen_t;

// Defines the list of parameters type.
typedef enum gl_message_parameter_type_t {
    GL_MESSAGE_PARAMETER_TYPE_N,
    GL_MESSAGE_PARAMETER_TYPE_M,
    GL_MESSAGE_PARAMETER_TYPE_S,
    GL_MESSAGE_PARAMETER_TYPE_ID,
    GL_MESSAGE_PARAMETER_TYPE_PORT,
    GL_MESSAGE_PARAMETER_TYPE_H,
    GL_MESSAGE_PARAMETER_TYPE_W,
    GL_MESSAGE_PARAMETER_TYPE_F,
    GL_MESSAGE_PARAMETER_TYPE_IP,
    GL_MESSAGE_PARAMETER_TYPE_X,
    GL_MESSAGE_PARAMETER_TYPE_Y,
    GL_MESSAGE_PARAMETER_TYPE_D,
    GL_MESSAGE_PARAMETER_TYPE_P,
    GL_MESSAGE_PARAMETER_TYPE_MESS,
    
    GL_MESSAGE_PARAMETER_TYPE_COUNT
} gl_message_parameter_type_t;

// Defines the list of parameters value type.
typedef enum gl_message_parameter_value_type_t {
    GL_MESSAGE_PARAMETER_VALUE_TYPE_STRING,
    GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT8,
    GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT16,
    GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT32,
    GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT64
} gl_message_parameter_value_type_t;

// Defines a parameter.
typedef struct gl_message_parameter_definition_t {
    const char *identifier;
    const gl_message_parameter_value_type_t value_type;
    const uint16_t value_length;
    const bool force_exact_length;
    const bool can_contain_spaces;
    const gl_conversion_type_t conversion_type;
    const bool has_max_uint_value;
    const uint64_t max_value_uint;
} gl_message_parameter_definition_t;

// Defines the list of message type.
typedef enum gl_message_type_t {
    // TCP
    GL_MESSAGE_TYPE_GAMES,
    GL_MESSAGE_TYPE_OGAMES,
    GL_MESSAGE_TYPE_NEWPL,
    GL_MESSAGE_TYPE_REGIS,
    GL_MESSAGE_TYPE_REGOK,
    GL_MESSAGE_TYPE_REGNO,
    GL_MESSAGE_TYPE_START,
    GL_MESSAGE_TYPE_UNREG,
    GL_MESSAGE_TYPE_UNROK,
    GL_MESSAGE_TYPE_DUNNO,
    GL_MESSAGE_TYPE_SIZE_REQ,
    GL_MESSAGE_TYPE_SIZE_RES,
    GL_MESSAGE_TYPE_LIST_REQ,
    GL_MESSAGE_TYPE_LIST_RES,
    GL_MESSAGE_TYPE_PLAYR,
    GL_MESSAGE_TYPE_GAME_REQ,
    GL_MESSAGE_TYPE_WELCO,
    GL_MESSAGE_TYPE_POSIT,
    GL_MESSAGE_TYPE_UPMOV,
    GL_MESSAGE_TYPE_DOMOV,
    GL_MESSAGE_TYPE_LEMOV,
    GL_MESSAGE_TYPE_RIMOV,
    GL_MESSAGE_TYPE_MOVE_RES,
    GL_MESSAGE_TYPE_MOVEF,
    GL_MESSAGE_TYPE_IQUIT,
    GL_MESSAGE_TYPE_GOBYE,
    GL_MESSAGE_TYPE_GLIS_REQ,
    GL_MESSAGE_TYPE_GLIS_RES,
    GL_MESSAGE_TYPE_GPLYR,
    GL_MESSAGE_TYPE_MALL_REQ,
    GL_MESSAGE_TYPE_MALL_RES,
    GL_MESSAGE_TYPE_SEND_REQ,
    GL_MESSAGE_TYPE_SEND_RES,
    GL_MESSAGE_TYPE_NSEND,
    GL_MESSAGE_TYPE_MULTI, // EXTENSION
    
    // UDP/Multicast
    GL_MESSAGE_TYPE_GHOST,
    GL_MESSAGE_TYPE_SCORE,
    GL_MESSAGE_TYPE_MESSA,
    GL_MESSAGE_TYPE_ENDGA,
    GL_MESSAGE_TYPE_MESSP,
    GL_MESSAGE_TYPE_SHUTD, // EXTENSION
    
    GL_MESSAGE_TYPE_COUNT
} gl_message_type_t;

// Defines the list of protocols that can be used.
typedef enum gl_message_protocol_t {
    GL_MESSAGE_PROTOCOL_TCP,
    GL_MESSAGE_PROTOCOL_UDP
} gl_message_protocol_t;

// Defines a message.
typedef struct gl_message_definition_t {
    const char *identifier;
    const gl_message_protocol_t protocol;
    void (*function)(struct gl_message_t*, int32_t, void*);
    bool hide_when_received;
    const gl_message_parameter_type_t parameters[];
} gl_message_definition_t;

// Defines a message parameter.
typedef struct gl_message_parameter_t {
    union {
        uint8_t *string_value;
        uint8_t uint8_value;
        uint16_t uint16_value;
        uint32_t uint32_value;
        uint64_t uint64_value;
    };
} gl_message_parameter_t;

// Defines a message of a given type with a list of values.
typedef struct gl_message_t {
    gl_message_type_t type;
    gl_message_parameter_t *parameters_value;
} gl_message_t;

// Sets the mutex to use before executing a message function.
void gl_message_set_mutex(void *mutex);

// Returns the maximum size of a message identifier.
uint8_t gl_message_get_max_identifier_size(gl_message_definition_t **msg_defs);

// Returns the number of parameters that a message can have.
uint32_t gl_message_get_num_parameters(gl_message_definition_t *msg_def);

// Writes a message in a buffer.
int32_t gl_message_write_to_buf(uint8_t **buf, struct gl_message_t *dst);

// Sends a message through TCP.
int32_t gl_message_send_tcp(int32_t fd, struct gl_message_t *msg);

// Sends a message through UDP.
int32_t gl_message_send_udp(const char *ip, const char *port, struct gl_message_t *msg);

// Sends a message through Multicast.
int32_t gl_message_send_multicast(const char *ip, const char *port, struct gl_message_t *msg);

// Receives a message.
int32_t gl_message_recv(int32_t fd, struct gl_message_t *dst, gl_message_protocol_t protocol);

// Logs the message.
int32_t gl_message_printf(struct gl_message_t *msg);

// Pushes a parameter to a message (useful before sending).
int32_t gl_message_push_parameter(struct gl_message_t *msg, struct gl_message_parameter_t msg_param);

// Frees a message.
void gl_message_free(struct gl_message_t *msg);

// Waits for a message and executes it after reception.
int32_t gl_message_wait_and_execute(int32_t socket_id, gl_message_protocol_t protocol);

// Returns the list of message parameter definitions.
gl_message_parameter_definition_t **gl_message_parameter_definitions();

// Returns the list of message definitions.
gl_message_definition_t **gl_message_definitions();

#endif /* GHOSTLAB_MESSAGE_H */
