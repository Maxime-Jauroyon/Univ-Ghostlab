#include "types.h"
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <printf.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>
#include "utils.h"
#include "message.h"

int main(int argc, char **argv) {
    int sock = socket(PF_INET,SOCK_STREAM,0);
    struct sockaddr_in address_sock;
    address_sock.sin_family = AF_INET;
    address_sock.sin_port = htons(4243);
    address_sock.sin_addr.s_addr = htonl(INADDR_ANY);
    int r = bind(sock, (struct sockaddr *)&address_sock, sizeof(struct sockaddr_in));
    
    if (!r) {
        r = listen(sock, 0);
        
        if (r < 0) {
            goto error;
        }
        
        while (1) {
            struct sockaddr_in client;
            socklen_t size_client = sizeof(client);
            int sock2 = accept(sock, (struct sockaddr *)&client, &size_client);
            if (sock2 >= 0) {
                while (1) {
                    gl_message_t msg = { 0 };
                    gl_read_message(sock2, &msg);
                    printf("%s(", gl_message_definitions()[msg.type]->identifier);
                    for (uint32_t i = 0; i < gl_message_definitions()[msg.type]->num_parameters; i++) {
                        printf("%s = ", gl_message_parameter_definitions()[gl_message_definitions()[msg.type]->parameters[i]]->identifier);
                        
                        if (gl_message_parameter_definitions()[gl_message_definitions()[msg.type]->parameters[i]]->value_type == GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT8) {
                            printf("%u", msg.parameters_value[i].uint8_value);
                        } else if (gl_message_parameter_definitions()[gl_message_definitions()[msg.type]->parameters[i]]->value_type == GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT16) {
                            printf("%hu", msg.parameters_value[i].uint16_value);
                        } else if (gl_message_parameter_definitions()[gl_message_definitions()[msg.type]->parameters[i]]->value_type == GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT32) {
                            printf("%u", msg.parameters_value[i].uint32_value);
                        } else if (gl_message_parameter_definitions()[gl_message_definitions()[msg.type]->parameters[i]]->value_type == GL_MESSAGE_PARAMETER_VALUE_TYPE_UINT64) {
                            printf("%llu", msg.parameters_value[i].uint64_value);
                        } else if (gl_message_parameter_definitions()[gl_message_definitions()[msg.type]->parameters[i]]->value_type == GL_MESSAGE_PARAMETER_VALUE_TYPE_STRING) {
                            printf("%s", msg.parameters_value[i].string_value);
                        }
                        
                        if (i != gl_message_definitions()[msg.type]->num_parameters - 1) {
                            printf(", ");
                        }
                    }
                    printf(")\n");
                }
            }
            close(sock2);
        }
    }
    
    return EXIT_SUCCESS;
    
error:
    return gl_get_error();
}
