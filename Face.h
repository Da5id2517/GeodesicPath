#ifndef RSPROJECT_FACE_H
#define RSPROJECT_FACE_H
#include "Edge.h"


//Due to deadlines consider the face only to be a triangle.
class Face
{
public:
    Face(std::vector<Edge> &edges, int index = 0);

    std::vector<Edge> getEdges();

    void setIndex(int new_index);
    int getIndex() const;

    std::tuple<double, double, double> getAngles();
    std::vector<int> face_as_index_k_tuple();

    int dimension();

private:
    std::vector<Edge> edges;
    std::tuple<double, double, double> angles;
    int index;

};

std::ostream &operator << (std::ostream &out, Face &face);

#endif //RSPROJECT_FACE_H
