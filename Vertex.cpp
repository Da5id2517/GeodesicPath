#include "Vertex.h"

Vertex::Vertex(const Vertex &vertex)
{
    this->x = vertex.x;
    this->y = vertex.y;
    this->index = vertex.index;
    this->degree = vertex.degree;
}

double Vertex::x_coord() const
{
    return x;
}

double Vertex::y_coord() const
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

void Vertex::setIndex(int new_index)
{
    this->index = new_index;
}

bool Vertex::operator==(const Vertex &other) const
{
    return this->index == other.index && this->x == other.x && this->y == other.y;
}

void Vertex::increase_degree()
{
    auto new_degree = this->degree + 1;
    this->degree = new_degree;
}

int Vertex::getDegree() const
{
    return this->degree;
}

std::ostream &operator << (std::ostream &out, const Vertex &vertex)
{
    out << "v " << vertex.x_coord() << " " << vertex.y_coord() << " 0.0" << std::endl;
    return out;
}