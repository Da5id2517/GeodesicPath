#include "Edge.h"

Edge::Edge(Vertex &start, Vertex &end, int index)
{
    this->start_vertex = start;
    this->end_vertex = end;
    this->index = index;
}

Vertex& Edge::getEnd()
{
    return end_vertex;
}

Vertex& Edge::getStart()
{
    return start_vertex;
}

int Edge::dimension()
{
    return 1;
}