#include <algorithm>
#include <map>
#include <numeric>

#include "mesh.h"

namespace gp {

namespace {
struct Route {
  double weight;
  int previous;
};
} // namespace

Mesh::Mesh(std::vector<Point> vertices, std::vector<Triangle> triangles)
    : vertices_(std::move(vertices)), triangles_(std::move(triangles)) {}

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

std::vector<int> Mesh::Wedge(const int node) const {
  // assume nodes is a joint of three nodes for now.
  std::vector<int> result{};
  const auto size = static_cast<int>(triangles_.size());
  for (int i = 0; i < size; ++i) {
    const auto triangle = triangles_[i];
    if (triangle.a == node || triangle.b == node || triangle.c == node) {
      result.push_back(i);
    }
  }
  return result;
}

std::vector<int> Mesh::Wedge(const std::vector<int> &nodes) const {
  std::vector<int> result{};

  for (auto current = nodes.begin() + 1; current != nodes.end() - 1;
       ++current) {
    // TODO: i do not like the name tmp.
    const auto tmp = Wedge(*current);
    result.insert(result.end(), tmp.begin(), tmp.end());
  }

  return result;
}

std::vector<int> ReconstructPath(std::map<const int, Route> &routes, int start,
                                 int end) {
  std::vector<int> path{};
  int current = end;
  while (current != start) {
    path.push_back(current);
    current = routes[current].previous;
  }
  path.push_back(start);
  std::reverse(path.begin(), path.end());
  return path;
}

std::vector<int> Mesh::ShortestPath(int start, int end) const {
  const auto size = static_cast<int>(vertices_.size());
  std::vector<int> unvisited(size);
  std::iota(unvisited.begin(), unvisited.end(), 0);
  std::vector<int> visited;
  visited.reserve(size);
  std::map<const int, Route> routes;
  for (int i = 0; i < size; ++i) {
    if (i == start) {
      routes[i] = Route{.weight = 0, .previous = -1};
      continue;
    }
    routes[i] =
        Route{.weight = std::numeric_limits<double>::max(), .previous = -1};
  }

  auto compare = [](std::pair<const int, Route> &lhs,
                    std::pair<const int, Route> &rhs) {
    return lhs.second.weight < rhs.second.weight;
  };

  int current = start;
  while (!unvisited.empty()) {
    auto adjacent = Adjacent(current);
    for (const auto &neighbour : adjacent) {
      if (std::find(visited.begin(), visited.end(), neighbour) !=
          visited.end()) {
        continue;
      }
      auto new_weight = routes[current].weight + Distance(current, neighbour);
      if (new_weight < routes[neighbour].weight) {
        routes[neighbour] = Route{.weight = new_weight, .previous = current};
      }
    }
    visited.push_back(current);
    unvisited.erase(unvisited.begin() + current);
    current = (*std::min_element(routes.begin(), routes.end(), compare))
                  .second.weight;
  }

  return ReconstructPath(routes, start, end);
}

} // namespace gp