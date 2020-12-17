#include <unordered_set>
#include "Face.h"

Face::Face(std::vector<Edge> &edges, int index)
{
    if(edges.size() <= 2)
    {
        throw std::invalid_argument("A face must contain at least three edges.");
    }

    auto start_of_cycle = edges[0].getStart();
    auto end_of_cycle = std::find_if(
            edges.begin(),
            edges.end(),
            [start_of_cycle](auto edge){return edge.getEnd() == start_of_cycle;});
    if(end_of_cycle == edges.end())
    {
        throw std::invalid_argument("A face must contain a cycle.");
    }

    this->edges = edges;
    this->index = index;
}

std::vector<Edge> Face::getEdges()
{
    return edges;
}

int Face::getIndex() const
{
    return this->index;
}

void Face::setIndex(int new_index)
{
    this->index = new_index;
}

int Face::dimension()
{
    return edges.size() - 1;
}

std::vector<int> Face::face_as_index_k_tuple()
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