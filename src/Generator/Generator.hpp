#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include "../DirectedGraph/DirectedGraph.hpp"
#include "GeneratorException.hpp"
#include "../Data/Data.hpp"

#include <type_traits>
#include <random>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <thread>

std::string randomString(size_t length)
{
    auto randchar = []() -> char
    {
        const char charset[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    //cout << length << endl;
    std::string str(length,0);
    std::generate_n(str.begin(), length, randchar);
    //cout << str << endl;
    return str;
}

template <typename T>
class MyGenerator {
public:
    T operator()(const T lower_bound, const T upper_bound, std::mt19937 & gen)
    {
        if (lower_bound >= upper_bound)
            throw ("lower_bound should be less than upper_bound");

        if constexpr(std::is_integral<T>::value)  // without constexpr static assertion fails
        {
            std::uniform_int_distribution<int> distrib(lower_bound, upper_bound);
            return distrib(gen);
        }
        else if (std::is_floating_point<T>::value)
        {
            std::uniform_real_distribution<T> distrib(lower_bound, upper_bound);
            return distrib(gen);
        }
        else
            throw GeneratorException<T>("unsupported type");
    }
};

template <typename T>
std::vector<T> myGenerator(const T min, const T max, const size_t amount)
{
    srand ( time(NULL) );
    std::this_thread::sleep_for(std::chrono::milliseconds(800));

    MyGenerator<T> gen;
    std::vector<T> generated;
    std::mt19937 generator(rand()); // Standard mersenne_twister_engine seeded with rand()

    for (size_t i=0; i<amount; ++i)
        generated.push_back(gen(min, max, generator));

    return generated;
}

std::vector<VertexData> vertexDataGenerator(const size_t amount)
// amount is the amount of Vertices we want to create
{
    std::vector<VertexData> data;
    int estimated = ceil(amount/52+3); // 26 letters in English alphabet and 52 including CAPS

    for (size_t i=0; i<amount; ++i)
    {
        std::string buffStr = randomString(estimated); // estimated is the size of each string which describes each vertex
        VertexData buffData(buffStr, i);
        data.push_back(buffData);
    }

    return data;
}

std::vector<EdgeData> edgeDataGenerator(const size_t amount, const double lower_bound, const double upper_bound)
{
    std::vector<EdgeData> data;
    int estimated = ceil(amount/52+3); // 26 letters in English alphabet and 52 including CAPS
    // +3 is added to make it less possible for generated strings to repeat

    std::vector<double> weights = myGenerator<double>(lower_bound, upper_bound, amount);

    for (size_t i=0; i<amount; ++i)
    {
        std::string buffStr = randomString(estimated);
        EdgeData buffData(buffStr, weights[i]);
        data.push_back(buffData);
        //data[i].edge_name = buffStr;
        //data[i].dist = abs(weights[i]); // abs is used because dijkstra algorithm works for positive weights only
    }
    return data;
}

template<typename T>
// T can be MyUndirectedGraph or MyDirectedGraph
// generate graph by specified topology
T & myDirectedGraphGenerator(const size_t amountOfVertices, const size_t amountOfEdges, const double lower_bound, const double upper_bound)
{
    std::vector<VertexData> genVertexData = vertexDataGenerator(amountOfVertices);
    std::vector<EdgeData> genEdgesData = edgeDataGenerator(amountOfEdges, lower_bound, upper_bound);

    std::vector<std::pair<int, int>> edgesPairs;
    size_t completeGraphVertices = floor(amountOfVertices*(amountOfVertices-1)); // formula for the amount of vertices in complete graph is n*(n-1)/2, but our graph is directed, so *2

    if (amountOfEdges > completeGraphVertices)
        throw GeneratorException<int>("exceed the amount of vertices in the complete graph of the same size");

    std::vector<int> left = myGenerator<int>(0, int(amountOfVertices-1), amountOfEdges); // generating indexes of vertices from which the edge is going out
    std::vector<int> right = myGenerator<int>(0, int(amountOfVertices-1), amountOfEdges); // generating indexes of vertices to which the edge goes
    // amountOfVertices-1 as the maximum index of a vertex is amountOfVertices-1

    for (size_t i=0; i<amountOfEdges; ++i)
    {
        std::pair<int, int> buff(left[i], right[i]);
        edgesPairs.push_back(buff);
    }

    T* generated = new T(genVertexData, genEdgesData, edgesPairs);
    return *generated;
}

#endif // GENERATOR_HPP


