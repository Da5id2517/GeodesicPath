#include <numeric>
#include "utils.h"

std::vector<std::vector<int>> assignElementIndices(
        int number_of_vertices,
        int number_of_edges,
        int number_of_faces)
{
    if(number_of_vertices - number_of_edges + number_of_faces != 1)
        throw std::invalid_argument("The arguments provided must satisfy Euler's formula.");

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

