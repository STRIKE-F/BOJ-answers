// 11563 연돌이와 고잠녀

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <algorithm>

template <typename T>
class Coordinates
{
public:
    T x;
    T y;

    Coordinates()
    {
        this->x = 0;
        this->y = 0;
    }

    Coordinates(const T x, const T y)
    {
        this->x = x;
        this->y = y;
    }

    const friend bool
    operator>(const Coordinates &lhs, const Coordinates &rhs)
    {
        if (lhs.x == rhs.x)
        {
            return lhs.y > rhs.y;
        }
        else
        {
            return lhs.x > rhs.x;
        }
    }
};

template <typename T>
T
distance_between(Coordinates<T> &coord_1, Coordinates<T> &coord_2)
{
    T x_diff = coord_2.x - coord_1.x;
    T y_diff = coord_2.y - coord_1.y;
    T length_sq = std::pow(x_diff, 2) + std::pow(y_diff, 2);
    T length = std::sqrt(length_sq);
    return length;
}

template <typename T>
class Vector
{
public:
    T x;
    T y;

    Vector()
    {
        this->x = 0;
        this->y = 0;
    }

    Vector(const T x, const T y)
    {
        this->x = x;
        this->y = y;
    }

    Vector(const Coordinates<T> &A, const Coordinates<T> &B)
    {
        this->x = B.x - A.x;
        this->y = B.y - A.y;
    }

    const friend T
    operator*(const Vector &lhs, const Vector &rhs)
    {
        return lhs.x * rhs.x + lhs.y * rhs.y;
    }

    T
    size()
    {
        return std::sqrt(std::pow(this->x, 2) + std::pow(this->y, 2));
    }
};

template <typename T>
class Road
{
public:
    Coordinates<T> A;
    Coordinates<T> B;

    Road(std::vector<T> &coordinates) : Road(Coordinates(coordinates.at(0), coordinates.at(1)), Coordinates(coordinates.at(2), coordinates.at(3)))
    {
        ;
    }

    Road(const Coordinates<T> &A, const Coordinates<T> &B)
    {
        this->A = A;
        this->B = B;

        if (this->A > this->B)
        {
            std::swap(this->A, this->B);
        }
    }

    T
    get_length()
    {
        return distance_between(this->A, this->B);
    }

    Vector<T>
    get_vector()
    {
        return Vector(this->A, this->B);
    }
};

template <typename T>
T
distance_between(Road<T> &road, Coordinates<T> &coord)
{
    T distance = -1;

    Vector vec(road.A, road.B);
    Vector vec_a(road.A, coord);
    T dot_a = vec * vec_a;

    Vector vec_r(road.B, road.A);
    Vector vec_b(road.B, coord);
    T dot_b = vec_r * vec_b;

    // dot_* > 0 means angle < 90
    // if angle == 90, it is same as end-to-end connection
    if (dot_a > 0 && dot_b > 0)
    {
        T hypotenuse = vec_a.size();
        T base = dot_a / vec.size();
        distance = std::sqrt(std::pow(hypotenuse, 2) - std::pow(base, 2));
    }
    return distance;
}

template <typename T>
T
distance_between(Road<T> &road_1, Road<T> &road_2)
{
    std::vector<T> calculated_distances;
    calculated_distances.reserve(8);
    T distance;

    // distances between ends of each road
    distance = distance_between(road_1.A, road_2.A);
    calculated_distances.push_back(distance);
    distance = distance_between(road_1.A, road_2.B);
    calculated_distances.push_back(distance);
    distance = distance_between(road_1.B, road_2.A);
    calculated_distances.push_back(distance);
    distance = distance_between(road_1.B, road_2.B);
    calculated_distances.push_back(distance);

    // distances between non-end and end of each road
    distance = distance_between(road_1, road_2.A);
    if (distance >= 0)
    {
        calculated_distances.push_back(distance);
    }
    distance = distance_between(road_1, road_2.B);
    if (distance >= 0)
    {
        calculated_distances.push_back(distance);
    }
    distance = distance_between(road_2, road_1.A);
    if (distance >= 0)
    {
        calculated_distances.push_back(distance);
    }
    distance = distance_between(road_2, road_1.B);
    if (distance >= 0)
    {
        calculated_distances.push_back(distance);
    }

    return *std::min_element(calculated_distances.begin(), calculated_distances.end());
}

template <typename T>
T
distance_between(std::vector<Road<T>> &area_1, std::vector<Road<T>> &area_2)
{
    // init distance
    T distance = distance_between(area_1.at(0), area_2.at(0));

    for (auto &road_1 : area_1)
    {
        for (auto &road_2 : area_2)
        {
            distance = std::min(distance, distance_between(road_1, road_2));
        }
    }

    return distance;
}

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
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    typedef double coord_type;

    std::string param_line;
    std::getline(std::cin, param_line);
    auto params = line_to_nums<int>(param_line);

    // roads to Sinchon
    int num_of_roads_sinchon = params.at(0);
    std::vector<Road<coord_type>> map_sinchon;
    map_sinchon.reserve(num_of_roads_sinchon);
    for (int i = 0; i < num_of_roads_sinchon; i++)
    {
        std::string coord_line;
        std::getline(std::cin, coord_line);
        auto coords = line_to_nums<coord_type>(coord_line);
        Road<coord_type> road(coords);
        map_sinchon.push_back(road);
    }

    // roads to Anam
    int num_of_roads_anam = params.at(1);
    std::vector<Road<coord_type>> map_anam;
    map_anam.reserve(num_of_roads_anam);
    for (int i = 0; i < num_of_roads_anam; i++)
    {
        std::string coord_line;
        std::getline(std::cin, coord_line);
        auto coords = line_to_nums<coord_type>(coord_line);
        Road<coord_type> road(coords);
        map_anam.push_back(road);
    }

    std::cout << std::setprecision(7) << std::fixed;
    std::cout << distance_between(map_sinchon, map_anam);
    return 0;
}