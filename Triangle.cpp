#include <unordered_set>
#include "Triangle.h"

Triangle::Triangle(Vertex &pointA, Vertex &pointB, Vertex &pointC, int index)
{
    this->vertices = {pointA, pointB, pointC};
    this->index = index;

    Edge edgeAB(pointA, pointB);
    Edge edgeBC(pointB, pointC);
    Edge edgeAC(pointA, pointC);
    this->edges = {edgeBC, edgeAC, edgeAB};

    auto a = edgeBC.edgeLength();
    auto b = edgeAC.edgeLength();
    auto c = edgeAB.edgeLength();
    auto alfa = SafeAcos((b*b + c*c - a*a)/(2*b*c));
    auto beta = SafeAcos((a*a + c*c - b*b)/(2*a*c));
    auto gamma = SafeAcos((a*a +b*b - c*c)/(2*a*b));

    // store angles as (vertex_index, angle) key, value pairs.
    this->vertex_index_angle_map[pointA.getIndex()] = alfa;
    this->vertex_index_angle_map[pointB.getIndex()] = beta;
    this->vertex_index_angle_map[pointC.getIndex()] = gamma;
}

double Triangle::getAngleByVertexIndex(int id)
{
    auto index_triple = this->triangle_as_index_triple();
    if(std::count(index_triple.begin(), index_triple.end(), id) == 0)
    {
        std::string output = "(";
        for(auto ind : index_triple)
        {
            output += std::to_string(ind);
            output += ", ";
        }
        output += ")";
        throw std::invalid_argument("Triangle " + output + " does not contain vertex " + std::to_string(id));
    }

    auto result = this->vertex_index_angle_map[id];
    return result;
}

std::vector<Vertex> Triangle::getVertices()
{
    return this->vertices;
}

std::vector<Edge> Triangle::getEdges()
{
    return this->edges;
}

int Triangle::getIndex() const
{
    return this->index;
}

void Triangle::setIndex(int new_index)
{
    this->index = new_index;
}

std::vector<int> Triangle::triangle_as_index_triple()
{
    auto result_set = std::unordered_set<int>();
    std::vector<int> result_vector;

    for(auto & vertex : vertices)
    {
        result_set.insert(vertex.getIndex());
    }

    for(auto it = result_set.begin(); it != result_set.end();)
    {
        result_vector.push_back(result_set.extract(it++).value());
    }
    std::sort(result_vector.begin(), result_vector.end());

    return result_vector;
}

void Triangle::setEdgeIndex(int new_index, Edge &edge)
{
    for(auto &e : this->edges)
    {
        if(e == edge)
            e.setIndex(new_index);
    }
}

std::ostream &operator << (std::ostream &out, Triangle &triangle)
{
    auto vertices = triangle.getVertices();

    out << "f ";
    for(auto &vertex : vertices)
    {
        out << std::to_string(vertex.getIndex() + 1);
        out << " ";
    }
    out << std::endl;

    return out;
}