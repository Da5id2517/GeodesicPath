#ifndef RSPROJECT_EDGE_H
#define RSPROJECT_EDGE_H
#include <vector>
#include "Vertex.h"

class Edge
{
public:
    Edge(Vertex &start_vertex,Vertex &end_vertex, int index = 0);

    Vertex& getStart();
    Vertex& getEnd();

    std::tuple<int, int> edge_as_index_pair();

    int getIndex() const;
    void setIndex(int new_index);
    double edgeLength() const;
    static int dimension();

private:
    int index;
    double edge_length;
    Vertex start_vertex;
    Vertex end_vertex;
};

#endif //RSPROJECT_EDGE_H
