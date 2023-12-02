#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8222

struct Message {
    char username[50];
    char message[1000];
};

void SerializeMessage(const Message& msg, char* buffer) {
    // Copy the struct to the buffer
    memcpy(buffer, &msg, sizeof(Message));
}

void DeserializeMessage(const char* buffer, Message& msg) {
    // Copy data from the buffer to the struct
    memcpy(&msg, buffer, sizeof(Message));
}

int main() {
    // Create the socket on the server side
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error creating the socket" << std::endl;
        return -1;
    } else {
        std::cout << "Socket Created" << std::endl;
    }

    // Bind the socket to the address of the computer for the server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "Error binding socket" << std::endl;
        return -1;
    } else {
        std::cout << "Binded with socket" << std::endl;
    }

    // Listen for connections from the client side
    listen(sockfd, 5);

    // Create a new socket to handle connection with the client
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);
    int newSd = accept(sockfd, (sockaddr*)&newSockAddr, &newSockAddrSize);
    if (newSd < 0) {
        std::cerr << "Error accepting request from client" << std::endl;
        return -1;
    }
    std::cout << "Connected with client!" << std::endl;

    while (true) {
        // Receive a message from the client
        char buffer[sizeof(Message)];
        memset(&buffer, 0, sizeof(buffer)); // Clear the buffer
        ssize_t bytesRead = recv(newSd, buffer, sizeof(buffer), 0);

        if (bytesRead <= 0) {
            std::cerr << "Error receiving message from the client" << std::endl;
            break;
        }

        // Deserialize the received message
        Message receivedMessage;
        DeserializeMessage(buffer, receivedMessage);

        // Display the received message
        std::cout << receivedMessage.username << ": " << receivedMessage.message << std::endl;

        // Get server's response
        Message responseMessage;
        std::cout << "Enter your response: ";
        std::cin.getline(responseMessage.message, sizeof(responseMessage.message));

        // Serialize the response message to a byte stream
        char responseBuffer[sizeof(Message)];
        SerializeMessage(responseMessage, responseBuffer);

        // Send the response to the client
        send(newSd, responseBuffer, sizeof(responseBuffer), 0);

        // Check for exit condition
        if (strcmp(responseMessage.message, "exit") == 0) {
            std::cout << "Server has closed the connection" << std::endl;
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
