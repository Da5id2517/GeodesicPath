#ifndef RSPROJECT_COMPLEX_H
#define RSPROJECT_COMPLEX_H
#include "Triangle.h"
#include "utils.h"

class Complex
{
public:
    Complex(std::vector<Vertex> &vertices, std::vector<Edge> &edges, std::vector<Triangle> &faces);

    SparseMatrix getEdgeVertexAdjacencyMatrix();
    SparseMatrix getFaceEdgeAdjacencyMatrix();
    std::vector<std::tuple<int, int>> edges_as_index_pairs();
    std::vector<std::vector<int>> triangles_as_index_triples();
    std::vector<Vertex> getVertices();
    std::vector<Triangle> getFaces();

    std::vector<int> vertexIndices();
    std::vector<int> buildVertexVector(std::vector<int> &vertices);

    Complex operator *(const Complex& subComplex);


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
