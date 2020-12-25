#include "Complex.h"

Complex::Complex(std::vector<Vertex> &vertices, std::vector<std::vector<int>> &face_indices)
{
    this->vertices = vertices;

    //Assume that vertex indices are sorted
    int i = 0, j = 0;
    for(auto &face : face_indices)
    {
        Triangle triangle(vertices[face[0]], vertices[face[1]], vertices[face[2]], i);

        auto triangle_edges = triangle.getEdges();
        for(auto &edge: triangle_edges)
        {
            // if edge is already in edges do not add it.
            if(!std::count(this->edges.begin(), this->edges.end(), edge))
            {
                // if not adjust index and add it.
                triangle.setEdgeIndex(j, edge);
                this->edges.push_back(edge);
                j++;
            }
        }
        this->faces.push_back(triangle);
        i++;
    }

    this->indices = assignElementIndices(this->vertices.size(), j, i);
    auto edgesAsIndexPairs = this->edges_as_index_pairs();
    auto trianglesAsIndexTriples = this->triangles_as_index_triples();
    this->edgeVertexAdjacencyMatrix = buildVertexEdgeAdjacencyMatrix(indices, edgesAsIndexPairs);
    this->faceEdgeAdjacencyMatrix = buildEdgeFaceAdjacencyMatrix(indices, trianglesAsIndexTriples);
}

std::vector<Vertex> Complex::getVertices()
{
    return this->vertices;
}

std::vector<Triangle> Complex::getFaces()
{
    return this->faces;
}

std::vector<indexPair_t> Complex::edges_as_index_pairs()
{
    std::vector<indexPair_t> edgesAsIndexPairs;
    for(auto &edge : this->edges)
    {
        edgesAsIndexPairs.push_back(edge.edge_as_index_pair());
    }

    //TODO: try it with transform later
//    std::transform(this->edges.begin(), this->edges.end(), edgesAsIndexPairs.begin(),
//                   [](auto e){return e.edge_as_index_pair();});
    return edgesAsIndexPairs;
}

std::vector<std::vector<int>> Complex::triangles_as_index_triples()
{
    std::vector<std::vector<int>> trianglesAsTriples(this->faces.size());
    int i = 0;
    for(auto &face: this->faces)
    {
        trianglesAsTriples[i] = face.triangle_as_index_triple();
        i++;
    }
    return trianglesAsTriples;
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


int Complex::branchThatContains(int start_index, int end_index)
{
    std::vector<int> index_vector = {start_index, end_index};
    std::sort(index_vector.begin(), index_vector.end());
    auto matrix = this->getEdgeVertexAdjacencyMatrix();

    for(int i = 0; i < matrix.getRows(); i++)
    {
        auto matrix_row = matrix.getColumnIndicesWithinRow(i);
        if(std::includes(matrix_row.begin(), matrix_row.end(), index_vector.begin(), index_vector.end()))
        {
            return i;
        }
    }

    //if branch doesn't exists return -1
    return -1;
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

