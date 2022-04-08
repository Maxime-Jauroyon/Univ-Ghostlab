#include "message.h"

static const gl_message_parameter_definition_t gl_message_parameter_n = {
    .identifier = "n",
    .value_type = GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT8,
    .length = 1,
    .precise_length = true,
    .endian_conversion = GL_MESSAGE_PARAMETER_ENDIAN_CONVERSION_NONE,
    .has_max_value = false
};

static const gl_message_parameter_definition_t gl_message_parameter_m = {
    .identifier = "m",
    .value_type = GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT8,
    .length = 1,
    .precise_length = true,
    .endian_conversion = GL_MESSAGE_PARAMETER_ENDIAN_CONVERSION_NONE,
    .has_max_value = false
};

static const gl_message_parameter_definition_t gl_message_parameter_s = {
    .identifier = "s",
    .value_type = GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT8,
    .length = 1,
    .precise_length = true,
    .endian_conversion = GL_MESSAGE_PARAMETER_ENDIAN_CONVERSION_NONE,
    .has_max_value = false
};

static const gl_message_parameter_definition_t gl_message_parameter_id = {
    .identifier = "id",
    .value_type = GL_MESSAGE_PARAMETER_VALUE_TYPE_STRING,
    .length = 8,
    .precise_length = true,
    .endian_conversion = GL_MESSAGE_PARAMETER_ENDIAN_CONVERSION_NONE
};

static const gl_message_parameter_definition_t gl_message_parameter_port = {
    .identifier = "port",
    .value_type = GL_MESSAGE_PARAMETER_VALUE_TYPE_STRING,
    .length = 4,
    .precise_length = true,
    .endian_conversion = GL_MESSAGE_PARAMETER_ENDIAN_CONVERSION_NONE,
    .has_max_value = false
};

static const gl_message_parameter_definition_t gl_message_parameter_h = {
    .identifier = "h",
    .value_type = GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT16,
    .length = 2,
    .precise_length = true,
    .endian_conversion = GL_MESSAGE_PARAMETER_ENDIAN_CONVERSION_AS_LITTLE,
    .has_max_value = true,
    .max_value = 1000
};

static const gl_message_parameter_definition_t gl_message_parameter_w = {
    .identifier = "w",
    .value_type = GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT16,
    .length = 2,
    .precise_length = true,
    .endian_conversion = GL_MESSAGE_PARAMETER_ENDIAN_CONVERSION_AS_LITTLE,
    .has_max_value = true,
    .max_value = 1000
};

static const gl_message_parameter_definition_t gl_message_parameter_f = {
    .identifier = "f",
    .value_type = GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT8,
    .length = 1,
    .precise_length = true,
    .endian_conversion = GL_MESSAGE_PARAMETER_ENDIAN_CONVERSION_NONE
};

static const gl_message_parameter_definition_t gl_message_parameter_ip = {
    .identifier = "ip",
    .value_type = GL_MESSAGE_PARAMETER_VALUE_TYPE_STRING,
    .length = 15,
    .precise_length = true,
    .endian_conversion = GL_MESSAGE_PARAMETER_ENDIAN_CONVERSION_NONE,
    .has_max_value = false
};

static const gl_message_parameter_definition_t gl_message_parameter_x = {
    .identifier = "x",
    .value_type = GL_MESSAGE_PARAMETER_VALUE_TYPE_STRING,
    .length = 3,
    .precise_length = true,
    .endian_conversion = GL_MESSAGE_PARAMETER_ENDIAN_CONVERSION_NONE,
    .has_max_value = false
};

static const gl_message_parameter_definition_t gl_message_parameter_y = {
    .identifier = "y",
    .value_type = GL_MESSAGE_PARAMETER_VALUE_TYPE_STRING,
    .length = 3,
    .precise_length = true,
    .endian_conversion = GL_MESSAGE_PARAMETER_ENDIAN_CONVERSION_NONE,
    .has_max_value = false
};

static const gl_message_parameter_definition_t gl_message_parameter_d = {
    .identifier = "d",
    .value_type = GL_MESSAGE_PARAMETER_VALUE_TYPE_STRING,
    .length = 3,
    .precise_length = true,
    .endian_conversion = GL_MESSAGE_PARAMETER_ENDIAN_CONVERSION_NONE,
    .has_max_value = false
};

static const gl_message_parameter_definition_t gl_message_parameter_p = {
    .identifier = "p",
    .value_type = GL_MESSAGE_PARAMETER_VALUE_TYPE_STRING,
    .length = 4,
    .precise_length = true,
    .endian_conversion = GL_MESSAGE_PARAMETER_ENDIAN_CONVERSION_NONE,
    .has_max_value = false
};

