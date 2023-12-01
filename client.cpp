#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>
#define PORT 8222

int main(int argc, char *argv[])
{
    char msg[1500];  //buffer for message
    int sockfdClient = socket(AF_INET, SOCK_STREAM, 0); //creating a socket for the client
    if (sockfdClient < 0) {
        std::cerr << "Error creating the socket" << std::endl;
        return -1;
    }

    struct sockaddr_in serverAddr;  //writing the server adress adress
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY; 
    sockaddr_in sendSockAddr;   

    //connecting and checking connection with server -----//
    if (connect(sockfdClient, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) { 
        std::cerr << "Error connecting to the server" << std::endl;
        close(sockfdClient);
        return -1;
    }
    std::cout << "Connected to the server!" << std::endl;

    //recieving and sending messages ----//
    while (true) {
        // Get user input
        std::cout << ">";
        std::string data;
        getline(std::cin, data);

        // Send the message to the server
        send(sockfdClient, data.c_str(), data.length(), 0);
        // Check for exit condition
        if (data == "exit") {
            break;
        }

        // Receive and display the server's response
        memset(&msg, 0, sizeof(msg));  // Clear the buffer
        recv(sockfdClient, msg, sizeof(msg), 0);

        std::cout << "Server: " << msg << std::endl;

        // Check for exit condition ----//
        if (!strcmp(msg, "exit")) {
            std::cout << "Server has closed the connection" << std::endl;
            break;
        }
    }

    // Close the socket ----//
    close(sockfdClient);

    std::cout << "Connection closed..." << std::endl;

    return 0;
}