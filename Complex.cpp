#include "Complex.h"

Complex::Complex(std::vector<Vertex> &vertices, std::vector<Edge> &edges, std::vector<Face> &faces)
{
    this->vertices = vertices;
    this->edges = edges;
    this->faces = faces;

    auto indices = assignElementIndices(vertices.size(), edges.size(), faces.size());
    // TODO: this broken
    auto edgesAsIndexPairs = this->edges_as_index_pairs();
    // fixerino

    auto facesAsKTuples = this->faces_as_index_k_tuples();

    this->edgeVertexAdjacencyMatrix = buildVertexEdgeAdjacencyMatrix(indices, edgesAsIndexPairs);
    this->faceEdgeAdjacencyMatrix = buildEdgeFaceAdjacencyMatrix(indices, facesAsKTuples);

}

Complex::Complex(std::vector<Vertex> &vertices, std::vector<Edge> &edges, std::vector<Face> &faces, std::vector<std::vector<int>> &indices)
{
    //check if vector sizes match
    if(vertices.size() != indices[0].size())
    {
        throw std::invalid_argument("Vertices and vertex indices size mismatch.");
    }

    if(edges.size() != indices[1].size())
    {
        throw std::invalid_argument("Edges and edge indices size mismatch.");
    }

    if(faces.size() != indices[2].size())
    {
        throw std::invalid_argument("Faces and face indices size mismatch.");
    }

    //index setting
    int i = 0;
    for(auto &vertex : vertices)
    {
        vertex.setIndex(i);
        i++;
    }

    this->vertices = vertices;

    int j = 0;
    for(auto &edge : edges)
    {
        edge.setIndex(j);
        j++;
    }

    this-> edges = edges;

    int k = 0;
    for(auto &face : faces)
    {
        face.setIndex(k);
        k++;
    }

    this->faces = faces;

    //TODO: move to a separate method
    auto edgesAsIndexPairs = this->edges_as_index_pairs();
    auto facesAsKTuples = this->faces_as_index_k_tuples();

    this->edgeVertexAdjacencyMatrix = buildVertexEdgeAdjacencyMatrix(indices, edgesAsIndexPairs);
    this->faceEdgeAdjacencyMatrix = buildEdgeFaceAdjacencyMatrix(indices, facesAsKTuples);
}

std::vector<std::tuple<int, int>> Complex::edges_as_index_pairs()
{
    std::vector<std::tuple<int, int>> edgesAsIndexPairs(this->edges.size());
    int i = 0;
    for(auto &edge : this->edges)
    {
        edgesAsIndexPairs[i] =  edge.edge_as_index_pair();
        i++;
    }
    return edgesAsIndexPairs;
}

std::vector<std::vector<int>> Complex::faces_as_index_k_tuples()
{
    std::vector<std::vector<int>> facesAsKTuples(this->faces.size());
    int i = 0;
    for(auto &face: this->faces)
    {
        facesAsKTuples[i] = face.face_as_index_k_tuple();
        i++;
    }
    return facesAsKTuples;
}

SparseMatrix Complex::getEdgeVertexAdjacencyMatrix()
{
    return this->edgeVertexAdjacencyMatrix;
}

SparseMatrix Complex::getFaceEdgeAdjacencyMatrix()
{
    return this->faceEdgeAdjacencyMatrix;
}