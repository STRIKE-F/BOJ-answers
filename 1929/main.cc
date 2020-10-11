// 1929 소수 구하기

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
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

std::vector<dtype>
collect_primes_not_greater_than(const dtype num)
{
    std::vector<dtype> primes;
    primes.push_back(2);

    // find primes >2 and <=num
    for (dtype prime_candidate = 3; prime_candidate <= num; prime_candidate++)
    {
        dtype prime_candidate_sqrt = std::sqrt(prime_candidate);
        for (const auto prime : primes)
        {
            // num is prime
            if (prime > prime_candidate_sqrt)
            {
                primes.push_back(prime_candidate);
                break;
            }

            // num is divisible by prime!
            if (prime_candidate % prime == 0)
            {
                break;
            }
        }
    }

    return primes;
}

std::vector<dtype>
collect_primes_in_range(const dtype range_min, const dtype range_max)
{
    auto primes = collect_primes_not_greater_than(range_max);

    std::vector<dtype> primes_in_range;
    std::copy_if(primes.begin(), primes.end(), std::back_inserter(primes_in_range),
                 [range_min](const auto prime) { return prime >= range_min; });

    return primes_in_range;
}

std::string
collect_vector_into_string(const std::vector<dtype> &nums)
{
    std::string text;
    for (const auto num : nums)
    {
        text += std::to_string(num);
        text += '\n';
    }

    return text;
}

int
main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string param_line;
    std::getline(std::cin, param_line);
    auto params = line_to_nums<dtype>(param_line, 2);
    dtype range_min = params.at(0);
    dtype range_max = params.at(1);

    auto primes_in_range = collect_primes_in_range(range_min, range_max);
    std::cout << collect_vector_into_string(primes_in_range);

    return 0;
}