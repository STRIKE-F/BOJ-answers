/* 5719 거의 최단 경로 */

#include <stdio.h>
#include <stdlib.h>

typedef struct edge
{
    int weight;
    int dept;
    int dest;
    char used;
} edge;

typedef struct vertex
{
    int edges;
    int backedges;
    int edgemax;
    int backedgemax;
    int heap_pos;
    int weight;
    edge **edgelist;
    edge **backedgelist;
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
    queue[heapsize] = NULL;
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
            if (v->edgelist[i]->used == 0 && vlist[v->edgelist[i]->dest].weight > v->weight + v->edgelist[i]->weight)
            {
                vlist[v->edgelist[i]->dest].weight = v->weight + v->edgelist[i]->weight;
                if (vlist[v->edgelist[i]->dest].heap_pos == 0)
                {
                    insert(vlist + v->edgelist[i]->dest);
                }
                else
                {
                    update(vlist + v->edgelist[i]->dest);
                }                
            }
        }
    }
}

void backquery()
{
    vertex *v;
    while (heapsize > 0)
    {
        v = dequeue();
        for (int i = 0; i < v->backedges; i++)
        {
            if (v->weight == vlist[v->backedgelist[i]->dept].weight + v->backedgelist[i]->weight)
            {
                v->backedgelist[i]->used = 1;
                //printf("out: edge from [%d] to [%d]\n", v->backedgelist[i]->dept, v->backedgelist[i]->dest);
                if (vlist[v->backedgelist[i]->dept].heap_pos == 0)
                {
                    insert(vlist + v->backedgelist[i]->dept);
                }
                else
                {
                    update(vlist + v->backedgelist[i]->dept);
                }                
            }
        }
    }
}

int main()
{
    int max_v, max_e;
    int init_v, dest_v;
    int v1, v2, wt;
    
    while (1)
    {
        scanf("%d %d", &max_v, &max_e);
        if (max_v == 0 && max_e == 0)
        {
            break;
        }
        vlist = (vertex*)malloc(sizeof(vertex) * max_v);
        queue = (vertex**)malloc(sizeof(vertex*) * (max_v+1));


        scanf("%d %d", &init_v, &dest_v);
        for (int i = 0; i < max_v; i++)
        {
            vlist[i].edges = 0;
            vlist[i].backedges = 0;
            vlist[i].edgemax = 1;
            vlist[i].backedgemax = 1;
            vlist[i].heap_pos = 0;
            vlist[i].edgelist = (edge**)malloc(sizeof(edge*));
            vlist[i].backedgelist = (edge**)malloc(sizeof(edge*));
            vlist[i].weight = 100001;
            if (i == init_v)
            {
                vlist[i].weight = 0;
                insert(vlist + i);
            }
        }


        for (int i = 0; i < max_e; i++)
        {
            scanf("%d %d %d", &v1, &v2, &wt);
            vlist[v1].edges += 1;
            if (vlist[v1].edges > vlist[v1].edgemax)
            {
                vlist[v1].edgemax *= 2;
                vlist[v1].edgelist = (edge**)realloc(vlist[v1].edgelist, sizeof(edge*) * vlist[v1].edgemax);
            }

            vlist[v2].backedges += 1;
            if (vlist[v2].backedges > vlist[v2].backedgemax)
            {
                vlist[v2].backedgemax *= 2;
                vlist[v2].backedgelist = (edge**)realloc(vlist[v2].backedgelist, sizeof(edge*) * vlist[v2].backedgemax);
            }

            vlist[v1].edgelist[vlist[v1].edges-1] = (edge*)malloc(sizeof(edge));
            vlist[v1].edgelist[vlist[v1].edges-1]->dept = v1;
            vlist[v1].edgelist[vlist[v1].edges-1]->dest = v2;
            vlist[v1].edgelist[vlist[v1].edges-1]->weight = wt;
            vlist[v1].edgelist[vlist[v1].edges-1]->used = 0;
            vlist[v2].backedgelist[vlist[v2].backedges-1] = vlist[v1].edgelist[vlist[v1].edges-1];

        }

        query();

        if (vlist[dest_v].weight != 100001)
        {
            for (int i = 0; i < max_v; i++)
            {
                vlist[i].heap_pos = 0;
            }
            insert(vlist + dest_v);
            backquery();

            for (int i = 0; i < max_v; i++)
            {
                vlist[i].heap_pos = 0;
                vlist[i].weight = 100001;
                if (i == init_v)
                {
                    vlist[i].weight = 0;
                }
            }
            insert(vlist + init_v);
            query();
        }


        if (vlist[dest_v].weight != 100001)
        {
            printf("%d\n", vlist[dest_v].weight);
        }
        else
        {
            printf("-1\n");
        }   

        free(vlist);
        free(queue);
        heapsize = 0;
    }

    return 0;
}
