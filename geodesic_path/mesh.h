#ifndef GEODESIC_PATH_MESH_H
#define GEODESIC_PATH_MESH_H

#include <vector>

#include "point.h"

namespace gp {
namespace {
struct Triangle {
  Point a;
  Point b;
  Point c;
};

} // namespace

class Mesh {
public:
  Mesh();
  explicit Mesh(std::vector<Point> &vertices, std::vector<Triangle> &faces);

  const std::vector<Point> &Vertices() const { return vertices_; }
  const std::vector<Triangle> &Triangles() const { return triangles_; }

  double Area() const;

private:
  std::vector<Point> vertices_{};
  std::vector<Triangle> triangles_{};
};

inline double Area(const Triangle &triangle) {
  const Point a_b{ToVector(triangle.a, triangle.b)};
  const Point a_c{ToVector(triangle.a, triangle.c)};
  return std::sqrt(SquaredLength(CrossProduct(a_b, a_c))) / 2;
}

} // namespace gp

#endif // GEODESIC_PATH_MESH_H
