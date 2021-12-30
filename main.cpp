#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/named_function_params.hpp>

#include "src/DirectedGraph/DirectedGraph.hpp"
#include "src/UndirectedGraph/UndirectedGraph.hpp"
#include "src/Generator/Generator.hpp"
#include "src/PriorityQueue/PriorityQueue.hpp"

int main()
{
    // Testing PriorityQueue
    PriorityQueue<int, float> priorityQueue(My::Sequence<int>::toSequence(myGenerator<int>(10, 1000, 20)), My::Sequence<float>::toSequence(myGenerator<float>(10, 1000, 20)));
    priorityQueue.print();

    cout << endl;

    priorityQueue.add(5789, 47.56);
    priorityQueue.add(5789, 50.444);
    priorityQueue.add(525, 39.67897);

    vector<int> priorities = {110, 16798, 412, 1678};
    vector<float> values = {578.271, 45.78, 56.789, 123.3789};

    priorityQueue.append(My::Sequence<int>::toSequence(priorities), My::Sequence<float>::toSequence(values));

    priorityQueue.print();

    cout << endl;

    priorityQueue.pop_back();
    priorityQueue.print();

    // Testing Graphs

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
