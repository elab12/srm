/*
 * Experiment 2 - Program B: Child processes created with boolean short-circuit fork()
 * fork(); fork() && fork() || fork(); fork();
 * Prints "Yes " 20 times (20 processes execute the printf)
 * SRM Institute of Science and Technology - OS Lab (21CSC202J)
 *
 * Compile: gcc e2b.c -o e2b
 * Run:     ./e2b
 */

#include <stdio.h>
#include <unistd.h>

int main()
{
    fork();
    fork() && fork() || fork();
    fork();
    printf("Yes ");
    return 0;
}
