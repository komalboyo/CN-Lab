// TCP Client Code
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int len, sockfd, n;
    struct sockaddr_in address;
    char buf[256];
    int num1, num2;
    char operator;

    // Create a socket for the client
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Name the socket as agreed with the server
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("172.16.48.71");
    address.sin_port = htons(10200);
    len = sizeof(address);

    // Connect your socket to the server’s socket
    if (connect(sockfd, (struct sockaddr *)&address, len) < 0) {
        perror("Connection failed");
        close(sockfd);
        exit(1);
    }

    // Read input from user
    printf("Enter first number: ");
    scanf("%d", &num1);
    printf("Enter operator (+, -, *, /): ");
    scanf(" %c", &operator);  // Space before %c to skip any whitespace
    printf("Enter second number: ");
    scanf("%d", &num2);

    // Format and send the message to the server
    snprintf(buf, sizeof(buf), "%d %c %d", num1, operator, num2);
    write(sockfd, buf, strlen(buf));

    // Read the result from the server
    n = read(sockfd, buf, sizeof(buf) - 1);
    if (n < 0) {
        perror("Read failed");
        close(sockfd);
        exit(1);
    }

    buf[n] = '\0';  // Null-terminate the buffer
    printf("Result from server: %s\n", buf);

    close(sockfd);
    return 0;
}
