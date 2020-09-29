// 2448 별 찍기 - 11

#include <iostream>
#include <vector>
#include <memory>

class StarMap
{
private:
    std::vector<std::vector<bool>> map;

public:
    StarMap(const int max_stars)
    {
        int map_width = max_stars * 2;
        map.reserve(map_width);

        for (int y = 0; y < max_stars; y++)
        {
            this->map.push_back(std::vector<bool>(map_width, false));
        }
    }

    void
    set(int x, int y)
    {
        this->map.at(y).at(x) = true;
    }

    std::vector<std::string>
    extract()
    {
        std::vector<std::string> map_in_string;
        map_in_string.reserve(map.size());

        for (auto &y : this->map)
        {
            std::string map_line;
            for (auto x : y)
            {
                if (x)
                {
                    map_line += '*';
                }
                else
                {
                    map_line += ' ';
                }
            }

            map_in_string.push_back(map_line);
        }

        return map_in_string;
    }
};


class Triangle
{
private:
    int size;
    int offset_x;
    int offset_y;
    std::unique_ptr<Triangle> subtriangle_up;
    std::unique_ptr<Triangle> subtriangle_left;
    std::unique_ptr<Triangle> subtriangle_right;

public:
    Triangle(const int size) : Triangle(size, 0, 0) {};

    Triangle(const int size, const int offset_x, const int offset_y)
    {
        this->size = size;
        this->offset_x = offset_x;
        this->offset_y = offset_y;

        if (size > 1)
        {
            int subtriangle_size = size / 2;
            int full_offset = size * 3;
            int half_offset = full_offset / 2;

            this->subtriangle_up = std::make_unique<Triangle>(subtriangle_size, offset_x + half_offset, offset_y);
            this->subtriangle_left = std::make_unique<Triangle>(subtriangle_size, offset_x, offset_y + half_offset);
            this->subtriangle_right = std::make_unique<Triangle>(subtriangle_size, offset_x + full_offset, offset_y + half_offset);
        }
    }

    void
    render(StarMap &map)
    {
        if (this->size > 1)
        {
            this->subtriangle_up->render(map);
            this->subtriangle_left->render(map);
            this->subtriangle_right->render(map);
        }
        else
        {
            int x = this->offset_x;
            int y = this->offset_y;
            map.set(x + 2, y);
            map.set(x + 1, y + 1);
            map.set(x + 3, y + 1);
            for (int o = 0; o < 5; o++)
            {
                map.set(x + o, y + 2);
            }
        }
    }
};


int
main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

    int num_of_star_lines;
    std::cin >> num_of_star_lines;
    // level is a line of triangles
    int max_size = num_of_star_lines / 3;

    Triangle full_triangle(max_size);
    StarMap map(num_of_star_lines);
    full_triangle.render(map);
    auto map_in_string = map.extract();
    for (auto &line : map_in_string)
    {
        std::cout << line << "\n";
    }

    return 0;
}