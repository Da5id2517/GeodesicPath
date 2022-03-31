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

} // namespace gp