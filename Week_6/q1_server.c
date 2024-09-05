#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<stdlib.h>
#define PORTNO 10205
int calculate(char*buf)
{
  char op;
  int num1,num2;
  sscanf(buf,"%d %c %d",&num1,&op,&num2);
  printf("%d\n",num1);
  printf("%d\n",num2);
  int sol;
  switch (op)
  {
    case '+': sol=num1+num2;
              return sol;
    case '-': sol=num1-num2;
              return sol;
    case '*': sol=num1*num2;
              return sol;
    case '/': sol=num1/num2;
              return sol;
    case '%': sol=num1%num2;
              return sol;   
  }
}
int main()
{
	int sockfd,newsockfd,portno,clilen,n=1;char buf[256];
	struct sockaddr_in seraddr,cliaddr;
	int i,value;
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	seraddr.sin_family = AF_INET;
	seraddr.sin_addr.s_addr = inet_addr("172.16.48.122"); 
	seraddr.sin_port = htons(PORTNO);
	bind(sockfd,(struct sockaddr *)&seraddr,sizeof(seraddr));
	// Create a connection queue, ignore child exit details, and wait for clients
	listen(sockfd,5);
	printf("Server waiting...\n");
	while(1){
		//Accept the connection
		clilen = sizeof(clilen);
		newsockfd=accept(sockfd,(struct sockaddr *)&cliaddr,&clilen);
		//Fork to create a process for this client and perform a test to see whether
		//you’re the parent or the child:
		if(fork()==0){
			// If you’re the child, you can now read/write to the client on newsockfd.
			n = read(newsockfd,buf,sizeof(buf));
			printf(" \nQuestion from Client %s \n",buf);
			int solution = calculate(buf);
			printf("Solution: %d \n",solution);
			sprintf(buf,"%d",solution);
			n = write(newsockfd,buf,sizeof(buf));
			close(newsockfd);
			exit(0);
		}
		//Otherwise, you must be the parent and your work for this client is finished
		else
		close(newsockfd);
	}
}
