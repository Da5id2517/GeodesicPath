#include <numeric>
#include <string>
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

DenseMatrix buildVertexEdgeAdjacencyMatrix(
        std::vector<std::vector<int>> &indices,
        const std::vector<std::tuple<int, int>> &twoSimplices)
{
    auto [rows, columns] = std::make_tuple(indices[1].size(), indices[0].size());
    auto edge_indices_it = indices[1].begin();
    DenseMatrix denseMatrix(rows, columns);

    for(auto simplex : twoSimplices)
    {
        denseMatrix(*edge_indices_it, std::get<0>(simplex)) = 1;
        denseMatrix(*edge_indices_it, std::get<1>(simplex)) = 1;
        edge_indices_it++;
    }

    return denseMatrix;
}

DenseMatrix buildEdgeFaceAdjacencyMatrix(
        std::vector<std::vector<int>> &indices,
        const std::vector<std::vector<int>> &kSimplices)
{
    //TODO: add a function that checks if a simplex is valid.
    // simplices must have at least three elements to form a face.
    if(std::count_if(kSimplices.begin(), kSimplices.end(), [](auto simplex){return simplex.size() < 3;}))
    {
        throw std::invalid_argument("Faces can only be formed with three or more edges.");
    }
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

    return denseMatrix;
}
