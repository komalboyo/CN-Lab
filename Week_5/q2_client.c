#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
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
    struct sockaddr_in address;
    int matrix[ROWS][COLS];

    // Create a UDP socket
    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Construct the address for the server
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("172.20.10.2");
    address.sin_port = htons(PORT);

    printf("Enter a %dx%d matrix (row-wise):\n", ROWS, COLS);
    
    // Input matrix from the user
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            scanf("%d", &matrix[i][j]);
        }
    }

    int len = sizeof(address);

    // Send matrix rows one at a time
    for(int i = 0; i < ROWS; i++) {
        char buf[100];
        memset(buf, 0, sizeof(buf));
        
        // Convert the matrix row to a string
        sprintf(buf, "%d %d %d", matrix[i][0], matrix[i][1], matrix[i][2]);

        // Send the row to the server
        int m = sendto(sd, buf, sizeof(buf), 0, (struct sockaddr *)&address, len);
        if(m < 0) {
            perror("Send failed");
            close(sd);
            exit(EXIT_FAILURE);
        }

        printf("Sent row %d: %s\n", i+1, buf);
    }

    close(sd);
    return 0;
}

