#ifndef GEODESIC_PATH_MESH_H
#define GEODESIC_PATH_MESH_H

#include <optional>
#include <vector>

#include "point.h"

namespace gp {

struct Triangle {
  int a;
  int b;
  int c;
};

struct Pair {
  int first;
  int second;
};

class Mesh {
public:
  Mesh();
  explicit Mesh(std::vector<Point> vertices, std::vector<Triangle> triangles);

  const std::vector<Point> &Vertices() const { return vertices_; }
  const std::vector<Triangle> &Triangles() const { return triangles_; }

  double Area(const Triangle &triangle) const;
  double Area() const;
  double Distance(int i, int j) const;

  int VertexIndex(const Point &a) const;
  std::vector<int> Adjacent(int point_index) const;
  std::optional<Pair> AdjacentInTriangle(int triangle_index,
                                         int point_index) const;
  std::vector<int> ShortestPath(int start, int end) const;

private:
  std::vector<Point> vertices_{};
  std::vector<Triangle> triangles_{};
};

} // namespace gp

#endif // GEODESIC_PATH_MESH_H
