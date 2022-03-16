#ifndef RSPROJECT_EDGE_H
#define RSPROJECT_EDGE_H
#include <vector>

#include "vertex.h"

namespace gp {

class Edge {
public:
  Edge(Vertex *first, Vertex *second, int index = 0)
      : first{first}, second{second}, index{index} {}

  Vertex *First() const;
  Vertex *Second() const;
  int Index() const;
  int StartIndex() const;
  int EndIndex() const;

private:
  Vertex *first{nullptr};
  Vertex *second{nullptr};
  int index;
};

inline bool operator==(const Edge &lhs, const Edge &rhs) {
  return lhs.First() == rhs.First() && lhs.Second() == rhs.Second() &&
         rhs.Index() == lhs.Index();
}

inline bool operator!=(const Edge &lhs, const Edge &rhs) {
  return !(lhs == rhs);
}

} // namespace gp

#endif // RSPROJECT_EDGE_H
