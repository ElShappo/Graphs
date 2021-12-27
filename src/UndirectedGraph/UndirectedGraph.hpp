#ifndef UNDIRECTED_GRAPH
#define UNDIRECTED_GRAPH

#include <iostream>
#include <string>
#include <utility>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/named_function_params.hpp>

#include "../Data/Data.hpp"

class MyUndirectedGraph
{
private:
    private:
    typedef boost::adjacency_list<boost::setS, boost::vecS, // setS prohibits adding parallel edges
                                boost::undirectedS,
                                VertexData,
                                boost::property<boost::edge_weight_t, double, EdgeData>
                                > MyGraphType;
    MyGraphType* G_;

public:
    MyUndirectedGraph() {}

    /*
    MyUndirectedGraph(const MyUndirectedGraph && other)
    {
        G_ = other.getBoostGraph();
    }

    MyGraphType && getBoostGraph() const
    {
        return std::move(*G_);
    }
    */

    MyUndirectedGraph(const size_t vertices)
    {
        G_ = new MyGraphType(5);
    }

    MyUndirectedGraph(const std::vector<VertexData> & verticesData, const std::vector<EdgeData> & edgesData, const std::vector<std::pair<int, int>> & pairs)
    // ctor by specified topology
    {
        int amountOfVertices = verticesData.size();
        int amountOfEdges = edgesData.size();

        std::vector<int> left, right;

        for (auto it : pairs)
        {
            left.push_back(it.first); // assign left element of pair
            right.push_back(it.second); // assign right element of pair
        }

        G_ = new MyGraphType;

        for (int i=0; i<amountOfVertices; ++i)
            addVertex(verticesData[i]);

        for (int i=0; i<amountOfEdges; ++i)
            addEdge(left[i], right[i], edgesData[i]);
    }

    auto operator[] (const auto index) const // if the initial class was virtual, auto wouldn't have been allowed
    {
        return (*G_)[index];
    }

    auto get(const auto index)
    {
        return (*G_)[index];
    }

    void assign(const int index, const VertexData & data)
    {
        (*G_)[index].first_name = data.first_name;
        (*G_)[index].num = data.num;
    }

    void assign(const auto index, const EdgeData & data)
    {
        (*G_)[index].edge_name = data.edge_name;
        (*G_)[index].dist = data.dist;
    }

    MyGraphType::vertex_descriptor addVertex(const VertexData & data)
    // returns the index of the added vertex
    // for vector vertex_descriptor is the same as index
    {
        auto v = add_vertex(*G_);
        (*G_)[v].first_name = data.first_name;
        (*G_)[v].num = data.num;
        return v;
    }

    void removeVertex(const int index)
    {
        return remove_vertex(index, *G_);
    }

    MyGraphType::edge_descriptor addEdge(const size_t v1, const size_t v2, const EdgeData & data)
    // returns the pair of two vertices which comprise the edge
    // for vector edge_descriptor is the same as pair of two ints (or is it?)
    {
        auto e = add_edge(v1,v2,*G_).first;
        (*G_)[e].edge_name = data.edge_name;
        (*G_)[e].dist = data.dist;
        return e;
    }

    void removeEdge(auto index)
    {
        return remove_edge(index, *G_);
    }

    ~MyUndirectedGraph() noexcept
    {
        delete G_;
    }

    auto getVertices() const
    {
        return vertices(*G_);
    }

    auto getEdges() const
    {
        return edges(*G_);
    }

    void printVertices() const
    {
        auto vpair = vertices(*G_);
        for(auto iter=vpair.first; iter!=vpair.second; iter++)
            std::cout << "vertex " << *iter << std::endl;
    }

    void printEdges() const
    {
        auto epair = edges(*G_);
        int iteration = 0;
        for(auto iter=epair.first; iter!=epair.second; iter++)
            std::cout << "edge[" << iteration++ << "] " << source(*iter, *G_) << " - " << target(*iter, *G_) << std::endl;
    }

    void materialize()
    {
        dynamic_properties dp;
        dp.property("label", boost::get(&EdgeData::dist, *G_)); // label weights of the edges
        dp.property("node_id", boost::get(&VertexData::num, *G_)); // label the indexes of the vertices
        dp.property("rankdir", boost::make_constant_property<MyGraphType*>(std::string("LR"))); // align graphs by their indexes from left to right

        std::ofstream f("figs/Undirected/input.dot");
        boost::write_graphviz(f, *G_);
        f.close();

        std::system("dot -Tpng figs/Undirected/input.dot -o figs/Undirected/output.png");
        std::system("start figs/Undirected/output.png");
    }
};

#endif // UNDIRECTED_GRAPH
