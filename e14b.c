/*
 * Experiment 14 - Part B: Sequential File Allocation Strategy in C
 * SRM Institute of Science and Technology - OS Lab (21CSC202J)
 *
 * Compile: gcc e14b.c -o e14b
 * Run:     ./e14b
 */

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int f[50] = {0}, j, c, k, st, len, count;

    printf("Files Allocated are : \n");
    do
    {
        count = 0;
        printf("Enter starting block and length of files: ");
        scanf("%d%d", &st, &len);

        if (st >= 0 && len > 0 && st + len <= 50)
            for (k = st; k < (st + len); k++)
                if (f[k] == 0)
                    count++;

        if (len > 0 && len == count)
        {
            for (j = st; j < (st + len); j++)
            {
                f[j] = 1;
                printf("%d\t%d\n", j, f[j]);
            }
            printf("The file is allocated to disk\n");
        }
        else
            printf("The file is not allocated \n");

        printf("Do you want to enter more file(Yes - 1/No - 0)");
        scanf("%d", &c);
    } while (c == 1);

    return 0;
}
