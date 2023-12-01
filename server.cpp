#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#define PORT 8222 //its my lucky number

int main(int argc, char const* argv[]) {
  //buffer for messages to be sent and recieved
  char msg[1500];

  //------- CREATE THE SOCKET ON THE SERVER SIDE --------- //
  int sockfd = socket(AF_INET, SOCK_STREAM , 0);
  if(sockfd < 0) {
    std::cout << "Error creating the socket" << std::endl;
    return -1;
  }

  //------- BIND THE SOCKET TO THE ADRESS OF THE COMPUTER FOR SERVER --------- //
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = INADDR_ANY;
  if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    std::cout << "Error binding socket" << std::endl;
    return -1;
  }

  //------- LISTEN FOR CONNECTIONS FROM CLIENT SIDE --------- //
  listen(sockfd, 5);

  //-------- CREATE NEW SOCKET TO CONNECT WITH CLIENT -------- //
  sockaddr_in newSockAddr;
  socklen_t newSockAddrSize = sizeof(newSockAddr);
  int newSd = accept(sockfd, (sockaddr *)&newSockAddr, &newSockAddrSize);

  

  return 0;
}