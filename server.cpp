#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>

#define PORT 8222

int main(int argc, char const* argv[]) {
    // Buffer for messages to be sent and received
    char msg[1500];

    // Create the socket on the server side
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cout << "Error creating the socket" << std::endl;
        return -1;
    }
    else {
        std::cout << "Socket Created" << std::endl;
    }

    // Bind the socket to the address of the computer for the server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cout << "Error binding socket" << std::endl;
        return -1;
    }
    else {
        std::cout << "Binded with socket" << std::endl;
    }

    // Listen for connections from the client side
    listen(sockfd, 5);

    // Create a new socket to handle connection with the client
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);
    int newSd = accept(sockfd, (sockaddr*)&newSockAddr, &newSockAddrSize);
    if (newSd < 0) {
        std::cout << "Error accepting request from client" << std::endl;
        return -1;
    }
    std::cout << "Connected with client!" << std::endl;

    while (1) {
        // Receive a message from the client
        std::cout << "Awaiting client response..." << std::endl;
        memset(&msg, 0, sizeof(msg));  // Clear the buffer

        // Receive a message from the client
        ssize_t bytesRead = recv(newSd, msg, sizeof(msg), 0);

        if (bytesRead <= 0) {
            std::cerr << "Error receiving message from the client" << std::endl;
            break;
        }

        // Check if the client wants to exit
        if (!strcmp(msg, "exit")) {
            std::cout << "Client has quit the session" << std::endl;
            break;
        }

        std::cout << "Client: " << msg << std::endl;
        std::cout << ">";

        std::string data;
        getline(std::cin, data);

        // Send the message to the client
        send(newSd, data.c_str(), data.length(), 0);

        if (data == "exit") {
            // Send to the client that the server has closed the connection
            send(newSd, (char*)&msg, strlen(msg), 0);
            break;
        }
    }

    // Close the socket descriptors after we're all done
    close(newSd);
    close(sockfd);
    std::cout << "********Session********" << std::endl;

    std::cout << "Connection closed..." << std::endl;
    return 0;
}
