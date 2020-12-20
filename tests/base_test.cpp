#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "../mesh.h"
#include "../Complex.h"
#include "../Wedge.h"
#include "polyscope/polyscope.h"
#include "polyscope/surface_mesh.h"
#include "geometrycentral/surface/meshio.h"
#include "geometrycentral/surface/surface_mesh.h"


TEST_CASE("Wedge class tests")
{
    auto test_vertex0 = Vertex(0.0, 0.0, 0);
    auto test_vertex1 = Vertex(0.0 , 1.0, 1);
    auto test_vertex2 = Vertex(1.0, 0.0, 2);
    auto test_edge0 = Edge(test_vertex0, test_vertex1, 0);
    auto test_edge1 = Edge(test_vertex1, test_vertex2, 1);
    auto test_edge_faulty1 = Edge(test_vertex2, test_vertex1);
    Wedge test_wedge(test_edge0, test_edge1);

    SECTION("Constructor throws invalid argument if joints are not joined.")
    {
       REQUIRE_THROWS_AS(Wedge(test_edge0, test_edge_faulty1), std::invalid_argument);
    }

    SECTION("Joint angle calculation.")
    {
        auto angle = test_wedge.wedge_angle();
        REQUIRE(abs(angle - M_PI/2) <= std::numeric_limits<double>::epsilon());
    }

}

TEST_CASE("Vertex class tests")
{
    SECTION("Operator == tests.")
    {
        auto test_vertex0 = Vertex();
        auto test_vertex1 = Vertex(1.1, 2.0, 1);
        REQUIRE(!(test_vertex1 == test_vertex0));

    }
}

TEST_CASE("Complex/Triangle/Edge/Vertex tests")
{
    auto test_vertex1 = Vertex();
    auto test_vertex2 = Vertex(1.0, 1.0, 1);
    auto test_vertex3 = Vertex(1.0, 0.0, 2);
    auto test_vertex4 = Vertex(5.0, 5.0, 3);
    auto test_edge1 = Edge(test_vertex1, test_vertex2, 0);
    auto test_edge2 = Edge(test_vertex2, test_vertex3, 1);
    auto test_edge3 = Edge(test_vertex3, test_vertex1, 2);
    auto test_edge4 = Edge(test_vertex3, test_vertex4, 3);

    std::vector<Edge> edges = {test_edge1, test_edge2, test_edge3};
    auto test_face = Triangle(test_edge1, test_edge2, test_edge3, 0);

    std::vector<Vertex> vertices = {test_vertex1, test_vertex2, test_vertex3};
    std::vector<Triangle> faces = {test_face};
    auto indices = assignElementIndices(3, 3, 1);
    Complex complex(vertices, edges, faces);

    SECTION("Vertex degree test")
    {
        REQUIRE(test_vertex2.getDegree() == 2);
        REQUIRE(test_vertex3.getDegree() == 3);
        REQUIRE(test_vertex4.getDegree() == 1);
    }

    SECTION("Edge length test")
    {
        REQUIRE(test_edge3.edgeLength() == 1.0);
        REQUIRE(test_edge1.edgeLength() - sqrt(2) < std::numeric_limits<double>::epsilon());
    }

    SECTION("Triangle angle calculation")
    {
        auto returned_angle = test_face.getAngleByVertexIndex(0);
        REQUIRE(abs(returned_angle - M_PI/2) <= std::numeric_limits<double>::epsilon());
    }

    SECTION("Edge represented as a tuple of ints")
    {
        auto test_edge1_as_tuple = std::make_tuple<int, int>(0,1);
        auto test_edge2_as_tuple = std::make_tuple<int, int>(1,2);
        REQUIRE(test_edge1.edge_as_index_pair() == test_edge1_as_tuple);
        REQUIRE(test_edge2.edge_as_index_pair() == test_edge2_as_tuple);
    }

    SECTION("Edges that make a face must form a cycle")
    {
        REQUIRE_THROWS_AS(Triangle(test_edge1, test_edge2 , test_edge4, 0), std::invalid_argument);
    }


    SECTION("Triangle as index k tuple test")
    {
        //TODO: add more test cases for this
        std::vector<int> expected_vector = {0, 1, 2};
        REQUIRE(test_face.triangle_as_index_triple() == expected_vector);
    }

    SECTION("Complex adjacency matrix construction")
    {
        auto returnedEdgeVertexMatrix = complex.getEdgeVertexAdjacencyMatrix();
        auto returnedFaceEdgeMatrix = complex.getFaceEdgeAdjacencyMatrix();
        std::vector<std::tuple<int, int, int>> expectedEdgeVertexMatrix = {
                {0,0,1}, {0,1,1},
                {1,1,1}, {1,2,1},
                {2,0,1}, {2,2,1}
        };

        std::vector<std::tuple<int, int, int>> expectedFaceEdgeMatrix = {{0,0,1}, {0,1,1}, {0,2,1}};
        REQUIRE(returnedEdgeVertexMatrix.getData() == expectedEdgeVertexMatrix);
        REQUIRE(returnedFaceEdgeMatrix.getData() == expectedFaceEdgeMatrix);
    }

    SECTION("buildVertexVector")
    {
        std::vector<int> faulty_vertex_indices1 = {1,2,3,4};
        std::vector<int> faulty_vertex_indices2 = {1,4};
        REQUIRE_THROWS_AS(complex.buildVertexVector(faulty_vertex_indices1), std::invalid_argument);
        REQUIRE_THROWS_AS(complex.buildVertexVector(faulty_vertex_indices2), std::invalid_argument);

        std::vector<int> vertex_indices = {0,1};
        std::vector<int> expected_result = {1,1,0};
        auto returned_vector = complex.buildVertexVector(vertex_indices);
        REQUIRE(returned_vector == expected_result);
    }

    SECTION("Index setter test")
    {
        test_vertex4.setIndex(4);
        test_edge2.setIndex(5);
        test_face.setIndex(15);
        REQUIRE(test_vertex4.getIndex() == 4);
        REQUIRE(test_edge2.getIndex() == 5);
        REQUIRE(test_face.getIndex() == 15);
    }
}

