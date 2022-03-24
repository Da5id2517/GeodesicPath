#ifndef GEODESIC_PATH_MESH_H
#define GEODESIC_PATH_MESH_H

#include <vector>

#include "point.h"

namespace gp {

namespace {
struct Triangle {
  int a;
  int b;
  int c;
};

struct Edge {
  const Point *start{nullptr};
  const Point *end{nullptr};
  int index;
};

inline bool operator==(const Edge &a, const Edge &b) {
  return *a.start == *b.start && *a.end == *b.end && a.index == b.index;
}

inline bool operator!=(const Edge &a, const Edge &b) { return !(a == b); }

} // namespace

class Mesh {
public:
  Mesh();
  explicit Mesh(std::vector<Point> vertices, std::vector<Triangle> triangles,
                std::vector<Edge> edges);

  const std::vector<Point> &Vertices() const { return vertices_; }
  const std::vector<Triangle> &Triangles() const { return triangles_; }
  const std::vector<Edge> &Edges() const { return edges_; }

  double Area() const;
  std::vector<Point> ShortestPath(const Point &a, const Point &b) const;

private:
  std::vector<Point> vertices_{};
  std::vector<Triangle> triangles_{};
  std::vector<Edge> edges_{};
};

inline double Area(const Triangle &triangle) {
  const Point a_b{ToVector(vertices_[triangle.a], vertices_[triangle.b])};
  const Point a_c{ToVector(vertices_[triangle.a], vertices_[triangle.c])};
  return std::sqrt(SquaredLength(CrossProduct(a_b, a_c))) / 2;
}

} // namespace gp

#endif // GEODESIC_PATH_MESH_H
