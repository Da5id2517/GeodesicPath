#ifndef RSPROJECT_UTILS_H
#define RSPROJECT_UTILS_H
#define RADTODEG(R)((180.0 * R) / M_PI)   //Converts Radians to Degrees


#include <numeric>
#include <string>
#include <unordered_set>
#include <vector>
#include "SparseMatrix.h"



double SafeAcos (double x);

std::vector<std::vector<int>> assignElementIndices(int number_of_vertices, int number_of_edges, int number_of_faces);

bool simplexChecker(std::vector<std::vector<int>> &simplex, int k = 0);

SparseMatrix buildVertexEdgeAdjacencyMatrix(std::vector<std::vector<int>> &indices,
                                            std::vector<indexPair_t> &twoSimplices);

SparseMatrix buildEdgeFaceAdjacencyMatrix(std::vector<std::vector<int>> &indices,
                                          std::vector<std::vector<int>> &threeSimplices);

#endif //RSPROJECT_UTILS_H
