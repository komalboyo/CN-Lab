// TCP Server Code
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#define PORTNO 10205
#define BUF_SIZE 1024

void remove_duplicates(char *sentence) {
    char *words[100];
    int word_count = 0;
    char temp[BUF_SIZE];
    char *token;

    // Tokenize the input sentence
    strcpy(temp, sentence);
    token = strtok(temp, " ");
    while (token != NULL) {
        words[word_count++] = strdup(token);
        token = strtok(NULL, " ");
    }

    // Remove duplicates
    for (int i = 0; i < word_count; i++) {
        for (int j = i + 1; j < word_count; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                free(words[j]);
                for (int k = j; k < word_count - 1; k++) {
                    words[k] = words[k + 1];
                }
                word_count--;
                j--;
            }
        }
    }

    // Reconstruct the sentence
    sentence[0] = '\0';
    for (int i = 0; i < word_count; i++) {
        strcat(sentence, words[i]);
        strcat(sentence, " ");
        free(words[i]);
    }
    sentence[strlen(sentence) - 1] = '\0'; // Remove the trailing space
}

void handle_client(int newsockfd) {
    char buf[BUF_SIZE];
    int n;

    // Read the message from the client
    n = read(newsockfd, buf, sizeof(buf) - 1);
    if (n < 0) {
        perror("Read failed");
        close(newsockfd);
        exit(1);
    }
    buf[n] = '\0'; // Null-terminate the buffer

    printf("Received message: %s\n", buf);

    // Remove duplicate words
    remove_duplicates(buf);

    printf("Processed message: %s\n", buf);

    // Send the result back to the client
    n = write(newsockfd, buf, strlen(buf));
    if (n < 0) {
        perror("Write failed");
        close(newsockfd);
        exit(1);
    }

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
    seraddr.sin_addr.s_addr = INADDR_ANY; // Bind to any interface
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
            handle_client(newsockfd);
            exit(0);
        } else {  // Parent process
            close(newsockfd);  // Close the connected socket in the parent
        }
    }

    close(sockfd);
    return 0;
}

