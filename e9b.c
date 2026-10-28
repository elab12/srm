/*
 * Experiment 9 - Part B: Round Robin CPU Scheduling in C
 * All processes arrive at time 0
 * SRM Institute of Science and Technology - OS Lab (21CSC202J)
 *
 * Compile: gcc e9b.c -o e9b
 * Run:     ./e9b
 */

#include <stdio.h>

int main()
{
    int n, i, quantum, time = 0, done = 0;
    int bt[20], rem[20], wt[20], tat[20];
    float avg_wt = 0, avg_tat = 0;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter burst time of each process:\n");
    for (i = 0; i < n; i++)
    {
        printf("P%d: ", i + 1);
        scanf("%d", &bt[i]);
        rem[i] = bt[i];
    }

    printf("Enter time quantum: ");
    scanf("%d", &quantum);

    while (done < n)
    {
        for (i = 0; i < n; i++)
        {
            if (rem[i] > 0)
            {
                if (rem[i] > quantum)
                {
                    time += quantum;
                    rem[i] -= quantum;
                }
                else
                {
                    time += rem[i];
                    rem[i] = 0;
                    tat[i] = time;              // completion time (arrival = 0)
                    wt[i] = tat[i] - bt[i];     // waiting = turnaround - burst
                    done++;
                }
            }
        }
    }

    printf("\nProcess\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for (i = 0; i < n; i++)
    {
        printf("P%d\t%d\t\t%d\t\t%d\n", i + 1, bt[i], wt[i], tat[i]);
        avg_wt += wt[i];
        avg_tat += tat[i];
    }

    printf("\nAverage Waiting Time    = %.2f", avg_wt / n);
    printf("\nAverage Turnaround Time = %.2f\n", avg_tat / n);

    return 0;
}
