// 2869 달팽이는 올라가고 싶다

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <cmath>

typedef int dtype;

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

dtype
calculate_days_to_reach(const dtype speed_climb, const dtype speed_drop, const dtype distance)
{
    dtype days = 1;
    double distance_left(distance);
    double distance_per_day(speed_climb - speed_drop);

    // if snail can't reach the top at the first day
    if (speed_climb < distance)
    {
        // initial climbup at first day
        distance_left -= speed_climb;
        // drop and climbup until next day
        days += std::ceil(distance_left / distance_per_day);
    }

    return days;
}

int
main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string line;
    std::getline(std::cin, line);
    auto nums = line_to_nums<dtype>(line, 3);

    dtype speed_climb = nums.at(0);
    dtype speed_drop = nums.at(1);
    dtype distance = nums.at(2);

    dtype days_to_reach = calculate_days_to_reach(speed_climb, speed_drop, distance);
    std::cout << days_to_reach;

    return 0;
}