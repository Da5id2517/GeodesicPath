#ifndef RSPROJECT_EDGE_H
#define RSPROJECT_EDGE_H
#include <vector>
#include "Vertex.h"


class Edge
{
public:
    Edge(Vertex &start_vertex,Vertex &end_vertex, int index = 0);
    Edge(const Edge &other);

    Vertex& getStart();
    Vertex& getEnd();
    int getStartIndex() const;
    int getEndIndex() const;

    indexPair_t edge_as_index_pair() const;

    int getIndex() const;
    void setIndex(int new_index);
    double edgeLength() const;

    bool operator == (const Edge &other) const;

private:
    int index;
    double edge_length;
    Vertex start_vertex;
    Vertex end_vertex;
};


#endif //RSPROJECT_EDGE_H
