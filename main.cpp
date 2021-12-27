#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/named_function_params.hpp>

#include "src/DirectedGraph/DirectedGraph.hpp"
#include "src/UndirectedGraph/UndirectedGraph.hpp"
#include "src/Generator/Generator.hpp"

int main()
{
    MyDirectedGraph newDigraph1 = myDirectedGraphGenerator<MyDirectedGraph>(100, 200, 1, 17);
    MyUndirectedGraph newDigraph2 = myDirectedGraphGenerator<MyUndirectedGraph>(50, 70, 10, 2719);

    newDigraph1.printVertices();
    newDigraph1.printEdges();

    std::cout << std::endl << std::endl;

    newDigraph2.printVertices();
    newDigraph2.printEdges();

    newDigraph1.dijkstraShortestPath();

    newDigraph1.materialize();
    newDigraph2.materialize();

    return 0;
}
