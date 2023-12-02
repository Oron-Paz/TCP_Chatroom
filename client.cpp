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
    // Create the socket on the client side
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error creating the socket" << std::endl;
        return -1;
    } else {
        std::cout << "Socket Created" << std::endl;
    }

    // Set up server address
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY; 

    // Connect to the server
    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Error connecting to the server" << std::endl;
        close(sockfd);
        return -1;
    }
    std::cout << "Connected to the server!" << std::endl;

    std::cout << "Enter a username you'd like to be displayed as" << std::endl;
    std::string username;
    getline(std::cin, username);

    while (true) {
        // Get user input
        std::cout << username << " >";
        std::string data;
        getline(std::cin, data);

        // Create a message struct
        Message dataToSend;
        strncpy(dataToSend.username, username.c_str(), sizeof(dataToSend.username) - 1);
        dataToSend.username[sizeof(dataToSend.username) - 1] = '\0';
        strncpy(dataToSend.message, data.c_str(), sizeof(dataToSend.message) - 1);
        dataToSend.message[sizeof(dataToSend.message) - 1] = '\0';

        // Serialize the message to a byte stream
        char serializedMessage[sizeof(Message)];
        SerializeMessage(dataToSend, serializedMessage);

        // Send the message to the server
        send(sockfd, serializedMessage, sizeof(serializedMessage), 0);

        // Check for exit condition
        if (strcmp(dataToSend.message, "exit") == 0) {
            break;
        }

        // Receive and display the server's response
        char buffer[sizeof(Message)];
        memset(&buffer, 0, sizeof(buffer)); // Clear the buffer
        ssize_t bytesRead = recv(sockfd, buffer, sizeof(buffer), 0);

        if (bytesRead <= 0) {
            std::cerr << "Error receiving message from the server" << std::endl;
            break;
        }

        // Deserialize the received message
        Message receivedMessage;
        DeserializeMessage(buffer, receivedMessage);

        // Display the received message
        std::cout << "Server: " << receivedMessage.message << std::endl;

        // Check for exit condition
        if (strcmp(receivedMessage.message, "exit") == 0) {
            std::cout << "Server has closed the connection" << std::endl;
            break;
        }
    }

    // Close the socket
    close(sockfd);

    std::cout << "Connection closed..." << std::endl;

    return 0;
}
