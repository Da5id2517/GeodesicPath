#ifndef RSPROJECT_COMPLEX_H
#define RSPROJECT_COMPLEX_H
#include "Triangle.h"

class Complex
{
public:
    Complex(std::vector<Vertex> &vertices, std::vector<std::vector<int>> &face_indices);

    SparseMatrix getEdgeVertexAdjacencyMatrix();
    SparseMatrix getFaceEdgeAdjacencyMatrix();
    std::vector<indexPair_t> edges_as_index_pairs();
    std::vector<std::vector<int>> triangles_as_index_triples();
    std::vector<Vertex> getVertices();
    std::vector<Triangle> getFaces();

    std::vector<int> vertexIndices();
    std::vector<int> buildVertexVector(std::vector<int> &vertices);

    int branchThatContains(int start_index, int end_index);


private:
    std::vector<Vertex> vertices;
    std::vector<Edge> edges;
    std::vector<Triangle> faces;
    std::vector<std::vector<int>> indices;
    SparseMatrix edgeVertexAdjacencyMatrix;
    SparseMatrix faceEdgeAdjacencyMatrix;
};

std::ostream &operator << (std::ostream &out, Complex &complex);

#endif //RSPROJECT_COMPLEX_H
