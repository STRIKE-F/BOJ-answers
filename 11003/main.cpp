#include <iostream>
#include <deque>
#include <memory>

int main()
{
    std::ios_base::sync_with_stdio(false);

    std::deque<int> scope;
    int N, L, tmp;
    std::cin >> N >> L;

    std::unique_ptr<int[]> arr(new int[N]);
    std::unique_ptr<int[]> arr_res(new int[N]);
    for (int i = 0; i < N; i++)
    {
        std::cin >> arr[i];
    }

    for (int i = 0; i < N; i++)
    {
        tmp = arr[i];
        if (scope.empty())
            ;
        else if (tmp <= arr[scope.front()])
        {
            scope.clear();
        }
        else
        {
            for (auto it = scope.rbegin(); it != scope.rend(); it++)
            {
                if (arr[*it] < tmp)
                {
                    scope.erase(it.base(), scope.end());
                    break;
                }
            }
        }
        scope.push_back(i);
        if (*scope.begin() <= i - L)
            scope.pop_front();

        arr_res[i] = arr[*scope.begin()];
    }

    for (int i = 0; i < N - 1; i++)
    {
        std::cout << arr_res[i] << " ";
    }
    std::cout << arr_res[N - 1];
    return 0;
}