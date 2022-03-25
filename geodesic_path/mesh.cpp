#include <algorithm>

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

std::optional<Pair> Mesh::AdjacentInTriangle(int triangle_index,
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

std::vector<int> Mesh::Adjacent(int point_index) const {
  std::vector<int> result{};
  const auto size = static_cast<int>(triangles_.size());
  for (int i = 0; i < size; ++i) {
    std::optional<Pair> adjacent = AdjacentInTriangle(i, point_index);
    if (adjacent && (std::find(result.begin(), result.end(), adjacent->first) ==
                     result.end())) {
      result.push_back(adjacent->first);
    }
    if (adjacent && (std::find(result.begin(), result.end(),
                               adjacent->second) == result.end())) {
      result.push_back(adjacent->second);
    }
  }
  return result;
}

void Mesh::ShortestPathsLocally(std::map<int, double> &weight,
                                std::map<int, int> &route,
                                std::vector<int> &visited, int current) const {
  const auto size = static_cast<int>(vertices_.size());
  if (static_cast<int>(visited.size()) == size ||
      std::find(visited.begin(), visited.end(), current) != visited.end()) {
    return;
  }
  auto adjacent = Adjacent(current);
  for (const auto adj_index : adjacent) {
    const auto distance = Distance(current, adj_index);
    if (weight[current] + distance < weight[adj_index]) {
      weight[adj_index] = weight[current] + distance;
      route[adj_index] = current;
    }
  }
  visited.push_back(current);
  auto next = std::min_element(
      weight.begin(), weight.end(),
      [](std::pair<const int, double> lhs, std::pair<const int, double> rhs) {
        return lhs.second < rhs.second ? lhs.first : rhs.first;
      });
  ShortestPathsLocally(weight, route, visited, next->first);
}

std::vector<int> Mesh::ShortestPaths(int start, int end) const {
  std::vector<int> path{};
  const auto size = static_cast<int>(vertices_.size());
  std::vector<int> visited;
  visited.reserve(size);
  std::map<int, double> weight{};
  std::map<int, int> route{};
  weight[0] = 0;
  route[0] = -1;
  for (int i = 1; i < size; ++i) {
    weight[i] = std::numeric_limits<double>::max();
    route[i] = -1;
  }
  int current = 0;
  ShortestPathsLocally(weight, route, visited, current);
  current = end;
  while (route[current] != start) {
    current = route[current];
    path.push_back(current);
  }
  path.push_back(start);
  std::reverse(path.begin(), path.end());
  return path;
}

} // namespace gp