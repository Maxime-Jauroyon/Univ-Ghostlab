#include "message.h"
#include <string.h>
#include <unistd.h>
#include <printf.h>
#include "array.h"
#include "utils.h"
#include "string.h"

static uint8_t g_max_message_identifier_size = 0;

static const gl_message_parameter_definition_t g_message_parameter_n = {
    .identifier = "n",
    .value_type = GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT8,
    .value_length = 1,
    .force_exact_length = true,
    .can_contain_spaces = false,
    .conversion_type = GL_CONVERSION_TYPE_AUTOMATIC,
    .has_max_uint_value = false
};

static const gl_message_parameter_definition_t g_message_parameter_m = {
    .identifier = "m",
    .value_type = GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT8,
    .value_length = 1,
    .force_exact_length = true,
    .can_contain_spaces = false,
    .conversion_type = GL_CONVERSION_TYPE_AUTOMATIC,
    .has_max_uint_value = false
};

static const gl_message_parameter_definition_t g_message_parameter_s = {
    .identifier = "s",
    .value_type = GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT8,
    .value_length = 1,
    .force_exact_length = true,
    .can_contain_spaces = false,
    .conversion_type = GL_CONVERSION_TYPE_AUTOMATIC,
    .has_max_uint_value = false
};

static const gl_message_parameter_definition_t g_message_parameter_id = {
    .identifier = "id",
    .value_type = GL_MESSAGE_PARAMETER_VALUE_TYPE_STRING,
    .value_length = 8,
    .force_exact_length = true,
    .can_contain_spaces = false,
    .conversion_type = GL_CONVERSION_TYPE_AUTOMATIC,
    .has_max_uint_value = false
};

static const gl_message_parameter_definition_t g_message_parameter_port = {
    .identifier = "port",
    .value_type = GL_MESSAGE_PARAMETER_VALUE_TYPE_STRING,
    .value_length = 4,
    .force_exact_length = true,
    .can_contain_spaces = false,
    .conversion_type = GL_CONVERSION_TYPE_AUTOMATIC,
    .has_max_uint_value = false
};

static const gl_message_parameter_definition_t g_message_parameter_h = {
    .identifier = "h",
    .value_type = GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT16,
    .value_length = 2,
    .force_exact_length = true,
    .can_contain_spaces = false,
    .conversion_type = GL_CONVERSION_TYPE_LITTLE_ENDIAN,
    .has_max_uint_value = true,
    .max_value_uint = 1000
};

static const gl_message_parameter_definition_t g_message_parameter_w = {
    .identifier = "w",
    .value_type = GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT16,
    .value_length = 2,
    .force_exact_length = true,
    .can_contain_spaces = false,
    .conversion_type = GL_CONVERSION_TYPE_LITTLE_ENDIAN,
    .has_max_uint_value = true,
    .max_value_uint = 1000
};

static const gl_message_parameter_definition_t g_message_parameter_f = {
    .identifier = "f",
    .value_type = GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT8,
    .value_length = 1,
    .force_exact_length = true,
    .can_contain_spaces = false,
    .conversion_type = GL_CONVERSION_TYPE_AUTOMATIC,
    .has_max_uint_value = false
};

static const gl_message_parameter_definition_t g_message_parameter_ip = {
    .identifier = "ip",
    .value_type = GL_MESSAGE_PARAMETER_VALUE_TYPE_STRING,
    .value_length = 15,
    .force_exact_length = true,
    .can_contain_spaces = false,
    .conversion_type = GL_CONVERSION_TYPE_AUTOMATIC,
    .has_max_uint_value = false
};

static const gl_message_parameter_definition_t g_message_parameter_x = {
    .identifier = "x",
    .value_type = GL_MESSAGE_PARAMETER_VALUE_TYPE_STRING,
    .value_length = 3,
    .force_exact_length = true,
    .can_contain_spaces = false,
    .conversion_type = GL_CONVERSION_TYPE_AUTOMATIC,
    .has_max_uint_value = false
};