//TODO:redundant
TEST_CASE("simplexChecker tests")
{
    SECTION("Faulty simplex throw std::invalid_argument")
    {
        std::vector<std::vector<int>> faulty_simplex1 = {{1,2,3}, {1,2,5}, {1,2}};
        std::vector<std::vector<int>> faulty_simplex2 = {{1,2}, {2,3}, {3,0}, {3,3}};
        REQUIRE_THROWS_AS(simplexChecker(faulty_simplex1, 3), std::invalid_argument);
        REQUIRE_THROWS_AS(simplexChecker(faulty_simplex2, 2), std::invalid_argument);
    }

    SECTION("Good simplex return true")
    {
        std::vector<std::vector<int>> good_simplex1 = {{1}, {2}, {3}};
        std::vector<std::vector<int>> good_simplex2 = {{1,2,3,4}, {3,2,1,4}, {0,2,1,4}, {0,1,3,4}};
        std::vector<std::vector<int>> good_simplex3 = {{1,2,3}, {3,2,1,4}, {0,2,1,4}, {0,1,3,4,5,6}};
        REQUIRE(simplexChecker(good_simplex1));
        REQUIRE(simplexChecker(good_simplex2, 4));
        REQUIRE(simplexChecker(good_simplex3, 3));
    }
}

TEST_CASE("DenseMatrix class tests")
{
    std::vector<std::vector<int>> test_matrix_input = {{1,2,3}, {2,3,4}, {5,6,7}};
    DenseMatrix test_dense_matrix(test_matrix_input);

    SECTION("No argument constructor returns empty vector.")
    {
        std::vector<std::vector<int>> expected_result = { };
        auto returned_value = DenseMatrix();
        REQUIRE(returned_value.getData() == expected_result);
    }

    SECTION("Accessing elements throws invalid argument if index out of bounds returns element otherwise.")
    {
        REQUIRE(test_dense_matrix(0,0) == 1);
        REQUIRE_THROWS_AS(test_dense_matrix(4,2), std::invalid_argument);
    }
}

