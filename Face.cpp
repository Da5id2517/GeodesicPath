#include <unordered_set>
#include "Face.h"

double SafeAcos (double x)
{
    if (x < -1.0) x = -1.0 ;
    else if (x > 1.0) x = 1.0 ;
    return acos (x) ;
}
//C++ being an actuall fucking pepega.

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

    //assumes edges are sorted by index
    auto a = edges[0].edgeLength();
    auto b = edges[1].edgeLength();
    auto c = edges[2].edgeLength();
    auto alfa = SafeAcos((b*b + c*c - a*a)/2*b*c);
    auto beta = SafeAcos((a*a + b*b - c*c)/2*a*c);
    auto gamma = SafeAcos((a*a +b*b - c*c)/2*a*b);

    this->angles = std::make_tuple(alfa, beta, gamma);
    this->edges = edges;
    this->index = index;
}

std::vector<Edge> Face::getEdges()
{
    return this->edges;
}

std::tuple<double, double, double> Face::getAngles()
{
    return this->angles;
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

std::ostream &operator << (std::ostream &out, Face &face)
{
    auto edges = face.getEdges();
    auto index0_adjusted = edges[0].getStart().getIndex() + 1;
    auto index1_adjusted = edges[1].getStart().getIndex() + 1;
    auto index2_adjusted = edges[2].getStart().getIndex() + 1;
    out << "f " << index0_adjusted << " " << index1_adjusted << " " << index2_adjusted << std::endl;
    return out;
}