#ifndef RSPROJECT_FACE_H
#define RSPROJECT_FACE_H
#include "Edge.h"

class Face
{
public:
    Face(std::vector<Edge> &edges, int index = 0);

    std::vector<Edge> getEdges();

    void setIndex(int new_index);
    int getIndex() const;

    std::vector<int> face_as_index_k_tuple();

    int dimension();

private:
    std::vector<Edge> edges;
    int index;

};
#endif //RSPROJECT_FACE_H
