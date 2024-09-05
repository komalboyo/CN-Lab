#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#define PORT 9704
#define ROWS 3
#define COLS 3

int main() {
    int sd;
    char buf[100];
    struct sockaddr_in sadd, cadd;
    int matrix[ROWS][COLS];
    int row_counter = 0;

    // Create a UDP socket
    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Construct the address for the server
    sadd.sin_family = AF_INET;
    sadd.sin_addr.s_addr = inet_addr("172.20.10.2");
    sadd.sin_port = htons(PORT);

    // Bind the socket to the address
    if(bind(sd, (struct sockaddr *)&sadd, sizeof(sadd)) < 0) {
        perror("Bind failed");
        close(sd);
        exit(EXIT_FAILURE);
    }

    int len = sizeof(cadd);

    printf("Server is waiting for matrix rows...\n");
    
    // Receive matrix rows one at a time
    while(row_counter < ROWS) {
        int m = recvfrom(sd, buf, sizeof(buf), 0, (struct sockaddr *)&cadd, &len);
        if(m < 0) {
            perror("Receive failed");
            close(sd);
            exit(EXIT_FAILURE);
        }
        
        buf[m] = '\0'; // Null-terminate the received string

        // Convert the received string to matrix row
        int i = 0;
        char *token = strtok(buf, " ");
        while(token != NULL && i < COLS) {
            matrix[row_counter][i] = atoi(token); // Convert to integer
            token = strtok(NULL, " ");
            i++;
        }
        row_counter++;
    }

    printf("The received matrix is:\n");
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    close(sd);
    return 0;
}

