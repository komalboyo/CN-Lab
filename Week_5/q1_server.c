// TCP Server code:
/*Write an iterative TCP client-server program where the client accepts a sentence from the user and sends it to the server. The server will check for duplicate words in the string. Server will find number of occurrences of duplicate words present and remove the duplicate words by retaining single occurrence of the word and send the resultant sentence to the client. The client displays the received data on the client screen. The process repeats until the user enter the string “Stop”. Then both the processes terminate.*/
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include <arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#define PORTNO 10208
#define BUFFER_SIZE 1024

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

int main()
{
	int sockfd,newsockfd,portno,clilen,n=1;
	struct sockaddr_in seraddr,cliaddr;
	int i,value;
	char buf[256], result[256];
	// create an unnamed socket for the server
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	//Name the socket
	seraddr.sin_family = AF_INET;
	seraddr.sin_addr.s_addr = inet_addr("172.16.48.27");// **
	seraddr.sin_port = htons(PORTNO);
	bind(sockfd,(struct sockaddr *)&seraddr,sizeof(seraddr));
	//Create a connection queue and wait for clients
	listen(sockfd,5);
	while (1) {
		memset(buf, 0, sizeof(buf));
		printf("server waiting");
		//Accept a connection
		clilen = sizeof(clilen);
		newsockfd=accept(sockfd,(struct sockaddr *)&cliaddr,&clilen);
		//Read and write to client on client_sockfd (Logic for problem mentioned here)
		n = read(newsockfd,buf,sizeof(buf));
		buf[n] = '\0';
		if (strcmp(buf, "stop") == 0) break;
		printf(" \nMessage from Client %s \n",buf);
		remove_duplicates(buf, result);
		n = write(newsockfd,result,sizeof(buf));
	}
}

