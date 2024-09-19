#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

int main()
{
	int len,result,sockfd,key,chlen,i, n=1;
	struct sockaddr_in address;
	char ch[256],buf[256], ch2[256];

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	address.sin_family=AF_INET;
	address.sin_addr.s_addr=inet_addr("172.16.48.48"); 
	address.sin_port=htons(10200);
	connect(sockfd,(struct sockaddr *)&address, sizeof(address));
	
	//Logic for problem mentioned here
	while(1){
		memset(buf, 0, sizeof(buf));
		memset(ch, 0, sizeof(ch));
		memset(ch2, 0, sizeof(ch2));
		printf("Enter secret key: ");
		scanf("%d", &key);
		write(sockfd, &key, sizeof(key));
		//SUPER IMPORTANT AFTER SCANF AND BEFORE READING STRING
		getchar();
		
		printf("\nEnter text: ");
		fgets(ch, sizeof(ch), stdin);
		ch[strcspn(ch, "\n")] = '\0';
		
		// YOU CAN'T STOP BEFORE WRITING YOU IDIOT
		//if (strcmp(ch, "stop") == 0) {
		//	close(sockfd);
		//	break;
		//}
		for (i=0;ch[i]!='\0';i++){
			ch2[i] = ch[i];
			ch[i] = ch[i] + key;
		}
		ch2[i] = '\0';
		//chlen = strlen(ch);
		write(sockfd,ch, strlen(ch)+1);
		if (strcmp(ch2, "stop") == 0) {
			close(sockfd);
			break;
		}
		
		printf("FROM SERVER decrypted text is ... ");
		n=read(sockfd,buf,sizeof(buf)-1);
		buf[n] = '\0';
		printf("%s\n",buf);
	}
	exit(0);
}
