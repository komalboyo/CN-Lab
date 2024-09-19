#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<stdlib.h>
#define PORTNO 10200

int main()
{
	int sockfd,newsockfd,portno,clilen, key,n=1;
	char buf[256];
	struct sockaddr_in seraddr,cliaddr;
	int i,value;
	
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	seraddr.sin_family = AF_INET;
	seraddr.sin_addr.s_addr = inet_addr("172.16.48.48"); 
	seraddr.sin_port = htons(PORTNO);
	
	bind(sockfd,(struct sockaddr *)&seraddr,sizeof(seraddr));
	listen(sockfd,5);
	printf("Server waiting...\n");
	
	while(1){
		clilen = sizeof(clilen);
		newsockfd=accept(sockfd,(struct sockaddr *)&cliaddr,&clilen);
		
		if(fork()==0) {
			while(1){
				memset(buf, 0, sizeof(buf));
				read(newsockfd, &key, sizeof(key));
				//printf("%d is key", key);
				//getchar();
				n = read(newsockfd,buf,sizeof(buf)-1);
				buf[n] = '\0';
				//printf("\nThe received string from client: %s\n", buf);
				printf(" \nEncrypted text from Client: %s \n",buf);
				for (int i=0;buf[i]!='\0';i++){
				    buf[i] = buf[i] - key;
				}
				if (strcmp(buf, "stop") == 0){
					printf("\nDisconecting client socket...\n");
					close(newsockfd);	
					break;
				}
				printf("Decrypted text: %s \n",buf);
				n = write(newsockfd,buf,n);
				sleep(5);
			}	
			exit(0);
		}
		
		else close(newsockfd);
	}
}

