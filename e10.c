/*
 * Experiment 10: FIFO Page Replacement Algorithm in C
 * SRM Institute of Science and Technology - OS Lab (21CSC202J)
 *
 * Compile: gcc e10.c -o e10
 * Run:     ./e10
 */

#include <stdio.h>

int main()
{
    int incomingStream[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1};
    int frames = 3;
    int pages = sizeof(incomingStream) / sizeof(incomingStream[0]);
    int temp[3];
    int pageFaults = 0, next = 0;       // next = frame holding the oldest page
    int m, n, hit;

    printf("Incoming \t Frame 1 \t Frame 2 \t Frame 3\n");
    for (m = 0; m < frames; m++)
        temp[m] = -1;

    for (m = 0; m < pages; m++)
    {
        hit = 0;
        for (n = 0; n < frames; n++)
        {
            if (incomingStream[m] == temp[n])
                hit = 1;
        }
        if (!hit)                       // page fault: replace the oldest page
        {
            temp[next] = incomingStream[m];
            next = (next + 1) % frames;
            pageFaults++;
        }
        printf("%d\t\t", incomingStream[m]);
        for (n = 0; n < frames; n++)
        {
            if (temp[n] != -1)
                printf(" %d\t\t", temp[n]);
            else
                printf(" - \t\t");
        }
        printf("%s\n", hit ? "(hit)" : "(fault)");
    }
    printf("\nTotal Page Faults:\t%d\n", pageFaults);

    return 0;
}
