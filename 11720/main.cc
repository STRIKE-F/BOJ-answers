// 11720 숫자의 합

#include <iostream>
#include <string>
#include <string_view>

typedef int dtype;

dtype
reduce_line(std::string_view line, dtype nums)
{
    dtype sum = 0;
    for (const auto ch : line)
    {
        sum += ch;
    }
    // each ch is i + '0', so subtract the sum of '0's
    sum -= nums * '0';

    return sum;
}

int
main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    dtype nums;
    std::cin >> nums;
    std::string line;
    std::cin >> line;

    dtype sum = reduce_line(line, nums);
    std::cout << sum;

    return 0;
}
