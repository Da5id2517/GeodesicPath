#ifndef RSPROJECT_UTILS_H
#define RSPROJECT_UTILS_H

#include <vector>
#include "DenseMatrix.h"


std::vector<std::vector<int>> assignElementIndices(int number_of_vertices, int number_of_edges, int number_of_faces);

DenseMatrix buildVertexEdgeAdjacencyMatrix(std::vector<std::vector<int>> &indices,
                                           const std::vector<std::tuple<int, int>> &twoSimplices);

DenseMatrix buildEdgeFaceAdjacencyMatrix(std::vector<std::vector<int>> &indices,
                                         const std::vector<std::vector<int>> &kSimplices);

#endif //RSPROJECT_UTILS_H
