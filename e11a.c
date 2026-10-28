/*
 * Experiment 11 - Part A: LRU (Least Recently Used) Page Replacement Algorithm in C
 * SRM Institute of Science and Technology - OS Lab (21CSC202J)
 *
 * Compile: gcc e11a.c -o e11a
 * Run:     ./e11a
 */

#include <stdio.h>
#include <limits.h>

int checkHit(int incomingPage, int queue[], int occupied)
{
    for (int i = 0; i < occupied; i++)
    {
        if (incomingPage == queue[i])
            return 1;
    }
    return 0;
}

void printFrame(int queue[], int occupied)
{
    for (int i = 0; i < occupied; i++)
        printf("%d\t\t", queue[i]);
}

int main()
{
    int incomingStream[] = {1, 2, 3, 2, 1, 5, 2, 1, 6, 2, 5, 6, 3, 1, 3};
    int n = sizeof(incomingStream) / sizeof(incomingStream[0]);
    int frames = 3;
    int queue[3];
    int distance[3];
    int occupied = 0;
    int pagefault = 0;

    printf("Page\t Frame1 \t Frame2 \t Frame3\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d: \t\t", incomingStream[i]);

        // page already in a frame: HIT
        if (checkHit(incomingStream[i], queue, occupied))
        {
            printFrame(queue, occupied);
        }
        // filling when frame(s) is/are empty
        else if (occupied < frames)
        {
            queue[occupied] = incomingStream[i];
            pagefault++;
            occupied++;
            printFrame(queue, occupied);
        }
        else
        {
            int max = INT_MIN;
            int index = 0;

            // get LRU distance for each item in frame
            for (int j = 0; j < frames; j++)
            {
                distance[j] = 0;
                // traverse in reverse direction to find
                // at what distance the frame item occurred last
                for (int k = i - 1; k >= 0; k--)
                {
                    ++distance[j];
                    if (queue[j] == incomingStream[k])
                        break;
                }

                // find frame item with max distance (least recently used)
                if (distance[j] > max)
                {
                    max = distance[j];
                    index = j;
                }
            }
            queue[index] = incomingStream[i];
            printFrame(queue, occupied);
            pagefault++;
        }
        printf("\n");
    }

    printf("\nPage Fault: %d\n", pagefault);
    return 0;
}
