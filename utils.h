#ifndef RSPROJECT_UTILS_H
#define RSPROJECT_UTILS_H

#include <numeric>
#include <string>
#include <unordered_set>
#include <vector>
#include "SparseMatrix.h"


std::vector<std::vector<int>> assignElementIndices(int number_of_vertices, int number_of_edges, int number_of_faces);

// add simplex checker
bool simplexChecker(std::vector<std::vector<int>> &simplex, int k = 0);

SparseMatrix buildVertexEdgeAdjacencyMatrix(std::vector<std::vector<int>> &indices,
                                            std::vector<std::tuple<int, int>> &twoSimplices);

SparseMatrix buildEdgeFaceAdjacencyMatrix(std::vector<std::vector<int>> &indices,
                                          std::vector<std::vector<int>> &kSimplices);

std::vector<int> buildVertexVector(std::vector<int> &simplices, std::vector<int> &indices);

#endif //RSPROJECT_UTILS_H
