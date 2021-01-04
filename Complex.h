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
    std::vector<std::vector<int>> getFaceIndices();
    std::vector<Vertex> getVertices();
    std::vector<Triangle> getFaces();
    std::vector<int> vertexIndices();
    std::vector<int> buildVertexVector(std::vector<int> &vertices);
    std::vector<int> triangleIndicesThatContain(int vertexId);
    Vertex & findOther(int edgeIndex, Vertex &current);
    void findGeodesic(std::vector<Vertex> &path, std::vector<Vertex> &geodesicPath);
    int branchThatContains(int start_index, int end_index);
    std::vector<int> thirdTriangleVertexIndex(int index0, int index1);
    std::vector<Vertex> outerArcOfFlexibleJoint(std::vector<Vertex> &flexibleJoint);
    Complex flipEdge(indexPair_t toRemove);

private:
    std::vector<Vertex> vertices;
    std::vector<Edge> edges;
    std::vector<Triangle> faces;
    std::vector<std::vector<int>> indices;
    std::vector<std::vector<int>> faceIndices;
    SparseMatrix edgeVertexAdjacencyMatrix;
    SparseMatrix faceEdgeAdjacencyMatrix;
};

std::ostream &operator << (std::ostream &out, Complex &complex);

#endif //RSPROJECT_COMPLEX_H
