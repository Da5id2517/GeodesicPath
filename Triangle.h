#ifndef RSPROJECT_TRIANGLE_H
#define RSPROJECT_TRIANGLE_H
#include "Edge.h"
#include <map>


//Due to deadlines consider the face only to be a triangle.
class Triangle
{
public:
    Triangle(Edge &aEdge, Edge &bEdge, Edge &cEdge, int index = 0);

    std::vector<Edge> getEdges();

    void setIndex(int new_index);
    int getIndex() const;

    double getAngleByVertexIndex(int id);
    std::vector<int> triangle_as_index_triple();


private:
    std::vector<Edge> edges;
    std::map<int, double> vertex_index_angle_map;
    int index;

};

std::ostream &operator << (std::ostream &out, Triangle &face);

#endif //RSPROJECT_TRIANGLE_H