static const gl_message_parameter_definition_t gl_message_parameter_mess = {
    .identifier = "mess",
    .value_type = GL_MESSAGE_PARAMETER_VALUE_TYPE_STRING,
    .length = 200,
    .precise_length = false,
    .endian_conversion = GL_MESSAGE_PARAMETER_ENDIAN_CONVERSION_NONE,
    .has_max_value = false
};

static const gl_message_parameter_definition_t *gl_message_parameter_definitions_array[] = {
    [GL_MESSAGE_PARAMETER_TYPE_N] = &gl_message_parameter_n,
    [GL_MESSAGE_PARAMETER_TYPE_M] = &gl_message_parameter_m,
    [GL_MESSAGE_PARAMETER_TYPE_S] = &gl_message_parameter_s,
    [GL_MESSAGE_PARAMETER_TYPE_ID] = &gl_message_parameter_id,
    [GL_MESSAGE_PARAMETER_TYPE_PORT] = &gl_message_parameter_port,
    [GL_MESSAGE_PARAMETER_TYPE_H] = &gl_message_parameter_h,
    [GL_MESSAGE_PARAMETER_TYPE_W] = &gl_message_parameter_w,
    [GL_MESSAGE_PARAMETER_TYPE_F] = &gl_message_parameter_f,
    [GL_MESSAGE_PARAMETER_TYPE_IP] = &gl_message_parameter_ip,
    [GL_MESSAGE_PARAMETER_TYPE_X] = &gl_message_parameter_x,
    [GL_MESSAGE_PARAMETER_TYPE_Y] = &gl_message_parameter_y,
    [GL_MESSAGE_PARAMETER_TYPE_D] = &gl_message_parameter_d,
    [GL_MESSAGE_PARAMETER_TYPE_P] = &gl_message_parameter_p,
    [GL_MESSAGE_PARAMETER_TYPE_MESS] = &gl_message_parameter_mess,
    
    [GL_MESSAGE_PARAMETER_TYPE_COUNT] = 0
};

static const gl_message_definition_t gl_message_game = {
    .identifier = "GAMES",
    .num_parameters = 1,
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_N
    }
};

static const gl_message_definition_t gl_message_ogame = {
    .identifier = "OGAMES",
    .num_parameters = 2,
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_M,
        GL_MESSAGE_PARAMETER_TYPE_S
    }
};

static const gl_message_definition_t gl_message_newpl = {
    .identifier = "NEWPL",
    .num_parameters = 2,
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_ID,
        GL_MESSAGE_PARAMETER_TYPE_PORT
    }
};

static const gl_message_definition_t gl_message_regis = {
    .identifier = "REGIS",
    .num_parameters = 3,
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_ID,
        GL_MESSAGE_PARAMETER_TYPE_PORT,
        GL_MESSAGE_PARAMETER_TYPE_M
    }
};

static const gl_message_definition_t gl_message_regok = {
    .identifier = "REGOK",
    .num_parameters = 1,
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_M
    }
};

static const gl_message_definition_t gl_message_regno = {
    .identifier = "REGNO",
    .num_parameters = 0,
    .protocol = GL_MESSAGE_PROTOCOL_TCP
};

static const gl_message_definition_t gl_message_start = {
    .identifier = "START",
    .num_parameters = 0,
    .protocol = GL_MESSAGE_PROTOCOL_TCP
};

static const gl_message_definition_t gl_message_unreg = {
    .identifier = "UNREG",
    .num_parameters = 0,
    .protocol = GL_MESSAGE_PROTOCOL_TCP
};

static const gl_message_definition_t gl_message_unrok = {
    .identifier = "UNROK",
    .num_parameters = 1,
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_M
    }
};

static const gl_message_definition_t gl_message_dunno = {
    .identifier = "DUNNO",
    .num_parameters = 0,
    .protocol = GL_MESSAGE_PROTOCOL_TCP
};

static const gl_message_definition_t gl_message_size_req = {
    .identifier = "SIZE?",
    .num_parameters = 1,
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_M
    }
};

static const gl_message_definition_t gl_message_size_res = {
    .identifier = "SIZE!",
    .num_parameters = 3,
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_M,
        GL_MESSAGE_PARAMETER_TYPE_H,
        GL_MESSAGE_PARAMETER_TYPE_W
    }
};

static const gl_message_definition_t gl_message_list_req = {
    .identifier = "LIST?",
    .num_parameters = 1,
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_M
    }
};

static const gl_message_definition_t gl_message_list_res = {
    .identifier = "LIST!",
    .num_parameters = 2,
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_M,
        GL_MESSAGE_PARAMETER_TYPE_S
    }
};

static const gl_message_definition_t gl_message_playr = {
    .identifier = "PLAYR",
    .num_parameters = 1,
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_ID
    }
};

