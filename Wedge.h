#ifndef RSPROJECT_WEDGE_H
#define RSPROJECT_WEDGE_H
#include "Triangle.h"

class Wedge
{
public:
    Wedge(Edge &aSegment, Edge& bSegment);

    double wedge_angle() const;

private:
    //TODO: pass only index?
    std::vector<Edge> segments;
};

#endif //RSPROJECT_WEDGE_H
