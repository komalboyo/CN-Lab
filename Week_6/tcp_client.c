// TCP Client Code:
// Make the necessary includes and set up the variables
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int len, result, sockfd, n = 1;
    struct sockaddr_in address;
    char ch[256], buf[256];

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
    result = connect(sockfd, (struct sockaddr *)&address, len);
    if (result == -1) {
        perror("CLIENT ERROR");
        close(sockfd);
        exit(1);
    }

    // You can now read and write via sockfd
    printf("\nENTER STRING: ");
    scanf("%[^\n]s", ch);
    write(sockfd, ch, strlen(ch));
    printf("STRING SENT BACK FROM SERVER IS:\n");
    //while (n > 0) {
        n = read(sockfd, buf, sizeof(buf));
        printf("%s\n", buf);
    //}

    close(sockfd);
    return 0;
}

