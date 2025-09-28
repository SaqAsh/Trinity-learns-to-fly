#include <netdb.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: %s hostname\n", argv[0]);
    return -1;
  }
  printf("Connecting to %s on port 80...\n", argv[1]);

  // lookup server address
  struct addrinfo hints = {.ai_family = AF_INET, .ai_socktype = SOCK_STREAM};
  struct addrinfo *server;
  getaddrinfo(argv[1], "80", &hints, &server);
  printf("getaddrinfo completed\n");

  // create socket
  int sockfd =
      socket(server->ai_family, server->ai_socktype, server->ai_protocol);
  printf("socket created\n");

  // connect to server
  connect(sockfd, server->ai_addr, server->ai_addrlen);
  printf("connected to server\n");

  // read file contents into buffer
  char buf[1024];
  FILE *file = fopen("request.txt", "r");
  fread(buf, 1, 1024, file);
  fclose(file);
  printf("file read completed\n");

  // send buffer to server
  send(sockfd, buf, 1024, 0);
  printf("data sent to server\n");

  // receive response and dump to screen
  const size_t bufSize = 10 * 1024 * 1024;
  uint8_t *buf_2 = malloc(bufSize);
  if (buf_2 == NULL) {
    printf("Failed to allocate memory\n");
    close(sockfd);
    freeaddrinfo(server);
    return -1;
  }

  size_t rcvd = 0, space = bufSize;

  do {
    ssize_t bytes = recv(sockfd, buf_2 + rcvd, space, 0);
    if (bytes <= 0)
      break;
    rcvd += bytes;
    space -= bytes;
  } while (space > 0);

  // dump response to screen
  write(1, buf_2, rcvd);

  free(buf_2);
  freeaddrinfo(server);
  close(sockfd);

  return 0;
}
