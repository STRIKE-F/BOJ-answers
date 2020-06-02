#include <stdio.h>
#include <stdlib.h>

int main()
{
    int input_list[3], res = 0, max = 15 * 19 * 28;
    scanf("%d %d %d", input_list, input_list+1, input_list+2);
    input_list[0] %= 15;
    input_list[1] %= 28;
    input_list[2] %= 19;
    while(1)
    {
        res += 1;
        if (res % 28 == input_list[1] && res % 19 == input_list[2] && res % 15 == input_list[0])
        {
            printf("%d", res);
            return 0;
        }
        else if (res > max)
        {
            return 0;
        }
    }
}
