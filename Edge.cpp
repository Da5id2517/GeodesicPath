#include "Edge.h"

Edge::Edge(Vertex &start, Vertex &end, int index)
{
    // self edges are allowed and may come as a side effect of the edge flipping algorithm
    this->start_vertex = start;
    this->end_vertex = end;
    this->index = index;
    start.increase_degree();
    end.increase_degree();
    auto startPoint = start.getPoint();
    auto endPoint = end.getPoint();

    auto xCoord = startPoint.x - endPoint.x;
    auto yCoord = startPoint.y - endPoint.y;
    auto zCoord = startPoint.z - endPoint.z;
    this->edge_length = sqrt(xCoord*xCoord + yCoord*yCoord + zCoord*zCoord);
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

int Edge::getStartIndex() const
{
    return start_vertex.getIndex();
}

int Edge::getEndIndex() const
{
    return end_vertex.getIndex();
}

int Edge::getIndex() const
{
    return this->index;
}

void Edge::setIndex(int new_index)
{
    this->index = new_index;
}

indexPair_t Edge::edge_as_index_pair() const
{
    indexPair_t tuple = {this->getStartIndex(), this->getEndIndex()};
    return tuple;
}

double Edge::edgeLength() const
{
    return this->edge_length;
}

bool Edge::operator == (const Edge &other) const
{
    auto equal_index_pairs = this->edge_as_index_pair() == other.edge_as_index_pair();
    auto equal_edge_indices = this->getIndex() == other.getIndex();
    auto equal_edge_lengths = this->edgeLength() == other.edgeLength();

    if(equal_edge_lengths && equal_index_pairs && !equal_edge_indices)
    {
        throw std::invalid_argument("Duplicate branch with a different index.");
    }

    return equal_index_pairs && equal_edge_indices && equal_edge_lengths;
}

