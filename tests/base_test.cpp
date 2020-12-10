#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include <vector>
#include "../mesh.h"
#include "../utils.h"
#include "../SparseMatrix.h"
#include "polyscope/polyscope.h"
#include "polyscope/surface_mesh.h"
#include "geometrycentral/surface/meshio.h"
#include "geometrycentral/surface/surface_mesh.h"



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

TEST_CASE("buildVertexEdgeAdjacencyMatrix tests")
{
    auto test_triangle_indices = assignElementIndices(3,3,1);
    auto test_square_indices = assignElementIndices(4,4,1);

    const std::vector<std::tuple<int,int>> test_triangle_simplicies = {{0,1},{1,2},{2,0}};
    const std::vector<std::tuple<int, int>> test_square_simplices = {{0,1},{1,2},{2,3},{3,0}};

    const std::vector<std::vector<int>> test_triangle_face_simplices = {{0,1,2}};
    const std::vector<std::vector<int>> test_square_face_simplices = {{0,1,2,3}};

    SECTION("A0 for test_triangle")
    {
        int expected_matrix[3][3] = {
                {1, 1, 0},
                {0, 1, 1},
                {1, 0, 1}
        };

        auto returned_matrix = buildVertexEdgeAdjacencyMatrix(test_triangle_indices, test_triangle_simplicies);

        for(int i = 0; i < 3; i++)
        {
            for(int j = 0; j < 3; j++)
            {
                REQUIRE(returned_matrix(i,j) == expected_matrix[i][j]);
            }
        }
    }

    SECTION("A0 for test_square")
    {
        int expected_matrix[4][4] = {
                {1, 1, 0, 0},
                {0, 1, 1, 0},
                {0, 0, 1, 1},
                {1, 0, 0, 1}
        };

        auto returned_matrix = buildVertexEdgeAdjacencyMatrix(test_square_indices, test_square_simplices);

        for(int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                REQUIRE(returned_matrix(i,j) == expected_matrix[i][j]);
            }
        }
    }

    SECTION("A1 for test_triangle")
    {
        int expected_matrix [1][3] = {{1, 1, 1}};
        auto returned_matrix = buildEdgeFaceAdjacencyMatrix(test_triangle_indices,test_triangle_face_simplices);

        for(int i = 0; i < 1; i++)
        {
            for(int j = 0; j < 3; j++)
            {
                REQUIRE(returned_matrix(i,j) == expected_matrix[i][j]);
            }
        }
    }

    SECTION("A1 for test_square")
    {
        int expected_matrix [1][4] = {{1,1,1,1}};
        auto returned_matrix = buildEdgeFaceAdjacencyMatrix(test_square_indices, test_square_face_simplices);

        for(int i = 0 ; i < 1; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                REQUIRE(returned_matrix(i,j) == expected_matrix[i][j]);
            }
        }
    }

}


TEST_CASE("Base functionality tests")
{

    std::ofstream output("test.obj");

    int rows = rand() % 10 + 2;
    int columns = rand() % 10 + 2;

    std::unique_ptr<geometrycentral::surface::SurfaceMesh> mesh;
    std::unique_ptr<geometrycentral::surface::VertexPositionGeometry> geometry;

    SECTION("Generate square of random dimensions, output an appropriate .obj file and visualize it")
    {
        polyscope::init();

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


