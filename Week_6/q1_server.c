// TCP Server Code
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define PORTNO 10200
#define BUF_SIZE 256

void handle_client(int newsockfd, struct sockaddr_in *cliaddr) {
    char buf[BUF_SIZE];
    int num1, num2;
    char operator;
    int result;
    socklen_t addr_len = sizeof(struct sockaddr_in);

    // Read the message from the client
    int n = read(newsockfd, buf, sizeof(buf) - 1);
    if (n < 0) {
        perror("Read failed");
        close(newsockfd);
        exit(1);
    }
    buf[n] = '\0';  // Null-terminate the buffer

    // Print client request
    printf("Received request from %s:%d - %s\n", 
           inet_ntoa(cliaddr->sin_addr), ntohs(cliaddr->sin_port), buf);
    sscanf(buf, "%d %c %d", &num1, &operator, &num2);

    // Perform the arithmetic operation
    switch (operator) {
        case '+': result = num1 + num2; break;
        case '-': result = num1 - num2; break;
        case '*': result = num1 * num2; break;
        case '/':
            if (num2 == 0) {
                snprintf(buf, sizeof(buf), "Error: Division by zero");
                write(newsockfd, buf, strlen(buf));
                close(newsockfd);
                exit(1);
            }
            result = num1 / num2; 
            break;
    }

    // Send the result back to the client
    snprintf(buf, sizeof(buf), "Result: %d", result);
    write(newsockfd, buf, strlen(buf));

    // Print the result
    printf("Sent response to %s:%d - %s\n",
           inet_ntoa(cliaddr->sin_addr), ntohs(cliaddr->sin_port), buf);

    close(newsockfd);
}

int main() {
    int sockfd, newsockfd, clilen;
    struct sockaddr_in seraddr, cliaddr;
    pid_t pid;

    // Create a socket for the server
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Set up server address
    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = inet_addr("172.16.48.71");
    seraddr.sin_port = htons(PORTNO);
    if (bind(sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(1);
    }

    // Listen for incoming connections
    listen(sockfd, 5);

    while (1) {
        // Accept a connection
        clilen = sizeof(cliaddr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);
        if (newsockfd < 0) {
            perror("Accept failed");
            close(sockfd);
            exit(1);
        }

        // Fork to handle the client
        pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            close(newsockfd);
            close(sockfd);
            exit(1);
        }

        if (pid == 0) {  // Child process
            close(sockfd);  // Close the listening socket in the child
            handle_client(newsockfd, &cliaddr);
            exit(0);
        } else {  // Parent process
            close(newsockfd);  // Close the connected socket in the parent
        }
    }

    close(sockfd);
    return 0;
}
