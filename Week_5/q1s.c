#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>

#define PORTNO 10208
#define BUFFER_SIZE 256

// Function to check if a word exists in the array
int word_exists(char words[][BUFFER_SIZE], int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

// Function to remove duplicates and form the result
void remove_duplicates(const char *input, char *output) {
    char words[100][BUFFER_SIZE];
    int word_count = 0;
    char *token = strtok((char *)input, " ");

    // Extract words from the input sentence
    while (token != NULL) {
        if (!word_exists(words, word_count, token)) {
            strcpy(words[word_count++], token);
        }
        token = strtok(NULL, " ");
    }

    // Form the result without duplicates
    output[0] = '\0';
    for (int i = 0; i < word_count; i++) {
        strcat(output, words[i]);
        strcat(output, " ");
    }

    // Remove trailing space
    if (strlen(output) > 0) {
        output[strlen(output) - 1] = '\0';
    }
}

int main() {
    int sockfd, newsockfd, clilen, n;
    struct sockaddr_in seraddr, cliaddr;
    char buf[BUFFER_SIZE], result[BUFFER_SIZE];

    // Create an unnamed socket for the server
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Name the socket
    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = inet_addr("172.16.48.27"); // Replace with appropriate IP
    seraddr.sin_port = htons(PORTNO);
    if (bind(sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Create a connection queue and wait for clients
    listen(sockfd, 5);

    while (1) {
        printf("Server waiting...\n");

        // Accept a connection
        clilen = sizeof(cliaddr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cliaddr, (socklen_t *)&clilen);
        if (newsockfd < 0) {
            perror("Accept failed");
            continue;
        }

        while (1) {
            // Read data from the client
            memset(buf, 0, sizeof(buf));
            n = read(newsockfd, buf, sizeof(buf) - 1);
            buf[n] = '\0';
            if (strcmp(buf, "stop") == 0) {
            printf("\nstopping..\n");
                break;
            }
            printf("Message from Client: %s\n", buf);
            remove_duplicates(buf, result);
            n = write(newsockfd, result, strlen(result) + 1); // Include null terminator
        }
        close(newsockfd);
    }

    close(sockfd);
    return 0;
}
