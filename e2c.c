/*
 * Experiment 2 - Program C: Process Creation and usage of getpid(), getppid(), wait()
 * SRM Institute of Science and Technology - OS Lab (21CSC202J)
 *
 * Compile: gcc e2c.c -o e2c
 * Run:     ./e2c
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
    int pid;
    pid = fork();
    if (pid == -1)
    {
        perror("fork failed");
        exit(1);
    }
    if (pid == 0)
    {
        printf("\nChild process is under execution");
        printf("\nProcess id of the child process is %d", getpid());
        printf("\nProcess id of the parent process is %d\n", getppid());
    }
    else
    {
        wait(NULL); /* parent waits for the child to finish */
        printf("\nParent process is under execution");
        printf("\nProcess id of the parent process is %d", getpid());
        printf("\nProcess id of the parent of parent is %d\n", getppid());
    }
    return 0;
}
