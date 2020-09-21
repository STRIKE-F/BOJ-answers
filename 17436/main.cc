// 17436 소수의 배수

#include <iostream>
#include <vector>
#include <bitset>

std::vector<int>
line_to_ints(std::string_view line, const int max_ints) 
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
            ints.push_back(std::stoi(int_str));
            int_index++;
            int_str.clear();
        }

        if (int_index == max_ints) 
        {
            // # of ints read has reached max_ints, so stop read ints
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

std::vector<long long>
line_to_llongs(std::string_view line, const int max_llongs) 
{
    std::vector<long long> llongs;
    llongs.reserve(max_llongs);
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
            llongs.push_back(std::stoll(int_str));
            int_index++;
            int_str.clear();
        }

        if (int_index == max_llongs) 
        {
            // # of llongs read has reached max_llongs, so stop read llongs
            break;
        }
    }

    if (int_index < max_llongs) 
    {
        // end of line before reaching max llongs
        llongs.push_back(std::stoll(int_str));
    }

    return llongs;
}

std::vector<std::vector<long long>>
collect_multiples_by_num_of_primes(std::vector<int> &primes)
{
    int primes_size = primes.size();
    int num_of_multiples = (1 << primes_size) - 1;
    std::vector<std::vector<long long>> multiples(primes_size);

    for (int i = 1; i <= num_of_multiples; i++)
    {
        std::bitset<10> include_or_not(i);
        long long multiple = 1;

        for (int radix = 0; radix < primes_size; radix++)
        {
            if (include_or_not[radix])
            {
                multiple *= primes.at(radix);
            }
        }

        int active_bits = include_or_not.count();
        multiples.at(active_bits - 1).push_back(multiple);
    }

    return multiples;
}

long long
count_divisible_llongs(const long long max_llong, std::vector<int> &primes)
{
    auto multiples = collect_multiples_by_num_of_primes(primes);
    bool odd_number_of_primes_used = true;
    long long num_of_divisibles = 0LL;
    for (const auto &multiples_with_same_primes_used : multiples)
    {
        long long num_of_divisibles_with_duplicates = 0LL;
        for (const auto multiple : multiples_with_same_primes_used)
        {
            num_of_divisibles_with_duplicates += max_llong / multiple;
        }
        if (odd_number_of_primes_used)
        {
            num_of_divisibles += num_of_divisibles_with_duplicates;
            odd_number_of_primes_used = false;
        }
        else
        {
            num_of_divisibles -= num_of_divisibles_with_duplicates;
            odd_number_of_primes_used = true;
        }
    }

    return num_of_divisibles;
}

int
main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::string params_line;
    std::getline(std::cin, params_line);
    auto params = line_to_llongs(params_line, 2);
    int num_of_primes = params[0];
    long long max_llong = params[1];

    std::string primes_line;
    std::getline(std::cin, primes_line);
    auto primes = line_to_ints(primes_line, num_of_primes);

    long long num_of_divisibles = count_divisible_llongs(max_llong, primes);
    std::cout << num_of_divisibles;

    return 0;
}