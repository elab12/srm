/*
 * Experiment 12 - Part B: Worst Fit Memory Management Policy in C
 * SRM Institute of Science and Technology - OS Lab (21CSC202J)
 *
 * Compile: gcc e12b.c -o e12b
 * Run:     ./e12b
 */

#include <stdio.h>

void implimentWorstFit(int blockSize[], int blocks, int processSize[], int processes)
{
    // This will store the block id of the allocated block to a process
    int allocation[processes];

    // initially assigning -1 to all allocation indexes (nothing allocated)
    for (int i = 0; i < processes; i++)
        allocation[i] = -1;

    // pick each process and find the largest block available
    for (int i = 0; i < processes; i++)
    {
        int indexPlaced = -1;
        for (int j = 0; j < blocks; j++)
        {
            if (blockSize[j] >= processSize[i])
            {
                // place it at the first block fit to accommodate the process
                if (indexPlaced == -1)
                    indexPlaced = j;
                // if any later block is larger than the current block where
                // the process is placed, change indexPlaced (largest hole = worst fit)
                else if (blockSize[indexPlaced] < blockSize[j])
                    indexPlaced = j;
            }
        }

        // If successfully able to find a block for the process
        if (indexPlaced != -1)
        {
            allocation[i] = indexPlaced;
            // reduce available memory in the block
            blockSize[indexPlaced] -= processSize[i];
        }
    }

    printf("\nProcess No.\tProcess Size\tBlock no.\n");
    for (int i = 0; i < processes; i++)
    {
        printf("%d \t\t %d \t\t ", i + 1, processSize[i]);
        if (allocation[i] != -1)
            printf("%d\n", allocation[i] + 1);
        else
            printf("Not Allocated\n");
    }
}

int main()
{
    int blockSize[] = {100, 50, 30, 120, 35};
    int processSize[] = {40, 10, 30, 60};
    int blocks = sizeof(blockSize) / sizeof(blockSize[0]);
    int processes = sizeof(processSize) / sizeof(processSize[0]);

    implimentWorstFit(blockSize, blocks, processSize, processes);
    return 0;
}
