#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>
#include<ctype.h>
#define PORTNO 10200
#define MAX_WORD_LENGTH 100
#define INITIAL_SIZE 10
// Function to check if a word is in the list
bool is_in_list(char **list, int size, const char *word) {
for (int i = 0; i < size; i++) {
   if (strcmp(list[i], word) == 0) {
      return true;
   }
}
return false;
}
// Function to remove duplicate words from a sentence and count duplicates
void remove_duplicate(char str[]) {
char **words = malloc(INITIAL_SIZE * sizeof(char*));
int capacity = INITIAL_SIZE;
int size = 0;
char buffer[MAX_WORD_LENGTH];
char *ptr = str;
char *result = malloc(strlen(str) + 1);
result[0] = '\0';
char *result_ptr = result;
while (*ptr) {
int len = 0;
while (*ptr && !isspace(*ptr)) {
buffer[len++] = *ptr++;
}
buffer[len] = '\0';

while (*ptr && isspace(*ptr)) {
ptr++;
}
if (len > 0) {
if (!is_in_list(words, size, buffer)) {
if (size >= capacity) {
capacity *= 2;
words = realloc(words, capacity * sizeof(char*));
}
words[size] = strdup(buffer);
size++;
if (result_ptr != result) {
*result_ptr++ = ' ';
}
strcpy(result_ptr, buffer);
result_ptr += strlen(buffer);
}
}
}
for (int i = 0; i < size; i++) {
free(words[i]);
}
free(words);
if (result_ptr != result) {
*result_ptr = '\0';
}
strcpy(str, result);
free(result);
}

int main()
{
int sockfd,newsockfd,portno,clilen,n=1;
struct sockaddr_in seraddr,cliaddr;
int i,value;
// create an unnamed socket for the server
sockfd = socket(AF_INET,SOCK_STREAM,0);
//Name the socket
seraddr.sin_family = AF_INET;
            seraddr.sin_addr.s_addr = inet_addr("172.16.48.71");
seraddr.sin_port = htons(PORTNO);
bind(sockfd,(struct sockaddr *)&seraddr,sizeof(seraddr));
//Create a connection queue and wait for clients
listen(sockfd,5);
while (1) {
char buf[256];
printf("server waiting\n");
//Accept a connection
clilen = sizeof(clilen);
newsockfd=accept(sockfd,(struct sockaddr *)&cliaddr,&clilen);
//Read and write to client on client_sockfd
while(1){
memset(buf, 0, sizeof(buf)); // Clear the buffer before reading
n = read(newsockfd,buf,sizeof(buf)-1);
buf[n] = '\0';
printf(" Sentence sent from Client is - \"%s\" \n",buf);
if(strcmp(buf,"stop")==0||strcmp(buf,"Stop")==0){
close(newsockfd);
break;
}
remove_duplicate(buf);
printf(" Modified sentence sent to Client is - \"%s\" \n",buf);
n = write(newsockfd,buf,strlen(buf)); // Send only the necessary bytes
}
if(!strcmp(buf,"stop")){
break;
}
}
}
