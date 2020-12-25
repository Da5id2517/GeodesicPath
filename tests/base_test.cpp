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
    auto test_vertex0 = Vertex(0.0, 0.0, 0.0, 0);
    auto test_vertex1 = Vertex(0.0 , 1.0, 0.0, 1);
    auto test_vertex2 = Vertex(1.0, 0.0, 0.0, 2);
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
        REQUIRE(abs(angle - M_PI/4) <= std::numeric_limits<double>::epsilon());
    }

}

TEST_CASE("Vertex class tests")
{
    SECTION("Operator == tests.")
    {
        auto test_vertex0 = Vertex();
        auto test_vertex1 = Vertex(1.1, 2.0, 5.0, 1);
        auto index_mismatch = Vertex(1.1, 2.0, 5.0, 2);
        REQUIRE_FALSE(test_vertex1 == test_vertex0);
        REQUIRE(test_vertex0 == Vertex());
        REQUIRE_THROWS_AS(test_vertex1 == index_mismatch, std::invalid_argument);
    }

    SECTION("Vertex degree tests")
    {
        auto vertex0 = Vertex();
        auto vertex1 = Vertex(1.0, 1.0, 1.0, 1);
        auto vertex2 = Vertex(2.0, 2.0, 2.0, 2);
        auto lone_vertex = Vertex(5.0, 5.0, 5.0, 3);
        vertex0.increase_degree();
        vertex0.increase_degree();
        vertex1.increase_degree();
        vertex2.increase_degree();

        REQUIRE(vertex0.getDegree() == 2);
        REQUIRE(vertex1.getDegree() == 1);
        REQUIRE(vertex2.getDegree() == 1);
        REQUIRE(lone_vertex.getDegree() == 0);

        vertex0.decrease_degree();
        REQUIRE(vertex0.getDegree() == 1);
        lone_vertex.decrease_degree();
        REQUIRE(lone_vertex.getDegree() == -1);
    }

    SECTION("Vertex index setter tests")
    {
        auto vertex = Vertex();
        REQUIRE(vertex.getIndex() == 0);
        vertex.setIndex(15);
        REQUIRE(vertex.getIndex() == 15);
        vertex.setIndex(2);
        REQUIRE(vertex.getIndex() == 2);
    }
}

TEST_CASE("Edge class tests")
{
    SECTION("Test edge length")
    {
        auto vertex0 = Vertex();
        auto vertex1 = Vertex(3.0, 4.0, 0.0);

        auto edge0 = Edge(vertex0, vertex1);
        REQUIRE(abs(edge0.edgeLength() - 5.0) < std::numeric_limits<double>::epsilon());

        auto self_edge = Edge(vertex0, vertex0);
        REQUIRE(self_edge.edgeLength() < std::numeric_limits<double>::epsilon());
    }

    SECTION("Edges as index tuples")
    {
        auto vertex0 = Vertex();
        auto vertex1 = Vertex(3.0, 4.0, 5.0, 1);
        auto vertex2 = Vertex(5.0, 5.0, 5.0, 2);
        auto edge0 = Edge(vertex0, vertex1);
        auto edge1 = Edge(vertex0, vertex2);
        auto self_edge = Edge(vertex0, vertex0);
        std::vector<indexPair_t> expected_results = {{0,1}, {0,2}, {0,0}};

        REQUIRE(edge0.edge_as_index_pair() == expected_results[0]);
        REQUIRE(edge1.edge_as_index_pair() == expected_results[1]);
        REQUIRE(self_edge.edge_as_index_pair() == expected_results[2]);
    }

    SECTION("Edge == operator")
    {
        Vertex vertex0;
        Vertex vertex1(1.0, 1.0, 0.0, 1);
        Vertex vertex2(1.2, 1.2, 1.2, 2);
        Vertex vertex_index_collision(5.0, 5.0, 5.0, 0);

        Edge edge01(vertex0, vertex1), edge10(vertex1, vertex0);
        Edge edge02(vertex0, vertex2), edge12(vertex1, vertex2);

        REQUIRE(edge01 == edge10);
        REQUIRE_FALSE(edge02 == edge12);

        edge10.setIndex(15);
        REQUIRE_THROWS_AS(edge01 == edge10, std::invalid_argument);
    }

    SECTION("Set edge index tests")
    {
        Vertex test_vertex;
        Edge test_edge(test_vertex, test_vertex);
        REQUIRE(test_edge.getIndex() == 0);
        test_edge.setIndex(5);
        REQUIRE(test_edge.getIndex() == 5);
    }
}

