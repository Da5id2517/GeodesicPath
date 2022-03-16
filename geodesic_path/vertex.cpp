#include "vertex.h"

namespace gp {

Vertex::Vertex(double x, double y, double z) : point_({x, y, z}) {}

int Vertex::Index() const { return index_; }

} // namespace gp
