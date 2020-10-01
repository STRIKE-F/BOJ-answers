// 5719 거의 최단 경로

#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <queue>
#include <algorithm>
#include <memory>

template <typename dT, typename vT>
class Edge;
template <typename dT, typename eT>
class Vertex;
template <typename vT, typename eT>
class Graph;

template <typename eT>
class VertexDataDijkstra
{
public:
    bool visited;
    eT distance;
    std::vector<Vertex<VertexDataDijkstra<eT>, eT>*> route_is_from;

    VertexDataDijkstra()
    {
        this->visited = false;
        this->distance = 0;
    }

    VertexDataDijkstra(const eT distance_limit)
    {
        this->visited = false;
        this->distance = distance_limit;
    }

    eT
    get_distance()
    {
        return this->distance;
    }
};

template <typename vT, typename eT>
class Visitor
{
public:
    virtual void 
    visit(Vertex<vT, eT>* const vertex) = 0;

    virtual void 
    visit(Graph<vT, eT>* const graph) = 0;
};

template <typename dT, typename vT>
class Edge
{
private:
    Vertex<vT, dT> *from;
    Vertex<vT, dT> *to;

protected:
    Vertex<vT, dT>*
    get_from_ptr() const
    {
        return this->from;
    }

    Vertex<vT, dT>*
    get_to_ptr() const
    {
        return this->to;
    }

public:
    dT data;    

    Edge(Vertex<vT, dT>* const from, Vertex<vT, dT>* const to)
    {
        this->from = from;
        this->to = to;
    }

    virtual
    ~Edge()
    {
        ;
    }

    virtual Vertex<vT, dT>*
    route_from(Vertex<vT, dT>* const from) const
    {
        if (this->from == from)
        {
            return this->to;
        }
        else
        {
            return this->from;
        }
    }
};

template <typename dT, typename vT>
class EdgeDirectional : public Edge<dT, vT>
{
private:
    
public:
    EdgeDirectional(Vertex<vT, dT> *const from, Vertex<vT, dT> *const to) : Edge<dT, vT>(from, to)
    {
        ;
    }

    Vertex<vT, dT>*
    route_from(Vertex<vT, dT>* const from) const override
    {
        if (this->get_from_ptr() == from)
        {
            return this->get_to_ptr();
        }
        else
        {
            return nullptr;
        }
    }
};

template <typename dT, typename eT>
class Vertex
{
public:
    std::vector<std::shared_ptr<Edge<eT, dT>>> edges;
    dT data;

    virtual
    ~Vertex()
    {
        ;
    }

    virtual void
    add_edge(std::shared_ptr<Edge<eT, dT>> edge)
    {
        // check if the other vertex is accessible through this edge
        if (edge->route_from(this) != nullptr)
        {
            this->edges.push_back(edge);
        }
    }

    virtual bool
    remove_edge_to(Vertex<dT, eT>* const vertex)
    {
        auto itr_to_remove =
            std::find_if(this->edges.begin(), this->edges.end(),
                         [this, vertex](auto edge_ptr) 
                         {
                             return edge_ptr->route_from(this) == vertex;
                         });
        // if such edge is found
        if (itr_to_remove != this->edges.end())
        {
            this->edges.erase(itr_to_remove);
            return true;
        }
        // if such edge is not found
        return false;
    }

    virtual void
    accept(Visitor<dT, eT> *visitor)
    {
        visitor->visit(this);
    }
};

template <typename vT, typename eT>
class Graph
{
public:
    std::vector<std::unique_ptr<Vertex<vT, eT>>> vertices;

    Graph(const int num_of_vertices)
    {
        vertices.reserve(num_of_vertices);
        for (int v = 0; v < num_of_vertices; v++)
        {
            vertices.push_back(std::make_unique<Vertex<vT, eT>>());
        }
    }

    virtual void
    connect_vertices(const int from, const int to, const eT data) = 0;

    virtual void
    disconnect_vertices(const int from, const int to) = 0;

    virtual vT
    get_data_at(int idx) const
    {
        return this->vertices.at(idx)->data;
    }

    virtual void
    accept(Visitor<vT, eT> *visitor)
    {
        visitor->visit(this);
    }
};

template <typename vT, typename eT>
class GraphDirectional : public Graph<vT, eT>
{
private:
    
public:
    GraphDirectional(const int num_of_vertices) : Graph<vT, eT>(num_of_vertices)
    {
        ;
    }

    virtual void
    connect_vertices(const int from, const int to, const eT data) override
    {
        Vertex<vT, eT> *vertex_from = this->vertices.at(from).get();
        Vertex<vT, eT> *vertex_to = this->vertices.at(to).get();
        auto edge_ptr = std::make_shared<EdgeDirectional<eT, vT>>(vertex_from, vertex_to);
        edge_ptr->data = data;
        vertex_from->add_edge(edge_ptr);
    }

    virtual void
    disconnect_vertices(const int from, const int to) override
    {
        Vertex<vT, eT> *vertex_from = this->vertices.at(from).get();
        Vertex<vT, eT> *vertex_to = this->vertices.at(to).get();
        vertex_from->remove_edge_to(vertex_to);
    }
};

template <typename eT>
class VisitorDijkstra : public Visitor<VertexDataDijkstra<eT>, eT>
{
private:
    int from;
    int to;
    eT distance_limit;

    typedef struct 
    {
        bool operator()(Vertex<VertexDataDijkstra<eT>, eT>* const lhs, Vertex<VertexDataDijkstra<eT>, eT>* const rhs)
        {
            return lhs->data.distance > rhs->data.distance;
        }
    } CmpDistance;

