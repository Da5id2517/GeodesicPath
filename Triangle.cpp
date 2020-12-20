#include <unordered_set>
#include "Triangle.h"

double SafeAcos (double x)
{
    if (x < -1.0) x = -1.0 ;
    else if (x > 1.0) x = 1.0 ;
    return acos (x) ;
}

Triangle::Triangle(Edge &aEdge, Edge &bEdge, Edge &cEdge, int index)
{

    this->edges = {aEdge, bEdge, cEdge};
    this->index = index;

    auto start_of_cycle = aEdge.getStart();
    if(std::find_if(edges.begin(),edges.end(),
                    [start_of_cycle](auto edge){return edge.getEnd() == start_of_cycle;}) == edges.end())
    {
        throw std::invalid_argument("Triangle must contain a cycle.");
    }

    auto a = aEdge.edgeLength();
    auto b = bEdge.edgeLength();
    auto c = cEdge.edgeLength();
    auto alfa = SafeAcos((b*b + c*c - a*a)/2*b*c);
    auto beta = SafeAcos((a*a + c*c - b*b)/2*a*c);
    auto gamma = SafeAcos((a*a +b*b - c*c)/2*a*b);

    // store angles as (vertex_index, angle) tuples.
    this->vertex_index_angle_map[aEdge.getStart().getIndex()] = alfa;
    this->vertex_index_angle_map[aEdge.getEnd().getIndex()] = beta;
    this->vertex_index_angle_map[bEdge.getEnd().getIndex()] = gamma;
}

std::vector<Edge> Triangle::getEdges()
{
    return this->edges;
}

double Triangle::getAngleByVertexIndex(int id)
{
    auto edge = std::find_if(edges.begin(), edges.end(),
                             [id](auto e){return e.getStart().getIndex() == id || e.getEnd().getIndex() == id;});

    if(edge == edges.end())
    {
        throw std::invalid_argument("Vertex with index " + std::to_string(id) + " not contained within triangle");
    }

    auto result = this->vertex_index_angle_map[id];
    return result;
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
    for(auto & edge : edges)
    {
        auto index_tuple = edge.edge_as_index_pair();
        result_set.insert(std::get<0>(index_tuple));
        result_set.insert(std::get<1>(index_tuple));
    }
    std::vector<int> result_vector;
    for(auto it = result_set.begin(); it != result_set.end();)
    {
        result_vector.push_back(result_set.extract(it++).value());
    }
    std::sort(result_vector.begin(), result_vector.end());

    return result_vector;
}

std::ostream &operator << (std::ostream &out, Triangle &face)
{
    auto edges = face.getEdges();
    auto index0_adjusted = edges[0].getStart().getIndex() + 1;
    auto index1_adjusted = edges[1].getStart().getIndex() + 1;
    auto index2_adjusted = edges[2].getStart().getIndex() + 1;
    out << "f " << index0_adjusted << " " << index1_adjusted << " " << index2_adjusted << std::endl;
    return out;
}