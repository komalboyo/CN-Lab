// TCP Client Code
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

#define PORTNO 10205
#define BUF_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    char buf[BUF_SIZE];

    // Create a socket for the client
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Set up server address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Change to server IP if needed
    servaddr.sin_port = htons(PORTNO);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        exit(1);
    }

    // Get the input sentence from the user
    printf("Enter a sentence: ");
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = '\0'; // Remove the trailing newline character

    // Send the sentence to the server
    if (write(sockfd, buf, strlen(buf)) < 0) {
        perror("Write failed");
        close(sockfd);
        exit(1);
    }

    // Receive the processed sentence from the server
    int n = read(sockfd, buf, sizeof(buf) - 1);
    if (n < 0) {
        perror("Read failed");
        close(sockfd);
        exit(1);
    }
    buf[n] = '\0'; // Null-terminate the buffer

    printf("Processed sentence from server: %s\n", buf);

    close(sockfd);
    return 0;
}

