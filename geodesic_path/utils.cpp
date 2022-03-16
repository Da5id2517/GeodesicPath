#include "utils.h"


double SafeAcos (double x)
{
    if (x < -1.0) x = -1.0 ;
    else if (x > 1.0) x = 1.0 ;
    return acos(x) ;
}

std::vector<std::vector<int>> assignElementIndices(int number_of_vertices, int number_of_edges, int number_of_faces)
{
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

SparseMatrix buildVertexEdgeAdjacencyMatrix(std::vector<std::vector<int>> &indices, std::vector<indexPair_t> &twoSimplices)
{
    //rows are edge indices, columns are vertex indices.
    auto [rows, columns] = std::make_tuple(indices[1].size(), indices[0].size());
    auto edge_indices_it = indices[1].begin();
    DenseMatrix denseMatrix(rows, columns);

    for(auto simplex : twoSimplices)
    {
        denseMatrix(*edge_indices_it, simplex.first) = 1;  //change to -1
        denseMatrix(*edge_indices_it, simplex.second) = 1;
        edge_indices_it++;
    }

    return SparseMatrix(denseMatrix);
}

SparseMatrix buildEdgeFaceAdjacencyMatrix(std::vector<std::vector<int>> &indices, std::vector<std::vector<int>> &kSimplices)
{

    // rows are triangle indices, columns are edge indices.
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

