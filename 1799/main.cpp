#include <iostream>
#include <string>
#include <string_view>
#include <memory>

/* 
 * convert "0" to false, "1" to true
 */
inline bool
string_to_bool(std::string_view token)
{
    return (token == "1");
}

/*
 * parse string into tokens using the given delimiter
 * and store them in bool array
 */
std::unique_ptr<bool[]>
parse_line(std::string& line, std::string_view delimiter, int max_elems)
{
    auto ret = std::make_unique<bool[]>(max_elems);

    int idx = 0;
    int delim_pos = line.find(delimiter);
    while (delim_pos != std::string::npos)
    {
        std::string token = line.substr(0, delim_pos);
        ret[idx++] = string_to_bool(token);

        line.erase(0, delim_pos + delimiter.length());
        delim_pos = line.find(delimiter);
    }
    ret[idx] = string_to_bool(line);

    return ret;
}

int 
main()
{
    std::ios::sync_with_stdio(false);

    std::string tmp;
    std::getline(std::cin, tmp);
    const int size = std::stoi(tmp);

    auto board = std::make_unique<std::unique_ptr<bool[]>[]>(size);

    for (int i = 0; i < size; i++)
    {
        std::string line;
        std::getline(std::cin, line);
        board[i] = parse_line(line, " ", size);
    }

    // print board for debugging
    /*
    for (int r = 0; r < size; r++)
    {
        for (int c = 0; c < size; c++)
        {
            std::cout << board[r][c] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
     */

    int bishops = 0;

    while (true)
    {
        // create counting board
        auto count_board = std::make_unique<std::unique_ptr<int[]>[]>(size);
        for (int r = 0; r < size; r++)
        {
            count_board[r] = std::make_unique<int[]>(size);
        }

        // LD-to-RU diagonal
        // idx sum 0 ~ size - 1
        for (int i = 0; i < size; i++)
        {
            int disabled = 0;
            for (int r = i, c = 0; c <= i; r--, c++)
            {
                if (board[r][c])
                    disabled++;
            }

            for (int r = i, c = 0; c <= i; r--, c++)
            {
                if (board[r][c])
                    count_board[r][c] = disabled;
                else
                    count_board[r][c] = __INT_MAX__;
            }
        }

        // idx sum size ~ size * 2 - 2
        for (int i = size; i < size * 2 - 1; i++)
        {
            int disabled = 0;
            for (int r = size-1, c = i - r; c < size; r--, c++)
            {
                if (board[r][c])
                    disabled++;
            }

            for (int r = size-1, c = i - r; c < size; r--, c++)
            {
                if (board[r][c])
                    count_board[r][c] = disabled;
                else
                    count_board[r][c] = __INT_MAX__;
            }
        }

        // LU-to-RD diagonal
        // idx diff 0 ~ -(size-1)
        for (int i = 0; i < size; i++)
        {
            int disabled = 0;
            for (int r = 0, c = i; c < size; r++, c++)
            {
                if (board[r][c])
                    disabled++;
            }

            for (int r = 0, c = i; c < size; r++, c++)
            {
                if (board[r][c])
                    count_board[r][c] += disabled;
                // else is redundant: it is already INTMAX
            }
        }

        // idx diff 1 ~ (size-1)
        for (int i = 1; i < size; i++)
        {
            int disabled = 0;
            for (int r = i, c = 0; r < size; r++, c++)
            {
                if (board[r][c])
                    disabled++;
            }

            for (int r = i, c = 0; r < size; r++, c++)
            {
                if (board[r][c])
                    count_board[r][c] += disabled;
                // else is redundant: it is already INTMAX
            }
        }

        int min_r, min_c;
        int min_val = __INT_MAX__;
        for (int r = 0; r < size; r++)
        {
            for (int c = 0; c < size; c++)
            {
                if (count_board[r][c] < min_val)
                {
                    min_r = r;
                    min_c = c;
                    min_val = count_board[r][c];
                }
            }
        }

        // cannot place bishop anymore!
        if (min_val == __INT_MAX__)
            break;

        bishops++;
        board[min_r][min_c] = false;

        // disable blocks LD-RU diagonal from the bishop
        int idx_sum = min_r + min_c;
        if (idx_sum < size)
        {
            for (int r = idx_sum, c = 0; c <= idx_sum; r--, c++)
                board[r][c] = false;
        }
        else
        {
            for (int r = size-1, c = idx_sum - r; c < size; r--, c++)
                board[r][c] = false;
        }

        // disable blocks LU-RD diagonal from the bishop
        int idx_diff = min_r - min_c;
        if (idx_diff <= 0)
        {
            for (int r = 0, c = -idx_diff; c < size; r++, c++)
                board[r][c] = false;
        }
        else
        {
            for (int r = idx_diff, c = 0; r < size; r++, c++)
                board[r][c] = false;
        }


        // print board for debugging
        /*
        for (int r = 0; r < size; r++)
        {
            for (int c = 0; c < size; c++)
            {
                std::cout << board[r][c] << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
         */
        
    }

    std::cout << bishops;

    return 0;
}