    std::priority_queue<Vertex<VertexDataDijkstra<eT>, eT>*,
                        std::vector<Vertex<VertexDataDijkstra<eT>, eT>*>, 
                        CmpDistance> ordered_vertices;

public:
    VisitorDijkstra(const int from, const int to, eT distance_limit) : Visitor<VertexDataDijkstra<eT>, eT>()
    {
        this->from = from;
        this->to = to;
        this->distance_limit = distance_limit;
    }

    void 
    visit(Vertex<VertexDataDijkstra<eT>, eT>* const vertex) override
    {
        for (auto &edge : vertex->edges)
        {
            auto destination_vertex = edge->route_from(vertex);
            // if this vertex has already been visited
            if (destination_vertex->data.visited)
            {
                continue;
            }
            else
            {
                eT new_distance = vertex->data.distance + edge->data;
                // std::cout << vertex << " to " << destination_vertex << ": Distance " << vertex->data.distance << " -> " << new_distance << std::endl;
                // there is a shorter route to this vertex
                if (destination_vertex->data.distance > new_distance)
                {
                    destination_vertex->data.distance = new_distance;
                    destination_vertex->data.route_is_from.clear();
                    destination_vertex->data.route_is_from.push_back(vertex);
                    this->ordered_vertices.push(destination_vertex);
                }
                // there's another shortest route to this vertex
                else if (destination_vertex->data.distance == new_distance)
                {
                    destination_vertex->data.route_is_from.push_back(vertex);
                }
            }
        }

        vertex->data.visited = true;
    }

    void 
    visit(Graph<VertexDataDijkstra<eT>, eT>* const graph) override
    {
        std::for_each(graph->vertices.begin(), graph->vertices.end(),
                      [this](auto &vertex) {
                          vertex->data = VertexDataDijkstra<eT>(this->distance_limit);
                      });
        graph->vertices.at(this->from)->data = VertexDataDijkstra<eT>();
        this->ordered_vertices.push(graph->vertices.at(this->from).get());

        // do until every vertex has been visited
        while (!this->ordered_vertices.empty())
        {
            Vertex<VertexDataDijkstra<eT>, eT> *vertex = this->ordered_vertices.top();
            this->ordered_vertices.pop();

            // only non-visited vertices can be in the ordered_vertices
            vertex->accept(this);
        }
    }
};

template <typename eT>
class VisitorRemover : public Visitor<VertexDataDijkstra<eT>, eT>
{
private:
    int to;
    eT distance_limit;

public:
    VisitorRemover(const int to, eT distance_limit) : Visitor<VertexDataDijkstra<eT>, eT>()
    {
        this->to = to;
        this->distance_limit = distance_limit;
    }

    void 
    visit(Vertex<VertexDataDijkstra<eT>, eT>* const vertex) override
    {
        for (auto prev_vertex : vertex->data.route_is_from)
        {
            // if this is the first time removing this edge
            if (prev_vertex->remove_edge_to(vertex))
            {
                prev_vertex->accept(this);
            }
        }
    }

    void
    visit(Graph<VertexDataDijkstra<eT>, eT>* const graph) override
    {
        graph->vertices.at(this->to)->accept(this);
    }
};

std::vector<int>
line_to_ints(std::string_view line, int max_ints) {
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
            int num = std::stoi(int_str);
            ints.push_back(num);
            int_index++;
            int_str.clear();
        }

        if (int_index == max_ints) 
        {
            // # of ints read has reached max_ints, so stop reading ints
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

int
main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

    typedef int dist_type;
    const dist_type INF = __INT_MAX__;
    
    std::vector<dist_type> near_shortest_distances;

    while (true)
    {
        std::string params_line;
        std::getline(std::cin, params_line);
        auto params = line_to_ints(params_line, 2);
        int num_of_vertices = params[0];
        int num_of_edges = params[1];
        if (num_of_vertices == 0 && num_of_edges == 0)
        {
            // termination condition
            break;
        }
        GraphDirectional<VertexDataDijkstra<dist_type>, dist_type> map(num_of_vertices);

        params_line.clear();
        std::getline(std::cin, params_line);
        params = line_to_ints(params_line, 2);
        int idx_of_source = params[0];
        int idx_of_sink = params[1];
        VisitorDijkstra<dist_type> dijk(idx_of_source, idx_of_sink, INF);
        VisitorRemover<dist_type> remv(idx_of_sink, INF);

        for (int i = 0; i < num_of_edges; i++)
        {
            std::string edge_line;
            std::getline(std::cin, edge_line);
            auto edge = line_to_ints(edge_line, 3);
            int from = edge[0];
            int to = edge[1];
            int distance = edge[2];
            map.connect_vertices(from, to, distance);
        }

        map.accept(&dijk);
        dist_type prev_distance = map.get_data_at(idx_of_sink).distance;

        dist_type curr_distance = prev_distance;
        // until there's a change in min. distance to sink
        while (curr_distance != INF && curr_distance == prev_distance)
        {
            map.accept(&remv);
            map.accept(&dijk);
            curr_distance = map.get_data_at(idx_of_sink).distance;
        }

        dist_type near_shortest_distance = map.get_data_at(idx_of_sink).get_distance();
        near_shortest_distances.push_back(near_shortest_distance);
    }

    for (auto distance : near_shortest_distances)
    {
        if (distance == INF)
        {
            std::cout << "-1\n";
        }
        else
        {
            std::cout << distance << "\n";
        }
    }

    return 0;
}