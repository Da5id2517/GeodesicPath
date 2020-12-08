#include <numeric>
#include <string>
#include "utils.h"

std::vector<std::vector<int>> assignElementIndices(
        int number_of_vertices,
        int number_of_edges,
        int number_of_faces)
{
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

std::vector<std::vector<int>> buildVertexEdgeAdjacencyMatrix(
        std::vector<std::vector<int>> &indices,
        const std::vector<std::tuple<int, int>> &twoSimplices)
{
    auto [rows, columns] = std::make_tuple(indices[1].size(), indices[0].size());

    // TODO: this is prone to breaking easily.
//    auto max_amount_of_edges = columns * (columns - 1)/2;
//    auto edges_provided = twoSimplices.size();
//    if(max_amount_of_edges > edges_provided)
//    {
//        throw std::invalid_argument(
//                "Mesh with "
//                + std::to_string(columns)
//                + " vertices cannot have "
//                + std::to_string(edges_provided)
//                + " edges.");
//    }


    auto edge_indices_it = indices[1].begin();
    std::vector<std::vector<int>> non_sparse_adjacency_matrix(rows);

    for(int i = 0; i < rows; i++)
    {
        non_sparse_adjacency_matrix[i].resize(columns);
    }

    for(auto simplex : twoSimplices)
    {
        non_sparse_adjacency_matrix[*edge_indices_it][std::get<0>(simplex)] = 1;
        non_sparse_adjacency_matrix[*edge_indices_it][std::get<1>(simplex)] = 1;
        edge_indices_it++;
    }

    return non_sparse_adjacency_matrix;
}
