#include "gtest/gtest.h"

#include "geodesic_path/mesh.h"

namespace gp {

TEST(Mesh, Constructor) {
  const Point p{0, 0, 0};
  const Point q{5, 0, 0};
  const Point r{0, 5, 0};
  const Point s{0, 0, 5};
  std::vector<Point> vertices{p, q, r, s};
  std::vector<Triangle> triangles{{p, q, r}, {q, r, s}};
  std::vector<Edge> expected_edges{
      {&p, &q, 0}, {&q, &r, 1}, {&r, &p, 2}, {&r, &s, 3}, {&s, &q, 4}};
  const Mesh created(vertices, triangles, expected_edges);
  EXPECT_EQ(expected_edges, created.Edges());
}

TEST(Mesh, TriangleArea) {
  const Triangle right_triangle{{0, 0, 0}, {5, 0, 0}, {0, 4, 0}};
  const double right_triangle_area = Area(right_triangle);

  EXPECT_EQ(right_triangle_area, 10);
}

//     REQUIRE_NOTHROW(std::tie(mesh, geometry) =
//                         geometrycentral::surface::readSurfaceMesh("test.obj"));
//     polyscope::registerSurfaceMesh("Basic Complex visualization test",
//                                    geometry->inputVertexPositions,
//                                    mesh->getFaceVertexList());

//     polyscope::show();
//   }

//   output.close();
} // namespace gp