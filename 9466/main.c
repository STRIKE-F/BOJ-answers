/* 9466 팀 프로젝트 */

#include <stdio.h>
#include <stdlib.h>

int input_width, *input_list, not_cycled, not_cycled_tmp;
char *checked_list;

int* readline()
{
    int *list = (int*)malloc(sizeof(int) * input_width);
    for (int i = 0; i < input_width; i++)
    {
        scanf("%d", list + i);
    }
    return list;
}

int check_cycle(int i)
{
    if (checked_list[i-1] == 1)
    {
        return i;
    }
    checked_list[i-1] = 1;
    int rec_result = check_cycle(input_list[i-1]);
    if (rec_result == 0)
    {
        return 0;
    }
    else if (rec_result == i)
    {
        not_cycled_tmp += 1;
        //printf("%d\n", i);
        not_cycled -= not_cycled_tmp;
        return 0;
    }
    else
    {
        not_cycled_tmp += 1;
        //printf("%d<-", i);
        return rec_result;
    }
}

int main()
{
    int case_count;
    scanf("%d", &case_count);
    for (int i = 0; i < case_count; i++)
    {
        scanf("%d", &input_width);
        not_cycled = input_width;
        input_list = readline();
        checked_list = (char*)calloc(input_width, sizeof(char));
        for (int j = 0; j < input_width; j++)
        {
            if (checked_list[j] == 1)
            {
                continue;
            }
            not_cycled_tmp = 0;
            check_cycle(j+1);
            //printf("\n");
        }
        free(input_list);
        free(checked_list);
        printf("%d\n", not_cycled);
    }
    return 0;
}