TEST_CASE("Triangle class tests")
{
    SECTION("Constructor tests")
    {
        Vertex vertex0, vertex1(1.0, 0.0, 0.0, 1), vertex2(0.0, 0.0, 1.0, 2);
        Triangle test_triangle_construction(vertex0, vertex1, vertex2, 0);
        auto edges = test_triangle_construction.getEdges();

        auto eps = std::numeric_limits<double>::epsilon();
        REQUIRE(abs(edges[0].edgeLength() - sqrt(2)) <= eps);
        REQUIRE(abs(edges[1].edgeLength() - 1.0) <= eps);
        REQUIRE(abs(edges[2].edgeLength() - 1.0) <= eps);
    }

    SECTION("Triangles as vertex index triples")
    {
        //TODO: add index collisions
        Vertex vertex0(0.0, 0.0, 0.0, 3);
        Vertex vertex1(0.0, 0.0, 0.0, 7);
        Vertex vertex2(0.0, 0.0, 0.0, 19);
        Triangle test_triangle(vertex0, vertex1, vertex2);

        std::vector<int> expected_triple = {3,7,19};
        auto result_triple = test_triangle.triangle_as_index_triple();
        REQUIRE(expected_triple == result_triple);

        vertex0.setIndex(1);
        vertex1.setIndex(2);
        vertex2.setIndex(3);

        // this fails, not sure if persistance is required.
        std::vector<int> expected_triple_after_update = {1,2,3};
        auto result_updated_triple = test_triangle.triangle_as_index_triple();
        CHECK(expected_triple_after_update == result_updated_triple);
    }

    SECTION("Angle calculation tests for right triangle.")
    {
        Vertex pVert0, pVert1(1.0, 0.0, 0.0, 1), pVert2(0.0, 1.0, 0.0, 2);
        Triangle test_triangle(pVert0, pVert1, pVert2);

        CHECK(abs(test_triangle.getAngleByVertexIndex(0) - M_PI/2) <= std::numeric_limits<double>::epsilon());
        CHECK(abs(test_triangle.getAngleByVertexIndex(1) - M_PI/4) <= std::numeric_limits<double>::epsilon());
        CHECK(abs(test_triangle.getAngleByVertexIndex(2) - M_PI/4) <= std::numeric_limits<double>::epsilon());
    }

    SECTION("Sum of angles within a triangle should equal pi")
    {
        Vertex vertex0(15.2, 17.3, 12.1, 0),
        vertex1(-24.17, 18.16, -7.56, 1),
        vertex2(102.15, -103.5, 0.0, 2);
        Triangle test_triangle(vertex0, vertex1, vertex2);
        auto angle0 = test_triangle.getAngleByVertexIndex(0);
        auto angle1 = test_triangle.getAngleByVertexIndex(1);
        auto angle2 = test_triangle.getAngleByVertexIndex(2);
        REQUIRE(abs((angle0 + angle1 + angle2) - M_PI) <= std::numeric_limits<double>::epsilon());
    }

}

