#ifndef RSPROJECT_TRIANGLE_H
#define RSPROJECT_TRIANGLE_H
#include "Edge.h"
#include <map>


//Due to deadlines consider the face only to be a triangle.
class Triangle
{
public:
    Triangle(Vertex &pointA, Vertex &pointB, Vertex &pointC, int index = 0);

    void setEdgeIndex(int new_index, Edge &edge);
    void setIndex(int new_index);
    int getIndex() const;
    std::vector<Vertex> getVertices();
    std::vector<Edge> getEdges();

    double getAngleByVertexIndex(int id);
    std::vector<int> triangle_as_index_triple();


private:
    std::vector<Vertex> vertices;
    std::vector<Edge> edges;
    std::map<int, double> vertex_index_angle_map;
    int index;

};

std::ostream &operator << (std::ostream &out, Triangle &face);

#endif //RSPROJECT_TRIANGLE_H
