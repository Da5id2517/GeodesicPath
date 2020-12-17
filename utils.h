#ifndef RSPROJECT_UTILS_H
#define RSPROJECT_UTILS_H

#include <numeric>
#include <string>
#include <unordered_set>
#include <vector>
#include "SparseMatrix.h"


std::vector<std::vector<int>> assignElementIndices(int number_of_vertices, int number_of_edges, int number_of_faces);

//TODO: remove or make this functionality a part of complex.h
bool simplexChecker(std::vector<std::vector<int>> &simplex, int k = 0);

SparseMatrix buildVertexEdgeAdjacencyMatrix(std::vector<std::vector<int>> &indices,
                                            std::vector<std::tuple<int, int>> &twoSimplices);

SparseMatrix buildEdgeFaceAdjacencyMatrix(std::vector<std::vector<int>> &indices,
                                          std::vector<std::vector<int>> &kSimplices);

#endif //RSPROJECT_UTILS_H
