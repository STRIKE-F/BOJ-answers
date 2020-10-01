// Generic graph implementation

#include <vector>
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
    Vertex<VertexDataDijkstra<eT>, eT> *route_is_from;

    VertexDataDijkstra()
    {
        this->visited = false;
        this->distance = 0;
        this->route_is_from = nullptr;
    }

    VertexDataDijkstra(const eT distance_limit)
    {
        this->visited = false;
        this->distance = distance_limit;
        this->route_is_from = nullptr;
    }

    void
    print_distance(const eT distance_limit)
    {
        // INF distance
        if (this->distance == distance_limit)
        {
            std::cout << "-1\n";
        }
        else
        {
            std::cout << this->distance << "\n";
        }
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

int
main()
{
    GraphDirectional<int, int> GD(3);
    GD.connect_vertices(0, 2, 0);
    GD.connect_vertices(1, 2, 3);

    return 0;
}