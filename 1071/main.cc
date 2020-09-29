// 1071 소트

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <iterator>
#include <algorithm>

std::vector<int>
line_to_ints(std::string_view line, int max_ints)
{
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

std::deque<int>
sort_int_array(const std::vector<int> &int_array)
{
    std::deque<int> sorted_array;
    std::copy(int_array.begin(), int_array.end(), std::back_inserter(sorted_array));
    std::sort(sorted_array.begin(), sorted_array.end());
    return sorted_array;
}

std::vector<int>
rearrange_int_array(const std::vector<int> &int_array)
{
    auto sorted_array = sort_int_array(int_array);
    std::vector<int> rearranged_array;
    rearranged_array.reserve(sorted_array.size());
    int bigger_than = -1;

    while (!sorted_array.empty())
    {
        auto itr_to_push = std::upper_bound(sorted_array.begin(), sorted_array.end(), bigger_than);
        if (itr_to_push == sorted_array.end())
        {
            // could not pick any number
            // return the last element into the sorted_array
            // and pick the number bigger than the popped one next time
            bigger_than = rearranged_array.back();
            auto itr_to_return = std::lower_bound(sorted_array.begin(), sorted_array.end(), bigger_than);
            sorted_array.insert(itr_to_return, bigger_than);
            rearranged_array.pop_back();
        }
        else if (!rearranged_array.empty() && *itr_to_push == rearranged_array.back() + 1)
        {
            // tried to pick n = arr[last] + 1
            bigger_than = rearranged_array.back() + 1;
        }
        else
        {
            rearranged_array.push_back(*itr_to_push);
            sorted_array.erase(itr_to_push);
            bigger_than = -1;
        }
    }

    return rearranged_array;
}

int
main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::string param_line;
    std::getline(std::cin, param_line);
    auto param = line_to_ints(param_line, 1);
    int num_of_ints = param.at(0);

    std::string array_input;
    std::getline(std::cin, array_input);
    auto int_array = line_to_ints(array_input, num_of_ints);

    auto rearranged_int_array = rearrange_int_array(int_array);
    for (const auto i : rearranged_int_array)
    {
        std::cout << i << " ";
    }

    return 0;
}