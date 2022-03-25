#ifndef GEODESIC_PATH_MESH_H
#define GEODESIC_PATH_MESH_H

#include <map>
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

struct Edge {
  const Point *start{nullptr};
  const Point *end{nullptr};
  int index;
};

inline bool operator==(const Edge &a, const Edge &b) {
  return *a.start == *b.start && *a.end == *b.end && a.index == b.index;
}

inline bool operator!=(const Edge &a, const Edge &b) { return !(a == b); }

class Mesh {
public:
  Mesh();
  explicit Mesh(std::vector<Point> vertices, std::vector<Triangle> triangles,
                std::vector<Edge> edges);

  const std::vector<Point> &Vertices() const { return vertices_; }
  const std::vector<Triangle> &Triangles() const { return triangles_; }
  const std::vector<Edge> &Edges() const { return edges_; }

  double Area(const Triangle &triangle) const;
  double Area() const;
  double Distance(int i, int j) const;

  int VertexIndex(const Point &a) const;
  std::vector<int> Adjacent(int point_index) const;
  std::optional<Pair> AdjacentInTriangle(int triangle_index,
                                         int point_index) const;
  std::vector<int> ShortestPaths(int start, int end) const;
  void ShortestPathsLocally(std::map<int, double> &weight,
                            std::map<int, int> &route,
                            std::vector<int> &visited, int current) const;

private:
  std::vector<Point> vertices_{};
  std::vector<Triangle> triangles_{};
  std::vector<Edge> edges_{};
};

} // namespace gp

#endif // GEODESIC_PATH_MESH_H
