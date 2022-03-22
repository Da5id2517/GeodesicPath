#include "gtest/gtest.h"

#include "geodesic_path/mesh.h"

namespace gp {

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