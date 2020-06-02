#include <stdio.h>
#include <stdlib.h>

int main()
{
    int repeat;
    int x1, y1, r1, x2, y2, r2;
    int dxs, dys, rss, rds, dps, res;
    scanf("%d", &repeat);

    for (int i = 0; i < repeat; i++)
    {
        scanf("%d %d %d %d %d %d", &x1, &y1, &r1, &x2, &y2, &r2);
        dxs = abs(x2-x1) * abs(x2-x1);
        dys = abs(y2-y1) * abs(y2-y1);
        dps = dxs + dys;
        rss = (r2+r1) * (r2+r1);
        rds = abs(r2-r1) * abs(r2-r1);
        if (dps > rss)
        {
            res = 0;
        }
        else if (dps == rss)
        {
            res = 1;
        }
        else
        {
            if (dps > rds)
            {
                res = 2;
            }
            else if (dps == rds)
            {   
                if (dps == 0)
                {
                    res = -1;
                }
                else
                {
                    res = 1;
                }
            }
            else if (dps < rds)
            {
                res = 0;
            }
            
            
        }
        
        
        printf("%d\n", res);
    }
    return 0;
}
