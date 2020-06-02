/* 9935 문자열 폭발 */

#include <iostream>
#include <stack>
#include <deque>
#include <string>

int main()
{
    std::string line, bomb;
    std::cin >> line >> bomb;
    
    int bomb_len = bomb.length();

    std::stack<int> matched_until;
    matched_until.push(0);
    std::deque<char> string_deque;

    for (auto &c : line)
    {
        string_deque.push_back(c);
        if (c == bomb[matched_until.top()])
            matched_until.top()++;
        else if (c == bomb[0])
            matched_until.push(1);
        else
        {
            while (!matched_until.empty())
                matched_until.pop();
            matched_until.push(0);
        }

        if (matched_until.top() == bomb_len)
        {
            matched_until.pop();
            for (int i = 0; i < bomb_len; i++)
            {
                string_deque.pop_back();
            }
        }

        if (matched_until.empty())
            matched_until.push(0);
    }

    if (string_deque.empty())
        std::cout << "FRULA";
    else
        std::cout << std::string(string_deque.begin(), string_deque.end());
}
