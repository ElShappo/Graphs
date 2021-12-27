#ifndef DIRECTED_GRAPH
#define DIRECTED_GRAPH

#include <iostream>
#include <string>
#include <utility>
#include <chrono>
#include <thread>

#include <boost/config.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/named_function_params.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <boost/graph/adj_list_serialize.hpp>
#include <boost/property_map/function_property_map.hpp>

#include "../Data/Data.hpp"

using namespace boost;

using VertexPropertyType = property<vertex_name_t, std::string, property<vertex_color_t, default_color_type>>;
using EdgePropertyType = property<edge_weight_t, int, property<edge_color_t, default_color_type>>;
using DirectedGraphType = adjacency_list<setS, vecS, directedS, VertexData, EdgeData>;
using VertexDescriptor = graph_traits<DirectedGraphType>::vertex_descriptor;

class MyDirectedGraph
{
private:
    private:
    typedef boost::adjacency_list<boost::setS, boost::vecS, // setS prohibits adding parallel edges
                                boost::directedS,
                                VertexData,
                                boost::property<boost::edge_weight_t, double, EdgeData>
                                > MyGraphType;
    MyGraphType* G_;


public:
    MyDirectedGraph() {}

    /*
    MyDirectedGraph(const MyDirectedGraph & other)
    {
        G_ = new MyDirectedGraph[sizeof(MyDirectedGraph)*2];
        *G_ = other.getBoostGraph();
    }

    MyGraphType & getBoostGraph() const
    {
        return *G_;
    }*/

    MyDirectedGraph(const size_t vertices)
    {
        G_ = new MyGraphType(5);
    }

    MyDirectedGraph(const std::vector<VertexData> & verticesData, const std::vector<EdgeData> & edgesData, const std::vector<std::pair<int, int>> & pairs)
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

    ~MyDirectedGraph() noexcept
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

    void dijkstraShortestPath()
    {
        auto id_map = boost::get(&VertexData::num, *G_);
        auto weightmap = boost::get(&EdgeData::dist, *G_);

        auto source = *vertices(*G_).first;

        typedef adjacency_list < setS, vecS, directedS, VertexData, boost::property<boost::edge_weight_t, double, EdgeData> > graph_t;

        typedef graph_traits < graph_t >::vertex_descriptor vertex_descriptor;
        typedef graph_traits < graph_t >::edge_descriptor edge_descriptor;

        using Vertex = MyGraphType::vertex_descriptor;
        std::map<Vertex, Vertex> predecessors;
        std::map<Vertex, double> distance;
        std::map<Vertex, boost::default_color_type> colors;

        boost::dijkstra_shortest_paths(
                *G_, source,
                boost::vertex_color_map(boost::make_assoc_property_map(colors))
                .predecessor_map(boost::make_assoc_property_map(predecessors))
                .distance_map(boost::make_assoc_property_map(distance))
                .weight_map(weightmap)
                .vertex_index_map(id_map));

                char name[] =
                                            //"0123456789"
                                            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                            "abcdefghijklmnopqrstuvwxyz";
            std::vector<vertex_descriptor> p(num_vertices(*G_));

          std::ofstream dot_file("figs/Algorithm/dijkstra.dot");

          dot_file << "digraph D {\n"
            << "  rankdir=LR\n"
            << "  size=\"4,3\"\n"
            << "  ratio=\"fill\"\n"
            << "  edge[style=\"bold\"]\n" << "  node[shape=\"circle\"]\n";

          graph_traits < graph_t >::edge_iterator ei, ei_end;
          for (tie(ei, ei_end) = edges(*G_); ei != ei_end; ++ei) {
            graph_traits < graph_t >::edge_descriptor e = *ei;
            graph_traits < graph_t >::vertex_descriptor
              u = boost::source(e, *G_), v = target(e, *G_);
            dot_file << u << " -> " << v
              << "[label=\"" << boost::get(weightmap, e) << "\"";
            if (p[v] == u)
              dot_file << ", color=\"black\"";
            else
              dot_file << ", color=\"grey\"";
            dot_file << "]";
          }
          dot_file << "}";

        std::system("dot -Tpng figs/Algorithm/dijkstra.dot -o figs/Algorithm/output.png");
        std::system("start figs/Algorithm/output.png");
    }

    void materialize()
    {
        dynamic_properties dp;
        dp.property("label", boost::get(&EdgeData::dist, *G_)); // label weights of the edges
        dp.property("node_id", boost::get(&VertexData::num, *G_)); // label the indexes of the vertices
        dp.property("rankdir", boost::make_constant_property<MyGraphType*>(std::string("LR"))); // align graphs by their indexes from left to right

        std::ofstream f("figs/Directed/input.dot");
        boost::write_graphviz_dp(f, *G_, dp);
        f.close();

        std::system("dot -Tpng figs/Directed/input.dot -o figs/Directed/output.png");
        std::system("start figs/Directed/output.png");
    }
};

#endif // DIRECTED_GRAPH
