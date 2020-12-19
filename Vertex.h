#ifndef RSPROJECT_VERTEX_H
#define RSPROJECT_VERTEX_H
#include <iostream>

class Vertex
{
public:
    Vertex(): x(0.0), y(0.0), index(0), degree(0) {};
    Vertex(double x, double y, int index = 0) : x(x), y(y), index(index), degree(0) {};
    Vertex(const Vertex &vertex);

    double x_coord() const;
    double y_coord() const;
    //TODO: add z_coord()

    int getIndex() const;
    void setIndex(int new_index);

    static int dimension();
    void increase_degree();
    int getDegree() const;

    bool operator == (const Vertex& other) const;

private:
    double x, y;
    int index, degree;

};

std::ostream &operator <<(std::ostream &out, const Vertex &vertex);

#endif //RSPROJECT_VERTEX_H