TEST_CASE("SparseMatrix class tests")
{
    DenseMatrix testDenseMatrix({{1,0,0}, {0,1,0}, {0,0,1}});
    auto returned_matrix = SparseMatrix(testDenseMatrix);

    SECTION("Dense to sparse conversion.")
    {
        std::vector<std::tuple<int, int, int>> expected_matrix = {{0,0,1},{1,1,1},{2,2,1}};
        REQUIRE(returned_matrix.getData() == expected_matrix);
    }

    SECTION("Accessing elements throws invalid argument if index out of bounds returns element otherwise.")
    {
        REQUIRE(returned_matrix(2,2) == 1);
        REQUIRE(returned_matrix(1,0) == 0);
        REQUIRE_THROWS_AS(returned_matrix(3,1), std::invalid_argument);
    }
}


TEST_CASE("assignElementIndices tests")
{
    SECTION("Triangle indices base case")
    {
        auto returned_collection = assignElementIndices(3,3,1);
        std::vector<std::vector<int>> expected_collection = {{0,1,2},{0,1,2},{0}};
        REQUIRE(returned_collection == expected_collection);
    }

    SECTION("Square indices base case")
    {
        auto returned_collection = assignElementIndices(4,4,1);
        std::vector<std::vector<int>> expected_collection = {{0,1,2,3},{0,1,2,3},{0}};
        REQUIRE(returned_collection == expected_collection);
    }

    SECTION("Invalid arguments throw an exception")
    {
        REQUIRE_THROWS_AS(assignElementIndices(-1,-2,0), std::invalid_argument);
        REQUIRE_THROWS_AS(assignElementIndices(0,0,0), std::invalid_argument);
        REQUIRE_THROWS_AS(assignElementIndices(0,0,5), std::invalid_argument);
        REQUIRE_NOTHROW(assignElementIndices(3,3,1));
    }
}

TEST_CASE("A0 and A1 tests")
{
    auto test_triangle_indices = assignElementIndices(3,3,1);
    auto test_square_indices = assignElementIndices(4,4,1);

    std::vector<std::tuple<int, int>> test_triangle_simplicies = {{0,1},{1,2},{2,0}};
    std::vector<std::tuple<int, int>> test_square_simplices = {{0,1},{1,2},{2,3},{3,0}};

    std::vector<std::vector<int>> test_triangle_face_simplices = {{0,1,2}};
    std::vector<std::vector<int>> test_square_face_simplices = {{0,1,2,3}};

    SECTION("A0 for test_triangle")
    {
        std::vector<std::tuple<int, int, int>> expected_matrix = {
                {0,0,1}, {0,1,1},
                {1,1,1}, {1,2,1},
                {2,0,1}, {2,2,1}
        };

        auto returned_matrix = buildVertexEdgeAdjacencyMatrix(test_triangle_indices, test_triangle_simplicies);

        REQUIRE(returned_matrix.getData() == expected_matrix);
    }

    SECTION("A0 for test_square")
    {
        std::vector<std::tuple<int, int, int>> expected_matrix = {
                {0,0,1}, {0,1,1},
                {1,1,1}, {1,2,1},
                {2,2,1}, {2,3,1},
                {3,0,1}, {3,3,1}
        };
        auto returned_matrix = buildVertexEdgeAdjacencyMatrix(test_square_indices, test_square_simplices);

        REQUIRE(returned_matrix.getData() == expected_matrix);
    }

    SECTION("A1 for test_triangle")
    {
        std::vector<std::tuple<int, int, int>> expected_matrix = {{0,0,1},{0,1,1}, {0,2,1}};
        auto returned_matrix = buildEdgeFaceAdjacencyMatrix(test_triangle_indices,test_triangle_face_simplices);

        REQUIRE(returned_matrix.getData() == expected_matrix);
    }

    SECTION("A1 for test_square")
    {
        std::vector<std::tuple<int, int, int>> expected_matrix = {{0,0,1}, {0,1,1}, {0,2,1}, {0,3,1}};
        auto returned_matrix = buildEdgeFaceAdjacencyMatrix(test_square_indices, test_square_face_simplices);

        REQUIRE(returned_matrix.getData() == expected_matrix);
    }

    SECTION("A1 for square with diagonal")
    {
        auto indices = assignElementIndices(5,8,4);
        std::vector<std::vector<int>> simplices = {{4,5,0}, {5,6,1}, {6,7,2}, {7,4,3}};

        auto returned_matrix = buildEdgeFaceAdjacencyMatrix(indices, simplices);
        std::vector<std::tuple<int, int, int>> expected_matrix = {
                {0,0,1}, {0,4,1}, {0,5,1},
                {1,1,1}, {1,5,1}, {1,6,1},
                {2,2,1}, {2,6,1}, {2,7,1},
                {3,3,1}, {3,4,1}, {3,7,1}
        };

        REQUIRE(returned_matrix.getData() == expected_matrix);
    }
}


