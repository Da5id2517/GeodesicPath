#ifndef RSPROJECT_UTILS_H
#define RSPROJECT_UTILS_H

#include <vector>


std::vector<std::vector<int>> assignElementIndices(
        int number_of_vertices,
        int number_of_edges,
        int number_of_faces);

auto buildVertexEdgeAdjacencyMatrix(std::vector<std::vector<int>>);
auto buildEdgeFacesAdjacencyMatrix(std::vector<std::vector<int>>);

#endif //RSPROJECT_UTILS_H
