#ifndef DATA_HPP
#define DATA_HPP

struct VertexData
{
    VertexData() {}

    VertexData(const std::string & str, const int & n)
        : first_name(str), num(n)
    {
    }
    std::string first_name;
    int num;
};

struct EdgeData
{
    EdgeData() {}

    EdgeData(const std::string & str, const int & n)
        : edge_name(str), dist(n)
    {
    }

    std::string edge_name;
    double dist;
};

#endif // DATA_HPP