static const gl_message_definition_t gl_message_game_req = {
    .identifier = "GAME?",
    .num_parameters = 0,
    .protocol = GL_MESSAGE_PROTOCOL_TCP
};

static const gl_message_definition_t gl_message_welco = {
    .identifier = "WELCO",
    .num_parameters = 6,
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
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
    .num_parameters = 3,
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_ID,
        GL_MESSAGE_PARAMETER_TYPE_X,
        GL_MESSAGE_PARAMETER_TYPE_Y
    }
};

static const gl_message_definition_t gl_message_upmov = {
    .identifier = "UPMOV",
    .num_parameters = 1,
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_D
    }
};

static const gl_message_definition_t gl_message_domov = {
    .identifier = "DOMOV",
    .num_parameters = 1,
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_D
    }
};

static const gl_message_definition_t gl_message_lemov = {
    .identifier = "LEMOV",
    .num_parameters = 1,
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_D
    }
};

static const gl_message_definition_t gl_message_rimov = {
    .identifier = "RIMOV",
    .num_parameters = 1,
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_D
    }
};

static const gl_message_definition_t gl_message_move_res = {
    .identifier = "MOVE!",
    .num_parameters = 2,
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_X,
        GL_MESSAGE_PARAMETER_TYPE_Y
    }
};

static const gl_message_definition_t gl_message_movef = {
    .identifier = "MOVEF",
    .num_parameters = 3,
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_X,
        GL_MESSAGE_PARAMETER_TYPE_Y,
        GL_MESSAGE_PARAMETER_TYPE_P
    }
};

static const gl_message_definition_t gl_message_iquit = {
    .identifier = "IQUIT",
    .num_parameters = 0,
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .parameters = { }
};

static const gl_message_definition_t gl_message_gobye = {
    .identifier = "GOBYE",
    .num_parameters = 0,
    .protocol = GL_MESSAGE_PROTOCOL_TCP
};

static const gl_message_definition_t gl_message_glis_req = {
    .identifier = "GLIS?",
    .num_parameters = 0,
    .protocol = GL_MESSAGE_PROTOCOL_TCP
};

static const gl_message_definition_t gl_message_glis_res = {
    .identifier = "GLIS!",
    .num_parameters = 1,
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_S,
    }
};

static const gl_message_definition_t gl_message_gplyr = {
    .identifier = "GPLYR",
    .num_parameters = 4,
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_ID,
        GL_MESSAGE_PARAMETER_TYPE_X,
        GL_MESSAGE_PARAMETER_TYPE_Y,
        GL_MESSAGE_PARAMETER_TYPE_P
    }
};

static const gl_message_definition_t gl_message_mall_req = {
    .identifier = "MALL?",
    .num_parameters = 1,
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_MESS,
    }
};

static const gl_message_definition_t gl_message_mall_res = {
    .identifier = "MALL!",
    .num_parameters = 0,
    .protocol = GL_MESSAGE_PROTOCOL_TCP
};

static const gl_message_definition_t gl_message_send_req = {
    .identifier = "SEND?",
    .num_parameters = 2,
    .protocol = GL_MESSAGE_PROTOCOL_TCP,
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
    .num_parameters = 0,
    .protocol = GL_MESSAGE_PROTOCOL_TCP
};

static const gl_message_definition_t gl_message_ghost = {
    .identifier = "GHOST",
    .num_parameters = 2,
    .protocol = GL_MESSAGE_PROTOCOL_UDP,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_X,
        GL_MESSAGE_PARAMETER_TYPE_Y
    }
};

static const gl_message_definition_t gl_message_score = {
    .identifier = "SCORE",
    .num_parameters = 4,
    .protocol = GL_MESSAGE_PROTOCOL_UDP,
    .parameters = {
        GL_MESSAGE_PARAMETER_TYPE_ID,
        GL_MESSAGE_PARAMETER_TYPE_P,
        GL_MESSAGE_PARAMETER_TYPE_X,
        GL_MESSAGE_PARAMETER_TYPE_Y
    }
};

static const gl_message_definition_t gl_message_messa = {
    .identifier = "MESSA",
    .num_parameters = 2,
    .protocol = GL_MESSAGE_PROTOCOL_UDP,
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
    .num_parameters = 2,
    .protocol = GL_MESSAGE_PROTOCOL_UDP,
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
    [GL_MESSAGE_TYPE_GLPYR] = &gl_message_gplyr,
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

const gl_message_parameter_definition_t **gl_message_parameter_definitions() {
    return gl_message_parameter_definitions_array;
}

const gl_message_definition_t **gl_message_definitions() {
    return gl_message_definitions_array;
}