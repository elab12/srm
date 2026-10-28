#include <stdio.h>

int main()
{
    int i = 0;
    int sum = 0;
    printf("Starting GDB debugging session...\n");
    for (i = 1; i <= 5; i++)
    {
        sum = sum + i;    /* Line 10: Breakpoint (gdb) b 10 */
        printf("i = %d, sum = %d\n", i, sum);
    }
    printf("Final sum = %d\n", sum);
    return 0;
}
