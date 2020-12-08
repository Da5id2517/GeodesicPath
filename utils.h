#ifndef RSPROJECT_UTILS_H
#define RSPROJECT_UTILS_H

#include <vector>


std::vector<std::vector<int>> assignElementIndices(
        int number_of_vertices,
        int number_of_edges,
        int number_of_faces);

std::vector<std::vector<int>> buildVertexEdgeAdjacencyMatrix(
        std::vector<std::vector<int>> &indices,
        const std::vector<std::tuple<int, int>> &simplices);
auto buildEdgeFacesAdjacencyMatrix(std::vector<std::vector<int>>);

#endif //RSPROJECT_UTILS_H