TEST_CASE("Base functionality tests")
{

    std::ofstream output("test.obj");

    //TODO: some sketchy shit going on with rand()
    int rows = 25;
    int columns = 50;

    std::unique_ptr<geometrycentral::surface::SurfaceMesh> mesh;
    std::unique_ptr<geometrycentral::surface::VertexPositionGeometry> geometry;

    SECTION("Visualize basic triangles")
    {
        polyscope::init();

        auto vertex0 = Vertex();
        auto vertex1 = Vertex(5.0, 5.0, 1);
        auto vertex2 = Vertex(0.0, -12.0, 2);
        auto vertex3 = Vertex(0.0, -5.0, 3);
        auto vertex4 = Vertex(-2.0, -1.0, 4);
        auto vertex5 = Vertex(15.2, 18.7, 5);
        std::vector<Vertex> vertices = {vertex0, vertex1, vertex2, vertex3, vertex4, vertex5};
        auto edge0 = Edge(vertex0, vertex1, 0);
        auto edge1 = Edge(vertex1, vertex2, 1);
        auto edge2 = Edge(vertex2, vertex0, 2);
        auto edge3 = Edge(vertex2, vertex3, 3);
        auto edge4 = Edge(vertex3,vertex1, 4);
        auto edge5 = Edge(vertex4, vertex5, 5);
        auto edge6 = Edge(vertex5, vertex0, 6);
        auto edge7 = Edge(vertex0, vertex4, 7);
        std::vector<Edge> edges = {edge0, edge1, edge2, edge3, edge4, edge5, edge6, edge7};
        auto face0 = Triangle(edge0, edge1, edge2, 0);
        auto face1 = Triangle(edge3, edge4, edge1, 1);
        auto face2 = Triangle(edge5, edge6, edge7, 2);
        std::vector<Triangle> faces = {face0, face1, face2};
        auto complex = Complex(vertices, edges, faces);

        output << complex;

        REQUIRE_NOTHROW(std::tie(mesh, geometry) = geometrycentral::surface::readSurfaceMesh("test.obj"));
        polyscope::registerSurfaceMesh(
                "Basic triangle",
                geometry->inputVertexPositions,
                mesh->getFaceVertexList());

        polyscope::show();

    }

    SECTION("Generate square of random dimensions, output an appropriate .obj file and visualize it")
    {
        square testSquare(rows, columns);
        testSquare.generate_obj(output);
        REQUIRE_NOTHROW(std::tie(mesh, geometry) = geometrycentral::surface::readSurfaceMesh("test.obj"));
        polyscope::registerSurfaceMesh(
                "Test square surface",
                geometry->inputVertexPositions,
                mesh->getFaceVertexList());

        polyscope::show();
    }

    SECTION("Generate triangle of random dimensions, output an appropriate .obj file and visualize it")
    {
        triangle testTriangle(rows, columns);
        testTriangle.generate_obj(output);
        REQUIRE_NOTHROW(std::tie(mesh, geometry) = geometrycentral::surface::readSurfaceMesh("test.obj"));
        polyscope::registerSurfaceMesh(
                "Test triangle surface",
                geometry->inputVertexPositions,
                mesh->getFaceVertexList());

        polyscope::show();
    }

    SECTION("Generate hexagon of random dimensions, output an appropriate .obj file and visualize it")
    {
        hexagon testHexagon(rows, columns);
        testHexagon.generate_obj(output);
        REQUIRE_NOTHROW(std::tie(mesh, geometry) = geometrycentral::surface::readSurfaceMesh("test.obj"));
        polyscope::registerSurfaceMesh(
                "Test hexagon surface",
                geometry->inputVertexPositions,
                mesh->getFaceVertexList());

        polyscope::show();
    }

    output.close();
}


