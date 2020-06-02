/* 1753 최단 경로 */

#include <stdio.h>
#include <stdlib.h>

typedef struct edge
{
    int weight;
    int dest;
} edge;

typedef struct vertex
{
    int edges;
    int edgemax;
    int heap_pos;
    int weight;
    edge* edgelist;
} vertex;

vertex vlist[20000];
vertex *queue[20001];

int heapsize = 0;

void heapify()
{
    int cur = 1;
    vertex *left, *right;
    while (1)
    {
        queue[heapsize] = NULL;
        if (cur*2 > heapsize)
        {
            left = NULL;
        }
        else
        {
            left = queue[cur*2];
        }

        if (cur*2 + 1 > heapsize)
        {
            right = NULL;
        }
        else
        {
            right = queue[cur*2+1];
        }
        
        if (left == NULL && right == NULL)
        {
            break;
        }
        else if (right == NULL)
        {
            if (left->weight < queue[cur]->weight)
            {
                queue[heapsize] = queue[cur];
                queue[cur] = queue[cur*2];
                queue[cur]->heap_pos = cur;
                cur *= 2;
                queue[cur] = queue[heapsize];
                queue[cur]->heap_pos = cur;
            }
            else
            {
                break;
            }
        }
        else
        {
            if (left->weight < queue[cur]->weight && right->weight < queue[cur]->weight)
            {
                queue[heapsize] = queue[cur];
                if (left->weight < right->weight)
                {
                    queue[cur] = queue[cur*2];
                    queue[cur]->heap_pos = cur;
                    cur *= 2;
                    queue[cur] = queue[heapsize];
                    queue[cur]->heap_pos = cur;
                }
                else
                {
                    queue[cur] = queue[cur*2+1];
                    queue[cur]->heap_pos = cur;
                    cur *= 2;
                    cur += 1;
                    queue[cur] = queue[heapsize];
                    queue[cur]->heap_pos = cur;
                }
            }
            else if (left->weight >= queue[cur]->weight && right->weight >= queue[cur]->weight)
            {
                break;
            }
            else if (left->weight < queue[cur]->weight)
            {
                queue[heapsize] = queue[cur];
                queue[cur] = queue[cur*2];
                queue[cur]->heap_pos = cur;
                cur *= 2;
                queue[cur] = queue[heapsize];
                queue[cur]->heap_pos = cur;
            }
            else if (right->weight < queue[cur]->weight)
            {
                queue[heapsize] = queue[cur];
                queue[cur] = queue[cur*2+1];
                queue[cur]->heap_pos = cur;
                cur *= 2;
                cur += 1;
                queue[cur] = queue[heapsize];
                queue[cur]->heap_pos = cur;
            }
        }
    }
}

void insert(vertex* v)
{
    heapsize += 1;
    int cur = heapsize;
    while (cur > 1 && v->weight < queue[cur/2]->weight)
    {
        queue[cur] = queue[cur/2];
        queue[cur]->heap_pos = cur;
        cur /= 2;
    }
    queue[cur] = v;
    v->heap_pos = cur;
}

void update(vertex* v)
{
    int cur = v->heap_pos;
    while (cur > 1 && v->weight < queue[cur/2]->weight)
    {
        queue[cur] = queue[cur/2];
        queue[cur]->heap_pos = cur;
        cur /= 2;
    }
    queue[cur] = v;
    v->heap_pos = cur;
}

vertex* dequeue()
{
    queue[0] = queue[1]; // queue[0] : rtn
    queue[0]->heap_pos = 0;
    queue[1] = queue[heapsize];
    heapify();
    heapsize -= 1; 
    return queue[0];
}

void query()
{
    vertex *v;
    while (heapsize > 0)
    {
        v = dequeue();
        for (int i = 0; i < v->edges; i++)
        {
            if (vlist[v->edgelist[i].dest].weight > v->weight + v->edgelist[i].weight)
            {
                vlist[v->edgelist[i].dest].weight = v->weight + v->edgelist[i].weight;
                if (vlist[v->edgelist[i].dest].heap_pos == 0)
                {
                    insert(vlist + v->edgelist[i].dest);
                }
                else
                {
                    update(vlist + v->edgelist[i].dest);
                }                
            }
        }
    }
}

int main()
{
    int max_v, max_e;
    int init_v;
    int v1, v2, wt;

    scanf("%d %d", &max_v, &max_e);
    scanf("%d", &init_v);
    for (int i = 0; i < max_v; i++)
    {
        vlist[i].edges = 0;
        vlist[i].edgemax = 1;
        vlist[i].heap_pos = 0;
        vlist[i].edgelist = (edge*)malloc(sizeof(edge));
        vlist[i].weight = 3000001;
        if (i == init_v-1)
        {
            vlist[i].weight = 0;
            insert(vlist + i);
        }
    }


    for (int i = 0; i < max_e; i++)
    {
        scanf("%d %d %d", &v1, &v2, &wt);
        vlist[v1-1].edges += 1;
        if (vlist[v1-1].edges > vlist[v1-1].edgemax)
        {
            vlist[v1-1].edgemax *= 2;
            vlist[v1-1].edgelist = (edge*)realloc(vlist[v1-1].edgelist, sizeof(edge) * vlist[v1-1].edgemax);
        }
        vlist[v1-1].edgelist[vlist[v1-1].edges-1].dest = v2 - 1;
        vlist[v1-1].edgelist[vlist[v1-1].edges-1].weight = wt; 
    }

    query();

    for (int i = 0; i < max_v; i++)
    {
        if (vlist[i].weight != 3000001)
        {
            printf("%d\n", vlist[i].weight);
        }
        else
        {
            printf("INF\n");
        }   
    }

    return 0;
}
