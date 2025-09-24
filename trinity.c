#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>


int main( int argc, char** argv ) {
    if ( argc != 2 ) {
        printf( "Usage: %s hostname\n", argv[0] );
        return -1;
    }
    printf("Connecting to %s on port 80...\n", argv[1]);

    // lookup server address
    struct addrinfo hints = { .ai_family = AF_INET, .ai_socktype = SOCK_STREAM };
    struct addrinfo *server;
    getaddrinfo(argv[1], "80", &hints, &server);

    // create socket
    int sockfd;

    // connect to server

    // read file contents into buffer

    // send buffer to server

    // receive response and dump to screen
    
    freeaddrinfo(server);
    close(sockfd);

    return 0;
}
