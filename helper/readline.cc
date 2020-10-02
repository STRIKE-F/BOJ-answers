#include <iostream>
#include <vector>
#include <string>
#include <sstream>

template <typename T>
std::vector<T>
line_to_nums(const std::string line, int max_nums = 0, const char delimiter=' ')
{
    max_nums = std::max(0, max_nums);
    std::vector<T> nums;
    nums.reserve(max_nums);

    std::istringstream line_stream(line);
    T num;

    while (line_stream >> num) 
    {
        nums.push_back(num);
        if (max_nums != 0 && nums.size() == max_nums)
        {
            break;
        }
    }

    return nums;
}

int
main()
{
    std::string line;
    std::getline(std::cin, line);
    auto nums = line_to_nums<double>(line);
    for (auto num : nums)
    {
        std::cout << num << " ";
    }

    return 0;
}