#include "Edge.h"

Edge::Edge(Vertex &start, Vertex &end, int index)
{
    // self edges are allowed and may come as a side effect of the edge flipping algorithm
    this->start_vertex = start;
    this->end_vertex = end;
    this->index = index;
    start.increase_degree();
    end.increase_degree();
    auto xCoord = start.x_coord() - end.x_coord();
    auto yCoord = start.y_coord() - end.y_coord();
    this->edge_length = sqrt(xCoord*xCoord + yCoord*yCoord);
}

Edge::Edge(const Edge &other)
{
    this->start_vertex = other.start_vertex;
    this->end_vertex = other.end_vertex;
    this->index = other.index;
    this->edge_length = other.edgeLength();
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

double Edge::edgeLength() const
{
    return this->edge_length;
}

int Edge::dimension()
{
    return 1;
}