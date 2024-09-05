//Example : fork()
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
int main(void)
{
int pid = fork();
if(pid == 0) {
//printf("Child => PPID: %d PID: %d\n", getppid(), getpid());
printf("child");
//exit(EXIT_SUCCESS);
}
if(pid > 0) {
//printf("Parent => PID: %d\n", getpid());
//printf("Waiting for child process to finish.\n");
//wait(NULL);
//printf("Child process finished.\n");
printf("Parent");
}
return 0;
}
