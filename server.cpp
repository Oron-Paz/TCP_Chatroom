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

  //-------- CREATE NEW SOCKET TO HANDLE CONNECTION WITH CLIENT -------- //
  sockaddr_in newSockAddr;
  socklen_t newSockAddrSize = sizeof(newSockAddr);
  std::cout << newSockAddrSize << std::endl;
  int newSd = accept(sockfd, (sockaddr *)&newSockAddr, &newSockAddrSize);
  if(newSd<0){
    std::cout << "Error accepting request from client" << std::endl;
    return -1;
  }
  std::cout << "Connected with client!" << std::endl;
  while(1)
    {
        //receive a message from the client (listen)
        std::cout << "Awaiting client response..." << std::endl;
        memset(&msg, 0, sizeof(msg));  //clear the buffer
        if(!strcmp(msg, "exit")) // if the client types exit the client stops being listened to
        {
            std::cout << "Client has quit the session" << std::endl;
            break;
        }
        std::cout << "Client: " << msg << std::endl;
        std::cout << ">";
        std::string data;
        getline(std::cin , data);
        memset(&msg, 0, sizeof(msg)); //clear the buffer
        strcpy(msg, data.c_str());
        if(data == "exit")
        {
            //send to the client that server has closed the connection
            send(newSd, (char*)&msg, strlen(msg), 0);
            break;
        }
        //send the message to client
    }
    //we need to close the socket descriptors after we're all done
    close(newSd);
    close(sockfd);
    std::cout << "********Session********" << std::endl;
   
    std::cout << "Connection closed..." << std::endl;
    return 0;   
}