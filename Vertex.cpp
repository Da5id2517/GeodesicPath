#include "Vertex.h"

Vertex::Vertex(const Vertex &vertex)
{
    this->x = vertex.x;
    this->y = vertex.y;
    this->index = vertex.index;
}

float Vertex::x_coord() const
{
    return x;
}

float Vertex::y_coord() const
{
    return y;
}

int Vertex::getIndex() const
{
    return index;
}

int Vertex::dimension()
{
    return 0;
}