#include "Complex.h"

Complex::Complex(std::vector<Vertex> &vertices, std::vector<Edge> &edges, std::vector<Face> &faces)
{
    auto complex_indices = assignElementIndices(vertices.size(), edges.size(), faces.size());
    this->indices = complex_indices;
    this->vertices = vertices;
    this->edges = edges;
    this->faces = faces;

    auto edgesAsIndexPairs = this->edges_as_index_pairs();
    auto facesAsKTuples = this->faces_as_index_k_tuples();

    this->edgeVertexAdjacencyMatrix = buildVertexEdgeAdjacencyMatrix(indices, edgesAsIndexPairs);
    this->faceEdgeAdjacencyMatrix = buildEdgeFaceAdjacencyMatrix(indices, facesAsKTuples);

}

std::vector<Vertex> Complex::getVertices()
{
    return this->vertices;
}

std::vector<Face> Complex::getFaces()
{
    return this->faces;
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

std::vector<int> Complex::vertexIndices()
{
    return this->indices[0];
}

SparseMatrix Complex::getEdgeVertexAdjacencyMatrix()
{
    return this->edgeVertexAdjacencyMatrix;
}

SparseMatrix Complex::getFaceEdgeAdjacencyMatrix()
{
    return this->faceEdgeAdjacencyMatrix;
}

std::vector<int> Complex::buildVertexVector(std::vector<int> &vertices_subset)
{
    auto vertex_indices = this->vertexIndices();
    if(vertices_subset.size() > vertex_indices.size())
    {
        throw std::invalid_argument("Too many vertices in subset.");
    }

    // both collection must be sorted for this to work
    // indices is generated sorted, simplices however, have to be manually sorted.
    // this covers cases with edges and faces since they are integers for now.
    std::sort(vertices_subset.begin(), vertices_subset.end());

    if(!std::includes(vertex_indices.begin(), vertex_indices.end(), vertices_subset.begin(), vertices_subset.end()))
    {
        throw std::invalid_argument("Provided subset isn't a subset of indices.");
    }

    std::vector<int> column_vector(indices.size());

    for(auto &vertex: vertices_subset)
    {
        column_vector[vertex] = 1;
    }

    return column_vector;
}

std::ostream &operator << (std::ostream &out, Complex &complex)
{
    auto vertices = complex.getVertices();
    auto faces = complex.getFaces();
    for(auto &vertex : vertices)
    {
        out << vertex;
    }

    for(auto &face : faces)
    {
        out << face;
    }
    return out;
}

