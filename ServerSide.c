#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define Max 1024

int main(int argc, char *argv[]) {
    // Declare variables for socket descriptor, server address, client address, and client message.
    int s, clientAccept;
    struct sockaddr_in server_addr, client_addr;
    char buffer[Max];
    int num1, num2, result;

    // Create a socket using socket() function and set the server address.
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("Error: Could not create socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(1234); // Change to desired port number

    // Bind the socket to the server address using bind() function.
    if (bind(s, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error: Could not bind socket to server address");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections using listen() function.
    if (listen(s, 1) < 0) {
        perror("Error: Could not listen for incoming connections");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", ntohs(server_addr.sin_port));

    // Accept incoming connections using accept() function.
    socklen_t client_len = sizeof(client_addr);
    clientAccept = accept(s, (struct sockaddr *)&client_addr,&client_len);
    if (clientAccept < 0) {
        perror("Error: Could not accept incoming connection");
        exit(EXIT_FAILURE);
    }

    // Receive the message from the client using recv() function and store it in a buffer.
    if (recv(clientAccept, buffer, Max, 0) < 0) {
        perror("Error: Could not receive message from client");
        exit(EXIT_FAILURE);
    }

    // Parse the message string into two integers using sscanf() function.
    sscanf(buffer, "%d+%d", &num1, &num2);

    // Perform addition on the two integers.
    result = num1 + num2;

    // Convert the result integer to a string using sprintf() function.
    sprintf(buffer, "%d", result);

    // Send the result to the client using send() function.
    if (send(clientAccept, buffer, strlen(buffer), 0) < 0) {
        perror("Error: Could not send result to client");
        exit(EXIT_FAILURE);
    }

    printf("Result sent to client: %d\n", result);

    // Close the client and server sockets.
    close(clientAccept);
    close(s);

    return 0;
}
