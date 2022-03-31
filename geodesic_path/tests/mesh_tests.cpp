#include "gtest/gtest.h"

#include "geodesic_path/mesh.h"

namespace gp {

TEST(Mesh, TriangleArea) {
  std::vector<Point> vertices{{0, 0, 0}, {5, 0, 0}, {0, 4, 0}};
  std::vector<Triangle> right_triangle{{0, 1, 2}};
  const Mesh mesh(vertices, right_triangle);
  const double right_triangle_area = mesh.Area();

  EXPECT_EQ(right_triangle_area, 10);
}

TEST(Mesh, Adjacent) {
  const Point isolated{12, 12, 12};
  std::vector<Point> vertices{{0, 0, 0}, {5, 0, 0}, {5, 12, 0}, {0, 4, 0}};
  vertices.push_back(isolated);
  std::vector<Triangle> triangles{{0, 1, 2}, {0, 3, 2}};
  Mesh mesh(vertices, triangles);

  auto adjacent = mesh.Adjacent(0);
  std::vector<int> expected{1, 2, 3};
  EXPECT_EQ(expected, adjacent);

  adjacent = mesh.Adjacent(3);
  expected = {0, 2};
  EXPECT_EQ(adjacent, expected);

  adjacent = mesh.Adjacent(4);
  EXPECT_EQ(adjacent, std::vector<int>());
}

TEST(Mesh, ShortestPath) {
  std::vector<Point> vertices{{0, 0, 0}, {5, 0, 0}, {5, 12, 0}, {0, 4, 0}};
  std::vector<Triangle> triangles{{0, 1, 2}, {0, 3, 2}};
  Mesh mesh(vertices, triangles);

  std::vector<int> expected{0};
  auto result = mesh.ShortestPath(0, 0);
  EXPECT_EQ(expected, result);

  expected = {0, 2};
  result = mesh.ShortestPath(0, 2);
  EXPECT_EQ(expected, result);
}

// TODO: fix ShortestPaths!

} // namespace gp