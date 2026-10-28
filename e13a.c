/*
 * Experiment 13 - Part A: FCFS (First Come First Serve) Disk Scheduling in C
 * SRM Institute of Science and Technology - OS Lab (21CSC202J)
 *
 * Compile: gcc e13a.c -o e13a
 * Run:     ./e13a
 */

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int RQ[100], i, n, TotalHeadMoment = 0, initial;

    printf("Enter the number of Requests\n");
    scanf("%d", &n);

    printf("Enter the Requests sequence\n");
    for (i = 0; i < n; i++)
        scanf("%d", &RQ[i]);

    printf("Enter initial head position\n");
    scanf("%d", &initial);

    // logic for FCFS disk scheduling
    for (i = 0; i < n; i++)
    {
        TotalHeadMoment = TotalHeadMoment + abs(RQ[i] - initial);
        initial = RQ[i];
    }

    printf("Total head movement is %d\n", TotalHeadMoment);
    return 0;
}
