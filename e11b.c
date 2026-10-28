/*
 * Experiment 11 - Part B: LFU (Least Frequently Used) Page Replacement Algorithm in C
 * Ties are broken by the oldest page (FIFO)
 * SRM Institute of Science and Technology - OS Lab (21CSC202J)
 *
 * Compile: gcc e11b.c -o e11b
 * Run:     ./e11b
 */

#include <stdio.h>

int main()
{
    int incomingStream[] = {1, 2, 3, 2, 1, 5, 2, 1, 6, 2, 5, 6, 3, 1, 3};
    int n = sizeof(incomingStream) / sizeof(incomingStream[0]);
    int frames = 3;
    int page[3], freq[3], arrival[3];
    int occupied = 0, pagefault = 0;

    printf("Page\t Frame1 \t Frame2 \t Frame3\n");
    for (int i = 0; i < n; i++)
    {
        int hit = -1;
        printf("%d: \t\t", incomingStream[i]);

        for (int j = 0; j < occupied; j++)
            if (page[j] == incomingStream[i])
                hit = j;

        if (hit != -1)                      // HIT: increase use count
        {
            freq[hit]++;
        }
        else if (occupied < frames)         // empty frame available
        {
            page[occupied] = incomingStream[i];
            freq[occupied] = 1;
            arrival[occupied] = i;
            occupied++;
            pagefault++;
        }
        else                                // replace least frequently used page
        {
            int victim = 0;
            for (int j = 1; j < frames; j++)
            {
                if (freq[j] < freq[victim] ||
                   (freq[j] == freq[victim] && arrival[j] < arrival[victim]))
                    victim = j;
            }
            page[victim] = incomingStream[i];
            freq[victim] = 1;
            arrival[victim] = i;
            pagefault++;
        }

        for (int j = 0; j < occupied; j++)
            printf("%d\t\t", page[j]);
        printf("\n");
    }

    printf("\nPage Fault: %d\n", pagefault);
    return 0;
}
