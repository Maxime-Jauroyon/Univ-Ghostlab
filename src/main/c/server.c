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
    
        struct sockaddr_in client;
        socklen_t size_client = sizeof(client);
        int fd = accept(sock, (struct sockaddr *)&client, &size_client);
        
        if (fd >= 0) {
            for (uint8_t i = 0; i < 39; i++) {
                {
                    gl_message_t msg = {0};
                    gl_read_message(fd, &msg);
                    gl_printf_message(&msg);
                    gl_write_message(fd, &msg);
                    gl_free_message(&msg);
                }
            }
        }
    
        close(fd);
    }
    
    return EXIT_SUCCESS;
    
error:
    return gl_get_error();
}
