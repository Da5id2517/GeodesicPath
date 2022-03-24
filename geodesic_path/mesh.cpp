#include <algorithm>
#include <map>

#include "mesh.h"

namespace gp {
Mesh::Mesh(std::vector<Point> vertices, std::vector<Triangle> triangles,
           std::vector<Edge> edges)
    : vertices_(std::move(vertices)), triangles_(std::move(triangles)),
      edges_(std::move(edges)) {}

double Mesh::Area() const {
  double area = 0.0;
  for (const auto &triangle : triangles_) {
    area += gp::Area(triangle);
  }
  return area;
}

std::vector<Point> Mesh::ShortestPath(const Point &a, const Point &b) const {
  const auto vertices_size = static_cast<int>(vertices_.size());
  std::vector<Point> path{a};
  std::map<Point, double> index{};
  for (int i = 0; i < vertices_size; ++i) {
    index.insert(std::make_pair(vertices_[i], 0));
  }

  path.push_back(b);
  return path;
}

} // namespace gp