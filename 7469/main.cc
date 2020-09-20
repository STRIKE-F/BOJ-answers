// 7469 K번째 수

#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <iterator>
#include <algorithm>
#include <memory>

class MSTreeNode;

class Visitor
{
public:
    virtual void 
    visit(const MSTreeNode* const node) = 0;
};

class MSTreeNode
{
public:
    std::vector<int> sorted_ints;

    int range_from;
    int range_to;
    std::unique_ptr<MSTreeNode> left;
    std::unique_ptr<MSTreeNode> right;

    MSTreeNode(std::vector<int> &int_array, int range_from, int range_to)
    {
        this->range_from = range_from;
        this->range_to = range_to;

        int vector_size = range_to - range_from + 1;
        if (vector_size == 1)
        {
            sorted_ints.push_back(int_array.at(range_from - 1));
        }
        else
        {
            // child nodes have left/right half of this node's vector
            int range_mid = (range_from + range_to) / 2;
            this->left = std::make_unique<MSTreeNode>(int_array, range_from, range_mid);
            this->right = std::make_unique<MSTreeNode>(int_array, range_mid + 1, range_to);

            this->sorted_ints.reserve(vector_size);
            std::merge(this->left->sorted_ints.begin(), this->left->sorted_ints.end(),
                       this->right->sorted_ints.begin(), this->right->sorted_ints.end(),
                       std::back_inserter(this->sorted_ints));
        }
    }

    void
    accept(Visitor* visitor)
    {
        visitor->visit(this);
    }
};

class VisitorCollectNodesInQuery : public Visitor
{
private:
    int range_from;
    int range_to;
    std::vector<const MSTreeNode*> nodes_in_range;

public:
    VisitorCollectNodesInQuery(int range_from, int range_to)
    {
        this->range_from = range_from;
        this->range_to = range_to;
    }

    void
    visit(const MSTreeNode* const node) override
    {
        if (this->range_from <= node->range_from && node->range_to <= this->range_to)
        {
            // this node is 'in' range
            this->nodes_in_range.push_back(node);
        }
        else if (this->range_to < node->range_from || node->range_to < this->range_from)
        {
            // this node is 'out of' range
            return;
        }
        else
        {
            // this node partially overlaps with range
            node->left->accept(this);
            node->right->accept(this);
        }
    }

    void
    print_nodes()
    {
        for (const auto node : this->nodes_in_range)
        {
            std::cout << "[" << node->range_from << " ~ " << node->range_to << "]\n";
        }
    }

    std::vector<const MSTreeNode*>
    get_nodes_in_range()
    {
        return this->nodes_in_range;
    }
};

class MSTree
{
private:
    std::vector<const MSTreeNode*>
    find_nodes_in_range(int range_from, int range_to)
    {
        VisitorCollectNodesInQuery visitor(range_from, range_to);
        visitor.visit(this->root.get());
        // visitor.print_nodes();
        return visitor.get_nodes_in_range();
    }

    int
    count_ints_smaller_than(const std::vector<const MSTreeNode*> nodes, int x)
    {
        int count = 0;
        for (const auto node : nodes)
        {
            auto first_greater = std::lower_bound(node->sorted_ints.begin(), node->sorted_ints.end(), x);
            int smaller_int_count = first_greater - node->sorted_ints.begin();
            count += smaller_int_count;
        }

        return count;
    }

    bool
    find_int(const std::vector<const MSTreeNode*> nodes, int x)
    {
        for (const auto node : nodes)
        {
            if (std::binary_search(node->sorted_ints.begin(), node->sorted_ints.end(), x))
            {
                return true;
            }
        }
        return false;
    }

public:
    std::unique_ptr<MSTreeNode> root;
    
    MSTree(std::vector<int> &input_ints)
    {
        this->root = std::make_unique<MSTreeNode>(input_ints, 1, input_ints.size());
    }

    int 
    kth_number(const std::vector<int> &query)
    {
        // query.size() will always be 3!
        int range_from = query.at(0);
        int range_to = query.at(1);
        int k = query.at(2);

        auto nodes_in_range = this->find_nodes_in_range(range_from, range_to);

        int search_range_left = -1000000000;
        int search_range_right = 1000000000;
        int search_mid = 0;

        while (search_range_left != search_range_right)
        {
            search_mid = search_range_left + search_range_right;
            if (search_mid < 0 && search_mid % 2 != 0)
            {
                // if left+right is negative number,
                // mid / 2 is floored unlike that of positive number
                search_mid -= 1;
            }
            search_mid /= 2;

            int assumed_k = count_ints_smaller_than(nodes_in_range, search_mid);
            if (assumed_k < k - 1)
            {
                search_range_left = search_mid + 1;
            }
            else if (assumed_k > k - 1)
            {
                search_range_right = search_mid;
            }
            else 
            {
                // even if assumed_k == k - 1,
                // search_mid may not exist in the actual array!
                if (find_int(nodes_in_range, search_mid))
                {
                    // search_mid actually exists in the array!
                    break;
                }
                else
                {
                    // the actual 'x' should be bigger than search_mid
                    // because if x was to be smaller than search_mid,
                    // the assumed_k > k - 1
                    search_range_left = search_mid + 1;
                }
            }
        }

        // if the above loop does not break,
        // there's multiple search_mid in this array
        return search_mid;
    }
};

std::vector<int>
line_to_ints(std::string_view line, int max_ints) {
    std::vector<int> ints;
    ints.reserve(max_ints);
    std::string int_str;
    int int_index = 0;

    for (const char ch : line) 
    {
        if ('0' <= ch && ch <= '9') 
        {
            // numerals
            int_str += ch;
        }
        else if (ch == '-')
        {
            // negative operator
            int_str += ch;
        }
        else if (ch == ' ') 
        {
            // delimiter (space)
            int num = std::stoi(int_str);
            ints.push_back(num);
            int_index++;
            int_str.clear();
        }

        if (int_index == max_ints) 
        {
            // # of ints read has reached max_ints, so stop reading ints
            break;
        }
    }

    if (int_index < max_ints)
    {
        // end of line before reaching max ints
        ints.push_back(std::stoi(int_str));
    }

    return ints;
}

std::vector<std::vector<int>>
read_queries(int query_count)
{
    std::vector<std::vector<int>> queries;
    queries.reserve(query_count);
    for (int q = 0; q < query_count; q++)
    {
        std::string query_input;
        std::getline(std::cin, query_input);
        auto query = line_to_ints(query_input, 3);
        queries.push_back(query);
    }

    return queries;
}

int
main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::string params_line;
    std::getline(std::cin, params_line);
    auto params = line_to_ints(params_line, 2);
    int array_size = params[0];
    int query_count = params[1];

    std::string array_input;
    std::getline(std::cin, array_input);
    auto int_array = line_to_ints(array_input, array_size);
    MSTree tree(int_array);

    auto queries = read_queries(query_count);   
    for (const auto &query : queries)
    {
        std::cout << tree.kth_number(query) << "\n";
    }

    return 0;
}