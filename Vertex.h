#ifndef RSPROJECT_VERTEX_H
#define RSPROJECT_VERTEX_H
#include <iostream>
#include "utils.h"
#include "utilTypes.h"


class Vertex
{
public:
    Vertex(): point({0.0, 0.0, 0.0}), index(0), degree(0) {};
    Vertex(double x, double y, double z, int index = 0) : point({x, y, z}), index(index), degree(0) {};
    Vertex(const Vertex &vertex);

    point_t getPoint() const;


    int getIndex() const;
    void setIndex(int new_index);

    void decrease_degree();
    void increase_degree();
    int getDegree() const;

    bool operator == (const Vertex& other) const;

private:
    point_t point{};
    int index, degree;
};

std::ostream &operator <<(std::ostream &out, const Vertex &vertex);

#endif //RSPROJECT_VERTEX_H
