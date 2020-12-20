#include "Wedge.h"

Wedge::Wedge(Edge &aSegment, Edge &bSegment)
{
    if(!(aSegment.getEnd() == bSegment.getStart()))
    {
        throw std::invalid_argument("Segments must be connect and share a node.");
    }
    std::vector<Edge> tuple = {aSegment, bSegment};
    this->segments = tuple;
}

double Wedge::wedge_angle() const
{
    auto edge1 = segments[0];
    auto edge2 = segments[1];
    auto phantom_edge = Edge(edge2.getEnd(), edge1.getStart(), 0);

    Triangle phantom_triangle(edge1, edge2, phantom_edge, 0);
    //TODO: this increases node degrees fictitiously
    auto angle = phantom_triangle.getAngleByVertexIndex(edge2.getStart().getIndex());
    return angle;
}