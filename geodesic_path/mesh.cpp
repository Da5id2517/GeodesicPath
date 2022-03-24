#include <algorithm>
#include <limits>
#include <map>

#include "mesh.h"

namespace gp {
Mesh::Mesh(std::vector<Point> vertices, std::vector<Triangle> triangles,
           std::vector<Edge> edges)
    : vertices_(std::move(vertices)), triangles_(std::move(triangles)),
      edges_(std::move(edges)) {}

double Mesh::Area(const Triangle &triangle) const {
  const Point a_b{ToVector(vertices_[triangle.a], vertices_[triangle.b])};
  const Point a_c{ToVector(vertices_[triangle.a], vertices_[triangle.c])};
  return std::sqrt(SquaredLength(CrossProduct(a_b, a_c))) / 2;
}

double Mesh::Area() const {
  double area = 0.0;
  for (const auto &triangle : triangles_) {
    area += Area(triangle);
  }
  return area;
}

int Mesh::VertexIndex(const Point &a) const {
  return std::find(vertices_.begin(), vertices_.end(), a) - vertices_.begin();
}

std::optional<Pair> Mesh::TriangleContainsPoint(int triangle_index,
                                                int point_index) const {
  const auto triangle = triangles_[triangle_index];
  if (triangle.a == point_index) {
    return Pair{triangle.b, triangle.c};
  }
  if (triangle.b == point_index) {
    return Pair{triangle.a, triangle.c};
  }
  if (triangle.c == point_index) {
    return Pair{triangle.a, triangle.b};
  }
  return std::nullopt;
}

double Mesh::Distance(int i, int j) const {
  return std::sqrt(SquaredLength(ToVector(vertices_[i], vertices_[j])));
}

std::vector<int> Mesh::ShortestPath(const Point &a, const Point &b) const {
  const auto vertices_size = static_cast<int>(vertices_.size());
  const auto triangles_size = static_cast<int>(triangles_.size());
  double max = std::numeric_limits<double>::max();
  int a_index = VertexIndex(a);
  int b_index = VertexIndex(b);
  std::vector<int> visited{a_index};
  std::map<int, double> weight{};
  for (int i = 0; i < vertices_size; ++i) {
    weight.insert(std::make_pair(i, max));
  }
  for (int i = 0; i < triangles_size; ++i) {
    std::vector<int> adjacent_indices{};
    std::optional<Pair> adjacent_in_triangle =
        TriangleContainsPoint(i, a_index);
    if (adjacent_in_triangle) {
      if (std::find(visited.begin(), visited.end(),
                    adjacent_in_triangle->first) == visited.end()) {
        visited.push_back(adjacent_in_triangle->first);
        double distance = Distance(a_index, adjacent_in_triangle->first);
        weight[adjacent_in_triangle->first] =
            std::min(distance, weight[adjacent_in_triangle->first]);
      }
      if (std::find(visited.begin(), visited.end(),
                    adjacent_in_triangle->second) == visited.end()) {
        visited.push_back(adjacent_in_triangle->second);
        double distance = Distance(a_index, adjacent_in_triangle->second);
        weight[adjacent_in_triangle->second] =
            std::min(distance, weight[adjacent_in_triangle->second]);
      }
    }
  }
  visited.push_back(b_index);
  return visited;
}

} // namespace gp