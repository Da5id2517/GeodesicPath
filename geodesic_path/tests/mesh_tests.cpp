#include "gtest/gtest.h"

#include "geodesic_path/mesh.h"

namespace gp {

TEST(Mesh, TriangleArea) {
  const std::vector<Point> vertices{{0, 0, 0}, {5, 0, 0}, {0, 4, 0}};
  const std::vector<Triangle> right_triangle{{0, 1, 2}};
  const Mesh mesh(vertices, right_triangle);
  const double right_triangle_area = mesh.Area();

  EXPECT_EQ(right_triangle_area, 10);
}

TEST(Mesh, AdjacentReturnsEmptyForIsolated) {
  const Mesh mesh({{0, 0, 0}}, {});
  auto result = mesh.Adjacent(0);
  EXPECT_EQ(result, std::vector<int>());
}

TEST(Mesh, Adjacent) {
  const std::vector<Point> vertices{
      {0, 0, 0}, {5, 0, 0}, {5, 12, 0}, {0, 4, 0}};
  const std::vector<Triangle> triangles{{0, 1, 2}, {0, 3, 2}};
  const Mesh mesh(vertices, triangles);

  auto adjacent = mesh.Adjacent(0);
  std::vector<int> expected{1, 2, 3};
  EXPECT_EQ(expected, adjacent);

  adjacent = mesh.Adjacent(3);
  expected = {0, 2};
  EXPECT_EQ(adjacent, expected);
}

TEST(Mesh, ShortestPath) {
  const std::vector<Point> vertices{
      {0, 0, 0}, {5, 0, 0}, {5, 12, 0}, {0, 4, 0}};
  const std::vector<Triangle> triangles{{0, 1, 2}, {0, 3, 2}};
  const Mesh mesh(vertices, triangles);

  std::vector<int> expected{0};
  auto result = mesh.ShortestPath(0, 0);
  EXPECT_EQ(expected, result);

  expected = {0, 2};
  result = mesh.ShortestPath(0, 2);
  EXPECT_EQ(expected, result);

  expected = {1, 0, 3};
  result = mesh.ShortestPath(1, 3);
  EXPECT_EQ(expected, result);
}

TEST(Mesh, Wedge) {
  const std::vector<Point> vertices{{0, 0, 0},   {10, 0, 0},  {15, 5, 0},
                                    {15, 15, 0}, {10, 20, 0}, {0, 20, 0},
                                    {-5, 15, 0}, {-5, 5, 0},  {5, 10, 0}};
  const std::vector<Triangle> triangles{{0, 1, 8}, {1, 2, 8}, {2, 3, 8},
                                        {3, 4, 8}, {5, 6, 8}, {7, 0, 8}};
  const auto size = static_cast<int>(triangles.size());
  const Mesh mesh(vertices, triangles);

  auto result = mesh.Wedge(8);
  const auto result_size = static_cast<int>(result.size());

  EXPECT_EQ(result_size, size);

  result = mesh.Wedge({0, 1, 2});
  const std::vector<int> expected{0, 1};

  EXPECT_EQ(result, expected);
}

} // namespace gp