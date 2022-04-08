#include "types.h"
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <printf.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"
#include "message.h"

int main(int argc, char **argv) {
    int sock = socket(PF_INET,SOCK_STREAM,0);
    struct sockaddr_in address_sock;
    address_sock.sin_family = AF_INET;
    address_sock.sin_port = htons(4242);
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
                printf("Port de l'appelant: %d\n", ntohs(client.sin_port));
                printf("Adresse de l'appelant: %s\n", inet_ntoa(client.sin_addr));
                
                gl_message_t msg = { 0 };
                gl_read_message(sock2, &msg);
                printf("received");
            }
            
            close(sock2);
        }
    }
    
    return EXIT_SUCCESS;
    
error:
    return gl_get_error();
}