static const gl_message_parameter_definition_t g_message_parameter_y = {
    .identifier = "y",
    .value_type = GL_MESSAGE_PARAMETER_VALUE_TYPE_STRING,
    .value_length = 3,
    .force_exact_length = true,
    .can_contain_spaces = false,
    .conversion_type = GL_CONVERSION_TYPE_AUTOMATIC,
    .has_max_uint_value = false
};

static const gl_message_parameter_definition_t g_message_parameter_d = {
    .identifier = "d",
    .value_type = GL_MESSAGE_PARAMETER_VALUE_TYPE_STRING,
    .value_length = 3,
    .force_exact_length = true,
    .can_contain_spaces = false,
    .conversion_type = GL_CONVERSION_TYPE_AUTOMATIC,
    .has_max_uint_value = false
};

static const gl_message_parameter_definition_t g_message_parameter_p = {
    .identifier = "p",
    .value_type = GL_MESSAGE_PARAMETER_VALUE_TYPE_STRING,
    .value_length = 4,
    .force_exact_length = true,
    .can_contain_spaces = false,
    .conversion_type = GL_CONVERSION_TYPE_AUTOMATIC,
    .has_max_uint_value = false
};

static const gl_message_parameter_definition_t g_message_parameter_mess = {
    .identifier = "mess",
    .value_type = GL_MESSAGE_PARAMETER_VALUE_TYPE_STRING,
    .value_length = 200,
    .force_exact_length = false,
    .can_contain_spaces = true,
    .conversion_type = GL_CONVERSION_TYPE_AUTOMATIC,
    .has_max_uint_value = false
};

static const gl_message_parameter_definition_t *gl_message_parameter_definitions_array[] = {
    [GL_MESSAGE_PARAMETER_TYPE_N] = &g_message_parameter_n,
    [GL_MESSAGE_PARAMETER_TYPE_M] = &g_message_parameter_m,
    [GL_MESSAGE_PARAMETER_TYPE_S] = &g_message_parameter_s,
    [GL_MESSAGE_PARAMETER_TYPE_ID] = &g_message_parameter_id,
    [GL_MESSAGE_PARAMETER_TYPE_PORT] = &g_message_parameter_port,
    [GL_MESSAGE_PARAMETER_TYPE_H] = &g_message_parameter_h,
    [GL_MESSAGE_PARAMETER_TYPE_W] = &g_message_parameter_w,
    [GL_MESSAGE_PARAMETER_TYPE_F] = &g_message_parameter_f,
    [GL_MESSAGE_PARAMETER_TYPE_IP] = &g_message_parameter_ip,
    [GL_MESSAGE_PARAMETER_TYPE_X] = &g_message_parameter_x,
    [GL_MESSAGE_PARAMETER_TYPE_Y] = &g_message_parameter_y,
    [GL_MESSAGE_PARAMETER_TYPE_D] = &g_message_parameter_d,
    [GL_MESSAGE_PARAMETER_TYPE_P] = &g_message_parameter_p,
    [GL_MESSAGE_PARAMETER_TYPE_MESS] = &g_message_parameter_mess,
    
    [GL_MESSAGE_PARAMETER_TYPE_COUNT] = 0
};

static const gl_message_definition_t gl_message_game = {
    .identifier = "GAMES",
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .num_parameters = 1,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_N
    }
};

static const gl_message_definition_t gl_message_ogame = {
    .identifier = "OGAMES",
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .num_parameters = 2,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_M,
        GL_MESSAGE_PARAMETER_TYPE_S
    }
};

static const gl_message_definition_t gl_message_newpl = {
    .identifier = "NEWPL",
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .num_parameters = 2,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_ID,
        GL_MESSAGE_PARAMETER_TYPE_PORT
    }
};

static const gl_message_definition_t gl_message_regis = {
    .identifier = "REGIS",
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .num_parameters = 3,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_ID,
        GL_MESSAGE_PARAMETER_TYPE_PORT,
        GL_MESSAGE_PARAMETER_TYPE_M
    }
};

