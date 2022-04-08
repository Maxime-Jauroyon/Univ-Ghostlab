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
    struct sockaddr_in adress_sock;
    adress_sock.sin_family = AF_INET;
    adress_sock.sin_port = htons(4243);
    inet_aton("127.0.0.1",&adress_sock.sin_addr);
    int fd = socket(PF_INET, SOCK_STREAM, 0);
    int r = connect(fd, (struct sockaddr *)&adress_sock, sizeof(struct sockaddr_in));
    if(r != -1) {
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_GAMES, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .uint8_value = 8 });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_OGAMES, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .uint8_value = 4 });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .uint8_value = 7 });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_NEWPL, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("hugoboss") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("4242") });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_REGIS, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("hugoboss") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("4242") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .uint8_value =  5 });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_REGOK, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .uint8_value =  5 });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_REGNO, 0 };
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_START, 0 };
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_UNREG, 0 };
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_UNROK, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .uint8_value =  5 });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_DUNNO, 0 };
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_SIZE_REQ, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .uint8_value =  5 });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_SIZE_RES, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .uint8_value =  5 });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .uint16_value =  265 });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .uint16_value =  345 });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_LIST_REQ, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .uint8_value =  5 });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_LIST_RES, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .uint8_value =  5 });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .uint16_value =  12 });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_PLAYR, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("hugoboss") });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_GAME_REQ, 0 };
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_WELCO, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .uint8_value = 4, });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .uint16_value = 543 });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .uint16_value = 654 });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .uint8_value = 12 });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("127.0.0.1######") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("4242") });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_POSIT, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("hugoboss") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("043") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("053") });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_UPMOV, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("023") });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_DOMOV, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("023") });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_LEMOV, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("023") });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_RIMOV, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("023") });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_MOVE_RES, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("023") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("023") });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_MOVEF, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("023") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("023") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("0023") });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_IQUIT, 0 };
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_GOBYE, 0 };
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_GLIS_REQ, 0 };
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_GLIS_RES, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .uint8_value = 4, });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_GPLYR, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("hugoboss") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("043") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("053") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("0053") });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_MALL_REQ, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("02dfgd gdfg dsfgdfg dfsdg dfg dsgdfg 3") });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_MALL_RES, 0 };
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_SEND_REQ, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("hugoboss") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("02dfgd gdfg dsfgdfg dfsdg dfg dsgdfg 3") });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_SEND_RES, 0 };
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_NSEND, 0 };
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_GHOST, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("023") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("023") });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_SCORE, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("hugoboss") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("0056") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("023") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("023") });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_MESSA, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("hugoboss") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("02dfgd gdfg dsfgdfg dfsdg dfg dsgdfg 3") });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_ENDGA, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("hugoboss") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("0054") });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        {
            gl_message_t msg = { .type = GL_MESSAGE_TYPE_MESSP, 0 };
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("hugoboss") });
            gl_add_parameter_to_message(&msg, (gl_message_parameter_t) { .string_value = gl_string_from_cstring("02dfgd gdfg dsfgdfg dfsdg dfg dsgdfg 3") });
            gl_write_message(fd, &msg);
            gl_free_message(&msg);
        }
    
        for (uint8_t i = 0; i < 39; i++) {
            {
                gl_message_t msg = {0};
                gl_read_message(fd, &msg);
                gl_printf_message(&msg);
            }
        }
        
        close(fd);
    }
    
    return EXIT_SUCCESS;
}
