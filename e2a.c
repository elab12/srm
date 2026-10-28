/*
 * Experiment 2 - Program A: Process creation with multiple fork() calls
 * Calculate the number of times the text "SRMIST" is printed (2^3 = 8 times)
 * SRM Institute of Science and Technology - OS Lab (21CSC202J)
 *
 * Compile: gcc e2a.c -o e2a
 * Run:     ./e2a
 */

#include <stdio.h>
#include <unistd.h>

int main()
{
    fork();
    fork();
    fork();
    printf("SRMIST\n");
    return 0;
}
