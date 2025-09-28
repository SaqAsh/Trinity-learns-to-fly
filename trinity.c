#include <netdb.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

/**
 * CHECK() macro from https://stackoverflow.com/a/3056992/5858370.
 */
#define CHECK(x)                                                               \
  do {                                                                         \
    if (!(x)) {                                                                \
      fprintf(stderr, "%s:%d: ", __func__, __LINE__);                          \
      perror(#x);                                                              \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
  } while (0)

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: %s hostname\n", argv[0]);
    return -1;
  }
  printf("Connecting to %s on port 80...\n", argv[1]);

  // lookup server address
  struct addrinfo hints = {.ai_family = AF_INET, .ai_socktype = SOCK_STREAM};
  struct addrinfo *server;
  CHECK(getaddrinfo(argv[1], "80", &hints, &server) == 0);

  // create socket
  int sockfd =
      socket(server->ai_family, server->ai_socktype, server->ai_protocol);
  CHECK(sockfd != -1);

  // connect to server
  CHECK(connect(sockfd, server->ai_addr, server->ai_addrlen) != -1);

  // read file contents into buffer
  char buf[1024];
  FILE *file = fopen("request.txt", "r");
  CHECK(file != NULL);
  CHECK(fread(buf, 1, 1024, file) > 0);
  fclose(file);

  // send buffer to server
  CHECK(send(sockfd, buf, 1024, 0) != -1);

  // receive response and dump to screen
  const size_t bufSize = 10 * 1024 * 1024;
  uint8_t *buf_2 = malloc(bufSize);
  CHECK(buf_2 != NULL);

  size_t rcvd = 0, space = bufSize;

  do {
    ssize_t bytes = recv(sockfd, buf_2 + rcvd, space, 0);
    CHECK(bytes != -1);
    if (bytes <= 0)
      break;
    rcvd += bytes;
    space -= bytes;
  } while (space > 0);

  // dump response to screen
  write(1, buf_2, rcvd);

  free(buf_2);
  freeaddrinfo(server);
  CHECK(close(sockfd) != -1);

  return 0;
}
