#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#define PORT 8080

int main(int argc, char const* argv[]) {
  int sockfd = socket(AF_LOCAL, SOCK_STREAM , 0);
  int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);


  std::cout << sockfd << std::endl ;
  std::cout << bind << std::endl ;


  return 0;
}