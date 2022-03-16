#ifndef RSPROJECT_VERTEX_H
#define RSPROJECT_VERTEX_H

#include <vector>

#include "point.h"

namespace gp {

class Vertex {
public:
  Vertex(double x, double y, double z);

  Point Point() const;
  int Index() const;

private:
  struct Point point_ {};
  int index_{0};
  std::vector<int> face_indices{};
};

inline bool operator==(const Vertex &lhs, const Vertex &rhs) {
  return lhs.Point() == rhs.Point() && lhs.Index() == rhs.Index();
}

inline bool operator!=(const Vertex &lhs, const Vertex &rhs) {
  return !(lhs == rhs);
}

} // namespace gp
#endif // RSPROJECT_VERTEX_H
