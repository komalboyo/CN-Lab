/*
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main() {
    int sd;
    struct sockaddr_in address;
    sd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("172.16.48.48");
    address.sin_port = htons(10200);

    if (connect(sd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    int pid = fork();
    char messages[10][300];
    int hjj=0;
    if (pid == 0) {
        // Child process to read messages from the server
        while (1) {
            char output[300];
            int h = read(sd, output, sizeof(output) - 1);
            if (h > 0) {
				output[h]='\0';
                //strncpy(messages[hjj], output, 300);
                //hjj++;
                printf("%s",output);
            }
        }
    } else {
        // Parent process to send messages to the server
        while (1) {
            char buf[256];
            //printf("Enter string: ");
            gets(buf);
            write(sd, buf, strlen(buf)); 
            sleep(3);// Send only the actual message length
            /*
            for(int i=0;i<hjj;i++)
				printf("%s\n",messages[i]);
			hjj=0;
        }
    }

    close(sd);
    return 0;
}
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

int sd; // Socket descriptor

void* read_messages(void* arg) {
    char output[300];
    while (1) {
        int bytes_read = read(sd, output, sizeof(output) - 1);
        if (bytes_read > 0) {
            output[bytes_read] = '\0'; // Null-terminate the string
            printf("Received: %s\n", output);
        }
    }
    return NULL;
}

void* write_messages(void* arg) {
    char buf[256];
    while (1) {
        //printf("Enter string: ");
        gets(buf); // Safer than gets
        // Remove newline character if present
        //buf[strcspn(buf, "\n")] = 0; 
        write(sd, buf, strlen(buf));
        //sleep(1); // Throttle sending
    }
    return NULL;
}

int main() {
    struct sockaddr_in address;
    sd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("172.16.48.48");
    address.sin_port = htons(10201);

    if (connect(sd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    pthread_t read_thread, write_thread;

    // Create threads for reading and writing
    pthread_create(&read_thread, NULL, read_messages, NULL);
    pthread_create(&write_thread, NULL, write_messages, NULL);

    // Wait for both threads to finish (they won't in this case)
    pthread_join(read_thread, NULL);
    pthread_join(write_thread, NULL);

    close(sd);
    return 0;
}