TEST_CASE("Complex tests")
{
    auto test_vertex1 = Vertex();
    auto test_vertex2 = Vertex(1.0, 1.0, 0.0, 1);
    auto test_vertex3 = Vertex(1.0, 0.0, 0.0, 2);

    std::vector<Vertex> vertices = {test_vertex1, test_vertex2, test_vertex3};
    std::vector<std::vector<int>> faces = {{0,1,2}};
    Complex complex(vertices, faces);

    SECTION("Complex adjacency matrix construction")
    {
        std::vector<rowColumnValue_t> expectedEdgeVertexMatrix = {
                {0,1,1}, {0,2,1},
                {1,0,1}, {1,2,1},
                {2,0,1}, {2,1,1}
        };
        std::vector<rowColumnValue_t> expectedFaceEdgeMatrix = {
                {0,0,1}, {0,1,1}, {0,2,1}
        };

        auto returnedEdgeVertexMatrix = complex.getEdgeVertexAdjacencyMatrix();
        auto returnedFaceEdgeMatrix = complex.getFaceEdgeAdjacencyMatrix();


        REQUIRE(returnedEdgeVertexMatrix.getData() == expectedEdgeVertexMatrix);
        REQUIRE(returnedFaceEdgeMatrix.getData() == expectedFaceEdgeMatrix);
    }

    SECTION("branchThatContains tests")
    {
        REQUIRE(complex.branchThatContains(0, 1) == 2);
        REQUIRE(complex.branchThatContains(0, 3) == -1);
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
    SECTION("Dense to sparse conversion.")
    {
        DenseMatrix testDenseMatrix({{1,0,0}, {0,1,0}, {0,0,1}});
        auto returned_matrix = SparseMatrix(testDenseMatrix);
        std::vector<rowColumnValue_t> expected_matrix = {
                {0,0,1},{1,1,1},{2,2,1}
        };

        REQUIRE(returned_matrix.getData() == expected_matrix);
    }

    SECTION("Accessing elements throws invalid argument if index out of bounds returns element otherwise.")
    {
        DenseMatrix testMatrix({{1,2,3,4}, {0,5,6,7}, {0,0,0,0}});
        SparseMatrix sparseTestMatrix(testMatrix);

        REQUIRE(sparseTestMatrix(1,3) == 7);
        REQUIRE(sparseTestMatrix(1,0) == 0);
        REQUIRE(sparseTestMatrix(2,2) == 0);
        REQUIRE(sparseTestMatrix(0,2) == 3);

        REQUIRE_THROWS_AS(sparseTestMatrix(3,1), std::invalid_argument);
        REQUIRE_THROWS_AS(sparseTestMatrix(5,-2), std::invalid_argument);
    }

    SECTION("getColumnWithinRow and getRowWithinColumn tests")
    {
        DenseMatrix testMatrix({{1,2,3,4}, {0,5,6,7}, {0,0,0,0}, {2,2,5,0}, {1,0,1,1}});
        SparseMatrix sparseTestMatrix(testMatrix);

        std::vector<int> expected_first_row = {0, 1, 2, 3};
        std::vector<int> expected_second_column = {0, 1, 3};
        std::vector<int> expected_third_column = {0, 1, 3, 4};

        auto result_first_row = sparseTestMatrix.getColumnIndicesWithinRow(0);
        auto result_second_column = sparseTestMatrix.getRowIndicesWithinColumn(1);
        auto result_third_column = sparseTestMatrix.getRowIndicesWithinColumn(2);

        REQUIRE(expected_first_row == result_first_row);
        REQUIRE(expected_second_column == result_second_column);
        REQUIRE(expected_third_column == result_third_column);
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

    std::vector<indexPair_t> test_triangle_simplicies = {{0,1},{1,2},{2,0}};
    std::vector<indexPair_t> test_square_simplices = {
            {0,1},{1,2},{2,3},{3,0}
    };

    std::vector<std::vector<int>> test_triangle_face_simplices = {{0,1,2}};
    std::vector<std::vector<int>> test_square_face_simplices = {{0,1,2,3}};

    SECTION("A0 for test_triangle")
    {
        std::vector<rowColumnValue_t> expected_matrix = {
                {0,0,1}, {0,1,1},
                {1,1,1}, {1,2,1},
                {2,0,1}, {2,2,1}
        };

        auto returned_matrix = buildVertexEdgeAdjacencyMatrix(test_triangle_indices, test_triangle_simplicies);

        REQUIRE(returned_matrix.getData() == expected_matrix);
    }

    SECTION("A0 for test_square")
    {
        std::vector<rowColumnValue_t> expected_matrix = {
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
        std::vector<rowColumnValue_t> expected_matrix = {
                {0,0,1},{0,1,1}, {0,2,1}
        };
        auto returned_matrix = buildEdgeFaceAdjacencyMatrix(test_triangle_indices,test_triangle_face_simplices);

        REQUIRE(returned_matrix.getData() == expected_matrix);
    }

    SECTION("A1 for test_square")
    {
        std::vector<rowColumnValue_t> expected_matrix = {
                {0,0,1}, {0,1,1}, {0,2,1}, {0,3,1}
        };
        auto returned_matrix = buildEdgeFaceAdjacencyMatrix(test_square_indices, test_square_face_simplices);

        REQUIRE(returned_matrix.getData() == expected_matrix);
    }

    SECTION("A1 for square with diagonal")
    {
        auto indices = assignElementIndices(5,8,4);
        std::vector<std::vector<int>> simplices = {{4,5,0}, {5,6,1}, {6,7,2}, {7,4,3}};

        auto returned_matrix = buildEdgeFaceAdjacencyMatrix(indices, simplices);
        std::vector<rowColumnValue_t> expected_matrix = {
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

    int rows = 25;
    int columns = 50;

    std::unique_ptr<geometrycentral::surface::SurfaceMesh> mesh;
    std::unique_ptr<geometrycentral::surface::VertexPositionGeometry> geometry;

    SECTION("Visualize basic triangles")
    {
        polyscope::init();

        Vertex vertex0, vertex1(50.0, 0.0, 0.0, 1), vertex2(0.0, 0.0, 50.0, 2);
        Vertex vertex3(0.0, 50.0, 0.0, 3);
        std::vector<Vertex> vertices = {vertex0, vertex1, vertex2, vertex3};
        std::vector<std::vector<int>> indices = {{0,1,2}, {0,1,3}, {0,2,3}, {1,2,3}};
        Complex complex(vertices, indices);

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


