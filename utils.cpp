#include "utils.h"

std::vector<std::vector<int>> assignElementIndices(
        int number_of_vertices,
        int number_of_edges,
        int number_of_faces)
{
    //TODO: right side of the formula should be 2 - 2g
    if(number_of_vertices - number_of_edges + number_of_faces != 1)
        throw std::invalid_argument("The arguments provided must satisfy Euler's formula.");
    if(number_of_vertices < 0 || number_of_edges < 0 || number_of_faces < 0)
        throw std::invalid_argument("Input must be non negative integers.");

    std::vector<std::vector<int>> result_collection(3);
    std::vector<int> vertex_indices(number_of_vertices);
    std::vector<int> edge_indices(number_of_edges);
    std::vector<int> face_indices(number_of_faces);

    std::iota(std::begin(vertex_indices), std::end(vertex_indices), 0);
    std::iota(std::begin(edge_indices), std::end(edge_indices), 0);
    std::iota(std::begin(face_indices), std::end(face_indices), 0);

    std::copy(
            std::begin(vertex_indices),
            std::end(vertex_indices),
            std::back_inserter(result_collection[0])
            );

    std::copy(
            std::begin(edge_indices),
            std::end(edge_indices),
            std::back_inserter(result_collection[1])
    );

    std::copy(
            std::begin(face_indices),
            std::end(face_indices),
            std::back_inserter(result_collection[2])
    );

    return result_collection;
}

SparseMatrix buildVertexEdgeAdjacencyMatrix(
        std::vector<std::vector<int>> &indices,
        std::vector<std::tuple<int, int>> &twoSimplices)
{

    auto [rows, columns] = std::make_tuple(indices[1].size(), indices[0].size());
    auto edge_indices_it = indices[1].begin();
    DenseMatrix denseMatrix(rows, columns);

    for(auto simplex : twoSimplices)
    {
        denseMatrix(*edge_indices_it, std::get<0>(simplex)) = 1;  //change to -1
        denseMatrix(*edge_indices_it, std::get<1>(simplex)) = 1;
        edge_indices_it++;
    }

    return SparseMatrix(denseMatrix);
}

SparseMatrix buildEdgeFaceAdjacencyMatrix(
        std::vector<std::vector<int>> &indices,
        std::vector<std::vector<int>> &kSimplices)
{
    // faces can only be formed by at least 3 edges
    simplexChecker(kSimplices, 3);

    auto [rows, columns] = std::make_tuple(indices[2].size(), indices[1].size());
    auto face_indices_it = indices[2].begin();
    DenseMatrix denseMatrix(rows, columns);

    for(auto &simplex : kSimplices)
    {
        for(auto element : simplex)
        {
            denseMatrix(*face_indices_it, element) = 1;
        }
        face_indices_it++;
    }

    return SparseMatrix(denseMatrix);
}

bool simplexChecker(std::vector<std::vector<int>> &simplices, int k)
{
    if(std::count_if(simplices.begin(), simplices.end(), [k](auto tuple){return tuple.size() < k;}))
    {
        throw std::invalid_argument("All simplices must have at least " + std::to_string(k) + " elements.");
    }

    auto lambda = [](std::vector<int> &tuple)
            {
        std::unordered_set<int> tmp;
        tmp.insert(tuple.cbegin(), tuple.cend());
        return tuple.size() > tmp.size();
            };

    if(std::count_if(simplices.begin(), simplices.end(), lambda))
    {
        throw std::invalid_argument("Tuples cannot contain duplicates.");
    }

    //TODO: simplex cannot contain duplicate tuples.

    return true;
}

std::vector<int> buildVertexVector(std::vector<int> &simplices, std::vector<int> &indices)
{
    if(simplices.size() > indices.size())
    {
        throw std::invalid_argument("Too many vertices in simplices.");
    }

    // both collection must be sorted for this to work
    // indices is generated sorted, simplices however, have to be manually sorted.
    // this covers cases with edges and faces since they are integers for now.
    std::sort(simplices.begin(), simplices.end());

    if(!std::includes(indices.begin(), indices.end(), simplices.begin(), simplices.end()))
    {
        throw std::invalid_argument("Simplices isnt a subset of indices.");
    }

    std::vector<int> column_vector(indices.size());

    for(auto &vertex: simplices)
    {
        column_vector[vertex] = 1;
    }

    return column_vector;
}