static const gl_message_definition_t gl_message_regok = {
    .identifier = "REGOK",
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .num_parameters = 1,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_M
    }
};

static const gl_message_definition_t gl_message_regno = {
    .identifier = "REGNO",
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .num_parameters = 0
};

static const gl_message_definition_t gl_message_start = {
    .identifier = "START",
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .num_parameters = 0
};

static const gl_message_definition_t gl_message_unreg = {
    .identifier = "UNREG",
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .num_parameters = 0
};

static const gl_message_definition_t gl_message_unrok = {
    .identifier = "UNROK",
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .num_parameters = 1,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_M
    }
};

static const gl_message_definition_t gl_message_dunno = {
    .identifier = "DUNNO",
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .num_parameters = 0
};

static const gl_message_definition_t gl_message_size_req = {
    .identifier = "SIZE?",
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .num_parameters = 1,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_M
    }
};

static const gl_message_definition_t gl_message_size_res = {
    .identifier = "SIZE!",
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .num_parameters = 3,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_M,
        GL_MESSAGE_PARAMETER_TYPE_H,
        GL_MESSAGE_PARAMETER_TYPE_W
    }
};

static const gl_message_definition_t gl_message_list_req = {
    .identifier = "LIST?",
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .num_parameters = 1,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_M
    }
};

static const gl_message_definition_t gl_message_list_res = {
    .identifier = "LIST!",
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .num_parameters = 2,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_M,
        GL_MESSAGE_PARAMETER_TYPE_S
    }
};

static const gl_message_definition_t gl_message_playr = {
    .identifier = "PLAYR",
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .num_parameters = 1,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_ID
    }
};

static const gl_message_definition_t gl_message_game_req = {
    .identifier = "GAME?",
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .num_parameters = 0
};

static const gl_message_definition_t gl_message_welco = {
    .identifier = "WELCO",
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .num_parameters = 6,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_M,
        GL_MESSAGE_PARAMETER_TYPE_H,
        GL_MESSAGE_PARAMETER_TYPE_W,
        GL_MESSAGE_PARAMETER_TYPE_F,
        GL_MESSAGE_PARAMETER_TYPE_IP,
        GL_MESSAGE_PARAMETER_TYPE_PORT
    }
};

static const gl_message_definition_t gl_message_posit = {
    .identifier = "POSIT",
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .num_parameters = 3,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_ID,
        GL_MESSAGE_PARAMETER_TYPE_X,
        GL_MESSAGE_PARAMETER_TYPE_Y
    }
};

static const gl_message_definition_t gl_message_upmov = {
    .identifier = "UPMOV",
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .num_parameters = 1,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_D
    }
};

static const gl_message_definition_t gl_message_domov = {
    .identifier = "DOMOV",
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .num_parameters = 1,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_D
    }
};

static const gl_message_definition_t gl_message_lemov = {
    .identifier = "LEMOV",
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .num_parameters = 1,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_D
    }
};

static const gl_message_definition_t gl_message_rimov = {
    .identifier = "RIMOV",
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .num_parameters = 1,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_D
    }
};

static const gl_message_definition_t gl_message_move_res = {
    .identifier = "MOVE!",
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .num_parameters = 2,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_X,
        GL_MESSAGE_PARAMETER_TYPE_Y
    }
};

static const gl_message_definition_t gl_message_movef = {
    .identifier = "MOVEF",
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .num_parameters = 3,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_X,
        GL_MESSAGE_PARAMETER_TYPE_Y,
        GL_MESSAGE_PARAMETER_TYPE_P
    }
};

static const gl_message_definition_t gl_message_iquit = {
    .identifier = "IQUIT",
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .num_parameters = 0
};

static const gl_message_definition_t gl_message_gobye = {
    .identifier = "GOBYE",
    .num_parameters = 0,
    .protocol = GL_MESSAGE_PROTOCOL_TCP
};

static const gl_message_definition_t gl_message_glis_req = {
    .identifier = "GLIS?",
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .num_parameters = 0
};

