#include <stdio.h>
#include <stdlib.h>

int main()
{
	int row, col;
	int *first = NULL, *second = NULL;
	char *line = NULL;
	int i, j;
	scanf("%d %d", &row, &col);
	line = (char*)malloc(sizeof(char)*col * 2 + 1);
	for (i = 0; i <= row; i++)
	{
		fgets(line, col * 2 + 1, stdin);
		for (j = 0; j < col; j++)
		{
			if (line[j * 2] == '1')
			{
				if (first == NULL)
				{
					first = (int*)malloc(sizeof(int) * 2);
					first[0] = i;
					first[1] = j;
				}
				else
				{
					second = (int*)malloc(sizeof(int) * 2);
					second[0] = i;
					second[1] = j;
				}
			}
		}
	}
	printf("%d", abs(second[1] - first[1]) + abs(second[0] - first[0]));
	return 0;
}
