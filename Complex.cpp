#include "Complex.h"

Complex::Complex(std::vector<Vertex> &vertices, std::vector<std::vector<int>> &face_indices)
{
    this->vertices = vertices;
    this->faceIndices = face_indices;
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

std::vector<std::vector<int>> Complex::getFaceIndices()
{
    return this->faceIndices;
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

Complex Complex::findGeodesic(Vertex &start, Vertex &joint, Vertex &end)
{
    auto segment0Index = branchThatContains(start.getIndex(), joint.getIndex());
    auto segment1Index = branchThatContains(joint.getIndex(), end.getIndex());
    if(segment0Index == -1 || segment1Index == -1)
    {
        throw std::invalid_argument("No such segments.");
    }

    auto start_index = thirdTriangleVertexIndex(start.getIndex(), joint.getIndex());
    auto indicesOfTrianglesThatContainStart = triangleIndicesThatContain(start_index);
    double angle_sum_of_start = 0.0;

    for(auto index : indicesOfTrianglesThatContainStart)
    {
        angle_sum_of_start += faces[index].getAngleByVertexIndex(start_index);
    }

    if(abs(angle_sum_of_start - M_PI) > std::numeric_limits<double>::epsilon())
    {
        auto index_of_edge_to_flip = branchThatContains(start_index, joint.getIndex());
        auto firstTriangle = faceIndices[indicesOfTrianglesThatContainStart[0]];
        auto secondTriangle = faceIndices[indicesOfTrianglesThatContainStart[1]];
        std::vector<std::vector<int>> new_indices = this->getFaceIndices();
        new_indices[indicesOfTrianglesThatContainStart[0]] = {start.getIndex(), start_index, end.getIndex()};
        new_indices[indicesOfTrianglesThatContainStart[1]] = {start.getIndex(), joint.getIndex(), end.getIndex()};
        return Complex(this->vertices, new_indices);
    }
}

//assumes this vertex is unique, which it needn't be.
int Complex::thirdTriangleVertexIndex(int index0, int index1)
{
    std::vector<int> index01 = {index0, index1};
    std::sort(index01.begin(), index01.end());
    for(auto &indexTriple : this->getFaceIndices())
    {
        if(std::includes(indexTriple.begin(), indexTriple.end(), index01.begin(), index01.end()))
        {
            for(auto &index : indexTriple)
            {
                if(index != index0 && index != index1)
                {
                    return index;
                }
            }
        }
    }
    // Ew...
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

std::vector<int> Complex::triangleIndicesThatContain(int vertexId)
{
    std::vector<int> result;
    auto face_indices = this->getFaceIndices();
    for(int i=0; i < face_indices.size(); i++)
    {
        if(std::count(face_indices[i].begin(), face_indices[i].end(), vertexId))
        {
            result.push_back(i);
        }
    }
    return result;
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

