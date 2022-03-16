#include "edge.h"

namespace gp {
int Edge::Index() const { return index; }
Vertex *Edge::First() const { return first; }
Vertex *Edge::Second() const { return second; }
} // namespace gp