static const gl_message_definition_t gl_message_glis_res = {
    .identifier = "GLIS!",
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .num_parameters = 1,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_S,
    }
};

static const gl_message_definition_t gl_message_gplyr = {
    .identifier = "GPLYR",
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .num_parameters = 4,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_ID,
        GL_MESSAGE_PARAMETER_TYPE_X,
        GL_MESSAGE_PARAMETER_TYPE_Y,
        GL_MESSAGE_PARAMETER_TYPE_P
    }
};

static const gl_message_definition_t gl_message_mall_req = {
    .identifier = "MALL?",
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .num_parameters = 1,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_MESS,
    }
};

static const gl_message_definition_t gl_message_mall_res = {
    .identifier = "MALL!",
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .num_parameters = 0
};

static const gl_message_definition_t gl_message_send_req = {
    .identifier = "SEND?",
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .num_parameters = 2,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_ID,
        GL_MESSAGE_PARAMETER_TYPE_MESS,
    }
};

static const gl_message_definition_t gl_message_send_res = {
    .identifier = "SEND!",
    .num_parameters = 0,
    .protocol = GL_MESSAGE_PROTOCOL_TCP
};

static const gl_message_definition_t gl_message_glis_nsend = {
    .identifier = "NSEND",
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .num_parameters = 0
};

static const gl_message_definition_t gl_message_ghost = {
    .identifier = "GHOST",
    .protocol = GL_MESSAGE_PROTOCOL_UDP,
    .num_parameters = 2,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_X,
        GL_MESSAGE_PARAMETER_TYPE_Y
    }
};

static const gl_message_definition_t gl_message_score = {
    .identifier = "SCORE",
    .protocol = GL_MESSAGE_PROTOCOL_UDP,
    .num_parameters = 4,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_ID,
        GL_MESSAGE_PARAMETER_TYPE_P,
        GL_MESSAGE_PARAMETER_TYPE_X,
        GL_MESSAGE_PARAMETER_TYPE_Y
    }
};

static const gl_message_definition_t gl_message_messa = {
    .identifier = "MESSA",
    .protocol = GL_MESSAGE_PROTOCOL_UDP,
    .num_parameters = 2,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_ID,
        GL_MESSAGE_PARAMETER_TYPE_MESS
    }
};

static const gl_message_definition_t gl_message_endga = {
    .identifier = "ENDGA",
    .num_parameters = 2,
    .protocol = GL_MESSAGE_PROTOCOL_UDP,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_ID,
        GL_MESSAGE_PARAMETER_TYPE_P
    }
};

static const gl_message_definition_t gl_message_messp = {
    .identifier = "MESSP",
    .protocol = GL_MESSAGE_PROTOCOL_UDP,
    .num_parameters = 2,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_ID,
        GL_MESSAGE_PARAMETER_TYPE_MESS
    }
};

