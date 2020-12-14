#ifndef RSPROJECT_EDGE_H
#define RSPROJECT_EDGE_H
#include "Vertex.h"

class Edge
{
public:
    Edge(Vertex &start_vertex,Vertex &end_vertex, int index);

    Vertex& getStart();
    Vertex& getEnd();

    static int dimension();

private:
    int index;
    Vertex start_vertex;
    Vertex end_vertex;
};

#endif //RSPROJECT_EDGE_H
