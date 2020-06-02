/* 4485 녹색 옷 입은 애가 젤다지? */

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

vertex *vlist;
vertex **queue;

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

void add_edge(vertex *v, int dest, int weight)
{
    v->edges += 1;
    if (v->edges > v->edgemax)
    {
        v->edgemax *= 2;
        v->edgelist = (edge*)realloc(v->edgelist, sizeof(edge) * v->edgemax);
    }
    v->edgelist[v->edges-1].dest = dest;
    v->edgelist[v->edges-1].weight = weight; 
}

int main()
{
    int max_size;
    int wt;
    vertex *cur;
    int ind;
    int cnt = 0;

    while (1)
    {
        cnt += 1;
        scanf("%d", &max_size);
        if (max_size == 0)
        {
            break;
        }

        vlist = (vertex*)malloc(sizeof(vertex) * ((max_size) * (max_size) + 1));
        queue = (vertex**)malloc(sizeof(vertex*) * ((max_size) * (max_size) + 2));
        for (int i = 0; i < ((max_size) * (max_size) + 1); i++)
        {
            vlist[i].edges = 0;
            vlist[i].edgemax = 1;
            vlist[i].heap_pos = 0;
            vlist[i].edgelist = (edge*)malloc(sizeof(edge));
            vlist[i].weight = 156250;
            if (i == 0)
            {
                vlist[i].weight = 0;
            }
        }

        for (int r = 0; r < max_size; r++)
        {
            for (int c = 0; c < max_size; c++)
            {
                scanf("%d", &wt);
                ind = r*max_size+c;
                cur = vlist + ind;
                if (c > 0)
                {
                    add_edge(cur, ind - 1, wt);
                }
                if (c < max_size-1)
                {
                    add_edge(cur, ind + 1, wt);
                }
                if (r > 0)
                {
                    add_edge(cur, ind - max_size, wt);
                }
                if (r < max_size-1)
                {
                    add_edge(cur, ind + max_size, wt);
                }
                if (c == max_size-1 && r == max_size-1)
                {
                    add_edge(cur, (max_size) * (max_size), wt);
                }
            }
        }

        insert(vlist);
        query();

        printf("Problem %d: %d\n", cnt, vlist[(max_size) * (max_size)].weight);
        free(vlist);
        free(queue);
        heapsize = 0;
    }

    return 0;
}
