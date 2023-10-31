
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define Max 1024

int main(int argc, char *argv[]) {
    // Declare variables for socket descriptor, server address, and client message.
    int s;
    struct sockaddr_in server_addr;
    char message[Max], buffer[Max];
    int num1, num2;

    // Parse the command-line arguments or prompt the user to input two integers.
    if (argc == 3) {
        num1 = atoi(argv[1]);
        num2 = atoi(argv[2]);
    } else {
        printf("Enter two integers: ");
        scanf("%d %d", &num1, &num2);
    }

    // Create a socket using socket() function and set the server address.
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("Error: Could not create socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Change to server IP address
    server_addr.sin_port = htons(1234); // Change to server port number

    // Connect to the server using connect() function.
    if (connect(s, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error: Could not connect to server");
        exit(EXIT_FAILURE);
    }

    // Convert the integers to strings using sprintf() function and concatenate them into a single message string.
    sprintf(message, "%d+%d", num1, num2);

    // Send the message to the server using send() function.
    if (send(s, message, strlen(message), 0) < 0) {
        perror("Error: Could not send message to server");
        exit(EXIT_FAILURE);
    }

    // Receive the response from the server using recv() function and store it in a buffer.
    if (recv(s, buffer, Max, 0) < 0) {
        perror("Error: Could not receive message from server");
        exit(EXIT_FAILURE);
    }

    // Convert the response from string to integer using atoi() function.
    int result = atoi(buffer);

    // Display the final result to the user.
    printf("Result: %d\n", result);

    // Close the socket.
    close(s);

    return 0;
}
