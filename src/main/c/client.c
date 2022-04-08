#include "types.h"
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <printf.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"
#include "array.h"
#include "message.h"

int main(int argc, char **argv) {
    /*struct sockaddr_in adress_sock;
    adress_sock.sin_family = AF_INET;
    adress_sock.sin_port = htons(4243);
    inet_aton("127.0.0.1",&adress_sock.sin_addr);
    int fd = socket(PF_INET, SOCK_STREAM, 0);
    int r = connect(fd, (struct sockaddr *)&adress_sock, sizeof(struct sockaddr_in));
    if(r != -1) {
        gl_message_t msg_games = {
            .type = GL_MESSAGE_TYPE_GAMES,
            .parameters_value = 0
        };
    
        gl_printf_message(&msg_games);
        
        close(fd);
    }*/
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_GAMES,
            .parameters_value = 0
        };
        gl_message_parameter_t p1 = {
            .uint8_value = 8
        };
        gl_array_push(msg.parameters_value, p1);
        gl_printf_message(&msg);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_OGAMES,
            .parameters_value = 0
        };
        gl_message_parameter_t p1 = {
            .uint8_value = 4
        };
        gl_message_parameter_t p2 = {
            .uint8_value = 7
        };
        gl_array_push(msg.parameters_value, p1);
        gl_array_push(msg.parameters_value, p2);
        gl_printf_message(&msg);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_NEWPL,
            .parameters_value = 0
        };
        gl_message_parameter_t p1 = {
            .string_value = gl_string_from_cstring("hugoboss")
        };
        gl_message_parameter_t p2 = {
            .string_value = gl_string_from_cstring("4242")
        };
        gl_array_push(msg.parameters_value, p1);
        gl_array_push(msg.parameters_value, p2);
        gl_printf_message(&msg);
        gl_array_free(p1.string_value);
        gl_array_free(p2.string_value);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_REGIS,
            .parameters_value = 0
        };
        gl_message_parameter_t p1 = {
            .string_value = gl_string_from_cstring("hugoboss")
        };
        gl_message_parameter_t p2 = {
            .string_value = gl_string_from_cstring("4242")
        };
        gl_message_parameter_t p3 = {
            .uint8_value =  5
        };
        gl_array_push(msg.parameters_value, p1);
        gl_array_push(msg.parameters_value, p2);
        gl_array_push(msg.parameters_value, p3);
        gl_printf_message(&msg);
        gl_array_free(p1.string_value);
        gl_array_free(p2.string_value);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_REGOK,
            .parameters_value = 0
        };
        gl_message_parameter_t p1 = {
            .uint8_value =  5
        };
        gl_array_push(msg.parameters_value, p1);
        gl_printf_message(&msg);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_REGNO,
            .parameters_value = 0
        };
        gl_printf_message(&msg);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_START,
            .parameters_value = 0
        };
        gl_printf_message(&msg);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_UNREG,
            .parameters_value = 0
        };
        gl_printf_message(&msg);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_UNROK,
            .parameters_value = 0
        };
        gl_message_parameter_t p1 = {
            .uint8_value =  5
        };
        gl_array_push(msg.parameters_value, p1);
        gl_printf_message(&msg);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_DUNNO,
            .parameters_value = 0
        };
        gl_printf_message(&msg);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_SIZE_REQ,
            .parameters_value = 0
        };
        gl_message_parameter_t p1 = {
            .uint8_value =  5
        };
        gl_array_push(msg.parameters_value, p1);
        gl_printf_message(&msg);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_SIZE_RES,
            .parameters_value = 0
        };
        gl_message_parameter_t p1 = {
            .uint8_value =  5
        };
        gl_message_parameter_t p2 = {
            .uint16_value =  265
        };
        gl_message_parameter_t p3 = {
            .uint16_value =  345
        };
        gl_array_push(msg.parameters_value, p1);
        gl_array_push(msg.parameters_value, p2);
        gl_array_push(msg.parameters_value, p3);
        gl_printf_message(&msg);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_LIST_REQ,
            .parameters_value = 0
        };
        gl_message_parameter_t p1 = {
            .uint8_value =  5
        };
        gl_array_push(msg.parameters_value, p1);
        gl_printf_message(&msg);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_LIST_RES,
            .parameters_value = 0
        };
        gl_message_parameter_t p1 = {
            .uint8_value =  5
        };
        gl_message_parameter_t p2 = {
            .uint16_value =  12
        };
        gl_array_push(msg.parameters_value, p1);
        gl_array_push(msg.parameters_value, p2);
        gl_printf_message(&msg);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_PLAYR,
            .parameters_value = 0
        };
        gl_message_parameter_t p1 = {
            .string_value = gl_string_from_cstring("hugoboss")
        };
        gl_array_push(msg.parameters_value, p1);
        gl_printf_message(&msg);
        gl_array_free(p1.string_value);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_GAME_REQ,
            .parameters_value = 0
        };
        gl_printf_message(&msg);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_WELCO,
            .parameters_value = 0
        };
        gl_message_parameter_t p1 = {
            .uint8_value = 4,
        };
        gl_message_parameter_t p2 = {
            .uint16_value = 543
        };
        gl_message_parameter_t p3 = {
            .uint16_value = 654
        };
        gl_message_parameter_t p4 = {
            .uint8_value = 12
        };
        gl_message_parameter_t p5 = {
            .string_value = gl_string_from_cstring("127.0.0.1######")
        };
        gl_message_parameter_t p6 = {
            .string_value = gl_string_from_cstring("4242")
        };
        gl_array_push(msg.parameters_value, p1);
        gl_array_push(msg.parameters_value, p2);
        gl_array_push(msg.parameters_value, p3);
        gl_array_push(msg.parameters_value, p4);
        gl_array_push(msg.parameters_value, p5);
        gl_array_push(msg.parameters_value, p6);
        gl_printf_message(&msg);
        gl_array_free(p5.string_value);
        gl_array_free(p6.string_value);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_POSIT,
            .parameters_value = 0
        };
        gl_message_parameter_t p1 = {
            .string_value = gl_string_from_cstring("hugoboss")
        };
        gl_message_parameter_t p2 = {
            .string_value = gl_string_from_cstring("043")
        };
        gl_message_parameter_t p3 = {
            .string_value = gl_string_from_cstring("053")
        };
        gl_array_push(msg.parameters_value, p1);
        gl_array_push(msg.parameters_value, p2);
        gl_array_push(msg.parameters_value, p3);
        gl_printf_message(&msg);
        gl_array_free(p1.string_value);
        gl_array_free(p2.string_value);
        gl_array_free(p3.string_value);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_UPMOV,
            .parameters_value = 0
        };
        gl_message_parameter_t p1 = {
            .string_value = gl_string_from_cstring("023")
        };
        gl_array_push(msg.parameters_value, p1);
        gl_printf_message(&msg);
        gl_array_free(p1.string_value);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_DOMOV,
            .parameters_value = 0
        };
        gl_message_parameter_t p1 = {
            .string_value = gl_string_from_cstring("023")
        };
        gl_array_push(msg.parameters_value, p1);
        gl_printf_message(&msg);
        gl_array_free(p1.string_value);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_LEMOV,
            .parameters_value = 0
        };
        gl_message_parameter_t p1 = {
            .string_value = gl_string_from_cstring("023")
        };
        gl_array_push(msg.parameters_value, p1);
        gl_printf_message(&msg);
        gl_array_free(p1.string_value);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_RIMOV,
            .parameters_value = 0
        };
        gl_message_parameter_t p1 = {
            .string_value = gl_string_from_cstring("023")
        };
        gl_array_push(msg.parameters_value, p1);
        gl_printf_message(&msg);
        gl_array_free(p1.string_value);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_MOVE_RES,
            .parameters_value = 0
        };
        gl_message_parameter_t p1 = {
            .string_value = gl_string_from_cstring("023")
        };
        gl_message_parameter_t p2 = {
            .string_value = gl_string_from_cstring("023")
        };
        gl_array_push(msg.parameters_value, p1);
        gl_array_push(msg.parameters_value, p2);
        gl_printf_message(&msg);
        gl_array_free(p1.string_value);
        gl_array_free(p2.string_value);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_MOVEF,
            .parameters_value = 0
        };
        gl_message_parameter_t p1 = {
            .string_value = gl_string_from_cstring("023")
        };
        gl_message_parameter_t p2 = {
            .string_value = gl_string_from_cstring("023")
        };
        gl_message_parameter_t p3 = {
            .string_value = gl_string_from_cstring("0023")
        };
        gl_array_push(msg.parameters_value, p1);
        gl_array_push(msg.parameters_value, p2);
        gl_array_push(msg.parameters_value, p3);
        gl_printf_message(&msg);
        gl_array_free(p1.string_value);
        gl_array_free(p2.string_value);
        gl_array_free(p3.string_value);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_IQUIT,
            .parameters_value = 0
        };
        gl_printf_message(&msg);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_GOBYE,
            .parameters_value = 0
        };
        gl_printf_message(&msg);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_GLIS_REQ,
            .parameters_value = 0
        };
        gl_printf_message(&msg);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_GLIS_RES,
            .parameters_value = 0
        };
        gl_message_parameter_t p1 = {
            .uint8_value = 4,
        };
        gl_array_push(msg.parameters_value, p1);
        gl_printf_message(&msg);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_GPLYR,
            .parameters_value = 0
        };
        gl_message_parameter_t p1 = {
            .string_value = gl_string_from_cstring("hugoboss")
        };
        gl_message_parameter_t p2 = {
            .string_value = gl_string_from_cstring("043")
        };
        gl_message_parameter_t p3 = {
            .string_value = gl_string_from_cstring("053")
        };
        gl_message_parameter_t p4 = {
            .string_value = gl_string_from_cstring("0053")
        };
        gl_array_push(msg.parameters_value, p1);
        gl_array_push(msg.parameters_value, p2);
        gl_array_push(msg.parameters_value, p3);
        gl_array_push(msg.parameters_value, p4);
        gl_printf_message(&msg);
        gl_array_free(p1.string_value);
        gl_array_free(p2.string_value);
        gl_array_free(p3.string_value);
        gl_array_free(p4.string_value);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_MALL_REQ,
            .parameters_value = 0
        };
        gl_message_parameter_t p1 = {
            .string_value = gl_string_from_cstring("02dfgd gdfg dsfgdfg dfsdg dfg dsgdfg 3")
        };
        gl_array_push(msg.parameters_value, p1);
        gl_printf_message(&msg);
        gl_array_free(p1.string_value);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_MALL_RES,
            .parameters_value = 0
        };
        gl_printf_message(&msg);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_SEND_REQ,
            .parameters_value = 0
        };
        gl_message_parameter_t p1 = {
            .string_value = gl_string_from_cstring("hugoboss")
        };
        gl_message_parameter_t p2 = {
            .string_value = gl_string_from_cstring("02dfgd gdfg dsfgdfg dfsdg dfg dsgdfg 3")
        };
        gl_array_push(msg.parameters_value, p1);
        gl_array_push(msg.parameters_value, p2);
        gl_printf_message(&msg);
        gl_array_free(p1.string_value);
        gl_array_free(p2.string_value);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_SEND_RES,
            .parameters_value = 0
        };
        gl_printf_message(&msg);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_NSEND,
            .parameters_value = 0
        };
        gl_printf_message(&msg);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_GHOST,
            .parameters_value = 0
        };
        gl_message_parameter_t p1 = {
            .string_value = gl_string_from_cstring("023")
        };
        gl_message_parameter_t p2 = {
            .string_value = gl_string_from_cstring("023")
        };
        gl_array_push(msg.parameters_value, p1);
        gl_array_push(msg.parameters_value, p2);
        gl_printf_message(&msg);
        gl_array_free(p1.string_value);
        gl_array_free(p2.string_value);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_SCORE,
            .parameters_value = 0
        };
        gl_message_parameter_t p1 = {
            .string_value = gl_string_from_cstring("hugoboss")
        };
        gl_message_parameter_t p2 = {
            .string_value = gl_string_from_cstring("0056")
        };
        gl_message_parameter_t p3 = {
            .string_value = gl_string_from_cstring("023")
        };
        gl_message_parameter_t p4 = {
            .string_value = gl_string_from_cstring("023")
        };
        gl_array_push(msg.parameters_value, p1);
        gl_array_push(msg.parameters_value, p2);
        gl_array_push(msg.parameters_value, p3);
        gl_array_push(msg.parameters_value, p4);
        gl_printf_message(&msg);
        gl_array_free(p1.string_value);
        gl_array_free(p2.string_value);
        gl_array_free(p3.string_value);
        gl_array_free(p4.string_value);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_MESSA,
            .parameters_value = 0
        };
        gl_message_parameter_t p1 = {
            .string_value = gl_string_from_cstring("hugoboss")
        };
        gl_message_parameter_t p2 = {
            .string_value = gl_string_from_cstring("02dfgd gdfg dsfgdfg dfsdg dfg dsgdfg 3")
        };
        gl_array_push(msg.parameters_value, p1);
        gl_array_push(msg.parameters_value, p2);
        gl_printf_message(&msg);
        gl_array_free(p1.string_value);
        gl_array_free(p2.string_value);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_ENDGA,
            .parameters_value = 0
        };
        gl_message_parameter_t p1 = {
            .string_value = gl_string_from_cstring("hugoboss")
        };
        gl_message_parameter_t p2 = {
            .string_value = gl_string_from_cstring("0054")
        };
        gl_array_push(msg.parameters_value, p1);
        gl_array_push(msg.parameters_value, p2);
        gl_printf_message(&msg);
        gl_array_free(p1.string_value);
        gl_array_free(p2.string_value);
        gl_array_free(msg.parameters_value);
    }
    
    {
        gl_message_t msg = {
            .type = GL_MESSAGE_TYPE_MESSP,
            .parameters_value = 0
        };
        gl_message_parameter_t p1 = {
            .string_value = gl_string_from_cstring("hugoboss")
        };
        gl_message_parameter_t p2 = {
            .string_value = gl_string_from_cstring("02dfgd gdfg dsfgdfg dfsdg dfg dsgdfg 3")
        };
        gl_array_push(msg.parameters_value, p1);
        gl_array_push(msg.parameters_value, p2);
        gl_printf_message(&msg);
        gl_array_free(p1.string_value);
        gl_array_free(p2.string_value);
        gl_array_free(msg.parameters_value);
    }
    
    return EXIT_SUCCESS;
}
