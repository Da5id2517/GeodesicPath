#ifndef RSPROJECT_FACE_H
#define RSPROJECT_FACE_H
#include <vector>
#include "Edge.h"

class Face
{
public:
    Face(std::vector<Edge> &edges, int index);

    std::vector<Edge> getEdges();

    int dimension();

private:
    std::vector<Edge> edges;
    int index;

};
#endif //RSPROJECT_FACE_H
