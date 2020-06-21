#include <iostream>
#include <vector>
#include <memory>
#include <utility>

class node
{
public:
    bool visited;
    bool is_full;
    int degree;
    int closest_idx;
    std::vector<std::pair<node *, int>> connections;

    node()
    {
        visited = false;
        degree = 0;
        closest_idx = -1;
    }

    void add(node* n, int d)
    {
        connections.push_back(std::make_pair(n, d));
        degree++;
    }

    std::pair<node*, int> closest()
    {
        if (closest_idx == -1 || std::get<node*>(connections[closest_idx])->visited)
        {
            closest_idx = -1;
            int min_val = __INT_MAX__;
            for (int i = 0; i < degree; i++)
            {
                std::pair<node *, int> tmp = connections[i];
                node *tmp_node = std::get<node *>(tmp);
                int tmp_dist = std::get<int>(tmp);
                if (!tmp_node->visited && tmp_dist < min_val)
                {
                    closest_idx = i;
                    min_val = tmp_dist;
                }
            }
        }

        if (closest_idx == -1) // every connected nodes have been visited
            return std::make_pair(nullptr, 0);

        return connections[closest_idx];
    }
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int V, E;
    std::cin >> V >> E;
    std::unique_ptr<node[]> nodes = std::make_unique<node[]>(V + 1);
    std::vector<node*> MST;

    // build spanning graph
    int v1, v2, d;
    for (int i = 0; i < E; i++)
    {
        std::cin >> v1 >> v2 >> d;
        nodes[v1].add(&nodes[v2], d);
        nodes[v2].add(&nodes[v1], d);
    }

    // find MST and calculate total cost
    int res = 0;
    node *node_ptr = &nodes[1];
    node_ptr->visited = true;
    MST.push_back(node_ptr);

    while (true)
    {
        std::vector<node *> tmp_MST;
        std::pair<node *, int> closest = std::make_pair(nullptr, __INT_MAX__);
        std::pair<node *, int> tmp;

        for (auto n : MST)
        {
            tmp = n->closest();            
            if (std::get<node*>(tmp) != nullptr)
            {
                tmp_MST.push_back(n);
                if (std::get<int>(tmp) < std::get<int>(closest))
                {
                    closest = tmp;
                }
            }
        }

        MST = std::move(tmp_MST);
        node_ptr = std::get<node *>(closest);
        if (node_ptr == nullptr)
            break;
        else
        {
            node_ptr->visited = true;
            MST.push_back(node_ptr);
            res += std::get<int>(closest);
        }
    }

    std::cout << res;

    return 0;
}