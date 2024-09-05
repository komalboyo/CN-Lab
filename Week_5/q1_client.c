#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // For close()
int main()
{
int len, result, sockfd;
struct sockaddr_in address;
char ch[256], buf[256];
// Create a socket for the client
sockfd = socket(AF_INET, SOCK_STREAM, 0);// SOCK_STREAM used for TCP
// AF_INET is for IPv4 and 0 is the default protocol.
if (sockfd < 0) {

perror("Socket creation failed");
exit(1);
}
// Set up the address structure
address.sin_family = AF_INET; // IPv4 address family
address.sin_addr.s_addr = inet_addr("172.16.48.71"); // Server's IP address
// inet_addr converts the dot representation into binary.
address.sin_port = htons(10200); // Server's port number
// Host to network short: converts port number from host byte order to network byte order.
len = sizeof(address);
// Connect your socket to the server’s socket
result = connect(sockfd, (struct sockaddr *)&address, len);
// Here address is socket address of server and sockfd is file descriptor returned while creating the socket.
if (result == -1) {
perror("Connection failed");
close(sockfd);
exit(1);
}
// Loop to repeatedly send and receive data
while (1) {
// Prompt user to enter a string
printf("ENTER STRING: ");
if (fgets(ch, sizeof(ch), stdin) == NULL) {// Use fgets instead of gets for safety
perror("Input error");
close(sockfd);
exit(1);
}// Remove newline character if present
ch[strcspn(ch, "\n")] = '\0';

// Write the input string to the server
write(sockfd, ch, strlen(ch));
// Send "stop" message to terminate both client and server
if (strcmp(ch, "stop") == 0) {
break;// Exit the loop and terminate the client
}
// Read the response from the server
int n = read(sockfd, buf, sizeof(buf) - 1);
if (n < 0) {
perror("Read error");
close(sockfd);
exit(1);
}
buf[n] = '\0';// Null-terminate the buffer
puts(buf); // Print the buffer content to stdout
}
close(sockfd);
return 0;
}