static const gl_message_definition_t *gl_message_definitions_array[] = {
    [GL_MESSAGE_TYPE_GAMES] = &gl_message_game,
    [GL_MESSAGE_TYPE_OGAMES] = &gl_message_ogame,
    [GL_MESSAGE_TYPE_NEWPL] = &gl_message_newpl,
    [GL_MESSAGE_TYPE_REGIS] = &gl_message_regis,
    [GL_MESSAGE_TYPE_REGOK] = &gl_message_regok,
    [GL_MESSAGE_TYPE_REGNO] = &gl_message_regno,
    [GL_MESSAGE_TYPE_START] = &gl_message_start,
    [GL_MESSAGE_TYPE_UNREG] = &gl_message_unreg,
    [GL_MESSAGE_TYPE_UNROK] = &gl_message_unrok,
    [GL_MESSAGE_TYPE_DUNNO] = &gl_message_dunno,
    [GL_MESSAGE_TYPE_SIZE_REQ] = &gl_message_size_req,
    [GL_MESSAGE_TYPE_SIZE_RES] = &gl_message_size_res,
    [GL_MESSAGE_TYPE_LIST_REQ] = &gl_message_list_req,
    [GL_MESSAGE_TYPE_LIST_RES] = &gl_message_list_res,
    [GL_MESSAGE_TYPE_PLAYR] = &gl_message_playr,
    [GL_MESSAGE_TYPE_GAME_REQ] = &gl_message_game_req,
    [GL_MESSAGE_TYPE_WELCO] = &gl_message_welco,
    [GL_MESSAGE_TYPE_POSIT] = &gl_message_posit,
    [GL_MESSAGE_TYPE_UPMOV] = &gl_message_upmov,
    [GL_MESSAGE_TYPE_DOMOV] = &gl_message_domov,
    [GL_MESSAGE_TYPE_LEMOV] = &gl_message_lemov,
    [GL_MESSAGE_TYPE_RIMOV] = &gl_message_rimov,
    [GL_MESSAGE_TYPE_MOVE_RES] = &gl_message_move_res,
    [GL_MESSAGE_TYPE_MOVEF] = &gl_message_movef,
    [GL_MESSAGE_TYPE_IQUIT] = &gl_message_iquit,
    [GL_MESSAGE_TYPE_GOBYE] = &gl_message_gobye,
    [GL_MESSAGE_TYPE_GLIS_REQ] = &gl_message_glis_req,
    [GL_MESSAGE_TYPE_GLIS_RES] = &gl_message_glis_res,
    [GL_MESSAGE_TYPE_GPLYR] = &gl_message_gplyr,
    [GL_MESSAGE_TYPE_MALL_REQ] = &gl_message_mall_req,
    [GL_MESSAGE_TYPE_MALL_RES] = &gl_message_mall_res,
    [GL_MESSAGE_TYPE_SEND_REQ] = &gl_message_send_req,
    [GL_MESSAGE_TYPE_SEND_RES] = &gl_message_send_res,
    [GL_MESSAGE_TYPE_NSEND] = &gl_message_glis_nsend,
    
    [GL_MESSAGE_TYPE_GHOST] = &gl_message_ghost,
    [GL_MESSAGE_TYPE_SCORE] = &gl_message_score,
    [GL_MESSAGE_TYPE_MESSA] = &gl_message_messa,
    [GL_MESSAGE_TYPE_ENDGA] = &gl_message_endga,
    [GL_MESSAGE_TYPE_MESSP] = &gl_message_messp,
    
    [GL_MESSAGE_TYPE_COUNT] = 0
};

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

int gl_message_write(int fd, struct gl_message_t *dst) {
    uint8_t *buf = 0;
    const gl_message_definition_t *msg_def = gl_message_definitions()[dst->type];
    
    gl_write_cstring(&buf, (const char **)&msg_def->identifier);
    
    for (uint8_t i = 0; i < msg_def->num_parameters; i++) {
        char separator = GHOSTLAB_SEPARATOR;
        gl_uint8_write(&buf, (uint8_t *) &separator);
        
        const gl_message_parameter_definition_t *msg_param_def = gl_message_parameter_definitions()[msg_def->parameters[i]];
        
        if (msg_param_def->value_type == GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT8) {
            gl_uint8_write(&buf, &dst->parameters_value[i].uint8_value);
        } else if (msg_param_def->value_type == GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT16) {
            gl_uint16_write(&buf, &dst->parameters_value[i].uint16_value, msg_param_def->conversion_type);
        } else if (msg_param_def->value_type == GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT32) {
            gl_uint32_write(&buf, &dst->parameters_value[i].uint32_value, msg_param_def->conversion_type);
        } else if (msg_param_def->value_type == GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT64) {
            gl_uint64_write(&buf, &dst->parameters_value[i].uint64_value, msg_param_def->conversion_type);
        } else {
            gl_write_string(&buf, (const uint8_t **)&dst->parameters_value[i].string_value);
        }
    }
    
    if (msg_def->protocol == GL_MESSAGE_PROTOCOL_UDP) {
        char terminator = GHOSTLAB_UDP_TERMINATOR;
        gl_uint8_write(&buf, (uint8_t *) &terminator);
        gl_uint8_write(&buf, (uint8_t *) &terminator);
        gl_uint8_write(&buf, (uint8_t *) &terminator);
    } else {
        char terminator = GHOSTLAB_TCP_TERMINATOR;
        gl_uint8_write(&buf, (uint8_t *) &terminator);
        gl_uint8_write(&buf, (uint8_t *) &terminator);
        gl_uint8_write(&buf, (uint8_t *) &terminator);
    }
    
    int size = gl_array_get_size(buf);
    write(fd, buf, size);
    
    gl_array_free(buf);
    
    return size;
}

