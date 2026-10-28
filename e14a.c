/*
 * Experiment 14 - Part A: Indexed File Allocation Strategy in C
 * SRM Institute of Science and Technology - OS Lab (21CSC202J)
 *
 * Compile: gcc e14a.c -o e14a
 * Run:     ./e14a
 */

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int f[50] = {0}, index[50], i, n, j, c, k, ind, count;

    do
    {
        printf("Enter the index block: ");
        scanf("%d", &ind);

        if (ind < 0 || ind >= 50 || f[ind] == 1)
        {
            printf("%d index is already allocated \n", ind);
            continue;
        }

        f[ind] = 1;     /* the index block itself is now in use */

        printf("Enter no of blocks needed and no of files for the index %d on the disk : \n", ind);
        scanf("%d", &n);

        do
        {
            count = 0;
            for (i = 0; i < n; i++)
            {
                scanf("%d", &index[i]);
                if (index[i] >= 0 && index[i] < 50 && f[index[i]] == 0)
                    count++;
            }
            if (count != n)
            {
                printf("File in the index is already allocated \n");
                printf("Enter another file indexed\n");
            }
        } while (count != n);

        for (j = 0; j < n; j++)
            f[index[j]] = 1;

        printf("Allocated\n");
        printf("File Indexed\n");
        for (k = 0; k < n; k++)
            printf("%d-------->%d : %d\n", ind, index[k], f[index[k]]);

        printf("Do you want to enter more file(Yes - 1/No - 0)");
        scanf("%d", &c);
    } while (c == 1);

    return 0;
}
