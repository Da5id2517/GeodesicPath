#include "Vertex.h"

Vertex::Vertex(const Vertex &vertex)
{
    this->point = vertex.point;
    this->index = vertex.index;
    this->degree = vertex.degree;
}

point_t Vertex::getPoint() const
{
    return this->point;
}

int Vertex::getIndex() const
{
    return index;
}


void Vertex::setIndex(int new_index)
{
    this->index = new_index;
}

bool Vertex::operator==(const Vertex &other) const
{
    auto equal_coordinates = this->point == other.point;
    auto equal_indices = this->index == other.index;
    if(equal_coordinates && !equal_indices)
    {
        throw std::invalid_argument("Two vertices with the same coordinates have different indices!");
    }
    return equal_coordinates && equal_indices;
}

void Vertex::increase_degree()
{
    auto new_degree = this->degree + 1;
    this->degree = new_degree;
}

void Vertex::decrease_degree()
{
    auto new_degree = this->degree - 1;
    this->degree = new_degree;
}

int Vertex::getDegree() const
{
    return this->degree;
}

std::ostream &operator << (std::ostream &out, const Vertex &vertex)
{
    auto point = vertex.getPoint();
    out << "v " << point.x << " " << point.y << " " << point.z << std::endl;
    return out;
}

std::vector<point_t> verticesToPoints(std::vector<Vertex> &vertices)
{
    std::vector<point_t> result;
    for(auto &vertex : vertices)
    {
        result.push_back(vertex.getPoint());
    }
    return result;
}

