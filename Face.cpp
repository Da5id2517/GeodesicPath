#include "Face.h"

Face::Face(std::vector<Edge> &edges, int index)
{
    this->edges = edges;
    this->index = index;
}

std::vector<Edge> Face::getEdges()
{
    return edges;
}

int Face::dimension()
{
    return edges.size() - 1;
}