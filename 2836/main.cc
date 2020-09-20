// 2836 수상 택시

#include <iostream>
#include <algorithm>
#include <vector>

typedef struct
{
    int from;
    int to;
} MoveRequest;

bool cmp_request_greater(MoveRequest lhs, MoveRequest rhs)
{
    if (lhs.from == rhs.from)
    {
        return lhs.to > rhs.to;
    }
    else
    {
        return lhs.from > rhs.from;
    }
}

std::vector<MoveRequest>
read_requests(int guests)
{
    std::vector<MoveRequest> requests;
    requests.reserve(guests);

    for (int i = 0; i < guests; i++)
    {
        int from, to;
        std::cin >> from >> to;
        if (from > to)
        {
            // from < to: left-to-right request can be completed on our way from 0 to M
            // from == to: invalid request
            MoveRequest request{from, to};
            requests.push_back(request);
        }
    }

    return requests;
}

std::vector<MoveRequest>
aggregate_RTL_requests(std::vector<MoveRequest> &requests)
{
    // sort requests (in decreasing order: RTL)
    std::sort(requests.begin(), requests.end(), cmp_request_greater);

    std::vector<MoveRequest> aggregated_requests;
    // initial values for temp from/to
    int aggregate_from = requests.front().from;
    int aggregate_to = requests.front().to;

    for (const auto &request : requests)
    {
        if (request.from < aggregate_to)
        {
            // cannot aggregate distant requests
            MoveRequest aggregation{aggregate_from, aggregate_to};
            aggregated_requests.push_back(aggregation);
            aggregate_from = request.from;
            aggregate_to = request.to;
        }
        else
        {
            // aggregate neighboring or overlapping requests
            aggregate_to = std::min(aggregate_to, request.to);
        }
    }
    // push back the last aggregation at the end 
    MoveRequest aggregation{aggregate_from, aggregate_to};
    aggregated_requests.push_back(aggregation);

    return aggregated_requests;
}

int
sum_move_distances(std::vector<MoveRequest> &requests)
{
    int distance_sum = 0;
    for (const auto &request : requests)
    {
        // all requests are RTL, so from > to
        distance_sum += request.from - request.to;
    }

    return distance_sum;
}

long long
min_distance_for_requests(int destination, std::vector<MoveRequest> &requests)
{
    long long total_distance = destination;
    auto minimal_RTL_moves = aggregate_RTL_requests(requests);
    total_distance += sum_move_distances(minimal_RTL_moves) * 2;
    return total_distance;
}

int
main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

    int guests, destination;
    std::cin >> guests >> destination;

    auto requests = read_requests(guests);
    std::cout << min_distance_for_requests(destination, requests);

    return 0;
}