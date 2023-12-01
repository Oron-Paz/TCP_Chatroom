#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8222

int main() {
    // Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error creating the socket" << std::endl;
        return -1;
    }

    // Set up server address
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Replace with the server's IP address or domain name

    // Connect to the server
    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Error connecting to the server" << std::endl;
        close(sockfd);
        return -1;
    }

    std::cout << "Connected to the server!" << std::endl;

    while (true) {
        // Get user input
        std::cout << ">";
        std::string data;
        getline(std::cin, data);

        // Send the message to the server
        send(sockfd, data.c_str(), data.length(), 0);

        // Check for exit condition
        if (data == "exit") {
            break;
        }

        // Receive and display the server's response
        char msg[1500];
        memset(&msg, 0, sizeof(msg));  // Clear the buffer
        recv(sockfd, msg, sizeof(msg), 0);

        std::cout << "Server: " << msg << std::endl;

        // Check for exit condition
        if (!strcmp(msg, "exit")) {
            std::cout << "Server has closed the connection" << std::endl;
            break;
        }
    }

    // Close the socket
    close(sockfd);

    std::cout << "Connection closed..." << std::endl;

    return 0;
}
