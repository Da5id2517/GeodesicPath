#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include <vector>
#include "../mesh.h"
#include "../utils.h"
#include "polyscope/polyscope.h"
#include "polyscope/surface_mesh.h"
#include "geometrycentral/surface/meshio.h"
#include "geometrycentral/surface/surface_mesh.h"



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

    //TODO find a cleaner solution for this.
    const std::vector<std::tuple<int,int>> test_triangle_simplicies = {
            std::make_tuple(0,1),
            std::make_tuple(1,2),
            std::make_tuple(2,0)
    };

    const std::vector<std::tuple<int, int>> test_square_simplices = {
            std::make_tuple(0,1),
            std::make_tuple(1,2),
            std::make_tuple(2,3),
            std::make_tuple(3,0)
    };

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
                REQUIRE(returned_matrix[i][j] == expected_matrix[i][j]);
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
                REQUIRE(returned_matrix[i][j] == expected_matrix[i][j]);
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


