/* 2961 도영이가 만든 맛있는 음식 */

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int cnt, cur = 1, max = 1;
    int *sum, *mul;
    int inp1, inp2, min;
    scanf("%d", &cnt);

    for (int i = 0; i < cnt; i++)
    {
        max *= 2;
    }

    mul = (int*)malloc(sizeof(int) * max);
    for (int i = 0; i < max; i++)
    {
        mul[i] = 1;
    }
    sum = (int*)calloc(max, sizeof(int));

    for (int i = 0; i < cnt; i++)
    {
        cur *= 2;
        scanf("%d %d", &inp1, &inp2);
        for (int j = 0; j < max; j++)
        {
            if (j % cur >= cur/2)
            {
                mul[j] *= inp1;
                sum[j] += inp2;
            }
        }
    }

    min = 1000000000;
    for (int i = 1; i < max; i++)
    {
        if (min > abs(mul[i] - sum[i]))
        {
            min = abs(mul[i] - sum[i]);
        }
    }

    printf("%d", min);
    return 0;
}