int gl_message_read(int fd, struct gl_message_t *dst) {
    uint16_t total_size = 0;
    
    uint8_t last_c = 0;
    uint8_t *identifier_buf = 0;
    
    // Reads the message type name.
    gl_assert(gl_uint8_read_until_separator(fd, &identifier_buf, &last_c, gl_calculate_max_message_identifier_size(), false, false) != 0);
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
        gl_assert(gl_uint8_read_until_separator(fd, &parameter_value_buf, &last_c, msg_param_def->value_length,
                                                msg_param_def->force_exact_length, msg_param_def->can_contain_spaces) != 0);
        total_size += gl_array_get_header(parameter_value_buf)->size + 1;
        
        gl_message_parameter_t parameter;
        
        // Convert if necessary.
        if (msg_param_def->value_type == GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT8) {
            parameter = (gl_message_parameter_t) { .uint8_value = parameter_value_buf[0] };
        } else if (msg_param_def->value_type == GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT16) {
            uint16_t n = gl_uint8_to_uint16(parameter_value_buf, msg_param_def->conversion_type);
            parameter = (gl_message_parameter_t) { .uint16_value = msg_param_def->has_max_uint_value && n > (uint16_t )msg_param_def->max_value_uint ? (uint16_t )msg_param_def->max_value_uint : n };
        } else if (msg_param_def->value_type == GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT32) {
            uint32_t n = gl_uint8_to_uint32(parameter_value_buf, msg_param_def->conversion_type);
            parameter = (gl_message_parameter_t) { .uint32_value = msg_param_def->has_max_uint_value && n > (uint32_t )msg_param_def->max_value_uint ? (uint32_t )msg_param_def->max_value_uint : n };
        } else if (msg_param_def->value_type == GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT64) {
            uint64_t n = gl_uint8_to_uint64(parameter_value_buf, msg_param_def->conversion_type);
            parameter = (gl_message_parameter_t) { .uint64_value = msg_param_def->has_max_uint_value && n > (uint64_t )msg_param_def->max_value_uint ? (uint64_t )msg_param_def->max_value_uint : n };
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
    gl_assert(gl_uint8_read(fd, &c) != 0);
    gl_assert(gl_uint8_read(fd, &c) != 0);
    total_size += 2;
    
    return total_size;
}

int gl_message_printf(struct gl_message_t *msg) {
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
            gl_string_printf(&msg->parameters_value[i].string_value);
        }
    }
    
    if (msg_def->protocol == GL_MESSAGE_PROTOCOL_UDP) {
        printf("%c%c%c", GHOSTLAB_UDP_TERMINATOR, GHOSTLAB_UDP_TERMINATOR, GHOSTLAB_UDP_TERMINATOR);
    } else {
        printf("%c%c%c", GHOSTLAB_TCP_TERMINATOR, GHOSTLAB_TCP_TERMINATOR, GHOSTLAB_TCP_TERMINATOR);
    }
    
    printf("\n");
    
    return 0;
}

int gl_message_push_parameter(struct gl_message_t *msg, struct gl_message_parameter_t msg_param) {
    gl_array_push(msg->parameters_value, msg_param);
    
    return 0;
}

int gl_message_free(struct gl_message_t *msg) {
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

const gl_message_parameter_definition_t **gl_message_parameter_definitions() {
    return gl_message_parameter_definitions_array;
}

const gl_message_definition_t **gl_message_definitions() {
    return gl_message_definitions_array;
}
