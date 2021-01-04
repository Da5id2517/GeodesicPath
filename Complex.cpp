#include "Complex.h"

Complex::Complex(std::vector<Vertex> &vertices, std::vector<std::vector<int>> &face_indices)
{
    this->vertices = vertices;
    this->faceIndices = face_indices;
    //Assume that vertex indices are sorted

    std::vector<std::vector<int>> trianglesAsEdgeIndexTriples;

    int i = 0, j = 0;
    for(auto &face : face_indices)
    {
        std::vector<int> triangleAsEdgeIndexTriple;
        Triangle triangle(vertices[face[0]], vertices[face[1]], vertices[face[2]], i);

        auto triangle_edges = triangle.getEdges();
        for(auto &edge: triangle_edges)
        {
            auto isContained = std::find(this->edges.begin(), this->edges.end(), edge);
            // if edge is already in edges do not add it.
            if(isContained == this->edges.end())
            {
                // if not adjust index and add it.
                triangle.setEdgeIndex(j, edge);
                triangleAsEdgeIndexTriple.push_back(j);
                this->edges.push_back(edge);
                j++;
            }
            else
            {
                triangleAsEdgeIndexTriple.push_back(isContained - this->edges.begin());
                triangle.setEdgeIndex(isContained - this->edges.begin(), edge);
            }

        }
        this->faces.push_back(triangle);
        trianglesAsEdgeIndexTriples.push_back(triangleAsEdgeIndexTriple);
        i++;
    }

    this->indices = assignElementIndices(this->vertices.size(), j, i);
    auto edgesAsIndexPairs = this->edges_as_index_pairs();

    this->edgeVertexAdjacencyMatrix = buildVertexEdgeAdjacencyMatrix(indices, edgesAsIndexPairs);
    this->faceEdgeAdjacencyMatrix = buildEdgeFaceAdjacencyMatrix(indices, trianglesAsEdgeIndexTriples);
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


void Complex::findGeodesic(std::vector<Vertex> &path, std::vector<Vertex> &geodesicPath)
{
    if(geodesicPath.empty())
    {
        geodesicPath.push_back(*path.begin());
    }
    auto current = path.begin();

    //recursion exit
    if(path.size() == 2)
    {
        geodesicPath = path;
        return;
    }

    //if outer arc is empty it is a 0 degree wedge.
    auto outerArc = outerArcOfFlexibleJoint(path);

    for(auto nodeIt = outerArc.begin(); nodeIt != outerArc.end(); nodeIt++)
    {
        auto triangleIndicesThatContainNode = triangleIndicesThatContain(nodeIt->getIndex());
        double nodeAngleSum = 0.0;
        for(auto & triangleIndex : triangleIndicesThatContainNode)
        {
            nodeAngleSum += faces[triangleIndex].getAngleByVertexIndex(nodeIt->getIndex());
        }
        if(abs(nodeAngleSum - M_PI) >= std::numeric_limits<double>::epsilon())
        {
            //Beta i < pi => flip the edges
            auto edgesOutOfNode = this->getEdgeVertexAdjacencyMatrix().getRowIndicesWithinColumn(nodeIt->getIndex());
            indexPair_t toRemove;
            toRemove.first = nodeIt->getIndex();
            for(auto & edgeId : edgesOutOfNode)
            {
                auto other = findOther(edgeId, *nodeIt);
                if(other == *(path.end()))
                {
                    toRemove.second = (current + 1)->getIndex();
                    geodesicPath.push_back(other);
                    Complex newTriangulation = this->flipEdge(toRemove);
                    *this = newTriangulation;
                    return;

                }
                if(*(nodeIt + 1) == other)
                {
                    toRemove.second = (nodeIt + 1)->getIndex();
                    geodesicPath.push_back(*(nodeIt + 1));
                    break;
                }
                //if previous or node in path continue
                if(nodeIt != outerArc.begin() && other == *(nodeIt-1)
                && std::find(path.begin(), path.end(), other) != path.end())
                {
                    continue;
                }
            }
        }
    }

    //outer arc was empty <=> all vertices belong to path
    // wedge start and joint form a triangle
    if(geodesicPath.size() == 1)
    {
        path.erase(path.begin() + 1);
        findGeodesic(path, geodesicPath);
    }
}


std::vector<int> Complex::thirdTriangleVertexIndex(int index0, int index1)
{
    std::vector<int> index01 = {index0, index1};
    std::sort(index01.begin(), index01.end());
    std::vector<int> result;

    for(auto &indexTriple : this->getFaceIndices())
    {
        if(std::includes(indexTriple.begin(), indexTriple.end(), index01.begin(), index01.end()))
        {
            for(auto &index : indexTriple)
            {
                if(index != index0 && index != index1)
                {
                    result.push_back(index);
                }
            }
        }
    }
    return result;
}

std::vector<Vertex> Complex::outerArcOfFlexibleJoint(std::vector<Vertex> &flexibleJoint)
{
    auto matrix = this->getEdgeVertexAdjacencyMatrix();
    std::vector<Vertex> result;
    for(auto & node : flexibleJoint)
    {
        auto indicesOfEdgesThatContainNode = matrix.getRowIndicesWithinColumn(node.getIndex());
        for(auto & index : indicesOfEdgesThatContainNode)
        {
            auto other = findOther(index, node);
            if(std::find(result.begin(), result.end(), other) == result.end()
            && std::find(flexibleJoint.begin(), flexibleJoint.end(), other) == flexibleJoint.end())
            {
                result.push_back(other);
            }
        }
    }
    return result;
}

Vertex & Complex::findOther(int edgeIndex, Vertex &current)
{
    auto edge = this->edges[edgeIndex];
    if(edge.getStart() == current)
        return this->vertices[edge.getEnd().getIndex()];
    else
        return this->vertices[edge.getStart().getIndex()];
}

Complex Complex::flipEdge(indexPair_t toRemove)
{
    auto matrix = this->getFaceEdgeAdjacencyMatrix();
    auto edgeIndex = branchThatContains(toRemove.first, toRemove.second);
    auto toAddIndices = thirdTriangleVertexIndex(toRemove.first, toRemove.second);
    auto facesThatContainEdge = matrix.getRowIndicesWithinColumn(edgeIndex);
    auto newFaceIndices = this->getFaceIndices();
    newFaceIndices[facesThatContainEdge[0]] = {toAddIndices[0], toAddIndices[1], toRemove.first};
    newFaceIndices[facesThatContainEdge[1]] = {toAddIndices[1], toAddIndices[0], toRemove.second};
    return Complex(this->vertices, newFaceIndices);
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

