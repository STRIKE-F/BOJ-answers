#include <iostream>

using namespace std;

class STnode
{
public:
    STnode *left;
    STnode *right;
    int range_left;
    int range_right;
    long long value;    
    STnode(long long value);
    ~STnode();
};


STnode::STnode(long long value)
{
    left = NULL;
    right = NULL;
    range_left = 0;
    range_right = 0;
    this->value = value;
}

STnode::~STnode()
{
}

class STree
{
public:
    STnode *root;
    STree(long long* values);
    ~STree();
    void update(int index, long long value);
    void update_rec(int index, long long value, STnode* cur);
    long long query(int start, int end);
    long long query_rec(int start, int end, STnode* cur);
};

STree::STree(long long* values)
{
    int size = (int)values[0];
    STnode *nodelist[size*2] = {NULL};
    
    for (int i = size*2-1; i >= size; i--)
    {
        nodelist[i] = new STnode(values[i-size+1]);
        nodelist[i]->range_right = i-size+1;
        nodelist[i]->range_left = i-size+1;
        //printf("index[%d] value[%lld]\n", nodelist[i]->range_left, nodelist[i]->value);
    }

    for (int i = size-1; i >= 1; i--)
    {
        nodelist[i] = new STnode(0);
        nodelist[i]->left = nodelist[i*2];
        nodelist[i]->right = nodelist[i*2+1];
        nodelist[i]->range_right = nodelist[i]->right->range_right;
        nodelist[i]->range_left = nodelist[i]->left->range_left;
        nodelist[i]->value = nodelist[i]->left->value + nodelist[i]->right->value;
        //printf("node [%d~%d] left [%d~%d] right [%d~%d]\n", nodelist[i]->range_left, nodelist[i]->range_right, nodelist[i]->left->range_left, nodelist[i]->left->range_right, nodelist[i]->right->range_left, nodelist[i]->right->range_right);
    }
    root = nodelist[1];
    
}

void STree::update_rec(int index, long long value, STnode* cur)
{
    if (cur->range_left == cur->range_right)
    {
        cur->value = value;
    }
    else if (abs(cur->range_left - index) > abs(cur->range_right - index))
    {
        update_rec(index, value, cur->right);
        cur->value = cur->left->value + cur->right->value;
    }
    else
    {
        update_rec(index, value, cur->left);
        cur->value = cur->left->value + cur->right->value;
    }
}

void STree::update(int index, long long value)
{
    update_rec(index, value, root);
}

long long STree::query_rec(int start, int end, STnode* cur)
{
    //printf("query %d to %d\n", start, end);
    if (cur->range_left == start && cur->range_right == end)
    {
        //printf("value: %lld\n", cur->value);
        return cur->value;
    }
    else if (cur->left->range_right >= end)
    {
        return query_rec(start, end, cur->left);
    }
    else if (cur->right->range_left <= start)
    {
        return query_rec(start, end, cur->right);
    }
    else
    {
        return query_rec(start, cur->left->range_right, cur->left) + query_rec(cur->right->range_left, end, cur->right);
    }
}

long long STree::query(int start, int end)
{
    return query_rec(start, end, root);
}

STree::~STree()
{

}

int main()
{
    int tmpsize, chg, qry;
    int cnt;
    int mod, p1, p2;

    scanf("%d %d %d", &tmpsize, &chg, &qry);
    cnt = chg + qry;

    int size = 1;
    while (tmpsize > size)
    {
        size *= 2;
    }

    long long values[size+1] = {0};
    values[0] = size;

    for (int i = 1; i <= tmpsize; i++)
    {
        scanf("%lld", &(values[i]));
    }

    STree *tree= new STree(values);

    for (int i = 0; i < cnt; i++)
    {
        scanf("%d %d %d", &mod, &p1, &p2);
        if (mod == 1)
        {
            tree->update(p1, p2);
        }
        else
        {
            printf("%lld\n", tree->query(p1,p2));
        }
    }

    return 0;
}
