#include "Edge.h"

Edge::Edge(Vertex &start, Vertex &end, int index)
{
    if(start == end)
    {
        throw std::invalid_argument("The vertices of a branch must differ.");
    }
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

int Edge::getIndex() const
{
    return this->index;
}

void Edge::setIndex(int new_index)
{
    this->index = new_index;
}

std::tuple<int, int> Edge::edge_as_index_pair()
{
    auto return_tuple = std::make_tuple<int, int>(this->getStart().getIndex(), this->getEnd().getIndex());
    return return_tuple;
}

int Edge::dimension()
{
    return 1;
}