#include "types.h"
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <printf.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv) {
    struct sockaddr_in adress_sock;
    adress_sock.sin_family = AF_INET;
    adress_sock.sin_port = htons(4242);
    inet_aton("127.0.0.1",&adress_sock.sin_addr);
    int descr=socket(PF_INET, SOCK_STREAM, 0);
    int r=connect(descr,(struct sockaddr *)&adress_sock, sizeof(struct sockaddr_in));
    if(r!=-1){
        char *mess="NEWPL 12345678 1234***";
        send(descr,mess,strlen(mess),0);
        close(descr);
    }
    
    return EXIT_SUCCESS;
}
